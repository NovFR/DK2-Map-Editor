
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres des joueurs
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
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des joueurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapPlayer(HWND hWnd, MAPPROPSCTX *MapProps, UINT Player)
{
	int	Result;

	MapProps->PlayerToModify = Player;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6801),hWnd,Properties_MapPlayerProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapPlayerProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapPlayerInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			((MEASUREITEMSTRUCT *)lParam)->itemWidth = 32+2+10+16;
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = 32+2;
			return(TRUE);

		case WM_DRAWITEM:
			Properties_MapPlayerDrawItem((DRAWITEMSTRUCT *)lParam,MapProps,1);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR FAR *) lParam)->code)
				{
				case LVN_GETDISPINFO:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapPlayerCreaturesDispInfo((LV_DISPINFO *)lParam,MapProps);
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hRooms)
						Properties_MapPlayerRoomsDispInfo((LV_DISPINFO *)lParam,MapProps);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapPlayerCreaturesChanged(hDlg,MapProps);
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hRooms)
						Properties_MapPlayerRoomsChanged(hDlg,MapProps);
					return(TRUE);
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 115:
							Misc_SetUpDownInt(hDlg,105,(NM_UPDOWN *)lParam);
							break;
						case 132:
							Misc_SetUpDownInt(hDlg,131,(NM_UPDOWN *)lParam);
							break;
						case 136:
							Misc_SetUpDownInt(hDlg,126,(NM_UPDOWN *)lParam);
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
						case 101:
							GetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName,64);
							break;
						case 105:
							lData = GetDlgItemInt(hDlg,105,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,105,0,FALSE);
							if (lData > 999999) SetDlgItemInt(hDlg,105,999999,FALSE);
							MapProps->CurrentPlayer.PlayerGold = GetDlgItemInt(hDlg,105,NULL,FALSE);
							break;
						case 126:
							lData = GetDlgItemInt(hDlg,126,NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,126,0,FALSE);
								break;
								}
							if (lData > 999)
								{
								SetDlgItemInt(hDlg,126,999,FALSE);
								break;
								}
							Properties_MapPlayerCreaturesChange(hDlg,FALSE,MapProps);
							break;
						case 131:
							lData = GetDlgItemInt(hDlg,131,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,131,0,FALSE);
							if (lData > 100) SetDlgItemInt(hDlg,131,100,FALSE);
							MapProps->CurrentPlayer.PlayerHealth = GetDlgItemInt(hDlg,131,NULL,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 102:
							if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED)
								{
								MapProps->CurrentPlayer.PlayerComputer = TRUE;
								EnableWindow(GetDlgItem(hDlg,103),TRUE);
								}
							else
								{
								MapProps->CurrentPlayer.PlayerComputer = FALSE;
								EnableWindow(GetDlgItem(hDlg,103),FALSE);
								}
							break;
						case 103:
							Properties_MapComputerOptions(hDlg,MapProps);
							break;

						case 110:
							if (IsDlgButtonChecked(hDlg,110) == BST_CHECKED)
								{
								MapProps->CurrentPlayer.PlayerCameraX = MapProps->MapCameraX;
								MapProps->CurrentPlayer.PlayerCameraY = MapProps->MapCameraY;
								EnableWindow(GetDlgItem(hDlg,111),TRUE);
								}
							else
								{
								MapProps->CurrentPlayer.PlayerCameraX = -1;
								MapProps->CurrentPlayer.PlayerCameraY = -1;
								EnableWindow(GetDlgItem(hDlg,111),FALSE);
								}
							break;
						case 111:
							Properties_MapPlayerSetCamera(hDlg,MapProps);
							break;

						case 118:
						case 119:
						case 120:
						case 121:
						case 122:
							Properties_MapPlayerRoomsChange(hDlg,LOWORD(wParam),MapProps);
							break;

						case 127:
							Properties_MapPlayerCreaturesChange(hDlg,TRUE,MapProps);
							break;

						case 129:
							Properties_MapPlayerToDefault(hDlg,MapProps);
							break;
						case IDOK:
							Properties_MapPlayerReset(hDlg,TRUE,MapProps);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Properties_MapPlayerReset(hDlg,FALSE,MapProps);
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

void Properties_MapPlayerInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	LONG		 X,Y;
	LONG		 Width;
	int		 i;

	CopyMemory(&MapProps->CurrentPlayer,&MapProps->Players[MapProps->PlayerToModify],sizeof(MAPOPTIONS));
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	//--- Création des informations ---

	Misc_CreateUpDownButton(hDlg,131,132);
	Misc_CreateUpDownButton(hDlg,105,115);
	Misc_CreateUpDownButton(hDlg,126,136);
	SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)32,(LPARAM)0);
	SendDlgItemMessage(hDlg,126,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)7,(LPARAM)0);
	SendDlgItemMessage(hDlg,131,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	EnableWindow(GetDlgItem(hDlg,126),FALSE);
	EnableWindow(GetDlgItem(hDlg,136),FALSE);

	SetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName);
	if (MapProps->CurrentPlayer.PlayerComputer)
		{
		CheckDlgButton(hDlg,102,BST_CHECKED);
		EnableWindow(GetDlgItem(hDlg,103),TRUE);
		}
	SetDlgItemInt(hDlg,105,MapProps->CurrentPlayer.PlayerGold,FALSE);
	SetDlgItemInt(hDlg,131,MapProps->CurrentPlayer.PlayerHealth,FALSE);
	Y = Compiler_InfoGoldAmountInRect(COMPILER_QUERYMAPGOLD|COMPILER_QUERYMAPGEMS,(BOOL *)&X,NULL);
	if (!X)
		SetDlgItemInt(hDlg,107,Y,FALSE);
	else 
		{
		char	Number[32];
		wsprintf(Number,szNumberPlus,Y);
		SetDlgItemText(hDlg,107,Number);
		}
	SetDlgItemInt(hDlg,109,Compiler_InfoTreasuryCapacityInRect(MapProps->PlayerToModify+1,NULL),FALSE);

	if (!Compiler_InfoHeartPosition(MapProps->PlayerToModify+1,&MapProps->MapCameraX,&MapProps->MapCameraY))
		{
		MapProps->MapCameraX = 1;
		MapProps->MapCameraY = 1;
		}
	if (MapProps->CurrentPlayer.PlayerCameraX != -1)
		{
		CheckDlgButton(hDlg,110,BST_CHECKED);
		EnableWindow(GetDlgItem(hDlg,111),TRUE);
		MapProps->MapCameraX = MapProps->CurrentPlayer.PlayerCameraX;
		MapProps->MapCameraY = MapProps->CurrentPlayer.PlayerCameraY;
		}

	//--- Désactive les options pour les joueurs gérés par l'ordinateur ---

	if ((MapProps->PlayerToModify == 0)||(MapProps->PlayerToModify == 1))
		{
		for (X = 100; X != 115+1; X++)
			EnableWindow(GetDlgItem(hDlg,X),FALSE);
		EnableWindow(GetDlgItem(hDlg,130),FALSE);
		EnableWindow(GetDlgItem(hDlg,131),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
		}

	//--- Initialise les images ---

	if (!Misc_CreateImageList(VarRooms,&MapProps->hImages,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(Traps,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Doors,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Spells,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(BadCreatures,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(BadElites,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Heros,&MapProps->hImages,32,32,TRUE)) goto Error_0;

	Y = 1;

	//--- Initialisation de la liste des éléments ---

	GetWindowRect(GetDlgItem(hDlg,116),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hRooms = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hRooms) goto Error_1;
	SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Width = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Width*80/100;
	Column.pszText = szCfgColumn0;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_2;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Width*20/100;
	Column.pszText = szCfgColumn1;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hRooms,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_2;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szMapRooms;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (X = 1, i = 0; VarRooms[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = VarRooms[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szTraps;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Traps[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Traps[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szDoors;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Doors[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Doors[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szSpells;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Spells[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Spells[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}

	//--- Initialisation de la liste des créatures ---

	GetWindowRect(GetDlgItem(hDlg,123),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hCreatures = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hCreatures) goto Error_2;
	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Width = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-4;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Width*80/100;
	Column.pszText = szCfgColumn2;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_3;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Width*20/100;
	Column.pszText = szCfgColumn3;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_3;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szBadCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (X = 1, i = 0; BadCreatures[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadCreatures[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}
	Item.iItem = X++;
	Item.pszText = szBadCreaturesElite;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (i = 0; BadElites[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadElites[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}
	Item.iItem = X++;
	Item.pszText = szGoodCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (i = 0; Heros[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Heros[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}

	return;

//--- Erreurs ---

Error_3:SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_2:SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_1:ImageList_Destroy(MapProps->hImages);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Réinitialisation des données ««««««««««««««««««««««««««««««««««««»

void Properties_MapPlayerReset(HWND hDlg, BOOL Save, MAPPROPSCTX *MapProps)
{
	LONG	Gold;

	if (Save)
		{
		Gold = Compiler_InfoTreasuryCapacityInRect(MapProps->PlayerToModify+1,NULL);
		if ( (MapProps->CurrentPlayer.PlayerGold > Gold) && (MapProps->PlayerToModify != 0) && (MapProps->PlayerToModify != 1) )
			{
			MESSAGEBOXSTRUCT	Request;

			ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
			Request.gadgets = szMessageBoxExtOk;
			Request.windowtext = szCfgTooManyGold;
			Request.windowtitle = (char *)-1;
			Request.requestbits = &Config.RequestsFlags;
			Request.statebit = REQUEST_MAPGOLD;
			Request.flags = MBF_AUTOREPLY;
			Request.defaultgadget = 1;
			Request.defaulticon = MB_ICONINFORMATION;
			Request.windowpos = MBP_FOLLOWMOUSE;
			Misc_MessageBoxExtArgs(hDlg,&Request);
			}
		CopyMemory(&MapProps->Players[MapProps->PlayerToModify],&MapProps->CurrentPlayer,sizeof(MAPOPTIONS));
		}

	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	ImageList_Destroy(MapProps->hImages);
	return;
}


// «»»» Valeurs par défaut ««««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapPlayerToDefault(HWND hDlg, MAPPROPSCTX *MapProps)
{
	Properties_MapOptionsToDefault(&MapProps->CurrentPlayer,MapProps->PlayerToModify+1);
	InvalidateRect(MapProps->hRooms,NULL,FALSE);
	InvalidateRect(MapProps->hCreatures,NULL,FALSE);
	UpdateWindow(MapProps->hRooms);
	UpdateWindow(MapProps->hCreatures);
	Properties_MapPlayerRoomsChanged(hDlg,MapProps);
	Properties_MapPlayerCreaturesChanged(hDlg,MapProps);
	SetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName);
	SetDlgItemInt(hDlg,105,MapProps->CurrentPlayer.PlayerGold,FALSE);
	if (MapProps->CurrentPlayer.PlayerComputer)
		{
		CheckDlgButton(hDlg,102,BST_CHECKED);
		EnableWindow(GetDlgItem(hDlg,103),TRUE);
		}
	else
		{
		CheckDlgButton(hDlg,102,BST_UNCHECKED);
		EnableWindow(GetDlgItem(hDlg,103),FALSE);
		}
	return;
}


// «»»» Affichage des entrées de la liste «««««««««««««««««««««««««««««««»

void Properties_MapPlayerDrawItem(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps, LONG Columns)
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
	char	 ItemText[64];

	if (ItemStruct->CtlType != ODT_LISTVIEW) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	FarItem.mask = LVIF_PARAM;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&FarItem);
	if (FarItem.lParam) goto Valid_Entry;

//--- Affichage d'une entrée sans icône (Séparateur) ---

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

	FarItem.mask = LVIF_TEXT;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	FarItem.pszText = ItemText;
	FarItem.cchTextMax = 64;
	if (SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem))
		{
		OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_WINDOWTEXT));
		hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&Size);
		WorkRect.left += 8;
		WorkRect.right = WorkRect.left+Size.cx+4;
		FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_WINDOW+1));
		WorkRect.left += 2;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		SelectObject(ItemStruct->hDC,hOldFont);
		SetTextColor(ItemStruct->hDC,OldColor);
		}
	return;

//--- Affichage d'une entrée avec un icône ---

Valid_Entry:
	WorkRect.left += 2+32;

	if (ItemStruct->itemState&ODS_SELECTED)
		Selected = TRUE;
	else
		Selected = FALSE;

	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)((Selected)?(COLOR_HIGHLIGHT+1):(COLOR_WINDOW+1)));

	/* Draws the bitmap */

	ImageList_Draw(MapProps->hImages,FarItem.lParam-1,ItemStruct->hDC,ItemStruct->rcItem.left+2,ItemStruct->rcItem.top+1,ILD_TRANSPARENT);

	/* Draws the text */

	FarItem.mask = LVIF_TEXT;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	FarItem.pszText = ItemText;
	FarItem.cchTextMax = 64;
	if (SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem))
		{
		OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
		hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&TextSize);
		WorkRect.left += 8;
		WorkRect.right = WorkRect.left+SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)0,(LPARAM)0)-40;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		SelectObject(ItemStruct->hDC,hOldFont);
		SetTextColor(ItemStruct->hDC,OldColor);
		}

	/* Draws subitems */

	FarItem.iSubItem = 1;
	OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
	hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
	while (Columns--)
		{
		SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&TextSize);
		WorkRect.left = WorkRect.right;
		WorkRect.right = WorkRect.left+SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)FarItem.iSubItem,(LPARAM)0)-8;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
		WorkRect.right += 8;
		FarItem.iSubItem++;
		}
	SelectObject(ItemStruct->hDC,hOldFont);
	SetTextColor(ItemStruct->hDC,OldColor);

	return;
}


// «»»» Change la position de démarrage de la caméra ««««««««««««««««««««»

void Properties_MapPlayerSetCamera(HWND hDlg, MAPPROPSCTX *MapProps)
{
	MAPRECT	MapRect;

	MapRect.x1 = MapProps->MapCameraX;
	MapRect.y1 = MapProps->MapCameraY;
	Map_ChoosePointEx(hDlg,&MapRect,MAP_CHOICELMBPOINT,szCfgChooseCamera);
	MapProps->MapCameraX = MapRect.x1;
	MapProps->MapCameraY = MapRect.y1;
	MapProps->CurrentPlayer.PlayerCameraX = MapRect.x1;
	MapProps->CurrentPlayer.PlayerCameraY = MapRect.y1;
	return;
}


// «»»» Change l'état d'un ou plusieurs éléments ««««««««««««««««««««««««»

void Properties_MapPlayerRoomsChange(HWND hDlg, UINT CtlId, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;
	LONG	 Count;
	LONG	 State;

	if (MapProps->DoNotUpdate) return;

	CheckRadioButton(hDlg,118,122,CtlId);
	if (CtlId == 118) State = -1;
	else State = CtlId-118;

	Count = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;

	while ( (Item.iItem = SendMessage(MapProps->hRooms,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Count++;
		MapProps->CurrentPlayer.Rooms[Item.lParam-1] = (BYTE)State;
		}

	if (Count)
		{
		InvalidateRect(MapProps->hRooms,NULL,FALSE);
		UpdateWindow(MapProps->hRooms);
		}

	return;
}


// «»»» Un ou plusieurs éléments ont été sélectionnés «««««««««««««««««««»

void Properties_MapPlayerRoomsChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;
	LONG	 Count;
	LONG	 RadioCount[5];

	if (MapProps->DoNotUpdate) return;
	Count = SendMessage(MapProps->hRooms,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (!Count) goto Disable;

	//--- Analyse les créatures sélectionnées ---

	Count = 0;
	RadioCount[0] = 0;
	RadioCount[1] = 0;
	RadioCount[2] = 0;
	RadioCount[3] = 0;
	RadioCount[4] = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hRooms,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= 1;
		Count++;
		switch(MapProps->CurrentPlayer.Rooms[Item.lParam])
			{
			case 1:	RadioCount[1]++;
				break;
			case 2:	RadioCount[2]++;
				break;
			case 3:	RadioCount[3]++;
				break;
			case 4:	RadioCount[4]++;
				break;
			default:RadioCount[0]++;
				break;
			}
		}
	if (!Count) goto Disable;

	// Note: BST_INDETERMINATE does not work with radio buttons...
	//	 ...so there are all selected normally :)
	//
	MapProps->DoNotUpdate = TRUE;
	for (Count = 0; Count != 4+1; Count++)
		{
		EnableWindow(GetDlgItem(hDlg,Count+118),TRUE);
		if (RadioCount[Count] > 0) CheckDlgButton(hDlg,Count+118,BST_CHECKED);
		if (!RadioCount[Count]) CheckDlgButton(hDlg,Count+118,BST_UNCHECKED);
		}
	MapProps->DoNotUpdate = FALSE;
	return;

//--- Aucun élément n'est réellement sélectionné ---

Disable:for (Count = 118; Count != 122+1; Count++)
		EnableWindow(GetDlgItem(hDlg,Count),FALSE);
	return;
}


// «»»» Affichage du statut des éléments ««««««««««««««««««««««««««««««««»

void Properties_MapPlayerRoomsDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;

	Item.mask = LVIF_PARAM;
	Item.iItem = DispInfo->item.iItem;
	Item.iSubItem = 0;
	SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;

	// The line below assumes that tables are in following order :
	//	Rooms, Traps, Doors, Spells
	//
	switch(MapProps->CurrentPlayer.Rooms[Item.lParam-1])
		{
		case 1:	strcpy(DispInfo->item.pszText,szCfgIndisp);
			break;
		case 2: strcpy(DispInfo->item.pszText,szCfgSearch);
			break;
		case 3: strcpy(DispInfo->item.pszText,szCfgAcquis);
			break;
		case 4: strcpy(DispInfo->item.pszText,szCfgLocked);
			break;
		default:strcpy(DispInfo->item.pszText,szCfgNA);
			break;
		}
	return;
}


// «»»» Change l'état d'une ou plusieurs créatures ««««««««««««««««««««««»

void Properties_MapPlayerCreaturesChange(HWND hDlg, BOOL CheckBox, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	LONG	Count;
	LONG	NewCount;

	if (MapProps->DoNotUpdate) return;

	if (CheckBox)
		{
		Count = IsDlgButtonChecked(hDlg,127);
		if (Count == BST_UNCHECKED)
			{
			NewCount = -1;
			CheckDlgButton(hDlg,127,BST_CHECKED);
			EnableWindow(GetDlgItem(hDlg,125),FALSE);
			EnableWindow(GetDlgItem(hDlg,126),FALSE);
			EnableWindow(GetDlgItem(hDlg,136),FALSE);
			}
		else
			{
			NewCount = GetDlgItemInt(hDlg,126,NULL,FALSE);
			CheckDlgButton(hDlg,127,BST_UNCHECKED);
			EnableWindow(GetDlgItem(hDlg,125),TRUE);
			EnableWindow(GetDlgItem(hDlg,126),TRUE);
			EnableWindow(GetDlgItem(hDlg,136),TRUE);
			}
		}
	else
		NewCount = GetDlgItemInt(hDlg,126,NULL,FALSE);

	Count = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= MAP_VARROOMS+MAP_VARTRAPS+MAP_VARDOORS+MAP_VARSPELLS+1;
		if ((!CheckBox)&&(MapProps->CurrentPlayer.CreaturesCount[Item.lParam] == -1)) continue;
		Count++;
		MapProps->CurrentPlayer.CreaturesCount[Item.lParam] = NewCount;
		}

	if (Count)
		{
		InvalidateRect(MapProps->hCreatures,NULL,FALSE);
		UpdateWindow(MapProps->hCreatures);
		}

	return;
}


// «»»» Une ou plusieurs créatures ont été sélectionnées ««««««««««««««««»

void Properties_MapPlayerCreaturesChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	LONG	Count;
	LONG	Number;
	LONG	NumberTotal;
	LONG	UndeterminedTotal;

	if (MapProps->DoNotUpdate) return;
	Count = SendMessage(MapProps->hCreatures,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (!Count) goto Disable;

	//--- Analyse les créatures sélectionnées ---

	Count = 0;
	NumberTotal = 0;
	UndeterminedTotal = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= MAP_VARROOMS+MAP_VARTRAPS+MAP_VARDOORS+MAP_VARSPELLS+1;
		Count++;
		Number = MapProps->CurrentPlayer.CreaturesCount[Item.lParam];
		if (Number == -1) UndeterminedTotal++;
		else NumberTotal += Number;
		}
	if (!Count) goto Disable;

	//--- Modifie l'état des boutons ---

	EnableWindow(GetDlgItem(hDlg,127),TRUE);
	MapProps->DoNotUpdate = TRUE;

	if (UndeterminedTotal == Count)
		{
		EnableWindow(GetDlgItem(hDlg,125),FALSE);
		EnableWindow(GetDlgItem(hDlg,126),FALSE);
		EnableWindow(GetDlgItem(hDlg,136),FALSE);
		CheckDlgButton(hDlg,127,BST_CHECKED);
		}
	else
		{
		EnableWindow(GetDlgItem(hDlg,125),TRUE);
		EnableWindow(GetDlgItem(hDlg,126),TRUE);
		EnableWindow(GetDlgItem(hDlg,136),TRUE);
		if (UndeterminedTotal) CheckDlgButton(hDlg,127,BST_INDETERMINATE);
		else CheckDlgButton(hDlg,127,BST_UNCHECKED);
		}

	if (Count-UndeterminedTotal == 0) NumberTotal = 0;
	else NumberTotal /= Count-UndeterminedTotal;
	SetDlgItemInt(hDlg,126,NumberTotal,FALSE);

	MapProps->DoNotUpdate = FALSE;
	return;

//--- Aucune créature n'est réellement sélectionnée ---

Disable:EnableWindow(GetDlgItem(hDlg,125),FALSE);
	EnableWindow(GetDlgItem(hDlg,126),FALSE);
	EnableWindow(GetDlgItem(hDlg,127),FALSE);
	EnableWindow(GetDlgItem(hDlg,136),FALSE);
	return;
}


// «»»» Affichage du nombre de créatures ««««««««««««««««««««««««««««««««»

void Properties_MapPlayerCreaturesDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LONG	Count;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;
	if (!(DispInfo->item.lParam)) return;

	// The line below assumes that tables are in following order :
	//	Bad creatures, Elites, Heros
	//
	Count = MapProps->CurrentPlayer.CreaturesCount[DispInfo->item.lParam-1-MAP_VARROOMS-MAP_VARTRAPS-MAP_VARDOORS-MAP_VARSPELLS];

	if (Count == -1)
		strcpy(DispInfo->item.pszText,szCfgNA);
	else
		wsprintf(DispInfo->item.pszText,szNumber,Count);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
