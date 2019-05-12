
//<<>-<>>---------------------------------------------------------------------()
/*
	Sous-routines liées aux évènements.
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

extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern CONFIG		Config;
extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de gestion des listes					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Supprime la mémoire utilisée ««««««««««««««««««««««««««««««««««««»

void Triggers_ReleaseMemory()
{
	List_ReleaseMemory(&MapActionPoints);
	List_ReleaseMemory(&MapTriggers);
	List_ReleaseMemory(&MapActions);
	ZeroMemory(TriggersVariables,TRIGGERS_MAXVARIABLES*TRIGGERS_MAXVARLEN);
	ZeroMemory(TriggersTimers,TRIGGERS_MAXTIMERS*TRIGGERS_MAXTIMLEN);
	return;
}


// «»»» Supprime toutes les structures liées ««««««««««««««««««««««««««««»

void Triggers_KillFromId(short ID)
{
	if (ID > 0)
		{
		TRIGGER	*Trigger;

		Trigger = Triggers_GetTriggerById(ID);
		if (!Trigger) return;
		Triggers_KillFromId(Trigger->and);
		Triggers_KillFromId(Trigger->or);
		List_RemEntry((NODE *)Trigger);
		GlobalFree(Trigger);
		return;
		}

	if (ID < 0)
		{
		ACTION	*Action;

		Action = Triggers_GetActionById(ID);
		if (!Action) return;
		Triggers_KillFromId(Action->next);
		List_RemEntry((NODE *)Action);
		GlobalFree(Action);
		return;
		}

	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une zone ««««««««»

void Triggers_CreateActionPtFromLevel(short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_LEVEL;
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'un joueur «««««««»

void Triggers_CreateActionPtFromPlayer(UINT PlayerID, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_PLAYER;
	Link->player = PlayerID;
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une creature ««««»

void Triggers_CreateActionPtFromCreature(MAPTHING *Thing, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_CREATURE;
	Link->creature = Triggers_GetFreeUniqueID();
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);

	Thing->uniqueid = Link->creature;
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une porte «««««««»

void Triggers_CreateActionPtFromDoor(MAPTHING *Thing, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_DOOR;
	Link->door = Triggers_GetFreeUniqueID();
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);

	Thing->uniqueid = Link->door;
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'un objet ««««««««»

void Triggers_CreateActionPtFromObject(MAPTHING *Thing, BOOL Magic, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = (Magic)?SCOPE_MAGIC:SCOPE_OBJECT;
	Link->door = Triggers_GetFreeUniqueID();
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);

	Thing->uniqueid = Link->object;
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une zone ««««««««»

void Triggers_CreateActionPtFromArea(UINT AreaID, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_AREA;
	Link->area = AreaID;
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une bande «««««««»

void Triggers_CreateActionPtFromBand(MTHBAND *Band, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return;

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return;

	Link->node.type = SCOPE_BAND;
	Link->band = Triggers_GetFreeUniqueID();
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);

	Band->uniqueid = Link->band;
	return;
}


// «»»» Création d'un nouveau pt d'action en fonction d'une bande «««««««»

TRIGGERLINK* Triggers_CreateActionPtFromCreatureInBand(MAPTHINGHERO *Hero, UINT Band, short Trigger)
{
	TRIGGERLINK	*Link;

	if (!Trigger) return(NULL);

	Link = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
	if (!Link) return(NULL);

	Link->node.type = SCOPE_CREATUREINBAND;
	Link->band = Band;
	Link->creature = Triggers_GetFreeUniqueID();
	Link->trigger = Trigger;
	List_AddEntry((NODE *)Link,&MapActionPoints);

	Hero->uniqueid = Link->creature;
	return(Link);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Obtention des structures & des identificateurs			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve un identificateur libre ««««««««««««««««««««««««««««««««»

ULONG Triggers_GetFreeUniqueID()
{
	ULONG		 Result;

	for (Result = 1;; Result++)
		if (!Triggers_GetStructureFromUniqueID(Result,SCOPE_CREATURE|SCOPE_DOOR|SCOPE_BAND|SCOPE_CREATUREINBAND)) break;

	return(Result);
}


// «»»» Retrouve une structure à partir d'un identificateur «««««««««««««»

void* Triggers_GetStructureFromUniqueID(ULONG UniqueID, DWORD Flags)
{
	if (Flags&SCOPE_CREATURE)
		{
		MAPTHING	*Thing;

		for (Thing = (MAPTHING *)MapCreatures.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			if (Thing->uniqueid == UniqueID) return(Thing);

		}

	if (Flags&SCOPE_DOOR)
		{
		MAPTHING	*Thing;

		for (Thing = (MAPTHING *)MapDoors.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			if (Thing->uniqueid == UniqueID) return(Thing);
		}

	if (Flags&SCOPE_OBJECT)
		{
		MAPTHING	*Thing;

		for (Thing = (MAPTHING *)MapObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			if (Thing->uniqueid == UniqueID) return(Thing);
		}

	if (Flags&SCOPE_MAGIC)
		{
		MAPTHING	*Thing;

		for (Thing = (MAPTHING *)MapMagicalObjects.next; Thing != 0; Thing = (MAPTHING *)Thing->node.next)
			if (Thing->uniqueid == UniqueID) return(Thing);
		}

	if (Flags&SCOPE_BAND)
		{
		MTHBAND		*Band;

		for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
			if (Band->uniqueid == UniqueID) return(Band);
		}

	if (Flags&SCOPE_CREATUREINBAND)
		{
		MTHBAND		*Band;
		int		 i;

		for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
			{
			i = 0;
			while(Band->heros[i].isvalid)
				{
				if (Band->heros[i].uniqueid == UniqueID) return(&Band->heros[i]);
				i++;
				}
			}
		}

	return(NULL);
}


// «»»» Reforme les liens des évènements ««««««««««««««««««««««««««««««««»

void Triggers_MakeLinks()
{
	TRIGGERLINK	*ActionPt;
	TRIGGER		*Trigger;

	for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
		{
		Trigger = Triggers_GetTriggerById(ActionPt->trigger);
		if (!Trigger) continue;
		Triggers_MakeTriggerLinks(Trigger,0);
		}

	return;
}

//--- Forme les liens déclencheurs<>déclencheurs<>actions ---

void Triggers_MakeTriggerLinks(TRIGGER *Trigger, WORD PrevID)
{
	TRIGGER	*NextTrigger;
	ACTION	*NextAction;

	Trigger->previd = PrevID;

Or:	if (Trigger->or > 0)
		{
		NextTrigger = Triggers_GetTriggerById(Trigger->or);
		if (!NextTrigger)
			{
			Trigger->or = 0x0000;
			goto And;
			}
		Triggers_MakeTriggerLinks(NextTrigger,Trigger->id);
		}
	else if (Trigger->or < 0)
		{
		NextAction = Triggers_GetActionById(Trigger->or);
		if (!NextAction)
			{
			Trigger->or = 0x0000;
			goto And;
			}
		Triggers_MakeActionLinks(NextAction,Trigger->id);
		}

And:	if (Trigger->and < 0)
		{
		NextAction = Triggers_GetActionById(Trigger->and);
		if (!NextAction)
			{
			Trigger->and = 0x0000;
			return;
			}
		Triggers_MakeActionLinks(NextAction,Trigger->id);
		}
	else if (Trigger->and > 0)
		{
		NextTrigger = Triggers_GetTriggerById(Trigger->and);
		if (!NextTrigger)
			{
			Trigger->and = 0x0000;
			return;
			}
		Triggers_MakeTriggerLinks(NextTrigger,Trigger->id);
		}

	return;
}

//--- Forme les liens déclencheurs<>actions<>actions ---

void Triggers_MakeActionLinks(ACTION *Action, WORD PrevID)
{
	TRIGGER	*NextTrigger;
	ACTION	*NextAction;

	Action->previd = PrevID;

	if (Action->next < 0)
		{
		NextAction = Triggers_GetActionById(Action->next);
		if (!NextAction)
			{
			Action->next = 0x0000;
			return;
			}
		Triggers_MakeActionLinks(NextAction,Action->id);
		}
	else if (Action->next > 0)
		{
		NextTrigger = Triggers_GetTriggerById(Action->next);
		if (!NextTrigger)
			{
			Action->next = 0x0000;
			return;
			}
		Triggers_MakeTriggerLinks(NextTrigger,Action->id);
		}

	return;
}


// «»»» Brise les liaisons virtuelles d'un déclencheur ««««««««««««««««««»

void Triggers_UnlinkTrigger(TRIGGER *Trigger)
{
	if (Trigger->previd > 0)
		{
		TRIGGER		*TempTrigger;
		ACTION		*TempAction;
		short		 NextPrevID;

		TempTrigger = Triggers_GetTriggerById(Trigger->previd);
		if (TempTrigger->or == Trigger->id)
			{
			TempTrigger->or = Trigger->or;
			NextPrevID = TempTrigger->id;

			if (Trigger->or > 0)
				{
				TempTrigger = Triggers_GetTriggerById(Trigger->or);
				TempTrigger->previd = NextPrevID;
				}
			else if (Trigger->or < 0)
				{
				TempAction = Triggers_GetActionById(Trigger->or);
				TempAction->previd = NextPrevID;
				}
			}

		if (TempTrigger->and == Trigger->id)
			{
			TempTrigger->and = Trigger->or;
			NextPrevID = TempTrigger->id;

			if (Trigger->or > 0)
				{
				TempTrigger = Triggers_GetTriggerById(Trigger->or);
				TempTrigger->previd = NextPrevID;
				}
			else if (Trigger->or < 0)
				{
				TempAction = Triggers_GetActionById(Trigger->or);
				TempAction->previd = NextPrevID;
				}
			}
		}

	if (Trigger->previd < 0)
		{
		ACTION		*TempAction;
		TRIGGER		*TempTrigger;
		short		 NextPrevID;

		TempAction = Triggers_GetActionById(Trigger->previd);
		TempAction->next = Trigger->or;
		NextPrevID = TempAction->id;

		if (Trigger->or > 0)
			{
			TempTrigger = Triggers_GetTriggerById(Trigger->or);
			TempTrigger->previd = NextPrevID;
			}
		else if (Trigger->or < 0)
			{
			TempAction = Triggers_GetActionById(Trigger->or);
			TempAction->previd = NextPrevID;
			}

		}

	if (Trigger->previd == 0)
		{
		TRIGGERLINK	*Link;
		TRIGGER		*TempTrigger;
		ACTION		*TempAction;

		for (Link = (TRIGGERLINK *)MapActionPoints.next; Link != 0; Link = (TRIGGERLINK *)Link->node.next)
			if (Link->trigger == Trigger->id) break;

		Link->trigger = Trigger->or;

		if (Trigger->or > 0)
			{
			TempTrigger = Triggers_GetTriggerById(Trigger->or);
			TempTrigger->previd = 0;
			}
		else if (Trigger->or < 0)
			{
			TempAction = Triggers_GetActionById(Trigger->or);
			TempAction->previd = 0;
			}
		}

	return;
}


// «»»» Brise les liaisons virtuelles d'une action ««««««««««««««««««««««»

void Triggers_UnlinkAction(ACTION *Action)
{
	TRIGGER			*Trigger;
	ACTION			*PrevAction;
	short			 PrevID = 0;

	for (Trigger = (TRIGGER *)MapTriggers.next; Trigger != 0; Trigger = (TRIGGER *)Trigger->node.next)
		{
		if (Trigger->or == Action->id)
			{
			Trigger->or = Action->next;
			PrevID = Trigger->id;
			goto Remove;
			}
		if (Trigger->and == Action->id)
			{
			Trigger->and = Action->next;
			PrevID = Trigger->id;
			goto Remove;
			}
		}

	for (PrevAction = (ACTION *)MapActions.next; PrevAction != 0; PrevAction = (ACTION *)PrevAction->node.next)
		if (PrevAction->next == Action->id)
			{
			PrevAction->next = Action->next;
			PrevID = PrevAction->id;
			}

Remove:	if (!PrevID)
		{
		MessageBeep(MB_ICONHAND);
		return;
		}

	if (Action->next > 0)
		{
		Trigger = Triggers_GetTriggerById(Action->next);
		Trigger->previd = PrevID;
		}
	else if (Action->next < 0)
		{
		PrevAction = Triggers_GetActionById(Action->next);
		PrevAction->previd = PrevID;
		}

	return;
}


// «»»» Trouve un identificateur libre ««««««««««««««««««««««««««««««««««»

short Triggers_GetTriggerFreeId()
{
	short	Result;

	for (Result = 1;; Result++)
		if (!Triggers_GetTriggerById(Result)) break;

	return(Result);
}


// «»»» Retrouve un déclencheur à l'aide de son identificateur ««««««««««»

TRIGGER* Triggers_GetTriggerById(short Id)
{
	TRIGGER	*Result;

	if (!Id) return(NULL);

	for (Result = (TRIGGER *)MapTriggers.next; Result != 0; Result = (TRIGGER *)Result->node.next)
		if (Result->id == Id) break;

	return(Result);
}


// «»»» Trouve un identificateur libre ««««««««««««««««««««««««««««««««««»

short Triggers_GetActionFreeId()
{
	short	Result;

	for (Result = -1;; Result--)
		if (!Triggers_GetActionById(Result)) break;

	return(Result);
}


// «»»» Retrouve une action à l'aide de son identificateur ««««««««««««««»

ACTION* Triggers_GetActionById(short Id)
{
	ACTION	*Result;

	if (!Id) return(NULL);

	for (Result = (ACTION *)MapActions.next; Result != 0; Result = (ACTION *)Result->node.next)
		if (Result->id == Id) break;

	return(Result);
}


// «»»» Retrouve l'identificateur de l'évènement lié à la zone ««««««««««»

short Triggers_GetFirstTrigger(ULONG Data, DWORD Scope, BOOL *Used)
{
	TRIGGERLINK	*ActionPt;

	if (!Data)
		{
		if (Scope&SCOPE_LEVEL)
			{
			if (Used) *Used = TRUE;
			for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
				if (ActionPt->node.type == SCOPE_LEVEL) return(ActionPt->trigger);
			}
		if (Used) *Used = FALSE;
		return(0);
		}
	if (Used) *Used = TRUE;

	for (ActionPt = (TRIGGERLINK *)MapActionPoints.next; ActionPt != 0; ActionPt = (TRIGGERLINK *)ActionPt->node.next)
		{
		if ((Scope&SCOPE_PLAYER)&&(ActionPt->player == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_CREATURE)&&(ActionPt->creature == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_DOOR)&&(ActionPt->door == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_OBJECT)&&(ActionPt->object == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_MAGIC)&&(ActionPt->object == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_AREA)&&(ActionPt->area == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_BAND)&&(ActionPt->band == Data)) return(ActionPt->trigger);
		else if ((Scope&SCOPE_CREATUREINBAND)&&(ActionPt->creature == Data)) return(ActionPt->trigger);
		}

	if (Used) *Used = FALSE;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Test si un élément est utilisé par les évènements		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Triggers_IsUsed(HWND hWnd, ULONG Data, BOOL *Change, DWORD Flags)
{
	ACTION	*Action;
	MTHBAND	*Band;
	BOOL	 Used;

	if (Change) *Change = TRUE;

	Triggers_GetFirstTrigger(Data,Flags,&Used);
	if (!Used)
		{
		for (Action = (ACTION *)MapActions.next; Action != 0; Action = (ACTION *)Action->node.next)
			{
			switch(Action->ctrl)
				{
				case ACTION_MOVECAMERA:
				case ACTION_ROTATECAMERA:
				case ACTION_SHOWAREA:
				case ACTION_ENABLEFOG:
					if (Flags != SCOPE_AREA) break;
					if (Action->param1 == Data) goto Done;
					break;
				case ACTION_RELEASEBAND:
					if ((Flags == SCOPE_AREA)&&(Action->param5 == Data)) goto Done;
					if (!(Flags&SCOPE_BAND)) break;
					Band = (MTHBAND *)Triggers_GetStructureFromUniqueID(Data,SCOPE_BAND);
					if (!Band) break;
					if (Band->id == Action->param1) goto Done;
					break;
				case ACTION_DESTROYGATE:
					if (Flags != SCOPE_GATE) break;
					if (Action->param5 == Data) goto Done;
					break;
				}
			}
		}

Done:	if ((Used)&&(!Change))
		return(0);
	else if (Used)
		{
		MESSAGEBOXSTRUCT	 Request;
		char			*Text = NULL;
		int			 Result;

		if (Flags&SCOPE_AREA) Text = szTriggersKillUsedArea;
		else if (Flags&SCOPE_GATE) Text = szTriggersKillUsedGate;
		else if ((Flags&SCOPE_CREATURE)&&(Flags&SCOPE_KILL)) Text = szTriggersKillUsedCreature;
		else if ((Flags&SCOPE_CREATURE)&&(Flags&SCOPE_MODIFY)) Text = szTriggersModifyUsedCreature;
		else if ((Flags&SCOPE_CREATURE)&&(Flags&SCOPE_MODIFY2)) Text = szTriggersModify2UsedCreature;
		if (!Text) return(1);

		ZeroMemory(&Request,sizeof(MESSAGEBOXSTRUCT));
		Request.gadgets = szTriggersUsedChoices;
		Request.windowtext = Text;
		Request.windowtitle = (char *)-1;
		Request.requestbits = &Config.RequestsFlags;
		Request.statebit = REQUEST_TRIGGERS;
		Request.flags = MBF_AUTOREPLY;
		Request.defaultgadget = 1;
		Request.defaulticon = MB_ICONQUESTION;
		Request.windowpos = MBP_CENTER;
		Result = Misc_MessageBoxExtArgs(hWnd,&Request);
		if (!Result) return(0);
		if (Result == 2) *Change = FALSE;
		return(1);
		}

	return(-1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
