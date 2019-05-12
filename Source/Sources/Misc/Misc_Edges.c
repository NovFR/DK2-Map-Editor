
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'affichage des cadres.
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

//--- Affichage du cadre à partir d'une structure RECT ---

void Misc_DrawBevelBoxRect(HWND dbbWnd, HDC dbbDC, RECT *dbbRect, DWORD dbbType)
{
	Misc_DrawBevelBox(dbbWnd,dbbDC,dbbRect->left,dbbRect->top,dbbRect->right-dbbRect->left-1,dbbRect->bottom-dbbRect->top-1,dbbType);
	return;
}

//--- Affichage du cadre ---

void Misc_DrawBevelBox(HWND dbbWnd, HDC dbbDC, LONG X, LONG Y, LONG Width, LONG Height, DWORD dbbType)
{
	HDC	hDC;
	UINT	Type = dbbType&0x7FFFFFFF;
	DWORD	Raised = dbbType&0x80000000;

	hDC = dbbDC;

	if (dbbDC == 0)
		{
		if (dbbWnd != 0)
			hDC = GetDC(dbbWnd);
		else
			return;
		}

	if (Type == 1)
		Misc_DrawClassical3DBoxBold(hDC,X,Y,Width,Height,Raised);
	else
		Misc_DrawClassical3DBox(hDC,X,Y,Width,Height,Raised,Type);

	if (dbbDC == 0)
		ReleaseDC(dbbWnd,hDC);

	return;
}

//--- Cadre 2 pixels ---

void Misc_DrawClassical3DBoxBold(HDC hDC, LONG X, LONG Y, LONG Width, LONG Height, DWORD Raised)
{
	Misc_DrawClassical3DBox(hDC,X,Y,Width,Height,Raised,1);
	Misc_DrawClassical3DBox(hDC,X+1,Y+1,Width-2,Height-2,Raised,0);
	return;
}

//--- Cadres classiques ---

void Misc_DrawClassical3DBox(HDC hDC, LONG X, LONG Y, LONG Width, LONG Height, DWORD Raised, UINT ColorMode)
{
	HPEN OldPen,BlackPen,WhitePen;

	switch(ColorMode)
		{
		case 0:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DLIGHT));
			break;
		case 1:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
			break;
		case 2:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
			break;
		case 3:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DLIGHT));
			break;
		case 4:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
			break;
		case 5:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
			break;
		case 6:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHIGHLIGHT));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHIGHLIGHT));
			break;
		case 7:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DFACE));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DFACE));
			break;
		case 8:
			BlackPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
			WhitePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
			break;
		}

	if (WhitePen)
		{
		if (BlackPen)
			{
			if (Raised != 0)
				{
				OldPen = (HPEN)SelectObject(hDC,BlackPen);
				MoveToEx(hDC,X+Width,Y,NULL);
				LineTo(hDC,X,Y);
				LineTo(hDC,X,Y+Height);
				SelectObject(hDC,WhitePen);
				LineTo(hDC,X+Width,Y+Height);
				LineTo(hDC,X+Width,Y);
				}
			else
				{
				OldPen = (HPEN)SelectObject(hDC,WhitePen);
				MoveToEx(hDC,X+Width,Y,NULL);
				LineTo(hDC,X,Y);
				LineTo(hDC,X,Y+Height+1);
				SelectObject(hDC,BlackPen);
				MoveToEx(hDC,X+1,Y+Height,NULL);
				LineTo(hDC,X+Width,Y+Height);
				LineTo(hDC,X+Width,Y);
				}
			SelectObject(hDC,OldPen);
			DeleteObject(BlackPen);
			}
		DeleteObject(WhitePen);
		}
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
