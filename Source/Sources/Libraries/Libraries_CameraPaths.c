
//<<>-<>>---------------------------------------------------------------------()
/*
	Description des chemins de caméra
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données de la bibliothèque					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Includes ***********************************************************/

#define CAMERAPATHDLL
#include <windows.h>
#include <commctrl.h>
#include "..\..\Sources\Datas\CameraPaths.h"

/*** Variables **********************************************************/

HINSTANCE	hInstance;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes des fonctions privées					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK		Editor_SelectProc(HWND,UINT,WPARAM,LPARAM);
void			Editor_SelectInit(HWND,CAMERAPATH *);
void			Editor_SelectChangePath(HWND,CAMERAPATH *);
void			Editor_SelectPaintBitmap(DRAWITEMSTRUCT *,CAMERAPATH *);
void			Editor_PrintError(HWND,char *,char *,UINT);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
// ¤¤									      ¤¤
// ¤¤ DllEntryPoint - Fonction appelée par Windows			      ¤¤
// ¤¤									      ¤¤
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

#pragma argsused
BOOL WINAPI DllEntryPoint(HINSTANCE hDLLInstance, DWORD Reason, LPVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		{
		hInstance = hDLLInstance;
		DisableThreadLibraryCalls(hDLLInstance);
		InitCommonControls();
		}

	return(TRUE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions publiques						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Test de la version ««««««««««««««««««««««««««««««««««««««««««««««»

long APIENTRY Editor_QueryVersion()
{
	return(37);
}


// «»»» Sélection d'un chemin de caméra «««««««««««««««««««««««««««««««««»

#pragma argsused
int APIENTRY Editor_SelectCameraPath(HWND hWnd, CAMERAPATH *Path)
{
	WNDCLASSEXA	wndClass;
	int		Result;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_DBLCLKS|CS_SAVEBITS|CS_BYTEALIGNWINDOW;
	wndClass.lpfnWndProc = DefDlgProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = DLGWINDOWEXTRA;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadImage(hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szDialogClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (RegisterClassEx(&wndClass) == 0)
		{
		Editor_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return(0);
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(9000),hWnd,Editor_SelectProc,(LPARAM)Path);
	if ((Result == 0)||(Result == -1))
		{
		if (Result == -1) Editor_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return(0);
		}

	UnregisterClass(szDialogClass,hInstance);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions privées						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Editor_SelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	CAMERAPATH	*Path;

	if (uMsgId == WM_INITDIALOG)
		{
		Editor_SelectInit(hDlg,(CAMERAPATH *)lParam);
		return(TRUE);
		}

	Path = (CAMERAPATH *)GetWindowLong(hDlg,DWL_USER);
	if (!Path) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Editor_SelectPaintBitmap((DRAWITEMSTRUCT *)lParam,Path);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					Editor_SelectChangePath(hDlg,Path);
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
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


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void Editor_SelectInit(HWND hDlg, CAMERAPATH *Path)
{
	char	TempText[8];
	int	i;

	for (i = 0; Description[i].Path != -1; i++)
		{
		wsprintf(TempText,szNumber,Description[i].Path);
		SendDlgItemMessage(hDlg,101,LB_ADDSTRING,(WPARAM)0,(LPARAM)TempText);
		}
	for (i = 0; Description[i].Path != -1; i++)
		{
		if (Description[i].Path == Path->PathId)
			{
			SendDlgItemMessage(hDlg,101,LB_SETCURSEL,(WPARAM)i,(LPARAM)0);
			SetDlgItemText(hDlg,202,Description[i].Description);
			break;
			}
		}

	SetWindowLong(hDlg,DWL_USER,(LONG)Path);
	return;
}


// «»»» Changement de chemin ««««««««««««««««««««««««««««««««««««««««««««»

void Editor_SelectChangePath(HWND hDlg, CAMERAPATH *Path)
{
	int	CurrentPath;

	CurrentPath = SendDlgItemMessage(hDlg,101,LB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	Path->PathId = Description[CurrentPath].Path;
	Path->ShowCell = Description[CurrentPath].ShowCell;
	SetDlgItemText(hDlg,202,Description[CurrentPath].Description);

	InvalidateRect(GetDlgItem(hDlg,201),NULL,FALSE);
	return;
}


// «»»» Affichage d'une image «««««««««««««««««««««««««««««««««««««««««««»

void Editor_SelectPaintBitmap(DRAWITEMSTRUCT *ItemStruct, CAMERAPATH *Path)
{
	HDC	hDC;
	HBITMAP	hBitmap,hOldBitmap;
	RECT	ClientRect;
	int	i;

	//
	// --- Recherche si le chemin est répertorié ---
	//

	for (i = 0; Description[i].Path != -1; i++)
		if (Description[i].Path == Path->PathId)
			break;

	if (Description[i].Path == -1) goto Draw_BlackRect;
	if (!Description[i].Bitmap) goto Draw_BlackRect;

	//
	// --- Chargement de l'image & création du contexte d'affichage ---
	//

	hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(Description[i].Bitmap),IMAGE_BITMAP,256,192,LR_DEFAULTCOLOR);
	if (!hBitmap) goto Draw_BlackRect;
	hDC = CreateCompatibleDC(ItemStruct->hDC);
	if (!hDC) goto Draw_BlackRect1;
	hOldBitmap = SelectObject(hDC,hBitmap);
	if (!hOldBitmap) goto Draw_BlackRect2;

	//
	// --- Affichage de l'image
	//

	CopyRect(&ClientRect,&ItemStruct->rcItem);
	DrawEdge(ItemStruct->hDC,&ItemStruct->rcItem,EDGE_SUNKEN,BF_RECT);
	ClientRect.left += 2;
	ClientRect.top += 2;
	ClientRect.right -= 2;
	ClientRect.bottom -= 2;
	if ((ClientRect.right-ClientRect.left > 256)&&(ClientRect.bottom-ClientRect.top > 192))
		{
		BitBlt(ItemStruct->hDC,ClientRect.left,ClientRect.top,ClientRect.right-ClientRect.left,ClientRect.bottom-ClientRect.top,NULL,0,0,BLACKNESS);
		ClientRect.left = ClientRect.left+(ClientRect.right-ClientRect.left-256)/2;
		ClientRect.top = ClientRect.top+(ClientRect.bottom-ClientRect.top-192)/2;
		BitBlt(ItemStruct->hDC,ClientRect.left,ClientRect.top,256,192,hDC,0,0,SRCCOPY);
		}
	else
		StretchBlt(ItemStruct->hDC,ClientRect.left,ClientRect.top,ClientRect.right-ClientRect.left,ClientRect.bottom-ClientRect.top,hDC,0,0,256,192,SRCCOPY);

	//
	// --- Sortie ---
	//

	SelectObject(hDC,hOldBitmap);
	DeleteDC(hDC);
	DeleteObject(hBitmap);
	return;

	//
	// --- Affichage d'un rectangle noir ---
	//

Draw_BlackRect2:
	DeleteDC(hDC);
Draw_BlackRect1:
	DeleteObject(hBitmap);
Draw_BlackRect:
	CopyRect(&ClientRect,&ItemStruct->rcItem);
	DrawEdge(ItemStruct->hDC,&ItemStruct->rcItem,EDGE_SUNKEN,BF_RECT);
	ClientRect.left += 2;
	ClientRect.top += 2;
	ClientRect.right -= 2;
	ClientRect.bottom -= 2;
	BitBlt(ItemStruct->hDC,ClientRect.left,ClientRect.top,ClientRect.right-ClientRect.left,ClientRect.bottom-ClientRect.top,NULL,0,0,BLACKNESS);
	return;
}


// «»»» Affichage d'une erreur ««««««««««««««««««««««««««««««««««««««««««»

void Editor_PrintError(HWND hWnd, char *WindowText, char *WindowTitle, UINT Flags)
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
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
