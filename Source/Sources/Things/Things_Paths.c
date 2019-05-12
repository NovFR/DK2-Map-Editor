
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestionnaire des chemins
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
extern NODE		MapAreas;
extern LONG		MapChanges;
extern MAPRECT		MapCursorHigh;
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorSel;
extern UINT		SelectLast;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création et modification des chemins				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void RectEx_MakePaths(HWND hWnd)
{
	MAPRECTEXPATHCTX	*Context;
	RECT			 MapRect;
	int			 Result;

	if (!List_EntryCount(&MapAreas))
		{
		Misc_MessageBoxExt(hWnd,szPathNoArea,(char *)-1,szMessageBoxExtOk,MB_ICONINFORMATION);
		return;
		}

	Context = GlobalAlloc(GPTR,sizeof(MAPRECTEXPATHCTX));
	if (!Context)
		{
		MessageBeep(MB_ICONHAND);
		return;
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6710),hWnd,RectEx_PathsProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONEXCLAMATION);

	MapCursorHigh.hidden = TRUE;
	MapCursorRect.hidden = FALSE;
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	Files_SaveUpdate();
	GlobalFree(Context);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK RectEx_PathsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEXPATHCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		RectEx_PathsInit(hDlg,(MAPRECTEXPATHCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPRECTEXPATHCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case TVN_SELCHANGED:
					RectEx_PathsShowTVArea(Context);
					EnableWindow(GetDlgItem(hDlg,102),(RectEx_PathsIsButtonActivable(RECTEX_PATHADD,Context))?TRUE:FALSE);
					EnableWindow(GetDlgItem(hDlg,103),(RectEx_PathsIsButtonActivable(RECTEX_PATHINS,Context))?TRUE:FALSE);
					EnableWindow(GetDlgItem(hDlg,104),(RectEx_PathsIsButtonActivable(RECTEX_PATHKILL,Context))?TRUE:FALSE);
					EnableWindow(GetDlgItem(hDlg,105),(RectEx_PathsIsButtonActivable(RECTEX_PATHMODIFY,Context))?TRUE:FALSE);
					RectEx_PathsCheck(hDlg,FALSE,Context);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
							RectEx_PathsCreateNewPath(hDlg,Context);
							break;
						case 102:
							RectEx_PathsAddArea(hDlg,FALSE,Context);
							break;
						case 103:
							RectEx_PathsAddArea(hDlg,TRUE,Context);
							break;
						case 104:
							RectEx_PathsKill(hDlg,Context);
							break;
						case 105:
							RectEx_PathsModifyPause(hDlg,Context);
							break;
						case IDOK:
						case IDCANCEL:
							List_ReleaseMemory(&Context->list);
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void RectEx_PathsInit(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	RECT			 ClientRect;
	LONG			 X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	Context->hpathslist = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-30,hDlg,NULL,hInstance,NULL);
	if (!Context->hpathslist) goto Error_0;
	if (!RectEx_PathsSetGroups(Context)) goto Error_0;
	if (!RectEx_PathsRedrawAll(Context)) goto Error_0;
	if (RectEx_PathsIsButtonActivable(RECTEX_PATHNEW,Context)) EnableWindow(GetDlgItem(hDlg,101),TRUE);
	return;

//--- Erreurs ---

Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Création des chemins dans la liste ««««««««««««««««««««««««««««««»

int RectEx_PathsRedrawAll(MAPRECTEXPATHCTX *Context)
{
	MAPRECTEX		*Area;
	MAPRECTEXPATHLIST	*Path;
	TV_INSERTSTRUCT		 Item;
	LONG			 X;

	for (Path = (MAPRECTEXPATHLIST *)Context->list.next; Path != 0; Path = (MAPRECTEXPATHLIST *)Path->node.next)
		{
		Item.hParent = TVI_ROOT;
		Item.hInsertAfter = TVI_LAST;
		Item.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_CHILDREN|TVIF_STATE;
		Item.DUMMYUNIONNAME.item.stateMask = TVIS_EXPANDED;
		Item.DUMMYUNIONNAME.item.state = TVIS_EXPANDED;
		Item.DUMMYUNIONNAME.item.pszText = (char *)szPathList;
		Item.DUMMYUNIONNAME.item.cChildren = 1;
		Item.DUMMYUNIONNAME.item.lParam = (LPARAM)Path;
		Item.hParent = (HTREEITEM)SendMessage(Context->hpathslist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
		if (!Item.hParent) goto Error_0;
		for (X = 0; Path->areas[X] != 0; X++)
			{
			Area = RectEx_FindById(Path->areas[X],&MapAreas);
			Item.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_CHILDREN;
			Item.DUMMYUNIONNAME.item.cChildren = 0;
			Item.DUMMYUNIONNAME.item.lParam = Path->areas[X];
			if (Area)
				{
				RectEx_StrCpy(Context->name+2048,Area->name);
				RectEx_StrCpy(Context->name+2048+MAP_RECTMAXNAME,Area->info);
				if (Area->info[0]) wsprintf(Context->name,szPathAreaInfo,Area->waitdelay,Area->id,Context->name+2048,Context->name+2048+MAP_RECTMAXNAME);
				else wsprintf(Context->name,szPathAreaNoInfo,Area->waitdelay,Area->id,Context->name+2048);
				Item.DUMMYUNIONNAME.item.pszText = Context->name;
				}
			else
				{
				wsprintf((char *)Context->temp,szPathArea,Path->areas[X]);
				Item.DUMMYUNIONNAME.item.pszText = (char *)Context->temp;
				}
			if (!SendMessage(Context->hpathslist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item)) goto Error_0;
			}
		}

	return(1);

//--- Erreurs ---

Error_0:List_ReleaseMemory(&Context->list);
	return(0);
}


// «»»» Création des groupes de chemins «««««««««««««««««««««««««««««««««»

int RectEx_PathsSetGroups(MAPRECTEXPATHCTX *Context)
{
	MAPRECTEX		*Rect;
	MAPRECTEX		*NextRect;
	MAPRECTEXPATHLIST	*PathList;
	int			 Offset;
	int			 Offset2;
	int			 Offset3;
	int			 i,j;

	for (Rect = (MAPRECTEX *)MapAreas.next; Rect != 0; Rect = (MAPRECTEX *)Rect->node.next)
		{
		if (Rect->nextid != 0)
			{
			//
			// Check if this point is part of an existing path
			//
			for (PathList = (MAPRECTEXPATHLIST *)Context->list.next; PathList != 0; PathList = (MAPRECTEXPATHLIST *)PathList->node.next)
				if (RectEx_PathsIsPointUsed(Rect->id,&PathList->areas[0])) break;
			if (PathList)
				continue;
			//
			// Save the first point (not necessary the very first point)
			//
			PathList = GlobalAlloc(GPTR,sizeof(MAPRECTEXPATHLIST));
			if (!PathList)
				{
				List_ReleaseMemory(&Context->list);
				return(0);
				}
			List_AddEntry((NODE *)PathList,&Context->list);
			//
			// Saves all points belong to this path
			//
			Offset = 0;
			PathList->areas[Offset] = Rect->id;
			while ( (NextRect = RectEx_FindById(PathList->areas[Offset],&MapAreas)) != 0 )
				{
				if (RectEx_PathsIsPointUsed(NextRect->nextid,&PathList->areas[0]))
					{
					PathList->areas[++Offset] = NextRect->nextid;
					break;
					}
				PathList->areas[++Offset] = NextRect->nextid;
				}
			//
			// Check if we missed points
			//
			for (NextRect = (MAPRECTEX *)MapAreas.next; NextRect != 0;)
				{
				if ((NextRect->nextid)&&(RectEx_PathsIsPointUsed(NextRect->nextid,&PathList->areas[0]))&&(!RectEx_PathsIsPointUsed(NextRect->id,&PathList->areas[0])))
					{
					for (Offset = 0; PathList->areas[Offset] != 0; Offset++)
						if (PathList->areas[Offset] == NextRect->nextid) break;
					for (Offset2 = 256*2; Offset2 != Offset-1; Offset2--)
						PathList->areas[Offset2+1] = PathList->areas[Offset2];
					PathList->areas[Offset] = NextRect->id;
					NextRect = (MAPRECTEX *)MapAreas.next;
					continue;
					}
				NextRect = (MAPRECTEX *)NextRect->node.next;
				}
			//
			// If it is a complete loop, no matter of the order
			//
			for (Offset = 0; PathList->areas[Offset] != 0; Offset++);
			if (PathList->areas[Offset-1] == PathList->areas[0]) goto Check;
			//
			// Checks if there is a partial loop in the path
			//
			for (Offset = 0; PathList->areas[Offset] != 0; Offset++)
				{
				for (Offset2 = 0; PathList->areas[Offset2] != 0; Offset2++)
					{
					if (Offset2 == Offset) continue;
					if (PathList->areas[Offset2] == PathList->areas[Offset]) goto Rebuild;
					}
				}
			goto Check;
			//
			// Ensures the last point that not belong to the loop
			// points to the first point that belong to the loop.
			// 1 2 3 4 5 1
			//           ^--- Offset2
			//       ^--- Offset3
			// ^--- Offset
                        //
                Rebuild:NextRect = RectEx_FindById(PathList->areas[Offset-1],&MapAreas);
			if (!NextRect) goto Check;		// (!!!)
			for (Offset3 = Offset; Offset3 != Offset2; Offset3++)
				if (PathList->areas[Offset3] == NextRect->nextid) break;
			ZeroMemory(&Context->temp[0],256*2+2);
			for (i = 0; PathList->areas[Offset3+i+1] != 0; i++)
				Context->temp[i] = PathList->areas[Offset3+i];
			for (j = 0; j != Offset3-Offset; j++, i++)
				Context->temp[i] = PathList->areas[Offset+j];
			for (i = 0; i != Offset2-Offset; i++)
				PathList->areas[Offset+i] = Context->temp[i];
			PathList->areas[Offset+i] = PathList->areas[Offset];
			//
			// Removes the points that have a destination belonging
			// to an existing path but making, in fact, another path
			// (cf. Level 10 - 29->30->4 & 47->4)
			//
		Check:	for (Offset = 0; PathList->areas[Offset] != 0; Offset++)
				{
				NextRect = RectEx_FindById(PathList->areas[Offset],&MapAreas);
				if (!NextRect) continue;	// (!!!)
				if ((NextRect->nextid != PathList->areas[Offset+1])&&(PathList->areas[Offset+1]))
					{
					for (Offset2 = Offset+1; PathList->areas[Offset2] != 0; Offset2++)
						PathList->areas[Offset2] = PathList->areas[Offset2+1];
					Offset = -1;
					}
				}
			}
		}

	return(1);
}

//--- Test si une zone est utilisée dans un chemin ---

int RectEx_PathsIsPointUsed(UINT Area, UINT *Points)
{
	int	Offset;

	for (Offset = 0; Points[Offset] != 0; Offset++)
		if (Points[Offset] == Area) return(1);

	return(0);
}


// «»»» Test si un chemin a été modifié «««««««««««««««««««««««««««««««««»

void RectEx_PathsCheck(HWND hDlg, BOOL All, MAPRECTEXPATHCTX *Context)
{
	MAPRECTEX		*Area;
	MAPRECTEXPATHLIST	*Path;
	BOOL			 Modified;
	int			 Offset;

	if (!All)
		{
		TV_ITEM			 Item;

		Item.mask = TVIF_PARAM;
		Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
		if (!Item.hItem) return;
		if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
		if (Item.lParam <= 255)
			{
			Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Item.hItem);
			if (!Item.hItem) return;
			if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
			}
		Path = (MAPRECTEXPATHLIST *)Item.lParam;
		}
	else
		Path = (MAPRECTEXPATHLIST *)Context->list.next;

Loop:	if (!Path) return;

	for (Modified = FALSE, Offset = 0; Path->areas[Offset] != 0; Offset++)
		{
		Area = RectEx_FindById(Path->areas[Offset],&MapAreas);
		if (!Area) continue;
		if (Area->nextid != Path->areas[Offset+1])
			{
			if (Path->areas[Offset+1] == 0)
				{
				for (Offset = 0; Path->areas[Offset] != 0; Offset++)
					if (Path->areas[Offset] == Area->nextid) break;
				if (Path->areas[Offset] == 0)
					Modified = TRUE;
				break;
				}
			else
				{
				Modified = TRUE;
				break;
				}
			}
		}

	if ((!Modified)&&(All))
		{
		Path = (MAPRECTEXPATHLIST *)Path->node.next;
		if (Path) goto Loop;
		}

	if (Modified)
		{
		MESSAGEBOXSTRUCT	 Request;

		ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
		Request.gadgets = szMessageBoxExtOk;
		Request.windowtext = (All)?szPathOtherModified:szPathModified;
		Request.windowtitle = (char *)-1;
		Request.requestbits = &Config.RequestsFlags;
		Request.statebit = REQUEST_RECTEXMODPATH;
		Request.flags = MBF_AUTOREPLY;
		Request.defaultgadget = 1;
		Request.defaulticon = MB_ICONQUESTION;
		Request.windowpos = MBP_FOLLOWMOUSE;
		Misc_MessageBoxExtArgs(hDlg,&Request);
		}

	return;
}


// «»»» Montre la zone sélectionnée (TreeView) ««««««««««««««««««««««««««»

void RectEx_PathsShowTVArea(MAPRECTEXPATHCTX *Context)
{
	TV_ITEM		 Item;
	MAPCLIENT	 Client;
	MAPRECTEX	*Area;

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (Item.lParam > 255) return;

	Area = RectEx_FindById(Item.lParam,&MapAreas);
	if (!Area) return;

	Map_CorrectRect(&Area->rect,&Client);
	Map_ScrollToPointEx(hWnd,Client.X,Client.Y,Client.Width,Client.Height,SHOW_CURSOR);
	return;
}


// «»»» Test si un bouton peut être activé ««««««««««««««««««««««««««««««»

int RectEx_PathsIsButtonActivable(UINT Button, MAPRECTEXPATHCTX *Context)
{
	MAPRECTEX	*Area;
	TV_ITEM		 Item;
	UINT		 AreaID;
	int		 i;

	switch(Button)
		{
		case RECTEX_PATHNEW:
			for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
				if (!Area->nextid) return(1);
			return(0);
		case RECTEX_PATHADD:
			Item.mask = TVIF_PARAM;
			Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
			if (!Item.hItem) return(0);
			if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
			if (Item.lParam <= 255)
				{
				Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Item.hItem);
				if (!Item.hItem) return(0);
				if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
				}
			if (Item.lParam > 255)
				{
				for (i = 0; ((MAPRECTEXPATHLIST *)Item.lParam)->areas[i] != 0; i++);
				AreaID = ((MAPRECTEXPATHLIST *)Item.lParam)->areas[i-1];
				for (i = 0; ((MAPRECTEXPATHLIST *)Item.lParam)->areas[i+1] != 0; i++)
					if (((MAPRECTEXPATHLIST *)Item.lParam)->areas[i] == AreaID) return(0);
				}
			for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
				if (!Area->nextid) return(1);
			return(0);
		case RECTEX_PATHINS:
			Item.mask = TVIF_PARAM;
			Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
			if (!Item.hItem) return(0);
			if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
			if (Item.lParam > 255) return(0);
			Area = RectEx_FindById(Item.lParam,&MapAreas);
			if (!Area) return(0);
			for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
				if (!Area->nextid) return(1);
			return(0);
		case RECTEX_PATHKILL:
			if (!SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT)) return(0);
			return(1);
		case RECTEX_PATHMODIFY:
			Item.mask = TVIF_PARAM;
			Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
			if (!Item.hItem) return(0);
			if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
			if (Item.lParam > 255) return(0);
			Area = RectEx_FindById(Item.lParam,&MapAreas);
			if (!Area) return(0);
			return(1);
		}

	return(0);
}


// «»»» Création d'un nouveau chemin ««««««««««««««««««««««««««««««««««««»

void RectEx_PathsCreateNewPath(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	TV_INSERTSTRUCT		 Item;
	MAPRECTEXPATHLIST	*Path;

	Path = GlobalAlloc(GPTR,sizeof(MAPRECTEXPATHLIST));
	if (!Path) goto Error_0;

	Item.hParent = TVI_ROOT;
	Item.hInsertAfter = TVI_LAST;
	Item.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_CHILDREN|TVIF_STATE;
	Item.DUMMYUNIONNAME.item.stateMask = TVIS_EXPANDED;
	Item.DUMMYUNIONNAME.item.state = TVIS_EXPANDED;
	Item.DUMMYUNIONNAME.item.pszText = szPathList;
	Item.DUMMYUNIONNAME.item.cChildren = 1;
	Item.DUMMYUNIONNAME.item.lParam = (LPARAM)Path;
	Item.hParent = (HTREEITEM)SendMessage(Context->hpathslist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
	if (!Item.hParent) goto Error_1;

	List_AddEntry((NODE *)Path,&Context->list);
	SendMessage(Context->hpathslist,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)Item.hParent);
	return;

Error_1:GlobalFree(Path);
Error_0:Misc_PrintError(hDlg,szPathCantCreatePath,NULL,MB_ICONHAND);
	return;
}


// «»»» Suppression d'un chemin ou d'un lien ««««««««««««««««««««««««««««»

void RectEx_PathsKill(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	MESSAGEBOXSTRUCT	 Request;
	TV_ITEM			 Item;
	TV_ITEM			 Parent;
	TV_ITEM			 Previous;
	RECT			 MapRect;
	MAPRECTEXPATHLIST	*Path;
	MAPRECTEX		*TempArea;
	MAPRECTEX		*Area;
	UNDO			*Undo;
	UNDO			*Child;
	int			 Result;

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	Area = RectEx_FindById(Item.lParam,&MapAreas);

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets = szMessageBoxExtYesNo;
	Request.windowtext = (Area)?szPathAreaConfirmKill:szPathConfirmKill;
	Request.windowtitle = (char *)-1;
	Request.requestbits = &Config.RequestsFlags;
	Request.statebit = REQUEST_RECTEXKILLPATH;
	Request.flags = MBF_AUTOREPLY;
	Request.defaultgadget = 1;
	Request.defaulticon = MB_ICONQUESTION;
	Request.windowpos = MBP_FOLLOWMOUSE;
	Result = Misc_MessageBoxExtArgs(hDlg,&Request);
	if (!Result) return;

	if (Area)
		{
		//
		// Supprime les liens d'une zone
		//
		Parent.mask = TVIF_PARAM;
		Parent.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Item.hItem);
		if (!Parent.hItem) return;
		if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Parent)) return;
		Path = (MAPRECTEXPATHLIST *)Parent.lParam;

		if ((Path->areas[0] != 0)&&(Path->areas[1] != 0))
			{
			for (Result = 0; Path->areas[Result] != 0; Result++)
				{
				if (Path->areas[Result] == Item.lParam)
					{
					Previous.mask = TVIF_PARAM;
					Previous.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Item.hItem);
					if ((!Previous.hItem)&&(!Result)) break;
					if ((Previous.hItem)&&(!Result)) continue;
					if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Previous)) return;
					Area = RectEx_FindById(Previous.lParam,&MapAreas);
					if (!Area) return;
					if (Area->id == Path->areas[Result-1]) break;
					}
				}
			if (Path->areas[Result] == 0) return;

			Area = RectEx_FindById(Path->areas[Result],&MapAreas);
			if (!Area) return;

			Undo = Undo_CreateNewEntry(UNDO_REMOVEPATHLINK,szMapRemovePathLink);
			if (Result)
				{
				TempArea = RectEx_FindById(Path->areas[Result-1],&MapAreas);
				if (TempArea)
					{
					Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
					Undo_StoreEntry(Child,(NODE *)TempArea,UNDO_MODAREA);
					TempArea->nextid = Path->areas[Result+1];
					}
				}

			while(Path->areas[Result])
				{
				Path->areas[Result] = Path->areas[Result+1];
				Result++;
				}

			//
			// Si la zone est utilisée par le chemin et qu'elle ne se
			// trouve pas à la fin de la liste (ancienne boucle),
			// ne pas rompre le liens.
			//
			if (!RectEx_PathsIsPointUsed(Area->id,&Path->areas[0]))
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
				Area->nextid = 0;
				}
			else
				{
				for (Result = 0; Path->areas[Result] != 0; Result++);
				if ((!Result)||(Path->areas[Result-1] == Area->id))
					{
					Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
					Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
					Area->nextid = 0;
					}
				}
			}

		SendMessage(Context->hpathslist,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);
		}
	else
		{
		//
		// Supprime un chemin entier
		//
		Path = (MAPRECTEXPATHLIST *)Item.lParam;

		if ((Path->areas[0] != 0)&&(Path->areas[1] != 0))
			{
			Undo = Undo_CreateNewEntry(UNDO_KILLPATH,szMapKillPath);
			for (Result = 0; Path->areas[Result+1] != 0; Result++)
				{
				TempArea = RectEx_FindById(Path->areas[Result],&MapAreas);
				if (!TempArea) continue;		// (!!!)
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				Undo_StoreEntry(Child,(NODE *)TempArea,UNDO_MODAREA);
				TempArea->nextid = 0;
				}
			}

		SendMessage(Context->hpathslist,TVM_DELETEITEM,(WPARAM)0,(LPARAM)Item.hItem);
		List_RemEntry((NODE *)Path);
		GlobalFree(Path);
		}

	MapChanges++;
	EnableWindow(GetDlgItem(hDlg,101),(RectEx_PathsIsButtonActivable(RECTEX_PATHNEW,Context))?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,102),(RectEx_PathsIsButtonActivable(RECTEX_PATHADD,Context))?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,103),(RectEx_PathsIsButtonActivable(RECTEX_PATHINS,Context))?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,104),(RectEx_PathsIsButtonActivable(RECTEX_PATHKILL,Context))?TRUE:FALSE);
	EnableWindow(GetDlgItem(hDlg,105),(RectEx_PathsIsButtonActivable(RECTEX_PATHMODIFY,Context))?TRUE:FALSE);
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	RectEx_PathsCheck(hDlg,TRUE,Context);
	return;
}


// «»»» Ajout ou insertion d'une zone «««««««««««««««««««««««««««««««««««»

void RectEx_PathsAddArea(HWND hDlg, BOOL InsertPoint, MAPRECTEXPATHCTX *Context)
{
	TV_INSERTSTRUCT		 Insert;
	TV_ITEM			 Item;
	TV_ITEM			 Parent;
	TV_ITEM			 Previous;
	UNDO			*Undo;
	UNDO			*Child;
	MAPRECTEX		*Area;
	MAPRECTEXPATHLIST	*Path;
	int			 Offset;
	int			 Offset2;

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if ((InsertPoint)&&(Item.lParam > 255)) return;

	if (Item.lParam > 255)
		{
		Parent.mask = TVIF_PARAM;
		Parent.hItem = Item.hItem;
		Parent.lParam = Item.lParam;
		}
	else
		{
		Parent.mask = TVIF_PARAM;
		Parent.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)Item.hItem);
		if (!Parent.hItem) return;
		if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Parent)) return;
		}

	Path = (MAPRECTEXPATHLIST *)Parent.lParam;
	Context->useexisting = FALSE;
	Context->allowlinked = (InsertPoint)?FALSE:TRUE;
	if (!RectEx_PathsSelectArea(hDlg,Context)) return;

	if (InsertPoint)
		{
		//
		// Insertion d'un point
		//
		for (Offset = 0; Path->areas[Offset] != 0; Offset++)
			{
			if (Path->areas[Offset] == Item.lParam)
				{
				Previous.mask = TVIF_PARAM;
				Previous.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Item.hItem);
				if ((!Previous.hItem)&&(!Offset)) break;
				if ((Previous.hItem)&&(!Offset)) continue;
				if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Previous)) return;
				Area = RectEx_FindById(Previous.lParam,&MapAreas);
				if (!Area) return;
				if (Area->id == Path->areas[Offset-1]) break;
				}
			}
		if (Path->areas[Offset] == 0) return;
		for (Offset2 = 256*2; Offset2 != Offset-1; Offset2--) Path->areas[Offset2+1] = Path->areas[Offset2];
		Path->areas[Offset] = Context->area;

		Undo = Undo_CreateNewEntry(UNDO_INSERTPATHLINK,szMapInsertPathLink);
		if (Offset)
			{
			Area = RectEx_FindById(Path->areas[Offset-1],&MapAreas);
			if (Area)
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
				Area->nextid = Context->area;
				}
			}
		Area = RectEx_FindById(Context->area,&MapAreas);
		if (Area)
			{
			Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
			Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
			Area->waitdelay = Context->delay;
			Area->nextid = Path->areas[Offset+1];
			}

		Insert.hInsertAfter = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)Item.hItem);
		if (!Insert.hInsertAfter) Insert.hInsertAfter = TVI_FIRST;
		}
	else
		{
		//
		// Ajout d'un point
		//
		Undo = Undo_CreateNewEntry(UNDO_ADDPATHLINK,szMapAddPathLink);
		for (Offset = 0; Path->areas[Offset] != 0; Offset++);
		Path->areas[Offset] = Context->area;

		if (Offset)
			{
			Area = RectEx_FindById(Path->areas[Offset-1],&MapAreas);
			if (Area)
				{
				Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
				Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
				Area->nextid = Context->area;
				}
			}
		Area = RectEx_FindById(Context->area,&MapAreas);
		if (Area)
			{
			Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
			Undo_StoreEntry(Child,(NODE *)Area,UNDO_MODAREA);
			Area->waitdelay = Context->delay;
			}

		Insert.hInsertAfter = TVI_LAST;
		}

	//--- Add the new entry ---

	Area = RectEx_FindById(Context->area,&MapAreas);
	Insert.hParent = Parent.hItem;
	Insert.DUMMYUNIONNAME.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_CHILDREN;
	Insert.DUMMYUNIONNAME.item.cChildren = 0;
	Insert.DUMMYUNIONNAME.item.lParam = Context->area;
	if (Area)
		{
		RectEx_StrCpy(Context->name+2048,Area->name);
		RectEx_StrCpy(Context->name+2048+MAP_RECTMAXNAME,Area->info);
		if (Area->info[0]) wsprintf(Context->name,szPathAreaInfo,Area->waitdelay,Area->id,Context->name+2048,Context->name+2048+MAP_RECTMAXNAME);
		else wsprintf(Context->name,szPathAreaNoInfo,Area->waitdelay,Area->id,Context->name+2048);
		Insert.DUMMYUNIONNAME.item.pszText = Context->name;
		}
	else
		{
		wsprintf((char *)Context->temp,szPathArea,Context->area);
		Insert.DUMMYUNIONNAME.item.pszText = (char *)Context->temp;
		}

	MapChanges++;
	Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_INSERTITEM,(WPARAM)0,(LPARAM)&Insert);
	if (Item.hItem) SendMessage(Context->hpathslist,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)Item.hItem);
	return;
}


// «»»» Modification d'une pause ««««««««««««««««««««««««««««««««««««««««»

void RectEx_PathsModifyPause(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	MESSAGEBOXSTRUCT Request;
	TV_ITEM		 Item;
	MAPRECTEX	*Area;
	int		 Result;

	Item.mask = TVIF_PARAM;
	Item.hItem = (HTREEITEM)SendMessage(Context->hpathslist,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,(LPARAM)TVI_ROOT);
	if (!Item.hItem) return;
	if (!SendMessage(Context->hpathslist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (Item.lParam > 255) return;
	Area = RectEx_FindById(Item.lParam,&MapAreas);
	if (!Area) return;

	ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
	Request.gadgets		= szMessageBoxExtOkCancel;
	Request.windowtext	= szPathPause;
	Request.windowtitle	= (char *)-1;
	Request.editnumber	= Area->waitdelay;
	Request.flags		= MBF_ESNUMBER|MBF_NOBEEP|MBF_EDITTEXT|MBF_NUMBER;
	Request.defaulticon	= MB_ICONQUESTION;
	Request.min		= 0;
	Request.max		= 250;
	Request.windowpos	= MBP_FOLLOWMOUSE;
	Result = Misc_MessageBoxExtArgs(hDlg,&Request);
	if (!Result) return;

	Area->waitdelay = Request.editnumber;
	MapChanges++;

	Item.mask = TVIF_TEXT;
	RectEx_StrCpy(Context->name+2048,Area->name);
	RectEx_StrCpy(Context->name+2048+MAP_RECTMAXNAME,Area->info);
	if (Area->info[0]) wsprintf(Context->name,szPathAreaInfo,Area->waitdelay,Area->id,Context->name+2048,Context->name+2048+MAP_RECTMAXNAME);
	else wsprintf(Context->name,szPathAreaNoInfo,Area->waitdelay,Area->id,Context->name+2048);
	Item.pszText = Context->name;
	SendMessage(Context->hpathslist,TVM_SETITEM,(WPARAM)0,(LPARAM)&Item);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection d'une zone						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int RectEx_PathsSelectArea(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	int	Result;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6711),hDlg,RectEx_PathsSelectProc,(LPARAM)Context);
	if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONEXCLAMATION);
	if (Result == 0) return(0);
	return(1);
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK RectEx_PathsSelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPRECTEXPATHCTX	*Context;
	long			 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		RectEx_PathsSelectInit(hDlg,(MAPRECTEXPATHCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPRECTEXPATHCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
			return(TRUE);

		case WM_DRAWITEM:
			Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,32,1,0);
			return(TRUE);

		case WM_HSCROLL:
			switch(LOWORD(wParam))
				{
				case TB_BOTTOM:
				case TB_LINEDOWN:
				case TB_LINEUP:
				case TB_PAGEDOWN:
				case TB_PAGEUP:
				case TB_THUMBPOSITION:
				case TB_THUMBTRACK:
				case TB_TOP:
					if (((HWND)lParam == GetDlgItem(hDlg,102))&&(!Context->donotupdate))
						{
						Context->donotupdate = TRUE;
						lData = SendDlgItemMessage(hDlg,102,TBM_GETPOS,(WPARAM)0,(LPARAM)0);
						SetDlgItemInt(hDlg,103,lData,FALSE);
						Context->donotupdate = FALSE;
						break;
						}
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_ITEMCHANGED:
					RectEx_PathsSelectChanged(hDlg,Context);
					return(TRUE);
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 104:
							Misc_SetUpDownInt(hDlg,103,(NM_UPDOWN *)lParam);
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
						case 103:
							if (Context->donotupdate) break;
							Context->donotupdate = TRUE;
							lData = GetDlgItemInt(hDlg,103,NULL,FALSE);
							if (lData > 250)
								{
								lData = 250;
								SetDlgItemInt(hDlg,103,lData,FALSE);
								}
							if (lData < 0)
								{
								lData = 0;
								SetDlgItemInt(hDlg,103,lData,FALSE);
								}
							SendDlgItemMessage(hDlg,102,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lData);
							Context->donotupdate = FALSE;
							break;
						}
					return(TRUE);
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Context->delay = GetDlgItemInt(hDlg,103,NULL,FALSE);
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


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

void RectEx_PathsSelectInit(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	RECT			 ClientRect;
	LV_COLUMN		 Column;
	LV_ITEM			 Item;
	MAPRECTEX		*Area;
	LONG			 X,Y;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	Context->hareaslist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_NOCOLUMNHEADER|LVS_OWNERDRAWFIXED|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-30,hDlg,NULL,hInstance,NULL);
	if (!Context->hareaslist) goto Error_0;
	if (!Misc_CreateImageList(PathAreas,&Context->himagelist,32,32,FALSE)) goto Error_0;
	SendMessage(Context->hareaslist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->himagelist);

	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->hareaslist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	Item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.iImage = 0;
	Item.pszText = Context->name;

	for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
		{
		if ((Area->nextid)&&(!Context->allowlinked)) continue;
		RectEx_StrCpy(Context->name+2048,Area->name);
		RectEx_StrCpy(Context->name+2048+MAP_RECTMAXNAME,Area->info);
		if ((!Context->allowlinked)||(!Area->nextid))
			{
			if (Area->info[0]) wsprintf(Context->name,szPathAreaInfo,Area->waitdelay,Area->id,Context->name+2048,Context->name+2048+MAP_RECTMAXNAME);
			else wsprintf(Context->name,szPathAreaNoInfo,Area->waitdelay,Area->id,Context->name+2048);
			}
		else
			{
			if (Area->info[0]) wsprintf(Context->name,szPathAreaInfoLinked,Area->waitdelay,Area->id,Context->name+2048,Context->name+2048+MAP_RECTMAXNAME);
			else wsprintf(Context->name,szPathAreaNoInfoLinked,Area->waitdelay,Area->id,Context->name+2048);
			}
		Item.lParam = (LPARAM)Area;
		if (SendMessage(Context->hareaslist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_0;
		Item.iItem++;
		}

	Context->donotupdate = TRUE;
	SendDlgItemMessage(hDlg,102,TBM_SETTICFREQ,(WPARAM)10,(LPARAM)0);
	SendDlgItemMessage(hDlg,102,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,250));
	Misc_CreateUpDownButton(hDlg,103,104);
	SetDlgItemInt(hDlg,103,0,FALSE);
	Context->donotupdate = FALSE;
	return;

//--- Erreurs ---

Error_0:EndDialog(hDlg,-1);
	return;
}


// «»»» Changement de sélection «««««««««««««««««««««««««««««««««««««««««»

void RectEx_PathsSelectChanged(HWND hDlg, MAPRECTEXPATHCTX *Context)
{
	LV_ITEM	Item;

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(Context->hareaslist,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	if (Item.iItem == -1) goto Disable;
	if (!SendMessage(Context->hareaslist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) goto Disable;
	if (!Item.lParam) goto Disable;

	Context->area = ((MAPRECTEX *)Item.lParam)->id;
	Context->delay = ((MAPRECTEX *)Item.lParam)->waitdelay;
	SetDlgItemInt(hDlg,103,Context->delay,FALSE);
	EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
	return;

Disable:EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines d'édition						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Tracage d'un chemin (méthode rapide) ««««««««««««««««««««««««««««»

void RectEx_DrawPath(HWND hWnd, BOOL Continue, BOOL CreateNew)
{
	RECT		 MapRect;
	MAPRECTEX	*Area;
	MAPRECTEX	*MatchArea = NULL;

	if ((!Continue)&&(!CreateNew))
		return;

	for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
		{
		if ((CreateNew)&&(Area->nextid)) continue;
		if (MapCursorSel.x1 < Area->rect.x1) continue;
		if (MapCursorSel.y1 < Area->rect.y1) continue;
		if (MapCursorSel.x2 > Area->rect.x2) continue;
		if (MapCursorSel.y2 > Area->rect.y2) continue;
		if ((MapCursorSel.x1 == Area->rect.x1)&&(MapCursorSel.y1 == Area->rect.y1)&&(MapCursorSel.x2 == Area->rect.x2)&&(MapCursorSel.y2 == Area->rect.y2))
			{
			MatchArea = Area;
			break;
			}
		if (MatchArea != NULL)
			{
			MAPCLIENT	MatchClient;
			MAPCLIENT	AreaClient;

			Map_CorrectRect(&MatchArea->rect,&MatchClient);
			Map_CorrectRect(&Area->rect,&AreaClient);
			if ((AreaClient.Width > MatchClient.Width)&&(AreaClient.Height > MatchClient.Height))
				continue;
			}
		MatchArea = Area;
		}

	if (!MatchArea)
		{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
		}
	if ((!SelectLast)||(CreateNew))
		{
		SelectLast = MatchArea->id;
		return;
		}

	Area = RectEx_FindById(SelectLast,&MapAreas);
	if ((!Area)||(Area->nextid))
		{
		MessageBeep(MB_ICONHAND);
		return;
		}

	Undo_CreatePathPoint(Area,SelectLast);
	Area->nextid = MatchArea->id;
	SelectLast = MatchArea->id;

	MapChanges++;
	Files_SaveUpdate();
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
