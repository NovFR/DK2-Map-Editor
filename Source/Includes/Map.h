
/*** Valeurs par défaut *************************************************/

#define MAP_CONTROLBYTES	4
#define MAP_VARCREATURES	45
#define MAP_VARROOMS		15
#define MAP_VARTRAPS		14
#define MAP_VARDOORS		8
#define MAP_VARSPELLS		12
#define MAP_MAXPLAYERS		7

#define MAP_DEFAULTWIDTH	80
#define MAP_DEFAULTHEIGHT	80
#define MAP_DEFAULTZOOMX	48
#define MAP_DEFAULTZOOMY	48
#define MAP_DEFAULTID		0x02020101

/*** Identificateurs ****************************************************/

#define MAP_CHOICELMBPOINT	0
#define MAP_CHOICELMBRECT	1

/*** Drapeaux ***********************************************************/

#define SHOW_CURSOR		0x00000001
#define SHOW_SELRECT		0x00000002
#define SHOW_UPDATEINFOWND	0x10000000

/*** Structures de la carte *********************************************/

typedef struct MAPPROPERTIES {
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
} MAPPROPERTIES;

typedef struct MAP {
	BYTE		*Map;
	HDC		 hDC;
	HBITMAP		 hBitmap;
	HBRUSH		 hBrush;
	HPEN		 hPen;
	HBITMAP		 hOldBitmap;
	HBRUSH		 hOldBrush;
	HPEN		 hOldPen;
	MAPPROPERTIES	 Properties;
} MAP;

typedef struct MAPRECT {
	LONG		 x1;
	LONG		 y1;
	LONG		 x2;
	LONG		 y2;
	BOOL		 hidden;
	DWORD		 color;
	DWORD		 textcolor;
	char		*name;
} MAPRECT;

typedef struct MAPCLIENT {
	LONG		 X;
	LONG		 Y;
	LONG		 Width;
	LONG		 Height;
} MAPCLIENT;

typedef struct MAPCURSOR {
	HDC		 hdc;
	HBITMAP		 hbitmap;
	HBITMAP		 holdbitmap;
} MAPCURSOR;

typedef struct MAPINFO {
	BYTE		*Map;
	LONG		 Width;
	LONG		 Height;
	LONG		 RectX;
	LONG		 RectY;
	LONG		 RectWidth;
	LONG		 RectHeight;
} MAPINFO;

typedef struct MAPSTATS {
	LONG		 Creatures;
	LONG		 Doors;
	LONG		 Traps;
	LONG		 Objects;
	LONG		 MagicalObjects;
	LONG		 Areas;
	LONG		 Gates;
	LONG		 HerosBands;
} MAPSTATS;

typedef struct MAPCOPY {
	BYTE		*Map;
	LONG		 Width;
	LONG		 Height;
} MAPCOPY;

/*** Structures de la routine de sélection d'une position ***************/

typedef struct MAPCHOOSEPOINTCTX {
	MAPRECT		 SavedRect;
	MAPRECT		 OrgRect;
	MAPRECT		 MapRect;
	UINT		 Mode;
	char		*Title;
	BOOL		 DefiningRect;
	LONG		 X;
	LONG		 Y;
	LONG		 CursorX;
	LONG		 CursorY;
	HWND		 hVScroll;
	HWND		 hHScroll;
	HWND		 hButton;
	HWND		 hInfo;
	HWND		 hOk;
	HWND		 hCancel;
	HICON		 hIcon;
	char		 Output[128];
} MAPCHOOSEPOINTCTX;

/*** Structures des propriétés de la carte ******************************/

typedef struct MAPNEWCTX {
	HWND		 NewList;
	HIMAGELIST	 NewImages;
	MAPPROPERTIES	 NewProperties;
} MAPNEWCTX;

typedef struct MAPOPTIONS {
	LONG		 CreaturesCount[MAP_VARCREATURES];
	BYTE		 Rooms[MAP_VARROOMS];
	BYTE		 Traps[MAP_VARTRAPS];
	BYTE		 Doors[MAP_VARDOORS];
	BYTE		 Spells[MAP_VARSPELLS];
	BYTE		 CreaturesAvailable[MAP_VARCREATURES];
	BYTE		 Pad0[2];
	//--- Player general params ---
	LONG		 PlayerHealth;
	LONG		 PlayerGold;
	BOOL		 PlayerComputer;
	//--- Start of CPU only params ---
	LONG		 PlayerAI;
	BYTE		 PlayerSpeed;
	BYTE		 PlayerOpenness;
	BYTE		 PlayerGoldValue;
	BYTE		 PlayerManaValue;
	LONG		 PlayerMineGoldAmount;
	LONG		 PlayerMana;
	//--- Player special params ---
	BYTE		 PlayerLibraryUsage;
	BYTE		 PlayerImpsToTilesRatio;
	BYTE		 PlayerFindSpecialsExplore;
	BYTE		 PlayerFindSpecialsChances;
	LONG		 PlayerFindSpecialsTiles;
	BYTE		 PlayerJailedFate;
	BYTE		 Pad1[3];
	//--- Player rooms params ---
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
	//--- Player doors, traps & spells params ---
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
	//--- Player creatures params ---
	BYTE		 PlayerCreaturesSackMethod;
	BYTE		 PlayerCreaturesImps;
	BYTE		 PlayerCreaturesSlap;
	BYTE		 Pad3[1];
	BOOL		 PlayerCreaturesHappyUnhappy;
	BOOL		 PlayerCreaturesHappyUngry;
	BOOL		 PlayerCreaturesDisposeUngry;
	BOOL		 PlayerCreaturesDisposeRubish;
	//--- Player battles params ---
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
	//--- Player build area ---
	LONG		 PlayerCPUAreaX1;
	LONG		 PlayerCPUAreaY1;
	LONG		 PlayerCPUAreaX2;
	LONG		 PlayerCPUAreaY2;
	//--- End of CPU only params ---
	LONG		 PlayerCameraX;
	LONG		 PlayerCameraY;
	char		 PlayerName[32];
} MAPOPTIONS;

typedef struct MAPPROPSCTX {
	HIMAGELIST	 hArrows;
	HIMAGELIST	 hImages;
	HIMAGELIST	 hCPUImages;
	HWND		 hRooms;
	HWND		 hCreatures;
	HWND		 hVariables;
	HWND		 hDefinitions;
	HWND		 hRoomsOrder;
	HICON		 UpIcon;
	HICON		 DownIcon;
	BOOL		 DoNotUpdate;
	UINT		 PlayerToModify;
	UINT		 MapNewPos;
	LONG		 MapNewWidth;
        LONG		 MapNewHeight;
	LONG		 MapCameraX;
	LONG		 MapCameraY;
	MAPPROPERTIES	 MapProperties;
	MAPOPTIONS	 CurrentPlayer;
	MAPOPTIONS	 CurrentPlayerCPUSettings;
	MAPOPTIONS	 Players[MAP_MAXPLAYERS];
	BYTE		 Allies[MAP_MAXPLAYERS][MAP_MAXPLAYERS];
	BOOL		 Selected[2][MAP_MAXPLAYERS];
	DWORD		 CreaturesParams[MAP_VARCREATURES][64];
	LONG		 Variables[256];
	BOOL		 PageInitialized[9];
	LONG		 SpeechMentorMaxIdentifier;
	NODE		 SpeechList;
	char		 SpeechFile[32];
} MAPPROPSCTX;

typedef struct MAPPROPSSOUND {
	NODE		 node;
	char		 name[64];
} MAPPROPSSOUND;

typedef struct MAPPROPSTEXT {
	NODE		 node;
	char		 textpreview[128];
} MAPPROPSTEXT;

