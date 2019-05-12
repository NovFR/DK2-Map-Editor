
/*** Tailles de certaines données ***************************************/

#define THING_MAXHEROSINBAND	16
#define THING_MAXBANDNAME	64

/*** Structures *********************************************************/

typedef struct MAPTHINGHERO {
	BOOL		 isvalid;
	LONG		 level;
	UINT		 enemy;
	UINT		 id;
	UINT		 health;
	UINT		 goldheld;
	UINT		 options;
	UINT		 objective;
	UINT		 options2;
	UINT		 area;
	ULONG		 uniqueid;
} MAPTHINGHERO;

typedef struct MAPTHINGHEROSBAND {
	NODE		 node;
	MAPTHINGHERO	 heros[THING_MAXHEROSINBAND+1];		// +1 for endmark
	UINT		 id;
	ULONG		 uniqueid;
	char		 name[THING_MAXBANDNAME];
} MAPTHINGHEROSBAND,MTHBAND;

typedef struct MAPTHINGHEROSBANDCTX {
	HWND		 hlist;
	HIMAGELIST	 himages;
	HTREEITEM	 hselected;
	NODE		*band;
} MAPTHINGHEROSBANDCTX,MTHCTX;

typedef struct MAPTHINGHEROSBANDMODCTX {
	MTHCTX		*context;
	MTHBAND		 band;
	HWND		 tvlist;
	HWND		 lvlist;
	UINT		 mode;
	DWORD		 options;
	DWORD		 options2;
	UINT		 objective;
	UINT		 area;
	UINT		 ennemy;
	HTREEITEM	 tvselected;
} MAPTHINGHEROSBANDMODCTX;

