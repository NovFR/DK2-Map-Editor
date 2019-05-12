
//<<>-<>>---------------------------------------------------------------------()
/*
	Sous-routines liées à l'affichage.
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
extern NODE		MapGates;
extern NODE		MapHerosBands;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve l'identificateur d'une image dans la liste «««««««««««««»

int Triggers_DisplayGetImageId(DWORD ObjectID, DWORD ObjectType)
{
	ELEMENT	*Table;
	int	 Result = 0;
	int	 i;

	switch(ObjectType)
		{
		case TRIGGERIMAGE_CREATURE:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++)
				if (ObjectID == BadCreatures[i].Id) return(Result);
			for (i = 0; BadElites[i].Id != 0; i++, Result++)
				if (ObjectID == BadElites[i].Id) return(Result);
			for (i = 0; Heros[i].Id != 0; i++, Result++)
				if (ObjectID == Heros[i].Id) return(Result);
			return(0);
		case TRIGGERIMAGE_BAD:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			Table = BadCreatures;
			break;
		case TRIGGERIMAGE_ELITE:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			Table = BadElites;
			break;
		case TRIGGERIMAGE_HERO:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			Table = Heros;
			break;
		case TRIGGERIMAGE_DOOR:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			for (i = 0; Heros[i].Id != 0; i++, Result++);
			Table = Doors;
			break;
		case TRIGGERIMAGE_TRAP:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			for (i = 0; Heros[i].Id != 0; i++, Result++);
			for (i = 0; Doors[i].Id != 0; i++, Result++);
			Table = Traps;
			break;
		case TRIGGERIMAGE_SPELL:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			for (i = 0; Heros[i].Id != 0; i++, Result++);
			for (i = 0; Doors[i].Id != 0; i++, Result++);
			for (i = 0; Traps[i].Id != 0; i++, Result++);
			Table = Spells;
			break;
		case TRIGGERIMAGE_ROOM:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			for (i = 0; Heros[i].Id != 0; i++, Result++);
			for (i = 0; Doors[i].Id != 0; i++, Result++);
			for (i = 0; Traps[i].Id != 0; i++, Result++);
			for (i = 0; Spells[i].Id != 0; i++, Result++);
			Table = Terrain;
			break;
		case TRIGGERIMAGE_PLAYER:
			for (i = 0; TrSpecial[i].Id != 0; i++, Result++);
			for (i = 0; BadCreatures[i].Id != 0; i++, Result++);
			for (i = 0; BadElites[i].Id != 0; i++, Result++);
			for (i = 0; Heros[i].Id != 0; i++, Result++);
			for (i = 0; Doors[i].Id != 0; i++, Result++);
			for (i = 0; Traps[i].Id != 0; i++, Result++);
			for (i = 0; Spells[i].Id != 0; i++, Result++);
			for (i = 0; Terrain[i].Id != 0; i++, Result++);
			Table = TrPlayers;
			break;
		default:
			return(0);
		}

	for (i = 0; Table[i].Id != 0; i++, Result++)
		if (Table[i].Variable == ObjectID) break;

	if (Table[i].Id == 0)
		return(0);

	return(Result);
}


// «»»» Création de la liste des passages «««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateGatesList(HWND hDlg, UINT CtlID)
{
	MAPRECTEX	*Gate;
	char		*Text;

	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text) return(0);

	for (Gate = (MAPRECTEX *)MapGates.next; Gate != 0; Gate = (MAPRECTEX *)Gate->node.next)
		{
		RectEx_StrCpy(Text+2048,Gate->name);
		RectEx_StrCpy(Text+2048+MAP_RECTMAXNAME,Gate->info);
		if (Gate->info[0]) wsprintf(Text,szTriggersGateInfo,Gate->id,Text+2048,Text+2048+MAP_RECTMAXNAME);
		else wsprintf(Text,szTriggersGate,Gate->id,Text+2048);
		if (SendDlgItemMessage(hDlg,CtlID,CB_ADDSTRING,(WPARAM)0,(LPARAM)Text) < 0)
			{
			GlobalFree(Text);
			return(0);
			}
		}

	GlobalFree(Text);
	return(1);
}


// «»»» Création de la liste des terrains «««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateTerrainList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD TerrainID, BOOL ShowRooms, BOOL SpecialRooms)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	int		 Selected;
	int		 i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	for (i = 0; Terrain[i].Id != 0; i++)
		{
		Item.pszText = Terrain[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(Terrain[i].Variable,TRIGGERIMAGE_ROOM);
		Item.state = 0;
		Item.lParam = Terrain[i].Id>>24;
		if (((char)Terrain[i].Variable > 0)&&(!ShowRooms)) continue;
		if ((Terrain[i].Variable == 5)&&(!SpecialRooms)) continue;
		if ((Terrain[i].Variable == 3)&&(!SpecialRooms)) continue;
		if ((Terrain[i].Variable == 5)&&(!ShowRooms)) continue;
		if ((Terrain[i].Variable == 3)&&(!ShowRooms)) continue;
		if (TerrainID == Item.lParam)
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des bandes de héros ««««««««««««««««««««««««»

int Triggers_DisplayCreateHerosList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD BandID)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	MTHBAND		*Band;
	char		*Text;
	int		 Selected;
	int		 i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Text = GlobalAlloc(GPTR,2048+2048);
	if (!Text) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = Text;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
		{
		if (Band->name[0]) wsprintf(Text,szTriggersBandInfo,Band->id,Band->name);
		else wsprintf(Text,szTriggersBand,Band->id);
		Item.iImage = 3;
		Item.state = 0;
		Item.lParam = Band->id+1;
		if (BandID == Band->id)
			{
			Selected = Item.iItem;
			Item.state = LVIS_SELECTED;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
		Item.iItem++;
		for (i = 0; i != THING_MAXHEROSINBAND; i++)
			{
			if (!Band->heros[i].isvalid) break;
			if (Band->heros[i].options2&0x02)
				{
				wsprintf(Text,szTriggersCreatureInBandLeader,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
				switch(Band->heros[i].objective)
					{
					case 0x0B:
						strcat(Text,szHerosObjectives0B);
						break;
					case 0x0C:
						strcat(Text,szHerosObjectives0C);
						break;
					case 0x0D:
						strcat(Text,szHerosObjectives0D);
						break;
					case 0x0E:
						strcat(Text,szHerosObjectives0E);
						break;
					case 0x11:
						strcat(Text,szHerosObjectives11);
						break;
					case 0x12:
						strcat(Text,szHerosObjectives12);
						break;
					case 0x13:
						strcat(Text,szHerosObjectives13);
						break;
					case 0x16:
						strcat(Text,szHerosObjectives16);
						break;
					case 0x17:
						wsprintf(Text+2048,szHerosFmtObjectives17,Band->heros[i].area);
						strcat(Text,Text+2048);
						break;
					case 0x1B:
						strcat(Text,szHerosObjectives1B);
						break;
					default:
						strcat(Text,szUnknown);
						break;
					}
				}
			else
				wsprintf(Text,szTriggersCreatureInBand,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
			Item.iImage = Triggers_DisplayGetImageId(Band->heros[i].id,TRIGGERIMAGE_CREATURE);
			Item.state = 0;
			Item.lParam = 0;
			if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
			Item.iItem++;
			}
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	GlobalFree(Text);
	return(1);

//--- Erreurs ---

Error_0:GlobalFree(Text);
	return(0);
}


// «»»» Création de la liste des boutons de l'interface «««««««««««««««««»

int Triggers_DisplayCreateButtonsList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD Type, DWORD ID, BOOL Click)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i,j;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	//--- Boutons de l'interface ---

	if (!Click)
		{
		for (i = 0, j = 8; TrButtons0[i].Id != 0; i++, j++, Item.iItem++)
			{
			Item.pszText = TrButtons0[i].Name;
			Item.iImage = j;
			Item.state = 0;
			Item.lParam = TrButtons0[i].Id;
			if ((!Type)&&(TrButtons0[i].Id == ID))
				{
				Item.state = LVIS_SELECTED;
				Selected = Item.iItem;
				}
			if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
			}
		}

	if ((Click)&&(Click != -1))
		{
		for (i = 0, j = 10; TrButtons1[i].Id != 0; i++, j++, Item.iItem++)
			{
			Item.pszText = TrButtons1[i].Name;
			Item.iImage = j;
			Item.state = 0;
			Item.lParam = TrButtons1[i].Id;
			if ((!Type)&&(TrButtons1[i].Id == ID))
				{
				Item.state = LVIS_SELECTED;
				Selected = Item.iItem;
				}
			if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
			}
		}

	//--- Boutons des créatures ---

	if (Click != -1)
		{
		Item.pszText = szTriggersCreaturesText;
		Item.iImage = 14;
		Item.state = 0;
		Item.lParam = 0x000000F8;
		if ((!Type)&&(ID == 0xF8))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	//--- Boutons des salles ---

	if (Click == -1)
		{
		Item.pszText = szTriggersRoomsElem;
		Item.iImage = 0;
		Item.state = 0;
		Item.lParam = 0;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}
	else
		{
		Item.pszText = szTriggersRoomsText;
		Item.iImage = 15;
		Item.state = 0;
		Item.lParam = 0x000000FB;
		if ((!Type)&&(ID == 0xFB))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0; VarRooms[i].Id != 0; i++, Item.iItem++)
		{
		Item.pszText = VarRooms[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(VarRooms[i].Variable,TRIGGERIMAGE_ROOM);
		Item.state = 0;
		Item.lParam = VarRooms[i].Variable|(1<<16);
		if ((Type == 1)&&(VarRooms[i].Variable == ID))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		}

	//--- Boutons des sorts ---

	if (Click == -1)
		{
		Item.pszText = szTriggersSpellsElem;
		Item.iImage = 0;
		Item.state = 0;
		Item.lParam = 0;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}
	else
		{
		Item.pszText = szTriggersSpellsText;
		Item.iImage = 16;
		Item.state = 0;
		Item.lParam = 0x000000FA;
		if ((!Type)&&(ID == 0xFA))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0; Spells[i].Id != 0; i++, Item.iItem++)
		{
		if ((Spells[i].Variable == 0x05)&&(!Click))
			{
			Item.iItem--;
			continue;
			}
		Item.pszText = Spells[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(Spells[i].Variable,TRIGGERIMAGE_SPELL);
		Item.state = 0;
		Item.lParam = Spells[i].Variable|(5<<16);
		if ((Type == 5)&&(Spells[i].Variable == ID))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		}

	//--- Boutons des objets de l'atelier ---

	if (Click == -1)
		{
		Item.pszText = szTriggersDoorsElem;
		Item.iImage = 0;
		Item.state = 0;
		Item.lParam = 0;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}
	else
		{
		Item.pszText = szTriggersObjectsText;
		Item.iImage = 17;
		Item.state = 0;
		Item.lParam = 0x000000F9;
		if ((!Type)&&(ID == 0xF9))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0; Doors[i].Id != 0; i++, Item.iItem++)
		{
		Item.pszText = Doors[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(Doors[i].Variable,TRIGGERIMAGE_DOOR);
		Item.state = 0;
		Item.lParam = Doors[i].Variable|(3<<16);
		if ((Type == 3)&&(Doors[i].Variable == ID))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		}

	if (Click == -1)
		{
		Item.pszText = szTriggersTrapsElem;
		Item.iImage = 0;
		Item.state = 0;
		Item.lParam = 0;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0; Traps[i].Id != 0; i++, Item.iItem++)
		{
		Item.pszText = Traps[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(Traps[i].Variable,TRIGGERIMAGE_TRAP);
		Item.state = 0;
		Item.lParam = Traps[i].Variable|(4<<16);
		if ((Type == 4)&&(Traps[i].Variable == ID))
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des sorts ««««««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateSpellsList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD SpellID, BOOL HasAny)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if (HasAny)
		{
		Item.pszText = szTriggersAnySpells;
		Item.iImage = 5;
		Item.state = 0;
		if (!SpellID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0, Item.iItem++; Spells[i].Name != 0; i++, Item.iItem++)
		{
		Item.iImage = Triggers_DisplayGetImageId(Spells[i].Variable,TRIGGERIMAGE_SPELL);
		Item.state = 0;
		Item.pszText = Spells[i].Name;
		Item.lParam = Spells[i].Variable;
		if (SpellID == Spells[i].Variable) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (SpellID == Spells[i].Variable) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des pièges «««««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateTrapsList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD TrapID, BOOL HasAny)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	if (HasAny != (BOOL)-1)
		{
		GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
		MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

		*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
		if (!*hWnd) return(0);
		Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
		Column.fmt = LVCFMT_LEFT;
		Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
		Column.iSubItem = 0;
		if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);
		}

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = (HasAny == (BOOL)-1)?(SendMessage(*hWnd,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0)+1):0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if ((HasAny)&&(HasAny != (BOOL)-1))
		{
		Item.pszText = szTriggersAnyTraps;
		Item.iImage = 5;
		Item.state = 0;
		if (!TrapID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0, Item.iItem++; Traps[i].Name != 0; i++, Item.iItem++)
		{
		Item.iImage = Triggers_DisplayGetImageId(Traps[i].Variable,TRIGGERIMAGE_TRAP);
		Item.state = 0;
		Item.pszText = Traps[i].Name;
		Item.lParam = Traps[i].Variable;
		if (TrapID == Traps[i].Variable) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (TrapID == Traps[i].Variable) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des portes «««««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateDoorsList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD DoorID, BOOL HasAny)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if (HasAny)
		{
		Item.pszText = (HasAny == (BOOL)-1)?szTriggersAny:szTriggersAnyDoors;
		Item.iImage = 5;
		Item.state = 0;
		if (!DoorID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0, Item.iItem++; Doors[i].Name != 0; i++, Item.iItem++)
		{
		Item.iImage = Triggers_DisplayGetImageId(Doors[i].Variable,TRIGGERIMAGE_DOOR);
		Item.state = 0;
		Item.pszText = Doors[i].Name;
		Item.lParam = Doors[i].Variable;
		if (DoorID == Doors[i].Variable) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (DoorID == Doors[i].Variable) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des salles «««««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateRoomsList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD RoomID, BOOL HasAny, BOOL SpecialRooms)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if (HasAny == -1)
		{
		Item.pszText = szTriggersNoRoom;
		Item.iImage = 18;
		Item.state = 0;
		Item.lParam = -1;
		if (!RoomID)
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}
	else if (HasAny)
		{
		Item.pszText = szTriggersAnyRooms;
		Item.iImage = 5;
		Item.state = 0;
		if (!RoomID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	if (SpecialRooms)
		{
		Item.pszText = szMapWorldHeart;
		Item.iImage = Triggers_DisplayGetImageId(5,TRIGGERIMAGE_ROOM);
		Item.state = 0;
		if (RoomID == 5)
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		Item.lParam = 5;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;

		Item.pszText = szMapWorldPortal;
		Item.iImage = Triggers_DisplayGetImageId(3,TRIGGERIMAGE_ROOM);
		Item.state = 0;
		if (RoomID == 3)
			{
			Item.state = LVIS_SELECTED;
			Selected = Item.iItem;
			}
		Item.lParam = 3;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0, Item.iItem++; VarRooms[i].Name != 0; i++, Item.iItem++)
		{
		Item.iImage = Triggers_DisplayGetImageId(VarRooms[i].Variable,TRIGGERIMAGE_ROOM);
		Item.state = 0;
		Item.pszText = VarRooms[i].Name;
		Item.lParam = VarRooms[i].Variable;
		if (RoomID == VarRooms[i].Variable) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (RoomID == VarRooms[i].Variable) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des joueurs ««««««««««««««««««««««««««««««««»

int Triggers_DisplayCreatePlayersList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD PlayerID, BOOL HasAny)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if (HasAny)
		{
		Item.pszText = szTriggersAnyPlayers;
		Item.iImage = 5;
		Item.state = 0;
		if (!PlayerID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	for (i = 0, Item.iItem++; Players[i].Name != 0; i++, Item.iItem++)
		{
		Item.state = 0;
		Item.pszText = Players[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(i+1,TRIGGERIMAGE_PLAYER);
		Item.lParam = i+1;
		if (PlayerID == i+1) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (PlayerID == i+1) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Création de la liste des créatures ««««««««««««««««««««««««««««««»

int Triggers_DisplayCreateCreaturesList(HWND hDlg, UINT CtlID, HWND *hWnd, DWORD CreatureID, BOOL HasAny, BOOL Elites)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	int		Selected;
	int		i;

	GetWindowRect(GetDlgItem(hDlg,CtlID),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	*hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!*hWnd) return(0);
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(*hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(0);

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.stateMask = LVIS_SELECTED;
	Selected = 0;

	if (HasAny)
		{
		Item.pszText = szTriggersAnyCreatures;
		Item.iImage = 5;
		Item.state = 0;
		if (!CreatureID) Item.state = LVIS_SELECTED;
		Item.lParam = -1;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		Item.iItem++;
		}

	Item.pszText = szBadCreatures;
	Item.iImage = 0;
	Item.state = 0;
	Item.lParam = 0;
	if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);

	for (i = 0, Item.iItem++; BadCreatures[i].Name != 0; i++, Item.iItem++)
		{
		Item.state = 0;
		Item.pszText = BadCreatures[i].Name;
		Item.iImage = Triggers_DisplayGetImageId(BadCreatures[i].Id,TRIGGERIMAGE_BAD);
		Item.lParam = BadCreatures[i].Id;
		if (CreatureID == BadCreatures[i].Id) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (CreatureID == BadCreatures[i].Id) Selected = Item.iItem;
		}

	if (Elites)
		{
		Item.pszText = szBadCreaturesElite;
		Item.iImage = 0;
		Item.state = 0;
		Item.lParam = 0;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);

		for (i = 0, Item.iItem++; BadElites[i].Name != 0; i++, Item.iItem++)
			{
			Item.state = 0;
			Item.pszText = BadElites[i].Name;
			Item.iImage = Triggers_DisplayGetImageId(BadElites[i].Id,TRIGGERIMAGE_ELITE);
			Item.lParam = BadElites[i].Id;
			if (CreatureID == BadElites[i].Id) Item.state = LVIS_SELECTED;
			if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
			if (CreatureID == BadElites[i].Id) Selected = Item.iItem;
			}
		}

	Item.pszText = szGoodCreatures;
	Item.iImage = 0;
	Item.state = 0;
	Item.lParam = 0;
	if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);

	for (i = 0, Item.iItem++; Heros[i].Name != 0; i++, Item.iItem++)
		{
		Item.state = 0;
		Item.iImage = Triggers_DisplayGetImageId(Heros[i].Id,TRIGGERIMAGE_HERO);
		Item.pszText = Heros[i].Name;
		Item.lParam = Heros[i].Id;
		if (CreatureID == Heros[i].Id) Item.state = LVIS_SELECTED;
		if (SendMessage(*hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) return(0);
		if (CreatureID == Heros[i].Id) Selected = Item.iItem;
		}

	SendMessage(*hWnd,LVM_ENSUREVISIBLE,(WPARAM)Selected,(LPARAM)0);
	return(1);
}


// «»»» Affichage d'une liste de propositions «««««««««««««««««««««««««««»

void Triggers_DisplayDrawList(DRAWITEMSTRUCT *ItemStruct, TRIGGERSCTX *Context)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		WorkRect;
	SIZE		TextSize;
	UINT		TextFormat;
	LONG		ImagesSize;
	HFONT		OldFont;
	COLORREF	OldColor;
	char		ItemText[128];

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	Item.mask = LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = ItemStruct->itemID;
	Item.iSubItem = 0;
	if (!SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;

	//--- Affichage des données graphiques ---

	ImagesSize = 32;
	TextSize.cx = SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)0,(LPARAM)0);
	TextSize.cx -= 2;
	if (TextSize.cx > 2)
		{
		if (TextSize.cx > ImagesSize) TextSize.cx = ImagesSize;
		if ((Context->Images)&&(Item.iImage != 0))
			{
			if (!Item.lParam)
				ImageList_DrawEx(Context->Images,Item.iImage,ItemStruct->hDC,ItemStruct->rcItem.left+2,ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top-ImagesSize)/2,TextSize.cx,ImagesSize,CLR_DEFAULT,0x00000000,ILD_BLEND50);
			else
				ImageList_DrawEx(Context->Images,Item.iImage,ItemStruct->hDC,ItemStruct->rcItem.left+2,ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top-ImagesSize)/2,TextSize.cx,ImagesSize,CLR_DEFAULT,CLR_DEFAULT,ILD_TRANSPARENT);
			}
		ImagesSize = TextSize.cx;
		}

	WorkRect.left += 2+ImagesSize;
	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)((ItemStruct->itemState&ODS_SELECTED)?(COLOR_HIGHLIGHT+1):(COLOR_WINDOW+1)));

	//--- Affichage du texte ---

	Column.mask = LVCF_WIDTH;
	Column.iSubItem = 0;
	Item.iSubItem = 0;
	Item.pszText = ItemText;
	Item.cchTextMax = 128;
	SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&Item);
	SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMN,(WPARAM)Column.iSubItem,(LPARAM)&Column);
	if (*ItemText == '?')
		{
		if (Item.lParam) OldColor = SetTextColor(ItemStruct->hDC,0x000000FF);
		else OldColor = SetTextColor(ItemStruct->hDC,0x00000055);
		Item.pszText++;
		}
	else
		{
		if (Item.lParam) OldColor = SetTextColor(ItemStruct->hDC,(ItemStruct->itemState&ODS_SELECTED)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
		else OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
		}
	OldFont = SelectObject(ItemStruct->hDC,Font.hFont);
	WorkRect.left += 2;
	GetTextExtentPoint32(ItemStruct->hDC,Item.pszText,strlen(Item.pszText),&TextSize);
	TextFormat = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_END_ELLIPSIS|DT_MODIFYSTRING|DT_LEFT;
	WorkRect.right = WorkRect.left+Column.cx-4-ImagesSize;
	DrawText(ItemStruct->hDC,Item.pszText,-1,&WorkRect,TextFormat);
	SelectObject(ItemStruct->hDC,OldFont);
	SetTextColor(ItemStruct->hDC,OldColor);

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
