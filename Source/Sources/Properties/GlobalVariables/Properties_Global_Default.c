
//<<>-<>>---------------------------------------------------------------------()
/*
	Valeurs par défaut pour les variables globales
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

extern HINSTANCE	hInstance;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement des valeurs par défaut				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Valeurs par défaut pour les éléments ««««««««««««««««««««««««««««»

int Properties_LoadDefaultElements(HWND hWnd)
{
	DWORD	 LastError;
	char	*Path;
	char	*Filename;

	Path = GlobalAlloc(GPTR,MAX_PATH);
	if (!Path)
		{
		LastError = GetLastError();
		goto Error_0;
		}

	GetModuleFileName(hInstance,Path,MAX_PATH);
	Filename = strrchr(Path,'\\');
	if (Filename) *Filename = 0;

	if (!Properties_LoadDefault(Path, szGlobalDefaultTerrain,      GTOrder,  552, &DefaultTerrain,      &LastError)) goto Error_0;
	if (!Properties_LoadDefault(Path, szGlobalDefaultRooms,	       GROrder, 1055, &DefaultRooms,        &LastError)) goto Error_0;
	if (!Properties_LoadDefault(Path, szGlobalDefaultDoors,	       GDOrder,  616, &DefaultDoors,        &LastError)) goto Error_0;
	if (!Properties_LoadDefault(Path, szGlobalDefaultTraps,	       GAOrder,  579, &DefaultTraps,        &LastError)) goto Error_0;
	if (!Properties_LoadDefault(Path, szGlobalDefaultKeeperSpells, GSOrder,  406, &DefaultKeeperSpells, &LastError)) goto Error_0;

	GlobalFree(Path);
	return(1);

//--- Erreurs ---

Error_0:Properties_FreeDefaultElements();
	Properties_LoadError(hWnd,szGlobalDefaultElemsErr,Path,LastError);
	if (Path) GlobalFree(Path);
	return(0);
}


// «»»» Valeurs par défaut pour les créatures «««««««««««««««««««««««««««»

int Properties_LoadDefaultCreatures(HWND hWnd)
{
	DWORD	 LastError;
	char	*Path;
	char	*Filename;

	Path = GlobalAlloc(GPTR,MAX_PATH);
	if (!Path)
		{
		LastError = GetLastError();
		goto Error_0;
		}

	GetModuleFileName(hInstance,Path,MAX_PATH);
	Filename = strrchr(Path,'\\');
	if (Filename) *Filename = 0;

	if (!Properties_LoadDefault(Path,szGlobalDefaultCreatures,GCOrder,5449,&DefaultCreatures,&LastError))
		goto Error_0;
	if (!Properties_LoadDefault(Path,szGlobalDefaultCreatureSpells,GOOrder,266,&DefaultCreatureSpells,&LastError))
		goto Error_0;

	GlobalFree(Path);
	return(1);

//--- Erreurs ---

Error_0:Properties_FreeDefaultElements();
	Properties_LoadError(hWnd,szGlobalDefaultElemsErr,Path,LastError);
	if (Path) GlobalFree(Path);
	return(0);
}


// «»»» Libère la mémoire utilisée par les éléments «««««««««««««««««««««»

void Properties_FreeDefaultElements()
{
	if (DefaultTerrain)		GlobalFree(DefaultTerrain);
	if (DefaultRooms)		GlobalFree(DefaultRooms);
	if (DefaultDoors)		GlobalFree(DefaultDoors);
	if (DefaultTraps)		GlobalFree(DefaultTraps);
	if (DefaultKeeperSpells)	GlobalFree(DefaultKeeperSpells);

	DefaultTerrain		= NULL;
	DefaultRooms		= NULL;
	DefaultDoors		= NULL;
	DefaultTraps		= NULL;
	DefaultKeeperSpells	= NULL;
	return;
}


// «»»» Libère la mémoire utilisée par les créatures ««««««««««««««««««««»

void Properties_FreeDefaultCreatures()
{
	if (DefaultCreatures)		GlobalFree(DefaultCreatures);
	if (DefaultCreatureSpells)	GlobalFree(DefaultCreatureSpells);

	DefaultCreatures 	= NULL;
	DefaultCreatureSpells	= NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Chargement des valeurs ««««««««««««««««««««««««««««««««««««««««««»

int Properties_LoadDefault(char *Drawer, char *File, DWORD **Table, LONG Size, BYTE **DefaultPtr, DWORD *LastError)
{
	HANDLE	 FileHandle;
	DWORD	 FileSize;
	DWORD	 RedBytes;
	BYTE	*TempPtr;
	char	*Folder;
	int	 i;

	//
	// Création du nom du fichier
	//

	Folder = strchr(Drawer,0);
	if (*(Folder-1) != '\\')
		{
		*Folder = '\\';
		*(Folder+1) = 0;
		}
	strcat(Folder,File);

	//
	// Chargement du fichier complet
	//

	FileHandle = CreateFile(Drawer,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	*LastError = GetLastError();
	if (FileHandle == INVALID_HANDLE_VALUE) goto Error_0;

	FileSize = GetFileSize(FileHandle,NULL);
	*LastError = GetLastError();
	if (FileSize == 0xFFFFFFFF) goto Error_1;

	*DefaultPtr = GlobalAlloc(GPTR,FileSize);
	*LastError = GetLastError();
	if (!*DefaultPtr) goto Error_1;

	ReadFile(FileHandle,*DefaultPtr,FileSize,&RedBytes,NULL);
	*LastError = GetLastError();
	if (RedBytes != FileSize) goto Error_2;

	CloseHandle(FileHandle);

	//
	// Stockage des pointeurs
	//

	for (TempPtr = *DefaultPtr, i = 0; Table[i] != (DWORD *)-1;)
		{
		Table[i+1] = (DWORD *)TempPtr;
		TempPtr += Size;
		i += 2;
		}

	//
	// Terminé
	//

	*Folder = 0;
	return(1);

//--- Erreurs de chargement --

Error_2:GlobalFree(*DefaultPtr);
Error_1:CloseHandle(FileHandle);
Error_0:*DefaultPtr = NULL;
	return(0);
}


// «»»» Affichage d'une erreur ««««««««««««««««««««««««««««««««««««««««««»

void Properties_LoadError(HWND hWnd, char *ErrorText, char *Path, DWORD LastError)
{
	char	*ErrorBuffer;
	char	*ErrorMsg;

	ErrorBuffer = GlobalAlloc(GPTR,2048+MAX_PATH);
	if (!ErrorBuffer)
		{
		Misc_MessageBoxExt(hWnd,ErrorText,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}

	strcpy(ErrorBuffer,ErrorText);
	if (Path)
		{
		strcat(ErrorBuffer,szGlobalDefaultMsgPart1);
		strcat(ErrorBuffer,Path);
		strcat(ErrorBuffer,szGlobalDefaultMsgPart2);
		}
	else
		{
		strcat(ErrorBuffer,szLF);
		}

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,LastError,LOCALE_USER_DEFAULT,(char *)&ErrorMsg,1,NULL))
		{
		Misc_MessageBoxExt(hWnd,ErrorBuffer,NULL,szMessageBoxExtOk,MB_ICONHAND);
		GlobalFree(ErrorBuffer);
		return;
		}

	strcat(ErrorBuffer,szGlobalDefaultMsgPart3);
	strcat(ErrorBuffer,ErrorMsg);
	Misc_MessageBoxExt(hWnd,ErrorBuffer,NULL,szMessageBoxExtOk,MB_ICONHAND);
	LocalFree(ErrorMsg);
	GlobalFree(ErrorBuffer);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
