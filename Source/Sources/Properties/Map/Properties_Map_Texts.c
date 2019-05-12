
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres des textes
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
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des textes						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapTexts(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6805),hWnd,Properties_MapTextsProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

#pragma argsused
BOOL CALLBACK Properties_MapTextsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapTextsInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 204:
						case 206:
						case 208:
						case 210:
						case 212:
						case 214:
						case 216:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-100,(NM_UPDOWN *)lParam);
							break;
						case 124:
						case 128:
						case 132:
						case 136:
						case 140:
						case 144:
						case 148:
						case 152:
							Misc_SetUpDownInt(hDlg,((NMHDR *)lParam)->idFrom-1,(NM_UPDOWN *)lParam);
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
						case 104:
						case 106:
						case 108:
						case 110:
						case 112:
						case 114:
						case 116:
						case 123:
						case 127:
						case 131:
						case 135:
						case 139:
						case 143:
						case 147:
						case 151:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								return(TRUE);
								}
							if (lData > 9999)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),9999,FALSE);
								return(TRUE);
								}
							return(TRUE);
						}
					break;

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 100:
							Properties_MapTextsSwitchList(hDlg);
							return(TRUE);
						case 161:
							Properties_MapTextsSwitchList(hDlg);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Properties_MapTextsSave(hDlg,MapProps);
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

void Properties_MapTextsInit(HWND hDlg, MAPPROPSCTX *Context)
{
	LONG	 Offsets[8];
	LONG	 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//
	// Fichiers texte
	//

	for (X = 0; TextFiles[X] != (char *)-1;)
		{
		SendDlgItemMessage(hDlg,100,CB_ADDSTRING,(WPARAM)0,(LPARAM)TextFiles[++X]);
		X++;
		X++;
		X++;
		}
	for (X = 0, Y = 0; TextFiles[X] != (char *)-1; Y++)
		{
		if ((WORD)TextFiles[X++] == Context->MapProperties.TextTableID) break;
		X++;
		X++;
		X++;
		}
	SendDlgItemMessage(hDlg,100,CB_SETCURSEL,(WPARAM)Y,(LPARAM)0);

	//
	// Identificateurs
	//

	SetDlgItemInt(hDlg,104,Context->MapProperties.TextTitleID,FALSE);
	SetDlgItemInt(hDlg,106,Context->MapProperties.TextPlotID,FALSE);
	SetDlgItemInt(hDlg,108,Context->MapProperties.TextDebriefingID,FALSE);
	SetDlgItemInt(hDlg,110,Context->MapProperties.TextObjectiveID,FALSE);
	SetDlgItemInt(hDlg,112,Context->MapProperties.TextSubObjective1ID,FALSE);
	SetDlgItemInt(hDlg,114,Context->MapProperties.TextSubObjective2ID,FALSE);
	SetDlgItemInt(hDlg,116,Context->MapProperties.TextSubObjective3ID,FALSE);

	Misc_CreateUpDownButton(hDlg,104,204);
	Misc_CreateUpDownButton(hDlg,106,206);
	Misc_CreateUpDownButton(hDlg,108,208);
	Misc_CreateUpDownButton(hDlg,110,210);
	Misc_CreateUpDownButton(hDlg,112,212);
	Misc_CreateUpDownButton(hDlg,114,214);
	Misc_CreateUpDownButton(hDlg,116,216);

	//
	// Textes de remplacement
	//

	Offsets[0] = 0;
	Offsets[1] = 0;
	Offsets[2] = 0;
	Offsets[3] = 0;
	Offsets[4] = 0;
	Offsets[5] = 0;
	Offsets[6] = 0;
	Offsets[7] = 0;
	SendDlgItemMessage(hDlg,122,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,138,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,142,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,146,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);
	SendDlgItemMessage(hDlg,150,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsNone);

	for (X = 0, Y = 1; BadCreatures[X].Name != NULL; X++, Y++)
		{
		if (Context->MapProperties.TextIntroCreatureID0 == BadCreatures[X].Id) Offsets[0] = Y;
		if (Context->MapProperties.TextIntroCreatureID1 == BadCreatures[X].Id) Offsets[1] = Y;
		if (Context->MapProperties.TextIntroCreatureID2 == BadCreatures[X].Id) Offsets[2] = Y;
		if (Context->MapProperties.TextIntroCreatureID3 == BadCreatures[X].Id) Offsets[3] = Y;
		if (Context->MapProperties.TextIntroCreatureID4 == BadCreatures[X].Id) Offsets[4] = Y;
		if (Context->MapProperties.TextIntroCreatureID5 == BadCreatures[X].Id) Offsets[5] = Y;
		if (Context->MapProperties.TextIntroCreatureID6 == BadCreatures[X].Id) Offsets[6] = Y;
		if (Context->MapProperties.TextIntroCreatureID7 == BadCreatures[X].Id) Offsets[7] = Y;
		SendDlgItemMessage(hDlg,122,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,138,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,142,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,146,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,150,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadCreatures[X].Name);
		SendDlgItemMessage(hDlg,122,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,126,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,130,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,134,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,138,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,142,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,146,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		SendDlgItemMessage(hDlg,150,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadCreatures[X].Id);
		}
	for (X = 0; BadElites[X].Name != NULL; X++, Y++)
		{
		if (Context->MapProperties.TextIntroCreatureID0 == BadElites[X].Id) Offsets[0] = Y;
		if (Context->MapProperties.TextIntroCreatureID1 == BadElites[X].Id) Offsets[1] = Y;
		if (Context->MapProperties.TextIntroCreatureID2 == BadElites[X].Id) Offsets[2] = Y;
		if (Context->MapProperties.TextIntroCreatureID3 == BadElites[X].Id) Offsets[3] = Y;
		if (Context->MapProperties.TextIntroCreatureID4 == BadElites[X].Id) Offsets[4] = Y;
		if (Context->MapProperties.TextIntroCreatureID5 == BadElites[X].Id) Offsets[5] = Y;
		if (Context->MapProperties.TextIntroCreatureID6 == BadElites[X].Id) Offsets[6] = Y;
		if (Context->MapProperties.TextIntroCreatureID7 == BadElites[X].Id) Offsets[7] = Y;
		SendDlgItemMessage(hDlg,122,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,138,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,142,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,146,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,150,CB_ADDSTRING,(WPARAM)0,(LPARAM)BadElites[X].Name);
		SendDlgItemMessage(hDlg,122,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,126,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,130,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,134,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,138,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,142,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,146,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		SendDlgItemMessage(hDlg,150,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)BadElites[X].Id);
		}
	for (X = 0; Heros[X].Name != NULL; X++, Y++)
		{
		if (Context->MapProperties.TextIntroCreatureID0 == Heros[X].Id) Offsets[0] = Y;
		if (Context->MapProperties.TextIntroCreatureID1 == Heros[X].Id) Offsets[1] = Y;
		if (Context->MapProperties.TextIntroCreatureID2 == Heros[X].Id) Offsets[2] = Y;
		if (Context->MapProperties.TextIntroCreatureID3 == Heros[X].Id) Offsets[3] = Y;
		if (Context->MapProperties.TextIntroCreatureID4 == Heros[X].Id) Offsets[4] = Y;
		if (Context->MapProperties.TextIntroCreatureID5 == Heros[X].Id) Offsets[5] = Y;
		if (Context->MapProperties.TextIntroCreatureID6 == Heros[X].Id) Offsets[6] = Y;
		if (Context->MapProperties.TextIntroCreatureID7 == Heros[X].Id) Offsets[7] = Y;
		SendDlgItemMessage(hDlg,122,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,126,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,130,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,134,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,138,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,142,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,146,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,150,CB_ADDSTRING,(WPARAM)0,(LPARAM)Heros[X].Name);
		SendDlgItemMessage(hDlg,122,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,126,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,130,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,134,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,138,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,142,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,146,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		SendDlgItemMessage(hDlg,150,CB_SETITEMDATA,(WPARAM)Y,(LPARAM)Heros[X].Id);
		}

	SendDlgItemMessage(hDlg,122,CB_SETCURSEL,(WPARAM)Offsets[0],(LPARAM)0);
	SendDlgItemMessage(hDlg,126,CB_SETCURSEL,(WPARAM)Offsets[1],(LPARAM)0);
	SendDlgItemMessage(hDlg,130,CB_SETCURSEL,(WPARAM)Offsets[2],(LPARAM)0);
	SendDlgItemMessage(hDlg,134,CB_SETCURSEL,(WPARAM)Offsets[3],(LPARAM)0);
	SendDlgItemMessage(hDlg,138,CB_SETCURSEL,(WPARAM)Offsets[4],(LPARAM)0);
	SendDlgItemMessage(hDlg,142,CB_SETCURSEL,(WPARAM)Offsets[5],(LPARAM)0);
	SendDlgItemMessage(hDlg,146,CB_SETCURSEL,(WPARAM)Offsets[6],(LPARAM)0);
	SendDlgItemMessage(hDlg,150,CB_SETCURSEL,(WPARAM)Offsets[7],(LPARAM)0);

	SetDlgItemInt(hDlg,123,Context->MapProperties.TextIntroTextID0,FALSE);
	SetDlgItemInt(hDlg,127,Context->MapProperties.TextIntroTextID1,FALSE);
	SetDlgItemInt(hDlg,131,Context->MapProperties.TextIntroTextID2,FALSE);
	SetDlgItemInt(hDlg,135,Context->MapProperties.TextIntroTextID3,FALSE);
	SetDlgItemInt(hDlg,139,Context->MapProperties.TextIntroTextID4,FALSE);
	SetDlgItemInt(hDlg,143,Context->MapProperties.TextIntroTextID5,FALSE);
	SetDlgItemInt(hDlg,147,Context->MapProperties.TextIntroTextID6,FALSE);
	SetDlgItemInt(hDlg,151,Context->MapProperties.TextIntroTextID7,FALSE);

	Misc_CreateUpDownButton(hDlg,123,124);
	Misc_CreateUpDownButton(hDlg,127,128);
	Misc_CreateUpDownButton(hDlg,131,132);
	Misc_CreateUpDownButton(hDlg,135,136);
	Misc_CreateUpDownButton(hDlg,139,140);
	Misc_CreateUpDownButton(hDlg,143,144);
	Misc_CreateUpDownButton(hDlg,147,148);
	Misc_CreateUpDownButton(hDlg,151,152);

	//
	// Création des listes de texte
	//

	SendDlgItemMessage(hDlg,161,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsCurrent);
	SendDlgItemMessage(hDlg,161,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgTextsBriefing);
	SendDlgItemMessage(hDlg,161,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	Properties_MapTextsSwitchList(hDlg);

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	return;
}


// «»»» Sauvegarde des modifications ««««««««««««««««««««««««««««««««««««»

void Properties_MapTextsSave(HWND hDlg, MAPPROPSCTX *Context)
{
	int	i;

	Context->MapProperties.TextTitleID	   = (WORD)GetDlgItemInt(hDlg,104,NULL,FALSE);
	Context->MapProperties.TextPlotID	   = (WORD)GetDlgItemInt(hDlg,106,NULL,FALSE);
	Context->MapProperties.TextDebriefingID    = (WORD)GetDlgItemInt(hDlg,108,NULL,FALSE);
	Context->MapProperties.TextObjectiveID 	   = (WORD)GetDlgItemInt(hDlg,110,NULL,FALSE);
	Context->MapProperties.TextSubObjective1ID = (WORD)GetDlgItemInt(hDlg,112,NULL,FALSE);
	Context->MapProperties.TextSubObjective2ID = (WORD)GetDlgItemInt(hDlg,114,NULL,FALSE);
	Context->MapProperties.TextSubObjective3ID = (WORD)GetDlgItemInt(hDlg,116,NULL,FALSE);

	Context->MapProperties.TextIntroTextID0 = (WORD)GetDlgItemInt(hDlg,123,NULL,FALSE);
	Context->MapProperties.TextIntroTextID1 = (WORD)GetDlgItemInt(hDlg,127,NULL,FALSE);
	Context->MapProperties.TextIntroTextID2 = (WORD)GetDlgItemInt(hDlg,131,NULL,FALSE);
	Context->MapProperties.TextIntroTextID3 = (WORD)GetDlgItemInt(hDlg,135,NULL,FALSE);
	Context->MapProperties.TextIntroTextID4 = (WORD)GetDlgItemInt(hDlg,139,NULL,FALSE);
	Context->MapProperties.TextIntroTextID5 = (WORD)GetDlgItemInt(hDlg,143,NULL,FALSE);
	Context->MapProperties.TextIntroTextID6 = (WORD)GetDlgItemInt(hDlg,147,NULL,FALSE);
	Context->MapProperties.TextIntroTextID7 = (WORD)GetDlgItemInt(hDlg,151,NULL,FALSE);

	i = SendDlgItemMessage(hDlg,122,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID0 = (BYTE)SendDlgItemMessage(hDlg,122,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID0 = 0;
	i = SendDlgItemMessage(hDlg,126,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID1 = (BYTE)SendDlgItemMessage(hDlg,126,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID1 = 0;
	i = SendDlgItemMessage(hDlg,130,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID2 = (BYTE)SendDlgItemMessage(hDlg,130,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID2 = 0;
	i = SendDlgItemMessage(hDlg,134,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID3 = (BYTE)SendDlgItemMessage(hDlg,134,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID3 = 0;
	i = SendDlgItemMessage(hDlg,138,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID4 = (BYTE)SendDlgItemMessage(hDlg,138,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID4 = 0;
	i = SendDlgItemMessage(hDlg,142,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID5 = (BYTE)SendDlgItemMessage(hDlg,142,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID5 = 0;
	i = SendDlgItemMessage(hDlg,146,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID6 = (BYTE)SendDlgItemMessage(hDlg,146,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID6 = 0;
	i = SendDlgItemMessage(hDlg,150,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextIntroCreatureID7 = (BYTE)SendDlgItemMessage(hDlg,150,CB_GETITEMDATA,(WPARAM)i,(LPARAM)0);
	else Context->MapProperties.TextIntroCreatureID7 = 0;

	i = SendDlgItemMessage(hDlg,100,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i) Context->MapProperties.TextTableID = (WORD)TextFiles[i*4];		
	return;
}


// «»»» Change la liste des textes ««««««««««««««««««««««««««««««««««««««»

void Properties_MapTextsSwitchList(HWND hDlg)
{
	NODE	 TextList;
	NODE	*CurrentText;
	char	*File;
	int	 i,j,k;

	SendDlgItemMessage(hDlg,160,LB_RESETCONTENT,(WPARAM)0,(LPARAM)0);

	j = SendDlgItemMessage(hDlg,161,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	k = SendDlgItemMessage(hDlg,100,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (!k) return;
	i = 0;
	while(k--)
		{
		if (TextFiles[i] == (char *)-1) return;
		i += 4;
		}
	File = TextFiles[i+j+2];
	if (!File) return;

	ZeroMemory(&TextList,sizeof(NODE));
	Properties_MapTextsGetList(hDlg,File,&TextList);
	for (CurrentText = TextList.next; CurrentText != NULL; CurrentText = CurrentText->next)
		SendDlgItemMessage(hDlg,160,LB_ADDSTRING,(WPARAM)0,(LPARAM)((MAPPROPSTEXT *)CurrentText)->textpreview);
	List_ReleaseMemory(&TextList);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines relatives aux textes du jeu				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'une liste de textes ««««««««««««««««««««««««««««««««««»

void Properties_MapTextsGetList(HWND hDlg, char *TextFileName, NODE *TextList)
{
	MAPPROPSTEXT	*Text;
	HANDLE		 FileHandle;
	DWORD		 LastError;
	DWORD		 Count;
	DWORD		 Offset;
	DWORD		 FileOffset;
	DWORD		 TextLen;
	char		*TextFile;
	char		*GameText;
	char		*TempPtr;

	if (!Config_GetGamePath(Config.GamePath)) return;

	TextFile = GlobalAlloc(GPTR,MAX_PATH);
	if (!TextFile) return;
	strcpy(TextFile,Config.GamePath);
	strcat(TextFile,szTextsPath);
	strcat(TextFile,TextFileName);

	FileHandle = CreateFile(TextFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		LastError = GetLastError();
		if ((LastError == ERROR_FILE_NOT_FOUND)||(LastError == ERROR_PATH_NOT_FOUND)) goto Error_0;
		goto Error_1;
		}

	//
	// Charge le nombre maximum d'identificateurs
	//

	if (SetFilePointer(FileHandle,8,NULL,FILE_BEGIN) == 0xFFFFFFFF)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	ReadFile(FileHandle,&Count,4,&LastError,NULL);
	if (LastError != 4)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	//
	// Charge tous les textes
	//

	TextLen = 0;
	FileOffset = 0;

	while(Count--)
		{
		/* Lecture du prochain offset */
		if (SetFilePointer(FileHandle,12+FileOffset,NULL,FILE_BEGIN) == 0xFFFFFFFF)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		/* Retrouve le texte dans le fichier */
		ReadFile(FileHandle,&Offset,4,&LastError,NULL);
		if (LastError != 4)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		if (SetFilePointer(FileHandle,12+Offset,NULL,FILE_BEGIN) == 0xFFFFFFFF)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		/* Lecture de l'en-tête du fichier */
		ReadFile(FileHandle,&Offset,1,&LastError,NULL);
		if (LastError != 1)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		ReadFile(FileHandle,&TextLen,2,&LastError,NULL);
		if (LastError != 2)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		if (SetFilePointer(FileHandle,1,NULL,FILE_CURRENT) == 0xFFFFFFFF)
			{
			LastError = GetLastError();
			goto Error_2;
			}

		/* Chargement & conversion du texte entier */
		#if (DEBUG_TEXTS)
		GameText = GlobalAlloc(GPTR,(TextLen+1)*8);
		#else
		GameText = GlobalAlloc(GPTR,(TextLen+1)*4);
		#endif
		if (!GameText) goto Error_2;

		ReadFile(FileHandle,GameText,TextLen,&LastError,NULL);
		if (LastError != TextLen)
			{
			LastError = GetLastError();
			goto Error_3;
			}

		Properties_MapTextsGameToASCII((BYTE *)GameText,(BYTE *)GameText+TextLen+1,TextLen);

		/* Stockage du texte dans la liste */
		Text = GlobalAlloc(GPTR,sizeof(MAPPROPSTEXT));
		if (!Text) goto Error_3;
		wsprintf(Text->textpreview,szSpecialNumber,FileOffset/4+1);
		TempPtr = strchr(Text->textpreview,0);
		strncpy(TempPtr,GameText+TextLen+1,80);
		if (strlen(GameText+TextLen+1) > 80) strcat(Text->textpreview,szMore);
		List_AddEntry((NODE *)Text,TextList);

		GlobalFree(GameText);
		FileOffset += 4;
		}

	CloseHandle(FileHandle);
	GlobalFree(TextFile);
	return;

//--- Erreurs ---

Error_3:GlobalFree(GameText);
Error_2:CloseHandle(FileHandle);
Error_1:SetLastError(LastError);
	Misc_PrintError(hDlg,szCfgTextsBadFile,NULL,MB_ICONEXCLAMATION);
Error_0:GlobalFree(TextFile);
	return;
}


// «»»» Affichage du texte correspondant à la voix ««««««««««««««««««««««»

void Properties_MapTextsSetText(HWND hDlg, UINT CtrlId, char* SoundFile, LONG Identifier)
{
	HANDLE	 FileHandle;
	DWORD	 LastError;
	DWORD	 Offset;
	char	*Text;
	char	*TextFile;
	char	*SoundPtr;

	SetDlgItemText(hDlg,CtrlId,NULL);
	if ((!SoundFile)||(!*SoundFile)) return;
	if (!Config.GamePath[0]) return;

	//
	// Accès au fichier texte correspondant
	//

	TextFile = GlobalAlloc(GPTR,MAX_PATH);
	if (!TextFile) return;

	SoundPtr = strchr(SoundFile,'_');
	if (SoundPtr) SoundPtr++;
	else SoundPtr = SoundFile;

	strcpy(TextFile,Config.GamePath);
	strcat(TextFile,szTextsPath);
	if (!strcmpi("MENTOR",SoundPtr)) strcat(TextFile,"Speech");
	else strcat(TextFile,SoundPtr);
	strcat(TextFile,szTextsExt);

	FileHandle = CreateFile(TextFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		LastError = GetLastError();
		if ((LastError == ERROR_FILE_NOT_FOUND)||(LastError == ERROR_PATH_NOT_FOUND)) goto Error_0;
		goto Error_1;
		}

	//
	// Charge le nombre maximum d'identificateurs
	//

	if (SetFilePointer(FileHandle,8,NULL,FILE_BEGIN) == 0xFFFFFFFF)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	ReadFile(FileHandle,&Offset,4,&LastError,NULL);
	if (LastError != 4)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	if (Identifier > Offset)
		{
		SetDlgItemText(hDlg,CtrlId,szCfgTextsOutOfRange);
		CloseHandle(FileHandle);
		GlobalFree(TextFile);
		return;
		}

	//
	// Recherche de l'offset du texte
	//

	if (SetFilePointer(FileHandle,12+(Identifier-1)*4,NULL,FILE_BEGIN) == 0xFFFFFFFF)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	ReadFile(FileHandle,&Offset,4,&LastError,NULL);
	if (LastError != 4)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	if (SetFilePointer(FileHandle,12+Offset,NULL,FILE_BEGIN) == 0xFFFFFFFF)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	//
	// Chargement de l'en-tête du texte
	//

	Offset = 0;
	ReadFile(FileHandle,&Offset,1,&LastError,NULL);
	if (LastError != 1)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	ReadFile(FileHandle,&Offset,2,&LastError,NULL);
	if (LastError != 2)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	if (SetFilePointer(FileHandle,1,NULL,FILE_CURRENT) == 0xFFFFFFFF)
		{
		LastError = GetLastError();
		goto Error_2;
		}

	//
	// Chargement du texte
	//

	#if (DEBUG_TEXTS)
	Text = GlobalAlloc(GPTR,(Offset+1)*8);
	#else
	Text = GlobalAlloc(GPTR,(Offset+1)*4);
	#endif
	if (!Text) goto Error_2;

	ReadFile(FileHandle,Text,Offset,&LastError,NULL);
	if (LastError != Offset)
		{
		LastError = GetLastError();
		goto Error_3;
		}

	//
	// Conversion du texte
	//

	Properties_MapTextsGameToASCII((BYTE *)Text,(BYTE *)Text+Offset+1,Offset);
	SetDlgItemText(hDlg,CtrlId,Text+Offset+1);

	//
	// Terminé
	//

	GlobalFree(Text);
	CloseHandle(FileHandle);
	GlobalFree(TextFile);
	return;

//--- Erreurs ---

Error_3:GlobalFree(Text);
Error_2:CloseHandle(FileHandle);
Error_1:SetLastError(LastError);
	Misc_PrintError(hDlg,szCfgTextsBadFile,NULL,MB_ICONEXCLAMATION);
Error_0:GlobalFree(TextFile);
	return;
}


// «»»» Conversion d'un texte du jeu en texte ASCII «««««««««««««««««««««»

void Properties_MapTextsGameToASCII(BYTE *GameText, BYTE *Buffer, LONG Size)
{
	BYTE	 Charset;

	while(Size--)
		{
		if ((*GameText <= 0xB4)&&(*GameText >= 0x9B))
			{
			Charset = (BYTE)(0xB4-*GameText+'a');
			goto Set_Charset;
			}

		if ((*GameText <= 0xD4)&&(*GameText >= 0xBB))
			{
			Charset = (BYTE)(0xD4-*GameText+'A');
			goto Set_Charset;
			}

		if ((*GameText >= 0x02)&&(*GameText <= 0x0A))
			{
			Charset = (BYTE)(*GameText-2+'1');
			goto Set_Charset;
			}

		switch(*GameText)
			{
			case 0x01: Charset = '*';
				   break;
			case 0x0B: Charset = '0';
				   break;
			case 0x0C: Charset = ' ';
				   break;
			case 0x0F: Charset = '.';
				   break;
			case 0x10: Charset = ',';
				   break;
			case 0x13: Charset = '-';
				   break;
			case 0x15: *Buffer++ = '.';
				   *Buffer++ = '.';
				   Charset = '.';
				   break;
			case 0x16: Charset = 'œ';
				   break;
			case 0x1B: Charset = 'û';
				   break;
			case 0x1D: Charset = 'ù';
				   break;
			case 0x22: Charset = 'ô';
				   break;
			case 0x27: Charset = 'ï';
				   break;
			case 0x28: Charset = 'î';
				   break;
			case 0x2C: Charset = 'ê';
				   break;
			case 0x2D: Charset = 'é';
				   break;
			case 0x2E: Charset = 'è';
				   break;
			case 0x2F: Charset = 'ç';
				   break;
			case 0x34: Charset = 'â';
				   break;
			case 0x36: Charset = 'à';
				   break;
			case 0x68: Charset = '®';
				   break;
			case 0xE0: Charset = '\'';
				   break;
			case 0xE4: Charset = '"';
				   break;
			case 0xE5: Charset = '!';
				   break;
			case 0xD6: Charset = '?';
				   break;
			case 0xDA: Charset = ';';
				   break;
			case 0xDB: Charset = ':';
				   break;
			case 0xDC: Charset = '/';
				   break;
			case 0xDD: Charset = '+';
				   break;
			case 0xDE: Charset = ')';
				   break;
			case 0xDF: Charset = '(';
				   break;
			default:   // Caractères non reconnus
				   #if (DEBUG_TEXTS)
				   *Buffer++ = '$';
				   Charset = (BYTE)((*GameText&0xF0)>>4);
				   if (Charset <= 9) *Buffer++ = (BYTE)(Charset+'0');
				   else *Buffer++ = (BYTE)(Charset-0x0A+'A');
				   Charset = (BYTE)(*GameText&0x0F);
				   if (Charset <= 9) *Buffer++ = (BYTE)(Charset+'0');
				   else *Buffer++ = (BYTE)(Charset-0x0A+'A');
				   Charset = '*';
				   break;
				   #else
				   Charset = '*';
				   break;
				   #endif
			}

	Set_Charset:
		GameText++;
		*Buffer++ = Charset;
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
