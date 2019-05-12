
#include "..\Includes\Editor.h"
#include "..\Includes\Texts.h"
extern char		CompilerError[256];

/*** Définitions ********************************************************/

ERRORTABLE		Compiler[]={	{COMPILERR_MAPALLOC,   szCompilerAllocErr},
					{COMPILERR_BADHEADER,  szCompilerBadHeaderErr},
					{COMPILERR_BADSIZE,    szCompilerBadSizeErr},
					{COMPILERR_EOF,	       szCompilerEOFErr},
					{COMPILERR_BADVERSION, szCompilerBadVersionErr},
					{COMPILERR_UNKHEADER,  szCompilerUnknownHeaderErr},
					{COMPILERR_WINDOWS,    CompilerError},
					{NULL,		       NULL}};

