
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
extern HMENU		hMenu;
extern NODE		UndoList;
extern MAP		Map;
extern MAPEDITSTATS	MapStats;
extern MAPOPTIONS	MapOptions[];
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapMagicalObjects;
extern NODE		MapObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern LONG		MapChanges;
extern UINT		SelectLast;
extern ULONG		InfoMemoryUsedByUndo;
extern GLOBALVARS	MapGlobalVariables;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Restauration des changements					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Annule la dernière opération effectuée ««««««««««««««««««««««««««»

void Undo_Restore()
{
	UNDO	*Undo;

	if (!UndoList.next) return;
	for (Undo = (UNDO *)UndoList.next; Undo->node.next != 0; Undo = (UNDO *)Undo->node.next);
	Map_Lock(TRUE,FALSE);
	Undo_RestoreNode(Undo);
	Map_Lock(FALSE,TRUE);
	Undo_Update();
	Files_SaveUpdate();
	Minimap_Redraw(hWnd);
	Thumb_UpdateWindow();
	MapStats.UsageOfUndo++;
	return;
}


// «»»» Annule une série d'opérations «««««««««««««««««««««««««««««««««««»

void Undo_RestoreNode(UNDO *Undo)
{
	ACTION	*Action;
	NODE	*Childs;
	BYTE	*MapPtr;
	BYTE	*LinePtr;
	BYTE	*UndoPtr;
	int	 i,j;

	while(Undo->childs.next)
		{
		for (Childs = Undo->childs.next; Childs->next != 0; Childs = Childs->next);
		Undo_RestoreNode((UNDO *)Childs);
		}

	switch(Undo->node.type)
		{
		//--- La cible de certaines actions avait été modifiée ---
		case UNDO_ACTIONRESIZED:
			Action = Triggers_GetActionById(Undo->data.action);
			if (!Action) break;
			switch(Action->ctrl)
				{
				case ACTION_ADDCREATURE:
				case ACTION_CHANGESQUARE:
				case ACTION_CHANGEOWNER:
					Action->param5 -= Undo->mod.dx;
					Action->param7 -= Undo->ext.dy;
					break;
				}
			break;

		//--- Les chemins avaient été modifiés ---
		case UNDO_PATHPOINT:
			Undo->data.area->nextid = Undo->mod.savedid;
			SelectLast = Undo->ext.currentid;
			break;

		//--- Les propriétés de la carte avaient été modifiées ---

		case UNDO_MAPPARAMS:
			CopyMemory(&MapOptions[0],Undo->data.mapoptions,sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);
			CopyMemory(&Map.Properties,&Undo->mod.map->Properties,sizeof(MAPPROPERTIES));
			break;

		case UNDO_GLOBALVARIABLES:
			CopyMemory(&MapGlobalVariables,Undo->data.variables,sizeof(GLOBALVARS));
			break;

		//--- Les dimensions de la carte avaient été modifiées ---

		case UNDO_MAPRESIZE:
			Map_Destroy(&Map);
			CopyMemory(&Map,Undo->mod.map,sizeof(MAP));
			GlobalFree(Undo->mod.map);
			Undo->mod.map = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAP)+Undo->width*Undo->height*MAP_CONTROLBYTES;
			Map_CorrectAfterResize();
			Mouse_MovedUpdate(hWnd);
			Map_SetPropThumb(hWnd,PROP_VERTICAL);
			Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
			Map_CorrectPos(hWnd);
			Minimap_Build(hWnd);
			Info_Update();
			break;

		//--- La carte avait été modifiée ---

		case UNDO_FULLMAP:
			CopyMemory(Map.Map,Undo->data.map,Map.Properties.Width*MAP_CONTROLBYTES*Map.Properties.Height);
			break;
		case UNDO_MAPRECT:
			MapPtr = Map.Map+Undo->x*MAP_CONTROLBYTES+Undo->y*Map.Properties.Width*MAP_CONTROLBYTES;
			UndoPtr = Undo->data.map;
			for (i = 0; i != Undo->height; i++)
				{
				LinePtr = MapPtr;
				for (j = 0; j != Undo->width*MAP_CONTROLBYTES; j++)
					*LinePtr++ = *UndoPtr++;
				MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
				}
			break;

		//--- Un élément avait été déplacé ---

		case UNDO_MOVETHING:
			List_RemEntry(Undo->data.entry);
			List_AddEntry(Undo->data.entry,Undo->mod.list);
			break;

		//--- Un élément avait été ajoutée ---

		case UNDO_ADDCREATURE:
		case UNDO_ADDTRAP:
		case UNDO_ADDDOOR:
		case UNDO_ADDMAGIC:
		case UNDO_ADDOBJECT:
		case UNDO_ADDBAND:
		case UNDO_ADDAREA:
		case UNDO_ADDGATE:
		case UNDO_ADDEFFECT:
			Find_UpdateList(Undo->data.entry);
			List_RemEntry(Undo->data.entry);
			GlobalFree(Undo->data.entry);
			break;

		//--- Un élément avait été supprimé ---

		case UNDO_SUBCREATURE:
			List_AddEntry(Undo->data.entry,&MapCreatures);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPTHING);
			break;
		case UNDO_SUBDOOR:
			List_AddEntry(Undo->data.entry,&MapDoors);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPTHING);
			break;
		case UNDO_SUBTRAP:
			List_AddEntry(Undo->data.entry,&MapTraps);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPTHING);
			break;
		case UNDO_SUBMAGIC:
			List_AddEntry(Undo->data.entry,&MapMagicalObjects);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPTHING);
			break;
		case UNDO_SUBOBJECT:
			List_AddEntry(Undo->data.entry,&MapObjects);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPTHING);
			break;
		case UNDO_SUBBAND:
			List_AddEntry(Undo->data.entry,&MapHerosBands);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MTHBAND);
			break;
		case UNDO_SUBAREA:
			List_AddEntry(Undo->data.entry,&MapAreas);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPRECTEX);
			break;
		case UNDO_SUBGATE:
			List_AddEntry(Undo->data.entry,&MapGates);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPRECTEX);
			break;
		case UNDO_SUBEFFECT:
			List_AddEntry(Undo->data.entry,&MapEffects);
			Undo->data.entry = NULL;
			InfoMemoryUsedByUndo -= sizeof(MAPRECTEX);
			break;

		//--- Un élément avait été modifié ---

		case UNDO_MODCREATURE:
		case UNDO_MODTRAP:
		case UNDO_MODDOOR:
		case UNDO_MODMAGIC:
		case UNDO_MODOBJECT:
			if (!Undo->mod.thing) break;
			CopyMemory(((BYTE *)Undo->data.entry)+sizeof(NODE),((BYTE *)Undo->mod.thing)+sizeof(NODE),sizeof(MAPTHING)-sizeof(NODE));
			((MAPTHING *)Undo->data.entry)->node.type = Undo->mod.thing->node.type;
			break;
		case UNDO_MODBAND:
			if (!Undo->mod.thing) break;
			CopyMemory(((BYTE *)Undo->data.entry)+sizeof(NODE),((BYTE *)Undo->mod.band)+sizeof(NODE),sizeof(MTHBAND)-sizeof(NODE));
			break;
		case UNDO_MODAREA:
		case UNDO_MODGATE:
		case UNDO_MODEFFECT:
			if (!Undo->mod.thing) break;
			CopyMemory(((BYTE *)Undo->data.entry)+sizeof(NODE),((BYTE *)Undo->mod.entry)+sizeof(NODE),sizeof(MAPRECTEX)-sizeof(NODE));
			break;
		}

	MapChanges = Undo->changes;
	Undo_DestroyEntry(Undo);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des entrées						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Entrée d'une liste ««««««««««««««««««««««««««««««««««««««««««««««»

UNDO* Undo_CreateEntry(NODE *Entry, UINT Type)
{
	UNDO	*Undo;
	
	Undo = Undo_CreateNewEntry(Type,NULL);
	if (!Undo_StoreEntry(Undo,Entry,Type))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		return(NULL);
		}

	return(Undo);
}


// «»»» Changement des variables globales «««««««««««««««««««««««««««««««»

UNDO* Undo_CreateGlobalVariables(GLOBALVARS *Variables)
{
	UNDO	*Undo;

	Undo = Undo_CreateNewEntry(UNDO_GLOBALVARIABLES,szMapGlobalVariables);
	if (!Undo) return(NULL);

	Undo->data.variables = GlobalAlloc(GPTR,sizeof(GLOBALVARS));
	if (!Undo->data.variables)
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		return(NULL);
		}
	InfoMemoryUsedByUndo += sizeof(GLOBALVARS);
	CopyMemory(Undo->data.variables,Variables,sizeof(GLOBALVARS));
	return(Undo);
}


// «»»» Dessin rapide d'un chemin «««««««««««««««««««««««««««««««««««««««»

UNDO* Undo_CreatePathPoint(MAPRECTEX *Area, UINT LastPath)
{
	UNDO	*Undo;

	Undo = Undo_CreateNewEntry(UNDO_PATHPOINT,szMapPathPoint);
	if (!Undo) return(NULL);

	Undo->data.area = Area;
	Undo->mod.savedid = Area->nextid;
	Undo->ext.currentid = LastPath;
	return(Undo);
}


// «»»» Rectangle dans la carte «««««««««««««««««««««««««««««««««««««««««»

UNDO* Undo_CreateMapRect(char *Description, MAPRECT *MapRect, MAP *Map)
{
	UNDO	*Undo;

	Undo = Undo_CreateNewEntry(UNDO_MAPRECT,Description);
	if (!Undo) return(NULL);
	if (!Undo_StoreMapRect(Undo,MapRect,Map))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		return(NULL);
		}

	return(Undo);
}


// «»»» Carte entière «««««««««««««««««««««««««««««««««««««««««««««««««««»

UNDO* Undo_CreateFullMap(char *Description, MAP *Map)
{
	UNDO	*Undo;

	Undo = Undo_CreateNewEntry(UNDO_FULLMAP,Description);
	if (!Undo) return(NULL);
	if (!Undo_StoreFullMap(Undo,Map))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		return(NULL);
		}

	return(Undo);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Stockage des changements						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Stockage d'un changement de liste «««««««««««««««««««««««««««««««»

int Undo_StoreMoveThing(UNDO *Undo, MAPTHING *Thing)
{
	if (!Undo) return(1);

	Undo->node.type = UNDO_MOVETHING;
	Undo->description = szMapMoveThing;
	Undo->data.entry = (NODE *)Thing;

	switch(Thing->node.type)
		{
		case THING_CREATURE:
			Undo->mod.list = &MapCreatures;
			break;
		case THING_DOOR:
			Undo->mod.list = &MapDoors;
			break;
		case THING_TRAP:
			Undo->mod.list = &MapTraps;
			break;
		case THING_OBJECT:
			Undo->mod.list = &MapObjects;
			break;
		case THING_MAGICALOBJECT:
			Undo->mod.list = &MapMagicalObjects;
			break;
		}

	List_RemEntry((NODE *)Thing);
	return(1);
}


// «»»» Stockage des propriétés de la carte «««««««««««««««««««««««««««««»

int Undo_StoreMapParams(UNDO *Undo, MAP *Map, MAPOPTIONS *MapOptions)
{
	if (!Undo) return(1);

	Undo->data.mapoptions = GlobalAlloc(GPTR,sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);
	if (!Undo->data.mapoptions) return(0);
	InfoMemoryUsedByUndo += sizeof(MAPOPTIONS)*MAP_MAXPLAYERS;
	Undo->mod.map = GlobalAlloc(GPTR,sizeof(MAP));
	if (!Undo->mod.map) return(0);
	InfoMemoryUsedByUndo += sizeof(MAP);

	CopyMemory(Undo->data.mapoptions,MapOptions,sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);
	CopyMemory(Undo->mod.map,Map,sizeof(MAP));
	return(1);
}


// «»»» Stockage de l'entrée d'une liste ««««««««««««««««««««««««««««««««»

int Undo_StoreEntry(UNDO *Undo, NODE *Entry, UINT Type)
{
	char	*Description;
	ULONG	 Size = 0;

	switch(Type)
		{
		case UNDO_ADDCREATURE:
			Description = szMapAddCreature;
			break;
		case UNDO_SUBCREATURE:
			Description = szMapSubCreature;
			Size = sizeof(MAPTHING);
			break;
		case UNDO_MODCREATURE:
			Description = szMapModCreature;
			break;
		case UNDO_ADDDOOR:
			Description = szMapAddDoor;
			break;
		case UNDO_SUBDOOR:
			Description = szMapSubDoor;
			Size = sizeof(MAPTHING);
			break;
		case UNDO_MODDOOR:
			Description = szMapModDoor;
			break;
		case UNDO_ADDTRAP:
			Description = szMapAddTrap;
			break;
		case UNDO_SUBTRAP:
			Description = szMapSubTrap;
			Size = sizeof(MAPTHING);
			break;
		case UNDO_MODTRAP:
			Description = szMapModTrap;
			break;
		case UNDO_ADDMAGIC:
			Description = szMapAddMagicalObject;
			break;
		case UNDO_SUBMAGIC:
			Description = szMapSubMagicalObject;
			Size = sizeof(MAPTHING);
			break;
		case UNDO_MODMAGIC:
			Description = szMapModMagicalObject;
			break;
		case UNDO_ADDOBJECT:
			Description = szMapAddObject;
			break;
		case UNDO_SUBOBJECT:
			Description = szMapSubObject;
			Size = sizeof(MAPTHING);
			break;
		case UNDO_MODOBJECT:
			Description = szMapModObject;
			break;
		case UNDO_ADDBAND:
			Description = szMapAddHerosBand;
			break;
		case UNDO_SUBBAND:
			Description = szMapSubHerosBand;
			Size = sizeof(MTHBAND);
			break;
		case UNDO_MODBAND:
			Description = szMapModHerosBand;
			break;
		case UNDO_ADDAREA:
			Description = szMapAddArea;
			break;
		case UNDO_SUBAREA:
			Description = szMapSubArea;
			Size = sizeof(MAPRECTEX);
			break;
		case UNDO_MODAREA:
			Description = szMapModArea;
			break;
		case UNDO_ADDGATE:
			Description = szMapAddGate;
			break;
		case UNDO_SUBGATE:
			Description = szMapSubGate;
			Size = sizeof(MAPRECTEX);
			break;
		case UNDO_MODGATE:
			Description = szMapModGate;
			break;
		case UNDO_ADDEFFECT:
			Description = szMapAddEffect;
			break;
		case UNDO_SUBEFFECT:
			Description = szMapSubEffect;
			Size = sizeof(MAPRECTEX);
			break;
		case UNDO_MODEFFECT:
			Description = szMapModEffect;
			break;
		}

	//--- Supprime l'entrée de la liste si c'est une suppression ----

	switch(Type)
		{
		case UNDO_SUBCREATURE:
		case UNDO_SUBDOOR:
		case UNDO_SUBTRAP:
		case UNDO_SUBMAGIC:
		case UNDO_SUBOBJECT:
		case UNDO_SUBBAND:
		case UNDO_SUBAREA:
		case UNDO_SUBGATE:
		case UNDO_SUBEFFECT:
			Find_UpdateList(Entry);
			List_RemEntry(Entry);
			break;
		}

	//--- Stock les données d'annulation ---

	if (Undo)
		{
		Undo->node.type = Type;
		Undo->description = Description;
		Undo->data.entry = Entry;
		InfoMemoryUsedByUndo += Size;

		switch(Type)
			{
			case UNDO_MODCREATURE:
			case UNDO_MODDOOR:
			case UNDO_MODTRAP:
			case UNDO_MODMAGIC:
			case UNDO_MODOBJECT:
				Undo->mod.thing = GlobalAlloc(GPTR,sizeof(MAPTHING));
				if (!Undo->mod.thing) return(0);
				InfoMemoryUsedByUndo += sizeof(MAPTHING);
				CopyMemory(Undo->mod.thing,Entry,sizeof(MAPTHING));
				Find_UpdateList(Entry);
				break;
			case UNDO_MODBAND:
				Undo->mod.band = GlobalAlloc(GPTR,sizeof(MTHBAND));
				if (!Undo->mod.band) return(0);
				InfoMemoryUsedByUndo += sizeof(MTHBAND);
				CopyMemory(Undo->mod.band,Entry,sizeof(MTHBAND));
				break;
			case UNDO_MODGATE:
			case UNDO_MODAREA:
			case UNDO_MODEFFECT:
				Undo->mod.entry = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
				if (!Undo->mod.entry) return(0);
				InfoMemoryUsedByUndo += sizeof(MAPRECTEX);
				CopyMemory(Undo->mod.entry,Entry,sizeof(MAPRECTEX));
				Find_UpdateList(Entry);
				break;
			}
		}
	//--- Supprime la mémoire utilisée si l'entrée n'est pas stockée ---
	else switch(Type)
		{
		case UNDO_SUBCREATURE:
		case UNDO_SUBDOOR:
		case UNDO_SUBTRAP:
		case UNDO_SUBMAGIC:
		case UNDO_SUBOBJECT:
		case UNDO_SUBBAND:
		case UNDO_SUBAREA:
		case UNDO_SUBGATE:
		case UNDO_SUBEFFECT:
			GlobalFree(Entry);
			break;
		}

	return(1);
}


// «»»» Stockage d'une partie de la carte «««««««««««««««««««««««««««««««»

int Undo_StoreMapRect(UNDO *Undo, MAPRECT *MapRect, MAP *Map)
{
	MAPCLIENT	 MapClient;
	BYTE		*UndoPtr;
	BYTE		*MapPtr;
	BYTE		*LinePtr;
	long		 i,j;

	if (!Undo) return(1);

	Map_CorrectRect(MapRect,&MapClient);
	Undo->x = MapClient.X;
	Undo->y = MapClient.Y;
	Undo->width = MapClient.Width;
	Undo->height = MapClient.Height;
	if (Undo->width+Undo->x > Map->Properties.Width) Undo->width = Map->Properties.Width-Undo->x;
	if (Undo->height+Undo->y > Map->Properties.Height) Undo->height = Map->Properties.Height-Undo->y;

	Undo->data.map = GlobalAlloc(GPTR,Undo->width*Undo->height*MAP_CONTROLBYTES);
	if (!Undo->data.map) return(0);
	InfoMemoryUsedByUndo += Undo->width*Undo->height*MAP_CONTROLBYTES;

	MapPtr = Map->Map+Undo->x*MAP_CONTROLBYTES+Undo->y*Map->Properties.Width*MAP_CONTROLBYTES;
	UndoPtr = Undo->data.map;
	for (i = 0; i != Undo->height; i++)
		{
		LinePtr = MapPtr;
		for (j = 0; j != Undo->width*MAP_CONTROLBYTES; j++)
			*UndoPtr++ = *LinePtr++;
		MapPtr += Map->Properties.Width*MAP_CONTROLBYTES;
		}

	return(1);
}


// «»»» Stockage de la totalité de la carte «««««««««««««««««««««««««««««»

int Undo_StoreFullMap(UNDO *Undo, MAP *Map)
{
	if (!Undo) return(1);

	Undo->data.map = GlobalAlloc(GMEM_FIXED,Map->Properties.Width*MAP_CONTROLBYTES*Map->Properties.Height);
	if (!Undo->data.map) return(0);

	Undo->width = Map->Properties.Width;
	Undo->height = Map->Properties.Height;
	CopyMemory(Undo->data.map,Map->Map,Map->Properties.Width*Map->Properties.Height*MAP_CONTROLBYTES);
	InfoMemoryUsedByUndo += Map->Properties.Width*Map->Properties.Height*MAP_CONTROLBYTES;
	return(1);
}


// «»»» Stockage d'un changement complet de la carte ««««««««««««««««««««»

int Undo_StoreFullMapChanges(UNDO *Undo, MAP *Map)
{
	if (Undo)
		{
		Undo->mod.map = GlobalAlloc(GMEM_FIXED,sizeof(MAP));
		if (Undo->mod.map)
			{
			Undo->width = Map->Properties.Width;
			Undo->height = Map->Properties.Height;
			CopyMemory(Undo->mod.map,Map,sizeof(MAP));
			InfoMemoryUsedByUndo += sizeof(MAP)+Map->Properties.Width*Map->Properties.Height*MAP_CONTROLBYTES;
			return(1);
			}
		else
			{
			Map_Destroy(Map);
			return(0);
			}
		}

	Map_Destroy(Map);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modifications							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mise à jour des données relatives à l'annulation ««««««««««««««««»

void Undo_Update()
{
	UINT	Enabled;

	Enabled = MF_BYCOMMAND|((List_EntryCount(&UndoList))?MF_ENABLED:MF_GRAYED);
	EnableMenuItem(hMenu,IDM_UNDOLAST,Enabled);
	EnableMenuItem(hMenu,IDM_UNDOHISTORY,Enabled);
	EnableMenuItem(hMenu,IDM_UNDOFREE,Enabled);
	Info_Update();
	return;
}


// «»»» Change le nombre d'opération mémorisées «««««««««««««««««««««««««»

void Undo_ChangeLevels()
{
	MESSAGEBOXSTRUCT	MessageBox;
	int			Result;

	ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
	MessageBox.gadgets = szUndoLevelsChoices;
	MessageBox.windowtext = szUndoLevels;
	MessageBox.windowtitle = (char *)-1;
	MessageBox.editnumber = Config.UndoLevels;
	MessageBox.flags = MBF_ESNUMBER|MBF_NUMBER|MBF_EDITTEXT|MBF_NOBEEP;
	MessageBox.defaulticon = MB_ICONQUESTION;
	MessageBox.min = 0;
	MessageBox.max = 65535;
	MessageBox.windowpos = MBP_FOLLOWMOUSE;
	Result = Misc_MessageBoxExtArgs(hWnd,&MessageBox);

	switch(Result)
		{
		case 1:	Config.UndoLevels = MessageBox.editnumber;
			break;
		case 2:	Config.UndoLevels = UNDO_LEVELS;
			break;
		}

	if (!Config.UndoLevels)
		Undo_DestroyAllEntries();

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Historique de l'annulation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Undo_History(HWND hWnd)
{
	int	Result;

	Result = DialogBox(hInstance,MAKEINTRESOURCE(6200),hWnd,Undo_HistoryProc);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	Undo_Update();
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

#pragma argsused
BOOL CALLBACK Undo_HistoryProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	static HWND	hList;

	switch(uMsgId)
		{
		case WM_INITDIALOG: {
			RECT	ClientRect;
			long	X,Y;

			Window_UnderCursor(hDlg,&X,&Y);
			SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER);

			GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
			MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

			hList = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
			if (!hList) {EndDialog(hDlg,-1); return(TRUE);}
			if (!Undo_HistoryBuildList(hList,UndoList.next,TVI_ROOT)) {EndDialog(hDlg,-1); return(TRUE);}

			SetDlgItemInt(hDlg,102,List_EntryCount(&UndoList),FALSE);
			SetDlgItemInt(hDlg,104,MapChanges,FALSE);
			SetDlgItemInt(hDlg,106,InfoMemoryUsedByUndo,FALSE);

			} return(TRUE);

		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case IDOK:
					EndDialog(hDlg,1);
					return(TRUE);
				case IDCANCEL:
					EndDialog(hDlg,1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Création de la liste ««««««««««««««««««««««««««««««««««««««««««««»

int Undo_HistoryBuildList(HWND hList, NODE *List, HTREEITEM hParent)
{
	TV_INSERTSTRUCT	 Item;
	HTREEITEM	 hItem;
	UNDO		*Undo;

	Item.hInsertAfter = TVI_LAST;
	Item.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_CHILDREN;

	for (Undo = (UNDO *)List; Undo != 0; Undo = (UNDO *)Undo->node.next)
		{
		Item.hParent = hParent;
		Item.DUMMYUNIONNAME.item.pszText = Undo->description;
		Item.DUMMYUNIONNAME.item.cChildren = (Undo->childs.next)?1:0;
		Item.DUMMYUNIONNAME.item.lParam = (LPARAM)Undo;
		if ((hItem = (HTREEITEM)SendMessage(hList,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item)) == NULL) return(0);
		if ((Undo->childs.next)&&(!Undo_HistoryBuildList(hList,Undo->childs.next,hItem))) return(0);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'une requête en cas d'erreur «««««««««««««««««««««««««»

void Undo_PrintError(HWND hWnd)
{
	MESSAGEBOXSTRUCT	 MessageBox;
	char			 TempText[256];
	char			*ErrorMsg;

	strcpy(TempText,szUndoErr);
	ErrorMsg = strchr(TempText,0);
	*ErrorMsg++ = 0x0A;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),LOCALE_USER_DEFAULT,ErrorMsg,256-strlen(TempText),NULL);

	ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
	MessageBox.gadgets = szMessageBoxExtOk;
	MessageBox.windowtext = TempText;
	MessageBox.windowtitle = NULL;
	MessageBox.requestbits = &Config.RequestsFlags;
	MessageBox.statebit = REQUEST_UNDOERROR;
	MessageBox.flags = MBF_AUTOREPLY;
	MessageBox.defaultgadget = 1;
	MessageBox.defaulticon = MB_ICONHAND;
	MessageBox.windowpos = MBP_FOLLOWMOUSE;
	Misc_MessageBoxExtArgs(hWnd,&MessageBox);
	return;
}


// «»»» Conversion d'un mode de dessin en texte «««««««««««««««««««««««««»

char* Undo_PaintModeToText(DWORD PaintMode)
{
	switch(PaintMode)
		{
		case PAINTMODE_RECT:
			return(szMainMenuRect);
		case PAINTMODE_RECTRND:
			return(szMainMenuRectRnd);
		case PAINTMODE_RECTFILL:
			return(szMainMenuRectFill);
		case PAINTMODE_RECTFILLRND:
			return(szMainMenuRectFillRnd);
		case PAINTMODE_CIRCLE:
			return(szMainMenuCircle);
		case PAINTMODE_CIRCLERND:
			return(szMainMenuCircleRnd);
		case PAINTMODE_CIRCLEFILL:
			return(szMainMenuCircleFill);
		case PAINTMODE_CIRCLEFILLRND:
			return(szMainMenuCircleFillRnd);
		case PAINTMODE_LINE:
			return(szMainMenuLine);
		case PAINTMODE_LINERND:
			return(szMainMenuLineRnd);
		}

	return(NULL);
}


// «»»» Création d'une nouvelle structure «««««««««««««««««««««««««««««««»

UNDO* Undo_CreateNewEntry(UINT Type, char *Description)
{
	UNDO	*Undo = NULL;

	if (Config.UndoLevels)
		{
		if (List_EntryCount(&UndoList) >= Config.UndoLevels)
			Undo_DestroyEntry((UNDO *)UndoList.next);

		Undo = GlobalAlloc(GPTR,sizeof(UNDO));
		if (Undo)
			{
			Undo->node.type = Type;
			Undo->changes = MapChanges;
			Undo->description = Description;
			List_AddEntry((NODE *)Undo,&UndoList);
			InfoMemoryUsedByUndo += sizeof(UNDO);
			Undo_Update();
			}
		else
			Undo_PrintError(hWnd);
		}

	return(Undo);
}


// «»»» Création d'une nouvelle structure annexe ««««««««««««««««««««««««»

UNDO* Undo_CreateNewChild(UNDO **Parent, BOOL DestroyParentOnError, UINT Type, char *Description)
{
	UNDO	*Undo;

	if (!*Parent) return(NULL);

	Undo = GlobalAlloc(GPTR,sizeof(UNDO));
	if (Undo)
		{
		Undo->node.type = Type;
		Undo->changes = MapChanges;
		Undo->description = Description;
		List_AddEntry((NODE *)Undo,&((*Parent)->childs));
		InfoMemoryUsedByUndo += sizeof(UNDO);
		}
	else
		{
		Undo_PrintError(hWnd);
		if (DestroyParentOnError)
			{
			Undo_DestroyEntry(*Parent);
			Undo_Update();
			*Parent = NULL;
			}
		}

	return(Undo);
}


// «»»» Supprime toutes les structures ««««««««««««««««««««««««««««««««««»

void Undo_DestroyAllEntries()
{
	NODE	*List;

	while (UndoList.next)
		{
		for (List = UndoList.next; List->next != 0; List = List->next);
		Undo_DestroyEntry((UNDO *)List);
		}

	UndoList.next = NULL;
	UndoList.prev = NULL;
	Undo_Update();
	return;
}


// «»»» Supprime une structure ««««««««««««««««««««««««««««««««««««««««««»

void Undo_DestroyEntry(UNDO *Undo)
{
	NODE	*Childs;

	if (!Undo) return;

	List_RemEntry((NODE *)Undo);
	while(Undo->childs.next)
		{
		for (Childs = Undo->childs.next; Childs->next != 0; Childs = Childs->next);
		Undo_DestroyEntry((UNDO *)Childs);
		}

	switch(Undo->node.type)
		{
		case UNDO_GLOBALVARIABLES:
			if (Undo->data.variables)
				{
				GlobalFree(Undo->data.variables);
				InfoMemoryUsedByUndo -= sizeof(GLOBALVARS);
				}
			break;

		case UNDO_MAPPARAMS:
			if (Undo->data.mapoptions)
				{
				GlobalFree(Undo->data.mapoptions);
				InfoMemoryUsedByUndo -= sizeof(MAPOPTIONS)*MAP_MAXPLAYERS;
				}
			if (Undo->mod.map)
				{
				GlobalFree(Undo->mod.map);
				InfoMemoryUsedByUndo -= sizeof(MAP);
				}
			break;

		case UNDO_MAPRESIZE:
			if (Undo->mod.map)
				{
				Map_Destroy(Undo->mod.map);
				GlobalFree(Undo->mod.map);
				InfoMemoryUsedByUndo -= sizeof(MAP)+Undo->width*Undo->height*MAP_CONTROLBYTES;
				}
			break;

		case UNDO_MAPRECT:
		case UNDO_FULLMAP:
			if (Undo->data.map)
				{
				GlobalFree(Undo->data.map);
				InfoMemoryUsedByUndo -= Undo->width*Undo->height*MAP_CONTROLBYTES;
				}
			break;

		case UNDO_SUBCREATURE:
		case UNDO_SUBDOOR:
		case UNDO_SUBTRAP:
		case UNDO_SUBMAGIC:
		case UNDO_SUBOBJECT:
			if (Undo->data.entry)
				{
				InfoMemoryUsedByUndo -= sizeof(MAPTHING);
				GlobalFree(Undo->data.entry);
				}
			break;

		case UNDO_MODCREATURE:
		case UNDO_MODDOOR:
		case UNDO_MODTRAP:
		case UNDO_MODMAGIC:
		case UNDO_MODOBJECT:
			if (Undo->mod.thing)
				{
				InfoMemoryUsedByUndo -= sizeof(MAPTHING);
				GlobalFree(Undo->mod.thing);
				}
			break;

		case UNDO_SUBBAND:
			if (Undo->data.entry)
				{
				InfoMemoryUsedByUndo -= sizeof(MTHBAND);
				GlobalFree(Undo->data.entry);
				}
			break;

		case UNDO_SUBAREA:
		case UNDO_SUBGATE:
		case UNDO_SUBEFFECT:
			if (Undo->data.entry)
				{
				InfoMemoryUsedByUndo -= sizeof(MAPRECTEX);
				GlobalFree(Undo->data.entry);
				}
			break;

		case UNDO_MODBAND:
			if (Undo->mod.band)
				{
				InfoMemoryUsedByUndo -= sizeof(MTHBAND);
				GlobalFree(Undo->mod.band);
				}
			break;

		case UNDO_MODGATE:
		case UNDO_MODAREA:
		case UNDO_MODEFFECT:
			if (Undo->mod.entry)
				{
				InfoMemoryUsedByUndo -= sizeof(MAPRECTEX);
				GlobalFree(Undo->mod.entry);
				}
			break;
		}

	InfoMemoryUsedByUndo -= sizeof(UNDO);
	GlobalFree(Undo);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
