#include "stdafx.h"
#include "bass.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifdef LINK_TRNG
#include "..\Tomb_NextGeneration\Tomb_NextGeneration.h"
#include "..\Tomb_NextGeneration\structures.h"
#include "..\Tomb_NextGeneration\DefTomb4Funct.h"
#include "..\Tomb_NextGeneration\macros.h"
#else
#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll
#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "macros.h"  // definition of macros
#endif

#include "trng.h"

// #TAG_ADDING_NEW_DECLARES#
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
int Service(DWORD SRV_Type, ...);


extern TYPE_StopEffect StopEffect;
extern TYPE_GetJointAbsPosition GetJointAbsPosition;
extern TYPE_InitialiseCamera InitialiseCamera;
extern TYPE_GetFloor GetFloor;
extern TYPE_GetHeight GetHeight;
extern TYPE_ItemNewRoom ItemNewRoom;
extern TYPE_SetMapRoom SetMapRoom;
extern TYPE_tombInviaLog tombInviaLog;
extern TYPE_pcMostraMessaggio pcMostraMessaggio;
extern TYPE_SetFogColor SetFogColor;
// -> last declare line



// #TAG_ADDING_NEW_VARIABLES#
extern char BufferLog[];
extern StrTrngInfos Trng;

char BufGlobalTimer[] = "#0033";
char BufLocalTimer[] = "#0073";
DWORD Splash_Rain=1;
float Rain_Float_20480 = 0x5000;
int VetIncTrasparenza[] = {0,0, 64, 32, 16, 4};
BYTE FlagMicroCaratteri=0;
//...



// #TAG_ADDING_NEW_FUNCTIONS#
// -> last added function bodies
// -> last added function bodies
// -> last added function bodies
void AggiornaCutsceneCamera(StrCutsceneCamera *pCut)
{
	Service(SRV_F_AggiornaCutsceneCamera, );
#=0
}

WORD GestionePickups(WORD Slot, int Operazione, short Valore)
{
	Service(SRV_F_GestionePickups, );
#=0
}

void StopBassSuoni(int NumeroCanale)
{
	Service(SRV_F_StopBassSuoni, );
#=0
}

void AddScriptAddEffect(int IndiceOggetto, short IdEffetto, WORD ExtraTimer)
{
	Service(SRV_F_AddScriptAddEffect, );
#=0
}

void SwapMeshLara(int IndicePerSlot, int TipoSwapMesh)
{
	Service(SRV_F_SwapMeshLara, );
#=0
}

int EseguiTriggerGroup(short IdTriggerGroup)
{
	Service(SRV_F_EseguiTriggerGroup, );
#=0
}

int EsecuzioneActionTrigger(WORD PluginID, WORD AzioneTimer, int ItemIndex, WORD Flags)
{
	Service(SRV_F_EsecuzioneActionTrigger, );
#=0
}

void AvviaPopUpImage(WORD IdImageCmd, StrScriptImage *pImage, WORD Contatore)
{
	Service(SRV_F_AvviaPopUpImage, );
#=0
}

void GestioneOverlapImage(StrScriptImage *pImage, WORD Secondi)
{
	Service(SRV_F_GestioneOverlapImage, );
#=0
}

 int LeggiVariabile(WORD CodiceVariabile)
{
	Service(SRV_F_LeggiVariabile, );
#=0
}

void ScriviVariabile(WORD CodiceVariabile, int Valore)
{
	Service(SRV_F_ScriviVariabile, );
#=0
}

int LeggiVariabileMem(WORD Codice)
{
	Service(SRV_F_LeggiVariabileMem, );
#=0
}

char *ReplaceVarPlaceFolders(char *pTesto)
{
	Service(SRV_F_ReplaceVarPlaceFolders, );
#=0
}

char *GetNomeVariabile(WORD Codice)
{
	Service(SRV_F_GetNomeVariabile, );
#=0
}

void TerminaStandBy(StrBaseStandBy *pStand)
{
	Service(SRV_F_TerminaStandBy, );
#=0
}

void InizializzaStandBy(StrBaseStandBy *pStand)
{
	Service(SRV_F_InizializzaStandBy, );
#=0
}

int EsecuzioneFlipeffect(WORD PluginId, int NumeroFlip, WORD Timer, WORD Flags)
{
	Service(SRV_F_EsecuzioneFlipeffect, );
#=0
}

int GestioneDemoInProgress(WORD FlagGestioneDemo, StrDemo *pDemo)
{
	Service(SRV_F_GestioneDemoInProgress, );
#=0
}

// -> last added function bodies
void FineDebugCounter(void)
{
	Service(SRV_F_FineDebugCounter, );
#=0
}

// -> last added function bodies

int ProporzioneDistanza(int Incremento, int Distanza)
{

	return Service(SRV_F_ProporzioneDistanza, Incremento, Distanza);

}


void SetFogEnd(void) 
{
	Service(SRV_F_SetFogEnd);
}

void SalvaDimensioneSchermo(void)
{
	Service(SRV_F_SalvaDimensioneSchermo);
	
}

void SalvaRecordSwapMesh(StrFlipSwapMesh *pSwap)
{
	Service(SRV_F_SalvaRecordSwapMesh, pSwap);
}

void GestioneCdPlay(short NumeroCd, int Loop)
{
	Service(SRV_F_GestioneCdPlay, NumeroCd, Loop);

}

void AggiornaBinocoloSuper(void)
{

	Service(SRV_F_AggiornaBinocoloSuper);
}

bool InizioDebugCounter(void)
{
	if (Service(SRV_F_InizioDebugCounter) != 0) return true;
	return false;
}


void PreparaLancioFilmato(int NumeroFMV)
{
	Service(SRV_F_PreparaLancioFilmato, NumeroFMV);

}

void SospendiHardCoded(int NAnimazione, int FrameToWait)
{
	Service(SRV_F_SospendiHardCoded, NAnimazione, FrameToWait);
}

void SospendiLogScript(int IndiceSave)
{
	Service(SRV_F_SospendiLogScript, IndiceSave);	
}
bool GestioneGlobaleOk(StrGlobalTrigger *pRec, bool TestEsegui)
{
	if (Service(SRV_F_GestioneGlobaleOk, pRec, TestEsegui) !=0) return true;
	return false;

}

void RiprendiLogScript(int IndiceSave) 
{
	Service(SRV_F_RiprendiLogScript, IndiceSave);
}

void GetJointAbsPositionStatic(StrItemTr4 *pItem, StrMovePosition *pMove)
{
	Service(SRV_F_GetJointAbsPositionStatic, pItem, pMove);
}
bool IsDoor(WORD Slot)
{
	if (Service(SRV_F_IsDoor, Slot) != 0) return true;
	return false;
}

int GetIndiceCanale(int IndiceAbs)
{
	return Service(SRV_F_GetIndiceCanale, IndiceAbs);
}

int CreaNuovaAzioneProgressiva(void)
{
	return Service(SRV_F_CreaNuovaAzioneProgressiva);

}

void AggiornaPosStringa(StrProgressiveAction *pAzione)
{
	
	Service(SRV_F_AggiornaPosStringa, pAzione);

}



bool InviaLog(char *pMessaggio)
{

	return SendToLog(pMessaggio);

}

void ShowMsgDebug(TCHAR *szFormat, ...)
{


    TCHAR szBuffer[512];  // Large buffer for very long filenames (like with HTTP)
	char BufTabs[80];
	int i;

	for (i=0;i<Trng.pGlobTomb4->DebugModeCounter-1;i++) {
		BufTabs[i]='\t';
	}

	BufTabs[i] = 0;

    va_list pArgs;
    va_start(pArgs, szFormat);
    vsprintf(szBuffer, szFormat, pArgs);
    va_end(pArgs);

	if (i > 0) {
		sprintf(BufferLog,"%s%s", BufTabs, szBuffer);
		InviaLog(BufferLog);
	}else {
		InviaLog(szBuffer);
	}

}
// converte da float a int con arrotondamento all'intero piu' vicino
int Float2Int(float x) 
{ 
    return (int)(x > 0.0 ? x + 0.5 : x - 0.5); 

}

// usato per variare valori di speed di PARAM_MOVE_ITEM comando basato su percentuale
short VariaPercentuale(short Valore, int Percentuale)
{
	float Parte;
	int Totale;

	if (Percentuale==0) return Valore;


	Parte = (float) Percentuale * (float) Valore;
	Parte /= 100.0f;

	Totale= Float2Int(Parte);

	if (Totale==0 && Valore !=0) {
		Totale=1;
		if (Valore < 0) Totale=-1;
	}

	return (short) Totale;

}
// inizializza valori per frog gravity 
void InitFrogVSpeed(StrProgressiveAction *pAzione, StrMoveParameters *pMove) 
{
	

	WORD VSpeed;
	short StartVSpeed;
	short IncVSpeed;
	short MaxVSpeed;

	VSpeed = (pMove->Speed >> 8) & 0xff;

	
	StartVSpeed= VariaPercentuale(-90, VSpeed);
	IncVSpeed = VariaPercentuale(5, VSpeed);
	MaxVSpeed = VariaPercentuale(64, VSpeed);

	pAzione->VetArgShort[3] = StartVSpeed;
	pAzione->VetArgShort[4] = IncVSpeed;
	pAzione->VetArgShort[5] = MaxVSpeed;


}

// converte tre byte red gree blue in un'unica word di colore
// dove ogni colore ha un'intensita di 5 bit
// nota: se TestRiduci == true vuol dire che i colori in vetbytes
// hanno 8 bit di intensita e andra' ridotta a 5
// se invece testriduci=false allora sono gia' a 5 bit
WORD ConvertiInColoreWord(BYTE* VetBytes, bool TestRiduci)
{

	int i;
	WORD Colore;
	WORD Temp;
	static WORD VetShift[3] = {10, 5, 0};
	Colore=0;

	for (i=0;i<3;i++) {
		if (TestRiduci) {
			Temp = VetBytes[i] >> 3;
		}else {
			Temp = VetBytes[i];
		}
		Temp = Temp << VetShift[i];
		Colore |= Temp;
	}
	return Colore;

}
// legge il contenuto di tomb4 per il volume e lo converte in float
// e lo salva basebass
// non viene effettuata alcuna chiamata di funzione
void AggiornaVolumeBass(void)
{
	Trng.pGlobTomb4->BaseBassHandles.VolumeMusica = 
			*Trng.pGlobTomb4->pAdr->pSetting_MusicVolume / (float) 100;
}

// calcola la massima distanza tra le diverse coordinate
// e' un calcolo molto approssimativo
// e restituisce sempre valori positivi
// nota: viene ignorata cordy
DWORD DistanzaLaraDaXZ(DWORD CordX, DWORD CordZ)
{
	DWORD DifX;
	DWORD DifZ;
	DWORD MaxDif;

	DifX = abs(CordX-Trng.pGlobTomb4->pAdr->pLara->CordX);
	DifZ = abs(CordZ-Trng.pGlobTomb4->pAdr->pLara->CordZ);

	MaxDif= DifX;


	if (DifZ > MaxDif) MaxDif = DifZ;
	return MaxDif;

}
// calcola in modo preciso la distanza tra due punti ignorando le Y
int DistanzaPrecisaXZ(DWORD X1, DWORD Z1, DWORD X2, DWORD Z2)
{
	float DifX, DifZ;

	DifX = (float) X1 - (float) X2;
	DifZ = (float) Z1 - (float) Z2;


	return Float2Int((float) sqrt(
			pow(DifX,2) +
			pow(DifZ,2)));

}

// calcola incrementi per arrivare ad una certa distanza
// nella direzione Orientamento
void CalcolaIncremento(WORD Orientamento, int *pIncX, int *pIncZ, int Distanza)
{
	int IncX, IncZ;
	int Indice;
	short * pMatriceOrientamento = (short *) 0x4B34D0;

	if (Distanza == 0) {
		*pIncX = 0;
		*pIncZ = 0;
		return;
	}

	Indice = Orientamento >> 3;
	Indice &= 0x1FFE;
	
	IncX = pMatriceOrientamento[Indice] <<	12;
	IncZ = pMatriceOrientamento[Indice+1] << 12;
	IncX = IncX >> 14;
	IncZ = IncZ >> 14;
	// modificarli sulla base di distanza richiesta
	if (Distanza != 1024) {
		IncX = ProporzioneDistanza(IncX, Distanza);
		IncZ = ProporzioneDistanza(IncZ, Distanza);
	}

	*pIncX = IncX;
	*pIncZ = IncZ;


}
// restituisce il frame di demo, oppure di contatore generale se non c'e' un demo in progresso
DWORD GetCutsceneFrame(void)
{
	if (Trng.pGlobTomb4->pBaseDemo->Status == RECF_PLAYING) return Trng.pGlobTomb4->pBaseDemo->IndexFrame;

	return *Trng.pGlobTomb4->pAdr->pFrameCounter;

}

// copia semplicmente il valore fog float da basefog a fog_distance
void SetFogStart(void)
{
	*Trng.pGlobTomb4->pAdr->pStartFog = Trng.pGlobTomb4->BaseFog.FloatFogStart;
}
void ModificaFogEnd(void)
{
	StrBaseFog *pFog;


	pFog = &Trng.pGlobTomb4->BaseFog;

	pFog->FloatFogEnd = (float) pFog->NowEndFog;


	pFog->FloatFogEnd = pFog->FloatFogEnd * 1024.0f;

	SetFogEnd();
}
// inserisce valore pFog->NowDistanceFog nella variabile tomb4,
// convertendolo in float e moltiplicandolo * 1024
// imposta anche floatstartfog di basefog
void ModificaFogStart(void)
{

	StrBaseFog *pFog;


	pFog = &Trng.pGlobTomb4->BaseFog;

	pFog->FloatFogStart = (float) pFog->NowStartFog  * 1024.0f;

	*Trng.pGlobTomb4->pAdr->pStartFog = pFog->FloatFogStart;


}
// inserisce in VetArg[0] di pAzione l'indirizzo fisico della stringa
// i cui dati di riferimento sono contenuto in pAzione
void AggiornaAdrStringa(StrProgressiveAction *pAzione)
{
	char *pChar;
	int i;
	WORD Indice;
	static WORD ** p2IndiciStringheDat = (WORD **) 0x7FD154;
	WORD *pIndiceStringheDat;
	static char **p2StringheScriptDat = (char**) 0x7FD198;
	char *pStringheScriptDat;

	pIndiceStringheDat = *p2IndiciStringheDat;
	pStringheScriptDat = *p2StringheScriptDat;

	if (pAzione->ItemIndex & 0x8000) {
		Indice= pAzione->ItemIndex & 0x7fff;

		// stringa NG
		for (i=0;i<Trng.pGlobTomb4->TotExtraStrings;i++) {
			if (Trng.pGlobTomb4->VetExtraStrings[i].Indice == Indice) {
				pChar = Trng.pGlobTomb4->VetExtraStrings[i].pTesto;
				pAzione->VetArg[0] = (int) pChar;
				return;
			}
		}
		// non e' stata trovata stringa

		sprintf(BufferLog,"ERROR: cann't locate extra ng string with index = %d", 
								pAzione->ItemIndex & 0x7fff);
		InviaLog(BufferLog);

		pAzione->ActionType = 0;
		return;

	}
	// stringa e' di tipo standard
	Indice = pIndiceStringheDat[pAzione->ItemIndex];
	pChar = &pStringheScriptDat[Indice];
	pAzione->VetArg[0] = (int) pChar;

}
void RipristinaDefaultPrintString(void)
{
	StrPrintString *pTextFormat;

	pTextFormat = &Trng.pGlobTomb4->PrintString;

	pTextFormat->BlinkSpeed = pTextFormat->DefBlinkSpeed;
	pTextFormat->Colore = pTextFormat->DefColore;
	pTextFormat->Flags = pTextFormat->DefFlags;
	pTextFormat->Posizione = pTextFormat->DefPosition;
	pTextFormat->FlagsMicro = pTextFormat->DefFlagsMicro;
	FlagMicroCaratteri = pTextFormat->DefALLFlagsMicro;



}
StrAIDataTr4 * TrovaRecordLaraPos(short Ocb, short Room)
{

	int i;



	for (i=0;i<*GlobTomb4.pAdr->pTotAIData;i++) {
		if (GlobTomb4.pAdr->pVetAIData[i].SlotAI  == 406 && 
			(GlobTomb4.pAdr->pVetAIData[i].Ocb == Ocb || Ocb==-1)) && 
			(GlobTomb4.pAdr->pVetAIData[i].Room == Room || Room==-1)){

			return &GlobTomb4.pAdr->pVetAIData[i];
		}
	}
	// non trovato
	sprintf(BufferLog,"ERROR: cann't find the LARA_START_POS with ocb=%d and room=%d", Ocb, Room);
	InviaLog(BufferLog);

	return NULL;

}
// aggiorna posizione di lara
// funziona in modo simile a AggiornaPositioneItem ma lavora 
// per lara aggiornando anche teelecamera
void AggiornaPositioneLara(DWORD CordX, int CordY, DWORD CordZ, 
						short NewRoom, bool TestVariaAltezza)
{

	short OldRoom;
	StrItemTr4 *pLara;
	short TempRoom;
	int DifferenzaAltezza;
	int Indice;
	void *pFloor;
	int SalvaTest;

	pLara = Trng.pGlobTomb4->pAdr->pLara;

	// salvare differenza relativa tra cordy e pavimento
	DifferenzaAltezza = pLara->HeightFloor - pLara->CordY;

	OldRoom = pLara->Room;
	TempRoom = NewRoom;

	pLara->CordX = CordX;
	pLara->CordY = CordY;
	pLara->CordZ = CordZ;

	pFloor=GetFloor(CordX,CordY -128,CordZ, &TempRoom);

	pLara->HeightFloor = GetHeight(pFloor,CordX, CordY,CordZ);

	if (TestVariaAltezza) {
		pLara->CordY = pLara->HeightFloor - DifferenzaAltezza;
	}


	if (OldRoom != NewRoom) {
		SalvaTest = *Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables;
		*Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables = 1;
		Indice = *Trng.pGlobTomb4->pAdr->pLaraIndex;

		ItemNewRoom(Indice, NewRoom);
		*Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables = SalvaTest;

	}
	SetMapRoom();
	InitialiseCamera();

}
// attiva o disattiva mirror che ha stanza hidden room
void CambiaStatusMirror(WORD HiddenRoom, bool TestAttiva)
{
	int i;
	int TotDisabled;
	RecordMirror *pMirror;
	int TotMirrors;

	// vedere se la stanza fa parte di quelle nascoste
	TotDisabled = Trng.pGlobTomb4->TotDisabledMirrors;

	for (i=0;i<TotDisabled;i++) {
		if (Trng.pGlobTomb4->VetDisabledMirrors[i] == HiddenRoom) break;
	}	

	if (TestAttiva== false && i == TotDisabled) {
		// bisogna inserire la stanza e non c'era
		Trng.pGlobTomb4->VetDisabledMirrors[TotDisabled] = HiddenRoom;
		Trng.pGlobTomb4->TotDisabledMirrors++;
	}

	if (TestAttiva==true && i < TotDisabled) {
		// bisognava togliere stanza e c'era
		TotDisabled--;
		if (TotDisabled > 0) {
			Trng.pGlobTomb4->VetDisabledMirrors[i] = 
					Trng.pGlobTomb4->VetDisabledMirrors[TotDisabled];
		}
		Trng.pGlobTomb4->TotDisabledMirrors=TotDisabled;

	}

	// ora cercare record mirror
	TotMirrors = Trng.pGlobTomb4->BaseMirror.TotMirror;
	pMirror = &Trng.pGlobTomb4->BaseMirror.VetMirror[0];

	for (i=0;i<TotMirrors;i++) {
		if (pMirror->HiddenRoom == HiddenRoom) {
			pMirror->TestAttivo = (WORD) TestAttiva;
			break;
		}
		pMirror++;
	}
}
// riceve in input l'indice di una stanza e restituisce l'indice effettivo
// della stanza principale nel caso che in quel momento ci fosse
// lastanza flippata
// se invece la stanza indiceroom e' proprio una stanza flippata allora
// restituisce -1
int TrovaIndiceMainRoom(int IndiceRoom)
{
	StrRoomTr4 *pRoom;
	int IndiceMappa;


	if (Trng.pGlobTomb4->FlipMapRooms.VetRoomMain[IndiceRoom] != -1 ) {
	
		// e' stato scelto un indice corrispondente 
		// corrispondente alla posizione di una stanza flippata
		// impedire qualunque accesso 
		return -1;
	}
	// ora so che questa NON e' una stanza flippata nascosta
	// vedere pero' se e' una stanza con flipmap
	pRoom = &Trng.pGlobTomb4->pAdr->pVetRooms[IndiceRoom];
	if (pRoom->AlternateRoom == -1) {
		// non e' una stanza con flipmap
		// tutto ok, usare indice normale
		return IndiceRoom;
	}
	// e' una stanza con flipmap
	// ora controllarese e' stata flippata per cui nella posizione di
	// stanza main ora c'e' quella flippata
	IndiceMappa = pRoom->FlipMapIndex;

	if (Trng.pGlobTomb4->pAdr->Remap.pVetEnabledFlipMaps[IndiceMappa]) {
		// si, e' stata spostata, quindi il vero indice della stanza
		// principale e' quello dell'alternate room
		return pRoom->AlternateRoom;
	}
	// non e' stata attivata quindi usare indice normale
	return IndiceRoom;

}
// chiamata quando si vuole terminare un CAMERA creato usando
// trigger action per attivazione
void RipristinoBlocchiCamera(void)
{
#ifdef DEBUG_ON
	InviaLog("RipristinoBlocchiCamera()");
#endif
	Trng.pGlobTomb4->TestDisableFeatures &= ~ (DF_GUARDA + DF_COMBAT_CAMERA);


}
// modifica tutti i valori di tomb4 dove e' presente un riferimento
// a worldfarview usando come valore	
// se TestHardware=true modifica tutto, sia valori hard che soft
// nel caso invece sia soft ignora quelli hard e modifica quelli soft
void ModificaWorldFarView(WORD NumBlocchi, bool TestHardware)
{
	float WorldFar;
	float *pFloat;
	DWORD *pDword;
	BYTE *pByte;

	DWORD DWord;
	// se valore 0xffff allora ignorare
	if (NumBlocchi == SCRIPT_IGNORE) return;
	// levelfarview
	WorldFar = (float) (NumBlocchi * 1024);
	// imposta valore SoftFarWorldView
	pFloat =(float *) 0x4B249C;
	*pFloat = WorldFar;




	if (TestHardware == false) return;

	// imposta valori per hardclicp
	DWord = NumBlocchi * 1024;
	// primo punto
	pDword = (DWORD*) 0x475107;
	*pDword = DWord;
	// secondo punto
	pDword = (DWORD*) 0x48CE33;
	*pDword = DWord;

	// impostanumero blocco push 14h
	pByte = (BYTE *) 0x47510C;
	*pByte = (BYTE) NumBlocchi;

	pByte = (BYTE *) 0x48CE38;
	*pByte = (BYTE) NumBlocchi;


}
void EseguiAnimazione(WORD NumeroAnimazione, int IndiceSlot, 
					  bool TestUsaNextId)
{

	int StartAnim;
	int IndiceAnim;



	if (IndiceSlot >0) {
		if ((Trng.pGlobTomb4->pAdr->pVetSlot[IndiceSlot].Flags  & 0x01)==0) {
			sprintf(BufferLog,"ERROR: Performing animation %d : The slot %d is missing",
				NumeroAnimazione, IndiceSlot);
			InviaLog(BufferLog);
			return;
		}

	}

	StartAnim = Trng.pGlobTomb4->pAdr->pVetSlot[IndiceSlot].IndexFirstAnim;
	IndiceAnim = StartAnim+NumeroAnimazione;

	if (Trng.pGlobTomb4->pAdr->pLara->AnimationNow == IndiceAnim) return;

	if (Trng.pGlobTomb4->pAdr->pVetAnimations[IndiceAnim].FrameSize == 0) {
		sprintf(BufferLog,"ERROR: missing animation number %d for %d slot", 
			NumeroAnimazione, IndiceSlot);
		InviaLog(BufferLog);
		return;
	}


	// animazione lara normale

	Trng.pGlobTomb4->pAdr->pLara->FrameNow = 
			Trng.pGlobTomb4->pAdr->pVetAnimations[IndiceAnim].FrameStart;

	if (IndiceSlot==0) {

		Trng.pGlobTomb4->pAdr->pLara->StateIdCurrent = 
				Trng.pGlobTomb4->pAdr->pVetAnimations[IndiceAnim].StateId;

		if (TestUsaNextId==true ) { 
				Trng.pGlobTomb4->pAdr->pLara->StateIdNext = 
							Trng.pGlobTomb4->pAdr->pLara->StateIdCurrent;
		}
	}
	
	Trng.pGlobTomb4->pAdr->pLara->AnimationNow = IndiceAnim;
	


}
// restituisce la posizione attuale del suono nel canale pCanale
DWORD TrovaPosizioneSuono(StrCanaleBass *pCanale)
{
	
	DWORD Offset;
	StrBassHandles *pBass;

	pBass = &Trng.pGlobTomb4->BaseBassHandles;

	Offset = (DWORD) pBass->Proc.BASS_ChannelGetPosition(
				pCanale->Canale, BASS_POS_BYTE);
	return Offset;

}
// crea il corrispettivo di Cord che era costruito per schermo SchermoRef
// col nuovo valore di schermo attuale SizeSchermo
float RapportoFloatSchermo(int SizeSchermo, float SchermoRef, float Cord)
{
	float Alfa;

	SalvaDimensioneSchermo();

	Alfa = (float) SizeSchermo;
	Alfa = Alfa / SchermoRef;

	Alfa = Alfa * Cord;
	return Alfa;
}

// resitituisce la struttura del diario corrispondente a ID di input
// se non lo trovoa visualizza errore in log e restituisce NULL
StrBaseDiario* GetDiarioConID(WORD Id)
{

	int i;

	for (i=0;Trng.pGlobTomb4->BaseDiari.TotDiari;i++) {
		if (Trng.pGlobTomb4->BaseDiari.VetBaseDiario[i].ID_Diario == Id) {
			return &Trng.pGlobTomb4->BaseDiari.VetBaseDiario[i];
		}
	}

	sprintf(BufferLog,"ERROR: cann't find Diary=%d, ...", Id);
	InviaLog(BufferLog);
	return NULL;
	


}
// modifcia il colore della fog chiamando directx
// e salva il valore anche in tutte le variabili globali
void ModificaColoreFog(BYTE Rosso, BYTE Verde, BYTE Blue)
{

	BYTE *pByte;


	SetFogColor(Rosso,Verde,Blue);

	pByte = (BYTE *) 0x7F7764;   // Savegame_21B_FogBulbColor
	pByte[2] = Rosso;
	pByte[1] = Verde;
	pByte[0] = Blue;

	pByte = (BYTE *) 0x4BF538;  // Colore_FOG
	pByte[2] = Rosso;
	pByte[1] = Verde;
	pByte[0] = Blue;
	
}
__declspec(naked) void ModificaHardwareFog(void)
{
	__asm {
	pushad



	mov eax, 7538B9h    ;pDirect3d_Principale
	mov  eax, dword ptr [eax] 
	push eax
	MoveOffset(eax, GlobalFields.BaseFog.TestHardFogEnabled)
	mov ecx, dword ptr [eax]
	pop eax
	and ecx, 0ffh
	push ecx	;attiva o disattiva 
	push 1ch       ; D3DRENDERSTATE_FOGENABLE 
	push eax 
	mov  edx, [eax] 
	call dword ptr [edx+58h]  ;SetRenderState
	push eax 
	mov eax, 48F8A0h    ;DXAttempt
	call eax
	add esp,4

	popad
	retn
	}

}
// imposta in variabile globale tomb4 il valore float per la max distanza
// per fogbulbs
// legge l'input da BaseFog.NowFogBulbDistance
void ModificaFogBulbDistance(void)
{
	float *pFloat;
	float ValFog;
	float Temp;

	Temp = (float) Trng.pGlobTomb4->BaseFog.NowFogBulbDistance;
	ValFog = (float) (1048576.0f * pow(Temp,2));
	pFloat = (float*) 0x4A7374;
	*pFloat = ValFog;
}
// restituisce un codice esteso e valido del Codice per variabile
// del campo timer che e' poi quello del combo di ngle
WORD ConvertiCodVar(WORD CodiceTimer)
{
	WORD Codice;

	Codice = CodiceTimer;
	if (Codice == VAR_SPECIAL_CURRENT_VALUE) Codice = VAR_TYPE_CURRENT_VALUE;
	if (Codice == VAR_SPECIAL_INPUT_NUMBER) Codice = VAR_TYPE_INPUT_NUMBER;
	return Codice;
}
// restituisce il puntatore al testo di una delle variabili
char *LeggiVariabileTesto(WORD CodiceVariabile)
{
	int Indice;


	// vedere se e' l'ultima testo inserito in inputtext
	if (CodiceVariabile == VAR_TYPE_INPUT_TEXT) {
		return Trng.pGlobTomb4->pBaseVariableTRNG->Globals.LastInputText;
	}

	if (CodiceVariabile == VAR_TYPE_BIG_TEXT) {
		return Trng.pGlobTomb4->pBaseVariableTRNG->Globals.TextBig;
	}

	Indice = CodiceVariabile & VAR_MASK_INDEX;
	return Trng.pGlobTomb4->pBaseVariableTRNG->Globals.VetTextVar[Indice].Text;

}
// restituisce puntaotre a stringa ng con valore indice
// se non la trova restituisce NULL
char *GetStringaNG(int Indice)
{
	int i;
	Indice &= 0x7fff;

	// stringa NG
	for (i=0;i<Trng.pGlobTomb4->TotExtraStrings;i++) {
		if (Trng.pGlobTomb4->VetExtraStrings[i].Indice == Indice) {
			return Trng.pGlobTomb4->VetExtraStrings[i].pTesto;

		}
	}
	return NULL;

}
char *FormattaTimer(int Valore) 
{
	static char Buffer[80];
	int Minuti,Secondi;
	float Resto;
	char MiniBuf[60];
	char BufDecimi[60];
	int i;
	int j;
	char MioCar;
	bool TestPreso;


	// ok, ora calcolare tempo
	Minuti = Valore / (FRAME_SECONDO * 60);
	Secondi = Valore % (FRAME_SECONDO * 60);
	
	Resto = (float) Secondi / FRAME_SECONDO;
	sprintf(MiniBuf, "%0.1f", Resto);
	Secondi /= FRAME_SECONDO;

	// ora prendere le due cifre decimali
	j=0;
	i=0;
	TestPreso=false;
	while (MiniBuf[i]) {
		MioCar = MiniBuf[i];
		if (TestPreso) {
			BufDecimi[j++] = MioCar;
			break;
		}else {
			if (MioCar == '.' || MioCar == ',') TestPreso=true;
		}
		i++;
	}

	BufDecimi[j] =0;


	sprintf(Buffer, "%02d:%02d:%s", Minuti, Secondi, BufDecimi);
	return Buffer;


}
// esegue calcolo a ritroso
// da indice interno di tomb, restituisce l'indice usato in ngle
int GetNgleIndice(int TombIndice)
{
	int i;

	for (i=0;i<6000;i++) {
		if (Trng.pGlobTomb4->VetRemapObjects[i] == TombIndice) return i;
	}
	return TombIndice;

}

// restituisce un numero casuale compreso tra 0 e Massimo-1
int Casuale(int Massimo)
{
	int Numero;



	Numero = rand();
	if (Numero >= Massimo) {
		Numero = Numero % Massimo;
	}
		
	return Numero;

}
// esegue 
void SwapMeshItem(short SlotItem, WORD SlotSwap)
{
	;
	StrFlipSwapMesh SwapMesh;
	int i;
	int IndiceMeshItem;
	int IndiceMeshSlot;
	int SlotSource;
	int TotMesh;
	StrMeshTr4* Valore;

	// impostare tutti i dati da salvare

	SwapMesh.SlotItem = SlotItem;
	SwapMesh.SlotAltroSlot = SlotSwap;
	SwapMesh.TipoSwapMesh = 0; // normale

	// salvare l'esecuzione di questo swap
	SalvaRecordSwapMesh(&SwapMesh);

	SlotSource = SlotItem;


	// ok, ora posso usare il codice quasi identico a quello standard

	TotMesh = Trng.pGlobTomb4->pAdr->pVetSlot[SlotSource].TotMesh;
	IndiceMeshItem = Trng.pGlobTomb4->pAdr->pVetSlot[SlotSource].IndexFirstMesh;
	IndiceMeshSlot = Trng.pGlobTomb4->pAdr->pVetSlot[SlotSwap].IndexFirstMesh;


	for (i=0;i<TotMesh;i++) {
		// salva mesh originale di lara
		// invertire le mesh
		Valore = Trng.pGlobTomb4->pAdr->VetMeshPointer[IndiceMeshItem];

		Trng.pGlobTomb4->pAdr->VetMeshPointer[IndiceMeshItem] =
				Trng.pGlobTomb4->pAdr->VetMeshPointer[IndiceMeshSlot];
		Trng.pGlobTomb4->pAdr->VetMeshPointer[IndiceMeshSlot] = Valore;
	
		IndiceMeshItem+=2;
		IndiceMeshSlot+=2;

	}

}
// oopia in basestandby il record script standby con indice 
void ImpostaStandBy1(int Indice)
{
	StrBaseStandBy *pBStand;
	StrScriptStandBy *pStand;

	pBStand = &Trng.pGlobTomb4->BaseStandBy;
	pStand = &Trng.pGlobTomb4->BaseStandBy.VetStandBy[Indice];

	pBStand->TestPresente=true;
	pBStand->AudioTrack = pStand->AudioTrack;
	pBStand->IdNow = pStand->IdStandBY;
	pBStand->Distanza = pStand->Distanza;
	pBStand->Flags = pStand->Flags;
	pBStand->IdTriggerGroupEnd = pStand->IdTriggerGroupEnd;
	pBStand->IdTriggerGroupStart = pStand->IdTriggerGroupStart;
	pBStand->IndicePrimaStringa = pStand->FirstText;
	pBStand->LimiteCicli = pStand->LimiteCicli;
	pBStand->OrientSpeed  = pStand->OrientSpeed;
	pBStand->TestFlipEffect =false;
	pBStand->TestIniziato=false;
	pBStand->TipoStandby = pStand->TipoStandby;
	pBStand->TotStringhe = pStand->NumTexts;
	pBStand->VerticalOrient = pStand->VerticalOrient;

}
// partendo da puntatore ad un record item restituisce l'indice
int GetIndiceItem(DWORD OffsetItem)
{
	static DWORD* BaseVetItems = (DWORD*) 0x7FE16C;

	return (OffsetItem - *BaseVetItems) / 0x15F6;
}
bool AggiornaStatici(StrScaleItem *pScale, WORD Percentuale)
{
	int TotIndici;
	WORD *pVetIndici;
	int i;
	int IndiceStatic;
	int IndiceRoom;
	int Indice;
	StrMeshInfo *pStatic;

	// trovare eventuale lista di indice statics
	TotIndici=0;
	if (pScale->Flags & FSCA_ITEMGROUP_INDEX) {
		// localizzare indici
		i = Trng.pGlobTomb4->BaseItemGroup.VetID[pScale->ItemIndex];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: in the Parameters=PARAM_SCALE_ITEM, %d command the index [%d] points to a missing ItemGroup= command",
				pScale->IdScale , pScale->ItemIndex);
			InviaLog(BufferLog);
			return false;
		}

		TotIndici = Trng.pGlobTomb4->BaseItemGroup.VetItemGroup[i].TotIndici;
		pVetIndici = (WORD *) &Trng.pGlobTomb4->BaseItemGroup.VetItemGroup[i].VetIndici[0];
	}else {
		TotIndici=1;
		pVetIndici = (WORD *) &pScale->ItemIndex;
	}
	// ok, ora impostare la percentuale richiesta per tutti
	// gli statici
	for (i=0;i<TotIndici;i++) {
		Indice = pVetIndici[i];
		IndiceRoom = Trng.pGlobTomb4->VetRemapStatics[Indice].IndiceRoom;
		IndiceStatic = Trng.pGlobTomb4->VetRemapStatics[Indice].IndiceStatic;
		if (IndiceStatic == -1) {
			sprintf(BufferLog,"ERROR: missing static with global index = %d",
				Indice);
			InviaLog(BufferLog);
			return false;
		}
		pStatic = &Trng.pGlobTomb4->pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];

		// prima eliminare gli ocb di pertinenze
		pStatic->OCB &= OCBS_MASK_SCALABLE;
		// aggiungere ocb di ridimensionamento
		pStatic->OCB |= OCBS_SCALABLE;
		// ora aggiungere fattore di ridimensionamento
		pStatic->OCB |= (Percentuale << 2);
		pStatic->OCB |= OCBS_SALVARE;
		
	}
	return true;

}
// clacola i valori per effetto attuale
// se testsetup=true imposta i valori all'inizio
// altrimenti aggiorna i valori
void CalcolaSpriteZoom(StrProgressiveAction *pAzione, StrShowSprite *pSprite, bool TestSetup)
{
	float InizioX, InizioY, InizioSizeX, InizioSizeY;
	float IncX, IncY;

	// prima clacolare dimensione iniziale 10 volteminore
	// fare tutto in microunita

	InizioSizeX = (float) pSprite->Posizione.right / 10;
	InizioSizeY = (float) pSprite->Posizione.bottom / 10;

	// ora a seconda se setup o esecuione 
	if (TestSetup) {
		// e' solo setup: impostare anche orgx,orgy di inizio e uscire
		// ora devo calcolare la coordinata iniziale
		// devo partire dal centro dell'immagine in posizione finale
		InizioX = (float) pSprite->Posizione.left  + ((float) pSprite->Posizione.right  / 2);
		InizioY = (float) pSprite->Posizione.top  + ((float) pSprite->Posizione.bottom  / 2);

		// adesso aumentare a questi orgx orgxy la meta' della dimensione
		// iniziale
		InizioX -= (InizioSizeX / 2);
		InizioY -= (InizioSizeY / 2);

		pAzione->VetArgFloat[0] = InizioX;
		pAzione->VetArgFloat[1] = InizioY;
		
		pAzione->VetArgFloat[2] = InizioSizeX;
		pAzione->VetArgFloat[3] = InizioSizeY;
		// impostare  gli incrementi per posizione
		pAzione->VetArgFloat[4] = ((float) pSprite->Posizione.left - pAzione->VetArgFloat[0]) / 
				(float) pSprite->ExtraValue;
		pAzione->VetArgFloat[5] = ((float) pSprite->Posizione.top  - pAzione->VetArgFloat[1]) / 
				(float) pSprite->ExtraValue;
	}else {
		// siamo in esecuzione
		// calcolare solo gli incrementi da applicare e aggfiornare il tutto
		// aggiornare origine
		pAzione->VetArgFloat[0] += pAzione->VetArgFloat[4];
		pAzione->VetArgFloat[1] += pAzione->VetArgFloat[5];
		// ora claoclare l'incrmeento per la dimensioene
		IncX = ((float) pSprite->Posizione.right - InizioSizeX) / (float) pSprite->ExtraValue;
		IncY = ((float) pSprite->Posizione.bottom  - InizioSizeY) / (float) pSprite->ExtraValue;

		pAzione->VetArgFloat[2] += IncX;
		pAzione->VetArgFloat[3] += IncY;
	}
	

}
// calcola la massima distanza tra le diverse coordinate
// e' un calcolo molto approssimativo
// e restituisce sempre valori positivi
DWORD DistanzaLaraDaXYZ(DWORD CordX, int CordY, DWORD CordZ)
{
	DWORD DifX;
	DWORD DifY;
	DWORD DifZ;
	DWORD MaxDif;

	DifX = abs(CordX-Trng.pGlobTomb4->pAdr->pLara->CordX);
	DifY = abs(CordY-Trng.pGlobTomb4->pAdr->pLara->CordY);
	DifZ = abs(CordZ-Trng.pGlobTomb4->pAdr->pLara->CordZ);

	MaxDif= DifX;

	if (DifY > MaxDif) MaxDif = DifY;
	if (DifZ > MaxDif) MaxDif = DifZ;
	return MaxDif;

}


bool CaricaDemo(int NumeroDemo)
{
	
	FILE *pFile;
	StrRecording *pRecording;
	char NomeFile[256];

	sprintf(NomeFile, "data\\demo%d.pak", NumeroDemo);

	pFile = fopen(NomeFile, "rb");
	if (pFile==NULL) {
		sprintf(BufferLog, "ERROR: can't load %s file", NomeFile);
		InviaLog(BufferLog);
		return false;
	}

	Trng.pGlobTomb4->DemoNumberLoaded= NumeroDemo;

	pRecording= Trng.pGlobTomb4->pBaseDemo;

	fread(&pRecording->Status, 2, 1, pFile);
	fread(&pRecording->Version, 2, 1, pFile);
	fread(&pRecording->DatiLara, sizeof(StrSalvaDatiLara),1, pFile);
	fread(&pRecording->IndexFrame, 4,1, pFile);
	fread(&pRecording->TotFrames, 4, 1, pFile);
	fread(&pRecording->VetFrames[0], 4, pRecording->TotFrames, pFile);

	fclose(pFile);
	
	pRecording->Status = RECF_STOP;
	pRecording->IndexFrame=0;
	return true;

}
// riprstina fixed camera usata, e riattiva camera standard per seguire lara
void LiberaCutsceneCamera(void)
{

	StrAdrCamera *pCam;
	StrCutsceneCamera *pCut;

	pCut = &Trng.pGlobTomb4->pBaseCutscene->BaseCamera;

	pCut->TestAttiva =false;
	if (pCut->TestAllocata==false) return;
	Trng.pGlobTomb4->TestDisableFeatures &= ~DF_GUARDA;	
	// ripristinare dati originali
	*pCut->pCamera = pCut->Original;
	pCut->TestAllocata=false;

	pCam = &Trng.pGlobTomb4->pAdr->Camera;

	*pCam->pModeCameraNext =0;
	*pCam->pIndexCameraNext = -1;
	*pCam->pTargetCameraNext = NULL;
	*pCam->pSecondsTimerCamera =0;


}
// scambia gruppo di animazioni sulla base di dati in Parameters=PARAM_SWAP_ANIMATIONS, SwapId
// per slot Slot
void SwapAnimations(WORD Slot, WORD SwapId)
{
	StrSwapAnim *pSwap;
	int i;
	StrAnimationTr4 SwapAnim;
	int FirstAnimSlot;
	StrAnimationTr4 *pVet;


	i= Trng.pGlobTomb4->pBaseSwapAnim->VetId[SwapId];
	if (i==-1) {
		sprintf(BufferLog, "ERROR: cann't find Parameters=PARAM_SWAP_ANIMATIONS, %d script command",
			SwapId);
		InviaLog(BufferLog);
		return;
	}
	pSwap = &Trng.pGlobTomb4->pBaseSwapAnim->VetSwapAnim[i];

	FirstAnimSlot= Trng.pGlobTomb4->pAdr->pVetSlot[Slot].IndexFirstAnim;

	pVet = &Trng.pGlobTomb4->pAdr->pVetAnimations[FirstAnimSlot];

	for (i=0;i<pSwap->NumberOfAim;i++) {
		SwapAnim = pVet[pSwap->FirstSourceAnim+i];
		pVet[pSwap->FirstSourceAnim+i] = pVet[pSwap->FirstTargetAnim+i];
		pVet[pSwap->FirstTargetAnim+i] = SwapAnim;
	}


}
// ripristina proccontrol originale di oggetto
void RestoreActor(StrItemTr4 *pOggetto, int ItemIndex) 
{
	WORD Slot;
	int i;
	StrSlot *pSlot;
	StrBaseSaveActors *pActors;
	StrBaseCutscene *pCut;

	pActors=&Trng.pGlobTomb4->pBaseCutscene->BaseSaveActors;
	Slot=pOggetto->SlotID;
	pSlot= &Trng.pGlobTomb4->pAdr->pVetSlot[Slot];

	for (i=0;i<pActors->TotActors;i++) {
		if (pActors->VetActors[i].Slot == Slot) {
			pSlot->pProcControl = pActors->VetActors[i].pOriginalCtrl;
			break;
		}
	}
	
	// se questo attore aveva basic collision rimuoverlo dall'elenco
	pCut=Trng.pGlobTomb4->pBaseCutscene;

	for (i=0;i<pCut->TotBasicCollision;i++) {
		if (pCut->VetBasicCollisionIndices[i] == ItemIndex) break;
	}
	// eliminare posizone (i) 
	if (i < pCut->TotBasicCollision ) {
		pCut->TotBasicCollision--;
		pCut->VetBasicCollisionIndices[i] = pCut->VetBasicCollisionIndices[pCut->TotBasicCollision];
		// se era attore principale o secondario, togliere l'indice
		if (Trng.pGlobTomb4->pBaseCutscene->LeadingActorIndex == ItemIndex) Trng.pGlobTomb4->pBaseCutscene->LeadingActorIndex=-1;
		if (Trng.pGlobTomb4->pBaseCutscene->ExtraActorIndex == ItemIndex) Trng.pGlobTomb4->pBaseCutscene->ExtraActorIndex=-1;

	}

}
// chiama in direzione opposta tutti gli swapping animation che erano stati fatti
void RestoreAllAnimSwap(void)
{
	int i;
	StrMemSwapAnim *pSwapMem;

	for (i=Trng.pGlobTomb4->pBaseMemSwapAnim->TotMemSwap-1;i>=0;i--) {
		pSwapMem= &Trng.pGlobTomb4->pBaseMemSwapAnim->VetMemSwapAnim[i];
		SwapAnimations(pSwapMem->Slot, pSwapMem->IdParamSwap);

	}

	Trng.pGlobTomb4->pBaseMemSwapAnim->TotMemSwap=0;


}
// suona un cd con bass senza usare in alcun
// modo, ne' influenzare, la gestione dei suoni di tomb
// il numero di canale dovrebbe partire da 2 fino a 4
void PlayExtraCD(short NumeroCd, int Canale, int Loop)
{
	if (Trng.pGlobTomb4->BaseBassHandles.TestPresente ==false) return;
	Trng.pGlobTomb4->BaseBassHandles.CanaleNow= Canale;
	GestioneCdPlay(NumeroCd, Loop);


}
// viene chiamato quando si trova trigger fmv
// in NumeroFmv c'e' il numero
// imposta in variabile globale Trng.pGlobTomb4->BaseFMV.IndiceFmvAttivato
void ElaboraTriggerFMV(int NumeroFmv)
{
	int i;
	BYTE *pFlagWindow = (BYTE*) 0x7538FD;

	if (Trng.pGlobTomb4->BaseFMV.VetFmvEseguiti[NumeroFmv]) return;

	for (i=0;i<Trng.pGlobTomb4->BaseFMV.TotFmv;i++) {
		if ((Trng.pGlobTomb4->BaseFMV.VetFmv[i] & 0x7f) == NumeroFmv) {
			PreparaLancioFilmato(Trng.pGlobTomb4->BaseFMV.VetFmv[i]);
			Trng.pGlobTomb4->BaseFMV.VetFmvEseguiti[NumeroFmv] =1;

			return;
		}
	}
	Trng.pGlobTomb4->BaseFMV.VetFmvEseguiti[NumeroFmv]=1;

	sprintf(BufferLog,"ERROR: Not found in script.dat FMV with number %d", NumeroFmv);
	InviaLog(BufferLog);


}
// -> last added function bodies
// -> last added function bodies
// restituisce true se il tasto virtualkey e' attualmente premuto
bool TastoVKPremuto(int VirtualKey) 
{
	if (GetAsyncKeyState(VirtualKey) < 0) 
		return true;
	else
		return false;

}
// -> last added function bodies
// -> last added function bodies
// -> last added function bodies
// verifica se c'e' l'esecuzione del singolo globaltrigger 
// col dato parametro
bool VerificaSingleGlobalTrigger(short GlobalTrigger, short Parametro, bool TestIgnoraParametro)
{
	int i;
	StrGlobalTrigger *pRec;
	int Tot;
	bool TestEsegui;
	bool TestTrovato;
	bool TestAlmenoUno;
	bool TestEsito;

	Tot = Trng.pGlobTomb4->pBaseGlobalTriggers->TotTriggers;
	pRec = &Trng.pGlobTomb4->pBaseGlobalTriggers->VetTriggers[0];

	TestAlmenoUno=false;

	for (i=0;i<Tot;i++) {
		TestEsegui=false;
		TestTrovato=false;

		if ((pRec->Flags & FGT_DISABLED) == 0) {

			// vedere se va disattivato il log per queso global trigger
			if (pRec->Flags & FGT_HIDE_IN_DEBUG) {
				SospendiLogScript(BREAK_GLOBAL_TRIGGER);
			}
			if  (pRec->GlobalTrigger == GlobalTrigger) {

				TestTrovato=true;
				if (TestIgnoraParametro) {
					TestEsegui=true;
				}else {

					if (Parametro == pRec->Parameter) TestEsegui=true;
				}

				if (pRec->Flags & FGT_NOT_TRUE) TestEsegui ^= 1;

			}

			if (TestTrovato) {
				TestEsito = GestioneGlobaleOk(pRec,TestEsegui);
				if (TestEsito) {
					if ((pRec->Flags & FGT_REPLACE_MANAGEMENT)!=0 || 
						pRec->GlobalTrigger != GT_SELECTED_INVENTORY_ITEM) TestAlmenoUno=true;
				}

			}
			if (pRec->Flags & FGT_HIDE_IN_DEBUG) {
				RiprendiLogScript(BREAK_GLOBAL_TRIGGER);
			}
		}
		pRec++;
	}

	return TestAlmenoUno;
}
// -> last added function bodies
// -> last added function bodies
void TrovaAbsPosition(StrItemTr4 *pItem, StrMovePosition *pMove, int NMesh)
{

	if (pItem->SlotID == SLOT_STATIC) {
		// e' uno static
		GetJointAbsPositionStatic(pItem, pMove);
		return;
	}

	//e' un moveable
	GetJointAbsPosition(pItem, pMove, NMesh);
	return;

}

// ogni volta che viene mosso un moveable con un action o flipeffect speciale
// viene chiamata questa procedura per memorizzare il suo indice
void AggiungiItemMosso(WORD Indice)
{
	int i;
	WORD TotItem;

	// prima vedere se e' gia' stato salvato nel qual
	// caso si puo' uscire subito
	TotItem = Trng.pGlobTomb4->BaseSalvaCoordinate.TotSalvati;
	for (i=0;i<TotItem;i++) {
		if (Trng.pGlobTomb4->BaseSalvaCoordinate.VetIndici[i] == Indice) return;
	}

	// non ancora salvato


	// ok, salvarlo
	Trng.pGlobTomb4->BaseSalvaCoordinate.VetIndici[TotItem] = Indice;
	Trng.pGlobTomb4->BaseSalvaCoordinate.TotSalvati ++;

}

// modifica posizione item da quella attuale  a quella delle nuove 
// coordinate fornite
// questa funzione va usata dopo aver ricaricato da savegame posizione
// aniating, oppure dopo aver mosso animating
void AggiornaPosizioneItem(short ItemIndex, 
						   DWORD CordX, int CordY, 
						   DWORD CordZ, int ExtraY)
{
	short NewRoom;
	short OldRoom;
	StrItemTr4 *pItem;
	BYTE SalvaTest;
	void *pFloor;


	pItem = &Trng.pGlobTomb4->pAdr->pVetItems[ItemIndex];

	OldRoom = pItem->Room;

	pItem->CordX = CordX;
	pItem->CordY = CordY;
	pItem->CordZ = CordZ;

	NewRoom = OldRoom;

	pFloor=GetFloor(CordX,CordY + ExtraY,CordZ, &NewRoom);
	pItem->HeightFloor = GetHeight(pFloor,CordX,
									CordY,CordZ);

	if (OldRoom != NewRoom && IsDoor(pItem->SlotID) == false) {
		SalvaTest = *Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables;

		*Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables = 1;
		
		ItemNewRoom(ItemIndex, NewRoom);
		*Trng.pGlobTomb4->pAdr->pTestWorkingOnMoveables = SalvaTest;


	}
	
}


// ferma sound effect
void FermaSoundEffect(int IndiceAbs)
{
	int IndiceCanale;
	StrZonaSound *pZonaSound = (StrZonaSound*) 0x7F7100;

	IndiceCanale=GetIndiceCanale(IndiceAbs);
	if (IndiceCanale < 0) return;
	StopEffect(IndiceCanale);
	pZonaSound[IndiceCanale].IndiceRel = -1;
}


// -> last added function bodies
