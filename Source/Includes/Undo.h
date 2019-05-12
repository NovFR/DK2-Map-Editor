
/*** Valeurs par défaut *************************************************/

#define UNDO_LEVELS		200

/*** Identificateurs ****************************************************/

#define UNDO_FULLMAP		1
#define UNDO_MAPRECT		2
#define UNDO_THINGS		3
#define UNDO_RECTEX		4
#define UNDO_CUT		5
#define UNDO_PASTE		6
#define UNDO_SYMMETRY		7
#define UNDO_ROTATION		8
#define UNDO_MAPPROPERTIES	9
#define UNDO_MAPPARAMS		10
#define UNDO_MAPRESIZE		11
#define UNDO_PATHPOINT		12
#define UNDO_ACTIONRESIZED	13
#define UNDO_REMOVEPATHLINK	14
#define UNDO_KILLPATH		15
#define UNDO_INSERTPATHLINK	16
#define UNDO_ADDPATHLINK	17
#define UNDO_GLOBALVARIABLES	18
#define UNDO_ADDCREATURE	100
#define UNDO_SUBCREATURE	101
#define UNDO_MODCREATURE	102
#define UNDO_ADDDOOR		103
#define UNDO_SUBDOOR		104
#define UNDO_MODDOOR		105
#define UNDO_ADDTRAP		106
#define UNDO_SUBTRAP		107
#define UNDO_MODTRAP		108
#define UNDO_ADDMAGIC		109
#define UNDO_SUBMAGIC		200
#define UNDO_MODMAGIC		201
#define UNDO_ADDOBJECT		202
#define UNDO_SUBOBJECT		203
#define UNDO_MODOBJECT		204
#define UNDO_ADDBAND		205
#define UNDO_SUBBAND		206
#define UNDO_MODBAND		207
#define UNDO_ADDAREA		208
#define UNDO_SUBAREA		209
#define UNDO_MODAREA		210
#define UNDO_ADDGATE		211
#define UNDO_SUBGATE		212
#define UNDO_MODGATE		213
#define UNDO_SUBEFFECT		214
#define UNDO_MODEFFECT		215
#define UNDO_ADDEFFECT		216
#define UNDO_MOVETHING		217

/*** Structures *********************************************************/

typedef struct UNDO {
	NODE		 node;
	NODE		 childs;
	LONG		 x;
	LONG		 y;
	LONG		 width;
	LONG		 height;
	LONG		 changes;
	union {
	BYTE		*map;
	NODE		*entry;
	MAPOPTIONS	*mapoptions;
	MAPRECTEX	*area;
	GLOBALVARS	*variables;
	short		 action;
	} data;
	union {
	NODE		*list;
	NODE		*entry;
	MAPTHING	*thing;
	MTHBAND		*band;
	MAP		*map;
	UINT		 savedid;
	LONG		 dx;
	} mod;
	union {
	UINT		 currentid;
	LONG		 dy;
	} ext;
	char		*description;
} UNDO;

