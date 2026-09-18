// NOTA: questo file non fa parte dei sorgenti del plugin ma ho rachiuso in questo file tutto il codice
// di trng usato per la gestione dei plugin
#include "stdafx.h"
#include "stdio.h"
#include <malloc.h>
#include <shellapi.h>
#include <math.h>
#include "bass.h"

#include "myconstants.h"
#include "Tomb_NextGeneration.h"
#include "macros.h"

#include "structures.h"
#include "MyStructures.h"
#include "DefTomb4Funct.h"

// ----------------- VARIABILI ESTERNE ------------------------------
// #TAG_ADDING_NEW_VARIABLES#
//...

extern StrMyGlob MyGlobPrivate;
extern int TipoExe; 

extern int TotCheckMemory;
extern StrGlobaliTomb4 GlobTomb4;
extern int TotAllocata;
extern void * VetAllocate[MAX_ALLOCA];
extern int TestBloccoTRY;
extern char BufferStatistiche[10000];
extern char BufferLog[8192];
extern char Dir_Trle[256];
extern char MexVersione[];
extern HINSTANCE MiaIstanzaDll;
extern StrCheckOverflow VetCheckMemory[128];
extern StrGlobMisc BaseGlobMisc;
extern TYPE_EnableBaddieAI EnableBaddieAI;
extern TYPE_KillItem KillItem;
extern TYPE_WadeSplash WadeSplash;
extern TYPE_RemoveActiveItem RemoveActiveItem;
extern TYPE_DisableBaddieAI DisableBaddieAI;
extern TYPE_AddActiveItem AddActiveItem;
extern TYPE_GetFloor GetFloor;
extern TYPE_GetHeight GetHeight;
extern TYPE_AlterFloorHeight AlterFloorHeight;
// ----------------- FUNZIONI ESTERNE --------------------------
// #TAG_ADDING_NEW_DECLARES#
void RiprendiLogScript(int IndiceSave);
void SwapMeshLara(int IndicePerSlot, int TipoSwapMesh);
WORD ConvertiCodVar(WORD CodiceTimer);
 int LeggiVariabile(WORD CodiceVariabile);
// -> last declare line
// -> last declare line
int ProporzioneDistanza(int Incremento, int Distanza);
void EseguiAnimNemico(StrItemTr4 *pOggetto, WORD IndiceAnim, short NextStateId);
void SetFogEnd(void);
void SalvaDimensioneSchermo(void);
char *LeggiVariabileTesto(WORD CodiceVariabile);
void SalvaRecordSwapMesh(StrFlipSwapMesh *pSwap);
void GestioneCdPlay(short NumeroCd, int Loop);
char *GetFileCrash(char * pNomeFile);
char* GetDataFile(char* pNomeFile);
void AggiornaBinocoloSuper(void);
bool IsBoxSettore(Tipo_CollGridTr4 *pFloor);
bool CollideItemConCustom(StrItemTr4 *pItem, StrCustomItem *pCustom, int Bordo);
bool AddTabLogScript(void);
void DeleteTestPosition(int IdTest, bool TestSempre);
int CreaNuovaAzioneProgressiva(void);
void PreparaLancioFilmato(int NumeroFMV);
char *TrovaVersioneFile(char *NomeFile, char *pBufProductVersion);
void DeleteTriggerGroup(int IdTriggerGroup, bool TestSempre);
void SospendiHardCoded(int NAnimazione, int FrameToWait);
bool IsCollisioneConItems(StrItemTr4 *pOggetto, bool TestIgnoraLara, 
						  DWORD X, int Y, DWORD Z, short Room, 
						  int MaxDistance, int MinLargerSize, 
						  int MinHeight, int Tollerance);
bool VerificaTestPosition(StrTestPositionCmd *pPosition, int IndiceOggetto);
void SospendiLogScript(int IndiceSave);
bool GestioneGlobaleOk(StrGlobalTrigger *pRec, bool TestEsegui);
void ScriviVariabileMem(WORD Codice, int Valore);
void GetJointAbsPositionStatic(StrItemTr4 *pItem, StrMovePosition *pMove);
bool IsDoor(WORD Slot);
void LiberaWindowsFont(StrWindowsFont *pFont);
void SospendiAudio(int NuovoCd, int NuovoTestLoop);
HFONT CreaWindowsFont(StrWindowsFont *pFont, bool TestNoResize);
void ScriveVariabileTesto(WORD CodiceVariabile, char *pTesto);
long QuantoLungo(FILE *tempfile);
void RiprendiAudio(void);
int GetIndiceCanale(int IndiceAbs);
WORD ConvertiCodText(WORD CodiceTimer);
bool TestEnvCondition(WORD EnvCondizione, WORD FlagCondition, 
						short DistanzaEnv, WORD Extra);
bool VerificaSingleGlobalTrigger(short GlobalTrigger, short Parametro, bool TestIgnoraParametro);
void AggiornaPosStringa(StrProgressiveAction *pAzione);
DWORD LeggiDirectInput(void);
void SubTabLogScript(void);
void DisegnaMeshAbs(StrPos3d *pPos, int IndiceMesh);
void ScansioneTextFormat(StrPrintString *pText, WORD VetScript[]);
void AggiornaCutsceneCamera(StrCutsceneCamera *pCut);
void MostraSprite2d(RECT *pRect, WORD Slot, int IndiceSprite, BYTE Opacita, COLORREF Colore);
void ConvertiSpriteMicroUnits(RECT *pRect);
void LiberaHdcTomb(StrShowImage *pBase, bool TestMantieniTemp);
bool AllocaHdcTomb(StrShowImage *pBase, bool TestHdcTemp, bool TestWriteHdc);
DWORD TrovaColoreRGB(int IdColor);
WORD GestionePickups(WORD Slot, int Operazione, short Valore);
void StopBassSuoni(int NumeroCanale);
void LiberaImmagine(StrRecordImage *pRecord);
void ShowOggettoSlot(WORD Slot, int CordX, int CordY, WORD OrientX, WORD OrientY, WORD OrientZ, int Distanza);
void DeleteColorRgb(int IdColorRgb,bool TestSempre);
void AddScriptAddEffect(int IndiceOggetto, short IdEffetto, WORD ExtraTimer);
void StampaStringa(int CordX, int CordY, char *pTesto, WORD FlagMicro, int Colore, WORD FlagsAlign);
void AggiungiItemMosso(WORD Indice);
void AvviaPopUpImage(WORD IdImageCmd, StrScriptImage *pImage, WORD Contatore);
void GestioneOverlapImage(StrScriptImage *pImage, WORD Secondi);
void ScriviVariabile(WORD CodiceVariabile, int Valore);
bool AllocaImmagine(int NImage, StrRecordImage *pRecord, int ForceSizeX, int ForceSizeY);
int ReadInputBox(HDC hDC, RECT *pRect, char Buffer[], DWORD MaxChars, WORD RIB_Flags, int SfxSound);
int LeggiVariabileMem(WORD Codice);
char *ReplaceVarPlaceFolders(char *pTesto);
char *GetNomeVariabile(WORD Codice);
void TerminaStandBy(StrBaseStandBy *pStand);
void InizializzaStandBy(StrBaseStandBy *pStand);
int GestioneDemoInProgress(WORD FlagGestioneDemo, StrDemo *pDemo);
COLORREF GetColoreId(short IdColorRgb, COLORREF ColoreDef, bool TestInverti, char *pMexComando);

DWORD InizioCodicePlugin(void);
DWORD FineCodicePlugin(void);
DWORD InizioCodiceOggetti(void);
DWORD FineCodiceOggetti(void);
DWORD InizioCodicePatchTr4(void);
DWORD FineCodicePatchTr4(void);
DWORD InizioCodiceRoom(void);
DWORD FineCodiceRoom(void);
DWORD InizioCodiceListTr4(void);
DWORD FineCodiceListTr4(void);
DWORD InizioCodiceSkills(void);
DWORD FineCodiceSkills(void);
DWORD InizioCodice(void);
DWORD FineCodice(void);

typedef short (__cdecl *TYPE_CreateItem)(void);
typedef void (__cdecl *TYPE_InitialiseItem) (short);
typedef void (__cdecl *TYPE_InitialiseCreature) (short);

typedef void (__cdecl *TYPE_ExplodingDeath2) (short ItemIndex, int TestMistery, DWORD Flags);
typedef void (__cdecl *TYPE_JeepExplode) (StrItemTr4 *pVeicolo);

bool InviaLog(char *pMessaggio);
char *GetFileTrle(char *pNomeFile);
bool ScopreModoFullScreen(void);
char * SoloNome(char FileName[]);
bool GetDataDelFileSys(char *NomeFile, SYSTEMTIME *pData);
bool InviaLog(char *pMessaggio);
int EseguiTriggerGroup(short IdTriggerGroup);
bool PrendiNumero(char *pTesto, int *pValore);
int EseguiScriptTrigger(StrScriptTrigger *pTrigger, int Index);
char *SoloNomeSenzaExt(char FileName[]);
int InStr(DWORD Start, char *pTesto, char *pFind);
int EsecuzioneActionTrigger(WORD PluginID, WORD AzioneTimer, int ItemIndex, WORD Flags);
char ** Split(char *pStringa, char Separatore, 
			  int* pTotPezzi, char *pBufferHost, 
			  char **pVetPtr, bool TestSoloPrimo);
bool FreeMine(void *pMemory);
StrListaFiles *TrovaFiles(char DirSource[], char Maschera[], int *pTotFiles);
void * ReallocMine(void * pMem, DWORD NewSize, char *pDescrizione);
int EsecuzioneFlipeffect(WORD PluginId, int NumeroFlip, WORD Timer, WORD Flags);
int PerformTRNGAction(WORD AzioneTimer, int ItemIndex, WORD Flags);
void * MallocMine(DWORD Size, char *pDescrizione);
int EseguiCondizione(WORD PluginId, WORD Condizione, WORD Indice, 
					 WORD ExtraButtons, bool *pTestRestore, 
					 bool *pTestSalta, int *pTipoBlocco, WORD Flags );
// ----------------- VARIABILI GLOBALI ------------------------

StrTrngInfos Trng;
// ======================================================================================
//                           FUNZIONI GESTIONE PATCH E PLUGINS
// ======================================================================================

// #TAG_ADDING_NEW_FUNCTIONS#
// -> last added function bodies
// -> last added function bodies
// -> last added function bodies


void InviaToLogFile(char *pNomeFile, char *pMessaggio)
{
	FILE *pFile;

	pFile = fopen(pNomeFile, "a");
	if (pFile == NULL) return;
	fprintf(pFile, "%s\r\n", pMessaggio);
	fclose(pFile);


}
void Tomb4MessageBox(char *pMessaggio, char *pTitolo)
{
	static HWND* pTombWind4 = (HWND*) 0x75385C;  //  ;HandleWindowMai
					
	MessageBox(*pTombWind4,pMessaggio,pTitolo,MB_APPLMODAL);

}

// se modo non e' esclusivo visualizza message box, altrimenti restituisce false
// in ogni caso manda il messaggio sul log
bool TryMessageBox(char *pMessage, DWORD ID_Plugin)
{
	static DWORD* pTombWind4 = (DWORD*) 0x75385C;  //  ;HandleWindowMai

	InviaLog(pMessage);
	if (*pTombWind4 == 0 || ScopreModoFullScreen()==false) {
		Tomb4MessageBox(pMessage, MyGlobPrivate.DataBase.pVetPlugins[ID_Plugin].Nome);
		return true;
	}
	return false;
}

// formatta nome di file log intestato a plugin con ID_Interno
// note: manca directory, solo nome
char* FormatLogPluginName(int ID_Interno)
{
	static char NomeFile[256];
	char *pPezzoNome;

	// aggiunge in log personale
	pPezzoNome=SoloNomeSenzaExt(MyGlobPrivate.DataBase.pVetPlugins[ID_Interno].Nome);	
	sprintf(NomeFile,"%s_warm_up_log.txt", pPezzoNome);

	return NomeFile;
}
// se non e' disponibile log scrive file su disco
// anzi comunque lo scrive su disco e in piu' lo mette nel log se posibile
// il messaggio verra' aggiunto ad un file col nome:
// "NOMEPLUGIN_warm_up_log.txt"

// se TestMsgBox=true visusaliza anche messagebox se e' possibile
void InviaErroreLog(char *pMessage, DWORD PluginID, bool TestMsgBox)
{
	char *pNome;

	if (TestMsgBox) {
		// vedere se si puo' mandare message box
		TryMessageBox(pMessage, PluginID);
			
	}else {
		InviaLog(pMessage);
	}
	// aggiunge in log personale
	pNome=GetFileCrash(FormatLogPluginName(PluginID));
	InviaToLogFile(pNome, pMessage);
}

void AzzeraLogFilePlugin(DWORD PluginID)
{

	char *pPezzoNome;
	char NomeFile[256];

	pPezzoNome=SoloNomeSenzaExt(MyGlobPrivate.DataBase.pVetPlugins[PluginID].Nome);	
	sprintf(NomeFile,"%s\\TEMP_LOGs\\%s_warm_up_log.txt", Dir_Trle, pPezzoNome);

	remove(NomeFile);
}
// Invia log con argomenti solo se e' attiva modalita' debug per trng oppure per plugin
bool LogOnDebug(char *szFormat, ...)
{
	static DWORD* pTombWind4 = (DWORD*) 0x75385C;  //  ;HandleWindowMain
	static char MyInternalBuffer[512];
	bool TestAttivo;

	TestAttivo= MyGlobPrivate.TestDebugVersione;
#ifdef DEBUG_ON
	TestAttivo=true;
#endif

	if (*pTombWind4 == 0) return false;
	if (TestAttivo==false) return false;

    va_list pArgs;
    va_start(pArgs, szFormat);
    vsprintf(MyInternalBuffer, szFormat, pArgs);
    va_end(pArgs);

	InviaLog(MyInternalBuffer);

	return true;

}
// controlla se le due zone di memoria sono sovrapposte
bool ConflittoZone(DWORD AlfaStart, DWORD AlfaEnd, 
				   DWORD BetaStart, DWORD BetaEnd, char *pInfo)
{

	if (AlfaStart > BetaEnd) return false;
	if (BetaStart > AlfaEnd) return false;
	// c'e' conflitto
	sprintf(pInfo, "Old zone=(0x%X-0x%X) ; New zone=(0x%X-0x%X)",
		BetaStart, BetaEnd, 
		AlfaStart, AlfaEnd);

	return true;

}
// alloca in MyGlobPrivate.DataBase il nome pNomePlugIn
// e restituisce l'indice che lo identifichera'
int AddPlugin(char *pNomePlugIn, SYSTEMTIME  *pTime)
{
	int i;
	StrMyDatabase *pDB;
	StrPluginRec *pRec;
	DWORD j;

	pDB  = &MyGlobPrivate.DataBase;

	i = pDB->TotPlugins;
	// allocare memoria
	pDB->pVetPlugins = (StrPluginRec *) 
			ReallocMine(pDB->pVetPlugins, sizeof(StrPluginRec) * (i+1), "pVetPlugins in AddPlugin");
	pRec = &pDB->pVetPlugins[i];
	memset(pRec, 0, sizeof(StrPluginRec));

	// copiare come nome il file ma senza .dll
	strcpy(pRec->Nome, pNomePlugIn);
	j=strlen(pRec->Nome);
	pRec->Nome[j-4]=0;


	AzzeraLogFilePlugin(i);
	InviaErroreLog("Attaching Library...",i,false);

	pRec->Data = *pTime;
	// inizializzare valori



	pDB->TotPlugins++;

	return i;

}
// riceve in input un nome di plugin e restituisce l'ID script di quel plugin o -1 se non lo trova
short GetIdScriptPlugin(char *pNome)
{
	int i;

	for (i=0;i<GlobTomb4.pPluginScriptTable->TotPlugins;i++) {
		if (strcmpi(GlobTomb4.pPluginScriptTable->VetPlugins[i].Name , pNome)==0) {
			return GlobTomb4.pPluginScriptTable->VetPlugins[i].PluginId;
		}
	}
	return -1;

}

int Esito;

int IsSfxPlaying(WORD SfxIndex)
{

	Esito=0;

	__asm {
		pushad
	mov eax, 4BF5ACh    ;TestOggettoSoundAttivo
	mov  eax, dword ptr [eax]	; START PROCEDURE 893 ;  SIZE=0x60
	test eax, eax 
	jz   goto3 
	movzx  eax, word ptr [SfxIndex] 
	mov ecx, 7F7580h    ;Ptr_VetSoundIndices
	mov  ecx, dword ptr [ecx]
	mov edx, 7F7584h    ;Ptr_VetSoundInfos
	mov  edx, dword ptr [edx]
	push ebx 
	movsx  ebx, word ptr [ecx][eax*2] 
	push ebp 
	push esi 
	mov  al, [edx+06h][ebx*8] 
	push edi 
	sar  eax, 02h 
	and  eax, 0fh 
	mov  esi,  7F7110h ;ZonaSound + 10h 
	add  eax, ebx 
	xor  edi, edi 
	mov  ebp, eax 
goto1: 
	mov  eax, [esi] 
	cmp  eax, ebx 
	jl   goto2 
	cmp  eax, ebp 
	jge  goto2 
	push edi 
	mov eax, 471F60h ; S_SoundSampleIsPlaying
	call eax
	add  esp, 04h 
	mov dword ptr [Esito], eax
	jmp DopoLoop	
goto2: 
	add  esi, 24h 
	inc  edi 
	cmp  esi,  7F7590h ;ZonaSoundFine + 10h 
	jl   goto1 
DopoLoop:
	pop  edi 
	pop  esi 
	pop  ebp 
	pop  ebx 
goto3: 
		popad

	}

	return Esito;

}
// restituisce il record della struttura di comando Plugin= script con id plugin (script id) fornito
// se non la trova restituisce NULL
StrRecPluginScript *GetPluginScript(WORD ScriptPluginId)
{
	int i;
	StrRecPluginScript *pRec;

	for (i=0;i<GlobTomb4.pPluginScriptTable->TotPlugins;i++) {
		pRec = &GlobTomb4.pPluginScriptTable->VetPlugins[i];

		if (pRec->PluginId == ScriptPluginId) return pRec;
	}

	return NULL;


}

// funzione che permette al plugin di richiede una callback 
// di tipo CB_.. con eventuale extra parametro Index
// in pProc cisara' l'indirizzo della callback
// nota dato che viene chiamata prima di avviare gioco si possono fare le cose
// con calma e per bene, riempiendo anche il database per le risrose (trigger e script)
// usate da ogni plugin
bool WINAPI RequireCallBack(WORD ID_Plugin, int CBValue, int CBT_Flags, int Index, void *pProc)
{

	StrRecPluginScript *pScriptPlugin;
	StrPluginRec *pRec;
	StrRecordCallBack *pCall;
	int i;
	WORD CodeNow;
	int Indice;
	StrMyDatabase *pDB;
	int j;
	char *pNomePlugin;
	char *pNomeAltro;
	char MyBufferTemp[1024];

	StrPluginRec *pRecOld;

	pRec= &MyGlobPrivate.DataBase.pVetPlugins[ID_Plugin];

	if (pRec->TotRequiredCB >= MAX_REQUIRED_CB) {
		sprintf(BufferLog, "ERROR: plugin [%s] reached max number of required callbacks (%d)",
			pRec->Nome, MAX_REQUIRED_CB);
		InviaLog(BufferLog);
		return false;
	}

	pScriptPlugin = GetPluginScript(pRec->PluginIdScript);
	// vedere se la callback richiesta deve essere rifiutata da comando script
	if (pScriptPlugin) {
		for (i=0;i<pScriptPlugin->TotDisable;i++) {
			CodeNow= pScriptPlugin->VetDisable[i];

			Indice= CodeNow & CODE_MASK_INDEX;		
			
			if (CodeNow & CODE_FLIPEFFECT) {

				if (CBValue == CB_FLIPEFFECT && Index == Indice) return true;
			}

			if (CodeNow & CODE_ACTION) {
				if (CBValue == CB_ACTION && Index == Indice) return true;
			}

			if (CodeNow & CODE_CONDITION) {
				if (CBValue == CB_CONDITION && Index == Indice) return true;
			}
		}
	}

	// se callback di tipo replace, dare un avviso se un altro plugin l'aveva gia' richiesta
	if ((CBT_Flags & CBT_REPLACE) != 0 && MyGlobPrivate.TestDebugVersione==true) {
		pDB = &MyGlobPrivate.DataBase;
		pNomePlugin = pDB->pVetPlugins[ID_Plugin].Nome;
		// analizzare solo callback di plugin precedenti

		for (i=1;i < ID_Plugin;i++) {
			pRecOld= &MyGlobPrivate.DataBase.pVetPlugins[i];
			
			for (j=0;j<pRecOld->TotRequiredCB;j++) {
				pCall= &pRec->VetRequiredCB[j];
				if ((pCall->CB_Tipo & CBT_REPLACE) != 0 &&
					pCall->CB_Tipo == CBValue && 
					pCall->Numero == Index) {

					pNomeAltro = pDB->pVetPlugins[i].Nome;
					sprintf(MyBufferTemp, "WARNING: Plugin \"%s\" has gotten a CBT_REPLACE callback (CB_=%d Parameter=%d) already required from \"plugin\" %s",
						pNomePlugin, CBValue, Index, pNomeAltro);
					InviaErroreLog(MyBufferTemp, ID_Plugin,false);

				}
			}
		
		}
	}

	pCall= &pRec->VetRequiredCB[pRec->TotRequiredCB];
	pCall->CB_Tipo = CBValue;
	pCall->Flags = CBT_Flags;
	pCall->Numero = Index;
	pCall->pCall = pProc;

	pRec->TotRequiredCB++;

	// se callback diretta (senza numero e flag) impostarla adesso
	// secondo me si puo' fare piu' rapidamente in questo modo
	if (CBValue < MAX_DIRECT_CB) {
		// e' una callback diretta
		pRec->VetDirectCB[CBValue] = pProc;
	}

	return true;
}
// controlla conflitti e se li trova visualizza errore 
// visualizza warning solo se testwarning =true
// se non ci sono errori inserisci i dati attuali in database
int CheckConflicts(DWORD Offset, int TotBytes, DWORD IndexPlugin, bool TestWarning)
{

	StrMyDatabase *pDB;
	StrCodePatch *pRec;
	DWORD EndOffset;
	int i;
	char MyBufferTemp[4096];
	char *pNomePlugin;
	int ErrorCode;
	StrReservedData *pRes;
	DWORD StartNow;
	DWORD EndNow;
	char *pNomeAltro;

	char BufConflitto[512];
	char MexTipoConflitto[640];
	bool TestIgnora;
	DWORD BloccoStart;
	DWORD BloccoEnd;

	if (TipoExe != EXE_TOMB4) return 0;
	// se iondirizzo e' oltre zona codice tomb4 non memorizzare niente
	if (Offset >= 0x4A7000) return 0;

	ErrorCode=0;
	EndOffset = Offset+TotBytes-1;

	// vedere se ci sono conflitti e con quale tipo di patch
	pDB = &MyGlobPrivate.DataBase;

	if (IndexPlugin >= pDB->TotPlugins) {
		// plugin non registrato
		sprintf(MyBufferTemp, "ERROR: it has been requeried a patch from an unregistered plugin. The Plugin ID %d is not registered\r\n\rnInfo about requiried patch: StartOffset=0x%X  BytesLenght=%d",
			IndexPlugin, Offset, TotBytes);

		Tomb4MessageBox(MyBufferTemp, "ERROR performing plugin patch");
		return APPC_ERROR_PLUGIN_ID_UNREGISTERED;

	}

	pNomePlugin = pDB->pVetPlugins[IndexPlugin].Nome;


	// --------------- CONFRONTO CON TUTTE LE PATCH  --------------------------
	// vedere se la nuova patch e' sopra codice di altra patch codice
	// in questo caso solo warning
	for (i=0;i<pDB->TotCodeZones;i++) {
		pRec = &pDB->pVetCodePatch[i];
		pNomeAltro = pDB->pVetPlugins[pRec->IndexPlugin].Nome;
		StartNow=pRec->Inizio;
		EndNow=pRec->Fine;
		TestIgnora=false;


		if (pRec->FlagsTYPP & TYPP_RILOCATOR) {
			// se patch che riloca procedura, usare come zona critica quella di intera procedura
			BloccoStart = pRec->ProcStart;
			BloccoEnd = pRec->ProcEnd;

		}else {
			// altrimenti usare la zona di inizio e fine codice modificato come sempre
			BloccoStart = pRec->Inizio;
			BloccoEnd = pRec->Fine;
		}
			// vedere che non sia uguale ad una gia' fatta dallo stesso plugin
		
		if (TestIgnora==false && 
				ConflittoZone(Offset, EndOffset, BloccoStart, BloccoEnd, BufConflitto)==true) {
			// c'e' un conflitto, ora analizzare tipo di patch con cui e' entrata in conflitto nuova
			// patch che si stava per applicare
	
			
			if (pRec->FlagsTYPP & TYPP_RUN_TIME) {
				// trovato un conflitto.
				// visto che e' patch dinamica, dare errore non recuperabile
				sprintf(MyBufferTemp,"ERROR: Plugin \"%s\" attempt to overwrite a run-time patch created by \"%s\" code. %s",
					pNomePlugin, pNomeAltro, BufConflitto);
				InviaErroreLog(MyBufferTemp, IndexPlugin, true);

				return APPC_ERROR_PATCH_OVERLAPPED_TO_RUNTIME_PATCH;

			}
			
		
			/// se patch NON parametrica e NON run time e non riloca procedure: impostazioni di default:

			strcpy(MexTipoConflitto, " a patch code created by another plugin or trng.");
			ErrorCode= APPC_WARN_PATCH_OVER_OLD_PATCH;	



			if (pRec->FlagsTYPP & TYPP_PARAMETRIC) {
				// patch parametrica
				if (pRec->FlagsTYPP & TYPP_PARAMETRIC_CONST) {

					sprintf(MexTipoConflitto,"a constant value that it had been previously changed to the new value = 0x%X",
						pRec->NewValue );
				}
				if (pRec->FlagsTYPP & TYPP_PARAMETRIC_CALL) {

					sprintf(MexTipoConflitto, "the address of a call that had been previously changed to point to the new procedure at offset: 0x%X",
						pRec->Inizio  + pRec->NewValue);
				}
				if (pRec->FlagsTYPP & TYPP_PARAMETRIC_MEM) {

					if (pRec->NewValue < 0x8113E7) {
						// e' ancora in zona tomb4
						sprintf(MexTipoConflitto, "the address of a buffer memory (array) that had been previously changed to the new address: 0x%X",
							pRec->NewValue);
					}else {
						// e' memoria allocata dinamicamente
						// dare consiglio
						sprintf(MexTipoConflitto, "the address of a zone memory (0x%X) that had been previously changed in dynamic way from trng. It's necessary replace this addres with that in specific Trng.pGlobTomb4->BaseRemap.Old_... variable",
							pRec->NewValue);

					}				
				}


				ErrorCode= APPC_WARN_PATCH_OVER_CHANGED_ARGUMENTS;				
			}

			if (pRec->FlagsTYPP & TYPP_RILOCATOR) {
				// c'e' conflitto con patch rilocata
				// dare warning se questa o precedenti procedure scrivevano esattamente
				// all'inizio ( o ancora prima) di procedura
				// dare errore in altri casi
				// prima vedere se e' proprio questa la patch che elimina la rilocazione
				if (Offset <= StartNow) {
					// se warning sono disattivati pero bisogna evitare messaggio
					if (TestWarning) {
						sprintf(MexTipoConflitto, " a code to relocate whole procedure starting from offset 0x%x. ", 
							StartNow);

						ErrorCode = APPC_WARN_PLUGIN_REMOVED_RELOCATION_OF_PROCEDURE;

						
					}
					
				}else {
					// la patch non e' all'inizio
					// e' errore vero, uscire subito

					sprintf(MyBufferTemp, "ERROR: plugin \"%s\" tried to write a patch in an unplugged code where there was a procedure rilocated by %s. %s",
						pNomePlugin, pNomeAltro, BufConflitto);
					InviaErroreLog(MyBufferTemp, IndexPlugin, true);
					ErrorCode=APPC_ERROR_PATCH_IN_RILOCATED_PROCEDURE;
					return ErrorCode;
											
				}
			}
			// se patch che e' stata coperta era di tipo restore non dare alcun avviso
			// se l'autore di quella patch era lo stesso plugin attuale
			if ((pRec->FlagsTYPP & TYPP_RESTORE) != 0 &&
				pRec->IndexPlugin == IndexPlugin) {
				TestIgnora=true;
			}
			// se patch che e' stata coperta era di tipo reserved non dare alcun avviso
			// se l'autore di quella patch era lo stesso plugin attuale
			if ((pRec->FlagsTYPP & TYPP_RESTORE) != 0 &&
				pRec->IndexPlugin == IndexPlugin) {
				TestIgnora=true;
			}


			if (TestWarning == true && TestIgnora==false) {


				// visualizzare wanring nel log (se attivi i warning)
				sprintf(MyBufferTemp,"WARNING: the \"%s\" plugin created a patch in a zone where there was %s. \r\n\tOverlapped Zones: %s\r\n\tConflict with plugin: %s",
					pNomePlugin, MexTipoConflitto, BufConflitto, pNomeAltro);
				InviaErroreLog(MyBufferTemp, IndexPlugin, false);
			}else {
				// se warning erano disabilitati, eliminare eventuale errore (warnng) residuo
				ErrorCode=0;
			}
			

		}
	}

	// controllare anche che non ci sia conflitto con reserved memory zone
	// Offset, EndOffset,
	for (i=0;i<pDB->TotReservedData;i++) {
		pRes = &pDB->pVetReserved[i];

		BloccoStart= pRes->StartOffset;
		BloccoEnd = pRes->EndOffset;
		pNomeAltro = pDB->pVetPlugins[pRes->PluginID].Nome;
		if (pRes->PluginID != IndexPlugin) {
			if (ConflittoZone(Offset, EndOffset, BloccoStart, BloccoEnd, BufConflitto)==true) {
				// c'e' un conflitto: dare errore
				sprintf(MyBufferTemp, "ERROR: plugin \"%s\" tried to write a patch in a reserved data zone by %s plugin. %s",
						pNomePlugin, pNomeAltro, BufConflitto);
					InviaErroreLog(MyBufferTemp, IndexPlugin, true);
					ErrorCode=APPC_ERROR_PATCH_OVERLAPPED_TO_RESERVED_ZONE;
					return ErrorCode;
			}
		}
		
	}
	return ErrorCode;
}


// salva patch di tipo parametrico
// NOTA: va chiamata solo UNA volta per informare delle caratterisriche gneerali e non per ogni elemnto del vettore
void SalvaPatchArg(int PluginId, DWORD Offset, DWORD OffsetLast, DWORD NewValue, WORD Flags, WORD Size)
{
	int i;
	StrMyDatabase *pDb;
	StrArgPatch *pRec;

	if (TipoExe != EXE_TOMB4 || GlobTomb4.TestSetup==true) return;


	pDb = &MyGlobPrivate.DataBase;

	i = pDb->TotPatchArg;
	pDb->pVetPatchArg = (StrArgPatch*) realloc(pDb->pVetPatchArg, 
										sizeof(StrArgPatch) * (i+1));
	pRec = &pDb->pVetPatchArg [i];

	pRec->PluginId = PluginId;
	pRec->OffsetFirst = Offset;
	pRec->OffsetLast = OffsetLast;
	pRec->ValoreNew = NewValue;
	pRec->Flags = Flags;
	pRec->Size = Size;
	pDb->TotPatchArg++;

}

// salva in MyGlobPrivate.DataBase.pVetMemZone
// le info su un blocco di codice modificato da una qualunque patch
// nota: NON SCRIVE BYTE DELLA PATCH, salva solo le informazioni
void SalvaInfoZonaPatch(DWORD ID_Plugin, DWORD Start, int TotBytes, WORD Flags, 
						int NewValue, DWORD ProcStart, DWORD ProcEnd)
{
	StrMyDatabase *pDB;
	int i;
	StrCodePatch *pRec;


	if (TipoExe != EXE_TOMB4 || GlobTomb4.TestSetup==true) return;


	pDB = &MyGlobPrivate.DataBase;

	i = pDB->TotCodeZones;

	pDB->pVetCodePatch = (StrCodePatch*) 
			ReallocMine(pDB->pVetCodePatch, sizeof(StrCodePatch) * (i+1), "pVetCodePatch in SalvaInfoZonaPatch");
	
	pRec = &pDB->pVetCodePatch[i];

	pRec->Inizio = Start;
	pRec->IndexPlugin = (WORD) ID_Plugin;
	pRec->Fine = Start+TotBytes-1;
	pRec->NewValue = NewValue;
	pRec->FlagsTYPP = Flags;
	pRec->ProcStart = ProcStart;
	pRec->ProcEnd = ProcEnd;

	pDB->TotCodeZones++;

}

// applica tutte le patch in posizioni di VetOffset() TotOffset
// inserendo il valore relativo per effettuare una CALL NewOffset
// chiamata solo da plugin 
// restituisce errorcode
int Plugin_PatchCall(StrPatchInfo *pDataPatch, bool TestWarnings)
{
	int i;
	DWORD NewValue;
	DWORD *pCodice;
	WORD Flags;
	DWORD *VetOffset;
	int TotOffset;
	DWORD NewOffset;

	VetOffset = (DWORD *) pDataPatch->pVetItems;
	TotOffset = pDataPatch->TotItems;
	NewOffset = pDataPatch->NewValue;

	Flags = TYPP_PARAMETRIC | TYPP_PARAMETRIC_CALL;
	if (MyGlobPrivate.TestPatchDinamiche==true) Flags |= TYPP_RUN_TIME;

	for (i=0;i<TotOffset;i++) {
		// controllo conflitti
		pDataPatch->ErrorCode = CheckConflicts(VetOffset[i],4,pDataPatch->PluginId, TestWarnings);
		if (pDataPatch->ErrorCode < 0) {
			// c'e' stato errore
			return pDataPatch->ErrorCode;
		}

		NewValue = VetOffset[i] + 4;
		NewValue = NewOffset - NewValue;
		pCodice = (DWORD *) VetOffset[i];
		*pCodice = NewValue;
		SalvaInfoZonaPatch(0, VetOffset[i], 4, Flags, NewOffset, 0,0);

	}
	// ora salvare patch parametrica
	SalvaPatchArg(0, VetOffset[0], VetOffset[TotOffset-1], NewValue, Flags,4);
	return 0;
}

// applica patch parametrica (usata solo da plugin)

int Plugin_ApplicaPatchImmediata(StrPatchInfo *pDataPatch, bool TestWarnings)
{
	StrParamPatch *pVetPatch;
	int TotPatch; 
	int Valore;
	int i;
	StrParamPatch *pRec;
	char *pChar;
	short *pShort;
	int *pInt;
	char ValoreByte;
	short ValoreWord;
	int ValoreDword;
	int Differenza;
	int NBytes;
	WORD Flags;
	WORD ID_Plugin;

	pVetPatch = (StrParamPatch*) pDataPatch->pVetItems;
	TotPatch = pDataPatch->TotItems;
	Valore = pDataPatch->NewValue;
	ID_Plugin = (WORD) pDataPatch->PluginId;

	Flags = TYPP_PARAMETRIC | TYPP_PARAMETRIC_CONST;
	if (MyGlobPrivate.TestPatchDinamiche==true) Flags |= TYPP_RUN_TIME;

	for (i=0;i<TotPatch;i++) {
		pRec = &pVetPatch[i];
		Differenza = pRec->Gap;
		switch (pRec->SizeType ) {
		case 'B':
			// tipo byte
			pDataPatch->ErrorCode = CheckConflicts(pRec->Offset, 1, ID_Plugin, TestWarnings);
			if (pDataPatch->ErrorCode < 0) return pDataPatch->ErrorCode;

			ValoreByte = (char) (Valore + Differenza);
			pChar = (char *) pRec->Offset;
			*pChar = ValoreByte;
			NBytes=1;
			break;
		case 'W':
			pDataPatch->ErrorCode = CheckConflicts(pRec->Offset, 2, ID_Plugin, TestWarnings);
			if (pDataPatch->ErrorCode < 0) return pDataPatch->ErrorCode;

			ValoreWord = (short) (Valore + Differenza);
			pShort = (short *) pRec->Offset;
			*pShort = ValoreWord;
			NBytes=2;
			break;
		case 'D':
			pDataPatch->ErrorCode = CheckConflicts(pRec->Offset, 4, ID_Plugin, TestWarnings);
			if (pDataPatch->ErrorCode < 0) return pDataPatch->ErrorCode;

			ValoreDword = (int) (Valore + Differenza);
			pInt = (int *) pRec->Offset;
			*pInt = ValoreDword;
			NBytes=4;
			break;
		}

		SalvaInfoZonaPatch(ID_Plugin, pRec->Offset, NBytes, Flags, Valore,0,0);

	}

	SalvaPatchArg( ID_Plugin, pVetPatch[0].Offset, pVetPatch[TotPatch-1].Offset, Valore, Flags, 0); 
	return 0;
}

// chiamata sia da trng che da plugins
// alloca una zona di memoria riserva usata per propri dati
// non c'e' nessuna reale patch, viene solo salvata l'informazione che quella zona di memoria e' gia' usata
// da una engine.
// restituisce APPC_OK (0) per tutto ok, oppure un errore negativo APPC_..

int SetReservedZone(DWORD ID_Plugin, DWORD StartOffset, int NumberOfBytes)
{
	StrMyDatabase *pDB;
	int i;
	StrReservedData *pRec;
	DWORD EndOffset;
	char BufZoneConflitto[256];
	char BufferTemp[512];
	char *pNome;
	char *pNomeAltro;
	int RetValue;

	EndOffset = StartOffset + NumberOfBytes -1;

	// verificare conflitti
	pDB = &MyGlobPrivate.DataBase;
	for (i=0;i<pDB->TotReservedData;i++) {
		pRec = &pDB->pVetReserved[i];
		
		if (ID_Plugin != pRec->PluginID) {

			if (ConflittoZone( StartOffset, EndOffset, pRec->StartOffset, pRec->EndOffset, BufZoneConflitto) == true) {
				pNome = pDB->pVetPlugins[ID_Plugin].Nome;
				pNomeAltro = pDB->pVetPlugins[pRec->PluginID].Nome;

				// conflitto tra reserved zones: dare errore
				sprintf(BufferTemp, "ERROR: plugin \"%s\" tried to reserve a data zone that had been already used/reserved by other plugin \"%s\" %s",
					pNome, pNomeAltro, BufZoneConflitto);
				InviaErroreLog( BufferTemp, ID_Plugin,true);
				return APPC_ERROR_RESERVED_ZONE_ALREADY_USED;
				
			}	
		}
	}
	// verificare conflitto tra zona che si richiede ora e zone patch 
	RetValue=CheckConflicts(StartOffset, NumberOfBytes-1, ID_Plugin, true);
	if (RetValue < 0) return RetValue;

	// aggiungere nuovo record
	i = pDB->TotReservedData;
	pDB->pVetReserved  = (StrReservedData*) 
			ReallocMine(pDB->pVetReserved , sizeof(StrReservedData) * (i+1), "Realloc pVetReserved in SetReservedZone()");
	pRec = &pDB->pVetReserved[i];

	pRec->StartOffset = StartOffset;
	pRec->EndOffset = EndOffset;
	pRec->PluginID = ID_Plugin;
	pDB->TotReservedData++;

	return RetValue;

}
// restituisce Errore
// 0 = tutto ok
// > 0 avviso. la patch e' stata applicata ma con qualche problema
// < 0 errore. la patch non e' stata applicata
// questa patch verra' chiamata anche da plugin esterni


int WINAPI SetNewPatch(StrPatchInfo *pDataPatch, bool TestWarnings)
{
	int ErrorCode;
	int i;
	BYTE *pMem;
	DWORD Offset;
	DWORD ID_Plugin;
	BYTE *pBytes;
	int TotItems;
	WORD Flags;
	DWORD BloccoStart;
	DWORD TotSize;
	char *pNomePlugin;
	StrMyDatabase *pDB;
	char MyBufferTemp[4096];

	Offset = pDataPatch->StartOffset;
	TotItems = pDataPatch->TotItems;
	ID_Plugin = pDataPatch->PluginId;
	Flags = pDataPatch->PatchType;
	ErrorCode=0;

	if (MyGlobPrivate.TestPatchDinamiche==true) Flags |= TYPP_RUN_TIME;
	ErrorCode=0;
	pDB = &MyGlobPrivate.DataBase;
	pNomePlugin = pDB->pVetPlugins[ID_Plugin].Nome;

	// se patch e' richiesta da plugin, verificare che, se e' runtime e di tipo diverso da parametric const
	// , va rifiutata
	if (ID_Plugin > 0 && (Flags & TYPP_RUN_TIME) !=0 && 
		(Flags & (TYPP_CONTIGUOUS_BYTES | TYPP_PARAMETRIC_CALL | TYPP_RESTORE)	!= 0)) {
		// richiesta patch in run time di tipo non concesso.
		ErrorCode = APPC_ERROR_WRONG_PATCH_FOR_RUN_TIME;
		pDataPatch->ErrorCode = ErrorCode;
		sprintf(MyBufferTemp, "ERROR: failed request from \"%s\" plugin, to apply a patch from address: 0x%X. It's not allowed performing this kind of patch in run-time mode. Set the call for this patch in CreateMyCodePatches() function, please.",
			pNomePlugin, pDataPatch->StartOffset);

		InviaErroreLog( MyBufferTemp, ID_Plugin, true);

		return ErrorCode;
	}

	// se a richiederla non e' trng ma un plugin e cerca di scrivere all'esterno di segmento codice
	// rifiutare
	if (ID_Plugin && (Flags & TYPP_CONTIGUOUS_BYTES) !=0) {
		// controllare limiti 
		if ((pDataPatch->StartOffset + TotItems) > 0x4A6FFF) {
			sprintf(MyBufferTemp, "ERROR: plugin \"%s\" tried to create a patch (at offset 0x%X) outside of code segment (0x401000 - 0x4A6FFF)",
				pNomePlugin, pDataPatch->StartOffset);
			InviaErroreLog(MyBufferTemp, ID_Plugin, true);
			pDataPatch->ErrorCode = APPC_ERROR_PATCH_OUTSIDE_CODE_SEGMENT;
			return pDataPatch->ErrorCode;
		}
	}
			

	// se patch restore, costruire ora la sequenza di byte da copiare
	if (Flags & TYPP_RESTORE) {
		pDataPatch->PatchType |= TYPP_CONTIGUOUS_BYTES;

		if (MyGlobPrivate.pTomb4Mem == NULL) {

			sprintf(MyBufferTemp, "ERROR: failed request from \"%s\" plugin, to restore zone code: 0x%X / 0x%X . It has been an internal error and original tomb4 image has not been found",
				pNomePlugin, pDataPatch->StartOffset, pDataPatch->StartOffset + TotItems);

			InviaErroreLog( MyBufferTemp, ID_Plugin, true);
			pDataPatch->ErrorCode = APPC_ERROR_NOT_FOUND_TOMB4_IMAGE;
			return pDataPatch->ErrorCode;
		}

		// ottenere l'indice relativo di image mem dove inizia codice che vogliamo ripristinare
		i = pDataPatch->StartOffset - 0x401000;  
		// verificare che la zona da copiare non sia esterna al range di inndirizzo codici coperti
		// dalla memoria code originale nelle risorse
		if (i < 0 || (i+ TotItems) >= 679936) {
			sprintf(MyBufferTemp,"ERROR:  failed request from Plugin \"%s\" to restore  the code zone at: 0x%X - 0x%X,  because this block is not fully inside at global tomb4 code range (0x401000 - 0x4A6FFF)",
				pNomePlugin, pDataPatch->StartOffset, pDataPatch->StartOffset + TotItems);

			InviaErroreLog( MyBufferTemp, ID_Plugin, true);
			pDataPatch->ErrorCode = APPC_ERROR_RESTORE_ZONE_OUTSIDE_CODE_RANGE;
			return pDataPatch->ErrorCode ;
		}

		// ok, ora impostare vettore
		pDataPatch->pVetItems = (BYTE *) &MyGlobPrivate.pTomb4Mem[i];

	}
	// se patch rilocatrice, controllare i conflitti con l'intera zona di procedura rilocata
	if (Flags & TYPP_RILOCATOR) {
		BloccoStart = pDataPatch->ProcStart;
		TotSize = pDataPatch->ProcEnd - pDataPatch->ProcStart + 1;
	}else {
		BloccoStart = pDataPatch->StartOffset;
		TotSize = pDataPatch->TotItems;
	}

	if (pDataPatch->PluginId != 0  &&
		(Flags & TYPP_CONTIGUOUS_BYTES)!=0) {
		// contorllo conflitti solo per patch di plugin

		// prima verificare che non ci siano confliti
		ErrorCode = CheckConflicts(BloccoStart, TotSize, ID_Plugin, TestWarnings);

		// se e' errore lo mostra subito
		// se e' warning lo mostra solo se testwarning=true
		if (ErrorCode < 0) {
			// fallito,  non continuare: niente patch
			return ErrorCode;

		}
	}

	if (Flags & TYPP_CONTIGUOUS_BYTES) {
		// eseguire patch contiguous e salvare info
		pBytes = (BYTE *) pDataPatch->pVetItems;
		// eseguire patch
		pMem = (BYTE *) Offset;

		for (i=0;i<TotItems;i++) {
			pMem[i] = pBytes[i];
		}

		// ora salvare i dati di questa patch in database
		SalvaInfoZonaPatch(ID_Plugin, Offset, TotItems, Flags, 0, pDataPatch->ProcStart, pDataPatch->ProcEnd);
		pDataPatch->ErrorCode = ErrorCode;

		return ErrorCode;
	}

	// da qui in poi puo essere solo una patch di plugin
	// vedere di che tipo
	ErrorCode=0;
	pDataPatch->ErrorCode=0;

	if (Flags & TYPP_PARAMETRIC) {
		// patch non contigua con diversi valori modificati in piu punti
		
		// TYPP_PARAMETRIC_CALL
		if (Flags & TYPP_PARAMETRIC_CALL) {
			// eseguire patch call
			ErrorCode = Plugin_PatchCall( pDataPatch, TestWarnings);
			pDataPatch->ErrorCode=ErrorCode;
		}

		if (Flags & TYPP_PARAMETRIC_CONST) {
			// eseguire patch parametrica
			ErrorCode = Plugin_ApplicaPatchImmediata(pDataPatch, TestWarnings);
			pDataPatch->ErrorCode = ErrorCode;
		}

	}
		
	return ErrorCode;
}
// questa e' sempre nel codice  trng e ha sempre pluginid=0
// fixata per nuova versione
//	c'e' poi una funzione con  stesso nome nel codice di plugin che chiama anch'essa SetNewPatch
// nota: SetNewPatch, che viene chiamata, e' identica sia per trng che per plugin
int ApplyCodePatch(DWORD Offset, BYTE *pVetBytes, int TotBytes) 
{
	bool TestWarnings;
	StrPatchInfo DataPatch;

#ifdef DEBUG_ON
	TestWarnings=true;
#else
	TestWarnings=false;
#endif

	DataPatch.ErrorCode =0;
	DataPatch.PluginId =0;  // chiamata sempre da trng
	DataPatch.pVetItems = pVetBytes;
	DataPatch.StartOffset = Offset;
	DataPatch.ProcStart = 0;
	DataPatch.ProcEnd = 0;
	DataPatch.TotItems = TotBytes;
	DataPatch.PatchType = TYPP_CONTIGUOUS_BYTES;

	return SetNewPatch(&DataPatch,TestWarnings);

}
// applica patch codice che serve a rilocare un'intera procedura compresa tra ProcStart e EndProc
int ApplyCodePatchXRiloca(DWORD Offset, BYTE *pVetBytes, int TotBytes, DWORD ProcStart, DWORD ProcEnd)
{

	bool TestWarnings;
	StrPatchInfo DataPatch;

#ifdef DEBUG_ON
	TestWarnings=true;
#else
	TestWarnings=false;
#endif

	DataPatch.ErrorCode =0;
	DataPatch.PluginId =0;  // chiamata sempre da trng
	DataPatch.pVetItems = pVetBytes;
	DataPatch.StartOffset = Offset;
	DataPatch.ProcStart = ProcStart;
	DataPatch.ProcEnd = ProcEnd;
	DataPatch.TotItems = TotBytes;
	DataPatch.PatchType =  TYPP_CONTIGUOUS_BYTES+TYPP_RILOCATOR;

	return SetNewPatch(&DataPatch,TestWarnings);
}
// riceve in input il vettore id di una struttura parameter e il max id permessi
// e restituisce un id non ancora = -1
short GetNuovoIdParam(short *VetId, int TotIds)
{
	short i;


	for (i=1;i<TotIds;i++) {
		if (VetId[i] == -1) break;
	}

	return i;


}
// questa procedura usa lo stesso codice di leggingsceripterplevel
// ma imposta i valori word puntati da pVetArg  per creare un Parameters=ParamValue
// in pNewId viene restituito l'id assegnato al comando
// se fallisce restituisce false

bool CreateNewParamCommand(WORD IdPlugin, short ParamValue, int TotArg, WORD *pVetArg, int *pNewId)
{
	int Indice;
	int i;
	StrParseNGField  ParseField;
	StrRecordParamLgtn *pLightning;
	StrCircle *pCircle;
	char MyBufInterno[200];
	StrTransformer *pTrans;
	StrMyRect *pMyRect;
	StrColoraItem *pColora;
	StrQuad *pQuad;
	int Index, n;
	StrScaleMovItem *pMovScale;
	StrRotateItem *pRotate;
	StrParamPrintText *pPrint;
	StrSwapAnim *pSwap;
	int TotItems;
	StrSpeechActor *pSpeech;
	StrParamTriangle *pTria;
	StrSetCamera *pCamera;
	StrMoveParameters *pMoveItem;
	WORD TotWords;
	StrShowSprite *pShowSprite;
	StrScaleItem *pScale;
	int j;

	// impostare i dati come fossero in un extra ng header
	ParseField.pData=pVetArg;
	ParseField.NextIndex = TotArg;
	ParseField.SizeData = TotArg*2;
	ParseField.StartDataIndex =0;
	ParseField.Type =NGTAG_SCRIPT_LEVEL;

	Indice=TotArg;
	i=0;
	TotWords = TotArg+1;
	n=ParamValue;
	*pNewId=-1;

	// il primo parametro e' sempre un PARAM_...
	// che definisce il tipo di lista
	switch (ParamValue) {

	case PARAM_ACTOR_SPEECH:
		n= GlobTomb4.pBaseSpeechActor->TotSpeechActor;
		if (n >= MAX_SPEECH_PARAM) {
			sprintf(BufferLog, "WARNING: too much Parameters=PARAM_ACTOR_SPEECH commands in current level section: %d",
					MAX_SPEECH_PARAM);
			InviaLog(BufferLog);
			return false;
		}

		pSpeech = &GlobTomb4.pBaseSpeechActor->VetSpeechActor[n];
		// TROVA ID LIBERO:
		pSpeech->IdSpeech = GetNuovoIdParam(GlobTomb4.pBaseSpeechActor->VetId, MAX_SPEECH_PARAM*10);
		LogOnDebug("Created %d=Parameters=PARAM_ACTOR_SPEECH", pSpeech->IdSpeech);

		*pNewId = pSpeech->IdSpeech;

		if (pSpeech->IdSpeech > MAX_SPEECH_PARAM*10) {
			sprintf(BufferLog, "ERROR: too big ID value (%d) for PARAM_ACTOR_SPEECH command in current level section",
				pSpeech->IdSpeech);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.pBaseSpeechActor->VetId[pSpeech->IdSpeech] = n;

		pSpeech->Flags = ParseField.pData[i++];
		if (pSpeech->Flags == SCRIPT_IGNORE) pSpeech->Flags=0;

		pSpeech->Parameter = ParseField.pData[i++];

		pSpeech->FrameRate = ParseField.pData[i++];
		if (pSpeech->FrameRate==-1) pSpeech->FrameRate = 5;
		
		pSpeech->SpeechSlot = ParseField.pData[i++];

		pSpeech->HeadSlotMesh = ParseField.pData[i++];

		pSpeech->FirstSpeechMesh = ParseField.pData[i++];

		pSpeech->TotSpeechMesh = ParseField.pData[i++];

		pSpeech->TotCommands = Indice-i;

		for (j=0;j<pSpeech->TotCommands;j++) {
			pSpeech->VetCommands[j] = ParseField.pData[i++];
		}
		pSpeech->Dynamic = IdPlugin;
		GlobTomb4.pBaseSpeechActor->TotSpeechActor++;
		break;


	case PARAM_MOVE_ITEM:
		n = GlobTomb4.BaseMoveItem.TotMove;
		if (n >= MAX_MOVE_PARAM) {
			sprintf(BufferLog,"WARNING: too much Parameters PARAM_MOVE_ITEM command in current level section: %d",
				n);
			InviaLog(BufferLog);
			return false;
		}

		pMoveItem = &GlobTomb4.BaseMoveItem.VetMove[n];
		// TROVA ID LIBERO:
		pMoveItem->IdMove  = GetNuovoIdParam(GlobTomb4.BaseMoveItem.VetID, MAX_MOVE_PARAM*10);
		*pNewId = pMoveItem->IdMove;
		LogOnDebug("Created %d=Parameters=PARAM_MOVE_ITEM", pMoveItem->IdMove);

		if (pMoveItem->IdMove >= MAX_MOVE_PARAM*10) {
			sprintf(BufferLog, "ERROR: Too big ID value (%d) for PARAM_MOVE_ITEM command in current level section",
				pMoveItem->IdMove);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseMoveItem.VetID[pMoveItem->IdMove] =n;
		pMoveItem->Flags = ParseField.pData[i++];
		if (pMoveItem->Flags == SCRIPT_IGNORE) pMoveItem->Flags =0;
		pMoveItem->ItemIndex = ParseField.pData[i++];
		pMoveItem->Direzione = ParseField.pData[i++];
		pMoveItem->Distanza = ParseField.pData[i++];
		pMoveItem->Speed = ParseField.pData[i++];
		if (pMoveItem->Speed == SCRIPT_IGNORE) pMoveItem->Speed=0;

		pMoveItem->SuonoMovimento = ParseField.pData[i++];
		pMoveItem->SuonoFinale = ParseField.pData[i++];
		pMoveItem->Extra = ParseField.pData[i++];
		if (pMoveItem->Extra == -1) pMoveItem->Extra =0;

		GlobTomb4.BaseMoveItem.TotMove++;
		pMoveItem->Dynamic = IdPlugin;

		break;
	case PARAM_BIG_NUMBERS:
		n = GlobTomb4.TotBigNumbers;
		*pNewId= n;
		TotItems = TotWords -1;

		for (j=0;j< (int) TotItems;j++) {
			GlobTomb4.VetBigNumbers[n++] =
				ParseField.pData[i++];
			LogOnDebug("Added new value (%d) for Parameters=PARAM_BIG_NUMBERS", GlobTomb4.VetBigNumbers[n-1]);
			
		}
		GlobTomb4.TotBigNumbers=n;

		break;
	case PARAM_SHOW_SPRITE:
		n = GlobTomb4.BaseShowSprites.TotShowSprites;
		if (n >= MAX_SHOW_SPRITES) {
			sprintf(BufferLog,"ERROR: too much Parameters=PARAM_SHOW_SPRITE commands in current level section. The max number was %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		
		pShowSprite = &GlobTomb4.BaseShowSprites.VetShowSprites[n];
		// TROVA ID LIBERO:
		pShowSprite->IdShowSprite  = GetNuovoIdParam(GlobTomb4.BaseShowSprites.VetIdShowSprites, MAX_SHOW_SPRITES*10);
		*pNewId = pShowSprite->IdShowSprite;
		LogOnDebug("Created %d=Parameters=PARAM_SHOW_SPRITE script command", pShowSprite->IdShowSprite);

		sprintf(MyBufInterno, "Parameters=PARAM_SHOW_SPRITE, %d", 
			pShowSprite->IdShowSprite);

		if (pShowSprite->IdShowSprite >= MAX_SHOW_SPRITES*10) {
			sprintf(BufferLog, "Too big ID value (%d) for PARAM_SHOW_SPRITE command in current level section",
				pShowSprite->IdShowSprite);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseShowSprites.VetIdShowSprites[pShowSprite->IdShowSprite] =n;
		
		pShowSprite->Flags = ParseField.pData[i++];
		if (pShowSprite->Flags == SCRIPT_IGNORE) pShowSprite->Flags =0;

		pShowSprite->Posizione.left  = ParseField.pData[i++];
		pShowSprite->Posizione.top  = ParseField.pData[i++];
		pShowSprite->Posizione.right = ParseField.pData[i++];
		pShowSprite->Posizione.bottom = ParseField.pData[i++];

		pShowSprite->SlotSprite = ParseField.pData[i++];
		pShowSprite->IndiceSprite = ParseField.pData[i++];
		// se c'e' colore trovarlo adesso
		n = ParseField.pData[i++];

		// trovare coloregb
		if (n== SCRIPT_IGNORE) {
			pShowSprite->Colore=0;
		}else {
			pShowSprite->Colore = GetColoreId(n, 0x808080, false, MyBufInterno);
			DeleteColorRgb(n,false);
		}
		
		pShowSprite->GridX = ParseField.pData[i++];
		pShowSprite->GridY = ParseField.pData[i++];
		pShowSprite->ExtraValue = ParseField.pData[i];
		pShowSprite->Dynamic = IdPlugin;
		GlobTomb4.BaseShowSprites.TotShowSprites++;
		break;
	case PARAM_TRANSFORMER:
		n=GlobTomb4.BaseTransformer.TotTransformer;
		if (n >= MAX_TRANSFORMERS) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_TRANSFORMER command in current level section (%d). The max value is %d",
				n, MAX_TRANSFORMERS);
			InviaLog(BufferLog);
			return false;								

		}



		pTrans = &GlobTomb4.BaseTransformer.VetTransformers[n];

		// TROVA ID LIBERO:
		pTrans->IdTransformer   = GetNuovoIdParam(GlobTomb4.BaseTransformer.VetIdTransformers , MAX_TRANSFORMERS*10);
		*pNewId = pTrans->IdTransformer;

		LogOnDebug("Created %d=Parameters=PARAM_TRANSFORMER", pTrans->IdTransformer );


		if (pTrans->IdTransformer  >= MAX_TRANSFORMERS*10) {
			sprintf(BufferLog, "Too big ID value (%d) for PARAM_TRANSFORMER command in current level section",
				pTrans->IdTransformer );
			InviaLog(BufferLog);
			break;
		}
		GlobTomb4.BaseTransformer.VetIdTransformers[pTrans->IdTransformer] = n;

		pTrans->TriggerGroupSrc = ParseField.pData[i++];
		pTrans->SrcLastAnim = ParseField.pData[i++];
		pTrans->SrcLastFrame = ParseField.pData[i++];
		pTrans->TargetSlot = ParseField.pData[i++];
		pTrans->TriggerGroupTarget = ParseField.pData[i++];
		pTrans->TargetFirstAnim = ParseField.pData[i++];
		pTrans->TargetFirstFrame = ParseField.pData[i++];
		pTrans->Flags = ParseField.pData[i++];
		if (pTrans->Flags == SCRIPT_IGNORE) pTrans->Flags=0;
		pTrans->Extra1 = ParseField.pData[i++];
		pTrans->Extra2 = ParseField.pData[i++];
		pTrans->Dynamic = IdPlugin;
		

		GlobTomb4.BaseTransformer.TotTransformer++;
		break;

	case PARAM_SCALE_ITEM:
		n = GlobTomb4.BaseScaleParam.TotScale;
		if (n >= MAX_SCALE_ITEM) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_SCALE_ITEM command in current level section. The max value is %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		pScale = &GlobTomb4.BaseScaleParam.VetScaleItem[n];
		// TROVA ID LIBERO:
		pScale->IdScale  = GetNuovoIdParam(GlobTomb4.BaseScaleParam.VetIdScale, MAX_SCALE_ITEM*10);
		*pNewId = pScale->IdScale;

		LogOnDebug("Created %d=Parameters=PARAM_SCALE_ITEM", pScale->IdScale);

		if (pScale->IdScale >= MAX_SCALE_ITEM*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_SCALE_ITEM command in current level section",
				pScale->IdScale);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseScaleParam.VetIdScale[pScale->IdScale] = n;
		pScale->ItemIndex = ParseField.pData[i++];
		pScale->Flags = ParseField.pData[i++];
		if (pScale->Flags == SCRIPT_IGNORE) pScale->Flags=0;
		pScale->BeginPerc  = ParseField.pData[i++];
		pScale->FinalPerc  = ParseField.pData[i++];
		pScale->IncPerc = ParseField.pData[i++];
		pScale->Dynamic = IdPlugin;
		GlobTomb4.BaseScaleParam.TotScale ++;
		break;

	case PARAM_SCALE_MOV_ITEM:
		n = GlobTomb4.BaseMovScale.TotMovScale;;
		if (n >= MAX_MOV_SCALE_ITEM) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_MOV_SCALE_ITEM command in current level section. The max value is %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		pMovScale = &GlobTomb4.BaseMovScale.VetMovScaleItem[n];
		// TROVA ID LIBERO:
		pMovScale->IdMovScale   = GetNuovoIdParam(GlobTomb4.BaseMovScale.VetIdMovScale , MAX_MOV_SCALE_ITEM*10);
		*pNewId = pMovScale->IdMovScale;

		LogOnDebug("Created %d=Parameters=PARAM_MOV_SCALE_ITEM", pMovScale->IdMovScale );

		if (pMovScale->IdMovScale >= MAX_MOV_SCALE_ITEM*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_MOV_SCALE_ITEM command in current level section",
				pMovScale->IdMovScale);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseMovScale.VetIdMovScale[n] = pMovScale->IdMovScale;

		pMovScale->Slot  = ParseField.pData[i++];
		pMovScale->FrameDurate  = ParseField.pData[i++];
	
		pMovScale->StartSize = ParseField.pData[i++];
		pMovScale->StartYOffset = ParseField.pData[i++];
		if (pMovScale->StartYOffset == -1) pMovScale->StartYOffset = 0;

		pMovScale->EndSize = ParseField.pData[i++];
		pMovScale->EndYOffset = ParseField.pData[i++];
		if (pMovScale->EndYOffset == -1) pMovScale->EndYOffset =0;

		GlobTomb4.BaseMovScale.TotMovScale++;
		break;

	case PARAM_COLOR_ITEM:
		n = GlobTomb4.BaseColoraItem.TotColoraItem;
		if (n >= MAX_COLORA_ITEM) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_COLOR_ITEM command in current level section: %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		pColora = &GlobTomb4.BaseColoraItem.VetColoraItem[n];
		// TROVA ID LIBERO:
		pColora->IdColItem  = GetNuovoIdParam(GlobTomb4.BaseColoraItem.VetID, MAX_COLORA_ITEM*10);
		LogOnDebug("Created %d=Parameters=PARAM_COLOR_ITEM", pColora->IdColItem);

		*pNewId = pColora->IdColItem;
		if (pColora->IdColItem >= MAX_COLORA_ITEM*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_COLORA_ITEM command in current level section",
				pColora->IdColItem);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseColoraItem.VetID[pColora->IdColItem] = n;
		GlobTomb4.BaseColoraItem.TotColoraItem++;
		
		pColora->Flags = ParseField.pData[i++];
		pColora->ItemIndex = ParseField.pData[i++];
		pColora->IdColore1 = ParseField.pData[i++];
		pColora->IdColore2 = ParseField.pData[i++];
		pColora->Speed = ParseField.pData[i];
		pColora->Dynamic = IdPlugin;

		break;

	case PARAM_CIRCLE:
		n = GlobTomb4.BaseCircles.TotCircles;
		if (n >= MAX_CIRCLES) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_CIRCLE commands in same level section: %d",
				MAX_CIRCLES);
			InviaLog(BufferLog);
			return false;
		}

		pCircle = &GlobTomb4.BaseCircles.VetCircle[n];
		// TROVA ID LIBERO:
		pCircle->IdParam = GetNuovoIdParam(GlobTomb4.BaseCircles.VetIdCircle, MAX_CIRCLES*10);
		LogOnDebug("Created %d=Parameters=PARAM_CIRCLE", pCircle->IdParam);
		*pNewId = pCircle->IdParam;
		if (pCircle->IdParam >= MAX_CIRCLES*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_CIRCLES command in current level section",
				pCircle->IdParam);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseCircles.VetIdCircle[pCircle->IdParam] =n;

		pCircle->CenterX = ParseField.pData[i++];
		pCircle->CenterY = ParseField.pData[i++];
		pCircle->Radius = ParseField.pData[i];
		GlobTomb4.BaseCircles.TotCircles++;
		pCircle->Dynamic = IdPlugin;
		break;

	case PARAM_QUADRILATERAL:
		n = GlobTomb4.BaseQuads.TotQuads;
		if (n >= MAX_QUADS) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_QUADRILATERAL commands in same level section: %d",
				MAX_QUADS);
			InviaLog(BufferLog);
			return false;
		}

		pQuad  = &GlobTomb4.BaseQuads.VetQuads[n];
		// TROVA ID LIBERO:
		pQuad->IdParam = GetNuovoIdParam(GlobTomb4.BaseQuads.VetIdQuads, MAX_QUADS*10);
		LogOnDebug("Created %d=Parameters=PARAM_QUADRILATERAL", pQuad->IdParam);
		*pNewId = pQuad->IdParam;
		if (pQuad->IdParam >= MAX_QUADS*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_QUADS command in current level section",
				pQuad->IdParam);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseQuads.VetIdQuads[pQuad->IdParam] =n;

		pQuad->Tria1.A.x = (float) ParseField.pData[i++];
		pQuad->Tria1.A.y = (float) ParseField.pData[i++];

		pQuad->Tria1.B.x = (float) ParseField.pData[i++];
		pQuad->Tria1.B.y = (float) ParseField.pData[i++];

		pQuad->Tria1.C.x = (float) ParseField.pData[i++];
		pQuad->Tria1.C.y = (float) ParseField.pData[i++];

		pQuad->Tria2.A.x = pQuad->Tria1.C.x;
		pQuad->Tria2.A.y = pQuad->Tria1.C.y;

		pQuad->Tria2.B.x = (float) ParseField.pData[i++];
		pQuad->Tria2.B.y = (float) ParseField.pData[i++];

		pQuad->Tria2.C.x = pQuad->Tria1.A.x;
		pQuad->Tria2.C.y = pQuad->Tria1.A.y;

		GlobTomb4.BaseQuads.TotQuads++;
		pQuad->Dynamic = IdPlugin;
		break;

	case PARAM_TRIANGLE:
		n = GlobTomb4.BaseTriangles.TotTriangles;
		if (n >= MAX_TRIANGLES) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_TRIANGLES commands in same level section: %d",
				MAX_TRIANGLES);
			InviaLog(BufferLog);
			return false;
		}
		
		pTria = &GlobTomb4.BaseTriangles.VetTriangles[n];
		// TROVA ID LIBERO:
		pTria->IdParam  = GetNuovoIdParam(GlobTomb4.BaseTriangles.VetIdTriangles, MAX_TRIANGLES*10);
		*pNewId = pTria->IdParam;
		LogOnDebug("Created %d=Parameters=PARAM_TRIANGLE", pTria->IdParam);

		if (pTria->IdParam >= MAX_TRIANGLES*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_TRIANGLES command in current level section",
				pTria->IdParam);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseTriangles.VetIdTriangles[pTria->IdParam] = n;

		pTria->Tria.A.x = (float) ParseField.pData[i++];
		pTria->Tria.A.y = (float) ParseField.pData[i++];

		pTria->Tria.B.x = (float) ParseField.pData[i++];
		pTria->Tria.B.y = (float) ParseField.pData[i++];

		pTria->Tria.C.x = (float) ParseField.pData[i++];
		pTria->Tria.C.y = (float) ParseField.pData[i++];

		GlobTomb4.BaseTriangles.TotTriangles++;
		pTria->Dynamic = IdPlugin;
		break;



	case PARAM_ROTATE_ITEM:
		n = GlobTomb4.BaseRotateItem.TotRotate;
		if (n >= MAX_ROTATE_PARAM) {
			sprintf(BufferLog,"WARNING: too much Parameters=PARAM_ROTATE_ITEM command in current level section: %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		
		pRotate = &GlobTomb4.BaseRotateItem.VetRotate[n];
		// TROVA ID LIBERO:
		pRotate->IdMove   = GetNuovoIdParam(GlobTomb4.BaseRotateItem.VetID, MAX_ROTATE_PARAM*10);
		*pNewId = pRotate->IdMove;
		if (pRotate->IdMove >= MAX_ROTATE_PARAM*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_ROTATE_PARAM command in current level section",
				pRotate->IdMove);
			InviaLog(BufferLog);
			return false;
		}

		LogOnDebug("Created %d=Parameters=PARAM_ROTATE_ITEM", pRotate->IdMove);
		GlobTomb4.BaseRotateItem.VetID[pRotate->IdMove] = n;
		pRotate->Flags = ParseField.pData[i++];
		if (pRotate->Flags == SCRIPT_IGNORE) 
				pRotate->Flags =0;
		pRotate->ItemIndex = ParseField.pData[i++];
		pRotate->DirezioneH = ParseField.pData[i++];
		pRotate->AngoloH = ParseField.pData[i++];
		pRotate->SpeedH = ParseField.pData[i++];

		pRotate->DirezioneV = ParseField.pData[i++];
		pRotate->AngoloV = ParseField.pData[i++];
		pRotate->SpeedV = ParseField.pData[i++];

		pRotate->SoundMoving= ParseField.pData[i++];
		pRotate->SoundFinal = ParseField.pData[i++];
		GlobTomb4.BaseRotateItem.TotRotate++;
		pRotate->Dynamic = IdPlugin;
		break;
	case PARAM_SET_CAMERA:
		n= GlobTomb4.BaseSetCamera.TotSetCamera;
		if (n >= MAX_SET_CAMERA) {
			sprintf(BufferLog,"WARNING: too much Parameters PARAM_SET_CAMERA command in current level section: %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		pCamera = &GlobTomb4.BaseSetCamera.VetSetCamera[n];
		// TROVA ID LIBERO:
		pCamera->IdCamera    = GetNuovoIdParam(GlobTomb4.BaseSetCamera.VetID, MAX_SET_CAMERA*10);
		LogOnDebug("Created %d=Parameters=PARAM_SET_CAMERA", pCamera->IdCamera);

		*pNewId = pCamera->IdCamera;
		if (pCamera->IdCamera >= MAX_SET_CAMERA*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_SET_CAMERA command in current level section",
				pCamera->IdCamera);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseSetCamera.VetID[pCamera->IdCamera] =n;
		// ora leggere altri parametri dopo l'id
		pCamera->Flags = ParseField.pData[i++];
		if (pCamera->Flags == SCRIPT_IGNORE) 
			pCamera->Flags =0;
		pCamera->Distance = ParseField.pData[i++];

		if (pCamera->Distance == -1) {
			pCamera->Distance = 
				GlobTomb4.pBaseCustomize->CameraCust.ChaseDistance;
		}

		pCamera->VOrient = ParseField.pData[i++];

		if (pCamera->VOrient == -1) {
			pCamera->VOrient =
				GlobTomb4.pBaseCustomize->CameraCust.ChaseHeight;
		}
		pCamera->HOrient = ParseField.pData[i++];
		if (pCamera->HOrient == -1) {
			pCamera->HOrient = 
				GlobTomb4.pBaseCustomize->CameraCust.ChaseHOrient;
		}
		pCamera->Speed = ParseField.pData[i++];
		if (pCamera->Speed == SCRIPT_IGNORE) {
			pCamera->Speed = 
				GlobTomb4.pBaseCustomize->CameraCust.Speed;
		}
		GlobTomb4.BaseSetCamera.TotSetCamera++;
		pCamera->Dynamic = IdPlugin;
		break;


	case PARAM_PRINT_TEXT:
		n = GlobTomb4.BaseParamPrint.TotPrintText;
		if (n >= MAX_PARAM_PRINT_TEXT) {
			sprintf(BufferLog,"WARNING: too much Parameters PARAM_PRINT_TEXT command in current level section: %d",
				n);
			InviaLog(BufferLog);
			return false;
		}
		pPrint = &GlobTomb4.BaseParamPrint.VetPrint[n];
		// TROVA ID LIBERO:
		pPrint->IdPrint = GetNuovoIdParam(GlobTomb4.BaseParamPrint.VetID, MAX_PARAM_PRINT_TEXT*10);
		LogOnDebug("Created %d=Parameters=PARAM_PRINT_TEXT", pPrint->IdPrint);

		*pNewId = pPrint->IdPrint;
		if (pPrint->IdPrint >= MAX_PARAM_PRINT_TEXT*10) {
			sprintf(BufferLog, "Too big ID value (%d) for MAX_PARAM_PRINT_TEXT command in current level section",
				pPrint->IdPrint);
			InviaLog(BufferLog);
			return false;
		}
		GlobTomb4.BaseParamPrint.VetID[pPrint->IdPrint] =n;
		// ora leggere tre word successive
		// come fossero quelli di comando TextFormat
		ScansioneTextFormat(&pPrint->Formatting, 
					&ParseField.pData[i]);
		i += 3;
		pPrint->DurateTime = ParseField.pData[i++];
		pPrint->OrgX = ParseField.pData[i++];
		pPrint->OrgY = ParseField.pData[i];

		pPrint->Dynamic = IdPlugin;
		GlobTomb4.BaseParamPrint.TotPrintText++;
		break;
	case PARAM_SWAP_ANIMATIONS:
		n=GlobTomb4.pBaseSwapAnim->TotSwapAnim;
		if (n >= MAX_SWAP_ANIM) {
			sprintf(BufferLog, "WARNING: too much Parameters=PARAM_SWAP_ANIMATIONS  commands in current level section: %d",
				PARAM_SWAP_ANIMATIONS);
			InviaLog(BufferLog);
			return false;
		}

		pSwap = &GlobTomb4.pBaseSwapAnim->VetSwapAnim[n];
		// TROVA ID LIBERO:
		pSwap->Id  = GetNuovoIdParam(GlobTomb4.pBaseSwapAnim->VetId, MAX_SWAP_ANIM*10);
		LogOnDebug("Created %d=Parameters=PARAM_SWAP_ANIMATIONS", pSwap->Id);
		*pNewId = pSwap->Id ;
		if (pSwap->Id >= MAX_SWAP_ANIM*10) {
			sprintf(BufferLog, "WARNING: too big ID value (%d) for Parameters=PARAM_SWAP_ANIMATIONS  command in current level section",
				pSwap->Id);
			InviaLog(BufferLog);
			return false;

		}

		GlobTomb4.pBaseSwapAnim->VetId[pSwap->Id] = n;

		pSwap->FirstSourceAnim = ParseField.pData[i++];
		pSwap->FirstTargetAnim = ParseField.pData[i++];
		pSwap->NumberOfAim = ParseField.pData[i++];
		pSwap->Dynamic = IdPlugin;
		GlobTomb4.pBaseSwapAnim->TotSwapAnim++;
		break;

	case PARAM_RECT:
		n=GlobTomb4.BaseParamMyRects.TotMyRect;
		if (n >= MAX_MY_RECT) {
			sprintf(BufferLog, "WARNING: too much Parameters=PARAM_RECT  commands in current level section: %d",
				MAX_MY_RECT);
			InviaLog(BufferLog);
			return false;
		}

		pMyRect = &GlobTomb4.BaseParamMyRects.VetMyRect[n];
		// TROVA ID LIBERO:
		pMyRect->Id   = GetNuovoIdParam(GlobTomb4.BaseParamMyRects.VetId, MAX_MY_RECT*10);\
		*pNewId = pMyRect->Id ;
		if (pMyRect->Id  >= MAX_MY_RECT*10) {
			sprintf(BufferLog, "WARNING: Too big ID value (%d) for Parameters=PARAM_RECT command in current level section",
				pMyRect->Id );
			InviaLog(BufferLog);
			return false;
		}
		LogOnDebug("Created %d=Parameters=PARAM_RECT", pMyRect->Id);
		GlobTomb4.BaseParamMyRects.VetId[pMyRect->Id] = n;

		pMyRect->OrgX = ParseField.pData[i++];
		pMyRect->OrgY = ParseField.pData[i++];
		pMyRect->SizeX = ParseField.pData[i++];
		pMyRect->SizeY = ParseField.pData[i++];
		pMyRect->ForeColor = ParseField.pData[i++];
		pMyRect->BackColor = ParseField.pData[i];
		pMyRect->Dynamic = IdPlugin;
		GlobTomb4.BaseParamMyRects.TotMyRect++;
		break;



	case PARAM_LIGHTNING:
		n = GlobTomb4.BaseParamLightning.TotParamLgtn;

		if (n >= MAX_PARAM_LIGHTNING) {
			sprintf(BufferLog, "WARNING: too much Parameters PARAM_LIGHTNING commands in current level section: %d",
				n);
			return false;
		}
		pLightning = &GlobTomb4.BaseParamLightning.VetRecordLgtn[n];
		// TROVA ID LIBERO:
		pLightning->IdParamLgtn   = GetNuovoIdParam(GlobTomb4.BaseParamLightning.VetId, MAX_PARAM_LIGHTNING*10);
		*pNewId = pLightning->IdParamLgtn ;
		if (pLightning->IdParamLgtn >= MAX_PARAM_LIGHTNING*10) {
			sprintf(BufferLog, "Too big ID value (%d) for PARAM_LIGHTNING command in current level section",
				pLightning->IdParamLgtn);
			InviaLog(BufferLog);
			return false;
		}

		LogOnDebug("Created %d=Paramaters=PARAM_LIGHTNING", pLightning->IdParamLgtn);

		GlobTomb4.BaseParamLightning.VetId[pLightning->IdParamLgtn] = n;
		// ora leggere i parametri
		pLightning->Flags = ParseField.pData[i++];
		if (pLightning->Flags == -1) pLightning->Flags =0;

		pLightning->IndexSource = ParseField.pData[i++];

		pLightning->IndexTarget = ParseField.pData[i++];
		// trovare 
		Index= ParseField.pData[i++];
		pLightning->Color = TrovaColoreRGB(Index);
		DeleteColorRgb(Index, false);

		pLightning->Intensity = ParseField.pData[i++];
		if (pLightning->Intensity == -1) pLightning->Intensity = 0x20;

		pLightning->SoundEffect = ParseField.pData[i++];

		pLightning->Size = ParseField.pData[i++];
		if (pLightning->Size == -1) pLightning->Size = 0x18;


		pLightning->ParticleDurate = ParseField.pData[i++];
		if (pLightning->ParticleDurate == -1) pLightning->ParticleDurate= 0x18;

		pLightning->IntervalMask = ParseField.pData[i++];	
		if (pLightning->IntervalMask < 1) pLightning->IntervalMask=1;


		pLightning->Arg1 = ParseField.pData[i++];
		if (pLightning->Arg1 == -1) pLightning->Arg1 = 1;

		pLightning->Arg3 = ParseField.pData[i];
		if (pLightning->Arg3 == -1) pLightning->Arg3 = 5;

		pLightning->Dynamic = IdPlugin;
		GlobTomb4.BaseParamLightning.TotParamLgtn++;
		break;

		

	default:


		sprintf(BufferLog,"ERROR: found a PARAM_ (%d) not supported",
			n);
		InviaLog(BufferLog);
		return false;
		


		
		

	}

	return true;


}
// crea un nuovo oggetto e restituisce l'indice
short CreateNewMoveable(WORD Slot, DWORD CordX, int CordY, DWORD CordZ, int Room)
{
	StrItemTr4 *pItem;
	StrSlot *pSlot;
	short Indice;
	void *pFloor;
	short TempRoom;

	TYPE_CreateItem CreateItem = (TYPE_CreateItem) 0x453840;
	TYPE_InitialiseItem InitialiseItem = (TYPE_InitialiseItem) 0x453890;
	TYPE_InitialiseCreature  InitialiseCreature = ( TYPE_InitialiseCreature) 0x43F330;

	Indice = CreateItem();
	if (Indice == -1) return -1;

	pItem = &GlobTomb4.pAdr->pVetItems[Indice];

	pItem->SlotID = Slot;
	pItem->CordX = CordX;
	pItem->CordY = CordY;
	pItem->CordZ = CordZ;
	pItem->Room = Room;
//	pItem->Objectbuttons = 0x3e00;
	TempRoom = Room;
	pFloor=GetFloor(CordX,CordY-128,CordZ, &TempRoom);

	pItem->HeightFloor = GetHeight(pFloor,CordX, CordY,CordZ);
	pItem->Room = TempRoom;

	pSlot= &GlobTomb4.pAdr->pVetSlot[Slot];
	pItem->Health = pSlot->Vitality;
	InitialiseItem(Indice);

	AddActiveItem(Indice);

	return Indice;

}

void DeleteNewMoveable(short Index)
{	
	StrItemTr4 *pItem;

	RemoveActiveItem(Index);
	KillItem(Index);

	pItem = &GlobTomb4.pAdr->pVetItems[Index];
	pItem->FlagsMain=0;
	pItem->SlotID=0;
	if ((Index+1) == GlobTomb4.pAdr->TotItemsMax) {
		GlobTomb4.pAdr->TotItemsMax--;
	}

	

}

// aggiungere un record AI
// se OcbValue == -1, assegna un valore ocb diverso da tutti quelli presenti e lo restituisce

int AddAIRecord(WORD Slot, DWORD CordX, int CordY, DWORD CordZ, WORD Room, int OcbValue, short Facing)
{
	WORD OcbNow;
	StrAIDataTr4 *pRecordAI;
	int TotAI;
	int i;
	bool TestUguale;


	TotAI = *GlobTomb4.pAdr->pTotAIData;

	if (TotAI >= BaseGlobMisc.TotMaxAIRecords) {
		sprintf(BufferLog, "ERROR: reached max number of AI records to add. Impossible adding AI record with slot=%d and room=%d",
			Slot, Room);
		InviaLog(BufferLog);
		return -1;

	}

	if (OcbValue == -1) {
		// cercare un valore ocb non ancora usato
		OcbNow=0;
		
		do {
			TestUguale=false;
			pRecordAI = GlobTomb4.pAdr->pVetAIData;
			OcbNow++;
			for (i=0;i<TotAI;i++) {
				if (pRecordAI->Ocb == OcbNow) {
					TestUguale=true;
					break;
				}
				pRecordAI++;
			}
		}while (TestUguale);

		
	}else {
		// e' stato fornito un valore ocb valido: usare quello
		OcbNow = (WORD) OcbValue;

	}

	pRecordAI = &GlobTomb4.pAdr->pVetAIData[TotAI];

	pRecordAI->Buttons=0;
	pRecordAI->CordX = CordX;
	pRecordAI->CordY = CordY;
	pRecordAI->CordZ = CordZ;
	pRecordAI->Ocb = OcbNow;
	pRecordAI->Orientation = Facing;
	pRecordAI->RoomIndex = Room;
	pRecordAI->SlotAI = Slot;
	LogOnDebug("Created AI Record of %d type with OCB=%d", pRecordAI->SlotAI, pRecordAI->Ocb);

	*GlobTomb4.pAdr->pTotAIData = TotAI+1;
	return OcbNow;


}
// rimuove il record AI con valore OcbValue
void DeleteAIRecord(WORD Slot, WORD OcbValue, bool TestAlls)
{
	int i;
	StrAIDataTr4 *pRecordAI;
	int TotAI;

	TotAI = *GlobTomb4.pAdr->pTotAIData;



	for (i=TotAI-1;i>= 0;i--) {
		pRecordAI = &GlobTomb4.pAdr->pVetAIData[i];
		if (pRecordAI->SlotAI == Slot && pRecordAI->Ocb == OcbValue) {
			// eliminare questo record
			LogOnDebug("Deleted Record AI of %d type with OCB=%d", pRecordAI->SlotAI, pRecordAI->Ocb);
			TotAI--;
			*pRecordAI = GlobTomb4.pAdr->pVetAIData[TotAI];
			
			if (TestAlls == false) break;
		}
	}

	*GlobTomb4.pAdr->pTotAIData = TotAI;

}
// creare uno script command ColorRgb e restituisce l'ID
int CreateColorRbg(int IdDynamic, int Rosso, int Verde, int Blu)
{
	int Tot;
	StrColorRGB *pColor;
	int IdColor;
	int i;


	Tot = GlobTomb4.BaseColoriRGB.TotColori;
	if (Tot >= MAX_COLORI_RGB) {
		sprintf(BufferLog,"WARNING: Too much ColorRGB script command in this level section. Max is %d",
			Tot);
		InviaLog(BufferLog);
		return -1;
	}
	for (i=1;i<MAX_COLORI_RGB*10;i++) {
		if (GlobTomb4.BaseColoriRGB.VetID[i] == -1) break;
	}

	IdColor = i;

	LogOnDebug("Created %d=ColorRgb=%d, %d, %d", IdColor, Rosso, Verde, Blu);

	pColor = &GlobTomb4.BaseColoriRGB.VetColori[Tot];
	pColor->IdColor = IdColor;
	pColor->Dynamic = IdDynamic;

	// salvare id
	GlobTomb4.BaseColoriRGB.VetID[pColor->IdColor] = Tot;

	pColor->Colore = Blu;
	pColor->Colore |= (Verde << 8);
	pColor->Colore |= (Rosso << 16);

	GlobTomb4.BaseColoriRGB.TotColori++;
	return IdColor;

}

// nota: cancella il comando script solo se Dynamic e' diverso da zero
void DeleteParamCommand(int ParamType, int IdParam, bool TestSempre, bool TestSottoCommandi)
{
	bool TestLast;
	int OldIndex;
	short *pTot;
	short n;
	int i;
	StrColoraItem *pColora;
	StrSpeechActor *pSpeech;
	WORD ComandoNow;
	int IdCommand;

	TestLast=false;

	switch (ParamType) {
	case PARAM_MOVE_ITEM:
		pTot= (short *) &GlobTomb4.BaseMoveItem.TotMove;
		OldIndex = GlobTomb4.BaseMoveItem.VetID[IdParam];
		if (OldIndex==-1) return;

		if (TestSempre == false && GlobTomb4.BaseMoveItem.VetMove[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_MOVE_ITEM with ID %d script command", IdParam);
		GlobTomb4.BaseMoveItem.VetID[IdParam] = -1;
		GlobTomb4.BaseMoveItem.VetMove[OldIndex].Dynamic=0;
		break;

	case PARAM_ROTATE_ITEM:
		pTot = (short *)  &GlobTomb4.BaseRotateItem.TotRotate;
		OldIndex = GlobTomb4.BaseRotateItem.VetID[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseRotateItem.VetRotate[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_ROTATE_ITEM with ID %d script command", IdParam);
		GlobTomb4.BaseRotateItem.VetID[IdParam] = -1;
		GlobTomb4.BaseRotateItem.VetRotate[OldIndex].Dynamic=0;
		break;

	case PARAM_COLOR_ITEM:
		pTot = (short *)  &GlobTomb4.BaseColoraItem.TotColoraItem;
		OldIndex = GlobTomb4.BaseColoraItem.VetID[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseColoraItem.VetColoraItem[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_COLOR_ITEM with ID %d script command", IdParam);
		GlobTomb4.BaseColoraItem.VetID[IdParam]=-1;
		GlobTomb4.BaseColoraItem.VetColoraItem[OldIndex].Dynamic=0;
		if (TestSottoCommandi==false) break;
		// cancellare anche colorrgb collegati
		pColora= &GlobTomb4.BaseColoraItem.VetColoraItem[OldIndex];

		if (pColora->IdColore1 != -1) {
			DeleteColorRgb(pColora->IdColore1, false);
		}

		if (pColora->IdColore2 != -1) {
			DeleteColorRgb(pColora->IdColore2, false);
		}

		break;

	case PARAM_PRINT_TEXT:
		pTot = (short *)  &GlobTomb4.BaseParamPrint.TotPrintText;
		OldIndex = GlobTomb4.BaseParamPrint.VetID[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre == false && GlobTomb4.BaseParamPrint.VetPrint[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_PRINT_TEXT with ID %d script command", IdParam);
		GlobTomb4.BaseParamPrint.VetID[IdParam] = -1;
		GlobTomb4.BaseParamPrint.VetPrint[OldIndex].Dynamic=0;
		break;

	case PARAM_SET_CAMERA:
		pTot= (short *) &GlobTomb4.BaseSetCamera.TotSetCamera;
		OldIndex = GlobTomb4.BaseSetCamera.VetID[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre == false && GlobTomb4.BaseSetCamera.VetSetCamera[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_SET_CAMERA with ID %d script command", IdParam);
		GlobTomb4.BaseSetCamera.VetID[IdParam] = -1;
		GlobTomb4.BaseSetCamera.VetSetCamera[OldIndex].Dynamic=0;
		break;

	case PARAM_TRANSFORMER:
		pTot = (short *) &GlobTomb4.BaseTransformer.TotTransformer;
		OldIndex = GlobTomb4.BaseTransformer.VetIdTransformers[IdParam];
		if (OldIndex == -1) return;
		if (TestSempre == false && GlobTomb4.BaseTransformer.VetTransformers[OldIndex].Dynamic  ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_TRANSFORMER with ID %d script command", IdParam);
		GlobTomb4.BaseTransformer.VetIdTransformers[IdParam] = -1;
		GlobTomb4.BaseTransformer.VetTransformers[OldIndex].Dynamic =0;
		break;

	case PARAM_SCALE_ITEM:
		pTot = (short *) &GlobTomb4.BaseScaleParam.TotScale;
		OldIndex = GlobTomb4.BaseScaleParam.VetIdScale[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre == false && GlobTomb4.BaseScaleParam.VetScaleItem[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_SCALE_ITEM with ID %d script command", IdParam);
		GlobTomb4.BaseScaleParam.VetIdScale[IdParam] = -1;
		GlobTomb4.BaseScaleParam.VetScaleItem[OldIndex].Dynamic=0;
		break;

	case PARAM_SCALE_MOV_ITEM:
		pTot = (short *) &GlobTomb4.BaseMovScale.TotMovScale;
		OldIndex = GlobTomb4.BaseMovScale.VetIdMovScale[IdParam];
		if (OldIndex == -1) return;
		if (TestSempre== false && GlobTomb4.BaseMovScale.VetMovScaleItem[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_MOV_SCALE_ITEM with ID %d script command", IdParam);
		GlobTomb4.BaseMovScale.VetIdMovScale[IdParam] = -1;
		GlobTomb4.BaseMovScale.VetMovScaleItem[OldIndex].Dynamic =0;
		break;

	case PARAM_SHOW_SPRITE:
		pTot = (short *) &GlobTomb4.BaseShowSprites.TotShowSprites;
		OldIndex = GlobTomb4.BaseShowSprites.VetIdShowSprites[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseShowSprites.VetShowSprites[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_SHOW_SPRITE with ID %d script command", IdParam);
		GlobTomb4.BaseShowSprites.VetIdShowSprites[IdParam] = -1;
		GlobTomb4.BaseShowSprites.VetShowSprites[OldIndex].Dynamic=0;
		// nota: il colorrgb viene eliminato subito dopo aver letto il colorergb quando viene creato il param_show_sprite
		break;

	case PARAM_TRIANGLE:
		pTot= (short *) &GlobTomb4.BaseTriangles.TotTriangles;
		OldIndex = GlobTomb4.BaseTriangles.VetIdTriangles[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseTriangles.VetTriangles[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_TRIANGLE with ID %d script command", IdParam);
		GlobTomb4.BaseTriangles.VetIdTriangles[IdParam]=-1;
		GlobTomb4.BaseTriangles.VetTriangles[OldIndex].Dynamic=0;
		break;
	case PARAM_QUADRILATERAL:
		pTot = (short *) &GlobTomb4.BaseQuads.TotQuads;
		OldIndex = GlobTomb4.BaseQuads.VetIdQuads[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseQuads.VetQuads[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_QUADRILATERAL with ID %d script command", IdParam);
		GlobTomb4.BaseQuads.VetIdQuads[IdParam] = -1;
		GlobTomb4.BaseQuads.VetQuads[OldIndex].Dynamic=0;
		break;
	case PARAM_BIG_NUMBERS:
		// gestione particolare. se IdParam (che dovrebbe essere l'ultimo numero allocato)
		// e' uguale all'ultimo numero della serie, allora viene ridotto il numero
		// di big numbers
		i = GlobTomb4.TotBigNumbers-1;
		if (i<0) return;
		if (GlobTomb4.VetBigNumbers[i] == IdParam) {
			LogOnDebug("Deleted last PARAM_BIG_NUMBERS value: %d", IdParam);
			GlobTomb4.TotBigNumbers=i;
		}
		return;

	case PARAM_CIRCLE:
		pTot = (short *)  &GlobTomb4.BaseCircles.TotCircles;
		OldIndex = GlobTomb4.BaseCircles.VetIdCircle[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseCircles.VetCircle[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_CIRCLE with ID %d script command", IdParam);
		GlobTomb4.BaseCircles.VetIdCircle[IdParam] = -1;
		GlobTomb4.BaseCircles.VetCircle[OldIndex].Dynamic =0;
		break;

	case PARAM_LIGHTNING:
		pTot = (short*) &GlobTomb4.BaseParamLightning.TotParamLgtn;
		OldIndex = GlobTomb4.BaseParamLightning.VetId[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseParamLightning.VetRecordLgtn[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_LIGHTNING with ID %d script command", IdParam);
		GlobTomb4.BaseParamLightning.VetId[IdParam]=-1;
		GlobTomb4.BaseParamLightning.VetRecordLgtn[OldIndex].Dynamic=0;

		break;


	case PARAM_RECT:
		pTot= (short*) &GlobTomb4.BaseParamMyRects.TotMyRect;
		OldIndex = GlobTomb4.BaseParamMyRects.VetId[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.BaseParamMyRects.VetMyRect[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_RECT with ID %d script command", IdParam);
		GlobTomb4.BaseParamMyRects.VetId[IdParam] = -1;
		GlobTomb4.BaseParamMyRects.VetMyRect[OldIndex].Dynamic=0;
		break;
	case PARAM_SWAP_ANIMATIONS:
		pTot =  (short*) &GlobTomb4.pBaseSwapAnim->TotSwapAnim;
		OldIndex = GlobTomb4.pBaseSwapAnim->VetId[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre == false && GlobTomb4.pBaseSwapAnim->VetSwapAnim[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_SWAP_ANIMATIONS with ID %d script command", IdParam);
		GlobTomb4.pBaseSwapAnim->VetId[IdParam] =-1;
		GlobTomb4.pBaseSwapAnim->VetSwapAnim[OldIndex].Dynamic =0;
		break;
		
	case PARAM_ACTOR_SPEECH:
		pTot=  (short*)  &GlobTomb4.pBaseSpeechActor->TotSpeechActor;
		OldIndex = GlobTomb4.pBaseSpeechActor->VetId[IdParam];
		if (OldIndex==-1) return;
		if (TestSempre==false && GlobTomb4.pBaseSpeechActor->VetSpeechActor[OldIndex].Dynamic ==0) return;
		LogOnDebug("Deleted Parameters=PARAM_ACTOR_SPEECH with ID %d script command", IdParam);
		GlobTomb4.pBaseSpeechActor->VetId[IdParam]=-1;
		GlobTomb4.pBaseSpeechActor->VetSpeechActor[OldIndex].Dynamic =0;
		pSpeech = &GlobTomb4.pBaseSpeechActor->VetSpeechActor[OldIndex];

		// se nei comandi c'e' il comando SPC_PERFORM_TG
		for (i=0;i<pSpeech->TotCommands;i++) {
			ComandoNow=pSpeech->VetCommands[i];

			if ((ComandoNow & SPC_MASK_COMMANDS) == SPC_PERFORM_TG) {
				IdCommand= ComandoNow & SPC_MASK_SINGLE;
				DeleteTriggerGroup(IdCommand, false);
			}
		}

		break;
	default:
		LogOnDebug("WARNING: attempt to delete an unsupported PARAM_ type %d", ParamType);
		return;

	}

	n= *pTot;
	n--;
	if (OldIndex == n) *pTot = n;


}

void DeleteTriggerGroup(int IdTriggerGroup, bool TestSempre)
{
	int OldIndex;
	int Tot;

	OldIndex = GlobTomb4.pBaseTriggerGroups->VetID[IdTriggerGroup];
	if (OldIndex==-1) return;
	if (TestSempre==false && GlobTomb4.pBaseTriggerGroups->VetTriggerGroups[OldIndex].Dynamic == 0) return;

	GlobTomb4.pBaseTriggerGroups->VetTriggerGroups[OldIndex].Dynamic=0;

	Tot = GlobTomb4.pBaseTriggerGroups->TotTriggerGroups-1;

	GlobTomb4.pBaseTriggerGroups->VetID[IdTriggerGroup]=-1;

	if (OldIndex == Tot) {
		GlobTomb4.pBaseTriggerGroups->TotTriggerGroups = Tot;		
	}

}


void DeleteColorRgb(int IdColorRgb, bool TestSempre)
{
	int OldIndex;
	int Tot;

	Tot=GlobTomb4.BaseColoriRGB.TotColori-1;

	OldIndex = GlobTomb4.BaseColoriRGB.VetID[IdColorRgb];
	if (OldIndex==-1) return;
	if (TestSempre==false && GlobTomb4.BaseColoriRGB.VetColori[OldIndex].Dynamic == 0) return;

	LogOnDebug("Deleted ColorRgb= with ID %d script command", IdColorRgb);
	GlobTomb4.BaseColoriRGB.VetColori[OldIndex].Dynamic=0;
	GlobTomb4.BaseColoriRGB.VetID[IdColorRgb]=-1;

	if (OldIndex == Tot) {
		GlobTomb4.BaseColoriRGB.TotColori=Tot;
	}

}


void DeleteMultEnvCondition(int IdMultEnv, bool TestSempre, bool TestSottoComandi)
{
	int OldIndex;
	int Tot;
	StrScriptEnvMultCondition *pEnv;
	int i;
	int IdTrigger;


	Tot =GlobTomb4.pBaseEnvConditions->TotScriptEnvCondition-1;
	OldIndex = GlobTomb4.pBaseEnvConditions->VetID[IdMultEnv];
	if (OldIndex==-1) return;
	if (GlobTomb4.pBaseEnvConditions->VetScriptEndCondition[OldIndex].Dynamic == 0) return;
	pEnv = &GlobTomb4.pBaseEnvConditions->VetScriptEndCondition[OldIndex];

	LogOnDebug("Deleted MultEnvCondition= with ID %d script command", IdMultEnv);
	GlobTomb4.pBaseEnvConditions->VetScriptEndCondition[OldIndex].Dynamic=0;


	GlobTomb4.pBaseEnvConditions->VetID[IdMultEnv] = -1;
	if (OldIndex == Tot) {
		GlobTomb4.pBaseEnvConditions->TotScriptEnvCondition = Tot;
	}
	if (TestSottoComandi==true) {
		// vedere se ci sono ENV_CONDITION_TRIGGER_GROUP da eliminare
		for (i=0;i<pEnv->TotEnvCondition;i++) {
			switch (pEnv->VetEnvCondition[i].EnvCondition) {
			case ENV_CONDITION_TRIGGER_GROUP:
				// provare ad eliminare triggergroup
				IdTrigger=pEnv->VetEnvCondition[i].DistanceEnv;
				DeleteTriggerGroup(IdTrigger,false);
				break;
			case ENV_ITEM_TEST_POSITION:
				// eliminare testposition
				IdTrigger=pEnv->VetEnvCondition[i].DistanceEnv;
				DeleteTestPosition(IdTrigger, false);
				break;

			}
		}
	}
}

void DeleteTestPosition(int IdTest, bool TestSempre)
{
	int OldIndex;
	int Tot;

	Tot = GlobTomb4.BaseTestPosition.TotTestPositions-1;
	OldIndex = GlobTomb4.BaseTestPosition.VetId[IdTest];
	if (OldIndex==-1) return;
	if (TestSempre == false && GlobTomb4.BaseTestPosition.VetTestPosition[OldIndex].Dynamic == 0) return;
	GlobTomb4.BaseTestPosition.VetTestPosition[OldIndex].Dynamic=0;
	GlobTomb4.BaseTestPosition.VetId[IdTest]=-1;

	if (OldIndex == Tot) {
		GlobTomb4.BaseTestPosition.TotTestPositions = Tot;
	}

	

}


// nota: se testsempre=true allora ignora il campo Dynamic e anche fosse =0 eliminera' comunque il comando
void DeleteAddEffect(int IdEffect, bool TestSempre)
{
	int OldIndex;
	int Tot;

	Tot = GlobTomb4.pBaseEffects->TotEffects -1;
	OldIndex = GlobTomb4.pBaseEffects->VetID[IdEffect];
	if (OldIndex==-1) return;
	if (TestSempre==false && GlobTomb4.pBaseEffects->VetEffects[OldIndex].Dynamic == 0) return;

	LogOnDebug("Deleted AddEffect= with ID %d script command", IdEffect);

	GlobTomb4.pBaseEffects->VetEffects[OldIndex].Dynamic=0;
	GlobTomb4.pBaseEffects->VetID[IdEffect] = -1;

	if (OldIndex == Tot) {
		GlobTomb4.pBaseEffects->TotEffects = Tot;
	}


}

// disabilita il salvataggio di savegame per TotFrames
// nota: se gia' c'era attivo un disable di questo tipo viene aumentato il numero di frame 
// a quello attuale, se maggiore, oppure viene ignorato il comando, se e' quello attuale ad essere minore
void DisableGameSaving(WORD TotFrames)
{
	StrProgressiveAction *pAzione;
	int i;

	pAzione= &GlobTomb4.VetProgressiveActions[0];

	for (i=0;i<GlobTomb4.TotProgressiveActions;i++) {

		if (pAzione->ActionType == AZ_BLOCK_DISABLE_KEY &&
			pAzione->VetArg[0] == CMD_SAVE_GAME) {
			// c'era gia': vedere sua durata
			if (TotFrames > pAzione->Arg1) {
				pAzione->Arg1=TotFrames;
			}
			return;
		}

		pAzione++;
	}

	// non c'era azione. Se pero' c'era gia' come tasti da bloccare quello di cmd_save_game allora
	// era attivo per sempre e quindi ingorare
	if (GlobTomb4.KeysToStop & CMD_SAVE_GAME) return;

	GlobTomb4.KeysToStop |= CMD_SAVE_GAME;
	// non c'era azione e non era attivo il blocco: creare ora azione
	i = CreaNuovaAzioneProgressiva();
	pAzione = &GlobTomb4.VetProgressiveActions[i];
	pAzione->ActionType = AZ_BLOCK_DISABLE_KEY;
	pAzione->Arg1 = TotFrames;
	pAzione->VetArg[0]= CMD_SAVE_GAME;



}
// if oggetto e' un pushable (che sta per essere spostato) disattiva collisioni
// poi quando sara' finito il movimento bisognera' ripristinarle con CheckForEndMovePushable()
bool CheckForStartMovePushable(int ItemIndex)
{
	int Altezza;
	StrItemTr4 *pOggetto;
	bool TestReturn;

	TestReturn=false;
	pOggetto = &GlobTomb4.pAdr->pVetItems[ItemIndex];

	// e' alla prima esecuzione:
	// se e' un pushable fare analisi particolare
	if (pOggetto->SlotID >= 156 &&
		pOggetto->SlotID <= 160) {

		if (pOggetto->OcbCode & OPU_ENABLE_NG) {

			// e' un pushable. ora bisogna inibire collisione
			// usando flag 0x08
			// nel caso pero' ci sia gia' collisione attiva
			// allora toglierla
			if (GlobTomb4.BasePushables.VetCollisionePushable[ItemIndex] & CP_COLLISION) {
				// togliere collisione
				Altezza = (pOggetto->OcbCode & OPU_MASCHERA_ALTEZZA) << 8;
				if (Altezza) {
					AlterFloorHeight(pOggetto, Altezza);
					GlobTomb4.BasePushables.VetCollisionePushable[ItemIndex] &= 
														~CP_COLLISION;
					TestReturn=true;
				}
			}
			GlobTomb4.BasePushables.VetCollisionePushable[ItemIndex] |= CP_MOVING_ACTION;
		}
	}
	return TestReturn;
}
// da chiamare DOPO aver finito lo spostamento di un moveable.
// verifica se si tratta di un pushable con collisioni, che verranno ripristinate
// nota: all'inizio del movimento deve essere gia' stata chiamata funzione CheckForStartMovePushable()
void CheckForEndMovePushable(int ItemIndex)
{
	StrItemTr4 *pOggetto;
	
	pOggetto = &GlobTomb4.pAdr->pVetItems[ItemIndex];

	// se oggetto era pushable ripristinare tutto
	if (pOggetto->SlotID >= 156 &&
		pOggetto->SlotID <= 160) {
		// rimuovere flag di movimento
		// e questo comportera la creazioe di collisione
		// nella procedura di pushable
		GlobTomb4.BasePushables.VetCollisionePushable[ItemIndex] &= ~CP_MOVING_ACTION;
		
	}

}
// fa esplodere il veicolo e uccide lara
void ExplosionOnVehicle(StrItemTr4 *pVeicolo)
{
	StrItemTr4 *pLara;
	static TYPE_ExplodingDeath2 ExplodingDeath2 = (TYPE_ExplodingDeath2) 0x43A030;
	static TYPE_JeepExplode JeepExplode = (TYPE_JeepExplode) 0x464550;

	pLara=GlobTomb4.pAdr->pLara;

	ExplodingDeath2(*GlobTomb4.pAdr->pLaraIndex, 0xff, 0x100);
	
	pLara->Health=0;
	pLara->Objectbuttons |= 0x100;

	JeepExplode(pVeicolo);

}
// chiamata solo da plugin
// legge una variabile trng
// TrigCode puo' essere codice preso da trigger, oppure, se e' presente il flag SCRIPT_CODE, come script
// code, ossia il codice di panello reference di ng_script relativo alle variabili

int ReadNumVariable(int TrigCode)
{
	int ScriptCode;

	if (TrigCode & SCRIPT_CODE) {
		// e' gia' scriptcode
		ScriptCode = TrigCode & MASK_SCRIPT_CODE;
	}else {
		// e' necessario convertire codice sintetico trigger, in codice script esteso
		ScriptCode = ConvertiCodVar(TrigCode);
	}

	return LeggiVariabile(ScriptCode);
}
void WriteNumVariable(int TrigCode, int Valore)
{
	WORD ScriptCode;

	if (TrigCode & SCRIPT_CODE) {
		// e' gia' scriptcode
		ScriptCode = TrigCode & MASK_SCRIPT_CODE;
	}else {
		// e' necessario convertire codice sintetico trigger, in codice script esteso
		ScriptCode = ConvertiCodVar(TrigCode);
	}
	ScriviVariabile(ScriptCode, Valore);

}

char * ReadTextVariable(int TrigCode)
{
	WORD ScriptCode;

	if (TrigCode & SCRIPT_CODE) {
		// e' gia' scriptcode
		ScriptCode = TrigCode & MASK_SCRIPT_CODE;
	}else {
		// e' necessario convertire codice sintetico trigger, in codice script esteso
		ScriptCode = ConvertiCodText(TrigCode);
	}	
	return LeggiVariabileTesto(ScriptCode);
}

void WriteTextVariable(int TrigCode, char *pString)
{
	WORD ScriptCode;

	if (TrigCode & SCRIPT_CODE) {
		// e' gia' scriptcode
		ScriptCode = TrigCode & MASK_SCRIPT_CODE;
	}else {
		// e' necessario convertire codice sintetico trigger, in codice script esteso
		ScriptCode = ConvertiCodText(TrigCode);
	}	
	ScriveVariabileTesto(ScriptCode, pString);
}

int ReadMemVariable(int TrigCode)
{
	if ((TrigCode & MEM_MASK_ALL_TYPES) == 0) {
		sprintf(BufferLog, "ERROR in ReadMemVariable() function: the TrigCode (0x%X) has no MEMT_ flag", TrigCode);
		InviaLog(BufferLog);
		return 0;
	}

	return LeggiVariabileMem(TrigCode);
}

void WriteMemVariable(int TrigCode, int Valore)
{
	if ((TrigCode & MEM_MASK_ALL_TYPES) == 0) {
		sprintf(BufferLog, "ERROR in WriteMemVariable() function: the TrigCode (0x%X) has no MEMT_ flag", TrigCode);
		InviaLog(BufferLog);
		return;
	}
	ScriviVariabileMem(TrigCode, Valore);

}
// esegue servizi richiesi dai plugin (questa e' quella che nel plugin si chiama "Trng.Service"
int WINAPI Servo(DWORD ID_Plugin, DWORD SRV_Value, va_list pArgs)
{

	int *pLastFlipEffect = (int *) 0x4ACBFC;
	int Arg1,Arg2,Arg3,Arg4;
	int Arg5,Arg6,Arg7, Arg8;
	char *pText;
	bool Test1, Test2; 
	bool TestBool;
	int i;
	HDC hDC;
	char *pChar;
	WORD VetTemp[500];
	WORD TotTemp;
	int IdParam;
	int Numero;
	int Indice;
	RECT *pRect;
	StrScriptTrigger MyTrigger;
	StrTriggerGroup *pMyTGroup;
	StrScriptTrigger *pSTrigger;
	StrAnimScript  *pMyAnim;
	StrAnimObjScript *pMyObjAnim;
	StrScriptEnvMultCondition *pScriptEnv;
	StrTestPositionCmd *pTest;
	StrBoxCollisione *pDist;
	StrBoxOrienting *pOrient;
	StrAddEffect  *pEffect;
	Tipo_CollGridTr4 *pFloor;
	StrCustomItem *pCustom;
	StrItemTr4 *pItem;
	int IdTG;
	int IdENV;
	int IdTP;
	int IdEFF;
	StrTestPositionCmd *pPosition;
	int IdDynamic;
	StrShowImage *pBase;
	StrRecordImage *pRecord;
	StrPos3d *pPos;
	char *pTesto;
	StrWindowsFont *pFont;

	IdDynamic=ID_Plugin;

	// iniziare da va_arg() perche' il va_start e' gia' stato
	// fatto dalla chiamante
	// esempio:
	//	i = va_arg( pArgs, int);
	switch (SRV_Value) {
	case SRV_F_IsSfxPlaying:
		Arg1 = va_arg(pArgs, WORD);

		return IsSfxPlaying(Arg1);

	case SRV_F_ManagePickups:
		// WORD GestionePickups(WORD Slot, int Operazione, short Valore)
		Arg1 = va_arg(pArgs, WORD);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, short);

		return GestionePickups(Arg1, Arg2, Arg3);

	case SRV_F_ReadNumVariable:
		// int ReadNumVariable(int TrigCode)
		Arg1 =va_arg(pArgs, int);

		return ReadNumVariable(Arg1);
	case SRV_F_WriteNumVariable:
		// void WriteNumVariable(int TrigCode, int Valore);
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);

		WriteNumVariable(Arg1, Arg2);
		return 0;
	case SRV_F_ReadTextVariable:
		// char * ReadTextVariable(int TrigCode);
		Arg1 = va_arg(pArgs, int);
		pChar = ReadTextVariable(Arg1);
		return (int) pChar;
	case SRV_F_WriteTextVariable:
		// void WriteTextVariable(int TrigCode, char *pString);
		Arg1 = va_arg(pArgs, int);
		pChar = va_arg(pArgs, char *);
		WriteTextVariable(Arg1, pChar);
		return 0;
	case SRV_F_ReadMemVariable:
		// int ReadMemVariable(int TrigCode);
		Arg1 = va_arg(pArgs, int);
		return ReadMemVariable(Arg1);
	case SRV_F_WriteMemVariable:
		// void WriteMemVariable(int TrigCode, int Valore)
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);	
		WriteMemVariable(Arg1, Arg2);
		return 0;

	case SRV_F_ReadInputBox:
		// int ReadInputBox(HDC hDC, RECT *pRect, char Buffer[], DWORD MaxChars, WORD RIB_Flags, int SfxSound)
		hDC =va_arg(pArgs, HDC);
		pRect = va_arg(pArgs, RECT *);
		pTesto = va_arg(pArgs, char*);
		Arg2 = va_arg(pArgs, DWORD);
		TotTemp = va_arg(pArgs, WORD);
		Arg3 = va_arg(pArgs, int);
		return ReadInputBox(hDC, pRect, pTesto, Arg2, TotTemp, Arg3);

	case SRV_F_FreeWindowsFont:
		// void LiberaWindowsFont(StrWindowsFont *pFont)
		pFont = va_arg(pArgs, StrWindowsFont *);
		LiberaWindowsFont(pFont);
		return 0;

	case SRV_F_CreateWindowsFont:
		// HFONT CreaWindowsFont(StrWindowsFont *pFont, bool TestNoResize)
		pFont = va_arg(pArgs, StrWindowsFont *);
		TestBool = va_arg(pArgs, bool);
		return (int) CreaWindowsFont(pFont, TestBool);


	case SRV_F_ResumeAudioTrack:
		// void RiprendiAudio(void)
		RiprendiAudio();
		return 0;

	case SRV_F_SuspendAudioTrack:
		// void SospendiAudio(int NuovoCd, int NuovoTestLoop)
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);

		SospendiAudio(Arg1, Arg2);
		return 0;

	case SRV_F_ReadDxInput:
		// DWORD LeggiDirectInput(void)
		return LeggiDirectInput();

	case SRV_F_PrintText:
		// void StampaStringa(int CordX, int CordY, char *pTesto, WORD FlagMicro, int Colore, WORD FlagsAlign)
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		pTesto = va_arg(pArgs, char *);
		Arg3 = va_arg(pArgs, WORD);
		Arg4 = va_arg(pArgs, int);
		Arg5 = va_arg(pArgs, WORD);

		StampaStringa(Arg1, Arg2, pTesto, Arg3, Arg4, Arg5);
		return 0;

	case SRV_F_DrawObject2D:
		// void ShowOggettoSlot(WORD Slot, int CordX, int CordY, WORD OrientX, 
		//					 WORD OrientY, WORD OrientZ, int Distanza)
		Arg1 = va_arg(pArgs, WORD);  // slot
		Arg2 = va_arg(pArgs, int);   // cordx
		Arg3 = va_arg(pArgs, int);   // cordy
		Arg4 = va_arg(pArgs, WORD);  // OrientX
		Arg5 = va_arg(pArgs, WORD);  // OrientY
		Arg6 = va_arg(pArgs, WORD);  // OrientZ
		Arg7 = va_arg(pArgs, int); // Distanza
		ShowOggettoSlot(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		return 0;

	case SRV_F_DrawMesh3D:
		// void DisegnaMeshAbs(StrPos3d *pPos, int IndiceMesh)
		pPos = va_arg(pArgs, StrPos3d *);
		Arg1= va_arg(pArgs, int);
		DisegnaMeshAbs(pPos, Arg1);
		return 0;


	case SRV_F_DrawSprite2D:
		// void MostraSprite2d(RECT *pRect, WORD Slot, int IndiceSprite, BYTE Opacita, COLORREF Colore);
		pRect = va_arg(pArgs, RECT *);
		TotTemp = va_arg(pArgs, WORD);
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, BYTE);
		Arg3 = va_arg(pArgs, COLORREF);
		MostraSprite2d(pRect, TotTemp, Arg1, Arg2, Arg3);
		return 0;

	case SRV_F_FreeTombHdc:
		// void LiberaHdcTomb(StrShowImage *pBase, bool TestMantieniTemp);
		pBase = va_arg(pArgs, StrShowImage *);
		Test1 = va_arg(pArgs, bool);

		LiberaHdcTomb(pBase, Test1);
		return 0;

	case SRV_F_AllocateTombHdc:
		// bool AllocaHdcTomb(StrShowImage *pBase, bool TestHdcTemp, bool TestWriteHdc);
		pBase = va_arg(pArgs, StrShowImage *);
		Test1 = va_arg(pArgs, bool);
		Test2 = va_arg(pArgs, bool);

		return AllocaHdcTomb(pBase, Test1, Test2);

	case SRV_F_FreeImage:
		// void LiberaImmagine(StrRecordImage *pRecord);
		pRecord = va_arg(pArgs, StrRecordImage *);
		LiberaImmagine(pRecord);
		return 0;

	case SRV_F_AllocateImage:
		// (int NImage, StrRecordImage *pRecord, int ForceSizeX, int ForceSizeY);
		Arg1 = va_arg(pArgs, int);
		pRecord = va_arg(pArgs, StrRecordImage *);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);
		return AllocaImmagine(Arg1, pRecord, Arg2, Arg3);


	case SRV_F_ConvertMicroUnits:
		pRect = va_arg(pArgs, RECT *);
		ConvertiSpriteMicroUnits(pRect);
		return 0;

	case SRV_SetReservedDataZone:
		// (IdPlugin, StartOffset, NumberOfBytes)
		Arg1 = va_arg(pArgs, DWORD);
		Arg2 = va_arg(pArgs, DWORD);


		return SetReservedZone(ID_Plugin, Arg1, Arg2);

	case SRV_F_ExplosionOnVehicle:
		pItem = va_arg(pArgs, StrItemTr4 *);
		ExplosionOnVehicle(pItem);
		return 0;

	case SRV_F_CheckForStartMovePushable:
		Arg1= va_arg(pArgs, int);
		if (CheckForStartMovePushable(Arg1)==true) return 1;
		return 0;

	case SRV_F_CheckForEndMovePushable:
		Arg1= va_arg(pArgs, int);
		CheckForEndMovePushable(Arg1);
		return 0;
	case SRV_F_AggiungiItemMosso:
		Arg1= va_arg(pArgs, int);
		AggiungiItemMosso(Arg1);
		return 0;
		
	case SRV_DeleteTriggerGroup:
		Arg1= va_arg(pArgs, int);
		DeleteTriggerGroup(Arg1,true);
		return 0;
	case SRV_F_DisableSaving:
		Arg1= va_arg(pArgs, int);
		DisableGameSaving(Arg1);
		return 0;
	
	case SRV_DeleteColorRgb:
		Arg1= va_arg(pArgs, int);
		DeleteColorRgb(Arg1,true);
		return 0;
	case SRV_DeleteMultEnvCondition:
		Arg1= va_arg(pArgs, int);
		DeleteMultEnvCondition(Arg1,true,false);
		return 0;
	case SRV_DeleteTestPosition:
		Arg1= va_arg(pArgs, int);
		DeleteTestPosition(Arg1,true);
		return 0;

	case SRV_DeleteAddEffect:
		Arg1= va_arg(pArgs, int);
		DeleteAddEffect(Arg1,true);
		return 0;

	case SRV_DeleteParamCommand:
		// void DeleteParamCommand(int ParamType, int IdParam)
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);
		DeleteParamCommand(Arg1, Arg2,true, false);
		return 0;

	case SRV_CREATE_COLOR_RGB_COMMAND:
		//  TestDynamic Reg  Green Blue
		TestBool = va_arg(pArgs, bool);
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);
		Arg3= va_arg(pArgs, int);
		if (TestBool == false) IdDynamic=0;
		
		return CreateColorRbg(IdDynamic, Arg1, Arg2, Arg3);

	case SRV_F_CreateNewMoveable:
		//                            1            2              3      4              5      
		// short CreateNewMoveable(WORD Slot, DWORD CordX, int CordY, DWORD CordZ, int Room)
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);
		Arg3= va_arg(pArgs, int);
		Arg4= va_arg(pArgs, int);
		Arg5 = va_arg(pArgs, int);;

		
		return CreateNewMoveable(Arg1, Arg2, Arg3, Arg4, Arg5);

	case SRV_F_DeleteNewMoveable:
		// void DeleteNewMoveable(short Index)
		Arg1= va_arg(pArgs, int);


		DeleteNewMoveable(Arg1);
		return 0;

	case SRV_F_CreateAIRecord:
		//        1            2        3              4         5           6               7
		// (WORD Slot, DWORD CordX, int CordY, DWORD CordZ, WORD Room, int OcbValue, short Facing)
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);
		Arg3= va_arg(pArgs, int);
		Arg4= va_arg(pArgs, int);
		Arg5= va_arg(pArgs, int);
		Arg6= va_arg(pArgs, int);
		Arg7= va_arg(pArgs, int);
		
		return AddAIRecord(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

	case SRV_F_DeleteAIRecord:
		// DeleteAIRecord(WORD Slot, WORD OcbValue, bool TestAlls)
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);

		TestBool = va_arg(pArgs, bool);
		DeleteAIRecord((WORD) Arg1, (WORD) Arg2, TestBool);
		return 0;

	case SRV_F_EseguiAnimNemico:
		// (StrItemTr4 *pOggetto, WORD IndiceAnim, short NextStateId)
		pItem = va_arg(pArgs, StrItemTr4 *);
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);


		EseguiAnimNemico(pItem, Arg1, Arg2);
		return 0;

	case SRV_F_IsCollisioneConItems:
		// bool IsCollisioneConItems(StrItemTr4 *pOggetto, bool TestIgnoraLara, 
		//				  DWORD X, int Y, DWORD Z, short Room, 
		//				  int MaxDistance, int MinLargerSize, 
		//				  int MinHeight, int Tollerance)

		pItem = va_arg(pArgs,  StrItemTr4 *);
		TestBool = va_arg(pArgs, bool);
		Arg1= va_arg(pArgs, int);  //  X
		Arg2= va_arg(pArgs, int);  //  Y
		Arg3= va_arg(pArgs, int);  //  Z
		Arg4= va_arg(pArgs, int);  //  room
		Arg5= va_arg(pArgs, int);  // MaxDistance
		Arg6= va_arg(pArgs, int);  // MinLargerSize
		Arg7= va_arg(pArgs, int);  // MinHeight
		Arg8= va_arg(pArgs, int);  // Tollerance

		if (IsCollisioneConItems(pItem, TestBool, Arg1, Arg2, Arg3, Arg4,Arg5, Arg6, Arg7, Arg8)==false) return 0;
		return true;

	case SRV_F_CollideItemConCustom:
		pItem = va_arg(pArgs, StrItemTr4 *);
		pCustom = va_arg(pArgs, StrCustomItem *);
		Arg3=va_arg(pArgs, int);

		if (CollideItemConCustom(pItem, pCustom, Arg3)==false) return 0;
		return true;

	case SRV_F_VerificaTestPosition:
		pPosition = va_arg(pArgs, StrTestPositionCmd *);
		Arg2 = va_arg(pArgs, int);
		if (VerificaTestPosition(pPosition, Arg2) == false) return 0;
		return 1;

	case SRV_F_IsBoxSettore:
		pFloor= va_arg(pArgs, Tipo_CollGridTr4 *);

		if (IsBoxSettore(pFloor)==true) return 1;
		return 0;

	case SRV_F_ProporzioneDistanza:
		//       (int Incremento, int Distanza);
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);

		return ProporzioneDistanza(Arg1, Arg2);




	case SRV_F_EseguiTriggerGroup:
		Arg1 = va_arg(pArgs, int);
		return EseguiTriggerGroup(Arg1);


	case SRV_F_DetectedGlobalTriggerEvent:
		Arg1= va_arg(pArgs, int);
		Arg2= va_arg(pArgs, int);
		TestBool= va_arg(pArgs, bool);
		return (int) VerificaSingleGlobalTrigger(Arg1, Arg2, TestBool);
	case SRV_F_InviaErroreLog:
		pText= va_arg(pArgs, char *);
		Arg2 = va_arg(pArgs, int);
		TestBool = va_arg(pArgs, bool);
		InviaErroreLog(pText, Arg2, TestBool);
		return 0;

	case SRV_F_InviaLog:
		pText = va_arg(pArgs, char *);
		InviaLog(pText);
		return 0;

	case SRV_CREATE_ADDEFFECT:
		//  TestDynamic, EffectType (ADD_), FlagsEffect (FADD_), JointType (JOINT_), 
		//      DispX, DispY, DispZ, DurateEmit, DuratePause, Extra param array

		i = GlobTomb4.pBaseEffects->TotEffects;
		if (i >= MAX_ADD_EFFECTS) return -1;

		Indice = i;
		for (i =0 ;i<MAX_ADD_EFFECTS;i++) {
			if (GlobTomb4.pBaseEffects->VetID[i] == -1) break;
		}
		IdEFF = i;
		GlobTomb4.pBaseEffects->VetID[IdEFF] = Indice;
		pEffect = &GlobTomb4.pBaseEffects->VetEffects[Indice];
		
		pEffect->Id = IdEFF;

		LogOnDebug("Created %d=AddEffect= command", IdEFF);

		TestBool = va_arg(pArgs, bool);
		if (TestBool==false) IdDynamic=0;
		pEffect->Dynamic = IdDynamic;

		// EffectType (ADD_)
		Arg1 = va_arg(pArgs, int);
		pEffect->Type = Arg1;

		// FlagsEffect (FADD_)
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pEffect->Flags = Arg1;

		// JointType (JOINT_)
		Arg1 = va_arg(pArgs, int);
		pEffect->Joint = Arg1;

		// DispX
		Arg1 = va_arg(pArgs, int);
		pEffect->DispX = Arg1;

		// DispY
		Arg1 = va_arg(pArgs, int);
		pEffect->DispY = Arg1;

		// DispX
		Arg1 = va_arg(pArgs, int);
		pEffect->DispZ = Arg1;

		// DurateEmit
		Arg1 = va_arg(pArgs, int);
		pEffect->DurateEmit = Arg1;

		// DuratePause
		Arg1 = va_arg(pArgs, int);
		pEffect->DuratePause = Arg1;

		// Extra param array
		i=0;
		Arg1 = va_arg(pArgs, int);
		while (Arg1 != END_LIST && i < MAX_EXTRA_EFFECT) {
			pEffect->VetExtra[i++] = Arg1;
			Arg1 = va_arg(pArgs, int);
		}
		if (Arg1 != END_LIST) {
			InviaLog("ERROR: creating dynamic AddEffect= command, missing END_LIST value");
			GlobTomb4.pBaseEffects->VetID[IdEFF] = -1;
			return -1;
		}

		GlobTomb4.pBaseEffects->TotEffects++;
		return IdEFF;



		
	case SRV_CREATE_TESTPOSITION:
		// Flags (TPOS_...), Slot Moveable, 
		//         XDistanceMin, XDistanceMax, 
		//              YDistanceMin, YDistanceMax, 
		//                   ZDistanceMin, ZDistanceMax,  
		//                        HOrientDiffMin, HOrientDiffMax, 
		//                             VOrientDiffMin, VOrientDiffMax, 
		//                                   ROrientDiffMin, ROrientDiffMax

		i = GlobTomb4.BaseTestPosition.TotTestPositions;
		if ( i >= MAX_TEST_POSITION) return -1;
		Indice= i;
		for (i=0;i<MAX_TEST_POSITION;i++) {
			if (GlobTomb4.BaseTestPosition.VetId[i] == -1) break;
		}
		IdTP=i;
		LogOnDebug("Created %d=TestPosition= script command", IdTP);

		GlobTomb4.BaseTestPosition.VetId[i] = Indice;
	
		pTest = &GlobTomb4.BaseTestPosition.VetTestPosition[Indice];
		GlobTomb4.BaseTestPosition.TotTestPositions++;

		pTest->IdTestPosition = IdTP;

		TestBool = va_arg(pArgs, bool);
		if (TestBool==false) IdDynamic=0;

		pTest->Dynamic = IdDynamic;

		// Flags (TPOS_...)
		Arg1 = va_arg(pArgs, int);
		pTest->Flags = Arg1;

		// Slot Moveable
		Arg1 = va_arg(pArgs, int);
		pTest->Slot = Arg1;

		pDist = &pTest->DatiPosition.Distance;
		// XDistanceMin
		Arg1 = va_arg(pArgs, int);
		pDist->MinX = Arg1;

		// XDistanceMax
		Arg1 = va_arg(pArgs, int);
		pDist->MaxX = Arg1;

		// YDistanceMin
		Arg1 = va_arg(pArgs, int);
		pDist->MinY  = Arg1;

		// YDistanceMax
		Arg1 = va_arg(pArgs, int);
		pDist->MaxY  = Arg1;
		
		// ZDistanceMin
		Arg1 = va_arg(pArgs, int);
		pDist->MinZ  = Arg1;

		// ZDistanceMax
		Arg1 = va_arg(pArgs, int);
		pDist->MaxZ  = Arg1;

		pOrient = &pTest->DatiPosition.Orienting;

		// HOrientDiffMin
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientHMin = Arg1;

		// HOrientDiffMax
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientHMax = Arg1;


		// VOrientDiffMin
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientVMin = Arg1;

		// VOrientDiffMax
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientVMax = Arg1;

		// ROrientDiffMin
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientRMin = Arg1;

		// ROrientDiffMax
		Arg1 = va_arg(pArgs, int);
		pOrient->OrientRMax = Arg1;

		return IdTP;

	case SRV_CREATE_MULTENVCONDITION:
		// ENV_ condition, DistanceForEnv, Extra field, ... fino a END_LIST
		i = GlobTomb4.pBaseEnvConditions->TotScriptEnvCondition;
		if (i >= MAX_ENV_SCRIPT_CONDITIONS) return -1;
		Indice=i;
		for (i=0;i< MAX_ENV_SCRIPT_CONDITIONS;i++) {
			if (GlobTomb4.pBaseEnvConditions->VetID[i] == -1) break;
		}
		IdENV = i;
		LogOnDebug("Created %d=MultEnvCondition= script command", IdENV);

		GlobTomb4.pBaseEnvConditions->VetID[IdENV] = Indice;
		pScriptEnv = &GlobTomb4.pBaseEnvConditions->VetScriptEndCondition[Indice];

		pScriptEnv->IdEnvScript = IdENV;
		pScriptEnv->TotEnvCondition=0;
		i=0;
		TestBool = va_arg(pArgs, bool);
		if (TestBool==false) IdDynamic=0;

		pScriptEnv->Dynamic = IdDynamic;

		// ENV_ condition
		Arg1 = va_arg(pArgs, int);
		while (Arg1 != END_LIST && i < MAX_ENV_CONDITIONS) {
			// DistanceForEnv
			Arg2 = va_arg(pArgs, int);
			// Extra field
			Arg3 = va_arg(pArgs, int);
			
			pScriptEnv->VetEnvCondition[i].EnvCondition = Arg1;
			pScriptEnv->VetEnvCondition[i].DistanceEnv = Arg2;
			pScriptEnv->VetEnvCondition[i].Extra = Arg3;
			i++;
			Arg1 = va_arg(pArgs, int);
		}
		if (Arg1 != END_LIST) {
			GlobTomb4.pBaseEnvConditions->VetID[IdENV]=-1;
			InviaLog("ERROR: creating dynamic MultEnvCondition command, missing END_LIST value");
			return -1;
		}
		pScriptEnv->TotEnvCondition = i;
		GlobTomb4.pBaseEnvConditions->TotScriptEnvCondition++;

		return IdENV;

	case SRV_CREATE_ANIMATIONSLOT:
		// Slot, ActionType (AXT_...), AnimIndex, 
		//     Key1, Key2, FAN_ flags, ENV_ Environment, 
		//         Distance For Env_, Extra, StateId (STATE_...) or (-)AnimationIndex array  (...)
		i = GlobTomb4.BaseAnimObj.TotAnimObj;
		if (i >= MAX_ANIM_OBJ_SCRIPT) return -1;

		Indice=i;

		pMyObjAnim = &GlobTomb4.BaseAnimObj.VetAnimObj[Indice];
		// Slot
		Arg1 = va_arg(pArgs, int);
		pMyObjAnim->Slot = Arg1;

		LogOnDebug("Created AnimationSlot=%d", Arg1);

		// ActionType (AXT_...)
		Arg1 = va_arg(pArgs, int);
		pMyObjAnim->ActionType = Arg1;

		pMyAnim=&pMyObjAnim->Animation;

		// AnimIndex
		Arg1 = va_arg(pArgs, int);
		pMyAnim->AnimIndex = Arg1;

		// KEY1_ 
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Key1 = Arg1;


		// KEY2_ 
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Key2 = Arg1;

		// FAN_ flags
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Flags = Arg1;

		// ENV_ Environment
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Environment = Arg1;

		// Distance for Env
		Arg1 = va_arg(pArgs, int);
		pMyAnim->DistanceEnv = Arg1;

		// Extra
		Arg1 = va_arg(pArgs, int);
		pMyAnim->Extra = Arg1;

		// ora lista variabile
		pMyAnim->TotStateId=0;

		Numero = va_arg(pArgs, int);
		i=0;
		while (Numero != END_LIST && i < 32) {
			pMyAnim->VetStateId[i++]=Numero;
			Numero = va_arg(pArgs, int);
		}
		if (Numero != END_LIST) {
			InviaLog("ERROR: creating dynamic AnimationSlot= command, missing the END_LIST value");
			return -1;
		}

		pMyAnim->TotStateId = i;

		GlobTomb4.BaseAnimObj.TotAnimObj++;
		return Indice;

	case SRV_CREATE_ANIMATION:

		// AnimIndex, KEY1_ , KEY2_ , FAN_ flags, 
		//		ENV_ Environment, Distance for Env, Extra, 
		//         StateId (STATE_...) or (-)AnimationIndex array  (...)
		i = GlobTomb4.pBaseAnimations->TotAnimazioni;
		if (i >= MAX_ANIM_SCRIPT) return -1;
		Indice = i;
		
		pMyAnim = &GlobTomb4.pBaseAnimations->VetAnimations[Indice];

		// AnimIndex
		Arg1 = va_arg(pArgs, int);
		pMyAnim->AnimIndex = Arg1;

		LogOnDebug("Created Animation=%d", Arg1);
		// KEY1_ 
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Key1 = Arg1;


		// KEY2_ 
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Key2 = Arg1;

		// FAN_ flags
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Flags = Arg1;

		// ENV_ Environment
		Arg1 = va_arg(pArgs, int);
		if (Arg1 == -1) Arg1=0;
		pMyAnim->Environment = Arg1;

		// Distance for Env
		Arg1 = va_arg(pArgs, int);
		pMyAnim->DistanceEnv = Arg1;

		// Extra
		Arg1 = va_arg(pArgs, int);
		pMyAnim->Extra = Arg1;

		// ora lista variabile
		pMyAnim->TotStateId=0;

		Numero = va_arg(pArgs, int);
		i=0;
		while (Numero != END_LIST && i < 32) {
			pMyAnim->VetStateId[i++]=Numero;
			Numero = va_arg(pArgs, int);
		}
		if (Numero != END_LIST) {
			InviaLog("ERROR: creating dynamic Animation= command, missing END_LIST value");
			return -1;
		}

		pMyAnim->TotStateId = i;

		GlobTomb4.pBaseAnimations->TotAnimazioni++;
		return Indice;
					
	case SRV_CREATE_TRIGGERGROUP:
		// sequence of triple arg, {FirstWord, SecondWord, ThirdWord}
		// qui bisogna copiare tutta la lista in una variabile temporanea
		Indice = GlobTomb4.pBaseTriggerGroups->TotTriggerGroups;
		if (Indice >= MAX_TRIGGER_GROUPS) return -1;
		// e poi salvarla temporaneamente in un triggergroup libero
		// trovare un id libero
		for (i=0;i<MAX_TRIGGER_GROUPS*10;i++) {
			if (GlobTomb4.pBaseTriggerGroups->VetID[i] == -1) break;
		}
		if (i == MAX_TRIGGER_GROUPS*10) return -1;



		GlobTomb4.pBaseTriggerGroups->VetID[i] = Indice;
		IdTG = i;

		LogOnDebug("Created %d=TriggerGroup= script command", IdTG);

		pMyTGroup = &GlobTomb4.pBaseTriggerGroups->VetTriggerGroups[Indice];

		pMyTGroup->IdGroup = IdTG;
		pMyTGroup->TotTriggers =0;

		i=0;
		TestBool = va_arg(pArgs, bool);
		if (TestBool==false) IdDynamic=0;
		pMyTGroup->Dynamic= IdDynamic;

		Arg1 = va_arg(pArgs, int);
		while (Arg1 != END_LIST  && i < MAX_TRIGGERS_IN_THE_GROUP) {

			Arg2 = va_arg(pArgs, int);
			Arg3 = va_arg(pArgs, int);
			
			pSTrigger = &pMyTGroup->VetTriggers[i++];

			pSTrigger->Flags  = Arg1;
			pSTrigger->Object = Arg2;
			pSTrigger->Timer = Arg3;

			Arg1 = va_arg(pArgs, int);


		}
		if (i == MAX_TRIGGERS_IN_THE_GROUP) {
			GlobTomb4.pBaseTriggerGroups->VetID[IdTG]=-1;
			InviaLog("ERROR: creating dynamic TriggerGroup= command, missing END_LIST value");
			return -1;
		}
		pMyTGroup->TotTriggers = i;


		GlobTomb4.pBaseTriggerGroups->TotTriggerGroups++;
		return IdTG;

	case SRV_CREATE_PARAM_COMMAND:
		// Arg1 PARAM_ constant , niente id, solo i parametri l'id lo devo trovare io uno libero
		i=0;
		TestBool = va_arg(pArgs, bool);
		if (TestBool==false) IdDynamic=0;
		// prendere il PARAM_...
		Arg1 = va_arg(pArgs, int);
		if (Arg1== END_LIST) return -1;
		// ora prendere tutti i valori e metterli in vettore temp
		Arg2= va_arg(pArgs, int);

		while (Arg2 != END_LIST && i < 500) {

			VetTemp[i++] = Arg2;
			Arg2 = va_arg(pArgs, int);

		}
		if (Arg2 != END_LIST) {
			sprintf(BufferLog, "ERROR: creating Parameters=%d . Missing END_LIST value", Arg1);
			InviaLog(BufferLog);
			return -1;
		}
		TotTemp=i;
		if (CreateNewParamCommand((WORD) IdDynamic, Arg1, TotTemp, VetTemp, &IdParam)==false) return -1;
		return IdParam;

	case SRV_PERFORM_EXPORTED_TRIGGER:
		// Arg1=PluginId  Arg2= Flags / Arg3=Object / Arg4=Timer  
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);
		Arg4 = va_arg(pArgs, int);
		// l'id (in Arg1) e' l'indice a pvetplugin ma gli exported trigger usando sempre l'id di script
		// per ora lo devo convertire
		if (Arg1 > 0) {
			Arg1= MyGlobPrivate.DataBase.pVetPlugins[Arg1].PluginIdScript;
		}
		MyTrigger.PluginId= Arg1;
		MyTrigger.Flags = Arg2;
		MyTrigger.Object = Arg3;
		MyTrigger.Timer = Arg4;


		return EseguiScriptTrigger(&MyTrigger, -1);

	case SRV_PERFORM_FLIPEFFECT:
		// Arg1=PluginId    Arg2=FlipNumber / Arg3= <&> Timer 
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);


		EsecuzioneFlipeffect(Arg1, Arg2, Arg3, SCANF_DIRECT_CALL);
		
		return 0;

	case SRV_PERFORM_ACTION:
		// (WORD PluginID, WORD AzioneTimer, int ItemIndex, WORD Flags)
		// Arg1=PluginId Arg2=ActionNumber / Arg3= (Tomb) item index / Arg4=Timer
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);
		Arg4 = va_arg(pArgs, int);

		Arg2 |= (Arg4 << 8);

		EsecuzioneActionTrigger(Arg1, Arg2, Arg3, SCANF_DIRECT_CALL);
		return 0;
	case SRV_PERFORM_CONDITION:
		//  Arg1=PluginId  / Arg2=ConditionNumber / Arg3 = Object to trigger <#> / Arg4 = (E) Extra
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);
		Arg4 = va_arg(pArgs, int);
		return EseguiCondizione(Arg1, Arg2, Arg3, Arg4, &Test1, &Test2, &Numero, SCANF_DIRECT_CALL);

	case SRV_PERFORM_TRIGGERGROUP_ID:
		// Arg1=IfOfTriggerGroup
		Arg1 = va_arg(pArgs, int);
		if (Arg1 < 0) return -1;
		i  = GlobTomb4.pBaseTriggerGroups->VetID[Arg1];
		if (i == -1) return -1;
		GlobTomb4.pMexLastTGCaller = "From plugin";
		return EseguiTriggerGroup(Arg1);

	case SRV_F_TestEnvCondition:
		//bool TestEnvCondition( WORD EnvCondizione, WORD FlagCondition, short DistanzaEnv, WORD Extra)
		
		Arg1 = va_arg(pArgs, int);
		Arg2 = va_arg(pArgs, int);
		Arg3 = va_arg(pArgs, int);
		Arg4 = va_arg(pArgs, int);
		
		if (TestEnvCondition(Arg1, Arg2, Arg3, Arg4)==true) return 1;
		return 0;
	}
	
	return -1;
}

// questa e' una mia funzione interna per accedere ais servizi dei plugin da trng
int Service(DWORD SRV_Type, ...)
{
	va_list pArgs;
	
	va_start(pArgs, SRV_Type);
	
	

	return Servo(0, SRV_Type, pArgs);
}

// imposta in struttura  posizionata in 0x4A6E70
// i dati trng e le funzioni da dare ai vari plugin
// nota: non viene imposta l'id, che andra' messo un attimo prima
// di collegare la libreria specifica

void ImpostaTrngInfo(DWORD Id_Plugin)
{
	StrTrngInfos *pInfo = (StrTrngInfos*) 0x4A6E70;
	StrRecPluginScript  *pRecScript;
	char *pNomePlugin;
	DWORD MpsFlags;

	memset(pInfo, 0, sizeof(StrTrngInfos));

	pNomePlugin = MyGlobPrivate.DataBase.pVetPlugins[Id_Plugin].Nome;
	pInfo->pMyPluginName = pNomePlugin;

	// ora vedere se c'e' un comando plugin= nello script per questo plugin
	
	MpsFlags=0;
	pRecScript= GetPluginScript(MyGlobPrivate.DataBase.pVetPlugins[Id_Plugin].PluginIdScript);
	if (pRecScript) {
		MpsFlags= pRecScript->MainPluginSettings;					
	}

	pInfo->IdMyPlugin = Id_Plugin;
	pInfo->pGlobTomb4 = &GlobTomb4;
	pInfo->RequireCallBack = &RequireCallBack;
	pInfo->SetNewPatch = &SetNewPatch;
	pInfo->Service = &Servo;
	pInfo->AdrTomb4Patcher = 0;
	pInfo->pDirectCallBack = NULL;
	pInfo->TestDebugMode =0;
	pInfo->pAdrDllPatcher = NULL;
	pInfo->MainPluginFlags = MpsFlags;	


}
// aggiunge dati per controllo zona di memoria STATICA ossia senza
// zona cuscinetto
void AddCheckMemory(DWORD OffsetEndMem, DWORD SizeMemory, char Name[])
{
	DWORD Numero;

	strcpy(VetCheckMemory[TotCheckMemory].Nome ,Name);
	VetCheckMemory[TotCheckMemory].pEndMem = (DWORD *) OffsetEndMem;
	VetCheckMemory[TotCheckMemory].SizeMemoria = SizeMemory;
	VetCheckMemory[TotCheckMemory].TestDinamica =false;
	VetCheckMemory[TotCheckMemory].PercentualeUsata =-1;
	VetCheckMemory[TotCheckMemory].SizeUsata =0;

	// ora calcolare qual'e' il numero di totdword da controllare (a ritroso)
	// per coprire l'ultimo 15 % della zona di memoria
	Numero = SizeMemory * 15;
	Numero = Numero / 100;
	// ora dividerlo per 4
	Numero = Numero / 4;
	VetCheckMemory[TotCheckMemory].TotDword = Numero;
	TotCheckMemory++;
}




// funziona usata per aggiungere un recordo in VetCheckMemory
// usata solo quando memoria e' dinamica

void AddCheckDMemory(DWORD OffsetEndMem, int TotDword, DWORD SizeMemory, char Name[])
{
	strcpy(VetCheckMemory[TotCheckMemory].Nome ,Name);
	VetCheckMemory[TotCheckMemory].pEndMem = (DWORD *) OffsetEndMem;
	VetCheckMemory[TotCheckMemory].TotDword = TotDword;
	VetCheckMemory[TotCheckMemory].SizeMemoria = SizeMemory;
	VetCheckMemory[TotCheckMemory].TestDinamica = true;
	VetCheckMemory[TotCheckMemory].PercentualeUsata =-1;
	VetCheckMemory[TotCheckMemory].SizeUsata =0;
	TotCheckMemory++;
}

// salva in:
// int TotAllocata=0;
// void * VetAllocate[MAX_ALLOCA];
void AddAllocata(void *pMem)
{
	int i;

	i = TotAllocata;

	if (i >= MAX_ALLOCA) return;

	VetAllocate[i]=pMem;
	TotAllocata++;

}

float PercentualeMemoriaOccupata(int IndiceCheck, DWORD OffsetAlterato)
{
	float SizeRimasta;
	float SizeTotale;
	float Percentuale;

	DWORD PuntoFinale;
	PuntoFinale = (DWORD) VetCheckMemory[IndiceCheck].pEndMem;
	SizeRimasta= (float) ( PuntoFinale - OffsetAlterato);
	if (SizeRimasta <= 0) SizeRimasta=0;

	SizeTotale = (float) VetCheckMemory[IndiceCheck].SizeMemoria;

	Percentuale = (float) (SizeRimasta * 100.0)  / SizeTotale;
	Percentuale = 100 - Percentuale;


	return Percentuale;

}




void * ChiediZonaMemoria(DWORD NewSizeMem, char NomeZonaMemoria[])

{
	// Restituisce memoria di dimensione NewSizeMem ma in realta' ne alloca
	// una leggermente piu' grande e la zona finale la riempie con 0x5555555
	// poi salva i dati di questa zona cuscinetto in VetCheckMemory[TotCheckMemory]

	int SizeExtraMem;
	int TotCheckDword;
	BYTE *pBaseMem;

	if (NewSizeMem > 0x200) {
		SizeExtraMem=0x200;
		TotCheckDword = 4;
	}else
	{
		SizeExtraMem = NewSizeMem;
		TotCheckDword = 1;
	}


	//  salva indirizzi in vetallataca() totallocata
	pBaseMem = (BYTE *) malloc(NewSizeMem + SizeExtraMem);
	AddAllocata(pBaseMem);
	
	// azzera memoria
	memset(pBaseMem,0,NewSizeMem);


	return (void*) pBaseMem;

}


// chiamata ogni volta che viene eseguita una patch su zone di memoria
// memorizza i dati sia GlobTomb4.BaseRemap
// che nel database interno che verra' poi registrato
void SalvaPatchMemoria(DWORD OldInizio, DWORD OldSize, DWORD NewInizio, DWORD NewSize)
{
	int i;
	StrMyDatabase *pDb;


	if (TipoExe != EXE_TOMB4 || GlobTomb4.TestSetup==true) return;

	// ora savarlo anche in mio databaseinterno
	pDb = &MyGlobPrivate.DataBase;

	i = pDb->TotMemZones;

	pDb->pVetMemZone = (StrRecordMemZone *) ReallocMine(pDb->pVetMemZone, 
				sizeof(StrRecordMemZone) * (i+1), "pVetMemZone in SalvaPatchMemoria");


	pDb->pVetMemZone[i].OldAddress = OldInizio;
	pDb->pVetMemZone[i].SizeOldMemory = OldSize;
	pDb->pVetMemZone[i].NewAddress  = NewInizio;
	pDb->pVetMemZone[i].SizeNewMemory = NewSize;
	pDb->TotMemZones++;


}

//funzione usata per estendere memoria
// modificando tutti i punti del codice dove si altera valori
// per ora SOLO trng puo creare queste patch
DWORD ApplicaPatchMemoria(StrPatch VetPatch[], int TotPatch, DWORD OffsetOldInizio, 
				  DWORD OffsetOldFine, DWORD NewSizeMem, char NomeZonaMemoria[])

{
	DWORD *pOffset;
	int i;
	int Differenza;
	DWORD ValoreNow;
	DWORD *pMem;
	DWORD OffsetNewInizio;
	DWORD OffsetNewFine;
	WORD Flags;


	pMem = (DWORD*) ChiediZonaMemoria(NewSizeMem, NomeZonaMemoria);

	OffsetNewInizio = (DWORD) pMem;
	OffsetNewFine = OffsetNewInizio + NewSizeMem;
	// qui vengono salvati indirizzi nuovi e dimensione zona
	SalvaPatchMemoria(OffsetOldInizio, OffsetOldFine-OffsetOldInizio, (DWORD) pMem, NewSizeMem);

	Flags = TYPP_PARAMETRIC | TYPP_PARAMETRIC_MEM;
	if (MyGlobPrivate.TestPatchDinamiche==true) Flags |= TYPP_RUN_TIME;

	// salvarla anche come info generale di patch parametrica
	SalvaPatchArg(0, VetPatch[0].Offset, VetPatch[TotPatch-1].Offset, OffsetNewInizio, Flags, 4);
	
	for (i=0;i<TotPatch;i++) {
		pOffset = (DWORD *) VetPatch[i].Offset;
		ValoreNow = *pOffset;
		if (VetPatch[i].TestInizio == true) {
			// Usare come riferimento OffsetOldInizio
			Differenza = (int) (ValoreNow - OffsetOldInizio);
			ValoreNow = OffsetNewInizio + Differenza;
		}
		else
		{
			// usare come riferimento OffsetOldFine
			Differenza = (int) (ValoreNow - OffsetOldFine);
			ValoreNow = OffsetNewFine + Differenza;
		}
		// il valore e' sempre long/dword (4 bytes)
		*pOffset = ValoreNow;
		SalvaInfoZonaPatch( 0, VetPatch[i].Offset, 4, Flags, OffsetNewInizio, 0,0); 
	}

	return OffsetNewInizio;
}

void ApplicaPatchDato(StrTotElementi VetPatch[], int TotPatch, 
					   DWORD TotOld, DWORD TotNew)
{
	int i;
	void *pPunta;
	DWORD Originale;
	BYTE ValByte;
	WORD ValWord;
	int Differenza;
	bool TestMoltiplicazione;
	WORD NBytes;
	WORD Flags;


	Flags= TYPP_PARAMETRIC | TYPP_PARAMETRIC_CONST;
	if (MyGlobPrivate.TestPatchDinamiche==true) Flags |= TYPP_RUN_TIME;

	for (i=0;i<TotPatch;i++){


		pPunta = (void *) VetPatch[i].Offset; 
		TestMoltiplicazione=false;
		switch (VetPatch[i].Tipo ) {
		case 'b':
			TestMoltiplicazione=true;
		case 'B':
			ValByte = *(BYTE *) pPunta;
			Originale = ValByte;
			break;
		case 'w':
			TestMoltiplicazione=true;
		case 'W':
			ValWord = *(WORD *) pPunta;
			Originale = ValWord;
			break;
		case 'd':
			TestMoltiplicazione=true;
		case 'D':
			Originale = *(DWORD *) pPunta;
			break;
		}
		if (TestMoltiplicazione == true) {
			// calcolare il rapporto tra valore originale e totold
			if (TotOld > Originale) {
				Differenza = TotOld / Originale;
				Originale = TotNew / Differenza;

			}
			else {
				Differenza = Originale / TotOld;
				Originale = TotNew * Differenza;
			}


		}else {

		
			Differenza = Originale - TotOld;

			Originale = TotNew + Differenza;
		}
		switch (VetPatch[i].Tipo ) {
		case 'b':
		case 'B':
			ValByte = (BYTE) Originale;
			*(BYTE*) pPunta = ValByte;
			NBytes=1;
			break;
		case 'w':
		case 'W':
			ValWord = (WORD) Originale;
			*(WORD*) pPunta = ValWord;
			NBytes=2;
			break;
		case 'd':
		case 'D':
			*(DWORD *) pPunta = Originale;
			NBytes=4;
			break;
		}		
		// salvarla anche come dato costante (a meno che valore
		// non sia maggiore >= di 0x40000 nel qual caso considerarlo
		// indirizzo di memoria
		// DWORD TotNew


		SalvaInfoZonaPatch(0, VetPatch[i].Offset, NBytes, Flags, TotNew,0,0);

	}
	// salvare riassunto generale 
	SalvaPatchArg( 0, VetPatch[0].Offset, VetPatch[TotPatch-1].Offset, TotNew, Flags, 0);

}
// se prima data e' maggiore di seconda restituisce +1
// se prima data e' minore di seconda resittuisce -1
// se sono uguali restituisce 0
int ConfrontaDate(SYSTEMTIME *pPrima, SYSTEMTIME *pSeconda)
{
	if (pPrima->wYear > pSeconda->wYear) return 1;
	if (pPrima->wYear < pSeconda->wYear) return -1;

	if (pPrima->wMonth > pSeconda->wMonth) return 1;
	if (pPrima->wMonth < pSeconda->wMonth) return -1;

	if (pPrima->wDay > pSeconda->wDay) return 1;
	if (pPrima->wDay < pSeconda->wDay) return -1;

	if (pPrima->wHour > pSeconda->wHour) return 1;
	if (pPrima->wHour < pSeconda->wHour) return -1;

	if (pPrima->wMinute > pSeconda->wMinute) return 1;
	if (pPrima->wMinute < pSeconda->wMinute) return -1;

	return 0;
}

// trova codice iniziale e finale di modulo attuale
void AnalisiCodiceAllocato(StrPluginRec *pRec)
{
	IMAGE_DOS_HEADER* pDOSHeader;
	IMAGE_NT_HEADERS* pNTHeaders;

	pDOSHeader = (IMAGE_DOS_HEADER*) pRec->hDllPlugin;
	pNTHeaders = (IMAGE_NT_HEADERS*) ((BYTE *) pDOSHeader + pDOSHeader->e_lfanew);


	pRec->OffCodeStart = pNTHeaders->OptionalHeader.BaseOfCode + (DWORD) pDOSHeader;
	pRec->OffCodeEnd = pRec->OffCodeStart + pNTHeaders->OptionalHeader.SizeOfCode;



}
// imposta callback per triggers old e controlla eventuali conflitti su callback replace di stesso elemento
// stessa analisi anche per tutte le callback relative a proc di slot
void ElaboraCallBackPlugins(void)
{
	DWORD i;
	StrPluginRec *pRec;
	int j;
	StrRecordCallBack *pReq;
	char *pNome;
	int Indice;
	char MyBufferTemp[256];

	for (i=0;i<MyGlobPrivate.DataBase.TotPlugins;i++) {
		pRec = &MyGlobPrivate.DataBase.pVetPlugins[i];
		pNome= pRec->Nome;

		for (j=0;j<pRec->TotRequiredCB;j++) {
			pReq= &pRec->VetRequiredCB[j];

			switch (pReq->CB_Tipo) {
			case CB_PAUSE_MANAGER:
			case CB_SAVEGAME_MANAGER:
			case CB_FMV_MANAGER:
			case CB_WINDOWS_FONT_CREATE:
			case CB_WINDOWS_UNICODE_CONVERT:
			case CB_WINDOWS_TEXT_PRINT:
				// callback di tipo replace unico (ma senza che sia stato specificato come flag)
				// sono di tipo manager.

				// avvisare in caso di conflitto
				if (MyGlobPrivate.BaseVetCbReplace.VetDirectCB[pReq->CB_Tipo]) {
					Indice = MyGlobPrivate.BaseVetCbReplace.VetDirectCB_PluginId[pReq->CB_Tipo];

					sprintf(MyBufferTemp, "WARNING: Plugin \"%s\" has gotten the callback #%d that it had been already required by \"%s\" plugin",
							pNome, pReq->CB_Tipo, MyGlobPrivate.DataBase.pVetPlugins[Indice].Nome );
					InviaErroreLog(MyBufferTemp, i,false);
				}

				MyGlobPrivate.BaseVetCbReplace.VetDirectCB[pReq->CB_Tipo]  = pRec->VetDirectCB[pReq->CB_Tipo];
				MyGlobPrivate.BaseVetCbReplace.VetDirectCB_PluginId[pReq->CB_Tipo] = i;

				break;

			case CB_NUMERIC_TRNG_PATCH:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetTrngAsmPatch[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetTrngAsmPatchAddr[pReq->Numero] = pReq->pCall;
					MyGlobPrivate.BaseVetCbReplace.VetTrngAsmPatchFlags[pReq->Numero] = pReq->Flags;
				}
					
				break;

			case CB_SLOT_INITIALISE:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotInitialise[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotInitialise[pReq->Numero] = pReq->pCall;
				}
				break;
 
			case CB_SLOT_CONTROL:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotControl[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotControl[pReq->Numero]  = pReq->pCall;
				}
				break;
			case CB_SLOT_COLLISION:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotCollision[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotCollision[pReq->Numero]  = pReq->pCall;
				}
				break;
			case CB_SLOT_DRAW:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotDraw [pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotDraw[pReq->Numero]  = pReq->pCall;
				}
				break;
			case CB_SLOT_FLOOR:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotFloor[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotFloor[pReq->Numero]  = pReq->pCall;
				}
				break;

			case CB_SLOT_CEILING:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotCeiling[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotCeiling[pReq->Numero]  = pReq->pCall;
				}
				break;
			case CB_STATE_ID_LARA_CTRL:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetStateIdControl[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetStateIdControl[pReq->Numero] = pReq->pCall;
				}
				break;

			case CB_STATE_ID_LARA_COLLISION:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetStateIdCollision[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetStateIdCollision[pReq->Numero] = pReq->pCall;
				}
				break;
			case CB_SLOT_DRAW_EXTRA:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetSlotDrawExtra[pReq->Numero] |= pReq->Flags;
				}
				if (pReq->Flags & CBT_REPLACE) {
					MyGlobPrivate.BaseVetCbReplace.VetSlotDrawExtra[pReq->Numero]  = pReq->pCall;
				}
				break;
			case CB_FLIPEFFECT:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetFlipeffects[pReq->Numero] |= pReq->Flags;
				}

				if (pReq->Flags & CBT_REPLACE) {
					// qui si potrebbe controllare conflitto
					MyGlobPrivate.BaseVetCbReplace.VetFlipeffects[pReq->Numero] = pReq->pCall;
				}
				break;
			case CB_ACTION:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetActions[pReq->Numero] |= pReq->Flags;
				}

				if (pReq->Flags & CBT_REPLACE) {
					// qui si potrebbe controllare conflitto
					MyGlobPrivate.BaseVetCbReplace.VetActions[pReq->Numero] = pReq->pCall;
				}
				break;
			case CB_CONDITION:
				if (pReq->Flags & (CBT_FIRST | CBT_AFTER)) {
					MyGlobPrivate.BaseVetCbFirstAfter.VetConditions[pReq->Numero] |= pReq->Flags;
				}

				if (pReq->Flags & CBT_REPLACE) {
					// qui si potrebbe controllare conflitto
					MyGlobPrivate.BaseVetCbReplace.VetConditions[pReq->Numero] = pReq->pCall;
				}
				break;
			default:
				// se e' una callback di tipo diretto, segnalare comunque la sua presenza
				if (pReq->CB_Tipo < MAX_DIRECT_CB) {
					// e' una callback diretta
					// inserire CBT_PRESENT per sapere subito che c'e' da cercare
					MyGlobPrivate.BaseVetCbFirstAfter.VetDirectCB[pReq->CB_Tipo] |= (CBT_PRESENT | pReq->Flags);
					// se e' di tipo replace salvare suo indirizzo
					if (pReq->Flags & CBT_REPLACE) {
						MyGlobPrivate.BaseVetCbReplace.VetDirectCB[pReq->CB_Tipo] = pReq->pCall;
					}
				}
				break;
			}
		}
	}
}
// estrae le versioni (file e product) del file plugin pNomeFile e le sava in pRec
void ImpostaVersioni(char *pNomeFile, StrPluginRec *pRec, DWORD ID_Plugin)
{
	char BufferProductVer[80];
	char *pFileVer;
	int i;
	char BufferHost[512];
	char *pVetPartiStore[20];
	int TotParti;
	int NCifre;
	int Numero;
	int z;
	char *pVerNow;
	BYTE *pVetVerNow;
	char **pVetParti;

	pFileVer=TrovaVersioneFile(pNomeFile, BufferProductVer);

	for (i=0;i<4;i++) {
		pRec->VersioneFile[i]=0;
		pRec->VersioneProduct[i]=0;
	}
	for (z=0;z<2;z++) {
		if (z==0) {
			pVerNow=pFileVer;
			pVetVerNow = pRec->VersioneFile;
		}else {
			pVerNow=BufferProductVer;
			pVetVerNow = pRec->VersioneProduct;
		}

		// separare in singoli numeri
		pVetParti=Split(pVerNow, '.', &TotParti, BufferHost, pVetPartiStore,false);
		NCifre = TotParti;
		if (NCifre > 4) NCifre=4;

		for (i=0;i<NCifre;i++) {
			if (PrendiNumero(pVetParti[i], &Numero)==false) {
				InviaErroreLog("ERROR: unvalid number in plugin version", ID_Plugin,false);
				return;
			}
			if (Numero > 255) {
				InviaErroreLog("ERROR: unvalid number (greater than 255) in plugin version", ID_Plugin,false);
				return;				
			}
			pVetVerNow[i] = (BYTE) Numero;
		}
	}
		

}

// restituisce +1  se ver1 e maggiore di quella di tomb_nextgeneration.dll attuale
// restituisse -1  se e' minore
// resituisce  0 se sono uguali
 int ConfrontaVersioneConTRNG(BYTE Ver1[])
{

	int i;

	for (i=0;i<4;i++) {

		if (Ver1[i] > GlobTomb4.BaseVersione.VetVersione[i]) return 1;
		if (Ver1[i] < GlobTomb4.BaseVersione.VetVersione[i]) return -1;
	}

	return 0;
}

// cerca eventuali dll plugin
// li carica in memoria e assegna ad ognuno un id
// nel caso ci sia errore restituisce false
bool CaricaPlugins(void)
{
	StrListaFiles *pLista;
	DWORD TotFiles;
	char BufPluginLog[1024];
	char NomeFile[256];
	char NomeFullPlugin[256];
	FILE * TempFile;
	DWORD SizeFile;
	DWORD i;
	DWORD j;
	DWORD z;
	StrTrngInfos *pInfo = (StrTrngInfos*) 0x4A6E70;
	SYSTEMTIME ScambioData;
	StrListaFiles ScambioNome;
	SYSTEMTIME *pVetDate;
	char *pNome;
	char *pNomeCorto;
	DWORD IdPlugin;
	StrMyDatabase *pDb;
	DWORD *pDword;
	StrPluginRec *pRec;
	bool TestIgnora;
	char NomeBootLog[256];
	char *pTexData;

	pLista= TrovaFiles(Dir_Trle, "PlugIn*.dll",(int*) &TotFiles);

	sprintf(NomeBootLog, "%s\\TEMP_LOGs\\PLUGINS_BOOT_LOG.txt", Dir_Trle);
	remove(NomeBootLog);

	if (TotFiles==0) {
		InviaToLogFile(NomeBootLog, "No plugins found");

		return true;
	}

	// se qualche plugin ha settato il MPS_DISABLE
	// escluderlo dalla lista
	z=0;
	for (i=0;i<TotFiles;i++) {
		pNome= SoloNomeSenzaExt(pLista[i].Testo);
		TestIgnora=false;
		for (j=0;j< (DWORD) GlobTomb4.pPluginScriptTable->TotPlugins;j++) {
			if (strcmpi(pNome, GlobTomb4.pPluginScriptTable->VetPlugins[j].Name)==0) {
				if (GlobTomb4.pPluginScriptTable->VetPlugins[j].MainPluginSettings & MPS_DISABLE) {
					TestIgnora=true;
					break;
				}
			}
		}

		if (TestIgnora==false) {
			pLista[z++] = pLista[i];
		}
	}
	TotFiles=z;

	if (TotFiles==0) return true;

	// ora ordinare i nomi file in base alla data: prima i piu' vecchi
	// e dopo i piu' recenti
	pVetDate= (SYSTEMTIME*) malloc(sizeof(SYSTEMTIME) * TotFiles);

	for (i=0;i<TotFiles;i++){
		GetDataDelFileSys(pLista[i].Testo, &pVetDate[i]);
	}

	// ora ordinare il vettore sulla base di date crescenti
	// ossia mettere prima quelle piu' piccole e vecchie
	for (i=0;i<TotFiles;i++) {
		for (j=i+1;j<TotFiles-1;j++) {
			// se i maggiore di j: invertire
			if (ConfrontaDate(&pVetDate[i], &pVetDate[j])>0) {
				// invertire tutto
				ScambioData = pVetDate[i];
				pVetDate[i] = pVetDate[j];
				pVetDate[j] = ScambioData;

				strcpy(ScambioNome.Testo, pLista[i].Testo);
				strcpy(pLista[i].Testo, pLista[j].Testo);
				strcpy(pLista[j].Testo, ScambioNome.Testo);

			}
		}

	}
	// ok, ora sono ordinati
	// salvare anche queesti dati in database
	// ora collegare tutti i plugin e inserire i dati nella struttura di scambio
	pDb = &MyGlobPrivate.DataBase;

	for (i=0;i<TotFiles;i++) {
		pNome= SoloNome(pLista[i].Testo);
		pNomeCorto= SoloNomeSenzaExt(pNome);
	
		
		IdPlugin = AddPlugin(pNome, &pVetDate[i]);

		pRec = &pDb->pVetPlugins[IdPlugin];
		pRec->PluginIdScript = GetIdScriptPlugin(pNomeCorto);

		// ------------------ LOG BOOT COLLECT -------------------
		// mettere nel log i plugin e la loro versione ecc
		
		ImpostaTrngInfo(IdPlugin);

		sprintf(NomeFullPlugin, "%s\\%s", Dir_Trle, pLista[i].Testo);


		// scrivere log in NomeBootLog per questo plugin
	
		sprintf(BufferLog, "--------------  %s ----------------------",pRec->Nome);
		InviaToLogFile(NomeBootLog, BufferLog);

		// ottiene data del file
		pTexData = GetDataFile(NomeFullPlugin);

		sprintf(BufferLog, "\tDate: %s", pTexData);
		InviaToLogFile(NomeBootLog, BufferLog);

		// legge versioni di questo plugin
		ImpostaVersioni(pNome, pRec, IdPlugin);

		sprintf(BufferLog, "\tVersions:  Plugin=%d.%d.%d.%d  RequiredTrng=%d.%d.%d.%d",
			pRec->VersioneFile[0], pRec->VersioneFile[1], pRec->VersioneFile[2], pRec->VersioneFile[3],
			pRec->VersioneProduct[0], pRec->VersioneProduct[1],pRec->VersioneProduct[2],pRec->VersioneProduct[3]);
		InviaToLogFile(NomeBootLog, BufferLog);

		// qui dare avviso se la versone richiesta dal plugin e' maggiore di qella attuale
		if (ConfrontaVersioneConTRNG(pRec->VersioneProduct) > 0) {
			sprintf(BufferLog, "\tWARNING: it looks that the \"%s\" plugin required a tomb_nextgeneration.dll with a version higher than current (%s)",
				SoloNome(NomeFullPlugin), MexVersione);
			InviaToLogFile(NomeBootLog, BufferLog);
			

		}
		

		pRec->hDllPlugin  =LoadLibrary(NomeFullPlugin);

		if (pRec->hDllPlugin==NULL) {

			// leggere il log di warm up del plugin che ha dato errore
			sprintf(NomeFile, "%s\\TEMP_LOGs\\%s_warm_up_log.txt", Dir_Trle, pNomeCorto);

			TempFile=fopen(NomeFile, "rb");
			SizeFile=QuantoLungo(TempFile);
			fread(BufPluginLog, SizeFile, 1, TempFile);
			fclose(TempFile);
			
			sprintf(BufferLog, "\tFailed LoadLibrary(\"%s\") call", NomeFullPlugin);
			InviaToLogFile(NomeBootLog, BufferLog);

			InviaToLogFile(NomeBootLog, "---- Log from plugin ------");
			InviaToLogFile(NomeBootLog, BufPluginLog);
			InviaToLogFile(NomeBootLog, "----- End File -------");

			InviaErroreLog("ERROR: failed the attaching!",IdPlugin,false);
			sprintf(BufferLog, "ERROR trying to load plugin: %s\r\n\r\nWarm-Up log of \"%s\" plugin:\r\n\r\n%s", 
				pNome, pNome, BufPluginLog);
			Tomb4MessageBox(BufferLog,"Error in warm up dll");
			return false;
		}


		// impostare pluginid script (se c'e')
		pRec->PluginIdScript =-1;

		for (j=0;j< (DWORD) GlobTomb4.pPluginScriptTable->TotPlugins;j++) {
			if (strcmpi(GlobTomb4.pPluginScriptTable->VetPlugins[j].Name, pRec->Nome) == 0) {

				pRec->PluginIdScript = GlobTomb4.pPluginScriptTable->VetPlugins[j].PluginId;
				break;
			}
		}
		// qui mettere messaggio nel log iniziale sul fatto che questo plugin.dll non ha un comando plugin= nello script
		if (pRec->PluginIdScript ==-1) {
			sprintf(BufferLog, "WARNING: %s.dll has not a Plugin= command in the script",
				pRec->Nome);

			InviaErroreLog(BufferLog, IdPlugin, false);
		}

		

		AnalisiCodiceAllocato(pRec);

		sprintf(BufferLog, "\tPlugin IDs:  Tomb4=%d  NGLE=%d  NG_Center=%d",
			IdPlugin, pRec->PluginIdNgle, pRec->PluginIdScript);
		InviaToLogFile(NomeBootLog, BufferLog);

		sprintf(BufferLog, "\tCode Range:  Start=0x%x  End=0x%x", pRec->OffCodeStart, pRec->OffCodeEnd);

		InviaToLogFile(NomeBootLog, BufferLog);



		// ora controllare l'offset di routine patcher
		// impostata da plugin
		if (pInfo->AdrTomb4Patcher) {


			
			if (pInfo->pAdrDllPatcher == NULL) {
				InviaErroreLog("ERROR: failed the attaching!",IdPlugin,false);
				sprintf(BufferLog, "ERROR: plugin \"%s\" didn't set DLL ADR PATCHER",
					pNome);
				Tomb4MessageBox(BufferLog,"Error in warm up dll");
				return false;
			}
			// impostare come reserved data quella usata dal AdrTombPatcher (a meno che non sia 0)
			if (pInfo->AdrTomb4Patcher) {
				if (SetReservedZone( IdPlugin, pInfo->AdrTomb4Patcher, 4) != APPC_OK) {
					// c'e' conflitto
					// conflitto
					InviaErroreLog("ERROR: failed the attaching!",IdPlugin,false);
					// dare errore spiegando che l'offset tomb4 scelto
					// e' in conflito con altro ... ecc
					sprintf(BufferLog,"ERROR: the plugin \"%s\" chose as TOMB4 ADR PATCHER an address (0x%X) that is in conflict with a reserved/used zone of another plugin or trng engine",
						pNome, 
						pInfo->AdrTomb4Patcher);
					Tomb4MessageBox(BufferLog, "Error in warm up dll");
					return false;
				}
			}

			// ok, niente conflitto: ora impostare indirizzo
			pDword = (DWORD *) pInfo->AdrTomb4Patcher;
			*pDword = (DWORD) pInfo->pAdrDllPatcher;
		}

		// e salvare indirizzo in vetplugin
		pRec->OffsetPatcher = pInfo->AdrTomb4Patcher;
		if (pInfo->TestDebugMode) {
			MyGlobPrivate.TestDebugVersione =true;
		}
		sprintf(BufferLog, "\tPatch Routine Offest = 0x%x", pRec->OffsetPatcher);
		InviaToLogFile(NomeBootLog, BufferLog);

		// eseguire le patch di plugin
		pRec->pDirectCall  = pInfo->pDirectCallBack;
		if (pRec->pDirectCall==NULL) {
			InviaErroreLog("ERROR: failed the attaching!",IdPlugin,false);
			// dare errore spiegando che non e' stato impostato l'indirizzo per chiamate dirette
			sprintf(BufferLog,"ERROR: the plugin \"%s\" didn't supply the address of CallFromTrng() function",
				pNome);
			Tomb4MessageBox(BufferLog, "Error in warm up dll");
			return false;
		}

	}
	InviaErroreLog("The library has been successfully attached!",IdPlugin,false);
	return true;
			
}
// trova sezione NomeSezione[] e restituisce il trigger col numero
// maggiore di quella sezione
int TrovaMaxOfSection(char *pTesto, char NomeSezione[])
{
	int MaxValore;
	int Numero;
	int Inizio;
	int Fine;
	char *pSezione;
	DWORD IndiceNow;
	DWORD SizeTesto;
	bool TestFinito;
	DWORD i;
	char BufLinea[512];
	char **pVetParti;
	int TotParti;
	char SalvaChar;

	Inizio = InStr(1, pTesto, NomeSezione);
	Inizio += strlen(NomeSezione);

	Fine = InStr(Inizio, pTesto, "<END>");
	SalvaChar=pTesto[Fine];
	pTesto[Fine]=0;

	pSezione = &pTesto[Inizio];
	SizeTesto = strlen(pSezione);
	IndiceNow=0;

	MaxValore=0;
	// ora prendere una riga alla volta
	while (IndiceNow < SizeTesto && *pSezione != 0) {
		// caricare in buflinea la linea a cui punta pTesto attualmente
		TestFinito=false;
		i=0;
		while (i < 511 && *pSezione != 0x0d && *pSezione != 0x0a) {
			if (IndiceNow >= SizeTesto || *pSezione == 0) {
				TestFinito=true;
				break;
			}
			IndiceNow++;
			BufLinea[i++]= *(pSezione++);
		}

		if (TestFinito==true) break;
		// saltare eventuali codici 0x0d e 0x0a in fondo a linea attuale
		while (*pSezione == 0x0d || *pSezione == 0x0a) pSezione++;

		BufLinea[i]=0;

		// ok, ora prenere primo numero
		pVetParti = Split(BufLinea, ':', &TotParti, NULL, NULL, true);

		if (TotParti == 2) {
			if (PrendiNumero(pVetParti[0], &Numero)==true) {
				if (Numero > MaxValore) MaxValore=Numero;

			}

		}
	}

	pTesto[Fine]=SalvaChar;
	return MaxValore;

	
}

// scandisce file ng_constant incluso in risorse
// per scoprire i flipeffect, action e condition usati da trng
// e poi scrive questi valori in database
// imposta anche first e last param e cust
void InizializzaPluginTrng(void)
{
	StrMyDatabase *pDb;
	StrPluginRec *pRec;
	HRSRC HandleRes;
	int *pInt;
	char *pChar;
	DWORD SizeTesto;
	char *pMemTemp;
	char NomeFile[256];
	char *pTesto;
	static 	char VetDecrypt[] = {33,127,67,91,55,66, 33, 23,54,0};
	DWORD i,j;
	SYSTEMTIME DataDll;	
	
	pDb = &MyGlobPrivate.DataBase;

	// aggiungere nome di next generation
	sprintf(NomeFile, "%s\\Tomb_NextGeneration.dll", Dir_Trle);
	GetDataDelFileSys(NomeFile, &DataDll);

	AddPlugin("Tomb_NextGeneration.dll", &DataDll);

	pRec = &pDb->pVetPlugins[0];

	pRec->OffsetPatcher = 0x4A6CA0; // Patch_GenericheAsm

	pRec->hDllPlugin = GetModuleHandle("Tomb_NextGeneration.dll");

	AnalisiCodiceAllocato(pRec);

	// impostare versioni file
	ImpostaVersioni("Tomb_NextGeneration.dll",pRec, 0);


	// ora caricare ng_constants.txt dalle risorse
	HandleRes = FindResource(MiaIstanzaDll, 
					MAKEINTRESOURCE(IDR_NGCONSTANTS), "LCURSOR");
	if (HandleRes == 0) {
		InviaErroreLog("ERROR loading resource file",0,false);

		return;
	}

	pChar = (char*) LoadResource(MiaIstanzaDll, HandleRes);	
	pInt = (int *) pChar;
	SizeTesto = *pInt;
	pChar +=4;

	pMemTemp = (char*) malloc(SizeTesto+10);
	memcpy(pMemTemp, pChar, SizeTesto);
	pTesto= pMemTemp;

	// decodificare
	j=0;
	for (i=0;i<SizeTesto;i++) {
		pTesto[i] ^= VetDecrypt[j++];
		if (VetDecrypt[j]==0) j=0;
	}

	pTesto[SizeTesto]=0;

	// terminare
	free(pMemTemp);


}
// cripta numero usando offset attuale di file
void SalvaNumeroCriptato(DWORD Valore, FILE *pFile)
{
	DWORD Offset;
	BYTE *pByte;
	BYTE *pVal;
	int i;

	Offset = ftell(pFile);
	
	pByte = (BYTE *) &Offset;
	pVal = (BYTE *) &Valore;

	for (i=0;i<4;i++) {
		pVal[i] ^= 0x55;
		pVal[i] ^= pByte[i];
	}

	fwrite(&Valore, 4,1, pFile);

}
// salva database con lista di tutte patch effettuate
// solitamente crea file con nome "db_patches.bin"
bool SalvaDatabase(char *pNome)
{

	FILE *pFile;
	StrMyDatabase *pDb;


	pFile = fopen(GetFileCrash(pNome),"wb");
	if (pFile == NULL) return false;

	pDb = &MyGlobPrivate.DataBase;
	
	// usare SalvaNumeroCriptato()
	// per tutti i numeri critici

	// salvare nomi Plugin
	fwrite(&pDb->TotPlugins, 4,1, pFile);
	fwrite(pDb->pVetPlugins, sizeof(StrPluginRec) * pDb->TotPlugins,1, pFile);

	// salvare zone estensione memoria
	fwrite(&pDb->TotMemZones, 4,1, pFile);
	fwrite(pDb->pVetMemZone, sizeof(StrRecordMemZone) * pDb->TotMemZones,1, pFile);

	// salvare patch di codice 
	fwrite(&pDb->TotCodeZones, 4,1, pFile);
	fwrite(pDb->pVetCodePatch, sizeof(StrCodePatch) * pDb->TotCodeZones,1 , pFile);


	// salvare patch arg (modifica dato costante, indirizzo memoria, call)
	fwrite(&pDb->TotPatchArg , 4,1, pFile);
	fwrite(pDb->pVetPatchArg , sizeof(StrArgPatch) * pDb->TotPatchArg, 1, pFile);


	// salvare zone data reserved
	fwrite(&pDb->TotReservedData, 4,1, pFile);
	fwrite(pDb->pVetReserved, sizeof(StrReservedData) * pDb->TotReservedData, 1 , pFile);
	fclose(pFile);

	return true;
}


// salva il database su disco e libera la memoria allocata nel database 
// nota: sideve chiaare solo DOPO funzione termina di tutte le prcoedure

void TerminaDatabase(void)
{
	char *pNome;
	StrMyDatabase *pDb;

	if (TipoExe != EXE_TOMB4) return;

	// salvare sempre
	pNome = "db_patches.bin";
	SalvaDatabase(pNome);
	
	// ora liberare memoria
	pDb = &MyGlobPrivate.DataBase;

	if (pDb->pVetCodePatch ) {
		FreeMine(pDb->pVetCodePatch);
		pDb->pVetCodePatch=NULL;
	}

	if (pDb->pVetMemZone){
		FreeMine(pDb->pVetMemZone);
		pDb->pVetMemZone=NULL;
	}

	if (pDb->pVetPlugins) {
		FreeMine(pDb->pVetPlugins);
		pDb->pVetPlugins=NULL;
	}
	
	if (pDb->pVetReserved) {
		FreeMine(pDb->pVetReserved);
		pDb->pVetReserved =NULL;
	}

	return;


}
