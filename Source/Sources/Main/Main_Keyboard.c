
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des raccourcis clavier.
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

extern HHOOK		KeyHook;
extern HWND		hWnd;
extern HWND		hMapVScroll;
extern HWND		hMapHScroll;
extern MAPTHING		MapImmCreature;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'initialisation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation de la gestion des raccourcis «««««««««««««««««««««»

int Keyboard_InitialiseHook()
{
	KeyHook = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)Keyboard_Hook,NULL,GetCurrentThreadId());
	if (!KeyHook)
		{
		Misc_PrintError(NULL,szHookErr,szInitErr,MB_ICONHAND);
		return(0);
		}
	return(1);
}


// «»»» Suppression du gestionnaire «««««««««««««««««««««««««««««««««««««»

void Keyboard_ResetHook()
{
	if (KeyHook) UnhookWindowsHookEx(KeyHook);
	KeyHook = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages du clavier					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG CALLBACK Keyboard_Hook(int Code, WPARAM wParam, LPARAM lParam)
{
	static BOOL CtrlDown;
	static BOOL ShiftDown;

	if (Code < 0)
		goto Dont_Filter;

	switch(wParam)
		{
		case VK_CONTROL:
			if (lParam&0x80000000)
				CtrlDown = FALSE;
			else
				CtrlDown = TRUE;
			break;
		case VK_SHIFT:
			if (lParam&0x80000000)
				ShiftDown = FALSE;
			else
				ShiftDown = TRUE;
			break;
		}

	if (!IsWindow(hWnd)||(GetActiveWindow() != hWnd)||(!IsWindowEnabled(hWnd)))
		goto Dont_Filter;
	if (lParam&0x80000000)
		goto Dont_Filter;

	if (Keyboard_ProcessSpecialKeys(wParam,ShiftDown,CtrlDown))
		return(1);

	if (Keyboard_ProcessShortCut(wParam,ShiftDown,CtrlDown))
		return(1);

Dont_Filter:
	return(CallNextHookEx(KeyHook,Code,wParam,lParam));
}


// «»»» Appel des fonctions associées à des touches spéciales (fixes) «««»

int Keyboard_ProcessSpecialKeys(int Key, BOOL ShiftDown, BOOL CtrlDown)
{
	UINT	Message;
	WORD	wParam;
	ULONG	lParam;

	if ((ShiftDown)&&(CtrlDown))
		return(0);

	switch(Key)
		{
		case VK_INSERT:
			if (ShiftDown) Things_SelectOptions(hWnd,&MapImmCreature.creaturesoptions,&MapImmCreature.herosoptions,THING_OPTIONSCREATURES);
			else if (CtrlDown) Things_SelectOptions(hWnd,&MapImmCreature.creaturesoptions,&MapImmCreature.herosoptions,THING_OPTIONSHEROS);
			else Things_SelectObjective(hWnd,&MapImmCreature.objective,&MapImmCreature.area,&MapImmCreature.ennemy);
			return(1);
		case VK_DELETE:
			lParam = THING_CREATURE;
			if (ShiftDown) lParam = THING_DOOR;
			if (CtrlDown) lParam = THING_OBJECT;
			SendMessage(hWnd,WM_KILLTHING,(WPARAM)0,(LPARAM)lParam);
			return(1);
		case VK_UP:
			Message = WM_VSCROLL;
			wParam = SB_LINEUP;
			lParam = (LPARAM)hMapVScroll;
			if (ShiftDown) wParam = SB_PAGEUP;
			else if (CtrlDown) wParam = SB_TOP;
			break;
		case VK_DOWN:
			Message = WM_VSCROLL;
			wParam = SB_LINEDOWN;
			lParam = (LPARAM)hMapVScroll;
			if (ShiftDown) wParam = SB_PAGEDOWN;
			else if (CtrlDown) wParam = SB_BOTTOM;
			break;
		case VK_LEFT:
			Message = WM_HSCROLL;
			wParam = SB_LINELEFT;
			lParam = (LPARAM)hMapHScroll;
			if (ShiftDown) wParam = SB_PAGELEFT;
			else if (CtrlDown) wParam = SB_TOP;
			break;
		case VK_RIGHT:
			Message = WM_HSCROLL;
			wParam = SB_LINERIGHT;
			lParam = (LPARAM)hMapHScroll;
			if (ShiftDown) wParam = SB_PAGERIGHT;
			else if (CtrlDown) wParam = SB_BOTTOM;
			break;
		case VK_PRIOR:
			Message = WM_VSCROLL;
			wParam = SB_PAGEUP;
			lParam = (LPARAM)hMapVScroll;
			break;
		case VK_NEXT:
			Message = WM_VSCROLL;
			wParam = SB_PAGEDOWN;
			lParam = (LPARAM)hMapVScroll;
			break;
		case VK_HOME:
			wParam = SB_TOP;
			if (CtrlDown)
				{
				Message = WM_VSCROLL;
				lParam = (LPARAM)hMapVScroll;
				}
			else
				{
				Message = WM_HSCROLL;
				lParam = (LPARAM)hMapHScroll;
				}
			break;
		case VK_END:
			wParam = SB_BOTTOM;
			if (CtrlDown)
				{
				Message = WM_VSCROLL;
				lParam = (LPARAM)hMapVScroll;
				}
			else
				{
				Message = WM_HSCROLL;
				lParam = (LPARAM)hMapHScroll;
				}
			break;

		default:return(0);
		}

	SendMessage(hWnd,Message,MAKELPARAM(wParam,0),lParam);
	return(1);
}


// «»»» Appel des fonctions associées aux raccourcis ««««««««««««««««««««»

int Keyboard_ProcessShortCut(int Key, BOOL ShiftDown, BOOL CtrlDown)
{
	BOOL	Continue;
	BOOL	Shift,Ctrl;
	int	ShortCutKey;
	int	i = 0;

	do {

		Continue = MainMenu[i].ciNext;
		if (MainMenu[i].ciKey)
			{
			Keyboard_GetShortCut(MainMenu[i].ciKey,&ShortCutKey,&Shift,&Ctrl);
			if ((Key == ShortCutKey)&&(Shift == ShiftDown)&&(Ctrl == CtrlDown))
				{
				SendMessage(hWnd,WM_COMMAND,(WPARAM)MainMenu[i].ciId,(LPARAM)hWnd);
				return(1);
				}
			}

		i++;

	} while(Continue);

	return(0);
}


// «»»» Transcription d'une chaîne de caractère en code clavier «««««««««»

void Keyboard_GetShortCut(char *Text, int *Key, BOOL *Shift, BOOL *Ctrl)
{
	char	 Temp[16];
	char	*ShortCut;
	char	*Space;
	int	 i,j;

	ShortCut = Text;
	*Shift = 0;
	*Ctrl = 0;

	//--- Test si une touche de fonction est spécifiée ---

	for (j = 0; j != 2; j++)
		{
		Space = strchr(ShortCut,'+');
		if (Space)
			{
			for (i = 0; *ShortCut != ' '; i++)
				Temp[i] = *ShortCut++;
			Temp[i] = 0;
			if (!strcmpi(Temp,szShift)) *Shift = 1;
			else if (!strcmpi(Temp,szCtrl)) *Ctrl = 1;
			ShortCut = Space+1;
			while(*ShortCut++ == ' ');
			ShortCut--;
			}
		}

	//--- Traduit la touche ---

	if (strlen(ShortCut) > 1)
		{
		if (!strcmpi(ShortCut,szF1)) *Key = VK_F1;
		else if (!strcmpi(ShortCut,szF2)) *Key = VK_F2;
		else if (!strcmpi(ShortCut,szF3)) *Key = VK_F3;
		else if (!strcmpi(ShortCut,szF4)) *Key = VK_F4;
		else if (!strcmpi(ShortCut,szF5)) *Key = VK_F5;
		else if (!strcmpi(ShortCut,szF6)) *Key = VK_F6;
		else if (!strcmpi(ShortCut,szF7)) *Key = VK_F7;
		else if (!strcmpi(ShortCut,szF8)) *Key = VK_F8;
		else if (!strcmpi(ShortCut,szF9)) *Key = VK_F9;
		else if (!strcmpi(ShortCut,szF10)) *Key = VK_F10;
		else if (!strcmpi(ShortCut,szF11)) *Key = VK_F11;
		else if (!strcmpi(ShortCut,szF12)) *Key = VK_F12;
		else if (!strcmpi(ShortCut,szReturn)) *Key = VK_RETURN;
		else if (!strcmpi(ShortCut,szDel)) *Key = VK_DELETE;
		else *Key = 0;
		}
	else
		*Key = *ShortCut;

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
