
//<<>-<>>---------------------------------------------------------------------()
/*
	Variables globales du jeu -- Eléments
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
extern GLOBALVARS	MapGlobalVariables;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_GlobalVariables(HWND hWnd)
{
	static DLGPROC	 pfn[]={Properties_GlobalTerrainProc,
				Properties_GlobalRoomProc,
				Properties_GlobalDoorProc,
				Properties_GlobalTrapProc,
				Properties_GlobalSpellsProc};
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	GLOBALCTX	*Context;
	int		 Result;

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*5);
	if (!psp) goto Error_1;
	Context = GlobalAlloc(GPTR,sizeof(GLOBALCTX));
	if (!Context) goto Error_2;

	for (Result = 0; Result != 5; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(6820+Result);
		psp[Result].pfnDlgProc = pfn[Result];
		psp[Result].lParam = (LPARAM)Context;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID;
	psh->hwndParent = hWnd;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = szGlobalVariables;
	psh->nPages = 5;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	if (!Misc_CreateImageList(GTerrains,&Context->TerrainsImages,32,32,FALSE))
		goto Error_4;
	if (!Misc_CreateImageList(GRooms,&Context->RoomsImages,32,32,FALSE))
		goto Error_4;
	if (!Misc_CreateImageList(GDoors,&Context->DoorsImages,32,32,FALSE))
		goto Error_4;
	if (!Misc_CreateImageList(GTraps,&Context->TrapsImages,32,32,FALSE))
		goto Error_4;
	if (!Misc_CreateImageList(GSpells,&Context->SpellsImages,32,32,FALSE))
		goto Error_4;

	CopyMemory(&Context->Variables,&MapGlobalVariables,sizeof(GLOBALVARS));
	Result = PropertySheet(psh);
	if (Result == -1) goto Error_4;

	Info_Update();
	Misc_DestroyImageList(&Context->SpellsImages);
	Misc_DestroyImageList(&Context->TrapsImages);
	Misc_DestroyImageList(&Context->DoorsImages);
	Misc_DestroyImageList(&Context->RoomsImages);
	Misc_DestroyImageList(&Context->TerrainsImages);
	GlobalFree(Context);
	GlobalFree(psp);
	GlobalFree(psh);
	Files_SaveUpdate();
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_4:Misc_DestroyImageList(&Context->SpellsImages);
	Misc_DestroyImageList(&Context->TrapsImages);
	Misc_DestroyImageList(&Context->DoorsImages);
	Misc_DestroyImageList(&Context->RoomsImages);
	Misc_DestroyImageList(&Context->TerrainsImages);
Error_3:GlobalFree(Context);
Error_2:GlobalFree(psp);
Error_1:GlobalFree(psh);
Error_0:Misc_MessageBoxExt(hWnd,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus des boîtes de dialogue					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Propriétés des terrains «««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalTerrainProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInit(hDlg,0,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON )
				{
				switch( ((DRAWITEMSTRUCT *)lParam)->CtlID )
					{
					case 145:
						RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->TerrainColor);
						break;
					case 146:
						RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->AmbientColor);
						break;
					}
				return(TRUE);
				}

			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GTerrains,Context->TerrainsImages);
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
				case LBN_SELCHANGE:
					Properties_GlobalSaveChanges(hDlg,0,Context);
					Properties_GlobalGetParams(hDlg,0,Context);
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 124:
							lData = GetDlgItemInt(hDlg,124,NULL,TRUE);
							if (lData < -32768) SetDlgItemInt(hDlg,124,-32768,TRUE);
							if (lData > 32767) SetDlgItemInt(hDlg,124,32767,TRUE);
							break;

						case 126:
						case 128:
						case 130:
						case 132:
						case 134:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 65535) SetDlgItemInt(hDlg,LOWORD(wParam),65535,FALSE);
							break;

						case 136:
						case 138:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 255) SetDlgItemInt(hDlg,LOWORD(wParam),255,FALSE);
							break;
						case 150:
							lData = GetDlgItemInt(hDlg,150,NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,150,0);
							if (lData > 16000) Misc_SetDlgItemFloat(hDlg,150,16000<<12);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 147:
							RectEx_ChooseColor(hDlg,145,&Context->TerrainColor,Context->TerrainColor);
							break;
						case 148:
							RectEx_ChooseColor(hDlg,146,&Context->AmbientColor,Context->AmbientColor);
							break;

						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentTerrain,MGRF_TERRAINS);
							Properties_GlobalGetParams(hDlg,0,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_TERRAINS);
							Properties_GlobalGetParams(hDlg,0,Context);
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
						case 224:
							Misc_SetUpDownIntSigned(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						case 226:
						case 228:
						case 230:
						case 232:
						case 234:
						case 236:
						case 238:
						case 250:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChanges(hDlg,0,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValues(hDlg,0,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Propriétés des salles «««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalRoomProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInit(hDlg,1,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON )
				{
				switch( ((DRAWITEMSTRUCT *)lParam)->CtlID )
					{
					case 146:
						RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->TorchColor);
						break;
					}
				return(TRUE);
				}
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GRooms,Context->RoomsImages);
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
				case LBN_SELCHANGE:
					Properties_GlobalSaveChanges(hDlg,1,Context);
					Properties_GlobalGetParams(hDlg,1,Context);
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 143:
							lData = GetDlgItemInt(hDlg,143,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,143,0,FALSE);
							if (lData > 65535) SetDlgItemInt(hDlg,143,65535,FALSE);
							break;
						case 148:
							lData = GetDlgItemInt(hDlg,148,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,148,0,FALSE);
							if (lData > 255) SetDlgItemInt(hDlg,148,255,FALSE);
							break;
						case 150:
							lData = GetDlgItemInt(hDlg,150,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,150,0,FALSE);
							if (lData > 8) SetDlgItemInt(hDlg,150,8,FALSE);
							break;
						case 152:
							lData = GetDlgItemInt(hDlg,152,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,152,0,FALSE);
							if (lData > 3) SetDlgItemInt(hDlg,152,3,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 147:
							RectEx_ChooseColor(hDlg,146,&Context->TorchColor,Context->TorchColor);
							break;

						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentRoom,MGRF_ROOMS);
							Properties_GlobalGetParams(hDlg,1,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_ROOMS);
							Properties_GlobalGetParams(hDlg,1,Context);
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
						case 243:
							Misc_SetUpDownInt(hDlg,143,(NM_UPDOWN *)lParam);
							break;
						case 248:
							Misc_SetUpDownInt(hDlg,148,(NM_UPDOWN *)lParam);
							break;
						case 250:
							Misc_SetUpDownInt(hDlg,150,(NM_UPDOWN *)lParam);
							break;
						case 252:
							Misc_SetUpDownInt(hDlg,152,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChanges(hDlg,1,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValues(hDlg,1,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Propriétés des portes «««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalDoorProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInit(hDlg,2,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GDoors,Context->DoorsImages);
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
				case LBN_SELCHANGE:
					Properties_GlobalSaveChanges(hDlg,2,Context);
					Properties_GlobalGetParams(hDlg,2,Context);
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 114:
						case 116:
						case 118:
						case 120:
						case 122:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 65535) SetDlgItemInt(hDlg,LOWORD(wParam),65535,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentDoor,MGRF_DOORS);
							Properties_GlobalGetParams(hDlg,2,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_DOORS);
							Properties_GlobalGetParams(hDlg,2,Context);
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
						case 214:
						case 216:
						case 218:
						case 220:
						case 222:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChanges(hDlg,2,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValues(hDlg,2,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Propriétés des pièges «««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalTrapProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInit(hDlg,3,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GTraps,Context->TrapsImages);
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
				case LBN_SELCHANGE:
					Properties_GlobalSaveChanges(hDlg,3,Context);
					Properties_GlobalGetParams(hDlg,3,Context);
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 120:
						case 138:
						case 140:
						case 148:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 16000) SetDlgItemInt(hDlg,LOWORD(wParam),16000,FALSE);
							break;

						case 158:
						case 160:
						case 134:
						case 136:
						case 142:
						case 144:
						case 146:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 65535) SetDlgItemInt(hDlg,LOWORD(wParam),65535,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 151:
							Properties_GlobalSetTypeOptions(hDlg,3,Context);
							break;
						case 155:
							Properties_GlobalSetShotOptions(hDlg,3,Context);
							break;
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentTrap,MGRF_TRAPS);
							Properties_GlobalGetParams(hDlg,3,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_TRAPS);
							Properties_GlobalGetParams(hDlg,3,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChanges(hDlg,3,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValues(hDlg,3,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Propriétés des sorts ««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalSpellsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInit(hDlg,4,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GSpells,Context->SpellsImages);
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
				case LBN_SELCHANGE:
					Properties_GlobalSaveChanges(hDlg,4,Context);
					Properties_GlobalGetParams(hDlg,4,Context);
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 124:
							lData = GetDlgItemInt(hDlg,124,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,124,0,FALSE);
							if (lData > 999999) SetDlgItemInt(hDlg,124,999999,FALSE);
							break;
						case 126:
						case 128:
						case 130:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 32767) SetDlgItemInt(hDlg,LOWORD(wParam),32767,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 145:
							Context->BonusShot = FALSE;
							Properties_GlobalSetShotOptions(hDlg,4,Context);
							break;
						case 148:
							Context->BonusShot = TRUE;
							Properties_GlobalSetShotOptions(hDlg,4,Context);
							break;
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentSpell,MGRF_SPELLS);
							Properties_GlobalGetParams(hDlg,4,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_SPELLS);
							Properties_GlobalGetParams(hDlg,4,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChanges(hDlg,4,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValues(hDlg,4,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection des paramètres d'activation				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Changement des paramètres d'activation ««««««««««««««««««««««««««»

void Properties_GlobalSetTypeOptions(HWND hDlg, UINT Page, GLOBALCTX *Context)
{
	int	Result;

	Properties_GlobalSaveChanges(hDlg,Page,Context);

	switch(Page)
		{
		case 3:	Context->TriggerType = Context->Variables.Traps[Context->CurrentTrap].TriggerType;
			Context->TriggerData = Context->Variables.Traps[Context->CurrentTrap].TriggerData;
			break;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6830),hDlg,Properties_GlobalTypeProc,(LPARAM)Context);
	if ((Result == -1)||(Result == 0))
		{
		if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	switch(Page)
		{
		case 3:	Context->Variables.Traps[Context->CurrentTrap].TriggerData = Context->TriggerData;
			break;
		}

	return;
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalTypeProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	 X,Y;
		char	*Text;

		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

		Context = (GLOBALCTX *)lParam;
		switch(Context->TriggerType)
			{
			case 1:	Text = szGlobalLineOfSight;
				break;
			case 2:	Text = szGlobalPressure;
				break;
			case 3:	Text = szGlobalTrigger;
				break;
			}

		Misc_CreateUpDownButton(hDlg,101,102);
		SetDlgItemText(hDlg,100,Text);
		SetDlgItemInt(hDlg,101,Context->TriggerData,FALSE);
		SetWindowLong(hDlg,DWL_USER,(LPARAM)Context);
		return(TRUE);
		}

	Context = (GLOBALCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch( ((NMHDR *)lParam)->idFrom )
						{
						case 102:
							Misc_SetUpDownInt(hDlg,101,(NM_UPDOWN *)lParam);
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
							lData = GetDlgItemInt(hDlg,101,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,101,0,FALSE);
							if (lData > 32767) SetDlgItemInt(hDlg,101,32767,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Context->TriggerData = GetDlgItemInt(hDlg,101,NULL,FALSE);
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection des paramètres des tirs				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Changement des paramètres des tirs ««««««««««««««««««««««««««««««»

void Properties_GlobalSetShotOptions(HWND hDlg, UINT Page, GLOBALCTX *Context)
{
	Properties_GlobalSaveChanges(hDlg,Page,Context);

	switch(Page)
		{
		case 3:	Context->ShotCtx.ShotType = Context->Variables.Traps[Context->CurrentTrap].ShotType;
			Context->ShotCtx.ShotData1 = Context->Variables.Traps[Context->CurrentTrap].ShotData1;
			Context->ShotCtx.ShotData2 = Context->Variables.Traps[Context->CurrentTrap].ShotData2;
			break;

		case 4:	if (!Context->BonusShot)
				Context->ShotCtx.ShotType = Context->Variables.Spells[Context->CurrentSpell].ShotType;
			else
				Context->ShotCtx.ShotType = Context->Variables.Spells[Context->CurrentSpell].UpgShotType;

			if (!Context->BonusShot)
				{
				Context->ShotCtx.ShotData1 = Context->Variables.Spells[Context->CurrentSpell].ShotData1;
				Context->ShotCtx.ShotData2 = Context->Variables.Spells[Context->CurrentSpell].ShotData2;
				}
			else
				{
				Context->ShotCtx.ShotData1 = Context->Variables.Spells[Context->CurrentSpell].UpgShotData1;
				Context->ShotCtx.ShotData2 = Context->Variables.Spells[Context->CurrentSpell].UpgShotData2;
				}
			break;

		default:return;
		}

	if (!Properties_GlobalShotOptions(hDlg,&Context->ShotCtx)) return;

	switch(Page)
		{
		case 3:	Context->Variables.Traps[Context->CurrentTrap].ShotData1 = Context->ShotCtx.ShotData1;
			Context->Variables.Traps[Context->CurrentTrap].ShotData2 = Context->ShotCtx.ShotData2;
			break;
		case 4:	if (!Context->BonusShot)
				{
				Context->Variables.Spells[Context->CurrentSpell].ShotData1 = Context->ShotCtx.ShotData1;
				Context->Variables.Spells[Context->CurrentSpell].ShotData2 = Context->ShotCtx.ShotData2;
				}
			else
				{
				Context->Variables.Spells[Context->CurrentSpell].UpgShotData1 = Context->ShotCtx.ShotData1;
				Context->Variables.Spells[Context->CurrentSpell].UpgShotData2 = Context->ShotCtx.ShotData2;
				}
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines communes aux boîtes de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisations de la boîte de dialogue «««««««««««««««««««««««««»

void Properties_GlobalInit(HWND hDlg, UINT Page, PROPSHEETPAGE *PageStruct)
{
	GLOBALCTX	*Context;
	int		 i;

	Context = (GLOBALCTX *)(PageStruct->lParam);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Page)
		{
		//
		// Propriétés des terrains
		//
		case 0:
			for (i = 0; GTerrains[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GTerrains[i].Id) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTerrains[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTerrains[i].Name) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateUpDownButton(hDlg,124,224);
			Misc_CreateUpDownButton(hDlg,126,226);
			Misc_CreateUpDownButton(hDlg,128,228);
			Misc_CreateUpDownButton(hDlg,130,230);
			Misc_CreateUpDownButton(hDlg,132,232);
			Misc_CreateUpDownButton(hDlg,134,234);
			Misc_CreateUpDownButton(hDlg,136,236);
			Misc_CreateUpDownButton(hDlg,138,238);
			Misc_CreateUpDownButton(hDlg,150,250);
			SendDlgItemMessage(hDlg,124,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,126,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,128,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,130,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,132,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,134,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,136,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,138,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,150,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[0] = TRUE;
			break;

		//
		// Propriétés des salles
		//
		case 1:
			for (i = 0; GRooms[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GRooms[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,108,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,110,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,112,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				}

			for (i = 124; i != 134+2; i += 2)
				if (SendDlgItemMessage(hDlg,i,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[0].Name) < 0)
					goto Error_0;
			for (i = 0; Objects[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,124,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,132,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateUpDownButton(hDlg,143,243);
			Misc_CreateUpDownButton(hDlg,148,248);
			Misc_CreateUpDownButton(hDlg,150,250);
			Misc_CreateUpDownButton(hDlg,152,252);
			SendDlgItemMessage(hDlg,143,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,148,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,150,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,152,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[1] = TRUE;
			break;

		//
		// Propriétés des portes
		//
		case 2:
			for (i = 0; GDoors[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GDoors[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GDoorsMats[i].Name) < 0)
					goto Error_0;

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;

			for (i = 0; GDoorsCrates[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,108,CB_ADDSTRING,(WPARAM)0,(LPARAM)GDoorsCrates[i].Name) < 0)
					goto Error_0;

			for (i = 0; GDoorsKeys[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,110,CB_ADDSTRING,(WPARAM)0,(LPARAM)GDoorsKeys[i].Name) < 0)
					goto Error_0;

			Misc_CreateUpDownButton(hDlg,114,214);
			Misc_CreateUpDownButton(hDlg,116,216);
			Misc_CreateUpDownButton(hDlg,118,218);
			Misc_CreateUpDownButton(hDlg,120,220);
			Misc_CreateUpDownButton(hDlg,122,222);
			SendDlgItemMessage(hDlg,114,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,116,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,118,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,120,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,122,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);

			Context->PageInitialized[2] = TRUE;
			break;

		//
		// Propriétés des pièges
		//
		case 3:
			for (i = 0; GTraps[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GTraps[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GDoorsMats[i].Name) < 0)
					goto Error_0;

			for (i = 0; GTrapsCrates[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTrapsCrates[i].Name) < 0)
					goto Error_0;

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,118,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GTrapsTypes[i].Id != 0; i++)
				if (SendDlgItemMessage(hDlg,150,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTrapsTypes[i].Name) < 0)
					goto Error_0;

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				if (SendDlgItemMessage(hDlg,154,CB_ADDSTRING,(WPARAM)0,(LPARAM)GShots[i].Name) < 0)
					goto Error_0;

			Misc_CreateUpDownButton(hDlg,158,258);
			Misc_CreateUpDownButton(hDlg,160,260);
			Misc_CreateUpDownButton(hDlg,120,220);
			Misc_CreateUpDownButton(hDlg,134,234);
			Misc_CreateUpDownButton(hDlg,136,236);
			Misc_CreateUpDownButton(hDlg,138,238);
			Misc_CreateUpDownButton(hDlg,140,240);
			Misc_CreateUpDownButton(hDlg,142,242);
			Misc_CreateUpDownButton(hDlg,144,244);
			Misc_CreateUpDownButton(hDlg,146,246);
			Misc_CreateUpDownButton(hDlg,148,248);
			SendDlgItemMessage(hDlg,158,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,160,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,120,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,134,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,136,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,138,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,140,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,142,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,144,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,146,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,148,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[3] = TRUE;
			break;

		//
		// Propriétés des sorts
		//
		case 4:
			for (i = 0; GSpells[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GSpells[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GSpellsHand[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GSpellsHand[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)GSpellsHand[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GSpellsCast[i].Id != 0; i++)
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)GSpellsCast[i].Name) < 0)
					goto Error_0;

			for (i = 0; GSpellsTrget[i].Id != 0; i++)
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)GSpellsTrget[i].Name) < 0)
					goto Error_0;

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,144,CB_ADDSTRING,(WPARAM)0,(LPARAM)GShots[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,147,CB_ADDSTRING,(WPARAM)0,(LPARAM)GShots[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateUpDownButton(hDlg,124,224);
			Misc_CreateUpDownButton(hDlg,126,226);
			Misc_CreateUpDownButton(hDlg,128,228);
			Misc_CreateUpDownButton(hDlg,130,230);
			SendDlgItemMessage(hDlg,124,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,126,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,128,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,130,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);

			Context->PageInitialized[4] = TRUE;
			break;
		}

	return;

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONEXCLAMATION);
	return;
}


// «»»» Place les valeurs «««««««««««««««««««««««««««««««««««««««««««««««»

int Properties_GlobalSetValues(HWND hDlg, UINT Page, GLOBALCTX *Context)
{
	switch(Page)
		{
		//
		// Propriétés des terrains
		//
		case 0:	if (!Context->PageInitialized[0]) return(0);
			Properties_GlobalGetParams(hDlg,0,Context);
			return(1);

		//
		// Propiétés des salles
		//
		case 1:	if (!Context->PageInitialized[1]) return(0);
			Properties_GlobalGetParams(hDlg,1,Context);
			return(1);

		//
		// Propriétés des portes
		//
		case 2:	if (!Context->PageInitialized[2]) return(0);
			Properties_GlobalGetParams(hDlg,2,Context);
			return(1);

		//
		// Propriétés des pièges
		//
		case 3:	if (!Context->PageInitialized[3]) return(0);
			Properties_GlobalGetParams(hDlg,3,Context);
			return(1);

		//
		// Propriétés des sorts
		//
		case 4:	if (!Context->PageInitialized[4]) return(0);
			Properties_GlobalGetParams(hDlg,4,Context);
			return(1);

		}

	return(0);
}


// «»»» Sauvegarde les valeurs ««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalSaveChanges(HWND hDlg, UINT Page, GLOBALCTX *Context)
{
	int	i;

	switch(Page)
		{
		//
		// Propriétés des terrains
		//
		case 0:	Context->Variables.Terrains[Context->CurrentTerrain].MaxHealthType	= GTerrains[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Terrains[Context->CurrentTerrain].DestroyedType	= GTerrains[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Terrains[Context->CurrentTerrain].MaxHealthEffect	= Effects[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Terrains[Context->CurrentTerrain].DestroyedEffect	= Effects[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Terrains[Context->CurrentTerrain].Damage		= GetDlgItemInt(hDlg,124,NULL,TRUE);
			Context->Variables.Terrains[Context->CurrentTerrain].GoldValue		= GetDlgItemInt(hDlg,126,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].StartingHealth	= GetDlgItemInt(hDlg,128,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].MaxHealth		= GetDlgItemInt(hDlg,130,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].ManaGain		= GetDlgItemInt(hDlg,132,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].MaxManaGain	= GetDlgItemInt(hDlg,134,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].WibbleH		= GetDlgItemInt(hDlg,136,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].WibbleV		= GetDlgItemInt(hDlg,138,NULL,FALSE);
			Context->Variables.Terrains[Context->CurrentTerrain].Visible		= (IsDlgButtonChecked(hDlg,139) == BST_CHECKED);
			Context->Variables.Terrains[Context->CurrentTerrain].TerrainLight	= (IsDlgButtonChecked(hDlg,143) == BST_CHECKED);
			Context->Variables.Terrains[Context->CurrentTerrain].AmbientLight	= (IsDlgButtonChecked(hDlg,144) == BST_CHECKED);
			Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightB	= (BYTE)((Context->TerrainColor&0x00FF0000)>>16);
			Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightG	= (BYTE)((Context->TerrainColor&0x0000FF00)>>8);
			Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightR	= (BYTE)((Context->TerrainColor&0x000000FF));
			Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightB	= (BYTE)((Context->AmbientColor&0x00FF0000)>>16);
			Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightG	= (BYTE)((Context->AmbientColor&0x0000FF00)>>8);
			Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightR	= (BYTE)((Context->AmbientColor&0x000000FF));
			if (!Context->Variables.Terrains[Context->CurrentTerrain].TerrainLight)
				{
				Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightB = 0;
				Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightG = 0;
				Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightR = 0;
				}
			if (!Context->Variables.Terrains[Context->CurrentTerrain].AmbientLight)
				{
				Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightB = 0;
				Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightG = 0;
				Context->Variables.Terrains[Context->CurrentTerrain].AmbientLightR = 0;
				}

			Context->Variables.Terrains[Context->CurrentTerrain].TerrainLightH = Misc_GetDlgItemFloat(hDlg,150);
			break;

		//
		// Propriétés des salles
		//

		case 1:	Context->Variables.Rooms[Context->CurrentRoom].Cost			= GetDlgItemInt(hDlg,143,NULL,FALSE);
			Context->Variables.Rooms[Context->CurrentRoom].BattleEffect		= Effects[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[0]		= Effects[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[1]		= Effects[SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[2]		= Effects[SendDlgItemMessage(hDlg,110,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[3]		= Effects[SendDlgItemMessage(hDlg,112,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[4]		= Effects[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Rooms[Context->CurrentRoom].Effects[5]		= Effects[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			i = SendDlgItemMessage(hDlg,124,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[0]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[0]	= 0;
			i = SendDlgItemMessage(hDlg,126,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[1]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[1]	= 0;
			i = SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[2]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[2]	= 0;
			i = SendDlgItemMessage(hDlg,130,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[3]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[3]	= 0;
			i = SendDlgItemMessage(hDlg,132,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[4]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[4]	= 0;
			i = SendDlgItemMessage(hDlg,134,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			if (i) Context->Variables.Rooms[Context->CurrentRoom].Objects[5]	= Objects[i-1].Id;
			else   Context->Variables.Rooms[Context->CurrentRoom].Objects[5]	= 0;
			Context->Variables.Rooms[Context->CurrentRoom].TorchLightB		= (BYTE)((Context->TorchColor&0x00FF0000)>>16);
			Context->Variables.Rooms[Context->CurrentRoom].TorchLightG		= (BYTE)((Context->TorchColor&0x0000FF00)>>8);
			Context->Variables.Rooms[Context->CurrentRoom].TorchLightR		= (BYTE)((Context->TorchColor&0x000000FF));
			Context->Variables.Rooms[Context->CurrentRoom].TorchIntensity		= GetDlgItemInt(hDlg,148,NULL,FALSE);
			Context->Variables.Rooms[Context->CurrentRoom].TorchLightH		= Misc_GetDlgItemFloat(hDlg,150);
			Context->Variables.Rooms[Context->CurrentRoom].TorchLightRad		= Misc_GetDlgItemFloat(hDlg,152);
			break;

		//
		// Propriétés des portes
		//

		case 2:	Context->Variables.Doors[Context->CurrentDoor].Cost			= GetDlgItemInt(hDlg,114,NULL,FALSE);
			Context->Variables.Doors[Context->CurrentDoor].BuildPoints		= GetDlgItemInt(hDlg,116,NULL,FALSE);
			Context->Variables.Doors[Context->CurrentDoor].Health			= GetDlgItemInt(hDlg,118,NULL,FALSE);
			Context->Variables.Doors[Context->CurrentDoor].HealthGain		= GetDlgItemInt(hDlg,120,NULL,FALSE);
			Context->Variables.Doors[Context->CurrentDoor].ManaCost			= GetDlgItemInt(hDlg,122,NULL,FALSE);
			Context->Variables.Doors[Context->CurrentDoor].Material			= GDoorsMats[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Doors[Context->CurrentDoor].DeathEffect		= Effects[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Doors[Context->CurrentDoor].Crate			= GDoorsCrates[SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Doors[Context->CurrentDoor].Key			= GDoorsKeys[SendDlgItemMessage(hDlg,110,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			break;

		//
		// Propriétés des pièges
		//

		case 3:	Context->Variables.Traps[Context->CurrentTrap].Material			= GDoorsMats[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].Crate			= GTrapsCrates[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].DeathEffect		= Effects[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].PowerlessEffect		= Effects[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].IdleEffect		= Effects[SendDlgItemMessage(hDlg,118,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].TriggerType		= GTrapsTypes[SendDlgItemMessage(hDlg,150,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].ShotType			= GShots[SendDlgItemMessage(hDlg,154,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Traps[Context->CurrentTrap].GoldCost			= GetDlgItemInt(hDlg,158,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].BuildPoints		= GetDlgItemInt(hDlg,160,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].IdleEffectDelay		= Misc_GetDlgItemFloat(hDlg,120);
			Context->Variables.Traps[Context->CurrentTrap].Health			= GetDlgItemInt(hDlg,134,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].HealthGain		= GetDlgItemInt(hDlg,136,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].ChargeTime		= Misc_GetDlgItemFloat(hDlg,138);
			Context->Variables.Traps[Context->CurrentTrap].RechargeTime		= Misc_GetDlgItemFloat(hDlg,140);
			Context->Variables.Traps[Context->CurrentTrap].ManaUsage		= GetDlgItemInt(hDlg,142,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].ManaCostToFire		= GetDlgItemInt(hDlg,144,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].Threat			= GetDlgItemInt(hDlg,146,NULL,FALSE);
			Context->Variables.Traps[Context->CurrentTrap].ThreatDuration		= Misc_GetDlgItemFloat(hDlg,148);
			Context->Variables.Traps[Context->CurrentTrap].Invisible		= (IsDlgButtonChecked(hDlg,123))?TRUE:FALSE;
			Context->Variables.Traps[Context->CurrentTrap].Disarmable		= (IsDlgButtonChecked(hDlg,124))?TRUE:FALSE;
			Context->Variables.Traps[Context->CurrentTrap].RevealedWhenFired	= (IsDlgButtonChecked(hDlg,125))?TRUE:FALSE;
			Context->Variables.Traps[Context->CurrentTrap].RequiresMana		= (IsDlgButtonChecked(hDlg,126))?TRUE:FALSE;
			Context->Variables.Traps[Context->CurrentTrap].SolidObstacle		= (IsDlgButtonChecked(hDlg,127))?TRUE:FALSE;
			Context->Variables.Traps[Context->CurrentTrap].FPObstacle		= (IsDlgButtonChecked(hDlg,128))?TRUE:FALSE;
			break;

		//
		// Propriétés des sorts
		//

		case 4:	Context->Variables.Spells[Context->CurrentSpell].ManaCost		= GetDlgItemInt(hDlg,124,NULL,FALSE);
			Context->Variables.Spells[Context->CurrentSpell].ManaDrain		= GetDlgItemInt(hDlg,126,NULL,FALSE);
			Context->Variables.Spells[Context->CurrentSpell].RechargeTime		= Misc_GetDlgItemFloat(hDlg,128);
			Context->Variables.Spells[Context->CurrentSpell].Attacking		= (IsDlgButtonChecked(hDlg,131))?TRUE:FALSE;
			Context->Variables.Spells[Context->CurrentSpell].Defensive		= (IsDlgButtonChecked(hDlg,132))?TRUE:FALSE;
			Context->Variables.Spells[Context->CurrentSpell].HandAnim		= GSpellsHand[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].NoGoHandAnim		= GSpellsHand[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].CastRule		= GSpellsCast[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].TargetRule		= GSpellsTrget[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].ShotType		= GShots[SendDlgItemMessage(hDlg,144,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].UpgShotType		= GShots[SendDlgItemMessage(hDlg,147,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Spells[Context->CurrentSpell].UpgRechargeTime	= GetDlgItemInt(hDlg,130,NULL,FALSE);
			break;

		}

	return;
}


// «»»» Place les paramètres dans les gadgets «««««««««««««««««««««««««««»

void Properties_GlobalGetParams(HWND hDlg, UINT Page, GLOBALCTX *Context)
{
	int	Current;
	int	Activate;
	int	i;

	Current = SendDlgItemMessage(hDlg,101,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);

	switch(Page)
		{
		//
		// Propriétés des terrains
		//
		case 0:	Context->CurrentTerrain = Current;
			Context->TerrainColor = RGB(Context->Variables.Terrains[Current].TerrainLightR,Context->Variables.Terrains[Current].TerrainLightG,Context->Variables.Terrains[Current].TerrainLightB);
			Context->AmbientColor = RGB(Context->Variables.Terrains[Current].AmbientLightR,Context->Variables.Terrains[Current].AmbientLightG,Context->Variables.Terrains[Current].AmbientLightB);

			Activate = (GTerrains[Current].MinWidth)?1:0;
			EnableWindow(GetDlgItem(hDlg,103),Activate);
			EnableWindow(GetDlgItem(hDlg,104),Activate);
			EnableWindow(GetDlgItem(hDlg,113),Activate);
			EnableWindow(GetDlgItem(hDlg,114),Activate);
			EnableWindow(GetDlgItem(hDlg,105),Activate);
			EnableWindow(GetDlgItem(hDlg,106),Activate);
			EnableWindow(GetDlgItem(hDlg,115),Activate);
			EnableWindow(GetDlgItem(hDlg,116),Activate);
			EnableWindow(GetDlgItem(hDlg,139),(GTerrains[Current].MaxWidth)?FALSE:TRUE);

			SendDlgItemMessage(hDlg,104,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
			SendDlgItemMessage(hDlg,114,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
			for (i = 0; GTerrains[i].Id != 0; i++)
				{
				if ((Activate)&&(GTerrains[i].MinHeight)) break;
				SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTerrains[i].Name);
				SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)GTerrains[i].Name);
				}

			for (i = 0; GTerrains[i].Id != 0; i++)
				{
				if (Context->Variables.Terrains[Current].MaxHealthType == GTerrains[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Terrains[Current].DestroyedType == GTerrains[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Terrains[Current].MaxHealthEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Terrains[Current].DestroyedEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			Misc_SetDlgItemFloat(hDlg,150,Context->Variables.Terrains[Current].TerrainLightH);
			SetDlgItemInt(hDlg,124,Context->Variables.Terrains[Current].Damage,TRUE);
			SetDlgItemInt(hDlg,126,Context->Variables.Terrains[Current].GoldValue,FALSE);
			SetDlgItemInt(hDlg,128,Context->Variables.Terrains[Current].StartingHealth,FALSE);
			SetDlgItemInt(hDlg,130,Context->Variables.Terrains[Current].MaxHealth,FALSE);
			SetDlgItemInt(hDlg,132,Context->Variables.Terrains[Current].ManaGain,FALSE);
			SetDlgItemInt(hDlg,134,Context->Variables.Terrains[Current].MaxManaGain,FALSE);
			SetDlgItemInt(hDlg,136,Context->Variables.Terrains[Current].WibbleH,FALSE);
			SetDlgItemInt(hDlg,138,Context->Variables.Terrains[Current].WibbleV,FALSE);
			CheckDlgButton(hDlg,139,(Context->Variables.Terrains[Current].Visible)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,143,(Context->Variables.Terrains[Current].TerrainLight)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,144,(Context->Variables.Terrains[Current].AmbientLight)?BST_CHECKED:BST_UNCHECKED);
			InvalidateRect(GetDlgItem(hDlg,145),NULL,FALSE);
			InvalidateRect(GetDlgItem(hDlg,146),NULL,FALSE);
			break;

		//
		// Propriétés des salles
		//
		case 1:	Context->CurrentRoom = Current;
			Context->TorchColor = RGB(Context->Variables.Rooms[Current].TorchLightR,Context->Variables.Rooms[Current].TorchLightG,Context->Variables.Rooms[Current].TorchLightB);

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Rooms[Current].BattleEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[0] == Effects[i].Id)
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[1] == Effects[i].Id)
					SendDlgItemMessage(hDlg,108,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[2] == Effects[i].Id)
					SendDlgItemMessage(hDlg,110,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[3] == Effects[i].Id)
					SendDlgItemMessage(hDlg,112,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[4] == Effects[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Effects[5] == Effects[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; i != 6; i++)
				if (!Context->Variables.Rooms[Current].Objects[i])
					SendDlgItemMessage(hDlg,124+i*2,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; Objects[i].Id != 0; i++)
				{
				if (Context->Variables.Rooms[Current].Objects[0] == Objects[i].Id)
					SendDlgItemMessage(hDlg,124,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Objects[1] == Objects[i].Id)
					SendDlgItemMessage(hDlg,126,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Objects[2] == Objects[i].Id)
					SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Objects[3] == Objects[i].Id)
					SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Objects[4] == Objects[i].Id)
					SendDlgItemMessage(hDlg,132,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				if (Context->Variables.Rooms[Current].Objects[5] == Objects[i].Id)
					SendDlgItemMessage(hDlg,134,CB_SETCURSEL,(WPARAM)i+1,(LPARAM)0);
				}

			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT0))
				{
				EnableWindow(GetDlgItem(hDlg,105),FALSE);
				EnableWindow(GetDlgItem(hDlg,106),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,105),TRUE);
				EnableWindow(GetDlgItem(hDlg,106),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT1))
				{
				EnableWindow(GetDlgItem(hDlg,107),FALSE);
				EnableWindow(GetDlgItem(hDlg,108),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,107),TRUE);
				EnableWindow(GetDlgItem(hDlg,108),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT2))
				{
				EnableWindow(GetDlgItem(hDlg,109),FALSE);
				EnableWindow(GetDlgItem(hDlg,110),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,109),TRUE);
				EnableWindow(GetDlgItem(hDlg,110),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT3))
				{
				EnableWindow(GetDlgItem(hDlg,111),FALSE);
				EnableWindow(GetDlgItem(hDlg,112),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,111),TRUE);
				EnableWindow(GetDlgItem(hDlg,112),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT4))
				{
				EnableWindow(GetDlgItem(hDlg,113),FALSE);
				EnableWindow(GetDlgItem(hDlg,114),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,113),TRUE);
				EnableWindow(GetDlgItem(hDlg,114),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_EFFECT5))
				{
				EnableWindow(GetDlgItem(hDlg,115),FALSE);
				EnableWindow(GetDlgItem(hDlg,116),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,115),TRUE);
				EnableWindow(GetDlgItem(hDlg,116),TRUE);
				}

			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT0))
				{
				EnableWindow(GetDlgItem(hDlg,123),FALSE);
				EnableWindow(GetDlgItem(hDlg,124),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,123),TRUE);
				EnableWindow(GetDlgItem(hDlg,124),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT1))
				{
				EnableWindow(GetDlgItem(hDlg,125),FALSE);
				EnableWindow(GetDlgItem(hDlg,126),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,125),TRUE);
				EnableWindow(GetDlgItem(hDlg,126),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT2))
				{
				EnableWindow(GetDlgItem(hDlg,127),FALSE);
				EnableWindow(GetDlgItem(hDlg,128),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,127),TRUE);
				EnableWindow(GetDlgItem(hDlg,128),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT3))
				{
				EnableWindow(GetDlgItem(hDlg,129),FALSE);
				EnableWindow(GetDlgItem(hDlg,130),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,129),TRUE);
				EnableWindow(GetDlgItem(hDlg,130),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT4))
				{
				EnableWindow(GetDlgItem(hDlg,131),FALSE);
				EnableWindow(GetDlgItem(hDlg,132),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,131),TRUE);
				EnableWindow(GetDlgItem(hDlg,132),TRUE);
				}
			if (!(((DWORD)GRooms[Current].ASCIIName)&MGEF_OBJECT5))
				{
				EnableWindow(GetDlgItem(hDlg,133),FALSE);
				EnableWindow(GetDlgItem(hDlg,134),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,133),TRUE);
				EnableWindow(GetDlgItem(hDlg,134),TRUE);
				}

			if (((DWORD)GRooms[Current].ASCIIName)&MGEF_CANTBUILD)
				{
				EnableWindow(GetDlgItem(hDlg,143),FALSE);
				EnableWindow(GetDlgItem(hDlg,144),FALSE);
				EnableWindow(GetDlgItem(hDlg,243),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,143),TRUE);
				EnableWindow(GetDlgItem(hDlg,144),TRUE);
				EnableWindow(GetDlgItem(hDlg,243),TRUE);
				}

			Misc_SetDlgItemFloat(hDlg,150,Context->Variables.Rooms[Current].TorchLightH);
			Misc_SetDlgItemFloat(hDlg,152,Context->Variables.Rooms[Current].TorchLightRad);
			SetDlgItemInt(hDlg,143,Context->Variables.Rooms[Current].Cost,FALSE);
			SetDlgItemInt(hDlg,148,Context->Variables.Rooms[Current].TorchIntensity,FALSE);
			InvalidateRect(GetDlgItem(hDlg,146),NULL,FALSE);
			break;

		//
		// Propriétés des portes
		//

		case 2:	Context->CurrentDoor = Current;

			for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Doors[Context->CurrentDoor].Material == GDoorsMats[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Doors[Context->CurrentDoor].DeathEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GDoorsCrates[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Doors[Context->CurrentDoor].Crate == GDoorsCrates[i].Id)
					SendDlgItemMessage(hDlg,108,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GDoorsKeys[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Doors[Context->CurrentDoor].Key == GDoorsKeys[i].Id)
					SendDlgItemMessage(hDlg,110,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			SetDlgItemInt(hDlg,114,Context->Variables.Doors[Context->CurrentDoor].Cost,FALSE);
			SetDlgItemInt(hDlg,116,Context->Variables.Doors[Context->CurrentDoor].BuildPoints,FALSE);
			SetDlgItemInt(hDlg,118,Context->Variables.Doors[Context->CurrentDoor].Health,FALSE);
			SetDlgItemInt(hDlg,120,Context->Variables.Doors[Context->CurrentDoor].HealthGain,FALSE);
			SetDlgItemInt(hDlg,122,Context->Variables.Doors[Context->CurrentDoor].ManaCost,FALSE);
			break;

		//
		// Propriétés des pièges
		//

		case 3:	Context->CurrentTrap = Current;

			for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Traps[Context->CurrentTrap].Material == GDoorsMats[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GTrapsCrates[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Traps[Context->CurrentTrap].Crate == GTrapsCrates[i].Id)
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Traps[Context->CurrentTrap].DeathEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Traps[Context->CurrentTrap].PowerlessEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Traps[Context->CurrentTrap].IdleEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,118,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; GTrapsTypes[i].Id != 0; i++)
				if (Context->Variables.Traps[Context->CurrentTrap].TriggerType == GTrapsTypes[i].Id)
					SendDlgItemMessage(hDlg,150,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.Traps[Context->CurrentTrap].ShotType == GShots[i].Id)
					SendDlgItemMessage(hDlg,154,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			// MagicDoor inactive parameters
			if (GTraps[Context->CurrentTrap].Id == 0x0000000C)
				{
				EnableWindow(GetDlgItem(hDlg,103),FALSE);
				EnableWindow(GetDlgItem(hDlg,104),FALSE);
				EnableWindow(GetDlgItem(hDlg,105),FALSE);
				EnableWindow(GetDlgItem(hDlg,106),FALSE);
				EnableWindow(GetDlgItem(hDlg,113),FALSE);
				EnableWindow(GetDlgItem(hDlg,114),FALSE);
				EnableWindow(GetDlgItem(hDlg,123),FALSE);
				EnableWindow(GetDlgItem(hDlg,124),FALSE);
				EnableWindow(GetDlgItem(hDlg,125),FALSE);
				EnableWindow(GetDlgItem(hDlg,126),FALSE);
				EnableWindow(GetDlgItem(hDlg,127),FALSE);
				EnableWindow(GetDlgItem(hDlg,128),FALSE);
				EnableWindow(GetDlgItem(hDlg,157),FALSE);
				EnableWindow(GetDlgItem(hDlg,158),FALSE);
				EnableWindow(GetDlgItem(hDlg,258),FALSE);
				EnableWindow(GetDlgItem(hDlg,159),FALSE);
				EnableWindow(GetDlgItem(hDlg,160),FALSE);
				EnableWindow(GetDlgItem(hDlg,260),FALSE);
				EnableWindow(GetDlgItem(hDlg,133),FALSE);
				EnableWindow(GetDlgItem(hDlg,134),FALSE);
				EnableWindow(GetDlgItem(hDlg,234),FALSE);
				EnableWindow(GetDlgItem(hDlg,135),FALSE);
				EnableWindow(GetDlgItem(hDlg,136),FALSE);
				EnableWindow(GetDlgItem(hDlg,236),FALSE);
				EnableWindow(GetDlgItem(hDlg,141),FALSE);	// Mana usage
				EnableWindow(GetDlgItem(hDlg,142),FALSE);
				EnableWindow(GetDlgItem(hDlg,242),FALSE);
				}
			else
				{
				EnableWindow(GetDlgItem(hDlg,103),TRUE);
				EnableWindow(GetDlgItem(hDlg,104),TRUE);
				EnableWindow(GetDlgItem(hDlg,105),TRUE);
				EnableWindow(GetDlgItem(hDlg,106),TRUE);
				EnableWindow(GetDlgItem(hDlg,113),TRUE);
				EnableWindow(GetDlgItem(hDlg,114),TRUE);
				EnableWindow(GetDlgItem(hDlg,123),TRUE);
				EnableWindow(GetDlgItem(hDlg,124),TRUE);
				EnableWindow(GetDlgItem(hDlg,125),TRUE);
				EnableWindow(GetDlgItem(hDlg,126),TRUE);
				EnableWindow(GetDlgItem(hDlg,127),TRUE);
				EnableWindow(GetDlgItem(hDlg,128),TRUE);
				EnableWindow(GetDlgItem(hDlg,157),TRUE);
				EnableWindow(GetDlgItem(hDlg,158),TRUE);
				EnableWindow(GetDlgItem(hDlg,258),TRUE);
				EnableWindow(GetDlgItem(hDlg,159),TRUE);
				EnableWindow(GetDlgItem(hDlg,160),TRUE);
				EnableWindow(GetDlgItem(hDlg,260),TRUE);
				EnableWindow(GetDlgItem(hDlg,133),TRUE);
				EnableWindow(GetDlgItem(hDlg,134),TRUE);
				EnableWindow(GetDlgItem(hDlg,234),TRUE);
				EnableWindow(GetDlgItem(hDlg,135),TRUE);
				EnableWindow(GetDlgItem(hDlg,136),TRUE);
				EnableWindow(GetDlgItem(hDlg,236),TRUE);
				EnableWindow(GetDlgItem(hDlg,141),TRUE);	// Mana usage
				EnableWindow(GetDlgItem(hDlg,142),TRUE);
				EnableWindow(GetDlgItem(hDlg,242),TRUE);
				}

			SetDlgItemInt(hDlg,158,Context->Variables.Traps[Context->CurrentTrap].GoldCost,FALSE);
			SetDlgItemInt(hDlg,160,Context->Variables.Traps[Context->CurrentTrap].BuildPoints,FALSE);
			Misc_SetDlgItemFloat(hDlg,120,Context->Variables.Traps[Context->CurrentTrap].IdleEffectDelay);
			SetDlgItemInt(hDlg,134,Context->Variables.Traps[Context->CurrentTrap].Health,FALSE);
			SetDlgItemInt(hDlg,136,Context->Variables.Traps[Context->CurrentTrap].HealthGain,FALSE);
			Misc_SetDlgItemFloat(hDlg,138,Context->Variables.Traps[Context->CurrentTrap].ChargeTime);
			Misc_SetDlgItemFloat(hDlg,140,Context->Variables.Traps[Context->CurrentTrap].RechargeTime);
			SetDlgItemInt(hDlg,142,Context->Variables.Traps[Context->CurrentTrap].ManaUsage,FALSE);
			SetDlgItemInt(hDlg,144,Context->Variables.Traps[Context->CurrentTrap].ManaCostToFire,FALSE);
			SetDlgItemInt(hDlg,146,Context->Variables.Traps[Context->CurrentTrap].Threat,FALSE);
			Misc_SetDlgItemFloat(hDlg,148,Context->Variables.Traps[Context->CurrentTrap].ThreatDuration);

			CheckDlgButton(hDlg,123,(Context->Variables.Traps[Context->CurrentTrap].Invisible)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,124,(Context->Variables.Traps[Context->CurrentTrap].Disarmable)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,125,(Context->Variables.Traps[Context->CurrentTrap].RevealedWhenFired)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,126,(Context->Variables.Traps[Context->CurrentTrap].RequiresMana)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,127,(Context->Variables.Traps[Context->CurrentTrap].SolidObstacle)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,128,(Context->Variables.Traps[Context->CurrentTrap].FPObstacle)?BST_CHECKED:BST_UNCHECKED);
			break;

		//
		// Propriétés des sorts
		//

		case 4:	Context->CurrentSpell = Current;

			for (i = 0; GSpellsHand[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Spells[Context->CurrentSpell].HandAnim == GSpellsHand[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Spells[Context->CurrentSpell].NoGoHandAnim == GSpellsHand[i].Id)
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; GSpellsCast[i].Id != 0; i++)
				if (Context->Variables.Spells[Context->CurrentSpell].CastRule == GSpellsCast[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GSpellsTrget[i].Id != 0; i++)
				if (Context->Variables.Spells[Context->CurrentSpell].TargetRule == GSpellsTrget[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Spells[Context->CurrentSpell].ShotType == GShots[i].Id)
					SendDlgItemMessage(hDlg,144,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Spells[Context->CurrentSpell].UpgShotType == GShots[i].Id)
					SendDlgItemMessage(hDlg,147,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			SetDlgItemInt(hDlg,124,Context->Variables.Spells[Context->CurrentSpell].ManaCost,FALSE);
			SetDlgItemInt(hDlg,126,Context->Variables.Spells[Context->CurrentSpell].ManaDrain,FALSE);
			Misc_SetDlgItemFloat(hDlg,128,Context->Variables.Spells[Context->CurrentSpell].RechargeTime);
			SetDlgItemInt(hDlg,130,Context->Variables.Spells[Context->CurrentSpell].UpgRechargeTime,FALSE);
			CheckDlgButton(hDlg,131,(Context->Variables.Spells[Context->CurrentSpell].Attacking)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,132,(Context->Variables.Spells[Context->CurrentSpell].Defensive)?BST_CHECKED:BST_UNCHECKED);

			break;


		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
