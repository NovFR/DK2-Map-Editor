
/*** Identificateurs ****************************************************/

#define THING_CREATURE		0
#define THING_DOOR		1
#define THING_TRAP		2
#define THING_OBJECT		3
#define THING_MAGICALOBJECT	4

#define THING_FLAGFROMBAND	0x00000001
#define THING_FLAGLOCKOWNER	0x00000002
#define THING_FLAGPOSITION	0x00000004

#define THING_OPTIONSCREATURES	0
#define THING_OPTIONSHEROS	1
#define THING_OPTIONSBANDS	2
#define THING_OPTIONSADD	3

/*** Structures *********************************************************/

typedef struct MAPTHING {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	UINT		 id;
	UINT		 owner;
	UINT		 health;
	UINT		 goldheld;
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
} MAPTHING;

typedef struct MAPTHINGCTX {
	MAPTHING	 thing;
	UINT		 type;
	DWORD		 flags;
	HWND		 hlist;
	HIMAGELIST	 himages;
	HICON		 hicon;
	union {
	HTREEITEM	 hselected;
	UINT		 iselected;
	} selected;
	HTREEITEM	 bads;
	HTREEITEM	 elites;
	HTREEITEM	 heros;
} MAPTHINGCTX;

typedef struct MAPTHINGOPTIONSCTX {
	DWORD		*CreatureOptions;
	DWORD		*HerosOptions;
	DWORD		 Mode;
} MAPTHINGOPTIONSCTX;

typedef struct MAPTHINGOBJECTIVECTX {
	UINT		*Objective;
	UINT		*Area;
	UINT		*Ennemy;
} MAPTHINGOBJECTIVECTX;

