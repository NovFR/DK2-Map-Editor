
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des images et des listes d'images.
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialise une liste d'images «««««««««««««««««««««««««««««««««««»

int Misc_CreateImageList(ELEMENT *Array, HIMAGELIST *hImageList, LONG SizeX, LONG SizeY, BOOL Expand)
{
	HBITMAP	hBitmap;
	int	i,j;

	if (!Expand)
		{
		if (*hImageList) Misc_DestroyImageList(hImageList);

		*hImageList = ImageList_Create(SizeX,SizeY,ILC_COLORDDB,1,1);
		if (!*hImageList) return(0);
		}

	for (i = 0; Array[i].Id != 0; i++)
		{
		hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(Array[i].Icon),IMAGE_BITMAP,SizeX,SizeY,LR_DEFAULTCOLOR);
		if (!hBitmap) goto Error_0;
		j = ImageList_Add(*hImageList,hBitmap,NULL);
		DeleteObject(hBitmap);
		if (j == -1) goto Error_0;
		}

	return(1);

Error_0:Misc_DestroyImageList(hImageList);
	return(0);
}


// «»»» Initialise la liste des images utilisées par le zoom ««««««««««««»

int Misc_CreatePlayerImageList(ELEMENT *Array, HIMAGELIST *hImageList, LONG SizeX, LONG SizeY, UINT Player)
{
	HDC	hSrcDC;
	HDC	hDstDC;
	HBITMAP	hSrcOldBitmap;
	HBITMAP	hDstOldBitmap;
	HBITMAP	hBitmap;
	HBITMAP	hSquare;
	int	i,j;

	*hImageList = ImageList_Create(SizeX,SizeY,ILC_COLORDDB,1,1);
	if (!*hImageList) goto Error_0;
	hSquare = LoadImage(hInstance,MAKEINTRESOURCE(Player+931),IMAGE_BITMAP,SizeX,SizeY,LR_DEFAULTCOLOR);
	if (!hSquare) goto Error_0;
	hSrcDC = CreateCompatibleDC(NULL);
	if (!hSrcDC) goto Error_1;
	hDstDC = CreateCompatibleDC(NULL);
	if (!hDstDC) goto Error_2;
	hSrcOldBitmap = SelectObject(hSrcDC,hSquare);
	if (!hSrcOldBitmap) goto Error_3;

	for (i = 0; Array[i].Id != 0; i++)
		{
		hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(Array[i].Icon),IMAGE_BITMAP,SizeX,SizeY,LR_DEFAULTCOLOR);
		if (!hBitmap) goto Error_4;
		hDstOldBitmap = SelectObject(hDstDC,hBitmap);
		if (!hDstOldBitmap) goto Error_4;
		BitBlt(hDstDC,0,0,SizeX,SizeY,hSrcDC,0,0,SRCPAINT);
		SelectObject(hDstDC,hDstOldBitmap);
		j = ImageList_Add(*hImageList,hBitmap,NULL);
		DeleteObject(hBitmap);
		if (j == -1) goto Error_4;
		}

	SelectObject(hSrcDC,hSrcOldBitmap);
	DeleteDC(hDstDC);
	DeleteDC(hSrcDC);
	DeleteObject(hSquare);
	return(1);

Error_4:SelectObject(hSrcDC,hSrcOldBitmap);
Error_3:DeleteDC(hDstDC);
Error_2:DeleteDC(hSrcDC);
Error_1:DeleteObject(hSquare);
Error_0:Misc_DestroyImageList(hImageList);
	return(0);
}



// «»»» Supprime une liste d'images «««««««««««««««««««««««««««««««««««««»

void Misc_DestroyImageList(HIMAGELIST *hImageList)
{
	if (!*hImageList) return;
	ImageList_Destroy(*hImageList);
	*hImageList = (HIMAGELIST)NULL;
	return;
}


// «»»» Initialise une image ««««««««««««««««««««««««««««««««««««««««««««»

int Misc_CreateImage(HBITMAP *hBitmap, UINT ResID, LONG Width, LONG Height)
{
	if (*hBitmap) Misc_DestroyImage(hBitmap);

	*hBitmap = LoadImage(hInstance,MAKEINTRESOURCE(ResID),IMAGE_BITMAP,Width,Height,LR_DEFAULTCOLOR);
	if (!*hBitmap) return(0);

	return(1);
}


// «»»» Supprime une image ««««««««««««««««««««««««««««««««««««««««««««««»

void Misc_DestroyImage(HBITMAP *hBitmap)
{
	if (!*hBitmap) return;
	DeleteObject(*hBitmap);
	*hBitmap = NULL;
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
