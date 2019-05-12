
/*** Drapeaux ***********************************************************/

//--- Fonction d'initialisation ---

#define MGRF_TERRAINS		0x00000001
#define MGRF_ROOMS		0x00000002
#define MGRF_DOORS		0x00000004
#define MGRF_TRAPS		0x00000008
#define MGRF_SPELLS		0x00000010
#define MGRF_CRATTRS		0x00000100
#define MGRF_CRPARAMS		0x00000200
#define MGRF_CRFLAGS		0x00000400
#define MGRF_CRATTACKS		0x00000800
#define MGRF_CRRESISTANCES	0x00001000
#define MGRF_CRPOSSESSION	0x00002000
#define MGRF_CRJOBS		0x00004000
#define MGRF_CRLIGHT		0x00008000
#define MGRF_CREATURESPELLS	0x00010000
#define MGRF_CREATURES		MGRF_CRATTRS|MGRF_CRPARAMS|MGRF_CRFLAGS|MGRF_CRATTACKS|MGRF_CRRESISTANCES|MGRF_CRPOSSESSION|MGRF_CRJOBS|MGRF_CRLIGHT
#define MGRF_ALL		-1

#define MGEF_EFFECT0		0x00000001
#define MGEF_EFFECT1		0x00000002
#define MGEF_EFFECT2		0x00000004
#define MGEF_EFFECT3		0x00000008
#define MGEF_EFFECT4		0x00000010
#define MGEF_EFFECT5		0x00000020
#define MGEF_OBJECT0		0x00000100
#define MGEF_OBJECT1		0x00000200
#define MGEF_OBJECT2		0x00000400
#define MGEF_OBJECT3		0x00000800
#define MGEF_OBJECT4		0x00001000
#define MGEF_OBJECT5		0x00002000
#define MGEF_CANTBUILD		0x80000000

//--- Pièges ---

#define GTF_REVEALEDWHENFIRED	0
#define GTF_SOLIDOBSTACLE	1
#define GTF_INVISIBLE		2
#define GTF_FIRSTPERSONOBSTACLE	3
#define GTF_REQUIREMANA		4
#define GTF_DISARMABLE		5

//--- Sorts ---

#define GSF_ATTACKING		0
#define GSF_DEFENSIVE		1

//--- Occupations ---

#define JOB_HAPPY		0
#define JOB_UNHAPPY		1
#define JOB_ANGRY		2
#define JOB_HATE		3

#define GLOBAL_HAPPYJOBS	3
#define GLOBAL_UNHAPPYJOBS	2
#define GLOBAL_ANGRYJOBS	3
#define GLOBAL_HATEJOBS		4

/*** Structures *********************************************************/

typedef struct GLOBALTERRAIN {
	UINT		 MaxHealthType;
	UINT		 MaxHealthEffect;
	UINT		 DestroyedType;
	UINT		 DestroyedEffect;
	LONG		 Damage;
	LONG		 GoldValue;
	LONG		 MaxHealth;
	LONG		 StartingHealth;
	LONG		 ManaGain;
	LONG		 MaxManaGain;
	LONG		 WibbleH;
	LONG		 WibbleV;
	BOOL		 Visible;
	BOOL		 TerrainLight;
	BOOL		 AmbientLight;
	LONG		 TerrainLightH;
	BYTE		 TerrainLightR;
	BYTE		 TerrainLightG;
	BYTE		 TerrainLightB;
	BYTE		 AmbientLightR;
	BYTE		 AmbientLightG;
	BYTE		 AmbientLightB;
	BYTE		 Pad[2];
} GLOBALTERRAIN;

typedef struct GLOBALROOM {
	LONG		 Cost;
	UINT		 Objects[6];
	UINT		 Effects[6];
	UINT		 BattleEffect;
	LONG		 CeilingHeight;
	LONG		 TorchIntensity;
	LONG		 TorchLightRad;
	LONG		 TorchLightH;
	BYTE		 TorchLightR;
	BYTE		 TorchLightG;
	BYTE		 TorchLightB;
	BYTE		 Pad[1];
} GLOBALROOM;

typedef struct GLOBALDOOR {
	LONG		 Cost;
	LONG		 BuildPoints;
	LONG		 Health;
	LONG		 HealthGain;
	LONG		 ManaCost;
	UINT		 Material;
	UINT		 DeathEffect;
	UINT		 Crate;
	UINT		 Key;
} GLOBALDOOR;

typedef struct GLOBALTRAP {
	LONG		 Health;
	LONG		 HealthGain;
	LONG		 ChargeTime;
	LONG		 RechargeTime;
	LONG		 GoldCost;
	LONG		 ManaCostToFire;
	LONG		 ManaUsage;
	LONG		 Threat;
	LONG		 ThreatDuration;
	LONG		 IdleEffectDelay;
	LONG		 ShotDelay;
	LONG		 BuildPoints;
	BOOL		 Invisible;
	BOOL		 RevealedWhenFired;
	BOOL		 RequiresMana;
	BOOL		 Disarmable;
	BOOL		 SolidObstacle;
	BOOL		 FPObstacle;
	UINT		 Material;
	UINT		 PowerlessEffect;
	UINT		 IdleEffect;
	UINT		 DeathEffect;
	UINT		 Crate;
	UINT		 ShotType;
	LONG		 ShotData1;
	LONG		 ShotData2;
	UINT		 TriggerType;
	LONG		 TriggerData;
} GLOBALTRAP;

typedef struct GLOBALSPELL {
	LONG		 ManaCost;
	LONG		 ManaDrain;
	LONG		 RechargeTime;
	BOOL		 Attacking;
	BOOL		 Defensive;
	UINT		 HandAnim;
	UINT		 NoGoHandAnim;
	UINT		 CastRule;
	UINT		 TargetRule;
	UINT		 ShotType;
	LONG		 ShotData1;
	LONG		 ShotData2;
	UINT		 UpgShotType;
	LONG		 UpgShotData1;
	LONG		 UpgShotData2;
	LONG		 UpgRechargeTime;
} GLOBALSPELL;

typedef struct GLOBALCREATURE {
	//--- Attributes ---
	LONG		 ExpPtsForNextLevel;
	LONG		 ExpPts;
	LONG		 ExpPtsTraining;
	LONG		 ResearchPts;
	LONG		 ManufacturePts;
	LONG		 Height;
	LONG		 EyeHeight;
	LONG		 DistanceCanSee;
	LONG		 DistanceCanHear;
	LONG		 Health;
	LONG		 HealthFromChicken;
	LONG		 SlapDamage;
	LONG		 Fear;
	LONG		 Threat;
	LONG		 PrayerMana;
	LONG		 Pay;
	LONG		 InitialGoldHeld;
	LONG		 MaxGoldHeld;
	LONG		 Speed;
	LONG		 RunSpeed;
	LONG		 AngerNoLair;
	LONG		 AngerNoFood;
	LONG		 AngerNoPay;
	LONG		 AngerNoWork;
	LONG		 AngerSlap;
	LONG		 AngerInHand;
	LONG		 TimeAwake;
	LONG		 SleepDuration;
	LONG		 StunDuration;
	LONG		 GuardDuration;
	LONG		 IdleDuration;
	LONG		 SlapDuration;
	LONG		 PossessionManaCost;
	LONG		 OwnLandHealthInc;
	LONG		 TortureHealthCng;
	LONG		 TortureMoodCng;
	LONG		 HungerRate;
	LONG		 DecomposeValue;
	//--- Params ---
	UINT		 ArmourType;
	UINT		 FightStyle;
	UINT		 JobClass;
	UINT		 LairObject;
	UINT		 SlapEffect;
	UINT		 HealthEffect;
	UINT		 EntranceEffect;
	UINT		 DeathEffect;
	UINT		 Attract1Type;
	UINT		 Attract1ID;
	UINT		 Attract2Type;
	UINT		 Attract2ID;
	LONG		 Attract2Data;
	//--- Flags ---
	BOOL		 DoNotTrain;
	BOOL		 CanBePicked;
	BOOL		 CanBeSlapped;
	BOOL		 CanWalkWater;
	BOOL		 CanWalkLava;
	BOOL		 CanBeHypnotized;
	BOOL		 ImmuneTurncoat;
	BOOL		 ImmuneChicken;
	BOOL		 ImmuneLightning;		// Does not work
	BOOL		 DeadBody;
	BOOL		 NoEnemies;
	BOOL		 FreeFriends;
	BOOL		 RevealTraps;
	BOOL		 Fearless;
	BOOL		 Emotionless;			// Does not work
	BOOL		 RandomInvader;
	//--- Attacks ---
	UINT		 Shot1ID;
	LONG		 Shot1Delay;
	LONG		 Shot1Level;
	LONG		 Shot1X;
	LONG		 Shot1Y;
	LONG		 Shot1Z;
	UINT		 Shot2ID;
	LONG		 Shot2Delay;
	LONG		 Shot2Level;
	LONG		 Shot2X;
	LONG		 Shot2Y;
	LONG		 Shot2Z;
	UINT		 Shot3ID;
	LONG		 Shot3Delay;
	LONG		 Shot3Level;
	LONG		 Shot3X;
	LONG		 Shot3Y;
	LONG		 Shot3Z;
	UINT		 MeleeID;
	LONG		 RechargeTime;
	LONG		 Range;
	LONG		 Dammage;
	LONG		 Delay;
	//--- Resistances ---
	UINT		 Res1;
	LONG		 Res1Rate;
	UINT		 Res2;
	LONG		 Res2Rate;
	UINT		 Res3;
	LONG		 Res3Rate;
	UINT		 Res4;
	LONG		 Res4Rate;
	//--- Possession ---
	UINT		 SpecialAb1;
	LONG		 SpecialAb1Data;
	UINT		 SpecialAb2;
	LONG		 SpecialAb2Data;
	UINT		 Melee1;
	UINT		 Melee2;
	UINT		 Special1;
	UINT		 Special2;
	UINT		 Spell;
	UINT		 GammaEffect;
	LONG		 WaddleScale;
	LONG		 OscillateScale;
	LONG		 WalkScale;
	BOOL		 CameraRolls;
	//--- Jobs ---
	UINT		 HJobs[GLOBAL_HAPPYJOBS];
	LONG		 HJobsMood[GLOBAL_HAPPYJOBS];
	LONG		 HJobsMana[GLOBAL_HAPPYJOBS];
	LONG		 HJobsChance[GLOBAL_HAPPYJOBS];
	UINT		 UJobs[GLOBAL_UNHAPPYJOBS];
	LONG		 UJobsMood[GLOBAL_UNHAPPYJOBS];
	LONG		 UJobsMana[GLOBAL_UNHAPPYJOBS];
	LONG		 UJobsChance[GLOBAL_UNHAPPYJOBS];
	UINT		 AJobs[GLOBAL_ANGRYJOBS];
	LONG		 AJobsMood[GLOBAL_ANGRYJOBS];
	LONG		 AJobsMana[GLOBAL_ANGRYJOBS];
	LONG		 AJobsChance[GLOBAL_ANGRYJOBS];
	UINT		 NJobs[GLOBAL_HATEJOBS];
	//--- Lumière ---
	LONG		 LightX;
	LONG		 LightY;
	LONG		 LightZ;
	LONG		 LightRadius;
	BYTE		 LightR;
	BYTE		 LightG;
	BYTE		 LightB;
	BYTE		 pad[1];
	BOOL		 LightFlicker;
	BOOL		 LightPulse;
	BOOL		 LightPlayer;
} GLOBALCREATURE;

typedef struct GLOBALCRSPELLS {
	LONG		 RechargeTime;
	LONG		 Range;
	UINT		 Shot;
	LONG		 ShotData1;
	LONG		 ShotData2;
	BOOL		 Attacking;
	BOOL		 Creature;
	BOOL		 Defensive;
	BOOL		 CastSelf;
	BOOL		 CastOthers;
	BOOL		 CastCreatures;
} GLOBALCRSPELLS;

typedef struct GLOBALVARS {
	GLOBALTERRAIN	 Terrains[38];
	GLOBALROOM	 Rooms[24];
	GLOBALDOOR	 Doors[8];
	GLOBALTRAP	 Traps[15];
	GLOBALSPELL	 Spells[13];
	GLOBALCREATURE	 Creatures[46];
	GLOBALCRSPELLS	 CrSpells[41];
} GLOBALVARS;

typedef struct GLOBALATTRLIST {
	DWORD		 Offset;
	DWORD		 DefaultOffset;
	BOOL		 Float;
	BOOL		 Signed;
	char		*Name;
} GLOBALATTRLIST;

/*** Structures des propriétés de la carte ******************************/

//--- Tir ---

typedef struct GLOBALSHOT {
	HIMAGELIST	 OptionsImages;
	UINT		 ShotType;
	LONG		 ShotData1;
	LONG		 ShotData2;
	UINT		 DialogID;
	UINT		 Text1ID;
	UINT		 Text2ID;
	BOOL		 Ctrl1Signed;
	BOOL		 Ctrl2Signed;
} GLOBALSHOT;

//--- Eléments ---

typedef struct GLOBALCTX {
	HIMAGELIST	 TerrainsImages;
	HIMAGELIST	 RoomsImages;
	HIMAGELIST	 DoorsImages;
	HIMAGELIST	 TrapsImages;
	HIMAGELIST	 SpellsImages;
	GLOBALVARS	 Variables;
	UINT		 CurrentTerrain;
	UINT		 CurrentRoom;
	UINT		 CurrentDoor;
	UINT		 CurrentTrap;
	UINT		 CurrentSpell;
	BOOL		 BonusShot;
	GLOBALSHOT	 ShotCtx;
	UINT		 TriggerType;
	LONG		 TriggerData;
	COLORREF	 TorchColor;
	COLORREF	 TerrainColor;
	COLORREF	 AmbientColor;
	BOOL		 PageInitialized[5];
} GLOBALCTX;

//--- Créatures ---

typedef struct GLOBALJOB {
	BOOL		 Selected;
	BOOL		 HasNoData;
	UINT		 CtrlId;
	UINT		 Job;
	LONG		 JobChance;
	LONG		 JobMood;
	LONG		 JobMana;
} GLOBALJOB;

typedef struct GLOBALCTX2 {
	HIMAGELIST	 SpellsImages;
	HIMAGELIST	 CreaturesImages;
	HWND		 CreaturesAttrList;
	UINT		 CurrentCreature;
	UINT		 CurrentSpell;
	GLOBALVARS	 Variables;
	GLOBALVARS	 DefaultVr;
	BOOL		 PageInitialized[9];
	GLOBALJOB	 HappyJobs[GLOBAL_HAPPYJOBS+1];
	GLOBALJOB	 UnhappyJobs[GLOBAL_UNHAPPYJOBS+1];
	GLOBALJOB	 AngryJobs[GLOBAL_ANGRYJOBS+1];
	GLOBALJOB	 HateJobs[GLOBAL_HATEJOBS+1];
	COLORREF	 LightColor;
	GLOBALSHOT	 ShotCtx;
	char		 TempText[1024];
} GLOBALCTX2;

