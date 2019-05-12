;
; Script d'installation de l'éditeur pour NSIS2.0
;

Name "Créateur de cartes pour DungeonKeeper II"

!define MUI_UI "${NSISDIR}\Contrib\UIs\modern.exe"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall.bmp"
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"
!define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\big.bmp"

!define MUI_WELCOMEPAGE_TEXT "Vous êtes sur le point d'installer $(^Name) sur votre ordinateur.\r\n\r\nCe programme est encore en cours de développment. Il est possible que certains problèmes apparaissent au cours de son utilisation. En outre, certaines fonctions ne sont pas terminées ou inaccessibles.\r\n\r\nCet éditeur n'est PAS un éditeur officiel.\r\n\r\n"
!define MUI_WELCOMEPAGE_TITLE_3LINES

!define MUI_FINISHPAGE_RUN "$INSTDIR\Editor.exe"

Var MUI_TEMP
Var STARTMENU_FOLDER

!include "MUI.nsh"
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "French"

!define TEMP1 $R0

InstallDir "$PROGRAMFILES\Editor"
OutFile "SetupEditor.exe"
AllowRootDirInstall false
InstallDirRegKey HKEY_LOCAL_MACHINE "Software\Dungeon Keeper 2 Unofficial Editor" "Path"
ShowInstDetails show
ShowUninstDetails show

;
; --- Copies ---
;

;
; Copie du programme principal
;
 
Section "Editeur" Editor

	DetailPrint "Installation du programme..."

	SectionIn RO

	SetOutPath "$INSTDIR"
	File "Test\Editor.exe"
	SetOutPath "$INSTDIR\Globals"
	File "Test\Globals\DefaultCreatures.dkg"
	File "Test\Globals\DefaultCreatureSpells.dkg"
	File "Test\Globals\DefaultDoors.dkg"
	File "Test\Globals\DefaultKeeperSpells.dkg"
	File "Test\Globals\DefaultRooms.dkg"
	File "Test\Globals\DefaultTerrain.dkg"
	File "Test\Globals\DefaultTraps.dkg"
	SetOutPath "$INSTDIR\Libs"
	File "Test\Libs\CameraPaths.dll"

	WriteRegStr HKEY_LOCAL_MACHINE "Software\Dungeon Keeper 2 Unofficial Editor" "Path" "$INSTDIR"
	WriteRegDWORD HKEY_LOCAL_MACHINE "Software\Dungeon Keeper 2 Unofficial Editor" "Version" 38
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Dungeon Keeper 2 Unofficial Editor" "DisplayName" "Créateur de cartes pour Dungeon Keeper II (déinstallation uniquement)"
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Dungeon Keeper 2 Unofficial Editor" "UninstallString" "$INSTDIR\Uninstall.exe"
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	; Write shortcuts

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	SetOutPath "$INSTDIR"
	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER\Désinstallation"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Editeur.lnk" "$INSTDIR\Editor.exe"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Désinstallation\Désinstaller l'éditeur.lnk" "$INSTDIR\Uninstall.exe"
	!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;
;  Copie de la documentation
;

Section "Documentation" HelpFile

	DetailPrint "Installation de la documentation..."

	SetOutPath "$INSTDIR\Help"
	File "Help\Editor.chm"

	; Write shortcuts

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	SetOutPath "$INSTDIR"
	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER\Aide"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Aide\Documentation.lnk" "$INSTDIR\Help\Editor.chm"
	!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;
; Copie des sons
;

Section "Sons" Sounds

	DetailPrint "Installation des sons..."

	SetOutPath "$INSTDIR\Sounds"
	File "Test\Sounds\Build - Claimed.wav"
	File "Test\Sounds\Build - Creature.wav"
	File "Test\Sounds\Build - Destroy.wav"
	File "Test\Sounds\Build - Gems.wav"
	File "Test\Sounds\Build - Gold.wav"
	File "Test\Sounds\Build - Heart.wav"
	File "Test\Sounds\Build - Imp.wav"
	File "Test\Sounds\Build - Lava.wav"
	File "Test\Sounds\Build - Room.wav"
	File "Test\Sounds\Build - Selection.wav"
	File "Test\Sounds\Build - Tiles.wav"
	File "Test\Sounds\Build - Wall.wav"
	File "Test\Sounds\Build - Water.wav"
	File "Test\Sounds\Button - Paint.wav"
	File "Test\Sounds\Button - Terrain.wav"

SectionEnd

;
; Copie des niveaux bonus
;

Section "Bonus" Bonus

	DetailPrint "Installation des niveaux bonus..."

	SetOutPath "$INSTDIR\Bonus\Niveaux"
	File "--- Niveaux ---\Editeur\0-002 Nouvelle alliance\Début2.dkm"
	File "--- Niveaux ---\Editeur\0-003 Mission\Mission3.dkm"
	File "--- Niveaux ---\Editeur\0-004 Priésouslesbois\Renforts.dkm"
	File "--- Niveaux ---\Editeur\0-005 Trahison\Trahison3.dkm"
	File "--- Niveaux ---\Editeur\0-006 Infiltration\Infiltration4.dkm"
	File "--- Niveaux ---\Editeur\0-007 Récupération\Récupération7.dkm"
	File "--- Niveaux ---\Editeur\0-010 Assiégé\Assiégé3.dkm"
	File "--- Niveaux ---\Editeur\0-012 Le monde à l'envers\Le monde à l'envers2.dkm"
	File "--- Niveaux ---\Editeur\0-014 La prison des enfers\La prison des enfers.dkm"
	File "--- Niveaux ---\Editeur\0-016 Gladiator\Gladiator.dkm"
	File "--- Niveaux ---\Editeur\0-017 Péril\Péril2.dkm"

	SetOutPath "$INSTDIR\Bonus\Spéciaux"
	File "--- Niveaux ---\Editeur\Bonus 01\Bonus1-2.dkm"
	File "--- Niveaux ---\Editeur\Bonus 02\Bonus2.dkm"
	File "--- Niveaux ---\Editeur\Bonus 03\Bonus3.dkm"
	File "--- Niveaux ---\Editeur\Bonus 04\Bonus4.dkm"
	File "--- Niveaux ---\Editeur\Bonus 05\Bonus5.dkm"
	File "--- Niveaux ---\Editeur\Bonus 06\Bonus6.dkm"
	File "--- Niveaux ---\Editeur\Bonus 07\Bonus7.dkm"
	File "--- Niveaux ---\Editeur\Bonus 08\Bonus8.dkm"
	File "--- Niveaux ---\Editeur\Bonus 09\Bonus9.dkm"

SectionEnd


;
; --- Descriptions ---
;

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN

	!insertmacro MUI_DESCRIPTION_TEXT ${Editor} "Il s'agit du programme principal. Vous devez impérativement installer ce composant."
	!insertmacro MUI_DESCRIPTION_TEXT ${HelpFile} "La documentation contient de nombreuses informations qui vous permettront d'utiliser au mieux le programme."
	!insertmacro MUI_DESCRIPTION_TEXT ${Sounds} "Si vous installez les sons, certaines de vos actions seront ponctuées par un petit effet sonore. DirectX (version 7.0 minimum) doit être installé pour que les sons fonctionnent."
	!insertmacro MUI_DESCRIPTION_TEXT ${Bonus} "Tout plein de niveaux bonus pour exercer vos talents de gardien."

!insertmacro MUI_FUNCTION_DESCRIPTION_END

;
; --- Avertissement avant l'installation ---
;

Function .onInit

	SetOutPath $TEMP
	File /oname=editor.bmp "Bitmaps\Splash.bmp"
	advsplash::show 4000 1000 500 0x00000000 $TEMP\editor
	Pop $0
	Delete $TEMP\editor.bmp

FunctionEnd

;
; --- Déinstallation ---
;

Section "Uninstall"

	; Removing shortcuts

	DetailPrint "Désinstallation des raccourcis..."
        !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
	Delete "$SMPROGRAMS\$MUI_TEMP\Editeur.lnk"
	Delete "$SMPROGRAMS\$MUI_TEMP\Désinstallation\Désinstaller l'éditeur.lnk"
	Delete "$SMPROGRAMS\$MUI_TEMP\Aide\Documentation.lnk"
	RMDir "$SMPROGRAMS\$MUI_TEMP\Désinstallation"
	RMDir "$SMPROGRAMS\$MUI_TEMP\Aide"
	RMDir "$SMPROGRAMS\$MUI_TEMP"

	; Removing files

	DetailPrint "Déinstallation des fichiers..."
	Delete "$INSTDIR\Editor.exe"
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\Globals\DefaultCreatures.dkg"
	Delete "$INSTDIR\Globals\DefaultCreatureSpells.dkg"
	Delete "$INSTDIR\Globals\DefaultDoors.dkg"
	Delete "$INSTDIR\Globals\DefaultKeeperSpells.dkg"
	Delete "$INSTDIR\Globals\DefaultRooms.dkg"
	Delete "$INSTDIR\Globals\DefaultTerrain.dkg"
	Delete "$INSTDIR\Globals\DefaultTraps.dkg"
	Delete "$INSTDIR\Libs\CameraPaths.dll"
	Delete "$INSTDIR\Help\Editor.chm"
	Delete "$INSTDIR\Sounds\Build - Claimed.wav"
	Delete "$INSTDIR\Sounds\Build - Creature.wav"
	Delete "$INSTDIR\Sounds\Build - Destroy.wav"
	Delete "$INSTDIR\Sounds\Build - Gems.wav"
	Delete "$INSTDIR\Sounds\Build - Gold.wav"
	Delete "$INSTDIR\Sounds\Build - Heart.wav"
	Delete "$INSTDIR\Sounds\Build - Imp.wav"
	Delete "$INSTDIR\Sounds\Build - Lava.wav"
	Delete "$INSTDIR\Sounds\Build - Room.wav"
	Delete "$INSTDIR\Sounds\Build - Selection.wav"
	Delete "$INSTDIR\Sounds\Build - Tiles.wav"
	Delete "$INSTDIR\Sounds\Build - Wall.wav"
	Delete "$INSTDIR\Sounds\Build - Water.wav"
	Delete "$INSTDIR\Sounds\Button - Paint.wav"
	Delete "$INSTDIR\Sounds\Button - Terrain.wav"
	Delete "$INSTDIR\Bonus\Niveaux\Début2.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Mission3.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Renforts.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Trahison3.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Infiltration4.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Récupération7.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Assiégé3.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Le monde à l'envers2.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\La prison des enfers.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Gladiator.dkm"
	Delete "$INSTDIR\Bonus\Niveaux\Péril2.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus1-2.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus2.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus3.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus4.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus5.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus6.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus7.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus8.dkm"
	Delete "$INSTDIR\Bonus\Spéciaux\Bonus9.dkm"

	DetailPrint "Mise à jour du registre..."
	DeleteRegKey HKEY_CURRENT_USER "Software\Dungeon Keeper 2 Unofficial Editor"
	DeleteRegKey HKEY_LOCAL_MACHINE "Software\Dungeon Keeper 2 Unofficial Editor"
	DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Dungeon Keeper 2 Unofficial Editor"

	RMDir "$INSTDIR\Bonus\Spéciaux"
	RMDir "$INSTDIR\Bonus\Niveaux"
	RMDir "$INSTDIR\Bonus"
	RMDir "$INSTDIR\Sounds"
	RMDir "$INSTDIR\Help"
	RMDir "$INSTDIR\Libs"
	RMDir "$INSTDIR\Globals"
	RMDir "$INSTDIR"

SectionEnd
