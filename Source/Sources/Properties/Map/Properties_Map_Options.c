
//<<>-<>>---------------------------------------------------------------------()
/*
	Options de la carte
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
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapLevelOptions(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6807),hWnd,Properties_MapLevelOptionsProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapLevelOptionsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapLevelOptionsInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_PAINT:
			Misc_PrintButtonRightText(hDlg,126,szCfgLevelOptionsPrev);
			Misc_PrintButtonRightText(hDlg,129,szCfgLevelOptionsNext);
			break;

		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				((MEASUREITEMSTRUCT *)lParam)->itemHeight = Font.FontHeight+2;
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Properties_MapLevelOptionsDrawBox((DRAWITEMSTRUCT *)lParam,MapProps);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				if (((DRAWITEMSTRUCT *)lParam)->CtlID == 101)
					Properties_MapLevelOptionsDrawHorny((DRAWITEMSTRUCT *)lParam);
				else
					Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					Misc_SetUpDownInt(hDlg,106,(NM_UPDOWN *)lParam);
					return(TRUE);
				}
			break;

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
					if ((HWND)lParam == GetDlgItem(hDlg,103))
						{
						lData = SendDlgItemMessage(hDlg,103,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,104,lData,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,111))
						{
						lData = SendDlgItemMessage(hDlg,111,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,112,lData,FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 106:
							lData = GetDlgItemInt(hDlg,106,NULL,FALSE);
							if (lData < 1) SetDlgItemInt(hDlg,106,1,FALSE);
							if (lData > 50) SetDlgItemInt(hDlg,106,50,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Properties_MapLevelOptionsSave(hDlg,MapProps);
							Misc_DestroyImageList(&MapProps->hImages);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Misc_DestroyImageList(&MapProps->hImages);
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

void Properties_MapLevelOptionsInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LONG		 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	SendDlgItemMessage(hDlg,103,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));
	SendDlgItemMessage(hDlg,103,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->MapProperties.HornyTalismanPieces);
	SetDlgItemInt(hDlg,104,MapProps->MapProperties.HornyTalismanPieces,FALSE);
	SetDlgItemInt(hDlg,106,MapProps->MapProperties.GameTicks,FALSE);
	SendDlgItemMessage(hDlg,108,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(1,3));
	SendDlgItemMessage(hDlg,108,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->MapProperties.Soundtrack);

	CheckDlgButton(hDlg,110,(MapProps->MapProperties.Options&0x20)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,113,(MapProps->MapProperties.Options&0x40)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,114,(MapProps->MapProperties.Options2&0x04)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,115,(MapProps->MapProperties.Options2&0x08)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,116,(MapProps->MapProperties.Options2&0x20)?BST_CHECKED:BST_UNCHECKED);
	SendDlgItemMessage(hDlg,111,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,16));
	SendDlgItemMessage(hDlg,111,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->MapProperties.SpecialLevelNumber);
	SetDlgItemInt(hDlg,112,MapProps->MapProperties.SpecialLevelNumber,FALSE);

	CheckDlgButton(hDlg,118,(MapProps->MapProperties.Options&0x08)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,119,(MapProps->MapProperties.Options2&0x02)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,120,(MapProps->MapProperties.Options&0x10)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,121,(MapProps->MapProperties.Options2&0x01)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,122,(MapProps->MapProperties.Options2&0x10)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,123,(MapProps->MapProperties.Options&0x04)?BST_UNCHECKED:BST_CHECKED);
	CheckDlgButton(hDlg,124,(MapProps->MapProperties.Options&0x80)?BST_CHECKED:BST_UNCHECKED);

	if (SendDlgItemMessage(hDlg,125,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,129,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	if (SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) goto Error_0;
	for (X = 0, Y = 1; Rewards[X].Id != 0; X++, Y++)
		{
		if (SendDlgItemMessage(hDlg,125,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		if (SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		if (SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		if (SendDlgItemMessage(hDlg,129,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		if (SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)Y) < 0) goto Error_0;
		}
	if (!Misc_CreateUpDownButton(hDlg,106,200)) goto Error_0;
	if (!Misc_CreateImageList(Rewards,&MapProps->hImages,Font.FontHeight+2,Font.FontHeight+2,FALSE)) goto Error_0;

	SendDlgItemMessage(hDlg,125,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SendDlgItemMessage(hDlg,126,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SendDlgItemMessage(hDlg,127,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SendDlgItemMessage(hDlg,129,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	for (X = 0; Rewards[X].Id != 0; X++)
		{
		if (Rewards[X].Id == MapProps->MapProperties.RewardPrev1) SendDlgItemMessage(hDlg,125,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		if (Rewards[X].Id == MapProps->MapProperties.RewardPrev2) SendDlgItemMessage(hDlg,126,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		if (Rewards[X].Id == MapProps->MapProperties.RewardPrev3) SendDlgItemMessage(hDlg,127,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		if (Rewards[X].Id == MapProps->MapProperties.RewardNext1) SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		if (Rewards[X].Id == MapProps->MapProperties.RewardNext2) SendDlgItemMessage(hDlg,129,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		if (Rewards[X].Id == MapProps->MapProperties.RewardNext3) SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
		}

	return;

//--- Erreurs ---

Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Sauvegarde des paramètres «««««««««««««««««««««««««««««««««««««««»

void Properties_MapLevelOptionsSave(HWND hDlg, MAPPROPSCTX *MapProps)
{
	long	Data;

	MapProps->MapProperties.Options		= 0x00;
	MapProps->MapProperties.Options2	= 0x00;
	MapProps->MapProperties.RewardPrev1	= 0x00;
	MapProps->MapProperties.RewardPrev2	= 0x00;
	MapProps->MapProperties.RewardPrev3	= 0x00;
	MapProps->MapProperties.RewardNext1	= 0x00;
	MapProps->MapProperties.RewardNext2	= 0x00;
	MapProps->MapProperties.RewardNext3	= 0x00;

	MapProps->MapProperties.HornyTalismanPieces	= (BYTE)GetDlgItemInt(hDlg,104,NULL,FALSE);
	MapProps->MapProperties.GameTicks		= (BYTE)GetDlgItemInt(hDlg,106,NULL,FALSE);
	MapProps->MapProperties.SpecialLevelNumber 	= (BYTE)GetDlgItemInt(hDlg,112,NULL,FALSE);
	MapProps->MapProperties.Soundtrack		= (BYTE)SendDlgItemMessage(hDlg,108,TBM_GETPOS,(WPARAM)0,(LPARAM)0);

	if (IsDlgButtonChecked(hDlg,110) == BST_CHECKED)   MapProps->MapProperties.Options  |= 0x20;
	if (IsDlgButtonChecked(hDlg,113) == BST_CHECKED)   MapProps->MapProperties.Options  |= 0x40;
	if (IsDlgButtonChecked(hDlg,114) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x04;
	if (IsDlgButtonChecked(hDlg,115) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x08;
	if (IsDlgButtonChecked(hDlg,116) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x20;
	if (IsDlgButtonChecked(hDlg,118) == BST_CHECKED)   MapProps->MapProperties.Options  |= 0x08;
	if (IsDlgButtonChecked(hDlg,119) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x02;
	if (IsDlgButtonChecked(hDlg,120) == BST_CHECKED)   MapProps->MapProperties.Options  |= 0x10;
	if (IsDlgButtonChecked(hDlg,121) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x01;
	if (IsDlgButtonChecked(hDlg,122) == BST_CHECKED)   MapProps->MapProperties.Options2 |= 0x10;
	if (IsDlgButtonChecked(hDlg,123) == BST_UNCHECKED) MapProps->MapProperties.Options  |= 0x04;
	if (IsDlgButtonChecked(hDlg,124) == BST_CHECKED)   MapProps->MapProperties.Options  |= 0x80;

	if ((Data = SendDlgItemMessage(hDlg,125,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardPrev1 = (BYTE)Rewards[Data-1].Id;
	if ((Data = SendDlgItemMessage(hDlg,126,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardPrev2 = (BYTE)Rewards[Data-1].Id;
	if ((Data = SendDlgItemMessage(hDlg,127,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardPrev3 = (BYTE)Rewards[Data-1].Id;
	if ((Data = SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardNext1 = (BYTE)Rewards[Data-1].Id;
	if ((Data = SendDlgItemMessage(hDlg,129,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardNext2 = (BYTE)Rewards[Data-1].Id;
	if ((Data = SendDlgItemMessage(hDlg,130,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)) != 0) MapProps->MapProperties.RewardNext3 = (BYTE)Rewards[Data-1].Id;

	if ((MapProps->MapProperties.Options&0x20)&&(MapProps->MapProperties.Options2&0x40)) MapProps->MapProperties.SpecialLevelNumber = 0;
	return;
}


// «»»» Affichage d'une récompense ««««««««««««««««««««««««««««««««««««««»

void Properties_MapLevelOptionsDrawBox(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps)
{
	RECT		 WorkRect;
	COLORREF	 OldTextColor;
	COLORREF	 OldBackColor;
	COLORREF	 BackColor;
	HBRUSH		 Brush,OldBrush;
	char		*Text;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 1;
	WorkRect.top += 1;
	WorkRect.right -= 1;
	WorkRect.bottom -= 1;
	BackColor = GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW);
	OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	OldBackColor = SetBkColor(ItemStruct->hDC,BackColor);

	if (ItemStruct->itemData != 0)
		Text = Rewards[ItemStruct->itemData-1].Name;
	else
		Text = szCfgLevelOptionsNone;

	if (ItemStruct->itemState&ODS_DISABLED)
		{
		SetBkColor(ItemStruct->hDC,GetSysColor(COLOR_MENU));
		SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
		}

	//--- Draws the image ---

	WorkRect.right = WorkRect.left+Font.FontHeight+2;
	if (ItemStruct->itemData) ImageList_DrawEx(MapProps->hImages,ItemStruct->itemData-1,ItemStruct->hDC,WorkRect.left,WorkRect.top,Font.FontHeight+2,Font.FontHeight+2,CLR_DEFAULT,CLR_DEFAULT,ILD_NORMAL);
	WorkRect.left = WorkRect.right+1;
	WorkRect.right = ItemStruct->rcItem.right-1;

	//--- Fills the control background ---

	Brush = CreateSolidBrush(BackColor);
	if (Brush)
		{
		OldBrush = SelectObject(ItemStruct->hDC,Brush);
		if (OldBrush)
			{
			FillRect(ItemStruct->hDC,&WorkRect,Brush);
			SelectObject(ItemStruct->hDC,OldBrush);
			}
		DeleteObject(Brush);
		}
	if (ItemStruct->itemState&ODS_FOCUS)
		DrawFocusRect(ItemStruct->hDC,&WorkRect);

	//--- Draws the player's text ---

	WorkRect.left++;
	DrawText(ItemStruct->hDC,Text,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	SetBkColor(ItemStruct->hDC,OldBackColor);
	SetTextColor(ItemStruct->hDC,OldTextColor);
	return;
}


// «»»» Affichage du talisman d'Horny «««««««««««««««««««««««««««««««««««»

void Properties_MapLevelOptionsDrawHorny(DRAWITEMSTRUCT *ItemStruct)
{
	HDC	hDC;
	HBITMAP	hBitmap;
	HBITMAP	hOldBitmap;

	hDC = CreateCompatibleDC(ItemStruct->hDC);
	if (hDC)
		{
		hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(997),IMAGE_BITMAP,ItemStruct->rcItem.right-ItemStruct->rcItem.left,ItemStruct->rcItem.bottom-ItemStruct->rcItem.top,LR_DEFAULTCOLOR);
		if (hBitmap)
			{
			hOldBitmap = SelectObject(hDC,hBitmap);
			if (hOldBitmap)
				{
				BitBlt(ItemStruct->hDC,ItemStruct->rcItem.left,ItemStruct->rcItem.top,ItemStruct->rcItem.right-ItemStruct->rcItem.left,ItemStruct->rcItem.bottom-ItemStruct->rcItem.top,hDC,0,0,SRCCOPY);
				SelectObject(hDC,hOldBitmap);
				}
			DeleteObject(hBitmap);
			}
		DeleteDC(hDC);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
