
//<<>-<>>---------------------------------------------------------------------()
/*
	Ajout, modification des actions
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
extern HWND		hWnd;
extern ULONG		SpeechUniqueID;
extern MAP		Map;
extern LONG		MapChanges;
extern NODE		MapActions;
extern NODE		MapAreas;
extern NODE		MapHerosBands;
extern NODE		MapEffects;
extern NODE		MapGates;
extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ajout ou Modification d'une action				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Triggers_ActionNew(HWND hDlg, TRIGGERSCTX *Context)
{
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	char		*FileName;
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
		CopyMemory(&Context->ActionCtx.TempAction,(void *)Item.lParam,sizeof(ACTION));
		Context->ActionCtx.SrceAction = (ACTION *)Item.lParam;
		}
	else
		{
		Context->ActionCtx.SrceAction = NULL;
		Context->ActionCtx.TempAction.node.next = NULL;
		Context->ActionCtx.TempAction.node.prev = NULL;
		Context->ActionCtx.TempAction.node.type = TYPE_ACTION;
		if (!Context->ActionCtx.TempAction.ctrl)
			{
			Context->ActionCtx.TempAction.param1 = 0;
			Context->ActionCtx.TempAction.param2 = 0x0a;
			Context->ActionCtx.TempAction.param3 = 0;
			Context->ActionCtx.TempAction.param4 = 0;
			Context->ActionCtx.TempAction.param5 = 0;
			Context->ActionCtx.TempAction.param6 = 0;
			Context->ActionCtx.TempAction.param7 = 0;
			Context->ActionCtx.TempAction.param8 = 0;
			Context->ActionCtx.TempAction.one = 0x01;
			Context->ActionCtx.TempAction.ctrl = ACTION_SETVARIABLE;
			}
		}

	for (Count = 1; Actions[Count].ctrl != 0; Count++);

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*(Count+1));
	if (!psp) goto Error_1;

	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].hInstance = hInstance;
	psp[0].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(7200);
	psp[0].pfnDlgProc = Triggers_ActionDlgList;
	psp[0].lParam = (LPARAM)Context;
	for (Result = 1; Result != Count+1; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(Actions[Result-1].dialog);
		psp[Result].pfnDlgProc = Triggers_ActionDlgParams;
		psp[Result].lParam = (LPARAM)Context;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_WIZARD;
	psh->hwndParent = hDlg;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = NULL;
	psh->nPages = Count+1;
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
	Context->PointIcon = LoadImage(hInstance,MAKEINTRESOURCE(19),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->PointIcon) goto Error_2;
	Context->RectIcon = LoadImage(hInstance,MAKEINTRESOURCE(25),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->RectIcon) goto Error_3;
	Context->CameraIcon = LoadImage(hInstance,MAKEINTRESOURCE(26),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->CameraIcon) goto Error_4;

	for (Result = 0; Result != Count; Result++)
		{
		Context->ActionCtx.Initialized[Actions[Result].ctrl] = FALSE;
		}

	Result = PropertySheet(psh);

	FileName = GlobalAlloc(GPTR,2048);
	if (FileName)
		{
		GetTempPath(2048,FileName);
		wsprintf(szTempSpeechID,szNumber,SpeechUniqueID-1);
		strcat(FileName,szTempSoundFile);
		strcat(FileName,szTempSpeechID);
		strcat(FileName,szTempMP2Ext);
		Properties_MapVoicesDeleteTempFile(hWnd,FileName);
		GlobalFree(FileName);
		SpeechUniqueID = 0;
		}
	else
		SpeechUniqueID--;

	if (Result == -1) goto Error_5;

	DestroyIcon(Context->CameraIcon);
	DestroyIcon(Context->RectIcon);
	DestroyIcon(Context->PointIcon);
	Misc_DestroyImageList(&Context->Images);
	List_ReleaseMemory(&Context->ActionCtx.SpeechList);
	GlobalFree(psp);
	GlobalFree(psh);
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_5:DestroyIcon(Context->CameraIcon);
Error_4:DestroyIcon(Context->RectIcon);
Error_3:DestroyIcon(Context->PointIcon);
Error_2:Misc_DestroyImageList(&Context->Images);
	GlobalFree(psp);
Error_1:GlobalFree(psh);
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	List_ReleaseMemory(&Context->ActionCtx.SpeechList);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection du type d'action					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_ActionDlgList(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		Triggers_ActionDlgInit(hDlg,Context);
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
							lData = (LONG)Triggers_ActionDlgGetParam(Context);
							if (!lData) return(TRUE);
							Triggers_ActionDlgSetDefault((BYTE)((TRIGGERINFO *)lData)->ctrl,Context);
							Context->ActionCtx.TempAction.ctrl = (BYTE)((TRIGGERINFO *)lData)->ctrl;
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)(((TRIGGERINFO *)lData)->dialog == -1)?PSWIZB_FINISH:PSWIZB_NEXT);
							}
						}
					return(TRUE);

                                case PSN_SETACTIVE:
					if (!Context->ActionCtx.MainSet)
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
						}
					else
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,0);
						PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_DISABLEDFINISH);
						lData = (LONG)Triggers_ActionDlgGetParam(Context);
						if (!lData) return(TRUE);
						PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)(((TRIGGERINFO *)lData)->dialog == -1)?PSWIZB_FINISH:PSWIZB_NEXT);
						}
					return(TRUE);

				case PSN_WIZNEXT:
					lData = (LONG)Triggers_ActionDlgGetParam(Context);
					if (!lData)
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
					else
						SetWindowLong(hDlg,DWL_MSGRESULT,((TRIGGERINFO *)lData)->dialog);
					return(TRUE);

				case PSN_WIZFINISH:
					Triggers_ActionDlgParamsSave(hDlg,Context);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void Triggers_ActionDlgInit(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	int		 Selected;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Context->ActionCtx.MainSet = FALSE;

	GetWindowRect(GetDlgItem(hDlg,101),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	Context->ActionCtx.ActionsList = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->ActionCtx.ActionsList) goto Error_0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->ActionCtx.ActionsList,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) goto Error_0;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) goto Error_0;
	if (!Item.lParam) goto Error_0;
	Context->ActionCtx.ParentPoint = (TRIGGERLINK *)Item.lParam;

	Item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE|LVIF_PARAM;
	Item.iSubItem = 0;
	Item.iImage = 7;
	for (Item.iItem = 0, Selected = 0; Actions[Item.iItem].ctrl != 0; Item.iItem++)
		{
		Item.state = 0;
		Item.stateMask = LVIS_SELECTED;
		Item.pszText = Actions[Item.iItem].text;
		if (Context->ActionCtx.ParentPoint->node.type&Actions[Item.iItem].scope) Item.lParam = Actions[Item.iItem].ctrl;
		#if (!TRIGGERS_SHOWINCOMPATIBLEAC)
		else continue;
		#else
		else Item.lParam = 0;
		#endif
		if (Actions[Item.iItem].ctrl == Context->ActionCtx.TempAction.ctrl) Item.state = LVIS_SELECTED;
		if (SendMessage(Context->ActionCtx.ActionsList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
		if (Actions[Item.iItem].ctrl == Context->ActionCtx.TempAction.ctrl) Selected = Item.iItem;
		}
	SendMessage(Context->ActionCtx.ActionsList,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);

	Context->ActionCtx.MainSet = TRUE;
	return;

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Obtention des paramètres d'un déclencheur «««««««««««««««««««««««»

TRIGGERINFO* Triggers_ActionDlgGetParam(TRIGGERSCTX *Context)
{
	LV_ITEM	Item;
	int	i;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->ActionCtx.ActionsList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) return(NULL);
	if (!SendMessage(Context->ActionCtx.ActionsList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(NULL);
	if (!Item.lParam) return(NULL);

	for (i = 0; Actions[i].ctrl != 0; i++)
		{
		if (Actions[i].ctrl != Item.lParam) continue;
		return(&Actions[i]);
		}

	return(NULL);
}


// «»»» Place les valeurs par défaut (si nécessaire) ««««««««««««««««««««»

void Triggers_ActionDlgSetDefault(BYTE NewCtrl, TRIGGERSCTX *Context)
{
	switch(NewCtrl)
		{
		case ACTION_ADDCREATURE:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 4;
				Context->ActionCtx.TempAction.param2 = 3;
				Context->ActionCtx.TempAction.param3 = 1;
				Context->ActionCtx.TempAction.param4 = 0x29;
				Context->ActionCtx.TempAction.param5 = 1;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 1;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_ADDELEMENT:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 3;
				Context->ActionCtx.TempAction.param2 = 5;
				Context->ActionCtx.TempAction.param3 = 5;
				Context->ActionCtx.TempAction.param4 = 1;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SETVARIABLE:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0x0a;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SETTIMER:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SHOWELEMENT:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0xF3;
				Context->ActionCtx.TempAction.param3 = 1;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_RELEASEBAND:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 1;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_CHANGEPARAMS:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 3;
				Context->ActionCtx.TempAction.param2 = 0x13;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 1;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SHOWAREA:
		case ACTION_TOGGLEEFFECT:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 1;
				Context->ActionCtx.TempAction.param2 = 1;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_ENABLEFOG:
		case ACTION_MOVECAMERA:
		case ACTION_RUN:
		case ACTION_CHANGEVIEW:
		case ACTION_PRIMARYTARGET:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 1;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_ALLIES:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 3;
				Context->ActionCtx.TempAction.param2 = 4;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_CHANGESQUARE:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 6;
				Context->ActionCtx.TempAction.param2 = 2;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 2;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 2;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_DISPLAYTIMER:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 16;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 160;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_ROTATECAMERA:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 1;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 360;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 7;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_DESTROYGATE:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 1;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SHOWNEXTROOM:
		case ACTION_PORTALS:
		case ACTION_MOODS:
		case ACTION_TIMERSPEECH:
		case ACTION_SYSTEMMESSAGES:
		case ACTION_MUSICLEVEL:
		case ACTION_ATTACK:
		case ACTION_SAY:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_CHANGEOWNER:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 3;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 2;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 2;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SLAPSREMAINING:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 16;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_GENERATE:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 7;
				Context->ActionCtx.TempAction.param2 = 1;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_CAMERAPATH:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0;
				Context->ActionCtx.TempAction.param2 = 1;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SHOWHEALTH:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 10;
				Context->ActionCtx.TempAction.param2 = 0;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;

		case ACTION_SHOWCRYSTAL:
			if (Context->ActionCtx.TempAction.ctrl != NewCtrl)
				{
				Context->ActionCtx.TempAction.param1 = 0x1C;
				Context->ActionCtx.TempAction.param2 = 1;
				Context->ActionCtx.TempAction.param3 = 0;
				Context->ActionCtx.TempAction.param4 = 0;
				Context->ActionCtx.TempAction.param5 = 0;
				Context->ActionCtx.TempAction.param6 = 0;
				Context->ActionCtx.TempAction.param7 = 0;
				Context->ActionCtx.TempAction.param8 = 0;
				}
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres de l'action						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_ActionDlgParams(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		Triggers_ActionDlgParamsInit(hDlg,Context);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_MeasurePlayerComboBoxList(303,(MEASUREITEMSTRUCT *)lParam);
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
				Misc_DisplayPlayerComboBoxList(303,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Triggers_DisplayDrawList((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				switch(((DRAWITEMSTRUCT *)lParam)->CtlID)
					{
					case 312:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->PointIcon);
						break;
					case 314:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->RectIcon);
						break;
					case 315:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->CameraIcon);
						break;
					}
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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 700:
						case 701:
						case 702:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_DISPLAYTIMER)
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
								SetDlgItemInt(hDlg,307,Hours*60*60+Minutes*60+Seconds,FALSE);
								}
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 200:
						case 201:
						case 202:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_SETVARIABLE)
								{
								EnableWindow(GetDlgItem(hDlg,210),TRUE);
								EnableWindow(GetDlgItem(hDlg,213),FALSE);
								EnableWindow(GetDlgItem(hDlg,214),FALSE);
								}
							break;
						case 203:
						case 204:
						case 205:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_SETVARIABLE)
								{
								EnableWindow(GetDlgItem(hDlg,210),FALSE);
								EnableWindow(GetDlgItem(hDlg,213),TRUE);
								EnableWindow(GetDlgItem(hDlg,214),TRUE);
								Context->ActionCtx.TempAction.param5 = SendDlgItemMessage(hDlg,214,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
								}
							break;

						case 313:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_ADDCREATURE)
								{
								Things_SelectOptions(hDlg,(DWORD *)&Context->ActionCtx.TempAction.param4,NULL,THING_OPTIONSADD);
								}
							break;

						case 312:
							Triggers_ActionDlgChoosePoint(hDlg);
							break;
						case 314:
							Map_ChoosePointEx(hDlg,&Context->ActionCtx.FullRect,MAP_CHOICELMBRECT,szTriggersChoiceRect);
							SetDlgItemInt(hDlg,307,Context->ActionCtx.FullRect.x1,FALSE);
							SetDlgItemInt(hDlg,309,Context->ActionCtx.FullRect.y1,FALSE);
							break;
						case 315:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_CAMERAPATH)
								{
								CAMERAPATH	CameraPath;

								ZeroMemory(&CameraPath,sizeof(CAMERAPATH));
								CameraPath.PathId = GetDlgItemInt(hDlg,307,NULL,FALSE);
								if (Library_Call(hDlg,LIBRARY_CAMERAPATH,&CameraPath))
									{
									SetDlgItemInt(hDlg,307,CameraPath.PathId,FALSE);
									CheckRadioButton(hDlg,200,201,(CameraPath.ShowCell)?200:201);
									}
								}
							else
								{
								if (IsDlgButtonChecked(hDlg,315) == BST_CHECKED)
									{
									EnableWindow(GetDlgItem(hDlg,306),FALSE);
									EnableWindow(GetDlgItem(hDlg,307),FALSE);
									EnableWindow(GetDlgItem(hDlg,308),FALSE);
									EnableWindow(GetDlgItem(hDlg,309),FALSE);
									EnableWindow(GetDlgItem(hDlg,310),FALSE);
									EnableWindow(GetDlgItem(hDlg,311),FALSE);
									EnableWindow(GetDlgItem(hDlg,312),FALSE);
									EnableWindow(GetDlgItem(hDlg,314),TRUE);
									Context->ActionCtx.ChangeRect = TRUE;
									Context->ActionCtx.FullRect.x1 = GetDlgItemInt(hDlg,307,NULL,FALSE);
									Context->ActionCtx.FullRect.y1 = GetDlgItemInt(hDlg,309,NULL,FALSE);
									Context->ActionCtx.FullRect.x2 = Context->ActionCtx.FullRect.x1;
									Context->ActionCtx.FullRect.y2 = Context->ActionCtx.FullRect.y1;
									}
								else
									{
									EnableWindow(GetDlgItem(hDlg,306),TRUE);
									EnableWindow(GetDlgItem(hDlg,307),TRUE);
									EnableWindow(GetDlgItem(hDlg,308),TRUE);
									EnableWindow(GetDlgItem(hDlg,309),TRUE);
									EnableWindow(GetDlgItem(hDlg,310),TRUE);
									EnableWindow(GetDlgItem(hDlg,311),TRUE);
									EnableWindow(GetDlgItem(hDlg,312),TRUE);
									EnableWindow(GetDlgItem(hDlg,314),FALSE);
									Context->ActionCtx.ChangeRect = FALSE;
									}
								break;
								}
							break;
						case 500:
							if (IsDlgButtonChecked(hDlg,500) == BST_CHECKED)
								EnableWindow(GetDlgItem(hDlg,501),TRUE);
							else
								{
								CheckDlgButton(hDlg,501,BST_UNCHECKED);
								EnableWindow(GetDlgItem(hDlg,501),FALSE);
								}
							break;
						}
					return(TRUE);

				case LBN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 401:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_SAY)
								{
								Properties_MapVoicesGetSoundList(hDlg,(char *)Map.Properties.SpeechFile,SendDlgItemMessage(hDlg,401,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1,NULL);
								return(TRUE);
								}
							break;
						}
					break;

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 401:
							if (Context->ActionCtx.TempAction.ctrl == ACTION_SAY)
								{
								SetDlgItemInt(hDlg,307,SendDlgItemMessage(hDlg,401,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1,FALSE);
								Properties_MapTextsSetText(hDlg,402,(char *)Map.Properties.SpeechFile,SendDlgItemMessage(hDlg,401,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1);
								return(TRUE);
								}
							break;

						case 311:
							switch(Context->ActionCtx.TempAction.ctrl)
								{
								case ACTION_ADDCREATURE:
								case ACTION_CHANGESQUARE:
								case ACTION_CHANGEOWNER: {
									MAPRECTEX	*Area;
									MAPCLIENT	 Client;

									lData = Misc_GetArea(hDlg,311,TRUE);
									if (!lData) break;
									Area = RectEx_FindById(lData,&MapAreas);
									if (!Area) break;
									Map_CorrectRect(&Area->rect,&Client);
									SetDlgItemInt(hDlg,307,Client.X,FALSE);
									SetDlgItemInt(hDlg,309,Client.Y,FALSE);
									} return(TRUE);
								}
							break;

						case 212:
							lData = SendDlgItemMessage(hDlg,212,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							if (lData < 0) break;
							Context->ActionCtx.TempAction.param1 = lData;
							break;
						case 214:
							lData = SendDlgItemMessage(hDlg,214,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							if (lData < 0) break;
							Context->ActionCtx.TempAction.param5 = lData;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case LVN_ITEMCHANGED:
					if (!IsWindow(Context->ActionCtx.TerrainList)) break;
					if (((NMHDR *)lParam)->hwndFrom == Context->ActionCtx.TerrainList)
						{
						if (!(((NM_LISTVIEW *)lParam)->uNewState&LVIS_SELECTED))
							EnableWindow(GetDlgItem(hDlg,303),FALSE);
						else
							{
							lData = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.TerrainList);
							if ((lData <= 0x07)||(lData == 0x1F))
								{
								SendDlgItemMessage(hDlg,303,CB_SETCURSEL,(WPARAM)1,(LPARAM)0);
								EnableWindow(GetDlgItem(hDlg,303),FALSE);
								}
							else if (lData == 0x23)
								{
								SendDlgItemMessage(hDlg,303,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
								EnableWindow(GetDlgItem(hDlg,303),FALSE);
								}
							else
								EnableWindow(GetDlgItem(hDlg,303),TRUE);
							}
						}
					return(TRUE);

                                case PSN_SETACTIVE:
					Triggers_ActionDlgParamsCheck(hDlg,Context);
					return(TRUE);

				case PSN_WIZBACK:
					Triggers_ActionDlgParamsStore(hDlg,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,7200);
					return(TRUE);

				case PSN_WIZFINISH:
					if (!Triggers_ActionDlgParamsStore(hDlg,Context))
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,-1);
						return(TRUE);
						}
					Triggers_ActionDlgParamsSave(hDlg,Context);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations des données fixes «««««««««««««««««««««««««««««««»

void Triggers_ActionDlgParamsInit(HWND hDlg, TRIGGERSCTX *Context)
{
	char	*TempText;
	int	 i;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Context->ActionCtx.TempAction.ctrl)
		{
		case ACTION_ADDCREATURE:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->ActionCtx.CreaturesList,Context->ActionCtx.TempAction.param1,FALSE,TRUE)) return;
			if (!Misc_CreateAreaComboBoxList(hDlg,311,TRUE)) return;
			if (!Misc_CreatePlayerComboBoxList(hDlg,303,1,FALSE)) return;
			if (!Misc_CreateLevelComboBoxList(hDlg,301,Context->ActionCtx.TempAction.param3)) return;
			Context->ActionCtx.Initialized[ACTION_ADDCREATURE] = TRUE;
			break;

		case ACTION_SHOWCRYSTAL:
			Context->ActionCtx.Initialized[ACTION_SHOWCRYSTAL] = TRUE;
			break;

		case ACTION_ADDELEMENT:
			if (!Triggers_DisplayCreateButtonsList(hDlg,400,&Context->ActionCtx.ElementsList,Context->ActionCtx.TempAction.param2,Context->ActionCtx.TempAction.param3,-1)) return;
			if (!Misc_CreatePlayerComboBoxList(hDlg,303,1,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_ADDELEMENT] = TRUE;
			break;

		case ACTION_SETVARIABLE:
			TempText = GlobalAlloc(GPTR,64);
			if (!TempText) return;
			for (i = 0; i != TRIGGERS_MAXVARIABLES; i++)
				{
				if (TriggersVariables[i][0]) wsprintf(TempText,szText,&TriggersVariables[i][0]);
				else wsprintf(TempText,szNumber,i);
				if (i == TRIGGERS_MAXVARIABLES-1)
					strcpy(TempText,szTriggersScoreVar);
				if ((SendDlgItemMessage(hDlg,212,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)||(SendDlgItemMessage(hDlg,214,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0))
					{
					GlobalFree(TempText);
					return;
					}
				}
			GlobalFree(TempText);
			Context->ActionCtx.Initialized[ACTION_SETVARIABLE] = TRUE;
			break;

		case ACTION_SETTIMER:
			TempText = GlobalAlloc(GPTR,64);
			if (!TempText) return;
			for (i = 0; i != TRIGGERS_MAXTIMERS-1; i++)
				{
				if (TriggersTimers[i][0]) wsprintf(TempText,szText,&TriggersTimers[i][0]);
				else wsprintf(TempText,szNumber,i);
				if (SendDlgItemMessage(hDlg,212,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
					{
					GlobalFree(TempText);
					return;
					}
				}
			GlobalFree(TempText);
			Context->ActionCtx.Initialized[ACTION_SETTIMER] = TRUE;
			break;

		case ACTION_SHOWELEMENT:
			if (!Triggers_DisplayCreateButtonsList(hDlg,400,&Context->ActionCtx.ButtonsList,Context->ActionCtx.TempAction.param1,Context->ActionCtx.TempAction.param2,0)) return;
			Context->ActionCtx.Initialized[ACTION_SHOWELEMENT] = TRUE;
			break;

		case ACTION_RELEASEBAND:
			if (!Triggers_DisplayCreateHerosList(hDlg,400,&Context->ActionCtx.HerosList,Context->ActionCtx.TempAction.param1)) return;
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_RELEASEBAND] = TRUE;
			break;

		case ACTION_CHANGEPARAMS:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->ActionCtx.PlayersList,Context->ActionCtx.TempAction.param1,TRUE)) return;
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_CHANGEPARAMS] = TRUE;
			break;

		case ACTION_SHOWAREA:
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_SHOWAREA] = TRUE;
			break;

		case ACTION_ENABLEFOG:
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_ENABLEFOG] = TRUE;
			break;

		case ACTION_ALLIES:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->ActionCtx.FirstAlliePlayersList,Context->ActionCtx.TempAction.param1,FALSE)) return;
			if (!Triggers_DisplayCreatePlayersList(hDlg,401,&Context->ActionCtx.SecondAlliePlayersList,Context->ActionCtx.TempAction.param2,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_ALLIES] = TRUE;
			break;

		case ACTION_CHANGESQUARE:
			if (!Triggers_DisplayCreateTerrainList(hDlg,400,&Context->ActionCtx.TerrainList,Context->ActionCtx.TempAction.param1,TRUE,FALSE)) return;
			if (!Misc_CreateAreaComboBoxList(hDlg,311,TRUE)) return;
			if (!Misc_CreatePlayerComboBoxList(hDlg,303,1,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_CHANGESQUARE] = TRUE;
			break;

		case ACTION_DISPLAYTIMER:
			TempText = GlobalAlloc(GPTR,64);
			if (!TempText) return;
			for (i = 0; i != TRIGGERS_MAXTIMERS; i++)
				{
				if (TriggersTimers[i][0]) wsprintf(TempText,szText,&TriggersTimers[i][0]);
				else wsprintf(TempText,szNumber,i);
				if (i == TRIGGERS_MAXTIMERS-1)
					strcpy(TempText,szTriggersLimitTime);
				if (SendDlgItemMessage(hDlg,212,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
					{
					GlobalFree(TempText);
					return;
					}
				}
			GlobalFree(TempText);
			Context->ActionCtx.Initialized[ACTION_DISPLAYTIMER] = TRUE;
			break;

		case ACTION_SAY:
			if (Context->ActionCtx.SpeechList.next == NULL) Properties_MapVoicesGetSoundList(hDlg,(char *)Map.Properties.SpeechFile,-1,&Context->ActionCtx.SpeechList);
			Properties_MapVoicesCreateDisplayList(hDlg,401,&Context->ActionCtx.SpeechList);
			Context->ActionCtx.Initialized[ACTION_SAY] = TRUE;
			break;

		case ACTION_MOVECAMERA:
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_MOVECAMERA] = TRUE;
			break;

		case ACTION_ROTATECAMERA:
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_ROTATECAMERA] = TRUE;
			break;

		case ACTION_DESTROYGATE:
			if (!Triggers_DisplayCreateGatesList(hDlg,311)) return;
			Context->ActionCtx.Initialized[ACTION_DESTROYGATE] = TRUE;
			break;

		case ACTION_RUN:
			Context->ActionCtx.Initialized[ACTION_RUN] = TRUE;
			break;

		case ACTION_CHANGEVIEW:
			Context->ActionCtx.Initialized[ACTION_CHANGEVIEW] = TRUE;
			break;

		case ACTION_PRIMARYTARGET:
			Context->ActionCtx.Initialized[ACTION_PRIMARYTARGET] = TRUE;
			break;

		case ACTION_SHOWNEXTROOM:
			if (!Triggers_DisplayCreateRoomsList(hDlg,400,&Context->ActionCtx.RoomsList,Context->ActionCtx.TempAction.param1,-1,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_SHOWNEXTROOM] = TRUE;
			break;

		case ACTION_CHANGEOWNER:
			if (!Triggers_DisplayCreatePlayersList(hDlg,400,&Context->ActionCtx.OwnersList,Context->ActionCtx.TempAction.param2,FALSE)) return;
			if (!Misc_CreateAreaComboBoxList(hDlg,311,TRUE)) return;
			Context->ActionCtx.Initialized[ACTION_CHANGEOWNER] = TRUE;
			break;

		case ACTION_SLAPSREMAINING:
			Context->ActionCtx.Initialized[ACTION_SLAPSREMAINING] = TRUE;
			break;

		case ACTION_GENERATE:
			if (!Triggers_DisplayCreateCreaturesList(hDlg,400,&Context->ActionCtx.GenCreaturesList,Context->ActionCtx.TempAction.param1,FALSE,TRUE)) return;
			if (!Misc_CreateLevelComboBoxList(hDlg,301,Context->ActionCtx.TempAction.param2)) return;
			Context->ActionCtx.Initialized[ACTION_GENERATE] = TRUE;
			break;

		case ACTION_PORTALS:
			Context->ActionCtx.Initialized[ACTION_PORTALS] = TRUE;
			break;

		case ACTION_MOODS:
			Context->ActionCtx.Initialized[ACTION_MOODS] = TRUE;
			break;

		case ACTION_TIMERSPEECH:
			Context->ActionCtx.Initialized[ACTION_TIMERSPEECH] = TRUE;
			break;

		case ACTION_CAMERAPATH:
			if (!Misc_CreateAreaComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_CAMERAPATH] = TRUE;
			break;

		case ACTION_SYSTEMMESSAGES:
			Context->ActionCtx.Initialized[ACTION_SYSTEMMESSAGES] = TRUE;
			break;

		case ACTION_MUSICLEVEL:
			Context->ActionCtx.Initialized[ACTION_MUSICLEVEL] = TRUE;
			SendDlgItemMessage(hDlg,500,TBM_SETTICFREQ,(WPARAM)25,(LPARAM)0);
			SendDlgItemMessage(hDlg,500,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(0,100));
			break;

		case ACTION_ATTACK:
			Context->ActionCtx.Initialized[ACTION_ATTACK] = TRUE;
			break;

		case ACTION_SHOWHEALTH:
			Context->ActionCtx.Initialized[ACTION_SHOWHEALTH] = TRUE;
			break;

		case ACTION_TOGGLEEFFECT:
			if (!Misc_CreateEffectComboBoxList(hDlg,311,FALSE)) return;
			Context->ActionCtx.Initialized[ACTION_TOGGLEEFFECT] = TRUE;
			break;
		}

	return;
}


// «»»» Initialisations des données variables «««««««««««««««««««««««««««»

void Triggers_ActionDlgParamsCheck(HWND hDlg, TRIGGERSCTX *Context)
{
	switch(Context->ActionCtx.TempAction.ctrl)
		{
		case ACTION_ADDCREATURE:
			if (!Context->ActionCtx.Initialized[ACTION_ADDCREATURE]) goto Error_0;
			SendDlgItemMessage(hDlg,303,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param2-1,(LPARAM)0);
			SendDlgItemMessage(hDlg,301,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param3-1,(LPARAM)0);
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5,FALSE);
			SetDlgItemInt(hDlg,309,Context->ActionCtx.TempAction.param7,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,309,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,311,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			break;

		case ACTION_SHOWCRYSTAL:
			if (!Context->ActionCtx.Initialized[ACTION_SHOWCRYSTAL]) goto Error_0;
			break;

		case ACTION_ADDELEMENT:
			if (!Context->ActionCtx.Initialized[ACTION_ADDELEMENT]) goto Error_0;
			SendDlgItemMessage(hDlg,303,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param1-1,(LPARAM)0);
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param4)?200:201);
			break;

		case ACTION_SETVARIABLE:
			if (!Context->ActionCtx.Initialized[ACTION_SETVARIABLE]) goto Error_0;
			SendDlgItemMessage(hDlg,214,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			switch(Context->ActionCtx.TempAction.param2)
				{
				case 0x0A:
					CheckRadioButton(hDlg,200,205,200);
					EnableWindow(GetDlgItem(hDlg,210),TRUE);
					EnableWindow(GetDlgItem(hDlg,213),FALSE);
					EnableWindow(GetDlgItem(hDlg,214),FALSE);
					break;
				case 0x0C:
					CheckRadioButton(hDlg,200,205,203);
					EnableWindow(GetDlgItem(hDlg,210),FALSE);
					EnableWindow(GetDlgItem(hDlg,213),TRUE);
					EnableWindow(GetDlgItem(hDlg,214),TRUE);
					SendDlgItemMessage(hDlg,214,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param5,(LPARAM)0);
					break;
				case 0x12:
					CheckRadioButton(hDlg,200,205,201);
					EnableWindow(GetDlgItem(hDlg,210),TRUE);
					EnableWindow(GetDlgItem(hDlg,213),FALSE);
					EnableWindow(GetDlgItem(hDlg,214),FALSE);
					break;
				case 0x14:
					CheckRadioButton(hDlg,200,205,204);
					EnableWindow(GetDlgItem(hDlg,210),FALSE);
					EnableWindow(GetDlgItem(hDlg,213),TRUE);
					EnableWindow(GetDlgItem(hDlg,214),TRUE);
					SendDlgItemMessage(hDlg,214,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param5,(LPARAM)0);
					break;
				case 0x22:
					CheckRadioButton(hDlg,200,205,202);
					EnableWindow(GetDlgItem(hDlg,210),TRUE);
					EnableWindow(GetDlgItem(hDlg,213),FALSE);
					EnableWindow(GetDlgItem(hDlg,214),FALSE);
					break;
				case 0x24:
					CheckRadioButton(hDlg,200,205,205);
					EnableWindow(GetDlgItem(hDlg,210),FALSE);
					EnableWindow(GetDlgItem(hDlg,213),TRUE);
					EnableWindow(GetDlgItem(hDlg,214),TRUE);
					SendDlgItemMessage(hDlg,214,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param5,(LPARAM)0);
					break;
				}
			SetDlgItemInt(hDlg,210,Context->ActionCtx.TempAction.param5,FALSE);
			SendDlgItemMessage(hDlg,210,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param1,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXVARLEN-1,(LPARAM)0);
			SendDlgItemMessage(hDlg,214,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXVARLEN-1,(LPARAM)0);
			break;

		case ACTION_SETTIMER:
			if (!Context->ActionCtx.Initialized[ACTION_SETTIMER]) goto Error_0;
			SendDlgItemMessage(hDlg,212,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param1,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXTIMLEN-1,(LPARAM)0);
			break;

		case ACTION_SHOWELEMENT:
			if (!Context->ActionCtx.Initialized[ACTION_SHOWELEMENT]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param3)?200:201);
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			break;

		case ACTION_RELEASEBAND:
			if (!Context->ActionCtx.Initialized[ACTION_RELEASEBAND]) goto Error_0;
			if (!MapHerosBands.next) goto Error_1;
			if (!MapAreas.next) goto Error_2;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param5);
			switch(Context->ActionCtx.TempAction.param2)
				{
				case 0:	CheckDlgButton(hDlg,500,BST_UNCHECKED);
					CheckDlgButton(hDlg,501,BST_UNCHECKED);
					EnableWindow(GetDlgItem(hDlg,501),FALSE);
					break;
				case 1:	CheckDlgButton(hDlg,500,BST_CHECKED);
					CheckDlgButton(hDlg,501,BST_UNCHECKED);
					EnableWindow(GetDlgItem(hDlg,501),TRUE);
					break;
				case 2:	CheckDlgButton(hDlg,500,BST_CHECKED);
					CheckDlgButton(hDlg,501,BST_CHECKED);
					EnableWindow(GetDlgItem(hDlg,501),TRUE);
					break;
				}
			break;

		case ACTION_CHANGEPARAMS:
			if (!Context->ActionCtx.Initialized[ACTION_CHANGEPARAMS]) goto Error_0;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param5);
			switch(Context->ActionCtx.TempAction.param2)
				{
				case 0x0B:
					CheckRadioButton(hDlg,200,209,200);
					break;
				case 0x0C:
					CheckRadioButton(hDlg,200,209,201);
					break;
				case 0x0D:
					CheckRadioButton(hDlg,200,209,202);
					break;
				case 0x0E:
					CheckRadioButton(hDlg,200,209,203);
					break;
				case 0x11:
					CheckRadioButton(hDlg,200,209,204);
					break;
				case 0x12:
					CheckRadioButton(hDlg,200,209,205);
					break;
				case 0x13:
					CheckRadioButton(hDlg,200,209,206);
					break;
				case 0x16:
					CheckRadioButton(hDlg,200,209,207);
					break;
				case 0x17:
					CheckRadioButton(hDlg,200,209,208);
					break;
				case 0x1B:
					CheckRadioButton(hDlg,200,209,209);
					break;
				}
			break;

		case ACTION_SHOWAREA:
			if (!Context->ActionCtx.Initialized[ACTION_SHOWAREA]) goto Error_0;
			if (!MapAreas.next) goto Error_2;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param1);
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param2)?201:200);
			break;

		case ACTION_ENABLEFOG:
			if (!Context->ActionCtx.Initialized[ACTION_ENABLEFOG]) goto Error_0;
			if (!MapAreas.next) goto Error_2;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param1);
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param2)?201:200);
			break;

		case ACTION_ALLIES:
			if (!Context->ActionCtx.Initialized[ACTION_ALLIES]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param3)?201:200);
			break;

		case ACTION_CHANGESQUARE:
			if (!Context->ActionCtx.Initialized[ACTION_CHANGESQUARE]) goto Error_0;
			if (Context->Modify) EnableWindow(GetDlgItem(hDlg,315),FALSE);
			SendDlgItemMessage(hDlg,303,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param2-1,(LPARAM)0);
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5-1,FALSE);
			SetDlgItemInt(hDlg,309,Context->ActionCtx.TempAction.param7-1,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,309,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,311,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			break;

		case ACTION_DISPLAYTIMER:
			if (!Context->ActionCtx.Initialized[ACTION_DISPLAYTIMER]) goto Error_0;
			SendDlgItemMessage(hDlg,212,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param1,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,CB_LIMITTEXT,(WPARAM)TRIGGERS_MAXTIMLEN-1,(LPARAM)0);
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5,FALSE);
			SetDlgItemInt(hDlg,700,Context->ActionCtx.TempAction.param5/60/60,FALSE);
			SetDlgItemInt(hDlg,701,Context->ActionCtx.TempAction.param5/60%60,FALSE);
			SetDlgItemInt(hDlg,702,Context->ActionCtx.TempAction.param5%60,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,700,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,701,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,702,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			break;

		case ACTION_SAY:
			if (!Context->ActionCtx.Initialized[ACTION_SAY]) goto Error_0;
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param1,FALSE);
			CheckDlgButton(hDlg,308,(Context->ActionCtx.TempAction.param5)?BST_CHECKED:BST_UNCHECKED);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case ACTION_MOVECAMERA:
			if (!Context->ActionCtx.Initialized[ACTION_MOVECAMERA]) goto Error_0;
			if (!MapAreas.next) goto Error_2;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param1);
			break;

		case ACTION_ROTATECAMERA:
			if (!Context->ActionCtx.Initialized[ACTION_ROTATECAMERA]) goto Error_0;
			if (!MapAreas.next) goto Error_2;
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param1);
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param3,FALSE);
			SetDlgItemInt(hDlg,309,Context->ActionCtx.TempAction.param5,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,309,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case ACTION_DESTROYGATE:
			if (!Context->ActionCtx.Initialized[ACTION_DESTROYGATE]) goto Error_0;
			if (!MapGates.next) goto Error_3;
			Triggers_ActionDlgParamsSetGate(hDlg,311,Context->ActionCtx.TempAction.param5);
			break;

		case ACTION_RUN:
			if (!Context->ActionCtx.Initialized[ACTION_RUN]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_CHANGEVIEW:
			if (!Context->ActionCtx.Initialized[ACTION_CHANGEVIEW]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_PRIMARYTARGET:
			if (!Context->ActionCtx.Initialized[ACTION_PRIMARYTARGET]) goto Error_0;
			switch(Context->ActionCtx.TempAction.param1)
				{
				case 0: CheckRadioButton(hDlg,200,203,200);
					break;
				case 2: CheckRadioButton(hDlg,200,203,202);
					break;
				case 3: CheckRadioButton(hDlg,200,203,203);
					break;
				default:CheckRadioButton(hDlg,200,203,201);
					break;
				}
			break;

		case ACTION_SHOWNEXTROOM:
			if (!Context->ActionCtx.Initialized[ACTION_SHOWNEXTROOM]) goto Error_0;
			break;

		case ACTION_CHANGEOWNER:
			if (!Context->ActionCtx.Initialized[ACTION_CHANGEOWNER]) goto Error_0;
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5-1,FALSE);
			SetDlgItemInt(hDlg,309,Context->ActionCtx.TempAction.param7-1,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,309,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,311,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			break;

		case ACTION_SLAPSREMAINING:
			if (!Context->ActionCtx.Initialized[ACTION_SLAPSREMAINING]) goto Error_0;
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param5,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case ACTION_GENERATE:
			if (!Context->ActionCtx.Initialized[ACTION_GENERATE]) goto Error_0;
			SendDlgItemMessage(hDlg,301,CB_SETCURSEL,(WPARAM)Context->ActionCtx.TempAction.param2-1,(LPARAM)0);
			break;

		case ACTION_PORTALS:
			if (!Context->ActionCtx.Initialized[ACTION_PORTALS]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_MOODS:
			if (!Context->ActionCtx.Initialized[ACTION_MOODS]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_TIMERSPEECH:
			if (!Context->ActionCtx.Initialized[ACTION_TIMERSPEECH]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_CAMERAPATH:
			if (!Context->ActionCtx.Initialized[ACTION_CAMERAPATH]) goto Error_0;
			if (!MapAreas.next) goto Error_2;
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param1,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			Misc_SetArea(hDlg,311,Context->ActionCtx.TempAction.param2);
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param3)?201:200);
			break;

		case ACTION_SYSTEMMESSAGES:
			if (!Context->ActionCtx.Initialized[ACTION_SYSTEMMESSAGES]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_MUSICLEVEL:
			if (!Context->ActionCtx.Initialized[ACTION_MUSICLEVEL]) goto Error_0;
			SendDlgItemMessage(hDlg,500,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Context->ActionCtx.TempAction.param1);
			break;

		case ACTION_ATTACK:
			if (!Context->ActionCtx.Initialized[ACTION_ATTACK]) goto Error_0;
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param1)?200:201);
			break;

		case ACTION_SHOWHEALTH:
			if (!Context->ActionCtx.Initialized[ACTION_SHOWHEALTH]) goto Error_0;
			SetDlgItemInt(hDlg,307,Context->ActionCtx.TempAction.param1,FALSE);
			SendDlgItemMessage(hDlg,307,EM_LIMITTEXT,(WPARAM)4,(LPARAM)0);
			break;

		case ACTION_TOGGLEEFFECT:
			if (!Context->ActionCtx.Initialized[ACTION_TOGGLEEFFECT]) goto Error_0;
			if (!MapEffects.next) goto Error_4;
			Misc_SetEffect(hDlg,311,Context->ActionCtx.TempAction.param1);
			CheckRadioButton(hDlg,200,201,(Context->ActionCtx.TempAction.param2)?200:201);
			break;
		}

	PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_FINISH);
	SetWindowLong(hDlg,DWL_MSGRESULT,0);
	return;

//--- La page n'a pas été correctement initialisée ---

Error_4:Misc_MessageBoxExt(hDlg,szTriggersNoEffect,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7200);
	return;
Error_3:Misc_MessageBoxExt(hDlg,szTriggersNoGate,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7200);
	return;
Error_2:Misc_MessageBoxExt(hDlg,szTriggersNoArea,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7200);
	return;
Error_1:Misc_MessageBoxExt(hDlg,szTriggersNoBand,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7200);
	return;
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	SetWindowLong(hDlg,DWL_MSGRESULT,7200);
	return;
}


// «»»» Choix de coordonnées ««««««««««««««««««««««««««««««««««««««««««««»

void Triggers_ActionDlgChoosePoint(HWND hDlg)
{
	MAPRECT	MapRect;

	MapRect.x1 = GetDlgItemInt(hDlg,307,NULL,FALSE);
	MapRect.y1 = GetDlgItemInt(hDlg,309,NULL,FALSE);
	Map_ChoosePointEx(hDlg,&MapRect,MAP_CHOICELMBPOINT,szTriggersChoice);
	SetDlgItemInt(hDlg,307,MapRect.x1,FALSE);
	SetDlgItemInt(hDlg,309,MapRect.y1,FALSE);
	return;
}


// «»»» Gestion des passage «««««««««««««««««««««««««««««««««««««««««««««»

//--- Retrouve le passage sélectionné ---

UINT Triggers_ActionDlgParamsGetGate(HWND hDlg, UINT CtlID)
{
	MAPRECTEX	*Gate;
	int		 Offset;

	Offset = SendDlgItemMessage(hDlg,CtlID,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	Gate = (MAPRECTEX *)MapGates.next;
	while (Offset--) Gate = (MAPRECTEX *)Gate->node.next;

	return(Gate->id);
}

//--- Liste des passages ---

void Triggers_ActionDlgParamsSetGate(HWND hDlg, UINT CtlID, UINT GateID)
{
	MAPRECTEX	*Gate;
	int		 Offset;

	for (Offset = 0, Gate = (MAPRECTEX *)MapGates.next; Gate != 0; Gate = (MAPRECTEX *)Gate->node.next)
		{
		if (Gate->id == GateID) break;
		Offset++;
		}

	if (!Gate)
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	else
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)Offset,(LPARAM)0);

	return;
}


// «»»» Validation des changements ««««««««««««««««««««««««««««««««««««««»

//--- Stockage temporaire des valeurs ---

int Triggers_ActionDlgParamsStore(HWND hDlg,TRIGGERSCTX *Context)
{
	int	Result = 0;

	switch(Context->ActionCtx.TempAction.ctrl)
		{
		case ACTION_ADDCREATURE:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.CreaturesList);
			if (Context->ActionCtx.TempAction.param1 == 0) goto NoCreatureSelected;
			Context->ActionCtx.TempAction.param2 = SendDlgItemMessage(hDlg,303,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Context->ActionCtx.TempAction.param3 = SendDlgItemMessage(hDlg,301,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Context->ActionCtx.TempAction.param7 = GetDlgItemInt(hDlg,309,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_ADDELEMENT:
			Context->ActionCtx.TempAction.param1 = SendDlgItemMessage(hDlg,303,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Result = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.ElementsList);
			if (!Result) goto NoElementSelected;
			Context->ActionCtx.TempAction.param2 = Result>>16;
			Context->ActionCtx.TempAction.param3 = Result&0x0000FFFF;
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param4 = 1;
			else Context->ActionCtx.TempAction.param4 = 0;
			Result = 1;
			break;

		case ACTION_SETVARIABLE:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0A;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x12;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x22;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0C;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x14;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x24;
			switch(Context->ActionCtx.TempAction.param2)
				{
				case 0x0A:
				case 0x12:
				case 0x22:
					Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,210,NULL,FALSE);
					break;
				default:
					if (SendDlgItemMessage(hDlg,214,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,214,WM_GETTEXT,(WPARAM)TRIGGERS_MAXVARLEN,(LPARAM)&TriggersVariables[Context->ActionCtx.TempAction.param5][0]);
					break;
				}
			if (SendDlgItemMessage(hDlg,212,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,212,WM_GETTEXT,(WPARAM)TRIGGERS_MAXVARLEN,(LPARAM)&TriggersVariables[Context->ActionCtx.TempAction.param1][0]);
			Result = 1;
			break;

		case ACTION_SETTIMER:
			if (SendDlgItemMessage(hDlg,212,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,212,WM_GETTEXT,(WPARAM)TRIGGERS_MAXTIMLEN,(LPARAM)&TriggersTimers[Context->ActionCtx.TempAction.param1][0]);
			Result = 1;
			break;

		case ACTION_SHOWELEMENT:
			Result = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.ButtonsList);
			if (!Result) goto NoButtonSelected;
			Context->ActionCtx.TempAction.param1 = Result>>16;
			Context->ActionCtx.TempAction.param2 = Result&0x0000FFFF;
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param3 = 1;
			else Context->ActionCtx.TempAction.param3 = 0;
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_RELEASEBAND:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.HerosList);
			Context->ActionCtx.TempAction.param5 = Misc_GetArea(hDlg,311,FALSE);
			if (IsDlgButtonChecked(hDlg,500) == BST_CHECKED)
				{
				Context->ActionCtx.TempAction.param2 = 1;
				if (IsDlgButtonChecked(hDlg,501) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 2;
				}
			else
				Context->ActionCtx.TempAction.param2 = 0;
			if (!Context->ActionCtx.TempAction.param1) goto NoBandSelected;
			Context->ActionCtx.TempAction.param1--;
			Result = 1;
			break;

		case ACTION_CHANGEPARAMS:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.PlayersList);
			if (!Context->ActionCtx.TempAction.param1) goto NoPlayerSelected;
			if (Context->ActionCtx.TempAction.param1 == -1) Context->ActionCtx.TempAction.param1 = 0;
			     if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0B;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0C;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0D;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x0E;
			else if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x11;
			else if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x12;
			else if (IsDlgButtonChecked(hDlg,206) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x13;
			else if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x16;
			else if (IsDlgButtonChecked(hDlg,208) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x17;
			else if (IsDlgButtonChecked(hDlg,209) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0x1B;
			Context->ActionCtx.TempAction.param5 = Misc_GetArea(hDlg,311,FALSE);
			Result = 1;
			break;

		case ACTION_SHOWAREA:
			Context->ActionCtx.TempAction.param1 = Misc_GetArea(hDlg,311,FALSE);
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0;
			else Context->ActionCtx.TempAction.param2 = 1;
			Result = 1;
			break;

		case ACTION_ENABLEFOG:
			Context->ActionCtx.TempAction.param1 = Misc_GetArea(hDlg,311,FALSE);
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 0;
			else Context->ActionCtx.TempAction.param2 = 1;
			Result = 1;
			break;

		case ACTION_ALLIES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param3 = 0;
			else Context->ActionCtx.TempAction.param3 = 1;
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.FirstAlliePlayersList);
			Context->ActionCtx.TempAction.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.SecondAlliePlayersList);
			if (!Context->ActionCtx.TempAction.param1) goto NoPlayerSelected;
			if (!Context->ActionCtx.TempAction.param2) goto NoPlayerSelected;
			Result = 1;
			break;

		case ACTION_CHANGESQUARE:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.TerrainList);
			Context->ActionCtx.TempAction.param2 = SendDlgItemMessage(hDlg,303,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE)+1;
			Context->ActionCtx.TempAction.param7 = GetDlgItemInt(hDlg,309,NULL,FALSE)+1;
			Result = 1;
			break;

		case ACTION_DISPLAYTIMER:
			if (SendDlgItemMessage(hDlg,212,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0)) SendDlgItemMessage(hDlg,212,WM_GETTEXT,(WPARAM)TRIGGERS_MAXTIMLEN,(LPARAM)&TriggersTimers[Context->ActionCtx.TempAction.param1][0]);
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_SAY:
			Context->ActionCtx.TempAction.param1 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Context->ActionCtx.TempAction.param5 = (IsDlgButtonChecked(hDlg,308) == BST_CHECKED)?1:0;
			Result = 1;
			break;

		case ACTION_MOVECAMERA:
			Context->ActionCtx.TempAction.param1 = Misc_GetArea(hDlg,311,FALSE);
			Result = 1;
			break;

		case ACTION_ROTATECAMERA:
			Context->ActionCtx.TempAction.param1 = Misc_GetArea(hDlg,311,FALSE);
			Context->ActionCtx.TempAction.param3 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,309,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_DESTROYGATE:
			Context->ActionCtx.TempAction.param5 = Triggers_ActionDlgParamsGetGate(hDlg,311);
			Result = 1;
			break;

		case ACTION_RUN:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_CHANGEVIEW:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_PRIMARYTARGET:
			if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 2;
			else if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 3;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_SHOWNEXTROOM:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.RoomsList);
			if (!Context->ActionCtx.TempAction.param1) goto NoRoomSelected;
			if (Context->ActionCtx.TempAction.param1 == -1) Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_CHANGEOWNER:
			Context->ActionCtx.TempAction.param2 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.OwnersList);
			if (!Context->ActionCtx.TempAction.param2) goto NoPlayerSelected;
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE)+1;
			Context->ActionCtx.TempAction.param7 = GetDlgItemInt(hDlg,309,NULL,FALSE)+1;
			Result = 1;
			break;

		case ACTION_SLAPSREMAINING:
			Context->ActionCtx.TempAction.param5 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_GENERATE:
			Context->ActionCtx.TempAction.param1 = Triggers_TriggerDlgGetSelectedEntryParam(Context->ActionCtx.GenCreaturesList);
			if (Context->ActionCtx.TempAction.param1 == 0) goto NoCreatureSelected;
			Context->ActionCtx.TempAction.param2 = SendDlgItemMessage(hDlg,301,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			Result = 1;
			break;

		case ACTION_PORTALS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_MOODS:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_TIMERSPEECH:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_CAMERAPATH:
			Context->ActionCtx.TempAction.param1 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Context->ActionCtx.TempAction.param2 = Misc_GetArea(hDlg,311,FALSE);
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param3 = 0;
			else Context->ActionCtx.TempAction.param3 = 1;
			Result = 1;
			break;

		case ACTION_SYSTEMMESSAGES:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_MUSICLEVEL:
			Context->ActionCtx.TempAction.param1 = SendDlgItemMessage(hDlg,500,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
			Result = 1;
			break;

		case ACTION_ATTACK:
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param1 = 1;
			else Context->ActionCtx.TempAction.param1 = 0;
			Result = 1;
			break;

		case ACTION_SHOWHEALTH:
			Context->ActionCtx.TempAction.param1 = GetDlgItemInt(hDlg,307,NULL,FALSE);
			Result = 1;
			break;

		case ACTION_TOGGLEEFFECT:
			Context->ActionCtx.TempAction.param1 = Misc_GetEffect(hDlg,311,FALSE);
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) Context->ActionCtx.TempAction.param2 = 1;
			else Context->ActionCtx.TempAction.param2 = 0;
			Result = 1;
			break;
		}

	return(Result);

//--- Une donnée requise n'a pas été sélectionnée ---

NoElementSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoElementSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoPlayerSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoPlayerSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoCreatureSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoCreatureSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoButtonSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoButtonSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoRoomSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoRoomSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
NoBandSelected:
	Misc_MessageBoxExt(hDlg,szTriggersNoBandSelected,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(0);
}

//--- Stockage définitif des valeurs ---

int Triggers_ActionDlgParamsSave(HWND hDlg,TRIGGERSCTX *Context)
{
	TV_INSERTSTRUCT	 Parent;
	ACTION		*NewAction;
	char		*TextBuffer;
	long		 X = 1;
	long		 Y = 0;

	switch(Context->ActionCtx.TempAction.ctrl)
		{
		case ACTION_VICTORY:
		case ACTION_DEFEAT:
		case ACTION_HASCRYSTAL:
		case ACTION_POSSESSCREATURE:
		case ACTION_REMOVE:
		case ACTION_SHOW:
			Context->ActionCtx.TempAction.param1 = 0;
			Context->ActionCtx.TempAction.param2 = 0;
			Context->ActionCtx.TempAction.param3 = 0;
			Context->ActionCtx.TempAction.param4 = 0;
			Context->ActionCtx.TempAction.param5 = 0;
			Context->ActionCtx.TempAction.param6 = 0;
			Context->ActionCtx.TempAction.param7 = 0;
			Context->ActionCtx.TempAction.param8 = 0;
			break;
		}

Loop:	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) goto Error_0;
	NewAction = GlobalAlloc(GPTR,sizeof(ACTION));
	if (!NewAction) goto Error_1;

	Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM;
	Parent.DUMMYUNIONNAME.item.lParam = 0;
	Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (Parent.DUMMYUNIONNAME.item.hItem) SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);

	//--- Modifie un déclencheur ---

	if (Context->Modify)
		{
		GlobalFree(NewAction);
		CopyMemory(Context->ActionCtx.SrceAction,&Context->ActionCtx.TempAction,sizeof(ACTION));
		Parent.DUMMYUNIONNAME.item.mask = TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtActionText(Context->ActionCtx.SrceAction,TextBuffer,TextBuffer+2048);
		if (Parent.DUMMYUNIONNAME.item.lParam) SendMessage(Context->List,TVM_SETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);
		goto Done;
		}

	//--- Ajoute ou insère un déclencheur ---

	if ((Context->Insert)&&(Parent.DUMMYUNIONNAME.item.lParam))
		{
		CopyMemory(NewAction,&Context->ActionCtx.TempAction,sizeof(ACTION));
		NewAction->id = Triggers_GetActionFreeId();
		Parent.hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
		Parent.hInsertAfter = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
		if (!Parent.hInsertAfter) Parent.hInsertAfter = TVI_FIRST;
		Parent.DUMMYUNIONNAME.item.hItem = NULL;
		Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtActionText(NewAction,TextBuffer,TextBuffer+2048);
		Parent.DUMMYUNIONNAME.item.iImage = 1;
		Parent.DUMMYUNIONNAME.item.iSelectedImage = 1;
		Parent.DUMMYUNIONNAME.item.cChildren = 0;
		Parent.DUMMYUNIONNAME.item.lParam = (LPARAM)NewAction;
		Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Parent);
		if (!Parent.DUMMYUNIONNAME.item.hItem) goto Error_2;
		}
	else
		{
		CopyMemory(NewAction,&Context->ActionCtx.TempAction,sizeof(ACTION));
		NewAction->id = Triggers_GetActionFreeId();
		if (Parent.DUMMYUNIONNAME.item.lParam)
			{
			if (((NODE *)Parent.DUMMYUNIONNAME.item.lParam)->type == TYPE_TRIGGER)
				Parent.hParent = Parent.DUMMYUNIONNAME.item.hItem;
			else
				Parent.hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);			Parent.hInsertAfter = TVI_LAST;
			}
		else
			{
			Parent.hParent = TVI_ROOT;
			Parent.hInsertAfter = TVI_LAST;
			}
		Parent.DUMMYUNIONNAME.item.hItem = NULL;
		Parent.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtActionText(NewAction,TextBuffer,TextBuffer+2048);
		Parent.DUMMYUNIONNAME.item.iImage = 1;
		Parent.DUMMYUNIONNAME.item.iSelectedImage = 1;
		Parent.DUMMYUNIONNAME.item.cChildren = 0;
		Parent.DUMMYUNIONNAME.item.lParam = (LPARAM)NewAction;
		Parent.DUMMYUNIONNAME.item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Parent);
		if (!Parent.DUMMYUNIONNAME.item.hItem) goto Error_2;
		}

	//--- Lie le déclencheur aux entrée précédentes ---

	List_AddEntry((NODE *)NewAction,&MapActions);
	if (!Triggers_SelectLink(NewAction,Context->ActionCtx.ParentPoint,Parent.DUMMYUNIONNAME.item.hItem,Context)) goto Error_3;

	#if (DEBUG_EVENTS)
	Parent.DUMMYUNIONNAME.item.mask = TVIF_TEXT;
	Parent.DUMMYUNIONNAME.item.pszText = Triggers_FmtActionText(NewAction,TextBuffer,TextBuffer+2048);
	SendMessage(Context->List,TVM_SETITEM,(WPARAM)0,(LPARAM)&Parent.DUMMYUNIONNAME.item);
	#endif

//--- Terminé... ----

Done:	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
	GlobalFree(TextBuffer);
	MapChanges++;

	if ((Context->ActionCtx.ChangeRect)&&(Context->ActionCtx.TempAction.ctrl == ACTION_CHANGESQUARE))
		{
		if (Context->ActionCtx.FullRect.x1+X == Context->ActionCtx.FullRect.x2+1)
			{
			X = 0;
			Y++;
			}
		if (Context->ActionCtx.FullRect.y1+Y != Context->ActionCtx.FullRect.y2+1)
			{
			Context->ActionCtx.TempAction.param5 = Context->ActionCtx.FullRect.x1+X+1;
			Context->ActionCtx.TempAction.param7 = Context->ActionCtx.FullRect.y1+Y+1;
			X++;
			goto Loop;
			}

		Context->ActionCtx.TempAction.param5 = Context->ActionCtx.FullRect.x1+1;
		Context->ActionCtx.TempAction.param7 = Context->ActionCtx.FullRect.y1+1;
		Context->ActionCtx.ChangeRect = FALSE;
		}

	return(1);

//--- Erreurs ---

Error_3:List_RemEntry((NODE *)NewAction);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Parent.DUMMYUNIONNAME.item.hItem);
Error_2:GlobalFree(NewAction);
Error_1:GlobalFree(TextBuffer);
Error_0:Misc_MessageBoxExt(hDlg,szTriggersNewActionErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
