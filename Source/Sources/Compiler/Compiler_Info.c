
//<<>-<>>---------------------------------------------------------------------()
/*
	Informations concernant la carte
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

extern NODE		MapCreatures;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern MAP		Map;
extern GLOBALVARS	MapGlobalVariables;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Enumère le nombre de terrains correspondants à la demande «««««««»

LONG Compiler_InfoEnumTerrainInRect(DWORD TerrainID, DWORD TerrainMask, DWORD Flags, MAPRECT *MapRect)
{
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	DWORD		 MapID;
	LONG		 Result;
	LONG		 X,Y;

	Result = 0;
	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (!Map_IsPointInRect(X,Y,MapRect))
				continue;

			switch(*(MapLinePtr+3))
				{
				// « World »
				//
				case 0:	MapID = Map_GetRealIDFromMap(MapLinePtr);
					// Closed mana are neutral rooms
					//
					if (MapID == 0x1F020101)
						{
						if (Flags&COMPILER_QUERYPLAYERS) goto Check;
						break;
						}
					// Heros rest are heroes rooms
					//
					if (MapID == 0x23010101)
						{
						if (Flags&COMPILER_QUERYPLAYERS) goto Check;
						break;
						}
					// Limits are considered solid
					//
					if ((MapID == 0x1E020101)||(MapID == 0xFE020101))
						MapID = 0x01020101;

					if (Flags&COMPILER_QUERYWORLD) goto Check;
					break;

				// « Rooms », « Gates », « Walls »
				//
				case 1:
				case 2:
				case 3:	MapID = Map_GetRealIDFromMap(MapLinePtr);
					if (Flags&COMPILER_QUERYPLAYERS) goto Check;
					break;
				}
			continue;

		Check:	MapID &= TerrainMask;
			if (MapID == TerrainID) Result++;
			continue;

			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	return(Result);
}


// «»»» Recherche la capacité de régénération en mana «««««««««««««««««««»

LONG Compiler_InfoManaInRect(UINT Player, MAPRECT *MapRect)
{
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	UINT		 ID;
	LONG		 VarPtr;
	LONG		 RoomsMana;
	LONG		 ManaVaults;
	LONG		 Result;
	LONG		 X,Y;

	RoomsMana = 0;
	ManaVaults = 0;
	Result = 0;
	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (!Map_IsPointInRect(X,Y,MapRect))
				continue;
			if (*(MapLinePtr+1) != Player)
				continue;

			switch(*(MapLinePtr+3))
				{
				// « Salles »
				//
				case 1:	ID = (Rooms[*MapLinePtr].Id&0xFF000000)>>24;
					break;
				// « Portails de héros »
				//
				case 2:	ID = (Gates[*MapLinePtr].Id&0xFF000000)>>24;
					break;
				// « Murs & terrain réclamés »
				//
				case 3:	ID = (Walls[*MapLinePtr].Id&0xFF000000)>>24;
					break;
				}

			if (!ID) continue;

			// Recherche le montant de mana généré par le terrain
			//
			for (VarPtr = 0; GTerrains[VarPtr].Id != 0; VarPtr++)
				if (GTerrains[VarPtr].Id == ID)
					{
					if (GTerrains[VarPtr].Id == 0x00000020)
						{
						// Store mana from mana vaults independently
						ManaVaults += MapGlobalVariables.Terrains[VarPtr].ManaGain;
						}
					else if (RoomsMana < Map.Properties.Variables[0x000000EB])
						{
						RoomsMana += MapGlobalVariables.Terrains[VarPtr].ManaGain;
						Result += MapGlobalVariables.Terrains[VarPtr].ManaGain;
						}
					break;
					}
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	// Dungeon heart always generate 30 additionnals mana units
	//
	if ((Compiler_InfoHeartPosition(Player,NULL,NULL))&&(RoomsMana < Map.Properties.Variables[0x000000EB]))
		Result += 30;

	if (Result > Map.Properties.Variables[0x000000EB])
		Result = Map.Properties.Variables[0x000000EB];

	// Mana vaults break the limit sets by variable 0xEB
	//
	Result += ManaVaults;

	if (Result > Map.Properties.Variables[0x000000A6])
		Result = Map.Properties.Variables[0x000000A6];

	return(Result);
}


// «»»» Recherche la capacité des hantres «««««««««««««««««««««««««««««««»

LONG Compiler_InfoRoomInRect(UINT Player, MAPRECT *MapRect)
{
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	LONG		 Result;
	LONG		 X,Y;

	Result = 0;
	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (!Map_IsPointInRect(X,Y,MapRect))
				continue;
			if (*(MapLinePtr+1) != Player)
				continue;
			if (*(MapLinePtr+3) != 1)		// Not a room
				continue;
			if (Rooms[*MapLinePtr].Id != 0x0B000101)// Not a lair
				continue;
			Result++;
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	return(Result);
}


// «»»» Recherche la capacité des salles du trésor ««««««««««««««««««««««»

LONG Compiler_InfoTreasuryCapacityInRect(UINT Player, MAPRECT *MapRect)
{
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	LONG		 Result;
	LONG		 X,Y;

	Result = 0;
	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (!Map_IsPointInRect(X,Y,MapRect))
				continue;
			if (*(MapLinePtr+1) != Player)
				continue;
			if (*(MapLinePtr+3) != 1)		// Not a room
				continue;
			if (Rooms[*MapLinePtr].Id != 0x0A000101)// Not a treasury
				continue;
			Result += Map.Properties.Variables[0x00000018];
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	if (Compiler_InfoHeartPosition(Player,NULL,NULL))
		Result += 16*Map.Properties.Variables[0x000000B2];

	return(Result);
}


// «»»» Recherche le nombre de créatures appartenant à un joueur ««««««««»

LONG Compiler_InfoCreaturesCountInRect(UINT Player, DWORD Flags, MAPRECT *MapRect)
{
	MAPTHING	*Thing;
	LONG		 Result;

	Result = 0;

	for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		if (!Map_IsPointInRect(Thing->x,Thing->y,MapRect))
			continue;
		if (Thing->owner != Player)
			continue;
		if ((Thing->id == 0x00000001)&&(!(Flags&COMPILER_QUERYIMPS)))
			continue;
		else if (Thing->id == 0x00000001)
			{
			Result++;
			continue;
			}
		if (!(Flags&COMPILER_QUERYALL))
			continue;
		Result++;
		}

	return(Result);
}


// «»»» Recherche l'or disponible dans le niveau ««««««««««««««««««««««««»

LONG Compiler_InfoGoldAmountInRect(DWORD Flags, BOOL *Gems, MAPRECT *MapRect)
{
	MAPTHING	*MapThing;
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	LONG		 Result;
	LONG		 Gold;
	LONG		 X,Y;

	Result = 0;
	MapPtr = Map.Map;

	// Get amount of gold in GOLD terrain
	//
	for (Gold = 3000, X = 0; GTerrains[X].Id != 0x00000000; X++)
		if (GTerrains[X].Id == 0x00000006)
			{
			Gold = MapGlobalVariables.Terrains[X].GoldValue;
			break;
			}

	// Checks if map contains gold or gems
	//
	if (Gems) *Gems = FALSE;
	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (!Map_IsPointInRect(X,Y,MapRect))
				continue;
			if (*(MapLinePtr+3) != 0)		// Not a terrain
				continue;
			switch(World[*MapLinePtr].Id)
				{
				case 0x06020101:		// Gold
					if (!(Flags&COMPILER_QUERYMAPGOLD)) break;
					Result += Gold;
					break;
				case 0x07020101:		// Gems
					if (!(Flags&COMPILER_QUERYMAPGEMS)) break;
					if (!Gems) break;
					*Gems = TRUE;
					break;
				}
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	// Check if map contains gold objects
	//
	if (Flags&COMPILER_QUERYOBJGOLD)
		{
		for (MapThing = (MAPTHING *)MapObjects.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
			{
			if (!Map_IsPointInRect(MapThing->x,MapThing->y,MapRect))
				continue;
			if ((MapThing->id != 0x00000001)&&(MapThing->id != 0x00000002)&&(MapThing->id != 0x00000003))
				continue;
			Result += MapThing->data.gold;
			}
		}

	// Check if map contains "influx cash" magical objects
	//
	if (Flags&COMPILER_QUERYMGCGOLD)
		{
		for (MapThing = (MAPTHING *)MapMagicalObjects.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
			{
			if (!Map_IsPointInRect(MapThing->x,MapThing->y,MapRect))
				continue;
			if (MapThing->id != 0x00000079)
				continue;
			Result += Map.Properties.Variables[0x0000001B];
			}
		}

	return(Result);
}


// «»»» Recherche le type de portail ««««««««««««««««««««««««««««««««««««»

void Compiler_InfoGateType(LONG X, LONG Y, LONG *Orientation, LONG *Options)
{
	BYTE	*MapPtr;

	*Orientation = 0x00000000;
	*Options = 0x00000021;

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	if (*(MapPtr+3) != 2) return;
	if ((Gates[*MapPtr].Id&0xFFFF00FF) != 0x25010001) return;

	//--- Defines Gates 6 & 7 ---

	*Options = 0x00000025;
	if ((Gates[*MapPtr].Id & 0x0000FF00) == 0x00000100) goto Set_HorizontalGate;

Set_VerticalGate:
	MapPtr += MAP_CONTROLBYTES;
	if (X+1 >= Map.Properties.Width) goto Set_WestVerticalGate;
	if (Things_IsSolid(MapPtr)) goto Set_WestVerticalGate;
Set_EastVerticalGate:
	*Orientation = 0x00000002;
	return;
Set_WestVerticalGate:
	*Orientation = 0x00000006;
	return;

Set_HorizontalGate:
	MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
	if (Y+1 >= Map.Properties.Height) goto Set_NorthHorizontalGate;
	if (Things_IsSolid(MapPtr)) goto Set_NorthHorizontalGate;
Set_SouthHorizontalGate:
	*Orientation = 0x00000004;
	return;
Set_NorthHorizontalGate:
	*Orientation = 0x00000000;
	return;
}


// «»»» Recherche les coeurs de donjon ««««««««««««««««««««««««««««««««««»

int Compiler_InfoHeartPosition(UINT Player, LONG *ResultX, LONG *ResultY)
{
	register BYTE	*MapPtr;
	register BYTE	*MapLinePtr;
	LONG		 X,Y;

	MapPtr = Map.Map;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (*(MapLinePtr+1) != Player)
				continue;
			if (*(MapLinePtr+3) != 1)		// Not a room
				continue;
			if (Rooms[*MapLinePtr].Id != 0x0E000101)// Not a heart
				continue;

			X += 2;
			Y += 2;
			if (X >= Map.Properties.Width) return(0);
			if (Y >= Map.Properties.Height) return(0);
			if (*(MapLinePtr+2*MAP_CONTROLBYTES+2*Map.Properties.Width*MAP_CONTROLBYTES+1) != Player) return(0);
			if (*(MapLinePtr+2*MAP_CONTROLBYTES+2*Map.Properties.Width*MAP_CONTROLBYTES+3) != 1) return(0);
			if (Rooms[*(MapLinePtr+2*MAP_CONTROLBYTES+2*Map.Properties.Width*MAP_CONTROLBYTES)].Id != 0x0E000101) return(0);

			if (ResultX) *ResultX = X;
			if (ResultY) *ResultY = Y;
			return(1);
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
