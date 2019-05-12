
//<<>-<>>---------------------------------------------------------------------()
/*
	Routine d'affichage des objets (portes, pièges, etc.)
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

extern FONTINFO		Font;
extern MAP		Map;
extern MAPTHING		MapImmCreature;
extern MAPTHING		MapImmDoor;
extern MAPTHING		MapImmTrap;
extern MAPTHING		MapImmObject;
extern MAPTHING		MapImmMagicalObject;
extern NODE		MapCreatures;
extern NODE		MapDoors;
extern NODE		MapTraps;
extern NODE		MapObjects;
extern NODE		MapMagicalObjects;
extern NODE		MapAreas;
extern NODE		MapGates;
extern NODE		MapEffects;
extern LONG		MapStartX;
extern LONG		MapStartY;
extern LONG		MapZoomCX;
extern LONG		MapZoomCY;
extern LONG		MapLocked;
extern LONG		MapChanges;
extern MAPEDITSTATS	MapStats;
extern CONFIG		Config;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de modification de la carte				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Things_Paint(HWND hWnd, DWORD PaintMode, LONG MapX, LONG MapY)
{
	RECT		 MapRect;
	RECT		 UpdateRect;
	DWORD		 Owner;
	BOOL		 Create;
	UINT		 AddType;
	UINT		 ModType;
	UNDO		*Undo;
	UNDO		*Child;
	MAPTHING	*Target;
	MAPTHING	*Source;
	NODE		*NodeList;
	BYTE		*MapPtr;

	//--- Initialisations ---

	MapPtr = Map.Map+MapX*MAP_CONTROLBYTES+MapY*Map.Properties.Width*MAP_CONTROLBYTES;

	switch(PaintMode)
		{
		case PAINTMODE_CREATURE:
			Create = TRUE;
			AddType = UNDO_ADDCREATURE;
			Source = &MapImmCreature;
			NodeList = &MapCreatures;
			break;
		case PAINTMODE_DOOR:
			Create = FALSE;
			AddType = UNDO_ADDDOOR;
			ModType = UNDO_MODDOOR;
			Source = &MapImmDoor;
			NodeList = &MapDoors;
			break;
		case PAINTMODE_TRAP:
			Create = FALSE;
			AddType = UNDO_ADDTRAP;
			ModType = UNDO_MODTRAP;
			Source = &MapImmTrap;
			NodeList = &MapTraps;
			break;
		case PAINTMODE_OBJECT:
			Create = FALSE;
			AddType = UNDO_ADDOBJECT;
			ModType = UNDO_MODOBJECT;
			Source = &MapImmObject;
			NodeList = &MapObjects;
			break;
		case PAINTMODE_MAGICALOBJECT:
			Create = FALSE;
			AddType = UNDO_ADDMAGIC;
			ModType = UNDO_MODMAGIC;
			Source = &MapImmMagicalObject;
			NodeList = &MapMagicalObjects;
			break;
		}

	Owner = Things_GetOwner(MapPtr,Source->owner);

	//--- Update existing entry if exists ---

	if (!Create)
		{
		Target = Things_GetAtMapPos(NodeList,MapX,MapY);
		if (Target)
			{
			Undo_CreateEntry((NODE *)Target,ModType);

			/* Toggle lock of existing door */
			switch(PaintMode)
				{
				case PAINTMODE_DOOR:
					if ((Target->id == Source->id)&&(Target->owner == Owner))
						{
						if (Target->data.status != 0)
							Target->data.status = 0;
						else
							Target->data.status = 1;
						}
					else
						Target->data.status = Source->data.status;
					break;

				default:Target->data.all = Source->data.all;
					break;
				}

			/* Overwrite old structure with new one */
			Target->id = Source->id;
			Target->owner = Owner;
			Target->herosoptions = Source->herosoptions;
			MapChanges++;
			MapStats.UsageOfPaintThings++;
			}
		else
			Create = TRUE;
		}

	//--- Create a completely new entry ---

	if (Create)
		{
		Target = GlobalAlloc(GPTR,sizeof(MAPTHING));
		if (!Target)
			{
			MessageBeep(MB_ICONHAND);
			return;
			}
		Undo = Undo_CreateNewEntry(UNDO_THINGS,szMapPaintThing);

		/* Remove trap/door or object/magical when creating a door/trap or magical/object */
		if (PaintMode == PAINTMODE_DOOR)
			Things_RemoveAtMapPosEx(&MapTraps,MapX,MapY,&Undo);
		if (PaintMode == PAINTMODE_TRAP)
			Things_RemoveAtMapPosEx(&MapDoors,MapX,MapY,&Undo);
		if (PaintMode == PAINTMODE_OBJECT)
			Things_RemoveAtMapPosEx(&MapMagicalObjects,MapX,MapY,&Undo);
		if (PaintMode == PAINTMODE_MAGICALOBJECT)
			Things_RemoveAtMapPosEx(&MapObjects,MapX,MapY,&Undo);

		Child = Undo_CreateNewChild(&Undo,TRUE,0,NULL);
		Undo_StoreEntry(Child,(NODE *)Target,AddType);
		CopyMemory(Target,Source,sizeof(MAPTHING));
		if (Owner != 1)
			{
			Target->objective = 0;
			Target->area = 0;
			Target->ennemy = 0;
			Target->herosoptions = 0;
			}
		Target->x = MapX;
		Target->y = MapY;
		Target->owner = Owner;
		List_AddEntry((NODE *)Target,NodeList);
		MapChanges++;
		MapStats.UsageOfPaintThings++;
		}

	//--- Update map ---

	Files_SaveUpdate();
	Map_GetMapArea(hWnd,&MapRect);
	UpdateRect.left = (MapX-MapStartX)*MapZoomCX+MapRect.left;
	UpdateRect.top = (MapY-MapStartY)*MapZoomCY+MapRect.top;
	UpdateRect.right = UpdateRect.left+MapZoomCX;
	UpdateRect.bottom = UpdateRect.top+MapZoomCY;

	if (PaintMode == PAINTMODE_CREATURE)
		{
		UpdateRect.right = UpdateRect.left+Things_GetCreaturesCountWidth(MapX,MapY);
		UpdateRect.bottom = UpdateRect.top+Font.FontHeight;
		if (UpdateRect.right > MapRect.right) UpdateRect.right = MapRect.right;
		if (UpdateRect.bottom > MapRect.bottom) UpdateRect.bottom = MapRect.bottom;
		}

	Status_DisplayMapElement(MapX,MapY);

	if (UpdateRect.left > MapRect.right) return;
	if (UpdateRect.right < MapRect.left) return;
	if (UpdateRect.top > MapRect.bottom) return;
	if (UpdateRect.bottom < MapRect.top) return;

	InvalidateRect(hWnd,&UpdateRect,FALSE);
	UpdateWindow(hWnd);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage des créatures «««««««««««««««««««««««««««««««««««««««««»

void Things_DrawCreatures(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	HFONT			 OldFont;
	COLORREF		 OldTextColor;
	COLORREF		 OldBackColor;
	UINT			 OldAlignement;
	BOOL			 Expand;
	LONG			 MX,MY;
	LONG			 HerosCount;
	LONG			 NeutralCount;
	LONG			 Keeper1Count;
	LONG			 Keeper2Count;
	LONG			 Keeper3Count;
	LONG			 Keeper4Count;
	LONG			 Keeper5Count;
	register MAPTHING	*Creatures;
	register LONG		 X,Y;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_CREATURES)) return;
	if (!Map.Map) return;

	OldFont = SelectObject(hDC,Font.hFont);
	if (!OldFont) return;
	OldTextColor = SetTextColor(hDC,0x00000000);
	OldBackColor = SetBkColor(hDC,0x00FFFFFF);
	OldAlignement = SetTextAlign(hDC,TA_UPDATECP);
	Expand = (Config.MapViewFlags&MVF_DETAILED);
//	if (Font.FontHeight*2 > MapZoomCY) Expand = FALSE;

	for (Y = MapStartY; Y != MapStartY+DY; Y++)
		for (X = MapStartX; X != MapStartX+DX; X++)
			{
			HerosCount = 0;
			NeutralCount = 0;
			Keeper1Count = 0;
			Keeper2Count = 0;
			Keeper3Count = 0;
			Keeper4Count = 0;
			Keeper5Count = 0;
			for (Creatures = (MAPTHING *)MapCreatures.next; Creatures != 0; Creatures = (MAPTHING *)Creatures->node.next)
				{
				if ((Creatures->x == X)&&(Creatures->y == Y))
					{
					switch(Creatures->owner)
						{
						case 1:	NeutralCount++;
							break;
						case 2:	HerosCount++;
							break;
						case 3:	Keeper1Count++;
							break;
						case 4:	Keeper2Count++;
							break;
						case 5:	Keeper3Count++;
							break;
						case 6:	Keeper4Count++;
							break;
						case 7:	Keeper5Count++;
							break;
						}
					}
				}

			if (!NeutralCount && !HerosCount && !Keeper1Count && !Keeper2Count && !Keeper3Count && !Keeper4Count && !Keeper5Count)
				continue;

			MX = (X-MapStartX)*MapZoomCX+MapRect->left;
			MY = (Y-MapStartY)*MapZoomCY+MapRect->top;

			if (Things_IsSolid(Map.Map+X*MAP_CONTROLBYTES+Y*Map.Properties.Width*MAP_CONTROLBYTES))
				{
				MoveToEx(hDC,MX,MY,NULL);
				SetTextColor(hDC,0x00FFFFFF);
				SetBkColor(hDC,0x000000FF);
				TextOut(hDC,0,0,szExclamation,3);
				SetBkColor(hDC,0x00FFFFFF);
				SetTextColor(hDC,0x00000000);
				continue;
				}

			if (Expand)
				{
				MoveToEx(hDC,MX,MY,NULL);
				Things_DrawCreaturesNumber(hDC,NeutralCount,Players[0].Color);
				Things_DrawCreaturesNumber(hDC,HerosCount,Players[1].Color);
				Things_DrawCreaturesNumber(hDC,Keeper1Count,Players[2].Color);
				Things_DrawCreaturesNumber(hDC,Keeper2Count,Players[3].Color);
				Things_DrawCreaturesNumber(hDC,Keeper3Count,Players[4].Color);
				Things_DrawCreaturesNumber(hDC,Keeper4Count,Players[5].Color);
				Things_DrawCreaturesNumber(hDC,Keeper5Count,Players[6].Color);
				}
			else
				{
				MoveToEx(hDC,MX,MY,NULL);
				Things_DrawCreaturesNumber(hDC,NeutralCount+HerosCount+Keeper1Count+Keeper2Count+Keeper3Count+Keeper4Count+Keeper5Count,0x00FFFFFF);
				}

			}

	SetTextAlign(hDC,OldAlignement);
	SetBkColor(hDC,OldBackColor);
	SetTextColor(hDC,OldTextColor);
	SelectObject(hDC,OldFont);
	return;
}

//--- Affichage du numéro avec la couleur de la créature ---

void Things_DrawCreaturesNumber(HDC hDC, LONG Number, COLORREF Color)
{
	char ASCIINumber[32];

	wsprintf(ASCIINumber,szNumber,Number);
	if (!Number) return;
	SetBkColor(hDC,Color);
	TextOut(hDC,0,0,ASCIINumber,strlen(ASCIINumber));
	return;
}


// «»»» Affichage des portes ««««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawDoors(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	register MAPTHING	*Doors;
	register RECT		 ClientRect;
	HBRUSH			 PlayerBrush,OldBrush;
	BYTE			*MapPtrU;
	BYTE			*MapPtrD;
	BYTE			*MapPtrL;
	BYTE			*MapPtrR;
	BYTE			*MapPtr;
	BOOL			 HorzDoor;
	BOOL			 DoorNoWall;
	LONG			 FX1,FY1;
	LONG			 FX2,FY2;
	LONG			 FX3,FY3;
	LONG			 MX,MY;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_DOORS)) return;
	if (!Map.Map) return;

	FX1 = (int)(((float)MapZoomCX*40.0f/100.0f)/2);
	FY1 = (int)(((float)MapZoomCY*40.0f/100.0f)/2);
	FX2 = (int)(((float)MapZoomCX*60.0f/100.0f)/2);
	FY2 = (int)(((float)MapZoomCY*60.0f/100.0f)/2);
	FX3 = (int)(((float)MapZoomCX*80.0f/100.0f)/2);
	FY3 = (int)(((float)MapZoomCY*80.0f/100.0f)/2);

	for (Doors = (MAPTHING *)MapDoors.next; Doors != 0; Doors = (MAPTHING *)Doors->node.next)
		{
		DoorNoWall = FALSE;

		//--- Test les coordonnées de la porte ---

		if (MapStartX > Doors->x) continue;
		if (MapStartY > Doors->y) continue;
		if (MapStartX+DX <= Doors->x) continue;
		if (MapStartY+DY <= Doors->y) continue;
		MX = (Doors->x-MapStartX)*MapZoomCX+MapRect->left;
		MY = (Doors->y-MapStartY)*MapZoomCY+MapRect->top;

		MapPtrU = Map.Map+Doors->x*MAP_CONTROLBYTES+(Doors->y-1)*Map.Properties.Width*MAP_CONTROLBYTES;
		MapPtrD = Map.Map+Doors->x*MAP_CONTROLBYTES+(Doors->y+1)*Map.Properties.Width*MAP_CONTROLBYTES;
		MapPtrL = Map.Map+(Doors->x-1)*MAP_CONTROLBYTES+Doors->y*Map.Properties.Width*MAP_CONTROLBYTES;
		MapPtrR = Map.Map+(Doors->x+1)*MAP_CONTROLBYTES+Doors->y*Map.Properties.Width*MAP_CONTROLBYTES;

		if ( (Things_IsSolid(MapPtrL)) && (Things_IsSolid(MapPtrR)) )
			HorzDoor = TRUE;
		else
			{
			if ( (!Things_IsSolid(MapPtrU)) || (!Things_IsSolid(MapPtrD)) )
				{
				DoorNoWall = TRUE;
				goto Set_Door;
				}
			HorzDoor = FALSE;
			}

		//--- Affichage de la porte ---

	Set_Door:
		MapPtr = Map.Map+Doors->x*MAP_CONTROLBYTES+Doors->y*Map.Properties.Width*MAP_CONTROLBYTES;
		PlayerBrush = CreateSolidBrush(Players[Doors->owner-1].Color);
		if (!PlayerBrush) continue;
		OldBrush = SelectObject(hDC,PlayerBrush);
		if (!OldBrush) {DeleteObject(PlayerBrush); continue;}

		if (Things_IsSolid(MapPtr))
			goto No_Wall;
		if (*(MapPtr+1) != Doors->owner)
			goto No_Wall;

		/* La porte est une barrière (gros carré) */
		if ((Doors->id == 0x00000006)||(Doors->id == 0x00000007))
			{
			ClientRect.left = MX+FX1;
			ClientRect.right = MX+MapZoomCX-FX1;
			ClientRect.top = MY+FY1;
			ClientRect.bottom = MY+MapZoomCY-FY1;
			goto Draw_Door;
			}

		/* La porte n'est pas correctement placée */
		if (DoorNoWall)
			{
		No_Wall:ClientRect.left = MX+FX3;
			ClientRect.right = MX+MapZoomCX-FX3;
			ClientRect.top = MY+FY3;
			ClientRect.bottom = MY+MapZoomCY-FY3;
			goto Draw_Door;
			}

		/* Dimensions de la porte */
		if (!HorzDoor)
			{
			ClientRect.left = MX+FX2;
			ClientRect.right = MX+MapZoomCX-FX2;
			ClientRect.top = MY;
			ClientRect.bottom = MY+MapZoomCY;
			}
		else
			{
			ClientRect.left = MX;
			ClientRect.right = MX+MapZoomCX;
			ClientRect.top = MY+FY2;
			ClientRect.bottom = MY+MapZoomCY-FY2;
			}

		/* La porte est fermée */
		if (Doors->data.status == 1)
			{
			HPEN hPen,OldPen;

			FillRect(hDC,&ClientRect,PlayerBrush);
			SelectObject(hDC,OldBrush);
			DeleteObject(PlayerBrush);
			ClientRect.left = MX+FX2;
			ClientRect.right = MX+MapZoomCX-FX2;
			ClientRect.top = MY+FY2;
			ClientRect.bottom = MY+MapZoomCY-FY2;
			hPen = CreatePen(PS_SOLID,1,0x00000000);
			if (hPen)
				{
				OldPen = SelectObject(hDC,hPen);
				if (OldPen)
					{
					MoveToEx(hDC,ClientRect.left,ClientRect.top,NULL);
					LineTo(hDC,ClientRect.right,ClientRect.bottom);
					MoveToEx(hDC,ClientRect.right,ClientRect.top,NULL);
					LineTo(hDC,ClientRect.left,ClientRect.bottom);
					SelectObject(hDC,OldPen);
					}
				DeleteObject(hPen);
				}
			continue;
			}

		/* C'est une esquisse */
	Draw_Door:
                if (Doors->data.status == 2)
			{
			HPEN	hPen,OldPen;

			SelectObject(hDC,OldBrush);
			DeleteObject(PlayerBrush);
			hPen = CreatePen(PS_SOLID,1,Players[Doors->owner-1].Color);
			if (hPen)
				{
				OldPen = SelectObject(hDC,hPen);
				if (OldPen)
					{
					MoveToEx(hDC,ClientRect.left,ClientRect.top,NULL);
					LineTo(hDC,ClientRect.right-1,ClientRect.top);
					LineTo(hDC,ClientRect.right-1,ClientRect.bottom-1);
					LineTo(hDC,ClientRect.left,ClientRect.bottom-1);
					LineTo(hDC,ClientRect.left,ClientRect.top);
					MoveToEx(hDC,ClientRect.left+1,ClientRect.top+1,NULL);
					LineTo(hDC,ClientRect.right-2,ClientRect.top+1);
					LineTo(hDC,ClientRect.right-2,ClientRect.bottom-2);
					LineTo(hDC,ClientRect.left+1,ClientRect.bottom-2);
					LineTo(hDC,ClientRect.left+1,ClientRect.top+1);
					SelectObject(hDC,OldPen);
					}
				DeleteObject(hPen);
				}
			continue;
			}

		/* Affichage d'une porte ouverte, d'une barrière ou d'une porte invalide */
		FillRect(hDC,&ClientRect,PlayerBrush);
		SelectObject(hDC,OldBrush);
		DeleteObject(PlayerBrush);
		continue;
		}

	return;
}


// «»»» Affichage des pièges ««««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawTraps(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	register RECT	 ClientRect;
	register MAPTHING	*Traps;
	register BYTE	*MapPtr;
	HBRUSH		 PlayerBrush,OldBrush;
	HPEN			 PaintPen,OldPen;
	LONG			 FX1,FY1;
	LONG			 FX3,FY3;
	LONG			 MX,MY;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_TRAPS)) return;
	if (!Map.Map) return;

	FX1 = (int)(((float)MapZoomCX*40.0f/100.0f)/2);
	FY1 = (int)(((float)MapZoomCY*40.0f/100.0f)/2);
	FX3 = (int)(((float)MapZoomCX*80.0f/100.0f)/2);
	FY3 = (int)(((float)MapZoomCY*80.0f/100.0f)/2);
	PaintPen = CreatePen(PS_SOLID,1,0x00000000);
	if (PaintPen)
		{
		OldPen = SelectObject(hDC,PaintPen);
		if (OldPen)
			{
			for (Traps = (MAPTHING *)MapTraps.next; Traps != 0; Traps = (MAPTHING *)Traps->node.next)
				{

				//--- Test les coordonnées du piège ---

				if (MapStartX > Traps->x) continue;
				if (MapStartY > Traps->y) continue;
				if (MapStartX+DX <= Traps->x) continue;
				if (MapStartY+DY <= Traps->y) continue;
				MX = (Traps->x-MapStartX)*MapZoomCX+MapRect->left;
				MY = (Traps->y-MapStartY)*MapZoomCY+MapRect->top;

				//--- Affichage du piège ---

				MapPtr = Map.Map+Traps->x*MAP_CONTROLBYTES+Traps->y*Map.Properties.Width*MAP_CONTROLBYTES;
				PlayerBrush = CreateSolidBrush(Players[Traps->owner-1].Color);
				if (!PlayerBrush) continue;
				OldBrush = SelectObject(hDC,PlayerBrush);
				if (!OldBrush) {DeleteObject(PlayerBrush); continue;}

				if (Things_IsSolid(MapPtr))
					goto Bad;
				if (*(MapPtr+1) != Traps->owner)
					goto Bad;
				if (Traps->data.shots != 0)
					goto Normal;

				/* Esquisse de piège */
				ClientRect.left = MX+FX1;
				ClientRect.right = MX+MapZoomCX-FX1;
				ClientRect.top = MY+FY1;
				ClientRect.bottom = MY+MapZoomCY-FY1;
				Chord(hDC,ClientRect.left,ClientRect.top,ClientRect.right,ClientRect.bottom,ClientRect.right,ClientRect.top,ClientRect.left,ClientRect.bottom);
				SelectObject(hDC,OldBrush);
				DeleteObject(PlayerBrush);
				continue;

				/* Piège normal */
			Normal:	ClientRect.left = MX+FX1;
				ClientRect.right = MX+MapZoomCX-FX1;
				ClientRect.top = MY+FY1;
				ClientRect.bottom = MY+MapZoomCY-FY1;
				goto Next;

				/* Piège incorrectement placé */
			Bad:	ClientRect.left = MX+FX3;
				ClientRect.right = MX+MapZoomCX-FX3;
				ClientRect.top = MY+FY3;
				ClientRect.bottom = MY+MapZoomCY-FY3;

				/* Routine commune */
			Next:	Ellipse(hDC,ClientRect.left,ClientRect.top,ClientRect.right,ClientRect.bottom);
				SelectObject(hDC,OldBrush);
				DeleteObject(PlayerBrush);
				continue;
				}

			SelectObject(hDC,OldPen);
			}
		DeleteObject(PaintPen);
		}

	return;
}


// «»»» Affichage des objets ««««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawObjects(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	register RECT	 ClientRect;
	register MAPTHING	*Objects;
	HBRUSH		 Brush,OldBrush;
	LONG			 FX3,FY3;
	LONG			 MX,MY;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_OBJECTS)) return;
	if (!Map.Map) return;

	FX3 = (int)((float)MapZoomCX*60.0f/100.0f);
	FY3 = (int)((float)MapZoomCY*60.0f/100.0f);

	for (Objects = (MAPTHING *)MapObjects.next; Objects != 0; Objects = (MAPTHING *)Objects->node.next)
		{
		if (MapStartX > Objects->x) continue;
		if (MapStartY > Objects->y) continue;
		if (MapStartX+DX <= Objects->x) continue;
		if (MapStartY+DY <= Objects->y) continue;
		MX = (Objects->x-MapStartX)*MapZoomCX+MapRect->left;
		MY = (Objects->y-MapStartY)*MapZoomCY+MapRect->top;

		if (Things_IsSolid(Map.Map+Objects->x*MAP_CONTROLBYTES+Objects->y*Map.Properties.Width*MAP_CONTROLBYTES))
			goto Bad;

		if (Config.MapViewFlags&MVF_OBJECTUSESPLYCOL)
			Brush = CreateSolidBrush(Players[Objects->owner-1].Color);
		else
			Brush = CreateSolidBrush(0x005555FF);
		if (Brush)
			{
			OldBrush = SelectObject(hDC,Brush);
			if (OldBrush)
				{
				ClientRect.left = MX+FX3;
				ClientRect.right = MX+MapZoomCX;
				ClientRect.top = MY;
				ClientRect.bottom = MY+MapZoomCY-FY3;
				FillRect(hDC,&ClientRect,Brush);
				SelectObject(hDC,OldBrush);
				}
			DeleteObject(Brush);
			}
		continue;

	Bad:	ClientRect.left = MX+FX3;
		ClientRect.right = MX+MapZoomCX;
		ClientRect.top = MY;
		ClientRect.bottom = MY+MapZoomCY-FY3;
		FillRect(hDC,&ClientRect,GetStockObject(BLACK_BRUSH));
		continue;
		}

	return;
}


// «»»» Affichage des objets magiques «««««««««««««««««««««««««««««««««««»

void Things_DrawMagicalObjects(HDC hDC, RECT *MapRect, LONG DX, LONG DY, LONG MapStartX, LONG MapStartY)
{
	register RECT	 ClientRect;
	register MAPTHING	*Objects;
	HPEN			 PaintPen,OldPen;
	HBRUSH		 Brush,OldBrush;
	LONG			 FX3,FY3;
	LONG			 MX,MY;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_MAGICALOBJECTS)) return;
	if (!Map.Map) return;

	FX3 = (int)((float)MapZoomCX*70.0f/100.0f);
	FY3 = (int)((float)MapZoomCY*70.0f/100.0f);
	PaintPen = CreatePen(PS_SOLID,1,0x00555555);
	if (!PaintPen) return;
	OldPen = SelectObject(hDC,PaintPen);
	if (!OldPen) {DeleteObject(PaintPen); return;}

	for (Objects = (MAPTHING *)MapMagicalObjects.next; Objects != 0; Objects = (MAPTHING *)Objects->node.next)
		{
		if (MapStartX > Objects->x) continue;
		if (MapStartY > Objects->y) continue;
		if (MapStartX+DX <= Objects->x) continue;
		if (MapStartY+DY <= Objects->y) continue;
		MX = (Objects->x-MapStartX)*MapZoomCX+MapRect->left;
		MY = (Objects->y-MapStartY)*MapZoomCY+MapRect->top;

		if (Things_IsSolid(Map.Map+Objects->x*MAP_CONTROLBYTES+Objects->y*Map.Properties.Width*MAP_CONTROLBYTES))
			goto Bad;

		if (Config.MapViewFlags&MVF_MAGICUSESPLYCOL)
			Brush = CreateSolidBrush(Players[Objects->owner-1].Color);
		else
			Brush = CreateSolidBrush(0x00FFFF00);
		if (Brush)
			{
			OldBrush = SelectObject(hDC,Brush);
			if (OldBrush)
				{
				ClientRect.left = MX;
				ClientRect.right = MX+MapZoomCX-FX3;
				ClientRect.top = MY+FY3;
				ClientRect.bottom = MY+MapZoomCY;
				FillRect(hDC,&ClientRect,Brush);

				MoveToEx(hDC,ClientRect.left+(ClientRect.right-ClientRect.left)/2,ClientRect.top,NULL);
				LineTo(hDC,ClientRect.left+(ClientRect.right-ClientRect.left)/2,ClientRect.bottom);
				MoveToEx(hDC,ClientRect.left,ClientRect.top+(ClientRect.bottom-ClientRect.top)/2,NULL);
				LineTo(hDC,ClientRect.right,ClientRect.top+(ClientRect.bottom-ClientRect.top)/2);

				SelectObject(hDC,OldBrush);
				}
			DeleteObject(Brush);
			}
		continue;

	Bad:	ClientRect.left = MX;
		ClientRect.right = MX+MapZoomCX-FX3;
		ClientRect.top = MY+FY3;
		ClientRect.bottom = MY+MapZoomCY;
		FillRect(hDC,&ClientRect,GetStockObject(BLACK_BRUSH));
		continue;
		}

	SelectObject(hDC,OldPen);
	DeleteObject(PaintPen);
	return;
}


// «»»» Affichage des zones «««««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawAreas(HWND hWnd, HDC hDC)
{
	register MAPRECTEX	*Areas;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_AREAS)) return;
	if (!Map.Map) return;

	for (Areas = (MAPRECTEX *)MapAreas.next; Areas != 0; Areas = (MAPRECTEX *)Areas->node.next)
		Map_DrawRectangle(hWnd,hDC,&Areas->rect);

	return;
}


// «»»» Affichage des zones «««««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawEffects(HWND hWnd, HDC hDC)
{
	register MAPRECTEX	*Effects;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_EFFECTS)) return;
	if (!Map.Map) return;

	for (Effects = (MAPRECTEX *)MapEffects.next; Effects != 0; Effects = (MAPRECTEX *)Effects->node.next)
		Map_DrawRectangle(hWnd,hDC,&Effects->rect);

	return;
}


// «»»» Affichage des chemins «««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawPaths(HWND hWnd, HDC hDC)
{
	MAPCLIENT		 Area1Client;
	MAPCLIENT		 Area2Client;
	RECT			 MapRect;
	POINT			 Area1Pt;
	POINT			 Area2Pt;
	HPEN			 hPen,hOldPen;
	HBRUSH			 hOldBrush;
	register MAPRECTEX	*Area1;
	register MAPRECTEX	*Area2;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_PATHS)) return;
	if (!Map.Map) return;

	Map_GetMapArea(hWnd,&MapRect);
	hPen = CreatePen(PS_SOLID,1,Config.PathsColor);
        if (!hPen) return;
	hOldPen = SelectObject(hDC,hPen);
	hOldBrush = SelectObject(hDC,GetStockObject(NULL_BRUSH));

	for (Area1 = (MAPRECTEX *)MapAreas.next; Area1 != 0; Area1 = (MAPRECTEX *)Area1->node.next)
		{
		if (!Area1->nextid) continue;

		if (Area1->nextid == Area1->id)
			{
			Map_CorrectRect(&Area1->rect,&Area1Client);
			Area1Pt.x = (Area1Client.X-MapStartX)*MapZoomCX;
			Area1Pt.x += (Area1Client.Width*MapZoomCX)/9;
			Area1Pt.x += MapRect.left;
			Area1Pt.y = (Area1Client.Y-MapStartY)*MapZoomCY;
			Area1Pt.y += (Area1Client.Height*MapZoomCY)/9;
			Area1Pt.y += MapRect.top;
			Area2Pt.x = (Area1Client.X+Area1Client.Width-MapStartX)*MapZoomCX;
			Area2Pt.x -= (Area1Client.Width*MapZoomCX)/9;
			Area2Pt.x += MapRect.left;
			Area2Pt.y = (Area1Client.Y+Area1Client.Height-MapStartY)*MapZoomCY;
			Area2Pt.y -= (Area1Client.Height*MapZoomCY)/9;
			Area2Pt.y += MapRect.top;
			Ellipse(hDC,Area1Pt.x,Area1Pt.y,Area2Pt.x,Area2Pt.y);
			continue;
			}

		Area2 = RectEx_FindById(Area1->nextid,&MapAreas);
		if (!Area2) continue;

		Map_CorrectRect(&Area1->rect,&Area1Client);
		Map_CorrectRect(&Area2->rect,&Area2Client);
		Area1Pt.x  = (Area1Client.X-MapStartX)*MapZoomCX;
		Area1Pt.x += (Area1Client.Width*MapZoomCX)/2;
		Area1Pt.x += MapRect.left;
		Area1Pt.y  = (Area1Client.Y-MapStartY)*MapZoomCY;
		Area1Pt.y += (Area1Client.Height*MapZoomCY)/2;
		Area1Pt.y += MapRect.top;
		Area2Pt.x  = (Area2Client.X-MapStartX)*MapZoomCX;
		Area2Pt.x += (Area2Client.Width*MapZoomCX)/2;
		Area2Pt.x += MapRect.left;
		Area2Pt.y  = (Area2Client.Y-MapStartY)*MapZoomCY;
		Area2Pt.y += (Area2Client.Height*MapZoomCY)/2;
		Area2Pt.y += MapRect.top;
		MoveToEx(hDC,Area1Pt.x,Area1Pt.y,NULL);
		LineTo(hDC,Area2Pt.x,Area2Pt.y);
		}

	SelectObject(hDC,hOldBrush);
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
	return;
}


// «»»» Affichage des passages ««««««««««««««««««««««««««««««««««««««««««»

void Things_DrawGates(HWND hWnd, HDC hDC)
{
	register MAPRECTEX	*Gates;
	register MAPRECT	 Rectangle;

	if (MapLocked) return;
	if (!(Config.MapViewFlags&MVF_GATES)) return;
	if (!Map.Map) return;

	ZeroMemory(&Rectangle,sizeof(MAPRECT));
	for (Gates = (MAPRECTEX *)MapGates.next; Gates != 0; Gates = (MAPRECTEX *)Gates->node.next)
		{
		Rectangle.x1 = Gates->rect.x1;
		Rectangle.y1 = Gates->rect.y1;
		Rectangle.x2 = Gates->rect.x1;
		Rectangle.y2 = Gates->rect.y1;
		if (Config.MapViewFlags&MVF_EXPANDGATES) Things_DrawGatesExpand(&Rectangle.x1,&Rectangle.y1,&Rectangle.x2,&Rectangle.y2);
		Rectangle.color = Gates->rect.color;
		Rectangle.textcolor = Gates->rect.textcolor;
		Rectangle.name = Gates->name;
		Map_DrawRectangle(hWnd,hDC,&Rectangle);
		}

	return;
}

//--- Etend les dimensions d'un portail ---

void Things_DrawGatesExpand(LONG *X1, LONG *Y1, LONG *X2, LONG *Y2)
{
	BYTE	*MapPtr;
	BYTE	*CheckPtr;

	*X2 = *X1;
	*Y2 = *Y1;
	MapPtr = Map.Map+*X1*MAP_CONTROLBYTES+*Y1*Map.Properties.Width*MAP_CONTROLBYTES;
	if (*(MapPtr+3) != 2) return;

	/* Check to the left */

	for (;;)
		{
		if (!*X1) break;
		CheckPtr = Map.Map+(*X1-1)*MAP_CONTROLBYTES+*Y1*Map.Properties.Width*MAP_CONTROLBYTES;
		if (*(CheckPtr+3) != 2) break;
		if (*CheckPtr != *MapPtr) break;
		*X1 = *X1-1;
		}

	/* Check to the right */

	for (;;)
		{
		if (!*X2 >= Map.Properties.Width) break;
		CheckPtr = Map.Map+(*X2+1)*MAP_CONTROLBYTES+*Y1*Map.Properties.Width*MAP_CONTROLBYTES;
		if (*(CheckPtr+3) != 2) break;
		if (*CheckPtr != *MapPtr) break;
		*X2 = *X2+1;
		}

	/* Check above */

	for (;;)
		{
		if (!*Y1) break;
		CheckPtr = Map.Map+*X1*MAP_CONTROLBYTES+(*Y1-1)*Map.Properties.Width*MAP_CONTROLBYTES;
		if (*(CheckPtr+3) != 2) break;
		if (*CheckPtr != *MapPtr) break;
		*Y1 = *Y1-1;
		}

	/* Check below */

	for (;;)
		{
		if (!*Y2 >= Map.Properties.Height) break;
		CheckPtr = Map.Map+*X2*MAP_CONTROLBYTES+(*Y2+1)*Map.Properties.Width*MAP_CONTROLBYTES;
		if (*(CheckPtr+3) != 2) break;
		if (*CheckPtr != *MapPtr) break;
		*Y2 = *Y2+1;
		}

	return;
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
