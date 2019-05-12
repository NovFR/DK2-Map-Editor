
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des sons.
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

extern HINSTANCE		hInstance;
extern HINSTANCE		hDirectSound;
extern IDirectSound*		MapDirectSound;
extern IDirectSoundBuffer*	MapSoundBuffer;
extern MAPSOUNDS		MapSounds;
extern MAPTHING			MapImmCreature;
extern MAPTHING			MapImmObject;
extern MAPRECT			MapCursorRect;
extern DWORD			WorldItemID;
extern DWORD			ToolsPlyrID;
extern CONFIG			Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations & restitutions					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation des sons «««««««««««««««««««««««««««««««««««««««««»

int Sound_Initialize(HWND hWnd)
{
	if (!Sound_InitializeDirectSound(hWnd)) goto Error;

	if (Config.SoundsFlags&SDF_TERRAIN)
		{
		if (!Sound_Load(szSoundClaimed,&MapSounds.Claimed)) goto Error;
		if (!Sound_Load(szSoundCreature,&MapSounds.Creature)) goto Error;
		if (!Sound_Load(szSoundDestroy,&MapSounds.Destroy)) goto Error;
		if (!Sound_Load(szSoundGems,&MapSounds.Gems)) goto Error;
		if (!Sound_Load(szSoundGold,&MapSounds.Gold)) goto Error;
		if (!Sound_Load(szSoundImp,&MapSounds.Imp)) goto Error;
		if (!Sound_Load(szSoundHeart,&MapSounds.Heart)) goto Error;
		if (!Sound_Load(szSoundLava,&MapSounds.Lava)) goto Error;
		if (!Sound_Load(szSoundRoom,&MapSounds.Room)) goto Error;
		if (!Sound_Load(szSoundSelection,&MapSounds.Selection)) goto Error;
		if (!Sound_Load(szSoundTiles,&MapSounds.Tiles)) goto Error;
		if (!Sound_Load(szSoundWall,&MapSounds.Wall)) goto Error;
		if (!Sound_Load(szSoundWater,&MapSounds.Water)) goto Error;
		}

	if (Config.SoundsFlags&SDF_INTERFACE)
		{
		if (!Sound_Load(szSoundPaint,&MapSounds.Paint)) goto Error;
		if (!Sound_Load(szSoundTerrain,&MapSounds.Terrain)) goto Error;
		}

	return(1);

Error:	Sound_Reset();
	return(0);
}


// «»»» Initialisation de DirectSound «««««««««««««««««««««««««««««««««««»

int Sound_InitializeDirectSound(HWND hWnd)
{
	typedef HRESULT	(WINAPI *DSOUNDPTR)(LPCGUID,LPDIRECTSOUND *,LPUNKNOWN);
	DSBUFFERDESC	PrimaryBuffer;
	WAVEFORMATEX	WaveFormat;
	HRESULT		Result;
	DSOUNDPTR	DirectSoundCreate;

	hDirectSound = LoadLibrary("dsound.dll");
	if (!hDirectSound) goto Error_0;
	DirectSoundCreate = (DSOUNDPTR)GetProcAddress(hDirectSound,"DirectSoundCreate");
	if (!DirectSoundCreate) goto Error_1;

	Result = DirectSoundCreate(NULL,&MapDirectSound,NULL);
	if (Result != 0) goto Error_1;

	Result = MapDirectSound->lpVtbl->SetCooperativeLevel(MapDirectSound,hWnd,DSSCL_PRIORITY);
	if (Result != 0) goto Error_2;

	ZeroMemory(&PrimaryBuffer,sizeof(DSBUFFERDESC));
	PrimaryBuffer.dwSize = sizeof(DSBUFFERDESC);
	PrimaryBuffer.dwFlags = DSBCAPS_PRIMARYBUFFER;
	Result = MapDirectSound->lpVtbl->CreateSoundBuffer(MapDirectSound,&PrimaryBuffer,&MapSoundBuffer,NULL);
	if (Result != 0) goto Error_2;

	ZeroMemory(&WaveFormat,sizeof(WAVEFORMATEX));
	WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormat.nChannels = 2;
	WaveFormat.nSamplesPerSec = 44100;
	WaveFormat.wBitsPerSample = 16;
	WaveFormat.nBlockAlign = (WORD)(WaveFormat.wBitsPerSample / 8 * WaveFormat.nChannels);
	WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec*WaveFormat.nBlockAlign;
	Result = MapSoundBuffer->lpVtbl->SetFormat(MapSoundBuffer,&WaveFormat);
	if (Result != 0) goto Error_3;

	return(1);

Error_3:MapSoundBuffer->lpVtbl->Release(MapSoundBuffer);
Error_2:MapDirectSound->lpVtbl->Release(MapDirectSound);
Error_1:FreeLibrary(hDirectSound);
Error_0:MapDirectSound = NULL;
	return(0);
}


// «»»» Initialisation des sons «««««««««««««««««««««««««««««««««««««««««»

int Sound_Load(char *Name, IDirectSoundBuffer **NameBuffer)
{
	DSBUFFERDESC		 SoundBuffer;
	HMMIO			 WaveHandle;
	MMCKINFO		 WaveChunk;
	MMCKINFO		 SubChunk;
	DWORD			 Result;
	DWORD			 WaveFormatSize;
	DWORD			 WaveDataSize;
	WAVEFORMATEX		*WaveFormat;
	DWORD			 DirectSoundData1Size;
	DWORD			 DirectSoundData2Size;
	IDirectSoundBuffer	*DirectSoundBuffer;
	void			*DirectSoundData1;
	void			*DirectSoundData2;
	char			*FileName;
	char			*NamePtr;

	//
	// Recherche le nom du fichier
	//

	FileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!FileName) goto Error_0;

	GetModuleFileName(hInstance,FileName,MAX_PATH);
	NamePtr = strrchr(FileName,'\\');
	if (NamePtr) *(NamePtr+1) = 0;
	else strcat(FileName,szPath);
	strcat(FileName,Name);

	//
	// Lecture des informations du fichier WAVE
	//

	WaveHandle = mmioOpen(FileName,NULL,MMIO_READ);
	if (!WaveHandle) goto Error_1;

	//--- Reads the wave header ---

	WaveChunk.fccType = EXTB('W','A','V','E');
	if (mmioDescend(WaveHandle,&WaveChunk,NULL,MMIO_FINDRIFF)) goto Error_2;

	ZeroMemory(&SubChunk,sizeof(MMCKINFO));
	SubChunk.fccType = EXTB('f','m','t',' ');
	if (mmioDescend(WaveHandle,&SubChunk,&WaveChunk,MMIO_FINDCHUNK)) goto Error_2;
	if (SubChunk.cksize < sizeof(WAVEFORMATEX)) WaveFormatSize = sizeof(WAVEFORMATEX);
	else WaveFormatSize = SubChunk.cksize;

	WaveFormat = GlobalAlloc(GPTR,WaveFormatSize);
	if (!WaveFormat) goto Error_2;
	Result = mmioRead(WaveHandle,(HPSTR)WaveFormat,SubChunk.cksize);
	if (Result != SubChunk.cksize) goto Error_3;
	mmioAscend(WaveHandle,&SubChunk,0);

	if (WaveFormatSize < sizeof(WAVEFORMAT)) goto Error_3;
	if (WaveFormat->wFormatTag != WAVE_FORMAT_PCM) goto Error_3;
	if (WaveFormatSize < sizeof(WAVEFORMATEX))
		{
		WaveFormat->wBitsPerSample = 8;
		WaveFormat->cbSize = 0;
		}

	//--- Gets the sound datas ---

	ZeroMemory(&SubChunk,sizeof(MMCKINFO));
	SubChunk.fccType = EXTB('d','a','t','a');
	if (mmioDescend(WaveHandle,&SubChunk,&WaveChunk,MMIO_FINDCHUNK)) goto Error_3;
	WaveDataSize = SubChunk.cksize;

	//
	// Création d'un buffer pour DirectSound
	//

	ZeroMemory(&SoundBuffer,sizeof(DSBUFFERDESC));
	SoundBuffer.dwSize = sizeof(DSBUFFERDESC);
	SoundBuffer.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_GLOBALFOCUS|DSBCAPS_STATIC|DSBCAPS_CTRLVOLUME;
	SoundBuffer.dwBufferBytes = WaveDataSize;
	SoundBuffer.lpwfxFormat = WaveFormat;
	Result = MapDirectSound->lpVtbl->CreateSoundBuffer(MapDirectSound,&SoundBuffer,&DirectSoundBuffer,NULL);
	if (Result != 0) goto Error_3;

	//
	// Lecture du fichier WAVE
	//

	Result = DirectSoundBuffer->lpVtbl->Lock(DirectSoundBuffer,0,WaveDataSize,&DirectSoundData1,&DirectSoundData1Size,&DirectSoundData2,&DirectSoundData2Size,DSBLOCK_ENTIREBUFFER);
	if (Result != 0) goto Error_4;
	Result = mmioRead(WaveHandle,DirectSoundData1,DirectSoundData1Size);
	if (Result != DirectSoundData1Size) goto Error_5;
	if (DirectSoundData2)
		{
		Result = mmioRead(WaveHandle,DirectSoundData2,DirectSoundData2Size);
		if (Result != DirectSoundData2Size) goto Error_5;
		}
	DirectSoundBuffer->lpVtbl->Unlock(DirectSoundBuffer,DirectSoundData1,DirectSoundData1Size,DirectSoundData2,DirectSoundData2Size);

	DirectSoundBuffer->lpVtbl->SetVolume(DirectSoundBuffer,Config.SoundsVolume);
	GlobalFree(WaveFormat);
	mmioClose(WaveHandle,0);
	GlobalFree(FileName);

	*NameBuffer = DirectSoundBuffer;
	return(1);

//
//--- Erreurs ---
//

Error_5:DirectSoundBuffer->lpVtbl->Unlock(DirectSoundBuffer,DirectSoundData1,DirectSoundData1Size,DirectSoundData2,DirectSoundData2Size);
Error_4:DirectSoundBuffer->lpVtbl->Release(DirectSoundBuffer);
Error_3:GlobalFree(WaveFormat);
Error_2:mmioClose(WaveHandle,0);
Error_1:GlobalFree(FileName);
Error_0:return(0);
}


// «»»» Suppression des sons ««««««««««««««««««««««««««««««««««««««««««««»

void Sound_Reset()
{
	if (MapDirectSound) MapDirectSound->lpVtbl->Release(MapDirectSound);

	MapDirectSound = NULL;
	ZeroMemory(&MapSounds,sizeof(MAPSOUNDS));
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Lecture d'un son							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Joue un son «««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Sound_Play(UINT Identifier, DWORD PaintMode)
{
	MAPCLIENT	 	 MapClient;
	IDirectSoundBuffer	*SoundToPlay = NULL;

	if (!Config.SoundsFlags) return;
	if (!MapDirectSound) return;

	switch(Identifier)
		{
		case SOUND_PAINT:
			if (!(Config.SoundsFlags&SDF_TERRAIN)) return;
			switch(PaintMode)
				{
				case PAINTMODE_CREATURE:
					if (MapImmCreature.id == 1)
						{
						SoundToPlay = MapSounds.Imp;
						break;
						}
					SoundToPlay = MapSounds.Creature;
					break;

				case PAINTMODE_DOOR:
				case PAINTMODE_TRAP:
				case PAINTMODE_MAGICALOBJECT:
					break;

				case PAINTMODE_OBJECT:
					switch(MapImmObject.id)
						{
						case 0x00000001:	// Or
						case 0x00000002:	// Sac d'or
						case 0x00000003:	// Coffre d'or
							SoundToPlay = MapSounds.Gold;
							break;
						}
					break;

				case PAINTMODE_RECT:
				case PAINTMODE_RECTFILL:
				case PAINTMODE_CIRCLE:
				case PAINTMODE_CIRCLEFILL:
				case PAINTMODE_LINE:
					switch(WorldItemID&0xFF000000)
						{
						case 0x08000000:	// Terrain réclamé
							SoundToPlay = MapSounds.Claimed;
							break;

						case 0x09000000:	// Mur
							SoundToPlay = MapSounds.Wall;
							break;

						case 0x04000000:	// Eau
							SoundToPlay = MapSounds.Water;
							break;

						case 0x05000000:	// Lave
							SoundToPlay = MapSounds.Lava;
							break;

						case 0x06000000:	// Or
						case 0x07000000:	// Gems
							SoundToPlay = MapSounds.Gems;
							break;

						case 0x01000000:	// Roche impénétrable
						case 0x02000000:	// Roche
						case 0x1E000000:	// Limite
						case 0x03000000:	// Terrain non réclamé
							SoundToPlay = MapSounds.Destroy;
							break;

						case 0x0E000000:	// Coeur du donjon
							if (ToolsPlyrID > 1)
								{
								SoundToPlay = MapSounds.Heart;
								break;
								}
						case 0x0C000000:	// Portail
						case 0x1C000000:	// Portail des héros 1
						case 0x1D000000:	// Portail des héros 2
						case 0x21000000:	// Portail des héros 3
						case 0x22000000:	// Portail des héros 4
						case 0x25000000:	// Portail des héros 6-7
						case 0x28000000:	// Portail des mercenaires
							SoundToPlay = MapSounds.Room;
							break;

						default:
							Map_CorrectRect(&MapCursorRect,&MapClient);
							if ((MapClient.Width < 2)&&(MapClient.Height < 2))
								SoundToPlay = MapSounds.Room;
							else
								SoundToPlay = MapSounds.Tiles;
							break;
						}
					break;

				case PAINTMODE_PEN:
					switch(WorldItemID&0xFF000000)
						{
						case 0x08000000:	// Terrain réclamé
							SoundToPlay = MapSounds.Claimed;
							break;

						case 0x09000000:	// Mur
							SoundToPlay = MapSounds.Wall;
							break;

						case 0x04000000:	// Eau
							SoundToPlay = MapSounds.Water;
							break;

						case 0x05000000:	// Lave
							SoundToPlay = MapSounds.Lava;
							break;

						case 0x06000000:	// Or
						case 0x07000000:	// Gems
							SoundToPlay = MapSounds.Gems;
							break;

						case 0x01000000:	// Roche impénétrable
						case 0x02000000:	// Roche
						case 0x1E000000:	// Limit
						case 0x03000000:	// Terrain non réclamé
							SoundToPlay = MapSounds.Destroy;
							break;

						case 0x0E000000:	// Coeur du donjon
							if (ToolsPlyrID > 1)
								{
								SoundToPlay = MapSounds.Heart;
								break;
								}
						default:
							SoundToPlay = MapSounds.Room;
							break;
						}
					break;

				case PAINTMODE_FILL:
					break;
				}
			break;

		case SOUND_DEFINERECT:
			if (!(Config.SoundsFlags&SDF_TERRAIN)) return;
			SoundToPlay = MapSounds.Selection;
			break;

		case SOUND_INTERFACE:
			if (!(Config.SoundsFlags&SDF_INTERFACE)) return;
			SoundToPlay = MapSounds.Terrain;
			break;

		case SOUND_PAINTMODE:
			if (!(Config.SoundsFlags&SDF_INTERFACE)) return;
			SoundToPlay = MapSounds.Paint;
			break;
		}

	if (!SoundToPlay) return;

	SoundToPlay->lpVtbl->Stop(SoundToPlay);
	SoundToPlay->lpVtbl->SetCurrentPosition(SoundToPlay,0);
	SoundToPlay->lpVtbl->Play(SoundToPlay,0,0,0);
	return;
}


// «»»» Stop tous les sons ««««««««««««««««««««««««««««««««««««««««««««««»

void Sound_Stop()
{
	if (MapSounds.Claimed)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Claimed);
	if (MapSounds.Creature)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Creature);
	if (MapSounds.Destroy)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Destroy);
	if (MapSounds.Gems)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Gems);
	if (MapSounds.Gold)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Gold);
	if (MapSounds.Heart)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Heart);
	if (MapSounds.Imp)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Imp);
	if (MapSounds.Lava)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Lava);
	if (MapSounds.Room)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Room);
	if (MapSounds.Selection)	MapSounds.Claimed->lpVtbl->Stop(MapSounds.Selection);
	if (MapSounds.Tiles)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Tiles);
	if (MapSounds.Wall)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Wall);
	if (MapSounds.Water)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Water);
	if (MapSounds.Paint)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Paint);
	if (MapSounds.Terrain)		MapSounds.Claimed->lpVtbl->Stop(MapSounds.Terrain);
	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
