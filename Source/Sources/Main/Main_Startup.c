
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de démarrage.
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
extern HINSTANCE	hDirectSound;
extern HMENU		hMenu;
extern HWND		hWnd;
extern HANDLE		AppMutex;
extern ULONG		InfoMemoryUsedByUndo;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
int APIENTRY WinMain(HINSTANCE hWinCInstance, HINSTANCE hWinPInstance, LPSTR CmdLine, int CmdShow)
{
	MSG	msg;

	hInstance = hWinCInstance;

	if (!Initialise_GetVersion())
		goto Init_Failure0;
	if (!Initialise_GetDeviceCaps())
		goto Init_Failure0;
	if (!Initialise_LockInstance())
		goto Init_Failure0;
	if (!Initialise_WindowsClasses())
		goto Init_Failure1;
	if (!Keyboard_InitialiseHook())
		goto Init_Failure1;
	if (!Initialise_Window())
		goto Init_Failure2;

	while (GetMessage(&msg,NULL,0,0))
		{
		Minimap_Messages(&msg);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	if (InfoMemoryUsedByUndo != 0)
		Misc_MessageBoxExt(NULL,szGlobalAllocErr,szInternalError,szMessageBoxExtOk,MB_ICONHAND);

	Keyboard_ResetHook();
	#if (LOCK_MULTIPLERUNS)
	CloseHandle(AppMutex);
	#endif
	if (hDirectSound) FreeLibrary(hDirectSound);
	return(msg.wParam);

//--- Erreurs d'initialisation ---

Init_Failure2:
	Keyboard_ResetHook();
Init_Failure1:
	#if (LOCK_MULTIPLERUNS)
	CloseHandle(AppMutex);
	#endif
Init_Failure0:
	return(0);
}


// «»»» Test les caractéristiques du système ««««««««««««««««««««««««««««»

//--- Test la version du système d'exploitation ---

int Initialise_GetVersion()
{
	OSVERSIONINFO osVer;

	osVer.dwOSVersionInfoSize = sizeof(osVer);
	if (GetVersionEx(&osVer) == 0)
		{
		MessageBox(NULL,szVersionErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}
	if (osVer.dwPlatformId == VER_PLATFORM_WIN32s)
		{
		MessageBox(NULL,szWinVersionErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	return(1);
}

//--- Test les caractéristiques de la carte graphique ---

int Initialise_GetDeviceCaps()
{
	HWND	hWnd;
	HDC	hDC;
	int	DeviceCaps;

	hWnd = GetDesktopWindow();
	hDC = GetWindowDC(hWnd);
	DeviceCaps = GetDeviceCaps(hDC,RASTERCAPS);
	ReleaseDC(hWnd,hDC);

	if ( !(DeviceCaps&(RC_BITBLT|RC_DI_BITMAP|RC_STRETCHBLT|RC_PALETTE)) )
		{
		MessageBox(NULL,szMissingCapsErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	return(1);
}


// «»»» Empèche le lancement de plusieurs instances «««««««««««««««««««««»

int Initialise_LockInstance()
{
	#if (LOCK_MULTIPLERUNS)
	AppMutex = CreateMutex(NULL,FALSE,szAppName);
	if (!AppMutex)
		{
		MessageBox(NULL,szMutexErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
		CloseHandle(AppMutex);
		MessageBox(NULL,szAlreadyExistsErr,szInitErr,MB_OK|MB_ICONINFORMATION);
		return(0);
		}
	#endif
	return(1);
}


// «»»» Initialisation des classes de fenêtres ««««««««««««««««««««««««««»

int Initialise_WindowsClasses()
{
	WNDCLASSEXA wndClass;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = Window_Proc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadImage(hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szAppName;
	wndClass.hIconSm = wndClass.hIcon;
	if (RegisterClassEx(&wndClass) == 0)
		{
		MessageBox(NULL,szWindowErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC|CS_NOCLOSE;
	wndClass.lpfnWndProc = Disk_ProcessMessages;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadImage(hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szDiskClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (RegisterClassEx(&wndClass) == 0)
		{
		MessageBox(NULL,szWindowErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_DBLCLKS|CS_SAVEBITS|CS_BYTEALIGNWINDOW;
	wndClass.lpfnWndProc = DefDlgProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = DLGWINDOWEXTRA;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadImage(hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szDialogClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (RegisterClassEx(&wndClass) == 0)
		{
		MessageBox(NULL,szWindowErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_DBLCLKS|CS_SAVEBITS|CS_BYTEALIGNWINDOW|CS_NOCLOSE;
	wndClass.lpfnWndProc = DefDlgProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = DLGWINDOWEXTRA;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadImage(hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szMessageClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (RegisterClassEx(&wndClass) == 0)
		{
		MessageBox(NULL,szWindowErr,szInitErr,MB_OK|MB_ICONHAND);
		return(0);
		}

	InitCommonControls();
	return(1);
}


// «»»» Initialisation de la fenêtre principale «««««««««««««««««««««««««»

int Initialise_Window(void)
{
	hMenu = LoadMenu(hInstance,"MAINMENU");
	if (!hMenu)
		{
		Misc_PrintError(NULL,szWindowMenuErr,szInitErr,MB_ICONHAND);
		return(0);
		}
	Misc_InitialiseMenus(hMenu,MainMenu);

	hWnd = CreateWindowEx(0,szAppName,szWindowTitle,WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,0,0,640,480,NULL,hMenu,hInstance,(void *)WINDOW_MAIN);
	if (!hWnd)
		{
		if (hMenu)
			{
			DestroyMenu(hMenu);
			Misc_ResetMenus(MainMenu);
			}
		if (GetLastError())
			Misc_PrintError(NULL,szWindowErr,szInitErr,MB_ICONHAND);
		return(0);
		}
	else
		{
		#if (START_MAXIMIZED)
		ShowWindow(hWnd,SW_MAXIMIZE);
		#else
		ShowWindow(hWnd,SW_SHOWNORMAL);
		#endif
		UpdateWindow(hWnd);
		}

	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
