
#include "..\Includes\Editor.h"
#include "..\Includes\Texts.h"

/*** Table des évènements ***********************************************/

DWORD	ASCII_Triggers[] = {	(DWORD)ASCII_TriggerControlArea, 0x29,
				(DWORD)ASCII_TriggerTime,	 0x22,
				(DWORD)ASCII_TriggerCount,	 0x47,
				(DWORD)0};

DWORD	ASCII_Actions[]={	(DWORD)ASCII_ActionDestroyGate,	 0x20, 0,
				(DWORD)ASCII_ActionRelease,	 0x0E, 0,
				(DWORD)ASCII_ActionAddRoom,	 0x06, 1,
				(DWORD)ASCII_ActionAddSpell,	 0x06, 5,
				(DWORD)ASCII_ActionAddTrap,	 0x06, 4,
				(DWORD)ASCII_ActionAddDoor,	 0x06, 3,
				(DWORD)0};

