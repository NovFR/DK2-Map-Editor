
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des erreurs.
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

extern char		CompilerError[256];
extern DWORD		CompilerErrorCode;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Stockage de la dernière erreur rencontrée «««««««««««««««««««««««»

//--- Définition de l'erreur ---

void Compiler_SetLastError(DWORD ErrorCode)
{
	CompilerErrorCode = ErrorCode;

	if (CompilerErrorCode == COMPILERR_WINDOWS)
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),LOCALE_USER_DEFAULT,CompilerError,256,NULL);

	return;
}

//--- Retourne le dernier code d'erreur ---

DWORD Compiler_GetLastError()
{
	return(CompilerErrorCode);
}


// «»»» Copie un texte d'erreur dans un buffer ««««««««««««««««««««««««««»

void Compiler_ErrorCodeToText(char *Dest)
{
	char	*ErrorText = NULL;
	int	 i;

	if (!CompilerErrorCode)
		{
		strcpy(Dest,szCompilerNoErr);
		return;
		}

	for (i = 0; Compiler[i].ErrorCode != 0; i++)
		if (Compiler[i].ErrorCode == CompilerErrorCode)
			{
			ErrorText = Compiler[i].ErrorText;
			break;
			}

	if (!ErrorText) ErrorText = szCompilerUnknownErr;

	strcpy(Dest,ErrorText);
	return;
}


// «»»» Retourne le pointeur sur le texte correspondant à l'erreur ««««««»

char* Compiler_GetErrorCodeText()
{
	int	 i;

	if (!CompilerErrorCode)
		return(szCompilerNoErr);

	for (i = 0; Compiler[i].ErrorCode != 0; i++)
		if (Compiler[i].ErrorCode == CompilerErrorCode)
			return(Compiler[i].ErrorText);

	return(szCompilerUnknownErr);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
