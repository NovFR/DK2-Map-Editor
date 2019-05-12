
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'initialisations de la carte et des éléments y étant
	affichés.
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
extern ULONG		AutoSaveTimer;
extern DWORD		TickCount;
extern MAP		Map;
extern UINT		MapFileType;
extern LONG		MapChanges;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern MAPRECT		MapCursorSel;
extern MAPFIND		MapFind;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern MAPEDITSTATS	MapStats;
extern MAPOPTIONS	MapOptions[];
extern MAPCURSOR	MapCursor;
extern MAPCURSOR	MapCursorH;
extern MAPCURSOR	MapCursorX;
extern MAPCURSOR	MapCursorV;
extern HIMAGELIST	MapWorld;
extern HIMAGELIST	MapWorldGates;
extern HIMAGELIST	MapRoomsWalls;
extern HIMAGELIST	MapRoomsHeros;
extern HIMAGELIST	MapRoomsNeutral;
extern HIMAGELIST	MapRoomsKeeper1;
extern HIMAGELIST	MapRoomsKeeper2;
extern HIMAGELIST	MapRoomsKeeper3;
extern HIMAGELIST	MapRoomsKeeper4;
extern HIMAGELIST	MapRoomsKeeper5;
extern GLOBALVARS	MapGlobalVariables;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Nouvelle carte							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Map_New(HWND hWnd)
{
	MAPNEWCTX	*Context;
	int		 Result;

	if (!Misc_CheckChanges(hWnd,IDM_NEW)) return;

	Context = GlobalAlloc(GPTR,sizeof(MAPNEWCTX));
	if (!Context)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		return;
		}

	//--- Obtention des paramètres de la nouvelle carte ---

	strcpy(Context->NewProperties.Name,Map.Properties.Name);
	Context->NewProperties.Version = Map.Properties.Version;
	Context->NewProperties.Players = Map.Properties.Players;
	Context->NewProperties.Width = Map.Properties.Width;
	Context->NewProperties.Height = Map.Properties.Height;
	Context->NewProperties.FillID = Map.Properties.FillID;
	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6900),hWnd,Map_NewProc,(LPARAM)Context);
	if ((Result == 0)||(Result == -1))
		{
		if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		GlobalFree(Context);
		return;
		}

	//--- Initialisation de la nouvelle carte ---

	Map_Lock(TRUE,TRUE);
	if (!Map_CreateNew(hWnd,&Context->NewProperties)) goto Done;

	Debug_ResetVersions(TRUE);
	Undo_DestroyAllEntries();
	List_ReleaseMemory(&MapFind.Results);
	List_ReleaseMemory(&MapCreatures);
	List_ReleaseMemory(&MapDoors);
	List_ReleaseMemory(&MapTraps);
	List_ReleaseMemory(&MapObjects);
	List_ReleaseMemory(&MapMagicalObjects);
	List_ReleaseMemory(&MapHerosBands);
	List_ReleaseMemory(&MapAreas);
	List_ReleaseMemory(&MapGates);
	List_ReleaseMemory(&MapEffects);
	Triggers_ReleaseMemory();
	Map_ResetStats(&MapStats);
	GetLocalTime(&MapStats.CreatedTime);
	AutoSaveTimer = 0;
	MapChanges = 0;
	MapFileType = 0;
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

Done:	Map_Lock(FALSE,TRUE);
	Info_Update();
	GlobalFree(Context);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Map_NewProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPNEWCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Map_NewInit(hDlg,(MAPNEWCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPNEWCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 117:
							Misc_SetUpDownInt(hDlg,107,(NM_UPDOWN *)lParam);
							break;
						case 119:
							Misc_SetUpDownInt(hDlg,109,(NM_UPDOWN *)lParam);
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
						case 107:
							lData = GetDlgItemInt(hDlg,107,NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,107,0,FALSE);
								break;
								}
							if (lData > 300)
								{
								SetDlgItemInt(hDlg,107,300,FALSE);
								break;
								}
							if (IsDlgButtonChecked(hDlg,110) == BST_CHECKED) SetDlgItemInt(hDlg,109,GetDlgItemInt(hDlg,107,NULL,FALSE),FALSE);
							break;
						case 109:
							lData = GetDlgItemInt(hDlg,109,NULL,FALSE);
							if (lData < 0)
								{
								SetDlgItemInt(hDlg,109,0,FALSE);
								break;
								}
							if (lData > 300)
								{
								SetDlgItemInt(hDlg,109,300,FALSE);
								break;
								}
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 110:
							if (IsDlgButtonChecked(hDlg,110) == BST_CHECKED)
								{
								SetDlgItemInt(hDlg,109,GetDlgItemInt(hDlg,107,NULL,FALSE),FALSE);
								EnableWindow(GetDlgItem(hDlg,109),FALSE);
								EnableWindow(GetDlgItem(hDlg,119),FALSE);
								}
							else
								{
								EnableWindow(GetDlgItem(hDlg,109),TRUE);
								EnableWindow(GetDlgItem(hDlg,119),TRUE);
								}
							break;
						case IDOK:
							if (!Map_NewReset(hDlg,TRUE,Context)) break;
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Map_NewReset(hDlg,FALSE,Context);
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations de la boîte de dialogue «««««««««««««««««««««««««»

void Map_NewInit(HWND hDlg, MAPNEWCTX *Context)
{
	LV_COLUMN	Column;
	LV_ITEM		Item;
	RECT		ClientRect;
	LONG		X,Y;
	char		Players[4];

	//--- Initialisation de la fenêtre ---

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	//--- Initialisation des paramètres ---

	Misc_CreateUpDownButton(hDlg,107,117);
	Misc_CreateUpDownButton(hDlg,109,119);
	SetDlgItemText(hDlg,102,Context->NewProperties.Name);
	SetDlgItemInt(hDlg,107,Context->NewProperties.Width,FALSE);
	SetDlgItemInt(hDlg,109,Context->NewProperties.Height,FALSE);
	SendDlgItemMessage(hDlg,102,EM_LIMITTEXT,(WPARAM)63,(LPARAM)0);
	SendDlgItemMessage(hDlg,107,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,109,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	if (Context->NewProperties.Width == Context->NewProperties.Height)
		{
		EnableWindow(GetDlgItem(hDlg,109),FALSE);
		EnableWindow(GetDlgItem(hDlg,119),FALSE);
		CheckDlgButton(hDlg,110,BST_CHECKED);
		}

	//--- Initialisation des informations (Version & joueurs) ---

	for (X = 0; Versions[X] != NULL; X++)
		SendDlgItemMessage(hDlg,104,CB_ADDSTRING,(WPARAM)0,(LPARAM)Versions[++X]);
	for (X = 0, Y = 0; Versions[X] != NULL; X++, Y++)
		{
		if ((BYTE)Versions[X++] == Context->NewProperties.Version)
			{
			SendDlgItemMessage(hDlg,104,CB_SETCURSEL,(WPARAM)Y,(LPARAM)0);
			break;
			}
		}

	for (X = 1; X != MAP_MAXPLAYERS-1; X++)
		{
		wsprintf(Players,szNumber,X);
		SendDlgItemMessage(hDlg,113,CB_ADDSTRING,(WPARAM)0,(LPARAM)Players);
		}
	SendDlgItemMessage(hDlg,113,CB_SETCURSEL,(WPARAM)Context->NewProperties.Players-1,(LPARAM)0);

	//--- Initialisation de la liste des éléments ---

	GetWindowRect(GetDlgItem(hDlg,111),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	Context->NewList = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_NOSORTHEADER|LVS_NOCOLUMNHEADER|LVS_SHAREIMAGELISTS,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Context->NewList) goto Error_0;
	if (!Misc_CreateImageList(NewMap,&Context->NewImages,32,32,FALSE)) goto Error_0;
	SendMessage(Context->NewList,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->NewImages);

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-8-1-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->NewList,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_1;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iSubItem = 0;
	for (X = 0; NewMap[X].Id != 0; X++)
		{
		Item.state = (NewMap[X].Id == Context->NewProperties.FillID)?LVIS_SELECTED:0;
		Item.stateMask = LVIS_SELECTED;
		Item.iItem = X;
		Item.pszText = NewMap[X].Name;
		Item.iImage = X;
		Item.lParam = NewMap[X].Id;
		if (SendMessage(Context->NewList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
		if (NewMap[X].Id == Context->NewProperties.FillID) SendMessage(Context->NewList,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)TRUE);
		}

	return;

//--- Erreurs ---

Error_1:SendMessage(Context->NewList,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	Misc_DestroyImageList(&Context->NewImages);
Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Restitutions des données allouées «««««««««««««««««««««««««««««««»

int Map_NewReset(HWND hDlg, BOOL Save, MAPNEWCTX *Context)
{
	if (Save)
		{
		LV_ITEM	Item;

		GetDlgItemText(hDlg,102,Context->NewProperties.Name,64);
		Context->NewProperties.Width = GetDlgItemInt(hDlg,107,NULL,FALSE);
		if (IsDlgButtonChecked(hDlg,110) == BST_UNCHECKED) Context->NewProperties.Height = GetDlgItemInt(hDlg,109,NULL,FALSE);
		else Context->NewProperties.Height = Context->NewProperties.Width;
		Context->NewProperties.FillID = MAP_DEFAULTID;
		if (Context->NewProperties.Width < 7) Context->NewProperties.Width = 7;
		if (Context->NewProperties.Height < 7) Context->NewProperties.Height = 7;

		switch(SendDlgItemMessage(hDlg,104,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))
			{
			case 1:	Context->NewProperties.Version = 16;
				break;
			case 2:	Context->NewProperties.Version = 17;
				break;
			default:Context->NewProperties.Version = 13;
				break;
			}
		Context->NewProperties.Players = SendDlgItemMessage(hDlg,113,CB_GETCURSEL,(WPARAM)0,(LPARAM)0)+1;

		Item.mask = LVIF_PARAM;
		Item.iItem = -1;
		Item.iItem = SendMessage(Context->NewList,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
		if (Item.iItem == -1) Item.iItem = SendMessage(Context->NewList,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_FOCUSED,0));
		if (Item.iItem != -1)
			if (SendMessage(Context->NewList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item))
				if (Item.lParam)
					Context->NewProperties.FillID = Item.lParam;

		if (Context->NewProperties.Width*Context->NewProperties.Height > 120*120)
			{
			MESSAGEBOXSTRUCT	MessageBox;
			int			Result;

			ZeroMemory(&MessageBox,sizeof(MESSAGEBOXSTRUCT));
			MessageBox.gadgets = szMessageBoxExtYesNo;
			MessageBox.windowtext = szMapSizeTooBig;
			MessageBox.windowtitle = (char *)-1;
			MessageBox.requestbits = &Config.RequestsFlags;
			MessageBox.statebit = REQUEST_BIGMAP;
			MessageBox.flags = MBF_AUTOREPLY;
			MessageBox.defaultgadget = 1;
			MessageBox.defaulticon = MB_ICONINFORMATION;
			MessageBox.windowpos = MBP_FOLLOWMOUSE;
			Result = Misc_MessageBoxExtArgs(hDlg,&MessageBox);
			if (!Result) return(0);
			}
		}

	SendMessage(Context->NewList,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	Misc_DestroyImageList(&Context->NewImages);
	return(1);
}


// «»»» Changement du titre de la fenêtre «««««««««««««««««««««««««««««««»

void Map_NewTitle(HWND hWnd)
{
	char	*Title;
	char	*File;
	ULONG	 Len;

	if ((Config.UseFilename)&&(Config.MapPath[0]))
		{
		if (Config.ShowFullName)
			File = Config.MapPath;
		else
			{
			File = strrchr(Config.MapPath,'\\');
			if (!File) File = Config.MapPath;
			else File++;
			}
		}
	else
		File = Map.Properties.Name;

	Len = strlen(szWindowTitle)+strlen(szMapSeparator)+strlen(File);
	Title = GlobalAlloc(GPTR,Len+1);
	if (!Title)
		{
		SetWindowText(hWnd,szWindowTitle);
		return;
		}

	strcpy(Title,szWindowTitle);
	strcat(Title,szMapSeparator);
	strcat(Title,File);
	SetWindowText(hWnd,Title);
	GlobalFree(Title);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'initialisations					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation de la carte ««««««««««««««««««««««««««««««««««««««»

int Map_CreateNew(HWND hWnd, MAPPROPERTIES *NewProperties)
{
	MAP	*TempMap;
	LONG	 Count;

	TempMap = GlobalAlloc(GPTR,sizeof(MAP));
	if (!TempMap) goto Error_0;

	Properties_MapResetOptions(TempMap,NULL);
	TempMap->Properties.Players = NewProperties->Players;
	TempMap->Properties.Version = NewProperties->Version;
	TempMap->Properties.Width = NewProperties->Width;
	TempMap->Properties.Height = NewProperties->Height;
	TempMap->Properties.FillID = NewProperties->FillID;
	strcpy(TempMap->Properties.Name,NewProperties->Name);
	CopyMemory(NewProperties,&TempMap->Properties,sizeof(MAPPROPERTIES));

	// Création des autres données par défaut
	//
	if (!Map_CreateDefaultMap(TempMap,NewProperties)) goto Error_1;

	// Places les options par défaut
	//
	for (Count = 0; Count != MAP_MAXPLAYERS; Count++)
		Properties_MapOptionsToDefault(&MapOptions[Count],Count+1);

	Properties_GlobalReset(&MapGlobalVariables,-1,MGRF_ALL);
	Map_Destroy(&Map);
	CopyMemory(&Map,TempMap,sizeof(MAP));
	GlobalFree(TempMap);
	return(1);

//--- Erreur d'initialisation ---

Error_1:Map_Destroy(TempMap);
	GlobalFree(TempMap);
Error_0:Misc_PrintError(hWnd,szMapMemoryErr,NULL,MB_ICONHAND);
	return(0);
}

//--- Initialisation des données que la carte doit contenir par défaut ---

int Map_CreateDefaultMap(MAP *TempMap, MAPPROPERTIES *NewProperties)
{
	BYTE	*MapPtr;
	BYTE	 FillID;
	BYTE	 FillTable;
	LONG	 Count;

	//--- Alloue la mémoire nécessaire à la carte ---

	ZeroMemory(TempMap,sizeof(MAP));
	TempMap->Map = GlobalAlloc(GPTR,NewProperties->Width*MAP_CONTROLBYTES*NewProperties->Height);
	if (!TempMap->Map) return(0);

	CopyMemory(&TempMap->Properties,NewProperties,sizeof(MAPPROPERTIES));
	if (!Map_CreateDisplayContext(TempMap)) return(0);

	//--- Initialise le contenu de la carte ---

	FillTable = 0x00;
	FillID = Map_TranslateID((BYTE)(NewProperties->FillID>>24),(BYTE)(NewProperties->FillID>>8),World);
	if (FillID == (BYTE)-1)
		{
		FillID = Map_TranslateIDEx((BYTE)(NewProperties->FillID>>24),(BYTE)(NewProperties->FillID>>16),(BYTE)(NewProperties->FillID>>8),Walls);
		FillTable = 0x03;
		}

	for (MapPtr = TempMap->Map, Count = 0; Count != NewProperties->Width*NewProperties->Height; Count++)
		{
		*(MapPtr+0) = FillID;
		*(MapPtr+1) = (BYTE)(NewProperties->FillID>>16);
		*(MapPtr+2) = (BYTE)(NewProperties->FillID>>8);
		*(MapPtr+3) = FillTable;
		MapPtr += MAP_CONTROLBYTES;
		}

	Map_CreateLimitWall(TempMap);
	return(1);
}

//--- Création d'un contour indestructible ---

void Map_CreateLimitWall(MAP *Map)
{
	BYTE	*MapPtr;
	BYTE	 FillID;
	LONG	 Count;

	FillID = Map_TranslateID(0xFE,0x01,World);

	for (MapPtr = Map->Map, Count = 0; Count != Map->Properties.Width; Count++)
		{
		*(MapPtr+0) = FillID;
		*(MapPtr+1) = 0x02;
		*(MapPtr+2) = 0x01;
		*(MapPtr+3) = 0x00;
		MapPtr += MAP_CONTROLBYTES;
		}
	for (MapPtr = Map->Map+Map->Properties.Width*MAP_CONTROLBYTES*(Map->Properties.Height-1), Count = 0; Count != Map->Properties.Width; Count++)
		{
		*(MapPtr+0) = FillID;
		*(MapPtr+1) = 0x02;
		*(MapPtr+2) = 0x01;
		*(MapPtr+3) = 0x00;
		MapPtr += MAP_CONTROLBYTES;
		}

	for (MapPtr = Map->Map, Count = 0; Count != Map->Properties.Height; Count++)
		{
		*(MapPtr+0) = FillID;
		*(MapPtr+1) = 0x02;
		*(MapPtr+2) = 0x01;
		*(MapPtr+3) = 0x00;
		*(MapPtr+Map->Properties.Width*MAP_CONTROLBYTES-MAP_CONTROLBYTES+0) = FillID;
		*(MapPtr+Map->Properties.Width*MAP_CONTROLBYTES-MAP_CONTROLBYTES+1) = 0x02;
		*(MapPtr+Map->Properties.Width*MAP_CONTROLBYTES-MAP_CONTROLBYTES+2) = 0x01;
		*(MapPtr+Map->Properties.Width*MAP_CONTROLBYTES-MAP_CONTROLBYTES+3) = 0x00;
		MapPtr += Map->Properties.Width*MAP_CONTROLBYTES;
		}

	return;
}

//--- Création des données liées à l'affichage ---

int Map_CreateDisplayContext(MAP *Map)
{
	HDC	hDC;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		Map->hDC = CreateCompatibleDC(hDC);
		Map->hBitmap = CreateCompatibleBitmap(hDC,Map->Properties.Width,Map->Properties.Height);
		Map->hBrush = CreateSolidBrush(0x00FFFFFF);
		Map->hPen = CreatePen(PS_SOLID,0,0x00FFFFFF);
		ReleaseDC(hWnd,hDC);
		if (!Map->hDC || !Map->hBitmap || !Map->hBrush || !Map->hPen) return(0);
		Map->hOldBitmap = SelectObject(Map->hDC,Map->hBitmap);
		Map->hOldBrush = SelectObject(Map->hDC,Map->hBrush);
		Map->hOldPen = SelectObject(Map->hDC,Map->hPen);
		if (!Map->hOldBitmap || !Map->hOldBrush || !Map->hOldPen) return(0);
		return(1);
		}

	return(0);
}


// «»»» Destruction de la carte «««««««««««««««««««««««««««««««««««««««««»

void Map_Destroy(MAP *Map)
{
	if (!Map) return;

	if (Map->Map) GlobalFree(Map->Map);
	if (Map->hOldPen) SelectObject(Map->hDC,Map->hOldPen);
	if (Map->hOldBrush) SelectObject(Map->hDC,Map->hOldBrush);
	if (Map->hOldBitmap) SelectObject(Map->hDC,Map->hOldBitmap);
	if (Map->hPen) DeleteObject(Map->hPen);
	if (Map->hBrush) DeleteObject(Map->hBrush);
	if (Map->hBitmap) DeleteObject(Map->hBitmap);
	if (Map->hDC) DeleteDC(Map->hDC);

	Map->Map = NULL;
	Map->hOldPen = NULL;
	Map->hOldBrush = NULL;
	Map->hOldBitmap = NULL;
	Map->hPen = NULL;
	Map->hBrush = NULL;
	Map->hBitmap = NULL;
	Map->hDC = NULL;
	return;
}


// «»»» Changement des dimensions «««««««««««««««««««««««««««««««««««««««»

MAP* Map_PrepareResize(LONG NewWidth, LONG NewHeight, MAP *Map)
{
	MAPPROPERTIES	*NewProperties;
	MAP		*Result;

	NewProperties = GlobalAlloc(GPTR,sizeof(MAPPROPERTIES));
	if (!NewProperties) return(NULL);
	Result = GlobalAlloc(GPTR,sizeof(MAP));
	if (!Result) goto Error;

	CopyMemory(NewProperties,&Map->Properties,sizeof(MAPPROPERTIES));
	NewProperties->Width = NewWidth;
	NewProperties->Height = NewHeight;
	if (!Map_CreateDefaultMap(Result,NewProperties)) goto Error;

	GlobalFree(NewProperties);
	return(Result);

//--- Erreurs ---

Error:	GlobalFree(NewProperties);
	Map_Destroy(Result);
	GlobalFree(Result);
	return(NULL);
}


// «»»» Initialisation du zoom ««««««««««««««««««««««««««««««««««««««««««»

int Map_InitialiseZoom(HWND hWnd, LONG ZoomX, LONG ZoomY)
{
	HIMAGELIST	TmpMapWorld = NULL;
	HIMAGELIST	TmpMapWorldGates = NULL;
	HIMAGELIST	TmpMapRoomsWalls = NULL;
	HIMAGELIST	TmpMapRoomsHeros = NULL;
	HIMAGELIST	TmpMapRoomsNeutral = NULL;
	HIMAGELIST	TmpMapRoomsKeeper1 = NULL;
	HIMAGELIST	TmpMapRoomsKeeper2 = NULL;
	HIMAGELIST	TmpMapRoomsKeeper3 = NULL;
	HIMAGELIST	TmpMapRoomsKeeper4 = NULL;
	HIMAGELIST	TmpMapRoomsKeeper5 = NULL;
	MAPCURSOR	TmpMapCursor;
	MAPCURSOR	TmpMapCursorV;
	MAPCURSOR	TmpMapCursorH;
	MAPCURSOR	TmpMapCursorX;
	DWORD		LastError;

	ZeroMemory(&TmpMapCursor,sizeof(MAPCURSOR));
	ZeroMemory(&TmpMapCursorV,sizeof(MAPCURSOR));
	ZeroMemory(&TmpMapCursorH,sizeof(MAPCURSOR));
	ZeroMemory(&TmpMapCursorX,sizeof(MAPCURSOR));
	if (!Misc_CreateImageList(World,&TmpMapWorld,ZoomX,ZoomY,FALSE)) goto Error;
	if (!Misc_CreateImageList(Gates,&TmpMapWorldGates,ZoomX,ZoomY,FALSE)) goto Error;
	if (!Misc_CreateImageList(Walls,&TmpMapRoomsWalls,ZoomX,ZoomY,FALSE)) goto Error;
	if (!Misc_CreateImageList(Rooms,&TmpMapRoomsHeros,ZoomX,ZoomY,FALSE)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsNeutral,ZoomX,ZoomY,1)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsKeeper1,ZoomX,ZoomY,2)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsKeeper2,ZoomX,ZoomY,3)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsKeeper3,ZoomX,ZoomY,4)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsKeeper4,ZoomX,ZoomY,5)) goto Error;
	if (!Misc_CreatePlayerImageList(Rooms,&TmpMapRoomsKeeper5,ZoomX,ZoomY,6)) goto Error;
	if (!Map_CreateCursor(hWnd,&TmpMapCursor,ZoomX,ZoomY,0x00FF0000)) goto Error;
	if (!Map_CreateCursor(hWnd,&TmpMapCursorV,ZoomX,ZoomY,0x0000FF00)) goto Error;
	if (!Map_CreateCursor(hWnd,&TmpMapCursorH,ZoomX,ZoomY,0x00FFFF00)) goto Error;
	if (!Map_CreateCursor(hWnd,&TmpMapCursorX,ZoomX,ZoomY,0x000000FF)) goto Error;

	Map_ResetZoom();

	CopyMemory(&MapCursor,&TmpMapCursor,sizeof(MAPCURSOR));
	CopyMemory(&MapCursorV,&TmpMapCursorV,sizeof(MAPCURSOR));
	CopyMemory(&MapCursorH,&TmpMapCursorH,sizeof(MAPCURSOR));
	CopyMemory(&MapCursorX,&TmpMapCursorX,sizeof(MAPCURSOR));
	MapZoomCX = ZoomX;
	MapZoomCY = ZoomY;
	MapWorld = TmpMapWorld;
	MapWorldGates = TmpMapWorldGates;
	MapRoomsWalls = TmpMapRoomsWalls;
	MapRoomsHeros = TmpMapRoomsHeros;
	MapRoomsNeutral = TmpMapRoomsNeutral;
	MapRoomsKeeper1 = TmpMapRoomsKeeper1;
	MapRoomsKeeper2 = TmpMapRoomsKeeper2;
	MapRoomsKeeper3 = TmpMapRoomsKeeper3;
	MapRoomsKeeper4 = TmpMapRoomsKeeper4;
	MapRoomsKeeper5 = TmpMapRoomsKeeper5;

	return(1);

//--- Erreur d'initialisation ---

Error:	LastError = GetLastError();
	Map_DestroyCursor(&TmpMapCursor);
	Map_DestroyCursor(&TmpMapCursorV);
	Map_DestroyCursor(&TmpMapCursorH);
	Map_DestroyCursor(&TmpMapCursorX);
	Misc_DestroyImageList(&TmpMapWorld);
	Misc_DestroyImageList(&TmpMapWorldGates);
	Misc_DestroyImageList(&TmpMapRoomsWalls);
	Misc_DestroyImageList(&TmpMapRoomsHeros);
	Misc_DestroyImageList(&TmpMapRoomsNeutral);
	Misc_DestroyImageList(&TmpMapRoomsKeeper1);
	Misc_DestroyImageList(&TmpMapRoomsKeeper2);
	Misc_DestroyImageList(&TmpMapRoomsKeeper3);
	Misc_DestroyImageList(&TmpMapRoomsKeeper4);
	Misc_DestroyImageList(&TmpMapRoomsKeeper5);
	SetLastError(LastError);
	Misc_PrintError(hWnd,szMapZoomErr,NULL,MB_ICONHAND);
	return(0);
}


// «»»» Suppression des données initialisées par le zoom ««««««««««««««««»

void Map_ResetZoom()
{
	Map_DestroyCursor(&MapCursor);
	Map_DestroyCursor(&MapCursorV);
	Map_DestroyCursor(&MapCursorH);
	Map_DestroyCursor(&MapCursorX);
	Misc_DestroyImageList(&MapWorld);
	Misc_DestroyImageList(&MapWorldGates);
	Misc_DestroyImageList(&MapRoomsWalls);
	Misc_DestroyImageList(&MapRoomsHeros);
	Misc_DestroyImageList(&MapRoomsNeutral);
	Misc_DestroyImageList(&MapRoomsKeeper1);
	Misc_DestroyImageList(&MapRoomsKeeper2);
	Misc_DestroyImageList(&MapRoomsKeeper3);
	Misc_DestroyImageList(&MapRoomsKeeper4);
	Misc_DestroyImageList(&MapRoomsKeeper5);
	return;
}


// «»»» Initialisation du curseur «««««««««««««««««««««««««««««««««««««««»

int Map_CreateCursor(HWND hWnd, MAPCURSOR *Cursor, LONG Width, LONG Height, COLORREF Color)
{
	HDC	hDC;
	long	i,j;

	hDC = GetDC(hWnd);
	if (!hDC) goto Error_0;
	Cursor->hdc = CreateCompatibleDC(hDC);
	if (!Cursor->hdc) goto Error_1;
	Cursor->hbitmap = CreateCompatibleBitmap(hDC,Width,Height);
	if (!Cursor->hbitmap) goto Error_2;
	Cursor->holdbitmap = SelectObject(Cursor->hdc,Cursor->hbitmap);
	if (!Cursor->holdbitmap) goto Error_3;

	for (i = 0; i != Width; i++)
		for (j = 0; j != Height; j++)
			SetPixelV(Cursor->hdc,i,j,Color);

	ReleaseDC(hWnd,hDC);
	return(1);

//--- Erreurs d'initialisation ---

Error_3:DeleteObject(Cursor->hbitmap);
Error_2:DeleteDC(Cursor->hdc);
Error_1:ReleaseDC(hWnd,hDC);
Error_0:Cursor->hdc = NULL;
	return(0);
}


// «»»» Réinitialisation d'un rectangle coloré ««««««««««««««««««««««««««»

void Map_DestroyCursor(MAPCURSOR *Cursor)
{
	if (Cursor->hdc)
		{
		SelectObject(Cursor->hdc,Cursor->holdbitmap);
		DeleteObject(Cursor->hbitmap);
		DeleteDC(Cursor->hdc);
		}
	Cursor->hdc = NULL;
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
