
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines diverses relatives aux fichiers.
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Obtention du type de carte à partir du nom du fichier «««««««««««»

UINT Files_GetMapTypeFromName(char *FileName)
{
	char	 Ext[4];
	char	*Point;

	Point = strrchr(FileName,'.');
	if (!Point) return(MAPFILE_UNKNOWN);
	if (strlen(++Point) > 3) return(MAPFILE_UNKNOWN);
	strcpy(Ext,Point);

	if (!strcmpi(szGameExt,Ext)) return(MAPFILE_GAME);
	if (!strcmpi(szTextExt,Ext)) return(MAPFILE_TEXT);
	if (!strcmpi(szEditorExt,Ext)) return(MAPFILE_EDITOR);

	return(MAPFILE_UNKNOWN);
}


// «»»» Chargement d'un fichier «««««««««««««««««««««««««««««««««««««««««»

void* Files_Load(char *FileName, DWORD *FileSizePtr)
{
	HANDLE	 FileHandle;
	DWORD	 FileSize;
	DWORD	 FileRead;
	void	*FileBuffer;

	Compiler_SetLastError(0);
	FileHandle = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	FileSize = GetFileSize(FileHandle,NULL);
	if (FileSize == 0xFFFFFFFF)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		CloseHandle(FileHandle);
		return(NULL);
		}

	FileBuffer = GlobalAlloc(GMEM_FIXED,FileSize);
	if (!FileBuffer)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		CloseHandle(FileHandle);
		return(NULL);
		}

	ReadFile(FileHandle,FileBuffer,FileSize,&FileRead,NULL);
	if (FileRead != FileSize)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		GlobalFree(FileBuffer);
		CloseHandle(FileHandle);
		return(NULL);
		}

	if (FileSizePtr) *FileSizePtr = FileSize;
	CloseHandle(FileHandle);
	return(FileBuffer);
}


// «»»» Création d'un fichier «««««««««««««««««««««««««««««««««««««««««««»

HANDLE Files_Create(char *FileName, DWORD Header, LONG Version, LONG Size)
{
	HEADER	FileHeader;
	HANDLE	FileHandle;
	DWORD	FileWritten;

	Compiler_SetLastError(0);
	FileHandle = CreateFile(FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	FileHeader.Header = Header;
	FileHeader.Version = Version;
	FileHeader.Size = Size;

	WriteFile(FileHandle,&FileHeader,sizeof(HEADER),&FileWritten,NULL);
	if (FileWritten != sizeof(HEADER))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		CloseHandle(FileHandle);
		DeleteFile(FileName);
		return(NULL);
		}

	return(FileHandle);
}


// «»»» Sauvegarde d'un identificateur ««««««««««««««««««««««««««««««««««»

int Files_WriteHeader(HANDLE FileHandle, DWORD Header, LONG Version, LONG Size)
{
	HEADER	FileHeader;
	DWORD	FileWritten;

	FileHeader.Header = Header;
	FileHeader.Version = Version;
	FileHeader.Size = Size;

	WriteFile(FileHandle,&FileHeader,sizeof(HEADER),&FileWritten,NULL);
	if (FileWritten != sizeof(HEADER))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	return(1);
}


// «»»» Sauvegarde d'une données ««««««««««««««««««««««««««««««««««««««««»

int Files_WriteData(HANDLE FileHandle, LPVOID Data, DWORD DataSize, UINT DataType)
{
	DWORD	FileWritten;
	DWORD	MinNode[2] = {0,0};
	char	Number[32];

	switch(DataType)
		{
		case DATATYPE_NODE:
			WriteFile(FileHandle,MinNode,8,&FileWritten,NULL);
			if (FileWritten != 8)
				{
				Compiler_SetLastError(COMPILERR_WINDOWS);
				return(0);
				}
			(BYTE *)Data += 8;
			DataSize -= 8;
			break;

		case DATATYPE_TEXT:
			DataSize = strlen((char *)Data);
			break;

		case DATATYPE_TEXTNUMBER:
			wsprintf(Number,(char *)Data,DataSize);
			Data = Number;
			DataSize = strlen((char *)Data);
			break;
		}

	WriteFile(FileHandle,Data,DataSize,&FileWritten,NULL);
	if (FileWritten != DataSize)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(0);
		}

	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
