
//<<>-<>>---------------------------------------------------------------------()
/*
	Variables globales du jeu -- Créatures
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
extern GLOBALVARS	MapGlobalVariables;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisation							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_GlobalCreatures(HWND hWnd)
{
	static DLGPROC	 pfn[]={Properties_GlobalAttributesProc,
				Properties_GlobalParamsProc,
				Properties_GlobalFlagsProc,
				Properties_GlobalAttacksProc,
				Properties_GlobalResistancesProc,
				Properties_GlobalPossessionProc,
				Properties_GlobalJobsProc,
				Properties_GlobalLightProc,
				Properties_GlobalCrSpellsProc};
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	GLOBALCTX2	*Context;
	int		 Result;

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*9);
	if (!psp) goto Error_1;
	Context = GlobalAlloc(GPTR,sizeof(GLOBALCTX2));
	if (!Context) goto Error_2;

	for (Result = 0; Result != 9; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(6860+Result);
		psp[Result].pfnDlgProc = pfn[Result];
		psp[Result].lParam = (LPARAM)Context;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID;
	psh->hwndParent = hWnd;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = szGlobalCreatures;
	psh->nPages = 9;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	if (!Misc_CreateImageList(GCreatures,&Context->CreaturesImages,32,32,FALSE))
		goto Error_4;
	if (!Misc_CreateImageList(GCrSpellsLst,&Context->SpellsImages,32,32,FALSE))
		goto Error_4;

	CopyMemory(&Context->Variables,&MapGlobalVariables,sizeof(GLOBALVARS));
	Properties_GlobalReset(&Context->DefaultVr,-1,MGRF_ALL);
	Result = PropertySheet(psh);
	if (Result == -1) goto Error_4;

	Info_Update();
	Misc_DestroyImageList(&Context->SpellsImages);
	Misc_DestroyImageList(&Context->CreaturesImages);
	GlobalFree(Context);
	GlobalFree(psp);
	GlobalFree(psh);
	Files_SaveUpdate();
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_4:Misc_DestroyImageList(&Context->SpellsImages);
	Misc_DestroyImageList(&Context->CreaturesImages);
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

// «»»» Attributs «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalAttributesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,0,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW )
				{
				((MEASUREITEMSTRUCT *)lParam)->itemHeight = Font.FontHeight+4;
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
				return(TRUE);
				}
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW )
				{
				Properties_GlobalDrawAttributes((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					Properties_GlobalSetNewValue(hDlg,Context);
					return(TRUE);

				case LBN_SELCHANGE:
					Properties_GlobalSaveChangesCr(hDlg,0,Context);
					Properties_GlobalGetParamsCr(hDlg,0,Context);
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRATTRS);
							Properties_GlobalGetParamsCr(hDlg,0,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRATTRS);
							Properties_GlobalGetParamsCr(hDlg,0,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					Properties_GlobalModifyValue(hDlg,(NM_UPDOWN *)lParam,Context);
					return(TRUE);

				case LVN_ITEMCHANGED:
					Properties_GlobalChangeValueType(hDlg,Context);
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,0,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,0,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Paramètres ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalParamsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,1,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 129:
							lData = GetDlgItemInt(hDlg,129,NULL,FALSE);
							switch(SendDlgItemMessage(hDlg,127,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))
								{
								case 0:	break;
								default:if (lData < 0)
										{
										SetDlgItemInt(hDlg,129,0,FALSE);
										break;
										}
									if (lData > 16000)
										{
										SetDlgItemInt(hDlg,129,16000,FALSE);
										break;
										}
									break;
								}
							return(TRUE);
						}
					break;

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,1,Context);
							Properties_GlobalGetParamsCr(hDlg,1,Context);
							return(TRUE);
						case 124:
							Properties_GlobalSetAttractions(hDlg,1,Context);
							return(TRUE);
						case 125:
							switch(SendDlgItemMessage(hDlg,124,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))
								{
								case 1:	Context->Variables.Creatures[Context->CurrentCreature].Attract1ID = GARooms[SendDlgItemMessage(hDlg,125,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
									break;
								}
							return(TRUE);
						case 127:
							Properties_GlobalSetAttractions(hDlg,2,Context);
							return(TRUE);
						case 128:
							switch(SendDlgItemMessage(hDlg,127,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))
								{
								case 1:	Context->Variables.Creatures[Context->CurrentCreature].Attract2ID = GARooms[SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
									break;
								case 4:	Context->Variables.Creatures[Context->CurrentCreature].Attract2ID = GCreatures[SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
									break;
								}
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRPARAMS);
							Properties_GlobalGetParamsCr(hDlg,1,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRPARAMS);
							Properties_GlobalGetParamsCr(hDlg,1,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					Misc_SetUpDownInt(hDlg,129,(NM_UPDOWN *)lParam);
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,1,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,1,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Drapeaux ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalFlagsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,2,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					Properties_GlobalSaveChangesCr(hDlg,2,Context);
					Properties_GlobalGetParamsCr(hDlg,2,Context);
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRFLAGS);
							Properties_GlobalGetParamsCr(hDlg,2,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRFLAGS);
							Properties_GlobalGetParamsCr(hDlg,2,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,2,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,2,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Attaques ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalAttacksProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,3,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 146:
							lData = GetDlgItemInt(hDlg,146,NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,146,0);
							if (lData > 255) Misc_SetDlgItemFloat(hDlg,146,255<<12);
							break;
						case 150:
							lData = GetDlgItemInt(hDlg,150,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,150,0,FALSE);
							if (lData > 32767) SetDlgItemInt(hDlg,150,32767,FALSE);
							break;
						case 148:
						case 152:
						case 108:
						case 120:
						case 132:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),0);
							if (lData > 1000) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),1000<<12);
							break;
						case 110:
						case 112:
						case 114:
						case 122:
						case 124:
						case 126:
						case 134:
						case 136:
						case 138:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,TRUE);
							if (lData < -32767) Misc_SetDlgItemFloatSigned(hDlg,LOWORD(wParam),-32767<<12);
							if (lData > 32767) Misc_SetDlgItemFloatSigned(hDlg,LOWORD(wParam),32767<<12);
							break;
						}
					return(TRUE);

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,3,Context);
							Properties_GlobalGetParamsCr(hDlg,3,Context);
							return(TRUE);
						case 104:
							Properties_GlobalChangeShots(hDlg,1);
							return(TRUE);
						case 116:
							Properties_GlobalChangeShots(hDlg,2);
							return(TRUE);
						case 128:
							Properties_GlobalChangeShots(hDlg,4);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRATTACKS);
							Properties_GlobalGetParamsCr(hDlg,3,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRATTACKS);
							Properties_GlobalGetParamsCr(hDlg,3,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch( ((NMHDR *)lParam)->idFrom )
						{
						case 208:
						case 220:
						case 232:
						case 246:
						case 248:
						case 250:
						case 252:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						case 210:
						case 212:
						case 214:
						case 222:
						case 224:
						case 226:
						case 234:
						case 236:
						case 238:
							Misc_SetUpDownIntSigned(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,3,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,3,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Résistances «««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalResistancesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,4,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,4,Context);
							Properties_GlobalGetParamsCr(hDlg,4,Context);
							return(TRUE);
						case 103:
							Properties_GlobalChangeRes(hDlg,1);
							return(TRUE);
						case 113:
							Properties_GlobalChangeRes(hDlg,2);
							return(TRUE);
						case 123:
							Properties_GlobalChangeRes(hDlg,4);
							return(TRUE);
						case 133:
							Properties_GlobalChangeRes(hDlg,8);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRRESISTANCES);
							Properties_GlobalGetParamsCr(hDlg,4,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRRESISTANCES);
							Properties_GlobalGetParamsCr(hDlg,4,Context);
							break;
						}
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
					if ((HWND)lParam == GetDlgItem(hDlg,104))
						{
						SetDlgItemInt(hDlg,107,SendDlgItemMessage(hDlg,104,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,114))
						{
						SetDlgItemInt(hDlg,117,SendDlgItemMessage(hDlg,114,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,124))
						{
						SetDlgItemInt(hDlg,127,SendDlgItemMessage(hDlg,124,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					if ((HWND)lParam == GetDlgItem(hDlg,134))
						{
						SetDlgItemInt(hDlg,137,SendDlgItemMessage(hDlg,134,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,4,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,4,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Possession ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalPossessionProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,5,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 105:
						case 108:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 32767) SetDlgItemInt(hDlg,LOWORD(wParam),32767,FALSE);
							return(TRUE);
						case 136:
						case 138:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),0);
							if (lData > 1) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),1<<12);
							return(TRUE);
						case 140:
							lData = GetDlgItemInt(hDlg,140,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,140,0,FALSE);
							if (lData > 12) SetDlgItemInt(hDlg,140,12,FALSE);
							return(TRUE);
						}
					break;

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,5,Context);
							Properties_GlobalGetParamsCr(hDlg,5,Context);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRPOSSESSION);
							Properties_GlobalGetParamsCr(hDlg,5,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRPOSSESSION);
							Properties_GlobalGetParamsCr(hDlg,5,Context);
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
					Properties_GlobalSaveChangesCr(hDlg,5,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,5,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Occupations «««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalJobsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,6,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				Properties_GlobalDrawJob((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 105:
						case 125:
						case 145:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 100) SetDlgItemInt(hDlg,LOWORD(wParam),100,FALSE);
							break;
						case 107:
						case 109:
						case 127:
						case 129:
						case 147:
						case 149:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,TRUE);
							if (lData < -32767) SetDlgItemInt(hDlg,LOWORD(wParam),-32767,TRUE);
							if (lData > 32767) SetDlgItemInt(hDlg,LOWORD(wParam),32767,TRUE);
							break;
						}
					return(TRUE);

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							if (!Properties_GlobalJobsCheckApply(hDlg,Context))
								{
								SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)Context->CurrentCreature,(LPARAM)0);
								return(TRUE);
								}
							Properties_GlobalSaveChangesCr(hDlg,6,Context);
							Properties_GlobalGetParamsCr(hDlg,6,Context);
							return(TRUE);
						}
					break;

				case BN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 112:
						case 113:
						case 114:
						case 132:
						case 133:
						case 152:
						case 153:
						case 154:
						case 172:
						case 173:
						case 174:
						case 175:
							Properties_GlobalToggleJob(hDlg,LOWORD(wParam),Context);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 112:
						case 113:
						case 114:
						case 132:
						case 133:
						case 152:
						case 153:
						case 154:
						case 172:
						case 173:
						case 174:
						case 175:
							Properties_GlobalToggleJob(hDlg,LOWORD(wParam),Context);
							break;
						case 111:
							Properties_GlobalRemJobs(hDlg,JOB_HAPPY,Context);
							break;
						case 131:
							Properties_GlobalRemJobs(hDlg,JOB_UNHAPPY,Context);
							break;
						case 151:
							Properties_GlobalRemJobs(hDlg,JOB_ANGRY,Context);
							break;
						case 171:
							Properties_GlobalRemJobs(hDlg,JOB_HATE,Context);
							break;
						case 110:
							Properties_GlobalAddJob(hDlg,JOB_HAPPY,Context);
							break;
						case 130:
							Properties_GlobalAddJob(hDlg,JOB_UNHAPPY,Context);
							break;
						case 150:
							Properties_GlobalAddJob(hDlg,JOB_ANGRY,Context);
							break;
						case 170:
							Properties_GlobalAddJob(hDlg,JOB_HATE,Context);
							break;

						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRJOBS);
							Properties_GlobalGetParamsCr(hDlg,6,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRJOBS);
							Properties_GlobalGetParamsCr(hDlg,6,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch( ((NMHDR *)lParam)->idFrom )
						{
						case 205:
						case 225:
						case 245:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						case 207:
						case 209:
						case 227:
						case 229:
						case 247:
						case 249:
							Misc_SetUpDownIntSigned(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					if (!Properties_GlobalJobsCheckApply(hDlg,Context))
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,TRUE);
						return(TRUE);
						}
					Properties_GlobalSaveChangesCr(hDlg,6,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_APPLY:
					if (!Properties_GlobalJobsCheckApply(hDlg,Context))
						{
						SetWindowLong(hDlg,DWL_MSGRESULT,PSNRET_INVALID_NOCHANGEPAGE);
						return(TRUE);
						}
					if (memcmp(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS)))
						{
						Undo_CreateGlobalVariables(&MapGlobalVariables);
						CopyMemory(&MapGlobalVariables,&Context->Variables,sizeof(GLOBALVARS));
						MapChanges++;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

                                case PSN_SETACTIVE:
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,6,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Lumière «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalLightProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,7,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON )
				{
				RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Context->LightColor);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 104:
						case 106:
						case 108:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,TRUE);
							if (lData < -32767) Misc_SetDlgItemFloatSigned(hDlg,LOWORD(wParam),-32767<<12);
							if (lData > 32767) Misc_SetDlgItemFloatSigned(hDlg,LOWORD(wParam),32767<<12);
							break;
						case 114:
							lData = GetDlgItemInt(hDlg,114,NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,114,0);
							if (lData > 3) Misc_SetDlgItemFloat(hDlg,114,3<<12);
							break;
						}
					return(TRUE);

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,7,Context);
							Properties_GlobalGetParamsCr(hDlg,7,Context);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 117:
							RectEx_ChooseColor(hDlg,116,&Context->LightColor,Context->LightColor);
							break;

						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentCreature,MGRF_CRLIGHT);
							Properties_GlobalGetParamsCr(hDlg,7,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CRLIGHT);
							Properties_GlobalGetParamsCr(hDlg,7,Context);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch( ((NMHDR *)lParam)->idFrom )
						{
						case 204:
						case 206:
						case 208:
							Misc_SetUpDownIntSigned(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						case 214:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,7,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,7,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Sorts «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalCrSpellsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALCTX2	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalInitCr(hDlg,8,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (GLOBALCTX2 *)GetWindowLong(hDlg,DWL_USER);
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
				if ( ((DRAWITEMSTRUCT *)lParam)->CtlID == 101 )
					{
					Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCrSpellsLst,Context->SpellsImages);
					return(TRUE);
					}
				if ( ((DRAWITEMSTRUCT *)lParam)->CtlID == 123 )
					{
					Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,GCreatures,Context->CreaturesImages);
					return(TRUE);
					}
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
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 104:
						case 106:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),0);
							if (lData > 1024) Misc_SetDlgItemFloat(hDlg,LOWORD(wParam),1024<<12);
							break;
						}
					return(TRUE);

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							Properties_GlobalSaveChangesCr(hDlg,8,Context);
							Properties_GlobalGetParamsCr(hDlg,8,Context);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 109:
							Context->ShotCtx.ShotType = GShots[SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
							Context->ShotCtx.ShotData1 = Context->Variables.CrSpells[Context->CurrentSpell].ShotData1;
							Context->ShotCtx.ShotData2 = Context->Variables.CrSpells[Context->CurrentSpell].ShotData2;
							if (Properties_GlobalShotOptions(hDlg,&Context->ShotCtx))
								{
								Context->Variables.CrSpells[Context->CurrentSpell].ShotData1 = Context->ShotCtx.ShotData1;
								Context->Variables.CrSpells[Context->CurrentSpell].ShotData2 = Context->ShotCtx.ShotData2;
								}
							break;

						case 998:
							Properties_GlobalReset(&Context->Variables,Context->CurrentSpell,MGRF_CREATURESPELLS);
							Properties_GlobalGetParamsCr(hDlg,8,Context);
							break;
						case 999:
							Properties_GlobalReset(&Context->Variables,-1,MGRF_CREATURESPELLS);
							Properties_GlobalGetParamsCr(hDlg,8,Context);
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
					break;

				case PSN_KILLACTIVE:
					Properties_GlobalSaveChangesCr(hDlg,8,Context);
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
					SetWindowLong(hDlg,DWL_MSGRESULT,(Properties_GlobalSetValuesCr(hDlg,8,Context))?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage & de sélection				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Test si le total des pourcentages est égale à 100 «««««««««««««««»

int Properties_GlobalJobsCheckApply(HWND hDlg, GLOBALCTX2 *Context)
{
	MESSAGEBOXSTRUCT	 Request;
	LONG			 Total;
	char			*Text;

	Text = szGlobalJobHappy;
	Total = Properties_GlobalJobsGetTotal(JOB_HAPPY,Context);
	if ((Total != 100)&&(Total != 0)) goto Error;
	Text = szGlobalJobUnhappy;
	Total = Properties_GlobalJobsGetTotal(JOB_UNHAPPY,Context);
	if ((Total != 100)&&(Total != 0)) goto Error;
	Text = szGlobalJobAngry;
	Total = Properties_GlobalJobsGetTotal(JOB_ANGRY,Context);
	if ((Total != 100)&&(Total != 0)) goto Error;
	return(1);

Error:	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	wsprintf(Context->TempText,szGlobalJobError,Text,Total);
	Request.gadgets		= szGlobalReplies;
	Request.windowtext	= Context->TempText;
	Request.windowtitle	= NULL;
	Request.requestbits	= &Config.RequestsFlags;
	Request.statebit	= REQUEST_GLOBALJOB;
	Request.flags		= MBF_AUTOREPLY;
	Request.defaultgadget	= 1;
	Request.defaulticon	= MB_ICONEXCLAMATION;
	Request.windowpos	= MBP_CENTER;
	return(Misc_MessageBoxExtArgs(hDlg,&Request));
}


// «»»» Calcul le pourcentage d'occupations utilisé «««««««««««««««««««««»

long Properties_GlobalJobsGetTotal(UINT JobType, GLOBALCTX2 *Context)
{
	long	Total = 0;
	int	i;

	switch(JobType)
		{
		case JOB_HAPPY:
			for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
				if (Context->HappyJobs[i].Job) Total += Context->HappyJobs[i].JobChance;
			break;
		case JOB_UNHAPPY:
			for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
				if (Context->UnhappyJobs[i].Job) Total += Context->UnhappyJobs[i].JobChance;
			break;
		case JOB_ANGRY:
			for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
				if (Context->AngryJobs[i].Job) Total += Context->AngryJobs[i].JobChance;
			break;
		}

	return(Total);
}


// «»»» Ajoute une occupation «««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalAddJob(HWND hDlg, UINT JobType, GLOBALCTX2 *Context)
{
	GLOBALJOB	*JobPtr = NULL;
	UINT		 CbxId = 0;
	UINT		 ChanceId = 0;
	UINT		 MoodId = 0;
	UINT		 ManaId = 0;
	int		 i;

	switch(JobType)
		{
		case JOB_HAPPY:
			CbxId    = 103;
			ChanceId = 105;
			MoodId   = 107;
			ManaId   = 109;
			for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
				if (!Context->HappyJobs[i].Job)
					{
					JobPtr = &Context->HappyJobs[i];
					break;
					}
			break;
		case JOB_UNHAPPY:
			CbxId    = 123;
			ChanceId = 125;
			MoodId   = 127;
			ManaId   = 129;
			for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
				if (!Context->UnhappyJobs[i].Job)
					{
					JobPtr = &Context->UnhappyJobs[i];
					break;
					}
			break;
		case JOB_ANGRY:
			CbxId    = 143;
			ChanceId = 145;
			MoodId   = 147;
			ManaId   = 149;
			for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
				if (!Context->AngryJobs[i].Job)
					{
					JobPtr = &Context->AngryJobs[i];
					break;
					}
			break;
		case JOB_HATE:
			CbxId    = 163;
			ChanceId = 165;
			MoodId   = 167;
			ManaId   = 169;
			for (i = 0; i != GLOBAL_HATEJOBS; i++)
				if (!Context->HateJobs[i].Job)
					{
					JobPtr = &Context->HateJobs[i];
					break;
					}
			break;
		}

	if (!JobPtr) return;

	JobPtr->Job = SendDlgItemMessage(hDlg,CbxId,CB_GETITEMDATA,(WPARAM)SendDlgItemMessage(hDlg,CbxId,CB_GETCURSEL,(WPARAM)0,(LPARAM)0),(LPARAM)0);
	if (!JobPtr->HasNoData)
		{
		JobPtr->JobChance = GetDlgItemInt(hDlg,ChanceId,NULL,FALSE);
		JobPtr->JobMood = GetDlgItemInt(hDlg,MoodId,NULL,TRUE);
		JobPtr->JobMana = GetDlgItemInt(hDlg,ManaId,NULL,TRUE);
		}
	InvalidateRect(GetDlgItem(hDlg,JobPtr->CtrlId),NULL,FALSE);
	Properties_GlobalUpdateJobs(hDlg,JobType,Context);
	return;
}


// «»»» Supprime une ou plusieurs occupations «««««««««««««««««««««««««««»

void Properties_GlobalRemJobs(HWND hDlg, UINT JobType, GLOBALCTX2 *Context)
{
	int	i,j;

	switch(JobType)
		{
		case JOB_HAPPY:
			for (i = 0; i != GLOBAL_HAPPYJOBS;)
				{
				if (Context->HappyJobs[i].Selected)
					{
					Context->HappyJobs[i].Selected = FALSE;
					Context->HappyJobs[i].Job = 0;
					Context->HappyJobs[i].JobChance = 0;
					Context->HappyJobs[i].JobMood = 0;
					Context->HappyJobs[i].JobMana = 0;
					for (j = i; j != GLOBAL_HAPPYJOBS; j++)
						{
						Context->HappyJobs[j].Selected = Context->HappyJobs[j+1].Selected;
						Context->HappyJobs[j].Job = Context->HappyJobs[j+1].Job;
						Context->HappyJobs[j].JobChance = Context->HappyJobs[j+1].JobChance;
						Context->HappyJobs[j].JobMood = Context->HappyJobs[j+1].JobMood;
						Context->HappyJobs[j].JobMana = Context->HappyJobs[j+1].JobMana;
						}
					i = 0;
					continue;
					}
				i++;
				}
			for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
				InvalidateRect(GetDlgItem(hDlg,Context->HappyJobs[i].CtrlId),NULL,FALSE);

			EnableWindow(GetDlgItem(hDlg,111),FALSE);
			break;

		case JOB_UNHAPPY:
			for (i = 0; i != GLOBAL_UNHAPPYJOBS;)
				{
				if (Context->UnhappyJobs[i].Selected)
					{
					Context->UnhappyJobs[i].Selected = FALSE;
					Context->UnhappyJobs[i].Job = 0;
					Context->UnhappyJobs[i].JobChance = 0;
					Context->UnhappyJobs[i].JobMood = 0;
					Context->UnhappyJobs[i].JobMana = 0;
					for (j = i; j != GLOBAL_UNHAPPYJOBS; j++)
						{
						Context->UnhappyJobs[j].Selected = Context->UnhappyJobs[j+1].Selected;
						Context->UnhappyJobs[j].Job = Context->UnhappyJobs[j+1].Job;
						Context->UnhappyJobs[j].JobChance = Context->UnhappyJobs[j+1].JobChance;
						Context->UnhappyJobs[j].JobMood = Context->UnhappyJobs[j+1].JobMood;
						Context->UnhappyJobs[j].JobMana = Context->UnhappyJobs[j+1].JobMana;
						}
					i = 0;
					continue;
					}
				i++;
				}
			for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
				InvalidateRect(GetDlgItem(hDlg,Context->UnhappyJobs[i].CtrlId),NULL,FALSE);

			EnableWindow(GetDlgItem(hDlg,131),FALSE);
			break;

		case JOB_ANGRY:
			for (i = 0; i != GLOBAL_ANGRYJOBS;)
				{
				if (Context->AngryJobs[i].Selected)
					{
					Context->AngryJobs[i].Selected = FALSE;
					Context->AngryJobs[i].Job = 0;
					Context->AngryJobs[i].JobChance = 0;
					Context->AngryJobs[i].JobMood = 0;
					Context->AngryJobs[i].JobMana = 0;
					for (j = i; j != GLOBAL_ANGRYJOBS; j++)
						{
						Context->AngryJobs[j].Selected = Context->AngryJobs[j+1].Selected;
						Context->AngryJobs[j].Job = Context->AngryJobs[j+1].Job;
						Context->AngryJobs[j].JobChance = Context->AngryJobs[j+1].JobChance;
						Context->AngryJobs[j].JobMood = Context->AngryJobs[j+1].JobMood;
						Context->AngryJobs[j].JobMana = Context->AngryJobs[j+1].JobMana;
						}
					i = 0;
					continue;
					}
				i++;
				}
			for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
				InvalidateRect(GetDlgItem(hDlg,Context->AngryJobs[i].CtrlId),NULL,FALSE);

			EnableWindow(GetDlgItem(hDlg,151),FALSE);
			break;

		case JOB_HATE:
			for (i = 0; i != GLOBAL_HATEJOBS;)
				{
				if (Context->HateJobs[i].Selected)
					{
					Context->HateJobs[i].Selected = FALSE;
					Context->HateJobs[i].Job = 0;
					Context->HateJobs[i].JobChance = 0;
					Context->HateJobs[i].JobMood = 0;
					Context->HateJobs[i].JobMana = 0;
					for (j = i; j != GLOBAL_HATEJOBS; j++)
						{
						Context->HateJobs[j].Selected = Context->HateJobs[j+1].Selected;
						Context->HateJobs[j].Job = Context->HateJobs[j+1].Job;
						Context->HateJobs[j].JobChance = Context->HateJobs[j+1].JobChance;
						Context->HateJobs[j].JobMood = Context->HateJobs[j+1].JobMood;
						Context->HateJobs[j].JobMana = Context->HateJobs[j+1].JobMana;
						}
					i = 0;
					continue;
					}
				i++;
				}
			for (i = 0; i != GLOBAL_HATEJOBS; i++)
				InvalidateRect(GetDlgItem(hDlg,Context->HateJobs[i].CtrlId),NULL,FALSE);

			EnableWindow(GetDlgItem(hDlg,171),FALSE);
			break;
		}

	Properties_GlobalUpdateJobs(hDlg,JobType,Context);
	return;
}


// «»»» Mise à jour des occupations disponibles «««««««««««««««««««««««««»

void Properties_GlobalUpdateJobs(HWND hDlg, UINT JobType, GLOBALCTX2 *Context)
{
	BOOL		Filter;
	UINT		CbxId;
	long		Count,Max;
	int		i,j,k;

	switch(JobType)
		{
		case JOB_HAPPY:
			CbxId = 103;
			break;
		case JOB_UNHAPPY:
			CbxId = 123;
			break;
		case JOB_ANGRY:
			CbxId = 143;
			break;
		case JOB_HATE:
			CbxId = 163;
			break;
		}
	SendDlgItemMessage(hDlg,CbxId,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0);

	for (i = 0, k = 0; GCrJobs[i].Id != 0; i++)
		{
		switch(JobType)
			{
			case JOB_HAPPY:
				if (!GCrJobs[i].MinWidth) continue;
				for (Filter = FALSE, j = 0; j != GLOBAL_HAPPYJOBS; j++)
					if (Context->HappyJobs[j].Job == GCrJobs[i].Id)
						{
						Filter = TRUE;
						break;
						}
				if (Filter) continue;
				break;
			case JOB_UNHAPPY:
				if (!GCrJobs[i].MinHeight) continue;
				for (Filter = FALSE, j = 0; j != GLOBAL_UNHAPPYJOBS; j++)
					if (Context->UnhappyJobs[j].Job == GCrJobs[i].Id)
						{
						Filter = TRUE;
						break;
						}
				if (Filter) continue;
				break;
			case JOB_ANGRY:
				if (!GCrJobs[i].MaxWidth) continue;
				for (Filter = FALSE, j = 0; j != GLOBAL_ANGRYJOBS; j++)
					if (Context->AngryJobs[j].Job == GCrJobs[i].Id)
						{
						Filter = TRUE;
						break;
						}
				if (Filter) continue;
				break;
			case JOB_HATE:
				if (!GCrJobs[i].MaxHeight) continue;
				for (Filter = FALSE, j = 0; j != GLOBAL_HATEJOBS; j++)
					if (Context->HateJobs[j].Job == GCrJobs[i].Id)
						{
						Filter = TRUE;
						break;
						}
				if (Filter) continue;
				break;
			}
		SendDlgItemMessage(hDlg,CbxId,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrJobs[i].Name);
		SendDlgItemMessage(hDlg,CbxId,CB_SETITEMDATA,(WPARAM)k++,(LPARAM)GCrJobs[i].Id);
		}
	SendDlgItemMessage(hDlg,CbxId,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);

	switch(JobType)
		{
		case JOB_HAPPY:
			Count = 0;
			Max = GLOBAL_HAPPYJOBS;
			CbxId = 110;
			for (i = 0; i != Max; i++)
				if (Context->HappyJobs[i].Job) Count++;
			break;
		case JOB_UNHAPPY:
			Count = 0;
			Max = GLOBAL_UNHAPPYJOBS;
			CbxId = 130;
			for (i = 0; i != Max; i++)
				if (Context->UnhappyJobs[i].Job) Count++;
			break;
		case JOB_ANGRY:
			Count = 0;
			Max = GLOBAL_ANGRYJOBS;
			CbxId = 150;
			for (i = 0; i != Max; i++)
				if (Context->AngryJobs[i].Job) Count++;
			break;
		case JOB_HATE:
			Count = 0;
			Max = GLOBAL_HATEJOBS;
			CbxId = 170;
			for (i = 0; i != Max; i++)
				if (Context->HateJobs[i].Job) Count++;
			break;
		}

	EnableWindow(GetDlgItem(hDlg,CbxId),(Count == Max)?FALSE:TRUE);
	return;
}


// «»»» (dé)Sélection des occupations «««««««««««««««««««««««««««««««««««»

void Properties_GlobalToggleJob(HWND hDlg, UINT CtrlId, GLOBALCTX2 *Context)
{
	GLOBALJOB	*JobPtr;
	BOOL		 RemOk;
	UINT		 RemId;
	int		 JobType;
	int		 i;

	for (JobType = JOB_HAPPY, i = 0; i != GLOBAL_HAPPYJOBS; i++)
		if (Context->HappyJobs[i].CtrlId == CtrlId)
			{
			JobPtr = &Context->HappyJobs[i];
			goto Toggle;
			}

	for (JobType = JOB_UNHAPPY, i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
		if (Context->UnhappyJobs[i].CtrlId == CtrlId)
			{
			JobPtr = &Context->UnhappyJobs[i];
			goto Toggle;
			}

	for (JobType = JOB_ANGRY, i = 0; i != GLOBAL_ANGRYJOBS; i++)
		if (Context->AngryJobs[i].CtrlId == CtrlId)
			{
			JobPtr = &Context->AngryJobs[i];
			goto Toggle;
			}

	for (JobType = JOB_HATE, i = 0; i != GLOBAL_HATEJOBS; i++)
		if (Context->HateJobs[i].CtrlId == CtrlId)
			{
			JobPtr = &Context->HateJobs[i];
			goto Toggle;
			}
	return;

Toggle:	if (JobPtr->Job)
		{
		JobPtr->Selected = (JobPtr->Selected)?FALSE:TRUE;
		InvalidateRect(GetDlgItem(hDlg,JobPtr->CtrlId),NULL,FALSE);
		}
	else
		{
		MessageBeep(MB_OK);
		return;
		}

	RemOk = FALSE;

	if (JobType == JOB_HAPPY)
		{
		RemId = 111;
		for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
			if (Context->HappyJobs[i].Selected)
				{
				RemOk = TRUE;
				break;
				}
		}
	else if (JobType == JOB_UNHAPPY)
		{
		RemId = 131;
		for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
			if (Context->UnhappyJobs[i].Selected)
				{
				RemOk = TRUE;
				break;
				}
		}
	else if (JobType == JOB_ANGRY)
		{
		RemId = 151;
		for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
			if (Context->AngryJobs[i].Selected)
				{
				RemOk = TRUE;
				break;
				}
		}
	else
		{
		RemId = 171;
		for (i = 0; i != GLOBAL_HATEJOBS; i++)
			if (Context->HateJobs[i].Selected)
				{
				RemOk = TRUE;
				break;
				}
		}

	EnableWindow(GetDlgItem(hDlg,RemId),RemOk);
	return;
}


// «»»» Affichage des occupations «««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalDrawJob(DRAWITEMSTRUCT *ItemStruct, GLOBALCTX2 *Context)
{
	GLOBALJOB	*JobPtr;
	UINT		 State;
	int		 TextColor;
	int		 BackColor;
	int		 i;

	for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
		if (Context->HappyJobs[i].CtrlId == ItemStruct->CtlID)
			{
			JobPtr = &Context->HappyJobs[i];
			goto Draw;
			}

	for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
		if (Context->UnhappyJobs[i].CtrlId == ItemStruct->CtlID)
			{
			JobPtr = &Context->UnhappyJobs[i];
			goto Draw;
			}

	for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
		if (Context->AngryJobs[i].CtrlId == ItemStruct->CtlID)
			{
			JobPtr = &Context->AngryJobs[i];
			goto Draw;
			}

	for (i = 0; i != GLOBAL_HATEJOBS; i++)
		if (Context->HateJobs[i].CtrlId == ItemStruct->CtlID)
			{
			JobPtr = &Context->HateJobs[i];
			goto Draw;
			}

	return;

Draw:	if (JobPtr->Selected)
		{
		State = BDR_SUNKENOUTER;
		BackColor = COLOR_HIGHLIGHT;
		TextColor = COLOR_HIGHLIGHTTEXT;
		}
	else
		{
		State = BDR_RAISEDINNER;
		BackColor = COLOR_MENU;
		TextColor = COLOR_MENUTEXT;
		}
	FillRect(ItemStruct->hDC,&ItemStruct->rcItem,(HBRUSH)(BackColor+1));
	DrawEdge(ItemStruct->hDC,&ItemStruct->rcItem,State,BF_RECT);

	if (JobPtr->Job)
		{
		RECT		 WorkRect;
		COLORREF	 OldTextColor;
		COLORREF	 OldBackColor;
		char		*JobText;

		for (i = 0; GCrJobs[i].Id != 0; i++)
			if (GCrJobs[i].Id == JobPtr->Job)
				{
				JobText = GCrJobs[i].Name;
				break;
				}
		if (!GCrJobs[i].Id)
			JobText = szUnknown;

		WorkRect.left = ItemStruct->rcItem.left+4;
		WorkRect.top = ItemStruct->rcItem.top+2;
		WorkRect.bottom = ItemStruct->rcItem.bottom-2;

		if (JobPtr->HasNoData)
				WorkRect.right = ItemStruct->rcItem.right-4;
			else
				WorkRect.right = ItemStruct->rcItem.left+4+(ItemStruct->rcItem.right-ItemStruct->rcItem.left)/2;
		OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(TextColor));
		OldBackColor = SetBkColor(ItemStruct->hDC,GetSysColor(BackColor));
		DrawText(ItemStruct->hDC,JobText,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

		if (!JobPtr->HasNoData)
			{
			wsprintf(Context->TempText,szGlobalJob,JobPtr->JobChance,JobPtr->JobMood,JobPtr->JobMana);
			WorkRect.left = ItemStruct->rcItem.left+4+(ItemStruct->rcItem.right-ItemStruct->rcItem.left)/2;
			WorkRect.right = ItemStruct->rcItem.right-4;
			DrawText(ItemStruct->hDC,Context->TempText,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
			}

		SetBkColor(ItemStruct->hDC,OldBackColor);
		SetTextColor(ItemStruct->hDC,OldTextColor);
		}

	return;
}


// «»»» Changement de résistance ««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalChangeRes(HWND hDlg, DWORD Flags)
{
	UINT	Type;
	int	i;

	if (Flags&1)
		{
		Type = SendDlgItemMessage(hDlg,103,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		for (i = 104; i != 107+1; i++) EnableWindow(GetDlgItem(hDlg,i),(Type)?TRUE:FALSE);
		}

	if (Flags&2)
		{
		Type = SendDlgItemMessage(hDlg,113,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		for (i = 114; i != 117+1; i++) EnableWindow(GetDlgItem(hDlg,i),(Type)?TRUE:FALSE);
		}

	if (Flags&4)
		{
		Type = SendDlgItemMessage(hDlg,123,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		for (i = 124; i != 127+1; i++) EnableWindow(GetDlgItem(hDlg,i),(Type)?TRUE:FALSE);
		}

	if (Flags&8)
		{
		Type = SendDlgItemMessage(hDlg,133,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		for (i = 134; i != 137+1; i++) EnableWindow(GetDlgItem(hDlg,i),(Type)?TRUE:FALSE);
		}

	return;
}


// «»»» Changement d'un tir «««««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalChangeShots(HWND hDlg, DWORD Flags)
{
	UINT	Type;
	int	i;

	if (Flags&1)
		{
		Type = SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (!Type)
			{
			for (i = 105; i != 114+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			for (i = 206; i != 214+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			}
		else
			{
			for (i = 105; i != 114+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			for (i = 206; i != 214+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			}
		}


	if (Flags&2)
		{
		Type = SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (!Type)
			{
			for (i = 117; i != 126+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			for (i = 218; i != 226+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			}
		else
			{
			for (i = 117; i != 126+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			for (i = 218; i != 226+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			}
		}

	if (Flags&4)
		{
		Type = SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (!Type)
			{
			for (i = 129; i != 138+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			for (i = 230; i != 238+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),FALSE);
			}
		else
			{
			for (i = 129; i != 138+1; i++)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			for (i = 230; i != 238+2; i += 2)
				EnableWindow(GetDlgItem(hDlg,i),TRUE);
			}
		}

	return;
}


// «»»» Affiche ce qui attire la créature «««««««««««««««««««««««««««««««»

void Properties_GlobalSetAttractions(HWND hDlg, DWORD Flags, GLOBALCTX2 *Context)
{
	UINT	Type;
	int	i;

	if (Flags&1)
		{
		SendDlgItemMessage(hDlg,125,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
		Type = SendDlgItemMessage(hDlg,124,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (!Type)
			{
			EnableWindow(GetDlgItem(hDlg,125),FALSE);
			SetDlgItemText(hDlg,135,NULL);
			}
		else
			{
			for (i = 0; GARooms[i].Id != 0; i++) SendDlgItemMessage(hDlg,125,CB_ADDSTRING,(WPARAM)0,(LPARAM)GARooms[i].Name);
			EnableWindow(GetDlgItem(hDlg,125),TRUE);
			for (i = 0; GARooms[i].Id != 0; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Attract1ID == GARooms[i].Id)
					{
					SendDlgItemMessage(hDlg,125,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}
			if (!GARooms[i].Id) SendDlgItemMessage(hDlg,125,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			SetDlgItemText(hDlg,135,szGlobalRoomType);
			}
		}

	if (Flags&2)
		{
		SendDlgItemMessage(hDlg,128,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
		SetDlgItemText(hDlg,129,NULL);
		Type = SendDlgItemMessage(hDlg,127,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (Type == 1)
			{
			for (i = 0; GARooms[i].Id != 0; i++) SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)GARooms[i].Name);
			EnableWindow(GetDlgItem(hDlg,128),TRUE);
			EnableWindow(GetDlgItem(hDlg,129),TRUE);
			EnableWindow(GetDlgItem(hDlg,229),TRUE);
			for (i = 0; GARooms[i].Id != 0; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Attract2ID == GARooms[i].Id)
					{
					SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}
			if (!GARooms[i].Id) SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			SetDlgItemInt(hDlg,129,Context->Variables.Creatures[Context->CurrentCreature].Attract2Data,FALSE);
			SetDlgItemText(hDlg,138,szGlobalRoomType);
			SetDlgItemText(hDlg,139,szGlobalRoomSquares);
			}
		else if (Type == 4)
			{
			for (i = 0; GCreatures[i].Id != 0; i++) SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Name);
			EnableWindow(GetDlgItem(hDlg,128),TRUE);
			EnableWindow(GetDlgItem(hDlg,129),TRUE);
			EnableWindow(GetDlgItem(hDlg,229),TRUE);
			for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Attract2ID == GCreatures[i].Id)
					{
					SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}
			if (!GCreatures[i].Id) SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			SetDlgItemInt(hDlg,129,Context->Variables.Creatures[Context->CurrentCreature].Attract2Data,FALSE);
			SetDlgItemText(hDlg,138,szGlobalCreatureType);
			SetDlgItemText(hDlg,139,szGlobalCreaturesCount);
			}
		else if (!Type)
			{
			EnableWindow(GetDlgItem(hDlg,128),FALSE);
			EnableWindow(GetDlgItem(hDlg,129),FALSE);
			EnableWindow(GetDlgItem(hDlg,229),FALSE);
			SetDlgItemText(hDlg,138,NULL);
			SetDlgItemText(hDlg,139,NULL);
			}
		else
			{
			SetDlgItemText(hDlg,138,NULL);
			EnableWindow(GetDlgItem(hDlg,128),FALSE);
			EnableWindow(GetDlgItem(hDlg,129),TRUE);
			EnableWindow(GetDlgItem(hDlg,229),TRUE);
			SetDlgItemInt(hDlg,129,Context->Variables.Creatures[Context->CurrentCreature].Attract2Data,FALSE);
			if (Type == 2) SetDlgItemText(hDlg,139,szGlobalGoldAmount);
			else if (Type == 3) SetDlgItemText(hDlg,139,szGlobalManaAmount);
			}
		}

	return;
}


// «»»» Changement d'une valeur (Up&Down) «««««««««««««««««««««««««««««««»

void Properties_GlobalModifyValue(HWND hDlg, NM_UPDOWN *UpDown, GLOBALCTX2 *Context)
{
	LV_ITEM		Item;

	if (!SendMessage(Context->CreaturesAttrList,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) return;
	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->CreaturesAttrList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	Item.iSubItem = 0;
	Item.lParam = 0;
	if (Item.iItem == -1) return;
	SendMessage(Context->CreaturesAttrList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;
	if (((GLOBALATTRLIST *)Item.lParam)->Offset == 0xFFFFFFFF) return;

	if (((GLOBALATTRLIST *)Item.lParam)->Float)
		Misc_SetUpDownIntSigned(hDlg,106,UpDown);
	else
		{
		if (((GLOBALATTRLIST *)Item.lParam)->Signed)
			Misc_SetUpDownIntSigned(hDlg,106,UpDown);
		else
			Misc_SetUpDownIntSigned(hDlg,106,UpDown);
		}

	return;
}


// «»»» Stockage d'un attribut ««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalSetNewValue(HWND hDlg, GLOBALCTX2 *Context)
{
	LV_ITEM		Item;
	LONG		Current;

	if (!SendMessage(Context->CreaturesAttrList,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) return;
	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->CreaturesAttrList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	Item.iSubItem = 0;
	Item.lParam = 0;
	if (Item.iItem == -1) return;
	SendMessage(Context->CreaturesAttrList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;
	if (((GLOBALATTRLIST *)Item.lParam)->Offset == 0xFFFFFFFF) return;

	if (((GLOBALATTRLIST *)Item.lParam)->Float)
		{
		Current = GetDlgItemInt(hDlg,106,NULL,TRUE);
		if ((Current < 0)||(Current > 32767))
			{
			if (Current < 0) Current = 0;
			if (Current > 32767) Current = 32767<<12;
			Misc_SetDlgItemFloat(hDlg,106,Current);
			return;
			}
		else
			Current = Misc_GetDlgItemFloat(hDlg,106);
		*(((LONG *)&Context->Variables.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)Item.lParam)->Offset) = Current;
		}
	else
		{
		if (((GLOBALATTRLIST *)Item.lParam)->Signed)
			{
			Current = GetDlgItemInt(hDlg,106,NULL,TRUE);
			if (Current > 32767)
				{
				SetDlgItemInt(hDlg,106,32767,TRUE);
				return;
				}
			}
		else
			{
			Current = GetDlgItemInt(hDlg,106,NULL,TRUE);
			if ((Current < 0)||(Current > 32767))
				{
				if (Current < 0) Current = 0;
				if (Current > 32767) Current = 32767;
				SetDlgItemInt(hDlg,106,Current,TRUE);
				return;
				}
			}
		*(((LONG *)&Context->Variables.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)Item.lParam)->Offset) = Current;
		}

	InvalidateRect(Context->CreaturesAttrList,NULL,FALSE);
	return;
}


// «»»» Change la valeur à éditer «««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalChangeValueType(HWND hDlg, GLOBALCTX2 *Context)
{
	LV_ITEM		Item;
	LONG		Current;
	LONG		Default;
	char		TempValue[64];

	if (!SendMessage(Context->CreaturesAttrList,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0))
		goto None_Selected;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->CreaturesAttrList,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	Item.iSubItem = 0;
	Item.lParam = 0;
	if (Item.iItem == -1) goto None_Selected;
	SendMessage(Context->CreaturesAttrList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) goto None_Selected;
	if (((GLOBALATTRLIST *)Item.lParam)->Offset == 0xFFFFFFFF) goto None_Selected;

	Current = *(((LONG *)&Context->Variables.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)Item.lParam)->Offset);
	Default = *(((LONG *)&Context->DefaultVr.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)Item.lParam)->Offset);

	if (((GLOBALATTRLIST *)Item.lParam)->Float)
		{
		Misc_FloatToText(Default,TempValue);
		SetDlgItemText(hDlg,104,TempValue);
		Misc_FloatToText(Current,TempValue);
		SetDlgItemText(hDlg,106,TempValue);
		}
	else
		{
		if (Default > 0x7FFF) Default = Default|0xFFFF0000;
		SetDlgItemInt(hDlg,104,Default,TRUE);
		if (Current > 0x7FFF) Current = Current|0xFFFF0000;
		SetDlgItemInt(hDlg,106,Current,TRUE);
		}

	EnableWindow(GetDlgItem(hDlg,105),TRUE);
	EnableWindow(GetDlgItem(hDlg,106),TRUE);
	EnableWindow(GetDlgItem(hDlg,206),TRUE);
	return;

//--- Aucune entrée (valide) n'est sélectionnée ---

None_Selected:
	EnableWindow(GetDlgItem(hDlg,105),FALSE);
	EnableWindow(GetDlgItem(hDlg,106),FALSE);
	EnableWindow(GetDlgItem(hDlg,206),FALSE);
	SetDlgItemText(hDlg,104,NULL);
	SetDlgItemText(hDlg,106,NULL);
	return;
}


// «»»» Affichage des attributs «««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalDrawAttributes(DRAWITEMSTRUCT *ItemStruct, GLOBALCTX2 *Context)
{
	HPEN		LinePen,OldPen;
	SIZE		Size;
	COLORREF	OldColor;
	LV_ITEM 	FarItem;
	RECT		WorkRect;
	SIZE		TextSize;
	HFONT		hOldFont;
	BOOL		Selected;
	LONG		Current,Default;
	int		LineY;
	char		TempValue[64];

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	FarItem.mask = LVIF_PARAM;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&FarItem);
	if (!FarItem.lParam) return;
	if (((GLOBALATTRLIST *)FarItem.lParam)->Offset != 0xFFFFFFFF) goto Valid_Entry;

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
	GetTextExtentPoint32(ItemStruct->hDC,((GLOBALATTRLIST *)FarItem.lParam)->Name,strlen(((GLOBALATTRLIST *)FarItem.lParam)->Name),&Size);
	WorkRect.left += 8;
	WorkRect.right = WorkRect.left+Size.cx+4;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_WINDOW+1));
	WorkRect.left += 2;
	DrawText(ItemStruct->hDC,((GLOBALATTRLIST *)FarItem.lParam)->Name,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
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

	//
	// Affichage de la valeur
	//

	WorkRect.left += 2;
	WorkRect.right = WorkRect.left+Font.FontMaxWidth*10;

	Current = *(((LONG *)&Context->Variables.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)FarItem.lParam)->Offset);
	Default = *(((LONG *)&Context->DefaultVr.Creatures[Context->CurrentCreature])+((GLOBALATTRLIST *)FarItem.lParam)->Offset);

	if (((GLOBALATTRLIST *)FarItem.lParam)->Float)
		Misc_FloatToText(Current,TempValue);
	else
		{
		if (Current > 0x7FFF) Current = Current|0xFFFF0000;
		if (Default > 0x7FFF) Default = Default|0xFFFF0000;
		wsprintf(TempValue,szNumber,Current);
		}

	if (Current == Default)
		OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
	else
		{
		OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
		GetTextExtentPoint32(ItemStruct->hDC,"->",2,&Size);
		DrawText(ItemStruct->hDC,"->",-1,&WorkRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX);
		WorkRect.left += Size.cx+2;
		}

	DrawText(ItemStruct->hDC,TempValue,-1,&WorkRect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER|DT_NOPREFIX);
	SetTextColor(ItemStruct->hDC,OldColor);

	//
	// Affichage du texte
	//

	OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
	GetTextExtentPoint32(ItemStruct->hDC,((GLOBALATTRLIST *)FarItem.lParam)->Name,strlen(((GLOBALATTRLIST *)FarItem.lParam)->Name),&TextSize);
	WorkRect.left  = WorkRect.right+8;
	WorkRect.right = ItemStruct->rcItem.right;
	DrawText(ItemStruct->hDC,((GLOBALATTRLIST *)FarItem.lParam)->Name,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
	SetTextColor(ItemStruct->hDC,OldColor);

	SelectObject(ItemStruct->hDC,hOldFont);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines communes aux boîtes de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisations de la boîte de dialogue «««««««««««««««««««««««««»

void Properties_GlobalInitCr(HWND hDlg, UINT Page, PROPSHEETPAGE *PageStruct)
{
	GLOBALCTX2	*Context;
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	int		 i;

	Context = (GLOBALCTX2 *)(PageStruct->lParam);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Page)
		{
		//
		// Attributs des créatures
		//
		case 0:	GetWindowRect(GetDlgItem(hDlg,102),&ClientRect);
			MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
			Context->CreaturesAttrList = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
			if (!Context->CreaturesAttrList) goto Error_0;
			Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = Font.FontMaxWidth*70;
			Column.iSubItem = 0;
			if (SendMessage(Context->CreaturesAttrList,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;
			Item.mask = LVIF_TEXT|LVIF_PARAM;
			Item.iSubItem = 0;
			for (i = 0; GAttribs[i].Offset != 0xFFFFFFFE; i++)
				{
				Item.iItem = i;
				Item.pszText = GAttribs[i].Name;
				Item.lParam = (LPARAM)&GAttribs[i];
				if (SendMessage(Context->CreaturesAttrList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
				}

			for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			Misc_CreateUpDownButton(hDlg,106,206);
			SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,106,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[0] = TRUE;
			break;

		//
		// Paramètres
		//
		case 1:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GDoorsMats[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GFightStyle[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,106,CB_ADDSTRING,(WPARAM)0,(LPARAM)GFightStyle[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GJobClass[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,108,CB_ADDSTRING,(WPARAM)0,(LPARAM)GJobClass[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GBeds[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,110,CB_ADDSTRING,(WPARAM)0,(LPARAM)GBeds[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,118,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,120,CB_ADDSTRING,(WPARAM)0,(LPARAM)Effects[i].Name) < 0)
					goto Error_0;
				}

			if (SendDlgItemMessage(hDlg,124,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType0) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,124,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType1) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType0) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType1) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType2) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType3) < 0) goto Error_0;
			if (SendDlgItemMessage(hDlg,127,CB_ADDSTRING,(WPARAM)0,(LPARAM)szAttractionType4) < 0) goto Error_0;

			Misc_CreateUpDownButton(hDlg,129,229);
			SendDlgItemMessage(hDlg,129,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			Context->PageInitialized[1] = TRUE;
			break;

		//
		// Drapeaux
		//
		case 2:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			CheckDlgButton(hDlg,110,BST_INDETERMINATE);
			CheckDlgButton(hDlg,116,BST_INDETERMINATE);

			Context->PageInitialized[2] = TRUE;
			break;

		//
		// Attaques
		//
		case 3:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GCrSpells[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSpells[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSpells[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,128,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSpells[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GCrMelee[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,144,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrMelee[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateLevelComboBoxList(hDlg,106,1);
			Misc_CreateLevelComboBoxList(hDlg,118,1);
			Misc_CreateLevelComboBoxList(hDlg,130,1);
			SendDlgItemMessage(hDlg,106,CB_INSERTSTRING,(WPARAM)0,(LPARAM)szZero);
			SendDlgItemMessage(hDlg,118,CB_INSERTSTRING,(WPARAM)0,(LPARAM)szZero);
			SendDlgItemMessage(hDlg,130,CB_INSERTSTRING,(WPARAM)0,(LPARAM)szZero);

			Misc_CreateUpDownButton(hDlg,108,208);
			Misc_CreateUpDownButton(hDlg,110,210);
			Misc_CreateUpDownButton(hDlg,112,212);
			Misc_CreateUpDownButton(hDlg,114,214);
			Misc_CreateUpDownButton(hDlg,120,220);
			Misc_CreateUpDownButton(hDlg,122,222);
			Misc_CreateUpDownButton(hDlg,124,224);
			Misc_CreateUpDownButton(hDlg,126,226);
			Misc_CreateUpDownButton(hDlg,132,232);
			Misc_CreateUpDownButton(hDlg,134,234);
			Misc_CreateUpDownButton(hDlg,136,236);
			Misc_CreateUpDownButton(hDlg,138,238);
			Misc_CreateUpDownButton(hDlg,146,246);
			Misc_CreateUpDownButton(hDlg,148,248);
			Misc_CreateUpDownButton(hDlg,150,250);
			Misc_CreateUpDownButton(hDlg,152,252);
			SendDlgItemMessage(hDlg,108,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,110,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,112,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,114,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,120,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,122,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,124,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,126,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,132,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,134,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,136,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,138,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,146,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,148,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,150,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,152,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[3] = TRUE;
			break;

		//
		// Résistances
		//
		case 4:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GCrRes[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,103,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrRes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,113,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrRes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,123,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrRes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,133,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrRes[i].Name) < 0)
					goto Error_0;
				}

			SendDlgItemMessage(hDlg,104,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(0,100));
			SendDlgItemMessage(hDlg,114,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(0,100));
			SendDlgItemMessage(hDlg,124,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(0,100));
			SendDlgItemMessage(hDlg,134,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(0,100));

			SendDlgItemMessage(hDlg,104,TBM_SETTICFREQ,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,114,TBM_SETTICFREQ,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,124,TBM_SETTICFREQ,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,134,TBM_SETTICFREQ,(WPARAM)5,(LPARAM)0);

			Context->PageInitialized[4] = TRUE;
			break;

		//
		// Possession
		//
		case 5:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GCrAbilities[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrAbilities[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,107,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrAbilities[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GCrSwipes[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,114,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSwipes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,116,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSwipes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,118,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSwipes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,120,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSwipes[i].Name) < 0)
					goto Error_0;
				if (SendDlgItemMessage(hDlg,122,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSwipes[i].Name) < 0)
					goto Error_0;
				}

			for (i = 0; GCrGammas[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)GCrGammas[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateUpDownButton(hDlg,105,205);
			Misc_CreateUpDownButton(hDlg,108,208);
			Misc_CreateUpDownButton(hDlg,136,236);
			Misc_CreateUpDownButton(hDlg,138,238);
			Misc_CreateUpDownButton(hDlg,140,240);
			SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,108,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
			SendDlgItemMessage(hDlg,136,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,138,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,140,EM_LIMITTEXT,(WPARAM)2,(LPARAM)0);

			Context->PageInitialized[5] = TRUE;
			break;

		//
		// Occupations
		//
		case 6:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			Misc_CreateUpDownButton(hDlg,105,205);
			Misc_CreateUpDownButton(hDlg,107,207);
			Misc_CreateUpDownButton(hDlg,109,209);
			Misc_CreateUpDownButton(hDlg,125,225);
			Misc_CreateUpDownButton(hDlg,127,227);
			Misc_CreateUpDownButton(hDlg,129,229);
			Misc_CreateUpDownButton(hDlg,145,245);
			Misc_CreateUpDownButton(hDlg,147,247);
			Misc_CreateUpDownButton(hDlg,149,249);
			SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,107,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,109,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,125,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,127,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,129,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,145,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
			SendDlgItemMessage(hDlg,147,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,149,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);

			Context->HappyJobs[0].CtrlId = 112;
			Context->HappyJobs[1].CtrlId = 113;
			Context->HappyJobs[2].CtrlId = 114;
			Context->UnhappyJobs[0].CtrlId = 132;
			Context->UnhappyJobs[1].CtrlId = 133;
			Context->AngryJobs[0].CtrlId = 152;
			Context->AngryJobs[1].CtrlId = 153;
			Context->AngryJobs[2].CtrlId = 154;
			Context->HateJobs[0].CtrlId = 172;
			Context->HateJobs[1].CtrlId = 173;
			Context->HateJobs[2].CtrlId = 174;
			Context->HateJobs[3].CtrlId = 175;
			Context->HateJobs[0].HasNoData = TRUE;
			Context->HateJobs[1].HasNoData = TRUE;
			Context->HateJobs[2].HasNoData = TRUE;
			Context->HateJobs[3].HasNoData = TRUE;

			Context->PageInitialized[6] = TRUE;
			break;

		//
		// Lumière
		//
		case 7:	for (i = 0; GCreatures[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			Misc_CreateUpDownButton(hDlg,104,204);
			Misc_CreateUpDownButton(hDlg,106,206);
			Misc_CreateUpDownButton(hDlg,108,208);
			Misc_CreateUpDownButton(hDlg,114,214);
			SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,106,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,108,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,114,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[7] = TRUE;
			break;

		//
		// Sorts
		//
		case 8:	for (i = 0; GCrSpellsLst[i].Id != 0; i++)
				{
				if (SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCrSpellsLst[i].Id) < 0)
					goto Error_0;
				}
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				{
				if (SendDlgItemMessage(hDlg,108,CB_ADDSTRING,(WPARAM)0,(LPARAM)GShots[i].Name) < 0)
					goto Error_0;
				}

			Misc_CreateUpDownButton(hDlg,104,204);
			Misc_CreateUpDownButton(hDlg,106,206);
			SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);
			SendDlgItemMessage(hDlg,106,EM_LIMITTEXT,(WPARAM)16,(LPARAM)0);

			Context->PageInitialized[8] = TRUE;
			break;
		}

	return;

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONEXCLAMATION);
	return;
}


// «»»» Place les valeurs «««««««««««««««««««««««««««««««««««««««««««««««»

int Properties_GlobalSetValuesCr(HWND hDlg, UINT Page, GLOBALCTX2 *Context)
{
	if (Page != 8)
		SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)Context->CurrentCreature,(LPARAM)0);

	switch(Page)
		{
		//
		// Attributs des créatures
		//
		case 0:	if (!Context->PageInitialized[0]) return(0);
			Properties_GlobalGetParamsCr(hDlg,0,Context);
			return(1);

		//
		// Paramètres
		//
		case 1:	if (!Context->PageInitialized[1]) return(0);
			Properties_GlobalGetParamsCr(hDlg,1,Context);
			return(1);

		//
		// Drapeaux
		//
		case 2:	if (!Context->PageInitialized[2]) return(0);
			Properties_GlobalGetParamsCr(hDlg,2,Context);
			return(1);

		//
		// Attaques
		//
		case 3:	if (!Context->PageInitialized[3]) return(0);
			Properties_GlobalGetParamsCr(hDlg,3,Context);
			return(1);

		//
		// Résistances
		//
		case 4:	if (!Context->PageInitialized[4]) return(0);
			Properties_GlobalGetParamsCr(hDlg,4,Context);
			return(1);

		//
		// Possession
		//
		case 5:	if (!Context->PageInitialized[5]) return(0);
			Properties_GlobalGetParamsCr(hDlg,5,Context);
			return(1);

		//
		// Occupations
		//
		case 6:	if (!Context->PageInitialized[6]) return(0);
			Properties_GlobalGetParamsCr(hDlg,6,Context);
			return(1);

		//
		// Lumière
		//
		case 7:	if (!Context->PageInitialized[7]) return(0);
			Properties_GlobalGetParamsCr(hDlg,7,Context);
			return(1);

		//
		// Sorts
		//
		case 8:	if (!Context->PageInitialized[8]) return(0);
			Properties_GlobalGetParamsCr(hDlg,8,Context);
			return(1);
		}

	return(0);
}


// «»»» Sauvegarde les valeurs ««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalSaveChangesCr(HWND hDlg, UINT Page, GLOBALCTX2 *Context)
{
	int	i;

	switch(Page)
		{
		//
		// Attributs des créatures
		//
		case 0:	break;

		//
		// Paramètres
		//
		case 1:	Context->Variables.Creatures[Context->CurrentCreature].ArmourType = GDoorsMats[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].FightStyle = GFightStyle[SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].JobClass = GJobClass[SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].LairObject = GBeds[SendDlgItemMessage(hDlg,110,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].SlapEffect = Effects[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].HealthEffect = Effects[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].EntranceEffect = Effects[SendDlgItemMessage(hDlg,118,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].DeathEffect = Effects[SendDlgItemMessage(hDlg,120,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Attract1Type = SendDlgItemMessage(hDlg,124,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Attract2Type = SendDlgItemMessage(hDlg,127,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			switch(Context->Variables.Creatures[Context->CurrentCreature].Attract1Type)
				{
				case 0:	Context->Variables.Creatures[Context->CurrentCreature].Attract1ID = 0;
					break;
				}
			switch(Context->Variables.Creatures[Context->CurrentCreature].Attract2Type)
				{
				case 0:	Context->Variables.Creatures[Context->CurrentCreature].Attract2ID = 0;
					Context->Variables.Creatures[Context->CurrentCreature].Attract2Data = 0;
					break;
				case 2:
				case 3:	Context->Variables.Creatures[Context->CurrentCreature].Attract2ID = 0;
				case 1:
				case 4:	Context->Variables.Creatures[Context->CurrentCreature].Attract2Data = GetDlgItemInt(hDlg,129,NULL,FALSE);
					break;
				}
			break;

		//
		// Drapeaux
		//
		case 2:	Context->Variables.Creatures[Context->CurrentCreature].DoNotTrain	= (IsDlgButtonChecked(hDlg,102) == BST_CHECKED)?FALSE:TRUE;
			Context->Variables.Creatures[Context->CurrentCreature].CanBePicked	= (IsDlgButtonChecked(hDlg,103) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].CanBeSlapped	= (IsDlgButtonChecked(hDlg,104) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].CanWalkWater	= (IsDlgButtonChecked(hDlg,105) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].CanWalkLava	= (IsDlgButtonChecked(hDlg,106) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].CanBeHypnotized	= (IsDlgButtonChecked(hDlg,107) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].ImmuneTurncoat	= (IsDlgButtonChecked(hDlg,108) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].ImmuneChicken	= (IsDlgButtonChecked(hDlg,109) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].DeadBody		= (IsDlgButtonChecked(hDlg,111) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].NoEnemies	= (IsDlgButtonChecked(hDlg,112) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].FreeFriends	= (IsDlgButtonChecked(hDlg,113) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].RevealTraps	= (IsDlgButtonChecked(hDlg,114) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].Fearless		= (IsDlgButtonChecked(hDlg,115) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].RandomInvader	= (IsDlgButtonChecked(hDlg,117) == BST_CHECKED)?TRUE:FALSE;
			break;

		//
		// Attaques
		//
		case 3:	Context->Variables.Creatures[Context->CurrentCreature].Shot1ID		= GCrSpells[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Shot2ID		= GCrSpells[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Shot3ID		= GCrSpells[SendDlgItemMessage(hDlg,128,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Shot1Level	= SendDlgItemMessage(hDlg,106,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Shot1Delay	= Misc_GetDlgItemFloat(hDlg,108);
			Context->Variables.Creatures[Context->CurrentCreature].Shot1X		= Misc_GetDlgItemFloatSigned(hDlg,110);
			Context->Variables.Creatures[Context->CurrentCreature].Shot1Y		= Misc_GetDlgItemFloatSigned(hDlg,112);
			Context->Variables.Creatures[Context->CurrentCreature].Shot1Z		= Misc_GetDlgItemFloatSigned(hDlg,114);
			Context->Variables.Creatures[Context->CurrentCreature].Shot2Level	= SendDlgItemMessage(hDlg,118,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Shot2Delay	= Misc_GetDlgItemFloat(hDlg,120);
			Context->Variables.Creatures[Context->CurrentCreature].Shot2X		= Misc_GetDlgItemFloatSigned(hDlg,122);
			Context->Variables.Creatures[Context->CurrentCreature].Shot2Y		= Misc_GetDlgItemFloatSigned(hDlg,124);
			Context->Variables.Creatures[Context->CurrentCreature].Shot2Z		= Misc_GetDlgItemFloatSigned(hDlg,126);
			Context->Variables.Creatures[Context->CurrentCreature].Shot3Level	= SendDlgItemMessage(hDlg,130,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Shot3Delay	= Misc_GetDlgItemFloat(hDlg,132);
			Context->Variables.Creatures[Context->CurrentCreature].Shot3X		= Misc_GetDlgItemFloatSigned(hDlg,134);
			Context->Variables.Creatures[Context->CurrentCreature].Shot3Y		= Misc_GetDlgItemFloatSigned(hDlg,136);
			Context->Variables.Creatures[Context->CurrentCreature].Shot3Z		= Misc_GetDlgItemFloatSigned(hDlg,138);
			Context->Variables.Creatures[Context->CurrentCreature].MeleeID		= GCrMelee[SendDlgItemMessage(hDlg,144,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].RechargeTime	= Misc_GetDlgItemFloat(hDlg,146);
			Context->Variables.Creatures[Context->CurrentCreature].Range		= Misc_GetDlgItemFloat(hDlg,148);
			Context->Variables.Creatures[Context->CurrentCreature].Dammage		= GetDlgItemInt(hDlg,150,NULL,FALSE);
			Context->Variables.Creatures[Context->CurrentCreature].Delay		= Misc_GetDlgItemFloat(hDlg,152);
			break;

		//
		// Résistances
		//
		case 4:	Context->Variables.Creatures[Context->CurrentCreature].Res1		= GCrRes[SendDlgItemMessage(hDlg,103,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Res2		= GCrRes[SendDlgItemMessage(hDlg,113,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Res3		= GCrRes[SendDlgItemMessage(hDlg,123,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Res4		= GCrRes[SendDlgItemMessage(hDlg,133,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Res1Rate		= SendDlgItemMessage(hDlg,104,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Res2Rate		= SendDlgItemMessage(hDlg,114,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Res3Rate		= SendDlgItemMessage(hDlg,124,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
			Context->Variables.Creatures[Context->CurrentCreature].Res4Rate		= SendDlgItemMessage(hDlg,134,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
			break;

		//
		// Possession
		//
		case 5:	Context->Variables.Creatures[Context->CurrentCreature].SpecialAb1	= GCrAbilities[SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].SpecialAb2	= GCrAbilities[SendDlgItemMessage(hDlg,107,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Melee1		= GCrSwipes[SendDlgItemMessage(hDlg,114,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Melee2		= GCrSwipes[SendDlgItemMessage(hDlg,116,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Special1		= GCrSwipes[SendDlgItemMessage(hDlg,118,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Special2		= GCrSwipes[SendDlgItemMessage(hDlg,120,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].Spell		= GCrSwipes[SendDlgItemMessage(hDlg,122,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].GammaEffect	= GCrGammas[SendDlgItemMessage(hDlg,134,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.Creatures[Context->CurrentCreature].SpecialAb1Data	= GetDlgItemInt(hDlg,105,NULL,FALSE);
			Context->Variables.Creatures[Context->CurrentCreature].SpecialAb2Data	= GetDlgItemInt(hDlg,108,NULL,FALSE);
			Context->Variables.Creatures[Context->CurrentCreature].WaddleScale	= Misc_GetDlgItemFloat(hDlg,136);
			Context->Variables.Creatures[Context->CurrentCreature].OscillateScale	= Misc_GetDlgItemFloat(hDlg,138);
			Context->Variables.Creatures[Context->CurrentCreature].WalkScale	= GetDlgItemInt(hDlg,140,NULL,FALSE);
			Context->Variables.Creatures[Context->CurrentCreature].CameraRolls	= (IsDlgButtonChecked(hDlg,141) == BST_CHECKED)?TRUE:FALSE;
			break;

		//
		// Occupations
		//
		case 6:	for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
				{
				Context->Variables.Creatures[Context->CurrentCreature].HJobs[i] = Context->HappyJobs[i].Job;
				Context->Variables.Creatures[Context->CurrentCreature].HJobsChance[i] = Context->HappyJobs[i].JobChance;
				Context->Variables.Creatures[Context->CurrentCreature].HJobsMood[i] = Context->HappyJobs[i].JobMood;
				Context->Variables.Creatures[Context->CurrentCreature].HJobsMana[i] = Context->HappyJobs[i].JobMana;
				}
			for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
				{
				Context->Variables.Creatures[Context->CurrentCreature].UJobs[i] = Context->UnhappyJobs[i].Job;
				Context->Variables.Creatures[Context->CurrentCreature].UJobsChance[i] = Context->UnhappyJobs[i].JobChance;
				Context->Variables.Creatures[Context->CurrentCreature].UJobsMood[i] = Context->UnhappyJobs[i].JobMood;
				Context->Variables.Creatures[Context->CurrentCreature].UJobsMana[i] = Context->UnhappyJobs[i].JobMana;
				}
			for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
				{
				Context->Variables.Creatures[Context->CurrentCreature].AJobs[i] = Context->AngryJobs[i].Job;
				Context->Variables.Creatures[Context->CurrentCreature].AJobsChance[i] = Context->AngryJobs[i].JobChance;
				Context->Variables.Creatures[Context->CurrentCreature].AJobsMood[i] = Context->AngryJobs[i].JobMood;
				Context->Variables.Creatures[Context->CurrentCreature].AJobsMana[i] = Context->AngryJobs[i].JobMana;
				}
			for (i = 0; i != GLOBAL_HATEJOBS; i++)
				{
				Context->Variables.Creatures[Context->CurrentCreature].NJobs[i] = Context->HateJobs[i].Job;
				}
			break;

		//
		// Lumière
		//
		case 7:	Context->Variables.Creatures[Context->CurrentCreature].LightX		= Misc_GetDlgItemFloatSigned(hDlg,104);
			Context->Variables.Creatures[Context->CurrentCreature].LightY		= Misc_GetDlgItemFloatSigned(hDlg,106);
			Context->Variables.Creatures[Context->CurrentCreature].LightZ 		= Misc_GetDlgItemFloatSigned(hDlg,108);
			Context->Variables.Creatures[Context->CurrentCreature].LightRadius	= Misc_GetDlgItemFloat(hDlg,114);
			Context->Variables.Creatures[Context->CurrentCreature].LightB		= (BYTE)((Context->LightColor&0x00FF0000)>>16);
			Context->Variables.Creatures[Context->CurrentCreature].LightG		= (BYTE)((Context->LightColor&0x0000FF00)>>8);
			Context->Variables.Creatures[Context->CurrentCreature].LightR		= (BYTE)((Context->LightColor&0x000000FF));
			Context->Variables.Creatures[Context->CurrentCreature].LightFlicker 	= (IsDlgButtonChecked(hDlg,123))?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].LightPulse 	= (IsDlgButtonChecked(hDlg,124))?TRUE:FALSE;
			Context->Variables.Creatures[Context->CurrentCreature].LightPlayer	= (IsDlgButtonChecked(hDlg,125))?TRUE:FALSE;
			break;

		//
		// Sorts
		//
		case 8:	Context->Variables.CrSpells[Context->CurrentSpell].Shot			= GShots[SendDlgItemMessage(hDlg,108,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->Variables.CrSpells[Context->CurrentSpell].RechargeTime		= Misc_GetDlgItemFloat(hDlg,104);
			Context->Variables.CrSpells[Context->CurrentSpell].Range		= Misc_GetDlgItemFloat(hDlg,106);
			Context->Variables.CrSpells[Context->CurrentSpell].Attacking		= (IsDlgButtonChecked(hDlg,113) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.CrSpells[Context->CurrentSpell].Defensive		= (IsDlgButtonChecked(hDlg,114) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.CrSpells[Context->CurrentSpell].Creature		= (IsDlgButtonChecked(hDlg,115) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.CrSpells[Context->CurrentSpell].CastSelf		= (IsDlgButtonChecked(hDlg,116) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.CrSpells[Context->CurrentSpell].CastOthers		= (IsDlgButtonChecked(hDlg,117) == BST_CHECKED)?TRUE:FALSE;
			Context->Variables.CrSpells[Context->CurrentSpell].CastCreatures	= (IsDlgButtonChecked(hDlg,118) == BST_CHECKED)?TRUE:FALSE;
			break;

		}

	return;
}


// «»»» Place les paramètres dans les gadgets «««««««««««««««««««««««««««»

void Properties_GlobalGetParamsCr(HWND hDlg, UINT Page, GLOBALCTX2 *Context)
{
	int	i;

	if (Page != 8)
		Context->CurrentCreature = SendDlgItemMessage(hDlg,101,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);

	switch(Page)
		{
		//
		// Attributs des créatures
		//
		case 0:	InvalidateRect(Context->CreaturesAttrList,NULL,FALSE);
			Properties_GlobalChangeValueType(hDlg,Context);
			break;

		//
		// Paramètres
		//
		case 1:	for (i = 0; GDoorsMats[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].ArmourType == GDoorsMats[i].Id)
					{
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}

			for (i = 0; GFightStyle[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].FightStyle == GFightStyle[i].Id)
					{
					SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}

			for (i = 0; GJobClass[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].JobClass == GJobClass[i].Id)
					{
					SendDlgItemMessage(hDlg,108,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}

			for (i = 0; GBeds[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].LairObject == GBeds[i].Id)
					{
					SendDlgItemMessage(hDlg,110,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}

			for (i = 0; Effects[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].SlapEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].HealthEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].EntranceEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,118,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].DeathEffect == Effects[i].Id)
					SendDlgItemMessage(hDlg,120,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			SendDlgItemMessage(hDlg,124,CB_SETCURSEL,(WPARAM)Context->Variables.Creatures[Context->CurrentCreature].Attract1Type,(LPARAM)0);
			SendDlgItemMessage(hDlg,127,CB_SETCURSEL,(WPARAM)Context->Variables.Creatures[Context->CurrentCreature].Attract2Type,(LPARAM)0);
			Properties_GlobalSetAttractions(hDlg,1|2,Context);
			break;

		//
		// Drapeaux
		//
		case 2:	CheckDlgButton(hDlg,102,(Context->Variables.Creatures[Context->CurrentCreature].DoNotTrain)?BST_UNCHECKED:BST_CHECKED);
			CheckDlgButton(hDlg,103,(Context->Variables.Creatures[Context->CurrentCreature].CanBePicked)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,104,(Context->Variables.Creatures[Context->CurrentCreature].CanBeSlapped)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,105,(Context->Variables.Creatures[Context->CurrentCreature].CanWalkWater)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,106,(Context->Variables.Creatures[Context->CurrentCreature].CanWalkLava)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,107,(Context->Variables.Creatures[Context->CurrentCreature].CanBeHypnotized)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,108,(Context->Variables.Creatures[Context->CurrentCreature].ImmuneTurncoat)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,109,(Context->Variables.Creatures[Context->CurrentCreature].ImmuneChicken)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,111,(Context->Variables.Creatures[Context->CurrentCreature].DeadBody)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,112,(Context->Variables.Creatures[Context->CurrentCreature].NoEnemies)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,113,(Context->Variables.Creatures[Context->CurrentCreature].FreeFriends)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,114,(Context->Variables.Creatures[Context->CurrentCreature].RevealTraps)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,115,(Context->Variables.Creatures[Context->CurrentCreature].Fearless)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,117,(Context->Variables.Creatures[Context->CurrentCreature].RandomInvader)?BST_CHECKED:BST_UNCHECKED);
			break;

		//
		// Attaques
		//
		case 3:	for (i = 0; GCrSpells[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Shot1ID == GCrSpells[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Shot2ID == GCrSpells[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Shot3ID == GCrSpells[i].Id)
					SendDlgItemMessage(hDlg,128,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			if (Context->Variables.Creatures[Context->CurrentCreature].Shot1Level)
				SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)Context->Variables.Creatures[Context->CurrentCreature].Shot1Level,(LPARAM)0);
			else
				SendDlgItemMessage(hDlg,106,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			Misc_SetDlgItemFloat(hDlg,108,Context->Variables.Creatures[Context->CurrentCreature].Shot1Delay);
			Misc_SetDlgItemFloatSigned(hDlg,110,Context->Variables.Creatures[Context->CurrentCreature].Shot1X);
			Misc_SetDlgItemFloatSigned(hDlg,112,Context->Variables.Creatures[Context->CurrentCreature].Shot1Y);
			Misc_SetDlgItemFloatSigned(hDlg,114,Context->Variables.Creatures[Context->CurrentCreature].Shot1Z);
			if (Context->Variables.Creatures[Context->CurrentCreature].Shot2Level)
				SendDlgItemMessage(hDlg,118,CB_SETCURSEL,(WPARAM)Context->Variables.Creatures[Context->CurrentCreature].Shot2Level,(LPARAM)0);
			else
				SendDlgItemMessage(hDlg,118,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			Misc_SetDlgItemFloat(hDlg,120,Context->Variables.Creatures[Context->CurrentCreature].Shot2Delay);
			Misc_SetDlgItemFloatSigned(hDlg,122,Context->Variables.Creatures[Context->CurrentCreature].Shot2X);
			Misc_SetDlgItemFloatSigned(hDlg,124,Context->Variables.Creatures[Context->CurrentCreature].Shot2Y);
			Misc_SetDlgItemFloatSigned(hDlg,126,Context->Variables.Creatures[Context->CurrentCreature].Shot2Z);
			if (Context->Variables.Creatures[Context->CurrentCreature].Shot3Level)
				SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)Context->Variables.Creatures[Context->CurrentCreature].Shot3Level,(LPARAM)0);
			else
				SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			Misc_SetDlgItemFloat(hDlg,132,Context->Variables.Creatures[Context->CurrentCreature].Shot3Delay);
			Misc_SetDlgItemFloatSigned(hDlg,134,Context->Variables.Creatures[Context->CurrentCreature].Shot3X);
			Misc_SetDlgItemFloatSigned(hDlg,136,Context->Variables.Creatures[Context->CurrentCreature].Shot3Y);
			Misc_SetDlgItemFloatSigned(hDlg,138,Context->Variables.Creatures[Context->CurrentCreature].Shot3Z);

			for (i = 0; GCrMelee[i].Id != 0; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].MeleeID == GCrMelee[i].Id)
					{
					SendDlgItemMessage(hDlg,144,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}

			Misc_SetDlgItemFloat(hDlg,146,Context->Variables.Creatures[Context->CurrentCreature].RechargeTime);
			Misc_SetDlgItemFloat(hDlg,148,Context->Variables.Creatures[Context->CurrentCreature].Range);
			SetDlgItemInt(hDlg,150,Context->Variables.Creatures[Context->CurrentCreature].Dammage,FALSE);
			Misc_SetDlgItemFloat(hDlg,152,Context->Variables.Creatures[Context->CurrentCreature].Delay);

			Properties_GlobalChangeShots(hDlg,1|2|4);
			break;

		//
		// Résistances
		//
		case 4:	for (i = 0; GCrRes[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Res1 == GCrRes[i].Id)
					SendDlgItemMessage(hDlg,103,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Res2 == GCrRes[i].Id)
					SendDlgItemMessage(hDlg,113,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Res3 == GCrRes[i].Id)
					SendDlgItemMessage(hDlg,123,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Res4 == GCrRes[i].Id)
					SendDlgItemMessage(hDlg,133,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			SendDlgItemMessage(hDlg,104,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Context->Variables.Creatures[Context->CurrentCreature].Res1Rate);
			SendDlgItemMessage(hDlg,114,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Context->Variables.Creatures[Context->CurrentCreature].Res2Rate);
			SendDlgItemMessage(hDlg,124,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Context->Variables.Creatures[Context->CurrentCreature].Res3Rate);
			SendDlgItemMessage(hDlg,134,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Context->Variables.Creatures[Context->CurrentCreature].Res4Rate);
			SetDlgItemInt(hDlg,107,Context->Variables.Creatures[Context->CurrentCreature].Res1Rate,FALSE);
			SetDlgItemInt(hDlg,117,Context->Variables.Creatures[Context->CurrentCreature].Res2Rate,FALSE);
			SetDlgItemInt(hDlg,127,Context->Variables.Creatures[Context->CurrentCreature].Res3Rate,FALSE);
			SetDlgItemInt(hDlg,137,Context->Variables.Creatures[Context->CurrentCreature].Res4Rate,FALSE);

			Properties_GlobalChangeRes(hDlg,1|2|4|8);
			break;

		//
		// Possession
		//
		case 5:	for (i = 0; GCrAbilities[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].SpecialAb1 == GCrAbilities[i].Id)
					SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].SpecialAb2 == GCrAbilities[i].Id)
					SendDlgItemMessage(hDlg,107,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; GCrSwipes[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].Melee1 == GCrSwipes[i].Id)
					SendDlgItemMessage(hDlg,114,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Melee2 == GCrSwipes[i].Id)
					SendDlgItemMessage(hDlg,116,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Special1 == GCrSwipes[i].Id)
					SendDlgItemMessage(hDlg,118,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Special2 == GCrSwipes[i].Id)
					SendDlgItemMessage(hDlg,120,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				if (Context->Variables.Creatures[Context->CurrentCreature].Spell == GCrSwipes[i].Id)
					SendDlgItemMessage(hDlg,122,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			for (i = 0; GCrGammas[i].Id != 0xFFFFFFFF; i++)
				{
				if (Context->Variables.Creatures[Context->CurrentCreature].GammaEffect == GCrGammas[i].Id)
					SendDlgItemMessage(hDlg,134,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
				}

			SetDlgItemInt(hDlg,105,Context->Variables.Creatures[Context->CurrentCreature].SpecialAb1Data,FALSE);
			SetDlgItemInt(hDlg,108,Context->Variables.Creatures[Context->CurrentCreature].SpecialAb2Data,FALSE);
			Misc_SetDlgItemFloat(hDlg,136,Context->Variables.Creatures[Context->CurrentCreature].WaddleScale);
			Misc_SetDlgItemFloat(hDlg,138,Context->Variables.Creatures[Context->CurrentCreature].OscillateScale);
			SetDlgItemInt(hDlg,140,Context->Variables.Creatures[Context->CurrentCreature].WalkScale,FALSE);
			CheckDlgButton(hDlg,141,(Context->Variables.Creatures[Context->CurrentCreature].CameraRolls)?BST_CHECKED:BST_UNCHECKED);
			break;

		//
		// Occupations
		//
		case 6:	SetDlgItemInt(hDlg,105,0,FALSE);
			SetDlgItemInt(hDlg,107,0,FALSE);
			SetDlgItemInt(hDlg,109,0,FALSE);
			SetDlgItemInt(hDlg,125,0,FALSE);
			SetDlgItemInt(hDlg,127,0,FALSE);
			SetDlgItemInt(hDlg,129,0,FALSE);
			SetDlgItemInt(hDlg,145,0,FALSE);
			SetDlgItemInt(hDlg,147,0,FALSE);
			SetDlgItemInt(hDlg,149,0,FALSE);

			for (i = 0; i != GLOBAL_HAPPYJOBS; i++)
				{
				Context->HappyJobs[i].Selected = FALSE;
				Context->HappyJobs[i].Job = Context->Variables.Creatures[Context->CurrentCreature].HJobs[i];
				Context->HappyJobs[i].JobChance = Context->Variables.Creatures[Context->CurrentCreature].HJobsChance[i];
				Context->HappyJobs[i].JobMood = Context->Variables.Creatures[Context->CurrentCreature].HJobsMood[i];
				Context->HappyJobs[i].JobMana = Context->Variables.Creatures[Context->CurrentCreature].HJobsMana[i];
				InvalidateRect(GetDlgItem(hDlg,Context->HappyJobs[i].CtrlId),NULL,FALSE);
				}
			for (i = 0; i != GLOBAL_UNHAPPYJOBS; i++)
				{
				Context->UnhappyJobs[i].Selected = FALSE;
				Context->UnhappyJobs[i].Job = Context->Variables.Creatures[Context->CurrentCreature].UJobs[i];
				Context->UnhappyJobs[i].JobChance = Context->Variables.Creatures[Context->CurrentCreature].UJobsChance[i];
				Context->UnhappyJobs[i].JobMood = Context->Variables.Creatures[Context->CurrentCreature].UJobsMood[i];
				Context->UnhappyJobs[i].JobMana = Context->Variables.Creatures[Context->CurrentCreature].UJobsMana[i];
				InvalidateRect(GetDlgItem(hDlg,Context->UnhappyJobs[i].CtrlId),NULL,FALSE);
				}
			for (i = 0; i != GLOBAL_ANGRYJOBS; i++)
				{
				Context->AngryJobs[i].Selected = FALSE;
				Context->AngryJobs[i].Job = Context->Variables.Creatures[Context->CurrentCreature].AJobs[i];
				Context->AngryJobs[i].JobChance = Context->Variables.Creatures[Context->CurrentCreature].AJobsChance[i];
				Context->AngryJobs[i].JobMood = Context->Variables.Creatures[Context->CurrentCreature].AJobsMood[i];
				Context->AngryJobs[i].JobMana = Context->Variables.Creatures[Context->CurrentCreature].AJobsMana[i];
				InvalidateRect(GetDlgItem(hDlg,Context->AngryJobs[i].CtrlId),NULL,FALSE);
				}
			for (i = 0; i != GLOBAL_HATEJOBS; i++)
				{
				Context->HateJobs[i].Selected = FALSE;
				Context->HateJobs[i].Job = Context->Variables.Creatures[Context->CurrentCreature].NJobs[i];
				InvalidateRect(GetDlgItem(hDlg,Context->HateJobs[i].CtrlId),NULL,FALSE);
				}

			Properties_GlobalUpdateJobs(hDlg,JOB_HAPPY,Context);
			Properties_GlobalUpdateJobs(hDlg,JOB_UNHAPPY,Context);
			Properties_GlobalUpdateJobs(hDlg,JOB_ANGRY,Context);
			Properties_GlobalUpdateJobs(hDlg,JOB_HATE,Context);
			EnableWindow(GetDlgItem(hDlg,111),FALSE);
			EnableWindow(GetDlgItem(hDlg,131),FALSE);
			EnableWindow(GetDlgItem(hDlg,151),FALSE);
			EnableWindow(GetDlgItem(hDlg,171),FALSE);
			break;

		//
		// Lumière
		//
		case 7:	Misc_SetDlgItemFloatSigned(hDlg,104,Context->Variables.Creatures[Context->CurrentCreature].LightX);
			Misc_SetDlgItemFloatSigned(hDlg,106,Context->Variables.Creatures[Context->CurrentCreature].LightY);
			Misc_SetDlgItemFloatSigned(hDlg,108,Context->Variables.Creatures[Context->CurrentCreature].LightZ);
			Misc_SetDlgItemFloat(hDlg,114,Context->Variables.Creatures[Context->CurrentCreature].LightRadius);
			Context->LightColor = RGB(Context->Variables.Creatures[Context->CurrentCreature].LightR,Context->Variables.Creatures[Context->CurrentCreature].LightG,Context->Variables.Creatures[Context->CurrentCreature].LightB);
			InvalidateRect(GetDlgItem(hDlg,116),NULL,FALSE);
			CheckDlgButton(hDlg,123,(Context->Variables.Creatures[Context->CurrentCreature].LightFlicker)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,124,(Context->Variables.Creatures[Context->CurrentCreature].LightPulse)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,125,(Context->Variables.Creatures[Context->CurrentCreature].LightPlayer)?BST_CHECKED:BST_UNCHECKED);
			break;

		//
		// Sorts
		//
		case 8:	Context->CurrentSpell = SendDlgItemMessage(hDlg,101,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);

			for (i = 0; GShots[i].Id != 0xFFFFFFFF; i++)
				if (Context->Variables.CrSpells[Context->CurrentSpell].Shot == GShots[i].Id)
					{
					SendDlgItemMessage(hDlg,108,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}

			Misc_SetDlgItemFloat(hDlg,104,Context->Variables.CrSpells[Context->CurrentSpell].RechargeTime);
			Misc_SetDlgItemFloat(hDlg,106,Context->Variables.CrSpells[Context->CurrentSpell].Range);
			CheckDlgButton(hDlg,113,(Context->Variables.CrSpells[Context->CurrentSpell].Attacking)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,114,(Context->Variables.CrSpells[Context->CurrentSpell].Defensive)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,115,(Context->Variables.CrSpells[Context->CurrentSpell].Creature)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,116,(Context->Variables.CrSpells[Context->CurrentSpell].CastSelf)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,117,(Context->Variables.CrSpells[Context->CurrentSpell].CastOthers)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,118,(Context->Variables.CrSpells[Context->CurrentSpell].CastCreatures)?BST_CHECKED:BST_UNCHECKED);

			SendDlgItemMessage(hDlg,123,LB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
			for (i = 0; GCreatures[i].Id != 0; i++)
				if ((Context->Variables.Creatures[i].Shot1ID == GCrSpellsLst[Context->CurrentSpell].Id)||(Context->Variables.Creatures[i].Shot2ID == GCrSpellsLst[Context->CurrentSpell].Id)||(Context->Variables.Creatures[i].Shot3ID == GCrSpellsLst[Context->CurrentSpell].Id))
					SendDlgItemMessage(hDlg,123,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[i].Id);

			break;
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
