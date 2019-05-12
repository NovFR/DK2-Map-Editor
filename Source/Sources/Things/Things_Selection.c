
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine de sélection des objets (portes, pièges, etc).
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
extern MAP		Map;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection d'un élément						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Things_Select(HWND hWnd, MAPTHING *Default, DWORD Flags)
{
	MAPTHINGCTX	*Context;
	UINT		 Template;
	int		 Result;

	Context = GlobalAlloc(GPTR,sizeof(MAPTHINGCTX));
	if (!Context) return(0);

	CopyMemory(&Context->thing,Default,sizeof(MAPTHING));
	Context->flags = Flags;

	switch(Context->thing.node.type)
		{
		case THING_CREATURE:
			Template = 6100;
			if (Flags&THING_FLAGFROMBAND) Template = 6150;
			break;
		case THING_DOOR:
			Template = 6101;
			break;
		case THING_TRAP:
			Template = 6102;
			break;
		case THING_OBJECT:
			Template = 6103;
			break;
		case THING_MAGICALOBJECT:
			Template = 6104;
			break;
		default:GlobalFree(Context);
			return(0);
		}

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(Template),hWnd,Things_SelectProc,(LPARAM)Context);
	if ((Result == 0)||(Result == -1))
		{
		if (Result == -1) Misc_PrintError(hWnd,szDialogErr,NULL,MB_ICONHAND);
		GlobalFree(Context);
		return(0);
		}

	CopyMemory(Default,&Context->thing,sizeof(MAPTHING));
	GlobalFree(Context);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus de sélection						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Things_SelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGCTX	*Context;
	long		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Things_SelectInit(hDlg,(MAPTHINGCTX *)lParam);
		return(TRUE);
		}

	Context = (MAPTHINGCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case TVN_SELCHANGED:
					Context->selected.hselected = ((NM_TREEVIEW *)lParam)->itemNew.hItem;
					if (!Context->selected.hselected)
						Things_SelectChangeState(hDlg,Context,FALSE);
					else
						Things_SelectChangeState(hDlg,Context,TRUE);
					return(TRUE);
				case LVN_ITEMCHANGED:
					Context->selected.iselected = List_FindSelectedItem(Context->hlist);
					if (Context->selected.iselected == -1)
						Things_SelectChangeState(hDlg,Context,FALSE);
					else
						Things_SelectChangeState(hDlg,Context,TRUE);
					return(TRUE);
				case UDN_DELTAPOS:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 127:
							Misc_SetUpDownInt(hDlg,117,(NM_UPDOWN *)lParam);
							break;
						case 129:
							Misc_SetUpDownInt(hDlg,119,(NM_UPDOWN *)lParam);
							break;
						case 132:
							Misc_SetUpDownInt(hDlg,130,(NM_UPDOWN *)lParam);
							break;
						case 410:
							Misc_SetUpDownInt(hDlg,400,(NM_UPDOWN *)lParam);
							break;
						case 411:
							Misc_SetUpDownInt(hDlg,401,(NM_UPDOWN *)lParam);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_MeasurePlayerComboBoxList(((MEASUREITEMSTRUCT *)lParam)->CtlID,(MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_DisplayPlayerComboBoxList(((MEASUREITEMSTRUCT *)lParam)->CtlID,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_BUTTON)
				{
				Misc_DrawButtonIcon((DRAWITEMSTRUCT *)lParam,Context->hicon);
				}
			break;

		case WM_PAINT:
			Misc_PrintButtonRightText(hDlg,111,szLevel);
			Misc_PrintButtonRightText(hDlg,119,szGoldHeld);
			Misc_PrintButtonRightText(hDlg,117,szHealth);
			Misc_PrintButtonRightText(hDlg,102,szOwner);
			Misc_PrintButtonRightText(hDlg,130,szGoldAmount);
			Misc_PrintButtonRightText(hDlg,131,szSpellBook);
			if (!(Context->flags&THING_FLAGFROMBAND)) Misc_PrintButtonRightText(hDlg,400,szThingPosition);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 400:
							lData = GetDlgItemInt(hDlg,400,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,400,0,FALSE);
							if (lData >= Map.Properties.Width) SetDlgItemInt(hDlg,400,Map.Properties.Width-1,FALSE);
							break;
						case 401:
							lData = GetDlgItemInt(hDlg,401,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,401,0,FALSE);
							if (lData >= Map.Properties.Height) SetDlgItemInt(hDlg,401,Map.Properties.Height-1,FALSE);
							break;
						case 117:
						case 119:
							lData = GetDlgItemInt(hDlg,LOWORD(wParam),NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,LOWORD(wParam),0,FALSE);
							if (lData > 100) SetDlgItemInt(hDlg,LOWORD(wParam),100,FALSE);
							break;
						case 130:
							lData = GetDlgItemInt(hDlg,130,NULL,FALSE);
							if (lData < 0) SetDlgItemInt(hDlg,130,0,FALSE);
							if (lData > 99999) SetDlgItemInt(hDlg,130,99999,FALSE);
							break;
						}
					break;

				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 102:
							if ((Context->thing.node.type == THING_CREATURE)&&(!(Context->flags&THING_FLAGLOCKOWNER)))
								{
								lData = SendDlgItemMessage(hDlg,102,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
								if (lData > 1) lData = FALSE;
								else lData = TRUE;
								EnableWindow(GetDlgItem(hDlg,114),lData);
								EnableWindow(GetDlgItem(hDlg,115),lData);
								EnableWindow(GetDlgItem(hDlg,118),lData);
								}
							break;
						}
					return(TRUE);

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 118:
							Things_SelectObjective(hDlg,&Context->thing.objective,&Context->thing.area,&Context->thing.ennemy);
							break;
						case 116:
							Things_SelectOptions(hDlg,&Context->thing.creaturesoptions,&Context->thing.herosoptions,(Context->flags&THING_FLAGFROMBAND)?THING_OPTIONSBANDS:THING_OPTIONSCREATURES);
							break;
						case 115:
							Things_SelectOptions(hDlg,&Context->thing.creaturesoptions,&Context->thing.herosoptions,THING_OPTIONSHEROS);
							break;
						case 141:
							if (Context->thing.node.type == THING_DOOR)
								{
								switch(Doors[Context->selected.iselected].Id)
									{
									case 0x00000006:
									case 0x00000007:
										break;
									default:
										if (IsDlgButtonChecked(hDlg,141) == BST_CHECKED)
											{
											EnableWindow(GetDlgItem(hDlg,140),FALSE);
											CheckDlgButton(hDlg,140,BST_UNCHECKED);
											}
										else
											EnableWindow(GetDlgItem(hDlg,140),TRUE);
										break;
									}
								}
							break;
						case 402:
							Things_SelectPoint(hDlg,Context);
							break;
						case IDOK:
							Things_SelectReset(hDlg,Context,TRUE);
							EndDialog(hDlg,1);
							break;
						case IDCANCEL:
							Things_SelectReset(hDlg,Context,FALSE);
							EndDialog(hDlg,0);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations & Restitutions					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_SelectInit(HWND hDlg, MAPTHINGCTX *Context)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	TREELIST	 List;
	RECT		 ClientRect;
	ELEMENT		*Elements;
	long		 X,Y;

	Window_UnderCursor(hDlg,&X,&Y);
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER);
	SetWindowLong(hDlg,DWL_USER,(LONG)Context);

	GetWindowRect(GetDlgItem(hDlg,100),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	//--- Routines communes à tous ---

	Misc_CreatePlayerComboBoxList(hDlg,102,1,TRUE);
	SendDlgItemMessage(hDlg,102,CB_SETCURSEL,(WPARAM)Context->thing.owner,(LPARAM)0);

	Context->hicon = LoadImage(hInstance,MAKEINTRESOURCE(19),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	if (!Context->hicon) goto Error;

	if (!(Context->flags&THING_FLAGFROMBAND))
		{
		Misc_CreateUpDownButton(hDlg,400,410);
		Misc_CreateUpDownButton(hDlg,401,411);
		}
	if (!(Context->flags&THING_FLAGPOSITION))
		{
		EnableWindow(GetDlgItem(hDlg,400),FALSE);
		EnableWindow(GetDlgItem(hDlg,401),FALSE);
		EnableWindow(GetDlgItem(hDlg,402),FALSE);
		EnableWindow(GetDlgItem(hDlg,410),FALSE);
		EnableWindow(GetDlgItem(hDlg,411),FALSE);
		SetDlgItemText(hDlg,400,szManualPosition);
		SetDlgItemText(hDlg,401,szManualPosition);
		}
	else
		{
		SetDlgItemInt(hDlg,400,Context->thing.x,FALSE);
		SetDlgItemInt(hDlg,401,Context->thing.y,FALSE);
		SendDlgItemMessage(hDlg,400,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		SendDlgItemMessage(hDlg,401,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
		}

//--- Type d'objet à sélectionner ---

	switch(Context->thing.node.type)
		{
		case THING_CREATURE:
			goto Setup_Creatures;
		case THING_DOOR:
			X = 32;
			Y = 32;
			Elements = Doors;
			break;
		case THING_TRAP:
			X = 32;
			Y = 32;
			Elements = Traps;
			break;
		case THING_OBJECT:
			X = 64;
			Y = 64;
			Elements = Objects;
			break;
		case THING_MAGICALOBJECT:
			X = 32;
			Y = 32;
			Elements = (Map.Properties.Version >= 17)?MagicsV17:Magics;
			break;

		default:EndDialog(hDlg,-1);
			return;
		}

	//--- Routine commune aux portes, pièges, objets et objets magiques ---

	Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_NOSORTHEADER|LVS_NOCOLUMNHEADER|LVS_SHAREIMAGELISTS,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Context->hlist) goto Error;
	if (!Misc_CreateImageList(Elements,&Context->himages,X,Y,FALSE)) goto Error;
	SendMessage(Context->hlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)Context->himages);

	ZeroMemory(&Column,sizeof(LV_COLUMN));
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-8-1-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(Context->hlist,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error2;

	ZeroMemory(&Item,sizeof(LV_ITEM));
	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE|LVIF_PARAM;
	Item.iSubItem = 0;

	for (X = 0; Elements[X].Id != 0; X++)
		{
		Item.state = (Elements[X].Id == Context->thing.id)?LVIS_SELECTED:0;
		Item.stateMask = LVIS_SELECTED;
		Item.iItem = X;
		Item.pszText = Elements[X].Name;
		Item.iImage = X;
		Item.lParam = Elements[X].Id;
		if (SendMessage(Context->hlist,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error2;
		if (Elements[X].Id == Context->thing.id) SendMessage(Context->hlist,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)TRUE);
		}

	//--- Initialisations spécifiques à chaque objet ---

	switch(Context->thing.node.type)
		{
		case THING_OBJECT:
			SendDlgItemMessage(hDlg,131,CB_ADDSTRING,(WPARAM)0,(LPARAM)szNoSpell);
			for (X = 0; Spells[X].Id != 0; X++) SendDlgItemMessage(hDlg,131,CB_ADDSTRING,(WPARAM)0,(LPARAM)Spells[X].Name);
			Misc_CreateUpDownButton(hDlg,130,132);
			switch(Context->thing.id)
				{
				case 0x00000001:
				case 0x00000002:
				case 0x00000003:
					SetDlgItemInt(hDlg,130,Context->thing.data.gold,FALSE);
					SendDlgItemMessage(hDlg,130,EM_LIMITTEXT,(WPARAM)5,(LPARAM)0);
					SendDlgItemMessage(hDlg,131,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
					break;
				case 0x00000004:
					SetDlgItemInt(hDlg,130,0,FALSE);
					for (X = 0; Spells[X].Id != 0; X++) if (Spells[X].Id == Context->thing.data.spell) break;
					if (!Spells[X].Id) X = -1;
					SendDlgItemMessage(hDlg,131,CB_SETCURSEL,(WPARAM)X+1,(LPARAM)0);
					break;
				default:SetDlgItemInt(hDlg,130,0,FALSE);
					SendDlgItemMessage(hDlg,131,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);
					break;
				}
			break;

		case THING_DOOR:
			if (Context->thing.data.status == 1)
				CheckDlgButton(hDlg,140,BST_CHECKED);
			else if (Context->thing.data.status == 2)
				{
				CheckDlgButton(hDlg,141,BST_CHECKED);
				EnableWindow(GetDlgItem(hDlg,140),FALSE);
				}
			switch(Context->thing.id)
				{
				case 0x00000006:
				case 0x00000007:
					EnableWindow(GetDlgItem(hDlg,140),FALSE);
					break;
				}
			break;

		case THING_TRAP:
			if (Context->thing.data.shots == 0x00)
				CheckDlgButton(hDlg,141,BST_CHECKED);
			break;
		}

	return;

	//--- Initialisation des créatures (routine particulière) ---

Setup_Creatures:
	Context->hlist = CreateWindowEx(WS_EX_CLIENTEDGE,szTreeViewClass,NULL,WS_BORDER|WS_VISIBLE|WS_CHILD|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS|TVS_DISABLEDRAGDROP,ClientRect.left+4,ClientRect.top+18,ClientRect.right-ClientRect.left-8,ClientRect.bottom-ClientRect.top-22,hDlg,NULL,hInstance,NULL);
	if (!Context->hlist) goto Error;
	if (!Misc_CreateImageList(CrIcons,&Context->himages,32,32,FALSE)) goto Error;
	if (!Misc_CreateImageList(BadCreatures,&Context->himages,32,32,TRUE)) goto Error;
	if (!Misc_CreateImageList(BadElites,&Context->himages,32,32,TRUE)) goto Error;
	if (!Misc_CreateImageList(Heros,&Context->himages,32,32,TRUE)) goto Error;
	SendMessage(Context->hlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)Context->himages);
	Misc_CreateLevelComboBoxList(hDlg,111,Context->thing.data.level);
	Misc_CreateUpDownButton(hDlg,117,127);
	Misc_CreateUpDownButton(hDlg,119,129);
	SetDlgItemInt(hDlg,117,Context->thing.health,FALSE);
	SetDlgItemInt(hDlg,119,Context->thing.goldheld,FALSE);
	SendDlgItemMessage(hDlg,117,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);
	SendDlgItemMessage(hDlg,119,EM_LIMITTEXT,(WPARAM)3,(LPARAM)0);

	List.hTree = Context->hlist;
	List.Flags = (Context->flags&THING_FLAGFROMBAND)?LIST_BADS|LIST_HEROES|LIST_LPARAMID:LIST_BADS|LIST_ELITES|LIST_HEROES|LIST_LPARAMID;
	List.Expand = 0;
	List.Selected = Context->thing.id;
	List.SelectedItem = &Context->selected.hselected;
	if (!Misc_CreateCreaturesTreeList(&List)) goto Error3;

	return;

//--- Erreur d'initialisation ---

Error3:	SendMessage(Context->hlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)NULL);
	goto Error;
Error2:	SendMessage(Context->hlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
Error:	Misc_DestroyImageList(&Context->himages);
	if (Context->hicon) DestroyIcon(Context->hicon);
	EndDialog(hDlg,-1);
	return;
}


// «»»» Restitutions ««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Things_SelectReset(HWND hDlg, MAPTHINGCTX *Context, BOOL Update)
{
	if (Update)
		{
		int	i;

		Context->thing.owner = SendDlgItemMessage(hDlg,102,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
		if (Context->flags&THING_FLAGPOSITION)
			{
			Context->thing.x = GetDlgItemInt(hDlg,400,NULL,FALSE);
			Context->thing.y = GetDlgItemInt(hDlg,401,NULL,FALSE);
			}

		switch(Context->thing.node.type)
			{
			case THING_CREATURE:
				Context->thing.id = Things_GetCreatureIDFromItem(Context,Context->selected.hselected);
				Context->thing.health = GetDlgItemInt(hDlg,117,NULL,FALSE);
				Context->thing.goldheld = GetDlgItemInt(hDlg,119,NULL,FALSE);
				Context->thing.data.level = (SendDlgItemMessage(hDlg,111,CB_GETCURSEL,(WPARAM)0,(LPARAM)0))+1;
				break;
			case THING_DOOR:
				Context->thing.id = Doors[Context->selected.iselected].Id;
				if (IsDlgButtonChecked(hDlg,140) == BST_CHECKED) Context->thing.data.status = 1;
				else if (IsDlgButtonChecked(hDlg,141) == BST_CHECKED) Context->thing.data.status = 2;
				else Context->thing.data.status = 0;
				break;
			case THING_TRAP:
				Context->thing.id = Traps[Context->selected.iselected].Id;
				if (IsDlgButtonChecked(hDlg,141) == BST_CHECKED) Context->thing.data.shots = 0;
				else Context->thing.data.shots = 4;
				break;
			case THING_OBJECT:
				Context->thing.id = Objects[Context->selected.iselected].Id;
				switch(Context->thing.id)
					{
					case 0x00000001:
					case 0x00000002:
					case 0x00000003:
						Context->thing.data.gold = GetDlgItemInt(hDlg,130,NULL,FALSE);
						break;
					case 0x00000004:
						i = SendDlgItemMessage(hDlg,131,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
						if (i) Context->thing.data.spell = Spells[i-1].Id;
						else Context->thing.data.spell = 0;
						break;
					default:
						Context->thing.data.gold = 0;
						break;
					}
				break;
			case THING_MAGICALOBJECT:
				Context->thing.id = Magics[Context->selected.iselected].Id;
				break;
			}
		}

	if (Context->thing.node.type != THING_CREATURE)
		SendMessage(Context->hlist,LVM_SETIMAGELIST,(WPARAM)LVSIL_SMALL,(LPARAM)NULL);
	else
		SendMessage(Context->hlist,TVM_SETIMAGELIST,(WPARAM)TVSIL_NORMAL,(LPARAM)NULL);

	Misc_DestroyImageList(&Context->himages);
	if (Context->hicon) DestroyIcon(Context->hicon);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines diverses						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélection de la position ««««««««««««««««««««««««««««««««««««««««»

void Things_SelectPoint(HWND hDlg, MAPTHINGCTX *Context)
{
	MAPRECT	 MapRect;
	char	*Text;

	switch(Context->thing.node.type)
		{
		case THING_CREATURE:
			Text = szSelectCreaturePos;
			break;
		case THING_DOOR:
			Text = szSelectDoorPos;
			break;
		case THING_TRAP:
			Text = szSelectTrapPos;
			break;
		case THING_OBJECT:
			Text = szSelectObjectPos;
			break;
		case THING_MAGICALOBJECT:
			Text = szSelectMagicalObjectPos;
			break;
		}

	MapRect.x1 = GetDlgItemInt(hDlg,400,NULL,FALSE);
	MapRect.y1 = GetDlgItemInt(hDlg,401,NULL,FALSE);
	Map_ChoosePointEx(hDlg,&MapRect,MAP_CHOICELMBPOINT,Text);
	SetDlgItemInt(hDlg,400,MapRect.x1,FALSE);
	SetDlgItemInt(hDlg,401,MapRect.y1,FALSE);
	return;
}


// «»»» Change l'état de tous les gadgets «««««««««««««««««««««««««««««««»

void Things_SelectChangeState(HWND hDlg, MAPTHINGCTX *Context, BOOL Enable)
{
	long	lData;

	EnableWindow(GetDlgItem(hDlg,IDOK),Enable);

	if (Context->thing.node.type != THING_CREATURE)
		{
		EnableWindow(GetDlgItem(hDlg,102),Enable);
		if (Context->flags&THING_FLAGPOSITION)
			{
			EnableWindow(GetDlgItem(hDlg,400),Enable);
			EnableWindow(GetDlgItem(hDlg,401),Enable);
			EnableWindow(GetDlgItem(hDlg,402),Enable);
			EnableWindow(GetDlgItem(hDlg,410),Enable);
			EnableWindow(GetDlgItem(hDlg,411),Enable);
			}
		}

	switch(Context->thing.node.type)
		{
		case THING_CREATURE:
			if (!Things_GetCreatureIDFromItem(Context,Context->selected.hselected))
				Enable = FALSE;
			EnableWindow(GetDlgItem(hDlg,102),(Context->flags&THING_FLAGLOCKOWNER)?FALSE:Enable);
			EnableWindow(GetDlgItem(hDlg,110),Enable);
			EnableWindow(GetDlgItem(hDlg,111),Enable);
			EnableWindow(GetDlgItem(hDlg,116),Enable);
			EnableWindow(GetDlgItem(hDlg,117),Enable);
			EnableWindow(GetDlgItem(hDlg,119),Enable);
			EnableWindow(GetDlgItem(hDlg,127),Enable);
			EnableWindow(GetDlgItem(hDlg,129),Enable);
			EnableWindow(GetDlgItem(hDlg,IDOK),Enable);
			if (Context->flags&THING_FLAGPOSITION)
				{
				EnableWindow(GetDlgItem(hDlg,400),Enable);
				EnableWindow(GetDlgItem(hDlg,401),Enable);
				EnableWindow(GetDlgItem(hDlg,402),Enable);
				EnableWindow(GetDlgItem(hDlg,410),Enable);
				EnableWindow(GetDlgItem(hDlg,411),Enable);
				}
			if (Context->flags&THING_FLAGFROMBAND)
				{
				EnableWindow(GetDlgItem(hDlg,118),Enable);
				break;
				}
			if (Enable == TRUE)
				{
				lData = SendDlgItemMessage(hDlg,102,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
				if (lData > 1)
					Enable = FALSE;
				}
			EnableWindow(GetDlgItem(hDlg,114),Enable);
			EnableWindow(GetDlgItem(hDlg,115),Enable);
			EnableWindow(GetDlgItem(hDlg,118),Enable);
			break;
		case THING_DOOR:
			EnableWindow(GetDlgItem(hDlg,141),Enable);
			if ((IsDlgButtonChecked(hDlg,141) == BST_CHECKED)&&(Enable))
				{
				Enable = FALSE;
				CheckDlgButton(hDlg,140,BST_UNCHECKED);
				}
			if (Enable == TRUE)
				{
				switch(Doors[Context->selected.iselected].Id)
					{
					case 0x00000006:
					case 0x00000007:
						Enable = FALSE;
						CheckDlgButton(hDlg,140,BST_UNCHECKED);
						break;
					}
				}
			EnableWindow(GetDlgItem(hDlg,140),Enable);
			break;
		case THING_TRAP:
			EnableWindow(GetDlgItem(hDlg,141),Enable);
			break;
		case THING_OBJECT:
			if (Enable == TRUE)
				{
				switch(Objects[Context->selected.iselected].Id)
					{
					case 0x00000001:
					case 0x00000002:
					case 0x00000003:
						EnableWindow(GetDlgItem(hDlg,130),Enable);
						EnableWindow(GetDlgItem(hDlg,131),FALSE);
						EnableWindow(GetDlgItem(hDlg,132),Enable);
						break;
					case 0x00000004:
						EnableWindow(GetDlgItem(hDlg,130),FALSE);
						EnableWindow(GetDlgItem(hDlg,131),TRUE);
						EnableWindow(GetDlgItem(hDlg,132),FALSE);
						break;
					default:
						EnableWindow(GetDlgItem(hDlg,130),FALSE);
						EnableWindow(GetDlgItem(hDlg,131),FALSE);
						EnableWindow(GetDlgItem(hDlg,132),FALSE);
						break;
					}
				}
			break;
		}

	Misc_PrintButtonRightText(hDlg,111,szLevel);
	Misc_PrintButtonRightText(hDlg,117,szHealth);
	Misc_PrintButtonRightText(hDlg,119,szGoldHeld);
	Misc_PrintButtonRightText(hDlg,102,szOwner);
	Misc_PrintButtonRightText(hDlg,130,szGoldAmount);
	Misc_PrintButtonRightText(hDlg,131,szSpellBook);
	if (!(Context->flags&THING_FLAGFROMBAND)) Misc_PrintButtonRightText(hDlg,400,szThingPosition);
	return;
}


// «»»» Retrouve l'identificateur de la créature ««««««««««««««««««««««««»

DWORD Things_GetCreatureIDFromItem(MAPTHINGCTX *Context, HTREEITEM hItem)
{
	TV_ITEM	Item;
	DWORD	Result = 0x00000000;

	if (hItem)
		{
		Item.mask = TVIF_PARAM;
		Item.hItem = hItem;
		if (SendMessage(Context->hlist,TVM_GETITEM,(WPARAM)0,(LPARAM)&Item))
			return(Item.lParam);
		}

	return(Result);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification des options						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_SelectOptions(HWND hDlg, DWORD *CreatureOptions, DWORD *HerosOptions, UINT Mode)
{
	MAPTHINGOPTIONSCTX	Context;
	int			ResID;
	int			Result;

	switch(Mode)
		{
		case THING_OPTIONSCREATURES:
			ResID = 6110;
			break;
		case THING_OPTIONSHEROS:
			ResID = 6120;
			break;
		case THING_OPTIONSBANDS:
			ResID = 6130;
			break;
		case THING_OPTIONSADD:
			ResID = 6111;
			break;
		}

	Context.CreatureOptions = CreatureOptions;
	Context.HerosOptions = HerosOptions;
	Context.Mode = Mode;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(ResID),hDlg,Things_SelectOptionsProc,(LPARAM)&Context);
	if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);

	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Things_SelectOptionsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGOPTIONSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		Context = (MAPTHINGOPTIONSCTX *)lParam;
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		SetWindowLong(hDlg,DWL_USER,(LONG)lParam);

		if ((Context->Mode == THING_OPTIONSCREATURES)||(Context->Mode == THING_OPTIONSBANDS)||(Context->Mode == THING_OPTIONSADD))
			{
			if (Context->Mode == THING_OPTIONSBANDS)
				{
				if      (*Context->CreatureOptions&0x02) CheckDlgButton(hDlg,200,BST_CHECKED);
				else if (*Context->CreatureOptions&0x04) CheckDlgButton(hDlg,207,BST_CHECKED);
				else					 CheckDlgButton(hDlg,208,BST_CHECKED);
				}
			if (Context->Mode == THING_OPTIONSADD)
				{
				if	(*Context->CreatureOptions&0x04) CheckDlgButton(hDlg,207,BST_CHECKED);
				}
			if (*Context->CreatureOptions&0x01) CheckDlgButton(hDlg,201,BST_CHECKED);
			if (*Context->CreatureOptions&0x08) CheckDlgButton(hDlg,202,BST_CHECKED);
			if (*Context->CreatureOptions&0x10) CheckDlgButton(hDlg,203,BST_CHECKED);
			if (*Context->CreatureOptions&0x20) CheckDlgButton(hDlg,204,BST_CHECKED);
			if ((Context->Mode == THING_OPTIONSCREATURES)||(Context->Mode == THING_OPTIONSADD))
				{
				if (*Context->CreatureOptions&0x40) CheckDlgButton(hDlg,205,BST_CHECKED);
				if (*Context->CreatureOptions&0x80) CheckDlgButton(hDlg,206,BST_CHECKED);
				}
			}
		if ((Context->Mode == THING_OPTIONSHEROS)||(Context->Mode == THING_OPTIONSBANDS))
			{
			if (*Context->HerosOptions&0x01) CheckDlgButton(hDlg,300,BST_CHECKED);
			if (*Context->HerosOptions&0x04) CheckDlgButton(hDlg,301,BST_CHECKED);
			if (*Context->HerosOptions&0x02) CheckDlgButton(hDlg,302,BST_CHECKED);
			if (*Context->HerosOptions&0x08) CheckDlgButton(hDlg,303,BST_CHECKED);
			}

		return(TRUE);
		}

	Context = (MAPTHINGOPTIONSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
						case IDCANCEL:
							if ((Context->Mode == THING_OPTIONSCREATURES)||(Context->Mode == THING_OPTIONSBANDS)||(Context->Mode == THING_OPTIONSADD))
								{
								*Context->CreatureOptions = 0x00;
								if (Context->Mode == THING_OPTIONSBANDS)
									{
									if      (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) *Context->CreatureOptions |= 0x02;
									else if (IsDlgButtonChecked(hDlg,207) == BST_CHECKED) *Context->CreatureOptions |= 0x04;
									}
								if (Context->Mode == THING_OPTIONSADD)
									{
									if	(IsDlgButtonChecked(hDlg,207) == BST_CHECKED) *Context->CreatureOptions |= 0x04;
									}
								if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) *Context->CreatureOptions |= 0x01;
								if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) *Context->CreatureOptions |= 0x08;
								if (IsDlgButtonChecked(hDlg,203) == BST_CHECKED) *Context->CreatureOptions |= 0x10;
								if (IsDlgButtonChecked(hDlg,204) == BST_CHECKED) *Context->CreatureOptions |= 0x20;
								if ((Context->Mode == THING_OPTIONSCREATURES)||(Context->Mode == THING_OPTIONSADD))
									{
									if (IsDlgButtonChecked(hDlg,205) == BST_CHECKED) *Context->CreatureOptions |= 0x40;
									if (IsDlgButtonChecked(hDlg,206) == BST_CHECKED) *Context->CreatureOptions |= 0x80;
									}
								}
							if ((Context->Mode == THING_OPTIONSHEROS)||(Context->Mode == THING_OPTIONSBANDS))
								{
								*Context->HerosOptions = 0x00;
								if (IsDlgButtonChecked(hDlg,300) == BST_CHECKED) *Context->HerosOptions |= 0x01;
								if (IsDlgButtonChecked(hDlg,301) == BST_CHECKED) *Context->HerosOptions |= 0x04;
								if (IsDlgButtonChecked(hDlg,302) == BST_CHECKED) *Context->HerosOptions |= 0x02;
								if (IsDlgButtonChecked(hDlg,303) == BST_CHECKED) *Context->HerosOptions |= 0x08;
								}
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification de l'objectif					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_SelectObjective(HWND hDlg, UINT *Objective, UINT *Area, UINT *Ennemy)
{
	MAPTHINGOBJECTIVECTX	Context;
	int			Result;

	Context.Objective = Objective;
	Context.Area = Area;
	Context.Ennemy = Ennemy;

	Result = DialogBoxParam(hInstance,MAKEINTRESOURCE(6140),hDlg,Things_SelectObjectiveProc,(LPARAM)&Context);
	if (Result == -1) Misc_PrintError(hDlg,szDialogErr,NULL,MB_ICONHAND);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL CALLBACK Things_SelectObjectiveProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MAPTHINGOBJECTIVECTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		LONG	X,Y;

		Context = (MAPTHINGOBJECTIVECTX *)lParam;
		Window_UnderCursor(hDlg,&X,&Y);
		SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
		SetWindowLong(hDlg,DWL_USER,(LONG)Context);
		Misc_CreatePlayerComboBoxList(hDlg,200,0,FALSE);
		Misc_CreateAreaComboBoxList(hDlg,201,FALSE);
		Misc_SetArea(hDlg,201,*Context->Area);
		SendDlgItemMessage(hDlg,200,CB_SETCURSEL,(WPARAM)*Context->Ennemy,(LPARAM)0);

		X = 101;
		switch(*Context->Objective)
			{
			case 0x1B:
				X++;
			case 0x17:
				X++;
			case 0x16:
				X++;
			case 0x13:
				X++;
			case 0x12:
				X++;
			case 0x11:
				X++;
			case 0x0E:
				X++;
			case 0x0D:
				X++;
			case 0x0C:
				X++;
			case 0x0B:
				break;
			}
		CheckRadioButton(hDlg,101,110,X);

		if (*Context->Objective == 0x17) EnableWindow(GetDlgItem(hDlg,201),TRUE);
		return(TRUE);
		}

	Context = (MAPTHINGOBJECTIVECTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_MeasurePlayerComboBoxList(((MEASUREITEMSTRUCT *)lParam)->CtlID,(MEASUREITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_COMBOBOX)
				{
				Misc_DisplayPlayerComboBoxList(((MEASUREITEMSTRUCT *)lParam)->CtlID,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_PAINT:
			Misc_PrintButtonRightText(hDlg,200,szEnnemy);
			Misc_PrintButtonRightText(hDlg,201,szTargetArea);
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 101:
						case 102:
						case 103:
						case 104:
						case 105:
						case 106:
						case 107:
						case 108:
						case 110:
							EnableWindow(GetDlgItem(hDlg,201),FALSE);
							break;
						case 109:
							EnableWindow(GetDlgItem(hDlg,201),TRUE);
							break;

						case IDOK:
						case IDCANCEL:
							     if (IsDlgButtonChecked(hDlg,101) == BST_CHECKED) *Context->Objective = 0x0B;
							else if (IsDlgButtonChecked(hDlg,102) == BST_CHECKED) *Context->Objective = 0x0C;
							else if (IsDlgButtonChecked(hDlg,103) == BST_CHECKED) *Context->Objective = 0x0D;
							else if (IsDlgButtonChecked(hDlg,104) == BST_CHECKED) *Context->Objective = 0x0E;
							else if (IsDlgButtonChecked(hDlg,105) == BST_CHECKED) *Context->Objective = 0x11;
							else if (IsDlgButtonChecked(hDlg,106) == BST_CHECKED) *Context->Objective = 0x12;
							else if (IsDlgButtonChecked(hDlg,107) == BST_CHECKED) *Context->Objective = 0x13;
							else if (IsDlgButtonChecked(hDlg,108) == BST_CHECKED) *Context->Objective = 0x16;
							else if (IsDlgButtonChecked(hDlg,109) == BST_CHECKED) *Context->Objective = 0x17;
							else if (IsDlgButtonChecked(hDlg,110) == BST_CHECKED) *Context->Objective = 0x1B;
							if (*Context->Objective == 0x17) *Context->Area = Misc_GetArea(hDlg,201,FALSE);
							else *Context->Area = 0;
							*Context->Ennemy = SendDlgItemMessage(hDlg,200,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
							EndDialog(hDlg,1);
							break;
						}
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
