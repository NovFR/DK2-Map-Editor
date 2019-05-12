
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition des variables et des compteurs
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
extern FONTINFO		Font;
extern LONG		MapChanges;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification des variables et des compteurs			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Triggers_Variables(HWND hWnd, TRIGGERSCTX *Context)
{
	int		 Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(7001),hWnd,Triggers_VariablesProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de la boîte de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Triggers_VariablesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Triggers_VariablesInit(hDlg,(TRIGGERSCTX *)lParam);
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
				Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,16,3,(1<<0)|(1<<2));
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case NM_DBLCLK:
					Triggers_VariablesSetName(hDlg,(((NMHDR *)lParam)->hwndFrom == Context->Variables)?TRUE:FALSE,Context);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == Context->Variables)
						EnableWindow(GetDlgItem(hDlg,103),(SendMessage(Context->Variables,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0))?TRUE:FALSE);
					else
						EnableWindow(GetDlgItem(hDlg,104),(SendMessage(Context->Timers,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0))?TRUE:FALSE);
					return(TRUE);
				case LVN_GETDISPINFO:
					Triggers_VariablesSetInfo((LV_DISPINFO *)lParam);
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
							Triggers_VariablesSetName(hDlg,TRUE,Context);
							break;
						case 104:
							Triggers_VariablesSetName(hDlg,FALSE,Context);
							break;

						case IDCANCEL:
						case IDOK:
							SendMessage(Context->Variables,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							SendMessage(Context->Timers,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							ImageList_Destroy(Context->IconList);
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

void Triggers_VariablesInit(HWND hDlg, TRIGGERSCTX *Context)
{
	RECT		 ClientRect;
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	HICON		 Icon;
	LONG		 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	//--- Création des icônes ---

	Context->IconList = ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,1,1);
	if (!Context->IconList) goto Error_0;
	Icon = LoadImage(hInstance,MAKEINTRESOURCE(54),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Icon) goto Error_1;
	X = ImageList_ReplaceIcon(Context->IconList,-1,Icon);
	DestroyIcon(Icon);
	if (X == -1) goto Error_1;
	Icon = LoadImage(hInstance,MAKEINTRESOURCE(55),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Icon) goto Error_1;
	X = ImageList_ReplaceIcon(Context->IconList,-1,Icon);
	DestroyIcon(Icon);
	if (X == -1) goto Error_1;

	//--- Création de la liste des variables ---

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->Variables = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SINGLESEL|LVS_NOSORTHEADER|LVS_SHOWSELALWAYS|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->Variables) goto Error_1;
	SendMessage(Context->Variables,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->IconList);

	Column.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.iSubItem = 0;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szTriggersVariablesColumn0;
	if (SendMessage(Context->Variables,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_2;
	Column.cx = Font.FontMaxWidth*30;
	Column.pszText = szTriggersVariablesColumn2;
	if (SendMessage(Context->Variables,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_2;
	Column.cx = Font.FontMaxWidth*4;
	Column.pszText = szTriggersVariablesColumn3;
	if (SendMessage(Context->Variables,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column) == -1) goto Error_2;

	for (X = 0; X != TRIGGERS_MAXVARIABLES; X++)
		{
		Item.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
		Item.iItem = X;
		Item.iSubItem = 0;
		Item.iImage = 0;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.lParam = 0;
		if (SendMessage(Context->Variables,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}

	//--- Création de la liste des compteurs ---

	GetWindowRect(GetDlgItem(hDlg,101),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->Timers = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SINGLESEL|LVS_NOSORTHEADER|LVS_SHOWSELALWAYS|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->Timers) goto Error_2;
	SendMessage(Context->Timers,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->IconList);

	Column.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.iSubItem = 0;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szTriggersVariablesColumn1;
	if (SendMessage(Context->Timers,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_3;
	Column.cx = Font.FontMaxWidth*30;
	Column.pszText = szTriggersVariablesColumn2;
	if (SendMessage(Context->Timers,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_3;
	Column.cx = Font.FontMaxWidth*4;
	Column.pszText = szTriggersVariablesColumn3;
	if (SendMessage(Context->Timers,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column) == -1) goto Error_3;

	for (X = 0; X != TRIGGERS_MAXTIMERS; X++)
		{
		Item.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
		Item.iItem = X;
		Item.iSubItem = 0;
		Item.iImage = 1;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.lParam = 1;
		if (SendMessage(Context->Timers,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}

	return;

//--- Erreurs ---

Error_3:SendMessage(Context->Timers,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_2:SendMessage(Context->Variables,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_1:ImageList_Destroy(Context->IconList);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Stockage des informations «««««««««««««««««««««««««««««««««««««««»

void Triggers_VariablesSetInfo(LV_DISPINFO *Info)
{
	LONG	 Count;
	char	*Name;

	if (!(Info->item.mask&LVIF_TEXT)) return;

	switch(Info->item.iSubItem)
		{
		case 0:	wsprintf(Info->item.pszText,szNumber,Info->item.iItem);
			break;
		case 1:	if (Info->item.lParam) Name = &TriggersTimers[Info->item.iItem][0];
			else Name = &TriggersVariables[Info->item.iItem][0];
			if (!*Name) Name = szTriggersVariablesNoName;
			if ((Info->item.lParam)&&(Info->item.iItem == 16)) Name = szTriggersLimitTime;
			else if ((!Info->item.lParam)&&(Info->item.iItem == 128)) Name = szTriggersScoreVar;
			strcpy(Info->item.pszText,Name);
			break;
		case 2:	Count = 0;
			if (Info->item.lParam)
				{
				TRIGGER	*Trigger;
				ACTION	*Action;

				for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
					if ((Trigger->ctrl == TRIGGER_TIMER)&&(Trigger->param2 == Info->item.iItem))
						Count++;

				for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
					if ((Action->ctrl == ACTION_SETTIMER)&&(Action->param1 == Info->item.iItem))
						Count++;
				}
			else
				{
				TRIGGER	*Trigger;
				ACTION	*Action;

				for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
					if (Trigger->ctrl == TRIGGER_VARIABLE)
						{
						if (Trigger->param2 == Info->item.iItem) Count++;
						if ((Trigger->param3 == 0x02)&&(Trigger->param4 == Info->item.iItem)) Count++;
						}

				for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
					if (Action->ctrl == ACTION_SETVARIABLE)
						{
						if (Action->param1 == Info->item.iItem) Count++;
						if ((Action->param2 == 0x0C)&&(Action->param5 == Info->item.iItem)) Count++;
						}
				}
			wsprintf(Info->item.pszText,szNumber,Count);
			break;
		}

	return;
}


// «»»» Changement de nom «««««««««««««««««««««««««««««««««««««««««««««««»

void Triggers_VariablesSetName(HWND hDlg, BOOL Variable, TRIGGERSCTX *Context)
{
	MESSAGEBOXSTRUCT	Request;
	LV_ITEM			Item;
	HWND			hList;

	if (Variable) hList = Context->Variables;
	else hList = Context->Timers;

	Item.iItem = SendMessage(hList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	if (Item.iItem == -1) return;

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = szMessageBoxExtOkCancel;
	Request.windowtext = (Variable)?szTriggersVariablesVarName:szTriggersVariablesTimName;
	Request.windowtitle = (char *)-1;
	Request.editbuffer = (Variable)?&TriggersVariables[Item.iItem][0]:&TriggersTimers[Item.iItem][0];
	Request.editbuffersize = (Variable)?TRIGGERS_MAXVARLEN:TRIGGERS_MAXTIMLEN;
	Request.flags = MBF_EDITTEXT|MBF_NOBEEP;
	Request.defaulticon = MB_ICONQUESTION;
	Request.editwidth = Font.FontMaxWidth*20;
	Request.windowpos = MBP_FOLLOWMOUSE;
	if (!Misc_MessageBoxExtArgs(hDlg,&Request)) return;

	MapChanges++;
	Item.mask = LVIF_TEXT;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.iSubItem = 0;
	SendMessage(hList,LVM_SETITEM,(WPARAM)0,(LPARAM)&Item);
	InvalidateRect(hList,NULL,FALSE);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
