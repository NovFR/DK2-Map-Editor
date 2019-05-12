
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion du fichier de configuration principal.
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

extern HWND		hWnd;
extern HMENU		hMenu;
extern CONFIG		Config;
extern NODE		RecentFiles;
extern LONG		RecentItems;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Valeurs par défaut de la configuration ««««««««««««««««««««««««««»

void Config_SetToDefault()
{
	ZeroMemory(&Config,sizeof(Config));

	Config.HideSplash = FALSE;
	Config.HideToolbar = FALSE;
	Config.HideMinimap = FALSE;
	Config.UseFilename = FALSE;
	Config.ShowFullName = FALSE;
	Config.PlayerUsesShell = TRUE;
	Config.MinimapSize = MINIMAP_DEFAULTSIZE;
	Config.MinimapKeepProps = TRUE;
	Config.DialogPosition = MBP_FOLLOWMOUSE;
	Config.RequestsPosition = MBP_CENTER;
	Config.AutoSaveTime = 5;
	Config.ZoomCX = 64;
	Config.ZoomCY = 64;
	Config.CompilerWindowX = CW_USEDEFAULT;
	Config.CompilerWindowY = CW_USEDEFAULT;
	Config.CompilerWindowWidth = 800;
	Config.CompilerWindowHeight = 600;
	Config.ThumbnailWindowX = CW_USEDEFAULT;
	Config.ThumbnailWindowY = CW_USEDEFAULT;
	Config.UndoLevels = UNDO_LEVELS;
	Config.SoundsVolume = -500;
	Config.SoundsFlags = SDF_INTERFACE|SDF_TERRAIN;
	Config.MapViewFlags = MVF_TERRAIN|MVF_CREATURES|MVF_DOORS|MVF_TRAPS|MVF_OBJECTS|MVF_MAGICALOBJECTS|MVF_GATES|MVF_AREAS|MVF_CAMERAS|MVF_EFFECTS|MVF_PATHS|MVF_DETAILED;
	Config.SaveReportFlags = CSR_WARNINGS|CSR_ALERTS|CSR_INFOS;
	Config.PathsColor = 0x00FFFFFF;
	Config.ManagerSortMethod = MNGR_SORTBYPOSITION;
	Config.ManagerModifyParams.ModifyFlags = MNGR_FLAGIGNOREINCOMP|MNGR_FLAGASKOPTIONS|MNGR_FLAGPREVIEW|MNGR_FLAGIGNOREASK|MNGR_MODIFYTYPE|MNGR_MODIFYOWNER|MNGR_MODIFYLEVEL|MNGR_MODIFYGOLD|MNGR_MODIFYHEALTH|MNGR_MODIFYCROPTS|MNGR_MODIFYHEROESOPTS|MNGR_MODIFYOBJECTIVE|MNGR_MODIFYDATA;
	Config.ManagerModifyParams.ModifyCoordsType = MNGR_COORDRELATIVE;
	Config.ManagerModifyParams.ModifyRemplaceType = MNGR_REMPLACEASK;
	Config.ManagerModifyParams.ModifyCrOptionsType = MNGR_MODIFYALL;
	Config.ManagerModifyParams.ModifyCrOptions = 0xFFFFFFFF;
	Config.ManagerModifyParams.ModifyHrOptionsType = MNGR_MODIFYALL;
	Config.ManagerModifyParams.ModifyHrOptions = 0xFFFFFFFF;
	Config.ManagerAddParams.AddFlags = MNGR_FLAGASKOPTIONS|MNGR_FLAGPREVIEW|MNGR_FLAGIGNOREASK;
	Config.ManagerAddParams.AddMode = MNGR_ADDONE;
	Config.ManagerAddParams.AddRemplaceType = MNGR_REMPLACEASK;
	Config.ManagerAddParams.AddRandomFreq = 2;
	Config.RandomFreq = 2;
	Config.RecentFiles = 10;
	Config.Thumbnail.KeepOldBitmap = TRUE;
	Config.Thumbnail.WorldFlags = 0xFFFFFFFF;
	Config.Thumbnail.PlayersFlags = 0xFFFFFFFF;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fichiers récents							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Suppression de tous les fichiers ««««««««««««««««««««««««««««««««»

void Config_RemoveAll(HWND hWnd)
{
	MENUITEMINFO	ItemInfo;
	UINT		ItemID;

	ItemInfo.cbSize = sizeof(MENUITEMINFO);
	ItemInfo.fMask = MIIM_SUBMENU;
	ItemInfo.hSubMenu = NULL;
	GetMenuItemInfo(hMenu,IDM_RECENT,FALSE,&ItemInfo);
	if (!ItemInfo.hSubMenu) return;

	while(1)
		{
		ItemID = GetMenuItemID(ItemInfo.hSubMenu,0);
		if (ItemID < IDM_RECENTITEMSFIRST) break;
		if (ItemID > IDM_RECENTITEMSMAX) break;
		RemoveMenu(ItemInfo.hSubMenu,0,MF_BYPOSITION);
		RecentItems = 0;
		}

	List_ReleaseMemory(&RecentFiles);
	Config_UpdateRecentMenu();
	Registry_WriteRecentFiles(hWnd);

	Misc_MessageBoxExt(hWnd,szRecentRemovedAll,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
	return;
}


// «»»» Suppression de tous les fichiers n'existant pas «««««««««««««««««»

void Config_RemoveObsolete(HWND hWnd)
{
	MENUITEMINFO	 ItemInfo;
	RECENTFILE	*File;
	LONG		 Count;
	UINT		 ItemID;
	HANDLE		 FileHandle;

	ItemInfo.cbSize = sizeof(MENUITEMINFO);
	ItemInfo.fMask = MIIM_SUBMENU;
	ItemInfo.hSubMenu = NULL;
	GetMenuItemInfo(hMenu,IDM_RECENT,FALSE,&ItemInfo);
	if (!ItemInfo.hSubMenu) return;

	while(1)
		{
		ItemID = GetMenuItemID(ItemInfo.hSubMenu,0);
		if (ItemID < IDM_RECENTITEMSFIRST) break;
		if (ItemID > IDM_RECENTITEMSMAX) break;
		RemoveMenu(ItemInfo.hSubMenu,0,MF_BYPOSITION);
		RecentItems = 0;
		}

	for (Count = 0, File = (RECENTFILE *)RecentFiles.next; File != 0; File = (RECENTFILE *)File->Node.next)
		{
		FileHandle = CreateFile(File->Name,0,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if (FileHandle == INVALID_HANDLE_VALUE)
			{
			Count++;
			List_RemEntry((NODE *)File);
			GlobalFree(File);
			File = (RECENTFILE *)RecentFiles.next;
			}
		else
			CloseHandle(FileHandle);
		}

	Config_UpdateRecentMenu();
	Registry_WriteRecentFiles(hWnd);

	if (!Count) Misc_MessageBoxExt(hWnd,szRecentNoObsolete,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
	else Misc_MessageBoxExt(hWnd,szRecentRemoved,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
	return;
}


// «»»» Mise à jour du menu «««««««««««««««««««««««««««««««««««««««««««««»

void Config_UpdateRecentMenu()
{
	MENUITEMINFO	 ItemInfo;
	RECENTFILE	*File;
	LONG		 Count;
	char		*Text;
	char		*Separator;

	ItemInfo.cbSize = sizeof(MENUITEMINFO);
	ItemInfo.fMask = MIIM_SUBMENU;
	ItemInfo.hSubMenu = NULL;
	GetMenuItemInfo(hMenu,IDM_RECENT,FALSE,&ItemInfo);
	if (!ItemInfo.hSubMenu) return;

	Text = GlobalAlloc(GPTR,MAX_PATH);
	if (!Text) return;

	for (Count = 0, File = (RECENTFILE *)RecentFiles.next; File != 0; File = (RECENTFILE *)File->Node.next, Count++)
		{
		if (Config.RecentPaths)
			strcpy(Text,File->Name);
		else
			{
			Separator = strrchr(File->Name,'\\');
			if (Separator) strcpy(Text,++Separator);
			else strcpy(Text,File->Name);
			}

		if (Count < RecentItems)
			{
			ModifyMenu(ItemInfo.hSubMenu,Count,MF_BYPOSITION,IDM_RECENTITEMSFIRST+Count,Text);
			continue;
			}
		InsertMenu(ItemInfo.hSubMenu,Count,MF_BYPOSITION,IDM_RECENTITEMSFIRST+Count,Text);
		RecentItems++;
		}

	GlobalFree(Text);
	return;
}


// «»»» Ajout d'un nouveau fichier ««««««««««««««««««««««««««««««««««««««»

void Config_AddRecent(char *Name)
{
	RECENTFILE	*File;
	LONG		 Count;

	for (Count = 0, File = (RECENTFILE *)RecentFiles.next; File != 0; File = (RECENTFILE *)File->Node.next, Count++)
		{
		if (!strcmpi(Name,File->Name))
			{
			List_RemEntry((NODE *)File);
			List_InsertEntry((NODE *)File,&RecentFiles,TRUE);
			return;
			}
		}

	if (Count >= Config.RecentFiles)
		{
		for (File = (RECENTFILE *)RecentFiles.next; File->Node.next != 0; File = (RECENTFILE *)File->Node.next);
		List_RemEntry((NODE *)File);
		GlobalFree(File);
		}

	File = GlobalAlloc(GPTR,sizeof(RECENTFILE));
	if (!File) return;

	strcpy(File->Name,Name);
	List_InsertEntry((NODE *)File,&RecentFiles,TRUE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Répertoires							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'un répertoire temporaire «««««««««««««««««««««««««««««»

char* Config_GetUserFileName(char *Folder, char *FileName, BOOL CreateFolder)
{
	LPITEMIDLIST	 Path;
	LPMALLOC	 ShellAlloc;
	LPSHELLFOLDER	 Desktop;
	STRRET		 Name;
	char		*PersonalFolder;

	PersonalFolder = GlobalAlloc(GPTR,MAX_PATH);
	if (!PersonalFolder) return(NULL);

	SHGetSpecialFolderLocation(hWnd,CSIDL_PERSONAL,&Path);
	SHGetDesktopFolder(&Desktop);
	Desktop->lpVtbl->GetDisplayNameOf(Desktop,Path,SHGDN_FORPARSING,&Name);
	Config_GetDisplayName(Path,&Name,PersonalFolder);
	Desktop->lpVtbl->Release(Desktop);
	SHGetMalloc(&ShellAlloc);
	ShellAlloc->lpVtbl->Free(ShellAlloc,Path);
	ShellAlloc->lpVtbl->Release(ShellAlloc);

	strcat(PersonalFolder,szPath);
	strcat(PersonalFolder,Folder);
	if (CreateFolder)
		{
		BOOL	Result;

		Result = CreateDirectory(PersonalFolder,NULL);
		if ((!Result)&&(GetLastError() != ERROR_ALREADY_EXISTS))
			{
			Misc_PrintError(hWnd,szTempErr,NULL,MB_ICONHAND);
			GlobalFree(PersonalFolder);
			return(NULL);
			}
		}
	strcat(PersonalFolder,szPath);
	strcat(PersonalFolder,FileName);
	return(PersonalFolder);
}


// «»»» Obtention du répertoire du jeu ««««««««««««««««««««««««««««««««««»

int Config_GetGamePath()
{
	if (!Config.GamePath[0])
		{
		LPMALLOC	ShellAlloc;
		LPSHELLFOLDER	Desktop;
		LPITEMIDLIST	Path;
		BROWSEINFO	Browser;
		STRRET		Name;

		Browser.hwndOwner = hWnd;
		Browser.pidlRoot = NULL;
		Browser.pszDisplayName = NULL;
		Browser.lpszTitle = szGetGamePath;
		Browser.ulFlags = BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT;
		Browser.lpfn = (BFFCALLBACK)Config_GamePathBrowser;
		Browser.lParam = NULL;
		Browser.iImage = NULL;

		Path = SHBrowseForFolder(&Browser);
		if (!Path) return(0);

		SHGetDesktopFolder(&Desktop);
		Desktop->lpVtbl->GetDisplayNameOf(Desktop,Path,SHGDN_FORPARSING,&Name);
		Config_GetDisplayName(Path,&Name,Config.GamePath);
		Desktop->lpVtbl->Release(Desktop);
		SHGetMalloc(&ShellAlloc);
		ShellAlloc->lpVtbl->Free(ShellAlloc,Path);
		ShellAlloc->lpVtbl->Release(ShellAlloc);

		Registry_SetGamePath();
		}

	return(1);
}

//--- Traduction d'un nom shell en nom ASCII ---

void Config_GetDisplayName(LPITEMIDLIST Object, STRRET *Name, char *TempName)
{
	LPMALLOC	 ShellAlloc;
	char		*NameBuffer;

	SHGetMalloc(&ShellAlloc);

	switch(Name->uType)
		{
		case STRRET_WSTR:
			NameBuffer = (char *)ShellAlloc->lpVtbl->Alloc(ShellAlloc,MAX_PATH);
			if (NameBuffer != 0)
				{
				wcstombs(NameBuffer,Name->DUMMYUNIONNAME.pOleStr,MAX_PATH);
				strcpy(TempName,NameBuffer);
				ShellAlloc->lpVtbl->Free(ShellAlloc,NameBuffer);
				}
			break;
		case STRRET_OFFSET:
			strcpy(TempName,((char *)Object)+(Name->DUMMYUNIONNAME.uOffset));
			break;
		case STRRET_CSTR:
			strcpy(TempName,Name->DUMMYUNIONNAME.cStr);
			break;
		}

	ShellAlloc->lpVtbl->Release(ShellAlloc);
	return;
}

//--- Vérifie s'il s'agit bien du répertoire du jeu ---

#pragma argsused
int CALLBACK Config_GamePathBrowser(HWND hDlg, UINT uMsgId, LPARAM lParam, LPARAM lpParam)
{
	LPSHELLFOLDER	 Desktop;
	STRRET		 Name;
	HANDLE		 FileHandle;
	char		*FileName;
	char		*Separator;

	switch(uMsgId)
		{
		case BFFM_SELCHANGED:
			FileName = GlobalAlloc(GPTR,MAX_PATH);
			if (!FileName) break;

			SHGetDesktopFolder(&Desktop);
			Desktop->lpVtbl->GetDisplayNameOf(Desktop,(LPITEMIDLIST)lParam,SHGDN_FORPARSING,&Name);
			Config_GetDisplayName((LPITEMIDLIST)lParam,&Name,FileName);

			Separator = strrchr(FileName,'\\');
			if (!Separator)
				{
				Desktop->lpVtbl->Release(Desktop);
				GlobalFree(FileName);
				break;
				}
			if (*(Separator+1)) strcat(FileName,"\\");
			strcat(FileName,szGameExecutable);

			FileHandle = CreateFile(FileName,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
			if (FileHandle == INVALID_HANDLE_VALUE)
				{
				SendMessage(hDlg,BFFM_SETSTATUSTEXT,(WPARAM)0,(LPARAM)szGamePathBadPath);
				SendMessage(hDlg,BFFM_ENABLEOK,(WPARAM)0,(LPARAM)0);
				}
			else
				{
				SendMessage(hDlg,BFFM_SETSTATUSTEXT,(WPARAM)0,(LPARAM)szGamePathGoodPath);
				SendMessage(hDlg,BFFM_ENABLEOK,(WPARAM)0,(LPARAM)1);
				CloseHandle(FileHandle);
				}

			Desktop->lpVtbl->Release(Desktop);
			GlobalFree(FileName);
			break;
		}

	return(0);
}


// «»»» Obtention du player pour jouer les sons «««««««««««««««««««««««««»

int Config_GetPlayerPath(HWND hWnd)
{
	if (Config.PlayerUsesShell)
		return(1);

	if (!Config.PlayerPath[0])
		{
		OPENFILENAME	Request;

		Request.lStructSize	  = sizeof(OPENFILENAME);
		Request.hwndOwner	  = hWnd;

		Request.lpstrFilter	  = szFileReqExecFilter;
		Request.lpstrCustomFilter = NULL;
		Request.nFilterIndex	  = 1;
		Request.lpstrFile	  = Config.PlayerPath;
		Request.nMaxFile	  = MAX_PATH;
		Request.lpstrFileTitle	  = NULL;
		Request.lpstrInitialDir	  = NULL;
		Request.lpstrTitle	  = szFileReqExecTitle;
		Request.Flags		  = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_NONETWORKBUTTON|OFN_READONLY;
		Request.nFileOffset	  = 0;
		Request.nFileExtension	  = 0;
		Request.lpstrDefExt	  = NULL;

		if (!GetOpenFileName(&Request))
			return(0);

		}

	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
