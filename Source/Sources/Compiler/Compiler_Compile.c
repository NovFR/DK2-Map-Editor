
//<<>-<>>---------------------------------------------------------------------()
/*
	Compilation de la carte
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "..\..\Includes\Editor.h"
#include "..\..\Includes\Prototypes.h"
#include "..\..\Includes\Structures.h"
#include "..\..\Includes\Texts.h"

extern COMPILERTASK*	CompilerTask;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapAreas;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern MAPEDITSTATS	MapStats;
extern MAPOPTIONS	MapOptions[MAP_MAXPLAYERS];
extern MAP		Map;
extern GLOBALVARS	MapGlobalVariables;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Méthodes de compilation						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Compile la carte ««««««««««««««««««««««««««««««««««««««««««««««««»

void Compiler_CompileMap(HWND hWnd)
{
	char	*MapName;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}

	if (Config_GetGamePath(MapName))
		{
		strcpy(MapName,Config.GamePath);
		strcat(MapName,szMapsPath);
		strcat(MapName,Map.Properties.Name);
		Compiler_BeginTask(MapName,COMPILER_COMPILE);
		}

	GlobalFree(MapName);
	return;
}


// «»»» Test la carte «««««««««««««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
void Compiler_TestMap(HWND hWnd)
{
	Compiler_BeginTask(NULL,COMPILER_TEST);
	return;
}


// «»»» Exécute la carte ««««««««««««««««««««««««««««««««««««««««««««««««»

void Compiler_RunMap(HWND hWnd)
{
	char	*MapName;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}

	if (Config_GetGamePath(MapName))
		{
		strcpy(MapName,Config.GamePath);
		strcat(MapName,szMapsPath);
		strcat(MapName,Map.Properties.Name);
		Compiler_BeginTask(MapName,COMPILER_RUN);
		}

	GlobalFree(MapName);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Tâche gérant la compilation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Compiler_BeginTask(char *MapName, UINT Method)
{
	if (Compiler_CreateWindow(MapName,Method))
		{
		int		 Thread;

		Thread = _beginthread(Compiler_Task,4096,CompilerTask);
		if (Thread == (ULONG)-1)
			{
			Compiler_SetLastError(COMPILERR_WINDOWS);
			SendMessage(CompilerTask->hWnd,WM_REPORTERROR,(WPARAM)REPORT_LASTERROR,(LPARAM)0);
			CompilerTask->Failed = TRUE;
			}
		}
	return;
}


// «»»» Tâche de fond «««««««««««««««««««««««««««««««««««««««««««««««««««»

void Compiler_Task(void *Task)
{
	SendMessage(((COMPILERTASK *)Task)->hWnd,WM_DISABLE,(WPARAM)TRUE,(LPARAM)0);

	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMBEGIN);
	SendMessage(((COMPILERTASK *)Task)->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)-1);
	Compiler_TaskTest((COMPILERTASK *)Task);
	Compiler_TaskGenerate((COMPILERTASK *)Task);
	Compiler_TaskEndText((COMPILERTASK *)Task);

	GetLocalTime(&MapStats.CompileTime);
	SendMessage(((COMPILERTASK *)Task)->hWnd,WM_DISABLE,(WPARAM)FALSE,(LPARAM)0);
	return;
}


// «»»» Affichage du texte de fin de compilation ««««««««««««««««««««««««»

void Compiler_TaskEndText(COMPILERTASK *Task)
{
	if (!Task->Failed) SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerDone);
	if (Task->Failed) SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAborted);
	else if (Task->Warnings) SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)Task->Warnings,(LPARAM)-2);
	else SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNoError);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Compilation des fichiers						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Compiler_TaskGenerate(COMPILERTASK *Task)
{
	char	*Extension;

	if (!Task->Generate) return;
	if (Task->Failed) return;

	strcpy(Task->MapKill,Task->MapPath);
	Extension = strrchr(Task->MapKill,'.');
	if (Extension) *Extension = 0;
	else Extension = strchr(Task->MapKill,0);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWriting);
	if (Compiler_TaskIsReserved(Map.Properties.Name))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_RESERVED,(LPARAM)0);
		Task->Failed = TRUE;
		return;
		}

	//
	// --- Génération des fichiers de la carte ---
	//

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingInfo);
	if (!Compiler_GenerateInfoFile(Task->MapPath)) goto Error_0;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEINF);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingPlayers);
	if (!Compiler_GeneratePlayersFile(Task->MapPath)) goto Error_1;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEPLY);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingThings);
	if (!Compiler_GenerateThingsFile(Task->MapPath)) goto Error_2;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITETHS);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingMap);
	if (!Compiler_GenerateMapFile(Task->MapPath)) goto Error_3;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEMAP);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingVariables);
	if (!Compiler_GenerateVariablesFile(Task->MapPath)) goto Error_4;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEVAR);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingTriggers);
	if (!Compiler_GenerateTriggersFile(Task->MapPath)) goto Error_5;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITETRS);

	//
	// --- Génération des variables globales ---
	//

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingTerrains);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_TERRAINS))
		{
		if (!Compiler_GenerateTerrainsFile(Task->MapPath)) goto Error_6;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameTerrainExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLT);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingRooms);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_ROOMS))
		{
		if (!Compiler_GenerateRoomsFile(Task->MapPath)) goto Error_7;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameRoomsExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLR);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingDoors);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_DOORS))
		{
		if (!Compiler_GenerateDoorsFile(Task->MapPath)) goto Error_8;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameDoorsExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLD);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingTraps);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_TRAPS))
		{
		if (!Compiler_GenerateTrapsFile(Task->MapPath)) goto Error_9;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameTrapsExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLA);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingSpells);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_SPELLS))
		{
		if (!Compiler_GenerateSpellsFile(Task->MapPath)) goto Error_A;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameKeeperSpellsExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLS);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingCreatures);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURES))
		{
		if (!Compiler_GenerateCreaturesFile(Task->MapPath)) goto Error_B;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameCreaturesExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLC);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerWritingCreatureSpells);
	if (Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURESPELLS))
		{
		if (!Compiler_GenerateCreatureSpellsFile(Task->MapPath)) goto Error_C;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		}
	else
		{
		strcat(Task->MapKill,szGameCreatureSpellsExt);
		DeleteFile(Task->MapKill);
		*Extension = 0;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerNotNeeded);
		}
	SendMessage(CompilerTask->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMWRITEGLO);

	//
	// --- Fin de l'écriture ---
	//

	SendMessage(CompilerTask->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
	return;

//--- Erreurs d'écritures ---

Error_C:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGCRSPELLS,(LPARAM)0);
	goto Error;
Error_B:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGCREATURES,(LPARAM)0);
	goto Error;
Error_A:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGSPELLS,(LPARAM)0);
	goto Error;
Error_9:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGTRAPS,(LPARAM)0);
	goto Error;
Error_8:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGDOORS,(LPARAM)0);
	goto Error;
Error_7:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGROOMS,(LPARAM)0);
	goto Error;
Error_6:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEGTERRAINS,(LPARAM)0);
	goto Error;
Error_5:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVETRIGGERS,(LPARAM)0);
	goto Error;
Error_4:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEVARIABLES,(LPARAM)0);
	goto Error;
Error_3:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEMAP,(LPARAM)0);
	goto Error;
Error_2:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVETHINGS,(LPARAM)0);
	goto Error;
Error_1:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEPLAYERS,(LPARAM)0);
	goto Error;
Error_0:SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_SAVEINFO,(LPARAM)0);

Error:	SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_LASTERROR,(LPARAM)0);
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
	strcat(Task->MapKill,szGameMainExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameMapExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGamePlayersExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameThingsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameTriggersExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameVariablesExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameTerrainExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameRoomsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameDoorsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameTrapsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameKeeperSpellsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameCreaturesExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	strcat(Task->MapKill,szGameCreatureSpellsExt);
	DeleteFile(Task->MapKill);
	*Extension = 0;
	Task->Failed = TRUE;
	SendMessage(CompilerTask->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
	return;
}


// «»»» Test si le nom de la carte est réservé ««««««««««««««««««««««««««»

int Compiler_TaskIsReserved(char *MapName)
{
	char	*ReservedNames[]={	"level1",
					"level2",
					"level3",
					"level4",
					"level5",
					"level6a",
					"level6b",
					"level7",
					"level8",
					"level9",
					"level10",
					"level11a",
					"level11b",
					"level11c",
					"level12",
					"level13",
					"level14",
					"level15a",
					"level15b",
					"level16",
					"level17",
					"level18",
					"level19",
					"level20",
					"mpd1",
					"mpd2",
					"mpd3",
					"mpd4",
					"mpd5",
					"mpd6",
					"mpd7",
					"secret1",
					"secret2",
					"secret3",
					"secret4",
					"secret5",
					"alcatraz",
					"bottleneck",
					"caverns",
					"circlet",
					"clover",
					"frosty's castle",
					"frosty's lair",
					"frosty's realm",
					"furnace",
					"hopping",
					"king of the hill",
					"ladder",
					"lavalsles",
					"no mans land",
					"olympia",
					"onslaught",
					"patrol",
					"platform",
					"pond",
					"pressure",
					"siege",
					"swiss cheese",
					"the deep end",
					"the pass",
					"thelabyrinth",
					"u turn",
					"warrens",
					NULL};
	int	 i;

	for (i = 0; ReservedNames[i] != NULL; i++)
		if (!strcmpi(MapName,ReservedNames[i])) return(1);

	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Effectue la série de tests de contrôle				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
void Compiler_TaskTest(COMPILERTASK *Task)
{
	SendMessage(CompilerTask->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyze);
	Compiler_TaskTestVersion(Task);
	Compiler_TaskTestMap(Task);
	Compiler_TaskTestPlayers(Task);
	Compiler_TaskTestThings(Task);
	Compiler_TaskTestAreas(Task);
	Compiler_TaskTestGates(Task);
	Compiler_TaskTestTriggers(Task);
	if ((Compiler_TaskIsReserved(Map.Properties.Name))&&(Task->Method == COMPILER_TEST)) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_RESERVED2,(LPARAM)0);
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
	if (!Task->Failed) SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLDONE);
	return;
}


// «»»» Test les problèmes de compatibilité «««««««««««««««««««««««««««««»

void Compiler_TaskTestVersion(COMPILERTASK *Task)
{
	MAPTHING	*Thing;
	TRIGGER		*Trigger;
	ACTION		*Action;
	DWORD		 Flags;

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeVersion);
	Flags = 0;

	for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		if ((Thing->id >= 0x20)&&(Thing->id <= 0x2C)) Flags |= 0x00000001;
		if (Thing->id == 0x2D) Flags |= 0x00000002;
		}

	for (Thing = (MAPTHING *)MapTraps.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		if (Thing->id == 0x10) Flags |= 0x00000002;

	for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
		{
		if (Trigger->ctrl == TRIGGER_CREATURES)
			{
			// Les créatures élites ne fonctionnent pas avec ce déclencheur
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		if (Trigger->ctrl == TRIGGER_CREAENTERAREA)
			{
			if ((Trigger->param2 >= 0x20)&&(Trigger->param2 <= 0x2C)) Flags |= 0x00000010;
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		if (Trigger->ctrl == TRIGGER_HAPPYCREATURES)
			{
			if ((Trigger->param2 >= 0x20)&&(Trigger->param2 <= 0x2C)) Flags |= 0x00000010;
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		if (Trigger->ctrl == TRIGGER_LOSTCREATURES)
			{
			if ((Trigger->param2 >= 0x20)&&(Trigger->param2 <= 0x2C)) Flags |= 0x00000010;
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		if (Trigger->ctrl == TRIGGER_TRAPS)
			{
			if (Trigger->param2 == 0x10) Flags |= 0x00000010;
			}
		if (Trigger->ctrl == TRIGGER_CREATURESLEVEL)
			{
			if ((Trigger->param2 >= 0x20)&&(Trigger->param2 <= 0x2C)) Flags |= 0x00000010;
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		if (Trigger->ctrl == TRIGGER_GROUPINPOSSESS)
			{
			if ((Trigger->param2 >= 0x20)&&(Trigger->param2 <= 0x2C)) Flags |= 0x00000010;
			if (Trigger->param2 == 0x2D) Flags |= 0x00000020;
			}
		}

	for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
		{
		if (Action->ctrl == ACTION_ADDCREATURE)
			{
			if ((Action->param1 >= 0x20)&&(Action->param1 <= 0x2C)) Flags |= 0x00000100;
			if (Action->param1 == 0x2D) Flags |= 0x00000200;
			}
		}

	if (Map.Properties.Version < 16)
		{
		if (Flags&0x00000001) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_THING61,(LPARAM)0);
		if (Flags&0x00000010) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_TRIGGER61,(LPARAM)0);
		if (Flags&0x00000100) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACTION61,(LPARAM)0);
		}
	if (Map.Properties.Version < 17)
		{
		if (Flags&0x00000002) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_THING70,(LPARAM)0);
		if (Flags&0x00000020) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_TRIGGER70,(LPARAM)0);
		if (Flags&0x00000200) SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACTION70,(LPARAM)0);
		}

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLVER);
	return;
}


// «»»» Test les erreurs contenues dans la carte ««««««««««««««««««««««««»

void Compiler_TaskTestMap(COMPILERTASK *Task)
{
	BYTE	*MapCtrl;
	BYTE	*MapPtr;
	BYTE	*MapPtrLine;
	BYTE	 Offset;
	BYTE	 Player;
	BYTE	 Table;
	BYTE	 Hearts;
	LONG	 X,Y;

	if (Task->Failed) return;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeMap);

	//--- Test les structures vitales du donjon ---

	MapCtrl = Task->MapCtrl;
	MapPtr = Map.Map;
	Hearts = 0x00000000;

	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapPtrLine = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapPtrLine += MAP_CONTROLBYTES, MapCtrl++)
			{
			if (*MapCtrl) continue;
			if (Compiler_TaskTestMapInRect(X,Y,Task)) continue;

			Offset = *(MapPtrLine+0);
			Player = *(MapPtrLine+1);
			Table = *(MapPtrLine+3);

			switch(Table)
				{
				case 1:	if (Rooms[Offset].Id == 0x0E000101)
						{
						if ((Hearts&(1<<Player))&&(Player != 1)&&(Player != 2))
							{
							Compiler_TaskTestMapExpand(X,Y,Rooms,Table,REPORT_DUPLICATEDHEART,Task);
							Task->Failed = TRUE;
							goto Done;
							}
						Hearts |= (BYTE)(1<<Player);
						Compiler_TaskTestMapRect(X,Y,Rooms,Table,REPORT_BADHEARTSIZE,Task);
						break;
						}
					if ((Rooms[Offset].Id == 0x28000101)||(Rooms[Offset].Id == 0x0C000101))
						{
						Compiler_TaskTestMapRect(X,Y,Rooms,Table,REPORT_BADPORTALSIZE,Task);
						break;
						}
					break;
				case 2:	Compiler_TaskTestMapRect(X,Y,Gates,Table,REPORT_BADGATESIZE,Task);
					break;
				}

			if (Task->Failed) goto Done;
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

Done:	if (!Hearts)
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_NOHEART,(LPARAM)0);
		Task->Failed = TRUE;
		}
	else if (!(Hearts&(1<<3)))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_NOPLAYER1,(LPARAM)0);
		Task->Failed = TRUE;
		}
	else if ((!(Hearts&(1<<4)))&&(Hearts&((1<<5)|(1<<6)|(1<<7))))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_NOPLAYER2,(LPARAM)0);
		Task->Failed = TRUE;
		}
	else if ((!(Hearts&(1<<5)))&&(Hearts&((1<<6)|(1<<7))))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_NOPLAYER3,(LPARAM)0);
		Task->Failed = TRUE;
		}
	else if ((!(Hearts&(1<<6)))&&(Hearts&(1<<7)))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_NOPLAYER4,(LPARAM)0);
		Task->Failed = TRUE;
		}

	if (!Task->Failed)
		{
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLMAP);
		}
	else
		{
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
		}

	return;
}

//--- Test un rectangle dans la carte ---

void Compiler_TaskTestMapRect(LONG X, LONG Y, ELEMENT *Table, DWORD TableID, UINT ErrorCode, COMPILERTASK *Task)
{
	BYTE	*MapPtr;
	BYTE	*MapPtrLine;
	DWORD	 BaseID;
	DWORD	 BaseOffset;
	LONG	 AvgWidth;
	LONG	 Width;
	LONG	 Height;
	LONG	 X2,Y2;

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	BaseOffset = *MapPtr;
	BaseID = Table[*MapPtr].Id;
	AvgWidth = 65535;

	for (Height = 0, Y2 = Y; Y2 != Map.Properties.Height; Y2++, Height++)
		{
		MapPtrLine = MapPtr;
		for (Width = 0, X2 = X; X2 != Map.Properties.Width; X2++, MapPtrLine += MAP_CONTROLBYTES, Width++)
			{
			if (*(MapPtrLine+3) != TableID) break;
			if (Table[*MapPtrLine].Id != BaseID) break;
			}
		if (!Width) break;
		if ((Table[BaseOffset].MinWidth != 0)&&(Width < Table[BaseOffset].MinWidth)) goto Bad_Size;
		if ((Table[BaseOffset].MaxWidth != 0)&&(Width > Table[BaseOffset].MaxWidth)) goto Bad_Size;
		if ((AvgWidth != 65535)&&(Width != AvgWidth)) goto Bad_Rect;
		if (Width < AvgWidth) AvgWidth = Width;
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	for (Y2 = Y; Y2 != Y+Height; Y2++)
		for (X2 = X; X2 != X+AvgWidth; X2++)
			*(Task->MapCtrl+X2+Y2*Map.Properties.Width) = 0xFF;

	if ((Table[BaseOffset].MinHeight != 0)&&(Height < Table[BaseOffset].MinHeight)) goto Bad_Size;
	if ((Table[BaseOffset].MaxHeight != 0)&&(Height > Table[BaseOffset].MaxHeight)) goto Bad_Size;
	return;

Bad_Rect:
	Compiler_TaskTestMapExpand(X,Y,Table,TableID,REPORT_NOTASQUARE,Task);
	Task->Failed = TRUE;
	return;

Bad_Size:
	Compiler_TaskTestMapExpand(X,Y,Table,TableID,ErrorCode,Task);
	if (BaseID == 0x0E000101) Task->Failed = TRUE;
	else Task->Warnings++;
	return;
}

//--- Stockage du rectangle où a eu lieu l'erreur ---

void Compiler_TaskTestMapExpand(LONG X, LONG Y, ELEMENT *Table, DWORD TableID, UINT ErrorCode, COMPILERTASK *Task)
{
	ERRORNODE	 Error;
	BYTE		*MapPtr;
	BYTE		*MapPtrLine;
	DWORD		 BaseID;
	LONG		 Width;
	LONG		 X2,Y2;

	ZeroMemory(&Error,sizeof(ERRORNODE));
	Error.node.type = ErrorCode;
	Error.x = X;
	Error.y = Y;
	Error.width = 65535;
	Error.height = 1;

	MapPtr = Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES;
	BaseID = Table[*MapPtr].Id;

	for (Error.height = 0, Y2 = Y; Y2 != Map.Properties.Height; Y2++, Error.height++)
		{
		MapPtrLine = MapPtr;
		for (Width = 0, X2 = X; X2 != Map.Properties.Width; X2++, MapPtrLine += MAP_CONTROLBYTES, Width++)
			{
			if (*(MapPtrLine+3) != TableID) break;
			if (Table[*MapPtrLine].Id != BaseID) break;
			}
		if (!Width) break;
		if (Width < Error.width) Error.width = Width;
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

	SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
	return;
}

//--- Test si une erreur a déjà été répertoriée ---

int Compiler_TaskTestMapInRect(LONG X, LONG Y, COMPILERTASK *Task)
{
	ERRORNODE	*Error;

	for (Error = (ERRORNODE *)Task->Errors.next; Error != 0; Error = (ERRORNODE *)Error->node.next)
		{
		if (Error->x == -1) continue;
		if (X < Error->x) continue;
		if (Y < Error->y) continue;
		if (X > Error->x+Error->width-1) continue;
		if (Y > Error->y+Error->height-1) continue;
		return(1);
		}

	return(0);
}


// «»»» Test les options des joueurs ««««««««««««««««««««««««««««««««««««»

void Compiler_TaskTestPlayers(COMPILERTASK *Task)
{
	int	i;

	if (Task->Failed) return;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzePlayers);

	for (i = 3-1; i != MAP_MAXPLAYERS; i++)
		{
		if (((MapOptions[i].PlayerCameraX != -1)&&(MapOptions[i].PlayerCameraX >= Map.Properties.Width))||((MapOptions[i].PlayerCameraY != -1)&&(MapOptions[i].PlayerCameraY >= Map.Properties.Height)))
			{
			SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_CAMERA,(LPARAM)i-1);
			Task->Warnings++;
			}
		if (MapOptions[i].PlayerComputer)
			{
			if ((MapOptions[i].PlayerCPUAreaX1 >= Map.Properties.Width)||(MapOptions[i].PlayerCPUAreaX2 >= Map.Properties.Width)||(MapOptions[i].PlayerCPUAreaY1 >= Map.Properties.Height)||(MapOptions[i].PlayerCPUAreaY2 >= Map.Properties.Height))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_CPUAREA,(LPARAM)i-1);
				Task->Warnings++;
				}
			}
		}

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLPLAYERS);
	return;
}


// «»»» Test les problèmes en relation avec les objets ««««««««««««««««««»

void Compiler_TaskTestThings(COMPILERTASK *Task)
{
	ERRORNODE	 Error;
	MAPTHING	*Thing;
	int		 Result;

	if (Task->Failed) return;
	ZeroMemory(&Error,sizeof(ERRORNODE));
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeThings);
	Error.width = 1;
	Error.height = 1;
	Error.text = Task->Date;

	for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Error.node.type = REPORT_CREATUREPB;
		Error.x = Thing->x;
		Error.y = Thing->y;
		Result = Things_CanPaintEx(&Map,Thing->x,Thing->y,THING_CREATURE,Thing->id,Thing->owner,Error.text);
		if (Result == 1)
			continue;
		if (Result == 2)
			{
			Error.node.type = REPORT_CREATURETERRAIN;
			SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
			continue;
			}
		SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
		Task->Warnings++;
		}

	Error.node.type = REPORT_DOORPB;
	for (Thing = (MAPTHING *)MapDoors.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Error.x = Thing->x;
		Error.y = Thing->y;
		if (Things_CanPaintEx(&Map,Thing->x,Thing->y,THING_DOOR,Thing->id,Thing->owner,Error.text) != 1)
			{
			SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
			Task->Warnings++;
			}
		}

	Error.node.type = REPORT_TRAPPB;
	for (Thing = (MAPTHING *)MapTraps.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Error.x = Thing->x;
		Error.y = Thing->y;
		if (Things_CanPaintEx(&Map,Thing->x,Thing->y,THING_TRAP,Thing->id,Thing->owner,Error.text) != 1)
			{
			SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
			Task->Warnings++;
			}
		}

	for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Error.node.type = REPORT_OBJECTPB;
		Error.x = Thing->x;
		Error.y = Thing->y;
		Result = Things_CanPaintEx(&Map,Thing->x,Thing->y,THING_OBJECT,Thing->id,Thing->owner,Error.text);
		if (Result == 1)
			continue;
		if (Result == 2)
			{
			Error.node.type = REPORT_OBJECTTERRAIN;
			SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
			continue;
			}
		SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
		Task->Warnings++;
		}

	for (Thing = (MAPTHING *)MapMagicalObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Error.node.type = REPORT_MAGICPB;
		Error.x = Thing->x;
		Error.y = Thing->y;
		Result = Things_CanPaintEx(&Map,Thing->x,Thing->y,THING_MAGICALOBJECT,Thing->id,Thing->owner,Error.text);
		if (Result == 1)
			continue;
		if (Result == 2)
			{
			Error.node.type = REPORT_MAGICTERRAIN;
			SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
			continue;
			}
		SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
		Task->Warnings++;
		}

	if (Map.Properties.Version < 17)
		{
		for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			{
			Error.node.type = REPORT_THING70;
			Error.x = Thing->x;
			Error.y = Thing->y;
			switch(Thing->id)
				{
				case 0x8A:
				case 0x8B:
				case 0x8D:
				case 0x8E:
				case 0x8F:
				case 0x90:
					SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
					break;
				}
			}
		}

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLTHINGS);
	return;
}


// «»»» Test les problèmes en relation avec les zones «««««««««««««««««««»

void Compiler_TaskTestAreas(COMPILERTASK *Task)
{
	if (Task->Failed) return;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeAreas);

	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLAREAS);
	return;
}


// «»»» Test les problèmes en relation avec les passages ««««««««««««««««»

void Compiler_TaskTestGates(COMPILERTASK *Task)
{
	MAPRECTEXGATESLIST	*Gate;
	NODE			 GatesList;
	ERRORNODE	 	 Error;

	if (Task->Failed) return;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeGates);

	ZeroMemory(&Error,sizeof(ERRORNODE));
	if (!RectEx_GetGatesList(&GatesList))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_LASTERROR,(LPARAM)0);
		List_ReleaseMemory(&GatesList);
		Task->Failed = TRUE;
		return;
		}

	for (Gate = (MAPRECTEXGATESLIST *)GatesList.next; Gate != 0; Gate = (MAPRECTEXGATESLIST *)Gate->node.next)
		{
		Error.x = Gate->x;
		Error.y = Gate->y;
		Error.width = Gate->width;
		Error.height = Gate->height;

		if (Gate->node.type == 3)
			{
			if (Gate->cantbedestroyed) continue;

			if (!Gate->hasgate)
				{
				Error.node.type = REPORT_NOGATE22;
				SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
				continue;
				}
			if (!Gate->hasarea)
				{
				Error.x--;
				Error.y--;
				Error.width += 2;
				Error.height += 2;
				Error.node.type = REPORT_NOAREA22;
				SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
				continue;
				}
			if (!Gate->hasevent)
				{
				Error.node.type = REPORT_NOTDESTROYABLE;
				SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
				continue;
				}
			continue;
			}

		if (Gate->node.type == 6)
			{
			if (!Gate->hasgate)
				{
				Task->Warnings++;
				Error.node.type = REPORT_NOGATE6;
				SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
				continue;
				}
			continue;
			}

		if (Gate->node.type == 7)
			{
			if (!Gate->hasgate)
				{
				Task->Warnings++;
				Error.node.type = REPORT_NOGATE7;
				SendMessage(Task->hWnd,WM_REPORTERROREX,(WPARAM)0,(LPARAM)&Error);
				continue;
				}
			continue;
			}
		}

	List_ReleaseMemory(&GatesList);
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
	SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLGATES);
	return;
}


// «»»» Test les problèmes en relation avec les évènements ««««««««««««««»

void Compiler_TaskTestTriggers(COMPILERTASK *Task)
{
	TRIGGERLINK	*Link;
	TRIGGER		*Trigger;
	ACTION		*Action;
	LONG		 UnusedTriggers;
	LONG		 UnusedActions;
	int		 Result;

	if (Task->Failed) return;
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerAnalyzeTriggers);

	//--- Test les déclencheurs et les actions ---

	for (Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		Result = Compiler_TaskTestTrigger(Link->trigger,Task);
		if (Result == 1) continue;
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCRLF);
		Task->Failed = TRUE;
		break;
		}

	//--- Recherches les déclencheurs & actions inutilisées ---

	for (UnusedTriggers = 0, Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
		if (!(Trigger->node.type&0xFF000000)) UnusedTriggers++;
	for (UnusedActions = 0, Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
		if (!(Action->node.type&0xFF000000)) UnusedActions++;

	if (!Task->Failed)
		{
		if (UnusedTriggers)
			{
			SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_UNUSEDTRCNT,(LPARAM)UnusedTriggers);
			Task->Warnings++;
			}
		if (UnusedActions)
			{
			SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_UNUSEDACCNT,(LPARAM)UnusedActions);
			Task->Warnings++;
			}
		if ((UnusedTriggers)||(UnusedActions))
			{
			Result = Misc_MessageBoxExt(Task->hWnd,szCompilerUnusedEvents,(char *)-1,szMessageBoxExtYesNo,MB_ICONQUESTION);
			if (Result)
				{
				for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0;)
					{
					if (!(Trigger->node.type&0xFF000000))
						{
						List_RemEntry((NODE *)Trigger);
						GlobalFree(Trigger);
						Trigger = (TRIGGER *)MapTriggers.next;
						continue;
						}
					Trigger = (TRIGGER *)Trigger->node.next;
					}

				for (Action = (ACTION *)MapActions.next; Action != 0;)
					{
					if (!(Action->node.type&0xFF000000))
						{
						List_RemEntry((NODE *)Action);
						GlobalFree(Action);
						Action = (ACTION *)MapActions.next;
						continue;
						}
					Action = (ACTION *)Action->node.next;
					}
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_UNUSEDREM,(LPARAM)0);
				}
			}
		}

	//--- Réinitialise les données des évènements ---

	for (Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		Compiler_TaskTestResetTrigger(Link->trigger,Task);

	if (!Task->Failed)
		{
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);
		SendMessage(Task->hWnd,WM_UPDATEPROGRESS,(WPARAM)0,(LPARAM)COMPILER_PBMANLTRIGGERS);
		}

	//--- Réparation des évènements (dk2mapper) ---

	if (Task->RepairTriggers)
		{
		for (Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0;)
			{
			if (Link->node.type == SCOPE_AREA)
				{
				Link = (TRIGGERLINK *)Link->node.next;
				continue;
				}
			List_RemEntry((NODE *)Link);
			GlobalFree(Link);
			Link = (TRIGGERLINK *)MapActionPoints.next;
			}
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_REPAIRED,(LPARAM)0);
		}

	return;
}

//--- Réinitialise les déclencheurs ---

void Compiler_TaskTestResetTrigger(short TriggerId, COMPILERTASK *Task)
{
	TRIGGER	*TriggerPtr;

	if (!TriggerId) return;
	TriggerPtr = Triggers_GetTriggerById(TriggerId);
	if (!TriggerPtr) return;

	if ((!(TriggerPtr->node.type&0xFF000000))&&(!Task->Failed))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_TRSUNUSED,(LPARAM)0);
		Task->Warnings++;
		}

	TriggerPtr->node.type &= 0x00FFFFFF;

	if (TriggerPtr->or > 0) Compiler_TaskTestResetTrigger(TriggerPtr->or,Task);
	else if (TriggerPtr->or < 0) Compiler_TaskTestResetAction(TriggerPtr->or,Task);

	if (TriggerPtr->and > 0) Compiler_TaskTestResetTrigger(TriggerPtr->and,Task);
	else if (TriggerPtr->and < 0) Compiler_TaskTestResetAction(TriggerPtr->and,Task);

	return;
}

//--- Réinitialise les actions ---

void Compiler_TaskTestResetAction(short ActionId, COMPILERTASK *Task)
{
	ACTION	*ActionPtr;

	if (!ActionId) return;
	ActionPtr = Triggers_GetActionById(ActionId);
	if (!ActionPtr) return;

	if ((!(ActionPtr->node.type&0xFF000000))&&(!Task->Failed))
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSUNUSED,(LPARAM)0);
		Task->Warnings++;
		}

	ActionPtr->node.type &= 0x00FFFFFF;

	if (ActionPtr->next > 0) Compiler_TaskTestResetTrigger(ActionPtr->next,Task);
	else if (ActionPtr->next < 0) Compiler_TaskTestResetAction(ActionPtr->next,Task);

	return;
}

//--- Test les déclencheurs ---

int Compiler_TaskTestTrigger(short TriggerId, COMPILERTASK *Task)
{
	TRIGGER	*TriggerPtr;
	int	 Result;

	if (!TriggerId) return(1);
	TriggerPtr = Triggers_GetTriggerById(TriggerId);
	if (!TriggerPtr)
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_TRSMISSINGID,(LPARAM)0);
		return(0);
		}

	if (TriggerPtr->node.type&0xFF000000)
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_TRSALREADYUSED,(LPARAM)0);
		Result = Misc_MessageBoxExt(Task->hWnd,szCompilerTriggerEmergency,(char *)-1,szMessageBoxExtYesNo,MB_ICONQUESTION);
		if (Result) Task->RepairTriggers = TRUE;
		return(0);
		}

	TriggerPtr->node.type |= 0xFF000000;

	if (TriggerPtr->or > 0)
		{
		Result = Compiler_TaskTestTrigger(TriggerPtr->or,Task);
		if (Result != 1) return(Result);
		}
	else if (TriggerPtr->or < 0)
		{
		Result = Compiler_TaskTestAction(TriggerPtr->or,Task);
		if (Result != 1) return(Result);
		}

	if (TriggerPtr->and > 0)
		{
		Result = Compiler_TaskTestTrigger(TriggerPtr->and,Task);
		if (Result != 1) return(Result);
		}
	else if (TriggerPtr->and < 0)
		{
		Result = Compiler_TaskTestAction(TriggerPtr->and,Task);
		if (Result != 1) return(Result);
		}

	return(1);
}

//--- Test les actions ---

int Compiler_TaskTestAction(short ActionId, COMPILERTASK *Task)
{
	ACTION	*ActionPtr;
	int	 Result;

	if (!ActionId) return(1);
	ActionPtr = Triggers_GetActionById(ActionId);
	if (!ActionPtr)
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSMISSINGID,(LPARAM)0);
		return(0);
		}

	if (ActionPtr->node.type&0xFF000000)
		{
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSALREADYUSED,(LPARAM)0);
		return(0);
		}

	ActionPtr->node.type |= 0xFF000000;

	switch(ActionPtr->ctrl)
		{
		case ACTION_ADDCREATURE:
			if ((ActionPtr->param5 >= Map.Properties.Width)||(ActionPtr->param7 >= Map.Properties.Height))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSCROUTOFMAP,(LPARAM)0);
				return(0);
				}
			break;

		case ACTION_CHANGESQUARE:
			if ((ActionPtr->param5 >= Map.Properties.Width)||(ActionPtr->param7 >= Map.Properties.Height))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSSQOUTOFMAP,(LPARAM)0);
				return(0);
				}
			break;

		case ACTION_SHOWAREA:
		case ACTION_ENABLEFOG:
			if (!RectEx_FindById(ActionPtr->param1,&MapAreas))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_MISSINGAREA,(LPARAM)ActionPtr->param1);
				return(0);
				}
			break;

		case ACTION_MOVECAMERA:
		case ACTION_ROTATECAMERA:
			if (!RectEx_FindById(ActionPtr->param1,&MapAreas))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_MISSINGAREAPATH,(LPARAM)ActionPtr->param1);
				return(0);
				}
			break;

		case ACTION_CAMERAPATH:
			if (!RectEx_FindById(ActionPtr->param2,&MapAreas))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_MISSINGAREAPATH,(LPARAM)ActionPtr->param2);
				return(0);
				}
			break;

		case ACTION_CHANGEOWNER:
			if ((ActionPtr->param5 >= Map.Properties.Width)||(ActionPtr->param7 >= Map.Properties.Height))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSRMOUTOFMAP,(LPARAM)0);
				return(0);
				}
			break;

		case ACTION_SHOWCRYSTAL:
			if (Things_GetThingPos(&MapObjects,0x1C,0) == -1)
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_ACSNOCRYSTAL,(LPARAM)0);
				return(0);
				}
			break;

		case ACTION_RELEASEBAND:
			if ((ActionPtr->param2)&&(!(Map.Properties.Options2&0x20)))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_PETBAND,(LPARAM)0);
				return(0);
				}

			if (!Heros_FindHerosById(ActionPtr->param1))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_MISSINGBAND,(LPARAM)ActionPtr->param1);
				return(0);
				}

			if (!RectEx_FindById(ActionPtr->param5,&MapAreas))
				{
				SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_MISSINGAREA,(LPARAM)ActionPtr->param5);
				return(0);
				}
			break;
		}

	if (ActionPtr->next > 0)
		{
		Result = Compiler_TaskTestTrigger(ActionPtr->next,Task);
		if (Result != 1) return(Result);
		}
	else if (ActionPtr->next < 0)
		{
		Result = Compiler_TaskTestAction(ActionPtr->next,Task);
		if (Result != 1) return(Result);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Lancement de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Compiler_TaskRun(COMPILERTASK *Task)
{
	HINSTANCE	 GameModule;
	char		*GamePath;
	char		*GameParameters;

	SendMessage(Task->hWnd,WM_DISABLE,(WPARAM)TRUE,(LPARAM)0);
	SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerExecute);

	GamePath = GlobalAlloc(GPTR,MAX_PATH);
	if (!GamePath)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_LASTERROR,(LPARAM)0);
		SendMessage(Task->hWnd,WM_DISABLE,(WPARAM)FALSE,(LPARAM)0);
		return;
		}

	wsprintf(GamePath,szGameCommandLine,Config.GamePath,Map.Properties.Name);
	GameParameters = strchr(GamePath,'?');
	*GameParameters++ = 0;

	GameModule = ShellExecute(NULL,"open",GamePath,GameParameters,Config.GamePath,SW_SHOWNORMAL);
	if (GameModule <= (HINSTANCE)32)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		SendMessage(Task->hWnd,WM_REPORTERROR,(WPARAM)REPORT_LASTERROR,(LPARAM)0);
		}
	else
		SendMessage(Task->hWnd,WM_PRINTTEXT,(WPARAM)0,(LPARAM)szCompilerOk);

	GlobalFree(GamePath);
	SendMessage(Task->hWnd,WM_DISABLE,(WPARAM)FALSE,(LPARAM)0);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
