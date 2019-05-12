
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres des alliances
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Etablissement des alliances					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapAlliances(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6802),hWnd,Properties_MapAlliancesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapAlliancesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		MapProps = (MAPPROPSCTX *)lParam;
		CopyMemory(&MapProps->Allies[0][0],&MapProps->MapProperties.Allies[0][0],MAP_MAXPLAYERS*MAP_MAXPLAYERS);
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesView);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesSet);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesClear);
		SendDlgItemMessage(hDlg,101,CB_SETCURSEL,(WPARAM)1,(LPARAM)0);
		SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Properties_MapAlliancesDisplay((DRAWITEMSTRUCT *)lParam,MapProps);
			return(TRUE);

		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE) break;
			Properties_MapAlliancesHide(hDlg,MapProps);
			break;

		case WM_LBUTTONDOWN:
			Properties_MapAlliancesHide(hDlg,MapProps);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					Properties_MapAlliancesHide(hDlg,MapProps);
					return(TRUE);
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 100:
							Properties_MapAlliancesButton(hDlg,MapProps);
							break;
						case 102:
							Properties_MapAlliancesDefault(hDlg,MapProps);
							break;
						case IDOK:
							CopyMemory(&MapProps->MapProperties.Allies[0][0],&MapProps->Allies[0][0],MAP_MAXPLAYERS*MAP_MAXPLAYERS);
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


// «»»» Alliances par défaut ««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesDefault(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HWND	hCtrl;
	int	i,j;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		for (j = 0; j != MAP_MAXPLAYERS; j++)
			MapProps->Allies[i][j] = 0;

	MapProps->Allies[1][0] = 1;
	MapProps->Allies[1][2] = 1;
	MapProps->Allies[1][3] = 1;
	MapProps->Allies[1][4] = 1;
	MapProps->Allies[1][5] = 1;
	MapProps->Allies[1][6] = 1;
	MapProps->Allies[0][1] = 1;
	MapProps->Allies[2][1] = 1;
	MapProps->Allies[3][1] = 1;
	MapProps->Allies[4][1] = 1;
	MapProps->Allies[5][1] = 1;
	MapProps->Allies[6][1] = 1;

	hCtrl = GetDlgItem(hDlg,100);
	InvalidateRect(hCtrl,NULL,TRUE);
	UpdateWindow(hCtrl);
	return;
}


// «»»» Désélectionne les boutons «««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesHide(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HWND	hCtrl;
	int	i;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		{
		MapProps->Selected[0][i] = 0;
		MapProps->Selected[1][i] = 0;
		}
			
	hCtrl = GetDlgItem(hDlg,100);
	InvalidateRect(hCtrl,NULL,FALSE);
	UpdateWindow(hCtrl);
	return;
}


// «»»» Sélection des boutons «««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesButton(HWND hDlg, MAPPROPSCTX *MapProps)
{
	POINT	CursorPos;
	RECT	ButtonRect;
	HWND	hCtrl;
	UINT	Mode;
	UINT	SideSelected;
	UINT	ButtonSelected;
	BOOL	EraseBckgnd = FALSE;
	long	Height;
	long	StartY;
	int	i;

	hCtrl = GetDlgItem(hDlg,100);
	GetCursorPos(&CursorPos);
	GetWindowRect(hDlg,&ButtonRect);
	if (!PtInRect(&ButtonRect,CursorPos)) goto UnSelect;
	MapWindowPoints(NULL,hCtrl,&CursorPos,1);
	GetClientRect(hCtrl,&ButtonRect);

	Height = ButtonRect.bottom-ButtonRect.top;
	StartY = (Height%MAP_MAXPLAYERS)/2;
	Height = Height/MAP_MAXPLAYERS;

	if (CursorPos.y < StartY) goto UnSelect;
	if ((CursorPos.x > 100)&&(CursorPos.x < ButtonRect.right-100)) goto UnSelect;
	CursorPos.y -= StartY;
	if (CursorPos.y >= Height*MAP_MAXPLAYERS) goto UnSelect;
	CursorPos.y /= Height;
	if (CursorPos.x < 100) CursorPos.x = 0;
	else CursorPos.x = 1;

	Mode = SendDlgItemMessage(hDlg,101,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	switch(Mode)
		{
		case 0:	for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				MapProps->Selected[0][i] = 0;
				MapProps->Selected[1][i] = 0;
				}
			MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
			break;
		case 1:
		case 2:	SideSelected = 2;
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				if (MapProps->Selected[0][i])
					{
					SideSelected = 0;
					ButtonSelected = i;
					break;
					}
				if (MapProps->Selected[1][i])
					{
					SideSelected = 1;
					ButtonSelected = i;
					break;
					}
				}
			/* First button to be selected ? */
			if (SideSelected == 2)
				{
				MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
				break;
				}
			/* The same button have been selected ? */
			if ((CursorPos.x == SideSelected)&&(CursorPos.y == ButtonSelected))
				{
				MapProps->Selected[CursorPos.x][CursorPos.y] = 0;
				break;
				}
			/* The same player have been selected in different row ? */
			if (CursorPos.y == ButtonSelected)
				{
				MapProps->Selected[SideSelected][ButtonSelected] = 0;
				MessageBeep(MB_ICONHAND);
				break;
				}
			/* A different player have been selected in the same row ? */
			if (CursorPos.x == SideSelected)
				{
				MapProps->Selected[SideSelected][ButtonSelected] = 0;
				MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
				break;
				}
			/* Set or clear alliance */
			MapProps->Selected[SideSelected][ButtonSelected] = 0;
			if (Mode == 1)
				{
				MapProps->Allies[ButtonSelected][CursorPos.y] = 1;
				MapProps->Allies[CursorPos.y][ButtonSelected] = 1;
				}
			else
				{
				MapProps->Allies[ButtonSelected][CursorPos.y] = 0;
				MapProps->Allies[CursorPos.y][ButtonSelected] = 0;
				}
			EraseBckgnd = TRUE;
			break;
		}

	InvalidateRect(hCtrl,NULL,EraseBckgnd);
	UpdateWindow(hCtrl);
	return;

//--- Désélectionne les boutons (cliqué hors du champ) ---

UnSelect:
	Properties_MapAlliancesHide(hDlg,MapProps);
	return;
}


// «»»» Affichage des alliances «««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesDisplay(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps)
{
	RECT	DrawRect;
	RECT	EdgeRect;
	RECT	ColorRect;
	HBRUSH	PlayerBrush;
	HPEN	DrawPenShadow;
	HPEN	DrawPenLight;
	HPEN	OldPen;
	HBRUSH	OldBrush;
	UINT	Flags,Flags2;
	long	Height;
	long	StartY;
	int	i,j;

	if (ItemStruct->CtlType != ODT_BUTTON) return;
	if (ItemStruct->CtlID != 100) return;
	if (!(ItemStruct->itemAction&ODA_DRAWENTIRE)) return;

	Height = ItemStruct->rcItem.bottom-ItemStruct->rcItem.top;
	StartY = (Height%MAP_MAXPLAYERS)/2;
	Height = Height/MAP_MAXPLAYERS;
	DrawRect.top = StartY+ItemStruct->rcItem.top;

	//--- Affichage des boutons des joueurs ---

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		{
		EdgeRect.top = DrawRect.top;
		EdgeRect.bottom = EdgeRect.top+Height;
		EdgeRect.left = ItemStruct->rcItem.left;
		EdgeRect.right = EdgeRect.left+100;

		PlayerBrush = CreateSolidBrush(Players[i].Color);
		if (PlayerBrush)
			{
			OldBrush = SelectObject(ItemStruct->hDC,PlayerBrush);
			if (!OldBrush) DeleteObject(PlayerBrush);
			}
		else
			OldBrush = NULL;

		//
		// Affichage des boutons de gauche
		//

		if (MapProps->Selected[0][i])
			{
			Flags = BDR_SUNKENINNER|BDR_SUNKENOUTER;
			Flags2 = BDR_RAISEDOUTER;
			}
		else
			{
			Flags = BDR_RAISEDINNER|BDR_RAISEDOUTER;
			Flags2 = BDR_SUNKENOUTER;
			}
		DrawEdge(ItemStruct->hDC,&EdgeRect,Flags,BF_RECT);

		ColorRect.top = DrawRect.top+6;
		ColorRect.left = ItemStruct->rcItem.left+6;
		ColorRect.bottom = DrawRect.top+Height-6;
		ColorRect.right = ColorRect.left+14;
		DrawEdge(ItemStruct->hDC,&ColorRect,Flags2,BF_RECT);
		if (OldBrush)
			{
			ColorRect.top++;
			ColorRect.left++;
			ColorRect.right--;
			ColorRect.bottom--;
			FillRect(ItemStruct->hDC,&ColorRect,PlayerBrush);
			}

		//
		// Affichage des boutons de droite
		//

		EdgeRect.left = ItemStruct->rcItem.right-100;
		EdgeRect.right = EdgeRect.left+100;
		if (MapProps->Selected[1][i])
			{
			Flags = BDR_SUNKENINNER|BDR_SUNKENOUTER;
			Flags2 = BDR_RAISEDOUTER;
			}
		else
			{
			Flags = BDR_RAISEDINNER|BDR_RAISEDOUTER;
			Flags2 = BDR_SUNKENOUTER;
			}
		DrawEdge(ItemStruct->hDC,&EdgeRect,Flags,BF_RECT);

		ColorRect.top = DrawRect.top+6;
		ColorRect.left = ItemStruct->rcItem.right-100+6;
		ColorRect.bottom = DrawRect.top+Height-6;
		ColorRect.right = ColorRect.left+14;
		DrawEdge(ItemStruct->hDC,&ColorRect,Flags2,BF_RECT);
		if (OldBrush)
			{
			ColorRect.top++;
			ColorRect.left++;
			ColorRect.right--;
			ColorRect.bottom--;
			FillRect(ItemStruct->hDC,&ColorRect,PlayerBrush);
			}

		//
		// Affichage des textes
		//

		DrawRect.bottom = DrawRect.top+Height;
		DrawRect.left = ItemStruct->rcItem.left+20;
		DrawRect.right = DrawRect.left+100-28;
		DrawText(ItemStruct->hDC,Players[i].Name,-1,&DrawRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

		DrawRect.left = ItemStruct->rcItem.right-100+20;
		DrawRect.right = DrawRect.left+100-28;
		DrawText(ItemStruct->hDC,Players[i].Name,-1,&DrawRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

		DrawRect.top += Height;
		if (OldBrush)
			{
			SelectObject(ItemStruct->hDC,OldBrush);
			DeleteObject(PlayerBrush);
			}
		}

	//--- Affichage des alliances ---

	DrawPenShadow = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
	DrawPenLight = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHIGHLIGHT));
	if ((DrawPenShadow)&&(DrawPenLight))
		{
		OldPen = SelectObject(ItemStruct->hDC,DrawPenShadow);
		if (OldPen)
			{
			DrawRect.top = StartY+ItemStruct->rcItem.top;
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				EdgeRect.top = StartY+ItemStruct->rcItem.top;
				for (j= 0; j != MAP_MAXPLAYERS; j++, EdgeRect.top += Height)
					{
					if (i == j) continue;

					if ((MapProps->Selected[0][i])||(MapProps->Selected[1][j]))
						SelectObject(ItemStruct->hDC,DrawPenLight);
					else
						SelectObject(ItemStruct->hDC,DrawPenShadow);

					if ((MapProps->Allies[i][j])||(MapProps->Allies[j][i]))
						{
						MoveToEx(ItemStruct->hDC,ItemStruct->rcItem.left+102,DrawRect.top+Height/2,NULL);
						LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-102,EdgeRect.top+Height/2);
						}

					SelectObject(ItemStruct->hDC,OldPen);
					}
				DrawRect.top += Height;
				}
			}
		}
	if (DrawPenShadow) DeleteObject(DrawPenShadow);
	if (DrawPenLight)  DeleteObject(DrawPenLight);

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
