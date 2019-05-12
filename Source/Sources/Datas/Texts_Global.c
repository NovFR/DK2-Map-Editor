
/*** Textes fixes *******************************************************/

char	szAppName[]				= "MapEditorWin32";

char	szButtonClass[]				= "BUTTON";
char	szScrollBarClass[]			= "SCROLLBAR";
char	szStaticClass[]				= "STATIC";
char	szEditClass[]				= "EDIT";
char	szListBoxClass[]			= "LISTBOX";
char	szComboBoxClass[]			= "COMBOBOX";
char	szListViewClass[]			= "SysListView32";
char	szTreeViewClass[]			= "SysTreeView32";
char	szProgressClass[]			= "msctls_progress32";
char	szDialogClass[]				= "MapEditorDialog32";
char	szMessageClass[]			= "MapEditorMessage32";
char	szDiskClass[]				= "MapEditorDisk32";
char	szMapSeparator[]			= " -- ";
char	szMapSlash[]				= " / ";
char	szLF[]					= "\n";
char	szCRLF[]				= "\r\n\0";
char	szRight[]				= ">>";
char	szUnknown[]				= "???";
char	szNone[]				= "-";
char	szCtxMenu[]				= "CTXMENU";
char	szMouseCoordinates[]			= "X: %ld / Y: %ld";
char	szRectCoordinates[]			= "X: %ld / Y: %ld -- L: %ld / H: %ld";
char	szZoom[]				= "%ld x %ld";
char	szNumber[]				= "%ld";
char	szNumberPlus[]				= "%ld+";
char	szNegNumber[]				= "-%ld";
char	szPoint[]				= ".";
char	szZero[]				= "0\0\0\0";
char	szText[]				= "%s";
char	szQuotedText[]				= "\"%s\"";
char	szSpace[]				= " ";
char	szTime[]				= "%02ld:%02ld:%02ld";
char	szX[]					= "X";
char	szY[]					= "Y";
char	szMapsPath[]				= "\\Data\\Editor\\Maps\\";
char	szSoundsPath[]				= "\\Data\\Sound\\sfx\\";
char	szTextsPath[]				= "\\Data\\Text\\Default\\";
char	szTextsExt[]				= ".str";
char	szSoundBuffer[]				= "HD.SDT";
char	szSoundFile[]				= "SPEECH_";
char	szSpeechMentor[]			= "SPEECH_MENTOR";
char	szTempSoundFile[]			= "dk2ed-speechfile";
char	szTempSpeechID[]			= "----------";
char	szTempMP2Ext[]				= ".mp2";
char	szGameCommandLine[]			= "%s\\dkii.exe?-level \"%s\"";
char	szPath[]				= "\\";
char	szExclamation[]				= " ! ";
char	szThingGold[]				= " [%ld]";
char	szThingSpell[]				= " [%s]";
char	szRegistryGlobalEntry[]			= "Software\\Dungeon Keeper 2 Unofficial Editor";
char	szRegistryRecentEntry[]			= "Software\\Dungeon Keeper 2 Unofficial Editor\\Recent";
char	szRegistryRecentTmp[]			= "LastFile%02ld";
char	szRegistryRecent[]			= "LastFile00";
char	szGameExecutable[]			= "DKII.exe";
char	szTemporaryFolder[]			= "Dungeon Keeper 2 Unofficial Editor";
char	szTemporaryFile[]			= "Autosave.dkm";
char	szRegistrySafeFlag[]			= "Clean exit";
char	szRegistryGamePath[]			= "GamePath";
char	szSpecialNumber[]			= "%04ld -- ";
char	szMore[]				= "...";

char	szSoundClaimed[]			= "Sounds\\Build - Claimed.wav";
char	szSoundCreature[]			= "Sounds\\Build - Creature.wav";
char	szSoundDestroy[]			= "Sounds\\Build - Destroy.wav";
char	szSoundGems[]				= "Sounds\\Build - Gems.wav";
char	szSoundGold[]				= "Sounds\\Build - Gold.wav";
char	szSoundHeart[]				= "Sounds\\Build - Heart.wav";
char	szSoundImp[]				= "Sounds\\Build - Imp.wav";
char	szSoundLava[]				= "Sounds\\Build - Lava.wav";
char	szSoundRoom[]				= "Sounds\\Build - Room.wav";
char	szSoundSelection[]			= "Sounds\\Build - Selection.wav";
char	szSoundTiles[]				= "Sounds\\Build - Tiles.wav";
char	szSoundWall[]				= "Sounds\\Build - Wall.wav";
char	szSoundWater[]				= "Sounds\\Build - Water.wav";
char	szSoundPaint[]				= "Sounds\\Button - Paint.wav";
char	szSoundTerrain[]			= "Sounds\\Button - Terrain.wav";

char	szGlobalDefaultTerrain[]		= "Globals\\DefaultTerrain.dkg";
char	szGlobalDefaultRooms[]			= "Globals\\DefaultRooms.dkg";
char	szGlobalDefaultDoors[]			= "Globals\\DefaultDoors.dkg";
char	szGlobalDefaultTraps[]			= "Globals\\DefaultTraps.dkg";
char	szGlobalDefaultKeeperSpells[]		= "Globals\\DefaultKeeperSpells.dkg";
char	szGlobalDefaultCreatures[]		= "Globals\\DefaultCreatures.dkg";
char	szGlobalDefaultCreatureSpells[]		= "Globals\\DefaultCreatureSpells.dkg";

char	szKwdFileTerrain[]			= "Data\\Editor\\Terrain";
char	szKwdFileObjects[]			= "Data\\Editor\\Objects";
char	szKwdFileRooms[]			= "Data\\Editor\\Rooms";
char	szKwdFileCreatures[]			= "Data\\Editor\\Creatures";
char	szKwdFileCreatureSpells[]		= "Data\\Editor\\CreatureSpells";
char	szKwdFileTraps[]			= "Data\\Editor\\Traps";
char	szKwdFileDoors[]			= "Data\\Editor\\Doors";
char	szKwdFileShots[]			= "Data\\Editor\\Shots";
char	szKwdFileKeeperSpells[]			= "Data\\Editor\\KeeperSpells";
char	szKwdFileUserPlayers[]			= "Data\\Editor\\Maps\\%sPlayers.kld";
char	szKwdFileUserMap[]			= "Data\\Editor\\Maps\\%sMap.kld";
char	szKwdFileUserTriggers[]			= "Data\\Editor\\Maps\\%sTriggers.kld";
char	szKwdFileGlobalVariables[]		= "Data\\Editor\\GlobalVariables.kwd";
char	szKwdFileUserVariables[]		= "Data\\Editor\\Maps\\%sVariables.kld";
char	szKwdFileUserThings[]			= "Data\\Editor\\Maps\\%sThings.kld";
char	szKwdFileUserTerrain[]			= "Data\\Editor\\Maps\\%sTerrain.kld";
char	szKwdFileUserRooms[]			= "Data\\Editor\\Maps\\%sRooms.kld";
char	szKwdFileUserDoors[]			= "Data\\Editor\\Maps\\%sDoors.kld";
char	szKwdFileUserTraps[]			= "Data\\Editor\\Maps\\%sTraps.kld";
char	szKwdFileUserKeeperSpells[]		= "Data\\Editor\\Maps\\%sKeeperSpells.kld";
char	szKwdFileUserCreatures[]		= "Data\\Editor\\Maps\\%sCreatures.kld";
char	szKwdFileUserCreatureSpells[]		= "Data\\Editor\\Maps\\%sCreatureSpells.kld";

char	szGameMainExt[]				= ".kwd";
char	szGameMapExt[]				= "Map.kld";
char	szGamePlayersExt[]			= "Players.kld";
char	szGameThingsExt[]			= "Things.kld";
char	szGameTriggersExt[]			= "Triggers.kld";
char	szGameVariablesExt[]			= "Variables.kld";
char	szGameTerrainExt[]			= "Terrain.kld";
char	szGameRoomsExt[]			= "Rooms.kld";
char	szGameDoorsExt[]			= "Doors.kld";
char	szGameTrapsExt[]			= "Traps.kld";
char	szGameKeeperSpellsExt[]			= "KeeperSpells.kld";
char	szGameCreaturesExt[]			= "Creatures.kld";
char	szGameCreatureSpellsExt[]		= "CreatureSpells.kld";
char	szTextMapExt[]				= ".map";
char	szAreaMapExt[]				= ".dkz";
char	szEditorMapExt[]			= ".dkm";
char	szGameExt[]				= "kwd";
char	szTextExt[]				= "map";
char	szEditorExt[]				= "dkm";
char	szTextMapHeader[]			= "GUI_GENERATED_MAP";
char	szTextMapIdent[]			= "MAP";
char	szTextMapPlayers[]			= "PLAYERS";
char	szTextMapMult[]				= " X ";
char	szTextMapThingsIdent[]			= "THINGS";
char	szTextMapCreaturesIdent[]		= "CREATURES";
char	szTextMapDoorsIdent[]			= "DOORS";
char	szTextMapTrapsIdent[]			= "TRAPS";
char	szTextMapEOFIdent[]			= "EOF";
char	szTextMapLevel[]			= "L%ld";
char	szTextMapPlayer[]			= "P";
char	szTextMapLocked[]			= "L";
char	szTextMapUnlocked[]			= "U";
char	szTextMapHearts[]			= "HEARTS_AND_GOLD";
char	szTextMapCenter[]			= "KPR%ld_HEART_CENTER";
char	szTextMapGold[]				= "GOLD%ld";
char	szTextMapCenterKpr1[]			= "KPR1_HEART_CENTER";
char	szTextMapCenterKpr2[]			= "KPR2_HEART_CENTER";
char	szTextMapCenterKpr3[]			= "KPR3_HEART_CENTER";
char	szTextMapCenterKpr4[]			= "KPR4_HEART_CENTER";
char	szTextMapGoldKpr1[]			= "GOLD1";
char	szTextMapGoldKpr2[]			= "GOLD2";
char	szTextMapGoldKpr3[]			= "GOLD3";
char	szTextMapGoldKpr4[]			= "GOLD4";
char	szTextMapBands[]			= "HERO_BANDS";
char	szTextMapHeros[]			= "HEROES";
char	szTextMapAreas[]			= "AREAS";
char	szTextMapGates[]			= "HERO_GATES";
char	szTextMapTriggers[]			= "TRIGGERS";
char	szTextMapOptions[]			= "SCENARIO_DESCRIPTION";
char	szTextMapEOF[]				= "EOF";

/*** Titres de la fenêtre ***********************************************/

char	szWindowTitle[]				= "Créateur de carte pour Dungeon Keeper II®";
char	szSplashScreen[]			= "Initialisation en cours... Veuillez patienter...";
char	szInitErr[]				= "Erreur d'initialisation";
char	szInternalError[]			= "Erreur interne";

/*** Date ***************************************************************/

char	szDate[]				= "le %s %ld %s %ld à %02ld:%02ld:%02ld";
char	szDateNoDay[]				= "le %ld %s %ld à %02ld:%02ld:%02ld";

/*** Messages d'erreur **************************************************/

char	szVersionErr[]				= "Une erreur s'est produite lors de l'obtention de la version du système d'exploitation.";
char	szWinVersionErr[]			= "Ce programme ne fonctionne qu'avec les systèmes d'exploitation Windows®95/98/Me (ou plus récent).";
char	szMutexErr[]				= "Impossible d'initialiser l'élément permettant d'empêcher les lancements multiples.";
char	szAlreadyExistsErr[]			= "Le programme est déjà présent en mémoire !";
char	szMissingCapsErr[]			= "Votre carte graphique ne prend pas en compte certaines fonctions nécessaires au programme.\nL'application va maintenant s'arrêter.";
char	szHookErr[]				= "Une erreur s'est produite lors de l'installation du gestionnaire des raccourcis clavier.";
char	szWindowErr[]				= "Une erreur s'est produite lors de l'initialisation d'une fenêtre.";
char	szWindowMenuErr[]			= "Une erreur s'est produite lors de la création du menu.";
char	szDialogErr[]				= "Une erreur s'est produite lors de la création de la boîte de dialogue.";
char	szPropertyErr[]				= "La création de la page a échoué.\nImpossible d'afficher la page demandée.";
char	szTaskErr[]				= "Une erreur s'est produite lors de la création de la tâche.";
char	szGlobalAllocErr[]			= "La mémoire allouée n'a pas été totalement restituée.";
char	szNoAreaErr[]				= "Cette carte ne comporte aucune zone !";
char	szNoEffectErr[]				= "Cette carte ne comporte aucun effet !";
char	szTempErr[]				= "Une erreur s'est produite lors de la création de la sauvegarde automatique !";
char	szNoTempMapErr[]			= "Aucune sauvegarde automatique n'a été trouvée...";
char	szHelpErr[]				= "Une erreur s'est produite lors de l'accès au fichier d'aide.";
char	szBitmapErr[]				= "Une erreur s'est produite lors de la création du fichier bitmap.";

/*** Nom ASCII des touches **********************************************/

char	szShift[]				= "Shift";
char	szCtrl[]				= "Ctrl";
char	szReturn[]				= "Entrée";
char	szDel[]					= "Suppr.";
char	szF1[]					= "F1";
char	szF2[]					= "F2";
char	szF3[]					= "F3";
char	szF4[]					= "F4";
char	szF5[]					= "F5";
char	szF6[]					= "F6";
char	szF7[]					= "F7";
char	szF8[]					= "F8";
char	szF9[]					= "F9";
char	szF10[]					= "F10";
char	szF11[]					= "F11";
char	szF12[]					= "F12";

/*** Textes en rapport avec la sélection ********************************/

char	szCopyRequest[]				= "La mémoire de copie contient déjà des données !\nRemplacer ces données par les nouvelles ?";
char	szSymmRequest[]				= "Le rectangle n'est pas suffisamment grand pour pouvoir procéder à la transformation !\nIl doit avoir au moins deux carrés de largeur pour une symétrie sur l'axe vertical\net deux carrés de hauteur pour une symétrie sur l'axe horizontal";
char	szRotationRequest[]			= "Les dimensions du rectangle ne sont pas compatibles avec la rotation !\nPour pouvoir effectuer une rotation, le rectangle doit former un carré parfait.\n(La hauteur et la largeur doivent avoir les mêmes dimensions).";

char	szCopyErr[]				= "Une erreur s'est produite lors de la copie.";
char	szTransErr[]				= "Une erreur s'est produite lors de la transformation.";
char	szSelectRectErr[]			= "Le rectangle définit dépasse la taille totale de la carte !\nVeuillez modifier les valeurs X, Y, la largeur ou la hauteur\nafin que le rectangle de sélection puisse tenir intégralement\nsur la carte.";
char	szAreaThingsErr[]			= "Une erreur s'est produite lors du chargement des éléments : \n\"%s\"\n\nDésirez-vous continuer malgré l'erreur ?";

/*** Textes des boutons *************************************************/

char	szAuto[]				= "Automatique";
char	szAny[]					= "N'importe quel joueur";
char	szHeros[]				= "Héros";
char	szNeutral[]				= "Neutre";
char	szKeeper1[]				= "Gardien 1";
char	szKeeper2[]				= "Gardien 2";
char	szKeeper3[]				= "Gardien 3";
char	szKeeper4[]				= "Gardien 4";
char	szKeeper5[]				= "Gardien 5";

char	szHealth[]				= "Santé";
char	szGoldHeld[]				= "Or";
char	szBehavior[]				= "Situation";
char	szOptions[]				= "Options";
char	szOwner[]				= "Détenteur";
char	szLevel[]				= "Niveau";
char	szGoldAmount[]				= "Quantité d'or";
char	szGoldAmountShort[]			= "Or";
char	szPosition[]				= "Position";
char	szEnnemy[]				= "Ennemi";
char	szObjective[]				= "Objectif";
char	szTargetArea[]				= "Zone à rejoindre";
char	szTargetAreaShort[]			= "Zone";
char	szSpellBook[]				= "Contenant le sort";
char	szSpellBookShort[]			= "Sort";
char	szNoSpell[]				= "Néant...";
char	szThingPosition[]			= "Position";
char	szManualPosition[]			= "-";

char	szClose[]				= "Fermer";
char	szRun[]					= "Exécuter";

char	szOk[]					= "Ok";
char	szCancel[]				= "Abandon";

/*** Textes de la carte du jeu ******************************************/

char	szKwdNeutral[]				= "Neutral Player";
char	szKwdHeros[]				= "Good Player";
char	szKwdKeeper1[]				= "Keeper 1";
char	szKwdKeeper2[]				= "Keeper 2";
char	szKwdKeeper3[]				= "Keeper 3";
char	szKwdKeeper4[]				= "Keeper 4";
char	szKwdKeeper5[]				= "Keeper 5";

