
//<<>-<>>---------------------------------------------------------------------()
/*
	Evènements
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
extern LONG		MapChanges;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern NODE		MapAreas;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification des évènements					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Triggers_Select(HWND hWnd)
{
	TRIGGERSCTX	*Context;
	int		 Result;

	Context = GlobalAlloc(GPTR,sizeof(TRIGGERSCTX));
	if (!Context)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	Context->Triggers = &MapTriggers;
	Context->Actions = &MapActions;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(7000),hWnd,Triggers_SelectProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	Files_SaveUpdate();
	GlobalFree(Context);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de la boîte de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Triggers_SelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Triggers_SelectInit(hDlg,(TRIGGERSCTX *)lParam);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,16);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,16,1,0);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				switch(((DRAWITEMSTRUCT *)lParam)->CtlID)
					{
					case 110:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->UpIcon);
						break;
					case 111:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->DownIcon);
						break;
					case 140:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->UpIcon);
						break;
					case 141:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->DownIcon);
						break;
					case 142:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->LeftIcon);
						break;
					case 143:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->RightIcon);
						break;
					}
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_KEYDOWN:
					switch(((LV_KEYDOWN *)lParam)->wVKey)
						{
						case VK_DELETE:
							Triggers_SelectKillPoint(hDlg,Context);
							break;
						case VK_INSERT:
							if (!SendMessage(Context->Points,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) break;
							Context->Modify = FALSE;
							Context->Insert = TRUE;
							Triggers_PointNew(hDlg,Context);
							break;
						}
					return(TRUE);
				case TVN_KEYDOWN:
					switch(((TV_KEYDOWN *)lParam)->wVKey)
						{
						case VK_DELETE:
							Triggers_SelectKill(hDlg,Context);
							break;
						case VK_INSERT:
							if (!SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT)) break;
							Context->Root = FALSE;
							Context->Modify = FALSE;
							Context->Insert = TRUE;
							Triggers_TriggerNew(hDlg,Context);
							break;
						}
					return(TRUE);
				case TVN_SELCHANGED:
					Triggers_SelectChanged(hDlg,(NM_TREEVIEW *)lParam,Context);
					return(TRUE);
				case TVN_ITEMEXPANDED:
					((TRIGGER *)((NM_TREEVIEW *)lParam)->itemNew.lParam)->expanded = (short)((((NM_TREEVIEW *)lParam)->itemNew.state&TVIS_EXPANDED)?1:0);
					return(TRUE);
				case LVN_ITEMCHANGED:
					Triggers_SelectPointChanged(hDlg,(NM_LISTVIEW *)lParam,Context);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 103:
							Context->Modify = FALSE;
							Context->Insert = FALSE;
							Triggers_PointNew(hDlg,Context);
							break;
						case 104:
							Context->Modify = FALSE;
							Context->Insert = TRUE;
							Triggers_PointNew(hDlg,Context);
							break;
						case 105:
							Context->Modify = TRUE;
							Context->Insert = FALSE;
							Triggers_PointNew(hDlg,Context);
							break;
						case 106:
							Triggers_SelectKillPoint(hDlg,Context);
							break;
						case 110:
							Triggers_SelectMovePtUp(hDlg,Context);
							break;
						case 111:
							Triggers_SelectMovePtDown(hDlg,Context);
							break;

						case 122:
							Context->Root = TRUE;
							Context->Modify = FALSE;
							Context->Insert = FALSE;
							Triggers_TriggerNew(hDlg,Context);
							break;
						case 123:
							Context->Root = FALSE;
							Context->Modify = FALSE;
							Context->Insert = FALSE;
							Triggers_TriggerNew(hDlg,Context);
							break;
						case 124:
							Context->Root = FALSE;
							Context->Modify = FALSE;
							Context->Insert = TRUE;
							Triggers_TriggerNew(hDlg,Context);
							break;
						case 125:
							Context->Root = FALSE;
							Context->Modify = TRUE;
							Context->Insert = FALSE;
							Triggers_TriggerNew(hDlg,Context);
							break;
						case 126:
							Triggers_SelectKill(hDlg,Context);
							break;

						case 127:
							Context->Modify = FALSE;
							Context->Insert = FALSE;
							Triggers_ActionNew(hDlg,Context);
							break;
						case 128:
							Context->Modify = FALSE;
							Context->Insert = TRUE;
							Triggers_ActionNew(hDlg,Context);
							break;
						case 129:
							Context->Modify = TRUE;
							Context->Insert = FALSE;
							Triggers_ActionNew(hDlg,Context);
							break;
						case 130:
							Triggers_SelectKill(hDlg,Context);
							break;

						case 140:
							Triggers_SelectMoveUp(hDlg,Context);
							break;
						case 141:
							Triggers_SelectMoveDown(hDlg,Context);
							break;
						case 142:
							Triggers_SelectMoveLeft(hDlg,Context);
							break;
						case 143:
							Triggers_SelectMoveRight(hDlg,Context);
							break;

						case 153:
							Triggers_Variables(hDlg,Context);
							break;

						case IDCANCEL:
						case IDOK:
							if (Context->UpIcon) DestroyIcon(Context->UpIcon);
							if (Context->DownIcon) DestroyIcon(Context->DownIcon);
							if (Context->LeftIcon) DestroyIcon(Context->LeftIcon);
							if (Context->RightIcon) DestroyIcon(Context->RightIcon);
							SendMessage(Context->Points,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)0);
							SendMessage(Context->List,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)0);
							Misc_DestroyImageList(&Context->ImageList);
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Triggers_SelectInit(HWND hDlg, TRIGGERSCTX *Context)
{
	RECT		 ClientRect;
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	TRIGGERLINK	*ActionPt;
	LONG		 X,Y;
	char		*Text;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->Points = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->Points) goto Error_0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->Points,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	GetWindowRect(GetDlgItem(hDlg,102),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->List = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->List) goto Error_0;
	if (!Misc_CreateImageList(TrImages,&Context->ImageList,16,16,FALSE)) goto Error_0;
	SendMessage(Context->List,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)Context->ImageList);
	SendMessage(Context->Points,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->ImageList);

	Context->UpIcon = LoadImage(hInstance,MAKEINTRESOURCE(50),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	Context->DownIcon = LoadImage(hInstance,MAKEINTRESOURCE(51),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	Context->LeftIcon = LoadImage(hInstance,MAKEINTRESOURCE(52),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	Context->RightIcon = LoadImage(hInstance,MAKEINTRESOURCE(53),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->UpIcon || !Context->DownIcon || !Context->LeftIcon || !Context->RightIcon) goto Error_1;

	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text) goto Error_1;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = Text;

	for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
		{
		if (Triggers_SelectAddPoint(ActionPt,&Item,Text,Context) == -1) goto Error_2;
		Item.iItem++;
		}

	if (!Triggers_SelectBuildList(Context)) goto Error_2;

	GlobalFree(Text);
	return;

//--- Erreurs ---

Error_2:GlobalFree(Text);
Error_1:if (Context->UpIcon) DestroyIcon(Context->UpIcon);
	if (Context->DownIcon) DestroyIcon(Context->DownIcon);
	if (Context->LeftIcon) DestroyIcon(Context->LeftIcon);
	if (Context->RightIcon) DestroyIcon(Context->RightIcon);
	SendMessage(Context->Points,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)0);
	SendMessage(Context->List,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)0);
	Misc_DestroyImageList(&Context->ImageList);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Stockage d'un point d'action dans la liste ««««««««««««««««««««««»

int Triggers_SelectAddPoint(TRIGGERLINK *ActionPt, LV_ITEM *Item, char *Text, TRIGGERSCTX *Context)
{
	MAPRECTEX	*Area;
	MAPTHING	*Thing;
	MTHBAND		*Band;
	MAPTHINGHERO	*Hero;

	switch(ActionPt->node.type)
		{
		case SCOPE_LEVEL:
			Item->iImage = 9;
			strcpy(Text,szTriggersLevel);
			break;
		case SCOPE_PLAYER:
			Item->iImage = 2;
			strcpy(Text,Players[ActionPt->player-1].Name);
			break;
		case SCOPE_BAND:
			Item->iImage = 5;
			Band = (MTHBAND *)Triggers_GetStructureFromUniqueID(ActionPt->band,SCOPE_BAND);
			if (Band)
				{
				if (Band->name[0]) wsprintf(Text,szTriggersBandInfo,Band->id,Band->name);
				else wsprintf(Text,szTriggersBand,Band->id);
				}
			else
				strcpy(Text,szTriggersBandBad);
			break;
		case SCOPE_CREATUREINBAND:
			Item->iImage = 3;
			Hero = (MAPTHINGHERO *)Triggers_GetStructureFromUniqueID(ActionPt->creature,SCOPE_CREATUREINBAND);
			if (Hero)
				{
				if (!Hero->enemy) wsprintf(Text,szTriggersCreatureInBandNoEnemy,ActionPt->band,Things_GetCreatureNameFromID(Hero->id),Hero->level);
				else wsprintf(Text,szTriggersCreatureInBandInfo,ActionPt->band,Things_GetCreatureNameFromID(Hero->id),Hero->level,Players[Hero->enemy-1]);
				}
			else
				wsprintf(Text,szTriggersCreatureInBandBad,ActionPt->band);
			break;
		case SCOPE_CREATURE:
			Item->iImage = 3;
			Thing = (MAPTHING *)Triggers_GetStructureFromUniqueID(ActionPt->creature,SCOPE_CREATURE);
			if (Thing)
				wsprintf(Text,szTriggersCreatureInfo,Things_GetCreatureNameFromID(Thing->id),Thing->data.level,Players[Thing->owner-1].Name,Thing->x,Thing->y);
			else
				strcpy(Text,szTriggersCreatureBad);
			break;
		case SCOPE_DOOR:
			Item->iImage = 6;
			Thing = (MAPTHING *)Triggers_GetStructureFromUniqueID(ActionPt->door,SCOPE_DOOR);
			if (Thing)
				wsprintf(Text,szTriggersDoorInfo,Things_GetThingNameFromID(Thing->id,Doors),Thing->x,Thing->y);
			else
				strcpy(Text,szTriggersDoorBad);
			break;
		case SCOPE_OBJECT:
			Item->iImage = 7;
			Thing = (MAPTHING *)Triggers_GetStructureFromUniqueID(ActionPt->object,SCOPE_OBJECT);
			if (Thing)
				wsprintf(Text,szTriggersObjectInfo,Things_GetThingNameFromID(Thing->id,Objects),Thing->x,Thing->y);
			else
				strcpy(Text,szTriggersObjectBad);
			break;
		case SCOPE_MAGIC:
			Item->iImage = 8;
			Thing = (MAPTHING *)Triggers_GetStructureFromUniqueID(ActionPt->object,SCOPE_MAGIC);
			if (Thing)
				wsprintf(Text,szTriggersMagicalObjectInfo,Things_GetThingNameFromID(Thing->id,Magics),Thing->x,Thing->y);
			else
				strcpy(Text,szTriggersMagicalObjectBad);
			break;
		case SCOPE_AREA:
			Item->iImage = 4;
			Area = RectEx_FindById(ActionPt->area,&MapAreas);
			if (Area)
				{
				RectEx_StrCpy(Text+2048,Area->name);
				RectEx_StrCpy(Text+2048+MAP_RECTMAXNAME,Area->info);
				}
			if ((Area)&&(Area->info[0])) wsprintf(Text,szTriggersAreaInfo,Area->id,Text+2048,Text+2048+MAP_RECTMAXNAME);
			else if (Area) wsprintf(Text,szTriggersArea,Area->id,Text+2048);
			else wsprintf(Text,szTriggersAreaBad,ActionPt->area);
			break;
		}

	Item->lParam = (LPARAM)ActionPt;
	return(SendMessage(Context->Points,LVM_INSERTITEM,(WPARAM)0,(LPARAM)Item));
}


// «»»» Changement de sélection dans la liste des pts «««««««««««««««««««»

void Triggers_SelectPointChanged(HWND hDlg, NM_LISTVIEW *ListView, TRIGGERSCTX *Context)
{
	LONG		lData;

	if (!(ListView->uChanged&LVIF_STATE)) return;

	if (!(ListView->uNewState&LVIS_SELECTED))
		{
		SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
		SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)TVI_ROOT);
		for (lData = 104; lData != 106+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);
		EnableWindow(GetDlgItem(hDlg,110),FALSE);
		EnableWindow(GetDlgItem(hDlg,111),FALSE);
		for (lData = 122; lData != 130+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);
		for (lData = 140; lData != 143+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);
		SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
		InvalidateRect(Context->List,NULL,FALSE);
		}
	else if (ListView->uNewState&LVIS_SELECTED)
		{
		LV_ITEM	Item;

		SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);

		EnableWindow(GetDlgItem(hDlg,104),TRUE);
		EnableWindow(GetDlgItem(hDlg,105),TRUE);
		EnableWindow(GetDlgItem(hDlg,106),TRUE);
		EnableWindow(GetDlgItem(hDlg,110),FALSE);
		EnableWindow(GetDlgItem(hDlg,111),FALSE);
		EnableWindow(GetDlgItem(hDlg,122),TRUE);
		Item.mask = LVIF_PARAM;
		Item.iItem = ListView->iItem;
		Item.iSubItem = 0;
		if (SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item))
		if (Item.lParam)
			{
			if (((NODE *)Item.lParam)->prev->prev) EnableWindow(GetDlgItem(hDlg,110),TRUE);
			if (((NODE *)Item.lParam)->next) EnableWindow(GetDlgItem(hDlg,111),TRUE);
			Triggers_SelectBuildList(Context);
			}

		SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
		InvalidateRect(Context->List,NULL,FALSE);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création de la liste des conditions & actions			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Triggers_SelectBuildList(TRIGGERSCTX *Context)
{
	LV_ITEM		 Item;
	TRIGGER		*NewTrigger;
	char		*Buffer;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	if (Item.iItem == -1) return(1);
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(1);
	if (!Item.lParam) return(0);

	Buffer = GlobalAlloc(GPTR,2048+2048);
	if (!Buffer) return(0);

	NewTrigger = Triggers_GetTriggerById(((TRIGGERLINK *)Item.lParam)->trigger);
	if (!NewTrigger)
		{
		GlobalFree(Buffer);
		return(0);
		}
	if (!Triggers_SelectCreateNewTrigger(NewTrigger,Context->List,TVI_ROOT,TVI_LAST,NULL,Buffer))
		{
		MessageBeep(MB_ICONHAND);
		GlobalFree(Buffer);
		return(0);
		}

	GlobalFree(Buffer);
	return(1);
}


// «»»» Création d'un nouveau déclencheur «««««««««««««««««««««««««««««««»

int Triggers_SelectCreateNewTrigger(TRIGGER *Trigger, HWND hList, HTREEITEM hParent, HTREEITEM hInsertAfter, HTREEITEM *hItemPtr, char *TextBuffer)
{
	TRIGGER		*NewTrigger;
	ACTION		*NewAction;
	TV_INSERTSTRUCT	 Insert;
	HTREEITEM	 hItem;

	Insert.hParent = hParent;
	Insert.hInsertAfter = hInsertAfter;
	Insert.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_STATE;
	Insert.DUMMYUNIONNAME.item.pszText = Triggers_FmtTriggerText(Trigger,TextBuffer,TextBuffer+2048);
	Insert.DUMMYUNIONNAME.item.state = (Trigger->expanded)?TVIS_EXPANDED:0;
	Insert.DUMMYUNIONNAME.item.stateMask = TVIS_EXPANDED;
	Insert.DUMMYUNIONNAME.item.iImage = 0;
	Insert.DUMMYUNIONNAME.item.iSelectedImage = 0;
	Insert.DUMMYUNIONNAME.item.cChildren = 1;
	Insert.DUMMYUNIONNAME.item.lParam = (LPARAM)Trigger;
	hItem = (HTREEITEM)SendMessage(hList,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Insert);
	if (!hItem) return(0);
	if (hItemPtr) *hItemPtr = hItem;

	if (Trigger->and > 0)
		{
		NewTrigger = Triggers_GetTriggerById(Trigger->and);
		if (!NewTrigger) return(0);
		if (!Triggers_SelectCreateNewTrigger(NewTrigger,hList,hItem,TVI_LAST,NULL,TextBuffer)) return(0);
		}
	else if (Trigger->and < 0)
		{
		NewAction = Triggers_GetActionById(Trigger->and);
		if (!NewAction) return(0);
		if (!Triggers_SelectCreateNewAction(NewAction,hList,hItem,TVI_LAST,NULL,TextBuffer)) return(0);
		}

	if (Trigger->or > 0)
		{
		NewTrigger = Triggers_GetTriggerById(Trigger->or);
		if (!NewTrigger) return(0);
		if (!Triggers_SelectCreateNewTrigger(NewTrigger,hList,hParent,TVI_LAST,NULL,TextBuffer)) return(0);
		}
	else if (Trigger->or < 0)
		{
		NewAction = Triggers_GetActionById(Trigger->or);
		if (!NewAction) return(0);
		if (!Triggers_SelectCreateNewAction(NewAction,hList,hParent,TVI_LAST,NULL,TextBuffer)) return(0);
		}

	return(1);
}


// «»»» Création d'une nouvelle action ««««««««««««««««««««««««««««««««««»

int Triggers_SelectCreateNewAction(ACTION *Action, HWND hList, HTREEITEM hParent, HTREEITEM hInsertAfter, HTREEITEM *hItemPtr, char *TextBuffer)
{
	TRIGGER		*NewTrigger;
	ACTION		*NewAction;
	TV_INSERTSTRUCT	 Insert;
	HTREEITEM	 hItem;

	Insert.hParent = hParent;
	Insert.hInsertAfter = hInsertAfter;
	Insert.DUMMYUNIONNAME.item.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	Insert.DUMMYUNIONNAME.item.pszText = Triggers_FmtActionText(Action,TextBuffer,TextBuffer+2048);
	Insert.DUMMYUNIONNAME.item.iImage = 1;
	Insert.DUMMYUNIONNAME.item.iSelectedImage = 1;
	Insert.DUMMYUNIONNAME.item.cChildren = 0;
	Insert.DUMMYUNIONNAME.item.lParam = (LPARAM)Action;
	hItem = (HTREEITEM)SendMessage(hList,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Insert);
	if (!hItem) return(0);
	if (hItemPtr) *hItemPtr = hItem;

	if (Action->next > 0)
		{
		NewTrigger = Triggers_GetTriggerById(Action->next);
		if (!NewTrigger) return(0);
		if (!Triggers_SelectCreateNewTrigger(NewTrigger,hList,hParent,TVI_LAST,NULL,TextBuffer)) return(0);
		}
	else if (Action->next < 0)
		{
		NewAction = Triggers_GetActionById(Action->next);
		if (!NewAction) return(0);
		if (!Triggers_SelectCreateNewAction(NewAction,hList,hParent,TVI_LAST,NULL,TextBuffer)) return(0);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des points d'actions					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Suppression d'un point d'action «««««««««««««««««««««««««««««««««»

void Triggers_SelectKillPoint(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_ITEM			 Item;
	MESSAGEBOXSTRUCT	 Request;
	void			*Struct;
	long			 lData;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	Request.gadgets = szMessageBoxExtYesNo;
	Request.windowtext = (((TRIGGERLINK *)Item.lParam)->trigger)?szTriggersKillPointAll:szTriggersKillPoint;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = REQUEST_KILLPOINT;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_CENTER;
	if (!Misc_MessageBoxExtArgs(hDlg,&Request)) return;

	for (lData = 104; lData != 106+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);
	EnableWindow(GetDlgItem(hDlg,110),FALSE);
	EnableWindow(GetDlgItem(hDlg,111),FALSE);
	for (lData = 122; lData != 130+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);
	for (lData = 140; lData != 143+1; lData++) EnableWindow(GetDlgItem(hDlg,lData),FALSE);

	Triggers_KillFromId(((TRIGGERLINK *)Item.lParam)->trigger);
	switch(((TRIGGERLINK *)Item.lParam)->node.type)
		{
		case SCOPE_LEVEL:
		case SCOPE_PLAYER:
		case SCOPE_AREA:
			break;
		case SCOPE_CREATURE:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->creature,SCOPE_CREATURE);
			if (Struct) ((MAPTHING *)Struct)->uniqueid = 0;
			break;
		case SCOPE_DOOR:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->door,SCOPE_DOOR);
			if (Struct) ((MAPTHING *)Struct)->uniqueid = 0;
			break;
		case SCOPE_OBJECT:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->object,SCOPE_OBJECT);
			if (Struct) ((MAPTHING *)Struct)->uniqueid = 0;
			break;
		case SCOPE_MAGIC:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->object,SCOPE_MAGIC);
			if (Struct) ((MAPTHING *)Struct)->uniqueid = 0;
			break;
		case SCOPE_BAND:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->band,SCOPE_BAND);
			if (Struct) ((MTHBAND *)Struct)->uniqueid = 0;
			break;
		case SCOPE_CREATUREINBAND:
			Struct = Triggers_GetStructureFromUniqueID(((TRIGGERLINK *)Item.lParam)->creature,SCOPE_CREATUREINBAND);
			if (Struct) ((MAPTHINGHERO *)Struct)->uniqueid = 0;
			break;
		}
	List_RemEntry((NODE *)Item.lParam);
	GlobalFree((void *)Item.lParam);

	SendMessage(Context->Points,LVM_DELETEITEM,(WPARAM)Item.iItem,(LPARAM)0);
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)TVI_ROOT);
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(Context->List,NULL,FALSE);
	MapChanges++;
	return;
}


// «»»» Déplace un point d'action vers le haut ««««««««««««««««««««««««««»

void Triggers_SelectMovePtUp(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_ITEM		 Item;
        NODE		*InsertAfter;

	Item.mask = TVIF_PARAM;
	Item.iItem = List_FindSelectedItem(Context->Points);
	if (Item.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	InsertAfter = ((NODE *)Item.lParam)->prev;
	List_RemEntry((NODE *)Item.lParam);
	List_InsertEntry((NODE *)Item.lParam,InsertAfter,FALSE);

	SendMessage(Context->Points,LVM_SORTITEMS,(WPARAM)0,(LPARAM)List_SortItems);

	if (((NODE *)Item.lParam)->prev->prev) EnableWindow(GetDlgItem(hDlg,110),TRUE);
	else EnableWindow(GetDlgItem(hDlg,110),FALSE);
	if (((NODE *)Item.lParam)->next) EnableWindow(GetDlgItem(hDlg,111),TRUE);
	else EnableWindow(GetDlgItem(hDlg,111),FALSE);

	Item.iItem = List_FindSelectedItem(Context->Points);
	SendMessage(Context->Points,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)0);
	MapChanges++;
	return;
}


// «»»» Déplace un point d'action vers le bas «««««««««««««««««««««««««««»

void Triggers_SelectMovePtDown(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_ITEM		 Item;
        NODE		*InsertAfter;

	Item.mask = TVIF_PARAM;
	Item.iItem = List_FindSelectedItem(Context->Points);
	if (Item.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	InsertAfter = ((NODE *)Item.lParam)->next;
	List_RemEntry((NODE *)Item.lParam);
	List_InsertEntry((NODE *)Item.lParam,InsertAfter,TRUE);

	SendMessage(Context->Points,LVM_SORTITEMS,(WPARAM)0,(LPARAM)List_SortItems);

	if (((NODE *)Item.lParam)->prev->prev) EnableWindow(GetDlgItem(hDlg,110),TRUE);
	else EnableWindow(GetDlgItem(hDlg,110),FALSE);
	if (((NODE *)Item.lParam)->next) EnableWindow(GetDlgItem(hDlg,111),TRUE);
	else EnableWindow(GetDlgItem(hDlg,111),FALSE);

	Item.iItem = List_FindSelectedItem(Context->Points);
	SendMessage(Context->Points,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)0);
	MapChanges++;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des conditions et des actions				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Augmente la sélection d'un niveau «««««««««««««««««««««««««««««««»

void Triggers_SelectMoveRight(HWND hDlg, TRIGGERSCTX *Context)
{
	TV_ITEM		 Item;
	TV_ITEM		 Parent;
	LV_ITEM		 RootItem;
	HTREEITEM	 hParent;
	HTREEITEM	 hInsertAfter;
	HTREEITEM	 hItem;
	TRIGGERLINK	*Root;
	char		*TextBuffer;

	//--- Retrouve l'entrée sélectionnée et l'entrée parente précédente ---

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	hParent = Item.hItem;
	while(1)
		{
		hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)hParent);
		if (!hParent) return;
		Parent.mask = TVIF_PARAM;
		Parent.hItem = hParent;
		if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Parent)) return;
		if (!Parent.lParam) return;
		if (((NODE *)Parent.lParam)->type != TYPE_TRIGGER) continue;
		break;
		}
	hInsertAfter = TVI_FIRST;

	//--- Retrouve la structure principale (point d'action) ---

	RootItem.mask = LVIF_PARAM;
	RootItem.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	RootItem.iSubItem = 0;
	if (RootItem.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&RootItem)) return;
	if (!RootItem.lParam) return;
	Root = (TRIGGERLINK *)RootItem.lParam;

	//--- Supprime les liens ---

	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) return;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_UnlinkTrigger((TRIGGER *)Item.lParam);
			((TRIGGER *)Item.lParam)->or = 0;
			((TRIGGER *)Item.lParam)->previd = 0;
			break;
		case TYPE_ACTION:
			Triggers_UnlinkAction((ACTION *)Item.lParam);
			((ACTION *)Item.lParam)->next = 0;
			((ACTION *)Item.lParam)->previd = 0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);

	//--- Restructure les liens ---

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_SelectCreateNewTrigger((TRIGGER *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((TRIGGER *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		case TYPE_ACTION:
			Triggers_SelectCreateNewAction((ACTION *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((ACTION *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)hItem);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	return;

//--- Erreurs (mauvais) ---

Error_0:switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_KillFromId(((TRIGGER *)Item.lParam)->id);
			break;
		case TYPE_ACTION:
			Triggers_KillFromId(((ACTION *)Item.lParam)->id);
			break;
		}
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	Misc_MessageBoxExt(hDlg,szTriggersMoveErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// «»»» Baisse la sélection d'un niveau «««««««««««««««««««««««««««««««««»

void Triggers_SelectMoveLeft(HWND hDlg, TRIGGERSCTX *Context)
{
	TV_ITEM		 Item;
	LV_ITEM		 RootItem;
	HTREEITEM	 hParent;
	HTREEITEM	 hInsertAfter;
	HTREEITEM	 hItem;
	TRIGGERLINK	*Root;
	char		*TextBuffer;

	//--- Retrouve l'entrée sélectionnée et l'entrée parente précédente ---

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	hInsertAfter = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Item.hItem);
	if (!hInsertAfter) return;
	hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hInsertAfter);
	if (!hParent) hParent = TVI_ROOT;
	hInsertAfter = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)hInsertAfter);
	if (!hInsertAfter) hInsertAfter = TVI_FIRST;

	//--- Retrouve la structure principale (point d'action) ---

	RootItem.mask = LVIF_PARAM;
	RootItem.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	RootItem.iSubItem = 0;
	if (RootItem.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&RootItem)) return;
	if (!RootItem.lParam) return;
	Root = (TRIGGERLINK *)RootItem.lParam;

	//--- Supprime les liens ---

	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) return;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_UnlinkTrigger((TRIGGER *)Item.lParam);
			((TRIGGER *)Item.lParam)->or = 0;
			((TRIGGER *)Item.lParam)->previd = 0;
			break;
		case TYPE_ACTION:
			Triggers_UnlinkAction((ACTION *)Item.lParam);
			((ACTION *)Item.lParam)->next = 0;
			((ACTION *)Item.lParam)->previd = 0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);

	//--- Restructure les liens ---

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_SelectCreateNewTrigger((TRIGGER *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((TRIGGER *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		case TYPE_ACTION:
			Triggers_SelectCreateNewAction((ACTION *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((ACTION *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)hItem);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	return;

//--- Erreurs (mauvais) ---

Error_0:switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_KillFromId(((TRIGGER *)Item.lParam)->id);
			break;
		case TYPE_ACTION:
			Triggers_KillFromId(((ACTION *)Item.lParam)->id);
			break;
		}
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	Misc_MessageBoxExt(hDlg,szTriggersMoveErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// «»»» Fait monter la sélection d'un cran ««««««««««««««««««««««««««««««»

void Triggers_SelectMoveUp(HWND hDlg, TRIGGERSCTX *Context)
{
	TV_ITEM		 Item;
	LV_ITEM		 RootItem;
	HTREEITEM	 hPrevItem;
	HTREEITEM	 hParent;
	HTREEITEM	 hInsertAfter;
	HTREEITEM	 hItem;
	TRIGGERLINK	*Root;
	char		*TextBuffer;

	//--- Retrouve l'entrée sélectionnée et l'entrée précédente ---

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	hPrevItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Item.hItem);
	if (!hPrevItem) return;

	//--- Retrouve la structure principale (point d'action) ---

	RootItem.mask = LVIF_PARAM;
	RootItem.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	RootItem.iSubItem = 0;
	if (RootItem.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&RootItem)) return;
	if (!RootItem.lParam) return;
	Root = (TRIGGERLINK *)RootItem.lParam;

	//--- Supprime les liens ---

	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) return;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_UnlinkTrigger((TRIGGER *)Item.lParam);
			((TRIGGER *)Item.lParam)->or = 0;
			((TRIGGER *)Item.lParam)->previd = 0;
			break;
		case TYPE_ACTION:
			Triggers_UnlinkAction((ACTION *)Item.lParam);
			((ACTION *)Item.lParam)->next = 0;
			((ACTION *)Item.lParam)->previd = 0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);

	//--- Restructure les liens ---

	hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hPrevItem);
	hInsertAfter = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)hPrevItem);
	if (!hInsertAfter) hInsertAfter = TVI_FIRST;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_SelectCreateNewTrigger((TRIGGER *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((TRIGGER *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		case TYPE_ACTION:
			Triggers_SelectCreateNewAction((ACTION *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((ACTION *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)hItem);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	return;

//--- Erreurs (mauvais) ---

Error_0:switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_KillFromId(((TRIGGER *)Item.lParam)->id);
			break;
		case TYPE_ACTION:
			Triggers_KillFromId(((ACTION *)Item.lParam)->id);
			break;
		}
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	Misc_MessageBoxExt(hDlg,szTriggersMoveErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// «»»» Fait descendre la sélection d'un cran «««««««««««««««««««««««««««»

void Triggers_SelectMoveDown(HWND hDlg, TRIGGERSCTX *Context)
{
	TV_ITEM		 Item;
	LV_ITEM		 RootItem;
	HTREEITEM	 hItem;
	HTREEITEM	 hNextItem;
	HTREEITEM	 hParent;
	HTREEITEM	 hInsertAfter;
	TRIGGERLINK	*Root;
	char		*TextBuffer;

	//--- Retrouve l'entrée sélectionnée et l'entrée précédente ---

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	hNextItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)Item.hItem);
	if (!hNextItem) return;

	//--- Retrouve la structure principale (point d'action) ---

	RootItem.mask = LVIF_PARAM;
	RootItem.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	RootItem.iSubItem = 0;
	if (RootItem.iItem == -1) return;
	if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&RootItem)) return;
	if (!RootItem.lParam) return;
	Root = (TRIGGERLINK *)RootItem.lParam;

	//--- Supprime les liens ---

	TextBuffer = GlobalAlloc(GPTR,4096);
	if (!TextBuffer) return;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_UnlinkTrigger((TRIGGER *)Item.lParam);
			((TRIGGER *)Item.lParam)->or = 0;
			((TRIGGER *)Item.lParam)->previd = 0;
			break;
		case TYPE_ACTION:
			Triggers_UnlinkAction((ACTION *)Item.lParam);
			((ACTION *)Item.lParam)->next = 0;
			((ACTION *)Item.lParam)->previd = 0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);

	//--- Restructure les liens ---

	hParent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hNextItem);
	hInsertAfter = hNextItem;

	switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_SelectCreateNewTrigger((TRIGGER *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((TRIGGER *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		case TYPE_ACTION:
			Triggers_SelectCreateNewAction((ACTION *)Item.lParam,Context->List,hParent,hInsertAfter,&hItem,TextBuffer);
			if (!Triggers_SelectLink((ACTION *)Item.lParam,Root,hItem,Context)) goto Error_0;
			break;
		}

	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	SendMessage(Context->List,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)hItem);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	return;

//--- Erreurs (mauvais) ---

Error_0:switch(((NODE *)Item.lParam)->type)
		{
		case TYPE_TRIGGER:
			Triggers_KillFromId(((TRIGGER *)Item.lParam)->id);
			break;
		case TYPE_ACTION:
			Triggers_KillFromId(((ACTION *)Item.lParam)->id);
			break;
		}
	SendMessage(Context->List,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(Context->List,NULL,FALSE);
	GlobalFree(TextBuffer);
	MapChanges++;
	Misc_MessageBoxExt(hDlg,szTriggersMoveErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// «»»» Changement de sélection «««««««««««««««««««««««««««««««««««««««««»

void Triggers_SelectChanged(HWND hDlg, NM_TREEVIEW *TreeView, TRIGGERSCTX *Context)
{
	TV_ITEM		Item;
	HTREEITEM	Parent;

	if (!(TreeView->itemNew.state&TVIS_SELECTED))
		{
		EnableWindow(GetDlgItem(hDlg,123),FALSE);
		EnableWindow(GetDlgItem(hDlg,124),FALSE);
		EnableWindow(GetDlgItem(hDlg,125),FALSE);
		EnableWindow(GetDlgItem(hDlg,126),FALSE);
		EnableWindow(GetDlgItem(hDlg,127),FALSE);
		EnableWindow(GetDlgItem(hDlg,128),FALSE);
		EnableWindow(GetDlgItem(hDlg,129),FALSE);
		EnableWindow(GetDlgItem(hDlg,130),FALSE);
		EnableWindow(GetDlgItem(hDlg,140),FALSE);
		EnableWindow(GetDlgItem(hDlg,141),FALSE);
		}

	if (TreeView->itemNew.state&TVIS_SELECTED)
		{
		if (!TreeView->itemNew.lParam) return;
		EnableWindow(GetDlgItem(hDlg,123),TRUE);
		EnableWindow(GetDlgItem(hDlg,124),TRUE);
		switch(((NODE *)TreeView->itemNew.lParam)->type)
			{
			case TYPE_TRIGGER:
				EnableWindow(GetDlgItem(hDlg,125),TRUE);
				EnableWindow(GetDlgItem(hDlg,126),TRUE);
				EnableWindow(GetDlgItem(hDlg,127),TRUE);
				Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)TreeView->itemNew.hItem);
				EnableWindow(GetDlgItem(hDlg,128),(Parent)?TRUE:FALSE);
				EnableWindow(GetDlgItem(hDlg,129),FALSE);
				EnableWindow(GetDlgItem(hDlg,130),FALSE);
				Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)TreeView->itemNew.hItem);
				EnableWindow(GetDlgItem(hDlg,142),(Parent)?TRUE:FALSE);
				break;
			case TYPE_ACTION:
				EnableWindow(GetDlgItem(hDlg,125),FALSE);
				EnableWindow(GetDlgItem(hDlg,126),FALSE);
				EnableWindow(GetDlgItem(hDlg,127),TRUE);
				EnableWindow(GetDlgItem(hDlg,128),TRUE);
				EnableWindow(GetDlgItem(hDlg,129),TRUE);
				EnableWindow(GetDlgItem(hDlg,130),TRUE);
				Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)TreeView->itemNew.hItem);
				if (Parent) Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Parent);
				EnableWindow(GetDlgItem(hDlg,142),(Parent)?TRUE:FALSE);
				break;
			}
		Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)TreeView->itemNew.hItem);
		EnableWindow(GetDlgItem(hDlg,140),(Parent)?TRUE:FALSE);
		Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)TreeView->itemNew.hItem);
		EnableWindow(GetDlgItem(hDlg,141),(Parent)?TRUE:FALSE);

		EnableWindow(GetDlgItem(hDlg,143),FALSE);
		Parent = TreeView->itemNew.hItem;
		while(1)
			{
			Parent = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)Parent);
			if (!Parent) break;
			Item.mask = TVIF_PARAM;
			Item.hItem = Parent;
			if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) break;
			if (!Item.lParam) break;
			if (((NODE *)Item.lParam)->type != TYPE_TRIGGER) continue;
			EnableWindow(GetDlgItem(hDlg,143),TRUE);
			break;
			}

		}

	return;
}


// «»»» Création des liaisons «««««««««««««««««««««««««««««««««««««««««««»

int Triggers_SelectLink(void *Struct, TRIGGERLINK *Root, HTREEITEM hItem, TRIGGERSCTX *Context)
{
	TV_ITEM		 PrevItem;

	PrevItem.mask = TVIF_PARAM;
	PrevItem.lParam = 0;
	PrevItem.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)hItem);

	switch(((NODE *)Struct)->type)
		{
		case TYPE_ACTION:
			if (PrevItem.hItem) // Previous entry is a child
				{
				SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&PrevItem);
				if (!PrevItem.lParam) return(0);
				switch(((NODE *)PrevItem.lParam)->type)
					{
					case TYPE_TRIGGER:
						((ACTION *)Struct)->next = ((TRIGGER *)PrevItem.lParam)->or;
						((TRIGGER *)PrevItem.lParam)->or = ((ACTION *)Struct)->id;
						break;
					case TYPE_ACTION:
						((ACTION *)Struct)->next = ((ACTION *)PrevItem.lParam)->next;
						((ACTION *)PrevItem.lParam)->next = ((ACTION *)Struct)->id;
						break;
					}
				}
			else
				{
				PrevItem.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hItem);
				if (PrevItem.hItem) // Previous entry is a parent (a trigger)
					{
					SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&PrevItem);
					if (!PrevItem.lParam) return(0);
					((ACTION *)Struct)->next = ((TRIGGER *)PrevItem.lParam)->and;
					((TRIGGER *)PrevItem.lParam)->and = ((ACTION *)Struct)->id;
					}
				else // Previous entry is the root of triggers (NOT POSSIBLE)
					return(0);
				}
			break;

		case TYPE_TRIGGER:
			if (PrevItem.hItem) // Previous entry is a child
				{
				SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&PrevItem);
				if (!PrevItem.lParam) return(0);
				switch(((NODE *)PrevItem.lParam)->type)
					{
					case TYPE_TRIGGER:
						((TRIGGER *)Struct)->or = ((TRIGGER *)PrevItem.lParam)->or;
						((TRIGGER *)PrevItem.lParam)->or = ((TRIGGER *)Struct)->id;
						break;
					case TYPE_ACTION:
						((TRIGGER *)Struct)->or = ((ACTION *)PrevItem.lParam)->next;
						((ACTION *)PrevItem.lParam)->next = ((TRIGGER *)Struct)->id;
						break;
					}
				}
			else
				{
				PrevItem.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hItem);
				if (PrevItem.hItem) // Previous entry is a parent (a trigger)
					{
					SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&PrevItem);
					if (!PrevItem.lParam) return(0);
					((TRIGGER *)Struct)->or = ((TRIGGER *)PrevItem.lParam)->and;
					((TRIGGER *)PrevItem.lParam)->and = ((TRIGGER *)Struct)->id;
					}
				else // Previous entry is the root of triggers
					{
					((TRIGGER *)Struct)->or = Root->trigger;
					Root->trigger = ((TRIGGER *)Struct)->id;
					}
				}
			break;
		}

	Triggers_MakeLinks();
	return(1);
}


// «»»» Suppression d'un déclencheur ou d'une action ««««««««««««««««««««»

void Triggers_SelectKill(HWND hDlg, TRIGGERSCTX *Context)
{
	TV_ITEM		Item;

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->List,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->List,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;

	if (((NODE *)Item.lParam)->type == TYPE_TRIGGER)
		Triggers_SelectKillTrigger(hDlg,(TRIGGER *)Item.lParam,Item.hItem,Context);
	else
		Triggers_SelectKillAction(hDlg,(ACTION *)Item.lParam,Item.hItem,Context);

	return;
}


// «»»» Supprime un déclencheur «««««««««««««««««««««««««««««««««««««««««»

void Triggers_SelectKillTrigger(HWND hDlg, TRIGGER *Trigger, HTREEITEM hItem, TRIGGERSCTX *Context)
{
	MESSAGEBOXSTRUCT	 Request;

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = szMessageBoxExtYesNo;
	Request.windowtext = szTriggersKillTrigger;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = REQUEST_KILLTRIGGER;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_CENTER;
	if (!Misc_MessageBoxExtArgs(hDlg,&Request)) return;

	Triggers_KillFromId(Trigger->and);
	Triggers_UnlinkTrigger(Trigger);
	List_RemEntry((NODE *)Trigger);
	GlobalFree(Trigger);
	EnableWindow(GetDlgItem(hDlg,124),FALSE);
	EnableWindow(GetDlgItem(hDlg,125),FALSE);
	EnableWindow(GetDlgItem(hDlg,126),FALSE);
	EnableWindow(GetDlgItem(hDlg,127),FALSE);
	EnableWindow(GetDlgItem(hDlg,128),FALSE);
	EnableWindow(GetDlgItem(hDlg,129),FALSE);
	EnableWindow(GetDlgItem(hDlg,130),FALSE);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)hItem);
	MapChanges++;
	return;
}


// «»»» Supprime une action «««««««««««««««««««««««««««««««««««««««««««««»

void Triggers_SelectKillAction(HWND hDlg, ACTION *Action, HTREEITEM hItem, TRIGGERSCTX *Context)
{
	MESSAGEBOXSTRUCT	 Request;

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = szMessageBoxExtYesNo;
	Request.windowtext = szTriggersKillAction;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = REQUEST_KILLACTION;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_CENTER;
	if (!Misc_MessageBoxExtArgs(hDlg,&Request)) return;

	Triggers_UnlinkAction(Action);
	List_RemEntry((NODE *)Action);
	GlobalFree(Action);
	EnableWindow(GetDlgItem(hDlg,124),FALSE);
	EnableWindow(GetDlgItem(hDlg,125),FALSE);
	EnableWindow(GetDlgItem(hDlg,126),FALSE);
	EnableWindow(GetDlgItem(hDlg,127),FALSE);
	EnableWindow(GetDlgItem(hDlg,128),FALSE);
	EnableWindow(GetDlgItem(hDlg,129),FALSE);
	EnableWindow(GetDlgItem(hDlg,130),FALSE);
	SendMessage(Context->List,TVM_DELETEITEM,(WPARAM)0,(LPARAM)hItem);
	MapChanges++;
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
