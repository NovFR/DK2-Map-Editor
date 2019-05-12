
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion de la carte miniature.
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
extern HWND		hInfo;
extern HWND		hToolbar;
extern HWND		hMinimap;
extern HBITMAP		hMinimapBitmap;
extern BOOL		MinimapIsMoving;
extern MAP		Map;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

int Minimap_CreateWindow(HWND hWnd)
{
	RECT	MinimapRect;

	Minimap_GetArea(hWnd,&MinimapRect,TRUE);
	hMinimap = CreateWindow(szButtonClass,NULL,WS_CHILD|BS_OWNERDRAW,MinimapRect.left,MinimapRect.top,MinimapRect.right-MinimapRect.left,MinimapRect.bottom-MinimapRect.top,hWnd,NULL,hInstance,NULL);
	if (!hMinimap) return(0);

	if (!Config.HideMinimap)
		{
		ShowWindow(hMinimap,SW_SHOWNORMAL);
		UpdateWindow(hMinimap);
		}

	return(1);
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Minimap_DestroyWindow()
{
	if (hMinimapBitmap) DeleteObject(hMinimapBitmap);
	hMinimapBitmap = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Minimap_Messages(MSG *msg)
{
	if ((IsWindow(hMinimap))&&(msg->hwnd == hMinimap))
		{
		switch(msg->message)
			{
			case WM_RBUTTONDOWN:
				Fullmap_CreateWindow();
				break;
			case WM_LBUTTONDOWN:
				if (Minimap_MoveCursor(LOWORD(msg->lParam),HIWORD(msg->lParam),TRUE))
					{
					SetCapture(msg->hwnd);
					MinimapIsMoving = TRUE;
					}
				break;
			case WM_MOUSEMOVE:
				if (MinimapIsMoving) Minimap_MoveCursor(LOWORD(msg->lParam),HIWORD(msg->lParam),FALSE);
				break;
			case WM_LBUTTONUP:
				MinimapIsMoving = FALSE;
				ReleaseCapture();
				break;
			}
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la miniature ««««««««««««««««««««««««««««««««««««««««»

void Minimap_Build(HWND hWnd)
{
	if (hMinimapBitmap)
		{
		DeleteObject(hMinimapBitmap);
		hMinimapBitmap = NULL;
		}

	hMinimapBitmap = Thumbnail_Generate(hWnd,&Map,NULL);

	InvalidateRect(hMinimap,NULL,FALSE);
	UpdateWindow(hMinimap);
	return;
}


// «»»» Redessine le carte ««««««««««««««««««««««««««««««««««««««««««««««»

void Minimap_Redraw(HWND hWnd)
{
	if (!hMinimapBitmap)
		{
		Minimap_Build(hWnd);
		return;
		}

	Thumbnail_Generate(hWnd,&Map,hMinimapBitmap);
	InvalidateRect(hMinimap,NULL,FALSE);
	UpdateWindow(hMinimap);
	return;
}


// «»»» Mise à jour de l'affichage ««««««««««««««««««««««««««««««««««««««»

//--- Réaffiche tout ---

void Minimap_Refresh()
{
	if ((IsWindow(hMinimap))&&(IsWindowVisible(hMinimap)))
		{
		InvalidateRect(hMinimap,NULL,FALSE);
		UpdateWindow(hMinimap);
		}
	return;
}

//--- Ne met à jour que le curseur ---

void Minimap_RefreshFast(LONG X, LONG Y)
{
	if ((IsWindow(hMinimap))&&(IsWindowVisible(hMinimap)))
		{
		HDC hDC = GetDC(hMinimap);
		if (hDC)
			{
			Minimap_DrawCursor(hDC,X,Y);
			Minimap_DrawCursor(hDC,MapStartX,MapStartY);
			ReleaseDC(hMinimap,hDC);
			}
		else
			Minimap_Refresh();
		}

	return;
}


// «»»» Montre ou cache la carte miniature ««««««««««««««««««««««««««««««»

void Minimap_Hide(BOOL Hide)
{
	if (!IsWindow(hMinimap)) return;

	if (!Hide)
		{
		if (IsWindowVisible(hMinimap)) return;
		ShowWindow(hMinimap,SW_SHOW);
		ShowWindow(hInfo,SW_SHOW);
		}
	else
		{
		if (!IsWindowVisible(hMinimap)) return;
		ShowWindow(hMinimap,SW_HIDE);
		ShowWindow(hInfo,SW_HIDE);
		}

	Window_RedrawAll();
	return;
}


// «»»» Dessine la carte ««««««««««««««««««««««««««««««««««««««««««««««««»

void Minimap_DrawWindow(DRAWITEMSTRUCT *DrawStruct)
{
	HDC		hDC;
	HBITMAP		hBitmap;
	register long	Prop;

	if ((DrawStruct->itemAction&ODA_FOCUS)||(DrawStruct->itemAction&ODA_SELECT)) return;

	if (!hMinimapBitmap) goto Black_Rect;

	hDC = CreateCompatibleDC(DrawStruct->hDC);
	if (!hDC) goto Black_Rect;
	hBitmap = SelectObject(hDC,hMinimapBitmap);
	if (hBitmap)
		{
		if ((Config.MinimapKeepProps)&&(Map.Properties.Width != Map.Properties.Height))
			{
			if (Map.Properties.Width < Map.Properties.Height)
				{
				Prop = (long)((float)Config.MinimapSize*((float)Map.Properties.Width/(float)Map.Properties.Height));
				BitBlt(DrawStruct->hDC,0,0,(Config.MinimapSize-Prop)/2,Config.MinimapSize,NULL,0,0,BLACKNESS);
				BitBlt(DrawStruct->hDC,(Config.MinimapSize-Prop)/2+Prop,0,(Config.MinimapSize-Prop)/2+((Config.MinimapSize-Prop)%2),Config.MinimapSize,NULL,0,0,BLACKNESS);
				StretchBlt(DrawStruct->hDC,(Config.MinimapSize-Prop)/2,0,Prop,Config.MinimapSize,hDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);
				}
			else // if (Map.Width > Map.Height)
				{
				Prop = (long)((float)Config.MinimapSize*((float)Map.Properties.Height/(float)Map.Properties.Width));
				BitBlt(DrawStruct->hDC,0,0,Config.MinimapSize,(Config.MinimapSize-Prop)/2,NULL,0,0,BLACKNESS);
				BitBlt(DrawStruct->hDC,0,(Config.MinimapSize-Prop)/2+Prop,Config.MinimapSize,(Config.MinimapSize-Prop)/2+((Config.MinimapSize-Prop)%2),NULL,0,0,BLACKNESS);
				StretchBlt(DrawStruct->hDC,0,(Config.MinimapSize-Prop)/2,Config.MinimapSize,Prop,hDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);
				}
			}
		else
			StretchBlt(DrawStruct->hDC,0,0,Config.MinimapSize,Config.MinimapSize,hDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);

		SelectObject(hDC,hBitmap);
		}
	DeleteDC(hDC);
	Minimap_DrawCursor(DrawStruct->hDC,MapStartX,MapStartY);
	return;

//--- Affichage d'un rectangle noir ---

Black_Rect:
	FillRect(DrawStruct->hDC,&DrawStruct->rcItem,GetStockObject(BLACK_BRUSH));
	Minimap_DrawCursor(DrawStruct->hDC,MapStartX,MapStartY);
	return;
}


// «»»» Dessine le curseur ««««««««««««««««««««««««««««««««««««««««««««««»

void Minimap_DrawCursor(HDC hDC, LONG MapStartX, LONG MapStartY)
{
	POINT		Rect[5];
	HPEN		hPen,hOldPen;
	LONG		DX,DY;
	register float	PX,PY;
	register float	SX,SY;
	register float	MX,MY;
	register long	PropX,PropY;
	int		OldMode;

	hPen = CreatePen(PS_SOLID,1,0x00000000);
	if (hPen)
		{
		hOldPen = SelectObject(hDC,hPen);
		if (hOldPen)
			{
			OldMode = SetROP2(hDC,R2_MERGEPENNOT);
			Map_GetMapSquares(hWnd,NULL,&DX,&DY);
			DX--;
			DY--;
			if (DX > Map.Properties.Width) DX = Map.Properties.Width;
			if (DY > Map.Properties.Height) DY = Map.Properties.Height;
			PX = (float)DX/(float)(Map.Properties.Width);
			PY = (float)DY/(float)(Map.Properties.Height);
			SX = (float)MapStartX/(float)(Map.Properties.Width);
			SY = (float)MapStartY/(float)(Map.Properties.Height);
			MX = (float)(Config.MinimapSize);
			MY = (float)(Config.MinimapSize);

			PropX = 0;
			PropY = 0;
			if ((Config.MinimapKeepProps)&&(Map.Properties.Width != Map.Properties.Height))
				{
				if (Map.Properties.Width < Map.Properties.Height)
					{
					MX = (long)((float)Config.MinimapSize*((float)Map.Properties.Width/(float)Map.Properties.Height));
					PropX = (Config.MinimapSize-MX)/2;
					}
				else // if (MapWidth > MapHeight)
					{
					MY = (long)((float)Config.MinimapSize*((float)Map.Properties.Height/(float)Map.Properties.Width));
					PropY = (Config.MinimapSize-MY)/2;
					}
				}

			Rect[0].x = (long)(MX*SX)+PropX;
			Rect[0].y = (long)(MY*SY)+PropY;
			Rect[1].x = (long)(MX*PX)+Rect[0].x-1;
			if (Rect[1].x > Config.MinimapSize) Rect[1].x = (long)MX;
			Rect[1].y = Rect[0].y;
			Rect[2].x = Rect[1].x;
			Rect[2].y = (long)(MY*PY)+Rect[0].y-1;
			if (Rect[2].y > Config.MinimapSize) Rect[2].y = (long)MY;
			Rect[3].x = Rect[0].x;
			Rect[3].y = Rect[2].y;
			Rect[4].x = Rect[0].x;
			Rect[4].y = Rect[0].y;
			Polyline(hDC,Rect,5);

			SetROP2(hDC,OldMode);
			SelectObject(hDC,hOldPen);
			}
		DeleteObject(hPen);
		}

	return;
}


// «»»» Déplacement du curseur ««««««««««««««««««««««««««««««««««««««««««»

int Minimap_MoveCursor(LONG X, LONG Y, BOOL Query)
{
	long		MSX,MSY;
	long		PropX,PropY;
	long		B0,B1;
	long		DX,DY;
	long		FX,FY;
	register float	MX,MY;
	register float	PX,PY;

	MX = (float)(Config.MinimapSize);
	MY = (float)(Config.MinimapSize);
	PropX = 0;
	PropY = 0;
	if ((Config.MinimapKeepProps)&&(Map.Properties.Width != Map.Properties.Height))
		{
		if (Map.Properties.Width < Map.Properties.Height)
			{
			MX = (long)((float)Config.MinimapSize*((float)Map.Properties.Width/(float)Map.Properties.Height));
			PropX = (Config.MinimapSize-MX)/2;
			}
		else // if (MapWidth > MapHeight)
			{
			MY = (long)((float)Config.MinimapSize*((float)Map.Properties.Height/(float)Map.Properties.Width));
			PropY = (Config.MinimapSize-MY)/2;
			}
		}

	Map_GetMapSquares(hWnd,NULL,&DX,&DY);
	DX--;
	DY--;
	if (DX > Map.Properties.Width) DX = Map.Properties.Width;
	if (DY > Map.Properties.Height) DY = Map.Properties.Height;
	PX = (float)DX/(float)(Map.Properties.Width);
	PY = (float)DY/(float)(Map.Properties.Height);

	if ((PX == 1.0f)&&(PY == 1.0f)) return(0);
	if (Query)
		{
		if (X < PropX) return(0);
		if (Y < PropY) return(0);
		if (X > Config.MinimapSize-PropX) return(0);
		if (Y > Config.MinimapSize-PropY) return(0);
		}

	PX = (PX*MX)/2;
	PY = (PY*MY)/2;
	X -= PX;
	Y -= PY;
	if (X < PropX) X = PropX;
	if (Y < PropY) Y = PropY;
	if (X > Config.MinimapSize-PropX) X = Config.MinimapSize-PropX;
	if (Y > Config.MinimapSize-PropY) Y = Config.MinimapSize-PropY;

	MSX = MapStartX;
	MSY = MapStartY;
	FX = ((X-PropX)*Map.Properties.Width)/MX;
	FY = ((Y-PropY)*Map.Properties.Height)/MY;
	B0 = Map_ScrollHorizontally(hWnd,SB_THUMBTRACK,FX,FALSE);
	B1 = Map_ScrollVertically(hWnd,SB_THUMBTRACK,FY,FALSE);

	if ((B0)||(B1))
		{
		HDC hDC = GetDC(hMinimap);
		if (hDC)
			{
			Minimap_DrawCursor(hDC,MSX,MSY);
			if (B0)	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
			if (B1)	Map_SetPropThumb(hWnd,PROP_VERTICAL);
			SendMessage(hWnd,WM_PAINTMAP,(WPARAM)0,(LPARAM)0);
			Minimap_DrawCursor(hDC,MapStartX,MapStartY);
			ReleaseDC(hMinimap,hDC);
			}
		else
			{
			if (B0)	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
			if (B1)	Map_SetPropThumb(hWnd,PROP_VERTICAL);
			SendMessage(hWnd,WM_PAINTMAP,(WPARAM)0,(LPARAM)0);
			InvalidateRect(hMinimap,NULL,FALSE);
			UpdateWindow(hMinimap);
			}
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Change la position de la carte ««««««««««««««««««««««««««««««««««»

void Minimap_Resize(HWND hWnd)
{
	RECT	ToolRect;

	Minimap_GetArea(hWnd,&ToolRect,FALSE);
	SetWindowPos(hMinimap,NULL,ToolRect.left,ToolRect.top,ToolRect.right-ToolRect.left,ToolRect.bottom-ToolRect.top,SWP_NOZORDER);
	return;
}


// «»»» Obtention de la zone affichable «««««««««««««««««««««««««««««««««»

void Minimap_GetArea(HWND hWnd, RECT *Rect, BOOL CheckFlagOnly)
{
	RECT	ToolRect;

	Rect->left = 0;
	Rect->right = Config.MinimapSize;
	Rect->top = 1;
	Rect->bottom = Config.MinimapSize+1;

	if ((CheckFlagOnly)&&(!Config.HideToolbar))
		{
		GetWindowRect(hToolbar,&ToolRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&ToolRect,2);
		Rect->top += ToolRect.bottom-1;
		Rect->bottom += ToolRect.bottom-1;
		}
	else if ((IsWindow(hToolbar))&&(IsWindowVisible(hToolbar)))
		{
		GetWindowRect(hToolbar,&ToolRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&ToolRect,2);
		Rect->top += ToolRect.bottom-1;
		Rect->bottom += ToolRect.bottom-1;
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
