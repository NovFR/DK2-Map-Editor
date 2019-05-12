
/*** Drapeaux ***********************************************************/

#define GAME_AREAFLAGHEROLAIR	0x00000001
//#define GAME_AREAFLAGTRIGGER	0x00000004
//#define GAME_AREAFLAGPORTAL	0x00000008
#define GAME_AREAFLAGVISIBLE	0x00000010
#define GAME_AREAFLAGTOOLBOX	0x00000020
#define GAME_AREAFLAGIGNSOLID	0x00000040

/*** Structures du jeu **************************************************/

typedef struct GAMEMAP {
	DWORD		 dwMapFileHeader;
	DWORD		 dwMapFileVersion;
	DWORD		 dwMapFileSize;
	DWORD		 dwMapInfoHeader;
	DWORD		 dwMapInfoVersion;
	DWORD		 dwMapWidth;
	DWORD		 dwMapHeight;
	DWORD		 dwMapDefineHeader;
	DWORD		 dwMapDefineSize;
} GAMEMAP;

typedef struct GAMEDATE {
	WORD		 wGlobalCreationYear;
	BYTE		 bGlobalCreationDate;
	BYTE		 bGlobalCreationMonth;
	BYTE		 bGlobalCreationPad0;
	BYTE		 bGlobalCreationPad1;
	BYTE		 bGlobalCreationHour;
	BYTE		 bGlobalCreationMinute;
	BYTE		 bGlobalCreationSeconds;
	BYTE		 bGlobalCreationPad2;
	WORD		 wGlobalModifiedYear;
	BYTE		 bGlobalModifiedDate;
	BYTE		 bGlobalModifiedMonth;
	BYTE		 bGlobalModifiedPad0;
	BYTE		 bGlobalModifiedPad1;
	BYTE		 bGlobalModifiedHour;
	BYTE		 bGlobalModifiedMinute;
	BYTE		 bGlobalModifiedSeconds;
	BYTE		 bGlobalModifiedPad2;
} GAMEDATE;

typedef struct GAMETHINGS {
	DWORD		 dwThingsFileHeader;
	DWORD		 dwThingsFileVersion;
	DWORD		 dwThingsFileSize;
	DWORD		 dwThingsInfoHeader;
	DWORD		 dwGlobalInfoHeader;
	DWORD		 dwThingsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateThingsDate;
	DWORD		 dwThingsDefinesHeader;
	DWORD		 dwThingsDefinesSize;
} GAMETHINGS;

typedef struct GAMEOBJECT {
	DWORD		 dwObjectHeader;
	DWORD		 dwObjectVersion;
	DWORD		 dwObjectLocationX;
	DWORD		 dwObjectLocationY;
	DWORD		 dwObjectUnknown1;
	DWORD		 dwObjectSpell;
	DWORD		 dwObjectGoldValue;
	WORD		 wObjectTrigger;
	BYTE		 bObjectID;
	BYTE		 bGlobalPlayer;
} GAMEOBJECT;

typedef struct GAMETRAP {
	DWORD		 dwTrapHeader;
	DWORD		 dwTrapVersion;
	DWORD		 dwTrapLocationX;
	DWORD		 dwTrapLocationY;
	DWORD		 dwTrapUnknown0;
	BYTE		 bTrapShots;
	BYTE		 bTrapID;
	BYTE		 bGlobalPlayer;
	BYTE		 bTrapUnknown2;
} GAMETRAP;

typedef struct GAMEDOOR {
	DWORD		 dwDoorHeader;
	DWORD		 dwDoorVersion;
	DWORD		 dwDoorLocationX;
	DWORD		 dwDoorLocationY;
	DWORD		 dwDoorUnknown0;
	WORD		 wDoorTrigger;
	BYTE		 bDoorID;
	BYTE		 bGlobalPlayer;
	BYTE		 bDoorLocked;
	BYTE		 bDoorUnknown2;
	BYTE		 bDoorUnknown3;
	BYTE		 bDoorUnknown4;
} GAMEDOOR;

typedef struct GAMENEUTRAL {
	DWORD		 dwNeutralHeader;
	DWORD		 dwNeutralVersion;
	DWORD		 dwNeutralLocationX;
	DWORD		 dwNeutralLocationY;
	DWORD		 dwNeutralUnknown0;
	BYTE		 bGlobalGoldHeld;
	BYTE		 bNeutralUnknown1;
	BYTE		 bGlobalLevel;
	BYTE		 bGlobalBehavior;
	DWORD		 dwGlobalHealth;
	WORD		 wNeutralTrigger;
	BYTE		 bGlobalCreatureID;
	BYTE		 bNeutralUnknown5;
} GAMENEUTRAL;

typedef struct GAMEHERO {
	DWORD		 dwHeroHeader;
	DWORD		 dwHeroVersion;
	DWORD		 dwHeroLocationX;
	DWORD		 dwHeroLocationY;
	DWORD		 dwHeroUnknown0;
	BYTE		 bGlobalGoldHeld;
	BYTE		 bHeroUnknown1;
	BYTE		 bGlobalLevel;
	BYTE		 bGlobalBehavior;
	BYTE		 bHeroTargetArea;
	BYTE		 bHeroUnknown2;
	BYTE		 bHeroUnknown3;
	BYTE		 bHeroUnknown4;
	DWORD		 dwGlobalHealth;
	WORD		 wHeroTrigger;
	BYTE		 bGlobalPlayer;
	BYTE		 bHeroObjective;
	BYTE		 bGlobalCreatureID;
	BYTE		 bHeroUnknown9;
	BYTE		 bHeroBandID;
	BYTE		 bHeroOptions;
} GAMEHERO;

typedef struct GAMEKEEPER {
	DWORD		 dwKeeperHeader;
	DWORD		 dwKeeperVersion;
	DWORD		 dwKeeperLocationX;
	DWORD		 dwKeeperLocationY;
	DWORD		 dwKeeperUnknown0;
	BYTE		 bGlobalGoldHeld;
	BYTE		 bKeeperUnknown1;
	BYTE		 bGlobalLevel;
	BYTE		 bGlobalBehavior;
	DWORD		 dwGlobalHealth;
	DWORD		 dwKeeperUnknown4;
	WORD		 wKeeperTrigger;
	BYTE		 bGlobalCreatureID;
	BYTE		 bGlobalPlayer;
} GAMEKEEPER;

typedef struct GAMEAREA {
	DWORD		 dwAreaHeader;
	DWORD		 dwAreaVersion;
	DWORD		 dwAreaLocationStartX;
	DWORD		 dwAreaLocationStartY;
	DWORD		 dwAreaLocationEndX;
	DWORD		 dwAreaLocationEndY;
	WORD		 wAreaWaitDelay;
	BYTE		 bAreaFlags;
	BYTE		 bAreaUnknown1;
	WORD		 wAreaTrigger;
	BYTE		 bAreaID;
	BYTE		 bAreaNextID;
	DWORD		 dwAreaUnknown5;
	DWORD		 dwAreaUnknown6;
	DWORD		 dwAreaUnknown7;
	DWORD		 dwAreaUnknown8;
	DWORD		 dwAreaUnknown9;
	DWORD		 dwAreaUnknown10;
	WORD		 wAreaUnknown11;
	WORD		 wAreaUnknown12;
	DWORD		 dwAreaUnknown13;
} GAMEAREA;

typedef struct GAMEGATE {
	DWORD		 dwGateHeader;
	DWORD		 dwGateVersion;
	DWORD		 dwGateLocationX;
	DWORD		 dwGateLocationY;
	DWORD		 dwGateUnknown0;
	WORD		 wGateUnknown1;
	BYTE		 bGateOrientation;
	BYTE		 bGateID;
	WORD		 wGateHealth;
	BYTE		 bGateType;
	BYTE		 bGlobalPlayer;
} GAMEGATE;

typedef struct GAMEEFFECT {
	DWORD		 dwEffectHeader;
	DWORD		 dwEffectVersion;
	DWORD		 dwEffectStartX;
	DWORD		 dwEffectStartY;
	DWORD		 dwEffectUnused0;
	DWORD		 dwEffectUnused1;
	WORD		 wEffectWidth;
	WORD		 wEffectHeight;
	WORD		 wEffect1;
	WORD		 wEffect2;
	WORD		 wEffect3;
	WORD		 wEffect4;
	BYTE		 bEffectFrequency;
	BYTE		 bEffectID;
	BYTE		 bUnused2[6];
} GAMEEFFECT;

typedef struct GAMEBAND {
	DWORD		 dwBandHeader;
	DWORD		 dwBandSize;
	DWORD		 dwBandVersion;
	DWORD		 dwBandUnknown0;
	DWORD		 dwBandUnknown1;
	WORD		 wBandUnknown2;
	WORD		 wBandUnknown3;		// WORD coz' of data alignment
	WORD		 wBandUnknown4;
	WORD		 wBandUnknown5;
	WORD		 wBandUnknown6;
	WORD		 wBandUnknown7;
	WORD		 wBandUnknown8;
	WORD		 wBandUnknown9;
	WORD		 wBandUnknown10;
	WORD		 wBandUnknown11;
	WORD		 wBandTrigger;
	BYTE		 bBandID;
	BYTE		 bBandUnknown13;
	BYTE		 bBandUnknown14;
	BYTE		 bBandUnknown15;
	BYTE		 bBandUnknown16;
	BYTE		 bBandUnknown17;
	BYTE		 bBandUnknown18;
	BYTE		 bBandUnknown19;
	BYTE		 bBandUnknown20;
	BYTE		 bBandUnknown21;
} GAMEBAND;

typedef struct GAMEBANDHERO {
	DWORD		 dwHeroLocationX;
	DWORD		 dwHeroLocationY;
	DWORD		 dwHeroUnknown0;
	BYTE		 bGlobalGoldHeld;
	BYTE		 bHeroUnknown1;
	BYTE		 bGlobalLevel;
	BYTE		 bGlobalBehavior;
	BYTE		 bHeroTargetArea;
	BYTE		 bHeroUnknown2;
	BYTE		 bHeroUnknown3;
	BYTE		 bHeroUnknown4;
	DWORD		 dwGlobalHealth;
	WORD		 wHeroTrigger;
	BYTE		 bGlobalPlayer;
	BYTE		 bHeroObjective;
	BYTE		 bGlobalCreatureID;
	BYTE		 bHeroUnknown7;
	BYTE		 bHeroBandID;
	BYTE		 bHeroOptions;
} GAMEBANDHERO;

typedef struct GAMEVARIABLES {
	DWORD		 dwVariablesFileHeader;
	DWORD		 dwVariablesFileVersion;
	DWORD		 dwVariablesFileSize;
	DWORD		 dwVariablesInfoHeader;
	DWORD		 dwGlobalInfoHeader;
	DWORD		 dwVariablesCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateVariablesDate;
	DWORD		 dwVariablesDefinesHeader;
	DWORD		 dwVariablesDefinesSize;
} GAMEVARIABLES;

typedef struct GAMEVARCREATURECOUNT {
	DWORD		 dwCreatureHeader;
	DWORD		 dwGlobalCreatureID;
	DWORD		 dwCreatureCount;
	DWORD		 dwGlobalPlayer;
} GAMEVARCREATURECOUNT;

typedef struct GAMEVARTHINGSTATUS {
	DWORD		 dwThingHeader;
	WORD		 wThingType;
	WORD		 wGlobalPlayer;
	DWORD		 dwThingID;
	DWORD		 dwThingState;
} GAMEVARTHINGSTATUS;

typedef struct GAMEVARALLIANCE {
	DWORD		 dwAllieHeader;
	DWORD		 dwGlobalPlayer;
	DWORD		 dwAllieWith;
	DWORD		 dwUnused;
} GAMEVARALLIANCE;

typedef struct GAMEVARCREATURE {
	DWORD		 dwCreatureParamHeader;
	DWORD		 dwGlobalCreatureID;
	DWORD		 dwCreatureParamType;
	DWORD		 dwCreatureNewValue;
} GAMEVARCREATURE;

typedef struct GAMEGLOBALVARIABLE {
	DWORD		 dwGlobalHeader;
	DWORD		 dwGlobalData;
	DWORD		 dwGlobalUnused0;
	DWORD		 dwGlobalUnused1;
} GAMEGLOBALVARIABLE;

typedef struct GAMEINFO {
	DWORD		 dwMainFileHeader;
	DWORD		 dwMainFileVersion;
	DWORD		 dwMainFileSize;
	DWORD		 dwMainInfoHeader;
	DWORD		 dwGlobalInfoHeader;
	DWORD		 dwMainUnknown0;
	DWORD		 dwMainUnknown1;
	GAMEDATE	 dateMainDate;
	DWORD		 dwMainParamsHeader;
	DWORD		 dwMainParamsSize;
	wchar_t		 wstrMainName[64];
	wchar_t		 wstrMainDescription[1024];
	wchar_t		 wstrMainAuthor[64];
	wchar_t		 wstrMainEmail[64];
	wchar_t		 wstrMainInformation[1024];
	WORD		 wMainTrigger;
	BYTE		 bMainGameTicks;
	BYTE		 bMainUnused2[21001];
	BYTE		 bMainOptions;
	BYTE		 bMainOptions2;
	char		 szMainSpeech[32];
	BYTE		 bMainHornyTalismanPieces;
	BYTE		 bMainRewardPrev1;
	BYTE		 bMainRewardPrev2;
	BYTE		 bMainRewardPrev3;
	BYTE		 bMainUnused3;
	BYTE		 bMainRewardNext1;
	BYTE		 bMainRewardNext2;
	BYTE		 bMainRewardNext3;
	BYTE		 bMainUnused4;
	BYTE		 bMainSoundtrack;
	BYTE		 bMainTextTableID;
	BYTE		 bMainTitleTextID[2];
	BYTE		 bMainTitlePlotTextID[2];
	BYTE		 bMainDebriefingTextID[2];
	BYTE		 bMainObjectiveTextID[2];
	BYTE		 bMainUnused5[2];
	BYTE		 bMainSubObjective3TextID[2];
	BYTE		 bMainSubObjective1TextID[2];
	BYTE		 bMainSubObjective2TextID[2];
	BYTE		 bMainSpecialLevelNumber;
	BYTE		 bMainUnused6;
	BYTE		 bMainIntroCreatureID0;
	BYTE		 bMainIntroCreatureID1;
	BYTE		 bMainIntroCreatureID2;
	BYTE		 bMainIntroCreatureID3;
	BYTE		 bMainIntroCreatureID4;
	BYTE		 bMainIntroCreatureID5;
	BYTE		 bMainIntroCreatureID6;
	BYTE		 bMainIntroCreatureID7;
	BYTE		 bMainIntroTextID0[2];
	BYTE		 bMainIntroTextID1[2];
	BYTE		 bMainIntroTextID2[2];
	BYTE		 bMainIntroTextID3[2];
	BYTE		 bMainIntroTextID4[2];
	BYTE		 bMainIntroTextID5[2];
	BYTE		 bMainIntroTextID6[2];
	BYTE		 bMainIntroTextID7[2];
	BYTE		 bMainUnused7[32];
	BYTE		 bMainLevelNumber;
	BYTE		 bMainPlayers;
	BYTE		 bMainUnused8[4];
	BYTE		 bMainPreLevelSpeechID[2];
	BYTE		 bMainWinSpeechID[2];
	BYTE		 bMainLooseSpeechID[2];
	BYTE		 bMainNewsSpeechID[2];
	BYTE		 bMainGenPreLevelSpeechID[2];
	BYTE		 bMainUnused9[80];
	char		 szFileTerrain[72];
	char		 szFileObjects[72];
	char		 szFileRooms[72];
	char		 szFileCreatures[72];
	char		 szFileCreatureSpells[72];
	char		 szFileTraps[72];
	char		 szFileDoors[72];
	char		 szFileShots[72];
	char		 szFileKeeperSpells[72];
	char		 szFileUserPlayers[72];
	char		 szFileUserMap[72];
	char		 szFileUserTriggers[72];
	char		 szFileGlobalVariables[72];
	char		 szFileUserVariables[72];
	char		 szFileUserThings[64];
	BYTE		 bMainUnknown3[28*4];
} GAMEINFO;

typedef struct GAMEPLAYERS {
	DWORD		 dwPlayersFileHeader;
	DWORD		 dwPlayersFileVersion;
	DWORD		 dwPlayersFileSize;
	DWORD		 dwPlayersInfoHeader;
	DWORD		 dwGlobalInfoHeader;
	DWORD		 dwPlayersCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 datePlayersDate;
	DWORD		 dwPlayersDefinesHeader;
	DWORD		 dwPlayersDefinesSize;
} GAMEPLAYERS;

typedef struct GAMEPLAYER {
	BYTE		 bPlayerGold[4];
	BYTE		 bPlayerCPU[4];
	BYTE		 bPlayerAI;
	BYTE		 bPlayerMiscSpeed;
	BYTE		 bPlayerMiscOpenness;
	BYTE		 bPlayerBattleCallToHarmsRemoveNoCreature;
	BYTE		 bPlayerRebuildRoom;
	BYTE		 bPlayerUnused0;
	BYTE		 bPlayerUnused1;
	BYTE		 bPlayerUnused2;
	BYTE		 bPlayerCreateEmptyAreasWhenIdle[4];
	BYTE		 bPlayerBuildBiggerLair[4];
	BYTE		 bPlayerSellCapturedRoomIfLowOnGold[4];
	BYTE		 bPlayerResearchedRoom;
	BYTE		 bPlayerDefaultTiles;
	BYTE		 bPlayerTilesLeftBetweenRooms;
	BYTE		 bPlayerDistance;
	BYTE		 bPlayerCorridorStyle;
	BYTE		 bPlayerExpandMode;
	BYTE		 bPlayerTilesFromHeart;
	BYTE		 bPlayerRoomToBuildPri1;
	BYTE		 bPlayerRoomToBuildPri2;
	BYTE		 bPlayerRoomToBuildPri3;
	BYTE		 bPlayerRoomToBuildPri4;
	BYTE		 bPlayerRoomToBuildPri5;
	BYTE		 bPlayerRoomToBuildPri6;
	BYTE		 bPlayerRoomToBuildPri7;
	BYTE		 bPlayerRoomToBuildPri8;
	BYTE		 bPlayerRoomToBuildPri9;
	BYTE		 bPlayerRoomToBuildPri10;
	BYTE		 bPlayerRoomToBuildPri11;
	BYTE		 bPlayerRoomToBuildPri12;
	BYTE		 bPlayerRoomToBuildPri13;
	BYTE		 bPlayerRoomToBuildPri14;
	BYTE		 bPlayerRoomToBuildPri15;
	BYTE		 bPlayerFlexibility;
	BYTE		 bPlayerTilesFromClaimedArea;
	BYTE		 bPlayerBattleCallToHarmsRemove[2];
	BYTE		 bPlayerBoulderInCorridors[4];
	BYTE		 bPlayerBoulderInAngles[4];
	BYTE		 bPlayerTrapUseStyle;
	BYTE		 bPlayerPreference;
	BYTE		 bPlayerDoorUsage;
	BYTE		 bPlayerChanceOfLookingTrapsnDoors;
	BYTE		 bPlayerUseLevelInFirstAttack[4];
	BYTE		 bPlayerUseThreatInFirstAttack[4];
	BYTE		 bPlayerFirstAttackAllRooms[4];
	BYTE		 bPlayerFirstAttackAllSpells[4];
	BYTE		 bPlayerFirstAttackOnlyAttackers[4];
	BYTE		 bPlayerFirstAttackDontAttack[4];
	BYTE		 bPlayerFirstAttackLevel;
	BYTE		 bPlayerFirstAttackThreat;
	BYTE		 bPlayerFirstAttackBreach;
	BYTE		 bPlayerFirstAttackDig;
	BYTE		 bPlayerFirstAttackPoints;
	BYTE		 bPlayerFirstAttackCreatures;
	BYTE		 bPlayerMiscMana;
	BYTE		 bPlayerUnused3;
	BYTE		 bPlayerBattleCallToHarmsPlace[2];
	BYTE		 bPlayerBattleCallToHarmsRemoveNoDanger1;
	BYTE		 bPlayerBattleCallToHarmsRemoveNoDanger2;
	BYTE		 bPlayerBattleRemove[4];
	BYTE		 bPlayerBattleDropped;
	BYTE		 bPlayerBattleSpellType;
	BYTE		 bPlayerBattlePrison;
	BYTE		 bPlayerBattleHealth;
	BYTE		 bPlayerMiscGold;
	BYTE		 bPlayerUnused4;
	BYTE		 bPlayerCreaturesHappyUnhappy[4];
	BYTE		 bPlayerCreaturesHappyUngry[4];
	BYTE		 bPlayerCreaturesDisposeUngry[4];
	BYTE		 bPlayerCreaturesDisposeRubish[4];
	BYTE		 bPlayerCreaturesSackMethod;
	BYTE		 bPlayerCreaturesImps;
	BYTE		 bPlayerCreaturesSlap;
	BYTE		 bPlayerFirstAttackCount;
	BYTE		 bPlayerSpellsLightning[4];
	BYTE		 bPlayerSpellsSightOfEvil;
	BYTE		 bPlayerSpellsInBattle;
	BYTE		 bPlayerSpellsPower;
	BYTE		 bPlayerSpellsCallToArms;
	BYTE		 bPlayerUnused5;
	BYTE		 bPlayerUnused6;
	BYTE		 bPlayerMiscMineGoldUntil[2];
	BYTE		 bPlayerBattleWait[2];
	BYTE		 bPlayerMiscStartingMana[2];
	BYTE		 bPlayerUnused7;
	BYTE		 bPlayerUnused8;
	BYTE		 bPlayerMiscTilesToFindSpecials[2];
	BYTE		 bPlayerMiscImpsToTilesRatio;
	BYTE		 bPlayerUnused9;
	BYTE		 bPlayerBuildAreaX1[2];
	BYTE		 bPlayerBuildAreaY1[2];
	BYTE		 bPlayerBuildAreaX2[2];
	BYTE		 bPlayerBuildAreaY2[2];
	BYTE		 bPlayerMiscLibrary;
	BYTE		 bPlayerMiscChanceToFindSpecials;
	BYTE		 bPlayerMiscChanceOfFindingSpecials;
	BYTE		 bPlayerMiscJailedFate;
	BYTE		 bPlayerTrigger[2];
	BYTE		 bGlobalPlayer;
	BYTE		 bPlayerCameraX[2];
	BYTE		 bPlayerCameraY[2];
	char		 szPlayerName[32];
} GAMEPLAYER;

typedef struct GAMETRIGGERS {
	DWORD		 dwTriggersFileHeader;
	DWORD		 dwTriggersFileVersion;
	DWORD		 dwTriggersFileSize;
	DWORD		 dwTriggersInfoHeader;
	DWORD		 dwTriggersInfoVersion;
	DWORD		 dwTriggersCount;
	DWORD		 dwTriggersActionsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateTriggersDate;
	DWORD		 dwTriggersDefinesHeader;
	DWORD		 dwTriggersDefinesSize;
} GAMETRIGGERS;

typedef struct GAMETERRAINS {
	DWORD		 dwTerrainsFileHeader;
	DWORD		 dwTerrainsFileVersion;
	DWORD		 dwTerrainsFileSize;
	DWORD		 dwTerrainsInfoHeader;
	DWORD		 dwTerrainsInfoVersion;
	DWORD		 dwTerrainsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateTerrainsDate;
	DWORD		 dwTerrainsDefinesHeader;
	DWORD		 dwTerrainsDefinesSize;
} GAMETERRAINS;

typedef struct GAMEROOMS {
	DWORD		 dwRoomsFileHeader;
	DWORD		 dwRoomsFileVersion;
	DWORD		 dwRoomsFileSize;
	DWORD		 dwRoomsInfoHeader;
	DWORD		 dwRoomsInfoVersion;
	DWORD		 dwRoomsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateRoomsDate;
	DWORD		 dwRoomsDefinesHeader;
	DWORD		 dwRoomsDefinesSize;
} GAMEROOMS;

typedef struct GAMEDOORS {
	DWORD		 dwDoorsFileHeader;
	DWORD		 dwDoorsFileVersion;
	DWORD		 dwDoorsFileSize;
	DWORD		 dwDoorsInfoHeader;
	DWORD		 dwDoorsInfoVersion;
	DWORD		 dwDoorsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateDoorsDate;
	DWORD		 dwDoorsDefinesHeader;
	DWORD		 dwDoorsDefinesSize;
} GAMEDOORS;

typedef struct GAMETRAPS {
	DWORD		 dwTrapsFileHeader;
	DWORD		 dwTrapsFileVersion;
	DWORD		 dwTrapsFileSize;
	DWORD		 dwTrapsInfoHeader;
	DWORD		 dwTrapsInfoVersion;
	DWORD		 dwTrapsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateTrapsDate;
	DWORD		 dwTrapsDefinesHeader;
	DWORD		 dwTrapsDefinesSize;
} GAMETRAPS;

typedef struct GAMESPELLS {
	DWORD		 dwSpellsFileHeader;
	DWORD		 dwSpellsFileVersion;
	DWORD		 dwSpellsFileSize;
	DWORD		 dwSpellsInfoHeader;
	DWORD		 dwSpellsInfoVersion;
	DWORD		 dwSpellsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateSpellsDate;
	DWORD		 dwSpellsDefinesHeader;
	DWORD		 dwSpellsDefinesSize;
} GAMESPELLS;

typedef struct GAMECREATURES {
	DWORD		 dwCreaturesFileHeader;
	DWORD		 dwCreaturesFileVersion;
	DWORD		 dwCreaturesFileSize;
	DWORD		 dwCreaturesInfoHeader;
	DWORD		 dwCreaturesInfoVersion;
	DWORD		 dwCreaturesCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateCreaturesDate;
	DWORD		 dwCreaturesDefinesHeader;
	DWORD		 dwCreaturesDefinesSize;
} GAMECREATURES;

typedef struct GAMECRSPELLS {
	DWORD		 dwCrSpellsFileHeader;
	DWORD		 dwCrSpellsFileVersion;
	DWORD		 dwCrSpellsFileSize;
	DWORD		 dwCrSpellsInfoHeader;
	DWORD		 dwCrSpellsInfoVersion;
	DWORD		 dwCrSpellsCount;
	DWORD		 dwGlobalDateHeader;
	GAMEDATE	 dateCrSpellsDate;
	DWORD		 dwCrSpellsDefinesHeader;
	DWORD		 dwCrSpellsDefinesSize;
} GAMECRSPELLS;

