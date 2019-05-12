
/*** Structure des menus ************************************************/

typedef struct CUSTOMITEM {
	BOOL		 ciNext;
	UINT		 ciId;
	UINT		 ciIconId;
	HICON		 ciIcon;
	HBITMAP		 ciImage;
	char		*ciText;
	ULONG		 ciTextLen;
	char		*ciKey;
	ULONG		 ciFlags;
} CUSTOMITEM;

typedef struct FLAGARRAY {
	UINT		 id;
	DWORD		 flag;
} FLAGARRAY;

