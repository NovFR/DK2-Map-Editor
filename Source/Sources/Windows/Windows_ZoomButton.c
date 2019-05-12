
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion du bouton de sélection des zooms.
	Les fonctions d'initialisation sont dans le fichier "Map\Props.c".
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
extern HICON		hMapZoomIcon;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage du bouton de sélection des zooms ««««««««««««««««««««««»

void Zoom_DrawButton(DRAWITEMSTRUCT *DrawStruct)
{
	UINT	State = 0;
	RECT	Rect;

	if (DrawStruct->itemState&ODS_DISABLED) State = DFCS_INACTIVE;
	if (DrawStruct->itemState&ODS_GRAYED) State = DFCS_INACTIVE;
	if (DrawStruct->itemState&ODS_SELECTED) State = DFCS_PUSHED;
	DrawFrameControl(DrawStruct->hDC,&DrawStruct->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|State);

	Rect.left = (DrawStruct->rcItem.right-DrawStruct->rcItem.left-16)/2;
	Rect.top = (DrawStruct->rcItem.bottom-DrawStruct->rcItem.top-16)/2;
	DrawIconEx(DrawStruct->hDC,Rect.left,Rect.top,hMapZoomIcon,16,16,0,NULL,DI_NORMAL);

	if (DrawStruct->itemState&ODS_FOCUS)
		{
		Rect.left = DrawStruct->rcItem.left+2;
		Rect.right = DrawStruct->rcItem.right-3;
		Rect.top = DrawStruct->rcItem.top+2;
		Rect.bottom = DrawStruct->rcItem.bottom-3;
		DrawFocusRect(DrawStruct->hDC,&Rect);
		}

	return;
}


// «»»» Sélection du bouton de zoom «««««««««««««««««««««««««««««««««««««»

void Zoom_Clicked()
{
	POINT	CursorPos;

	GetCursorPos(&CursorPos);
	Misc_DisplayContextMenu(hWnd,2,ZoomMenu,CursorPos.x,CursorPos.y,Zoom_GetUsed()+IDM_ZOOMSTRT);
	return;
}


// «»»» Changement du zoom ««««««««««««««««««««««««««««««««««««««««««««««»

void Zoom_Change(UINT ZoomID)
{
	register LONG	ZX;
	register LONG	ZY;

	switch(ZoomID)
		{
		case 0:	ZX = 10;
			ZY = 10;
			break;
		case 1: ZX = 16;
			ZY = 16;
			break;
		case 2: ZX = 20;
			ZY = 20;
			break;
		case 3: ZX = 32;
			ZY = 32;
			break;
		case 4: ZX = 40;
			ZY = 40;
			break;
		case 5: ZX = 48;
			ZY = 48;
			break;
		case 6: ZX = 60;
			ZY = 60;
			break;
		case 7: ZX = 80;
			ZY = 80;
			break;
		case 8: ZX = 96;
			ZY = 96;
			break;
		case 9: ZX = 100;
			ZY = 100;
			break;
		case 10:ZX = 128;
			ZY = 128;
			break;
		case 11:ZX = Config.ZoomCX;
			ZY = Config.ZoomCY;
			break;
		case 99:ZX = MapZoomCX;
			ZY = MapZoomCY;
			break;
		default:return;
		}

	Map_Lock(TRUE,TRUE);
	Map_InitialiseZoom(hWnd,ZX,ZY);
	Map_CorrectPos(hWnd);
	Map_SetPropThumb(hWnd,PROP_VERTICAL);
	Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
	Map_Lock(FALSE,TRUE);
	Minimap_Refresh();
	Status_DisplayZoom();
	return;
}


// «»»» Obtention du type de zoom utilisé «««««««««««««««««««««««««««««««»

int Zoom_GetUsed()
{
	register LONG	ZX;
	register LONG	ZY;

	ZX = MapZoomCX;
	ZY = MapZoomCY;

	if ((ZX == 10)&&(ZY == 10))
		return(0);
	if ((ZX == 16)&&(ZY == 16))
		return(1);
	if ((ZX == 20)&&(ZY == 20))
		return(2);
	if ((ZX == 32)&&(ZY == 32))
		return(3);
	if ((ZX == 40)&&(ZY == 40))
		return(4);
	if ((ZX == 48)&&(ZY == 48))
		return(5);
	if ((ZX == 60)&&(ZY == 60))
		return(6);
	if ((ZX == 80)&&(ZY == 80))
		return(7);
	if ((ZX == 96)&&(ZY == 96))
		return(8);
	if ((ZX == 100)&&(ZY == 100))
		return(9);
	if ((ZX == 128)&&(ZY == 128))
		return(10);

	return(11);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
