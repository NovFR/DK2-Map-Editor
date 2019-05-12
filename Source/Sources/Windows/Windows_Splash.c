
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre temporaire affichée au démarrage.
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
extern HWND		hSplash;
extern HDC		hSplashDC;
extern HBITMAP		hSplashOldBitmap;
extern HBITMAP		hSplashBitmap;
extern FONTINFO		SplashFont;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Splash_CreateWindow()
{
	#if (!SPLASH_DISABLED)
	if (Config.HideSplash) return;

	SplashFont.hFont = GetStockObject(DEFAULT_GUI_FONT);
	if (!Misc_GetFontInfo(GetDesktopWindow(),&SplashFont)) return;
	hSplash = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST,szAppName,NULL,WS_POPUP,0,0,327,123+SplashFont.FontHeight+4,NULL,NULL,hInstance,(void *)WINDOW_SPLASH);
	if (hSplash)
		{
		ShowWindow(hSplash,SW_SHOWNORMAL);
		UpdateWindow(hSplash);
		}

	#endif
	return;
}


// «»»» Destruction de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Splash_DestroyWindow()
{
	if (IsWindow(hSplash))
		{
		#if (SPLASH_TIMEOUT)
		Sleep(SPLASH_TIMEOUT);
		#endif
		DestroyWindow(hSplash);
		}
	return;
}


// «»»» Lecture des messages «««««««««««««««««««««««««««««««««««««««««««««««»

LONG Splash_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	HDC		hDC;
	PAINTSTRUCT	paintStruct;

	switch(uMsgId)
		{
		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			hDC = BeginPaint(hWnd,&paintStruct);
			Splash_PaintWindow(hDC);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_DESTROY:
			if (hSplashDC)
				{
				SelectObject(hSplashDC,hSplashOldBitmap);
				DeleteObject(hSplashBitmap);
				DeleteDC(hSplashDC);
				}
			hSplashDC = NULL;
			hSplash = NULL;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}

//--- WM_CREATE -- Création de la fenêtre ---

LONG Splash_Create(HWND hWnd)
{
	HDC	hDC;
	long	X,Y;
	int	Result = -1;

	hSplashBitmap = LoadImage(hInstance,MAKEINTRESOURCE(998),IMAGE_BITMAP,327,123,LR_DEFAULTCOLOR);
	if (hSplashBitmap)
		{
		hDC = GetDC(hWnd);
		if (hDC)
			{
			hSplashDC = CreateCompatibleDC(hDC);
			if (hSplashDC)
				{
				hSplashOldBitmap = SelectObject(hSplashDC,hSplashBitmap);
				if (hSplashOldBitmap) Result = 0;
				}
			}
		}

	if (Result)
		{
		if (hSplashDC) DeleteDC(hSplashDC);
		if (hSplashBitmap) DeleteObject(hSplashBitmap);
		hSplashDC = NULL;
		}
	else
		{
		Window_Center(hWnd,NULL,&X,&Y);
		SetWindowPos(hWnd,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		}

	return(Result);
}

//--- WM_PAINT -- Affichage du contenu de la fenêtre ---

void Splash_PaintWindow(HDC hDC)
{
	RECT		Rect;
	COLORREF	TextColor;
	COLORREF	BackColor;
	HFONT		hOldFont;

	if (!hSplashDC) return;

	BitBlt(hDC,0,0,327,123,hSplashDC,0,0,SRCCOPY);
	Rect.left = 0;
	Rect.right = 327;
	Rect.top = 123;
	Rect.bottom = 123+4+SplashFont.FontHeight;
	FillRect(hDC,&Rect,GetStockObject(BLACK_BRUSH));

	hOldFont = SelectObject(hDC,SplashFont.hFont);
	if (hOldFont)
		{
		TextColor = SetTextColor(hDC,0x00FFFFFF);
		BackColor = SetBkColor(hDC,0x00000000);
		DrawText(hDC,szSplashScreen,-1,&Rect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetTextColor(hDC,TextColor);
		SetBkColor(hDC,BackColor);
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
