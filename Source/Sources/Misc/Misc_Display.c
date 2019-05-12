
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'affichage globales.
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
extern FONTINFO		Font;
extern NODE		MapAreas;
extern NODE		MapEffects;
extern LONG		LocaleGlobal;
extern char		Locale[LOCALE_MAXLEN*7+LOCALE_MAXLEN*12];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage générales					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'une date ««««««««««««««««««««««««««««««««««««««««««««»

void Misc_FmtDate(char *Buffer, SYSTEMTIME *FmtTime)
{
	SYSTEMTIME	Time;
	int		i;

	for (i = CAL_SDAYNAME1, LocaleGlobal = 0; i <= CAL_SDAYNAME7; i++, LocaleGlobal++)
		EnumCalendarInfo(Misc_FmtDateEnumProc,LOCALE_USER_DEFAULT,ENUM_ALL_CALENDARS,i);
	for (i = CAL_SMONTHNAME1; i <= CAL_SMONTHNAME12; i++, LocaleGlobal++)
		EnumCalendarInfo(Misc_FmtDateEnumProc,LOCALE_USER_DEFAULT,ENUM_ALL_CALENDARS,i);

	if (!FmtTime) GetLocalTime(&Time);
	else CopyMemory(&Time,FmtTime,sizeof(SYSTEMTIME));
	if (Time.wDayOfWeek == 0) Time.wDayOfWeek = 6;
	else if (Time.wDayOfWeek != (WORD)-1) Time.wDayOfWeek--;
	Time.wMonth--;

	#if (FRENCH)
	if (Time.wYear == 0) strcpy(Buffer,szNone);
	else if (Time.wDayOfWeek != (WORD)-1) wsprintf(Buffer,szDate,&Locale[Time.wDayOfWeek*LOCALE_MAXLEN],Time.wDay,&Locale[7*LOCALE_MAXLEN+Time.wMonth*LOCALE_MAXLEN],Time.wYear,Time.wHour,Time.wMinute,Time.wSecond);
	else wsprintf(Buffer,szDateNoDay,Time.wDay,&Locale[7*LOCALE_MAXLEN+Time.wMonth*LOCALE_MAXLEN],Time.wYear,Time.wHour,Time.wMinute,Time.wSecond);
	#else
	#error Language is not french, Date needs to be changed...
	#endif
	return;
}

//--- Obtention des noms des jours et des mois ---

BOOL CALLBACK Misc_FmtDateEnumProc(LPTSTR DateFormat)
{
	strncpy(&Locale[LOCALE_MAXLEN*LocaleGlobal],DateFormat,LOCALE_MAXLEN);
	return(TRUE);
}


// «»»» Affichage des groupes «««««««««««««««««««««««««««««««««««««««««««»

void Misc_DrawGroup(DRAWITEMSTRUCT *ItemStruct)
{
	RECT		WorkRect;
	SIZE		TextSize;
	COLORREF	OldBackColor;
	COLORREF	OldTextColor;
	HPEN		LightPen;
	HPEN		ShadowPen;
	HPEN		OldPen;
	LONG		Y;
	char		Text[64];

	LightPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHIGHLIGHT));
	ShadowPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
	if ((LightPen)&&(ShadowPen))
		{
		OldPen = SelectObject(ItemStruct->hDC,LightPen);
		if (OldPen)
			{
			Y = ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top)/2;
			MoveToEx(ItemStruct->hDC,ItemStruct->rcItem.left,Y,NULL);
			LineTo(ItemStruct->hDC,ItemStruct->rcItem.right,Y);
			SelectObject(ItemStruct->hDC,ShadowPen);
			MoveToEx(ItemStruct->hDC,ItemStruct->rcItem.left,Y-1,NULL);
			LineTo(ItemStruct->hDC,ItemStruct->rcItem.right,Y-1);
			SelectObject(ItemStruct->hDC,OldPen);
			}
		}
	if (LightPen) DeleteObject(LightPen);
	if (ShadowPen) DeleteObject(ShadowPen);

	GetWindowText(ItemStruct->hwndItem,Text,64);
	if (!Text[0]) return;

	GetTextExtentPoint32(ItemStruct->hDC,Text,strlen(Text),&TextSize);
	WorkRect.left = ItemStruct->rcItem.left+8;
	WorkRect.top = ItemStruct->rcItem.top;
	WorkRect.right = WorkRect.left+TextSize.cx+4;
	WorkRect.bottom = ItemStruct->rcItem.bottom;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_MENU+1));

	OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_MENUTEXT));
	OldBackColor = SetBkColor(ItemStruct->hDC,GetSysColor(COLOR_MENU));
	DrawText(ItemStruct->hDC,Text,-1,&WorkRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	SetTextColor(ItemStruct->hDC,OldTextColor);
	SetBkColor(ItemStruct->hDC,OldBackColor);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage relatives aux comboboxes			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve/Sélectionne une zone «««««««««««««««««««««««««««««««««««»

//--- Retrouve la zone sélectionnée ---

UINT Misc_GetArea(HWND hDlg, UINT CtlID, BOOL Decrease)
{
	MAPRECTEX	*Area;
	int		 Offset;

	if (!MapAreas.next) return(0);
	Offset = SendDlgItemMessage(hDlg,CtlID,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (Decrease) Offset--;
	if (Offset < 0) return(0);
	Area = (MAPRECTEX *)MapAreas.next;
	while (Offset--) Area = (MAPRECTEX *)Area->node.next;

	return(Area->id);
}

//--- Liste des zones ---

void Misc_SetArea(HWND hDlg, UINT CtlID, UINT AreaID)
{
	MAPRECTEX	*Area;
	int		 Offset;

	for (Offset = 0, Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
		{
		if (Area->id == AreaID) break;
		Offset++;
		}

	if (!Area)
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	else
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)Offset,(LPARAM)0);

	return;
}


// «»»» Retrouve/Sélectionne un effet «««««««««««««««««««««««««««««««««««»

//--- Retrouve l'effet sélectionné ---

UINT Misc_GetEffect(HWND hDlg, UINT CtlID, BOOL Decrease)
{
	MAPRECTEX	*Effect;
	int		 Offset;

	if (!MapEffects.next) return(0);
	Offset = SendDlgItemMessage(hDlg,CtlID,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (Decrease) Offset--;
	if (Offset < 0) return(0);
	Effect = (MAPRECTEX *)MapEffects.next;
	while (Offset--) Effect = (MAPRECTEX *)Effect->node.next;

	return(Effect->id);
}

//--- Liste des zones ---

void Misc_SetEffect(HWND hDlg, UINT CtlID, UINT EffectID)
{
	MAPRECTEX	*Effect;
	int		 Offset;

	for (Offset = 0, Effect = (MAPRECTEX *)MapEffects.next; Effect != 0; Effect = (MAPRECTEX *)Effect->node.next)
		{
		if (Effect->id == EffectID) break;
		Offset++;
		}

	if (!Effect)
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	else
		SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)Offset,(LPARAM)0);

	return;
}


// «»»» Création de la liste des effets «««««««««««««««««««««««««««««««««»

int Misc_CreateEffectComboBoxList(HWND hDlg, UINT CtlID, BOOL None)
{
	MAPRECTEX	*Effect;
	char		*Text;

	if (!MapEffects.next)
		return((SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)szNoEffectErr) < 0)?0:1);

	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text) return(0);

	if (None)
		{
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)szTriggersNoEffectSelected) < 0)
			{
			GlobalFree(Text);
			return(0);
			}
		}

	for (Effect = (MAPRECTEX *)MapEffects.next; Effect != 0; Effect = (MAPRECTEX *)Effect->node.next)
		{
		RectEx_StrCpy(Text+2048,Effect->name);
		RectEx_StrCpy(Text+2048+MAP_RECTMAXNAME,Effect->info);
		if (Effect->info[0]) wsprintf(Text,szTriggersEffectInfo,Effect->id,Text+2048,Text+2048+MAP_RECTMAXNAME);
		else wsprintf(Text,szTriggersEffect,Effect->id,Text+2048);
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)Text) < 0)
			{
			GlobalFree(Text);
			return(0);
			}
		}

	GlobalFree(Text);
	return(1);
}


// «»»» Création de la liste des zones ««««««««««««««««««««««««««««««««««»

int Misc_CreateAreaComboBoxList(HWND hDlg, UINT CtlID, BOOL None)
{
	MAPRECTEX	*Area;
	char		*Text;

	if (!MapAreas.next)
		return((SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)szNoAreaErr) < 0)?0:1);

	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text) return(0);

	if (None)
		{
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)szTriggersNoAreaSelected) < 0)
			{
			GlobalFree(Text);
			return(0);
			}
		}

	for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
		{
		RectEx_StrCpy(Text+2048,Area->name);
		RectEx_StrCpy(Text+2048+MAP_RECTMAXNAME,Area->info);
		if (Area->info[0]) wsprintf(Text,szTriggersAreaInfo,Area->id,Text+2048,Text+2048+MAP_RECTMAXNAME);
		else wsprintf(Text,szTriggersArea,Area->id,Text+2048);
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)Text) < 0)
			{
			GlobalFree(Text);
			return(0);
			}
		}

	GlobalFree(Text);
	return(1);
}


// «»»» Création d'une liste de niveaux «««««««««««««««««««««««««««««««««»

int Misc_CreateLevelComboBoxList(HWND hDlg, UINT CtlID, LONG Level)
{
	char	Number[4];
	int	i;

	for (i = 1; i != 10+1; i++)
		{
		wsprintf(Number,szNumber,i);
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)Number) < 0) return(0);
		}
	SendDlgItemMessage(hDlg,CtlID,CB_SETCURSEL,(WPARAM)Level-1,(LPARAM)0);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage relatives à la liste des joueurs		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la liste des joueurs ««««««««««««««««««««««««««««««««»

int Misc_CreatePlayerComboBoxList(HWND hDlg, UINT CtlID, UINT StartAtPlayer, BOOL EnableAuto)
{
	int	i;

	if (EnableAuto) if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)0) < 0) return(0);
	if (!StartAtPlayer) if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)-1) < 0) return(0);

	for (i = (StartAtPlayer)?(StartAtPlayer-1):0; i != MAP_MAXPLAYERS; i++)
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)i+1) < 0) return(0);

	return(1);
}


// «»»» Mesure la taille des entrées ««««««««««««««««««««««««««««««««««««»

void Misc_MeasurePlayerComboBoxList(UINT CtlID, MEASUREITEMSTRUCT *ItemStruct)
{
	if (!(ItemStruct->CtlType&ODT_COMBOBOX)) return;
	if (ItemStruct->CtlID != CtlID) return;

	ItemStruct->itemHeight = Font.FontHeight+2;
	return;
}


// «»»» Affichage d'un joueur «««««««««««««««««««««««««««««««««««««««««««»

void Misc_DisplayPlayerComboBoxList(UINT CtlID, DRAWITEMSTRUCT *ItemStruct)
{
	RECT		 WorkRect;
	COLORREF	 OldTextColor;
	COLORREF	 OldBackColor;
	COLORREF	 BackColor;
	COLORREF	 PlayerColor;
	HBRUSH		 PlayerBrush;
	HBRUSH		 PlayerOldBrush;
	char		*PlayerText;

	if (!(ItemStruct->CtlType&ODT_COMBOBOX)) return;
	if (ItemStruct->CtlID != CtlID) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 1;
	WorkRect.top += 1;
	WorkRect.right -= 1;
	WorkRect.bottom -= 1;
	BackColor = GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW);
	OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	OldBackColor = SetBkColor(ItemStruct->hDC,BackColor);

	switch(ItemStruct->itemData)
		{
		case 0:	PlayerColor = GetSysColor(COLOR_WINDOW);
			PlayerText = szAuto;
			break;
		case -1:PlayerColor = GetSysColor(COLOR_WINDOW);
			PlayerText = szAny;
			break;
		default:PlayerColor = Players[ItemStruct->itemData-1].Color;
			PlayerText = Players[ItemStruct->itemData-1].Name;
		break;
		}

	if (ItemStruct->itemState&ODS_DISABLED)
		{
		PlayerColor = GetSysColor(COLOR_MENU);
		BackColor = PlayerColor;
		SetBkColor(ItemStruct->hDC,PlayerColor);
		SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
		}

	//--- Draws the player's color ---

	WorkRect.right = WorkRect.left+16;
	PlayerBrush = CreateSolidBrush(PlayerColor);
	if (PlayerBrush)
		{
		PlayerOldBrush = SelectObject(ItemStruct->hDC,PlayerBrush);
		if (PlayerOldBrush)
			{
			FillRect(ItemStruct->hDC,&WorkRect,PlayerBrush);
			SelectObject(ItemStruct->hDC,PlayerOldBrush);
			}
		DeleteObject(PlayerBrush);
		}
	WorkRect.left = WorkRect.right+1;
	WorkRect.right = ItemStruct->rcItem.right-1;

	//--- Fills the control background ---

	PlayerBrush = CreateSolidBrush(BackColor);
	if (PlayerBrush)
		{
		PlayerOldBrush = SelectObject(ItemStruct->hDC,PlayerBrush);
		if (PlayerOldBrush)
			{
			FillRect(ItemStruct->hDC,&WorkRect,PlayerBrush);
			SelectObject(ItemStruct->hDC,PlayerOldBrush);
			}
		DeleteObject(PlayerBrush);
		}
	if (ItemStruct->itemState&ODS_FOCUS)
		DrawFocusRect(ItemStruct->hDC,&WorkRect);

	//--- Draws the player's text ---

	WorkRect.left++;
	DrawText(ItemStruct->hDC,PlayerText,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	SetBkColor(ItemStruct->hDC,OldBackColor);
	SetTextColor(ItemStruct->hDC,OldTextColor);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage relatives aux boutons			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage des textes à droites des boutons d'édition ««««««««««««»

void Misc_PrintButtonRightText(HWND hDlg, UINT CtlID, char *Text)
{
	HWND		hCtrl;

	hCtrl = GetDlgItem(hDlg,CtlID);
	if (hCtrl)
		{
		HDC		hDC;
		RECT		Rect;
		SIZE		Size;
		HFONT		OldFont;
		COLORREF	OldBackColor;
		COLORREF	OldTextColor;
		UINT		State;

		hDC = GetDC(hDlg);
		if (hDC)
			{
			OldBackColor = SetBkColor(hDC,GetSysColor(COLOR_MENU));
			OldTextColor = SetTextColor(hDC,GetSysColor(COLOR_WINDOWTEXT));

			GetWindowRect(hCtrl,&Rect);
			MapWindowPoints(NULL,hDlg,(POINT *)&Rect,2);

			OldFont = SelectObject(hDC,Font.hFont);
			if (OldFont)
				{
				GetTextExtentPoint32(hDC,Text,strlen(Text),&Size);
				Rect.left -= Size.cx+8;
				Rect.top = Rect.top+(Rect.bottom-Rect.top-Size.cy)/2;
				State = DST_TEXT;
				if (!IsWindowEnabled(hCtrl)) State |= DSS_DISABLED;
				DrawState(hDC,NULL,NULL,(LPARAM)Text,(WPARAM)0,Rect.left,Rect.top,Size.cx,Size.cy,State);
				SelectObject(hDC,OldFont);
				}

			SetBkColor(hDC,OldBackColor);
			SetTextColor(hDC,OldTextColor);
			ReleaseDC(hWnd,hDC);
			}
		}

	return;
}


// «»»» Affichage des boutons haut et bas «««««««««««««««««««««««««««««««»

void Misc_DrawButtonIcon(DRAWITEMSTRUCT *ItemStruct, HICON hIcon)
{
	RECT	WorkRect;
	UINT	State;
	LONG	X,Y;

	if (ItemStruct->CtlType != ODT_BUTTON) return;

	State = DFCS_BUTTONPUSH;
	if (ItemStruct->itemState&ODS_SELECTED) State |= DFCS_PUSHED;
	if (ItemStruct->itemState&ODS_GRAYED) State |= DFCS_INACTIVE;
	DrawFrameControl(ItemStruct->hDC,&ItemStruct->rcItem,DFCS_BUTTONPUSH,State);

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 2;
	WorkRect.top += 2;
	WorkRect.right -= 2;
	WorkRect.bottom -= 2;

	X = WorkRect.left+(WorkRect.right-WorkRect.left-16)/2;
	Y = WorkRect.top+(WorkRect.bottom-WorkRect.top-16)/2;
	State = DST_ICON;
	if (ItemStruct->itemState&ODS_DISABLED) State |= DSS_DISABLED;
	DrawState(ItemStruct->hDC,NULL,NULL,(LPARAM)hIcon,(WPARAM)0,X,Y,16,16,State);

	if (ItemStruct->itemState&ODS_FOCUS)
		{
		WorkRect.left += 2;
		WorkRect.top += 2;
		WorkRect.right -= 2;
		WorkRect.bottom -= 2;
		DrawFocusRect(ItemStruct->hDC,&WorkRect);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création d'un bouton de défilement pour les nombres		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création du bouton de défilement ««««««««««««««««««««««««««««««««»

int Misc_CreateUpDownButton(HWND hDlg, UINT CtlID, UINT UpDownID)
{
	RECT	WindowRect;
	HWND	hCtrl;
	HWND	hWnd;

	hCtrl = GetDlgItem(hDlg,CtlID);
	GetWindowRect(hCtrl,&WindowRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&WindowRect,2);

	hWnd = CreateUpDownControl(WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT,0,0,WindowRect.bottom-WindowRect.top,WindowRect.bottom-WindowRect.top,hDlg,UpDownID,hInstance,hCtrl,0,1,0);
	return((hWnd)?1:0);
}


// «»»» Changement d'un nombre ««««««««««««««««««««««««««««««««««««««««««»

//--- [Uniquement nombres positifs] ---

void Misc_SetUpDownInt(HWND hDlg, UINT CtlID, NM_UPDOWN *UpDown)
{
	int	Number;

	Number = GetDlgItemInt(hDlg,CtlID,NULL,FALSE);
	if (UpDown->iDelta > 0)
		Number--;
	else
		Number++;
	SetDlgItemInt(hDlg,CtlID,Number,FALSE);
	return;
}

//--- [Tous les nombres] ---

void Misc_SetUpDownIntSigned(HWND hDlg, UINT CtlID, NM_UPDOWN *UpDown)
{
	int	Number;

	Number = GetDlgItemInt(hDlg,CtlID,NULL,TRUE);
	if (UpDown->iDelta > 0)
		Number--;
	else
		Number++;
	SetDlgItemInt(hDlg,CtlID,Number,TRUE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage des listes					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mesure la taille des entrées ««««««««««««««««««««««««««««««««««««»

void Misc_MeasureListViewItem(MEASUREITEMSTRUCT *ItemStruct, LONG ImagesSize)
{
	if (!(ItemStruct->CtlType&ODT_LISTVIEW)) return;

	ItemStruct->itemHeight = Font.FontHeight+2;
	if (ItemStruct->itemHeight < ImagesSize+2) ItemStruct->itemHeight = ImagesSize+2;
	return;
}


// «»»» Affichage des entrées de la liste «««««««««««««««««««««««««««««««»

void Misc_DrawListViewItem(DRAWITEMSTRUCT *ItemStruct, LONG ImagesSize, LONG Columns, DWORD Alignement)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		WorkRect;
	SIZE		TextSize;
	UINT		TextFormat;
	HIMAGELIST	ImageList;
	HFONT		OldFont;
	COLORREF	OldColor;
	char		ItemText[128];

	if (ItemStruct->CtlType != ODT_LISTVIEW) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	Item.mask = LVIF_IMAGE;
	Item.iItem = ItemStruct->itemID;
	Item.iSubItem = 0;
	if (!SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	ImageList = (HIMAGELIST)SendMessage(ItemStruct->hwndItem,LVM_GETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)0);

	//--- Affichage des données graphiques ---

	if ((ImageList)&&(Item.iImage != -1))
		{
		TextSize.cx = SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)0,(LPARAM)0);
		TextSize.cx -= 2;
		if (TextSize.cx > 2)
			{
			if (TextSize.cx > ImagesSize) TextSize.cx = ImagesSize;
			ImageList_DrawEx(ImageList,Item.iImage,ItemStruct->hDC,ItemStruct->rcItem.left+2,ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top-ImagesSize)/2,TextSize.cx,ImagesSize,CLR_DEFAULT,CLR_DEFAULT,ILD_TRANSPARENT);
			ImagesSize = TextSize.cx;
			}
		else
			ImagesSize = 0;
		}

	if (Item.iImage == -1)
		{
		RECT	TempRect;
		TempRect.left = WorkRect.left;
		TempRect.top = WorkRect.top;
		TempRect.right = WorkRect.left+2+ImagesSize;
		TempRect.bottom = WorkRect.bottom;
		FillRect(ItemStruct->hDC,&TempRect,(HBRUSH)(COLOR_WINDOW+1));
		}

	WorkRect.left += 2+ImagesSize;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)((ItemStruct->itemState&ODS_SELECTED)?(COLOR_HIGHLIGHT+1):(COLOR_WINDOW+1)));

	//--- Affichage du texte ---

	Column.mask = LVCF_WIDTH;
	Column.iSubItem = 0;
	Item.iSubItem = 0;
	Item.pszText = ItemText;
	Item.cchTextMax = 128;
	OldColor = SetTextColor(ItemStruct->hDC,(ItemStruct->itemState&ODS_SELECTED)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
	OldFont = SelectObject(ItemStruct->hDC,Font.hFont);
	WorkRect.left += 2;
	while (Columns--)
		{
		SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&Item);
		SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMN,(WPARAM)Column.iSubItem,(LPARAM)&Column);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&TextSize);
		TextFormat = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_END_ELLIPSIS|DT_MODIFYSTRING;
		WorkRect.right = WorkRect.left+Column.cx-4-ImagesSize;
		if (Alignement&(1<<Column.iSubItem)) TextFormat |= DT_CENTER;
		else TextFormat |= DT_LEFT;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,TextFormat);
		WorkRect.right += 4;
		WorkRect.left = WorkRect.right;
		ImagesSize = 0;
		Column.iSubItem++;
		Item.iSubItem++;
		}
	SelectObject(ItemStruct->hDC,OldFont);
	SetTextColor(ItemStruct->hDC,OldColor);

	return;
}


// «»»» Création de la liste des créatures (TreeView) «««««««««««««««««««»

int Misc_CreateCreaturesTreeList(TREELIST *List)
{
	TV_INSERTSTRUCT	TreeItem;
	HTREEITEM	TreeInserted;
	HTREEITEM	TreeBads;
	HTREEITEM	TreeElites;
	HTREEITEM	TreeHeros;
	int		TreeImage;
	int		i;

	if (List->SelectedItem) *List->SelectedItem = 0;
	TreeImage = 0;
	TreeItem.hParent = TVI_ROOT;
	TreeItem.hInsertAfter = TVI_LAST;
	TreeItem.DUMMYUNIONNAME.item.mask = TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
	TreeItem.DUMMYUNIONNAME.item.cChildren = 1;
	TreeItem.DUMMYUNIONNAME.item.lParam = NULL;

	//--- Groupes de créatures ---

	if (List->Flags&LIST_BADS)
		{
		TreeItem.DUMMYUNIONNAME.item.pszText = szBadCreatures;
		TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
		TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
		TreeBads = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
		if (!TreeBads) return(0);
		}
	else
		TreeImage++;

	if (List->Flags&LIST_ELITES)
		{
		TreeItem.DUMMYUNIONNAME.item.pszText = szBadCreaturesElite;
		TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
		TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
		TreeElites = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
		if (!TreeElites) return(0);
		}
	else
		TreeImage++;

	if (List->Flags&LIST_HEROES)
		{
		TreeItem.DUMMYUNIONNAME.item.pszText = szGoodCreatures;
		TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
		TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
		TreeHeros = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
		if (!TreeHeros) return(0);
		}
	else
		TreeImage++;

	//--- Liste des créatures dans chaque groupe ---

	TreeItem.DUMMYUNIONNAME.item.cChildren = 0;
	if (List->Flags&LIST_BADS)
		{
		TreeItem.hParent = TreeBads;
		for (i = 0; BadCreatures[i].Id != 0; i++)
			{
			TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
			TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
			TreeItem.DUMMYUNIONNAME.item.pszText = BadCreatures[i].Name;
			if (List->Flags&LIST_LPARAMPTR) TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)&BadCreatures[i];
			if (List->Flags&LIST_LPARAMID)  TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)BadCreatures[i].Id;
			TreeInserted = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
			if (!TreeInserted) return(0);
			if ((List->SelectedItem)&&(List->Selected == BadCreatures[i].Id)) *List->SelectedItem = TreeInserted;
			}
		}
	else
		for (i = 0; BadCreatures[i].Id != 0; i++, TreeImage++);

	if (List->Flags&LIST_ELITES)
		{
		TreeItem.hParent = TreeElites;
		for (i = 0; BadElites[i].Id != 0; i++)
			{
			TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
			TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
			TreeItem.DUMMYUNIONNAME.item.pszText = BadElites[i].Name;
			if (List->Flags&LIST_LPARAMPTR) TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)&BadElites[i];
			if (List->Flags&LIST_LPARAMID)  TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)BadElites[i].Id;
			TreeInserted = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
			if (!TreeInserted) return(0);
			if ((List->SelectedItem)&&(List->Selected == BadElites[i].Id)) *List->SelectedItem = TreeInserted;
			}
		}
	else
		for (i = 0; BadElites[i].Id != 0; i++, TreeImage++);

	if (List->Flags&LIST_HEROES)
		{
		TreeItem.hParent = TreeHeros;
		for (i = 0; Heros[i].Id != 0; i++)
			{
			TreeItem.DUMMYUNIONNAME.item.iImage = TreeImage;
			TreeItem.DUMMYUNIONNAME.item.iSelectedImage = TreeImage++;
			TreeItem.DUMMYUNIONNAME.item.pszText = Heros[i].Name;
			if (List->Flags&LIST_LPARAMPTR) TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)&Heros[i];
			if (List->Flags&LIST_LPARAMID)  TreeItem.DUMMYUNIONNAME.item.lParam = (LPARAM)Heros[i].Id;
			TreeInserted = (HTREEITEM)SendMessage(List->hTree,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&TreeItem);
			if (!TreeInserted) return(0);
			if ((List->SelectedItem)&&(List->Selected == Heros[i].Id)) *List->SelectedItem = TreeInserted;
			}
		}

	if (List->Expand&LIST_BADS) SendMessage(List->hTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)TreeBads);
	if (List->Expand&LIST_ELITES) SendMessage(List->hTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)TreeElites);
	if (List->Expand&LIST_HEROES) SendMessage(List->hTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)TreeHeros);

	if ((List->SelectedItem)&&(*List->SelectedItem))
		{
		SendMessage(List->hTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)*List->SelectedItem);
		SendMessage(List->hTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)*List->SelectedItem);
		}

	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
