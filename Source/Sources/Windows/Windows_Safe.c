
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines relatives à la récupération des données perdues.
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
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Test si le programme s'est terminé normalement ««««««««««««««««««»

void Safe_CheckSafeFlag(HWND hWnd)
{
	int	Result;

	if (Registry_CheckSafeFlag(hWnd)) return;

	Result = DialogBox(hInstance,MAKEINTRESOURCE(6002),hWnd,Safe_WindowProc);
	if (Result == -1)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	if (Result)
		Files_LoadSafeMap(hWnd);

	return;
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
BOOL CALLBACK Safe_WindowProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		case WM_INITDIALOG: {
			LONG	 X,Y;
			char	*FileName;

			Window_Center(hDlg,NULL,&X,&Y);
			SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);

			FileName = Config_GetUserFileName(szTemporaryFolder,szTemporaryFile,FALSE);
			if (FileName)
				{
				HANDLE	FileHandle;

				FileHandle = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
				if (FileHandle != INVALID_HANDLE_VALUE)
					{
					CloseHandle(FileHandle);
					EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
					}

				GlobalFree(FileName);
				}

			} return(TRUE);

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							EndDialog(hDlg,1);
							return(TRUE);
						case IDCANCEL:
							EndDialog(hDlg,0);
							return(TRUE);
						}
					break;
				}
			break;
		}

	return(FALSE);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
