
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres de la carte
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
extern LONG		MapChanges;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapActions;
extern MAP		Map;
extern MAPOPTIONS	MapOptions[MAP_MAXPLAYERS];
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Modifie les options de la carte «««««««««««««««««««««««««««««««««»

void Properties_MapOptions(HWND hWnd)
{
	MAPPROPSCTX	*MapProps;
	UNDO		*Undo;
	UNDO		*Child;
	BOOL		 Nasty;
	int		 Result;

	MapProps = GlobalAlloc(GPTR,sizeof(MAPPROPSCTX));
	if (!MapProps)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	CopyMemory(&MapProps->MapProperties,&Map.Properties,sizeof(MAPPROPERTIES));
	MapProps->MapNewWidth = Map.Properties.Width;
	MapProps->MapNewHeight = Map.Properties.Height;
	CopyMemory(&MapProps->Players[0],&MapOptions[0],sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6800),hWnd,Properties_MapProc,(LPARAM)MapProps);
	if ((Result == -1)||(!Result))
		{
		GlobalFree(MapProps);
		if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	//--- Valide les changements dans les options ---

	Undo = Undo_CreateNewEntry(UNDO_MAPPROPERTIES,szMapChangedProperties);
	Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPPARAMS,szMapChangedParams);
	if (!Undo_StoreMapParams(Child,&Map,&MapOptions[0]))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		Undo = NULL;
		}

	CopyMemory(&Map.Properties,&MapProps->MapProperties,sizeof(MAPPROPERTIES));
	CopyMemory(&MapOptions[0],&MapProps->Players[0],sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);
	MapChanges++;

	//--- Valide les changements dans les dimensions ---

	if ((MapProps->MapNewWidth != Map.Properties.Width)||(MapProps->MapNewHeight != Map.Properties.Height))
		{
		MAP	*ResizedMap;

		ResizedMap = Map_PrepareResize(MapProps->MapNewWidth,MapProps->MapNewHeight,&Map);
		if (ResizedMap)
			{
			BYTE	*SrcPtr,*DstPtr;
			BYTE	*SrcLinePtr,*DstLinePtr;
			LONG	 InMapX,InMapY;
			LONG	 OutMapX,OutMapY;
			LONG	 X,Y;
			LONG	 Width,Height;

			Map_Lock(TRUE,TRUE);

			// Pass 1... Change map sizes

			if (ResizedMap->Properties.Width > Map.Properties.Width)
				{
				Width = Map.Properties.Width;
				InMapX = 0;
				switch(MapProps->MapNewPos)
					{
					case 0:
					case 3:
					case 6:	OutMapX = 0;
						break;
					case 1:	
					case 4:	
					case 7:	OutMapX = (ResizedMap->Properties.Width-Map.Properties.Width)/2;
						break;
					case 2:	
					case 5:	
					case 8:	OutMapX = ResizedMap->Properties.Width-Map.Properties.Width;
						break;
					}
				}
			else
				{
				Width = ResizedMap->Properties.Width;
				OutMapX = 0;
				switch(MapProps->MapNewPos)
					{
					case 0:
					case 3:
					case 6:	InMapX = 0;
						break;
					case 1:
					case 4:
					case 7:	InMapX = (Map.Properties.Width-ResizedMap->Properties.Width)/2;
						break;
					case 2:
					case 5:
					case 8:	InMapX = Map.Properties.Width-ResizedMap->Properties.Width;
						break;
					}
				}

			if (ResizedMap->Properties.Height > Map.Properties.Height)
				{
				Height = Map.Properties.Height;
				InMapY = 0;
				switch(MapProps->MapNewPos)
					{
					case 0:
					case 1:
					case 2:	OutMapY = 0;
						break;
					case 3:
					case 4:
					case 5:	OutMapY = (ResizedMap->Properties.Height-Map.Properties.Height)/2;
						break;
					case 6:
					case 7:
					case 8:	OutMapY = ResizedMap->Properties.Height-Map.Properties.Height;
						break;
					}
				}
			else
				{
				Height = ResizedMap->Properties.Height;
				OutMapY = 0;
				switch(MapProps->MapNewPos)
					{
					case 0:
					case 1:
					case 2:	InMapY = 0;
						break;
					case 3:
					case 4:
					case 5:	InMapY = (Map.Properties.Height-ResizedMap->Properties.Height)/2;
						break;
					case 6:
					case 7:
					case 8:	InMapY = Map.Properties.Height-ResizedMap->Properties.Height;
						break;
					}
				}

			SrcPtr = Map.Map+(InMapX+1)*MAP_CONTROLBYTES+(InMapY+1)*Map.Properties.Width*MAP_CONTROLBYTES;
			DstPtr = ResizedMap->Map+(OutMapX+1)*MAP_CONTROLBYTES+(OutMapY+1)*ResizedMap->Properties.Width*MAP_CONTROLBYTES;

			for (Y = 0; Y != Height-2; Y++)
				{
				SrcLinePtr = SrcPtr;
				DstLinePtr = DstPtr;
				for (X = 0; X != (Width-2)*MAP_CONTROLBYTES; X++)
					*DstLinePtr++ = *SrcLinePtr++;
				SrcPtr += Map.Properties.Width*MAP_CONTROLBYTES;
				DstPtr += ResizedMap->Properties.Width*MAP_CONTROLBYTES;
				}

			Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPRESIZE,szMapChangedSize);
			if (!Undo_StoreFullMapChanges(Child,&Map))
				{
				Undo_PrintError(hWnd);
				Undo_DestroyEntry(Undo);
				Undo_Update();
				Undo = NULL;
				}
			MapChanges++;

			CopyMemory(&Map,ResizedMap,sizeof(MAP));
			GlobalFree(ResizedMap);

			// Pass 2... Update things positions

			Properties_UpdateThings(&MapCreatures,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateThings(&MapDoors,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateThings(&MapTraps,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateThings(&MapObjects,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateThings(&MapMagicalObjects,OutMapX-InMapX,OutMapY-InMapY,&Undo);

			// Pass 3... Remove things if necessary

			Properties_RemoveThings(&MapCreatures,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveThings(&MapDoors,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveThings(&MapTraps,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveThings(&MapObjects,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveThings(&MapMagicalObjects,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);

			// Pass 4... Update areas, gates & effects positions

			Properties_UpdateRectEx(&MapAreas,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateRectEx(&MapGates,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateRectEx(&MapEffects,OutMapX-InMapX,OutMapY-InMapY,&Undo);

			// Pass 5... Remove areas, gates & effects if necessary
			//
			//	     Note: Even partially displayed areas, gates & effects
			//		   are removed !

			Properties_RemoveRectEx(&MapAreas,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveRectEx(&MapGates,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveRectEx(&MapEffects,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);

			// Pass 6... Update triggers

			Properties_UpdateTriggers(OutMapX-InMapX,OutMapY-InMapY,&Undo);

			// Pass 7... Update keepers properties
			//
			//	     Note: There is no need to undo since the properties
			//		   are already stored previously :)

			Properties_UpdateProperties(OutMapX-InMapX,OutMapY-InMapY);

			// Pass 8... Update everything

			Map_CorrectAfterResize();
			Mouse_MovedUpdate(hWnd);
			Map_SetPropThumb(hWnd,PROP_VERTICAL);
			Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
			Map_CorrectPos(hWnd);
			Minimap_Build(hWnd);
			Map_Lock(FALSE,TRUE);
			Info_Update();
			Thumb_UpdateWindow();
			}
		}
	else
		{
		RECT	MapRect;

		Map_GetMapArea(hWnd,&MapRect);
		InvalidateRect(hWnd,&MapRect,NULL);
		UpdateWindow(hWnd);
		}

	Files_SaveUpdate();
	Map_NewTitle(hWnd);
	GlobalFree(MapProps);
	return;
}

//--- Supprime les éléments se trouvant hors de la carte ---

void Properties_RemoveThings(NODE *List, LONG Width, LONG Height, BOOL *Nasty, UNDO **Parent)
{
	MAPTHING	*Thing;

	for (Thing = (MAPTHING *)List->next; Thing != 0;)
		{
		if ((Thing->x <= 0)||(Thing->y <= 0)||(Thing->x >= Width-1)||(Thing->y >= Height-1))
			{
			if ((Thing->node.type == THING_CREATURE)&&(!Triggers_IsUsed(NULL,Thing->uniqueid,NULL,SCOPE_CREATURE))) *Nasty = TRUE;
			Things_RemoveAtMapPosEx(List,Thing->x,Thing->y,Parent);
			Thing = (MAPTHING *)List->next;
			continue;
			}
		Thing = (MAPTHING *)Thing->node.next;
		}
	return;
}

//--- Modifie les paramètres des gardiens ---

void Properties_UpdateProperties(LONG DX, LONG DY)
{
	UINT	Keeper;

	for (Keeper = 3-1; Keeper != MAP_MAXPLAYERS; Keeper++)
		{
		if (!MapOptions[Keeper].PlayerComputer) continue;
		if (MapOptions[Keeper].PlayerCPUAreaX1 != -1)
			{
			MapOptions[Keeper].PlayerCPUAreaX1 += DX;
			MapOptions[Keeper].PlayerCPUAreaY1 += DY;
			MapOptions[Keeper].PlayerCPUAreaX2 += DX;
			MapOptions[Keeper].PlayerCPUAreaY2 += DY;
			}
		if (MapOptions[Keeper].PlayerCameraX != -1)
			{
			MapOptions[Keeper].PlayerCameraX += DX;
			MapOptions[Keeper].PlayerCameraY += DY;
			}
		}

	return;
}

//--- Met à jour les positions des évènements ---

void Properties_UpdateTriggers(LONG DX, LONG DY, UNDO **Parent)
{
	ACTION		*Action;
	UNDO		*Child;

	for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
		{
		switch(Action->ctrl)
			{
			case ACTION_ADDCREATURE:
			case ACTION_CHANGESQUARE:
			case ACTION_CHANGEOWNER:
				Child = Undo_CreateNewChild(Parent,TRUE,UNDO_ACTIONRESIZED,szMapActionResized);
				if (Child)
					{
					Child->data.action = Action->id;
					Child->mod.dx = DX;
					Child->ext.dy = DY;
					}
				else
					{
					Undo_PrintError(hWnd);
					Undo_DestroyEntry(*Parent);
					Undo_Update();
					*Parent = NULL;
					}
				Action->param5 += DX;
				Action->param7 += DY;
				break;
			}
		}

	return;
}

//--- Met à jour les positions des éléments ---

void Properties_UpdateThings(NODE *List, LONG DX, LONG DY, UNDO **Parent)
{
	MAPTHING	*Thing;
	UNDO		*Child;
	UINT		 Type;
	char		*Text;

	if ((!DX)&&(!DY)) return;

	for (Thing = (MAPTHING *)List->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		switch(Thing->node.type)
			{
			case THING_CREATURE:
				Type = UNDO_MODCREATURE;
				Text = szMapModCreature;
				break;
			case THING_DOOR:
				Type = UNDO_MODDOOR;
				Text = szMapModDoor;
				break;
			case THING_TRAP:
				Type = UNDO_MODTRAP;
				Text = szMapModTrap;
				break;
			case THING_OBJECT:
				Type = UNDO_MODOBJECT;
				Text = szMapModObject;
				break;
			case THING_MAGICALOBJECT:
				Type = UNDO_MODMAGIC;
				Text = szMapModMagicalObject;
				break;
			}
		Child = Undo_CreateNewChild(Parent,TRUE,Type,Text);
		if (!Undo_StoreEntry(Child,(NODE *)Thing,Type))
			{
			Undo_PrintError(hWnd);
			Undo_DestroyEntry(*Parent);
			Undo_Update();
			*Parent = NULL;
			}
		Thing->x = Thing->x+DX;
		Thing->y = Thing->y+DY;
		MapChanges++;
		}

	return;
}

//--- Supprime les zones & passages se trouvant (partiellement) hors de la carte ---

void Properties_RemoveRectEx(NODE *List, LONG Width, LONG Height, BOOL *Nasty, UNDO **Parent)
{
	MAPRECTEX	*RectEx;
	UNDO		*Child;
	UINT		 Type;
	char		*Text;

	for (RectEx = (MAPRECTEX *)List->next; RectEx != 0;)
		{
		if ( (RectEx->rect.x1 <= 0) || (RectEx->rect.x1 >= Width-1) || (RectEx->rect.y1 <= 0) || (RectEx->rect.y1 >= Height-1) || (RectEx->rect.x2 <= 0) || (RectEx->rect.x2 >= Width-1) || (RectEx->rect.y2 <= 0) || (RectEx->rect.y2 >= Height-1) )
			{
			switch(RectEx->node.type)
				{
				case RECTEX_AREA:
					if (!Triggers_IsUsed(NULL,RectEx->id,NULL,SCOPE_AREA)) *Nasty = TRUE;
					Type = UNDO_SUBAREA;
					Text = szMapSubArea;
					break;
				case RECTEX_GATE:
					if (!Triggers_IsUsed(NULL,RectEx->id,NULL,SCOPE_GATE)) *Nasty = TRUE;
					Type = UNDO_SUBGATE;
					Text = szMapSubGate;
					break;
				case RECTEX_EFFECT:
					Type = UNDO_SUBEFFECT;
					Text = szMapSubEffect;
					break;
				}
			Child = Undo_CreateNewChild(Parent,TRUE,Type,Text);
			if (!Undo_StoreEntry(Child,(NODE *)RectEx,Type))
				{
				Undo_PrintError(hWnd);
				Undo_DestroyEntry(*Parent);
				Undo_Update();
				*Parent = NULL;
				}
			RectEx = (MAPRECTEX *)List->next;
			MapChanges++;
			continue;
			}
		RectEx = (MAPRECTEX *)RectEx->node.next;
		}

	return;
}

//--- Met à jour les positions des zones & passages ---

void Properties_UpdateRectEx(NODE *List, LONG DX, LONG DY, UNDO **Parent)
{
	MAPRECTEX	*RectEx;
	UNDO		*Child;
	UINT		 Type;
	char		*Text;

	if ((!DX)&&(!DY)) return;

	for (RectEx = (MAPRECTEX *)List->next; RectEx != 0; RectEx = (MAPRECTEX *)RectEx->node.next)
		{
		switch(RectEx->node.type)
			{
			case RECTEX_AREA:
				Type = UNDO_MODAREA;
				Text = szMapModArea;
				break;
			case RECTEX_GATE:
				Type = UNDO_MODGATE;
				Text = szMapModGate;
				break;
			case RECTEX_EFFECT:
				Type = UNDO_MODEFFECT;
				Text = szMapModEffect;
				break;
			}
		Child = Undo_CreateNewChild(Parent,TRUE,Type,Text);
		if (!Undo_StoreEntry(Child,(NODE *)RectEx,Type))
			{
			Undo_PrintError(hWnd);
			Undo_DestroyEntry(*Parent);
			Undo_Update();
			*Parent = NULL;
			}
		RectEx->rect.x1 = RectEx->rect.x1+DX;
		RectEx->rect.y1 = RectEx->rect.y1+DY;
		RectEx->rect.x2 = RectEx->rect.x2+DX;
		RectEx->rect.y2 = RectEx->rect.y2+DY;
		MapChanges++;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Valeurs par défaut						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Réinitialise les options d'une carte ««««««««««««««««««««««««««««»

void Properties_MapResetOptions(MAP *Map, MAPOPTIONS *MapOptions)
{
	int	Count;
	int	i;

	ZeroMemory(Map,sizeof(MAP));
	Map->Properties.Players = 1;
	Map->Properties.Version = 17;
	Map->Properties.FillID = MAP_DEFAULTID;
	strcpy(Map->Properties.Name,szNoName);

	Map->Properties.Allies[1][0] = 1;
	Map->Properties.Allies[1][2] = 1;
	Map->Properties.Allies[1][3] = 1;
	Map->Properties.Allies[1][4] = 1;
	Map->Properties.Allies[1][5] = 1;
	Map->Properties.Allies[1][6] = 1;
	Map->Properties.Allies[0][1] = 1;
	Map->Properties.Allies[2][1] = 1;
	Map->Properties.Allies[3][1] = 1;
	Map->Properties.Allies[4][1] = 1;
	Map->Properties.Allies[5][1] = 1;
	Map->Properties.Allies[6][1] = 1;
	Map->Properties.Options = 0x04;
	Map->Properties.Soundtrack = 1;
	Map->Properties.GameTicks = 4;
	Map->Properties.Players = 1;

	ZeroMemory(&Map->Properties.Variables[0],256);
	for (i = 0; MapVars[i].Id != 0; i++)
		{
		if (MapVars[i].Id == 0xFFFFFFFF) continue;
		Map->Properties.Variables[MapVars[i].Id] = MapVars[i].Variable;
		}

	if (MapOptions)
		{
		for (Count = 0; Count != MAP_MAXPLAYERS; Count++)
			{
			Properties_MapOptionsToDefault(&MapOptions[Count],Count+1);
			for (i = 0; i != 45; i++) MapOptions[Count].CreaturesCount[i] = -1;
			for (i = 0; i != 15; i++) MapOptions[Count].Rooms[i] = -1;
			for (i = 0; i != 14; i++) MapOptions[Count].Traps[i] = -1;
			for (i = 0; i != 8; i++)  MapOptions[Count].Doors[i] = -1;
			for (i = 0; i != 12; i++) MapOptions[Count].Spells[i] = -1;
			}
		}

	return;
}


// «»»» Options d'un joueur par défaut ««««««««««««««««««««««««««««««««««»

void Properties_MapOptionsToDefault(MAPOPTIONS *MapOptions, UINT Player)
{
	int	i;

	ZeroMemory(MapOptions,sizeof(MAPOPTIONS));

	MapOptions->PlayerHealth				= 100;
	MapOptions->PlayerGold					= 3000;
	MapOptions->PlayerComputer				= FALSE;

	Properties_MapOptionsKeeperAI(0,MapOptions);

	MapOptions->PlayerCPUAreaX1				= -1;
	MapOptions->PlayerCPUAreaY1				= -1;
	MapOptions->PlayerCPUAreaX2				= -1;
	MapOptions->PlayerCPUAreaY2				= -1;
	MapOptions->PlayerCameraX				= -1;
	MapOptions->PlayerCameraY				= -1;

	switch(Player)
		{
		case 1:	MapOptions->PlayerComputer = TRUE;
			strcpy(MapOptions->PlayerName,szKwdHeros);
			break;
		case 2:	MapOptions->PlayerComputer = TRUE;
			strcpy(MapOptions->PlayerName,szKwdNeutral);
			break;
		case 3:	strcpy(MapOptions->PlayerName,szKwdKeeper1);
			break;
		case 4:	strcpy(MapOptions->PlayerName,szKwdKeeper2);
			break;
		case 5:	strcpy(MapOptions->PlayerName,szKwdKeeper3);
			break;
		case 6:	strcpy(MapOptions->PlayerName,szKwdKeeper4);
			break;
		case 7:	strcpy(MapOptions->PlayerName,szKwdKeeper5);
			break;
		}

	//--- Nombre de créatures maximum ---

	if ((Player == 1)||(Player == 2))
		{
		for (i = 0; i != 45; i++)
			MapOptions->CreaturesCount[i] = -1;
		}
	else
		{
						MapOptions->CreaturesCount[0]  = -1;	// Lutin
		for (i = 1; i != 13; i++)	MapOptions->CreaturesCount[i]  = 32;
						MapOptions->CreaturesCount[6]  = -1;	// Squelette
						MapOptions->CreaturesCount[11] = -1;	// Vampire
						MapOptions->CreaturesCount[13] = 20;	// Ange noir
						MapOptions->CreaturesCount[14] = 20;	// Vierge
		for (i = 15; i != 45; i++)	MapOptions->CreaturesCount[i] = -1;	// Elites & Héros
		}

	//--- Statut des portes, pièges, sorts et salles ---

	if ((Player != 1)&&(Player != 2))
		{
		for (i = 0; i != 15; i++)	MapOptions->Rooms[i] = 3;
		for (i = 0; i != 11; i++)	MapOptions->Traps[i] = 3;
						MapOptions->Traps[11] = -1;		// Sentinelle (héros)
						MapOptions->Traps[12] = -1;		// Alarme (héros)
						MapOptions->Traps[13] = -1;		// Diable à ressort
		for (i = 0; i != 6; i++)	MapOptions->Doors[i] = 3;
						MapOptions->Doors[6] = -1;		// Barrière en pierre
						MapOptions->Doors[7] = -1;		// Porte renforcée des héros
						MapOptions->Spells[0] = 3;		// Création de lutins
						MapOptions->Spells[1] = 3;		// Possession
		for (i = 2; i != 11; i++)	MapOptions->Spells[i] = 2;
						MapOptions->Spells[11] = 3;		// Invoquer Horny
		}
	return;
}


// «»»» Paramètre des gardiens informatique «««««««««««««««««««««««««««««»

void Properties_MapOptionsKeeperAI(UINT AI, MAPOPTIONS *MapOptions)
{
	switch(AI)
		{
		//
		// Maître
		//
		case 0:	MapOptions->PlayerAI					= 0;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 50;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 9000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 0;
			MapOptions->PlayerImpsToTilesRatio			= 15;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 90;
			MapOptions->PlayerFindSpecialsTiles			= 10;
			MapOptions->PlayerJailedFate				= 0;
			MapOptions->PlayerResearchedRoom			= 0;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 25;
			MapOptions->PlayerTilesBetweenRooms			= 2;
			MapOptions->PlayerDistance				= 0;
			MapOptions->PlayerCorridorStyle				= 1;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 25;
			MapOptions->PlayerTilesFromClaimed			= 4;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= TRUE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 0;
			MapOptions->PlayerUseTrapsnDoors			= 99;
			MapOptions->PlayerSpellsSightOfEvil			= 2;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 1;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= TRUE;
			MapOptions->PlayerSpellsLightning			= TRUE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 20;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= TRUE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= TRUE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= FALSE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 15;
			MapOptions->PlayerFirstAttackLevel			= 8;
			MapOptions->PlayerFirstAttackThreat			= 20;
			MapOptions->PlayerFirstAttackBreach			= 2;
			MapOptions->PlayerFirstAttackDig			= 3;
			MapOptions->PlayerFirstAttackPoints			= 2;
			MapOptions->PlayerFirstAttackCreatures			= 50;
			MapOptions->PlayerBattleDropped				= 125;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 5;
			MapOptions->PlayerBattleHealth				= 10;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 10;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2000;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 1;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Conquérant
		//
		case 1:	MapOptions->PlayerAI					= 1;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 50;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 1;
			MapOptions->PlayerImpsToTilesRatio			= 25;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 50;
			MapOptions->PlayerFindSpecialsTiles			= 8;
			MapOptions->PlayerJailedFate				= 2;
			MapOptions->PlayerResearchedRoom			= 15;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 16;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 0;
			MapOptions->PlayerCorridorStyle				= 0;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 20;
			MapOptions->PlayerTilesFromClaimed			= 3;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 0;
			MapOptions->PlayerUseTrapsnDoors			= 70;
			MapOptions->PlayerSpellsSightOfEvil			= 1;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 1;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= TRUE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 15;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= FALSE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 10;
			MapOptions->PlayerFirstAttackLevel			= 5;
			MapOptions->PlayerFirstAttackThreat			= 125;
			MapOptions->PlayerFirstAttackBreach			= 0;
			MapOptions->PlayerFirstAttackDig			= 3;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 75;
			MapOptions->PlayerBattleDropped				= 100;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 5;
			MapOptions->PlayerBattleHealth				= 10;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 5;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 1500;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 1;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Psychopathe
		//
		case 2:	MapOptions->PlayerAI					= 2;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 50;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 2;
			MapOptions->PlayerImpsToTilesRatio			= 35;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 20;
			MapOptions->PlayerFindSpecialsTiles			= 15;
			MapOptions->PlayerJailedFate				= 1;
			MapOptions->PlayerResearchedRoom			= 30;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 9;
			MapOptions->PlayerTilesBetweenRooms			= 0;
			MapOptions->PlayerDistance				= 1;
			MapOptions->PlayerCorridorStyle				= 0;
			MapOptions->PlayerExpandMode				= 1;
			MapOptions->PlayerTilesFromHeart			= 40;
			MapOptions->PlayerTilesFromClaimed			= 5;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 1;
			MapOptions->PlayerUseTrapsnDoors			= 30;
			MapOptions->PlayerSpellsSightOfEvil			= 2;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 2;
			MapOptions->PlayerBoulderInCorridors			= FALSE;
			MapOptions->PlayerBoulderInAngles			= FALSE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 0;
			MapOptions->PlayerCreaturesImps				= 10;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= FALSE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= FALSE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= FALSE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 5;
			MapOptions->PlayerFirstAttackLevel			= 1;
			MapOptions->PlayerFirstAttackThreat			= 0;
			MapOptions->PlayerFirstAttackBreach			= 3;
			MapOptions->PlayerFirstAttackDig			= 2;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 100;
			MapOptions->PlayerBattleDropped				= 50;
			MapOptions->PlayerBattleSpellType			= 0;
			MapOptions->PlayerBattlePrison				= 0;
			MapOptions->PlayerBattleHealth				= 10;
			MapOptions->PlayerBattleRemove				= FALSE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 1;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 1000;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 1;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Inébranlable
		//
		case 3:	MapOptions->PlayerAI					= 3;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 20;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 9000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 0;
			MapOptions->PlayerImpsToTilesRatio			= 15;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 90;
			MapOptions->PlayerFindSpecialsTiles			= 8;
			MapOptions->PlayerJailedFate				= 0;
			MapOptions->PlayerResearchedRoom			= 0;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 25;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 0;
			MapOptions->PlayerCorridorStyle				= 1;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 25;
			MapOptions->PlayerTilesFromClaimed			= 3;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 0;
			MapOptions->PlayerUseTrapsnDoors			= 99;
			MapOptions->PlayerSpellsSightOfEvil			= 2;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 0;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= TRUE;
			MapOptions->PlayerSpellsLightning			= TRUE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 15;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= TRUE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= TRUE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= FALSE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 15;
			MapOptions->PlayerFirstAttackLevel			= 8;
			MapOptions->PlayerFirstAttackThreat			= 100;
			MapOptions->PlayerFirstAttackBreach			= 2;
			MapOptions->PlayerFirstAttackDig			= 0;
			MapOptions->PlayerFirstAttackPoints			= 2;
			MapOptions->PlayerFirstAttackCreatures			= 50;
			MapOptions->PlayerBattleDropped				= 125;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 15;
			MapOptions->PlayerBattleHealth				= 20;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 10;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2150;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 2;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Vieux sage
		//
		case 4:	MapOptions->PlayerAI					= 4;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 20;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 1;
			MapOptions->PlayerImpsToTilesRatio			= 20;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 50;
			MapOptions->PlayerFindSpecialsTiles			= 5;
			MapOptions->PlayerJailedFate				= 2;
			MapOptions->PlayerResearchedRoom			= 15;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 16;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 0;
			MapOptions->PlayerCorridorStyle				= 0;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 20;
			MapOptions->PlayerTilesFromClaimed			= 3;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 0;
			MapOptions->PlayerUseTrapsnDoors			= 50;
			MapOptions->PlayerSpellsSightOfEvil			= 1;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 0;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= FALSE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 10;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= FALSE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 15;
			MapOptions->PlayerFirstAttackLevel			= 5;
			MapOptions->PlayerFirstAttackThreat			= 100;
			MapOptions->PlayerFirstAttackBreach			= 1;
			MapOptions->PlayerFirstAttackDig			= 1;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 40;
			MapOptions->PlayerBattleDropped				= 110;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 8;
			MapOptions->PlayerBattleHealth				= 20;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 10;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2000;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 2;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Idiot
		//
		case 5:	MapOptions->PlayerAI					= 5;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 20;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 2;
			MapOptions->PlayerImpsToTilesRatio			= 25;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 20;
			MapOptions->PlayerFindSpecialsTiles			= 2;
			MapOptions->PlayerJailedFate				= 1;
			MapOptions->PlayerResearchedRoom			= 30;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 12;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 1;
			MapOptions->PlayerCorridorStyle				= 2;
			MapOptions->PlayerExpandMode				= 1;
			MapOptions->PlayerTilesFromHeart			= 15;
			MapOptions->PlayerTilesFromClaimed			= 3;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 1;
			MapOptions->PlayerUseTrapsnDoors			= 25;
			MapOptions->PlayerSpellsSightOfEvil			= 2;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 2;
			MapOptions->PlayerBoulderInCorridors			= FALSE;
			MapOptions->PlayerBoulderInAngles			= FALSE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 0;
			MapOptions->PlayerCreaturesImps				= 5;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= TRUE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 15;
			MapOptions->PlayerFirstAttackLevel			= 2;
			MapOptions->PlayerFirstAttackThreat			= 100;
			MapOptions->PlayerFirstAttackBreach			= 3;
			MapOptions->PlayerFirstAttackDig			= 2;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 30;
			MapOptions->PlayerBattleDropped				= 90;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 25;
			MapOptions->PlayerBattleHealth				= 20;
			MapOptions->PlayerBattleRemove				= FALSE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 10;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2500;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 2;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Maton
		//
		case 6:	MapOptions->PlayerAI					= 6;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 15;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 9000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 0;
			MapOptions->PlayerImpsToTilesRatio			= 15;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 90;
			MapOptions->PlayerFindSpecialsTiles			= 5;
			MapOptions->PlayerJailedFate				= 0;
			MapOptions->PlayerResearchedRoom			= 0;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 25;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 0;
			MapOptions->PlayerCorridorStyle				= 2;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 15;
			MapOptions->PlayerTilesFromClaimed			= 2;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 0;
			MapOptions->PlayerUseTrapsnDoors			= 99;
			MapOptions->PlayerSpellsSightOfEvil			= 2;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 0;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= TRUE;
			MapOptions->PlayerSpellsLightning			= TRUE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 12;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= TRUE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= TRUE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= TRUE;
			MapOptions->PlayerFirstAttackDontAttack			= FALSE;
			MapOptions->PlayerFirstAttackCount			= 4;
			MapOptions->PlayerFirstAttackLevel			= 0;
			MapOptions->PlayerFirstAttackThreat			= 200;
			MapOptions->PlayerFirstAttackBreach			= 2;
			MapOptions->PlayerFirstAttackDig			= 0;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 40;
			MapOptions->PlayerBattleDropped				= 150;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 15;
			MapOptions->PlayerBattleHealth				= 30;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 4;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2000;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 5;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Dur à cuir
		//
		case 7:	MapOptions->PlayerAI					= 7;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 15;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 1;
			MapOptions->PlayerImpsToTilesRatio			= 15;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 50;
			MapOptions->PlayerFindSpecialsTiles			= 4;
			MapOptions->PlayerJailedFate				= 3;
			MapOptions->PlayerResearchedRoom			= 15;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 16;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 1;
			MapOptions->PlayerCorridorStyle				= 2;
			MapOptions->PlayerExpandMode				= 0;
			MapOptions->PlayerTilesFromHeart			= 14;
			MapOptions->PlayerTilesFromClaimed			= 2;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 2;
			MapOptions->PlayerUseTrapsnDoors			= 50;
			MapOptions->PlayerSpellsSightOfEvil			= 1;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 1;
			MapOptions->PlayerBoulderInCorridors			= TRUE;
			MapOptions->PlayerBoulderInAngles			= TRUE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 1;
			MapOptions->PlayerCreaturesImps				= 8;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= TRUE;
			MapOptions->PlayerFirstAttackDontAttack			= TRUE;
			MapOptions->PlayerFirstAttackCount			= 4;
			MapOptions->PlayerFirstAttackLevel			= 1;
			MapOptions->PlayerFirstAttackThreat			= 200;
			MapOptions->PlayerFirstAttackBreach			= 1;
			MapOptions->PlayerFirstAttackDig			= 3;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 20;
			MapOptions->PlayerBattleDropped				= 150;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 20;
			MapOptions->PlayerBattleHealth				= 30;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 4;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 2500;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 5;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		//
		// Paranoïaque
		//
		case 8:	MapOptions->PlayerAI					= 8;
			MapOptions->PlayerSpeed					= 1;
			MapOptions->PlayerOpenness				= 15;
			MapOptions->PlayerGoldValue				= 100;
			MapOptions->PlayerManaValue				= 100;
			MapOptions->PlayerMineGoldAmount			= 6000;
			MapOptions->PlayerMana					= 100;
			MapOptions->PlayerLibraryUsage				= 2;
			MapOptions->PlayerImpsToTilesRatio			= 15;
			MapOptions->PlayerFindSpecialsExplore			= 0;
			MapOptions->PlayerFindSpecialsChances			= 20;
			MapOptions->PlayerFindSpecialsTiles			= 3;
			MapOptions->PlayerJailedFate				= 1;
			MapOptions->PlayerResearchedRoom			= 30;
			MapOptions->PlayerRebuildRoom				= 0;
			MapOptions->PlayerDefaultTiles				= 12;
			MapOptions->PlayerTilesBetweenRooms			= 1;
			MapOptions->PlayerDistance				= 1;
			MapOptions->PlayerCorridorStyle				= 0;
			MapOptions->PlayerExpandMode				= 1;
			MapOptions->PlayerTilesFromHeart			= 13;
			MapOptions->PlayerTilesFromClaimed			= 1;
			MapOptions->PlayerFlexibility				= 0;
			MapOptions->PlayerRoomToBuildPri[0]			= 1;
			MapOptions->PlayerRoomToBuildPri[1]			= 2;
			MapOptions->PlayerRoomToBuildPri[2]			= 3;
			MapOptions->PlayerRoomToBuildPri[3]			= 4;
			MapOptions->PlayerRoomToBuildPri[4]			= 6;
			MapOptions->PlayerRoomToBuildPri[5]			= 7;
			MapOptions->PlayerRoomToBuildPri[6]			= 8;
			MapOptions->PlayerRoomToBuildPri[7]			= 9;
			MapOptions->PlayerRoomToBuildPri[8]			= 10;
			MapOptions->PlayerRoomToBuildPri[9]			= 11;
			MapOptions->PlayerRoomToBuildPri[10]			= 12;
			MapOptions->PlayerRoomToBuildPri[11]			= 13;
			MapOptions->PlayerRoomToBuildPri[12]			= 14;
			MapOptions->PlayerRoomToBuildPri[13] 			= 15;
			MapOptions->PlayerRoomToBuildPri[14]			= 16;
			MapOptions->PlayerSellCapturedRoom			= FALSE;
			MapOptions->PlayerCreateEmptyAreas			= FALSE;
			MapOptions->PlayerBuildBiggerLair			= FALSE;
			MapOptions->PlayerTrapUseStyle				= 2;
			MapOptions->PlayerPreference				= 2;
			MapOptions->PlayerDoorUsage				= 1;
			MapOptions->PlayerUseTrapsnDoors			= 25;
			MapOptions->PlayerSpellsSightOfEvil			= 1;
			MapOptions->PlayerSpellsInBattle			= 2;
			MapOptions->PlayerSpellsPower				= 2;
			MapOptions->PlayerSpellsCallToArms			= 2;
			MapOptions->PlayerBoulderInCorridors			= FALSE;
			MapOptions->PlayerBoulderInAngles			= FALSE;
			MapOptions->PlayerSpellsLightning			= FALSE;
			MapOptions->PlayerCreaturesSackMethod			= 0;
			MapOptions->PlayerCreaturesImps				= 4;
			MapOptions->PlayerCreaturesSlap				= 0x20;
			MapOptions->PlayerCreaturesHappyUnhappy			= FALSE;
			MapOptions->PlayerCreaturesHappyUngry			= FALSE;
			MapOptions->PlayerCreaturesDisposeUngry			= TRUE;
			MapOptions->PlayerCreaturesDisposeRubish		= FALSE;
			MapOptions->PlayerUseLevelInFirstAttack			= TRUE;
			MapOptions->PlayerUseCountInFirstAttack			= FALSE;
			MapOptions->PlayerUseThreatInFirstAttack		= TRUE;
			MapOptions->PlayerFirstAttackAllRooms			= FALSE;
			MapOptions->PlayerFirstAttackAllSpells			= FALSE;
			MapOptions->PlayerFirstAttackOnlyAttackers		= TRUE;
			MapOptions->PlayerFirstAttackDontAttack			= TRUE;
			MapOptions->PlayerFirstAttackCount			= 4;
			MapOptions->PlayerFirstAttackLevel			= 0;
			MapOptions->PlayerFirstAttackThreat			= 200;
			MapOptions->PlayerFirstAttackBreach			= 3;
			MapOptions->PlayerFirstAttackDig			= 2;
			MapOptions->PlayerFirstAttackPoints			= 1;
			MapOptions->PlayerFirstAttackCreatures			= 10;
			MapOptions->PlayerBattleDropped				= 150;
			MapOptions->PlayerBattleSpellType			= 2;
			MapOptions->PlayerBattlePrison				= 25;
			MapOptions->PlayerBattleHealth				= 30;
			MapOptions->PlayerBattleRemove				= TRUE;
			MapOptions->PlayerBattleWait				= 90;
			MapOptions->PlayerBattleCallToHarmsRemoveNoCreature	= 4;
			MapOptions->PlayerBattleCallToHarmsRemove		= 240;
			MapOptions->PlayerBattleCallToHarmsPlace		= 3500;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger1	= 5;
			MapOptions->PlayerBattleCallToHarmsRemoveNoDanger2	= 5;
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres globaux						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlID == 110)
				Properties_MapSizeButton((DRAWITEMSTRUCT *)lParam,MapProps);
			else
				Properties_MapKeeperButton((DRAWITEMSTRUCT *)lParam);
			return(TRUE);

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 115:
							Misc_SetUpDownInt(hDlg,105,(NM_UPDOWN *)lParam);
							break;
						case 117:
							Misc_SetUpDownInt(hDlg,107,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 105:
							MapProps->MapNewWidth = GetDlgItemInt(hDlg,105,NULL,FALSE);
							if (MapProps->MapNewWidth < 0)
								{
								MapProps->MapNewWidth = 0;
								SetDlgItemInt(hDlg,105,0,FALSE);
								}
							if (MapProps->MapNewWidth > 300)
								{
								MapProps->MapNewWidth = 300;
								SetDlgItemInt(hDlg,105,300,FALSE);
								}
							InvalidateRect(GetDlgItem(hDlg,110),NULL,FALSE);
							UpdateWindow(GetDlgItem(hDlg,110));
							break;
						case 107:
							MapProps->MapNewHeight = GetDlgItemInt(hDlg,107,NULL,FALSE);
							if (MapProps->MapNewHeight < 0)
								{
								MapProps->MapNewHeight = 0;
								SetDlgItemInt(hDlg,107,0,FALSE);
								}
							if (MapProps->MapNewHeight > 300)
								{
								MapProps->MapNewHeight = 300;
								SetDlgItemInt(hDlg,107,300,FALSE);
								}
							InvalidateRect(GetDlgItem(hDlg,110),NULL,FALSE);
							UpdateWindow(GetDlgItem(hDlg,110));
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 120:
						case 121:
						case 122:
						case 123:
						case 124:
						case 125:
						case 126:
							Properties_MapPlayer(hDlg,MapProps,LOWORD(wParam)-120);
							break;
						case 127:
							Properties_MapAlliances(hDlg,MapProps);
							break;
						case 128:
							Properties_MapCreatures(hDlg,MapProps);
							break;
						case 129:
							Properties_MapVoices(hDlg,MapProps);
							break;
						case 130:
							Properties_MapTexts(hDlg,MapProps);
							break;
						case 131:
							Properties_MapVariables(hDlg,MapProps);
							break;
						case 132:
							Properties_MapLevelOptions(hDlg,MapProps);
							break;
						case 108:
							SetDlgItemInt(hDlg,105,MapProps->MapProperties.Width,FALSE);
							SetDlgItemInt(hDlg,107,MapProps->MapProperties.Height,FALSE);
							break;
						case 110:
							Properties_MapChangeSizeButton(hDlg,MapProps);
							break;
						case IDOK:
							if (Properties_MapReset(hDlg,MapProps,TRUE))
								EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Properties_MapReset(hDlg,MapProps,FALSE);
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

void Properties_MapInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HICON	hIcon;
	LONG	X,Y;
	char	Players[4];

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	Misc_CreateUpDownButton(hDlg,105,115);
	Misc_CreateUpDownButton(hDlg,107,117);
	SetDlgItemText(hDlg,101,MapProps->MapProperties.Name);
	SetDlgItemInt(hDlg,105,MapProps->MapNewWidth,FALSE);
	SetDlgItemInt(hDlg,107,MapProps->MapNewHeight,FALSE);
	SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)63,(LPARAM)0);
	SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,107,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);

	MapProps->hArrows = ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,1,1);
	if (!MapProps->hArrows) goto Error_0;
	for (X = 40; X != 47+1; X++)
		{
		hIcon = LoadImage(hInstance,MAKEINTRESOURCE(X),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		if (!hIcon) goto Error_1;
		Y = ImageList_ReplaceIcon(MapProps->hArrows,-1,hIcon);
		DestroyIcon(hIcon);
		if (Y == -1) goto Error_1;
		}

	for (X = 0; Versions[X] != NULL; X++)
		SendDlgItemMessage(hDlg,141,CB_ADDSTRING,(WPARAM)0,(LPARAM)Versions[++X]);
	for (X = 0, Y = 0; Versions[X] != NULL; X++, Y++)
		{
		if ((BYTE)Versions[X++] == MapProps->MapProperties.Version)
			{
			SendDlgItemMessage(hDlg,141,CB_SETCURSEL,(WPARAM)Y,(LPARAM)0);
			break;
			}
		}
	for (X = 1; X != MAP_MAXPLAYERS-1; X++)
		{
		wsprintf(Players,szNumber,X);
		SendDlgItemMessage(hDlg,143,CB_ADDSTRING,(WPARAM)0,(LPARAM)Players);
		}
	SendDlgItemMessage(hDlg,143,CB_SETCURSEL,(WPARAM)MapProps->MapProperties.Players-1,(LPARAM)0);

	return;

//--- Erreurs ---

Error_1:ImageList_Destroy(MapProps->hArrows);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Sauvegarde des changements et réinitialisations «««««««««««««««««»

int Properties_MapReset(HWND hDlg, MAPPROPSCTX *MapProps, BOOL Save)
{
	MESSAGEBOXSTRUCT	MessageBox;
	int			Result = 1;

	if (Save)
		{
		if (SendDlgItemMessage(hDlg,101,WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0) > 64)
			{
			ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
			MessageBox.gadgets = szBufferOverflowChoices;
			MessageBox.windowtext = szBufferOverflow;
			MessageBox.windowtitle = (char *)-1;
			MessageBox.requestbits = &Config.RequestsFlags;
			MessageBox.statebit = REQUEST_BUFOVERFLOW;
			MessageBox.flags = MBF_AUTOREPLY;
			MessageBox.defaultgadget = 1;
			MessageBox.defaulticon = MB_ICONQUESTION;
			MessageBox.windowpos = MBP_FOLLOWMOUSE;
			Result = Misc_MessageBoxExtArgs(hDlg,&MessageBox);
			if (!Result) return(0);
			}
		if (Result == 1) GetDlgItemText(hDlg,101,MapProps->MapProperties.Name,64);

		if (MapProps->MapNewWidth < 7) MapProps->MapNewWidth = 7;
		if (MapProps->MapNewHeight < 7) MapProps->MapNewHeight = 7;
		if (((MapProps->MapNewWidth != MapProps->MapProperties.Width)||(MapProps->MapNewHeight != MapProps->MapProperties.Height))&&(MapProps->MapNewWidth*MapProps->MapNewHeight > 120*120))
			{
			ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
			MessageBox.gadgets = szMessageBoxExtYesNo;
			MessageBox.windowtext = szMapSizeTooBig;
			MessageBox.windowtitle = (char *)-1;
			MessageBox.requestbits = &Config.RequestsFlags;
			MessageBox.statebit = REQUEST_BIGMAP;
			MessageBox.flags = MBF_AUTOREPLY;
			MessageBox.defaultgadget = 1;
			MessageBox.defaulticon = MB_ICONINFORMATION;
			MessageBox.windowpos = MBP_FOLLOWMOUSE;
			Result = Misc_MessageBoxExtArgs(hDlg,&MessageBox);
			if (!Result) return(0);
			}

		switch(SendDlgItemMessage(hDlg,141,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))
			{
			case 1:	MapProps->MapProperties.Version = 16;
				break;
			case 2:	MapProps->MapProperties.Version = 17;
				break;
			default:MapProps->MapProperties.Version = 13;
				break;
			}
		MapProps->MapProperties.Players = SendDlgItemMessage(hDlg,143,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;
		}

	ImageList_Destroy(MapProps->hArrows);
	return(1);
}


// «»»» Changement du bouton de dimensionnement «««««««««««««««««««««««««»

void Properties_MapChangeSizeButton(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HWND	hCtrl;
	POINT	CursorPos;
	RECT	WindowRect;
	LONG	Width,Height;

	hCtrl = GetDlgItem(hDlg,110);
	GetWindowRect(hCtrl,&WindowRect);
	GetCursorPos(&CursorPos);
	Width = (WindowRect.right-WindowRect.left)/3;
	Height = (WindowRect.bottom-WindowRect.top)/3;

	if (CursorPos.x < WindowRect.left) return;
	if (CursorPos.x > WindowRect.right) return;
	if (CursorPos.y < WindowRect.top) return;
	if (CursorPos.y > WindowRect.bottom) return;

	CursorPos.x -= WindowRect.left;
	CursorPos.y -= WindowRect.top;
	CursorPos.x /= Width;
	CursorPos.y /= Height;
	MapProps->MapNewPos = CursorPos.x+CursorPos.y*3;

	InvalidateRect(hCtrl,NULL,FALSE);
	UpdateWindow(hCtrl);
	return;
}


// «»»» Affichage du bouton de dimensionnement ««««««««««««««««««««««««««»

void Properties_MapSizeButton(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps)
{
	UINT	Arrows[3][3];
	UINT	Images[3][3];
	RECT	WorkRect;
	UINT	State;
	LONG	Width,Height;
	LONG	CX,CY;
	LONG	X,Y;
	int	i,j,k;

	if (ItemStruct->CtlType != ODT_BUTTON) return;
	if (ItemStruct->CtlID != 110) return;
	if (!(ItemStruct->itemAction&ODA_DRAWENTIRE)) return;

	Width = (ItemStruct->rcItem.right-ItemStruct->rcItem.left)/3;
	Height = (ItemStruct->rcItem.bottom-ItemStruct->rcItem.top)/3;
	WorkRect.left = ItemStruct->rcItem.left;
	WorkRect.right = WorkRect.left+Width;
	WorkRect.top = ItemStruct->rcItem.top;
	WorkRect.bottom = WorkRect.top+Height;

	for (k = 0, j = 0; j != 3; j++)
		{
		for (i = 0; i != 3; i++, k++)
			{
			State = DFCS_BUTTONPUSH;
			if (MapProps->MapNewPos == k) State |= DFCS_PUSHED;
			DrawFrameControl(ItemStruct->hDC,&WorkRect,DFC_BUTTON,State);
			WorkRect.left = WorkRect.right;
			WorkRect.right = WorkRect.left+Width;
			}
		WorkRect.left = ItemStruct->rcItem.left;
		WorkRect.right = WorkRect.left+Width;
		WorkRect.top = WorkRect.bottom;
		WorkRect.bottom = WorkRect.top+Height;
		}

	if ((MapProps->MapNewWidth >= MapProps->MapProperties.Width)&&(MapProps->MapNewHeight >= MapProps->MapProperties.Height))
		{
		Images[0][0] = 7;
		Images[0][1] = 5;
		Images[0][2] = 6;
		Images[1][0] = 4;
		Images[1][1] = 0;
		Images[1][2] = 3;
		Images[2][0] = 2;
		Images[2][1] = 0;
		Images[2][2] = 1;
		}
	else
		{
		Images[0][0] = 1;
		Images[0][2] = 2;
		Images[1][1] = 0;
		Images[2][0] = 6;
		Images[2][2] = 7;
		if (MapProps->MapNewHeight < MapProps->MapProperties.Height)
			{
			Images[0][1] = 0;
			Images[2][1] = 5;
			}
		else
			{
			Images[0][1] = 5;
			Images[2][1] = 0;
			}
		if (MapProps->MapNewWidth < MapProps->MapProperties.Width)
			{
			Images[1][0] = 3;
			Images[1][2] = 4;
			}
		else
			{
			Images[1][0] = 4;
			Images[1][2] = 3;
			}
		}

	switch(MapProps->MapNewPos)
		{
		//--- Bouton du dessus ---
		case 0:	Arrows[0][0] = 0;	/*		*/
			Arrows[0][1] = 0;	/*	 o-	*/
			Arrows[0][2] = 0;	/*	 |\	*/
			Arrows[1][0] = 0;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 0;
			Arrows[2][1] = 1;
			Arrows[2][2] = 1;
			CX = ItemStruct->rcItem.left+Width/2;
			CY = ItemStruct->rcItem.top+Height/2;
			break;
		case 1:	Arrows[0][0] = 0;	/*		*/
			Arrows[0][1] = 0;	/*	-o-	*/
			Arrows[0][2] = 0;	/*	/|\	*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 1;
			Arrows[2][1] = 1;
			Arrows[2][2] = 1;
			CX = ItemStruct->rcItem.left+Width/2+Width;
			CY = ItemStruct->rcItem.top+Height/2;
			break;
		case 2:	Arrows[0][0] = 0;	/*		*/
			Arrows[0][1] = 0;	/*	-o	*/
			Arrows[0][2] = 0;	/*	/|	*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 0;
			Arrows[2][0] = 1;
			Arrows[2][1] = 1;
			Arrows[2][2] = 0;
			CX = ItemStruct->rcItem.left+Width/2+Width*2;
			CY = ItemStruct->rcItem.top+Height/2;
			break;
		//--- Bouton du milieu ---
		case 3:	Arrows[0][0] = 0;	/*	 |/	*/
			Arrows[0][1] = 1;	/*	 o-	*/
			Arrows[0][2] = 1;	/*	 |\	*/
			Arrows[1][0] = 0;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 0;
			Arrows[2][1] = 1;
			Arrows[2][2] = 1;
			CX = ItemStruct->rcItem.left+Width/2;
			CY = ItemStruct->rcItem.top+Height/2+Height;
			break;
		case 4:	Arrows[0][0] = 1;	/*	\|/	*/
			Arrows[0][1] = 1;	/*	-o-	*/
			Arrows[0][2] = 1;	/*	/|\	*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 1;
			Arrows[2][1] = 1;
			Arrows[2][2] = 1;
			CX = ItemStruct->rcItem.left+Width/2+Width;
			CY = ItemStruct->rcItem.top+Height/2+Height;
			break;
		case 5:	Arrows[0][0] = 1;	/*	\|	*/
			Arrows[0][1] = 1;	/*	-o	*/
			Arrows[0][2] = 0;	/*	/|	*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 0;
			Arrows[2][0] = 1;
			Arrows[2][1] = 1;
			Arrows[2][2] = 0;
			CX = ItemStruct->rcItem.left+Width/2+Width*2;
			CY = ItemStruct->rcItem.top+Height/2+Height;
			break;
		//--- Boutons du bas ---
		case 6:	Arrows[0][0] = 0;	/*	 |/	*/
			Arrows[0][1] = 1;	/*	 o-	*/
			Arrows[0][2] = 1;	/*		*/
			Arrows[1][0] = 0;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 0;
			Arrows[2][1] = 0;
			Arrows[2][2] = 0;
			CX = ItemStruct->rcItem.left+Width/2;
			CY = ItemStruct->rcItem.top+Height/2+Height*2;
			break;
		case 7:	Arrows[0][0] = 1;	/*	\|/	*/
			Arrows[0][1] = 1;	/*	-o-	*/
			Arrows[0][2] = 1;	/*		*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 1;
			Arrows[2][0] = 0;
			Arrows[2][1] = 0;
			Arrows[2][2] = 0;
			CX = ItemStruct->rcItem.left+Width/2+Width;
			CY = ItemStruct->rcItem.top+Height/2+Height*2;
			break;
		case 8:	Arrows[0][0] = 1;	/*	\|	*/
			Arrows[0][1] = 1;	/*	-o	*/
			Arrows[0][2] = 0;	/*		*/
			Arrows[1][0] = 1;
			Arrows[1][1] = 0;	/* Never Used ! */
			Arrows[1][2] = 0;
			Arrows[2][0] = 0;
			Arrows[2][1] = 0;
			Arrows[2][2] = 0;
			CX = ItemStruct->rcItem.left+Width/2+Width*2;
			CY = ItemStruct->rcItem.top+Height/2+Height*2;
			break;
		}

	Y = CY-Height/2;
	for (j = 0; j != 3; j++)
		{
		X = CX-Width/2;
		for (i = 0; i != 3; i++)
			{
			if (Arrows[j][i]) ImageList_Draw(MapProps->hArrows,Images[j][i],ItemStruct->hDC,X-8,Y-8,ILD_TRANSPARENT);
			X += Width/2;
			}
		Y += Height/2;
		}

	return;
}


// «»»» Affichage des boutons des joueurs «««««««««««««««««««««««««««««««»

void Properties_MapKeeperButton(DRAWITEMSTRUCT *ItemStruct)
{
	RECT	 WorkRect;
	HBRUSH	 PlayerBrush;
	HBRUSH	 OldBrush;

	DrawFrameControl(ItemStruct->hDC,&ItemStruct->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|((ItemStruct->itemState&ODS_SELECTED)?DFCS_PUSHED:0));
	CopyRect(&WorkRect,&ItemStruct->rcItem);
	WorkRect.left += 4;
	WorkRect.top += 4;
	WorkRect.right = WorkRect.left+16;
	WorkRect.bottom = WorkRect.bottom-4;
	DrawEdge(ItemStruct->hDC,&WorkRect,(ItemStruct->itemState&ODS_SELECTED)?BDR_RAISEDOUTER:BDR_SUNKENOUTER,BF_RECT);

	PlayerBrush = CreateSolidBrush(Players[ItemStruct->CtlID-120].Color);
	if (PlayerBrush)
		{
		OldBrush = SelectObject(ItemStruct->hDC,PlayerBrush);
		if (OldBrush)
			{
			WorkRect.left++;
			WorkRect.top++;
			WorkRect.right--;
			WorkRect.bottom--;
			FillRect(ItemStruct->hDC,&WorkRect,PlayerBrush);
			SelectObject(ItemStruct->hDC,OldBrush);
			}
		DeleteObject(PlayerBrush);
		}

	WorkRect.left = ItemStruct->rcItem.left+24;
	WorkRect.top = ItemStruct->rcItem.top+2;
	WorkRect.right = ItemStruct->rcItem.right-2;
	WorkRect.bottom = ItemStruct->rcItem.bottom-2;
	DrawText(ItemStruct->hDC,Players[ItemStruct->CtlID-120].Name,-1,&WorkRect,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
