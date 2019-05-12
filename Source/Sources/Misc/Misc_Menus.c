
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'initialisation & de manipulation des menus.
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
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'initialisation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation des éléments des menus «««««««««««««««««««««««««««»

void Misc_InitialiseMenus(HMENU hMenu, CUSTOMITEM *CustomItems)
{
	MENUITEMINFO	MenuItemInfo;
	BOOL		Continue;
	long		Width;
	long		Height;

	Width = 16;
	Height = 16;

	do {
		if (CustomItems->ciFlags&2)
			wsprintf(szMenuZoomUT,szMenuZoomU0,Config.ZoomCX,Config.ZoomCY);
		if (CustomItems->ciIconId)
			CustomItems->ciIcon = LoadImage(hInstance,MAKEINTRESOURCE(CustomItems->ciIconId),IMAGE_ICON,Width,Height,LR_DEFAULTCOLOR);
		if (CustomItems->ciFlags&1)
			CustomItems->ciImage = LoadImage(NULL,MAKEINTRESOURCE(OBM_CHECK),IMAGE_BITMAP,Width,Height,LR_DEFAULTCOLOR);

		MenuItemInfo.cbSize = sizeof(MenuItemInfo);
		MenuItemInfo.fMask = MIIM_TYPE|MIIM_DATA|MIIM_ID;
		MenuItemInfo.fType = MFT_OWNERDRAW;
		MenuItemInfo.wID = CustomItems->ciId;
		MenuItemInfo.dwItemData = (DWORD)CustomItems;
		CustomItems->ciTextLen = strlen(CustomItems->ciText);
		SetMenuItemInfo(hMenu,CustomItems->ciId,FALSE,&MenuItemInfo);
		Continue = CustomItems->ciNext;
		CustomItems++;
	} while(Continue);

	return;
}


// «»»» Suppression des données des éléments des menus ««««««««««««««««««»

void Misc_ResetMenus(CUSTOMITEM *CustomItems)
{
	BOOL	Continue;

	do {
		if (CustomItems->ciIcon) DestroyIcon(CustomItems->ciIcon);
		if (CustomItems->ciImage) DeleteObject(CustomItems->ciImage);
		CustomItems->ciIcon = 0;
		CustomItems->ciImage = 0;
		Continue = CustomItems->ciNext;
		CustomItems++;
	} while(Continue);

	return;
}


// «»»» Affichage d'un menu contextuel ««««««««««««««««««««««««««««««««««»

void Misc_DisplayContextMenu(HWND hWnd, UINT Menu, CUSTOMITEM *Items, LONG X, LONG Y, UINT CheckItem)
{
	HMENU	CtxMenu;
	HMENU	CtxPopup;
	int	Result = 0;

	CtxMenu = LoadMenu(hInstance,szCtxMenu);
	if (CtxMenu)
		{
		CtxPopup = GetSubMenu(CtxMenu,Menu);
		if (CtxPopup)
			{
			if (CheckItem != 0) Misc_CheckMenuItem(CtxPopup,CheckItem,CheckItem,CheckItem,TRUE);
			Misc_InitialiseMenus(CtxPopup,Items);
			Result = TrackPopupMenu(CtxPopup,TPM_RIGHTBUTTON|TPM_LEFTBUTTON,X,Y,NULL,hWnd,NULL);
			Misc_ResetMenus(Items);
			}
		DestroyMenu(CtxMenu);
		}

	if (!Result) Misc_PrintError(hWnd,szWindowMenuErr,NULL,MB_ICONHAND);
	return;
}


// «»»» (dé)coche un point de menu ««««««««««««««««««««««««««««««««««««««»

void Misc_CheckMenuItem(HMENU hMenu, UINT FirstItem, UINT LastItem, UINT Item, BOOL Check)
{
	UINT	i;

	if (FirstItem > LastItem)
		{
		i = LastItem;
		LastItem = FirstItem;
		FirstItem = i;
		}

	for (i = FirstItem; i != LastItem+1; i++)
		CheckMenuItem(hMenu,i,MF_UNCHECKED);

	if ((Item < FirstItem)||(Item > LastItem))
		return;

	if (Check) CheckMenuItem(hMenu,Item,MF_CHECKED);
	return;
}


// «»»» Change l'état d'une série de points de menu en fnct de drapeaux «»

void Misc_FlagsChangeMenu(HMENU hMenu, DWORD Flags, FLAGARRAY *Array)
{
	int	i;

	for (i = 0; Array[i].id != 0; i++)
		if (Array[i].flag&Flags) CheckMenuItem(hMenu,Array[i].id,MF_CHECKED);
		else CheckMenuItem(hMenu,Array[i].id,MF_UNCHECKED);

	return;
}


// «»»» Change l'état de drapeaux en fonction des pts de menu cochés ««««»

void Misc_MenuChangesFlags(HMENU hMenu, DWORD *Flags, FLAGARRAY *Array)
{
	int	i;

	for (*Flags = 0, i = 0; Array[i].id != 0; i++)
		if (Misc_IsMenuItemChecked(hMenu,Array[i].id,FALSE)) *Flags |= Array[i].flag;

	return;
}


// «»»» Test si un menu est coché (met à jour son état en conséquence) ««»

int Misc_IsMenuItemChecked(HMENU hMenu, UINT Item, BOOL Toggle)
{
	MENUITEMINFO ItemInfo;

	ZeroMemory(&ItemInfo,sizeof(MENUITEMINFO));
	ItemInfo.cbSize = sizeof(MENUITEMINFO);
	ItemInfo.fMask = MIIM_STATE;
	GetMenuItemInfo(hMenu,Item,FALSE,&ItemInfo);
	ItemInfo.fState &= MFS_CHECKED;
	if (!Toggle) return(ItemInfo.fState);

	if (ItemInfo.fState)
		{
		CheckMenuItem(hMenu,Item,MF_UNCHECKED);
		return(0);
		}
	else
		{
		CheckMenuItem(hMenu,Item,MF_CHECKED);
		return(1);
		}
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage des points de menu ««««««««««««««««««««««««««««««««««««»

void Misc_DrawMenuItem(DRAWITEMSTRUCT *DrawStruct)
{
	HFONT		 OldFont;
	TEXTMETRIC	 FontInfo;
	MENUITEMINFO	 MenuItemInfo;
	CUSTOMITEM	*CustomItem;
	COLORREF	 OldPen,OldBck,Back;
	RECT		 WindowRect;
	SIZE		 Size;
	BOOL		 IsGrayed;
	long		 Width,Height;
	long		 X,Y;
	int		 OldMode;

	MenuItemInfo.cbSize = sizeof(MenuItemInfo);
	MenuItemInfo.fMask = MIIM_DATA;
	GetMenuItemInfo((HMENU)DrawStruct->hwndItem,DrawStruct->itemID,FALSE,&MenuItemInfo);
	CustomItem = (CUSTOMITEM *)MenuItemInfo.dwItemData;
	if (!CustomItem) return;
	Width = 16;
	Height = 16;

	if (DrawStruct->itemState & ODS_SELECTED)
		{
		Back = COLOR_HIGHLIGHT;
		OldPen = SetTextColor(DrawStruct->hDC,GetSysColor(COLOR_HIGHLIGHTTEXT));
		OldBck = SetBkColor(DrawStruct->hDC,GetSysColor(COLOR_HIGHLIGHT));
		}
	else
		{
		Back = COLOR_MENU;
		OldPen = SetTextColor(DrawStruct->hDC,GetSysColor(COLOR_MENUTEXT));
		OldBck = SetBkColor(DrawStruct->hDC,GetSysColor(COLOR_MENU));
		}

	/* Affichage du fond */

	if (DrawStruct->itemState & ODS_GRAYED)
		IsGrayed = DSS_DISABLED;
	else
		{
		CopyRect(&WindowRect,&DrawStruct->rcItem);
		if (CustomItem->ciIcon) WindowRect.left += 8+Width;
		FillRect(DrawStruct->hDC,&WindowRect,(HBRUSH)(Back+1));
		WindowRect.right = WindowRect.left-2;
		WindowRect.left = DrawStruct->rcItem.left;
		if ((CustomItem->ciIcon)&&(Back == COLOR_HIGHLIGHT)) Misc_DrawBevelBoxRect(0,DrawStruct->hDC,&WindowRect,0x00000002);
		else if (CustomItem->ciIcon) FillRect(DrawStruct->hDC,&WindowRect,(HBRUSH)(Back+1));
		IsGrayed = 0;
		}

	/* Affichage des images */

	if ((CustomItem->ciImage)&&(DrawStruct->itemState&ODS_CHECKED))
		{
		X = DrawStruct->rcItem.left+4;
		Y = DrawStruct->rcItem.top+(DrawStruct->rcItem.bottom-DrawStruct->rcItem.top-Height)/2;
		DrawState(DrawStruct->hDC,NULL,NULL,(LPARAM)CustomItem->ciImage,0,X,Y,Width,Height,DST_BITMAP|IsGrayed);
		}

	if (CustomItem->ciIcon)
		{
		X = DrawStruct->rcItem.left+4;
		Y = DrawStruct->rcItem.top+(DrawStruct->rcItem.bottom-DrawStruct->rcItem.top-Height)/2;
		DrawState(DrawStruct->hDC,NULL,NULL,(LPARAM)CustomItem->ciIcon,0,X,Y,Width,Height,DST_ICON|IsGrayed);
		}

	OldFont = SelectObject(DrawStruct->hDC,Font.hFont);
	GetTextMetrics(DrawStruct->hDC,&FontInfo);
	X = DrawStruct->rcItem.left;
	Y = DrawStruct->rcItem.top+(DrawStruct->rcItem.bottom-DrawStruct->rcItem.top-FontInfo.tmHeight-FontInfo.tmExternalLeading)/2;
	X += 10+Width;

	/* Affichage du texte */

	if (CustomItem->ciFlags&4)
		{
		HPEN	LinePen,OldPen;
		long	LineY;

		LineY = DrawStruct->rcItem.top+(DrawStruct->rcItem.bottom-DrawStruct->rcItem.top)/2;
		LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
		if (LinePen)
			{
			OldPen = SelectObject(DrawStruct->hDC,LinePen);
			if (OldPen)
				{
				MoveToEx(DrawStruct->hDC,0,LineY,NULL);
				LineTo(DrawStruct->hDC,DrawStruct->rcItem.right,LineY);
				SelectObject(DrawStruct->hDC,OldPen);
				}
			DeleteObject(LinePen);
			}
		LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
		if (LinePen)
			{
			OldPen = SelectObject(DrawStruct->hDC,LinePen);
			if (OldPen)
				{
				MoveToEx(DrawStruct->hDC,0,LineY+1,NULL);
				LineTo(DrawStruct->hDC,DrawStruct->rcItem.right,LineY+1);
				SelectObject(DrawStruct->hDC,OldPen);
				}
			DeleteObject(LinePen);
			}
		GetTextExtentPoint32(DrawStruct->hDC,CustomItem->ciText,CustomItem->ciTextLen,&Size);
		CopyRect(&WindowRect,&DrawStruct->rcItem);
		WindowRect.left = WindowRect.right-Size.cx-4;
		WindowRect.right -= 4;
		FillRect(DrawStruct->hDC,&WindowRect,(HBRUSH)(COLOR_MENU+1));
		DrawState(DrawStruct->hDC,NULL,NULL,(LPARAM)CustomItem->ciText,0,DrawStruct->rcItem.right-Size.cx-4,Y,DrawStruct->rcItem.right-DrawStruct->rcItem.left,DrawStruct->rcItem.bottom-DrawStruct->rcItem.top,DST_PREFIXTEXT|DSS_DISABLED);
		}

	else if (CustomItem->ciFlags&8)
		{
		HFONT	 BoldFont,OldOldFont;
		LOGFONT  LogFont;
		#if (GRADIENT)
		HBRUSH	 ColorBrush;
		COLORREF Start;
		COLORREF End;
		COLORREF Current;
		BYTE	 R,G,B;
		int	 ColorCount;
		#endif

		CopyRect(&WindowRect,&DrawStruct->rcItem);
		WindowRect.bottom -= 2;

		#if (GRADIENT)
		Start = GetSysColor(COLOR_ACTIVECAPTION);
		End =0x00000000;
		Current = Start;
		ColorCount = WindowRect.right-WindowRect.left-61;
		WindowRect.right = WindowRect.left+1;

		for (; ColorCount != 0; --ColorCount)
			{
			ColorBrush = CreateSolidBrush(Current);
			if (ColorBrush)
				{
				FillRect(DrawStruct->hDC,&WindowRect,ColorBrush);
				R = Display_SpreadColor(ColorCount,(int)((BYTE)Start),(int)((BYTE)End),(int)((BYTE)Current));
				G = Display_SpreadColor(ColorCount,(int)((WORD)(Start)>>8),(int)((WORD)(End)>>8),(int)((WORD)(Current)>>8));
				B = Display_SpreadColor(ColorCount,(int)((Start)>>16),(int)((End)>>16),(int)((Current)>>16));
				Current = RGB(R,G,B);
				WindowRect.left++;
				WindowRect.right++;
				DeleteObject(ColorBrush);
				}
			}
		WindowRect.right = WindowRect.left+60;
		FillRect(DrawStruct->hDC,&WindowRect,GetStockObject(BLACK_BRUSH));
		#else
		FillRect(DrawStruct->hDC,&WindowRect,GetStockObject(BLACK_BRUSH));
		#endif

		GetObject(Font.hFont,sizeof(LOGFONT),&LogFont);
		LogFont.lfWeight = FW_BOLD;
		BoldFont = CreateFontIndirect(&LogFont);
		if (BoldFont) OldOldFont = SelectObject(DrawStruct->hDC,BoldFont);
		OldMode = SetBkMode(DrawStruct->hDC,TRANSPARENT);
		SetTextColor(DrawStruct->hDC,0x00FFFFFF);
		GetTextExtentPoint32(DrawStruct->hDC,CustomItem->ciText,CustomItem->ciTextLen,&Size);
		TextOut(DrawStruct->hDC,DrawStruct->rcItem.right-Size.cx-4,Y-1,CustomItem->ciText,CustomItem->ciTextLen);
		SetBkMode(DrawStruct->hDC,OldMode);
		if (BoldFont)
			{
			if (OldOldFont) SelectObject(DrawStruct->hDC,OldOldFont);
			DeleteObject(BoldFont);
			}
		}

	else DrawState(DrawStruct->hDC,NULL,NULL,(LPARAM)CustomItem->ciText,0,(CustomItem->ciFlags&0x10)?(DrawStruct->rcItem.left+8):X,Y,DrawStruct->rcItem.right-DrawStruct->rcItem.left,DrawStruct->rcItem.bottom-DrawStruct->rcItem.top,DST_PREFIXTEXT|IsGrayed);

	/* Affichage des touches de raccourci */

	if (CustomItem->ciKey)
		{
		GetTextExtentPoint32(DrawStruct->hDC,CustomItem->ciKey,strlen(CustomItem->ciKey),&Size);
		X = DrawStruct->rcItem.right-8-Size.cx;
		DrawState(DrawStruct->hDC,NULL,NULL,(LPARAM)CustomItem->ciKey,0,X,Y,DrawStruct->rcItem.right-X,DrawStruct->rcItem.bottom-DrawStruct->rcItem.top,DST_TEXT|IsGrayed);
		}

	if (OldFont) SelectObject(DrawStruct->hDC,OldFont);
	SetBkColor(DrawStruct->hDC,OldBck);
	SetTextColor(DrawStruct->hDC,OldPen);
	return;
}


// «»»» Mesure la taille d'un élément «««««««««««««««««««««««««««««««««««»

void Misc_MeasureMenuItem(HWND hItemWnd, UINT CtrlID, MEASUREITEMSTRUCT *ItemStruct)
{
	HDC		 hDC;
	SIZE		 Size;
	HFONT		 OldFont;
	CUSTOMITEM	*CustomItem;

	if (CtrlID)
		return;

	CustomItem = (CUSTOMITEM *)ItemStruct->itemData;

	hDC = GetDC(hItemWnd);
	OldFont = SelectObject(hDC,Font.hFont);
	GetTextExtentPoint32(hDC,CustomItem->ciText,CustomItem->ciTextLen,&Size);
	ItemStruct->itemWidth = Size.cx+12*2;
	ItemStruct->itemHeight = Size.cy;
	if (Size.cy < 16+5) ItemStruct->itemHeight = 16+5;
	if (CustomItem->ciFlags&8) ItemStruct->itemHeight += 2;

	if (CustomItem->ciKey)
		{
		GetTextExtentPoint32(hDC,CustomItem->ciKey,strlen(CustomItem->ciKey),&Size);
		ItemStruct->itemWidth += Size.cx+12;
		}

	if (OldFont) SelectObject(hDC,OldFont);
	ReleaseDC(hItemWnd,hDC);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END

