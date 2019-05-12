
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres de l'ordinateur
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
extern NODE		MapAreas;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des gardiens informatique				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapComputerOptions(HWND hDlg, MAPPROPSCTX *MapProps)
{
	static DLGPROC	 pfn[]={Properties_MapComputerProc0,
				Properties_MapComputerProc1,
				Properties_MapComputerProc2,
				Properties_MapComputerProc3,
				Properties_MapComputerProc4,
				Properties_MapComputerProc5,
				Properties_MapComputerProc6,
				Properties_MapComputerProc7,
				Properties_MapComputerProc8};
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	int		 Result;

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*9);
	if (!psp) goto Error_1;

	for (Result = 0; Result != 9; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(6810+Result);
		psp[Result].pfnDlgProc = pfn[Result];
		psp[Result].lParam = (LPARAM)MapProps;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID;
	psh->hwndParent = hDlg;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = szCfgCPU;
	psh->nPages = 9;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	CopyMemory(&MapProps->CurrentPlayerCPUSettings,&MapProps->CurrentPlayer,sizeof(MAPOPTIONS));
	for (Result = 0; Result != 9; Result++) MapProps->PageInitialized[Result] = FALSE;
	MapProps->UpIcon = LoadImage(hInstance,MAKEINTRESOURCE(50),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	MapProps->DownIcon = LoadImage(hInstance,MAKEINTRESOURCE(51),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!MapProps->UpIcon || !MapProps->DownIcon) goto Error_3;
	if (!Misc_CreateImageList(CPURooms,&MapProps->hCPUImages,32,32,FALSE)) goto Error_3;

	Result = PropertySheet(psh);
	if (Result == -1) goto Error_2;

	DestroyIcon(MapProps->DownIcon);
	DestroyIcon(MapProps->UpIcon);
	GlobalFree(psp);
	GlobalFree(psh);
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_3:Misc_DestroyImageList(&MapProps->hCPUImages);
	if (MapProps->DownIcon) DestroyIcon(MapProps->DownIcon);
	if (MapProps->UpIcon) DestroyIcon(MapProps->UpIcon);
Error_2:GlobalFree(psp);
Error_1:GlobalFree(psh);
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus des boîtes de dialogue					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Prédéfinitions ««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc0(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,0,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

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
				case LBN_SELCHANGE:
					MapProps->CurrentPlayerCPUSettings.PlayerAI = SendMessage(MapProps->hDefinitions,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					return(TRUE);

				case BN_CLICKED:
					if (LOWORD(wParam) == 101) Properties_MapOptionsKeeperAI(MapProps->CurrentPlayerCPUSettings.PlayerAI,&MapProps->CurrentPlayerCPUSettings);
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_MapComputerSetValues(hDlg,0,MapProps))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Ordre de construction «««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc1(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	MAPRECTEX	*Area;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,1,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

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
				Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1,0);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				switch(((DRAWITEMSTRUCT *)lParam)->CtlID)
					{
					case 201:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,MapProps->UpIcon);
						break;
					case 202:
						Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,MapProps->DownIcon);
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
						case 211:
							lData = GetDlgItemInt(hDlg,211,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,211,0,FALSE);
								}
							if (lData > MapProps->MapProperties.Width-1)
								{
								lData = MapProps->MapProperties.Width-1;
								SetDlgItemInt(hDlg,211,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaX1 = lData;
							break;
						case 212:
							lData = GetDlgItemInt(hDlg,212,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,212,0,FALSE);
								}
							if (lData > MapProps->MapProperties.Height-1)
								{
								lData = MapProps->MapProperties.Height-1;
								SetDlgItemInt(hDlg,212,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaY1 = lData;
							break;
						case 213:
							lData = GetDlgItemInt(hDlg,213,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,213,0,FALSE);
								}
							if (lData > MapProps->MapProperties.Width-1)
								{
								lData = MapProps->MapProperties.Width-1;
								SetDlgItemInt(hDlg,213,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaX2 = lData;
							break;
						case 214:
							lData = GetDlgItemInt(hDlg,214,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,214,0,FALSE);
								}
							if (lData > MapProps->MapProperties.Height-1)
								{
								lData = MapProps->MapProperties.Height-1;
								SetDlgItemInt(hDlg,214,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaY2 = lData;
							break;
						}
					return(TRUE);

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 221:
							lData = Misc_GetArea(hDlg,221,TRUE);
							if (!lData) break;
							Area = RectEx_FindById(lData,&MapAreas);
							if (!Area) break;
							SetDlgItemInt(hDlg,211,Area->rect.x1,FALSE);
							SetDlgItemInt(hDlg,212,Area->rect.y1,FALSE);
							SetDlgItemInt(hDlg,213,Area->rect.x2,FALSE);
							SetDlgItemInt(hDlg,214,Area->rect.y2,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 201:
							Properties_MapComputerChangeOrder(hDlg,FALSE,MapProps);
							break;
						case 202:
							Properties_MapComputerChangeOrder(hDlg,TRUE,MapProps);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 281:
							Misc_SetUpDownInt(hDlg,211,(NM_UPDOWN *)lParam);
							break;
						case 282:
							Misc_SetUpDownInt(hDlg,212,(NM_UPDOWN *)lParam);
							break;
						case 283:
							Misc_SetUpDownInt(hDlg,213,(NM_UPDOWN *)lParam);
							break;
						case 284:
							Misc_SetUpDownInt(hDlg,214,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case LVN_ITEMCHANGED:
					Properties_MapComputerToggleArrows(hDlg,MapProps);
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_MapComputerSetValues(hDlg,1,MapProps))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Construction des salles «««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc2(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,2,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 303:
						case 306:
						case 322:
						case 325:
						case 332:
						case 335:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								break;
								}
							if (lData > 255)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),255,FALSE);
								break;
								}
							break;
						case 345:
							lData = GetDlgItemInt(hDlg,345,NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,345,0,FALSE);
								break;
								}
							if (lData > 100)
								{
								SetDlgItemInt(hDlg,345,100,FALSE);
								break;
								}
							break;
						}

					switch(LOWORD(wParam))
						{
						case 303:
							MapProps->CurrentPlayerCPUSettings.PlayerResearchedRoom = (BYTE)GetDlgItemInt(hDlg,303,NULL,FALSE);
							break;
						case 306:
							MapProps->CurrentPlayerCPUSettings.PlayerRebuildRoom = (BYTE)GetDlgItemInt(hDlg,306,NULL,FALSE);
							break;
						case 322:
							MapProps->CurrentPlayerCPUSettings.PlayerDefaultTiles = (BYTE)GetDlgItemInt(hDlg,322,NULL,FALSE);
							break;
						case 325:
							MapProps->CurrentPlayerCPUSettings.PlayerTilesBetweenRooms = (BYTE)GetDlgItemInt(hDlg,325,NULL,FALSE);
							break;
						case 332:
							MapProps->CurrentPlayerCPUSettings.PlayerTilesFromHeart = (BYTE)GetDlgItemInt(hDlg,332,NULL,FALSE);
							break;
						case 335:
							MapProps->CurrentPlayerCPUSettings.PlayerTilesFromClaimed = (BYTE)GetDlgItemInt(hDlg,335,NULL,FALSE);
							break;
						case 345:
							MapProps->CurrentPlayerCPUSettings.PlayerFlexibility = (BYTE)GetDlgItemInt(hDlg,345,NULL,FALSE);
							break;
						}
					return(TRUE);

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 309:
							MapProps->CurrentPlayerCPUSettings.PlayerExpandMode = (BYTE)SendDlgItemMessage(hDlg,309,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						case 328:
							MapProps->CurrentPlayerCPUSettings.PlayerDistance = (BYTE)SendDlgItemMessage(hDlg,328,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						case 330:
							MapProps->CurrentPlayerCPUSettings.PlayerCorridorStyle = (BYTE)SendDlgItemMessage(hDlg,330,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 341:
							MapProps->CurrentPlayerCPUSettings.PlayerCreateEmptyAreas = (IsDlgButtonChecked(hDlg,341) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 342:
							MapProps->CurrentPlayerCPUSettings.PlayerBuildBiggerLair = (IsDlgButtonChecked(hDlg,342) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 343:
							MapProps->CurrentPlayerCPUSettings.PlayerSellCapturedRoom = (IsDlgButtonChecked(hDlg,343) == BST_CHECKED)?TRUE:FALSE;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{

				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 381:
							Misc_SetUpDownInt(hDlg,303,(NM_UPDOWN *)lParam);
							break;
						case 382:
							Misc_SetUpDownInt(hDlg,306,(NM_UPDOWN *)lParam);
							break;
						case 383:
							Misc_SetUpDownInt(hDlg,322,(NM_UPDOWN *)lParam);
							break;
						case 384:
							Misc_SetUpDownInt(hDlg,325,(NM_UPDOWN *)lParam);
							break;
						case 385:
							Misc_SetUpDownInt(hDlg,332,(NM_UPDOWN *)lParam);
							break;
						case 386:
							Misc_SetUpDownInt(hDlg,335,(NM_UPDOWN *)lParam);
							break;
						case 387:
							Misc_SetUpDownInt(hDlg,345,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,2,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Pièges et portes ««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc3(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,3,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
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
					if ((HWND)lParam == GetDlgItem(hDlg,402))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerTrapUseStyle = (BYTE)SendDlgItemMessage(hDlg,402,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,406))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerPreference = (BYTE)SendDlgItemMessage(hDlg,406,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,442))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerUseTrapsnDoors = (BYTE)SendDlgItemMessage(hDlg,442,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,445,MapProps->CurrentPlayerCPUSettings.PlayerUseTrapsnDoors,FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 421:
							if (IsDlgButtonChecked(hDlg,421) == BST_CHECKED) MapProps->CurrentPlayerCPUSettings.PlayerDoorUsage = 0;
							break;
						case 422:
							if (IsDlgButtonChecked(hDlg,422) == BST_CHECKED) MapProps->CurrentPlayerCPUSettings.PlayerDoorUsage = 1;
							break;
						case 423:
							if (IsDlgButtonChecked(hDlg,423) == BST_CHECKED) MapProps->CurrentPlayerCPUSettings.PlayerDoorUsage = 2;
							break;
						case 431:
							MapProps->CurrentPlayerCPUSettings.PlayerBoulderInCorridors = (IsDlgButtonChecked(hDlg,431) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 432:
							MapProps->CurrentPlayerCPUSettings.PlayerBoulderInAngles = (IsDlgButtonChecked(hDlg,432) == BST_CHECKED)?TRUE:FALSE;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_MapComputerSetValues(hDlg,3,MapProps))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Sorts «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc4(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,4,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
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
					if ((HWND)lParam == GetDlgItem(hDlg,502))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerSpellsInBattle = (BYTE)SendDlgItemMessage(hDlg,502,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,506))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerSpellsPower = (BYTE)SendDlgItemMessage(hDlg,506,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 522:
							MapProps->CurrentPlayerCPUSettings.PlayerSpellsSightOfEvil = (BYTE)SendDlgItemMessage(hDlg,522,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						case 524:
							MapProps->CurrentPlayerCPUSettings.PlayerSpellsCallToArms = (BYTE)SendDlgItemMessage(hDlg,524,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 525:
							MapProps->CurrentPlayerCPUSettings.PlayerSpellsLightning = (IsDlgButtonChecked(hDlg,525) == BST_CHECKED)?TRUE:FALSE;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,4,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Créatures «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc5(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,5,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

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
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 622:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesSackMethod = (BYTE)SendDlgItemMessage(hDlg,622,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						}
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 624:
							lData = GetDlgItemInt(hDlg,624,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,624,0,FALSE);
								}
							if (lData > 255)
								{
								lData = 255;
								SetDlgItemInt(hDlg,624,255,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesImps = (BYTE)lData;
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 601:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesHappyUnhappy = (IsDlgButtonChecked(hDlg,601) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 602:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesHappyUngry = (IsDlgButtonChecked(hDlg,602) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 603:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesDisposeUngry = (IsDlgButtonChecked(hDlg,603) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 604:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesDisposeRubish = (IsDlgButtonChecked(hDlg,604) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 625:
							MapProps->CurrentPlayerCPUSettings.PlayerCreaturesSlap = (BYTE)((IsDlgButtonChecked(hDlg,625) == BST_CHECKED)?0x20:0x00);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 681:
							Misc_SetUpDownInt(hDlg,624,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,5,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Première attaque ««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc6(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,6,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

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
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 722:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackBreach = (BYTE)SendDlgItemMessage(hDlg,722,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						case 724:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackDig = (BYTE)SendDlgItemMessage(hDlg,724,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						}
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 706:
						case 710:
						case 726:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								break;
								}
							if (lData > 255)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),255,FALSE);
								break;
								}
							break;
						}

					switch(LOWORD(wParam))
						{
						case 703:
							lData = GetDlgItemInt(hDlg,703,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,703,0,FALSE);
								}
							if (lData > 10)
								{
								lData = 10;
								SetDlgItemInt(hDlg,703,10,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackLevel = (BYTE)lData;
							break;
						case 706:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackCount = (BYTE)GetDlgItemInt(hDlg,706,NULL,FALSE);
							break;
						case 710:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackThreat = (BYTE)GetDlgItemInt(hDlg,710,NULL,FALSE);
							break;
						case 726:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackPoints = (BYTE)GetDlgItemInt(hDlg,726,NULL,FALSE);
							break;
						case 729:
							lData = GetDlgItemInt(hDlg,729,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,729,0,FALSE);
								}
							if (lData > 100)
								{
								lData = 100;
								SetDlgItemInt(hDlg,729,100,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackCreatures = (BYTE)lData;
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 701:
							MapProps->CurrentPlayerCPUSettings.PlayerUseLevelInFirstAttack = (IsDlgButtonChecked(hDlg,701) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 708:
							MapProps->CurrentPlayerCPUSettings.PlayerUseThreatInFirstAttack = (IsDlgButtonChecked(hDlg,708) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 712:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackAllRooms = (IsDlgButtonChecked(hDlg,712) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 714:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackAllSpells = (IsDlgButtonChecked(hDlg,714) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 741:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackOnlyAttackers = (IsDlgButtonChecked(hDlg,741) == BST_CHECKED)?TRUE:FALSE;
							break;
						case 742:
							MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackDontAttack = (IsDlgButtonChecked(hDlg,742) == BST_CHECKED)?TRUE:FALSE;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 781:
							Misc_SetUpDownInt(hDlg,703,(NM_UPDOWN *)lParam);
							break;
						case 782:
							Misc_SetUpDownInt(hDlg,706,(NM_UPDOWN *)lParam);
							break;
						case 783:
							Misc_SetUpDownInt(hDlg,710,(NM_UPDOWN *)lParam);
							break;
						case 784:
							Misc_SetUpDownInt(hDlg,726,(NM_UPDOWN *)lParam);
							break;
						case 785:
							Misc_SetUpDownInt(hDlg,729,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,6,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Combats «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc7(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,7,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
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
					if ((HWND)lParam == GetDlgItem(hDlg,805))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerBattleSpellType = (BYTE)SendDlgItemMessage(hDlg,805,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
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
						case 802:
						case 847:
						case 850:
						case 852:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								break;
								}
							if (lData > 255)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),255,FALSE);
								break;
								}
							break;
						case 809:
						case 824:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								break;
								}
							if (lData > 100)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),100,FALSE);
								break;
								}
							break;
						case 842:
						case 844:
						case 862:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								break;
								}
							if (lData > 32767)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),32767,FALSE);
								break;
								}
							break;
						}

					switch(LOWORD(wParam))
						{
						case 802:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleDropped = (BYTE)GetDlgItemInt(hDlg,802,NULL,FALSE);
							break;
						case 809:
							MapProps->CurrentPlayerCPUSettings.PlayerBattlePrison = (BYTE)GetDlgItemInt(hDlg,809,NULL,FALSE);
							break;
						case 824:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleHealth = (BYTE)GetDlgItemInt(hDlg,824,NULL,FALSE);
							break;
						case 842:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsPlace = GetDlgItemInt(hDlg,842,NULL,FALSE);
							break;
						case 844:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemove = GetDlgItemInt(hDlg,844,NULL,FALSE);
							break;
						case 847:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoCreature = GetDlgItemInt(hDlg,847,NULL,FALSE);
							break;
						case 850:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoDanger1 = GetDlgItemInt(hDlg,850,NULL,FALSE);
							break;
						case 852:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoDanger2 = GetDlgItemInt(hDlg,852,NULL,FALSE);
							break;
						case 862:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleWait = GetDlgItemInt(hDlg,862,NULL,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 821:
							MapProps->CurrentPlayerCPUSettings.PlayerBattleRemove = (BYTE)((IsDlgButtonChecked(hDlg,821) == BST_CHECKED)?TRUE:FALSE);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 881:
							Misc_SetUpDownInt(hDlg,802,(NM_UPDOWN *)lParam);
							break;
						case 882:
							Misc_SetUpDownInt(hDlg,809,(NM_UPDOWN *)lParam);
							break;
						case 883:
							Misc_SetUpDownInt(hDlg,824,(NM_UPDOWN *)lParam);
							break;
						case 884:
							Misc_SetUpDownInt(hDlg,842,(NM_UPDOWN *)lParam);
							break;
						case 885:
							Misc_SetUpDownInt(hDlg,844,(NM_UPDOWN *)lParam);
							break;
						case 886:
							Misc_SetUpDownInt(hDlg,847,(NM_UPDOWN *)lParam);
							break;
						case 887:
							Misc_SetUpDownInt(hDlg,850,(NM_UPDOWN *)lParam);
							break;
						case 888:
							Misc_SetUpDownInt(hDlg,852,(NM_UPDOWN *)lParam);
							break;
						case 889:
							Misc_SetUpDownInt(hDlg,862,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,7,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Divers ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc8(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapComputerInit(hDlg,8,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
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
					if ((HWND)lParam == GetDlgItem(hDlg,927))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsExplore = (BYTE)SendDlgItemMessage(hDlg,927,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,928,MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsExplore,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,931))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsChances = (BYTE)SendDlgItemMessage(hDlg,931,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,932,MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsChances,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,946))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerSpeed = (BYTE)SendDlgItemMessage(hDlg,946,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,947,MapProps->CurrentPlayerCPUSettings.PlayerSpeed,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,949))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerManaValue = (BYTE)SendDlgItemMessage(hDlg,949,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,950,MapProps->CurrentPlayerCPUSettings.PlayerManaValue,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,953))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerGoldValue = (BYTE)SendDlgItemMessage(hDlg,953,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,954,MapProps->CurrentPlayerCPUSettings.PlayerGoldValue,FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,958))
						{
						MapProps->CurrentPlayerCPUSettings.PlayerOpenness = (BYTE)SendDlgItemMessage(hDlg,958,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,959,MapProps->CurrentPlayerCPUSettings.PlayerOpenness,FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 902:
							MapProps->CurrentPlayerCPUSettings.PlayerLibraryUsage = (BYTE)SendDlgItemMessage(hDlg,902,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						case 904:
							MapProps->CurrentPlayerCPUSettings.PlayerJailedFate = (BYTE)SendDlgItemMessage(hDlg,904,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							break;
						}
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 922:
							lData = GetDlgItemInt(hDlg,922,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,922,0,FALSE);
								}
							if (lData > 65535)
								{
								lData = 65535;
								SetDlgItemInt(hDlg,922,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerMineGoldAmount = lData;
							break;
						case 924:
							lData = GetDlgItemInt(hDlg,924,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,924,0,FALSE);
								}
							if (lData > 32767)
								{
								lData = 32767;
								SetDlgItemInt(hDlg,924,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsTiles = lData;
							break;
						case 942:
							lData = GetDlgItemInt(hDlg,942,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,942,0,FALSE);
								}
							if (lData > 255)
								{
								lData = 255;
								SetDlgItemInt(hDlg,942,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerImpsToTilesRatio = (BYTE)lData;
							break;
						case 944:
							lData = GetDlgItemInt(hDlg,944,NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,944,0,FALSE);
								}
							if (lData > 64000)
								{
								lData = 64000;
								SetDlgItemInt(hDlg,944,lData,FALSE);
								}
							MapProps->CurrentPlayerCPUSettings.PlayerMana = lData;
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 981:
							Misc_SetUpDownInt(hDlg,922,(NM_UPDOWN *)lParam);
							break;
						case 982:
							Misc_SetUpDownInt(hDlg,924,(NM_UPDOWN *)lParam);
							break;
						case 983:
							Misc_SetUpDownInt(hDlg,942,(NM_UPDOWN *)lParam);
							break;
						case 984:
							Misc_SetUpDownInt(hDlg,944,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_APPLY:
					CopyMemory(&MapProps->CurrentPlayer,&MapProps->CurrentPlayerCPUSettings,sizeof(MAPOPTIONS));
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					Properties_MapComputerSetValues(hDlg,8,MapProps);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines communes						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisations de la boîte de dialogue «««««««««««««««««««««««««»

void Properties_MapComputerInit(HWND hDlg, UINT Page, PROPSHEETPAGE *PageStruct)
{
	static char	*AI[]		= {szCfgCPUAI0,szCfgCPUAI1,szCfgCPUAI2,szCfgCPUAI3,szCfgCPUAI4,szCfgCPUAI5,szCfgCPUAI6,szCfgCPUAI7,szCfgCPUAI8,NULL};
	static char	*Expand[]	= {szCfgCPUExpand0,szCfgCPUExpand1,szCfgCPUExpand2,NULL};
	static char	*Angle[]	= {szCfgCPUAngle0,szCfgCPUAngle1,szCfgCPUAngle2,NULL};
	static char	*Distance[]	= {szCfgCPUDistance0,szCfgCPUDistance1,NULL};
	static char	*SightOfEvil[]	= {szCfgCPUSightOfEvil0,szCfgCPUSightOfEvil1,szCfgCPUSightOfEvil2,NULL};
	static char	*CallToArms[]	= {szCfgCPUCallToArms0,szCfgCPUCallToArms1,szCfgCPUCallToArms2,NULL};
	static char	*Sack[]		= {szCfgCPUSack0,szCfgCPUSack1,NULL};
	static char	*Breach[]	= {szCfgCPUBreach0,szCfgCPUBreach1,szCfgCPUBreach2,szCfgCPUBreach3,NULL};
	static char	*Dig[]		= {szCfgCPUDig0,szCfgCPUDig1,szCfgCPUDig2,szCfgCPUDig3,NULL};
	static char	*Library[]	= {szCfgCPULibrary0,szCfgCPULibrary1,szCfgCPULibrary2,NULL};
	static char	*Jailed[]	= {szCfgCPUJailed0,szCfgCPUJailed1,szCfgCPUJailed2,szCfgCPUJailed3,NULL};
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	RECT		 ButtonRect;
	MAPPROPSCTX	*Context;
	int		 i;

	Context = (MAPPROPSCTX *)(PageStruct->lParam);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Page)
		{
		//
		// Prédéfinitions
		//
		case 0:
			GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
			GetWindowRect(GetDlgItem(hDlg,101),&ButtonRect);
			MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
			MapWindowPoints(NULL,hDlg,(POINT *)&ButtonRect,2);
			Context->hDefinitions = CreateWindowEx(WS_EX_CLIENTEDGE,szListBoxClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ButtonRect.top-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
			if (!Context->hDefinitions) goto Error_0;

			for (i = 0; AI[i] != NULL; i++)
				if (SendMessage(Context->hDefinitions,LB_ADDSTRING,(WPARAM)0,(LPARAM)AI[i]) < 0) goto Error_0;

			Context->PageInitialized[0] = TRUE;
			break;

		//
		// Options générales de construction
		//
		case 1:
			GetWindowRect(GetDlgItem(hDlg,200),&ClientRect);
			GetWindowRect(GetDlgItem(hDlg,201),&ButtonRect);
			MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
			MapWindowPoints(NULL,hDlg,(POINT *)&ButtonRect,2);
			Context->hRoomsOrder = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOCOLUMNHEADER|LVS_SINGLESEL|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ButtonRect.top-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
			if (!Context->hRoomsOrder) goto Error_0;

			Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
			Column.iSubItem = 0;
			if (SendMessage(Context->hRoomsOrder,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;
			SendMessage(Context->hRoomsOrder,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->hCPUImages);

			Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			Item.iSubItem = 0;
			for (Item.iItem = 0; CPURooms[Item.iItem].Id != 0; Item.iItem++)
				{
				Item.pszText = CPURooms[Item.iItem].Name;
				Item.iImage = Item.iItem;
				Item.lParam = CPURooms[Item.iItem].Id;
				if (SendMessage(Context->hRoomsOrder,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
				}

			if (!Misc_CreateAreaComboBoxList(hDlg,221,TRUE)) goto Error_0;

			if (!Misc_CreateUpDownButton(hDlg,211,281)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,212,282)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,213,283)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,214,284)) goto Error_0;
			Context->PageInitialized[1] = TRUE;
			break;

		//
		// Options de construction des salles
		//
		case 2:
			for (i = 0; Expand[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,309,CB_ADDSTRING,(WPARAM)0,(LPARAM)Expand[i]) < 0) goto Error_0;
			for (i = 0; Distance[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,328,CB_ADDSTRING,(WPARAM)0,(LPARAM)Distance[i]) < 0) goto Error_0;
			for (i = 0; Angle[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,330,CB_ADDSTRING,(WPARAM)0,(LPARAM)Angle[i]) < 0) goto Error_0;

			if (!Misc_CreateUpDownButton(hDlg,303,381)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,306,382)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,322,383)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,325,384)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,332,385)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,335,386)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,345,387)) goto Error_0;
			Context->PageInitialized[2] = TRUE;
			break;

		//
		// Portes et pièges
		//
		case 3:
			SendDlgItemMessage(hDlg,402,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,402,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));
			SendDlgItemMessage(hDlg,406,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,406,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));
			SendDlgItemMessage(hDlg,442,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,442,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));

			Context->PageInitialized[3] = TRUE;
			break;

		//
		// Sorts
		//
		case 4:
			SendDlgItemMessage(hDlg,502,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,502,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));
			SendDlgItemMessage(hDlg,506,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,506,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));

			for (i = 0; SightOfEvil[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,522,CB_ADDSTRING,(WPARAM)0,(LPARAM)SightOfEvil[i]) < 0) goto Error_0;
			for (i = 0; CallToArms[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,524,CB_ADDSTRING,(WPARAM)0,(LPARAM)CallToArms[i]) < 0) goto Error_0;

			Context->PageInitialized[4] = TRUE;
			break;

		//
		// Créatures
		//
		case 5:
			for (i = 0; Sack[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,622,CB_ADDSTRING,(WPARAM)0,(LPARAM)Sack[i]) < 0) goto Error_0;

			if (!Misc_CreateUpDownButton(hDlg,624,681)) goto Error_0;
			Context->PageInitialized[5] = TRUE;
			break;

		//
		// Attaque initiale
		//
		case 6:
			for (i = 0; Breach[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,722,CB_ADDSTRING,(WPARAM)0,(LPARAM)Breach[i]) < 0) goto Error_0;
			for (i = 0; Dig[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,724,CB_ADDSTRING,(WPARAM)0,(LPARAM)Dig[i]) < 0) goto Error_0;

			if (!Misc_CreateUpDownButton(hDlg,703,781)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,706,782)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,710,783)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,726,784)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,729,785)) goto Error_0;
			Context->PageInitialized[6] = TRUE;
			break;

		//
		// Combats
		//
		case 7:
			SendDlgItemMessage(hDlg,805,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,805,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,4));

			if (!Misc_CreateUpDownButton(hDlg,802,881)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,809,882)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,824,883)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,842,884)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,844,885)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,847,886)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,850,887)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,852,888)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,862,889)) goto Error_0;
			Context->PageInitialized[7] = TRUE;
			break;

		//
		// Divers
		//
		case 8:
			for (i = 0; Library[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,902,CB_ADDSTRING,(WPARAM)0,(LPARAM)Library[i]) < 0) goto Error_0;
			for (i = 0; Jailed[i] != NULL; i++)
				if (SendDlgItemMessage(hDlg,904,CB_ADDSTRING,(WPARAM)0,(LPARAM)Jailed[i]) < 0) goto Error_0;

			SendDlgItemMessage(hDlg,927,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,927,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));
			SendDlgItemMessage(hDlg,931,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,931,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));
			SendDlgItemMessage(hDlg,946,TBM_SETTICFREQ,(WPARAM)1,(LPARAM)0);
			SendDlgItemMessage(hDlg,946,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(1,4));
			SendDlgItemMessage(hDlg,949,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,949,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));
			SendDlgItemMessage(hDlg,953,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,953,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));
			SendDlgItemMessage(hDlg,958,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
			SendDlgItemMessage(hDlg,958,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,100));

			if (!Misc_CreateUpDownButton(hDlg,922,981)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,924,982)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,942,983)) goto Error_0;
			if (!Misc_CreateUpDownButton(hDlg,944,984)) goto Error_0;
			Context->PageInitialized[8] = TRUE;
			break;
		}

	return;

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONEXCLAMATION);
	return;
}


// «»»» Place les valeurs «««««««««««««««««««««««««««««««««««««««««««««««»

int Properties_MapComputerSetValues(HWND hDlg, UINT Page, MAPPROPSCTX *MapProps)
{
	switch(Page)
		{
		//
		// Prédéfinitions
		//
		case 0:	if (!MapProps->PageInitialized[0]) return(0);
			SendMessage(MapProps->hDefinitions,LB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerAI,(LPARAM)0);
			return(1);

		//
		// Propiétés générales de construction
		//
		case 1:	if (!MapProps->PageInitialized[1]) return(0);

			SendMessage(MapProps->hRoomsOrder,LVM_SORTITEMS,(WPARAM)MapProps,(LPARAM)Properties_MapComputerSort);
			if (MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaX1 < 0)
				{
				SetDlgItemInt(hDlg,211,2,FALSE);
				SetDlgItemInt(hDlg,212,2,FALSE);
				SetDlgItemInt(hDlg,213,MapProps->MapProperties.Width-2,FALSE);
				SetDlgItemInt(hDlg,214,MapProps->MapProperties.Height-2,FALSE);
				}
			else
				{
				SetDlgItemInt(hDlg,211,MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaX1,FALSE);
				SetDlgItemInt(hDlg,212,MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaY1,FALSE);
				SetDlgItemInt(hDlg,213,MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaX2,FALSE);
				SetDlgItemInt(hDlg,214,MapProps->CurrentPlayerCPUSettings.PlayerCPUAreaY2,FALSE);
				}
			SendDlgItemMessage(hDlg,211,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,212,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,213,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,214,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,221,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			return(1);

		//
		// Propriétés de construction des salles
		//
		case 2:	if (!MapProps->PageInitialized[2]) return(0);

			SetDlgItemInt(hDlg,303,MapProps->CurrentPlayerCPUSettings.PlayerResearchedRoom,FALSE);
			SetDlgItemInt(hDlg,306,MapProps->CurrentPlayerCPUSettings.PlayerRebuildRoom,FALSE);
			SetDlgItemInt(hDlg,322,MapProps->CurrentPlayerCPUSettings.PlayerDefaultTiles,FALSE);
			SetDlgItemInt(hDlg,325,MapProps->CurrentPlayerCPUSettings.PlayerTilesBetweenRooms,FALSE);
			SetDlgItemInt(hDlg,332,MapProps->CurrentPlayerCPUSettings.PlayerTilesFromHeart,FALSE);
			SetDlgItemInt(hDlg,335,MapProps->CurrentPlayerCPUSettings.PlayerTilesFromClaimed,FALSE);
			SetDlgItemInt(hDlg,345,MapProps->CurrentPlayerCPUSettings.PlayerFlexibility,FALSE);
			SendDlgItemMessage(hDlg,303,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,306,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,322,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,325,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,332,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,335,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,345,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,309,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerExpandMode,(LPARAM)0);
			SendDlgItemMessage(hDlg,328,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerDistance,(LPARAM)0);
			SendDlgItemMessage(hDlg,330,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerCorridorStyle,(LPARAM)0);
			CheckDlgButton(hDlg,341,(MapProps->CurrentPlayerCPUSettings.PlayerCreateEmptyAreas)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,342,(MapProps->CurrentPlayerCPUSettings.PlayerBuildBiggerLair)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,343,(MapProps->CurrentPlayerCPUSettings.PlayerSellCapturedRoom)?BST_CHECKED:BST_UNCHECKED);
			return(1);

		//
		// Pièges et portes
		//
		case 3:	if (!MapProps->PageInitialized[3]) return(0);
			SendDlgItemMessage(hDlg,402,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerTrapUseStyle);
			SendDlgItemMessage(hDlg,406,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerPreference);
			SendDlgItemMessage(hDlg,442,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerUseTrapsnDoors);
			CheckRadioButton(hDlg,421,423,MapProps->CurrentPlayerCPUSettings.PlayerDoorUsage+421);
			CheckDlgButton(hDlg,431,(MapProps->CurrentPlayerCPUSettings.PlayerBoulderInCorridors)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,432,(MapProps->CurrentPlayerCPUSettings.PlayerBoulderInAngles)?BST_CHECKED:BST_UNCHECKED);
			SetDlgItemInt(hDlg,445,MapProps->CurrentPlayerCPUSettings.PlayerUseTrapsnDoors,FALSE);
			return(1);

		//
		// Sorts
		//
		case 4:	if (!MapProps->PageInitialized[4]) return(0);
			SendDlgItemMessage(hDlg,502,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerSpellsInBattle);
			SendDlgItemMessage(hDlg,506,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerSpellsPower);
			SendDlgItemMessage(hDlg,522,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerSpellsSightOfEvil,(LPARAM)0);
			SendDlgItemMessage(hDlg,524,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerSpellsCallToArms,(LPARAM)0);
			CheckDlgButton(hDlg,525,(MapProps->CurrentPlayerCPUSettings.PlayerSpellsLightning)?BST_CHECKED:BST_UNCHECKED);
			return(1);

		//
		// Créatures
		//
		case 5:	if (!MapProps->PageInitialized[5]) return(0);
			CheckDlgButton(hDlg,601,(MapProps->CurrentPlayerCPUSettings.PlayerCreaturesHappyUnhappy)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,602,(MapProps->CurrentPlayerCPUSettings.PlayerCreaturesHappyUngry)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,603,(MapProps->CurrentPlayerCPUSettings.PlayerCreaturesDisposeUngry)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,604,(MapProps->CurrentPlayerCPUSettings.PlayerCreaturesDisposeRubish)?BST_CHECKED:BST_UNCHECKED);
			SendDlgItemMessage(hDlg,622,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerCreaturesSackMethod,(LPARAM)0);
			SetDlgItemInt(hDlg,624,MapProps->CurrentPlayerCPUSettings.PlayerCreaturesImps,FALSE);
			SendDlgItemMessage(hDlg,624,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			CheckDlgButton(hDlg,625,(MapProps->CurrentPlayerCPUSettings.PlayerCreaturesSlap&0x20)?BST_CHECKED:BST_UNCHECKED);
			return(1);

		//
		// Bataille initiale
		//
		case 6:	if (!MapProps->PageInitialized[6]) return(0);
			CheckDlgButton(hDlg,701,(MapProps->CurrentPlayerCPUSettings.PlayerUseLevelInFirstAttack)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,704,BST_INDETERMINATE);
			CheckDlgButton(hDlg,708,(MapProps->CurrentPlayerCPUSettings.PlayerUseThreatInFirstAttack)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,712,(MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackAllRooms)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,714,(MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackAllSpells)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,741,(MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackOnlyAttackers)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,742,(MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackDontAttack)?BST_CHECKED:BST_UNCHECKED);
			SetDlgItemInt(hDlg,703,MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackLevel,FALSE);
			SetDlgItemInt(hDlg,706,MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackCount,FALSE);
			SetDlgItemInt(hDlg,710,MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackThreat,FALSE);
			SetDlgItemInt(hDlg,726,MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackPoints,FALSE);
			SetDlgItemInt(hDlg,729,MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackCreatures,FALSE);
			SendDlgItemMessage(hDlg,703,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);
			SendDlgItemMessage(hDlg,706,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,710,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,726,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,729,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,722,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackBreach,(LPARAM)0);
			SendDlgItemMessage(hDlg,724,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerFirstAttackDig,(LPARAM)0);
			return(1);

		//
		// Batailles
		//
		case 7:	if (!MapProps->PageInitialized[7]) return(0);
			SendDlgItemMessage(hDlg,805,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerBattleSpellType);
			CheckDlgButton(hDlg,821,(MapProps->CurrentPlayerCPUSettings.PlayerBattleRemove)?BST_CHECKED:BST_UNCHECKED);
			SetDlgItemInt(hDlg,802,MapProps->CurrentPlayerCPUSettings.PlayerBattleDropped,FALSE);
			SetDlgItemInt(hDlg,809,MapProps->CurrentPlayerCPUSettings.PlayerBattlePrison,FALSE);
			SetDlgItemInt(hDlg,824,MapProps->CurrentPlayerCPUSettings.PlayerBattleHealth,FALSE);
			SetDlgItemInt(hDlg,842,MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsPlace,FALSE);
			SetDlgItemInt(hDlg,844,MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemove,FALSE);
			SetDlgItemInt(hDlg,847,MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoCreature,FALSE);
			SetDlgItemInt(hDlg,850,MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoDanger1,FALSE);
			SetDlgItemInt(hDlg,852,MapProps->CurrentPlayerCPUSettings.PlayerBattleCallToHarmsRemoveNoDanger2,FALSE);
			SetDlgItemInt(hDlg,862,MapProps->CurrentPlayerCPUSettings.PlayerBattleWait,FALSE);
			SendDlgItemMessage(hDlg,802,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,809,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,824,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,842,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,844,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,847,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,850,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,852,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,862,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			return(1);

		//
		// Divers
		//
		case 8:	if (!MapProps->PageInitialized[8]) return(0);
			SendDlgItemMessage(hDlg,902,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerLibraryUsage,(LPARAM)0);
			SendDlgItemMessage(hDlg,904,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayerCPUSettings.PlayerJailedFate,(LPARAM)0);

			SetDlgItemInt(hDlg,922,MapProps->CurrentPlayerCPUSettings.PlayerMineGoldAmount,FALSE);
			SetDlgItemInt(hDlg,924,MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsTiles,FALSE);
			SetDlgItemInt(hDlg,942,MapProps->CurrentPlayerCPUSettings.PlayerImpsToTilesRatio,FALSE);
			SetDlgItemInt(hDlg,944,MapProps->CurrentPlayerCPUSettings.PlayerMana,FALSE);
			SendDlgItemMessage(hDlg,922,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,924,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,942,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,944,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);

			SendDlgItemMessage(hDlg,927,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsExplore);
			SendDlgItemMessage(hDlg,931,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsChances);
			SendDlgItemMessage(hDlg,946,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerSpeed);
			SendDlgItemMessage(hDlg,949,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerManaValue);
			SendDlgItemMessage(hDlg,953,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerGoldValue);
			SendDlgItemMessage(hDlg,958,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)MapProps->CurrentPlayerCPUSettings.PlayerOpenness);

			SetDlgItemInt(hDlg,928,MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsExplore,FALSE);
			SetDlgItemInt(hDlg,932,MapProps->CurrentPlayerCPUSettings.PlayerFindSpecialsChances,FALSE);
			SetDlgItemInt(hDlg,947,MapProps->CurrentPlayerCPUSettings.PlayerSpeed,FALSE);
			SetDlgItemInt(hDlg,950,MapProps->CurrentPlayerCPUSettings.PlayerManaValue,FALSE);
			SetDlgItemInt(hDlg,954,MapProps->CurrentPlayerCPUSettings.PlayerGoldValue,FALSE);
			SetDlgItemInt(hDlg,959,MapProps->CurrentPlayerCPUSettings.PlayerOpenness,FALSE);
			return(1);
		}

	return(0);
} 


// «»»» Classement des salles «««««««««««««««««««««««««««««««««««««««««««»

int CALLBACK Properties_MapComputerSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	MAPPROPSCTX	*MapProps;
	int		 i,j;

	MapProps = (MAPPROPSCTX *)lParamSort;

	for (i = 0; i != 15; i++)
		if (MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i] == lParam1) break;
	for (j = 0; j != 15; j++)
		if (MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[j] == lParam2) break;

	if (i < j) return(-1);
	if (i > j) return(1);

	return(0);
}


// «»»» Change le classement ««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapComputerChangeOrder(HWND hDlg, BOOL MoveDown, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	int	i,j,k;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(MapProps->hRoomsOrder,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) return;
	if (!SendMessage(MapProps->hRoomsOrder,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;

	for (i = 0; i != 15; i++)
		if (MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i] == Item.lParam) break;

	if (i == 15) return;

	if (MoveDown)
		{
		if (i == 14) return;
		j = MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i];
		k = MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i+1];
		MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i+1] = (BYTE)j;
		MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i] = (BYTE)k;
		}
	else
		{
		if (!i) return;
		j = MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i];
		k = MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i-1];
		MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i-1] = (BYTE)j;
		MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i] = (BYTE)k;
		}

	SendMessage(MapProps->hRoomsOrder,LVM_SORTITEMS,(WPARAM)MapProps,(LPARAM)Properties_MapComputerSort);
	SendMessage(MapProps->hRoomsOrder,LVM_ENSUREVISIBLE,(WPARAM)SendMessage(MapProps->hRoomsOrder,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED),(LPARAM)FALSE);

	EnableWindow(GetDlgItem(hDlg,201),(i != 1)?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,202),(i != 13)?TRUE:FALSE);
	return;
}


// «»»» Active/Désactive les flèches ««««««««««««««««««««««««««««««««««««»

void Properties_MapComputerToggleArrows(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	int	i;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(MapProps->hRoomsOrder,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) goto Disable;
	if (!SendMessage(MapProps->hRoomsOrder,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) goto Disable;

	for (i = 0; i != 15; i++)
		if (MapProps->CurrentPlayerCPUSettings.PlayerRoomToBuildPri[i] == Item.lParam) break;

	EnableWindow(GetDlgItem(hDlg,201),(i)?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,202),(i != 14)?TRUE:FALSE);
	return;

Disable:EnableWindow(GetDlgItem(hDlg,201),FALSE);
	EnableWindow(GetDlgItem(hDlg,202),FALSE);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
