
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines relatives à la sélection.
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
extern HWND		hWnd;
extern HMENU		hMenu;
extern MAP		Map;
extern MAPCOPY		MapCopy;
extern MAPRECT		MapCursorSel;
extern NODE		MapCopyThings;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern LONG		MapChanges;
extern DWORD		WorldItemID;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines diverses						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» La sélection a été modifiée «««««««««««««««««««««««««««««««««««««»

void Select_Update()
{
	UINT	State;

	State = (MapCursorSel.hidden)?MF_GRAYED:MF_ENABLED;

	EnableMenuItem(hMenu,IDM_COPY,State);
	EnableMenuItem(hMenu,IDM_CUT,State);
	EnableMenuItem(hMenu,IDM_LOADBRUSHINSEL,State);
	EnableMenuItem(hMenu,IDM_ROTATEP90,State);
	EnableMenuItem(hMenu,IDM_ROTATEM90,State);
	EnableMenuItem(hMenu,IDM_SYMHORZ,State);
	EnableMenuItem(hMenu,IDM_SYMVERT,State);
	EnableMenuItem(hMenu,IDM_SELECTION,State);
	EnableMenuItem(hMenu,IDM_SELECTNONE,State);

	if ((State == MF_ENABLED)&&(MapCopy.Map))
		EnableMenuItem(hMenu,IDM_PASTE,State);
	else
		EnableMenuItem(hMenu,IDM_PASTE,MF_GRAYED);

	Info_Update();
	return;
}


// «»»» La copie a été modifiée «««««««««««««««««««««««««««««««««««««««««»

void Select_UpdateCopy()
{
	UINT	State;

	State = (MapCopy.Map)?MF_ENABLED:MF_GRAYED;

	EnableMenuItem(hMenu,IDM_COPYZONE,State);

	if ((State == MF_ENABLED)&&(!MapCursorSel.hidden))
		EnableMenuItem(hMenu,IDM_PASTE,State);
	else
		EnableMenuItem(hMenu,IDM_PASTE,MF_GRAYED);

	Info_Update();
	return;
}


// «»»» Modification de la sélection (tout ou rien) «««««««««««««««««««««»

void Select_All(BOOL All)
{
	RECT	MapRect;

	Mouse_HideSelRect(hWnd);
	Select_Update();
	if (!All) return;

	MapCursorSel.x1 = 1;
	MapCursorSel.y1 = 1;
	MapCursorSel.x2 = Map.Properties.Width-2;
	MapCursorSel.y2 = Map.Properties.Height-2;
	MapCursorSel.hidden = FALSE;
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Select_Update();
	return;
}


// «»»» Destruction de la copie «««««««««««««««««««««««««««««««««««««««««»

void Select_ReleaseCopyMemory()
{
	if (!MapCopy.Map) return;

	GlobalFree(MapCopy.Map);
	MapCopy.Map = NULL;
	List_ReleaseMemory(&MapCopyThings);
	Select_UpdateCopy();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Changement des dimensions de la sélection par requête		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Select_Change()
{
	MAPCLIENT	Client;
	int		Result;

	if (!MapCursorSel.hidden)
		Map_CorrectRect(&MapCursorSel,&Client);
	else
		{
		Client.X = 1;
		Client.Y = 1;
		Client.Width = 1;
		Client.Height = 1;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6300),hWnd,Select_ChangeProc,(LPARAM)&Client);
	if ((Result == 0)||(Result == -1))
		{
		if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	Mouse_HideSelRect(hWnd);
	MapCursorSel.x1 = Client.X;
	MapCursorSel.y1 = Client.Y;
	MapCursorSel.x2 = Client.X+Client.Width-1;
	MapCursorSel.y2 = Client.Y+Client.Height-1;
	MapCursorSel.hidden = FALSE;

	if (Map_RectangleToClient(hWnd,&MapCursorSel,&Client))
		{
		Client.Width += Client.X;
		Client.Height += Client.Y;
		InvalidateRect(hWnd,(RECT *)&Client,FALSE);
		UpdateWindow(hWnd);
		}

	Select_Update();
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Select_ChangeProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPCLIENT	*MapClient;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER);
		MapClient = (MAPCLIENT *)lParam;
		SetWindowLong(hDlg,DWL_USER,(LONG)MapClient);
		SetDlgItemInt(hDlg,101,MapClient->X,FALSE);
		SetDlgItemInt(hDlg,102,MapClient->Y,FALSE);
		SetDlgItemInt(hDlg,103,MapClient->Width,FALSE);
		SetDlgItemInt(hDlg,104,MapClient->Height,FALSE);
		SendDlgItemMessage(hDlg,101,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		SendDlgItemMessage(hDlg,102,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		SendDlgItemMessage(hDlg,103,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		SendDlgItemMessage(hDlg,104,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		Misc_CreateUpDownButton(hDlg,101,105);
		Misc_CreateUpDownButton(hDlg,102,106);
		Misc_CreateUpDownButton(hDlg,103,107);
		Misc_CreateUpDownButton(hDlg,104,108);
		return(TRUE);
		}

	MapClient = (MAPCLIENT *)GetWindowLong(hDlg,DWL_USER);
	if (!MapClient) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 105:
							Misc_SetUpDownInt(hDlg,101,(NM_UPDOWN *)lParam);
							break;
						case 106:
							Misc_SetUpDownInt(hDlg,102,(NM_UPDOWN *)lParam);
							break;
						case 107:
							Misc_SetUpDownInt(hDlg,103,(NM_UPDOWN *)lParam);
							break;
						case 108:
							Misc_SetUpDownInt(hDlg,104,(NM_UPDOWN *)lParam);
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
						case 101:
							MapClient->X = GetDlgItemInt(hDlg,101,NULL,FALSE);
							if (MapClient->X > Map.Properties.Width-1) SetDlgItemInt(hDlg,101,Map.Properties.Width-1,FALSE);
							if (MapClient->X < 0) SetDlgItemInt(hDlg,101,0,FALSE);
							break;
						case 102:
							MapClient->Y = GetDlgItemInt(hDlg,102,NULL,FALSE);
							if (MapClient->Y > Map.Properties.Height-1) SetDlgItemInt(hDlg,102,Map.Properties.Height-1,FALSE);
							if (MapClient->Y < 0) SetDlgItemInt(hDlg,102,0,FALSE);
							break;
						case 103:
							MapClient->Width = GetDlgItemInt(hDlg,103,NULL,FALSE);
							if (MapClient->Width > Map.Properties.Width-1) SetDlgItemInt(hDlg,103,Map.Properties.Width-1,FALSE);
							if (MapClient->Width < 0) SetDlgItemInt(hDlg,103,0,FALSE);
							break;
						case 104:
							MapClient->Height = GetDlgItemInt(hDlg,104,NULL,FALSE);
							if (MapClient->Height > Map.Properties.Height-1) SetDlgItemInt(hDlg,104,Map.Properties.Height-1,FALSE);
							if (MapClient->Height < 0) SetDlgItemInt(hDlg,104,0,FALSE);
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							MapClient->X = GetDlgItemInt(hDlg,101,NULL,FALSE);
							MapClient->Y = GetDlgItemInt(hDlg,102,NULL,FALSE);
							MapClient->Width = GetDlgItemInt(hDlg,103,NULL,FALSE);
							MapClient->Height = GetDlgItemInt(hDlg,104,NULL,FALSE);
							if (MapClient->X+MapClient->Width > Map.Properties.Width) goto Rect_Error;
							if (MapClient->Y+MapClient->Height > Map.Properties.Height) goto Rect_Error;
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

//--- Erreurs dans les dimensions ---

Rect_Error:
	Misc_MessageBoxExt(hDlg,szSelectRectErr,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
	return(TRUE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement et sauvegarde						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Chargement d'une zone sauvegardée «««««««««««««««««««««««««««««««»

void Select_Load(BOOL ToCopyMemory)
{
	MAPINFO	*MapInfo;
	NODE	 MapThings;

	if ((!ToCopyMemory)&&(MapCursorSel.hidden)) return;
	if (!Misc_FileSelect(hWnd,Config.AreaPath,FILE_AREA)) return;

	MapInfo = Areas_ExtractMap(Config.AreaPath);
	if (!MapInfo)
		{
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}
	if (!Areas_ExtractThings(Config.AreaPath,&MapThings,NULL))
		{
		char	ErrorText[256];

		wsprintf(ErrorText,szAreaThingsErr,Compiler_GetErrorCodeText());
		if (!Misc_MessageBoxExt(hWnd,ErrorText,NULL,szMessageBoxExtYesNo,MB_ICONQUESTION))
			{
			GlobalFree(MapInfo->Map);
			GlobalFree(MapInfo);
			return;
			}
		}

	if (ToCopyMemory)
		{
		if (Select_CopyRequest())
			{
			Select_ReleaseCopyMemory();
			MapCopy.Map = MapInfo->Map;
			MapCopy.Width = MapInfo->Width;
			MapCopy.Height = MapInfo->Height;
			List_Move(&MapThings,&MapCopyThings);
			GlobalFree(MapInfo);
			Select_UpdateCopy();
			}
		else
			{
			GlobalFree(MapInfo->Map);
			GlobalFree(MapInfo);
			}
		}
	else
		{
		MAPCOPY	MapCopy;

		MapCopy.Map = MapInfo->Map;
		MapCopy.Width = MapInfo->Width;
		MapCopy.Height = MapInfo->Height;
		Select_Paste(&MapCopy,&MapThings);
		List_ReleaseMemory(&MapThings);
		GlobalFree(MapInfo->Map);
		GlobalFree(MapInfo);
		}

	return;
}


// «»»» Sauvegarde d'une zone sauvegardée «««««««««««««««««««««««««««««««»

void Select_Save(BOOL CopyMemory)
{
	DWORD		*BinMap;
	MAPINFO		 MapInfo;
	MAPCLIENT	 MapClient;
	LONG		 Width;
	LONG		 Height;
	HANDLE		 FileHandle;
	DWORD		 FileWritten;
	LONG		 Size;

	if ((CopyMemory)&&(!MapCopy.Map)) return;
	if ((!CopyMemory)&&(MapCursorSel.hidden)) return;
	if (!Misc_FileSelectSave(hWnd,Config.AreaPath,FILE_AREA)) return;

	//--- Initialisations ---

	ZeroMemory(&MapInfo,sizeof(MAPINFO));
	if (CopyMemory)
		{
		Width = MapCopy.Width;
		Height = MapCopy.Height;
		MapInfo.Map = MapCopy.Map;
		MapInfo.Width = MapCopy.Width;
		MapInfo.Height = MapCopy.Height;
		}
	else
		{
		Map_CorrectRect(&MapCursorSel,&MapClient);
		Width = MapClient.Width;
		Height = MapClient.Height;
		MapInfo.Width = MapClient.Width;
		MapInfo.Height = MapClient.Height;
		}

	//--- Traduit la zone en carte du jeu ---

	if (CopyMemory)
		BinMap = Compiler_EditorMapToGameMap(&MapInfo);
	else
		{
		BYTE	*MapPtr;
		BYTE	*MapLinePtr;
		BYTE	*BinPtr;
		BYTE	*BinLinePtr;
		int	 i,j;

		MapInfo.Map = GlobalAlloc(GPTR,MapClient.Width*MapClient.Height*MAP_CONTROLBYTES);
		if (MapInfo.Map)
			{
			MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
			BinPtr = MapInfo.Map;
			for (i = 0; i != MapClient.Height; i++)
				{
				BinLinePtr = BinPtr;
				MapLinePtr = MapPtr;
				for (j = 0; j != MapClient.Width; j++)
					{
					*(BinLinePtr+0) = *(MapLinePtr+0);
					*(BinLinePtr+1) = *(MapLinePtr+1);
					*(BinLinePtr+2) = *(MapLinePtr+2);
					*(BinLinePtr+3) = *(MapLinePtr+3);
					BinLinePtr += MAP_CONTROLBYTES;
					MapLinePtr += MAP_CONTROLBYTES;
					}
				BinPtr += MapClient.Width*MAP_CONTROLBYTES;
				MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
				}
			BinMap = Compiler_EditorMapToGameMap(&MapInfo);
			GlobalFree(MapInfo.Map);
			}
		else
			Compiler_SetLastError(COMPILERR_MAPALLOC);
		}


	if (!BinMap)
		{
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}

	//--- Sauvegarde la zone et les éléments associés à celle-ci ---

	Size = sizeof(HEADER)+8+Width*Height*4;
	Size += Areas_GetThingsSize(CopyMemory,&MapClient);

	FileHandle = Files_Create(Config.AreaPath,FILEHEADER_AREA,DKZVERSION,Size);
	if (!FileHandle)
		{
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		return;
		}

	if (!Files_WriteHeader(FileHandle,FILEHEADER_MAP,DKMAPVERSION,Width*Height*4+8))
		goto Done;

	WriteFile(FileHandle,&Width,4,&FileWritten,NULL);
	if (FileWritten != 4)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Done;
		}
	WriteFile(FileHandle,&Height,4,&FileWritten,NULL);
	if (FileWritten != 4)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Done;
		}

	WriteFile(FileHandle,BinMap,Width*Height*4,&FileWritten,NULL);
	if (FileWritten != Width*Height*4)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Done;
		}

	Areas_WriteThings(FileHandle,CopyMemory,&MapClient);

Done:	CloseHandle(FileHandle);
	GlobalFree(BinMap);

	if (Compiler_GetLastError())
		{
		DeleteFile(Config.AreaPath);
		Misc_MessageBoxExt(hWnd,Compiler_GetErrorCodeText(),NULL,szMessageBoxExtOk,MB_ICONHAND);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Transformations du rectangle de sélection			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Rotation ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Select_Rotate(BOOL Clockwise)
{
	MAPRECT		 MapRect;
	MAPCLIENT	 MapClient;
	MAPTHING	*Thing;
	NODE		 List;
	UNDO		*Undo;
	UNDO		*Child;
	BYTE		 MapBadID;
	BYTE		*MapPtr;
	BYTE		*WorkPtr;
	BYTE		*LinePtr;
	BYTE		*TempPtr;
	BYTE		*TempLinePtr;
	int		 i,j;

	Map_CorrectRect(&MapCursorSel,&MapClient);
	ZeroMemory(&List,sizeof(NODE));

	//--- Test si le rectangle est ... un carré ---

	if ( MapClient.Width != MapClient.Height )
		{
		Misc_MessageBoxExt(hWnd,szRotationRequest,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	//--- Procède à la transformation dans une zone temporaire ---

	WorkPtr = GlobalAlloc(GPTR,MapClient.Width*MapClient.Height*MAP_CONTROLBYTES);
	if (!WorkPtr)
		{
		Misc_PrintError(hWnd,szTransErr,NULL,MB_ICONHAND);
		return;
		}

	Undo = Undo_CreateNewEntry(UNDO_ROTATION,(Clockwise)?szMapRotation:szMapCounterRotation);
	Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPRECT,szMapModify);
	if (!Undo_StoreMapRect(Child,&MapCursorSel,&Map))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		Undo = NULL;
		}
	Map_Lock(TRUE,FALSE);
	MapBadID = Map_TranslateID(0xFE,0x01,World);
	MapChanges++;

	for (j = 0; j != MapClient.Height; j++)
		for (i = 0; i != MapClient.Width; i++)
			{
			Things_CloneAtMapPos(&MapCreatures,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapDoors,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapTraps,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapObjects,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&List);
			Things_RemoveAtMapPosEx(&MapCreatures,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapDoors,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapTraps,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapObjects,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&Undo);
			}

	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	i = MapClient.Width;

	if (!Clockwise)
		{
		TempPtr = WorkPtr+MapClient.Width*(MapClient.Height-1)*MAP_CONTROLBYTES;

		do {
			//	1
			//	2	->
			//	3
			//	4		1234
			for (j = 0; j != MapClient.Width; j++)
				{
				for (Thing = (MAPTHING *)List.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
					{
					if ((Thing->x == MapClient.X+MapClient.Width-i)&&(Thing->y == MapClient.Y+j))
						{
						NODE		*List;
						MAPTHING	*NewThing;
						UINT		 Type;

						switch(Thing->node.type)
							{
							case THING_CREATURE:
								List = &MapCreatures;
								Type = UNDO_ADDCREATURE;
								break;
							case THING_DOOR:
								List = &MapDoors;
								Type = UNDO_ADDDOOR;
								break;
							case THING_TRAP:
								List = &MapTraps;
								Type = UNDO_ADDTRAP;
								break;
							case THING_OBJECT:
								List = &MapObjects;
								Type = UNDO_ADDOBJECT;
								break;
							case THING_MAGICALOBJECT:
								List = &MapMagicalObjects;
								Type = UNDO_ADDMAGIC;
								break;
							}
						NewThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
						if (!NewThing) continue;
						CopyMemory(NewThing,Thing,sizeof(MAPTHING));
						NewThing->x = MapClient.X+j;
						NewThing->y = MapClient.Y+i-1;
						List_AddEntry((NODE *)NewThing,List);
						Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
						Undo_StoreEntry(Child,(NODE *)NewThing,Type);
						}
					}
				*(TempPtr+j*MAP_CONTROLBYTES+0) = *(MapPtr+j*Map.Properties.Width*MAP_CONTROLBYTES+0);
				*(TempPtr+j*MAP_CONTROLBYTES+1) = *(MapPtr+j*Map.Properties.Width*MAP_CONTROLBYTES+1);
				*(TempPtr+j*MAP_CONTROLBYTES+2) = *(MapPtr+j*Map.Properties.Width*MAP_CONTROLBYTES+2);
				*(TempPtr+j*MAP_CONTROLBYTES+3) = *(MapPtr+j*Map.Properties.Width*MAP_CONTROLBYTES+3);
				}

			TempPtr -= MapClient.Width*MAP_CONTROLBYTES;
			MapPtr += MAP_CONTROLBYTES;

		} while (--i);

		}

	else

		{
		TempPtr = WorkPtr;

		do {

			//	1		4321
			//	2	->
			//	3
			//	4
			for (j = 0; j != MapClient.Width; j++)
				{
				for (Thing = (MAPTHING *)List.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
					{
					if ((Thing->x == MapClient.X+MapClient.Width-i)&&(Thing->y == MapClient.Y+j))
						{
						NODE		*List;
						MAPTHING	*NewThing;
						UINT		 Type;

						switch(Thing->node.type)
							{
							case THING_CREATURE:
								List = &MapCreatures;
								Type = UNDO_ADDCREATURE;
								break;
							case THING_DOOR:
								List = &MapDoors;
								Type = UNDO_ADDDOOR;
								break;
							case THING_TRAP:
								List = &MapTraps;
								Type = UNDO_ADDTRAP;
								break;
							case THING_OBJECT:
								List = &MapObjects;
								Type = UNDO_ADDOBJECT;
								break;
							case THING_MAGICALOBJECT:
								List = &MapMagicalObjects;
								Type = UNDO_ADDMAGIC;
								break;
							}
						NewThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
						if (!NewThing) continue;
						CopyMemory(NewThing,Thing,sizeof(MAPTHING));
						NewThing->x = MapClient.X+MapClient.Width-j-1;
						NewThing->y = MapClient.Y+MapClient.Width-i;
						List_AddEntry((NODE *)NewThing,List);
						Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
						Undo_StoreEntry(Child,(NODE *)NewThing,Type);
						}
					}
				*(TempPtr+j*MAP_CONTROLBYTES+0) = *(MapPtr+(MapClient.Width-j-1)*Map.Properties.Width*MAP_CONTROLBYTES+0);
				*(TempPtr+j*MAP_CONTROLBYTES+1) = *(MapPtr+(MapClient.Width-j-1)*Map.Properties.Width*MAP_CONTROLBYTES+1);
				*(TempPtr+j*MAP_CONTROLBYTES+2) = *(MapPtr+(MapClient.Width-j-1)*Map.Properties.Width*MAP_CONTROLBYTES+2);
				*(TempPtr+j*MAP_CONTROLBYTES+3) = *(MapPtr+(MapClient.Width-j-1)*Map.Properties.Width*MAP_CONTROLBYTES+3);
				}

			TempPtr += MapClient.Width*MAP_CONTROLBYTES;
			MapPtr += MAP_CONTROLBYTES;

		} while(--i);

		}

	//--- Copie la transformation ---

	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	TempPtr = WorkPtr;
	for (j = 0; j != MapClient.Height; j++)
		{
		LinePtr = MapPtr;
		TempLinePtr = TempPtr;
		for (i = 0; i != MapClient.Width; i++, LinePtr += MAP_CONTROLBYTES, TempLinePtr += MAP_CONTROLBYTES)
			{
			if ((*(LinePtr+0) == MapBadID)&&(*(LinePtr+3) == 0))
				continue;
			if ((*(TempLinePtr+0) == MapBadID)&&(*(TempLinePtr+3) == 0))
				{
				*(LinePtr+0) = Map_TranslateID(0x01,0x01,World);
				*(LinePtr+1) = 0x02;
				*(LinePtr+2) = 0x01;
				*(LinePtr+3) = 0x00;
				continue;
				}
			*(LinePtr+0) = *(TempLinePtr+0);
			*(LinePtr+1) = *(TempLinePtr+1);
			*(LinePtr+2) = *(TempLinePtr+2);
			*(LinePtr+3) = *(TempLinePtr+3);
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		TempPtr += MapClient.Width*MAP_CONTROLBYTES;
		}

	List_ReleaseMemory(&List);
	GlobalFree(WorkPtr);

	//--- Met à jour l'affichage ---

	Map_Lock(FALSE,FALSE);
	Minimap_Redraw(hWnd);
	Files_SaveUpdate();
	MapRect.x1 = MapClient.X;
	MapRect.y1 = MapClient.Y;
	MapRect.x2 = MapClient.X+MapClient.Width;
	MapRect.y2 = MapClient.Y+MapClient.Height;
	MapRect.x1--;						// Increase the...
	MapRect.y1--;						// update rectangle...
	MapRect.x2++;						// to update doors...
	MapRect.y2++;						// attributes.
	if (Map_RectangleToClient(hWnd,&MapRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}


// «»»» Symétrie ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Select_Symmetry(BOOL Vertical)
{
	MAPRECT		 MapRect;
	MAPCLIENT	 MapClient;
	MAPTHING	*Thing;
	NODE		 List;
	UNDO		*Undo;
	UNDO		*Child;
	BYTE		 MapBadID;
	BYTE		*MapPtr;
	BYTE		*WorkPtr;
	BYTE		*LinePtr;
	BYTE		*TempPtr;
	BYTE		*TempLinePtr;
	int		 i,j;

	if (MapCursorSel.hidden) return;

	Map_CorrectRect(&MapCursorSel,&MapClient);
	ZeroMemory(&List,sizeof(NODE));

	//--- Test si le rectangle est suffisamment grand ---

	if ( ((Vertical)&&(MapClient.Width < 2)) || ((!Vertical)&&(MapClient.Height < 2)) )
		{
		Misc_MessageBoxExt(hWnd,szSymmRequest,(char *)-1,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	//--- Procède à la transformation dans une zone temporaire ---

	WorkPtr = GlobalAlloc(GPTR,MapClient.Width*MapClient.Height*MAP_CONTROLBYTES);
	if (!WorkPtr)
		{
		Misc_PrintError(hWnd,szTransErr,NULL,MB_ICONHAND);
		return;
		}

	Undo = Undo_CreateNewEntry(UNDO_SYMMETRY,(Vertical)?szMapVertSym:szMapHorzSym);
	Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPRECT,szMapModify);
	if (!Undo_StoreMapRect(Child,&MapCursorSel,&Map))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		Undo = NULL;
		}
	Map_Lock(TRUE,FALSE);
	MapBadID = Map_TranslateID(0xFE,0x01,World);
	MapChanges++;

	for (j = 0; j != MapClient.Height; j++)
		for (i = 0; i != MapClient.Width; i++)
			{
			Things_CloneAtMapPos(&MapCreatures,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapDoors,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapTraps,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapObjects,MapClient.X+i,MapClient.Y+j,&List);
			Things_CloneAtMapPos(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&List);
			Things_RemoveAtMapPosEx(&MapCreatures,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapDoors,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapTraps,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapObjects,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&Undo);
			}

	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	if (!Vertical)
		{
		TempPtr = WorkPtr+MapClient.Width*(MapClient.Height-1)*MAP_CONTROLBYTES;

		for (i = 0; i != MapClient.Height; i++)
			{
			TempLinePtr = TempPtr;
			LinePtr = MapPtr;
			for (j = 0; j != MapClient.Width*MAP_CONTROLBYTES; j++)
				{
				for (Thing = (MAPTHING *)List.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
					{
					if ((Thing->x == MapClient.X+j)&&(Thing->y == MapClient.Y+i))
						{
						NODE		*List;
						MAPTHING	*NewThing;
						UINT		 Type;

						switch(Thing->node.type)
							{
							case THING_CREATURE:
								List = &MapCreatures;
								Type = UNDO_ADDCREATURE;
								break;
							case THING_DOOR:
								List = &MapDoors;
								Type = UNDO_ADDDOOR;
								break;
							case THING_TRAP:
								List = &MapTraps;
								Type = UNDO_ADDTRAP;
								break;
							case THING_OBJECT:
								List = &MapObjects;
								Type = UNDO_ADDOBJECT;
								break;
							case THING_MAGICALOBJECT:
								List = &MapMagicalObjects;
								Type = UNDO_ADDMAGIC;
								break;
							}
						NewThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
						if (!NewThing) continue;
						CopyMemory(NewThing,Thing,sizeof(MAPTHING));
						NewThing->y = MapClient.Y+MapClient.Height-i-1;
						List_AddEntry((NODE *)NewThing,List);
						Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
						Undo_StoreEntry(Child,(NODE *)NewThing,Type);
						}
					}
				*TempLinePtr++ = *LinePtr++;
				}
			TempPtr -= MapClient.Width*MAP_CONTROLBYTES;
			MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
			}
		}
	else
		{
		TempPtr = WorkPtr+(MapClient.Width-1)*MAP_CONTROLBYTES;

		for (i = 0; i != MapClient.Height; i++)
			{
			TempLinePtr = TempPtr;
			LinePtr = MapPtr;
			for (j = 0; j != MapClient.Width; j++)
				{
				for (Thing = (MAPTHING *)List.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
					{
					if ((Thing->x == MapClient.X+j)&&(Thing->y == MapClient.Y+i))
						{
						NODE		*List;
						MAPTHING	*NewThing;
						UINT		 Type;

						switch(Thing->node.type)
							{
							case THING_CREATURE:
								List = &MapCreatures;
								Type = UNDO_ADDCREATURE;
								break;
							case THING_DOOR:
								List = &MapDoors;
								Type = UNDO_ADDDOOR;
								break;
							case THING_TRAP:
								List = &MapTraps;
								Type = UNDO_ADDTRAP;
								break;
							case THING_OBJECT:
								List = &MapObjects;
								Type = UNDO_ADDOBJECT;
								break;
							case THING_MAGICALOBJECT:
								List = &MapMagicalObjects;
								Type = UNDO_ADDMAGIC;
								break;
							}
						NewThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
						if (!NewThing) continue;
						CopyMemory(NewThing,Thing,sizeof(MAPTHING));
						NewThing->x = MapClient.X+MapClient.Width-j-1;
						List_AddEntry((NODE *)NewThing,List);
						Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
						Undo_StoreEntry(Child,(NODE *)NewThing,Type);
						}
					}
				*(TempLinePtr+0) = *(LinePtr+0);
				*(TempLinePtr+1) = *(LinePtr+1);
				*(TempLinePtr+2) = *(LinePtr+2);
				*(TempLinePtr+3) = *(LinePtr+3);
				TempLinePtr -= MAP_CONTROLBYTES;
				LinePtr += MAP_CONTROLBYTES;
				}
			TempPtr += MapClient.Width*MAP_CONTROLBYTES;
			MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
			}
		}

	//--- Copie la transformation ---

	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	TempPtr = WorkPtr;
	for (j = 0; j != MapClient.Height; j++)
		{
		LinePtr = MapPtr;
		TempLinePtr = TempPtr;
		for (i = 0; i != MapClient.Width; i++, LinePtr += MAP_CONTROLBYTES, TempLinePtr += MAP_CONTROLBYTES)
			{
			if ((*(LinePtr+0) == MapBadID)&&(*(LinePtr+3) == 0))
				continue;
			if ((*(TempLinePtr+0) == MapBadID)&&(*(TempLinePtr+3) == 0))
				{
				*(LinePtr+0) = Map_TranslateID(0x01,0x01,World);
				*(LinePtr+1) = 0x02;
				*(LinePtr+2) = 0x01;
				*(LinePtr+3) = 0x00;
				continue;
				}
			*(LinePtr+0) = *(TempLinePtr+0);
			*(LinePtr+1) = *(TempLinePtr+1);
			*(LinePtr+2) = *(TempLinePtr+2);
			*(LinePtr+3) = *(TempLinePtr+3);
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		TempPtr += MapClient.Width*MAP_CONTROLBYTES;
		}

	List_ReleaseMemory(&List);
	GlobalFree(WorkPtr);

	//--- Met à jour l'affichage ---

	Map_Lock(FALSE,FALSE);
	Minimap_Redraw(hWnd);
	Files_SaveUpdate();
	MapRect.x1 = MapClient.X;
	MapRect.y1 = MapClient.Y;
	MapRect.x2 = MapClient.X+MapClient.Width;
	MapRect.y2 = MapClient.Y+MapClient.Height;
	MapRect.x1--;						// Increase the...
	MapRect.y1--;						// update rectangle...
	MapRect.x2++;						// to update doors...
	MapRect.y2++;						// attributes.
	if (Map_RectangleToClient(hWnd,&MapRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Copie et collage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie la zone sélectionnée ««««««««««««««««««««««««««««««««««««««»

void Select_Copy(BOOL Cut)
{
	MAPCLIENT	 MapClient;
	MAPTHING	*Thing;
	UNDO		*Undo;
	UNDO		*Child;
	DWORD		 MapID;
	BYTE		 MapBadID;
	BYTE		*CopyPtr;
	BYTE		*CopyTmpPtr;
	BYTE		*MapPtr;
	BYTE		*LinePtr;
	int		 i,j;

	if (MapCursorSel.hidden) return;
	if (!Select_CopyRequest()) return;

	Map_CorrectRect(&MapCursorSel,&MapClient);

	CopyPtr = GlobalAlloc(GPTR,MapClient.Width*MapClient.Height*MAP_CONTROLBYTES);
	if (!CopyPtr)
		{
		Misc_PrintError(hWnd,szCopyErr,NULL,MB_ICONHAND);
		return;
		}

	Select_ReleaseCopyMemory();
	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	CopyTmpPtr = CopyPtr;
	MapBadID = Map_TranslateID(0xFE,0x01,World);

	if (Cut)
		{
		switch(WorldItemID>>24)
			{
			case 0x0E:		// Coeur de donjon
			case 0x0C:		// Portail
			case 0x28:		// Portail des mercenaires
			case 0x1C:		// Portail 1
			case 0x21:		// Portail 3
			case 0x22:		// Portail 4
			case 0x25:		// Portail 6
				MapID = (DWORD)(Map_TranslateID((BYTE)Map.Properties.FillID,0x01,World))<<24;
				if (MapID == 0x23)
					MapID |= 0x00010100;
				else
					MapID |= 0x00020100;
				break;
			default:
				MapID = Map_TranslateWorldItem();
				break;
			}
		Map_Lock(TRUE,FALSE);
		Undo = Undo_CreateNewEntry(UNDO_CUT,szMapCut);
		Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPRECT,szMainMenuRectFill);
		if (!Undo_StoreMapRect(Child,&MapCursorSel,&Map))
			{
			Undo_PrintError(hWnd);
			Undo_DestroyEntry(Undo);
			Undo_Update();
			Undo = NULL;
			}
		MapChanges++;
		}

	for (j = 0; j != MapClient.Height; j++)
		{
		LinePtr = MapPtr;
		for (i = 0; i != MapClient.Width; i++, CopyTmpPtr += MAP_CONTROLBYTES, LinePtr += MAP_CONTROLBYTES)
			{
			*(CopyTmpPtr+0) = *(LinePtr+0);
			*(CopyTmpPtr+1) = *(LinePtr+1);
			*(CopyTmpPtr+2) = *(LinePtr+2);
			*(CopyTmpPtr+3) = *(LinePtr+3);
			if ((*(LinePtr+0) == MapBadID)&&(*(LinePtr+3) == 0))
				continue;

			Things_CloneAtMapPos(&MapCreatures,MapClient.X+i,MapClient.Y+j,&MapCopyThings);
			Things_CloneAtMapPos(&MapDoors,MapClient.X+i,MapClient.Y+j,&MapCopyThings);
			Things_CloneAtMapPos(&MapTraps,MapClient.X+i,MapClient.Y+j,&MapCopyThings);
			Things_CloneAtMapPos(&MapObjects,MapClient.X+i,MapClient.Y+j,&MapCopyThings);
			Things_CloneAtMapPos(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&MapCopyThings);

			if (Cut)
				{
				Things_RemoveAtMapPosEx(&MapCreatures,MapClient.X+i,MapClient.Y+j,&Undo);
				Things_RemoveAtMapPosEx(&MapDoors,MapClient.X+i,MapClient.Y+j,&Undo);
				Things_RemoveAtMapPosEx(&MapTraps,MapClient.X+i,MapClient.Y+j,&Undo);
				Things_RemoveAtMapPosEx(&MapObjects,MapClient.X+i,MapClient.Y+j,&Undo);
				Things_RemoveAtMapPosEx(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&Undo);
				*(LinePtr+0) = (BYTE)((MapID)>>24);
				*(LinePtr+1) = (BYTE)((MapID)>>16);
				*(LinePtr+2) = (BYTE)((MapID)>>8);
				*(LinePtr+3) = (BYTE)(MapID);
				}
			}
		MapPtr = MapPtr+Map.Properties.Width*MAP_CONTROLBYTES;
		}

	for (Thing = (MAPTHING *)MapCopyThings.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
		{
		Thing->x = Thing->x-MapClient.X;
		Thing->y = Thing->y-MapClient.Y;
		}

	MapCopy.Width = MapClient.Width;
	MapCopy.Height = MapClient.Height;
	MapCopy.Map = CopyPtr;
	Select_UpdateCopy();

	if (Cut)
		{
		MAPRECT	MapRect;

		Map_Lock(FALSE,FALSE);
		Minimap_Redraw(hWnd);
		Info_Update();
		Files_SaveUpdate();
		Map_CorrectRect(&MapCursorSel,&MapClient);
		MapRect.x1 = MapClient.X;
		MapRect.y1 = MapClient.Y;
		MapRect.x2 = MapClient.X+MapClient.Width;
		MapRect.y2 = MapClient.Y+MapClient.Height;
		MapRect.x1--;					// Increase the...
		MapRect.y1--;					// update rectangle...
		MapRect.x2++;					// to update doors...
		MapRect.y2++;					// attributes.
		if (Map_RectangleToClient(hWnd,&MapRect,&MapClient))
			{
			MapClient.Width += MapClient.X;
			MapClient.Height += MapClient.Y;
			InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
			UpdateWindow(hWnd);
			}
		}

	return;
}


// «»»» Colle la zone sélectionnée ««««««««««««««««««««««««««««««««««««««»

void Select_Paste(MAPCOPY *MapCopy, NODE *MapCopyThings)
{
	MAPCLIENT	 MapClient;
	MAPRECT		 MapRect;
	MAPTHING	*Thing;
	UNDO		*Undo;
	UNDO		*Child;
	BYTE		 MapBadID;
	BYTE		*CopyPtr;
	BYTE		*TmpPtr;
	BYTE		*MapPtr;
	BYTE		*LinePtr;
	int		 i,j;

	if (MapCursorSel.hidden) return;
	if (!MapCopy->Map) return;

	Map_CorrectRect(&MapCursorSel,&MapClient);
	Map_Lock(TRUE,FALSE);
	MapRect.x1 = MapClient.X;
	MapRect.y1 = MapClient.Y;
	MapRect.x2 = MapClient.X+MapCopy->Width;
	MapRect.y2 = MapClient.Y+MapCopy->Height;
	Undo = Undo_CreateNewEntry(UNDO_PASTE,szMapPaste);
	Child = Undo_CreateNewChild(&Undo,TRUE,UNDO_MAPRECT,szMainMenuRectFill);
	if (!Undo_StoreMapRect(Child,&MapRect,&Map))
		{
		Undo_PrintError(hWnd);
		Undo_DestroyEntry(Undo);
		Undo_Update();
		Undo = NULL;
		}
	MapChanges++;

	MapPtr = Map.Map+MapClient.X*MAP_CONTROLBYTES+MapClient.Y*Map.Properties.Width*MAP_CONTROLBYTES;
	CopyPtr = MapCopy->Map;
	MapBadID = Map_TranslateID(0xFE,0x01,World);

	for (j = 0; j != MapCopy->Height; j++)
		{
		if (MapClient.Y+j >= Map.Properties.Height)
			break;
		LinePtr = MapPtr;
		TmpPtr = CopyPtr;
		for (i = 0; i != MapCopy->Width; i++, LinePtr += MAP_CONTROLBYTES, TmpPtr += MAP_CONTROLBYTES)
			{
			if (MapClient.X+i >= Map.Properties.Width)
				break;
			if ((*(LinePtr+0) == MapBadID)&&(*(LinePtr+3) == 0))
				continue;

			Things_RemoveAtMapPosEx(&MapCreatures,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapDoors,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapTraps,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapObjects,MapClient.X+i,MapClient.Y+j,&Undo);
			Things_RemoveAtMapPosEx(&MapMagicalObjects,MapClient.X+i,MapClient.Y+j,&Undo);

			if ((*(TmpPtr+0) == MapBadID)&&(*(TmpPtr+3) == 0))
				{
				*(LinePtr+0) = Map_TranslateID(0x01,0x01,World);
				*(LinePtr+1) = 0x02;
				*(LinePtr+2) = 0x01;
				*(LinePtr+3) = 0x00;
				continue;
				}

			for (Thing = (MAPTHING *)MapCopyThings->next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				{
				if ((Thing->x == i)&&(Thing->y == j))
					{
					NODE		*List;
					MAPTHING	*NewThing;
					UINT		 Type;

					switch(Thing->node.type)
						{
						case THING_CREATURE:
							List = &MapCreatures;
							Type = UNDO_ADDCREATURE;
							break;
						case THING_DOOR:
							List = &MapDoors;
							Type = UNDO_ADDDOOR;
							break;
						case THING_TRAP:
							List = &MapTraps;
							Type = UNDO_ADDTRAP;
							break;
						case THING_OBJECT:
							List = &MapObjects;
							Type = UNDO_ADDOBJECT;
							break;
						case THING_MAGICALOBJECT:
							List = &MapMagicalObjects;
							Type = UNDO_ADDMAGIC;
							break;
						}
					NewThing = GlobalAlloc(GPTR,sizeof(MAPTHING));
					if (!NewThing) continue;
					CopyMemory(NewThing,Thing,sizeof(MAPTHING));
					NewThing->x += MapClient.X;
					NewThing->y += MapClient.Y;
					NewThing->uniqueid = 0;
					List_AddEntry((NODE *)NewThing,List);
					Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
					Undo_StoreEntry(Child,(NODE *)NewThing,Type);
					}
				}

			*(LinePtr+0) = *(TmpPtr+0);
			*(LinePtr+1) = *(TmpPtr+1);
			*(LinePtr+2) = *(TmpPtr+2);
			*(LinePtr+3) = *(TmpPtr+3);
			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		CopyPtr += MapCopy->Width*MAP_CONTROLBYTES;
		}

	Map_Lock(FALSE,FALSE);
	Minimap_Redraw(hWnd);
	Info_Update();
	Files_SaveUpdate();
	Map_CorrectRect(&MapCursorSel,&MapClient);
	MapRect.x1 = MapClient.X;
	MapRect.y1 = MapClient.Y;
	MapRect.x2 = MapClient.X+MapCopy->Width;
	MapRect.y2 = MapClient.Y+MapCopy->Height;
	MapRect.x1--;						// Increase the...
	MapRect.y1--;						// update rectangle...
	MapRect.x2++;						// to update doors...
	MapRect.y2++;						// attributes.
	if (Map_RectangleToClient(hWnd,&MapRect,&MapClient))
		{
		MapClient.Width += MapClient.X;
		MapClient.Height += MapClient.Y;
		InvalidateRect(hWnd,(RECT *)&MapClient,FALSE);
		UpdateWindow(hWnd);
		}

	return;
}


// «»»» Confirmation de la copie ««««««««««««««««««««««««««««««««««««««««»

int Select_CopyRequest()
{
	MESSAGEBOXSTRUCT	Request;

	if (!MapCopy.Map) return(1);

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = szMessageBoxExtYesNo;
	Request.windowtext = szCopyRequest;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = REQUEST_COPY;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_FOLLOWMOUSE;

	return(Misc_MessageBoxExtArgs(hWnd,&Request));
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
