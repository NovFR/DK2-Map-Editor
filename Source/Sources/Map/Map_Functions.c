
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines diverses en rapport avec la carte.
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

extern DWORD		WorldItemID;
extern DWORD		ToolsPlyrID;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Transforme un identificateur en offset ««««««««««««««««««««««««««»

//--- Transformation simple ---

BYTE Map_TranslateID(BYTE MapID, BYTE MapOPT, ELEMENT *Table)
{
	DWORD	RealID;
	int	i;

	RealID = ((DWORD)MapID)<<24;
	RealID |= ((DWORD)MapOPT)<<8;

	for (i = 0; Table[i].Id != 0; i++)
		if ((Table[i].Id&0xFF00FF00) == RealID) return((BYTE)i);

	return((BYTE)-1);
}

//--- Avec identificateur du joueur ---

BYTE Map_TranslateIDEx(BYTE MapID, BYTE MapPLY, BYTE MapOPT, ELEMENT *Table)
{
	DWORD	RealID;
	int	i;

	RealID = ((DWORD)MapID)<<24;
	RealID |= ((DWORD)MapPLY)<<16;
	RealID |= ((DWORD)MapOPT)<<8;

	for (i = 0; Table[i].Id != 0; i++)
		if ((Table[i].Id&0xFFFFFF00) == RealID) return((BYTE)i);

	return((BYTE)-1);
}


// «»»» Transforme un offset en identificateur complet ««««««««««««««««««»

DWORD Map_GetRealIDFromMap(BYTE *MapPtr)
{
	DWORD	Result;

	switch(*(MapPtr+3))
		{
		case 0:	Result = World[*MapPtr].Id;
			break;
		case 1:	Result = Rooms[*MapPtr].Id;
			break;
		case 2:	Result = Gates[*MapPtr].Id;
			break;
		case 3:	Result = Walls[*MapPtr].Id;
			break;
		}

	Result &= 0xFF00FF00;
	Result |= (*(MapPtr+1))<<16;
	Result |= 0x00000001;
	return(Result);
}


// «»»» Traduit l'élément sélectionné en donnée de la carte «««««««««««««»

DWORD Map_TranslateWorldItem()
{
	register DWORD	MapFullID;
	register BYTE	MapID;

	if ((MapID = Map_TranslateID((BYTE)(WorldItemID>>24),(BYTE)(WorldItemID>>8),World)) != (BYTE)-1)
		{
		MapFullID = World[MapID].Id;
		MapFullID &= 0x00FFFF00;
		MapFullID |= (DWORD)(MapID<<24);
		}
	else if ((MapID = Map_TranslateID((BYTE)(WorldItemID>>24),(BYTE)(WorldItemID>>8),Rooms)) != (BYTE)-1)
		{
		MapFullID = Rooms[MapID].Id;
		MapFullID &= 0x0000FF00;
		MapFullID |= (DWORD)(MapID<<24);
		MapFullID |= (DWORD)((ToolsPlyrID+1)<<16);
		MapFullID |= 0x00000001;
		}
	else if ((MapID = Map_TranslateID((BYTE)(WorldItemID>>24),(BYTE)(WorldItemID>>8),Gates)) != (BYTE)-1)
		{
		MapFullID = Gates[MapID].Id;
		MapFullID &= 0x00FFFF00;
		MapFullID |= (DWORD)(MapID<<24);
		MapFullID |= 0x00000002;
		}
	else if ((MapID = Map_TranslateIDEx((BYTE)(WorldItemID>>24),(BYTE)(WorldItemID>>16),(BYTE)(WorldItemID>>8),Walls)) != (BYTE)-1)
		{
		MapFullID = Walls[MapID].Id;
		MapFullID &= 0x00FFFF00;
		MapFullID |= (DWORD)(MapID<<24);
		MapFullID |= 0x00000003;
		}
	else return(0xFFFFFFFF);

	return(MapFullID);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
