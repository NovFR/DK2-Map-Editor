
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des zones utilisables par la carte.
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

extern HWND		hStatusWnd;
extern HWND		hToolbar;
extern HWND		hMinimap;
extern HWND		hMapHScroll;
extern HWND		hMapVScroll;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapCursorMaxWidth;
extern LONG		MapCursorMaxHeight;
extern MAP		Map;
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorSel;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Obtention des dimensions d'affichage (en totalité) ««««««««««««««»

void Map_GetMapFullArea(HWND hWnd, RECT *AreaRect)
{
	RECT	MapRect;
	RECT	ChildRect;

	GetClientRect(hWnd,AreaRect);

	//--- Corrige les dimensions ---

	GetWindowRect(hStatusWnd,&ChildRect);
	MapWindowPoints(NULL,hWnd,(POINT *)&ChildRect,2);
	SubtractRect(&MapRect,AreaRect,&ChildRect);
	CopyRect(AreaRect,&MapRect);

	if ((IsWindow(hToolbar))&&(IsWindowVisible(hToolbar)))
		{
		GetWindowRect(hToolbar,&ChildRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&ChildRect,2);
		SubtractRect(&MapRect,AreaRect,&ChildRect);
		CopyRect(AreaRect,&MapRect);
		}
	else
		AreaRect->top += 1;

	if ((IsWindow(hMinimap))&&(IsWindowVisible(hMinimap)))
		{
		GetWindowRect(hMinimap,&ChildRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&ChildRect,2);
		ChildRect.bottom = AreaRect->bottom;
		SubtractRect(&MapRect,AreaRect,&ChildRect);
		CopyRect(AreaRect,&MapRect);
		AreaRect->left += 1;
		}

	return;
}


// «»»» Obtention des dimensions d'affichage (réelles) ««««««««««««««««««»

void Map_GetMapArea(HWND hWnd, RECT *AreaRect)
{
	RECT	MapRect;
	RECT	ChildRect;

	Map_GetMapFullArea(hWnd,AreaRect);

	GetWindowRect(hMapHScroll,&ChildRect);
	MapWindowPoints(NULL,hWnd,(POINT *)&ChildRect,2);
	ChildRect.right += GetSystemMetrics(SM_CXHSCROLL);
	SubtractRect(&MapRect,AreaRect,&ChildRect);
	CopyRect(AreaRect,&MapRect);

	GetWindowRect(hMapVScroll,&ChildRect);
	MapWindowPoints(NULL,hWnd,(POINT *)&ChildRect,2);
	ChildRect.bottom += GetSystemMetrics(SM_CYVSCROLL);
	SubtractRect(&MapRect,AreaRect,&ChildRect);
	CopyRect(AreaRect,&MapRect);

	return;
}


// «»»» Obtention du nombre de carrés affichables «««««««««««««««««««««««»

void Map_GetMapSquares(HWND hWnd, RECT *AreaRect, LONG *DX, LONG *DY)
{
	RECT	MapRect;

	if (!AreaRect)
		Map_GetMapArea(hWnd,&MapRect);
	else
		CopyRect(&MapRect,AreaRect);

	if (DX) *DX = ((MapRect.right-MapRect.left)/MapZoomCX)+1;
	if (DY) *DY = ((MapRect.bottom-MapRect.top)/MapZoomCY)+1;
	return;
}


// «»»» Test si un point se trouvant dans un rectangle ««««««««««««««««««»

int Map_IsPointInRect(LONG X, LONG Y, MAPRECT *MapRect)
{
	if (MapRect)
		{
		MAPCLIENT	MapClient;
		Map_CorrectRect(MapRect,&MapClient);
		if ((X < MapClient.X)||(X >= MapClient.X+MapClient.Width)||(Y < MapClient.Y)||(Y >= MapClient.Y+MapClient.Height)) return(0);
		}

	return(1);
}


// «»»» Conversion d'un rectangle en zone effectivement affichable ««««««»

int Map_RectangleToClient(HWND hWnd, MAPRECT *MapRect, MAPCLIENT *MapClient)
{
	RECT	MapArea;
	LONG	DX,DY;

	Map_CorrectRect(MapRect,MapClient);

	//--- Corrige le rectangle pour le rendre visible ---

	Map_GetMapArea(hWnd,&MapArea);
	Map_GetMapSquares(NULL,&MapArea,&DX,&DY);

	if (MapClient->X < 0) MapClient->X = 0;
	if (MapClient->Y < 0) MapClient->Y = 0;
	if (MapClient->X+MapClient->Width < MapStartX) return(0);
	if (MapClient->Y+MapClient->Height < MapStartY) return(0);

	if (MapClient->Y < MapStartY)
		{
		MapClient->Height -= (MapStartY-MapClient->Y);
		MapClient->Y = 0;
		}
	else
		MapClient->Y -= MapStartY;

	if (MapClient->X < MapStartX)
		{
		MapClient->Width -= (MapStartX-MapClient->X);
		MapClient->X = 0;
		}
	else
		MapClient->X -= MapStartX;

	if (MapClient->Width > DX) MapClient->Width = DX;
	if (MapClient->Height > DY) MapClient->Height = DY;
	if (MapClient->Width >= Map.Properties.Width) MapClient->Width = Map.Properties.Width-1;
	if (MapClient->Height >= Map.Properties.Height) MapClient->Height = Map.Properties.Height-1;

	//--- Transforme les coordonnées en coordonnées clientes ---

	MapClient->X *= MapZoomCX;
	MapClient->Y *= MapZoomCY;
	MapClient->X += MapArea.left;
	MapClient->Y += MapArea.top;

	MapClient->Width *= MapZoomCX;
	MapClient->Height *= MapZoomCY;

	return(1);
}


// «»»» Copie (et corrige) les dimensions d'un rectangle [PAS DE CNV.] ««»

void Map_CorrectRect(MAPRECT *MapRect, MAPCLIENT *MapClient)
{
	if (MapRect->x1 > MapRect->x2)
		{
		MapClient->X = MapRect->x2;
		MapClient->Width = MapRect->x1-MapRect->x2+1;
		}
	else
		{
		MapClient->X = MapRect->x1;
		MapClient->Width = MapRect->x2-MapRect->x1+1;
		}

	if (MapRect->y1 > MapRect->y2)
		{
		MapClient->Y = MapRect->y2;
		MapClient->Height = MapRect->y1-MapRect->y2+1;
		}
	else
		{
		MapClient->Y = MapRect->y1;
		MapClient->Height = MapRect->y2-MapRect->y1+1;
		}

	return;
}


// «»»» Corrige les pos. & dim. du curseur et de la sélection «««««««««««»

void Map_CorrectAfterResize()
{
	MAPCLIENT	MapClient;

	Map_CorrectRect(&MapCursorRect,&MapClient);
	if (MapClient.Width >= Map.Properties.Width) MapClient.Width = Map.Properties.Width;
	if (MapClient.Height >= Map.Properties.Height) MapClient.Height = Map.Properties.Height;
	if (MapClient.X+MapClient.Width >= Map.Properties.Width) MapClient.X = Map.Properties.Width-MapClient.Width;
	if (MapClient.Y+MapClient.Height >= Map.Properties.Height) MapClient.Y = Map.Properties.Height-MapClient.Height;
	if (MapClient.X < 0) MapClient.X = 0;
	if (MapClient.Y < 0) MapClient.Y = 0;
	MapCursorRect.x1 = MapClient.X;
	MapCursorRect.y1 = MapClient.Y;
	MapCursorRect.x2 = MapClient.X+MapClient.Width-1;
	MapCursorRect.y2 = MapClient.Y+MapClient.Height-1;

	Map_CorrectRect(&MapCursorSel,&MapClient);
	if (MapClient.Width >= Map.Properties.Width) MapClient.Width = Map.Properties.Width;
	if (MapClient.Height >= Map.Properties.Height) MapClient.Height = Map.Properties.Height;
	if (MapClient.X+MapClient.Width >= Map.Properties.Width) MapClient.X = Map.Properties.Width-MapClient.Width;
	if (MapClient.Y+MapClient.Height >= Map.Properties.Height) MapClient.Y = Map.Properties.Height-MapClient.Height;
	if (MapClient.X < 0) MapClient.X = 0;
	if (MapClient.Y < 0) MapClient.Y = 0;
	MapCursorSel.x1 = MapClient.X;
	MapCursorSel.y1 = MapClient.Y;
	MapCursorSel.x2 = MapClient.X+MapClient.Width-1;
	MapCursorSel.y2 = MapClient.Y+MapClient.Height-1;
	return;
}


// «»»» Conversion de la position du curseur en position sur la carte «««»

void Map_CursorToMap(HWND hWnd, POINT *CursorPos)
{
	RECT	MapRect;

	Map_GetMapArea(hWnd,&MapRect);
	if (CursorPos->x < MapRect.left)
		CursorPos->x = MapRect.left;
	if (CursorPos->y < MapRect.top)
		CursorPos->y = MapRect.top;

	CursorPos->x -= MapRect.left;
	CursorPos->x /= MapZoomCX;
	CursorPos->x += MapStartX;
	CursorPos->y -= MapRect.top;
	CursorPos->y /= MapZoomCY;
	CursorPos->y += MapStartY;
	if (CursorPos->x >= Map.Properties.Width) CursorPos->x = Map.Properties.Width-1;
	if (CursorPos->y >= Map.Properties.Height) CursorPos->y = Map.Properties.Height-1;
	return;
}


// «»»» Obtient un pointeur sur la carte à une position donnée ««««««««««»

BYTE* Map_GetMapPtrFromClientPt(HWND hWnd, LONG X, LONG Y)
{
	RECT	 MapRect;
	BYTE	*MapPtr;

	Map_GetMapArea(hWnd,&MapRect);
	X -= MapRect.left;
	Y -= MapRect.top;
	X /= MapZoomCX;
	Y /= MapZoomCY;
	X += MapStartX;
	Y += MapStartY;
	if ((X >= Map.Properties.Width)||(Y >= Map.Properties.Height)) return(NULL);
	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	return(MapPtr);
}


// «»»» Test si l'élément sélectionné peut être affiché «««««««««««««««««»

int Map_CanPaint(HWND hWnd, BYTE Incompatibility)
{
	MAPCLIENT	 MapClient;
	register BYTE	*MapPtr;
	register BYTE	 MapID;
	register long	 i,j;

	if ((MapCursorMaxWidth != 0)||(MapCursorMaxHeight != 0))
		{
		if (Map_RectangleToClient(hWnd,&MapCursorRect,&MapClient))
			{
			MapPtr = Map_GetMapPtrFromClientPt(hWnd,MapClient.X,MapClient.Y);
			MapID = Map_TranslateID(Incompatibility,0x01,World);
			MapClient.Width /= MapZoomCX;
			MapClient.Height /= MapZoomCY;
			for (i = 0; i != MapClient.Height; i++)
				for (j = 0; j != MapClient.Width; j++)
					{
					if ((*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES) == MapID)&&(*(MapPtr+i*Map.Properties.Width*MAP_CONTROLBYTES+j*MAP_CONTROLBYTES+3) == 0))
						return(0);
					}
			}
		}
	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
