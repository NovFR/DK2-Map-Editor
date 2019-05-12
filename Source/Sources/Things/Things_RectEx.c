
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestionnaire des zones, des portails et des chemins
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
extern FONTINFO		Font;
extern MAP		Map;
extern LONG		MapChanges;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern MAPRECT		MapCursorSel;
extern COLORREF		MapRectExColors[];
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ajout et modification d'une zone ou d'un portail			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Appel de la boîte de dialogue «««««««««««««««««««««««««««««««««««»

void RectEx_Select(HWND hWnd, UINT Type)
{
	RECT		UpdateRect;
	MAPCLIENT	Client;
	MAPRECTEXCTX	Context;
	int		Result;

	ZeroMemory(&Context,sizeof(MAPRECTEXCTX));
	Context.rect.node.type = Type;
	Context.hicon = LoadImage(hInstance,MAKEINTRESOURCE(19),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context.hicon)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONEXCLAMATION);
		return;
		}
	if (!MapCursorSel.hidden)
		{
		Map_CorrectRect(&MapCursorSel,&Client);
		Context.rect.rect.x1 = Client.X;
		Context.rect.rect.y1 = Client.Y;
		Context.rect.rect.x2 = Client.X+Client.Width-1;
		Context.rect.rect.y2 = Client.Y+Client.Height-1;
		}

	switch(Type)
		{
		case RECTEX_AREA:
			Context.rect.id = RectEx_GetFreeID(&MapAreas);
			Context.rect.rect.color = MapRectExColors[0];
			Context.rect.rect.textcolor = MapRectExColors[1];
			Context.rect.flags = 0x00000000;
			wsprintf(Context.rect.name,szArea,Context.rect.id);
			Context.defcolor = 0x00FFFF00;
			Context.deftxtcolor = 0x00FFFF00;
			break;
		case RECTEX_GATE:
			Context.rect.rect.x2 = Context.rect.rect.x1;
			Context.rect.rect.y2 = Context.rect.rect.y1;
			Context.rect.id = RectEx_GetFreeID(&MapGates);
			Context.rect.rect.color = MapRectExColors[2];
			Context.rect.rect.textcolor = MapRectExColors[3];
			wsprintf(Context.rect.name,szGate,Context.rect.id);
			Context.defcolor = 0x00AAAAAA;
			Context.deftxtcolor = 0x00AAAAAA;
			break;
		case RECTEX_EFFECT:
			Context.rect.id = RectEx_GetFreeID(&MapEffects);
			Context.rect.freq = 1;
			Context.rect.rect.color = MapRectExColors[4];
			Context.rect.rect.textcolor = MapRectExColors[5];
			wsprintf(Context.rect.name,szEffect,Context.rect.id);
			Context.defcolor = 0x007F00FF;
			Context.deftxtcolor = 0x007F00FF;
			break;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6700),hWnd,RectEx_Proc,(LPARAM)&Context);
	if (Result == -1)
		{
		DestroyIcon(Context.hicon);
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONEXCLAMATION);
		return;
		}

	switch(Context.rect.node.type)
		{
		case RECTEX_AREA:
			MapRectExColors[0] = Context.rect.rect.color;
			MapRectExColors[1] = Context.rect.rect.textcolor;
			break;
		case RECTEX_GATE:
			MapRectExColors[2] = Context.rect.rect.color;
			MapRectExColors[3] = Context.rect.rect.textcolor;
			break;
		case RECTEX_EFFECT:
			MapRectExColors[4] = Context.rect.rect.color;
			MapRectExColors[5] = Context.rect.rect.textcolor;
			break;
		}

	DestroyIcon(Context.hicon);
	Files_SaveUpdate();
	Map_GetMapArea(hWnd,&UpdateRect);
	InvalidateRect(hWnd,&UpdateRect,FALSE);
	UpdateWindow(hWnd);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Messages provenants de la boîte de dialogue			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK RectEx_Proc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEXCTX	*Context;
	long		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		RectEx_Init(hDlg,(MAPRECTEXCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPRECTEXCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,0);
			return(TRUE);

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 105:
					RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->rect.rect.color);
					break;
				case 107:
					RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->rect.rect.textcolor);
					break;
				case 109:
					Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->hicon);
					break;					
				}
			Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,0,1+2,(1<<1));
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 114:
							Misc_SetUpDownInt(hDlg,101,(NM_UPDOWN *)lParam);
							break;
						case 115:
							Misc_SetUpDownInt(hDlg,102,(NM_UPDOWN *)lParam);
							break;
						case 116:
							Misc_SetUpDownInt(hDlg,103,(NM_UPDOWN *)lParam);
							break;
						case 117:
							Misc_SetUpDownInt(hDlg,104,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);
				case NM_DBLCLK:
					RectEx_Modify(hDlg,Context);
					return(TRUE);
				case LVN_GETDISPINFO:
					RectEx_DisplayList((LV_DISPINFO *)lParam);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom != Context->hlist) break;
					RectEx_ChangeSelectState(hDlg,Context);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
					switch(LOWORD(wParam))
						{
						case 103:
							if (Context->rect.node.type == RECTEX_GATE) break;
						case 101:
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								}
							if (lData > Map.Properties.Width-1)
								{
								lData = Map.Properties.Width-1;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							if (Context->rect.node.type == RECTEX_GATE)
								SetDlgItemInt(hDlg,103,lData,FALSE);
							break;
						case 104:
							if (Context->rect.node.type == RECTEX_GATE) break;
						case 102:
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								}
							if (lData > Map.Properties.Height-1)
								{
								lData = Map.Properties.Height-1;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							if (Context->rect.node.type == RECTEX_GATE)
								SetDlgItemInt(hDlg,104,lData,FALSE);
							break;
						}
					return(TRUE);
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 106:
							RectEx_ChooseColor(hDlg,105,&Context->rect.rect.color,Context->defcolor);
							break;
						case 108:
							RectEx_ChooseColor(hDlg,107,&Context->rect.rect.textcolor,Context->deftxtcolor);
							break;
						case 109:
							RectEx_ModifyRect(hDlg,Context);
							break;
						case 112:
							switch(Context->rect.node.type)
								{
								case RECTEX_AREA:
									RectEx_AreaOptions(hDlg,&Context->rect);
									break;
								case RECTEX_GATE:
									RectEx_GenerateGates(hDlg,Context);
									break;
								case RECTEX_EFFECT:
									RectEx_EffectOptions(hDlg,&Context->rect);
									break;
								}
							break;
						case 113:
							RectEx_AddRect(hDlg,Context);
							break;
						case 121:
							RectEx_RemoveRects(hDlg,Context);
							break;
						case 122:
							RectEx_Modify(hDlg,Context);
							break;

						case 130:
							if (!MapCursorSel.hidden)
								{
								MAPCLIENT	MapClient;

								Map_CorrectRect(&MapCursorSel,&MapClient);
								Context->rect.rect.x1 = MapClient.X;
								Context->rect.rect.y1 = MapClient.Y;
								switch(Context->rect.node.type)
									{
									case RECTEX_AREA:
									case RECTEX_EFFECT:
										Context->rect.rect.x2 = MapClient.X+MapClient.Width-1;
										Context->rect.rect.y2 = MapClient.Y+MapClient.Height-1;
										break;
									case RECTEX_GATE:
										Context->rect.rect.x2 = MapClient.X;
										Context->rect.rect.y2 = MapClient.Y;
										break;
									}
								SetDlgItemInt(hDlg,101,Context->rect.rect.x1,FALSE);
								SetDlgItemInt(hDlg,102,Context->rect.rect.y1,FALSE);
								SetDlgItemInt(hDlg,103,Context->rect.rect.x2,FALSE);
								SetDlgItemInt(hDlg,104,Context->rect.rect.y2,FALSE);
								}
							break;

						case IDOK:
							Context->rect.rect.x1 = GetDlgItemInt(hDlg,101,NULL,FALSE);
							Context->rect.rect.y1 = GetDlgItemInt(hDlg,102,NULL,FALSE);
							Context->rect.rect.x2 = GetDlgItemInt(hDlg,103,NULL,FALSE);
							Context->rect.rect.y2 = GetDlgItemInt(hDlg,104,NULL,FALSE);
							GetDlgItemText(hDlg,110,Context->rect.name,MAP_RECTMAXNAME);
							GetDlgItemText(hDlg,111,Context->rect.info,MAP_RECTMAXINFO);
							EndDialog(hDlg,1);
							break;

						case IDCANCEL:
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initalisations de la boîte de dialogue ««««««««««««««««««««««««««»

void RectEx_Init(HWND hDlg, MAPRECTEXCTX *Context)
{
	LV_COLUMN	 Column;
	RECT		 ClientRect;
	RECT		 ClientRect2;
	LONG		 Width;
	LONG		 X,Y;
	char		*Title;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	if (!Context->modify)
		{
		GetWindowRect(GetDlgItem(hDlg,120),&ClientRect);
		MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
		GetWindowRect(GetDlgItem(hDlg,121),&ClientRect2);
		MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect2,2);
		Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect2.top-ClientRect.top-30,hDlg,NULL,hInstance,NULL);
		if (!Context->hlist)
			{
			EndDialog(hDlg,-1);
			return;
			}
		Width = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-4;
		Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		Column.fmt = LVCFMT_LEFT;
		Column.cx = Width*30/100;
		Column.pszText = szAGColumn0;
		Column.iSubItem = 0;
		SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column);
		Column.fmt = LVCFMT_CENTER;
		Column.cx = Width*10/100;
		Column.pszText = szAGColumn1;
		Column.iSubItem = 1;
		SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column);
		Column.fmt = LVCFMT_LEFT;
		Column.cx = Width*60/100;
		Column.pszText = szAGColumn2;
		Column.iSubItem = 2;
		SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column);

		RectEx_BuildList(Context);
		}

	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			if (Context->modify)
				Title = szAreaModifyTitle;
			else
				Title = szAreaTitle;
			break;

		case RECTEX_GATE:
			if (Context->modify)
				Title = szGateModifyTitle;
			else
				Title = szGateTitle;
			EnableWindow(GetDlgItem(hDlg,103),FALSE);
			EnableWindow(GetDlgItem(hDlg,104),FALSE);
			break;

		case RECTEX_EFFECT:
			if (Context->modify)
				Title = szEffectModifyTitle;
			else
				Title = szEffectTitle;
			break;
		}

	Misc_CreateUpDownButton(hDlg,101,114);
	Misc_CreateUpDownButton(hDlg,102,115);
	Misc_CreateUpDownButton(hDlg,103,116);
	Misc_CreateUpDownButton(hDlg,104,117);

	SetWindowText(hDlg,Title);
	SetDlgItemInt(hDlg,101,Context->rect.rect.x1,FALSE);
	SetDlgItemInt(hDlg,102,Context->rect.rect.y1,FALSE);
	SetDlgItemInt(hDlg,103,Context->rect.rect.x2,FALSE);
	SetDlgItemInt(hDlg,104,Context->rect.rect.y2,FALSE);
	SetDlgItemText(hDlg,110,Context->rect.name);
	SetDlgItemText(hDlg,111,Context->rect.info);

	SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,102,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,103,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,110,EM_LIMITTEXT,(WPARAM)MAP_RECTMAXNAME-1,(LPARAM)0);
	SendDlgItemMessage(hDlg,111,EM_LIMITTEXT,(WPARAM)MAP_RECTMAXINFO-1,(LPARAM)0);
	return;
}


// «»»» Classement des entrées ««««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int CALLBACK RectEx_Sort(LPARAM FirstItem, LPARAM SecondItem, LPARAM Unused)
{
	if ( ((MAPRECTEX *)FirstItem)->id < ((MAPRECTEX *)SecondItem)->id )
		return(-1);
	if ( ((MAPRECTEX *)FirstItem)->id > ((MAPRECTEX *)SecondItem)->id )
		return(1);

	return(0);
}


// «»»» Change l'état des boutons de modification «««««««««««««««««««««««»

void RectEx_ChangeSelectState(HWND hDlg, MAPRECTEXCTX *Context)
{
	long	Count;

	Count = SendMessage(Context->hlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (Count)
		{
		EnableWindow(GetDlgItem(hDlg,121),TRUE);
		if (Count == 1)
			EnableWindow(GetDlgItem(hDlg,122),TRUE);
		else
			EnableWindow(GetDlgItem(hDlg,122),FALSE);
		}
	else
		{
		EnableWindow(GetDlgItem(hDlg,121),FALSE);
		EnableWindow(GetDlgItem(hDlg,122),FALSE);
		}

	return;
}


// «»»» Suppression d'une ou plusieurs entrées ««««««««««««««««««««««««««»

void RectEx_RemoveRects(HWND hDlg, MAPRECTEXCTX *Context)
{
	MESSAGEBOXSTRUCT	 MessageBox;
	LV_ITEM			 Item;
	RECT			 UpdateRect;
	UNDO			*Undo;
	UNDO			*Child;
	UINT			 SavedId;
	UINT			 SavedNextId;
	UINT			 Type;
	LONG			 Result;
	BOOL			 KillAll;
	int			 Used;
	char			*Text;
	char			*Info;
	char			*Temp;

	if ((Result = SendMessage(Context->hlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) == 0) return;
	if ((Temp = GlobalAlloc(GPTR,2048)) == 0) return;
	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			Info = szMapKillArea;
			Text = szAreaConfirmKill;
			Type = UNDO_SUBAREA;
			break;
		case RECTEX_GATE:
			Info = szMapKillGate;
			Text = szGateConfirmKill;
			Type = UNDO_SUBGATE;
			break;
		case RECTEX_EFFECT:
			Info = szMapKillEffect;
			Text = szEffectConfirmKill;
			Type = UNDO_SUBEFFECT;
			break;
		}
	wsprintf(Temp,Text,Result);
	ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
	MessageBox.gadgets = szMessageBoxExtYesNo;
	MessageBox.windowtext = Temp;
	MessageBox.windowtitle = (char *)-1;
	MessageBox.requestbits = &Config.RequestsFlags;
	MessageBox.statebit = REQUEST_RECTEXKILL;
	MessageBox.flags = MBF_AUTOREPLY;
	MessageBox.defaultgadget = 1;
	MessageBox.defaulticon = MB_ICONQUESTION;
	MessageBox.windowpos = MBP_FOLLOWMOUSE;
	Result = Misc_MessageBoxExtArgs(hDlg,&MessageBox);
	GlobalFree(Temp);
	if (Result != 1) return;

	//--- Test si les zones sont utilisées dans le cadre d'un évènement ---

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;
	KillAll = TRUE;
	while ((Item.iItem = SendMessage(Context->hlist,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(Context->hlist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		switch(((MAPRECTEX *)Item.lParam)->node.type)
			{
			case RECTEX_AREA:
				Used = Triggers_IsUsed(hDlg,((MAPRECTEX *)Item.lParam)->id,&KillAll,SCOPE_AREA);
				break;
			case RECTEX_GATE:
				Used = Triggers_IsUsed(hDlg,((MAPRECTEX *)Item.lParam)->id,&KillAll,SCOPE_GATE);
				break;
			case RECTEX_EFFECT:
				Used = Triggers_IsUsed(hDlg,((MAPRECTEX *)Item.lParam)->id,&KillAll,SCOPE_EFFECT);
				break;
			}
		if (Used == -1) continue;
		if (Used) break;
		return;
		}

	//--- Supprime les zones ou passages ---

	Map_Lock(TRUE,FALSE);
	Undo = Undo_CreateNewEntry(UNDO_RECTEX,Info);
	Item.iItem = -1;
	while ((Item.iItem = SendMessage(Context->hlist,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(Context->hlist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		switch(((MAPRECTEX *)Item.lParam)->node.type)
			{
			case RECTEX_AREA:
				if (Triggers_IsUsed(NULL,((MAPRECTEX *)Item.lParam)->id,NULL,SCOPE_AREA)) break;
				if (!KillAll) continue;
				break;
			case RECTEX_GATE:
				if (Triggers_IsUsed(NULL,((MAPRECTEX *)Item.lParam)->id,NULL,SCOPE_GATE)) break;
				if (!KillAll) continue;
				break;
			case RECTEX_EFFECT:
				if (Triggers_IsUsed(NULL,((MAPRECTEX *)Item.lParam)->id,NULL,SCOPE_EFFECT)) break;
				if (!KillAll) continue;
				break;
			}
		SavedId = ((MAPRECTEX *)Item.lParam)->id;
		SavedNextId = ((MAPRECTEX *)Item.lParam)->nextid;
		Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
		Undo_StoreEntry(Child,(NODE *)Item.lParam,Type);
		//
		// Supprime les liens entre les zones
		//
		if (Type == UNDO_SUBAREA)
			{
			MAPRECTEX	*MapRect;

			for (MapRect = (MAPRECTEX *)MapAreas.next; MapRect != 0; MapRect = (MAPRECTEX *)MapRect->node.next)
				{
				if (MapRect->nextid == SavedId)
					{
					Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
					Undo_StoreEntry(Child,(NODE *)MapRect,UNDO_MODAREA);
					MapRect->nextid = SavedNextId;
					break;
					}
				}
			}
		}
	Map_Lock(FALSE,FALSE);
	MapChanges++;
	RectEx_BuildList(Context);
	RectEx_ChangeSelectState(hDlg,Context);
	Map_GetMapArea(hWnd,&UpdateRect);
	InvalidateRect(hWnd,&UpdateRect,FALSE);
	UpdateWindow(hWnd);

	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			if ((Context->rect.name[0] == szArea[0])&&(atoi(&Context->rect.name[1]) == Context->rect.id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapAreas);
				wsprintf(Context->rect.name,szArea,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		case RECTEX_GATE:
			if ((Context->rect.name[0] == szGate[0])&&(atoi(&Context->rect.name[1]) == Context->rect.id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapGates);
				wsprintf(Context->rect.name,szGate,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		case RECTEX_EFFECT:
			if ((Context->rect.name[0] == szEffect[0])&&(atoi(&Context->rect.name[1]) == Context->rect.id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapEffects);
				wsprintf(Context->rect.name,szEffect,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		}

	return;
}


// «»»» Modification d'une entrée «««««««««««««««««««««««««««««««««««««««»

void RectEx_Modify(HWND hDlg, MAPRECTEXCTX *Context)
{
	LV_ITEM		Item;
	RECT		UpdateRect;
	MAPCLIENT	Client;
	MAPRECTEXCTX	ModifyContext;
	UINT		Type;
	int		Result;

	CopyMemory(&ModifyContext,Context,sizeof(MAPRECTEXCTX));
	ModifyContext.modify = TRUE;

	Item.mask = LVIF_PARAM;
	Item.iItem = List_FindSelectedItem(Context->hlist);
	Item.iSubItem = 0;
	if (!SendMessage(Context->hlist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;

	Map_CorrectRect(&((MAPRECTEX *)Item.lParam)->rect,&Client);
	ModifyContext.rect.rect.x1 = Client.X;
	ModifyContext.rect.rect.y1 = Client.Y;
	ModifyContext.rect.rect.x2 = Client.X+Client.Width-1;
	ModifyContext.rect.rect.y2 = Client.Y+Client.Height-1;
	ModifyContext.rect.rect.color = ((MAPRECTEX *)(Item.lParam))->rect.color;
	ModifyContext.rect.rect.textcolor = ((MAPRECTEX *)(Item.lParam))->rect.textcolor;
	ModifyContext.rect.flags = ((MAPRECTEX *)(Item.lParam))->flags;
	ModifyContext.rect.effect1 = ((MAPRECTEX *)(Item.lParam))->effect1;
	ModifyContext.rect.effect2 = ((MAPRECTEX *)(Item.lParam))->effect2;
	ModifyContext.rect.effect3 = ((MAPRECTEX *)(Item.lParam))->effect3;
	ModifyContext.rect.effect4 = ((MAPRECTEX *)(Item.lParam))->effect4;
	ModifyContext.rect.freq = ((MAPRECTEX *)(Item.lParam))->freq;
	strcpy(ModifyContext.rect.name,((MAPRECTEX *)(Item.lParam))->name);
	strcpy(ModifyContext.rect.info,((MAPRECTEX *)(Item.lParam))->info);

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6701),hDlg,RectEx_Proc,(LPARAM)&ModifyContext);
	if ((Result == -1)||(!Result))
		return;

	switch(((MAPRECTEX *)Item.lParam)->node.type)
		{
		case RECTEX_AREA:
			Type = UNDO_MODAREA;
			break;
		case RECTEX_GATE:
			Type = UNDO_MODGATE;
			break;
		case RECTEX_EFFECT:
			Type = UNDO_MODEFFECT;
			break;
		}
	Undo_CreateEntry((NODE *)Item.lParam,Type);
	Map_CorrectRect(&ModifyContext.rect.rect,&Client);
	((MAPRECTEX *)(Item.lParam))->rect.x1 = Client.X;
	((MAPRECTEX *)(Item.lParam))->rect.y1 = Client.Y;
	((MAPRECTEX *)(Item.lParam))->rect.x2 = Client.X+Client.Width-1;
	((MAPRECTEX *)(Item.lParam))->rect.y2 = Client.Y+Client.Height-1;
	((MAPRECTEX *)(Item.lParam))->rect.color = ModifyContext.rect.rect.color;
	((MAPRECTEX *)(Item.lParam))->rect.textcolor = ModifyContext.rect.rect.textcolor;
	((MAPRECTEX *)(Item.lParam))->flags = ModifyContext.rect.flags;
	((MAPRECTEX *)(Item.lParam))->effect1 = ModifyContext.rect.effect1;
	((MAPRECTEX *)(Item.lParam))->effect2 = ModifyContext.rect.effect2;
	((MAPRECTEX *)(Item.lParam))->effect3 = ModifyContext.rect.effect3;
	((MAPRECTEX *)(Item.lParam))->effect4 = ModifyContext.rect.effect4;
	((MAPRECTEX *)(Item.lParam))->freq = ModifyContext.rect.freq;
	strncpy(((MAPRECTEX *)(Item.lParam))->name,ModifyContext.rect.name,MAP_RECTMAXNAME-1);
	strncpy(((MAPRECTEX *)(Item.lParam))->info,ModifyContext.rect.info,MAP_RECTMAXINFO-1);

	Item.mask = LVIF_PARAM|LVIF_TEXT;
	Item.pszText = LPSTR_TEXTCALLBACK;
	SendMessage(Context->hlist,LVM_SETITEM,(WPARAM)0,(LPARAM)&Item);
	InvalidateRect(Context->hlist,NULL,FALSE);
	UpdateWindow(Context->hlist);
	SendMessage(Context->hlist,LVM_SORTITEMS,(WPARAM)0,(LPARAM)RectEx_Sort);
	Map_GetMapArea(hWnd,&UpdateRect);
	InvalidateRect(hWnd,&UpdateRect,FALSE);
	UpdateWindow(hWnd);
	MapChanges++;
	return;
}


// «»»» Ajoute une entrée à la liste ««««««««««««««««««««««««««««««««««««»

void RectEx_AddRect(HWND hDlg, MAPRECTEXCTX *Context)
{
	LV_ITEM		 Item;
	RECT		 UpdateRect;
	MAPCLIENT	 Client;
	MAPRECTEX	*Rect;

	Rect = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
	if (!Rect) return;

	Context->rect.rect.x1 = GetDlgItemInt(hDlg,101,NULL,FALSE);
	Context->rect.rect.y1 = GetDlgItemInt(hDlg,102,NULL,FALSE);
	Context->rect.rect.x2 = GetDlgItemInt(hDlg,103,NULL,FALSE);
	Context->rect.rect.y2 = GetDlgItemInt(hDlg,104,NULL,FALSE);
	GetDlgItemText(hDlg,110,Context->rect.name,MAP_RECTMAXNAME);
	GetDlgItemText(hDlg,111,Context->rect.info,MAP_RECTMAXINFO);
	CopyMemory(Rect,&Context->rect,sizeof(MAPRECTEX));
	Map_CorrectRect(&Context->rect.rect,&Client);
	Rect->rect.x1 = Client.X;
	Rect->rect.y1 = Client.Y;
	Rect->rect.x2 = Client.X+Client.Width-1;
	Rect->rect.y2 = Client.Y+Client.Height-1;
	Rect->rect.name = Rect->name;

	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			Rect->id = RectEx_GetFreeID(&MapAreas);
			if (Rect->id > 255)
				{
				GlobalFree(Rect);
				Misc_MessageBoxExt(hDlg,szAreaFull,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
				return;
				}
			List_AddEntry((NODE *)Rect,&MapAreas);
			Undo_CreateEntry((NODE *)Rect,UNDO_ADDAREA);
			if ((Context->rect.name[0] == szArea[0])&&(atoi(&Context->rect.name[1]) == Rect->id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapAreas);
				wsprintf(Context->rect.name,szArea,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		case RECTEX_GATE:
			Rect->rect.x2 = Rect->rect.x1;
			Rect->rect.y2 = Rect->rect.y2;
			Rect->id = RectEx_GetFreeID(&MapGates);
			if (Rect->id > 255)
				{
				GlobalFree(Rect);
				Misc_MessageBoxExt(hDlg,szGateFull,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
				return;
				}
			List_AddEntry((NODE *)Rect,&MapGates);
			Undo_CreateEntry((NODE *)Rect,UNDO_ADDGATE);
			if ((Context->rect.name[0] == szGate[0])&&(atoi(&Context->rect.name[1]) == Rect->id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapGates);
				wsprintf(Context->rect.name,szGate,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		case RECTEX_EFFECT:
			Rect->id = RectEx_GetFreeID(&MapEffects);
			if (Rect->id > 255)
				{
				GlobalFree(Rect);
				Misc_MessageBoxExt(hDlg,szEffectFull,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
				return;
				}
			List_AddEntry((NODE *)Rect,&MapEffects);
			Undo_CreateEntry((NODE *)Rect,UNDO_ADDEFFECT);
			if ((Context->rect.name[0] == szEffect[0])&&(atoi(&Context->rect.name[1]) == Rect->id))
				{
				Context->rect.id = RectEx_GetFreeID(&MapEffects);
				wsprintf(Context->rect.name,szEffect,Context->rect.id);
				SetDlgItemText(hDlg,110,Context->rect.name);
				}
			break;
		}

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = SendMessage(Context->hlist,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0);
	Item.iSubItem = 0;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.lParam = (LPARAM)Rect;
	SendMessage(Context->hlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
	SendMessage(Context->hlist,LVM_SORTITEMS,(WPARAM)0,(LPARAM)RectEx_Sort);
	Map_GetMapArea(hWnd,&UpdateRect);
	InvalidateRect(hWnd,&UpdateRect,FALSE);
	UpdateWindow(hWnd);
	MapChanges++;
	return;
}


// «»»» Modification des dimensions «««««««««««««««««««««««««««««««««««««»

void RectEx_ModifyRect(HWND hDlg, MAPRECTEXCTX *Context)
{
	MAPRECT		 MapRect;
	MAPCLIENT	 MapClient;
	UINT		 Mode;
	char		*Info;

	MapRect.x1 = GetDlgItemInt(hDlg,101,NULL,FALSE);
	MapRect.y1 = GetDlgItemInt(hDlg,102,NULL,FALSE);
	MapRect.x2 = GetDlgItemInt(hDlg,103,NULL,FALSE);
	MapRect.y2 = GetDlgItemInt(hDlg,104,NULL,FALSE);

	Map_CorrectRect(&MapRect,&MapClient);
	MapRect.x1 = MapClient.X;
	MapRect.y1 = MapClient.Y;
	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			MapRect.x2 = MapClient.X+MapClient.Width-1;
			MapRect.y2 = MapClient.Y+MapClient.Height-1;
			Info = szAreaChoice;
			Mode = MAP_CHOICELMBRECT;
			break;
		case RECTEX_GATE:
			MapRect.x2 = MapClient.X;
			MapRect.y2 = MapClient.Y;
			Info = szGateChoice;
			Mode = MAP_CHOICELMBPOINT;
			break;
		case RECTEX_EFFECT:
			MapRect.x2 = MapClient.X+MapClient.Width-1;
			MapRect.y2 = MapClient.Y+MapClient.Height-1;
			Info = szEffectChoice;
			Mode = MAP_CHOICELMBRECT;
			break;
		}

	Map_ChoosePointEx(hDlg,&MapRect,Mode,Info);

	SetDlgItemInt(hDlg,101,MapRect.x1,FALSE);
	SetDlgItemInt(hDlg,102,MapRect.y1,FALSE);
	SetDlgItemInt(hDlg,103,MapRect.x2,FALSE);
	SetDlgItemInt(hDlg,104,MapRect.y2,FALSE);
	return;
}


// «»»» Création de la liste des entrée «««««««««««««««««««««««««««««««««»

void RectEx_BuildList(MAPRECTEXCTX *Context)
{
	LV_ITEM		 Item;
	MAPRECTEX	*List;
	int		 i;

	SendMessage(Context->hlist,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	Item.mask = LVIF_TEXT|LVIF_PARAM;

	switch(Context->rect.node.type)
		{
		case RECTEX_AREA:
			List = (MAPRECTEX *)MapAreas.next;
			break;
		case RECTEX_GATE:
			List = (MAPRECTEX *)MapGates.next;
			break;
		case RECTEX_EFFECT:
			List = (MAPRECTEX *)MapEffects.next;
			break;
		}

	for (i = 0; List != 0; List = (MAPRECTEX *)List->node.next, i++)
		{
		Item.iItem = i;
		Item.iSubItem = 0;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Context->hlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	SendMessage(Context->hlist,LVM_SORTITEMS,(WPARAM)0,(LPARAM)RectEx_Sort);
	return;
}


// «»»» Affichage des entrées dans la liste «««««««««««««««««««««««««««««»

void RectEx_DisplayList(LV_DISPINFO *DispInfo)
{
	MAPRECTEX	*Rect;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;

	Rect = (MAPRECTEX *)DispInfo->item.lParam;

	switch(DispInfo->item.iSubItem)
		{
		case 0:	RectEx_StrCpy(DispInfo->item.pszText,Rect->name);
			break;
		case 1:	wsprintf(DispInfo->item.pszText,szNumber,Rect->id);
			break;
		case 2:	RectEx_StrCpy(DispInfo->item.pszText,Rect->info);
			break;
		}
	return;
}


// «»»» Affichage d'un bouton coloré ««««««««««««««««««««««««««««««««««««»

void RectEx_DrawColoredButton(DRAWITEMSTRUCT *ItemStruct, COLORREF Color)
{
	HBRUSH	hBrush;

	if (ItemStruct->CtlType != ODT_BUTTON) return;

	hBrush = CreateSolidBrush(Color);
	if (hBrush)
		{
		FillRect(ItemStruct->hDC,&ItemStruct->rcItem,hBrush);
		DeleteObject(hBrush);
		}

	Misc_DrawBevelBoxRect(NULL,ItemStruct->hDC,&ItemStruct->rcItem,0x80000001);
	return;
}


// «»»» Modification d'une couleur ««««««««««««««««««««««««««««««««««««««»

void RectEx_ChooseColor(HWND hDlg, UINT CtrlId, COLORREF *ColorPtr, COLORREF DefaultColor)
{
	CHOOSECOLOR	ColorSelector;
	static COLORREF	CustomColors[16];

	ColorSelector.lStructSize = sizeof(CHOOSECOLOR);
	ColorSelector.hwndOwner = hDlg;
	ColorSelector.rgbResult = *ColorPtr;
	ColorSelector.lpCustColors = CustomColors;
	ColorSelector.Flags = CC_FULLOPEN|CC_RGBINIT;
	ColorSelector.lpfnHook = NULL;
	CustomColors[0] = *ColorPtr;
	CustomColors[1] = DefaultColor;
	if (ChooseColor(&ColorSelector))
		{
		*ColorPtr = ColorSelector.rgbResult;
		InvalidateRect(GetDlgItem(hDlg,CtrlId),NULL,FALSE);
		UpdateWindow(GetDlgItem(hDlg,CtrlId));
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Options des zones						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void RectEx_AreaOptions(HWND hDlg, MAPRECTEX *Area)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6702),hDlg,RectEx_AreaOptionsProc,(LPARAM)Area);
	if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK RectEx_AreaOptionsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEX	*Area;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		Area = (MAPRECTEX *)lParam;
		SetWindowLong(hDlg,DWL_USER,(LONG)Area);
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		if (Area->flags&GAME_AREAFLAGHEROLAIR)	CheckDlgButton(hDlg,100,BST_CHECKED);
		if (Area->flags&GAME_AREAFLAGIGNSOLID)  CheckDlgButton(hDlg,101,BST_CHECKED);
		if (Area->flags&GAME_AREAFLAGVISIBLE)	CheckDlgButton(hDlg,102,BST_CHECKED);
		if (Area->flags&GAME_AREAFLAGTOOLBOX)	CheckDlgButton(hDlg,103,BST_CHECKED);
		return(TRUE);
		}

	Area = (MAPRECTEX *)GetWindowLong(hDlg,DWL_USER);
	if (!Area) return(FALSE);

	switch(uMsgId)
		{
		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case IDOK:
				case IDCANCEL:
					Area->flags = 0;
					if (IsDlgButtonChecked(hDlg,100) == BST_CHECKED) Area->flags |= GAME_AREAFLAGHEROLAIR;
					if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED) Area->flags |= GAME_AREAFLAGIGNSOLID;
					if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED) Area->flags |= GAME_AREAFLAGVISIBLE;
					if (IsDlgButtonChecked(hDlg,103) == BST_CHECKED) Area->flags |= GAME_AREAFLAGTOOLBOX;
					EndDialog(hDlg,1);
					break;
				}
			return(TRUE);
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Options des effets						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void RectEx_EffectOptions(HWND hDlg, MAPRECTEX *Effect)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6703),hDlg,RectEx_EffectOptionsProc,(LPARAM)Effect);
	if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK RectEx_EffectOptionsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEX	*Effect;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		Effect = (MAPRECTEX *)lParam;
		SetWindowLong(hDlg,DWL_USER,(LONG)Effect);
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

		SendDlgItemMessage(hDlg,101,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(1,255));
		SendDlgItemMessage(hDlg,101,TBM_SETTICFREQ,(WPARAM)5,(LPARAM)0);
		SendDlgItemMessage(hDlg,101,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Effect->freq);
		Misc_CreateUpDownButton(hDlg,102,113);
		SetDlgItemInt(hDlg,102,Effect->freq,FALSE);

		for (X = 0; Effects[X].Id != 0xFFFFFFFF; X++)
			{
			if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[X].Name) < 0)
				{
				EndDialog(hDlg,-1);
				return(TRUE);
				}
			if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[X].Name) < 0)
				{
				EndDialog(hDlg,-1);
				return(TRUE);
				}
			if (SendDlgItemMessage(hDlg,108,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[X].Name) < 0)
				{
				EndDialog(hDlg,-1);
				return(TRUE);
				}
			if (SendDlgItemMessage(hDlg,110,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[X].Name) < 0)
				{
				EndDialog(hDlg,-1);
				return(TRUE);
				}
			}
		SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)Effect->effect1,(LPARAM)0);
		SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)Effect->effect2,(LPARAM)0);
		SendDlgItemMessage(hDlg,108,CB_SETCURSEL,(WPARAM)Effect->effect3,(LPARAM)0);
		SendDlgItemMessage(hDlg,110,CB_SETCURSEL,(WPARAM)Effect->effect4,(LPARAM)0);

		return(TRUE);
		}

	Effect = (MAPRECTEX *)GetWindowLong(hDlg,DWL_USER);
	if (!Effect) return(FALSE);

	switch(uMsgId)
		{
		case WM_HSCROLL:
			switch(LOWORD(wParam))
				{
				case TB_BOTTOM:
				case TB_LINEDOWN:
				case TB_LINEUP:
				case TB_PAGEDOWN:
				case TB_PAGEUP:
				case TB_THUMBPOSITION:
				case TB_THUMBTRACK:
				case TB_TOP:
					if ((HWND)lParam == GetDlgItem(hDlg,101))
						{
						SetDlgItemInt(hDlg,102,SendDlgItemMessage(hDlg,101,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 113:
							Misc_SetUpDownInt(hDlg,102,(NM_UPDOWN *)lParam);
							lData = GetDlgItemInt(hDlg,102,NULL,FALSE);
							if (lData < 1) SetDlgItemInt(hDlg,102,1,FALSE);
							if (lData > 255) SetDlgItemInt(hDlg,102,255,FALSE);
							SendDlgItemMessage(hDlg,101,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)GetDlgItemInt(hDlg,102,NULL,FALSE));
							break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case IDOK:
				case IDCANCEL:
					Effect->freq = SendDlgItemMessage(hDlg,101,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
					Effect->effect1 = SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					Effect->effect2 = SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					Effect->effect3 = SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					Effect->effect4 = SendDlgItemMessage(hDlg,110,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					EndDialog(hDlg,1);
					break;
				}
			return(TRUE);
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Options des passages						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Génération automatique des portails «««««««««««««««««««««««««««««»

void RectEx_GenerateGates(HWND hWnd, MAPRECTEXCTX *ParentContext)
{
	MAPRECTEXGATESCTX	*Context;
	int			 Result;

	Context = GlobalAlloc(GPTR,sizeof(MAPRECTEXGATESCTX));
	if (!Context)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}
	if (!RectEx_GetGatesList(&Context->gates))
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		List_ReleaseMemory(&Context->gates);
		GlobalFree(Context);
		return;
		}

	Context->parentctx = ParentContext;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6704),hWnd,RectEx_GenerateProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	if ((Result != 0)&&(Result != -1))
		{
		RectEx_Generate(hWnd,Context);
		if (Context->parentctx)
			RectEx_BuildList(Context->parentctx);
		}

	List_ReleaseMemory(&Context->gates);
	GlobalFree(Context);
	return;
}


// «»»» Création de la liste des portails «««««««««««««««««««««««««««««««»

int RectEx_GetGatesList(NODE *GatesList)
{
	MAPRECTEXGATESLIST	*CurrentGate;
	MAPRECTEX		*Area;
	TRIGGERLINK		*ActionPt;
	TRIGGER			*Trigger;
	BYTE			*MapPtr;
	BYTE			*MapByte;
	DWORD			 Gate;
	UINT			 Type;
	LONG			 X,Y;
	LONG			 Width,Height;
	BOOL			 NotDestroyable;
	static LONG		 Offsets[]={	0,0,
						1,0,
						2,0,
						3,0,
						3,1,
						3,2,
						3,3,
						2,3,
						1,3,
						0,3,
						0,2,
						0,1,
						-1};

	//
	// Comptabilise tous les portails
	//

	ZeroMemory(GatesList,sizeof(NODE));
	MapPtr = Map.Map;
	for (Y = 0; Y != Map.Properties.Height-1; Y++)
		{
		MapByte = MapPtr;
		for (X = 0; X != Map.Properties.Width-1; X++)
			{
			if (*(MapByte+3) == 2)
				{
				Gate = Gates[*MapByte].Id&0xFF00FF00;
				/* Seuls les portails 2, 3, 6 et 7 peuvent être déclarés */
				if ((Gate == 0x1D000100)||(Gate == 0x21000100)||(Gate == 0x25000100)||(Gate == 0x25000200))
					{
					if (Gate == 0x1D000100)
						{
						Type = 2;
						Width = 1;
						Height = 1;
						NotDestroyable = TRUE;
						}
					else if (Gate == 0x21000100)
						{
						Type = 3;
						Width = 2;
						Height = 2;
						NotDestroyable = FALSE;
						}
					else if (Gate == 0x25000100)
						{
						Type = 6;
						Width = 3;
						Height = 1;
						NotDestroyable = TRUE;
						}
					else
						{
						Type = 7;
						Width = 1;
						Height = 3;
						NotDestroyable = TRUE;
						}

					if (!RectEx_GateInList(GatesList,X,Y))
						{
						CurrentGate = GlobalAlloc(GPTR,sizeof(MAPRECTEXGATESLIST));
						if (!CurrentGate) return(0);
						CurrentGate->node.type = Type;
						CurrentGate->x = X;
						CurrentGate->y = Y;
						CurrentGate->width = Width;
						CurrentGate->height = Height;
						CurrentGate->cantbedestroyed = NotDestroyable;
						List_AddEntry((NODE *)CurrentGate,GatesList);
						}
					}
				}
			MapByte += MAP_CONTROLBYTES;
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	//
	// Recherche les portails déjà alloués
	//

	for (CurrentGate = (MAPRECTEXGATESLIST *)GatesList->next; CurrentGate != 0; CurrentGate = (MAPRECTEXGATESLIST *)CurrentGate->node.next)
		{
		for (Area = (MAPRECTEX *)MapGates.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
			{
			if (Area->rect.x1 < CurrentGate->x) continue;
			if (Area->rect.y1 < CurrentGate->y) continue;
			if (Area->rect.x2 >= CurrentGate->x+CurrentGate->width) continue;
			if (Area->rect.y2 >= CurrentGate->y+CurrentGate->height) continue;
			CurrentGate->gate = Area->id;
			CurrentGate->hasgate = TRUE;
			break;
			}
		}

	//
	// Recherche les zones déjà allouées
	//

	for (CurrentGate = (MAPRECTEXGATESLIST *)GatesList->next; CurrentGate != 0; CurrentGate = (MAPRECTEXGATESLIST *)CurrentGate->node.next)
		{
		for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
			{
			/* Zone allouée pour la libération uniquement */
			if ((CurrentGate->node.type == 6)||(CurrentGate->node.type == 7))
				{
				if (Area->rect.x1 < CurrentGate->x) continue;
				if (Area->rect.y1 < CurrentGate->y) continue;
				if (Area->rect.x2 < CurrentGate->x) continue;
				if (Area->rect.y2 < CurrentGate->y) continue;
				if (Area->rect.x1 >= CurrentGate->x+CurrentGate->width) continue;
				if (Area->rect.y1 >= CurrentGate->y+CurrentGate->height) continue;
				if (Area->rect.x2 >= CurrentGate->x+CurrentGate->width) continue;
				if (Area->rect.y2 >= CurrentGate->y+CurrentGate->height) continue;
				CurrentGate->area = Area->id;
				CurrentGate->hasarea = TRUE;
				break;
				}
			/* Zone allouée pour la libération uniquement */
			else if (CurrentGate->node.type == 2)
				{
				if (CurrentGate->x != Area->rect.x1) continue;
				if (CurrentGate->y != Area->rect.y1) continue;
				if (CurrentGate->x+CurrentGate->width-1 != Area->rect.x2) continue;
				if (CurrentGate->y+CurrentGate->height-1 != Area->rect.y2) continue;
				CurrentGate->area = Area->id;
				CurrentGate->hasarea = TRUE;
				break;
				}
			/* Zone allouée pour la destruction uniquement */
			else if (CurrentGate->node.type == 3)
				{
				if (CurrentGate->x-1 != Area->rect.x1) continue;
				if (CurrentGate->y-1 != Area->rect.y1) continue;
				if (CurrentGate->x+CurrentGate->width != Area->rect.x2) continue;
				if (CurrentGate->y+CurrentGate->height != Area->rect.y2) continue;
				CurrentGate->area = Area->id;
				CurrentGate->hasarea = TRUE;
				break;
				}
			}
		}

	//
	// Recherche les portails indestructibles en raison du terrain adjacant
	// (uniquement tests sur portail n°3, de fait)
	//

	for (CurrentGate = (MAPRECTEXGATESLIST *)GatesList->next; CurrentGate != 0; CurrentGate = (MAPRECTEXGATESLIST *)CurrentGate->node.next)
		{
		if (CurrentGate->cantbedestroyed) continue;
		MapPtr = Map.Map+(CurrentGate->x-1)*MAP_CONTROLBYTES+(CurrentGate->y-1)*Map.Properties.Width*MAP_CONTROLBYTES;
		for (X = 0; Offsets[X] != -1; X += 2)
			{
			MapByte = MapPtr+Offsets[X]*MAP_CONTROLBYTES+Offsets[X+1]*Map.Properties.Width*MAP_CONTROLBYTES;
			if (*(MapByte+3) == 2)
				{
				CurrentGate->cantbedestroyed = TRUE;
				break;
				}
			if (*(MapByte+3) == 0)
				{
				Gate = World[*MapByte].Id&0xFF000000;
				// Si un des éléments suivants entour le portail, ce dernier
				// ne pourra pas être détruit : Limite, Gems, Roche impénétrable,
				// Salle de repos des héros, limite de l'éditeur.
				//
				if ((Gate == 0x1E00000000)||(Gate == 0x07000000)||(Gate == 0x01000000)||(Gate == 0x23000000)||(Gate == 0xFE000000))
					{
					CurrentGate->cantbedestroyed = TRUE;
					break;
					}
				}
			}
		}

	//
	// Recherche les portails pouvant être détruits par évènements
	// (uniquement tests sur portail n°3, de fait)
	//

	for (CurrentGate = (MAPRECTEXGATESLIST *)GatesList->next; CurrentGate != 0; CurrentGate = (MAPRECTEXGATESLIST *)CurrentGate->node.next)
		{
		if (CurrentGate->cantbedestroyed) continue;
		if (!CurrentGate->hasarea) continue;
		if (!CurrentGate->hasgate) continue;
		for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
			{
			if (ActionPt->node.type != SCOPE_AREA) continue;
			if (ActionPt->area != CurrentGate->area) continue;
			break;
			}
		if (!ActionPt) continue;
		for (Trigger = Triggers_GetTriggerById(ActionPt->trigger); Trigger != 0; Trigger = Triggers_GetTriggerById(Trigger->or))
			{
			if (Trigger->ctrl != TRIGGER_CLAIMAREA) continue;
			if (Trigger->param5 != 12) continue;
			if (!RectEx_FindDestroyAction(Trigger->and)) continue;
			CurrentGate->hasevent = TRUE;
			break;
			}
		}

	//
	// Recherche à nouveau les zones qui coïncident avec les portails indestructibles
	// (uniquement tests sur portail n°3, de fait)
	//

	for (CurrentGate = (MAPRECTEXGATESLIST *)GatesList->next; CurrentGate != 0; CurrentGate = (MAPRECTEXGATESLIST *)CurrentGate->node.next)
		{
		if (!CurrentGate->cantbedestroyed) continue;
		for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
			{
			if (CurrentGate->x != Area->rect.x1) continue;
			if (CurrentGate->y != Area->rect.y1) continue;
			if (CurrentGate->x+CurrentGate->width-1 != Area->rect.x2) continue;
			if (CurrentGate->y+CurrentGate->height-1 != Area->rect.y2) continue;
			CurrentGate->area = Area->id;
			CurrentGate->hasarea = TRUE;
			break;
			}
		}

	return(1);
}


//--- Recherche l'action de destruction ---

int RectEx_FindDestroyAction(short Ctrl)
{
	TRIGGER	*Trigger;
	ACTION	*Action;

	if (Ctrl > 0)
		{
		Trigger = Triggers_GetTriggerById(Ctrl);
		if (Trigger)
			{
			if (RectEx_FindDestroyAction(Trigger->and)) return(1);
			if (RectEx_FindDestroyAction(Trigger->or)) return(1);
			}
		}

	if (Ctrl < 0)
		{
		Action = Triggers_GetActionById(Ctrl);
		if (Action)
			{
			if (Action->ctrl == ACTION_DESTROYGATE) return(1);
			return(RectEx_FindDestroyAction(Action->next));
			}
		}

	return(0);
}


//--- Vérifie si le portail est déjà répertorié ---

int RectEx_GateInList(NODE *GatesList, LONG X, LONG Y)
{
	MAPRECTEXGATESLIST	*List;

	for (List = (MAPRECTEXGATESLIST *)GatesList->next; List != 0; List = (MAPRECTEXGATESLIST *)List->node.next)
		{
		if (X < List->x) continue;
		if (Y < List->y) continue;
		if (X >= List->x+List->width) continue;
		if (Y >= List->y+List->height) continue;
		return(1);
		}

	return(0);
}


// «»»» Génération des structures désirées ««««««««««««««««««««««««««««««»

void RectEx_Generate(HWND hWnd, MAPRECTEXGATESCTX *Context)
{
	MAPRECTEXGATESLIST	*Gate;
	UINT			 GateID;
	UINT			 AreaID;
	BOOL			 Error = FALSE;
	int			 i;

	Map_Lock(TRUE,FALSE);

	//
	//--- Ajoute les structures primitives ---
	//

	for (Gate = (MAPRECTEXGATESLIST *)Context->gates.next; Gate != NULL; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
		{
		GateID = RectEx_GetFreeID(&MapGates);
		AreaID = RectEx_GetFreeID(&MapAreas);

		//
		// Test quelles structures sont à ajouter
		//

		if (!Gate->hasgate)
			{
			if (Context->flags&RECTEX_AUTOFGRELEASE)
				Gate->addgate = TRUE;
			else if ((Context->flags&RECTEX_AUTOFGDISPLAY) && (Gate->node.type == 6))
				Gate->addgate = TRUE;
			else if ((Context->flags&RECTEX_AUTOFGDISPLAY) && (Gate->node.type == 7))
				Gate->addgate = TRUE;
			else if ((Context->flags&RECTEX_AUTOFGDESTROY) && (!Gate->cantbedestroyed))
				Gate->addgate = TRUE;
			}

		if (!Gate->hasarea)
			{
			if (Context->flags&RECTEX_AUTOFARELEASE)
				Gate->addarea = TRUE;
			else if ((Context->flags&RECTEX_AUTOFADESTROY) && (!Gate->cantbedestroyed))
				Gate->addarea = TRUE;
			}

		if ((!Gate->hasevent)&&(Context->flags&RECTEX_AUTOFTRIGGERS)&&(!Gate->cantbedestroyed))
				Gate->addevent = TRUE;

		//
		// Ajoute les passages et les zones
		//

		if (Gate->addgate)
			{
			MAPRECTEX	*GatePtr;

			Error = TRUE;
			GatePtr = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
			if (!GatePtr) break;
			Error = FALSE;

			GatePtr->node.type = RECTEX_GATE;
			GatePtr->rect.name = GatePtr->name;
			GatePtr->id = GateID;
			List_AddEntry((NODE *)GatePtr,&MapGates);
			Gate->gate = GateID;
			Gate->gateptr = GatePtr;
			MapChanges++;
			}

		if (Gate->addarea)
			{
			MAPRECTEX	*AreaPtr;

			Error = TRUE;
			AreaPtr = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
			if (!AreaPtr) break;
			Error = FALSE;

			AreaPtr->node.type = RECTEX_AREA;
			AreaPtr->rect.name = AreaPtr->name;
			AreaPtr->id = AreaID;
			List_AddEntry((NODE *)AreaPtr,&MapAreas);
			Gate->area = AreaID;
			Gate->areaptr = AreaPtr;
			MapChanges++;
			}

		//
		// Ajoute & initialise les évènements
		//

		if (Gate->addevent)
			{
			TRIGGERLINK	*ActionPt;
			TRIGGER		*Trigger;
			ACTION		*Action;
			short		 PrevTrigger = 0;

			for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
				{
				if (ActionPt->node.type != SCOPE_AREA) continue;
				if (ActionPt->area != Gate->area) continue;
				break;
				}

			if (!ActionPt)
				{
				Error = TRUE;
				ActionPt = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
				if (!ActionPt) break;
				List_AddEntry((NODE *)ActionPt,&MapActionPoints);
				Error = FALSE;

				Gate->actionptptr = ActionPt;
				ActionPt->node.type = SCOPE_AREA;
				ActionPt->area = Gate->area;
				MapChanges++;
				}

			if (ActionPt->trigger)
				{
				Trigger = Triggers_GetTriggerById(ActionPt->trigger);
				while (Trigger)
					{
					PrevTrigger = Trigger->id;
					Trigger = Triggers_GetTriggerById(Trigger->or);
					}
				}

			for (i = 0; i != Map.Properties.Players; i++)
				{
				Error = TRUE;

				/* Création du déclencheur */
				Trigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
				if (!Trigger) break;
				List_AddEntry((NODE *)Trigger,&MapTriggers);
				Gate->triggerptr[i] = Trigger;
				MapChanges++;

				Trigger->node.type = TYPE_TRIGGER;
				Trigger->previd = PrevTrigger;
				Trigger->param1 = 0x05;
				Trigger->param2 = i+3;
				Trigger->param5 = 12;
				Trigger->id = Triggers_GetTriggerFreeId();
				Trigger->ctrl = TRIGGER_CLAIMAREA;
				Trigger->repeat = 1;

				/* Création de l'action */
				Action = GlobalAlloc(GPTR,sizeof(ACTION));
				if (!Action) break;
				List_AddEntry((NODE *)Action,&MapActions);
				Gate->actionptr[i] = Action;
				MapChanges++;

				Action->node.type = TYPE_ACTION;
				Action->param5 = Gate->gate;
				Action->id = Triggers_GetActionFreeId();
				Action->ctrl = ACTION_DESTROYGATE;
				Action->one = 1;

				Trigger->and = Action->id;

				/* Lie le déclencheur aux précédents */
				if (PrevTrigger)
					{
					TRIGGER *Prev = Triggers_GetTriggerById(PrevTrigger);
					if (Prev) Prev->or = Trigger->id;
					}
				else
					{
					ActionPt->trigger = Trigger->id;
					}

				PrevTrigger = Trigger->id;
				Error = FALSE;
				}

			if (Error) break;
			}
		}

	//
	//--- Test si une erreur est survenue ---
	//

	if (Error)
		{
		for (Gate = (MAPRECTEXGATESLIST *)Context->gates.next; Gate != NULL; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
			{
			if (Gate->gateptr)
				{
				List_RemEntry((NODE *)Gate->gateptr);
				GlobalFree(Gate->gateptr);
				MapChanges--;
				}
			if (Gate->areaptr)
				{
				List_RemEntry((NODE *)Gate->areaptr);
				GlobalFree(Gate->areaptr);
				MapChanges--;
				}
			if (Gate->actionptptr)
				{
				List_RemEntry((NODE *)Gate->actionptptr);
				GlobalFree(Gate->actionptptr);
				MapChanges--;
				}
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				if (Gate->triggerptr[i])
					{
					List_RemEntry((NODE *)Gate->triggerptr[i]);
					GlobalFree(Gate->triggerptr[i]);
					MapChanges--;
					}
				if (Gate->actionptr[i])
					{
					List_RemEntry((NODE *)Gate->actionptr[i]);
					GlobalFree(Gate->actionptr[i]);
					MapChanges--;
					}
				}
			}
		Misc_MessageBoxExt(hWnd,szNGFailed,NULL,szMessageBoxExtOk,MB_ICONHAND);
		Map_Lock(FALSE,TRUE);
		return;
		}

	//
	//--- Initialise les structures ---
	//

	for (Gate = (MAPRECTEXGATESLIST *)Context->gates.next; Gate != NULL; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
		{
		if (Gate->gateptr)
			{
			Gate->gateptr->rect.color = Context->gatestemplate.rect.color;
			Gate->gateptr->rect.textcolor = Context->gatestemplate.rect.textcolor;
			if ((Gate->node.type == 2)||(Gate->node.type == 3))
				{
				Gate->gateptr->rect.x1 = Gate->x;
				Gate->gateptr->rect.y1 = Gate->y;
				Gate->gateptr->rect.x2 = Gate->x;
				Gate->gateptr->rect.y2 = Gate->y;
				}
			else if (Gate->node.type == 6)
				{
				Gate->gateptr->rect.x1 = Gate->x+1;
				Gate->gateptr->rect.y1 = Gate->y;
				Gate->gateptr->rect.x2 = Gate->x+1;
				Gate->gateptr->rect.y2 = Gate->y;
				}
			else if (Gate->node.type == 7)
				{
				Gate->gateptr->rect.x1 = Gate->x;
				Gate->gateptr->rect.y1 = Gate->y+1;
				Gate->gateptr->rect.x2 = Gate->x;
				Gate->gateptr->rect.y2 = Gate->y+1;
				}
			RectEx_GenerateRawDoFmt(Gate,Gate->gateptr->name,Context->gatestemplate.name,MAP_RECTMAXNAME);
			RectEx_GenerateRawDoFmt(Gate,Gate->gateptr->info,Context->gatestemplate.info,MAP_RECTMAXINFO);
			}

		if (Gate->areaptr)
			{
			Gate->areaptr->rect.color = Context->areastemplate.rect.color;
			Gate->areaptr->rect.textcolor = Context->areastemplate.rect.textcolor;
			if (!Gate->cantbedestroyed)
				{
				Gate->areaptr->rect.x1 = Gate->x-1;
				Gate->areaptr->rect.y1 = Gate->y-1;
				Gate->areaptr->rect.x2 = Gate->x+Gate->width;
				Gate->areaptr->rect.y2 = Gate->y+Gate->height;
				}
			else if ((Gate->node.type == 2)||(Gate->node.type == 3))
				{
				Gate->areaptr->rect.x1 = Gate->x;
				Gate->areaptr->rect.y1 = Gate->y;
				Gate->areaptr->rect.x2 = Gate->x+Gate->width-1;
				Gate->areaptr->rect.y2 = Gate->y+Gate->height-1;
				}
			else if (Gate->node.type == 6)
				{
				Gate->areaptr->rect.x1 = Gate->x+1;
				Gate->areaptr->rect.y1 = Gate->y;
				Gate->areaptr->rect.x2 = Gate->x+1;
				Gate->areaptr->rect.y2 = Gate->y;
				}
			else if (Gate->node.type == 7)
				{
				Gate->areaptr->rect.x1 = Gate->x;
				Gate->areaptr->rect.y1 = Gate->y+1;
				Gate->areaptr->rect.x2 = Gate->x;
				Gate->areaptr->rect.y2 = Gate->y+1;
				}
			RectEx_GenerateRawDoFmt(Gate,Gate->areaptr->name,Context->areastemplate.name,MAP_RECTMAXNAME);
			RectEx_GenerateRawDoFmt(Gate,Gate->areaptr->info,Context->areastemplate.info,MAP_RECTMAXINFO);
			}
		}

	//
	//--- Terminé ! ---
	//

	Map_Lock(FALSE,TRUE);
	Files_SaveUpdate();
	return;
}

//--- Formatage des textes ---

void RectEx_GenerateRawDoFmt(MAPRECTEXGATESLIST *Gate, char *Dest, char *Source, LONG Size)
{
	LONG	 Copied = 0;
	BOOL	 SetArea = FALSE;
	BOOL	 SetGate = FALSE;
	char	*DestPtr;

	ZeroMemory(Dest,Size);
	DestPtr = Dest;

	while(*Source != 0)
		{
		if (Copied >= Size-4) break;

		if (*Source == '%')
			{
			Source++;
			switch(*Source++)
				{
				case 'z':
				case 'Z':
				case 'a':
				case 'A':
					wsprintf(Dest,szNumber,Gate->area);
					Copied += strlen(Dest);
					Dest = strchr(Dest,0);
					SetArea = TRUE;
					break;
				case 'p':
				case 'P':
				case 'g':
				case 'G':
					wsprintf(Dest,szNumber,Gate->gate);
					Copied += strlen(Dest);
					Dest = strchr(Dest,0);
					SetGate = TRUE;
					break;
				case '%':
					*Dest++ = '%';
					Copied++;
					break;
				}
			continue;
			}
		*Dest++ = *Source++;
		Copied++;
		}

	*Dest = 0;

	if ((SetArea)&&((!Gate->hasarea)&&(!Gate->areaptr)))
		ZeroMemory(DestPtr,Size);
	if ((SetGate)&&((!Gate->hasgate)&&(!Gate->gateptr)))
		ZeroMemory(DestPtr,Size);

	return;
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK RectEx_GenerateProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEXGATESCTX	*Context;
	LONG			 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		RectEx_GenerateInit(hDlg,(MAPRECTEXGATESCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPRECTEXGATESCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,16);
			return(TRUE);

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 113:
					if (Context->templatetype == RECTEX_GATE) lData = Context->gatestemplate.rect.color;
					else lData = Context->areastemplate.rect.color;
					RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,lData);
					break;
				case 115:
					if (Context->templatetype == RECTEX_GATE) lData = Context->gatestemplate.rect.textcolor;
					else lData = Context->areastemplate.rect.textcolor;
					RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,lData);
					break;
				}
			Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,16,1+6,(1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5));
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_GETDISPINFO:
					RectEx_GenerateText((LV_DISPINFO *)lParam);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 112:
							lData = SendDlgItemMessage(hDlg,112,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							if (lData)
								{
								GetDlgItemText(hDlg,117,Context->gatestemplate.name,MAP_RECTMAXNAME);
								GetDlgItemText(hDlg,118,Context->gatestemplate.info,MAP_RECTMAXINFO);
								Context->templatetype = RECTEX_AREA;
								SetDlgItemText(hDlg,117,Context->areastemplate.name);
								SetDlgItemText(hDlg,118,Context->areastemplate.info);
								}
							else
								{
								GetDlgItemText(hDlg,117,Context->areastemplate.name,MAP_RECTMAXNAME);
								GetDlgItemText(hDlg,118,Context->areastemplate.info,MAP_RECTMAXINFO);
								Context->templatetype = RECTEX_GATE;
								SetDlgItemText(hDlg,117,Context->gatestemplate.name);
								SetDlgItemText(hDlg,118,Context->gatestemplate.info);
								}
							InvalidateRect(GetDlgItem(hDlg,113),NULL,FALSE);
							InvalidateRect(GetDlgItem(hDlg,115),NULL,FALSE);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
						case 102:
						case 103:
						case 104:
						case 105:
						case 106:
						case 107:
						case 108:
							RectEx_GenerateFlags(hDlg,LOWORD(wParam),Context);
							return(TRUE);

						case 114:
							if (Context->templatetype == RECTEX_AREA)
								RectEx_ChooseColor(hDlg,113,&Context->areastemplate.rect.color,0x00FFFF00);
							else
								RectEx_ChooseColor(hDlg,113,&Context->gatestemplate.rect.color,0x00AAAAAA);
							break;
						case 116:
							if (Context->templatetype == RECTEX_AREA)
								RectEx_ChooseColor(hDlg,115,&Context->areastemplate.rect.textcolor,0x00FFFF00);
							else
								RectEx_ChooseColor(hDlg,115,&Context->gatestemplate.rect.textcolor,0x00AAAAAA);
							break;

						case IDOK:
							if (RectEx_GenerateSave(hDlg,Context))
								EndDialog(hDlg,1);
							return(TRUE);

						case IDCANCEL:
							EndDialog(hDlg,0);
							return(TRUE);
						}
					break;
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void RectEx_GenerateInit(HWND hDlg, MAPRECTEXGATESCTX *Context)
{
	MAPRECTEXGATESLIST	*Gate;
	LV_COLUMN		 Column;
	LV_ITEM			 Item;
	RECT			 ClientRect;
	HICON			 hIcon;
	LONG			 X,Y;

	//
	// Valeurs par défaut
	//

	Context->flags = RECTEX_AUTOFALL;
	Context->tempflags = RECTEX_AUTOFALL;
	Context->templatetype = RECTEX_GATE;
	Context->gatestemplate.rect.color = Context->parentctx->rect.rect.color;
	Context->gatestemplate.rect.textcolor = Context->parentctx->rect.rect.textcolor;
	Context->areastemplate.rect.color = MapRectExColors[0];
	Context->areastemplate.rect.textcolor = MapRectExColors[1];
	strcpy(Context->gatestemplate.name,szNewGateName);
	strcpy(Context->gatestemplate.info,szNewGateDescription);
	strcpy(Context->areastemplate.name,szNewAreaName);
	strcpy(Context->areastemplate.info,szNewAreaDescription);

	if (SendDlgItemMessage(hDlg,112,CB_ADDSTRING,(WPARAM)0,(LPARAM)szNGGates) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,112,CB_ADDSTRING,(WPARAM)0,(LPARAM)szNGAreas) < 0) goto Error_0;
	SendDlgItemMessage(hDlg,112,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SetDlgItemText(hDlg,117,szNewGateName);
	SetDlgItemText(hDlg,118,szNewGateDescription);

	for (X = 101; X != 108+1; X++)
		CheckDlgButton(hDlg,X,BST_CHECKED);

	if (!List_EntryCount(&Context->gates))
		EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);

	//
	// Liste des images
	//

	Context->icons = ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,1,1);
	if (!Context->icons) goto Error_0;
	for (X = 87; X != 89+1; X++)
		{
		hIcon = LoadImage(hInstance,MAKEINTRESOURCE(X),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		if (!hIcon) goto Error_1;
		if (ImageList_ReplaceIcon(Context->icons,-1,hIcon) == -1)
			{
			DestroyIcon(hIcon);
			goto Error_1;
			}
		}

	//
	// Création de la liste des portails
	//

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	GetWindowRect(GetDlgItem(hDlg,120),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-30,hDlg,NULL,hInstance,NULL);
	if (!Context->hlist) goto Error_1;

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*7;
	Column.pszText = szNGColumn0;
	Column.iSubItem = 0;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column);
	Column.cx = Font.FontMaxWidth*12;
	Column.pszText = szNGColumn1;
	Column.iSubItem = 1;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column);
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*5;
	Column.pszText = szNGColumn2;
	Column.iSubItem = 2;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column);
	Column.cx = Font.FontMaxWidth*5;
	Column.pszText = szNGColumn3;
	Column.iSubItem = 3;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)3,(LPARAM)&Column);
	Column.cx = Font.FontMaxWidth*5;
	Column.pszText = szNGColumn4;
	Column.iSubItem = 4;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)4,(LPARAM)&Column);
	Column.cx = Font.FontMaxWidth*5;
	Column.pszText = szNGColumn5;
	Column.iSubItem = 5;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)5,(LPARAM)&Column);
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*30;
	Column.pszText = szNGColumn6;
	Column.iSubItem = 6;
	SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)6,(LPARAM)&Column);

	//
	// Création de la liste
	//

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.iImage = I_IMAGECALLBACK;
	for (Gate = (MAPRECTEXGATESLIST *)Context->gates.next; Gate != NULL; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
		{
		Item.lParam = (LPARAM)Gate;
		if (SendMessage(Context->hlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1)
			goto Error_1;
		Item.iItem++;
		}

	SendMessage(Context->hlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->icons);

	return;

//
//--- Erreurs ---
//

Error_1:ImageList_Destroy(Context->icons);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Sauvegarde des valeurs ««««««««««««««««««««««««««««««««««««««««««««»

int RectEx_GenerateSave(HWND hDlg, MAPRECTEXGATESCTX *Context)
{
	MAPRECTEXGATESLIST	*Gate;
	long			 NeededTriggers;
	long			 NeededActions;
	long			 NeededAreas;
	long			 NeededGates;
	long			 Areas;
	long			 Gates;

	//
	// Sauvegarde les valeurs
	//

	if (Context->templatetype == RECTEX_AREA)
		{
		GetDlgItemText(hDlg,117,Context->areastemplate.name,MAP_RECTMAXNAME);
		GetDlgItemText(hDlg,118,Context->areastemplate.info,MAP_RECTMAXINFO);
		}
	else
		{
		GetDlgItemText(hDlg,117,Context->gatestemplate.name,MAP_RECTMAXNAME);
		GetDlgItemText(hDlg,118,Context->gatestemplate.info,MAP_RECTMAXINFO);
		}

	//
	// Test s'il y a suffisamment de zones et de passages
	//

	Areas = List_EntryCount(&MapAreas);
	Gates = List_EntryCount(&MapGates);
	NeededGates = 0;
	NeededAreas = 0;
	NeededTriggers = 0;
	NeededActions = 0;

	for (Gate = (MAPRECTEXGATESLIST *)Context->gates.next; Gate != 0; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
		{
		/* Le portail n'a pas de passage */
		if (!Gate->hasgate)
			{
			if (Context->flags&RECTEX_AUTOFGRELEASE)
				NeededGates++;
			else if ((Context->flags&RECTEX_AUTOFGDISPLAY) && (Gate->node.type == 6))
				NeededGates++;
			else if ((Context->flags&RECTEX_AUTOFGDISPLAY) && (Gate->node.type == 7))
				NeededGates++;
			else if ((Context->flags&RECTEX_AUTOFGDESTROY) && (!Gate->cantbedestroyed))
				NeededGates++;
			}
		/* Le portail n'a pas de zones */
		if (!Gate->hasarea)
			{
			if (Context->flags&RECTEX_AUTOFARELEASE)
				NeededAreas++;
			else if ((Context->flags&RECTEX_AUTOFADESTROY) && (!Gate->cantbedestroyed))
				NeededAreas++;
			}
		/* Le portail n'a pas d'évènement */
		if ((!Gate->hasevent)&&(Context->flags&RECTEX_AUTOFTRIGGERS)&&(!Gate->cantbedestroyed))
			{
			NeededTriggers += Map.Properties.Players;
			NeededActions += Map.Properties.Players;
			}
		}

	if ((Areas+NeededAreas >= 256)||(Gates+NeededGates >= 256))
		{
		MESSAGEBOXSTRUCT	Request;

		ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
		Request.gadgets		= szMessageBoxExtOk;
		Request.windowtitle	= NULL;
		Request.requestbits	= &Config.RequestsFlags;
		Request.statebit	= REQUEST_GATESADDPB;
		Request.flags		= MBF_AUTOREPLY;
		Request.defaultgadget	= 1;
		Request.defaulticon	= MB_ICONEXCLAMATION;
		Request.windowpos	= MBP_FOLLOWMOUSE;
		if ((Areas+NeededAreas >= 256)&&(Gates+NeededGates >= 256))
			Request.windowtext = szNGNoEnoughRects;
		else if (Areas+NeededAreas >= 256)
			Request.windowtext = szNGNoEnoughAreas;
		else if (Gates+NeededGates >= 256)
			Request.windowtext = szNGNoEnoughGates;

		Misc_MessageBoxExtArgs(hDlg,&Request);
		return(0);
		}
	else if ((!NeededAreas)&&(!NeededGates))
		{
		Misc_MessageBoxExt(hDlg,szNGNoNeed,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
		return(0);
		}
	else
		{
		MESSAGEBOXSTRUCT	Request;

		ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
		wsprintf(Context->temp,szNGInfo,NeededGates,NeededAreas,NeededTriggers,NeededActions);
		Request.gadgets		= szMessageBoxExtYesNo;
		Request.windowtext	= Context->temp;
		Request.windowtitle	= (char *)-1;
		Request.requestbits	= &Config.RequestsFlags;
		Request.statebit	= REQUEST_GATESADD;
		Request.flags		= MBF_AUTOREPLY;
		Request.defaultgadget	= 1;
		Request.defaulticon	= MB_ICONQUESTION;
		Request.windowpos	= MBP_FOLLOWMOUSE;
		return(Misc_MessageBoxExtArgs(hDlg,&Request));
		}
}


// «»»» Copie les textes pour la LISTVIEW ««««««««««««««««««««««««««««««««»

void RectEx_GenerateText(LV_DISPINFO *DispInfo)
{
	MAPRECTEXGATESLIST	*Gate;

	Gate = (MAPRECTEXGATESLIST *)DispInfo->item.lParam;

	//
	// Place le texte
	//

	if (DispInfo->item.mask&LVIF_TEXT)
		{
		switch(DispInfo->item.iSubItem)
			{
			case 0:	wsprintf(DispInfo->item.pszText,szNGPosition,Gate->x,Gate->y);
				break;
			case 1:	switch(Gate->node.type)
					{
					case 2:	strcpy(DispInfo->item.pszText,szNGName2);
						break;
					case 3:	strcpy(DispInfo->item.pszText,szNGName3);
						break;
					case 6:	strcpy(DispInfo->item.pszText,szNGName6);
						break;
					case 7:	strcpy(DispInfo->item.pszText,szNGName7);
						break;
					default:strcpy(DispInfo->item.pszText,szUnknown);
						break;
					}
				break;
			case 2:	if (Gate->hasgate) wsprintf(DispInfo->item.pszText,szNumber,Gate->gate);
				else strcpy(DispInfo->item.pszText,szNGNA);
				break;
			case 3:	if (Gate->hasarea) wsprintf(DispInfo->item.pszText,szNumber,Gate->area);
				else strcpy(DispInfo->item.pszText,szNGNA);
				break;
			case 4:	if (Gate->hasevent) strcpy(DispInfo->item.pszText,szNGYes);
				else strcpy(DispInfo->item.pszText,szNGNo);
				break;
			case 5:	if (Gate->node.type == 3) strcpy(DispInfo->item.pszText,szNGYes);
				else strcpy(DispInfo->item.pszText,szNGNo);
				break;
			case 6:	strcpy(DispInfo->item.pszText,szNGCommentRAS);
				if (Gate->node.type == 3)
					{
					if (Gate->cantbedestroyed)
						{
						strcpy(DispInfo->item.pszText,szNGComment0);
						break;
						}
					if ((!Gate->hasgate)||(!Gate->hasarea)||(!Gate->hasevent))
						{
						strcpy(DispInfo->item.pszText,szNGComment1);
						break;
						}
					}
				else if ((Gate->node.type == 6)||(Gate->node.type == 7))
					{
					if (!Gate->hasgate)
						{
						strcpy(DispInfo->item.pszText,szNGComment2);
						break;
						}
					}
				break;
			}
		}

	//
	// Place l'image
	//

	if (DispInfo->item.mask&LVIF_IMAGE)
		{
		DispInfo->item.iImage = 2;

		if (Gate->node.type == 3)
			{
			if (Gate->cantbedestroyed) goto Done;
			if ((!Gate->hasgate)||(!Gate->hasarea)||(!Gate->hasevent)) DispInfo->item.iImage = 0;
			}
		else if ((Gate->node.type == 6)||(Gate->node.type == 7))
			{
			if (!Gate->hasgate) DispInfo->item.iImage = 1;
			}
		}

Done:	return;
}


// «»»» Modification des drapeaux «««««««««««««««««««««««««««««««««««««««»

void RectEx_GenerateFlags(HWND hDlg, UINT CtrlId, MAPRECTEXGATESCTX *Context)
{
	static DWORD Flags[]={	0,				// (101)
				RECTEX_AUTOFGDISPLAY,		//  102
				RECTEX_AUTOFGDESTROY,		//  103
				RECTEX_AUTOFGRELEASE,		//  104
				0,				// (105)
				RECTEX_AUTOFADESTROY,		//  106
				RECTEX_AUTOFARELEASE,		//  107
				RECTEX_AUTOFTRIGGERS};		//  108
	static LONG Excl[]={	101,102,103,104,108,0,
				105,106,107,108,0,
				103,108,0,
				106,108,0,
				-1};

	BOOL			Unchecked;
	int			i,j;

	Context->flags &= ~Flags[CtrlId-101];
	if (IsDlgButtonChecked(hDlg,CtrlId) == BST_CHECKED)
		Context->flags |= Flags[CtrlId-101];

	Context->tempflags &= ~Flags[CtrlId-101];
	if (IsDlgButtonChecked(hDlg,CtrlId) == BST_CHECKED)
		Context->tempflags |= Flags[CtrlId-101];

	//
	// Active/désactive les boutons liés
	//

	Unchecked = (IsDlgButtonChecked(hDlg,CtrlId) == BST_UNCHECKED)?TRUE:FALSE;
	for (i = 0; Excl[i] != -1;)
		{
		if (CtrlId == Excl[i])
			{
			for (j = 1; Excl[i+j] != 0; j++)
				{
				if (Unchecked)
					{
					EnableWindow(GetDlgItem(hDlg,Excl[i+j]),FALSE);
					CheckDlgButton(hDlg,Excl[i+j],BST_UNCHECKED);
					Context->flags &= ~Flags[Excl[i+j]-101];
					}
				else
					{
					if (Context->tempflags&Flags[Excl[i+j]-101])
						{
						CheckDlgButton(hDlg,Excl[i+j],BST_CHECKED);
						Context->flags |= Flags[Excl[i+j]-101];
						}
					else
						{
						CheckDlgButton(hDlg,Excl[i+j],BST_UNCHECKED);
						Context->flags &= ~Flags[Excl[i+j]-101];
						}
					EnableWindow(GetDlgItem(hDlg,Excl[i+j]),TRUE);
					}
				}
			}
		while(Excl[i++] != 0);
		}

	//
	// Désactive les évènements si certaines options ne sont pas activées
	//

	if ( (!(Context->flags&RECTEX_AUTOFGDESTROY)) || (!(Context->flags&RECTEX_AUTOFADESTROY)) )
		{
		EnableWindow(GetDlgItem(hDlg,108),FALSE);
		CheckDlgButton(hDlg,108,BST_UNCHECKED);
		Context->flags &= ~RECTEX_AUTOFTRIGGERS;
		}

	//
	// Désactive le bouton d'ajout si aucune option n'est sélectionnée
	//

	if (!Context->flags)
		EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
	else if (List_EntryCount(&Context->gates))
		EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie un texte en supprimant les retours à la ligne «««««««««««««»

void RectEx_StrCpy(char *Dst, char *Src)
{
	char	*SrcPtr;
	char	*DstPtr;

	SrcPtr = Src;
	DstPtr = Dst;

	do {

		switch(*SrcPtr)
			{
			case 0x0D:
				break;
			case 0x0A:
				*DstPtr++ = ' ';
				*DstPtr++ = '|';
				*DstPtr++ = ' ';
				break;
			case 0x09:
				*DstPtr++ = ' ';
				break;
			default:
				*DstPtr++ = *SrcPtr;
				break;
			}

		} while(*SrcPtr++);

	return;
}


// «»»» Détermine la première entrée libre ««««««««««««««««««««««««««««««»

int RectEx_GetFreeID(NODE *RectList)
{
	int	 Result;

	for (Result = 1;; Result++) if (!RectEx_FindById(Result,RectList)) break;
	return(Result);
}


// «»»» Retrouve une entrée en fonction de son identificateur «««««««««««»

MAPRECTEX* RectEx_FindById(UINT Id, NODE *RectList)
{
	NODE	*List;

	for (List = RectList->next; List != 0; List = List->next)
		if (((MAPRECTEX *)List)->id == Id) return((MAPRECTEX *)List);

	return(NULL);
}


// «»»» Obtention de la position d'un portail «««««««««««««««««««««««««««»

WORD RectEx_GetRectLocation(NODE *RectList, UINT RectID, UINT What)
{
	NODE	*List;
	LONG	 X,Y;

	for (List = RectList->next; List != 0; List = List->next)
		{
		if (((MAPRECTEX *)List)->id == RectID)
			{
			X = ((MAPRECTEX *)List)->rect.x1;
			Y = ((MAPRECTEX *)List)->rect.y1;
			if (!What) return((WORD)X);
			else return((WORD)Y);
			}
		}

	return(0);
}


// «»»» Obtention d'un portail en fonction de sa position «««««««««««««««»

int RectEx_GetGateAtMapPos(LONG X, LONG Y)
{
	MAPRECTEX	*Gate;

	for (Gate = (MAPRECTEX *)MapGates.next; Gate != 0; Gate = (MAPRECTEX *)Gate->node.next)
		{
		if (Gate->rect.x1 != X) continue;
		if (Gate->rect.y1 != Y) continue;
		return(Gate->id);
		}

	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END

