
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des fichiers texte.
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
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapHerosBands;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern NODE		MapActionPoints;
extern NODE		MapTriggers;
extern NODE		MapActions;
extern MAP		Map;
extern MAPFIND		MapFind;
extern MAPEDITSTATS	MapStats;
extern MAPOPTIONS	MapOptions[MAP_MAXPLAYERS];
extern GLOBALVARS	MapGlobalVariables;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement de la carte complète					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Text_ExtractAll(HWND hWnd, char *FileName, NODE *Errors)
{
	MAP	*NewMap;
	MAPINFO	*TextMap;
	DWORD	 FileSize;
	char	*File;
	char	*Name;
	int	 i;

	Compiler_SetLastError(0);

	NewMap = GlobalAlloc(GPTR,sizeof(MAP));
	if (!NewMap)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	File = Text_ReadFile(FileName,&FileSize);
	if (!File)
		{
		GlobalFree(NewMap);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

//--- Chargement de la carte ---

	Disk_CreateWindow(DISK_TEXTLOAD);
	Disk_Update(DISK_STPRMAP);
	TextMap = Text_ExtractMap(File,FileSize,Errors);
	if (!TextMap)
		{
		GlobalFree(NewMap);
		GlobalFree(File);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Disk_DestroyWindow(FALSE);
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	// Place les valeurs par défaut
	//
	Properties_MapResetOptions(NewMap,&MapOptions[0]);
	Properties_GlobalReset(&MapGlobalVariables,-1,MGRF_ALL);
	Map_ResetStats(&MapStats);
	NewMap->Map = TextMap->Map;
	NewMap->Properties.Width = TextMap->Width;
	NewMap->Properties.Height = TextMap->Height;
	NewMap->Properties.Version = 13;
	NewMap->Properties.Options2 = 0x04|0x08;
	Map_CreateLimitWall(NewMap);
	if (!Map_CreateDisplayContext(NewMap))
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		Map_Destroy(NewMap);
		GlobalFree(NewMap);
		GlobalFree(File);
		strcpy(FileName,szLoadMapErr);
		strcat(FileName,Compiler_GetErrorCodeText());
		Disk_DestroyWindow(FALSE);
		Misc_MessageBoxExt(hWnd,FileName,NULL,szMessageBoxExtOk,MB_ICONHAND);
		return(0);
		}

	// Place le nom de la carte
	//
	Name = Text_FindHeader(File,FileSize,szTextMapHeader);
	if (Name)
		{
		while(*Name++ != 0x0A)
			{
			if (Name >= File+FileSize)
				{
				Name = NULL;
				break;
				}
			}
		if (Name)
			{
			i = 0;
			while(1)
				{
				if (*Name == 0x0D) break;
				NewMap->Properties.Name[i++] = *Name++;
				if (Name >= File+FileSize) break;
				}
			NewMap->Properties.Name[i] = 0;
			}
		}
	if (!Name)
		strcpy(NewMap->Properties.Name,szNoName);

	// Supprime l'ancienne carte
	//
	GlobalFree(TextMap);
	Map_Destroy(&Map);
	CopyMemory(&Map,NewMap,sizeof(MAP));
	GlobalFree(NewMap);
	Debug_ResetVersions(TRUE);
	Undo_DestroyAllEntries();
	List_ReleaseMemory(&MapFind.Results);
	List_ReleaseMemory(&MapCreatures);
	List_ReleaseMemory(&MapDoors);
	List_ReleaseMemory(&MapTraps);
	List_ReleaseMemory(&MapObjects);
	List_ReleaseMemory(&MapMagicalObjects);
	List_ReleaseMemory(&MapHerosBands);
	List_ReleaseMemory(&MapAreas);
	List_ReleaseMemory(&MapGates);
	List_ReleaseMemory(&MapEffects);
	Triggers_ReleaseMemory();

//--- Chargement des autres éléments ---

	Disk_Update(DISK_LTPRTHINGS);
	if (!Text_ExtractThings(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRCREATURES);
	if (!Text_ExtractCreatures(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRDOORS);
	if (!Text_ExtractDoors(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRTRAPS);
	if (!Text_ExtractTraps(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_THINGSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRINFO);
	if (!Text_ExtractInfos(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_MAINFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRBANDS);
	if (!Text_ExtractBands(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_HEROSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRAREAS);
	if (!Text_ExtractAreas(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_AREASFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRGATES);
	if (!Text_ExtractGates(File,FileSize,Errors,NULL,TRUE))
		{
		Compiler_ReportErrorText(Errors,REPORT_GATESFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRTRIGGERS);
	if (!Text_ExtractTriggers(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_TRIGGERSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRVARIABLES);
	if (!Text_ExtractVariables(File,FileSize,Errors))
		{
		Compiler_ReportErrorText(Errors,REPORT_VARSFAILURE,0);
		Compiler_ReportErrorText(Errors,REPORT_LASTERROR,0);
		}

	Disk_Update(DISK_LTPRDONE);
	Disk_DestroyWindow(FALSE);
	GlobalFree(File);
	return(1);
}


// «»»» Extraction du fichier «««««««««««««««««««««««««««««««««««««««««««»

char* Text_ReadFile(char *Filename, DWORD *Filesize)
{
	DWORD	 FileSize;
	char	*File;
	char	*FileName;
	char	*Point;

	FileName = GlobalAlloc(GPTR,MAX_PATH);
	if (!FileName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	Compiler_SetLastError(0);
	strcpy(FileName,Filename);
	Point = strrchr(FileName,'.');
	if (Point) *Point = 0;
	strcat(FileName,szTextMapExt);

	File = (char *)Files_Load(FileName,&FileSize);
	if (!File)
		{
		GlobalFree(FileName);
		return(NULL);
		}

	//--- Test les données ---

	if (strncmpi(szTextMapHeader,File,strlen(szTextMapHeader))) goto Bad_Header;

	if (Filesize) *Filesize = FileSize;
	GlobalFree(FileName);
	return(File);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	GlobalFree(FileName);
	GlobalFree(File);
	return(NULL);
}


// «»»» Recherche un identificateur «««««««««««««««««««««««««««««««««««««»

char* Text_FindHeader(char *File, DWORD FileSize, char *Header)
{
	char	*CurrentPtr;

	CurrentPtr = File;

	while(1)
		{
		if (CurrentPtr >= File+FileSize)
			return(NULL);
		if (*CurrentPtr == ' ')
			{
			CurrentPtr++;
			continue;
			}
		if (*CurrentPtr == 0x09)
			{
			CurrentPtr++;
			continue;
			}
		if (*CurrentPtr == 0x0D)
			{
			CurrentPtr++;
			continue;
			}
		if (*CurrentPtr == 0x0A)
			{
			CurrentPtr++;
			continue;
			}
		if (!strncmpi(szTextMapEOFIdent,CurrentPtr,strlen(szTextMapEOFIdent))) return(NULL);
		if (!strncmpi(Header,CurrentPtr,strlen(Header))) break;

		while(1)
			{
			if (*CurrentPtr   == 0x0A) break;
			if (*CurrentPtr++ == 0x0D) break;
			if (CurrentPtr >= File+FileSize) return(NULL);
			}
		}

	return(CurrentPtr);
}


// «»»» Copie un nom ««««««««««««««««««««««««««««««««««««««««««««««««««««»

char* Text_CompareName(char *Begin, char *FileEnd, ELEMENT *Table, DWORD *Id)
{
	int	i;

	if (!strncmpi(Begin,szTextMapEOFIdent,strlen(szTextMapEOFIdent)))
		return(NULL);

	for (i = 0; Table[i].Id != 0; i++)
		{
		if (!Table[i].ASCIIName) continue;
		if (!strncmpi(Begin,Table[i].ASCIIName,strlen(Table[i].ASCIIName)))
			{
			Begin += strlen(Table[i].ASCIIName);
			if (Begin >= FileEnd) return(NULL);
			*Id = Table[i].Id;
			return(Begin);
			}
		}

	return(NULL);
}


// «»»» Transcription d'un élément texte en élément de l'éditeur ««««««««»

DWORD Text_TranslateID(char TextIdent, char TextElement, ELEMENT *Table)
{
	int	i;

	for (i = 0; Table[i].Id != 0; i++)
		{
		if (!Table[i].ASCIIName) continue;
		if (TextIdent)
			{
			if ((*Table[i].ASCIIName == TextIdent)&&(*(Table[i].ASCIIName+1) == TextElement)) return(Table[i].Id);
			}
		else
			if (*Table[i].ASCIIName == TextElement) return(Table[i].Id);
		}

	return(0xFFFFFFFF);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement des différentes informations concernant la carte	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Extraction de la carte ««««««««««««««««««««««««««««««««««««««««««»

MAPINFO* Text_ExtractMap(char *File, DWORD FileSize, NODE *Errors)
{
	MAPINFO		*EditorMap;
	MAPINFO		 TextMapInfo;
	char		*Point;

	//--- Recherche l'identificateur MAP ---

	ZeroMemory(&TextMapInfo,sizeof(MAPINFO));

	Point = Text_FindHeader(File,FileSize,szTextMapIdent);
	if (!Point) goto Bad_Header;

	while(*Point++ != 0x0A) if (Point >= File+FileSize) goto Bad_Header;
	TextMapInfo.Width = atol(Point);
	while(*Point++ != ' ') if (Point >= File+FileSize) goto Bad_Header;
	if (*Point++ != 'X') goto Bad_Header;
	TextMapInfo.Height = atol(Point);
	while(*Point++ != 0x0A) if (Point >= File+FileSize) goto Bad_Header;

	if (Point+((TextMapInfo.Width*2)+2)*TextMapInfo.Height > File+FileSize) goto Bad_Size;

	//--- Conversion de la carte ---

	TextMapInfo.Map = (BYTE *)Point;
	EditorMap = Compiler_TextMapToEditorMap(&TextMapInfo,Errors);
	return(EditorMap);

//--- Erreurs ---

Bad_Header:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(NULL);

Bad_Size:
	Compiler_SetLastError(COMPILERR_EOF);
	return(NULL);
}


// «»»» Extraction des objets «««««««««««««««««««««««««««««««««««««««««««»

int Text_ExtractThings(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPTHING	*Thing = NULL;
	DWORD		 Id;
	long		 Count;
	char		*Things;
	char		*Temp;

	//--- Obtention du nombre d'objets ---

	Things = Text_FindHeader(File,FileSize,szTextMapThingsIdent);
	if (!Things) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
	Count = atol(Things);
	if (!Count) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;

	while (Count--)
		{
		if (Extract)
			{
			Thing = GlobalAlloc(GPTR,sizeof(MAPTHING));
			if (!Thing) goto Bad_Memory;
			}

		//--- Objet de la carte ? ---

		Temp = Text_CompareName(Things,File+FileSize,Objects,&Id);
		if (Temp)
			{
			if (Stats) Stats->Objects++;
			if (Extract)
				{
				Thing->node.type = THING_OBJECT;
				Thing->owner = 2;
				Thing->id = Id;
				while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				Thing->x = atol(--Temp);
				while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				Thing->y = atol(--Temp);
				List_AddEntry((NODE *)Thing,&MapObjects);
				Thing = NULL;
				}
			while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
			continue;
			}

		//--- Objet magique ? ---

		Temp = Text_CompareName(Things,File+FileSize,Magics,&Id);
		if (Temp)
			{
			if (Stats) Stats->MagicalObjects++;
			if (Extract)
				{
				Thing->node.type = THING_MAGICALOBJECT;
				Thing->owner = 2;
				Thing->id = Id;
				while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				Thing->x = atol(--Temp);
				while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				Thing->y = atol(--Temp);
				List_AddEntry((NODE *)Thing,&MapMagicalObjects);
				Thing = NULL;
				}
			while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
			continue;
			}

		//--- Objet non reconnu... ---

		if (Extract)
			{
			GlobalFree(Thing);
			Thing = NULL;
			Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Things);
			}

		while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Pointer:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des créatures ««««««««««««««««««««««««««««««««««««««««»

int Text_ExtractCreatures(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPTHING	*Thing = NULL;
	DWORD		 Id;
	long		 Count;
	char		*Things;
	char		*Temp;

	//--- Obtention du nombre de créatures ---

	Things = Text_FindHeader(File,FileSize,szTextMapCreaturesIdent);
	if (!Things) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
	Count = atol(Things);
	if (!Count) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->Creatures++;
	if (!Extract) return(1);

	//--- Ajout des créatures ---

	while (Count--)
		{
		Thing = GlobalAlloc(GPTR,sizeof(MAPTHING));
		if (!Thing) goto Bad_Memory;

		Temp = Text_CompareName(Things,File+FileSize,BadCreatures,&Id);
		if (Temp) goto Set_Creature;
		Temp = Text_CompareName(Things,File+FileSize,Heros,&Id);
		if (Temp) goto Set_Creature;

		//--- Créature non reconnue ---

		GlobalFree(Thing);
		Thing = NULL;
		Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Things);
		while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
		continue;

		//--- Créature reconnue ---

	Set_Creature:
		Thing->node.type = THING_CREATURE;
		Thing->id = Id;
		Thing->health = 100;
		Thing->creaturesoptions = 0x09;
		Thing->objective = 0x13;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->x = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->y = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		--Temp;
		if (*Temp++ != 'L') goto Bad_Data;
		Thing->data.level = atol(Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		--Temp;
		if (*Temp++ != 'P') goto Bad_Data;
		switch(*Temp)
			{
			case 'H':
				Thing->owner = 1;
				break;
			case 'N':
				Thing->owner = 2;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
				Thing->owner = *Temp-'1'+3;
				break;
			default:
				goto Bad_Data;
			}

		List_AddEntry((NODE *)Thing,&MapCreatures);
		Thing = NULL;
		while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
		continue;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Data:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(0);

Bad_Pointer:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des portes «««««««««««««««««««««««««««««««««««««««««««»

int Text_ExtractDoors(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPTHING	*Thing = NULL;
	DWORD		 Id;
	long		 Count;
	char		*Things;
	char		*Temp;

	//--- Obtention du nombre de portes ---

	Things = Text_FindHeader(File,FileSize,szTextMapDoorsIdent);
	if (!Things) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
	Count = atol(Things);
	if (!Count) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->Doors++;
	if (!Extract) return(1);

	//--- Ajout des portes ---

	while (Count--)
		{
		Thing = GlobalAlloc(GPTR,sizeof(MAPTHING));
		if (!Thing) goto Bad_Memory;

		//--- Recherche le type de porte ---

		Temp = Text_CompareName(Things,File+FileSize,Doors,&Id);
		if (!Temp)
			{
			GlobalFree(Thing);
			Thing = NULL;
			Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Things);
			while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
			continue;
			}

		//--- Ajout de la porte ---

		Thing->node.type = THING_DOOR;
		Thing->id = Id;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->x = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->y = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		--Temp;

		if (*Temp++ != 'P') goto Bad_Data;
		switch(*Temp)
			{
			case 'H':
				Thing->owner = 1;
				break;
			case 'N':
				Thing->owner = 2;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
				Thing->owner = *Temp-'1'+3;
				break;
			default:
				goto Bad_Data;
			}

		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		--Temp;
		if (*Temp == 'L') Thing->data.status = 1;
		List_AddEntry((NODE *)Thing,&MapDoors);

		Thing = NULL;
		while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
		continue;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Data:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(0);

Bad_Pointer:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des pièges «««««««««««««««««««««««««««««««««««««««««««»

int Text_ExtractTraps(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPTHING	*Thing = NULL;
	DWORD		 Id;
	long		 Count;
	char		*Things;
	char		*Temp;

	//--- Recherche le nombre de pièges ---

	Things = Text_FindHeader(File,FileSize,szTextMapTrapsIdent);
	if (!Things) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
	Count = atol(Things);
	if (!Count) return(1);
	while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->Traps++;
	if (!Extract) return(1);

	//--- Ajout des pièges ---

	while (Count--)
		{
		Thing = GlobalAlloc(GPTR,sizeof(MAPTHING));
		if (!Thing) goto Bad_Memory;

		//--- Recherche le type de piège ---

		Temp = Text_CompareName(Things,File+FileSize,Traps,&Id);
		if (!Temp)
			{
			GlobalFree(Thing);
			Thing = NULL;
			Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Things);
			while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
			continue;
			}

		//--- Ajout du piège ---

		Thing->node.type = THING_TRAP;
		Thing->id = Id;
		Thing->data.shots = 4;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->x = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		Thing->y = atol(--Temp);
		while(*Temp++ != ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
		--Temp;

		if (*Temp++ != 'P') goto Bad_Data;
		switch(*Temp)
			{
			case 'H':
				Thing->owner = 1;
				break;
			case 'N':
				Thing->owner = 2;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
				Thing->owner = *Temp-'1'+3;
				break;
			default:
				goto Bad_Data;
			}

		List_AddEntry((NODE *)Thing,&MapTraps);
		Thing = NULL;

		while(*Things++ != 0x0A) if (Things >= File+FileSize) goto Bad_Pointer;
		continue;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Data:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(0);

Bad_Pointer:
	if (Thing) GlobalFree(Thing);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des informations «««««««««««««««««««««««««««««««««««««»

#pragma argsused
int Text_ExtractInfos(char *File, DWORD FileSize, NODE *Error)
{
	char		*Hearts;

	Hearts = Text_FindHeader(File,FileSize,szTextMapHearts);
	if (!Hearts) return(1);

	while(*Hearts++ != 0x0A) if (Hearts >= File+FileSize) goto Bad_Pointer;

	//--- Informations concernant la position des donjons de joueurs (ignoré) ---

Loop:	if (!strncmpi(szTextMapCenterKpr1,Hearts,strlen(szTextMapCenterKpr1)))
		goto Next;
	if (!strncmpi(szTextMapCenterKpr2,Hearts,strlen(szTextMapCenterKpr2)))
		goto Next;
	if (!strncmpi(szTextMapCenterKpr3,Hearts,strlen(szTextMapCenterKpr3)))
		goto Next;
	if (!strncmpi(szTextMapCenterKpr4,Hearts,strlen(szTextMapCenterKpr4)))
		goto Next;

	//--- Informations concernant l'or des joueurs ---

	if (!strncmpi(szTextMapGoldKpr1,Hearts,strlen(szTextMapGoldKpr1)))
		{
		Hearts += strlen(szTextMapGoldKpr1);
		if (Hearts >= File+FileSize) goto Bad_Pointer;
		while(*Hearts++ == ' ') if (Hearts >= File+FileSize) goto Bad_Pointer;
		MapOptions[3-1].PlayerGold = atol(--Hearts);
		goto Next;
		}
	if (!strncmpi(szTextMapGoldKpr2,Hearts,strlen(szTextMapGoldKpr2)))
		{
		Hearts += strlen(szTextMapGoldKpr2);
		if (Hearts >= File+FileSize) goto Bad_Pointer;
		while(*Hearts++ == ' ') if (Hearts >= File+FileSize) goto Bad_Pointer;
		MapOptions[4-1].PlayerGold = atol(--Hearts);
		goto Next;
		}
	if (!strncmpi(szTextMapGoldKpr3,Hearts,strlen(szTextMapGoldKpr3)))
		{
		Hearts += strlen(szTextMapGoldKpr3);
		if (Hearts >= File+FileSize) goto Bad_Pointer;
		while(*Hearts++ == ' ') if (Hearts >= File+FileSize) goto Bad_Pointer;
		MapOptions[5-1].PlayerGold = atol(--Hearts);
		goto Next;
		}
	if (!strncmpi(szTextMapGoldKpr4,Hearts,strlen(szTextMapGoldKpr4)))
		{
		Hearts += strlen(szTextMapGoldKpr4);
		if (Hearts >= File+FileSize) goto Bad_Pointer;
		while(*Hearts++ == ' ') if (Hearts >= File+FileSize) goto Bad_Pointer;
		MapOptions[6-1].PlayerGold = atol(--Hearts);
		goto Next;
		}

	//--- Information non reconnue ---

	// Print an error here...

	//--- Prochaine information... ---

Next:	while(*Hearts++ != 0x0A) if (Hearts >= File+FileSize) goto Bad_Pointer;
	if (*Hearts != 0x0D)
		goto Loop;

	return(1);

//--- Erreurs ---

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des bandes de héros ««««««««««««««««««««««««««««««««««»

int Text_ExtractBands(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MTHBAND		*Band = NULL;
	DWORD		 Id;
	UINT		 Enemy;
	long		 Count;
	long		 HerosCount;
	char		*Bands;
	char		*Temp;
	int		 i;

	//--- Obtention du nombre de bandes ---

	Bands = Text_FindHeader(File,FileSize,szTextMapBands);
	if (!Bands) return(1);
	while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;
	Count = atol(Bands);
	if (!Count) return(1);
	while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->HerosBands++;
	if (!Extract) return(1);

	//--- Ajout des bandes ---

	while (Count--)
		{
		Band = GlobalAlloc(GPTR,sizeof(MTHBAND));
		if (!Band) goto Bad_Memory;

		if (*Bands == 0x0D)
			while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;

		//--- Obtention des infos concernant la bande ---

		if (!strncmpi(szTextMapHeros,Bands,strlen(szTextMapHeros)))
			{
			while(*Bands++ != ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			while(*Bands++ == ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			HerosCount = atol(--Bands);
			while(*Bands++ != ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			while(*Bands++ == ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			--Bands;

			if (*Bands++ != 'P') goto Bad_Data;
			switch(*Bands)
				{
				case 'H':
					Enemy = 1;
					break;
				case 'N':
					Enemy = 2;
					break;
				case '1':
				case '2':
				case '3':
				case '4':
					Enemy = *Bands-'1'+3;
					break;
				default:
					goto Bad_Data;
				}

			while(*Bands++ != ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			while(*Bands++ == ' ') if (Bands >= File+FileSize) goto Bad_Pointer;
			Band->id = atol(--Bands);
			wsprintf(Band->name,szBandID,Band->id);
			while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;

			//--- Ajout des héros dans la bande ---

			i = 0;
			while(HerosCount--)
				{
				Temp = Text_CompareName(Bands,File+FileSize,Heros,&Id);
				if (Temp)
					Band->heros[i].id = Id;
				else
					{
					Temp = Text_CompareName(Bands,File+FileSize,BadCreatures,&Id);
					if (Temp)
						Band->heros[i].id = Id;
					else
						{
						Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Bands);
						while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;
						continue;
						}
					}

				//--- Ajout d'un héro ---

				while(*Temp++ == ' ') if (Temp >= File+FileSize) goto Bad_Pointer;
				--Temp;
				if (*Temp++ != 'L') goto Bad_Data;
				Band->heros[i].level = atol(Temp);
				Band->heros[i].enemy = Enemy;
				Band->heros[i].isvalid = TRUE;
				Band->heros[i].health = 100;
				Band->heros[i].options = 0;
				Band->heros[i].objective = 0x13;
				Band->heros[i].options2 = 0x01|0x04|0x08|0x20;
				List_AddEntry((NODE *)Band,&MapHerosBands);
				while(*Bands++ != 0x0A) if (Bands >= File+FileSize) goto Bad_Pointer;
				i++;
				}

			// Le premier héro de la bande est le leader
			//
			Band->heros[0].options2 = 0x01|0x02|0x08|0x20;
			}
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Data:
	if (Band) GlobalFree(Band);
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(0);

Bad_Pointer:
	if (Band) GlobalFree(Band);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des zones ««««««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int Text_ExtractAreas(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPRECTEX	*Area = NULL;
	long		 Count;
	char		*Areas;

	//--- Obtention du nombre de zones ---

	Areas = Text_FindHeader(File,FileSize,szTextMapAreas);
	if (!Areas) return(1);
	while(*Areas++ != 0x0A) if (Areas >= File+FileSize) goto Bad_Pointer;
	Count = atol(Areas);
	if (!Count) return(1);
	while(*Areas++ != 0x0A) if (Areas >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->Areas++;
	if (!Extract) return(1);

	//--- Ajout des zones ---

	while (Count--)
		{
		Area = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
		if (!Area) goto Bad_Memory;

		Area->node.type = RECTEX_AREA;
		Area->id = atol(Areas);
		Area->rect.color = 0x00FFFF00;
		Area->rect.textcolor = 0x00FFFF00;
		Area->rect.name = Area->name;
		wsprintf(Area->name,szArea,Area->id);

		while(*Areas++ != ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		while(*Areas++ == ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		Area->rect.x1 = atol(--Areas);
		while(*Areas++ != ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		while(*Areas++ == ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		Area->rect.y1 = atol(--Areas);
		while(*Areas++ != ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		while(*Areas++ == ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		Area->rect.x2 = atol(--Areas);
		while(*Areas++ != ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		while(*Areas++ == ' ') if (Areas >= File+FileSize) goto Bad_Pointer;
		Area->rect.y2 = atol(--Areas);

		List_AddEntry((NODE *)Area,&MapAreas);
		Area = NULL;
		while(*Areas++ != 0x0A) if (Areas >= File+FileSize) goto Bad_Pointer;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Pointer:
	if (Area) GlobalFree(Area);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des passages «««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int Text_ExtractGates(char *File, DWORD FileSize, NODE *Error, MAPSTATS *Stats, BOOL Extract)
{
	MAPRECTEX	*Gate = NULL;
	long		 Count;
	char		*Gates;

	//--- Obtention du nombre de passages ---

	Gates = Text_FindHeader(File,FileSize,szTextMapGates);
	if (!Gates) return(1);
	while(*Gates++ != 0x0A) if (Gates >= File+FileSize) goto Bad_Pointer;
	Count = atol(Gates);
	if (!Count) return(1);
	while(*Gates++ != 0x0A) if (Gates >= File+FileSize) goto Bad_Pointer;

	//--- Stockage des informations ---

	if (Stats) Stats->Gates++;
	if (!Extract) return(1);

	//--- Ajout des passages ---

	while (Count--)
		{
		Gate = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
		if (!Gate) goto Bad_Memory;

		Gate->node.type = RECTEX_GATE;
		Gate->id = atol(Gates);
		Gate->rect.color = 0x00AAAAAA;
		Gate->rect.textcolor = 0x00AAAAAA;
		Gate->rect.name = Gate->name;
		wsprintf(Gate->name,szGate,Gate->id);

		while(*Gates++ != ' ') if (Gates >= File+FileSize) goto Bad_Pointer;
		while(*Gates++ == ' ') if (Gates >= File+FileSize) goto Bad_Pointer;
		Gate->rect.x1 = atol(--Gates);
		while(*Gates++ != ' ') if (Gates >= File+FileSize) goto Bad_Pointer;
		while(*Gates++ == ' ') if (Gates >= File+FileSize) goto Bad_Pointer;
		Gate->rect.y1 = atol(--Gates);
		Gate->rect.x2 = Gate->rect.x1;
		Gate->rect.y2 = Gate->rect.y1;

		List_AddEntry((NODE *)Gate,&MapGates);
		Gate = NULL;
		while(*Gates++ != 0x0A) if (Gates >= File+FileSize) goto Bad_Pointer;
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Pointer:
	if (Gate) GlobalFree(Gate);
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des évènements «««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int Text_ExtractTriggers(char *File, DWORD FileSize, NODE *Error)
{
	TRIGGERLINK	*ActionPoint;
	TRIGGER		*NewTrigger;
	TRIGGER		*PrevTrigger;
	TRIGGER		*TempTrigger;
	DWORD		 Player;
	DWORD		 Data;

	ELEMENT		*Table;
	ACTION		*NewAction;
	ACTION		*PrevAction;
	long		 ActionCount;

	short		 TriggerID = 0x0001;
	short		 ActionID  = 0xFFFF;
	char		*Triggers;
	long		 Count;
	int		 i;

	//--- Obtention du nombre d'évènements ---

	Triggers = Text_FindHeader(File,FileSize,szTextMapTriggers);
	if (!Triggers) return(1);
	while(*Triggers++ != 0x0A) if (Triggers >= File+FileSize) goto Bad_Pointer;
	Count = atol(Triggers);
	if (!Count) return(1);
Jump:	while(*Triggers++ != 0x0A) if (Triggers >= File+FileSize) goto Bad_Pointer;
	if ((*Triggers == 0x0D)||(*Triggers == 0x0A)) goto Jump;

	//--- Ajout des évènements ---

	while (Count--)
		{
		for (i = 0; ASCII_Triggers[i] != 0; i += 2)
			{
			if (!strncmpi((char *)ASCII_Triggers[i],Triggers,strlen((char *)ASCII_Triggers[i])))
				goto Set_Trigger;
			}

		Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Triggers);
		goto Bad_Data;

		//--- Ajout du déclencheur ---

	Set_Trigger:
		while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
		while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
		--Triggers;

		switch(ASCII_Triggers[i+1])
			{
			case TRIGGER_CLAIMENTIREAREA:
				if (*Triggers++ != 'P') goto Bad_Data;
				switch(*Triggers)
					{
					case 'H':	Player = 1;
							break;
					case 'N':	Player = 2;
							break;
					case '1':
					case '2':
					case '3':
					case '4':	Player = *Triggers-'1'+3;
							break;
					default:	goto Bad_Data;
					}
				while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
				while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
				Data = atol(--Triggers);
				PrevTrigger = NULL;

				for (ActionPoint = (TRIGGERLINK *)MapActionPoints.next; ActionPoint != 0; ActionPoint = (TRIGGERLINK *)ActionPoint->node.next)
					{
					if ((ActionPoint->node.type == SCOPE_AREA)&&(ActionPoint->area == Data))
						{
						PrevTrigger = Triggers_GetTriggerById(ActionPoint->trigger);
						TempTrigger = PrevTrigger;
						while(TempTrigger)
							{
							TempTrigger = Triggers_GetTriggerById(PrevTrigger->or);
							if (TempTrigger) PrevTrigger = TempTrigger;
							}
						break;
						}
					}

				NewTrigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
				if (!NewTrigger) goto Bad_Memory;
				NewTrigger->node.type = TYPE_TRIGGER;
				NewTrigger->param1 = 5;
				NewTrigger->param2 = Player;
				NewTrigger->id = TriggerID;
				NewTrigger->ctrl = TRIGGER_CLAIMENTIREAREA;
				NewTrigger->repeat = 1;

				if (!PrevTrigger)
					{
					ActionPoint = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
					if (!ActionPoint)
						{
						GlobalFree(NewTrigger);
						goto Bad_Memory;
						}
					ActionPoint->node.type = SCOPE_AREA;
					ActionPoint->area = Data;
					ActionPoint->trigger = TriggerID++;
					List_AddEntry((NODE *)ActionPoint,&MapActionPoints);
					}
				else
					{
					PrevTrigger->or = TriggerID++;
					NewTrigger->previd = PrevTrigger->id;
					}

				List_AddEntry((NODE *)NewTrigger,&MapTriggers);
				break;

			case TRIGGER_TIMEELAPSED:
				Data = atol(Triggers);
				PrevTrigger = NULL;

				for (ActionPoint = (TRIGGERLINK *)MapActionPoints.next; ActionPoint != 0; ActionPoint = (TRIGGERLINK *)ActionPoint->node.next)
					{
					if ((ActionPoint->node.type == SCOPE_PLAYER)&&(ActionPoint->player == 3))
						{
						PrevTrigger = Triggers_GetTriggerById(ActionPoint->trigger);
						TempTrigger = PrevTrigger;
						while(TempTrigger)
							{
							TempTrigger = Triggers_GetTriggerById(PrevTrigger->or);
							if (TempTrigger) PrevTrigger = TempTrigger;
							}
						break;
						}
					}

				NewTrigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
				if (!NewTrigger) goto Bad_Memory;
				NewTrigger->node.type = TYPE_TRIGGER;
				NewTrigger->param1 = 5;
				NewTrigger->param5 = Data;
				NewTrigger->id = TriggerID;
				NewTrigger->ctrl = TRIGGER_TIMEELAPSED;
				NewTrigger->repeat = 1;

				if (!PrevTrigger)
					{
					ActionPoint = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
					if (!ActionPoint)
						{
						GlobalFree(NewTrigger);
						goto Bad_Memory;
						}
					ActionPoint->node.type = SCOPE_PLAYER;
					ActionPoint->player = 3;
					ActionPoint->trigger = TriggerID++;
					List_AddEntry((NODE *)ActionPoint,&MapActionPoints);
					}
				else
					{
					PrevTrigger->or = TriggerID++;
					NewTrigger->previd = PrevTrigger->id;
					}

				List_AddEntry((NODE *)NewTrigger,&MapTriggers);
				break;

			case TRIGGER_SLAPCOUNT:
				Data = atol(Triggers);
				PrevTrigger = NULL;

				for (ActionPoint = (TRIGGERLINK *)MapActionPoints.next; ActionPoint != 0; ActionPoint = (TRIGGERLINK *)ActionPoint->node.next)
					{
					if ((ActionPoint->node.type == SCOPE_PLAYER)&&(ActionPoint->player == 3))
						{
						PrevTrigger = Triggers_GetTriggerById(ActionPoint->trigger);
						TempTrigger = PrevTrigger;
						while(TempTrigger)
							{
							TempTrigger = Triggers_GetTriggerById(PrevTrigger->or);
							if (TempTrigger) PrevTrigger = TempTrigger;
							}
						break;
						}
					}

				NewTrigger = GlobalAlloc(GPTR,sizeof(TRIGGER));
				if (!NewTrigger) goto Bad_Memory;
				NewTrigger->node.type = TYPE_TRIGGER;
				NewTrigger->param1 = 5;
				NewTrigger->param5 = Data;
				NewTrigger->id = TriggerID;
				NewTrigger->ctrl = TRIGGER_SLAPCOUNT;
				NewTrigger->repeat = 1;

				if (!PrevTrigger)
					{
					ActionPoint = GlobalAlloc(GPTR,sizeof(TRIGGERLINK));
					if (!ActionPoint)
						{
						GlobalFree(NewTrigger);
						goto Bad_Memory;
						}
					ActionPoint->node.type = SCOPE_PLAYER;
					ActionPoint->player = 3;
					ActionPoint->trigger = TriggerID++;
					List_AddEntry((NODE *)ActionPoint,&MapActionPoints);
					}
				else
					{
					PrevTrigger->or = TriggerID++;
					NewTrigger->previd = PrevTrigger->id;
					}

				List_AddEntry((NODE *)NewTrigger,&MapTriggers);
				break;
			}

		//--- Recherche les actions ---

		while(*Triggers++ != 0x0A) if (Triggers >= File+FileSize) goto Bad_Pointer;
		ActionCount = atol(Triggers);
		while(*Triggers++ != 0x0A) if (Triggers >= File+FileSize) goto Bad_Pointer;
		if (!ActionCount) continue;

		while(ActionCount--)
			{
			for (i = 0; ASCII_Actions[i] != 0; i += 3)
				{
				if (!strncmpi((char *)ASCII_Actions[i],Triggers,strlen((char *)ASCII_Actions[i])))
					goto Set_Action;
				}

			Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Triggers);
			goto Bad_Data;

			//--- Ajoute une action ---

		Set_Action:
			while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
			while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
			--Triggers;

			NewAction = GlobalAlloc(GPTR,sizeof(ACTION));
			if (!NewAction) goto Bad_Memory;
			NewAction->node.type = TYPE_ACTION;
			NewAction->id = ActionID;
			NewAction->ctrl = (BYTE)ASCII_Actions[i+1];
			NewAction->one = 0x01;

			if (NewTrigger->and)
				{
				PrevAction = Triggers_GetActionById(NewTrigger->and);
				while(1)
					{
					if (!PrevAction->next) break;
					PrevAction = Triggers_GetActionById(PrevAction->next);
					}
				PrevAction->next = ActionID--;
				NewAction->previd = PrevAction->id;
				}
			else
				NewTrigger->and = ActionID--;

			switch(NewAction->ctrl)
				{
				case ACTION_DESTROYGATE:
					NewAction->param5 = 1;
					NewAction->param5 = atol(Triggers);
					break;

				case ACTION_RELEASEBAND:
					NewAction->param1 = 0;
					NewAction->param5 = 1;
					NewAction->param1 = atol(Triggers);
					while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					Data = *(Triggers-1);
					while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					switch(Data)
						{
						case 'A':
							NewAction->param5 = atol(--Triggers);
							break;
						case 'G': {
							MAPRECTEX	*Area;
							LONG		 X,Y;
							UINT		 GateID;

							GateID = atol(--Triggers);
							X = (LONG)RectEx_GetRectLocation(&MapGates,GateID,0);
							Y = (LONG)RectEx_GetRectLocation(&MapGates,GateID,1);

							for (Area = (MAPRECTEX *)MapAreas.next; Area != 0; Area = (MAPRECTEX *)Area->node.next)
								{
								if (Area->rect.x2 < X) continue;
								if (Area->rect.y2 < Y) continue;
								if (Area->rect.x1 > X) continue;
								if (Area->rect.y1 > Y) continue;
								break;
								}

							if (Area)
								{
								NewAction->param5 = Area->id;
								break;
								}

							Area = GlobalAlloc(GPTR,sizeof(MAPRECTEX));
							if (!Area) goto Bad_Memory;
							Area->node.type = RECTEX_AREA;
							Area->id = RectEx_GetFreeID(&MapAreas);
							Area->rect.x1 = X;
							Area->rect.y1 = Y;
							Area->rect.x2 = X;
							Area->rect.y2 = Y;
							Area->rect.color = 0x000000FF;
							Area->rect.textcolor = 0x007F00FF;
							Area->rect.name = Area->name;
							Area->flags = 0x00000000;
							wsprintf(Area->name,szArea,Area->id);
							List_AddEntry((NODE *)Area,&MapAreas);
							NewAction->param5 = Area->id;
							} break;
						}
					break;

				case ACTION_ADDELEMENT:
					NewAction->param1 = 3;
					NewAction->param2 = 5;
					NewAction->param3 = 5;
					NewAction->param4 = 1;
					if (*Triggers++ != 'P') goto Bad_Data;
					switch(*Triggers)
						{
						case 'H':	NewAction->param1 = 1;
								break;
						case 'N':	NewAction->param1 = 2;
								break;
						case '1':
						case '2':
						case '3':
						case '4':	NewAction->param1 = *Triggers-'1'+3;
								break;
						default:	goto Bad_Data;
						}
					while(*Triggers++ != ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					while(*Triggers++ == ' ') if (Triggers >= File+FileSize) goto Bad_Pointer;
					--Triggers;

					NewAction->param2 = ASCII_Actions[i+2];
					switch(NewAction->param2)
						{
						case 1:	Table = VarRooms;
							break;
						case 3:	Table = Doors;
							break;
						case 4:	Table = Traps;
							break;
						case 5:	Table = Spells;
							break;
						}
					for (i = 0; Table[i].Id != 0; i++)
						{
						if (!Table[i].ASCIIName) continue;
						if (!strncmpi(Table[i].ASCIIName,Triggers,strlen(Table[i].ASCIIName))) break;
						}

					if (Table[i].Id == 0)
						Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Triggers);
					else
						{
						if (NewAction->param2 == 1)
							NewAction->param3 = Table[i].Variable;
						else
							NewAction->param3 = Table[i].Id;
						}
					break;
				}

			List_AddEntry((NODE *)NewAction,&MapActions);
		Jump2:	while(*Triggers++ != 0x0A) if (Triggers >= File+FileSize) goto Bad_Pointer;
			if ((*Triggers == 0x0D)||(*Triggers == 0x0A)) goto Jump2;
			}
		}

	return(1);

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Data:
	Compiler_SetLastError(COMPILERR_BADHEADER);
	return(0);

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// «»»» Extraction des variables ««««««««««««««««««««««««««««««««««««««««»

#pragma argsused
int Text_ExtractVariables(char *File, DWORD FileSize, NODE *Error)
{
	char		*Variables;
	long		 Player;
	long		 Count;
	int		 i,j;

	//--- Valeurs par défaut ---

	for (Count = 0; Count != MAP_MAXPLAYERS; Count++)
		{
		for (i = 1; i != 14; i++) MapOptions[Count].CreaturesCount[i] = 32;
		for (i = 0; i != 15; i++) MapOptions[Count].Rooms[i] = 3;
		for (i = 0; i != 11; i++) MapOptions[Count].Traps[i] = 3;
		for (i = 0; i != 6; i++)  MapOptions[Count].Doors[i] = 3;
		for (i = 0; i != 12; i++) MapOptions[Count].Spells[i] = 2;
		MapOptions[Count].CreaturesCount[6] = -1;
		MapOptions[Count].CreaturesCount[11] = -1;
		MapOptions[Count].Spells[0] = 3;
		MapOptions[Count].Spells[1] = 3;
		MapOptions[Count].Spells[11] = 3;
		}

	//--- Changement des valeurs ---

	Variables = Text_FindHeader(File,FileSize,szTextMapOptions);
	if (!Variables) return(1);
	while(*Variables++ != 0x0A) if (Variables >= File+FileSize) goto Bad_Pointer;

Loop:	if (!strncmpi(szTextMapEOF,Variables,strlen(szTextMapEOF)))
		return(1);

	if (!strncmpi(szTextMapNo,Variables,strlen(szTextMapNo)))
		{
		Count = 1;
		goto Set_Status;
		}
	if (!strncmpi(szTextMapMustResearch,Variables,strlen(szTextMapMustResearch)))
		{
		Count = 2;
		goto Set_Status;
		}
	if (!strncmpi(szTextMapStartWith,Variables,strlen(szTextMapStartWith)))
		{
		Count = 3;
		goto Set_Status;
		}

	if (!strncmpi(szTextMapCreatureCount,Variables,strlen(szTextMapCreatureCount)))
		goto Set_CreatureCount;

	Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Variables);
Next:	while(*Variables++ != 0x0A) if (Variables >= File+FileSize) goto Bad_Pointer;
	if ((*Variables == 0x0D)||(*Variables == 0x0A)) goto Next;
	goto Loop;

//--- Change le status des salles, portes, etc. ---

Set_Status:
	while(*Variables++ != ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	while(*Variables++ == ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	if (*(Variables-1) != 'P')
		{
		Compiler_ReportErrorText(Error,REPORT_BADPLAYER,(DWORD)Variables);
		goto Next;
		}

	switch(*Variables++)
		{
		case 'H':
			Player = 1;
			break;
		case 'N':
			Player = 2;
			break;
		case '1':
			Player = 3;
			break;
		case '2':
			Player = 4;
			break;
		case '3':
			Player = 5;
			break;
		case '4':
			Player = 6;
			break;

		default:Compiler_ReportErrorText(Error,REPORT_BADPLAYER,(DWORD)Variables);
			goto Next;
		}

	while(*Variables++ != ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	while(*Variables++ == ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	--Variables;

	for (i = 0; VarRooms[i].Id != 0; i++)
		{
		if (!VarRooms[i].ASCIIName) continue;
		if (!strncmpi(VarRooms[i].ASCIIName,Variables,strlen(VarRooms[i].ASCIIName)))
			{
			MapOptions[Player-1].Rooms[i] = (BYTE)Count;
			goto Next;
			}
		}

	for (i = 0; Traps[i].Id != 0; i++)
		{
		if (!Traps[i].ASCIIName) continue;
		if (!strncmpi(Traps[i].ASCIIName,Variables,strlen(Traps[i].ASCIIName)))
			{
			MapOptions[Player-1].Traps[i] = (BYTE)Count;
			goto Next;
			}
		}

	for (i = 0; Doors[i].Id != 0; i++)
		{
		if (!Doors[i].ASCIIName) continue;
		if (!strncmpi(Doors[i].ASCIIName,Variables,strlen(Doors[i].ASCIIName)))
			{
			MapOptions[Player-1].Doors[i] = (BYTE)Count;
			goto Next;
			}
		}

	for (i = 0; Spells[i].Id != 0; i++)
		{
		if (!Spells[i].ASCIIName) continue;
		if (!strncmpi(Spells[i].ASCIIName,Variables,strlen(Spells[i].ASCIIName)))
			{
			MapOptions[Player-1].Spells[i] = (BYTE)Count;
			goto Next;
			}
		}

	Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Variables);
	goto Next;

//--- Change le nombre de créatures sortant des portails ---

Set_CreatureCount:
	while(*Variables++ != ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	while(*Variables++ == ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	if (*(Variables-1) != 'P')
		{
		Compiler_ReportErrorText(Error,REPORT_BADPLAYER,(DWORD)Variables);
		goto Next;
		}

	switch(*Variables++)
		{
		case 'H':
			Player = 1;
			break;
		case 'N':
			Player = 2;
			break;
		case '1':
			Player = 3;
			break;
		case '2':
			Player = 4;
			break;
		case '3':
			Player = 5;
			break;
		case '4':
			Player = 6;
			break;

		default:Compiler_ReportErrorText(Error,REPORT_BADPLAYER,(DWORD)Variables);
			goto Next;
		}

	while(*Variables++ != ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	while(*Variables++ == ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	--Variables;

	for (i = 0; BadCreatures[i].Id != 0; i++)
		{
		if (!BadCreatures[i].ASCIIName) continue;
		if (!strncmpi(BadCreatures[i].ASCIIName,Variables,strlen(BadCreatures[i].ASCIIName)))
			goto Set_Count;
		}

	for (j = 0; BadElites[j].Id != 0; j++, i++);

	for (j = 0; Heros[j].Id != 0; j++, i++)
		{
		if (!Heros[j].ASCIIName) continue;
		if (!strncmpi(Heros[j].ASCIIName,Variables,strlen(Heros[j].ASCIIName)))
			goto Set_Count;
		}

	Compiler_ReportErrorText(Error,REPORT_UNKASCIIID,(DWORD)Variables);
	goto Next;

Set_Count:
	while(*Variables++ != ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	while(*Variables++ == ' ') if (Variables >= File+FileSize) goto Bad_Pointer;
	--Variables;
	MapOptions[Player-1].CreaturesCount[i] = atol(Variables);
	goto Next;

//--- Erreurs ---

Bad_Memory:
	Compiler_SetLastError(COMPILERR_WINDOWS);
	return(0);

Bad_Pointer:
	Compiler_SetLastError(COMPILERR_EOF);
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde de la carte						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int Text_GenerateFile(HWND hWnd, char *FileName)
{
	MAPINFO		 EditorMapInfo;
	char		*TextMap;
	HANDLE		 FileHandle;
	MTHBAND		*Band;
	MAPRECTEX	*Rect;
	char		*MapName;
	char		*Point;
	int		 i,j;

	MapName = GlobalAlloc(GPTR,MAX_PATH);
	if (!MapName)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		return(NULL);
		}

	Compiler_SetLastError(0);
	ZeroMemory(&EditorMapInfo,sizeof(MAPINFO));
	strcpy(MapName,FileName);
	Point = strrchr(MapName,'.');
	if (Point) *Point = 0;
	strcat(MapName,szTextMapExt);

	EditorMapInfo.Map = Map.Map;
	EditorMapInfo.Width = Map.Properties.Width;
	EditorMapInfo.Height = Map.Properties.Height;

	TextMap = Compiler_EditorMapToTextMap(&EditorMapInfo);
	if (!TextMap) goto Error_0;

//--- Création du fichier ---

	FileHandle = CreateFile(MapName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		{
		Compiler_SetLastError(COMPILERR_WINDOWS);
		goto Error_1;
		}

	Disk_CreateWindow(DISK_TEXTSAVE);

	Disk_Update(DISK_STPRHEADER);
	if (!Files_WriteData(FileHandle,szTextMapHeader,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,Map.Properties.Name,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapPlayers,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
		goto Error_2;
	if (Map.Properties.Players > 4)
		{
		if (!Files_WriteData(FileHandle,szNumber,4,DATATYPE_TEXTNUMBER))
			goto Error_2;
		}
	else
		{
		if (!Files_WriteData(FileHandle,szNumber,Map.Properties.Players,DATATYPE_TEXTNUMBER))
			goto Error_2;
		}
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

/* Inscription de la carte */

	Disk_Update(DISK_STPRMAP);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapIdent,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,Map.Properties.Width,DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapMult,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,Map.Properties.Height,DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,TextMap,0,DATATYPE_TEXT))
		goto Error_2;

/* Inscription des éléments */

	Disk_Update(DISK_STPRTHINGS);
	if (!Text_WriteThings(FileHandle,szTextMapThingsIdent,THING_OBJECT,TRUE))
		goto Error_2;
	if (!Text_WriteThings(FileHandle,szTextMapCreaturesIdent,THING_CREATURE,TRUE))
		goto Error_2;
	if (!Text_WriteThings(FileHandle,szTextMapDoorsIdent,THING_DOOR,TRUE))
		goto Error_2;
	if (!Text_WriteThings(FileHandle,szTextMapTrapsIdent,THING_TRAP,TRUE))
		goto Error_2;

/* Inscription des informations de la carte */

	Disk_Update(DISK_STPRMAPINFO);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapHearts,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	for (i = 0; i != Map.Properties.Players; i++)
		{
		if (!Files_WriteData(FileHandle,szTextMapCenter,i+1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,-1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,-1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;
		}
	for (i = 0; i != Map.Properties.Players; i++)
		{
		if (!Files_WriteData(FileHandle,szTextMapGold,i+1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,MapOptions[i+2].PlayerGold,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;
		}

/* Inscription des bandes de héros */

	Disk_Update(DISK_STPRBANDS);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapBands,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,List_EntryCount(&MapHerosBands),DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	for (Band = (MTHBAND *)MapHerosBands.next; Band != 0; Band = (MTHBAND *)Band->node.next)
		{
		char	Player[4];

		i = 0;
		while(Band->heros[i++].isvalid);
		if (!--i) continue;

		Player[1] = 0;
		switch(Band->heros[0].enemy)
			{
			case 4:	Player[0] = '2';
				break;
			case 5:	Player[0] = '3';
				break;
			case 6:	Player[0] = '4';
				break;
			default:Player[0] = '1';
				break;
			}

		if (!Files_WriteData(FileHandle,szTextMapHeros,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,i,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szTextMapPlayer,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,Player,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Band->id,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;

		i = 0;
		while(Band->heros[i].isvalid)
			{
			Point = Things_GetThingASCIIFromID(Band->heros[i].id,Heros);
			if (!Point) Point = Things_GetThingASCIIFromID(Band->heros[i].id,BadCreatures);
			if (!Point) Point = Things_GetThingASCIIFromID(Band->heros[i].id,BadElites);
			if (!Point) Point = ASCII_Creature11;
			if (!Files_WriteData(FileHandle,Point,0,DATATYPE_TEXT))
				goto Error_2;
			if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
				goto Error_2;
			if (!Files_WriteData(FileHandle,szTextMapLevel,Band->heros[i].level,DATATYPE_TEXTNUMBER))
				goto Error_2;
			if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
				goto Error_2;
			i++;
			}

		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;
		}

/* Inscription des zones */

	Disk_Update(DISK_STPRAREAS);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapAreas,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,List_EntryCount(&MapAreas),DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	for (Rect = (MAPRECTEX *)MapAreas.next; Rect != 0; Rect = (MAPRECTEX *)Rect->node.next)
		{
		if (!Files_WriteData(FileHandle,szNumber,Rect->id,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.x1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.y1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.x2,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.y2,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;
		}

/* Inscription des passages */

	Disk_Update(DISK_STPRGATES);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapGates,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,List_EntryCount(&MapGates),DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	for (Rect = (MAPRECTEX *)MapGates.next; Rect != 0; Rect = (MAPRECTEX *)Rect->node.next)
		{
		if (!Files_WriteData(FileHandle,szNumber,Rect->id,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.x1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szNumber,Rect->rect.y1,DATATYPE_TEXTNUMBER))
			goto Error_2;
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			goto Error_2;
		}

/* Inscription des évènements */

	Disk_Update(DISK_STPRTRIGGERS);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapTriggers,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szNumber,0,DATATYPE_TEXTNUMBER))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

/* Inscription des options de la carte */

	Disk_Update(DISK_STPRVARIABLES);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapOptions,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	for (i = 0; i != Map.Properties.Players+2; i++)
		{
		if (i > 5) break;
		j = 0;
		if (!Text_FillPlayerOptions(FileHandle,BadCreatures,i+1,(UINT *)&j))
			goto Error_2;
		if (!Text_FillPlayerOptions(FileHandle,BadElites,i+1,(UINT *)&j))
			goto Error_2;
		if (!Text_FillPlayerOptions(FileHandle,Heros,i+1,(UINT *)&j))
			goto Error_2;
		}

	for (i = 0; i != Map.Properties.Players+2; i++)
		{
		if (i > 5) break;
		if (!Text_FillRoomsOptions(FileHandle,VarRooms,i+1,&MapOptions[i].Rooms[0]))
			goto Error_2;
		if (!Text_FillRoomsOptions(FileHandle,Traps,i+1,&MapOptions[i].Traps[0]))
			goto Error_2;
		if (!Text_FillRoomsOptions(FileHandle,Doors,i+1,&MapOptions[i].Doors[0]))
			goto Error_2;
		if (!Text_FillRoomsOptions(FileHandle,Spells,i+1,&MapOptions[i].Spells[0]))
			goto Error_2;
		}

//--- Terminé ---

	Disk_Update(DISK_STPRDONE);
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szTextMapEOF,0,DATATYPE_TEXT))
		goto Error_2;
	if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
		goto Error_2;

	Disk_DestroyWindow(TRUE);
	CloseHandle(FileHandle);
	GlobalFree(MapName);
	GlobalFree(TextMap);
	return(1);

//--- Erreurs ---

Error_2:CloseHandle(FileHandle);
	Disk_DestroyWindow(FALSE);
	DeleteFile(MapName);
Error_1:GlobalFree(TextMap);
Error_0:strcpy(MapName,szSaveMapErr);
	strcat(MapName,Compiler_GetErrorCodeText());
	Misc_MessageBoxExt(hWnd,MapName,NULL,szMessageBoxExtOk,MB_ICONHAND|MB_OK);
	GlobalFree(MapName);
	return(0);
}


// «»»» Inscription de l'état des salles, pièges, etc. ««««««««««««««««««»

int Text_FillRoomsOptions(HANDLE FileHandle, ELEMENT *Table, UINT PlayerID, BYTE *RoomStatus)
{
	char	 Player[4];
	char	*Text;
	int	 i;

	Player[1] = 0;
	switch(PlayerID)
		{
		case 1:	Player[0] = 'H';
			break;
		case 2:	Player[0] = 'N';
			break;
		case 3:	Player[0] = '1';
			break;
		case 4:	Player[0] = '2';
			break;
		case 5:	Player[0] = '3';
			break;
		case 6:	Player[0] = '4';
			break;
		}

	for (i = 0; Table[i].Id != 0; i++)
		{
		if (!RoomStatus[i]) continue;
		if (!Table[i].ASCIIName) continue;
		// Corrige le fait qu'une alarme des héros
		// est considérée comme une alarme normale
		// dans les fichiers ASCII.
		if ((i == 12)&&(Table == Traps)) continue;

		switch(RoomStatus[i])
			{
			case 2:	Text = szTextMapMustResearch;
				break;
			case 3:	Text = szTextMapStartWith;
				break;
			default:Text = szTextMapNo;
				break;
			}

		if (!Files_WriteData(FileHandle,Text,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szTextMapPlayer,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,Player,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,Table[i].ASCIIName,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			return(0);
		}

	return(1);
}


// «»»» Inscription du nombre de créatures ««««««««««««««««««««««««««««««»

int Text_FillPlayerOptions(HANDLE FileHandle, ELEMENT *Table, UINT PlayerID, UINT *Count)
{
	char	Player[4];
	int	i;

	Player[1] = 0;
	switch(PlayerID)
		{
		case 1:	Player[0] = 'H';
			break;
		case 2:	Player[0] = 'N';
			break;
		case 3:	Player[0] = '1';
			break;
		case 4:	Player[0] = '2';
			break;
		case 5:	Player[0] = '3';
			break;
		case 6:	Player[0] = '4';
			break;
		}

	for (i = 0; Table[i].Id != 0; i++, *Count += 1)
		{
		if (MapOptions[PlayerID-1].CreaturesCount[*Count] == -1) continue;
		if (!Table[i].ASCIIName) continue;
		if (!Files_WriteData(FileHandle,szTextMapCreatureCount,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szTextMapPlayer,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,Player,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,Table[i].ASCIIName,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szNumber,MapOptions[PlayerID-1].CreaturesCount[*Count],DATATYPE_TEXTNUMBER))
			return(0);
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			return(0);
		}

	return(1);
}


// «»»» Ecriture d'une liste d'éléments «««««««««««««««««««««««««««««««««»

int Text_WriteThings(HANDLE FileHandle, char *Header, UINT Type, BOOL WriteHeader)
{
	NODE		*List;
	MAPTHING	*MapThing;
	char		*ASCIIName;
	char		 Player[4];
	long		 Count = 1;

	switch(Type)
		{
		case THING_CREATURE:
			List = &MapCreatures;
			break;
		case THING_DOOR:
			List = &MapDoors;
			break;
		case THING_TRAP:
			List = &MapTraps;
			break;
		case THING_OBJECT:
			List = &MapObjects;
			break;
		case THING_MAGICALOBJECT:
			List = &MapMagicalObjects;
			break;
		}

	if (WriteHeader)
		{
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,Header,0,DATATYPE_TEXT))
			return(0);
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			return(0);

		switch(Type)
			{
			case THING_CREATURE:
				for (Count = 0, MapThing = (MAPTHING *)List->next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
					{
					if (Things_GetThingASCIIFromID(MapThing->id,BadCreatures)) Count++;
					else if (Things_GetThingASCIIFromID(MapThing->id,BadElites)) Count++;
					else if (Things_GetThingASCIIFromID(MapThing->id,Heros)) Count++;
					}
				break;
			case THING_TRAP:
				for (Count = 0, MapThing = (MAPTHING *)List->next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
					if (Things_GetThingASCIIFromID(MapThing->id,Traps)) Count++;
				break;
			case THING_DOOR:
				for (Count = 0, MapThing = (MAPTHING *)List->next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
					if (Things_GetThingASCIIFromID(MapThing->id,Doors)) Count++;
				break;
			case THING_OBJECT:
				for (Count = 0, MapThing = (MAPTHING *)List->next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
					if (Things_GetThingASCIIFromID(MapThing->id,Objects)) Count++;
				for (MapThing = (MAPTHING *)&MapMagicalObjects.next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
					if (Things_GetThingASCIIFromID(MapThing->id,Magics)) Count++;
				break;
			}

		if (!Files_WriteData(FileHandle,szNumber,Count,DATATYPE_TEXTNUMBER))
			return(0);
		if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
			return(0);
		}

	if (Count)
		{
		for (MapThing = (MAPTHING *)List->next; MapThing != 0; MapThing = (MAPTHING *)MapThing->node.next)
			{
			switch(Type)
				{
				case THING_CREATURE:
					ASCIIName = Things_GetThingASCIIFromID(MapThing->id,BadCreatures);
					if (!ASCIIName) ASCIIName = Things_GetThingASCIIFromID(MapThing->id,BadElites);
					if (!ASCIIName) ASCIIName = Things_GetThingASCIIFromID(MapThing->id,Heros);
					break;
				case THING_DOOR:
					ASCIIName = Things_GetThingASCIIFromID(MapThing->id,Doors);
					break;
				case THING_TRAP:
					ASCIIName = Things_GetThingASCIIFromID(MapThing->id,Traps);
					break;
				case THING_OBJECT:
					ASCIIName = Things_GetThingASCIIFromID(MapThing->id,Objects);
					break;
				case THING_MAGICALOBJECT:
					ASCIIName = Things_GetThingASCIIFromID(MapThing->id,Magics);
					break;
				}
			if (!ASCIIName) continue;
			if (!Files_WriteData(FileHandle,ASCIIName,0,DATATYPE_TEXT))
				return(0);
			if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
				return(0);
			if (!Files_WriteData(FileHandle,szNumber,MapThing->x,DATATYPE_TEXTNUMBER))
				return(0);
			if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
				return(0);
			if (!Files_WriteData(FileHandle,szNumber,MapThing->y,DATATYPE_TEXTNUMBER))
				return(0);

			if (Type == THING_CREATURE)
				{
				if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
					return(0);
				if (!Files_WriteData(FileHandle,szTextMapLevel,MapThing->data.level,DATATYPE_TEXTNUMBER))
					return(0);
				}

			if ((Type != THING_OBJECT)&&(Type != THING_MAGICALOBJECT))
				{
				if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
					return(0);
				if (!Files_WriteData(FileHandle,szTextMapPlayer,0,DATATYPE_TEXT))
					return(0);
				Player[1] = 0;
				switch(MapThing->owner)
					{
					case 1:	Player[0] = 'H';
						break;
					case 3:	Player[0] = '1';
						break;
					case 4:	Player[0] = '2';
						break;
					case 5:	Player[0] = '3';
						break;
					case 6:	Player[0] = '4';
						break;
					default:Player[0] = 'N';
						break;
					}
				if (!Files_WriteData(FileHandle,Player,0,DATATYPE_TEXT))
					return(0);
				}

			if (Type == THING_DOOR)
				{
				if (!Files_WriteData(FileHandle,szSpace,0,DATATYPE_TEXT))
					return(0);
				if (MapThing->data.status == 1)
					{
					if (!Files_WriteData(FileHandle,szTextMapLocked,0,DATATYPE_TEXT))
						return(0);
					}
				else
					{
					if (!Files_WriteData(FileHandle,szTextMapUnlocked,0,DATATYPE_TEXT))
						return(0);
					}
				}

			if (!Files_WriteData(FileHandle,szCRLF,0,DATATYPE_TEXT))
				return(0);
			}
		}

	if (Type == THING_OBJECT)
		return(Text_WriteThings(FileHandle,Header,THING_MAGICALOBJECT,FALSE));

	return(1);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
