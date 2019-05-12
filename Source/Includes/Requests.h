
/*** Drapeaux des requêtes **********************************************/

#define REQUEST_COPY		0x00000001
#define REQUEST_MNGRKILL	0x00000002
#define REQUEST_BUFOVERFLOW	0x00000004
#define REQUEST_UNDOERROR	0x00000008
#define REQUEST_RECTEXKILL	0x00000010
#define REQUEST_BIGMAP		0x00000020
#define REQUEST_MAPGOLD		0x00000040
#define REQUEST_AREASIZE	0x00000080
#define REQUEST_TRIGGERS	0x00000100
#define REQUEST_KILLTRIGGER	0x00000200
#define REQUEST_KILLACTION	0x00000400
#define REQUEST_KILLPOINT	0x00000800
#define REQUEST_RECTEXKILLPATH	0x00001000
#define REQUEST_RECTEXMODPATH	0x00002000
#define REQUEST_GATESADDPB	0x00004000
#define REQUEST_GATESADD	0x00008000
#define REQUEST_GLOBALJOB	0x00010000
#define REQUEST_MNGRUNDOERR	0x00020000

/*** Données de MessageBoxExt() *****************************************/

#define MESSAGEBOXBUFFER	8192
#define MBF_CENTERTEXT		0x00000001
#define MBF_ESNUMBER		0x00000002
#define MBF_NOTEXT		0x00000100
#define MBF_NOGADGETS		0x00000200
#define MBF_NOTITLE		0x00000400
#define MBF_NOICON		0x00000800
#define MBF_NOMINMAX		0x00001000
#define MBF_NOBEEP		0x00002000
#define MBF_AUTOREPLY		0x00010000
#define MBF_EDITTEXT		0x00020000
#define MBF_NOOVERFLOWREQ	0x01000000
#define MBF_NOOUTOFRANGEREQ	0x02000000
#define MBF_NUMBER		0x10000000
#define MBF_USEFONT		0x20000000
#define MBF_USEICON		0x40000000
#define MBF_DESTROYICON		0x80000000
#define MBP_CENTER		0x00000000
#define MBP_FOLLOWMOUSE		0x00000001
#define MBP_CUSTOM		0x80000000
#define MBJ_MIDDLE		0x00000000
#define MBJ_LEFT		0x00000001
#define MBJ_RIGHT		0x00000002

/*** Structures des requêtes ********************************************/

typedef struct MESSAGEBOXSTRUCT {
	char		 *gadgets;
	char		 *windowtext;
	char		 *windowtitle;
	char		 *editbuffer;
	LONG		  editbuffersize;
	LONG		  editnumber;
	DWORD		 *requestbits;
	DWORD		  statebit;
	DWORD		  flags;
	UINT		  justification;
	UINT		  defaultgadget;
	UINT		  defaulticon;
	LONG		  min;
	LONG		  max;
	LONG		  numberwidth;
	LONG		  editwidth;
	char		  fontname[LF_FACESIZE];
	LONG		  fontsize;
	UINT		  windowpos;
	LONG		  x;
	LONG		  y;
	HICON		  windowicon;
	DWORD		  internalflags;	// <Private>
	LONG		  numgadgets;		// <Private>
	char		 *textfont;		// <Private>
	LONG		  textfontsize;		// <Private>
} MESSAGEBOXSTRUCT;


