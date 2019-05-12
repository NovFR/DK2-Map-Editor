
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre de statut.
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
extern HWND		hStatusWnd;
extern HIMAGELIST	hDrawingTools;
extern HIMAGELIST	hMiscTools;
extern MAP		Map;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern UINT		Timer;
extern DWORD		TickCount;
extern DWORD		ToolsItemID;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions relatives à la fenêtre					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre de status ««««««««««««««««««««««««««««««««»

int Status_CreateWindow(HWND hWnd)
{
	hStatusWnd = CreateStatusWindow(WS_CHILD|WS_VISIBLE,NULL,hWnd,7777);
	if (!hStatusWnd) return(0);

	Status_UpdateParts(hWnd);
	return(1);
}


// «»»» Modifie la taille des parties de la barre de statut «««««««««««««»

void Status_UpdateParts(HWND hWnd)
{
	RECT	ClientRect;
	int	StatusParts[5]={-1,-1,-1,-1,-1};

	GetClientRect(hWnd,&ClientRect);
	StatusParts[0] = ClientRect.right-356;
	StatusParts[1] = ClientRect.right-178;
	StatusParts[2] = ClientRect.right-148;
	StatusParts[3] = ClientRect.right-70;
	SendMessage(hStatusWnd,SB_SETPARTS,(WPARAM)5,(LPARAM)&StatusParts);
	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TOOLPART|SBT_OWNERDRAW,(LPARAM)NULL);
	return;
}


// «»»» Modification de la taille de la fenêtre «««««««««««««««««««««««««»

void Status_Resize(HWND hWnd, LONG Width)
{
	SetWindowPos(hStatusWnd,NULL,0,0,Width,0,SWP_NOMOVE|SWP_NOZORDER);
	Status_UpdateParts(hWnd);
	return;
}


// «»»» Affichage de l'élément se trouvant sous la souris «««««««««««««««»

void Status_DisplayMapElement(LONG MapX, LONG MapY)
{
	MAPTHING	*Thing;
	register BYTE	*MapPtr;
	char	 	 Output[256];

	MapPtr = Map.Map+MapX*MAP_CONTROLBYTES+MapY*Map.Properties.Width*MAP_CONTROLBYTES;

	switch(*(MapPtr+3))
		{
		case 0:	strcpy(Output,World[*MapPtr].Name);
			break;
		case 1:	strcpy(Output,Rooms[*MapPtr].Name);
			strcat(Output,szMapSeparator);
			strcat(Output,Players[(*(MapPtr+1))-1].Name);
			break;
		case 2:	strcpy(Output,Gates[*MapPtr].Name);
			break;
		case 3:	strcpy(Output,Walls[*MapPtr].Name);
			strcat(Output,szMapSeparator);
			strcat(Output,Players[(*(MapPtr+1))-1].Name);
			break;
		default:Output[0] = 0;
			break;
		}

	Thing = Things_GetAtMapPos(&MapDoors,MapX,MapY);
	Things_StrCat(szMapSlash,Output,Thing,Doors);
	Thing = Things_GetAtMapPos(&MapTraps,MapX,MapY);
	Things_StrCat(szMapSlash,Output,Thing,Traps);
	Thing = Things_GetAtMapPos(&MapObjects,MapX,MapY);
	Things_StrCat(szMapSlash,Output,Thing,Objects);
	Thing = Things_GetAtMapPos(&MapMagicalObjects,MapX,MapY);
	Things_StrCat(szMapSlash,Output,Thing,(Map.Properties.Version >= 17)?MagicsV17:Magics);

	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_INFOPART,(LPARAM)Output);
	return;
}


// «»»» Affichage des coordonnées de la souris ««««««««««««««««««««««««««»

void Status_DisplayMouseCoords(MAPRECT *MapRect, UINT Type)
{
	MAPCLIENT	MapClient;
	char		Output[64];

	switch(Type)
		{
		case PAINTMODE_RECT:
			Map_CorrectRect(MapRect,&MapClient);
			wsprintf(Output,szRectCoordinates,MapClient.X,MapClient.Y,MapClient.Width,MapClient.Height);
			break;

		default:wsprintf(Output,szMouseCoordinates,MapRect->x1,MapRect->y1);
			break;
		}

	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_MOUSEPART,(LPARAM)Output);
	return;
}


// «»»» Affichage du zoom «««««««««««««««««««««««««««««««««««««««««««««««»

void Status_DisplayZoom()
{
	char	Output[64];

	wsprintf(Output,szZoom,MapZoomCX,MapZoomCY);
	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_ZOOMPART,(LPARAM)Output);
	return;
}


// «»»» Affichage du mode de dessin «««««««««««««««««««««««««««««««««««««»

void Status_DrawPaintMode(DRAWITEMSTRUCT *ItemStruct)
{
	HIMAGELIST	ImageList;
	LONG		X,Y;
	LONG		W,H;
	int		Image;

	FillRect(ItemStruct->hDC,&ItemStruct->rcItem,GetStockObject(BLACK_BRUSH));

	for (Image = 0; DrawingTools[Image].Id != 0; Image++)
		if (DrawingTools[Image].Id == ToolsItemID)
			{
			ImageList = hDrawingTools;
			goto Draw_Button;
			}

	for (Image = 0; MiscTools[Image].Id != 0; Image++)
		if (MiscTools[Image].Id == ToolsItemID)
			{
			ImageList = hMiscTools;
			goto Draw_Button;
			}

	return;

Draw_Button:
	W = 20;
	H = 20;
	if (W > ItemStruct->rcItem.right-ItemStruct->rcItem.left) W = ItemStruct->rcItem.right-ItemStruct->rcItem.left;
	if (H > ItemStruct->rcItem.bottom-ItemStruct->rcItem.top) H = ItemStruct->rcItem.bottom-ItemStruct->rcItem.top;
	X = ItemStruct->rcItem.left+(ItemStruct->rcItem.right-ItemStruct->rcItem.left-W)/2;
	Y = ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top-H)/2;
	if (X < 0) X = 0;
	if (Y < 0) Y = 0;
	ImageList_DrawEx(ImageList,Image,ItemStruct->hDC,X,Y,W,H,CLR_DEFAULT,CLR_DEFAULT,ILD_NORMAL);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions relatives au timer					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création du timer «««««««««««««««««««««««««««««««««««««««««««««««»

int Status_CreateTimer(HWND hWnd)
{
	TickCount = GetTickCount();
	Timer = SetTimer(hWnd,1,1000,NULL);
	if (!Timer) return(0);
	Status_DisplayTimer();
	return(1);
}


// «»»» Suppression du timer ««««««««««««««««««««««««««««««««««««««««««««»

void Status_KillTimer()
{
	if (Timer) KillTimer(hWnd,1);
	Timer = 0;
	return;
}


// «»»» Affichage du timer ««««««««««««««««««««««««««««««««««««««««««««««»

void Status_DisplayTimer()
{
	DWORD	CurrentCount;
	UINT	Hours;
	UINT	Minutes;
	UINT	Seconds;
	char	Time[64];

	CurrentCount = GetTickCount()-TickCount;
	Seconds = CurrentCount/1000%60;
	Minutes = CurrentCount/1000/60%60;
	Hours = CurrentCount/1000/60/60%24;
	wsprintf(Time,szTime,Hours,Minutes,Seconds);
	SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_TIMERPART,(LPARAM)Time);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
