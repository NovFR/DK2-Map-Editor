
#ifndef EDITOR_INCLUDES
#define EDITOR_INCLUDES

/*** Includes ***********************************************************/

#define OEMRESOURCE
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <process.h>
#include <time.h>
#include <math.h>
//#include <dsound.h>

#include "Version.h"
#include "MenuIdentifiers.h"

/*** Compilation ********************************************************/

#define DEBUG_EVENTS			0
#define DEBUG_TEXTS			1
#define LOCK_MULTIPLERUNS		1
#define START_MAXIMIZED			1
#define SPLASH_DISABLED			1
#define TRIGGERS_SHOWINCOMPATIBLETR	1
#define TRIGGERS_SHOWINCOMPATIBLEAC	1
#define FLOAT_PRECISION			8

/*** Données et définitions du programme ********************************/

#include "Global.h"

#include "Config.h"
#include "Paint.h"
#include "Thumbnails.h"
#include "Map.h"
#include "Things.h"
#include "Stats.h"
#include "Manager.h"
#include "Triggers.h"
#include "RectEx.h"
#include "Bands.h"
#include "Find.h"
#include "GlobalVariables.h"
#include "Game.h"
#include "Files.h"
#include "Disk.h"
#include "Menus.h"
#include "Compiler.h"
#include "LastError.h"
#include "Report.h"
#include "Requests.h"
#include "Undo.h"
#include "Libraries.h"
#include "CameraPaths.h"
//#include "Sounds.h"
#include "Compatibility.h"
#include "Debug.h"

#endif
