
//<<>-<>>---------------------------------------------------------------------()
/*
	Traduction des déclencheurs et actions en texte.
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

extern char		TriggersVariables[TRIGGERS_MAXVARIABLES][TRIGGERS_MAXVARLEN];
extern char		TriggersTimers[TRIGGERS_MAXTIMERS][TRIGGERS_MAXTIMLEN];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Transformation d'un déclencheur en texte				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

char* Triggers_FmtTriggerText(TRIGGER *Trigger, char *Text, char *DataText)
{
	switch(Trigger->ctrl)
		{
		case TRIGGER_VARIABLE:
			strcpy(Text,szTriggersIf);
			if (Trigger->param2 == TRIGGERS_MAXVARIABLES-1)
				strcat(Text,szTriggersScore);
			else
				{
				strcat(Text,szTriggersVariable);
				strcat(Text,szSpace);
				strcat(Text,Triggers_FmtVariableText(Trigger->param2,DataText));
				}
			strcat(Text,szSpace);
			strcat(Text,szTriggersIs);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtOperationText(Trigger->param1,(Trigger->param2 == TRIGGERS_MAXVARIABLES-1)?1:0,DataText));
			if (Trigger->param3 == 0x02)
				{
				if (Trigger->param4 == TRIGGERS_MAXVARIABLES-1)
					{
					char *Last = strchr(Text,0);
					*(Last-2) = 0;
					strcat(Text,szTriggersScore2);
					}
				else
					{
					strcat(Text,szTriggersVariable);
					strcat(Text,szSpace);
					strcat(Text,Triggers_FmtVariableText(Trigger->param4,DataText));
					}
				}
			else
				strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			break;
		case TRIGGER_TIMER:
			strcpy(Text,szTriggersIf);
			if (Trigger->param2 == TRIGGERS_MAXTIMERS-1)
				strcat(Text,szTriggersTime);
			else
				{
				strcat(Text,szTriggersTimer);
				strcat(Text,szSpace);
				strcat(Text,Triggers_FmtTimerText(Trigger->param2,DataText));
				}
			strcat(Text,szSpace);
			strcat(Text,szTriggersIs);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtOperationText(Trigger->param1,(Trigger->param2 == TRIGGERS_MAXTIMERS-1)?0:1,DataText));
			strcat(Text,Triggers_FmtTimeText(Trigger->param5,FALSE,DataText));
			break;
		case TRIGGER_DOORDESTROYED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersDoorDestroyed);
			break;
		case TRIGGER_OBJECTCLAIMED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersObjectClaimed);
			break;
		case TRIGGER_APPEAR:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureAppears);
			break;
		case TRIGGER_DEATH:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureDies);
			break;
		case TRIGGER_SLAPPED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureSlapped);
			break;
		case TRIGGER_SEE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureSeesEnnemy);
			break;
		case TRIGGER_JAILED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureJailed);
			break;
		case TRIGGER_TORTURED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureTortured);
			break;
		case TRIGGER_CONVERTED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureConverted);
			break;
		case TRIGGER_TURNCOAT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureTurnCoat);
			break;
		case TRIGGER_HUNGRY:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureHungry);
			break;
		case TRIGGER_AFRAID:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureAfraid);
			break;
		case TRIGGER_STEAL:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureSteal);
			break;
		case TRIGGER_GOAWAY:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureGoAway);
			break;
		case TRIGGER_STUNNED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureStunned);
			break;
		case TRIGGER_TRANSITIONENDS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersTransition);
			break;
		case TRIGGER_NOTHUNGRY:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureNotHungry);
			break;
		case TRIGGER_FIRED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureFired);
			break;
		case TRIGGER_TAKE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureTake);
			break;
		case TRIGGER_CREATURES:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Trigger->param2,Trigger->param5,DataText));
			break;
		case TRIGGER_HAPPYCREATURES:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Trigger->param2,Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtEUText(Trigger->param2,Trigger->param5,szTriggersHappyCreature,DataText));
			break;
		case TRIGGER_ANGRYCREATURES:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Trigger->param2,Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtEUText(Trigger->param2,Trigger->param5,szTriggersAngryCreature,DataText));
			break;
		case TRIGGER_LOSTCREATURES:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param5 > 1) strcat(Text,szTriggersHaveBeenPlural);
			else strcat(Text,szTriggersHaveBeen);
			strcat(Text,szSpace);
			strcat(Text,szTriggersKilled);
			strcat(Text,szTriggersFemale);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersBy);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param4,DataText));
			break;
		case TRIGGER_KILLEDCREATURES:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHasKill);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersBelongTo);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtObjectPlayerText(Trigger->param4,DataText));
			break;
		case TRIGGER_ROOMS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSquare);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtRoomWithPrefix(Trigger->param2,DataText));
			break;
		case TRIGGER_ROOMSSIZE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,szTriggersA);
			if (Triggers_IsFemaleRoom(Trigger->param2)) strcat(Text,szTriggersFemale);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtRoomText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtSpecialPrefix(Triggers_FmtObjectOperationText(Trigger->param1,DataText)));
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSquare);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			break;
		case TRIGGER_ROOMSCONTENT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,szTriggersA);
			if (Triggers_IsFemaleRoom(Trigger->param2)) strcat(Text,szTriggersFemale);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtRoomText(Trigger->param2,DataText));
			strcat(Text,szTriggersThatHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersObject);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			break;
		case TRIGGER_ROOMSTYPES:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtRoomExText(Trigger->param2,Trigger->param5,DataText));
			break;
		case TRIGGER_DOORS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param3 == 0x09)
				{
				strcat(Text,szTriggersBlueprint);
				if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
				strcat(Text,szSpace);
				strcat(Text,szTriggersOf);
				strcat(Text,szSpace);
				}
			strcat(Text,Triggers_FmtDoorText(Trigger->param2,Trigger->param5,DataText));
			if (Trigger->param3 != 0x09)
				{
				strcat(Text,szTriggersBuild);
				strcat(Text,szTriggersFemale);
				if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
				if (Trigger->param3 != 0x11) strcat(Text,szTriggersOrWaiting);
				}
			break;
		case TRIGGER_TRAPS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param3 == 0x09)
				{
				strcat(Text,szTriggersBlueprint);
				if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
				strcat(Text,szSpace);
				strcat(Text,szTriggersOf);
				strcat(Text,szSpace);
				}
			strcat(Text,Triggers_FmtTrapText(Trigger->param2,Trigger->param5,DataText));
			if (Trigger->param3 != 0x09)
				{
				strcat(Text,szTriggersBuild);
				if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
				if (Trigger->param3 != 0x11) strcat(Text,szTriggersOrWaiting);
				}
			break;
		case TRIGGER_DESTROYED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHasDestroyed);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (!Trigger->param2)
				{
				strcat(Text,Triggers_FmtDoorText(0,Trigger->param5,DataText));
				strcat(Text,szSpace);
				strcat(Text,szTriggersOr);
				strcat(Text,szSpace);
				strcat(Text,Triggers_FmtTrapText(0,Trigger->param5,DataText));
				}
			else
				{
				/* Doesn't work since doors and traps have same ID */
				if (Things_GetThingNameFromID(Trigger->param2,Traps))
					strcat(Text,Triggers_FmtTrapText(Trigger->param2,Trigger->param5,DataText));
				else
					strcat(Text,Triggers_FmtDoorText(Trigger->param2,Trigger->param5,DataText));
				}
			break;
		case TRIGGER_SPELLS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHasDiscovered);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtSpellText(Trigger->param2,DataText));
			break;
		case TRIGGER_GOLDAMOUNT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersGoldUnit);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szTriggersGold);
			break;
		case TRIGGER_GOLDMINED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHasCollected);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersGoldUnit);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szTriggersGold);
			break;
		case TRIGGER_MANAAMOUNT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerHas);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szTriggersMana);
			break;
		case TRIGGER_TIMEELAPSED:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtTimeText(Trigger->param5,TRUE,DataText));
			strcat(Text,szTriggersFromBeginning);
			break;
		case TRIGGER_HURTED:
			strcpy(Text,szTriggersIf2);
			strcat(Text,szTriggersRemain);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersHealth);
			break;
		case TRIGGER_CREAENTERAREA:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Trigger->param3,Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersOwnedBy);
			strcat(Text,Triggers_FmtObjectPlayerText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersEnter);
			if (Trigger->param5 > 1) strcat(Text,szTriggersVerbPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheArea);
			break;
		case TRIGGER_CLAIMAREA:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersOwn);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSquare);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheArea);
			break;
		case TRIGGER_CLAIMENTIREAREA:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersOwn);
			strcat(Text,szTriggersAllTheArea);
			break;
		case TRIGGER_AREACONTENT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersAreaContains);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSquare);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtTerrainWithPrefix(Trigger->param3,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersOwnedBy);
			strcat(Text,Triggers_FmtObjectPlayerText(Trigger->param2,DataText));
			break;
		case TRIGGER_BANDCREATED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersTheBand);
			strcat(Text,Triggers_FmtNumberText(Trigger->param2,DataText));
			strcat(Text,szTriggersHasBeenCreated);
			break;
		case TRIGGER_JAILEDINBAND:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param5 > 1) strcat(Text,szTriggersAre);
			else strcat(Text,szTriggersIs);
			strcat(Text,szSpace);
			strcat(Text,szTriggersJailed);
			strcat(Text,szTriggersFemale);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheBand);
			strcat(Text,Triggers_FmtNumberText(Trigger->param2,DataText));
			break;
		case TRIGGER_DEATHSINBAND:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param5 > 1) strcat(Text,szTriggersAre);
			else strcat(Text,szTriggersIs);
			strcat(Text,szSpace);
			strcat(Text,szTriggersDead);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheBand);
			strcat(Text,Triggers_FmtNumberText(Trigger->param2,DataText));
			break;
		case TRIGGER_GOLDDAY:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersGoldDay);
			break;
		case TRIGGER_HEARTDESTROYED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersHeartDestroyed);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param1,DataText));
			break;
		case TRIGGER_CREATURESLEVEL:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param5 > 1) strcat(Text,szTriggersHavePlural);
			else strcat(Text,szTriggersHave);
			strcat(Text,szTriggersReachedLevel);
			strcat(Text,Triggers_FmtNumberText(Trigger->param4,DataText));
			break;
		case TRIGGER_CREATUREEXP:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersCreatureExp);
			strcat(Text,Triggers_FmtOperationText(Trigger->param1,1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			break;
		case TRIGGER_CLICKELEMENT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerClicks);
			strcat(Text,Triggers_FmtElementText(Trigger->param1,Trigger->param2,0,DataText));
			break;
		case TRIGGER_TAKECREATURE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerTakeCreature);
			break;
		case TRIGGER_FREECREATURE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerFreeCreature);
			break;
		case TRIGGER_FIRECREATURE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerFireCreature);
			break;
		case TRIGGER_HURTCREATURE:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerSlapsAnything);
			break;
		case TRIGGER_SELECTAREA:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSelects);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSquare);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheArea);
			break;
		case TRIGGER_STUNNEDINBAND:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			strcat(Text,szSpace);
			if (Trigger->param5 > 1) strcat(Text,szTriggersAre);
			else strcat(Text,szTriggersIs);
			strcat(Text,szSpace);
			strcat(Text,szTriggersStunned);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheBand);
			strcat(Text,Triggers_FmtNumberText(Trigger->param2,DataText));
			break;
		case TRIGGER_SELECTALLAREA:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param2,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSelects);
			strcat(Text,szTriggersAllTheArea);
			break;
		case TRIGGER_ENTERAREAINPOSS:
			strcpy(Text,szTriggersIf);
			strcat(Text,Triggers_FmtPlayerText(Trigger->param2,DataText));
			strcat(Text,szTriggersEnterAreaUsingPossession);
			break;
		case TRIGGER_SLAPCOUNT:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersPlayerSlaps);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersSlap);
			if (Trigger->param5 > 1) strcat(Text,szTriggersPlural);
			strcat(Text,szSpace);
			strcat(Text,szTriggersToIts);
			strcat(Text,Triggers_FmtCreatureText(0,2,DataText));
			break;
		case TRIGGER_GROUPINPOSSESS:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersGroup);
			strcat(Text,Triggers_FmtObjectOperationText(Trigger->param1,DataText));
			strcat(Text,Triggers_FmtNumberText(Trigger->param5,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(0,Trigger->param5,DataText));
			break;
		case TRIGGER_BREACHED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersBreached);
			break;
		case TRIGGER_ENNEMYBREACHED:
			strcpy(Text,szTriggersIf);
			strcat(Text,szTriggersEnnemyBreached);
			break;
		default:
			wsprintf(Text,szTriggersUnknown,Trigger->ctrl);
			break;
		}

	if (Trigger->repeat == 0xFF)
		strcat(Text,szTriggersRepeat);
	else if (Trigger->repeat > 1)
		{
		wsprintf(DataText,szTriggersRepeat2,Trigger->repeat);
		strcat(Text,DataText);
		}

	#if (DEBUG_EVENTS)
	wsprintf(DataText," --> Trigger: $%08lX / Prev: $%08lX / Or: $%08lX / And: $%08lX",(LONG)Trigger->id,(LONG)Trigger->previd,(LONG)Trigger->or,(LONG)Trigger->and);
	strcat(Text,DataText);
	#endif

	return(Text);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Transformation d'une action en texte				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

char* Triggers_FmtActionText(ACTION *Action, char *Text, char *DataText)
{
	switch(Action->ctrl)
		{
		case ACTION_ADDCREATURE:
			strcpy(Text,szActionsAdd);
			strcat(Text,szSpace);
			strcat(Text,szActionsA);
			if (Triggers_IsFemale(Action->param1)) strcat(Text,szTriggersFemale);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Action->param1,1,DataText));
			strcat(Text,szSpace);
			strcat(Text,szActionsOfLevel);
			strcat(Text,Triggers_FmtNumberText(Action->param3,DataText));
			strcat(Text,szActionsAtPosition);
			strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
			strcat(Text,szActionsPositionSeparator);
			strcat(Text,Triggers_FmtNumberText(Action->param7,DataText));
			strcat(Text,szActionsPositionEnd);
			strcat(Text,szActionsFor);
			strcat(Text,Triggers_FmtPlayerText(Action->param2,DataText));
			break;
		case ACTION_SHOWCRYSTAL:
			strcpy(Text,szActionsShowCrystal);
			break;
		case ACTION_ADDELEMENT:
			strcpy(Text,(Action->param4)?szActionsAdd:szActionsRemove);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtElementText(Action->param2,Action->param3,1,DataText));
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
			break;
		case ACTION_SETVARIABLE:
			switch(Action->param2)
				{
				case 0x0A:
					strcpy(Text,szActionsSetValue);
					strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
					break;
				case 0x0C:
					if (Action->param5 == TRIGGERS_MAXVARIABLES-1)
						strcpy(Text,szActionsSetToScore);
					else
						{
						strcpy(Text,szActionsSetToVarValue);
						strcat(Text,Triggers_FmtVariableText(Action->param5,DataText));
						}
					break;
				case 0x12:
					strcpy(Text,szActionsAddValue);
					strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
					break;
				case 0x14:
					if (Action->param5 == TRIGGERS_MAXVARIABLES-1)
						strcpy(Text,szActionsAddScore);
					else
						{
						strcpy(Text,szActionsAddVarValue);
						strcat(Text,Triggers_FmtVariableText(Action->param5,DataText));
						}
					break;
				case 0x22:
					strcpy(Text,szActionsSubValue);
					strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
					break;
				case 0x24:
					if (Action->param5 == TRIGGERS_MAXVARIABLES-1)
						strcpy(Text,szActionsSubScore);
					else
						{
						strcpy(Text,szActionsSubVarValue);
						strcat(Text,Triggers_FmtVariableText(Action->param5,DataText));
						}
					break;
				default:
					strcpy(Text,szUnknown);
					strcat(Text,szSpace);
					strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
					break;
				}
			if (Action->param1 == TRIGGERS_MAXVARIABLES-1)
				strcat(Text,szActionsToScore);
			else
				{
				strcat(Text,szActionsToVariable);
				strcat(Text,Triggers_FmtVariableText(Action->param1,DataText));
				}
			break;
		case ACTION_SETTIMER:
			strcpy(Text,szActionsSetTimer);
			strcat(Text,Triggers_FmtTimerText(Action->param1,DataText));
			break;
		case ACTION_SHOWELEMENT:
			strcpy(Text,(Action->param3)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsStars);
			strcat(Text,Triggers_FmtElementText(Action->param1,Action->param2,0,DataText));
			if ((Action->param5)&&(Action->param3))
				{
				strcat(Text,szActionsDuring);
				strcat(Text,Triggers_FmtTimeText(Action->param5,FALSE,DataText));
				}
			break;
		case ACTION_VICTORY:
			strcpy(Text,szActionsVictory);
			break;
		case ACTION_DEFEAT:
			strcpy(Text,szActionsDefeat);
			break;
		case ACTION_RELEASEBAND:
			strcpy(Text,szActionsReleaseBand);
			strcat(Text,Triggers_FmtNumberText(Action->param1,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersInTheArea);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
			switch(Action->param2)
				{
				case 1:	strcat(Text,szActionsBandInvasion);
					break;
				case 2:	strcat(Text,szActionsBandRandom);
					break;
				}
			break;
		case ACTION_CHANGEPARAMS:
			strcpy(Text,szActionsChangeParams);
			switch(Action->param2)
				{
				case 0x0B:
					strcat(Text,szActionsChangeDestroyRooms);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x0C:
					strcat(Text,szActionsChangeDestroyWalls);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x0D:
					strcat(Text,szActionsChangeStealGold);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x0E:
					strcat(Text,szActionsChangeStealSpells);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x11:
					strcat(Text,szActionsChangeStealCrates);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x12:
					strcat(Text,szActionsChangeKillCreatures);
					strcat(Text,Triggers_FmtObjectPlayerText(Action->param1,DataText));
					break;
				case 0x13:
					strcat(Text,szActionsChangeAttack);
					strcat(Text,Triggers_FmtPlayerText(Action->param1,DataText));
					break;
				case 0x16:
					strcat(Text,szActionsChangeStay);
					break;
				case 0x17:
					strcat(Text,szActionsChangeArea);
					strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
					break;
				case 0x1B:
					strcat(Text,szActionsChangeJail);
					break;
				default:
					strcat(Text,szActionsChangeUnknown);
					break;
				}
			break;
		case ACTION_SHOWAREA:
			strcpy(Text,(Action->param2)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsHighlightArea);
			strcat(Text,Triggers_FmtNumberText(Action->param1,DataText));
			break;
		case ACTION_ENABLEFOG:
			strcpy(Text,(Action->param2)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsFog);
			strcat(Text,Triggers_FmtNumberText(Action->param1,DataText));
			break;
		case ACTION_ALLIES:
			strcpy(Text,(Action->param3)?szActionsClear:szActionsSet);
			strcat(Text,szActionsAllies);
			strcat(Text,Triggers_FmtPlayerText(Action->param1,DataText));
			strcat(Text,szActionsAnd);
			strcat(Text,Triggers_FmtPlayerText(Action->param2,DataText));
			break;
		case ACTION_HASCRYSTAL:
			strcpy(Text,szActionsHasCrystal);
			break;
		case ACTION_POSSESSCREATURE:
			strcpy(Text,szActionsPossessCreature);
			break;
		case ACTION_CHANGESQUARE:
			strcpy(Text,szActionsChangeSquare);
			strcat(Text,Triggers_FmtNumberText(Action->param5-1,DataText));
			strcat(Text,szActionsPositionSeparator);
			strcat(Text,Triggers_FmtNumberText(Action->param7-1,DataText));
			strcat(Text,szActionsPositionEnd);
			strcat(Text,szSpace);
			strcat(Text,szTriggersMaterial);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtTerrainText(Action->param1,DataText));
			strcat(Text,szSpace);
			strcat(Text,szTriggersOwnedBy);
			strcat(Text,Triggers_FmtObjectPlayerText(Action->param2,DataText));
			break;
		case ACTION_DISPLAYTIMER:
			strcpy(Text,(Action->param5)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szSpace);
			strcat(Text,szTriggersTime);
			strcat(Text,szActionsTimer);
			if (Action->param5)
				{
				strcat(Text,szActionsOpen);
				strcat(Text,Triggers_FmtTimeText(Action->param5,FALSE,DataText));
				strcat(Text,szActionsPositionEnd);
				}
			break;
		case ACTION_SAY:
			strcpy(Text,szActionsSay);
			strcat(Text,Triggers_FmtVoiceText(Action->param1,DataText));
			if (Action->param5) strcat(Text,szActionsSayNoText);
			break;
		case ACTION_MOVECAMERA:
			wsprintf(Text,szActionsPlaceCamera,Action->param1);
			break;
		case ACTION_ROTATECAMERA:
			wsprintf(Text,szActionsRotateCamera,Action->param3,Action->param1);
			strcat(Text,Triggers_FmtTimeText(Action->param5,FALSE,DataText));
			break;
		case ACTION_GENERATE:
			strcpy(Text,szActionsGenerate);
			strcat(Text,szSpace);
			strcat(Text,szActionsA);
			if (Triggers_IsFemale(Action->param1)) strcat(Text,szTriggersFemale);
			strcat(Text,szSpace);
			strcat(Text,Triggers_FmtCreatureText(Action->param1,1,DataText));
			strcat(Text,szSpace);
			strcat(Text,szActionsOfLevel);
			strcat(Text,Triggers_FmtNumberText(Action->param2,DataText));
			strcat(Text,szActionsFromAPortal);
			break;
		case ACTION_MAKEHUNGRY:
			strcpy(Text,szActionsMakeHungry);
			break;
		case ACTION_SHOWHEALTH:
			strcpy(Text,szActionsShowHealth);
			strcat(Text,szActionsDuring);
			strcat(Text,Triggers_FmtTimeText(Action->param1,FALSE,DataText));
			break;
		case ACTION_CAMERAPATH:
			wsprintf(Text,szActionsCameraPath,Action->param1,Action->param2);
			strcat(Text,(Action->param3)?szActionsHideCeiling:szActionsShowCeiling);
			break;
		case ACTION_DESTROYGATE:
			wsprintf(Text,szActionsDestroyGate,Action->param5);
			break;
		case ACTION_RUN:
			if (Action->param1) strcpy(Text,szActionsMake);
			else strcpy(Text,szActionsDontMake);
			strcat(Text,szActionsRun);
			break;
		case ACTION_MUSICLEVEL:
			strcpy(Text,szActionsMusicLevel);
			strcat(Text,Triggers_FmtNumberText(Action->param1,DataText));
			break;
		case ACTION_REMOVE:
			strcpy(Text,szActionsRemoveCreature);
			break;
		case ACTION_ATTACK:
			if (Action->param1) strcpy(Text,szActionsAttack);
			else strcpy(Text,szActionsDontAttack);
			break;
		case ACTION_PORTALS:
			strcpy(Text,(Action->param1)?szActionsOpened:szActionsClosed);
			strcat(Text,szActionsPortals);
			break;
		case ACTION_CHANGEVIEW:
			strcpy(Text,(Action->param1)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsChangeView);
			break;
		case ACTION_PRIMARYTARGET:
			if (Action->param1 > 0) strcpy(Text,szActionsActivate);
			else strcpy(Text,szActionsDeactivate);
			strcat(Text,szActionsPrimaryTarget);
			switch(Action->param1)
				{
				case 0:	break;
				case 2:	strcat(Text,szActionsPrimaryTarget2);
					break;
				case 3:	strcat(Text,szActionsPrimaryTarget3);
					break;
				default:strcat(Text,szActionsPrimaryTarget1);
					break;
				}
			break;
		case ACTION_SHOW:
			strcpy(Text,szActionsShow);
			break;
		case ACTION_MOODS:
			strcpy(Text,(Action->param1)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsMoods);
			break;
		case ACTION_SYSTEMMESSAGES:
			strcpy(Text,(Action->param1)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsSystemMessages);
			break;
		case ACTION_SHOWNEXTROOM:
			if (Action->param1)
				{
				strcpy(Text,szActionsNextRoom);
				strcat(Text,Triggers_FmtRoomText(Action->param1,DataText));
				}
			else
				strcpy(Text,szActionsNoMoreNextRoom);
			break;
		case ACTION_TOGGLEEFFECT:
			wsprintf(Text,szActionsToggleEffect,(Action->param2)?szActionsActivate:szActionsDeactivate,Action->param1);
			break;
		case ACTION_CHANGEOWNER:
			wsprintf(Text,szActionsChangeOwner,Action->param5-1,Action->param7-1);
			strcat(Text,Triggers_FmtObjectPlayerText(Action->param2,DataText));
			break;
		case ACTION_SLAPSREMAINING:
			if (Action->param5)
				{
				strcpy(Text,szActionsSlapsRemaining);
				strcat(Text,Triggers_FmtNumberText(Action->param5,DataText));
				}
			else
				strcpy(Text,szActionsNoMoreSlaps);
			break;
		case ACTION_TIMERSPEECH:
			strcpy(Text,(Action->param1)?szActionsActivate:szActionsDeactivate);
			strcat(Text,szActionsTimerSpeech);
			break;
		default:
			wsprintf(Text,szActionsUnknown,Action->ctrl);
			break;
		}

	if ((*((unsigned char *)Text) >= 'a')&&(*((unsigned char *)Text) <= 'z'))
		*Text = (char)(*Text-0x20);

	#if (DEBUG_EVENTS)
	wsprintf(DataText," --> Action: $%08lX / Prev: $%08lX / Next: $%08lX",(LONG)Action->id,(LONG)Action->previd,(LONG)Action->next);
	strcat(Text,DataText);
	#endif

	return(Text);
}



// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Transformation des paramètres en texte				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Transforme une voix en texte ««««««««««««««««««««««««««««««««««««»

char* Triggers_FmtVoiceText(LONG VoiceID, char *DataText)
{
	wsprintf(DataText,szActionsTextNumber,VoiceID);
	return(DataText);
}


// «»»» Transforme les variables en texte «««««««««««««««««««««««««««««««»

char* Triggers_FmtVariableText(LONG VariableID, char *DataText)
{
	if (TriggersVariables[VariableID][0])
		{
		long	Variable;

		Variable = atol(&TriggersVariables[VariableID][0]);
		ltoa(Variable,DataText,10);
		if ((Variable == VariableID)&&(strlen(DataText) == strlen(&TriggersVariables[VariableID][0])))
			goto Done;

		wsprintf(DataText,szQuotedText,&TriggersVariables[VariableID][0]);
		return(DataText);
		}

Done:	wsprintf(DataText,szNumber,VariableID);
	return(DataText);
}


// «»»» Transforme les compteurs en texte «««««««««««««««««««««««««««««««»

char* Triggers_FmtTimerText(LONG TimerID, char *DataText)
{
	if (TriggersTimers[TimerID][0])
		{
		long	Timer;

		Timer = atol(&TriggersTimers[TimerID][0]);
		ltoa(Timer,DataText,10);
		if ((Timer == TimerID)&&(strlen(DataText) == strlen(&TriggersTimers[TimerID][0])))
			goto Done;

		wsprintf(DataText,szQuotedText,&TriggersTimers[TimerID][0]);
		return(DataText);
		}

Done:	wsprintf(DataText,szNumber,TimerID);
	return(DataText);
}


// «»»» Transforme un temps en texte ««««««««««««««««««««««««««««««««««««»

char* Triggers_FmtTimeText(LONG Time, BOOL Elapsed, char *DataText)
{
	LONG	Hours;
	LONG	Minutes;
	LONG	Seconds;
	char	Number[32];

	Hours = Time/60/60;
	Minutes = Time/60%60;
	Seconds = Time%60;
	*DataText = 0;

	if (Hours)
		{
		wsprintf(Number,szNumber,Hours);
		strcat(DataText,Number);
		strcat(DataText,szSpace);
		strcat(DataText,szTriggersHour);
		if (Hours > 1) strcat(DataText,szTriggersPlural);
		}
	if (Minutes)
		{
		wsprintf(Number,szNumber,Minutes);
		if (Hours) strcat(DataText,szSpace);
		strcat(DataText,Number);
		strcat(DataText,szSpace);
		strcat(DataText,szTriggersMinute);
		if (Minutes > 1) strcat(DataText,szTriggersPlural);
		}
	if (Seconds)
		{
		wsprintf(Number,szNumber,Seconds);
		if ((Hours)||(Minutes)) strcat(DataText,szSpace);
		strcat(DataText,Number);
		strcat(DataText,szSpace);
		strcat(DataText,szTriggersSecond);
		if (Seconds > 1) strcat(DataText,szTriggersPlural);
		}

	if ((!Hours)&&(!Minutes)&&(!Seconds))
		{
		wsprintf(Number,szNumber,0);
		strcat(DataText,Number);
		strcat(DataText,szSpace);
		strcat(DataText,szTriggersSecond);
		}

	if (Elapsed)
		{
		if (Hours)
			{
			if (Hours > 1) strcat(DataText,szTriggersElapsedPlural);
			else strcat(DataText,szTriggersElapsed);
			}
		else if (Minutes)
			{
			if (Minutes > 1) strcat(DataText,szTriggersElapsedPlural);
			else strcat(DataText,szTriggersElapsed);
			}
		else
			{
			if (Seconds > 1) strcat(DataText,szTriggersElapsedPlural);
			else strcat(DataText,szTriggersElapsed);
			}
		}

	return(DataText);
}


// «»»» Transforme un nombre en texte «««««««««««««««««««««««««««««««««««»

char* Triggers_FmtNumberText(LONG Number, char *DataText)
{
	wsprintf(DataText,szNumber,Number);
	return(DataText);
}


// «»»» Transforme une opération en texte «««««««««««««««««««««««««««««««»

char* Triggers_FmtOperationText(LONG Operation, BOOL Male, char *DataText)
{
	switch(Operation)
		{
		case 0x01:
			strcpy(DataText,szTriggersLower);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersFrom);
			break;
		case 0x02:
			strcpy(DataText,szTriggersLower);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersOr);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersEqual);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersFrom);
			break;
		case 0x03:
			strcpy(DataText,szTriggersEqual);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersFrom);
			break;
		case 0x04:
			strcpy(DataText,szTriggersGreater);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersFrom);
			break;
		case 0x05:
			strcpy(DataText,szTriggersGreater);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersOr);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersEqual);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersFrom);
			break;
		case 0x06:
			strcpy(DataText,szTriggersDifferent);
			if (!Male) strcat(DataText,szTriggersFemale);
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersOf);
			break;
		default:
			strcpy(DataText,szUnknown);
			break;
		}

	strcat(DataText,szSpace);
	return(DataText);
}


// «»»» Transforme une opération en texte «««««««««««««««««««««««««««««««»

char* Triggers_FmtObjectOperationText(LONG Operation, char *DataText)
{
	switch(Operation)
		{
		case 0x01:
			strcpy(DataText,szTriggersLessThan);
			strcat(DataText,szSpace);
			break;
		case 0x02:
			strcpy(DataText,szTriggersLessOrEqualThan);
			strcat(DataText,szSpace);
			break;
		case 0x03:
			*DataText = 0;
			break;
		case 0x04:
			strcpy(DataText,szTriggersMoreThan);
			strcat(DataText,szSpace);
			break;
		case 0x05:
			strcpy(DataText,szTriggersMoreOrEqualThan);
			strcat(DataText,szSpace);
			break;
		case 0x06:
			strcpy(DataText,szTriggersDifferentThan);
			strcat(DataText,szSpace);
			break;
		default:
			strcpy(DataText,szUnknown);
			strcat(DataText,szSpace);
			break;
		}

	return(DataText);
}


// «»»» Transforme une créature en texte ««««««««««««««««««««««««««««««««»

char* Triggers_FmtCreatureText(LONG CreatureID, LONG NumberOfCreature, char *DataText)
{
	BOOL	 Done;
	char	*ThingName;
	char	*Begin;
	char	*End;

	if (!CreatureID)
		{
		strcpy(DataText,szTriggersCreature);
		if (NumberOfCreature > 1) strcat(DataText,szTriggersPlural);
		return(DataText);
		}

	ThingName = Things_GetThingNameFromID(CreatureID,TrCreatures);
	if (!ThingName)
		{
		strcpy(DataText,szUnknown);
		return(DataText);
		}

	Done = FALSE;
	Begin = ThingName;
	*DataText = 0;
Loop:	End = strchr(Begin,' ');
	if (End)
		strncat(DataText,Begin,End-Begin);
	else
		{
		strcat(DataText,Begin);
		End = strchr(ThingName,'\0');
		Done = TRUE;
		}
	switch(*(--End))
		{
		case 'e':
		case 'i':
		case 'r':
		case 'n':
		case 'l':
		case 'd':
		case 'u':
		case 't':
			if (NumberOfCreature > 1)
				{
				Begin = strrchr(DataText,' ');
				if (Begin)
					{
					Begin++;
					if (!strcmpi(Begin,szTriggersReginald)) break;
					if (!strcmpi(Begin,szTriggersBalder)) break;
					if (!strcmpi(Begin,szTriggersTristan)) break;
					if (!strcmpi(Begin,szTriggersRoyal))
						{
						*Begin = 0;
						strcat(Begin,szTriggersRoyals);
						break;
						}
					}
				strcat(DataText,szTriggersPlural);
				}
			break;
		}
	Begin = End+2;
	if (!Done)
		{
		strcat(DataText,szSpace);
		goto Loop;
		}

	if ((*((unsigned char *)DataText) >= 'A')&&(*((unsigned char *)DataText) <= 'Z'))
		*DataText = (char)(*DataText+0x20);

	return(DataText);
}


// «»»» Transforme un mot se terminant par "eu" «««««««««««««««««««««««««»

char* Triggers_FmtEUText(LONG CreatureID, LONG NumberOfCreature, char *Text, char *DataText)
{
	strcpy(DataText,Text);
	if (Triggers_IsFemale(CreatureID))
		{
		if (NumberOfCreature > 1) strcat(DataText,szTriggersEUFemalePlural);
		else strcat(DataText,szTriggersEUFemale);
		}
	else
		strcat(DataText,szTriggersEUX);

	return(DataText);
}


// «»»» Transforme un gardien en texte ««««««««««««««««««««««««««««««««««»

char* Triggers_FmtPlayerText(LONG PlayerID, char *DataText)
{
	switch(PlayerID)
		{
		case 0x00:
			strcpy(DataText,szTriggersAnyOne);
			break;
		case 0x01:
			strcpy(DataText,szTriggersHeros);
			break;
		case 0x02:
			strcpy(DataText,szTriggersNeutral);
			break;
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			strcpy(DataText,szTriggersKeeper);
			break;
		default:
			strcpy(DataText,szUnknown);
			break;
		}

	if ((PlayerID >= 0x03)&&(PlayerID <= 0x07))
		{
		char	Keeper[4];

		strcat(DataText,szSpace);
		wsprintf(Keeper,szNumber,PlayerID-2);
		strcat(DataText,Keeper);
		}

	return(DataText);
}


// «»»» Transforme un gardien en texte (appartenance) «««««««««««««««««««»

char* Triggers_FmtObjectPlayerText(LONG PlayerID, char *DataText)
{
	switch(PlayerID)
		{
		case 0x00:
			strcpy(DataText,szTriggersObjectAnyOne);
			break;
		case 0x01:
			strcpy(DataText,szTriggersObjectHeros);
			break;
		case 0x02:
			strcpy(DataText,szTriggersObjectNeutral);
			break;
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			strcpy(DataText,szTriggersObjectKeeper);
			break;
		default:
			strcpy(DataText,szUnknown);
			break;
		}

	if ((PlayerID >= 0x03)&&(PlayerID <= 0x07))
		{
		char	Keeper[4];

		strcat(DataText,szSpace);
		wsprintf(Keeper,szNumber,PlayerID-2);
		strcat(DataText,Keeper);
		}

	return(DataText);
}


// «»»» Place le préfix "d'" ou "de" avant un texte «««««««««««««««««««««»

char* Triggers_FmtSpecialPrefix(char *DataText)
{
	if (*DataText == 'a')
		{
		strcpy(DataText,szTriggersOf2);
		return(DataText);
		}

	strcpy(DataText,szTriggersOf);
	strcat(DataText,szSpace);
	return(DataText);
}


// «»»» Transforme une salle en texte avec préfixe ««««««««««««««««««««««»

char* Triggers_FmtRoomWithPrefix(LONG RoomID, char *DataText)
{
	char	TempRoom[64];

	*DataText = 0;
	Triggers_FmtRoomText(RoomID,TempRoom);
	switch(TempRoom[0])
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
                case 'h':
			strcat(DataText,szTriggersOf2);
			break;
		default:
			strcat(DataText,szTriggersOf);
			strcat(DataText,szSpace);
			break;
		}
	strcat(DataText,TempRoom);
	return(DataText);
}


// «»»» Transforme une salle en texte avec mise au pluriel ««««««««««««««»

char* Triggers_FmtRoomExText(LONG RoomID, LONG NumberOfRooms, char *DataText)
{
	char	 TempRoom[64];
	char	*Begin;
	char	*End;
	BOOL	 Done;

	Triggers_FmtRoomText(RoomID,TempRoom);
	if (!RoomID)
		{
		strcpy(DataText,szTriggersRoom);
		if (NumberOfRooms > 1) strcat(DataText,szTriggersPlural);
		return(DataText);
		}

	*DataText = 0;
	Done = FALSE;
	Begin = TempRoom;
	End = strchr(Begin,' ');
	if (End)
		strncat(DataText,Begin,End-Begin);
	else
		{
		strcat(DataText,Begin);
		End = strchr(TempRoom,'\0');
		Done = TRUE;
		}
	if (NumberOfRooms > 1) strcat(DataText,szTriggersPlural);
	if (!Done)
		{
		strcat(DataText,End);
		if (!strncmpi(DataText,szTriggersBridge,strlen(szTriggersBridge))) goto Done;
		if (!strncmpi(DataText,szTriggersHeart,strlen(szTriggersHeart))) goto Done;
		if (!strncmpi(DataText,szTriggersPortal,strlen(szTriggersPortal))) goto Done;
		if (!strncmpi(DataText,szTriggersRoom,strlen(szTriggersRoom))) goto Done;
		if (NumberOfRooms > 1) strcat(DataText,szTriggersPlural);
		}

Done:	return(DataText);
}


// «»»» Transforme une salle en texte «««««««««««««««««««««««««««««««««««»

char* Triggers_FmtRoomText(LONG RoomID, char *DataText)
{
	if (!RoomID)
		strcpy(DataText,szTriggersAnyRoom);
	else if (RoomID == 0x03)
		strcpy(DataText,szMapWorldPortal);
	else if (RoomID == 0x05)
		strcpy(DataText,szMapWorldHeart);
	else
		{
		int	i;

		for (i = 0; VarRooms[i].Id != 0; i++)
			if (VarRooms[i].Variable == RoomID)
				{
				strcpy(DataText,VarRooms[i].Name);
				break;
				}
		if (VarRooms[i].Id == 0) strcpy(DataText,szUnknown);
		}

	if ((*((unsigned char *)DataText) >= 'A')&&(*((unsigned char *)DataText) <= 'Z'))
		*DataText = (char)(*DataText+0x20);

	return(DataText);
}


// «»»» Transforme une porte en texte «««««««««««««««««««««««««««««««««««»

char* Triggers_FmtDoorText(LONG DoorID, LONG NumberOfDoors, char *DataText)
{
	BOOL	 Done;
	char	*ThingName;
	char	*Begin;
	char	*End;

	if (!DoorID) ThingName = szTriggersDoor;
	else ThingName = Things_GetThingNameFromID(DoorID,Doors);
	if (!ThingName)
		{
		strcpy(DataText,szUnknown);
		return(DataText);
		}

	Done = FALSE;
	Begin = ThingName;
	*DataText = 0;
Loop:	End = strchr(Begin,' ');
	if (End)
		strncat(DataText,Begin,End-Begin);
	else
		{
		strcat(DataText,Begin);
		End = strchr(ThingName,'\0');
		Done = TRUE;
		}
	if (!strncmpi(Begin,szTriggersOf3,strlen(szTriggersOf3)))
		{
		strcat(DataText,End);
		goto Done;
		}
	if (!strncmpi(Begin,szTriggersMaterial,strlen(szTriggersMaterial)))
		{
		strcat(DataText,End);
		goto Done;
		}
	switch(*(--End))
		{
		case 'e':
			if (NumberOfDoors > 1)
				strcat(DataText,szTriggersPlural);
			break;
		}
	Begin = End+2;
	if (!Done)
		{
		strcat(DataText,szSpace);
		goto Loop;
		}

Done:	if ((*((unsigned char *)DataText) >= 'A')&&(*((unsigned char *)DataText) <= 'Z'))
		*DataText = (char)(*DataText+0x20);

	return(DataText);
}


// «»»» Transforme un piège en texte ««««««««««««««««««««««««««««««««««««»

char* Triggers_FmtTrapText(LONG TrapID, LONG NumberOfTraps, char *DataText)
{
	char	*ThingName;
	char	*Begin;

	strcpy(DataText,szTriggersTrap);
	if (NumberOfTraps > 1) strcat(DataText,szTriggersPlural);
	if (TrapID)
		{
		strcat(DataText,szSpace);
		ThingName = Things_GetThingNameFromID(TrapID,Traps);
		if (!ThingName)
			{
			strcpy(DataText,szUnknown);
			return(DataText);
			}
		Begin = strchr(DataText,'\0');
		strcat(DataText,ThingName);
		if ((*((unsigned char *)Begin) >= 'A')&&(*((unsigned char *)Begin) <= 'Z'))
			*Begin = (char)(*Begin+0x20);
		}

	return(DataText);
}


// «»»» Transforme un sort en texte «««««««««««««««««««««««««««««««««««««»

char* Triggers_FmtSpellText(LONG SpellID, char *DataText)
{
	char	*ThingName;
	char	*Begin;

	strcpy(DataText,szTriggersThe);
	strcat(DataText,szSpace);
	strcat(DataText,szTriggersSpell);
	strcat(DataText,szSpace);
	ThingName = Things_GetThingNameFromID(SpellID,Spells);
	if (!ThingName)
		{
		strcpy(DataText,szUnknown);
		return(DataText);
		}
	Begin = strchr(DataText,'\0');
	strcat(DataText,ThingName);
	if ((*((unsigned char *)Begin) >= 'A')&&(*((unsigned char *)Begin) <= 'Z'))
		*Begin = (char)(*Begin+0x20);

	return(DataText);
}


// «»»» Transforme un terrain en texte (avec préfixe) «««««««««««««««««««»

char* Triggers_FmtTerrainWithPrefix(LONG TerrainID, char *DataText)
{
	char	TempTerrain[64];

	Triggers_FmtTerrainText(TerrainID,TempTerrain);
	switch(TempTerrain[0])
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'h':
			strcpy(DataText,szTriggersOf2);
			break;
		default:
			strcpy(DataText,szTriggersOf);
			strcat(DataText,szSpace);
			break;
		}
	strcat(DataText,TempTerrain);

	return(DataText);
}


// «»»» Transforme un terrain en texte ««««««««««««««««««««««««««««««««««»

char* Triggers_FmtTerrainText(LONG TerrainID, char *DataText)
{
	char	*ThingName;

	if (!TerrainID)
		ThingName = szTriggersTerrain;
	else
		{
		int	i;

		ThingName = NULL;

		for (i = 0; Terrain[i].Id != 0; i++)
			if ((Terrain[i].Id&0xFF000000) == (TerrainID<<24))
				ThingName = Terrain[i].Name;

		if (!ThingName) ThingName = szUnknown;
		}

	strcpy(DataText,ThingName);
	if ((*((unsigned char *)DataText) >= 'A')&&(*((unsigned char *)DataText) <= 'Z'))
		*DataText = (char)(*DataText+0x20);

	return(DataText);
}


// «»»» Transforme un élément en texte ««««««««««««««««««««««««««««««««««»

char* Triggers_FmtElementText(LONG Type, LONG ElementID, LONG Prefix, char *DataText)
{
	char	 TempRoom[64];
	char	*ThingName = NULL;

	switch(Type)
		{
		case 0:	strcpy(DataText,Things_GetThingNameFromID(ElementID,Interface));
			ThingName = (char *)-1;
			break;
		case 1:	ThingName = Triggers_FmtRoomText(ElementID,TempRoom);
			switch(*ThingName)
				{
				case 'a':
				case 'h':
					switch(Prefix)
						{
						case 0:	strcpy(DataText,szTriggersOf6);
							break;
						case 1:	strcpy(DataText,szTriggersThe1);
							break;
						}
					break;
				case 't':
				case 'c':
				Male:	switch(Prefix)
						{
						case 0:	strcpy(DataText,szTriggersOf4);
							strcat(DataText,szSpace);
							break;
						case 1:	strcpy(DataText,szTriggersThe);
							strcat(DataText,szSpace);
							break;
						}
					break;
				case 'p':
					if (*(ThingName+1) != 'r') goto Male;
				default:
					switch(Prefix)
						{
						case 0:	strcpy(DataText,szTriggersOf5);
							strcat(DataText,szSpace);
							break;
						case 1:	strcpy(DataText,szTriggersThe2);
							strcat(DataText,szSpace);
							break;
						}
					break;
				}
			break;
		case 2:	Triggers_FmtCreatureText(ElementID,1,TempRoom);
			switch(Prefix)
				{
				case 0: if (Triggers_IsFemale(ElementID))
						strcpy(DataText,szTriggersOf5);
					else
						strcpy(DataText,szTriggersOf4);
					break;
				case 1:	if (Triggers_IsFemale(ElementID))
						strcpy(DataText,szTriggersThe2);
					else
						strcpy(DataText,szTriggersThe);
					break;
				}
			strcat(DataText,szSpace);
			ThingName = TempRoom;
			break;
		case 3:	switch(Prefix)
				{
				case 0:	strcpy(DataText,szTriggersOf5);
					break;
				case 1:	strcpy(DataText,szTriggersThe2);
					break;
				}
			strcat(DataText,szSpace);
			ThingName = Things_GetThingNameFromID(ElementID,Doors);
			break;
		case 4:	switch(Prefix)
				{
				case 0:	strcpy(DataText,szTriggersOf4);
					break;
				case 1:	strcpy(DataText,szTriggersThe);
					break;
				}
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersTrap);
			strcat(DataText,szSpace);
			ThingName = Things_GetThingNameFromID(ElementID,Traps);
			break;
		case 5:	switch(Prefix)
				{
				case 0:	strcpy(DataText,szTriggersOf4);
					break;
				case 1:	strcpy(DataText,szTriggersThe);
					break;
				}
			strcat(DataText,szSpace);
			strcat(DataText,szTriggersSpell);
			strcat(DataText,szSpace);
			ThingName = Things_GetThingNameFromID(ElementID,Spells);
			break;
		}

	if (!ThingName)
		{
		strcpy(DataText,szUnknown);
		return(DataText);
		}

	if (ThingName != (char *)-1)
		{
		char	*Begin;

		Begin = strchr(DataText,'\0');
		strcat(DataText,ThingName);
		if ((*((unsigned char *)Begin) >= 'A')&&(*((unsigned char *)Begin) <= 'Z'))
			*Begin = (char)(*Begin+0x20);
		}

	return(DataText);
}


// «»»» Test si une créature est féminine ou masculine ««««««««««««««««««»

BOOL Triggers_IsFemale(LONG CreatureID)
{
	int	i;

	for (i = 0; TrCreatures[i].Id != 0; i++)
		{
		if (TrCreatures[i].Id == CreatureID)
			{
			if (TrCreatures[i].Variable) return(FALSE);
			else return(TRUE);
			}
		}

	return(TRUE);
}


// «»»» Test si une salle est féminine ou masculine «««««««««««««««««««««»

BOOL Triggers_IsFemaleRoom(LONG RoomID)
{
	int	i;

	for (i = 0; TrRooms[i].Id != 0; i++)
		{
		if (TrRooms[i].Id == RoomID)
			{
			if (TrRooms[i].Variable) return(FALSE);
			else return(TRUE);
			}
		}

	return(TRUE);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
