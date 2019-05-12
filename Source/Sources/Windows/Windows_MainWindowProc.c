
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine principale de gestion des messages.
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
extern HWND		hStatusWnd;
extern HWND		hToolbar;
extern HWND		hMapZoom;
extern HWND		hMinimap;
extern HWND		hInfo;
extern HMENU		hMenu;
extern FONTINFO		Font;
extern FONTINFO		FixedFont;
extern HBITMAP		hSquare32;
extern HIMAGELIST	hWalls;
extern HIMAGELIST	hWorld;
extern HIMAGELIST	hRooms;
extern HIMAGELIST	hGates;
extern HIMAGELIST	hDrawingTools;
extern HIMAGELIST	hMiscTools;
extern MAPOPTIONS	MapOptions[];
extern MAP		Map;
extern MAPFIND		MapFind;
extern LONG		MapChanges;
extern UINT		MapFileType;
extern MAPTHING		MapImmCreature;
extern MAPTHING		MapImmDoor;
extern MAPTHING		MapImmTrap;
extern MAPTHING		MapImmObject;
extern MAPTHING		MapImmMagicalObject;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapCopyThings;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern UINT		MapCursorMode;
extern MAPRECT		MapCursorSel;
extern MAPRECT		MapCursorHigh;
extern MAPCURSOR	MapCursor;
extern MAPEDITSTATS	MapStats;
extern MAPCOPY		MapCopy;
extern GLOBALVARS	MapGlobalVariables;
extern DWORD		ToolsItemID;
extern BOOL		MinimapIsMoving;
extern NODE		RecentFiles;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Lecture des messages						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG CALLBACK Window_Proc(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	LONG		WindowID;

	if (uMsgId == WM_CREATE)
		{
		SetWindowLong(hWnd,GWL_USERDATA,(LONG)(((CREATESTRUCT *)lParam)->lpCreateParams));
		switch((LONG)(((CREATESTRUCT *)lParam)->lpCreateParams))
			{
			case WINDOW_MAIN:
				return(Window_Create(hWnd));
			case WINDOW_SPLASH:
				return(Splash_Create(hWnd));
			case WINDOW_COMPILER:
				return(Compiler_Create(hWnd));
			case WINDOW_FULLMAP:
				return(Fullmap_Create(hWnd));
			case WINDOW_THUMBNAIL:
				return(Thumb_Create(hWnd));
			}
		return(-1);
		}


	WindowID = GetWindowLong(hWnd,GWL_USERDATA);
	switch(WindowID)
		{
		case WINDOW_MAIN:
			return(Window_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_SPLASH:
			return(Splash_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_COMPILER:
			return(Compiler_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_FULLMAP:
			return(Fullmap_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_THUMBNAIL:
			return(Thumb_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		}

	return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
}


// «»»» Lecture des messages provenant de la fenêtre principale «««««««««»

LONG Window_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	HDC		hDC;
	LONG		DX,DY;
	RECT		MapRect;
	PAINTSTRUCT	paintStruct;

	switch(uMsgId)
		{
		case WM_SIZING:
			Window_Sizing((RECT *)lParam);
			break;

		case WM_SIZE:
			Window_Resize(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_TIMER:
			Status_DisplayTimer();
			Files_AutoSave();
			break;

		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			hDC = BeginPaint(hWnd,&paintStruct);
			Map_Paint(hWnd,hDC);
			Map_GetMapArea(hWnd,&MapRect);
			Map_GetMapSquares(NULL,&MapRect,&DX,&DY);
			Things_DrawDoors(hDC,&MapRect,DX,DY,MapStartX,MapStartY);
			Things_DrawTraps(hDC,&MapRect,DX,DY,MapStartX,MapStartY);
			Things_DrawMagicalObjects(hDC,&MapRect,DX,DY,MapStartX,MapStartY);
			Things_DrawObjects(hDC,&MapRect,DX,DY,MapStartX,MapStartY);
			Things_DrawCreatures(hDC,&MapRect,DX,DY,MapStartX,MapStartY);
			Things_DrawEffects(hWnd,hDC);
			Things_DrawGates(hWnd,hDC);
			Things_DrawAreas(hWnd,hDC);
			Things_DrawPaths(hWnd,hDC);
			Map_DrawCameras(hWnd,hDC);
			Map_DrawRectangle(hWnd,hDC,&MapCursorSel);
			Mouse_Paint(hWnd,hDC);
			Map_PaintCursor(hWnd,hDC,&MapCursorHigh,&MapCursor,FALSE);
			Window_Paint(hWnd,hDC);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_PAINTMAP:
			Map_Refresh();
			break;

		case WM_KILLTHING:
			Mouse_RemoveThingUnderCursor(hWnd,lParam);
			break;

		case WM_ACTIVATE:
			if ((LOWORD(wParam) == WA_INACTIVE)||(HIWORD(wParam)))
				{
				if (MapCursorMode)
					{
					ReleaseCapture();
					MapCursorMode = 0;
					Select_Update();
					break;
					}
				if (MinimapIsMoving)
					{
					ReleaseCapture();
					MinimapIsMoving = FALSE;
					break;
					}
				}
			break;

		case WM_MOUSEMOVE:
			Mouse_Moved(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_LBUTTONDOWN:
			Mouse_LeftButtonDown(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			Mouse_LeftButtonUp(hWnd,LOWORD(lParam),HIWORD(lParam),wParam&MK_SHIFT);
			break;

		case WM_RBUTTONDOWN:
			Mouse_RightButtonDown(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			Mouse_RightButtonUp(hWnd,LOWORD(lParam),HIWORD(lParam),wParam&MK_SHIFT,wParam&MK_CONTROL);
			break;

		case WM_MEASUREITEM:
			Misc_MeasureMenuItem(hWnd,(UINT)wParam,(MEASUREITEMSTRUCT *)lParam);
			break;

		case WM_DRAWITEM:
			Window_DrawItems((DRAWITEMSTRUCT *)lParam);
			break;

		case WM_HSCROLL:
			Map_ScrollHorizontally(hWnd,LOWORD(wParam),HIWORD(wParam),TRUE);
			break;

		case WM_VSCROLL:
			Map_ScrollVertically(hWnd,LOWORD(wParam),HIWORD(wParam),TRUE);
			break;

		case WM_COMMAND:
			Window_Command(HIWORD(wParam),LOWORD(wParam),(HWND)lParam);
			break;

		case WM_CONTEXTMENU:
			Window_ContextMenu(LOWORD(lParam),HIWORD(lParam),(HWND)wParam);
			break;

		case WM_DISPLAYCHANGE:
			Window_AdjustSize(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_ENDSESSION:
			DestroyWindow(hWnd);
			break;

		case WM_CLOSE:
			if (!Misc_CheckChanges(hWnd,IDM_QUIT)) break;
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			Sound_Stop();
			Disk_DestroyWindow(FALSE);
			Fullmap_DestroyWindow();
			Compiler_DestroyWindow();
			Thumb_DestroyWindow();
			Splash_DestroyWindow();
			Status_KillTimer();
			Undo_DestroyAllEntries();
			Select_ReleaseCopyMemory();
			DestroyMenu(hMenu);
			Info_DestroyWindow();
			Minimap_DestroyWindow();
			Toolbar_DestroyWindow();
			Misc_ResetMenus(MainMenu);
			Misc_DestroyImageList(&hWalls);
			Misc_DestroyImageList(&hWorld);
			Misc_DestroyImageList(&hRooms);
			Misc_DestroyImageList(&hGates);
			Misc_DestroyImageList(&hDrawingTools);
			Misc_DestroyImageList(&hMiscTools);
			Misc_DestroyImage(&hSquare32);
			Map_DestroyProp();
			Map_DestroyCameras();
			Map_Destroy(&Map);
			Map_ResetZoom();
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
			List_ReleaseMemory(&RecentFiles);
			Triggers_ReleaseMemory();
			Properties_FreeDefaultElements();
			Properties_FreeDefaultCreatures();
			Sound_Reset();
			Registry_SetSafeFlag(TRUE);
			MapChanges = 0;
			hMenu = NULL;
			PostQuitMessage(0);
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_CREATE -- Création d'une nouvelle fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG Window_Create(HWND hWnd)
{
	MAPPROPERTIES	NewMap;

	Splash_CreateWindow();
	Config_SetToDefault();
	Registry_GetRecentFiles(hWnd);
	Config_UpdateRecentMenu();
	if (!Properties_LoadDefaultElements(hWnd)) goto Error_X;
	if (!Properties_LoadDefaultCreatures(hWnd)) goto Error_X;

	Debug_ResetVersions(TRUE);
	Properties_MapResetOptions(&Map,NULL);
	Properties_GlobalReset(&MapGlobalVariables,-1,MGRF_ALL);
	Properties_MapOptionsToDefault(&MapOptions[0],1);
	Properties_MapOptionsToDefault(&MapOptions[1],2);
	Properties_MapOptionsToDefault(&MapOptions[2],3);
	Properties_MapOptionsToDefault(&MapOptions[3],4);
	Properties_MapOptionsToDefault(&MapOptions[4],5);
	Properties_MapOptionsToDefault(&MapOptions[5],6);
	Properties_MapOptionsToDefault(&MapOptions[6],7);
	CopyMemory(&NewMap,&Map.Properties,sizeof(MAPPROPERTIES));
	NewMap.Width = MAP_DEFAULTWIDTH;
	NewMap.Height = MAP_DEFAULTHEIGHT;
	strcpy(NewMap.Name,szNoName);

	Font.hFont = GetStockObject(DEFAULT_GUI_FONT);
	FixedFont.hFont = GetStockObject(ANSI_FIXED_FONT);
	if (!Font.hFont || !FixedFont.hFont) goto Error_0;
	if (!Misc_GetFontInfo(hWnd,&Font)) goto Error_0;
	if (!Misc_GetFontInfo(hWnd,&FixedFont)) goto Error_0;
	if (!Misc_CreateImageList(World,&hWorld,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(Rooms,&hRooms,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(Gates,&hGates,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(Walls,&hWalls,32,32,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(DrawingTools,&hDrawingTools,20,20,FALSE)) goto Error_0;
	if (!Misc_CreateImageList(MiscTools,&hMiscTools,20,20,FALSE)) goto Error_0;
	if (!Misc_CreateImage(&hSquare32,930,32,32)) goto Error_0;
	if (!Status_CreateWindow(hWnd)) goto Error_0;
	if (!Status_CreateTimer(hWnd)) goto Error_0;
	if (!Toolbar_CreateWindow(hWnd)) goto Error_0;
	if (!Minimap_CreateWindow(hWnd)) goto Error_0;
	if (!Info_CreateWindow(hWnd)) goto Error_0;
	if (!Map_CreateCameras()) goto Error_0;
	if (!Map_CreateProp(hWnd)) goto Error_0;
	if (!Map_InitialiseZoom(hWnd,MapZoomCX,MapZoomCY)) goto Error_0;
	if (!Map_CreateNew(hWnd,&NewMap)) goto Error_0;

	if (!Config.HideToolbar) CheckMenuItem(hMenu,IDM_TOOLBAR,MF_CHECKED);
	if (!Config.HideMinimap) CheckMenuItem(hMenu,IDM_MINIMAP,MF_CHECKED);
	if (!Config.MinimapKeepProps) CheckMenuItem(hMenu,IDM_KEEPPROPS,MF_UNCHECKED);

	Map_ResetStats(&MapStats);
	GetLocalTime(&MapStats.CreatedTime);
	Misc_FlagsChangeMenu(hMenu,Config.MapViewFlags,DisplayFlags);
	Map_NewTitle(hWnd);
	Minimap_Build(hWnd);
	Sound_Initialize(hWnd);
	Status_DisplayZoom();
	Splash_DestroyWindow();
	Safe_CheckSafeFlag(hWnd);
	Registry_SetSafeFlag(FALSE);
	Registry_GetGamePath(hWnd);
	return(0);

//--- Erreurs d'initialisation ---

Error_0:Misc_PrintError(hWnd,szWindowErr,szInitErr,MB_ICONHAND);
Error_X:SetLastError(0);
	return(-1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_DRAWITEM -- Affichage des différents éléments de la fenêtre	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_DrawItems(DRAWITEMSTRUCT *ItemStruct)
{
	if (ItemStruct->hwndItem == hStatusWnd)
		{
		Status_DrawPaintMode(ItemStruct);
		return;
		}

	if (ItemStruct->CtlType == ODT_MENU)
		{
		Misc_DrawMenuItem(ItemStruct);
		return;
		}

	if (ItemStruct->CtlType == ODT_BUTTON)
		{
		if (ItemStruct->hwndItem == hToolbar)
			Toolbar_DrawWindow(ItemStruct);
		if (ItemStruct->hwndItem == hMapZoom)
			Zoom_DrawButton(ItemStruct);
		if (ItemStruct->hwndItem == hMinimap)
			Minimap_DrawWindow(ItemStruct);
		return;
		}

	if (ItemStruct->CtlType == ODT_STATIC)
		{
		if (ItemStruct->hwndItem == hInfo)
			Info_DrawWindow(ItemStruct);
		return;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_COMMAND -- Sélection d'un bouton ou d'un menu			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Command(UINT NotifyCode, UINT ItemID, HWND hWndCmd)
{
	switch(NotifyCode)
		{
		case BN_DBLCLK:
		case BN_CLICKED:
			if (hWndCmd == hToolbar)
				{
				Toolbar_Clicked();
				break;
				}
			if (hWndCmd == hMapZoom)
				{
				Zoom_Clicked();
				break;
				}

			if ((ItemID >= IDM_MAPWORLDSTRT)&&(ItemID <= IDM_MAPWORLDSTOP))
				{
				Toolbar_ChangeWorldElem(ItemID-IDM_MAPWORLDSTRT);
				break;
				}

			if ((ItemID >= IDM_MAPWORLDHEROPORTALSSTRT)&&(ItemID <= IDM_MAPWORLDHEROPORTALSSTOP))
				{
				Toolbar_ChangeGate(ItemID-IDM_MAPWORLDHEROPORTALSSTRT);
				break;
				}

			if ((ItemID >= IDM_PLAYERSSTRT)&&(ItemID <= IDM_PLAYERSSTOP))
				{
				Toolbar_ChangePlayer(ItemID-IDM_PLAYERSSTRT);
				break;
				}

			if ((ItemID == IDM_MAPWORLDWALL)||(ItemID == IDM_MAPWORLDCLAIMED))
				{
				Toolbar_ChangeWall(ItemID);
				break;
				}

			if ((ItemID >= IDM_ROOMSSTRT)&&(ItemID <= IDM_ROOMSSTOP))
				{
				Toolbar_ChangeRoom(ItemID-IDM_ROOMSSTRT);
				break;
				}

			if ((ItemID >= IDM_PAINTMODESTRT)&&(ItemID <= IDM_PAINTMODESTOP))
				{
				Toolbar_ChangePaintMode(ItemID-IDM_PAINTMODESTRT);
				break;
				}

			if ((ItemID >= IDM_PUTSTRT)&&(ItemID <= IDM_PUTSTOP))
				{
				Toolbar_ChangeMiscMode(ItemID-IDM_PUTSTRT);
				break;
				}

			if ((ItemID >= IDM_ZOOMSTRT)&&(ItemID <= IDM_ZOOMSTOP))
				{
				Zoom_Change(ItemID-IDM_ZOOMSTRT);
				break;
				}

			if ((ItemID >= IDM_RECENTITEMSFIRST)&&(ItemID <= IDM_RECENTITEMSMAX))
				{
				Files_LoadRecentMap(hWnd,ItemID-IDM_RECENTITEMSFIRST);
				break;
				}

			switch(ItemID)
				{
				case IDM_NEW:
					Map_New(hWnd);
					break;
				case IDM_OPEN:
					Files_LoadMap(hWnd,NULL,TRUE);
					break;
				case IDM_SAFEFILE:
					Files_LoadSafeMap(hWnd);
					break;
				case IDM_RECENTREMOVEOBSOLETE:
					Config_RemoveObsolete(hWnd);
					break;
				case IDM_RECENTREMOVEALL:
					Config_RemoveAll(hWnd);
					break;
				case IDM_SAVE:
					Files_SaveMapQuick(hWnd,MapFileType,Config.MapPath);
					break;
				case IDM_SAVEAS:
					Files_SaveMap(hWnd);
					break;

				case IDM_COMPILE:
					Compiler_CompileMap(hWnd);
					break;
				case IDM_TEST:
					Compiler_TestMap(hWnd);
					break;
				case IDM_EXECUTE:
					Compiler_RunMap(hWnd);
					break;

				case IDM_UNDOLAST:
					Undo_Restore();
					break;
				case IDM_UNDOHISTORY:
					Undo_History(hWnd);
					break;
				case IDM_UNDOFREE:
					Undo_DestroyAllEntries();
					break;
				case IDM_UNDOCOUNT:
					Undo_ChangeLevels();
					break;

				case IDM_COPY:
					Select_Copy(FALSE);
					break;
				case IDM_CUT:
					Select_Copy(TRUE);
					break;
				case IDM_PASTE:
					Select_Paste(&MapCopy,&MapCopyThings);
					break;
				case IDM_LOADBRUSHINSEL:
					Select_Load(FALSE);
					break;
				case IDM_LOADBRUSHINCOPY:
					Select_Load(TRUE);
					break;
				case IDM_SELECTION:
					Select_Save(FALSE);
					break;
				case IDM_COPYZONE:
					Select_Save(TRUE);
					break;

				case IDM_ROTATEP90:
					Select_Rotate(TRUE);
					break;
				case IDM_ROTATEM90:
					Select_Rotate(FALSE);
					break;
				case IDM_SYMHORZ:
					Select_Symmetry(FALSE);
					break;
				case IDM_SYMVERT:
					Select_Symmetry(TRUE);
					break;
				case IDM_SELECTCHANGE:
					Select_Change();
					break;
				case IDM_SELECTALL:
					Select_All(TRUE);
					break;
				case IDM_SELECTNONE:
					Select_All(FALSE);
					break;

				case IDM_FIND:
					Find_Select(hWnd);
					break;

				case IDM_CREATURES:
					Things_Manager(THINGMNGR_CREATURES);
					break;
				case IDM_DOORSTRAPS:
					Things_Manager(THINGMNGR_DOORSNTRAPS);
					break;
				case IDM_OBJECTS:
					Things_Manager(THINGMNGR_OBJECTS);
					break;

				case IDM_HEROS:
					Heros_Select(hWnd);
					break;
				case IDM_AREAS:
					RectEx_Select(hWnd,RECTEX_AREA);
					break;
				case IDM_GATES:
					RectEx_Select(hWnd,RECTEX_GATE);
					break;
				case IDM_EFFECTS:
					RectEx_Select(hWnd,RECTEX_EFFECT);
					break;
				case IDM_PATHS:
					RectEx_MakePaths(hWnd);
					break;
				case IDM_EVENTS:
					Triggers_Select(hWnd);
					break;

				case IDM_MAPSTATS:
					Map_DisplayStats(hWnd);
					break;
				case IDM_CREATURESPROPERTIES:
					Properties_GlobalCreatures(hWnd);
					break;
				case IDM_GAMEPROPERTIES:
					Properties_GlobalVariables(hWnd);
					break;
				case IDM_MAPPROPERTIES:
					Properties_MapOptions(hWnd);
					break;

				case IDM_THUMBNAIL:
					Thumb_CreateWindow(hWnd);
					break;
				case IDM_FULLSCREEN:
					Fullmap_CreateWindow();
					break;

				case IDM_TOOLBAR:
					Toolbar_Hide(!Misc_IsMenuItemChecked(hMenu,IDM_TOOLBAR,TRUE));
					break;
				case IDM_MINIMAP:
					Minimap_Hide(!Misc_IsMenuItemChecked(hMenu,IDM_MINIMAP,TRUE));
					break;
				case IDM_KEEPPROPS:
					Config.MinimapKeepProps = Misc_IsMenuItemChecked(hMenu,IDM_KEEPPROPS,TRUE);
					Minimap_Refresh();
					break;

				case IDM_MODIFYCREATURE:
					if ((Things_Select(hWnd,&MapImmCreature,0))&&(ToolsItemID != PAINTMODE_CREATURE))
						Toolbar_ChangeMiscMode(IDM_PUTCREATURE-IDM_PUTSTRT);
					break;
				case IDM_MODIFYDOOR:
					if ((Things_Select(hWnd,&MapImmDoor,0))&&(ToolsItemID != PAINTMODE_DOOR))
						Toolbar_ChangeMiscMode(IDM_PUTDOOR-IDM_PUTSTRT);
					break;
				case IDM_MODIFYTRAP:
					if ((Things_Select(hWnd,&MapImmTrap,0))&&(ToolsItemID != PAINTMODE_TRAP))
						Toolbar_ChangeMiscMode(IDM_PUTTRAP-IDM_PUTSTRT);
					break;
				case IDM_MODIFYOBJECT:
					if ((Things_Select(hWnd,&MapImmObject,0))&&(ToolsItemID != PAINTMODE_OBJECT))
						Toolbar_ChangeMiscMode(IDM_PUTOBJECT-IDM_PUTSTRT);
					break;
				case IDM_MODIFYMAGICALOBJECT:
					if ((Things_Select(hWnd,&MapImmMagicalObject,0))&&(ToolsItemID != PAINTMODE_MAGICALOBJECT))
						Toolbar_ChangeMiscMode(IDM_PUTMAGICALOBJECT-IDM_PUTSTRT);
					break;

				case IDM_FLAGTERRAIN:
				case IDM_FLAGCREATURES:
				case IDM_FLAGDOORS:
				case IDM_FLAGTRAPS:
				case IDM_FLAGMAGICALS:
				case IDM_FLAGOBJECTS:
				case IDM_FLAGAREAS:
				case IDM_FLAGPATHS:
				case IDM_FLAGGATES:
				case IDM_FLAGEFFECTS:
				case IDM_FLAGCAMERAS:
				case IDM_MOREDETAILS:
				case IDM_EXPANDGATES:
				case IDM_OBJECTSCOLOR:
				case IDM_MAGICSCOLOR:
					Misc_IsMenuItemChecked(hMenu,ItemID,TRUE);
					Misc_MenuChangesFlags(hMenu,&Config.MapViewFlags,DisplayFlags);
					Map_Refresh();
					break;

				case IDM_QUIT:
					PostMessage(hWnd,WM_CLOSE,(WPARAM)0,(LPARAM)0);
					break;

				case IDM_HELPCONTENT:
					Help_Content();
					break;
				case IDM_HELPVERSIONS:
					Debug_ShowVersions(hWnd);
					break;
				}

			break;
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_CONTEXTMENU -- Appuis sur le bouton droit de la souris	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_ContextMenu(LONG X, LONG Y, HWND hWnd)
{
	if (hWnd == hToolbar)
		Toolbar_RightClicked(X,Y);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_SIZING -- Vérification de la taille de la fenêtre		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Sizing(RECT *WindowRect)
{
	if (WindowRect->right-WindowRect->left < 600)
		WindowRect->right = WindowRect->left+600;
	if (WindowRect->bottom-WindowRect->top < 350)
		WindowRect->bottom = WindowRect->top+350;

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_SIZE -- Changement de la taille de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_AdjustSize(HWND hWnd, LONG ScrWidth, LONG ScrHeight)
{
	WINDOWPLACEMENT	WindowPlacement;

	WindowPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd,&WindowPlacement);

	if (WindowPlacement.showCmd == SW_SHOWMAXIMIZED)
		SetWindowPos(hWnd,NULL,0,0,ScrWidth,ScrHeight,SWP_NOZORDER|SWP_NOMOVE);

	return;
}

#pragma argsused
void Window_Resize(HWND hWnd, LONG Width, LONG Height)
{
	Toolbar_Resize(Width);
	Status_Resize(hWnd,Width);
	Map_PropResize(hWnd);
	Map_CorrectPos(hWnd);
	Minimap_Refresh();
	Info_Resize(Height);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_PAINT -- Affichage des éléments de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Paint(HWND hWnd, HDC hDC)
{
	if ( (!IsWindow(hToolbar)) || (!IsWindowVisible(hToolbar)) )
		{
		RECT	ClientRect;
		HPEN	hPen;
		HPEN	hOldPen;

		GetClientRect(hWnd,&ClientRect);
		ClientRect.bottom = 1;
		hPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
		if (hPen)
			{
			hOldPen = SelectObject(hDC,hPen);
			if (hOldPen)
				{
				MoveToEx(hDC,0,0,NULL);
				LineTo(hDC,ClientRect.right,0);
				SelectObject(hDC,hOldPen);
				}
			DeleteObject(hPen);
			}
		}

	if ( (IsWindow(hMinimap)) && (IsWindowVisible(hMinimap)) )
		{
		RECT	DrawRect;
		RECT	ClientRect;
		HPEN	hPen;
		HPEN	hOldPen;

		GetWindowRect(hMinimap,&DrawRect);
		MapWindowPoints(NULL,hWnd,(POINT *)&DrawRect,2);
		GetClientRect(hWnd,&ClientRect);

		DrawRect.left = DrawRect.right;
		hPen = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
		if (hPen)
			{
			hOldPen = SelectObject(hDC,hPen);
			if (hOldPen)
				{
				MoveToEx(hDC,ClientRect.left,DrawRect.bottom,NULL);
				LineTo(hDC,DrawRect.right,DrawRect.bottom);
				DrawRect.bottom = ClientRect.bottom;
				MoveToEx(hDC,DrawRect.left,DrawRect.top,NULL);
				LineTo(hDC,DrawRect.right,DrawRect.bottom);
				SelectObject(hDC,hOldPen);
				}
			DeleteObject(hPen);
			}
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
