
//<<>-<>>---------------------------------------------------------------------()
/*
	Variables
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des joueurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapVariables(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6806),hWnd,Properties_MapVariablesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapVariablesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	long		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapVariablesInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = Font.FontHeight+4;
			return(TRUE);

		case WM_DRAWITEM:
			Properties_MapVariablesDraw((DRAWITEMSTRUCT *)lParam,MapProps);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case NM_DBLCLK:
					Properties_MapVariablesModify(hDlg,MapProps);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((ELEMENT *)(((NM_LISTVIEW *)lParam)->lParam))->Id == 0xFFFFFFFF)
						EnableWindow(GetDlgItem(hDlg,101),FALSE);
					else
						EnableWindow(GetDlgItem(hDlg,101),TRUE);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_MapVariablesModify(hDlg,MapProps);
							break;
						case 102:
							for (lData = 0; MapVars[lData].Id != 0; lData++)
								{
								if (MapVars[lData].Id == 0xFFFFFFFF) continue;
								MapProps->Variables[MapVars[lData].Id] = MapVars[lData].Variable;
								}
							InvalidateRect(MapProps->hVariables,NULL,FALSE);
							break;
						case IDOK:
							CopyMemory(&MapProps->MapProperties.Variables[0],&MapProps->Variables[0],256*4);
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


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapVariablesInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	LONG		 X,Y;

	CopyMemory(&MapProps->Variables[0],&MapProps->MapProperties.Variables[0],256*4);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	//--- Initialisation de la liste ---

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hVariables = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hVariables) goto Error_0;

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*70;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hVariables,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iSubItem = 0;

	for (X = 0; MapVars[X].Id != 0; X++)
		{
		Item.iItem = X;
		Item.pszText = MapVars[X].Name;
		Item.lParam = (LPARAM)&MapVars[X];
		if (SendMessage(MapProps->hVariables,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
		}

	return;

//--- Erreurs ---

Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Affichage des variables «««««««««««««««««««««««««««««««««««««««««»

void Properties_MapVariablesDraw(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps)
{
	HPEN	 LinePen,OldPen;
	SIZE	 Size;
	COLORREF OldColor;
	LV_ITEM  FarItem;
	RECT	 WorkRect;
	SIZE	 TextSize;
	HFONT	 hOldFont;
	BOOL	 Selected;
	int	 LineY;
	char	 TempValue[64];

	if (ItemStruct->CtlType != ODT_LISTVIEW) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	FarItem.mask = LVIF_PARAM;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&FarItem);
	if (!FarItem.lParam) return;
	if (((ELEMENT *)FarItem.lParam)->Id != 0xFFFFFFFF) goto Valid_Entry;

//--- Affichage d'un séparateur ---

Separator:
	WorkRect.left += 2;

	LineY = ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top)/2;
	LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DLIGHT));
	if (LinePen)
		{
		OldPen = SelectObject(ItemStruct->hDC,LinePen);
		MoveToEx(ItemStruct->hDC,WorkRect.left,LineY,NULL);
		LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-2,LineY);
		SelectObject(ItemStruct->hDC,OldPen);
		DeleteObject(LinePen);
		}
	LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
	if (LinePen)
		{
		OldPen = SelectObject(ItemStruct->hDC,LinePen);
		MoveToEx(ItemStruct->hDC,WorkRect.left,LineY+1,NULL);
		LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-2,LineY+1);
		SelectObject(ItemStruct->hDC,OldPen);
		DeleteObject(LinePen);
		}

	OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_WINDOWTEXT));
	hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
	GetTextExtentPoint32(ItemStruct->hDC,((ELEMENT *)FarItem.lParam)->Name,strlen(((ELEMENT *)FarItem.lParam)->Name),&Size);
	WorkRect.left += 8;
	WorkRect.right = WorkRect.left+Size.cx+4;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_WINDOW+1));
	WorkRect.left += 2;
	DrawText(ItemStruct->hDC,((ELEMENT *)FarItem.lParam)->Name,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
	SelectObject(ItemStruct->hDC,hOldFont);
	SetTextColor(ItemStruct->hDC,OldColor);
	return;

//--- Affichage d'une variable ---

Valid_Entry:
	WorkRect.left += 2;

	if (ItemStruct->itemState&ODS_SELECTED)
		Selected = TRUE;
	else
		Selected = FALSE;

	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)((Selected)?(COLOR_HIGHLIGHT+1):(COLOR_WINDOW+1)));
	hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);

	/* Draws the number */

	WorkRect.left += 2;
	WorkRect.right = WorkRect.left+Font.FontMaxWidth*10;

	if (MapProps->Variables[((ELEMENT *)FarItem.lParam)->Id] == ((ELEMENT *)FarItem.lParam)->Variable)
		OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
	else
		{
		OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
		GetTextExtentPoint32(ItemStruct->hDC,"->",2,&Size);
		DrawText(ItemStruct->hDC,"->",-1,&WorkRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX);
		WorkRect.left += Size.cx+2;
		}

	if (((ELEMENT *)FarItem.lParam)->Icon)
		Misc_FloatToText(MapProps->Variables[((ELEMENT *)FarItem.lParam)->Id],TempValue);
	else
		wsprintf(TempValue,szNumber,MapProps->Variables[((ELEMENT *)FarItem.lParam)->Id]);

	DrawText(ItemStruct->hDC,TempValue,-1,&WorkRect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER|DT_NOPREFIX);
	SetTextColor(ItemStruct->hDC,OldColor);

	/* Draws the text */

	OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
	GetTextExtentPoint32(ItemStruct->hDC,((ELEMENT *)FarItem.lParam)->Name,strlen(((ELEMENT *)FarItem.lParam)->Name),&TextSize);
	WorkRect.left  = WorkRect.right+8;
	WorkRect.right = ItemStruct->rcItem.right;
	DrawText(ItemStruct->hDC,((ELEMENT *)FarItem.lParam)->Name,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
	SetTextColor(ItemStruct->hDC,OldColor);

	SelectObject(ItemStruct->hDC,hOldFont);
	return;
}


// «»»» Modification d'une variable «««««««««««««««««««««««««««««««««««««»

void Properties_MapVariablesModify(HWND hDlg, MAPPROPSCTX *MapProps)
{
	MESSAGEBOXSTRUCT	Message;
	LV_ITEM			Item;
	int			Result;
	char			TempText[64];

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(MapProps->hVariables,LVM_GETNEXTITEM,(WPARAM)LVNI_ALL,(LPARAM)LVNI_SELECTED);
	if (Item.iItem == -1) return;
	Item.iSubItem = 0;
	if (!SendMessage(MapProps->hVariables,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;
	if (((ELEMENT *)Item.lParam)->Id == 0xFFFFFFFF) return;

	if (((ELEMENT *)Item.lParam)->Icon)
		{
		Misc_MessageBoxExt(hDlg,szCfgVariablesErr,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	ZeroMemory(&Message,sizeof(MESSAGEBOXSTRUCT));
	wsprintf(TempText,szCfgVariablesText,((ELEMENT *)Item.lParam)->Variable);
	Message.gadgets = szCfgVariablesGadgets;
	Message.windowtext = TempText;
	Message.windowtitle = szCfgVariablesModify;
	Message.editnumber = MapProps->Variables[((ELEMENT *)Item.lParam)->Id];
	Message.flags = MBF_EDITTEXT|MBF_NUMBER|MBF_NOBEEP;
	Message.defaultgadget = 1;
	Message.defaulticon = MB_ICONQUESTION;
	Message.min = ((ELEMENT *)Item.lParam)->MinWidth;
	Message.max = ((ELEMENT *)Item.lParam)->MinHeight;
	Message.windowpos = MBP_FOLLOWMOUSE;
	Result = Misc_MessageBoxExtArgs(hDlg,&Message);

	switch(Result)
		{
		case 0:	return;
		case 1:	MapProps->Variables[((ELEMENT *)Item.lParam)->Id] = Message.editnumber;
			break;
		case 2:	MapProps->Variables[((ELEMENT *)Item.lParam)->Id] = ((ELEMENT *)Item.lParam)->Variable;
			break;
		}

	InvalidateRect(MapProps->hVariables,NULL,FALSE);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
