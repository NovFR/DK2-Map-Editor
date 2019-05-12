
/*** Structures *********************************************************/

typedef struct MAPEDITSTATS {
	SYSTEMTIME	 StartTime;
	ULONG		 TotalTicks;
	ULONG		 LMBClicks;
	ULONG		 RMBClicks;
	ULONG		 UsageOfPaintThings;
	ULONG		 UsageOfPaintFill;
	ULONG		 UsageOfPaint;
	ULONG		 UsageOfPaintRnd;
	ULONG		 SavedTimes;
	SYSTEMTIME	 LastSaved;
	ULONG		 RealTicks;
	ULONG		 UsageOfUndo;
	SYSTEMTIME	 CreatedTime;
	SYSTEMTIME	 CompileTime;
	ULONG		 Ext[22];
} MAPEDITSTATS;

typedef struct MAPEDITPLYSTATS {
	LONG		 CreaturesCount[256];
	LONG		 CreaturesLevels[256];
	LONG		 TrapsCount[256];
	LONG		 DoorsCount[256];
	LONG		 ObjectsCount[256];
	LONG		 MagicalObjectsCount[256];
	LONG		 TreasuryCapacity;
	LONG		 RoomsCapacity;
	LONG		 ManaIncrease;
	LONG		 Imps;
	LONG		 Creatures;
	LONG		 Rooms;
	LONG		 Walls;
	LONG		 Claimed;
	LONG		 Traps;
	LONG		 Doors;
	LONG		 Objects;
	LONG		 MagicalObjects;
	LONG		 HeartX;
	LONG		 HeartY;
	LONG		 CreaturesAvgLevel;
	UINT		 PreferredCreature;
	UINT		 PreferredTrap;
	UINT		 PreferredDoor;
	UINT		 PreferredObject;
	UINT		 PreferredMagicalObject;
} MAPEDITPLYSTATS;

typedef struct MAPEDITBANDSTATS {
	LONG		 UseCount;
	LONG		 CreaturesCount[256];
	LONG		 CreaturesLevels[256];
	LONG		 Dwarfs;
	LONG		 Imps;
	LONG		 Creatures;
	LONG		 CreaturesAvgLevel;
	UINT		 PreferredCreature;
} MAPEDITBANDSTATS;

typedef struct MAPEDITSTATSCTX {
	BOOL		 AlreadySet;
	LONG		 CreaturesCountInBands[256];
	LONG		 CreaturesLevelsInBands[256];
	LONG		 CreaturesCount[256];
	LONG		 CreaturesLevels[256];
	LONG		 TrapsCount[256];
	LONG		 DoorsCount[256];
	LONG		 ObjectsCount[256];
	LONG		 MagicalObjectsCount[256];
	LONG		 TotalHeroesCreatures;
	LONG		 TotalDwarfsInBands;
	LONG		 TotalImpsInBands;
	LONG		 TotalCreaturesInBands;
	LONG		 TotalCreatures;
	LONG		 TotalTraps;
	LONG		 TotalDoors;
	LONG		 TotalObjects;
	LONG		 TotalMagicalObjects;
	LONG		 TotalSquares;
	LONG		 TotalWater;
	LONG		 TotalLava;
	LONG		 TotalSolid;
	LONG		 TotalRock;
	LONG		 TotalUnclaimed;
	double		 RelWater;
	double		 RelLava;
	double		 RelSolid;
	double		 RelRock;
	double		 RelUnclaimed;
	LONG		 HeroesCreaturesAvgLevel;
	LONG		 CreaturesAvgLevelInBands;
	LONG		 CreaturesAvgLevel;
	LONG		 GoldInLevel;
	LONG		 GoldInObjects;
	LONG		 GoldInMagics;
	BOOL		 IsThereUnlimitedGold;
	UINT		 PreferredHeroesCreature;
	UINT		 PreferredCreatureInBands;
	UINT		 PreferredCreature;
	UINT		 PreferredTrap;
	UINT		 PreferredDoor;
	UINT		 PreferredObject;
	UINT		 PreferredMagicalObject;
	MAPEDITPLYSTATS	 Players[MAP_MAXPLAYERS];
	MAPEDITBANDSTATS Bands[24];
	char		 Date[256];
	char		 BandName[256];
} MAPEDITSTATSCTX;

