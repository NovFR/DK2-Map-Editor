
//<<>-<>>---------------------------------------------------------------------()
/*
	Sauvegarde de la carte.
	Format supportés : MAPFILE_EDITOR & MAPFILE_TEXT
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

extern HWND		hWnd;
extern HMENU		hMenu;
extern ULONG		AutoSaveTimer;
extern LONG		MapChanges;
extern LONG		MapLocked;
extern UINT		MapFileType;
extern NODE		MapCreatures;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapHerosBands;
extern MAP		Map;
extern MAPEDITSTATS	MapStats;
extern CONFIG		Config;



// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de sauvegarde						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sauvegarde automatique de la carte ««««««««««««««««««««««««««««««»

void Files_AutoSave()
{
	if (!Config.AutoSaveTime) return;
	if (AutoSaveTimer++ < Config.AutoSaveTime*60) return;
	if (MapLocked) return;
	AutoSaveTimer = 0;
	if (!MapChanges) return;

	Files_SaveMapQuick(hWnd,MAPFILE_TEMP,NULL);
	return;
}


// «»»» Sauvegarde de la carte ««««««««««««««««««««««««««««««««««««««««««»

//--- Demande le nom & le type de la carte à sauvegarder ---

void Files_SaveMap(HWND hWnd)
{
	char   *MapFileName;
	UINT	MapFileType;
	int	MapType;

	MapFileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapFileName) return;

	strcpy(MapFileName,Config.MapPath);
	MapType = Misc_FileSelectSave(hWnd,MapFileName,FILE_MAP);
	if (!MapType)
		{
		GlobalFree(MapFileName);
		return;
		}

	if (MapType == 1)
		MapFileType = MAPFILE_EDITOR;
	else
		MapFileType = MAPFILE_TEXT;

	Files_SaveMapQuick(hWnd,MapFileType,MapFileName);
	GlobalFree(MapFileName);
	return;
}

//--- Ecriture de la carte ---

void Files_SaveMapQuick(HWND hWnd, UINT FileType, char *FileName)
{
	char	*TempName;
	int	 Result = 0;

	if (!FileType) return;
	if (!Files_SaveCompatibility(hWnd,FileType)) return;

	switch(FileType)
		{
		case MAPFILE_EDITOR:
			Result = Editor_GenerateFile(hWnd,FileName,TRUE);
			break;
		case MAPFILE_TEXT:
			Result = Text_GenerateFile(hWnd,FileName);
			break;
		case MAPFILE_TEMP:
			TempName = Config_GetUserFileName(szTemporaryFolder,szTemporaryFile,TRUE);
			if (!TempName) return;
			Result = MapStats.SavedTimes;
			Editor_GenerateFile(hWnd,TempName,FALSE);
			MapStats.SavedTimes = Result;
			GlobalFree(TempName);
			return;
		}

	if (Result)
		{
		AutoSaveTimer = 0;
		Undo_DestroyAllEntries();
		strcpy(Config.MapPath,FileName);
		MapFileType = FileType;
		MapChanges = 0;
		Files_SaveUpdate();
		Map_NewTitle(hWnd);
		if (FileType != MAPFILE_TEMP)
			{
			Config_AddRecent(FileName);
			Config_UpdateRecentMenu();
			Registry_WriteRecentFiles(hWnd);
			}
		}

	return;
}


// «»»» Mise à jour des menus «««««««««««««««««««««««««««««««««««««««««««»

void Files_SaveUpdate()
{
	UINT	Enabled;

	if ((MapChanges)&&(MapFileType))
		Enabled = MF_BYCOMMAND|MF_ENABLED;
	else
		Enabled = MF_BYCOMMAND|MF_GRAYED;

	EnableMenuItem(hMenu,IDM_SAVE,Enabled);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Test les erreurs de compatibilité				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Files_SaveCompatibility(HWND hWnd, UINT MapType)
{
	NODE		 ErrorList;
	ERRORNODE	*Error;
	MAPTHING	*MapThing;
	BYTE		*MapPtr;
	BYTE		*MapLinePtr;
	char		*ErrorText;
	long		 ErrorVersion;
	long		 X,Y;
	int		 Result;

	if (!Config.SaveReportFlags) return(1);
	if (MapType == MAPFILE_TEMP) return(1);

	ZeroMemory(&ErrorList,sizeof(NODE));

//--- Ajoute les informations préalables ---

	Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
	if (!Error) goto Error;
	Error->x = -1;
	Error->text = szCompilerMapOptionsErr;
	List_AddEntry((NODE *)Error,&ErrorList);

	for (MapThing = (MAPTHING *)&MapCreatures.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
		{
		if (Things_IsHero(MapThing->id))
			{
			Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
			if (!Error) goto Error;
			Error->x = -1;
			Error->text = szCompilerHerosOptionsErr;
			List_AddEntry((NODE *)Error,&ErrorList);
			break;
			}
		}

	if (MapCreatures.next)
		{
		Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
		if (!Error) goto Error;
		Error->x = -1;
		Error->text = szCompilerCreaturesOptionsErr;
		List_AddEntry((NODE *)Error,&ErrorList);
		}

	if (MapAreas.next)
		{
		Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
		if (!Error) goto Error;
		Error->x = -1;
		Error->text = szCompilerAreasOptionsErr;
		List_AddEntry((NODE *)Error,&ErrorList);
		}

	if (MapGates.next)
		{
		Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
		if (!Error) goto Error;
		Error->x = -1;
		Error->text = szCompilerGatesOptionsErr;
		List_AddEntry((NODE *)Error,&ErrorList);
		}

	if (MapHerosBands.next)
		{
		Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
		if (!Error) goto Error;
		Error->x = -1;
		Error->text = szCompilerHerosBandsErr;
		List_AddEntry((NODE *)Error,&ErrorList);
		}

//--- Test la carte ---

	MapPtr = Map.Map;
	for (Y = 0; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		for (X = 0; X != Map.Properties.Width; X++, MapLinePtr += MAP_CONTROLBYTES)
			{
			if (Files_SaveCompReported(X,Y,&ErrorList)) continue;

			// Test si un joueur non supporté a été utilisé
			//
			if (*(MapLinePtr+1) > 6)
				{
				Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
				if (!Error) goto Error;
				Error->x = X;
				Error->y = Y;
				Files_SaveCompExpand(0x00FF0000,X,Y,&Error->width,&Error->height);
				Error->text = szCompilerPlayerNotSupportedErr;
				List_AddEntry((NODE *)Error,&ErrorList);
				continue;
				}

			ErrorText = NULL;

			// Test si un terrain non supporté a été utilisé
			//
			switch(*(MapLinePtr+3))
				{
				case 0:	switch(World[*MapLinePtr].Id)
						{
						case 0x1F020101:
							ErrorText = szCompilerClosedManaUsedErr;
							ErrorVersion = 0;
							break;
						}
					break;
				case 1:	switch(Rooms[*MapLinePtr].Id)
						{
						case 0x11000101:
						case 0x11000201:
						case 0x1A000101:
						case 0x1A000201:
							ErrorText = szCompilerBridgeErr;
							ErrorVersion = 0;
							break;
						case 0x28000101:
							ErrorText = szCompilerMercenariesPortalErr;
							ErrorVersion = 170;
							break;
						case 0x24000101:
						case 0x24000201:
							ErrorText = szCompilerGate5Err;
							ErrorVersion = 0;
							break;
						}
					break;
				case 2:	switch(Gates[*MapLinePtr].Id)
						{
						case 0x25010101:
						case 0x25010201:
							ErrorText = szCompilerGate6Err;
							ErrorVersion = 0;
							break;
						}
					break;
				}

			if (ErrorText)
				{
				Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
				if (!Error) goto Error;
				Error->x = X;
				Error->y = Y;
				Files_SaveCompExpand(0xFFFFFFFF,X,Y,&Error->width,&Error->height);
				Error->version = ErrorVersion;
				Error->text = ErrorText;
				List_AddEntry((NODE *)Error,&ErrorList);
				}

			}
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		}

//--- Test les créatures ---

	for (MapThing = (MAPTHING *)MapCreatures.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
		{
		ErrorText = NULL;

		switch(MapThing->id)
			{
			case 0x0000002D:
				ErrorText = szCompilerMaidenErr;
				ErrorVersion = 170;
				break;
			case 0x00000021:
			case 0x00000022:
			case 0x00000023:
			case 0x00000024:
			case 0x00000025:
			case 0x00000026:
			case 0x00000027:
			case 0x00000028:
			case 0x00000029:
			case 0x0000002A:
			case 0x0000002B:
				ErrorText = szCompilerEliteErr;
				ErrorVersion = 151;
				break;
			}

		if (ErrorText)
			{
			Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
			if (!Error) goto Error;
			Error->x = MapThing->x;
			Error->y = MapThing->y;
			Error->width = 1;
			Error->height = 1;
			Error->version = ErrorVersion;
			Error->text = ErrorText;
			List_AddEntry((NODE *)Error,&ErrorList);
			}
		}

//--- Test les pièges ---

	for (MapThing = (MAPTHING *)MapTraps.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
		{
		ErrorText = NULL;

		switch(MapThing->id)
			{
			case 0x0000000F:
				ErrorText = szCompilerHerosAlarmErr;
				ErrorVersion = 0;
				break;
			case 0x00000010:
				ErrorText = szCompilerJackInBoxErr;
				ErrorVersion = 170;
				break;
			}

		if (ErrorText)
			{
			Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
			if (!Error) goto Error;
			Error->x = MapThing->x;
			Error->y = MapThing->y;
			Error->width = 1;
			Error->height = 1;
			Error->version = ErrorVersion;
			Error->text = ErrorText;
			List_AddEntry((NODE *)Error,&ErrorList);
			}
		}

//--- Test les objets ---

	for (MapThing = (MAPTHING *)MapObjects.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
		{
		if (!Objects[MapThing->id].ASCIIName)
			{
			Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
			if (!Error) goto Error;
			Error->x = MapThing->x;
			Error->y = MapThing->y;
			Error->width = 1;
			Error->height = 1;
			Error->version = 0;
			Error->text = szCompilerBadObjectErr;
			List_AddEntry((NODE *)Error,&ErrorList);
			}
		switch(MapThing->id)
			{
			case 0x8A:
			case 0x8B:
			case 0x8D:
			case 0x8E:
			case 0x8F:
			case 0x90:
				Error = GlobalAlloc(GPTR,sizeof(ERRORNODE));
				if (!Error) goto Error;
				Error->x = MapThing->x;
				Error->y = MapThing->y;
				Error->width = 1;
				Error->height = 1;
				Error->version = 170;
				Error->text = szCompilerObjectErr;
				List_AddEntry((NODE *)Error,&ErrorList);
				break;
			}
		}

//--- Affichage du rapport si nécessaire ---

	Result = 1;

	if (ErrorList.next == NULL) goto Done;
	for (Error = (ERRORNODE *)ErrorList.next; Error != 0; Error = (ERRORNODE *)Error->node.next)
		{
		// Les fichiers de l'éditeur n'affichent que les problèmes de version
		//
		if (MapType == MAPFILE_EDITOR)
			{
			if ((Error->version)&&(Config.SaveReportFlags&CSR_VERSION))
				goto Show;
			}
		else
			{
			if ((*Error->text == '?')&&(Config.SaveReportFlags&CSR_INFOS))
					goto Show;
			if ((*Error->text == '#')&&(Config.SaveReportFlags&CSR_WARNINGS))
					goto Show;
			if ((*Error->text == '!')&&(Config.SaveReportFlags&CSR_ALERTS))
					goto Show;
			}
		}
	goto Done;

Show:	Result = Compiler_Report(hWnd,REPORT_COMPATIBILITY,MapType,&ErrorList);
Done:	List_ReleaseMemory(&ErrorList);
	return(Result);

//--- Erreurs ---

Error:	List_ReleaseMemory(&ErrorList);
	Misc_PrintError(hWnd,szSaveCheckErr,NULL,MB_ICONHAND);
	return(0);
}


// «»»» Prévient une fréquence trop importante de la même erreur ««««««««»

//--- Test si l'erreur a déjà été détectée ---

int Files_SaveCompReported(LONG X, LONG Y, NODE *List)
{
	ERRORNODE	*Error;

	for (Error = (ERRORNODE *)List->next; Error != 0; Error = (ERRORNODE *)Error->node.next)
		{
		if (X < Error->x) continue;
		if (Y < Error->y) continue;
		if (X >= Error->x+Error->width) continue;
		if (Y >= Error->y+Error->height) continue;
		return(1);
		}

	return(0);
}

//--- Recherche le carré dans lequel s'est produite l'erreur ---

void Files_SaveCompExpand(DWORD Mask, LONG SrcX, LONG SrcY, LONG *Width, LONG *Height)
{
	BYTE	*MapPtr;
	BYTE	*MapLinePtr;
	DWORD	 MapID;
	DWORD	 ID;
	LONG	 TmpWidth;
	LONG	 ChkWidth;
	LONG	 X,Y;

	*Width = 1;
	*Height = 1;
	ChkWidth = -1;

	MapPtr = Map.Map+SrcX*MAP_CONTROLBYTES+SrcY*Map.Properties.Width*MAP_CONTROLBYTES;
	ID =  (DWORD)((*(MapPtr+0))<<24);
	ID |= (DWORD)((*(MapPtr+1))<<16);
	ID |= (DWORD)((*(MapPtr+2))<<8);
	ID |= (DWORD)((*(MapPtr+3)));
	ID &= Mask;
	for (Y = SrcY; Y != Map.Properties.Height; Y++)
		{
		MapLinePtr = MapPtr;
		TmpWidth = 0;
		for (X = SrcX;  X != Map.Properties.Width; X++)
			{
			MapID =  (DWORD)((*(MapLinePtr+0))<<24);
			MapID |= (DWORD)((*(MapLinePtr+1))<<16);
			MapID |= (DWORD)((*(MapLinePtr+2))<<8);
			MapID |= (DWORD)((*(MapLinePtr+3)));
			MapID &= Mask;
			if (MapID != ID) break;
			TmpWidth++;
			MapLinePtr += MAP_CONTROLBYTES;
			}
		if (ChkWidth == -1) ChkWidth = TmpWidth;
		if (TmpWidth <= ChkWidth) *Width = TmpWidth;
		MapPtr += Map.Properties.Width*MAP_CONTROLBYTES;
		MapID =  (DWORD)((*(MapPtr+0))<<24);
		MapID |= (DWORD)((*(MapPtr+1))<<16);
		MapID |= (DWORD)((*(MapPtr+2))<<8);
		MapID |= (DWORD)((*(MapPtr+3)));
		MapID &= Mask;
		if (MapID != ID) break;
		*Height += 1;
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
