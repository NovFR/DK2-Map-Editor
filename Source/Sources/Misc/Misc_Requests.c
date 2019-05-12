
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'affichage des requêtes.
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage des erreurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Misc_PrintError(HWND hWnd, char *WindowText, char *WindowTitle, UINT Flags)
{
	DWORD	 ResultLen;
	char	*ErrorMsg;
	char	*FinalMsg;

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),LOCALE_USER_DEFAULT,(char *)&ErrorMsg,1,NULL))
		goto Print_VanillaMsg;

	ResultLen = strlen(WindowText)+strlen(szLF)+strlen(ErrorMsg)+1;
	FinalMsg = GlobalAlloc(GPTR,ResultLen);
	if (!FinalMsg) goto Print_VanillaMsg2;

	strcpy(FinalMsg,WindowText);
	strcat(FinalMsg,szLF);
	strcat(FinalMsg,ErrorMsg);
	MessageBox(hWnd,FinalMsg,WindowTitle,MB_OK|Flags);
	GlobalFree(FinalMsg);
	LocalFree(ErrorMsg);
	return;

//--- Affichage du message brut ---

Print_VanillaMsg2:
	LocalFree(ErrorMsg);
Print_VanillaMsg:
	MessageBox(hWnd,WindowText,WindowTitle,MB_OK|Flags);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage d'une requête en cas de changement effectués		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Misc_CheckChanges(HWND hWnd, UINT Source)
{
	MESSAGEBOXSTRUCT	 MessageBox;
	char			*WindowText;
	char			*RequestText;
	int			 Result = 1;

	if (MapChanges)
		{
		ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
		MessageBox.windowtitle = (char *)-1;
		MessageBox.flags = MBF_USEICON|MBF_DESTROYICON;
		MessageBox.defaulticon = MB_ICONQUESTION;
		MessageBox.windowpos = MBP_CENTER;
		MessageBox.windowicon = LoadImage(hInstance,MAKEINTRESOURCE(75),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
		switch(Source)
			{
			case IDM_NEW:
				WindowText = szNewRequest;
				MessageBox.gadgets = szMessageBoxExtYesNo;
				break;
			case IDM_OPEN:
				WindowText = szLoadRequest;
				MessageBox.gadgets = szMessageBoxExtYesNo;
				break;
			case IDM_QUIT:
				WindowText = szExitRequest;
				MessageBox.gadgets = szMessageBoxExtYesNo;
				break;
			default:
				return(1);
			}

		RequestText = GlobalAlloc(GPTR,2048);
		if (!RequestText)
			{
			MessageBeep(MB_ICONHAND);
			return(0);
			}
		wsprintf(RequestText,WindowText,MapChanges);

		MessageBox.windowtext = RequestText;
		Result = Misc_MessageBoxExtArgs(hWnd,&MessageBox);
		GlobalFree(RequestText);
		}

	return(Result);
}



// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage d'une requête						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'une requête simple ««««««««««««««««««««««««««««««««««»

int Misc_MessageBoxExt(HWND hWnd, char *WindowText, char *WindowTitle, char *Gadgets, UINT Icon)
{
	MESSAGEBOXSTRUCT	MsgStruct;

	ZeroMemory(&MsgStruct,sizeof(MESSAGEBOXSTRUCT));
	if (Gadgets) MsgStruct.gadgets = Gadgets;
	if (WindowText) MsgStruct.windowtext = WindowText;
	if (WindowTitle) MsgStruct.windowtitle = WindowTitle;
	MsgStruct.defaulticon = Icon;
	return(Misc_MessageBoxExtArgs(hWnd,&MsgStruct));
}


// «»»» Affichage de la requête «««««««««««««««««««««««««««««««««««««««««»

int Misc_MessageBoxExtArgs(HWND hWnd, MESSAGEBOXSTRUCT *MsgStruct)
{
	DLGTEMPLATE		*DialogBox;
	DWORD			 DialogFlags;
	DWORD			 DialogGadgetId;
	WORD			*DialogBoxPtr;
	WORD			*DialogBoxString;
	WORD			 DialogGadgets;
	char			*TextPtr;
	char			*PrevPtr;
	char			 Charset[4];
	long			 CharsetCount;

	/* Test les différentes possibilités */

	MsgStruct->internalflags = 0;
	MsgStruct->numgadgets = 0;

	if (MsgStruct->flags&MBF_CENTERTEXT)
		MsgStruct->internalflags |= MBF_CENTERTEXT;
	if (MsgStruct->flags&MBF_EDITTEXT)
		MsgStruct->internalflags |= MBF_EDITTEXT;
	if (MsgStruct->flags&MBF_NOMINMAX)
		MsgStruct->internalflags |= MBF_NOMINMAX;
	if (MsgStruct->flags&MBF_ESNUMBER)
		MsgStruct->internalflags |= MBF_ESNUMBER;
	if (((MsgStruct->flags&MBF_ESNUMBER)||(MsgStruct->flags&MBF_NUMBER))&&(MsgStruct->flags&MBF_EDITTEXT))
		MsgStruct->internalflags |= MBF_NUMBER;
	if ((!MsgStruct->windowtext)||(MsgStruct->flags&MBF_NOTEXT))
		MsgStruct->internalflags |= MBF_NOTEXT;
	if ((MsgStruct->windowtitle == (char *)-1)||(MsgStruct->flags&MBF_NOTITLE))
		MsgStruct->internalflags |= MBF_NOTITLE;
	if ((!MsgStruct->gadgets)||(MsgStruct->flags&MBF_NOGADGETS))
		MsgStruct->internalflags |= MBF_NOGADGETS;

	if ((MsgStruct->internalflags&MBF_NOTEXT)||(MsgStruct->flags&MBF_NOICON))
		MsgStruct->internalflags |= MBF_NOICON;
	if ( (!(MsgStruct->internalflags&MBF_NOTEXT)) && (MsgStruct->flags&MBF_AUTOREPLY) && (!(MsgStruct->flags&MBF_EDITTEXT)) && (MsgStruct->requestbits) )
		MsgStruct->internalflags |= MBF_AUTOREPLY;
	if ((!(MsgStruct->internalflags&MBF_NOICON))&&(MsgStruct->flags&MBF_USEICON)&&(MsgStruct->windowicon))
		{
		MsgStruct->internalflags |= MBF_USEICON;
		if (MsgStruct->flags&MBF_DESTROYICON) MsgStruct->internalflags |= MBF_DESTROYICON;
		}

	DialogFlags = MsgStruct->internalflags;

	/* Initialisations */

	if ((DialogFlags&MBF_AUTOREPLY)&&(*MsgStruct->requestbits&MsgStruct->statebit))
		return(MsgStruct->defaultgadget);

	if (!MsgStruct->numberwidth)
		MsgStruct->numberwidth = 70;

	MsgStruct->textfont = szMessageBoxExtFont;
	MsgStruct->textfontsize = 8;
	if ((MsgStruct->flags&MBF_USEFONT)&&(MsgStruct->fontname[0])&&(MsgStruct->fontsize))
		{
		HDC	hDC;
		DWORD	NumFonts;
		hDC = CreateCompatibleDC(NULL);
		if (hDC)
			{
			NumFonts = 0;
			EnumFonts(hDC,MsgStruct->fontname,(FONTENUMPROC)Misc_MessageBoxExtEnumFonts,(LPARAM)&NumFonts);
			if (NumFonts)
				{
				MsgStruct->textfont = MsgStruct->fontname;
				MsgStruct->textfontsize = (WORD)MsgStruct->fontsize;
				}
			DeleteDC(hDC);
			}
		}

	DialogGadgets = 3;
	if (DialogFlags&MBF_NOTEXT)
		--DialogGadgets;
	if (DialogFlags&MBF_NOICON)
		--DialogGadgets;
	if ((DialogFlags&MBF_NUMBER)&&(!(DialogFlags&MBF_NOMINMAX)))
		DialogGadgets += (WORD)4;

	/* Nombre & taille des gadgets */

	if (!(DialogFlags&MBF_NOGADGETS))
		{
		for (TextPtr = MsgStruct->gadgets; *TextPtr != 0;)
			{
			for (PrevPtr = TextPtr;;)
				{
				if (*PrevPtr == 0) break;
				if (*PrevPtr == '|') break;
				PrevPtr++;
				}
			DialogGadgets++;
			MsgStruct->numgadgets++;
			if (*PrevPtr == 0) break;
			TextPtr = ++PrevPtr;
			}
		}

	if (!DialogGadgets)
		{
		if ((MsgStruct->windowicon)&&(MsgStruct->internalflags&MBF_DESTROYICON))
			{
			DestroyIcon(MsgStruct->windowicon);
			MsgStruct->windowicon = NULL;
			}
		MessageBeep(MB_ICONHAND);
		return(0);
		}

	//--- Création de la requête ---

	DialogBox = (DLGTEMPLATE *)GlobalAlloc(GPTR,MESSAGEBOXBUFFER);
	if (!DialogBox)
		{
		if ((MsgStruct->windowicon)&&(MsgStruct->internalflags&MBF_DESTROYICON))
			{
			DestroyIcon(MsgStruct->windowicon);
			MsgStruct->windowicon = NULL;
			}
		MessageBeep(MB_ICONHAND);
		return(0);
		}

	/* En-tête de la boîte de dialog */

	DialogBoxPtr = (WORD *)DialogBox;
	*((DWORD *)DialogBoxPtr)++ = WS_POPUP|WS_BORDER|WS_SYSMENU|DS_MODALFRAME|WS_CAPTION|DS_SETFONT;
	*((DWORD *)DialogBoxPtr)++ = 0;
	*DialogBoxPtr++ = (WORD)DialogGadgets;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = 0;

	DialogBoxString = (WORD *)DialogBoxPtr;
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szMessageClass,-1,DialogBoxString,2048);
	DialogBoxPtr = (WORD *)(DialogBoxString + lstrlenW(DialogBoxString) + 1);

	DialogBoxString = (WORD *)DialogBoxPtr;
	if (MsgStruct->windowtitle == NULL)
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szMessageBoxExtErrorTitle,-1,DialogBoxString,2048);
	else
		if (DialogFlags&MBF_NOTITLE)
			{
			if ((DialogFlags&MBF_NOGADGETS)||(DialogGadgets <= 4))
				MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szMessageBoxExtInfoTitle,-1,DialogBoxString,2048);
			else
				MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szMessageBoxExtReqTitle,-1,DialogBoxString,2048);
			}
		else
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,MsgStruct->windowtitle,-1,DialogBoxString,2048);
	DialogBoxPtr = (WORD *)(DialogBoxString + lstrlenW(DialogBoxString) + 1);

	*DialogBoxPtr++ = (WORD)MsgStruct->textfontsize;
	DialogBoxString = (WORD *)DialogBoxPtr;
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,MsgStruct->textfont,-1,DialogBoxString,2048);
	DialogBoxPtr = (WORD *)(DialogBoxString + lstrlenW(DialogBoxString) + 1);
	if (((DWORD)DialogBoxPtr)%4) *DialogBoxPtr++ = 0;

	/* Icône */

	if (!(DialogFlags&MBF_NOICON))
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1000,BS_PUSHBUTTON|WS_DISABLED|BS_OWNERDRAW,0x0080,szMessageBoxExtEmpty);

	/* Texte */

	if (!(DialogFlags&MBF_NOTEXT))
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1100,SS_LEFT|SS_NOPREFIX|((DialogFlags&MBF_CENTERTEXT)?SS_CENTER:0),0x0082,MsgStruct->windowtext);

	/* Texte à éditer ou Confirmation */

	if (DialogFlags&MBF_EDITTEXT)
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1400,WS_BORDER|ES_AUTOHSCROLL|((DialogFlags&MBF_ESNUMBER)?(ES_RIGHT|ES_NUMBER):ES_LEFT),0x0081,szMessageBoxExtEmpty);
	else
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1200,BS_AUTOCHECKBOX,0x0080,szMessageBoxExtDoNotDisplayAgain);

	/* Information min. / max. */

	if ((DialogFlags&MBF_NUMBER)&&(!(DialogFlags&MBF_NOMINMAX)))
		{
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1300,WS_BORDER|SS_CENTER,0x0082,szMessageBoxExtMin);
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1301,WS_BORDER|SS_CENTER,0x0082,szMessageBoxExtMax);
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1302,WS_BORDER|ES_RIGHT|ES_READONLY,0x0081,szMessageBoxExtEmpty);
		DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,1303,WS_BORDER|ES_RIGHT|ES_READONLY,0x0081,szMessageBoxExtEmpty);
		}

	/* Boutons */

	if ((MsgStruct->gadgets)&&(!(MsgStruct->flags&MBF_NOGADGETS)))
		{
		DWORD	TempFlags;

		for (DialogGadgetId = 1, TextPtr = MsgStruct->gadgets; *TextPtr != 0; DialogGadgetId++)
			{
			TempFlags = 0;

			if (!MsgStruct->defaultgadget)
				{
				if (DialogFlags&MBF_EDITTEXT)
					{
					if (DialogGadgetId == 1)
						TempFlags |= BS_DEFPUSHBUTTON;
					}
				else
					{
					if (DialogGadgetId == MsgStruct->numgadgets)
						TempFlags |= BS_DEFPUSHBUTTON;
					}
				}
			else
				{
				if (DialogGadgetId == MsgStruct->defaultgadget)
					TempFlags |= BS_DEFPUSHBUTTON;
				}

			for (PrevPtr = TextPtr;;)
				{
				if (*PrevPtr == 0) break;
				if (*PrevPtr == '|') break;
				PrevPtr++;
				}
			Charset[0] = *PrevPtr;
			*PrevPtr = 0;

			DialogBoxPtr = Misc_MessageBoxExtFillDlgTemplate(DialogBoxPtr,(short)DialogGadgetId,TempFlags,0x0080,TextPtr);

			*PrevPtr = Charset[0];
			if (*PrevPtr == 0) break;
			TextPtr = ++PrevPtr;
			}
		}
	*DialogBoxPtr = 0;

	//--- Affichage de la requête ---

	CharsetCount = DialogBoxIndirectParam(hInstance,(DLGTEMPLATE *)DialogBox,hWnd,Misc_MessageBoxExtProc,(LPARAM)MsgStruct);
	if (CharsetCount == -1)
		{
		Misc_PrintError(hWnd,"???",NULL,MB_ICONHAND);
		if ((MsgStruct->windowicon)&&(MsgStruct->internalflags&MBF_DESTROYICON))
			{
			DestroyIcon(MsgStruct->windowicon);
			MsgStruct->windowicon = NULL;
			}
		GlobalFree(DialogBox);
		MessageBeep(MB_ICONHAND);
		return(0);
		}

	GlobalFree(DialogBox);
	return(CharsetCount);
}

//--- Enumère les caractères disponibles ---

#pragma argsused
int CALLBACK Misc_MessageBoxExtEnumFonts(LOGFONT *LogFont, TEXTMETRIC *TextMetric, DWORD Type, LPARAM lParam)
{
	*((DWORD *)lParam) = 1;
	return(0);
}

//--- Remplissage d'une structure ---

WORD* Misc_MessageBoxExtFillDlgTemplate(WORD *DialogBoxPtr, WORD CtlID, DWORD CtlStyle, WORD CtlType, char *CtlText)
{
	WORD			*DialogBoxString;

	*((DWORD *)DialogBoxPtr)++ = WS_CHILD|WS_VISIBLE|CtlStyle;
	*((DWORD *)DialogBoxPtr)++ = 0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = (short)0;
	*DialogBoxPtr++ = CtlID;
	*DialogBoxPtr++ = 0xFFFF;
	*DialogBoxPtr++ = CtlType;

	DialogBoxString = (WORD *)DialogBoxPtr;
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,CtlText,-1,DialogBoxString,2048);
	DialogBoxPtr = (WORD *)(DialogBoxString + lstrlenW(DialogBoxString) + 1);
	*DialogBoxPtr++ = 0;
	if (((DWORD)DialogBoxPtr)%4) *DialogBoxPtr++ = 0;

	return(DialogBoxPtr);
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Misc_MessageBoxExtProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MESSAGEBOXSTRUCT *MsgStruct;

	if (uMsgId == WM_INITDIALOG)
		{
		Misc_MessageBoxExtInitialise(hDlg,(MESSAGEBOXSTRUCT *)lParam);
		SetWindowLong(hDlg,DWL_USER,(LONG)lParam);
		return(TRUE);
		}

	MsgStruct = (MESSAGEBOXSTRUCT *)GetWindowLong(hDlg,DWL_USER);
	if (!MsgStruct) return(FALSE);

	switch (uMsgId)
		{
		case WM_DRAWITEM:
			Misc_MessageBoxExtDrawItem((DRAWITEMSTRUCT *)lParam,MsgStruct);
			return(TRUE);

		case WM_CLOSE:
			EndDialog(hDlg,0);
			return(TRUE);

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					if (LOWORD(wParam) < 1000)
						{
						int ReturnCode;

						ReturnCode = LOWORD(wParam);
						if ((ReturnCode == MsgStruct->numgadgets)&&(MsgStruct->numgadgets > 1)) ReturnCode = 0;
						if (!Misc_MessageBoxExtReset(hDlg,ReturnCode,MsgStruct)) break;
						EndDialog(hDlg,ReturnCode);
						return(TRUE);
						}
					if ((LOWORD(wParam) == 1200)&&(MsgStruct->numgadgets > 1))
						{
						BOOL Activate;
						int  i;

						Activate = (IsDlgButtonChecked(hDlg,1200) == BST_UNCHECKED);
						for (i = 0; i != MsgStruct->numgadgets+1; i++)
							{
							if ((!Activate)&&(i == MsgStruct->defaultgadget))
								EnableWindow(GetDlgItem(hDlg,i),TRUE);
							else
								EnableWindow(GetDlgItem(hDlg,i),Activate);
							}
						return(TRUE);
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void Misc_MessageBoxExtInitialise(HWND hDlg, MESSAGEBOXSTRUCT *MsgStruct)
{
	HDC		 hDC;
	HWND		 hCtrl;
	HFONT		 hFont;
	HFONT		 hOldFont;
	LONG		 TempValue;
	UINT		 ResourceIdentifier;
	DWORD		 DialogFlags;
	LONG		 DialogLimitsHeight;
	LONG		 DialogWidth;
	LONG		 DialogHeight;
	LONG		 DialogTextHeight;
	LONG		 CharsetCount;
	LONG		 GadgetsRealWidth;
	LONG		 GadgetsWidth;
	RECT		 RequestRect;
	RECT		 GadgetRect;
	SIZE		 GadgetSize;
	SIZE		 FontSize;
	TEXTMETRIC	 FontInfo;
	char		*TextPtr;
	char		*PrevPtr;
	char		*GadgetText;
	long		 X,Y;
	int		 i;

//--- Calcul la taille de la fenêtre ---

	hDC = CreateCompatibleDC(NULL);
	if (!hDC)
		{
		EndDialog(hDlg,-1);
		return;
		}
	hFont = CreateFont(-MulDiv(MsgStruct->textfontsize,GetDeviceCaps(hDC,LOGPIXELSY),72),0,0,0,0,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,MsgStruct->textfont);
	if (!hFont)
		{
		DeleteDC(hDC);
		EndDialog(hDlg,-1);
		return;
		}
	hOldFont = SelectObject(hDC,hFont);
	GetTextMetrics(hDC,&FontInfo);

	/* Taille du texte de la requête */

	DialogFlags = MsgStruct->internalflags;
	DialogWidth = 0;
	DialogHeight = 0;
	DialogTextHeight = 0;
	DialogLimitsHeight = 0;
	FontSize.cy = 0;

	if (!(DialogFlags&MBF_NOTEXT))
		{
		for (TextPtr = MsgStruct->windowtext, PrevPtr = MsgStruct->windowtext, CharsetCount = 0; *TextPtr != 0x00; TextPtr++)
			{
			CharsetCount++;
			if (*TextPtr == 0x0A)
				{
				--CharsetCount;
				if (PrevPtr != TextPtr)
					{
					GetTextExtentPoint32(hDC,PrevPtr,CharsetCount,&FontSize);
					if (FontSize.cx > DialogWidth)
						DialogWidth = (WORD)(FontSize.cx);
					DialogTextHeight += (WORD)FontSize.cy;
					}
				else
					DialogTextHeight += FontInfo.tmExternalLeading+FontInfo.tmHeight;

				CharsetCount = 0;
				PrevPtr = TextPtr+1;
				}
			}
		if (CharsetCount)
			{
			GetTextExtentPoint32(hDC,PrevPtr,CharsetCount,&FontSize);
			if (FontSize.cx > DialogWidth)
				DialogWidth = (WORD)(FontSize.cx);
			DialogTextHeight += (WORD)FontSize.cy;
			}

		if ((!(DialogFlags&MBF_NOICON)) && (DialogTextHeight < 32))
			DialogTextHeight = 32;

		FontSize.cx = 0;
		FontSize.cy = 0;
		DialogTextHeight += 20;
		DialogHeight += DialogTextHeight;

		if (DialogFlags&MBF_AUTOREPLY)
			{
			GetTextExtentPoint32(hDC,szMessageBoxExtDoNotDisplayAgain,strlen(szMessageBoxExtDoNotDisplayAgain),&FontSize);
			if (DialogWidth < 32+FontSize.cx) DialogWidth = (WORD)(32+FontSize.cx);
			DialogHeight += FontSize.cy+10;
			FontSize.cy += 10;
			}

		DialogWidth += 64+32;
		}

	if (DialogFlags&MBF_EDITTEXT)
		{
		FontSize.cx = FontInfo.tmAveCharWidth;
		FontSize.cy = FontInfo.tmExternalLeading+FontInfo.tmHeight+8+16;
		TempValue = 64+32+FontSize.cx*16+GetSystemMetrics(SM_CXDLGFRAME)*2;
		if (DialogWidth < TempValue) DialogWidth = (WORD)TempValue;
		if (DialogWidth < MsgStruct->editwidth+64) DialogWidth = (WORD)MsgStruct->editwidth+64;
		DialogHeight += FontSize.cy;
		}

	if ((DialogFlags&MBF_NUMBER)&&(!(DialogFlags&MBF_NOMINMAX)))
		{
		DialogLimitsHeight = FontInfo.tmExternalLeading+FontInfo.tmHeight+6+10;
		TempValue = 64+32+50*2+4*2+MsgStruct->numberwidth*2+8+GetSystemMetrics(SM_CXDLGFRAME)*2;
		if (DialogWidth < TempValue) DialogWidth = (WORD)TempValue;
		DialogHeight += DialogLimitsHeight;
		}

//--- Dimensionne les gadgets ---

	if (!(DialogFlags&MBF_NOGADGETS))
		{
		GadgetText = GlobalAlloc(GPTR,2048);
		if (!GadgetText)
			{
			SelectObject(hDC,hOldFont);
			DeleteObject(hFont);
			DeleteDC(hDC);
			EndDialog(hDlg,-1);
			return;
			}

		for (GadgetsRealWidth = 0, GadgetsWidth = 0, i = 1; i != MsgStruct->numgadgets+1; i++)
			{
			hCtrl = GetDlgItem(hDlg,i);
			GetWindowText(hCtrl,GadgetText,2047);
			GetTextExtentPoint32(hDC,GadgetText,strlen(GadgetText),&GadgetSize);
			if (GadgetSize.cx < FontInfo.tmAveCharWidth*6) GadgetSize.cx = FontInfo.tmAveCharWidth*6;
			SetWindowPos(hCtrl,NULL,8,DialogTextHeight+FontSize.cy+DialogLimitsHeight+8,GadgetSize.cx+16,GadgetSize.cy+12,SWP_NOREDRAW|SWP_NOZORDER);
			GetWindowRect(hCtrl,&GadgetRect);
			GadgetsWidth += (GadgetRect.right-GadgetRect.left)+8;
			GadgetsRealWidth += GadgetRect.right-GadgetRect.left;
			}
		GadgetsWidth -= 8;

		if (GadgetsWidth+16 > DialogWidth) DialogWidth = GadgetsWidth+16;
		DialogHeight += GadgetSize.cy+12+8*2;
		}

	GlobalFree(GadgetText);
	SelectObject(hDC,hOldFont);
	DeleteObject(hFont);
	DeleteDC(hDC);

//--- Place les différents éléments de la fenêtre ---

	if (!(DialogFlags&MBF_NOICON)) SetWindowPos(GetDlgItem(hDlg,1000),NULL,14,10,32,32,SWP_NOREDRAW|SWP_NOZORDER);
	if (!(DialogFlags&MBF_NOTEXT)) SetWindowPos(GetDlgItem(hDlg,1100),NULL,(DialogFlags&MBF_NOICON)?20:64,8,(DialogFlags&MBF_NOICON)?(DialogWidth-40):(DialogWidth-84),DialogTextHeight,SWP_NOREDRAW|SWP_NOZORDER);

	if (DialogFlags&MBF_EDITTEXT)
		SetWindowPos(GetDlgItem(hDlg,1400),NULL,20,DialogTextHeight+8,DialogWidth-40,FontSize.cy-16,SWP_NOREDRAW|SWP_NOZORDER);
	else
		SetWindowPos(GetDlgItem(hDlg,1200),NULL,(DialogFlags&MBF_NOICON)?20:64,DialogTextHeight,(DialogFlags&MBF_NOICON)?(DialogWidth-40):(DialogWidth-84),FontSize.cy,SWP_NOREDRAW|SWP_NOZORDER);

	if ((DialogFlags&MBF_NUMBER)&&(!(DialogFlags&MBF_NOMINMAX)))
		{
		LONG	StartX;
		LONG	StartY;
		LONG	Height;

		StartX = (DialogWidth-(50*2+4*2+MsgStruct->numberwidth*2+8))/2;
		StartY =  DialogTextHeight+FontSize.cy;
		Height = FontInfo.tmExternalLeading+FontInfo.tmHeight+6;
		SetWindowPos(GetDlgItem(hDlg,1300),NULL,StartX,StartY,50,Height,SWP_NOREDRAW|SWP_NOZORDER);
		StartX += 54;
		SetWindowPos(GetDlgItem(hDlg,1302),NULL,StartX,StartY,MsgStruct->numberwidth,Height,SWP_NOREDRAW|SWP_NOZORDER);
		StartX += MsgStruct->numberwidth+8;
		SetWindowPos(GetDlgItem(hDlg,1301),NULL,StartX,StartY,50,Height,SWP_NOREDRAW|SWP_NOZORDER);
		StartX += 54;
		SetWindowPos(GetDlgItem(hDlg,1303),NULL,StartX,StartY,MsgStruct->numberwidth,Height,SWP_NOREDRAW|SWP_NOZORDER);
		}

//--- Place les gadgets (axe X) ---

	SetWindowPos(hDlg,NULL,0,0,DialogWidth+GetSystemMetrics(SM_CXDLGFRAME)*2,DialogHeight+GetSystemMetrics(SM_CYDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),SWP_NOREDRAW|SWP_NOMOVE|SWP_NOZORDER);
	switch(MsgStruct->windowpos)
		{
		case MBP_FOLLOWMOUSE:
			Window_UnderCursor(hDlg,&X,&Y);
			break;
		case MBP_CUSTOM:
			X = MsgStruct->x;
			Y = MsgStruct->y;
			Window_CorrectPos(hDlg,&X,&Y);
			break;
		default:
			Window_Center(hDlg,GetParent(hDlg),&X,&Y);
			break;
		}
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);

	if (!(DialogFlags&MBF_NOGADGETS))
		{
		GetClientRect(hDlg,&RequestRect);
		if (MsgStruct->numgadgets == 1)
			{
			hCtrl = GetDlgItem(hDlg,1);
			GetWindowRect(hCtrl,&GadgetRect);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.left);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.right);
			switch(MsgStruct->justification)
				{
				case MBJ_LEFT:
					X = RequestRect.right-(GadgetRect.right-GadgetRect.left)-8;
					break;
				case MBJ_RIGHT:
					X = 8;
					break;
				default:X = (((RequestRect.right-RequestRect.left-16)-(GadgetRect.right-GadgetRect.left))/2)+8;
					break;
				}
			SetWindowPos(hCtrl,NULL,X,GadgetRect.top,0,0,SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);
			goto Set_Icon;
			}

		if (MsgStruct->numgadgets == 2)
			{
			hCtrl = GetDlgItem(hDlg,1);
			GetWindowRect(hCtrl,&GadgetRect);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.left);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.right);
			X = 8;
			SetWindowPos(hCtrl,NULL,X,GadgetRect.top,0,0,SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);

			hCtrl = GetDlgItem(hDlg,2);
			GetWindowRect(hCtrl,&GadgetRect);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.left);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.right);
			X = RequestRect.right-(GadgetRect.right-GadgetRect.left)-8;
			SetWindowPos(hCtrl,NULL,X,GadgetRect.top,0,0,SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);
			goto Set_Icon;
			}

		for (X = 8, i = 1; i != MsgStruct->numgadgets+1; i++)
			{
			hCtrl = GetDlgItem(hDlg,i);
			GetWindowRect(hCtrl,&GadgetRect);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.left);
			ScreenToClient(hDlg,(POINT *)&GadgetRect.right);
			SetWindowPos(hCtrl,NULL,X,GadgetRect.top,0,0,SWP_NOREDRAW|SWP_NOSIZE|SWP_NOZORDER);
			X += GadgetRect.right-GadgetRect.left;
			X += (RequestRect.right-RequestRect.left-16-GadgetsRealWidth)/(MsgStruct->numgadgets-1);
			}
		}

//--- Initialise l'icône de la requête ---

Set_Icon:
	if (!(MsgStruct->flags&MBF_NOBEEP))
		{
		switch(MsgStruct->defaulticon)
			{
			case MB_ICONINFORMATION:
				MessageBeep(MB_ICONINFORMATION);
				break;
			case MB_ICONEXCLAMATION:
				MessageBeep(MB_ICONEXCLAMATION);
				break;
			case MB_ICONHAND:
				MessageBeep(MB_ICONHAND);
				break;
			case MB_ICONQUESTION:
				MessageBeep(MB_ICONQUESTION);
				break;
			default:
				MessageBeep(MB_OK);
				break;
			}
		}

	if (DialogFlags&MBF_USEICON)
		goto Set_Default;

	if (DialogFlags&MBF_NOICON)
		ResourceIdentifier = 0;
	else
		switch(MsgStruct->defaulticon)
			{
			case MB_ICONINFORMATION:
				ResourceIdentifier = 80;
				break;
			case MB_ICONEXCLAMATION:
				ResourceIdentifier = 81;
				break;
			case MB_ICONHAND:
				ResourceIdentifier = 82;
				break;
			case MB_ICONQUESTION:
				ResourceIdentifier = 83;
				break;
			default:
				ResourceIdentifier = 0;
				break;
			}

	MsgStruct->internalflags |= MBF_DESTROYICON;
	MsgStruct->windowicon = NULL;
	if ((ResourceIdentifier)&&(!(DialogFlags&MBF_NOTEXT)))
		MsgStruct->windowicon = LoadImage(hInstance,MAKEINTRESOURCE(ResourceIdentifier),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);

//--- Place les valeurs par défaut ---

Set_Default:
	if ((DialogFlags&MBF_NUMBER)&&(!(DialogFlags&MBF_NOMINMAX)))
		{
		SetDlgItemInt(hDlg,1302,MsgStruct->min,TRUE);
		SetDlgItemInt(hDlg,1303,MsgStruct->max,TRUE);
		}

	if (DialogFlags&MBF_NUMBER)
		SetDlgItemInt(hDlg,1400,MsgStruct->editnumber,TRUE);
	else if ((DialogFlags&MBF_EDITTEXT)&&(MsgStruct->editbuffer))
		SetDlgItemText(hDlg,1400,MsgStruct->editbuffer);

	if (DialogFlags&MBF_EDITTEXT)
		{
		SetFocus(GetDlgItem(hDlg,1400));
		SendDlgItemMessage(hDlg,1400,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
		}
	else if (!(DialogFlags&MBF_NOGADGETS))
		SetFocus(GetDlgItem(hDlg,1));

	if (!(DialogFlags&MBF_AUTOREPLY))
		ShowWindow(GetDlgItem(hDlg,1200),SW_HIDE);

	return;
}


// «»»» Suppression des données allouées lors de l'initialisation «««««««»

int Misc_MessageBoxExtReset(HWND hDlg, int ReturnCode, MESSAGEBOXSTRUCT *MsgStruct)
{
	LONG	TextLen;

	if ((MsgStruct->internalflags&MBF_AUTOREPLY)&&(ReturnCode == MsgStruct->defaultgadget))
		{
		*MsgStruct->requestbits &= ~MsgStruct->statebit;
		if (IsDlgButtonChecked(hDlg,1200) == BST_CHECKED)
			*MsgStruct->requestbits |= MsgStruct->statebit;
		}

	if (!ReturnCode) goto Free_Datas;

	if (MsgStruct->flags&MBF_EDITTEXT)
		{
		if (MsgStruct->flags&MBF_NUMBER)
			{
			MsgStruct->editnumber = GetDlgItemInt(hDlg,1400,NULL,TRUE);
			if (MsgStruct->editnumber < MsgStruct->min)
				{
				if (!(MsgStruct->flags&MBF_NOOUTOFRANGEREQ)) Misc_MessageBoxExt(hDlg,szMessageBoxExtOutOfRangeLowErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
				else MessageBeep(MB_ICONEXCLAMATION);
				SetFocus(GetDlgItem(hDlg,1400));
				SendDlgItemMessage(hDlg,1400,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
				return(0);
				}
			if (MsgStruct->editnumber > MsgStruct->max)
				{
				if (!(MsgStruct->flags&MBF_NOOUTOFRANGEREQ)) Misc_MessageBoxExt(hDlg,szMessageBoxExtOutOfRangeHighErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
				else MessageBeep(MB_ICONEXCLAMATION);
				SetFocus(GetDlgItem(hDlg,1400));
				SendDlgItemMessage(hDlg,1400,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
				return(0);
				}
			}
		else
			{
			TextLen = SendDlgItemMessage(hDlg,1400,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0);
			TextLen++;
			if (MsgStruct->editbuffer)
				{
				if ((TextLen > MsgStruct->editbuffersize)&&(!(MsgStruct->flags&MBF_NOOVERFLOWREQ)))
					{
					TextLen = Misc_MessageBoxExt(hDlg,szMessageBoxExtBufferTooSmallErr,NULL,szMessageBoxExtYesNo,MB_ICONEXCLAMATION);
					if (!TextLen) return(0);
					}
				ZeroMemory(MsgStruct->editbuffer,MsgStruct->editbuffersize);
				}
			else
				{
				MsgStruct->editbuffer = GlobalAlloc(GPTR,TextLen);
				if (!MsgStruct->editbuffer)
					{
					MessageBeep(MB_ICONHAND);
					return(0);
					}
				MsgStruct->editbuffersize = TextLen;
				}
			GetDlgItemText(hDlg,1400,MsgStruct->editbuffer,MsgStruct->editbuffersize);
			}
		}

Free_Datas:
	if ((MsgStruct->windowicon)&&(MsgStruct->internalflags&MBF_DESTROYICON))
		{
		DestroyIcon(MsgStruct->windowicon);
		MsgStruct->windowicon = NULL;
		}
	return(1);
}


// «»»» Affichage de l'icône ««««««««««««««««««««««««««««««««««««««««««««»

void Misc_MessageBoxExtDrawItem(DRAWITEMSTRUCT *ItemStruct, MESSAGEBOXSTRUCT *MsgStruct)
{
	if (ItemStruct->CtlType != ODT_BUTTON)
		return;
	if (ItemStruct->CtlID != 1000)
		return;
	if (!MsgStruct->windowicon)
		return;

	DrawIconEx(ItemStruct->hDC,(ItemStruct->rcItem.right-32)/2,(ItemStruct->rcItem.bottom-32)/2,MsgStruct->windowicon,32,32,NULL,NULL,DI_NORMAL);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection d'un fichier						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

//--- En vue de la sauvegarde d'un fichier ---

int Misc_FileSelectSave(HWND hWnd, char *Name, UINT FileType)
{
	FILEHEADER	 FileHeader;
	OPENFILENAME	 Request;
	char		*Title;
	char		*Filter;
	char		*Extension;
	char		*FileName;

	FileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!FileName) return(0);

	ZeroMemory(&FileHeader,sizeof(FILEHEADER));
	FileHeader.type = FileType;
	Request.nFilterIndex = 1;

	switch(FileType)
		{
		case FILE_MAP:
			Title = szFileReqMapSaveTitle;
			Filter = szFileReqMapSaveFilter;
			Extension = szFileReqMapDefExt;
			if (Files_GetMapTypeFromName(Name) == MAPFILE_TEXT) Request.nFilterIndex = 2;
			break;
		case FILE_AREA:
			Title = szFileReqAreaSaveTitle;
			Filter = szFileReqAreaFilter;
			Extension = szFileReqAreaDefExt;
			break;
		case FILE_THUMBNAIL:
			Title = szFileReqThumbSaveTitle;
			Filter = szFileReqBrushFilter;
			Extension = szFileReqBrushDefExt;
			break;
		}

	strcpy(FileName,Name);
	Request.lStructSize = sizeof(OPENFILENAME);
	Request.hwndOwner = hWnd;
	Request.hInstance = hInstance;
	Request.lpstrFilter = Filter;
	Request.lpstrCustomFilter = NULL;
	Request.nMaxCustFilter = 0;
	Request.lpstrFile = FileName;
	Request.nMaxFile = MAX_PATH;
	Request.lpstrFileTitle = NULL;
	Request.nMaxFileTitle = 0;
	Request.lpstrInitialDir = NULL;
	Request.lpstrTitle = Title;
	Request.Flags = OFN_EXPLORER|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON|OFN_ENABLETEMPLATE|OFN_ENABLEHOOK;
	Request.nFileOffset = 0;
	Request.nFileExtension = 0;
	Request.lpstrDefExt = Extension;
	Request.lCustData = (DWORD)&FileHeader;
	Request.lpfnHook = (LPOFNHOOKPROC)Misc_FileSelectProc;
	Request.lpTemplateName = MAKEINTRESOURCE(6000);

	if (!GetSaveFileName(&Request))
		{
		GlobalFree(FileName);
		return(0);
		}

	strcpy(Name,FileName);
	GlobalFree(FileName);
	return(Request.nFilterIndex);
}

//--- En vue du chargement du fichier ---

int Misc_FileSelect(HWND hWnd, char *Name, UINT FileType)
{
	FILEHEADER	 FileHeader;
	OPENFILENAME	 Request;
	UINT		 Type;
	char		*Title;
	char		*Filter;
	char		*Extension;
	char		*FileName;

	FileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!FileName) return(0);

	ZeroMemory(&FileHeader,sizeof(FILEHEADER));
	FileHeader.type = FileType;
	Request.nFilterIndex = 1;

	switch(FileType)
		{
		case FILE_MAP:
			Title = szFileReqMapTitle;
			Filter = szFileReqMapFilter;
			Extension = szFileReqMapDefExt;
			Type = Files_GetMapTypeFromName(Name);
			switch(Type)
				{
				case MAPFILE_EDITOR:
					Request.nFilterIndex = 2;
					break;
				case MAPFILE_GAME:
					Request.nFilterIndex = 3;
					break;
				case MAPFILE_TEXT:
					Request.nFilterIndex = 4;
					break;
				}
			break;
		case FILE_AREA:
			Title = szFileReqAreaTitle;
			Filter = szFileReqAreaFilter;
			Extension = szFileReqAreaDefExt;
			break;
		case FILE_BRUSH:
			Title = szFileReqBrushTitle;
			Filter = szFileReqBrushFilter;
			Extension = szFileReqBrushDefExt;
			break;
		}

	strcpy(FileName,Name);
	Request.lStructSize = sizeof(OPENFILENAME);
	Request.hwndOwner = hWnd;
	Request.hInstance = hInstance;
	Request.lpstrFilter = Filter;
	Request.lpstrCustomFilter = NULL;
	Request.nMaxCustFilter = 0;
	Request.lpstrFile = FileName;
	Request.nMaxFile = MAX_PATH;
	Request.lpstrFileTitle = NULL;
	Request.nMaxFileTitle = 0;
	Request.lpstrInitialDir = NULL;
	Request.lpstrTitle = Title;
	Request.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_NONETWORKBUTTON|OFN_READONLY|OFN_ENABLETEMPLATE|OFN_ENABLEHOOK;
	Request.nFileOffset = 0;
	Request.nFileExtension = 0;
	Request.lpstrDefExt = Extension;
	Request.lCustData = (DWORD)&FileHeader;
	Request.lpfnHook = (LPOFNHOOKPROC)Misc_FileSelectProc;
	Request.lpTemplateName = MAKEINTRESOURCE(6000);

	if (!GetOpenFileName(&Request))
		{
		GlobalFree(FileName);
		return(0);
		}

	strcpy(Name,FileName);
	GlobalFree(FileName);
	return(Request.nFilterIndex);
}


// «»»» Procédure de sélection des fichiers «««««««««««««««««««««««««««««»

#pragma argsused
BOOL CALLBACK Misc_FileSelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	FILEHEADER	*FileHeader;

	if (uMsgId == WM_INITDIALOG)
		{
		FileHeader = (FILEHEADER *)(((OPENFILENAME *)lParam)->lCustData);
		SetWindowLong(hDlg,DWL_USER,(LONG)FileHeader);
		return(TRUE);
		}

	FileHeader = (FILEHEADER *)GetWindowLong(hDlg,DWL_USER);
	if (!FileHeader) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Misc_FileDraw((DRAWITEMSTRUCT *)lParam,FileHeader);
			break;

		case WM_NOTIFY:
			switch(((OFNOTIFY *)lParam)->hdr.code)
				{
				case CDN_SELCHANGE:
					Misc_FileGet(hDlg,FileHeader);
					break;
				case CDN_FOLDERCHANGE:
					break;
				}
			break;

		case WM_DESTROY:
			if (FileHeader->bitmap) DeleteObject(FileHeader->bitmap);
			FileHeader->bitmap = NULL;
			break;
		}

	return(FALSE);
}


// «»»» Sélection d'une nouvelle image ««««««««««««««««««««««««««««««««««»

void Misc_FileGet(HWND hDlg, FILEHEADER *FileHeader)
{
	MAP		*Map;
	BYTE		*MapPtr = NULL;
	MAPSTATS	*MapStats;
	MAPINFO		*MapInfo;
	DWORD		 MapSize;
	HANDLE		 FileHD;
	BITMAP		 BitmapInfo;
	char		 FileName[MAX_PATH];

	// Check if file exists....

	SendMessage(GetParent(hDlg),CDM_GETFILEPATH,(WPARAM)MAX_PATH,(LPARAM)FileName);
	FileHD = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (FileHD == INVALID_HANDLE_VALUE) return;
	CloseHandle(FileHD);

	// Destroy previous image....

	if (FileHeader->bitmap) DeleteObject(FileHeader->bitmap);
	FileHeader->bitmap = NULL;
	FileHeader->width = 0;
	FileHeader->height = 0;
	Compiler_SetLastError(0);

	Map = GlobalAlloc(GPTR,sizeof(MAP));
	if (!Map)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		SetDlgItemText(hDlg,110,Compiler_GetErrorCodeText());
		return;
		}
	MapStats = GlobalAlloc(GPTR,sizeof(MAPSTATS));
	if (!MapStats)
		{
		GlobalFree(Map);
		Compiler_SetLastError(COMPILERR_WINDOWS);
		SetDlgItemText(hDlg,110,Compiler_GetErrorCodeText());
		return;
		}

	switch(FileHeader->type)
		{
		case FILE_MAP:
			switch(Files_GetMapTypeFromName(FileName))
				{
				case MAPFILE_GAME:
					MapInfo = Game_ExtractMap(FileName,NULL);
					if (!MapInfo) break;
					Map->Map = MapInfo->Map;
					Map->Properties.Width = MapInfo->Width;
					Map->Properties.Height = MapInfo->Height;
					FileHeader->width = MapInfo->Width;
					FileHeader->height = MapInfo->Height;
					FileHeader->bitmap = Thumbnail_Generate(hDlg,Map,NULL);
					GlobalFree(MapInfo->Map);
					GlobalFree(MapInfo);
					Game_ExtractThings(FileName,NULL,MapStats,FALSE);
					break;
				case MAPFILE_EDITOR:
					MapPtr = Editor_ReadFile(FileName,&MapSize);
					if (!MapPtr) break;
					MapInfo = Editor_ExtractMap(MapPtr,MapSize,NULL);
					if (!MapInfo) break;
					Map->Map = MapInfo->Map;
					Map->Properties.Width = MapInfo->Width;
					Map->Properties.Height = MapInfo->Height;
					FileHeader->width = MapInfo->Width;
					FileHeader->height = MapInfo->Height;
					FileHeader->bitmap = Thumbnail_Generate(hDlg,Map,NULL);
					GlobalFree(MapInfo->Map);
					GlobalFree(MapInfo);
					Editor_ExtractThings(MapPtr,MapSize,NULL,MapStats,FALSE);
					break;
				case MAPFILE_TEXT:
					MapPtr = (BYTE *)Text_ReadFile(FileName,&MapSize);
					if (!MapPtr) break;
					MapInfo = Text_ExtractMap((char *)MapPtr,MapSize,NULL);
					if (!MapInfo) break;
					Map->Map = MapInfo->Map;
					Map->Properties.Width = MapInfo->Width;
					Map->Properties.Height = MapInfo->Height;
					FileHeader->width = MapInfo->Width;
					FileHeader->height = MapInfo->Height;
					FileHeader->bitmap = Thumbnail_Generate(hDlg,Map,NULL);
					GlobalFree(MapInfo->Map);
					GlobalFree(MapInfo);
					break;
				}
			break;

		case FILE_AREA:
			MapInfo = Areas_ExtractMap(FileName);
			if (!MapInfo) break;
			Map->Map = MapInfo->Map;
			Map->Properties.Width = MapInfo->Width;
			Map->Properties.Height = MapInfo->Height;
			FileHeader->width = MapInfo->Width;
			FileHeader->height = MapInfo->Height;
			FileHeader->bitmap = Thumbnail_Generate(hDlg,Map,NULL);
			GlobalFree(MapInfo->Map);
			GlobalFree(MapInfo);
			Areas_ExtractThings(FileName,NULL,MapStats);
			break;

		case FILE_BRUSH:
			break;

		case FILE_THUMBNAIL:
			FileHeader->bitmap = LoadImage(NULL,FileName,IMAGE_BITMAP,0,0,LR_DEFAULTCOLOR|LR_LOADFROMFILE);
			if (!FileHeader->bitmap)
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				break;
				}
			GetObject(FileHeader->bitmap,sizeof(BITMAP),&BitmapInfo);
			FileHeader->width = BitmapInfo.bmWidth;
			FileHeader->height = BitmapInfo.bmHeight;
			break;
		}

	if (MapPtr) GlobalFree(MapPtr);
	SetDlgItemText(hDlg,110,Compiler_GetErrorCodeText());
	SetDlgItemInt(hDlg,101,FileHeader->width,FALSE);
	SetDlgItemInt(hDlg,102,FileHeader->height,FALSE);
	SetDlgItemInt(hDlg,103,MapStats->Creatures,FALSE);
	SetDlgItemInt(hDlg,104,MapStats->Doors,FALSE);
	SetDlgItemInt(hDlg,105,MapStats->Traps,FALSE);
	SetDlgItemInt(hDlg,106,MapStats->Objects,FALSE);
	SetDlgItemInt(hDlg,107,MapStats->MagicalObjects,FALSE);
	GlobalFree(Map);
	GlobalFree(MapStats);
	InvalidateRect(GetDlgItem(hDlg,100),NULL,FALSE);
	UpdateWindow(GetDlgItem(hDlg,100));
	return;
}


// «»»» Affichage de l'image ««««««««««««««««««««««««««««««««««««««««««««»

void Misc_FileDraw(DRAWITEMSTRUCT *Item, FILEHEADER *FileHeader)
{
	HDC	TempDC;
	HBITMAP OldBitmap;
	RECT	DrawRect;
	LONG	Size;
	LONG	Prop;
	LONG	X,Y;

	DrawFrameControl(Item->hDC,&Item->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
	CopyRect(&DrawRect,&Item->rcItem);
	DrawRect.left += 4;
	DrawRect.right -= 4;
	DrawRect.top += 4;
	DrawRect.bottom -= 4;

	if (!FileHeader->bitmap) return;

	TempDC = CreateCompatibleDC(Item->hDC);
	if (TempDC)
		{
		OldBitmap = SelectObject(TempDC,FileHeader->bitmap);
		if (OldBitmap)
			{
			Size = DrawRect.right-DrawRect.left;
			if (Size > DrawRect.bottom-DrawRect.top) Size = DrawRect.bottom-DrawRect.top;
			X = DrawRect.left+(DrawRect.right-DrawRect.left-Size)/2;
			Y = DrawRect.top+(DrawRect.bottom-DrawRect.top-Size)/2;

			if (FileHeader->width != FileHeader->height)
				{
				if (FileHeader->width < FileHeader->height)
					{
					Prop = (long)((float)Size*((float)FileHeader->width/(float)FileHeader->height));
					BitBlt(Item->hDC,X,Y,(Size-Prop)/2,Size,NULL,0,0,BLACKNESS);
					BitBlt(Item->hDC,X+(Size-Prop)/2+Prop,Y,(Size-Prop)/2,Size,NULL,0,0,BLACKNESS);
					StretchBlt(Item->hDC,X+(Size-Prop)/2,Y,Prop,Size,TempDC,0,0,FileHeader->width,FileHeader->height,SRCCOPY);
					}
				else // if (FileHeader->width > FileHeader->height)
					{
					Prop = (long)((float)Size*((float)FileHeader->height/(float)FileHeader->width));
					BitBlt(Item->hDC,X,Y,Size,(Size-Prop)/2,NULL,0,0,BLACKNESS);
					BitBlt(Item->hDC,X,Y+(Size-Prop)/2+Prop,Size,(Size-Prop)/2,NULL,0,0,BLACKNESS);
					StretchBlt(Item->hDC,X,Y+(Size-Prop)/2,Size,Prop,TempDC,0,0,FileHeader->width,FileHeader->height,SRCCOPY);
					}
				}
			else
				StretchBlt(Item->hDC,X,Y,Size,Size,TempDC,0,0,FileHeader->width,FileHeader->height,SRCCOPY);

			SelectObject(TempDC,OldBitmap);
			}
		DeleteDC(TempDC);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
