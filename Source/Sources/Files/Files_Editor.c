
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des fichiers de l'editeur.
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

extern HWND		hStatusWnd;
extern DWORD		TickCount;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern MAP		Map;
extern MAPOPTIONS	MapOptions[MAP_MAXPLAYERS];
extern MAPFIND		MapFind;
extern MAPEDITSTATS	MapStats;
extern DEBUGVERSIONS	MapVersions;
extern GLOBALVARS	MapGlobalVariables;
extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractAll(HWND hWnd, char *FileName, NODE *Errors)
{
	MAP	*NewMap;
	MAPINFO	*EditorMap;
	BYTE	*File;
	DWORD	 FileSize;
	char	*Name;

	Compiler_SetLastError(0);

	NewMap = GlobalAlloc(GPTR,sizeof(MAP));
	if (!NewMap)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	File = Editor_ReadFile(FileName,&FileSize);
	if (!File)
		{
		GlobalFree(NewMap);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	//--- Extraction de la carte ---

	Disk_CreateWindow(DISK_EDITORLOAD);
	Disk_Update(DISK_LEPRMAP);
	EditorMap = Editor_ExtractMap(File,FileSize,Errors);
	if (!EditorMap)
		{
		GlobalFree(NewMap);
		GlobalFree(File);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Disk_DestroyWindow(FALSE);
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	// Place les valeurs par défaut
	//
	Properties_MapResetOptions(NewMap,&MapOptions[0]);
	Properties_GlobalReset(&MapGlobalVariables,-1,MGRF_ALL);
	Map_ResetStats(&MapStats);
	NewMap->Map = EditorMap->Map;
	NewMap->Properties.Width = EditorMap->Width;
	NewMap->Properties.Height = EditorMap->Height;
	Map_CreateLimitWall(NewMap);
	if (!Map_CreateDisplayContext(NewMap))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		Map_Destroy(NewMap);
		GlobalFree(NewMap);
		GlobalFree(File);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Disk_DestroyWindow(FALSE);
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	// Place le nom de la carte (uniqt en cas de pb)
	//
	Name = strrchr(FileName,'\\');
	if (Name) strncpy(NewMap->Properties.Name,Name+1,63);
	else strncpy(NewMap->Properties.Name,FileName,63);
	Name = strrchr(NewMap->Properties.Name,'.');
	if (Name) *Name = 0;

	// Supprime l'ancienne carte
	//
	GlobalFree(EditorMap);
	Map_Destroy(&Map);
	CopyMemory(&Map,NewMap,sizeof(MAP));
	GlobalFree(NewMap);
	Debug_ResetVersions(FALSE);
	Undo_DestroyAllEntries();
	List_ReleaseMemory(&MapFind.Results);
	List_ReleaseMemory(&MapCreatures);
	List_ReleaseMemory(&MapDoors);
	List_ReleaseMemory(&MapTraps);
	List_ReleaseMemory(&MapObjects);
	List_ReleaseMemory(&MapMagicalObjects);
	List_ReleaseMemory(&MapHerosBands);
	List_ReleaseMemory(&MapAreas);
	List_ReleaseMemory(&MapGates);
	List_ReleaseMemory(&MapEffects);
	Triggers_ReleaseMemory();

	//--- Extraction des autres objets ---

	Disk_Update(DISK_LEPRSTATS);
	if (!Editor_ExtractStats(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_STATSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRMAPINFO);
	if (!Editor_ExtractMapInfo(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_MAINFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRPLAYERSINFO);
	if (!Editor_ExtractPlayers(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_PLAYERSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRTHINGS);
	if (!Editor_ExtractThings(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRRECTEX);
	if (!Editor_ExtractRectEx(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_RECTEXFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRBANDS);
	if (!Editor_ExtractHeros(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_HEROSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRTRIGGERS);
	if (!Editor_ExtractTriggers(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_TRIGGERSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRVARIABLES);
	if (!Editor_ExtractGlobalVariables(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_GLOBALSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LEPRDONE);
	Disk_DestroyWindow(FALSE);
	GlobalFree(File);
	return(1);
}


// «»»» Chargement du fichier complet «««««««««««««««««««««««««««««««««««»

BYTE* Editor_ReadFile(char *Filename, DWORD *Filesize)
{
	BYTE	*File;
	DWORD	 FileSize;
	char	*FileName;
	char	*Point;

	FileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!FileName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	Compiler_SetLastError(0);
	strcpy(FileName,Filename);
	Point = strrchr(FileName,'.');
	if (Point) *Point = 0;
	strcat(FileName,szEditorMapExt);

	File = (BYTE *)Files_Load(FileName,&FileSize);
	if (!File)
		{
		GlobalFree(FileName);
		return(NULL);
		}

	//--- Test les données ---

	if (((HEADER *)File)->Header != FILEHEADER_MAPFILE) goto Bad_Header;
	if (((HEADER *)File)->Version < DKMMINVERSION) goto Bad_Version;
	if (((HEADER *)File)->Version > DKMVERSION) goto Bad_Version;
	if (((HEADER *)File)->Size != FileSize-sizeof(HEADER)) goto Bad_Size;

	if (Filesize) *Filesize = FileSize;
	GlobalFree(FileName);
	return(File);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(FileName);
	GlobalFree(File);
	return(NULL);

Bad_Version:
	Compiler_SetLastError(COMPILERR_BADVERSION);
	GlobalFree(FileName);
	GlobalFree(File);
	return(NULL);

Bad_Size:
	Compiler_SetLastError(COMPILERR_BADSIZE);
	GlobalFree(FileName);
	GlobalFree(File);
	return(NULL);
}


// «»»» Recherche un en-tête dans un fichier ««««««««««««««««««««««««««««»

BYTE* Editor_FindHeader(DWORD Header, BYTE *FileMap, DWORD FileMapSize)
{
	BYTE	*FileMapPtr;

	FileMapPtr = FileMap+sizeof(HEADER);

	for (;;)
		{
		if (FileMapPtr >= FileMap+FileMapSize)
			return(NULL);
		if (((HEADER *)FileMapPtr)->Header == Header)
			break;
		switch(((HEADER *)FileMapPtr)->Header)
			{
			case FILEHEADER_STATS:
			case FILEHEADER_MAP:
			case FILEHEADER_MAPINFO:
			case FILEHEADER_MAPPLAYERS:
			case FILEHEADER_MAPRECTEX:
			case FILEHEADER_THINGS:
			case FILEHEADER_HEROS:
			case FILEHEADER_TRIGGERS:
			case FILEHEADER_VARIABLES:
				FileMapPtr += sizeof(HEADER)+((HEADER *)FileMapPtr)->Size;
				break;
			default:
				return((BYTE *)-1);
			}
		}

	return(FileMapPtr);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des statistiques					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractStats(BYTE *File, DWORD FileSize, NODE *Errors)
{
	BYTE		*FilePtr;
	LONG		 Version;

	FilePtr = Editor_FindHeader(FILEHEADER_STATS,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKSTSMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKSTSVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.StatsVersion = Version;

	FilePtr += sizeof(HEADER);

	CopyMemory(&MapStats,FilePtr,sizeof(MAPEDITSTATS));
	MapStats.TotalTicks = MapStats.RealTicks;
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des informations concernant la carte			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractMapInfo(BYTE *File, DWORD FileSize, NODE *Errors)
{
	BYTE	*FilePtr;
	LONG	 Version;

	FilePtr = Editor_FindHeader(FILEHEADER_MAPINFO,File,FileSize);
	if (!FilePtr)
		{
		Compiler_SetLastError(COMPILERR_EOF);
		return(0);
		}
	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKINFMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKINFVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.MapInfoVersion = Version;

	switch(Version)
		{
		case 1:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV1))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV1));
			ZeroMemory(&Map.Properties.SpeechFile[0],32);
			Map.Properties.Version = 17;
			Map.Properties.Options = 0x04;
			Map.Properties.GameTicks = 4;
			Map.Properties.Soundtrack = 1;
			break;
		case 2:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV2))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV2));
			Map.Properties.Version = 17;
			Map.Properties.Options = 0x04;
			Map.Properties.GameTicks = 4;
			Map.Properties.Soundtrack = 1;
			break;
		case 3:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV2))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV2));
			Map.Properties.Version = 17;
			Map.Properties.GameTicks = 4;
			Map.Properties.Soundtrack = 1;
			break;
		case 4:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV4))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV4));
			Map.Properties.Version = 17;
			Map.Properties.GameTicks = 4;
			Map.Properties.Soundtrack = 1;
			Map.Properties.Variables[0x0000009B] = 0x00002800;
			Map.Properties.Variables[0x0000009E] = 0x00001333;
			Map.Properties.Variables[0x000000DD] = 0x00001800;
			Map.Properties.Variables[0x000000DC] = 0x00001800;
			break;
		case 5:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV5))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV5));
			Map.Properties.Version = 17;
			Map.Properties.Variables[0x0000009B] = 0x00002800;
			Map.Properties.Variables[0x0000009E] = 0x00001333;
			Map.Properties.Variables[0x000000DD] = 0x00001800;
			Map.Properties.Variables[0x000000DC] = 0x00001800;
			break;
		case 6:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIESV6))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIESV6));
			Map.Properties.Variables[0x0000009B] = 0x00002800;
			Map.Properties.Variables[0x0000009E] = 0x00001333;
			Map.Properties.Variables[0x000000DD] = 0x00001800;
			Map.Properties.Variables[0x000000DC] = 0x00001800;
			break;
		case 7:	if (((HEADER *)FilePtr)->Size != sizeof(MAPPROPERTIES))
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&Map.Properties,FilePtr+sizeof(HEADER),sizeof(MAPPROPERTIES));
			break;
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des informations concernant les joueurs		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractPlayers(BYTE *File, DWORD FileSize, NODE *Errors)
{
	BYTE	*FilePtr;
	LONG	 Version;
	int	 i;

	FilePtr = Editor_FindHeader(FILEHEADER_MAPPLAYERS,File,FileSize);
	if (!FilePtr)
		{
		Compiler_SetLastError(COMPILERR_EOF);
		return(0);
		}
	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKPLYMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKPLYVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.PlayersVersion = Version;

	switch(Version)
		{
		case 1:	if (((HEADER *)FilePtr)->Size != sizeof(MAPOPTIONSV1)*MAP_MAXPLAYERS)
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			for (FilePtr += sizeof(HEADER), i = 0; i != MAP_MAXPLAYERS; i++)
				{
				CopyMemory(&MapOptions[i].CreaturesCount[0],&((MAPOPTIONSV1 *)FilePtr)->CreaturesCount[0],MAP_VARCREATURES*4);
				CopyMemory(&MapOptions[i].Rooms[0],&((MAPOPTIONSV1 *)FilePtr)->Rooms[0],MAP_VARROOMS);
				CopyMemory(&MapOptions[i].Traps[0],&((MAPOPTIONSV1 *)FilePtr)->Traps[0],MAP_VARTRAPS);
				CopyMemory(&MapOptions[i].Doors[0],&((MAPOPTIONSV1 *)FilePtr)->Doors[0],MAP_VARDOORS);
				CopyMemory(&MapOptions[i].Spells[0],&((MAPOPTIONSV1 *)FilePtr)->Spells[0],MAP_VARSPELLS);
				CopyMemory(&MapOptions[i].CreaturesAvailable[0],&((MAPOPTIONSV1 *)FilePtr)->CreaturesAvailable[0],MAP_VARCREATURES);
				MapOptions[i].PlayerHealth	= ((MAPOPTIONSV1 *)FilePtr)->Health;
				MapOptions[i].PlayerGold	= ((MAPOPTIONSV1 *)FilePtr)->Gold;
				MapOptions[i].PlayerComputer	= ((MAPOPTIONSV1 *)FilePtr)->ComputerPlayer;
				MapOptions[i].PlayerCPUAreaX1	= ((MAPOPTIONSV1 *)FilePtr)->CPUAreaX1;
				MapOptions[i].PlayerCPUAreaY1	= ((MAPOPTIONSV1 *)FilePtr)->CPUAreaY1;
				MapOptions[i].PlayerCPUAreaX2	= ((MAPOPTIONSV1 *)FilePtr)->CPUAreaX2;
				MapOptions[i].PlayerCPUAreaY2	= ((MAPOPTIONSV1 *)FilePtr)->CPUAreaY2;
				MapOptions[i].PlayerCameraX	= -1;
				MapOptions[i].PlayerCameraY	= -1;
				CopyMemory(&MapOptions[i].PlayerName[0],&((MAPOPTIONSV1 *)FilePtr)->PlayerName[0],32);
				FilePtr += sizeof(MAPOPTIONSV1);
				}
			break;

		case 2:if (((HEADER *)FilePtr)->Size != sizeof(MAPOPTIONSV2)*MAP_MAXPLAYERS)
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&MapOptions[0],FilePtr+sizeof(HEADER),sizeof(MAPOPTIONSV2)*MAP_MAXPLAYERS);
			for (i = 0; i != MAP_MAXPLAYERS; i++)
				{
				MapOptions[i].PlayerCameraX	= -1;
				MapOptions[i].PlayerCameraY	= -1;
				}
			break;

		default:if (((HEADER *)FilePtr)->Size != sizeof(MAPOPTIONS)*MAP_MAXPLAYERS)
				{
				Compiler_SetLastError(COMPILERR_BADSIZE);
				Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
				return(0);
				}
			CopyMemory(&MapOptions[0],FilePtr+sizeof(HEADER),sizeof(MAPOPTIONS)*MAP_MAXPLAYERS);
			break;
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

MAPINFO* Editor_ExtractMap(BYTE *FileMap, DWORD FileMapSize, NODE *Errors)
{
	MAPINFO	*EditorMap;
	BYTE	*FileMapPtr;
	MAPINFO	 FileMapInfo;
	LONG	 Version;

	ZeroMemory(&FileMapInfo,sizeof(MAPINFO));

	FileMapPtr = Editor_FindHeader(FILEHEADER_MAP,FileMap,FileMapSize);
	if (!FileMapPtr)
		{
		Compiler_SetLastError(COMPILERR_EOF);
		return(NULL);
		}
	if (FileMapPtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(NULL);
		}

	Version = ((HEADER *)FileMapPtr)->Version;
	if (Version < DKMAPMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(NULL);
		}
	if (Version > DKMAPVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(NULL);
		}
	MapVersions.MapVersion = Version;

	FileMapInfo.Width = *((LONG *)(FileMapPtr+sizeof(HEADER)));
	FileMapInfo.Height = *((LONG *)(FileMapPtr+sizeof(HEADER)+4));
	if (((HEADER *)FileMapPtr)->Size-8 != FileMapInfo.Width*FileMapInfo.Height*4)
		{
		Compiler_SetLastError(COMPILERR_BADSIZE);
		Compiler_ReportErrorText(Errors,REPORT_BADSIZE,0);
		return(NULL);
		}
	FileMapPtr += sizeof(HEADER)+8;

	FileMapInfo.Map = FileMapPtr;
	EditorMap = Compiler_GameMapToEditorMap(&FileMapInfo,Errors);
	return(EditorMap);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des zones & passages					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractRectEx(BYTE *File, DWORD FileSize, NODE *Errors, MAPSTATS *Stats, BOOL Extract)
{
	BYTE	*FilePtr;
	NODE	*List;
	LONG	 RectCount;
	LONG	 Version;

	FilePtr = Editor_FindHeader(FILEHEADER_MAPRECTEX,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKRECMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKRECVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.RectExVersion = Version;

	RectCount = ((HEADER *)FilePtr)->Size/sizeof(MAPRECTEX);
	FilePtr += sizeof(HEADER);

	while(RectCount--)
		{
		switch(((MAPRECTEX *)FilePtr)->node.type)
			{
			case RECTEX_AREA:
				if (Stats) Stats->Areas++;
				List = &MapAreas;
				break;
			case RECTEX_GATE:
				if (Stats) Stats->Gates++;
				List = &MapGates;
				break;
			case RECTEX_EFFECT:
				List = &MapEffects;
				break;
			default:List = NULL;
				Compiler_ReportErrorText(Errors,REPORT_BADRECTEX,((MAPRECTEX *)FilePtr)->node.type);
				break;
			}

		if ((List)&&(Extract))
			{
			MAPRECTEX	*MapRectEx;

			MapRectEx = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
			if (MapRectEx)
				{
				CopyMemory(MapRectEx,FilePtr,sizeof(MAPRECTEX));
				MapRectEx->rect.name = MapRectEx->name;
				// The following line fixes a stupid bug in default
				// text color (was 0xFFFFFF00 instead of 0x00FFFF00).
				// Error was ignored by WindowsMe but not by WindowsXP.
				MapRectEx->rect.textcolor &= 0x00FFFFFF;
				List_AddEntry((NODE *)MapRectEx,List);
				}
			else
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				List_ReleaseMemory(&MapAreas);
				List_ReleaseMemory(&MapGates);
				List_ReleaseMemory(&MapEffects);
				return(0);
				}
			}

		FilePtr += sizeof(MAPRECTEX);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des éléments						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#if (DKTGSVERSION != 6)
#error Do not forget to update ThingsCount...
#endif

int Editor_ExtractThings(BYTE *File, DWORD FileSize, NODE *Errors, MAPSTATS *Stats, BOOL Extract)
{
	BYTE	*FilePtr;
	NODE	*List;
	LONG	 ThingsCount;
	LONG	 Version;

	FilePtr = Editor_FindHeader(FILEHEADER_THINGS,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKTGSMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKTGSVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.ThingsVersion = Version;

	switch(Version)
		{
		case 1:	ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHINGV1);
			break;
		case 2:	ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHINGV2);
			break;
		case 3:	ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHINGV2);
			break;
		case 4:	ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHINGV4);
			break;
		case 5:	ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHINGV5);
			break;
		default:ThingsCount = ((HEADER *)FilePtr)->Size/sizeof(MAPTHING);
			break;
		}
	FilePtr += sizeof(HEADER);

	while(ThingsCount--)
		{
		switch(((MAPTHING *)FilePtr)->node.type)
			{
			case THING_CREATURE:
				if (Stats) Stats->Creatures++;
				List = &MapCreatures;
				break;
			case THING_DOOR:
				if (Stats) Stats->Doors++;
				List = &MapDoors;
				break;
			case THING_TRAP:
				if (Stats) Stats->Traps++;
				List = &MapTraps;
				break;
			case THING_OBJECT:
				if (Stats) Stats->Objects++;
				List = &MapObjects;
				break;
			case THING_MAGICALOBJECT:
				if (Stats) Stats->MagicalObjects++;
				List = &MapMagicalObjects;
				break;
			default:List = NULL;
				Compiler_ReportErrorText(Errors,REPORT_BADTHING,((MAPRECTEX *)FilePtr)->node.type);
				break;
			}

		if ((List)&&(Extract))
			{
			MAPTHING	*MapThing;

			MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
			if (MapThing)
				{
				switch(Version)
					{
					case 1:	MapThing->node.type 	   = ((MAPTHINGV1 *)FilePtr)->node.type;
						MapThing->x		   = ((MAPTHINGV1 *)FilePtr)->x;
						MapThing->y		   = ((MAPTHINGV1 *)FilePtr)->y;
						MapThing->id		   = ((MAPTHINGV1 *)FilePtr)->id;
						MapThing->owner		   = ((MAPTHINGV1 *)FilePtr)->owner;
						MapThing->health	   = ((MAPTHINGV1 *)FilePtr)->health;
						MapThing->data.all	   = ((MAPTHINGV1 *)FilePtr)->data.all;
						MapThing->herosoptions	   = ((MAPTHINGV1 *)FilePtr)->options;
						if (((MAPTHINGV1 *)FilePtr)->node.type == THING_CREATURE)
							{
							MapThing->creaturesoptions = 0x09;
							if (((MAPTHINGV1 *)FilePtr)->owner == 1)
								{
								MapThing->objective = 0x13;
								MapThing->ennemy = 0x00;
								}
							}
						if (((MAPTHINGV1 *)FilePtr)->node.type == THING_TRAP)
							MapThing->data.shots = 4;
						break;
					case 2:	MapThing->node.type 	   = ((MAPTHINGV2 *)FilePtr)->node.type;
						MapThing->x		   = ((MAPTHINGV2 *)FilePtr)->x;
						MapThing->y		   = ((MAPTHINGV2 *)FilePtr)->y;
						MapThing->id		   = ((MAPTHINGV2 *)FilePtr)->id;
						MapThing->owner		   = ((MAPTHINGV2 *)FilePtr)->owner;
						MapThing->health	   = ((MAPTHINGV2 *)FilePtr)->health;
						MapThing->data.all	   = ((MAPTHINGV2 *)FilePtr)->data.all;
						MapThing->herosoptions	   = ((MAPTHINGV2 *)FilePtr)->options;
						MapThing->area		   = ((MAPTHINGV2 *)FilePtr)->area;
						MapThing->uniqueid	   = ((MAPTHINGV2 *)FilePtr)->uniqueid;
						if (((MAPTHINGV2 *)FilePtr)->node.type == THING_CREATURE)
							{
							MapThing->creaturesoptions = 0x09;
							if (((MAPTHINGV2 *)FilePtr)->owner == 1)
								{
								MapThing->objective = 0x13;
								MapThing->ennemy = 0x00;
								}
							}
						if (((MAPTHINGV2 *)FilePtr)->node.type == THING_TRAP)
							MapThing->data.shots = 4;
						break;
					case 3:	MapThing->node.type 	   = ((MAPTHINGV2 *)FilePtr)->node.type;
						MapThing->x		   = ((MAPTHINGV2 *)FilePtr)->x;
						MapThing->y		   = ((MAPTHINGV2 *)FilePtr)->y;
						MapThing->id		   = ((MAPTHINGV2 *)FilePtr)->id;
						MapThing->owner		   = ((MAPTHINGV2 *)FilePtr)->owner;
						MapThing->health	   = ((MAPTHINGV2 *)FilePtr)->health;
						MapThing->data.all	   = ((MAPTHINGV2 *)FilePtr)->data.all;
						MapThing->herosoptions	   = ((MAPTHINGV2 *)FilePtr)->options;
						MapThing->area		   = ((MAPTHINGV2 *)FilePtr)->area;
						MapThing->uniqueid	   = ((MAPTHINGV2 *)FilePtr)->uniqueid;
						if (((MAPTHINGV2 *)FilePtr)->node.type == THING_CREATURE)
							{
							MapThing->creaturesoptions = 0x09;
							if (((MAPTHINGV2 *)FilePtr)->owner == 1)
								{
								MapThing->objective = 0x13;
								MapThing->ennemy = 0x00;
								}
							}
						if (((MAPTHINGV2 *)FilePtr)->node.type == THING_TRAP)
							MapThing->data.shots = 4;
						break;
					case 4:	MapThing->node.type 	   = ((MAPTHINGV4 *)FilePtr)->node.type;
						MapThing->x		   = ((MAPTHINGV4 *)FilePtr)->x;
						MapThing->y		   = ((MAPTHINGV4 *)FilePtr)->y;
						MapThing->id		   = ((MAPTHINGV4 *)FilePtr)->id;
						MapThing->owner		   = ((MAPTHINGV4 *)FilePtr)->owner;
						MapThing->health	   = ((MAPTHINGV4 *)FilePtr)->health;
						MapThing->data.all	   = ((MAPTHINGV4 *)FilePtr)->data.all;
						MapThing->herosoptions	   = ((MAPTHINGV4 *)FilePtr)->herosoptions;
						MapThing->creaturesoptions = ((MAPTHINGV4 *)FilePtr)->creaturesoptions;
						MapThing->objective	   = ((MAPTHINGV4 *)FilePtr)->objective;
						MapThing->area		   = ((MAPTHINGV4 *)FilePtr)->area;
						MapThing->uniqueid	   = ((MAPTHINGV4 *)FilePtr)->uniqueid;
						MapThing->ennemy	   = ((MAPTHINGV4 *)FilePtr)->ennemy;
						if (((MAPTHINGV4 *)FilePtr)->node.type == THING_TRAP)
							MapThing->data.shots = 4;
						break;
					case 5:	MapThing->node.type 	   = ((MAPTHINGV5 *)FilePtr)->node.type;
						MapThing->x		   = ((MAPTHINGV5 *)FilePtr)->x;
						MapThing->y		   = ((MAPTHINGV5 *)FilePtr)->y;
						MapThing->id		   = ((MAPTHINGV5 *)FilePtr)->id;
						MapThing->owner		   = ((MAPTHINGV5 *)FilePtr)->owner;
						MapThing->health	   = ((MAPTHINGV5 *)FilePtr)->health;
						MapThing->data.all	   = ((MAPTHINGV5 *)FilePtr)->data.all;
						MapThing->herosoptions	   = ((MAPTHINGV5 *)FilePtr)->herosoptions;
						MapThing->creaturesoptions = ((MAPTHINGV5 *)FilePtr)->creaturesoptions;
						MapThing->objective	   = ((MAPTHINGV5 *)FilePtr)->objective;
						MapThing->area		   = ((MAPTHINGV5 *)FilePtr)->area;
						MapThing->uniqueid	   = ((MAPTHINGV5 *)FilePtr)->uniqueid;
						MapThing->ennemy	   = ((MAPTHINGV5 *)FilePtr)->ennemy;
						break;
					default:CopyMemory(MapThing,FilePtr,sizeof(MAPTHING));
						break;
					}
				List_AddEntry((NODE *)MapThing,List);
				}
			else
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				List_ReleaseMemory(&MapCreatures);
				List_ReleaseMemory(&MapDoors);
				List_ReleaseMemory(&MapTraps);
				List_ReleaseMemory(&MapObjects);
				List_ReleaseMemory(&MapMagicalObjects);
				return(0);
				}
			}

		switch(Version)
			{
			case 1:	FilePtr += sizeof(MAPTHINGV1);
				break;
			case 2:	
			case 3:	FilePtr += sizeof(MAPTHINGV2);
				break;
			case 4:	FilePtr += sizeof(MAPTHINGV4);
				break;
			case 5:	FilePtr += sizeof(MAPTHINGV5);
				break;
			default:FilePtr += sizeof(MAPTHING);
				break;
			}
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des bandes de héros					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#if (DKMTHVERSION != 5)
#error Do not forget to update HerosCount...
#endif

int Editor_ExtractHeros(BYTE *File, DWORD FileSize, NODE *Errors, MAPSTATS *Stats, BOOL Extract)
{
	BYTE	*FilePtr;
	LONG	 HerosCount;
	LONG	 Version;

	int	 i;

	FilePtr = Editor_FindHeader(FILEHEADER_HEROS,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKMTHMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKMTHVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.BandsVersion = Version;

	switch(Version)
		{
		case 1:	HerosCount = ((HEADER *)FilePtr)->Size/sizeof(MTHBANDV1);
			break;
		case 2:	HerosCount = ((HEADER *)FilePtr)->Size/sizeof(MTHBANDV2);
			break;
		case 3:	HerosCount = ((HEADER *)FilePtr)->Size/sizeof(MTHBANDV3);
			break;
		case 4:	HerosCount = ((HEADER *)FilePtr)->Size/sizeof(MTHBANDV4);
			break;
		default:HerosCount = ((HEADER *)FilePtr)->Size/sizeof(MTHBAND);
			break;
		}
	FilePtr += sizeof(HEADER);

	while(HerosCount--)
		{
		if (Stats) Stats->HerosBands++;

		if (Extract)
			{
			MTHBAND	*Band;

			Band = GlobalAlloc(GPTR,sizeof(MTHBAND));
			if (!Band)
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				List_ReleaseMemory(&MapHerosBands);
				return(0);
				}
			else
				{
				switch(Version)
					{
					case 1:	Band->id = ((MTHBANDV1 *)FilePtr)->id;
						CopyMemory(Band->name,((MTHBANDV1 *)FilePtr)->name,THING_MAXBANDNAME);
						for (i = 0; i != THING_MAXHEROSINBAND; i++)
							{
							Band->heros[i].isvalid	 = ((MTHBANDV1 *)FilePtr)->heros[i].isvalid;
							Band->heros[i].level	 = ((MTHBANDV1 *)FilePtr)->heros[i].level;
							Band->heros[i].enemy	 = ((MTHBANDV1 *)FilePtr)->heros[i].enemy;
							Band->heros[i].id	 = ((MTHBANDV1 *)FilePtr)->heros[i].id;
							Band->heros[i].health	 = ((MTHBANDV1 *)FilePtr)->heros[i].health;
							Band->heros[i].options	 = ((MTHBANDV1 *)FilePtr)->heros[i].options;
							Band->heros[i].objective = 0x13;
							Band->heros[i].options2	 = 0x01|0x08|0x04|0x20;
							}
						Band->heros[0].options2 = 0x01|0x08|0x02|0x20;
						break;

					case 2:	Band->id = ((MTHBANDV2 *)FilePtr)->id;
						Band->uniqueid = ((MTHBANDV2 *)FilePtr)->uniqueid;
						CopyMemory(Band->name,((MTHBANDV2 *)FilePtr)->name,THING_MAXBANDNAME);
						for (i = 0; i != THING_MAXHEROSINBAND; i++)
							{
							Band->heros[i].isvalid	 = ((MTHBANDV2 *)FilePtr)->heros[i].isvalid;
							Band->heros[i].level	 = ((MTHBANDV2 *)FilePtr)->heros[i].level;
							Band->heros[i].enemy	 = ((MTHBANDV2 *)FilePtr)->heros[i].enemy;
							Band->heros[i].id	 = ((MTHBANDV2 *)FilePtr)->heros[i].id;
							Band->heros[i].health	 = ((MTHBANDV2 *)FilePtr)->heros[i].health;
							Band->heros[i].options	 = ((MTHBANDV2 *)FilePtr)->heros[i].options;
							Band->heros[i].objective = 0x13;
							Band->heros[i].options2	 = 0x01|0x08|0x04|0x20;
							Band->heros[i].area	 = ((MTHBANDV2 *)FilePtr)->heros[i].area;
							Band->heros[i].uniqueid	 = ((MTHBANDV2 *)FilePtr)->heros[i].uniqueid;
							}
						Band->heros[0].options2 = 0x01|0x08|0x02|0x20;
						break;

					case 3:	Band->id = ((MTHBANDV3 *)FilePtr)->id;
						Band->uniqueid = ((MTHBANDV3 *)FilePtr)->uniqueid;
						CopyMemory(Band->name,((MTHBANDV3 *)FilePtr)->name,THING_MAXBANDNAME);
						for (i = 0; i != THING_MAXHEROSINBAND; i++)
							{
							Band->heros[i].isvalid	 = ((MTHBANDV3 *)FilePtr)->heros[i].isvalid;
							Band->heros[i].level	 = ((MTHBANDV3 *)FilePtr)->heros[i].level;
							Band->heros[i].enemy	 = ((MTHBANDV3 *)FilePtr)->heros[i].enemy;
							Band->heros[i].id	 = ((MTHBANDV3 *)FilePtr)->heros[i].id;
							Band->heros[i].health	 = ((MTHBANDV3 *)FilePtr)->heros[i].health;
							Band->heros[i].options	 = ((MTHBANDV3 *)FilePtr)->heros[i].options;
							Band->heros[i].objective = ((MTHBANDV3 *)FilePtr)->heros[i].objective;
							Band->heros[i].options2	 = 0x01|0x08|0x04|0x20;
							Band->heros[i].area	 = ((MTHBANDV3 *)FilePtr)->heros[i].area;
							Band->heros[i].uniqueid	 = ((MTHBANDV3 *)FilePtr)->heros[i].uniqueid;
							}
						Band->heros[0].options2 = 0x01|0x08|0x02|0x20;
						break;

					case 4:	Band->id = ((MTHBANDV4 *)FilePtr)->id;
						Band->uniqueid = ((MTHBANDV4 *)FilePtr)->uniqueid;
						CopyMemory(Band->name,((MTHBANDV4 *)FilePtr)->name,THING_MAXBANDNAME);
						for (i = 0; i != THING_MAXHEROSINBAND; i++)
							{
							Band->heros[i].isvalid	 = ((MTHBANDV4 *)FilePtr)->heros[i].isvalid;
							Band->heros[i].level	 = ((MTHBANDV4 *)FilePtr)->heros[i].level;
							Band->heros[i].enemy	 = ((MTHBANDV4 *)FilePtr)->heros[i].enemy;
							Band->heros[i].id	 = ((MTHBANDV4 *)FilePtr)->heros[i].id;
							Band->heros[i].health	 = ((MTHBANDV4 *)FilePtr)->heros[i].health;
							Band->heros[i].options	 = ((MTHBANDV4 *)FilePtr)->heros[i].options;
							Band->heros[i].objective = ((MTHBANDV4 *)FilePtr)->heros[i].objective;
							Band->heros[i].options2	 = ((MTHBANDV4 *)FilePtr)->heros[i].options2;
							Band->heros[i].area	 = ((MTHBANDV4 *)FilePtr)->heros[i].area;
							Band->heros[i].uniqueid	 = ((MTHBANDV4 *)FilePtr)->heros[i].uniqueid;
							}
						break;

					default:CopyMemory(Band,FilePtr,sizeof(MTHBAND));
						break;
					}
				List_AddEntry((NODE *)Band,&MapHerosBands);
				}
			}

		switch(Version)
			{
			case 1:	FilePtr += sizeof(MTHBANDV1);
				break;
			case 2:	FilePtr += sizeof(MTHBANDV2);
				break;
			case 3:	FilePtr += sizeof(MTHBANDV3);
				break;
			case 4:	FilePtr += sizeof(MTHBANDV4);
				break;
			default:FilePtr += sizeof(MTHBAND);
				break;
			}
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des évènements					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractTriggers(BYTE *File, DWORD FileSize, NODE *Errors)
{
	BYTE		*FilePtr;
	LONG		 Version;

	FilePtr = Editor_FindHeader(FILEHEADER_TRIGGERS,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKTRGMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKTRGVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.TriggersVersion = Version;

	FilePtr += sizeof(HEADER);

	switch(Version)
		{
		case 1:{
			TRIGGERLINK		*Link;
			TRIGGER			*Trigger;
			ACTION			*Action;
			LONG			 ActionPtCount;
			LONG			 TriggersCount;
			LONG			 ActionsCount;
			WORD			 TriggerID;
			WORD			 ActionID;

			TriggerID = 0x0001;
			ActionID = 0xFFFF;
			ActionPtCount = *((LONG *)FilePtr)++;

			while(ActionPtCount--)
				{
				if (*((LONG *)FilePtr)++ != 0x000000C5)
					{
					Compiler_SetLastError(COMPILERR_BADHEADER);
					goto Error;
					}
				TriggersCount = *((LONG *)FilePtr)++;

				Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
				if (!Link)
					{
					Compiler_SetLastError(COMPILERR_WINDOWS);
					goto Error;
					}
				Link->node.type = SCOPE_AREA;
				Link->area = ((MAPACTIONPOINTV1 *)FilePtr)->area;
				Link->trigger = TriggerID;
				List_AddEntry((NODE *)Link,&MapActionPoints);

				FilePtr += sizeof(MAPACTIONPOINTV1);

				while(TriggersCount--)
					{
					if (*((LONG *)FilePtr)++ != 0x000000D5)
						{
						Compiler_SetLastError(COMPILERR_BADHEADER);
						goto Error;
						}
					ActionsCount = *((LONG *)FilePtr)++;

					Trigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
					if (!Trigger)
						{
						Compiler_SetLastError(COMPILERR_WINDOWS);
						goto Error;
						}
					Trigger->id = TriggerID++;
					Trigger->and = ActionID;
					Trigger->repeat = 0x01;
					if (TriggersCount) Trigger->or = TriggerID;
					if (((MAPTRIGGERV1 *)FilePtr)->reusable) Trigger->repeat = 0xFF;
					switch(((MAPTRIGGERV1 *)FilePtr)->node.type)
						{
						case TRV1_CONTROLAREA:
							Trigger->param1 = 5;
							Trigger->param2 = ((MAPTRIGGERV1 *)FilePtr)->player;
							Trigger->param5 = ((MAPTRIGGERV1 *)FilePtr)->squares;
							Trigger->ctrl = TRIGGER_CLAIMAREA;
							break;
						case TRV1_DISCOVERAREA:
							Trigger->param1 = 5;
							Trigger->param2 = ((MAPTRIGGERV1 *)FilePtr)->player;
							Trigger->param5 = 1;
							Trigger->ctrl = TRIGGER_CREAENTERAREA;
							break;
						case TRV1_SELECTAREA:
							Trigger->param2 = ((MAPTRIGGERV1 *)FilePtr)->player;
							Trigger->ctrl = TRIGGER_SELECTALLAREA;
							break;
						case TRV1_TIMEELAPSED:
							Trigger->param1 = 5;
							Trigger->param5 = ((MAPTRIGGERV1 *)FilePtr)->seconds;
							Trigger->ctrl = TRIGGER_TIMEELAPSED;
							break;
						}
					Trigger->node.type = TYPE_TRIGGER;
					List_AddEntry((NODE *)Trigger,&MapTriggers);

					FilePtr += sizeof(MAPTRIGGERV1);

					while(ActionsCount--)
						{
						if (*((LONG *)FilePtr)++ != 0x000000D6)
							{
							Compiler_SetLastError(COMPILERR_BADHEADER);
							goto Error;
							}

						Action = GlobalAlloc(GPTR,sizeof(ACTION));
						if (!Action)
							{
							Compiler_SetLastError(COMPILERR_WINDOWS);
							goto Error;
							}
						Action->id = ActionID--;
						if (ActionsCount) Action->next = ActionID;
						Action->one = 0x01;
						switch(((MAPACTIONV1 *)FilePtr)->node.type)
							{
							case ACV1_RELEASEBAND:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->band;
								Action->param5 = ((MAPACTIONV1 *)FilePtr)->area;
								Action->ctrl = ACTION_RELEASEBAND;
								break;
							case ACV1_DESTROYGATE:
								Action->param5 = ((MAPACTIONV1 *)FilePtr)->gate;
								Action->ctrl = ACTION_DESTROYGATE;
								break;
							case ACV1_SHOWAREA:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->area;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->activate;
								Action->ctrl = ACTION_SHOWAREA;
								break;
							case ACV1_MAKEAREAVISIBLE:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->area;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->activate;
								Action->ctrl = ACTION_ENABLEFOG;
								break;
							case ACV1_ADDCREATURE:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->creatureid;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->player;
								Action->param3 = ((MAPACTIONV1 *)FilePtr)->level;
								Action->param5 = ((MAPACTIONV1 *)FilePtr)->x-1;
								Action->param7 = ((MAPACTIONV1 *)FilePtr)->y-1;
								Action->ctrl = ACTION_ADDCREATURE;
								break;
							case ACV1_CHANGESQUARE:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->terrainid;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->player;
								Action->param5 = ((MAPACTIONV1 *)FilePtr)->x;
								Action->param7 = ((MAPACTIONV1 *)FilePtr)->y;
								Action->ctrl = ACTION_CHANGESQUARE;
								break;
							case ACV1_ADDELEMENT:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->player;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->type;
								Action->param3 = ((MAPACTIONV1 *)FilePtr)->element;
								Action->param4 = 1;
								Action->ctrl = ACTION_ADDELEMENT;
								break;
							case ACV1_HIGHLIGHTELEM:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->type;
								Action->param2 = ((MAPACTIONV1 *)FilePtr)->element;
								Action->param3 = !(((MAPACTIONV1 *)FilePtr)->activate);
								Action->ctrl = ACTION_SHOWELEMENT;
								break;
							case ACV1_SHOWTIMER:
								Action->param1 = 16;
								Action->param5 = ((MAPACTIONV1 *)FilePtr)->seconds;
								Action->ctrl = ACTION_DISPLAYTIMER;
								break;
							case ACV1_SHOWROOM:
								Action->param1 = ((MAPACTIONV1 *)FilePtr)->room;
								Action->ctrl = ACTION_SHOWNEXTROOM;
								break;
							}
						Action->node.type = TYPE_ACTION;
						List_AddEntry((NODE *)Action,&MapActions);

						FilePtr += sizeof(MAPACTIONV1);
						}
					}
				}
			Triggers_MakeLinks();
			} break;

		case 2:
		case 3:	{
			LONG	ActionPoints;
			LONG	Triggers;
			LONG	Actions;

			ActionPoints = *((LONG *)FilePtr)++;
			Triggers = *((LONG *)FilePtr)++;
			Actions = *((LONG *)FilePtr)++;

			if (Version == 2)
				{
				while(ActionPoints--)
					{
					TRIGGERLINK *ActionPt = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
					if (!ActionPt) goto Error;
					ActionPt->node.type	= ((TRIGGERLINKV2 *)FilePtr)->node.type;
					ActionPt->player	= ((TRIGGERLINKV2 *)FilePtr)->player;
					ActionPt->area		= ((TRIGGERLINKV2 *)FilePtr)->area;
					ActionPt->creature	= ((TRIGGERLINKV2 *)FilePtr)->creature;
					ActionPt->band		= ((TRIGGERLINKV2 *)FilePtr)->band;
					ActionPt->trigger	= ((TRIGGERLINKV2 *)FilePtr)->trigger;
					List_AddEntry((NODE *)ActionPt,&MapActionPoints);
					FilePtr += sizeof(TRIGGERLINKV2);
					}
				}
			else
				{
				while(ActionPoints--)
					{
					TRIGGERLINK *ActionPt = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
					if (!ActionPt) goto Error;
					CopyMemory(ActionPt,FilePtr,sizeof(TRIGGERLINK));
					List_AddEntry((NODE *)ActionPt,&MapActionPoints);
					FilePtr += sizeof(TRIGGERLINK);
					}
				}

			while(Triggers--)
				{
				TRIGGER *Trigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
				if (!Trigger) goto Error;
				CopyMemory(Trigger,FilePtr,sizeof(TRIGGER));
				List_AddEntry((NODE *)Trigger,&MapTriggers);
				FilePtr += sizeof(TRIGGER);
				}

			while(Actions--)
				{
				TRIGGER *Action = GlobalAlloc(GPTR,sizeof(ACTION));
				if (!Action) goto Error;
				CopyMemory(Action,FilePtr,sizeof(ACTION));
				List_AddEntry((NODE *)Action,&MapActions);
				FilePtr += sizeof(ACTION);
				}

			CopyMemory(TriggersVariables,FilePtr,TRIGGERS_MAXVARIABLES*TRIGGERS_MAXVARLEN);
			FilePtr += TRIGGERS_MAXVARIABLES*TRIGGERS_MAXVARLEN;
			CopyMemory(TriggersTimers,FilePtr,TRIGGERS_MAXTIMERS*TRIGGERS_MAXTIMLEN);

			} break;
		}

	return(1);

//--- Erreur de chargement ---

Error:	Triggers_ReleaseMemory();
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des propriétés des éléments (variables globales)	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_ExtractGlobalVariables(BYTE *File, DWORD FileSize, NODE *Errors)
{
	BYTE		*FilePtr;
	LONG		 Version;
	DWORD		 Flags;

	FilePtr = Editor_FindHeader(FILEHEADER_VARIABLES,File,FileSize);
	if (!FilePtr) return(1);

	if (FilePtr == (BYTE *)-1)
		{
		Compiler_SetLastError(COMPILERR_UNKHEADER);
		Compiler_ReportErrorText(Errors,REPORT_BADHEADER,0);
		return(0);
		}

	Version = ((HEADER *)FilePtr)->Version;
	if (Version < DKGBLMINVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_OBSOLETEVERSION,0);
		return(0);
		}
	if (Version > DKGBLVERSION)
		{
		Compiler_SetLastError(COMPILERR_BADVERSION);
		Compiler_ReportErrorText(Errors,REPORT_BADVERSION,Version);
		return(0);
		}
	MapVersions.GlobalVersion = Version;

	FilePtr += sizeof(HEADER);

	Flags = *((DWORD *)FilePtr);
	FilePtr += 4;

	if (Flags&MGRF_TERRAINS)
		{
		CopyMemory(&MapGlobalVariables.Terrains[0],FilePtr,sizeof(MapGlobalVariables.Terrains));
		FilePtr += sizeof(MapGlobalVariables.Terrains);
		}
	if (Flags&MGRF_ROOMS)
		{
		CopyMemory(&MapGlobalVariables.Rooms[0],FilePtr,sizeof(MapGlobalVariables.Rooms));
		FilePtr += sizeof(MapGlobalVariables.Rooms);
		}
	if (Flags&MGRF_DOORS)
		{
		CopyMemory(&MapGlobalVariables.Doors[0],FilePtr,sizeof(MapGlobalVariables.Doors));
		FilePtr += sizeof(MapGlobalVariables.Doors);
		}
	if (Flags&MGRF_TRAPS)
		{
		CopyMemory(&MapGlobalVariables.Traps[0],FilePtr,sizeof(MapGlobalVariables.Traps));
		FilePtr += sizeof(MapGlobalVariables.Traps);
		}
	if (Flags&MGRF_SPELLS)
		{
		CopyMemory(&MapGlobalVariables.Spells[0],FilePtr,sizeof(MapGlobalVariables.Spells));
		FilePtr += sizeof(MapGlobalVariables.Spells);
		}
	if (Flags&(MGRF_CREATURES))
		{
		CopyMemory(&MapGlobalVariables.Creatures[0],FilePtr,sizeof(MapGlobalVariables.Creatures));
//		FilePtr += sizeof(MapGlobalVariables.Creatures);
		}

	return(1);
}



// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Editor_GenerateFile(HWND hWnd, char *FileName, BOOL OpenDiskWindow)
{
	char	 MapName[MAX_PATH];
	MAPINFO	 MapInfo;
	HANDLE	 FileHandle;
	ULONG	 Size;
	ULONG	 SizeKSTS;
	ULONG	 SizeKINF;
	ULONG	 SizeKPLY;
	ULONG	 SizeKMAP;
	ULONG	 SizeKTGS;
	ULONG	 SizeKREC;
	ULONG	 SizeKMTH;
	ULONG	 SizeKTRG;
	ULONG	 SizeKGBL;
	NODE	*Node;
	DWORD	*GameMap;
	char	*Point;

	Compiler_SetLastError(0);
	strcpy(MapName,FileName);
	Point = strrchr(MapName,'.');
	if (Point) *Point = 0;
	strcat(MapName,szEditorMapExt);

//--- Calcul la taille du fichier ---

	// Calcul la taille des statistiques
	//
	SizeKSTS = sizeof(MAPEDITSTATS);
	Size = sizeof(HEADER)+SizeKSTS;

	// Calcul la taille des informations primaires de la carte
	//
	SizeKINF = sizeof(MAPPROPERTIES);
	Size += sizeof(HEADER)+SizeKINF;

	// Calcul la taille des informations pour les joueurs
	//
	SizeKPLY = sizeof(MAPOPTIONS)*MAP_MAXPLAYERS;
	Size += sizeof(HEADER)+SizeKPLY;

	// Calcul la taille de la carte
	//
	SizeKMAP = 8+Map.Properties.Width*Map.Properties.Height*4;
	Size += sizeof(HEADER)+SizeKMAP;

	// Calcul la taille des objets
	//
	SizeKTGS  = List_EntryCount(&MapCreatures)*sizeof(MAPTHING);
	SizeKTGS += List_EntryCount(&MapDoors)*sizeof(MAPTHING);
	SizeKTGS += List_EntryCount(&MapTraps)*sizeof(MAPTHING);
	SizeKTGS += List_EntryCount(&MapObjects)*sizeof(MAPTHING);
	SizeKTGS += List_EntryCount(&MapMagicalObjects)*sizeof(MAPTHING);
	if (SizeKTGS) Size += sizeof(HEADER)+SizeKTGS;

	// Calcul la taille des zones & des passages
	//
	SizeKREC  = List_EntryCount(&MapAreas)*sizeof(MAPRECTEX);
	SizeKREC += List_EntryCount(&MapGates)*sizeof(MAPRECTEX);
	SizeKREC += List_EntryCount(&MapEffects)*sizeof(MAPRECTEX);
	if (SizeKREC) Size += sizeof(HEADER)+SizeKREC;

	// Calcul la taille des bandes de héros
	//
	SizeKMTH = List_EntryCount(&MapHerosBands)*sizeof(MTHBAND);
	if (SizeKMTH) Size += sizeof(HEADER)+SizeKMTH;

	// Calcul la taille des triggers
	//
	SizeKTRG = List_EntryCount(&MapActionPoints)*sizeof(TRIGGERLINK)+List_EntryCount(&MapTriggers)*sizeof(TRIGGER)+List_EntryCount(&MapActions)*sizeof(ACTION)+TRIGGERS_MAXVARIABLES*TRIGGERS_MAXVARLEN+TRIGGERS_MAXTIMERS*TRIGGERS_MAXTIMLEN;
	if (SizeKTRG) Size += sizeof(HEADER)+12+SizeKTRG;

	// Calcul la taille des propriétés des éléments
	//
	SizeKGBL  = Properties_GlobalChanged(&MapGlobalVariables,MGRF_TERRAINS)*sizeof(MapGlobalVariables.Terrains);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_ROOMS)*sizeof(MapGlobalVariables.Rooms);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_DOORS)*sizeof(MapGlobalVariables.Doors);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_TRAPS)*sizeof(MapGlobalVariables.Traps);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_SPELLS)*sizeof(MapGlobalVariables.Spells);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURES)*sizeof(MapGlobalVariables.Creatures);
	SizeKGBL += Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURESPELLS)*sizeof(MapGlobalVariables.CrSpells);
	if (SizeKGBL) Size += sizeof(HEADER)+4+SizeKGBL;

//--- Création du fichier ---

	ZeroMemory(&MapInfo,sizeof(MAPINFO));
	MapInfo.Map = Map.Map;
	MapInfo.Width = Map.Properties.Width;
	MapInfo.Height = Map.Properties.Height;
	GameMap = Compiler_EditorMapToGameMap(&MapInfo);
	if (!GameMap) goto Error_0;

	FileHandle = Files_Create(MapName,FILEHEADER_MAPFILE,DKMVERSION,Size);
	if (!FileHandle) goto Error_1;

	if (!OpenDiskWindow)
		SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_INFOPART,(LPARAM)szDiskEditorSaveTemp);
	else
		Disk_CreateWindow(DISK_EDITORSAVE);

/* Sauvegarde des statistiques */

	Disk_Update(DISK_SEPRSTATS);
	MapStats.SavedTimes++;
	MapStats.RealTicks = MapStats.TotalTicks+(GetTickCount()-TickCount);
	GetLocalTime(&MapStats.LastSaved);

	if (!Files_WriteHeader(FileHandle,FILEHEADER_STATS,DKSTSVERSION,SizeKSTS))
		goto Error_2;

	if (!Files_WriteData(FileHandle,&MapStats,sizeof(MAPEDITSTATS),DATATYPE_BINARY))
		goto Error_2;

/* Sauvegarde les informations primaires de la carte */

	Disk_Update(DISK_SEPRMAPINFO);
	if (!Files_WriteHeader(FileHandle,FILEHEADER_MAPINFO,DKINFVERSION,SizeKINF))
		goto Error_2;

	if (!Files_WriteData(FileHandle,&Map.Properties,sizeof(MAPPROPERTIES),DATATYPE_BINARY))
		goto Error_2;

/* Sauvegarde les informations des joueurs */

	if (OpenDiskWindow)
		Disk_Update(DISK_SEPRPLAYERSINFO);

	if (!Files_WriteHeader(FileHandle,FILEHEADER_MAPPLAYERS,DKPLYVERSION,SizeKPLY))
		goto Error_2;

	if (!Files_WriteData(FileHandle,MapOptions,SizeKPLY,DATATYPE_BINARY))
		goto Error_2;

/* Sauvegarde la carte */

	if (OpenDiskWindow)
		Disk_Update(DISK_SEPRMAP);

	if (!Files_WriteHeader(FileHandle,FILEHEADER_MAP,DKMAPVERSION,SizeKMAP))
		goto Error_2;

	if (!Files_WriteData(FileHandle,&Map.Properties.Width,4,DATATYPE_BINARY))
		goto Error_2;
	if (!Files_WriteData(FileHandle,&Map.Properties.Height,4,DATATYPE_BINARY))
		goto Error_2;

	if (!Files_WriteData(FileHandle,GameMap,SizeKMAP-8,DATATYPE_BINARY))
		goto Error_2;

/* Sauvegarde les éléments */

	if (SizeKTGS)
		{
		if (OpenDiskWindow)
			Disk_Update(DISK_SEPRTHINGS);

		if (!Files_WriteHeader(FileHandle,FILEHEADER_THINGS,DKTGSVERSION,SizeKTGS))
			goto Error_2;

		for (Node = MapCreatures.next; Node != 0; Node = Node->next)
			{
			if (((MAPTHING *)Node)->owner != 1)
				{
				((MAPTHING *)Node)->objective = 0;
				((MAPTHING *)Node)->area = 0;
				((MAPTHING *)Node)->ennemy = 0;
				((MAPTHING *)Node)->herosoptions = 0;
				}
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPTHING),DATATYPE_NODE))
				goto Error_2;
			}
		for (Node = MapDoors.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPTHING),DATATYPE_NODE))
				goto Error_2;
		for (Node = MapTraps.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPTHING),DATATYPE_NODE))
				goto Error_2;
		for (Node = MapObjects.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPTHING),DATATYPE_NODE))
				goto Error_2;
		for (Node = MapMagicalObjects.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPTHING),DATATYPE_NODE))
				goto Error_2;
		}

/* Sauvegarde les zones et les passages */

	if (SizeKREC)
		{
		if (OpenDiskWindow)
			Disk_Update(DISK_SEPRRECTEX);

		if (!Files_WriteHeader(FileHandle,FILEHEADER_MAPRECTEX,DKRECVERSION,SizeKREC))
			goto Error_2;

		for (Node = MapAreas.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPRECTEX),DATATYPE_NODE))
				goto Error_2;
		for (Node = MapGates.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPRECTEX),DATATYPE_NODE))
				goto Error_2;
		for (Node = MapEffects.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MAPRECTEX),DATATYPE_NODE))
				goto Error_2;
		}

/* Sauvegarde les bandes de héros */

	if (SizeKMTH)
		{
		if (OpenDiskWindow)
			Disk_Update(DISK_SEPRBANDS);

		if (!Files_WriteHeader(FileHandle,FILEHEADER_HEROS,DKMTHVERSION,SizeKMTH))
			goto Error_2;

		for (Node = MapHerosBands.next; Node != 0; Node = Node->next)
			if (!Files_WriteData(FileHandle,Node,sizeof(MTHBAND),DATATYPE_NODE))
				goto Error_2;
		}



/* Sauvegarde les évènements */

	if (SizeKTRG)
		{
		TRIGGERLINK	*ActionPoint;
		TRIGGER		*Trigger;
		ACTION		*Action;
		long		 Data[3];

		if (OpenDiskWindow)
			Disk_Update(DISK_SEPRTRIGGERS);

		if (!Files_WriteHeader(FileHandle,FILEHEADER_TRIGGERS,DKTRGVERSION,SizeKTRG+12))
			goto Error_2;

		Data[0] = List_EntryCount(&MapActionPoints);
		Data[1] = List_EntryCount(&MapTriggers);
		Data[2] = List_EntryCount(&MapActions);

		if (!Files_WriteData(FileHandle,&Data[0],12,DATATYPE_BINARY))
			goto Error_2;

		for (ActionPoint = (TRIGGERLINK *)MapActionPoints.next; ActionPoint != 0; ActionPoint = (TRIGGERLINK *)ActionPoint->node.next)
			{
			if (!Files_WriteData(FileHandle,ActionPoint,sizeof(TRIGGERLINK),DATATYPE_NODE))
				goto Error_2;
			}

		for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
			{
			if (!Files_WriteData(FileHandle,Trigger,sizeof(TRIGGER),DATATYPE_NODE))
				goto Error_2;
			}

		for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
			{
			if (!Files_WriteData(FileHandle,Action,sizeof(ACTION),DATATYPE_NODE))
				goto Error_2;
			}

		if (!Files_WriteData(FileHandle,TriggersVariables,TRIGGERS_MAXVARIABLES*TRIGGERS_MAXVARLEN,DATATYPE_BINARY))
			goto Error_2;

		if (!Files_WriteData(FileHandle,TriggersTimers,TRIGGERS_MAXTIMERS*TRIGGERS_MAXTIMLEN,DATATYPE_BINARY))
			goto Error_2;
		}

/* Sauvegarde des propriétés des éléments */

	if (SizeKGBL)
		{
		DWORD	Flags;

		if (OpenDiskWindow)
			Disk_Update(DISK_SEPRVARIABLES);

		if (!Files_WriteHeader(FileHandle,FILEHEADER_VARIABLES,DKGBLVERSION,SizeKGBL+4))
			goto Error_2;

		Flags = 0;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_TERRAINS))	Flags |= MGRF_TERRAINS;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_ROOMS))		Flags |= MGRF_ROOMS;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_DOORS))		Flags |= MGRF_DOORS;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_TRAPS))		Flags |= MGRF_TRAPS;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_SPELLS))		Flags |= MGRF_SPELLS;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURES))	Flags |= MGRF_CREATURES;
		if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURESPELLS))	Flags |= MGRF_CREATURESPELLS;

		if (!Files_WriteData(FileHandle,&Flags,4,DATATYPE_BINARY))
			goto Error_2;

		if (Flags&MGRF_TERRAINS)
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Terrains[0],sizeof(MapGlobalVariables.Terrains),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&MGRF_ROOMS)
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Rooms[0],sizeof(MapGlobalVariables.Rooms),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&MGRF_DOORS)
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Doors[0],sizeof(MapGlobalVariables.Doors),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&MGRF_TRAPS)
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Traps[0],sizeof(MapGlobalVariables.Traps),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&MGRF_SPELLS)
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Spells[0],sizeof(MapGlobalVariables.Spells),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&(MGRF_CREATURES))
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.Creatures[0],sizeof(MapGlobalVariables.Creatures),DATATYPE_BINARY))
				goto Error_2;
		if (Flags&(MGRF_CREATURESPELLS))
			if (!Files_WriteData(FileHandle,&MapGlobalVariables.CrSpells[0],sizeof(MapGlobalVariables.CrSpells),DATATYPE_BINARY))
				goto Error_2;
		}

//--- Sauvegarde terminée ---

	if (OpenDiskWindow)
		{
		Disk_Update(DISK_SEPRDONE);
		Disk_DestroyWindow(TRUE);
		}
	else
		SendMessage(hStatusWnd,SB_SETTEXT,(WPARAM)STATUS_INFOPART,(LPARAM)szDiskEditorSaveTempDone);

	CloseHandle(FileHandle);
	GlobalFree(GameMap);

	return(1);

//--- Erreurs de sauvegarde ---

Error_2:Disk_DestroyWindow(FALSE);
	CloseHandle(FileHandle);
	DeleteFile(MapName);
Error_1:GlobalFree(GameMap);
Error_0:strcpy(MapName,szSaveMapErr);
	strcat(MapName,Compiler_GetErrorCodeText());
	Misc_MessageBoxExt(hWnd,MapName,NULL,szMessageBoxExtOk,MB_ICONHAND|MB_OK);
	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
