
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre de rapport.
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
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorHigh;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage des erreurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage du rapport ««««««««««««««««««««««««««««««««««««««««««««»

int Compiler_Report(HWND hWnd, UINT ReportFrom, UINT MapType, NODE *List)
{
	ERRORCTX	Context;
	RECT		MapRect;
	int		Result;

	if (!List->next) return(1);
	ZeroMemory(&Context,sizeof(ERRORCTX));
	Context.reportfrom = ReportFrom;
	Context.type = MapType;
	Context.list = List;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6001),hWnd,Compiler_ReportProc,(LPARAM)&Context);
	if (Result == -1)
		{
		Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		Result = 0;
		}

	MapCursorHigh.hidden = TRUE;
	MapCursorRect.hidden = FALSE;
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);
	return(Result);
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Compiler_ReportProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	ERRORCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Compiler_ReportInit(hDlg,(ERRORCTX *)lParam);
		return(FALSE);
		}

	Context = (ERRORCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Compiler_ReportDraw((DRAWITEMSTRUCT *)lParam);
			return(TRUE);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_GETDISPINFO:
					Compiler_ReportInfo((LV_DISPINFO *)lParam,Context);
					return(TRUE);
				case LVN_ITEMCHANGED:
					Compiler_ReportShow(Context);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
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


// «»»» Initialisations de la boîte de dialogue «««««««««««««««««««««««««»

void Compiler_ReportInit(HWND hDlg, ERRORCTX *Context)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	ERRORNODE	*List;
	LONG		 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	switch(Context->reportfrom)
		{
		case REPORT_COMPATIBILITY:
			SetWindowText(hDlg,szSaveReportTitle);
			SetDlgItemText(hDlg,IDOK,szSaveReportSave);
			SetDlgItemText(hDlg,IDCANCEL,szSaveReportCancel);
			break;
		case REPORT_LOADERROR:
			SetWindowText(hDlg,szLoadReportTitle);
			SetDlgItemText(hDlg,IDCANCEL,szLoadReportClose);
			ShowWindow(GetDlgItem(hDlg,IDOK),SW_HIDE);
			break;
		}

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);
	Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_NOCOLUMNHEADER,ClientRect.left+4,ClientRect.top+14,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-20,hDlg,NULL,hInstance,NULL);
	if (!Context->hlist) goto Error;

	Context->himagelist = ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,1,1);
	if (!Context->himagelist) goto Error;
	for (X = 87; X != 89+1; X++)
		{
		HICON hIcon = LoadImage(hInstance,MAKEINTRESOURCE(X),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		if (!hIcon) goto Error1;
		ImageList_ReplaceIcon(Context->himagelist,-1,hIcon);
		DestroyIcon(hIcon);
		}
	SendMessage(Context->hlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->himagelist);

	Column.mask = LVCF_FMT|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-8-1-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	if (SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iSubItem = 0;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.iImage = I_IMAGECALLBACK;
	for (X = 0, List = (ERRORNODE *)Context->list->next; List != 0; List = (ERRORNODE *)List->node.next)
		{
		switch(Context->reportfrom)
			{
			case REPORT_COMPATIBILITY:
				if (Context->type == MAPFILE_EDITOR)
					{
					if (!List->version)
						continue;
					}
				else
					{
					if ((*List->text == '?')&&(!(Config.SaveReportFlags&CSR_INFOS)))
							continue;
					if ((*List->text == '#')&&(!(Config.SaveReportFlags&CSR_WARNINGS)))
							continue;
					if ((*List->text == '!')&&(!(Config.SaveReportFlags&CSR_ALERTS)))
							continue;
					}
				break;
			case REPORT_LOADERROR:
				break;
			}
		Item.iItem = X++;
		Item.lParam = (LPARAM)List;
		if (SendMessage(Context->hlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error;
		}

	switch(Context->reportfrom)
		{
		case REPORT_COMPATIBILITY:
			if (Context->type == MAPFILE_EDITOR)
				SetDlgItemText(hDlg,101,szSaveReportVersion);
			else
				SetDlgItemText(hDlg,101,szSaveReportCompatibility);
			break;
		case REPORT_LOADERROR:
			SetDlgItemText(hDlg,101,szLoadReportError);
			break;
		}

	SetFocus(GetDlgItem(hDlg,IDCANCEL));
	return;

//--- Erreurs d'initialisation ---

Error1:	ImageList_Destroy(Context->himagelist);
Error:	EndDialog(hDlg,-1);
	return;
}


// «»»» Affichage des informations ««««««««««««««««««««««««««««««««««««««»

void Compiler_ReportInfo(LV_DISPINFO *Info, ERRORCTX *Context)
{
	ERRORNODE	*Error;

	Error = (ERRORNODE *)Info->item.lParam;
	if (!Error) return;

	if (Info->item.mask&LVIF_IMAGE)
		{
		Info->item.iImage = 0;
		switch(Context->reportfrom)
			{
			case REPORT_COMPATIBILITY:
				if ((Context->type != MAPFILE_EDITOR)&&(*Error->text == '!')) Info->item.iImage = 1;
				else if (*Error->text == '?') Info->item.iImage = 2;
				break;
			case REPORT_LOADERROR:
				if (*Error->text == '!') Info->item.iImage = 1;
				else if (*Error->text == '?') Info->item.iImage = 2;
				break;
			}
		}

	if (Info->item.mask&LVIF_TEXT)
		strcpy(Info->item.pszText,Error->text+1);

	return;
}


// «»»» Affichage du texte ««««««««««««««««««««««««««««««««««««««««««««««»

void Compiler_ReportDraw(DRAWITEMSTRUCT *ItemStruct)
{
	COLORREF	TextColor;
	COLORREF	BackColor;
	char		Text[256];

	FillRect(ItemStruct->hDC,&ItemStruct->rcItem,(HBRUSH)(COLOR_WINDOW+1));
	DrawEdge(ItemStruct->hDC,&ItemStruct->rcItem,BDR_SUNKENINNER|BDR_SUNKENOUTER,BF_RECT);
	TextColor = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_WINDOWTEXT));
	BackColor = SetBkColor(ItemStruct->hDC,GetSysColor(COLOR_WINDOW));
	GetWindowText(ItemStruct->hwndItem,Text,256);
	DrawText(ItemStruct->hDC,Text,-1,&ItemStruct->rcItem,DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	SetBkColor(ItemStruct->hDC,BackColor);
	SetTextColor(ItemStruct->hDC,TextColor);
	return;
}


// «»»» Montre l'élément sélectionné ««««««««««««««««««««««««««««««««««««»

void Compiler_ReportShow(ERRORCTX *Context)
{
	LV_ITEM	Item;

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iItem = SendMessage(Context->hlist,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	if (Item.iItem == -1) return;
	if (!SendMessage(Context->hlist,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;
	if (((ERRORNODE *)Item.lParam)->x == -1) return;

	Map_ScrollToPointEx(hWnd,((ERRORNODE *)Item.lParam)->x,((ERRORNODE *)Item.lParam)->y,((ERRORNODE *)Item.lParam)->width,((ERRORNODE *)Item.lParam)->height,SHOW_CURSOR);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions diverses						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ajoute une erreur à la liste «««««««««««««««««««««««»

ERRORNODE* Compiler_ReportErrorText(NODE *Errors, UINT ErrorType, DWORD Data)
{
	ERRORNODE	*Error;

	if (!Errors) return(NULL);
	Error = GlobalAlloc(GPTR,sizeof(ERRORNODE)+128);
	if (!Error) return(NULL);
	Error->x = -1;
	Error->y = -1;
	Error->width = 1;
	Error->height = 1;
	Error->queued = 1;
	Error->text = (char *)Error+sizeof(ERRORNODE);
	Compiler_ReportErrorCopyText(ErrorType,Error->text,Data);
	List_AddEntry((NODE *)Error,Errors);
	return(Error);
}

ERRORNODE* Compiler_ReportErrorXY(NODE *Errors, UINT ErrorType, LONG X, LONG Y, DWORD Data)
{
	ERRORNODE	*Error;

	if (!Errors) return(NULL);
	Error = GlobalAlloc(GPTR,sizeof(ERRORNODE)+128);
	if (!Error) return(NULL);
	Error->x = X;
	Error->y = Y;
	Error->width = 1;
	Error->height = 1;
	Error->queued = 1;
	Error->text = (char *)Error+sizeof(ERRORNODE);
	Compiler_ReportErrorCopyText(ErrorType,Error->text,Data);
	List_AddEntry((NODE *)Error,Errors);
	return(Error);
}

ERRORNODE* Compiler_ReportErrorFull(NODE *Errors, UINT ErrorType, LONG X, LONG Y, LONG Width, LONG Height, DWORD Data)
{
	ERRORNODE	*Error;

	if (!Errors) return(NULL);
	Error = GlobalAlloc(GPTR,sizeof(ERRORNODE)+128);
	if (!Error) return(NULL);
	Error->x = X;
	Error->y = Y;
	Error->width = Width;
	Error->height = Height;
	Error->queued = 1;
	Error->text = (char *)Error+sizeof(ERRORNODE);
	Compiler_ReportErrorCopyText(ErrorType,Error->text,Data);
	List_AddEntry((NODE *)Error,Errors);
	return(Error);
}

void Compiler_ReportErrorCopyText(UINT ErrorType, char *Text, DWORD Data)
{
	char	*NoLF;
	int	 i;

	switch(ErrorType)
		{
		case REPORT_BADPLAYER:
			wsprintf(Text,szCompilerReportBadPlayer,Data);
			break;
		case REPORT_BADOPTION:
			wsprintf(Text,szCompilerReportBadOption,Data);
			break;
		case REPORT_BADTERRAIN:
			wsprintf(Text,szCompilerReportBadTerrain,Data);
			break;
		case REPORT_BADTXTTERRAIN:
			wsprintf(Text,szCompilerReportBadTextTerrain,(char)(Data>>8),(char)Data);
			break;
		case REPORT_BADCREATURE:
			wsprintf(Text,szCompilerReportBadCreature,Data);
			break;
		case REPORT_BADDOOR:
			wsprintf(Text,szCompilerReportBadDoor,Data);
			break;
		case REPORT_BADTRAP:
			wsprintf(Text,szCompilerReportBadTrap,Data);
			break;
		case REPORT_BADOBJECT:
			wsprintf(Text,szCompilerReportBadObject,Data);
			break;
		case REPORT_BADMAGICALOBJECT:
			wsprintf(Text,szCompilerReportBadMagic,Data);
			break;
		case REPORT_BADTHINGPLAYER:
			wsprintf(Text,szCompilerReportBadThingPlayer,Data);
			break;
		case REPORT_BADTHINGHEADER:
			wsprintf(Text,szCompilerReportBadThingHeader,Data);
			break;
		case REPORT_BADTRIGGERID:
			wsprintf(Text,szCompilerReportBadTriggerID,Data);
			break;
		case REPORT_UNKTHINGHEADER:
			wsprintf(Text,szCompilerReportUnkThingHeader,Data);
			break;
		case REPORT_THINGSFAILURE:
			strcpy(Text,szCompilerReportThingsFailure);
			break;
		case REPORT_UNKAREAFLAGS:
			wsprintf(Text,szCompilerReportUnkAreaFlags,Data);
			break;
		case REPORT_BADGATETYPE:
			wsprintf(Text,szCompilerReportBadGateType,Data);
			break;
		case REPORT_VARSFAILURE:
			strcpy(Text,szCompilerReportVarsFailure);
			break;
		case REPORT_BADVARIABLE:
			wsprintf(Text,szCompilerReportBadVariable,Data);
			break;
		case REPORT_BADVARCREATURE:
			wsprintf(Text,szCompilerReportBadVarCreature,Data);
			break;
		case REPORT_BADVARTYPE:
			wsprintf(Text,szCompilerReportBadVarType,Data);
			break;
		case REPORT_BADVARTHING:
			wsprintf(Text,szCompilerReportBadVarThing,Data);
			break;
		case REPORT_MAINFAILURE:
			strcpy(Text,szCompilerReportMainFailure);
			break;
		case REPORT_PLAYERS:
			strcpy(Text,szCompilerReportPlayers);
			break;
		case REPORT_PLAYERSFAILURE:
			strcpy(Text,szCompilerReportPlayersFailure);
			break;
		case REPORT_TRIGGERSFAILURE:
			strcpy(Text,szCompilerReportTriggersFailure);
			break;
		case REPORT_BADVARALLIE:
			wsprintf(Text,szCompilerReportBadAllie,Data);
			break;
		case REPORT_BADVARALLIED:
			wsprintf(Text,szCompilerReportBadAllied,Data);
			break;
		case REPORT_BADVARPARAM:
			wsprintf(Text,szCompilerReportBadParam,Data);
			break;
		case REPORT_RECTEXFAILURE:
			strcpy(Text,szCompilerReportRectExFailure);
			break;
		case REPORT_HEROSFAILURE:
			strcpy(Text,szCompilerReportHerosFailure);
			break;
		case REPORT_OBSOLETEVERSION:
			strcpy(Text,szCompilerReportObsoleteVersion);
			break;
		case REPORT_BADVERSION:
			wsprintf(Text,szCompilerReportBadVersion,Data);
			break;
		case REPORT_BADSIZE:
			strcpy(Text,szCompilerReportBadSize);
			break;
		case REPORT_BADRECTEX:
			wsprintf(Text,szCompilerReportBadRectEx,Data);
			break;
		case REPORT_BADTHING:
			wsprintf(Text,szCompilerReportBadThing,Data);
			break;
		case REPORT_BADHEADER:
			strcpy(Text,szCompilerReportBadHeader);
			break;
		case REPORT_BADTHINGBEHAVIOR:
			wsprintf(Text,szCompilerReportBadThingBehavior,Data);
			break;
		case REPORT_BADTHINGTARGET:
			wsprintf(Text,szCompilerReportBadThingTarget,Data);
			break;
		case REPORT_STATSFAILURE:
			strcpy(Text,szCompilerReportStatsFailure);
			break;
		case REPORT_SAVEINFO:
			strcpy(Text,szCompilerReportSaveInfo);
			break;
		case REPORT_SAVEPLAYERS:
			strcpy(Text,szCompilerReportSavePlayers);
			break;
		case REPORT_SAVETHINGS:
			strcpy(Text,szCompilerReportSaveThings);
			break;
		case REPORT_SAVEMAP:
			strcpy(Text,szCompilerReportSaveMap);
			break;
		case REPORT_SAVEVARIABLES:
			strcpy(Text,szCompilerReportSaveVariables);
			break;
		case REPORT_SAVETRIGGERS:
			strcpy(Text,szCompilerReportSaveTriggers);
			break;
		case REPORT_DUPLICATEDHEART:
			strcpy(Text,szCompilerReportDuplicatedHeart);
			break;
		case REPORT_BADHEARTSIZE:
			strcpy(Text,szCompilerReportBadHeartSize);
			break;
		case REPORT_BADPORTALSIZE:
			strcpy(Text,szCompilerReportBadPortalSize);
			break;
		case REPORT_BADGATESIZE:
			strcpy(Text,szCompilerReportBadGateSize);
			break;
		case REPORT_NOHEART:
			strcpy(Text,szCompilerReportNoHeart);
			break;
		case REPORT_RESERVED:
			strcpy(Text,szCompilerReportReserved);
			break;
		case REPORT_NOPLAYER1:
			strcpy(Text,szCompilerReportNoPlayer1);
			break;
		case REPORT_NOPLAYER2:
			strcpy(Text,szCompilerReportNoPlayer2);
			break;
		case REPORT_NOPLAYER3:
			strcpy(Text,szCompilerReportNoPlayer3);
			break;
		case REPORT_NOPLAYER4:
			strcpy(Text,szCompilerReportNoPlayer4);
			break;
		case REPORT_THING61:
			strcpy(Text,szCompilerReportThing61);
			break;
		case REPORT_THING70:
			strcpy(Text,szCompilerReportThing70);
			break;
		case REPORT_TRIGGER61:
			strcpy(Text,szCompilerReportTrigger61);
			break;
		case REPORT_TRIGGER70:
			strcpy(Text,szCompilerReportTrigger70);
			break;
		case REPORT_ACTION61:
			strcpy(Text,szCompilerReportAction61);
			break;
		case REPORT_ACTION70:
			strcpy(Text,szCompilerReportAction70);
			break;
		case REPORT_CREATUREPB:
			wsprintf(Text,szCompilerReportCreaturePb,Data);
			break;
		case REPORT_DOORPB:
			wsprintf(Text,szCompilerReportDoorPb,Data);
			break;
		case REPORT_TRAPPB:
			wsprintf(Text,szCompilerReportTrapPb,Data);
			break;
		case REPORT_OBJECTPB:
			wsprintf(Text,szCompilerReportObjectPb,Data);
			break;
		case REPORT_MAGICPB:
			wsprintf(Text,szCompilerReportMagicPb,Data);
			break;
		case REPORT_NOTASQUARE:
			strcpy(Text,szCompilerReportNotASquare);
			break;
		case REPORT_TRSUNUSED:
			strcpy(Text,szCompilerReportTrsUnused);
			break;
		case REPORT_TRSMISSINGID:
			strcpy(Text,szCompilerReportTrsMissingId);
			break;
		case REPORT_TRSALREADYUSED:
			strcpy(Text,szCompilerReportTrsAlreadyUsed);
			break;
		case REPORT_ACSUNUSED:
			strcpy(Text,szCompilerReportAcsUnused);
			break;
		case REPORT_ACSMISSINGID:
			strcpy(Text,szCompilerReportAcsMissingId);
			break;
		case REPORT_ACSALREADYUSED:
			strcpy(Text,szCompilerReportAcsAlreadyUsed);
			break;
		case REPORT_REPAIRED:
			strcpy(Text,szCompilerReportRepaired);
			break;
		case REPORT_RESERVED2:
			strcpy(Text,szCompilerReportReserved2);
			break;
		case REPORT_CREATURETERRAIN:
			strcpy(Text,szCompilerReportCreatureTerrain);
			break;
		case REPORT_OBJECTTERRAIN:
			strcpy(Text,szCompilerReportObjectTerrain);
			break;
		case REPORT_MAGICTERRAIN:
			strcpy(Text,szCompilerReportMagicTerrain);
			break;
		case REPORT_BADVARIABLEDATA:
			wsprintf(Text,szCompilerReportBadVariableData,Data);
			break;
		case REPORT_AREASFAILURE:
			strcpy(Text,szCompilerReportAreasFailure);
			break;
		case REPORT_GATESFAILURE:
			strcpy(Text,szCompilerReportGatesFailure);
			break;
		case REPORT_UNKASCIIID:
			strcpy(Text,szCompilerReportUnkASCIIId);
			NoLF = strchr(Text,0);
			*NoLF++ = '\"';
			for (i = 0;; i++)
				{
				switch( *((char *)Data) )
					{
					case 0x20:
					case 0x09:
					case 0x0A:
					case 0x0D:
						i = -1;
						break;
					default:
						*NoLF++ = *((char *)Data)++;
						break;
					}
				if (i == -1) break;
				if (i >= 32)
					{
					*NoLF++ = '.';
					*NoLF++ = '.';
					*NoLF++ = '.';
					break;
					}
				}
			*NoLF++ = '\"';
			*NoLF = 0x00;
			break;
		case REPORT_ACSNOCRYSTAL:
			strcpy(Text,szCompilerReportAcsNoCrystal);
			break;
		case REPORT_ACSSQOUTOFMAP:
			strcpy(Text,szCompilerReportAcsSqOutOfMap);
			break;
		case REPORT_ACSCROUTOFMAP:
			strcpy(Text,szCompilerReportAcsCrOutOfMap);
			break;
		case REPORT_ACSRMOUTOFMAP:
			strcpy(Text,szCompilerReportAcsRmOutOfMap);
			break;
		case REPORT_PETBAND:
			strcpy(Text,szCompilerReportPetBand);
			break;
		case REPORT_CAMERA:
			wsprintf(Text,szCompilerReportCamera,Data);
			break;
		case REPORT_CPUAREA:
			wsprintf(Text,szCompilerReportCPUArea,Data);
			break;
		case REPORT_UNUSEDTRCNT:
			wsprintf(Text,szCompilerReportUnusedTrCnt,Data);
			break;
		case REPORT_UNUSEDACCNT:
			wsprintf(Text,szCompilerReportUnusedAcCnt,Data);
			break;
		case REPORT_UNUSEDREM:
			strcpy(Text,szCompilerReportUnusedRem);
			break;
		case REPORT_UNKASCIIDATA:
			strcpy(Text,szCompilerReportUnknownASCIIData);
			break;
		case REPORT_MISSINGBAND:
			wsprintf(Text,szCompilerReportMissingBand,Data);
			break;
		case REPORT_MISSINGAREA:
			wsprintf(Text,szCompilerReportMissingArea,Data);
			break;
		case REPORT_MISSINGAREAPATH:
			wsprintf(Text,szCompilerReportMissingAreaPath,Data);
			break;
		case REPORT_SAVEGTERRAINS:
			strcpy(Text,szCompilerReportSaveGTerrains);
			break;
		case REPORT_SAVEGROOMS:
			strcpy(Text,szCompilerReportSaveGRooms);
			break;
		case REPORT_SAVEGDOORS:
			strcpy(Text,szCompilerReportSaveGDoors);
			break;
		case REPORT_SAVEGTRAPS:
			strcpy(Text,szCompilerReportSaveGTraps);
			break;
		case REPORT_SAVEGSPELLS:
			strcpy(Text,szCompilerReportSaveGSpells);
			break;
		case REPORT_GLOBALSFAILURE:
			strcpy(Text,szCompilerReportGlobalsFailure);
			break;
		case REPORT_NOGATE22:
			strcpy(Text,szCompilerReportNoGate22);
			break;
		case REPORT_NOGATE6:
			strcpy(Text,szCompilerReportNoGate6);
			break;
		case REPORT_NOGATE7:
			strcpy(Text,szCompilerReportNoGate7);
			break;
		case REPORT_NOAREA22:
			strcpy(Text,szCompilerReportNoArea22);
			break;
		case REPORT_NOTDESTROYABLE:
			strcpy(Text,szCompilerReportNotDestroyable);
			break;
		case REPORT_SAVEGCREATURES:
			strcpy(Text,szCompilerReportSaveGCreatures);
			break;
		case REPORT_SAVEGCRSPELLS:
			strcpy(Text,szCompilerReportSaveGCrSpells);
			break;
		case REPORT_LASTERROR:
			if (Compiler_GetLastError())
				wsprintf(Text,szCompilerReportLastError,Compiler_GetErrorCodeText());
			break;
		}

	NoLF = strrchr(Text,'\r');
	if (NoLF) *NoLF = 0;
	NoLF = strrchr(Text,'\n');
	if (NoLF) *NoLF = 0;

	if (strlen(Text) > 128)
		MessageBox(hWnd,"Buffer overflow in compiler report error","Debug",MB_ICONHAND|MB_OK);

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
