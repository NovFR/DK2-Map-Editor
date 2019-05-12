
//<<>-<>>---------------------------------------------------------------------()
/*
	Paramètres des voix
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
extern ULONG		SpeechUniqueID;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres des voix						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Properties_MapVoices(HWND hWnd, MAPPROPSCTX *MapProps)
{
	char	*FileName;
	int	 Result;

	if (!Config_GetGamePath(Config.GamePath)) return;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6804),hWnd,Properties_MapVoicesProc,(LPARAM)MapProps);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);

	//--- Efface les données allouées ---

	List_ReleaseMemory(&MapProps->SpeechList);
	FileName = GlobalAlloc(GPTR,2048);
	if (FileName)
		{
		GetTempPath(2048,FileName);
		wsprintf(szTempSpeechID,szNumber,SpeechUniqueID-1);
		strcat(FileName,szTempSoundFile);
		strcat(FileName,szTempSpeechID);
		strcat(FileName,szTempMP2Ext);
		Properties_MapVoicesDeleteTempFile(hWnd,FileName);
		GlobalFree(FileName);
		SpeechUniqueID = 0;
		}
	else
		SpeechUniqueID--;

	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Properties_MapVoicesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPPROPSCTX	*MapProps;
	long		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Properties_MapVoicesInit(hDlg,(MAPPROPSCTX *)lParam);
		return(TRUE);
		}

	MapProps = (MAPPROPSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!MapProps) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 124:
							Misc_SetUpDownInt(hDlg,104,(NM_UPDOWN *)lParam);
							break;
						case 126:
							Misc_SetUpDownInt(hDlg,106,(NM_UPDOWN *)lParam);
							break;
						case 128:
							Misc_SetUpDownInt(hDlg,108,(NM_UPDOWN *)lParam);
							break;
						case 130:
							Misc_SetUpDownInt(hDlg,110,(NM_UPDOWN *)lParam);
							break;
						case 132:
							Misc_SetUpDownInt(hDlg,112,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 104:
						case 106:
						case 108:
						case 110:
						case 112:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
								return(TRUE);
								}
							if (lData > MapProps->SpeechMentorMaxIdentifier)
								{
								SetDlgItemInt(hDlg,LOWORD(wParam),MapProps->SpeechMentorMaxIdentifier,FALSE);
								return(TRUE);
								}
							return(TRUE);
						}
					break;

				case LBN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 201:
							Properties_MapVoicesGetSoundList(hDlg,MapProps->SpeechFile,SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1,NULL);
							return(TRUE);
						}
					break;

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 100:
							Properties_MapVoicesChangeFile(hDlg,MapProps);
							return(TRUE);
						case 201:
							Properties_MapTextsSetText(hDlg,202,MapProps->SpeechFile,SendDlgItemMessage(hDlg,201,LB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Properties_MapVoicesStoreDatas(hDlg,MapProps);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation des paramètres des voix ««««««««««««««««««««««««««»

void Properties_MapVoicesInit(HWND hDlg, MAPPROPSCTX *Context)
{
	NODE	 SoundBuffers;
	NODE	*CurrentBuffer;
	LONG	 X,Y;
	int	 i;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//
	// Création de la liste des fichiers
	//
	ZeroMemory(&SoundBuffers,sizeof(NODE));

	if (!Properties_MapVoicesBuildList(&SoundBuffers))
		{
		EndDialog(hDlg,-1);
		return;
		}

	for (CurrentBuffer = SoundBuffers.next; CurrentBuffer != NULL; CurrentBuffer = CurrentBuffer->next)
		{
		if (SendDlgItemMessage(hDlg,100,CB_ADDSTRING,(WPARAM)0,(LPARAM)((MAPPROPSSOUND *)CurrentBuffer)->name) < 0)
			{
			List_ReleaseMemory(&SoundBuffers);
			EndDialog(hDlg,-1);
			return;
			}
		}

	SendDlgItemMessage(hDlg,100,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
	List_ReleaseMemory(&SoundBuffers);

	if (Context->MapProperties.SpeechFile[0])
		{
		char	*TempText;

		TempText = GlobalAlloc(GPTR,2048);
		if (TempText)
			{
			for (i = 0;; i++)
				{
				if (SendDlgItemMessage(hDlg,100,CB_GETLBTEXT,(WPARAM)i,(LPARAM)TempText) < 0)
					break;
				if (!strcmpi(TempText,(char *)Context->MapProperties.SpeechFile))
					{
					SendDlgItemMessage(hDlg,100,CB_SETCURSEL,(WPARAM)i,(LPARAM)0);
					break;
					}
				}
			GlobalFree(TempText);
			}
		}

	//
	// Place les valeurs par défaut
	//
	SetDlgItemInt(hDlg,104,Context->MapProperties.SpeechPreLevelID,FALSE);
	SetDlgItemInt(hDlg,106,Context->MapProperties.SpeechWinID,FALSE);
	SetDlgItemInt(hDlg,108,Context->MapProperties.SpeechLooseID,FALSE);
	SetDlgItemInt(hDlg,110,Context->MapProperties.SpeechNewsID,FALSE);
	SetDlgItemInt(hDlg,112,Context->MapProperties.SpeechGenericPreLevelID,FALSE);

	i = Properties_MapVoicesGetSoundList(hDlg,szSpeechMentor,-1,NULL);
	if (i == -1) i = 1505;
	Misc_CreateUpDownButton(hDlg,104,124);
	Misc_CreateUpDownButton(hDlg,106,126);
	Misc_CreateUpDownButton(hDlg,108,128);
	Misc_CreateUpDownButton(hDlg,110,130);
	Misc_CreateUpDownButton(hDlg,112,132);
	Context->SpeechMentorMaxIdentifier = i;

	strncpy(Context->SpeechFile,(char *)Context->MapProperties.SpeechFile,32);

	//
	// Création de la liste des sons dans le fichier sélectionné
	//
	if (Context->MapProperties.SpeechFile[0])
		{
		ZeroMemory(&Context->SpeechList,sizeof(NODE));
		Properties_MapVoicesGetSoundList(hDlg,(char *)Context->MapProperties.SpeechFile,-1,&Context->SpeechList);
		Properties_MapVoicesCreateDisplayList(hDlg,201,&Context->SpeechList);
		}

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	return;
}


// «»»» Sauvegarde des modifications ««««««««««««««««««««««««««««««««««««»

void Properties_MapVoicesStoreDatas(HWND hDlg, MAPPROPSCTX *Context)
{
	strncpy((char *)Context->MapProperties.SpeechFile,Context->SpeechFile,32);

	Context->MapProperties.SpeechPreLevelID		= (WORD)GetDlgItemInt(hDlg,104,NULL,FALSE);
	Context->MapProperties.SpeechWinID		= (WORD)GetDlgItemInt(hDlg,106,NULL,FALSE);
	Context->MapProperties.SpeechLooseID		= (WORD)GetDlgItemInt(hDlg,108,NULL,FALSE);
	Context->MapProperties.SpeechNewsID		= (WORD)GetDlgItemInt(hDlg,110,NULL,FALSE);
	Context->MapProperties.SpeechGenericPreLevelID  = (WORD)GetDlgItemInt(hDlg,112,NULL,FALSE);
	return;
}


// «»»» Affichage de la liste des sons ««««««««««««««««««««««««««««««««««»

void Properties_MapVoicesCreateDisplayList(HWND hDlg, UINT CtlId, NODE *List)
{
	MAPPROPSSOUND	*Sound;
	char		*Text;
	long		 i;

	Text = GlobalAlloc(GPTR,2048);
	if (!Text) return;

	for (i = 1, Sound = (MAPPROPSSOUND *)List->next; Sound != NULL; Sound = (MAPPROPSSOUND *)(((NODE *)Sound)->next), i++)
		{
		wsprintf(Text,szCfgVoicesList,i,Sound->name,Sound->node.type);
		SendDlgItemMessage(hDlg,CtlId,LB_ADDSTRING,(WPARAM)0,(LPARAM)Text);
		}

	GlobalFree(Text);
	return;
}


// «»»» Changement de sélection «««««««««««««««««««««««««««««««««««««««««»

void Properties_MapVoicesChangeFile(HWND hDlg, MAPPROPSCTX *Context)
{
	char	*File;
	long	 Cursor;

	Cursor = SendDlgItemMessage(hDlg,100,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if (Cursor < 0) return;
	if (!Cursor)
		{
		ZeroMemory(Context->SpeechFile,32);
		return;
		}

	File = GlobalAlloc(GPTR,2048);
	if (!File) return;

	Cursor = SendDlgItemMessage(hDlg,100,CB_GETLBTEXT,(WPARAM)Cursor,(LPARAM)File);
	if (Cursor < 0)
		{
		GlobalFree(File);
		return;
		}
	strncpy(Context->SpeechFile,File,32);
	GlobalFree(File);

	SendDlgItemMessage(hDlg,201,LB_RESETCONTENT,(WPARAM)0,(LPARAM)0);
	List_ReleaseMemory(&Context->SpeechList);
	ZeroMemory(&Context->SpeechList,sizeof(NODE));
	Properties_MapVoicesGetSoundList(hDlg,Context->SpeechFile,-1,&Context->SpeechList);
	Properties_MapVoicesCreateDisplayList(hDlg,201,&Context->SpeechList);
	return;
}



// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des fichiers son						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Efface un fichier temporaire ««««««««««««««««««««««««««««««««««««»

void Properties_MapVoicesDeleteTempFile(HWND hWnd, char *FileName)
{
Loop:	if	(  (!DeleteFile(FileName))
		&& (GetLastError() != ERROR_FILE_NOT_FOUND)
		&& (Misc_MessageBoxExt(hWnd,szCfgVoicesTemp,(char *)-1,szMessageBoxExtOkCancel,MB_ICONEXCLAMATION) != 0)
		)
		goto Loop;

	return;
}


// «»»» Création de la liste des fichiers son «««««««««««««««««««««««««««»

int Properties_MapVoicesBuildList(NODE *SoundList)
{
	LPSHELLFOLDER	 Desktop;
	LPSHELLFOLDER	 Folder;
	LPENUMIDLIST	 Directory;
	LPITEMIDLIST	 Object;
	LPCITEMIDLIST	 ObjectList;
	LPMALLOC	 Malloc;
	STRRET		 Name;
	ULONG		 Unused = 0;
	ULONG		 ObjectAttributes = 0;
	UINT		 Result;
	MAPPROPSSOUND	*NewSound;
	char		*SoundPath;
	char		*NameBuffer;
	char		*NameSlash;
	wchar_t		*DisplayNameBuffer;

	SoundPath = GlobalAlloc(GPTR,MAX_PATH);
	if (!SoundPath) goto Error_0;
	DisplayNameBuffer = GlobalAlloc(GPTR,MAX_PATH*2);
	if (!DisplayNameBuffer) goto Error_1;

	CopyMemory(SoundPath,Config.GamePath,MAX_PATH);
	strcat(SoundPath,szSoundsPath);

	//--- Retrieve the path in the shellnamespace ---

	SHGetMalloc(&Malloc);
	SHGetDesktopFolder(&Desktop);
	mbstowcs(DisplayNameBuffer,SoundPath,strlen(SoundPath));
	Result = Desktop->lpVtbl->ParseDisplayName(Desktop,NULL,NULL,DisplayNameBuffer,&Unused,&Object,&ObjectAttributes);
	if (Result != NOERROR) goto Error_2;
	Result = Desktop->lpVtbl->BindToObject(Desktop,Object,0,&IID_IShellFolder,&(LPVOID)Folder);
	if (Result != NOERROR) goto Error_2;
	Desktop->lpVtbl->Release(Desktop);
	Desktop = Folder;

	//--- Enumerate objects in the list (= obtain the file list, he!) ---

	Result = Desktop->lpVtbl->EnumObjects(Desktop,NULL,SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|SHCONTF_INCLUDEHIDDEN,&Directory);
	if (Result != NOERROR) goto Error_2;
	Directory->lpVtbl->Reset(Directory);

	NewSound = GlobalAlloc(GPTR,sizeof(MAPPROPSSOUND));
	if (!NewSound) goto Error_4;
	strcpy(NewSound->name,szCfgVoicesNoFile);
	List_AddEntry((NODE *)NewSound,SoundList);

	for (;;)
		{
		Result = Directory->lpVtbl->Next(Directory,1,&Object,NULL);
		if (Result == S_FALSE) break;
		if (Result != NOERROR) goto Error_4;

		ObjectList = Object;
		ObjectAttributes = 0;
		Desktop->lpVtbl->GetAttributesOf(Desktop,1,&ObjectList,&ObjectAttributes);
		Desktop->lpVtbl->GetDisplayNameOf(Desktop,Object,SHGDN_FORPARSING,&Name);

		switch(Name.uType)
			{
			case STRRET_WSTR:
				NameBuffer = (char *)Malloc->lpVtbl->Alloc(Malloc,MAX_PATH);
				if (!NameBuffer) goto Error_4;
				wcstombs(NameBuffer,Name.DUMMYUNIONNAME.pOleStr,MAX_PATH);
				strcpy(SoundPath,NameBuffer);
				Malloc->lpVtbl->Free(Malloc,NameBuffer);
				break;
			case STRRET_OFFSET:
				strcpy(SoundPath,((char *)Object)+(Name.DUMMYUNIONNAME.uOffset));
				break;
			case STRRET_CSTR:
				strcpy(SoundPath,Name.DUMMYUNIONNAME.cStr);
				break;
			}

		if (ObjectAttributes & SFGAO_FOLDER)
			{
			NewSound = GlobalAlloc(GPTR,sizeof(MAPPROPSSOUND));
			if (!NewSound) goto Error_4;
			NameSlash = strrchr(SoundPath,'\\');
			if (NameSlash) strncpy(NewSound->name,++NameSlash,64);
			else strncpy(NewSound->name,SoundPath,64);
			CharUpperBuff(NewSound->name,strlen(NewSound->name));
			if (strncmpi(NewSound->name,szSoundFile,strlen(szSoundFile)))
				{
				GlobalFree(NewSound);
				continue;
				}
			List_AddEntry((NODE *)NewSound,SoundList);
			}
		}

	//--- Oh Dear! It is done and ok ---

	Directory->lpVtbl->Release(Directory);
	Desktop->lpVtbl->Release(Desktop);
	Malloc->lpVtbl->Release(Malloc);
	GlobalFree(DisplayNameBuffer);
	GlobalFree(SoundPath);
	return(1);

//--- Erreurs ---

Error_4:List_ReleaseMemory(SoundList);
	Directory->lpVtbl->Release(Directory);
Error_2:Desktop->lpVtbl->Release(Desktop);
	Malloc->lpVtbl->Release(Malloc);
	GlobalFree(DisplayNameBuffer);
Error_1:GlobalFree(SoundPath);
Error_0:return(0);
}


// «»»» Création de la liste des sons à l'intérieur du fichier ««««««««««»

//
// Note:
//	Les identificateurs vont de 1 à x (ils ne commencent pas par 0)
//

int Properties_MapVoicesGetSoundList(HWND hWnd, char *SoundName, LONG Identifier, NODE *SoundList)
{
	NODE	*Current;
	HANDLE	 FileHandle;
	DWORD	 LastError;
	DWORD	 RedBytes;
	DWORD	 Count;
	DWORD	*Table;
	DWORD	*SoundHeader;
	DWORD	*SoundFile;
	char	*FileName;
	long	 i;

	if (!SoundName) return(-1);
	if (!*SoundName) return(-1);

	//
	// Chargement de l'en-tête
	//

	FileName = GlobalAlloc(GPTR,2048);
	if (!FileName) return(-1);

	strcpy(FileName,Config.GamePath);
	strcat(FileName,szSoundsPath);
	strcat(FileName,SoundName);
	strcat(FileName,szPath);
	strcat(FileName,SoundName);
	strcat(FileName,szSoundBuffer);

	FileHandle = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	LastError = GetLastError();
	if (FileHandle == INVALID_HANDLE_VALUE) goto Error_0;

	ReadFile(FileHandle,&Count,4,&RedBytes,NULL);
	LastError = GetLastError();
	if (RedBytes != 4) goto Error_1;

	if ((Identifier == -1)&&(SoundList == NULL))
		{
		CloseHandle(FileHandle);
		GlobalFree(FileName);
		return(Count);
		}

	//--- Chargement de la table des valeurs ---

	Table = GlobalAlloc(GPTR,Count*4);
	LastError = GetLastError();
	if (!Table) goto Error_1;

	ReadFile(FileHandle,Table,Count*4,&RedBytes,NULL);
	LastError = GetLastError();
	if (RedBytes != Count*4) goto Error_2;

	//--- Préparation de la lecture ---

	SoundHeader = GlobalAlloc(GPTR,38);
	LastError = GetLastError();
	if (!SoundHeader) goto Error_2;

	if (SoundList == NULL) goto Read_Sound;

	//
	// Lecture de la liste des fichiers
	//

Read_List:
	for (i = 0; i != Count; i++)
		{
		RedBytes = SetFilePointer(FileHandle,Table[i],NULL,FILE_BEGIN);
		LastError = GetLastError();
		if (RedBytes == 0xFFFFFFFF) goto Error_7;

		ReadFile(FileHandle,SoundHeader,40,&RedBytes,NULL);
		LastError = GetLastError();
		if (RedBytes != 40) goto Error_7;

		Current = GlobalAlloc(GPTR,sizeof(MAPPROPSSOUND));
		LastError = GetLastError();
		if (!Current) goto Error_7;

		strcpy(((MAPPROPSSOUND *)Current)->name,(char *)SoundHeader+8);
		Current->type = *(((LONG *)SoundHeader)+1);
		List_AddEntry(Current,SoundList);
		}

	CloseHandle(FileHandle);
	GlobalFree(SoundHeader);
	GlobalFree(Table);
	GlobalFree(FileName);
	return(0);

	//
	// Lecture d'un son particulier
	//

Read_Sound:
	if (Identifier > Count)
		{
		GlobalFree(SoundHeader);
		GlobalFree(Table);
		CloseHandle(FileHandle);
		GlobalFree(FileName);
		Misc_MessageBoxExt(hWnd,szCfgVoicesNoSuchSound,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return(-1);
		}

	if (!Config_GetPlayerPath(hWnd))
		{
		GlobalFree(SoundHeader);
		GlobalFree(Table);
		CloseHandle(FileHandle);
		GlobalFree(FileName);
		return(-1);
		}

	RedBytes = SetFilePointer(FileHandle,Table[(Identifier-1)],NULL,FILE_BEGIN);
	LastError = GetLastError();
	if (RedBytes == 0xFFFFFFFF) goto Error_3;

	ReadFile(FileHandle,SoundHeader,40,&RedBytes,NULL);
	LastError = GetLastError();
	if (RedBytes != 40) goto Error_3;

	if (SoundHeader[0] != 0x00000028)
		{
		GlobalFree(SoundHeader);
		GlobalFree(Table);
		CloseHandle(FileHandle);
		GlobalFree(FileName);
		Misc_MessageBoxExt(hWnd,szCfgVoicesBadSound,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return(-1);
		}

	//--- Lecture du son ---

	SoundFile = GlobalAlloc(GPTR,SoundHeader[1]);
	LastError = GetLastError();
	if (!SoundFile) goto Error_3;

	ReadFile(FileHandle,SoundFile,SoundHeader[1],&RedBytes,NULL);
	LastError = GetLastError();
	if (RedBytes != SoundHeader[1]) goto Error_4;

	//--- Ecriture du son dans un fichier temporaire ---

	CloseHandle(FileHandle);
	GetTempPath(2048,FileName);
	wsprintf(szTempSpeechID,szNumber,SpeechUniqueID++);
	strcat(FileName,szTempSoundFile);
	strcat(FileName,szTempSpeechID);
	strcat(FileName,szTempMP2Ext);
	FileHandle = CreateFile(FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE) goto Error_5;

	WriteFile(FileHandle,SoundFile,SoundHeader[1],&RedBytes,NULL);
	if (RedBytes != SoundHeader[1]) goto Error_6;

	CloseHandle(FileHandle);
	GlobalFree(SoundFile);
	GlobalFree(SoundHeader);
	GlobalFree(Table);

	if (Config.PlayerUsesShell) ShellExecute(hWnd,"open",FileName,NULL,NULL,SW_SHOWDEFAULT);
	else ShellExecute(hWnd,"open",Config.PlayerPath,FileName,NULL,SW_SHOWDEFAULT);

	//--- Efface le fichier PRECEDENT (le fichier actuel est en cours d'utilisation) ---

	Sleep(1000);
	GetTempPath(2048,FileName);
	wsprintf(szTempSpeechID,szNumber,SpeechUniqueID-2);
	strcat(FileName,szTempSoundFile);
	strcat(FileName,szTempSpeechID);
	strcat(FileName,szTempMP2Ext);
	Properties_MapVoicesDeleteTempFile(hWnd,FileName);

	GlobalFree(FileName);
	return(0);

//--- Erreurs ---

Error_7:List_ReleaseMemory(SoundList);
	goto Error_3;

Error_6:LastError = GetLastError();
	GlobalFree(SoundFile);
	GlobalFree(SoundHeader);
	GlobalFree(Table);
	CloseHandle(FileHandle);
	DeleteFile(FileName);
	goto Error_0;

Error_5:LastError = GetLastError();
	GlobalFree(SoundFile);
	GlobalFree(SoundHeader);
	GlobalFree(Table);
	DeleteFile(FileName);
	goto Error_0;

Error_4:GlobalFree(SoundFile);
Error_3:GlobalFree(SoundHeader);
Error_2:GlobalFree(Table);
Error_1:CloseHandle(FileHandle);
Error_0:GlobalFree(FileName);
	SetLastError(LastError);
	Misc_PrintError(hWnd,szCfgVoicesError,NULL,MB_ICONHAND);
	return(-1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
