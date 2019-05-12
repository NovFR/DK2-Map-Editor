
//<<>-<>>---------------------------------------------------------------------()
/*
	Statistiques
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
extern DWORD		TickCount;
extern MAP		Map;
extern MAPEDITSTATS	MapStats;
extern NODE		MapCreatures;
extern NODE		MapTraps;
extern NODE		MapDoors;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapActions;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Statistiques de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Réinitialise les statistiques «««««««««««««««««««««««««««««««««««»

void Map_ResetStats(MAPEDITSTATS *Stats)
{
	ZeroMemory(Stats,sizeof(MAPEDITSTATS));
	GetLocalTime(&Stats->StartTime);
	return;
}


// «»»» Création des statistiques «««««««««««««««««««««««««««««««««««««««»

void Map_CreateStats(MAPEDITSTATSCTX *Context)
{
	MAPTHING	*Thing;
	MTHBAND		*Band;
	ACTION		*Action;
	int		 i,j;

	if (Context->AlreadySet) return;
	Context->AlreadySet = TRUE;

	//--- Enumerate things in map ---

	for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Context->CreaturesCount[Thing->id]++;
		Context->CreaturesLevels[Thing->id] += Thing->data.level;
		Context->TotalCreatures++;
		Context->Players[Thing->owner-1].CreaturesLevels[Thing->id] += Thing->data.level;
		Context->Players[Thing->owner-1].CreaturesCount[Thing->id]++;
		}

	for (Thing = (MAPTHING *)MapTraps.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Context->TrapsCount[Thing->id]++;
		Context->TotalTraps++;
		Context->Players[Thing->owner-1].Traps++;
		Context->Players[Thing->owner-1].TrapsCount[Thing->id]++;
		}

	for (Thing = (MAPTHING *)MapDoors.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Context->DoorsCount[Thing->id]++;
		Context->TotalDoors++;
		Context->Players[Thing->owner-1].Doors++;
		Context->Players[Thing->owner-1].DoorsCount[Thing->id]++;
		}

	for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Context->ObjectsCount[Thing->id]++;
		Context->TotalObjects++;
		Context->Players[Thing->owner-1].Objects++;
		Context->Players[Thing->owner-1].ObjectsCount[Thing->id]++;
		}

	for (Thing = (MAPTHING *)MapMagicalObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Context->MagicalObjectsCount[Thing->id]++;
		Context->TotalMagicalObjects++;
		Context->Players[Thing->owner-1].MagicalObjects++;
		Context->Players[Thing->owner-1].MagicalObjectsCount[Thing->id]++;
		}

	Context->TotalSquares	= Map.Properties.Width*Map.Properties.Height;
	Context->GoldInLevel	= Compiler_InfoGoldAmountInRect(COMPILER_QUERYMAPGOLD|COMPILER_QUERYMAPGEMS,&Context->IsThereUnlimitedGold,NULL);
	Context->GoldInObjects	= Compiler_InfoGoldAmountInRect(COMPILER_QUERYOBJGOLD,NULL,NULL);
	Context->GoldInMagics	= Compiler_InfoGoldAmountInRect(COMPILER_QUERYMGCGOLD,NULL,NULL);
	Context->TotalWater	= Compiler_InfoEnumTerrainInRect(0x04020101,0xFFFFFFFF,COMPILER_QUERYWORLD,NULL);
	Context->TotalLava	= Compiler_InfoEnumTerrainInRect(0x05020101,0xFFFFFFFF,COMPILER_QUERYWORLD,NULL);
	Context->TotalSolid	= Compiler_InfoEnumTerrainInRect(0x01020101,0xFFFFFFFF,COMPILER_QUERYWORLD,NULL);
	Context->TotalRock	= Compiler_InfoEnumTerrainInRect(0x02020101,0xFFFFFFFF,COMPILER_QUERYWORLD,NULL);
	Context->TotalUnclaimed	= Compiler_InfoEnumTerrainInRect(0x03020101,0xFFFFFFFF,COMPILER_QUERYWORLD,NULL);
	Context->RelWater	= (double)Context->TotalWater*100.0f/(double)Context->TotalSquares;
	Context->RelLava	= (double)Context->TotalLava*100.0f/(double)Context->TotalSquares;
	Context->RelSolid	= (double)Context->TotalSolid*100.0f/(double)Context->TotalSquares;
	Context->RelRock	= (double)Context->TotalRock*100.0f/(double)Context->TotalSquares;
	Context->RelUnclaimed	= (double)Context->TotalUnclaimed*100.0f/(double)Context->TotalSquares;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		{
		Context->Players[i].TreasuryCapacity = Compiler_InfoTreasuryCapacityInRect(i+1,NULL);
		Context->Players[i].RoomsCapacity = Compiler_InfoRoomInRect(i+1,NULL);
		Context->Players[i].ManaIncrease = Compiler_InfoManaInRect(i+1,NULL);
		Context->Players[i].Imps = Compiler_InfoCreaturesCountInRect(i+1,COMPILER_QUERYIMPS,NULL);
		Context->Players[i].Creatures = Compiler_InfoCreaturesCountInRect(i+1,COMPILER_QUERYALL,NULL);
		Context->Players[i].Rooms = Compiler_InfoEnumTerrainInRect((i+1)<<16,0x00FF0000,COMPILER_QUERYPLAYERS,NULL);
		Context->Players[i].Walls = Compiler_InfoEnumTerrainInRect(((i+1)<<16)|0x09000000,0xFFFF0000,COMPILER_QUERYPLAYERS,NULL);
		Context->Players[i].Claimed = Compiler_InfoEnumTerrainInRect(((i+1)<<16)|0x08000000,0xFFFF0000,COMPILER_QUERYPLAYERS,NULL);
		if (!Compiler_InfoHeartPosition(i+1,&Context->Players[i].HeartX,&Context->Players[i].HeartY)) Context->Players[i].HeartX = -1;
		for (j = 0; j != 255; j++)
			{
			if (j == 1) continue;
			Context->Players[i].CreaturesAvgLevel += Context->Players[i].CreaturesLevels[j];
			}
		if (Context->Players[i].Creatures) Context->Players[i].CreaturesAvgLevel /= Context->Players[i].Creatures;
		for (Context->Players[i].PreferredCreature = 0, j = 0; j != 255; j++)
			{
			if (j == 1) continue;
			if (Context->Players[i].CreaturesCount[j] > Context->Players[i].CreaturesCount[Context->Players[i].PreferredCreature]) Context->Players[i].PreferredCreature = j;
			}
		for (Context->Players[i].PreferredDoor = 0, j = 0; j != 255; j++)
			if (Context->Players[i].DoorsCount[j] > Context->Players[i].DoorsCount[Context->Players[i].PreferredDoor]) Context->Players[i].PreferredDoor = j;
		for (Context->Players[i].PreferredTrap = 0, j = 0; j != 255; j++)
			if (Context->Players[i].TrapsCount[j] > Context->Players[i].TrapsCount[Context->Players[i].PreferredTrap]) Context->Players[i].PreferredTrap = j;
		for (Context->Players[i].PreferredObject = 0, j = 0; j != 255; j++)
			if (Context->Players[i].ObjectsCount[j] > Context->Players[i].ObjectsCount[Context->Players[i].PreferredObject]) Context->Players[i].PreferredObject = j;
		for (Context->Players[i].PreferredMagicalObject = 0, j = 0; j != 255; j++)
			if (Context->Players[i].MagicalObjectsCount[j] > Context->Players[i].MagicalObjectsCount[Context->Players[i].PreferredMagicalObject]) Context->Players[i].PreferredMagicalObject = j;
		}

	//--- Set relative stats ---

	for (Context->PreferredCreature = 0, i = 0; i != 255; i++)
		if (Context->CreaturesCount[i] > Context->CreaturesCount[Context->PreferredCreature]) Context->PreferredCreature = i;

	for (Context->PreferredDoor = 0, i = 0; i != 255; i++)
		if (Context->DoorsCount[i] > Context->DoorsCount[Context->PreferredDoor]) Context->PreferredDoor = i;

	for (Context->PreferredTrap = 0, i = 0; i != 255; i++)
		if (Context->TrapsCount[i] > Context->TrapsCount[Context->PreferredTrap]) Context->PreferredTrap = i;

	for (Context->PreferredObject = 0, i = 0; i != 255; i++)
		if (Context->ObjectsCount[i] > Context->ObjectsCount[Context->PreferredObject]) Context->PreferredObject = i;

	for (Context->PreferredMagicalObject = 0, i = 0; i != 255; i++)
		if (Context->MagicalObjectsCount[i] > Context->MagicalObjectsCount[Context->PreferredMagicalObject]) Context->PreferredMagicalObject = i;

	for (i = 0; i != 255; i++) Context->CreaturesAvgLevel += Context->CreaturesLevels[i];
	if (Context->TotalCreatures) Context->CreaturesAvgLevel /= Context->TotalCreatures;

	//--- Set heroes bands stats ---

	for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
		{
		for (i = 0; i != THING_MAXHEROSINBAND; i++)
			{
			if (!Band->heros[i].isvalid) break;
			Context->CreaturesCountInBands[Band->heros[i].id]++;
			Context->CreaturesLevelsInBands[Band->heros[i].id] += Band->heros[i].level;
			Context->Bands[Band->id].CreaturesCount[Band->heros[i].id]++;
			Context->Bands[Band->id].CreaturesLevels[Band->heros[i].id] += Band->heros[i].level;
			switch(Band->heros[i].id)
				{
				case 0x01:
					Context->TotalImpsInBands++;
					Context->Bands[Band->id].Imps++;
					break;
				case 0x0E:
					Context->TotalDwarfsInBands++;
					Context->Bands[Band->id].Dwarfs++;
					break;
				default:Context->TotalCreaturesInBands++;
					Context->Bands[Band->id].Creatures++;
					break;
				}
			}
		for (Context->Bands[Band->id].PreferredCreature = 0, i = 0; i != 255; i++)
			{
			if (i == 0x01) continue;
			if (i == 0x0E) continue;
			if (Context->Bands[Band->id].CreaturesCount[i] > Context->Bands[Band->id].CreaturesCount[Context->Bands[Band->id].PreferredCreature]) Context->Bands[Band->id].PreferredCreature = i;
			}
		for (i = 0; i != 255; i++)
			Context->Bands[Band->id].CreaturesAvgLevel += Context->Bands[Band->id].CreaturesLevels[i];
		if (Context->Bands[Band->id].Creatures+Context->Bands[Band->id].Imps+Context->Bands[Band->id].Dwarfs)
			Context->Bands[Band->id].CreaturesAvgLevel /= Context->Bands[Band->id].Creatures+Context->Bands[Band->id].Imps+Context->Bands[Band->id].Dwarfs;
		for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
			if ((Action->ctrl == ACTION_RELEASEBAND)&&(Action->param1 == Band->id)) Context->Bands[Band->id].UseCount++;
		}

	for (Context->PreferredCreatureInBands = 0, i = 0; i != 255; i++)
		{
		if (i == 0x01) continue;
		if (i == 0x0E) continue;
		if (Context->CreaturesCountInBands[i] > Context->CreaturesCountInBands[Context->PreferredCreatureInBands]) Context->PreferredCreatureInBands = i;
		}
	for (i = 0; i != 255; i++)
		Context->CreaturesAvgLevelInBands += Context->CreaturesLevelsInBands[i];
	if (Context->TotalImpsInBands+Context->TotalDwarfsInBands+Context->TotalCreaturesInBands)
		Context->CreaturesAvgLevelInBands /= Context->TotalImpsInBands+Context->TotalDwarfsInBands+Context->TotalCreaturesInBands;

	//--- Set heroes total stats (in heroes bands page) ---

	Context->TotalHeroesCreatures = Context->TotalCreaturesInBands+Context->Players[0].Imps+Context->Players[0].Creatures;
	for (i = 0; i != 255; i++)
		Context->HeroesCreaturesAvgLevel += Context->CreaturesLevelsInBands[i]+Context->Players[0].CreaturesLevels[i];
	if (Context->Players[0].Imps+Context->Players[0].Creatures+Context->TotalImpsInBands+Context->TotalDwarfsInBands+Context->TotalCreaturesInBands)
		Context->HeroesCreaturesAvgLevel /= Context->Players[0].Imps+Context->Players[0].Creatures+Context->TotalImpsInBands+Context->TotalDwarfsInBands+Context->TotalCreaturesInBands;

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage des statistiques					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Map_DisplayStats(HWND hWnd)
{
	MAPEDITSTATSCTX	*Context;
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	int		 Result;

	Context = GlobalAlloc(GPTR,sizeof(MAPEDITSTATSCTX));
	if (!Context) return;
	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) {GlobalFree(Context); return;}
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*4);
	if (!psp) {GlobalFree(psh); GlobalFree(Context); return;}

	for (Result = 0; Result != 4; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(6901+Result);
		psp[Result].pfnDlgProc = Map_StatsProc;
		psp[Result].lParam = (LPARAM)Context;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID;
	psh->hwndParent = hWnd;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = szMapStatsTitle;
	psh->nPages = 4;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	Result = PropertySheet(psh);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	GlobalFree(psh);
	GlobalFree(psp);
	GlobalFree(Context);
	return;
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
BOOL CALLBACK Map_StatsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPEDITSTATSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Map_StatsInit(hDlg,(PROPSHEETPAGE *)lParam);
		return(TRUE);
		}

	Context = (MAPEDITSTATSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_MeasurePlayerComboBoxList(199,(MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_DisplayPlayerComboBoxList(199,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
                                case PSN_SETACTIVE:
					PostMessage(GetParent(hDlg),PSM_CANCELTOCLOSE,(WPARAM)0,(LPARAM)0);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 199:
							Map_StatsDisplayPlayer(hDlg,Context);
							break;
						case 299:
							Map_StatsDisplayBand(hDlg,Context);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void Map_StatsInit(HWND hDlg, PROPSHEETPAGE *Page)
{
	MAPEDITSTATSCTX	*Context;
	MTHBAND		*Band;
	LONG		 X,Y;
	char		*Name;
	char		 Temp[16];

	Context = (MAPEDITSTATSCTX *)(Page->lParam);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	Map_CreateStats(Context);

	switch((LONG)Page->DUMMYUNIONNAME.pszTemplate)
		{
		case 6901:
			X = GetTickCount()-TickCount;
			SetDlgItemInt(hDlg,299,(MapStats.TotalTicks+X)/1000/60/60/24,FALSE);
			SetDlgItemInt(hDlg,201,(MapStats.TotalTicks+X)/1000/60/60%24,FALSE);
			SetDlgItemInt(hDlg,202,(MapStats.TotalTicks+X)/1000/60%60,FALSE);
			SetDlgItemInt(hDlg,203,(MapStats.TotalTicks+X)/1000%60,FALSE);
			SetDlgItemInt(hDlg,205,MapStats.LMBClicks,FALSE);
			SetDlgItemInt(hDlg,207,MapStats.RMBClicks,FALSE);
			SetDlgItemInt(hDlg,225,MapStats.UsageOfUndo,FALSE);
			SetDlgItemInt(hDlg,209,MapStats.UsageOfPaintThings,FALSE);
			SetDlgItemInt(hDlg,211,MapStats.UsageOfPaintFill,FALSE);
			SetDlgItemInt(hDlg,213,MapStats.UsageOfPaint,FALSE);
			SetDlgItemInt(hDlg,215,MapStats.UsageOfPaintRnd,FALSE);
			SetDlgItemInt(hDlg,217,MapStats.SavedTimes,FALSE);
			Misc_FmtDate(Context->Date,&MapStats.LastSaved);
			SetDlgItemText(hDlg,219,Context->Date);
			Misc_FmtDate(Context->Date,&MapStats.CreatedTime);
			SetDlgItemText(hDlg,221,Context->Date);
			Misc_FmtDate(Context->Date,&MapStats.CompileTime);
			SetDlgItemText(hDlg,223,Context->Date);
			break;
		case 6902:
			SetDlgItemInt(hDlg,201,Context->TotalCreatures,FALSE);
			SetDlgItemInt(hDlg,204,Context->CreaturesAvgLevel,FALSE);
			SetDlgItemInt(hDlg,206,Context->TotalTraps,FALSE);
			SetDlgItemInt(hDlg,209,Context->TotalDoors,FALSE);
			SetDlgItemInt(hDlg,212,Context->TotalObjects,FALSE);
			SetDlgItemInt(hDlg,215,Context->TotalMagicalObjects,FALSE);
			Name = Things_GetCreatureNameFromID(Context->PreferredCreature);
			if (Name) SetDlgItemText(hDlg,202,Name);
			else SetDlgItemText(hDlg,202,szNone);
			Name = Things_GetThingNameFromID(Context->PreferredTrap,Traps);
			if (Name) SetDlgItemText(hDlg,207,Name);
			else SetDlgItemText(hDlg,207,szNone);
			Name = Things_GetThingNameFromID(Context->PreferredDoor,Doors);
			if (Name) SetDlgItemText(hDlg,210,Name);
			else SetDlgItemText(hDlg,210,szNone);
			Name = Things_GetThingNameFromID(Context->PreferredObject,Objects);
			if (Name) SetDlgItemText(hDlg,213,Name);
			else SetDlgItemText(hDlg,213,szNone);
			Name = Things_GetThingNameFromID(Context->PreferredMagicalObject,Magics);
			if (Name) SetDlgItemText(hDlg,216,Name);
			else SetDlgItemText(hDlg,216,szNone);
			SetDlgItemInt(hDlg,238,Context->TotalSquares,FALSE);
			SetDlgItemInt(hDlg,218,Context->TotalWater,FALSE);
			SetDlgItemInt(hDlg,220,Context->TotalLava,FALSE);
			SetDlgItemInt(hDlg,222,Context->TotalSolid,FALSE);
			SetDlgItemInt(hDlg,224,Context->TotalRock,FALSE);
			SetDlgItemInt(hDlg,226,Context->TotalUnclaimed,FALSE);
			SetDlgItemText(hDlg,227,Misc_DoubleToText(Context->RelWater,3,Temp));
			SetDlgItemText(hDlg,229,Misc_DoubleToText(Context->RelLava,3,Temp));
			SetDlgItemText(hDlg,231,Misc_DoubleToText(Context->RelSolid,3,Temp));
			SetDlgItemText(hDlg,233,Misc_DoubleToText(Context->RelRock,3,Temp));
			SetDlgItemText(hDlg,235,Misc_DoubleToText(Context->RelUnclaimed,3,Temp));
			SetDlgItemInt(hDlg,240,Context->GoldInLevel,FALSE);
			SetDlgItemInt(hDlg,242,Context->GoldInObjects,FALSE);
			SetDlgItemInt(hDlg,244,Context->GoldInMagics,FALSE);
			CheckDlgButton(hDlg,245,(Context->IsThereUnlimitedGold)?BST_CHECKED:BST_UNCHECKED);
			break;
		case 6903:
			Misc_CreatePlayerComboBoxList(hDlg,199,1,FALSE);
			SendDlgItemMessage(hDlg,199,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			Map_StatsDisplayPlayer(hDlg,Context);
			break;
		case 6904:
			SetDlgItemInt(hDlg,201,Context->TotalHeroesCreatures,FALSE);
			SetDlgItemInt(hDlg,204,Context->HeroesCreaturesAvgLevel,FALSE);
			SetDlgItemInt(hDlg,211,Context->TotalDwarfsInBands,FALSE);
			SetDlgItemInt(hDlg,213,Context->TotalImpsInBands,FALSE);
			SetDlgItemInt(hDlg,215,Context->TotalCreaturesInBands,FALSE);
			Name = Things_GetCreatureNameFromID(Context->PreferredCreatureInBands);
			if (Name) SetDlgItemText(hDlg,216,Name);
			else SetDlgItemText(hDlg,216,szNone);
			SetDlgItemInt(hDlg,218,Context->CreaturesAvgLevelInBands,FALSE);
			if (!MapHerosBands.next)
				SendDlgItemMessage(hDlg,299,CB_ADDSTRING,(WPARAM)0,(LPARAM)szMapStatsNoBand);
			else for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
				{
				wsprintf(Context->BandName,szMapStatsBand,Band->id,Band->name);
				SendDlgItemMessage(hDlg,299,CB_ADDSTRING,(WPARAM)0,(LPARAM)Context->BandName);
				}
			SendDlgItemMessage(hDlg,299,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
			Map_StatsDisplayBand(hDlg,Context);
			break;
		}

	return;
}


// «»»» Affichage des informations des bandes de héros ««««««««««««««««««»

void Map_StatsDisplayBand(HWND hDlg, MAPEDITSTATSCTX *Context)
{
	MTHBAND	*Band;
	char	*Name;
	int	 i;

	i = SendDlgItemMessage(hDlg,299,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i < 0) return;

	Band = (MTHBAND *)MapHerosBands.next;
	while(i--)
		{
		if (!Band) break;
		Band = (MTHBAND *)Band->node.next;
		}

	if (!Band)
		{
		SetDlgItemText(hDlg,221,szNone);
		SetDlgItemText(hDlg,223,szNone);
		SetDlgItemText(hDlg,225,szNone);
		SetDlgItemText(hDlg,226,szNone);
		SetDlgItemText(hDlg,228,szNone);
		SetDlgItemText(hDlg,231,szNone);
		return;
		}

	SetDlgItemInt(hDlg,221,Context->Bands[Band->id].Dwarfs,FALSE);
	SetDlgItemInt(hDlg,223,Context->Bands[Band->id].Imps,FALSE);
	SetDlgItemInt(hDlg,225,Context->Bands[Band->id].Creatures,FALSE);
	Name = Things_GetCreatureNameFromID(Context->Bands[Band->id].PreferredCreature);
	if (Name) SetDlgItemText(hDlg,226,Name);
	else SetDlgItemText(hDlg,226,szNone);
	SetDlgItemInt(hDlg,228,Context->Bands[Band->id].CreaturesAvgLevel,FALSE);
	SetDlgItemInt(hDlg,231,Context->Bands[Band->id].UseCount,FALSE);
	return;
}


// «»»» Affichage des informations des joueurs ««««««««««««««««««««««««««»

void Map_StatsDisplayPlayer(HWND hDlg, MAPEDITSTATSCTX *Context)
{
	char	*Name;
	int	 i;

	i = SendDlgItemMessage(hDlg,199,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (i < 0) return;

	SetDlgItemInt(hDlg,201,Context->Players[i].Imps,FALSE);
	SetDlgItemInt(hDlg,203,Context->Players[i].Creatures,FALSE);
	SetDlgItemInt(hDlg,206,Context->Players[i].CreaturesAvgLevel,FALSE);
	SetDlgItemInt(hDlg,209,Context->Players[i].Traps,FALSE);
	SetDlgItemInt(hDlg,212,Context->Players[i].Doors,FALSE);
	SetDlgItemInt(hDlg,215,Context->Players[i].Objects,FALSE);
	SetDlgItemInt(hDlg,218,Context->Players[i].MagicalObjects,FALSE);
	Name = Things_GetCreatureNameFromID(Context->Players[i].PreferredCreature);
	if (Name) SetDlgItemText(hDlg,204,Name);
	else SetDlgItemText(hDlg,204,szNone);
	Name = Things_GetThingNameFromID(Context->Players[i].PreferredTrap,Traps);
	if (Name) SetDlgItemText(hDlg,210,Name);
	else SetDlgItemText(hDlg,210,szNone);
	Name = Things_GetThingNameFromID(Context->Players[i].PreferredDoor,Doors);
	if (Name) SetDlgItemText(hDlg,213,Name);
	else SetDlgItemText(hDlg,213,szNone);
	Name = Things_GetThingNameFromID(Context->Players[i].PreferredObject,Objects);
	if (Name) SetDlgItemText(hDlg,216,Name);
	else SetDlgItemText(hDlg,216,szNone);
	Name = Things_GetThingNameFromID(Context->Players[i].PreferredMagicalObject,Magics);
	if (Name) SetDlgItemText(hDlg,219,Name);
	else SetDlgItemText(hDlg,219,szNone);
	if (Context->Players[i].HeartX != -1)
		{
		SetDlgItemInt(hDlg,221,Context->Players[i].HeartX,FALSE);
		SetDlgItemInt(hDlg,222,Context->Players[i].HeartY,FALSE);
		}
	else
		{
		SetDlgItemText(hDlg,221,szNone);
		SetDlgItemText(hDlg,222,szNone);
		}
	SetDlgItemInt(hDlg,224,Context->Players[i].TreasuryCapacity,FALSE);
	SetDlgItemInt(hDlg,226,Context->Players[i].RoomsCapacity,FALSE);
	SetDlgItemInt(hDlg,228,Context->Players[i].ManaIncrease,FALSE);
	SetDlgItemInt(hDlg,230,Context->Players[i].Walls,FALSE);
	SetDlgItemInt(hDlg,232,Context->Players[i].Claimed,FALSE);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
