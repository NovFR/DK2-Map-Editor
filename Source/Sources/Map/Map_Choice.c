
//<<>-<>>---------------------------------------------------------------------()
/*
	Sélection d'un emplacement sur la carte
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
extern FONTINFO		Font;
extern MAP		Map;
extern NODE		MapEffects;
extern NODE		MapGates;
extern NODE		MapAreas;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern HIMAGELIST	MapRoomsHeros;
extern HIMAGELIST	MapRoomsNeutral;
extern HIMAGELIST	MapRoomsKeeper1;
extern HIMAGELIST	MapRoomsKeeper2;
extern HIMAGELIST	MapRoomsKeeper3;
extern HIMAGELIST	MapRoomsKeeper4;
extern HIMAGELIST	MapRoomsKeeper5;
extern HIMAGELIST	MapRoomsWalls;
extern HIMAGELIST	MapWorldGates;
extern HIMAGELIST	MapWorld;
extern MAPCURSOR	MapCursor;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Map_ChoosePointEx(HWND hWnd, MAPRECT *MapRect, UINT Mode, char *Info)
{
	MAPCHOOSEPOINTCTX	*Context;
	int			 Result;

	Context = GlobalAlloc(GPTR,sizeof(MAPCHOOSEPOINTCTX));
	if (!Context)
		{
		MessageBeep(MB_ICONHAND);
		return;
		}

	CopyMemory(&Context->MapRect,MapRect,sizeof(MAPRECT));
	Context->Mode = Mode;
	Context->Title = Info;
	switch(Mode)
		{
		case MAP_CHOICELMBPOINT:
			Context->MapRect.x2 = Context->MapRect.x1;
			Context->MapRect.y2 = Context->MapRect.y1;
			break;
		}
	CopyMemory(&Context->OrgRect,&Context->MapRect,sizeof(MAPRECT));
	Context->OrgRect.hidden = FALSE;
	Context->OrgRect.color = 0x00FFFFFF;
	Context->OrgRect.textcolor = 0x00FFFFFFFF;
	Context->OrgRect.name = NULL;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(5000),hWnd,Map_ChoosePointProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	if (Result)
		{
		MapRect->x1 = Context->MapRect.x1;
		MapRect->x2 = Context->MapRect.x2;
		MapRect->y1 = Context->MapRect.y1;
		MapRect->y2 = Context->MapRect.y2;
		}

	GlobalFree(Context);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Map_ChoosePointProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	HDC			 hDC;
	MAPCHOOSEPOINTCTX	*Context;
	PAINTSTRUCT		 paintStruct;

	if (uMsgId == WM_INITDIALOG)
		{
		Map_ChoosePointInit(hDlg,(MAPCHOOSEPOINTCTX *)lParam);
		return(FALSE);
		}

	Context = (MAPCHOOSEPOINTCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_TIMER:
			if (Context->DefiningRect) Map_ChoosePointAutoScroll(hDlg,Context);
			return(TRUE);

		case WM_LBUTTONDOWN:
			Map_ChoosePointBeginRect(hDlg,Context);
			return(TRUE);

		case WM_LBUTTONUP:
			if (Context->DefiningRect)
				{
				ReleaseCapture();
				Context->DefiningRect = FALSE;
				Map_ChoosePointEndRect(hDlg,TRUE,Context);
				}
			return(TRUE);

		case WM_RBUTTONDOWN:
			if (Context->DefiningRect)
				{
				ReleaseCapture();
				Context->DefiningRect = FALSE;
				Map_ChoosePointEndRect(hDlg,FALSE,Context);
				}
			return(TRUE);

		case WM_ACTIVATE:
			if ((LOWORD(wParam) == WA_INACTIVE)&&(Context->DefiningRect))
				{
				ReleaseCapture();
				Context->DefiningRect = FALSE;
				Map_ChoosePointEndRect(hDlg,TRUE,Context);
				}
			return(TRUE);

		case WM_MOUSEMOVE:
			Map_ChoosePointPaintUpdate(hDlg,Context);
			return(TRUE);

		case WM_PAINT:
			if (!GetUpdateRect(hDlg,NULL,FALSE)) break;
			hDC = BeginPaint(hDlg,&paintStruct);
			Map_ChoosePointDraw(hDlg,hDC,Context);
			Map_ChoosePointDrawRect(hDlg,hDC,Context);
			Map_ChoosePointDrawList(hDlg,hDC,Context,&MapEffects);
			Map_ChoosePointDrawList(hDlg,hDC,Context,&MapGates);
			Map_ChoosePointDrawList(hDlg,hDC,Context,&MapAreas);
			Map_ChoosePointDrawRectangle(hDlg,hDC,&Context->OrgRect,Context->X,Context->Y);
			EndPaint(hDlg,&paintStruct);
			return(TRUE);

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->hIcon);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Map_ChoosePointPaintInfo((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			break;

		case WM_HSCROLL:
			Map_ChoosePointScrollHorz(hDlg,LOWORD(wParam),HIWORD(wParam),Context);
			break;

		case WM_VSCROLL:
			Map_ChoosePointScrollVert(hDlg,LOWORD(wParam),HIWORD(wParam),Context);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					if ((HWND)lParam == Context->hOk)
						{
						DestroyIcon(Context->hIcon);
						KillTimer(hDlg,1);
						EndDialog(hDlg,1);
						break;
						}
					if ((HWND)lParam == Context->hCancel)
						{
						DestroyIcon(Context->hIcon);
						KillTimer(hDlg,1);
						EndDialog(hDlg,0);
						break;
						}
					switch(LOWORD(wParam))
						{
						case IDCANCEL:
							DestroyIcon(Context->hIcon);
							KillTimer(hDlg,1);
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointInit(HWND hDlg, MAPCHOOSEPOINTCTX *Context)
{
	RECT	Rect;
	LONG	X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowText(hDlg,Context->Title);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	if (!SetTimer(hDlg,1,100,NULL))
		{
		EndDialog(hDlg,-1);
		return;
		}

	Context->hIcon = LoadImage(hInstance,MAKEINTRESOURCE(19),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->hIcon)
		{
		KillTimer(hDlg,1);
		EndDialog(hDlg,-1);
		return;
		}

	X = GetSystemMetrics(SM_CXVSCROLL);
	Y = GetSystemMetrics(SM_CYHSCROLL);
	GetClientRect(hDlg,&Rect);
	Rect.top += 10+Font.FontHeight+10;
	Rect.left = Rect.right-10-X;
	Rect.right = Rect.left+X;
	Rect.bottom -= 10+Y;
	Context->hVScroll = CreateWindow(szScrollBarClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_VERT,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);

	GetClientRect(hDlg,&Rect);
	Rect.top = Rect.bottom-10-Y;
	Rect.left += 10;
	Rect.bottom = Rect.top+Y;
	Rect.right -= 10+X;
	Context->hHScroll = CreateWindow(szScrollBarClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SBS_HORZ,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);

	GetClientRect(hDlg,&Rect);
	Rect.top = Rect.bottom-10-Y;
	Rect.left = Rect.right-10-X;
	Rect.right = Rect.left+X;
	Rect.bottom = Rect.top+Y;
	Context->hButton = CreateWindow(szButtonClass,NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE|WS_CLIPSIBLINGS|BS_OWNERDRAW,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);

	GetClientRect(hDlg,&Rect);
	Rect.top += 10-2;
	Rect.bottom = Rect.top+Font.FontHeight+8;
	Rect.left += 10-2;
	Rect.right -= 10+2+200;
	Context->hInfo = CreateWindow(szStaticClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|SS_OWNERDRAW,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);
	Rect.left = Rect.right+2;
	Rect.right += 100+2;
	Context->hOk = CreateWindow(szButtonClass,szOk,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);
	Rect.left = Rect.right+2;
	Rect.right += 100+2;
	Context->hCancel = CreateWindow(szButtonClass,szCancel,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hDlg,NULL,hInstance,NULL);

	if (!Context->hVScroll || !Context->hHScroll || !Context->hButton || !Context->hInfo || !Context->hOk || !Context->hCancel)
		{
		KillTimer(hDlg,1);
		DestroyIcon(Context->hIcon);
		EndDialog(hDlg,-1);
		return;
		}

	Map_ChoosePointScrollTo(hDlg,Context);
	SendMessage(Context->hOk,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(Context->hCancel,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	Map_ChoosePointSetProp(hDlg,PROP_VERTICAL,Context);
	Map_ChoosePointSetProp(hDlg,PROP_HORIZONTAL,Context);

	return;
}


// «»»» Affichage du contenu de la fenêtre ««««««««««««««««««««««««««««««»

void Map_ChoosePointDraw(HWND hDlg, HDC hDC, MAPCHOOSEPOINTCTX *Context)
{
	RECT	DrawRect;
	LONG	DX,DY;

	GetClientRect(hDlg,&DrawRect);
	DrawRect.left += 10;
	DrawRect.top += 10+Font.FontHeight+10;
	DrawRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	DrawRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	DX = ((DrawRect.right-DrawRect.left)/MapZoomCX)+1;
	DY = ((DrawRect.bottom-DrawRect.top)/MapZoomCY)+1;
	Map_ChoosePointPaintMap(hDC,&DrawRect,DX,DY,Context->X,Context->Y);
	Things_DrawDoors(hDC,&DrawRect,DX,DY,Context->X,Context->Y);
	Things_DrawTraps(hDC,&DrawRect,DX,DY,Context->X,Context->Y);
	Things_DrawObjects(hDC,&DrawRect,DX,DY,Context->X,Context->Y);
	Things_DrawMagicalObjects(hDC,&DrawRect,DX,DY,Context->X,Context->Y);
	Things_DrawCreatures(hDC,&DrawRect,DX,DY,Context->X,Context->Y);

	DrawRect.left -= 2;
	DrawRect.right += 2+GetSystemMetrics(SM_CXVSCROLL);
	DrawRect.top -= 2;
	DrawRect.bottom += 2+GetSystemMetrics(SM_CYHSCROLL);
	DrawEdge(hDC,&DrawRect,BDR_SUNKENINNER|BDR_SUNKENOUTER,BF_RECT);
	return;
}


// «»»» Affichage du contenu de la fenêtre ««««««««««««««««««««««««««««««»

void Map_ChoosePointScrollTo(HWND hDlg, MAPCHOOSEPOINTCTX *Context)
{
	RECT	DrawRect;
	LONG	DX,DY;

	GetClientRect(hDlg,&DrawRect);
	DrawRect.left += 10;
	DrawRect.top += 10+Font.FontHeight+10;
	DrawRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	DrawRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	DX = ((DrawRect.right-DrawRect.left)/MapZoomCX)+1;
	DY = ((DrawRect.bottom-DrawRect.top)/MapZoomCY)+1;

	Context->X = Context->MapRect.x1-(DX/2);
	Context->Y = Context->MapRect.y1-(DY/2);
	if (Context->X < 0) Context->X = 0;
	if (Context->Y < 0) Context->Y = 0;
	if (Context->X > Map.Properties.Width+1-DX) Context->X = Map.Properties.Width+1-DX;
	if (Context->Y > Map.Properties.Height+1-DY) Context->Y = Map.Properties.Height+1-DY;
	if (DX >= Map.Properties.Width+1) Context->X = 0;
	if (DY >= Map.Properties.Height+1) Context->Y = 0;
	return;
}


// «»»» Déplacement automatique «««««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointAutoScroll(HWND hDlg, MAPCHOOSEPOINTCTX *Context)
{
	POINT	CursorPos;
	RECT	MapRect;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hDlg,&CursorPos,1);

	if (CursorPos.x < MapRect.left)
		Map_ChoosePointScrollHorz(hDlg,SB_LINEUP,0,Context);
	if (CursorPos.x > MapRect.right)
		Map_ChoosePointScrollHorz(hDlg,SB_LINEDOWN,0,Context);
	if (CursorPos.y < MapRect.top)
		Map_ChoosePointScrollVert(hDlg,SB_LINEUP,0,Context);
	if (CursorPos.y > MapRect.bottom)
		Map_ChoosePointScrollVert(hDlg,SB_LINEDOWN,0,Context);

	return;
}


// «»»» Déplacement horizontal ««««««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointScrollHorz(HWND hDlg, UINT NotificationCode, LONG Track, MAPCHOOSEPOINTCTX *Context)
{
	RECT	MapRect;
	LONG	SavedPos;
	LONG	DX;

	SavedPos = Context->X;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	DX = ((MapRect.right-MapRect.left)/MapZoomCX)+1;

	if (Map.Properties.Width < DX) return;

	switch(NotificationCode)
		{
		case SB_TOP:
			Context->X = 0;
			goto Scroll_Map;
		case SB_PAGEUP:
			if (!Context->X) break;
			Context->X -= DX;
			goto Scroll_Map;
		case SB_PAGEDOWN:
			Context->X += DX;
			goto Scroll_Map;
		case SB_LINEUP:
			if (!Context->X) break;
			--Context->X;
			goto Scroll_Map;
		case SB_LINEDOWN:
			Context->X++;
			goto Scroll_Map;
		case SB_THUMBTRACK:
			Context->X = Track;
			goto Scroll_Map;
		case SB_BOTTOM:
			Context->X = Map.Properties.Width+1-DX;
			goto Scroll_Map;
		}

	return;

//--- Changement de la position de la carte ---

Scroll_Map:
	if (Context->X < 0) Context->X = 0;
	if (Context->X > Map.Properties.Width+1-DX) Context->X = Map.Properties.Width+1-DX;
	if (Context->X == SavedPos) return;

	InvalidateRect(hDlg,&MapRect,FALSE);
	Map_ChoosePointPaintUpdate(hDlg,Context);
	Map_ChoosePointSetProp(hDlg,PROP_HORIZONTAL,Context);
	return;
}


// «»»» Déplacement vertical ««««««««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointScrollVert(HWND hDlg, UINT NotificationCode, LONG Track, MAPCHOOSEPOINTCTX *Context)
{
	RECT	MapRect;
	LONG	SavedPos;
	LONG	DY;

	SavedPos = Context->Y;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	DY = ((MapRect.bottom-MapRect.top)/MapZoomCY)+1;

	if (Map.Properties.Height < DY) return;

	switch(NotificationCode)
		{
		case SB_TOP:
			Context->Y = 0;
			goto Scroll_Map;
		case SB_PAGEUP:
			if (!Context->Y) break;
			Context->Y -= DY;
			goto Scroll_Map;
		case SB_PAGEDOWN:
			Context->Y += DY;
			goto Scroll_Map;
		case SB_LINEUP:
			if (!Context->Y) break;
			--Context->Y;
			goto Scroll_Map;
		case SB_LINEDOWN:
			Context->Y++;
			goto Scroll_Map;
		case SB_THUMBTRACK:
			Context->Y = Track;
			goto Scroll_Map;
		case SB_BOTTOM:
			Context->Y = Map.Properties.Height+1-DY;
			goto Scroll_Map;
		}

	return;

//--- Changement de la position de la carte ---

Scroll_Map:
	if (Context->Y < 0) Context->Y = 0;
	if (Context->Y > Map.Properties.Height+1-DY) Context->Y = Map.Properties.Height+1-DY;
	if (Context->Y == SavedPos) return;

	InvalidateRect(hDlg,&MapRect,FALSE);
	Map_ChoosePointPaintUpdate(hDlg,Context);
	Map_ChoosePointSetProp(hDlg,PROP_VERTICAL,Context);
	return;
}


// «»»» Sélection du rectangle de sélection «««««««««««««««««««««««««««««»

//--- Début de la définition ---

void Map_ChoosePointBeginRect(HWND hDlg, MAPCHOOSEPOINTCTX *Context)
{
	POINT	CursorPos;
	RECT	MapRect;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);

	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hDlg,&CursorPos,1);
	if (CursorPos.x < MapRect.left) return;
	if (CursorPos.y < MapRect.top) return;
	CursorPos.x -= MapRect.left;
	CursorPos.x /= MapZoomCX;
	CursorPos.x += Context->X;
	CursorPos.y -= MapRect.top;
	CursorPos.y /= MapZoomCY;
	CursorPos.y += Context->Y;
	if (CursorPos.x >= Map.Properties.Width) return;
	if (CursorPos.y >= Map.Properties.Height) return;

	switch(Context->Mode)
		{
		case MAP_CHOICELMBPOINT:
			Context->MapRect.x1 = CursorPos.x;
			Context->MapRect.y1 = CursorPos.y;
			Context->MapRect.x2 = CursorPos.x;
			Context->MapRect.y2 = CursorPos.y;
			InvalidateRect(hDlg,&MapRect,FALSE);
			InvalidateRect(Context->hInfo,NULL,FALSE);
			break;
		case MAP_CHOICELMBRECT:
			CopyMemory(&Context->SavedRect,&Context->MapRect,sizeof(MAPRECT));
			Context->MapRect.x1 = CursorPos.x;
			Context->MapRect.y1 = CursorPos.y;
			Context->MapRect.x2 = CursorPos.x;
			Context->MapRect.y2 = CursorPos.y;
			InvalidateRect(hDlg,&MapRect,FALSE);
			InvalidateRect(Context->hInfo,NULL,FALSE);
			SetCapture(hDlg);
			Context->DefiningRect = TRUE;
			break;
		}

	return;
}

//--- Fin de la définition ---

void Map_ChoosePointEndRect(HWND hDlg, BOOL UpdateRect, MAPCHOOSEPOINTCTX *Context)
{
	RECT	MapRect;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);

	if (!UpdateRect)
		{
		CopyMemory(&Context->MapRect,&Context->SavedRect,sizeof(MAPRECT));
		}
	else
		{
		MAPCLIENT	MapClient;

		Map_CorrectRect(&Context->MapRect,&MapClient);
		Context->MapRect.x1 = MapClient.X;
		Context->MapRect.y1 = MapClient.Y;
		Context->MapRect.x2 = MapClient.X+MapClient.Width-1;
		Context->MapRect.y2 = MapClient.Y+MapClient.Height-1;
		}

	InvalidateRect(hDlg,&MapRect,FALSE);
	InvalidateRect(Context->hInfo,NULL,FALSE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage du rectangle de sélection «««««««««««««««««««««««««««««»

void Map_ChoosePointDrawRect(HWND hDlg, HDC hDC, MAPCHOOSEPOINTCTX *Context)
{
	MAPCLIENT	MapClient;
	RECT		MapRect;
	RECT		RectRect;
	LONG		X,Y;
	LONG		W,H;

	GetClientRect(hDlg,&MapRect);
	MapRect.left   += 10;
	MapRect.top    += 10+Font.FontHeight+10;
	MapRect.right  -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);

	RectRect.left   = Context->MapRect.x1;
	RectRect.top    = Context->MapRect.y1;
	RectRect.right  = Context->MapRect.x2;
	RectRect.bottom = Context->MapRect.y2;
	Map_CorrectRect((MAPRECT *)&RectRect,&MapClient);
	RectRect.left   = MapClient.X;
	RectRect.top    = MapClient.Y;
	RectRect.right  = MapClient.X+MapClient.Width-1;
	RectRect.bottom = MapClient.Y+MapClient.Height-1;

	if (RectRect.right < Context->X)	return;
	if (RectRect.bottom < Context->Y)	return;

	RectRect.left   *= MapZoomCX;
	RectRect.left   += MapRect.left;
	RectRect.left   -= Context->X*MapZoomCX;
	RectRect.top    *= MapZoomCY;
	RectRect.top    += MapRect.top;
	RectRect.top    -= Context->Y*MapZoomCY;
	RectRect.right  *= MapZoomCX;
	RectRect.right  += MapRect.left;
	RectRect.right  -= Context->X*MapZoomCX;
	RectRect.bottom *= MapZoomCY;
	RectRect.bottom += MapRect.top;
	RectRect.bottom -= Context->Y*MapZoomCY;

	if (RectRect.left < MapRect.left)	RectRect.left   = MapRect.left;
	if (RectRect.top < MapRect.top)		RectRect.top    = MapRect.top;
	if (RectRect.right > MapRect.right)	RectRect.right  = MapRect.right;
	if (RectRect.bottom > MapRect.bottom)	RectRect.bottom = MapRect.bottom;

	for (Y = RectRect.top; Y <= RectRect.bottom; Y += H)
		{
		for (X = RectRect.left; X <= RectRect.right; X += W)
			{
			W = MapZoomCX;
			H = MapZoomCY;
			if (X+W > MapRect.right)  W -= (X+MapZoomCX)-MapRect.right;
			if (Y+H > MapRect.bottom) H -= (Y+MapZoomCY)-MapRect.bottom;
			BitBlt(hDC,X,Y,W,H,MapCursor.hdc,0,0,SRCPAINT);
			if (W != MapZoomCX) break;
			}
		if (H != MapZoomCY) break;
		}

	return;
}


// «»»» Mise à jour de la position du curseur «««««««««««««««««««««««««««»

void Map_ChoosePointPaintUpdate(HWND hDlg, MAPCHOOSEPOINTCTX *Context)
{
	POINT	CursorPos;
	RECT	MapRect;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);

	GetCursorPos(&CursorPos);
	MapWindowPoints(NULL,hDlg,&CursorPos,1);
	if (CursorPos.x < MapRect.left) CursorPos.x = MapRect.left;
	if (CursorPos.y < MapRect.top) CursorPos.y = MapRect.top;
	CursorPos.x -= MapRect.left;
	CursorPos.x /= MapZoomCX;
	CursorPos.x += Context->X;
	CursorPos.y -= MapRect.top;
	CursorPos.y /= MapZoomCY;
	CursorPos.y += Context->Y;
	if (CursorPos.x >= Map.Properties.Width)  CursorPos.x = Map.Properties.Width-1;
	if (CursorPos.y >= Map.Properties.Height) CursorPos.y = Map.Properties.Height-1;
	if ((CursorPos.x == Context->CursorX)&&(CursorPos.y == Context->CursorY)) return;

	Context->CursorX = CursorPos.x;
	Context->CursorY = CursorPos.y;
	InvalidateRect(Context->hInfo,NULL,FALSE);

	if (Context->DefiningRect)
		{
		Context->MapRect.x2 = CursorPos.x;
		Context->MapRect.y2 = CursorPos.y;
		InvalidateRect(hDlg,&MapRect,FALSE);
		}

	return;
}


// «»»» Affichage des informations ««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointPaintInfo(DRAWITEMSTRUCT *ItemStruct, MAPCHOOSEPOINTCTX *Context)
{
	MAPCLIENT	MapClient;
	RECT		WorkRect;
	HFONT		hFont;
	COLORREF	OldTextColor;
	COLORREF	OldBkColor;

	DrawEdge(ItemStruct->hDC,&ItemStruct->rcItem,BDR_SUNKENINNER|BDR_SUNKENOUTER,BF_RECT);
	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 6;
	WorkRect.top += 4;
	WorkRect.right -= 6;
	WorkRect.bottom -= 4;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_MENU+1));

	Map_CorrectRect(&Context->MapRect,&MapClient);
	wsprintf(Context->Output,szChoiceCoordinates,Context->CursorX,Context->CursorY,MapClient.X,MapClient.Y,MapClient.Width,MapClient.Height);
	hFont = SelectObject(ItemStruct->hDC,Font.hFont);
	if (hFont)
		{
		OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_MENUTEXT));
		OldBkColor = SetBkColor(ItemStruct->hDC,GetSysColor(COLOR_MENU));
		DrawText(ItemStruct->hDC,Context->Output,-1,&WorkRect,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkColor(ItemStruct->hDC,OldBkColor);
		SetTextColor(ItemStruct->hDC,OldTextColor);
		SelectObject(ItemStruct->hDC,hFont);
		}

	return;
}


// «»»» Traduction des codes binaires en données graphiques «««««««««««««»

void Map_ChoosePointPaintMap(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	register BYTE	*MapPtr;
	register BYTE	*MapSavedPtr;
	HIMAGELIST	 MapImageList;
	RECT		 ClipRect;
	LONG		 MX,MY;
	LONG		 X,Y;
	LONG		 W,H;
	long		 i,j;

	if (!Map.Map) goto Draw_BlackRect;

	if (DX+MapStartX > Map.Properties.Width) DX = Map.Properties.Width;
	if (DY+MapStartY > Map.Properties.Height) DY = Map.Properties.Height;
	X = MapRect->left;
	Y = MapRect->top;
	MY = 0;
	MapPtr = Map.Map+MapStartX*MAP_CONTROLBYTES+(MapStartY*Map.Properties.Width*MAP_CONTROLBYTES);

	for (i = 0; i != DY; i++)
		{
		MapSavedPtr = MapPtr;
		MX = 0;

		for (j = 0; j != DX; j++)
			{
			if (MapPtr >= MapSavedPtr+Map.Properties.Width*MAP_CONTROLBYTES-MapStartX*MAP_CONTROLBYTES) break;

			switch(*(MapPtr+3))
				{
				case 0:	MapImageList = MapWorld;
					break;
				case 1:	switch(*(MapPtr+1))
						{
						case 1:	MapImageList = MapRoomsHeros;
							break;
						case 2:	MapImageList = MapRoomsNeutral;
							break;
						case 3:	MapImageList = MapRoomsKeeper1;
							break;
						case 4:	MapImageList = MapRoomsKeeper2;
							break;
						case 5:	MapImageList = MapRoomsKeeper3;
							break;
						case 6:	MapImageList = MapRoomsKeeper4;
							break;
						case 7:	MapImageList = MapRoomsKeeper5;
							break;
						default:MapImageList = MapRoomsNeutral;
							break;
						}
					break;
				case 2:	MapImageList = MapWorldGates;
					break;
				case 3: MapImageList = MapRoomsWalls;
					break;
				}

			if ((X < MapRect->right)&&(Y < MapRect->bottom))
				{
				W = MapZoomCX;
				H = MapZoomCY;
				if (X+MapZoomCX > MapRect->right)  W -= (X+MapZoomCX)-MapRect->right;
				if (Y+MapZoomCY > MapRect->bottom) H -= (Y+MapZoomCY)-MapRect->bottom;
				ImageList_DrawEx(MapImageList,*MapPtr,hDC,X,Y,W,H,CLR_DEFAULT,CLR_DEFAULT,ILD_NORMAL);
				}

			MX += W;
			X += MapZoomCX;
			MapPtr += MAP_CONTROLBYTES;
			}

		MapPtr = MapSavedPtr+Map.Properties.Width*MAP_CONTROLBYTES;
		if (MapPtr >= Map.Map+Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height) break;
		MY += H;
		Y += MapZoomCY;
		X = MapRect->left;
		}

	if (MX < MapRect->right)
		{
		ClipRect.left = MX+MapRect->left;
		ClipRect.right = MapRect->left+MapRect->right;
		ClipRect.top = MapRect->top;
		ClipRect.bottom = MapRect->bottom;
		FillRect(hDC,&ClipRect,GetStockObject(BLACK_BRUSH));
		}

	if (MY < MapRect->bottom-MapRect->top)
		{
		ClipRect.left = MapRect->left;
		ClipRect.right = MX+MapRect->left;
		ClipRect.top = MY+MapZoomCY+MapRect->top;
		ClipRect.bottom = MapRect->bottom;
		FillRect(hDC,&ClipRect,GetStockObject(BLACK_BRUSH));
		}

	return;

//--- Affichage d'une carte vide ---

Draw_BlackRect:
	FillRect(hDC,MapRect,GetStockObject(BLACK_BRUSH));
	return;
}


// «»»» Obtention des dimensions des barres «««««««««««««««««««««««««««««»

void Map_ChoosePointSetProp(HWND hDlg, UINT Type, MAPCHOOSEPOINTCTX *Context)
{
	SCROLLINFO	ScrollInfo;
	HWND		hScroll;
	RECT		MapRect;

	GetClientRect(hDlg,&MapRect);
	MapRect.left += 10;
	MapRect.top += 10+Font.FontHeight+10;
	MapRect.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapRect.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);

	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin = 0;

	switch(Type)
		{
		case PROP_VERTICAL:
			hScroll = Context->hVScroll;
			ScrollInfo.nMax = Map.Properties.Height-1;
			ScrollInfo.nPage = (MapRect.bottom-MapRect.top)/MapZoomCY;
			ScrollInfo.nPos = Context->Y;
			break;
		case PROP_HORIZONTAL:
			hScroll = Context->hHScroll;
			ScrollInfo.nMax = Map.Properties.Width-1;
			ScrollInfo.nPage = (MapRect.right-MapRect.left)/MapZoomCX;
			ScrollInfo.nPos = Context->X;
			break;
		}

	SetScrollInfo(hScroll,SB_CTL,&ScrollInfo,TRUE);
	return;
}


// «»»» Affichage des zones et des passages «««««««««««««««««««««««««««««»

void Map_ChoosePointDrawList(HWND hDlg, HDC hDC, MAPCHOOSEPOINTCTX *Context, NODE *List)
{
	MAPRECTEX	*Rect;

	for (Rect = (MAPRECTEX *)List->next; Rect != 0; Rect = (MAPRECTEX *)Rect->node.next)
		Map_ChoosePointDrawRectangle(hDlg,hDC,&Rect->rect,Context->X,Context->Y);

	return;
}


// «»»» Affichage des rectangles ««««««««««««««««««««««««««««««««««««««««»

void Map_ChoosePointDrawRectangle(HWND hDlg, HDC hDC, MAPRECT *Rect, LONG MapStartX, LONG MapStartY)
{
	RECT			MapArea;
	HPEN			RectPen,OldPen;
	register POINT		Limits[5];
	register MAPRECT	MapRect;
	long			DX,DY;

	if (Rect->hidden) return;

	GetClientRect(hDlg,&MapArea);
	MapArea.left += 10;
	MapArea.top += 10+Font.FontHeight+10;
	MapArea.right -= 10+GetSystemMetrics(SM_CXVSCROLL);
	MapArea.bottom -= 10+GetSystemMetrics(SM_CYHSCROLL);
	DX = ((MapArea.right-MapArea.left)/MapZoomCX)+1;
	DY = ((MapArea.bottom-MapArea.top)/MapZoomCY)+1;

	/* Invert coordinates in case of X1,Y1 is after X2,Y2 */

	if (Rect->x1 > Rect->x2)
		{
		MapRect.x1 = Rect->x2;
		MapRect.x2 = Rect->x1;
		}
	else
		{
		MapRect.x1 = Rect->x1;
		MapRect.x2 = Rect->x2;
		}

	if (Rect->y1 > Rect->y2)
		{
		MapRect.y1 = Rect->y2;
		MapRect.y2 = Rect->y1;
		}
	else
		{
		MapRect.y1 = Rect->y1;
		MapRect.y2 = Rect->y2;
		}

	/* Correct rectangle positions according to map position */

	MapRect.x1 -= MapStartX;
	MapRect.y1 -= MapStartY;
	MapRect.x2 -= MapStartX;
	MapRect.y2 -= MapStartY;

	/* Do not draw outbounds rectangles */

	if (MapRect.x2 < 0) return;
	if (MapRect.y2 < 0) return;
	if (MapRect.x1 > DX) return;
	if (MapRect.y1 > DY) return;

	/* Correct rectangle positions according to displayable area */

	if (MapRect.x1 < 0)
		{
		MapRect.x1 = 0;
		Limits[0].x = 0;
		}
	else
		Limits[0].x = -1;

	if (MapRect.y1 < 0)
		{
		MapRect.y1 = 0;
		Limits[0].y = 0;
		}
	else
		Limits[0].y = -1;

	if (MapRect.x2 > DX)
		{
		MapRect.x2 = DX;
		Limits[1].x = 0;
		}
	else
		Limits[1].x = -1;

	if (MapRect.y2 > DY)
		{
		MapRect.y2 = DY;
		Limits[1].y = 0;
		}
	else
		Limits[1].y = -1;

	/* Convert Map coordinates into Client coordinates */

	MapRect.x1 *= MapZoomCX;
	MapRect.y1 *= MapZoomCY;
	MapRect.x2 *= MapZoomCX;
	MapRect.y2 *= MapZoomCY;
	MapRect.x1 += MapArea.left;
	MapRect.y1 += MapArea.top;
	MapRect.x2 += MapArea.left+MapZoomCX-1;
	MapRect.y2 += MapArea.top+MapZoomCY-1;
	if (MapRect.x1 > MapArea.right) return;
	if (MapRect.y1 > MapArea.bottom) return;
	if (MapRect.x2 > MapArea.right) MapRect.x2 = MapArea.right;
	if (MapRect.y2 > MapArea.bottom) MapRect.y2 = MapArea.bottom;

	/* Draw rectangle name if possible */

	if (Rect->name)
		{
		COLORREF OldTextColor;
		UINT	 OldTextMode;
		HFONT	 OldTextFont;
		RECT	 TextRect;

		OldTextColor = SetTextColor(hDC,0x00000000);
		OldTextMode = SetBkMode(hDC,TRANSPARENT);
		OldTextFont = SelectObject(hDC,Font.hFont);
		TextRect.left = MapRect.x1+3;
		TextRect.right = MapRect.x2-3;
		TextRect.top = MapRect.y1+3;
		TextRect.bottom = MapRect.y2-3;
		DrawText(hDC,Rect->name,-1,&TextRect,DT_LEFT);
		--TextRect.left;
		TextRect.right++;
		--TextRect.top;
		TextRect.bottom++;
		SetTextColor(hDC,Rect->textcolor);
		DrawText(hDC,Rect->name,-1,&TextRect,DT_LEFT);
		SelectObject(hDC,OldTextFont);
		SetBkMode(hDC,OldTextMode);
		SetTextColor(hDC,OldTextColor);
		}

	/* Draw (partially) the rectangle */
	// Limit value is -1 if coord is OK

	RectPen = CreatePen(PS_SOLID,0,Rect->color);
	if (RectPen)
		{
		OldPen = SelectObject(hDC,RectPen);
		if (OldPen)
			{
			if ((Limits[0].x == -1)&&(Limits[0].y == -1)&&(Limits[1].x == -1)&&(Limits[1].y == -1))
				goto Rectangle_01;
			if ((Limits[0].x ==  0)&&(Limits[0].y ==  0)&&(Limits[1].x == -1)&&(Limits[1].y == -1))
				goto Rectangle_02;
			if ((Limits[0].x ==  0)&&(Limits[0].y == -1)&&(Limits[1].x == -1)&&(Limits[1].y ==  0))
				goto Rectangle_03;
			if ((Limits[0].x == -1)&&(Limits[0].y == -1)&&(Limits[1].x ==  0)&&(Limits[1].y ==  0))
				goto Rectangle_04;
			if ((Limits[0].x == -1)&&(Limits[0].y ==  0)&&(Limits[1].x ==  0)&&(Limits[1].y == -1))
				goto Rectangle_05;
			if ((Limits[0].x == -1)&&(Limits[0].y ==  0)&&(Limits[1].x == -1)&&(Limits[1].y == -1))
				goto Rectangle_06;
			if ((Limits[0].x == -1)&&(Limits[0].y == -1)&&(Limits[1].x ==  0)&&(Limits[1].y == -1))
				goto Rectangle_07;
			if ((Limits[0].x == -1)&&(Limits[0].y == -1)&&(Limits[1].x == -1)&&(Limits[1].y ==  0))
				goto Rectangle_08;
			if ((Limits[0].x ==  0)&&(Limits[0].y == -1)&&(Limits[1].x == -1)&&(Limits[1].y == -1))
				goto Rectangle_09;
			if ((Limits[0].x ==  0)&&(Limits[0].y == -1)&&(Limits[1].x ==  0)&&(Limits[1].y == -1))
				goto Rectangle_10;
			if ((Limits[0].x == -1)&&(Limits[0].y ==  0)&&(Limits[1].x == -1)&&(Limits[1].y ==  0))
				goto Rectangle_11;
			if ((Limits[0].x ==  0)&&(Limits[0].y ==  0)&&(Limits[1].x ==  0)&&(Limits[1].y == -1))
				goto Rectangle_12;
			if ((Limits[0].x == -1)&&(Limits[0].y ==  0)&&(Limits[1].x ==  0)&&(Limits[1].y ==  0))
				goto Rectangle_13;
			if ((Limits[0].x ==  0)&&(Limits[0].y == -1)&&(Limits[1].x ==  0)&&(Limits[1].y ==  0))
				goto Rectangle_14;
			if ((Limits[0].x ==  0)&&(Limits[0].y ==  0)&&(Limits[1].x == -1)&&(Limits[1].y ==  0))
				goto Rectangle_15;
		Draw_Done:
			SelectObject(hDC,OldPen);
			}
		DeleteObject(RectPen);
		}

	return;

//---  1: a,b(-1,-1) / c,d(-1,-1) ---

Rectangle_01:
	Limits[0].x = MapRect.x1;
	Limits[0].y = MapRect.y1;
	Limits[1].x = MapRect.x2;
	Limits[1].y = MapRect.y1;
	Limits[2].x = MapRect.x2;
	Limits[2].y = MapRect.y2;
	Limits[3].x = MapRect.x1;
	Limits[3].y = MapRect.y2;
	Limits[4].x = MapRect.x1;
	Limits[4].y = MapRect.y1;
	Polyline(hDC,Limits,5);
	goto Draw_Done;

//---  2: a,b(0,0) / c,d(-1,-1) ---

Rectangle_02:
	Limits[0].x = MapRect.x2;
	Limits[0].y = MapRect.y1;
	Limits[1].x = MapRect.x2;
	Limits[1].y = MapRect.y2;
	Limits[2].x = MapRect.x1;
	Limits[2].y = MapRect.y2;
	Polyline(hDC,Limits,3);
	goto Draw_Done;

//---  3: a,b(0,-1) / c,d(-1,0) ---

Rectangle_03:
	MoveToEx(hDC,MapRect.x2,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x1,MapRect.y1);
	MoveToEx(hDC,MapRect.x2,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	goto Draw_Done;

//---  4: a,b(-1,-1) / c,d(0,0) ---

Rectangle_04:
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y1);
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x1,MapRect.y2);
	goto Draw_Done;

//---  5: a,b(-1,0) / c,d(0,-1) ---

Rectangle_05:
	MoveToEx(hDC,MapRect.x1,MapRect.y2,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	MoveToEx(hDC,MapRect.x1,MapRect.y2,NULL);
	LineTo(hDC,MapRect.x1,MapRect.y1);
	goto Draw_Done;

//---  6: a,b(-1,0) / c,d(-1,-1) ---

Rectangle_06:
	Limits[0].x = MapRect.x1;
	Limits[0].y = MapRect.y1;
	Limits[1].x = MapRect.x1;
	Limits[1].y = MapRect.y2;
	Limits[2].x = MapRect.x2;
	Limits[2].y = MapRect.y2;
	Limits[3].x = MapRect.x2;
	Limits[3].y = MapRect.y1;
	Polyline(hDC,Limits,4);
	goto Draw_Done;

//---  7: a,b(-1,-1) / c,d(0,-1) ---

Rectangle_07:
	Limits[0].x = MapRect.x2;
	Limits[0].y = MapRect.y1;
	Limits[1].x = MapRect.x1;
	Limits[1].y = MapRect.y1;
	Limits[2].x = MapRect.x1;
	Limits[2].y = MapRect.y2;
	Limits[3].x = MapRect.x2;
	Limits[3].y = MapRect.y2;
	Polyline(hDC,Limits,4);
	goto Draw_Done;

//---  8: a,b(-1,-1) / c,d(-1,0) ---

Rectangle_08:
	Limits[0].x = MapRect.x1;
	Limits[0].y = MapRect.y2;
	Limits[1].x = MapRect.x1;
	Limits[1].y = MapRect.y1;
	Limits[2].x = MapRect.x2;
	Limits[2].y = MapRect.y1;
	Limits[3].x = MapRect.x2;
	Limits[3].y = MapRect.y2;
	Polyline(hDC,Limits,4);
	goto Draw_Done;

//---  9: a,b(0,-1) / c,d(-1,-1) ---

Rectangle_09:
	Limits[0].x = MapRect.x1;
	Limits[0].y = MapRect.y1;
	Limits[1].x = MapRect.x2;
	Limits[1].y = MapRect.y1;
	Limits[2].x = MapRect.x2;
	Limits[2].y = MapRect.y2;
	Limits[3].x = MapRect.x1;
	Limits[3].y = MapRect.y2;
	Polyline(hDC,Limits,4);
	goto Draw_Done;

//--- 10: a,b(0,-1) / c,d(0,-1) ---

Rectangle_10:
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y1);
	MoveToEx(hDC,MapRect.x1,MapRect.y2,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	goto Draw_Done;

//--- 11: a,b(-1,0) / c,d(-1,0) ---

Rectangle_11:
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x1,MapRect.y2);
	MoveToEx(hDC,MapRect.x2,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	goto Draw_Done;

//--- 12: a,b(0,0) / c,d(0,-1) ---

Rectangle_12:
	MoveToEx(hDC,MapRect.x1,MapRect.y2,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	goto Draw_Done;

//--- 13: a,b(-1,0) / c,d(0,0) ---

Rectangle_13:
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x1,MapRect.y2);
	goto Draw_Done;

//--- 14: a,b(0,-1) / c,d(0,0) ---

Rectangle_14:
	MoveToEx(hDC,MapRect.x1,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y1);
	goto Draw_Done;

//--- 15: a,b(0,0) / c,d(-1,0) ---

Rectangle_15:
	MoveToEx(hDC,MapRect.x2,MapRect.y1,NULL);
	LineTo(hDC,MapRect.x2,MapRect.y2);
	goto Draw_Done;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
