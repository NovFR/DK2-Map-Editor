
//<<>-<>>---------------------------------------------------------------------()
/*
	Barre d'outils.
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
extern HMENU		hMenu;
extern HWND		hWnd;
extern HWND		hStatusWnd;
extern HWND		hToolbar;
extern HBRUSH		hToolbrush;
extern HBITMAP		hSquare32;
extern HIMAGELIST	hWorld;
extern HIMAGELIST	hRooms;
extern HIMAGELIST	hWalls;
extern HIMAGELIST	hGates;
extern HIMAGELIST	hMiscTools;
extern HIMAGELIST	hDrawingTools;
extern FONTINFO		Font;
extern CONFIG		Config;
extern DWORD		ToolsPlyrID;
extern DWORD		ToolsGateID;
extern DWORD		ToolsItemID;
extern DWORD		WorldItemID;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

int Toolbar_CreateWindow(HWND hWnd)
{
	RECT	ClientRect;
	HBITMAP	hBitmap;

	//--- Création de l'image de fond ---

	hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(999),IMAGE_BITMAP,115,74,LR_DEFAULTCOLOR);
	if (!hBitmap) return(0);
	hToolbrush = CreatePatternBrush(hBitmap);
	DeleteObject(hBitmap);
	if (!hToolbrush) return(0);

	//--- Création de la fenêtre ---

	GetClientRect(hWnd,&ClientRect);
	if (ClientRect.right < 1024) ClientRect.right = 1024;
	hToolbar = CreateWindow(szButtonClass,NULL,WS_CHILD|BS_OWNERDRAW,0,0,ClientRect.right,74,hWnd,NULL,hInstance,NULL);
	if (!hToolbar) return(0);

	if (!Config.HideToolbar)
		{
		ShowWindow(hToolbar,SW_SHOWNORMAL);
		UpdateWindow(hToolbar);
		}

	return(1);
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Toolbar_DestroyWindow()
{
	if (hToolbrush) DeleteObject(hToolbrush);
	hToolbrush = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Cache/Montre la fenêtre «««««««««««««««««««««««««««««««««««««««««»

void Toolbar_Hide(BOOL Hide)
{
	if (!IsWindow(hToolbar)) return;

	if (!Hide)
		{
		if (IsWindowVisible(hToolbar)) return;
		ShowWindow(hToolbar,SW_SHOW);
		}
	else
		{
		if (!IsWindowVisible(hToolbar)) return;
		ShowWindow(hToolbar,SW_HIDE);
		}

	Window_RedrawAll();
	return;
}


// «»»» Affichage des différents boutons ««««««««««««««««««««««««««««««««»

void Toolbar_DrawWindow(DRAWITEMSTRUCT *ItemStruct)
{
	HDC		hDC;
	HDC		hWorkDC;
	HDC		hDoubleDC;
	HBITMAP		hDoubleBitmap;
	HBITMAP		hDoubleOldBitmap;
	HBITMAP		hOldBitmap;
	RECT		WorkRect;
	RECT		PlayerRect;
	HBRUSH		PlayerBrush;
	HFONT		hOldFont;
	COLORREF	TextColor;
	COLORREF	BackColor;
	int		i;

	if ((ItemStruct->itemAction&ODA_SELECT)||(ItemStruct->itemAction&ODA_FOCUS)) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	hWorkDC = ItemStruct->hDC;
	hDoubleDC = CreateCompatibleDC(ItemStruct->hDC);
	if (hDoubleDC)
		{
		hDoubleBitmap = CreateCompatibleBitmap(ItemStruct->hDC,ItemStruct->rcItem.right-ItemStruct->rcItem.left,ItemStruct->rcItem.bottom-ItemStruct->rcItem.top);
		if (hDoubleBitmap)
			{
			hDoubleOldBitmap = SelectObject(hDoubleDC,hDoubleBitmap);
			if (!hDoubleOldBitmap)
				{
				DeleteObject(hDoubleBitmap);
				DeleteDC(hDoubleDC);
				hDoubleDC = NULL;
				}
			else
				{
				hWorkDC = hDoubleDC;
				WorkRect.right -= WorkRect.left;
				WorkRect.bottom -= WorkRect.top;
				WorkRect.left = 0;
				WorkRect.top = 0;
				}
			}
		else
			{
			DeleteDC(hDoubleDC);
			hDoubleDC = NULL;
			}
		}

	DrawEdge(hWorkDC,&WorkRect,BDR_SUNKENOUTER,BF_TOP);
	DrawEdge(hWorkDC,&WorkRect,BDR_RAISEDOUTER|BDR_RAISEDINNER,BF_BOTTOM);
	WorkRect.top++;
	WorkRect.bottom -= 2;
	FillRect(hWorkDC,&WorkRect,hToolbrush);
	WorkRect.top++;

	hDC = CreateCompatibleDC(ItemStruct->hDC);
	if (hDC) hOldBitmap = SelectObject(hDC,hSquare32);
	else hOldBitmap = NULL;

	//--- Affichage des éléments du monde ---

	for (i = 0; i != 5; i++)
		{
		ImageList_Draw(hWorld,i,hWorkDC,WorkRect.left+2+i*32,WorkRect.top+2,ILD_NORMAL);
		if ((World[i].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+i*32,WorkRect.top+2,32,32,hDC,0,0,SRCPAINT);
		}
	for (i = 0; i != 5; i++)
		{
		ImageList_Draw(hWorld,i+5,hWorkDC,WorkRect.left+2+i*32,WorkRect.top+34,ILD_NORMAL);
		if ((World[i+5].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+i*32,WorkRect.top+34,32,32,hDC,0,0,SRCPAINT);
		}

	ImageList_Draw(hGates,ToolsGateID,hWorkDC,WorkRect.left+4+32*5,WorkRect.top+18,ILD_NORMAL);
	if ((Gates[ToolsGateID].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+4+32*5,WorkRect.top+18,32,32,hDC,0,0,SRCPAINT);

	//--- Affichage du joueur ---

	PlayerRect.left = WorkRect.left+4+32*5+34+10;
	PlayerRect.top = WorkRect.top+18+(32-Font.FontHeight)/2-4;
	PlayerRect.right = PlayerRect.left+120;
	PlayerRect.bottom = PlayerRect.top+Font.FontHeight+9;
	FillRect(hWorkDC,&PlayerRect,GetStockObject(BLACK_BRUSH));

	PlayerBrush = CreateSolidBrush(Players[ToolsPlyrID].Color);
	if (PlayerBrush)
		{
		PlayerRect.left = WorkRect.left+4+32*5+38+10;
		PlayerRect.top = WorkRect.top+18+(32-Font.FontHeight)/2;
		PlayerRect.right = PlayerRect.left+16;
		PlayerRect.bottom = PlayerRect.top+Font.FontHeight;
		FillRect(hWorkDC,&PlayerRect,PlayerBrush);
		DeleteObject(PlayerBrush);
		}

	hOldFont = SelectObject(hWorkDC,Font.hFont);
	if (hOldFont)
		{
		TextColor = SetTextColor(hWorkDC,0x00FFFFFF);
		BackColor = SetBkColor(hWorkDC,0x00000000);
		PlayerRect.left = WorkRect.left+4+32*5+38+20+10;
		PlayerRect.top = WorkRect.top+18+(32-Font.FontHeight)/2;
		PlayerRect.right = PlayerRect.left+90;
		PlayerRect.bottom = PlayerRect.top+Font.FontHeight;
		DrawText(hWorkDC,Players[ToolsPlyrID].Name,-1,&PlayerRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		DrawText(hWorkDC,szRight,-1,&PlayerRect,DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkColor(hWorkDC,BackColor);
		SetTextColor(hWorkDC,TextColor);
		SelectObject(hWorkDC,hOldFont);
		}

	//--- Affichage des murs ---

	ImageList_Draw(hWalls,ToolsPlyrID*2,hWorkDC,WorkRect.left+2+32*5+158+10,WorkRect.top+2,ILD_NORMAL);
	if ((Walls[ToolsPlyrID*2].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+32*5+158+10,WorkRect.top+2,32,32,hDC,0,0,SRCPAINT);
	ImageList_Draw(hWalls,ToolsPlyrID*2+1,hWorkDC,WorkRect.left+2+32*5+158+10,WorkRect.top+2+32,ILD_NORMAL);
	if ((Walls[ToolsPlyrID*2+1].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+32*5+158+10,WorkRect.top+2+32,32,32,hDC,0,0,SRCPAINT);

	//--- Affichage des salles ---

	for (i = 0; i != 12; i++)
		{
		ImageList_Draw(hRooms,i,hWorkDC,WorkRect.left+2+32*5+158+44+i*32,WorkRect.top+2,ILD_NORMAL);
		if ((Rooms[i].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+32*5+158+44+i*32,WorkRect.top+2,32,32,hDC,0,0,SRCPAINT);
		}
	for (i = 0; i != 11; i++)
		{
		ImageList_Draw(hRooms,i+12,hWorkDC,WorkRect.left+2+32*5+158+44+i*32,WorkRect.top+2+32,ILD_NORMAL);
		if ((Rooms[i+12].Id == WorldItemID)&&(hOldBitmap)) BitBlt(hWorkDC,WorkRect.left+2+32*5+158+44+i*32,WorkRect.top+2+32,32,32,hDC,0,0,SRCPAINT);
		}

	//--- Affichage des outils ---

	for (i = 0; i != 7; i++)
		{
		ImageList_Draw(hDrawingTools,i,hWorkDC,WorkRect.left+2+32*5+600+i*22,WorkRect.top+12,ILD_NORMAL);
		if ((DrawingTools[i].Id == ToolsItemID)&&(hOldBitmap)) StretchBlt(hWorkDC,WorkRect.left+2+32*5+600+i*22,WorkRect.top+12,20,20,hDC,0,0,32,32,SRCPAINT);
		}
	for (i = 0; i != 5; i++)
		{
		ImageList_Draw(hMiscTools,i,hWorkDC,WorkRect.left+2+32*5+600+i*22,WorkRect.top+34,ILD_NORMAL);
		if ((MiscTools[i].Id == ToolsItemID)&&(hOldBitmap)) StretchBlt(hWorkDC,WorkRect.left+2+32*5+600+i*22,WorkRect.top+34,20,20,hDC,0,0,32,32,SRCPAINT);
		}

	//--- Supprime les données allouées ---

	if (hDoubleDC)
		{
		BitBlt(ItemStruct->hDC,ItemStruct->rcItem.left,ItemStruct->rcItem.top,ItemStruct->rcItem.right-ItemStruct->rcItem.left,ItemStruct->rcItem.bottom-ItemStruct->rcItem.top,hDoubleDC,0,0,SRCCOPY);
		SelectObject(hDoubleDC,hDoubleOldBitmap);
		DeleteObject(hDoubleBitmap);
		DeleteDC(hDoubleDC);
		}

	if (hDC)
		{
		if (hOldBitmap) SelectObject(hDC,hOldBitmap);
		DeleteDC(hDC);
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélection avec le bouton gauche de la souris ««««««««««««««««««««»

void Toolbar_Clicked()
{
	POINT	CursorPos;
	RECT	HitRect;

	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hToolbar,&CursorPos,1);

	//--- Test si un élément du monde a été sélectionné ---

	HitRect.left = 2;
	HitRect.top = 4;
	HitRect.right = 2+32*5;
	HitRect.bottom = 4+64;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= 2;
		CursorPos.x /= 32;
		CursorPos.y -= 4;
		CursorPos.y /= 32;
		if (CursorPos.y) CursorPos.x += 5;
		if (WorldItemID == World[CursorPos.x].Id) return;
		WorldItemID = World[CursorPos.x].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,CursorPos.x+IDM_MAPWORLDSTRT,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	//--- Test si le portail des héros a été sélectionné ---

	HitRect.left = 2+4+32*5;
	HitRect.top = 2+18;
	HitRect.right = HitRect.left+32;
	HitRect.bottom = HitRect.top+32;
	if (PtInRect(&HitRect,CursorPos))
		{
		if (WorldItemID == Gates[ToolsGateID].Id)
			{
			if (Gates[ToolsGateID+1].Id == 0)
				ToolsGateID = 0;
			else
				ToolsGateID++;
			}
		WorldItemID = Gates[ToolsGateID].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,IDM_MAPWORLDHEROPORTALSSTRT+ToolsGateID,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	//--- Test si un mur a été sélectionné ---

	HitRect.left = 2+32*5+158+10;
	HitRect.top = 4;
	HitRect.right = HitRect.left+32;
	HitRect.bottom = HitRect.top+32;
	if (PtInRect(&HitRect,CursorPos))
		{
		if (WorldItemID == Walls[ToolsPlyrID*2].Id) return;
		WorldItemID = Walls[ToolsPlyrID*2].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,IDM_MAPWORLDWALL,TRUE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}
	HitRect.top += 32;
	HitRect.bottom += 32;
	if (PtInRect(&HitRect,CursorPos))
		{
		if (WorldItemID == Walls[ToolsPlyrID*2+1].Id) return;
		WorldItemID = Walls[ToolsPlyrID*2+1].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,IDM_MAPWORLDCLAIMED,TRUE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	//--- Test si une salle a été sélectionnée ---

	HitRect.left = 2+32*5+158+44;
	HitRect.top = 4;
	HitRect.right = HitRect.left+12*32;
	HitRect.bottom = HitRect.top+32;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= HitRect.left;
		CursorPos.x /= 32;
		if (WorldItemID == Rooms[CursorPos.x].Id) return;
		WorldItemID = Rooms[CursorPos.x].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,IDM_ROOMSSTRT+CursorPos.x,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	HitRect.top = HitRect.bottom;
	HitRect.right -= 32;
	HitRect.bottom = HitRect.top+32;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= HitRect.left;
		CursorPos.x /= 32;
		if (WorldItemID == Rooms[CursorPos.x+12].Id) return;
		WorldItemID = Rooms[CursorPos.x+12].Id;
		Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
		Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,IDM_ROOMSSTRT+CursorPos.x+10,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
		Sound_Play(SOUND_INTERFACE,WorldItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	//--- Test si un outils a été sélectionné ---

	HitRect.left = 2+32*5+600;
	HitRect.top = 2+12;
	HitRect.right = HitRect.left+22*7;
	HitRect.bottom = HitRect.top+20;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= HitRect.left;
		CursorPos.x /= 22;
		if (ToolsItemID == DrawingTools[CursorPos.x].Id) return;
		ToolsItemID = DrawingTools[CursorPos.x].Id;
		Misc_CheckMenuItem(hMenu,IDM_PAINTMODESTRT,IDM_PAINTMODESTOP,CursorPos.x+IDM_PAINTMODESTRT,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_PUTSTRT,IDM_PUTSTOP,0,FALSE);
		Sound_Play(SOUND_PAINTMODE,ToolsItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TOOLPART|SBT_OWNERDRAW,(LPARAM)NULL);
		return;
		}

	HitRect.top = HitRect.bottom+2;
	HitRect.right = HitRect.left+22*5;
	HitRect.bottom = HitRect.top+20;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= HitRect.left;
		CursorPos.x /= 22;
		if (ToolsItemID == MiscTools[CursorPos.x].Id)
			{
			SendMessage(hWnd,WM_COMMAND,(WPARAM)IDM_MODIFYSTRT+CursorPos.x,(LPARAM)hWnd);
			return;
			}
		ToolsItemID = MiscTools[CursorPos.x].Id;
		Misc_CheckMenuItem(hMenu,IDM_PAINTMODESTRT,IDM_PAINTMODESTOP,IDM_PUTELEM,TRUE);
		Misc_CheckMenuItem(hMenu,IDM_PUTSTRT,IDM_PUTSTOP,CursorPos.x+IDM_PUTSTRT,TRUE);
		Sound_Play(SOUND_PAINTMODE,ToolsItemID);
		InvalidateRect(hToolbar,NULL,FALSE);
		UpdateWindow(hToolbar);
		SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TOOLPART|SBT_OWNERDRAW,(LPARAM)NULL);
		return;
		}

	//--- Test si le joueur a été sélectionné ---

	HitRect.left = 2+4+32*5+34+10;
	HitRect.top = 2+18+(32-Font.FontHeight)/2-4;
	HitRect.right = HitRect.left+120;
	HitRect.bottom = HitRect.top+Font.FontHeight+9;
	if (PtInRect(&HitRect,CursorPos))
		{
		if (Players[ToolsPlyrID+1].Name == NULL)
			{
			if (WorldItemID == Walls[ToolsPlyrID*2].Id)
				WorldItemID = Walls[0].Id;
			if (WorldItemID == Walls[ToolsPlyrID*2+1].Id)
				WorldItemID = Walls[1].Id;
			ToolsPlyrID = 0;
			}
		else
			{
			if (WorldItemID == Walls[ToolsPlyrID*2].Id)
				WorldItemID = Walls[ToolsPlyrID*2+2].Id;
			if (WorldItemID == Walls[ToolsPlyrID*2+1].Id)
				WorldItemID = Walls[ToolsPlyrID*2+2+1].Id;
			ToolsPlyrID++;
			}
		Misc_CheckMenuItem(hMenu,IDM_PLAYERSSTRT,IDM_PLAYERSSTOP,ToolsPlyrID+IDM_PLAYERSSTRT,TRUE);
		InvalidateRect(hToolbar,&HitRect,FALSE);
		HitRect.left = 2+32*5+158+10;
		HitRect.top = 4;
		HitRect.right = HitRect.left+32;
		HitRect.bottom = HitRect.top+64;
		InvalidateRect(hToolbar,&HitRect,FALSE);
		UpdateWindow(hToolbar);
		return;
		}

	return;
}


// «»»» Sélection avec le bouton droit de la souris «««««««««««««««««««««»

void Toolbar_RightClicked(LONG X, LONG Y)
{
	RECT	HitRect;
	POINT	CursorPos;

	CursorPos.x = X;
	CursorPos.y = Y;
	MapWindowPoints(NULL,hToolbar,&CursorPos,1);

	//--- Test si le joueur a été sélectionné ---

	HitRect.left = 2+4+32*5+34+10;
	HitRect.top = 2+18+(32-Font.FontHeight)/2-4;
	HitRect.right = HitRect.left+120;
	HitRect.bottom = HitRect.top+Font.FontHeight+9;
	if (PtInRect(&HitRect,CursorPos))
		{
		Misc_DisplayContextMenu(hWnd,0,PlayersMenu,X,Y,0);
		return;
		}

	//--- Test si le portail des héros a été sélectionné ---

	HitRect.left = 2+4+32*5;
	HitRect.top = 2+18;
	HitRect.right = HitRect.left+32;
	HitRect.bottom = HitRect.top+32;
	if (PtInRect(&HitRect,CursorPos))
		{
		Misc_DisplayContextMenu(hWnd,1,GatesMenu,X,Y,0);
		return;
		}


	//--- Test si un outils a été sélectionné ---

	HitRect.left = 2+32*5+600;
	HitRect.top = 2+12+20+2;
	HitRect.right = HitRect.left+22*5;
	HitRect.bottom = HitRect.top+20;
	if (PtInRect(&HitRect,CursorPos))
		{
		CursorPos.x -= HitRect.left;
		CursorPos.x /= 22;
		SendMessage(hWnd,WM_COMMAND,(WPARAM)IDM_MODIFYSTRT+CursorPos.x,(LPARAM)0);
		return;
		}

	return;
}


// «»»» Changement d'élément (menu) «««««««««««««««««««««««««««««««««««««»

void Toolbar_ChangeWorldElem(DWORD ElemID)
{
	WorldItemID = World[ElemID].Id;
	Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,ElemID+IDM_MAPWORLDSTRT,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	return;
}


// «»»» Changement de portail de héro (menu contextuel) «««««««««««««««««»

void Toolbar_ChangeGate(DWORD GateID)
{
	ToolsGateID = GateID;
	WorldItemID = Gates[GateID].Id;
	Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,IDM_MAPWORLDHEROPORTALSSTRT+GateID,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	return;
}


// «»»» Changement de joueur (menu contextuel) ««««««««««««««««««««««««««»

void Toolbar_ChangePlayer(DWORD PlayerID)
{
	RECT	HitRect;

	HitRect.left = 2+4+32*5+34+10;
	HitRect.top = 2+18+(32-Font.FontHeight)/2-4;
	HitRect.right = HitRect.left+120;
	HitRect.bottom = HitRect.top+Font.FontHeight+9;
	if (WorldItemID == Walls[ToolsPlyrID*2].Id) WorldItemID = Walls[PlayerID*2].Id;
	if (WorldItemID == Walls[ToolsPlyrID*2+1].Id) WorldItemID = Walls[PlayerID*2+1].Id;
	ToolsPlyrID = PlayerID;
	Misc_CheckMenuItem(hMenu,IDM_PLAYERSSTRT,IDM_PLAYERSSTOP,PlayerID+IDM_PLAYERSSTRT,TRUE);
	InvalidateRect(hToolbar,&HitRect,FALSE);
	HitRect.left = 2+32*5+158+10;
	HitRect.top = 4;
	HitRect.right = HitRect.left+32;
	HitRect.bottom = HitRect.top+64;
	InvalidateRect(hToolbar,&HitRect,FALSE);
	UpdateWindow(hToolbar);
	return;
}


// «»»» Changement de salle (menu) ««««««««««««««««««««««««««««««««««««««»

//--- Eléments du monde appartenant à un joueur ---

void Toolbar_ChangeWall(DWORD WallID)
{
	if (WallID == IDM_MAPWORLDWALL)
		WorldItemID = Walls[ToolsPlyrID*2].Id;
	else
		WorldItemID = Walls[ToolsPlyrID*2+1].Id;

	Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,WallID,TRUE);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	return;
}

//--- Salles conventionnelles ---

void Toolbar_ChangeRoom(DWORD RoomID)
{
	WorldItemID = Rooms[RoomID].Id;
	Misc_CheckMenuItem(hMenu,IDM_WORLDELEM,IDM_WORLDELEM,IDM_WORLDELEM,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,IDM_MAPWORLDHEROPORTALS,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMS,IDM_ROOMS,IDM_ROOMS,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDSTRT,IDM_MAPWORLDSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDHEROPORTALSSTRT,IDM_MAPWORLDHEROPORTALSSTOP,0,FALSE);
	Misc_CheckMenuItem(hMenu,IDM_ROOMSSTRT,IDM_ROOMSSTOP,IDM_ROOMSSTRT+RoomID,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_MAPWORLDWALL,IDM_MAPWORLDCLAIMED,0,FALSE);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	return;
}


// «»»» Changement de mode de dessin ««««««««««««««««««««««««««««««««««««»

//--- Mode de dessin standard ---

void Toolbar_ChangePaintMode(DWORD ModeID)
{
	if (ToolsItemID == DrawingTools[ModeID].Id) return;

	ToolsItemID = DrawingTools[ModeID].Id;
	Misc_CheckMenuItem(hMenu,IDM_PAINTMODESTRT,IDM_PAINTMODESTOP,ModeID+IDM_PAINTMODESTRT,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_PUTSTRT,IDM_PUTSTOP,0,FALSE);
	Mouse_MovedUpdate(hWnd);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TOOLPART|SBT_OWNERDRAW,(LPARAM)NULL);
	return;
}

//--- Placement des éléments ---

void Toolbar_ChangeMiscMode(DWORD ModeID)
{
	if (ToolsItemID == MiscTools[ModeID].Id)
		{
		SendMessage(hWnd,WM_COMMAND,(WPARAM)IDM_MODIFYSTRT+ModeID,(LPARAM)hWnd);
		return;
		}
	ToolsItemID = MiscTools[ModeID].Id;
	Misc_CheckMenuItem(hMenu,IDM_PAINTMODESTRT,IDM_PAINTMODESTOP,IDM_PUTELEM,TRUE);
	Misc_CheckMenuItem(hMenu,IDM_PUTSTRT,IDM_PUTSTOP,ModeID+IDM_PUTSTRT,TRUE);
	Mouse_MovedUpdate(hWnd);
	InvalidateRect(hToolbar,NULL,FALSE);
	UpdateWindow(hToolbar);
	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TOOLPART|SBT_OWNERDRAW,(LPARAM)NULL);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Changement de taille						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Toolbar_Resize(LONG Width)
{
	if (Width < 1024) Width = 1024;
	SetWindowPos(hToolbar,NULL,0,0,Width,74,SWP_NOMOVE|SWP_NOZORDER);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
