
#include "stdafx.h"
#include "bass.h"
#include "stdio.h"
#include <math.h>

#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll

#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "macros.h"  // definition of macros



// ------- constant data and global variables -------------
// #TAG_ADDING_NEW_VARIABLES#
extern char BufGlobalTimer[];
extern char BufLocalTimer[];
extern DWORD Splash_Rain;
extern float Rain_Float_2;
extern int VetIncTrasparenza[] ;
extern BYTE FlagMicroCaratteri;
extern int VetExtraLaraSlots[];
//...
extern StrGlobaliTomb4 GlobTomb4;
extern char BufferLog[];
// +++++++++++ GLOBAL DATA +++++++++++++++++++++
int VetCommandKey[] = {CMD_ALL, CMD_UP, CMD_DOWN, CMD_LEFT, CMD_RIGHT,
		CMD_DUCK, CMD_DASH, CMD_WALK, CMD_JUMP, CMD_ACTION, CMD_DRAW_WEAPON,
		CMD_USE_FLARE, CMD_LOOK, CMD_ROLL, CMD_INVENTORY, CMD_STEP_LEFT,
		CMD_STEP_RIGHT, CMD_PAUSE, CMD_SAVE_GAME, CMD_LOAD_GAME, 
		CMD_WEAPON_KEYS};	

int VetTempi[] = {0, 100, 200, 300, 400, 500, 700, 1000, 2000,
		3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 15000,
		20000, 25000, 30000, 35000}; 
// ------- declare external functions

// #TAG_ADDING_NEW_DECLARES#
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
extern TYPE_phd_PushMatrix phd_PushMatrix;
extern TYPE_phd_TranslateAbs phd_TranslateAbs;
extern TYPE_phd_RotYXZ phd_RotYXZ;
extern TYPE_phd_PutPolygons_train phd_PutPolygons_train;
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
void CambioIntensitaWeather(void);
// -> last declare line
// -> last declare line
void InitFrogVSpeed(StrProgressiveAction *pAzione, StrMoveParameters *pMove);
// -> last declare line
// -> last declare line
// -> last declare line
void ShowMsgDebug(TCHAR *szFormat, ...);
void DoWeather(void);
extern TYPE_mGetAngle mGetAngle;
short VariaPercentuale(short Valore, int Percentuale);
WORD ConvertiInColoreWord(BYTE* VetBytes, bool TestRiduci);
void AggiornaVolumeBass(void);
bool AddTabLogScript(void);
void SubTabLogScript(void);
DWORD DistanzaLaraDaXZ(DWORD CordX, DWORD CordZ);
int DistanzaPrecisaXZ(DWORD X1, DWORD Z1, DWORD X2, DWORD Z2);
extern TYPE_GetWaterHeight GetWaterHeight;
int Float2Int(float x);
void CalcolaIncremento(WORD Orientamento, int *pIncX, int *pIncZ, int Distanza);
extern TYPE_SetupRipple SetupRipple;
extern TYPE_Splash Splash;
extern TYPE_GetBestFrame GetBestFrame;
void AggiornaCutsceneCamera(StrCutsceneCamera *pCut);
DWORD GetCutsceneFrame(void);
// -> last declare line
WORD GestionePickups(WORD Slot, int Operazione, short Valore);
extern TYPE_AddDisplayPickup AddDisplayPickup;
int CreaNuovaAzioneProgressiva(void);
bool InviaLog(char *pMessaggio);
void SetFogStart(void);
void ModificaFogEnd(void);
void ModificaFogStart(void);
extern TYPE_LaraBurn LaraBurn;
void GestioneCompletedProgActions(int ActionType, int ActionIndex);
void AggiornaAdrStringa(StrProgressiveAction *pAzione);
void AggiornaPosStringa(StrProgressiveAction *pAzione);
extern TYPE_S_CDPlay S_CDPlay;
void StopBassSuoni(int NumeroCanale);
extern TYPE_SoundEffect SoundEffect;
extern TYPE_StopSoundEffect StopSoundEffect;
extern TYPE_StopAllSounds StopAllSounds;
void RipristinaDefaultPrintString(void);
StrAIDataTr4 * TrovaRecordLaraPos(short Ocb, short Room);
void AggiornaPositioneLara(DWORD CordX, int CordY, DWORD CordZ, 
						short NewRoom, bool TestVariaAltezza);
extern TYPE_SetScreenFadeIn SetScreenFadeIn;
extern TYPE_SetScreenFadeOut SetScreenFadeOut;
void AddScriptAddEffect(int IndiceOggetto, short IdEffetto, WORD ExtraTimer);
void CambiaStatusMirror(WORD HiddenRoom, bool TestAttiva);
extern TYPE_KillEffect KillEffect;
extern TYPE_KillItem KillItem;
void SalvaRecordSwapMesh(StrFlipSwapMesh *pSwap);
void SwapMeshLara(int IndicePerSlot, int TipoSwapMesh);
extern TYPE_GetFloor GetFloor;
extern TYPE_GetHeight GetHeight;
extern TYPE_TestTriggers TestTriggers;
int TrovaIndiceMainRoom(int IndiceRoom);
int EseguiTriggerGroup(short IdTriggerGroup);

void RipristinoBlocchiCamera(void);
extern TYPE_FlipMap FlipMap;
int EsecuzioneActionTrigger(WORD PluginID, WORD AzioneTimer, int ItemIndex, WORD Flags);
void ModificaWorldFarView(WORD NumBlocchi, bool TestHardware);
extern TYPE_ShatterObject ShatterObject;
void EseguiAnimazione(WORD NumeroAnimazione, int IndiceSlot, bool TestSetNextStateId);
extern TYPE_TriggerExplosionSparks TriggerExplosionSparks;
extern TYPE_TriggerUnderwaterExplosion TriggerUnderwaterExplosion;
extern TYPE_ExplosionFX ExplosionFX;
DWORD TrovaPosizioneSuono(StrCanaleBass *pCanale);
extern TYPE_GetFlameTorch GetFlameTorch;
float RapportoFloatSchermo(int SizeSchermo, float SchermoRef, float Cord);
extern TYPE_GetStringLength GetStringLength;
void SospendiHardCoded(int NAnimazione, int FrameToWait);
void AvviaPopUpImage(WORD IdImageCmd, StrScriptImage *pImage, WORD Contatore);
void GestioneOverlapImage(StrScriptImage *pImage, WORD Secondi);
StrBaseDiario* GetDiarioConID(WORD Id);
void ModificaColoreFog(BYTE Rosso, BYTE Verde, BYTE Blue);
void ModificaHardwareFog(void);
void ModificaFogBulbDistance(void);
WORD ConvertiCodVar(WORD CodiceTimer);
 int LeggiVariabile(WORD CodiceVariabile);
void ScriviVariabile(WORD CodiceVariabile, int Valore);
char *LeggiVariabileTesto(WORD CodiceVariabile);
void ConvertiRectMicroUnits(RECT *pRect);
char *GetStringaNG(int Indice);
char *FormattaTimer(int Valore);
int LeggiVariabileMem(WORD Codice);
int GetNgleIndice(int TombIndice);
void AggiungiItemMosso(WORD Indice);
int Casuale(int Massimo);
int GetNumero(char *pCifre, bool *pTestErrore);
extern TYPE_have_i_got_object have_i_got_object;
char *ReplaceVarPlaceFolders(char *pTesto);
char *GetNomeVariabile(WORD Codice);
void EseguiAnimazioneSlot(WORD NumeroAnimazione, int IndiceSlot, int FirstFrame, int NewStateId);
void SwapMeshItem(short SlotItem, WORD SlotSwap, WORD TipoSwap);
void TerminaStandBy(StrBaseStandBy *pStand);
void ImpostaStandBy1(int Indice);
void InizializzaStandBy(StrBaseStandBy *pStand);
int GetIndiceItem(DWORD OffsetItem);
void DeleteParamCommand(int ParamType, int IdParam, bool TestSempre, bool TestSottoComandi);
bool AggiornaStatici(StrScaleItem *pScale, WORD Percentuale);
int EsecuzioneFlipeffect(WORD PluginId, int NumeroFlip, WORD Timer, WORD Flags);
void CalcolaSpriteZoom(StrProgressiveAction *pAzione, 
					   StrShowSprite *pSprite, bool TestSetup);
DWORD DistanzaLaraDaXYZ(DWORD CordX, int CordY, DWORD CordZ);
int Service(DWORD SRV_Type, ...);
char * GetString(int IndiceStringa);
void SalvaRecordSwapSingleMesh(StrSwapSingleMesh *pSwap);
void SwapSingleMesh(StrSwapSingleMesh *pSwap);
void AggiornaBinocoloSuper(void);
bool CaricaDemo(int NumeroDemo);
int GestioneDemoInProgress(WORD FlagGestioneDemo, StrDemo *pDemo);
void LiberaCutsceneCamera(void);
void SwapAnimations(WORD Slot, WORD SwapId);
void RestoreActor(StrItemTr4 *pOggetto, int ItemIndex);
void RestoreAllAnimSwap(bool TestClear);
void PlayExtraCD(short NumeroCd, int Canale, int Loop);
void ElaboraTriggerFMV(int NumeroFmv);


// ++++++ functions +++++++++++++++



// #TAG_ADDING_NEW_FUNCTIONS#
// -> last added function bodies
// disegna mesh presa da oggetto Slot, ed e' la NMesh esima (da 0)
// in pPos ci sono sia le coordinate assolute che i possibili orientaenti
void DisegnaMeshAbs(StrPos3d *pPos, int IndiceMesh)
{
	StrMeshTr4 *pMesh;

	pMesh = GlobTomb4.pAdr->VetMeshPointer[IndiceMesh];
	
	phd_PushMatrix();
	phd_TranslateAbs(pPos->Posizione.OrgX  , pPos->Posizione.OrgY  , 
			pPos->Posizione.OrgZ  );

	phd_RotYXZ(pPos->OrientH, pPos->OrientV, pPos->OrientR );

	phd_PutPolygons_train(pMesh, 0);

	*GlobTomb4.pAdr->pZonaRecord30 -= 0x30;
}
// -> last added function bodies
// restituisce l'incremento con segno per far avvicinare ColoreDa a ColoreAA in circa 30 cicli
char CalcolaIncrementoColore(BYTE ColoreDA, BYTE ColoreAA)
{
	char Incremento;

	if (ColoreDA == ColoreAA) return 0;

	Incremento= (ColoreAA-ColoreDA) / 30;

	if (Incremento==0) {
		if (ColoreDA > ColoreAA) return -1;
		return 1;
	}
	return Incremento;

}
void MostraMieMesh(void)
{
	StrShowMeshAbs *pShow;
	int i;

	// ------------ SHOW MESH ----------------------

	for (i=0;i<GlobTomb4.BaseShowMesh.TotMesh;i++){
		pShow = &GlobTomb4.BaseShowMesh.VetMesh[i];
		DisegnaMeshAbs(&pShow->PosData , pShow->Mesh);
	}
}
// -> last added function bodies
// InitWTextEffect(pAzione, pWText);
// inizializza dati in pAzione per l'effetto (evntuale) collegato a WTEXT
void InitWTextEffect(StrProgressiveAction *pAzione, 	StrParamWText *pWText)
{

	StrWindowsFont *pFont;
	int i;
	BYTE *pByte;

	i = GlobTomb4.BaseFonts.VetID[pWText->WindowsFontId];
	if (i == -1) {
		sprintf(BufferLog,"ERROR: WindowsFont ID of PARAM_WTEXT= %d command is unvalid (%d)",
			pWText->IdParam, pWText->WindowsFontId);
		InviaLog(BufferLog);
		return;
	}

	pFont = &GlobTomb4.BaseFonts.VetFonts[i];

	if (pWText->Flags & WTF_FLYING_TEXT) {
		// salvare dimensione finale del font
		pAzione->VetArg[0] = pFont->SizeFont;
		// dimensione attuale font
		pAzione->VetArgFloat[1] = 2.0f;
		// dimensione finale
		pAzione->VetArgFloat[2] = (float) pFont->SizeFont;
		pAzione->VetArgFloat[3]= 1.15f;  // fattore di incremento attuale
		pAzione->VetArgFloat[4]= 0; // incremento dell'incremento (da sommare in modo algebrico)
		pAzione->VetArg[5] = 0; // fase di incremento (0) / poi quando 1 incremento finito e resta in attesa

		
	}

	if (pWText->Flags & WTF_PULSING_TEXT) {
		// salvare dimensione attuale del font
		pAzione->VetArg[0] = pFont->SizeFont;
		// dimensione mutevole del font
		pAzione->VetArgFloat[1] = (float) pFont->SizeFont;
		// dimensione minima (e' gia' quella iniziale)
		pAzione->VetArgFloat[2]= pAzione->VetArgFloat[1];
		// dimensione massima e' 1/3 in piu'
		pAzione->VetArgFloat[3] = pAzione->VetArgFloat[2] * 1.3f;
		// impostare incremento iniziale
		pAzione->VetArgFloat[4] = 10.0f;
		// impostare fase
		pAzione->VetArg[5]=0;  // fase modifica
	

	}

	if (pWText->Flags & WTF_CHANGE_COLOR) {
		// salvare colore attuale primario di windowsfont
		pAzione->VetArgDword[0] = pFont->ColoreText;
		// salvare i componenti colore e incrementi
		pByte= (BYTE *) &pFont->ColoreShadow;
		pAzione->VetArgDword[1] = pFont->ColoreText;
		// ora gli incrementi
		pAzione->VetArgSignedBytes[8] = CalcolaIncrementoColore(pAzione->VetArgBytes[0], pByte[0]);
		pAzione->VetArgSignedBytes[9] = CalcolaIncrementoColore(pAzione->VetArgBytes[1], pByte[1]);
		pAzione->VetArgSignedBytes[10] = CalcolaIncrementoColore(pAzione->VetArgBytes[2], pByte[2]);
		// valori minimi
		pAzione->VetArgBytes[12] = pAzione->VetArgBytes[0];
		if (pAzione->VetArgBytes[12] > pByte[0]) pAzione->VetArgBytes[12]= pByte[0];

		pAzione->VetArgBytes[13] = pAzione->VetArgBytes[1];
		if (pAzione->VetArgBytes[13] > pByte[1]) pAzione->VetArgBytes[13]= pByte[1];

		pAzione->VetArgBytes[14] = pAzione->VetArgBytes[2];
		if (pAzione->VetArgBytes[14] > pByte[2]) pAzione->VetArgBytes[14]= pByte[2];

		// valori massimi
		pAzione->VetArgBytes[16] = pAzione->VetArgBytes[0];
		if (pAzione->VetArgBytes[16] <= pByte[0]) pAzione->VetArgBytes[16]= pByte[0];

		pAzione->VetArgBytes[17] = pAzione->VetArgBytes[1];
		if (pAzione->VetArgBytes[17] <= pByte[1]) pAzione->VetArgBytes[17]= pByte[1];

		pAzione->VetArgBytes[18] = pAzione->VetArgBytes[2];
		if (pAzione->VetArgBytes[18] <= pByte[2]) pAzione->VetArgBytes[18]= pByte[2];

	}


}

// -> last added function bodies
// riceve in input: 
//		pColl (box relativo di collisione)
//		pOrigine (struttura con coordinate x,y,z)
// Scrive in ouput:
//		pBoxAbs  (box collisone con valori assoluti ruotato)	
//      esito   true = ortogonale / false = no	
bool RuotaCollBox(StrBoxCollisione *pColl, StrPosizione *pOrigine, StrCollisionAbsBox *pBoxAbs, WORD Orient)
{

	pBoxAbs->MinY = pOrigine->OrgY  + pColl->MinY;
	pBoxAbs->MaxY = pOrigine->OrgY  + pColl->MaxY;

	switch (Orient) {
	case 0x000:
		pBoxAbs->MinX = pOrigine->OrgX  + pColl->MinX;
		pBoxAbs->MaxX = pOrigine->OrgX  + pColl->MaxX;

		pBoxAbs->MinZ = pOrigine->OrgZ  + pColl->MinZ;
		pBoxAbs->MaxZ = pOrigine->OrgZ  + pColl->MaxZ;
		break;
	case 0x4000:
		pBoxAbs->MinX = pOrigine->OrgX  + pColl->MinZ;
		pBoxAbs->MaxX = pOrigine->OrgX  + pColl->MaxZ;

		pBoxAbs->MinZ = pOrigine->OrgZ  - pColl->MaxX;
		pBoxAbs->MaxZ = pOrigine->OrgZ  - pColl->MinX;
		break;

	case 0x8000:
		pBoxAbs->MinX = pOrigine->OrgX - pColl->MaxX;
		pBoxAbs->MaxX = pOrigine->OrgX - pColl->MinX;

		pBoxAbs->MinZ = pOrigine->OrgZ - pColl->MaxZ;
		pBoxAbs->MaxZ = pOrigine->OrgZ - pColl->MinZ;
		break;

	case 0xC000:
		pBoxAbs->MinX = pOrigine->OrgX - pColl->MaxZ;
		pBoxAbs->MaxX = pOrigine->OrgX - pColl->MinZ;

		pBoxAbs->MinZ = pOrigine->OrgZ + pColl->MinX;
		pBoxAbs->MaxZ = pOrigine->OrgZ + pColl->MaxX;
		break;

	default:
		// non e' ortogonale
		pBoxAbs->MinX = pOrigine->OrgX + pColl->MinX;
		pBoxAbs->MaxX = pOrigine->OrgX + pColl->MaxX;

		pBoxAbs->MinZ = pOrigine->OrgZ + pColl->MinZ;
		pBoxAbs->MaxZ = pOrigine->OrgZ + pColl->MaxZ;
		return false;
	}
	return true;
}
// -> last added function bodies
void SetVolumeBass(int IndiceCanale)
{
	float Volume;
	StrBassHandles *pBass;
	StrCanaleBass *pCanale;
	int Inizio,Fine;
	int i;

	pBass= &GlobTomb4.BaseBassHandles;
	AggiornaVolumeBass();
	Volume =  pBass->VolumeMusica;


	switch (IndiceCanale) {
	case -1:
		Inizio = 0;
		Fine=5;
		break;
	case 0:
		Inizio=0;
		Fine=1;
		break;
	case 1:
		Inizio=1;
		Fine=2;
		break;
	}


	for (i=Inizio;i<Fine;i++) {
		pCanale = &pBass->VetCanali[i];
		if (pCanale->Canale) {

			pBass->Proc.BASS_ChannelSetAttribute(pCanale->Canale, 
				BASS_ATTRIB_VOL, Volume);
		}
	}

	
}
// ruota box collisione di static e restituisce i valori
// assoluti di tutte le coordinate in pBoxAbs
// nota: questo funziona solo se orienamento e' ortogonale
// se non lo e' restituisce false e questo vuol dire che orientamenrto
// e ' diagonale
// se e' ortognale in pCenterX pCenterZ scrive il nuovo pivot 
// reralmente centrato
// se invece e' diagonale sara' semplicemente la coordina originale
bool RuotaBoxCollItem(StrItemTr4 *pItem, StrCollisionAbsBox *pBoxAbs)
{
	StrBoxCollisione *pColl;


	pColl=GetBestFrame(pItem);
	

	if (RuotaCollBox(pColl, (StrPosizione*) &pItem->CordX , pBoxAbs, 
							pItem->OrientationH)==false) return false; 


	return true;

}
// ruota box di static e restituisce i valori
// assoluti di tutte le coordinate in pBoxAbs
// nota: questo funziona solo se orienamento e' ortogonale
// se non lo e' restituisce false e questo vuol dire che orientamenrto
// e ' diagonale
// se invece e' diagonale sara' semplicemente il box  originale
// nota; Se TestViewBox = true, usa il box view invece che quello di collisione
bool RuotaBoxCollStatic(StrMeshInfo *pMesh, StrCollisionAbsBox *pBoxAbs, bool TestViewBox)
{
	StrBoxCollisione *pColl;

	if (TestViewBox) {
		pColl = &GlobTomb4.pAdr->pVetEditObjects[pMesh->SlotId].ViewBox;
	}else {
		pColl= &GlobTomb4.pAdr->pVetEditObjects[pMesh->SlotId].CollisionBox;
	}

	if (RuotaCollBox( pColl, (StrPosizione *) &pMesh->x , pBoxAbs, pMesh->Orient)==false) return false; 


	return true;

}
bool AllocaCutsceneCamera(void)
{
	int i;
	StrCameraTr4 *pCamera;
	StrCutsceneCamera *pCut;

	pCut= &GlobTomb4.pBaseCutscene->BaseCamera;

	if (pCut->TestAllocata == true) return true;

	// cercare un fixed camera 
	for (i=0;i<*GlobTomb4.pAdr->Camera.pTotCameras;i++) {
		pCamera = &GlobTomb4.pAdr->Camera.pVetCamera[i];

		if (pCamera->Flags & 0x0001) {
			// trovata
			pCut->IndexCamera = i;
			pCut->Original = *pCamera;
			pCut->pCamera = pCamera;
			pCut->pCamera->Flags = 0x0001;
			pCut->TestAllocata=true;
			GlobTomb4.TestDisableFeatures |= DF_GUARDA;
			return true;
		}
	}

	InviaLog("ERROR: cann't allocate Cutscene camera. Not found any fixed camera in whole level");
	return false;
}
// -> last added function bodies
// cerca nel record degli effetti allo scopo di scoprire 
// l'indice dell'effetto che corrisponde alle fiamme di lara
int TrovaEffettoFiamme(void)
{

	StrEffettoTr4 ** p2_PtrEffetti = (StrEffettoTr4**) 0x8011C0;
	StrEffettoTr4 * VetEffetti;
	short *pIndiceEffettoNow = (short *) 0x8011D0;
	StrEffettoTr4 *pEffetto;
	short PrimoIndice;
	short IndiceNow;


	VetEffetti = *p2_PtrEffetti;

	
	PrimoIndice = *pIndiceEffettoNow;

	if (PrimoIndice == -1) return -1;
	IndiceNow = PrimoIndice;

	while (IndiceNow != -1) {
		pEffetto = &VetEffetti[IndiceNow];
		if (pEffetto->CodiceEffetto == 0x8e && 
			pEffetto->Flag == 0x4210) return IndiceNow;
		// non e' questo
		// andare a ritroso
		IndiceNow = pEffetto->IndicePrevious;
		if (IndiceNow == PrimoIndice) return -1;
	}
	return -1;

}
WORD GetRecharge(WORD Percentage, WORD Totale)
{
	// se il valore e' <= 200 allora basta restituire quel valore
	// se invece e' maggiore va visto come percentuale sul totale
	if (Percentage <=200) return Percentage;

	Percentage -= 200;
	Percentage *= 10;

	return (Percentage * Totale) / 100;


}
// crea un azione progressiva per eseguire sempre questo trigger group
void CreaTriggerGroupContinuo(int IdTriggerGroup)
{

	int IndiceGroup;
	int IndiceAzione;
	StrProgressiveAction *pAzione;
	int i;
	IndiceGroup = GlobTomb4.pBaseTriggerGroups->VetID[IdTriggerGroup];
	if (IndiceGroup==-1) {

		sprintf(BufferLog,"ERROR: cann't find trigger group with id=%d (triggergroup to set in CONTINUE mode)", 
				IdTriggerGroup);
		InviaLog(BufferLog);
		return;
	}
	// prima verificare che non sia gia' stata attivata
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		pAzione= &GlobTomb4.VetProgressiveActions[i];
		if (pAzione->ActionType == AZ_TRIGGER_GROUP_CONTINUO &&
			pAzione->ItemIndex == IdTriggerGroup) return;
	}

	IndiceAzione = CreaNuovaAzioneProgressiva();

	pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
	pAzione->ActionType = AZ_TRIGGER_GROUP_CONTINUO;
	pAzione->ItemIndex = IdTriggerGroup;


}
void AttivaFlipMap(WORD Timer, WORD ExtraTimer)
{

	if (GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] & 0x100) return;


	GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= (ExtraTimer & 0x3e00);

	if ((GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] & 0x3e00) == 0x3e00) {
		if (ExtraTimer & 0x100) {
			// se attivo one-shot attivarlo anche nei flags
			GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= 0x100;
		}

		// ok, tutti i bit, quindi attivarla, a meno che non sia gia' attiva
		if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer]) return;

	}else {
		// non tutti i pulsanti sono a posto
		// se era disattiva non fare niente
		if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer] == 0) return;

	}

	FlipMap(Timer);

}
// cambia volume di canale con volume e fade out
void CambiaVolumeConFade(int Canale, int Volume)
{
	StrBassHandles *pBass;
	float FloatVol;

	pBass = &GlobTomb4.BaseBassHandles;
	if (pBass->TestPresente == false) return;

	if (pBass->VetCanali[Canale].Canale ==0) return;

	FloatVol = (float) Volume / (float) 100;

	pBass->Proc.BASS_ChannelSlideAttribute(pBass->VetCanali[Canale].Canale,
			BASS_ATTRIB_VOL, FloatVol, GlobTomb4.pBaseCustomize->BassDll.TimeFadeOut);

}
// crea azione per movimento static o moveable con i dati in pMove
void CreaMoveItem(StrMoveParameters *pMove, bool TestStatic)
{
	StrProgressiveAction *pAzione;
	int Indice;
	WORD Cicli;
	short Incremento;
	WORD IndiceRoom;
	short IndiceStatic;
	int i;
	WORD TipoAzione;
	bool TestOldGestione;
	WORD Orient;
	StrPosAndFacing ItemPos;
	StrMeshInfo *pStatic;
	StrItemTr4 *pMoveable;
	WORD Direzione;
	StrItemTr4 *pLara;
	WORD HSpeed;
	WORD VSpeed;
	short StartVSpeed;
	short IncVSpeed;
	short MaxVSpeed;

	short StartHSpeed;
	short IncHSpeed;
	short MaxHSpeed;
	WORD FinalOrient;
	short IncOrient;
	short TSpeed;




	// vedere se si puo'usare vecchia gestione
	TestOldGestione=true;
	Direzione = pMove->Direzione & DIR_MASK_DIRECTION;

	// se ci sono direzioni relative, allora NON e' vecchia gestione
	if (Direzione >= DIR_FORWARD) TestOldGestione =false;

	// se ci sono velocita' variabili NON e' vecchia gestione
	if (pMove->Flags & FMOV_MASK_SPEED) TestOldGestione=false;

	// se c'e' gravita speciale e movimento era dir_up o dir_down allora non va bene gestione OLD
	if ((Direzione == DIR_UP || Direzione == DIR_DOWN)  && (pMove->Flags & FMOV_MASK_GRAVITY) !=0) TestOldGestione=false;
	
	// trovare indici corrispondenti
	if (TestStatic) {
		IndiceRoom = GlobTomb4.VetRemapStatics[pMove->ItemIndex].IndiceRoom;
		
		IndiceStatic = GlobTomb4.VetRemapStatics[pMove->ItemIndex].IndiceStatic;
		TipoAzione= AZ_MOVE_STATIC;
	}else {
		// e' un moveable
		IndiceStatic=-1;
		if (pMove->Flags & FMOV_USE_LEADING_ACTOR_INDEX) {
			IndiceStatic = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (IndiceStatic==-1) {
				InviaLog("ERROR: index of Leading Actor not valid (-1)");

				return;
			}

		}
		if (pMove->Flags & FMOV_USE_EXTRA_ACTOR_INDEX) {
			IndiceStatic = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (IndiceStatic==-1) {
				InviaLog("ERROR: index of Extra Actor not valid (-1)");
				return;
			}
		}
		if (IndiceStatic==-1) {
			IndiceStatic = GlobTomb4.VetRemapObjects[pMove->ItemIndex];
			if (IndiceStatic == -1) {
				sprintf(BufferLog, "ERROR in PARAM_MOVE_ITEM script command. The IndexItem (%d) is not present",
					pMove->ItemIndex);
				InviaLog(BufferLog);
				return;
			}
		}
		IndiceRoom=GlobTomb4.pAdr->pVetItems[IndiceStatic].Room;
		TipoAzione=AZ_MOVE_MOVEABLE;
	}
	// prima verificare che non sia gia' stato azionato
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		pAzione = &GlobTomb4.VetProgressiveActions[i];
		if (pAzione->ActionType  == TipoAzione && 
			pAzione->ItemIndex == IndiceStatic &&
			pAzione->Arg1 == IndiceRoom &&
			pAzione->VetArgWord[2] == pMove->Direzione) return;
	}


	Indice = CreaNuovaAzioneProgressiva();
	pAzione = &GlobTomb4.VetProgressiveActions[Indice];
	pAzione->ActionType = TipoAzione;
	if (TestStatic == false) AggiungiItemMosso(IndiceStatic);

	if (TestOldGestione) {
		// per vecchia gestione usare vecchi dati 
		if (pMove->Speed==0) {
			Cicli=1;
		}else {
			Cicli = pMove->Distanza / pMove->Speed;
			//Cicli++;
		}
		// calcolo incremento sulla base di direzione, speed e distanza
		Incremento = pMove->Speed;
		switch (pMove->Direzione) {
		case DIR_UP:
		case DIR_NORTH:
		case DIR_WEST:
			Incremento = -Incremento;
			break;
		}

		if (pMove->Direzione  & DIR_INVERT_DIRECTION) Incremento=-Incremento;
		

		pAzione->VetArgShort[0] = Incremento;
		pAzione->VetArgShort[1] = MOVE_ITEM_OLD_MANAGEMENT;
		
		pAzione->ItemIndex = IndiceStatic;
		pAzione->Arg1 = IndiceRoom;
		pAzione->Arg2 = Cicli;
		pAzione->VetArgWord[2] = pMove->Direzione;
		pAzione->VetArgWord[3] = pMove->Flags;
		pAzione->VetArgShort[4] = pMove->SuonoMovimento;
		pAzione->VetArgShort[5] = pMove->SuonoFinale;
		// salvare cicli originali
		pAzione->VetArgWord[6] = Cicli;  // cicli totali
		pAzione->VetArgWord[7] =0;  //prima esecuzione
		

	}else {
		// dati per nuova gestione
		pAzione->VetArgWord[0] = 0; // distanza gia' percorsa
		pAzione->VetArgShort[1] = MOVE_ITEM_NEW_MANAGEMENT;
		pAzione->ItemIndex = IndiceStatic;
		pAzione->Arg1 = IndiceRoom;
		
		HSpeed = pMove->Speed & 0xff;
		VSpeed = (pMove->Speed >> 8) & 0xff;
		TSpeed = pMove->Extra;


		IncOrient=0;
		FinalOrient=0;
		
		// inserire dati in ItemPos
		if (TestStatic) {
			pStatic = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];

			ItemPos.CordX = pStatic->x;
			ItemPos.CordY = pStatic->y;
			ItemPos.CordZ = pStatic->z;
			ItemPos.Orient = pStatic->Orient;
		}else {
			pMoveable = &GlobTomb4.pAdr->pVetItems[IndiceStatic];

			ItemPos.CordX= pMoveable->CordX;
			ItemPos.CordY =pMoveable->CordY;
			ItemPos.CordZ =pMoveable->CordZ;
			ItemPos.Orient = pMoveable->OrientationH;
		}

		pLara= GlobTomb4.pAdr->pLara;

		// in primo campo WORD l'orientamento orizzontale
		switch (Direzione) {
		case DIR_NORTH:
			Orient= 0xC000;
			break;
		case DIR_EAST:
			Orient = 0x0000;
			break;
		case DIR_SOUTH:
			Orient = 0x4000;
			break;
		case DIR_WEST:
			Orient= 0x8000;
			break;
		case DIR_UP:
			HSpeed= -HSpeed;
			break;
	
		case DIR_FORWARD:
		case DIR_TURNING_LEFT_90: 
		case DIR_TURNING_LEFT_45:
		case DIR_LU_TURNING_180:
		case DIR_TURNING_RIGHT_45:
		case DIR_TURNING_RIGHT_90:
		case DIR_RU_TURNING_180 :
			Orient = ItemPos.Orient;
			break;
		case DIR_LARA_FACING:
			Orient = pLara->OrientationH;
			break;
		case DIR_LEADING_ACTOR_FACING:
			Indice = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (Indice==-1) {
				InviaLog("ERROR: index of Leading Actor is not valid (-1)");
				pAzione->ActionType=0;
				return;
			}
			Orient = GlobTomb4.pAdr->pVetItems[Indice].OrientationH;
			break;
		case DIR_DIRECTION_LARA_LEADING_ACTOR:
			Indice = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (Indice==-1) {
				InviaLog("ERROR: index of Leading Actor is not valid (-1)");
				pAzione->ActionType=0;
				return;
			}
			Orient = mGetAngle(pLara->CordX  , pLara->CordZ, 
							GlobTomb4.pAdr->pVetItems[Indice].CordX  , GlobTomb4.pAdr->pVetItems[Indice].CordZ );
			Orient += 0x4000; // si usa sempre per avere valore ok
			break;
		case DIR_HEAD_FOR_LARA:
			Orient = mGetAngle(ItemPos.CordX, ItemPos.CordZ, pLara->CordX, pLara->CordZ);

			Orient += 0x4000; // si usa sempre per avere valore ok
			break;
		case DIR_HEAD_FOR_LEADING_ACTOR:
			Indice = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (Indice==-1) {
				InviaLog("ERROR: index of Leading Actor is not valid (-1)");
				pAzione->ActionType=0;
				return;
			}
			Orient = mGetAngle(ItemPos.CordX, ItemPos.CordZ, 
				GlobTomb4.pAdr->pVetItems[Indice].CordX, GlobTomb4.pAdr->pVetItems[Indice].CordZ);
			Orient += 0x4000; // si usa sempre per avere valore ok
			break;
		case DIR_HEAD_FOR_EXTRA_ACTOR:
			Indice = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (Indice==-1) {
				InviaLog("ERROR: index of Extra Actor is not valid (-1)");
				pAzione->ActionType=0;
				return;
			}
			Orient = mGetAngle(ItemPos.CordX, ItemPos.CordZ, 
				GlobTomb4.pAdr->pVetItems[Indice].CordX, GlobTomb4.pAdr->pVetItems[Indice].CordZ);
			Orient += 0x4000; // si usa sempre per avere valore ok
			break;
		}
		if (pMove->Direzione  & DIR_INVERT_DIRECTION) Orient += 0x8000;

		// analisi per turning
		// nota: nel caso di turning mettere in Arg2 sempre zero all'inizio dove poi faro' in ctneggio
		// della rotazione avvenuta 
		switch (Direzione) {
		case DIR_TURNING_LEFT_90:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed ==0) TSpeed = 512; 

			FinalOrient= 0x4000;
			IncOrient = -TSpeed;
			break;
		case DIR_TURNING_LEFT_45:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed==0) TSpeed = 256;
			

			FinalOrient = 0x2000;
			IncOrient = -TSpeed;
			break;
		case DIR_RU_TURNING_180:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed==0) TSpeed = 1024;

			FinalOrient = 0x8000;
			IncOrient = TSpeed;
			break;
		case DIR_LU_TURNING_180:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed==0) TSpeed = 1024;

			FinalOrient =  0x8000;
			IncOrient = -TSpeed;
			break;
		case DIR_TURNING_RIGHT_45:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed==0) TSpeed = 256;

			FinalOrient = 0x2000;
			IncOrient = TSpeed;
			break;
		case DIR_TURNING_RIGHT_90:
			// se non e' impostata velocita' di rotazione, dare valore ragionevole
			if (TSpeed==0) TSpeed = 512;

			FinalOrient = 0x4000;
			IncOrient = TSpeed;
			break;

		}

		pAzione->Arg2 = Orient;  // orient iniziale


		StartVSpeed=0;
		IncVSpeed=0;
		MaxVSpeed=0;

		// ora calcolare variazioni velocita' h e v
		switch (pMove->Flags & FMOV_MASK_GRAVITY) {
		case FMOV_FROG_JUMP_GRAVITY:
			InitFrogVSpeed(pAzione, pMove);


			break;
		case FMOV_LEAF_GRAVITY:
			StartVSpeed = 0;
			IncVSpeed=1;
			MaxVSpeed= VariaPercentuale(8, VSpeed);
			break;
		case FMOV_APPLE_GRAVITY:
			StartVSpeed = 5;
			IncVSpeed = VariaPercentuale(8, VSpeed);
			MaxVSpeed= VariaPercentuale(180, VSpeed);
			break;
		case FMOV_MAN_GRAVITY:
			StartVSpeed= VariaPercentuale(10, VSpeed);
			IncVSpeed = VariaPercentuale(12, VSpeed);
			MaxVSpeed= VariaPercentuale(200, VSpeed);
			break;
		case FMOV_ANVIL_GRAVITY:
			StartVSpeed= VariaPercentuale(15, VSpeed);
			IncVSpeed = VariaPercentuale(15, VSpeed);
			MaxVSpeed= VariaPercentuale(240, VSpeed);
			break;


		case FMOV_EXPLOSION_GRAVITY:

			StartVSpeed = -200;
			IncVSpeed = VariaPercentuale(20, VSpeed);
			MaxVSpeed = VariaPercentuale(64, VSpeed); 
			break;

		case FMOV_APOLLO_GRAVITY:
			StartVSpeed = 0;
			IncVSpeed = VariaPercentuale(-5, VSpeed);
			MaxVSpeed = VariaPercentuale(-180, VSpeed);
			break;
		}
		StartHSpeed=HSpeed;
		IncHSpeed=0;
		MaxHSpeed=HSpeed;
	
		// calcolare speed velolcita horizontal
		switch (pMove->Flags & FMOV_MASK_SPEED) {
		case FMOV_EXPLOSION_SPEED:
			StartHSpeed = VariaPercentuale(300, HSpeed);
			IncHSpeed = VariaPercentuale(-10, HSpeed);
			MaxHSpeed = VariaPercentuale(80, HSpeed);
			break;
		case FMOV_MAGNET_SPEED:
			StartHSpeed=0;
			IncHSpeed = VariaPercentuale(3, HSpeed);
			MaxHSpeed= VariaPercentuale(200, HSpeed);
			break;
		case FMOV_CAR_SPEED:
			StartHSpeed = 0;
			IncHSpeed = VariaPercentuale(1, HSpeed);
			MaxHSpeed= VariaPercentuale(80, HSpeed);
			break;
		}
		// caricare tutti i dati
		pAzione->VetArgWord[2] = pMove->IdMove;
		pAzione->VetArgShort[3] = StartVSpeed;
		pAzione->VetArgShort[4] = IncVSpeed;
		pAzione->VetArgShort[5] = MaxVSpeed;

		pAzione->VetArgShort[6]= StartHSpeed;
		pAzione->VetArgShort[7]= IncHSpeed;
		pAzione->VetArgShort[8]= MaxHSpeed;

		pAzione->VetArgWord[9] = FinalOrient; // relativo e sempre positivo (ossia +0x4000 o 0x2000 0x8000)
		pAzione->VetArgShort[10] = IncOrient;  // turning speed
		pAzione->VetArgWord[11] =0; // conta cambiamento di orientamento partendo da zero, sempre positivo
		

	}


	if (TestStatic) {
		// attivare richiesta di salvataggio
		GlobTomb4.pAdr->pVetRooms[IndiceRoom].
				Ptr_StaticMesh[IndiceStatic].OCB |= OCBS_SALVARE;
	}

	


}
// crea nuova azione progressiva per rotazione oggetto: moveable o static
void CreaAzioneRotateItem(StrRotateItem *pRotate, bool TestStatic)
{
	StrProgressiveAction *pAzione;
	int Indice;
	WORD Cicli;
	WORD IndiceRoom;
	WORD IndiceStatic;
	int i;
	WORD TipoAzione;
	int AngoloRotazione;
	
	if (TestStatic) {
		TipoAzione = AZ_ROTATE_STATIC;
		IndiceStatic = GlobTomb4.VetRemapStatics[pRotate->ItemIndex].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[pRotate->ItemIndex].IndiceRoom;
		
		
	}else {
		TipoAzione = AZ_ROTATE_MOVEABLE;
		IndiceStatic = GlobTomb4.VetRemapObjects[pRotate->ItemIndex];
		if (IndiceStatic == SCRIPT_IGNORE) return;
		IndiceRoom=GlobTomb4.pAdr->pVetItems[IndiceStatic].Room;

	}

	// qui andra' controllato che questo comando di rotazione per questo
	// item non sia gia' presente
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		pAzione= &GlobTomb4.VetProgressiveActions[i];
		if (pAzione->ActionType == TipoAzione && 
			pAzione->ItemIndex == IndiceStatic &&
			pAzione->Arg1 == IndiceRoom) return;
	}

		
	Indice = CreaNuovaAzioneProgressiva();
	pAzione = &GlobTomb4.VetProgressiveActions[Indice];
	pAzione->ActionType = TipoAzione;
	if (TestStatic == false) AggiungiItemMosso(IndiceStatic);
	pAzione->ItemIndex = IndiceStatic;
	pAzione->Arg1 = IndiceRoom;
	pAzione->Arg2 = pRotate->Flags;
	pAzione->VetArgShort[6] = pRotate->DirezioneH;
	pAzione->VetArgShort[7] = pRotate->DirezioneV;

	// convertire valore finale da raggiungere in termini assoluti
	if (pRotate->DirezioneH != -1) {
		// gestione orientamento orizzontale
		AngoloRotazione = pRotate->AngoloH;
		if (pRotate->Flags & FROT_LOOP) {
			Cicli = 0xffff;
		}else {
			Cicli = AngoloRotazione / pRotate->SpeedH;
			Cicli++;
		}
	
		pAzione->VetArgShort[0] = Cicli;
		if (pRotate->DirezioneH == ROTH_CLOCKWISE) {
			pAzione->VetArgShort[4] = pRotate->SpeedH;
		}
		if (pRotate->DirezioneH == ROTH_INV_CLOCKWISE) {
			pAzione->VetArgShort[4] = -pRotate->SpeedH;
		}

	}

	if (pRotate->DirezioneV != -1) {
		// gestione orientamento verticale
		AngoloRotazione = pRotate->AngoloV;
		if (pRotate->Flags & FROT_LOOP) {
			Cicli = 0xffff;
		}else {
			Cicli = AngoloRotazione / pRotate->SpeedV;
			Cicli++;
		}
	
		pAzione->VetArgShort[1] = Cicli;
		if (pRotate->DirezioneV  == ROTV_FORWARD) {
			pAzione->VetArgShort[5] = -pRotate->SpeedV;
		}
		if (pRotate->DirezioneV  == ROTV_BACKWARD) {
			pAzione->VetArgShort[5] = pRotate->SpeedV;
		}

	}

	pAzione->VetArgShort[2] = pRotate->SoundMoving;
	pAzione->VetArgShort[3] = pRotate->SoundFinal;

	if (TestStatic) {
		// attivare richiesta di salvataggio
		GlobTomb4.pAdr->pVetRooms[IndiceRoom].
				Ptr_StaticMesh[IndiceStatic].OCB |= OCBS_SALVARE;
	}

}
// applica cambiamento colore per static o moveable
void EseguiCambiaColoreItem(StrColoraItem *pColora, bool TestStatic)
{
	BYTE *pVetBytes;
	int i;
	short IndiceStatic;
	short IndiceRoom;
	WORD TipoAzione;
	StrProgressiveAction *pAzione;
	StrMeshInfo *pMesh;
	DWORD Colore32;
	BYTE *pVetBytesFine;
	BYTE VetColFinale[3];
	WORD TempColor;
	int Indice;


	if (TestStatic) {
		IndiceStatic = GlobTomb4.VetRemapStatics[pColora->ItemIndex].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[pColora->ItemIndex].IndiceRoom;
		if (IndiceStatic == -1) return;
		TipoAzione=AZ_CHANGE_COLOR_STATIC;
	}else{
		IndiceStatic = GlobTomb4.VetRemapObjects[pColora->ItemIndex];
		if (IndiceStatic==-1) return;
		IndiceRoom = GlobTomb4.pAdr->pVetItems[IndiceStatic].Room;
		TipoAzione=AZ_CHANGE_COLOR_MOVEABLE;
	}

	i = pColora->IdColore1;

	i = GlobTomb4.BaseColoriRGB.VetID[i];
	if (i == -1) return;

	pVetBytes = (BYTE*) &GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
	Colore32 = GlobTomb4.BaseColoriRGB.VetColori[i].Colore;

	if  (pColora->Flags == COLTYPE_SET_COLOR) {

		// basta copiare il colore
		// prima pero' rimuovere eventuali azioni di cambio colore
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione=&GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == TipoAzione &&
				pAzione->ItemIndex == IndiceStatic && 
				pAzione->Arg1 == IndiceRoom) {
				pAzione->ActionType =0;
				break;
			}
		}
		if (TestStatic) {

			pMesh = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
			pMesh->Color =	ConvertiInColoreWord(pVetBytes,true);
			pMesh->OCB |= OCBS_SALVARE;
		}else {
			// e' moveable
			GlobTomb4.pAdr->pVetItems[IndiceStatic].LightRGB  = Colore32;
			AggiungiItemMosso(IndiceStatic);
		}
		return;
	}

	// e' di tipo variazione
	// come prima cosa verificare se gia' esiste un'azione
	// di questo tipo
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		pAzione=&GlobTomb4.VetProgressiveActions[i];
		if (pAzione->ActionType == TipoAzione &&
			pAzione->ItemIndex == IndiceStatic && 
			pAzione->Arg1 == IndiceRoom) return;
	}

	Indice = CreaNuovaAzioneProgressiva();
	pAzione = &GlobTomb4.VetProgressiveActions[Indice];

	pAzione->ActionType = TipoAzione;
	pAzione->Bytes.AlfaByte = (BYTE) pColora->Speed+1;
	pAzione->Bytes.BetaByte = (BYTE) pColora->Speed;
	pAzione->ItemIndex = IndiceStatic;
	pAzione->Arg1 = IndiceRoom;

	if (TestStatic) {
		// e' uno static
		pMesh = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMesh->OCB |= OCBS_SALVARE;
	}else {
		// e' moveable
		AggiungiItemMosso(IndiceStatic);
	}

	if (pColora->Flags == COLTYPE_SHADE_COLORS) {
		// impostare vetbytesfine con colore 2
		i = pColora->IdColore2;
		if (i == -1) return;
		i = GlobTomb4.BaseColoriRGB.VetID[i];

		if (i== -1) return;
		pVetBytesFine = (BYTE *) &GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
	}

	if (pColora->Flags == COLTYPE_SET_PULSE) {
		// come colore destinazione usare lo stesso colore ma con luminosita
		// aumentata
		for (i=0;i<3;i++) {
			TempColor = pVetBytes[i] + 64;
			if (TempColor > 255) TempColor = 255;
			VetColFinale[i] = TempColor & 0xff;
		}
		pVetBytesFine = VetColFinale;
	}


	// copiare i tre valori attuali di rosso verde e blu
	for (i=0;i<3;i++) {
		pAzione->VetArgFloat[i+3] = (float) pVetBytes[i];
	}
	// ora calcolare differenza di incrementi


	for (i=0;i<3;i++) {
		pAzione->VetArgFloat[i] = 
			(float) pVetBytesFine[i] - pAzione->VetArgFloat[i];
		// ora dividerlo per il tempo
		pAzione->VetArgFloat[i] /= (float) pColora->Speed;
	}


			
}
void TogliTorcia(void)
{
	int i;
	*GlobTomb4.pAdr->pObjInLaraHandsNow = 
		*GlobTomb4.pAdr->pWeaponSelected;
	*GlobTomb4.pAdr->pFlagsLara &= ~1;
	*GlobTomb4.pAdr->pObjInLaraHandsNext =0;
	*GlobTomb4.pAdr->pFlagsLaraHands = 0;
	// mettere mesh di mano vuota
	i = GlobTomb4.pAdr->pVetSlot[0].IndexFirstMesh;

	GlobTomb4.pAdr->VetMeshLara[MESH_LEFT_HAND] =
		GlobTomb4.pAdr->VetMeshPointer[i+26];
}
// funzione usata per correggere un numero float con wirgola in modo
// tale che i valori dopo la virgola siano sempre un multiplo preciso
// di 10, ossia: 0, 1, 2 o 5
// cosicche' con incrementi succesivi si arrivi sempre con la stessa 
// temporizzazione a un nuovo pixel
float CorreggiDecimali(float Valore)
{
	int Intero;
	float Decimale;
	float Risultato;

	Intero = (int) Valore;

	Decimale = Valore - Intero;

	if (Decimale < 0.1f) {
		Risultato = (float) Intero;
		return Risultato;
	}

	if (Decimale < 0.2f) {
		Risultato = (float) Intero + 0.1f;
		return Risultato;
	}

	if (Decimale < 0.4f) {
		Risultato = (float) Intero + 0.2f;
		return Risultato;
	}

	if (Decimale < 0.8f) {
		Risultato = (float) Intero + 0.5f;
		return Risultato;
	}

	Risultato = (float) (Intero+1);
	return Risultato;

}
void CreateCrossFadeAction(int EffectType, int IdEffect, short Time)
{
	StrProgressiveAction *pAzione;
	int Indice;
	int i;
	// verfificare se gia' esisteva, ignorarla
	pAzione= &GlobTomb4.VetProgressiveActions[0];

	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		if (pAzione->ActionType== AZ_CROSS_FADE_IMAGE && pAzione->ItemIndex == IdEffect && 
			pAzione->Arg2 == EffectType) return;
		pAzione++;
	}


	Indice=CreaNuovaAzioneProgressiva();
	pAzione = &GlobTomb4.VetProgressiveActions[Indice];

	pAzione->ActionType= AZ_CROSS_FADE_IMAGE;
	pAzione->Arg1 = Time;
	pAzione->ItemIndex = IdEffect;
	pAzione->Arg2 = EffectType;

	GlobTomb4.KeysToStopBackup = GlobTomb4.KeysToStop;
	GlobTomb4.KeysToStop = CMD_ALL;

	SetScreenFadeOut(8,1);


}
__declspec(naked) void MostraStatistiche(void)
{
	__asm {

	push esi	; INIZIO PROCEDURA 1242 ;  SIZE=0xC0 
	push edi 
	xor  edi, edi 
	mov eax, 4780E0h    ;CreateMonoScreen
	call eax 
	mov eax, 5B7370h   ; TipoPausaMenu
	mov dword ptr [eax], 2
	push 02h  ;era 01h
	mov eax, 47AB80h    ;S_DisplayPauseMenu
	call eax 
	add  esp, 04h 
	push eax
	mov eax, 4BF2A0h   ;TestGrayScreen
	mov  dword ptr [eax], 01h 
	pop eax
Salto1: 
	mov eax, 47B6E0h    ;S_InitialisePolyList
	call eax 
	test edi, edi 
	jnz  Salto2 
	mov eax, 475A80h    ;S_UpdateInput
	call eax 
	jmp  Salto3 
Salto2: 
	push eax
	mov eax, 535784h   ;TestGameOInventario
	mov  dword ptr [eax], 00h 
	pop eax
Salto3: 
	push eax
	mov eax, 7FE040h   ;TestInventarioAttivo
	mov  dword ptr [eax], 01h 
	pop eax
	mov eax, 4786C0h    ;S_DisplayMonoScreen
	call eax 
	push 00h 
	mov eax, 47AB80h    ;S_DisplayPauseMenu
	call eax 

	add  esp, 04h 
	mov  esi, eax 


	mov eax, 462AE0h    ;UpdatePulseColour
	call eax 
	mov eax, 47D990h    ;S_OutputPolyList
	call eax 
	mov eax, 47DCB0h    ;S_DumpScreen
	call eax 
	mov eax, 5B7370h   ; TipoPausaMenu
	cmp word ptr [eax], 2
	jnz  Salto8  // provaread uscire subito

	cmp  esi, 01h 
	jz   Salto8 
	cmp  esi, 08h 
	jnz  Salto4 
	mov  edi, esi 
	xor  esi, esi 
	push 0ffh 
	push esi 
	mov eax, 48A000h    ;SetFade
	call eax 
	add  esp, 08h 
	jmp  Salto5 
Salto4: 
	test edi, edi 
	jz   Salto6 
Salto5: 
	push eax
	mov eax, 672D30h   ;SetTipoFade
	cmp  dword ptr [eax], 02h 
	pop eax
	jz   Salto7 
Salto6: 
	mov eax, 5339DCh    ;OggettoBIK
	mov  eax, dword ptr [eax] 
	test eax, eax 
	jz   Salto1 
	jmp  Salto8 
Salto7: 
 
	mov  esi, edi 
Salto8: 
	mov eax, 48C580h    ;TIME_Init
	call eax 
	mov eax, 478100h    ;FreeMonoScreen
	call eax 
	mov  eax, esi 
	pop  edi 
	push eax
	mov eax, 4BF2A0h   ;TestGrayScreen
	mov  dword ptr [eax], 00h 
	pop eax
	pop  esi 
	ret 
 


	}

}
// crea azione progressiva per variazione fog 
void PreparaVariazioneFog(BYTE TipoVariazioneFog, WORD Timer, WORD ExtraTimer)
{
	int i;
	float NSettori;
	float FogNow;
	/*
	0:Very Slow 
	1:Slow   
	2:Middle
	3:Fast 
	4:Very Fast
	*/
	static WORD VetTempiDensitaFog[] = {6 * FRAME_SECONDO, 
				4 * FRAME_SECONDO, 3 * FRAME_SECONDO,  2 * FRAME_SECONDO,
				FRAME_SECONDO, FRAME_SECONDO / 2};

	StrProgressiveAction *pAzione;

	// prima controllare che non sia gia' presente un azione di variazione
	// fog
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_CHANGE_FOG_DENSITY &&
			GlobTomb4.VetProgressiveActions[i].Bytes.BetaByte == TipoVariazioneFog ) break;
	}

	if (i == GlobTomb4.TotProgressiveActions) {
		i = CreaNuovaAzioneProgressiva();
	}

	pAzione = &GlobTomb4.VetProgressiveActions[i];

	switch (TipoVariazioneFog) {
	case TVF_FOG_END:
		FogNow = GlobTomb4.BaseFog.FloatFogEnd;
		break;

	case TVF_FOG_DISTANCE:
		FogNow = GlobTomb4.BaseFog.FloatFogStart;
		break;
	}

	pAzione->ActionType = AZ_CHANGE_FOG_DENSITY;

	// calcolo di numero di settori
	NSettori = (float) (1 << (Timer & 0x07));
	// memorizzare se e' di tipo random o pulse
	pAzione->Bytes.AlfaByte = Timer & 0x08;
	pAzione->Bytes.BetaByte = TipoVariazioneFog;
	// calcola range minimo e massimo per fog end
	pAzione->VetArgFloat[0] = FogNow - 	(NSettori * 1024.0f) / 2.0f;
	pAzione->VetArgFloat[1] = FogNow + 	(NSettori * 1024.0f) / 2.0f;
	// ora calcolare tempo per il cambio totale dal minimo al massimo
	pAzione->Arg1 = VetTempiDensitaFog[ExtraTimer];

	// ora calcolare incremento
	pAzione->VetArgFloat[2] = (NSettori * 1024.0f) / (float) pAzione->Arg1;
	
	if (pAzione->Bytes.AlfaByte==0) {
		// tipo random
		// stabilire per quanti tick usare l'incremento attuale
		pAzione->ItemIndex = Casuale(pAzione->Arg1 / 2) + FRAME_SECONDO / 2;

		
	}else {
		// tipo pulse
		pAzione->ItemIndex = -1;
	}


}
WORD ConvertiCodText(WORD CodiceTimer)
{
	WORD Codice;

	Codice=CodiceTimer;

	if (Codice == VAR_SPECIAL_INPUT_TEXT) Codice = VAR_TYPE_INPUT_TEXT;
	if (Codice == VAR_SPECIAL_BIG_TEXT) Codice = VAR_TYPE_BIG_TEXT;

	return Codice;
}
// scrive in variabile testuale CodiceVariabile il testo pTesto
void ScriveVariabileTesto(WORD CodiceVariabile, char *pTesto)
{
	int Indice;
	char *pChar;


	switch (CodiceVariabile) {
	case VAR_TYPE_BIG_TEXT:
		pChar = GlobTomb4.pBaseVariableTRNG->Globals.TextBig;
		break;
	case VAR_TYPE_INPUT_TEXT:
		pChar = GlobTomb4.pBaseVariableTRNG->Globals.LastInputText;
		break;
	default:
		Indice = CodiceVariabile & VAR_MASK_INDEX;
		pChar = GlobTomb4.pBaseVariableTRNG->Globals.VetTextVar[Indice].Text;
		break;
	}


	strcpy(pChar, pTesto);
}
// scrive Valore in memoria Codice
void ScriviVariabileMem(WORD Codice, int Valore)
{
	StrAdrMemory *pVet;
	int Indice;
	StrAdrMemory *pMem;
	BYTE *pByte;
	DWORD Base;
	bool TestRelative;


	TestRelative=false;
	// imposta vettore relativo alla memoria da usare
	if (Codice & MEM_TIPO_SAVEGAME) pVet = GlobTomb4.pVetMemorySavegame;
	if (Codice & MEM_TIPO_ITEM) pVet = GlobTomb4.pVetItemMemory;
	if (Codice & MEM_TIPO_CODICE) pVet = GlobTomb4.pVetCodeMemory;
	if (Codice & MEM_TIPO_SLOT) pVet = GlobTomb4.pVetSlotMemory;
	if (Codice & MEM_TIPO_ANIMATION) pVet= GlobTomb4.pVetMemoryAnimation;
	if (Codice & MEM_TIPO_INVENTORY) pVet = GlobTomb4.pVetMemoryInventory;	

	Indice = Codice & MEM_MASK_INDEX;
	pMem = &pVet[Indice];

	if (Codice & MEM_TIPO_ITEM) {
		pByte = (BYTE *) &GlobTomb4.pAdr->pVetItems[GlobTomb4.ItemIndexSelected];
		Base = (DWORD) pMem->pByte;
		pByte += Base;
		TestRelative =true;
	}
	if (Codice & MEM_TIPO_INVENTORY) {
		TestRelative=true;
		Base = (DWORD) pMem->pByte;
		if (Base == 255) {
			// e' la lista dei flag type in un vettore word separato
			pByte = (BYTE *) &GlobTomb4.pAdr->pVetFlagsInventoryItems[GlobTomb4.CurrentInventoryIndex];

		}else {
			pByte = (BYTE *) &GlobTomb4.pAdr->pVetStructInventoryItems[GlobTomb4.CurrentInventoryIndex];
			pByte += Base;

		}
	}
	if (Codice & MEM_TIPO_SLOT) {
		pByte = (BYTE *) &GlobTomb4.pAdr->pVetSlot[GlobTomb4.CurrentSlotIndex];
		Base = (DWORD) pMem->pByte;
		pByte += Base;
		TestRelative =true;
	}

	if (Codice & MEM_TIPO_ANIMATION) {
		pByte = (BYTE *) &GlobTomb4.pAdr->pVetAnimations[GlobTomb4.CurrentAnimationIndex];
		Base = (DWORD) pMem->pByte;
		pByte += Base;
		TestRelative =true;
	}
	SHOW_DEBUG("{%s} = %d", pMem->pDescrizione, Valore);

	switch (pMem->TipoSize) {
	case 'B':
		if (TestRelative) {
			*pByte = (BYTE) Valore;

			return;
		}
		*pMem->pByte = (BYTE) Valore;

		break;
	case 'S':
		if (TestRelative) {
			*(short*) pByte = (short) Valore;
			return;
		}
		*pMem->pShort = (short) Valore;
		break;
	case 'L':
		if (TestRelative) {
			*(int*) pByte = Valore;
			return;
		}
		*pMem->pLong = Valore;

		if (pMem->pLong == (int *) 0x4B1710) {
			// aggiornare volume musica
			SHOW_DEBUG("Update music volume in bass.dll",0,0);
			AggiornaVolumeBass();
			SetVolumeBass(-1);
		}

		break;
	}


}
// avvia azione per viusaizzare timer trng di tipo CodiceTimer (locale o globale)
// e in posizione StringPos (costanti STRING_...
// CodiceTimer puo' essere:
//  0x33 = global timer
//  0x73 = local timer
void CreaAzionePrintTimer(WORD CodiceTimer, int StringPos)
{
	int IndiceAzione;
	int i;
	bool TestOld;

	StrProgressiveAction *pAzione;

	// se gia' c'era un azione AZ_SHOW_TIMER_TRNG
	// usare quella, in caso contrario crearla nuova
	TestOld=false;
	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
		pAzione = &GlobTomb4.VetProgressiveActions[i];
		if (pAzione->ActionType == AZ_SHOW_TIMER_TRNG && 
			pAzione->ItemIndex == CodiceTimer) {
			TestOld=true;
			break;
		}
	

	}
	if (i == GlobTomb4.TotProgressiveActions) {
		// non c'era, crearla adesso

		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
	}

	pAzione->ActionType = AZ_SHOW_TIMER_TRNG;

	// durata infinita
	pAzione->Arg1 = 0xffff;
	pAzione->Arg2 = FT_DOUBLE_SIZEX;

	// imposta indirizzo testo
	if (CodiceTimer == 0x33) {
		// global timer
		pAzione->VetArg[0] = (int) &BufGlobalTimer[0];
	}else {
		// local timer
		pAzione->VetArg[0] = (int) &BufLocalTimer[0];
	}

	// salvare indice di stringa
	pAzione->ItemIndex  = CodiceTimer;
	// alineamento = centrale

	pAzione->VetArgWord[2] = 0;
	pAzione->VetArgShort[3] =-1;

	// salvare colore attuale per stringhe
	pAzione->VetArgWord[4] = FC_WHITE;

	pAzione->VetArg[3] = StringPos;
	AggiornaPosStringa(pAzione);	
}
// esegue lo switch with id: IdSwitch
void EseguiSwitch(int IdSwitch)
{
	int i;
	StrRecordSwitch *pSwitch;
	int Incremento;
	char MyBufInterno[80];
	int IndiceTriggerGroup;

	i = GlobTomb4.BaseSwitch.VetID[IdSwitch];
	if (i == -1) {
		sprintf(BufferLog,"ERROR: cann't find the Switch=%d  script command",
			IdSwitch);
		InviaLog(BufferLog);
		return;
	}
	SHOW_DEBUG("Perform Switch=%d", IdSwitch,0);
	AddTabLogScript();

	pSwitch = &GlobTomb4.BaseSwitch.VetSwitch[i];

	Incremento=1;
	if (pSwitch->Flags & SWT_BASE_ZERO) Incremento=0;

	if (pSwitch->Flags & SWT_RANDOM_MODE) {
		// in modalita' random non conta la variabile
		i = Casuale(pSwitch->TotIndici);

		SHOW_DEBUG("Random Number = %d", i+Incremento,0);


	}else {
		// modo normale: usare il contenuto della variabile 

		i = LeggiVariabile(pSwitch->Variable);
		SHOW_DEBUG("Value Variable = %d", i, 0);
		i = i-Incremento;
	}

	if (i < 0 || i >= pSwitch->TotIndici) {
		SHOW_DEBUG("The selected position (%d) is out of the range", 
			i+Incremento,0);
		SubTabLogScript();
		return;
	}

	IndiceTriggerGroup = pSwitch->VetIndici[i];

	if (IndiceTriggerGroup == -1) {
		SHOW_DEBUG("The chosen index is IGNORE (-1)",0,0);
	}else {
		sprintf(MyBufInterno, "Switch=%d", pSwitch->IdSwitch);
		GlobTomb4.pMexLastTGCaller = MyBufInterno;

		EseguiTriggerGroup(IndiceTriggerGroup);
	}

	SubTabLogScript();
	


}
// inseerisce in buffer la rappresentazione binaria di Valore
void FormattaBinario(char Buffer[], DWORD Valore)
{

	int i;
	int Indice;

	Indice=0;

	for (i=31;i>=0;i--) {
		if (Valore & (1 << i)) {
			Buffer[Indice++] = '1';
		}else {
			Buffer[Indice++] = '0';
		}
	}
	Buffer[Indice]=0;

}
// esegue gorgo su lara o su veicolo lara se e' su una barca
void EseguiGorgo(StrAIDataTr4 *pCentroAI, 
				 int Diametro, bool TestClockWise)
{
	StrItemTr4 *pItem;
	int Indice;
	bool TestLara;
	WORD OrientSpeed;
	int IncY;
	int MaxSpeed;
	bool TestTiraGiu;
	int IndiceTabella;
	WORD Slot;
	float Rapporto;
	DWORD CenterX, CenterZ;
	int CenterY;
	WORD FlagsLara;
	int ClickDistanza;
	bool TestGalleggia;
	int Distanza;
	int Raggio;
	short Orient;
	int WaterY;
	int Intensita;
	short SpeedRotate;
	int SpeedDistanza;
	int IncX, IncZ;

	// Codice tabella. Formula da usare Indice = (Intensita-1) * 21 + Raggio
	// Valore Hex: Word bassa= OrientSpeed, WordAlta =DistSpeed
	// Rotazione=16   Fattore=0.333
	// vettore speed: 1,3, 5, 7, 9, 11, 13, 15, 17, 20, 23, 26, 29, 32, 35, 39, 43, 48, 53, 60
	static DWORD VetTabella[] ={
0x10000, 0x1028F, 0x10147, 0x100D9, 0x100A3, 0x10082, 0x1006C, 
		0x1005D, 0x10051, 0x10048, 0x10041, 0x1003B, 0x10036, 
		0x10032, 0x1002E, 0x1002B, 0x10028, 0x10026, 0x10024, 
		0x10022, 0x10020, 
	0x30000, 0x3035E, 0x301AC, 0x3011E, 0x300D6, 0x300AB, 0x3008E, 
		0x3007A, 0x3006B, 0x3005F, 0x30055, 0x3004D, 0x30047, 
		0x30041, 0x3003D, 0x30039, 0x30035, 0x30032, 0x3002F, 
		0x3002D, 0x3002A, 
	0x50000, 0x50456, 0x50226, 0x50170, 0x50113, 0x500DC, 0x500B7, 
		0x5009D, 0x50089, 0x5007A, 0x5006E, 0x50064, 0x5005B, 
		0x50054, 0x5004E, 0x50049, 0x50044, 0x50040, 0x5003D, 
		0x50039, 0x50037, 
	0x70000, 0x7051E, 0x7028F, 0x701B4, 0x70147, 0x70105, 0x700D9, 
		0x700BA, 0x700A3, 0x70090, 0x70082, 0x70076, 0x7006C, 
		0x70064, 0x7005D, 0x70057, 0x70051, 0x7004C, 0x70048, 
		0x70044, 0x70041, 
	0x90000, 0x905F4, 0x902FA, 0x901F8, 0x9017A, 0x9012E, 0x900FC, 
		0x900D7, 0x900BC, 0x900A7, 0x90097, 0x90089, 0x9007D, 
		0x90074, 0x9006B, 0x90064, 0x9005E, 0x90058, 0x90053, 
		0x9004F, 0x9004B, 
	0xB0000, 0xB06EB, 0xB0375, 0xB0249, 0xB01B7, 0xB0160, 0xB0124, 
		0xB00FB, 0xB00DB, 0xB00C3, 0xB00AF, 0xB009F, 0xB0092, 
		0xB0086, 0xB007D, 0xB0075, 0xB006D, 0xB0067, 0xB0061, 
		0xB005C, 0xB0057, 
	0xD0000, 0xD07C1, 0xD03E0, 0xD028F, 0xD01EC, 0xD0188, 0xD0147, 
		0xD0118, 0xD00F5, 0xD00D9, 0xD00C4, 0xD00B2, 0xD00A3, 
		0xD0096, 0xD008C, 0xD0082, 0xD007A, 0xD0073, 0xD006C, 
		0xD0067, 0xD0061, 
	0xF0000, 0xF0888, 0xF0444, 0xF02D0, 0xF021D, 0xF01B2, 0xF0168, 
		0xF0135, 0xF010E, 0xF00F0, 0xF00D8, 0xF00C4, 0xF00B4, 
		0xF00A6, 0xF009A, 0xF0090, 0xF0087, 0xF007F, 0xF0078, 
		0xF0071, 0xF006C, 
	0x110000, 0x11097B, 0x1104BD, 0x110329, 0x110259, 0x1101E1, 0x110192, 
		0x110158, 0x11012E, 0x11010B, 0x1100F0, 0x1100DB, 0x1100C9, 
		0x1100B9, 0x1100AC, 0x1100A0, 0x110096, 0x11008D, 0x110085, 
		0x11007E, 0x110078, 
	0x140000, 0x140A3D, 0x14051E, 0x140369, 0x14028F, 0x14020C, 0x1401B4, 
		0x140176, 0x140147, 0x140121, 0x140105, 0x1400ED, 0x1400D9, 
		0x1400C9, 0x1400BA, 0x1400AE, 0x1400A3, 0x140099, 0x140090, 
		0x140089, 0x140082, 
	0x170000, 0x170B21, 0x170590, 0x1703B5, 0x1702C0, 0x170234, 0x1701D7, 
		0x170192, 0x170160, 0x170139, 0x170119, 0x170100, 0x1700EA, 
		0x1700D9, 0x1700C9, 0x1700BB, 0x1700B0, 0x1700A5, 0x17009C, 
		0x170094, 0x17008C, 
	0x1A0000, 0x1A0C30, 0x1A0618, 0x1A0400, 0x1A0303, 0x1A0264, 0x1A0200, 
		0x1A01B4, 0x1A017F, 0x1A0155, 0x1A0132, 0x1A0116, 0x1A00FF, 
		0x1A00EB, 0x1A00DA, 0x1A00CC, 0x1A00BF, 0x1A00B4, 0x1A00AA, 
		0x1A00A1, 0x1A0099, 
	0x1D0000, 0x1D0CCC, 0x1D0666, 0x1D0444, 0x1D0333, 0x1D028F, 0x1D0222, 
		0x1D01D4, 0x1D0199, 0x1D016C, 0x1D0147, 0x1D0128, 0x1D010F, 
		0x1D00FB, 0x1D00E9, 0x1D00D9, 0x1D00CC, 0x1D00C0, 0x1D00B5, 
		0x1D00AC, 0x1D00A3, 
	0x200000, 0x200E38, 0x2006EB, 0x200492, 0x200369, 0x2002B9, 0x200243, 
		0x2001F0, 0x2001B2, 0x200181, 0x20015A, 0x20013B, 0x200121, 
		0x20010B, 0x2000F8, 0x2000E7, 0x2000D9, 0x2000CC, 0x2000C0, 
		0x2000B6, 0x2000AD, 
	0x230000, 0x230F0F, 0x230750, 0x2304D4, 0x2303A8, 0x2302E8, 0x23026A, 
		0x230214, 0x2301D0, 0x23019C, 0x230174, 0x230151, 0x230135, 
		0x23011E, 0x230109, 0x2300F7, 0x2300E8, 0x2300DA, 0x2300CE, 
		0x2300C3, 0x2300B9, 
	0x270000, 0x271000, 0x2707C1, 0x27051E, 0x2703E0, 0x270315, 0x27028F, 
		0x270230, 0x2701EC, 0x2701B4, 0x270188, 0x270164, 0x270147, 
		0x27012E, 0x270118, 0x270105, 0x2700F5, 0x2700E6, 0x2700D9, 
		0x2700CE, 0x2700C4, 
	0x2B0000, 0x2B1111, 0x2B0842, 0x2B0572, 0x2B0410, 0x2B033D, 0x2B02B1, 
		0x2B024E, 0x2B0204, 0x2B01CA, 0x2B019C, 0x2B0176, 0x2B0157, 
		0x2B013E, 0x2B0127, 0x2B0113, 0x2B0102, 0x2B00F2, 0x2B00E5, 
		0x2B00D9, 0x2B00CE, 
	0x300000, 0x301111, 0x300888, 0x3005B0, 0x300444, 0x300369, 0x3002D8, 
		0x300270, 0x300222, 0x3001E5, 0x3001B4, 0x30018D, 0x30016C, 
		0x300150, 0x300138, 0x300123, 0x300111, 0x300101, 0x3000F2, 
		0x3000E5, 0x3000DA, 
	0x350000, 0x351249, 0x350924, 0x3505F4, 0x35047D, 0x35039B, 0x3502FA, 
		0x35028F, 0x35023E, 0x3501FC, 0x3501CA, 0x3501A1, 0x35017D, 
		0x350160, 0x350147, 0x350130, 0x35011E, 0x35010C, 0x3500FE, 
		0x3500F0, 0x3500E4, 
	0x3C0000, 0x3C13B1, 0x3C097B, 0x3C063E, 0x3C04BD, 0x3C03C3, 0x3C031F, 
		0x3C02AA, 0x3C0259, 0x3C0214, 0x3C01DE, 0x3C01B2, 0x3C018F, 
		0x3C0170, 0x3C0155, 0x3C013E, 0x3C012B, 0x3C0119, 0x3C0109, 
		0x3C00FB, 0x3C00EF};


	// scoprire se lara e' su un veicolo o meno
	Indice = *GlobTomb4.pAdr->pVehicleIndex;


	CenterX = pCentroAI->CordX;
	CenterY = pCentroAI->CordY;
	CenterZ = pCentroAI->CordZ;
	Raggio = Diametro >> 1;


	TestGalleggia=true;

	GlobTomb4.BoatSinking.TestGorgo=false;
	GlobTomb4.BoatSinking.TestFinito=false;


	if (Indice == -1) {
		// e' lara da sola
		TestLara=true;
		pItem = GlobTomb4.pAdr->pLara;
		// attivare gorgo solo se lara e' in acqua
		FlagsLara=*GlobTomb4.pAdr->pLaraLocationFlags;

		if (FlagsLara != 1 && FlagsLara != 2) {
			// non e' in acqua: ignorare
			return;
		}
		if (FlagsLara == 1) TestGalleggia=false;

	}else {
		// e' un veicolo, scoprire quale
		pItem = &GlobTomb4.pAdr->pVetItems[Indice];
		Slot = pItem->SlotID;
		TestLara=false;
		// verificare che sia una barca

		if  (Slot != 465 && Slot != 467 && Slot != 492 && 
			Slot != GlobTomb4.BaseIndiciNew.IndiceMotorBoat && 
			Slot != GlobTomb4.BaseIndiciNew.IndiceRubberBoat ) {
			// non e' una barca, quindi ignorare il gorgo
			return;
			
		}


	}
	// prima verifica con distanza raw
	Distanza = DistanzaLaraDaXZ(CenterX, CenterZ);
	if (Distanza > Raggio) return;

	// ora calcolo distanza precisa
	Distanza = DistanzaPrecisaXZ(CenterX, CenterZ, 
					pItem->CordX, pItem->CordZ);

	if (Distanza > Raggio) return;

	GlobTomb4.BoatSinking.TestGorgo=true;

	// ok, e' sotto l'influsso
	// ora calcola l'orientamento relativo attuale
	Orient = mGetAngle(CenterX, CenterZ, 
				pItem->CordX, pItem->CordZ);
	Orient += 0x4000;

	// ora stabilire intnsita
	WaterY = GetWaterHeight(CenterX, CenterY, CenterZ, pCentroAI->RoomIndex);
	if (WaterY == WALL_FLOOR) return;

	Intensita = CenterY - WaterY;
	Intensita = Intensita >> 8;

	// intensita' e' il numero di click di distanza tra
	// superfice acqua e posizione y di AI_LARA_POS del centro
	// almeno un click deve esserci sempre
	if (Intensita < 1) Intensita=1;

	// stabilkire anche un massimo di potenza
	if (Intensita > 20) Intensita = 20;

	// Accedere a dati tabella
	ClickDistanza = Distanza >> 8;
	IndiceTabella = (Intensita -1) * 21 + ClickDistanza;


	if (ClickDistanza == 0) {
		SpeedRotate = 0;
	}else {
		SpeedRotate = (WORD) (VetTabella[IndiceTabella] & 0xffff);
	}
	
	// ora calcolo piu' complicato per speeddistanza
	SpeedDistanza = (VetTabella[IndiceTabella] >> 16);
	// questa e' la distsanza minima da applicarsi solo quando
	// lara e' nel click pu' distante di gorgo attuale
	// ora variabirla proporzionalemnte
	// diciamo che il massimo e' 4 volte il minimo
	MaxSpeed = (SpeedDistanza << 1) + SpeedDistanza;
	
	// adesso creare una speed proporzionale a distnza attuale
	// di lara dal centro in rapporto al valore massimo
	// Raggio = raggio del gorgo attuale
	// Distanza = posizione di lara rispetto a centro
	// nota: se siamo gia' in fase di affondo
	// allora potenziare distanzaspeed per impedire si possa sfugfgire
	if (GlobTomb4.BoatSinking.TestAffonda) {
		SpeedDistanza = 128;
	}else {

		Rapporto = ((float) (Raggio-Distanza) / (float) Raggio);
	
		SpeedDistanza += Float2Int(MaxSpeed* Rapporto);
		// se maggiore di 128 limitarla a 128
		if (SpeedDistanza > 128) SpeedDistanza=128;
	}


	if (TestClockWise) {
		// senso orario
		Orient += SpeedRotate;
		pItem->OrientationH += (SpeedRotate >> 1);
	}else {
		// senso antiorario
		Orient -= SpeedRotate;
		pItem->OrientationH -= (SpeedRotate >> 1);
	}
	

	Distanza -= SpeedDistanza;
	if (Distanza < 0) Distanza =0;
	

	CalcolaIncremento(Orient, &IncX, &IncZ, Distanza);

	pItem->CordX = CenterX + IncX;
	pItem->CordZ = CenterZ + IncZ;

	if (TestLara==false) {
		GlobTomb4.pAdr->pLara->CordX = pItem->CordX;
		GlobTomb4.pAdr->pLara->CordZ = pItem->CordZ;
	}

	// vedere se bisogna far sprofondare lara o barca
	TestTiraGiu=false;


	if (Distanza < 257) {

		// e' arrivata in centro
		// vedere se si puo' risucchiare verso il basso
		if (TestLara && Intensita >=3) TestTiraGiu=true;

		if (TestLara == false) {
			if (Intensita >= 5 && Slot == 492) TestTiraGiu=true;

			if (Intensita >= 8 && 
				(Slot == 467 || Slot == GlobTomb4.BaseIndiciNew.IndiceRubberBoat) ) {
				TestTiraGiu=true;
			}

			if (Intensita >= 12 && 
				(Slot == 465 || Slot == GlobTomb4.BaseIndiciNew.IndiceMotorBoat)) {
				TestTiraGiu=true;
			}
		}

		OrientSpeed = (WORD) (VetTabella[IndiceTabella+1] & 0xffff);

		OrientSpeed = OrientSpeed >> 1;

		// far variare qualcosa di orientmento 
		if (TestClockWise) {
			pItem->OrientationH += (OrientSpeed >> 1);
		}else {
			pItem->OrientationH -= (OrientSpeed >> 1);
		}
		if (TestTiraGiu) {
			if (GlobTomb4.BoatSinking.TestAffonda ==false) {
				// iniziato affondamento
				GlobTomb4.BoatSinking.CordY = pItem->CordY;
				GlobTomb4.BoatSinking.RoomOld = pItem->Room;

				SoundEffect(GlobTomb4.pBaseCustomize->VetCustSFX[TS_WHIRLPOOL_SINKED_LARA], 
						&pItem->CordX, 0);



			}
			GlobTomb4.BoatSinking.TestAffonda=true;
			IncY = CenterY - pItem->CordY;

			if (abs(IncY) < 50) {
				GlobTomb4.BoatSinking.TestFinito=true;
				IncY = 0;
				pItem->CordX = CenterX;
				pItem->CordZ = CenterZ;
			}else {
				GlobTomb4.BoatSinking.TestFinito=false;
				IncY = IncY >> 1;
				if (IncY > 128) IncY = 128;
				if (IncY < -128) IncY = -128;
			}


			if (TestLara == false) {
				// qui bisogna ridurre vitalita'
				GlobTomb4.pAdr->pLara->Health -= 8;
				GlobTomb4.pAdr->pLara->FlagsMain |= 0x10;


				GlobTomb4.BoatSinking.CordY += IncY;


				
			}else {

				// se e' lara, nel caso stia galleggiando
				// attivarfe lo stato di sottacqua
				pItem->CordY += IncY;
				if (FlagsLara == 2) {
					FlagsLara = 1;
				}


				*GlobTomb4.pAdr->pAirAvailable -= 8;
			}
		}
	}
}
// prepara i dati per effetto sprite di movimento
void PreparaSpriteMovimento(StrProgressiveAction *pAzione, StrShowSprite *pSprite)
{
	// mettere origine
	if (pSprite->Flags & FSS_EFFECT_FROM_BOTTOM) {
		pAzione->VetArgFloat[0] = (float) pSprite->Posizione.left;
		pAzione->VetArgFloat[1] = 1000;
		
	}

	if (pSprite->Flags & FSS_EFFECT_FROM_TOP) {
		pAzione->VetArgFloat[0] = (float) pSprite->Posizione.left;
		pAzione->VetArgFloat[1] = - (float) pSprite->Posizione.bottom;
	}

	if (pSprite->Flags & FSS_EFFECT_FROM_LEFT) {
		pAzione->VetArgFloat[0] = - (float) pSprite->Posizione.right;
		pAzione->VetArgFloat[1] = (float) pSprite->Posizione.top;
	}

	if (pSprite->Flags & FSS_EFFECT_FROM_RIGHT) {
		pAzione->VetArgFloat[0] = 1000;
		pAzione->VetArgFloat[1] = (float) pSprite->Posizione.top;
	}

	// calcolare gli incrementi
	pAzione->VetArgFloat[2] = ((float) pSprite->Posizione.left - 
					pAzione->VetArgFloat[0]) / (float) pSprite->ExtraValue;
	pAzione->VetArgFloat[3] = ((float) pSprite->Posizione.top  - 
					pAzione->VetArgFloat[1]) / (float) pSprite->ExtraValue;
	pAzione->Arg2 = pSprite->ExtraValue;


}
// analisi complessa per trovare un punto cqasuale dove far cadere fulmine
// in una zona visibile da lara
StrPosizione * TrovaTargetCasuale(void)
{
	StrAbsBoxCollision LevelBox;
	int i;
	StrRoomTr4 *pRoom;
	int Temp;
	StrItemTr4 *pLara;
	WORD Orient;
	int IncX, IncZ;
	WORD Distanza;
	static StrPosizione Target;
	short Room;
	void *pFloor;


	// prima scoprire i limiti max e min dove e' contenuto l'intero livello
	LevelBox.MinX = 200 * 1024;
	LevelBox.MaxX = 0;

	LevelBox.MinZ = 200 * 1024;
	LevelBox.MaxZ = 0;

	LevelBox.MaxY = -128 * 256;
	LevelBox.MinY = 128 * 256;



	for (i=0;i<*GlobTomb4.pAdr->pTotRooms;i++) {
		pRoom = &GlobTomb4.pAdr->pVetRooms[i];

		if (pRoom->OriginX < LevelBox.MinX) LevelBox.MinX = pRoom->OriginX;

		Temp= pRoom->OriginX + pRoom->X_SizeSectors * 1024;
		if (Temp > LevelBox.MaxX) LevelBox.MaxX = Temp;

		if (pRoom->OriginZ < LevelBox.MinZ) LevelBox.MinZ = pRoom->OriginZ;

		Temp=pRoom->OriginZ + pRoom->Z_SizeSectors * 1024;
		if (Temp > LevelBox.MaxZ) LevelBox.MaxZ = Temp;

		if (pRoom->OrigYTop < LevelBox.MinY) LevelBox.MinY = pRoom->OrigYTop;

		if (pRoom->OrigYBottom > LevelBox.MaxY) LevelBox.MaxY = pRoom->OrigYBottom;
	}
	// aggiustamento per compensare muri
	LevelBox.MinX += 2048;
	LevelBox.MaxX -= 3072;

	LevelBox.MinZ += 2048;
	LevelBox.MaxZ -= 3072;

	LevelBox.MinY += 128;
	LevelBox.MaxY -= 128;


	// trovare posizione lara
	pLara = GlobTomb4.pAdr->pLara;

	// ora scegliere distanza in settori 
	// adesso calcolare distanza
	Distanza = 8 + Casuale(20);

	
	// scegli orientmento casuale
	Orient = pLara->OrientationH + (0x2000  - Casuale(0x4000));
	CalcolaIncremento(Orient, &IncX, &IncZ, Distanza*1024);

	Target.OrgX = pLara->CordX + IncX;
	Target.OrgZ = pLara->CordZ + IncZ;

	// se sono oltre i limtii del livello, metterli entro i limiti
	if (Target.OrgX < LevelBox.MinX) Target.OrgX = LevelBox.MinX;
	if (Target.OrgX > LevelBox.MaxX) Target.OrgX = LevelBox.MaxX;

	if (Target.OrgZ < LevelBox.MinZ) Target.OrgZ = LevelBox.MinZ;
	if (Target.OrgZ > LevelBox.MaxZ) Target.OrgZ = LevelBox.MaxZ;

	Target.OrgY = pLara->CordY;

	Room = pLara->Room;

	pFloor= GetFloor(Target.OrgX , Target.OrgY , Target.OrgZ , &Room);
	if (pFloor) {
		if (Room != pLara->Room) {
			pFloor= GetFloor(Target.OrgX , Target.OrgY , Target.OrgZ , &Room);
		}
		// se stanza e' legale provare ad usare valore floor di questa stanza
		if (Room < *GlobTomb4.pAdr->pTotRooms) {
			Target.OrgY = GlobTomb4.pAdr->pVetRooms[Room].OrigYBottom -256;

		}

	}



	return &Target;



	

}
// calcola coordinata di item (puo' essere moveable, static o AI_OBJECT
// se c'e' errore restituisce false
bool CalcolaCordItem(int Indice, DWORD VetCord[], BYTE *pTestLara)
{

	int i;
	StrItemTr4 *pItem;
	StrMeshInfo *pMeshInfo;
	StrAIDataTr4  *pLaraPos;
	int IndiceRoom;
	int IndiceStatic;
	StrCollisionAbsBox AbsColl;

	if ((Indice & (OTYPE_STATIC | OTYPE_AI_DATA))==0) {
		// e' un moveable
		// devo convertire l'indice
		i = GlobTomb4.VetRemapObjects[Indice];
		if (i < 0 || i >= *GlobTomb4.pAdr->pTotItems) {
			sprintf(BufferLog,"Error: moveable index (%d) of PARAM_LIGHTNING command is wrong", Indice);
			InviaLog(BufferLog);
			return false;
		}

		pItem = &GlobTomb4.pAdr->pVetItems[i];
		// scoprire la dimensione del box visibile
		RuotaBoxCollItem(pItem, &AbsColl);

		VetCord[0] = (AbsColl.MaxX + AbsColl.MinX) / 2;
		VetCord[1] = AbsColl.MinY;
		VetCord[2] = (AbsColl.MaxZ + AbsColl.MinZ) / 2;
		
		if (pItem->SlotID==0) *pTestLara =1; // e' lara

	}

	if (Indice & OTYPE_STATIC) {
		// e' uno static
		Indice &= (OTYPE_STATIC-1);
	
		IndiceRoom = GlobTomb4.VetRemapStatics[Indice].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[Indice].IndiceStatic;
		if (IndiceRoom == -1 || IndiceStatic==-1 || IndiceRoom >= *GlobTomb4.pAdr->pTotRooms) {
			sprintf(BufferLog, "Static index (%d) of PARAM_LIGHTNING command is wrong", Indice);
			InviaLog(BufferLog);
			return false;
		}

		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];

		RuotaBoxCollStatic(pMeshInfo, &AbsColl, true);


		VetCord[0] = (AbsColl.MaxX + AbsColl.MinX) / 2;
		VetCord[1] = AbsColl.MinY;
		VetCord[2] = (AbsColl.MaxZ + AbsColl.MinZ) / 2;

	}

	if (Indice & OTYPE_AI_DATA) {
		// e' un lara_start_pos
		Indice &= (OTYPE_AI_DATA-1);

		pLaraPos = TrovaRecordLaraPos(Indice, -1);
		if (pLaraPos==NULL) {
			sprintf(BufferLog, "Wrong OCB code (%d) to locate LARA_START_POS in PARAM_LIGHTNING", Indice);
			InviaLog(BufferLog);
			return false;
		}
		

		VetCord[0] = pLaraPos->CordX;
		VetCord[1] = pLaraPos->CordY;
		VetCord[2] = pLaraPos->CordZ;
	}
	return true;

}


// corrisponde a DoWeather() di tomb5
// nota qui si possono mettere funzioni per attaccare o visualizzare mesh
// ad esempio con funzione AttaccaMesh

__declspec(naked) void PatchDrawRooms(void)
{
	__asm {

	call DoWeather


	// mostra mie mesh
	call MostraMieMesh
	// codice eliminato a causa di patch
	mov eax, 4885D0h    
	call eax
	mov eax, 437F90h    
	call eax

	retn

	}

}
// aggiunge i cerchietti nell'acqua dove si trova enemy
// se non e' in stanza con acqua non fare niente
void AddRippleToItem(StrItemTr4 *pItem, int Intensity, int Quantity)
{
	StrRoomTr4 *pRoom;
	int WaterY;

	pRoom = &GlobTomb4.pAdr->pVetRooms[pItem->Room];

	if ((pRoom->FlagsRoom & 0x001) == 0) return;

	// trovare altezza massima di stanza acqua
	WaterY = pRoom->OrigYTop;


	SetupRipple(pItem->CordX , WaterY, pItem->CordZ, Intensity, Quantity);

	if (Intensity >= 100) {
		Splash(pItem);
	}
	



}
// attiva look di qualche elemento di cutscene (leading, extra actor or lara)
void AttivaLookCutsceneCamera(int IndexTarget, WORD AngleView, int Distance, WORD FlagsTFC)
{

	StrCutsceneCamera *pCut;
	StrBoxCollisione *pColl;
	StrItemTr4 *pItem;

	pCut = &GlobTomb4.pBaseCutscene->BaseCamera;

	// trasforma distance da click avalori assoluti

	Distance = Distance << 8;
	
	if (AllocaCutsceneCamera()==false) return;

	pCut->AngleViews = AngleView;
	pCut->TargetIndex = IndexTarget;
	pCut->Distance = Distance;
	// azzerare eventuali effetti in corso
	pCut->DistanceEndFrame=0;
	pCut->DistanceInc=0;
	pCut->HeightEndFrame=0;
	pCut->HeightInc=0;
	pCut->Rotate=0;
	pCut->RotateInc=0;
	pCut->RotateEndFrame=0;
	pCut->TestFreeze=false;
	pCut->FreezeEndFrame =0;
	pCut->LastPerformedFrame = 0xffff0000;
	pCut->Flags = FlagsTFC;

	// calcolare cordy di camera
	pItem = &GlobTomb4.pAdr->pVetItems[IndexTarget];
	pColl = GetBestFrame(pItem);
	pCut->Height = pItem->CordY + pColl->MinY;

	// calcolare posizione attuale di camera
	pCut->TestAttiva=true;
	AggiornaCutsceneCamera(pCut);






}
// esegue comando dinamico su cutscene camera attualmente attiva
void CommandForCutsceneCamera(int Comando, int Clicks, int Frames)
{
	StrCutsceneCamera *pCut;
	int Distanza;
	int Orient;
	float FOrient;

	pCut = &GlobTomb4.pBaseCutscene->BaseCamera;
	if (pCut->TestAttiva == false) {
		sprintf(BufferLog,"WARNING: cutscene camera has not yet been engaged. Impossible applying the command %d with %d click distance in %d demo frames",
			Comando, Clicks, Frames);
		InviaLog(BufferLog);
		return;
	}
	
	Distanza= Clicks << 8;

	switch (Comando) {
	case CCT_MOVE_UP:
	case CCT_MOVE_DOWN:
		// diminuire cordy di Click distanza in Frames 
		pCut->HeightEndFrame = GetCutsceneFrame() + Frames;
		pCut->HeightInc = Distanza / (Frames+1);
		if (pCut->HeightInc == 0) pCut->HeightInc = 1;
		if (Comando == CCT_MOVE_UP) pCut->HeightInc = -pCut->HeightInc;
		break;
	case CCT_ZOOM_IN:
	case CCT_ZOOM_OUT:
		// avvicina o allontana camera da target
		pCut->DistanceEndFrame = GetCutsceneFrame() + Frames;
		pCut->DistanceInc = Distanza / (Frames+1);
		if (pCut->DistanceInc == 0) pCut->DistanceInc =1;

		if (Comando == CCT_ZOOM_IN) pCut->DistanceInc = -pCut->DistanceInc;
		break;
	case CCT_ROTATE_LEFT:
	case CCT_ROTATE_RIGHT:
		// ruota a destra o sinistra camera
		if (Clicks==0) {
			pCut->RotateEndFrame = 1000000; // vuol dire infinito
		}else {
			pCut->RotateEndFrame = GetCutsceneFrame() + Frames;
		}

		// calcolare gradi effettivi

		if (Clicks ==0 ) {
			Orient = 0x10000;
		}else {
			FOrient = Clicks * 2730.66666666666666666f;
			Orient = Float2Int(FOrient);
		}
			
		pCut->Rotate=0;
		pCut->RotateInc = Orient / (Frames+1);

		if (Comando == CCT_ROTATE_LEFT) pCut->RotateInc = -pCut->RotateInc;
		break;
	case CCT_FREEZE:
		pCut->TestFreeze=true;
		if (Frames==0) {
			// infinito
			pCut->FreezeEndFrame= 1000000; // vuol dire infinito
		}else {
			pCut->FreezeEndFrame= GetCutsceneFrame() + Frames;
		}
		break;

	}
	

}
// memorizza lo swap di animazioni, seguendo la regola pero' che due swap con gli stessi dati si annullano
// a vicenda e non richiedono piu' di essere memorizzati
void SalvaSwapAnim(WORD Slot, WORD SwapId)
{
	int i;
	int Indice;

	StrMemSwapAnim *pSwapMem;

	for (i=0;i<GlobTomb4.pBaseMemSwapAnim->TotMemSwap;i++) {
		pSwapMem= &GlobTomb4.pBaseMemSwapAnim->VetMemSwapAnim[i];
		
		if (pSwapMem->IdParamSwap == SwapId && pSwapMem->Slot == Slot) {
			// c'era gia'
			break;
		}
	}

	if (i== GlobTomb4.pBaseMemSwapAnim->TotMemSwap) {
		// non c'era: memorizzarlo adesso
		pSwapMem= &GlobTomb4.pBaseMemSwapAnim->VetMemSwapAnim[i];
		pSwapMem->IdParamSwap = SwapId;
		pSwapMem->Slot = Slot;
		GlobTomb4.pBaseMemSwapAnim->TotMemSwap++;
		return;
	}

	// era gia' presente in posizone i: eliminarlo senza mettere in disordine il vettore
	Indice=i;

	GlobTomb4.pBaseMemSwapAnim->TotMemSwap--;

	for (i=Indice;i<GlobTomb4.pBaseMemSwapAnim->TotMemSwap;i++) {
		GlobTomb4.pBaseMemSwapAnim->VetMemSwapAnim[i] = GlobTomb4.pBaseMemSwapAnim->VetMemSwapAnim[i+1];
	}



}
// scambia una singola mesh tra due slot A e B
void SwapSingleMeshItem(short SlotA, short SlotB, int MeshIndex)
{
	int IndiceMeshA;
	int IndiceMeshB;
	StrMeshTr4* Valore;

	IndiceMeshA = GlobTomb4.pAdr->pVetSlot[SlotA].IndexFirstMesh + MeshIndex*2;
	IndiceMeshB = GlobTomb4.pAdr->pVetSlot[SlotB].IndexFirstMesh + MeshIndex*2;


	Valore = GlobTomb4.pAdr->VetMeshPointer[IndiceMeshA];

	GlobTomb4.pAdr->VetMeshPointer[IndiceMeshA]=  GlobTomb4.pAdr->VetMeshPointer[IndiceMeshB];
	GlobTomb4.pAdr->VetMeshPointer[IndiceMeshB]= Valore;

	Valore = GlobTomb4.pAdr->VetMeshPointer[IndiceMeshA+1];

	GlobTomb4.pAdr->VetMeshPointer[IndiceMeshA+1]=  GlobTomb4.pAdr->VetMeshPointer[IndiceMeshB+1];
	GlobTomb4.pAdr->VetMeshPointer[IndiceMeshB+1]= Valore;

}
// -> last added function bodies

// ************************************* TRNG FLIPEFFECT ***************************************************


// riceve in input 1 o 2 per segnalare primo o secondo p-range
// e restituisce l'indiceNG originale corrispondente
int TrovaIndiceNG_P_Frame(int NumeroP)
{
	int i;
	int n;

	n=0;
	for (i=0;i<GlobTomb4.TexAnimate.TotaleRangeNG ;i++) {
		if ((GlobTomb4.TexAnimate.VetInfoRangeAnim[i] & FAN_MASK_ANIM)
			==FAN_P_FRAMES) n++;
		if (n == NumeroP) return i;
	}
	return -1;

}
// riceve in input il record del comando script Parameters=PARAM_INPUT_BOX
void CreaInputBox(StrMyInputBox *pInputBox)
{
	int i;
	// GestioneOverlapImage
	StrScriptImage *pImage;

	// localizzare image collegata con inputbox
	

	i = GlobTomb4.pBaseScriptImages->VetID[pInputBox->IdImage];
	if (i == -1) {
		sprintf(BufferLog,"ERROR: not found ImageID (%d) in InputBox parameter", pInputBox->IdImage);
		InviaLog(BufferLog);
		return;
	}

	pImage = &GlobTomb4.pBaseScriptImages->VetImages[i];
	// ora salvare in variabile globale il puntatore a pInputBox e poi chiamare GestioneOverlapImage()
	GlobTomb4.BaseInputBoxes.pInputBoxNow = pInputBox;

	// se c'e' crossfade fare analisi complessa
	if (pImage->Flags & IF_EFFECT_CROSS_FADE) {
		// avviare azione progressiva per attivare corss fade
		// e poi alla fine ci pensera quella ad visualizzare immagine
		CreateCrossFadeAction(ET_CROSS_FADE_SHOW_IMAGE, pInputBox->IdImage , INPUT_BOX_MODE);
		return;
	}

	GestioneOverlapImage(pImage, INPUT_BOX_MODE);
}
// estrae origine (in formato schermo) di stringa ng (il cui indice e' in pAzione->ItemIndex,
//  e salva i dati in VetArg[4] = CordX e VetArg[5] = CordY
// e imposta il testo (senza formattatori) in VetArg[0] e il tipo di allineamnto in pAzione->VetArgWord[2]
// se la stringa non ha formattatore, restiusice false
bool EstraiFormattatore(StrProgressiveAction *pAzione)
{
	RECT MioRect;
	char MioCar;
	char *pStringa;
	char BufOrgX[40];
	char BufOrgY[40];
	int j;
	int i;
	int OrgX;
	int OrgY;
	char BufferTemp[256];
	bool TestErrore;
	WORD FlagAlign;

	// il formsato e' (primi caratteri della stringa): "#XXX:YYY:testo della stringa"
	pStringa = GetStringaNG(pAzione->ItemIndex);

	if (pStringa==NULL) return false;

	i=0;
	if (pStringa[i++] != '#') return false;
	// di default allineamento left
	FlagAlign = FTS_ALIGN_LEFT;
	j=0;
	// vedere se primo carattere e' letttera per allineamento:  L  R o C
	MioCar = pStringa[i];
	switch (MioCar) {
	case 'L':
	case 'l':
		i++;
		break;
	case 'R':
	case 'r':
		FlagAlign = FTS_ALIGN_RIGHT;
		i++;
		break;
	case 'C':
	case 'c':
		FlagAlign = FTS_ALIGN_CENTER;
		i++;
		break;
	}
	// copiare prima cifra (orgX)
	do {
		MioCar = pStringa[i++];
		if (!(MioCar >= '0' && MioCar <= '9')) break;
		BufOrgX[j++] = MioCar;
	}while(MioCar);

	BufOrgX[j] = 0;

	if (j ==0 || MioCar != ':') {
		// errore di sintassi
		sprintf(BufferTemp, "ERROR: in ng string with index = %d: syntax error in position formatter.",
			pAzione->ItemIndex & 0x7fff);
		InviaLog(BufferTemp);
		return false;
	}

	OrgX = GetNumero(BufOrgX, &TestErrore);

	if (TestErrore==true) {
		// errore di sintassi
		sprintf(BufferTemp, "ERROR: in ng string with index = %d: syntax error in position formatter.",
			pAzione->ItemIndex & 0x7fff);
		InviaLog(BufferTemp);
		return false;
	}
	
	// copaire seconda cifra

	j=0;
	// copiare prima cifra (orgX)
	do {
		MioCar = pStringa[i++];
		if (!(MioCar >= '0' && MioCar <= '9')) break;
		BufOrgY[j++] = MioCar;
	}while(MioCar);

	BufOrgY[j] = 0;
	pStringa = &pStringa[i];


	if (j ==0 || MioCar != ':') {
		// errore di sintassi
		sprintf(BufferTemp, "ERROR: in ng string with index = %d: syntax error in position formatter.",
			pAzione->ItemIndex & 0x7fff);
		InviaLog(BufferTemp);
		return false;
	}

	OrgY = GetNumero(BufOrgY, &TestErrore);

	if (TestErrore==true) {
		// errore di sintassi
		sprintf(BufferTemp, "ERROR: in ng string with index = %d: syntax error in position formatter.",
			pAzione->ItemIndex & 0x7fff);
		InviaLog(BufferTemp);
		return false;
	}	 

	// convertire da microunits a valori screen
	MioRect.left = OrgX;
	MioRect.top = OrgY;
	MioRect.right = 999;
	MioRect.bottom = 999;

	ConvertiRectMicroUnits(&MioRect);

	// salvare valori in pazione di azione
	pAzione->VetArg[4] = MioRect.left;
	pAzione->VetArg[5] = MioRect.top;

	// salvare inizia di stringa escludendo formattatore
	pAzione->VetArg[0] = (int) pStringa;

	// aggiornare flg di allineaento
	pAzione->VetArgWord[2] &= ~(FTS_ALIGN_RIGHT | FTS_ALIGN_CENTER);
	pAzione->VetArgWord[2] |= FlagAlign;
	return true;


}
// qui c'e' tutto (e solo) il codice relativo all'esecuzione di flipeffect standards
// ossia senza callback e diverse engine
// questo e' un codice da esport
int PerformTRNGFlipEffect(int FlipNumber, WORD Timer, WORD Flags)
{
	int RepeatType;
	int ExtraTimer;
	int OldAction;
	StrFlipSwapMesh SwapMesh;	
	static WORD *pMaskSegreti = (WORD*) 0x7F7758; // Savegame_20F_MascheraSegreti
	static BYTE *pTotSegreti = (BYTE*) 0x7F7744; // Savegame_1FB_Segreti
	StrPosizione *pPunto;
	WORD FlagGD;
	int CurrentValue;
	StrScaleMovItem *pMovScale;
	int Variable;
	short Incremento;
	static BYTE *VetVonCroyData = (BYTE*) 0x80E017;
	StrMyInputBox *pInputBox;
	WORD FlagsFCT;
	StrSwapSingleMesh SwapSingle;
	StrItemTr4 *pItem;
	DWORD Distanza;
	short TipoFormattatore;
	char *pText;
	StrDemo *pDemo;
	int n;
	StrParamWText *pWText;
	StrRecordParamLgtn *pLight;
	StrFish *pFish;
	bool TestClockWise;
	static short *pCamera_PosNow_Room = (short *) 0x7FE71C;
	short SalvaRoom;
	char *pChar;
	static BYTE *pValOscilla255 = (BYTE *) 0x4BF2D2; //   ;ValOscilla0_255
	StrShowSprite *pSprite;
	static int *pFlashDurata = (int *) 0x4BF264;
	static BYTE VetFlashRed[] = {0xff, 0xff, 0xff, 0xff, 10 ,240,85 ,35, 78, 140,113};
	static BYTE VetFlashGreen[]={0x40, 0x80, 0xc0, 0xff, 229,29, 234,39, 213,140, 40};
	static BYTE VetFlashBlue[]={ 0x00, 0x00, 0x40, 0xff, 15, 213,89, 232,225,140, 50};
	WORD *pVetIndici;
	int TotIndici;
	StrScaleItem *pScale;
	StrBaseVarAll *pVar;
	short MaxView;
	int SlotSwap;
	static DWORD *pContatoreSchermo = (DWORD *) 0x7FD138;
	static WORD *pValOscilla = (WORD *) 0x4BF2D2;
	bool TestImmediate;
	static DWORD *pTestTorciaAccesa = (DWORD *)  0x536DE0; // TestTorciaAccesa
	char *pMex;
	BYTE *pByte;
	WORD Codice;
	static int *pRequiredItem = (int*) 0x4AC060; // ItemSlotRichiesto
	static char *VetSeparatori[] = {" ", ", ", ". ", ".\n", ".\n\n"};
	static DWORD *VetColoriFog = (DWORD *) 0x4ABE10; // VetColoriFogBulb
	WORD Tempo;
	static DWORD *pDword = (DWORD *) 0x7E70D0; // TipoFaseDraw:
	char *pTesto;
	bool TestAggiornaY;
	StrScriptImage *pImage;
	StrBaseDiario *pDiario;
	int OldRoom;
	BYTE SalvaMicro;
	static float VetIncFloat[] = {1.0f, 0.5f, 0.2f, 2.0f, 3.0f};
	float FloatIncY;
	StrParamPrintText *pPrint;
	short AltezzaY;
	WORD NumAzione;
	StrMeshInfo *pMeshInfo;
	WORD IndiceRoom;
	StrCanaleBass *pCanale;
	StrItemTr4 TempItem;
	StrBassHandles *pBass;
	short IndiceStatic;
	StrAnimFrame *pFrame;
	int Indice;
	char Buffer[80];
	bool TestEsegui;
	float TempFloat;
	WORD NumeroWord;
	WORD TimerFull;
	int Slot;
	DWORD Colore;
	BYTE *pVetBytes;
	short Speed;
	StrItemGroup *pItemGroup;
	short IndiceStringa;
	int IncY;
	short Room;
	int Valore;
	static int VetIncX[] = {-1, -1,  0, 1, 1,  1, 0, -1 };
	static int VetIncZ[] = { 0,  1 , 1, 1, 0, -1,-1, -1};
	int IncX, IncZ;
	StrStatusOrganizer *pStatusOrg;
	void *pFloor;
	static short **p2FloorDataNow = (short **) 0x7FE128;
	static WORD *pZonaSave_11F_Sconosciuto = (WORD*) 0x80DF88;
	static WORD *pZonaSave_10D_Sconosciuto = (WORD*) 0x80DF76;

	static WORD *pZonaSave_123_TestRiponendoArma = (WORD*) 0x80DF8C;
	static WORD *pZonaSave_111_TestRiponeOggetto = (WORD*) 0x80DF7A;
	static WORD *pZonaSave_12D_SlotMeshFondine = (WORD *) 0x80DF96;
	static WORD *pMeshFondina = (WORD *) 0x80DF96; //  ;ZonaSave_12D_SlotMeshFondine
	static WORD *pZonaSave_83_ArmaSullaSchiena = (WORD *) 0x80DEEC;
	static WORD VetPoison[] = {1,80,160,512,2048,4096};
	short *pFloorDataNow;
	StrMeshTr4 *pMeshObj;
	StrAnimFrame *pRange;
	DWORD LaraX, LaraZ;
	DWORD GapX, GapZ, CordX, CordZ;
	int CordY;
	WORD IndiceSlot;

	WORD ValWord;
	static int *pLastFlipEffect = (int *) 0x4ACBFC;
	static short VetSlotArmi[] = {349, 351, 353, 356, 361, 366,-1};
	static short VetSlotMunizioni[] = {350, 352, 354, 355, 357, 358, 359, 
		362, 363, 364, 367,-1};
	StrTexSequence *pTexSeq; 
	WORD IndiceSeq;

	int StartRange;
	int Numero;
	bool TestStop;
	int IndiceAzione;
	int j,i;
	static short ** p2VetSoundIndices = (short **) 0x7F7580;
	short *pVetSoundIndices;

	int StartAnim;
	int IndiceAnim;
	StrProgressiveAction *pAzione;
	StrAIDataTr4  *pLaraPos;

	static BYTE VetSizeChar[] = {0, 
			FT_HALF_SIZEX, 
			FT_HALF_SIZEY,
			FT_HALF_SIZEX+FT_HALF_SIZEY, 
			FT_DOUBLE_SIZEX, 
			FT_DOUBLE_SIZEY, 
			FT_DOUBLE_SIZEX+FT_DOUBLE_SIZEY, 
			FT_HALF_SIZEX+FT_HALF_SIZEY+FT_NO_BORDERS,
			FT_LITTLE_ALWAYS, 
			FT_SIZE_ATOMIC_CHAR};

	static WORD *pStandard_FOV_Valore = (WORD*) 0x80EBB8;
	static StrSoundSample *pVetSoundSample = (StrSoundSample*) 0x52B598;

	static VetPercentuale[] = {1,2,3,4,5,6,7,8,9,
							10,20,30,40,50,60,70,80,90,
							100,200,300,400,500,600,700,800,900,1000};
	// ----- code -----------
	pVetSoundIndices = *p2VetSoundIndices;
	TimerFull = Timer;

	ExtraTimer = Timer >> 8;
	ExtraTimer &= 0x7f;
	Timer &= 0xFF;

	RepeatType=0;
	// se siamo all'interno di controllo dummy abortire
	if (GlobTomb4.BaseSalvaFloor.InsideDummyTrigger) return 1;
	// ----------------------- FLIPEFFECT OLD 0-46 ? -------------------------------------------------------------

	if (FlipNumber <= 46) {
		// eseguire flip standard
		// prima vedere se e' uno di quei flip che richiede
		// come argomento la struttura di un moveable

		__asm {
			pushad
			mov ecx, 4BF2D4h // ValTimerTrigger
			mov ax, word ptr [Timer]
			mov word ptr [ecx], ax

			mov  ecx, 4ABE80h  ; VetSaltiFlipeffectNumber
			mov eax, 80E01Ch    ;Ptr_StrutturaLara]
			mov eax, dword ptr [eax]
			push eax
			lea  eax, FlipNumber
			mov  eax, dword ptr [eax]
			
			call dword ptr [ecx][eax*4]
			add  esp,4
			popad
		}
		// ok, ora scoprire in modo bizzarro
		// se va inserito come single shot temporaneo
		if ((*pLastFlipEffect) != -1) {
			// e' stato lasciato flip, il che vuole 
			// dire (da vedrificare)
			// che si vuole disattivarlo temporaneamente
			RepeatType=1;  // blocco temporaneo

		}
		if (Flags & SCANF_BUTTON_ONE_SHOT) {
			RepeatType=2; // blocco totale
		}
		return RepeatType;
	}
	// ----------------------- FLIPEFFECT NEW (TRNG) -------------------------------------------------------------
	switch (FlipNumber) {
	case 47:
		// 47:Remove &inventory-item from inventory
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		GestionePickups(IndiceSlot,INV_WRITE,0);
		RepeatType=1;
		break;
	case 48:
		// 48:Increase (+1) number of &inventory-item in inventory
		// extra:
		// 0: Hidden way (change number of items with no visible effect)
		// 1: Show animation of item (like it was picked up)
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		
		if (ExtraTimer==0) {
			GestionePickups(IndiceSlot,INV_INCREMENT,0);
		}else {
			AddDisplayPickup(IndiceSlot);
		}
		RepeatType=1;
		break;
	case 49:
		// Decrease (-1) number of &inventory-item in inventory
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		GestionePickups(IndiceSlot,INV_DECREMENT,0);
		RepeatType=1;
		break;	
	case 50:
		// Set (E) number of &inventory-item
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		GestionePickups(IndiceSlot,INV_WRITE,ExtraTimer);
		RepeatType=1;
		break;
	case 51:
		// 51:Keyboard. Disable &keyboard command for (E) time
		Numero = VetCommandKey[Timer];
		RepeatType=1;
		// calcolare tempo.
		// se e' infinito mettere semplicemente in blocca key i valori
		// attuali.
		// se invece e' un tempo preordinato allora creare un azione progressiva
		// allo scopo di riabilitare quel valore quando il tempo e' passato
		Tempo = ExtraTimer;

		GlobTomb4.KeysToStop |= Numero;
		if (Tempo == 0) break;
		// e' un tempo espresso in secondi, moltiplicare per 30

		Tempo *= FRAME_SECONDO;		
		// se eseguito in modo continuo c'e' il problema del sovraccarico
		// di azioni.
		// allora, se c'e' gia' una azione per disabilitare il blocco
		// usare quell'azione e aggiornare solo i secondi
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_BLOCK_DISABLE_KEY &&
				pAzione->VetArg[0] == Numero) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {

			IndiceAzione = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		}
#ifdef DEBUG_ON
		InviaLog("Crea azione AZ_BLOCK_DISABLE_KEY");
#endif
		pAzione->ActionType = AZ_BLOCK_DISABLE_KEY;
		pAzione->VetArg[0] = Numero;
		pAzione->Arg1 = (WORD) Tempo;
		break;
	case 52:
		// 52:Keyboard. Enable newly &keyboard command
		Numero = VetCommandKey[Timer];
		RepeatType=1;
		
		if (Numero == -1) {
			GlobTomb4.KeysToStop = 0;
		}else {
			Numero ^= -1;
			GlobTomb4.KeysToStop &= Numero;
		}
		break;
	case 53:
		// 53:Keyboard. Simulate receivement of &keyboard comand in (E) way
		Numero = VetCommandKey[Timer];
		RepeatType=1;
		if (Numero == -1) break;

		Tempo = VetTempi[ExtraTimer];
		// provare a usare invio singolo quando c'e' invio singolo
		if (Tempo==0) {
			GlobTomb4.KeySingleToSend = Numero;
			break;
		}


		Tempo /= FRAME_SECONDO;

		GlobTomb4.KeysToSend |= Numero;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_BLOCK_SENDING_KEY;
		pAzione->VetArg[0] = Numero;
#ifdef DEBUG_ON
		sprintf(BufferLog,"Avvio di azione per blocco di tasto simulato %d",
			Numero);
		InviaLog(BufferLog);
#endif
		pAzione->Arg1 = (WORD) Tempo;
		break;
	case 54:
		// 54:Screen. Hide screen for &time
		Tempo = Timer;
		RepeatType=1;
		if (ExtraTimer == 0) {
			// azzerare anche schermo
			__asm {
				pushad

				mov eax, 48C640h    ;ClearSurfaces
				call eax
				popad

			}
		}
		GlobTomb4.TestNoUpdate =true;


		if (Tempo == 0) break;

		
		Tempo *= FRAME_SECONDO;
		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_HIDE_SCREEN;

		pAzione->Arg1 = (WORD) Tempo;

		break;
	case 55:
		// 55:Screen. Show screen (to abort Hide Screen effect)
		RepeatType=1;
		GlobTomb4.TestNoUpdate =false;
		break;
	case 56:
	case 57:
		// 56:Animated Textures. Stop animation of &th range
		// 57:Animated Textures. Restart animation of &th range
		RepeatType=1;
		

		if (FlipNumber == 56) 
			TestStop=true;
		else
			TestStop=false;

		for (i=0;i<GlobTomb4.BaseAnimTr4.TotFrameRanges;i++) {
			if (GlobTomb4.BaseAnimTr4.VetFrameRanges[i].IndiceRangeNG == Timer)
				GlobTomb4.BaseAnimTr4.VetFrameRanges[i].TestStop=TestStop;
		}

		for (i=0;i<GlobTomb4.BaseAnimTr4.TotUvRanges;i++) {
			if (GlobTomb4.BaseAnimTr4.VetUVRanges[i].IndiceRangeNG==Timer) 
				GlobTomb4.BaseAnimTr4.VetUVRanges[i].TestStop=TestStop;
		}
		break;
	case 58:
	case 59:
		// 58:Animated Textures. Set &th frame in (E)th position of first p-range
		// 59:Animated Textures. Set &th frame in (E)th position of second p-range
		RepeatType=1;
		if (FlipNumber==58) 
			Numero = 1;
		else 
			Numero=2;

		j = TrovaIndiceNG_P_Frame(Numero);
		if (j == -1) {
			sprintf(BufferLog, "ERROR: cann't find %dth P-Frame", Numero);
			InviaLog(BufferLog);
			break;
		}
		
		Numero=j;

		pFrame = &GlobTomb4.BaseAnimTr4.VetFrameRanges[0];

		for (i=0;i<GlobTomb4.BaseAnimTr4.TotFrameRanges;i++) {
			if (pFrame->TipoAnim ==FAN_P_FRAMES && 
				pFrame->IndiceRangeNG == Numero) {

				pFrame->IndiceToSet = ExtraTimer;
				pFrame->FrameToSet = Timer;
				pFrame->TestStop=false;
			
			}
			pFrame++;
		}
		/*
		if (i == GlobTomb4.BaseAnimTr4.TotFrameRanges) {
			sprintf(BufferLog, "INTERNAL ERROR: cann't find the animated frame corresponding to %d p-frame index",
				Numero);
			InviaLog(BufferLog);
		}
		*/

		break;

	case 60:
		// 60:Animated Textures. Invert scroll direction in &th range
		for (i=0;i<GlobTomb4.BaseAnimTr4.TotUvRanges;i++) {
			if (GlobTomb4.BaseAnimTr4.VetUVRanges[i].IndiceRangeNG == Timer) 
				GlobTomb4.BaseAnimTr4.VetUVRanges[i].UvRotate *= -1;
		}

		break;
	case 61:
		// 61:VolumetricFX. Enable Volumetric FX in current level
		// e disattiva distance fog

		RepeatType=1;
		if (*GlobTomb4.pAdr->pSetting_Volumetric) break;

		// imposta il colore previto per volumtric fx


		// salvare in basefog il valore di fogdistanza
		GlobTomb4.BaseFog.NowStartFog  = (short) (GlobTomb4.BaseFog.FloatFogStart / 1024.0f);

		GlobTomb4.BaseFog.FloatFogStart  = 12 * 1024;

		MaxView = 20;
		if (GlobTomb4.WorldFarView != -1) MaxView = GlobTomb4.WorldFarView;
		if (GlobTomb4.LevelFarView != -1) MaxView = GlobTomb4.LevelFarView;

		GlobTomb4.BaseFog.NowEndFog = MaxView;
		SetFogStart();
		ModificaFogEnd();
		*GlobTomb4.pAdr->pSetting_Volumetric  = 1;

		break;


	case 62:

		// 62:VolumetricFX. Disable Volumetric FX in current level
		if (*GlobTomb4.pAdr->pSetting_Volumetric == 0) break;		
		*GlobTomb4.pAdr->pSetting_Volumetric = 0;

		GlobTomb4.BaseFog.NowEndFog = GlobTomb4.BaseFog.DefMaxScript;
		ModificaFogStart();
		ModificaFogEnd();


		RepeatType=1;
		break;

	case 63:
		// 63:Kill Lara in &way	
		// fa morire lara secondo Timer
		RepeatType=1;
		switch (Timer) {
		case 0:
			// vitalita lara a zero
			GlobTomb4.pAdr->pLara->Health=0;
			break;
		case 1:
			// infiamma lara con morte immediata
			LaraBurn();
			GlobTomb4.pAdr->pLara->Health = 0;
			break;
		case 2:
			// infiamma 
			LaraBurn();
			break;
		case 3:
			// esplosione
			GlobTomb4.pAdr->pFlagsLara2[0] |= 1;
			
			break;
		}
		break;
	case 64:
	case 65:
	case 360:
	case 424:
	case 425:
		// 64:Text. Print ExtraNG &string on screen for (E) seconds
		// 65:Text. Print Standard &string on screen for (E) seconds
		// 360:Text. Print <&>Extra NG String with current settings for infinite time
		// 424:Text. Print unlimited <&>NG Extra string with formatters for position in micro units.
		// 425:Text. Print <&>NG Extra string, with formatters for position in microunits, for (E)seconds.

		RepeatType=1;
		if (FlipNumber == 64 || FlipNumber == 360 || FlipNumber==424 || FlipNumber == 425)  {
			// e' stringa NG segnalarlo con il flag 0x8000
			Timer |= 0x8000;
		}

		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PRINT_STRING &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_PRINT_STRING;

		if (ExtraTimer==0 || FlipNumber == 360 || FlipNumber == 424 ) {
			// durata infinita
			pAzione->Arg1 = 0xffff;
		}else {
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		}

		// dimensione
		pAzione->Arg2 = GlobTomb4.PrintString.FlagsMicro;
		// salvare indice di stringa
		pAzione->ItemIndex  = Timer; 
		// alineamento = centrale

		pAzione->VetArgWord[2] = GlobTomb4.PrintString.Flags;
		// salvare colore attuale per stringhe
		pAzione->VetArgWord[4] = GlobTomb4.PrintString.Colore;
		pAzione->VetArg[3] = GlobTomb4.PrintString.Posizione;

		TipoFormattatore=-1;

		if (FlipNumber == 424 || FlipNumber == 425) {
			// sarebbe una stringa con formattatore ma, nel caso non ci fosse, considerarla come una stringa qualunque
			// verifica se c'e' formattatore
			if (EstraiFormattatore(pAzione) == true) TipoFormattatore=-2;
		}
		pAzione->VetArgShort[3] = TipoFormattatore;

		AggiornaAdrStringa(pAzione);
		AggiornaPosStringa(pAzione);



		break;
	case 66:
		// 66:Text. Set &color and (E)position for next Print String flipeffect
		RepeatType=1;
		GlobTomb4.PrintString.Colore  = Timer;
		GlobTomb4.PrintString.Posizione = ExtraTimer;

		break;
	case 67:
		// 67:Text. Erase all strings showed with Print String effect
		// eleiminare tutte le azioni che hanno un print text in azione
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PRINT_STRING)
					GlobTomb4.VetProgressiveActions[i].ActionType =0;
		}
		break;
	case 68:
		// 68:Sound. Play <&>CD track in (E) way on channel1
		RepeatType=1;
		GlobTomb4.BaseBassHandles.CanaleNow=0x40;
		S_CDPlay(Timer, ExtraTimer);
		break;
	case 69:
		//69:Sound. Stop all CD tracks (channel1 and channel2)
		RepeatType=1;
		StopBassSuoni(-1);

		break;
	case 70:
	case 71:
		// 70:Sound. Play &Sound sample of first group (0-255) for (E) time
		// 71:Sound. Play &Sound sample of second group (256-max) for (E) time
		// AZ_SOUND_SAMPLE
		// se la scelta e' : singola esecuzione (valore 31 allora niente
		// azione progressiva

		RepeatType=1;
		if (FlipNumber == 71) Timer+= 256;
		if (pVetSoundIndices[Timer] <0) {

			sprintf(BufferLog,"Flipeffect %d: sound sample with index = %d is not present",
					FlipNumber,Timer);
			InviaLog(BufferLog);

			break;
		}

		
		SoundEffect(Timer, &GlobTomb4.pAdr->pLara->CordX , 2);	
		// se e' singola esecuzione uscire subito
		if (ExtraTimer == 31) break;




		// creare azione progressiva (che forse non serve se riesco a far funzionare
		// illoop


		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_SOUND_SAMPLE;
		// salvare indice assoluto
		pAzione->ItemIndex = Timer;

		if (ExtraTimer == 0) 
			pAzione->Arg1 = 0xffff;
		else
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;

		break;
	case 72:
	case 73:
		// 72:Sound. Stop &Sound sample of first group (0-255)
		// 73:Sound. Stop &Sound sample of second group (256-max)
		RepeatType=1;
		if (FlipNumber == 73) Timer+= 256;

		StopSoundEffect(Timer);

		pAzione=&GlobTomb4.VetProgressiveActions[0];

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (pAzione->ActionType == 	AZ_SOUND_SAMPLE &&
				pAzione->ItemIndex == Timer) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_SOUND_SAMPLE, i);
			}
			pAzione++;
		}

		break;
	case 74:
		// 74:Sound. Stop all sound samples currently played
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SOUND_SAMPLE)
					GlobTomb4.VetProgressiveActions[i].ActionType =0;
		}
		StopAllSounds();
		break;
	case 75:
		//75:Text. Set blink &status and (E)speed
		RepeatType=1;
		GlobTomb4.PrintString.Flags &= ~0x2000;
		if (Timer) GlobTomb4.PrintString.Flags |= 0x2000;
		GlobTomb4.PrintString.BlinkSpeed = ExtraTimer;

		break;
	case 76:
		// 76:Text. Reset all text formatting settings with default values 
		RepeatType=1;
		RipristinaDefaultPrintString();

		break;
	case 77:
	case 80:
		// 77:Force. Force <&>Animation for lara using (E) slot
		// 80:Lara. Force <&>Animation (256-512) of (E)slot for Lara
		RepeatType=1;
		IndiceSlot = ExtraTimer;
		if (FlipNumber == 80) Timer += 256;


		StartAnim = GlobTomb4.pAdr->pVetSlot[IndiceSlot].IndexFirstAnim;
		IndiceAnim = Timer+StartAnim;
		if (GlobTomb4.pAdr->pLara->AnimationNow == IndiceAnim) break;

		GlobTomb4.pAdr->pLara->FrameNow = GlobTomb4.pAdr->pVetAnimations[IndiceAnim].FrameStart;
		GlobTomb4.pAdr->pLara->StateIdCurrent = GlobTomb4.pAdr->pVetAnimations[IndiceAnim].StateId;

		GlobTomb4.pAdr->pLara->AnimationNow = IndiceAnim;
		break;
	case 78:
		// 78:Lara. Force <&>StateID and (E)next StateID for Lara
		RepeatType=1;
		if (Timer != 127) GlobTomb4.pAdr->pLara->StateIdCurrent = Timer;
		if (ExtraTimer != 127) GlobTomb4.pAdr->pLara->StateIdNext = ExtraTimer;
		break;	
	case 79:
		// 79:Lara. Move Lara in same sector of LARA_START_POS with <&>OCB value
		RepeatType=1;
		// si deve trovare oggetto AI di tipo LARA_START_POS con ocb
		// uguale a Timer
		pLaraPos = TrovaRecordLaraPos(Timer,-1);
		if (pLaraPos==NULL) {
			RepeatType=2;
			break;
		}

		OldRoom = GlobTomb4.pAdr->pLara->Room;
		if (ExtraTimer ==0) {
			// utilizza dati di lara target
			CordX = pLaraPos->CordX;
			CordY  = pLaraPos->CordY;
			CordZ = pLaraPos->CordZ;
			Room = pLaraPos->RoomIndex;

			GlobTomb4.pAdr->pLara->OrientationH  = 
					(WORD) pLaraPos->Orientation;
			TestAggiornaY = false;

		}else {
			// mantiene preceente orientamento e posizione relativa.
			LaraX = GlobTomb4.pAdr->pLara->CordX;
			LaraZ = GlobTomb4.pAdr->pLara->CordZ;
			GapX = LaraX & 0x3ff;
			GapZ = LaraZ & 0x3ff;


			// ora trovare altezza di floor dove c''e lara start pos
			CordX = pLaraPos->CordX;
			CordY = pLaraPos->CordY;
			CordZ = pLaraPos->CordZ;
			Room = pLaraPos->RoomIndex;

			// aggiunge spostamento di offset
			CordX &= ~0x3ff;
			CordZ &= ~0x3ff;
			CordX += GapX;
			CordZ += GapZ;
			TestAggiornaY =true;


		}
		AggiornaPositioneLara(CordX, CordY, CordZ, Room, TestAggiornaY);

		break;
	case 81:
		//
		// VetSizeChar
		// 81:Text. Set <&>size characters for next print string command
		RepeatType=1;
		GlobTomb4.PrintString.FlagsMicro = VetSizeChar[Timer];
		break;
	case 82:
		// 82:Delay. Load <#>level in (E)seconds
		// creare azione progressiva.
		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_DELAY_TRIGGER;
		// in indiceitem il numero del livello
		pAzione->ItemIndex = Timer;
		// in arg1 il tempo da attendere
		pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		// in arg2 il tipo di cosa da attivare (usare codifica di what trigger)
		pAzione->Arg2 = TO_FINISH;
		RepeatType=1;
		break;
	case 83:
		// 83:Lara. Remove weapons or flare from lara's hands
		// vedere se ha armi in mano
		RepeatType=1;
		if (*GlobTomb4.pAdr->pFlagsLaraHands  !=0) {
			// togliere armi
			// eliminare arma corrente
			//*pOggettoPrecendente =0;
			// e ora attivare l'invio di spazio
			GlobTomb4.KeySingleToSend = CMD_DRAW_WEAPON;

		}

		if (*GlobTomb4.pAdr->pFlagsLara & 0x1) {
			*GlobTomb4.pAdr->pWeaponSelected =0;
			GlobTomb4.KeySingleToSend = CMD_DRAW_WEAPON;
			}
		break;
	case 84:
		// 84:CutScene. Set FadeIn of <#>time
		SetScreenFadeIn(Timer);
		RepeatType=1;
		break;
	case 85:
		// 85: cutscnee. set fadeout of <#>time
		SetScreenFadeOut(Timer,ExtraTimer);
		RepeatType=1;
		break;
	case 86:
		// 86:Timer. <&>Enable/disable screen timer
		RepeatType=1;
		if (Timer==0) {
			// hide screen timer
			*GlobTomb4.pAdr->pTimerScreen = 0;
	
		}else {
			// visualizza timer
			*GlobTomb4.pAdr->pTimerScreen=1;

		}
			
		break;
	case 87:
		// 87:AnimCommand. Add to current object the <&>Effect for (E)time
		RepeatType=1;
		// trovare indice di oggetto a cui applicarlo

		Indice = GlobTomb4.ItemIndexCurrent;
		if (Indice == -1) Indice = *GlobTomb4.pAdr->pLaraIndex;
		if (Indice == -1) break;

		AddScriptAddEffect(Indice, Timer+1, ExtraTimer);

		break;	
	case 88:
		// 88:Mirror. (E)Enable/Disable mirror of <&>hidden room 
		RepeatType=1;

		switch (ExtraTimer) {
		case 0:

			// disattivare il mirror con stanza hidden numero Timer
			// mettere stanza tra quelle da non visualizzare
			CambiaStatusMirror(Timer,false);


			break;
		case 1:

			// attivare mirror.
			CambiaStatusMirror(Timer,true);


			break;
		case 2:
			// Riattivare mirror con blink
			// avviare l'azione blink_mirror

			// prima verificare non ci sia gia' un azione in corso
			for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
				if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_BLINK_MIRROR &&
					GlobTomb4.VetProgressiveActions[i].Arg1 == Timer) break;
			}

			if (i <GlobTomb4.TotProgressiveActions) break;
			// azione non c'era
			// vedere pero' che questo mirror non sia gia' attivo
			// se la stanza manca tra quelle disabilitate allora
			// e' inutile
			for (i=0;i<GlobTomb4.TotDisabledMirrors;i++) {
				if (GlobTomb4.VetDisabledMirrors[i] == Timer) break;
			}
			// se la stanza non e' tra quelle da nascondere
			// allora il mirror e' gia' attivo e non c'e' 
			// bisogno di fare altro

			if (i == GlobTomb4.TotDisabledMirrors) break;
			// ok creare azione progressiva
			IndiceAzione = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
			pAzione->Arg1 = Timer; // numero stanza hidden
			pAzione->ActionType = AZ_BLINK_MIRROR;
			pAzione->Arg2 = FRAME_SECONDO * 2; // durata due secondi
			pAzione->ItemIndex = 1;  // numero di frame visibili
			pAzione->VetArg[0] = 1; // status attivare
			pAzione->VetArg[1] = 1;  // valore massimo di cicli

			break;



		}

		break;
	case 89:
		// 89:Lara. Damage Lara, decreasing life by <&>percentage of full vitality in (E)
		// timer = indice a percentuale
		// extratimer:
		// 0:Immediatly, one-shot
		// 1:Continuosly, until lara is over current trigger

		// non eseguire se flag lara gia' morta (o invulnerabilita)
		if ((*GlobTomb4.pAdr->pFlagsLara & 0x08) == 0 && 
							GlobTomb4.pAdr->pLara->Health > 0) { 
			Numero = VetPercentuale[Timer];
			GlobTomb4.pAdr->pLara->Health -= Numero;
			if (GlobTomb4.pAdr->pLara->Health < 0) GlobTomb4.pAdr->pLara->Health=0;
			GlobTomb4.pAdr->pLara->FlagsMain |= 0x10;


		}
		// eseguirlo ancora o meno a seconda di extratimer
		if (ExtraTimer) {
			RepeatType=0;
		}else {
			RepeatType=1;
		}
		break;
	case 90:
		// 90:Lara. Recharge Lara, increase life by <&>percentage of full vitality in (E) way
		if (GlobTomb4.pAdr->pLara->Health > 0) { 
			Numero = VetPercentuale[Timer];
			GlobTomb4.pAdr->pLara->Health += Numero;
			if (GlobTomb4.pAdr->pLara->Health > 1000) GlobTomb4.pAdr->pLara->Health=1000;
			GlobTomb4.pAdr->pLara->FlagsMain |= 0x10;
		}
		// eseguirlo ancora o meno a seconda di extratimer
		if (ExtraTimer) {
			RepeatType=0;
		}else {
			RepeatType=1;
		}
		break;		
	case 91:
		// 91:Lara. Lara invulnerable for <&>time with (E)effect
		if (*GlobTomb4.pAdr->pFlagsLara & 0x08) break;

		Tempo = Timer;

		if (Tempo != 0) {
			// e' un tempo espresso in secondi, moltiplicare per 30

			Tempo *= FRAME_SECONDO;
		}else {
			Tempo = 0xffff;
		}

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == 
						AZ_LARA_INVULNERABLE) break;
		}
		if (i <GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {

			IndiceAzione = CreaNuovaAzioneProgressiva();
		}


		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];	
		pAzione->ActionType = AZ_LARA_INVULNERABLE;
		pAzione->Arg1 = Tempo;
		pAzione->Arg2 = (WORD) GlobTomb4.pAdr->pLara->Health;

		// ora analizzare per effetto
		// 0:No effect
		// 1:Transparency
		// 2:Blinking transparency
		if (ExtraTimer) {
			// vedere se si deve attivare anche un effetto per trasparenza
			for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
				if (GlobTomb4.VetProgressiveActions[i].ActionType == 
								AZ_LARA_TRANSPARENT) break;
			
			}
			if (i <GlobTomb4.TotProgressiveActions) {
				IndiceAzione = i;
			}else {

				IndiceAzione = CreaNuovaAzioneProgressiva();
			}			
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
			pAzione->ActionType = AZ_LARA_TRANSPARENT;
			pAzione->Arg1 = Tempo;
			if (ExtraTimer == 1) {
				pAzione->Arg2 = 0x80;
			}else {
				pAzione->Arg2=0xff;
				pAzione->VetArg[0] = - VetIncTrasparenza[ExtraTimer];;
			}
			pAzione->ItemIndex = ExtraTimer;

		}

		break;
	case 92:
		// 92:Lara. Remove flames from Lara
		RepeatType=1;

		// trovare indice di effetto che corrisponde a fuoco per lara
		Indice= TrovaEffettoFiamme();
		if (Indice == -1) break;

		KillEffect(Indice);
		*GlobTomb4.pAdr->pFlagsLara2 &= ~0x02;
		// vedere se c'e' azione di invulnerabilita
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_LARA_INVULNERABLE) break;
		}

		if (i==GlobTomb4.TotProgressiveActions) {
			// togliere flag di effetto fiamme gia' attaccato
			*GlobTomb4.pAdr->pFlagsLara &= ~0x08;
		}
		GlobTomb4.StatusNG &= ~SNG_IMMORTAL_LARA;

		break;
	case 93:
		// 93:Lara. Remove invulnerable status for Lara
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_LARA_INVULNERABLE) {
				GlobTomb4.VetProgressiveActions[i].ActionType =0;
				
			}
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_LARA_TRANSPARENT) {
				GlobTomb4.VetProgressiveActions[i].ActionType =0;
				GlobTomb4.TestChangeTransparencyLara=false;
			}
		}
		*GlobTomb4.pAdr->pFlagsLara &= ~0x08;
		GlobTomb4.StatusNG &= ~SNG_IMMORTAL_LARA;
		break;

	case 94:
		// 94:Animated Textures. Start <&>Texture Sequence for (E)animated range
		RepeatType=1;


		for (i=0;i<GlobTomb4.BaseAnimTr4.TotFrameRanges;i++) {
			if (GlobTomb4.BaseAnimTr4.VetFrameRanges[i].IndiceRangeNG == ExtraTimer) break;
		}

		if (i==GlobTomb4.BaseAnimTr4.TotFrameRanges) {
#ifdef DEBUG_ON
			sprintf(BufferLog,"ERROR: cann't find anim range %d for texture sequence %d",
				ExtraTimer, Timer);
			InviaLog(BufferLog);
#endif
			break;
		}

		StartRange = i;

		i = GlobTomb4.BaseTexSequence.VetID[Timer];
		if (i == -1) break;

		IndiceSeq = i;

		// ora verificare che non sia gia' attiva quell'azione
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione =&GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TEXTURE_SEQUENCE && 
				pAzione->ItemIndex == IndiceSeq && 
				pAzione->Arg1 == StartRange) break;
		}

		if (i<GlobTomb4.TotProgressiveActions) break;
		
		// creare nuova azione
		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_TEXTURE_SEQUENCE;
		pAzione->ItemIndex = IndiceSeq;
		pAzione->Arg1 = StartRange;
		pAzione->Arg2 =1; // delay per primo frame da eseguire
		pAzione->VetArg[0] = 0; // indice frame now
		pAzione->VetArg[1] = 1; // incfremento +1

		break;

	case 95:
		// 95:Animated Textures. Stop <&>Texture Sequence
		// cercare indice reale di questa texture sequnce da eliminare
		RepeatType=1;
		i = GlobTomb4.BaseTexSequence.VetID[Timer];
		if (i == -1) break;

		IndiceSeq=i;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TEXTURE_SEQUENCE &&
				pAzione->ItemIndex == IndiceSeq) {
			
				pAzione->ActionType =0;
				break;
			}
		}
		if (i == GlobTomb4.TotProgressiveActions) break;

		// adesso se c'era flag specifico per lo stop
		// impostare prima texture della serie.
		pTexSeq = &GlobTomb4.BaseTexSequence.VetSequenze[IndiceSeq];
		pRange = &GlobTomb4.BaseAnimTr4.VetFrameRanges[pAzione->Arg1];
		if (pTexSeq->Flags & SEQ_STOP_AT_FIRST) {
			i = pRange->VetTailIndex[0];
			GlobTomb4.pAdr->VetTexInfo[i] = pRange->VetTexInfoRecords[0];


		}
			
		break;
	case 96:
		// 96:Lara. Disarm Lara in <&>way
		// timer:
		// 0:Remove All. (Weapons + Ammos)
		// 1:Remove only weapons
		RepeatType=1;
		// togliere tutte le armi
		i=0;
		while (VetSlotArmi[i] != -1) {
			GestionePickups(VetSlotArmi[i],INV_WRITE,0);
			i++;
		}
		if (Timer == 0) {
			// rimuovere anche munizioni
			i=0;
			while (VetSlotMunizioni[i] != -1) {
				GestionePickups(VetSlotMunizioni[i],INV_WRITE,0);
				i++;
			}
		}
		// ora impostare fondina vuota
		*pMeshFondina=13;
		// ora azzerare arma attuale
		if (*GlobTomb4.pAdr->pFlagsLaraHands  == 4) 
				*GlobTomb4.pAdr->pFlagsLaraHands =0;

		if (*GlobTomb4.pAdr->pObjInLaraHandsNow < 7) 
					*GlobTomb4.pAdr->pObjInLaraHandsNow=0;
		if (*GlobTomb4.pAdr->pObjInLaraHandsNext < 7) *GlobTomb4.pAdr->pObjInLaraHandsNext=0;

		// forzare mani vuote
		Indice = GlobTomb4.pAdr->pVetSlot[0].IndexFirstMesh;

		GlobTomb4.pAdr->VetMeshLara[MESH_LEFT_HAND] = 
			GlobTomb4.pAdr->VetMeshPointer[Indice+26];
		GlobTomb4.pAdr->VetMeshLara[MESH_RIGHT_HAND] =
			GlobTomb4.pAdr->VetMeshPointer[Indice+20];
		// se c'e' un arma in mano fare le cose per bene.

		*GlobTomb4.pAdr->pFlagsLaraHands =0;
		*GlobTomb4.pAdr->p2CurrentEnemyTarget=NULL;
		*pZonaSave_123_TestRiponendoArma=0;
		*pZonaSave_111_TestRiponeOggetto=0;
		if (*GlobTomb4.pAdr->pWeaponHolding != -1) {
			KillItem(*GlobTomb4.pAdr->pWeaponHolding );
			*GlobTomb4.pAdr->pWeaponHolding = -1;
		}

		*pZonaSave_11F_Sconosciuto=0;
		*pZonaSave_10D_Sconosciuto=0;
		
		*pZonaSave_83_ArmaSullaSchiena=0;
		*GlobTomb4.pAdr->pWeaponSelected =0;

		break;
	case 97:
		// 97:Backup. Save in silent way the current game in <&> backup file
		RepeatType=1;
		GlobTomb4.OperazioneNext = ON_SAVE_BACKGAME;
		GlobTomb4.OperazioneArg = Timer;
		break;
	case 98:
		// 98:Backup. Restore (load) the <&>backup file in (E)way
		// 0: Standard way (progress bar + load camera screen)
		// 1: Hidden screen (let last game screen and load in background)
		// 2: Black screen (Set black screen and no progress bar)
		RepeatType=1;
		if (ExtraTimer) {
			GlobTomb4.OperazioneNext = ON_WAIT_AND_HIDE;
			GlobTomb4.OperazioneCount = 3;
		}
		else {
			GlobTomb4.OperazioneNext = ON_LOAD_BACKGAME;
		}
		GlobTomb4.OperazioneArg = Timer | (ExtraTimer << 8);
		
		break;
	case 99:
		// 99:Lara. Swap meshes of Lara with <&>slot in (E)way
		// extratimer
		// 0: Lara Skin  (standard)
		// 1: Lara Skin + Lara Joints (Slot+1)
		// 2: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2)
		// 3: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2) + ShootingHead (Slot+3)
		// 4: Lara Skin + Shooting Head (Slot+1)
		// 5: Lara Skin + Shooting Head (Slot+1) + Hairs (Slot+2)
		RepeatType=1;
		SlotSwap = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshLara(SlotSwap, ExtraTimer);
		// salvare l'esecuzione
		SwapMesh.SlotItem=0;
		SwapMesh.SlotAltroSlot = SlotSwap;
		SwapMesh.TipoSwapMesh = ExtraTimer;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	
	case 100:
		// 100: AnimCommand. Set single <&>mesh of Lara with mesh got from (E)slot
		RepeatType=1;
		Slot = GlobTomb4.pVetStandardSwapMesh[ExtraTimer];
		switch (Timer) {
		case 15:
			// oggetto sulla schiena
			*pZonaSave_83_ArmaSullaSchiena = Slot;
			break;
		case 16:
			// tipo di holsters
			*pZonaSave_12D_SlotMeshFondine = Slot;
			break;

		default:

			Indice = GlobTomb4.pAdr->pVetSlot[Slot].IndexFirstMesh;

			GlobTomb4.pAdr->VetMeshLara[Timer] =
				GlobTomb4.pAdr->VetMeshPointer[Indice+Timer *2];
			break;
		}
		break;
	case 101:
		// AnimCommand. Active heavy triggers in sector where is Lara
		RepeatType=1;
		Room = GlobTomb4.pAdr->pLara->Room;
		pFloor = GetFloor(GlobTomb4.pAdr->pLara->CordX,
					GlobTomb4.pAdr->pLara->CordY, 
					GlobTomb4.pAdr->pLara->CordZ, &Room);
		GetHeight(pFloor, GlobTomb4.pAdr->pLara->CordX,
					GlobTomb4.pAdr->pLara->CordY, 
					GlobTomb4.pAdr->pLara->CordZ);
		pFloorDataNow=*p2FloorDataNow;

		if (pFloorDataNow) 	TestTriggers(pFloorDataNow, 1,0);

		break;
	case 102:
		// 102:AnimCommand. Turn (facing) current object of <&>degrees in (E)wa
		// 2:  45 degress 
		// 4:  90 degrees
		// 6: 135 degrees
		// 8: 180 degress
		// extra timer
		// 0: Clockwise direction
		// 1: Inverse Clockwise direction

		RepeatType=1;

		ValWord = Timer << 12;
		pItem = &GlobTomb4.pAdr->pVetItems[GlobTomb4.ItemIndexAnimCommand];

		if (ExtraTimer) {
			// sottrarre
			pItem->OrientationH -= (short) ValWord;
		}else {
			// addizionare
			pItem->OrientationH += (short) ValWord;
		}
		break;
	case 103:
		// 103:AnimCommand. Update also original Lara position. (after a SetPosition
		RepeatType=1;
		GlobTomb4.pAdr->pCollisionLara->LaraOldX  =
			GlobTomb4.pAdr->pLara->CordX;
		GlobTomb4.pAdr->pCollisionLara->LaraOldY  =
			GlobTomb4.pAdr->pLara->CordY;
		GlobTomb4.pAdr->pCollisionLara->LaraOldZ  =
			GlobTomb4.pAdr->pLara->CordZ;
		break;
	case 104:
		// 104:Air. Change Infinite Air for Lara underwater in <&>way
		// 0: Disable Infinite air
		// 1: Enable Infinite air
		RepeatType=1;
		if (Timer) 
			GlobTomb4.StatusNG |= SNG_INFINITE_AIR;
		else
			GlobTomb4.StatusNG &= ~SNG_INFINITE_AIR;


		break;
	case 105:
		// 105:Lara. Copy meshes from <&>slot to Lara meshes in (E)way
		// extratimer
		// 0: Lara Skin  (standard)
		// 1: Lara Skin + Lara Joints (Slot+1)
		// 2: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2)
		// 3: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2) + ShootingHead (Slot+3)
		// 4: Lara Skin + Shooting Head (Slot+1)
		// 5: Lara Skin + Shooting Head (Slot+1) + Hairs (Slot+2)
		RepeatType=1;
		ExtraTimer |= 0x4000;
		SlotSwap = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshLara(SlotSwap, ExtraTimer);
		// salvare la copia
		SwapMesh.SlotItem=0;
		SwapMesh.SlotAltroSlot = SlotSwap;
		SwapMesh.TipoSwapMesh = ExtraTimer;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 106:
		// 106:Lara. Backup meshes of Lara in <&>slot in (E)way
		// extratimer
		// 0: Lara Skin  (standard)
		// 1: Lara Skin + Lara Joints (Slot+1)
		// 2: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2)
		// 3: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2) + ShootingHead (Slot+3)
		// 4: Lara Skin + Shooting Head (Slot+1)
		// 5: Lara Skin + Shooting Head (Slot+1) + Hairs (Slot+2)
		RepeatType=1;
		ExtraTimer |= 0x8000;
		SlotSwap = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshLara(SlotSwap, ExtraTimer);
		// salvare la copia
		SwapMesh.SlotItem=0;
		SwapMesh.SlotAltroSlot = SlotSwap;
		SwapMesh.TipoSwapMesh = ExtraTimer;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 107:
		// 107:Lara. <&>Enable/disable usage of weapons for Lara
		

		RepeatType=1;
		if (Timer ==0) {
			// ripristina uso di armi
			GlobTomb4.StatusNG &= ~SNG_DISABLE_WEAPONS;

			break;
		}

		// se invece bisogna disabilitare uso armi e' piu' complicato
		GlobTomb4.StatusNG |= SNG_DISABLE_WEAPONS;

		// ora impostare fondina vuota
		*pMeshFondina=13;
		// ora azzerare arma attuale
		if (*GlobTomb4.pAdr->pFlagsLaraHands  == 4) 
				*GlobTomb4.pAdr->pFlagsLaraHands =0;

		if (*GlobTomb4.pAdr->pObjInLaraHandsNow < 7) 
					*GlobTomb4.pAdr->pObjInLaraHandsNow=0;
		if (*GlobTomb4.pAdr->pObjInLaraHandsNext < 7) *GlobTomb4.pAdr->pObjInLaraHandsNext=0;

		// forzare mani vuote
		Indice = GlobTomb4.pAdr->pVetSlot[0].IndexFirstMesh;

		GlobTomb4.pAdr->VetMeshLara[MESH_LEFT_HAND] = 
			GlobTomb4.pAdr->VetMeshPointer[Indice+26];
		GlobTomb4.pAdr->VetMeshLara[MESH_RIGHT_HAND] =
			GlobTomb4.pAdr->VetMeshPointer[Indice+20];
		// se c'e' un arma in mano fare le cose per bene.

		*GlobTomb4.pAdr->pFlagsLaraHands =0;
		*GlobTomb4.pAdr->p2CurrentEnemyTarget=NULL;
		*pZonaSave_123_TestRiponendoArma=0;
		*pZonaSave_111_TestRiponeOggetto=0;
		if (*GlobTomb4.pAdr->pWeaponHolding != -1) {
			KillItem(*GlobTomb4.pAdr->pWeaponHolding );
			*GlobTomb4.pAdr->pWeaponHolding = -1;
		}

		*pZonaSave_11F_Sconosciuto=0;
		*pZonaSave_10D_Sconosciuto=0;
		
		*pZonaSave_83_ArmaSullaSchiena=0;
		*GlobTomb4.pAdr->pWeaponSelected =0;
		break;
	case 108:
		// 108:Lara. <&>Hide/Show holster meshes
		RepeatType=1;
		if (Timer) 
			GlobTomb4.StatusNG |= SNG_HIDE_HOLSTERS;
		else
			GlobTomb4.StatusNG &= ~SNG_HIDE_HOLSTERS;


		break;
	case 109:
		// 109:GlobalTriggers. <&>Enable/Disable the (E)GlobalTrigger
		RepeatType=1;
		i=GlobTomb4.pBaseGlobalTriggers->VetID[ExtraTimer];



		if (i == -1) break;
		if (Timer) {
			// abilitare globaltrigger
			GlobTomb4.pBaseGlobalTriggers->VetTriggers[i].Flags &= ~FGT_DISABLED;

		}else {
			// disabilitare global trigger

			GlobTomb4.pBaseGlobalTriggers->VetTriggers[i].Flags |= FGT_DISABLED;

		}

		break;
	case 110:
		// 110:Lara. (Health) Increase air for Lara of <&>units in (E)way
		// extratimer:
		// 0: Continue, until lara remains in current sector (30 times for second)
		// 1: Single recharge. (You can use One-shot button to forbid furhter charging)
		RepeatType=ExtraTimer;
		// 1800 e' il valore massimo per aria
		Valore = GetRecharge(Timer, 1800);

		*GlobTomb4.pAdr->pAirAvailable += Valore;
		if (*GlobTomb4.pAdr->pAirAvailable > 1800) 
				    *GlobTomb4.pAdr->pAirAvailable = 1800;
		break;
	case 111:
		// 111:Lara. (Health) Increase Damage bar of <&>units in (E)way
		RepeatType=ExtraTimer;
		// 1000 e' il valore massimo per damage
		Valore = GetRecharge(Timer, 1000);
		GlobTomb4.DamageRoom.DamValue += (Valore*10);
		if (GlobTomb4.DamageRoom.DamValue > 10000)
				GlobTomb4.DamageRoom.DamValue=10000;
		break;

	case 112:
		// 112:Lara. (Health) Increase Cold bar of <&>units in (E)way
		RepeatType=ExtraTimer;
		// 1000 e' il valore massimo per cold
		Valore = GetRecharge(Timer, 1000);
		GlobTomb4.DamageColdWater.DamValue += (Valore*10);
		if (GlobTomb4.DamageColdWater.DamValue > 10000)
				GlobTomb4.DamageColdWater.DamValue=10000;
		break;
	case 113:
		// 113:Lara. (Health) Poison Lara with <&>intensity of poison
		RepeatType=1;
		Valore = VetPoison[Timer];
		*GlobTomb4.pAdr->pPoison1 = Valore;
		*GlobTomb4.pAdr->pPoison2 = Valore;
		break;
	case 114:
		// 114:Lara. (Health) Remove poison from Lara
		RepeatType=1;
		*GlobTomb4.pAdr->pPoison1 = 0;
		*GlobTomb4.pAdr->pPoison2 = 0;
		break;
	case 115:
		// 115:Room. Set the <&>room as (E)type of room
		RepeatType=1;
		Valore = 1 << ExtraTimer;
		i = TrovaIndiceMainRoom(Timer);
		if (i == -1) break;
		GlobTomb4.pAdr->pVetRooms[i].FlagsRoom |= Valore;
		break;
	case 116:
		// 116:Room. Remove from the <&>room the (E)type of room
		RepeatType=1;
		Valore = 1 << ExtraTimer;
		i = TrovaIndiceMainRoom(Timer);
		if (i == -1) break;
		GlobTomb4.pAdr->pVetRooms[i].FlagsRoom &= ~Valore;
		break;
	case 117:
		// 117:Room. Set Rain/Snow intensity for <&>room with the new (E)intensity value
		RepeatType=1;
		GlobTomb4.VetExtraInfoRoom[Timer].WaterIntensity  = ExtraTimer;
		GlobTomb4.DatiRain.LastRoomCamera = -1;
		GlobTomb4.DatiSnow.LastRoomCamera = -1;

		break;
	case 118:
		// 118:TriggerGroup. Perform <&>TriggerGroup from script.dat in (E)way
		// 0: Mutiple performing (to use when in TriggerGroup there is some condition)
		// 1: Single performing (to use when in TriggerGroup there only commands)
		// 2: Continue performing (it will be always performed until you stop it)
		// in timer=id 
		// prima salvare questo triggergroup come eseguito
		i = GlobTomb4.BaseTG_SingleShotResumed.TGEseguiti;
		GlobTomb4.BaseTG_SingleShotResumed.VetTGEseguiti[i] = Timer;
		GlobTomb4.BaseTG_SingleShotResumed.TGEseguiti++;

		if (ExtraTimer == 2) {
			RepeatType=1;
			CreaTriggerGroupContinuo(Timer);
		}else {
			RepeatType=ExtraTimer;
			GlobTomb4.pMexLastTGCaller = "Flipeffect 118";
			EseguiTriggerGroup(Timer);
		}
		break;
	case 119:
		// 119:Camera. Set current CAMERA as follow camera on(&)Axis until (E)condition
		RepeatType=1;

		// vedere che non sia gia' attiva questa azione
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_CAMERA_AXIS ||
				pAzione->ActionType == AZ_CAMERA_EFFECT) {
				if (pAzione->Arg1 == Timer &&
					pAzione->Arg2 == ExtraTimer) {
					break;
				}else {
#ifdef DEBUG_ON
					InviaLog("FlipEffect 119: avvia camera axes. Trovata altra camera axis attiva: disattivarla");
#endif
					OldAction = pAzione->ActionType;
					pAzione->ActionType=0;

					GestioneCompletedProgActions(OldAction, i);
				}
			}
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_CAMERA_AXIS;
		pAzione->ItemIndex  = -1; // attesa di trovare camera
		pAzione->Arg1 = Timer; // tipo di asse
		pAzione->Arg2 = ExtraTimer;  // tipo di condizione finale
		pAzione->VetArg[0] = 10; // cicli massimi per attesa camera
		pAzione->VetArg[3] = GlobTomb4.pAdr->pLara->Room;
		break;
	case 120:
		// 120:Camera. Stop current follow axis camera and the CAMERA linked with it
		// cercare azione AZ_CAMERA_AXIS
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			Numero=GlobTomb4.VetProgressiveActions[i].ActionType;

			if (Numero == AZ_CAMERA_AXIS || Numero == AZ_CAMERA_TARGET ||
				Numero == AZ_CAMERA_EFFECT) {
				GlobTomb4.VetProgressiveActions[i].ActionType =0;
			}


		}

		*GlobTomb4.pAdr->Camera.pSecondsTimerCamera =1;
		RipristinoBlocchiCamera();

		break;
	case 121:
		// 121:Lara. (Skills) Disable <&>Skill
		RepeatType=1;
		i = 1 << Timer;
		GlobTomb4.TestDisableFeatures |= i;
		break;
	case 122:
		// 122:Lara. (Skills) Restore <&>Skill
		RepeatType=1;
		i = 1 << Timer;
		GlobTomb4.TestDisableFeatures &= ~i;
		break;

	case 123:
		// 123:Camera. Use current CAMERA to perform <&>Effect at (E)distance from target


		RepeatType=1;

		// vedere che non sia gia' attiva questa azione
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_CAMERA_AXIS ||
				pAzione->ActionType == AZ_CAMERA_EFFECT) {
				if (pAzione->Arg1 == Timer &&
					pAzione->Arg2 == ExtraTimer) {
					break;
				}else {
#ifdef DEBUG_ON
					InviaLog("FlipEffect 123: avvia camera effect. Trovata altra camera effect attiva: disattivarla");
#endif
					OldAction = pAzione->ActionType;
					pAzione->ActionType=0;
					GestioneCompletedProgActions(OldAction, i);
				}
			}
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_CAMERA_EFFECT;
		pAzione->ItemIndex  = -1; // attesa di trovare camera
		pAzione->Arg1 = Timer; // tipo di effetto
		pAzione->Arg2 = (ExtraTimer+1) * 256;;  // distanza
		pAzione->VetArg[0] = 10; // cicli massimi per attesa camera
		break;
	case 124:
		// 124:FlipMap. Enable <&>flipmap with (E)buttons for activation
		RepeatType=1;
		AttivaFlipMap(Timer, ExtraTimer << 8);
		break;
	case 125:
		// 125:FlipMap. Perform a FlipOn for <&>flipmap 
		RepeatType=1;
		if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer]) break;
		GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= 0x3e00;
		FlipMap(Timer);
		break;
	case 126:
		// 126:FlipMap. Perform a FlipOff for <&>flipmap
		RepeatType=1;
		if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer]==0) break;
		GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= 0x3e00;
		FlipMap(Timer);
		break;
	case 127:
		// 127:Organizer. Enable <&>Organizer
		RepeatType=1;

		i = GlobTomb4.pBaseOrganizer->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: cann't find organizer to enable with ID=%d", TimerFull);
			InviaLog(BufferLog);
			break;

		}


		pStatusOrg = &GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i];
		if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_DEMO_ORGANIZER) {
			InviaLog("ERROR: attempt to enable a demo organizer. Only a demo cutscene can enable a demo organizer");
			break;
		}


		pStatusOrg->indiceNow =0;
		if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_TICK_TIME) {
			pStatusOrg->StartPerformed = GlobTomb4.pBaseOrganizer->CounterGame;
		}else {
			pStatusOrg->StartPerformed = GlobTomb4.pBaseOrganizer->CounterGame / 30;
		}
		pStatusOrg->Status =1;
		break;
	case 128:
		// 128:Organizer. Disable (and stop) <&>Organizer
		RepeatType=1;

		i = GlobTomb4.pBaseOrganizer->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: cann't find organizer to enable with ID=%d", TimerFull);
			InviaLog(BufferLog);
			break;

		}

		if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_DEMO_ORGANIZER) {
			InviaLog("ERROR: attempt to disable a demo organizer. Only a demo cutscene can stop/disable a demo organizer");
			break;
		}
		GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i].Status =0;


		break;
	case 129:

		// 129:Sound. Play <&>CD track in (E) way on channel2
		RepeatType=1;
		GlobTomb4.BaseBassHandles.CanaleNow=1;
		S_CDPlay(Timer, ExtraTimer);
		break;
	case 130:
		// 130:Sound. Stop CD track of <&>Channel
		RepeatType=1;
		StopBassSuoni(Timer);
		break;
	case 131:
	case 132:
		// 131:Sound. (CD) Play <&>Imported file in loop mode on (E)channel
		// 132:Sound. (CD) Play <&>Imported file in single play mode on (E)channel
		RepeatType=1;
		if (FlipNumber == 131) {
			// looped
			i= 1;
		}else {
			// single shot
			i=0;
		}
		GlobTomb4.BaseBassHandles.CanaleNow = ExtraTimer | 0x40;

		if (GlobTomb4.BaseBassHandles.TestPresente==false) break;
		S_CDPlay(Timer | 0x4000, i);
		break;

	case 133:
		// 133:Sound. (CD) Set <&>volume for audio track on (E)channel
		RepeatType=1;

		CambiaVolumeConFade(ExtraTimer, Timer);
		break;

	case 134:
	case 135:
	case 158:
		// 134:Lara. (Physics) Pad Mode. Attract Lara in <&>direction with (E)speed
		// 135:Lara. (Physics) Over Mode. Attract Lara in <&>direction with (E)speed
		// 158:Lara. (Physics) Pad+Over Mode. Attract Lara in <&>direction with (E)speed

		RepeatType=0;
		TestEsegui=true;

		// prima verificare se la condizione e' over/pad e' ok
		switch (FlipNumber) {
		case 134:
			// lara deve toccare
			if (GlobTomb4.pAdr->pLara->CordY < 
					GlobTomb4.pAdr->pLara->HeightFloor) TestEsegui=false;
			break;
		case 135:

			// lara deve stare sopra
			if (GlobTomb4.pAdr->pLara->CordY >= 
					GlobTomb4.pAdr->pLara->HeightFloor) TestEsegui=false;
			break;
		}

		if (TestEsegui==false) break;

		// ok, ora applicare modifica
		IncX = VetIncX[Timer] * ExtraTimer;
		IncZ = VetIncZ[Timer] * ExtraTimer;

		GlobTomb4.pAdr->pLara->CordX += IncX;
		GlobTomb4.pAdr->pLara->CordZ += IncZ;
		break;
	case 136:
		// 136:Lara. (Physics) Jumping. Attract Lara <&>up/down with (E)speed
		// se velocita' verticale e' zero non fare nulla
		RepeatType=0;
		Speed = GlobTomb4.pAdr->pLara->SpeedV;
		if (Speed == 0) break;
		if (Timer & 0x1) {
			// movimento verso il basso
			IncY = ExtraTimer;
			// vedere se caduta giu' solo in fase di caduta
			// allora uscire se speed non e' verso il basso
			if ((Timer & 0x02) == 0 && Speed < 0) break;
			
		}else {
			// movimento verso l'alto
			IncY = -ExtraTimer;
			if ((Timer & 0x02)==0 && Speed >0) break;
		}
		if (Timer & 0x04) {
			// prop
			GlobTomb4.pAdr->pLara->SpeedV  += IncY;
		}else {
			GlobTomb4.pAdr->pLara->CordY  += IncY;
		}
		break;
	case 137:
	case 138:
	case 139:
	case 140:
	case 141:
	case 142:
	case 143:
	case 144:
	case 145:
	case 146:

		// esegui una specifica ACTION su tutti gli indici dell ItemGroup
		RepeatType=1;
		// trovare il numero corrispondente all'azione
		NumAzione = GlobTomb4.BaseItemGroup.VetRemapFlipActions[FlipNumber];
		if (NumAzione == -1) break;

		// ok ora trovare l'itemgroup con gli indici da elaborare

		i = GlobTomb4.BaseItemGroup.VetID[Timer];

		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found ItemGroup with id=%d", Timer);
			InviaLog(BufferLog);
			break;
		}

		pItemGroup = &GlobTomb4.BaseItemGroup.VetItemGroup[i];

		// ok, adesso calcolare quello che sara' il valore di timer
		Timer = NumAzione | (ExtraTimer << 8);
		for (i=0;i<pItemGroup->TotIndici;i++) {
			if (pItemGroup->VetIndici[i] != -1) {
				EsecuzioneActionTrigger(0, Timer, pItemGroup->VetIndici[i], SCANF_SCRIPT_TRIGGER);
				
			}
		}
		break;
	case 147:
		// ferma tutte le azioni progressive di itemgroup
		RepeatType=1;
		// trovare il numero corrispondente all'azione
		NumAzione = 47;

		// ok ora trovare l'itemgroup con gli indici da elaborare

		i = GlobTomb4.BaseItemGroup.VetID[TimerFull];

		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found ItemGroup with id=%d", TimerFull);
			InviaLog(BufferLog);
			break;
		}

		pItemGroup = &GlobTomb4.BaseItemGroup.VetItemGroup[i];

		// ok, adesso calcolare quello che sara' il valore di timer
	
		for (i=0;i<pItemGroup->TotIndici;i++) {
			if (pItemGroup->VetIndici[i] != -1) {
				EsecuzioneActionTrigger(0, NumAzione, pItemGroup->VetIndici[i],SCANF_SCRIPT_TRIGGER);
				
			}
		}
		break;
	case 148:
	case 149:
		// 148:Text. Print ExtraNG <&>string on screen, freeze game and wait Escape
		// 149:Text. Print Standard <&>string on screen, freeze game and wait Escape

					
		RepeatType=1;
		if (FlipNumber == 148) {
			// e' stringa NG segnalarlo con il flag 0x8000
			Timer |= 0x8000;
		}
		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PRINT_STRING &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_PRINT_STRING;

		pAzione->Arg1 = 2;

		// dimensione
		pAzione->Arg2 = GlobTomb4.PrintString.FlagsMicro;
		// salvare indice di stringa
		pAzione->ItemIndex  = Timer; 
		// alineamento = centrale

		pAzione->VetArgWord[2]  = GlobTomb4.PrintString.Flags;
		pAzione->VetArgShort[3] = -1;

		// salvare colore attuale per stringhe
		pAzione->VetArgWord[4] = GlobTomb4.PrintString.Colore;
		pAzione->VetArg[3] = GlobTomb4.PrintString.Posizione;

		AggiornaAdrStringa(pAzione);
		AggiornaPosStringa(pAzione);
		GlobTomb4.OperazioneNext = ON_WAIT_ESCAPE;

	case 150:
		// 150:Weather. Sky, <#>Enable/Disable the (E)Layer1/2
		RepeatType=1;
		ValWord = 8 << ExtraTimer;

		if (Timer) {
			// attivare 
			*GlobTomb4.pAdr->pScriptLevelFlags |= ValWord;


		}else {
			// disattivare
			*GlobTomb4.pAdr->pScriptLevelFlags &= ~ValWord;

		}

		break;
	case 151: 
		// 151:Weather. Lightning, <#>Enabled/Disable the Lightning
		RepeatType=1;
		if (Timer) {
			// attivare lightning
			*GlobTomb4.pAdr->pScriptLevelFlags |= 0x40;
		}else {
			// disatrtivare lightning
			*GlobTomb4.pAdr->pScriptLevelFlags &= ~0x40;
		}

		break;
	case 152:
		// 152:Weather. Sky, Set new <#>Color for (E)Layer1/2
		RepeatType=1;

		i = GlobTomb4.BaseColoriRGB.VetID[Timer];


		if (i == -1) break;
		if (ExtraTimer) {
			// layer2
			*GlobTomb4.pAdr->pColorLayer2 = 
						GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
		}else {
			// layer1
			*GlobTomb4.pAdr->pColorLayer1 = 
						GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
		}
		break;
	case 153:
		// 153:Weather. Sky, Set new <#>Speed for (E)Layer1/2
		RepeatType=1;
		if (ExtraTimer) {
			// layer2
			*GlobTomb4.pAdr->pSpeedLayer2 = (BYTE) Timer;
		}else {
			// layer1
			*GlobTomb4.pAdr->pSpeedLayer1 = (BYTE) Timer;
		}
		break;
	case 154:
	case 155:
		// 154:Weather. Sky, change slowly the color of Layer1 to <#>color in (E)seconds
		// 155:Weather. Sky, change slowly the color of Layer2 to <#>color in (E)seconds
		RepeatType=1;
		i = GlobTomb4.BaseColoriRGB.VetID[Timer];

		
		if (i == -1) break;
		Colore = GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
		// calcolare numero di layer
		j = FlipNumber & 1;

		// creare azione
		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_CHANGE_LAYER_COLOR &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == j) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_CHANGE_LAYER_COLOR;
		pAzione->ItemIndex = j;
		pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		// ora calcolare valori float
		if (j==0) {
			// layer1
			pVetBytes = (BYTE*) GlobTomb4.pAdr->pColorLayer1;
		}else {
			// layer2
			pVetBytes = (BYTE*) GlobTomb4.pAdr->pColorLayer2;
		}
		// copiare i tre valori attuali di rosso verde e blu
		for (i=0;i<3;i++) {
			pAzione->VetArgFloat[i] = (float) pVetBytes[i];
		}
		// ora calcolare differenza di incrementi
		pVetBytes = (BYTE *) &Colore;

		for (i=0;i<3;i++) {
			pAzione->VetArgFloat[i+3] = 
				(float) pVetBytes[i] - pAzione->VetArgFloat[i];
			// ora dividerlo per il tempo
			pAzione->VetArgFloat[i+3] /= (float) pAzione->Arg1;
		}


		break;
	case 156:
		// 156:Weather. Snow. Set <#>new state for Snow in current level
		// Timer:
		// 0: Snow DISABLED
		// 1: Snow SINGLE ROOMS
		// 2: Snow ALL OUTSIDE
		RepeatType=1;
		switch (Timer) {
		case 0:
			// disabilita tutto
			GlobTomb4.pScriptLevelNow->LevelFlags &= ~(fngl_Snow+fngl_Snow_All);
			break;
		case 1:
			// single room
			GlobTomb4.pScriptLevelNow->LevelFlags &= ~fngl_Snow_All;
			GlobTomb4.pScriptLevelNow->LevelFlags |= fngl_Snow;
			break;
		case 2:
			// Snow ALL OUTSIDE
			GlobTomb4.pScriptLevelNow->LevelFlags |= (fngl_Snow+fngl_Snow_All);
			break;
		}
		break;

	case 157:
		// Weather. Rain. Set <#>new state for Rain in current level
		RepeatType=1;
		// #define RAIN_DISABLED 0
		// #define RAIN_SINGLE_ROOMS 1
		// #define RAIN_ALL_OUTSIDE 2
		// fngl_Rain  o  fngl_Rain_All
		switch (Timer) {
		case 0:
			// 0: Rain DISABLED
			GlobTomb4.pScriptLevelNow->LevelFlags &= ~(fngl_Rain + fngl_Rain_All);
			break;
		case 1:
			// RAIN_SINGLE_ROOMS
			GlobTomb4.pScriptLevelNow->LevelFlags &= ~fngl_Rain_All;
			GlobTomb4.pScriptLevelNow->LevelFlags |= fngl_Rain;
			break;
		case 2:
			// RAIN_ALL_OUTSIDE
			GlobTomb4.pScriptLevelNow->LevelFlags |= (fngl_Rain + fngl_Rain_All);
			break;
		}
		break;
	case 159:
		// 159:FarView. Set level far view (max distance) to <#>number of sectors
		RepeatType=1;
		ModificaWorldFarView(Timer,false);
		break;
	case 160:
		// 160:Statics. Explosion. To do explode <#>static
		RepeatType=1;

		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;

		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		if ((pMeshInfo->OCB & OCBS_ATTIVO)==0) break;

		ShatterObject(NULL, pMeshInfo,  0x80, IndiceRoom, -2);
		pMeshInfo->OCB &= ~OCBS_ATTIVO;
		SoundEffect(0x15b, pMeshInfo, 0);
		i = *GlobTomb4.pAdr->pTotDestroyedShatters;
		GlobTomb4.pAdr->pVetRoomOfDestroyedShatter[i] = IndiceRoom;
		GlobTomb4.pAdr->pVetStructOfDestroyedShatter[i] = pMeshInfo;
		(*GlobTomb4.pAdr->pTotDestroyedShatters)++;
		pMeshInfo->OCB |= OCBS_SALVARE;
		break;
	case 161:
	case 162:
		// 161:Statics. Collision. Remove collision from <#>static
		// 162:Statics. Collision. Restore collision for <#>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		if ((pMeshInfo->OCB & OCBS_ATTIVO)==0) break;
		pMeshInfo->OCB |= OCBS_SALVARE;
		if (FlipNumber == 161) 
			pMeshInfo->OCB |= OCBS_NO_COLLISIONI;
		else
			pMeshInfo->OCB &= ~OCBS_NO_COLLISIONI;
		break;
	case 163:
		// 163:Statics. Transparency. Set ICE transparency for <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;

		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		if ((pMeshInfo->OCB & OCBS_ATTIVO)==0) break;
		pMeshInfo->OCB |= OCBS_SALVARE;
		pMeshInfo->OCB &= ~OCBS_TRASPARENZA_GLASS; //toglie trasparenza glass
		pMeshInfo->OCB |= OCBS_TRASPARENZA_ICE; // mette trasparenza ice
		break;
	case 164:
		// 164:Statics. Transparency. Set GLASS transparency for <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		if ((pMeshInfo->OCB & OCBS_ATTIVO)==0) break;
		pMeshInfo->OCB |= OCBS_SALVARE;

		pMeshInfo->OCB &= ~OCBS_TRASPARENZA_ICE; // toglie trasprenza ice
		pMeshInfo->OCB |= OCBS_TRASPARENZA_GLASS; // mette trasparenza glass
		break;
	case 165:
		// 165:Statics. Transparency. Remove any transparency from <&>static
		RepeatType=1;

		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		if ((pMeshInfo->OCB & OCBS_ATTIVO)==0) break;
		pMeshInfo->OCB |= OCBS_SALVARE;

		pMeshInfo->OCB &= ~ (OCBS_TRASPARENZA_ICE+ OCBS_TRASPARENZA_GLASS);
		break;
	case 166:
		// 166:Statics. Move. Move static with data in <&>parameters list
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		i = GlobTomb4.BaseMoveItem.VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found the Parameters=PARAM_MOVE_ITEM,%d command", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		CreaMoveItem(&GlobTomb4.BaseMoveItem.VetMove[i],true);
		break;

	case 167:
		// 167:Moveable. Move. Move moveable with data in <&>paramters list
		RepeatType=1;
		i = GlobTomb4.BaseMoveItem.VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found the Parameters=PARAM_MOVE_ITEM,%d command", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		CreaMoveItem(&GlobTomb4.BaseMoveItem.VetMove[i],false);
		break;

	case 168:
		// 168:Sound. Play <&>sound in single playback of global sound map (v130)
		RepeatType=1;
		SoundEffect(TimerFull, NULL,2);
		break;
	case 169:
		// 169:Lara. (Animation) Force <&>Animation for Lara, preserve StateId
		RepeatType=1;
		EseguiAnimazione(TimerFull, 0,false);
		break;
	case 170:
		// 170:Lara. (Animation) Force <&>Animation for Lara, set new StateId
		RepeatType=1;
		EseguiAnimazione(TimerFull,0, true);
		break;
	case 171:
		// 171:Lara. (Animation) Force <&>Animation for Lara, set neutral StateId 
		RepeatType=1;
		EseguiAnimazione(TimerFull,0,false);
		GlobTomb4.pAdr->pLara->StateIdCurrent = 69;
		break;
	case 172:
		// 172:Statics. Rotation. Rotate static with data of <&>Parameters list
		RepeatType=1;
		i = GlobTomb4.BaseRotateItem.VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found the Paramaters=PARAM_ROTATE_ITEM, %d", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		CreaAzioneRotateItem(&GlobTomb4.BaseRotateItem.VetRotate[i], true);

		DeleteParamCommand(PARAM_ROTATE_ITEM, TimerFull,false,true);


		break;

	case 173:
		// 173:Moveable. Rotation. Rotate moveable using data of <&>Parameters list
		RepeatType=1;
		i = GlobTomb4.BaseRotateItem.VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found the Paramaters=PARAM_ROTATE_ITEM, %d", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		CreaAzioneRotateItem(&GlobTomb4.BaseRotateItem.VetRotate[i], false);

		DeleteParamCommand(PARAM_ROTATE_ITEM, TimerFull,false,true);
		break;
	// ok, fare tutti i nuovi flip per bloccoare rotazione
	// e movimento, poi in action fare quelli per fermare specifici moveable
	case 174:
		// 174:Statics. Rotation. Stop all rotations for <&>Static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;

		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_ROTATE_STATIC &&
				pAzione->ItemIndex == IndiceStatic &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;
	case 175:
		// 175:Statics. Rotation. Stop all rotations of statics in <&>Room
		RepeatType=1;
		IndiceRoom = Timer;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_ROTATE_STATIC &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;		

	case 176:
		// 176:Moveable. Rotation. Stop all rotations of moveables in <&>Room
		RepeatType=1;
		IndiceRoom = Timer;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_ROTATE_MOVEABLE &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;
	case 177:
		// 177:Statics. Move. Stop the motion of <&>Static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_MOVE_STATIC &&
				pAzione->ItemIndex == IndiceStatic &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;
	case 178:
		// 178:Moveable. Move. Stop motions of all moveables in <&>Room
		RepeatType=1;
		IndiceRoom = Timer;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_MOVE_MOVEABLE &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;		
	case 179:
		// 179:Statics. Move. Stop the movements of all statics in <&>Room
		RepeatType=1;
		IndiceRoom = Timer;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_MOVE_STATIC &&
				pAzione->Arg1 == IndiceRoom) pAzione->ActionType =0;
		}
		break;	
	case 180:
		// 180:Statics. Destroy. Explosion of <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		
		if ((pMeshInfo->OCB & OCBS_ATTIVO) ==0) break;


		if (*GlobTomb4.pAdr->pLaraLocationFlags == 0) {
			// terraferma
			TriggerExplosionSparks(pMeshInfo->x , 
					pMeshInfo->y , pMeshInfo->z ,
					3,0xfe,0, IndiceRoom);

			for (i=0;i<3;i++) {
				TriggerExplosionSparks(pMeshInfo->x , 
						pMeshInfo->y , pMeshInfo->z ,	3,0xff,0, IndiceRoom);
			}			
		}else {
			// lara sott'acqua
			TempItem.CordX = pMeshInfo->x;
			TempItem.CordY = pMeshInfo->y;
			TempItem.CordZ = pMeshInfo->z;
			TempItem.Room = IndiceRoom;

			TriggerUnderwaterExplosion(&TempItem, 0);

		}

		ShatterObject(NULL, pMeshInfo,  0x80, IndiceRoom, -2);
		pMeshInfo->OCB &= ~OCBS_ATTIVO;

		i = *GlobTomb4.pAdr->pTotDestroyedShatters;
		GlobTomb4.pAdr->pVetRoomOfDestroyedShatter[i] = IndiceRoom;
		GlobTomb4.pAdr->pVetStructOfDestroyedShatter[i] = pMeshInfo;
		(*GlobTomb4.pAdr->pTotDestroyedShatters)++;
		pMeshInfo->OCB |= OCBS_SALVARE;
		ExplosionFX();
		
		break;
	case 181:
	case 182:
		// 181:Statics. Explosion. Set explosive attribute for <&>static
		// 182:Statics. Explosion. Remove explosive attribute for <&>static

		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMeshInfo->OCB |= OCBS_SALVARE;

		if (FlipNumber == 181)
			pMeshInfo->OCB |= OCBS_EXPLODE;
		else
			pMeshInfo->OCB &= ~OCBS_EXPLODE;
		break;
	case 183:
	case 184:
		// 183:Statics. Poison. Set poison attribute for <&>static
		// 184:Statics. Poison. Remove poison attribute for <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMeshInfo->OCB |= OCBS_SALVARE;

		if (FlipNumber == 183)
			pMeshInfo->OCB |= OCBS_POISON;
		else
			pMeshInfo->OCB &= ~OCBS_POISON;
		break;
	case 185:
	case 186:
		// 185:Statics. Damage. Set damage attribute for <&>static
		// 186:Statics. Damage. Remove damage attribute for <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMeshInfo->OCB |= OCBS_SALVARE;

		if (FlipNumber == 185)
			pMeshInfo->OCB |= OCBS_DAMAGE_LARA;
		else
			pMeshInfo->OCB &= ~OCBS_DAMAGE_LARA;
		break;		
	case 187:
	case 188:
		// 187:Statics. Burning. Set burning attribute for <&>static
		// 188:Statics. Burning. Remove burning attribute for <&>static

		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMeshInfo->OCB |= OCBS_SALVARE;

		if (FlipNumber == 187)
			pMeshInfo->OCB |= OCBS_BURN_LARA;
		else
			pMeshInfo->OCB &= ~OCBS_BURN_LARA;
		break;		
	case 189:
	case 190:
		// 189:Statics. Visibility. Set <&>static as invisible
		// 190:Statics. Visibility. Render newly visibile <&>static
		RepeatType=1;
		if (GlobTomb4.FoundIndexForStaticItem != -1) TimerFull = GlobTomb4.FoundIndexForStaticItem;
		IndiceStatic = GlobTomb4.VetRemapStatics[TimerFull].IndiceStatic;
		IndiceRoom = GlobTomb4.VetRemapStatics[TimerFull].IndiceRoom;
		pMeshInfo = &GlobTomb4.pAdr->pVetRooms[IndiceRoom].Ptr_StaticMesh[IndiceStatic];
		pMeshInfo->OCB |= OCBS_SALVARE;

		if (FlipNumber == 190)
			pMeshInfo->OCB |= OCBS_ATTIVO;
		else
			pMeshInfo->OCB &= ~OCBS_ATTIVO;
		break;	
	case 191:
		// 191:Statics. Color. Change color of static using <&>PARAM_COLOR_ITEM data
		RepeatType=1;
		i = GlobTomb4.BaseColoraItem.VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found the Parameters=PARAM_COLOR_ITEM, %d", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		EseguiCambiaColoreItem(&GlobTomb4.BaseColoraItem.VetColoraItem[i], true);
		DeleteParamCommand(PARAM_COLOR_ITEM, TimerFull,false,true);
		break;
	case 192:
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TRIGGER_GROUP_CONTINUO &&
				pAzione->ItemIndex == TimerFull) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_TRIGGER_GROUP_CONTINUO, i);
				break;
			}
		}
		break;

	case 193:
		// 193:Sound. (CD) Play <&>CD Track on (E)channel with final restore of old track		
		RepeatType=1;
		// prima controllare che davvero ci sia un suono
		// sul canale (ExtraTimer) 
		pBass = &GlobTomb4.BaseBassHandles;
		pCanale = &pBass->VetCanali[ExtraTimer];

		if (pCanale->Canale == 0 ||
			pBass->TestPresente == false ||
			pBass->Proc.BASS_ChannelIsActive(pCanale->Canale) == 0) {

				// non c'era una traccia su questo canale, quindi
				// avviare normalmente il suono e basta
				pBass->CanaleNow=ExtraTimer | 0x40;
				S_CDPlay(Timer, 0);
				break;
		}

		
		
		// va creata un azione che controlla quando finisce il suono
		// cha faccio partire adesso e poi ripristinera'
		// quello precedente
		// se era gia' stato azionato ignorarlo
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			
			if (pAzione->ActionType == AZ_PLAY_CD_RESTORE) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType= AZ_PLAY_CD_RESTORE;
		pAzione->ItemIndex = Timer;
		pAzione->Arg1  = pCanale->NumeroCd;
		pAzione->Bytes.AlfaByte  = ExtraTimer;
		pAzione->Bytes.BetaByte = pCanale->Loop;
		// fare una piccola pausa prima di iniziare il controllo

		pAzione->VetArgWord[2] = 15;
		
		// ok ora catturare la posizione attuale della traccia
		// e anche il suo tipo (loop o meno)
		pAzione->VetArgDword[0] = TrovaPosizioneSuono(pCanale);

		// ok ora devo farpartire il suono
		pBass->CanaleNow=ExtraTimer | 0x40;
		S_CDPlay(Timer, 0);

		break;
	case 194:
		// 194:Weather. Fog. Set <&>Distance for Fog Distance
		RepeatType=1;
		i = 120 - Timer;

		GlobTomb4.BaseFog.NowStartFog  = i;
		ModificaFogStart();
		
		break;
	case 195:
		// 195:Weather. Fog. Change Fog distance to <&>Distance in (E)seconds
		// creare azione che modifica progressivamente la distanza
		// per la fog
		RepeatType=1;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			
			if (pAzione->ActionType == AZ_CHANGE_FOG_DISTANCE) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();	

		pAzione= &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_CHANGE_FOG_DISTANCE;
		pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		TempFloat = (float) (120 - Timer);
		TempFloat *= 1024;

		pAzione->VetArgFloat[0] = GlobTomb4.BaseFog.FloatFogStart;
		pAzione->VetArgFloat[1] = TempFloat;
		pAzione->VetArgFloat[2] = 
			(TempFloat - GlobTomb4.BaseFog.FloatFogStart) / pAzione->Arg1 ;
		break;
	case 196:
		// 196:Weather. Fog. Pulse Fog from current to <&>Distance in (E)seconds

		// per la fog
		RepeatType=1;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			
			if (pAzione->ActionType == AZ_PULSE_FOG_DISTANCE) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();	

		pAzione= &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_PULSE_FOG_DISTANCE;
		pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		pAzione->Arg2 = pAzione->Arg1;
		TempFloat = (float) (120 - Timer);
		TempFloat *= 1024;

		pAzione->VetArgFloat[0] = GlobTomb4.BaseFog.FloatFogStart;
		pAzione->VetArgFloat[1] = TempFloat;
		pAzione->VetArgFloat[2] = 
			(TempFloat - GlobTomb4.BaseFog.FloatFogStart) / pAzione->Arg1 ;
		break;
	case 197:
		// 197:Weather. Fog. <&>Enable/Disable all fog (distance fog or fog bulbs)
		RepeatType=1;
		if (Timer) {
			// disattivare tutto
			// disattiva fog bulb
			*GlobTomb4.pAdr->pSetting_Volumetric = 0;
			// reimposta distanza fog al valore 120
			GlobTomb4.BaseFog.NowStartFog  = 120;
			ModificaFogStart();

		}else {
			// riattivare
			GlobTomb4.BaseFog.NowStartFog  = GlobTomb4.BaseFog.DefMinScript;
			ModificaFogStart();

		}
		break;
	case 198:
		// 198:Weather. Fog. Disable the Pulse Fog effect and set new <&>Fog Distance
		RepeatType=1;

		i = 120 - Timer;

		GlobTomb4.BaseFog.NowStartFog = i;

		ModificaFogStart();

		for (i = 0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PULSE_FOG_DISTANCE) {
				GlobTomb4.VetProgressiveActions[i].ActionType =0;
				break;
			}
		}
		break;
	case 199:
		// 199:Lara. Torch. <&>Light/Put-Out the Torch in the hands of Lara
		// eseguirlo solo se lara ha in mano torcia
		RepeatType=1;
		if (*GlobTomb4.pAdr->pObjInLaraHandsNow != 8) break;
		if (Timer == 1) {
			*GlobTomb4.pAdr->pFlagsLara2 |= 0x20;
		}else
		{
			*GlobTomb4.pAdr->pFlagsLara2 &= ~0x20;
		}
		break;
	case 200:
		// Lara. (Mesh) Torch. <&>Give/Remove Torch to/from hand of Lara
		RepeatType=1;
		if (Timer==0) {
			// togleire torcia
			if (*GlobTomb4.pAdr->pObjInLaraHandsNow != 8) break;
			TogliTorcia();
		}else {
			// mettere torcia
			if (*GlobTomb4.pAdr->pObjInLaraHandsNow == 8) break;
			GetFlameTorch();
		}
		break;
	case 201:
		// 201:Text. Vertical Scrolling of ExtraNG <&>string with (E) speed
		RepeatType=1;
		Timer |= 0x8000;

		// vedere che non sia gia' stata lanciata
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_SCROLLING_VTEXT &&
				pAzione->ItemIndex == Timer) break;
		}
		if (i < GlobTomb4.TotProgressiveActions) break;
		IndiceAzione = CreaNuovaAzioneProgressiva();	

		pAzione= &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_SCROLLING_VTEXT;
		pAzione->ItemIndex = Timer; // indice stringa

		pAzione->Arg2 = (BYTE) GlobTomb4.PrintString.FlagsMicro;
		pAzione->VetArgWord[2]= (GlobTomb4.PrintString.Flags & 
				(FTS_STRETCH_TEXT + FTS_BLINK)) | FTS_ALIGN_CENTER;
		pAzione->VetArgShort[3] = -1;
		pAzione->VetArgWord[4] = GlobTomb4.PrintString.Colore;
		pAzione->VetArgShort[5] = -1; // cordx =-1: sempre centrale allignamento
		pAzione->VetArgShort[6] = 0; // offset per stringa
		pAzione->VetArgShort[7] = 1; // contatore per ritardo
		i = ExtraTimer;
		if (i > 4) i = i-5;

		FloatIncY = VetIncFloat[i];
		if (ExtraTimer > 4){
			// ora cambiare sulla base di schermo attuale
			// converte valore float sulla base di schermo 1024x768
			FloatIncY = RapportoFloatSchermo(*GlobTomb4.pAdr->pSizeScreenY, 
									768.0f, FloatIncY);
			FloatIncY = CorreggiDecimali(FloatIncY);


		}
		
		pAzione->VetArgFloat[4] = FloatIncY;

		pAzione->VetArgFloat[5] = (float) *GlobTomb4.pAdr->pSizeScreenY;       // orgy
		// ora aumentare orgy di altezza di testo
		AggiornaAdrStringa(pAzione);
		pTesto = (char *) pAzione->VetArg[0];
		SalvaMicro  = FlagMicroCaratteri;
		FlagMicroCaratteri = GlobTomb4.PrintString.FlagsMicro;

		GetStringLength(pTesto, NULL, &AltezzaY);
		FlagMicroCaratteri = SalvaMicro;

		pAzione->VetArgFloat[5] += (float) (AltezzaY*2);
		// disattivare controllo
		GlobTomb4.ScrollingLastIndex=-1;
		break;
	case 202:
		// 202:Text. Vertical Scrollling. Abort all vertical scrolling text operations
		RepeatType=1;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SCROLLING_VTEXT) {
				GlobTomb4.VetProgressiveActions[i].ActionType = 0;
			}
		}
		break;
	case 203:
	case 207:
	case 208:
	case 209:
	case 210:
		// 203:Text. Print formatted text with <&>ng string and (E)formatting data
		// 207:Text. Print PSX (&)String with (E)formatting data
		// 208:Text. Print PC (&)String with (E)formatting data
		// 209:Text. Print PC <&>string with (E)formatting data, and wait Escape 
		// 210:Text. Print ExtraNG <&>string with (E)formatting data, and wait Escape

		RepeatType=1;
		switch (FlipNumber) {
		case 203:
		case 210:
			Timer |= 0x8000;
			break;
		case 207:
			Timer += 204;
			break;
		case 208:
		case 209:
			Timer += 300;
			break;
		}

		
		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PRINT_STRING &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_PRINT_STRING;

		i = GlobTomb4.BaseParamPrint.VetID[ExtraTimer];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: missing PARAM_PRINT_TEXT with id=%d", ExtraTimer);
			InviaLog(BufferLog);
			pAzione->ActionType =0;
			break;
		}
		pPrint = &GlobTomb4.BaseParamPrint.VetPrint[i];
		if ( pPrint->DurateTime== -1) {
			// durata infinita
			pAzione->Arg1 = 0xffff;
		}else {
			pAzione->Arg1 = pPrint->DurateTime * FRAME_SECONDO;
		}

		if (FlipNumber == 209 || FlipNumber == 210) {
			pAzione->Arg1 =2;
		}

		// dimensione
		pAzione->Arg2 = pPrint->Formatting.FlagsMicro;

		// salvare indice di stringa
		pAzione->ItemIndex  = Timer; 
		// alineamento = centrale

		pAzione->VetArgWord[2] = pPrint->Formatting.Flags;
		pAzione->VetArgShort[3] = ExtraTimer;

		// salvare colore attuale per stringhe
		pAzione->VetArgWord[4] = pPrint->Formatting.Colore;
		pAzione->VetArg[3] = pPrint->Formatting.Posizione;

		AggiornaAdrStringa(pAzione);
		AggiornaPosStringa(pAzione);

		if (FlipNumber == 209 || FlipNumber==210) {
			GlobTomb4.OperazioneNext = ON_WAIT_ESCAPE;
		}
		break;
	case 204:
		// 204:Text. Print. Remove (&)Extra NG String from screen
		RepeatType=1;
		IndiceStringa= Timer | 0x8000;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_PRINT_STRING &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == IndiceStringa) {

				GlobTomb4.VetProgressiveActions[i].ActionType=0;
				GestioneCompletedProgActions(AZ_PRINT_STRING, i);
	
			}
		}
		break;
	case 205:
		// 205:Text. Vertical Scolling of ExtraNG <&>string with (E)formatting data

		RepeatType=1;

		Timer |= 0x8000;
		
		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SCROLLING_VTEXT &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];


		i = GlobTomb4.BaseParamPrint.VetID[ExtraTimer];
		pAzione->VetArgShort[3] = ExtraTimer;

		if (i == -1) break;
		pPrint = &GlobTomb4.BaseParamPrint.VetPrint[i];

		pAzione->ActionType = AZ_SCROLLING_VTEXT;
		pAzione->ItemIndex = Timer; // indice stringa

		pAzione->Arg2 = (BYTE) pPrint->Formatting.FlagsMicro;
		// porca vacca e' qui il problema
		pAzione->VetArgWord[2]= pPrint->Formatting.Flags;
		pAzione->VetArgWord[4] = pPrint->Formatting.Colore;
		pAzione->VetArgShort[6] = 0; // offset per stringa
		pAzione->VetArgShort[7] = 1; // contatore per ritardo
		// il tipo di speed e' in durate time
		ExtraTimer = 0;
		if (pPrint->DurateTime != -1) ExtraTimer=pPrint->DurateTime;
		i = ExtraTimer;

		if (i > 4) i = i-5;

		FloatIncY = VetIncFloat[i];
		if (ExtraTimer > 4){
			// ora cambiare sulla base di schermo attuale
			// converte valore float sulla base di schermo 1024x768
			FloatIncY = RapportoFloatSchermo(*GlobTomb4.pAdr->pSizeScreenY, 
									768.0f, FloatIncY);
			FloatIncY = CorreggiDecimali(FloatIncY);

		}


		// ora aumentare orgy di altezza di testo
		AggiornaAdrStringa(pAzione);
		AggiornaPosStringa(pAzione);
		// ora e' stata messa orgx in VetArg[4]
		// copiare in VetArgShort[5]
		pAzione->VetArgShort[5] = (short) pAzione->VetArg[4];

		pAzione->VetArgFloat[4] = FloatIncY;
		pAzione->VetArgFloat[5] = (float) *GlobTomb4.pAdr->pSizeScreenY;       // orgy

		pTesto = (char *) pAzione->VetArg[0];
		SalvaMicro  = FlagMicroCaratteri;
		FlagMicroCaratteri = pPrint->Formatting.FlagsMicro;

		GetStringLength(pTesto, NULL, &AltezzaY);
		FlagMicroCaratteri = SalvaMicro;

		pAzione->VetArgFloat[5] += (float) (AltezzaY*2);
		// disattivare controllo
		GlobTomb4.ScrollingLastIndex=-1;
		break;

	case 206:
		// Text. Horizontal Scrolling of ExtraNG <&>string with (E)formatting data
		RepeatType=1;

		Timer |= 0x8000;
		
		// se esiste gia' una stringa con stesso indice 
		// usare quel record
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SCROLLING_HTEXT &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) {
			IndiceAzione = i;
		}else {
			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];


		i = GlobTomb4.BaseParamPrint.VetID[ExtraTimer];
		pAzione->VetArgShort[3] = ExtraTimer;

		if (i == -1) break;
		pPrint = &GlobTomb4.BaseParamPrint.VetPrint[i];

		pAzione->ActionType = AZ_SCROLLING_HTEXT;
		pAzione->ItemIndex = Timer; // indice stringa

		pAzione->Arg2 = (BYTE) pPrint->Formatting.FlagsMicro;

		pAzione->VetArgWord[2]= pPrint->Formatting.Flags;
		pAzione->VetArgWord[4] = pPrint->Formatting.Colore;
		pAzione->VetArgShort[6] = 0; // offset per stringa

		// il tipo di speed e' in durate time
		ExtraTimer = 0;
		if (pPrint->DurateTime != -1) ExtraTimer=pPrint->DurateTime;
		i = ExtraTimer;

		if (i > 4) i = i-5;

		FloatIncY = VetIncFloat[i];
		if (ExtraTimer > 4){
			// ora cambiare sulla base di schermo attuale
			// converte valore float sulla base di schermo 1024x768
			FloatIncY = RapportoFloatSchermo(*GlobTomb4.pAdr->pSizeScreenY, 
									1024.0f, FloatIncY);
			FloatIncY = CorreggiDecimali(FloatIncY);
		}


		// ora aumentare diminuire orgx 
		AggiornaAdrStringa(pAzione);
		AggiornaPosStringa(pAzione);
		// ora e' stata messa orgy in VetArg[5]
		// copiare in VetArgShort[5]
		pAzione->VetArgShort[5] = (short) pAzione->VetArg[5];

		pAzione->VetArgFloat[4] = FloatIncY * 2.0f;
		pAzione->VetArgFloat[5] = (float) *GlobTomb4.pAdr->pSizeScreenX;       // orgy


		break;
	case 211:
		// 211:AnimCommand. Set temporary Free Hands until is performing <&>Animation
		RepeatType=1;
		SospendiHardCoded(TimerFull,-1);
		break;
	case 212:
		// 212:AnimCommand. Set temporary Free Hands for <&>Seconds
		RepeatType=1;
		if (Timer == 0) {
			SospendiHardCoded(-1,-1);
		}else {
			SospendiHardCoded(-1, Timer * 30);
		}
		break;
	case 213:
		// 213:AnimCommand. Remove Free Hands and restore previous status
		// cercare azione
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_TEMP_FREE_HANDS) {
				// imposta come frame alla "1"
				GlobTomb4.VetProgressiveActions[i].Arg1 =1;
				break;
			}
		}
		break;

	case 214:
		// 214:Camera. Change camera mode with <&>Parameters for (E)time
		// creare azione AZ_SET_CAMERA
		RepeatType=1;
		// controllare che non sia gia' stata avviata
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SET_CAMERA) break;
		}
		if (i <GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_SET_CAMERA;
		pAzione->ItemIndex = Timer; // id parameter
		if (ExtraTimer == 0) {
			pAzione->Arg1 = 0xffff;
		}else {
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		}
		break;
	case 215:
		// 215:Camera. Restore camera mode after a "change camera" flipeffect
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SET_CAMERA) {
				GlobTomb4.VetProgressiveActions[i].Arg1 =1;
				break;
			}
		}
		break;
	case 216:
		// 216:AnimCommand. Reset the number of turns (jump power) of ParallelBar
		RepeatType=1;
		GlobTomb4.ParallelBar.TotFrames=0;
		GlobTomb4.ParallelBar.GiriCompleti =0;
		break;
	case 217:
		// 217:Images. Show image with data in <&>Image script command for (E)Seconds
		RepeatType=1;

		// a seconda se e' immagine di tipo pop oppure immagine che blocca
		// esegue codici diversi
		i = GlobTomb4.pBaseScriptImages->VetID[Timer];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: missing Image=%d", Timer);
			InviaLog(BufferLog);
			break;
		}

		pImage = &GlobTomb4.pBaseScriptImages->VetImages[i];


		if (pImage->Flags & IF_POP_IMAGE) {
			if (ExtraTimer == 0) {
				Tempo = 0xffff;
			}else {
				Tempo = ExtraTimer * FRAME_SECONDO;
			}
			AvviaPopUpImage(Timer, pImage, Tempo);
		}else {
			// immagine che copre tutto
			// se c'e' crossfade fare analisi complessa
			if (pImage->Flags & IF_EFFECT_CROSS_FADE) {
				// avviare azione progressiva per attivare corss fade
				// e poi alla fine ci pensera quella ad visualizzare immagine
				CreateCrossFadeAction(ET_CROSS_FADE_SHOW_IMAGE, Timer, ExtraTimer);
				break;
			}

			GestioneOverlapImage(pImage, ExtraTimer);
		}
		break;
	case 218:
		// 218:Images. Remove from screen the current Pop Up Image
		RepeatType=1;

		GlobTomb4.BaseImages.PopUp.Contatore =1;
		GlobTomb4.BaseImages.Effetto.TestAttivo=false;

		break;

	case 219:
		// 219:Diary. Add <&>Extra NG String to (E)Diary
		RepeatType=1;

		pDiario = GetDiarioConID(ExtraTimer);
		if (pDiario == NULL) break;

		NumeroWord = Timer | 0x8000;
		if (pDiario->TotStringhe >= MAX_STRINGHE_DIARIO) {
			sprintf(BufferLog,"ERROR: cann't add extrang string, the diary %d reached max number of strings",
				ExtraTimer);
			InviaLog(BufferLog);
			break;
		}
		// nel caso fosse gia' stata inserita ignorare
		if (pDiario->TotStringhe > 0) {
			i = pDiario->TotStringhe -1;
			if (pDiario->VetStringhe[i] == NumeroWord) break;
		}

		i = pDiario->TotStringhe;
		pDiario->VetStringhe[i] = NumeroWord;
		pDiario->TotStringhe++;
		break;
	case 220:
		// 220:Diary. Clear all strings in <&>Diary
		RepeatType=1;
		pDiario = GetDiarioConID(Timer);
		if (pDiario == NULL) break;
		pDiario->TotStringhe =0;
		break;
	case 221:
		// 221:Diary. Remove last string from <&>Diary
		RepeatType=1;
		pDiario = GetDiarioConID(Timer);
		if (pDiario == NULL) break;
		if (pDiario->TotStringhe > 0) pDiario->TotStringhe--;
		break;
	case 222:
		// 222:Diary. Show <&>Diary at (E)page
		RepeatType=1;
		
		pDiario = GetDiarioConID(Timer);
		if (pDiario == NULL) break;

		GlobTomb4.TestStartDiary=true;
		GlobTomb4.DiaryIDToStart= Timer;
		GlobTomb4.DiaryPage = ExtraTimer;


		break;
	case 223:
		// 223:Show Statistics Screen
		RepeatType=1;
		MostraStatistiche();
		break;
	case 224:
		// 224:Weather. Fog. Set the <&>Color for Distance Fog
		RepeatType=1;
		if (*GlobTomb4.pAdr->pSetting_Volumetric) break;

		pByte = (BYTE *) &VetColoriFog[Timer];
		ModificaColoreFog(pByte[2], pByte[1], pByte[0]);
		break;
	case 225:
		// 225:Weather. Fog. <&>Enable/Disable Hardware Fog (onfly for nightmare effects)
		RepeatType=1;

		// attivare hardweare fog
		GlobTomb4.BaseFog.TestHardFogEnabled = true;

		
		ModificaHardwareFog();
		break;
	case 226:
		// 226:Weather. Fog. Set <&>Max visibility distance for Fog Bulbs
	
		RepeatType=1;
		i = 120 - Timer;
		GlobTomb4.BaseFog.NowFogBulbDistance  = i;

		ModificaFogBulbDistance();

		break;
	case 227:
		// 227:Weather. Fog. Set Hardware <&>End Fog limit
		RepeatType=1;
		i = 120 - Timer;
		GlobTomb4.BaseFog.NowEndFog = i;
		ModificaFogEnd();
		break;
	case 228:
		// Weather. Fog. Change End limit of Distance Fog in <&>way with (E)speed
		RepeatType=1;

		PreparaVariazioneFog(TVF_FOG_END, Timer, ExtraTimer);

		break;
	case 229:

		// Weather. Fog. Change Start Distance Fog in <&>way with (E)speed
		RepeatType=1;
		PreparaVariazioneFog(TVF_FOG_DISTANCE, Timer, ExtraTimer);
		break;
	case 230:
		// 230:Weather. Fog. Stop the <&>Change limit of Distance fog effect
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_CHANGE_FOG_DENSITY &&
				GlobTomb4.VetProgressiveActions[i].Bytes.BetaByte == Timer) {

				GlobTomb4.VetProgressiveActions[i].ActionType =0;
			}
		}
		break;
	case 231:
		// 231:Variables. Numeric. Add to <&>Variable the (E)value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);

		Valore= LeggiVariabile(Codice);
		Valore += ExtraTimer;
		ScriviVariabile(Codice, Valore);
		break;
	case 232:
		// 232:Variables. Numeric. Set <&>Variable with (E)value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = ExtraTimer;
		ScriviVariabile(Codice, Valore);
		break;
	case 233:
		// 233:Variables. Numeric. Substract from <&>Variable the (E)value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Valore -= ExtraTimer;
		ScriviVariabile(Codice,Valore);
		break;
	case 234:
		// 234:Variables. Numeric. Set in <&>Variable the (E)bit
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);		
		Valore = LeggiVariabile(Codice);
		Valore |= (1 << ExtraTimer);
		ScriviVariabile(Codice,Valore);
		break;
	case 235:
		// 235:Variables. Numeric. Clear in <&>Variable the (E)bit
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Valore &= ~(1 << ExtraTimer);
		ScriviVariabile(Codice,Valore);
		break;
	case 236:
		// 236:Variables. Store. Copy CurrentValue to <&>Store variable

		RepeatType=1;
		Codice = Timer | VAR_TYPE_STORE;
		Valore= LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabile(Codice, Valore);
		break;
	case 237:
		// 237:Variables. Store. Copy <&>Store variable to CurrentValue
		RepeatType=1;
		Codice = Timer | VAR_TYPE_STORE;

		Valore = LeggiVariabile(Codice);
		ScriviVariabile(VAR_TYPE_CURRENT_VALUE, Valore);
		break;

	case 238:
		// 238:Variables. Text. Copy the <&>Text Variable to (E)Text Variable
		RepeatType=1;
		Codice = ConvertiCodText(Timer);
		pChar = LeggiVariabileTesto(Codice);
		Codice= ConvertiCodText(ExtraTimer);

		ScriveVariabileTesto(Codice, pChar);
		
		break;
	case 239:
		// 239:Variables. Text. Copy the <&>NG String to (E)Text Variable.
		RepeatType=1;
		pChar = GetStringaNG(Timer);
		if (pChar) {
			ScriveVariabileTesto(ConvertiCodText(ExtraTimer), pChar);
		}
		break;
	case 240:
		// 240:Variables. Test. Add in Big Text the <&>Extra NG String with (E)separator
		RepeatType=1;
		pChar = LeggiVariabileTesto(VAR_TYPE_BIG_TEXT);
		strcat(pChar, VetSeparatori[ExtraTimer]);
		strcat(pChar, GetStringaNG(Timer));
		break;
	case 241:
		// 241:Variables. Clear. Clear the <&>Variable group
		RepeatType=1;
		pVar = GlobTomb4.pBaseVariableTRNG;

		switch (Timer) {
		case 0:
			// 0: Clear ALL variables
			memset(pVar, 0, sizeof(StrBaseVarAll));
			break;
		case 1:
			// 1: Clear All Common Numeric Variables
			memset(&pVar->Locals, 0, sizeof(StrBloccoNumVar));
			memset(&pVar->Globals.NumWar, 0, sizeof(StrBloccoNumVar));
			break;
		case 2:
			// 2: Clear Store Variables
			memset(&pVar->Globals.VetStoreLong[0], 0, 16 * 4);
			break;
		case 3:
			// 3: Clear Text Variables
			memset(&pVar->Globals.VetTextVar[0], 0, sizeof(StrText80) * 4);
			pVar->Globals.TextBig[0]=0;
			break;
		case 4:
			// 4: Clear Common Numeric Local Variables
			memset(&pVar->Locals, 0, sizeof(StrBloccoNumVar));
			break;
		case 5:
			// 5: Clear Common Numeric Global Variables
			memset(&pVar->Globals.NumWar, 0, sizeof(StrBloccoNumVar));
			break;
		}
		break;

	case 242:
		// 242:Variables. Text. Add to Big Text the <&>Text Variable with (E)Separator
		RepeatType=1;
		pChar = LeggiVariabileTesto(VAR_TYPE_BIG_TEXT);
		strcat(pChar, VetSeparatori[ExtraTimer]);
		Codice = Timer;
		strcat(pChar, LeggiVariabileTesto(Codice));
		break;

	case 243:
		// 243:Variables. Text. Add to Big Text the <&>Numeric Variable with (E)Separator
		RepeatType=1;
		pChar = LeggiVariabileTesto(VAR_TYPE_BIG_TEXT);
		strcat(pChar, VetSeparatori[ExtraTimer]);
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		// se pero' e' timer usare testo formattato
		if (Codice == 0x33 || Codice == 0x73) {
			// e' timer
			pMex = FormattaTimer(Valore);
		}else {
			sprintf(BufferLog,"%d", Valore);
			pMex = BufferLog;
		}
		strcat(pChar, pMex);
		break;
	case 244:
		// 244:Variables. Memory. Copy to <&>Numeric Variable the (E)Savegame Memory value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabileMem(ExtraTimer | MEM_TIPO_SAVEGAME);
		ScriviVariabile(Codice, Valore);
		break;
	case 245:
		// 245:Variables. Memory. Copy from <&>Numeric Variable to (E)Savegame Memory
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		ScriviVariabileMem(ExtraTimer | MEM_TIPO_SAVEGAME, Valore);
		break;

	case 246:
		// 246:Variables. Memory. Set in <&>Savegame Memory the (E)Value
		RepeatType=1;
		Codice = Timer;
		ScriviVariabileMem(Codice | MEM_TIPO_SAVEGAME, ExtraTimer);
		break;

	case 247:
		// 247:Variables. Memory. Set in <&>Savegame Memory the (E)Bit
		RepeatType=1;

		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		Valore |= (1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 248:
		// 248:Variables. Memory. Clear in <&>Savegame Memory the (E)Bit
		RepeatType=1;

		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		Valore &= ~(1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 249:
		// 249:Variables. Memory. Add to <&>Savegame Memory the (E)Value
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		Valore += ExtraTimer;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 250:
		// 250:Variables. Memory. Subtract to <&>Savegame Memory the (E)Value
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		Valore -= ExtraTimer;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 251:
		// 251:Variables. Numeric. Multiply <&>Numeric Variable by (E)Value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Valore *= ExtraTimer;
		ScriviVariabile(Codice, Valore);
		break;
	case 252:
		// 252:Variables. Numeric. Set in <&>Numeric Variable the (E)Negative Value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = ExtraTimer - 128;
		ScriviVariabile(Codice,Valore);
		break;
	case 253:
		// 253:Variables. Numeric. Divide <&>Numeric Variable by (E)Value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore =LeggiVariabile(Codice);
		Valore /= ExtraTimer;
		ScriviVariabile(Codice,Valore);
		break;
	case 254:
		// 254:Variables. Memory. Set in <&>Savegame Memory the (E)Negative Value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		Valore = ExtraTimer - 128;
		ScriviVariabileMem(Codice, Valore);
		break;

	case 255:
		// 255:Variables. Memory. Set in <&>Selected Item memory the (E)Value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_ITEM;
		Valore = ExtraTimer;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 256:
		// 256:Variables. Memory. Copy to <&> Numeric Variable the (E)Selected Item Memory 
		RepeatType=1;
		Codice = ExtraTimer | MEM_TIPO_ITEM;
		Valore = LeggiVariabileMem(Codice);
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 257:
		// 257:Variables. Memory. Copy from <&>Numeric Variable to (E)Selected Item Memory
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Codice = ExtraTimer | MEM_TIPO_ITEM;
		ScriviVariabileMem(Codice,Valore);
		break;
	case 258:
		// 258:Variables. Memory. Add to <&>Selected Item Memory the (E)Value
		RepeatType=1;

		Codice = Timer | MEM_TIPO_ITEM;
		Valore = LeggiVariabileMem(Codice);
		Valore += ExtraTimer;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 259:
		// 259:Variables. Memory. Set in <&>Selected Item Memory the (E)Bit
		RepeatType=1;

		Codice = Timer | MEM_TIPO_ITEM;
		Valore = (1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 260:
		// 260:Variables. Memory. Clear in <&>Selected Item Memory the (E)Bit
		RepeatType=1;

		Codice = Timer | MEM_TIPO_ITEM;
		Valore = LeggiVariabileMem(Codice);
		Valore &= ~(1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;

	case 261:
		// 261:Variables. Memory. Copy to <&>Selected Item Memory the (E)Big Number value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_ITEM;
		Valore = GlobTomb4.VetBigNumbers[ExtraTimer];
		ScriviVariabileMem(Codice, Valore);
		break;
	case 262:
		// 262:Variables. Memory. Set in <&>Savegame Memory the (E)Big Number value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = GlobTomb4.VetBigNumbers[ExtraTimer];
		ScriviVariabileMem(Codice, Valore);
		break;
	case 263:
		// 263:Variables. Numeric. Set in <&>Numeric Variable the (E)Big Number value
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = GlobTomb4.VetBigNumbers[ExtraTimer];
		ScriviVariabile(Codice, Valore);
		break;
	case 264:
		// 264:Variables. Timer. Start the <&>TRNG Timer in (E)Mode		
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}


		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];

			if (pAzione->ActionType == AZ_TIMER_TRNG &&
				pAzione->Arg2  == Codice) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {
			i = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[i];
		}

		pAzione->ActionType = AZ_TIMER_TRNG;
		pAzione->Arg2 = Codice;
		if (ExtraTimer == 0) {
			pAzione->VetArg[0] = 1;
		}else {
			pAzione->VetArg[0] = -1;
		}
		break;
	case 265:
		// 265:Variables. Timer. Stop the <&>TRNG Timer
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];

			if (pAzione->ActionType == AZ_TIMER_TRNG &&
				pAzione->Arg2  == Codice) {
				pAzione->ActionType =0;
				break;
			}

		}
		break;
	case 266:
		// 266:Variables. Timer. Initialize <&>TRNG Timer to (E)Seconds
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		Valore = ExtraTimer * FRAME_SECONDO;
		ScriviVariabile(Codice, Valore);
		break;
	case 267:
		// 267:Variables. Timer. Initialize <&>TRNG Timer to (E)Big Number seconds
		// 266:Variables. Timer. Initialize <&>TRNG Timer to (E)Seconds
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		Valore = GlobTomb4.VetBigNumbers[ExtraTimer] * FRAME_SECONDO;
		ScriviVariabile(Codice, Valore);
		break;
	case 268:
		// 268:Variables. Timer. Initialize <&>TRNG Timer to (E)Frame Ticks (1/30 of second)
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		Valore = ExtraTimer;
		ScriviVariabile(Codice, Valore);
		break;
	case 269:
		// 269:Variables. Timer. Show <&>TRNG Timer in (E)Position
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		CreaAzionePrintTimer(Codice, ExtraTimer);
		break;

	case 270:
		// 270:Variables. Timer. Hide <&>TRNG Timer in (E)Seconds
		RepeatType=1;
		if (Timer ==0) {
			// local timer
			Codice = 0x0073;
		}else {
			// global timer
			Codice = 0x0033;
		}
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];

			if (pAzione->ActionType == AZ_SHOW_TIMER_TRNG &&
				pAzione->ItemIndex == Codice) {
				if (ExtraTimer==0) {
					// toglierla subito
					pAzione->ActionType =0;
				}else {
					pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
				}
				break;
			}
		}
		break;
	case 271:
		// 271:Variables. Numeric. Copy <&>Numeric Variable to CurrentValue
		RepeatType=1;

		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);

		ScriviVariabile(VAR_TYPE_CURRENT_VALUE,Valore);
		break;
	case 272:
		// 272:Variables. Numeric. Copy CurrentValue to <&>Numeric Variable
		RepeatType=1;
		Valore = LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 273:
		// 273:Variables. Memory. Add to <&>Savegame Memory the CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		
		Valore += LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 274:
		// 274:Variables. Memory. Subtract from <&>Savegame Memory the CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_SAVEGAME;
		Valore = LeggiVariabileMem(Codice);
		
		Valore -= LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice, Valore);
		break;

	case 275:
		// 275:Variables. Memory. Add to <&>Selected Item Memory  the CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_ITEM;
		Valore = LeggiVariabileMem(Codice);
		
		Valore += LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 276:
		// 276:Variables. Memory. Subtract from <&>Savegame Memory the CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_ITEM;
		Valore = LeggiVariabileMem(Codice);
		
		Valore -= LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 277:
		// 277:Variables. Memory. Copy from <&>Code Memory to CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabileMem(Codice);

		ScriviVariabile(VAR_TYPE_CURRENT_VALUE, Valore);
		break;
	case 278:
		// 278:Variables. Memory. Copy from CurrentValue to <&>Code Memory
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 279:
		// 279:Variables. Memory. Set in <&>Code Memory the (E)Value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		ScriviVariabileMem(Codice, ExtraTimer);
		break;
	case 280:
		// 280:Variables. Memory. Add to <&>Code Memory the (E)Value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabileMem(Codice);
		Valore += ExtraTimer;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 281:
		// 281:Variables. Memory. Set in <&>Code Memory the (E)Bit
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabileMem(Codice);
		Valore |= (1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 282:
		// 282:Variables. Memory. Clear in <&>Code Memory the (E)Bit
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabileMem(Codice);
		Valore &= ~(1 << ExtraTimer);
		ScriviVariabileMem(Codice, Valore);
		break;
	case 283:
		// 283:Variables. Memory. Add to <&>Code Memory the CurrentValue
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
		Valore = LeggiVariabileMem(Codice);
		Valore += LeggiVariabile(VAR_TYPE_CURRENT_VALUE);
		ScriviVariabileMem(Codice,Valore);
		break;
	case 284:
		// 284:Variables. Numeric. Invert the sign of <&>Numeric Variable
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Valore = -Valore;
		ScriviVariabile(Codice,Valore);
		break;
	case 285:
		// 285:Variables. Numeric. Add to CurrentValue the <&>Numeric Variable
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue += Valore;
		SHOW_DEBUG("{CurrentValue} = %d", 
				GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue,0);
		break;
	case 286:
		// 286:Variables. Numeric. Subtract from CurrentValue the <&>Numeric Variable
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue -= Valore;
		SHOW_DEBUG("{CurrentValue} = %d", 
				GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue,0);
		break;
	case 287:
		// 287:Variables. Numeric. Divide CurrentValue by <&>Numeric Variable
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		if (Valore) {
			GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue /= Valore;
		}else {
			SHOW_DEBUG("ERROR: division by zero",0,0);
		}
		SHOW_DEBUG("{CurrentValue} = %d", 
				GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue,0);
		break;
	case 288:
		// 288:Variables. Numeric. Multiply CurrentValue by <&>Numeric Variable
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue *= Valore;
		SHOW_DEBUG("{CurrentValue} = %d", 
				GlobTomb4.pBaseVariableTRNG->Globals.CurrentValue,0);

		break;
	case 289:
		// 289:Switch. Perform the <&>Switch script command
		RepeatType=1;
		EseguiSwitch(TimerFull);
		break;
	case 290:
		// 290:Organizer. Resume <&>Organizer in (E)way
		RepeatType=1;
		i = GlobTomb4.pBaseOrganizer->VetID[Timer];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: cann't find Organizer=%d (called from flipeffect 290", Timer);
			InviaLog(BufferLog);
			break;
		}


		pStatusOrg = &GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i];

		switch (ExtraTimer) {
		case 0:
			// 0:Resume from first command (like a new Enabling)
			pStatusOrg->indiceNow =0;
			TestImmediate=false;
			break;
		case 1:
			// 1:Resume from next command immediatly
			TestImmediate=true;
			break;
		case 2:
			// 2:Resume from next command in required time set in next 
			TestImmediate=false;
			break;
		}


		if (TestImmediate) {
			// mettere tempo in modo che sia eseguito subito
			pStatusOrg->StartPerformed=0;
			
		}else {
			// eseguirlo al tempo giusto
			if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_TICK_TIME) {
				pStatusOrg->StartPerformed = 
						GlobTomb4.pBaseOrganizer->CounterGame;
			}else {
				pStatusOrg->StartPerformed = 
						GlobTomb4.pBaseOrganizer->CounterGame / 30;
			}
		}

		pStatusOrg->Status =1;
		break;
	case 291:
		// 291:Variables. Numeric. Copy to<&>Numeric Variable the (E)ColorRGB
		RepeatType=1;

		i = GlobTomb4.BaseColoriRGB.VetID[ExtraTimer];
		if (i == -1) break;

		Valore = GlobTomb4.BaseColoriRGB.VetColori[i].Colore;
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 292:
		// 292:Variables. Memory. Set the <&>Slot as Selected Slot Memory
		RepeatType=1;

		GlobTomb4.CurrentSlotIndex = TimerFull;
		break;
	case 293:
		// 293:Variables. Memory. Copy from (E)Selected Slot Memory to <&>Numeric Variable
		RepeatType=1;
		Codice = ExtraTimer | MEM_TIPO_SLOT;
		Valore = LeggiVariabileMem(Codice);
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 294:
		// 294:Variables. Memory. Copy from <&>Numeric Variable to (E)Selected Slot Memory
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Codice = ExtraTimer | MEM_TIPO_SLOT;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 295:
		// 295:Variables. Memory. Copy from (E)Selected Animation Memory to <&>Numeric Variable
		RepeatType=1;
		Codice = ExtraTimer | MEM_TIPO_ANIMATION;
		Valore = LeggiVariabileMem(Codice);
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 296:
		// 296:Variables. Memory. Copy from <&>Numeric Variable to (E)Selected Animation Memory
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Codice = ExtraTimer | MEM_TIPO_ANIMATION;
		ScriviVariabileMem(Codice, Valore);
		break;
	case 297:
		// 297:Variables. Convert. Convert the <&>NGLE Room index in TOMB Room Index to CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore =GlobTomb4.VetRemapRooms[TimerFull];
		if (Valore==-1) Valore=TimerFull;
		ScriviVariabile(Codice,Valore);
		break;
	case 298:
		// 298:Variables. Convert. Convert the Tomb Room Index to NGLE Room Index in CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);

		for (i=0;i<MAX_ROOMS;i++) {
			if (GlobTomb4.VetRemapRooms[i] == Valore) {
				Valore = i;
				break;
			}
		}

		if (i <MAX_ROOMS) {
			ScriviVariabile(Codice,Valore);
		}
		break;
	case 299:
		// 299:Variables. Convert. Convert the NGLE Room Index to Tomb Room Index in CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);
		Valore = GlobTomb4.VetRemapRooms[Valore];
		ScriviVariabile(Codice, Valore);
		break;
	case 300:
		// 300:Variables. Convert. Convert the Tomb Item Index to NGLE Item Index in CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);
		Valore = GetNgleIndice(Valore);
		ScriviVariabile(Codice,Valore);
		break;
	case 301:
		// 301:Variables. Convert. Convert the NGLE Item Index to Tomb Item Index in CurrentValue
		RepeatType=1;

		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);
		Valore = GlobTomb4.VetRemapObjects[Valore];
		ScriviVariabile(Codice, Valore);
		break;
	case 302:
		// 302:Variables. Save the coordinates and facing of Item index in CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);
		AggiungiItemMosso(Valore);
		break;
		
	case 303:
		// 303:Variables. Numeric. Generate in <&>Numeric Variable the (E)random number
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = Casuale(ExtraTimer+1);
		ScriviVariabile(Codice, Valore);
		break;
	case 304:
		// 304:Variables. Numeric. Generate in <&>Numeric Variable a random CurrentValue
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);
		Valore = Casuale(Valore);
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 305:
		// 305:Variables. Numeric. Perform operation <&>NumericVariable AND (E)Number
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		Valore &= ExtraTimer;
		ScriviVariabile(Codice, Valore);
		break;
	case 306:
		// 306:Inventory-Item. Pop up inventory screen to select the <&>Item
		RepeatType=1;
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		if (have_i_got_object(IndiceSlot) == false) {
			if (ExtraTimer == 1) {
				SoundEffect(GlobTomb4.pBaseCustomize->VetCustSFX[TS_MISSING_REQUIRED_ITEM], 
						&GlobTomb4.pAdr->pLara->CordX, 2);
			}
			break;
		}
		*pRequiredItem = IndiceSlot;
		break;
	case 307:
		// 307:Variables. Memory. Set <&>Animation as Seleted Animation Memory
		RepeatType=1;
		GlobTomb4.CurrentAnimationIndex = TimerFull;
		break;
	case 308:
		// 308:Variables. Log. Print in log file the <&>Extra NG String
		RepeatType=1;
		InviaLog(ReplaceVarPlaceFolders(GetStringaNG(Timer)));
		break;
	case 309:
		// 309:Variables. Log. Print the value of <&>Numeric Variable in (E)Format
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);
		strcpy(BufferLog, GetNomeVariabile(Codice));
		strcat(BufferLog," = ");

		if (ExtraTimer & 4) {
			// binary per prima
			FormattaBinario(Buffer, Valore);
			strcat(BufferLog, " %%");
			strcat(BufferLog,Buffer);

		}

		if (ExtraTimer & 2) {
			sprintf(Buffer," $%X", Valore);
			strcat(BufferLog,Buffer);
		}

		if (ExtraTimer & 1) {
			sprintf(Buffer," %d", Valore);
			strcat(BufferLog, Buffer);
		}
		InviaLog(BufferLog);

		break;
	case 330:
		// 330:Collision. Set the <&>Side of closed sector as non-hangable from Lara
		RepeatType=0;
		GlobTomb4.HangForbidden |= (1 << Timer);
		GlobTomb4.HangCounter =2;

		break;
	case 331:
		// 331:Custom Bar. Show the <&>Custom bar on screen for (E)Seconds
		RepeatType=1;
		// vedere se gia' c'era un comando per questa barra
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_SHOW_CUSTOM_BAR &&
				pAzione->ItemIndex == Timer) break;
		}

		if (i == GlobTomb4.TotProgressiveActions) {
			// non c'era crearla adesso
			i = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[i];
		}

		pAzione->ActionType = AZ_SHOW_CUSTOM_BAR;
		pAzione->ItemIndex = Timer;
		if (ExtraTimer == 0) {
			pAzione->Arg1 = 0xffff;
		}else {
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		}	
		
		break;
	case 332:
		// 332:Custom Bar. Hide the <&>Custom bar
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_SHOW_CUSTOM_BAR &&
				pAzione->ItemIndex == Timer) {

				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_SHOW_CUSTOM_BAR, i);
				break;
			}
		}
		break;
	case 333:
		// 333:Flipmap. Alternate continuosly the <&>flipmap with the (E)Frame interval
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_ALTERNATE_FLIPMAP &&
				pAzione->ItemIndex == Timer) break;
		}

		if (i == GlobTomb4.TotProgressiveActions) {
			// crearlaadesso
			i = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[i];
		}

		pAzione->ActionType = AZ_ALTERNATE_FLIPMAP;
		pAzione->ItemIndex = Timer;
		pAzione->Arg1 = SCRIPT_IGNORE;
		pAzione->Bytes.AlfaByte  = ExtraTimer;
		pAzione->Bytes.BetaByte = ExtraTimer;

		break;
	case 334:
		// 334:Flipmap. Stop the alternate of <&>flimap letting the (E)Flipmap type
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione = &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_ALTERNATE_FLIPMAP &&
				pAzione->ItemIndex == Timer) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) break;
		pAzione->ActionType =0;
		// ora vedere se lo status attuale della flipmap e' quello giusto
		switch (ExtraTimer) {
		case 0:
			// 0:Let the main room
			if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer] == 0) break;
			// bisogna disattivarla
			GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= 0x3e00;
			FlipMap(Timer);
			break;
		case 1:
			// 1:Let the flipped room
			if (GlobTomb4.pAdr->Remap.pVetEnabledFlipMaps[Timer]) break;
			GlobTomb4.pAdr->Remap.pVetButtonFlipMaps[Timer] |= 0x3e00;
			FlipMap(Timer);
			break;
		}
	case 335:
		//335:Variables. Memory. Set the <&>Inventory item like Current Inventory Memory
		RepeatType=1;
		GlobTomb4.CurrentInventoryIndex = Timer;
		break;
	case 336:
		// 336:Variables. Memory. Copy from <&>Numeric Variable to (E)Inventory Memory
		RepeatType=1;
		Codice = ConvertiCodVar(Timer);
		Valore = LeggiVariabile(Codice);

		Codice= ExtraTimer | MEM_TIPO_INVENTORY;
		ScriviVariabileMem(Codice,Valore);
		break;
	case 337:
		// 337:Variables. Memory. Set in <&>Inventory Memory the (E)Value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_INVENTORY;
		ScriviVariabileMem(Codice,ExtraTimer);
		break;
	case 338:
		// 338:Variables. Memory. Set in <&>Inventory Memory the (E)Big Number value
		RepeatType=1;
		Codice = Timer | MEM_TIPO_INVENTORY;
		Valore = GlobTomb4.VetBigNumbers[ExtraTimer];
		ScriviVariabileMem(Codice,Valore);
		break;		
	case 339:
		// 339:Variables. Memory. Copy to <&>Numeric Variable the (E)Inventory Memory
		RepeatType=1;
		Codice = ExtraTimer | MEM_TIPO_INVENTORY;
		Valore = LeggiVariabileMem(Codice);
		
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 340:
		// 340:Lara. (Mesh) Swap Mesh of <&>Extra Lara Slots with that of (E)Slot
		RepeatType=1;
		i = GlobTomb4.pVetMeshSwapSlot[ExtraTimer];
		SwapMeshItem(Timer, i,0);

		// salvare record swap
		SwapMesh.SlotItem=Timer;
		SwapMesh.SlotAltroSlot = i;
		SwapMesh.TipoSwapMesh = 0;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	
	case 341:
		// 341:Swap Mesh. Swap mesh of <&>Slot with mesh of (E)Slot
		RepeatType=1;
		SwapMeshItem(GlobTomb4.pVetSlotMeshMoveables[Timer], 
					GlobTomb4.pVetSlotMeshMoveables[ExtraTimer],0);
		// salvare record swap
		SwapMesh.SlotItem=GlobTomb4.pVetSlotMeshMoveables[Timer];
		SwapMesh.SlotAltroSlot = GlobTomb4.pVetSlotMeshMoveables[ExtraTimer];
		SwapMesh.TipoSwapMesh = 0;
		SalvaRecordSwapMesh(&SwapMesh);

		break;
	case 342:
		// 342:Variables. Memory. Set in <&>Code Memory the (E)Negative number
		RepeatType=1;
		Codice = Timer | MEM_TIPO_CODICE;
	
		Valore = ExtraTimer - 128;
		ScriviVariabileMem(Codice, Valore);
		break;		
	case 343:
		// 343:Fish. Clear (disable) all fishes of <&>fish Type
		RepeatType=1;

		pFish = &GlobTomb4.pAdr->pVetFish[0];

		for (i=0;i<128;i++) {
			if (pFish->TipoUsato == 2) {
				switch (Timer) {
				case 0:
					// 0:ALL FISHES
					pFish->TipoUsato=0;
					break;
				case 1:
					// 1:Predator Fish (all fishes that attack Lara)
					if (pFish->FlagFish  & FISH_ATTACK) pFish->TipoUsato=0;
					break;
				case 2:
					// 2:Good Fish (all fishes that don't attack Lara)
					if ((pFish->FlagFish  & FISH_ATTACK)==0) pFish->TipoUsato=0;
					break;
				case 3:
					// 3:Pirana Fish (Gray fish)
					if ((pFish->FlagFish  & 0x60)==0) pFish->TipoUsato =0;
					break;
				case 4:
					// 4:Clown Fish (White/Red fish)
					if ((pFish->FlagFish  & 0x60)== 0x20) pFish->TipoUsato=0;
					break;
				case 5:
					// 5:Butterfly Fish (Yellow fish)
					if ((pFish->FlagFish  & 0x60)== 0x40) pFish->TipoUsato=0;
					break;
				case 6:
					// 6:Sail Fish (white/azure fish)
					if ((pFish->FlagFish  & 0x60)== 0x60) pFish->TipoUsato=0;
					break;
				}


			}
			pFish++;
		}
		break;
	case 344:
		// 344:Swap Mesh. Swap mesh of current Horizont object with <&>Slot mesh
		RepeatType=1;

		i = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshItem(459, i, 0);

		// salvare record swap
		SwapMesh.SlotItem=459;
		SwapMesh.SlotAltroSlot = i;
		SwapMesh.TipoSwapMesh = 0;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 345:
		// TriggerGroup. Enable newly the one-shot <&>TriggerGroup already performed
		RepeatType=1;
		i = GlobTomb4.pBaseTriggerGroups->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: illegal ID (%d) for TriggerGroup", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		GlobTomb4.pBaseTriggerGroups->VetTriggerGroups[i].
						VetTriggers[0].Flags &= ~TGROUP_DISABLED;
		break;
	case 346:
		// Camera. Enable the <&>StandBy camera effect for (E)seconds
		RepeatType=1;
		// se l'id e' gia' attivonon fare niente
		if (GlobTomb4.BaseStandBy.TestIniziato == true &&
			GlobTomb4.BaseStandBy.IdNow == Timer) break;

		if (GlobTomb4.BaseStandBy.TestIniziato==true) TerminaStandBy(&GlobTomb4.BaseStandBy);
		i = GlobTomb4.BaseStandBy.VetID[Timer];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: missing StandBy= with ID=%d", Timer);
			InviaLog(BufferLog);
			break;
		}

		ImpostaStandBy1(i);
		InizializzaStandBy(&GlobTomb4.BaseStandBy);

		GlobTomb4.BaseStandBy.TestFlipEffect=true;

		GlobTomb4.BaseStandBy.LimiteCicli = ExtraTimer * FRAME_SECONDO;
		break;
	case 347:
		// 347:Enemy. Freeze all enemies for <&>seconds in (E)way
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_FREEZE_ALL) break;
		}

		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_FREEZE_ALL;
		if (Timer == 0) {
			pAzione->Arg1 = SCRIPT_IGNORE;
		}else {
			pAzione->Arg1 = Timer * FRAME_SECONDO;
		}
		pAzione->ItemIndex = ExtraTimer;
		pAzione->Arg2 = *pValOscilla;
		if (pAzione->ItemIndex) {
			// salvare anche tempo di sistema
			// e tempo timer
			pAzione->VetArg[0] = *GlobTomb4.pAdr->pTimerScreen;
			pAzione->VetArg[1] = *pContatoreSchermo;			
		}
		break;
	case 348:
		// 348:Enemy. Remove the "freeze all enemies" mode
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_FREEZE_ALL) { 
				GlobTomb4.VetProgressiveActions[i].Arg1 = 1;
				break;
			}
		}
		break;
	case 349:
	case 350:
		// 349:Lara. (Physics) Trigger clockwise whirl at <&>LARA_START_POS and with (E) Diameter
		// 350:Lara. (Physics) Trigger unclockwise whirl at <&>LARA_START_POS and with (E)Diameter
		RepeatType=0;

		pLaraPos = TrovaRecordLaraPos(Timer,-1);

		if (pLaraPos == NULL) {
			RepeatType=2;


			break;
		}
		if (FlipNumber== 349) {
			TestClockWise=true;
		}else {
			TestClockWise=false;
		}
		EseguiGorgo(pLaraPos,	ExtraTimer << 8, TestClockWise);
		break;
	case 351:
		// 351:Variables. Numeric. Convert the item memory address in Current Value t  item index
		RepeatType=1;
		Codice = VAR_TYPE_CURRENT_VALUE;
		Valore = LeggiVariabile(Codice);


		if (Valore) {
			Valore = GetIndiceItem(Valore);
		}

		ScriviVariabile(Codice, Valore);
		break;
	case 352:
		// 352:Statics. Scale a static item using the data in <&>Parameters command
		RepeatType=1;
		i = GlobTomb4.BaseScaleParam.VetIdScale[TimerFull];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: missing PARAM_SCALE_ITEM with Id = %d", 
				TimerFull);
			InviaLog(BufferLog);
			break;
		}
		pScale = &GlobTomb4.BaseScaleParam.VetScaleItem[i];

		// se e' immediato eseguirlo subito
		if (pScale->Flags & FSCA_IMMEDIATE) {
			AggiornaStatici(pScale, pScale->FinalPerc);
			DeleteParamCommand(PARAM_SCALE_ITEM, TimerFull,false,true);
			break;

		}
		// e' dinamico: creare un azione progressiva
		// a meno che non sia gia' presente
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SCALE_STATIC &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == TimerFull) {

				sprintf(BufferLog, "Flipeffect 352: progressive action to scale static item %d was already in progress",
					pScale->ItemIndex);
				InviaLog(BufferLog);
				break;
			}
		}
		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_SCALE_STATIC;
		pAzione->ItemIndex = TimerFull; // id scale item

		pAzione->Arg1 = SCRIPT_IGNORE;
		pAzione->Arg2 = pScale->Flags;
		pAzione->VetArgFloat[0] = (float) pScale->BeginPerc;
		pAzione->VetArgFloat[1] = (float) pScale->IncPerc;
		pAzione->VetArgFloat[1] /= 10;

		// vedere maggiore e minore
		if (pScale->BeginPerc < pScale->FinalPerc) {
			// 
			pAzione->VetArgFloat[2] = (float) pScale->BeginPerc;
			pAzione->VetArgFloat[3] = (float) pScale->FinalPerc;
		}else {
			pAzione->VetArgFloat[2] = (float) pScale->FinalPerc;
			pAzione->VetArgFloat[3] = (float) pScale->BeginPerc;
			if (pAzione->VetArgFloat[1] > 0) pAzione->VetArgFloat[1] = -pAzione->VetArgFloat[1];

	
		}
		break;
	case 353:
		// 353:Statics. Stop the endless scaling of the static started with the <&>Parameters command
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione=&GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_SCALE_STATIC &&
				pAzione->ItemIndex == TimerFull) {
				pAzione->ActionType =0;
				GestioneCompletedProgActions(AZ_SCALE_STATIC, i);
				break;
			}
		}
		break;
	case 354:
		// 354:ItemGroup. Statics. Perform <&>trigger on (E)ItemGroup of statics
		RepeatType=1;
		i = GlobTomb4.BaseItemGroup.VetID[ExtraTimer];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: the index [%d] points to a missing ItemGroup= command",
				ExtraTimer);
			InviaLog(BufferLog);
			break;
		}

		TotIndici = GlobTomb4.BaseItemGroup.VetItemGroup[i].TotIndici;
		pVetIndici = (WORD *) &GlobTomb4.BaseItemGroup.VetItemGroup[i].VetIndici[0];

		for (i=0;i<TotIndici;i++) {
			EsecuzioneFlipeffect(0, Timer, pVetIndici[i], Flags);

		}
		break;
	case 355:
		// 355:Screen. Flash screen with <&>Light for (E)Durate
		RepeatType=1;

		// colore singolo
		*GlobTomb4.pAdr->pFlashRed  = VetFlashRed[Timer];
		*GlobTomb4.pAdr->pFlashGreen  = VetFlashGreen[Timer];
		*GlobTomb4.pAdr->pFlashBlue = VetFlashBlue[Timer];
		*GlobTomb4.pAdr->pFlashDurate = ExtraTimer;
		break;
	case 356:
		// 356:Screen. Remove Infinite Flash effect
		RepeatType=1;
		*pFlashDurata=0;
		break;
	case 357:
		// 357:Sprite. Show sprite with data in <&>Parameters for (E)Durate
		RepeatType=1;
		// localizzare il record show sprite
		i = GlobTomb4.BaseShowSprites.VetIdShowSprites[Timer];
		if (i < 0) {
			sprintf(BufferLog,"ERROR: cann't find Parameters=PARAM_SHOW_SPRITE with id=%d",
				Timer);
			InviaLog(BufferLog);
			break;
		}
		pSprite = &GlobTomb4.BaseShowSprites.VetShowSprites[i];

		// creare nuova azione
		// e' dinamico: creare un azione progressiva
		// a meno che non sia gia' presente
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SHOW_SPRITE &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) {

				GlobTomb4.VetProgressiveActions[i].ActionType = 0;
				GestioneCompletedProgActions(AZ_SHOW_SPRITE, i);

				break;
			}
		}
		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_SHOW_SPRITE;
		pAzione->ItemIndex = Timer; // id di paramrter
		// mettere in arg1 il tempo di visualizzazione in posizione finale
		if (ExtraTimer == 0) {
			pAzione->Arg1 = SCRIPT_IGNORE;
		}else {
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		}
		pAzione->Arg2 = 0;

		// se c'e' un effetto mettere in arg2 il numero di frame necessari
		if (pSprite->Flags & FSS_EFFECT_ZOOM) {
			pAzione->Arg2 = pSprite->ExtraValue;
			CalcolaSpriteZoom(pAzione, pSprite, true);
		}
		if (pSprite->Flags & (FSS_EFFECT_FROM_BOTTOM | FSS_EFFECT_FROM_TOP |
			FSS_EFFECT_FROM_LEFT | FSS_EFFECT_FROM_RIGHT)) {
			// c'e' un effetto di movimento
			// preparare i dati
			PreparaSpriteMovimento(pAzione, pSprite);
		}
		if (pSprite->Flags & FSS_ANIMATE) {
			// salva indice da mostrare
			pAzione->VetArgShort[11] = pSprite->IndiceSprite;
			// e numero di frame di durata
			pAzione->VetArgShort[10] = (pSprite->ExtraValue & 0xff00) >> 8;
			// incrfemento inziiale
			pAzione->VetArgSignedBytes[18] = 1;  // era short[9]
			pAzione->VetArgBytes[19] = 255;  // se diverso da 255 allora e' il frame su cui fermarsi


		}

		break;
	case 358:
		// 358:Sprite. Remove from the screen the sprite with data in <&>Parameters
		RepeatType=1;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SHOW_SPRITE &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == TimerFull) {

				GlobTomb4.VetProgressiveActions[i].ActionType = 0;
				GestioneCompletedProgActions(AZ_SHOW_SPRITE, i);
				break;
			}
		}
		break;
	case 359:
		// 359:Weather. Perform a lightning with data in <&>Parameters for (E)Durate
		RepeatType=1;
		// esegue comando:Parameters=PARAM_LIGHTNING, IdParamList, Lightning flags (LGTN_...), 
		//           SourcePosItem, TargetPosItem, IdColorRGB, Durate, Intensity, SoundEffect, 
		//			Size, ParticleDurate, Alfa, Beta

		// creare azione progressiva AZ_LIGHTNING		
		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		// trovare la struttura PARAM_LIGHTNING associata
		i=GlobTomb4.BaseParamLightning.VetId[Timer];
		if (i == -1) {
			sprintf(BufferLog,"ERROR: not found the Parameteres=PARAM_LIGHTNING with ID = %d", Timer);
			InviaLog(BufferLog);
			break;
		}
		pLight = &GlobTomb4.BaseParamLightning.VetRecordLgtn[i];


		
		pAzione->ActionType = AZ_LIGHTNING;
		// set durate time
		pAzione->Arg1 = ExtraTimer;   
		if (pAzione->Arg1 == 0) {
			pAzione->Arg1 = SCRIPT_IGNORE;
		}else {
			pAzione->Arg1 = ExtraTimer;
		}
		pAzione->ItemIndex  = Timer;  // id param_lightning
		pAzione->Bytes.AlfaByte  =0 ; // mettere "1" se e' lara
		pAzione->Bytes.BetaByte = 1;  // prima esecuzione, poi deve essere azzerato
		// trovare coordinate destinazione
		Indice = pLight->IndexTarget;
		// scoprire le sue coordinate
		if (Indice == -1) {
			// target casuale: analisi complessa per trovare un punto casuale lontano da lara.
			pPunto = TrovaTargetCasuale();
			if (pPunto==NULL) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_LIGHTNING, IndiceAzione);
				break;
			
			}
			pAzione->VetArgDword[0] = pPunto->OrgX;
			pAzione->VetArgDword[1] = pPunto->OrgY;
			pAzione->VetArgDword[2] = pPunto->OrgZ;

		}else {
			
			if (CalcolaCordItem(Indice, &pAzione->VetArgDword[0], &pAzione->Bytes.AlfaByte )==false) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_LIGHTNING, IndiceAzione);
				break;
			}

		}

		// calcolare posizione sorgente
		Indice = pLight->IndexSource;

		if (Indice == -1) {
			if (pLight->Flags & LGTN_INCLINED_RANDOM) {
				pAzione->VetArgDword[3] = pAzione->VetArgDword[0] + 4096- Casuale(8192);
				pAzione->VetArgDword[4] = pAzione->VetArgDword[1] - 1024*10;
				pAzione->VetArgDword[5] = pAzione->VetArgDword[2] + 4096- Casuale(8192);
			}else {

				// usare sky sopra target
				pAzione->VetArgDword[3] = pAzione->VetArgDword[0] + (256-Casuale(512));
				pAzione->VetArgDword[4] = pAzione->VetArgDword[1] - 1024*10;
				pAzione->VetArgDword[5] = pAzione->VetArgDword[2] + (256-Casuale(512));
			}
		}else {
			if (CalcolaCordItem(Indice, &pAzione->VetArgDword[3], &SalvaMicro)==false) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_LIGHTNING, IndiceAzione);
				break;
			}

	
		}


		if (pLight->Flags & LGTN_HEARTHQUAKE) {
			// vedere se lara e' abbastanza vicina al fulmine
			Distanza =6;
			if (pLight->IndexTarget == -1) Distanza = 11;
			if (DistanzaLaraDaXYZ(pAzione->VetArgDword[0], pAzione->VetArg[1], pAzione->VetArgDword[2]) <= (1024 * Distanza)) {
				// earthquake
				Service(SRV_PERFORM_FLIPEFFECT, 0, 1, 0);
			}
		}
		if (pLight->Flags & LGTN_FIRE_LARA) {
			if (pLight->Flags & LGTN_KILL_TARGET) {
				i= 1;
				if (pLight->Flags & LGTN_LARA_SCREAM) SoundEffect(31, NULL, 0);
			}else {
				// urlo lungo (e' a fuoco e scappa)
				if (pLight->Flags & LGTN_LARA_SCREAM) SoundEffect(30, NULL, 0);
				i= 2;
			}
			// infuoca lara
			Service(SRV_PERFORM_FLIPEFFECT, 0, 63, i);
		}

		

	

		
		break;
	case 361:
		// 361:Weather. Set in advance the rain setting using the intensity of <&>Room
		// in timer numero stanza
		RepeatType=1;
		SalvaRoom = *pCamera_PosNow_Room;
		*pCamera_PosNow_Room = Timer;
		CambioIntensitaWeather();
		*pCamera_PosNow_Room = SalvaRoom;
		break;
	case 362:
		// 362: "Weather. Stop the endless lightning effect with <&>Parameters"
		RepeatType=1;
		pAzione= &GlobTomb4.VetProgressiveActions[0];
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (pAzione->ActionType == AZ_LIGHTNING && 
				pAzione->ItemIndex == TimerFull) {
				// trovato
				pAzione->Arg1=1;
				break;
			}
			pAzione++;
		}
		break;
	case 363:
		// 363:Text. Print <&>extraNg string with windows font and formatting data in the (E)Parameters command 
		RepeatType=1;


		// verificare che sia presente sia extra ng string che il comando parameter
		pChar = GetString(Timer | STRING_NG);
		if (pChar==NULL) break;

		i = GlobTomb4.BaseParamWText.VetIds[ExtraTimer];
		if (i==-1) {
			sprintf(BufferLog, "ERROR: cann't find the Parameters=PARAM_WTEXT with id=%d", ExtraTimer);
			InviaLog(BufferLog);
			break;
		}
		pWText = &GlobTomb4.BaseParamWText.VetParamWText[i];


		Indice = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[Indice];

		pAzione->ActionType = AZ_PRINT_WINDOWS_TEXT;
		pAzione->Arg1= pWText->Durate;
		pAzione->Arg2 = Timer;

		pAzione->ItemIndex = ExtraTimer;
		InitWTextEffect(pAzione, pWText);

		break;
	case 364:
		// 364:Text. Remove print ExtraNG<&>string with windows font that used data in the (E)Parameters command
		RepeatType=1;
		pAzione = &GlobTomb4.VetProgressiveActions[0];
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {

			if (pAzione->ActionType  == AZ_PRINT_WINDOWS_TEXT && 
				pAzione->Arg2 == Timer &&
				pAzione->ItemIndex == ExtraTimer) {
				
				pAzione->Arg1=1;
				break;
			}
			pAzione++;
		}
		break;
	case 365:
		// 365:Camera. Increase the zoom factor (enable Super Zoom mode) for Binocular
		RepeatType=1;
		GlobTomb4.StatusNG |= SNG_SUPER_BINOCULARS;
		AggiornaBinocoloSuper();
		break;
	
	case 366:
		// 366:Variables. Boat. Move <&>fuel to boat tank in (E)way
		RepeatType=1;
		switch (ExtraTimer) {
		case 0:
			// 0: Add to current fuel
			Valore=LeggiVariabile(0x72);
			Valore += Timer * 1800;
			ScriviVariabile(0x72, Valore);
			break;
		case 1:
			// 1: Set as new fuel, ignoring previous level of fuel
			Valore = Timer * 1800;
			ScriviVariabile(0x72, Valore);
			break;
		}
		break;
	case 367:
		// 367:Camera. Show blank screen for <&>seconds with final curtain effect
		RepeatType=1;

		// creare azione per gestire questo effetto se rieschiede tempo infinito
		if (Timer) {
			*GlobTomb4.pAdr->pFadeCurtain = (Timer+5) * FRAME_SECONDO;
			break;
		}

		// qui bisogna fare azione che mantiene sempre il valore prefissato
		IndiceAzione= CreaNuovaAzioneProgressiva();
		pAzione= &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_CURTAIN_FADE_EFFECT;
		pAzione->Arg1 = SCRIPT_IGNORE;
		pAzione->ItemIndex = 30*5;


		break;
	case 368:
		// 368:Camera. Fade. Stop black screen and open curtain 
		RepeatType=1;

		pAzione= &GlobTomb4.VetProgressiveActions[0];
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (pAzione->ActionType == AZ_CURTAIN_FADE_EFFECT &&
				pAzione->ItemIndex == 30*5) {
				pAzione->ActionType=0;
				GestioneCompletedProgActions(AZ_CURTAIN_FADE_EFFECT, i);
			}
			pAzione++;
		}
		break;
	case 369:
		// Camera. Set Cinema screen with black rows of <&>depth for (E)seconds
		RepeatType=1;

		if (Timer==0) {
			// rimuovere azione
			pAzione= &GlobTomb4.VetProgressiveActions[0];
			for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
				if (pAzione->ActionType == AZ_CURTAIN_FADE_EFFECT &&
					pAzione->ItemIndex != 30*5) {
					pAzione->ActionType=0;
					GestioneCompletedProgActions(AZ_CURTAIN_FADE_EFFECT, i);
				}
				pAzione++;
			}
			break;
		}

		switch (Timer) {
		case 1:
			// 1: Tiny
			n= 20;
			break;
		case 2:
			// 2: Middle (cinema effect di cutscene)
			n= 30;
			break;
		case 3:
			// 3: Big
			n= 40;
			break;
		case 4:
			// 4: huge
			n= 60;
			break;
		case 5:
			// 5: fissure
			n= 90;
			break;
		default:
			n= 10;
			break;
		}
		// qui bisogna fare azione che mantiene sempre il valore prefissato
		IndiceAzione= CreaNuovaAzioneProgressiva();
		pAzione= &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_CURTAIN_FADE_EFFECT;
		if (ExtraTimer==0) {
			pAzione->Arg1 = SCRIPT_IGNORE;
		}else {
			pAzione->Arg1 = ExtraTimer * FRAME_SECONDO;
		}
		pAzione->ItemIndex = n;

		break;
	case 370:
		// AnimCommand. If current item is wading water, add to him twirls (ripples) with <&>Intensity
		RepeatType=0;
		Indice = GlobTomb4.ItemIndexCurrent;
		if (Indice == -1) {
			InviaLog("ERROR: undefined the owner item index for flipeffect 370");
			Indice = *GlobTomb4.pAdr->pLaraIndex;
		}
		pItem= &GlobTomb4.pAdr->pVetItems[Indice];

		AddRippleToItem(pItem, Timer, ExtraTimer);
		break;
	case 371:
		// TriggerGroup. Perform <&>TriggerGroup from script.dat (single execution)
	case 372:
		// 372:TriggerGroup. Perform <&>TriggerGroup from script.dat (always performed until lara stands on current sector)
	case 373:
		// 373:TriggerGroup. Perform <&>TriggerGroup from script.dat (continue performing to stop with f192 trigger)
		i = GlobTomb4.BaseTG_SingleShotResumed.TGEseguiti;
		GlobTomb4.BaseTG_SingleShotResumed.VetTGEseguiti[i] = TimerFull;
		GlobTomb4.BaseTG_SingleShotResumed.TGEseguiti++;

		switch (FlipNumber) {
		case 371:
			RepeatType=1;
			GlobTomb4.pMexLastTGCaller = "Flipeffect 371";
			EseguiTriggerGroup(TimerFull);
			break;
		case 372:
			RepeatType=0;
			GlobTomb4.pMexLastTGCaller = "Flipeffect 372";
			EseguiTriggerGroup(TimerFull);
			break;
		case 373:
			RepeatType=1;
			CreaTriggerGroupContinuo(TimerFull);
		}

		break;

	case 374:
		// "GlobalTriggers. Enable GlobalTrigger with ID<&>"
		RepeatType=1;
		i=GlobTomb4.pBaseGlobalTriggers->VetID[TimerFull];



		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found GlobalTrigger with ID=%d called from flipeffect 374",
				TimerFull);
			InviaLog(BufferLog);
			break;
		}
		
		// abilitare globaltrigger
		GlobTomb4.pBaseGlobalTriggers->VetTriggers[i].Flags &= ~FGT_DISABLED;
		break;
	case 375:
		// "GlobalTriggers. Disable GlobalTrigger with ID<&>"
		RepeatType=1;
		i=GlobTomb4.pBaseGlobalTriggers->VetID[TimerFull];



		if (i == -1) {
			sprintf(BufferLog, "ERROR: not found GlobalTrigger with ID=%d called from flipeffect 375",
				TimerFull);
			InviaLog(BufferLog);
			break;
		}
		
		// disattivare globaltrigger
		GlobTomb4.pBaseGlobalTriggers->VetTriggers[i].Flags |= FGT_DISABLED;
		break;
	case 376:
		// 376:Organizer. Resume <&>Organizer from first command (reset)

		RepeatType=1;
		i = GlobTomb4.pBaseOrganizer->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: cann't find Organizer=%d (called from flipeffect 376", TimerFull);
			InviaLog(BufferLog);
			break;
		}		

		pStatusOrg = &GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i];
		pStatusOrg->indiceNow =0;

		// eseguirlo al tempo giusto
		if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_TICK_TIME) {
			pStatusOrg->StartPerformed = 
					GlobTomb4.pBaseOrganizer->CounterGame;
		}else {
			pStatusOrg->StartPerformed = 
					GlobTomb4.pBaseOrganizer->CounterGame / 30;
		}
		pStatusOrg->Status =1;
		break;
	case 377:
		// 377:Organizer. Resume <&>Organizer from next command (immediatly)
		RepeatType=1;
		i = GlobTomb4.pBaseOrganizer->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: cann't find Organizer=%d (called from flipeffect 377", TimerFull);
			InviaLog(BufferLog);
			break;
		}		

		pStatusOrg = &GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i];
		pStatusOrg->StartPerformed=0;
		pStatusOrg->Status =1;
		break;
	case 378:
		// 378:Organizer. Resume <&>Organizer from next command in the given time set in next command
		RepeatType=1;
		i = GlobTomb4.pBaseOrganizer->VetID[TimerFull];
		if (i == -1) {
			sprintf(BufferLog, "ERROR: cann't find Organizer=%d (called from flipeffect 378", TimerFull);
			InviaLog(BufferLog);
			break;
		}		

		pStatusOrg = &GlobTomb4.pBaseOrganizer->VetStatusOrganizer[i];
		if (GlobTomb4.pBaseOrganizer->VetOrganizer[i].Flags & FO_TICK_TIME) {
			pStatusOrg->StartPerformed = 
					GlobTomb4.pBaseOrganizer->CounterGame;
		}else {
			pStatusOrg->StartPerformed = 
					GlobTomb4.pBaseOrganizer->CounterGame / 30;
		}
		pStatusOrg->Status =1;
		break;
	case 379:
		// 379:Cutscene. Perform the demo.pak at <&>index of Demo script command
		RepeatType=1;

		if (GlobTomb4.pDemoLevel->TotDemoIDs==0) {
			InviaLog("ERROR: no Demo script command found in current level");
			break;
		}
		
		i = TimerFull-1;
		if (i < 0 || i >= GlobTomb4.pDemoLevel->TotDemoIDs) {
			sprintf(BufferLog, "ERROR: in F379 trigger the supplied index (%d) is outside of valid range 1 / %d",
				TimerFull, GlobTomb4.pDemoLevel->TotDemoIDs);
			InviaLog(BufferLog);
			break;
		}

		n= GlobTomb4.pDemoLevel->VetDemoIDs[i];
		GlobTomb4.TestEditingDemo=false;
		if (CaricaDemo(n)==false) break;
		GlobTomb4.DemoNumberLoaded= n;
		GlobTomb4.pDemoLevel->TestDemoInProgress=true;
		GlobTomb4.pBaseDemo->Status = RECF_PLAYING;
		break;
	case 380:
		// 380:Cutscene. Stop the current demo in progress
		RepeatType=1;
		if (GlobTomb4.pBaseDemo->Status != RECF_PLAYING) {
			InviaLog("Warning: no demo in progress to stop (F380)");
			break;
		}
		// vedere che tipo di demo e'
		if (GlobTomb4.pDemoTitle->TestDemoInProgress==true) {
			
			pDemo = GlobTomb4.pDemoTitle;
			FlagGD = FDG_FROM_TITLE;

		}else {
			pDemo = GlobTomb4.pDemoLevel;
			FlagGD= FDG_NONE;
		}

		if (pDemo->TestDemoInProgress == false) {
			InviaLog("Warning: no level demo is in progress currently, only a testing demo");
			GlobTomb4.pBaseDemo->Status = RECF_STOP;
			break;
		}

		GestioneDemoInProgress(FlagGD | FDG_END | FDG_END_FOR_TRIGGER , pDemo);
		break;
	case 381:
		// 381:Cutscene. Camera. Look Leading Actor from <&>View angle and (E)Distance
	case 382:
		// 382:Cutscene. Camera. Look Extra Actor from <&>View angle and (E)Distance
	case 383:
		// 383:Cutscene. Camera. Look Lara from <&>View angle and (E)Distance
		RepeatType=1;
		switch (FlipNumber) {
		case 381:
			i = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (i == -1) {
				InviaLog("ERROR: not set Leading Actor, cann't find target for cutscene camera");
				break;
			}
			break;
		case 382:
			i = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Extra Actor, cann't find target for cutscene camera");
				break;
			}				
			break;
		case 383:
			i = *GlobTomb4.pAdr->pLaraIndex;
			break;
		}
		if (i==-1) break;

		AttivaLookCutsceneCamera(i, Timer, ExtraTimer, FTC_NONE);
		break;
	case 384:
		// 384:Cutscene. Camera. Reset cutscene camera and come back to lara's chase camera
		RepeatType=1;

		LiberaCutsceneCamera();
		break;
	case 385:
		// 385:Cutscene. Camera. Move up cutscene camera of <&>Clicks in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_MOVE_UP, Timer, ExtraTimer);
		break;
	case 386:
		// 386:Cutscene. Camera. Move down cutscene camera of <&>Clicks in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_MOVE_DOWN, Timer, ExtraTimer);
		break;
	
	case 387:
		// 387:Cutscene. Camera. Zoom-in cutscene camera covering <&>Distance in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_ZOOM_IN, Timer, ExtraTimer);
		break;

	case 388:
		// 388:Cutscene. Camera. Zoom-out cutscene camera covering <&>Distance in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_ZOOM_OUT, Timer, ExtraTimer);
		break;
	case 389:
		// 389:Cutscene. Camera. Rotate at right cutscene camera around target by <&>Degrees in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_ROTATE_RIGHT, Timer, ExtraTimer);
		break;
	case 390:
		// 390:Cutscene. Camera. Rotate at left cutscene camera around target by <&>Degrees in (E)demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_ROTATE_LEFT, Timer, ExtraTimer);
		break;
	case 391:
		// 391:Cutscene. Camera. Freeze cutscene camera for <&>demo frames
		RepeatType=1;
		CommandForCutsceneCamera(CCT_FREEZE, 0, Timer);
		break;
	case 392:
		// 392:Cutscene. Camera. Remove freeze from cutscene camera
		RepeatType=1;
		GlobTomb4.pBaseCutscene->BaseCamera.TestFreeze=false;
		GlobTomb4.pBaseCutscene->BaseCamera.FreezeEndFrame=0;
		break;
	case 393:
		// 393:Cutscene. Animations. Swap animations set in data of <&>Parameter command for (E)Actor
		RepeatType=1;

		switch (ExtraTimer) {
		case 0:
			// 0:Lara
			i = *GlobTomb4.pAdr->pLaraIndex;
			break;
		case 1:
			// 1:Leading Actor
			i = GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (i == -1) {
				InviaLog("ERROR: not set Leading Actor, cann't find slot for animation swapping");
				break;
			}
			break;
		case 2:
			// 2=2:Extra Actor
			i = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Extra Actor, cann't find target for animation swapping");
				break;
			}				
			break;

		}

		pItem= &GlobTomb4.pAdr->pVetItems[i];

		// salvare swapping in modo da poter poi ripristinsre le cose o salvare i cmabiamenti
		// in savegame
		SalvaSwapAnim(pItem->SlotID, Timer);

		SwapAnimations(pItem->SlotID, Timer);
		break;
	case 394:
		// 394:Cutscene. Free cutscene <&>resources (to call at end of cutscene)
		RepeatType=1;
		// togliere status di actor a tutti gli attori e anche basic collision
		for (i=0;i<GlobTomb4.pBaseCutscene->BaseSaveActors.TotActors;i++) {
			Indice=GlobTomb4.pBaseCutscene->BaseSaveActors.VetActors[i].ItemIndex;
			pItem = &GlobTomb4.pAdr->pVetItems[Indice];

			RestoreActor(pItem, Indice);
		}

		GlobTomb4.pBaseCutscene->BaseSaveActors.TotActors=0;

		LiberaCutsceneCamera();

		// ora vedere se ripristinare anche swap animazioni
		if (Timer==0) {
			RestoreAllAnimSwap(true);
		}
		break;

	case 395:
		// 395:Cutscene. Camera. Actor with <&>role will look Lara (enemy subjective view, enemy not visible)
		RepeatType=1;
		switch (Timer) {
		case 0:
			// 0:Leading Actor
			i= GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Leading Actor role");
				break;
			}
			FlagsFCT= FTC_LEADING_LOOK_LARA;
			break;
		case 1:
			// 1:Extra Actor
			i = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Extra Actor role");
				break;
			}
			FlagsFCT= FTC_EXTRA_LOOK_LARA;
			break;
		}
		if (i==-1) break;

		AttivaLookCutsceneCamera(*GlobTomb4.pAdr->pLaraIndex , 0, 0, FlagsFCT);
		break;
	case 396:
		// 396:Cutscene. Camera. Lara will look Actor with <&>Role (lara subjective view, lara not visible)
		RepeatType=1;
		switch (Timer) {
		case 0:
			// 0:Leading Actor
			i= GlobTomb4.pBaseCutscene->LeadingActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Leading Actor role");
				break;
			}
			FlagsFCT= FTC_LARA_LOOK_LEADING;
			break;
		case 1:
			// 1:Extra Actor
			i = GlobTomb4.pBaseCutscene->ExtraActorIndex;
			if (i==-1) {
				InviaLog("ERROR: not set Extra Actor role");
				break;
			}
			FlagsFCT= FTC_LARA_LOOK_EXTRA;
			break;
		}
		if (i==-1) break;

		AttivaLookCutsceneCamera(i , 0, 0, FlagsFCT);
		break;
	case 397:
		// 397:Cutscene. Swap VonCroy <&>mesh with same mesh of (E)slot
		RepeatType=1;
		// trovare slot in extratimer
		if (ExtraTimer < 3) {
			// un mesh swap
			ExtraTimer+= 417; // MESHSWAP1
		}else {
			// e' un animating
			ExtraTimer+= 427 - 3; // ANIMATING1
		}
		// 39 = VON_CROY
		SwapSingleMeshItem(39, ExtraTimer, Timer);
		break;
	case 398:
		// 398:Cutscene. AI. Set Von Croy<&>Data with (E)Value 
		RepeatType=1;

		switch (Timer) {
		case 0:
		case 1:
		case 2:
			VetVonCroyData[Timer]=ExtraTimer;
			break;
		case 3:
			// 3:Von Croy Mode (0=Angkor Wat / 1=Iris Race)
			if (ExtraTimer==0) {
				GlobTomb4.pAdr->pVetSlot[39].pProcControl = (void *) 0x419490; // Sub_AttivaEMuovi_39_VON_CROY
			}else {
				GlobTomb4.pAdr->pVetSlot[39].pProcControl = (void *) 0x41ADA0; // VoncroyRaceControl
			}
			break;
		case 4:
			// knife in right hand
			// 4:Knife in right hand (0=No / 1=Knife)

			i= GlobTomb4.pAdr->pVetSlot[39].IndexFirstMesh + 18*2;

			if (ExtraTimer==0) {
				pMeshObj = GlobTomb4.pAdr->pVonCroyMeshFreeHand;
			}else {
				pMeshObj = GlobTomb4.pAdr->pVonCroyMeskKnifeHand;
			}
			
			GlobTomb4.pAdr->VetMeshPointer[i+1] = pMeshObj;

			break;
		}
		break;
	case 399:
		// 399:Cutscene. Lara's texts. Set <&>Color and (E)position for lara's texts
		RepeatType=1;
		GlobTomb4.pBaseCutscene->TextLara.Colore = Timer;
		GlobTomb4.pBaseCutscene->TextLara.Posizione = ExtraTimer;
		break;
	case 400:
		// 400.Cutscene. Leading Actor's texts. Set <&>Color and (E)position for leading actor's texts
		RepeatType=1;
		GlobTomb4.pBaseCutscene->TextLeading.Colore = Timer;
		GlobTomb4.pBaseCutscene->TextLeading.Posizione = ExtraTimer;
		break;
	case 401:
		// 401.Cutscene. Extra Actor's texts. Set <&>Color and (E)position for extra actor's texts
		RepeatType=1;
		GlobTomb4.pBaseCutscene->TextExtra.Colore = Timer;
		GlobTomb4.pBaseCutscene->TextExtra.Posizione= ExtraTimer;
		break;

	case 402:
		// 402:Sprite. Stop the animated sprites with data in <&>Parameter at (E)frame
		RepeatType=1;
		// localizzare azione di questo sprite
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SHOW_SPRITE &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) {
				GlobTomb4.VetProgressiveActions[i].VetArgBytes[19]= ExtraTimer;
				break;
			}
		}
		if (i==GlobTomb4.TotProgressiveActions) {
			sprintf(BufferLog, "ERROR: not found showing sprite with data in Parameters=PARAM_SHOW_SPRITE,%d", Timer);
			InviaLog(BufferLog);
		}

		break;
	case 403:
		// 403:Sprite. Resume animation of sprites with data in <&>Parameter
		RepeatType=1;
		// localizzare azione di questo sprite
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SHOW_SPRITE &&
				GlobTomb4.VetProgressiveActions[i].ItemIndex == Timer) {
				GlobTomb4.VetProgressiveActions[i].VetArgBytes[19]= 255;
				break;
			}
		}
		if (i==GlobTomb4.TotProgressiveActions) {
			sprintf(BufferLog, "ERROR: not found showing sprite with data in Parameters=PARAM_SHOW_SPRITE,%d", Timer);
			InviaLog(BufferLog);
		}

		break;
	case 404:
		// 404:Secret. Trigger <&>secret
		RepeatType=1;

		Codice= 1 << Timer;

		if (Codice & *pMaskSegreti) break;

		PlayExtraCD(5,1, 0);

		(*pMaskSegreti) |= Codice;
		(*pTotSegreti)++;
		break;
	case 405:
		// 405:FMV. Play <&>fmv
		RepeatType=1;

		ElaboraTriggerFMV(Timer);
		break;
	case 406:
		// 406:Camera. Get/Remove<&> infinite durate for current camera (not flyby)
		RepeatType=1;

		// find progressive action for infinite durate
		pAzione = &GlobTomb4.VetProgressiveActions[0];

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (pAzione->ActionType == AZ_CAMERA_INFINITE) break;
			pAzione++;
		}

		if (Timer == 0) {
			// remove progressive action
			if (i == GlobTomb4.TotProgressiveActions) {
				// non trovata
				InviaLog("ERROR: cann't find an enabled, current infinite camera");
				break;
			}

			pAzione->ActionType=0;
			*GlobTomb4.pAdr->Camera.pSecondsTimerCamera = 1;
			break;
		}

		// enable now but only if there is an eabled camera
		if (*GlobTomb4.pAdr->Camera.pSecondsTimerCamera==-1) {
			InviaLog("ERROR: cann't find any enabled camera, currently");
			break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {
			i=CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[i];
		}

		pAzione->ActionType = AZ_CAMERA_INFINITE;
		pAzione->Arg1= SCRIPT_IGNORE;
		pAzione->ItemIndex = *GlobTomb4.pAdr->Camera.pIndexCameraNext;
		pAzione->Arg2 = GetIndiceItem((DWORD) GlobTomb4.pTarget);
		break;
	case 407:
		// 407:Lara. (Weapons) Force <&>holsters type for Lara
		RepeatType=1;
		if (Timer==0) {
			// non visualizzare holsters
			*pMeshFondina=13;
			GlobTomb4.StatusNG |= SNG_HIDE_HOLSTERS;

		}else {
			// visualizzare holsters di un certo tipo
			GlobTomb4.StatusNG &= ~SNG_HIDE_HOLSTERS;

			*pMeshFondina= Timer;
		}
		break;

	case 408:
		// 408:Lara. (Weapons) Set current selected weapon<&> (no change for mesh)
		RepeatType=1;
		*GlobTomb4.pAdr->pWeaponSelected = Timer;
		break;
	case 409:
		// 409:Images. Perform <&>InputBox parameters and wait for player input
		// TimerFull 1/999
		RepeatType=1;
		i= GlobTomb4.BaseInputBoxes.VetId[TimerFull];
		if (i==-1) {
			sprintf(BufferLog, "ERROR: there is no Paramaters=PARAM_INPUT_BOX with ID=%d", TimerFull);
			InviaLog(BufferLog);
			break;
		}
		pInputBox = &GlobTomb4.BaseInputBoxes.VetInputBox[i];

		CreaInputBox(pInputBox);
		break;
	case 410:
		// 410:Variables. Text. Clear <&>Text variable
		RepeatType=1;
		pText= LeggiVariabileTesto(Timer);
		pText[0]=0;
		break;

	case 411:
		// 411:Lara. Set <&>Opacity level of Lara for (E)Seconds
		// opacity 0= del tutto invisibile
		// 0xff  disattiva ogni tipo di trasparenza
		RepeatType=1;


		// cercare per precedente azione progressiva
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == 
							AZ_LARA_TRANSPARENT) break;
		
		}
		if (i <GlobTomb4.TotProgressiveActions) {
			// trovata!
			IndiceAzione = i;
			// se pero' era richiestra lara normale (opacita' 255) 
			// allora disattivarla
			if (Timer==255) {
				GlobTomb4.VetProgressiveActions[i].ActionType =0;
				GlobTomb4.TestChangeTransparencyLara =false;
				GlobTomb4.StatusNG &= ~SNG_IMMORTAL_LARA;
				break;
			}
		}else {
			// non c'era.
			// nel caso il trigger richieda opacita' normale (255) togliere trasparenza, uscire e basta
			if (Timer==255) {
				GlobTomb4.TestChangeTransparencyLara =false;
				GlobTomb4.StatusNG &= ~SNG_IMMORTAL_LARA;
				break;
			}

			IndiceAzione = CreaNuovaAzioneProgressiva();
		}
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_LARA_TRANSPARENT;
		Tempo = ExtraTimer;

		if (Tempo != 0) {
			// e' un tempo espresso in secondi, moltiplicare per 30

			Tempo *= FRAME_SECONDO;
		}else {
			Tempo = 0xffff;
		}
		pAzione->Arg1 = Tempo;

		pAzione->Arg2 = Timer;
		pAzione->VetArg[0]=0;

		pAzione->ItemIndex = -1;
		break;
	case 412:
		// Set infinite (-1) amount number of &inventory-item
		IndiceSlot = GlobTomb4.VetInventory[Timer];
		GestionePickups(IndiceSlot,INV_WRITE, -1);
		RepeatType=1;
		break;
	case 413:
		// 413:Lara. (Mesh) COPY TO <&>Extra Lara Slots the meshes taken FROM (E)Slot
		RepeatType=1;

		i = GlobTomb4.pVetMeshSwapSlot[ExtraTimer];
		SwapMeshItem(i, Timer, 0x4000);

		// salvare record swap

		SwapMesh.SlotItem=i;
		SwapMesh.SlotAltroSlot = Timer;
		SwapMesh.TipoSwapMesh = 0x4000;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 414:
		// 414: Copy Mesh. COPY TO <&>Slot the meshes taken FROM (E)Slot
		RepeatType=1;
		SwapMeshItem(GlobTomb4.pVetSlotMeshMoveables[ExtraTimer], 
					GlobTomb4.pVetSlotMeshMoveables[Timer], 0x4000);
		// salvare record swap
		SwapMesh.SlotItem=GlobTomb4.pVetSlotMeshMoveables[ExtraTimer];
		SwapMesh.SlotAltroSlot = GlobTomb4.pVetSlotMeshMoveables[Timer];
		SwapMesh.TipoSwapMesh = 0x4000;
		SalvaRecordSwapMesh(&SwapMesh);

		break;
	case 415:
		// 415: Lara. (Mesh) COPY meshes of Lara from <&>slot to (E)slots
		// extratimer
		// 0: Lara Skin  (standard)
		// 1: Lara Skin + Lara Joints (Slot+1)
		// 2: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2)
		// 3: Lara Skin + Lara Joints (Slot+1) + Hairs (Slot+2) + ShootingHead (Slot+3)
		// 4: Lara Skin + Shooting Head (Slot+1)
		// 5: Lara Skin + Shooting Head (Slot+1) + Hairs (Slot+2)
		RepeatType=1;
		// modalita' solo COPY
		ExtraTimer |= 0x4000;
		SlotSwap = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshLara(SlotSwap, ExtraTimer );
		// salvare l'esecuzione
		SwapMesh.SlotItem=0;
		SwapMesh.SlotAltroSlot = SlotSwap;
		SwapMesh.TipoSwapMesh = ExtraTimer;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 416:
		// 416: Copy Mesh. Copy mesh from <&>Slot to HORIZONT slot
		RepeatType=1;

		i = GlobTomb4.pVetMeshSwapSlot[Timer];
		SwapMeshItem(i, 459, 0x4000);

		// salvare record swap
		SwapMesh.SlotItem=i;
		SwapMesh.SlotAltroSlot = 459;
		SwapMesh.TipoSwapMesh = 0x4000;
		SalvaRecordSwapMesh(&SwapMesh);
		break;
	case 417:
		// FMV. Set <&>FMV as newly playable
		RepeatType=1;
		GlobTomb4.BaseFMV.VetFmvEseguiti[Timer] = 0;
		break;
	case 418:
		// 418:Lara. (Animation) Force Lara to perform <&>Animation of (E)Lara's Extra slot
		RepeatType=1;
		IndiceSlot = VetExtraLaraSlots[ExtraTimer];

		StartAnim = GlobTomb4.pAdr->pVetSlot[IndiceSlot].IndexFirstAnim;
		IndiceAnim = Timer+StartAnim;
		if (GlobTomb4.pAdr->pLara->AnimationNow == IndiceAnim) break;

		GlobTomb4.pAdr->pLara->FrameNow = GlobTomb4.pAdr->pVetAnimations[IndiceAnim].FrameStart;
		GlobTomb4.pAdr->pLara->StateIdCurrent = GlobTomb4.pAdr->pVetAnimations[IndiceAnim].StateId;

		GlobTomb4.pAdr->pLara->AnimationNow = IndiceAnim;
		break;
	case 419:
		// 419:Lara. (Mesh) Swap(E)Mesh of Lara with, same position mesh, of <&>slot
		RepeatType=1;
		IndiceSlot = VetExtraLaraSlots[Timer];

		SwapSingle.ExtraSlot = IndiceSlot;
		SwapSingle.ExtraMesh = ExtraTimer;
		SwapSingle.LaraMesh = ExtraTimer;
		SwapSingle.Flags=0;

		SwapSingleMesh(&SwapSingle);
		SalvaRecordSwapSingleMesh(&SwapSingle);
		break;
	case 420:
		// Lara. (Mesh) Swap Lara's RIGHT HAND mesh with (E)Mesh of <&>Slot
		RepeatType=1;
		IndiceSlot = VetExtraLaraSlots[Timer];
		SwapSingle.ExtraMesh = ExtraTimer;
		SwapSingle.ExtraSlot = IndiceSlot;
		SwapSingle.Flags =0;
		SwapSingle.LaraMesh= 10;
		SwapSingleMesh(&SwapSingle);
		SalvaRecordSwapSingleMesh(&SwapSingle);
		break;
	case 421:
		// 421:Lara. (Mesh) Swap Lara's LEFT HAND mesh with (E)Mesh of <&>Slot
		RepeatType=1;
		IndiceSlot = VetExtraLaraSlots[Timer];
		SwapSingle.ExtraMesh = ExtraTimer;
		SwapSingle.ExtraSlot = IndiceSlot;
		SwapSingle.Flags =0;
		SwapSingle.LaraMesh= 13;
		SwapSingleMesh(&SwapSingle);
		SalvaRecordSwapSingleMesh(&SwapSingle);
		break;
	case 422:
		// 422:Lara. (Animation) Perform <&>animation of CUSTOM_LARA slot, from (E)Frame
		RepeatType=1;
		IndiceSlot = 508;
		EseguiAnimazioneSlot(Timer, IndiceSlot, ExtraTimer, -1);
		break;
	case 423:
		// 423:Lara. (Animation) Perform <&>animation of CUSTOM_LARA slot, setting also (E)State-id
		RepeatType=1;
		IndiceSlot=508;
		EseguiAnimazioneSlot(Timer, IndiceSlot, -1, ExtraTimer);
		break;
	case 426:
	case 427:
	case 428:
		// 426:Camera. Turn current camera of <&>degrees in Clockwise direction in (E)frame time
		// 427:Camera. Reset turning camera, setting all camera view at normal 0 value in <&>frame time
		// 428:Camera. Turn camera of <&>degrees in Counter-Clockwise direction in (E)frame time
		RepeatType=1;

		if (FlipNumber == 427) {
			Speed= Timer;
		}else {
			Speed = ExtraTimer;
		}
		// calcolare i gradi da raggiungere
		if (FlipNumber == 427) {
			Incremento = 0 - GlobTomb4.pAdr->CamRotation;
			MaxView = 0;
		}else {
			MaxView = (Timer * 2 * 182);
			if (FlipNumber == 428) MaxView = -MaxView;

			Incremento = MaxView;

			MaxView = GlobTomb4.pAdr->CamRotation + MaxView;
		}

		// se rischiesto in meno di 2 frame, allora esegire subito, senza progressive action
		if (Speed < 2) {
			GlobTomb4.pAdr->CamRotation = MaxView;
			break;
		}

		// 2 o piu' frames: usare azione progressiva

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TURN_CAMERA ||
				pAzione->ActionType == AZ_RIPPLING_CAMERA) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {

			IndiceAzione = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		}

		
		pAzione->ActionType = AZ_TURN_CAMERA;
		pAzione->Arg1 = Speed;			// durata
		pAzione->VetArgShort[0] = Incremento / Speed; // incremento da eseguire ad ogni ciclo
		pAzione->VetArgShort[1] = MaxView;  // gradi finali da raggiungere

		break;
	case 429:
		// 429:Camera. Turn camera performing a full circle with (E)direction and in <&>frame time
		RepeatType=1;

		i = 65536 / Timer;
		// se anti orario, usare negativo
		if (ExtraTimer == 1) i = -1;

		Incremento=i;
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TURN_CAMERA || 
				pAzione->ActionType == AZ_RIPPLING_CAMERA) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {

			IndiceAzione = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		}

		pAzione->ActionType = AZ_TURN_CAMERA;
		pAzione->Arg1 = Timer;
		pAzione->VetArgShort[0] = Incremento;
		pAzione->VetArgShort[1] = GlobTomb4.pAdr->CamRotation;
		break;

	case 430:
	case 431:
		// 430:Camera. Rippling effect by +/-<&>degrees in (E)frame time (for each degrees range)
		// 431:Camera. Turning. Earthquake effect by +/-<&>degrees in (E)frame time (for each degrees range)
		RepeatType=1;

		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			pAzione= &GlobTomb4.VetProgressiveActions[i];
			if (pAzione->ActionType == AZ_TURN_CAMERA || 
				pAzione->ActionType == AZ_RIPPLING_CAMERA) break;
		}
		if (i == GlobTomb4.TotProgressiveActions) {

			IndiceAzione = CreaNuovaAzioneProgressiva();
			pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		}

		// max view
		MaxView = Timer * 182;

		// set increment
		Incremento = MaxView / ExtraTimer;

		pAzione->ActionType = AZ_RIPPLING_CAMERA;
		pAzione->Arg1 = FlipNumber;
		pAzione->VetArgShort[0] = Incremento;
		pAzione->VetArgShort[1] = -MaxView;
		pAzione->VetArgShort[2] = MaxView;
		break;
	case 432:
	case 433:
		// 432:Text. Start printing of chain texts with 0-255<&>NG String and with proportional (E)time for each string
		// 433:Text. Start printing of chain texts with 256-511<&>NG String and with proportional (E)time for each string

		RepeatType=1;

		// creare azione

		IndiceAzione = CreaNuovaAzioneProgressiva();
		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];

		pAzione->ActionType = AZ_CHAIN_TEXTS;
		pAzione->Arg1 = 1;  // tempo durata testo precedente
		pAzione->Arg2 = ExtraTimer; // tipo tempo
		if (FlipNumber == 433) Timer+= 256;
		pAzione->ItemIndex  = Timer;    // indice inizio chain
		break;
	case 434:
		// 434:Variables.Numeric. Perform the change sign <&>Operation on CurrentValue variable#REMARK#Operation on single CurrentValue variable
		RepeatType=1;

		CurrentValue = LeggiVariabile(VAR_TYPE_CURRENT_VALUE);

		switch (Timer) {
		case 0:
			// 0:Abs (force plus sign)
			CurrentValue = abs(CurrentValue);
			break;
		case 1:
			// 1:Invert sign (negation)
			CurrentValue = -CurrentValue;
			break;
		case 2:
			// 2:Force minus sign (convert to negative the value if it was positive)
			if (CurrentValue > 0) CurrentValue = -CurrentValue;
			break;
		}

		ScriviVariabile(VAR_TYPE_CURRENT_VALUE, CurrentValue);
		break;
	case 435:
		// 435:Variables. Numeric. Perform following <E>operation between CurrentValue and <&>Variable with outcome saved to CurrentValue variable
		RepeatType=1;
		CurrentValue = LeggiVariabile(VAR_TYPE_CURRENT_VALUE);

		Codice = ConvertiCodVar(Timer);
		Variable= LeggiVariabile(Codice);

		switch (ExtraTimer) {
		case 0:
			// 0:Add (CurrentValue = Variable + CurrentValue)
			CurrentValue += Variable;
			break;
		case 1:
			// 1:Subtract (CurrentValue = CurrentValue - Variable)
			CurrentValue -= Variable;
			break;
		case 2:
			// 2:Subtract (CurrentValue = Variable - CurrentValue)
			CurrentValue = Variable-CurrentValue;
			break;
		case 3:
			// 3:Multiply (CurrentValue = CurrentValue * Variable)
			CurrentValue *= Variable;
			break;
		case 4:
			// 4:Division (CurrentValue = CurrentValue / Variable)
			if (Variable == 0) {
				InviaLog("Error: flipeffect 435: attempt to divide by 0");
				break;
			}
			CurrentValue /= Variable;
			break;
		case 5:
			// 5:Division (CurrentValue = Variable / CurrentValue)
			if (CurrentValue ==0) {
				InviaLog("Error: flipeffect 435: attempt to divide by 0");
				break;
			}
			CurrentValue = Variable / CurrentValue;
			break;
		case 6:
			// 6:Average (CurrentValue = (CurrentVariable + Variable) / 2
			CurrentValue += Variable;
			CurrentValue /= 2;
			break;
		case 7:
			// 7:Littler (CurrentValue = the littler value between CurrentValue and Variable)
			if (CurrentValue > Variable) CurrentValue =Variable;
			break;
		case 8:
			// 8:Bigger (CurrentValue = the bigger value between CurrentValue and Variable)
			if (Variable > CurrentValue) CurrentValue = Variable;
			break;
		}
		ScriviVariabile(VAR_TYPE_CURRENT_VALUE, CurrentValue);
		break;
	case 436:
		// 436:Variables. Numeric. Copy to <&>Variale the power by 2 (E)value
		RepeatType=1;
		// compute power by 2 on ExtraTimer
		Valore = 1 << ExtraTimer;
		// save outcome to given variable
		Codice = ConvertiCodVar(Timer);
		ScriviVariabile(Codice, Valore);
		break;
	case 437:
		// 437:Slot. Scale size of items in dynamic way with data of <&>PARAM_MOV_SCALE_ITEM Parameters= script command
		RepeatType=1;
		// locate id parameter

		i = GlobTomb4.BaseMovScale.VetIdMovScale[Timer];

		if (i == -1) {
			sprintf(BufferLog,"ERROR: missing PARAM_MOV_SCALE_ITEM with Id = %d", 
				TimerFull);
			InviaLog(BufferLog);
			break;
		}
		pMovScale = &GlobTomb4.BaseMovScale.VetMovScaleItem[i];

		// veririfcare che non sia gia' attiva un'azione progressiva con questo id param
		
		// e' dinamico: creare un azione progressiva
		// a meno che non sia gia' presente
		for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {
			if (GlobTomb4.VetProgressiveActions[i].ActionType == AZ_SCALE_MOV_ITEM &&
				GlobTomb4.VetProgressiveActions[i].Arg2  == Timer) {

				GlobTomb4.VetProgressiveActions[i].ActionType = 0;
				sprintf(BufferLog, "Flipeffect 437: progressive action for dynamic scaling of moveable item of %d slot is already in progress",
					pMovScale->Slot);
				InviaLog(BufferLog);
				break;
			}
		}
		if (i < GlobTomb4.TotProgressiveActions) break;

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		
		pAzione->ActionType = AZ_SCALE_MOV_ITEM;
		pAzione->ItemIndex = pMovScale->Slot;
		pAzione->Arg1 = pMovScale->FrameDurate;
		pAzione->Arg2 = Timer;

		// impostare come float i valori di cambio dinamico
		pAzione->VetArgFloat[0] = (float) pMovScale->StartSize;
		pAzione->VetArgFloat[2] = (float) pMovScale->EndSize;
		// calcolo incremento
		pAzione->VetArgFloat[1] = pAzione->VetArgFloat[2] - pAzione->VetArgFloat[0];
		pAzione->VetArgFloat[1] /= (float) pAzione->Arg1;



		// calcolo incremento per offset y
		pAzione->VetArgFloat[3] = pMovScale->StartYOffset;
		pAzione->VetArgFloat[5] = pMovScale->EndYOffset;

		pAzione->VetArgFloat[4] = pAzione->VetArgFloat[5] - pAzione->VetArgFloat[3];
		pAzione->VetArgFloat[4] /= (float) pAzione->Arg1;
		break;

	default:
		RepeatType=1;

		sprintf(BufferLog,"WARNING: unknown flipeffect number #%d", FlipNumber);
		InviaLog(BufferLog);


		break;


	}
	// se trigger aveva pulsante one-shot selezionare blocco totale
	if (Flags & SCANF_BUTTON_ONE_SHOT) RepeatType=2;

	return RepeatType;

}


