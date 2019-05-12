
#ifndef EDITORPROTOS_INCLUDES
#define EDITORPROTOS_INCLUDES

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Programme principal						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Main_Startup.c *****************************************************/

int			Initialise_GetVersion(void);
int			Initialise_GetDeviceCaps(void);
int			Initialise_LockInstance(void);
int			Initialise_WindowsClasses(void);
int			Initialise_Window(void);

/*** Main_Keyboard.c ****************************************************/

int			Keyboard_InitialiseHook(void);
void			Keyboard_ResetHook(void);
LONG CALLBACK		Keyboard_Hook(int,WPARAM,LPARAM);
int			Keyboard_ProcessSpecialKeys(int,BOOL,BOOL);
int			Keyboard_ProcessShortCut(int,BOOL,BOOL);
void			Keyboard_GetShortCut(char *,int *,BOOL *,BOOL *);

/*** Main_Debug.c *******************************************************/

void			Debug_ResetVersions(BOOL);
void			Debug_ShowVersions(HWND);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fenêtres								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Windows_MainWindowProc.c *******************************************/

LONG CALLBACK		Window_Proc(HWND,UINT,WPARAM,LPARAM);
LONG			Window_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Window_Create(HWND);
void			Window_DrawItems(DRAWITEMSTRUCT *);
void			Window_Command(UINT,UINT,HWND);
void			Window_ContextMenu(LONG,LONG,HWND);
void			Window_Sizing(RECT *);
void			Window_AdjustSize(HWND,LONG,LONG);
void			Window_Resize(HWND,LONG,LONG);
void			Window_Paint(HWND,HDC);

/*** Windows_StatusWindow.c *********************************************/

int			Status_CreateWindow(HWND);
void			Status_UpdateParts(HWND);
void			Status_Resize(HWND,LONG);
void			Status_DisplayMapElement(LONG,LONG);
void			Status_DisplayMouseCoords(MAPRECT *,UINT);
void			Status_DisplayZoom(void);
void			Status_DrawPaintMode(DRAWITEMSTRUCT *);

int			Status_CreateTimer(HWND);
void			Status_KillTimer(void);
void			Status_DisplayTimer(void);

/*** Windows_Toolbar.c **************************************************/

int			Toolbar_CreateWindow(HWND);
void			Toolbar_DestroyWindow(void);
void			Toolbar_Hide(BOOL);
void			Toolbar_DrawWindow(DRAWITEMSTRUCT *);
void			Toolbar_Clicked(void);
void			Toolbar_RightClicked(LONG,LONG);
void			Toolbar_ChangeWorldElem(DWORD);
void			Toolbar_ChangeGate(DWORD);
void			Toolbar_ChangePlayer(DWORD);
void			Toolbar_ChangeWall(DWORD);
void			Toolbar_ChangeRoom(DWORD);
void			Toolbar_ChangePaintMode(DWORD);
void			Toolbar_ChangeMiscMode(DWORD);
void			Toolbar_Resize(LONG);

/*** Windows_Minimap.c **************************************************/

int			Minimap_CreateWindow(HWND);
void			Minimap_DestroyWindow(void);

void			Minimap_Messages(MSG *);
void			Minimap_Build(HWND);
void			Minimap_Redraw(HWND);
void			Minimap_Refresh(void);
void			Minimap_RefreshFast(LONG,LONG);
void			Minimap_Hide(BOOL);
void			Minimap_DrawWindow(DRAWITEMSTRUCT *);
void			Minimap_DrawCursor(HDC,LONG,LONG);
int			Minimap_MoveCursor(LONG,LONG,BOOL);

void			Minimap_Resize(HWND);
void			Minimap_GetArea(HWND,RECT *,BOOL);

/*** Windows_Info.c *****************************************************/

int			Info_CreateWindow(HWND);
void			Info_DestroyWindow(void);
void			Info_DrawWindow(DRAWITEMSTRUCT *);
void			Info_Update(void);
void			Info_Resize(LONG);

/*** Windows_Fullmap.c **************************************************/

void			Fullmap_CreateWindow(void);
void			Fullmap_DestroyWindow(void);

LONG			Fullmap_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Fullmap_Create(HWND);

void			Fullmap_PaintWindow(HWND,HDC);
void			Fullmap_PaintNeutral(HWND,HDC);
void			Fullmap_PaintTraps(HWND,HDC);
void			Fullmap_PaintDoors(HWND,HDC);
void			Fullmap_PaintCreatures(HWND,HDC);
void			Fullmap_PaintRoomIcon(HWND,LONG,LONG);
void			Fullmap_PaintIcon(DRAWITEMSTRUCT *);

void			Fullmap_ScrollToPoint(HWND,LONG,LONG);
void			Fullmap_FullmapToMap(RECT *,LONG,LONG,LONG *,LONG *);
void			Fullmap_MapToFullmap(RECT *,LONG,LONG,LONG *,LONG *);
void			Fullmap_GetClientRect(HWND,RECT *);

/*** Windows_Thumbnail.c ************************************************/

void			Thumb_CreateWindow(HWND);
void			Thumb_DestroyWindow(void);
void			Thumb_UpdateWindow(void);

LONG			Thumb_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Thumb_Create(HWND);
void			Thumb_PaintWindow(HDC);

void			Thumb_DisplayFlags(HWND);
BOOL CALLBACK		Thumb_DisplayProc(HWND,UINT,WPARAM,LPARAM);
void			Thumb_DisplaySetPlayerOptions(HWND);

/*** Windows_Splash.c ***************************************************/

void			Splash_CreateWindow(void);
void			Splash_DestroyWindow(void);
LONG			Splash_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Splash_Create(HWND);
void			Splash_PaintWindow(HDC);

/*** Windows_Functions.c ************************************************/

void			Window_RedrawAll(void);
void			Window_UnderCursor(HWND,LONG *,LONG *);
void			Window_CorrectPos(HWND,LONG *,LONG *);
void			Window_Center(HWND,HWND,LONG *,LONG *);

/*** Windows_ZoomButton.c ***********************************************/

void			Zoom_DrawButton(DRAWITEMSTRUCT *);
void			Zoom_Clicked(void);
void			Zoom_Change(UINT);
int			Zoom_GetUsed(void);

/*** Windows_Compiler.c *************************************************/

int			Compiler_CreateWindow(char *MapPath, UINT Method);
void			Compiler_DestroyWindow(void);
LONG			Compiler_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Compiler_Create(HWND);
void			Compiler_Destroy(HWND);
void			Compiler_ResizeWindows(HWND);
void			Compiler_CompilerInfo(LV_DISPINFO *);
void			Compiler_CompilerShow(void);
void			Compiler_PrintText(char *,DWORD);
void			Compiler_QueueError(UINT,DWORD);
void			Compiler_QueueErrorEx(ERRORNODE *);

/*** Windows_Disk.c *****************************************************/

void			Disk_CreateWindow(UINT);
void			Disk_DestroyWindow(BOOL);
void			Disk_Update(UINT);

LONG CALLBACK		Disk_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
LONG			Disk_Create(HWND);
void			Disk_Progress(UINT);

/*** Windows_Safe.c *****************************************************/

void			Safe_CheckSafeFlag(HWND);
BOOL CALLBACK		Safe_WindowProc(HWND,UINT,WPARAM,LPARAM);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Carte								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Map_Areas.c ********************************************************/

void			Map_GetMapFullArea(HWND,RECT *);
void			Map_GetMapArea(HWND,RECT *);
void			Map_GetMapSquares(HWND,RECT *,LONG *,LONG *);
int			Map_IsPointInRect(LONG,LONG,MAPRECT *);
int			Map_RectangleToClient(HWND,MAPRECT *,MAPCLIENT *);
void			Map_CorrectRect(MAPRECT *,MAPCLIENT *);
void			Map_CorrectAfterResize(void);
void			Map_CursorToMap(HWND,POINT *);
int			Map_CanPaint(HWND,BYTE);
BYTE*			Map_GetMapPtrFromClientPt(HWND,LONG,LONG);

/*** Map_Cameras.c ******************************************************/

int			Map_CreateCameras(void);
void			Map_DestroyCameras(void);
void			Map_DrawCameras(HWND,HDC);

/*** Map_Choice.c *******************************************************/

void			Map_ChoosePointEx(HWND,MAPRECT *,UINT,char *);

BOOL CALLBACK		Map_ChoosePointProc(HWND,UINT,WPARAM,LPARAM);
void			Map_ChoosePointInit(HWND,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointDraw(HWND,HDC,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointScrollTo(HWND,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointAutoScroll(HWND,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointScrollHorz(HWND,UINT,LONG,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointScrollVert(HWND,UINT,LONG,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointBeginRect(HWND,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointEndRect(HWND,BOOL,MAPCHOOSEPOINTCTX *);

void			Map_ChoosePointDrawRect(HWND,HDC,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointPaintUpdate(HWND,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointPaintInfo(DRAWITEMSTRUCT *,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointPaintMap(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Map_ChoosePointSetProp(HWND,UINT,MAPCHOOSEPOINTCTX *);
void			Map_ChoosePointDrawList(HWND,HDC,MAPCHOOSEPOINTCTX *,NODE *);
void			Map_ChoosePointDrawRectangle(HWND,HDC,MAPRECT *,LONG,LONG);

/*** Map_Display.c ******************************************************/

void			Map_Lock(BOOL,BOOL);
void			Map_Refresh(void);
void			Map_Paint(HWND,HDC);
void			Map_DrawRectangle(HWND,HDC,MAPRECT *);
void			Map_PaintCursor(HWND,HDC,MAPRECT *,MAPCURSOR *,BOOL);
void			Map_PaintTemporaryElement(MAPRECT *,DWORD);
void			Map_FillArea(HWND,LONG,LONG);
void			Map_PaintElementSimple(HWND,LONG,LONG);
void			Map_PaintElement(HWND,DWORD);

/*** Map_Find.c *********************************************************/

void			Find_Select(HWND);
void			Find_UpdateList(void *);

/*** Map_Functions.c ****************************************************/

BYTE			Map_TranslateID(BYTE,BYTE,ELEMENT *);
BYTE			Map_TranslateIDEx(BYTE,BYTE,BYTE,ELEMENT *);
DWORD			Map_GetRealIDFromMap(BYTE *);
DWORD			Map_TranslateWorldItem(void);

/*** Map_Mouse.c ********************************************************/

void			Mouse_Paint(HWND,HDC);
void			Mouse_RemoveThingUnderCursor(HWND,LONG);
void			Mouse_MovedUpdate(HWND);
void			Mouse_Moved(HWND,LONG,LONG);
void			Mouse_LeftButtonDown(HWND,LONG,LONG);
void			Mouse_LeftButtonUp(HWND,LONG,LONG,BOOL);
void			Mouse_RightButtonDown(HWND,LONG,LONG);
void			Mouse_RightButtonUp(HWND,LONG,LONG,BOOL,BOOL);
void			Mouse_HideCursor(HWND);
void			Mouse_UpdateCursor(HWND,LONG,LONG,BOOL);
void			Mouse_UpdateRect(HWND,LONG,LONG);
void			Mouse_HideSelRect(HWND);
void			Mouse_UpdateSelRect(HWND,LONG,LONG);

/*** Map_Props.c ********************************************************/

int			Map_CreateProp(HWND);
void			Map_DestroyProp(void);

void			Map_SetPropThumb(HWND,UINT);
void			Map_GetPropSizes(HWND,RECT *,UINT);
void			Map_PropResize(HWND);

void			Map_ScrollToPoint(HWND,LONG,LONG,DWORD);
void			Map_ScrollToPointEx(HWND,LONG,LONG,LONG,LONG,DWORD);
int			Map_ScrollHorizontally(HWND,UINT,LONG,BOOL);
int			Map_ScrollVertically(HWND,UINT,LONG,BOOL);
void			Map_CorrectPos(HWND);

/*** Map_Select.c *******************************************************/

void			Select_Update(void);
void			Select_UpdateCopy(void);
void			Select_All(BOOL);
void			Select_ReleaseCopyMemory(void);
void			Select_Change(void);
BOOL CALLBACK		Select_ChangeProc(HWND,UINT,WPARAM,LPARAM);
void			Select_Load(BOOL);
void			Select_Save(BOOL);
void			Select_Symmetry(BOOL);
void			Select_Rotate(BOOL);
void			Select_Copy(BOOL);
void			Select_Paste(MAPCOPY *,NODE *);
int			Select_CopyRequest(void);

/*** Map_Setup.c ********************************************************/

void			Map_New(HWND);
BOOL CALLBACK		Map_NewProc(HWND,UINT,WPARAM,LPARAM);
void			Map_NewInit(HWND,MAPNEWCTX *);
int			Map_NewReset(HWND,BOOL,MAPNEWCTX *);
void			Map_NewTitle(HWND);

int			Map_CreateNew(HWND,MAPPROPERTIES *);
int			Map_CreateDefaultMap(MAP *,MAPPROPERTIES *);
void			Map_CreateLimitWall(MAP *);
int			Map_CreateDisplayContext(MAP *);
void			Map_Destroy(MAP *);
MAP*			Map_PrepareResize(LONG,LONG,MAP *);
int			Map_InitialiseZoom(HWND,LONG,LONG);
void			Map_ResetZoom(void);
int			Map_CreateCursor(HWND,MAPCURSOR *,LONG,LONG,COLORREF);
void			Map_DestroyCursor(MAPCURSOR *);

/*** Map_Sounds.c *******************************************************/
/*
int			Sound_Initialize(HWND);
int			Sound_InitializeDirectSound(HWND);
int			Sound_Load(char *,IDirectSoundBuffer **);
void			Sound_Reset(void);

void			Sound_Play(UINT,DWORD);
void			Sound_Stop(void);
*/
/*** Map_Stats.c ********************************************************/

void			Map_ResetStats(MAPEDITSTATS *);
void			Map_CreateStats(MAPEDITSTATSCTX *);

void			Map_DisplayStats(HWND);
BOOL CALLBACK		Map_StatsProc(HWND,UINT,WPARAM,LPARAM);
void			Map_StatsInit(HWND,PROPSHEETPAGE *);
void			Map_StatsDisplayBand(HWND,MAPEDITSTATSCTX *);
void			Map_StatsDisplayPlayer(HWND,MAPEDITSTATSCTX *);

/*** Map_Undo.c *********************************************************/

void			Undo_Restore(void);
void			Undo_RestoreNode(UNDO *);

UNDO*			Undo_CreateEntry(NODE *,UINT);
UNDO*			Undo_CreateGlobalVariables(GLOBALVARS *);
UNDO*			Undo_CreatePathPoint(MAPRECTEX *,UINT);
UNDO*			Undo_CreateMapRect(char *,MAPRECT *,MAP *);
UNDO*			Undo_CreateFullMap(char *,MAP *);

int			Undo_StoreMoveThing(UNDO *,MAPTHING *);
int			Undo_StoreMapParams(UNDO *,MAP *,MAPOPTIONS *);
int			Undo_StoreEntry(UNDO *,NODE *,UINT);
int			Undo_StoreMapRect(UNDO *,MAPRECT *,MAP *);
int			Undo_StoreFullMap(UNDO *,MAP *);
int			Undo_StoreFullMapChanges(UNDO *,MAP *);

void			Undo_Update(void);
void			Undo_ChangeLevels(void);

void			Undo_History(HWND);
BOOL CALLBACK		Undo_HistoryProc(HWND,UINT,WPARAM,LPARAM);
int			Undo_HistoryBuildList(HWND,NODE *,HTREEITEM);

void			Undo_PrintError(HWND);
char*			Undo_PaintModeToText(DWORD);
UNDO*			Undo_CreateNewEntry(UINT,char *);
UNDO*			Undo_CreateNewChild(UNDO **,BOOL,UINT,char *);
void			Undo_DestroyAllEntries(void);
void			Undo_DestroyEntry(UNDO *);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Elements								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Things_Display.c ***************************************************/

void			Things_Paint(HWND,DWORD,LONG,LONG);
void			Things_DrawCreatures(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Things_DrawCreaturesNumber(HDC,LONG,COLORREF);
void			Things_DrawDoors(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Things_DrawTraps(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Things_DrawObjects(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Things_DrawMagicalObjects(HDC,RECT *,LONG,LONG,LONG,LONG);
void			Things_DrawAreas(HWND,HDC);
void			Things_DrawEffects(HWND,HDC);
void			Things_DrawPaths(HWND,HDC);
void			Things_DrawGates(HWND,HDC);
void			Things_DrawGatesExpand(LONG *,LONG *,LONG *,LONG *);

/*** Things_Selection.c *************************************************/

int			Things_Select(HWND,MAPTHING *,DWORD);
BOOL CALLBACK		Things_SelectProc(HWND,UINT,WPARAM,LPARAM);
void			Things_SelectInit(HWND,MAPTHINGCTX *);
void			Things_SelectReset(HWND,MAPTHINGCTX *,BOOL);
void			Things_SelectPoint(HWND,MAPTHINGCTX *);
void			Things_SelectChangeState(HWND,MAPTHINGCTX *,BOOL);
DWORD			Things_GetCreatureIDFromItem(MAPTHINGCTX *,HTREEITEM);

void			Things_SelectOptions(HWND,DWORD *,DWORD *,UINT);
BOOL CALLBACK		Things_SelectOptionsProc(HWND,UINT,WPARAM,LPARAM);

void			Things_SelectObjective(HWND,UINT *,UINT *,UINT *);
BOOL CALLBACK		Things_SelectObjectiveProc(HWND,UINT,WPARAM,LPARAM);

/*** Things_Manager.c ***************************************************/

void			Things_Manager(UINT);

BOOL CALLBACK		Things_ManagerProc(HWND,UINT,WPARAM,LPARAM);
void			Things_ManagerInit(HWND,MAPTHINGMANAGER *);
int CALLBACK		Things_ManagerSort(LPARAM,LPARAM,LPARAM);
void			Things_ManagerSetCreaturesList(HWND,BOOL,MAPTHINGMANAGER *);
void			Things_ManagerSetDoorsnTrapsList(HWND,BOOL,MAPTHINGMANAGER *);
void			Things_ManagerSetObjectsList(HWND,BOOL,MAPTHINGMANAGER *);
void			Things_ManagerDisplayCount(HWND,MAPTHINGMANAGER *);
void			Things_ManagerSetDispInfo(LV_DISPINFO *);
int			Things_ManagerGetImage(UINT,UINT,UINT);

void			Things_ManagerChoosePoint(HWND);
void			Things_ManagerShowWindow(HWND);
BOOL CALLBACK		Things_ManagerShowProc(HWND,UINT,WPARAM,LPARAM);
void			Things_ManagerCenterView(HWND,MAPTHINGMANAGER *);
int			Things_ManagerRequest(HWND,char *,char *,DWORD);
void			Things_ManagerChangeListState(HWND,MAPTHINGMANAGER *);
void			Things_ManagerSelectAll(MAPTHINGMANAGER *);

void			Things_ManagerUpdateUndo(HWND,MAPTHINGMANAGER *);
void			Things_ManagerUndo(HWND,MAPTHINGMANAGER *);

void			Things_ManagerKill(HWND,MAPTHINGMANAGER *);
void			Things_ManagerModify(HWND,MAPTHINGMANAGER *);
int			Things_ManagerIsThingInRect(MAPTHING *,MAPTHINGMANAGER *);
void			Things_ManagerRemoveFromList(MAPTHING *,MAPTHINGMANAGER *);
void			Things_ManagerAdd(HWND,MAPTHINGMANAGER *);

int			Things_ManagerAddOptions(HWND,UINT,MNGRADDPARAMS *);
BOOL CALLBACK		Things_ManagerAddProc(HWND,UINT,WPARAM,LPARAM);
void			Things_ManagerAddInit(HWND,MNGRADDPARAMSCTX *);
void			Things_ManagerAddSave(HWND,MNGRADDPARAMSCTX *);

int			Things_ManagerModifyOptions(HWND,UINT,MNGRMODPARAMS *);
BOOL CALLBACK		Things_ManagerModifyProc(HWND,UINT,WPARAM,LPARAM);
void			Things_ManagerModifyInit(HWND,MNGRMODPARAMSCTX *);
void			Things_ManagerModifySave(HWND,MNGRMODPARAMSCTX *);

int			Things_ManagerPreview(HWND,NODE *,UINT,MAPTHINGMANAGER *);
BOOL CALLBACK		Things_ManagerPreviewProc(HWND,UINT,WPARAM,LPARAM);
void			Things_ManagerPreviewInit(HWND,MAPTHINGMANAGER *);
void			Things_ManagerPreviewToggle(UINT,MAPTHINGMANAGER *);
void			Things_ManagerPreviewSelect(MAPTHINGMANAGER *);
void			Things_ManagerPreviewPbsInfo(LV_DISPINFO *,MAPTHINGMANAGER *);
void			Things_ManagerPreviewDispInfo(LV_DISPINFO *,MAPTHINGMANAGER *);

/*** Things_Functions.c *************************************************/

long			Things_GetCreaturesCountWidth(LONG,LONG);
MAPTHING*		Things_GetAtMapPos(NODE *,LONG,LONG);
long			Things_GetCountAtMapPos(NODE *,LONG,LONG);
void			Things_StrCat(char *,char *,MAPTHING *,ELEMENT *);
void			Things_RemoveAtMapPos(NODE *,LONG,LONG);
void			Things_RemoveAtMapPosSafe(NODE *,LONG,LONG);
void			Things_RemoveAtMapPosEx(NODE *,LONG,LONG,UNDO **);
void			Things_CloneAtMapPos(NODE *,LONG,LONG,NODE *);
int			Things_IsHero(DWORD);
int			Things_GetOffsetFromID(DWORD,UINT);
char*			Things_GetCreatureNameFromID(DWORD);
char*			Things_GetThingNameFromID(DWORD,ELEMENT *);
char*			Things_GetThingASCIIFromID(DWORD,ELEMENT *);
long			Things_GetThingPos(NODE *,DWORD,LONG);

int			Things_IsSolid(BYTE *);
int			Things_CanPaint(MAP *,LONG,LONG);
int			Things_CanPaintEx(MAP *,LONG,LONG,DWORD,UINT,UINT,char *);
BYTE			Things_GetOwner(BYTE *,UINT);

/*** Things_Heros.c *****************************************************/

void			Heros_Select(HWND);
BOOL CALLBACK		Heros_SelectProc(HWND,UINT,WPARAM,LPARAM);
void			Heros_SelectInit(HWND,MAPTHINGHEROSBANDCTX *);
void			Heros_SelectReset(MAPTHINGHEROSBANDCTX *);
int			Heros_SelectBuildList(MAPTHINGHEROSBANDCTX *);
void			Heros_SelectKill(HWND,MAPTHINGHEROSBANDCTX *);
void			Heros_SelectChange(HWND,UINT,MAPTHINGHEROSBANDCTX *);

BOOL CALLBACK		Heros_ModifyProc(HWND,UINT,WPARAM,LPARAM);
void			Heros_ModifyInit(HWND,MAPTHINGHEROSBANDMODCTX *);
int			Heros_ModifyCheckBand(HWND,MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifySetDispInfo(LV_DISPINFO *,MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifyBuildList(MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifyRemplaceHero(HWND,MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifyModify(HWND,MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifyAddHero(HWND,MAPTHINGHEROSBANDMODCTX *);
void			Heros_ModifyKill(HWND,MAPTHINGHEROSBANDMODCTX *);

int			Heros_GetFreeBand(void);
MTHBAND*		Heros_FindHerosById(UINT);
long			Heros_GetHerosCount(MTHBAND *);

/*** Things_RectEx.c ****************************************************/

void			RectEx_Select(HWND,UINT);
BOOL CALLBACK		RectEx_Proc(HWND,UINT,WPARAM,LPARAM);
int CALLBACK		RectEx_Sort(LPARAM,LPARAM,LPARAM);
void			RectEx_Init(HWND,MAPRECTEXCTX *);
void			RectEx_ChangeSelectState(HWND,MAPRECTEXCTX *);
void			RectEx_RemoveRects(HWND,MAPRECTEXCTX *);
void			RectEx_Modify(HWND,MAPRECTEXCTX *);
void			RectEx_AddRect(HWND,MAPRECTEXCTX *);
void			RectEx_ModifyRect(HWND,MAPRECTEXCTX *);
void			RectEx_BuildList(MAPRECTEXCTX *);
void			RectEx_DisplayList(LV_DISPINFO *);
void			RectEx_DrawColoredButton(DRAWITEMSTRUCT *,COLORREF);
void			RectEx_ChooseColor(HWND,UINT,COLORREF *,COLORREF);

void			RectEx_AreaOptions(HWND,MAPRECTEX *);
BOOL CALLBACK		RectEx_AreaOptionsProc(HWND,UINT,WPARAM,LPARAM);

void			RectEx_EffectOptions(HWND,MAPRECTEX *);
BOOL CALLBACK		RectEx_EffectOptionsProc(HWND,UINT,WPARAM,LPARAM);

void			RectEx_GenerateGates(HWND,MAPRECTEXCTX *);
int			RectEx_FindDestroyAction(short);
int			RectEx_GetGatesList(NODE *);
int			RectEx_GateInList(NODE *,LONG,LONG);
void			RectEx_Generate(HWND,MAPRECTEXGATESCTX *);
void			RectEx_GenerateRawDoFmt(MAPRECTEXGATESLIST *,char *,char *,LONG);
BOOL CALLBACK		RectEx_GenerateProc(HWND,UINT,WPARAM,LPARAM);
void			RectEx_GenerateInit(HWND,MAPRECTEXGATESCTX *);
int			RectEx_GenerateSave(HWND,MAPRECTEXGATESCTX *);
void			RectEx_GenerateText(LV_DISPINFO *);
void			RectEx_GenerateFlags(HWND,UINT,MAPRECTEXGATESCTX *);

void			RectEx_StrCpy(char *,char *);
int			RectEx_GetFreeID(NODE *);
MAPRECTEX*		RectEx_FindById(UINT,NODE *);
WORD			RectEx_GetRectLocation(NODE *,UINT,UINT);
int			RectEx_GetGateAtMapPos(LONG,LONG);

/*** Things_Paths.c *****************************************************/

void			RectEx_MakePaths(HWND);
BOOL CALLBACK		RectEx_PathsProc(HWND,UINT,WPARAM,LPARAM);
void			RectEx_PathsInit(HWND,MAPRECTEXPATHCTX *);
int			RectEx_PathsRedrawAll(MAPRECTEXPATHCTX *);
int			RectEx_PathsSetGroups(MAPRECTEXPATHCTX *);
int			RectEx_PathsIsPointUsed(UINT,UINT *);
void			RectEx_PathsCheck(HWND,BOOL,MAPRECTEXPATHCTX *);
void			RectEx_PathsShowTVArea(MAPRECTEXPATHCTX *);
int			RectEx_PathsIsButtonActivable(UINT,MAPRECTEXPATHCTX *);
void			RectEx_PathsCreateNewPath(HWND,MAPRECTEXPATHCTX *);
void			RectEx_PathsKill(HWND,MAPRECTEXPATHCTX *);
void			RectEx_PathsAddArea(HWND,BOOL,MAPRECTEXPATHCTX *);
void			RectEx_PathsModifyPause(HWND,MAPRECTEXPATHCTX *);

int			RectEx_PathsSelectArea(HWND,MAPRECTEXPATHCTX *);
BOOL CALLBACK		RectEx_PathsSelectProc(HWND,UINT,WPARAM,LPARAM);
void			RectEx_PathsSelectInit(HWND,MAPRECTEXPATHCTX *);
void			RectEx_PathsSelectChanged(HWND,MAPRECTEXPATHCTX *);

void			RectEx_DrawPath(HWND,BOOL,BOOL);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Evènements							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Triggers_Select.c **************************************************/

void			Triggers_Select(HWND);
BOOL CALLBACK		Triggers_SelectProc(HWND,UINT,WPARAM,LPARAM);
void			Triggers_SelectInit(HWND,TRIGGERSCTX *);
int			Triggers_SelectAddPoint(TRIGGERLINK *,LV_ITEM *,char *,TRIGGERSCTX *);
void			Triggers_SelectPointChanged(HWND,NM_LISTVIEW *,TRIGGERSCTX *);

int			Triggers_SelectBuildList(TRIGGERSCTX *);
int			Triggers_SelectCreateNewTrigger(TRIGGER *,HWND,HTREEITEM,HTREEITEM,HTREEITEM *,char *);
int			Triggers_SelectCreateNewAction(ACTION *,HWND,HTREEITEM,HTREEITEM,HTREEITEM *,char *);

void			Triggers_SelectKillPoint(HWND,TRIGGERSCTX *);
void			Triggers_SelectMovePtUp(HWND,TRIGGERSCTX *);
void			Triggers_SelectMovePtDown(HWND,TRIGGERSCTX *);

void			Triggers_SelectMoveRight(HWND,TRIGGERSCTX *);
void			Triggers_SelectMoveLeft(HWND,TRIGGERSCTX *);
void			Triggers_SelectMoveUp(HWND,TRIGGERSCTX *);
void			Triggers_SelectMoveDown(HWND,TRIGGERSCTX *);
void			Triggers_SelectChanged(HWND,NM_TREEVIEW *,TRIGGERSCTX *);
int			Triggers_SelectLink(void *,TRIGGERLINK *,HTREEITEM,TRIGGERSCTX *);
void			Triggers_SelectKill(HWND,TRIGGERSCTX *);
void			Triggers_SelectKillTrigger(HWND,TRIGGER *,HTREEITEM,TRIGGERSCTX *);
void			Triggers_SelectKillAction(HWND,ACTION *,HTREEITEM,TRIGGERSCTX *);

/*** Triggers_ActionPoints.c ********************************************/

void			Triggers_PointNew(HWND,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_PointDlgList(HWND,UINT,WPARAM,LPARAM);
void			Triggers_PointDlgCheckKeeper(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckCreature(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckDoor(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckObject(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckMagicalObject(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckArea(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckBand(HWND,LONG *,TRIGGERSCTX *);
void			Triggers_PointDlgCheckCreatureInBand(HWND,LONG *,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_PointDlgParams(HWND,UINT,WPARAM,LPARAM);
void			Triggers_PointDlgParamsInit(HWND,TRIGGERSCTX *);
void			Triggers_PointDlgParamsCheck(HWND,TRIGGERSCTX *);
int			Triggers_PointDlgParamsStore(BOOL,TRIGGERSCTX *);

/*** Triggers_Triggers.c ************************************************/

void			Triggers_TriggerNew(HWND,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_TriggerDlgList(HWND,UINT,WPARAM,LPARAM);
void			Triggers_TriggerDlgInit(HWND,TRIGGERSCTX *);
TRIGGERINFO*		Triggers_TriggerDlgGetParam(TRIGGERSCTX *);
void			Triggers_TriggerDlgSetDefault(BYTE,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_TriggerDlgParams(HWND,UINT,WPARAM,LPARAM);
void			Triggers_TriggerDlgParamsInit(HWND,TRIGGERSCTX *);
void			Triggers_TriggerDlgParamsCheck(HWND,TRIGGERSCTX *);
int			Triggers_TriggerDlgParamsStore(HWND,TRIGGERSCTX *);
int			Triggers_TriggerDlgParamsSave(HWND,TRIGGERSCTX *);
long			Triggers_TriggerDlgGetSelectedEntryParam(HWND);

BOOL CALLBACK		Triggers_TriggerDlgRepeat(HWND,UINT,WPARAM,LPARAM);

/*** Triggers_Actions.c *************************************************/

void			Triggers_ActionNew(HWND,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_ActionDlgList(HWND,UINT,WPARAM,LPARAM);
void			Triggers_ActionDlgInit(HWND,TRIGGERSCTX *);
TRIGGERINFO*		Triggers_ActionDlgGetParam(TRIGGERSCTX *);
void			Triggers_ActionDlgSetDefault(BYTE,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_ActionDlgParams(HWND,UINT,WPARAM,LPARAM);
void			Triggers_ActionDlgParamsInit(HWND,TRIGGERSCTX *);
void			Triggers_ActionDlgParamsCheck(HWND,TRIGGERSCTX *);
void			Triggers_ActionDlgChoosePoint(HWND);
UINT			Triggers_ActionDlgParamsGetGate(HWND,UINT);
void			Triggers_ActionDlgParamsSetGate(HWND,UINT,UINT);
int			Triggers_ActionDlgParamsStore(HWND,TRIGGERSCTX *);
int			Triggers_ActionDlgParamsSave(HWND,TRIGGERSCTX *);

/*** Triggers_Variables.c ***********************************************/

void			Triggers_Variables(HWND,TRIGGERSCTX *);

BOOL CALLBACK		Triggers_VariablesProc(HWND,UINT,WPARAM,LPARAM);
void			Triggers_VariablesInit(HWND,TRIGGERSCTX *);
void			Triggers_VariablesSetInfo(LV_DISPINFO *);
void			Triggers_VariablesSetName(HWND,BOOL,TRIGGERSCTX *);

/*** Triggers_Functions.c ***********************************************/

void			Triggers_ReleaseMemory(void);
void			Triggers_KillFromId(short);
void			Triggers_CreateActionPtFromLevel(short);
void			Triggers_CreateActionPtFromPlayer(UINT,short);
void			Triggers_CreateActionPtFromCreature(MAPTHING *,short);
void			Triggers_CreateActionPtFromDoor(MAPTHING *,short);
void			Triggers_CreateActionPtFromObject(MAPTHING *,BOOL,short);
void			Triggers_CreateActionPtFromArea(UINT,short);
void			Triggers_CreateActionPtFromBand(MTHBAND *,short);
TRIGGERLINK*		Triggers_CreateActionPtFromCreatureInBand(MAPTHINGHERO *,UINT,short);

ULONG			Triggers_GetFreeUniqueID(void);
void*			Triggers_GetStructureFromUniqueID(ULONG,DWORD);
void			Triggers_MakeLinks(void);
void			Triggers_MakeTriggerLinks(TRIGGER *,WORD);
void			Triggers_MakeActionLinks(ACTION *,WORD);
void			Triggers_UnlinkTrigger(TRIGGER *);
void			Triggers_UnlinkAction(ACTION *);
short			Triggers_GetTriggerFreeId(void);
TRIGGER*		Triggers_GetTriggerById(short);
short			Triggers_GetActionFreeId(void);
ACTION*			Triggers_GetActionById(short);
short			Triggers_GetFirstTrigger(ULONG,DWORD,BOOL *);

int			Triggers_IsUsed(HWND,ULONG,BOOL *,DWORD);

/*** Triggers_Display.c *************************************************/

int			Triggers_DisplayGetImageId(DWORD,DWORD);
int			Triggers_DisplayCreateGatesList(HWND,UINT);
int			Triggers_DisplayCreateTerrainList(HWND,UINT,HWND *,DWORD,BOOL,BOOL);
int			Triggers_DisplayCreateHerosList(HWND,UINT,HWND *,DWORD);
int			Triggers_DisplayCreateButtonsList(HWND,UINT,HWND *,DWORD,DWORD,BOOL);
int			Triggers_DisplayCreateSpellsList(HWND,UINT,HWND *,DWORD,BOOL);
int			Triggers_DisplayCreateTrapsList(HWND,UINT,HWND *,DWORD,BOOL);
int			Triggers_DisplayCreateDoorsList(HWND,UINT,HWND *,DWORD,BOOL);
int			Triggers_DisplayCreateRoomsList(HWND,UINT,HWND *,DWORD,BOOL,BOOL);
int			Triggers_DisplayCreatePlayersList(HWND,UINT,HWND *,DWORD,BOOL);
int			Triggers_DisplayCreateCreaturesList(HWND,UINT,HWND *,DWORD,BOOL,BOOL);
void			Triggers_DisplayDrawList(DRAWITEMSTRUCT *,TRIGGERSCTX *);

/*** Triggers_Texts.c ***************************************************/

char*			Triggers_FmtTriggerText(TRIGGER *,char *,char *);
char*			Triggers_FmtActionText(ACTION *,char *, char *);

char*			Triggers_FmtVoiceText(LONG,char *);
char*			Triggers_FmtVariableText(LONG,char *);
char*			Triggers_FmtTimerText(LONG,char *);
char*			Triggers_FmtTimeText(LONG,BOOL,char *);
char*			Triggers_FmtNumberText(LONG,char *);
char*			Triggers_FmtOperationText(LONG,BOOL,char *);
char*			Triggers_FmtObjectOperationText(LONG,char *);
char*			Triggers_FmtCreatureText(LONG,LONG,char *);
char*			Triggers_FmtEUText(LONG,LONG,char *,char *);
char*			Triggers_FmtPlayerText(LONG,char *);
char*			Triggers_FmtObjectPlayerText(LONG,char *);
char*			Triggers_FmtSpecialPrefix(char *);
char*			Triggers_FmtRoomWithPrefix(LONG,char *);
char*			Triggers_FmtRoomExText(LONG,LONG,char *);
char*			Triggers_FmtRoomText(LONG,char *);
char*			Triggers_FmtDoorText(LONG,LONG,char *);
char*			Triggers_FmtTrapText(LONG,LONG,char *);
char*			Triggers_FmtSpellText(LONG,char *);
char*			Triggers_FmtTerrainWithPrefix(LONG,char *);
char*			Triggers_FmtTerrainText(LONG,char *);
char*			Triggers_FmtElementText(LONG,LONG,LONG,char *);
BOOL			Triggers_IsFemale(LONG);
BOOL			Triggers_IsFemaleRoom(LONG);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Compilateur							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Compiler_Info.c ****************************************************/

LONG			Compiler_InfoEnumTerrainInRect(DWORD,DWORD,DWORD,MAPRECT *);
LONG			Compiler_InfoManaInRect(UINT,MAPRECT *);
LONG			Compiler_InfoRoomInRect(UINT,MAPRECT *);
LONG			Compiler_InfoTreasuryCapacityInRect(UINT,MAPRECT *);
LONG			Compiler_InfoCreaturesCountInRect(UINT,DWORD,MAPRECT *);
LONG			Compiler_InfoGoldAmountInRect(DWORD,BOOL *,MAPRECT *);
void			Compiler_InfoGateType(LONG,LONG,LONG *,LONG *);
int			Compiler_InfoHeartPosition(UINT,LONG *,LONG *);

/*** Compiler_Compile.c *************************************************/

void			Compiler_CompileMap(HWND);
void			Compiler_TestMap(HWND);
void			Compiler_RunMap(HWND);

void			Compiler_BeginTask(char *,UINT);
void			Compiler_Task(void *);
void			Compiler_TaskEndText(COMPILERTASK *);
void			Compiler_TaskGenerate(COMPILERTASK *);
int			Compiler_TaskIsReserved(char *);
void			Compiler_TaskTest(COMPILERTASK *);
void			Compiler_TaskTestVersion(COMPILERTASK *);
void			Compiler_TaskTestMap(COMPILERTASK *);
void			Compiler_TaskTestPlayers(COMPILERTASK *);
void			Compiler_TaskTestMapRect(LONG,LONG,ELEMENT *,DWORD,UINT,COMPILERTASK *);
void			Compiler_TaskTestMapExpand(LONG,LONG,ELEMENT *,DWORD,UINT,COMPILERTASK *);
int			Compiler_TaskTestMapInRect(LONG,LONG,COMPILERTASK *);
void			Compiler_TaskTestThings(COMPILERTASK *);
void			Compiler_TaskTestAreas(COMPILERTASK *);
void			Compiler_TaskTestGates(COMPILERTASK *);
void			Compiler_TaskTestTriggers(COMPILERTASK *);
void			Compiler_TaskTestResetTrigger(short,COMPILERTASK *);
void			Compiler_TaskTestResetAction(short,COMPILERTASK *);
int			Compiler_TaskTestTrigger(short,COMPILERTASK *);
int			Compiler_TaskTestAction(short,COMPILERTASK *);
void			Compiler_TaskRun(COMPILERTASK *);

/*** Compiler_Generate.c ************************************************/

int			Compiler_GenerateInfoFile(char *);
int			Compiler_GeneratePlayersFile(char *);
int			Compiler_GenerateThingsFile(char *);
int			Compiler_GenerateObjects(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateMagicalObjects(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateTraps(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateDoors(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateCreatures(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateAreas(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateGates(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateEffects(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateHeros(HANDLE,DWORD *,DWORD *);
int			Compiler_GenerateMapFile(char *);
int			Compiler_GenerateVariablesFile(char *);
BYTE*			Compiler_GeneratePlayerVariables(BYTE *,DWORD *,UINT);
BYTE*			Compiler_GenerateCreaturesVariables(BYTE *,DWORD *);
BYTE*			Compiler_GenerateAlliesVariables(BYTE *,DWORD *);
BYTE*			Compiler_GenerateGlobalVariables(BYTE *,DWORD *);
int			Compiler_GenerateTriggersFile(char *);
int			Compiler_GenerateTriggers(HANDLE);
int			Compiler_GenerateActions(HANDLE);
int			Compiler_GenerateTerrainsFile(char *);
int			Compiler_GenerateRoomsFile(char *);
int			Compiler_GenerateDoorsFile(char *);
int			Compiler_GenerateTrapsFile(char *);
int			Compiler_GenerateSpellsFile(char *);
int			Compiler_GenerateCreaturesFile(char *);
int			Compiler_GenerateCreatureSpellsFile(char *);

void			Compiler_SetDate(GAMEDATE *);

/*** Compiler_Conversions.c *********************************************/

DWORD*			Compiler_EditorMapToGameMap(MAPINFO *);
char*			Compiler_EditorMapToTextMap(MAPINFO *);
MAPINFO*		Compiler_GameMapToEditorMap(MAPINFO *,NODE *);
MAPINFO*		Compiler_TextMapToEditorMap(MAPINFO *,NODE *);

/*** Compiler_Report.c **************************************************/

int			Compiler_Report(HWND,UINT,UINT,NODE *);
BOOL CALLBACK		Compiler_ReportProc(HWND,UINT,WPARAM,LPARAM);
void			Compiler_ReportInit(HWND,ERRORCTX *);
void			Compiler_ReportInfo(LV_DISPINFO *,ERRORCTX *);
void			Compiler_ReportDraw(DRAWITEMSTRUCT *);
void			Compiler_ReportShow(ERRORCTX *);
ERRORNODE*		Compiler_ReportErrorText(NODE *,UINT,DWORD);
ERRORNODE*		Compiler_ReportErrorXY(NODE *,UINT,LONG,LONG,DWORD);
ERRORNODE*		Compiler_ReportErrorFull(NODE *,UINT,LONG,LONG,LONG,LONG,DWORD);
void			Compiler_ReportErrorCopyText(UINT,char *,DWORD);

/*** Compiler_Errors.c **************************************************/

void			Compiler_SetLastError(DWORD);
DWORD			Compiler_GetLastError(void);
void			Compiler_ErrorCodeToText(char *);
char*			Compiler_GetErrorCodeText(void);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fichiers								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Files_Editor.c *****************************************************/

int			Editor_ExtractAll(HWND,char *,NODE *);
BYTE*			Editor_ReadFile(char *,DWORD *);
BYTE*			Editor_FindHeader(DWORD,BYTE *,DWORD);
int			Editor_ExtractStats(BYTE *,DWORD,NODE *);
int			Editor_ExtractMapInfo(BYTE *,DWORD,NODE *);
int			Editor_ExtractPlayers(BYTE *,DWORD,NODE *);
MAPINFO*		Editor_ExtractMap(BYTE *,DWORD,NODE *);
int			Editor_ExtractThings(BYTE *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Editor_ExtractRectEx(BYTE *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Editor_ExtractHeros(BYTE *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Editor_ExtractTriggers(BYTE *,DWORD,NODE *);
int			Editor_ExtractGlobalVariables(BYTE *,DWORD,NODE *);

int			Editor_GenerateFile(HWND,char *,BOOL);

/*** Files_Game.c *******************************************************/

int			Game_ExtractAll(HWND,char *,NODE *);

int			Game_ExtractMain(char *,NODE *);
int			Game_ExtractPlayers(char *,NODE *);
MAPINFO*		Game_ExtractMap(char *,NODE *);

int			Game_ExtractThings(char *,NODE *,MAPSTATS *,BOOL);
void			Game_AddRectEx(MAPRECTEX *,DWORD,NODE *,NODE *);
void			Game_AddThing(MAPTHING *,NODE *,NODE *);

int			Game_ExtractVariables(char *,NODE *);
void			Game_SetGlobalVariable(GAMEGLOBALVARIABLE *,NODE *);
void			Game_SetCreatureCount(GAMEVARCREATURECOUNT *,NODE *);
void			Game_SetVariable(GAMEVARTHINGSTATUS *,NODE *);
void			Game_SetAlliance(GAMEVARALLIANCE *,NODE *);
void			Game_SetCreature(GAMEVARCREATURE *,NODE *);
int			Game_GetOffsetFromID(DWORD,ELEMENT *);

int			Game_ExtractTriggers(char *,NODE *);

/*** Files_Text.c *******************************************************/

int			Text_ExtractAll(HWND,char *,NODE *);
char*			Text_ReadFile(char *,DWORD *);
char*			Text_FindHeader(char *,DWORD,char *);
char*			Text_CompareName(char *,char *,ELEMENT *,DWORD *);
DWORD			Text_TranslateID(char,char,ELEMENT *);

MAPINFO*		Text_ExtractMap(char *,DWORD,NODE *);
int			Text_ExtractThings(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractCreatures(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractDoors(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractTraps(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractInfos(char *,DWORD,NODE *);
int			Text_ExtractBands(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractAreas(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractGates(char *,DWORD,NODE *,MAPSTATS *,BOOL);
int			Text_ExtractTriggers(char *,DWORD,NODE *);
int			Text_ExtractVariables(char *,DWORD,NODE *);

int			Text_GenerateFile(HWND,char *);
int			Text_FillRoomsOptions(HANDLE,ELEMENT *,UINT,BYTE *);
int			Text_FillPlayerOptions(HANDLE,ELEMENT *,UINT,UINT *);
int			Text_WriteThings(HANDLE,char *,UINT,BOOL);

/*** Files_Areas.c ******************************************************/

MAPINFO*		Areas_ExtractMap(char *);
int			Areas_ExtractThings(char *,NODE *,MAPSTATS *);
int			Areas_WriteThings(HANDLE,BOOL,MAPCLIENT *);
LONG			Areas_GetThingsSize(BOOL,MAPCLIENT *);

/*** Files_Functions.c **************************************************/

UINT			Files_GetMapTypeFromName(char *);
void*			Files_Load(char *,DWORD *);
HANDLE			Files_Create(char *,DWORD,LONG,LONG);
int			Files_WriteHeader(HANDLE,DWORD,LONG,LONG);
int			Files_WriteData(HANDLE,LPVOID,DWORD,UINT);

/*** Files_Load.c *******************************************************/

void			Files_LoadRecentMap(HWND,UINT);
void			Files_LoadSafeMap(HWND);
void			Files_LoadMap(HWND,char *,BOOL);

/*** Files_Save.c *******************************************************/

void			Files_AutoSave(void);
void			Files_SaveMap(HWND);
void			Files_SaveMapQuick(HWND,UINT,char *);
void			Files_SaveUpdate(void);

int			Files_SaveCompatibility(HWND,UINT);
int			Files_SaveCompReported(LONG,LONG,NODE *);
void			Files_SaveCompExpand(DWORD,LONG,LONG,LONG *,LONG *);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fichiers								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Misc_Display.c *****************************************************/

void			Misc_FmtDate(char *,SYSTEMTIME *);
BOOL CALLBACK		Misc_FmtDateEnumProc(LPTSTR);
void			Misc_DrawGroup(DRAWITEMSTRUCT *);

UINT			Misc_GetArea(HWND,UINT,BOOL);
void			Misc_SetArea(HWND,UINT,UINT);
UINT			Misc_GetEffect(HWND,UINT,BOOL);
void			Misc_SetEffect(HWND,UINT,UINT);
int			Misc_CreateEffectComboBoxList(HWND,UINT,BOOL);
int			Misc_CreateAreaComboBoxList(HWND,UINT,BOOL);
int			Misc_CreateLevelComboBoxList(HWND,UINT,LONG);
int			Misc_CreatePlayerComboBoxList(HWND,UINT,UINT,BOOL);
void			Misc_MeasurePlayerComboBoxList(UINT,MEASUREITEMSTRUCT *);
void			Misc_DisplayPlayerComboBoxList(UINT,DRAWITEMSTRUCT *);

void			Misc_PrintButtonRightText(HWND,UINT,char *);
void			Misc_DrawButtonIcon(DRAWITEMSTRUCT *,HICON);

int			Misc_CreateUpDownButton(HWND,UINT,UINT);
void			Misc_SetUpDownInt(HWND,UINT,NM_UPDOWN *);
void			Misc_SetUpDownIntSigned(HWND,UINT,NM_UPDOWN *);

void			Misc_MeasureListViewItem(MEASUREITEMSTRUCT *,LONG);
void			Misc_DrawListViewItem(DRAWITEMSTRUCT *,LONG,LONG,DWORD);
int 			Misc_CreateCreaturesTreeList(TREELIST *);

/*** Misc_Edges.c *******************************************************/

void			Misc_DrawBevelBoxRect(HWND,HDC,RECT *,DWORD);
void			Misc_DrawBevelBox(HWND,HDC,LONG,LONG,LONG,LONG,DWORD);
void			Misc_DrawClassical3DBoxBold(HDC,LONG,LONG,LONG,LONG,DWORD);
void			Misc_DrawClassical3DBox(HDC,LONG,LONG,LONG,LONG,DWORD,UINT);

/*** Misc_Font.c ********************************************************/

int			Misc_GetFontInfo(HWND,FONTINFO *);

/*** Misc_Images.c ******************************************************/

int			Misc_CreateImageList(ELEMENT *,HIMAGELIST *,LONG,LONG,BOOL);
int			Misc_CreatePlayerImageList(ELEMENT *,HIMAGELIST *,LONG,LONG,UINT);
void			Misc_DestroyImageList(HIMAGELIST *);
int			Misc_CreateImage(HBITMAP *,UINT,LONG,LONG);
void			Misc_DestroyImage(HBITMAP *);

/*** Misc_List.c ********************************************************/

long			List_EntryCount(NODE *);
void			List_Move(NODE *,NODE *);
void			List_AddEntry(NODE *,NODE *);
void			List_InsertEntry(NODE *,NODE *,BOOL);
void			List_RemEntry(NODE *);
void			List_ReleaseMemory(NODE *);
long			List_CompareEntries(NODE *,NODE *);
int			List_FindSelectedItem(HWND);
int CALLBACK		List_SortItems(LPARAM,LPARAM,LPARAM);

/*** Misc_Maths.c *******************************************************/

DWORD			Misc_GetDlgItemFloatSigned(HWND,UINT);
DWORD			Misc_GetDlgItemFloat(HWND,UINT);
void			Misc_SetDlgItemFloatSigned(HWND,UINT,DWORD);
void			Misc_SetDlgItemFloat(HWND,UINT,DWORD);
void			Misc_FloatToText(DWORD,char *);

char*			Misc_DoubleToText(double,int,char *);
DWORD			Misc_CalculatePercent(DWORD,DWORD);

/*** Misc_Menus.c *******************************************************/

void			Misc_InitialiseMenus(HMENU,CUSTOMITEM *);
void			Misc_ResetMenus(CUSTOMITEM *);
void			Misc_DisplayContextMenu(HWND,UINT,CUSTOMITEM *,LONG,LONG,UINT);
void			Misc_CheckMenuItem(HMENU,UINT,UINT,UINT,BOOL);
void			Misc_FlagsChangeMenu(HMENU,DWORD,FLAGARRAY *);
void			Misc_MenuChangesFlags(HMENU,DWORD *,FLAGARRAY *);
int			Misc_IsMenuItemChecked(HMENU,UINT,BOOL);

void			Misc_DrawMenuItem(DRAWITEMSTRUCT *);
void			Misc_MeasureMenuItem(HWND,UINT,MEASUREITEMSTRUCT *);

/*** Misc_Requests.c ****************************************************/

void			Misc_PrintError(HWND,char *,char *,UINT);
int			Misc_CheckChanges(HWND,UINT);

int			Misc_MessageBoxExt(HWND,char *,char *,char *,UINT);
int			Misc_MessageBoxExtArgs(HWND,MESSAGEBOXSTRUCT *);
int CALLBACK		Misc_MessageBoxExtEnumFonts(LOGFONT *,TEXTMETRIC *,DWORD,LPARAM);
WORD*			Misc_MessageBoxExtFillDlgTemplate(WORD *,WORD,DWORD,WORD,char *);
BOOL WINAPI		Misc_MessageBoxExtProc(HWND,UINT,WPARAM,LPARAM);
void			Misc_MessageBoxExtInitialise(HWND,MESSAGEBOXSTRUCT *);
int			Misc_MessageBoxExtReset(HWND,int,MESSAGEBOXSTRUCT *);
void			Misc_MessageBoxExtDrawItem(DRAWITEMSTRUCT *,MESSAGEBOXSTRUCT *);

int			Misc_FileSelectSave(HWND,char *,UINT);
int			Misc_FileSelect(HWND,char *,UINT);
BOOL CALLBACK		Misc_FileSelectProc(HWND,UINT,WPARAM,LPARAM);
void			Misc_FileGet(HWND,FILEHEADER *);
void			Misc_FileDraw(DRAWITEMSTRUCT *,FILEHEADER *);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Propriétés							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Properties_Editor_Config.c *****************************************/

void			Config_SetToDefault(void);

void			Config_RemoveAll(HWND);
void			Config_RemoveObsolete(HWND);
void			Config_UpdateRecentMenu(void);
void			Config_AddRecent(char *);

char*			Config_GetUserFileName(char *,char *,BOOL);
int			Config_GetGamePath(char *);
void			Config_GetDisplayName(LPITEMIDLIST,STRRET *,char *);
int CALLBACK		Config_GamePathBrowser(HWND,UINT,LPARAM,LPARAM);
int			Config_GetPlayerPath(HWND);

/*** Properties_Editor_Registry.c ***************************************/

void			Registry_GetRecentFiles(HWND);
void			Registry_WriteRecentFiles(HWND);

void			Registry_SetSafeFlag(BOOL);
int			Registry_CheckSafeFlag(HWND);

void			Registry_SetGamePath(void);
void			Registry_GetGamePath(HWND);

/*** Properties_Global_Creatures.c **************************************/

void			Properties_GlobalCreatures(HWND);

BOOL CALLBACK		Properties_GlobalAttributesProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalParamsProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalFlagsProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalAttacksProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalResistancesProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalPossessionProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalJobsProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalLightProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalCrSpellsProc(HWND,UINT,WPARAM,LPARAM);

int			Properties_GlobalJobsCheckApply(HWND,GLOBALCTX2 *);
long			Properties_GlobalJobsGetTotal(UINT,GLOBALCTX2 *);
void			Properties_GlobalAddJob(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalRemJobs(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalUpdateJobs(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalToggleJob(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalDrawJob(DRAWITEMSTRUCT *,GLOBALCTX2 *);
void			Properties_GlobalChangeRes(HWND,DWORD);
void			Properties_GlobalChangeShots(HWND,DWORD);
void			Properties_GlobalSetAttractions(HWND,DWORD,GLOBALCTX2 *);
void			Properties_GlobalModifyValue(HWND,NM_UPDOWN *,GLOBALCTX2 *);
void			Properties_GlobalSetNewValue(HWND,GLOBALCTX2 *);
void			Properties_GlobalChangeValueType(HWND,GLOBALCTX2 *);
void			Properties_GlobalDrawAttributes(DRAWITEMSTRUCT *,GLOBALCTX2 *);

void			Properties_GlobalInitCr(HWND,UINT,PROPSHEETPAGE *);
int			Properties_GlobalSetValuesCr(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalSaveChangesCr(HWND,UINT,GLOBALCTX2 *);
void			Properties_GlobalGetParamsCr(HWND,UINT,GLOBALCTX2 *);

/*** Properties_Global_Default.c ****************************************/

int			Properties_LoadDefaultElements(HWND);
int			Properties_LoadDefaultCreatures(HWND);
void			Properties_FreeDefaultElements(void);
void			Properties_FreeDefaultCreatures(void);

int			Properties_LoadDefault(char *,char *,DWORD **,LONG,BYTE **,DWORD *);
void			Properties_LoadError(HWND,char *,char *,DWORD);

/*** Properties_Global_Elements.c ***************************************/

void			Properties_GlobalVariables(HWND);

BOOL CALLBACK		Properties_GlobalTerrainProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalRoomProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalDoorProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalTrapProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_GlobalSpellsProc(HWND,UINT,WPARAM,LPARAM);

void			Properties_GlobalSetTypeOptions(HWND,UINT,GLOBALCTX *);
BOOL CALLBACK		Properties_GlobalTypeProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_GlobalSetShotOptions(HWND,UINT,GLOBALCTX *);

void			Properties_GlobalInit(HWND,UINT,PROPSHEETPAGE *);
int			Properties_GlobalSetValues(HWND,UINT,GLOBALCTX *);
void			Properties_GlobalSaveChanges(HWND,UINT,GLOBALCTX *);
void			Properties_GlobalGetParams(HWND,UINT,GLOBALCTX *);

/*** Properties_Global_Functions.c **************************************/

int			Properties_GlobalShotOptions(HWND,GLOBALSHOT *);
BOOL CALLBACK		Properties_GlobalShotProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_GlobalShotInit(HWND,GLOBALSHOT *);
void			Properties_GlobalShotSave(HWND,GLOBALSHOT *);

void			Properties_GlobalReset(GLOBALVARS *,LONG,DWORD);
int			Properties_GlobalChanged(GLOBALVARS *,DWORD);

void			Properties_GlobalSetList(MEASUREITEMSTRUCT *);
void			Properties_GlobalDrawList(DRAWITEMSTRUCT *,ELEMENT *,HIMAGELIST);

/*** Properties_Map_Alliances.c *****************************************/

void			Properties_MapAlliances(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapAlliancesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapAlliancesDefault(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesHide(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesButton(HWND,MAPPROPSCTX *);
void			Properties_MapAlliancesDisplay(DRAWITEMSTRUCT *,MAPPROPSCTX *);

/*** Properties_Map_Computer.c ******************************************/

void			Properties_MapComputerOptions(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapComputerProc0(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc1(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc2(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc3(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc4(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc5(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc6(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc7(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK		Properties_MapComputerProc8(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapComputerInit(HWND,UINT,PROPSHEETPAGE *);
int			Properties_MapComputerSetValues(HWND,UINT,MAPPROPSCTX *);
int CALLBACK		Properties_MapComputerSort(LPARAM,LPARAM,LPARAM);
void			Properties_MapComputerChangeOrder(HWND,BOOL,MAPPROPSCTX *);
void			Properties_MapComputerToggleArrows(HWND,MAPPROPSCTX *);

/*** Properties_Map_Creatures.c *****************************************/

void			Properties_MapCreatures(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapCreaturesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapCreaturesInit(HWND,MAPPROPSCTX *);
void			Properties_MapCreaturesChanged(HWND,MAPPROPSCTX *);
void			Properties_MapCreaturesSetParam(HWND,UINT,MAPPROPSCTX *);
void			Properties_MapCreaturesDispInfo(LV_DISPINFO *,MAPPROPSCTX *);

/*** Properties_Map_Global.c ********************************************/

void			Properties_MapOptions(HWND);
void			Properties_RemoveThings(NODE *,LONG,LONG,BOOL *,UNDO **);
void			Properties_UpdateThings(NODE *,LONG,LONG,UNDO **);
void			Properties_RemoveRectEx(NODE *,LONG,LONG,BOOL *,UNDO **);
void			Properties_UpdateRectEx(NODE *,LONG,LONG,UNDO **);
void			Properties_UpdateTriggers(LONG,LONG,UNDO **);
void			Properties_UpdateProperties(LONG,LONG);
void			Properties_MapResetOptions(MAP *,MAPOPTIONS *);
void			Properties_MapOptionsToDefault(MAPOPTIONS *,UINT);
void			Properties_MapOptionsKeeperAI(UINT,MAPOPTIONS *);

BOOL CALLBACK		Properties_MapProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapInit(HWND,MAPPROPSCTX *);
int			Properties_MapReset(HWND,MAPPROPSCTX *,BOOL);
void			Properties_MapChangeSizeButton(HWND,MAPPROPSCTX *);
void			Properties_MapSizeButton(DRAWITEMSTRUCT *,MAPPROPSCTX *);
void			Properties_MapKeeperButton(DRAWITEMSTRUCT *);

/*** Properties_Map_Options.c *******************************************/

void			Properties_MapLevelOptions(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapLevelOptionsProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapLevelOptionsInit(HWND,MAPPROPSCTX *);
void			Properties_MapLevelOptionsSave(HWND,MAPPROPSCTX *);
void			Properties_MapLevelOptionsDrawBox(DRAWITEMSTRUCT *,MAPPROPSCTX *);
void			Properties_MapLevelOptionsDrawHorny(DRAWITEMSTRUCT *);

/*** Properties_Map_Players.c *******************************************/

void			Properties_MapPlayer(HWND,MAPPROPSCTX *,UINT);
BOOL CALLBACK		Properties_MapPlayerProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapPlayerInit(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerReset(HWND,BOOL,MAPPROPSCTX *);
void			Properties_MapPlayerToDefault(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerDrawItem(DRAWITEMSTRUCT *,MAPPROPSCTX *,LONG);
void			Properties_MapPlayerSetCamera(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerRoomsChange(HWND,UINT,MAPPROPSCTX *);
void			Properties_MapPlayerRoomsChanged(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerRoomsDispInfo(LV_DISPINFO *,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesChange(HWND,BOOL,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesChanged(HWND,MAPPROPSCTX *);
void			Properties_MapPlayerCreaturesDispInfo(LV_DISPINFO *,MAPPROPSCTX *);

/*** Properties_Map_Texts.c *********************************************/

void			Properties_MapTexts(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapTextsProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapTextsInit(HWND,MAPPROPSCTX *);
void			Properties_MapTextsSave(HWND,MAPPROPSCTX *);
void			Properties_MapTextsSwitchList(HWND);

void			Properties_MapTextsGetList(HWND,char *,NODE *);
void			Properties_MapTextsSetText(HWND,UINT,char*,LONG);
void			Properties_MapTextsGameToASCII(BYTE *,BYTE *,LONG);

/*** Properties_Map_Variables.c *****************************************/

void			Properties_MapVariables(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapVariablesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapVariablesInit(HWND,MAPPROPSCTX *);
void			Properties_MapVariablesDraw(DRAWITEMSTRUCT *,MAPPROPSCTX *);
void			Properties_MapVariablesModify(HWND,MAPPROPSCTX *);

/*** Properties_Map_Voices.c ********************************************/

void			Properties_MapVoices(HWND,MAPPROPSCTX *);
BOOL CALLBACK		Properties_MapVoicesProc(HWND,UINT,WPARAM,LPARAM);
void			Properties_MapVoicesInit(HWND,MAPPROPSCTX *);
void			Properties_MapVoicesStoreDatas(HWND,MAPPROPSCTX *);
void			Properties_MapVoicesCreateDisplayList(HWND,UINT,NODE *);
void			Properties_MapVoicesChangeFile(HWND,MAPPROPSCTX *);

void			Properties_MapVoicesDeleteTempFile(HWND,char *);
int			Properties_MapVoicesBuildList(NODE *);
int			Properties_MapVoicesGetSoundList(HWND,char *,LONG,NODE *);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Réductions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Thumbnail_Functions.c **********************************************/

HBITMAP			Thumbnail_Generate(HWND,MAP *,HBITMAP);
HBITMAP			Thumbnail_GenerateExt(HWND,MAP *,HBITMAP,THUMBOPTS *);
void			Thumbnail_CopyBitmap(HDC,LONG,LONG,LONG,HDC,LONG,LONG,HBITMAP,BOOL,COLORREF);
void			Thumbnail_GenerateFile(HWND,HBITMAP,HBITMAP,LONG);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Bibliothèques							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Libraries_Calls.c **************************************************/

int			Library_Call(HWND,UINT,void *);
HMODULE			Library_Open(HWND,char *,LONG);
void			Library_Close(HWND,HMODULE);


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Aide								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

/*** Help_Help.c ********************************************************/

void			Help_Content(void);

#endif
