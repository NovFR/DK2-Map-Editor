
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre de compilation
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
extern FONTINFO		Font;
extern COMPILERTASK*	CompilerTask;
extern MAP		Map;
extern MAPRECT		MapCursorRect;
extern MAPRECT		MapCursorHigh;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création & destruction de la fenêtre				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

int Compiler_CreateWindow(char *MapPath, UINT Method)
{
	if ((CompilerTask)&&(!CompilerTask->hWnd)) return(0);
	if ((CompilerTask)&&(!DestroyWindow(CompilerTask->hWnd))) return(0);

	//--- Allocation des zones mémoires de travail ---

	CompilerTask = GlobalAlloc(GPTR,sizeof(COMPILERTASK));
	if (!CompilerTask)
		{
		Misc_PrintError(hWnd,szWindowErr,NULL,MB_ICONHAND);
		return(0);
		}
	CompilerTask->MapCtrl = GlobalAlloc(GPTR,Map.Properties.Width*Map.Properties.Height);
	if (!CompilerTask->MapCtrl)
		{
		Compiler_DestroyWindow();
		Misc_PrintError(hWnd,szWindowErr,NULL,MB_ICONHAND);
		return(0);
		}

	//--- Initialisation des données ---

	CompilerTask->Method = Method;

	switch(CompilerTask->Method)
		{
		case COMPILER_COMPILE:
		case COMPILER_RUN:
			CopyMemory(CompilerTask->MapPath,MapPath,MAX_PATH);
			CompilerTask->Generate = TRUE;
			break;
		}

	//--- Création de la fenêtre ---

	CompilerTask->hWnd = CreateWindowEx(0,szAppName,NULL,WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,Config.CompilerWindowX,Config.CompilerWindowY,Config.CompilerWindowWidth,Config.CompilerWindowHeight,hWnd,NULL,hInstance,(void *)WINDOW_COMPILER);
	if (!CompilerTask->hWnd)
		{
		Compiler_DestroyWindow();
		Misc_PrintError(hWnd,szWindowErr,NULL,MB_ICONHAND);
		return(0);
		}

	ShowWindow(CompilerTask->hWnd,SW_SHOWNORMAL);
	UpdateWindow(CompilerTask->hWnd);
	return(1);
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Compiler_DestroyWindow()
{
	if (!CompilerTask) return;

	if (CompilerTask->hWnd)
		DestroyWindow(CompilerTask->hWnd);
	else
		{
		if (CompilerTask->MapCtrl) GlobalFree(CompilerTask->MapCtrl);
		GlobalFree(CompilerTask);
		CompilerTask = NULL;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages provenants de la fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LONG Compiler_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	paintStruct;

	if (!CompilerTask) return(DefWindowProc(hWnd,uMsgId,wParam,lParam));

	switch(uMsgId)
		{
		case WM_DISABLE:
			EnableWindow(CompilerTask->hButton,!wParam);
			if (CompilerTask->Failed) SetWindowText(CompilerTask->hButton,szClose);
			break;

		case WM_PRINTTEXT:
			Compiler_PrintText((char *)lParam,(DWORD)wParam);
			break;

		case WM_REPORTERROR:
			Compiler_QueueError((UINT)wParam,(DWORD)lParam);
			break;

		case WM_REPORTERROREX:
			Compiler_QueueErrorEx((ERRORNODE *)lParam);
			break;

		case WM_UPDATEPROGRESS:
			SendMessage(CompilerTask->hProgress,PBM_SETPOS,(WPARAM)lParam,(LPARAM)0);
			break;

		case WM_SIZING:
			if (((RECT *)lParam)->right-((RECT *)lParam)->left < Font.FontMaxWidth*10+4+200)
				((RECT *)lParam)->right = ((RECT *)lParam)->left+Font.FontMaxWidth*10+4+200;
			if (((RECT *)lParam)->bottom-((RECT *)lParam)->top < 8+Font.FontHeight*11+8+8+12)
				((RECT *)lParam)->bottom = ((RECT *)lParam)->top+8+Font.FontHeight*11+8+8+12;
			break;

		case WM_SIZE:
			Compiler_ResizeWindows(hWnd);
			break;

		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,16);
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType != ODT_LISTVIEW) break;
			Misc_DrawListViewItem((DRAWITEMSTRUCT *)lParam,16,1,0);
			break;

		case WM_PAINT:
			if (!GetUpdateRect(hWnd,NULL,FALSE)) break;
			BeginPaint(hWnd,&paintStruct);
			EndPaint(hWnd,&paintStruct);
			break;

		case WM_ERASEBKGND:
			if (!GetUpdateRect(hWnd,&paintStruct.rcPaint,FALSE)) break;
			FillRect((HDC)wParam,&paintStruct.rcPaint,(HBRUSH)(COLOR_MENU+1));
			return(1);

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_GETDISPINFO:
					Compiler_CompilerInfo((LV_DISPINFO *)lParam);
					return(TRUE);
				case LVN_ITEMCHANGED:
					Compiler_CompilerShow();
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			if ((HWND)lParam == CompilerTask->hButton)
				{
				switch(CompilerTask->Method)
					{
					case COMPILER_COMPILE:
					case COMPILER_TEST:
						PostMessage(hWnd,WM_CLOSE,(WPARAM)0,(LPARAM)0);
						break;
					case COMPILER_RUN:
						if (CompilerTask->Failed)
							PostMessage(hWnd,WM_CLOSE,(WPARAM)0,(LPARAM)0);
						else
							{
							Compiler_TaskRun(CompilerTask);
							CompilerTask->Method = COMPILER_TEST;
							SetWindowText(CompilerTask->hButton,szClose);
							}
						break;
					}
				}
			break;

		case WM_DESTROY:
			Compiler_Destroy(hWnd);
			break;
		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Initialisation des données de la fenêtre ««««««««««««««««««««««««»

LONG Compiler_Create(HWND hChild)
{
	LV_COLUMN	Column;
	RECT		ClientRect;
	LONG		Width,Height;
	LONG		X,Y;

	if (Config.CompilerWindowX == CW_USEDEFAULT)
		{
		Window_Center(hChild,NULL,&X,&Y);
		SetWindowPos(hChild,hWnd,X,Y,0,0,SWP_NOSIZE);
		}
	else
		SetWindowPos(hChild,hWnd,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	//--- Initialisation des fenêtres ---

	GetClientRect(hChild,&ClientRect);
	Width = ClientRect.right-8;
	Height = ClientRect.bottom-8-(Font.FontHeight+12+8+8);
	X = 4;
	Y = 4;

	CompilerTask->hText = CreateWindowEx(WS_EX_CLIENTEDGE,szEditClass,NULL,WS_VSCROLL|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY,X,Y,Width,Height*40/100,hChild,NULL,hInstance,NULL);
	if (!CompilerTask->hText) return(-1);

	Y += 8+Height*40/100;
	CompilerTask->hList = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER|LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_SINGLESEL,X,Y,Width,Height*60/100,hChild,NULL,hInstance,NULL);
	if (!CompilerTask->hList) return(-1);
	Column.mask = LVCF_SUBITEM|LVCF_WIDTH;
	Column.iSubItem = 0;
	Column.cx = Width-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	if (SendMessage(CompilerTask->hList,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) return(-1);

	Y += 8+Height*60/100;
	Width -= Font.FontMaxWidth*10+4;
	CompilerTask->hProgress = CreateWindowEx(WS_EX_CLIENTEDGE,szProgressClass,NULL,WS_CHILD|WS_VISIBLE,X,Y,Width,Font.FontHeight+12,hChild,NULL,hInstance,NULL);
	if (!CompilerTask->hProgress) return(-1);

	CompilerTask->hButton = CreateWindowEx(0,szButtonClass,NULL,WS_CHILD|WS_VISIBLE,X+Width+4,Y,Font.FontMaxWidth*10,Font.FontHeight+12,hChild,NULL,hInstance,NULL);
	if (!CompilerTask->hButton) return(-1);

	//--- Initialisation des caractères ---

	SendMessage(CompilerTask->hText,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);
	SendMessage(CompilerTask->hButton,WM_SETFONT,(WPARAM)Font.hFont,(LPARAM)0);

	//--- Initialisation des icônes ---

	CompilerTask->hImageList = ImageList_Create(16,16,ILC_COLORDDB|ILC_MASK,1,1);
	if (!CompilerTask->hImageList) return(-1);

	for (X = 87; X != 89+1; X++)
		{
		HICON hIcon = LoadImage(hInstance,MAKEINTRESOURCE(X),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
		if (!hIcon)
			{
			ImageList_Destroy(CompilerTask->hImageList);
			return(-1);
			}
		ImageList_ReplaceIcon(CompilerTask->hImageList,-1,hIcon);
		DestroyIcon(hIcon);
		}
	SendMessage(CompilerTask->hList,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)CompilerTask->hImageList);

	//--- Mise à jour des textes et des paramètres ---

	switch(CompilerTask->Method)
		{
		case COMPILER_COMPILE:
			SetWindowText(hChild,szCompilerCompile);
			SetWindowText(CompilerTask->hButton,szClose);
			SendMessage(CompilerTask->hProgress,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,COMPILER_PBMCMPMAX));
			break;
		case COMPILER_TEST:
			SetWindowText(hChild,szCompilerTest);
			SetWindowText(CompilerTask->hButton,szClose);
			SendMessage(CompilerTask->hProgress,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,COMPILER_PBMTSTMAX));
			break;
		case COMPILER_RUN:
			SetWindowText(hChild,szCompilerRun);
			SetWindowText(CompilerTask->hButton,szRun);
			SendMessage(CompilerTask->hProgress,PBM_SETRANGE,(WPARAM)0,(LPARAM)MAKELPARAM(0,COMPILER_PBMRUNMAX));
			break;
		}

	return(0);
}


// «»»» Destruction de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Compiler_Destroy(HWND hChild)
{
	RECT	MapRect;

	MapCursorHigh.hidden = TRUE;
	Map_GetMapArea(hWnd,&MapRect);
	InvalidateRect(hWnd,&MapRect,FALSE);
	UpdateWindow(hWnd);

	if ((!IsZoomed(hChild))&&(!IsIconic(hChild)))
		{
		GetWindowRect(hChild,&MapRect);
		Config.CompilerWindowX = MapRect.left;
		Config.CompilerWindowY = MapRect.top;
		Config.CompilerWindowWidth = MapRect.right-MapRect.left;
		Config.CompilerWindowHeight = MapRect.bottom-MapRect.top;
		}	
	List_ReleaseMemory(&CompilerTask->Errors);
	GlobalFree(CompilerTask->MapCtrl);
	GlobalFree(CompilerTask);
	CompilerTask = NULL;
	SetActiveWindow(hWnd);
	return;
}


// «»»» Change les dimensions des fenêtres ««««««««««««««««««««««««««««««»

void Compiler_ResizeWindows(HWND hWnd)
{
	RECT	ClientRect;
	LONG	X,Y;
	LONG	Width,Height;

	GetClientRect(hWnd,&ClientRect);
	Width = ClientRect.right-8;
	Height = ClientRect.bottom-8-(Font.FontHeight+12+8+8);
	X = 4;
	Y = 4;
	SendMessage(CompilerTask->hList,LVM_SETCOLUMNWIDTH,(WPARAM)0,(LPARAM)MAKELPARAM(Width-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2,0));
	SetWindowPos(CompilerTask->hText,NULL,X,Y,Width,Height*40/100,SWP_NOZORDER);
	Y += 8+Height*40/100;
	SetWindowPos(CompilerTask->hList,NULL,X,Y,Width,Height*60/100,SWP_NOZORDER);
	Y += 8+Height*60/100;
	Width -= Font.FontMaxWidth*10+4;
	SetWindowPos(CompilerTask->hProgress,NULL,X,Y,Width,Font.FontHeight+12,SWP_NOZORDER);
	SetWindowPos(CompilerTask->hButton,NULL,X+Width+4,Y,Font.FontMaxWidth*10,Font.FontHeight+12,SWP_NOZORDER);
	return;
}


// «»»» Affichage des informations ««««««««««««««««««««««««««««««««««««««»

void Compiler_CompilerInfo(LV_DISPINFO *Info)
{
	ERRORNODE	*Error;

	Error = (ERRORNODE *)Info->item.lParam;
	if (!Error) return;

	if (Info->item.mask&LVIF_IMAGE)
		{
		Info->item.iImage = 0;
		if (*Error->text == '!') Info->item.iImage = 1;
		else if (*Error->text == '?') Info->item.iImage = 2;
		}

	if (Info->item.mask&LVIF_TEXT)
		strcpy(Info->item.pszText,Error->text+1);

	return;
}

// «»»» Montre l'élément sélectionné ««««««««««««««««««««««««««««««««««««»

void Compiler_CompilerShow()
{
	LV_ITEM	Item;

	Item.mask = LVIF_PARAM;
	Item.iItem = -1;
	Item.iItem = SendMessage(CompilerTask->hList,LVM_GETNEXTITEM,(WPARAM)Item.iItem,(LPARAM)MAKELPARAM(LVNI_ALL|LVNI_SELECTED,0));
	if (Item.iItem == -1) return;
	if (!SendMessage(CompilerTask->hList,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
	if (!Item.lParam) return;
	if (((ERRORNODE *)Item.lParam)->x == -1) return;

	Map_ScrollToPointEx(hWnd,((ERRORNODE *)Item.lParam)->x,((ERRORNODE *)Item.lParam)->y,((ERRORNODE *)Item.lParam)->width,((ERRORNODE *)Item.lParam)->height,SHOW_CURSOR);
	MapCursorRect.hidden = FALSE;
	return;
}


// «»»» Affichage d'un texte ««««««««««««««««««««««««««««««««««««««««««««»

void Compiler_PrintText(char *Text, DWORD Data)
{
	int	TextLen;

	if (Text == (char *)-1)
		{
		Misc_FmtDate(CompilerTask->Date,NULL);
		strcat(CompilerTask->Text,szCompilerBegin);
		strcat(CompilerTask->Text,Map.Properties.Name);
		strcat(CompilerTask->Text,szCompilerBeginEnd);
		strcat(CompilerTask->Text,CompilerTask->Date);
		strcat(CompilerTask->Text,szCRLF);
		strcat(CompilerTask->Text,szCRLF);
		}
	else if (Text == (char *)-2)
		{
		wsprintf(CompilerTask->Date,szCompilerWarnings,Data);
		strcat(CompilerTask->Text,CompilerTask->Date);
		}
	else
		strcat(CompilerTask->Text,Text);

	SetWindowText(CompilerTask->hText,CompilerTask->Text);
	TextLen = strlen(CompilerTask->Text);
	SendMessage(CompilerTask->hText,EM_SETSEL,(WPARAM)TextLen,(LPARAM)TextLen);
	SendMessage(CompilerTask->hText,EM_SCROLLCARET,(WPARAM)0,(LPARAM)0);
	return;
}


// «»»» Ajoute un nouveau message d'erreur ««««««««««««««««««««««««««««««»

//--- Texte simple ---

void Compiler_QueueError(UINT ErrorType, DWORD Data)
{
	LV_ITEM		 Item;
	ERRORNODE	*Error;

	Error = Compiler_ReportErrorText(&CompilerTask->Errors,ErrorType,Data);
	if (!Error) return;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = SendMessage(CompilerTask->hList,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0)+1;
	Item.iSubItem = 0;
	Item.iImage = I_IMAGECALLBACK;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.lParam = (LPARAM)Error;
	Item.iItem = SendMessage(CompilerTask->hList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
	SendMessage(CompilerTask->hList,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)FALSE);
	return;
}

//--- Texte avec dimensions ---

void Compiler_QueueErrorEx(ERRORNODE *Temp)
{
	LV_ITEM		 Item;
	ERRORNODE	*Error;

	Error = Compiler_ReportErrorFull(&CompilerTask->Errors,Temp->node.type,Temp->x,Temp->y,Temp->width,Temp->height,(DWORD)Temp->text);
	if (!Error) return;

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = SendMessage(CompilerTask->hList,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0)+1;
	Item.iSubItem = 0;
	Item.iImage = I_IMAGECALLBACK;
	Item.pszText = LPSTR_TEXTCALLBACK;
	Item.lParam = (LPARAM)Error;
	Item.iItem = SendMessage(CompilerTask->hList,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item);
	SendMessage(CompilerTask->hList,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)FALSE);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
