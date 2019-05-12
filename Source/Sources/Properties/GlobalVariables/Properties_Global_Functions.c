
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines des variables globales
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection des paramètres d'un tir				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

//
// Valeur pour DialogID :
//				0 - Deux valeurs
//				1 - Une valeur
//				2 - Sélection d'une créature (avec nombre)
//				3 - Sélection d'une créature (avec niveau)
//				4 - Sélection d'un objet
//

// «»»» Changement des paramètres des tirs ««««««««««««««««««««««««««««««»

int Properties_GlobalShotOptions(HWND hDlg, GLOBALSHOT *Context)
{
	int	Result;

	for (Result = 0; GShots[Result].Id != 0xFFFFFFFF; Result++)
		{
		if (Context->ShotType == GShots[Result].Id)
			{
			Context->DialogID	= GShots[Result].Variable;
			Context->Text1ID	= GShots[Result].MinWidth;
			Context->Text2ID	= GShots[Result].MinHeight;
			Context->Ctrl1Signed	= GShots[Result].MaxWidth;
			Context->Ctrl2Signed	= GShots[Result].MaxHeight;
			break;
			}
		}
	if (Context->DialogID == 0xFF)
		{
		Misc_MessageBoxExt(hDlg,szGlobalNoParam,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
		return(0);
		}

	switch(Context->DialogID)
		{
		case 2:
		case 3:	Misc_CreateImageList(GCreatures,&Context->OptionsImages,32,32,FALSE);
			if (!Context->OptionsImages) return(0);
			break;
		case 4:	Misc_CreateImageList(Objects,&Context->OptionsImages,32,32,FALSE);
			if (!Context->OptionsImages) return(0);
			break;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6840+Context->DialogID),hDlg,Properties_GlobalShotProc,(LPARAM)Context);
	if ((Result == -1)||(Result == 0))
		{
		if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		Misc_DestroyImageList(&Context->OptionsImages);
		return(0);
		}

	Misc_DestroyImageList(&Context->OptionsImages);
	return(1);
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_GlobalShotProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GLOBALSHOT	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_GlobalShotInit(hDlg,(GLOBALSHOT *)lParam);
		return(TRUE);
		}

	Context = (GLOBALSHOT *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if ( ((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				Properties_GlobalSetList((MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if ( ((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX )
				{
				ELEMENT	*Table;

				switch(Context->DialogID)
					{
					case 2:
					case 3:	Table = GCreatures;
						break;
					case 4:	Table = Objects;
						break;
					default:return(FALSE);
					}

				Properties_GlobalDrawList((DRAWITEMSTRUCT *)lParam,Table,Context->OptionsImages);
				return(TRUE);
				}

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch( ((NMHDR *)lParam)->idFrom )
						{
						case 206:
							Misc_SetUpDownInt(hDlg,205,(NM_UPDOWN *)lParam);
							break;

						case 204:
							Misc_SetUpDownInt(hDlg,203,(NM_UPDOWN *)lParam);
							break;

						case 104:
							if (Context->Ctrl1Signed)
								Misc_SetUpDownIntSigned(hDlg,101,(NM_UPDOWN *)lParam);
							else
								Misc_SetUpDownInt(hDlg,101,(NM_UPDOWN *)lParam);
							break;
						case 105:
							if (Context->Ctrl2Signed)
								Misc_SetUpDownIntSigned(hDlg,101,(NM_UPDOWN *)lParam);
							else
								Misc_SetUpDownInt(hDlg,101,(NM_UPDOWN *)lParam);
							break;
						}	
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 201:
							if (Context->DialogID != 4) break;
							if (Objects[SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id != 1)
								{
								EnableWindow(GetDlgItem(hDlg,202),FALSE);
								EnableWindow(GetDlgItem(hDlg,205),FALSE);
								EnableWindow(GetDlgItem(hDlg,206),FALSE);
								}
							else
								{
								EnableWindow(GetDlgItem(hDlg,202),TRUE);
								EnableWindow(GetDlgItem(hDlg,205),TRUE);
								EnableWindow(GetDlgItem(hDlg,206),TRUE);
								}
							return(TRUE);
						}
					break;

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 205:
							lData = GetDlgItemInt(hDlg,205,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,205,0,FALSE);
							if (lData > 80000) SetDlgItemInt(hDlg,205,80000,FALSE);
							break;

						case 203:
							lData = GetDlgItemInt(hDlg,203,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,203,0,FALSE);
							if (lData > 100) SetDlgItemInt(hDlg,203,100,FALSE);
							break;

						case 101:
							if (Context->Ctrl1Signed)
								{
								lData = GetDlgItemInt(hDlg,101,NULL,TRUE);
								if (lData < -32768) SetDlgItemInt(hDlg,101,-32768,FALSE);
								if (lData > 32767) SetDlgItemInt(hDlg,101,32767,FALSE);
								}
							else
								{
								lData = GetDlgItemInt(hDlg,101,NULL,FALSE);
								if (lData < 0) SetDlgItemInt(hDlg,101,0,FALSE);
								if (lData > 32767) SetDlgItemInt(hDlg,101,32767,FALSE);
								}
							break;
						case 103:
							if (Context->Ctrl2Signed)
								{
								lData = GetDlgItemInt(hDlg,103,NULL,TRUE);
								if (lData < -32768) SetDlgItemInt(hDlg,103,-32768,FALSE);
								if (lData > 32767) SetDlgItemInt(hDlg,103,32767,FALSE);
								}
							else
								{
								lData = GetDlgItemInt(hDlg,103,NULL,FALSE);
								if (lData < 0) SetDlgItemInt(hDlg,103,0,FALSE);
								if (lData > 32767) SetDlgItemInt(hDlg,103,32767,FALSE);
								}
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Properties_GlobalShotSave(hDlg,Context);
							EndDialog(hDlg,1);
							return(TRUE);

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


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

void Properties_GlobalShotInit(HWND hDlg, GLOBALSHOT *Context)
{
	LONG	 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LPARAM)Context);

	//
	// Initialise les données propres à chaque boîte de dialogue
	//

	switch(Context->DialogID)
		{
		case 0:	Misc_CreateUpDownButton(hDlg,101,104);
			Misc_CreateUpDownButton(hDlg,103,105);
			SetDlgItemInt(hDlg,101,Context->ShotData1,(Context->Ctrl1Signed)?TRUE:FALSE);
			SetDlgItemInt(hDlg,103,Context->ShotData2,(Context->Ctrl2Signed)?TRUE:FALSE);
			SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			SendDlgItemMessage(hDlg,103,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			break;

		case 1:	Misc_CreateUpDownButton(hDlg,101,104);
			SetDlgItemInt(hDlg,101,Context->ShotData1,(Context->Ctrl1Signed)?TRUE:FALSE);
			SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)6,(LPARAM)0);
			break;

		case 2:
		case 3:	if (Context->DialogID == 2)
				{
				Misc_CreateUpDownButton(hDlg,203,204);
				SetDlgItemInt(hDlg,203,Context->ShotData2,FALSE);
				SendDlgItemMessage(hDlg,203,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
				}
			else
				{
				char	TempText[4];

				for (X = 1; X != 10+1; X++)
					{
					wsprintf(TempText,szNumber,X);
					if (SendDlgItemMessage(hDlg,203,CB_ADDSTRING,(WPARAM)0,(LPARAM)TempText) < 0)
						{
						EndDialog(hDlg,-1);
						return;
						}
					}
				if ((Context->ShotData2 >= 1)&&(Context->ShotData2 <= 10))
					SendDlgItemMessage(hDlg,203,CB_SETCURSEL,(WPARAM)Context->ShotData2-1,(LPARAM)0);
				else
					SendDlgItemMessage(hDlg,203,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
				}

			for (X = 0; GCreatures[X].Id != 0; X++)
				if (SendDlgItemMessage(hDlg,201,LB_ADDSTRING,(WPARAM)0,(LPARAM)GCreatures[X].Id) < 0)
					{
					EndDialog(hDlg,-1);
					return;
					}
			for (X = 0; GCreatures[X].Id != 0; X++)
				if (Context->ShotData1 == GCreatures[X].Id)
					{
					SendDlgItemMessage(hDlg,201,LB_SETCURSEL,(WPARAM)X,(LPARAM)0);
					goto Next;
					}
			SendDlgItemMessage(hDlg,201,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			break;

		case 4:	Misc_CreateUpDownButton(hDlg,205,206);
			SetDlgItemInt(hDlg,205,Context->ShotData2,FALSE);
			SendDlgItemMessage(hDlg,205,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);

			if (Context->ShotData1 != 1)
				{
				EnableWindow(GetDlgItem(hDlg,202),FALSE);
				EnableWindow(GetDlgItem(hDlg,205),FALSE);
				EnableWindow(GetDlgItem(hDlg,206),FALSE);
				}

			for (X = 0; Objects[X].Id != 0; X++)
				if (SendDlgItemMessage(hDlg,201,LB_ADDSTRING,(WPARAM)0,(LPARAM)Objects[X].Id) < 0)
					{
					EndDialog(hDlg,-1);
					return;
					}
			for (X = 0; Objects[X].Id != 0; X++)
				if (Context->ShotData1 == Objects[X].Id)
					{
					SendDlgItemMessage(hDlg,201,LB_SETCURSEL,(WPARAM)X,(LPARAM)0);
					goto Next;
					}

			EnableWindow(GetDlgItem(hDlg,202),TRUE);
			EnableWindow(GetDlgItem(hDlg,205),TRUE);
			EnableWindow(GetDlgItem(hDlg,206),TRUE);
			SendDlgItemMessage(hDlg,201,LB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			break;
		}

	//
	// Place les textes des zones d'édition
	//

Next:	switch(Context->Text1ID)
		{
		case 1:	SetDlgItemText(hDlg,100,szGlobalPower);
			break;
		case 2:	SetDlgItemText(hDlg,100,szGlobalModify);
			break;
		case 3:	SetDlgItemText(hDlg,100,szGlobalIncrease);
			break;
		case 4:	SetDlgItemText(hDlg,100,szGlobalTime);
			break;
		case 5:	SetDlgItemText(hDlg,100,szGlobalDecrease);
			break;
		case 6:	SetDlgItemText(hDlg,100,szGlobalRadius);
			break;
		default:break;
		}

	switch(Context->Text2ID)
		{
		case 1:	SetDlgItemText(hDlg,102,szGlobalRadius);
			break;
		case 2:	SetDlgItemText(hDlg,102,szGlobalTime);
			break;
		default:break;
		}

	//
	// Modifie les zones d'éditions pour qu'elles acceptent les signes
	//

	if (Context->Ctrl1Signed)
		{
		X = GetWindowLong(GetDlgItem(hDlg,101),GWL_STYLE);
		X &= ~ES_NUMBER;
		SetWindowLong(GetDlgItem(hDlg,101),GWL_STYLE,X);
		}

	if (Context->Ctrl2Signed)
		{
		X = GetWindowLong(GetDlgItem(hDlg,103),GWL_STYLE);
		X &= ~ES_NUMBER;
		SetWindowLong(GetDlgItem(hDlg,103),GWL_STYLE,X);
		}

	return;
}


// «»»» Sauvegarde des valeurs ««««««««««««««««««««««««««««««««««««««««««»

void Properties_GlobalShotSave(HWND hDlg, GLOBALSHOT *Context)
{
	switch(Context->DialogID)
		{
		case 0:	Context->ShotData1 = GetDlgItemInt(hDlg,101,NULL,(Context->Ctrl1Signed)?TRUE:FALSE);
			Context->ShotData2 = GetDlgItemInt(hDlg,103,NULL,(Context->Ctrl2Signed)?TRUE:FALSE);
			break;
		case 1:	Context->ShotData1 = GetDlgItemInt(hDlg,101,NULL,(Context->Ctrl1Signed)?TRUE:FALSE);
			break;
		case 2:	Context->ShotData1 = GCreatures[SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->ShotData2 = GetDlgItemInt(hDlg,203,NULL,FALSE);
			break;
		case 3:	Context->ShotData1 = GCreatures[SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->ShotData2 = SendDlgItemMessage(hDlg,203,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
			break;
		case 4:	Context->ShotData1 = Objects[SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)].Id;
			Context->ShotData2 = GetDlgItemInt(hDlg,205,NULL,FALSE);
			if (Context->ShotData1 != 1) Context->ShotData2 = 0;
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Définition de la taille des entrées dans une LISTBOX ««««««««««««»

void Properties_GlobalSetList(MEASUREITEMSTRUCT *ItemStruct)
{
	ItemStruct->itemHeight = 32+2;
	if (Font.FontHeight > 32) ItemStruct->itemHeight = Font.FontHeight+2;
	return;
}


// «»»» Affichage d'une entrée dans une LISTBOX «««««««««««««««««««««««««»

void Properties_GlobalDrawList(DRAWITEMSTRUCT *ItemStruct, ELEMENT *Table, HIMAGELIST hImageList)
{
	RECT		WorkRect;
	HBRUSH		hBrush,hOldBrush;
	COLORREF	BackColor;
	COLORREF	OldTextColor;
	COLORREF	OldBackColor;
	int		i;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 1;
	WorkRect.top += 1;
	WorkRect.right -= 1;
	WorkRect.bottom -= 1;
	BackColor = GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW);
	OldTextColor = SetTextColor(ItemStruct->hDC,GetSysColor(ItemStruct->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	OldBackColor = SetBkColor(ItemStruct->hDC,BackColor);

	//--- Affichage de l'image ---

	for (i = 0; Table[i].Id != 0; i++)
		{
		if (Table[i].Id == ItemStruct->itemData)
			{
			ImageList_Draw(hImageList,i,ItemStruct->hDC,WorkRect.left,WorkRect.top+(WorkRect.bottom-WorkRect.top-32)/2,ItemStruct->itemState&ODS_SELECTED?ILD_SELECTED:ILD_NORMAL);
			break;
			}
		}


	//--- Affichage du fond du texte ---

	WorkRect.left += 33;
	hBrush = CreateSolidBrush(BackColor);
	if (hBrush)
		{
		hOldBrush = SelectObject(ItemStruct->hDC,hBrush);
		if (hBrush)
			{
			FillRect(ItemStruct->hDC,&WorkRect,hBrush);
			SelectObject(ItemStruct->hDC,hOldBrush);
			}
		DeleteObject(hBrush);
		}
	if (ItemStruct->itemState&ODS_FOCUS)
		DrawFocusRect(ItemStruct->hDC,&WorkRect);

	//--- Affichage du texte ---

	WorkRect.left++;
	WorkRect.right--;
	for (i = 0; Table[i].Id != 0; i++)
		{
		if (Table[i].Id == ItemStruct->itemData)
			{
			DrawText(ItemStruct->hDC,Table[i].Name,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
			break;
			}
		}

	//--- Terminé ! ---

	SetBkColor(ItemStruct->hDC,OldBackColor);
	SetTextColor(ItemStruct->hDC,OldTextColor);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines générales						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation des variables globales «««««««««««««««««««««««««««»

void Properties_GlobalReset(GLOBALVARS *Variables, LONG UseOffset, DWORD Flags)
{
	BYTE	*Default;
	long	 Offset;
	int	 i;

	if (Flags&MGRF_TERRAINS)
		{
		for (i = 0; GTOrder[i] != (DWORD *)-1; i += 2)
			{
			if (GTOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GTOrder[i];
			Default = (BYTE *)GTOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Terrains[Offset].MaxHealthType	= (UINT)(*((BYTE *)(Default+0x1D9)));
			Variables->Terrains[Offset].MaxHealthEffect	= (UINT)(*((WORD *)(Default+0x1A4)));
			Variables->Terrains[Offset].DestroyedType	= (UINT)(*((BYTE *)(Default+0x1DA)));
			Variables->Terrains[Offset].DestroyedEffect	= (UINT)(*((WORD *)(Default+0x1A6)));
			Variables->Terrains[Offset].Damage		= (LONG)(*((short *)(Default+0x194)));
			Variables->Terrains[Offset].GoldValue		= (LONG)(*((WORD *)(Default+0x19A)));
			Variables->Terrains[Offset].MaxHealth		= (LONG)(*((WORD *)(Default+0x1FF)));
			Variables->Terrains[Offset].StartingHealth	= (LONG)(*((WORD *)(Default+0x1D7)));
			Variables->Terrains[Offset].ManaGain		= (LONG)(*((WORD *)(Default+0x19C)));
			Variables->Terrains[Offset].MaxManaGain		= (LONG)(*((WORD *)(Default+0x19E)));
			Variables->Terrains[Offset].WibbleH		= (LONG)(*((BYTE *)(Default+0x1CE)));
			Variables->Terrains[Offset].WibbleV		= (LONG)(*((BYTE *)(Default+0x1D2)));
			Variables->Terrains[Offset].Visible		= (BOOL)(((*(Default+0x193))&0x01)?TRUE:FALSE);
			Variables->Terrains[Offset].TerrainLight	= (BOOL)(((*(Default+0x193))&0x10)?TRUE:FALSE);
			Variables->Terrains[Offset].AmbientLight	= (BOOL)(((*(Default+0x193))&0x20)?TRUE:FALSE);
			Variables->Terrains[Offset].TerrainLightR	= (BYTE)(*((BYTE *)(Default+0x01DB)));
			Variables->Terrains[Offset].TerrainLightG	= (BYTE)(*((BYTE *)(Default+0x01DC)));
			Variables->Terrains[Offset].TerrainLightB	= (BYTE)(*((BYTE *)(Default+0x01DD)));
			Variables->Terrains[Offset].AmbientLightR	= (BYTE)(*((BYTE *)(Default+0x0201)));
			Variables->Terrains[Offset].AmbientLightG	= (BYTE)(*((BYTE *)(Default+0x0202)));
			Variables->Terrains[Offset].AmbientLightB	= (BYTE)(*((BYTE *)(Default+0x0203)));
			Variables->Terrains[Offset].TerrainLightH	= (LONG)(*((LONG *)(Default+0x018C)));
			}
		}

	if (Flags&MGRF_ROOMS)
		{
		for (i = 0; GROrder[i] != (DWORD *)-1; i += 2)
			{
			if (GROrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GROrder[i];
			Default = (BYTE *)GROrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Rooms[Offset].Cost			= (LONG)(*((WORD *)(Default+0x378)));
			Variables->Rooms[Offset].Objects[0]		= (UINT)(*((BYTE *)(Default+0x39A)));
			Variables->Rooms[Offset].Objects[1]		= (UINT)(*((BYTE *)(Default+0x39B)));
			Variables->Rooms[Offset].Objects[2]		= (UINT)(*((BYTE *)(Default+0x39C)));
			Variables->Rooms[Offset].Objects[3]		= (UINT)(*((BYTE *)(Default+0x39D)));
			Variables->Rooms[Offset].Objects[4]		= (UINT)(*((BYTE *)(Default+0x39E)));
			Variables->Rooms[Offset].Objects[5]		= (UINT)(*((BYTE *)(Default+0x39F)));
			Variables->Rooms[Offset].BattleEffect		= (UINT)(*((WORD *)(Default+0x37A)));
			Variables->Rooms[Offset].Effects[0]		= (UINT)(*((WORD *)(Default+0x382)));
			Variables->Rooms[Offset].Effects[1]		= (UINT)(*((WORD *)(Default+0x384)));
			Variables->Rooms[Offset].Effects[2]		= (UINT)(*((WORD *)(Default+0x386)));
			Variables->Rooms[Offset].Effects[3]		= (UINT)(*((WORD *)(Default+0x388)));
			Variables->Rooms[Offset].Effects[4]		= (UINT)(*((WORD *)(Default+0x38A)));
			Variables->Rooms[Offset].Effects[5]		= (UINT)(*((WORD *)(Default+0x38C)));
			Variables->Rooms[Offset].TorchIntensity		= (LONG)(*((BYTE *)(Default+0x36E)));
			Variables->Rooms[Offset].TorchLightRad		= (LONG)(*((WORD *)(Default+0x3C3)));
			Variables->Rooms[Offset].TorchLightH		= (LONG)(*((WORD *)(Default+0x380)));
			Variables->Rooms[Offset].TorchLightR		= (BYTE)(*((BYTE *)(Default+0x397)));
			Variables->Rooms[Offset].TorchLightG		= (BYTE)(*((BYTE *)(Default+0x398)));
			Variables->Rooms[Offset].TorchLightB		= (BYTE)(*((BYTE *)(Default+0x399)));
			}
		}

	if (Flags&MGRF_DOORS)
		{
		for (i = 0; GDOrder[i] != (DWORD *)-1; i += 2)
			{
			if (GDOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GDOrder[i];
			Default = (BYTE *)GDOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Doors[Offset].Cost			= (LONG)(*((WORD *)(Default+0x22E)));
			Variables->Doors[Offset].BuildPoints		= (LONG)(*((WORD *)(Default+0x234)));
			Variables->Doors[Offset].Health			= (LONG)(*((WORD *)(Default+0x22C)));
			Variables->Doors[Offset].HealthGain		= (LONG)(*((WORD *)(Default+0x21C)));
			Variables->Doors[Offset].ManaCost		= (LONG)(*((WORD *)(Default+0x238)));
			Variables->Doors[Offset].Material		= (UINT)(*((BYTE *)(Default+0x226)));
			Variables->Doors[Offset].DeathEffect		= (UINT)(*((WORD *)(Default+0x232)));
			Variables->Doors[Offset].Crate			= (UINT)(*((BYTE *)(Default+0x246)));
			Variables->Doors[Offset].Key			= (UINT)(*((BYTE *)(Default+0x247)));
			}
		}

	if (Flags&MGRF_TRAPS)
		{
		for (i = 0; GAOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GAOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GAOrder[i];
			Default = (BYTE *)GAOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Traps[Offset].Health			= (LONG)(*((WORD *)(Default+0x1F0)));
			Variables->Traps[Offset].HealthGain		= (LONG)(*((WORD *)(Default+0x241)));
			Variables->Traps[Offset].ChargeTime		= (LONG)(*((LONG *)(Default+0x1CC)));
			Variables->Traps[Offset].RechargeTime		= (LONG)(*((LONG *)(Default+0x1C8)));
			Variables->Traps[Offset].GoldCost		= (LONG)(*((WORD *)(Default+0x1F2)));
			Variables->Traps[Offset].ManaCostToFire		= (LONG)(*((WORD *)(Default+0x1D4)));
			Variables->Traps[Offset].ManaUsage		= (LONG)(*((WORD *)(Default+0x202)));
			Variables->Traps[Offset].Threat			= (LONG)(*((WORD *)(Default+0x1EA)));
			Variables->Traps[Offset].ThreatDuration		= (LONG)(*((LONG *)(Default+0x1D0)));
			Variables->Traps[Offset].IdleEffectDelay	= (LONG)(*((LONG *)(Default+0x1D8)));
			Variables->Traps[Offset].ShotDelay		= (LONG)(*((LONG *)(Default+0x23D)));
			Variables->Traps[Offset].BuildPoints		= (LONG)(*((WORD *)(Default+0x1FA)));
			Variables->Traps[Offset].Invisible		= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x20)?TRUE:FALSE);
			Variables->Traps[Offset].RevealedWhenFired	= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x02)?TRUE:FALSE);
			Variables->Traps[Offset].RequiresMana		= (BOOL)(((*((BYTE *)(Default+0x1ED)))&0x01)?TRUE:FALSE);
			Variables->Traps[Offset].Disarmable		= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x10)?TRUE:FALSE);
			Variables->Traps[Offset].SolidObstacle		= (BOOL)(((*((BYTE *)(Default+0x1EE)))&0x02)?TRUE:FALSE);
			Variables->Traps[Offset].FPObstacle		= (BOOL)(((*((BYTE *)(Default+0x1EE)))&0x01)?TRUE:FALSE);
			Variables->Traps[Offset].Material		= (UINT)(*((BYTE *)(Default+0x22F)));
			Variables->Traps[Offset].PowerlessEffect	= (UINT)(*((WORD *)(Default+0x1F4)));
			Variables->Traps[Offset].IdleEffect		= (UINT)(*((WORD *)(Default+0x1F6)));
			Variables->Traps[Offset].DeathEffect		= (UINT)(*((WORD *)(Default+0x1F8)));
			Variables->Traps[Offset].Crate			= (UINT)(*((BYTE *)(Default+0x20E)));
			Variables->Traps[Offset].ShotType		= (UINT)(*((BYTE *)(Default+0x20D)));
			Variables->Traps[Offset].ShotData1		= (LONG)(*((WORD *)(Default+0x1E0)));
			Variables->Traps[Offset].ShotData2		= (LONG)(*((WORD *)(Default+0x1E4)));
			Variables->Traps[Offset].TriggerType		= (UINT)(*((BYTE *)(Default+0x20B)));
			Variables->Traps[Offset].TriggerData		= (LONG)(*((WORD *)(Default+0x1DC)));
			}
		}

	if (Flags&MGRF_SPELLS)
		{
		for (i = 0; GSOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GSOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GSOrder[i];
			Default = (BYTE *)GSOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Spells[Offset].ManaCost		= (LONG)(*((LONG *)(Default+0x11C)));
			Variables->Spells[Offset].ManaDrain		= (LONG)(*((WORD *)(Default+0x0E2)));
			Variables->Spells[Offset].RechargeTime		= (LONG)(*((LONG *)(Default+0x0CC)));
			Variables->Spells[Offset].HandAnim		= (UINT)(*((BYTE *)(Default+0x194)));
			Variables->Spells[Offset].NoGoHandAnim		= (UINT)(*((BYTE *)(Default+0x195)));
			Variables->Spells[Offset].CastRule		= (UINT)(*((BYTE *)(Default+0x0EF)));
			Variables->Spells[Offset].TargetRule		= (UINT)(*((BYTE *)(Default+0x0DA)));
			Variables->Spells[Offset].ShotType		= (UINT)(*((BYTE *)(Default+0x0F0)));
			Variables->Spells[Offset].ShotData1		= (LONG)(*((WORD *)(Default+0x0D0)));
			Variables->Spells[Offset].ShotData2		= (LONG)(*((WORD *)(Default+0x0D4)));
			Variables->Spells[Offset].UpgShotType		= (UINT)(*((BYTE *)(Default+0x113)));
			Variables->Spells[Offset].UpgShotData1		= (LONG)(*((WORD *)(Default+0x114)));
			Variables->Spells[Offset].UpgShotData2		= (LONG)(*((WORD *)(Default+0x118)));
			Variables->Spells[Offset].UpgRechargeTime	= (LONG)(*((WORD *)(Default+0x111)));
			Variables->Spells[Offset].Attacking		= (BOOL)(((*((BYTE *)(Default+0x0DC)))&0x04)?TRUE:FALSE);
			Variables->Spells[Offset].Defensive		= (BOOL)(((*((BYTE *)(Default+0x0DC)))&0x08)?TRUE:FALSE);
			}
		}

	if (Flags&MGRF_CRATTRS)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].ExpPtsForNextLevel	= (LONG)(*((WORD *)(Default+0x0F00)));
			Variables->Creatures[Offset].ExpPts		= (LONG)(*((WORD *)(Default+0x0F04)));
			Variables->Creatures[Offset].ExpPtsTraining	= (LONG)(*((WORD *)(Default+0x0F06)));
			Variables->Creatures[Offset].ResearchPts	= (LONG)(*((WORD *)(Default+0x0F08)));
			Variables->Creatures[Offset].ManufacturePts	= (LONG)(*((WORD *)(Default+0x0F0A)));
			Variables->Creatures[Offset].Height		= (LONG)(*((WORD *)(Default+0x0EA4)));
			Variables->Creatures[Offset].EyeHeight		= (LONG)(*((WORD *)(Default+0x0EB0)));
			Variables->Creatures[Offset].DistanceCanSee	= (LONG)(*((WORD *)(Default+0x0EC8)));
			Variables->Creatures[Offset].DistanceCanHear	= (LONG)(*((WORD *)(Default+0x0ECC)));
			Variables->Creatures[Offset].Health		= (LONG)(*((WORD *)(Default+0x0F0C)));
			Variables->Creatures[Offset].HealthFromChicken	= (LONG)(*((WORD *)(Default+0x0F0E)));
			Variables->Creatures[Offset].SlapDamage		= (LONG)(*((WORD *)(Default+0x0F16)));
			Variables->Creatures[Offset].Fear		= (LONG)(*((WORD *)(Default+0x0F10)));
			Variables->Creatures[Offset].Threat		= (LONG)(*((WORD *)(Default+0x0F12)));
			Variables->Creatures[Offset].PrayerMana		= (LONG)(*((WORD *)(Default+0x0F18)));
			Variables->Creatures[Offset].Pay		= (LONG)(*((WORD *)(Default+0x0F1C)));
			Variables->Creatures[Offset].InitialGoldHeld	= (LONG)(*((WORD *)(Default+0x0F34)));
			Variables->Creatures[Offset].MaxGoldHeld	= (LONG)(*((WORD *)(Default+0x0F1E)));
			Variables->Creatures[Offset].Speed		= (LONG)(*((WORD *)(Default+0x0EB4)));
			Variables->Creatures[Offset].RunSpeed		= (LONG)(*((WORD *)(Default+0x0EB8)));
			Variables->Creatures[Offset].AngerNoLair	= (LONG)(*((WORD *)(Default+0x0F28)));
			Variables->Creatures[Offset].AngerNoFood	= (LONG)(*((WORD *)(Default+0x0F2A)));
			Variables->Creatures[Offset].AngerNoPay		= (LONG)(*((WORD *)(Default+0x0F2C)));
			Variables->Creatures[Offset].AngerNoWork	= (LONG)(*((WORD *)(Default+0x0F2E)));
			Variables->Creatures[Offset].AngerSlap		= (LONG)(*((WORD *)(Default+0x0F30)));
			Variables->Creatures[Offset].AngerInHand	= (LONG)(*((WORD *)(Default+0x0F32)));
			Variables->Creatures[Offset].TimeAwake		= (LONG)(*((WORD *)(Default+0x0EC0)));
			Variables->Creatures[Offset].SleepDuration	= (LONG)(*((WORD *)(Default+0x0EC4)));
			Variables->Creatures[Offset].StunDuration	= (LONG)(*((WORD *)(Default+0x0ED0)));
			Variables->Creatures[Offset].GuardDuration	= (LONG)(*((WORD *)(Default+0x0ED4)));
			Variables->Creatures[Offset].IdleDuration	= (LONG)(*((WORD *)(Default+0x0ED8)));
			Variables->Creatures[Offset].SlapDuration	= (LONG)(*((WORD *)(Default+0x0EDC)));
			Variables->Creatures[Offset].PossessionManaCost	= (LONG)(*((WORD *)(Default+0x0EE8)));
			Variables->Creatures[Offset].OwnLandHealthInc	= (LONG)(*((WORD *)(Default+0x0EEA)));
			Variables->Creatures[Offset].TortureHealthCng	= (LONG)(*((WORD *)(Default+0x107F)));
			Variables->Creatures[Offset].TortureMoodCng	= (LONG)(*((WORD *)(Default+0x1081)));
			Variables->Creatures[Offset].HungerRate		= (LONG)(*((LONG *)(Default+0x0EBC)));
			Variables->Creatures[Offset].DecomposeValue	= (LONG)(*((WORD *)(Default+0x0F22)));
			}
		}

	if (Flags&MGRF_CRPARAMS)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].ArmourType		= (UINT)(*((BYTE *)(Default+0x0F76)));
			Variables->Creatures[Offset].FightStyle		= (UINT)(*((BYTE *)(Default+0x0F03)));
			Variables->Creatures[Offset].JobClass		= (UINT)(*((BYTE *)(Default+0x0F02)));
			Variables->Creatures[Offset].LairObject		= (UINT)(*((BYTE *)(Default+0x0F52)));
			Variables->Creatures[Offset].SlapEffect		= (UINT)(*((WORD *)(Default+0x0F3E)));
			Variables->Creatures[Offset].HealthEffect	= (UINT)(*((WORD *)(Default+0x0CFD)));
			Variables->Creatures[Offset].EntranceEffect	= (UINT)(*((WORD *)(Default+0x0F36)));
			Variables->Creatures[Offset].DeathEffect	= (UINT)(*((WORD *)(Default+0x0F40)));
			Variables->Creatures[Offset].Attract1Type	= (UINT)(*((WORD *)(Default+0x0DAC)));
			Variables->Creatures[Offset].Attract1ID		= (UINT)(*((WORD *)(Default+0x0DB0)));
			Variables->Creatures[Offset].Attract2Type	= (UINT)(*((WORD *)(Default+0x0DB4)));
			Variables->Creatures[Offset].Attract2ID		= (UINT)(*((WORD *)(Default+0x0DB8)));
			Variables->Creatures[Offset].Attract2Data	= (LONG)(*((WORD *)(Default+0x0DBA)));
			}
		}

	if (Flags&MGRF_CRFLAGS)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].CanBeHypnotized	= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x01)?TRUE:FALSE);
			Variables->Creatures[Offset].DoNotTrain		= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x20)?TRUE:FALSE);
			Variables->Creatures[Offset].CanBePicked	= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x02)?TRUE:FALSE);
			Variables->Creatures[Offset].CanBeSlapped	= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x04)?TRUE:FALSE);
			Variables->Creatures[Offset].CanWalkWater	= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x10)?TRUE:FALSE);
			Variables->Creatures[Offset].CanWalkLava	= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x20)?TRUE:FALSE);
			Variables->Creatures[Offset].ImmuneTurncoat	= (BOOL)(((*((BYTE *)(Default+0x0EFD)))&0x02)?TRUE:FALSE);
			Variables->Creatures[Offset].DeadBody		= (BOOL)(((*((BYTE *)(Default+0x0EFD)))&0x80)?TRUE:FALSE);
			Variables->Creatures[Offset].ImmuneChicken	= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x02)?TRUE:FALSE);
			Variables->Creatures[Offset].Fearless		= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x04)?TRUE:FALSE);
			Variables->Creatures[Offset].NoEnemies		= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x01)?TRUE:FALSE);
			Variables->Creatures[Offset].FreeFriends	= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x02)?TRUE:FALSE);
			Variables->Creatures[Offset].RevealTraps	= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x04)?TRUE:FALSE);
			Variables->Creatures[Offset].RandomInvader	= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x40)?TRUE:FALSE);
			}
		}

	if (Flags&MGRF_CRATTACKS)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].Shot1ID		= (UINT)(*((BYTE *)(Default+0x0DDA)));
			Variables->Creatures[Offset].Shot1Delay		= (LONG)(*((LONG *)(Default+0x0DD4)));
			Variables->Creatures[Offset].Shot1Level		= (LONG)(*((BYTE *)(Default+0x0DDB)));
			Variables->Creatures[Offset].Shot1X		= (LONG)(*((LONG *)(Default+0x0DC4)));
			Variables->Creatures[Offset].Shot1Y		= (LONG)(*((LONG *)(Default+0x0DC8)));
			Variables->Creatures[Offset].Shot1Z		= (LONG)(*((LONG *)(Default+0x0DCC)));
			Variables->Creatures[Offset].Shot2ID		= (UINT)(*((BYTE *)(Default+0x0DF2)));
			Variables->Creatures[Offset].Shot2Delay		= (LONG)(*((LONG *)(Default+0x0DEC)));
			Variables->Creatures[Offset].Shot2Level		= (LONG)(*((BYTE *)(Default+0x0DF3)));
			Variables->Creatures[Offset].Shot2X		= (LONG)(*((LONG *)(Default+0x0DDC)));
			Variables->Creatures[Offset].Shot2Y		= (LONG)(*((LONG *)(Default+0x0DE0)));
			Variables->Creatures[Offset].Shot2Z		= (LONG)(*((LONG *)(Default+0x0DE4)));
			Variables->Creatures[Offset].Shot3ID		= (UINT)(*((BYTE *)(Default+0x0E0A)));
			Variables->Creatures[Offset].Shot3Delay		= (LONG)(*((LONG *)(Default+0x0E04)));
			Variables->Creatures[Offset].Shot3Level		= (LONG)(*((BYTE *)(Default+0x0E0B)));
			Variables->Creatures[Offset].Shot3X		= (LONG)(*((LONG *)(Default+0x0DF4)));
			Variables->Creatures[Offset].Shot3Y		= (LONG)(*((LONG *)(Default+0x0DF8)));
			Variables->Creatures[Offset].Shot3Z		= (LONG)(*((LONG *)(Default+0x0DFC)));
			Variables->Creatures[Offset].MeleeID		= (UINT)(*((BYTE *)(Default+0x0F50)));
			Variables->Creatures[Offset].RechargeTime	= (LONG)(*((LONG *)(Default+0x0EF8)));
			Variables->Creatures[Offset].Range		= (LONG)(*((LONG *)(Default+0x0EEC)));
			Variables->Creatures[Offset].Dammage		= (LONG)(*((WORD *)(Default+0x0F14)));
			Variables->Creatures[Offset].Delay		= (LONG)(*((LONG *)(Default+0x0CF9)));
			}
		}

	if (Flags&MGRF_CRRESISTANCES)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].Res1		= (UINT)(*((BYTE *)(Default+0x0E0C)));
			Variables->Creatures[Offset].Res1Rate		= (LONG)(*((BYTE *)(Default+0x0E0D)));
			Variables->Creatures[Offset].Res2		= (UINT)(*((BYTE *)(Default+0x0E0E)));
			Variables->Creatures[Offset].Res2Rate		= (LONG)(*((BYTE *)(Default+0x0E0F)));
			Variables->Creatures[Offset].Res3		= (UINT)(*((BYTE *)(Default+0x0E10)));
			Variables->Creatures[Offset].Res3Rate		= (LONG)(*((BYTE *)(Default+0x0E11)));
			Variables->Creatures[Offset].Res4		= (UINT)(*((BYTE *)(Default+0x0E12)));
			Variables->Creatures[Offset].Res4Rate		= (LONG)(*((BYTE *)(Default+0x0E13)));
			}
		}

	if (Flags&MGRF_CRPOSSESSION)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].SpecialAb1		= (UINT)(*((BYTE *)(Default+0x0F46)));
			Variables->Creatures[Offset].SpecialAb1Data	= (LONG)(*((WORD *)(Default+0x14E9)));
			Variables->Creatures[Offset].SpecialAb2		= (UINT)(*((BYTE *)(Default+0x0F47)));
			Variables->Creatures[Offset].SpecialAb2Data	= (LONG)(*((WORD *)(Default+0x14ED)));
			Variables->Creatures[Offset].Melee1		= (UINT)(*((BYTE *)(Default+0x0F42)));
			Variables->Creatures[Offset].Melee2		= (UINT)(*((BYTE *)(Default+0x0F43)));
			Variables->Creatures[Offset].Special1		= (UINT)(*((BYTE *)(Default+0x1025)));
			Variables->Creatures[Offset].Special2		= (UINT)(*((BYTE *)(Default+0x1026)));
			Variables->Creatures[Offset].Spell		= (UINT)(*((BYTE *)(Default+0x0F45)));
			Variables->Creatures[Offset].GammaEffect	= (UINT)(*((BYTE *)(Default+0x0D3C)));
			Variables->Creatures[Offset].WaddleScale	= (LONG)(*((WORD *)(Default+0x0DBC)));
			Variables->Creatures[Offset].OscillateScale	= (LONG)(*((WORD *)(Default+0x0DC0)));
			Variables->Creatures[Offset].WalkScale		= (LONG)(*((BYTE *)(Default+0x0D3D)));
			Variables->Creatures[Offset].CameraRolls	= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x10)?TRUE:FALSE);
			}
		}

	if (Flags&MGRF_CRJOBS)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].HJobs[0]		= (UINT)(*((LONG *)(Default+0x0E14)));
			Variables->Creatures[Offset].HJobsMood[0]	= (LONG)(*((WORD *)(Default+0x0E18)));
			Variables->Creatures[Offset].HJobsMana[0]	= (LONG)(*((WORD *)(Default+0x0E1A)));
			Variables->Creatures[Offset].HJobsChance[0]	= (LONG)(*((BYTE *)(Default+0x0E1C)));
			Variables->Creatures[Offset].HJobs[1]		= (UINT)(*((LONG *)(Default+0x0E20)));
			Variables->Creatures[Offset].HJobsMood[1]	= (LONG)(*((WORD *)(Default+0x0E24)));
			Variables->Creatures[Offset].HJobsMana[1]	= (LONG)(*((WORD *)(Default+0x0E26)));
			Variables->Creatures[Offset].HJobsChance[1]	= (LONG)(*((BYTE *)(Default+0x0E28)));
			Variables->Creatures[Offset].HJobs[2]		= (UINT)(*((LONG *)(Default+0x0E2C)));
			Variables->Creatures[Offset].HJobsMood[2]	= (LONG)(*((WORD *)(Default+0x0E30)));
			Variables->Creatures[Offset].HJobsMana[2]	= (LONG)(*((WORD *)(Default+0x0E32)));
			Variables->Creatures[Offset].HJobsChance[2]	= (LONG)(*((BYTE *)(Default+0x0E34)));
			Variables->Creatures[Offset].UJobs[0]		= (UINT)(*((LONG *)(Default+0x0E38)));
			Variables->Creatures[Offset].UJobsMood[0]	= (LONG)(*((WORD *)(Default+0x0E3C)));
			Variables->Creatures[Offset].UJobsMana[0]	= (LONG)(*((WORD *)(Default+0x0E3E)));
			Variables->Creatures[Offset].UJobsChance[0]	= (LONG)(*((BYTE *)(Default+0x0E40)));
			Variables->Creatures[Offset].UJobs[1]		= (UINT)(*((LONG *)(Default+0x0E44)));
			Variables->Creatures[Offset].UJobsMood[1]	= (LONG)(*((WORD *)(Default+0x0E48)));
			Variables->Creatures[Offset].UJobsMana[1]	= (LONG)(*((WORD *)(Default+0x0E4A)));
			Variables->Creatures[Offset].UJobsChance[1]	= (LONG)(*((BYTE *)(Default+0x0E4C)));
			Variables->Creatures[Offset].AJobs[0]		= (UINT)(*((LONG *)(Default+0x0E50)));
			Variables->Creatures[Offset].AJobsMood[0]	= (LONG)(*((WORD *)(Default+0x0E54)));
			Variables->Creatures[Offset].AJobsMana[0]	= (LONG)(*((WORD *)(Default+0x0E56)));
			Variables->Creatures[Offset].AJobsChance[0]	= (LONG)(*((BYTE *)(Default+0x0E58)));
			Variables->Creatures[Offset].AJobs[1]		= (UINT)(*((LONG *)(Default+0x0E5C)));
			Variables->Creatures[Offset].AJobsMood[1]	= (LONG)(*((WORD *)(Default+0x0E60)));
			Variables->Creatures[Offset].AJobsMana[1]	= (LONG)(*((WORD *)(Default+0x0E62)));
			Variables->Creatures[Offset].AJobsChance[1]	= (LONG)(*((BYTE *)(Default+0x0E64)));
			Variables->Creatures[Offset].AJobs[2]		= (UINT)(*((LONG *)(Default+0x0E68)));
			Variables->Creatures[Offset].AJobsMood[2]	= (LONG)(*((WORD *)(Default+0x0E6C)));
			Variables->Creatures[Offset].AJobsMana[2]	= (LONG)(*((WORD *)(Default+0x0E6E)));
			Variables->Creatures[Offset].AJobsChance[2]	= (LONG)(*((BYTE *)(Default+0x0E70)));
			Variables->Creatures[Offset].NJobs[0]		= (UINT)(*((LONG *)(Default+0x0E74)));
			Variables->Creatures[Offset].NJobs[1]		= (UINT)(*((LONG *)(Default+0x0E78)));
			Variables->Creatures[Offset].NJobs[2]		= (UINT)(*((LONG *)(Default+0x0E7C)));
			Variables->Creatures[Offset].NJobs[3]		= (UINT)(*((LONG *)(Default+0x0E80)));
			}
		}

	if (Flags&MGRF_CRLIGHT)
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->Creatures[Offset].LightX		= (LONG)(*((LONG *)(Default+0x0D94)));
			Variables->Creatures[Offset].LightY		= (LONG)(*((LONG *)(Default+0x0D98)));
			Variables->Creatures[Offset].LightZ		= (LONG)(*((LONG *)(Default+0x0D9C)));
			Variables->Creatures[Offset].LightRadius	= (LONG)(*((WORD *)(Default+0x0DA0)));
			Variables->Creatures[Offset].LightR		= (BYTE)(*((BYTE *)(Default+0x0DA8)));
			Variables->Creatures[Offset].LightG		= (BYTE)(*((BYTE *)(Default+0x0DA9)));
			Variables->Creatures[Offset].LightB		= (BYTE)(*((BYTE *)(Default+0x0DAA)));
			Variables->Creatures[Offset].LightFlicker	= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x02)?TRUE:FALSE);
			Variables->Creatures[Offset].LightPulse		= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x04)?TRUE:FALSE);
			Variables->Creatures[Offset].LightPlayer	= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x80)?TRUE:FALSE);
			}
		}

	if (Flags&MGRF_CREATURESPELLS)
		{
		for (i = 0; GOOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GOOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GOOrder[i];
			Default = (BYTE *)GOOrder[i+1];
			if ((UseOffset != -1)&&(UseOffset != Offset)) continue;

			Variables->CrSpells[Offset].RechargeTime	= (LONG)(*((WORD *)(Default+0x00EA)));
			Variables->CrSpells[Offset].Range		= (LONG)(*((WORD *)(Default+0x00D0)));
			Variables->CrSpells[Offset].Shot		= (UINT)(*((BYTE *)(Default+0x00E7)));
			Variables->CrSpells[Offset].ShotData1		= (LONG)(*((WORD *)(Default+0x00C8)));
			Variables->CrSpells[Offset].ShotData2		= (LONG)(*((WORD *)(Default+0x00CC)));
			Variables->CrSpells[Offset].Attacking		= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x01)?TRUE:FALSE);
			Variables->CrSpells[Offset].Creature		= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x02)?TRUE:FALSE);
			Variables->CrSpells[Offset].Defensive		= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x04)?TRUE:FALSE);
			Variables->CrSpells[Offset].CastSelf		= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x80)?TRUE:FALSE);
			Variables->CrSpells[Offset].CastOthers		= (BOOL)(((*((BYTE *)(Default+0x00D5)))&0x01)?TRUE:FALSE);
			Variables->CrSpells[Offset].CastCreatures	= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x08)?TRUE:FALSE);
			}
		}

	return;
}


// «»»» Test si les variables ont été changées ««««««««««««««««««««««««««»

int Properties_GlobalChanged(GLOBALVARS *Variables, DWORD Flags)
{
	BYTE	*Default;
	int	 Offset;
	int	 i;

	if (Flags&MGRF_TERRAINS)
		{
		for (i = 0; GTOrder[i] != (DWORD *)-1; i += 2)
			{
			if (GTOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GTOrder[i];
			Default = (BYTE *)GTOrder[i+1];
			if (Variables->Terrains[Offset].MaxHealthType	!= (UINT)(*((BYTE *)(Default+0x1D9)))) return(1);
			if (Variables->Terrains[Offset].MaxHealthEffect	!= (UINT)(*((WORD *)(Default+0x1A4)))) return(1);
			if (Variables->Terrains[Offset].DestroyedType	!= (UINT)(*((BYTE *)(Default+0x1DA)))) return(1);
			if (Variables->Terrains[Offset].DestroyedEffect	!= (UINT)(*((WORD *)(Default+0x1A6)))) return(1);
			if (Variables->Terrains[Offset].Damage		!= (LONG)(*((short *)(Default+0x194)))) return(1);
			if (Variables->Terrains[Offset].GoldValue	!= (LONG)(*((WORD *)(Default+0x19A)))) return(1);
			if (Variables->Terrains[Offset].MaxHealth	!= (LONG)(*((WORD *)(Default+0x1FF)))) return(1);
			if (Variables->Terrains[Offset].StartingHealth	!= (LONG)(*((WORD *)(Default+0x1D7)))) return(1);
			if (Variables->Terrains[Offset].ManaGain	!= (LONG)(*((WORD *)(Default+0x19C)))) return(1);
			if (Variables->Terrains[Offset].MaxManaGain	!= (LONG)(*((WORD *)(Default+0x19E)))) return(1);
			if (Variables->Terrains[Offset].WibbleH		!= (LONG)(*((BYTE *)(Default+0x1CE)))) return(1);
			if (Variables->Terrains[Offset].WibbleV		!= (LONG)(*((BYTE *)(Default+0x1D2)))) return(1);
			if (Variables->Terrains[Offset].Visible		!= (BOOL)(((*(Default+0x193))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->Terrains[Offset].TerrainLight	!= (BOOL)(((*(Default+0x193))&0x10)?TRUE:FALSE)) return(1);
			if (Variables->Terrains[Offset].AmbientLight	!= (BOOL)(((*(Default+0x193))&0x20)?TRUE:FALSE)) return(1);
			if (Variables->Terrains[Offset].TerrainLightR	!= (BYTE)(*((BYTE *)(Default+0x01DB)))) return(1);
			if (Variables->Terrains[Offset].TerrainLightG	!= (BYTE)(*((BYTE *)(Default+0x01DC)))) return(1);
			if (Variables->Terrains[Offset].TerrainLightB	!= (BYTE)(*((BYTE *)(Default+0x01DD)))) return(1);
			if (Variables->Terrains[Offset].AmbientLightR	!= (BYTE)(*((BYTE *)(Default+0x0201)))) return(1);
			if (Variables->Terrains[Offset].AmbientLightG	!= (BYTE)(*((BYTE *)(Default+0x0202)))) return(1);
			if (Variables->Terrains[Offset].AmbientLightB	!= (BYTE)(*((BYTE *)(Default+0x0203)))) return(1);
			if (Variables->Terrains[Offset].TerrainLightH	!= (LONG)(*((LONG *)(Default+0x018C)))) return(1);
			}
		}

	if (Flags&MGRF_ROOMS)
		{
		for (i = 0; GROrder[i] != (DWORD *)-1; i += 2)
			{
			if (GROrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GROrder[i];
			Default = (BYTE *)GROrder[i+1];

			if (Variables->Rooms[Offset].Cost		!= (LONG)(*((WORD *)(Default+0x378)))) return(1);
			if (Variables->Rooms[Offset].Objects[0]		!= (UINT)(*((BYTE *)(Default+0x39A)))) return(1);
			if (Variables->Rooms[Offset].Objects[1]		!= (UINT)(*((BYTE *)(Default+0x39B)))) return(1);
			if (Variables->Rooms[Offset].Objects[2]		!= (UINT)(*((BYTE *)(Default+0x39C)))) return(1);
			if (Variables->Rooms[Offset].Objects[3]		!= (UINT)(*((BYTE *)(Default+0x39D)))) return(1);
			if (Variables->Rooms[Offset].Objects[4]		!= (UINT)(*((BYTE *)(Default+0x39E)))) return(1);
			if (Variables->Rooms[Offset].Objects[5]		!= (UINT)(*((BYTE *)(Default+0x39F)))) return(1);
			if (Variables->Rooms[Offset].BattleEffect	!= (UINT)(*((WORD *)(Default+0x37A)))) return(1);
			if (Variables->Rooms[Offset].Effects[0]		!= (UINT)(*((WORD *)(Default+0x382)))) return(1);
			if (Variables->Rooms[Offset].Effects[1]		!= (UINT)(*((WORD *)(Default+0x384)))) return(1);
			if (Variables->Rooms[Offset].Effects[2]		!= (UINT)(*((WORD *)(Default+0x386)))) return(1);
			if (Variables->Rooms[Offset].Effects[3]		!= (UINT)(*((WORD *)(Default+0x388)))) return(1);
			if (Variables->Rooms[Offset].Effects[4]		!= (UINT)(*((WORD *)(Default+0x38A)))) return(1);
			if (Variables->Rooms[Offset].Effects[5]		!= (UINT)(*((WORD *)(Default+0x38C)))) return(1);
			if (Variables->Rooms[Offset].TorchLightRad	!= (LONG)(*((WORD *)(Default+0x3C3)))) return(1);
			if (Variables->Rooms[Offset].TorchIntensity	!= (LONG)(*((BYTE *)(Default+0x36E)))) return(1);
			if (Variables->Rooms[Offset].TorchLightH	!= (LONG)(*((WORD *)(Default+0x380)))) return(1);
			if (Variables->Rooms[Offset].TorchLightR	!= (BYTE)(*((BYTE *)(Default+0x397)))) return(1);
			if (Variables->Rooms[Offset].TorchLightG	!= (BYTE)(*((BYTE *)(Default+0x398)))) return(1);
			if (Variables->Rooms[Offset].TorchLightB	!= (BYTE)(*((BYTE *)(Default+0x399)))) return(1);
			}
		}

	if (Flags&MGRF_DOORS)
		{
		for (i = 0; GDOrder[i] != (DWORD *)-1; i += 2)
			{
			if (GDOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GDOrder[i];
			Default = (BYTE *)GDOrder[i+1];

			if (Variables->Doors[Offset].Cost		!= (LONG)(*((WORD *)(Default+0x22E)))) return(1);
			if (Variables->Doors[Offset].BuildPoints	!= (LONG)(*((WORD *)(Default+0x234)))) return(1);
			if (Variables->Doors[Offset].Health		!= (LONG)(*((WORD *)(Default+0x22C)))) return(1);
			if (Variables->Doors[Offset].HealthGain		!= (LONG)(*((WORD *)(Default+0x21C)))) return(1);
			if (Variables->Doors[Offset].ManaCost		!= (LONG)(*((WORD *)(Default+0x238)))) return(1);
			if (Variables->Doors[Offset].Material		!= (UINT)(*((BYTE *)(Default+0x226)))) return(1);
			if (Variables->Doors[Offset].DeathEffect	!= (UINT)(*((WORD *)(Default+0x232)))) return(1);
			if (Variables->Doors[Offset].Crate		!= (UINT)(*((BYTE *)(Default+0x246)))) return(1);
			if (Variables->Doors[Offset].Key		!= (UINT)(*((BYTE *)(Default+0x247)))) return(1);
			}
		}

	if (Flags&MGRF_TRAPS)
		{
		for (i = 0; GAOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GAOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GAOrder[i];
			Default = (BYTE *)GAOrder[i+1];

			if (Variables->Traps[Offset].Health		!= (LONG)(*((WORD *)(Default+0x1F0)))) return(1);
			if (Variables->Traps[Offset].HealthGain		!= (LONG)(*((WORD *)(Default+0x241)))) return(1);
			if (Variables->Traps[Offset].ChargeTime		!= (LONG)(*((LONG *)(Default+0x1CC)))) return(1);
			if (Variables->Traps[Offset].RechargeTime	!= (LONG)(*((LONG *)(Default+0x1C8)))) return(1);
			if (Variables->Traps[Offset].GoldCost		!= (LONG)(*((WORD *)(Default+0x1F2)))) return(1);
			if (Variables->Traps[Offset].ManaCostToFire	!= (LONG)(*((WORD *)(Default+0x1D4)))) return(1);
			if (Variables->Traps[Offset].ManaUsage		!= (LONG)(*((WORD *)(Default+0x202)))) return(1);
			if (Variables->Traps[Offset].Threat		!= (LONG)(*((WORD *)(Default+0x1EA)))) return(1);
			if (Variables->Traps[Offset].ThreatDuration	!= (LONG)(*((LONG *)(Default+0x1D0)))) return(1);
			if (Variables->Traps[Offset].IdleEffectDelay	!= (LONG)(*((LONG *)(Default+0x1D8)))) return(1);
			if (Variables->Traps[Offset].ShotDelay		!= (LONG)(*((LONG *)(Default+0x23D)))) return(1);
			if (Variables->Traps[Offset].BuildPoints	!= (LONG)(*((WORD *)(Default+0x1FA)))) return(1);
			if (Variables->Traps[Offset].Invisible		!= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x20)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].RevealedWhenFired	!= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].RequiresMana	!= (BOOL)(((*((BYTE *)(Default+0x1ED)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].Disarmable		!= (BOOL)(((*((BYTE *)(Default+0x1EC)))&0x10)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].SolidObstacle	!= (BOOL)(((*((BYTE *)(Default+0x1EE)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].FPObstacle		!= (BOOL)(((*((BYTE *)(Default+0x1EE)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->Traps[Offset].Material		!= (UINT)(*((BYTE *)(Default+0x22F)))) return(1);
			if (Variables->Traps[Offset].PowerlessEffect	!= (UINT)(*((WORD *)(Default+0x1F4)))) return(1);
			if (Variables->Traps[Offset].IdleEffect		!= (UINT)(*((WORD *)(Default+0x1F6)))) return(1);
			if (Variables->Traps[Offset].DeathEffect	!= (UINT)(*((WORD *)(Default+0x1F8)))) return(1);
			if (Variables->Traps[Offset].Crate		!= (UINT)(*((BYTE *)(Default+0x20E)))) return(1);
			if (Variables->Traps[Offset].ShotType		!= (UINT)(*((BYTE *)(Default+0x20D)))) return(1);
			if (Variables->Traps[Offset].ShotData1		!= (LONG)(*((WORD *)(Default+0x1E0)))) return(1);
			if (Variables->Traps[Offset].ShotData2		!= (LONG)(*((WORD *)(Default+0x1E4)))) return(1);
			if (Variables->Traps[Offset].TriggerType	!= (UINT)(*((BYTE *)(Default+0x20B)))) return(1);
			if (Variables->Traps[Offset].TriggerData	!= (LONG)(*((WORD *)(Default+0x1DC)))) return(1);
			}
		}

	if (Flags&MGRF_SPELLS)
		{
		for (i = 0; GSOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GSOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GSOrder[i];
			Default = (BYTE *)GSOrder[i+1];

			if (Variables->Spells[Offset].ManaCost		!= (LONG)(*((LONG *)(Default+0x11C)))) return(1);
			if (Variables->Spells[Offset].ManaDrain		!= (LONG)(*((WORD *)(Default+0x0E2)))) return(1);
			if (Variables->Spells[Offset].RechargeTime	!= (LONG)(*((LONG *)(Default+0x0CC)))) return(1);
			if (Variables->Spells[Offset].HandAnim		!= (UINT)(*((BYTE *)(Default+0x194)))) return(1);
			if (Variables->Spells[Offset].NoGoHandAnim	!= (UINT)(*((BYTE *)(Default+0x195)))) return(1);
			if (Variables->Spells[Offset].CastRule		!= (UINT)(*((BYTE *)(Default+0x0EF)))) return(1);
			if (Variables->Spells[Offset].TargetRule	!= (UINT)(*((BYTE *)(Default+0x0DA)))) return(1);
			if (Variables->Spells[Offset].ShotType		!= (UINT)(*((BYTE *)(Default+0x0F0)))) return(1);
			if (Variables->Spells[Offset].ShotData1		!= (LONG)(*((WORD *)(Default+0x0D0)))) return(1);
			if (Variables->Spells[Offset].ShotData2		!= (LONG)(*((WORD *)(Default+0x0D4)))) return(1);
			if (Variables->Spells[Offset].UpgShotType	!= (UINT)(*((BYTE *)(Default+0x113)))) return(1);
			if (Variables->Spells[Offset].UpgShotData1	!= (LONG)(*((WORD *)(Default+0x114)))) return(1);
			if (Variables->Spells[Offset].UpgShotData2	!= (LONG)(*((WORD *)(Default+0x118)))) return(1);
			if (Variables->Spells[Offset].UpgRechargeTime	!= (LONG)(*((WORD *)(Default+0x111)))) return(1);
			if (Variables->Spells[Offset].Attacking		!= (BOOL)(((*((BYTE *)(Default+0x0DC)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->Spells[Offset].Defensive		!= (BOOL)(((*((BYTE *)(Default+0x0DC)))&0x08)?TRUE:FALSE)) return(1);
			}
		}

	if (Flags&(MGRF_CREATURES))
		{
		for (i = 0; GCOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GCOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GCOrder[i];
			Default = (BYTE *)GCOrder[i+1];

			//--- Attributes ---
			if (Variables->Creatures[Offset].ExpPtsForNextLevel	!= (LONG)(*((WORD *)(Default+0x0F00)))) return(1);
			if (Variables->Creatures[Offset].ExpPts			!= (LONG)(*((WORD *)(Default+0x0F04)))) return(1);
			if (Variables->Creatures[Offset].ExpPtsTraining		!= (LONG)(*((WORD *)(Default+0x0F06)))) return(1);
			if (Variables->Creatures[Offset].ResearchPts		!= (LONG)(*((WORD *)(Default+0x0F08)))) return(1);
			if (Variables->Creatures[Offset].ManufacturePts		!= (LONG)(*((WORD *)(Default+0x0F0A)))) return(1);
			if (Variables->Creatures[Offset].Height			!= (LONG)(*((WORD *)(Default+0x0EA4)))) return(1);
			if (Variables->Creatures[Offset].EyeHeight		!= (LONG)(*((WORD *)(Default+0x0EB0)))) return(1);
			if (Variables->Creatures[Offset].DistanceCanSee		!= (LONG)(*((WORD *)(Default+0x0EC8)))) return(1);
			if (Variables->Creatures[Offset].DistanceCanHear	!= (LONG)(*((WORD *)(Default+0x0ECC)))) return(1);
			if (Variables->Creatures[Offset].Health			!= (LONG)(*((WORD *)(Default+0x0F0C)))) return(1);
			if (Variables->Creatures[Offset].HealthFromChicken	!= (LONG)(*((WORD *)(Default+0x0F0E)))) return(1);
			if (Variables->Creatures[Offset].SlapDamage		!= (LONG)(*((WORD *)(Default+0x0F16)))) return(1);
			if (Variables->Creatures[Offset].Fear			!= (LONG)(*((WORD *)(Default+0x0F10)))) return(1);
			if (Variables->Creatures[Offset].Threat			!= (LONG)(*((WORD *)(Default+0x0F12)))) return(1);
			if (Variables->Creatures[Offset].PrayerMana		!= (LONG)(*((WORD *)(Default+0x0F18)))) return(1);
			if (Variables->Creatures[Offset].Pay			!= (LONG)(*((WORD *)(Default+0x0F1C)))) return(1);
			if (Variables->Creatures[Offset].InitialGoldHeld	!= (LONG)(*((WORD *)(Default+0x0F34)))) return(1);
			if (Variables->Creatures[Offset].MaxGoldHeld		!= (LONG)(*((WORD *)(Default+0x0F1E)))) return(1);
			if (Variables->Creatures[Offset].Speed			!= (LONG)(*((WORD *)(Default+0x0EB4)))) return(1);
			if (Variables->Creatures[Offset].RunSpeed		!= (LONG)(*((WORD *)(Default+0x0EB8)))) return(1);
			if (Variables->Creatures[Offset].AngerNoLair		!= (LONG)(*((WORD *)(Default+0x0F28)))) return(1);
			if (Variables->Creatures[Offset].AngerNoFood		!= (LONG)(*((WORD *)(Default+0x0F2A)))) return(1);
			if (Variables->Creatures[Offset].AngerNoPay		!= (LONG)(*((WORD *)(Default+0x0F2C)))) return(1);
			if (Variables->Creatures[Offset].AngerNoWork		!= (LONG)(*((WORD *)(Default+0x0F2E)))) return(1);
			if (Variables->Creatures[Offset].AngerSlap		!= (LONG)(*((WORD *)(Default+0x0F30)))) return(1);
			if (Variables->Creatures[Offset].AngerInHand		!= (LONG)(*((WORD *)(Default+0x0F32)))) return(1);
			if (Variables->Creatures[Offset].TimeAwake		!= (LONG)(*((WORD *)(Default+0x0EC0)))) return(1);
			if (Variables->Creatures[Offset].SleepDuration		!= (LONG)(*((WORD *)(Default+0x0EC4)))) return(1);
			if (Variables->Creatures[Offset].StunDuration		!= (LONG)(*((WORD *)(Default+0x0ED0)))) return(1);
			if (Variables->Creatures[Offset].GuardDuration		!= (LONG)(*((WORD *)(Default+0x0ED4)))) return(1);
			if (Variables->Creatures[Offset].IdleDuration		!= (LONG)(*((WORD *)(Default+0x0ED8)))) return(1);
			if (Variables->Creatures[Offset].SlapDuration		!= (LONG)(*((WORD *)(Default+0x0EDC)))) return(1);
			if (Variables->Creatures[Offset].PossessionManaCost	!= (LONG)(*((WORD *)(Default+0x0EE8)))) return(1);
			if (Variables->Creatures[Offset].OwnLandHealthInc	!= (LONG)(*((WORD *)(Default+0x0EEA)))) return(1);
			if (Variables->Creatures[Offset].TortureHealthCng	!= (LONG)(*((WORD *)(Default+0x107F)))) return(1);
			if (Variables->Creatures[Offset].TortureMoodCng		!= (LONG)(*((WORD *)(Default+0x1081)))) return(1);
			if (Variables->Creatures[Offset].HungerRate		!= (LONG)(*((LONG *)(Default+0x0EBC)))) return(1);
			if (Variables->Creatures[Offset].DecomposeValue		!= (LONG)(*((WORD *)(Default+0x0F22)))) return(1);
			//--- Flags ---
			if (Variables->Creatures[Offset].CanBeHypnotized	!= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].DoNotTrain		!= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x20)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].CanBePicked		!= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].CanBeSlapped		!= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].CanWalkWater		!= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x10)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].CanWalkLava		!= (BOOL)(((*((BYTE *)(Default+0x0EFC)))&0x20)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].ImmuneTurncoat		!= (BOOL)(((*((BYTE *)(Default+0x0EFD)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].DeadBody		!= (BOOL)(((*((BYTE *)(Default+0x0EFD)))&0x80)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].ImmuneChicken		!= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].Fearless		!= (BOOL)(((*((BYTE *)(Default+0x0EFE)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].NoEnemies		!= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].FreeFriends		!= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].RevealTraps		!= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].RandomInvader		!= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x40)?TRUE:FALSE)) return(1);
			//--- Params ---
			if (Variables->Creatures[Offset].ArmourType		!= (UINT)(*((BYTE *)(Default+0x0F76)))) return(1);
			if (Variables->Creatures[Offset].FightStyle		!= (UINT)(*((BYTE *)(Default+0x0F03)))) return(1);
			if (Variables->Creatures[Offset].JobClass		!= (UINT)(*((BYTE *)(Default+0x0F02)))) return(1);
			if (Variables->Creatures[Offset].LairObject		!= (UINT)(*((BYTE *)(Default+0x0F52)))) return(1);
			if (Variables->Creatures[Offset].SlapEffect		!= (UINT)(*((WORD *)(Default+0x0F3E)))) return(1);
			if (Variables->Creatures[Offset].HealthEffect		!= (UINT)(*((WORD *)(Default+0x0CFD)))) return(1);
			if (Variables->Creatures[Offset].EntranceEffect		!= (UINT)(*((WORD *)(Default+0x0F36)))) return(1);
			if (Variables->Creatures[Offset].DeathEffect		!= (UINT)(*((WORD *)(Default+0x0F40)))) return(1);
			if (Variables->Creatures[Offset].Attract1Type		!= (UINT)(*((WORD *)(Default+0x0DAC)))) return(1);
			if (Variables->Creatures[Offset].Attract1ID		!= (UINT)(*((WORD *)(Default+0x0DB0)))) return(1);
			if (Variables->Creatures[Offset].Attract2Type		!= (UINT)(*((WORD *)(Default+0x0DB4)))) return(1);
			if (Variables->Creatures[Offset].Attract2ID		!= (UINT)(*((WORD *)(Default+0x0DB8)))) return(1);
			if (Variables->Creatures[Offset].Attract2Data		!= (LONG)(*((WORD *)(Default+0x0DBA)))) return(1);
			//--- Attacks ---
			if (Variables->Creatures[Offset].Shot1ID		!= (UINT)(*((BYTE *)(Default+0x0DDA)))) return(1);
			if (Variables->Creatures[Offset].Shot1Delay		!= (LONG)(*((LONG *)(Default+0x0DD4)))) return(1);
			if (Variables->Creatures[Offset].Shot1Level		!= (LONG)(*((BYTE *)(Default+0x0DDB)))) return(1);
			if (Variables->Creatures[Offset].Shot1X			!= (LONG)(*((LONG *)(Default+0x0DC4)))) return(1);
			if (Variables->Creatures[Offset].Shot1Y			!= (LONG)(*((LONG *)(Default+0x0DC8)))) return(1);
			if (Variables->Creatures[Offset].Shot1Z			!= (LONG)(*((LONG *)(Default+0x0DCC)))) return(1);
			if (Variables->Creatures[Offset].Shot2ID		!= (UINT)(*((BYTE *)(Default+0x0DF2)))) return(1);
			if (Variables->Creatures[Offset].Shot2Delay		!= (LONG)(*((LONG *)(Default+0x0DEC)))) return(1);
			if (Variables->Creatures[Offset].Shot2Level		!= (LONG)(*((BYTE *)(Default+0x0DF3)))) return(1);
			if (Variables->Creatures[Offset].Shot2X			!= (LONG)(*((LONG *)(Default+0x0DDC)))) return(1);
			if (Variables->Creatures[Offset].Shot2Y			!= (LONG)(*((LONG *)(Default+0x0DE0)))) return(1);
			if (Variables->Creatures[Offset].Shot2Z			!= (LONG)(*((LONG *)(Default+0x0DE4)))) return(1);
			if (Variables->Creatures[Offset].Shot3ID		!= (UINT)(*((BYTE *)(Default+0x0E0A)))) return(1);
			if (Variables->Creatures[Offset].Shot3Delay		!= (LONG)(*((LONG *)(Default+0x0E04)))) return(1);
			if (Variables->Creatures[Offset].Shot3Level		!= (LONG)(*((BYTE *)(Default+0x0E0B)))) return(1);
			if (Variables->Creatures[Offset].Shot3X			!= (LONG)(*((LONG *)(Default+0x0DF4)))) return(1);
			if (Variables->Creatures[Offset].Shot3Y			!= (LONG)(*((LONG *)(Default+0x0DF8)))) return(1);
			if (Variables->Creatures[Offset].Shot3Z			!= (LONG)(*((LONG *)(Default+0x0DFC)))) return(1);
			if (Variables->Creatures[Offset].MeleeID		!= (UINT)(*((BYTE *)(Default+0x0F50)))) return(1);
			if (Variables->Creatures[Offset].RechargeTime		!= (LONG)(*((LONG *)(Default+0x0EF8)))) return(1);
			if (Variables->Creatures[Offset].Range			!= (LONG)(*((LONG *)(Default+0x0EEC)))) return(1);
			if (Variables->Creatures[Offset].Dammage		!= (LONG)(*((WORD *)(Default+0x0F14)))) return(1);
			if (Variables->Creatures[Offset].Delay			!= (LONG)(*((LONG *)(Default+0x0CF9)))) return(1);
			//--- Resistances ---
			if (Variables->Creatures[Offset].Res1			!= (UINT)(*((BYTE *)(Default+0x0E0C)))) return(1);
			if (Variables->Creatures[Offset].Res1Rate		!= (LONG)(*((BYTE *)(Default+0x0E0D)))) return(1);
			if (Variables->Creatures[Offset].Res2			!= (UINT)(*((BYTE *)(Default+0x0E0E)))) return(1);
			if (Variables->Creatures[Offset].Res2Rate		!= (LONG)(*((BYTE *)(Default+0x0E0F)))) return(1);
			if (Variables->Creatures[Offset].Res3			!= (UINT)(*((BYTE *)(Default+0x0E10)))) return(1);
			if (Variables->Creatures[Offset].Res3Rate		!= (LONG)(*((BYTE *)(Default+0x0E11)))) return(1);
			if (Variables->Creatures[Offset].Res4			!= (UINT)(*((BYTE *)(Default+0x0E12)))) return(1);
			if (Variables->Creatures[Offset].Res4Rate		!= (LONG)(*((BYTE *)(Default+0x0E13)))) return(1);
			//--- Possession ---
			if (Variables->Creatures[Offset].SpecialAb1		!= (UINT)(*((BYTE *)(Default+0x0F46)))) return(1);
			if (Variables->Creatures[Offset].SpecialAb1Data		!= (LONG)(*((WORD *)(Default+0x14E9)))) return(1);
			if (Variables->Creatures[Offset].SpecialAb2		!= (UINT)(*((BYTE *)(Default+0x0F47)))) return(1);
			if (Variables->Creatures[Offset].SpecialAb2Data		!= (LONG)(*((WORD *)(Default+0x14ED)))) return(1);
			if (Variables->Creatures[Offset].Melee1			!= (UINT)(*((BYTE *)(Default+0x0F42)))) return(1);
			if (Variables->Creatures[Offset].Melee2			!= (UINT)(*((BYTE *)(Default+0x0F43)))) return(1);
			if (Variables->Creatures[Offset].Special1		!= (UINT)(*((BYTE *)(Default+0x1025)))) return(1);
			if (Variables->Creatures[Offset].Special2		!= (UINT)(*((BYTE *)(Default+0x1026)))) return(1);
			if (Variables->Creatures[Offset].Spell			!= (UINT)(*((BYTE *)(Default+0x0F45)))) return(1);
			if (Variables->Creatures[Offset].GammaEffect		!= (UINT)(*((BYTE *)(Default+0x0D3C)))) return(1);
			if (Variables->Creatures[Offset].WaddleScale		!= (LONG)(*((WORD *)(Default+0x0DBC)))) return(1);
			if (Variables->Creatures[Offset].OscillateScale		!= (LONG)(*((WORD *)(Default+0x0DC0)))) return(1);
			if (Variables->Creatures[Offset].WalkScale		!= (LONG)(*((BYTE *)(Default+0x0D3D)))) return(1);
			if (Variables->Creatures[Offset].CameraRolls		!= (BOOL)(((*((BYTE *)(Default+0x0EFF)))&0x10)?TRUE:FALSE)) return(1);
			//--- Jobs ---
			if (Variables->Creatures[Offset].HJobs[0]		!= (UINT)(*((LONG *)(Default+0x0E14)))) return(1);
			if (Variables->Creatures[Offset].HJobsMood[0]		!= (LONG)(*((WORD *)(Default+0x0E18)))) return(1);
			if (Variables->Creatures[Offset].HJobsMana[0]		!= (LONG)(*((WORD *)(Default+0x0E1A)))) return(1);
			if (Variables->Creatures[Offset].HJobsChance[0]		!= (LONG)(*((BYTE *)(Default+0x0E1C)))) return(1);
			if (Variables->Creatures[Offset].HJobs[1]		!= (UINT)(*((LONG *)(Default+0x0E20)))) return(1);
			if (Variables->Creatures[Offset].HJobsMood[1]		!= (LONG)(*((WORD *)(Default+0x0E24)))) return(1);
			if (Variables->Creatures[Offset].HJobsMana[1]		!= (LONG)(*((WORD *)(Default+0x0E26)))) return(1);
			if (Variables->Creatures[Offset].HJobsChance[1]		!= (LONG)(*((BYTE *)(Default+0x0E28)))) return(1);
			if (Variables->Creatures[Offset].HJobs[2]		!= (UINT)(*((LONG *)(Default+0x0E2C)))) return(1);
			if (Variables->Creatures[Offset].HJobsMood[2]		!= (LONG)(*((WORD *)(Default+0x0E30)))) return(1);
			if (Variables->Creatures[Offset].HJobsMana[2]		!= (LONG)(*((WORD *)(Default+0x0E32)))) return(1);
			if (Variables->Creatures[Offset].HJobsChance[2]		!= (LONG)(*((BYTE *)(Default+0x0E34)))) return(1);
			if (Variables->Creatures[Offset].UJobs[0]		!= (UINT)(*((LONG *)(Default+0x0E38)))) return(1);
			if (Variables->Creatures[Offset].UJobsMood[0]		!= (LONG)(*((WORD *)(Default+0x0E3C)))) return(1);
			if (Variables->Creatures[Offset].UJobsMana[0]		!= (LONG)(*((WORD *)(Default+0x0E3E)))) return(1);
			if (Variables->Creatures[Offset].UJobsChance[0]		!= (LONG)(*((BYTE *)(Default+0x0E40)))) return(1);
			if (Variables->Creatures[Offset].UJobs[1]		!= (UINT)(*((LONG *)(Default+0x0E44)))) return(1);
			if (Variables->Creatures[Offset].UJobsMood[1]		!= (LONG)(*((WORD *)(Default+0x0E48)))) return(1);
			if (Variables->Creatures[Offset].UJobsMana[1]		!= (LONG)(*((WORD *)(Default+0x0E4A)))) return(1);
			if (Variables->Creatures[Offset].UJobsChance[1]		!= (LONG)(*((BYTE *)(Default+0x0E4C)))) return(1);
			if (Variables->Creatures[Offset].AJobs[0]		!= (UINT)(*((LONG *)(Default+0x0E50)))) return(1);
			if (Variables->Creatures[Offset].AJobsMood[0]		!= (LONG)(*((WORD *)(Default+0x0E54)))) return(1);
			if (Variables->Creatures[Offset].AJobsMana[0]		!= (LONG)(*((WORD *)(Default+0x0E56)))) return(1);
			if (Variables->Creatures[Offset].AJobsChance[0]		!= (LONG)(*((BYTE *)(Default+0x0E58)))) return(1);
			if (Variables->Creatures[Offset].AJobs[1]		!= (UINT)(*((LONG *)(Default+0x0E5C)))) return(1);
			if (Variables->Creatures[Offset].AJobsMood[1]		!= (LONG)(*((WORD *)(Default+0x0E60)))) return(1);
			if (Variables->Creatures[Offset].AJobsMana[1]		!= (LONG)(*((WORD *)(Default+0x0E62)))) return(1);
			if (Variables->Creatures[Offset].AJobsChance[1]		!= (LONG)(*((BYTE *)(Default+0x0E64)))) return(1);
			if (Variables->Creatures[Offset].AJobs[2]		!= (UINT)(*((LONG *)(Default+0x0E68)))) return(1);
			if (Variables->Creatures[Offset].AJobsMood[2]		!= (LONG)(*((WORD *)(Default+0x0E6C)))) return(1);
			if (Variables->Creatures[Offset].AJobsMana[2]		!= (LONG)(*((WORD *)(Default+0x0E6E)))) return(1);
			if (Variables->Creatures[Offset].AJobsChance[2]		!= (LONG)(*((BYTE *)(Default+0x0E70)))) return(1);
			if (Variables->Creatures[Offset].NJobs[0]		!= (UINT)(*((LONG *)(Default+0x0E74)))) return(1);
			if (Variables->Creatures[Offset].NJobs[1]		!= (UINT)(*((LONG *)(Default+0x0E78)))) return(1);
			if (Variables->Creatures[Offset].NJobs[2]		!= (UINT)(*((LONG *)(Default+0x0E7C)))) return(1);
			if (Variables->Creatures[Offset].NJobs[3]		!= (UINT)(*((LONG *)(Default+0x0E80)))) return(1);
			//--- Lights ---
			if (Variables->Creatures[Offset].LightX			!= (LONG)(*((LONG *)(Default+0x0D94)))) return(1);
			if (Variables->Creatures[Offset].LightY			!= (LONG)(*((LONG *)(Default+0x0D98)))) return(1);
			if (Variables->Creatures[Offset].LightZ			!= (LONG)(*((LONG *)(Default+0x0D9C)))) return(1);
			if (Variables->Creatures[Offset].LightRadius		!= (LONG)(*((WORD *)(Default+0x0DA0)))) return(1);
			if (Variables->Creatures[Offset].LightR			!= (LONG)(*((BYTE *)(Default+0x0DA8)))) return(1);
			if (Variables->Creatures[Offset].LightG			!= (LONG)(*((BYTE *)(Default+0x0DA9)))) return(1);
			if (Variables->Creatures[Offset].LightB			!= (LONG)(*((BYTE *)(Default+0x0DAA)))) return(1);
			if (Variables->Creatures[Offset].LightFlicker		!= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].LightPulse		!= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->Creatures[Offset].LightPlayer		!= (BOOL)(((*((BYTE *)(Default+0x0DA4)))&0x80)?TRUE:FALSE)) return(1);
			}
		}

	if (Flags&MGRF_CREATURESPELLS)
		{
		for (i = 0; GOOrder[i] != (DWORD *)-1; i+= 2)
			{
			if (GOOrder[i] == (DWORD *)-2) continue;
			Offset = (DWORD)GOOrder[i];
			Default = (BYTE *)GOOrder[i+1];

			if (Variables->CrSpells[Offset].RechargeTime		!= (LONG)(*((WORD *)(Default+0x00EA)))) return(1);
			if (Variables->CrSpells[Offset].Range			!= (LONG)(*((WORD *)(Default+0x00D0)))) return(1);
			if (Variables->CrSpells[Offset].Shot			!= (UINT)(*((BYTE *)(Default+0x00E7)))) return(1);
			if (Variables->CrSpells[Offset].ShotData1		!= (LONG)(*((WORD *)(Default+0x00C8)))) return(1);
			if (Variables->CrSpells[Offset].ShotData2		!= (LONG)(*((WORD *)(Default+0x00CC)))) return(1);
			if (Variables->CrSpells[Offset].Attacking		!= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->CrSpells[Offset].Creature		!= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x02)?TRUE:FALSE)) return(1);
			if (Variables->CrSpells[Offset].Defensive		!= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x04)?TRUE:FALSE)) return(1);
			if (Variables->CrSpells[Offset].CastSelf		!= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x80)?TRUE:FALSE)) return(1);
			if (Variables->CrSpells[Offset].CastOthers		!= (BOOL)(((*((BYTE *)(Default+0x00D5)))&0x01)?TRUE:FALSE)) return(1);
			if (Variables->CrSpells[Offset].CastCreatures		!= (BOOL)(((*((BYTE *)(Default+0x00D4)))&0x08)?TRUE:FALSE)) return(1);
			}
		}

	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
