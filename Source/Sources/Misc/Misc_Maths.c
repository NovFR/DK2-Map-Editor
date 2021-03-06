
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines math幦atiques.
									      */
//()-------------------------------------------------------------------<<>-<>>//

// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Donn嶪s								  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

#include "..\Includes\Editor.h"
#include "..\Includes\Prototypes.h"
#include "..\Includes\Structures.h"
#include "..\Includes\Texts.h"


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Routines math幦atiques en rapport avec le jeu			  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

// 姣遙 Conversion d'un texte en nombre � virgule 垂垂垂垂垂垂垂垂垂垂垂姣

//--- Nombre sign� ---

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

//--- Nombre non sign� ---

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


// 姣遙 Conversion d'un nombre � virgule en texte 垂垂垂垂垂垂垂垂垂垂垂姣

//--- Place directement le texte dans un "EDITTEXT control" / Nombre sign� ---

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

//--- Copie le texte dans une zone m幦oire ---

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


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Fonctions							  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

// 姣遙 Converti un nombre � virgule flotante en text 垂垂垂垂垂垂垂垂垂姣

char* Misc_DoubleToText(double Double, int nDigits, char *Buffer)
{
	gcvt(Double,nDigits,Buffer);
	return(Buffer);
}


// 姣遙 Calcul un pourcentage 垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂垂姣

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

// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// END
