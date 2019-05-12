
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines des caméras des joueurs
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
extern HICON		hMapCameras[];
extern MAP		Map;
extern LONG		MapLocked;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern MAPOPTIONS	MapOptions[];
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création et destruction des caméras				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création des caméras ««««««««««««««««««««««««««««««««««««««««««««»

int Map_CreateCameras()
{
	int	i;

	for (i = 0; i != 4+1; i++)
		{
		hMapCameras[i] = LoadImage(hInstance,MAKEINTRESOURCE(20+i),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		if (!hMapCameras[i]) return(0);
		}

	return(1);
}


// «»»» Destruction des caméras «««««««««««««««««««««««««««««««««««««««««»

void Map_DestroyCameras()
{
	int	i;

	for (i = 0; i != 4+1; i++)
		{
		if (hMapCameras[i]) DestroyIcon(hMapCameras[i]);
		hMapCameras[i] = NULL;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage des caméras						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Map_DrawCameras(HWND hWnd, HDC hDC)
{
	RECT	MapRect;
	LONG	DX,DY;
	LONG	X,Y,A,B,C,D;
	HPEN	hPen,hOldPen;
	HBRUSH	hOldBrush;
	int	i,j,k,l;

	if ((!Map.Map)||(MapLocked)) return;
	if (!(Config.MapViewFlags&MVF_CAMERAS)) return;
	Map_GetMapArea(hWnd,&MapRect);
	Map_GetMapSquares(NULL,&MapRect,&DX,&DY);
	if (DX+MapStartX > Map.Properties.Width) DX = Map.Properties.Width;
	if (DY+MapStartY > Map.Properties.Height) DY = Map.Properties.Height;

	for (k = 0, i = 0; i != 4+1; i++, k++)
		{
		if (MapOptions[i+2].PlayerCameraX == -1) continue;
		if (MapOptions[i+2].PlayerCameraY == -1) continue;
		if (MapOptions[i+2].PlayerCameraX < MapStartX) continue;
		if (MapOptions[i+2].PlayerCameraY < MapStartY) continue;
		if (MapOptions[i+2].PlayerCameraX > MapStartX+DX) continue;
		if (MapOptions[i+2].PlayerCameraY > MapStartY+DY) continue;

		for (l = 0, j = 0; j != 4+1; j++)
			{
			if (j == i) continue;
			if (MapOptions[j+2].PlayerCameraX == -1) continue;
			if (MapOptions[j+2].PlayerCameraX != MapOptions[i+2].PlayerCameraX) continue;
			if (MapOptions[j+2].PlayerCameraY == MapOptions[i+2].PlayerCameraY) l++;
			}
	
		X  = MapOptions[i+2].PlayerCameraX;
		Y  = MapOptions[i+2].PlayerCameraY;
		X -= MapStartX;
		Y -= MapStartY;
		X *= MapZoomCX;
		Y *= MapZoomCY;
		X += MapRect.left;
		Y += MapRect.top;
		X += (MapZoomCX-16)/2;
		Y += (MapZoomCY-16)/2;

		hOldBrush = SelectObject(hDC,GetStockObject(NULL_BRUSH));
		if (hOldBrush)
			{
			hPen = CreatePen(PS_SOLID,2,Players[i+2].Color);
			if (hPen)
				{
				hOldPen = SelectObject(hDC,hPen);
				if (hOldPen)
					{
					A = X-MapZoomCX*2+8;
					B = Y-MapZoomCY*2+8;
					C = X+MapZoomCX*2+8;
					D = Y+MapZoomCY*2+8;

					if (l)
						{
						A -= (l-k)*2;
						B -= (l-k)*2;
						C += (l-k)*2;
						D += (l-k)*2;
						}

					Ellipse(hDC,A,B,C,D);
					SelectObject(hDC,hOldPen);
					}
				DeleteObject(hPen);
				}
			SelectObject(hDC,hOldBrush);
			}

		if (l)
			{
			X -= (l-k)*2;
			Y -= (l-k)*2;
			}

		DrawIconEx(hDC,X,Y,hMapCameras[i],16,16,0,NULL,DI_NORMAL);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
