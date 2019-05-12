
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des barres de défilement.
	Les fonctions relatives au bouton de sélection des zooms sont
	dans le fichier "Windows\ZoomButton.c".
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
extern HWND		hMapVScroll;
extern HWND		hMapHScroll;
extern HWND		hMapZoom;
extern HICON		hMapZoomIcon;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern MAP		Map;
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorHigh;
extern MAPRECT		MapCursorSel;
extern UINT		MapCursorMode;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création des barres de défilement «««««««««««««««««««««««««««««««»

int Map_CreateProp(HWND hWnd)
{
	RECT	MapRect;

	hMapZoomIcon = LoadImage(hInstance,MAKEINTRESOURCE(10),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!hMapZoomIcon) return(0);
	Map_GetPropSizes(hWnd,&MapRect,PROP_VERTICAL);
	hMapVScroll = CreateWindow(szScrollBarClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_VERT,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,hWnd,NULL,hInstance,NULL);
	if (!hMapVScroll) return(0);
	Map_GetPropSizes(hWnd,&MapRect,PROP_HORIZONTAL);
	hMapHScroll = CreateWindow(szScrollBarClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_HORZ,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,hWnd,NULL,hInstance,NULL);
	if (!hMapHScroll) return(0);
	Map_GetPropSizes(hWnd,&MapRect,PROP_ZOOMBUTTON);
	hMapZoom = CreateWindow(szButtonClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_OWNERDRAW,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,hWnd,NULL,hInstance,NULL);
	if (!hMapZoom) return(0);

	return(1);
}


// «»»» Destruction des données relatives aux barres ««««««««««««««««««««»

void Map_DestroyProp()
{
	if (hMapZoomIcon) DestroyIcon(hMapZoomIcon);
	hMapZoomIcon = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Dimensions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Obtention des dimensions des barres «««««««««««««««««««««««««««««»

void Map_SetPropThumb(HWND hWnd, UINT Type)
{
	SCROLLINFO	ScrollInfo;
	HWND		hScroll;
	RECT		MapRect;

	Map_GetMapArea(hWnd,&MapRect);
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin = 0;

	switch(Type)
		{
		case PROP_VERTICAL:
			hScroll = hMapVScroll;
			ScrollInfo.nMax = Map.Properties.Height-1;
			ScrollInfo.nPage = (MapRect.bottom-MapRect.top)/MapZoomCY;
			ScrollInfo.nPos = MapStartY;
			break;
		case PROP_HORIZONTAL:
			hScroll = hMapHScroll;
			ScrollInfo.nMax = Map.Properties.Width-1;
			ScrollInfo.nPage = (MapRect.right-MapRect.left)/MapZoomCX;
			ScrollInfo.nPos = MapStartX;
			break;
		}

	SetScrollInfo(hScroll,SB_CTL,&ScrollInfo,TRUE);
	return;
}


// «»»» Obtention des dimensions des barres «««««««««««««««««««««««««««««»

void Map_GetPropSizes(HWND hWnd, RECT *PropRect, UINT Type)
{
	RECT	MapRect;
	LONG	VBarSize,HBarSize;

	Map_GetMapFullArea(hWnd,&MapRect);
	VBarSize = GetSystemMetrics(SM_CXVSCROLL);
	HBarSize = GetSystemMetrics(SM_CYHSCROLL);
	switch(Type)
		{
		case PROP_VERTICAL:
			MapRect.left = MapRect.right-VBarSize;
			MapRect.right = MapRect.left+VBarSize;
			MapRect.bottom -= HBarSize;
			break;
		case PROP_HORIZONTAL:
			MapRect.top = MapRect.bottom-HBarSize;
			MapRect.bottom = MapRect.top+HBarSize;
			MapRect.right -= VBarSize;
			break;
		case PROP_ZOOMBUTTON:
			MapRect.left = MapRect.right-VBarSize;
			MapRect.right = MapRect.left+VBarSize;
			MapRect.top = MapRect.bottom-HBarSize;
			MapRect.bottom = MapRect.top+HBarSize;
			break;
		}
	CopyRect(PropRect,&MapRect);
	return;
}


// «»»» Change les dimensions des barres ««««««««««««««««««««««««««««««««»

void Map_PropResize(HWND hWnd)
{
	RECT	MapRect;

	Map_GetPropSizes(hWnd,&MapRect,PROP_VERTICAL);
	SetWindowPos(hMapVScroll,NULL,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,SWP_NOZORDER);
	Map_SetPropThumb(hWnd,PROP_VERTICAL);
	Map_GetPropSizes(hWnd,&MapRect,PROP_HORIZONTAL);
	SetWindowPos(hMapHScroll,NULL,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,SWP_NOZORDER);
	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
	Map_GetPropSizes(hWnd,&MapRect,PROP_ZOOMBUTTON);
	SetWindowPos(hMapZoom,NULL,MapRect.left,MapRect.top,MapRect.right-MapRect.left,MapRect.bottom-MapRect.top,SWP_NOZORDER);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Changement de position						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Déplacement déterminé «««««««««««««««««««««««««««««««««««««««««««»

//--- Déplacement sur un point ---

void Map_ScrollToPoint(HWND hWnd, LONG X, LONG Y, DWORD Flags)
{
	RECT	MapRect;
	LONG	DX,DY;

	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,&DX,&DY);
	if (X >= Map.Properties.Width) return;
	if (Y >= Map.Properties.Height) return;

	MapStartX = X-(DX/2);
	MapStartY = Y-(DY/2);
	if (MapStartX < 0) MapStartX = 0;
	if (MapStartY < 0) MapStartY = 0;
	if (MapStartX > Map.Properties.Width+1-DX) MapStartX = Map.Properties.Width+1-DX;
	if (MapStartY > Map.Properties.Height+1-DY) MapStartY = Map.Properties.Height+1-DY;
	if (DX >= Map.Properties.Width+1) MapStartX = 0;
	if (DY >= Map.Properties.Height+1) MapStartY = 0;

	if (Flags&SHOW_CURSOR)
		{
		MapCursorRect.hidden = TRUE;
		MapCursorHigh.hidden = FALSE;
		MapCursorHigh.x1 = X;
		MapCursorHigh.y1 = Y;
		MapCursorHigh.x2 = X;
		MapCursorHigh.y2 = Y;
		}
	if (Flags&SHOW_SELRECT)
		{
		Mouse_HideSelRect(hWnd);
		MapCursorSel.hidden = FALSE;
		MapCursorSel.x1 = X;
		MapCursorSel.y1 = Y;
		MapCursorSel.x2 = X;
		MapCursorSel.y2 = Y;
		}

	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Minimap_Refresh();
	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
	Map_SetPropThumb(hWnd,PROP_VERTICAL);

	if (Flags&SHOW_UPDATEINFOWND) Info_Update();
	return;
}

//--- Déplacement sur un rectangle ---

void Map_ScrollToPointEx(HWND hWnd, LONG X, LONG Y, LONG Width, LONG Height, DWORD Flags)
{
	RECT	MapRect;
	LONG	DX,DY;

	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,&DX,&DY);
	if (X >= Map.Properties.Width) return;
	if (Y >= Map.Properties.Height) return;
	if (X+Width-1 >= Map.Properties.Width) return;
	if (Y+Height-1 >= Map.Properties.Height) return;

	if (Width < DX) MapStartX = X-(DX/2)+Width/2;
	else MapStartX = X-2;
	if (Height < DY) MapStartY = Y-(DY/2)+Height/2;
	else MapStartY = Y-2;

	if (MapStartX < 0) MapStartX = 0;
	if (MapStartY < 0) MapStartY = 0;
	if (MapStartX > Map.Properties.Width+1-DX) MapStartX = Map.Properties.Width+1-DX;
	if (MapStartY > Map.Properties.Height+1-DY) MapStartY = Map.Properties.Height+1-DY;
	if (DX >= Map.Properties.Width+1) MapStartX = 0;
	if (DY >= Map.Properties.Height+1) MapStartY = 0;

	if (Flags&SHOW_CURSOR)
		{
		MapCursorRect.hidden = TRUE;
		MapCursorHigh.hidden = FALSE;
		MapCursorHigh.x1 = X;
		MapCursorHigh.y1 = Y;
		MapCursorHigh.x2 = X+Width-1;
		MapCursorHigh.y2 = Y+Height-1;
		}
	if (Flags&SHOW_SELRECT)
		{
		Mouse_HideSelRect(hWnd);
		MapCursorSel.hidden = FALSE;
		MapCursorSel.x1 = X;
		MapCursorSel.y1 = Y;
		MapCursorSel.x2 = X+Width-1;
		MapCursorSel.y2 = Y+Height-1;
		}

	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Minimap_Refresh();
	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
	Map_SetPropThumb(hWnd,PROP_VERTICAL);

	if (Flags&SHOW_UPDATEINFOWND) Info_Update();
	return;
}


// «»»» Déplacement horizontal ««««««««««««««««««««««««««««««««««««««««««»

int Map_ScrollHorizontally(HWND hWnd, UINT NotificationCode, LONG Track, BOOL Refresh)
{
	RECT	MapRect;
	LONG	SavedPos;
	LONG	DX;

	SavedPos = MapStartX;
	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,&DX,NULL);
	if (Map.Properties.Width < DX) return(0);

	switch(NotificationCode)
		{
		case SB_TOP:
			MapStartX = 0;
			goto Scroll_Map;
		case SB_PAGEUP:
			if (!MapStartX) break;
			MapStartX -= DX;
			goto Scroll_Map;
		case SB_PAGEDOWN:
			MapStartX += DX;
			goto Scroll_Map;
		case SB_LINEUP:
			if (!MapStartX) break;
			--MapStartX;
			goto Scroll_Map;
		case SB_LINEDOWN:
			MapStartX++;
			goto Scroll_Map;
		case SB_THUMBTRACK:
			MapStartX = Track;
			goto Scroll_Map;
		case SB_BOTTOM:
			MapStartX = Map.Properties.Width+1-DX;
			goto Scroll_Map;
		}

	return(0);

//--- Changement de la position de la carte ---

Scroll_Map:
	if (MapStartX < 0)
		MapStartX = 0;

	if (MapStartX > Map.Properties.Width+1-DX)
		MapStartX = Map.Properties.Width+1-DX;

	if (MapStartX == SavedPos) return(0);
	if (!Refresh) return(1);

	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Minimap_RefreshFast(SavedPos,MapStartY);
	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);

	if (MapCursorMode)
		{
		POINT CursorPos;
		GetCursorPos(&CursorPos);
		MapWindowPoints(NULL,hWnd,&CursorPos,1);
		Mouse_Moved(hWnd,CursorPos.x,CursorPos.y);
		}

	return(1);
}


// «»»» Déplacement vertical ««««««««««««««««««««««««««««««««««««««««««««»

int Map_ScrollVertically(HWND hWnd, UINT NotificationCode, LONG Track, BOOL Refresh)
{
	RECT	MapRect;
	LONG	SavedPos;
	LONG	DY;

	SavedPos = MapStartY;
	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,NULL,&DY);
	if (Map.Properties.Height < DY) return(0);

	switch(NotificationCode)
		{
		case SB_TOP:
			MapStartY = 0;
			goto Scroll_Map;
		case SB_PAGEUP:
			if (!MapStartY) break;
			MapStartY -= DY;
			goto Scroll_Map;
		case SB_PAGEDOWN:
			MapStartY += DY;
			goto Scroll_Map;
		case SB_LINEUP:
			if (!MapStartY) break;
			--MapStartY;
			goto Scroll_Map;
		case SB_LINEDOWN:
			MapStartY++;
			goto Scroll_Map;
		case SB_THUMBTRACK:
			MapStartY = Track;
			goto Scroll_Map;
		case SB_BOTTOM:
			MapStartY = Map.Properties.Height+1-DY;
			goto Scroll_Map;
		}

	return(0);

//--- Changement de la position de la carte ---

Scroll_Map:
	if (MapStartY < 0)
		MapStartY = 0;

	if (MapStartY > Map.Properties.Height+1-DY)
		MapStartY = Map.Properties.Height+1-DY;

	if (MapStartY == SavedPos) return(0);
	if (!Refresh) return(1);

	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Minimap_RefreshFast(MapStartX,SavedPos);
	Map_SetPropThumb(hWnd,PROP_VERTICAL);

	if (MapCursorMode)
		{
		POINT CursorPos;
		GetCursorPos(&CursorPos);
		MapWindowPoints(NULL,hWnd,&CursorPos,1);
		Mouse_Moved(hWnd,CursorPos.x,CursorPos.y);
		}

	return(1);
}


// «»»» Corrige la position de la carte en cas de redimensionnement «««««»

void Map_CorrectPos(HWND hWnd)
{
	RECT	MapRect;
	BOOL	Update = FALSE;
	LONG	DX,DY;

	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,&DX,&DY);

	if (MapStartX > Map.Properties.Width+1-DX)
		{
		Update = TRUE;
		MapStartX = Map.Properties.Width+1-DX;
		}
	if (MapStartY > Map.Properties.Height+1-DY)
		{
		Update = TRUE;
		MapStartY = Map.Properties.Height+1-DY;
		}
	if (DX >= Map.Properties.Width+1)
		{
		Update = TRUE;
		MapStartX = 0;
		}
	if (DY >= Map.Properties.Height+1)
		{
		Update = TRUE;
		MapStartY = 0;
		}

	if (Update)
		{
		InvalidateRect(hWnd,&MapRect,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
