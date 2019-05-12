
/*** Définitions globales ***********************************************/

#define TRIGGERIMAGE_CREATURE	0
#define TRIGGERIMAGE_BAD	1
#define TRIGGERIMAGE_ELITE	2
#define TRIGGERIMAGE_HERO	3
#define TRIGGERIMAGE_DOOR	4
#define TRIGGERIMAGE_TRAP	5
#define TRIGGERIMAGE_SPELL	6
#define TRIGGERIMAGE_ROOM	7
#define TRIGGERIMAGE_PLAYER	8
#define TRIGGERIMAGE_OBJECT	9
#define TRIGGERIMAGE_MAGIC	10

#define TYPE_TRIGGER		0x000000D5
#define TYPE_ACTION		0x000000D6

#define TRIGGERS_MAXVARIABLES	129
#define TRIGGERS_MAXTIMERS	17
#define TRIGGERS_MAXVARLEN	64
#define TRIGGERS_MAXTIMLEN	64

#define SCOPE_PLAYER		0x00000001
#define SCOPE_CREATURE		0x00000002
#define SCOPE_AREA		0x00000004
#define SCOPE_BAND		0x00000008
#define SCOPE_CREATUREINBAND	0x00000010
#define SCOPE_LEVEL		0x00000020
#define SCOPE_DOOR		0x00000040
#define SCOPE_OBJECT		0x00000080
#define SCOPE_MAGIC		0x00000100
#define SCOPE_GATE		0x01000000	// For test routine ONLY
#define SCOPE_EFFECT		0x02000000	// For test routine ONLY

#define SCOPE_KILL		0x10000000
#define SCOPE_MODIFY2		0x20000000
#define SCOPE_MODIFY		0x40000000
#define SCOPE_RESIZE		0x80000000

typedef struct TRIGGERINFO {
	DWORD		 ctrl;
	char		*text;
	UINT		 dialog;
	DWORD		 scope;
} TRIGGERINFO;

typedef struct TRIGGERLINK {
	NODE		 node;
	unsigned long	 player;
	unsigned long	 area;
	unsigned long	 creature;
	unsigned long	 door;
	unsigned long	 object;
	unsigned long	 band;
	short		 trigger;
	short		 pad;
} TRIGGERLINK;

/*** Définitions des déclencheurs ***************************************/

#define TRIGGER_VARIABLE	0x01
#define TRIGGER_TIMER		0x02
#define TRIGGER_APPEAR		0x03
#define TRIGGER_DEATH		0x04
#define TRIGGER_SLAPPED		0x05
#define TRIGGER_SEE		0x06
#define TRIGGER_JAILED		0x07
#define TRIGGER_TORTURED	0x08
#define TRIGGER_CONVERTED	0x09
#define TRIGGER_TURNCOAT	0x0A
#define TRIGGER_HUNGRY		0x0B
#define TRIGGER_AFRAID		0x0C
#define TRIGGER_STEAL		0x0D
#define TRIGGER_GOAWAY		0x0E
#define TRIGGER_STUNNED		0x10
#define TRIGGER_DOORDESTROYED	0x11
#define TRIGGER_OBJECTCLAIMED	0x12
#define TRIGGER_CREATURES	0x13
#define TRIGGER_HAPPYCREATURES	0x14
#define TRIGGER_ANGRYCREATURES	0x15
#define TRIGGER_LOSTCREATURES	0x16
#define TRIGGER_KILLEDCREATURES	0x17
#define TRIGGER_ROOMS		0x18
#define TRIGGER_ROOMSTYPES	0x19
#define TRIGGER_ROOMSSIZE	0x1A
#define TRIGGER_DOORS		0x1B
#define TRIGGER_TRAPS		0x1C
#define TRIGGER_SPELLS		0x1D
#define TRIGGER_GOLDAMOUNT	0x1E
#define TRIGGER_GOLDMINED	0x1F
#define TRIGGER_MANAAMOUNT	0x20
#define TRIGGER_DESTROYED	0x21
#define TRIGGER_TIMEELAPSED	0x22
#define TRIGGER_CRGLOBALLEVEL	0x23		// Not usable yet (Global)
#define TRIGGER_HURTED		0x24
#define TRIGGER_GOLDOWNED	0x25		// Not usable yet (Creature)
#define TRIGGER_CONGREGATE	0x26		// Not usable yet (???)
#define TRIGGER_CREAENTERAREA	0x27
#define TRIGGER_CLAIMAREA	0x28
#define TRIGGER_CLAIMENTIREAREA	0x29
#define TRIGGER_AREACONTENT	0x2A
#define TRIGGER_BANDCREATED	0x2B
#define TRIGGER_DEATHSINBAND	0x2C
#define TRIGGER_JAILEDINBAND	0x2D
#define TRIGGER_GOLDDAY		0x2E
#define TRIGGER_HEARTDESTROYED	0x2F
#define TRIGGER_GEMINHAND	0x30		// Not usable yet (Creature)
#define TRIGGER_CREATUREEXP	0x31
#define TRIGGER_CREATURESLEVEL	0x34
#define TRIGGER_CLICKELEMENT	0x35
#define TRIGGER_NOTHUNGRY	0x36
#define TRIGGER_TAKESGEM	0x37		// Not usable yet (Creature)
#define TRIGGER_BREACHED	0x38
#define TRIGGER_ENNEMYBREACHED	0x39
#define TRIGGER_TAKECREATURE	0x3A
#define TRIGGER_FREECREATURE	0x3B
#define TRIGGER_HURTCREATURE	0x3C
#define TRIGGER_FIRECREATURE	0x3D
#define TRIGGER_SELECTAREA	0x3E
#define TRIGGER_FIRED		0x3F
#define TRIGGER_STUNNEDINBAND	0x40
#define TRIGGER_TAKE		0x41
#define TRIGGER_LEVELPLAYED	0x43		// Not usable yet (Global)
#define TRIGGER_ROOMSCONTENT	0x44
#define TRIGGER_SELECTALLAREA	0x45
#define TRIGGER_ENTERAREAINPOSS	0x46
#define TRIGGER_SLAPCOUNT	0x47
#define TRIGGER_TRANSITIONENDS	0x48
#define TRIGGER_GROUPINPOSSESS	0x49

#define TRIGGER_IDENTIFIERLIMIT	0x50

typedef struct TRIGGER {
	NODE		 node;
	short		 previd;
	short		 expanded;
	long		 param1;
	long		 param2;
	long		 param3;
	long		 param4;
	long		 param5;
	long		 param6;
	long		 param7;
	long		 param8;
	short		 id;
	short		 or;
	short		 and;
	unsigned char	 ctrl;
	unsigned char	 repeat;
} TRIGGER;

/*** Définition des actions *********************************************/

#define ACTION_ADDCREATURE	0x01
#define ACTION_SHOWCRYSTAL	0x02
#define ACTION_SENDTOAREA	0x03		// Not usable yet (???)
#define ACTION_SHOWOBJECTIVE	0x04		// Not usable yet (Player)
#define ACTION_SHOWINFORMATION	0x05		// Not usable yet (Player)
#define ACTION_ADDELEMENT	0x06
#define ACTION_SETVARIABLE	0x07
#define ACTION_SETTIMER		0x08
#define ACTION_SHOWELEMENT	0x09
#define ACTION_VICTORY		0x0A
#define ACTION_DEFEAT		0x0B
#define ACTION_RELEASEBAND	0x0E
#define ACTION_CHANGEPARAMS	0x0F
#define ACTION_SHOWAREA		0x10
#define ACTION_ENABLEFOG	0x11
#define ACTION_ALLIES		0x12
#define ACTION_HASCRYSTAL	0x14
#define ACTION_POSSESSCREATURE	0x15
#define ACTION_CHANGESQUARE	0x16
#define ACTION_DISPLAYTIMER	0x17
#define ACTION_SAY		0x18
#define ACTION_DISPLAYTEXT	0x19		// Not usable yet (Player)
#define ACTION_MOVECAMERA	0x1A
#define ACTION_ROTATECAMERA	0x1B
#define ACTION_GENERATE		0x1C
#define ACTION_MAKEHUNGRY	0x1D
#define ACTION_SHOWHEALTH	0x1E
#define ACTION_CAMERAPATH	0x1F
#define ACTION_DESTROYGATE	0x20
#define ACTION_RUN		0x21
#define ACTION_MUSICLEVEL	0x22
#define ACTION_REMOVE		0x23
#define ACTION_ATTACK		0x24
#define ACTION_PORTALS		0x25
#define ACTION_CHANGEVIEW	0x26
#define ACTION_PRIMARYTARGET	0x2A
#define ACTION_SHOW		0x2B
#define ACTION_MOODS		0x2C
#define ACTION_SYSTEMMESSAGES	0x2D
#define ACTION_SHOWNEXTROOM	0x2F
#define ACTION_TOGGLEEFFECT	0x30
#define ACTION_CHANGEOWNER	0x31
#define ACTION_SLAPSREMAINING	0x32
#define ACTION_TIMERSPEECH	0x33

#define ACTION_IDENTIFIERLIMIT	0x34

typedef struct ACTION {
	NODE		 node;
	short		 previd;
	short		 pad;
	long		 param1;
	long		 param2;
	long		 param3;
	long		 param4;
	long		 param5;
	long		 param6;
	long		 param7;
	long		 param8;
	short		 id;
	short		 next;
	short		 unused;
	unsigned char	 ctrl;
	unsigned char	 one;
} ACTION;

/*** Structures des routines de modification ****************************/

typedef struct TRIGGERSPTCTX {
	HWND		 KeepersList;
	HWND		 CreaturesList;
	HWND		 DoorsList;
	HWND		 ObjectsList;
	HWND		 MagicalObjectsList;
	HWND		 AreasList;
	HWND		 BandsList;
	HWND		 CreaturesInBandList;
	TRIGGERLINK	 TempLink;
	TRIGGERLINK	*SrceLink;
	BOOL		 Initialized[8];
	int		 Selected[8];
} TRIGGERSPTCTX;

typedef struct TRIGGERSTRCTX {
	HWND		 TriggersList;
	HWND		 CreaturesList;
	HWND		 HappyCreaturesList;
	HWND		 AngryCreaturesList;
	HWND		 LostPlayersList;
	HWND		 KilledPlayersList;
	HWND		 RoomsList;
	HWND		 RoomsTypeList;
	HWND		 RoomsSizeList;
	HWND		 RoomsContentList;
	HWND		 DoorsList;
	HWND		 TrapsList;
	HWND		 DestroyedList;
	HWND		 SpellsList;
	HWND		 ButtonsList;
	HWND		 HeartList;
	HWND		 CreateHerosList;
	HWND		 JailHerosList;
	HWND		 DeadHerosList;
	HWND		 StunHerosList;
	HWND		 AreaPossessPlayersList;
	HWND		 AreaCreaturesList;
	HWND		 AreaPlayersList;
	HWND		 AreaClaimPlayersList;
	HWND		 AreaClaimEntirePlayersList;
	HWND		 AreaContentList;
	HWND		 AreaSelectPlayersList;
	HWND		 AreaSelectAllPlayersList;
	BOOL		 MainSet;
	TRIGGERLINK	*ParentPoint;
	TRIGGER		 TempTrigger;
	TRIGGER		*SrceTrigger;
	BOOL		 Initialized[TRIGGER_IDENTIFIERLIMIT];
	char		 TempNumber[128];
} TRIGGERSTRCTX;

typedef struct TRIGGERSACCTX {
	HWND		 ActionsList;
	HWND		 CreaturesList;
	HWND		 ElementsList;
	HWND		 HerosList;
	HWND		 TerrainList;
	HWND		 ButtonsList;
	HWND		 RoomsList;
	HWND		 PlayersList;
	HWND		 OwnersList;
	HWND		 FirstAlliePlayersList;
	HWND		 SecondAlliePlayersList;
	HWND		 GenCreaturesList;
	BOOL		 MainSet;
	NODE		 SpeechList;
	TRIGGERLINK	*ParentPoint;
	ACTION		 TempAction;
	ACTION		*SrceAction;
	BOOL		 ChangeRect;
	MAPRECT		 FullRect;
	BOOL		 Initialized[ACTION_IDENTIFIERLIMIT];
} TRIGGERSACCTX;

typedef struct TRIGGERSCTX {
	NODE		*Triggers;
	NODE		*Actions;
	HWND		 Points;
	HWND		 List;
	HWND		 Variables;
	HWND		 Timers;
	HIMAGELIST	 IconList;
	HIMAGELIST	 ImageList;
	HICON		 CameraIcon;
	HICON		 PointIcon;
	HICON		 RectIcon;
	HICON		 UpIcon;
	HICON		 DownIcon;
	HICON		 LeftIcon;
	HICON		 RightIcon;
	BOOL		 Root;
	BOOL		 Modify;
	BOOL		 Insert;
	HIMAGELIST	 Images;
	TRIGGERSPTCTX	 PointCtx;
	TRIGGERSTRCTX	 TriggerCtx;
	TRIGGERSACCTX	 ActionCtx;
} TRIGGERSCTX;

