
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des bandes de héros.
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
extern NODE		MapHerosBands;
extern LONG		MapChanges;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des bandes de héros					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Heros_Select(HWND hWnd)
{
	MAPTHINGHEROSBANDCTX	Context;
	int	 		Result;

	ZeroMemory(&Context,sizeof(MAPTHINGHEROSBANDCTX));
	Context.band = &MapHerosBands;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6600),hWnd,Heros_SelectProc,(LPARAM)&Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	Files_SaveUpdate();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de la boîte de dialogue (sélection des bandes)		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Heros_SelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGHEROSBANDCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Heros_SelectInit(hDlg,(MAPTHINGHEROSBANDCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPTHINGHEROSBANDCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case TVN_SELCHANGED:
					Context->hselected = ((NM_TREEVIEW *)lParam)->itemNew.hItem;
					if (((NM_TREEVIEW *)lParam)->itemNew.lParam == 0)
						{
						EnableWindow(GetDlgItem(hDlg,101),FALSE);
						EnableWindow(GetDlgItem(hDlg,102),FALSE);
						}
					else
						{
						EnableWindow(GetDlgItem(hDlg,101),TRUE);
						EnableWindow(GetDlgItem(hDlg,102),TRUE);
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case 101:
					Heros_SelectChange(hDlg,0,Context);
					break;
				case 103:
					Heros_SelectChange(hDlg,1,Context);
					break;
				case 102:
					Heros_SelectKill(hDlg,Context);
					break;

				case IDCANCEL:
				case IDOK:
					Heros_SelectReset(Context);
					EndDialog(hDlg,1);
					break;
				}
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Heros_SelectInit(HWND hDlg, MAPTHINGHEROSBANDCTX *Context)
{
	RECT	ClientRect;
	LONG	X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	ScreenToClient(hDlg,(POINT *)&ClientRect.left);
	ScreenToClient(hDlg,(POINT *)&ClientRect.right);
	Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->hlist) goto Error_0;
	if (!Misc_CreateImageList(CrIcons,&Context->himages,32,32,FALSE)) goto Error_1;
	if (!Misc_CreateImageList(BadCreatures,&Context->himages,32,32,TRUE)) goto Error_1;
	if (!Misc_CreateImageList(BadElites,&Context->himages,32,32,TRUE)) goto Error_1;
	if (!Misc_CreateImageList(Heros,&Context->himages,32,32,TRUE)) goto Error_1;
	SendMessage(Context->hlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)Context->himages);

	if (!Heros_SelectBuildList(Context)) goto Error_1;
	return;

//--- Erreurs ---

Error_1:Heros_SelectReset(Context);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Restitutions ««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Heros_SelectReset(MAPTHINGHEROSBANDCTX *Context)
{
	SendMessage(Context->hlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)NULL);
	Misc_DestroyImageList(&Context->himages);
	return;
}


// «»»» Fabrication de la liste des héros «««««««««««««««««««««««««««««««»

int Heros_SelectBuildList(MAPTHINGHEROSBANDCTX *Context)
{
	TV_INSERTSTRUCT 	 InsertStruct;
	HTREEITEM		 Parent;
	MAPTHINGHEROSBAND	*HerosBand;
	MAPTHINGHERO		*Hero;
	char			*Name;
	char			*Temp;
	int			 i,j;

	Temp = GlobalAlloc(GPTR,2048*2);
	if (!Temp) return(0);

	for (HerosBand = (MAPTHINGHEROSBAND *)Context->band->next; HerosBand != 0; HerosBand = (MAPTHINGHEROSBAND *)HerosBand->node.next)
		{
		InsertStruct.hParent = TVI_ROOT;
		InsertStruct.hInsertAfter = (HTREEITEM)TVI_LAST;
		InsertStruct.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		InsertStruct.DUMMYUNIONNAME.item.iImage = 2;
		InsertStruct.DUMMYUNIONNAME.item.iSelectedImage = 2;
		wsprintf(Temp,szBandHero,HerosBand->name,HerosBand->id);
		InsertStruct.DUMMYUNIONNAME.item.pszText = Temp;
		InsertStruct.DUMMYUNIONNAME.item.lParam = (LPARAM)HerosBand;
		Parent = (HTREEITEM)SendMessage(Context->hlist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&InsertStruct);
		if (!Parent)
			{
			GlobalFree(Temp);
			return(0);
			}

		for (Hero = (MAPTHINGHERO *)&HerosBand->heros; Hero->isvalid != 0; Hero++)
			{
			for (j = 3, i = 0; BadCreatures[i].Id != 0; i++, j++)
				if (Hero->id == BadCreatures[i].Id)
					{
					Name = BadCreatures[i].Name;
					goto Set_Item;
					}

			for (i = 0; BadElites[i].Id != 0; i++, j++)
				if (Hero->id == BadElites[i].Id)
					{
					Name = BadElites[i].Name;
					goto Set_Item;
					}

			for (i = 0; Heros[i].Id != 0; i++, j++)
				if (Hero->id == Heros[i].Id)
					{
					Name = Heros[i].Name;
					goto Set_Item;
					}

			Name = szUnknown;

		Set_Item:
			strcpy(Temp+2048,szBandFlags);
			if (Hero->options2&0x02) *(Temp+2048) ='*';
			if (Hero->options2&0x04) *(Temp+2048) ='+';
			if (Hero->options2&0x01) *(Temp+2049) ='T';
			if (Hero->options2&0x08) *(Temp+2050) ='A';
			if (Hero->options2&0x10) *(Temp+2051) ='H';
			if (Hero->options2&0x20) *(Temp+2052) ='L';
			if (Hero->options&0x01)  *(Temp+2054) ='S';
			if (Hero->options&0x02)  *(Temp+2055) ='J';
			if (Hero->options&0x04)  *(Temp+2056) ='M';
			if (Hero->options&0x08)  *(Temp+2057) ='C';

			if (Hero->options2&0x02)
				{
				wsprintf(Temp,szBandLeader,Name,Hero->level,Temp+2048);
				switch(Hero->objective)
					{
					case 0x0B:
						strcat(Temp,szHerosObjectives0B);
						break;
					case 0x0C:
						strcat(Temp,szHerosObjectives0C);
						break;
					case 0x0D:
						strcat(Temp,szHerosObjectives0D);
						break;
					case 0x0E:
						strcat(Temp,szHerosObjectives0E);
						break;
					case 0x11:
						strcat(Temp,szHerosObjectives11);
						break;
					case 0x12:
						strcat(Temp,szHerosObjectives12);
						break;
					case 0x13:
						strcat(Temp,szHerosObjectives13);
						break;
					case 0x16:
						strcat(Temp,szHerosObjectives16);
						break;
					case 0x17:
						wsprintf(Temp+2048,szHerosFmtObjectives17,Hero->area);
						strcat(Temp,Temp+2048);
						break;
					case 0x1B:
						strcat(Temp,szHerosObjectives1B);
						break;
					default:
						strcat(Temp,szUnknown);
						break;
					}
				}
			else
				wsprintf(Temp,szBandFollower,Name,Hero->level,Temp+2048);

			InsertStruct.DUMMYUNIONNAME.item.pszText = Temp;
			InsertStruct.DUMMYUNIONNAME.item.iImage = j;
			InsertStruct.DUMMYUNIONNAME.item.iSelectedImage = j;
			InsertStruct.hParent = Parent;
			InsertStruct.hInsertAfter = (HTREEITEM)TVI_LAST;
			InsertStruct.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			InsertStruct.DUMMYUNIONNAME.item.lParam = 0;
			if (!SendMessage(Context->hlist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&InsertStruct))
				{
				GlobalFree(Temp);
				return(0);
				}
			}
		}

	GlobalFree(Temp);
	return(1);
}


// «»»» Supprime une bande de héros «««««««««««««««««««««««««««««««««««««»

void Heros_SelectKill(HWND hDlg, MAPTHINGHEROSBANDCTX *Context)
{
	TV_ITEM		 Item;

	if (!Context->hselected) return;
	Item.mask = TVIF_PARAM;
	Item.hItem = Context->hselected;
	SendMessage(Context->hlist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;

	Map_Lock(TRUE,FALSE);
	Undo_CreateEntry((NODE *)Item.lParam,UNDO_SUBBAND);
	Map_Lock(FALSE,FALSE);
	MapChanges++;
	Context->hselected = NULL;
	EnableWindow(GetDlgItem(hDlg,101),FALSE);
	EnableWindow(GetDlgItem(hDlg,102),FALSE);
	SendMessage(Context->hlist,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);
	return;
}


// «»»» Modifie/Ajoute une bande de héros «««««««««««««««««««««««««««««««»

void Heros_SelectChange(HWND hDlg, UINT Mode, MAPTHINGHEROSBANDCTX *Context)
{
	MAPTHINGHEROSBANDMODCTX	*ModContext;
	TV_ITEM			 Item;
	int			 Result;

	if (Mode)
		{
		MTHBAND	*Band;
		LONG	 Count;

		for (Count = 0, Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next, Count++);
		if (Count == 24)
			{
			Misc_MessageBoxExt(hDlg,szBandTooManyBandsErr,szMessageBoxExtInfoTitle,szMessageBoxExtOk,MB_ICONINFORMATION);
			return;
			}
		}

	ModContext = GlobalAlloc(GPTR,sizeof(MAPTHINGHEROSBANDMODCTX));
	if (!ModContext) return;

	ZeroMemory(&Item,sizeof(TV_ITEM));
	if (!Mode)
		{
		if (!Context->hselected) goto Done;
		Item.mask = TVIF_PARAM;
		Item.hItem = Context->hselected;
		SendMessage(Context->hlist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
		if (!Item.lParam) goto Done;
		}

	ModContext->context = Context;
	ModContext->mode = Mode;
	ModContext->options2 = 0x20|0x08|0x04|0x01;
	ModContext->options = 0x00;
	ModContext->objective = 0x13;
	ModContext->ennemy = 3;
	if (Item.lParam)
		CopyMemory(&ModContext->band,(void *)Item.lParam,sizeof(MAPTHINGHEROSBAND));
	else
		ModContext->band.id = Heros_GetFreeBand();

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6601),hDlg,Heros_ModifyProc,(LPARAM)ModContext);
	if (Result == -1)
		{
		Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONEXCLAMATION);
		goto Done;
		}
	if (!Result)
		goto Done;

	if (!ModContext->band.heros[0].isvalid)
		{
		if (Item.lParam)
			Undo_CreateEntry((NODE *)Item.lParam,UNDO_SUBBAND);
		else
			goto Done;
		}
	else
		{
		if (Item.lParam)
			{
			Undo_CreateEntry((NODE *)Item.lParam,UNDO_MODBAND);
			CopyMemory(((BYTE *)Item.lParam)+sizeof(NODE),((BYTE *)&ModContext->band)+sizeof(NODE),sizeof(MTHBAND)-sizeof(NODE));
			}
		else
			{
			MTHBAND *Heros;
			Heros = GlobalAlloc(GPTR,sizeof(MAPTHINGHEROSBAND));
			if (!Heros)
				{
				Misc_PrintError(hDlg,szBandErr,NULL,MB_ICONHAND);
				goto Done;
				}
			CopyMemory(Heros,&ModContext->band,sizeof(MAPTHINGHEROSBAND));
			List_AddEntry((NODE *)Heros,Context->band);
			Undo_CreateEntry((NODE *)Heros,UNDO_ADDBAND);
			}
		}

	MapChanges++;
	SendMessage(Context->hlist,TVM_DELETEITEM,(WPARAM)0,(LPARAM)TVI_ROOT);
	Heros_SelectBuildList(Context);
	EnableWindow(GetDlgItem(hDlg,101),FALSE);
	EnableWindow(GetDlgItem(hDlg,102),FALSE);
Done:	GlobalFree(ModContext);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de la boîte de dialogue (ajout & modification des bandes)¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Heros_ModifyProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGHEROSBANDMODCTX *Context;
	LONG			 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Heros_ModifyInit(hDlg,(MAPTHINGHEROSBANDMODCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPTHINGHEROSBANDMODCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case NM_DBLCLK:
					Heros_ModifyModify(hDlg,Context);
					return(TRUE);
				case TVN_SELCHANGED:
					if (((NM_TREEVIEW *)lParam)->itemNew.lParam == 0)
						{
						EnableWindow(GetDlgItem(hDlg,150),FALSE);
						EnableWindow(GetDlgItem(hDlg,151),FALSE);
						Context->tvselected = NULL;
						}
					else
						{
						EnableWindow(GetDlgItem(hDlg,150),TRUE);
						EnableWindow(GetDlgItem(hDlg,151),(SendMessage(Context->lvlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0))?TRUE:FALSE);
						Context->tvselected = ((NM_TREEVIEW *)lParam)->itemNew.hItem;
						}
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (!SendMessage(Context->lvlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0))
						{
						EnableWindow(GetDlgItem(hDlg,201),FALSE);
						EnableWindow(GetDlgItem(hDlg,202),FALSE);
						EnableWindow(GetDlgItem(hDlg,151),FALSE);
						}
					else
						{
						EnableWindow(GetDlgItem(hDlg,201),TRUE);
						EnableWindow(GetDlgItem(hDlg,202),(SendMessage(Context->lvlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0) == 1)?TRUE:FALSE);
						EnableWindow(GetDlgItem(hDlg,151),(Context->tvselected)?TRUE:FALSE);
						}
					return(TRUE);
				case LVN_GETDISPINFO:
					Heros_ModifySetDispInfo((LV_DISPINFO *)lParam,Context);
					return(TRUE);
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 114:
							Misc_SetUpDownInt(hDlg,104,(NM_UPDOWN *)lParam);
							break;
						case 115:
							Misc_SetUpDownInt(hDlg,105,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+4,(1<<1)|(1<<2));
				return(TRUE);
				}
			break;

		case WM_PAINT:
			Misc_PrintButtonRightText(hDlg,101,szLevel);
			Misc_PrintButtonRightText(hDlg,104,szHealth);
			Misc_PrintButtonRightText(hDlg,105,szGoldHeld);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 104:
						case 105:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 100) SetDlgItemInt(hDlg,LOWORD(wParam),100,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 108:
							Things_SelectOptions(hDlg,&Context->options2,&Context->options,THING_OPTIONSBANDS);
							break;
						case 109:
							Things_SelectObjective(hDlg,&Context->objective,&Context->area,&Context->ennemy);
							break;
						case 150:
							Heros_ModifyAddHero(hDlg,Context);
							break;
						case 151:
							Heros_ModifyRemplaceHero(hDlg,Context);
							break;
						case 201:
							Heros_ModifyKill(hDlg,Context);
							break;
						case 202:
							Heros_ModifyModify(hDlg,Context);
							break;

						case IDOK:
							if (!Heros_ModifyCheckBand(hDlg,Context)) break;
							GetDlgItemText(hDlg,300,Context->band.name,THING_MAXBANDNAME);
							if (!Context->band.name[0]) wsprintf(Context->band.name,szBandID,Context->band.id);
							SendMessage(Context->tvlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)NULL);
							SendMessage(Context->lvlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							SendMessage(Context->tvlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)NULL);
							SendMessage(Context->lvlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Heros_ModifyInit(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	LV_COLUMN	 Column;
	TREELIST	 List;
	RECT		 ClientRect;
	LONG		 X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	/* Création de la treeview */

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	ScreenToClient(hDlg,(POINT *)&ClientRect.left);
	ScreenToClient(hDlg,(POINT *)&ClientRect.right);
	Context->tvlist = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->tvlist) goto Error_0;
	SendMessage(Context->tvlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)Context->context->himages);

	/* Création de la listview */

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	GetWindowRect(GetDlgItem(hDlg,200),&ClientRect);
	ScreenToClient(hDlg,(POINT *)&ClientRect.left);
	ScreenToClient(hDlg,(POINT *)&ClientRect.right);
	Context->lvlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!Context->lvlist) goto Error_0;
	SendMessage(Context->lvlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->context->himages);

	/* Initialise la listview */

	ZeroMemory(&Column,sizeof(LV_COLUMN));
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*15;
	Column.pszText = szBandCreature;
	Column.iSubItem = 0;
	SendMessage(Context->lvlist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column);
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*4;
	Column.pszText = szBandLevel;
	Column.iSubItem = 1;
	SendMessage(Context->lvlist,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column);
	Column.cx = Font.FontMaxWidth*4;
	Column.pszText = szBandEnemy;
	Column.iSubItem = 2;
	SendMessage(Context->lvlist,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column);
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*10;
	Column.pszText = szBandOptions;
	Column.iSubItem = 3;
	SendMessage(Context->lvlist,LVM_INSERTCOLUMN,(WPARAM)3,(LPARAM)&Column);
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*25;
	Column.pszText = szBandObjective;
	Column.iSubItem = 4;
	SendMessage(Context->lvlist,LVM_INSERTCOLUMN,(WPARAM)4,(LPARAM)&Column);

	Heros_ModifyBuildList(Context);
	List.hTree = Context->tvlist;
	List.Flags = LIST_BADS|LIST_HEROES|LIST_LPARAMPTR;
	List.Expand = LIST_HEROES;
	List.SelectedItem = 0;
	Misc_CreateCreaturesTreeList(&List);

	/* Création des données de démarrage */

	Misc_CreateUpDownButton(hDlg,105,115);
	Misc_CreateUpDownButton(hDlg,104,114);
	SetDlgItemText(hDlg,300,Context->band.name);
	SetDlgItemInt(hDlg,104,100,FALSE);
	SetDlgItemInt(hDlg,105,0,FALSE);
	SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,300,EM_LIMITTEXT,(WPARAM)THING_MAXBANDNAME,(LPARAM)0);

	Misc_CreateLevelComboBoxList(hDlg,101,1);
	return;

//--- Erreurs ---

Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Test si la bande de héro ne contient pas d'erreur «««««««««««««««»

int Heros_ModifyCheckBand(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	BOOL	HasLeader = FALSE;
	LONG	LeaderCount = 0;
	int	i;

	for (i = 0; i != THING_MAXHEROSINBAND; i++)
		{
		if (!Context->band.heros[i].isvalid) break;
		if (Context->band.heros[i].options2&0x02)
			{
			HasLeader = TRUE;
			LeaderCount++;
			}
		}

	if (!HasLeader)
		{
		Misc_MessageBoxExt(hDlg,szBandNoLeaderErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return(0);
		}

	if (LeaderCount > 1)
		{
		Misc_MessageBoxExt(hDlg,szBandTooManyLeadersErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return(0);
		}

	return(1);
}


// «»»» Affiche les informations sur les héros ««««««««««««««««««««««««««»

void Heros_ModifySetDispInfo(LV_DISPINFO *DispInfo, MAPTHINGHEROSBANDMODCTX *Context)
{
	int	j,k;

	//--- Set image of the item ---

	if (DispInfo->item.mask&LVIF_IMAGE)
		{
		for (k = 3, j = 0; BadCreatures[j].Id != 0; j++, k++)
			if (Context->band.heros[DispInfo->item.iItem].id == BadCreatures[j].Id)
				{
				DispInfo->item.iImage = k;
				goto Text;
				}
		for (j = 0; BadElites[j].Id != 0; j++, k++);
		for (j = 0; Heros[j].Id != 0; j++, k++)
			if (Context->band.heros[DispInfo->item.iItem].id == Heros[j].Id)
				DispInfo->item.iImage = k;
		}

	//--- Set text of the item ---

Text:	if (!(DispInfo->item.mask&LVIF_TEXT)) return;

	switch(DispInfo->item.iSubItem)
		{
		case 0:	for (j = 3, k = 0; BadCreatures[k].Id != 0; k++, j++)
				if (Context->band.heros[DispInfo->item.iItem].id == BadCreatures[k].Id)
					{
					if (Context->band.heros[DispInfo->item.iItem].options2&0x02) wsprintf(DispInfo->item.pszText,szBandLeaderShort,BadCreatures[k].Name);
					else strcpy(DispInfo->item.pszText,BadCreatures[k].Name);
					return;
					}
			for (k = 0; BadElites[k].Id != 0; k++, j++);
			for (k = 0; Heros[k].Id != 0; k++, j++)
				if (Context->band.heros[DispInfo->item.iItem].id == Heros[k].Id)
					{
					if (Context->band.heros[DispInfo->item.iItem].options2&0x02) wsprintf(DispInfo->item.pszText,szBandLeaderShort,Heros[k].Name);
					else strcpy(DispInfo->item.pszText,Heros[k].Name);
					return;
					}
			break;
		case 1:	wsprintf(DispInfo->item.pszText,szNumber,Context->band.heros[DispInfo->item.iItem].level);
			break;
		case 2:	DispInfo->item.pszText[0] = 0;
			switch(Context->band.heros[DispInfo->item.iItem].enemy)
				{
				case 0:	DispInfo->item.pszText[0] = '?';
					break;
				case 1:	DispInfo->item.pszText[0] = 'H';
					break;
				case 2:	DispInfo->item.pszText[0] = 'N';
					break;
				case 3:	DispInfo->item.pszText[0] = '1';
					break;
				case 4:	DispInfo->item.pszText[0] = '2';
					break;
				case 5:	DispInfo->item.pszText[0] = '3';
					break;
				case 6:	DispInfo->item.pszText[0] = '4';
					break;
				case 7:	DispInfo->item.pszText[0] = '5';
					break;
				}
			DispInfo->item.pszText[1] = 0;
			break;
		case 3:	strcpy(DispInfo->item.pszText,szBandFlags);
			if (Context->band.heros[DispInfo->item.iItem].options2&0x02) DispInfo->item.pszText[0]='*';
			if (Context->band.heros[DispInfo->item.iItem].options2&0x04) DispInfo->item.pszText[0]='+';
			if (Context->band.heros[DispInfo->item.iItem].options2&0x01) DispInfo->item.pszText[1]='T';
			if (Context->band.heros[DispInfo->item.iItem].options2&0x08) DispInfo->item.pszText[2]='A';
			if (Context->band.heros[DispInfo->item.iItem].options2&0x10) DispInfo->item.pszText[3]='H';
			if (Context->band.heros[DispInfo->item.iItem].options2&0x20) DispInfo->item.pszText[4]='L';
			if (Context->band.heros[DispInfo->item.iItem].options&0x01)  DispInfo->item.pszText[6]='S';
			if (Context->band.heros[DispInfo->item.iItem].options&0x02)  DispInfo->item.pszText[7]='J';
			if (Context->band.heros[DispInfo->item.iItem].options&0x04)  DispInfo->item.pszText[8]='M';
			if (Context->band.heros[DispInfo->item.iItem].options&0x08)  DispInfo->item.pszText[9]='C';
			break;
		case 4:	switch(Context->band.heros[DispInfo->item.iItem].objective)
				{
				case 0x0B:
					strcpy(DispInfo->item.pszText,szHerosObjectives0B);
					break;
				case 0x0C:
					strcpy(DispInfo->item.pszText,szHerosObjectives0C);
					break;
				case 0x0D:
					strcpy(DispInfo->item.pszText,szHerosObjectives0D);
					break;
				case 0x0E:
					strcpy(DispInfo->item.pszText,szHerosObjectives0E);
					break;
				case 0x11:
					strcpy(DispInfo->item.pszText,szHerosObjectives11);
					break;
				case 0x12:
					strcpy(DispInfo->item.pszText,szHerosObjectives12);
					break;
				case 0x13:
					strcpy(DispInfo->item.pszText,szHerosObjectives13);
					break;
				case 0x16:
					strcpy(DispInfo->item.pszText,szHerosObjectives16);
					break;
				case 0x17:
					wsprintf(DispInfo->item.pszText,szHerosFmtObjectives17,Context->band.heros[DispInfo->item.iItem].area);
					break;
				case 0x1B:
					strcpy(DispInfo->item.pszText,szHerosObjectives1B);
					break;
				default:
					strcpy(DispInfo->item.pszText,szUnknown);
					break;
				}
			break;
		}
	return;
}


// «»»» Création de la liste des héros dans la bande ««««««««««««««««««««»

void Heros_ModifyBuildList(MAPTHINGHEROSBANDMODCTX *Context)
{
	LV_ITEM	 Item;
	int	 i;

	Item.mask = LVIF_TEXT|LVIF_IMAGE;
	Item.iSubItem = 0;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.iImage = I_IMAGECALLBACK;

	for (i = 0; Context->band.heros[i].isvalid != 0; i++)
		{
		Item.iItem = i;
		SendMessage(Context->lvlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	return;
}


// «»»» Modification des créatures dans la bande ««««««««««««««««««««««««»

#pragma argsused
void Heros_ModifyRemplaceHero(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	TV_ITEM	 TVItem;
	LV_ITEM	 LVItem;

	TVItem.mask = TVIF_PARAM;
	TVItem.hItem = Context->tvselected;
	if (!SendMessage(Context->tvlist,TVM_GETITEM,(WPARAM)0,(LPARAM)&TVItem)) return;

	LVItem.mask = LVIF_TEXT|LVIF_IMAGE;
	LVItem.iItem = -1;
	LVItem.iSubItem = 0;
	LVItem.pszText = LPSTR_TEXTCALLBACK;
	LVItem.iImage = I_IMAGECALLBACK;

	while ( (LVItem.iItem = SendMessage(Context->lvlist,LVM_GETNEXTITEM,(WPARAM)LVItem.iItem,(LPARAM)LVNI_ALL|LVNI_SELECTED)) != -1)
		{
		Context->band.heros[LVItem.iItem].level = SendDlgItemMessage(hDlg,101,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
		Context->band.heros[LVItem.iItem].health = GetDlgItemInt(hDlg,104,NULL,FALSE);
		Context->band.heros[LVItem.iItem].goldheld = GetDlgItemInt(hDlg,105,NULL,FALSE);
		Context->band.heros[LVItem.iItem].enemy = Context->ennemy;
		Context->band.heros[LVItem.iItem].options = Context->options;
		Context->band.heros[LVItem.iItem].options2 = Context->options2;
		Context->band.heros[LVItem.iItem].objective = Context->objective;
		Context->band.heros[LVItem.iItem].area = Context->area;
		Context->band.heros[LVItem.iItem].id = ((ELEMENT *)TVItem.lParam)->Id;
		SendMessage(Context->lvlist,LVM_SETITEM,(WPARAM)0,(LPARAM)&LVItem);
		}

	InvalidateRect(Context->lvlist,NULL,FALSE);
	return;
}


// «»»» Modifie le héro sélectionné «««««««««««««««««««««««««««««««««««««»

void Heros_ModifyModify(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	MAPTHING	MapThing;
	LV_ITEM		LVItem;

	LVItem.mask = LVIF_TEXT|LVIF_IMAGE;
	LVItem.iItem = SendMessage(Context->lvlist,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	LVItem.iSubItem = 0;
	LVItem.pszText = LPSTR_TEXTCALLBACK;
	LVItem.iImage = I_IMAGECALLBACK;
	if (LVItem.iItem == -1) return;

	ZeroMemory(&MapThing,sizeof(MAPTHING));
	MapThing.node.type = THING_CREATURE;
	MapThing.owner = 1;
	MapThing.data.level = Context->band.heros[LVItem.iItem].level;
	MapThing.health = Context->band.heros[LVItem.iItem].health;
	MapThing.goldheld = Context->band.heros[LVItem.iItem].goldheld;
	MapThing.ennemy = Context->band.heros[LVItem.iItem].enemy;
	MapThing.herosoptions = Context->band.heros[LVItem.iItem].options;
	MapThing.creaturesoptions = Context->band.heros[LVItem.iItem].options2;
	MapThing.objective = Context->band.heros[LVItem.iItem].objective;
	MapThing.area = Context->band.heros[LVItem.iItem].area;
	MapThing.id = Context->band.heros[LVItem.iItem].id;

	if (!Things_Select(hDlg,&MapThing,THING_FLAGFROMBAND|THING_FLAGLOCKOWNER)) return;

	Context->band.heros[LVItem.iItem].level = MapThing.data.level;
	Context->band.heros[LVItem.iItem].health = MapThing.health;
	Context->band.heros[LVItem.iItem].goldheld = MapThing.goldheld;
	Context->band.heros[LVItem.iItem].enemy = MapThing.ennemy;
	Context->band.heros[LVItem.iItem].options = MapThing.herosoptions;
	Context->band.heros[LVItem.iItem].options2 = MapThing.creaturesoptions;
	Context->band.heros[LVItem.iItem].objective = MapThing.objective;
	Context->band.heros[LVItem.iItem].area = MapThing.area;
	Context->band.heros[LVItem.iItem].id = MapThing.id;
	SendMessage(Context->lvlist,LVM_SETITEM,(WPARAM)0,(LPARAM)&LVItem);
	InvalidateRect(Context->lvlist,NULL,FALSE);
	return;
}


// «»»» Ajoute une créature «««««««««««««««««««««««««««««««««««««««««««««»

void Heros_ModifyAddHero(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	TV_ITEM	 TVItem;
	LV_ITEM	 LVItem;
	int	 i;

	TVItem.mask = TVIF_PARAM;
	TVItem.hItem = Context->tvselected;
	if (!SendMessage(Context->tvlist,TVM_GETITEM,(WPARAM)0,(LPARAM)&TVItem)) return;

	for (i = 0; i != THING_MAXHEROSINBAND; i++)
		if (!Context->band.heros[i].isvalid) break;

	if (i == THING_MAXHEROSINBAND)
		{
		Misc_MessageBoxExt(hDlg,szBandTooManyHerosErr,szMessageBoxExtInfoTitle,szMessageBoxExtOk,MB_ICONINFORMATION);
		return;
		}

	Context->band.heros[i].isvalid = TRUE;
	Context->band.heros[i].level = SendDlgItemMessage(hDlg,101,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
	Context->band.heros[i].health = GetDlgItemInt(hDlg,104,NULL,FALSE);
	Context->band.heros[i].goldheld = GetDlgItemInt(hDlg,105,NULL,FALSE);
	Context->band.heros[i].enemy = Context->ennemy;
	Context->band.heros[i].options = Context->options;
	Context->band.heros[i].options2 = Context->options2;
	Context->band.heros[i].objective = Context->objective;
	Context->band.heros[i].area = Context->area;
	Context->band.heros[i].id = ((ELEMENT *)TVItem.lParam)->Id;

	LVItem.mask = LVIF_TEXT|LVIF_IMAGE;
	LVItem.iItem = SendMessage(Context->lvlist,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0);
	LVItem.iSubItem = 0;
	LVItem.pszText = LPSTR_TEXTCALLBACK;
	LVItem.iImage = I_IMAGECALLBACK;
	SendMessage(Context->lvlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&LVItem);
	return;
}


// «»»» Supprime un ou plusieurs héros ««««««««««««««««««««««««««««««««««»

void Heros_ModifyKill(HWND hDlg, MAPTHINGHEROSBANDMODCTX *Context)
{
	int	i = -1;

	if (!SendMessage(Context->lvlist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) return;

	while((i = SendMessage(Context->lvlist,LVM_GETNEXTITEM,(WPARAM)i,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		Context->band.heros[i].isvalid = EXTB('N','O','P','!');

Redo:	for (i = 0; i != THING_MAXHEROSINBAND; i++)
		{
		if (Context->band.heros[i].isvalid != EXTB('N','O','P','!')) continue;
		for (; i != THING_MAXHEROSINBAND; i++) CopyMemory(&Context->band.heros[i],&Context->band.heros[i+1],sizeof(MAPTHINGHERO));
		goto Redo;
		}

	EnableWindow(GetDlgItem(hDlg,201),FALSE);
	SendMessage(Context->lvlist,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	Heros_ModifyBuildList(Context);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Détermine la première bande de héros libre ««««««««««««««««««««««»

int Heros_GetFreeBand()
{
	int	 Result;

	for (Result = 0;; Result++) if (!Heros_FindHerosById(Result)) break;
	return(Result);
}


// «»»» Retrouve une bande en fonction de son identificateur ««««««««««««»

MTHBAND* Heros_FindHerosById(UINT HerosId)
{
	NODE	*List;

	for (List = MapHerosBands.next; List != 0; List = List->next)
		if (((MAPTHINGHEROSBAND *)List)->id == HerosId) return((MAPTHINGHEROSBAND *)List);
	return(NULL);
}


// «»»» Calcul le nombre de héros dans une bande déterminée «««««««««««««»

long Heros_GetHerosCount(MTHBAND *Band)
{
	if (Band)
		{
		long	Result = 0;

		while(1)
			{
			if (!Band->heros[Result].isvalid) return(Result);
			Result++;
			}
		}

	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
