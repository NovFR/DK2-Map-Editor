
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des fichiers des zones sauvegardées.
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

extern NODE		MapCopyThings;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#if (DKMAPVERSION != 1)
#error: Map version changed, Areas_ExtractMap() have to be updated
#endif

MAPINFO* Areas_ExtractMap(char *FileName)
{
	MAPINFO	*EditorMap;
	BYTE	*AreaMap;
	MAPINFO	 AreaMapInfo;
	DWORD	 AreaMapSize;
	char	*MapName;
	char	*Point;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	Compiler_SetLastError(0);
	ZeroMemory(&AreaMapInfo,sizeof(MAPINFO));
	strcpy(MapName,FileName);
	Point = strrchr(MapName,'.');
	if (Point) *Point = 0;
	strcat(MapName,szAreaMapExt);

	//--- Chargement du fichier complet ---

	AreaMap = (BYTE *)Files_Load(MapName,&AreaMapSize);
	if (!AreaMap)
		{
		GlobalFree(MapName);
		return(NULL);
		}

	//--- Test l'en-tête principal ---

	if (((HEADER *)AreaMap)->Header != FILEHEADER_AREA) goto Bad_Header;
	if (((HEADER *)AreaMap)->Version < DKZMINVERSION) goto Bad_Version;
	if (((HEADER *)AreaMap)->Version > DKZVERSION) goto Bad_Version;
	if (((HEADER *)AreaMap)->Size != AreaMapSize-sizeof(HEADER)) goto Bad_Size;

	//--- Test l'en-tête de la carte ---

	if (((HEADER *)(AreaMap+sizeof(HEADER)))->Header != FILEHEADER_MAP) goto Bad_Header;
	if (((HEADER *)(AreaMap+sizeof(HEADER)))->Version < DKMAPMINVERSION) goto Bad_Version;
	if (((HEADER *)(AreaMap+sizeof(HEADER)))->Version > DKMAPVERSION) goto Bad_Version;

	AreaMapInfo.Width = *((LONG *)(AreaMap+sizeof(HEADER)*2));
	AreaMapInfo.Height = *((LONG *)(AreaMap+sizeof(HEADER)*2+4));

	if (((HEADER *)(AreaMap+sizeof(HEADER)))->Size-8 != AreaMapInfo.Width*AreaMapInfo.Height*4) goto Bad_Size;

	//--- Conversion de la carte ---

	AreaMapInfo.Map = AreaMap+sizeof(HEADER)*2+8;
	EditorMap = Compiler_GameMapToEditorMap(&AreaMapInfo,NULL);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(EditorMap);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(NULL);

Bad_Version:
	Compiler_SetLastError(COMPILERR_BADVERSION);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(NULL);

Bad_Size:
	Compiler_SetLastError(COMPILERR_BADSIZE);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(NULL);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement des éléments						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#if (DKTGSVERSION != 6)
#error: Things version changed, Areas_ExtractThings() have to be updated
#endif

int Areas_ExtractThings(char *FileName, NODE *MapCopyThings, MAPSTATS *MapStats)
{
	MAPTHING	*MapThing;
	BYTE		*AreaMap;
	BYTE		*AreaThings;
	DWORD		 AreaMapSize;
	LONG		 Count;
	char		*MapName;
	char		*Point;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	if (MapCopyThings) ZeroMemory(MapCopyThings,sizeof(NODE));
	if (MapStats) ZeroMemory(MapStats,sizeof(MAPSTATS));
	strcpy(MapName,FileName);
	Point = strrchr(MapName,'.');
	if (Point) *Point = 0;
	strcat(MapName,szAreaMapExt);

	//--- Chargement du fichier complet ---

	AreaMap = (BYTE *)Files_Load(MapName,&AreaMapSize);
	if (!AreaMap)
		{
		GlobalFree(MapName);
		return(0);
		}

	//--- Test l'en-tête principal ---

	if (((HEADER *)AreaMap)->Header != FILEHEADER_AREA) goto Bad_Header;
	if (((HEADER *)AreaMap)->Version < DKZMINVERSION) goto Bad_Version;
	if (((HEADER *)AreaMap)->Version > DKZVERSION) goto Bad_Version;
	if (((HEADER *)AreaMap)->Size != AreaMapSize-sizeof(HEADER)) goto Bad_Size;

	//--- Ignore l'en-tête de la carte ---

	AreaThings = AreaMap+sizeof(HEADER);
	if (((HEADER *)AreaThings)->Header != FILEHEADER_MAP) goto Bad_Header;
	AreaThings = AreaMap+sizeof(HEADER)*2+((HEADER *)AreaThings)->Size;
	if (AreaThings >= AreaMap+AreaMapSize) goto Done;

	//--- Test l'en-tête des éléments ---

	if (((HEADER *)AreaThings)->Header != FILEHEADER_THINGS) goto Bad_Header;
	if (((HEADER *)AreaThings)->Version < DKTGSMINVERSION) goto Bad_Version;
	if (((HEADER *)AreaThings)->Version > DKTGSVERSION) goto Bad_Version;

	//--- Chargement des éléments ---

	Count = ((HEADER *)AreaThings)->Size/sizeof(MAPTHING);
	AreaThings += sizeof(HEADER);

	while(Count--)
		{
		if (AreaThings >= AreaMap+AreaMapSize)
			goto Bad_Pointer;
		if (MapStats)
			{
			switch(((MAPTHING *)AreaThings)->node.type)
				{
				case THING_CREATURE:
					MapStats->Creatures++;
					break;
				case THING_DOOR:
					MapStats->Doors++;
					break;
				case THING_TRAP:
					MapStats->Traps++;
					break;
				case THING_OBJECT:
					MapStats->Objects++;
					break;
				case THING_MAGICALOBJECT:
					MapStats->MagicalObjects++;
					break;
				}
			}
		if (MapCopyThings)
			{
			MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
			if (!MapThing)
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				List_ReleaseMemory(MapCopyThings);
				GlobalFree(MapName);
				GlobalFree(AreaMap);
				return(0);
				}
			CopyMemory(MapThing,AreaThings,sizeof(MAPTHING));
			List_AddEntry((NODE *)MapThing,MapCopyThings);
			}
		AreaThings += sizeof(MAPTHING);
		}

Done:	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(1);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(0);

Bad_Version:
	Compiler_SetLastError(COMPILERR_BADVERSION);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(0);

Bad_Size:
	Compiler_SetLastError(COMPILERR_BADSIZE);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(0);

Bad_Pointer:
	if (MapCopyThings) List_ReleaseMemory(MapCopyThings);
	Compiler_SetLastError(COMPILERR_EOF);
	GlobalFree(MapName);
	GlobalFree(AreaMap);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde des éléments associés à une zone			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Areas_WriteThings(HANDLE FileHandle, BOOL FromCopyMemory, MAPCLIENT *Client)
{
	MAPTHING	 TempThing;
	MAPTHING	*Thing;
	DWORD		 Written;
	long		 Size;
	long		 X,Y;

	Size = Areas_GetThingsSize(FromCopyMemory,Client);
	if (!Size) return(1);
	Size -= sizeof(HEADER);

	if (!Files_WriteHeader(FileHandle,FILEHEADER_THINGS,DKTGSVERSION,Size))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	if (FromCopyMemory)
		{
		for (Thing = (MAPTHING *)MapCopyThings.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			{
			WriteFile(FileHandle,Thing,sizeof(MAPTHING),&Written,NULL);
			if (Written != sizeof(MAPTHING))
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				return(0);
				}
			}

		return(1);
		}

	for (Y = Client->Y; Y != Client->Y+Client->Height; Y++)
		for (X = Client->X; X != Client->X+Client->Width; X++)
			{
			for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				if ((Thing->x == X)&&(Thing->y == Y))
					{
					CopyMemory(&TempThing,Thing,sizeof(MAPTHING));
					TempThing.node.next = NULL;
					TempThing.node.prev = NULL;
					TempThing.x = TempThing.x-Client->X;
					TempThing.y = TempThing.y-Client->Y;
					WriteFile(FileHandle,&TempThing,sizeof(MAPTHING),&Written,NULL);
					if (Written != sizeof(MAPTHING))
						{
						Compiler_SetLastError(COMPILERR_WINDOWS);
						return(0);
						}
					}
			if ((Thing = Things_GetAtMapPos(&MapDoors,X,Y)) != NULL)
				{
				CopyMemory(&TempThing,Thing,sizeof(MAPTHING));
				TempThing.node.next = NULL;
				TempThing.node.prev = NULL;
				TempThing.x = TempThing.x-Client->X;
				TempThing.y = TempThing.y-Client->Y;
				WriteFile(FileHandle,&TempThing,sizeof(MAPTHING),&Written,NULL);
				if (Written != sizeof(MAPTHING))
					{
					Compiler_SetLastError(COMPILERR_WINDOWS);
					return(0);
					}
				}
			if ((Thing = Things_GetAtMapPos(&MapTraps,X,Y)) != NULL)
				{
				CopyMemory(&TempThing,Thing,sizeof(MAPTHING));
				TempThing.node.next = NULL;
				TempThing.node.prev = NULL;
				TempThing.x = TempThing.x-Client->X;
				TempThing.y = TempThing.y-Client->Y;
				WriteFile(FileHandle,&TempThing,sizeof(MAPTHING),&Written,NULL);
				if (Written != sizeof(MAPTHING))
					{
					Compiler_SetLastError(COMPILERR_WINDOWS);
					return(0);
					}
				}
			if ((Thing = Things_GetAtMapPos(&MapObjects,X,Y)) != NULL)
				{
				CopyMemory(&TempThing,Thing,sizeof(MAPTHING));
				TempThing.node.next = NULL;
				TempThing.node.prev = NULL;
				TempThing.x = TempThing.x-Client->X;
				TempThing.y = TempThing.y-Client->Y;
				WriteFile(FileHandle,&TempThing,sizeof(MAPTHING),&Written,NULL);
				if (Written != sizeof(MAPTHING))
					{
					Compiler_SetLastError(COMPILERR_WINDOWS);
					return(0);
					}
				}
			if ((Thing = Things_GetAtMapPos(&MapMagicalObjects,X,Y)) != NULL)
				{
				CopyMemory(&TempThing,Thing,sizeof(MAPTHING));
				TempThing.node.next = NULL;
				TempThing.node.prev = NULL;
				TempThing.x = TempThing.x-Client->X;
				TempThing.y = TempThing.y-Client->Y;
				WriteFile(FileHandle,&TempThing,sizeof(MAPTHING),&Written,NULL);
				if (Written != sizeof(MAPTHING))
					{
					Compiler_SetLastError(COMPILERR_WINDOWS);
					return(0);
					}
				}
			}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Obtention de la taille totale d'une zone sauvegardée ««««««««««««»

LONG Areas_GetThingsSize(BOOL FromCopyMemory, MAPCLIENT *Client)
{
	LONG	Result = 0;
	long	X,Y;

	if (FromCopyMemory)
		{
		Result = List_EntryCount(&MapCopyThings)*sizeof(MAPTHING);
		if (Result) Result += sizeof(HEADER);
		return(Result);
		}

	for (Y = Client->Y; Y != Client->Y+Client->Height; Y++)
		for (X = Client->X; X != Client->X+Client->Width; X++)
			{
			Result += Things_GetCountAtMapPos(&MapCreatures,X,Y)*sizeof(MAPTHING);
			Result += Things_GetCountAtMapPos(&MapDoors,X,Y)*sizeof(MAPTHING);
			Result += Things_GetCountAtMapPos(&MapTraps,X,Y)*sizeof(MAPTHING);
			Result += Things_GetCountAtMapPos(&MapObjects,X,Y)*sizeof(MAPTHING);
			Result += Things_GetCountAtMapPos(&MapMagicalObjects,X,Y)*sizeof(MAPTHING);
			}

	if (Result) Result += sizeof(HEADER);
	return(Result);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
