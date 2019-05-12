
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion de la carte plein écran.
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
extern HWND		hFullWnd;
extern HBITMAP		hFullMap;
extern HWND		hFullRoom;
extern HBITMAP		hFullRoomIcon;
extern DWORD		hFullRoomID;
extern LONG		FullDoorsOffset;
extern LONG		FullTrapsOffset;
extern LONG		FullNeutralOffset;
extern LONG		FullNeutralDelay;
extern LONG		FullRoomOffset;
extern MAP		Map;
extern NODE		MapTraps;
extern NODE		MapDoors;
extern NODE		MapCreatures;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Fullmap_CreateWindow()
{
	hFullWnd = CreateWindowEx(WS_EX_TOPMOST,szAppName,NULL,WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),hWnd,NULL,hInstance,(void *)WINDOW_FULLMAP);
	if (!hFullWnd)
		{
		Misc_PrintError(hWnd,szWindowErr,NULL,MB_ICONHAND);
		return;
		}

	ShowWindow(hFullWnd,SW_SHOWNORMAL);
	UpdateWindow(hFullWnd);
	return;
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Fullmap_DestroyWindow()
{
	if (hFullWnd) DestroyWindow(hFullWnd);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages provenants de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG Fullmap_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	HDC		hDC;
	PAINTSTRUCT	paintStruct;

	switch(uMsgId)
		{
		case WM_TIMER:
			if (FullTrapsOffset) FullTrapsOffset = 0;
			else FullTrapsOffset = 1;
			if (FullNeutralDelay)
				{
				if (FullDoorsOffset) FullDoorsOffset = 0;
				else FullDoorsOffset = 1;
				FullNeutralOffset++;
				FullRoomOffset++;
				FullNeutralDelay = -1;
				}
			FullNeutralDelay++;
			if (FullNeutralOffset > 6) FullNeutralOffset = 0;
			if (FullRoomOffset > 6) FullRoomOffset = 0;
			hDC = GetDC(hWnd);
			if (hDC)
				{
				Fullmap_PaintNeutral(hWnd,hDC);
				Fullmap_PaintTraps(hWnd,hDC);
				Fullmap_PaintDoors(hWnd,hDC);
				Fullmap_PaintCreatures(hWnd,hDC);
				ReleaseDC(hWnd,hDC);
				if (hFullRoom)
					{
					InvalidateRect(hFullRoom,NULL,FALSE);
					UpdateWindow(hFullRoom);
					}
				}
			break;

		case WM_MOUSEMOVE:
			Fullmap_PaintRoomIcon(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_LBUTTONDOWN:
			Fullmap_ScrollToPoint(hWnd,LOWORD(lParam),HIWORD(lParam));
			DestroyWindow(hWnd);
			break;

		case WM_RBUTTONDOWN:
			DestroyWindow(hWnd);
			break;

		case WM_ACTIVATE:
			switch(LOWORD(wParam))
				{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					break;
				case WA_INACTIVE:
					SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					SetWindowPos(GetForegroundWindow(),HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					break;
				}
			break;

		case WM_DRAWITEM:
			Fullmap_PaintIcon((DRAWITEMSTRUCT *)lParam);
			break;

		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			hDC = BeginPaint(hWnd,&paintStruct);
			Fullmap_PaintWindow(hWnd,hDC);
			Fullmap_PaintNeutral(hWnd,hDC);
			Fullmap_PaintTraps(hWnd,hDC);
			Fullmap_PaintDoors(hWnd,hDC);
			Fullmap_PaintCreatures(hWnd,hDC);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_DESTROY:
			KillTimer(hWnd,1);
			if (hFullRoom) DestroyWindow(hFullRoom);
			if (hFullMap) DeleteObject(hFullMap);
			if (hFullRoomIcon) DeleteObject(hFullRoomIcon);
			hFullRoomID = 0;
			hFullRoomIcon = NULL;
			hFullRoom = NULL;
			hFullMap = NULL;
			hFullWnd = NULL;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Initialisation de la fenêtre ««««««««««««««««««««««««««««««««««««»

LONG Fullmap_Create(HWND hWnd)
{
	RECT	ClientRect;
	HDC	hDC;
	HDC	hFullDC;
	HDC	hMiniDC;
	HBITMAP	hMinimap;
	HBITMAP	hMiniOld;
	HBITMAP	hFullOld;
	LONG	Prop;
	BOOL	Initialized;

	Fullmap_GetClientRect(hWnd,&ClientRect);
	Initialized = FALSE;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		hFullMap = CreateCompatibleBitmap(hDC,ClientRect.right,ClientRect.bottom);
		if (hFullMap)
			{
			hMinimap = Thumbnail_Generate(hWnd,&Map,NULL);
			if (hMinimap)
				{
				hMiniDC = CreateCompatibleDC(hDC);
				if (hMiniDC)
					{
					hFullDC = CreateCompatibleDC(hDC);
					if (hFullDC)
						{
						hMiniOld = SelectObject(hMiniDC,hMinimap);
						if (hMiniOld)
							{
							hFullOld = SelectObject(hFullDC,hFullMap);
							if (hFullOld)
								{

								if (Map.Properties.Width != Map.Properties.Height)
									{
									if (Map.Properties.Width < Map.Properties.Height)
										{
										Prop = (long)((float)ClientRect.right*((float)Map.Properties.Width/(float)Map.Properties.Height));
										BitBlt(hFullDC,0,0,(ClientRect.right-Prop)/2,ClientRect.bottom,NULL,0,0,BLACKNESS);
										BitBlt(hFullDC,(ClientRect.right-Prop)/2+Prop,0,(ClientRect.right-Prop)/2+((ClientRect.right-Prop)%2),ClientRect.bottom,NULL,0,0,BLACKNESS);
										StretchBlt(hFullDC,(ClientRect.right-Prop)/2,0,Prop,ClientRect.bottom,hMiniDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);
										}
									else // if (Map.Width > Map.Height)
										{
										Prop = (long)((float)ClientRect.bottom*((float)Map.Properties.Height/(float)Map.Properties.Width));
										BitBlt(hFullDC,0,0,ClientRect.right,(ClientRect.bottom-Prop)/2,NULL,0,0,BLACKNESS);
										BitBlt(hFullDC,0,(ClientRect.bottom-Prop)/2+Prop,ClientRect.right,(ClientRect.bottom-Prop)/2+((ClientRect.bottom-Prop)%2),NULL,0,0,BLACKNESS);
										StretchBlt(hFullDC,0,(ClientRect.bottom-Prop)/2,ClientRect.right,Prop,hMiniDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);
										}
									}
								else
									StretchBlt(hFullDC,0,0,ClientRect.right,ClientRect.bottom,hMiniDC,0,0,Map.Properties.Width,Map.Properties.Height,SRCCOPY);

								Initialized = TRUE;
								SelectObject(hFullDC,hFullOld);
								}
							SelectObject(hMiniDC,hMiniOld);
							}
						DeleteDC(hFullDC);
						}
					DeleteDC(hMiniDC);
					}
				DeleteObject(hMinimap);
				}
			}
		ReleaseDC(hWnd,hDC);
		}

	if (!Initialized) return(-1);

	SetTimer(hWnd,1,100,NULL);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage de la carte «««««««««««««««««««««««««««««««««««««««««««»

void Fullmap_PaintWindow(HWND hWnd, HDC hDC)
{
	RECT	RealClient;
	RECT	ClientRect;
	HDC	hMapDC;
	HBITMAP	hMapBitmap;

	GetClientRect(hWnd,&RealClient);
	Fullmap_GetClientRect(hWnd,&ClientRect);

	hMapDC = CreateCompatibleDC(hDC);
	if (hMapDC)
		{
		hMapBitmap = SelectObject(hMapDC,hFullMap);
		if (hMapBitmap)
			{
			if (ClientRect.left != 0)
				{
				BitBlt(hDC,0,0,ClientRect.left,ClientRect.bottom,NULL,0,0,BLACKNESS);
				BitBlt(hDC,ClientRect.right,0,RealClient.right,ClientRect.bottom,NULL,0,0,BLACKNESS);
				}
			if (ClientRect.top != 0)
				{
				BitBlt(hDC,0,0,ClientRect.right,ClientRect.top,NULL,0,0,BLACKNESS);
				BitBlt(hDC,0,ClientRect.bottom,ClientRect.right,RealClient.bottom,NULL,0,0,BLACKNESS);
				}
			BitBlt(hDC,ClientRect.left,ClientRect.top,ClientRect.right,ClientRect.bottom,hMapDC,0,0,SRCCOPY);

			SelectObject(hMapDC,hMapBitmap);
			}
		DeleteDC(hMapDC);
		}

	return;
}


// «»»» Affichage des terrains neutres ««««««««««««««««««««««««««««««««««»

void Fullmap_PaintNeutral(HWND hWnd, HDC hDC)
{
	RECT	 ClientRect;
	RECT	 PaintRect;
	HBRUSH	 hBrush,hOldBrush;
	LONG	 X,Y;
	BYTE	*MapPtr;
	BYTE	*MapLinePtr;

	hBrush = CreateSolidBrush(WallsCols[FullRoomOffset*2]);
	if (!hBrush)
		return;
	hOldBrush = SelectObject(hDC,hBrush);
	if (!hOldBrush)
		{
		DeleteObject(hBrush);
		return;
		}

	Fullmap_GetClientRect(hWnd,&ClientRect);
	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (*(MapLinePtr+1) != 2) continue;
			switch(*(MapLinePtr+3))
				{
				case 0:	if (World[*MapLinePtr].Id != 0x1F020101) break;
					goto Set_Neutral;
				case 1:	goto Set_Neutral;
				case 2:	break;
				case 3:	if ((Walls[*MapLinePtr].Id&0xFF000000) != 0x08000000) break;
					goto Set_Neutral;
				}
			continue;
		Set_Neutral:
			Fullmap_MapToFullmap(&ClientRect,X,Y,&PaintRect.left,&PaintRect.top);
			Fullmap_MapToFullmap(&ClientRect,X+1,Y+1,&PaintRect.right,&PaintRect.bottom);
			PaintRect.right++;
			PaintRect.bottom++;
			FillRect(hDC,&PaintRect,hBrush);
			continue;
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	return;
}


// «»»» Affichage des pièges ««««««««««««««««««««««««««««««««««««««««««««»

void Fullmap_PaintTraps(HWND hWnd, HDC hDC)
{
	RECT		 ClientRect;
	HPEN		 hPen,hOldPen;
	HBRUSH		 hOldBrush;
	MAPTHING	*Trap;
	DWORD		 Color;
	LONG		 A,B,C,D;
	LONG		 X1,Y1,X2,Y2;

	Fullmap_GetClientRect(hWnd,&ClientRect);
	hOldBrush = SelectObject(hDC,GetStockObject(NULL_BRUSH));
	if (!hOldBrush) return;

	for (Trap = (MAPTHING *)MapTraps.next; Trap != 0; Trap = (MAPTHING *)Trap->node.next)
		{
		Fullmap_MapToFullmap(&ClientRect,Trap->x,Trap->y,&A,&B);
		Fullmap_MapToFullmap(&ClientRect,Trap->x+1,Trap->y+1,&C,&D);
		if (Trap->owner == 2) Color = WallsCols[FullNeutralOffset*2];
		else Color = WallsCols[(Trap->owner-1)*2+FullTrapsOffset];
		hPen = CreatePen(PS_SOLID,2,Color);
		if (hPen)
			{
			hOldPen = SelectObject(hDC,hPen);
			if (hOldPen)
				{
				if (A+2 >= C-2)
					{
					X1 = A;
					X2 = C;
					}
				else
					{
					X1 = A+2;
					X2 = C-2;
					}
				if (B+2 >= D-2)
					{
					Y1 = B;
					Y2 = D;
					}
				else
					{
					Y1 = B+2;
					Y2 = D-2;
					}
				Ellipse(hDC,X1,Y1,X2,Y2);
				SelectObject(hDC,hOldPen);
				}
			DeleteObject(hPen);
			}
		}

	SelectObject(hDC,hOldBrush);
	return;
}


// «»»» Affichage des portes ««««««««««««««««««««««««««««««««««««««««««««»

void Fullmap_PaintDoors(HWND hWnd, HDC hDC)
{
	RECT		 ClientRect;
	RECT		 PaintRect;
	HBRUSH		 hBrush,hOldBrush;
	MAPTHING	*Door;
	DWORD		 Color;
	LONG		 A,B,C,D;

	Fullmap_GetClientRect(hWnd,&ClientRect);

	for (Door = (MAPTHING *)MapDoors.next; Door != 0; Door = (MAPTHING *)Door->node.next)
		{
		if (Door->owner == 2) Color = WallsCols[FullNeutralOffset*2];
		else Color = WallsCols[(Door->owner-1)*2+((FullDoorsOffset)?0:1)];
		Fullmap_MapToFullmap(&ClientRect,Door->x,Door->y,&A,&B);
		Fullmap_MapToFullmap(&ClientRect,Door->x+1,Door->y+1,&C,&D);
		hBrush = CreateSolidBrush(Color);
		if (hBrush)
			{
			hOldBrush = SelectObject(hDC,hBrush);
			if (hOldBrush)
				{
				if (A+2 >= C-2)
					{
					PaintRect.left = A;
					PaintRect.right = C;
					}
				else
					{
					PaintRect.left = A+2;
					PaintRect.right = C-2;
					}
				if (B+2 >= D-2)
					{
					PaintRect.top = B;
					PaintRect.bottom = D;
					}
				else
					{
					PaintRect.top = B+2;
					PaintRect.bottom = D-2;
					}
				FillRect(hDC,&PaintRect,hBrush);
				SelectObject(hDC,hOldBrush);
				}
			DeleteObject(hBrush);
			}
		}

	return;
}


// «»»» Affichage des créatures «««««««««««««««««««««««««««««««««««««««««»

void Fullmap_PaintCreatures(HWND hWnd, HDC hDC)
{
	RECT		 ClientRect;
	RECT		 PaintRect;
	HBRUSH		 hBrush,hOldBrush;
	MAPTHING	*Creature;
	DWORD		 Color;
	LONG		 A,B,C,D;

	Fullmap_GetClientRect(hWnd,&ClientRect);

	for (Creature = (MAPTHING *)MapCreatures.next; Creature != 0; Creature = (MAPTHING *)Creature->node.next)
		{
		if (Creature->owner == 2) Color = WallsCols[FullNeutralOffset*2];
		else Color = WallsCols[(Creature->owner-1)*2];
		Fullmap_MapToFullmap(&ClientRect,Creature->x,Creature->y,&A,&B);
		Fullmap_MapToFullmap(&ClientRect,Creature->x+1,Creature->y+1,&C,&D);
		hBrush = CreateSolidBrush(Color);
		if (hBrush)
			{
			hOldBrush = SelectObject(hDC,hBrush);
			if (hOldBrush)
				{
				PaintRect.left = A+(C-A-3)/2;
				PaintRect.top = B+(D-B-3)/2;
				PaintRect.right = PaintRect.left+3;
				PaintRect.bottom = PaintRect.top+3;
				FillRect(hDC,&PaintRect,hBrush);
				SelectObject(hDC,hOldBrush);
				}
			DeleteObject(hBrush);
			}
		}

	return;
}


// «»»» Affichage de l'icône de la salle concernée ««««««««««««««««««««««»

void Fullmap_PaintRoomIcon(HWND hWnd, LONG X, LONG Y)
{
	RECT	 ClientRect;
	LONG	 WX,WY;
	LONG	 SX,SY;
	LONG	 Square;
	BYTE	*MapPtr;
	int	 Res;

	Fullmap_GetClientRect(hWnd,&ClientRect);
	Fullmap_FullmapToMap(&ClientRect,X,Y,&SX,&SY);

	MapPtr = Map.Map+SX*MAP_CONTROLBYTES+SY*Map.Properties.Width*MAP_CONTROLBYTES;
	switch(*(MapPtr+3))
		{
		case 0:	Square = World[*MapPtr].Id;
			Res = World[*MapPtr].Icon;
			if (Square != 0x1F020101) Square = 0;
			break;
		case 1:	Square = Rooms[*MapPtr].Id;
			Res = Rooms[*MapPtr].Icon;
			break;
		case 2:	Square = Gates[*MapPtr].Id;
			Res = Gates[*MapPtr].Icon;
			break;
		default:Square = 0;
			break;
		}

	if (!Square)
		{
		if (hFullRoom) DestroyWindow(hFullRoom);
		if (hFullRoomIcon) DeleteObject(hFullRoomIcon);
		hFullRoom = NULL;
		hFullRoomIcon = NULL;
		hFullRoomID = 0;
		return;
		}

	WX = X+16;
	WY = Y+16;
	GetClientRect(hWnd,&ClientRect);
	if (WX+32 > ClientRect.right) WX = ClientRect.right-32;
	if (WY+32 > ClientRect.bottom) WY = ClientRect.bottom-32;

	if (hFullRoomID != (Square&0xFF000000))
		goto Create_New;
	if (!hFullRoom)
		goto Create_New;

	SetWindowPos(hFullRoom,NULL,WX,WY,0,0,SWP_NOSIZE|SWP_NOZORDER);
	return;

//--- Création d'un nouvel icône ---

Create_New:
	if (hFullRoom) DestroyWindow(hFullRoom);
	if (hFullRoomIcon) DeleteObject(hFullRoomIcon);
	hFullRoom = NULL;
	hFullRoomIcon = NULL;

	hFullRoomID = Square&0xFF000000;
	hFullRoomIcon = LoadImage(hInstance,MAKEINTRESOURCE(Res),IMAGE_BITMAP,32,32,LR_DEFAULTCOLOR);
	if (!hFullRoomIcon)
		{
		hFullRoomID = 0;
		return;
		}

	hFullRoom = CreateWindowEx(0,szStaticClass,NULL,WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,WX,WY,32,32,hWnd,NULL,hInstance,NULL);
	if (!hFullRoom)
		{
		DeleteObject(hFullRoomIcon);
		hFullRoomIcon = NULL;
		hFullRoomID = 0;
		}

	return;
}


// «»»» Affichage de l'icône ««««««««««««««««««««««««««««««««««««««««««««»

void Fullmap_PaintIcon(DRAWITEMSTRUCT *DrawStruct)
{
	HDC	hDC;
	HBITMAP	hOldBitmap;

	if (DrawStruct->CtlType != ODT_STATIC) return;

	hDC = CreateCompatibleDC(DrawStruct->hDC);
	if (hDC)
		{
		hOldBitmap = SelectObject(hDC,hFullRoomIcon);
		if (hOldBitmap)
			{
			BitBlt(DrawStruct->hDC,0,0,32,32,hDC,0,0,SRCCOPY);
			SelectObject(hDC,hOldBitmap);
			}
		DeleteDC(hDC);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Place la carte à l'endroit où l'utilisateur a cliqué ««««««««««««»

void Fullmap_ScrollToPoint(HWND hClient, LONG X, LONG Y)
{
	RECT	ClientRect;
	LONG	SX,SY;

	Fullmap_GetClientRect(hClient,&ClientRect);
	Fullmap_FullmapToMap(&ClientRect,X,Y,&SX,&SY);
	Map_ScrollToPoint(hWnd,SX,SY,0);
	Mouse_MovedUpdate(hWnd);
	return;
}


// «»»» Conversion de coordonnées «««««««««««««««««««««««««««««««««««««««»

//--- Carte plein écran -> Carte ---

void Fullmap_FullmapToMap(RECT *ClientRect, LONG X, LONG Y, LONG *RX, LONG *RY)
{
	float	SX,SY;
	float	MX,MY;
	float	PropX,PropY;

	MX = (float)(ClientRect->right);
	MY = (float)(ClientRect->bottom);

	PropX = 0;
	PropY = 0;
	if (Map.Properties.Width != Map.Properties.Height)
		{
		if (Map.Properties.Width < Map.Properties.Height)
			{
			MX = (float)ClientRect->right*((float)Map.Properties.Width/(float)Map.Properties.Height);
			PropX = ((float)ClientRect->right-MX)/2.0f;
			}
		else // if (MapWidth > MapHeight)
			{
			MY = (float)ClientRect->bottom*((float)Map.Properties.Height/(float)Map.Properties.Width);
			PropY = ((float)ClientRect->bottom-MY)/2.0f;
			}
		}

	SX = ((float)X-(float)ClientRect->left-PropX)/MX;
	SY = ((float)Y-(float)ClientRect->top-PropY)/MY;
	SX *= (float)Map.Properties.Width;
	SY *= (float)Map.Properties.Height;

	if (SX < 0.0f) SX = 0.0f;
	if (SY < 0.0f) SY = 0.0f;
	if (SX >= (float)Map.Properties.Width) SX = (float)Map.Properties.Width-1.0f;
	if (SY >= (float)Map.Properties.Height) SY = (float)Map.Properties.Height-1.0f;

	*RX = (long)SX;
	*RY = (long)SY;
	return;
}

//--- Carte -> Carte plein écran ---

void Fullmap_MapToFullmap(RECT *ClientRect, LONG X, LONG Y, LONG *RX, LONG *RY)
{
	float	MX,MY;
	float	SX,SY;
	float	PropX,PropY;

	SX = (float)X/(float)(Map.Properties.Width);
	SY = (float)Y/(float)(Map.Properties.Height);
	MX = (float)(ClientRect->right);
	MY = (float)(ClientRect->bottom);

	PropX = 0;
	PropY = 0;
	if (Map.Properties.Width != Map.Properties.Height)
		{
		if (Map.Properties.Width < Map.Properties.Height)
			{
			MX = (float)ClientRect->right*((float)Map.Properties.Width/(float)Map.Properties.Height);
			PropX = ((float)ClientRect->right-MX)/2.0f;
			}
		else // if (MapWidth > MapHeight)
			{
			MY = (float)ClientRect->bottom*((float)Map.Properties.Height/(float)Map.Properties.Width);
			PropY = ((float)ClientRect->bottom-MY)/2.0f;
			}
		}

	*RX = (long)(MX*SX+PropX)+ClientRect->left;
	*RY = (long)(MY*SY+PropY)+ClientRect->top;
	return;
}


// «»»» Obtention des dimensions de la carte ««««««««««««««««««««««««««««»

void Fullmap_GetClientRect(HWND hWnd, RECT *ClientRect)
{
	GetClientRect(hWnd,ClientRect);
	if (ClientRect->right > ClientRect->bottom)
		{
		ClientRect->left = (ClientRect->right-ClientRect->bottom)/2;
		ClientRect->right = ClientRect->bottom;
		}
	if (ClientRect->right < ClientRect->bottom)
		{
		ClientRect->top = (ClientRect->bottom-ClientRect->right)/2;
		ClientRect->bottom = ClientRect->right;
		}
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
