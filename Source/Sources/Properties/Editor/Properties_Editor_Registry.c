
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres de la base de registres
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

extern CONFIG		Config;
extern NODE		RecentFiles;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fichiers récents							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Lecture des fichiers récents ««««««««««««««««««««««««««««««««««««»

void Registry_GetRecentFiles(HWND hWnd)
{
	RECENTFILE	*File;
	HKEY		 Key;
	DWORD		 LastError;
	int		 i;

	LastError = RegOpenKeyEx(HKEY_CURRENT_USER,szRegistryRecentEntry,0,KEY_READ,&Key);
	if (LastError != ERROR_SUCCESS)
		{
		if (LastError == ERROR_FILE_NOT_FOUND) return;
		goto Error_0;
		}

	for (i = 0; i != Config.RecentFiles+1; i++)
		{
		File = GlobalAlloc(GPTR,sizeof(RECENTFILE));
		LastError = GetLastError();
		if (!File) goto Error_1;

		wsprintf(szRegistryRecent,szRegistryRecentTmp,i);
		LastError = MAX_PATH;
		LastError = RegQueryValueEx(Key,szRegistryRecent,NULL,NULL,(BYTE *)&File->Name[0],&LastError);
		if (LastError != ERROR_SUCCESS)
			{
			GlobalFree(File);
			if (LastError == ERROR_FILE_NOT_FOUND) break;
			goto Error_1;
			}
		List_AddEntry((NODE *)File,&RecentFiles);
		}

	RegCloseKey(Key);
	return;

//--- Erreurs ---

Error_1:RegCloseKey(Key);
Error_0:SetLastError(LastError);
	Misc_PrintError(hWnd,szRegistryErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Ecriture des fichiers récents «««««««««««««««««««««««««««««««««««»

void Registry_WriteRecentFiles(HWND hWnd)
{
	RECENTFILE	*File;
	HKEY		 Key;
	DWORD		 Creation;
	DWORD		 LastError;
	int		 i;

	RegDeleteKey(HKEY_CURRENT_USER,szRegistryRecentEntry);

	LastError = RegCreateKeyEx(HKEY_CURRENT_USER,szRegistryRecentEntry,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&Key,&Creation);
	if (LastError != ERROR_SUCCESS) goto Error_0;

	for (i = 0, File = (RECENTFILE *)RecentFiles.next; File != 0; File = (RECENTFILE *)File->Node.next, i++)
		{
		wsprintf(szRegistryRecent,szRegistryRecentTmp,i);
		LastError = RegSetValueEx(Key,szRegistryRecent,0,REG_SZ,(BYTE *)File->Name,strlen(File->Name)+1);
		if (LastError != ERROR_SUCCESS) goto Error_1;
		}

	RegCloseKey(Key);
	return;

//--- Erreurs ---

Error_1:RegCloseKey(Key);
Error_0:SetLastError(LastError);
	Misc_PrintError(hWnd,szRegistryErr,NULL,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sortie du programme						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Modification du drapeau «««««««««««««««««««««««««««««««««««««««««»

void Registry_SetSafeFlag(BOOL Set)
{
	HKEY	Key;
	DWORD	Creation;
	DWORD	LastError;

	LastError = RegCreateKeyEx(HKEY_CURRENT_USER,szRegistryGlobalEntry,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&Key,&Creation);
	if (LastError != ERROR_SUCCESS) return;
	RegSetValueEx(Key,szRegistrySafeFlag,0,REG_DWORD,(BYTE *)&Set,4);
	RegCloseKey(Key);
	return;
}


// «»»» Test le drapeau «««««««««««««««««««««««««««««««««««««««««««««««««»

int Registry_CheckSafeFlag(HWND hWnd)
{
	HKEY	Key;
	DWORD	LastError;
	DWORD	Value;
	DWORD	Size = 4;

	LastError = RegOpenKeyEx(HKEY_CURRENT_USER,szRegistryGlobalEntry,0,KEY_READ,&Key);
	if (LastError != ERROR_SUCCESS)
		{
		if (LastError == ERROR_FILE_NOT_FOUND) return(1);
		goto Error_0;
		}

	LastError = RegQueryValueEx(Key,szRegistrySafeFlag,NULL,NULL,(BYTE *)&Value,&Size);
	RegCloseKey(Key);
	if (LastError != ERROR_SUCCESS)
		{
		if (LastError == ERROR_FILE_NOT_FOUND) return(1);
		goto Error_0;
		}

	if (!Value) return(0);

	return(1);

//--- Erreurs ---

Error_0:SetLastError(LastError);
	Misc_PrintError(hWnd,szRegistryErr,NULL,MB_ICONHAND);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Répertoires							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ecriture du répertoire du jeu «««««««««««««««««««««««««««««««««««»

void Registry_SetGamePath()
{
	HKEY	Key;
	DWORD	Creation;
	DWORD	LastError;

	LastError = RegCreateKeyEx(HKEY_CURRENT_USER,szRegistryGlobalEntry,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&Key,&Creation);
	if (LastError != ERROR_SUCCESS) return;
	RegSetValueEx(Key,szRegistryGamePath,0,REG_SZ,(BYTE *)&Config.GamePath[0],strlen(Config.GamePath)+1);
	RegCloseKey(Key);
	return;
}


// «»»» Lecture du répertoire du jeu ««««««««««««««««««««««««««««««««««««»

void Registry_GetGamePath(HWND hWnd)
{
	HKEY	Key;
	DWORD	LastError;
	DWORD	Size = MAX_PATH;

	LastError = RegOpenKeyEx(HKEY_CURRENT_USER,szRegistryGlobalEntry,0,KEY_READ,&Key);
	if (LastError != ERROR_SUCCESS)
		{
		if (LastError == ERROR_FILE_NOT_FOUND) return;
		goto Error_0;
		}

	LastError = RegQueryValueEx(Key,szRegistryGamePath,NULL,NULL,(BYTE *)&Config.GamePath[0],&Size);
	RegCloseKey(Key);
	if (LastError != ERROR_SUCCESS)
		{
		if (LastError == ERROR_FILE_NOT_FOUND) return;
		goto Error_0;
		}

	return;

//--- Erreurs ---

Error_0:SetLastError(LastError);
	Misc_PrintError(hWnd,szRegistryErr,NULL,MB_ICONHAND);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
