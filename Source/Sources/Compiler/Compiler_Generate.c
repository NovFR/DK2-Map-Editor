
//<<>-<>>---------------------------------------------------------------------()
/*
	Création des fichiers du jeu
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

extern NODE		MapCreatures;
extern NODE		MapTraps;
extern NODE		MapDoors;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern MAP		Map;
extern MAPOPTIONS	MapOptions[MAP_MAXPLAYERS];
extern GLOBALVARS	MapGlobalVariables;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}.kwd					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateInfoFile(char *FileName)
{
	HANDLE		 FileHandle;
	DWORD		 Written;
	WORD		*PadFix;
	GAMEINFO	*WorkFile;
	char		*WorkName;
	char		*Extension;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}
	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameMainExt);

	//--- Création du contenu du fichier ---

	WorkFile = GlobalAlloc(GPTR,sizeof(GAMEINFO));
	if (!WorkFile) goto Error_1;

	WorkFile->dwMainFileHeader		= 0x000000DC;
	WorkFile->dwMainFileVersion		= 0x00000004;
	WorkFile->dwMainFileSize		= 26939;
	WorkFile->dwMainInfoHeader		= 0x000000DD;
	WorkFile->dwGlobalInfoHeader		= 0x0000001C;
	WorkFile->dwMainUnknown0		= 0x001C000F;
	WorkFile->dwMainUnknown1		= 0x00003000;
	Compiler_SetDate(&WorkFile->dateMainDate);
	WorkFile->dwMainParamsHeader		= 0x000000DF;
	WorkFile->dwMainParamsSize		= 0x00006453;
	MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,Map.Properties.Name,-1,WorkFile->wstrMainName,63);
	MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,Map.Properties.Description,-1,WorkFile->wstrMainDescription,1023);
	MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,Map.Properties.Author,-1,WorkFile->wstrMainAuthor,63);
	MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,Map.Properties.Email,-1,WorkFile->wstrMainEmail,63);
	MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,Map.Properties.Information,-1,WorkFile->wstrMainInformation,1023);
	WorkFile->wMainTrigger			= (WORD)Triggers_GetFirstTrigger(0,SCOPE_LEVEL,NULL);
	WorkFile->bMainGameTicks		= Map.Properties.GameTicks;
	WorkFile->bMainOptions			= Map.Properties.Options;
	WorkFile->bMainOptions2			= Map.Properties.Options2;
	CopyMemory(&WorkFile->szMainSpeech[0],&Map.Properties.SpeechFile[0],32);
	WorkFile->bMainHornyTalismanPieces	= Map.Properties.HornyTalismanPieces;
	WorkFile->bMainRewardPrev1		= Map.Properties.RewardPrev1;
	WorkFile->bMainRewardPrev2		= Map.Properties.RewardPrev2;
	WorkFile->bMainRewardPrev3		= Map.Properties.RewardPrev3;
	WorkFile->bMainRewardNext1		= Map.Properties.RewardNext1;
	WorkFile->bMainRewardNext2		= Map.Properties.RewardNext2;
	WorkFile->bMainRewardNext3		= Map.Properties.RewardNext3;
	WorkFile->bMainSoundtrack		= Map.Properties.Soundtrack;
	WorkFile->bMainTextTableID		= (BYTE)Map.Properties.TextTableID;
	PadFix					= (WORD *)&WorkFile->bMainTitleTextID[0];
	*PadFix					= Map.Properties.TextTitleID;
	PadFix					= (WORD *)&WorkFile->bMainTitlePlotTextID[0];
	*PadFix					= Map.Properties.TextPlotID;
	PadFix					= (WORD *)&WorkFile->bMainDebriefingTextID[0];
	*PadFix					= Map.Properties.TextDebriefingID;
	PadFix					= (WORD *)&WorkFile->bMainObjectiveTextID[0];
	*PadFix					= Map.Properties.TextObjectiveID;
	WorkFile->bMainUnused5[0]		= 0xF7;
	WorkFile->bMainUnused5[1]		= 0x15;
	PadFix					= (WORD *)&WorkFile->bMainSubObjective3TextID[0];
	*PadFix					= Map.Properties.TextSubObjective3ID;
	PadFix					= (WORD *)&WorkFile->bMainSubObjective1TextID[0];
	*PadFix					= Map.Properties.TextSubObjective1ID;
	PadFix					= (WORD *)&WorkFile->bMainSubObjective2TextID[0];
	*PadFix					= Map.Properties.TextSubObjective2ID;
	WorkFile->bMainSpecialLevelNumber	= Map.Properties.SpecialLevelNumber;
	WorkFile->bMainIntroCreatureID0		= Map.Properties.TextIntroCreatureID0;
	WorkFile->bMainIntroCreatureID1		= Map.Properties.TextIntroCreatureID1;
	WorkFile->bMainIntroCreatureID2		= Map.Properties.TextIntroCreatureID2;
	WorkFile->bMainIntroCreatureID3		= Map.Properties.TextIntroCreatureID3;
	WorkFile->bMainIntroCreatureID4		= Map.Properties.TextIntroCreatureID4;
	WorkFile->bMainIntroCreatureID5		= Map.Properties.TextIntroCreatureID5;
	WorkFile->bMainIntroCreatureID6		= Map.Properties.TextIntroCreatureID6;
	WorkFile->bMainIntroCreatureID7		= Map.Properties.TextIntroCreatureID7;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID0[0];
	*PadFix					= Map.Properties.TextIntroTextID0;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID1[0];
	*PadFix					= Map.Properties.TextIntroTextID1;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID2[0];
	*PadFix					= Map.Properties.TextIntroTextID2;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID3[0];
	*PadFix					= Map.Properties.TextIntroTextID3;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID4[0];
	*PadFix					= Map.Properties.TextIntroTextID4;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID5[0];
	*PadFix					= Map.Properties.TextIntroTextID5;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID6[0];
	*PadFix					= Map.Properties.TextIntroTextID6;
	PadFix					= (WORD *)&WorkFile->bMainIntroTextID7[0];
	*PadFix					= Map.Properties.TextIntroTextID7;
	WorkFile->bMainLevelNumber		= Map.Properties.LevelNumber;
	WorkFile->bMainPlayers			= (BYTE)Map.Properties.Players;
	PadFix					= (WORD *)&WorkFile->bMainPreLevelSpeechID[0];
	*PadFix					= Map.Properties.SpeechPreLevelID;
	PadFix					= (WORD *)&WorkFile->bMainWinSpeechID[0];
	*PadFix					= Map.Properties.SpeechWinID;
	PadFix					= (WORD *)&WorkFile->bMainLooseSpeechID[0];
	*PadFix					= Map.Properties.SpeechLooseID;
	PadFix					= (WORD *)&WorkFile->bMainNewsSpeechID[0];
	*PadFix					= Map.Properties.SpeechNewsID;
	PadFix					= (WORD *)&WorkFile->bMainGenPreLevelSpeechID[0];
	*PadFix					= Map.Properties.SpeechGenericPreLevelID;
	WorkFile->bMainUnused9[64]		= 0xDE;
	WorkFile->bMainUnused9[68]		= 0xA8;
	WorkFile->bMainUnused9[69]		= 0x04;
	WorkFile->bMainUnused9[72]		= 0x6E;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_TERRAINS)) strcpy(WorkFile->szFileTerrain,szKwdFileTerrain);
	else wsprintf(WorkFile->szFileTerrain,szKwdFileUserTerrain,Map.Properties.Name);
	WorkFile->szFileTerrain[64]		= 0xF0;
	strcpy(WorkFile->szFileObjects,szKwdFileObjects);
	WorkFile->szFileObjects[64]		= 0x78;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_ROOMS)) strcpy(WorkFile->szFileRooms,szKwdFileRooms);
	else wsprintf(WorkFile->szFileRooms,szKwdFileUserRooms,Map.Properties.Name);
	WorkFile->szFileRooms[64]		= 0xAA;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURES)) strcpy(WorkFile->szFileCreatures,szKwdFileCreatures);
	else wsprintf(WorkFile->szFileCreatures,szKwdFileUserCreatures,Map.Properties.Name);
	WorkFile->szFileCreatures[64]		= 0xA0;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_CREATURESPELLS)) strcpy(WorkFile->szFileCreatureSpells,szKwdFileCreatureSpells);
	else wsprintf(WorkFile->szFileCreatureSpells,szKwdFileUserCreatureSpells,Map.Properties.Name);
	WorkFile->szFileCreatureSpells[64]	= 0x82;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_TRAPS)) strcpy(WorkFile->szFileTraps,szKwdFileTraps);
	else wsprintf(WorkFile->szFileTraps,szKwdFileUserTraps,Map.Properties.Name);
	WorkFile->szFileTraps[64]		= 0x8C;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_DOORS)) strcpy(WorkFile->szFileDoors,szKwdFileDoors);
	else wsprintf(WorkFile->szFileDoors,szKwdFileUserDoors,Map.Properties.Name);
	WorkFile->szFileDoors[64]		= 0x04;
	WorkFile->szFileDoors[65]		= 0x01;
	strcpy(WorkFile->szFileShots,szKwdFileShots);
	WorkFile->szFileShots[64]		= 0x96;
	if (!Properties_GlobalChanged(&MapGlobalVariables,MGRF_SPELLS)) strcpy(WorkFile->szFileKeeperSpells,szKwdFileKeeperSpells);
	else wsprintf(WorkFile->szFileKeeperSpells,szKwdFileUserKeeperSpells,Map.Properties.Name);
	WorkFile->szFileKeeperSpells[64]	= 0xB4;
	wsprintf(WorkFile->szFileUserPlayers,szKwdFileUserPlayers,Map.Properties.Name);
	WorkFile->szFileUserPlayers[64]		= 0x64;
	wsprintf(WorkFile->szFileUserMap,szKwdFileUserMap,Map.Properties.Name);
	WorkFile->szFileUserMap[64]		= 0xD2;
	wsprintf(WorkFile->szFileUserTriggers,szKwdFileUserTriggers,Map.Properties.Name);
	WorkFile->szFileUserTriggers[64]	= 0xE6;
	strcpy(WorkFile->szFileGlobalVariables,szKwdFileGlobalVariables);
	WorkFile->szFileGlobalVariables[64]	= 0xE6;
	wsprintf(WorkFile->szFileUserVariables,szKwdFileUserVariables,Map.Properties.Name);
	WorkFile->szFileUserVariables[64]	= 0xBE;
	wsprintf(WorkFile->szFileUserThings,szKwdFileUserThings,Map.Properties.Name);
	WorkFile->bMainUnknown3[0]		= 0x01;
	WorkFile->bMainUnknown3[4]		= 0x07;
	WorkFile->bMainUnknown3[8]		= 0x03;
	WorkFile->bMainUnknown3[12]		= 0x0C;
	WorkFile->bMainUnknown3[16]		= 0x06;
	WorkFile->bMainUnknown3[20]		= 0x05;
	WorkFile->bMainUnknown3[24]		= 0x09;
	WorkFile->bMainUnknown3[28]		= 0x16;
	WorkFile->bMainUnknown3[32]		= 0x08;
	WorkFile->bMainUnknown3[36]		= 0x04;
	WorkFile->bMainUnknown3[40]		= 0x0B;
	WorkFile->bMainUnknown3[44]		= 0x17;
	WorkFile->bMainUnknown3[48]		= 0x18;
	WorkFile->bMainUnknown3[52]		= 0x0A;
	WorkFile->bMainUnknown3[56]		= 0x10;
	WorkFile->bMainUnknown3[60]		= 0x1C;
	WorkFile->bMainUnknown3[64]		= 0x13;
	WorkFile->bMainUnknown3[68]		= 0x0D;
	WorkFile->bMainUnknown3[72]		= 0x00;
	WorkFile->bMainUnknown3[76]		= 0x12;
	WorkFile->bMainUnknown3[80]		= 0x0E;
	WorkFile->bMainUnknown3[84]		= 0x19;
	WorkFile->bMainUnknown3[88]		= 0x14;
	WorkFile->bMainUnknown3[92]		= 0x02;
	WorkFile->bMainUnknown3[96]		= 0x1E;
	WorkFile->bMainUnknown3[100]		= 0x1D;
	WorkFile->bMainUnknown3[104]		= 0x0F;
	WorkFile->bMainUnknown3[108]		= 0x11;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}
	WriteFile(FileHandle,WorkFile,WorkFile->dwMainFileSize,&Written,NULL);
	if (Written != WorkFile->dwMainFileSize) goto Error_3;
	CloseHandle(FileHandle);

	GlobalFree(WorkFile);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_3:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(WorkFile);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Players.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GeneratePlayersFile(char *FileName)
{
	DWORD		 Count;
	DWORD		 Written;
	LONG		 X,Y;
	HANDLE		 FileHandle;
	GAMEPLAYERS	*Players;
	GAMEPLAYER	*Player;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}
	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGamePlayersExt);

	//--- Comptabilise le nombre de joueurs ---

	for (Count = 2, i = 3; i != MAP_MAXPLAYERS+1; i++)
		{
		if (!Compiler_InfoHeartPosition(i,&X,&Y)) continue;
		Count++;
		}

	//--- Création du contenu du fichier ---

	Players = GlobalAlloc(GPTR,Count*205+sizeof(GAMEPLAYERS));
	if (!Players)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Players->dwPlayersFileHeader		= 0x000000B4;
	Players->dwPlayersFileVersion		= 0x00000004;
	Players->dwPlayersFileSize		= Count*205+sizeof(GAMEPLAYERS);
	Players->dwPlayersInfoHeader		= 0x000000B5;
	Players->dwGlobalInfoHeader		= 0x0000001C;
	Players->dwPlayersCount			= Count;
	Players->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&Players->datePlayersDate);
	Players->dwPlayersDefinesHeader		= 0x000000B6;
	Players->dwPlayersDefinesSize		= Count*205;

	for (Player = (GAMEPLAYER *)(((BYTE *)Players)+sizeof(GAMEPLAYERS)), i = 0; Count != 0; --Count, i++)
		{
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerGold[0]))				= (LONG)MapOptions[i].PlayerGold;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCPU[0]))				= (LONG)MapOptions[i].PlayerComputer;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerAI))					= (BYTE)MapOptions[i].PlayerAI;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscSpeed))				= (BYTE)MapOptions[i].PlayerSpeed;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscOpenness))			= (BYTE)MapOptions[i].PlayerOpenness;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscGold))				= (BYTE)MapOptions[i].PlayerGoldValue;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscMana))				= (BYTE)MapOptions[i].PlayerManaValue;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerMiscMineGoldUntil[0]))		= (WORD)MapOptions[i].PlayerMineGoldAmount;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerMiscStartingMana[0]))		= (WORD)MapOptions[i].PlayerMana;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscLibrary))			= (BYTE)MapOptions[i].PlayerLibraryUsage;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscImpsToTilesRatio))		= (BYTE)MapOptions[i].PlayerImpsToTilesRatio;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscChanceToFindSpecials))		= (BYTE)MapOptions[i].PlayerFindSpecialsExplore;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscChanceOfFindingSpecials))	= (BYTE)MapOptions[i].PlayerFindSpecialsChances;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerMiscTilesToFindSpecials))		= (WORD)MapOptions[i].PlayerFindSpecialsTiles;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerMiscJailedFate))			= (BYTE)MapOptions[i].PlayerJailedFate;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerResearchedRoom))			= (BYTE)MapOptions[i].PlayerResearchedRoom;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRebuildRoom))			= (BYTE)MapOptions[i].PlayerRebuildRoom;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerDefaultTiles))			= (BYTE)MapOptions[i].PlayerDefaultTiles;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerTilesLeftBetweenRooms))		= (BYTE)MapOptions[i].PlayerTilesBetweenRooms;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerDistance))				= (BYTE)MapOptions[i].PlayerDistance;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerCorridorStyle))			= (BYTE)MapOptions[i].PlayerCorridorStyle;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerExpandMode))				= (BYTE)MapOptions[i].PlayerExpandMode;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerTilesFromHeart))			= (BYTE)MapOptions[i].PlayerTilesFromHeart;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerTilesFromClaimedArea))		= (BYTE)MapOptions[i].PlayerTilesFromClaimed;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFlexibility))			= (BYTE)MapOptions[i].PlayerFlexibility;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri1))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[0];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri2))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[1];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri3))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[2];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri4))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[3];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri5))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[4];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri6))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[5];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri7))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[6];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri8))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[7];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri9))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[8];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri10))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[9];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri11))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[10];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri12))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[11];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri13))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[12];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri14))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[13];
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerRoomToBuildPri15))			= (BYTE)MapOptions[i].PlayerRoomToBuildPri[14];
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerSellCapturedRoomIfLowOnGold[0]))	= (LONG)MapOptions[i].PlayerSellCapturedRoom;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCreateEmptyAreasWhenIdle[0]))	= (LONG)MapOptions[i].PlayerCreateEmptyAreas;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerBuildBiggerLair[0]))			= (LONG)MapOptions[i].PlayerBuildBiggerLair;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerTrapUseStyle))			= (BYTE)MapOptions[i].PlayerTrapUseStyle;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerPreference))				= (BYTE)MapOptions[i].PlayerPreference;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerDoorUsage))				= (BYTE)MapOptions[i].PlayerDoorUsage;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerChanceOfLookingTrapsnDoors))		= (BYTE)MapOptions[i].PlayerUseTrapsnDoors;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerSpellsSightOfEvil))			= (BYTE)MapOptions[i].PlayerSpellsSightOfEvil;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerSpellsInBattle))			= (BYTE)MapOptions[i].PlayerSpellsInBattle;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerSpellsPower))			= (BYTE)MapOptions[i].PlayerSpellsPower;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerSpellsCallToArms))			= (BYTE)MapOptions[i].PlayerSpellsCallToArms;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerBoulderInCorridors[0]))		= (LONG)MapOptions[i].PlayerBoulderInCorridors;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerBoulderInAngles[0]))			= (LONG)MapOptions[i].PlayerBoulderInAngles;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerSpellsLightning[0]))			= (LONG)MapOptions[i].PlayerSpellsLightning;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesSackMethod))		= (BYTE)MapOptions[i].PlayerCreaturesSackMethod;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesImps))			= (BYTE)MapOptions[i].PlayerCreaturesImps;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesSlap))			= (BYTE)MapOptions[i].PlayerCreaturesSlap;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesHappyUnhappy[0]))		= (LONG)MapOptions[i].PlayerCreaturesHappyUnhappy;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesHappyUngry[0]))		= (LONG)MapOptions[i].PlayerCreaturesHappyUngry;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesDisposeUngry[0]))		= (LONG)MapOptions[i].PlayerCreaturesDisposeUngry;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerCreaturesDisposeRubish[0]))		= (LONG)MapOptions[i].PlayerCreaturesDisposeRubish;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerUseLevelInFirstAttack[0]))		= (LONG)MapOptions[i].PlayerUseLevelInFirstAttack;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerUseThreatInFirstAttack[0]))		= (LONG)MapOptions[i].PlayerUseThreatInFirstAttack;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackAllRooms[0]))		= (LONG)MapOptions[i].PlayerFirstAttackAllRooms;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackAllSpells[0]))		= (LONG)MapOptions[i].PlayerFirstAttackAllSpells;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackOnlyAttackers[0]))	= (LONG)MapOptions[i].PlayerFirstAttackOnlyAttackers;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackDontAttack[0]))		= (LONG)MapOptions[i].PlayerFirstAttackDontAttack;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackCount))			= (BYTE)MapOptions[i].PlayerFirstAttackCount;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackLevel))			= (BYTE)MapOptions[i].PlayerFirstAttackLevel;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackThreat))			= (BYTE)MapOptions[i].PlayerFirstAttackThreat;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackBreach))			= (BYTE)MapOptions[i].PlayerFirstAttackBreach;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackDig))			= (BYTE)MapOptions[i].PlayerFirstAttackDig;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackPoints))			= (BYTE)MapOptions[i].PlayerFirstAttackPoints;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerFirstAttackCreatures))		= (BYTE)MapOptions[i].PlayerFirstAttackCreatures;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleDropped))			= (BYTE)MapOptions[i].PlayerBattleDropped;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleSpellType))			= (BYTE)MapOptions[i].PlayerBattleSpellType;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattlePrison))			= (BYTE)MapOptions[i].PlayerBattlePrison;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleHealth))			= (BYTE)MapOptions[i].PlayerBattleHealth;
		*((LONG *)(&((GAMEPLAYER *)Player)->bPlayerBattleRemove[0]))			= (BYTE)MapOptions[i].PlayerBattleRemove;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBattleWait[0]))			= (WORD)MapOptions[i].PlayerBattleWait;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleCallToHarmsRemoveNoCreature))	= (BYTE)MapOptions[i].PlayerBattleCallToHarmsRemoveNoCreature;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBattleCallToHarmsRemove[0]))		= (WORD)MapOptions[i].PlayerBattleCallToHarmsRemove;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBattleCallToHarmsPlace[0]))		= (WORD)MapOptions[i].PlayerBattleCallToHarmsPlace;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleCallToHarmsRemoveNoDanger1))	= (BYTE)MapOptions[i].PlayerBattleCallToHarmsRemoveNoDanger1;
		*((BYTE *)(&((GAMEPLAYER *)Player)->bPlayerBattleCallToHarmsRemoveNoDanger2))	= (BYTE)MapOptions[i].PlayerBattleCallToHarmsRemoveNoDanger2;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaX1[0]))			= (WORD)MapOptions[i].PlayerCPUAreaX1;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaY1[0]))			= (WORD)MapOptions[i].PlayerCPUAreaY1;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaX2[0]))			= (WORD)MapOptions[i].PlayerCPUAreaX2;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaY2[0]))			= (WORD)MapOptions[i].PlayerCPUAreaY2;
		*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerTrigger[0]))				= (WORD)Triggers_GetFirstTrigger(i+1,SCOPE_PLAYER,NULL);
		*((BYTE *)(&((GAMEPLAYER *)Player)->bGlobalPlayer))				= (BYTE)(i+1);

		if (i >= 2)
			{
			if (MapOptions[i].PlayerCameraX == -1)
				{
				Compiler_InfoHeartPosition(i+1,&X,&Y);
				*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraX[0]))		= (WORD)X;
				*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraY[0]))		= (WORD)Y;
				}
			else
				{
				*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraX[0]))		= (WORD)MapOptions[i].PlayerCameraX;
				*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraY[0]))		= (WORD)MapOptions[i].PlayerCameraY;
				}
			}
		else
			{
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraX[0]))			= (WORD)0;
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerCameraY[0]))			= (WORD)0;
			}

		if (MapOptions[i].PlayerCPUAreaX1 < 0)
			{
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaX1[0])) = (WORD)2;
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaY1[0])) = (WORD)2;
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaX2[0])) = (WORD)(Map.Properties.Width-2);
			*((WORD *)(&((GAMEPLAYER *)Player)->bPlayerBuildAreaY2[0])) = (WORD)(Map.Properties.Height-2);
			}

		strcpy(Player->szPlayerName,MapOptions[i].PlayerName);
		Player = (GAMEPLAYER *)(((BYTE *)Player)+205);
		}

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}
	WriteFile(FileHandle,Players,Players->dwPlayersFileSize,&Written,NULL);
	if (Written != Players->dwPlayersFileSize) goto Error_3;
	CloseHandle(FileHandle);

	GlobalFree(Players);
	GlobalFree(WorkName);
	return(1);

// --- Erreurs ---

Error_3:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(Players);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Things.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateThingsFile(char *FileName)
{
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 TotalThings;
	DWORD		 ThingsLen;
	GAMETHINGS	*Things;
	char		*WorkName;
	char		*Extension;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}
	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameThingsExt);

	//--- Ecriture du contenu du fichier ---

	Things = GlobalAlloc(GPTR,sizeof(GAMETHINGS));
	if (!Things)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	TotalThings = 0;
	ThingsLen = 0;
	Compiler_GenerateObjects(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateMagicalObjects(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateTraps(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateDoors(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateCreatures(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateAreas(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateGates(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateEffects(NULL,&TotalThings,&ThingsLen);
	Compiler_GenerateHeros(NULL,&TotalThings,&ThingsLen);

	Things->dwThingsFileHeader		= 0x000000BE;
	Things->dwThingsFileVersion		= 0x00000004;
	Things->dwThingsFileSize		= ThingsLen+sizeof(GAMETHINGS);
	Things->dwThingsInfoHeader		= 0x000000BF;
	Things->dwGlobalInfoHeader		= 0x0000001C;
	Things->dwThingsCount			= TotalThings;
	Things->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&Things->dateThingsDate);
	Things->dwThingsDefinesHeader		= 0x000000C0;
	Things->dwThingsDefinesSize		= ThingsLen;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}
	WriteFile(FileHandle,Things,sizeof(GAMETHINGS),&Written,NULL);
	if (Written != sizeof(GAMETHINGS)) goto Error_3;

	if (!Compiler_GenerateObjects(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateMagicalObjects(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateTraps(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateDoors(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateCreatures(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateAreas(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateGates(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateEffects(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;
	if (!Compiler_GenerateHeros(FileHandle,&TotalThings,&ThingsLen)) goto Error_3;

	CloseHandle(FileHandle);

	GlobalFree(Things);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_3:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(Things);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// «»»» Ecriture des objets «««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateObjects(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEOBJECT	 Object;
	MAPTHING	*List;
	DWORD		 Written;

	ZeroMemory(&Object,sizeof(GAMEOBJECT));
	Object.dwObjectHeader	= 0x000000C2;
	Object.dwObjectVersion	= 0x00000018;

	for (List = (MAPTHING *)MapObjects.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		Object.dwObjectLocationX = List->x;
		Object.dwObjectLocationY = List->y;
		Object.bObjectID	 = (BYTE)List->id;
		Object.bGlobalPlayer	 = (BYTE)List->owner;
		Object.wObjectTrigger	 = (WORD)Triggers_GetFirstTrigger(List->uniqueid,SCOPE_OBJECT,NULL);

		switch(List->id)
			{
			case 0x00000001:
			case 0x00000002:
			case 0x00000003:
				Object.dwObjectGoldValue = List->data.gold;
				Object.dwObjectSpell = 0;
				break;
			case 0x00000004:
				Object.dwObjectGoldValue = 0;
				Object.dwObjectSpell = List->data.spell;
				break;
			default:
				Object.dwObjectGoldValue = 0;
				Object.dwObjectSpell = 0;
				break;
			}

		if (FileHandle)
			{
			WriteFile(FileHandle,&Object,sizeof(GAMEOBJECT),&Written,NULL);
			if (Written != sizeof(GAMEOBJECT)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEOBJECT);
		}

	return(1);
}


// «»»» Ecriture des objets magiques ««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateMagicalObjects(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEOBJECT	 Object;
	MAPTHING	*List;
	DWORD		 Written;

	ZeroMemory(&Object,sizeof(GAMEOBJECT));
	Object.dwObjectHeader	= 0x000000C2;
	Object.dwObjectVersion	= 0x00000018;

	for (List = (MAPTHING *)MapMagicalObjects.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		Object.dwObjectLocationX = List->x;
		Object.dwObjectLocationY = List->y;
		Object.bObjectID	 = (BYTE)List->id;
		Object.bGlobalPlayer	 = (BYTE)List->owner;
		Object.wObjectTrigger	 = (WORD)Triggers_GetFirstTrigger(List->uniqueid,SCOPE_MAGIC,NULL);

		if (FileHandle)
			{
			WriteFile(FileHandle,&Object,sizeof(GAMEOBJECT),&Written,NULL);
			if (Written != sizeof(GAMEOBJECT)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEOBJECT);
		}

	return(1);
}


// «»»» Ecriture des pièges «««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateTraps(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMETRAP	 Trap;
	MAPTHING	*List;
	DWORD		 Written;

	Trap.dwTrapHeader	= 0x000000C3;
	Trap.dwTrapVersion	= 0x00000010;
	Trap.dwTrapUnknown0	= 0x00000000;

	for (List = (MAPTHING *)MapTraps.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		Trap.dwTrapLocationX	= List->x;
		Trap.dwTrapLocationY	= List->y;
		Trap.bTrapID		= (BYTE)List->id;
		Trap.bGlobalPlayer	= (BYTE)List->owner;
		Trap.bTrapShots		= (BYTE)List->data.shots;

		if (FileHandle)
			{
			WriteFile(FileHandle,&Trap,sizeof(GAMETRAP),&Written,NULL);
			if (Written != sizeof(GAMETRAP)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMETRAP);
		}

	return(1);
}


// «»»» Ecriture des portes «««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateDoors(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEDOOR	 Door;
	MAPTHING	*List;
	DWORD		 Written;

	ZeroMemory(&Door,sizeof(GAMEDOOR));
	Door.dwDoorHeader	= 0x000000C4;
	Door.dwDoorVersion	= 0x00000014;
//	Door.bDoorUnknown2	= 0x63;
//	Door.bDoorUnknown3	= 0x5F;

	for (List = (MAPTHING *)MapDoors.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		Door.dwDoorLocationX	= List->x;
		Door.dwDoorLocationY	= List->y;
		Door.bDoorID		= (BYTE)List->id;
		Door.bGlobalPlayer	= (BYTE)List->owner;
		Door.bDoorLocked	= (BYTE)List->data.status;
		Door.wDoorTrigger	= (WORD)Triggers_GetFirstTrigger(List->uniqueid,SCOPE_DOOR,NULL);

		if (FileHandle)
			{
			WriteFile(FileHandle,&Door,sizeof(GAMEDOOR),&Written,NULL);
			if (Written != sizeof(GAMEDOOR)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEDOOR);
		}

	return(1);
}


// «»»» Ecriture des créatures ««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateCreatures(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	LPVOID		 Data;
	DWORD		 DataLen;
	MAPTHING	*List;
	DWORD		 Written;

	for (List = (MAPTHING *)MapCreatures.next; List != 0; List = (MAPTHING *)List->node.next)
		{
		switch(List->owner)
			{
			case 1:	Data = GlobalAlloc(GPTR,sizeof(GAMEHERO));
				if (!Data) return(0);
				((GAMEHERO *)Data)->dwHeroHeader	= 0x000000C7;
				((GAMEHERO *)Data)->dwHeroVersion	= 0x00000020;
				((GAMEHERO *)Data)->dwHeroLocationX	= List->x;
				((GAMEHERO *)Data)->dwHeroLocationY	= List->y;
				((GAMEHERO *)Data)->bGlobalGoldHeld	= (BYTE)List->goldheld;
				((GAMEHERO *)Data)->bGlobalLevel	= (BYTE)List->data.level;
				((GAMEHERO *)Data)->bGlobalBehavior	= (BYTE)List->creaturesoptions;
				((GAMEHERO *)Data)->bHeroTargetArea	= (BYTE)List->area;
				((GAMEHERO *)Data)->dwGlobalHealth	= List->health;
				((GAMEHERO *)Data)->wHeroTrigger	= Triggers_GetFirstTrigger(List->uniqueid,SCOPE_CREATURE,NULL);
				((GAMEHERO *)Data)->bGlobalPlayer	= (BYTE)List->ennemy;
				((GAMEHERO *)Data)->bHeroObjective	= (BYTE)List->objective;
				((GAMEHERO *)Data)->bGlobalCreatureID	= (BYTE)List->id;
				((GAMEHERO *)Data)->bHeroBandID		= (BYTE)0xFF;
				((GAMEHERO *)Data)->bHeroOptions	= (BYTE)List->herosoptions;
				DataLen = sizeof(GAMEHERO);
				break;
			case 2:	Data = GlobalAlloc(GPTR,sizeof(GAMENEUTRAL));
				if (!Data) return(0);
				((GAMENEUTRAL *)Data)->dwNeutralHeader	  = 0x000000C6;
				((GAMENEUTRAL *)Data)->dwNeutralVersion	  = 0x00000018;
				((GAMENEUTRAL *)Data)->dwNeutralLocationX = List->x;
				((GAMENEUTRAL *)Data)->dwNeutralLocationY = List->y;
				((GAMENEUTRAL *)Data)->bGlobalGoldHeld	  = (BYTE)List->goldheld;
				((GAMENEUTRAL *)Data)->bGlobalLevel	  = (BYTE)List->data.level;
				((GAMENEUTRAL *)Data)->bGlobalBehavior	  = (BYTE)List->creaturesoptions;
				((GAMENEUTRAL *)Data)->dwGlobalHealth	  = List->health;
				((GAMENEUTRAL *)Data)->wNeutralTrigger	  = Triggers_GetFirstTrigger(List->uniqueid,SCOPE_CREATURE,NULL);
				((GAMENEUTRAL *)Data)->bGlobalCreatureID  = (BYTE)List->id;
				DataLen = sizeof(GAMENEUTRAL);
				break;
			default:Data = GlobalAlloc(GPTR,sizeof(GAMEKEEPER));
				if (!Data) return(0);
				((GAMEKEEPER *)Data)->dwKeeperHeader	= 0x000000C8;
				((GAMEKEEPER *)Data)->dwKeeperVersion	= 0x0000001C;
				((GAMEKEEPER *)Data)->dwKeeperLocationX	= List->x;
				((GAMEKEEPER *)Data)->dwKeeperLocationY	= List->y;
				((GAMEKEEPER *)Data)->bGlobalGoldHeld	= (BYTE)List->goldheld;
				((GAMEKEEPER *)Data)->bGlobalLevel	= (BYTE)List->data.level;
				((GAMEKEEPER *)Data)->bGlobalBehavior	= (BYTE)List->creaturesoptions;
				((GAMEKEEPER *)Data)->dwGlobalHealth	= List->health;
				((GAMEKEEPER *)Data)->wKeeperTrigger	= Triggers_GetFirstTrigger(List->uniqueid,SCOPE_CREATURE,NULL);
				((GAMEKEEPER *)Data)->bGlobalCreatureID	= (BYTE)List->id;
				((GAMEKEEPER *)Data)->bGlobalPlayer	= (BYTE)List->owner;
				DataLen = sizeof(GAMEKEEPER);
				break;
			}

		if (FileHandle)
			{
			WriteFile(FileHandle,Data,DataLen,&Written,NULL);
			if (Written != DataLen)
				{
				GlobalFree(Data);
				return(0);
				}
			}

		*TotalThings += 1;
		*TotalLen += DataLen;
		GlobalFree(Data);
		}

	return(1);
}


// «»»» Ecriture des zones ««««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateAreas(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEAREA	 Area;
	MAPRECTEX	*List;
	DWORD		 Written;

	ZeroMemory(&Area,sizeof(GAMEAREA));
	Area.dwAreaHeader	= 0x000000C5;
	Area.dwAreaVersion	= 0x00000038;

	for (List = (MAPRECTEX *)MapAreas.next; List != 0; List = (MAPRECTEX *)List->node.next)
		{
		Area.dwAreaLocationStartX	= List->rect.x1;
		Area.dwAreaLocationStartY	= List->rect.y1;
		Area.dwAreaLocationEndX		= List->rect.x2;
		Area.dwAreaLocationEndY		= List->rect.y2;
		Area.wAreaWaitDelay		= (WORD)List->waitdelay;
		Area.bAreaFlags			= (BYTE)List->flags;
		Area.wAreaTrigger		= Triggers_GetFirstTrigger(List->id,SCOPE_AREA,NULL);
		Area.bAreaID			= (BYTE)List->id;
		Area.bAreaNextID		= (BYTE)List->nextid;

		if (FileHandle)
			{
			WriteFile(FileHandle,&Area,sizeof(GAMEAREA),&Written,NULL);
			if (Written != sizeof(GAMEAREA)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEAREA);
		}

	return(1);
}


// «»»» Ecriture des portails «««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateGates(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEGATE	 Gate;
	MAPRECTEX	*List;
	DWORD		 Written;
	UINT		 Player;
	LONG		 X,Y;

	ZeroMemory(&Gate,sizeof(GAMEGATE));
	Gate.dwGateHeader	= 0x000000CC;
	Gate.dwGateVersion	= 0x00000014;

	//--- Writes heroes gates ---

	for (List = (MAPRECTEX *)MapGates.next; List != 0; List = (MAPRECTEX *)List->node.next)
		{
		Compiler_InfoGateType(List->rect.x1,List->rect.y1,&X,&Y);
		Gate.dwGateLocationX	= List->rect.x1;
		Gate.dwGateLocationY	= List->rect.y1;
		Gate.bGateOrientation	= (BYTE)X;
		Gate.bGateID		= 1;
		Gate.wGateHealth	= 0x64;
		Gate.bGateType		= (BYTE)Y;
		Gate.bGlobalPlayer	= 0x01;

		if (FileHandle)
			{
			WriteFile(FileHandle,&Gate,sizeof(GAMEGATE),&Written,NULL);
			if (Written != sizeof(GAMEGATE)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEGATE);
		}

	//--- Writes players gates ---

	for (Player = 3; Player != MAP_MAXPLAYERS+1; Player++)
		{
		if (!Compiler_InfoHeartPosition(Player,&X,&Y)) continue;
		Gate.dwGateLocationX	= X;
		Gate.dwGateLocationY	= Y;
		Gate.bGateOrientation	= 0x00;
		Gate.bGateID		= 0;
		Gate.wGateHealth	= (WORD)MapOptions[Player-1].PlayerHealth;
		Gate.bGateType		= 0x0E;
		Gate.bGlobalPlayer	= (BYTE)Player;

		if (FileHandle)
			{
			WriteFile(FileHandle,&Gate,sizeof(GAMEGATE),&Written,NULL);
			if (Written != sizeof(GAMEGATE)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEGATE);
		}

	return(1);
}


// «»»» Ecriture des zones ««««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateEffects(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	GAMEEFFECT	 Effect;
	MAPRECTEX	*List;
	DWORD		 Written;

	ZeroMemory(&Effect,sizeof(GAMEEFFECT));
	Effect.dwEffectHeader	= 0x000000CB;
	Effect.dwEffectVersion	= 0x00000024;

	for (List = (MAPRECTEX *)MapEffects.next; List != 0; List = (MAPRECTEX *)List->node.next)
		{
		Effect.dwEffectStartX		= List->rect.x1;
		Effect.dwEffectStartY		= List->rect.y1;
		Effect.wEffectWidth		= (WORD)(List->rect.x2-List->rect.x1);
		Effect.wEffectHeight		= (WORD)(List->rect.y2-List->rect.y1);
		Effect.wEffect1			= (WORD)List->effect1;
		Effect.wEffect2			= (WORD)List->effect2;
		Effect.wEffect3			= (WORD)List->effect3;
		Effect.wEffect4			= (WORD)List->effect4;
		Effect.bEffectFrequency		= (BYTE)List->freq;
		Effect.bEffectID		= (BYTE)List->id;

		if (FileHandle)
			{
			WriteFile(FileHandle,&Effect,sizeof(GAMEEFFECT),&Written,NULL);
			if (Written != sizeof(GAMEEFFECT)) return(0);
			}

		*TotalThings += 1;
		*TotalLen += sizeof(GAMEEFFECT);
		}

	return(1);
}


// «»»» Ecriture des bandes de héros ««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateHeros(HANDLE FileHandle, DWORD *TotalThings, DWORD *TotalLen)
{
	BYTE		 GameHeros[564];
	GAMEBANDHERO	*GameHero;
	DWORD		 Written;
	MTHBAND		*List;
	MAPTHINGHERO	*Hero;

	for (List = (MTHBAND *)MapHerosBands.next; List != 0; List = (MTHBAND *)List->node.next)
		{
		ZeroMemory(GameHeros,564);
		((GAMEBAND *)GameHeros)->dwBandHeader	= 0x000000C9;
		((GAMEBAND *)GameHeros)->dwBandSize	= 0x0000022B;
		((GAMEBAND *)GameHeros)->dwBandVersion	= 0x00000009;
		((GAMEBAND *)GameHeros)->dwBandUnknown0	= 0x00000000;	// 0x018E45A0;
		((GAMEBAND *)GameHeros)->dwBandUnknown1	= 0x00000000;	// 0x0115EEA0;
		((GAMEBAND *)GameHeros)->wBandUnknown2	= 0x0000;	// 0x2E7E;
		((GAMEBAND *)GameHeros)->wBandUnknown3	= 0x0000;	// 0x0058;
		((GAMEBAND *)GameHeros)->wBandUnknown4	= 0x0000;	// 0x0111;
		((GAMEBAND *)GameHeros)->wBandUnknown5	= 0x0000;
		((GAMEBAND *)GameHeros)->wBandUnknown6	= 0x0000;
		((GAMEBAND *)GameHeros)->wBandUnknown7	= 0x0000;
		((GAMEBAND *)GameHeros)->wBandUnknown8	= 0x0000;	// 0x0001;
		((GAMEBAND *)GameHeros)->wBandUnknown9	= 0x0000;
		((GAMEBAND *)GameHeros)->wBandUnknown10	= 0x0000;	// 0x72E0;
		((GAMEBAND *)GameHeros)->wBandUnknown11	= 0x0000;	// 0x00E8;
		((GAMEBAND *)GameHeros)->wBandTrigger	= Triggers_GetFirstTrigger(List->uniqueid,SCOPE_BAND,NULL);
		((GAMEBAND *)GameHeros)->bBandUnknown13	= 0x00;
		((GAMEBAND *)GameHeros)->bBandUnknown15	= 0x00;		// 0x0C;
		((GAMEBAND *)GameHeros)->bBandUnknown16	= 0x00;		// 0xF7;
		((GAMEBAND *)GameHeros)->bBandUnknown17	= 0x00;		// 0x15;
		((GAMEBAND *)GameHeros)->bBandUnknown18	= 0x00;		// 0x01;
		((GAMEBAND *)GameHeros)->bBandUnknown19	= 0x00;		// 0x0C;
		((GAMEBAND *)GameHeros)->bBandUnknown20	= 0x00;		// 0xF7;
		((GAMEBAND *)GameHeros)->bBandUnknown21	= 0x00;		// 0x15;
		((GAMEBAND *)GameHeros)->bBandID = (BYTE)List->id;

		for (GameHero = (GAMEBANDHERO *)(&GameHeros[51]), Hero = List->heros; Hero->isvalid != 0; Hero++, GameHero++)
			{
			GameHero->dwHeroLocationX	= 0x00000000;
			GameHero->dwHeroLocationY	= 0x00000000;
			GameHero->dwHeroUnknown0	= 0x00000000;
			GameHero->bGlobalGoldHeld	= (BYTE)Hero->goldheld;
			GameHero->bHeroUnknown1		= 0x00;
			GameHero->bGlobalLevel		= (BYTE)Hero->level;
			GameHero->bGlobalBehavior	= (BYTE)Hero->options2;
			GameHero->bHeroTargetArea	= (BYTE)Hero->area;
			GameHero->bHeroUnknown2		= 0x00;
			GameHero->bHeroUnknown3		= 0x00;
			GameHero->bHeroUnknown4		= 0x00;
			GameHero->dwGlobalHealth	= Hero->health;
			GameHero->wHeroTrigger		= Triggers_GetFirstTrigger(Hero->uniqueid,SCOPE_CREATUREINBAND,NULL);
			GameHero->bGlobalPlayer		= (BYTE)Hero->enemy;
			GameHero->bHeroObjective	= (BYTE)Hero->objective;
			GameHero->bGlobalCreatureID	= (BYTE)Hero->id;
			GameHero->bHeroUnknown7		= 0x00;
			GameHero->bHeroBandID		= (BYTE)List->id;
			GameHero->bHeroOptions		= (BYTE)Hero->options;
			}

		if (FileHandle)
			{
			WriteFile(FileHandle,GameHeros,563,&Written,NULL);
			if (Written != 563) return(0);
			}

		*TotalThings += 1;
		*TotalLen += 563;
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Map.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateMapFile(char *FileName)
{
	MAPINFO	 EditorMapInfo;
	HANDLE	 FileHandle;
	DWORD	 Written;
	GAMEMAP	*GameMap;
	DWORD	*MapPtr;
	char	*Extension;
	char	*WorkName;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}
	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameMapExt);

	//--- Création du contenu du fichier ---

	GameMap = GlobalAlloc(GPTR,sizeof(GAMEMAP));
	if (!GameMap)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	GameMap->dwMapFileHeader	= 0x00000064;
	GameMap->dwMapFileVersion	= 0x00000004;
	GameMap->dwMapFileSize		= Map.Properties.Width*4*Map.Properties.Height+sizeof(GAMEMAP);
	GameMap->dwMapInfoHeader	= 0x00000065;
	GameMap->dwMapInfoVersion	= 0x00000008;
	GameMap->dwMapWidth		= Map.Properties.Width;
	GameMap->dwMapHeight		= Map.Properties.Height;
	GameMap->dwMapDefineHeader	= 0x00000066;
	GameMap->dwMapDefineSize	= Map.Properties.Width*4*Map.Properties.Height;

	WriteFile(FileHandle,GameMap,sizeof(GAMEMAP),&Written,NULL);
	if (Written != sizeof(GAMEMAP))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}

	//--- Création de la carte ---

	EditorMapInfo.Map	= Map.Map;
	EditorMapInfo.Width	= Map.Properties.Width;
	EditorMapInfo.Height	= Map.Properties.Height;
	EditorMapInfo.RectX	= 0;
	EditorMapInfo.RectY	= 0;
	EditorMapInfo.RectWidth = Map.Properties.Width;
	EditorMapInfo.RectHeight= Map.Properties.Height;
	MapPtr = Compiler_EditorMapToGameMap(&EditorMapInfo);
	if (!MapPtr) goto Error_3;

	WriteFile(FileHandle,MapPtr,Map.Properties.Width*4*Map.Properties.Height,&Written,NULL);
	if (Written != Map.Properties.Width*4*Map.Properties.Height) goto Error_4;

	CloseHandle(FileHandle);
	GlobalFree(MapPtr);
	GlobalFree(GameMap);
	GlobalFree(WorkName);

	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	GlobalFree(MapPtr);
Error_3:CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(GameMap);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Variables.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateVariablesFile(char *FileName)
{
	GAMEVARIABLES	*Variables;
	BYTE		*VariablesPtr;
	DWORD		 VariablesCount;
	HANDLE		 FileHandle;
	DWORD		 Written;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameVariablesExt);

	VariablesCount = 0;
	for (i = 1; i != MAP_MAXPLAYERS+1; i++) Compiler_GeneratePlayerVariables(NULL,&VariablesCount,i);
	Compiler_GenerateCreaturesVariables(NULL,&VariablesCount);
	Compiler_GenerateAlliesVariables(NULL,&VariablesCount);
	Compiler_GenerateGlobalVariables(NULL,&VariablesCount);

	Variables = GlobalAlloc(GPTR,sizeof(GAMEVARIABLES)+VariablesCount*16);
	if (!Variables)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Variables->dwVariablesFileHeader	= 0x000000E6;
	Variables->dwVariablesFileVersion	= 0x00000004;
	Variables->dwVariablesFileSize		= sizeof(GAMEVARIABLES)+VariablesCount*16;
	Variables->dwVariablesInfoHeader	= 0x000000E7;
	Variables->dwGlobalInfoHeader		= 0x0000001C;
	Variables->dwVariablesCount		= VariablesCount;
	Variables->dwGlobalDateHeader 		= 0x00000001;
	Compiler_SetDate(&Variables->dateVariablesDate);
	Variables->dwVariablesDefinesHeader	= 0x000000E8;
	Variables->dwVariablesDefinesSize	= VariablesCount*16;

	VariablesPtr = ((BYTE *)Variables)+sizeof(GAMEVARIABLES);
	for (i = 1; i != MAP_MAXPLAYERS+1; i++) VariablesPtr = Compiler_GeneratePlayerVariables(VariablesPtr,NULL,i);
	VariablesPtr = Compiler_GenerateCreaturesVariables(VariablesPtr,NULL);
	VariablesPtr = Compiler_GenerateAlliesVariables(VariablesPtr,NULL);
	Compiler_GenerateGlobalVariables(VariablesPtr,NULL);

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}
	WriteFile(FileHandle,Variables,Variables->dwVariablesFileSize,&Written,NULL);
	if (Written != Variables->dwVariablesFileSize) goto Error_3;

	CloseHandle(FileHandle);
	GlobalFree(Variables);
	GlobalFree(WorkName);

	return(1);

//--- Erreurs ---

Error_3:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(Variables);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// «»»» Ecriture des variables des joueurs ««««««««««««««««««««««««««««««»

BYTE* Compiler_GeneratePlayerVariables(BYTE *VariablesPtr, DWORD *Count, UINT Player)
{
	LONG	X,Y;

	if (!Compiler_InfoHeartPosition(Player,&X,&Y)) return(VariablesPtr);

	//--- Genere le nombre de créatures ---

	for (X = 0, Y = 0; BadCreatures[X].Id != 0; X++, Y++)
		{
		if (MapOptions[Player-1].CreaturesCount[Y] == -1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000001;
			*((ULONG *)VariablesPtr)++ = BadCreatures[X].Variable;
			*((ULONG *)VariablesPtr)++ = MapOptions[Player-1].CreaturesCount[Y];
			*((ULONG *)VariablesPtr)++ = (ULONG)Player;
			}
		if (Count) *Count += 1;
		}

	for (X = 0; BadElites[X].Id != 0; X++, Y++)
		{
		if (MapOptions[Player-1].CreaturesCount[Y] == -1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000001;
			*((ULONG *)VariablesPtr)++ = BadElites[X].Variable;
			*((ULONG *)VariablesPtr)++ = MapOptions[Player-1].CreaturesCount[Y];
			*((ULONG *)VariablesPtr)++ = (ULONG)Player;
			}
		if (Count) *Count += 1;
		}

	for (X = 0; Heros[X].Id != 0; X++, Y++)
		{
		if (MapOptions[Player-1].CreaturesCount[Y] == -1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000001;
			*((ULONG *)VariablesPtr)++ = Heros[X].Variable;
			*((ULONG *)VariablesPtr)++ = MapOptions[Player-1].CreaturesCount[Y];
			*((ULONG *)VariablesPtr)++ = (ULONG)Player;
			}
		if (Count) *Count += 1;
		}

	//--- Genere les salles ---

	for (X = 0; VarRooms[X].Id != 0; X++)
		{
		if (MapOptions[Player-1].Rooms[X] == (BYTE)-1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000002;
			*((WORD *)VariablesPtr)++  = 0x0001;
			*((WORD *)VariablesPtr)++  = (WORD)Player;
			*((ULONG *)VariablesPtr)++ = VarRooms[X].Variable;
			*((ULONG *)VariablesPtr)++ = (ULONG)MapOptions[Player-1].Rooms[X];
			}
		if (Count) *Count += 1;
		}

	//--- Genere les pièges ---

	for (X = 0; Traps[X].Id != 0; X++)
		{
		if (MapOptions[Player-1].Traps[X] == (BYTE)-1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000002;
			*((WORD *)VariablesPtr)++  = 0x0003;
			*((WORD *)VariablesPtr)++  = (WORD)Player;
			*((ULONG *)VariablesPtr)++ = Traps[X].Variable;
			*((ULONG *)VariablesPtr)++ = (ULONG)MapOptions[Player-1].Traps[X];
			}
		if (Count) *Count += 1;
		}

	//--- Genere les portes ---

	for (X = 0; Doors[X].Id != 0; X++)
		{
		if (MapOptions[Player-1].Doors[X] == (BYTE)-1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000002;
			*((WORD *)VariablesPtr)++  = 0x0004;
			*((WORD *)VariablesPtr)++  = (WORD)Player;
			*((ULONG *)VariablesPtr)++ = Doors[X].Variable;
			*((ULONG *)VariablesPtr)++ = (ULONG)MapOptions[Player-1].Doors[X];
			}
		if (Count) *Count += 1;
		}

	//--- Genere les sorts ---

	for (X = 0; Spells[X].Id != 0; X++)
		{
		if (MapOptions[Player-1].Spells[X] == (BYTE)-1) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = 0x00000002;
			*((WORD *)VariablesPtr)++  = 0x0005;
			*((WORD *)VariablesPtr)++  = (WORD)Player;
			*((ULONG *)VariablesPtr)++ = Spells[X].Variable;
			*((ULONG *)VariablesPtr)++ = (ULONG)MapOptions[Player-1].Spells[X];
			}
		if (Count) *Count += 1;
		}

	return(VariablesPtr);
}


// «»»» Ecriture des variables des créatures ««««««««««««««««««««««««««««»

BYTE* Compiler_GenerateCreaturesVariables(BYTE *VariablesPtr, DWORD *Count)
{
	int	l;
	int	i,j;

	for (l = 0, i = 0; BadCreatures[i].Id != 0; i++, l++)
		{
		for (j = 1; j != 4+1; j++)
			{
			if (!Map.Properties.CreaturesParams[l][j-1]) continue;
			if (VariablesPtr)
				{
				*((LONG *)VariablesPtr)++ = 0x0000004D;
				*((LONG *)VariablesPtr)++ = BadCreatures[i].Id;
				*((LONG *)VariablesPtr)++ = j;
				*((LONG *)VariablesPtr)++ = Map.Properties.CreaturesParams[l][j-1];
				}
			if (Count) *Count += 1;
			}
		}

	for (i = 0; BadElites[i].Id != 0; i++, l++);

	for (i = 0; Heros[i].Id != 0; i++, l++)
		{
		for (j = 1; j != 4+1; j++)
			{
			if (!Map.Properties.CreaturesParams[l][j-1]) continue;
			if (VariablesPtr)
				{
				*((LONG *)VariablesPtr)++ = 0x0000004D;
				*((LONG *)VariablesPtr)++ = Heros[i].Id;
				*((LONG *)VariablesPtr)++ = j;
				*((LONG *)VariablesPtr)++ = Map.Properties.CreaturesParams[l][j-1];
				}
			if (Count) *Count += 1;
			}
		}

	return(VariablesPtr);
}


// «»»» Ecriture des alliances ««««««««««««««««««««««««««««««««««««««««««»

BYTE* Compiler_GenerateAlliesVariables(BYTE *VariablesPtr, DWORD *Count)
{
	int	i,j;

	for (i = 0; i != MAP_MAXPLAYERS; i++)
		for (j = i; j != MAP_MAXPLAYERS; j++)
			if (Map.Properties.Allies[i][j])
				{
				if (VariablesPtr)
					{
					*((ULONG *)VariablesPtr)++ = 0x00000042;
					*((ULONG *)VariablesPtr)++ = i+1;
					*((ULONG *)VariablesPtr)++ = j+1;
					*((ULONG *)VariablesPtr)++ = 0;
					}
				if (Count) *Count += 1;
				}

	return(VariablesPtr);
}


// «»»» Ecriture des variables globales «««««««««««««««««««««««««««««««««»

BYTE* Compiler_GenerateGlobalVariables(BYTE *VariablesPtr, DWORD *Count)
{
	int	X;

	//--- Genere les variables globales ---

	for (X = 0; MapVars[X].Id != 0; X++)
		{
		if (MapVars[X].Id == 0xFFFFFFFF) continue;
		if (Map.Properties.Variables[MapVars[X].Id] == MapVars[X].Variable) continue;
		if (MapVars[X].Icon) continue;
		if (VariablesPtr)
			{
			*((ULONG *)VariablesPtr)++ = MapVars[X].Id;
			*((ULONG *)VariablesPtr)++ = Map.Properties.Variables[MapVars[X].Id];
			*((ULONG *)VariablesPtr)++ = 0x00000000;
			*((ULONG *)VariablesPtr)++ = 0x00000000;
			}
		if (Count) *Count += 1;
		}

	return(VariablesPtr);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Triggers.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateTriggersFile(char *FileName)
{
	GAMETRIGGERS	*Triggers;
	HANDLE		 FileHandle;
	DWORD		 Written;
	char		*WorkName;
	char		*Extension;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameTriggersExt);

	//--- Ecriture du contenu du fichier ---

	Triggers = GlobalAlloc(GPTR,sizeof(GAMETRIGGERS));
	if (!Triggers)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Triggers->dwTriggersFileHeader		= 0x000000D2;
	Triggers->dwTriggersFileVersion		= 0x00000004;
	Triggers->dwTriggersFileSize		= sizeof(GAMETRIGGERS)+List_EntryCount(&MapTriggers)*8+List_EntryCount(&MapActions)*8;
	Triggers->dwTriggersInfoHeader		= 0x000000D3;
	Triggers->dwTriggersInfoVersion		= 0x00000020;
	Triggers->dwTriggersCount		= List_EntryCount(&MapTriggers);
	Triggers->dwTriggersActionsCount	= List_EntryCount(&MapActions);
	Triggers->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&Triggers->dateTriggersDate);
	Triggers->dwTriggersDefinesHeader	= 0x000000D4;
	Triggers->dwTriggersDefinesSize		= Triggers->dwTriggersCount*16;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}
	WriteFile(FileHandle,Triggers,sizeof(GAMETRIGGERS),&Written,NULL);
	if (Written != sizeof(GAMETRIGGERS)) goto Error_3;
	if (!Compiler_GenerateTriggers(FileHandle)) goto Error_3;
	if (!Compiler_GenerateActions(FileHandle)) goto Error_3;

	CloseHandle(FileHandle);

	GlobalFree(Triggers);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_3:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_2:GlobalFree(Triggers);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// «»»» Ecriture des déclencheurs «««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateTriggers(HANDLE FileHandle)
{
	BYTE		 TriggerDatas[24];
	DWORD		 TriggerSize;
	TRIGGER		*Trigger;
	void		*TriggerPtr;

	for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
		{
		TriggerPtr = TriggerDatas;
		*((DWORD *)TriggerPtr)++ = 0x000000D5;
		*((DWORD *)TriggerPtr)++ = 0x00000010;

		switch(Trigger->ctrl)
			{
			case TRIGGER_VARIABLE:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param3;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param4;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_TIMER:
			case TRIGGER_CREATURES:
			case TRIGGER_HAPPYCREATURES:
			case TRIGGER_ANGRYCREATURES:
			case TRIGGER_ROOMS:
			case TRIGGER_ROOMSTYPES:
			case TRIGGER_ROOMSSIZE:
			case TRIGGER_ROOMSCONTENT:
			case TRIGGER_GROUPINPOSSESS:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x01;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_CLAIMAREA:
			case TRIGGER_SELECTAREA:
			case TRIGGER_JAILEDINBAND:
			case TRIGGER_DEATHSINBAND:
			case TRIGGER_STUNNEDINBAND:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_BANDCREATED:
				*((BYTE *)TriggerPtr)++ = 0x03;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_DOORDESTROYED:
			case TRIGGER_OBJECTCLAIMED:
			case TRIGGER_APPEAR:
			case TRIGGER_DEATH:
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
				*((LONG *)TriggerPtr)++ = 0x00000000;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_SPELLS:
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x01;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_SELECTALLAREA:
			case TRIGGER_ENTERAREAINPOSS:
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_GOLDMINED:
			case TRIGGER_GOLDAMOUNT:
			case TRIGGER_MANAAMOUNT:
			case TRIGGER_SLAPCOUNT:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x01;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_TIMEELAPSED:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_HURTED:
			case TRIGGER_CREATUREEXP:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param5;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				break;
			case TRIGGER_CREAENTERAREA:
			case TRIGGER_AREACONTENT:
			case TRIGGER_DOORS:
			case TRIGGER_TRAPS:
			case TRIGGER_DESTROYED:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param3;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			case TRIGGER_CLAIMENTIREAREA:
				*((BYTE *)TriggerPtr)++ = 0x05;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_CLICKELEMENT:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param2;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_HEARTDESTROYED:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((LONG *)TriggerPtr)++ = 0x00000000;
				break;
			case TRIGGER_CREATURESLEVEL:
			case TRIGGER_LOSTCREATURES:
			case TRIGGER_KILLEDCREATURES:
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param1;
				*((BYTE *)TriggerPtr)++ = 0x00;
				*((BYTE *)TriggerPtr)++ = 0x01;
				*((BYTE *)TriggerPtr)++ = (BYTE)Trigger->param4;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			default:*((LONG *)TriggerPtr)++ = Trigger->param1;
				*((LONG *)TriggerPtr)++ = Trigger->param5;
				break;
			}

		*((WORD *)TriggerPtr)++ = Trigger->id;
		*((WORD *)TriggerPtr)++ = Trigger->or;
		*((WORD *)TriggerPtr)++ = Trigger->and;
		*((BYTE *)TriggerPtr)++ = Trigger->ctrl;
		*((BYTE *)TriggerPtr)   = Trigger->repeat;

		WriteFile(FileHandle,TriggerDatas,24,&TriggerSize,NULL);
		if (TriggerSize != 24) return(0);
		}

	return(1);
}


// «»»» Ecriture des actions ««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_GenerateActions(HANDLE FileHandle)
{
	BYTE	 	 ActionDatas[24];
	DWORD		 ActionSize;
	ACTION		*Action;
	void		*ActionPtr;

	for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
		{
		ActionPtr = ActionDatas;
		*((DWORD *)ActionPtr)++ = 0x000000D6;
		*((DWORD *)ActionPtr)++ = 0x00000010;

		switch(Action->ctrl)
			{
			case ACTION_ADDCREATURE:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param3;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param4;
				*((WORD *)ActionPtr)++ = (WORD)Action->param5;
				*((WORD *)ActionPtr)++ = (WORD)Action->param7;
				break;
			case ACTION_SHOWCRYSTAL:
				*((BYTE *)ActionPtr)++ = 0x1C;
				*((BYTE *)ActionPtr)++ = 0x01;
				*((WORD *)ActionPtr)++ = 0x0000000;
				*((WORD *)ActionPtr)++ = (WORD)Things_GetThingPos(&MapObjects,0x1C,0);
				*((WORD *)ActionPtr)++ = (WORD)Things_GetThingPos(&MapObjects,0x1C,1);
				break;
			case ACTION_ADDELEMENT:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param3;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param4;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_SETVARIABLE:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
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
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_ALLIES:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param3;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_SHOWELEMENT:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param3;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
			case ACTION_VICTORY:
			case ACTION_DEFEAT:
			case ACTION_HASCRYSTAL:
			case ACTION_SHOW:
			case ACTION_POSSESSCREATURE:
			case ACTION_REMOVE:
			case ACTION_MAKEHUNGRY:
				*((LONG *)ActionPtr)++ = 0x00000000;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_RELEASEBAND:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((WORD *)ActionPtr)++ = 0x0000;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
			case ACTION_CHANGEPARAMS:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param5;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				break;
			case ACTION_SHOWAREA:
			case ACTION_ENABLEFOG:
			case ACTION_GENERATE:
			case ACTION_TOGGLEEFFECT:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_CHANGESQUARE:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((WORD *)ActionPtr)++ = (WORD)Action->param5;
				*((WORD *)ActionPtr)++ = (WORD)Action->param7;
				break;
			case ACTION_DISPLAYTIMER:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
			case ACTION_SAY:
				*((WORD *)ActionPtr)++ = (WORD)Action->param1;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param5;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((WORD *)ActionPtr)++ = 0x0000;
				break;
			case ACTION_ROTATECAMERA:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((WORD *)ActionPtr)++ = (WORD)Action->param3;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param5;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				break;
			case ACTION_DESTROYGATE:
				*((LONG *)ActionPtr)++ = 0x00000000;
				*((WORD *)ActionPtr)++ = (WORD)(RectEx_GetRectLocation(&MapGates,Action->param5,0)+1);
				*((WORD *)ActionPtr)++ = (WORD)(RectEx_GetRectLocation(&MapGates,Action->param5,1)+1);
				break;
			case ACTION_CHANGEOWNER:
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((WORD *)ActionPtr)++ = (WORD)Action->param5;
				*((WORD *)ActionPtr)++ = (WORD)Action->param7;
				break;
			case ACTION_SLAPSREMAINING:
				*((LONG *)ActionPtr)++ = 0x00000000;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
			case ACTION_CAMERAPATH:
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param1;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param2;
				*((BYTE *)ActionPtr)++ = (BYTE)Action->param3;
				*((BYTE *)ActionPtr)++ = 0x00;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			case ACTION_SHOWHEALTH:
				*((LONG *)ActionPtr)++ = Action->param1;
				*((LONG *)ActionPtr)++ = 0x00000000;
				break;
			default:*((LONG *)ActionPtr)++ = Action->param1;
				*((LONG *)ActionPtr)++ = Action->param5;
				break;
			}

		*((WORD *)ActionPtr)++ = (WORD)Action->id;
		*((WORD *)ActionPtr)++ = (WORD)Action->next;
		*((WORD *)ActionPtr)++ = 0x0000;
		*((BYTE *)ActionPtr)++ = (BYTE)Action->ctrl;
		*((BYTE *)ActionPtr)   = (BYTE)Action->one;

		WriteFile(FileHandle,ActionDatas,24,&ActionSize,NULL);
		if (ActionSize != 24) return(0);
		}

	return(1);

}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Terrains.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateTerrainsFile(char *FileName)
{
	GAMETERRAINS	*Terrains;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Terrain;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameTerrainExt);

	//--- Ecriture du contenu du fichier ---

	Terrains = GlobalAlloc(GPTR,sizeof(GAMETERRAINS));
	if (!Terrains)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Terrain = GlobalAlloc(GPTR,552);
	if (!Terrain)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Terrains->dwTerrainsFileHeader		= 0x0000006E;
	Terrains->dwTerrainsFileVersion		= 0x00000004;
	Terrains->dwTerrainsFileSize		= 22688; // sizeof(GAMETERRAINS)+41*552;
	Terrains->dwTerrainsInfoHeader		= 0x0000006F;
	Terrains->dwTerrainsInfoVersion		= 0x0000001C;
	Terrains->dwTerrainsCount		= 41;
	Terrains->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&Terrains->dateTerrainsDate);
	Terrains->dwTerrainsDefinesHeader	= 0x00000070;
	Terrains->dwTerrainsDefinesSize		= 41*552;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Terrains,sizeof(GAMETERRAINS),&Written,NULL);
	if (Written != sizeof(GAMETERRAINS)) goto Error_3;

	for (i = 0; GTOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Terrain,GTOrder[i+1],552);
		if (GTOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GTOrder[i];
			*((BYTE *)(Terrain+0x1D9))  = (BYTE)MapGlobalVariables.Terrains[Offset].MaxHealthType;
			*((WORD *)(Terrain+0x1A4))  = (WORD)MapGlobalVariables.Terrains[Offset].MaxHealthEffect;
			*((BYTE *)(Terrain+0x1DA))  = (BYTE)MapGlobalVariables.Terrains[Offset].DestroyedType;
			*((WORD *)(Terrain+0x1A6))  = (WORD)MapGlobalVariables.Terrains[Offset].DestroyedEffect;
			*((short *)(Terrain+0x194)) = (short)MapGlobalVariables.Terrains[Offset].Damage;
			*((WORD *)(Terrain+0x19A))  = (WORD)MapGlobalVariables.Terrains[Offset].GoldValue;
			*((WORD *)(Terrain+0x1FF))  = (WORD)MapGlobalVariables.Terrains[Offset].MaxHealth;
			*((WORD *)(Terrain+0x1D7))  = (WORD)MapGlobalVariables.Terrains[Offset].StartingHealth;
			*((WORD *)(Terrain+0x19C))  = (WORD)MapGlobalVariables.Terrains[Offset].ManaGain;
			*((WORD *)(Terrain+0x19E))  = (WORD)MapGlobalVariables.Terrains[Offset].MaxManaGain;
			*((BYTE *)(Terrain+0x1CE))  = (BYTE)MapGlobalVariables.Terrains[Offset].WibbleH;
			*((BYTE *)(Terrain+0x1D2))  = (BYTE)MapGlobalVariables.Terrains[Offset].WibbleV;
			*((BYTE *)(Terrain+0x193)) &= (BYTE)~0x31;
			*((BYTE *)(Terrain+0x193)) |= (BYTE)((MapGlobalVariables.Terrains[Offset].Visible)?0x01:0x00);
			*((BYTE *)(Terrain+0x193)) |= (BYTE)((MapGlobalVariables.Terrains[Offset].TerrainLight)?0x10:0x00);
			*((BYTE *)(Terrain+0x193)) |= (BYTE)((MapGlobalVariables.Terrains[Offset].AmbientLight)?0x20:0x00);
			*((BYTE *)(Terrain+0x01DB)) = MapGlobalVariables.Terrains[Offset].TerrainLightR;
			*((BYTE *)(Terrain+0x01DC)) = MapGlobalVariables.Terrains[Offset].TerrainLightG;
			*((BYTE *)(Terrain+0x01DD)) = MapGlobalVariables.Terrains[Offset].TerrainLightB;
			*((BYTE *)(Terrain+0x0201)) = MapGlobalVariables.Terrains[Offset].AmbientLightR;
			*((BYTE *)(Terrain+0x0202)) = MapGlobalVariables.Terrains[Offset].AmbientLightG;
			*((BYTE *)(Terrain+0x0203)) = MapGlobalVariables.Terrains[Offset].AmbientLightB;
			*((LONG *)(Terrain+0x018C)) = MapGlobalVariables.Terrains[Offset].TerrainLightH;
			}
		WriteFile(FileHandle,Terrain,552,&Written,NULL);
		if (Written != 552) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Terrain);
	GlobalFree(Terrains);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Terrain);
Error_2:GlobalFree(Terrains);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Rooms.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateRoomsFile(char *FileName)
{
	GAMEROOMS	*Rooms;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Room;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameRoomsExt);

	//--- Ecriture du contenu du fichier ---

	Rooms = GlobalAlloc(GPTR,sizeof(GAMEROOMS));
	if (!Rooms)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Room = GlobalAlloc(GPTR,1055);
	if (!Room)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Rooms->dwRoomsFileHeader	= 0x00000078;
	Rooms->dwRoomsFileVersion	= 0x00000004;
	Rooms->dwRoomsFileSize		= 27486; // sizeof(GAMEROOMS)+26*1055;
	Rooms->dwRoomsInfoHeader	= 0x00000079;
	Rooms->dwRoomsInfoVersion	= 0x0000001C;
	Rooms->dwRoomsCount		= 26;
	Rooms->dwGlobalDateHeader	= 0x00000001;
	Compiler_SetDate(&Rooms->dateRoomsDate);
	Rooms->dwRoomsDefinesHeader	= 0x0000007A;
	Rooms->dwRoomsDefinesSize	= 26*1055;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Rooms,sizeof(GAMEROOMS),&Written,NULL);
	if (Written != sizeof(GAMEROOMS)) goto Error_3;

	for (i = 0; GROrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Room,GROrder[i+1],1055);
		if (GROrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GROrder[i];
			*((WORD *)(Room+0x378)) = (WORD)MapGlobalVariables.Rooms[Offset].Cost;
			*((BYTE *)(Room+0x39A)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[0];
			*((BYTE *)(Room+0x39B)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[1];
			*((BYTE *)(Room+0x39C)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[2];
			*((BYTE *)(Room+0x39D)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[3];
			*((BYTE *)(Room+0x39E)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[4];
			*((BYTE *)(Room+0x39F)) = (BYTE)MapGlobalVariables.Rooms[Offset].Objects[5];
			*((WORD *)(Room+0x37A)) = (WORD)MapGlobalVariables.Rooms[Offset].BattleEffect;
			*((WORD *)(Room+0x382)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[0];
			*((WORD *)(Room+0x384)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[1];
			*((WORD *)(Room+0x386)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[2];
			*((WORD *)(Room+0x388)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[3];
			*((WORD *)(Room+0x38A)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[4];
			*((WORD *)(Room+0x38C)) = (WORD)MapGlobalVariables.Rooms[Offset].Effects[5];
			*((BYTE *)(Room+0x36E)) = (BYTE)MapGlobalVariables.Rooms[Offset].TorchIntensity;
			*((WORD *)(Room+0x3C3)) = (WORD)MapGlobalVariables.Rooms[Offset].TorchLightRad;
			*((WORD *)(Room+0x380)) = (WORD)MapGlobalVariables.Rooms[Offset].TorchLightH;
			*((BYTE *)(Room+0x397)) = (BYTE)MapGlobalVariables.Rooms[Offset].TorchLightR;
			*((BYTE *)(Room+0x398)) = (BYTE)MapGlobalVariables.Rooms[Offset].TorchLightG;
			*((BYTE *)(Room+0x399)) = (BYTE)MapGlobalVariables.Rooms[Offset].TorchLightB;
			}
		WriteFile(FileHandle,Room,1055,&Written,NULL);
		if (Written != 1055) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Room);
	GlobalFree(Rooms);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Room);
Error_2:GlobalFree(Rooms);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Doors.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateDoorsFile(char *FileName)
{
	GAMEDOORS	*Doors;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Door;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameDoorsExt);

	//--- Ecriture du contenu du fichier ---

	Doors = GlobalAlloc(GPTR,sizeof(GAMEDOORS));
	if (!Doors)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Door = GlobalAlloc(GPTR,616);
	if (!Door)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Doors->dwDoorsFileHeader	= 0x0000008C;
	Doors->dwDoorsFileVersion	= 0x00000004;
	Doors->dwDoorsFileSize		= 4984; // sizeof(GAMEDOORS)+8*616;
	Doors->dwDoorsInfoHeader	= 0x0000008D;
	Doors->dwDoorsInfoVersion	= 0x0000001C;
	Doors->dwDoorsCount		= 8;
	Doors->dwGlobalDateHeader	= 0x00000001;
	Compiler_SetDate(&Doors->dateDoorsDate);
	Doors->dwDoorsDefinesHeader	= 0x0000008E;
	Doors->dwDoorsDefinesSize	= 8*616;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Doors,sizeof(GAMEDOORS),&Written,NULL);
	if (Written != sizeof(GAMEDOORS)) goto Error_3;

	for (i = 0; GDOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Door,GDOrder[i+1],616);
		if (GDOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GDOrder[i];
			*((WORD *)(Door+0x22E)) = (WORD)MapGlobalVariables.Doors[Offset].Cost;
			*((WORD *)(Door+0x234)) = (WORD)MapGlobalVariables.Doors[Offset].BuildPoints;
			*((WORD *)(Door+0x22C)) = (WORD)MapGlobalVariables.Doors[Offset].Health;
			*((WORD *)(Door+0x21C)) = (WORD)MapGlobalVariables.Doors[Offset].HealthGain;
			*((WORD *)(Door+0x238)) = (WORD)MapGlobalVariables.Doors[Offset].ManaCost;
			*((BYTE *)(Door+0x226)) = (BYTE)MapGlobalVariables.Doors[Offset].Material;
			*((WORD *)(Door+0x232)) = (WORD)MapGlobalVariables.Doors[Offset].DeathEffect;
			*((BYTE *)(Door+0x246)) = (BYTE)MapGlobalVariables.Doors[Offset].Crate;
			*((BYTE *)(Door+0x247)) = (BYTE)MapGlobalVariables.Doors[Offset].Key;
			}
		WriteFile(FileHandle,Door,616,&Written,NULL);
		if (Written != 616) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Door);
	GlobalFree(Doors);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Door);
Error_2:GlobalFree(Doors);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Traps.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateTrapsFile(char *FileName)
{
	GAMETRAPS	*Traps;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Trap;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameTrapsExt);

	//--- Ecriture du contenu du fichier ---

	Traps = GlobalAlloc(GPTR,sizeof(GAMETRAPS));
	if (!Traps)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Trap = GlobalAlloc(GPTR,579);
	if (!Trap)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Traps->dwTrapsFileHeader	= 0x00000082;
	Traps->dwTrapsFileVersion	= 0x00000004;
	Traps->dwTrapsFileSize		= 9320; // sizeof(GAMETRAPS)+16*579;
	Traps->dwTrapsInfoHeader	= 0x00000083;
	Traps->dwTrapsInfoVersion	= 0x0000001C;
	Traps->dwTrapsCount		= 16;
	Traps->dwGlobalDateHeader	= 0x00000001;
	Compiler_SetDate(&Traps->dateTrapsDate);
	Traps->dwTrapsDefinesHeader	= 0x00000084;
	Traps->dwTrapsDefinesSize	= 16*579;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Traps,sizeof(GAMETRAPS),&Written,NULL);
	if (Written != sizeof(GAMETRAPS)) goto Error_3;

	for (i = 0; GAOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Trap,GAOrder[i+1],579);
		if (GAOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GAOrder[i];

			*((WORD *)(Trap+0x1F0)) = (WORD)MapGlobalVariables.Traps[Offset].Health;
			*((WORD *)(Trap+0x241)) = (WORD)MapGlobalVariables.Traps[Offset].HealthGain;
			*((LONG *)(Trap+0x1CC)) = (LONG)MapGlobalVariables.Traps[Offset].ChargeTime;
			*((LONG *)(Trap+0x1C8)) = (LONG)MapGlobalVariables.Traps[Offset].RechargeTime;
			*((WORD *)(Trap+0x1F2)) = (WORD)MapGlobalVariables.Traps[Offset].GoldCost;
			*((WORD *)(Trap+0x1D4)) = (WORD)MapGlobalVariables.Traps[Offset].ManaCostToFire;
			*((WORD *)(Trap+0x202)) = (WORD)MapGlobalVariables.Traps[Offset].ManaUsage;
			*((WORD *)(Trap+0x1EA)) = (WORD)MapGlobalVariables.Traps[Offset].Threat;
			*((LONG *)(Trap+0x1D0)) = (LONG)MapGlobalVariables.Traps[Offset].ThreatDuration;
			*((LONG *)(Trap+0x1D8)) = (LONG)MapGlobalVariables.Traps[Offset].IdleEffectDelay;
			*((LONG *)(Trap+0x23D)) = (LONG)MapGlobalVariables.Traps[Offset].ShotDelay;
			*((WORD *)(Trap+0x1FA)) = (WORD)MapGlobalVariables.Traps[Offset].BuildPoints;

			*((BYTE *)(Trap+0x1EC)) &= ~(0x01|0x02|0x20);
			*((BYTE *)(Trap+0x1ED)) &= ~(0x01);
			*((BYTE *)(Trap+0x1EE)) &= ~(0x01|0x02);
			if (MapGlobalVariables.Traps[Offset].Invisible)		*((BYTE *)(Trap+0x1EC)) |= 0x20;
			if (MapGlobalVariables.Traps[Offset].RevealedWhenFired)	*((BYTE *)(Trap+0x1EC)) |= 0x02;
			if (MapGlobalVariables.Traps[Offset].RequiresMana)	*((BYTE *)(Trap+0x1ED)) |= 0x01;
			if (MapGlobalVariables.Traps[Offset].Disarmable)	*((BYTE *)(Trap+0x1EC)) |= 0x10;
			if (MapGlobalVariables.Traps[Offset].SolidObstacle)	*((BYTE *)(Trap+0x1EE)) |= 0x02;
			if (MapGlobalVariables.Traps[Offset].FPObstacle)	*((BYTE *)(Trap+0x1EE)) |= 0x01;

			*((BYTE *)(Trap+0x22F)) = (BYTE)MapGlobalVariables.Traps[Offset].Material;
			*((WORD *)(Trap+0x1F4)) = (WORD)MapGlobalVariables.Traps[Offset].PowerlessEffect;
			*((WORD *)(Trap+0x1F6)) = (WORD)MapGlobalVariables.Traps[Offset].IdleEffect;
			*((WORD *)(Trap+0x1F8)) = (WORD)MapGlobalVariables.Traps[Offset].DeathEffect;
			*((BYTE *)(Trap+0x20E)) = (BYTE)MapGlobalVariables.Traps[Offset].Crate;
			*((BYTE *)(Trap+0x20D)) = (BYTE)MapGlobalVariables.Traps[Offset].ShotType;
			*((WORD *)(Trap+0x1E0)) = (WORD)MapGlobalVariables.Traps[Offset].ShotData1;
			*((WORD *)(Trap+0x1E4)) = (WORD)MapGlobalVariables.Traps[Offset].ShotData2;
			*((BYTE *)(Trap+0x20B)) = (BYTE)MapGlobalVariables.Traps[Offset].TriggerType;
			*((WORD *)(Trap+0x1DC)) = (WORD)MapGlobalVariables.Traps[Offset].TriggerData;

			if (GShots[MapGlobalVariables.Traps[Offset].ShotType].Variable == 0x01)
				*((WORD *)(Trap+0x1E4)) = 0;
			if (GShots[MapGlobalVariables.Traps[Offset].ShotType].Variable == 0xFF)
				*((WORD *)(Trap+0x1E4)) = 0;
			}
		WriteFile(FileHandle,Trap,579,&Written,NULL);
		if (Written != 579) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Trap);
	GlobalFree(Traps);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Trap);
Error_2:GlobalFree(Traps);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Spells.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateSpellsFile(char *FileName)
{
	GAMESPELLS	*Spells;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Spell;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameKeeperSpellsExt);

	//--- Ecriture du contenu du fichier ---

	Spells = GlobalAlloc(GPTR,sizeof(GAMESPELLS));
	if (!Spells)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Spell = GlobalAlloc(GPTR,406);
	if (!Spell)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Spells->dwSpellsFileHeader	= 0x00000096;
	Spells->dwSpellsFileVersion	= 0x00000004;
	Spells->dwSpellsFileSize	= 5334; // sizeof(GAMESPELLS)+13*406;
	Spells->dwSpellsInfoHeader	= 0x00000097;
	Spells->dwSpellsInfoVersion	= 0x0000001C;
	Spells->dwSpellsCount		= 13;
	Spells->dwGlobalDateHeader	= 0x00000001;
	Compiler_SetDate(&Spells->dateSpellsDate);
	Spells->dwSpellsDefinesHeader	= 0x00000098;
	Spells->dwSpellsDefinesSize	= 13*406;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Spells,sizeof(GAMESPELLS),&Written,NULL);
	if (Written != sizeof(GAMESPELLS)) goto Error_3;

	for (i = 0; GSOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Spell,GSOrder[i+1],406);
		if (GSOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GSOrder[i];

			*((LONG *)(Spell+0x11C)) = (LONG)MapGlobalVariables.Spells[Offset].ManaCost;
			*((WORD *)(Spell+0x0E2)) = (WORD)MapGlobalVariables.Spells[Offset].ManaDrain;
			*((LONG *)(Spell+0x0CC)) = (LONG)MapGlobalVariables.Spells[Offset].RechargeTime;
			*((BYTE *)(Spell+0x194)) = (BYTE)MapGlobalVariables.Spells[Offset].HandAnim;
			*((BYTE *)(Spell+0x195)) = (BYTE)MapGlobalVariables.Spells[Offset].NoGoHandAnim;
			*((BYTE *)(Spell+0x0EF)) = (BYTE)MapGlobalVariables.Spells[Offset].CastRule;
			*((BYTE *)(Spell+0x0DA)) = (BYTE)MapGlobalVariables.Spells[Offset].TargetRule;
			*((BYTE *)(Spell+0x0F0)) = (BYTE)MapGlobalVariables.Spells[Offset].ShotType;
			*((WORD *)(Spell+0x0D0)) = (WORD)MapGlobalVariables.Spells[Offset].ShotData1;
			*((WORD *)(Spell+0x0D4)) = (WORD)MapGlobalVariables.Spells[Offset].ShotData2;
			*((BYTE *)(Spell+0x113)) = (BYTE)MapGlobalVariables.Spells[Offset].UpgShotType;
			*((WORD *)(Spell+0x114)) = (WORD)MapGlobalVariables.Spells[Offset].UpgShotData1;
			*((WORD *)(Spell+0x118)) = (WORD)MapGlobalVariables.Spells[Offset].UpgShotData2;
			*((WORD *)(Spell+0x111)) = (WORD)MapGlobalVariables.Spells[Offset].UpgRechargeTime;

			if (GShots[MapGlobalVariables.Spells[Offset].ShotType].Variable == 0x01)
				*((WORD *)(Spell+0x0D4)) = 0;
			if (GShots[MapGlobalVariables.Spells[Offset].UpgShotType].Variable == 0x01)
				*((WORD *)(Spell+0x118)) = 0;
			if (GShots[MapGlobalVariables.Spells[Offset].ShotType].Variable == 0xFF)
				*((WORD *)(Spell+0x0D4)) = 0;
			if (GShots[MapGlobalVariables.Spells[Offset].UpgShotType].Variable == 0xFF)
				*((WORD *)(Spell+0x118)) = 0;

			*((BYTE *)(Spell+0x0DC)) &= ~(0x04|0x08);
			if (MapGlobalVariables.Spells[Offset].Attacking) *((BYTE *)(Spell+0x0DC)) |= (BYTE)0x04;
			if (MapGlobalVariables.Spells[Offset].Defensive) *((BYTE *)(Spell+0x0DC)) |= (BYTE)0x08;

			}
		WriteFile(FileHandle,Spell,406,&Written,NULL);
		if (Written != 406) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Spell);
	GlobalFree(Spells);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Spell);
Error_2:GlobalFree(Spells);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}Creatures.kld				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateCreaturesFile(char *FileName)
{
	GAMECREATURES	*Creatures;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*Creature;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameCreaturesExt);

	//--- Ecriture du contenu du fichier ---

	Creatures = GlobalAlloc(GPTR,sizeof(GAMECREATURES));
	if (!Creatures)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Creature = GlobalAlloc(GPTR,5449);
	if (!Creature)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	Creatures->dwCreaturesFileHeader	= 0x000000AA;
	Creatures->dwCreaturesFileVersion	= 0x00000004;
	Creatures->dwCreaturesFileSize		= 250710; // sizeof(GAMECREATURES)+46*5449;
	Creatures->dwCreaturesInfoHeader	= 0x000000AB;
	Creatures->dwCreaturesInfoVersion	= 0x0000001C;
	Creatures->dwCreaturesCount		= 46;
	Creatures->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&Creatures->dateCreaturesDate);
	Creatures->dwCreaturesDefinesHeader	= 0x000000AC;
	Creatures->dwCreaturesDefinesSize	= 46*5449;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,Creatures,sizeof(GAMECREATURES),&Written,NULL);
	if (Written != sizeof(GAMECREATURES)) goto Error_3;

	for (i = 0; GCOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(Creature,GCOrder[i+1],5449);
		if (GCOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GCOrder[i];
			*((WORD *)(Creature+0x0F00)) = (WORD)MapGlobalVariables.Creatures[Offset].ExpPtsForNextLevel;
			*((WORD *)(Creature+0x0F04)) = (WORD)MapGlobalVariables.Creatures[Offset].ExpPts;
			*((WORD *)(Creature+0x0F06)) = (WORD)MapGlobalVariables.Creatures[Offset].ExpPtsTraining;
			*((WORD *)(Creature+0x0F08)) = (WORD)MapGlobalVariables.Creatures[Offset].ResearchPts;
			*((WORD *)(Creature+0x0F0A)) = (WORD)MapGlobalVariables.Creatures[Offset].ManufacturePts;
			*((WORD *)(Creature+0x0EA4)) = (WORD)MapGlobalVariables.Creatures[Offset].Height;
			*((WORD *)(Creature+0x0EB0)) = (WORD)MapGlobalVariables.Creatures[Offset].EyeHeight;
			*((WORD *)(Creature+0x0EC8)) = (WORD)MapGlobalVariables.Creatures[Offset].DistanceCanSee;
			*((WORD *)(Creature+0x0ECC)) = (WORD)MapGlobalVariables.Creatures[Offset].DistanceCanHear;
			*((WORD *)(Creature+0x0F0C)) = (WORD)MapGlobalVariables.Creatures[Offset].Health;
			*((WORD *)(Creature+0x0F0E)) = (WORD)MapGlobalVariables.Creatures[Offset].HealthFromChicken;
			*((WORD *)(Creature+0x0F16)) = (WORD)MapGlobalVariables.Creatures[Offset].SlapDamage;
			*((WORD *)(Creature+0x0F10)) = (WORD)MapGlobalVariables.Creatures[Offset].Fear;
			*((WORD *)(Creature+0x0F12)) = (WORD)MapGlobalVariables.Creatures[Offset].Threat;
			*((WORD *)(Creature+0x0F18)) = (WORD)MapGlobalVariables.Creatures[Offset].PrayerMana;
			*((WORD *)(Creature+0x0F1C)) = (WORD)MapGlobalVariables.Creatures[Offset].Pay;
			*((WORD *)(Creature+0x0F34)) = (WORD)MapGlobalVariables.Creatures[Offset].InitialGoldHeld;
			*((WORD *)(Creature+0x0F1E)) = (WORD)MapGlobalVariables.Creatures[Offset].MaxGoldHeld;
			*((WORD *)(Creature+0x0EB4)) = (WORD)MapGlobalVariables.Creatures[Offset].Speed;
			*((WORD *)(Creature+0x0EB8)) = (WORD)MapGlobalVariables.Creatures[Offset].RunSpeed;
			*((WORD *)(Creature+0x0F28)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerNoLair;
			*((WORD *)(Creature+0x0F2A)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerNoFood;
			*((WORD *)(Creature+0x0F2C)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerNoPay;
			*((WORD *)(Creature+0x0F2E)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerNoWork;
			*((WORD *)(Creature+0x0F30)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerSlap;
			*((WORD *)(Creature+0x0F32)) = (WORD)MapGlobalVariables.Creatures[Offset].AngerInHand;
			*((WORD *)(Creature+0x0EC0)) = (WORD)MapGlobalVariables.Creatures[Offset].TimeAwake;
			*((WORD *)(Creature+0x0EC4)) = (WORD)MapGlobalVariables.Creatures[Offset].SleepDuration;
			*((WORD *)(Creature+0x0ED0)) = (WORD)MapGlobalVariables.Creatures[Offset].StunDuration;
			*((WORD *)(Creature+0x0ED4)) = (WORD)MapGlobalVariables.Creatures[Offset].GuardDuration;
			*((WORD *)(Creature+0x0ED8)) = (WORD)MapGlobalVariables.Creatures[Offset].IdleDuration;
			*((WORD *)(Creature+0x0EDC)) = (WORD)MapGlobalVariables.Creatures[Offset].SlapDuration;
			*((WORD *)(Creature+0x0EE8)) = (WORD)MapGlobalVariables.Creatures[Offset].PossessionManaCost;
			*((WORD *)(Creature+0x0EEA)) = (WORD)MapGlobalVariables.Creatures[Offset].OwnLandHealthInc;
			*((WORD *)(Creature+0x107F)) = (WORD)MapGlobalVariables.Creatures[Offset].TortureHealthCng;
			*((WORD *)(Creature+0x1081)) = (WORD)MapGlobalVariables.Creatures[Offset].TortureMoodCng;
			*((LONG *)(Creature+0x0EBC)) = (LONG)MapGlobalVariables.Creatures[Offset].HungerRate;
			*((WORD *)(Creature+0x0F22)) = (WORD)MapGlobalVariables.Creatures[Offset].DecomposeValue;

			*((BYTE *)(Creature+0x0EFC)) &= (BYTE)~(0x02|0x04|0x10|0x20);
			*((BYTE *)(Creature+0x0EFD)) &= (BYTE)~(0x02|0x80);
			*((BYTE *)(Creature+0x0EFE)) &= (BYTE)~(0x01|0x02|0x04|0x20);
			*((BYTE *)(Creature+0x0EFF)) &= (BYTE)~(0x01|0x02|0x04|0x40);
			if (MapGlobalVariables.Creatures[Offset].CanBeHypnotized) *((BYTE *)(Creature+0x0EFE)) |= (BYTE)0x01;
			if (MapGlobalVariables.Creatures[Offset].DoNotTrain)	  *((BYTE *)(Creature+0x0EFE)) |= (BYTE)0x20;
			if (MapGlobalVariables.Creatures[Offset].CanBePicked)	  *((BYTE *)(Creature+0x0EFC)) |= (BYTE)0x02;
			if (MapGlobalVariables.Creatures[Offset].CanBeSlapped)	  *((BYTE *)(Creature+0x0EFC)) |= (BYTE)0x04;
			if (MapGlobalVariables.Creatures[Offset].CanWalkWater)	  *((BYTE *)(Creature+0x0EFC)) |= (BYTE)0x10;
			if (MapGlobalVariables.Creatures[Offset].CanWalkLava)	  *((BYTE *)(Creature+0x0EFC)) |= (BYTE)0x20;
			if (MapGlobalVariables.Creatures[Offset].ImmuneTurncoat)  *((BYTE *)(Creature+0x0EFD)) |= (BYTE)0x02;
			if (MapGlobalVariables.Creatures[Offset].DeadBody)	  *((BYTE *)(Creature+0x0EFD)) |= (BYTE)0x80;
			if (MapGlobalVariables.Creatures[Offset].ImmuneChicken)   *((BYTE *)(Creature+0x0EFE)) |= (BYTE)0x02;
			if (MapGlobalVariables.Creatures[Offset].Fearless)	  *((BYTE *)(Creature+0x0EFE)) |= (BYTE)0x04;
			if (MapGlobalVariables.Creatures[Offset].NoEnemies)	  *((BYTE *)(Creature+0x0EFF)) |= (BYTE)0x01;
			if (MapGlobalVariables.Creatures[Offset].FreeFriends)	  *((BYTE *)(Creature+0x0EFF)) |= (BYTE)0x02;
			if (MapGlobalVariables.Creatures[Offset].RevealTraps)	  *((BYTE *)(Creature+0x0EFF)) |= (BYTE)0x04;
			if (MapGlobalVariables.Creatures[Offset].RandomInvader)   *((BYTE *)(Creature+0x0EFF)) |= (BYTE)0x40;

			*((BYTE *)(Creature+0x0F76)) = (BYTE)MapGlobalVariables.Creatures[Offset].ArmourType;
			*((BYTE *)(Creature+0x0F03)) = (BYTE)MapGlobalVariables.Creatures[Offset].FightStyle;
			*((BYTE *)(Creature+0x0F02)) = (BYTE)MapGlobalVariables.Creatures[Offset].JobClass;
			*((BYTE *)(Creature+0x0F52)) = (BYTE)MapGlobalVariables.Creatures[Offset].LairObject;
			*((WORD *)(Creature+0x0F3E)) = (WORD)MapGlobalVariables.Creatures[Offset].SlapEffect;
			*((WORD *)(Creature+0x0CFD)) = (WORD)MapGlobalVariables.Creatures[Offset].HealthEffect;
			*((WORD *)(Creature+0x0F36)) = (WORD)MapGlobalVariables.Creatures[Offset].EntranceEffect;
			*((WORD *)(Creature+0x0F40)) = (WORD)MapGlobalVariables.Creatures[Offset].DeathEffect;
			*((WORD *)(Creature+0x0DAC)) = (WORD)MapGlobalVariables.Creatures[Offset].Attract1Type;
			*((WORD *)(Creature+0x0DB0)) = (WORD)MapGlobalVariables.Creatures[Offset].Attract1ID;
			*((WORD *)(Creature+0x0DB4)) = (WORD)MapGlobalVariables.Creatures[Offset].Attract2Type;
			*((WORD *)(Creature+0x0DB8)) = (WORD)MapGlobalVariables.Creatures[Offset].Attract2ID;
			*((WORD *)(Creature+0x0DBA)) = (WORD)MapGlobalVariables.Creatures[Offset].Attract2Data;

			*((BYTE *)(Creature+0x0DDA)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot1ID;
			*((LONG *)(Creature+0x0DD4)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot1Delay;
			*((BYTE *)(Creature+0x0DDB)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot1Level;
			*((LONG *)(Creature+0x0DC4)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot1X;
			*((LONG *)(Creature+0x0DC8)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot1Y;
			*((LONG *)(Creature+0x0DCC)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot1Z;
			*((BYTE *)(Creature+0x0DF2)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot2ID;
			*((LONG *)(Creature+0x0DEC)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot2Delay;
			*((BYTE *)(Creature+0x0DF3)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot2Level;
			*((LONG *)(Creature+0x0DDC)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot2X;
			*((LONG *)(Creature+0x0DE0)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot2Y;
			*((LONG *)(Creature+0x0DE4)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot2Z;
			*((BYTE *)(Creature+0x0E0A)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot3ID;
			*((LONG *)(Creature+0x0E04)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot3Delay;
			*((BYTE *)(Creature+0x0E0B)) = (BYTE)MapGlobalVariables.Creatures[Offset].Shot3Level;
			*((LONG *)(Creature+0x0DF4)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot3X;
			*((LONG *)(Creature+0x0DF8)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot3Y;
			*((LONG *)(Creature+0x0DFC)) = (LONG)MapGlobalVariables.Creatures[Offset].Shot3Z;
			*((BYTE *)(Creature+0x0F50)) = (BYTE)MapGlobalVariables.Creatures[Offset].MeleeID;
			*((LONG *)(Creature+0x0EF8)) = (LONG)MapGlobalVariables.Creatures[Offset].RechargeTime;
			*((LONG *)(Creature+0x0EEC)) = (LONG)MapGlobalVariables.Creatures[Offset].Range;
			*((WORD *)(Creature+0x0F14)) = (WORD)MapGlobalVariables.Creatures[Offset].Dammage;
			*((LONG *)(Creature+0x0CF9)) = (LONG)MapGlobalVariables.Creatures[Offset].Delay;

			*((BYTE *)(Creature+0x0E0C)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res1;
			*((BYTE *)(Creature+0x0E0D)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res1Rate;
			*((BYTE *)(Creature+0x0E0E)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res2;
			*((BYTE *)(Creature+0x0E0F)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res2Rate;
			*((BYTE *)(Creature+0x0E10)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res3;
			*((BYTE *)(Creature+0x0E11)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res3Rate;
			*((BYTE *)(Creature+0x0E12)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res4;
			*((BYTE *)(Creature+0x0E13)) = (BYTE)MapGlobalVariables.Creatures[Offset].Res4Rate;

			*((BYTE *)(Creature+0x0F46)) = (BYTE)MapGlobalVariables.Creatures[Offset].SpecialAb1;
			*((WORD *)(Creature+0x14E9)) = (WORD)MapGlobalVariables.Creatures[Offset].SpecialAb1Data;
			*((BYTE *)(Creature+0x0F47)) = (BYTE)MapGlobalVariables.Creatures[Offset].SpecialAb2;
			*((WORD *)(Creature+0x14ED)) = (WORD)MapGlobalVariables.Creatures[Offset].SpecialAb2Data;
			*((BYTE *)(Creature+0x0F42)) = (BYTE)MapGlobalVariables.Creatures[Offset].Melee1;
			*((BYTE *)(Creature+0x0F43)) = (BYTE)MapGlobalVariables.Creatures[Offset].Melee2;
			*((BYTE *)(Creature+0x1025)) = (BYTE)MapGlobalVariables.Creatures[Offset].Special1;
			*((BYTE *)(Creature+0x1026)) = (BYTE)MapGlobalVariables.Creatures[Offset].Special2;
			*((BYTE *)(Creature+0x0F45)) = (BYTE)MapGlobalVariables.Creatures[Offset].Spell;
			*((BYTE *)(Creature+0x0D3C)) = (BYTE)MapGlobalVariables.Creatures[Offset].GammaEffect;
			*((WORD *)(Creature+0x0DBC)) = (WORD)MapGlobalVariables.Creatures[Offset].WaddleScale;
			*((WORD *)(Creature+0x0DC0)) = (WORD)MapGlobalVariables.Creatures[Offset].OscillateScale;
			*((BYTE *)(Creature+0x0D3D)) = (BYTE)MapGlobalVariables.Creatures[Offset].WalkScale;
			*((BYTE *)(Creature+0x0EFF)) &= (BYTE)~(0x10);
			if (MapGlobalVariables.Creatures[Offset].CameraRolls) *((BYTE *)(Creature+0x0EFF)) |= (BYTE)0x10;

			*((LONG *)(Creature+0x0E14)) = (LONG)MapGlobalVariables.Creatures[Offset].HJobs[0];
			*((WORD *)(Creature+0x0E18)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMood[0];
			*((WORD *)(Creature+0x0E1A)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMana[0];
			*((BYTE *)(Creature+0x0E1C)) = (BYTE)MapGlobalVariables.Creatures[Offset].HJobsChance[0];
			*((LONG *)(Creature+0x0E20)) = (LONG)MapGlobalVariables.Creatures[Offset].HJobs[1];
			*((WORD *)(Creature+0x0E24)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMood[1];
			*((WORD *)(Creature+0x0E26)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMana[1];
			*((BYTE *)(Creature+0x0E28)) = (BYTE)MapGlobalVariables.Creatures[Offset].HJobsChance[1];
			*((LONG *)(Creature+0x0E2C)) = (LONG)MapGlobalVariables.Creatures[Offset].HJobs[2];
			*((WORD *)(Creature+0x0E30)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMood[2];
			*((WORD *)(Creature+0x0E32)) = (WORD)MapGlobalVariables.Creatures[Offset].HJobsMana[2];
			*((BYTE *)(Creature+0x0E34)) = (BYTE)MapGlobalVariables.Creatures[Offset].HJobsChance[2];
			*((LONG *)(Creature+0x0E38)) = (LONG)MapGlobalVariables.Creatures[Offset].UJobs[0];
			*((WORD *)(Creature+0x0E3C)) = (WORD)MapGlobalVariables.Creatures[Offset].UJobsMood[0];
			*((WORD *)(Creature+0x0E3E)) = (WORD)MapGlobalVariables.Creatures[Offset].UJobsMana[0];
			*((BYTE *)(Creature+0x0E40)) = (BYTE)MapGlobalVariables.Creatures[Offset].UJobsChance[0];
			*((LONG *)(Creature+0x0E44)) = (LONG)MapGlobalVariables.Creatures[Offset].UJobs[1];
			*((WORD *)(Creature+0x0E48)) = (WORD)MapGlobalVariables.Creatures[Offset].UJobsMood[1];
			*((WORD *)(Creature+0x0E4A)) = (WORD)MapGlobalVariables.Creatures[Offset].UJobsMana[1];
			*((BYTE *)(Creature+0x0E4C)) = (BYTE)MapGlobalVariables.Creatures[Offset].UJobsChance[1];
			*((LONG *)(Creature+0x0E50)) = (LONG)MapGlobalVariables.Creatures[Offset].AJobs[0];
			*((WORD *)(Creature+0x0E54)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMood[0];
			*((WORD *)(Creature+0x0E56)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMana[0];
			*((BYTE *)(Creature+0x0E58)) = (BYTE)MapGlobalVariables.Creatures[Offset].AJobsChance[0];
			*((LONG *)(Creature+0x0E5C)) = (LONG)MapGlobalVariables.Creatures[Offset].AJobs[1];
			*((WORD *)(Creature+0x0E60)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMood[1];
			*((WORD *)(Creature+0x0E62)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMana[1];
			*((BYTE *)(Creature+0x0E64)) = (BYTE)MapGlobalVariables.Creatures[Offset].AJobsChance[1];
			*((LONG *)(Creature+0x0E68)) = (LONG)MapGlobalVariables.Creatures[Offset].AJobs[2];
			*((WORD *)(Creature+0x0E6C)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMood[2];
			*((WORD *)(Creature+0x0E6E)) = (WORD)MapGlobalVariables.Creatures[Offset].AJobsMana[2];
			*((BYTE *)(Creature+0x0E70)) = (BYTE)MapGlobalVariables.Creatures[Offset].AJobsChance[2];
			*((LONG *)(Creature+0x0E74)) = (LONG)MapGlobalVariables.Creatures[Offset].NJobs[0];
			*((LONG *)(Creature+0x0E78)) = (LONG)MapGlobalVariables.Creatures[Offset].NJobs[1];
			*((LONG *)(Creature+0x0E7C)) = (LONG)MapGlobalVariables.Creatures[Offset].NJobs[2];
			*((LONG *)(Creature+0x0E80)) = (LONG)MapGlobalVariables.Creatures[Offset].NJobs[3];

			*((LONG *)(Creature+0x0D94)) = (LONG)MapGlobalVariables.Creatures[Offset].LightX;
			*((LONG *)(Creature+0x0D98)) = (LONG)MapGlobalVariables.Creatures[Offset].LightY;
			*((LONG *)(Creature+0x0D9C)) = (LONG)MapGlobalVariables.Creatures[Offset].LightZ;
			*((WORD *)(Creature+0x0DA0)) = (WORD)MapGlobalVariables.Creatures[Offset].LightRadius;
			*((BYTE *)(Creature+0x0DA8)) = (BYTE)MapGlobalVariables.Creatures[Offset].LightR;
			*((BYTE *)(Creature+0x0DA9)) = (BYTE)MapGlobalVariables.Creatures[Offset].LightG;
			*((BYTE *)(Creature+0x0DAA)) = (BYTE)MapGlobalVariables.Creatures[Offset].LightB;
			*((BYTE *)(Creature+0x0DA4)) &= (BYTE)~(0x02|0x04|0x80);
			if (MapGlobalVariables.Creatures[Offset].LightFlicker) *((BYTE *)(Creature+0x0DA4)) |= (BYTE)0x02;
			if (MapGlobalVariables.Creatures[Offset].LightPulse)   *((BYTE *)(Creature+0x0DA4)) |= (BYTE)0x04;
			if (MapGlobalVariables.Creatures[Offset].LightPlayer)  *((BYTE *)(Creature+0x0DA4)) |= (BYTE)0x80;
			}
		WriteFile(FileHandle,Creature,5449,&Written,NULL);
		if (Written != 5449) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(Creature);
	GlobalFree(Creatures);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(Creature);
Error_2:GlobalFree(Creatures);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ecriture du fichier {Carte}CreatureSpells.kld			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Compiler_GenerateCreatureSpellsFile(char *FileName)
{
	GAMECRSPELLS	*CrSpells;
	HANDLE		 FileHandle;
	DWORD		 Written;
	DWORD		 Offset;
	BYTE		*CrSpell;
	char		*WorkName;
	char		*Extension;
	int		 i;

	WorkName = GlobalAlloc(GPTR,MAX_PATH);
	if (!WorkName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_0;
		}

	strcpy(WorkName,FileName);
	Extension = strrchr(WorkName,'.');
	if (Extension) *Extension = 0;
	strcat(WorkName,szGameCreatureSpellsExt);

	//--- Ecriture du contenu du fichier ---

	CrSpells = GlobalAlloc(GPTR,sizeof(GAMECRSPELLS));
	if (!CrSpells)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	CrSpell = GlobalAlloc(GPTR,266);
	if (!CrSpell)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_2;
		}

	CrSpells->dwCrSpellsFileHeader		= 0x000000A0;
	CrSpells->dwCrSpellsFileVersion		= 0x00000004;
	CrSpells->dwCrSpellsFileSize		= 10962; // sizeof(GAMECRSPELLS)+41*266;
	CrSpells->dwCrSpellsInfoHeader		= 0x000000A1;
	CrSpells->dwCrSpellsInfoVersion		= 0x0000001C;
	CrSpells->dwCrSpellsCount		= 41;
	CrSpells->dwGlobalDateHeader		= 0x00000001;
	Compiler_SetDate(&CrSpells->dateCrSpellsDate);
	CrSpells->dwCrSpellsDefinesHeader	= 0x000000A2;
	CrSpells->dwCrSpellsDefinesSize		= 41*266;

	//--- Ecriture du fichier ---

	FileHandle = CreateFile(WorkName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_3;
		}
	WriteFile(FileHandle,CrSpells,sizeof(GAMECRSPELLS),&Written,NULL);
	if (Written != sizeof(GAMECRSPELLS)) goto Error_3;

	for (i = 0; GOOrder[i] != (DWORD *)-1; i += 2)
		{
		CopyMemory(CrSpell,GOOrder[i+1],266);
		if (GOOrder[i] != (DWORD *)-2)
			{
			Offset = (DWORD)GOOrder[i];

			*((WORD *)(CrSpell+0x00EA)) = (WORD)MapGlobalVariables.CrSpells[Offset].RechargeTime;
			*((WORD *)(CrSpell+0x00D0)) = (WORD)MapGlobalVariables.CrSpells[Offset].Range;
			*((BYTE *)(CrSpell+0x00E7)) = (BYTE)MapGlobalVariables.CrSpells[Offset].Shot;
			*((WORD *)(CrSpell+0x00C8)) = (WORD)MapGlobalVariables.CrSpells[Offset].ShotData1;
			*((WORD *)(CrSpell+0x00CC)) = (WORD)MapGlobalVariables.CrSpells[Offset].ShotData2;

			*((BYTE *)(CrSpell+0x00D4)) &= (BYTE)~(0x01|0x02|0x04|0x08|0x80);
			*((BYTE *)(CrSpell+0x00D5)) &= (BYTE)~(0x01);
			if (MapGlobalVariables.CrSpells[Offset].Attacking)	*((BYTE *)(CrSpell+0x00D4)) |= (BYTE)0x01;
			if (MapGlobalVariables.CrSpells[Offset].Creature)	*((BYTE *)(CrSpell+0x00D4)) |= (BYTE)0x02;
			if (MapGlobalVariables.CrSpells[Offset].Defensive)	*((BYTE *)(CrSpell+0x00D4)) |= (BYTE)0x04;
			if (MapGlobalVariables.CrSpells[Offset].CastSelf)	*((BYTE *)(CrSpell+0x00D4)) |= (BYTE)0x80;
			if (MapGlobalVariables.CrSpells[Offset].CastOthers)	*((BYTE *)(CrSpell+0x00D5)) |= (BYTE)0x01;
			if (MapGlobalVariables.CrSpells[Offset].CastCreatures)	*((BYTE *)(CrSpell+0x00D4)) |= (BYTE)0x08;

			}
		WriteFile(FileHandle,CrSpell,266,&Written,NULL);
		if (Written != 266) goto Error_4;
		}

	CloseHandle(FileHandle);
	GlobalFree(CrSpell);
	GlobalFree(CrSpells);
	GlobalFree(WorkName);
	return(1);

//--- Erreurs ---

Error_4:Compiler_SetLastError(COMPILERR_WINDOWS);
	CloseHandle(FileHandle);
	DeleteFile(WorkName);
Error_3:GlobalFree(CrSpell);
Error_2:GlobalFree(CrSpells);
Error_1:GlobalFree(WorkName);
Error_0:return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ecriture de la date dans un fichier DK2 «««««««««««««««««««««««««»

void Compiler_SetDate(GAMEDATE *Date)
{
	SYSTEMTIME	Time;

	GetLocalTime(&Time);
	Date->wGlobalCreationYear	= Time.wYear;
	Date->bGlobalCreationDate	= (BYTE)Time.wDay;
	Date->bGlobalCreationMonth	= (BYTE)Time.wMonth;
	Date->bGlobalCreationHour	= (BYTE)Time.wHour;
	Date->bGlobalCreationMinute	= (BYTE)Time.wMinute;
	Date->bGlobalCreationSeconds	= (BYTE)Time.wSecond;
	Date->wGlobalModifiedYear	= Time.wYear;
	Date->bGlobalModifiedDate	= (BYTE)Time.wDay;
	Date->bGlobalModifiedMonth	= (BYTE)Time.wMonth;
	Date->bGlobalModifiedHour	= (BYTE)Time.wHour;
	Date->bGlobalModifiedMinute	= (BYTE)Time.wMinute;
	Date->bGlobalModifiedSeconds	= (BYTE)Time.wSecond;
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
