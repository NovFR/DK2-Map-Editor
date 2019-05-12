
//<<>-<>>---------------------------------------------------------------------()
/*
	Chargement de la carte.
	Format supportés : MAPFILE_EDITOR, MAPFILE_GAME & MAPFILE_TEXT
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"

extern ULONG		AutoSaveTimer;
extern DWORD		TickCount;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapChanges;
extern UINT		MapFileType;
extern MAPRECT		MapCursorSel;
extern NODE		RecentFiles;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de chargement						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Chargement d'une carte déjà chargée auparavant ««««««««««««««««««»

void Files_LoadRecentMap(HWND hWnd, UINT FileID)
{
	RECENTFILE	*File;

	for (File = (RECENTFILE *)RecentFiles.next; File != 0; File = (RECENTFILE *)File->Node.next)
		if (!FileID--) break;

	if (!File) return;

	Files_LoadMap(hWnd,File->Name,TRUE);
	return;
}


// «»»» Chargement de la sauvegarde automatique «««««««««««««««««««««««««»

void Files_LoadSafeMap(HWND hWnd)
{
	HANDLE	 FileHandle;
	char	*FileName;

	FileName = Config_GetUserFileName(szTemporaryFolder,szTemporaryFile,FALSE);
	if (!FileName) return;

	FileHandle = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		if ((GetLastError() == ERROR_FILE_NOT_FOUND)||(GetLastError() == ERROR_PATH_NOT_FOUND))
			{
			Misc_MessageBoxExt(hWnd,szNoTempMapErr,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
			GlobalFree(FileName);
			return;
			}
		}
	else
		CloseHandle(FileHandle);

	Files_LoadMap(hWnd,FileName,FALSE);
	GlobalFree(FileName);
	MapFileType = 0;
	ZeroMemory(&Config.MapPath,MAX_PATH);
	return;
}


// «»»» Chargement d'une carte ««««««««««««««««««««««««««««««««««««««««««»

void Files_LoadMap(HWND hWnd, char *MapName, BOOL UpdateRecent)
{
	NODE	 MapErrors;
	UINT	 MapDiskType;
	char	*MapFileName;
	int	 MapType;

	if (!Misc_CheckChanges(hWnd,IDM_OPEN)) return;

	MapFileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapFileName) return;

	ZeroMemory(&MapErrors,sizeof(NODE));
	if (!MapName)
		{
		strcpy(MapFileName,Config.MapPath);
		MapType = Misc_FileSelect(hWnd,MapFileName,FILE_MAP);
		}
	else
		{
		MapType = 1;
		strcpy(MapFileName,MapName);
		}

	switch(MapType)
		{
		case 0:	GlobalFree(MapFileName);
			return;
		case 1:	MapDiskType = Files_GetMapTypeFromName(MapFileName);
			break;
		case 2:	MapDiskType = MAPFILE_EDITOR;
			break;
		case 3:	MapDiskType = MAPFILE_GAME;
			break;
		case 4:	MapDiskType = MAPFILE_TEXT;
			break;
		}

	Map_Lock(TRUE,TRUE);
	MapType = 0;

	switch(MapDiskType)
		{
		case MAPFILE_GAME:
			MapType = Game_ExtractAll(hWnd,MapFileName,&MapErrors);
			break;
		case MAPFILE_EDITOR:
			MapType = Editor_ExtractAll(hWnd,MapFileName,&MapErrors);
			break;
		case MAPFILE_TEXT:
			MapType = Text_ExtractAll(hWnd,MapFileName,&MapErrors);
			break;
		}

	if (MapType)
		{
		if (MapDiskType == MAPFILE_GAME)
			{
			MapFileType = 0;
			ZeroMemory(&Config.MapPath,MAX_PATH);
			}
		else
			{
			MapFileType = MapDiskType;
			strcpy(Config.MapPath,MapFileName);
			}
		AutoSaveTimer = 0;
		MapChanges = 0;
		MapStartX = 0;
		MapStartY = 0;
		MapCursorSel.hidden = TRUE;
		TickCount = GetTickCount();
		Map_CorrectAfterResize();
		Mouse_MovedUpdate(hWnd);
		Map_SetPropThumb(hWnd,PROP_VERTICAL);
		Map_SetPropThumb(hWnd,PROP_HORIZONTAL);
		Minimap_Build(hWnd);
		Thumb_UpdateWindow();
		Files_SaveUpdate();
		Map_NewTitle(hWnd);
		if (UpdateRecent)
			{
			Config_AddRecent(MapFileName);
			Config_UpdateRecentMenu();
			Registry_WriteRecentFiles(hWnd);
			}
		}

	Map_Lock(FALSE,TRUE);
	Compiler_Report(hWnd,REPORT_LOADERROR,0,&MapErrors);
	List_ReleaseMemory(&MapErrors);
	Info_Update();

	GlobalFree(MapFileName);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
