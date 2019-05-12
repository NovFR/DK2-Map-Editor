
//<<>-<>>---------------------------------------------------------------------()
/*
	Ajout, modification des déclencheurs
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"

extern HINSTANCE	hInstance;
extern NODE		MapTriggers;
extern NODE		MapAreas;
extern NODE		MapHerosBands;
extern LONG		MapChanges;
extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ajout ou Modification d'un déclencheur				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Triggers_TriggerNew(HWND hDlg, TRIGGERSCTX *Context)
{
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	int		 Count;
	int		 Result;

	//--- Si modification, copie les données précédentes ---

	if (Context->Modify)
		{
		TV_ITEM	Item;

		Item.mask = TVIF_PARAM;
		Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
		if (!Item.hItem) return;
		if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
		if (!Item.lParam) return;
		CopyMemory(&Context->TriggerCtx.TempTrigger,(void *)Item.lParam,sizeof(TRIGGER));
		Context->TriggerCtx.SrceTrigger = (TRIGGER *)Item.lParam;
		}
	else
		{
		Context->TriggerCtx.SrceTrigger = NULL;
		Context->TriggerCtx.TempTrigger.node.next = NULL;
		Context->TriggerCtx.TempTrigger.node.prev = NULL;
		Context->TriggerCtx.TempTrigger.node.type = TYPE_TRIGGER;
		Context->TriggerCtx.TempTrigger.or = 0;
		Context->TriggerCtx.TempTrigger.and = 0;
		if (!Context->TriggerCtx.TempTrigger.ctrl)
			{
			Context->TriggerCtx.TempTrigger.ctrl = TRIGGER_VARIABLE;
			Context->TriggerCtx.TempTrigger.param1 = 3;
			Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param3 = 1;
			Context->TriggerCtx.TempTrigger.param4 = 0;
			Context->TriggerCtx.TempTrigger.param5 = 0;
			Context->TriggerCtx.TempTrigger.param6 = 0;
			Context->TriggerCtx.TempTrigger.param7 = 0;
			Context->TriggerCtx.TempTrigger.param8 = 0;
			Context->TriggerCtx.TempTrigger.repeat = 0x01;
			}
		}

	for (Count = 1; Triggers[Count].ctrl != 0; Count++);

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*(Count+2));
	if (!psp) goto Error_1;

	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].hInstance = hInstance;
	psp[0].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(7100);
	psp[0].pfnDlgProc = Triggers_TriggerDlgList;
	psp[0].lParam = (LPARAM)Context;
	for (Result = 1; Result != Count+1; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(Triggers[Result-1].dialog);
		psp[Result].pfnDlgProc = Triggers_TriggerDlgParams;
		psp[Result].lParam = (LPARAM)Context;
		}
	psp[Result].dwSize = sizeof(PROPSHEETPAGE);
	psp[Result].hInstance = hInstance;
	psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(7199);
	psp[Result].pfnDlgProc = Triggers_TriggerDlgRepeat;
	psp[Result].lParam = (LPARAM)Context;

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_WIZARD;
	psh->hwndParent = hDlg;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = NULL;
	psh->nPages = Count+2;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	if (!Misc_CreateImageList(TrSpecial,&Context->Images,32,32,FALSE)) goto Error_2;
	if (!Misc_CreateImageList(BadCreatures,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(BadElites,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Heros,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Doors,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Traps,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Spells,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Terrain,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(TrPlayers,&Context->Images,32,32,TRUE)) goto Error_2;

	for (Result = 0; Result != Count; Result++)
		{
		Context->TriggerCtx.Initialized[Triggers[Result].ctrl] = FALSE;
		}

	Result = PropertySheet(psh);
	if (Result == -1) goto Error_2;

	Misc_DestroyImageList(&Context->Images);
	GlobalFree(psp);
	GlobalFree(psh);
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_2:Misc_DestroyImageList(&Context->Images);
	GlobalFree(psp);
Error_1:GlobalFree(psh);
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection du type de déclencheur					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_TriggerDlgList(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		Triggers_TriggerDlgInit(hDlg,Context);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Triggers_DisplayDrawList((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case LVN_ITEMCHANGED:
					if (((NM_LISTVIEW *)lParam)->uChanged&LVIF_STATE)
						{
						SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_DISABLEDFINISH);
						if (((NM_LISTVIEW *)lParam)->uNewState&LVIS_SELECTED)
							{
							lData = (LONG)Triggers_TriggerDlgGetParam(Context);
							if (!lData) return(TRUE);
							Triggers_TriggerDlgSetDefault((BYTE)((TRIGGERINFO *)lData)->ctrl,Context);
							Context->TriggerCtx.TempTrigger.ctrl = (BYTE)((TRIGGERINFO *)lData)->ctrl;
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
							}
						}
					return(TRUE);

                                case PSN_SETACTIVE:
					if (!Context->TriggerCtx.MainSet)
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
						}
					else
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,0);
						PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_DISABLEDFINISH);
						lData = (LONG)Triggers_TriggerDlgGetParam(Context);
						if (!lData) return(TRUE);
						PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
						}
					return(TRUE);

				case PSN_WIZNEXT:
					lData = (LONG)Triggers_TriggerDlgGetParam(Context);
					if (!lData)
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
					else if (((TRIGGERINFO *)lData)->dialog == -1)
						SetWindowLong(hDlg,DWL_MSGRESULT,7199);
					else
						SetWindowLong(hDlg,DWL_MSGRESULT,((TRIGGERINFO *)lData)->dialog);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void Triggers_TriggerDlgInit(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	int		 Selected;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Context->TriggerCtx.MainSet = FALSE;

	GetWindowRect(GetDlgItem(hDlg,101),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	Context->TriggerCtx.TriggersList = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->TriggerCtx.TriggersList) goto Error_0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->TriggerCtx.TriggersList,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) goto Error_0;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) goto Error_0;
	if (!Item.lParam) goto Error_0;
	Context->TriggerCtx.ParentPoint = (TRIGGERLINK *)Item.lParam;

	Item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE|LVIF_PARAM;
	Item.iSubItem = 0;
	Item.iImage = 6;
	for (Item.iItem = 0, Selected = 0; Triggers[Item.iItem].ctrl != 0; Item.iItem++)
		{
		Item.state = 0;
		Item.stateMask = LVIS_SELECTED;
		Item.pszText = Triggers[Item.iItem].text;
		if (Context->TriggerCtx.ParentPoint->node.type&Triggers[Item.iItem].scope) Item.lParam = Triggers[Item.iItem].ctrl;
		#if (!TRIGGERS_SHOWINCOMPATIBLETR)
		else continue;
		#else
		else Item.lParam = 0;
		#endif
		if (Triggers[Item.iItem].ctrl == Context->TriggerCtx.TempTrigger.ctrl) Item.state = LVIS_SELECTED;
		if (SendMessage(Context->TriggerCtx.TriggersList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
		if (Triggers[Item.iItem].ctrl == Context->TriggerCtx.TempTrigger.ctrl) Selected = Item.iItem;
		}
	SendMessage(Context->TriggerCtx.TriggersList,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);

	Context->TriggerCtx.MainSet = TRUE;
	return;

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Obtention des paramètres d'un déclencheur «««««««««««««««««««««««»

TRIGGERINFO* Triggers_TriggerDlgGetParam(TRIGGERSCTX *Context)
{
	LV_ITEM	Item;
	int	i;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->TriggerCtx.TriggersList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) return(NULL);
	if (!SendMessage(Context->TriggerCtx.TriggersList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(NULL);
	if (!Item.lParam) return(NULL);

	for (i = 0; Triggers[i].ctrl != 0; i++)
		{
		if (Triggers[i].ctrl != Item.lParam) continue;
		return(&Triggers[i]);
		}

	return(NULL);
}


// «»»» Place les valeurs par défaut (si nécessaire) ««««««««««««««««««««»

void Triggers_TriggerDlgSetDefault(BYTE NewCtrl, TRIGGERSCTX *Context)
{
	switch(NewCtrl)
		{
		case TRIGGER_VARIABLE:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 3;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_TIMER:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_TIMEELAPSED:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CREATURES:
		case TRIGGER_HAPPYCREATURES:
		case TRIGGER_ANGRYCREATURES:
		case TRIGGER_LOSTCREATURES:
		case TRIGGER_KILLEDCREATURES:
		case TRIGGER_ROOMS:
		case TRIGGER_ROOMSTYPES:
		case TRIGGER_ROOMSSIZE:
		case TRIGGER_ROOMSCONTENT:
		case TRIGGER_GOLDMINED:
		case TRIGGER_GOLDAMOUNT:
		case TRIGGER_MANAAMOUNT:
		case TRIGGER_SLAPCOUNT:
		case TRIGGER_GROUPINPOSSESS:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 4;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CREATURESLEVEL:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 4;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 4;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CREATUREEXP:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 1;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_DOORS:
		case TRIGGER_TRAPS:
		case TRIGGER_DESTROYED:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 4;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_DEATHSINBAND:
		case TRIGGER_JAILEDINBAND:
		case TRIGGER_STUNNEDINBAND:
		case TRIGGER_HURTED:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 4;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_SPELLS:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 0;
				Context->TriggerCtx.TempTrigger.param2 = 6;
				Context->TriggerCtx.TempTrigger.param3 = 1;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CLICKELEMENT:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 1;
				Context->TriggerCtx.TempTrigger.param2 = 2;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_HEARTDESTROYED:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 0;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_ENTERAREAINPOSS:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 0;
				Context->TriggerCtx.TempTrigger.param2 = 3;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CREAENTERAREA:
		case TRIGGER_CLAIMAREA:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 3;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 1;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_CLAIMENTIREAREA:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 3;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_AREACONTENT:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 2;
				Context->TriggerCtx.TempTrigger.param3 = 3;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 1;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_SELECTAREA:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 3;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 1;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_SELECTALLAREA:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 5;
				Context->TriggerCtx.TempTrigger.param2 = 3;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;

		case TRIGGER_BANDCREATED:
			if (Context->TriggerCtx.TempTrigger.ctrl != NewCtrl)
				{
				Context->TriggerCtx.TempTrigger.param1 = 3;
				Context->TriggerCtx.TempTrigger.param2 = 0;
				Context->TriggerCtx.TempTrigger.param3 = 0;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				Context->TriggerCtx.TempTrigger.param6 = 0;
				Context->TriggerCtx.TempTrigger.param7 = 0;
				Context->TriggerCtx.TempTrigger.param8 = 0;
				}
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres du déclencheur					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Triggers_TriggerDlgParams(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		Triggers_TriggerDlgParamsInit(hDlg,Context);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_MeasurePlayerComboBoxList(501,(MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_DisplayPlayerComboBoxList(501,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Triggers_DisplayDrawList((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 206:
							if (Context->TriggerCtx.TempTrigger.ctrl != TRIGGER_VARIABLE) break;
							if (IsDlgButtonChecked(hDlg,206) == BST_CHECKED) break;
							CheckDlgButton(hDlg,206,BST_CHECKED);
							CheckDlgButton(hDlg,207,BST_UNCHECKED);
							EnableWindow(GetDlgItem(hDlg,210),TRUE);
							EnableWindow(GetDlgItem(hDlg,209),FALSE);
							return(TRUE);
						case 207:
							if (Context->TriggerCtx.TempTrigger.ctrl != TRIGGER_VARIABLE) break;
							if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED) break;
							CheckDlgButton(hDlg,206,BST_UNCHECKED);
							CheckDlgButton(hDlg,207,BST_CHECKED);
							EnableWindow(GetDlgItem(hDlg,209),TRUE);
							EnableWindow(GetDlgItem(hDlg,210),FALSE);
							return(TRUE);
						}
					break;

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 700:
						case 701:
						case 702:
							if (Context->TriggerCtx.TempTrigger.ctrl == TRIGGER_TIMEELAPSED)
								{
								long	Hours;
								long	Minutes;
								long	Seconds;

								Hours = GetDlgItemInt(hDlg,700,NULL,FALSE);
								Minutes = GetDlgItemInt(hDlg,701,NULL,FALSE);
								Seconds = GetDlgItemInt(hDlg,702,NULL,FALSE);
								if (Hours > 23)
									{
									SetDlgItemInt(hDlg,700,23,FALSE);
									break;
									}
								if (Minutes > 59)
									{
									SetDlgItemInt(hDlg,701,59,FALSE);
									break;
									}
								if (Seconds > 59)
									{
									SetDlgItemInt(hDlg,702,59,FALSE);
									break;
									}
								SetDlgItemInt(hDlg,210,Hours*60*60+Minutes*60+Seconds,FALSE);
								}
							break;
						}
					return(TRUE);

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 212:
							lData = SendDlgItemMessage(hDlg,212,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							if (lData < 0) break;
							Context->TriggerCtx.TempTrigger.param2 = lData;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case LVN_ITEMCHANGED:
					switch(Context->TriggerCtx.TempTrigger.ctrl)
						{
						case TRIGGER_DEATHSINBAND:
							lData = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.DeadHerosList);
							if (!lData)
								{
								SetDlgItemText(hDlg,102,NULL);
								break;
								}
							lData = Heros_GetHerosCount(Heros_FindHerosById(lData-1));
							wsprintf(Context->TriggerCtx.TempNumber,szTriggersHeroesCount,lData);
							SetDlgItemText(hDlg,102,Context->TriggerCtx.TempNumber);
							break;
						case TRIGGER_JAILEDINBAND:
							lData = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.JailHerosList);
							if (!lData)
								{
								SetDlgItemText(hDlg,102,NULL);
								break;
								}
							lData = Heros_GetHerosCount(Heros_FindHerosById(lData-1));
							wsprintf(Context->TriggerCtx.TempNumber,szTriggersHeroesCount,lData);
							SetDlgItemText(hDlg,102,Context->TriggerCtx.TempNumber);
							break;
						case TRIGGER_STUNNEDINBAND:
							lData = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.StunHerosList);
							if (!lData)
								{
								SetDlgItemText(hDlg,102,NULL);
								break;
								}
							lData = Heros_GetHerosCount(Heros_FindHerosById(lData-1));
							wsprintf(Context->TriggerCtx.TempNumber,szTriggersHeroesCount,lData);
							SetDlgItemText(hDlg,102,Context->TriggerCtx.TempNumber);
							break;
						}

					if (!IsWindow(Context->TriggerCtx.AreaContentList)) break;
					if (((NMHDR *)lParam)->hwndFrom == Context->TriggerCtx.AreaContentList)
						{
						if (!(((NM_LISTVIEW *)lParam)->uNewState&LVIS_SELECTED))
							EnableWindow(GetDlgItem(hDlg,501),FALSE);
						else
							{
							lData = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaContentList);
							if ((lData <= 0x07)||(lData == 0x1F))
								{
								SendDlgItemMessage(hDlg,501,CB_SETCURSEL,(WPARAM)1,(LPARAM)0);
								EnableWindow(GetDlgItem(hDlg,501),FALSE);
								}
							else if (lData == 0x23)
								{
								SendDlgItemMessage(hDlg,501,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
								EnableWindow(GetDlgItem(hDlg,501),FALSE);
								}
							else
								EnableWindow(GetDlgItem(hDlg,501),TRUE);
							}
						}
					return(TRUE);

                                case PSN_SETACTIVE:
					Triggers_TriggerDlgParamsCheck(hDlg,Context);
					return(TRUE);

				case PSN_WIZBACK:
					Triggers_TriggerDlgParamsStore(hDlg,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,7100);
					return(TRUE);

				case PSN_WIZNEXT:
					if (!Triggers_TriggerDlgParamsStore(hDlg,Context))
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
					else
						SetWindowLong(hDlg,DWL_MSGRESULT,7199);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations des données fixes «««««««««««««««««««««««««««««««»

void Triggers_TriggerDlgParamsInit(HWND hDlg, TRIGGERSCTX *Context)
{
	MAPCLIENT	 Client;
	MAPRECTEX	*Area;
	char		*TempText;
	int		 i;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Context->TriggerCtx.TempTrigger.ctrl)
		{
		case TRIGGER_VARIABLE:
			TempText = GlobalAlloc(GPTR,64);
			if (!TempText) return;
			for (i = 0; i != TRIGGERS_MAXVARIABLES; i++)
				{
				if (TriggersVariables[i][0]) wsprintf(TempText,szText,&TriggersVariables[i][0]);
				else wsprintf(TempText,szNumber,i);
				if (i == TRIGGERS_MAXVARIABLES-1) strcpy(TempText,szTriggersScoreVar);
				if (SendDlgItemMessage(hDlg,212,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
					{
					GlobalFree(TempText);
					return;
					}
				if (SendDlgItemMessage(hDlg,209,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
					{
					GlobalFree(TempText);
					return;
					}
				}
			GlobalFree(TempText);
			Context->TriggerCtx.Initialized[TRIGGER_VARIABLE] = TRUE;
			break;

		case TRIGGER_TIMER:
			TempText = GlobalAlloc(GPTR,64);
			if (!TempText) return;
			for (i = 0; i != TRIGGERS_MAXTIMERS; i++)
				{
				if (TriggersTimers[i][0]) wsprintf(TempText,szText,&TriggersTimers[i][0]);
				else wsprintf(TempText,szNumber,i);
				if (i == TRIGGERS_MAXTIMERS-1) strcpy(TempText,szTriggersLimitTime);
				if (SendDlgItemMessage(hDlg,212,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
					{
					GlobalFree(TempText);
					return;
					}
				}
			GlobalFree(TempText);
			Context->TriggerCtx.Initialized[TRIGGER_TIMER] = TRUE;
			break;

		case TRIGGER_TIMEELAPSED:
			Context->TriggerCtx.Initialized[TRIGGER_TIMEELAPSED] = TRUE;
			break;

		case TRIGGER_CREATURES:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->TriggerCtx.CreaturesList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CREATURES] = TRUE;
			break;

		case TRIGGER_HAPPYCREATURES:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->TriggerCtx.HappyCreaturesList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_HAPPYCREATURES] = TRUE;
			break;

		case TRIGGER_ANGRYCREATURES:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->TriggerCtx.AngryCreaturesList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ANGRYCREATURES] = TRUE;
			break;

		case TRIGGER_LOSTCREATURES:
			if (!Triggers_DisplayCreatePlayersList(hDlg,403,&Context->TriggerCtx.LostPlayersList,Context->TriggerCtx.TempTrigger.param4,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_LOSTCREATURES] = TRUE;
			break;

		case TRIGGER_KILLEDCREATURES:
			if (!Triggers_DisplayCreatePlayersList(hDlg,403,&Context->TriggerCtx.KilledPlayersList,Context->TriggerCtx.TempTrigger.param4,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_KILLEDCREATURES] = TRUE;
			break;

		case TRIGGER_CREATURESLEVEL:
			if (!Misc_CreateLevelComboBoxList(hDlg,404,Context->TriggerCtx.TempTrigger.param4)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CREATURESLEVEL] = TRUE;
			break;

		case TRIGGER_CREATUREEXP:
			if (!Misc_CreateLevelComboBoxList(hDlg,404,Context->TriggerCtx.TempTrigger.param5)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CREATUREEXP] = TRUE;
			break;

		case TRIGGER_ROOMS:
			if (!Triggers_DisplayCreateRoomsList(hDlg,400,&Context->TriggerCtx.RoomsList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ROOMS] = TRUE;
			break;

		case TRIGGER_ROOMSTYPES:
			if (!Triggers_DisplayCreateRoomsList(hDlg,400,&Context->TriggerCtx.RoomsTypeList,Context->TriggerCtx.TempTrigger.param2,TRUE,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ROOMSTYPES] = TRUE;
			break;

		case TRIGGER_ROOMSSIZE:
			if (!Triggers_DisplayCreateRoomsList(hDlg,400,&Context->TriggerCtx.RoomsSizeList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ROOMSSIZE] = TRUE;
			break;

		case TRIGGER_ROOMSCONTENT:
			if (!Triggers_DisplayCreateRoomsList(hDlg,400,&Context->TriggerCtx.RoomsContentList,Context->TriggerCtx.TempTrigger.param2,TRUE,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ROOMSCONTENT] = TRUE;
			break;

		case TRIGGER_DOORS:
			if (!Triggers_DisplayCreateDoorsList(hDlg,400,&Context->TriggerCtx.DoorsList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_DOORS] = TRUE;
			break;

		case TRIGGER_TRAPS:
			if (!Triggers_DisplayCreateTrapsList(hDlg,400,&Context->TriggerCtx.TrapsList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_TRAPS] = TRUE;
			break;

		case TRIGGER_DESTROYED:
			if (!Triggers_DisplayCreateDoorsList(hDlg,400,&Context->TriggerCtx.DestroyedList,Context->TriggerCtx.TempTrigger.param2,(BOOL)-1)) return;
			if (!Triggers_DisplayCreateTrapsList(hDlg,400,&Context->TriggerCtx.DestroyedList,Context->TriggerCtx.TempTrigger.param2,(BOOL)-1)) return;
			Context->TriggerCtx.Initialized[TRIGGER_DESTROYED] = TRUE;
			break;

		case TRIGGER_SPELLS:
			if (!Triggers_DisplayCreateSpellsList(hDlg,400,&Context->TriggerCtx.SpellsList,Context->TriggerCtx.TempTrigger.param2,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_SPELLS] = TRUE;
			break;

		case TRIGGER_GOLDMINED:
			Context->TriggerCtx.Initialized[TRIGGER_GOLDMINED] = TRUE;
			break;

		case TRIGGER_GOLDAMOUNT:
			Context->TriggerCtx.Initialized[TRIGGER_GOLDAMOUNT] = TRUE;
			break;

		case TRIGGER_MANAAMOUNT:
			Context->TriggerCtx.Initialized[TRIGGER_MANAAMOUNT] = TRUE;
			break;

		case TRIGGER_SLAPCOUNT:
			Context->TriggerCtx.Initialized[TRIGGER_SLAPCOUNT] = TRUE;
			break;

		case TRIGGER_CLICKELEMENT:
			if (!Triggers_DisplayCreateButtonsList(hDlg,400,&Context->TriggerCtx.ButtonsList,Context->TriggerCtx.TempTrigger.param1,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CLICKELEMENT] = TRUE;
			break;

		case TRIGGER_GROUPINPOSSESS:
			Context->TriggerCtx.Initialized[TRIGGER_GROUPINPOSSESS] = TRUE;
			break;

		case TRIGGER_HEARTDESTROYED:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.HeartList,Context->TriggerCtx.TempTrigger.param1,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_HEARTDESTROYED] = TRUE;
			break;

		case TRIGGER_ENTERAREAINPOSS:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.AreaPossessPlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_ENTERAREAINPOSS] = TRUE;
			break;

		case TRIGGER_CREAENTERAREA:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->TriggerCtx.AreaCreaturesList,Context->TriggerCtx.TempTrigger.param3,TRUE,FALSE)) return;
			if (!Triggers_DisplayCreatePlayersList(hDlg,403,&Context->TriggerCtx.AreaPlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CREAENTERAREA] = TRUE;
			break;

		case TRIGGER_CLAIMAREA:
			TempText = GlobalAlloc(GPTR,128);
			if (!TempText) return;
			Area = RectEx_FindById(Context->TriggerCtx.ParentPoint->area,&MapAreas);
			Map_CorrectRect(&Area->rect,&Client);
			wsprintf(TempText,szTriggersAreaTotalSquares,Client.Width*Client.Height);
			SetDlgItemText(hDlg,101,TempText);
			wsprintf(TempText,szTriggersAreaPerimeterSquares,(Client.Width > 1)?((Client.Width-2)*2+Client.Height*2):1);
			SetDlgItemText(hDlg,102,TempText);
			GlobalFree(TempText);
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.AreaClaimPlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CLAIMAREA] = TRUE;
			break;

		case TRIGGER_CLAIMENTIREAREA:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.AreaClaimEntirePlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_CLAIMENTIREAREA] = TRUE;
			break;

		case TRIGGER_AREACONTENT:
			TempText = GlobalAlloc(GPTR,128);
			if (!TempText) return;
			Area = RectEx_FindById(Context->TriggerCtx.ParentPoint->area,&MapAreas);
			Map_CorrectRect(&Area->rect,&Client);
			wsprintf(TempText,szTriggersAreaTotalSquares,Client.Width*Client.Height);
			SetDlgItemText(hDlg,101,TempText);
			GlobalFree(TempText);
			if (!Triggers_DisplayCreateTerrainList(hDlg,400,&Context->TriggerCtx.AreaContentList,Context->TriggerCtx.TempTrigger.param3,TRUE,TRUE)) return;
			if (!Misc_CreatePlayerComboBoxList(hDlg,501,1,FALSE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_AREACONTENT] = TRUE;
			break;

		case TRIGGER_SELECTAREA:
			TempText = GlobalAlloc(GPTR,128);
			if (!TempText) return;
			Area = RectEx_FindById(Context->TriggerCtx.ParentPoint->area,&MapAreas);
			Map_CorrectRect(&Area->rect,&Client);
			wsprintf(TempText,szTriggersAreaTotalSquares,Client.Width*Client.Height);
			SetDlgItemText(hDlg,101,TempText);
			GlobalFree(TempText);
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.AreaSelectPlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_SELECTAREA] = TRUE;
			break;

		case TRIGGER_SELECTALLAREA:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->TriggerCtx.AreaSelectAllPlayersList,Context->TriggerCtx.TempTrigger.param2,TRUE)) return;
			Context->TriggerCtx.Initialized[TRIGGER_SELECTALLAREA] = TRUE;
			break;

		case TRIGGER_BANDCREATED:
			if (!Triggers_DisplayCreateHerosList(hDlg,400,&Context->TriggerCtx.CreateHerosList,Context->TriggerCtx.TempTrigger.param2)) return;
			Context->TriggerCtx.Initialized[TRIGGER_BANDCREATED] = TRUE;
			break;

		case TRIGGER_DEATHSINBAND:
			if (!Triggers_DisplayCreateHerosList(hDlg,400,&Context->TriggerCtx.DeadHerosList,Context->TriggerCtx.TempTrigger.param2)) return;
			Context->TriggerCtx.Initialized[TRIGGER_DEATHSINBAND] = TRUE;
			break;

		case TRIGGER_JAILEDINBAND:
			if (!Triggers_DisplayCreateHerosList(hDlg,400,&Context->TriggerCtx.JailHerosList,Context->TriggerCtx.TempTrigger.param2)) return;
			Context->TriggerCtx.Initialized[TRIGGER_JAILEDINBAND] = TRUE;
			break;

		case TRIGGER_STUNNEDINBAND:
			if (!Triggers_DisplayCreateHerosList(hDlg,400,&Context->TriggerCtx.StunHerosList,Context->TriggerCtx.TempTrigger.param2)) return;
			Context->TriggerCtx.Initialized[TRIGGER_STUNNEDINBAND] = TRUE;
			break;

		case TRIGGER_HURTED:
			Context->TriggerCtx.Initialized[TRIGGER_HURTED] = TRUE;
			break;
		}

	return;
}


// «»»» Initialisations des données variables «««««««««««««««««««««««««««»

void Triggers_TriggerDlgParamsCheck(HWND hDlg, TRIGGERSCTX *Context)
{
	switch(Context->TriggerCtx.TempTrigger.ctrl)
		{
		case TRIGGER_VARIABLE:
			if (!Context->TriggerCtx.Initialized[TRIGGER_VARIABLE]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param2,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXVARLEN-1,(LPARAM)0);
			SendDlgItemMessage(hDlg,209,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param4,(LPARAM)0);
			if (Context->TriggerCtx.TempTrigger.param3 == 0x02)
				{
				CheckDlgButton(hDlg,206,BST_UNCHECKED);
				CheckDlgButton(hDlg,207,BST_CHECKED);
				EnableWindow(GetDlgItem(hDlg,210),FALSE);
				EnableWindow(GetDlgItem(hDlg,209),TRUE);
				}
			else
				{
				CheckDlgButton(hDlg,206,BST_CHECKED);
				CheckDlgButton(hDlg,207,BST_UNCHECKED);
				EnableWindow(GetDlgItem(hDlg,210),TRUE);
				EnableWindow(GetDlgItem(hDlg,209),FALSE);
				}
			break;

		case TRIGGER_TIMER:
			if (!Context->TriggerCtx.Initialized[TRIGGER_TIMER]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,300,Context->TriggerCtx.TempTrigger.param5/60/60,FALSE);
			SetDlgItemInt(hDlg,301,Context->TriggerCtx.TempTrigger.param5/60%60,FALSE);
			SetDlgItemInt(hDlg,302,Context->TriggerCtx.TempTrigger.param5%60,FALSE);
			SendDlgItemMessage(hDlg,300,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,301,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,302,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param2,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXTIMLEN-1,(LPARAM)0);
			break;

		case TRIGGER_TIMEELAPSED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_TIMEELAPSED]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SetDlgItemInt(hDlg,700,Context->TriggerCtx.TempTrigger.param5/60/60,FALSE);
			SetDlgItemInt(hDlg,701,Context->TriggerCtx.TempTrigger.param5/60%60,FALSE);
			SetDlgItemInt(hDlg,702,Context->TriggerCtx.TempTrigger.param5%60,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)9,(LPARAM)0);
			SendDlgItemMessage(hDlg,700,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,701,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,702,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			break;

		case TRIGGER_CREATURES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CREATURES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_HAPPYCREATURES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_HAPPYCREATURES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_ANGRYCREATURES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ANGRYCREATURES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_LOSTCREATURES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_LOSTCREATURES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_KILLEDCREATURES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_KILLEDCREATURES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_CREATURESLEVEL:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CREATURESLEVEL]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SendDlgItemMessage(hDlg,404,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param4-1,(LPARAM)0);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_CREATUREEXP:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CREATUREEXP]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SendDlgItemMessage(hDlg,404,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param5-1,(LPARAM)0);
			break;

		case TRIGGER_ROOMS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ROOMS]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_ROOMSTYPES:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ROOMSTYPES]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_ROOMSSIZE:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ROOMSSIZE]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_ROOMSCONTENT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ROOMSCONTENT]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_DOORS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_DOORS]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			switch(Context->TriggerCtx.TempTrigger.param3)
				{
				case 0x09:
					CheckRadioButton(hDlg,206,208,206);
					break;
				case 0x11:
					CheckRadioButton(hDlg,206,208,207);
					break;
				default:
					CheckRadioButton(hDlg,206,208,208);
					break;
				}
			break;

		case TRIGGER_TRAPS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_TRAPS]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			switch(Context->TriggerCtx.TempTrigger.param3)
				{
				case 0x09:
					CheckRadioButton(hDlg,206,208,206);
					break;
				case 0x11:
					CheckRadioButton(hDlg,206,208,207);
					break;
				default:
					CheckRadioButton(hDlg,206,208,208);
					break;
				}
			break;

		case TRIGGER_DESTROYED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_DESTROYED]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			break;

		case TRIGGER_SPELLS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_SPELLS]) goto Error_0;
			break;

		case TRIGGER_GOLDMINED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_GOLDMINED]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)9,(LPARAM)0);
			break;

		case TRIGGER_GOLDAMOUNT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_GOLDAMOUNT]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)7,(LPARAM)0);
			break;

		case TRIGGER_MANAAMOUNT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_MANAAMOUNT]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			break;

		case TRIGGER_SLAPCOUNT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_SLAPCOUNT]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_CLICKELEMENT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CLICKELEMENT]) goto Error_0;
			break;

		case TRIGGER_GROUPINPOSSESS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_GROUPINPOSSESS]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_HEARTDESTROYED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_HEARTDESTROYED]) goto Error_0;
			break;

		case TRIGGER_ENTERAREAINPOSS:
			if (!Context->TriggerCtx.Initialized[TRIGGER_ENTERAREAINPOSS]) goto Error_0;
			break;

		case TRIGGER_CREAENTERAREA:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CREAENTERAREA]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_CLAIMAREA:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CLAIMAREA]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_CLAIMENTIREAREA:
			if (!Context->TriggerCtx.Initialized[TRIGGER_CLAIMENTIREAREA]) goto Error_0;
			break;

		case TRIGGER_AREACONTENT:
			if (!Context->TriggerCtx.Initialized[TRIGGER_AREACONTENT]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			SendDlgItemMessage(hDlg,501,CB_SETCURSEL,(WPARAM)Context->TriggerCtx.TempTrigger.param2-1,(LPARAM)0);
			break;

		case TRIGGER_SELECTAREA:
			if (!Context->TriggerCtx.Initialized[TRIGGER_SELECTAREA]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_SELECTALLAREA:
			if (!Context->TriggerCtx.Initialized[TRIGGER_SELECTALLAREA]) goto Error_0;
			break;

		case TRIGGER_BANDCREATED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_BANDCREATED]) goto Error_0;
			if (!MapHerosBands.next) goto Error_1;
			break;

		case TRIGGER_DEATHSINBAND:
			if (!Context->TriggerCtx.Initialized[TRIGGER_DEATHSINBAND]) goto Error_0;
			if (!MapHerosBands.next) goto Error_1;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_JAILEDINBAND:
			if (!Context->TriggerCtx.Initialized[TRIGGER_JAILEDINBAND]) goto Error_0;
			if (!MapHerosBands.next) goto Error_1;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_STUNNEDINBAND:
			if (!Context->TriggerCtx.Initialized[TRIGGER_STUNNEDINBAND]) goto Error_0;
			if (!MapHerosBands.next) goto Error_1;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case TRIGGER_HURTED:
			if (!Context->TriggerCtx.Initialized[TRIGGER_HURTED]) goto Error_0;
			CheckRadioButton(hDlg,200,205,199+Context->TriggerCtx.TempTrigger.param1);
			SetDlgItemInt(hDlg,210,Context->TriggerCtx.TempTrigger.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			break;
		}

	PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_NEXT);
	SetWindowLong(hDlg,DWL_MSGRESULT,0);
	return;

//--- La page n'a pas été correctement initialisée ---

Error_1:Misc_MessageBoxExt(hDlg,szTriggersNoBand,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7100);
	return;
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7100);
	return;
}


// «»»» Validation des changements ««««««««««««««««««««««««««««««««««««««»

//--- Stockage temporaire des valeurs ---

int Triggers_TriggerDlgParamsStore(HWND hDlg,TRIGGERSCTX *Context)
{
	int	Result = 0;

	switch(Context->TriggerCtx.TempTrigger.ctrl)
		{
		case TRIGGER_VARIABLE:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Context->TriggerCtx.TempTrigger.param4 = SendDlgItemMessage(hDlg,209,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (SendDlgItemMessage(hDlg,212,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,212,WM_GETTEXT,(WPARAM)TRIGGERS_MAXVARLEN,(LPARAM)&TriggersVariables[Context->TriggerCtx.TempTrigger.param2][0]);
			if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED)
				{
				Context->TriggerCtx.TempTrigger.param3 = 0x02;
				Context->TriggerCtx.TempTrigger.param5 = 0;
				}
			else
				{
				Context->TriggerCtx.TempTrigger.param3 = 0x01;
				Context->TriggerCtx.TempTrigger.param4 = 0;
				}
			Result = 1;
			break;

		case TRIGGER_TIMER:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,300,NULL,FALSE)*60*60+GetDlgItemInt(hDlg,301,NULL,FALSE)*60+GetDlgItemInt(hDlg,302,NULL,FALSE);
			if (SendDlgItemMessage(hDlg,212,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,212,WM_GETTEXT,(WPARAM)TRIGGERS_MAXTIMLEN,(LPARAM)&TriggersTimers[Context->TriggerCtx.TempTrigger.param2][0]);
			Result = 1;
			break;

		case TRIGGER_TIMEELAPSED:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CREATURES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.CreaturesList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoCreatureSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_HAPPYCREATURES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.HappyCreaturesList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoCreatureSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_ANGRYCREATURES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AngryCreaturesList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoCreatureSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_LOSTCREATURES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param4 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.LostPlayersList);
			if (Context->TriggerCtx.TempTrigger.param4 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param4 == -1) Context->TriggerCtx.TempTrigger.param4 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_KILLEDCREATURES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param4 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.KilledPlayersList);
			if (Context->TriggerCtx.TempTrigger.param4 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param4 == -1) Context->TriggerCtx.TempTrigger.param4 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CREATURESLEVEL:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param4 = SendDlgItemMessage(hDlg,404,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CREATUREEXP:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = SendDlgItemMessage(hDlg,404,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Result = 1;
			break;

		case TRIGGER_ROOMS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.RoomsList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoRoomSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_ROOMSTYPES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.RoomsTypeList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoRoomSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_ROOMSSIZE:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.RoomsSizeList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoRoomSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_ROOMSCONTENT:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.RoomsContentList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoRoomSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_DOORS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.DoorsList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoDoorSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			if (IsDlgButtonChecked(hDlg,206) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param3 = 0x09;
			else if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param3 = 0x11;
			else Context->TriggerCtx.TempTrigger.param3 = 0x01;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_TRAPS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.TrapsList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoTrapSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			if (IsDlgButtonChecked(hDlg,206) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param3 = 0x09;
			else if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param3 = 0x11;
			else Context->TriggerCtx.TempTrigger.param3 = 0x01;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_DESTROYED:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.DestroyedList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoDoorTrapSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_SPELLS:
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.SpellsList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoSpellSelected;
			Result = 1;
			break;

		case TRIGGER_GOLDMINED:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_GOLDAMOUNT:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_MANAAMOUNT:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_SLAPCOUNT:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CLICKELEMENT:
			Result = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.ButtonsList);
			if (!Result) goto NoButtonSelected;
			Context->TriggerCtx.TempTrigger.param1 = Result>>16;
			Context->TriggerCtx.TempTrigger.param2 = Result&0x0000FFFF;
			Result = 1;
			break;

		case TRIGGER_GROUPINPOSSESS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_HEARTDESTROYED:
			Context->TriggerCtx.TempTrigger.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.HeartList);
			if (Context->TriggerCtx.TempTrigger.param1 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param1 == -1) Context->TriggerCtx.TempTrigger.param1 = 0;
			Result = 1;
			break;

		case TRIGGER_ENTERAREAINPOSS:
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaPossessPlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Result = 1;
			break;

		case TRIGGER_CREAENTERAREA:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaPlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param3 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaCreaturesList);
			if (Context->TriggerCtx.TempTrigger.param3 == 0) goto NoCreatureSelected;
			if (Context->TriggerCtx.TempTrigger.param3 == -1) Context->TriggerCtx.TempTrigger.param3 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CLAIMAREA:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaClaimPlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_CLAIMENTIREAREA:
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaClaimEntirePlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Result = 1;
			break;

		case TRIGGER_AREACONTENT:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = SendDlgItemMessage(hDlg,501,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Context->TriggerCtx.TempTrigger.param3 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaContentList);
			if (Context->TriggerCtx.TempTrigger.param3 == 0) goto NoTerrainSelected;
			if (Context->TriggerCtx.TempTrigger.param3 == -1) Context->TriggerCtx.TempTrigger.param3 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_SELECTAREA:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaSelectPlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_SELECTALLAREA:
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.AreaSelectAllPlayersList);
			if (Context->TriggerCtx.TempTrigger.param2 == 0) goto NoPlayerSelected;
			if (Context->TriggerCtx.TempTrigger.param2 == -1) Context->TriggerCtx.TempTrigger.param2 = 0;
			Result = 1;
			break;

		case TRIGGER_BANDCREATED:
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.CreateHerosList)-1;
			if (Context->TriggerCtx.TempTrigger.param2 < 0) goto NoBandSelected;
			Result = 1;
			break;

		case TRIGGER_DEATHSINBAND:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.DeadHerosList)-1;
			if (Context->TriggerCtx.TempTrigger.param2 < 0) goto NoBandSelected;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_JAILEDINBAND:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.JailHerosList)-1;
			if (Context->TriggerCtx.TempTrigger.param2 < 0) goto NoBandSelected;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_STUNNEDINBAND:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->TriggerCtx.StunHerosList)-1;
			if (Context->TriggerCtx.TempTrigger.param2 < 0) goto NoBandSelected;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;

		case TRIGGER_HURTED:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 3;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 4;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 5;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->TriggerCtx.TempTrigger.param1 = 6;
			Context->TriggerCtx.TempTrigger.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
			Result = 1;
			break;
		}

	return(Result);

//--- Une donnée requise n'a pas été sélectionnée ---

NoCreatureSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoCreatureSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoTerrainSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoTerrainSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoDoorTrapSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoDoorTrapSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoDoorSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoDoorSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoTrapSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoTrapSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoPlayerSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoPlayerSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoButtonSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoButtonSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoSpellSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoSpellSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoRoomSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoRoomSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoBandSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoBandSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
}

//--- Stockage définitif des valeurs ---

int Triggers_TriggerDlgParamsSave(HWND hDlg,TRIGGERSCTX *Context)
{
	TV_INSERTSTRUCT	 Parent;
	TRIGGER		*NewTrigger;
	char		*TextBuffer;

	switch(Context->TriggerCtx.TempTrigger.ctrl)
		{
		case TRIGGER_DOORDESTROYED:
		case TRIGGER_OBJECTCLAIMED:
		case TRIGGER_APPEAR:
		case TRIGGER_DEATH:
		case TRIGGER_SEE:
		case TRIGGER_TURNCOAT:
		case TRIGGER_STUNNED:
		case TRIGGER_HURTCREATURE:
		case TRIGGER_SLAPPED:
		case TRIGGER_JAILED:
		case TRIGGER_TORTURED:
		case TRIGGER_CONVERTED:
		case TRIGGER_HUNGRY:
		case TRIGGER_AFRAID:
		case TRIGGER_STEAL:
		case TRIGGER_GOAWAY:
		case TRIGGER_NOTHUNGRY:
		case TRIGGER_FIRED:
		case TRIGGER_TAKE:
		case TRIGGER_TRANSITIONENDS:
			Context->TriggerCtx.TempTrigger.param1 = 0;
			Context->TriggerCtx.TempTrigger.param2 = 0;
			Context->TriggerCtx.TempTrigger.param3 = 0;
			Context->TriggerCtx.TempTrigger.param4 = 0;
			Context->TriggerCtx.TempTrigger.param5 = 0;
			Context->TriggerCtx.TempTrigger.param6 = 0;
			Context->TriggerCtx.TempTrigger.param7 = 0;
			Context->TriggerCtx.TempTrigger.param8 = 0;
			break;
		}

	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) goto Error_0;
	NewTrigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
	if (!NewTrigger) goto Error_1;

	Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM;
	Parent.DUMMYUNIONNAME.item.lParam = 0;
	Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (Parent.DUMMYUNIONNAME.item.hItem) SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);

	//--- Modifie un déclencheur ---

	if (Context->Modify)
		{
		GlobalFree(NewTrigger);
		CopyMemory(Context->TriggerCtx.SrceTrigger,&Context->TriggerCtx.TempTrigger,sizeof(TRIGGER));
		Parent.DUMMYUNIONNAME.item.mask = TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtTriggerText(Context->TriggerCtx.SrceTrigger,TextBuffer,TextBuffer+2048);
		if (Parent.DUMMYUNIONNAME.item.lParam) SendMessage(Context->List,TVM_SETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);
		goto Done;
		}

	//--- Ajoute ou insère un déclencheur ---

	if ((Context->Insert)&&(Parent.DUMMYUNIONNAME.item.lParam))
		{
		CopyMemory(NewTrigger,&Context->TriggerCtx.TempTrigger,sizeof(TRIGGER));
		NewTrigger->id = Triggers_GetTriggerFreeId();
		Parent.hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
		Parent.hInsertAfter = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
		if (!Parent.hInsertAfter) Parent.hInsertAfter = TVI_FIRST;
		Parent.DUMMYUNIONNAME.item.hItem = NULL;
		Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtTriggerText(NewTrigger,TextBuffer,TextBuffer+2048);
		Parent.DUMMYUNIONNAME.item.iImage = 0;
		Parent.DUMMYUNIONNAME.item.iSelectedImage = 0;
		Parent.DUMMYUNIONNAME.item.cChildren = 1;
		Parent.DUMMYUNIONNAME.item.lParam = (LPARAM)NewTrigger;
		Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Parent);
		if (!Parent.DUMMYUNIONNAME.item.hItem) goto Error_2;
		}
	else
		{
		CopyMemory(NewTrigger,&Context->TriggerCtx.TempTrigger,sizeof(TRIGGER));
		NewTrigger->id = Triggers_GetTriggerFreeId();
		if ((Parent.DUMMYUNIONNAME.item.lParam)&&(!Context->Root))
			{
			if (((NODE *)Parent.DUMMYUNIONNAME.item.lParam)->type == TYPE_TRIGGER)
				Parent.hParent = Parent.DUMMYUNIONNAME.item.hItem;
			else
				Parent.hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
			Parent.hInsertAfter = TVI_LAST;
			}
		else
			{
			Parent.hParent = TVI_ROOT;
			Parent.hInsertAfter = TVI_LAST;
			}
		Parent.DUMMYUNIONNAME.item.hItem = NULL;
		Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtTriggerText(NewTrigger,TextBuffer,TextBuffer+2048);
		Parent.DUMMYUNIONNAME.item.iImage = 0;
		Parent.DUMMYUNIONNAME.item.iSelectedImage = 0;
		Parent.DUMMYUNIONNAME.item.cChildren = 1;
		Parent.DUMMYUNIONNAME.item.lParam = (LPARAM)NewTrigger;
		Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Parent);
		if (!Parent.DUMMYUNIONNAME.item.hItem) goto Error_2;
		}

	//--- Lie le déclencheur aux entrée précédentes ---

	List_AddEntry((NODE *)NewTrigger,&MapTriggers);
	if (!Triggers_SelectLink(NewTrigger,Context->TriggerCtx.ParentPoint,Parent.DUMMYUNIONNAME.item.hItem,Context)) goto Error_3;

	#if (DEBUG_EVENTS)
	Parent.DUMMYUNIONNAME.item.mask = TVIF_TEXT;
	Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtTriggerText(NewTrigger,TextBuffer,TextBuffer+2048);
	SendMessage(Context->List,TVM_SETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);
	#endif

//--- Terminé... ----

Done:	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
	GlobalFree(TextBuffer);
	MapChanges++;
	return(1);

//--- Erreurs ---

Error_3:List_RemEntry((NODE *)NewTrigger);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
Error_2:GlobalFree(NewTrigger);
Error_1:GlobalFree(TextBuffer);
Error_0:Misc_MessageBoxExt(hDlg,szTriggersNewTriggerErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return(0);
}


// «»»» Obtention des paramètres de l'entrée sélectionnée «««««««««««««««»

long Triggers_TriggerDlgGetSelectedEntryParam(HWND hList)
{
	LV_ITEM	Item;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(hList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) return(0);
	if (!SendMessage(hList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
	if (!Item.lParam) return(0);
	return((long)Item.lParam);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Drapeaux de répétition et sauvegarde définitive			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_TriggerDlgRepeat(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	TRIGGERINFO	*Info;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		SetWindowLong(hDlg,DWL_USER,(LONG)Context);
		SendDlgItemMessage(hDlg,803,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 800:
						case 802:
							EnableWindow(GetDlgItem(hDlg,803),FALSE);
							break;
						case 801:
							EnableWindow(GetDlgItem(hDlg,803),TRUE);
							break;
						}
					return(TRUE);
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 803:
							lData = GetDlgItemInt(hDlg,803,NULL,FALSE);
							if (lData > 254) SetDlgItemInt(hDlg,803,254,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,803,0,FALSE);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
                                case PSN_SETACTIVE:
					if (Context->TriggerCtx.TempTrigger.repeat == 0x01)
						{
						CheckDlgButton(hDlg,800,BST_CHECKED);
						SetDlgItemInt(hDlg,803,2,FALSE);
						EnableWindow(GetDlgItem(hDlg,803),FALSE);
						}
					else if (Context->TriggerCtx.TempTrigger.repeat == 0xFF)
						{
						CheckDlgButton(hDlg,802,BST_CHECKED);
						SetDlgItemInt(hDlg,803,2,FALSE);
						EnableWindow(GetDlgItem(hDlg,803),FALSE);
						}
					else	
						{
						CheckDlgButton(hDlg,801,BST_CHECKED);
						SetDlgItemInt(hDlg,803,Context->TriggerCtx.TempTrigger.repeat,FALSE);
						EnableWindow(GetDlgItem(hDlg,803),TRUE);
						}
					PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_FINISH);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_WIZBACK:
					if (IsDlgButtonChecked(hDlg,800) == BST_CHECKED) Context->TriggerCtx.TempTrigger.repeat = 0x01;
					else if (IsDlgButtonChecked(hDlg,801) == BST_CHECKED) Context->TriggerCtx.TempTrigger.repeat = (BYTE)GetDlgItemInt(hDlg,803,NULL,FALSE);
					else Context->TriggerCtx.TempTrigger.repeat = 0xFF;
					Info = Triggers_TriggerDlgGetParam(Context);
					if ((!Info)||(Info->dialog == -1)) lData = 7100;
					else lData = Info->dialog;
					SetWindowLong(hDlg,DWL_MSGRESULT,lData);
					return(TRUE);

				case PSN_WIZFINISH:
					if (IsDlgButtonChecked(hDlg,800) == BST_CHECKED) Context->TriggerCtx.TempTrigger.repeat = 0x01;
					else if (IsDlgButtonChecked(hDlg,801) == BST_CHECKED) Context->TriggerCtx.TempTrigger.repeat = (BYTE)GetDlgItemInt(hDlg,803,NULL,FALSE);
					else Context->TriggerCtx.TempTrigger.repeat = 0xFF;
					Triggers_TriggerDlgParamsSave(hDlg,Context);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
