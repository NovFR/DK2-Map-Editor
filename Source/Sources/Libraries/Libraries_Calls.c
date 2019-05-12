
//<<>-<>>---------------------------------------------------------------------()
/*
	Appel des fonctions externes
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
// ¤¤¤ Appel d'une fonction						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Library_Call(HWND hWnd, UINT Function, void *Data)
{
	HMODULE	 Library;
	LONG	 LibraryVersion;
	char	*LibraryName;
	int	 Result;

	switch(Function)
		{
		case LIBRARY_CAMERAPATH:
			LibraryName = szCameraPathsName;
			LibraryVersion = 37;
			break;

		default:return(0);
		}

	Library = Library_Open(hWnd,LibraryName,LibraryVersion);
	if (!Library) return(0);

	switch(Function)
		{
		case LIBRARY_CAMERAPATH: {
			int (WINAPI *CameraPath)(HWND,CAMERAPATH *);
			CameraPath = (void *)GetProcAddress(Library,szCameraPathsFunction);
			if (!CameraPath)
				{
				Misc_PrintError(hWnd,szLibraryAccessErr,szLibraryErr,MB_ICONHAND);
				return(0);
				}
			Result = CameraPath(hWnd,(CAMERAPATH *)Data);
			} break;

		default:Result = 0;
		}

	Library_Close(hWnd,Library);
	return(Result);

}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ouverture d'une bibliothèque ««««««««««««««««««««««««««««««««««««»

HMODULE Library_Open(HWND hWnd, char *LibName, LONG Version)
{
	LONG	(WINAPI *QueryVersion)(void);
	HMODULE  LibraryInstance;
	char	*LibraryNamePtr;
	char	*LibraryNamePtrTmp;

	//
	// --- Charge la bibliothèqe ---
	//

	LibraryNamePtr = GlobalAlloc(GPTR,MAX_PATH);
	if (!LibraryNamePtr)
		{
		Misc_PrintError(hWnd,szLibraryErr,NULL,MB_ICONHAND);
		return(NULL);
		}
	GetModuleFileName(hInstance,LibraryNamePtr,MAX_PATH);
	LibraryNamePtrTmp = strrchr(LibraryNamePtr,'\\');
	if (LibraryNamePtrTmp) *LibraryNamePtrTmp = 0;
	strcat(LibraryNamePtr,szLibrariesPath);
	strcat(LibraryNamePtr,LibName);

	LibraryInstance = LoadLibraryEx(LibraryNamePtr,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!LibraryInstance)
		{
		Misc_PrintError(hWnd,szLibraryErr,NULL,MB_ICONHAND);
		GlobalFree(LibraryNamePtr);
		return(NULL);
		}
	GlobalFree(LibraryNamePtr);

	//
	// --- Test la version ---
	//

	QueryVersion = (void *)GetProcAddress(LibraryInstance,szLibrariesVersion);
	if (!QueryVersion)
		{
		Misc_PrintError(hWnd,szQueryVersionErr,NULL,MB_ICONHAND);
		Library_Close(hWnd,LibraryInstance);
		return(NULL);
		}

	if (QueryVersion() < Version)
		{
		MessageBox(hWnd,szLibraryVersionErr,NULL,MB_ICONEXCLAMATION);
		Library_Close(hWnd,LibraryInstance);
		return(NULL);
		}

	return(LibraryInstance);
}


// «»»» Fermeture d'une bibliothèque ««««««««««««««««««««««««««««««««««««»

void Library_Close(HWND hWnd, HMODULE Library)
{
Loop:	if (!FreeLibrary(Library))
		{
		Misc_PrintError(hWnd,szLibraryCloseErr,NULL,MB_ICONHAND);
		goto Loop;
		}
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
