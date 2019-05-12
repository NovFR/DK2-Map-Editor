
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres de la carte
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes des fonctions						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Properties_MapOptions(HWND);
void			Properties_RemoveThings(NODE *,LONG,LONG,BOOL *,UNDO **);
void			Properties_UpdateThings(NODE *,LONG,LONG,UNDO **);
void			Properties_RemoveRectEx(NODE *,LONG,LONG,BOOL *,UNDO **);
void			Properties_UpdateRectEx(NODE *,LONG,LONG,UNDO **);
void			Properties_UpdateTriggers(LONG,LONG,UNDO **);
void			Properties_MapResetOptions(MAP *,MAPOPTIONS *);
void			Properties_MapOptionsToDefault(MAPOPTIONS *,UINT);

BOOL CALLBACK		Properties_MapProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapInit(HWND,MAPPROPSCTX *);
int			Properties_MapReset(HWND,MAPPROPSCTX *,BOOL);
void			Properties_MapChangeSizeButton(HWND,MAPPROPSCTX *);
void			Properties_MapSizeButton(DRAWITEMSTRUCT *,MAPPROPSCTX *);

void			Properties_MapPlayer(HWND,MAPPROPSCTX *,UINT);
BOOL CALLBACK		Properties_MapPlayerProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapPlayerInit(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerReset(HWND,BOOL,MAPPROPSCTX *);
void			Properties_MapPlayerToDefault(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerDrawItem(DRAWITEMSTRUCT *,MAPPROPSCTX *,LONG);
void			Properties_MapPlayerRoomsChange(HWND,UINT,MAPPROPSCTX *);
void			Properties_MapPlayerRoomsChanged(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerRoomsDispInfo(LV_DISPINFO *,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesChange(HWND,BOOL,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesChanged(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesDispInfo(LV_DISPINFO *,MAPPROPSCTX *);

void			Properties_MapComputerOptions(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapComputerProc(HWND,UINT,WPARAM,LPARAM);

void			Properties_MapAlliances(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapAlliancesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapAlliancesDefault(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesHide(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesButton(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesDisplay(DRAWITEMSTRUCT *,MAPPROPSCTX *);

void			Properties_MapCreatures(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapCreaturesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapCreaturesInit(HWND,MAPPROPSCTX *);
void			Properties_MapCreaturesChanged(HWND,MAPPROPSCTX *);
void			Properties_MapCreaturesSetParam(HWND,UINT,MAPPROPSCTX *);
void			Properties_MapCreaturesDispInfo(LV_DISPINFO *,MAPPROPSCTX *);

extern MAP*		Map_PrepareResize(LONG,LONG,MAP *);
extern void		Map_CorrectPos(HWND);
extern void		Map_Destroy(MAP *);
extern void		Map_Lock(BOOL,BOOL);
extern void		Map_SetPropThumb(HWND,UINT);
extern void		Mouse_MovedUpdate(HWND);
extern void		Minimap_Build(HWND);
extern void		Map_CorrectAfterResize(void);
extern void		Info_Update(void);
extern void		Files_SaveUpdate(void);
extern char*		Things_GetThingNameFromID(DWORD,ELEMENT *);

extern LONG		Compiler_InfoRoomInRect(UINT,MAPRECT *);
extern LONG		Compiler_InfoTreasuryCapacityInRect(UINT,MAPRECT *);
extern LONG		Compiler_InfoCreaturesCountInRect(UINT,DWORD,MAPRECT *);
extern LONG		Compiler_InfoGoldAmountInRect(DWORD,BOOL *,MAPRECT *);

extern UINT		Misc_GetArea(HWND,UINT,BOOL);
extern MAPRECTEX*	RectEx_FindById(UINT,NODE *);
extern int		Triggers_IsUsed(HWND,ULONG,BOOL *,DWORD);

extern void		List_AddEntry(NODE *,NODE *);
extern void		List_ReleaseMemory(NODE *);

extern UNDO*		Undo_CreateNewEntry(UINT,char *);
extern UNDO*		Undo_CreateNewChild(UNDO **,BOOL,UINT,char *);
extern int		Undo_StoreEntry(UNDO *,NODE *,UINT);
extern int		Undo_StoreMapParams(UNDO *,MAP *,MAPOPTIONS *);
extern int		Undo_StoreFullMapChanges(UNDO *,MAP *);
extern void		Undo_PrintError(HWND);
extern void		Undo_DestroyEntry(UNDO *);
extern void		Undo_Update(void);
extern void		Window_UnderCursor(HWND,LONG *,LONG *);
extern int		Misc_CreateAreaComboBoxList(HWND,UINT,BOOL);
extern int		Misc_CreateImageList(ELEMENT *,HIMAGELIST *,LONG,LONG,BOOL);
extern void		Misc_DestroyImageList(HIMAGELIST *);
extern void		Misc_PrintError(HWND,char *,char *,UINT);
extern int		Misc_MessageBoxExtArgs(HWND,MESSAGEBOXSTRUCT *);


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

	if (Lock&LOCK_MAPPROPERTIES) return;

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
	CopyMemory(&MapOptions[0],&MapProps->Players[0],sizeof(MAPOPTIONS)*7);
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

			// Pass 4... Update areas & gates positions

			Properties_UpdateRectEx(&MapAreas,OutMapX-InMapX,OutMapY-InMapY,&Undo);
			Properties_UpdateRectEx(&MapGates,OutMapX-InMapX,OutMapY-InMapY,&Undo);

			// Pass 5... Remove areas & gates if necessary
			//
			//	     Note: Even partially displayed areas and gates
			//		   are removed !

			Properties_RemoveRectEx(&MapAreas,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);
			Properties_RemoveRectEx(&MapGates,Map.Properties.Width,Map.Properties.Height,&Nasty,&Undo);

			// Pass 6... Update triggers

			Properties_UpdateTriggers(OutMapX-InMapX,OutMapY-InMapY,&Undo);

			// Pass 7... Update everything

			Map_CorrectAfterResize();
			Mouse_MovedUpdate(hWnd);
			Map_SetPropThumb(hWnd,PROP_VERTICAL);
			Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
			Map_CorrectPos(hWnd);
			Minimap_Build(hWnd);
			Map_Lock(FALSE,TRUE);
			Info_Update();
			}
		}
	Files_SaveUpdate();
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


// «»»» Place les valeurs par défaut pour les joueurs «««««««««««««««««««»

//--- Réinitialise les options d'une carte (chargement) ---

void Properties_MapResetOptions(MAP *Map, MAPOPTIONS *MapOptions)
{
	int	Count;
	int	i;

	ZeroMemory(Map,sizeof(MAP));
	Map->Properties.Players = 1;
	Map->Properties.FillID = MAP_DEFAULTID;
	strcpy(Map->Properties.Name,szNoName);

	for (Count = 0; Count != MAP_MAXPLAYERS; Count++)
		{
		Properties_MapOptionsToDefault(&MapOptions[Count],Count+1);
		for (i = 0; i != 45; i++) MapOptions[Count].CreaturesCount[i] = -1;
		for (i = 0; i != 15; i++) MapOptions[Count].Rooms[i] = -1;
		for (i = 0; i != 14; i++) MapOptions[Count].Traps[i] = -1;
		for (i = 0; i != 8; i++)  MapOptions[Count].Doors[i] = -1;
		for (i = 0; i != 12; i++) MapOptions[Count].Spells[i] = -1;
		}

	return;
}

//--- Options d'un joueur par défaut ---

void Properties_MapOptionsToDefault(MAPOPTIONS *MapOptions, UINT Player)
{
	int	i;

	ZeroMemory(MapOptions,sizeof(MAPOPTIONS));
	MapOptions->HeartX = 0;
	MapOptions->HeartY = 0;
	MapOptions->Gold = 3000;
	MapOptions->Health = 100;
	MapOptions->ComputerPlayer = FALSE;
	MapOptions->AI = 0;
	MapOptions->CPUAreaX1 = -1;
	MapOptions->CPUAreaY1 = -1;
	MapOptions->CPUAreaX2 = -1;
	MapOptions->CPUAreaY2 = -1;
	switch(Player)
		{
		case 1:	MapOptions->ComputerPlayer = TRUE;
			strcpy(MapOptions->PlayerName,szKwdHeros);
			break;
		case 2:	MapOptions->ComputerPlayer = TRUE;
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
			Properties_MapSizeButton((DRAWITEMSTRUCT *)lParam,MapProps);
			return(TRUE);

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 105:
							MapProps->MapNewWidth = GetDlgItemInt(hDlg,105,NULL,FALSE);
							InvalidateRect(GetDlgItem(hDlg,110),NULL,FALSE);
							UpdateWindow(GetDlgItem(hDlg,110));
							break;
						case 107:
							MapProps->MapNewHeight = GetDlgItemInt(hDlg,107,NULL,FALSE);
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
						case 108:
							SetDlgItemInt(hDlg,105,MapProps->MapNewWidth,FALSE);
							SetDlgItemInt(hDlg,107,MapProps->MapNewHeight,FALSE);
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
	char	Number[4];

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	SetDlgItemText(hDlg,101,MapProps->MapProperties.Name);
	SetDlgItemInt(hDlg,105,MapProps->MapNewWidth,FALSE);
	SetDlgItemInt(hDlg,107,MapProps->MapNewHeight,FALSE);

	for (X = 1; X != 5+1; X++)
		{
		wsprintf(Number,szNumber,X);
		SendDlgItemMessage(hDlg,103,CB_ADDSTRING,(WPARAM)0,(LPARAM)Number);
		}
	SendDlgItemMessage(hDlg,103,CB_SETCURSEL,(WPARAM)MapProps->MapProperties.Players-1,(LPARAM)0);

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
		MapProps->MapProperties.Players = SendDlgItemMessage(hDlg,103,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;

		if (MapProps->MapNewWidth < 12) MapProps->MapNewWidth = 12;
		if (MapProps->MapNewHeight < 12) MapProps->MapNewHeight = 12;
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des joueurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapPlayer(HWND hWnd, MAPPROPSCTX *MapProps, UINT Player)
{
	int	Result;

	MapProps->PlayerToModify = Player;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6801),hWnd,Properties_MapPlayerProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapPlayerProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapPlayerInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			((MEASUREITEMSTRUCT *)lParam)->itemWidth = 32+2+10+16;
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = 32+2;
			return(TRUE);

		case WM_DRAWITEM:
			Properties_MapPlayerDrawItem((DRAWITEMSTRUCT *)lParam,MapProps,1);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR FAR *) lParam)->code)
				{
				case LVN_GETDISPINFO:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapPlayerCreaturesDispInfo((LV_DISPINFO *)lParam,MapProps);
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hRooms)
						Properties_MapPlayerRoomsDispInfo((LV_DISPINFO *)lParam,MapProps);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapPlayerCreaturesChanged(hDlg,MapProps);
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hRooms)
						Properties_MapPlayerRoomsChanged(hDlg,MapProps);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 101:
							GetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName,64);
							break;
						case 105:
							MapProps->CurrentPlayer.Gold = GetDlgItemInt(hDlg,105,NULL,FALSE);
							break;
						case 126:
							Properties_MapPlayerCreaturesChange(hDlg,FALSE,MapProps);
							break;
						case 131:
							MapProps->CurrentPlayer.Health = GetDlgItemInt(hDlg,131,NULL,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 102:
							if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED)
								{
								MapProps->CurrentPlayer.ComputerPlayer = TRUE;
								EnableWindow(GetDlgItem(hDlg,103),TRUE);
								}
							else
								{
								MapProps->CurrentPlayer.ComputerPlayer = FALSE;
								EnableWindow(GetDlgItem(hDlg,103),FALSE);
								}
							break;
						case 103:
							Properties_MapComputerOptions(hDlg,MapProps);
							break;
						case 118:
						case 119:
						case 120:
						case 121:
						case 122:
							Properties_MapPlayerRoomsChange(hDlg,LOWORD(wParam),MapProps);
							break;
						case 127:
							Properties_MapPlayerCreaturesChange(hDlg,TRUE,MapProps);
							break;
						case 129:
							Properties_MapPlayerToDefault(hDlg,MapProps);
							break;
						case IDOK:
							Properties_MapPlayerReset(hDlg,TRUE,MapProps);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Properties_MapPlayerReset(hDlg,FALSE,MapProps);
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

void Properties_MapPlayerInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	LONG		 X,Y;
	LONG		 Width;
	int		 i;

	CopyMemory(&MapProps->CurrentPlayer,&MapProps->Players[MapProps->PlayerToModify],sizeof(MAPOPTIONS));
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	//--- Création des informations ---

	SetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName);
	if (MapProps->CurrentPlayer.ComputerPlayer)
		{
		CheckDlgButton(hDlg,102,BST_CHECKED);
		EnableWindow(GetDlgItem(hDlg,103),TRUE);
		}
	SetDlgItemInt(hDlg,105,MapProps->CurrentPlayer.Gold,FALSE);
	SetDlgItemInt(hDlg,131,MapProps->CurrentPlayer.Health,FALSE);
	Y = Compiler_InfoGoldAmountInRect(COMPILER_QUERYMAPGOLD|COMPILER_QUERYMAPGEMS,(BOOL *)&X,NULL);
	if (!X)
		SetDlgItemInt(hDlg,107,Y,FALSE);
	else 
		{
		char	Number[32];
		wsprintf(Number,szNumberPlus,Y);
		SetDlgItemText(hDlg,107,Number);
		}
	SetDlgItemInt(hDlg,109,Compiler_InfoTreasuryCapacityInRect(MapProps->PlayerToModify+1,NULL),FALSE);
	SetDlgItemInt(hDlg,111,Compiler_InfoRoomInRect(MapProps->PlayerToModify+1,NULL),FALSE);
	SetDlgItemInt(hDlg,113,Compiler_InfoCreaturesCountInRect(MapProps->PlayerToModify+1,COMPILER_QUERYALL,NULL),FALSE);
	SetDlgItemInt(hDlg,115,Compiler_InfoCreaturesCountInRect(MapProps->PlayerToModify+1,COMPILER_QUERYIMPS,NULL),FALSE);

	//--- Désactive les options pour les joueurs gérés par l'ordinateur ---

	if ((MapProps->PlayerToModify == 0)||(MapProps->PlayerToModify == 1))
		{
		for (X = 100; X != 115+1; X++)
			EnableWindow(GetDlgItem(hDlg,X),FALSE);
		EnableWindow(GetDlgItem(hDlg,130),FALSE);
		EnableWindow(GetDlgItem(hDlg,131),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
		}

	//--- Initialise les images ---

	if (!Misc_CreateImageList(VarRooms,&MapProps->hImages,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(Traps,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Doors,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Spells,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(BadCreatures,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(BadElites,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Heros,&MapProps->hImages,32,32,TRUE)) goto Error_0;

	Y = 1;

	//--- Initialisation de la liste des éléments ---

	GetWindowRect(GetDlgItem(hDlg,116),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hRooms = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hRooms) goto Error_1;
	SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Width = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Width*80/100;
	Column.pszText = szCfgColumn0;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_2;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Width*20/100;
	Column.pszText = szCfgColumn1;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hRooms,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_2;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szMapRooms;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (X = 1, i = 0; VarRooms[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = VarRooms[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szTraps;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Traps[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Traps[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szDoors;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Doors[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Doors[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szSpells;
	Item.lParam = 0;
	if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Spells[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Spells[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hRooms,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}

	//--- Initialisation de la liste des créatures ---

	GetWindowRect(GetDlgItem(hDlg,123),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hCreatures = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hCreatures) goto Error_2;
	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Width = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-4;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Width*80/100;
	Column.pszText = szCfgColumn2;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_3;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Width*20/100;
	Column.pszText = szCfgColumn3;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_3;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szBadCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (X = 1, i = 0; BadCreatures[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadCreatures[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}
	Item.iItem = X++;
	Item.pszText = szBadCreaturesElite;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (i = 0; BadElites[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadElites[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}
	Item.iItem = X++;
	Item.pszText = szGoodCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
	for (i = 0; Heros[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Heros[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_3;
		}

	return;

//--- Erreurs ---

Error_3:SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_2:SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_1:ImageList_Destroy(MapProps->hImages);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Réinitialisation des données ««««««««««««««««««««««««««««««««««««»

void Properties_MapPlayerReset(HWND hDlg, BOOL Save, MAPPROPSCTX *MapProps)
{
	LONG	Gold;

	if (Save)
		{
		Gold = Compiler_InfoTreasuryCapacityInRect(MapProps->PlayerToModify+1,NULL);
		if ( (MapProps->CurrentPlayer.Gold > Gold) && (MapProps->PlayerToModify != 0) && (MapProps->PlayerToModify != 1) )
			{
			MESSAGEBOXSTRUCT	Request;

			ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
			Request.gadgets = szMessageBoxExtOk;
			Request.windowtext = szCfgTooManyGold;
			Request.windowtitle = (char *)-1;
			Request.requestbits = &Config.RequestsFlags;
			Request.statebit = REQUEST_MAPGOLD;
			Request.flags = MBF_AUTOREPLY;
			Request.defaultgadget = 1;
			Request.defaulticon = MB_ICONINFORMATION;
			Request.windowpos = MBP_FOLLOWMOUSE;
			Misc_MessageBoxExtArgs(hDlg,&Request);
			}
		CopyMemory(&MapProps->Players[MapProps->PlayerToModify],&MapProps->CurrentPlayer,sizeof(MAPOPTIONS));
		}

	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	SendMessage(MapProps->hRooms,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	ImageList_Destroy(MapProps->hImages);
	return;
}


// «»»» Valeurs par défaut ««««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapPlayerToDefault(HWND hDlg, MAPPROPSCTX *MapProps)
{
	Properties_MapOptionsToDefault(&MapProps->CurrentPlayer,MapProps->PlayerToModify+1);
	InvalidateRect(MapProps->hRooms,NULL,FALSE);
	InvalidateRect(MapProps->hCreatures,NULL,FALSE);
	UpdateWindow(MapProps->hRooms);
	UpdateWindow(MapProps->hCreatures);
	Properties_MapPlayerRoomsChanged(hDlg,MapProps);
	Properties_MapPlayerCreaturesChanged(hDlg,MapProps);
	SetDlgItemText(hDlg,101,MapProps->CurrentPlayer.PlayerName);
	SetDlgItemInt(hDlg,105,MapProps->CurrentPlayer.Gold,FALSE);
	if (MapProps->CurrentPlayer.ComputerPlayer)
		{
		CheckDlgButton(hDlg,102,BST_CHECKED);
		EnableWindow(GetDlgItem(hDlg,103),TRUE);
		}
	else
		{
		CheckDlgButton(hDlg,102,BST_UNCHECKED);
		EnableWindow(GetDlgItem(hDlg,103),FALSE);
		}
	return;
}


// «»»» Affichage des entrées de la liste «««««««««««««««««««««««««««««««»

void Properties_MapPlayerDrawItem(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps, LONG Columns)
{
	HPEN	 LinePen,OldPen;
	SIZE	 Size;
	COLORREF OldColor;
	LV_ITEM  FarItem;
	RECT	 WorkRect;
	SIZE	 TextSize;
	HFONT	 hOldFont;
	BOOL	 Selected;
	int	 LineY;
	char	 ItemText[64];

	if (ItemStruct->CtlType != ODT_LISTVIEW) return;

	CopyRect(&WorkRect,&ItemStruct->rcItem);
	FarItem.mask = LVIF_PARAM;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	SendMessage(ItemStruct->hwndItem,LVM_GETITEM,(WPARAM)0,(LPARAM)&FarItem);
	if (FarItem.lParam) goto Valid_Entry;

//--- Affichage d'une entrée sans icône (Séparateur) ---

Separator:
	WorkRect.left += 2;

	LineY = ItemStruct->rcItem.top+(ItemStruct->rcItem.bottom-ItemStruct->rcItem.top)/2;
	LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DLIGHT));
	if (LinePen)
		{
		OldPen = SelectObject(ItemStruct->hDC,LinePen);
		MoveToEx(ItemStruct->hDC,WorkRect.left,LineY,NULL);
		LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-2,LineY);
		SelectObject(ItemStruct->hDC,OldPen);
		DeleteObject(LinePen);
		}
	LinePen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DDKSHADOW));
	if (LinePen)
		{
		OldPen = SelectObject(ItemStruct->hDC,LinePen);
		MoveToEx(ItemStruct->hDC,WorkRect.left,LineY+1,NULL);
		LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-2,LineY+1);
		SelectObject(ItemStruct->hDC,OldPen);
		DeleteObject(LinePen);
		}

	FarItem.mask = LVIF_TEXT;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	FarItem.pszText = ItemText;
	FarItem.cchTextMax = 64;
	if (SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem))
		{
		OldColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_WINDOWTEXT));
		hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&Size);
		WorkRect.left += 8;
		WorkRect.right = WorkRect.left+Size.cx+4;
		FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)(COLOR_WINDOW+1));
		WorkRect.left += 2;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		SelectObject(ItemStruct->hDC,hOldFont);
		SetTextColor(ItemStruct->hDC,OldColor);
		}
	return;

//--- Affichage d'une entrée avec un icône ---

Valid_Entry:
	WorkRect.left += 2+32;

	if (ItemStruct->itemState&ODS_SELECTED)
		Selected = TRUE;
	else
		Selected = FALSE;

	FillRect(ItemStruct->hDC,&WorkRect,(HBRUSH)((Selected)?(COLOR_HIGHLIGHT+1):(COLOR_WINDOW+1)));

	/* Draws the bitmap */

	ImageList_Draw(MapProps->hImages,FarItem.lParam-1,ItemStruct->hDC,ItemStruct->rcItem.left+2,ItemStruct->rcItem.top+1,ILD_TRANSPARENT);

	/* Draws the text */

	FarItem.mask = LVIF_TEXT;
	FarItem.iItem = ItemStruct->itemID;
	FarItem.iSubItem = 0;
	FarItem.pszText = ItemText;
	FarItem.cchTextMax = 64;
	if (SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem))
		{
		OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
		hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&TextSize);
		WorkRect.left += 8;
		WorkRect.right = WorkRect.left+SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)0,(LPARAM)0)-40;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		SelectObject(ItemStruct->hDC,hOldFont);
		SetTextColor(ItemStruct->hDC,OldColor);
		}

	/* Draws subitems */

	FarItem.iSubItem = 1;
	OldColor = SetTextColor(ItemStruct->hDC,(Selected)?(GetSysColor(COLOR_HIGHLIGHTTEXT)):(GetSysColor(COLOR_WINDOWTEXT)));
	hOldFont = SelectObject(ItemStruct->hDC,Font.hFont);
	while (Columns--)
		{
		SendMessage(ItemStruct->hwndItem,LVM_GETITEMTEXT,(WPARAM)ItemStruct->itemID,(LPARAM)&FarItem);
		GetTextExtentPoint32(ItemStruct->hDC,ItemText,strlen(ItemText),&TextSize);
		WorkRect.left = WorkRect.right;
		WorkRect.right = WorkRect.left+SendMessage(ItemStruct->hwndItem,LVM_GETCOLUMNWIDTH,(WPARAM)FarItem.iSubItem,(LPARAM)0)-8;
		DrawText(ItemStruct->hDC,ItemText,-1,&WorkRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
		WorkRect.right += 8;
		FarItem.iSubItem++;
		}
	SelectObject(ItemStruct->hDC,hOldFont);
	SetTextColor(ItemStruct->hDC,OldColor);

	return;
}


// «»»» Change l'état d'un ou plusieurs éléments ««««««««««««««««««««««««»

void Properties_MapPlayerRoomsChange(HWND hDlg, UINT CtlId, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;
	LONG	 Count;
	LONG	 State;

	if (MapProps->DoNotUpdate) return;

	CheckRadioButton(hDlg,118,122,CtlId);
	if (CtlId == 118) State = -1;
	else State = CtlId-118;

	Count = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;

	while ( (Item.iItem = SendMessage(MapProps->hRooms,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Count++;
		MapProps->CurrentPlayer.Rooms[Item.lParam-1] = (BYTE)State;
		}

	if (Count)
		{
		InvalidateRect(MapProps->hRooms,NULL,FALSE);
		UpdateWindow(MapProps->hRooms);
		}

	return;
}


// «»»» Un ou plusieurs éléments ont été sélectionnés «««««««««««««««««««»

void Properties_MapPlayerRoomsChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;
	LONG	 Count;
	LONG	 RadioCount[5];

	if (MapProps->DoNotUpdate) return;
	Count = SendMessage(MapProps->hRooms,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (!Count) goto Disable;

	//--- Analyse les créatures sélectionnées ---

	Count = 0;
	RadioCount[0] = 0;
	RadioCount[1] = 0;
	RadioCount[2] = 0;
	RadioCount[3] = 0;
	RadioCount[4] = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hRooms,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= 1;
		Count++;
		switch(MapProps->CurrentPlayer.Rooms[Item.lParam])
			{
			case 1:	RadioCount[1]++;
				break;
			case 2:	RadioCount[2]++;
				break;
			case 3:	RadioCount[3]++;
				break;
			case 4:	RadioCount[4]++;
				break;
			default:RadioCount[0]++;
				break;
			}
		}
	if (!Count) goto Disable;

	// Note: BST_INDETERMINATE does not work with radio buttons...
	//	 ...so there are all selected normally :)
	//
	MapProps->DoNotUpdate = TRUE;
	for (Count = 0; Count != 4+1; Count++)
		{
		EnableWindow(GetDlgItem(hDlg,Count+118),TRUE);
		if (RadioCount[Count] > 0) CheckDlgButton(hDlg,Count+118,BST_CHECKED);
		if (!RadioCount[Count]) CheckDlgButton(hDlg,Count+118,BST_UNCHECKED);
		}
	MapProps->DoNotUpdate = FALSE;
	return;

//--- Aucun élément n'est réellement sélectionné ---

Disable:for (Count = 118; Count != 122+1; Count++)
		EnableWindow(GetDlgItem(hDlg,Count),FALSE);
	return;
}


// «»»» Affichage du statut des éléments ««««««««««««««««««««««««««««««««»

void Properties_MapPlayerRoomsDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LV_ITEM	 Item;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;

	Item.mask = LVIF_PARAM;
	Item.iItem = DispInfo->item.iItem;
	Item.iSubItem = 0;
	SendMessage(MapProps->hRooms,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.lParam) return;

	// The line below assumes that tables are in following order :
	//	Rooms, Traps, Doors, Spells
	//
	switch(MapProps->CurrentPlayer.Rooms[Item.lParam-1])
		{
		case 1:	strcpy(DispInfo->item.pszText,szCfgIndisp);
			break;
		case 2: strcpy(DispInfo->item.pszText,szCfgSearch);
			break;
		case 3: strcpy(DispInfo->item.pszText,szCfgAcquis);
			break;
		case 4: strcpy(DispInfo->item.pszText,szCfgLocked);
			break;
		default:strcpy(DispInfo->item.pszText,szCfgNA);
			break;
		}
	return;
}


// «»»» Change l'état d'une ou plusieurs créatures ««««««««««««««««««««««»

void Properties_MapPlayerCreaturesChange(HWND hDlg, BOOL CheckBox, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	LONG	Count;
	LONG	NewCount;

	if (MapProps->DoNotUpdate) return;

	if (CheckBox)
		{
		Count = IsDlgButtonChecked(hDlg,127);
		if (Count == BST_UNCHECKED)
			{
			NewCount = -1;
			CheckDlgButton(hDlg,127,BST_CHECKED);
			EnableWindow(GetDlgItem(hDlg,125),FALSE);
			EnableWindow(GetDlgItem(hDlg,126),FALSE);
			}
		else
			{
			NewCount = GetDlgItemInt(hDlg,126,NULL,FALSE);
			CheckDlgButton(hDlg,127,BST_UNCHECKED);
			EnableWindow(GetDlgItem(hDlg,125),TRUE);
			EnableWindow(GetDlgItem(hDlg,126),TRUE);
			}
		}
	else
		NewCount = GetDlgItemInt(hDlg,126,NULL,FALSE);

	Count = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= MAP_VARROOMS+MAP_VARTRAPS+MAP_VARDOORS+MAP_VARSPELLS+1;
		if ((!CheckBox)&&(MapProps->CurrentPlayer.CreaturesCount[Item.lParam] == -1)) continue;
		Count++;
		MapProps->CurrentPlayer.CreaturesCount[Item.lParam] = NewCount;
		}

	if (Count)
		{
		InvalidateRect(MapProps->hCreatures,NULL,FALSE);
		UpdateWindow(MapProps->hCreatures);
		}

	return;
}


// «»»» Une ou plusieurs créatures ont été sélectionnées ««««««««««««««««»

void Properties_MapPlayerCreaturesChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	LONG	Count;
	LONG	Number;
	LONG	NumberTotal;
	LONG	UndeterminedTotal;

	if (MapProps->DoNotUpdate) return;
	Count = SendMessage(MapProps->hCreatures,LVM_GETSELECTEDCOUNT,(WPARAM)0,(LPARAM)0);
	if (!Count) goto Disable;

	//--- Analyse les créatures sélectionnées ---

	Count = 0;
	NumberTotal = 0;
	UndeterminedTotal = 0;
	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	while ( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1)
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		Item.lParam -= MAP_VARROOMS+MAP_VARTRAPS+MAP_VARDOORS+MAP_VARSPELLS+1;
		Count++;
		Number = MapProps->CurrentPlayer.CreaturesCount[Item.lParam];
		if (Number == -1) UndeterminedTotal++;
		else NumberTotal += Number;
		}
	if (!Count) goto Disable;

	//--- Modifie l'état des boutons ---

	EnableWindow(GetDlgItem(hDlg,127),TRUE);
	MapProps->DoNotUpdate = TRUE;

	if (UndeterminedTotal == Count)
		{
		EnableWindow(GetDlgItem(hDlg,125),FALSE);
		EnableWindow(GetDlgItem(hDlg,126),FALSE);
		CheckDlgButton(hDlg,127,BST_CHECKED);
		}
	else
		{
		EnableWindow(GetDlgItem(hDlg,125),TRUE);
		EnableWindow(GetDlgItem(hDlg,126),TRUE);
		if (UndeterminedTotal) CheckDlgButton(hDlg,127,BST_INDETERMINATE);
		else CheckDlgButton(hDlg,127,BST_UNCHECKED);
		}

	if (Count-UndeterminedTotal == 0) NumberTotal = 0;
	else NumberTotal /= Count-UndeterminedTotal;
	SetDlgItemInt(hDlg,126,NumberTotal,FALSE);

	MapProps->DoNotUpdate = FALSE;
	return;

//--- Aucune créature n'est réellement sélectionnée ---

Disable:EnableWindow(GetDlgItem(hDlg,125),FALSE);
	EnableWindow(GetDlgItem(hDlg,126),FALSE);
	EnableWindow(GetDlgItem(hDlg,127),FALSE);
	return;
}


// «»»» Affichage du nombre de créatures ««««««««««««««««««««««««««««««««»

void Properties_MapPlayerCreaturesDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LONG	Count;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;
	if (!(DispInfo->item.lParam)) return;

	// The line below assumes that tables are in following order :
	//	Bad creatures, Elites, Heros
	//
	Count = MapProps->CurrentPlayer.CreaturesCount[DispInfo->item.lParam-1-MAP_VARROOMS-MAP_VARTRAPS-MAP_VARDOORS-MAP_VARSPELLS];

	if (Count == -1)
		strcpy(DispInfo->item.pszText,szCfgNA);
	else
		wsprintf(DispInfo->item.pszText,szNumber,Count);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des gardiens informatique				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapComputerOptions(HWND hDlg, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6804),hDlg,Properties_MapComputerProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapComputerProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	MAPRECTEX	*Area;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		MapProps = (MAPPROPSCTX *)lParam;
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);
		for (X = 0; CPUAI[X] != 0; X += 2) SendDlgItemMessage(hDlg,102,CB_ADDSTRING,(WPARAM)0,(LPARAM)CPUAI[X]);
		SendDlgItemMessage(hDlg,102,CB_SETCURSEL,(WPARAM)MapProps->CurrentPlayer.AI,(LPARAM)0);
		SetDlgItemText(hDlg,103,CPUAI[MapProps->CurrentPlayer.AI*2+1]);
		if (MapProps->CurrentPlayer.CPUAreaX1 < 0) MapProps->CurrentPlayer.CPUAreaX1 = 2;
		if (MapProps->CurrentPlayer.CPUAreaY1 < 0) MapProps->CurrentPlayer.CPUAreaY1 = 2;
		if (MapProps->CurrentPlayer.CPUAreaX2 < 0) MapProps->CurrentPlayer.CPUAreaX2 = MapProps->MapProperties.Width-2;
		if (MapProps->CurrentPlayer.CPUAreaY2 < 0) MapProps->CurrentPlayer.CPUAreaY2 = MapProps->MapProperties.Height-2;
		SetDlgItemInt(hDlg,111,MapProps->CurrentPlayer.CPUAreaX1,FALSE);
		SetDlgItemInt(hDlg,112,MapProps->CurrentPlayer.CPUAreaY1,FALSE);
		SetDlgItemInt(hDlg,113,MapProps->CurrentPlayer.CPUAreaX2,FALSE);
		SetDlgItemInt(hDlg,114,MapProps->CurrentPlayer.CPUAreaY2,FALSE);
		Misc_CreateAreaComboBoxList(hDlg,121,TRUE);
		SendDlgItemMessage(hDlg,121,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 111:
						case 113:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							if (lData > MapProps->MapProperties.Width-1)
								{
								lData = MapProps->MapProperties.Width-1;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							break;
						case 112:
						case 114:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							if (lData > MapProps->MapProperties.Height-1)
								{
								lData = MapProps->MapProperties.Height-1;
								SetDlgItemInt(hDlg,LOWORD(wParam),lData,FALSE);
								}
							break;
						}
					return(TRUE);
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 102:
							MapProps->CurrentPlayer.AI = SendDlgItemMessage(hDlg,102,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							SetDlgItemText(hDlg,103,CPUAI[MapProps->CurrentPlayer.AI*2+1]);
							break;
						case 121:
							lData = Misc_GetArea(hDlg,121,TRUE);
							if (!lData) break;
							Area = RectEx_FindById(lData,&MapAreas);
							if (!Area) break;
							SetDlgItemInt(hDlg,111,Area->rect.x1,FALSE);
							SetDlgItemInt(hDlg,112,Area->rect.y1,FALSE);
							SetDlgItemInt(hDlg,113,Area->rect.x2,FALSE);
							SetDlgItemInt(hDlg,114,Area->rect.y2,FALSE);
							break;
						}
					return(TRUE);
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
						case IDCANCEL:
							MapProps->CurrentPlayer.CPUAreaX1 = GetDlgItemInt(hDlg,111,NULL,FALSE);
							MapProps->CurrentPlayer.CPUAreaY1 = GetDlgItemInt(hDlg,112,NULL,FALSE);
							MapProps->CurrentPlayer.CPUAreaX2 = GetDlgItemInt(hDlg,113,NULL,FALSE);
							MapProps->CurrentPlayer.CPUAreaY2 = GetDlgItemInt(hDlg,114,NULL,FALSE);
							if (MapProps->CurrentPlayer.CPUAreaX2 < MapProps->CurrentPlayer.CPUAreaX1)
								{
								lData = MapProps->CurrentPlayer.CPUAreaX1;
								MapProps->CurrentPlayer.CPUAreaX1 = MapProps->CurrentPlayer.CPUAreaX2;
								MapProps->CurrentPlayer.CPUAreaX2 = lData;
								}
							if (MapProps->CurrentPlayer.CPUAreaY2 < MapProps->CurrentPlayer.CPUAreaY1)
								{
								lData = MapProps->CurrentPlayer.CPUAreaY1;
								MapProps->CurrentPlayer.CPUAreaY1 = MapProps->CurrentPlayer.CPUAreaY2;
								MapProps->CurrentPlayer.CPUAreaY2 = lData;
								}
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Etablissement des alliances					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapAlliances(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6802),hWnd,Properties_MapAlliancesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapAlliancesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		MapProps = (MAPPROPSCTX *)lParam;
		CopyMemory(&MapProps->Allies[0][0],&MapProps->MapProperties.Allies[0][0],MAP_MAXPLAYERS*MAP_MAXPLAYERS);
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesView);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesSet);
		SendDlgItemMessage(hDlg,101,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCfgAlliesClear);
		SendDlgItemMessage(hDlg,101,CB_SETCURSEL,(WPARAM)1,(LPARAM)0);
		SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Properties_MapAlliancesDisplay((DRAWITEMSTRUCT *)lParam,MapProps);
			return(TRUE);

		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE) break;
			Properties_MapAlliancesHide(hDlg,MapProps);
			break;

		case WM_LBUTTONDOWN:
			Properties_MapAlliancesHide(hDlg,MapProps);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					Properties_MapAlliancesHide(hDlg,MapProps);
					return(TRUE);
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 100:
							Properties_MapAlliancesButton(hDlg,MapProps);
							break;
						case 102:
							Properties_MapAlliancesDefault(hDlg,MapProps);
							break;
						case IDOK:
							CopyMemory(&MapProps->MapProperties.Allies[0][0],&MapProps->Allies[0][0],MAP_MAXPLAYERS*MAP_MAXPLAYERS);
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


// «»»» Alliances par défaut ««««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesDefault(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HWND	hCtrl;
	int	i,j;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		for (j = 0; j != MAP_MAXPLAYERS; j++)
			MapProps->Allies[i][j] = 0;

	MapProps->Allies[1][0] = 1;
	MapProps->Allies[1][2] = 1;
	MapProps->Allies[1][3] = 1;
	MapProps->Allies[1][4] = 1;
	MapProps->Allies[1][5] = 1;
	MapProps->Allies[1][6] = 1;
	MapProps->Allies[0][1] = 1;
	MapProps->Allies[2][1] = 1;
	MapProps->Allies[3][1] = 1;
	MapProps->Allies[4][1] = 1;
	MapProps->Allies[5][1] = 1;
	MapProps->Allies[6][1] = 1;

	hCtrl = GetDlgItem(hDlg,100);
	InvalidateRect(hCtrl,NULL,TRUE);
	UpdateWindow(hCtrl);
	return;
}


// «»»» Désélectionne les boutons «««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesHide(HWND hDlg, MAPPROPSCTX *MapProps)
{
	HWND	hCtrl;
	int	i;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		{
		MapProps->Selected[0][i] = 0;
		MapProps->Selected[1][i] = 0;
		}
			
	hCtrl = GetDlgItem(hDlg,100);
	InvalidateRect(hCtrl,NULL,FALSE);
	UpdateWindow(hCtrl);
	return;
}


// «»»» Sélection des boutons «««««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesButton(HWND hDlg, MAPPROPSCTX *MapProps)
{
	POINT	CursorPos;
	RECT	ButtonRect;
	HWND	hCtrl;
	UINT	Mode;
	UINT	SideSelected;
	UINT	ButtonSelected;
	BOOL	EraseBckgnd = FALSE;
	long	Height;
	long	StartY;
	int	i;

	hCtrl = GetDlgItem(hDlg,100);
	GetCursorPos(&CursorPos);
	GetWindowRect(hDlg,&ButtonRect);
	if (!PtInRect(&ButtonRect,CursorPos)) goto UnSelect;
	MapWindowPoints(NULL,hCtrl,&CursorPos,1);
	GetClientRect(hCtrl,&ButtonRect);

	Height = ButtonRect.bottom-ButtonRect.top;
	StartY = (Height%MAP_MAXPLAYERS)/2;
	Height = Height/MAP_MAXPLAYERS;

	if (CursorPos.y < StartY) goto UnSelect;
	if ((CursorPos.x > 100)&&(CursorPos.x < ButtonRect.right-100)) goto UnSelect;
	CursorPos.y -= StartY;
	if (CursorPos.y >= Height*MAP_MAXPLAYERS) goto UnSelect;
	CursorPos.y /= Height;
	if (CursorPos.x < 100) CursorPos.x = 0;
	else CursorPos.x = 1;

	Mode = SendDlgItemMessage(hDlg,101,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	switch(Mode)
		{
		case 0:	for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				MapProps->Selected[0][i] = 0;
				MapProps->Selected[1][i] = 0;
				}
			MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
			break;
		case 1:
		case 2:	SideSelected = 2;
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				if (MapProps->Selected[0][i])
					{
					SideSelected = 0;
					ButtonSelected = i;
					break;
					}
				if (MapProps->Selected[1][i])
					{
					SideSelected = 1;
					ButtonSelected = i;
					break;
					}
				}
			/* First button to be selected ? */
			if (SideSelected == 2)
				{
				MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
				break;
				}
			/* The same button have been selected ? */
			if ((CursorPos.x == SideSelected)&&(CursorPos.y == ButtonSelected))
				{
				MapProps->Selected[CursorPos.x][CursorPos.y] = 0;
				break;
				}
			/* The same player have been selected in different row ? */
			if (CursorPos.y == ButtonSelected)
				{
				MapProps->Selected[SideSelected][ButtonSelected] = 0;
				MessageBeep(MB_ICONHAND);
				break;
				}
			/* A different player have been selected in the same row ? */
			if (CursorPos.x == SideSelected)
				{
				MapProps->Selected[SideSelected][ButtonSelected] = 0;
				MapProps->Selected[CursorPos.x][CursorPos.y] = 1;
				break;
				}
			/* Set or clear alliance */
			MapProps->Selected[SideSelected][ButtonSelected] = 0;
			if (Mode == 1)
				{
				MapProps->Allies[ButtonSelected][CursorPos.y] = 1;
				MapProps->Allies[CursorPos.y][ButtonSelected] = 1;
				}
			else
				{
				MapProps->Allies[ButtonSelected][CursorPos.y] = 0;
				MapProps->Allies[CursorPos.y][ButtonSelected] = 0;
				}
			EraseBckgnd = TRUE;
			break;
		}

	InvalidateRect(hCtrl,NULL,EraseBckgnd);
	UpdateWindow(hCtrl);
	return;

//--- Désélectionne les boutons (cliqué hors du champ) ---

UnSelect:
	Properties_MapAlliancesHide(hDlg,MapProps);
	return;
}


// «»»» Affichage des alliances «««««««««««««««««««««««««««««««««««««««««»

void Properties_MapAlliancesDisplay(DRAWITEMSTRUCT *ItemStruct, MAPPROPSCTX *MapProps)
{
	RECT	DrawRect;
	RECT	EdgeRect;
	HPEN	DrawPenShadow;
	HPEN	DrawPenLight;
	HPEN	OldPen;
	UINT	Flags;
	long	Height;
	long	StartY;
	int	i,j;

	if (ItemStruct->CtlType != ODT_BUTTON) return;
	if (ItemStruct->CtlID != 100) return;
	if (!(ItemStruct->itemAction&ODA_DRAWENTIRE)) return;

	Height = ItemStruct->rcItem.bottom-ItemStruct->rcItem.top;
	StartY = (Height%MAP_MAXPLAYERS)/2;
	Height = Height/MAP_MAXPLAYERS;
	DrawRect.top = StartY+ItemStruct->rcItem.top;

	//--- Affichage des boutons des joueurs ---

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		{
		EdgeRect.top = DrawRect.top;
		EdgeRect.bottom = EdgeRect.top+Height;
		EdgeRect.left = ItemStruct->rcItem.left;
		EdgeRect.right = EdgeRect.left+100;
		if (MapProps->Selected[0][i]) Flags = BDR_SUNKENINNER|BDR_SUNKENOUTER;
		else Flags = BDR_RAISEDINNER|BDR_RAISEDOUTER;
		DrawEdge(ItemStruct->hDC,&EdgeRect,Flags,BF_RECT);
		EdgeRect.left = ItemStruct->rcItem.right-100;
		EdgeRect.right = EdgeRect.left+100;
		if (MapProps->Selected[1][i]) Flags = BDR_SUNKENINNER|BDR_SUNKENOUTER;
		else Flags = BDR_RAISEDINNER|BDR_RAISEDOUTER;
		DrawEdge(ItemStruct->hDC,&EdgeRect,Flags,BF_RECT);

		DrawRect.bottom = DrawRect.top+Height;
		DrawRect.left = ItemStruct->rcItem.left+8;
		DrawRect.right = DrawRect.left+100-16;
		DrawText(ItemStruct->hDC,Players[i].Name,-1,&DrawRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

		DrawRect.left = ItemStruct->rcItem.right-100+8;
		DrawRect.right = DrawRect.left+100-16;
		DrawText(ItemStruct->hDC,Players[i].Name,-1,&DrawRect,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

		DrawRect.top += Height;
		}

	//--- Affichage des alliances ---

	DrawPenShadow = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
	DrawPenLight = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHIGHLIGHT));
	if ((DrawPenShadow)&&(DrawPenLight))
		{
		OldPen = SelectObject(ItemStruct->hDC,DrawPenShadow);
		if (OldPen)
			{
			DrawRect.top = StartY+ItemStruct->rcItem.top;
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				EdgeRect.top = StartY+ItemStruct->rcItem.top;
				for (j= 0; j != MAP_MAXPLAYERS; j++, EdgeRect.top += Height)
					{
					if (i == j) continue;

					if ((MapProps->Selected[0][i])||(MapProps->Selected[1][j]))
						SelectObject(ItemStruct->hDC,DrawPenLight);
					else
						SelectObject(ItemStruct->hDC,DrawPenShadow);

					if ((MapProps->Allies[i][j])||(MapProps->Allies[j][i]))
						{
						MoveToEx(ItemStruct->hDC,ItemStruct->rcItem.left+102,DrawRect.top+Height/2,NULL);
						LineTo(ItemStruct->hDC,ItemStruct->rcItem.right-102,EdgeRect.top+Height/2);
						}

					SelectObject(ItemStruct->hDC,OldPen);
					}
				DrawRect.top += Height;
				}
			}
		}
	if (DrawPenShadow) DeleteObject(DrawPenShadow);
	if (DrawPenLight)  DeleteObject(DrawPenLight);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des créatures						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapCreatures(HWND hWnd, MAPPROPSCTX *MapProps)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6803),hWnd,Properties_MapCreaturesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapCreaturesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapCreaturesInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			((MEASUREITEMSTRUCT *)lParam)->itemWidth = 32+2+10+16;
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = 32+2;
			return(TRUE);

		case WM_DRAWITEM:
			Properties_MapPlayerDrawItem((DRAWITEMSTRUCT *)lParam,MapProps,4);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR FAR *) lParam)->code)
				{
				case LVN_GETDISPINFO:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapCreaturesDispInfo((LV_DISPINFO *)lParam,MapProps);
					return(TRUE);
				case LVN_ITEMCHANGED:
					if (((NMHDR *)lParam)->hwndFrom == MapProps->hCreatures)
						Properties_MapCreaturesChanged(hDlg,MapProps);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					if ((LOWORD(wParam) < 103)&&(LOWORD(wParam) > 109)) break;
					Properties_MapCreaturesSetParam(hDlg,LOWORD(wParam),MapProps);
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							MapProps->DoNotUpdate = FALSE;
							CopyMemory(&MapProps->MapProperties.CreaturesParams[0][0],&MapProps->CreaturesParams[0][0],MAP_VARCREATURES*64*sizeof(DWORD));
							SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							Misc_DestroyImageList(&MapProps->hImages);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							MapProps->DoNotUpdate = FALSE;
							SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
							Misc_DestroyImageList(&MapProps->hImages);
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

void Properties_MapCreaturesInit(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	LONG		X,Y;
	int		i;

	MapProps->DoNotUpdate = TRUE;
	CopyMemory(&MapProps->CreaturesParams[0][0],&MapProps->MapProperties.CreaturesParams[0][0],MAP_VARCREATURES*64*sizeof(DWORD));
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)MapProps);

	if (!Misc_CreateImageList(BadCreatures,&MapProps->hImages,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(BadElites,&MapProps->hImages,32,32,TRUE)) goto Error_0;
	if (!Misc_CreateImageList(Heros,&MapProps->hImages,32,32,TRUE)) goto Error_0;

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	MapProps->hCreatures = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_OWNERDRAWFIXED|LVS_SHAREIMAGELISTS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!MapProps->hCreatures) goto Error_1;
	SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)MapProps->hImages);

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = Font.FontMaxWidth*20;
	Column.pszText = szCfgColumn4;
	Column.iSubItem = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_2;
	Column.fmt = LVCFMT_CENTER;
	Column.cx = Font.FontMaxWidth*6;
	Column.pszText = szCfgColumn5;
	Column.iSubItem = 1;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)1,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn6;
	Column.iSubItem = 2;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)2,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn7;
	Column.iSubItem = 3;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)3,(LPARAM)&Column) == -1) goto Error_2;
	Column.pszText = szCfgColumn8;
	Column.iSubItem = 4;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTCOLUMN,(WPARAM)4,(LPARAM)&Column) == -1) goto Error_2;

	Item.mask = LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = szBadCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (X = 1, Y = 1, i = 0; BadCreatures[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadCreatures[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szBadCreaturesElite;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; BadElites[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = BadElites[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}
	Item.iItem = X++;
	Item.pszText = szGoodCreatures;
	Item.lParam = 0;
	if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
	for (i = 0; Heros[i].Id != 0; i++)
		{
		Item.iItem = X++;
		Item.pszText = Heros[i].Name;
		Item.lParam = (LPARAM)Y++;
		if (SendMessage(MapProps->hCreatures,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_2;
		}

	SetDlgItemInt(hDlg,103,0,FALSE);
	SetDlgItemInt(hDlg,105,0,FALSE);
	SetDlgItemInt(hDlg,107,0,FALSE);
	SetDlgItemInt(hDlg,109,0,FALSE);
	MapProps->DoNotUpdate = FALSE;
	return;

//--- Erreurs ---

Error_2:SendMessage(MapProps->hCreatures,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error_1:Misc_DestroyImageList(&MapProps->hImages);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Mise à jour de paramètres «««««««««««««««««««««««««««««««««««««««»

void Properties_MapCreaturesChanged(HWND hDlg, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	long	Params[4];
	long	Total[4];

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;

	Total[0] = 0;
	Total[1] = 0;
	Total[2] = 0;
	Total[3] = 0;
	Params[0] = 0;
	Params[1] = 0;
	Params[2] = 0;
	Params[3] = 0;

	while( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1 )
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		if ((Item.lParam >= 17)&&(Item.lParam <= 29)) continue;

		if (MapProps->CreaturesParams[Item.lParam-1][0])
			{
			Params[0] += MapProps->CreaturesParams[Item.lParam-1][0];
			Total[0]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][1])
			{
			Params[1] += MapProps->CreaturesParams[Item.lParam-1][1];
			Total[1]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][2])
			{
			Params[2] += MapProps->CreaturesParams[Item.lParam-1][2];
			Total[2]++;
			}
		if (MapProps->CreaturesParams[Item.lParam-1][3])
			{
			Params[3] += MapProps->CreaturesParams[Item.lParam-1][3];
			Total[3]++;
			}
		}

	MapProps->DoNotUpdate = TRUE;
	if (Total[0]) SetDlgItemInt(hDlg,103,Params[0]/Total[0],FALSE);
	else SetDlgItemInt(hDlg,103,0,FALSE);
	if (Total[1]) SetDlgItemInt(hDlg,105,Params[1]/Total[1],FALSE);
	else SetDlgItemInt(hDlg,105,0,FALSE);
	if (Total[2]) SetDlgItemInt(hDlg,107,Params[2]/Total[2],FALSE);
	else SetDlgItemInt(hDlg,107,0,FALSE);
	if (Total[3]) SetDlgItemInt(hDlg,109,Params[3]/Total[3],FALSE);
	else SetDlgItemInt(hDlg,109,0,FALSE);
	MapProps->DoNotUpdate = FALSE;
	return;
}


// «»»» Changement de paramètres ««««««««««««««««««««««««««««««««««««««««»

void Properties_MapCreaturesSetParam(HWND hDlg, UINT CtlID, MAPPROPSCTX *MapProps)
{
	LV_ITEM	Item;
	DWORD	NewParam;

	if (MapProps->DoNotUpdate) return;

	NewParam = GetDlgItemInt(hDlg,CtlID,NULL,FALSE);
	CtlID -= 103;
	CtlID /= 2;	

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iSubItem = 0;

	while( (Item.iItem = SendMessage(MapProps->hCreatures,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0))) != -1 )
		{
		if (!SendMessage(MapProps->hCreatures,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) continue;
		if (!Item.lParam) continue;
		// Les paramètres des créatures elites (Offset 17 à 29)
		// ne peuvent pas être modifiés.
		//
		if ((Item.lParam >= 17)&&(Item.lParam <= 29)) continue;

		MapProps->CreaturesParams[Item.lParam-1][CtlID] = NewParam;
		}

	InvalidateRect(MapProps->hCreatures,NULL,FALSE);
	UpdateWindow(MapProps->hCreatures);
	return;
}


// «»»» Affichage des informations concernant les créatures «««««««««««««»

void Properties_MapCreaturesDispInfo(LV_DISPINFO *DispInfo, MAPPROPSCTX *MapProps)
{
	LONG	Count;

	if (!(DispInfo->item.mask&LVIF_TEXT)) return;
	if (!(DispInfo->item.iSubItem)) return;
	if (!(DispInfo->item.lParam)) return;

	// The line below assumes that tables are in following order :
	//	Bad creatures, Elites, Heros
	//
	switch(DispInfo->item.iSubItem)
		{
		case 1:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][0];
			break;
		case 2:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][1];
			break;
		case 3:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][2];
			break;
		case 4:	Count = MapProps->CreaturesParams[DispInfo->item.lParam-1][3];
			break;
		default:return;
		}

	if (Count)
		wsprintf(DispInfo->item.pszText,szNumber,Count);
	else
		{
		if ((DispInfo->item.lParam >= 17)&&(DispInfo->item.lParam <= 29))
			strcpy(DispInfo->item.pszText,szCfgNull);
		else
			strcpy(DispInfo->item.pszText,szCfgNA);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres audio							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la liste des fichiers son «««««««««««««««««««««««««««»

int Properties_MapSoundBuildList(NODE *SoundList)
{
	LPSHELLFOLDER	 Desktop;
	LPSHELLFOLDER	 Folder;
	LPENUMIDLIST	 Directory;
	LPITEMIDLIST	 Object;
	LPCITEMIDLIST	 ObjectList;
	LPMALLOC	 Malloc;
	STRRET		 Name;
	ULONG		 Unused = 0;
	ULONG		 ObjectAttributes = 0;
	UINT		 Result;
	MAPPROPSSOUND	*NewSound;
	char		*SoundPath;
	char		*NameBuffer;
	char		*NameSlash;
	wchar_t		*DisplayNameBuffer;
	int		 NameLen;

	SoundPath = GlobalAlloc(GPTR,MAX_PATH);
	if (!SoundPath) goto Error_0;
	DisplayNameBuffer = GlobalAlloc(GPTR,MAX_PATH*2);
	if (!DisplayNameBuffer) goto Error_1;

	CopyMemory(SoundPath,Config.GamePath,MAX_PATH);
	strcat(SoundPath,szSoundsPath);

	//--- Retrieve the path in the shellnamespace ---

	SHGetMalloc(&Malloc);
	SHGetDesktopFolder(&Desktop);
	mbstowcs(DisplayNameBuffer,SoundPath,strlen(SoundPath));
	Result = Desktop->lpVtbl->ParseDisplayName(Desktop,NULL,NULL,DisplayNameBuffer,&Unused,&Object,&ObjectAttributes);
	if (Result != NOERROR) goto Error_2;
	Result = Desktop->lpVtbl->BindToObject(Desktop,Object,0,&IID_IShellFolder,&(LPVOID)Folder);
	if (Result != NOERROR) goto Error_2;
	Desktop->lpVtbl->Release(Desktop);
	Desktop = Folder;

	//--- Enumerate objects in the list (= obtain the file list, he!) ---

	Result = Desktop->lpVtbl->EnumObjects(Desktop,NULL,SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN,&Directory);
	if (Result != NOERROR) goto Error_2;
	Directory->lpVtbl->Reset(Directory);

	for (;;)
		{
		Result = Directory->lpVtbl->Next(Directory,1,&Object,NULL);
		if (Result == S_FALSE) break;
		if (Result != NOERROR) goto Error_4;

		ObjectList = Object;
		ObjectAttributes = 0;
		Desktop->lpVtbl->GetAttributesOf(Desktop,1,&ObjectList,&ObjectAttributes);
		Desktop->lpVtbl->GetDisplayNameOf(Desktop,Object,SHGDN_FORPARSING,&Name);

		switch(Name.uType)
			{
			case STRRET_WSTR:
				NameLen = WideCharToMultiByte(CP_OEMCP,WC_DEFAULTCHAR,Name.DUMMYUNIONNAME.pOleStr,-1,NULL,0,NULL,NULL);
				NameBuffer = (char *)Malloc->lpVtbl->Alloc(Malloc,NameLen);
				if (!NameBuffer) goto Error_4;
				WideCharToMultiByte(CP_OEMCP,WC_DEFAULTCHAR,Name.DUMMYUNIONNAME.pOleStr,-1,NameBuffer,NameLen,NULL,NULL);
				strcpy(SoundPath,NameBuffer);
				Malloc->lpVtbl->Free(Malloc,NameBuffer);
				break;
			case STRRET_OFFSET:
				strcpy(SoundPath,((char *)Object)+(Name.DUMMYUNIONNAME.uOffset));
				break;
			case STRRET_CSTR:
				strcpy(SoundPath,Name.DUMMYUNIONNAME.cStr);
				break;
			}

		if (ObjectAttributes & SFGAO_FOLDER)
			{
			NewSound = GlobalAlloc(GPTR,sizeof(MAPPROPSSOUND));
			if (!NewSound) goto Error_4;
			NameSlash = strrchr(SoundPath,'\\');
			if (NameSlash) strncpy(NewSound->name,++NameSlash,64);
			else strncpy(NewSound->name,SoundPath,64);
			CharUpperBuff(NewSound->name,strlen(NewSound->name));
			List_AddEntry((NODE *)NewSound,SoundList);
			}
		}

	//--- Oh Dear! It is done and ok ---

	Directory->lpVtbl->Release(Directory);
	Desktop->lpVtbl->Release(Desktop);
	Malloc->lpVtbl->Release(Malloc);
	GlobalFree(DisplayNameBuffer);
	GlobalFree(SoundPath);
	return(1);

//--- Erreurs ---

Error_4:List_ReleaseMemory(SoundList);
	Directory->lpVtbl->Release(Directory);
Error_2:Desktop->lpVtbl->Release(Desktop);
	Malloc->lpVtbl->Release(Malloc);
	GlobalFree(DisplayNameBuffer);
Error_1:GlobalFree(SoundPath);
Error_0:return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
