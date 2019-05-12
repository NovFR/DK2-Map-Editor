
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des informations.
	Cette fenêtre dépend de la carte miniature.
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
extern HWND		hStatusWnd;
extern HWND		hInfo;
extern HICON		hInfoRectIcon;
extern HICON		hInfoCopyIcon;
extern HICON		hInfoGoldIcon;
extern FONTINFO		Font;
extern FONTINFO		FixedFont;
extern LONG		MapLocked;
extern MAP		Map;
extern MAPRECT		MapCursorSel;
extern MAPCOPY		MapCopy;
extern CONFIG		Config;

static DWORD		StatsCols[]={	 0x04020101, 0xFFFFFFFF, THUMB_WATER,	 COMPILER_QUERYWORLD,
					 0x05020101, 0xFFFFFFFF, THUMB_LAVA,	 COMPILER_QUERYWORLD,
					 0x06020101, 0xFFFFFFFF, THUMB_GOLD,	 COMPILER_QUERYWORLD,
					 0x07020101, 0xFFFFFFFF, THUMB_GEMS,	 COMPILER_QUERYWORLD,
					 0x01020101, 0xFFFFFFFF, THUMB_SOLID,	 COMPILER_QUERYWORLD,
					 0x02020101, 0xFFFFFFFF, THUMB_ROCK,	 COMPILER_QUERYWORLD,
					 0x03020101, 0xFFFFFFFF, THUMB_UNCLAIMED,COMPILER_QUERYWORLD,
					 0x00010000, 0x00FF0000, THUMB_HERO,	 COMPILER_QUERYPLAYERS,
					 0x00020000, 0x00FF0000, THUMB_NEUTRAL,	 COMPILER_QUERYPLAYERS,
					 0x00030000, 0x00FF0000, THUMB_KEEPER1,	 COMPILER_QUERYPLAYERS,
					 0x00040000, 0x00FF0000, THUMB_KEEPER2,	 COMPILER_QUERYPLAYERS,
					 0x00050000, 0x00FF0000, THUMB_KEEPER3,	 COMPILER_QUERYPLAYERS,
					 0x00060000, 0x00FF0000, THUMB_KEEPER4,	 COMPILER_QUERYPLAYERS,
					 0x00070000, 0x00FF0000, THUMB_KEEPER5,	 COMPILER_QUERYPLAYERS,
					 0x00000000, 0x00000000, 0,		 0};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

int Info_CreateWindow(HWND hWnd)
{
	RECT	MinimapRect;
	RECT	StatusRect;
	RECT	ClientRect;

	hInfoRectIcon = LoadImage(hInstance,MAKEINTRESOURCE(30),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	hInfoCopyIcon = LoadImage(hInstance,MAKEINTRESOURCE(31),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	hInfoGoldIcon = LoadImage(hInstance,MAKEINTRESOURCE(32),IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	if (!hInfoRectIcon || !hInfoCopyIcon || !hInfoGoldIcon) return(0);

	GetClientRect(hWnd,&ClientRect);
	GetWindowRect(hStatusWnd,&StatusRect);
	MapWindowPoints(NULL,hWnd,(POINT *)&StatusRect,2);
	Minimap_GetArea(hWnd,&MinimapRect,TRUE);
	hInfo = CreateWindow(szStaticClass,NULL,WS_CHILD|SS_OWNERDRAW,MinimapRect.left,MinimapRect.bottom,MinimapRect.right-MinimapRect.left,ClientRect.bottom-MinimapRect.bottom-(StatusRect.bottom-StatusRect.top),hWnd,NULL,hInstance,NULL);
	if (!hInfo) return(0);

	if (!Config.HideMinimap)
		{
		ShowWindow(hInfo,SW_SHOWNORMAL);
		UpdateWindow(hInfo);
		}

	return(1);
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Info_DestroyWindow()
{
	if (hInfoRectIcon) DestroyIcon(hInfoRectIcon);
	if (hInfoCopyIcon) DestroyIcon(hInfoCopyIcon);
	if (hInfoGoldIcon) DestroyIcon(hInfoGoldIcon);
	hInfoRectIcon = NULL;
	hInfoCopyIcon = NULL;
	hInfoGoldIcon = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage & Dimensions						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage des informations ««««««««««««««««««««««««««««««««««««««»

void Info_DrawWindow(DRAWITEMSTRUCT *DrawStruct)
{
	HDC		hDC;
	HDC		hDoubleDC;
	HBITMAP		hDoubleBitmap;
	HBITMAP		hDoubleOldBitmap;
	RECT		DrawRect;
	HPEN		hPen,hOldPen;

	hDC = DrawStruct->hDC;
	CopyRect(&DrawRect,&DrawStruct->rcItem);

	hDoubleDC = CreateCompatibleDC(hDC);
	if (hDoubleDC)
		{
		hDoubleBitmap = CreateCompatibleBitmap(hDC,DrawRect.right-DrawRect.left,DrawRect.bottom-DrawRect.top);
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
				DrawRect.right -= DrawRect.left;
				DrawRect.bottom -= DrawRect.top;
				DrawRect.left = 0;
				DrawRect.top = 0;
				hDC = hDoubleDC;
				}
			}
		else
			{
			DeleteDC(hDoubleDC);
			hDoubleDC = NULL;
			}
		}

	//--- Affichage des décorations ---

	FillRect(hDC,&DrawRect,GetStockObject(DKGRAY_BRUSH));

	hPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DLIGHT));
	if (hPen)
		{
		hOldPen = SelectObject(hDC,hPen);
		if (hOldPen)
			{
			MoveToEx(hDC,DrawRect.left,DrawRect.bottom-1,NULL);
			LineTo(hDC,DrawRect.right,DrawRect.bottom-1);
			SelectObject(hDC,hOldPen);
			}
		DeleteObject(hPen);
		}

	DrawRect.left += 2;
	DrawRect.top += 2;
	DrawRect.right -= 2;
	DrawRect.bottom -= 2;

	//--- Affichage des informations concernant la sélection ---

	if (DrawRect.bottom-DrawRect.top >= 32*2+FixedFont.FontHeight*3+50)
		{
		HFONT		OldFont;
		MAPCLIENT	MapClient;
		RECT		TextRect;
		COLORREF	OldTextColor;
		COLORREF	OldBackColor;
		char		TempText[128];

		OldFont = SelectObject(hDC,Font.hFont);
		if (OldFont)
			{
			OldTextColor = SetTextColor(hDC,0x00FFFFFF);
			OldBackColor = SetBkColor(hDC,GetPixel(hDC,0,0));

			// Rectangle de sélection
			//
			DrawIconEx(hDC,DrawRect.left,DrawRect.top,hInfoRectIcon,32,32,0,GetStockObject(DKGRAY_BRUSH),DI_NORMAL);
			TextRect.left = DrawRect.left+4+32;
			TextRect.right = DrawRect.right;
			TextRect.top = DrawRect.top;
			TextRect.bottom = DrawRect.top+32;
			DrawText(hDC,szInfoSelect,-1,&TextRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
			SelectObject(hDC,FixedFont.hFont);
			Map_CorrectRect(&MapCursorSel,&MapClient);
			if (!MapCursorSel.hidden) wsprintf(TempText,szInfoSelectRect,MapClient.X,MapClient.Y,MapClient.Width,MapClient.Height);
			else strcpy(TempText,szInfoSelectRectEmpty);
			SetTextColor(hDC,0x00CCCCCC);
			TextRect.top = TextRect.bottom;
			TextRect.bottom = TextRect.top+FixedFont.FontHeight*2;
			DrawText(hDC,TempText,-1,&TextRect,DT_LEFT|DT_NOPREFIX);

			// Copie
			//
			DrawRect.top = TextRect.bottom;
			DrawIconEx(hDC,DrawRect.left,DrawRect.top,hInfoCopyIcon,32,32,0,GetStockObject(DKGRAY_BRUSH),DI_NORMAL);
			TextRect.top = DrawRect.top;
			TextRect.bottom = DrawRect.top+32;
			SetTextColor(hDC,0x00FFFFFF);
			SelectObject(hDC,Font.hFont);
			DrawText(hDC,szInfoCopy,-1,&TextRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
			SelectObject(hDC,FixedFont.hFont);
			if (MapCopy.Map) wsprintf(TempText,szInfoCopyRect,MapCopy.Width,MapCopy.Height);
			else strcpy(TempText,szInfoCopyRectEmpty);
			SetTextColor(hDC,0x00CCCCCC);
			TextRect.top = TextRect.bottom;
			TextRect.bottom = TextRect.top+FixedFont.FontHeight;
			DrawText(hDC,TempText,-1,&TextRect,DT_LEFT|DT_NOPREFIX);

			DrawRect.top = TextRect.bottom+12;
			SetTextColor(hDC,OldTextColor);
			SetBkColor(hDC,OldBackColor);
			SelectObject(hDC,OldFont);
			}
		}

	//--- Affichage des informations concernant l'or disponible ---

	if (DrawRect.bottom-DrawRect.top >= 32+FixedFont.FontHeight*2+50)
		{
		HFONT		OldFont;
		RECT		TextRect;
		COLORREF	OldTextColor;
		COLORREF	OldBackColor;
		char		TempText[128];

		OldFont = SelectObject(hDC,Font.hFont);
		if (OldFont)
			{
			OldTextColor = SetTextColor(hDC,0x00FFFFFF);
			OldBackColor = SetBkColor(hDC,GetPixel(hDC,0,0));

			// Affichage de l'icône
			//
			DrawIconEx(hDC,DrawRect.left,DrawRect.top,hInfoGoldIcon,32,32,0,GetStockObject(DKGRAY_BRUSH),DI_NORMAL);
			TextRect.left = DrawRect.left+4+32;
			TextRect.right = DrawRect.right;
			TextRect.top = DrawRect.top;
			TextRect.bottom = DrawRect.top+32;
			DrawText(hDC,szInfoGold,-1,&TextRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

			// Or disponible dans tout le niveau
			//
			if ((MapLocked)||(!Map.Map)) strcpy(TempText,szInfoGoldInLevelNA);
			else wsprintf(TempText,szInfoGoldInLevel,Compiler_InfoGoldAmountInRect(COMPILER_QUERYMAPGOLD,NULL,NULL));
			SelectObject(hDC,FixedFont.hFont);
			SetTextColor(hDC,0x00CCCCCC);
			TextRect.top = TextRect.bottom;
			TextRect.bottom = TextRect.top+FixedFont.FontHeight;
			DrawText(hDC,TempText,-1,&TextRect,DT_LEFT|DT_NOPREFIX);

			// Or disponible dans le rectangle de sélection
			//
			if ((MapLocked)||(!Map.Map)||(MapCursorSel.hidden)) strcpy(TempText,szInfoGoldInRectNA);
			else wsprintf(TempText,szInfoGoldInRect,Compiler_InfoGoldAmountInRect(COMPILER_QUERYMAPGOLD,NULL,&MapCursorSel));
			TextRect.top = TextRect.bottom;
			TextRect.bottom = TextRect.top+FixedFont.FontHeight;
			DrawText(hDC,TempText,-1,&TextRect,DT_LEFT|DT_NOPREFIX);

			DrawRect.top = TextRect.bottom+12;
			SetTextColor(hDC,OldTextColor);
			SetBkColor(hDC,OldBackColor);
			SelectObject(hDC,OldFont);
			}
		}

	//--- Affichage de l'utilisation des éléments ---

	if (DrawRect.bottom-DrawRect.top >= Font.FontHeight+20)
		{
		HBRUSH		hBrush;
		HFONT		OldFont;
		RECT		TextRect;
		COLORREF	OldTextColor;
		COLORREF	OldBackColor;
		long		Count;
		long		Percent;
		long		Width,Height;
		long		Start;
		int		i;

		TextRect.left = DrawRect.left;
		TextRect.right = DrawRect.right;
		TextRect.top = DrawRect.top;
		TextRect.bottom = DrawRect.bottom-2;
		Misc_DrawBevelBoxRect(NULL,hDC,&TextRect,0x80000008);
		TextRect.left++;
		TextRect.right--;
		TextRect.top++;
		TextRect.bottom--;
		FillRect(hDC,&TextRect,GetStockObject(BLACK_BRUSH));

		// Affichage de l'information
		//
		OldFont = SelectObject(hDC,Font.hFont);
		if (OldFont)
			{
			OldTextColor = SetTextColor(hDC,0x00FFFFFF);
			OldBackColor = SetBkColor(hDC,0x00000000);

			TextRect.left = DrawRect.left+4;
			TextRect.right = DrawRect.right-4;
			TextRect.top = DrawRect.top+4;
			TextRect.bottom = DrawRect.top+4+Font.FontHeight;
			DrawText(hDC,szInfoStats,-1,&TextRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

			DrawRect.top = TextRect.bottom+4;
			SetTextColor(hDC,OldTextColor);
			SetBkColor(hDC,OldBackColor);
			SelectObject(hDC,OldFont);
			}

		// Affichage des statistiques
		//
		Width = (DrawRect.right-DrawRect.left-12)/(sizeof(StatsCols)/(4*4));
		Start = (DrawRect.right-DrawRect.left-12)%(sizeof(StatsCols)/(4*4));
		for (TextRect.left = DrawRect.left+6+Start, i = 0; StatsCols[i] != 0; i += 4, TextRect.left += Width)
			{
			TextRect.right = TextRect.left+Width-2;
			TextRect.top = DrawRect.top+4;
			TextRect.bottom = DrawRect.bottom-6;

			Height = TextRect.bottom-TextRect.top-1;
			Count = Compiler_InfoEnumTerrainInRect(StatsCols[i],StatsCols[i+1],StatsCols[i+3],NULL);
			Percent = Count*100 / (Map.Properties.Width*Map.Properties.Height);
			Height = Percent*Height/100;
			Height++;
			TextRect.top = TextRect.bottom-Height;

			hBrush = CreateSolidBrush(StatsCols[i+2]);
			if (hBrush)
				{
				FillRect(hDC,&TextRect,hBrush);
				DeleteObject(hBrush);
				}
			}
		}

	//--- Met à jour l'affichage si nécessaire ---

	if (hDoubleDC)
		{
		BitBlt(DrawStruct->hDC,DrawStruct->rcItem.left,DrawStruct->rcItem.top,DrawStruct->rcItem.right-DrawStruct->rcItem.left,DrawStruct->rcItem.bottom-DrawStruct->rcItem.top,hDoubleDC,0,0,SRCCOPY);
		SelectObject(hDoubleDC,hDoubleOldBitmap);
		DeleteObject(hDoubleBitmap);
		DeleteDC(hDoubleDC);
		}

	return;
}


// «»»» Met à jour le contenu de la fenêtre «««««««««««««««««««««««««««««»

void Info_Update()
{
	if ( (IsWindow(hInfo)) && (IsWindowVisible(hInfo)) )
		{
		InvalidateRect(hInfo,NULL,FALSE);
		UpdateWindow(hInfo);
		}
	return;
}


// «»»» Changement de la taille de la fenêtre «««««««««««««««««««««««««««»

void Info_Resize(LONG Height)
{
	if ( (IsWindow(hInfo)) && (IsWindowVisible(hInfo)) )
		{
		RECT	MinimapRect;
		RECT	StatusRect;

		Minimap_GetArea(hWnd,&MinimapRect,FALSE);
		GetWindowRect(hStatusWnd,&StatusRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&StatusRect,2);
		SetWindowPos(hInfo,NULL,MinimapRect.left,MinimapRect.bottom,MinimapRect.right-MinimapRect.left,Height-MinimapRect.bottom-(StatusRect.bottom-StatusRect.top),SWP_NOZORDER);
		InvalidateRect(hInfo,NULL,FALSE);
		UpdateWindow(hInfo);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
