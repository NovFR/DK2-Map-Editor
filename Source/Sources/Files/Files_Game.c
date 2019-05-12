
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des fichiers du jeu.
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
extern MAPEDITSTATS	MapStats;
extern MAPFIND		MapFind;
extern GLOBALVARS	MapGlobalVariables;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Game_ExtractAll(HWND hWnd, char *FileName, NODE *Errors)
{
	MAPINFO		*GameMap;
	MAP		*NewMap;
	char		*Name;

	NewMap = GlobalAlloc(GPTR,sizeof(MAP));
	if (!NewMap) return(0);

//--- Extraction de la carte (seul information réellement requise) ---

	Disk_CreateWindow(DISK_GAMELOAD);
	Disk_Update(DISK_LGPRMAP);

	GameMap = Game_ExtractMap(FileName,Errors);
	if (!GameMap) goto Error_0;

	Properties_MapResetOptions(NewMap,&MapOptions[0]);
	Properties_GlobalReset(&MapGlobalVariables,-1,MGRF_ALL);
	NewMap->Map = GameMap->Map;
	NewMap->Properties.Width = GameMap->Width;
	NewMap->Properties.Height = GameMap->Height;
	Map_CreateLimitWall(NewMap);
	if (!Map_CreateDisplayContext(NewMap)) goto Error_1;

	// Place le nom de la carte (uniqt en cas de pb)
	//
	Name = strrchr(FileName,'\\');
	if (Name) strncpy(NewMap->Properties.Name,Name+1,63);
	else strncpy(NewMap->Properties.Name,FileName,63);
	Name = strrchr(NewMap->Properties.Name,'.');
	if (Name) *Name = 0;

	// Supprime l'ancienne carte
	//
	GlobalFree(GameMap);
	Map_Destroy(&Map);
	CopyMemory(&Map,NewMap,sizeof(MAP));
	GlobalFree(NewMap);
	Debug_ResetVersions(TRUE);
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
	Map_ResetStats(&MapStats);
	Triggers_ReleaseMemory();

//--- Chargement des autres objets ---

	Disk_Update(DISK_LGPRMAIN);
	if (!Game_ExtractMain(FileName,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_MAINFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LGPRPLAYERS);
	if (!Game_ExtractPlayers(FileName,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_PLAYERSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LGPRTHINGS);
	if (!Game_ExtractThings(FileName,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LGPRVARIABLES);
	if (!Game_ExtractVariables(FileName,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_VARSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LGPRTRIGGERS);
	if (!Game_ExtractTriggers(FileName,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_TRIGGERSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LGPRDONE);
	Disk_DestroyWindow(FALSE);
	return(1);

//--- Echec du chargement ---

Error_1:Compiler_SetLastError(COMPILERR_WINDOWS);
	Map_Destroy(NewMap);
	GlobalFree(NewMap);
	GlobalFree(GameMap);
	strcpy(FileName,szLoadMapErr);
	strcat(FileName,Compiler_GetErrorCodeText());
	Disk_DestroyWindow(FALSE);
	Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return(0);

Error_0:GlobalFree(NewMap);
	strcpy(FileName,szLoadMapErr);
	strcat(FileName,Compiler_GetErrorCodeText());
	Disk_DestroyWindow(FALSE);
	Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction du fichier principal					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
int Game_ExtractMain(char *FileName, NODE *Errors)
{
	BYTE	*MainFile;
	DWORD	 MainFileSize;
	WORD	*PadFix;
	char	*MainFileName;
	char	*Point;

	MainFileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MainFileName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	strcpy(MainFileName,FileName);
	Point = strrchr(MainFileName,'.');
	if (Point) *Point = 0;
	strcat(MainFileName,szGameMainExt);

	//--- Chargement du fichier complet ---

	MainFile = (BYTE *)Files_Load(MainFileName,&MainFileSize);
	if (!MainFile)
		{
		GlobalFree(MainFileName);
		return(0);
		}

	//--- Test les données ---

	if (((GAMEINFO *)MainFile)->dwMainFileHeader != 0x000000DC) goto Bad_Header;
	if (((GAMEINFO *)MainFile)->dwMainFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMEINFO *)MainFile)->dwMainInfoHeader != 0x000000DD) goto Bad_Header;

	MapStats.CreatedTime.wYear 	= ((GAMEINFO *)MainFile)->dateMainDate.wGlobalCreationYear;
	MapStats.CreatedTime.wMonth	= ((GAMEINFO *)MainFile)->dateMainDate.bGlobalCreationMonth;
	MapStats.CreatedTime.wDayOfWeek = -1;
	MapStats.CreatedTime.wDay	= ((GAMEINFO *)MainFile)->dateMainDate.bGlobalCreationDate;
	MapStats.CreatedTime.wHour	= ((GAMEINFO *)MainFile)->dateMainDate.bGlobalCreationHour;
	MapStats.CreatedTime.wMinute	= ((GAMEINFO *)MainFile)->dateMainDate.bGlobalCreationMinute;
	MapStats.CreatedTime.wSecond	= ((GAMEINFO *)MainFile)->dateMainDate.bGlobalCreationSeconds;
	CopyMemory(&MapStats.CompileTime,&MapStats.CreatedTime,sizeof(SYSTEMTIME));

	WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,((GAMEINFO *)MainFile)->wstrMainName,-1,Map.Properties.Name,64,NULL,NULL);
	WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,((GAMEINFO *)MainFile)->wstrMainDescription,-1,Map.Properties.Description,1024,NULL,NULL);
	WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,((GAMEINFO *)MainFile)->wstrMainAuthor,-1,Map.Properties.Author,64,NULL,NULL);
	WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,((GAMEINFO *)MainFile)->wstrMainEmail,-1,Map.Properties.Email,64,NULL,NULL);
	WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,((GAMEINFO *)MainFile)->wstrMainInformation,-1,Map.Properties.Information,1024,NULL,NULL);
	Map.Properties.GameTicks				= ((GAMEINFO *)MainFile)->bMainGameTicks;
	Map.Properties.Version					= 17;
	Map.Properties.Options					= ((GAMEINFO *)MainFile)->bMainOptions;
	Map.Properties.Options2					= ((GAMEINFO *)MainFile)->bMainOptions2;
	CopyMemory(&Map.Properties.SpeechFile[0],&((GAMEINFO *)MainFile)->szMainSpeech[0],32);
	Map.Properties.HornyTalismanPieces			= ((GAMEINFO *)MainFile)->bMainHornyTalismanPieces;
	Map.Properties.RewardPrev1				= ((GAMEINFO *)MainFile)->bMainRewardPrev1;
	Map.Properties.RewardPrev2				= ((GAMEINFO *)MainFile)->bMainRewardPrev2;
	Map.Properties.RewardPrev3				= ((GAMEINFO *)MainFile)->bMainRewardPrev3;
	Map.Properties.RewardNext1				= ((GAMEINFO *)MainFile)->bMainRewardNext1;
	Map.Properties.RewardNext2				= ((GAMEINFO *)MainFile)->bMainRewardNext2;
	Map.Properties.RewardNext3				= ((GAMEINFO *)MainFile)->bMainRewardNext3;
	Map.Properties.Soundtrack				= ((GAMEINFO *)MainFile)->bMainSoundtrack;
	Map.Properties.TextTableID				= ((GAMEINFO *)MainFile)->bMainTextTableID;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainTitleTextID[0];
	Map.Properties.TextTitleID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainTitlePlotTextID[0];
	Map.Properties.TextPlotID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainDebriefingTextID[0];
	Map.Properties.TextDebriefingID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainObjectiveTextID[0];
	Map.Properties.TextObjectiveID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainSubObjective3TextID[0];
	Map.Properties.TextSubObjective3ID			= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainSubObjective1TextID[0];
	Map.Properties.TextSubObjective1ID			= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainSubObjective2TextID[0];
	Map.Properties.TextSubObjective2ID			= *PadFix;
	Map.Properties.SpecialLevelNumber			= ((GAMEINFO *)MainFile)->bMainSpecialLevelNumber;
	Map.Properties.TextIntroCreatureID0			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID0;
	Map.Properties.TextIntroCreatureID1			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID1;
	Map.Properties.TextIntroCreatureID2			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID2;
	Map.Properties.TextIntroCreatureID3			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID3;
	Map.Properties.TextIntroCreatureID4			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID4;
	Map.Properties.TextIntroCreatureID5			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID5;
	Map.Properties.TextIntroCreatureID6			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID6;
	Map.Properties.TextIntroCreatureID7			= ((GAMEINFO *)MainFile)->bMainIntroCreatureID7;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID0[0];
	Map.Properties.TextIntroTextID0				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID1[0];
	Map.Properties.TextIntroTextID1				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID2[0];
	Map.Properties.TextIntroTextID2				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID3[0];
	Map.Properties.TextIntroTextID3				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID4[0];
	Map.Properties.TextIntroTextID4				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID5[0];
	Map.Properties.TextIntroTextID5				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID6[0];
	Map.Properties.TextIntroTextID6				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainIntroTextID7[0];
	Map.Properties.TextIntroTextID7				= *PadFix;
	Map.Properties.LevelNumber				= ((GAMEINFO *)MainFile)->bMainLevelNumber;
	Map.Properties.Players					= ((GAMEINFO *)MainFile)->bMainPlayers;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainPreLevelSpeechID[0];
	Map.Properties.SpeechPreLevelID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainWinSpeechID[0];
	Map.Properties.SpeechWinID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainLooseSpeechID[0];
	Map.Properties.SpeechLooseID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainNewsSpeechID[0];
	Map.Properties.SpeechNewsID				= *PadFix;
	PadFix							= (WORD *)&((GAMEINFO *)MainFile)->bMainGenPreLevelSpeechID[0];
	Map.Properties.SpeechGenericPreLevelID			= *PadFix;

	Triggers_CreateActionPtFromLevel(((GAMEINFO *)MainFile)->wMainTrigger);
	GlobalFree(MainFileName);
	GlobalFree(MainFile);
	return(1);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(MainFileName);
	GlobalFree(MainFile);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des joueurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Game_ExtractPlayers(char *FileName, NODE *Errors)
{
	BYTE	*Players;
	BYTE	*PlayersPtr;
	DWORD	 PlayersSize;
	char	*PlayersName;
	char	*Point;
	int	 PlayersMax;
	int	 PlayersCount;

	PlayersName = GlobalAlloc(GPTR,MAX_PATH);
	if (!PlayersName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	strcpy(PlayersName,FileName);
	Point = strrchr(PlayersName,'.');
	if (Point) *Point = 0;
	strcat(PlayersName,szGamePlayersExt);

	//--- Chargement du fichier complet ---

	Players = (BYTE *)Files_Load(PlayersName,&PlayersSize);
	if (!Players)
		{
		GlobalFree(PlayersName);
		return(0);
		}

	//--- Test les données ---

	if (((GAMEPLAYERS *)Players)->dwPlayersFileHeader != 0x000000B4) goto Bad_Header;
	if (((GAMEPLAYERS *)Players)->dwPlayersFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMEPLAYERS *)Players)->dwPlayersInfoHeader != 0x000000B5) goto Bad_Header;
	if (((GAMEPLAYERS *)Players)->dwPlayersDefinesHeader != 0x000000B6) goto Bad_Header;

	if (((GAMEPLAYERS *)Players)->dwPlayersDefinesSize != (Map.Properties.Players+2)*205)
		{
		Compiler_SetLastError(COMPILERR_BADSIZE);
		Compiler_ReportErrorText(Errors,REPORT_PLAYERS,0);
		}

	PlayersPtr = Players+sizeof(GAMEPLAYERS);
	PlayersMax = ((GAMEPLAYERS *)Players)->dwPlayersDefinesSize/205;
	while(PlayersMax)
		{
		PlayersCount = ((GAMEPLAYER *)PlayersPtr)->bGlobalPlayer;
		MapOptions[PlayersCount-1].PlayerGold					= (LONG)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerGold[0])));
		MapOptions[PlayersCount-1].PlayerComputer				= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCPU[0])));
		MapOptions[PlayersCount-1].PlayerAI					= (LONG)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerAI)));
		MapOptions[PlayersCount-1].PlayerSpeed					= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscSpeed)));
		MapOptions[PlayersCount-1].PlayerOpenness				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscOpenness)));
		MapOptions[PlayersCount-1].PlayerGoldValue				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscGold)));
		MapOptions[PlayersCount-1].PlayerManaValue				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscMana)));
		MapOptions[PlayersCount-1].PlayerMineGoldAmount				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscMineGoldUntil[0])));
		MapOptions[PlayersCount-1].PlayerMana					= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscStartingMana[0])));
		MapOptions[PlayersCount-1].PlayerLibraryUsage				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscLibrary)));
		MapOptions[PlayersCount-1].PlayerImpsToTilesRatio			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscImpsToTilesRatio)));
		MapOptions[PlayersCount-1].PlayerFindSpecialsExplore			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscChanceToFindSpecials)));
		MapOptions[PlayersCount-1].PlayerFindSpecialsChances			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscChanceOfFindingSpecials)));
		MapOptions[PlayersCount-1].PlayerFindSpecialsTiles			= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscTilesToFindSpecials[0])));
		MapOptions[PlayersCount-1].PlayerJailedFate				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerMiscJailedFate)));
		MapOptions[PlayersCount-1].PlayerResearchedRoom				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerResearchedRoom)));
		MapOptions[PlayersCount-1].PlayerRebuildRoom				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRebuildRoom)));
		MapOptions[PlayersCount-1].PlayerDefaultTiles				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerDefaultTiles)));
		MapOptions[PlayersCount-1].PlayerTilesBetweenRooms			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerTilesLeftBetweenRooms)));
		MapOptions[PlayersCount-1].PlayerDistance				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerDistance)));
		MapOptions[PlayersCount-1].PlayerCorridorStyle				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCorridorStyle)));
		MapOptions[PlayersCount-1].PlayerExpandMode				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerExpandMode)));
		MapOptions[PlayersCount-1].PlayerTilesFromHeart				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerTilesFromHeart)));
		MapOptions[PlayersCount-1].PlayerTilesFromClaimed			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerTilesFromClaimedArea)));
		MapOptions[PlayersCount-1].PlayerFlexibility				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFlexibility)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[0]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri1)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[1]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri2)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[2]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri3)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[3]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri4)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[4]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri5)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[5]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri6)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[6]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri7)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[7]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri8)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[8]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri9)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[9]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri10)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[10]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri11)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[11]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri12)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[12]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri13)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[13]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri14)));
		MapOptions[PlayersCount-1].PlayerRoomToBuildPri[14]			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerRoomToBuildPri15)));
		MapOptions[PlayersCount-1].PlayerSellCapturedRoom			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSellCapturedRoomIfLowOnGold[0])));
		MapOptions[PlayersCount-1].PlayerCreateEmptyAreas			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreateEmptyAreasWhenIdle[0])));
		MapOptions[PlayersCount-1].PlayerBuildBiggerLair			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBuildBiggerLair[0])));
		MapOptions[PlayersCount-1].PlayerTrapUseStyle				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerTrapUseStyle)));
		MapOptions[PlayersCount-1].PlayerPreference				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerPreference)));
		MapOptions[PlayersCount-1].PlayerDoorUsage				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerDoorUsage)));
		MapOptions[PlayersCount-1].PlayerUseTrapsnDoors				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerChanceOfLookingTrapsnDoors)));
		MapOptions[PlayersCount-1].PlayerSpellsSightOfEvil			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSpellsSightOfEvil)));
		MapOptions[PlayersCount-1].PlayerSpellsInBattle				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSpellsInBattle)));
		MapOptions[PlayersCount-1].PlayerSpellsPower				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSpellsPower)));
		MapOptions[PlayersCount-1].PlayerSpellsCallToArms			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSpellsCallToArms)));
		MapOptions[PlayersCount-1].PlayerBoulderInCorridors			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBoulderInCorridors[0])));
		MapOptions[PlayersCount-1].PlayerBoulderInAngles			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBoulderInAngles[0])));
		MapOptions[PlayersCount-1].PlayerSpellsLightning			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerSpellsLightning[0])));
		MapOptions[PlayersCount-1].PlayerCreaturesSackMethod			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesSackMethod)));
		MapOptions[PlayersCount-1].PlayerCreaturesImps				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesImps)));
		MapOptions[PlayersCount-1].PlayerCreaturesSlap				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesSlap)));
		MapOptions[PlayersCount-1].PlayerCreaturesHappyUnhappy			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesHappyUnhappy[0])));
		MapOptions[PlayersCount-1].PlayerCreaturesHappyUngry			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesHappyUngry[0])));
		MapOptions[PlayersCount-1].PlayerCreaturesDisposeUngry			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesDisposeUngry[0])));
		MapOptions[PlayersCount-1].PlayerCreaturesDisposeRubish			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCreaturesDisposeRubish[0])));
		MapOptions[PlayersCount-1].PlayerUseLevelInFirstAttack			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerUseLevelInFirstAttack[0])));
		MapOptions[PlayersCount-1].PlayerUseThreatInFirstAttack			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerUseThreatInFirstAttack[0])));
		MapOptions[PlayersCount-1].PlayerFirstAttackAllRooms			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackAllRooms[0])));
		MapOptions[PlayersCount-1].PlayerFirstAttackAllSpells			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackAllSpells[0])));
		MapOptions[PlayersCount-1].PlayerFirstAttackOnlyAttackers		= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackOnlyAttackers[0])));
		MapOptions[PlayersCount-1].PlayerFirstAttackDontAttack			= (BOOL)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackDontAttack[0])));
		MapOptions[PlayersCount-1].PlayerFirstAttackCount			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackCount)));
		MapOptions[PlayersCount-1].PlayerFirstAttackLevel			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackLevel)));
		MapOptions[PlayersCount-1].PlayerFirstAttackThreat			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackThreat)));
		MapOptions[PlayersCount-1].PlayerFirstAttackBreach			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackBreach)));
		MapOptions[PlayersCount-1].PlayerFirstAttackDig				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackDig)));
		MapOptions[PlayersCount-1].PlayerFirstAttackPoints			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackPoints)));
		MapOptions[PlayersCount-1].PlayerFirstAttackCreatures			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerFirstAttackCreatures)));
		MapOptions[PlayersCount-1].PlayerBattleDropped				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleDropped)));
		MapOptions[PlayersCount-1].PlayerBattleSpellType			= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleSpellType)));
		MapOptions[PlayersCount-1].PlayerBattlePrison				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattlePrison)));
		MapOptions[PlayersCount-1].PlayerBattleHealth				= (BYTE)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleHealth)));
		MapOptions[PlayersCount-1].PlayerBattleRemove				= (BYTE)(*((LONG *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleRemove[0])));
		MapOptions[PlayersCount-1].PlayerBattleWait				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleWait[0])));
		MapOptions[PlayersCount-1].PlayerBattleCallToHarmsRemoveNoCreature	= (LONG)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleCallToHarmsRemoveNoCreature)));
		MapOptions[PlayersCount-1].PlayerBattleCallToHarmsRemove		= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleCallToHarmsRemove[0])));
		MapOptions[PlayersCount-1].PlayerBattleCallToHarmsPlace			= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleCallToHarmsPlace[0])));
		MapOptions[PlayersCount-1].PlayerBattleCallToHarmsRemoveNoDanger1	= (LONG)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleCallToHarmsRemoveNoDanger1)));
		MapOptions[PlayersCount-1].PlayerBattleCallToHarmsRemoveNoDanger2	= (LONG)(*((BYTE *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBattleCallToHarmsRemoveNoDanger2)));
		MapOptions[PlayersCount-1].PlayerCPUAreaX1				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBuildAreaX1[0])));
		MapOptions[PlayersCount-1].PlayerCPUAreaY1				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBuildAreaY1[0])));
		MapOptions[PlayersCount-1].PlayerCPUAreaX2				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBuildAreaX2[0])));
		MapOptions[PlayersCount-1].PlayerCPUAreaY2				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerBuildAreaY2[0])));
		MapOptions[PlayersCount-1].PlayerCameraX				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCameraX[0])));
		MapOptions[PlayersCount-1].PlayerCameraY				= (LONG)(*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerCameraY[0])));
		strcpy(MapOptions[PlayersCount-1].PlayerName,((GAMEPLAYER *)PlayersPtr)->szPlayerName);
		Triggers_CreateActionPtFromPlayer(PlayersCount,*((WORD *)(&((GAMEPLAYER *)PlayersPtr)->bPlayerTrigger[0])));
		PlayersPtr += 205;
		PlayersMax--;
		}

	GlobalFree(PlayersName);
	GlobalFree(Players);
	return(1);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(PlayersName);
	GlobalFree(Players);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

MAPINFO* Game_ExtractMap(char *FileName, NODE *Errors)
{
	MAPINFO	*EditorMap;
	BYTE	*GameMap;
	MAPINFO	 GameMapInfo;
	DWORD	 GameMapSize;
	char	*MapName;
	char	*Point;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	Compiler_SetLastError(0);
	ZeroMemory(&GameMapInfo,sizeof(MAPINFO));
	strcpy(MapName,FileName);
	Point = strrchr(MapName,'.');
	if (Point) *Point = 0;
	strcat(MapName,szGameMapExt);

	//--- Chargement du fichier complet ---

	GameMap = (BYTE *)Files_Load(MapName,&GameMapSize);
	if (!GameMap)
		{
		GlobalFree(MapName);
		return(NULL);
		}

	//--- Test les données ---

	if (((GAMEMAP *)GameMap)->dwMapFileHeader != 0x00000064) goto Bad_Header;
	if (((GAMEMAP *)GameMap)->dwMapFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMEMAP *)GameMap)->dwMapInfoHeader != 0x00000065) goto Bad_Header;
	if (((GAMEMAP *)GameMap)->dwMapInfoVersion != 0x00000008) goto Bad_Header;

	GameMapInfo.Width = ((GAMEMAP *)GameMap)->dwMapWidth;
	GameMapInfo.Height = ((GAMEMAP *)GameMap)->dwMapHeight;

	if (((GAMEMAP *)GameMap)->dwMapDefineHeader != 0x00000066) goto Bad_Header;
	if (GameMapSize-sizeof(GAMEMAP) < GameMapInfo.Width*GameMapInfo.Height*4) goto Bad_Size;

	//--- Conversion de la carte ---

	GameMapInfo.Map = GameMap+sizeof(GAMEMAP);
	EditorMap = Compiler_GameMapToEditorMap(&GameMapInfo,Errors);
	GlobalFree(MapName);
	GlobalFree(GameMap);
	return(EditorMap);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(MapName);
	GlobalFree(GameMap);
	return(NULL);

Bad_Size:
	Compiler_SetLastError(COMPILERR_BADSIZE);
	GlobalFree(MapName);
	GlobalFree(GameMap);
	return(NULL);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des éléments						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Game_ExtractThings(char *FileName, NODE *Errors, MAPSTATS *Stats, BOOL Extract)
{
	BYTE	*Things;
	BYTE	*ThingsPtr;
	DWORD	 ThingsSize;
	ULONG	 ThingsCount;
	char	*ThingsName;
	char	*Point;

	ThingsName = GlobalAlloc(GPTR,MAX_PATH);
	if (!ThingsName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	if (Stats) ZeroMemory(Stats,sizeof(MAPSTATS));
	strcpy(ThingsName,FileName);
	Point = strrchr(ThingsName,'.');
	if (Point) *Point = 0;
	strcat(ThingsName,szGameThingsExt);

	Things = (BYTE *)Files_Load(ThingsName,&ThingsSize);
	if (!Things)
		{
		GlobalFree(ThingsName);
		return(0);
		}

//--- Test l'en-tête ---

	if (((GAMETHINGS *)Things)->dwThingsFileHeader != 0x000000BE) goto Bad_Header;
	if (((GAMETHINGS *)Things)->dwThingsFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMETHINGS *)Things)->dwThingsInfoHeader != 0x000000BF) goto Bad_Header;
	if (((GAMETHINGS *)Things)->dwGlobalInfoHeader != 0x0000001C) goto Bad_Header;
	ThingsCount = ((GAMETHINGS *)Things)->dwThingsCount;
	if (((GAMETHINGS *)Things)->dwThingsDefinesHeader != 0x000000C0) goto Bad_Header;

	ThingsPtr = Things+sizeof(GAMETHINGS);
	while(ThingsCount)
		{
		if (ThingsPtr >= Things+ThingsSize) goto Bad_Pointer;

		switch(*((DWORD *)ThingsPtr))
			{
			case 0x000000C2:
				if (Stats)
					{
					if ((((GAMEOBJECT *)ThingsPtr)->bObjectID >= 0x75)&&(((GAMEOBJECT *)ThingsPtr)->bObjectID <= 0x81))
						Stats->MagicalObjects++;
					else
						Stats->Objects++;
					}
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_OBJECT;
					MapThing->x = ((GAMEOBJECT *)ThingsPtr)->dwObjectLocationX;
					MapThing->y = ((GAMEOBJECT *)ThingsPtr)->dwObjectLocationY;
					MapThing->id = ((GAMEOBJECT *)ThingsPtr)->bObjectID;
					MapThing->owner = ((GAMEOBJECT *)ThingsPtr)->bGlobalPlayer;
					switch(MapThing->id)
						{
						case 0x00000001:
						case 0x00000002:
						case 0x00000003:
							MapThing->data.gold = ((GAMEOBJECT *)ThingsPtr)->dwObjectGoldValue;
							break;
						case 0x00000004:
							MapThing->data.spell = ((GAMEOBJECT *)ThingsPtr)->dwObjectSpell;
							break;
						}
					if ((((GAMEOBJECT *)ThingsPtr)->bObjectID >= 0x75)&&(((GAMEOBJECT *)ThingsPtr)->bObjectID <= 0x81))
						{
						MapThing->node.type = THING_MAGICALOBJECT;
						Game_AddThing(MapThing,&MapMagicalObjects,Errors);
						Triggers_CreateActionPtFromObject(MapThing,FALSE,((GAMEOBJECT *)ThingsPtr)->wObjectTrigger);
						}
					else
						{
						MapThing->node.type = THING_OBJECT;
						Game_AddThing(MapThing,&MapObjects,Errors);
						Triggers_CreateActionPtFromObject(MapThing,TRUE,((GAMEOBJECT *)ThingsPtr)->wObjectTrigger);
						}
					}
				ThingsPtr += 32;
				break;
			case 0x000000C3:
				if (Stats) Stats->Traps++;
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_TRAP;
					MapThing->x = ((GAMETRAP *)ThingsPtr)->dwTrapLocationX;
					MapThing->y = ((GAMETRAP *)ThingsPtr)->dwTrapLocationY;
					MapThing->id = ((GAMETRAP *)ThingsPtr)->bTrapID;
					MapThing->owner = ((GAMETRAP *)ThingsPtr)->bGlobalPlayer;
					MapThing->data.shots = ((GAMETRAP *)ThingsPtr)->bTrapShots;
					Game_AddThing(MapThing,&MapTraps,Errors);
					}
				ThingsPtr += 24;
				break;
			case 0x000000C4:
				if (Stats) Stats->Doors++;
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_DOOR;
					MapThing->x = ((GAMEDOOR *)ThingsPtr)->dwDoorLocationX;
					MapThing->y = ((GAMEDOOR *)ThingsPtr)->dwDoorLocationY;
					MapThing->id = ((GAMEDOOR *)ThingsPtr)->bDoorID;
					MapThing->owner = ((GAMEDOOR *)ThingsPtr)->bGlobalPlayer;
					MapThing->data.status = ((GAMEDOOR *)ThingsPtr)->bDoorLocked;
					Game_AddThing(MapThing,&MapDoors,Errors);
					Triggers_CreateActionPtFromCreature(MapThing,((GAMEDOOR *)ThingsPtr)->wDoorTrigger);
					}
				ThingsPtr += 28;
				break;
			case 0x000000C5:
				if (Stats) Stats->Areas++;
				if (Extract)
					{
					MAPRECTEX	*MapRectEx;

					MapRectEx = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
					if (!MapRectEx) goto No_Memory;
					MapRectEx->node.type = RECTEX_AREA;
					MapRectEx->rect.x1 = ((GAMEAREA *)ThingsPtr)->dwAreaLocationStartX;
					MapRectEx->rect.y1 = ((GAMEAREA *)ThingsPtr)->dwAreaLocationStartY;
					MapRectEx->rect.x2 = ((GAMEAREA *)ThingsPtr)->dwAreaLocationEndX;
					MapRectEx->rect.y2 = ((GAMEAREA *)ThingsPtr)->dwAreaLocationEndY;
					MapRectEx->rect.color = 0x00FFFF00;
					MapRectEx->rect.textcolor = 0x00FFFF00;
					MapRectEx->rect.name = MapRectEx->name;
					MapRectEx->waitdelay = ((GAMEAREA *)ThingsPtr)->wAreaWaitDelay;
					MapRectEx->flags = ((GAMEAREA *)ThingsPtr)->bAreaFlags;
					MapRectEx->id = ((GAMEAREA *)ThingsPtr)->bAreaID;
					MapRectEx->nextid = ((GAMEAREA *)ThingsPtr)->bAreaNextID;
					Triggers_CreateActionPtFromArea(((GAMEAREA *)ThingsPtr)->bAreaID,((GAMEAREA *)ThingsPtr)->wAreaTrigger);
					wsprintf(MapRectEx->name,szArea,MapRectEx->id);
					Game_AddRectEx(MapRectEx,0,&MapAreas,Errors);
					}
				ThingsPtr += 64;
				break;
			case 0x000000C6:
				if (Stats) Stats->Creatures++;
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_CREATURE;
					MapThing->x = ((GAMENEUTRAL *)ThingsPtr)->dwNeutralLocationX;
					MapThing->y = ((GAMENEUTRAL *)ThingsPtr)->dwNeutralLocationY;
					MapThing->id = ((GAMENEUTRAL *)ThingsPtr)->bGlobalCreatureID;
					MapThing->owner = 2;
					MapThing->data.level = ((GAMENEUTRAL *)ThingsPtr)->bGlobalLevel;
					MapThing->goldheld = ((GAMENEUTRAL *)ThingsPtr)->bGlobalGoldHeld;
					MapThing->health = ((GAMENEUTRAL *)ThingsPtr)->dwGlobalHealth;
					MapThing->creaturesoptions = ((GAMENEUTRAL *)ThingsPtr)->bGlobalBehavior;
					Game_AddThing(MapThing,&MapCreatures,Errors);
					Triggers_CreateActionPtFromCreature(MapThing,((GAMENEUTRAL *)ThingsPtr)->wNeutralTrigger);
					}
				ThingsPtr += 32;
				break;
			case 0x000000C7:
				if (Stats) Stats->Creatures++;
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_CREATURE;
					MapThing->x = ((GAMEHERO *)ThingsPtr)->dwHeroLocationX;
					MapThing->y = ((GAMEHERO *)ThingsPtr)->dwHeroLocationY;
					MapThing->id = ((GAMEHERO *)ThingsPtr)->bGlobalCreatureID;
					MapThing->owner = 1;
					MapThing->data.level = ((GAMEHERO *)ThingsPtr)->bGlobalLevel;
					MapThing->goldheld = ((GAMEHERO *)ThingsPtr)->bGlobalGoldHeld;
					MapThing->health = ((GAMEHERO *)ThingsPtr)->dwGlobalHealth;
					MapThing->herosoptions = ((GAMEHERO *)ThingsPtr)->bHeroOptions;
					MapThing->objective = ((GAMEHERO *)ThingsPtr)->bHeroObjective;
					MapThing->creaturesoptions = ((GAMEHERO *)ThingsPtr)->bGlobalBehavior;
					MapThing->area = ((GAMEHERO *)ThingsPtr)->bHeroTargetArea;
					MapThing->ennemy = ((GAMEHERO *)ThingsPtr)->bGlobalPlayer;
					Game_AddThing(MapThing,&MapCreatures,Errors);
					Triggers_CreateActionPtFromCreature(MapThing,((GAMEHERO *)ThingsPtr)->wHeroTrigger);
					}
				ThingsPtr += 40;
				break;
			case 0x000000C8:
				if (Stats) Stats->Creatures++;
				if (Extract)
					{
					MAPTHING	*MapThing;

					MapThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!MapThing) goto No_Memory;
					MapThing->node.type = THING_CREATURE;
					MapThing->x = ((GAMEKEEPER *)ThingsPtr)->dwKeeperLocationX;
					MapThing->y = ((GAMEKEEPER *)ThingsPtr)->dwKeeperLocationY;
					MapThing->id = ((GAMEKEEPER *)ThingsPtr)->bGlobalCreatureID;
					MapThing->owner = ((GAMEKEEPER *)ThingsPtr)->bGlobalPlayer;
					MapThing->data.level = ((GAMEKEEPER *)ThingsPtr)->bGlobalLevel;
					MapThing->goldheld = ((GAMEKEEPER *)ThingsPtr)->bGlobalGoldHeld;
					MapThing->health = ((GAMEKEEPER *)ThingsPtr)->dwGlobalHealth;
					MapThing->creaturesoptions = ((GAMEKEEPER *)ThingsPtr)->bGlobalBehavior;
					Game_AddThing(MapThing,&MapCreatures,Errors);
					Triggers_CreateActionPtFromCreature(MapThing,((GAMEKEEPER *)ThingsPtr)->wKeeperTrigger);
					}
				ThingsPtr += 36;
				break;
			case 0x000000C9:
				if (Stats) Stats->HerosBands++;
				if (Extract)
					{
					MTHBAND		*Heros;
					GAMEBANDHERO	*GameHeros;
					TRIGGERLINK	*Point;
					int		 i,j;

					Heros = GlobalAlloc(GPTR,sizeof(MTHBAND));
					if (!Heros) goto No_Memory;
					Heros->id = ((GAMEBAND *)ThingsPtr)->bBandID;
					Triggers_CreateActionPtFromBand(Heros,((GAMEBAND *)ThingsPtr)->wBandTrigger);
					wsprintf(Heros->name,szBandID,Heros->id);

					GameHeros = (GAMEBANDHERO *)(ThingsPtr+51);
					for (i = 0; i != THING_MAXHEROSINBAND; i++, GameHeros++)
						{
						if (!GameHeros->bGlobalCreatureID) break;
						if (!GameHeros->bGlobalBehavior) break;
						Heros->heros[i].isvalid = TRUE;
						Heros->heros[i].level = GameHeros->bGlobalLevel;
						Heros->heros[i].enemy = GameHeros->bGlobalPlayer;
						Heros->heros[i].area = GameHeros->bHeroTargetArea;
						Heros->heros[i].objective = GameHeros->bHeroObjective;
						Heros->heros[i].id = GameHeros->bGlobalCreatureID;
						Heros->heros[i].health = GameHeros->dwGlobalHealth;
						Heros->heros[i].goldheld = GameHeros->bGlobalGoldHeld;
						Heros->heros[i].options = GameHeros->bHeroOptions;
						Heros->heros[i].options2 = GameHeros->bGlobalBehavior;
						Point = Triggers_CreateActionPtFromCreatureInBand(&Heros->heros[i],Heros->id,GameHeros->wHeroTrigger);
						if (!Point) continue;
						for (j = 0; j != THING_MAXHEROSINBAND; j++)
							{
							if (!Heros->heros[j].isvalid) break;
							if (j == i) break;
							if (!Heros->heros[i].uniqueid) break;
							if (!Heros->heros[j].uniqueid) continue;
							if (Heros->heros[j].uniqueid == Heros->heros[i].uniqueid)
								{
								Heros->heros[i].uniqueid++;
								while(Triggers_GetStructureFromUniqueID(Heros->heros[i].uniqueid,SCOPE_CREATURE|SCOPE_BAND|SCOPE_CREATUREINBAND)) Heros->heros[i].uniqueid++;
								Point->creature = Heros->heros[i].uniqueid;
								j = -1;
								}
							}
						}
					List_AddEntry((NODE *)Heros,&MapHerosBands);
					}
				ThingsPtr += 563;
				break;
			case 0x000000CB:
				if (Extract)
					{
					MAPRECTEX	*MapRectEx;

					MapRectEx = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
					if (!MapRectEx) goto No_Memory;
					MapRectEx->node.type = RECTEX_EFFECT;
					MapRectEx->rect.x1 = ((GAMEEFFECT *)ThingsPtr)->dwEffectStartX;
					MapRectEx->rect.y1 = ((GAMEEFFECT *)ThingsPtr)->dwEffectStartY;
					MapRectEx->rect.x2 = MapRectEx->rect.x1+((GAMEEFFECT *)ThingsPtr)->wEffectWidth;
					MapRectEx->rect.y2 = MapRectEx->rect.y1+((GAMEEFFECT *)ThingsPtr)->wEffectHeight;
					MapRectEx->rect.color = 0x007F00FF;
					MapRectEx->rect.textcolor = 0x007F00FF;
					MapRectEx->rect.name = MapRectEx->name;
					MapRectEx->effect1 = ((GAMEEFFECT *)ThingsPtr)->wEffect1;
					MapRectEx->effect2 = ((GAMEEFFECT *)ThingsPtr)->wEffect2;
					MapRectEx->effect3 = ((GAMEEFFECT *)ThingsPtr)->wEffect3;
					MapRectEx->effect4 = ((GAMEEFFECT *)ThingsPtr)->wEffect4;
					MapRectEx->freq = ((GAMEEFFECT *)ThingsPtr)->bEffectFrequency;
					MapRectEx->id = ((GAMEEFFECT *)ThingsPtr)->bEffectID;
					wsprintf(MapRectEx->name,szEffect,MapRectEx->id);
					Game_AddRectEx(MapRectEx,0,&MapEffects,Errors);
					}
				ThingsPtr += 44;
				break;
			case 0x000000CC:
				if (((GAMEGATE *)ThingsPtr)->bGateType == 0x0E)
					{
					MapOptions[((GAMEGATE *)ThingsPtr)->bGlobalPlayer-1].PlayerHealth = ((GAMEGATE *)ThingsPtr)->wGateHealth;
					ThingsPtr += 28;
					break;
					}
				if (Stats) Stats->Areas++;
				if (Extract)
					{
					MAPRECTEX	*MapRectEx;

					MapRectEx = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
					if (!MapRectEx) goto No_Memory;
					MapRectEx->node.type = RECTEX_GATE;
					MapRectEx->rect.x1 = ((GAMEGATE *)ThingsPtr)->dwGateLocationX;
					MapRectEx->rect.y1 = ((GAMEGATE *)ThingsPtr)->dwGateLocationY;
					MapRectEx->rect.x2 = ((GAMEGATE *)ThingsPtr)->dwGateLocationX;
					MapRectEx->rect.y2 = ((GAMEGATE *)ThingsPtr)->dwGateLocationY;
					MapRectEx->rect.color = 0x00AAAAAA;
					MapRectEx->rect.textcolor = 0x00AAAAAA;
					MapRectEx->id = List_EntryCount(&MapGates)+1;
					MapRectEx->rect.name = MapRectEx->name;
					wsprintf(MapRectEx->name,szGate,MapRectEx->id);
					Game_AddRectEx(MapRectEx,((GAMEGATE *)ThingsPtr)->bGateType,&MapGates,Errors);
					}
				ThingsPtr += 28;
				break;
			default:
				Compiler_ReportErrorText(Errors,REPORT_BADTHINGHEADER,*((DWORD *)ThingsPtr));
				goto Unk_Header;
			}
		ThingsCount--;
		}

	GlobalFree(ThingsName);
	GlobalFree(Things);
	return(1);

//--- Erreurs ---

Unk_Header:
	if (Extract)
		{
		List_ReleaseMemory(&MapCreatures);
		List_ReleaseMemory(&MapDoors);
		List_ReleaseMemory(&MapTraps);
		List_ReleaseMemory(&MapObjects);
		List_ReleaseMemory(&MapMagicalObjects);
		List_ReleaseMemory(&MapHerosBands);
		List_ReleaseMemory(&MapAreas);
		List_ReleaseMemory(&MapGates);
		}
	Compiler_SetLastError(COMPILERR_UNKHEADER);
	GlobalFree(ThingsName);
	GlobalFree(Things);
	return(0);

No_Memory:
	if (Extract)
		{
		List_ReleaseMemory(&MapCreatures);
		List_ReleaseMemory(&MapDoors);
		List_ReleaseMemory(&MapTraps);
		List_ReleaseMemory(&MapObjects);
		List_ReleaseMemory(&MapMagicalObjects);
		List_ReleaseMemory(&MapHerosBands);
		List_ReleaseMemory(&MapAreas);
		List_ReleaseMemory(&MapGates);
		}
	Compiler_SetLastError(COMPILERR_WINDOWS);
	GlobalFree(ThingsName);
	GlobalFree(Things);
	return(0);

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	GlobalFree(ThingsName);
	GlobalFree(Things);
	return(0);

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(ThingsName);
	GlobalFree(Things);
	return(0);
}


// «»»» Test et ajoute une zone ou un passage «««««««««««««««««««««««««««»

void Game_AddRectEx(MAPRECTEX *MapRectEx, DWORD GateType, NODE *List, NODE *Errors)
{
	switch(MapRectEx->node.type)
		{
		case RECTEX_AREA:
			if (MapRectEx->flags&~(0x01|0x04|0x08|0x10|0x20|0x40))
				Compiler_ReportErrorXY(Errors,REPORT_UNKAREAFLAGS,MapRectEx->rect.x1,MapRectEx->rect.y1,MapRectEx->flags);
			break;

		case RECTEX_GATE:
			if ((GateType != 0x21)&&(GateType != 0x25))
				Compiler_ReportErrorXY(Errors,REPORT_BADGATETYPE,MapRectEx->rect.x1,MapRectEx->rect.y1,GateType);
			break;
		}

	List_AddEntry((NODE *)MapRectEx,List);
	return;
}


// «»»» Test et ajoute un élément «««««««««««««««««««««««««««««««««««««««»

void Game_AddThing(MAPTHING *MapThing, NODE *List, NODE *Errors)
{
	char	*Text;

	if (MapThing->owner > MAP_MAXPLAYERS)
		{
		Compiler_ReportErrorXY(Errors,REPORT_BADTHINGPLAYER,MapThing->x,MapThing->y,MapThing->owner);
		MapThing->owner = 2;
		}

	switch(MapThing->node.type)
		{
		case THING_CREATURE:
			Text = Things_GetCreatureNameFromID(MapThing->id);
			if (!Text) Compiler_ReportErrorText(Errors,REPORT_BADCREATURE,MapThing->id);
			if (MapThing->creaturesoptions&0x02) Compiler_ReportErrorXY(Errors,REPORT_BADTHINGBEHAVIOR,MapThing->x,MapThing->y,MapThing->creaturesoptions);
			if (MapThing->owner == 1)
				{
				if (MapThing->herosoptions&0xF0) Compiler_ReportErrorXY(Errors,REPORT_BADTHINGBEHAVIOR,MapThing->x,MapThing->y,MapThing->herosoptions);
				switch(MapThing->objective)
					{
					case 0x0B:
					case 0x0C:
					case 0x0D:
					case 0x0E:
					case 0x11:
					case 0x12:
					case 0x13:
					case 0x16:
					case 0x17:
					case 0x1B:
						break;
					default:
						Compiler_ReportErrorXY(Errors,REPORT_BADTHINGTARGET,MapThing->x,MapThing->y,MapThing->objective);
						break;
					}
				}
			break;
		case THING_DOOR:
			Text = Things_GetThingNameFromID(MapThing->id,Doors);
			if (!Text) Compiler_ReportErrorText(Errors,REPORT_BADDOOR,MapThing->id);
			break;
		case THING_TRAP:
			Text = Things_GetThingNameFromID(MapThing->id,Traps);
			if (!Text) Compiler_ReportErrorText(Errors,REPORT_BADTRAP,MapThing->id);
			break;
		case THING_OBJECT:
			Text = Things_GetThingNameFromID(MapThing->id,Objects);
			if (!Text) Compiler_ReportErrorText(Errors,REPORT_BADOBJECT,MapThing->id);
			break;
		case THING_MAGICALOBJECT:
			Text = Things_GetThingNameFromID(MapThing->id,Magics);
			if (!Text) Compiler_ReportErrorText(Errors,REPORT_BADMAGICALOBJECT,MapThing->id);
			break;
		}

	if (!Text)
		GlobalFree(MapThing);
	else
		List_AddEntry((NODE *)MapThing,List);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction des variables						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Game_ExtractVariables(char *FileName, NODE *Errors)
{
	BYTE	*Variables;
	BYTE	*VariablesPtr;
	DWORD	 VariablesSize;
	ULONG	 VariablesCount;
	char	*VariablesName;
	char	*Point;

	VariablesName = GlobalAlloc(GPTR,MAX_PATH);
	if (!VariablesName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	strcpy(VariablesName,FileName);
	Point = strrchr(VariablesName,'.');
	if (Point) *Point = 0;
	strcat(VariablesName,szGameVariablesExt);

	Variables = (BYTE *)Files_Load(VariablesName,&VariablesSize);
	if (!Variables)
		{
		GlobalFree(VariablesName);
		return(0);
		}

//--- Test l'en-tête ---

	if (((GAMEVARIABLES *)Variables)->dwVariablesFileHeader != 0x000000E6) goto Bad_Header;
	if (((GAMEVARIABLES *)Variables)->dwVariablesFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMEVARIABLES *)Variables)->dwVariablesInfoHeader != 0x000000E7) goto Bad_Header;
	if (((GAMEVARIABLES *)Variables)->dwGlobalInfoHeader != 0x0000001C) goto Bad_Header;
	VariablesCount = ((GAMEVARIABLES *)Variables)->dwVariablesCount;
	if (((GAMEVARIABLES *)Variables)->dwVariablesDefinesHeader != 0x000000E8) goto Bad_Header;

	VariablesPtr = Variables+sizeof(GAMEVARIABLES);
	while(VariablesCount)
		{
		if (VariablesPtr >= Variables+VariablesSize) goto Bad_Pointer;

		switch(*((LONG *)VariablesPtr))
			{
			case 0x00000001:
				Game_SetCreatureCount((GAMEVARCREATURECOUNT *)VariablesPtr,Errors);
				break;
			case 0x00000002:
				Game_SetVariable((GAMEVARTHINGSTATUS *)VariablesPtr,Errors);
				break;
			case 0x00000042:
				Game_SetAlliance((GAMEVARALLIANCE *)VariablesPtr,Errors);
				break;
			case 0x0000004D:
				Game_SetCreature((GAMEVARCREATURE *)VariablesPtr,Errors);
				break;
			default:Game_SetGlobalVariable((GAMEGLOBALVARIABLE *)VariablesPtr,Errors);
				break;
			}

		VariablesPtr += 16;
		VariablesCount--;
		}

	GlobalFree(VariablesName);
	GlobalFree(Variables);
	return(1);

//--- Erreurs ---

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	GlobalFree(VariablesName);
	GlobalFree(Variables);
	return(0);

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(VariablesName);
	GlobalFree(Variables);
	return(0);
}


// «»»» Modification d'une variable globale «««««««««««««««««««««««««««««»

void Game_SetGlobalVariable(GAMEGLOBALVARIABLE *Variable, NODE *Errors)
{
	int	i;

	for (i = 0; MapVars[i].Id != 0; i++)
		{
		if (MapVars[i].Id == 0xFFFFFFFF) continue;
		if (MapVars[i].Id == Variable->dwGlobalHeader) break;
		}

	if (MapVars[i].Id == 0)
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARIABLE,*((LONG *)Variable));
		return;
		}
	if (MapVars[i].Icon)
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARIABLEDATA,*((LONG *)Variable));
		return;
		}

	Map.Properties.Variables[MapVars[i].Id] = Variable->dwGlobalData;
	return;
}


// «»»» Place le nombre de créatures ««««««««««««««««««««««««««««««««««««»

void Game_SetCreatureCount(GAMEVARCREATURECOUNT *Variable, NODE *Errors)
{
	int	Offset;

	Offset = Game_GetOffsetFromID(Variable->dwGlobalCreatureID,BadCreatures);
	if (!Offset) Offset = Game_GetOffsetFromID(Variable->dwGlobalCreatureID,BadElites);
	if (!Offset) Offset = Game_GetOffsetFromID(Variable->dwGlobalCreatureID,Heros);

	if (!Offset)
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARCREATURE,Variable->dwGlobalCreatureID);
		return;
		}

	MapOptions[Variable->dwGlobalPlayer-1].CreaturesCount[Offset-1] = Variable->dwCreatureCount;
	return;
}


// «»»» Modifie le statut des éléments ««««««««««««««««««««««««««««««««««»

void Game_SetVariable(GAMEVARTHINGSTATUS *Variable, NODE *Errors)
{
	int	Offset;

	switch(Variable->wThingType)
		{
		case 1:	Offset = Game_GetOffsetFromID(Variable->dwThingID,VarRooms);
			break;
		case 3:	Offset = Game_GetOffsetFromID(Variable->dwThingID,Traps);
			Offset += MAP_VARROOMS;
			break;
		case 4:	Offset = Game_GetOffsetFromID(Variable->dwThingID,Doors);
			Offset += MAP_VARROOMS+MAP_VARTRAPS;
			break;
		case 5:	Offset = Game_GetOffsetFromID(Variable->dwThingID,Spells);
			Offset += MAP_VARROOMS+MAP_VARTRAPS+MAP_VARDOORS;
			break;
		default:Compiler_ReportErrorText(Errors,REPORT_BADVARTYPE,Variable->wThingType);
			return;
		}

	if (!Offset)
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARTHING,Variable->dwThingID);
		return;
		}

	MapOptions[Variable->wGlobalPlayer-1].Rooms[Offset-1] = (BYTE)Variable->dwThingState;
	return;
}


// «»»» Modifie les alliances «««««««««««««««««««««««««««««««««««««««««««»

void Game_SetAlliance(GAMEVARALLIANCE *Alliance, NODE *Errors)
{
	if ((Alliance->dwGlobalPlayer == 0)||(Alliance->dwGlobalPlayer > MAP_MAXPLAYERS))
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARALLIE,Alliance->dwGlobalPlayer);
		return;
		}

	if ((Alliance->dwAllieWith == 0)||(Alliance->dwAllieWith > MAP_MAXPLAYERS))
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARALLIED,Alliance->dwAllieWith);
		return;
		}

	Map.Properties.Allies[Alliance->dwGlobalPlayer-1][Alliance->dwAllieWith-1] = 1;
	Map.Properties.Allies[Alliance->dwAllieWith-1][Alliance->dwGlobalPlayer-1] = 1;
	return;
}


// «»»» Modification des paramètres d'une créature ««««««««««««««««««««««»

void Game_SetCreature(GAMEVARCREATURE *Creature, NODE *Errors)
{
	LONG	Value;
	UINT	Offset;

	switch(Creature->dwCreatureParamType)
		{
		case 1:	Value = 0;
			break;
		case 2:	Value = 1;
			break;
		case 3:	Value = 2;
			break;
		case 4:	Value = 3;
			break;
		default:Compiler_ReportErrorText(Errors,REPORT_BADVARPARAM,Creature->dwCreatureParamType);
			return;
		}

	Offset = Game_GetOffsetFromID(Creature->dwGlobalCreatureID,BadCreatures);
	if (!Offset) Offset = Game_GetOffsetFromID(Creature->dwGlobalCreatureID,BadElites);
	if (!Offset) Offset = Game_GetOffsetFromID(Creature->dwGlobalCreatureID,Heros);

	if (!Offset)
		{
		Compiler_ReportErrorText(Errors,REPORT_BADVARCREATURE,Creature->dwGlobalCreatureID);
		return;
		}

	Map.Properties.CreaturesParams[Offset-1][Value] = Creature->dwCreatureNewValue;
	return;
}


// «»»» Obtention de l'offset d'une variable ««««««««««««««««««««««««««««»

int Game_GetOffsetFromID(DWORD ID, ELEMENT *Table)
{
	int	Result = 0;
	int	i;

	if ((Table == BadElites)||(Table == Heros))
		{
		for (i = 0; BadCreatures[i].Id != 0; i++) Result++;
		if (Table == Heros) for (i = 0; BadElites[i].Id != 0; i++) Result++;
		}

	for (Result++, i = 0; Table[i].Id != 0; Result++, i++)
		if (Table[i].Variable == ID) return(Result);

	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Extraction du fichier principal					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Game_ExtractTriggers(char *FileName, NODE *Errors)
{
	ACTION	*Action;
	TRIGGER	*Trigger;
	BYTE	*Triggers;
	BYTE	*TriggersPtr;
	DWORD	 TriggersSize;
	LONG	 X,Y,Count;
	char	*TriggersName;
	char	*Point;

	TriggersName = GlobalAlloc(GPTR,MAX_PATH);
	if (!TriggersName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	Compiler_SetLastError(0);
	strcpy(TriggersName,FileName);
	Point = strrchr(TriggersName,'.');
	if (Point) *Point = 0;
	strcat(TriggersName,szGameTriggersExt);

	//--- Chargement du fichier complet ---

	Triggers = (BYTE *)Files_Load(TriggersName,&TriggersSize);
	if (!Triggers)
		{
		GlobalFree(TriggersName);
		return(0);
		}

	//--- Test les données ---

	if (((GAMETRIGGERS *)Triggers)->dwTriggersFileHeader != 0x000000D2) goto Bad_Header;
	if (((GAMETRIGGERS *)Triggers)->dwTriggersFileVersion != 0x00000004) goto Bad_Header;
	if (((GAMETRIGGERS *)Triggers)->dwTriggersInfoHeader != 0x000000D3) goto Bad_Header;
	if (((GAMETRIGGERS *)Triggers)->dwTriggersInfoVersion != 0x00000020) goto Bad_Header;

	//--- Ajoute les déclencheurs ---

	Count = ((GAMETRIGGERS *)Triggers)->dwTriggersCount;
	TriggersPtr = Triggers+sizeof(GAMETRIGGERS);
	while(Count--)
		{
		if (TriggersPtr >= Triggers+TriggersSize) goto Bad_Pointer;
		if (*((LONG *)TriggersPtr)++ != 0x000000D5) goto Bad_Trigger;
		if (*((LONG *)TriggersPtr)++ != 0x00000010) goto Bad_Version;

		Trigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
		if (!Trigger) goto No_Memory;

		switch(*(TriggersPtr+14))
			{
			case TRIGGER_VARIABLE:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				Trigger->param2 = *((BYTE *)TriggersPtr)++;
				Trigger->param3 = *((BYTE *)TriggersPtr)++;
				Trigger->param4 = *((BYTE *)TriggersPtr)++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			case TRIGGER_TIMER:
			case TRIGGER_CREATURES:
			case TRIGGER_HAPPYCREATURES:
			case TRIGGER_ANGRYCREATURES:
			case TRIGGER_ROOMS:
			case TRIGGER_ROOMSTYPES:
			case TRIGGER_ROOMSSIZE:
			case TRIGGER_ROOMSCONTENT:
			case TRIGGER_CLAIMAREA:
			case TRIGGER_BANDCREATED:
			case TRIGGER_DEATHSINBAND:
			case TRIGGER_JAILEDINBAND:
			case TRIGGER_STUNNEDINBAND:
			case TRIGGER_GROUPINPOSSESS:
			case TRIGGER_SELECTAREA:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				Trigger->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			case TRIGGER_DOORDESTROYED:
			case TRIGGER_OBJECTCLAIMED:
			case TRIGGER_APPEAR:
			case TRIGGER_DEATH:
			case TRIGGER_TURNCOAT:
			case TRIGGER_SEE:
			case TRIGGER_STUNNED:
			case TRIGGER_HURTCREATURE:
			case TRIGGER_GOLDDAY:
			case TRIGGER_TAKECREATURE:
			case TRIGGER_FREECREATURE:
			case TRIGGER_FIRECREATURE:
			case TRIGGER_SLAPPED:
			case TRIGGER_JAILED:
			case TRIGGER_TORTURED:
			case TRIGGER_CONVERTED:
			case TRIGGER_HUNGRY:
			case TRIGGER_AFRAID:
			case TRIGGER_STEAL:
			case TRIGGER_GOAWAY:
			case TRIGGER_NOTHUNGRY:
			case TRIGGER_FIRED:
			case TRIGGER_TAKE:
			case TRIGGER_TRANSITIONENDS:
			case TRIGGER_BREACHED:
			case TRIGGER_ENNEMYBREACHED:
				TriggersPtr += 8;
				break;
			case TRIGGER_SPELLS:
			case TRIGGER_SELECTALLAREA:
			case TRIGGER_ENTERAREAINPOSS:
				TriggersPtr++;
				Trigger->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 6;
				break;
			case TRIGGER_GOLDMINED:
			case TRIGGER_GOLDAMOUNT:
			case TRIGGER_MANAAMOUNT:
			case TRIGGER_TIMEELAPSED:
			case TRIGGER_SLAPCOUNT:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				TriggersPtr++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			case TRIGGER_HURTED:
			case TRIGGER_CREATUREEXP:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				TriggersPtr++;
				Trigger->param5 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				TriggersPtr++;
				break;
			case TRIGGER_CREAENTERAREA:
			case TRIGGER_AREACONTENT:
			case TRIGGER_DOORS:
			case TRIGGER_TRAPS:
			case TRIGGER_DESTROYED:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				Trigger->param2 = *((BYTE *)TriggersPtr)++;
				Trigger->param3 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			case TRIGGER_CLAIMENTIREAREA:
			case TRIGGER_CLICKELEMENT:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				Trigger->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 6;
				break;
			case TRIGGER_HEARTDESTROYED:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 7;
				break;
			case TRIGGER_LOSTCREATURES:
			case TRIGGER_KILLEDCREATURES:
			case TRIGGER_CREATURESLEVEL:
				Trigger->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Trigger->param4 = *((BYTE *)TriggersPtr)++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			default:Trigger->param1 = *((LONG *)TriggersPtr)++;
				Trigger->param5 = *((LONG *)TriggersPtr)++;
				break;
			}

		Trigger->id	= *((WORD *)TriggersPtr)++;
		Trigger->or	= *((WORD *)TriggersPtr)++;
		Trigger->and	= *((WORD *)TriggersPtr)++;
		Trigger->ctrl	= *((BYTE *)TriggersPtr)++;
		Trigger->repeat	= *((BYTE *)TriggersPtr)++;

		Trigger->node.type = TYPE_TRIGGER;
		List_AddEntry((NODE *)Trigger,&MapTriggers);
		}

	//--- Ajouter les actions ---

	Count = ((GAMETRIGGERS *)Triggers)->dwTriggersActionsCount;
	while(Count--)
		{
		if (TriggersPtr >= Triggers+TriggersSize) goto Bad_Pointer;
		if (*((LONG *)TriggersPtr)++ != 0x000000D6) goto Bad_Trigger;
		if (*((LONG *)TriggersPtr)++ != 0x00000010) goto Bad_Version;

		Action = GlobalAlloc(GPTR,sizeof(TRIGGER));
		if (!Action) goto No_Memory;

		switch(*(TriggersPtr+14))
			{
			case ACTION_ADDCREATURE:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				Action->param3 = *((BYTE *)TriggersPtr)++;
				Action->param4 = *((BYTE *)TriggersPtr)++;
				Action->param5 = *((WORD *)TriggersPtr)++;
				Action->param7 = *((WORD *)TriggersPtr)++;
				break;
			case ACTION_ADDELEMENT:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				Action->param3 = *((BYTE *)TriggersPtr)++;
				Action->param4 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 4;
				break;
			case ACTION_SETVARIABLE:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			case ACTION_SHOWCRYSTAL:
			case ACTION_SETTIMER:
			case ACTION_MOVECAMERA:
			case ACTION_CHANGEVIEW:
			case ACTION_PRIMARYTARGET:
			case ACTION_SHOWNEXTROOM:
			case ACTION_RUN:
			case ACTION_MUSICLEVEL:
			case ACTION_SYSTEMMESSAGES:
			case ACTION_ATTACK:
			case ACTION_PORTALS:
			case ACTION_MOODS:
			case ACTION_TIMERSPEECH:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 7;
				break;
			case ACTION_SHOWELEMENT:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				Action->param3 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			case ACTION_ALLIES:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				Action->param3 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 5;
				break;
			case ACTION_VICTORY:
			case ACTION_DEFEAT:
			case ACTION_HASCRYSTAL:
			case ACTION_SHOW:
			case ACTION_POSSESSCREATURE:
			case ACTION_REMOVE:
			case ACTION_MAKEHUNGRY:
				TriggersPtr += 8;
				break;
			case ACTION_RELEASEBAND:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			case ACTION_CHANGEPARAMS:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				TriggersPtr++;
				break;
			case ACTION_SHOWAREA:
			case ACTION_ENABLEFOG:
			case ACTION_GENERATE:
			case ACTION_TOGGLEEFFECT:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 6;
				break;
			case ACTION_CHANGESQUARE:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((WORD *)TriggersPtr)++;
				Action->param7 = *((WORD *)TriggersPtr)++;
				break;
			case ACTION_DISPLAYTIMER:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			case ACTION_SAY:
				Action->param1 = *((WORD *)TriggersPtr)++;
				TriggersPtr += 2;
				Action->param5 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 3;
				break;
			case ACTION_ROTATECAMERA:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				Action->param3 = *((WORD *)TriggersPtr)++;
				Action->param5 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 3;
				break;
			case ACTION_DESTROYGATE:
				TriggersPtr += 4;
				X = *((WORD *)TriggersPtr)++;
				Y = *((WORD *)TriggersPtr)++;
				Action->param5 = RectEx_GetGateAtMapPos(X-1,Y-1);
				break;
			case ACTION_CHANGEOWNER:
				TriggersPtr++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				TriggersPtr++;
				TriggersPtr++;
				Action->param5 = *((WORD *)TriggersPtr)++;
				Action->param7 = *((WORD *)TriggersPtr)++;
				break;
			case ACTION_SLAPSREMAINING:
				TriggersPtr += 4;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			case ACTION_CAMERAPATH:
				Action->param1 = *((BYTE *)TriggersPtr)++;
				Action->param2 = *((BYTE *)TriggersPtr)++;
				Action->param3 = *((BYTE *)TriggersPtr)++;
				TriggersPtr += 5;
				break;
			case ACTION_SHOWHEALTH:
				Action->param1 = *((LONG *)TriggersPtr)++;
				TriggersPtr += 4;
				break;
			default:Action->param1 = *((LONG *)TriggersPtr)++;
				Action->param5 = *((LONG *)TriggersPtr)++;
				break;
			}

		Action->id	= *((WORD *)TriggersPtr)++;
		Action->next	= *((WORD *)TriggersPtr)++;
		((WORD *)TriggersPtr)++;
		Action->ctrl	= *((BYTE *)TriggersPtr)++;
		Action->one	= *((BYTE *)TriggersPtr)++;

		Action->node.type = TYPE_ACTION;
		List_AddEntry((NODE *)Action,&MapActions);
		}

	Triggers_MakeLinks();
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(1);

//--- Erreurs ---

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	Triggers_ReleaseMemory();
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(0);

No_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	Triggers_ReleaseMemory();
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(0);

Bad_Version:
	TriggersPtr -= 4;
	Compiler_ReportErrorText(Errors,REPORT_BADVERSION,*((LONG *)TriggersPtr));
	Compiler_SetLastError(COMPILERR_BADVERSION);
	Triggers_ReleaseMemory();
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(0);

Bad_Trigger:
	TriggersPtr -= 4;
	Compiler_ReportErrorText(Errors,REPORT_BADTRIGGERID,*((LONG *)TriggersPtr));
	Compiler_SetLastError(COMPILERR_BADHEADER);
	Triggers_ReleaseMemory();
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(0);

Bad_Header:
	Triggers_ReleaseMemory();
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(TriggersName);
	GlobalFree(Triggers);
	return(0);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
