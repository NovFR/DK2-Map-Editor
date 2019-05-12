
//<<>-<>>---------------------------------------------------------------------()
/*
	Fonctions relatives aux fenêtres.
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

extern HWND		hWnd;
extern HWND		hToolbar;
extern HWND		hMinimap;
extern HWND		hInfo;
extern HWND		hMapZoom;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Met à jour toutes les données d'affichage «««««««««««««««««««««««»

void Window_RedrawAll()
{
	RECT	ClientRect;

	GetClientRect(hWnd,&ClientRect);
	Minimap_Resize(hWnd);
	Window_Resize(hWnd,ClientRect.right,ClientRect.bottom);
	InvalidateRect(hWnd,NULL,TRUE);
	InvalidateRect(hToolbar,NULL,TRUE);
	InvalidateRect(hMinimap,NULL,TRUE);
	InvalidateRect(hInfo,NULL,TRUE);
	InvalidateRect(hMapZoom,NULL,TRUE);
	UpdateWindow(hWnd);
	UpdateWindow(hToolbar);
	UpdateWindow(hMinimap);
	UpdateWindow(hInfo);
	UpdateWindow(hMapZoom);
	return;
}


// «»»» Place une fenêtre à l'emplacement du curseur ««««««««««««««««««««»

void Window_UnderCursor(HWND hWnd, LONG *X, LONG *Y)
{
	APPBARDATA	AppBarData;
	RECT		WindowRect;
	POINT		CursorPos;
	long		Width,Height;
	long		ScreenWidth,ScreenHeight;

	AppBarData.cbSize = sizeof(APPBARDATA);
	GetCursorPos(&CursorPos);
	GetWindowRect(hWnd,&WindowRect);
	SHAppBarMessage(ABM_GETTASKBARPOS,&AppBarData);

	Width = WindowRect.right-WindowRect.left;
	Height = WindowRect.bottom-WindowRect.top;
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	switch(AppBarData.uEdge)
		{
		case ABE_BOTTOM:
			if (CursorPos.y+Height > AppBarData.rc.top) CursorPos.y = AppBarData.rc.top-Height;
			ScreenHeight = ScreenHeight-(AppBarData.rc.bottom-AppBarData.rc.top);
			break;
		case ABE_LEFT:
			if (CursorPos.x < AppBarData.rc.right) CursorPos.x = AppBarData.rc.right;
			ScreenWidth = ScreenWidth-(AppBarData.rc.right-AppBarData.rc.left);
			break;
		case ABE_RIGHT:
			if (CursorPos.x+Width > AppBarData.rc.left) CursorPos.x = AppBarData.rc.left-Width;
			ScreenWidth = ScreenWidth-(AppBarData.rc.right-AppBarData.rc.left);
			break;
		case ABE_TOP:
			if (CursorPos.y < AppBarData.rc.bottom) CursorPos.y = AppBarData.rc.bottom;
			ScreenHeight = ScreenHeight-(AppBarData.rc.bottom-AppBarData.rc.top);
			break;
		}

	if (CursorPos.x+Width > ScreenWidth)
		*X = ScreenWidth-Width;
	else
		*X = CursorPos.x;
	if (CursorPos.y+Height > ScreenHeight)
		*Y = ScreenHeight-Height;
	else
		*Y = CursorPos.y;

	if (*X < 0)
		*X = 0;
	if (*Y < 0)
		*Y = 0;

	return;
}


// «»»» Corrige les positions X & Y d'une fenêtre «««««««««««««««««««««««»

void Window_CorrectPos(HWND hWnd, LONG *X, LONG *Y)
{
	APPBARDATA	AppBarData;
	RECT		WindowRect;
	long		Width,Height;
	long		ScreenWidth,ScreenHeight;

	AppBarData.cbSize = sizeof(APPBARDATA);
	GetWindowRect(hWnd,&WindowRect);
	SHAppBarMessage(ABM_GETTASKBARPOS,&AppBarData);

	Width = WindowRect.right-WindowRect.left;
	Height = WindowRect.bottom-WindowRect.top;
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	switch(AppBarData.uEdge)
		{
		case ABE_BOTTOM:
			if (*Y+Height > AppBarData.rc.top) *Y = AppBarData.rc.top-Height;
			ScreenHeight = ScreenHeight-(AppBarData.rc.bottom-AppBarData.rc.top);
			break;
		case ABE_LEFT:
			if (*X < AppBarData.rc.right) *X = AppBarData.rc.right;
			ScreenWidth = ScreenWidth-(AppBarData.rc.right-AppBarData.rc.left);
			break;
		case ABE_RIGHT:
			if (*X+Width > AppBarData.rc.left) *X = AppBarData.rc.left-Width;
			ScreenWidth = ScreenWidth-(AppBarData.rc.right-AppBarData.rc.left);
			break;
		case ABE_TOP:
			if (*Y < AppBarData.rc.bottom) *Y = AppBarData.rc.bottom;
			ScreenHeight = ScreenHeight-(AppBarData.rc.bottom-AppBarData.rc.top);
			break;
		}

	if (*X+Width > ScreenWidth)
		*X = ScreenWidth-Width;
	if (*Y+Height > ScreenHeight)
		*Y = ScreenHeight-Height;

	if (*X < 0)
		*X = 0;
	if (*Y < 0)
		*Y = 0;
	return;
}


// «»»» Centre une fenêtre «««««««««««««««««««««««««««««««««««««««««««««««»

void Window_Center(HWND hWnd, HWND hParentWnd, LONG *X, LONG *Y)
{
	RECT	WindowRect;
	RECT	ParentWindowRect;

	GetWindowRect(hWnd,&WindowRect);
	if (!hParentWnd) hParentWnd = GetDesktopWindow();
	GetWindowRect(hParentWnd,&ParentWindowRect);

	*X = ((ParentWindowRect.right-ParentWindowRect.left)-(WindowRect.right-WindowRect.left))/2;
	*Y = ((ParentWindowRect.bottom-ParentWindowRect.top)-(WindowRect.bottom-WindowRect.top))/2;

	*X = ParentWindowRect.left+*X;
	*Y = ParentWindowRect.top+*Y;

	Window_CorrectPos(hWnd,X,Y);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
