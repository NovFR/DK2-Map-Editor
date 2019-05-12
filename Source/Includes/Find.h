
/*** Identificateurs ****************************************************/

#define MAP_THING		0
#define MAP_RECTEX		1

/*** Structures de la recherche *****************************************/

typedef struct MAPFIND {
	UINT		 ResultsType;
	NODE		 Results;
} MAPFIND;

typedef struct MAPFINDENTRY {
	NODE		 node;
	void		*address;
	union {
	MAPRECTEX	 area;
	MAPRECTEX	 gate;
	MAPRECTEX	 effect;
	MAPTHING	 creature;
	MAPTHING	 door;
	MAPTHING	 object;
	MAPTHING	 magicalobject;
	} data;
} MAPFINDENTRY;
