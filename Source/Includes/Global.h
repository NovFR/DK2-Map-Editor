
/*** Identificateurs ****************************************************/

#define WM_PAINTMAP		WM_USER+1
#define WM_PRINTTEXT		WM_USER+2
#define WM_DISABLE		WM_USER+3
#define WM_KILLTHING		WM_USER+4
#define WM_REPORTERROR		WM_USER+5
#define WM_UPDATEPROGRESS	WM_USER+6
#define WM_REPORTERROREX	WM_USER+7

#define WINDOW_MAIN		0
#define WINDOW_SPLASH		1
#define WINDOW_COMPILER		2
#define WINDOW_DISK		3
#define WINDOW_FULLMAP		4
#define WINDOW_THUMBNAIL	5

#define STATUS_INFOPART		0
#define STATUS_MOUSEPART	1
#define STATUS_TOOLPART		2
#define STATUS_TIMERPART	3
#define STATUS_ZOOMPART		4

#define MOUSE_FREE		0
#define MOUSE_DEFINERECT	1
#define MOUSE_PAINT		2
#define MOUSE_RIGHTRECT		3

#define PROP_VERTICAL		0
#define PROP_HORIZONTAL		1
#define PROP_ZOOMBUTTON		2

/*** Drapeaux ***********************************************************/

#define LIST_BADS		0x00000001
#define LIST_ELITES		0x00000002
#define LIST_HEROES		0x00000004
#define LIST_LPARAMID		0x10000000
#define LIST_LPARAMPTR		0x20000000

/*** Valeurs par défaut *************************************************/

#define LOCALE_MAXLEN		16
#define DISK_TIMEOUT		250
#define MINIMAP_DEFAULTSIZE	144
#define SPLASH_TIMEOUT		500

/*** Macro instructions *************************************************/

#define EXTB(a,b,c,d)		((LONG) ((BYTE)(a) | ((LONG)(((BYTE)(b))<<8)) | ((LONG)(((BYTE)(c))<<16)) | ((LONG)(((BYTE)(d))<<24)) ))

/*** Structures *********************************************************/

typedef struct NODE {
	struct NODE	*next;
	struct NODE	*prev;
	UINT		 type;
} NODE;

typedef struct FONTINFO {
	HFONT		 hFont;
	UINT		 FontHeight;
	UINT		 FontMaxWidth;
	UINT		 FontAveWidth;
} FONTINFO;

typedef struct PLAYERINFO {
	char		*Name;
	COLORREF	 Color;
} PLAYERINFO;

typedef struct ELEMENT {
	UINT		 Id;
	UINT		 Variable;
	UINT		 Icon;
	LONG		 MinWidth;
	LONG		 MinHeight;
	LONG		 MaxWidth;
	LONG		 MaxHeight;
	char		*Name;
	char		*ASCIIName;
} ELEMENT;

typedef struct TREELIST {
	HWND		 hTree;
	DWORD		 Flags;
	DWORD		 Expand;
	DWORD		 Selected;
	HTREEITEM	*SelectedItem;
} TREELIST;
