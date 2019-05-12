
/*** Drapeaux d'affichage ***********************************************/

#define MVF_TERRAIN		0x00000001
#define MVF_CREATURES		0x00000002
#define MVF_DOORS		0x00000004
#define MVF_TRAPS		0x00000008
#define MVF_OBJECTS		0x00000010
#define MVF_MAGICALOBJECTS	0x00000020
#define MVF_AREAS		0x00000040
#define MVF_GATES		0x00000080
#define MVF_PATHS		0x00000100
#define MVF_EFFECTS		0x00000200
#define MVF_CAMERAS		0x00000400
#define MVF_DETAILED		0x00010000
#define MVF_EXPANDGATES		0x00020000
#define MVF_OBJECTUSESPLYCOL	0x00040000
#define MVF_MAGICUSESPLYCOL	0x00080000

/*** Drapeaux d'affichage pour la réduction *****************************/

#define TOF_SHOWLIMIT		0x00000001
#define TOF_SHOWWATER		0x00000002
#define TOF_SHOWLAVA		0x00000004
#define TOF_SHOWGOLD		0x00000008
#define TOF_SHOWGEMS		0x00000010
#define TOF_SHOWSOLID		0x00000020
#define TOF_SHOWROCK		0x00000040
#define TOF_SHOWUNCLAIMED	0x00000080
#define TOF_SHOWHEROESREST	0x00000100

#define TOF_SHOWHEROESWALLS	0x00000010
#define TOF_SHOWHEROESROOMS	0x00000020
#define TOF_SHOWHEROESPORTALS	0x00000040
#define TOF_SHOWHEROESHEARTS	0x00000080
#define TOF_SHOWNEUTRALWALLS	0x00000100
#define TOF_SHOWNEUTRALROOMS	0x00000200
#define TOF_SHOWNEUTRALPORTALS	0x00000400
#define TOF_SHOWNEUTRALHEARTS	0x00000800
#define TOF_SHOWKEEPER1WALLS	0x00001000
#define TOF_SHOWKEEPER1ROOMS	0x00002000
#define TOF_SHOWKEEPER1PORTALS	0x00004000
#define TOF_SHOWKEEPER1HEART	0x00008000
#define TOF_SHOWKEEPER2WALLS	0x00010000
#define TOF_SHOWKEEPER2ROOMS	0x00020000
#define TOF_SHOWKEEPER2PORTALS	0x00040000
#define TOF_SHOWKEEPER2HEART	0x00080000
#define TOF_SHOWKEEPER3WALLS	0x00100000
#define TOF_SHOWKEEPER3ROOMS	0x00200000
#define TOF_SHOWKEEPER3PORTALS	0x00400000
#define TOF_SHOWKEEPER3HEART	0x00800000
#define TOF_SHOWKEEPER4WALLS	0x01000000
#define TOF_SHOWKEEPER4ROOMS	0x02000000
#define TOF_SHOWKEEPER4PORTALS	0x04000000
#define TOF_SHOWKEEPER4HEART	0x08000000
#define TOF_SHOWKEEPER5WALLS	0x10000000
#define TOF_SHOWKEEPER5ROOMS	0x20000000
#define TOF_SHOWKEEPER5PORTALS	0x40000000
#define TOF_SHOWKEEPER5HEART	0x80000000

/*** Drapeaux des sons **************************************************/

#define SDF_INTERFACE		0x00000001
#define SDF_TERRAIN		0x00000002

/*** Configuration ******************************************************/

typedef struct RECENTFILE {
	NODE		 Node;
	char		 Name[MAX_PATH];
} RECENTFILE;

typedef struct THUMBOPTS {
	BOOL		 KeepOldBitmap;
	BOOL		 UseColorKey;
	COLORREF	 HiddenColor;
	LONG		 ActualPlayer;
	DWORD		 WorldFlags;
	DWORD		 PlayersFlags;
} THUMBOPTS;

typedef struct MNGRMODPARAMS {
	DWORD		 ModifyFlags;
	DWORD		 ModifyCoordsType;
	DWORD		 ModifyRemplaceType;
	DWORD		 ModifyCrOptionsType;
	DWORD		 ModifyCrOptions;
	DWORD		 ModifyHrOptionsType;
	DWORD		 ModifyHrOptions;
} MNGRMODPARAMS;

typedef struct MNGRADDPARAMS {
	DWORD		 AddFlags;
	DWORD		 AddMode;
	DWORD		 AddRemplaceType;
	int		 AddRandomFreq;
} MNGRADDPARAMS;

typedef struct CONFIG {
	BOOL		 HideSplash;
	BOOL		 HideToolbar;
	BOOL		 HideMinimap;
	BOOL		 HideDisk;
	BOOL		 UseFilename;
	BOOL		 PlayerUsesShell;
	BOOL		 ShowFullName;
	LONG		 MinimapSize;
	BOOL		 MinimapKeepProps;
	RECT		 MainWindowRect;
	UINT		 MainWindowShow;
	UINT		 DialogPosition;
	UINT		 RequestsPosition;
	LONG		 AutoSaveTime;
	LONG		 ZoomCX;
	LONG		 ZoomCY;
	LONG		 CompilerWindowX;
        LONG		 CompilerWindowY;
	LONG		 CompilerWindowWidth;
	LONG		 CompilerWindowHeight;
	LONG		 ThumbnailWindowX;
	LONG		 ThumbnailWindowY;
	LONG		 UndoLevels;
	LONG		 RecentFiles;
	BOOL		 RecentPaths;
	LONG		 SoundsVolume;
	DWORD		 SoundsFlags;
	DWORD		 MapViewFlags;
	DWORD		 RequestsFlags;
	DWORD		 SaveReportFlags;
	DWORD		 PathsColor;
	DWORD		 ManagerSortMethod;
	MNGRMODPARAMS	 ManagerModifyParams;
	MNGRADDPARAMS	 ManagerAddParams;
	int		 ManagerSubType;
	int		 RandomFreq;
	char		 MapPath[MAX_PATH];
	char		 AreaPath[MAX_PATH];
	char		 GamePath[MAX_PATH];
	char		 PlayerPath[MAX_PATH];
	char		 ThumbPath[MAX_PATH];
	THUMBOPTS	 Thumbnail;
} CONFIG;

