
/*** Identificateurs ****************************************************/

#define REPORT_COMPATIBILITY	0
#define REPORT_LOADERROR	1

#define CSR_VERSION		0x00000001
#define CSR_WARNINGS		0x00000002
#define CSR_ALERTS		0x00000004
#define CSR_INFOS		0x00000008

#define FILE_MAP		0
#define FILE_AREA		1
#define FILE_BRUSH		2
#define FILE_THUMBNAIL		3

#define FILEHEADER_AREA		EXTB('A','R','E','A')
#define FILEHEADER_STATS	EXTB('K','S','T','S')
#define FILEHEADER_MAP		EXTB('K','M','A','P')
#define FILEHEADER_THINGS	EXTB('K','T','G','S')
#define FILEHEADER_MAPFILE	EXTB('M','A','P','F')
#define FILEHEADER_MAPINFO	EXTB('K','I','N','F')
#define FILEHEADER_MAPPLAYERS	EXTB('K','P','L','Y')
#define FILEHEADER_MAPRECTEX	EXTB('K','R','E','C')
#define FILEHEADER_HEROS	EXTB('K','M','T','H')
#define FILEHEADER_TRIGGERS	EXTB('K','T','R','G')
#define FILEHEADER_VARIABLES	EXTB('K','G','B','L')

#define DATATYPE_BINARY		0
#define DATATYPE_NODE		1
#define DATATYPE_TEXT		2
#define DATATYPE_TEXTNUMBER	3

#define MAPFILE_GAME		0
#define MAPFILE_TEXT		1
#define MAPFILE_EDITOR		2
#define MAPFILE_TEMP		3
#define MAPFILE_UNKNOWN		(UINT)-1

/*** Structures *********************************************************/

typedef struct HEADER {
	DWORD		 Header;
	LONG		 Version;
	LONG		 Size;
} HEADER;

typedef struct FILEHEADER {
	UINT		 type;
	LONG		 width;
	LONG		 height;
	HBITMAP		 bitmap;
} FILEHEADER;

