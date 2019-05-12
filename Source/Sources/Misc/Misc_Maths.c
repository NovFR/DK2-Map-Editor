
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines mathématiques.
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Routines mathématiques en rapport avec le jeu			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Conversion d'un texte en nombre à virgule «««««««««««««««««««««««»

//--- Nombre signé ---

DWORD Misc_GetDlgItemFloatSigned(HWND hDlg, UINT NumberCtrl)
{
	DWORD	 Result;
	DWORD	 TempNumber;
	float	 TempDec;
	char	*TempTextPtr;
	char	 TempText[64];

	GetDlgItemText(hDlg,NumberCtrl,TempText,63);
	TempTextPtr = strrchr(TempText,'.');
	if (TempTextPtr) TempDec = atof(TempTextPtr)*1000.0f/0.24414063f;
	else TempDec = 0;

	if (TempText[0] == '-')
		{
		TempNumber = atol(&TempText[1]);
		Result = -((TempNumber<<12)|(DWORD)TempDec);
		}
	else
		{
		TempNumber = atol(TempText);
		Result = (TempNumber<<12)|(DWORD)TempDec;
		}

	return(Result);
}

//--- Nombre non signé ---

DWORD Misc_GetDlgItemFloat(HWND hDlg, UINT NumberCtrl)
{
	DWORD	 TempNumber;
	float	 TempDec;
	char	*TempTextPtr;
	char	 TempText[64];

	GetDlgItemText(hDlg,NumberCtrl,TempText,63);
	TempTextPtr = strrchr(TempText,'.');
	if (TempTextPtr) TempDec = atof(TempTextPtr)*1000.0f/0.24414063f;
	else TempDec = 0;
	TempNumber = atol(TempText);

	return( (TempNumber<<12)|(DWORD)TempDec );
}


// «»»» Conversion d'un nombre à virgule en texte «««««««««««««««««««««««»

//--- Place directement le texte dans un "EDITTEXT control" / Nombre signé ---

void Misc_SetDlgItemFloatSigned(HWND hDlg, UINT NumberCtrl, DWORD GameFloat)
{
	DWORD	 Number;
	float	 Temp;
	int	 Dec,Sign;
	char	*FloatText;
	char	*FmtText;
	char	 TempText[64];

	if (((GameFloat&0xFFFFF000)>>12) > 0x7FFFF)
		{
		GameFloat = -GameFloat;
		FmtText = szNegNumber;
		}
	else
		FmtText = szNumber;

	Temp = (float)(GameFloat&0x00000FFF)*0.24414063f/1000.0f;
	Number = (GameFloat&0xFFFFF000)>>12;

	FloatText = fcvt(Temp,FLOAT_PRECISION,&Dec,&Sign);
	wsprintf(TempText,FmtText,Number);
	strcat(TempText,szPoint);
	if (Dec < 0)
		{
		while(Dec++) strcat(TempText,szZero);
		strcat(TempText,FloatText);
		}
	else
		strcat(TempText,FloatText+Dec);

	for (Number = strlen(TempText)-1; TempText[Number-1] != '.'; Number--)
		{
		if (TempText[Number] != '0') break;
		TempText[Number] = 0;
		}

	SetDlgItemText(hDlg,NumberCtrl,TempText);
	return;
}

//--- Place directement le texte dans un "EDITTEXT control" ---

void Misc_SetDlgItemFloat(HWND hDlg, UINT NumberCtrl, DWORD GameFloat)
{
	DWORD	 Number;
	float	 Temp;
	int	 Dec,Sign;
	char	*FloatText;
	char	 TempText[64];

	Temp = (float)(GameFloat&0x00000FFF)*0.24414063f/1000.0f;
	Number = (GameFloat&0xFFFFF000)>>12;

	FloatText = fcvt(Temp,FLOAT_PRECISION,&Dec,&Sign);
	wsprintf(TempText,szNumber,Number);
	strcat(TempText,szPoint);
	if (Dec < 0)
		{
		while(Dec++) strcat(TempText,szZero);
		strcat(TempText,FloatText);
		}
	else
		strcat(TempText,FloatText+Dec);

	for (Number = strlen(TempText)-1; TempText[Number-1] != '.'; Number--)
		{
		if (TempText[Number] != '0') break;
		TempText[Number] = 0;
		}

	SetDlgItemText(hDlg,NumberCtrl,TempText);
	return;
}

//--- Copie le texte dans une zone mémoire ---

void Misc_FloatToText(DWORD GameFloat, char *Text)
{
	DWORD	 Number;
	int	 Dec,Sign;
	float	 Temp;
	char	*FloatText;

	Temp = (float)(GameFloat&0x00000FFF)*0.24414063f/1000.0f;
	Number = (GameFloat&0xFFFFF000)>>12;

	FloatText = fcvt(Temp,FLOAT_PRECISION,&Dec,&Sign);
	wsprintf(Text,szNumber,Number);
	strcat(Text,szPoint);
	if (Dec < 0)
		{
		while(Dec++) strcat(Text,szZero);
		strcat(Text,FloatText);
		}
	else
		strcat(Text,FloatText+Dec);

	for (Number = strlen(Text)-1; Text[Number-1] != '.'; Number--)
		{
		if (Text[Number] != '0') break;
		Text[Number] = 0;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Converti un nombre à virgule flotante en text «««««««««««««««««««»

char* Misc_DoubleToText(double Double, int nDigits, char *Buffer)
{
	gcvt(Double,nDigits,Buffer);
	return(Buffer);
}


// «»»» Calcul un pourcentage «««««««««««««««««««««««««««««««««««««««««««»

DWORD Misc_CalculatePercent(DWORD CurrentValue, DWORD MaxValue)
{
	double	ValueIn;
	double	ValueOut;
	double	Percent;

	ValueIn = (double)CurrentValue;
	ValueOut = (double)MaxValue;
	if (ValueOut != 0)
		Percent = ValueIn/ValueOut*100;
	else
		Percent = 100;
	return((DWORD)Percent);
}

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// END
