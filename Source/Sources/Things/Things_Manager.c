
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestionnaire des éléments.

	NOTE:
		Things_ManagerKill() et Things_ManagerAdd() ne testent
		pas la valeur de retour de Undo_StoreEntry(). Cela ne
		pose pas de problème particulier puisque cette fonction
		ne peut échouer que lorsqu'il s'agit d'une modification
		mais c'est un comportement à surveiller (en cas de
		changements dans la routine Undo_StoreEntry()).
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
extern MAPTHING		ManagerCreature;
extern MAPTHING		ManagerDoor;
extern MAPTHING		ManagerTrap;
extern MAPTHING		ManagerObject;
extern MAPTHING		ManagerMagicalObject;
extern LONG		ManagerX;
extern LONG		ManagerY;
extern MAP		Map;
extern LONG		MapChanges;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapMagicalObjects;
extern NODE		MapObjects;
extern MAPRECT		MapCursorSel;
extern MAPRECT		MapCursorHigh;
extern MAPRECT		MapCursorRect;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création de la boîte de dialogue					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_Manager(UINT Thing)
{
	MAPTHINGMANAGER	*Manager;
	RECT		 MapRect;
	UINT		 Resource;
	int		 Result;

	Manager = GlobalAlloc(GPTR,sizeof(MAPTHINGMANAGER));
	if (!Manager) return;
	Manager->type = Thing;

	switch(Thing)
		{
		case THINGMNGR_CREATURES:
			Resource = 6400;
			break;
		case THINGMNGR_DOORSNTRAPS:
			Resource = 6401;
			break;
		case THINGMNGR_OBJECTS:
			Resource = 6402;
			break;
		}
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(Resource),hWnd,Things_ManagerProc,(LPARAM)Manager);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	GlobalFree(Manager);
	MapCursorHigh.hidden = TRUE;
	MapCursorRect.hidden = FALSE;
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Files_SaveUpdate();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de la boîte de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Things_ManagerProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGMANAGER	*Manager;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Things_ManagerInit(hDlg,(MAPTHINGMANAGER *)lParam);
		return(TRUE);
		}

	Manager = (MAPTHINGMANAGER *)GetWindowLong(hDlg,DWL_USER);
	if (!Manager) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
			return(TRUE);

		case WM_DRAWITEM:
			switch(Manager->type)
				{
				case THINGMNGR_CREATURES:
					Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+7,(1<<2)|(1<<3)|(1<<4)|(1<<7));
					break;
				case THINGMNGR_DOORSNTRAPS:
				case THINGMNGR_OBJECTS:
					Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+3,(1<<3));
					break;
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Manager->icon);
				}
			return(TRUE);

		case WM_PAINT:
			Misc_PrintButtonRightText(hDlg,106,szPosition);
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 186:
							Misc_SetUpDownInt(hDlg,106,(NM_UPDOWN *)lParam);
							break;
						case 187:
							Misc_SetUpDownInt(hDlg,107,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);

				case LVN_GETDISPINFO:
					Things_ManagerSetDispInfo((LV_DISPINFO *)lParam);
					return(TRUE);

				case LVN_ITEMCHANGED:
					Things_ManagerDisplayCount(hDlg,Manager);
					Things_ManagerChangeListState(hDlg,Manager);
					return(TRUE);

				case NM_DBLCLK:
					if (((NMHDR *)lParam)->hwndFrom == Manager->list)
						Things_ManagerModify(hDlg,Manager);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 100:
							Manager->onmap = SendDlgItemMessage(hDlg,100,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							switch(Manager->type)
								{
								case THINGMNGR_CREATURES:
									Things_ManagerSetCreaturesList(hDlg,Manager->onmap,Manager);
									break;
								case THINGMNGR_DOORSNTRAPS:
									Things_ManagerSetDoorsnTrapsList(hDlg,Manager->onmap,Manager);
									break;
								case THINGMNGR_OBJECTS:
									Things_ManagerSetObjectsList(hDlg,Manager->onmap,Manager);
									break;
								}
							break;
						case 101:
							Config.ManagerSortMethod = SendDlgItemMessage(hDlg,101,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
							break;
						case 300:
							Config.ManagerSubType = SendDlgItemMessage(hDlg,300,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							switch(Manager->type)
								{
								case THINGMNGR_DOORSNTRAPS:
									SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerDoor.id:ManagerTrap.id,(!Config.ManagerSubType)?Doors:Traps));
									break;
								case THINGMNGR_OBJECTS:
									SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerObject.id:ManagerMagicalObject.id,(!Config.ManagerSubType)?Objects:((Map.Properties.Version == 17)?MagicsV17:Magics)));
									break;
								}
							break;
						}
					return(TRUE);

				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 106:
							lData = GetDlgItemInt(hDlg,106,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,106,0,FALSE);
							if (lData >= Map.Properties.Width) SetDlgItemInt(hDlg,106,Map.Properties.Width-1,FALSE);
							ManagerX = lData;
							break;
						case 107:
							lData = GetDlgItemInt(hDlg,107,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,107,0,FALSE);
							if (lData >= Map.Properties.Height) SetDlgItemInt(hDlg,107,Map.Properties.Height-1,FALSE);
							ManagerY = lData;
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 105:
							switch(Manager->type)
								{
								case THINGMNGR_CREATURES:
									Things_Select(hDlg,&ManagerCreature,0);
									SetDlgItemText(hDlg,104,Things_GetCreatureNameFromID(ManagerCreature.id));
									break;
								case THINGMNGR_DOORSNTRAPS:
									Things_Select(hDlg,(!Config.ManagerSubType)?&ManagerDoor:&ManagerTrap,0);
									SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerDoor.id:ManagerTrap.id,(!Config.ManagerSubType)?Doors:Traps));
									break;
								case THINGMNGR_OBJECTS:
									Things_Select(hDlg,(!Config.ManagerSubType)?&ManagerObject:&ManagerMagicalObject,0);
									SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerObject.id:ManagerMagicalObject.id,(!Config.ManagerSubType)?Objects:((Map.Properties.Version == 17)?MagicsV17:Magics)));
									break;
								}
							break;

						case 108:
							Map_ScrollToPoint(hWnd,GetDlgItemInt(hDlg,106,NULL,FALSE),GetDlgItemInt(hDlg,107,NULL,FALSE),SHOW_CURSOR);
							Things_ManagerShowWindow(hDlg);
							break;
						case 109:
							Things_ManagerChoosePoint(hDlg);
							break;

						case 114:
							Config.ManagerModifyParams.ModifyFlags &= ~MNGR_FLAGASKOPTIONS;
							Config.ManagerModifyParams.ModifyFlags |= (IsDlgButtonChecked(hDlg,114) == BST_CHECKED)?MNGR_FLAGASKOPTIONS:0;
							break;
						case 115:
							Config.ManagerModifyParams.ModifyFlags &= ~MNGR_FLAGPREVIEW;
							Config.ManagerModifyParams.ModifyFlags |= (IsDlgButtonChecked(hDlg,115) == BST_CHECKED)?MNGR_FLAGPREVIEW:0;
							break;
						case 124:
							Config.ManagerAddParams.AddFlags &= ~MNGR_FLAGASKOPTIONS;
							Config.ManagerAddParams.AddFlags |= (IsDlgButtonChecked(hDlg,124) == BST_CHECKED)?MNGR_FLAGASKOPTIONS:0;
							break;
						case 125:
							Config.ManagerAddParams.AddFlags &= ~MNGR_FLAGPREVIEW;
							Config.ManagerAddParams.AddFlags |= (IsDlgButtonChecked(hDlg,125) == BST_CHECKED)?MNGR_FLAGPREVIEW:0;
							break;

						case 116:
							Things_ManagerModify(hDlg,Manager);
							break;
						case 117:
							Things_ManagerModifyOptions(hDlg,Manager->type,&Config.ManagerModifyParams);
							break;
						case 126:
							Things_ManagerAdd(hDlg,Manager);
							break;
						case 127:
							Things_ManagerAddOptions(hDlg,Manager->type,&Config.ManagerAddParams);
							break;

						case 156:
							Things_ManagerKill(hDlg,Manager);
							break;
						case 157:
							Things_ManagerCenterView(hDlg,Manager);
							break;
						case 158:
							Things_ManagerSelectAll(Manager);
							break;
						case 180:
							Things_ManagerUndo(hDlg,Manager);
							break;

						case 200:
							Things_SelectOptions(hDlg,&ManagerCreature.creaturesoptions,NULL,THING_OPTIONSCREATURES);
							break;
						case 201:
							Things_SelectOptions(hDlg,NULL,&ManagerCreature.herosoptions,THING_OPTIONSHEROS);
							break;
						case 202:
							Things_SelectObjective(hDlg,&ManagerCreature.objective,&ManagerCreature.area,&ManagerCreature.ennemy);
							break;

						case IDOK:
						case IDCANCEL:
							SendMessage(Manager->list,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							DestroyIcon(Manager->icon);
							Misc_DestroyImageList(&Manager->images);
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerInit(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_COLUMN	Column;
	RECT		ClientRect;
	BOOL		OnMap;
	LONG		X,Y;

	Window_Center(hDlg,hWnd,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER);
	SetWindowLong(hDlg,DWL_USER,(LONG)Manager);

	GetWindowRect(GetDlgItem(hDlg,102),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Manager->list = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Manager->list) goto Error;
	Manager->icon = LoadImage(hInstance,MAKEINTRESOURCE(19),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Manager->icon) goto Error;

	ZeroMemory(&Column,sizeof(LV_COLUMN));
	X = 0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH|LVCF_TEXT;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szManagerName;
	Column.iSubItem = X;
	if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szManagerOwner;
	Column.iSubItem = X;
	if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;

	switch(Manager->type)
		{
		case THINGMNGR_CREATURES:
			Column.fmt = LVCFMT_CENTER;
			Column.cx = Font.FontMaxWidth*4;
			Column.pszText = szManagerLevel;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.cx = Font.FontMaxWidth*5;
			Column.pszText = szManagerGoldHeld;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.cx = Font.FontMaxWidth*5;
			Column.pszText = szManagerHealth;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = Font.FontMaxWidth*10;
			Column.pszText = szManagerOptions;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = Font.FontMaxWidth*15;
			Column.pszText = szManagerObjective;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			if (!Misc_CreateImageList(CrIcons,&Manager->images,32,32,FALSE)) goto Error;
			if (!Misc_CreateImageList(BadCreatures,&Manager->images,32,32,TRUE)) goto Error;
			if (!Misc_CreateImageList(BadElites,&Manager->images,32,32,TRUE)) goto Error;
			if (!Misc_CreateImageList(Heros,&Manager->images,32,32,TRUE)) goto Error;
			break;
		case THINGMNGR_DOORSNTRAPS:
			Column.cx = Font.FontMaxWidth*6;
			Column.pszText = szManagerStatut;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			if (!Misc_CreateImageList(DtIcons,&Manager->images,32,32,FALSE)) goto Error;
			if (!Misc_CreateImageList(Doors,&Manager->images,32,32,TRUE)) goto Error;
			if (!Misc_CreateImageList(Traps,&Manager->images,32,32,TRUE)) goto Error;
			SendDlgItemMessage(hDlg,300,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerDoor);
			SendDlgItemMessage(hDlg,300,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerTrap);
			break;
		case THINGMNGR_OBJECTS:
			Column.cx = Font.FontMaxWidth*15;
			Column.pszText = szManagerData;
			Column.iSubItem = X;
			if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			if (!Misc_CreateImageList(ObIcons,&Manager->images,32,32,FALSE)) goto Error;
			if (!Misc_CreateImageList(Magics,&Manager->images,32,32,TRUE)) goto Error;
			if (!Misc_CreateImageList(Objects,&Manager->images,32,32,TRUE)) goto Error;
			SendDlgItemMessage(hDlg,300,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerObject);
			SendDlgItemMessage(hDlg,300,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerMagicalObject);
			break;
		}

	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szManagerPosition;
	Column.iSubItem = X;
	if (SendMessage(Manager->list,LVM_INSERTCOLUMN,(WPARAM)X,(LPARAM)&Column) == -1) goto Error;

	if (!Misc_CreateUpDownButton(hDlg,106,186)) goto Error;
	if (!Misc_CreateUpDownButton(hDlg,107,187)) goto Error;
	SendMessage(Manager->list,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Manager->images);

	SendDlgItemMessage(hDlg,100,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerSelect);
	SendDlgItemMessage(hDlg,100,CB_ADDSTRING,(WPARAM)0,(LPARAM)szManagerMap);
	if (MapCursorSel.hidden)
		{
		OnMap = TRUE;
		Manager->onmap = TRUE;
		SendDlgItemMessage(hDlg,100,CB_SETCURSEL,(WPARAM)1,(LPARAM)0);
		EnableWindow(GetDlgItem(hDlg,100),FALSE);
		}
	else
		{
		OnMap = FALSE;
		Manager->onmap = FALSE;
		SendDlgItemMessage(hDlg,100,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
		}
	for (X = 0; MngrSort[X] != 0; X++) SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)MngrSort[X]);
	SendDlgItemMessage(hDlg,101,CB_SETCURSEL,(WPARAM)Config.ManagerSortMethod,(LPARAM)0);

	if (ManagerX >= Map.Properties.Width) ManagerX = Map.Properties.Width-1;
	if (ManagerY >= Map.Properties.Height) ManagerY = Map.Properties.Height-1;
	SetDlgItemInt(hDlg,106,ManagerX,FALSE);
	SetDlgItemInt(hDlg,107,ManagerY,FALSE);
	SendDlgItemMessage(hDlg,106,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,107,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	CheckDlgButton(hDlg,114,(Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGASKOPTIONS)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,115,(Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGPREVIEW)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,124,(Config.ManagerAddParams.AddFlags&MNGR_FLAGASKOPTIONS)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,125,(Config.ManagerAddParams.AddFlags&MNGR_FLAGPREVIEW)?BST_CHECKED:BST_UNCHECKED);

	switch(Manager->type)
		{
		case THINGMNGR_CREATURES:
			Things_ManagerSetCreaturesList(hDlg,OnMap,Manager);
			SetDlgItemText(hDlg,104,Things_GetCreatureNameFromID(ManagerCreature.id));
			break;
		case THINGMNGR_DOORSNTRAPS:
			Things_ManagerSetDoorsnTrapsList(hDlg,OnMap,Manager);
			SendDlgItemMessage(hDlg,300,CB_SETCURSEL,(WPARAM)Config.ManagerSubType,(LPARAM)0);
			SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerDoor.id:ManagerTrap.id,(!Config.ManagerSubType)?Doors:Traps));
			break;
		case THINGMNGR_OBJECTS:
			Things_ManagerSetObjectsList(hDlg,OnMap,Manager);
			SendDlgItemMessage(hDlg,300,CB_SETCURSEL,(WPARAM)Config.ManagerSubType,(LPARAM)0);
			SetDlgItemText(hDlg,104,Things_GetThingNameFromID((!Config.ManagerSubType)?ManagerObject.id:ManagerMagicalObject.id,(!Config.ManagerSubType)?Objects:((Map.Properties.Version == 17)?MagicsV17:Magics)));
			break;
		}

	SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
	return;

//--- Erreur d'initialisation ---

Error2:	SendMessage(Manager->list,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	SendMessage(Manager->list,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error:	if (Manager->icon) DestroyIcon(Manager->icon);
	Misc_DestroyImageList(&Manager->images);
	EndDialog(hDlg,-1);
	return;
}


// «»»» Classement des éléments «««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int CALLBACK Things_ManagerSort(LPARAM FirstItem, LPARAM SecondItem, LPARAM Unused)
{
	DWORD	 SortMethod;
	char	*FirstItemName;
	char	*SecondItemName;
	int	 FirstItemOffset;
	int	 SecondItemOffset;
	int	 Result;

	//--- Doors are before traps ---
	if ( (((MAPTHING *)FirstItem)->node.type == THING_DOOR) && (((MAPTHING *)SecondItem)->node.type == THING_TRAP) )
		return(-1);
	//--- Magical objects are before objects ---
	if ( (((MAPTHING *)FirstItem)->node.type == THING_MAGICALOBJECT) && (((MAPTHING *)SecondItem)->node.type == THING_OBJECT) )
		return(-1);

	SortMethod = Config.ManagerSortMethod;

Redo:	switch(SortMethod)
		{
		case MNGR_SORTBYPOSITION:
			//--- Object that has the lowest x position is inserted before ---
			if ( ((MAPTHING *)FirstItem)->y == ((MAPTHING *)SecondItem)->y )
				{
				if ( ((MAPTHING *)FirstItem)->x > ((MAPTHING *)SecondItem)->x )
					return(1);
				if ( ((MAPTHING *)FirstItem)->x < ((MAPTHING *)SecondItem)->x )
					return(-1);
				return(0);
				}
			//--- Object that has the lowest y position is inserted before ---
			if ( ((MAPTHING *)FirstItem)->y > ((MAPTHING *)SecondItem)->y )
				return(1);
			if ( ((MAPTHING *)FirstItem)->y < ((MAPTHING *)SecondItem)->y )
				return(-1);
			break;
		case MNGR_SORTBYNAME:
			switch(((MAPTHING *)FirstItem)->node.type)
				{
				case THING_CREATURE:
					FirstItemName = Things_GetCreatureNameFromID(((MAPTHING *)FirstItem)->id);
					break;
				case THING_DOOR:
					FirstItemName = Things_GetThingNameFromID(((MAPTHING *)FirstItem)->id,Doors);
					break;
				case THING_TRAP:
					FirstItemName = Things_GetThingNameFromID(((MAPTHING *)FirstItem)->id,Traps);
					break;
				case THING_OBJECT:
					FirstItemName = Things_GetThingNameFromID(((MAPTHING *)FirstItem)->id,Objects);
					break;
				case THING_MAGICALOBJECT:
					FirstItemName = Things_GetThingNameFromID(((MAPTHING *)FirstItem)->id,((Map.Properties.Version == 17)?MagicsV17:Magics));
					break;
				}
			switch(((MAPTHING *)SecondItem)->node.type)
				{
				case THING_CREATURE:
					SecondItemName = Things_GetCreatureNameFromID(((MAPTHING *)SecondItem)->id);
					break;
				case THING_DOOR:
					SecondItemName = Things_GetThingNameFromID(((MAPTHING *)SecondItem)->id,Doors);
					break;
				case THING_TRAP:
					SecondItemName = Things_GetThingNameFromID(((MAPTHING *)SecondItem)->id,Traps);
					break;
				case THING_OBJECT:
					SecondItemName = Things_GetThingNameFromID(((MAPTHING *)SecondItem)->id,Objects);
					break;
				case THING_MAGICALOBJECT:
					SecondItemName = Things_GetThingNameFromID(((MAPTHING *)SecondItem)->id,((Map.Properties.Version == 17)?MagicsV17:Magics));
					break;
				}
			Result = strcmpi(FirstItemName,SecondItemName);
			if (Result) return(Result);
			break;
		case MNGR_SORTBYOWNER:
			if ( ((MAPTHING *)FirstItem)->owner > ((MAPTHING *)SecondItem)->owner )
				return(1);
			if ( ((MAPTHING *)FirstItem)->owner < ((MAPTHING *)SecondItem)->owner )
				return(-1);
			break;
		case MNGR_SORTBYID:
			if ( ((MAPTHING *)FirstItem)->id < ((MAPTHING *)SecondItem)->id )
				return(-1);
			if ( ((MAPTHING *)FirstItem)->id > ((MAPTHING *)SecondItem)->id )
				return(1);
			break;
		case MNGR_SORTBYTYPE:
			FirstItemOffset = Things_GetOffsetFromID(((MAPTHING *)FirstItem)->id,((MAPTHING *)FirstItem)->node.type);
			SecondItemOffset = Things_GetOffsetFromID(((MAPTHING *)SecondItem)->id,((MAPTHING *)SecondItem)->node.type);
			if (FirstItemOffset < SecondItemOffset)
				return(-1);
			if (FirstItemOffset > SecondItemOffset)
				return(1);
			if (((MAPTHING *)FirstItem)->data.all < ((MAPTHING *)SecondItem)->data.all)
				return(-1);
			if (((MAPTHING *)FirstItem)->data.all > ((MAPTHING *)SecondItem)->data.all)
				return(1);
			return(0);
		}

	SortMethod = MNGR_SORTBYTYPE;
	goto Redo;
}


// «»»» Elements sur la carte «««««««««««««««««««««««««««««««««««««««««««»

//--- Créatures sur la carte ---

void Things_ManagerSetCreaturesList(HWND hDlg, BOOL WholeMap, MAPTHINGMANAGER *Manager)
{
	LV_ITEM		 Item;
	MAPCLIENT	 MapClient;
	MAPTHING	*List;
	int		 ItemID;

	if (WholeMap)
		{
		MapClient.X = 0;
		MapClient.Y = 0;
		MapClient.Width = Map.Properties.Width;
		MapClient.Height = Map.Properties.Height;
		}
	else
		Map_CorrectRect(&MapCursorSel,&MapClient);

	SendMessage(Manager->list,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	ZeroMemory(&Item,sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iSubItem = 0;

	for (ItemID = 0, List = (MAPTHING *)MapCreatures.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		if (List->x < MapClient.X) continue;
		if (List->y < MapClient.Y) continue;
		if (List->x > MapClient.X+MapClient.Width-1) continue;
		if (List->y > MapClient.Y+MapClient.Height-1) continue;

		Item.iItem = ItemID++;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.iImage = I_IMAGECALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	Things_ManagerDisplayCount(hDlg,Manager);
	return;
}

//--- Portes et pièges sur la carte ---

void Things_ManagerSetDoorsnTrapsList(HWND hDlg, BOOL WholeMap, MAPTHINGMANAGER *Manager)
{
	LV_ITEM		 Item;
	MAPCLIENT	 MapClient;
	MAPTHING	*List;
	int		 ItemID;

	if (WholeMap)
		{
		MapClient.X = 0;
		MapClient.Y = 0;
		MapClient.Width = Map.Properties.Width;
		MapClient.Height = Map.Properties.Height;
		}
	else
		Map_CorrectRect(&MapCursorSel,&MapClient);

	SendMessage(Manager->list,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	ZeroMemory(&Item,sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iSubItem = 0;

	for (ItemID = 0, List = (MAPTHING *)MapDoors.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		if (List->x < MapClient.X) continue;
		if (List->y < MapClient.Y) continue;
		if (List->x > MapClient.X+MapClient.Width-1) continue;
		if (List->y > MapClient.Y+MapClient.Height-1) continue;

		Item.iItem = ItemID++;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.iImage = I_IMAGECALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	for (List = (MAPTHING *)MapTraps.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		if (List->x < MapClient.X) continue;
		if (List->y < MapClient.Y) continue;
		if (List->x > MapClient.X+MapClient.Width-1) continue;
		if (List->y > MapClient.Y+MapClient.Height-1) continue;

		Item.iItem = ItemID++;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.iImage = I_IMAGECALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	Things_ManagerDisplayCount(hDlg,Manager);
	return;
}

//--- Objets sur la carte ---

void Things_ManagerSetObjectsList(HWND hDlg, BOOL WholeMap, MAPTHINGMANAGER *Manager)
{
	LV_ITEM		 Item;
	MAPCLIENT	 MapClient;
	MAPTHING	*List;
	int		 ItemID;

	if (WholeMap)
		{
		MapClient.X = 0;
		MapClient.Y = 0;
		MapClient.Width = Map.Properties.Width;
		MapClient.Height = Map.Properties.Height;
		}
	else
		Map_CorrectRect(&MapCursorSel,&MapClient);

	SendMessage(Manager->list,LVM_DELETEALLITEMS,(WPARAM)0,(LPARAM)0);
	ZeroMemory(&Item,sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iSubItem = 0;

	for (ItemID = 0, List = (MAPTHING *)MapMagicalObjects.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		if (List->x < MapClient.X) continue;
		if (List->y < MapClient.Y) continue;
		if (List->x > MapClient.X+MapClient.Width-1) continue;
		if (List->y > MapClient.Y+MapClient.Height-1) continue;

		Item.iItem = ItemID++;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.iImage = I_IMAGECALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	for (List = (MAPTHING *)MapObjects.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		if (List->x < MapClient.X) continue;
		if (List->y < MapClient.Y) continue;
		if (List->x > MapClient.X+MapClient.Width-1) continue;
		if (List->y > MapClient.Y+MapClient.Height-1) continue;

		Item.iItem = ItemID++;
		Item.pszText = LPSTR_TEXTCALLBACK;
		Item.iImage = I_IMAGECALLBACK;
		Item.lParam = (LPARAM)List;
		SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		}

	Things_ManagerDisplayCount(hDlg,Manager);
	return;
}

//--- Affichage du nombre d'éléments ---

void Things_ManagerDisplayCount(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LONG	Selected;
	LONG	Total;

	Selected = SendMessage(Manager->list,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	Total = SendMessage(Manager->list,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0);
	wsprintf(Manager->temp,szManagerCount,Selected,Total);
	SetWindowText(GetDlgItem(hDlg,102),Manager->temp);
	return;
}


// «»»» Mise à jour des entrées de la liste «««««««««««««««««««««««««««««»

void Things_ManagerSetDispInfo(LV_DISPINFO *Info)
{
	MAPTHING	*Thing;

	if (!Info->item.lParam) return;
	Thing = (MAPTHING *)Info->item.lParam;

	if (Info->item.mask&LVIF_IMAGE)
		{
		switch(Thing->node.type)
			{
			case THING_CREATURE:
				Info->item.iImage = Things_ManagerGetImage(THINGMNGR_CREATURES,THING_CREATURE,Thing->id);
				break;
			case THING_DOOR:
				Info->item.iImage = Things_ManagerGetImage(THINGMNGR_DOORSNTRAPS,THING_DOOR,Thing->id);
				break;
			case THING_TRAP:
				Info->item.iImage = Things_ManagerGetImage(THINGMNGR_DOORSNTRAPS,THING_TRAP,Thing->id);
				break;
			case THING_MAGICALOBJECT:
				Info->item.iImage = Things_ManagerGetImage(THINGMNGR_OBJECTS,THING_MAGICALOBJECT,Thing->id);
				break;
			case THING_OBJECT:
				Info->item.iImage = Things_ManagerGetImage(THINGMNGR_OBJECTS,THING_OBJECT,Thing->id);
				break;
			}
		}

	if (Info->item.mask&LVIF_TEXT)
		{
		switch(Info->item.iSubItem)
			{
			case 0:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						wsprintf(Info->item.pszText,szText,Things_GetCreatureNameFromID(Thing->id));
						break;
					case THING_DOOR:
						wsprintf(Info->item.pszText,szText,Things_GetThingNameFromID(Thing->id,Doors));
						break;
					case THING_TRAP:
						wsprintf(Info->item.pszText,szText,Things_GetThingNameFromID(Thing->id,Traps));
						break;
					case THING_OBJECT:
						wsprintf(Info->item.pszText,szText,Things_GetThingNameFromID(Thing->id,Objects));
						break;
					case THING_MAGICALOBJECT:
						wsprintf(Info->item.pszText,szText,Things_GetThingNameFromID(Thing->id,(Map.Properties.Version >= 17)?MagicsV17:Magics));
						break;
					}
				break;
			case 1:	wsprintf(Info->item.pszText,szText,Players[Thing->owner-1].Name);
				break;
			case 2:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						wsprintf(Info->item.pszText,szNumber,Thing->data.level);
						break;
					case THING_DOOR:
						switch(Thing->id)
							{
							case 0x00000006:
							case 0x00000007:
								if (Thing->data.status == 2) strcpy(Info->item.pszText,szManagerBlueprint);
								else *Info->item.pszText = 0;
								break;
							default:
								switch(Thing->data.status)
									{
									case 0:	strcpy(Info->item.pszText,szManagerOpened);
										break;
									case 1:	strcpy(Info->item.pszText,szManagerLocked);
										break;
									case 2:	strcpy(Info->item.pszText,szManagerBlueprint);
										break;
									}
								break;
							}
						break;
					case THING_TRAP:
						if (Thing->data.shots == 0) strcpy(Info->item.pszText,szManagerBlueprint);
						else *Info->item.pszText = 0;
						break;
					case THING_MAGICALOBJECT:
						*Info->item.pszText = 0;
						break;
					case THING_OBJECT:
						switch(Thing->id)
							{
							case 0x00000001:
							case 0x00000002:
							case 0x00000003:
								wsprintf(Info->item.pszText,szManagerGold,Thing->data.gold);
								break;
							case 0x00000004:
								if ((!Thing->data.spell)||(!Things_GetThingNameFromID(Thing->data.spell,Spells)))
									*Info->item.pszText = 0;
								else
									wsprintf(Info->item.pszText,szManagerSpell,Things_GetThingNameFromID(Thing->data.spell,Spells));
								break;
							default:*Info->item.pszText = 0;
								break;
							}
						break;
					}
				break;
			case 3:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						wsprintf(Info->item.pszText,szNumber,Thing->goldheld);
						break;
					case THING_DOOR:
					case THING_TRAP:
					case THING_OBJECT:
					case THING_MAGICALOBJECT:
						wsprintf(Info->item.pszText,szManagerXY,Thing->x,Thing->y);
						break;
					}
				break;
			case 4:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						wsprintf(Info->item.pszText,szNumber,Thing->health);
						break;
					}
				break;
			case 5:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						strcpy(Info->item.pszText,szManagerFlags);
						if (Thing->creaturesoptions&0x01) Info->item.pszText[0]='T';
						if (Thing->creaturesoptions&0x08) Info->item.pszText[1]='A';
						if (Thing->creaturesoptions&0x10) Info->item.pszText[2]='H';
						if (Thing->creaturesoptions&0x20) Info->item.pszText[3]='L';
						if (Thing->creaturesoptions&0x40) Info->item.pszText[4]='D';
						if (Thing->creaturesoptions&0x80) Info->item.pszText[5]='M';
						if (Thing->owner == 1)
							{
							if (Thing->herosoptions&0x01) Info->item.pszText[7]='S';
							if (Thing->herosoptions&0x02) Info->item.pszText[8]='J';
							if (Thing->herosoptions&0x04) Info->item.pszText[9]='M';
							if (Thing->herosoptions&0x08) Info->item.pszText[10]='C';
							}
						break;
					}
				break;
			case 6:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						switch(Thing->objective)
                                        		{
							case 0x0B:
								strcpy(Info->item.pszText,szHerosObjectives0B);
                                                                break;
							case 0x0C:
								strcpy(Info->item.pszText,szHerosObjectives0C);
                                                                break;
							case 0x0D:
								strcpy(Info->item.pszText,szHerosObjectives0D);
                                                                break;
							case 0x0E:
								strcpy(Info->item.pszText,szHerosObjectives0E);
                                                                break;
							case 0x11:
								strcpy(Info->item.pszText,szHerosObjectives11);
                                                                break;
							case 0x12:
								strcpy(Info->item.pszText,szHerosObjectives12);
                                                                break;
							case 0x13:
								strcpy(Info->item.pszText,szHerosObjectives13);
                                                                break;
							case 0x16:
								strcpy(Info->item.pszText,szHerosObjectives16);
                                                                break;
							case 0x17:
								wsprintf(Info->item.pszText,szHerosFmtObjectives17,Thing->area);
                                                                break;
							case 0x1B:
								strcpy(Info->item.pszText,szHerosObjectives1B);
                                                                break;
							default:
								*Info->item.pszText = 0;
                                                                break;
							}
						break;
					}
				break;
			case 7:	switch(Thing->node.type)
					{
					case THING_CREATURE:
						wsprintf(Info->item.pszText,szManagerXY,Thing->x,Thing->y);
						break;
					}
				break;
			}
		}

	return;
}

//--- Obtention de l'image associée à un objet ---

int Things_ManagerGetImage(UINT Type, UINT SubType, UINT ThingID)
{
	int	Result;
	int	i;

	switch(Type)
		{
		case THINGMNGR_CREATURES:
			Result = 3;
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++)
				if (BadCreatures[i].Id == ThingID)
					return(Result);
			for (i = 0; BadElites[i].Id != 0; i++, Result++)
				if (BadElites[i].Id == ThingID)
					return(Result);
			for (i = 0; Heros[i].Id != 0; i++, Result++)
				if (Heros[i].Id == ThingID)
					return(Result);
			break;
		case THINGMNGR_DOORSNTRAPS:
			Result = 2;
			for (i = 0; Doors[i].Id != 0; i++, Result++)
				if ((Doors[i].Id == ThingID)&&(SubType == THING_DOOR))
					return(Result);
			for (i = 0; Traps[i].Id != 0; i++, Result++)
				if ((Traps[i].Id == ThingID)&&(SubType == THING_TRAP))
					return(Result);
			break;
		case THINGMNGR_OBJECTS:
			Result = 2;
			for (i = 0; Magics[i].Id != 0; i++, Result++)
				if ((Magics[i].Id == ThingID)&&(SubType == THING_MAGICALOBJECT))
					return(Result);
			for (i = 0; Objects[i].Id != 0; i++, Result++)
				if ((Objects[i].Id == ThingID)&&(SubType == THING_OBJECT))
					return(Result);
			break;
		}

	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélectionne la position sur la carte ««««««««««««««««««««««««««««»

void Things_ManagerChoosePoint(HWND hDlg)
{
	MAPRECT	MapRect;

	MapRect.x1 = GetDlgItemInt(hDlg,106,NULL,FALSE);
	MapRect.y1 = GetDlgItemInt(hDlg,107,NULL,FALSE);
	Map_ChoosePointEx(hDlg,&MapRect,MAP_CHOICELMBPOINT,szManagerChoice);
	SetDlgItemInt(hDlg,106,MapRect.x1,FALSE);
	SetDlgItemInt(hDlg,107,MapRect.y1,FALSE);
	return;
}


// «»»» Cache la fenêtre après une mise en évidence «««««««««««««««««««««»

void Things_ManagerShowWindow(HWND hDlg)
{
	ShowWindow(hDlg,SW_HIDE);
	DialogBox(hInstance,MAKEINTRESOURCE(6501),hDlg,Things_ManagerShowProc);
	ShowWindow(hDlg,SW_SHOW);
	return;
}

//--- Processus de la fenêtre ---

#pragma argsused
BOOL CALLBACK Things_ManagerShowProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		case WM_INITDIALOG: {
			RECT	ParentRect;
			RECT	WindowRect;
			long	X,Y;

			GetWindowRect(hWnd,&ParentRect);
			GetWindowRect(hDlg,&WindowRect);
			X = ParentRect.right-(WindowRect.right-WindowRect.left);
			Y = ParentRect.top+GetSystemMetrics(SM_CYCAPTION)/2;
			Window_CorrectPos(hDlg,&X,&Y);
			SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);
			} return(TRUE);

		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg,1);
					return(TRUE);
				}
		}

	return(FALSE);
}


// «»»» Met en évidence un élément ««««««««««««««««««««««««««««««««««««««»

void Things_ManagerCenterView(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_ITEM	Item;

	if (!SendMessage(Manager->list,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0)) return;
	Item.iItem = List_FindSelectedItem(Manager->list);
	if (Item.iItem == -1) return;

	Item.mask = LVIF_PARAM;
	Item.iSubItem = 0;
	Item.lParam = 0;
	SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;

	Map_ScrollToPoint(hWnd,((MAPTHING *)Item.lParam)->x,((MAPTHING *)Item.lParam)->y,SHOW_CURSOR);
	Things_ManagerShowWindow(hDlg);
	return;
}


// «»»» Affichage d'une requête avec réponse auto. ««««««««««««««««««««««»

int Things_ManagerRequest(HWND hDlg, char *WindowText, char *Gadgets, DWORD Flag)
{
	MESSAGEBOXSTRUCT	Request;

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = Gadgets;
	Request.windowtext = WindowText;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = Flag;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_FOLLOWMOUSE;

	return(Misc_MessageBoxExtArgs(hDlg,&Request));
}


// «»»» Change l'état des boutons associés aux listes «««««««««««««««««««»

void Things_ManagerChangeListState(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	BOOL	Activate;
	LONG	Count;

	Count = SendMessage(Manager->list,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	Activate = (Count)?TRUE:FALSE;
	EnableWindow(GetDlgItem(hDlg,116),Activate);
	EnableWindow(GetDlgItem(hDlg,156),Activate);
	if (Count > 1) Activate = FALSE;
	EnableWindow(GetDlgItem(hDlg,157),Activate);
	return;
}


// «»»» Sélectionne toutes les entrées ««««««««««««««««««««««««««««««««««»

void Things_ManagerSelectAll(MAPTHINGMANAGER *Manager)
{
	LV_ITEM	Item;

	Item.iItem = -1;

	while((Item.iItem = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
		{
		Item.stateMask = LVIS_SELECTED;
		Item.state = LVIS_SELECTED;
		SendMessage(Manager->list,LVM_SETITEMSTATE,(WPARAM)Item.iItem,(LPARAM)&Item);
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion interne de l'annulation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Stockage du nombres d'opérations effectuées «««««««««««««««««««««»

void Things_ManagerUpdateUndo(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	Manager->undocnt++;
	EnableWindow(GetDlgItem(hDlg,180),TRUE);
	return;
}


// «»»» Annulation des opérations «««««««««««««««««««««««««««««««««««««««»

void Things_ManagerUndo(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	BOOL	OnMap;

	if (!Manager->undocnt) return;

	Undo_Restore();
	Manager->undocnt--;
	if (!Manager->undocnt) EnableWindow(GetDlgItem(hDlg,180),FALSE);

	OnMap = (SendDlgItemMessage(hDlg,100,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))?TRUE:FALSE;
	switch(Manager->type)
		{
		case THINGMNGR_CREATURES:
			Things_ManagerSetCreaturesList(hDlg,OnMap,Manager);
			break;
		case THINGMNGR_DOORSNTRAPS:
			Things_ManagerSetDoorsnTrapsList(hDlg,OnMap,Manager);
			break;
		case THINGMNGR_OBJECTS:
			Things_ManagerSetObjectsList(hDlg,OnMap,Manager);
			break;
		}

	SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
	Things_ManagerChangeListState(hDlg,Manager);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Suppression des éléments						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_ManagerKill(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_ITEM	 Item;
	RECT	 MapRect;
	UNDO	*Undo;
	UNDO	*Child;
	UINT	 Type;
	LONG	 Count;
	BOOL	 KillAll;
	char	 TempText[256];
	int	 i;

	Count = SendMessage(Manager->list,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (!Count) return;
	if (Count == 1)
		strcpy(TempText,szManagerSingleKillRequest);
	else
		wsprintf(TempText,szManagerMultiKillRequest,Count);

	if (!Things_ManagerRequest(hDlg,TempText,szMessageBoxExtYesNo,REQUEST_MNGRKILL)) return;

	Item.iSubItem = 0;
	Item.mask = LVIF_STATE|LVIF_PARAM;
	Item.stateMask = LVIS_SELECTED;

	//--- Test si un élément est utilisé par un déclencheur ---

	KillAll = TRUE;
	Item.iItem = -1;
	while ((Item.iItem = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		if (!((MAPTHING *)Item.lParam)->uniqueid) continue;
		i = Triggers_IsUsed(hDlg,((MAPTHING *)Item.lParam)->uniqueid,&KillAll,SCOPE_CREATURE|SCOPE_KILL);
		if (i == -1) continue;
		if (i == 0) return;
		break;
		}

	//--- Supprime les éléments ---

	Undo = Undo_CreateNewEntry(UNDO_THINGS,szMapKillThings);
	if (!Undo)
		{
		MessageBeep(MB_ICONHAND);
		return;
		}

	Things_ManagerUpdateUndo(hDlg,Manager);
	Map_Lock(TRUE,FALSE);
	for (i = 0;;)
		{
		Item.iItem = i++;
		if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) break;
		if (!(Item.state&LVIS_SELECTED)) continue;
		if (!Item.lParam) continue;
		if ((((MAPTHING *)Item.lParam)->uniqueid)&&(!KillAll)) continue;

		switch(((MAPTHING *)Item.lParam)->node.type)
			{
			case THING_CREATURE:
				Type = UNDO_SUBCREATURE;
				break;
			case THING_DOOR:
				Type = UNDO_SUBDOOR;
				break;
			case THING_TRAP:
				Type = UNDO_SUBTRAP;
				break;
			case THING_OBJECT:
				Type = UNDO_SUBOBJECT;
				break;
			case THING_MAGICALOBJECT:
				Type = UNDO_SUBMAGIC;
				break;
			}
		Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
		Undo_StoreEntry(Child,(NODE *)Item.lParam,Type);
		MapChanges++;
		SendMessage(Manager->list,LVM_DELETEITEM,(WPARAM)Item.iItem,(LPARAM)0);
		i = 0;
		}

	Map_Lock(FALSE,FALSE);
	SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
	Things_ManagerDisplayCount(hDlg,Manager);
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification des éléments					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_ManagerModify(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_ITEM		 Item;
	MAPTHINGMNGRTMP	*Template;
	MAPTHING	*Thing;
	UNDO		*Undo;
	UNDO		*Child;
	NODE		 WorkList;
	RECT		 MapRect;
	DWORD		 MapID;
	DWORD		 TempOptions;
	DWORD		 ModifyCoordsType;
	BOOL		 ModifyMulti;
	BOOL		 ModifyTriggeredElements;
	BOOL		 ThingsModified;
	BOOL		 ShowPreview;
	int		 UsedByTriggers;
	int		 i;

	i = SendMessage(Manager->list,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (i == 0)
		return;
	if (i == 1)
		ModifyMulti = FALSE;
	else
		ModifyMulti = TRUE;

	//
	// Pass 1... Used by triggers ?
	//

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;
	ModifyTriggeredElements = TRUE;
	while ((Item.iItem = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		if (!((MAPTHING *)Item.lParam)->uniqueid) continue;
		UsedByTriggers = Triggers_IsUsed(hDlg,((MAPTHING *)Item.lParam)->uniqueid,&ModifyTriggeredElements,SCOPE_CREATURE|SCOPE_MODIFY2);
		if (UsedByTriggers == -1) continue;
		if (UsedByTriggers == 0) return;
		break;
		}

	//
	// Pass 2... Locks the window and prepare datas
	//

	if ((Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGASKOPTIONS)&&(ModifyMulti))
		if (!Things_ManagerModifyOptions(hDlg,Manager->type,&Config.ManagerModifyParams))
			return;

	ZeroMemory(&WorkList,sizeof(NODE));

	if (ModifyMulti)
		{
		switch(Manager->type)
			{
			case THINGMNGR_CREATURES:
				Thing = &ManagerCreature;
				ModifyCoordsType = Config.ManagerModifyParams.ModifyCoordsType;
				break;
			case THINGMNGR_DOORSNTRAPS:
				Thing = (!Config.ManagerSubType)?&ManagerDoor:&ManagerTrap;
				ModifyCoordsType = MNGR_COORDRELATIVE;
				break;
			case THINGMNGR_OBJECTS:
				Thing = (!Config.ManagerSubType)?&ManagerObject:&ManagerMagicalObject;
				ModifyCoordsType = MNGR_COORDRELATIVE;
				break;
			}
		}

	//
	// Pass 3... Creates a temporary entry for each modifiable elements
	//

	Item.iItem = -1;
	while ((Item.iItem = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		if ((((MAPTHING *)Item.lParam)->uniqueid)&&(!ModifyTriggeredElements)) continue;

		Template = GlobalAlloc(GPTR,sizeof(MAPTHINGMNGRTMP));
		if (!Template) goto Error_0;

		Template->srcthing = (MAPTHING *)Item.lParam;
		CopyMemory(&Template->newthing,(void *)Item.lParam,sizeof(MAPTHING));
		List_AddEntry((NODE *)Template,&WorkList);
		}


	if (!WorkList.next)
		{
		/* Nothing to modify ! */
		return;
		}

	//
	// Pass 4... Calculating new coordinates (only on multiple modifications)
	//

	if ((Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYCOORDS)&&(ModifyMulti))
		{
		LONG	x,y;
		LONG	e,s;

		x = GetDlgItemInt(hDlg,106,NULL,FALSE);
		y = GetDlgItemInt(hDlg,107,NULL,FALSE);
		s = 60000;
		e = 60000;

		switch(ModifyCoordsType)
			{
			case MNGR_COORDSAME:
				break;
			case MNGR_COORDRELATIVE:
				for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
					{
					if (Template->srcthing->x < s) s = Template->srcthing->x;
					if (Template->srcthing->y < e) e = Template->srcthing->y;
					}
				break;
			}

		for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
			{
			if (ModifyCoordsType == MNGR_COORDSAME)
				{
				Template->newthing.x = x;
				Template->newthing.y = y;
				}
			else
				{
				Template->newthing.x = Template->srcthing->x+(x-s);
				Template->newthing.y = Template->srcthing->y+(y-e);
				}
			}
		}

	//
	// Pass 5... Changes the element(s) (according to the masks)
	//

	if (!ModifyMulti)
		{
		if (!Things_Select(hDlg,&Template->newthing,THING_FLAGPOSITION))
			return;

		if (!Template->newthing.owner)
			{
			MapID = Map_GetRealIDFromMap(Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES);
			MapID &= 0x00FF0000;
			MapID = MapID>>16;
			Template->newthing.owner = MapID;
			}
		}
	else
		{
		for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
			{
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYTYPE)
				{
				Template->newthing.node.type = Thing->node.type;
				Template->newthing.id = Thing->id;
				}
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYOWNER)
				{
				Template->newthing.owner = Thing->owner;
				if (!Template->newthing.owner)
					{
					if ((Template->newthing.x >= Map.Properties.Width)||(Template->newthing.y >= Map.Properties.Height))
						Template->newthing.owner = 2;
					else
						{
						MapID = Map_GetRealIDFromMap(Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES);
						MapID &= 0x00FF0000;
						MapID = MapID>>16;
						Template->newthing.owner = MapID;
						}
					}
				}

			switch(Manager->type)
				{
				case THINGMNGR_CREATURES:
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYLEVEL)
						Template->newthing.data.level = Thing->data.level;
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYGOLD)
						Template->newthing.goldheld = Thing->goldheld;
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYHEALTH)
						Template->newthing.health = Thing->health;
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYCROPTS)
						{
						switch(Config.ManagerModifyParams.ModifyCrOptionsType)
							{
							case MNGR_MODIFYALL:
								Template->newthing.creaturesoptions = Thing->creaturesoptions;
								break;
							case MNGR_MODIFYMASK:
								TempOptions = Thing->creaturesoptions;
								TempOptions &= Config.ManagerModifyParams.ModifyCrOptions;
								Template->newthing.creaturesoptions &= ~Config.ManagerModifyParams.ModifyCrOptions;
								Template->newthing.creaturesoptions |= TempOptions;
								break;
							}
						}
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYHEROESOPTS)
						{
						switch(Config.ManagerModifyParams.ModifyHrOptionsType)
							{
							case MNGR_MODIFYALL:
								Template->newthing.herosoptions = Thing->herosoptions;
								break;
							case MNGR_MODIFYMASK:
								TempOptions = Thing->herosoptions;
								TempOptions &= Config.ManagerModifyParams.ModifyHrOptions;
								Template->newthing.herosoptions &= ~Config.ManagerModifyParams.ModifyHrOptions;
								Template->newthing.herosoptions |= TempOptions;
								break;
							}
						}
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYOBJECTIVE)
						{
						Template->newthing.objective = Thing->objective;
						Template->newthing.area = Thing->area;
						Template->newthing.ennemy = Thing->ennemy;
						}

					//
					// Non-heroes creatures can't have heroes options or
					// objective
					//
					if (Template->newthing.owner != 1)
						{
						if ((Template->newthing.herosoptions)||(Template->newthing.objective))
							{
							Template->newthing.objective = 0;
							Template->newthing.herosoptions = 0;
							Template->newthing.area = 0;
							Template->newthing.ennemy = 0;
							Template->warnflags |= MNGR_WARNINCOMPATIBLE;
							strcpy(Template->warntext,szManagerWarningNonHero);
							}
						}
					//
					// Heroes options must have an objective
					// If none, sets it to attack any player
					//
					if (Template->newthing.owner == 1)
						{
						if (!Template->newthing.objective)
							{
							Template->newthing.objective = 0x13;
							Template->warnflags |= MNGR_WARNINCOMPATIBLE;
							strcpy(Template->warntext,szManagerWarningHero);
							}
						}
					break;

				case THINGMNGR_DOORSNTRAPS:
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYDATA)
						Template->newthing.data.all = Thing->data.all;
					switch(Template->newthing.node.type)
						{
						//
						// Doors data can be :
						// 0 - Build and opened
						// 1 - Build and closed (not for barricades)
						// 2 - Blueprint
						//
						case THING_DOOR:
							if (!Template->newthing.data.status) break;
							if (Template->newthing.data.status == 2) break;
							if (Template->newthing.data.status == 4) Template->newthing.data.status = 0;
							switch(Template->newthing.id)
								{
								case 0x00000006:
								case 0x00000007:
									if (Template->newthing.data.status == 2) break;
									if (Template->newthing.data.status == 0) break;
									Template->newthing.data.status = 0;
									Template->warnflags |= MNGR_WARNINCOMPATIBLE;
									strcpy(Template->warntext,szManagerWarningBarricade);
									break;
								}
							break;
						//
						// Traps data can be
						// 0 - Blueprint
						// 4 - Build
						//
						case THING_TRAP:
							if (Template->newthing.data.shots == 2)
								Template->newthing.data.shots = 0;
							if (Template->newthing.data.shots == 1)
								{
								Template->newthing.data.shots = 4;
								Template->warnflags |= MNGR_WARNINCOMPATIBLE;
								strcpy(Template->warntext,szManagerWarningTrap);
								}
							break;
						}
					break;

				case THINGMNGR_OBJECTS:
					if (Config.ManagerModifyParams.ModifyFlags&MNGR_MODIFYDATA)
						Template->newthing.data.all = Thing->data.all;
					switch(Template->newthing.node.type)
						{
						//
						// Object valid datas are
						// 0 - For all objects but gold and spell book
						//
						case THING_OBJECT:
							switch(Template->newthing.id)
								{
								case 0x00000001:
								case 0x00000002:
								case 0x00000003:
									break;
								case 0x00000004:
									for (i = 0; Spells[i].Id != 0; i++)
										if (Spells[i].Id == Template->newthing.data.spell)
											break;
									if (Spells[i].Id) break;
									Template->newthing.data.spell = 0;
									Template->warnflags |= MNGR_WARNINCOMPATIBLE;
									strcpy(Template->warntext,szManagerWarningSpell);
									break;
								default:
									if (!Template->newthing.data.all) break;
									Template->newthing.data.all = 0;
									Template->warnflags |= MNGR_WARNINCOMPATIBLE;
									strcpy(Template->warntext,szManagerWarningObject);
									break;
								}
							break;
						//
						// Magical objects have no datas
						//
						case THING_MAGICALOBJECT:
							if (!Template->newthing.data.shots) break;
							Template->newthing.data.shots = 0;
							Template->warnflags |= MNGR_WARNINCOMPATIBLE;
							strcpy(Template->warntext,szManagerWarningMagic);
							break;
						}
					break;
				}
			}
		}

	//
	// Pass 6... Checks for errors
	//

Redo:	for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		/* Is the thing is out of map ? */

		if ((Template->newthing.x >= Map.Properties.Width-1)||(Template->newthing.y >= Map.Properties.Height-1))
			Template->warnflags |= MNGR_WARNOUTOFMAP;

		/* Is the thing being placed on bad terrain ? */

		MapID = Map_GetRealIDFromMap(Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES);
		switch(Things_CanPaintEx(&Map,Template->newthing.x,Template->newthing.y,Template->newthing.node.type,Template->newthing.id,Template->newthing.owner,Template->text))
			{
			case 0:	Template->warnflags |= MNGR_WARNBADTERRAIN;
				if ((MapID>>24) == 0xFE) Template->warnflags |= MNGR_WARNLIMIT;
				break;
			case 2:	Template->warnflags |= MNGR_WARNENEMY;
				break;
			case 3:	Template->warnflags |= MNGR_WARNFATAL;
				break;
			}

		/* Is the thing overwrites another thing ? */

		switch(Template->newthing.node.type)
			{
			case THING_DOOR:
			case THING_TRAP:
				Thing = Things_GetAtMapPos(&MapDoors,Template->newthing.x,Template->newthing.y);
				if (Thing) break;
				Thing = Things_GetAtMapPos(&MapTraps,Template->newthing.x,Template->newthing.y);
				break;
			case THING_OBJECT:
			case THING_MAGICALOBJECT:
				Thing = Things_GetAtMapPos(&MapObjects,Template->newthing.x,Template->newthing.y);
				if (Thing) break;
				Thing = Things_GetAtMapPos(&MapMagicalObjects,Template->newthing.x,Template->newthing.y);
				break;
			default:
				Thing = NULL;
				break;
			}

		if (Thing)
			{
			MAPTHINGMNGRTMP	*Tmp;

			for (Tmp = (MAPTHINGMNGRTMP *)WorkList.next; Tmp != NULL; Tmp = (MAPTHINGMNGRTMP *)Tmp->node.next)
				if (Tmp->srcthing == Thing)
					{
					Thing = NULL;
					break;
					}

			if (Thing)
				{
				Template->oldthing = Thing;
				Template->warnflags |= MNGR_WARNREMPLACE;
				if (Thing->node.type == Template->newthing.node.type)
					{
					Template->warnflags |= MNGR_WARNREMSAMEFAMILY;
					if (Thing->id == Template->newthing.id) Template->warnflags |= MNGR_WARNREMSAME;
					}
				}
			}
		}

	//
	// Pass 7... Sets preview parameters
	//

	if (!ModifyMulti)
		{
		Template = (MAPTHINGMNGRTMP *)WorkList.next;
		if (Template->warnflags)
			{
			ShowPreview = TRUE;
			if ((Template->warnflags&MNGR_WARNOUTOFMAP)||(Template->warnflags&MNGR_WARNLIMIT))
				Template->locked = TRUE;
			}
		else
			{
			ShowPreview = FALSE;
			Template->modify = TRUE;
			}
		}
	else
		{
		for (ShowPreview = FALSE, Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
			{
			if (Template->verified)
				continue;

			if (!Template->warnflags)
				{
				Template->modify = TRUE;
				continue;
				}

			//
			// Problems that can be overridden (the thing is modified)
			//
			TempOptions = Template->warnflags&(MNGR_WARNBADTERRAIN|MNGR_WARNFATAL|MNGR_WARNENEMY|MNGR_WARNINCOMPATIBLE);
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGIGNORETERRAIN) TempOptions &= ~MNGR_WARNBADTERRAIN;
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGIGNOREFATALS) TempOptions &= ~MNGR_WARNFATAL;
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGIGNOREENEMIES) TempOptions &= ~MNGR_WARNENEMY;
			if (Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGIGNOREINCOMP) TempOptions &= ~MNGR_WARNINCOMPATIBLE;
			if (!TempOptions)
				Template->modify = TRUE;
			else
				ShowPreview = TRUE;

			//
			// Remplace problems (can be overridden)
			//
			if (Template->warnflags&MNGR_WARNREMPLACE)
				{
				switch(Config.ManagerModifyParams.ModifyRemplaceType)
					{
					case MNGR_REMPLACEALL:
						Template->modify = TRUE;
						break;
					case MNGR_REMPLACESAME:
						if (Template->warnflags&MNGR_WARNREMSAME)
							Template->modify = TRUE;
						else
							Template->modify = FALSE;
						break;
					case MNGR_REMPLACEFAMILY:
						if (Template->warnflags&MNGR_WARNREMSAMEFAMILY)
							Template->modify = TRUE;
						else
							Template->modify = FALSE;
						break;
					case MNGR_REMPLACENONE:
						Template->modify = FALSE;
						break;
					case MNGR_REMPLACEASK:
						Template->modify = FALSE;
						ShowPreview = TRUE;
						break;
					}
				}

			//
			// Following errors CAN'T be overridden
			// The thing CANNOT modified but CAN be deleted
			//
			if ((Template->warnflags&MNGR_WARNOUTOFMAP)||(Template->warnflags&MNGR_WARNLIMIT))
				{
				Template->modify = FALSE;
				Template->locked = TRUE;
				if (Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGIGNOREASK) ShowPreview = TRUE;
				}
			}
		}

	//
	// Pass 8... Displays the preview
	//

	if ((Config.ManagerModifyParams.ModifyFlags&MNGR_FLAGPREVIEW)||(ShowPreview))
		{
		if (!Things_ManagerPreview(hDlg,&WorkList,MNGR_PREVIEWMODIFY,Manager))
			{
			List_ReleaseMemory(&WorkList);
			return;
			}
		}

	//
	// Pass 9... Does last verifications (remplacements problems)
	// (Not for single modification and creatures)
	//

	if ((ModifyMulti)&&(Manager->type != THINGMNGR_CREATURES))
		{
		BOOL	DoItAgain = FALSE;

		for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL;)
			{
			if ((Template->modify)||(Template->remove))
				{
				Template = (MAPTHINGMNGRTMP *)Template->node.next;				
				continue;
				}
			List_RemEntry((NODE *)Template);
			GlobalFree(Template);
			Template = (MAPTHINGMNGRTMP *)WorkList.next;
			}

		for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
			{
			switch(Template->newthing.node.type)
				{
				case THING_DOOR:
				case THING_TRAP:
					Thing = Things_GetAtMapPos(&MapDoors,Template->newthing.x,Template->newthing.y);
					if (Thing) break;
					Thing = Things_GetAtMapPos(&MapTraps,Template->newthing.x,Template->newthing.y);
					break;
				case THING_OBJECT:
				case THING_MAGICALOBJECT:
					Thing = Things_GetAtMapPos(&MapObjects,Template->newthing.x,Template->newthing.y);
					if (Thing) break;
					Thing = Things_GetAtMapPos(&MapMagicalObjects,Template->newthing.x,Template->newthing.y);
					break;
				default:
					Thing = NULL;
					break;
				}
			if ((Thing)&&(!Template->oldthing))
				{
				MAPTHINGMNGRTMP	*Tmp;

				for (Tmp = (MAPTHINGMNGRTMP *)WorkList.next; Tmp != NULL; Tmp = (MAPTHINGMNGRTMP *)Tmp->node.next)
					if (Tmp->srcthing == Thing)
						{
						Thing = NULL;
						break;
						}
				if (Thing)
					{
					DoItAgain = TRUE;
					continue;
					}
				}
			Template->verified = TRUE;
			}
		if (DoItAgain) goto Redo;
		}

	//
	// Pass 10... Saves the changes
	//

	Map_Lock(TRUE,FALSE);

	for (Undo = NULL, ThingsModified = FALSE, Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		//
		//--- The thing has to be removed from map ---
		//
		if (Template->remove)
			{
			if (!Undo)
				{
				Undo = Undo_CreateNewEntry(UNDO_THINGS,szMapModifyThings);
				if (!Undo) goto Error_1;
				}
			Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
			if (!Child)
				{
				i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
				if (!i)
					{
					Undo_Restore();
					break;
					}
				else if (i == 2) break;
				}

			switch(Template->srcthing->node.type)
				{
				case THING_CREATURE:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_SUBCREATURE);
					MapChanges++;
					break;
				case THING_DOOR:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_SUBDOOR);
					MapChanges++;
					break;
				case THING_TRAP:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_SUBTRAP);
					MapChanges++;
					break;
				case THING_OBJECT:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_SUBOBJECT);
					MapChanges++;
					break;
				case THING_MAGICALOBJECT:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_SUBMAGIC);
					MapChanges++;
					break;
				}

			Things_ManagerRemoveFromList(Template->srcthing,Manager);
			ThingsModified = TRUE;
			continue;
			}
		//
		//--- The thing has to be modified ---
		//
		if (Template->modify)
			{
			if (!Undo)
				{
				Undo = Undo_CreateNewEntry(UNDO_THINGS,szMapModifyThings);
				if (!Undo) goto Error_1;
				}

			//
			// Removes the thing located at the new position
			//

			if (Template->oldthing)
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				if (!Child)
					{
					i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
					if (!i)
						{
						Undo_Restore();
						break;
						}
					else if (i == 2) break;
					}
				Things_ManagerRemoveFromList(Template->oldthing,Manager);
				switch(Template->oldthing->node.type)
					{
					case THING_CREATURE:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBCREATURE);
						MapChanges++;
						break;
					case THING_DOOR:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBDOOR);
						MapChanges++;
						break;
					case THING_TRAP:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBTRAP);
						MapChanges++;
						break;
					case THING_OBJECT:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBOBJECT);
						MapChanges++;
						break;
					case THING_MAGICALOBJECT:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBMAGIC);
						MapChanges++;
						break;
					}
				ThingsModified = TRUE;
				}

			//
			// Prepares undo memory for modification
			//

			Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
			if (!Child)
				{
				i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
				if (!i)
					{
					Undo_Restore();
					break;
					}
				else if (i == 2) break;
				}

			switch(Template->srcthing->node.type)
				{
				case THING_CREATURE:
					i = Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_MODCREATURE);
					MapChanges++;
					break;
				case THING_DOOR:
					i = Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_MODDOOR);
					MapChanges++;
					break;
				case THING_TRAP:
					i = Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_MODTRAP);
					MapChanges++;
					break;
				case THING_OBJECT:
					i = Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_MODOBJECT);
					MapChanges++;
					break;
				case THING_MAGICALOBJECT:
					i = Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_MODMAGIC);
					MapChanges++;
					break;
				}
			if (!i)
				{
				i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
				if (!i)
					{
					Undo_Restore();
					break;
					}
				else if (i == 2) break;
				}

			//
			// Prepares undo memory for moves through lists.
			// (Only for multiple modifications)
			//

			if (ModifyMulti)
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				if (!Child)
					{
					i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
					if (!i)
						{
						Undo_Restore();
						break;
						}
					else if (i == 2) break;
					}
				Undo_StoreMoveThing(Child,Template->srcthing);
				}

			//
			// Modifies the thing.
			// Adds it into its new list (multiple modifications only)
			//

			ThingsModified = TRUE;
			CopyMemory(Template->srcthing,&Template->newthing,sizeof(MAPTHING));
			if (ModifyMulti)
				{
				switch(Template->srcthing->node.type)
					{
					case THING_CREATURE:
						List_AddEntry((NODE *)Template->srcthing,&MapCreatures);
						break;
					case THING_DOOR:
						List_AddEntry((NODE *)Template->srcthing,&MapDoors);
						break;
					case THING_TRAP:
						List_AddEntry((NODE *)Template->srcthing,&MapTraps);
						break;
					case THING_OBJECT:
						List_AddEntry((NODE *)Template->srcthing,&MapObjects);
						break;
					case THING_MAGICALOBJECT:
						List_AddEntry((NODE *)Template->srcthing,&MapMagicalObjects);
						break;
					}
				}

			//
			// Eventually removes the thing from the listview
			//

			if (!Things_ManagerIsThingInRect(Template->srcthing,Manager))
				Things_ManagerRemoveFromList(Template->srcthing,Manager);

			continue;
			}
		}

	//
	// Cleans up the undo list (empty modifications are removed from the list)
	// and updates the internal undo feature...
	//

	if ((!ThingsModified)&&(Undo))
		Undo_Restore();
	else if (ThingsModified)
		Things_ManagerUpdateUndo(hDlg,Manager);

	List_ReleaseMemory(&WorkList);

	//
	// Done...
	//

	Map_Lock(FALSE,TRUE);

	SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
	Things_ManagerDisplayCount(hDlg,Manager);
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;

//
//--- Erreurs ---
//

Error_1:Map_Lock(FALSE,TRUE);
Error_0:Misc_PrintError(hDlg,szManagerModifyErr,NULL,MB_ICONHAND);
	List_ReleaseMemory(&WorkList);
	return;
}


// «»»» Test si l'élément est toujours dans le cadre ««««««««««««««««««««»

int Things_ManagerIsThingInRect(MAPTHING *Thing, MAPTHINGMANAGER *Manager)
{
	if (!Manager->onmap)
		{
		MAPCLIENT	 MapClient;

		Map_CorrectRect(&MapCursorSel,&MapClient);
		if (Thing->x < MapClient.X) return(0);
		if (Thing->y < MapClient.Y) return(0);
		if (Thing->x > MapClient.X+MapClient.Width-1) return(0);
		if (Thing->y > MapClient.Y+MapClient.Height-1) return(0);
		}

	return(1);
}


// «»»» Supprime un élément de la liste «««««««««««««««««««««««««««««««««»

void Things_ManagerRemoveFromList(MAPTHING *Thing, MAPTHINGMANAGER *Manager)
{
	LV_ITEM	Item;

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;

	while ((Item.iItem = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
		{
		if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;

		if ((MAPTHING *)Item.lParam == Thing)
			{
			SendMessage(Manager->list,LVM_DELETEITEM,(WPARAM)Item.iItem,(LPARAM)0);
			Item.iItem = -1;
			}
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ajout de nouveaux éléments					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_ManagerAdd(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_ITEM		 Item;
	UNDO		*Undo;
	UNDO		*Child;
	MAPTHINGMNGRTMP	*Template;
	MAPTHINGMNGRTMP	*Next;
	MAPTHING	*Thing;
	MAPCLIENT	 MapClient;
	RECT		 MapRect;
	NODE		 WorkList;
	DWORD		 MapID;
	DWORD		 TempOptions;
	BOOL		 ShowPreview;
	BOOL		 ThingsAdded;
	LONG		 Count,TmpCount;
	LONG		 X,Y;
	int		 i;

	//
	// Pass 1... Prepares datas
	//

	if (Config.ManagerAddParams.AddFlags&MNGR_FLAGASKOPTIONS)
		if (!Things_ManagerAddOptions(hDlg,Manager->type,&Config.ManagerAddParams))
			return;

	ZeroMemory(&WorkList,sizeof(NODE));

	switch(Manager->type)
		{
		case THINGMNGR_CREATURES:
			Thing = &ManagerCreature;
			break;
		case THINGMNGR_DOORSNTRAPS:
			Thing = (!Config.ManagerSubType)?&ManagerDoor:&ManagerTrap;
			break;
		case THINGMNGR_OBJECTS:
			Thing = (!Config.ManagerSubType)?&ManagerObject:&ManagerMagicalObject;
			break;
		}

	//
	// Pass 2... Creates a temporary entry for each modifiable elements
	//

	switch(Config.ManagerAddParams.AddMode)
		{
		case MNGR_ADDONE:
			Count = 1;
			break;
		case MNGR_ADDRECT:
		case MNGR_ADDRANDOMRECT:
			if (Manager->onmap)
				{
				MapClient.X = 1;
				MapClient.Y = 1;
				MapClient.Width = Map.Properties.Width-1;
				MapClient.Height = Map.Properties.Height-1;
				}
			else
				Map_CorrectRect(&MapCursorSel,&MapClient);

			Count = MapClient.Width*MapClient.Height;
			break;
		}

	for (TmpCount = 0; TmpCount != Count; TmpCount++)
		{
		Template = GlobalAlloc(GPTR,sizeof(MAPTHINGMNGRTMP));
		if (!Template) goto Error_0;
		CopyMemory(&Template->newthing,Thing,sizeof(MAPTHING));
		List_AddEntry((NODE *)Template,&WorkList);
		}

	//
	// Pass 3... Set things coordinates & owner
	//

	X = MapClient.X;
	Y = MapClient.Y;
	randomize();

	for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = Next)
		{
		Next = (MAPTHINGMNGRTMP *)Template->node.next;
		switch(Config.ManagerAddParams.AddMode)
			{
			case MNGR_ADDONE:
				Template->newthing.x = GetDlgItemInt(hDlg,106,NULL,FALSE);
				Template->newthing.y = GetDlgItemInt(hDlg,107,NULL,FALSE);
				break;
			case MNGR_ADDRECT:
				Template->newthing.x = X++;
				Template->newthing.y = Y;
				break;
			case MNGR_ADDRANDOMRECT:
				Count = random(Config.ManagerAddParams.AddRandomFreq);
				if (Count) Template->remove = TRUE;
				Template->newthing.x = X++;
				Template->newthing.y = Y;
				break;
			}

		if (!Template->newthing.owner)
			{
			MapID = Map_GetRealIDFromMap(Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES);
			MapID &= 0x00FF0000;
			MapID = MapID>>16;
			Template->newthing.owner = MapID;
			}

		if (Config.ManagerAddParams.AddMode == MNGR_ADDONE) continue;

		if (X >= MapClient.X+MapClient.Width)
			{
			X = MapClient.X;
			Y++;
			}
		}

	for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL;)
		{
		if (Template->remove)
			{
			List_RemEntry((NODE *)Template);
			GlobalFree(Template);
			Template = (MAPTHINGMNGRTMP *)WorkList.next;
			continue;
			}
		Template = (MAPTHINGMNGRTMP *)Template->node.next;
		}

	if (!WorkList.next) return;

	//
	// Pass 4... Checks for errors
	//

	for (Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		/* Is the thing being placed on bad terrain ? */

		MapID = Map_GetRealIDFromMap(Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES);
		switch(Things_CanPaintEx(&Map,Template->newthing.x,Template->newthing.y,Template->newthing.node.type,Template->newthing.id,Template->newthing.owner,Template->text))
			{
			case 0:	Template->warnflags |= MNGR_WARNBADTERRAIN;
				if ((MapID>>24) == 0xFE) Template->warnflags |= MNGR_WARNLIMIT;
				break;
			case 2:	Template->warnflags |= MNGR_WARNENEMY;
				break;
			case 3:	Template->warnflags |= MNGR_WARNFATAL;
				break;
			}

		/* Is the thing overwrites another thing ? */

		switch(Template->newthing.node.type)
			{
			case THING_DOOR:
			case THING_TRAP:
				Thing = Things_GetAtMapPos(&MapDoors,Template->newthing.x,Template->newthing.y);
				if (Thing) break;
				Thing = Things_GetAtMapPos(&MapTraps,Template->newthing.x,Template->newthing.y);
				break;
			case THING_OBJECT:
			case THING_MAGICALOBJECT:
				Thing = Things_GetAtMapPos(&MapObjects,Template->newthing.x,Template->newthing.y);
				if (Thing) break;
				Thing = Things_GetAtMapPos(&MapMagicalObjects,Template->newthing.x,Template->newthing.y);
				break;
			default:
				Thing = NULL;
				break;
			}

		if (Thing)
			{
			MAPTHINGMNGRTMP	*Tmp;

			for (Tmp = (MAPTHINGMNGRTMP *)WorkList.next; Tmp != NULL; Tmp = (MAPTHINGMNGRTMP *)Tmp->node.next)
				if (Tmp->srcthing == Thing)
					{
					Thing = NULL;
					break;
					}

			if (Thing)
				{
				Template->oldthing = Thing;
				Template->warnflags |= MNGR_WARNREMPLACE;
				if (Thing->node.type == Template->newthing.node.type)
					{
					Template->warnflags |= MNGR_WARNREMSAMEFAMILY;
					if (Thing->id == Template->newthing.id) Template->warnflags |= MNGR_WARNREMSAME;
					}
				}
			}
		}

	//
	// Pass 5... Set preview parameteres for each entry
	//

	for (ShowPreview = FALSE, Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		if (!Template->warnflags)
			{
			Template->modify = TRUE;
			continue;
			}

		//
		// Problems that can be overridden (the thing is modified)
		//
		TempOptions = Template->warnflags&(MNGR_WARNBADTERRAIN|MNGR_WARNFATAL|MNGR_WARNENEMY|MNGR_WARNINCOMPATIBLE);
		if (Config.ManagerAddParams.AddFlags&MNGR_FLAGIGNORETERRAIN) TempOptions &= ~MNGR_WARNBADTERRAIN;
		if (Config.ManagerAddParams.AddFlags&MNGR_FLAGIGNOREFATALS) TempOptions &= ~MNGR_WARNFATAL;
		if (Config.ManagerAddParams.AddFlags&MNGR_FLAGIGNOREENEMIES) TempOptions &= ~MNGR_WARNENEMY;
		if (Config.ManagerAddParams.AddFlags&MNGR_FLAGIGNOREINCOMP) TempOptions &= ~MNGR_WARNINCOMPATIBLE;
		if (!TempOptions)
			Template->modify = TRUE;
		else
			ShowPreview = TRUE;

		//
		// Remplace problems (can be overridden)
		//
		if (Template->warnflags&MNGR_WARNREMPLACE)
			{
			switch(Config.ManagerAddParams.AddRemplaceType)
				{
				case MNGR_REMPLACEALL:
					Template->modify = TRUE;
					break;
				case MNGR_REMPLACESAME:
					if (Template->warnflags&MNGR_WARNREMSAME)
						Template->modify = TRUE;
					else
						Template->modify = FALSE;
					break;
				case MNGR_REMPLACEFAMILY:
					if (Template->warnflags&MNGR_WARNREMSAMEFAMILY)
						Template->modify = TRUE;
					else
						Template->modify = FALSE;
					break;
				case MNGR_REMPLACENONE:
					Template->modify = FALSE;
					break;
				case MNGR_REMPLACEASK:
					Template->modify = FALSE;
					ShowPreview = TRUE;
					break;
				}
			}

		//
		// Following errors CAN'T be overridden
		// The thing CANNOT modified but CAN be deleted
		//
		if ((Template->warnflags&MNGR_WARNOUTOFMAP)||(Template->warnflags&MNGR_WARNLIMIT))
			{
			Template->modify = FALSE;
			Template->locked = TRUE;
			if (Config.ManagerAddParams.AddFlags&MNGR_FLAGIGNOREASK) ShowPreview = TRUE;
			}
		}

	//
	// Pass 6... Display the preview
	//

	if ((Config.ManagerAddParams.AddFlags&MNGR_FLAGPREVIEW)||(ShowPreview))
		{
		if (!Things_ManagerPreview(hDlg,&WorkList,MNGR_PREVIEWADD,Manager))
			{
			List_ReleaseMemory(&WorkList);
			return;
			}
		}

	//
	// Pass 7... Save the changes
	//

	Map_Lock(TRUE,FALSE);

	for (Undo = NULL, ThingsAdded = FALSE, Template = (MAPTHINGMNGRTMP *)WorkList.next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		//
		//--- The thing has to be added ---
		//
		if (Template->modify)
			{
			Template->srcthing = GlobalAlloc(GPTR,sizeof(MAPTHING));
			if (!Template->srcthing) goto Error_1;
			CopyMemory(Template->srcthing,&Template->newthing,sizeof(MAPTHING));

			if (!Undo)
				{
				Undo = Undo_CreateNewEntry(UNDO_THINGS,szMapAddThings);
				if (!Undo)
					{
					GlobalFree(Template->srcthing);
					goto Error_1;
					}
				}

			//
			// Removes the thing located at the new position
			//

			if (Template->oldthing)
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				if (!Child)
					{
					i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
					if (!i)
						{
						GlobalFree(Template->srcthing);
						Undo_Restore();
						break;
						}
					else if (i == 2)
						{
						GlobalFree(Template->srcthing);
						break;
						}
					}
				Things_ManagerRemoveFromList(Template->oldthing,Manager);
				switch(Template->oldthing->node.type)
					{
					case THING_CREATURE:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBCREATURE);
						MapChanges++;
						break;
					case THING_DOOR:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBDOOR);
						MapChanges++;
						break;
					case THING_TRAP:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBTRAP);
						MapChanges++;
						break;
					case THING_OBJECT:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBOBJECT);
						MapChanges++;
						break;
					case THING_MAGICALOBJECT:
						Undo_StoreEntry(Child,(NODE *)Template->oldthing,UNDO_SUBMAGIC);
						MapChanges++;
						break;
					}
				ThingsAdded = TRUE;
				}

			//
			// Adds the new thing in its list
			//

			Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
			if (!Child)
				{
				i = Things_ManagerRequest(hDlg,szManagerModifyUndoErr,szManagerModifyUndoChoices,REQUEST_MNGRUNDOERR);
				if (!i)
					{
					GlobalFree(Template->srcthing);
					Undo_Restore();
					break;
					}
				else if (i == 2)
					{
					GlobalFree(Template->srcthing);
					break;
					}
				}

			switch(Template->srcthing->node.type)
				{
				case THING_CREATURE:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_ADDCREATURE);
					List_AddEntry((NODE *)Template->srcthing,&MapCreatures);
					MapChanges++;
					break;
				case THING_DOOR:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_ADDDOOR);
					List_AddEntry((NODE *)Template->srcthing,&MapDoors);
					MapChanges++;
					break;
				case THING_TRAP:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_ADDTRAP);
					List_AddEntry((NODE *)Template->srcthing,&MapTraps);
					MapChanges++;
					break;
				case THING_OBJECT:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_ADDOBJECT);
					List_AddEntry((NODE *)Template->srcthing,&MapObjects);
					MapChanges++;
					break;
				case THING_MAGICALOBJECT:
					Undo_StoreEntry(Child,(NODE *)Template->srcthing,UNDO_ADDMAGIC);
					List_AddEntry((NODE *)Template->srcthing,&MapMagicalObjects);
					MapChanges++;
					break;
				}

			//
			// Adds the things in the listview
			//

			if (!Manager->onmap)
				{
				Map_CorrectRect(&MapCursorSel,&MapClient);
				if (Template->srcthing->x < MapClient.X) continue;
				if (Template->srcthing->y < MapClient.Y) continue;
				if (Template->srcthing->x > MapClient.X+MapClient.Width-1) continue;
				if (Template->srcthing->y > MapClient.Y+MapClient.Height-1) continue;
				}

			Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			Item.iItem = SendMessage(Manager->list,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0)+1;
			Item.iSubItem = 0;
			Item.pszText = LPSTR_TEXTCALLBACK;
			Item.iImage = I_IMAGECALLBACK;
			Item.lParam = (LPARAM)Template->srcthing;
			SendMessage(Manager->list,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);

			ThingsAdded = TRUE;
			continue;
			}
		}


	//
	// Cleans up the undo list (empty operations are removed from the list)
	// and updates the internal undo feature...
	//

	if ((!ThingsAdded)&&(Undo))
		Undo_Restore();
	else if (ThingsAdded)
		Things_ManagerUpdateUndo(hDlg,Manager);

	List_ReleaseMemory(&WorkList);

	//
	// Done...
	//

	Map_Lock(FALSE,TRUE);

	SendMessage(Manager->list,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Things_ManagerSort);
	Things_ManagerDisplayCount(hDlg,Manager);
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;

//
//--- Erreurs ---
//

Error_1:Map_Lock(FALSE,TRUE);
Error_0:Misc_PrintError(hDlg,szManagerAddErr,NULL,MB_ICONHAND);
	List_ReleaseMemory(&WorkList);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Changement des paramètres d'ajout				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Things_ManagerAddOptions(HWND hDlg, UINT Type, MNGRADDPARAMS *Params)
{
	MNGRADDPARAMSCTX	*Context;
	int			 Result;

	Context = GlobalAlloc(GPTR,sizeof(MNGRADDPARAMSCTX));
	if (!Context) return(0);

	Context->type = Type;
	CopyMemory(&Context->params,Params,sizeof(MNGRADDPARAMS));

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6460),hDlg,Things_ManagerAddProc,(LPARAM)Context);
	if ((!Result)||(Result == -1))
		{
		GlobalFree(Context);
		if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		return(0);
		}

	CopyMemory(Params,&Context->params,sizeof(MNGRADDPARAMS));
	GlobalFree(Context);
	return(1);
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Things_ManagerAddProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MNGRADDPARAMSCTX	*Params;

	if (uMsgId == WM_INITDIALOG)
		{
		Things_ManagerAddInit(hDlg,(MNGRADDPARAMSCTX *)lParam);
		return(TRUE);
		}

	Params = (MNGRADDPARAMSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Params) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_HSCROLL:
			switch(LOWORD(wParam))
				{
				case TB_BOTTOM:
				case TB_LINEDOWN:
				case TB_LINEUP:
				case TB_PAGEDOWN:
				case TB_PAGEUP:
				case TB_THUMBPOSITION:
				case TB_THUMBTRACK:
				case TB_TOP:
					if ((HWND)lParam == GetDlgItem(hDlg,105))
						{
						SetDlgItemInt(hDlg,106,SendDlgItemMessage(hDlg,105,TBM_GETPOS,(WPARAM)0,(LPARAM)0),FALSE);
						break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
						case 102:
						case 103:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							else CheckRadioButton(hDlg,101,103,LOWORD(wParam));
							if (IsDlgButtonChecked(hDlg,103) == BST_CHECKED)
								{
								EnableWindow(GetDlgItem(hDlg,104),TRUE);
								EnableWindow(GetDlgItem(hDlg,105),TRUE);
								EnableWindow(GetDlgItem(hDlg,106),TRUE);
								}
							else
								{
								EnableWindow(GetDlgItem(hDlg,104),FALSE);
								EnableWindow(GetDlgItem(hDlg,105),FALSE);
								EnableWindow(GetDlgItem(hDlg,106),FALSE);
								}
							break;

						case 141:
						case 142:
						case 143:
						case 144:
						case 145:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							else CheckRadioButton(hDlg,141,145,LOWORD(wParam));
							break;

						case IDOK:
							Things_ManagerAddSave(hDlg,Params);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation ««««««««««««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerAddInit(HWND hDlg, MNGRADDPARAMSCTX *Params)
{
	LONG	X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Params);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	SendDlgItemMessage(hDlg,105,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(2,32));
	SendDlgItemMessage(hDlg,105,TBM_SETTICFREQ,(WPARAM)2,(LPARAM)0);

	//
	// Méthode d'ajout
	//

	CheckRadioButton(hDlg,101,103,101+Params->params.AddMode);
	SendDlgItemMessage(hDlg,105,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)Params->params.AddRandomFreq);
	SetDlgItemInt(hDlg,106,Params->params.AddRandomFreq,FALSE);

	if (Params->params.AddMode != MNGR_ADDRANDOMRECT)
		{
		EnableWindow(GetDlgItem(hDlg,104),FALSE);
		EnableWindow(GetDlgItem(hDlg,105),FALSE);
		EnableWindow(GetDlgItem(hDlg,106),FALSE);
		}

	//
	// En cas de problème
	//

	if (Params->params.AddFlags&MNGR_FLAGIGNORETERRAIN)	CheckDlgButton(hDlg,122,BST_CHECKED);
	if (Params->params.AddFlags&MNGR_FLAGIGNOREFATALS)	CheckDlgButton(hDlg,123,BST_CHECKED);
	if (Params->params.AddFlags&MNGR_FLAGIGNOREENEMIES)	CheckDlgButton(hDlg,124,BST_CHECKED);
	if (Params->params.AddFlags&MNGR_FLAGIGNOREASK)		CheckDlgButton(hDlg,125,BST_CHECKED);

	//
	// Méthode de remplacement
	//

	CheckRadioButton(hDlg,141,145,141+Params->params.AddRemplaceType);
	if (Params->type == THINGMNGR_CREATURES)
		{
		EnableWindow(GetDlgItem(hDlg,141),FALSE);
		EnableWindow(GetDlgItem(hDlg,142),FALSE);
		EnableWindow(GetDlgItem(hDlg,143),FALSE);
		EnableWindow(GetDlgItem(hDlg,144),FALSE);
		EnableWindow(GetDlgItem(hDlg,145),FALSE);
		}

	return;
}


// «»»» Sauvegarde des paramètres «««««««««««««««««««««««««««««««««««««««»

void Things_ManagerAddSave(HWND hDlg, MNGRADDPARAMSCTX *Params)
{
	//
	// Méthode d'ajout
	//

	if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED) Params->params.AddMode = MNGR_ADDONE;
	else if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED) Params->params.AddMode = MNGR_ADDRECT;
	else if (IsDlgButtonChecked(hDlg,103) == BST_CHECKED) Params->params.AddMode = MNGR_ADDRANDOMRECT;

	//
	// En cas de problème
	//

	Params->params.AddFlags &= ~(MNGR_FLAGIGNORETERRAIN|MNGR_FLAGIGNOREFATALS|MNGR_FLAGIGNOREENEMIES|MNGR_FLAGIGNOREASK);
	if (IsDlgButtonChecked(hDlg,122) == BST_CHECKED) Params->params.AddFlags |= MNGR_FLAGIGNORETERRAIN;
	if (IsDlgButtonChecked(hDlg,123) == BST_CHECKED) Params->params.AddFlags |= MNGR_FLAGIGNOREFATALS;
	if (IsDlgButtonChecked(hDlg,124) == BST_CHECKED) Params->params.AddFlags |= MNGR_FLAGIGNOREENEMIES;
	if (IsDlgButtonChecked(hDlg,125) == BST_CHECKED) Params->params.AddFlags |= MNGR_FLAGIGNOREASK;

	//
	// Méthode de remplacement
	//

	if (IsDlgButtonChecked(hDlg,141) == BST_CHECKED) Params->params.AddRemplaceType = MNGR_REMPLACEALL;
	else if (IsDlgButtonChecked(hDlg,142) == BST_CHECKED) Params->params.AddRemplaceType = MNGR_REMPLACESAME;
	else if (IsDlgButtonChecked(hDlg,143) == BST_CHECKED) Params->params.AddRemplaceType = MNGR_REMPLACEFAMILY;
	else if (IsDlgButtonChecked(hDlg,144) == BST_CHECKED) Params->params.AddRemplaceType = MNGR_REMPLACENONE;
	else if (IsDlgButtonChecked(hDlg,145) == BST_CHECKED) Params->params.AddRemplaceType = MNGR_REMPLACEASK;

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Changement des paramètres de modification			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Things_ManagerModifyOptions(HWND hDlg, UINT Type, MNGRMODPARAMS *Params)
{
	MNGRMODPARAMSCTX	*Context;
	UINT			 CtrlId;
	int			 Result;

	switch(Type)
		{
		case THINGMNGR_CREATURES:
			CtrlId = 6450;
			break;
		case THINGMNGR_DOORSNTRAPS:
			CtrlId = 6451;
			break;
		case THINGMNGR_OBJECTS:
			CtrlId = 6452;
			break;
		default:return(0);
		}

	Context = GlobalAlloc(GPTR,sizeof(MNGRMODPARAMSCTX));
	if (!Context) return(0);
	Context->type = Type;
	CopyMemory(&Context->params,Params,sizeof(MNGRMODPARAMS));

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(CtrlId),hDlg,Things_ManagerModifyProc,(LPARAM)Context);
	if ((!Result)||(Result == -1))
		{
		GlobalFree(Context);
		if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		return(0);
		}

	CopyMemory(Params,&Context->params,sizeof(MNGRMODPARAMS));
	GlobalFree(Context);
	return(1);
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Things_ManagerModifyProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MNGRMODPARAMSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Things_ManagerModifyInit(hDlg,(MNGRMODPARAMSCTX *)lParam);
		return(TRUE);
		}

	Context = (MNGRMODPARAMSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
							if (Context->type == THINGMNGR_CREATURES)
								{
								if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED)
									{
									EnableWindow(GetDlgItem(hDlg,102),TRUE);
									EnableWindow(GetDlgItem(hDlg,103),TRUE);
									}
								else
									{
									EnableWindow(GetDlgItem(hDlg,102),FALSE);
									EnableWindow(GetDlgItem(hDlg,103),FALSE);
									}
								}
							else
								{
								if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED)
									{
									EnableWindow(GetDlgItem(hDlg,111),TRUE);
									EnableWindow(GetDlgItem(hDlg,112),TRUE);
									EnableWindow(GetDlgItem(hDlg,113),TRUE);
									EnableWindow(GetDlgItem(hDlg,114),TRUE);
									EnableWindow(GetDlgItem(hDlg,115),TRUE);
									}
								else
									{
									EnableWindow(GetDlgItem(hDlg,111),FALSE);
									EnableWindow(GetDlgItem(hDlg,112),FALSE);
									EnableWindow(GetDlgItem(hDlg,113),FALSE);
									EnableWindow(GetDlgItem(hDlg,114),FALSE);
									EnableWindow(GetDlgItem(hDlg,115),FALSE);
									}
								}
							break;

						case 102:
						case 103:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							CheckRadioButton(hDlg,102,103,(IsDlgButtonChecked(hDlg,102) == BST_CHECKED)?103:102);
							break;
						case 111:
						case 112:
						case 113:
						case 114:
						case 115:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							else CheckRadioButton(hDlg,111,115,LOWORD(wParam));
							break;
						case 145:
						case 146:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							CheckRadioButton(hDlg,145,146,(IsDlgButtonChecked(hDlg,145) == BST_CHECKED)?146:145);
							break;
						case 149:
						case 150:
							if (IsDlgButtonChecked(hDlg,LOWORD(wParam)) == BST_CHECKED) break;
							CheckRadioButton(hDlg,149,150,(IsDlgButtonChecked(hDlg,149) == BST_CHECKED)?150:149);
							break;

						case 144:
							if (IsDlgButtonChecked(hDlg,144) == BST_CHECKED)
								{
								EnableWindow(GetDlgItem(hDlg,145),TRUE);
								EnableWindow(GetDlgItem(hDlg,146),TRUE);
								EnableWindow(GetDlgItem(hDlg,147),TRUE);
								}
							else
								{
								EnableWindow(GetDlgItem(hDlg,145),FALSE);
								EnableWindow(GetDlgItem(hDlg,146),FALSE);
								EnableWindow(GetDlgItem(hDlg,147),FALSE);
								}
							break;
						case 147:
							Things_SelectOptions(hDlg,&Context->params.ModifyCrOptions,NULL,THING_OPTIONSCREATURES);
							break;

						case 148:
							if (IsDlgButtonChecked(hDlg,148) == BST_CHECKED)
								{
								EnableWindow(GetDlgItem(hDlg,149),TRUE);
								EnableWindow(GetDlgItem(hDlg,150),TRUE);
								EnableWindow(GetDlgItem(hDlg,151),TRUE);
								}
							else
								{
								EnableWindow(GetDlgItem(hDlg,149),FALSE);
								EnableWindow(GetDlgItem(hDlg,150),FALSE);
								EnableWindow(GetDlgItem(hDlg,151),FALSE);
								}
							break;
						case 151:
							Things_SelectOptions(hDlg,NULL,&Context->params.ModifyHrOptions,THING_OPTIONSHEROS);
							break;

						case IDOK:
							Things_ManagerModifySave(hDlg,Context);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation ««««««««««««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerModifyInit(HWND hDlg, MNGRMODPARAMSCTX *Context)
{
	LONG	X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//
	// Paramètres généraux
	//

	if (Context->params.ModifyFlags&MNGR_MODIFYCOORDS)	CheckDlgButton(hDlg,101,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_MODIFYTYPE)	CheckDlgButton(hDlg,104,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_MODIFYOWNER)	CheckDlgButton(hDlg,105,BST_CHECKED);

	if (Context->params.ModifyFlags&MNGR_FLAGIGNORETERRAIN) CheckDlgButton(hDlg,122,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_FLAGIGNOREFATALS)  CheckDlgButton(hDlg,123,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_FLAGIGNOREENEMIES) CheckDlgButton(hDlg,124,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_FLAGIGNOREINCOMP)  CheckDlgButton(hDlg,125,BST_CHECKED);
	if (Context->params.ModifyFlags&MNGR_FLAGIGNOREASK)	CheckDlgButton(hDlg,126,BST_CHECKED);

	if (Context->type == THINGMNGR_CREATURES)
		{
		CheckRadioButton(hDlg,102,103,102+Context->params.ModifyCoordsType);
		if (!(Context->params.ModifyFlags&MNGR_MODIFYCOORDS))
			{
			EnableWindow(GetDlgItem(hDlg,102),FALSE);
			EnableWindow(GetDlgItem(hDlg,103),FALSE);
			}
		}
	else
		{
		CheckRadioButton(hDlg,102,103,103);
		EnableWindow(GetDlgItem(hDlg,102),FALSE);
		EnableWindow(GetDlgItem(hDlg,103),FALSE);
		if (!(Context->params.ModifyFlags&MNGR_MODIFYCOORDS))
			{
			EnableWindow(GetDlgItem(hDlg,111),FALSE);
			EnableWindow(GetDlgItem(hDlg,112),FALSE);
			EnableWindow(GetDlgItem(hDlg,113),FALSE);
			EnableWindow(GetDlgItem(hDlg,114),FALSE);
			EnableWindow(GetDlgItem(hDlg,115),FALSE);
			}
		CheckRadioButton(hDlg,111,115,111+Context->params.ModifyRemplaceType);
		}

	//
	// Paramètres des créatures
	//

	if (Context->type == THINGMNGR_CREATURES)
		{
		if (Context->params.ModifyFlags&MNGR_MODIFYLEVEL)	CheckDlgButton(hDlg,141,BST_CHECKED);
		if (Context->params.ModifyFlags&MNGR_MODIFYGOLD)	CheckDlgButton(hDlg,142,BST_CHECKED);
		if (Context->params.ModifyFlags&MNGR_MODIFYHEALTH)	CheckDlgButton(hDlg,143,BST_CHECKED);
		if (Context->params.ModifyFlags&MNGR_MODIFYCROPTS)	CheckDlgButton(hDlg,144,BST_CHECKED);
		if (Context->params.ModifyFlags&MNGR_MODIFYHEROESOPTS)	CheckDlgButton(hDlg,148,BST_CHECKED);
		if (Context->params.ModifyFlags&MNGR_MODIFYOBJECTIVE)	CheckDlgButton(hDlg,152,BST_CHECKED);

		CheckRadioButton(hDlg,145,146,145+Context->params.ModifyCrOptionsType);
		CheckRadioButton(hDlg,149,150,149+Context->params.ModifyHrOptionsType);

		if (!(Context->params.ModifyFlags&MNGR_MODIFYCROPTS))
			{
			EnableWindow(GetDlgItem(hDlg,145),FALSE);
			EnableWindow(GetDlgItem(hDlg,146),FALSE);
			EnableWindow(GetDlgItem(hDlg,147),FALSE);
			}
		if (!(Context->params.ModifyFlags&MNGR_MODIFYHEROESOPTS))
			{
			EnableWindow(GetDlgItem(hDlg,149),FALSE);
			EnableWindow(GetDlgItem(hDlg,150),FALSE);
			EnableWindow(GetDlgItem(hDlg,151),FALSE);
			}
		}

	//
	// Paramètres des autres éléments
	//

	if (Context->type != THINGMNGR_CREATURES)
		{
		if (Context->params.ModifyFlags&MNGR_MODIFYDATA) CheckDlgButton(hDlg,160,BST_CHECKED);
		}

	return;
}


// «»»» Sauvegarde des paramètres «««««««««««««««««««««««««««««««««««««««»

void Things_ManagerModifySave(HWND hDlg, MNGRMODPARAMSCTX *Context)
{

	//
	// Paramètres généraux
	//

	Context->params.ModifyFlags &= ~(MNGR_MODIFYCOORDS|MNGR_MODIFYTYPE|MNGR_MODIFYOWNER);
	if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYCOORDS;
	if (IsDlgButtonChecked(hDlg,104) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYTYPE;
	if (IsDlgButtonChecked(hDlg,105) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYOWNER;

	if (Context->type == THINGMNGR_CREATURES)
		{
		if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED) Context->params.ModifyCoordsType = MNGR_COORDSAME;
		else Context->params.ModifyCoordsType = MNGR_COORDRELATIVE;
		}
	else
		{
		if (IsDlgButtonChecked(hDlg,111) == BST_CHECKED) Context->params.ModifyRemplaceType = MNGR_REMPLACEALL;
		else if (IsDlgButtonChecked(hDlg,112) == BST_CHECKED) Context->params.ModifyRemplaceType = MNGR_REMPLACESAME;
		else if (IsDlgButtonChecked(hDlg,113) == BST_CHECKED) Context->params.ModifyRemplaceType = MNGR_REMPLACEFAMILY;
		else if (IsDlgButtonChecked(hDlg,114) == BST_CHECKED) Context->params.ModifyRemplaceType = MNGR_REMPLACENONE;
		else if (IsDlgButtonChecked(hDlg,115) == BST_CHECKED) Context->params.ModifyRemplaceType = MNGR_REMPLACEASK;
		}

	Context->params.ModifyFlags &= ~(MNGR_FLAGIGNORETERRAIN|MNGR_FLAGIGNOREFATALS|MNGR_FLAGIGNOREENEMIES|MNGR_FLAGIGNOREINCOMP|MNGR_FLAGIGNOREASK);
	if (IsDlgButtonChecked(hDlg,122) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_FLAGIGNORETERRAIN;
	if (IsDlgButtonChecked(hDlg,123) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_FLAGIGNOREFATALS;
	if (IsDlgButtonChecked(hDlg,124) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_FLAGIGNOREENEMIES;
	if (IsDlgButtonChecked(hDlg,125) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_FLAGIGNOREINCOMP;
	if (IsDlgButtonChecked(hDlg,126) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_FLAGIGNOREASK;

	//
	// Paramètres des créatures
	//

	if (Context->type == THINGMNGR_CREATURES)
		{
		Context->params.ModifyFlags &= ~(MNGR_MODIFYLEVEL|MNGR_MODIFYGOLD|MNGR_MODIFYHEALTH|MNGR_MODIFYCROPTS|MNGR_MODIFYHEROESOPTS|MNGR_MODIFYOBJECTIVE);
		if (IsDlgButtonChecked(hDlg,141) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYLEVEL;
		if (IsDlgButtonChecked(hDlg,142) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYGOLD;
		if (IsDlgButtonChecked(hDlg,143) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYHEALTH;
		if (IsDlgButtonChecked(hDlg,144) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYCROPTS;
		if (IsDlgButtonChecked(hDlg,148) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYHEROESOPTS;
		if (IsDlgButtonChecked(hDlg,152) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYOBJECTIVE;

		if (IsDlgButtonChecked(hDlg,145) == BST_CHECKED) Context->params.ModifyCrOptionsType = MNGR_MODIFYALL;
		else Context->params.ModifyCrOptionsType = MNGR_MODIFYMASK;
		if (IsDlgButtonChecked(hDlg,149) == BST_CHECKED) Context->params.ModifyHrOptionsType = MNGR_MODIFYALL;
		else Context->params.ModifyHrOptionsType = MNGR_MODIFYMASK;
		}

	//
	// Paramètres des autres éléments
	//

	if (Context->type != THINGMNGR_CREATURES)
		{
		Context->params.ModifyFlags &= ~MNGR_MODIFYDATA;
		if (IsDlgButtonChecked(hDlg,160) == BST_CHECKED) Context->params.ModifyFlags |= MNGR_MODIFYDATA;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Aperçu des changements						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Things_ManagerPreview(HWND hDlg, NODE *WorkList, UINT ModifyMode, MAPTHINGMANAGER *Manager)
{
	UINT	ResID;
	int	Result;

	Manager->beforelist = NULL;
	Manager->afterlist = NULL;
	Manager->worklist = WorkList;
	Manager->mode = ModifyMode;

	if (!Misc_CreateImageList(PvIcons,&Manager->icons,32,32,FALSE))
		{
		Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		return(0);
		}

	switch(ModifyMode)
		{
		case MNGR_PREVIEWMODIFY:
			ResID = 6502;
			break;
		case MNGR_PREVIEWADD:
			ResID = 6503;
			break;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(ResID),hDlg,Things_ManagerPreviewProc,(LPARAM)Manager);
	if (Result == -1)
		{
		Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
		Result = 0;
		}

	Misc_DestroyImageList(&Manager->icons);
	return(Result);
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Things_ManagerPreviewProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGMANAGER	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Things_ManagerPreviewInit(hDlg,(MAPTHINGMANAGER *)lParam);
		return(TRUE);
		}

	Context = (MAPTHINGMANAGER *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
			return(TRUE);

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->hwndItem == Context->pbslist)
				{
				Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+5,(1<<1));
				return(TRUE);
				}
			switch(Context->type)
				{
				case THINGMNGR_CREATURES:
					Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+7,(1<<2)|(1<<3)|(1<<4)|(1<<7));
					break;
				case THINGMNGR_DOORSNTRAPS:
				case THINGMNGR_OBJECTS:
					Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1+3,(1<<3));
					break;
				}
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == Context->pbslist)
						{
						if (SendMessage(Context->pbslist,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0) == 0)
							{
							EnableWindow(GetDlgItem(hDlg,104),FALSE);
							EnableWindow(GetDlgItem(hDlg,105),FALSE);
							EnableWindow(GetDlgItem(hDlg,106),FALSE);
							}
						else
							{
							EnableWindow(GetDlgItem(hDlg,104),TRUE);
							if (Context->mode == MNGR_PREVIEWMODIFY) EnableWindow(GetDlgItem(hDlg,105),TRUE);
							EnableWindow(GetDlgItem(hDlg,106),TRUE);
							}
						return(TRUE);
						}
					break;

				case LVN_GETDISPINFO:
					if (((NMHDR *)lParam)->hwndFrom != Context->pbslist)
						Things_ManagerPreviewDispInfo((LV_DISPINFO *)lParam,Context);
					else
						Things_ManagerPreviewPbsInfo((LV_DISPINFO *)lParam,Context);
					return(TRUE);

				case NM_CLICK:
					if (((NMHDR *)lParam)->hwndFrom == Context->pbslist)
						Things_ManagerPreviewSelect(Context);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 104:
						case 105:
						case 106:
							Things_ManagerPreviewToggle(LOWORD(wParam)-104,Context);
							break;

						case IDOK:
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation ««««««««««««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerPreviewInit(HWND hDlg, MAPTHINGMANAGER *Manager)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	MAPTHINGMNGRTMP	*Template;
	LONG		 X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Manager);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//
	// Initialisation de la liste des éléments précédents
	// ** Uniquement pour la modification **
	//

	if (Manager->mode == MNGR_PREVIEWMODIFY)
		{
		GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
		MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
		Manager->beforelist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
		if (!Manager->beforelist) goto Error;

		SendMessage(Manager->beforelist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Manager->images);

		ZeroMemory(&Column,sizeof(LV_COLUMN));
		X = 0;
		Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH|LVCF_TEXT;
		Column.fmt = LVCFMT_LEFT;
		Column.cx = Font.FontMaxWidth*20;
		Column.pszText = szManagerName;
		Column.iSubItem = X;
		if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
		Column.cx = Font.FontMaxWidth*8;
		Column.pszText = szManagerOwner;
		Column.iSubItem = X;
		if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;

		switch(Manager->type)
			{
			case THINGMNGR_CREATURES:
				Column.fmt = LVCFMT_CENTER;
				Column.cx = Font.FontMaxWidth*4;
				Column.pszText = szManagerLevel;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				Column.cx = Font.FontMaxWidth*5;
				Column.pszText = szManagerGoldHeld;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				Column.cx = Font.FontMaxWidth*5;
				Column.pszText = szManagerHealth;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				Column.fmt = LVCFMT_LEFT;
				Column.cx = Font.FontMaxWidth*10;
				Column.pszText = szManagerOptions;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				Column.fmt = LVCFMT_LEFT;
				Column.cx = Font.FontMaxWidth*15;
				Column.pszText = szManagerObjective;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				break;
			case THINGMNGR_DOORSNTRAPS:
				Column.cx = Font.FontMaxWidth*6;
				Column.pszText = szManagerStatut;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				break;
			case THINGMNGR_OBJECTS:
				Column.cx = Font.FontMaxWidth*15;
				Column.pszText = szManagerData;
				Column.iSubItem = X;
				if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
				break;
			}

		Column.fmt = LVCFMT_CENTER;
		Column.cx = Font.FontMaxWidth*8;
		Column.pszText = szManagerPosition;
		Column.iSubItem = X;
		if (SendMessage(Manager->beforelist,LVM_INSERTCOLUMN,(WPARAM)X,(LPARAM)&Column) == -1) goto Error;

		ZeroMemory(&Item,sizeof(LV_ITEM));
		Item.iSubItem = 0;
		X = 0;
		Y = -1;

		while ((Y = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Y,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
			{
			Item.mask = LVIF_PARAM;
			Item.iItem = Y;
			if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
			if (!Item.lParam) continue;

			Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			Item.iItem = X++;
			Item.pszText = LPSTR_TEXTCALLBACK;
			Item.iImage = I_IMAGECALLBACK;
			if (SendMessage(Manager->beforelist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error;
			}
		}

	//
	// Initialisation de la liste des nouveaux éléments
	//

	GetWindowRect(GetDlgItem(hDlg,101),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Manager->afterlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Manager->afterlist) goto Error;

	SendMessage(Manager->afterlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Manager->images);

	ZeroMemory(&Column,sizeof(LV_COLUMN));
	X = 0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH|LVCF_TEXT;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szManagerName;
	Column.iSubItem = X;
	if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szManagerOwner;
	Column.iSubItem = X;
	if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;

	switch(Manager->type)
		{
		case THINGMNGR_CREATURES:
			Column.fmt = LVCFMT_CENTER;
			Column.cx = Font.FontMaxWidth*4;
			Column.pszText = szManagerLevel;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.cx = Font.FontMaxWidth*5;
			Column.pszText = szManagerGoldHeld;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.cx = Font.FontMaxWidth*5;
			Column.pszText = szManagerHealth;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = Font.FontMaxWidth*10;
			Column.pszText = szManagerOptions;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			Column.fmt = LVCFMT_LEFT;
			Column.cx = Font.FontMaxWidth*15;
			Column.pszText = szManagerObjective;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			break;
		case THINGMNGR_DOORSNTRAPS:
			Column.cx = Font.FontMaxWidth*6;
			Column.pszText = szManagerStatut;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			break;
		case THINGMNGR_OBJECTS:
			Column.cx = Font.FontMaxWidth*15;
			Column.pszText = szManagerData;
			Column.iSubItem = X;
			if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X++,(LPARAM)&Column) == -1) goto Error;
			break;
		}

	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*8;
	Column.pszText = szManagerPosition;
	Column.iSubItem = X;
	if (SendMessage(Manager->afterlist,LVM_INSERTCOLUMN,(WPARAM)X,(LPARAM)&Column) == -1) goto Error;

	ZeroMemory(&Item,sizeof(LV_ITEM));
	Item.iSubItem = 0;
	X = 0;
	Y = -1;

	if (Manager->mode == MNGR_PREVIEWADD)
		{
		Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		Item.iItem = 0;
		for (Template = (MAPTHINGMNGRTMP *)Manager->worklist->next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
			{
			Item.pszText = LPSTR_TEXTCALLBACK;
			Item.iImage = I_IMAGECALLBACK;
			Item.lParam = (LPARAM)Template;
			if (SendMessage(Manager->afterlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error;
			Item.iItem++;
			}
		}
	else
		{
		while ((Y = SendMessage(Manager->list,LVM_GETNEXTITEM,(WPARAM)Y,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
			{
			Item.mask = LVIF_PARAM;
			Item.iItem = Y;
			if (!SendMessage(Manager->list,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
			if (!Item.lParam) continue;

			for (Template = (MAPTHINGMNGRTMP *)Manager->worklist->next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
				if (Template->srcthing == (MAPTHING *)Item.lParam) break;

			Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			Item.iItem = X++;
			Item.pszText = LPSTR_TEXTCALLBACK;
			Item.iImage = I_IMAGECALLBACK;
			Item.lParam = (LPARAM)Template;
			if (SendMessage(Manager->afterlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error;
			}
		}

	//
	// Initialisation des problèmes
	//

	GetWindowRect(GetDlgItem(hDlg,102),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Manager->pbslist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Manager->pbslist) goto Error;
	SendMessage(Manager->pbslist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Manager->icons);

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH|LVCF_TEXT;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*10;
	Column.pszText = szManagerPreviewAction;
	Column.iSubItem = 0;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*10;
	Column.pszText = szManagerPreviewPosition;
	Column.iSubItem = 1;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szManagerPreviewTerrain;
	Column.iSubItem = 2;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column) == -1) goto Error;
	Column.cx = Font.FontMaxWidth*30;
	Column.pszText = szManagerPreviewTerrainProblem;
	Column.iSubItem = 3;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)3,(LPARAM)&Column) == -1) goto Error;
	Column.cx = Font.FontMaxWidth*30;
	Column.pszText = szManagerPreviewIncompProblem;
	Column.iSubItem = 4;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)4,(LPARAM)&Column) == -1) goto Error;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szManagerPreviewRemplace;
	Column.iSubItem = 5;
	if (SendMessage(Manager->pbslist,LVM_INSERTCOLUMN,(WPARAM)5,(LPARAM)&Column) == -1) goto Error;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iSubItem = 0;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.iImage = I_IMAGECALLBACK;

	for (X = 0, Template = (MAPTHINGMNGRTMP *)Manager->worklist->next; Template != NULL; Template = (MAPTHINGMNGRTMP *)Template->node.next)
		{
		if (Template->verified) continue;
		if (!Template->warnflags) continue;
		Item.iItem = X++;
		Item.lParam = (LPARAM)Template;
		if (SendMessage(Manager->pbslist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error;
		}

	return;

//
//--- Erreurs ---
//

Error:	EndDialog(hDlg,-1);
	return;
}


// «»»» Modifie le type de modification «««««««««««««««««««««««««««««««««»

void Things_ManagerPreviewToggle(UINT NewMode, MAPTHINGMANAGER *Manager)
{
	LV_ITEM	Item;

	Item.iItem = -1;
	Item.iSubItem = 0;

	while ((Item.iItem = SendMessage(Manager->pbslist,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		Item.mask = LVIF_PARAM;
		if (!SendMessage(Manager->pbslist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;

		switch(NewMode)
			{
			case 0:	// Ne rien faire
				((MAPTHINGMNGRTMP *)Item.lParam)->modify = FALSE;
				((MAPTHINGMNGRTMP *)Item.lParam)->remove = FALSE;
				break;
			case 1:	// Supprimer
				((MAPTHINGMNGRTMP *)Item.lParam)->modify = FALSE;
				((MAPTHINGMNGRTMP *)Item.lParam)->remove = TRUE;
				break;
			case 2:	// Modifier/Ajouter
				if (((MAPTHINGMNGRTMP *)Item.lParam)->locked) break;
				((MAPTHINGMNGRTMP *)Item.lParam)->modify = TRUE;
				((MAPTHINGMNGRTMP *)Item.lParam)->remove = FALSE;
				break;
			}
		}

	InvalidateRect(Manager->pbslist,NULL,FALSE);
	InvalidateRect(Manager->afterlist,NULL,FALSE);
	return;
}


// «»»» Sélectionne les éléments en relation avec les problèmes «««««««««»

void Things_ManagerPreviewSelect(MAPTHINGMANAGER *Manager)
{
	LV_ITEM	Item;
	LV_ITEM	ListItem;
	int	FirstItem;

	Item.iSubItem = 0;
	ListItem.iSubItem = 0;
	FirstItem = 0x7FFFFFFF;

	//
	// Unselect all selected items in the lists
	//

	ListItem.iItem = -1;
	while ((ListItem.iItem = SendMessage(Manager->afterlist,LVM_GETNEXTITEM,(WPARAM)ListItem.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
		{
		ListItem.stateMask = LVIS_SELECTED;
		ListItem.state = 0;
		SendMessage(Manager->afterlist,LVM_SETITEMSTATE,(WPARAM)ListItem.iItem,(LPARAM)&ListItem);
		if (Manager->mode == MNGR_PREVIEWMODIFY) SendMessage(Manager->beforelist,LVM_SETITEMSTATE,(WPARAM)ListItem.iItem,(LPARAM)&ListItem);
		}

	//
	// Find associated items in the lists
	//

	Item.iItem = -1;
	while ((Item.iItem = SendMessage(Manager->pbslist,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		Item.mask = LVIF_PARAM;
		if (!SendMessage(Manager->pbslist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;

		ListItem.iItem = -1;
		while ((ListItem.iItem = SendMessage(Manager->afterlist,LVM_GETNEXTITEM,(WPARAM)ListItem.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
			{
			ListItem.mask = LVIF_PARAM;
			if (!SendMessage(Manager->afterlist,LVM_GETITEM,(WPARAM)0,(LPARAM)&ListItem)) continue;
			if (ListItem.lParam == Item.lParam) break;
			}

		if (ListItem.iItem == -1) continue;

		if (ListItem.iItem < FirstItem) FirstItem = ListItem.iItem;
		ListItem.stateMask = LVIS_SELECTED;
		ListItem.state = LVIS_SELECTED;
		SendMessage(Manager->afterlist,LVM_SETITEMSTATE,(WPARAM)ListItem.iItem,(LPARAM)&ListItem);
		if (Manager->mode == MNGR_PREVIEWMODIFY) SendMessage(Manager->beforelist,LVM_SETITEMSTATE,(WPARAM)ListItem.iItem,(LPARAM)&ListItem);
		}

	//
	// Make first item visible
	//

	if (FirstItem != 0x7FFFFFFF)
		{
		if (Manager->mode == MNGR_PREVIEWMODIFY) SendMessage(Manager->beforelist,LVM_ENSUREVISIBLE,(WPARAM)FirstItem,(LPARAM)FALSE);
		SendMessage(Manager->afterlist,LVM_ENSUREVISIBLE,(WPARAM)FirstItem,(LPARAM)FALSE);
		}

	return;
}


// «»»» Affichage des problèmes «««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerPreviewPbsInfo(LV_DISPINFO *DispInfo, MAPTHINGMANAGER *Manager)
{
	MAPTHINGMNGRTMP	*Template;
	char		*Text;

	Template = (MAPTHINGMNGRTMP *)DispInfo->item.lParam;
	if (!Template) return;

	if (DispInfo->item.mask&LVIF_IMAGE)
		{
		if (Template->locked)
			DispInfo->item.iImage = 0;
		else
			{
			DispInfo->item.iImage = -1;
			if (Template->warnflags&(MNGR_WARNBADTERRAIN|MNGR_WARNENEMY|MNGR_WARNFATAL|MNGR_WARNINCOMPATIBLE))
				DispInfo->item.iImage = 2;
			if (Template->warnflags&(MNGR_WARNOUTOFMAP|MNGR_WARNREMPLACE))
				DispInfo->item.iImage = 1;
			}
		if (Template->modify) DispInfo->item.iImage = -1;
		}

	if (DispInfo->item.mask&LVIF_TEXT)
		{
		switch(DispInfo->item.iSubItem)
			{
			case 0:	if (Template->remove) strcpy(DispInfo->item.pszText,szManagerPreviewActionDelete);
				else if (Template->modify) strcpy(DispInfo->item.pszText,(Manager->mode == MNGR_PREVIEWMODIFY)?szManagerPreviewActionModify:szManagerPreviewActionAdd);
				else strcpy(DispInfo->item.pszText,szManagerPreviewActionIgnore);
				break;
			case 1:	if ((Template->newthing.x >= Map.Properties.Width)||(Template->newthing.y >= Map.Properties.Height))
					strcpy(DispInfo->item.pszText,szManagerPreviewBad);
				else
					wsprintf(DispInfo->item.pszText,szManagerPreviewPosFmt,Template->newthing.x,Template->newthing.y);
				break;
			case 2:	if ((Template->newthing.x >= Map.Properties.Width)||(Template->newthing.y >= Map.Properties.Height))
					strcpy(DispInfo->item.pszText,szManagerPreviewBad);
				else
					{
					BYTE *MapPtr = Map.Map+Template->newthing.x*MAP_CONTROLBYTES+Template->newthing.y*Map.Properties.Width*MAP_CONTROLBYTES;
					switch(*(MapPtr+3))
						{
						case 0:	strcpy(DispInfo->item.pszText,World[*MapPtr].Name);
							break;
						case 1:	strcpy(DispInfo->item.pszText,Rooms[*MapPtr].Name);
							strcat(DispInfo->item.pszText,szMapSeparator);
							strcat(DispInfo->item.pszText,Players[*(MapPtr+1)-1].Name);
							break;
						case 2:	strcpy(DispInfo->item.pszText,Gates[*MapPtr].Name);
							break;
						case 3:	strcpy(DispInfo->item.pszText,Walls[*MapPtr].Name);
							strcat(DispInfo->item.pszText,szMapSeparator);
							strcat(DispInfo->item.pszText,Players[*(MapPtr+1)-1].Name);
							break;
						}
					}
				break;
			case 3:	strcpy(DispInfo->item.pszText,Template->text);
				break;
			case 4:	strcpy(DispInfo->item.pszText,Template->warntext);
				break;
			case 5:	if (!Template->oldthing)
					{
					strcpy(DispInfo->item.pszText,szManagerPreviewNothing);
					}
				else
					{
					switch(Template->oldthing->node.type)
						{
						case THING_CREATURE:
							Text = Things_GetCreatureNameFromID(Template->oldthing->id);
							break;
						case THING_DOOR:
							Text = Things_GetThingNameFromID(Template->oldthing->id,Doors);
							break;
						case THING_TRAP:
							Text = Things_GetThingNameFromID(Template->oldthing->id,Traps);
							break;
						case THING_OBJECT:
							Text = Things_GetThingNameFromID(Template->oldthing->id,Objects);
							break;
						case THING_MAGICALOBJECT:
							Text = Things_GetThingNameFromID(Template->oldthing->id,Magics);
							break;
						}
					if (Text) strcpy(DispInfo->item.pszText,Text);
					else strcpy(DispInfo->item.pszText,szManagerPreviewNothing);
					}
				break;
			}
		}
	return;

}


// «»»» Affichage des infos «««««««««««««««««««««««««««««««««««««««««««««»

void Things_ManagerPreviewDispInfo(LV_DISPINFO *DispInfo, MAPTHINGMANAGER *Manager)
{
	LV_DISPINFO	 TempInfo;
	MAPTHINGMNGRTMP	*Template;

	//
	//--- Fait scroller les deux listes en même temps ---
	//

	if (Manager->mode == MNGR_PREVIEWMODIFY)
		{
		LONG		 BeforeTop;
		LONG		 AfterTop;

		BeforeTop = SendMessage(Manager->beforelist,LVM_GETTOPINDEX,(WPARAM)0,(LPARAM)0);
		AfterTop = SendMessage(Manager->afterlist,LVM_GETTOPINDEX,(WPARAM)0,(LPARAM)0);

		if (BeforeTop != AfterTop)
			{
			int	Dista;
			int	Mult;

			Mult = Font.FontHeight+2;
			if (Mult < 32+2) Mult = 32+2;
			if (DispInfo->hdr.hwndFrom == Manager->beforelist)
				{
				Dista = BeforeTop-AfterTop;
				SendMessage(Manager->afterlist,LVM_SCROLL,(WPARAM)0,(LPARAM)Dista*Mult);
				}
			if (DispInfo->hdr.hwndFrom == Manager->afterlist)
				{
				Dista = AfterTop-BeforeTop;
				SendMessage(Manager->beforelist,LVM_SCROLL,(WPARAM)0,(LPARAM)Dista*Mult);
				}
			}
		}

	//
	//--- Liste "Avant" ---
	//

	if (DispInfo->hdr.hwndFrom == Manager->beforelist)
		{
		Things_ManagerSetDispInfo(DispInfo);
		return;
		}

	//
	//--- Liste "Après" ---
	//

	//
	// L'entrée n'est pas répertoriée
	// -> élément utilisé par les évènements et modification abandonnée
	//

	Template = (MAPTHINGMNGRTMP *)DispInfo->item.lParam;
	if (!Template)
		{
		if (DispInfo->item.mask&LVIF_IMAGE)
			DispInfo->item.iImage = -1;

		if (DispInfo->item.mask&LVIF_TEXT)
			{
			switch(DispInfo->item.iSubItem)
				{
				case 0:	strcpy(DispInfo->item.pszText,szManagerPreviewTriggers);
					break;
				default:*DispInfo->item.pszText = 0;
					break;
				}
			}
		return;
		}

	//
	// L'entrée est répertoriée
	//

	CopyMemory(&TempInfo,DispInfo,sizeof(LV_DISPINFO));
	if (Template->modify)
		{
		TempInfo.item.lParam = (LPARAM)&Template->newthing;
		Things_ManagerSetDispInfo(&TempInfo);
		if (DispInfo->item.mask&LVIF_IMAGE) DispInfo->item.iImage = TempInfo.item.iImage;
		if (DispInfo->item.mask&LVIF_TEXT) strcpy(DispInfo->item.pszText,TempInfo.item.pszText);
		return;
		}
	else
		{
		if (DispInfo->item.mask&LVIF_IMAGE)
			DispInfo->item.iImage = -1;

		if (DispInfo->item.mask&LVIF_TEXT)
			{
			switch(DispInfo->item.iSubItem)
				{
				case 0:	if (Template->remove) strcpy(DispInfo->item.pszText,szManagerPreviewDeleted);
					else strcpy(DispInfo->item.pszText,(Manager->mode == MNGR_PREVIEWMODIFY)?szManagerPreviewNotChanged:szManagerPreviewNotAdded);
					break;
				default:*DispInfo->item.pszText = 0;
					break;
				}
			}
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
