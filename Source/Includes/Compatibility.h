
/*** Ancien identificateurs *********************************************/

#define TRV1_CONTROLAREA	0
#define TRV1_DISCOVERAREA	1
#define TRV1_SELECTAREA		2
#define TRV1_TIMEELAPSED	3

#define ACV1_RELEASEBAND	0
#define ACV1_DESTROYGATE	1
#define ACV1_SHOWAREA		2
#define ACV1_MAKEAREAVISIBLE	3
#define ACV1_ADDCREATURE	4
#define ACV1_CHANGESQUARE	5
#define ACV1_ADDELEMENT		6
#define ACV1_HIGHLIGHTELEM	7
#define ACV1_SHOWTIMER		8
#define ACV1_SHOWROOM		9

/*** Structures des anciennes versions **********************************/

typedef struct MAPTHINGV1 {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	UINT		 id;
	UINT		 owner;
	UINT		 health;
	union {
	LONG		 gold;
	LONG		 level;
	BOOL		 locked;
	UINT		 spell;
	LONG		 all;
	} data;
	UINT		 options;
	UINT		 behavior;
} MAPTHINGV1;

typedef struct MAPTHINGV2 {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	UINT		 id;
	UINT		 owner;
	UINT		 health;
	union {
	LONG		 gold;
	LONG		 level;
	BOOL		 locked;
	UINT		 spell;
	LONG		 all;
	} data;
	UINT		 options;
	UINT		 behavior;
	UINT		 objective;
	UINT		 area;
	ULONG		 uniqueid;
} MAPTHINGV2;

typedef struct MAPTHINGV4 {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	UINT		 id;
	UINT		 owner;
	UINT		 health;
	union {
	LONG		 gold;
	LONG		 level;
	BOOL		 locked;
	UINT		 spell;
	LONG		 all;
	} data;
	DWORD		 herosoptions;
	DWORD		 creaturesoptions;
	UINT		 objective;
	UINT		 area;
	ULONG		 uniqueid;
	UINT		 ennemy;
} MAPTHINGV4;

typedef struct MAPTHINGV5 {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	UINT		 id;
	UINT		 owner;
	UINT		 health;
	union {
	LONG		 gold;
	LONG		 level;
	LONG		 shots;
	UINT		 status;
	UINT		 spell;
	LONG		 all;
	} data;
	DWORD		 herosoptions;
	DWORD		 creaturesoptions;
	UINT		 objective;
	UINT		 area;
	ULONG		 uniqueid;
	UINT		 ennemy;
} MAPTHINGV5;

typedef struct MAPTHINGHEROV1 {
	BOOL		 isvalid;
	LONG		 level;
	UINT		 enemy;
	UINT		 id;
	UINT		 health;
	UINT		 options;
	UINT		 objective;
	UINT		 behavior;
} MAPTHINGHEROV1;

typedef struct MAPTHINGHEROSBANDV1 {
	NODE		 node;
	MAPTHINGHEROV1	 heros[THING_MAXHEROSINBAND+1];		// +1 for endmark
	UINT		 id;
	char		 name[THING_MAXBANDNAME];
} MAPTHINGHEROSBANDV1,MTHBANDV1;

typedef struct MAPTHINGHEROV2 {
	BOOL		 isvalid;
	LONG		 level;
	UINT		 enemy;
	UINT		 id;
	UINT		 health;
	UINT		 options;
	UINT		 objective;
	UINT		 behavior;
	UINT		 area;
	ULONG		 uniqueid;
} MAPTHINGHEROV2;

typedef struct MAPTHINGHEROSBANDV2 {
	NODE		 node;
	MAPTHINGHEROV2	 heros[THING_MAXHEROSINBAND+1];		// +1 for endmark
	UINT		 id;
	ULONG		 uniqueid;
	char		 name[THING_MAXBANDNAME];
} MAPTHINGHEROSBANDV2,MTHBANDV2;

typedef struct MAPTHINGHEROV3 {
	BOOL		 isvalid;
	LONG		 level;
	UINT		 enemy;
	UINT		 id;
	UINT		 health;
	UINT		 options;
	UINT		 objective;
	UINT		 options2;
	UINT		 area;
	ULONG		 uniqueid;
} MAPTHINGHEROV3;

typedef struct MAPTHINGHEROSBANDV3 {
	NODE		 node;
	MAPTHINGHEROV3	 heros[THING_MAXHEROSINBAND+1];		// +1 for endmark
	UINT		 id;
	ULONG		 uniqueid;
	char		 name[THING_MAXBANDNAME];
} MAPTHINGHEROSBANDV3,MTHBANDV3;

typedef struct MAPTHINGHEROV4 {
	BOOL		 isvalid;
	LONG		 level;
	UINT		 enemy;
	UINT		 id;
	UINT		 health;
	UINT		 options;
	UINT		 objective;
	UINT		 options2;
	UINT		 area;
	ULONG		 uniqueid;
} MAPTHINGHEROV4;

typedef struct MAPTHINGHEROSBANDV4 {
	NODE		 node;
	MAPTHINGHEROV4	 heros[THING_MAXHEROSINBAND+1];		// +1 for endmark
	UINT		 id;
	ULONG		 uniqueid;
	char		 name[THING_MAXBANDNAME];
} MAPTHINGHEROSBANDV4,MTHBANDV4;

typedef struct MAPTRIGGERV1 {
	NODE		 node;
	NODE		 actions;
	BOOL		 reusable;
	UINT		 player;
	LONG		 seconds;
	LONG		 squares;
	LONG		 pad[16];
} MAPTRIGGERV1;

typedef struct MAPACTIONV1 {
	NODE		 node;
	UINT		 area;
	UINT		 gate;
	UINT		 band;
	LONG		 x;
	LONG		 y;
	UINT		 creatureid;
	UINT		 terrainid;
	UINT		 player;
	LONG		 level;
	UINT		 type;
	UINT		 element;
	UINT		 room;
	LONG		 seconds;
	BOOL		 activate;
	LONG		 pad[16];
} MAPACTIONV1;

typedef struct MAPACTIONPOINTV1 {
	NODE		 node;
	NODE		 triggers;
	UINT		 area;
	char		 name[64];
} MAPACTIONPOINTV1;

typedef struct MAPPROPERTIESV1 {
	LONG		 Width;
	LONG		 Height;
	LONG		 Players;
	LONG		 FillID;
	char		 Name[64];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BYTE		 pad[15];
} MAPPROPERTIESV1;

typedef struct MAPPROPERTIESV2 {
	LONG		 Width;
	LONG		 Height;
	LONG		 Players;
	LONG		 FillID;
	char		 Name[64];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BYTE		 pad[15];
	BYTE		 SpeechFile[32];
} MAPPROPERTIESV2;

typedef struct MAPPROPERTIESV4 {
	LONG		 Width;
	LONG		 Height;
	LONG		 Players;
	LONG		 FillID;
	char		 Name[64];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BYTE		 pad[13];
	BYTE		 Options;
	BYTE		 Unused;
	BYTE		 SpeechFile[32];
	LONG		 Variables[256];
} MAPPROPERTIESV4;

typedef struct MAPPROPERTIESV5 {
	LONG		 Width;
	LONG		 Height;
	LONG		 Players;
	LONG		 FillID;
	char		 Name[64];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BYTE		 pad[13];
	BYTE		 Options;
	BYTE		 Options2;
	BYTE		 SpeechFile[32];
	LONG		 Variables[256];
	char		 Description[1024];
	char		 Author[64];
	char		 Email[64];
	char		 Information[1024];
	BYTE		 GameTicks;
	BYTE		 HornyTalismanPieces;
	BYTE		 RewardPrev1;
	BYTE		 RewardPrev2;
	BYTE		 RewardPrev3;
	BYTE		 RewardNext1;
	BYTE		 RewardNext2;
	BYTE		 RewardNext3;
	BYTE		 LevelNumber;
	BYTE		 SpecialLevelNumber;
	BYTE		 Soundtrack;
	BYTE		 pad1;
	WORD		 TextTableID;
	WORD		 TextTitleID;
	WORD		 TextPlotID;
	WORD		 TextDebriefingID;
	WORD		 TextObjectiveID;
	WORD		 TextSubObjective1ID;
	WORD		 TextSubObjective2ID;
	WORD		 TextSubObjective3ID;
	BYTE		 TextIntroCreatureID0;
	BYTE		 TextIntroCreatureID1;
	BYTE		 TextIntroCreatureID2;
	BYTE		 TextIntroCreatureID3;
	BYTE		 TextIntroCreatureID4;
	BYTE		 TextIntroCreatureID5;
	BYTE		 TextIntroCreatureID6;
	BYTE		 TextIntroCreatureID7;
	WORD		 TextIntroTextID0;
	WORD		 TextIntroTextID1;
	WORD		 TextIntroTextID2;
	WORD		 TextIntroTextID3;
	WORD		 TextIntroTextID4;
	WORD		 TextIntroTextID5;
	WORD		 TextIntroTextID6;
	WORD		 TextIntroTextID7;
	WORD		 SpeechPreLevelID;
	WORD		 SpeechWinID;
	WORD		 SpeechLooseID;
	WORD		 SpeechNewsID;
	WORD		 SpeechGenericPreLevelID;
} MAPPROPERTIESV5;

typedef struct MAPPROPERTIESV6 {
	LONG		 Width;
	LONG		 Height;
	LONG		 Players;
	LONG		 FillID;
	char		 Name[64];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BYTE		 pad[12];
	BYTE		 Version;
	BYTE		 Options;
	BYTE		 Options2;
	BYTE		 SpeechFile[32];
	LONG		 Variables[256];
	char		 Description[1024];
	char		 Author[64];
	char		 Email[64];
	char		 Information[1024];
	BYTE		 GameTicks;
	BYTE		 HornyTalismanPieces;
	BYTE		 RewardPrev1;
	BYTE		 RewardPrev2;
	BYTE		 RewardPrev3;
	BYTE		 RewardNext1;
	BYTE		 RewardNext2;
	BYTE		 RewardNext3;
	BYTE		 LevelNumber;
	BYTE		 SpecialLevelNumber;
	BYTE		 Soundtrack;
	BYTE		 pad1;
	WORD		 TextTableID;
	WORD		 TextTitleID;
	WORD		 TextPlotID;
	WORD		 TextDebriefingID;
	WORD		 TextObjectiveID;
	WORD		 TextSubObjective1ID;
	WORD		 TextSubObjective2ID;
	WORD		 TextSubObjective3ID;
	BYTE		 TextIntroCreatureID0;
	BYTE		 TextIntroCreatureID1;
	BYTE		 TextIntroCreatureID2;
	BYTE		 TextIntroCreatureID3;
	BYTE		 TextIntroCreatureID4;
	BYTE		 TextIntroCreatureID5;
	BYTE		 TextIntroCreatureID6;
	BYTE		 TextIntroCreatureID7;
	WORD		 TextIntroTextID0;
	WORD		 TextIntroTextID1;
	WORD		 TextIntroTextID2;
	WORD		 TextIntroTextID3;
	WORD		 TextIntroTextID4;
	WORD		 TextIntroTextID5;
	WORD		 TextIntroTextID6;
	WORD		 TextIntroTextID7;
	WORD		 SpeechPreLevelID;
	WORD		 SpeechWinID;
	WORD		 SpeechLooseID;
	WORD		 SpeechNewsID;
	WORD		 SpeechGenericPreLevelID;
} MAPPROPERTIESV6;

typedef struct MAPOPTIONSV1 {
	LONG		 HeartX;
	LONG		 HeartY;
	LONG		 Gold;
	LONG		 Health;
	BOOL		 ComputerPlayer;
	UINT		 AI;
	char		 PlayerName[32];
	LONG		 CreaturesCount[MAP_VARCREATURES];
	BYTE		 Rooms[MAP_VARROOMS];
	BYTE		 Traps[MAP_VARTRAPS];
	BYTE		 Doors[MAP_VARDOORS];
	BYTE		 Spells[MAP_VARSPELLS];
	BYTE		 Pad[15];
	LONG		 CPUAreaX1;
	LONG		 CPUAreaY1;
	LONG		 CPUAreaX2;
	LONG		 CPUAreaY2;
	BYTE		 CreaturesAvailable[MAP_VARCREATURES];
	BYTE		 ext[67];
} MAPOPTIONSV1;

typedef struct MAPOPTIONSV2 {
	LONG		 CreaturesCount[MAP_VARCREATURES];
	BYTE		 Rooms[MAP_VARROOMS];
	BYTE		 Traps[MAP_VARTRAPS];
	BYTE		 Doors[MAP_VARDOORS];
	BYTE		 Spells[MAP_VARSPELLS];
	BYTE		 CreaturesAvailable[MAP_VARCREATURES];
	BYTE		 Pad0[2];
	LONG		 PlayerHealth;
	LONG		 PlayerGold;
	BOOL		 PlayerComputer;
	LONG		 PlayerAI;
	BYTE		 PlayerSpeed;
	BYTE		 PlayerOpenness;
	BYTE		 PlayerGoldValue;
	BYTE		 PlayerManaValue;
	LONG		 PlayerMineGoldAmount;
	LONG		 PlayerMana;
	BYTE		 PlayerLibraryUsage;
	BYTE		 PlayerImpsToTilesRatio;
	BYTE		 PlayerFindSpecialsExplore;
	BYTE		 PlayerFindSpecialsChances;
	LONG		 PlayerFindSpecialsTiles;
	BYTE		 PlayerJailedFate;
	BYTE		 Pad1[3];
	BYTE		 PlayerResearchedRoom;
	BYTE		 PlayerRebuildRoom;
	BYTE		 PlayerDefaultTiles;
	BYTE		 PlayerTilesBetweenRooms;
	BYTE		 PlayerDistance;
	BYTE		 PlayerCorridorStyle;
	BYTE		 PlayerExpandMode;
	BYTE		 PlayerTilesFromHeart;
	BYTE		 PlayerTilesFromClaimed;
	BYTE		 PlayerFlexibility;
	BYTE		 PlayerRoomToBuildPri[15];
	BYTE		 Pad2[3];
	BOOL		 PlayerSellCapturedRoom;
	BOOL		 PlayerCreateEmptyAreas;
	BOOL		 PlayerBuildBiggerLair;
	BYTE		 PlayerTrapUseStyle;
	BYTE		 PlayerPreference;
	BYTE		 PlayerDoorUsage;
	BYTE		 PlayerUseTrapsnDoors;
	BYTE		 PlayerSpellsSightOfEvil;
	BYTE		 PlayerSpellsInBattle;
	BYTE		 PlayerSpellsPower;
	BYTE		 PlayerSpellsCallToArms;
	BOOL		 PlayerBoulderInCorridors;
	BOOL		 PlayerBoulderInAngles;
	BOOL		 PlayerSpellsLightning;
	BYTE		 PlayerCreaturesSackMethod;
	BYTE		 PlayerCreaturesImps;
	BYTE		 PlayerCreaturesSlap;
	BYTE		 Pad3[1];
	BOOL		 PlayerCreaturesHappyUnhappy;
	BOOL		 PlayerCreaturesHappyUngry;
	BOOL		 PlayerCreaturesDisposeUngry;
	BOOL		 PlayerCreaturesDisposeRubish;
	BOOL		 PlayerUseLevelInFirstAttack;
	BOOL		 PlayerUseCountInFirstAttack;
	BOOL		 PlayerUseThreatInFirstAttack;
	BOOL		 PlayerFirstAttackAllRooms;
	BOOL		 PlayerFirstAttackAllSpells;
	BOOL		 PlayerFirstAttackOnlyAttackers;
	BOOL		 PlayerFirstAttackDontAttack;
	BYTE		 PlayerFirstAttackCount;
	BYTE		 PlayerFirstAttackLevel;
	BYTE		 PlayerFirstAttackThreat;
	BYTE		 PlayerFirstAttackBreach;
	BYTE		 PlayerFirstAttackDig;
	BYTE		 PlayerFirstAttackPoints;
	BYTE		 PlayerFirstAttackCreatures;
	BYTE		 PlayerBattleDropped;
	BYTE		 PlayerBattleSpellType;
	BYTE		 PlayerBattlePrison;
	BYTE		 PlayerBattleHealth;
	BYTE		 PlayerBattleRemove;
	LONG		 PlayerBattleWait;
	LONG		 PlayerBattleCallToHarmsRemoveNoCreature;
	LONG		 PlayerBattleCallToHarmsRemove;
	LONG		 PlayerBattleCallToHarmsPlace;
	LONG		 PlayerBattleCallToHarmsRemoveNoDanger1;
	LONG		 PlayerBattleCallToHarmsRemoveNoDanger2;
	LONG		 PlayerCPUAreaX1;
	LONG		 PlayerCPUAreaY1;
	LONG		 PlayerCPUAreaX2;
	LONG		 PlayerCPUAreaY2;
	LONG		 PlayerCameraX;			// Ignored in this version
	LONG		 PlayerCameraY;			// Ignored in this version
	char		 PlayerName[32];
} MAPOPTIONSV2;

typedef struct TRIGGERLINKV2 {
	NODE		 node;
	unsigned long	 player;
	unsigned long	 area;
	unsigned long	 creature;
	unsigned long	 band;
	short		 trigger;
	short		 pad;
} TRIGGERLINKV2;
