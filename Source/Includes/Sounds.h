
/*** Identificateurs ****************************************************/

#define SOUND_PAINT		0
#define SOUND_DEFINERECT	2
#define SOUND_INTERFACE		3
#define SOUND_PAINTMODE		4

/*** Structures *********************************************************/

typedef struct MAPSOUNDS {
	IDirectSoundBuffer	*Claimed;
	IDirectSoundBuffer	*Creature;
	IDirectSoundBuffer	*Destroy;
	IDirectSoundBuffer	*Gems;
	IDirectSoundBuffer	*Gold;
	IDirectSoundBuffer	*Heart;
	IDirectSoundBuffer	*Imp;
	IDirectSoundBuffer	*Lava;
	IDirectSoundBuffer	*Room;
	IDirectSoundBuffer	*Selection;
	IDirectSoundBuffer	*Tiles;
	IDirectSoundBuffer	*Wall;
	IDirectSoundBuffer	*Water;
	IDirectSoundBuffer	*Paint;
	IDirectSoundBuffer	*Terrain;
} MAPSOUNDS;
