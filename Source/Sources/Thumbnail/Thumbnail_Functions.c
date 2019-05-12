
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des réductions.
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

extern CONFIG		Config;
extern HBITMAP		hThumbBck;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'une réduction avec options d'affichage «««««««««««««««»

HBITMAP Thumbnail_GenerateExt(HWND hWnd, MAP *Map, HBITMAP Thumbnail, THUMBOPTS *Options)
{
	HDC		 hDC;
	HDC		 hThumbDC;
	HBITMAP		 hThumbBitmap = NULL;
	HBITMAP		 hThumbOldBitmap;
	register BYTE	*MapPtr,*LinePtr;
	register DWORD	 PixelColor;
	register long	 i,j;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		hThumbDC = CreateCompatibleDC(hDC);
		if (hThumbDC)
			{
			if (Thumbnail)
				hThumbBitmap = Thumbnail;
			else
				hThumbBitmap = CreateCompatibleBitmap(hDC,Map->Properties.Width,Map->Properties.Height);

			if (hThumbBitmap)
				{
				hThumbOldBitmap = SelectObject(hThumbDC,hThumbBitmap);
				if (hThumbOldBitmap)
					{
					MapPtr = Map->Map;
					for (i = 0; i != Map->Properties.Height; i++)
						{
						LinePtr = MapPtr;
						for (j = 0; j != Map->Properties.Width; j++)
							{
							switch(*(LinePtr+3))
								{
								case 0:	PixelColor = WorldCols[*LinePtr];
									switch(World[*LinePtr].Id&0xFF000000)
										{
										case 0x1E000000:
											if (Options->WorldFlags&TOF_SHOWLIMIT) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x04000000:
											if (Options->WorldFlags&TOF_SHOWWATER) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x05000000:
											if (Options->WorldFlags&TOF_SHOWLAVA) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x06000000:
											if (Options->WorldFlags&TOF_SHOWGOLD) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x07000000:
											if (Options->WorldFlags&TOF_SHOWGEMS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x01000000:
											if (Options->WorldFlags&TOF_SHOWSOLID) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x02000000:
											if (Options->WorldFlags&TOF_SHOWROCK) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x03000000:
											if (Options->WorldFlags&TOF_SHOWUNCLAIMED) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x23000000:
											if (Options->WorldFlags&TOF_SHOWHEROESREST) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0x1F000000:
											if (Options->PlayersFlags&TOF_SHOWNEUTRALROOMS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 0xFE000000:
											if (Options->WorldFlags&TOF_SHOWLIMIT) break;
											PixelColor = Options->HiddenColor;
											break;
										}
									break;									
								case 1:	PixelColor = RoomsCols[*LinePtr][(*(LinePtr+1))-1];
									switch(Rooms[*LinePtr].Id&0xFF000000)
										{
										case 0x0E000000:
											switch(*(LinePtr+1))
												{
												case 1:
													if (Options->WorldFlags&TOF_SHOWHEROESHEARTS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 2:
													if (Options->PlayersFlags&TOF_SHOWNEUTRALHEARTS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 3:
													if (Options->PlayersFlags&TOF_SHOWKEEPER1HEART) break;
													PixelColor = Options->HiddenColor;
													break;
												case 4:
													if (Options->PlayersFlags&TOF_SHOWKEEPER2HEART) break;
													PixelColor = Options->HiddenColor;
													break;
												case 5:
													if (Options->PlayersFlags&TOF_SHOWKEEPER3HEART) break;
													PixelColor = Options->HiddenColor;
													break;
												case 6:
													if (Options->PlayersFlags&TOF_SHOWKEEPER4HEART) break;
													PixelColor = Options->HiddenColor;
													break;
												case 7:
													if (Options->PlayersFlags&TOF_SHOWKEEPER5HEART) break;
													PixelColor = Options->HiddenColor;
													break;
												}
											break;
										case 0x28000000:
										case 0x0C000000:
											switch(*(LinePtr+1))
												{
												case 1:
													if (Options->PlayersFlags&TOF_SHOWHEROESPORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 2:
													if (Options->PlayersFlags&TOF_SHOWNEUTRALPORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 3:
													if (Options->PlayersFlags&TOF_SHOWKEEPER1PORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 4:
													if (Options->PlayersFlags&TOF_SHOWKEEPER2PORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 5:
													if (Options->PlayersFlags&TOF_SHOWKEEPER3PORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 6:
													if (Options->PlayersFlags&TOF_SHOWKEEPER4PORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 7:
													if (Options->PlayersFlags&TOF_SHOWKEEPER5PORTALS) break;
													PixelColor = Options->HiddenColor;
													break;
												}
											break;

										default:switch(*(LinePtr+1))
												{
												case 1:
													if (Options->PlayersFlags&TOF_SHOWHEROESROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 2:
													if (Options->PlayersFlags&TOF_SHOWNEUTRALROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 3:
													if (Options->PlayersFlags&TOF_SHOWKEEPER1ROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 4:
													if (Options->PlayersFlags&TOF_SHOWKEEPER2ROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 5:
													if (Options->PlayersFlags&TOF_SHOWKEEPER3ROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 6:
													if (Options->PlayersFlags&TOF_SHOWKEEPER4ROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												case 7:
													if (Options->PlayersFlags&TOF_SHOWKEEPER5ROOMS) break;
													PixelColor = Options->HiddenColor;
													break;
												}
											break;
										}
									break;
								case 2:	PixelColor = THUMB_HEROPORTAL;
									if (Options->PlayersFlags&TOF_SHOWHEROESPORTALS) break;
									PixelColor = Options->HiddenColor;
									break;
								case 3:	PixelColor = WallsCols[*LinePtr];
									switch(*(LinePtr+1))
										{
										case 1:	if (Options->PlayersFlags&TOF_SHOWHEROESWALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 2:	if (Options->PlayersFlags&TOF_SHOWNEUTRALWALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 3:	if (Options->PlayersFlags&TOF_SHOWKEEPER1WALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 4:	if (Options->PlayersFlags&TOF_SHOWKEEPER2WALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 5:	if (Options->PlayersFlags&TOF_SHOWKEEPER3WALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 6:	if (Options->PlayersFlags&TOF_SHOWKEEPER4WALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										case 7:	if (Options->PlayersFlags&TOF_SHOWKEEPER5WALLS) break;
											PixelColor = Options->HiddenColor;
											break;
										}
									break;
								default:PixelColor = THUMB_LIMIT;
									break;
								}
							SetPixelV(hThumbDC,j,i,PixelColor);
							LinePtr += MAP_CONTROLBYTES;
							}
						MapPtr += Map->Properties.Width*MAP_CONTROLBYTES;
						}
					}
				else
					{
					if (!Options->KeepOldBitmap) DeleteObject(hThumbBitmap);
					hThumbBitmap = NULL;
					}
				SelectObject(hThumbDC,hThumbOldBitmap);
				}

			DeleteDC(hThumbDC);
			}
		ReleaseDC(hWnd,hDC);
		}

	return(hThumbBitmap);
}


// «»»» Création d'une réduction ««««««««««««««««««««««««««««««««««««««««»

HBITMAP Thumbnail_Generate(HWND hWnd, MAP *Map, HBITMAP Thumbnail)
{
	HDC		 hDC;
	HDC		 hThumbDC;
	HBITMAP		 hThumbBitmap = NULL;
	HBITMAP		 hThumbOldBitmap;
	register BYTE	*MapPtr,*LinePtr;
	register DWORD	 PixelColor;
	register long	 i,j;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		hThumbDC = CreateCompatibleDC(hDC);
		if (hThumbDC)
			{
			if (Thumbnail)
				hThumbBitmap = Thumbnail;
			else
				hThumbBitmap = CreateCompatibleBitmap(hDC,Map->Properties.Width,Map->Properties.Height);

			if (hThumbBitmap)
				{
				hThumbOldBitmap = SelectObject(hThumbDC,hThumbBitmap);
				if (hThumbOldBitmap)
					{
					MapPtr = Map->Map;
					for (i = 0; i != Map->Properties.Height; i++)
						{
						LinePtr = MapPtr;
						for (j = 0; j != Map->Properties.Width; j++)
							{
							switch(*(LinePtr+3))
								{
								case 0:	PixelColor = WorldCols[*LinePtr];
									break;
								case 1:	PixelColor = RoomsCols[*LinePtr][(*(LinePtr+1))-1];
									break;
								case 2:	PixelColor = THUMB_HEROPORTAL;
									break;
								case 3:	PixelColor = WallsCols[*LinePtr];
									break;
								default:PixelColor = THUMB_LIMIT;
									break;
								}
							SetPixelV(hThumbDC,j,i,PixelColor);
							LinePtr += MAP_CONTROLBYTES;
							}
						MapPtr += Map->Properties.Width*MAP_CONTROLBYTES;
						}
					SelectObject(hThumbDC,hThumbOldBitmap);
					}
				else
					{
					DeleteObject(hThumbBitmap);
					hThumbBitmap = NULL;
					}
				}
			DeleteDC(hThumbDC);
			}
		ReleaseDC(hWnd,hDC);
		}

	return(hThumbBitmap);
}


// «»»» Copie de la réduction dans un contexte d'affichage ««««««««««««««»

#pragma argsused
void Thumbnail_CopyBitmap(HDC hDestDC, LONG X, LONG Y, LONG ThumbSize, HDC hSrcDC, LONG Width, LONG Height, HBITMAP Background, BOOL UseColorKey, COLORREF ColorKey)
{
	LONG	Prop;

	if (!Background) goto Normal_Blit;
	if (UseColorKey) goto Transparency_Blit;

//
// --- Affichage classique ---
//

Normal_Blit:
	if ((Config.MinimapKeepProps)&&(Width != Height))
		{
		HDC	hBckDC;
		HBITMAP	hBckBitmap;

		/* Copie l'image de fond (si existante) */

		if (hThumbBck)
			{
			hBckDC = CreateCompatibleDC(hDestDC);
			if (hBckDC)
				{
				hBckBitmap = SelectObject(hBckDC,hThumbBck);
				if (!hBckBitmap)
					{
					DeleteDC(hBckDC);
					hBckDC = NULL;
					}
				else
					StretchBlt(hDestDC,X,Y,ThumbSize,ThumbSize,hBckDC,0,0,144,144,SRCCOPY);
				}
			}
		else
			hBckDC = NULL;

		/* Copie la réduction (en corrigeant les proportions) */

		if (Width < Height)
			{
			Prop = (long)((float)ThumbSize*((float)Width/(float)Height));
			if (!hBckDC)
				{
				BitBlt(hDestDC,X,Y,(ThumbSize-Prop)/2,ThumbSize,NULL,0,0,BLACKNESS);
				BitBlt(hDestDC,(ThumbSize-Prop)/2+Prop+X,Y,(ThumbSize-Prop)/2+((ThumbSize-Prop)%2),ThumbSize,NULL,0,0,BLACKNESS);
				}
			StretchBlt(hDestDC,(ThumbSize-Prop)/2+X,Y,Prop,ThumbSize,hSrcDC,0,0,Width,Height,SRCCOPY);
			}
		else
			{
			Prop = (long)((float)ThumbSize*((float)Height/(float)Width));
			if (!hBckDC)
				{
				BitBlt(hDestDC,X,Y,ThumbSize,(ThumbSize-Prop)/2,NULL,0,0,BLACKNESS);
				BitBlt(hDestDC,X,(ThumbSize-Prop)/2+Prop+Y,ThumbSize,(ThumbSize-Prop)/2+((ThumbSize-Prop)%2),NULL,0,0,BLACKNESS);
				}
			StretchBlt(hDestDC,X,(ThumbSize-Prop)/2+Y,ThumbSize,Prop,hSrcDC,0,0,Width,Height,SRCCOPY);
			}

		if (hBckDC)
			{
			SelectObject(hBckDC,hBckBitmap);
			DeleteDC(hBckDC);
			}
		}
	else
		StretchBlt(hDestDC,X,Y,ThumbSize,ThumbSize,hSrcDC,0,0,Width,Height,SRCCOPY);

	return;

//
// --- Affichage avec gestion de la transparence ---
//

Transparency_Blit: {
	HDC		hTempDC;
	HBITMAP		hTempBitmap;
	HBITMAP		hTempOldBitmap;
	HBRUSH		hTempBrush;
	RECT		Rect;
	HDC		hBckDC;
	HBITMAP		hBckBitmap;
	COLORREF	Color;
	LONG		PX,PY;

	hTempDC = CreateCompatibleDC(hDestDC);
	if (!hTempDC) goto Error_0;
	hTempBitmap = CreateCompatibleBitmap(hDestDC,ThumbSize,ThumbSize);
	if (!hTempBitmap) goto Error_1;
	hTempOldBitmap = SelectObject(hTempDC,hTempBitmap);
	if (!hTempOldBitmap) goto Error_2;
	hTempBrush = CreateSolidBrush(ColorKey);
	if (!hTempBrush) goto Error_3;

	/* Remplissage de l'image temporaire avec la couleur de transparence */

	Rect.left = 0;
	Rect.top = 0;
	Rect.right = ThumbSize;
	Rect.bottom = ThumbSize;
	FillRect(hTempDC,&Rect,hTempBrush);

	/* Copie la réduction dans l'image temporaire */

	if ((Config.MinimapKeepProps)&&(Width != Height))
		{
		if (Width < Height)
			{
			Prop = (long)((float)ThumbSize*((float)Width/(float)Height));
			StretchBlt(hTempDC,(ThumbSize-Prop)/2,0,Prop,ThumbSize,hSrcDC,0,0,Width,Height,SRCCOPY);
			}
		else
			{
			Prop = (long)((float)ThumbSize*((float)Height/(float)Width));
			StretchBlt(hTempDC,0,(ThumbSize-Prop)/2,ThumbSize,Prop,hSrcDC,0,0,Width,Height,SRCCOPY);
			}
		}
	else
		StretchBlt(hTempDC,0,0,ThumbSize,ThumbSize,hSrcDC,0,0,Width,Height,SRCCOPY);

	/* Copie l'image de fond dans l'image définitive */

	hBckDC = CreateCompatibleDC(hDestDC);
	if (!hBckDC) goto Error_4;
	hBckBitmap = SelectObject(hBckDC,hThumbBck);
	if (!hBckBitmap) goto Error_5;
	StretchBlt(hDestDC,X,Y,ThumbSize,ThumbSize,hBckDC,0,0,144,144,SRCCOPY);
	SelectObject(hBckDC,hBckBitmap);
	DeleteDC(hBckDC);

	/* Copie l'image temporaire dans l'image définitive */

	for (PY = 0; PY != ThumbSize; PY++)
		for (PX = 0; PX != ThumbSize; PX++)
			{
			Color = GetPixel(hTempDC,PX,PY);
			if (Color == ColorKey) continue;
			SetPixelV(hDestDC,PX+X,PY+Y,Color);
			}

	/* Fin de l'opération */

	DeleteObject(hTempBrush);
	SelectObject(hTempDC,hTempOldBitmap);
	DeleteObject(hTempBitmap);
	DeleteDC(hTempDC);
	return;

// --- Erreurs lors de l'affichage en transparence ---

Error_5:DeleteDC(hBckDC);
Error_4:DeleteObject(hTempBrush);
Error_3:SelectObject(hTempDC,hTempOldBitmap);
Error_2:DeleteObject(hTempBitmap);
Error_1:DeleteDC(hTempDC);
Error_0:goto Normal_Blit; }
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde d'une réduction au format BMP				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Thumbnail_GenerateFile(HWND hWnd, HBITMAP hThumbBmp, HBITMAP hThumbBck, LONG ThumbSize)
{
	BITMAPFILEHEADER	 FileHeader;
	BITMAPINFOHEADER	 InfoHeader;
	BITMAP			 BitmapInfo;
	BYTE			*BitmapFile;
	BYTE			*BitmapBits;
	DWORD			 PixelColor;
	DWORD			 LastError;
	DWORD			 Written;
	LONG			 X,Y;
	HDC			 hDC,hWndDC,hWorkDC;
	HBITMAP			 hBitmap,hWorkBitmap,hWorkOldBitmap;
	HANDLE			 FileHandle;

	//
	// Les réductions en 144x144 font en réalité 128x128
	//

	if (ThumbSize == 144) ThumbSize = 128;

	//
	// Inscription des paramètres dans les structures
	//

	GetObject(hThumbBmp,sizeof(BITMAP),&BitmapInfo);
	ZeroMemory(&FileHeader,sizeof(BITMAPFILEHEADER));
	ZeroMemory(&InfoHeader,sizeof(BITMAPINFOHEADER));

	FileHeader.bfType = MAKEWORD('B','M');
	FileHeader.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ThumbSize*ThumbSize*3;
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	InfoHeader.biWidth = ThumbSize;
	InfoHeader.biHeight = ThumbSize;
	InfoHeader.biPlanes = 1;
	InfoHeader.biBitCount = 24;
	InfoHeader.biCompression = BI_RGB;
	InfoHeader.biSizeImage = ThumbSize*ThumbSize*3;
	InfoHeader.biXPelsPerMeter = 0;
	InfoHeader.biYPelsPerMeter = 0;
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;

	//
	// Création de l'image
	//

	BitmapFile = GlobalAlloc(GPTR,FileHeader.bfSize);
	LastError = GetLastError();
	if (!BitmapFile) goto Error_0;

	hWndDC = GetDC(hWnd);
	LastError = GetLastError();
	if (!hWndDC) goto Error_1;
	hDC = CreateCompatibleDC(hWndDC);
	LastError = GetLastError();
	if (!hDC) goto Error_2;
	hBitmap = SelectObject(hDC,hThumbBmp);
	LastError = GetLastError();
	if (!hBitmap) goto Error_3;

	hWorkDC = CreateCompatibleDC(hDC);
	LastError = GetLastError();
	if (!hWorkDC) goto Error_4;
	hWorkBitmap = CreateCompatibleBitmap(hDC,ThumbSize,ThumbSize);
	LastError = GetLastError();
	if (!hWorkBitmap) goto Error_5;
	hWorkOldBitmap = SelectObject(hWorkDC,hWorkBitmap);
	LastError = GetLastError();
	if (!hWorkOldBitmap) goto Error_6;

	Thumbnail_CopyBitmap(hWorkDC,0,0,ThumbSize,hDC,BitmapInfo.bmWidth,BitmapInfo.bmHeight,hThumbBck,Config.Thumbnail.UseColorKey,Config.Thumbnail.HiddenColor);
	CopyMemory(BitmapFile,&FileHeader,sizeof(BITMAPFILEHEADER));
	CopyMemory(BitmapFile+sizeof(BITMAPFILEHEADER),&InfoHeader,sizeof(BITMAPINFOHEADER));
	BitmapBits = BitmapFile+FileHeader.bfOffBits;

	for (Y = 0; Y != ThumbSize; Y++)
		for (X = 0; X != ThumbSize; X++)
			{
			PixelColor = GetPixel(hWorkDC,X,ThumbSize-Y-1);
			*BitmapBits++ = (BYTE)((PixelColor&0x00FF0000)>>16);
			*BitmapBits++ = (BYTE)((PixelColor&0x0000FF00)>>8);
			*BitmapBits++ = (BYTE)((PixelColor&0x000000FF));
			}

	SelectObject(hWorkDC,hWorkOldBitmap);
	DeleteObject(hWorkBitmap);
	DeleteDC(hWorkDC);
	SelectObject(hDC,hBitmap);
	DeleteDC(hDC);
	ReleaseDC(hWnd,hWndDC);

	//
	// Sélection du fichier
	//

	if (!Misc_FileSelectSave(hWnd,&Config.ThumbPath[0],FILE_THUMBNAIL))
		{
		GlobalFree(BitmapFile);
		return;
		}

	//
	// Sauvegarde du fichier
	//

	FileHandle = CreateFile(Config.ThumbPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		LastError = GetLastError();
		goto Error_1;
		}

	WriteFile(FileHandle,BitmapFile,FileHeader.bfSize,&Written,NULL);
	LastError = GetLastError();
	CloseHandle(FileHandle);

	if (Written != FileHeader.bfSize)
		{
		DeleteFile(Config.ThumbPath);
		goto Error_1;
		}

	GlobalFree(BitmapFile);
	return;

//--- Erreurs ---

Error_6:DeleteObject(hWorkBitmap);
Error_5:DeleteDC(hWorkDC);
Error_4:SelectObject(hDC,hBitmap);
Error_3:DeleteDC(hDC);
Error_2:ReleaseDC(hWnd,hWndDC);
Error_1:GlobalFree(BitmapFile);
Error_0:SetLastError(LastError);
	Misc_PrintError(hWnd,szBitmapErr,NULL,MB_ICONHAND);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
