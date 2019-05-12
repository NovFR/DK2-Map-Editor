
//<<>-<>>---------------------------------------------------------------------()
/*
	Ajout, modification des points d'action
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
extern NODE		MapActionPoints;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapAreas;
extern NODE		MapHerosBands;
extern LONG		MapChanges;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Ajout ou Modification d'un point d'action			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Triggers_PointNew(HWND hDlg, TRIGGERSCTX *Context)
{
	PROPSHEETHEADER	*psh;
	PROPSHEETPAGE	*psp;
	int		 Result;

	//--- Si modification, copie les données précédentes ---

	if (Context->Modify)
		{
		LV_ITEM	Item;

		Item.mask = LVIF_PARAM;
		Item.iItem = List_FindSelectedItem(Context->Points);
		Item.iSubItem = 0;
		if (Item.iItem == -1) return;
		if (!SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return;
		if (!Item.lParam) return;
		CopyMemory(&Context->PointCtx.TempLink,(void *)Item.lParam,sizeof(TRIGGERLINK));
		Context->PointCtx.SrceLink = (TRIGGERLINK *)Item.lParam;
		}
	else
		{
		Context->PointCtx.SrceLink = NULL;
		Context->PointCtx.TempLink.node.next = NULL;
		Context->PointCtx.TempLink.node.prev = NULL;
		Context->PointCtx.TempLink.trigger = 0;
		if (!Context->PointCtx.TempLink.node.type)
			{
			Context->PointCtx.TempLink.node.type = SCOPE_PLAYER;
			Context->PointCtx.TempLink.player = 3;
			}
		}

	//--- Initialise les pages ---

	psh = GlobalAlloc(GPTR,sizeof(PROPSHEETHEADER));
	if (!psh) goto Error_0;
	psp = GlobalAlloc(GPTR,sizeof(PROPSHEETPAGE)*9);
	if (!psp) goto Error_1;

	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].hInstance = hInstance;
	psp[0].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(7010);
	psp[0].pfnDlgProc = Triggers_PointDlgList;
	psp[0].lParam = (LPARAM)Context;
	for (Result = 1; Result != 8+1; Result++)
		{
		psp[Result].dwSize = sizeof(PROPSHEETPAGE);
		psp[Result].hInstance = hInstance;
		psp[Result].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(7010+Result);
		psp[Result].pfnDlgProc = Triggers_PointDlgParams;
		psp[Result].lParam = (LPARAM)Context;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_WIZARD;
	psh->hwndParent = hDlg;
	psh->hInstance = hInstance;
	psh->DUMMYUNIONNAME.pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = NULL;
	psh->nPages = 9;
	psh->DUMMYUNIONNAME3.ppsp = psp;

	if (!Misc_CreateImageList(TrSpecial,&Context->Images,32,32,FALSE)) goto Error_2;
	if (!Misc_CreateImageList(BadCreatures,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(BadElites,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Heros,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Doors,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Traps,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Spells,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(Terrain,&Context->Images,32,32,TRUE)) goto Error_2;
	if (!Misc_CreateImageList(TrPlayers,&Context->Images,32,32,TRUE)) goto Error_2;

	for (Result = 0; Result != 4+1; Result++)
		{
		Context->PointCtx.Initialized[Result] = FALSE;
		Context->PointCtx.Selected[Result] = -1;
		}

	Result = PropertySheet(psh);
	if (Result == -1) goto Error_2;

	Misc_DestroyImageList(&Context->Images);
	GlobalFree(psp);
	GlobalFree(psh);
	return;

//--- Erreurs survenues lors de l'initialisation ---

Error_2:Misc_DestroyImageList(&Context->Images);
	GlobalFree(psp);
Error_1:GlobalFree(psh);
Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection du type de point d'action				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_PointDlgList(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;
	LONG		 lData;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		if (Context->PointCtx.TempLink.node.type == SCOPE_LEVEL) lData = 0;
		if (Context->PointCtx.TempLink.node.type == SCOPE_PLAYER) lData = 1;
		if (Context->PointCtx.TempLink.node.type == SCOPE_CREATURE) lData = 2;
		if (Context->PointCtx.TempLink.node.type == SCOPE_DOOR) lData = 3;
		if (Context->PointCtx.TempLink.node.type == SCOPE_OBJECT) lData = 4;
		if (Context->PointCtx.TempLink.node.type == SCOPE_MAGIC) lData = 5;
		if (Context->PointCtx.TempLink.node.type == SCOPE_AREA) lData = 6;
		if (Context->PointCtx.TempLink.node.type == SCOPE_BAND) lData = 7;
		if (Context->PointCtx.TempLink.node.type == SCOPE_CREATUREINBAND) lData = 8;
		CheckRadioButton(hDlg,101,109,101+lData);
		if (Context->Modify)
			{
			for (lData = 101; lData != 109+1; lData++)
				EnableWindow(GetDlgItem(hDlg,lData),FALSE);
			}
		SetWindowLong(hDlg,DWL_USER,(LONG)Context);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
			return(TRUE);

		case WM_COMMAND:
			switch(LOWORD(wParam))
				{
				case 101:
					Context->PointCtx.TempLink.node.type = SCOPE_LEVEL;
					Triggers_GetFirstTrigger(0,SCOPE_LEVEL,(BOOL *)&lData);
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)lData?PSWIZB_DISABLEDFINISH:PSWIZB_FINISH);
					break;
				case 102:
					Context->PointCtx.TempLink.node.type = SCOPE_PLAYER;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 103:
					Context->PointCtx.TempLink.node.type = SCOPE_CREATURE;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 104:
					Context->PointCtx.TempLink.node.type = SCOPE_DOOR;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 105:
					Context->PointCtx.TempLink.node.type = SCOPE_OBJECT;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 106:
					Context->PointCtx.TempLink.node.type = SCOPE_MAGIC;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 107:
					Context->PointCtx.TempLink.node.type = SCOPE_AREA;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 108:
					Context->PointCtx.TempLink.node.type = SCOPE_BAND;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				case 109:
					Context->PointCtx.TempLink.node.type = SCOPE_CREATUREINBAND;
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					break;
				}
			return(TRUE);

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
                                case PSN_SETACTIVE:
					if (Context->PointCtx.TempLink.node.type == SCOPE_LEVEL)
						{
						Triggers_GetFirstTrigger(0,SCOPE_LEVEL,(BOOL *)&lData);
						SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)lData?PSWIZB_DISABLEDFINISH:PSWIZB_FINISH);
						}
					else
						SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_NEXT);
					return(TRUE);

				case PSN_WIZFINISH:
					if (Context->PointCtx.TempLink.node.type != SCOPE_LEVEL) break;
					Triggers_PointDlgParamsStore(TRUE,Context);
					SetWindowLong(hDlg,DWL_MSGRESULT,0);
					return(TRUE);

				case PSN_WIZNEXT:
					switch(Context->PointCtx.TempLink.node.type)
						{
						case SCOPE_PLAYER:
							lData = 7011;
							Triggers_PointDlgCheckKeeper(hDlg,&lData,Context);
							break;
						case SCOPE_CREATURE:
							lData = 7012;
							Triggers_PointDlgCheckCreature(hDlg,&lData,Context);
							break;
						case SCOPE_DOOR:
							lData = 7013;
							Triggers_PointDlgCheckDoor(hDlg,&lData,Context);
							break;
						case SCOPE_OBJECT:
							lData = 7014;
							Triggers_PointDlgCheckObject(hDlg,&lData,Context);
							break;
						case SCOPE_MAGIC:
							lData = 7015;
							Triggers_PointDlgCheckMagicalObject(hDlg,&lData,Context);
							break;
						case SCOPE_AREA:
							lData = 7016;
							Triggers_PointDlgCheckArea(hDlg,&lData,Context);
							break;
						case SCOPE_BAND:
							lData = 7017;
							Triggers_PointDlgCheckBand(hDlg,&lData,Context);
							break;
						case SCOPE_CREATUREINBAND:
							lData = 7018;
							Triggers_PointDlgCheckCreatureInBand(hDlg,&lData,Context);
							break;
						}
					SetWindowLong(hDlg,DWL_MSGRESULT,lData);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Test s'il reste un gardien libre ««««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckKeeper(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	int		 Used;

	for (Used = 0, Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_PLAYER) continue;
		Used++;
		}

	if (Used == MAP_MAXPLAYERS)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_PLAYER))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreePlayer,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste une créature libre ««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckCreature(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapCreatures.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoCreature,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapCreatures), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_CREATURE) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_CREATURE))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeCreature,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste une porte libre «««««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckDoor(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapDoors.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoDoor,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapDoors), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_DOOR) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_DOOR))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeDoor,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste un objet libre ««««««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckObject(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapObjects.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoObject,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapObjects), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_OBJECT) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_OBJECT))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeObject,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste un objet magique libre ««««««««««««««««««««««««««»

void Triggers_PointDlgCheckMagicalObject(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapMagicalObjects.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoMagicalObject,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapMagicalObjects), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_MAGIC) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_MAGIC))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeMagicalObject,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste une zone libre ««««««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckArea(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapAreas.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoArea,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapAreas), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_AREA) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_AREA))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeArea,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste une bande de héros ««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckBand(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	TRIGGERLINK	*Link;
	long		 Free;

	if (!MapHerosBands.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoBand,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Free = List_EntryCount(&MapHerosBands), Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
		{
		if (Link->node.type != SCOPE_BAND) continue;
		--Free;
		}

	if (!Free)
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_BAND))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeBand,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// «»»» Test s'il reste une bande de héros ««««««««««««««««««««««««««««««»

void Triggers_PointDlgCheckCreatureInBand(HWND hDlg, LONG *lData, TRIGGERSCTX *Context)
{
	MTHBAND		*Band;
	long		 Total;
	long		 Used;
	int		 i;

	if (!MapHerosBands.next)
		{
		if (lData) *lData = -1;
		Misc_MessageBoxExt(hDlg,szTriggersNoBand,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
		return;
		}

	for (Total = 0, Used = 0, Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
		{
		for (i = 0; i != THING_MAXHEROSINBAND; i++)
			{
			if (!Band->heros[i].isvalid) break;
			Total++;
			if (Band->heros[i].uniqueid) Used++;
			}
		}

	if (!(Total-Used))
		{
		if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_CREATUREINBAND))
			{
			if (lData) *lData = -1;
			Misc_MessageBoxExt(hDlg,szTriggersNoFreeCreature,NULL,szMessageBoxExtOk,MB_ICONEXCLAMATION);
			return;
			}
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Paramètres du point d'action					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#pragma argsused
BOOL CALLBACK Triggers_PointDlgParams(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	TRIGGERSCTX	*Context;

	if (uMsgId == WM_INITDIALOG)
		{
		Context = (TRIGGERSCTX *)(((PROPSHEETPAGE *)lParam)->lParam);
		Triggers_PointDlgParamsInit(hDlg,Context);
		return(TRUE);
		}

	Context = (TRIGGERSCTX *)GetWindowLong(hDlg,DWL_USER);
	if (!Context) return(FALSE);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			Misc_MeasureListViewItem((MEASUREITEMSTRUCT *)lParam,32);
			return(TRUE);

		case WM_DRAWITEM:
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_LISTVIEW)
				{
				Triggers_DisplayDrawList((DRAWITEMSTRUCT *)lParam,Context);
				return(TRUE);
				}
			if (((DRAWITEMSTRUCT *)lParam)->CtlType == ODT_STATIC)
				{
				Misc_DrawGroup((DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch( ((NMHDR *)lParam)->code )
				{
				case LVN_ITEMCHANGED:
					if (((NM_LISTVIEW *)lParam)->uChanged&LVIF_STATE)
						{
						if ((((NM_LISTVIEW *)lParam)->uNewState&LVIS_SELECTED)&&(Triggers_PointDlgParamsStore(FALSE,Context)))
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_FINISH);
						else
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
						}
					return(TRUE);
                                case PSN_SETACTIVE:
					Triggers_PointDlgParamsCheck(hDlg,Context);
					return(TRUE);

				case PSN_WIZBACK:
					SetWindowLong(hDlg,DWL_MSGRESULT,7010);
					return(TRUE);

				case PSN_WIZFINISH:
					Triggers_PointDlgParamsStore(TRUE,Context);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisations des données fixes «««««««««««««««««««««««««««««««»

void Triggers_PointDlgParamsInit(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_COLUMN	 Column;
	LV_ITEM		 Item;
	RECT		 ClientRect;
	HWND		 hWnd;
	BOOL		 Used;
	MAPTHING	*Thing;
	MAPRECTEX	*Area;
	MTHBAND		*Band;
	char		*Text;
	int		 i;

	SetWindowLong(hDlg,DWL_USER,(LONG)Context);
	GetWindowRect(GetDlgItem(hDlg,101),&ClientRect);
	MapWindowPoints(NULL,hDlg,(POINT *)&ClientRect,2);

	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,szListViewClass,NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_NOCOLUMNHEADER|LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED,ClientRect.left+6,ClientRect.top+20,ClientRect.right-ClientRect.left-12,ClientRect.bottom-ClientRect.top-26,hDlg,NULL,hInstance,NULL);
	if (!hWnd) goto Error_0;
	Column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_WIDTH;
	Column.fmt = LVCFMT_LEFT;
	Column.cx = ClientRect.right-ClientRect.left-12-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	Column.iSubItem = 0;
	if (SendMessage(hWnd,LVM_INSERTCOLUMN,(WPARAM)0,(LPARAM)&Column) == -1) goto Error_0;

	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text) goto Error_0;

	Item.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.pszText = Text;

	switch(Context->PointCtx.TempLink.node.type)
		{
		case SCOPE_PLAYER:
			Context->PointCtx.KeepersList = hWnd;
			for (i = 0; i != MAP_MAXPLAYERS; i++, Item.iItem++)
				{
				strcpy(Text,Players[i].Name);
				Item.iImage = Triggers_DisplayGetImageId(i+1,TRIGGERIMAGE_PLAYER);
				Item.lParam = i+1;
				Triggers_GetFirstTrigger(i+1,SCOPE_PLAYER,&Used);
				if (Used)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_PLAYER)&&(Context->PointCtx.SrceLink->player != i+1))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_PLAYER))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[0] = TRUE;
			break;
		case SCOPE_CREATURE:
			Context->PointCtx.CreaturesList = hWnd;
			for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next, Item.iItem++)
				{
				wsprintf(Text,szTriggersCreatureList,Things_GetCreatureNameFromID(Thing->id),Thing->data.level,Players[Thing->owner-1].Name,Thing->x,Thing->y);
				Item.iImage = Triggers_DisplayGetImageId(Thing->id,TRIGGERIMAGE_CREATURE);
				Item.lParam = (LPARAM)Thing;
				if (Thing->uniqueid)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_CREATURE)&&(Context->PointCtx.SrceLink->creature != Thing->uniqueid))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_CREATURE))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[1] = TRUE;
			break;
		case SCOPE_DOOR:
			Context->PointCtx.DoorsList = hWnd;
			for (Thing = (MAPTHING *)MapDoors.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next, Item.iItem++)
				{
				wsprintf(Text,szTriggersDoorList,Things_GetThingNameFromID(Thing->id,Doors),Thing->x,Thing->y);
				Item.iImage = Triggers_DisplayGetImageId(Thing->id,TRIGGERIMAGE_DOOR);
				Item.lParam = (LPARAM)Thing;
				if (Thing->uniqueid)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_DOOR)&&(Context->PointCtx.SrceLink->door != Thing->uniqueid))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_DOOR))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[2] = TRUE;
			break;
		case SCOPE_OBJECT:
			Context->PointCtx.ObjectsList = hWnd;
			for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next, Item.iItem++)
				{
				wsprintf(Text,szTriggersObjectList,Things_GetThingNameFromID(Thing->id,Objects),Thing->x,Thing->y);
				Item.iImage = Triggers_DisplayGetImageId(Thing->id,TRIGGERIMAGE_OBJECT);
				Item.lParam = (LPARAM)Thing;
				if (Thing->uniqueid)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_OBJECT)&&(Context->PointCtx.SrceLink->object != Thing->uniqueid))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_OBJECT))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[3] = TRUE;
			break;
		case SCOPE_MAGIC:
			Context->PointCtx.MagicalObjectsList = hWnd;
			for (Thing = (MAPTHING *)MapMagicalObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next, Item.iItem++)
				{
				wsprintf(Text,szTriggersObjectList,Things_GetThingNameFromID(Thing->id,Magics),Thing->x,Thing->y);
				Item.iImage = Triggers_DisplayGetImageId(Thing->id,TRIGGERIMAGE_MAGIC);
				Item.lParam = (LPARAM)Thing;
				if (Thing->uniqueid)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_MAGIC)&&(Context->PointCtx.SrceLink->object != Thing->uniqueid))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_MAGIC))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[4] = TRUE;
			break;
		case SCOPE_AREA:
			Context->PointCtx.AreasList = hWnd;
			Item.iImage = 2;
			for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next, Item.iItem++)
				{
				RectEx_StrCpy(Text+2048,Area->name);
				RectEx_StrCpy(Text+2048+MAP_RECTMAXNAME,Area->info);
				if (Area->info[0]) wsprintf(Text,szTriggersAreaInfo,Area->id,Text+2048,Text+2048+MAP_RECTMAXNAME);
				else wsprintf(Text,szTriggersArea,Area->id,Text+2048);
				Item.lParam = (LPARAM)Area;
				Triggers_GetFirstTrigger(Area->id,SCOPE_AREA,&Used);
				if (Used)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_AREA)&&(Context->PointCtx.SrceLink->area != Area->id))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_AREA))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				}
			Context->PointCtx.Initialized[5] = TRUE;
			break;
		case SCOPE_BAND:
			Context->PointCtx.BandsList = hWnd;
			for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next, Item.iItem++)
				{
				if (Band->name[0]) wsprintf(Text,szTriggersBandInfo,Band->id,Band->name);
				else wsprintf(Text,szTriggersBand,Band->id);
				Item.iImage = 3;
				Item.lParam = (LPARAM)Band;
				if (Band->uniqueid)
					{
					if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_BAND)&&(Context->PointCtx.SrceLink->band != Band->uniqueid))
						Item.lParam = 0;
					else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_BAND))
						Item.lParam = 0;
					else if (!Context->PointCtx.SrceLink)
						Item.lParam = 0;
					}
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				Item.iItem++;
				for (i = 0; i != THING_MAXHEROSINBAND; i++)
					{
					if (!Band->heros[i].isvalid) break;
					if (Band->heros[i].options2&0x02)
						{
						wsprintf(Text,szTriggersCreatureInBandLeader,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
						switch(Band->heros[i].objective)
							{
							case 0x0B:
								strcat(Text,szHerosObjectives0B);
								break;
							case 0x0C:
								strcat(Text,szHerosObjectives0C);
								break;
							case 0x0D:
								strcat(Text,szHerosObjectives0D);
								break;
							case 0x0E:
								strcat(Text,szHerosObjectives0E);
								break;
							case 0x11:
								strcat(Text,szHerosObjectives11);
								break;
							case 0x12:
								strcat(Text,szHerosObjectives12);
								break;
							case 0x13:
								strcat(Text,szHerosObjectives13);
								break;
							case 0x16:
								strcat(Text,szHerosObjectives16);
								break;
							case 0x17:
								wsprintf(Text+2048,szHerosFmtObjectives17,Band->heros[i].area);
								strcat(Text,Text+2048);
								break;
							case 0x1B:
								strcat(Text,szHerosObjectives1B);
								break;
							default:
								strcat(Text,szUnknown);
								break;
							}
						}
					else
						wsprintf(Text,szTriggersCreatureInBand,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
					Item.iImage = Triggers_DisplayGetImageId(Band->heros[i].id,TRIGGERIMAGE_CREATURE);
					Item.lParam = 0;
					if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
					Item.iItem++;
					}
				}
			Context->PointCtx.Initialized[6] = TRUE;
			break;
		case SCOPE_CREATUREINBAND:
			Context->PointCtx.CreaturesInBandList = hWnd;
			for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
				{
				if (Band->name[0]) wsprintf(Text,szTriggersBandInfo,Band->id,Band->name);
				else wsprintf(Text,szTriggersBand,Band->id);
				Item.iImage = 3;
				Item.lParam = 0;
				if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
				Item.iItem++;
				for (i = 0; i != THING_MAXHEROSINBAND; i++)
					{
					if (!Band->heros[i].isvalid) break;
					if (Band->heros[i].options2&0x02)
						{
						wsprintf(Text,szTriggersCreatureInBandLeader,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
						switch(Band->heros[i].objective)
							{
							case 0x0B:
								strcat(Text,szHerosObjectives0B);
								break;
							case 0x0C:
								strcat(Text,szHerosObjectives0C);
								break;
							case 0x0D:
								strcat(Text,szHerosObjectives0D);
								break;
							case 0x0E:
								strcat(Text,szHerosObjectives0E);
								break;
							case 0x11:
								strcat(Text,szHerosObjectives11);
								break;
							case 0x12:
								strcat(Text,szHerosObjectives12);
								break;
							case 0x13:
								strcat(Text,szHerosObjectives13);
								break;
							case 0x16:
								strcat(Text,szHerosObjectives16);
								break;
							case 0x17:
								wsprintf(Text+2048,szHerosFmtObjectives17,Band->heros[i].area);
								strcat(Text,Text+2048);
								break;
							case 0x1B:
								strcat(Text,szHerosObjectives1B);
								break;
							default:
								strcat(Text,szUnknown);
								break;
							}
						}
					else
						wsprintf(Text,szTriggersCreatureInBand,Things_GetCreatureNameFromID(Band->heros[i].id),Band->heros[i].level);
					Item.iImage = Triggers_DisplayGetImageId(Band->heros[i].id,TRIGGERIMAGE_CREATURE);
					Item.lParam = (LPARAM)&Band->heros[i];
					if (Band->heros[i].uniqueid)
						{
						if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type == SCOPE_CREATUREINBAND)&&(Context->PointCtx.SrceLink->creature != Band->heros[i].uniqueid))
							Item.lParam = 0;
						else if ((Context->PointCtx.SrceLink)&&(Context->PointCtx.SrceLink->node.type != SCOPE_CREATUREINBAND))
							Item.lParam = 0;
						else if (!Context->PointCtx.SrceLink)
							Item.lParam = 0;
						}
					if (SendMessage(hWnd,LVM_INSERTITEM,(WPARAM)0,(LPARAM)&Item) == -1) goto Error_1;
					Item.iItem++;
					}
				}
			Context->PointCtx.Initialized[7] = TRUE;
			break;
		}

	GlobalFree(Text);
	return;

//--- Erreurs d'initialisation ---

Error_1:GlobalFree(Text);
Error_0:return;
}


// «»»» Test si les paramètres sont correctement initialisés ««««««««««««»

void Triggers_PointDlgParamsCheck(HWND hDlg, TRIGGERSCTX *Context)
{
	LV_ITEM		 Item;
	HWND		 hWnd;
	MAPTHING	*Thing;
	MAPRECTEX	*Area;
	MTHBAND		*Band;
	int		 i;

	Item.iItem = 0;
	Item.iSubItem = 0;
	Item.state = LVIS_SELECTED;
	Item.stateMask = LVIS_SELECTED;

	switch(Context->PointCtx.TempLink.node.type)
		{
		case SCOPE_PLAYER:
			hWnd = Context->PointCtx.KeepersList;
			if (!Context->PointCtx.Initialized[0]) goto Error_0;
			if (Context->PointCtx.Selected[0] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[0];
				break;
				}
			if ((!Context->PointCtx.SrceLink)||(Context->PointCtx.SrceLink->node.type != SCOPE_PLAYER)) Context->PointCtx.TempLink.player = 3;
			Item.iItem = Context->PointCtx.TempLink.player-1;
			break;
		case SCOPE_CREATURE:
			hWnd = Context->PointCtx.CreaturesList;
			if (!Context->PointCtx.Initialized[1]) goto Error_0;
			if (Context->PointCtx.Selected[1] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[1];
				break;
				}
			for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				{
				if (Thing->uniqueid == Context->PointCtx.TempLink.creature) break;
				Item.iItem++;
				}
			if (!Thing) Item.iItem = 0;
			break;
		case SCOPE_DOOR:
			hWnd = Context->PointCtx.DoorsList;
			if (!Context->PointCtx.Initialized[2]) goto Error_0;
			if (Context->PointCtx.Selected[2] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[2];
				break;
				}
			for (Thing = (MAPTHING *)MapDoors.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				{
				if (Thing->uniqueid == Context->PointCtx.TempLink.door) break;
				Item.iItem++;
				}
			if (!Thing) Item.iItem = 0;
			break;
		case SCOPE_OBJECT:
			hWnd = Context->PointCtx.ObjectsList;
			if (!Context->PointCtx.Initialized[3]) goto Error_0;
			if (Context->PointCtx.Selected[3] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[3];
				break;
				}
			for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				{
				if (Thing->uniqueid == Context->PointCtx.TempLink.object) break;
				Item.iItem++;
				}
			if (!Thing) Item.iItem = 0;
			break;
		case SCOPE_MAGIC:
			hWnd = Context->PointCtx.MagicalObjectsList;
			if (!Context->PointCtx.Initialized[4]) goto Error_0;
			if (Context->PointCtx.Selected[4] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[4];
				break;
				}
			for (Thing = (MAPTHING *)MapMagicalObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
				{
				if (Thing->uniqueid == Context->PointCtx.TempLink.object) break;
				Item.iItem++;
				}
			if (!Thing) Item.iItem = 0;
			break;
		case SCOPE_AREA:
			hWnd = Context->PointCtx.AreasList;
			if (!Context->PointCtx.Initialized[5]) goto Error_0;
			if (Context->PointCtx.Selected[5] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[5];
				break;
				}
			for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
				{
				if (Area->id == Context->PointCtx.TempLink.area) break;
				Item.iItem++;
				}
			if (!Area) Item.iItem = 0;
			break;
		case SCOPE_BAND:
			hWnd = Context->PointCtx.BandsList;
			if (!Context->PointCtx.Initialized[6]) goto Error_0;
			if (Context->PointCtx.Selected[6] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[6];
				break;
				}
			for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
				{
				if (Band->uniqueid == Context->PointCtx.TempLink.band) break;
				Item.iItem++;
				for (i = 0; i != THING_MAXHEROSINBAND; i++)
					{
					if (!Band->heros[i].isvalid) break;
					Item.iItem++;
					}
				}
			if (!Band) Item.iItem = 0;
			break;
		case SCOPE_CREATUREINBAND:
			hWnd = Context->PointCtx.CreaturesInBandList;
			if (!Context->PointCtx.Initialized[7]) goto Error_0;
			if (Context->PointCtx.Selected[7] != -1)
				{
				Item.iItem = Context->PointCtx.Selected[7];
				break;
				}
			for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
				{
				Item.iItem++;
				for (i = 0; i != THING_MAXHEROSINBAND; i++)
					{
					if (!Band->heros[i].isvalid) break;
					if (Band->heros[i].uniqueid == Context->PointCtx.TempLink.creature) goto Done;
					Item.iItem++;
					}
				}
		Done:	if (!Band) Item.iItem = 0;
			break;
		}

	SendMessage(hWnd,LVM_SETITEMSTATE,(WPARAM)Item.iItem,(LPARAM)&Item);
	SendMessage(hWnd,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)0);
	if (!Triggers_PointDlgParamsStore(FALSE,Context))
		PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_DISABLEDFINISH);
	else
		PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,(WPARAM)0,(LPARAM)PSWIZB_BACK|PSWIZB_FINISH);
	return;

//--- Erreur d'initialisation ---

Error_0:Misc_MessageBoxExt(hDlg,szDialogErr,NULL,szMessageBoxExtOk,MB_ICONHAND);
	SetWindowLong(hDlg,DWL_MSGRESULT,7010);
	return;
}


// «»»» Stock les nouveaux paramètres «««««««««««««««««««««««««««««««««««»

int Triggers_PointDlgParamsStore(BOOL StoreForReal, TRIGGERSCTX *Context)
{
	LV_ITEM		 Item;
	HWND		 hWnd;
	TRIGGERLINK	*NewLink;
	MTHBAND		*Band;
	int		*Selected;
	void		*Old;
	char		*Text;
	int		 i;

	switch(Context->PointCtx.TempLink.node.type)
		{
		case SCOPE_LEVEL:
			goto New;
		case SCOPE_PLAYER:
			hWnd = Context->PointCtx.KeepersList;
			Selected = &Context->PointCtx.Selected[0];
			break;
		case SCOPE_CREATURE:
			hWnd = Context->PointCtx.CreaturesList;
			Selected = &Context->PointCtx.Selected[1];
			break;
		case SCOPE_DOOR:
			hWnd = Context->PointCtx.DoorsList;
			Selected = &Context->PointCtx.Selected[2];
			break;
		case SCOPE_OBJECT:
			hWnd = Context->PointCtx.ObjectsList;
			Selected = &Context->PointCtx.Selected[3];
			break;
		case SCOPE_MAGIC:
			hWnd = Context->PointCtx.MagicalObjectsList;
			Selected = &Context->PointCtx.Selected[4];
			break;
		case SCOPE_AREA:
			hWnd = Context->PointCtx.AreasList;
			Selected = &Context->PointCtx.Selected[5];
			break;
		case SCOPE_BAND:
			hWnd = Context->PointCtx.BandsList;
			Selected = &Context->PointCtx.Selected[6];
			break;
		case SCOPE_CREATUREINBAND:
			hWnd = Context->PointCtx.CreaturesInBandList;
			Selected = &Context->PointCtx.Selected[7];
			break;
		}

	Item.mask = LVIF_PARAM;
	Item.iItem = SendMessage(hWnd,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.iSubItem = 0;
	*Selected = Item.iItem;
	if (Item.iItem == -1) return(0);
	if (!SendMessage(hWnd,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item)) return(0);
	if (!Item.lParam) return(0);
	if (!StoreForReal) return(1);

New:	NewLink = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!NewLink)
		{
		MessageBeep(MB_ICONHAND);
		return(0);
		}
	Text = GlobalAlloc(GPTR,2048+MAP_RECTMAXNAME+MAP_RECTMAXINFO);
	if (!Text)
		{
		GlobalFree(NewLink);
		MessageBeep(MB_ICONHAND);
		return(0);
		}

	if (Context->PointCtx.SrceLink)
		{
		switch(Context->PointCtx.SrceLink->node.type)
			{
			case SCOPE_LEVEL:
			case SCOPE_PLAYER:
				break;
			case SCOPE_CREATURE:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->creature,SCOPE_CREATURE);
				if (Old) ((MAPTHING *)Old)->uniqueid = 0;
				break;
			case SCOPE_DOOR:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->door,SCOPE_DOOR);
				if (Old) ((MAPTHING *)Old)->uniqueid = 0;
				break;
			case SCOPE_OBJECT:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->door,SCOPE_OBJECT);
				if (Old) ((MAPTHING *)Old)->uniqueid = 0;
				break;
			case SCOPE_MAGIC:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->door,SCOPE_MAGIC);
				if (Old) ((MAPTHING *)Old)->uniqueid = 0;
				break;
			case SCOPE_AREA:
				break;
			case SCOPE_BAND:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->band,SCOPE_BAND);
				if (Old) ((MTHBAND *)Old)->uniqueid = 0;
				break;
			case SCOPE_CREATUREINBAND:
				Old = Triggers_GetStructureFromUniqueID(Context->PointCtx.SrceLink->creature,SCOPE_CREATUREINBAND);
				if (Old) ((MAPTHINGHERO *)Old)->uniqueid = 0;
				break;
			}
		}

	switch(Context->PointCtx.TempLink.node.type)
		{
		case SCOPE_LEVEL:
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_PLAYER:
			Context->PointCtx.TempLink.player = Item.lParam;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_CREATURE:
			((MAPTHING *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = ((MAPTHING *)Item.lParam)->uniqueid;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_DOOR:
			((MAPTHING *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = ((MAPTHING *)Item.lParam)->uniqueid;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_OBJECT:
			((MAPTHING *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = ((MAPTHING *)Item.lParam)->uniqueid;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_MAGIC:
			((MAPTHING *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = ((MAPTHING *)Item.lParam)->uniqueid;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_AREA:
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = ((MAPRECTEX *)Item.lParam)->id;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			break;
		case SCOPE_BAND:
			((MTHBAND *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = 0;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = ((MTHBAND *)Item.lParam)->uniqueid;
			break;
		case SCOPE_CREATUREINBAND:
			((MAPTHINGHERO *)Item.lParam)->uniqueid = Triggers_GetFreeUniqueID();
			Context->PointCtx.TempLink.player = 0;
			Context->PointCtx.TempLink.area = 0;
			Context->PointCtx.TempLink.creature = ((MAPTHINGHERO *)Item.lParam)->uniqueid;
			Context->PointCtx.TempLink.door = 0;
			Context->PointCtx.TempLink.object = 0;
			Context->PointCtx.TempLink.band = 0;
			for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
				{
				for (i = 0; i != THING_MAXHEROSINBAND; i++)
					if (&Band->heros[i] == (MAPTHINGHERO *)Item.lParam) goto Done;
				Context->PointCtx.TempLink.band++;
				}
			break;
		}

Done:	if (Context->Modify)
		{
		GlobalFree(NewLink);
		Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
		SendMessage(Context->Points,LVM_DELETEITEM,(WPARAM)Item.iItem,(LPARAM)0);
		CopyMemory(Context->PointCtx.SrceLink,&Context->PointCtx.TempLink,sizeof(TRIGGERLINK));
		NewLink = Context->PointCtx.SrceLink;
		}
	else	{
		CopyMemory(NewLink,&Context->PointCtx.TempLink,sizeof(TRIGGERLINK));
		if (Context->Insert)
			{
			Item.mask = LVIF_PARAM;
			Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
			SendMessage(Context->Points,LVM_GETITEM,(WPARAM)0,(LPARAM)&Item);
			List_InsertEntry((NODE *)NewLink,(NODE *)Item.lParam,FALSE);
			}
		else
			List_AddEntry((NODE *)NewLink,&MapActionPoints);
		}

	Item.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	Item.iItem = SendMessage(Context->Points,LVM_GETITEMCOUNT,(WPARAM)0,(LPARAM)0);
	Item.iSubItem = 0;
	Item.pszText = Text;
	i = Triggers_SelectAddPoint(NewLink,&Item,Text,Context);
	if (i != -1)
		{
		Item.iItem = i;
		Item.state = LVIS_SELECTED;
		Item.stateMask = LVIS_SELECTED;
		SendMessage(Context->Points,LVM_SETITEMSTATE,(WPARAM)Item.iItem,(LPARAM)&Item);
		}
	SendMessage(Context->Points,LVM_SORTITEMS,(WPARAM)0,(LPARAM)List_SortItems);
	Item.iItem = SendMessage(Context->Points,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_ALL|LVNI_SELECTED);
	Item.state = LVIS_SELECTED;
	Item.stateMask = LVIS_SELECTED;
	SendMessage(Context->Points,LVM_ENSUREVISIBLE,(WPARAM)Item.iItem,(LPARAM)0);
	MapChanges++;
	GlobalFree(Text);
	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
