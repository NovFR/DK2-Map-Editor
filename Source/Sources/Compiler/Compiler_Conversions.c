
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de conversions.
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "..\..\Includes\Editor.h"
#include "..\..\Includes\Prototypes.h"
#include "..\..\Includes\Structures.h"
#include "..\..\Includes\Texts.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Conversion de la carte de l'éditeur en carte du jeu		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

DWORD* Compiler_EditorMapToGameMap(MAPINFO *MapInfo)
{
	DWORD	*GameMap;
	DWORD	*GameMapPtr;
	BYTE	*EditorPtr;
	BYTE	*EditorLinePtr;
	DWORD	 MapFullID;
	long	 i,j;

	Compiler_SetLastError(0);
	GameMap = GlobalAlloc(GPTR,MapInfo->Width*4*MapInfo->Height);
	if (!GameMap)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		return(NULL);
		}

	EditorPtr = MapInfo->Map;
	GameMapPtr = GameMap;

	for (j = 0; j != MapInfo->Height; j++)
		{
		EditorLinePtr = EditorPtr;
		for (i = 0; i != MapInfo->Width; i++)
			{
			MapFullID = Map_GetRealIDFromMap(EditorLinePtr);
			if ((MapFullID&0xFF000000) == 0xFE000000)
				{
				MapFullID &= 0x00FFFFFF;
				MapFullID |= 0x1E000000;
				}
			*GameMapPtr++ = ((BYTE)(MapFullID>>24)) | (((BYTE)(MapFullID>>16))<<8) | (((BYTE)(MapFullID>>8))<<16) | (((BYTE)(MapFullID))<<24);
			EditorLinePtr += MAP_CONTROLBYTES;
			}
		EditorPtr += MapInfo->Width*MAP_CONTROLBYTES;
		}

	return(GameMap);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Conversion de la carte de l'éditeur en carte texte		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

char* Compiler_EditorMapToTextMap(MAPINFO *MapInfo)
{
	char	*TextMap;
	char	*TextMapPtr;
	BYTE	*EditorPtr;
	BYTE	*EditorLinePtr;
	long	 i,j;

	Compiler_SetLastError(0);
	TextMap = GlobalAlloc(GPTR,(MapInfo->Width*2+2)*MapInfo->Height+1);
	if (!TextMap)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		return(NULL);
		}

	TextMapPtr = TextMap;
	EditorPtr = MapInfo->Map;

	for (j = 0; j != MapInfo->Height; j++)
		{
		EditorLinePtr = EditorPtr;
		for (i = 0; i != MapInfo->Width; i++)
			{
			switch(*(EditorLinePtr+3))
				{
				case 0:	if (World[*EditorLinePtr].ASCIIName)
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = *World[*EditorLinePtr].ASCIIName;
						}
					else
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = 'c';
						}
					break;
				case 1:	if ((Rooms[*EditorLinePtr].Id&0xFF000000) == 0x24000000)
						{
						// Converts heroes bridges into Gate 5
						//
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = 'm';
						break;
						}
					if (Rooms[*EditorLinePtr].ASCIIName)
						{
						switch(*(EditorLinePtr+1))
							{
							case 1:	*TextMapPtr++ = 'H';
								break;
							case 3:	*TextMapPtr++ = '1';
								break;
							case 4:	*TextMapPtr++ = '2';
								break;
							case 5:	*TextMapPtr++ = '3';
								break;
							case 6:	*TextMapPtr++ = '4';
								break;
							default:*TextMapPtr++ = 'N';
								break;
							}
						*TextMapPtr++ = *Rooms[*EditorLinePtr].ASCIIName;
						}
					else
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = 'c';
						}
					break;
				case 2:	if (Gates[*EditorLinePtr].ASCIIName)
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = *Gates[*EditorLinePtr].ASCIIName;
						}
					else
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = 'c';
						}
					break;
				case 3:	if (Walls[*EditorLinePtr].ASCIIName)
						{
						*TextMapPtr++ = Walls[*EditorLinePtr].ASCIIName[0];
						*TextMapPtr++ = Walls[*EditorLinePtr].ASCIIName[1];
						}
					else
						{
						*TextMapPtr++ = 'O';
						*TextMapPtr++ = 'c';
						}
					break;
				}
			EditorLinePtr += MAP_CONTROLBYTES;
			}
		*TextMapPtr++ = 0x0D;
		*TextMapPtr++ = 0x0A;
		EditorPtr += MapInfo->Width*MAP_CONTROLBYTES;
		}

	return(TextMap);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Conversion d'une carte du jeu en carte de l'éditeur		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

MAPINFO* Compiler_GameMapToEditorMap(MAPINFO *MapInfo, NODE *Errors)
{
	MAPINFO	*EditorMapInfo;
	DWORD	*GamePtr;
	DWORD	*GameLinePtr;
	BYTE	*EditorPtr;
	BYTE	*EditorLinePtr;
	BYTE	 MapID;
	BYTE	 GameID;
	BYTE	 GamePlayer;
	BYTE	 GameOption;
	long	 i,j;

	Compiler_SetLastError(0);

	EditorMapInfo = GlobalAlloc(GPTR,sizeof(MAPINFO));
	if (!EditorMapInfo)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		return(NULL);
		}

	EditorMapInfo->Map = GlobalAlloc(GPTR,MapInfo->Width*MapInfo->Height*MAP_CONTROLBYTES);
	if (!EditorMapInfo->Map)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		GlobalFree(EditorMapInfo);
		return(NULL);
		}

	EditorMapInfo->Width = MapInfo->Width;
	EditorMapInfo->Height = MapInfo->Height;

	GamePtr = (DWORD *)(MapInfo->Map+MapInfo->RectX*4+MapInfo->RectY*MapInfo->Width*4);
	EditorPtr = EditorMapInfo->Map;

	for (j = 0; j != MapInfo->Height; j++)
		{
		EditorLinePtr = EditorPtr;
		GameLinePtr = GamePtr;

		for (i = 0; i != MapInfo->Width; i++)
			{
			GameID = *(((BYTE *)GameLinePtr)+0);
			GamePlayer = *(((BYTE *)GameLinePtr)+1);
			GameOption = *(((BYTE *)GameLinePtr)+2);

			if ((GamePlayer > 7)||(GamePlayer == 0x00))
				{
				Compiler_ReportErrorXY(Errors,REPORT_BADPLAYER,i,j,GamePlayer);
				GamePlayer = 2;
				}

			if ((GameOption != 0x01)&&(GameOption != 0x02))
				{
				Compiler_ReportErrorXY(Errors,REPORT_BADOPTION,i,j,GameOption);
				GameOption = 0x01;
				}

			if ((MapID = Map_TranslateID(GameID,GameOption,World)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(World[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(World[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x00;
				}
			else if ((MapID = Map_TranslateID(GameID,GameOption,Rooms)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = GamePlayer;
				*(EditorLinePtr+2) = (BYTE)(Rooms[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x01;
				}
			else if ((MapID = Map_TranslateID(GameID,GameOption,Gates)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(Gates[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(Gates[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x02;
				}
			else if ((MapID = Map_TranslateIDEx(GameID,GamePlayer,GameOption,Walls)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(Walls[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(Walls[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x03;
				}
			else
				{
				*(EditorLinePtr+0) = Map_TranslateID(0x02,0x01,World);
				*(EditorLinePtr+1) = 0x02;
				*(EditorLinePtr+2) = 0x01;
				*(EditorLinePtr+3) = 0x00;
				Compiler_ReportErrorXY(Errors,REPORT_BADTERRAIN,i,j,((DWORD)(GameID<<24))|((DWORD)(GamePlayer<<16))|((DWORD)(GameOption<<8))|((DWORD)0x01));
				}

			EditorLinePtr += MAP_CONTROLBYTES;
			GameLinePtr++;
			}

		GamePtr += MapInfo->Width;
		EditorPtr += MapInfo->Width*MAP_CONTROLBYTES;
		}

	return(EditorMapInfo);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Conversion d'une carte texte en carte de l'éditeur		  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

MAPINFO* Compiler_TextMapToEditorMap(MAPINFO *MapInfo, NODE *Errors)
{
	MAPINFO	*EditorMapInfo;
	BYTE	*EditorPtr;
	BYTE	*EditorLinePtr;
	char	*TextPtr;
	char	*TextLinePtr;
	DWORD	 EditorLong;
	DWORD	 EditorPlayer;
	BYTE	 MapID;
	char	 TextIdent;
	char	 TextElement;
	long	 i,j;

	Compiler_SetLastError(0);

	EditorMapInfo = GlobalAlloc(GPTR,sizeof(MAPINFO));
	if (!EditorMapInfo)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		return(NULL);
		}

	EditorMapInfo->Map = GlobalAlloc(GPTR,MapInfo->Width*MapInfo->Height*MAP_CONTROLBYTES);
	if (!EditorMapInfo->Map)
		{
		Compiler_SetLastError(COMPILERR_MAPALLOC);
		GlobalFree(EditorMapInfo);
		return(NULL);
		}

	EditorMapInfo->Width = MapInfo->Width;
	EditorMapInfo->Height = MapInfo->Height;

	TextPtr = (char *)(MapInfo->Map+MapInfo->RectX*2+MapInfo->RectY*(MapInfo->Width*2+2));
	EditorPtr = EditorMapInfo->Map;

	for (i = 0; i != MapInfo->Height; i++)
		{
		EditorLinePtr = EditorPtr;
		TextLinePtr = TextPtr;

		for (j = 0; j != MapInfo->Width; j++)
			{
			TextIdent = *TextLinePtr++;
			TextElement = *TextLinePtr++;

			switch(TextIdent)
				{
				case 'O':
					if (TextElement == 'm')
						{
						// Translates Gates 5 into Heroes bridges
						// owned by heros...
						EditorLong = Text_TranslateID(0x00,TextElement,Rooms);
						EditorLong |= 0x00010000;
						break;
						}
					if (TextElement == 'n')
						EditorLong = 0xFFFFFFFF;
					else if ((TextElement >= 'i')&&(TextElement != 'o'))
						{
						if (TextElement == 'r') TextElement = 'p';
						else if (TextElement == 's') TextElement = 'q';
						EditorLong = Text_TranslateID(0x00,TextElement,Gates);
						}
					else
						EditorLong = Text_TranslateID(0x00,TextElement,World);
					break;
				case 'H':
					EditorPlayer = 0x00010000;
					goto Rooms;
				case 'N':
					EditorPlayer = 0x00020000;
					goto Rooms;
				case '1':
					EditorPlayer = 0x00030000;
					goto Rooms;
				case '2':
					EditorPlayer = 0x00040000;
					goto Rooms;
				case '3':
					EditorPlayer = 0x00050000;
					goto Rooms;
				case '4':
					EditorPlayer = 0x00060000;
				Rooms:	if ((TextElement == 'A')||(TextElement == 'B'))
						EditorLong = Text_TranslateID(TextIdent,TextElement,Walls);
					else
						{
						EditorLong = Text_TranslateID(0x00,TextElement,Rooms);
						EditorLong |= EditorPlayer;
						}
					break;
				default:
					EditorLong = 0x02020101;
					Compiler_ReportErrorXY(Errors,REPORT_BADTXTTERRAIN,j,i,(DWORD)(TextIdent<<8)|(DWORD)(TextElement));
					break;
				}

			if ((MapID = Map_TranslateID((BYTE)(EditorLong>>24),(BYTE)(EditorLong>>8),World)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(World[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(World[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x00;
				}
			else if ((MapID = Map_TranslateID((BYTE)(EditorLong>>24),(BYTE)(EditorLong>>8),Rooms)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(EditorLong>>16);
				*(EditorLinePtr+2) = (BYTE)(Rooms[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x01;
				}
			else if ((MapID = Map_TranslateID((BYTE)(EditorLong>>24),(BYTE)(EditorLong>>8),Gates)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(Gates[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(Gates[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x02;
				}
			else if ((MapID = Map_TranslateIDEx((BYTE)(EditorLong>>24),(BYTE)(EditorLong>>16),(BYTE)(EditorLong>>8),Walls)) != (BYTE)-1)
				{
				*(EditorLinePtr+0) = MapID;
				*(EditorLinePtr+1) = (BYTE)(Walls[MapID].Id>>16);
				*(EditorLinePtr+2) = (BYTE)(Walls[MapID].Id>>8);
				*(EditorLinePtr+3) = 0x03;
				}
			else
				{
				*(EditorLinePtr+0) = Map_TranslateID(0x02,0x01,World);
				*(EditorLinePtr+1) = 0x02;
				*(EditorLinePtr+2) = 0x01;
				*(EditorLinePtr+3) = 0x00;
				Compiler_ReportErrorXY(Errors,REPORT_BADTXTTERRAIN,j,i,(DWORD)(TextIdent<<8)|(DWORD)(TextElement));
				}

			EditorLinePtr += MAP_CONTROLBYTES;
			}

		TextPtr += MapInfo->Width*2+2;
		EditorPtr += MapInfo->Width*MAP_CONTROLBYTES;
		}

	return(EditorMapInfo);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
