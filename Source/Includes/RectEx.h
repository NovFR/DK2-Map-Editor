
/*** Valeurs par défaut *************************************************/

#define MAP_RECTMAXNAME		64
#define MAP_RECTMAXINFO		128

/*** Identificateurs ****************************************************/

#define RECTEX_AREA		0
#define RECTEX_GATE		1
#define RECTEX_EFFECT		2

#define RECTEX_PATHNEW		0
#define RECTEX_PATHADD		1
#define RECTEX_PATHINS		2
#define RECTEX_PATHKILL		3
#define RECTEX_PATHMODIFY	4

/*** Drapeaux ***********************************************************/

#define RECTEX_AUTOFGDISPLAY	0x00000001
#define RECTEX_AUTOFGDESTROY	0x00000002
#define RECTEX_AUTOFGRELEASE	0x00000004
#define RECTEX_AUTOFADESTROY	0x00000200
#define RECTEX_AUTOFARELEASE	0x00000400
#define RECTEX_AUTOFTRIGGERS	0x00100000
#define RECTEX_AUTOFALL		0x00100607

/*** Structures *********************************************************/

typedef struct MAPRECTEX {
	NODE		 node;
	MAPRECT		 rect;
	UINT		 id;
	//--- Areas only ---
	DWORD		 flags;
	UINT		 nextid;
	LONG		 waitdelay;
	//--- Effects only ---
	LONG		 effect1;
	LONG		 effect2;
	LONG		 effect3;
	LONG		 effect4;
	LONG		 freq;
	//--- Common to all ---
	BYTE		 ext[20];
	char		 name[MAP_RECTMAXNAME];
	char		 info[MAP_RECTMAXINFO];
} MAPRECTEX;

typedef struct MAPRECTEXCTX {
	MAPRECTEX	 rect;
	HWND		 hlist;
	HICON		 hicon;
	BOOL		 modify;
	COLORREF	 defcolor;
	COLORREF	 deftxtcolor;
} MAPRECTEXCTX;

typedef struct MAPRECTEXPATHCTX {
	NODE		 list;
	HWND		 hpathslist;
	HWND		 hareaslist;
	HIMAGELIST	 himagelist;
	UINT		 area;
	LONG		 delay;
	BOOL		 useexisting;
	BOOL		 allowlinked;
	BOOL		 donotupdate;
	UINT		 temp[256*2+2];
	char		 name[2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO];
} MAPRECTEXPATHCTX;

typedef struct MAPRECTEXPATHLIST {
	NODE		 node;
	UINT		 areas[256*2+2];
} MAPRECTEXPATHLIST;

typedef struct MAPRECTEXGATESLIST {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	LONG		 width;
	LONG		 height;
	UINT		 gate;
	UINT		 area;
	BOOL		 hasgate;
	BOOL		 hasarea;
	BOOL		 hasevent;
	BOOL		 cantbedestroyed;
	BOOL		 addgate;
	BOOL		 addarea;
	BOOL		 addevent;
	MAPRECTEX	*gateptr;
	MAPRECTEX	*areaptr;
	TRIGGERLINK	*actionptptr;
	TRIGGER		*triggerptr[MAP_MAXPLAYERS];
	ACTION		*actionptr[MAP_MAXPLAYERS];
} MAPRECTEXGATESLIST;

typedef struct MAPRECTEXGATESCTX {
	NODE		 gates;
	MAPRECTEXCTX	*parentctx;
	MAPRECTEX	 gatestemplate;
	MAPRECTEX	 areastemplate;
	UINT		 templatetype;
	DWORD		 flags;
	DWORD		 tempflags;
	HWND		 hlist;
	HIMAGELIST	 icons;
	LONG		 neededgates;
	LONG		 neededareas;
	BOOL		 fromwindows;
	char		 temp[MAP_RECTMAXINFO*2];
} MAPRECTEXGATESCTX;

