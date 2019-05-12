
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre d'accès disque	
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
extern HWND		hWnd;
extern HWND		hDiskWnd;
extern HWND		hDiskProgressWnd;
extern HWND		hDiskStaticWnd;
extern UINT		DiskMethod;
extern FONTINFO		Font;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions appelée depuis "l'extérieur"				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Disk_CreateWindow(UINT Method)
{
	LONG	Width;
	LONG	Height;

	if (Config.HideDisk) return;
	DiskMethod = Method;
	Width = GetSystemMetrics(SM_CXSCREEN)*50/100;
	Height = GetSystemMetrics(SM_CYSMCAPTION)+GetSystemMetrics(SM_CYFIXEDFRAME)*2+4+Font.FontHeight+4+Font.FontHeight*2+4;

	hDiskWnd = CreateWindowEx(WS_EX_TOOLWINDOW,szDiskClass,NULL,WS_CAPTION|WS_BORDER|WS_CHILD|WS_POPUP|WS_CLIPCHILDREN|WS_SYSMENU,0,0,Width,Height,hWnd,NULL,hInstance,NULL);
	if (hDiskWnd)
		{
		ShowWindow(hDiskWnd,SW_SHOWNORMAL);
		UpdateWindow(hDiskWnd);
		}

	return;
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Disk_DestroyWindow(BOOL WaitALittleBit)
{
	if (hDiskWnd)
		{
		if (WaitALittleBit) Sleep(DISK_TIMEOUT);
		DestroyWindow(hDiskWnd);
		}
	SetActiveWindow(hWnd);
	return;
}


// «»»» Mise à jour de l'affichage ««««««««««««««««««««««««««««««««««««««»

void Disk_Update(UINT ID)
{
	if (!hDiskWnd) return;
	SendMessage(hDiskWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)ID);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages provenants de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG CALLBACK Disk_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	paintStruct;

	switch(uMsgId)
		{
		case WM_CREATE:
			return(Disk_Create(hWnd));

		case WM_UPDATEPROGRESS:
			Disk_Progress((UINT)lParam);
			break;

		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			BeginPaint(hWnd,&paintStruct);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_ERASEBKGND:
			if (!GetUpdateRect(hWnd,&paintStruct.rcPaint,FALSE)) break;
			FillRect((HDC)wParam,&paintStruct.rcPaint,(HBRUSH)(COLOR_MENU+1));
			return(1);

		case WM_DESTROY:
			hDiskWnd = NULL;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Initialisation des données de la fenêtre ««««««««««««««««««««««««»

LONG Disk_Create(HWND hChild)
{
	RECT	ClientRect;
	LONG	X,Y;

	Window_Center(hChild,hWnd,&X,&Y);
	SetWindowPos(hChild,hWnd,X,Y,0,0,SWP_NOSIZE);

	GetClientRect(hChild,&ClientRect);
	hDiskStaticWnd = CreateWindowEx(0,szStaticClass,NULL,WS_CHILD|WS_VISIBLE,4,4,ClientRect.right-ClientRect.left-8,Font.FontHeight,hChild,NULL,hInstance,NULL);
	hDiskProgressWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szProgressClass,NULL,WS_CHILD|WS_VISIBLE,2,4+Font.FontHeight+4,ClientRect.right-ClientRect.left-4,Font.FontHeight*2,hChild,NULL,hInstance,NULL);
	if (!hDiskStaticWnd || !hDiskProgressWnd) return(-1);

	SendMessage(hDiskStaticWnd,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);

	switch(DiskMethod)
		{
		case DISK_EDITORLOAD:
			SendMessage(hDiskProgressWnd,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,DISK_LEPRMAX));
			SetWindowText(hChild,szDiskLoading);
			break;
		case DISK_EDITORSAVE:
			SendMessage(hDiskProgressWnd,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,DISK_SEPRMAX));
			SetWindowText(hChild,szDiskSaving);
			break;
		case DISK_GAMELOAD:
			SendMessage(hDiskProgressWnd,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,DISK_LGPRMAX));
			SetWindowText(hChild,szDiskLoading);
			break;
		case DISK_TEXTSAVE:
			SendMessage(hDiskProgressWnd,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,DISK_STPRMAX));
			SetWindowText(hChild,szDiskSaving);
			break;
		}

	return(0);
}


// «»»» Mise à jour de l'affichage ««««««««««««««««««««««««««««««««««««««»

void Disk_Progress(UINT ID)
{
	char	*WindowText;

	switch(DiskMethod)
		{
		case DISK_EDITORLOAD:
			switch(ID)
				{
				case DISK_LEPRMAP:
					WindowText = szDiskEditorLoadMap;
					break;
				case DISK_LEPRSTATS:
					WindowText = szDiskEditorLoadStats;
					break;
				case DISK_LEPRMAPINFO:
					WindowText = szDiskEditorLoadMapInfo;
					break;
				case DISK_LEPRPLAYERSINFO:
					WindowText = szDiskEditorLoadPlayersInfo;
					break;
				case DISK_LEPRTHINGS:
					WindowText = szDiskEditorLoadThings;
					break;
				case DISK_LEPRRECTEX:
					WindowText = szDiskEditorLoadRectEx;
					break;
				case DISK_LEPRBANDS:
					WindowText = szDiskEditorLoadBands;
					break;
				case DISK_LEPRTRIGGERS:
					WindowText = szDiskEditorLoadTriggers;
					break;
				case DISK_LEPRVARIABLES:
					WindowText = szDiskEditorLoadVariables;
					break;
				case DISK_LEPRDONE:
					WindowText = szDiskEditorLoadDone;
					break;
				default:
					return;
				}
			break;
		case DISK_EDITORSAVE:
			switch(ID)
				{
				case DISK_SEPRSTATS:
					WindowText = szDiskEditorSaveStats;
					break;
				case DISK_SEPRMAPINFO:
					WindowText = szDiskEditorSaveMapInfo;
					break;
				case DISK_SEPRPLAYERSINFO:
					WindowText = szDiskEditorSavePlayersInfo;
					break;
				case DISK_SEPRMAP:
					WindowText = szDiskEditorSaveMap;
					break;
				case DISK_SEPRTHINGS:
					WindowText = szDiskEditorSaveThings;
					break;
				case DISK_SEPRRECTEX:
					WindowText = szDiskEditorSaveRectEx;
					break;
				case DISK_SEPRBANDS:
					WindowText = szDiskEditorSaveBands;
					break;
				case DISK_SEPRTRIGGERS:
					WindowText = szDiskEditorSaveTriggers;
					break;
				case DISK_SEPRVARIABLES:
					WindowText = szDiskEditorSaveVariables;
					break;
				case DISK_SEPRDONE:
					WindowText = szDiskEditorSaveDone;
					break;
				default:
					return;
				}
			break;
		case DISK_GAMELOAD:
			switch(ID)
				{
				case DISK_LGPRMAP:
					WindowText = szDiskEditorLoadMap;
					break;
				case DISK_LGPRMAIN:
					WindowText = szDiskEditorLoadMapInfo;
					break;
				case DISK_LGPRPLAYERS:
					WindowText = szDiskEditorLoadPlayersInfo;
					break;
				case DISK_LGPRTHINGS:
					WindowText = szDiskEditorLoadThings;
					break;
				case DISK_LGPRVARIABLES:
					WindowText = szDiskGameLoadVariables;
					break;
				case DISK_LGPRTRIGGERS:
					WindowText = szDiskEditorLoadTriggers;
					break;
				case DISK_LGPRDONE:
					WindowText = szDiskEditorLoadDone;
					break;
				default:
					return;
				}
			break;
		case DISK_TEXTLOAD:
			switch(ID)
				{
				case DISK_LTPRMAP:
					WindowText = szDiskEditorLoadMap;
					break;
				case DISK_LTPRTHINGS:
					WindowText = szDiskTextLoadThings;
					break;
				case DISK_LTPRCREATURES:
					WindowText = szDiskTextLoadCreatures;
					break;
				case DISK_LTPRDOORS:
					WindowText = szDiskTextLoadDoors;
					break;
				case DISK_LTPRTRAPS:
					WindowText = szDiskTextLoadTraps;
					break;
				case DISK_LTPRINFO:
					WindowText = szDiskEditorLoadMapInfo;
					break;
				case DISK_LTPRBANDS:
					WindowText = szDiskEditorLoadBands;
					break;
				case DISK_LTPRAREAS:
					WindowText = szDiskTextLoadAreas;
					break;
				case DISK_LTPRGATES:
					WindowText = szDiskTextLoadGates;
					break;
				case DISK_LTPRTRIGGERS:
					WindowText = szDiskEditorLoadTriggers;
					break;
				case DISK_LTPRVARIABLES:
					WindowText = szDiskGameLoadVariables;
					break;
				default:
					return;
				}
			break;
		case DISK_TEXTSAVE:
			switch(ID)
				{
				case DISK_STPRHEADER:
					WindowText = szDiskTextSaveHeader;
					break;
				case DISK_STPRMAP:
					WindowText = szDiskEditorSaveMap;
					break;
				case DISK_STPRTHINGS:
					WindowText = szDiskEditorSaveThings;
					break;
				case DISK_STPRMAPINFO:
					WindowText = szDiskEditorSaveMapInfo;
					break;
				case DISK_STPRBANDS:
					WindowText = szDiskEditorSaveBands;
					break;
				case DISK_STPRAREAS:
					WindowText = szDiskTextSaveAreas;
					break;
				case DISK_STPRGATES:
					WindowText = szDiskTextSaveGates;
					break;
				case DISK_STPRTRIGGERS:
					WindowText = szDiskEditorSaveTriggers;
					break;
				case DISK_STPRVARIABLES:
					WindowText = szDiskTextSaveVariables;
					break;
				case DISK_STPRDONE:
					WindowText = szDiskEditorSaveDone;
					break;
				default:
					return;
				}
			break;
		default:
			return;
		}

	SendMessage(hDiskProgressWnd,PBM_SETPOS,(WPARAM)ID,(LPARAM)0);
	SetWindowText(hDiskStaticWnd,WindowText);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
