
/*** Donn�es de pr�compilation ******************************************/

#define REPORT_BADPLAYER	1
#define REPORT_BADOPTION	2
#define REPORT_BADTERRAIN	3
#define REPORT_BADTXTTERRAIN	4
#define REPORT_BADCREATURE	5
#define REPORT_BADDOOR		6
#define REPORT_BADTRAP		7
#define REPORT_BADOBJECT	8
#define REPORT_BADMAGICALOBJECT	9
#define REPORT_BADTHINGPLAYER	10
#define REPORT_BADTHINGHEADER	11
#define REPORT_BADTRIGGERID	12
#define REPORT_UNKTHINGHEADER	14
#define REPORT_THINGSFAILURE	16
#define REPORT_UNKAREAFLAGS	17
#define REPORT_BADGATETYPE	18
#define REPORT_VARSFAILURE	19
#define REPORT_BADVARIABLE	20
#define REPORT_BADVARCREATURE	21
#define REPORT_BADVARTYPE	22
#define REPORT_BADVARTHING	23
#define REPORT_MAINFAILURE	24
#define REPORT_PLAYERS		25
#define REPORT_PLAYERSFAILURE	26
#define REPORT_TRIGGERSFAILURE	27
#define REPORT_BADVARALLIE	28
#define REPORT_BADVARALLIED	29
#define REPORT_BADVARPARAM	30
#define REPORT_RECTEXFAILURE	31
#define REPORT_HEROSFAILURE	32
#define REPORT_BADSIZE		33
#define REPORT_OBSOLETEVERSION	34
#define REPORT_BADVERSION	35
#define REPORT_BADRECTEX	36
#define REPORT_BADTHING		37
#define REPORT_BADHEADER	38
#define REPORT_BADTHINGBEHAVIOR	39
#define REPORT_BADTHINGTARGET	40
#define REPORT_STATSFAILURE	41
#define REPORT_SAVEINFO		42
#define REPORT_SAVEPLAYERS	43
#define REPORT_SAVETHINGS	44
#define REPORT_SAVEMAP		45
#define REPORT_SAVEVARIABLES	46
#define REPORT_SAVETRIGGERS	47
#define REPORT_DUPLICATEDHEART	48
#define REPORT_BADHEARTSIZE	49
#define REPORT_BADPORTALSIZE	50
#define REPORT_BADGATESIZE	51
#define REPORT_NOHEART		52
#define REPORT_RESERVED		53
#define REPORT_NOPLAYER1	54
#define REPORT_NOPLAYER2	55
#define REPORT_NOPLAYER3	56
#define REPORT_NOPLAYER4	57
#define REPORT_THING61		58
#define REPORT_THING70		59
#define REPORT_TRIGGER61	60
#define REPORT_TRIGGER70	61
#define REPORT_ACTION61		62
#define REPORT_ACTION70		63
#define REPORT_CREATUREPB	64
#define REPORT_DOORPB		65
#define REPORT_TRAPPB		66
#define REPORT_OBJECTPB		67
#define REPORT_MAGICPB		68
#define REPORT_NOTASQUARE	69
#define REPORT_TRSUNUSED	70
#define REPORT_TRSMISSINGID	71
#define REPORT_TRSALREADYUSED	72
#define REPORT_ACSUNUSED	73
#define REPORT_ACSMISSINGID	74
#define REPORT_ACSALREADYUSED	75
#define REPORT_REPAIRED		76
#define REPORT_RESERVED2	77
#define REPORT_CREATURETERRAIN	78
#define REPORT_OBJECTTERRAIN	79
#define REPORT_MAGICTERRAIN	80
#define REPORT_BADVARIABLEDATA	82
#define REPORT_AREASFAILURE	83
#define REPORT_GATESFAILURE	84
#define REPORT_UNKASCIIID	85
#define REPORT_ACSNOCRYSTAL	86
#define REPORT_ACSSQOUTOFMAP	87
#define REPORT_ACSCROUTOFMAP	88
#define REPORT_ACSRMOUTOFMAP	89
#define REPORT_PETBAND		90
#define REPORT_CAMERA		91
#define REPORT_CPUAREA		92
#define REPORT_UNUSEDTRCNT	93
#define REPORT_UNUSEDACCNT	94
#define REPORT_UNUSEDREM	95
#define REPORT_UNKASCIIDATA	96
#define REPORT_MISSINGBAND	97
#define REPORT_MISSINGAREA	98
#define REPORT_MISSINGAREAPATH	99
#define REPORT_SAVEGTERRAINS	100
#define REPORT_SAVEGROOMS	101
#define REPORT_SAVEGDOORS	102
#define REPORT_SAVEGTRAPS	103
#define REPORT_SAVEGSPELLS	104
#define REPORT_GLOBALSFAILURE	105
#define REPORT_NOGATE22		106
#define REPORT_NOGATE6		107
#define REPORT_NOGATE7		108
#define REPORT_NOAREA22		109
#define REPORT_NOTDESTROYABLE	110
#define REPORT_SAVEGCREATURES	111
#define REPORT_SAVEGCRSPELLS	112
#define REPORT_NULL		255
#define REPORT_LASTERROR	256

/*** Structures *********************************************************/

typedef struct ERRORNODE {
	NODE		 node;
	LONG		 x;
	LONG		 y;
	LONG		 width;
	LONG		 height;
	LONG		 version;
	LONG		 queued;
	char		*text;
} ERRORNODE;

typedef struct ERRORCTX {
	HWND		 hlist;
	HIMAGELIST	 himagelist;
	UINT		 reportfrom;
	UINT		 type;
	DWORD		*flags;
	NODE		*list;
} ERRORCTX;
