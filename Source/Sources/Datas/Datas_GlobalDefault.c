
#include "..\Includes\Editor.h"
#include "..\Includes\Texts.h"

/*** Pointeurs **********************************************************/

BYTE*		DefaultTerrain;
BYTE*		DefaultRooms;
BYTE*		DefaultDoors;
BYTE*		DefaultTraps;
BYTE*		DefaultKeeperSpells;
BYTE*		DefaultCreatures;
BYTE*		DefaultCreatureSpells;

/*** Table des valeurs **************************************************/

//--- Terrains ---

DWORD*		GTOrder[]={		(DWORD *)0x01, NULL,	// ImpenetrableRock
					(DWORD *)0x02, NULL,	// Rock
					(DWORD *)0x03, NULL,	// DirtPath
					(DWORD *)0x04, NULL,	// Water
					(DWORD *)0x05, NULL,	// Lava
					(DWORD *)0x06, NULL,	// Gold
					(DWORD *)0x07, NULL,	// Gems
					(DWORD *)0x08, NULL,	// ClaimedPath
					(DWORD *)0x09, NULL,	// ReinforcedWall
					(DWORD *)0x11, NULL,	// Treasury
					(DWORD *)0x0F, NULL,	// Lair
					(DWORD *)0x1E, NULL,	// Portal
					(DWORD *)0x17, NULL,	// Hatchery
					(DWORD *)0x0E, NULL,	// DungeonHeart
					(DWORD *)0x10, NULL,	// Library
					(DWORD *)0x18, NULL,	// TrainingRoom
					(DWORD *)0x13, NULL,	// WoodenBridge
					(DWORD *)0x1A, NULL,	// GuardRoom
					(DWORD *)0x19, NULL,	// WorkShop
					(DWORD *)0x1B, NULL,	// Prison
					(DWORD *)0x14, NULL,	// TortureChamber
					(DWORD *)0x16, NULL,	// Temple
					(DWORD *)0x15, NULL,	// GraveYard
					(DWORD *)0x12, NULL,	// Casino
					(DWORD *)0x1D, NULL,	// CombatPit
					(DWORD *)0x1C, NULL,	// StoneBridge
					(DWORD *)  -2, NULL,	// FogOfWar
					(DWORD *)0x21, NULL,	// HeroGateFinal
					(DWORD *)0x22, NULL,	// HeroGateTile
					(DWORD *)0x00, NULL,	// EdgeOfMap
					(DWORD *)0x0A, NULL,	// ManaVault
					(DWORD *)0x0B, NULL,	// ClaimedVault
					(DWORD *)0x23, NULL,	// HeroGate22
					(DWORD *)0x24, NULL,	// HeroGateFrontEnd
					(DWORD *)0x0C, NULL,	// HeroLair
					(DWORD *)0x1F, NULL,	// HeroStoneBridge
					(DWORD *)0x25, NULL,	// HeroGate31
					(DWORD *)0x0D, NULL,	// ClaimScan
					(DWORD *)  -2, NULL,	// Mercenary
					(DWORD *)0x20, NULL,	// HeroPortal
					(DWORD *)  -2, NULL,	// Crypt
					(DWORD *)  -1, NULL};

//--- Salles ---

DWORD*		GROrder[]={		(DWORD *)0x03, NULL,	// Treasury
					(DWORD *)0x01, NULL,	// Lair
					(DWORD *)0x10, NULL,	// Portal
					(DWORD *)0x09, NULL,	// Hatchery
					(DWORD *)0x00, NULL,	// DungeonHeart
					(DWORD *)0x02, NULL,	// Library
					(DWORD *)0x0A, NULL,	// TrainingRoom
					(DWORD *)0x05, NULL,	// WoodenBridge
					(DWORD *)0x0C, NULL,	// GuardRoom
					(DWORD *)0x0B, NULL,	// WorkShop
					(DWORD *)0x0D, NULL,	// Prison
					(DWORD *)0x06, NULL,	// TortureChamber
					(DWORD *)0x08, NULL,	// Temple
					(DWORD *)0x07, NULL,	// GraveYard
					(DWORD *)0x04, NULL,	// Casino
					(DWORD *)0x0F, NULL,	// CombatPit
					(DWORD *)0x0E, NULL,	// StoneBridge
					(DWORD *)0x13, NULL,	// HeroGateFinal
					(DWORD *)0x14, NULL,	// HeroGateTile
					(DWORD *)0x15, NULL,	// HeroGate22
					(DWORD *)0x16, NULL,	// HeroGateFrontEnd
					(DWORD *)0x11, NULL,	// HeroStoneBridge
					(DWORD *)0x17, NULL,	// HeroGate31
					(DWORD *)  -2, NULL,	// Mercenary
					(DWORD *)0x12, NULL,	// HeroPortal
					(DWORD *)  -2, NULL,	// Crypt
					(DWORD *)  -1, NULL};

//--- Portes ---

DWORD*		GDOrder[]={		(DWORD *)0x01, NULL,	// WoodenDoor
					(DWORD *)0x03, NULL,	// SteelDoor
					(DWORD *)0x02, NULL,	// BracedDoor
					(DWORD *)0x04, NULL,	// MagicDoor
					(DWORD *)0x05, NULL,	// SecretDoor
					(DWORD *)0x00, NULL,	// Barricade
					(DWORD *)0x06, NULL,	// HeroBarricade
					(DWORD *)0x07, NULL,	// HeroPortcullis
					(DWORD *)  -1, NULL};

//--- Pièges ---

DWORD*		GAOrder[]={		(DWORD *)0x05, NULL,	// Gas
					(DWORD *)0x08, NULL,	// Boulder
					(DWORD *)0x09, NULL,	// Sentry
					(DWORD *)0x00, NULL,	// Alarm
					(DWORD *)0x01, NULL,	// Fireburst
					(DWORD *)0x02, NULL,	// Lightning
					(DWORD *)0x0A, NULL,	// Freeze
					(DWORD *)0x04, NULL,	// Fear
					(DWORD *)0x03, NULL,	// Trigger
					(DWORD *)0x06, NULL,	// Spike
					(DWORD *)0x07, NULL,	// GuardPost
					(DWORD *)0x0E, NULL,	// MagicDoor
					(DWORD *)  -2, NULL,	// SpareDoor
					(DWORD *)0x0B, NULL,	// HeroSentry
					(DWORD *)0x0C, NULL,	// HeroAlarm
					(DWORD *)0x0D, NULL,	// JackInTheBox
					(DWORD *)  -1, NULL};

//--- Sorts du gardien ---

DWORD*		GSOrder[]={		(DWORD *)0x02, NULL,	// SightOfEvil
					(DWORD *)0x01, NULL,	// Possession
					(DWORD *)0x05, NULL,	// CallToArms
					(DWORD *)0x00, NULL,	// CreateImp
					(DWORD *)0x0B, NULL,	// SummonHorny
					(DWORD *)0x03, NULL,	// Thunderbolt
					(DWORD *)0x08, NULL,	// Turncoat
					(DWORD *)0x06, NULL,	// Tremor
					(DWORD *)0x09, NULL,	// Inferno
					(DWORD *)0x04, NULL,	// Heal
					(DWORD *)0x0A, NULL,	// Chicken
					(DWORD *)0x07, NULL,	// CreateGold
					(DWORD *)0x0C, NULL,	// OneShotReaper
					(DWORD *)  -1, NULL};

//--- Créatures ---

DWORD*		GCOrder[]={		(DWORD *)0x00, NULL,	// Imp
					(DWORD *)0x2C, NULL,	// PrinceTristan
					(DWORD *)0x0A, NULL,	// BileDemon
					(DWORD *)0x07, NULL,	// DarkMistress
					(DWORD *)0x02, NULL,	// Warlock
					(DWORD *)0x05, NULL,	// DarkElf
					(DWORD *)0x01, NULL,	// Goblin
					(DWORD *)0x0B, NULL,	// Vampire
					(DWORD *)0x06, NULL,	// Skeleton
					(DWORD *)0x04, NULL,	// Troll
					(DWORD *)0x08, NULL,	// Salamander
					(DWORD *)0x03, NULL,	// Firefly
					(DWORD *)0x1E, NULL,	// Knight
					(DWORD *)0x1D, NULL,	// Dwarf
					(DWORD *)0x22, NULL,	// Giant
					(DWORD *)0x21, NULL,	// Wizard
					(DWORD *)0x23, NULL,	// ElvenArcher
					(DWORD *)0x1F, NULL,	// Thief
					(DWORD *)0x24, NULL,	// Monk
					(DWORD *)0x25, NULL,	// Fairy
					(DWORD *)0x29, NULL,	// KingReginald
					(DWORD *)0x0C, NULL,	// BlackKnight
					(DWORD *)0x0D, NULL,	// DarkAngel
					(DWORD *)0x09, NULL,	// Rogue
					(DWORD *)0x20, NULL,	// Guard
					(DWORD *)0x2A, NULL,	// PrinceFelix
					(DWORD *)0x0F, NULL,	// Horny
					(DWORD *)0x27, NULL,	// StoneKnight
					(DWORD *)0x28, NULL,	// LordOfTheLand
					(DWORD *)0x26, NULL,	// RoyalGuard
					(DWORD *)0x2B, NULL,	// PrinceBalder
					(DWORD *)0x12, NULL,	// EliteFirefly
					(DWORD *)0x10, NULL,	// EliteGoblin
					(DWORD *)0x11, NULL,	// EliteWarlock
					(DWORD *)0x13, NULL,	// EliteTroll
					(DWORD *)0x14, NULL,	// EliteDarkElf
					(DWORD *)0x15, NULL,	// EliteSkeleton
					(DWORD *)0x16, NULL,	// EliteMistress
					(DWORD *)0x17, NULL,	// EliteSalamander
					(DWORD *)0x18, NULL,	// EliteRogue
					(DWORD *)0x19, NULL,	// EliteBileDemon
					(DWORD *)0x1A, NULL,	// EliteVampire
					(DWORD *)0x1B, NULL,	// EliteBlackKnight
					(DWORD *)0x1C, NULL,	// EliteDarkAngel
					(DWORD *)0x0E, NULL,	// MaidenOfTheNest
					(DWORD *)  -2, NULL,	// JackInTheBox
					(DWORD *)  -1, NULL};

//--- Sorts des créatures ---

DWORD*		GOOrder[]={		(DWORD *)0x00, NULL,
					(DWORD *)0x01, NULL,
					(DWORD *)0x02, NULL,
					(DWORD *)0x03, NULL,
					(DWORD *)0x04, NULL,
					(DWORD *)0x05, NULL,
					(DWORD *)0x06, NULL,
					(DWORD *)0x07, NULL,
					(DWORD *)0x08, NULL,
					(DWORD *)0x09, NULL,
					(DWORD *)0x0A, NULL,
					(DWORD *)0x0B, NULL,
					(DWORD *)0x0C, NULL,
					(DWORD *)0x0D, NULL,
					(DWORD *)0x0E, NULL,
					(DWORD *)0x0F, NULL,
					(DWORD *)0x10, NULL,
					(DWORD *)0x11, NULL,
					(DWORD *)0x12, NULL,
					(DWORD *)0x13, NULL,
					(DWORD *)0x14, NULL,
					(DWORD *)0x15, NULL,
					(DWORD *)0x16, NULL,
					(DWORD *)0x17, NULL,
					(DWORD *)0x18, NULL,
					(DWORD *)0x19, NULL,
					(DWORD *)0x1A, NULL,
					(DWORD *)0x1B, NULL,
					(DWORD *)0x1C, NULL,
					(DWORD *)0x1D, NULL,
					(DWORD *)0x1E, NULL,
					(DWORD *)0x1F, NULL,
					(DWORD *)0x20, NULL,
					(DWORD *)0x21, NULL,
					(DWORD *)0x22, NULL,
					(DWORD *)0x23, NULL,
					(DWORD *)0x24, NULL,
					(DWORD *)0x25, NULL,
					(DWORD *)0x26, NULL,
					(DWORD *)0x27, NULL,
					(DWORD *)0x28, NULL,
					(DWORD *)  -1, NULL};
