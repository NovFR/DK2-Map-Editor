
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres des créatures
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
// ¤¤¤ Paramètres des créatures						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapCreatures(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6803),hWnd,Properties_MapCreaturesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapCreaturesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapCreaturesInit(hDlg,(MAPPROPSCTX *)lParam);
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
			Properties_MapPlayerDrawItem((DRAWITEMSTRUCT *)lParam,MapProps,4);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR FAR *) lParam)->code)
				{
				case LVN_GETDISPINFO:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapCreaturesDispInfo((LV_DISPINFO *)lParam,MapProps);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapCreaturesChanged(hDlg,MapProps);
					return(TRUE);
				case UDN_DELTAPOS:
					if ((((NMHDR *)lParam)->idFrom < 113)&&(((NMHDR *)lParam)->idFrom > 119)) break;
					Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-10,(NM_UPDOWN *)lParam);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					if ((LOWORD(wParam) < 103)&&(LOWORD(wParam) > 109)) break;
					lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
					if (lData < 0)
						{
						SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
						break;
						}
					if (lData > 65535)
						{
						SetDlgItemInt(hDlg,LOWORD(wParam),65535,FALSE);
						break;
						}
					Properties_MapCreaturesSetParam(hDlg,LOWORD(wParam),MapProps);
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							MapProps->DoNotUpdate = FALSE;
							CopyMemory(&MapProps->MapProperties.CreaturesParams[0][0],&MapProps->CreaturesParams[0][0],MAP_VARCREATURES*64*sizeof(DWORD));
							SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							Misc_DestroyImageList(&MapProps->hImages);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							MapProps->DoNotUpdate = FALSE;
							SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
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

void Properties_MapCreaturesInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	LONG		X,Y;
	int		i;

	MapProps->DoNotUpdate = TRUE;
	CopyMemory(&MapProps->CreaturesParams[0][0],&MapProps->MapProperties.CreaturesParams[0][0],MAP_VARCREATURES*64*sizeof(DWORD));
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	if (!Misc_CreateImageList(BadCreatures,&MapProps->hImages,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(BadElites,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Heros,&MapProps->hImages,32,32,TRUE)) goto Error_0;

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hCreatures = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hCreatures) goto Error_1;
	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szCfgColumn4;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_2;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*6;
	Column.pszText = szCfgColumn5;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn6;
	Column.iSubItem = 2;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn7;
	Column.iSubItem = 3;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)3,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn8;
	Column.iSubItem = 4;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)4,(LPARAM)&Column) == -1) goto Error_2;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szBadCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (X = 1, Y = 1, i = 0; BadCreatures[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadCreatures[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szBadCreaturesElite;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; BadElites[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadElites[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szGoodCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Heros[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Heros[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}

	Misc_CreateUpDownButton(hDlg,103,113);
	Misc_CreateUpDownButton(hDlg,105,115);
	Misc_CreateUpDownButton(hDlg,107,117);
	Misc_CreateUpDownButton(hDlg,109,119);
	SetDlgItemInt(hDlg,103,0,FALSE);
	SetDlgItemInt(hDlg,105,0,FALSE);
	SetDlgItemInt(hDlg,107,0,FALSE);
	SetDlgItemInt(hDlg,109,0,FALSE);
	SendDlgItemMessage(hDlg,103,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
	SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
	SendDlgItemMessage(hDlg,107,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
	SendDlgItemMessage(hDlg,109,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
	MapProps->DoNotUpdate = FALSE;
	return;

//--- Erreurs ---

Error_2:SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_1:Misc_DestroyImageList(&MapProps->hImages);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Mise à jour de paramètres «««««««««««««««««««««««««««««««««««««««»

void Properties_MapCreaturesChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	long	Params[4];
	long	Total[4];

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;

	Total[0] = 0;
	Total[1] = 0;
	Total[2] = 0;
	Total[3] = 0;
	Params[0] = 0;
	Params[1] = 0;
	Params[2] = 0;
	Params[3] = 0;

	while( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1 )
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		if ((Item.lParam >= 17)&&(Item.lParam <= 29)) continue;

		if (MapProps->CreaturesParams[Item.lParam-1][0])
			{
			Params[0] += MapProps->CreaturesParams[Item.lParam-1][0];
			Total[0]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][1])
			{
			Params[1] += MapProps->CreaturesParams[Item.lParam-1][1];
			Total[1]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][2])
			{
			Params[2] += MapProps->CreaturesParams[Item.lParam-1][2];
			Total[2]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][3])
			{
			Params[3] += MapProps->CreaturesParams[Item.lParam-1][3];
			Total[3]++;
			}
		}

	MapProps->DoNotUpdate = TRUE;
	if (Total[0]) SetDlgItemInt(hDlg,103,Params[0]/Total[0],FALSE);
	else SetDlgItemInt(hDlg,103,0,FALSE);
	if (Total[1]) SetDlgItemInt(hDlg,105,Params[1]/Total[1],FALSE);
	else SetDlgItemInt(hDlg,105,0,FALSE);
	if (Total[2]) SetDlgItemInt(hDlg,107,Params[2]/Total[2],FALSE);
	else SetDlgItemInt(hDlg,107,0,FALSE);
	if (Total[3]) SetDlgItemInt(hDlg,109,Params[3]/Total[3],FALSE);
	else SetDlgItemInt(hDlg,109,0,FALSE);
	MapProps->DoNotUpdate = FALSE;
	return;
}


// «»»» Changement de paramètres ««««««««««««««««««««««««««««««««««««««««»

void Properties_MapCreaturesSetParam(HWND hDlg, UINT CtlID, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	DWORD	NewParam;

	if (MapProps->DoNotUpdate) return;

	NewParam = GetDlgItemInt(hDlg,CtlID,NULL,FALSE);
	CtlID -= 103;
	CtlID /= 2;	

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;

	while( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1 )
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		// Les paramètres des créatures elites (Offset 17 à 29)
		// ne peuvent pas être modifiés.
		//
		if ((Item.lParam >= 17)&&(Item.lParam <= 29)) continue;

		MapProps->CreaturesParams[Item.lParam-1][CtlID] = NewParam;
		}

	InvalidateRect(MapProps->hCreatures,NULL,FALSE);
	UpdateWindow(MapProps->hCreatures);
	return;
}


// «»»» Affichage des informations concernant les créatures «««««««««««««»

void Properties_MapCreaturesDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LONG	Count;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;
	if (!(DispInfo->item.lParam)) return;

	// The line below assumes that tables are in following order :
	//	Bad creatures, Elites, Heros
	//
	switch(DispInfo->item.iSubItem)
		{
		case 1:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][0];
			break;
		case 2:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][1];
			break;
		case 3:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][2];
			break;
		case 4:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][3];
			break;
		default:return;
		}

	if (Count)
		wsprintf(DispInfo->item.pszText,szNumber,Count);
	else
		{
		if ((DispInfo->item.lParam >= 17)&&(DispInfo->item.lParam <= 29))
			strcpy(DispInfo->item.pszText,szCfgNull);
		else
			strcpy(DispInfo->item.pszText,szCfgNA);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
