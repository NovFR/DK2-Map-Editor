
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine des gestions des évènements liés à la souris
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

extern MAP		Map;
extern MAPCURSOR	MapCursor;
extern MAPCURSOR	MapCursorX;
extern MAPCURSOR	MapCursorV;
extern MAPCURSOR	MapCursorH;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern MAPEDITSTATS	MapStats;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern LONG		MapCursorMinWidth;
extern LONG		MapCursorMaxWidth;
extern LONG		MapCursorMinHeight;
extern LONG		MapCursorMaxHeight;
extern MAPRECT		MapCursorPen;
extern MAPRECT		MapCursorPenOld;
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorSel;
extern LONG		MapLocked;
extern UINT		MapCursorMode;
extern DWORD		WorldItemID;
extern DWORD		ToolsItemID;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Mouse_Paint(HWND hWnd, HDC hDC)
{
	MAPCURSOR	*Cursor;
	int		 CursorId;

	if (MapLocked) return;
	if (MapCursorMode == MOUSE_RIGHTRECT) return;

	Cursor = &MapCursor;
	if (!MapCursorMode)
		{
		switch(ToolsItemID)
			{
			case PAINTMODE_CREATURE:
			case PAINTMODE_DOOR:
			case PAINTMODE_TRAP:
			case PAINTMODE_OBJECT:
			case PAINTMODE_MAGICALOBJECT:
				CursorId = Things_CanPaint(&Map,MapCursorRect.x1,MapCursorRect.y1);
				switch(CursorId)
					{
					case 0:	Cursor = &MapCursorX;
						break;
					case 2:	Cursor = &MapCursorV;
						break;
					case 3:	Cursor = &MapCursorH;
						break;
					default:Cursor = &MapCursor;
						break;
					}
				break;

			default:if (!Map_CanPaint(hWnd,0xFE))
					Cursor = &MapCursorX;
				break;
			}
		}

	Map_PaintCursor(hWnd,hDC,&MapCursorRect,Cursor,TRUE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Effacage d'un objet se trouvant sous le curseur			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Mouse_RemoveThingUnderCursor(HWND hWnd, LONG Preferred)
{
	MAPCLIENT	MapClient;
	MAPRECT		MapRect;
	RECT		WindowRect;
	POINT		CursorPos;
	LONG		Creatures;
	LONG		Doors;
	LONG		Objects;

	GetCursorPos(&CursorPos);
	GetWindowRect(hWnd,&WindowRect);
	if (CursorPos.x < WindowRect.left) return;
	if (CursorPos.y < WindowRect.top) return;
	if (CursorPos.x > WindowRect.right) return;
	if (CursorPos.y > WindowRect.bottom) return;

	MapWindowPoints(NULL,hWnd,&CursorPos,1);
	Map_CursorToMap(hWnd,&CursorPos);

	Creatures = (LONG)Things_GetAtMapPos(&MapCreatures,CursorPos.x,CursorPos.y);
	Doors = (LONG)Things_GetAtMapPos(&MapDoors,CursorPos.x,CursorPos.y);
	if (!Doors) Doors = (LONG)Things_GetAtMapPos(&MapTraps,CursorPos.x,CursorPos.y);
	Objects = (LONG)Things_GetAtMapPos(&MapObjects,CursorPos.x,CursorPos.y);
	if (!Objects) Objects = (LONG)Things_GetAtMapPos(&MapMagicalObjects,CursorPos.x,CursorPos.y);

	if ((Creatures)&&(!Doors)&&(!Objects))
		{
		Things_RemoveAtMapPosSafe(&MapCreatures,CursorPos.x,CursorPos.y);
		goto Update;
		}
	if ((!Creatures)&&(Doors)&&(!Objects))
		{
		Things_RemoveAtMapPos(&MapDoors,CursorPos.x,CursorPos.y);
		Things_RemoveAtMapPos(&MapTraps,CursorPos.x,CursorPos.y);
		goto Update;
		}
	if ((!Creatures)&&(!Doors)&&(Objects))
		{
		Things_RemoveAtMapPos(&MapObjects,CursorPos.x,CursorPos.y);
		Things_RemoveAtMapPos(&MapMagicalObjects,CursorPos.x,CursorPos.y);
		goto Update;
		}

	switch(Preferred)
		{
		case THING_CREATURE:
			Things_RemoveAtMapPosSafe(&MapCreatures,CursorPos.x,CursorPos.y);
			goto Update;
		case THING_DOOR:
			Things_RemoveAtMapPos(&MapDoors,CursorPos.x,CursorPos.y);
			Things_RemoveAtMapPos(&MapTraps,CursorPos.x,CursorPos.y);
			goto Update;
		case THING_OBJECT:
			Things_RemoveAtMapPos(&MapObjects,CursorPos.x,CursorPos.y);
			Things_RemoveAtMapPos(&MapMagicalObjects,CursorPos.x,CursorPos.y);
			goto Update;
		}

	return;

Update:	MapRect.x1 = CursorPos.x;
	MapRect.y1 = CursorPos.y;
	MapRect.x2 = CursorPos.x;
	MapRect.y2 = CursorPos.y;
	if (Map_RectangleToClient(hWnd,&MapRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	Files_SaveUpdate();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines relatives au bouton gauche de la souris			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Le bouton a été appuyé ««««««««««««««««««««««««««««««««««««««««««»

void Mouse_LeftButtonDown(HWND hWnd, LONG X, LONG Y)
{
	RECT		MapRect;
	POINT		CursorPos;

	if (MapCursorMode)
		{
		ReleaseCapture();
		MapCursorMode = 0;
		Select_Update();
		Mouse_Moved(hWnd,X,Y);
		return;
		}

	if (MapLocked) return;
	Map_GetMapArea(hWnd,&MapRect);
	if (X < MapRect.left) goto OutOfRange;
	if (Y < MapRect.top) goto OutOfRange;
	if (X > MapRect.right) goto OutOfRange;
	if (Y > MapRect.bottom) goto OutOfRange;
	if (!Map_GetMapPtrFromClientPt(hWnd,X,Y)) goto OutOfRange;
	if (!Map_CanPaint(hWnd,0xFE)) goto CantPaint;
	MapStats.LMBClicks++;

	switch(ToolsItemID)
		{
		case PAINTMODE_CREATURE:
		case PAINTMODE_DOOR:
		case PAINTMODE_TRAP:
		case PAINTMODE_OBJECT:
		case PAINTMODE_MAGICALOBJECT:
			CursorPos.x = X;
			CursorPos.y = Y;
			Map_CursorToMap(hWnd,&CursorPos);
			if (!Things_CanPaint(&Map,CursorPos.x,CursorPos.y)) goto CantPaint;
			Sound_Play(SOUND_PAINT,ToolsItemID);
			Things_Paint(hWnd,ToolsItemID,CursorPos.x,CursorPos.y);
			return;
		}

	if ((MapCursorMinWidth == MapCursorMaxWidth)&&(MapCursorMinHeight == MapCursorMaxHeight))
		goto PaintImmRect;

	switch(ToolsItemID)
		{
		case PAINTMODE_RECT:
		case PAINTMODE_RECTFILL:
		case PAINTMODE_CIRCLE:
		case PAINTMODE_CIRCLEFILL:
		case PAINTMODE_LINE:
			Sound_Play(SOUND_DEFINERECT,ToolsItemID);
			MapCursorMode = MOUSE_DEFINERECT;
			goto Paint;
		case PAINTMODE_PEN:
			if ((MapCursorMinWidth != 1)||(MapCursorMinHeight != 1))
				goto PaintImmRect;
			MapCursorMode = MOUSE_PAINT;
			MapCursorPen.x1 = X;
			MapCursorPen.y1 = Y;
			MapCursorPenOld.x1 = 0;
			MapCursorPenOld.y1 = 0;
			Sound_Play(SOUND_PAINT,PAINTMODE_PEN);
			Undo_CreateFullMap(szMainMenuPen,&Map);
			goto Paint;
		case PAINTMODE_FILL:
			if ((MapCursorMinWidth != 1)||(MapCursorMinHeight != 1))
				goto PaintImmRect;
			Sound_Play(SOUND_PAINT,PAINTMODE_FILL);
			Undo_CreateFullMap(szMainMenuFill,&Map);
			Map_FillArea(hWnd,X,Y);
			Status_DisplayMapElement(MapCursorRect.x1,MapCursorRect.y1);
			Minimap_Redraw(hWnd);
			Info_Update();
			Files_SaveUpdate();
			break;
		}

	return;

//--- Démarrage du dessin ---

Paint:	SetCapture(hWnd);
	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hWnd,&CursorPos,1);
	Mouse_Moved(hWnd,CursorPos.x,CursorPos.y);
	return;

//--- Force le dessin immédiat ---

PaintImmRect:
	Sound_Play(SOUND_PAINT,PAINTMODE_PEN);
	Undo_CreateMapRect(szMainMenuRectFill,&MapCursorRect,&Map);
	Map_PaintElement(hWnd,PAINTMODE_RECTFILL);
	Status_DisplayMapElement(MapCursorRect.x1,MapCursorRect.y1);
	Minimap_Redraw(hWnd);
	Info_Update();
	Files_SaveUpdate();
	return;

//--- Impossible de peindre ---

CantPaint:
	MessageBeep(MB_ICONEXCLAMATION);
	return;
OutOfRange:
	MessageBeep(MB_OK);
	return;
}


// «»»» Le bouton a été relaché «««««««««««««««««««««««««««««««««««««««««»

void Mouse_LeftButtonUp(HWND hWnd, LONG X, LONG Y, BOOL Shift)
{
	DWORD	FillID;

	if (!MapCursorMode) return;

	FillID = ToolsItemID;
	if (Shift)
		{
		switch(ToolsItemID)
			{
			case PAINTMODE_RECT:
				FillID = PAINTMODE_RECTRND;
				break;
			case PAINTMODE_RECTFILL:
				FillID = PAINTMODE_RECTFILLRND;
				break;
			case PAINTMODE_CIRCLE:
				FillID = PAINTMODE_CIRCLERND;
				break;
			case PAINTMODE_CIRCLEFILL:
				FillID = PAINTMODE_CIRCLEFILLRND;
				break;
			case PAINTMODE_LINE:
				FillID = PAINTMODE_LINERND;
				break;
			}
		}

	switch(FillID)
		{
		case PAINTMODE_RECT:
		case PAINTMODE_RECTRND:
		case PAINTMODE_RECTFILL:
		case PAINTMODE_RECTFILLRND:
		case PAINTMODE_CIRCLE:
		case PAINTMODE_CIRCLERND:
		case PAINTMODE_CIRCLEFILL:
		case PAINTMODE_CIRCLEFILLRND:
		case PAINTMODE_LINE:
		case PAINTMODE_LINERND:
			Sound_Play(SOUND_PAINT,ToolsItemID);
			Undo_CreateMapRect(Undo_PaintModeToText(FillID),&MapCursorRect,&Map);
			if ((MapCursorMinWidth != 1)||(MapCursorMinHeight != 1))
				Map_PaintElement(hWnd,PAINTMODE_RECTFILL);
			else
				Map_PaintElement(hWnd,FillID);
		case PAINTMODE_PEN:
			goto Paint_Done;
		}

	return;

//--- Dessin terminé ---

Paint_Done:
	ReleaseCapture();
	MapCursorMode = 0;
	Status_DisplayMapElement(MapCursorRect.x1,MapCursorRect.y1);
	Minimap_Redraw(hWnd);
	Mouse_Moved(hWnd,X,Y);
	Info_Update();
	Files_SaveUpdate();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Le bouton droit de la souris a été sélectionné			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Le bouton a été appuyé ««««««««««««««««««««««««««««««««««««««««««»

void Mouse_RightButtonDown(HWND hWnd, LONG X, LONG Y)
{
	RECT	MapRect;

	if (MapCursorMode)
		{
		ReleaseCapture();
		MapCursorMode = 0;
		Mouse_Moved(hWnd,X,Y);
		return;
		}

	if (MapLocked) return;
	Map_GetMapArea(hWnd,&MapRect);
	if (X < MapRect.left) return;
	if (Y < MapRect.top) return;
	if (X > MapRect.right) return;
	if (Y > MapRect.bottom) return;
	if (!Map_GetMapPtrFromClientPt(hWnd,X,Y)) return;

	Mouse_HideSelRect(hWnd);
	Mouse_HideCursor(hWnd);
	Select_Update();
	MapCursorSel.x1 = X;
	MapCursorSel.y1 = Y;
	MapCursorSel.x2 = X+MapZoomCX;
	MapCursorSel.y2 = Y+MapZoomCY;
	MapCursorMode = MOUSE_RIGHTRECT;
	Map_CursorToMap(hWnd,(POINT *)&MapCursorSel.x1);
	Map_CursorToMap(hWnd,(POINT *)&MapCursorSel.x2);
	SetCapture(hWnd);
	Mouse_Moved(hWnd,X,Y);
	MapStats.RMBClicks++;
	return;
}


// «»»» Le bouton a été relaché «««««««««««««««««««««««««««««««««««««««««»

void Mouse_RightButtonUp(HWND hWnd, LONG X, LONG Y, BOOL Shift, BOOL Ctrl)
{
	if (!MapCursorMode) return;

	ReleaseCapture();
	MapCursorMode = 0;
	Select_Update();
	Mouse_Moved(hWnd,X,Y);
	RectEx_DrawPath(hWnd,Shift,Ctrl);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Déplacement de la souris						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Simule un déplacement de souris «««««««««««««««««««««««««««««««««»

void Mouse_MovedUpdate(HWND hWnd)
{
	POINT	CursorPos;

	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hWnd,&CursorPos,1);
	Mouse_Moved(hWnd,CursorPos.x,CursorPos.y);
	return;
}


// «»»» La souris s'est déplacée ««««««««««««««««««««««««««««««««««««««««»

void Mouse_Moved(HWND hWnd, LONG X, LONG Y)
{
	if (MapLocked) return;

	switch(MapCursorMode)
		{
		case MOUSE_FREE:
			Mouse_UpdateCursor(hWnd,X,Y,TRUE);
			break;
		case MOUSE_DEFINERECT:
			Mouse_UpdateRect(hWnd,X,Y);
			break;
		case MOUSE_PAINT:
			MapCursorPenOld.x2 = X;
			MapCursorPenOld.y2 = Y;
			Map_CursorToMap(hWnd,(POINT *)&MapCursorPenOld.x2);
			if ((MapCursorPenOld.x1 == MapCursorPenOld.x2)&&(MapCursorPenOld.y1 == MapCursorPenOld.y2)) break;
			MapCursorPen.x2 = X;
			MapCursorPen.y2 = Y;
			Map_CursorToMap(hWnd,(POINT *)&MapCursorPen.x1);
			Map_CursorToMap(hWnd,(POINT *)&MapCursorPen.x2);
			MapCursorRect.x1 = MapCursorPen.x1;
			MapCursorRect.y1 = MapCursorPen.y1;
			MapCursorRect.x2 = MapCursorPen.x2;
			MapCursorRect.y2 = MapCursorPen.y2;
			MapCursorPen.x1 = X;
			MapCursorPen.y1 = Y;
			MapCursorPenOld.x1 = MapCursorPen.x2;
			MapCursorPenOld.y1 = MapCursorPen.y2;
			Map_PaintElement(hWnd,PAINTMODE_LINE);
			Mouse_UpdateCursor(hWnd,X,Y,FALSE);
			break;
		case MOUSE_RIGHTRECT:
			Mouse_UpdateSelRect(hWnd,X,Y);
			break;
		}
	return;
}


// «»»» Mise à jour de la forme du curseur lors du dessin d'une forme «««»

void Mouse_UpdateRect(HWND hWnd, LONG X, LONG Y)
{
	MAPRECT		NewRect;
	MAPCLIENT	MapClient;
	POINT		CursorPos;

	CursorPos.x = X;
	CursorPos.y = Y;
	Map_CursorToMap(hWnd,&CursorPos);
	NewRect.x1 = MapCursorRect.x1;
	NewRect.x2 = CursorPos.x;
	NewRect.y1 = MapCursorRect.y1;
	NewRect.y2 = CursorPos.y;

	if (NewRect.x2 >= Map.Properties.Width) NewRect.x2 = Map.Properties.Width-1;
	if (NewRect.y2 >= Map.Properties.Height) NewRect.y2 = Map.Properties.Height-1;

	//--- Bloque les dimensions si nécessaire ---

	Map_CorrectRect(&NewRect,&MapClient);
	if ((MapClient.Height < MapCursorMinHeight)&&(MapCursorMinHeight))
		NewRect.y2 = NewRect.y1+MapCursorMinHeight-1;
	if ((MapClient.Width < MapCursorMinWidth)&&(MapCursorMinWidth))
		NewRect.x2 = NewRect.x1+MapCursorMinWidth-1;
	if ((MapClient.Height > MapCursorMaxHeight)&&(MapCursorMaxHeight))
		{
		if (NewRect.y1 > NewRect.y2)
			NewRect.y2 = NewRect.y1-MapCursorMaxHeight+1;
		else
			NewRect.y2 = NewRect.y1+MapCursorMaxHeight-1;
		}
	if ((MapClient.Width > MapCursorMaxWidth)&&(MapCursorMaxWidth))
		{
		if (NewRect.x1 > NewRect.x2)
			NewRect.x2 = NewRect.x1-MapCursorMaxWidth+1;
		else
			NewRect.x2 = NewRect.x1+MapCursorMaxWidth-1;
		}

	//--- Test si une mise à jour est réellement nécessaire ---

	if ((NewRect.x1 == MapCursorRect.x1)&&(NewRect.x2 == MapCursorRect.x2)&&(NewRect.y1 == MapCursorRect.y1)&&(NewRect.y2 == MapCursorRect.y2))
		return;

	Status_DisplayMouseCoords(&NewRect,PAINTMODE_RECT);
	Mouse_HideCursor(hWnd);

	//--- Affiche le curseur à la nouvelle position ---

	if (Map_RectangleToClient(hWnd,&NewRect,&MapClient))
		{
		HDC	hDC;

		MapCursorRect.x2 = NewRect.x2;
		MapCursorRect.y2 = NewRect.y2;
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		MapCursorRect.hidden = FALSE;

		hDC = GetDC(hWnd);
		if (hDC)
			{
			Mouse_Paint(hWnd,hDC);
			ReleaseDC(hWnd,hDC);
			}
		else
			{
			InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
			UpdateWindow(hWnd);
			}
		}

	return;
}


// «»»» Mise à jour du curseur ««««««««««««««««««««««««««««««««««««««««««»

//--- Cache le curseur ---

void Mouse_HideCursor(HWND hWnd)
{
	MAPCLIENT	MapClient;

	if (MapCursorRect.hidden) return;

	MapCursorRect.hidden = TRUE;
	if (Map_RectangleToClient(hWnd,&MapCursorRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}

//--- Actualise les dimensions du curseur ---

void Mouse_UpdateCursor(HWND hWnd, LONG X, LONG Y, BOOL UpdateInfo)
{
	MAPRECT		NewRect;
	MAPCLIENT	MapClient;
	POINT		CursorPos;
	LONG		NewMinWidth,NewMinHeight;
	LONG		NewMaxWidth,NewMaxHeight;
	BYTE		ItemID;
	BYTE		ItemOPT;
	BYTE		MapID;

	CursorPos.x = X;
	CursorPos.y = Y;
	Map_CursorToMap(hWnd,&CursorPos);

	//--- Obtention des dimensions du curseur ---

	ItemID = (BYTE)(WorldItemID>>24);
	ItemOPT = (BYTE)(WorldItemID>>8);
	if ((MapID = Map_TranslateID(ItemID,ItemOPT,World)) != (BYTE)-1)
		{
		NewMinWidth = World[MapID].MinWidth;
		NewMinHeight = World[MapID].MinHeight;
		NewMaxWidth = World[MapID].MaxWidth;
		NewMaxHeight = World[MapID].MaxHeight;
		}
	else if ((MapID = Map_TranslateID(ItemID,ItemOPT,Rooms)) != (BYTE)-1)
		{
		NewMinWidth = Rooms[MapID].MinWidth;
		NewMinHeight = Rooms[MapID].MinHeight;
		NewMaxWidth = Rooms[MapID].MaxWidth;
		NewMaxHeight = Rooms[MapID].MaxHeight;
		}
	else if ((MapID = Map_TranslateID(ItemID,ItemOPT,Gates)) != (BYTE)-1)
		{
		NewMinWidth = Gates[MapID].MinWidth;
		NewMinHeight = Gates[MapID].MinHeight;
		NewMaxWidth = Gates[MapID].MaxWidth;
		NewMaxHeight = Gates[MapID].MaxHeight;
		}
	else
		{
		NewMinWidth = 1;
		NewMinHeight = 1;
		NewMaxWidth = 0;
		NewMaxHeight = 0;
		}

	switch(ToolsItemID)
		{
		case PAINTMODE_CREATURE:
		case PAINTMODE_DOOR:
		case PAINTMODE_TRAP:
		case PAINTMODE_OBJECT:
		case PAINTMODE_MAGICALOBJECT:
			NewMinWidth = 1;
			NewMinHeight = 1;
			NewMaxWidth = 1;
			NewMaxHeight = 1;
			break;
		}

	//--- Modifie les positions du curseur ---

	NewRect.x1 = CursorPos.x;
	NewRect.x2 = CursorPos.x+NewMinWidth-1;
	NewRect.y1 = CursorPos.y;
	NewRect.y2 = CursorPos.y+NewMinHeight-1;

	if (NewRect.x2 >= Map.Properties.Width)
		{
		NewRect.x1 = Map.Properties.Width-NewMinWidth;
		NewRect.x2 = Map.Properties.Width-1;
		}
	if (NewRect.y2 >= Map.Properties.Height)
		{
		NewRect.y1 = Map.Properties.Height-NewMinHeight;
		NewRect.y2 = Map.Properties.Height-1;
		}

	//--- Test si un changement est nécessaire ---

	if ((MapCursorRect.x1 == NewRect.x1)&&(MapCursorRect.x2 == NewRect.x2)&&(MapCursorRect.y1 == NewRect.y1)&&(MapCursorRect.y2 == NewRect.y2)&&(MapCursorMinWidth == NewMinWidth)&&(MapCursorMinHeight == NewMinHeight)&&(MapCursorMaxWidth == NewMaxWidth)&&(MapCursorMaxHeight == NewMaxHeight))
		return;

	if (UpdateInfo) Status_DisplayMapElement(NewRect.x1,NewRect.y1);
	Status_DisplayMouseCoords(&NewRect,0);
	Mouse_HideCursor(hWnd);

	//--- Affiche le curseur à la nouvelle position ---

	if (Map_RectangleToClient(hWnd,&NewRect,&MapClient))
		{
		HDC	hDC;

		MapCursorRect.x1 = NewRect.x1;
		MapCursorRect.x2 = NewRect.x2;
		MapCursorRect.y1 = NewRect.y1;
		MapCursorRect.y2 = NewRect.y2;
		MapCursorMinWidth = NewMinWidth;
		MapCursorMinHeight = NewMinHeight;
		MapCursorMaxWidth = NewMaxWidth;
		MapCursorMaxHeight = NewMaxHeight;
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		MapCursorRect.hidden = FALSE;

		hDC = GetDC(hWnd);
		if (hDC)
			{
			Mouse_Paint(hWnd,hDC);
			ReleaseDC(hWnd,hDC);
			}
		else
			{
			InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
			UpdateWindow(hWnd);
			}
		}
	return;
}


// «»»» Mise à jour du rectangle de sélection «««««««««««««««««««««««««««»

//--- Cache le rectangle ---

void Mouse_HideSelRect(HWND hWnd)
{
	MAPCLIENT	MapClient;

	if (MapCursorSel.hidden) return;

	MapCursorSel.hidden = TRUE;
	if (Map_RectangleToClient(hWnd,&MapCursorSel,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}

//--- Actualise les dimensions du rectangle ---

void Mouse_UpdateSelRect(HWND hWnd, LONG X, LONG Y)
{
	POINT		CursorPos;
	MAPCLIENT	MapClient;
	MAPRECT		NewRect;

	CursorPos.x = X;
	CursorPos.y = Y;
	Map_CursorToMap(hWnd,&CursorPos);
	NewRect.x1 = MapCursorSel.x1;
	NewRect.x2 = CursorPos.x;
	NewRect.y1 = MapCursorSel.y1;
	NewRect.y2 = CursorPos.y;
	if (NewRect.x2 >= Map.Properties.Width) NewRect.x2 = Map.Properties.Width-1;
	if (NewRect.y2 >= Map.Properties.Height) NewRect.y2 = Map.Properties.Height-1;

	//--- Test si une mise à jour est réellement nécessaire ---

	if ((NewRect.x1 == MapCursorSel.x1)&&(NewRect.x2 == MapCursorSel.x2)&&(NewRect.y1 == MapCursorSel.y1)&&(NewRect.y2 == MapCursorSel.y2))
		return;

	Status_DisplayMouseCoords(&NewRect,PAINTMODE_RECT);
	Mouse_HideSelRect(hWnd);

	//--- Affiche le curseur à la nouvelle position ---

	if (Map_RectangleToClient(hWnd,&NewRect,&MapClient))
		{
		HDC	hDC;

		MapCursorSel.x2 = NewRect.x2;
		MapCursorSel.y2 = NewRect.y2;
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		MapCursorSel.hidden = FALSE;

		hDC = GetDC(hWnd);
		if (hDC)
			{
			Map_DrawRectangle(hWnd,hDC,&MapCursorSel);
			ReleaseDC(hWnd,hDC);
			}
		else
			{
			InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
			UpdateWindow(hWnd);
			}
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
