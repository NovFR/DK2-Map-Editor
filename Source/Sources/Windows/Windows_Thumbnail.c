
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion du générateur de réductions.
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
extern HWND		hThumbWnd;
extern HWND		hThumbFlagsWnd;
extern HWND		hThumbRefreshWnd;
extern HWND		hThumbSaveWnd;
extern HWND		hThumbSizeWnd;
extern HBITMAP		hThumbBck;
extern HBITMAP		hThumbBmp;
extern LONG		ThumbSize;
extern FONTINFO		Font;
extern MAP		Map;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Thumb_CreateWindow(HWND hWnd)
{
	long	X,Y;
	long	Width,Height;

	if (hThumbWnd)
		{
		SetWindowPos(hThumbWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
		SetActiveWindow(hThumbWnd);
		return;
		}

	Width = 154+GetSystemMetrics(SM_CXSIZEFRAME)*2+150;
	Height = 152+GetSystemMetrics(SM_CYSMCAPTION)+GetSystemMetrics(SM_CYSIZEFRAME)*2;
	if (Config.ThumbnailWindowX == CW_USEDEFAULT)
		{
		RECT	ParentRect;

		GetWindowRect(hWnd,&ParentRect);
		X = ParentRect.right-Width-12;
		Y = ParentRect.top+GetSystemMetrics(SM_CYCAPTION);
		if (X < 0) X = 0;
		}
	else
		{
		X = Config.ThumbnailWindowX;
		Y = Config.ThumbnailWindowY;
		}
	hThumbWnd = CreateWindowEx(WS_EX_TOOLWINDOW,szAppName,szThumbTitle,WS_POPUP|WS_BORDER|WS_CAPTION|WS_SYSMENU,X,Y,Width,Height,hWnd,NULL,hInstance,(void *)WINDOW_THUMBNAIL);
	if (!hThumbWnd)
		{
		Misc_PrintError(hWnd,szWindowErr,NULL,MB_ICONHAND);
		return;
		}

	ShowWindow(hThumbWnd,SW_SHOWNORMAL);
	UpdateWindow(hThumbWnd);
	return;
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Thumb_DestroyWindow()
{
	if (hThumbWnd) DestroyWindow(hThumbWnd);
	return;
}


// «»»» Mise à jour de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Thumb_UpdateWindow()
{
	HDC	hDC;

	if (!hThumbWnd) return;
	if (!hThumbBmp) return;

	SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
	DeleteObject(hThumbBmp);
	hThumbBmp = NULL;

	hDC = GetDC(hWnd);
	if (hDC)
		hThumbBmp = CreateCompatibleBitmap(hDC,Map.Properties.Width,Map.Properties.Height);
	else
		{
		SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
		InvalidateRect(hThumbWnd,NULL,FALSE);
		UpdateWindow(hThumbWnd);
		return;
		}
	ReleaseDC(hWnd,hDC);

	if (hThumbBmp) Thumbnail_GenerateExt(hThumbWnd,&Map,hThumbBmp,&Config.Thumbnail);
	SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
	InvalidateRect(hThumbWnd,NULL,FALSE);
	UpdateWindow(hThumbWnd);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages provenants de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG Thumb_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	HDC		hDC;
	PAINTSTRUCT	paintStruct;

	switch(uMsgId)
		{
		case WM_MOVE:
			Config.ThumbnailWindowX = LOWORD(lParam);
			Config.ThumbnailWindowY = HIWORD(lParam);
			break;

		case WM_ERASEBKGND:
			if (!GetUpdateRect(hWnd,&paintStruct.rcPaint,FALSE)) break;
			FillRect((HDC)wParam,&paintStruct.rcPaint,(HBRUSH)(COLOR_MENU+1));
			return(1);

		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			hDC = BeginPaint(hWnd,&paintStruct);
			Thumb_PaintWindow(hDC);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					if ((HWND)lParam == hThumbFlagsWnd)
						{
						Thumb_DisplayFlags(hWnd);
						break;
						}
					if ((HWND)lParam == hThumbRefreshWnd)
						{
						if (!hThumbBmp) break;
						SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
						Thumbnail_GenerateExt(hWnd,&Map,hThumbBmp,&Config.Thumbnail);
						SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
						InvalidateRect(hWnd,NULL,FALSE);
						UpdateWindow(hWnd);
						break;
						}
					if ((HWND)lParam == hThumbSaveWnd)
						{
						if (!hThumbBmp) break;
						Thumbnail_GenerateFile(hWnd,hThumbBmp,hThumbBck,ThumbSize);
						break;
						}
					break;
				case CBN_SELCHANGE:
					if ((HWND)lParam == hThumbSizeWnd)
						{
						ThumbSize = (SendMessage(hThumbSizeWnd,CB_GETCURSEL,(WPARAM)0,(LPARAM)0) == 0)?144:96;
						InvalidateRect(hWnd,NULL,FALSE);
						UpdateWindow(hWnd);
						break;
						}
					break;
				}
			break;

		case WM_DESTROY:
			if (hThumbBmp) DeleteObject(hThumbBmp);
			if (hThumbBck) DeleteObject(hThumbBck);
			hThumbWnd = NULL;
			hThumbBmp = NULL;
			hThumbBck = NULL;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Initialisation de la fenêtre ««««««««««««««««««««««««««««««««««««»

LONG Thumb_Create(HWND hWnd)
{
	HDC	hDC;
	RECT	ClientRect;
	long	FrameSizeX,FrameSizeY;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		hThumbBmp = CreateCompatibleBitmap(hDC,Map.Properties.Width,Map.Properties.Height);
		if (!hThumbBmp)
			{
			ReleaseDC(hWnd,hDC);
			return(-1);
			}
		else
			{
			if (!Thumbnail_GenerateExt(hWnd,&Map,hThumbBmp,&Config.Thumbnail))
				{
				ReleaseDC(hWnd,hDC);
				return(-1);
      				}
			}
		ReleaseDC(hWnd,hDC);
		}

	hThumbBck = LoadImage(hInstance,MAKEINTRESOURCE(996),IMAGE_BITMAP,144,144,LR_DEFAULTCOLOR);
	GetClientRect(hWnd,&ClientRect);
	FrameSizeX = GetSystemMetrics(SM_CXSIZEFRAME);
	FrameSizeY = GetSystemMetrics(SM_CYSIZEFRAME);

	hThumbSizeWnd	 = CreateWindow(szComboBoxClass,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|CBS_DROPDOWNLIST|CBS_HASSTRINGS,158,4,ClientRect.right-158-FrameSizeX,100,hWnd,NULL,hInstance,NULL);
	if (!hThumbSizeWnd) return(-1);
	hThumbFlagsWnd	 = CreateWindow(szButtonClass,szThumbFlags,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,158,ClientRect.bottom-28*2-44-12-FrameSizeY,ClientRect.right-158-FrameSizeX,28,hWnd,NULL,hInstance,NULL);
	if (!hThumbFlagsWnd) return(-1);
	hThumbRefreshWnd = CreateWindow(szButtonClass,szThumbRefresh,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,158,ClientRect.bottom-28*2-44-12+32-FrameSizeY,ClientRect.right-158-FrameSizeX,28,hWnd,NULL,hInstance,NULL);
	if (!hThumbRefreshWnd) return(-1);
	hThumbSaveWnd	 = CreateWindow(szButtonClass,szThumbSave,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|BS_PUSHBUTTON,158,ClientRect.bottom-28*2-44-12+32*2+6-FrameSizeY,ClientRect.right-158-FrameSizeX,28,hWnd,NULL,hInstance,NULL);
	if (!hThumbSaveWnd) return(-1);

	SendMessage(hThumbSizeWnd,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(hThumbFlagsWnd,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(hThumbRefreshWnd,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(hThumbSaveWnd,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(hThumbSizeWnd,CB_ADDSTRING,(WPARAM)0,(LPARAM)szThumbSize144);
	SendMessage(hThumbSizeWnd,CB_ADDSTRING,(WPARAM)0,(LPARAM)szThumbSize96);
	SendMessage(hThumbSizeWnd,CB_SETCURSEL,(WPARAM)((ThumbSize == 144)?0:1),(LPARAM)0);
	return(0);
}


// «»»» Affichage de la réduction «««««««««««««««««««««««««««««««««««««««»

void Thumb_PaintWindow(HDC hDC)
{
	RECT	WorkRect;
	HDC	hWorkDC;
	HBITMAP	hWorkBmp;

	WorkRect.left = 2;
	WorkRect.top = 2;
	WorkRect.right = 150;
	WorkRect.bottom = 150;
	DrawFrameControl(hDC,&WorkRect,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);

	if (!hThumbBmp) return;

	hWorkDC = CreateCompatibleDC(hDC);
	if (!hWorkDC) return;
	hWorkBmp = SelectObject(hWorkDC,hThumbBmp);
	if (hWorkBmp)
		{
		Thumbnail_CopyBitmap(hDC,(144-ThumbSize)/2+4,(144-ThumbSize)/2+4,ThumbSize,hWorkDC,Map.Properties.Width,Map.Properties.Height,hThumbBck,Config.Thumbnail.UseColorKey,Config.Thumbnail.HiddenColor);
		SelectObject(hWorkDC,hWorkBmp);
		}
	DeleteDC(hWorkDC);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Options d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ouverture des options «««««««««««««««««««««««««««««««««««««««««««»

void Thumb_DisplayFlags(HWND hWnd)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(4000),hWnd,Thumb_DisplayProc,(LPARAM)hWnd);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Thumb_DisplayProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	static DWORD	WorldFlags[]={	TOF_SHOWLIMIT,
					TOF_SHOWWATER,
					TOF_SHOWLAVA,
					TOF_SHOWGOLD,
					TOF_SHOWGEMS,
					TOF_SHOWSOLID,
					TOF_SHOWROCK,
					TOF_SHOWUNCLAIMED,
					TOF_SHOWHEROESREST};
	static DWORD	PlyFlags[][7]={{TOF_SHOWHEROESWALLS,   TOF_SHOWNEUTRALWALLS,   TOF_SHOWKEEPER1WALLS,   TOF_SHOWKEEPER2WALLS,   TOF_SHOWKEEPER3WALLS,   TOF_SHOWKEEPER4WALLS,   TOF_SHOWKEEPER5WALLS},
				       {TOF_SHOWHEROESROOMS,   TOF_SHOWNEUTRALROOMS,   TOF_SHOWKEEPER1ROOMS,   TOF_SHOWKEEPER2ROOMS,   TOF_SHOWKEEPER3ROOMS,   TOF_SHOWKEEPER4ROOMS,   TOF_SHOWKEEPER5ROOMS},
				       {TOF_SHOWHEROESPORTALS, TOF_SHOWNEUTRALPORTALS, TOF_SHOWKEEPER1PORTALS, TOF_SHOWKEEPER2PORTALS, TOF_SHOWKEEPER3PORTALS, TOF_SHOWKEEPER4PORTALS, TOF_SHOWKEEPER5PORTALS},
				       {TOF_SHOWHEROESHEARTS,  TOF_SHOWNEUTRALHEARTS,  TOF_SHOWKEEPER1HEART,   TOF_SHOWKEEPER2HEART,   TOF_SHOWKEEPER3HEART,   TOF_SHOWKEEPER4HEART,   TOF_SHOWKEEPER5HEART}};
	LONG		lData;
	HWND		hParentWnd;

	if (uMsgId == WM_INITDIALOG)
		{
		RECT	WindowRect;
		RECT	DialogRect;
		LONG	X,Y;

		SetWindowLong(hDlg,DWL_USER,(LONG)lParam);
		GetWindowRect((HWND)lParam,&WindowRect);
		GetWindowRect(hDlg,&DialogRect);
		X = WindowRect.left-(DialogRect.right-DialogRect.left)-2;
		Y = WindowRect.top;
		if (X < 0) X = WindowRect.right+2;
		Window_CorrectPos(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);

		for (lData = 100; lData != 108+1; lData++)
			CheckDlgButton(hDlg,lData,(Config.Thumbnail.WorldFlags&WorldFlags[lData-100])?BST_CHECKED:BST_UNCHECKED);

		if (!Misc_CreatePlayerComboBoxList(hDlg,190,1,FALSE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		CheckDlgButton(hDlg,302,(Config.Thumbnail.UseColorKey)?BST_CHECKED:BST_UNCHECKED);
		SendDlgItemMessage(hDlg,190,CB_SETCURSEL,(WPARAM)Config.Thumbnail.ActualPlayer,(LPARAM)0);
		Thumb_DisplaySetPlayerOptions(hDlg);
		return(TRUE);
		}

	hParentWnd = (HWND)GetWindowLong(hDlg,DWL_USER);
	if (!hParentWnd) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_DisplayPlayerComboBoxList(((MEASUREITEMSTRUCT *)lParam)->CtlID,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				RectEx_DrawColoredButton((DRAWITEMSTRUCT *)lParam,Config.Thumbnail.HiddenColor);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					Config.Thumbnail.ActualPlayer = SendDlgItemMessage(hDlg,190,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
					Thumb_DisplaySetPlayerOptions(hDlg);
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 100:
						case 101:
						case 102:
						case 103:
						case 104:
						case 105:
						case 106:
						case 107:
						case 108:
							Config.Thumbnail.WorldFlags &= ~WorldFlags[LOWORD(wParam)-100];
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) Config.Thumbnail.WorldFlags |= WorldFlags[LOWORD(wParam)-100];
							if (!hThumbBmp) break;
							SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
							Thumbnail_GenerateExt(hParentWnd,&Map,hThumbBmp,&Config.Thumbnail);
							SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
							InvalidateRect(hParentWnd,NULL,FALSE);
							UpdateWindow(hParentWnd);
							return(TRUE);

						case 200:
						case 201:
						case 202:
						case 203:
							Config.Thumbnail.PlayersFlags &= ~PlyFlags[LOWORD(wParam)-200][Config.Thumbnail.ActualPlayer];
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) Config.Thumbnail.PlayersFlags |= PlyFlags[LOWORD(wParam)-200][Config.Thumbnail.ActualPlayer];
							if (!hThumbBmp) break;
							SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
							Thumbnail_GenerateExt(hParentWnd,&Map,hThumbBmp,&Config.Thumbnail);
							SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
							InvalidateRect(hParentWnd,NULL,FALSE);
							UpdateWindow(hParentWnd);
							return(TRUE);

						case 301: {
							CHOOSECOLOR	ColorSelector;
							COLORREF	CustomColors[16];

							ColorSelector.lStructSize = sizeof(CHOOSECOLOR);
							ColorSelector.hwndOwner = hDlg;
							ColorSelector.rgbResult = Config.Thumbnail.HiddenColor;
							ColorSelector.lpCustColors = CustomColors;
							ColorSelector.Flags = CC_FULLOPEN|CC_RGBINIT;
							ColorSelector.lpfnHook = NULL;
							CustomColors[0] = Config.Thumbnail.HiddenColor;
							CustomColors[1] = THUMB_WATER;
							CustomColors[2] = THUMB_LAVA;
							CustomColors[3] = THUMB_GOLD;
							CustomColors[4] = THUMB_GEMS;
							CustomColors[5] = THUMB_SOLID;
							CustomColors[6] = THUMB_ROCK;
							CustomColors[7] = THUMB_UNCLAIMED;
							CustomColors[8] = THUMB_NEUTRAL;
							CustomColors[9] = THUMB_HERO;
							CustomColors[10] = THUMB_KEEPER1;
							CustomColors[11] = THUMB_KEEPER2;
							CustomColors[12] = THUMB_KEEPER3;
							CustomColors[13] = THUMB_KEEPER4;
							CustomColors[14] = THUMB_KEEPER5;
							CustomColors[15] = THUMB_NEUTRALPORTAL;
							if (ChooseColor(&ColorSelector))
								{
								Config.Thumbnail.HiddenColor = ColorSelector.rgbResult;
								InvalidateRect(GetDlgItem(hDlg,300),NULL,FALSE);
								UpdateWindow(GetDlgItem(hDlg,300));
								if (!hThumbBmp) break;
								SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)FALSE,(LPARAM)0);
								Thumbnail_GenerateExt(hParentWnd,&Map,hThumbBmp,&Config.Thumbnail);
								SendMessage(hThumbWnd,WM_SETREDRAW,(WPARAM)TRUE,(LPARAM)0);
								InvalidateRect(hParentWnd,NULL,FALSE);
								UpdateWindow(hParentWnd);
								}
							} return(TRUE);

						case 302:
							Config.Thumbnail.UseColorKey = (IsDlgButtonChecked(hDlg,302) == BST_CHECKED)?TRUE:FALSE;
							InvalidateRect(hParentWnd,NULL,FALSE);
							UpdateWindow(hParentWnd);
							return(TRUE);

						case IDOK:
						case IDCANCEL:
							EndDialog(hDlg,0);
							return(TRUE);
						}
					break;
				}
			break;
		}

	return(FALSE);
}


// «»»» Actualise les options des joueurs «««««««««««««««««««««««««««««««»

void Thumb_DisplaySetPlayerOptions(HWND hDlg)
{
	switch(Config.Thumbnail.ActualPlayer)
		{
		case 0:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWHEROESWALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWHEROESROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWHEROESPORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWHEROESHEARTS)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 1:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWNEUTRALWALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWNEUTRALROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWNEUTRALPORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWNEUTRALHEARTS)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 2:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER1WALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER1ROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER1PORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER1HEART)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 3:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER2WALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER2ROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER2PORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER2HEART)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 4:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER3WALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER3ROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER3PORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER3HEART)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 5:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER4WALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER4ROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER4PORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER4HEART)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 6:	CheckDlgButton(hDlg,200,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER5WALLS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,201,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER5ROOMS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,202,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER5PORTALS)?BST_CHECKED:BST_UNCHECKED);
			CheckDlgButton(hDlg,203,(Config.Thumbnail.PlayersFlags&TOF_SHOWKEEPER5HEART)?BST_CHECKED:BST_UNCHECKED);
			break;
		}
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
