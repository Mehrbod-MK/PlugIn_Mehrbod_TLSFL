
#include "stdafx.h"
#include "bass.h"
#include "stdio.h"

#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll

#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "macros.h"  // definition of macros



// ------- constant data and global variables -------------
// #TAG_ADDING_NEW_VARIABLES#
//...
extern StrGlobaliTomb4 GlobTomb4;
extern char BufferLog[];


// ------- declare external functions

// #TAG_ADDING_NEW_DECLARES#
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line

bool TastoVKPremuto(int VirtualKey);
bool VerificaSingleGlobalTrigger(short GlobalTrigger, short Parametro, 
								 bool TestIgnoraParametro);
void EseguiAnimazione(WORD NumeroAnimazione, int IndiceSlot, bool TestSetNextStateId);
extern TYPE_SoundEffect SoundEffect;

int GetNgleIndice(int TombIndice);



// ++++++ functions +++++++++++++++

// #TAG_ADDING_NEW_FUNCTIONS#

// modifica posizione tasto IndiceTasto, su o giu
void CambiaTasto(int *pTree, int IndiceTasto, int Valore, bool TestRestore)
{

	if (TestRestore==false) {
		pTree[IndiceTasto*4 + 2] = Valore;
	}else {
		// ripristinare l'intera coppia di tasti
		pTree[IndiceTasto*4 + 2] = 10;
		pTree[(IndiceTasto+12) * 4 + 2] = 0;
	}

}

// dunque, qui ci dovrebbe essere gestione completa di tastierino
// lo mostra, attende input, ecc.
// se restituisce -1 vuol dire che utente ha rinunciato
// altrimenti si tratta del numero battuto.
int GestioneKeyPad(int TipoKeyPad, WORD OcbCode, WORD Slot)
{
	//nota poi tutti i dati saranno forniti da comando script
	// ma intanto usare roba fissa.
	int Colonna, Riga;
	StrKeyPad *pKeyPad;
	int TastiInput;
	int IndiceTree;
	int **p2MeshTree = (int **) 0x533958;
	int *pMeshTree;
	bool TestShowNumero;

	int i;

	DWORD FramePassati;
	bool TestNuovoTasto;
	WORD ValoreTasto;
	const TempoDelay = 10;
	bool TestEsegui;
	int Moltiplicatore;
	int MaxCifre;
	bool TestPremutoTasto;

	DWORD FrameNow;
	bool TestIgnora;
	int Range;


	TestShowNumero=false;

	

	pKeyPad = &GlobTomb4.BaseKeyPad;

	if (pKeyPad->TestAttivo == false) {
		if (((*GlobTomb4.pAdr->pFrameCounter) - pKeyPad->LastFrameExit) <= 
				(FRAME_SECONDO /2)) return 0;


		pKeyPad->TestAttivo=true;


		// pKeyPad->ValoreAtteso 
		// prima esecuzione, lara ha appena premuto ctrl
		// disattivare i comandi tastiera tradizionali
		pKeyPad->OldTastiBloccati = GlobTomb4.KeysToStop;
		GlobTomb4.KeysToStop = CMD_ALL;
		// inizializzare altri valori
		pKeyPad->IndiceTasto =0;
		pKeyPad->IndiceTastoOld =-1;
		pKeyPad->Fase = FKP_ATTESA;

		pKeyPad->TotInseriti =0;
		pKeyPad->Slot= Slot; // per switch1 era 458;
		pKeyPad->LastFrameKey = *GlobTomb4.pAdr->pFrameCounter;
		// trovare puntatore di meshtree
		pMeshTree = *p2MeshTree;
		IndiceTree = GlobTomb4.pAdr->pVetSlot[pKeyPad->Slot].IndexFirstTree;
		pKeyPad->pStartMeshTree = &pMeshTree[IndiceTree];
		pKeyPad->IndiceTastoOld = SCRIPT_IGNORE;
		pKeyPad->BufOldText[0]=0;
		pKeyPad->TipoKeyPad = TipoKeyPad;
		pKeyPad->TestAttivaTriggerSwitch=false;

		pKeyPad->ValoreAtteso = OcbCode;


		pKeyPad->LaraX = GlobTomb4.pAdr->pLara->CordX;
		pKeyPad->LaraZ = GlobTomb4.pAdr->pLara->CordZ;

		if (pKeyPad->ScriptSettings.IndexFirstAnim != SCRIPT_IGNORE) {
			pKeyPad->Fase = FKP_WAIT_ANIMATION;
				
			// eseguire animazione
			EseguiAnimazione(pKeyPad->ScriptSettings.IndexFirstAnim ,0,false);
		}else {
			// nessuna animazione  in teoria, ma in pratica
			// se non metto un'animazione si blocca tutto
			EseguiAnimazione(0x67,0, false);
		}


	}

	if (pKeyPad->Fase == FKP_WAIT_ANIMATION) {

		FrameNow = *GlobTomb4.pAdr->pFrameCounter - pKeyPad->LastFrameKey;

		if (GlobTomb4.pAdr->pLara->AnimationNow == pKeyPad->ScriptSettings.IndexFirstAnim  && 
			FrameNow >= pKeyPad->ScriptSettings.FrameFirstAnim ) {
				// togliere fase
				pKeyPad->Fase = FKP_ATTESA;
				// e faree suono
				if (pKeyPad->ScriptSettings.ClickSound != SCRIPT_IGNORE) 
						SoundEffect(pKeyPad->ScriptSettings.ClickSound ,0,0);
				// ora salvare coordinate di lara e se viene
				// cambiata uscire come se fosse stato premuto escape
				pKeyPad->LaraX = GlobTomb4.pAdr->pLara->CordX;
				pKeyPad->LaraZ = GlobTomb4.pAdr->pLara->CordZ;

				VerificaSingleGlobalTrigger(GT_KEYPAD_SHOWED, 
							GetNgleIndice(pKeyPad->IndiceKeypad),false);

		}else {
			return -1;
		}

	}
	TestEsegui=false;

	if (pKeyPad->Fase == FKP_RIMUOVI) {
		if (pKeyPad->IndiceTastoOld != -1) {
			CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTastoOld,0,true);

			
		}
		// riattivare input
		*GlobTomb4.pAdr->pInputExtGameCommands =0;

		GlobTomb4.KeysToStop = pKeyPad->OldTastiBloccati;
		pKeyPad->LastFrameExit = *GlobTomb4.pAdr->pFrameCounter;
		if (pKeyPad->TestAttivo) {
			VerificaSingleGlobalTrigger(GT_KEYPAD_REMOVED, 
						GetNgleIndice(pKeyPad->IndiceKeypad),false);
		}		
		pKeyPad->TestAttivo=false;
		return 0;
	}

	if (pKeyPad->Fase == FKP_WAIT_END_ANIMATION) {
		// adesso controllare se animazione e' terminata

		if (GlobTomb4.pAdr->pLara->AnimationNow == 
					pKeyPad->ScriptSettings.IndexAfterAnim) return 0;
		// ok, animazione finale e' terminata, ora gestire uscita
		// come prima.
		// se animazione e' finita e  il tasto era conferma tutto
		// allora disattivare keypad e uscire con il valore fornito
		// a meno che non sia di tipo switch
		if (pKeyPad->TipoKeyPad == TKP_SWITCH) {
			
		
			if (pKeyPad->ValoreInserito == pKeyPad->ValoreAtteso ||
				pKeyPad->ValoreAtteso > 9999) {
				pKeyPad->TestAttivaTriggerSwitch =true;
				GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
							pKeyPad->ValoreInserito;

				return 0;
			}
		}
		if (pKeyPad->TestAttivo) {
			VerificaSingleGlobalTrigger(GT_KEYPAD_REMOVED, 
						GetNgleIndice(pKeyPad->IndiceKeypad),false);
		}
		pKeyPad->TestAttivo=false;
		GlobTomb4.KeysToStop=pKeyPad->OldTastiBloccati;
		pKeyPad->LastFrameExit = *GlobTomb4.pAdr->pFrameCounter;
		GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
					pKeyPad->ValoreInserito;
		return pKeyPad->ValoreInserito;
	}

	if (pKeyPad->Fase == FKP_ATTENDI_FINE_ESCAPE ) {
		if (GlobTomb4.VetKeysToStop[1] == 0) {
			ValoreTasto= pKeyPad->IndiceTasto + 1;
			// ripristinare tasti
			if (pKeyPad->IndiceTastoOld != -1) {
				CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTastoOld,0,true);

				
			}
			// riattivare input
			GlobTomb4.KeysToStop = pKeyPad->OldTastiBloccati;
			pKeyPad->ValoreInserito=-1;
			pKeyPad->LastFrameExit = *GlobTomb4.pAdr->pFrameCounter;
			if (pKeyPad->TestAttivo) {
				VerificaSingleGlobalTrigger(GT_KEYPAD_REMOVED, 
							GetNgleIndice(pKeyPad->IndiceKeypad),false);
			}
			pKeyPad->TestAttivo=false;
		}
		GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
					pKeyPad->ValoreInserito;
		return 0;
	}

	if (pKeyPad->Fase == FKP_ANIMAZIONE_TASTO) {
		// gestire animazione tasto
		pKeyPad->CordYAnimazione += pKeyPad->IncYAnimazione;

		if (pKeyPad->CordYAnimazione == 0) {
			// animazione finita
			pKeyPad->Fase = FKP_ATTESA;
		}

		if (pKeyPad->CordYAnimazione == 10) {
			// arrivati a massima pressione, ora invertire incremento
			pKeyPad->IncYAnimazione=-2;
		}

		// visualizzare tasti
		CambiaTasto(pKeyPad->pStartMeshTree, pKeyPad->IndiceTasto , 
					pKeyPad->CordYAnimazione + 10, false);
		CambiaTasto(pKeyPad->pStartMeshTree, pKeyPad->IndiceTasto + 12, 
					pKeyPad->CordYAnimazione , false);
		// se e' stato inserito un numero e siamo in modalita
		
		if (pKeyPad->Fase != FKP_ANIMAZIONE_TASTO && 
								(pKeyPad->IndiceTasto == 11 ||
								pKeyPad->ValoreAtteso >9999)  && 
								pKeyPad->TotInseriti > 0) { 

			ValoreTasto =0;
			Moltiplicatore=1;
			for (i=pKeyPad->TotInseriti-1;i>=0;i--) {
				ValoreTasto += Moltiplicatore * pKeyPad->VetInseriti[i];
				Moltiplicatore *= 10;
			}
			pKeyPad->ValoreInserito = ValoreTasto;
			// controllo se c'e' bisogno di animazione finale
			if (pKeyPad->ScriptSettings.IndexAfterAnim == SCRIPT_IGNORE) {


				// se animazione e' finita e  il tasto era conferma tutto
				// allora disattivare keypad e uscire con il valore fornito
				// a meno che non sia di tipo switch
				if (pKeyPad->TipoKeyPad == TKP_SWITCH) {
					
				
					if (pKeyPad->ValoreInserito == pKeyPad->ValoreAtteso ||
						pKeyPad->ValoreAtteso > 9999) {
						pKeyPad->TestAttivaTriggerSwitch =true;
						GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
									pKeyPad->ValoreInserito;
						return 0;
					}
				}
				if (pKeyPad->TestAttivo) {
					VerificaSingleGlobalTrigger(GT_KEYPAD_REMOVED, 
								GetNgleIndice(pKeyPad->IndiceKeypad),false);
				}
				pKeyPad->TestAttivo=false;
				pKeyPad->LastFrameExit = *GlobTomb4.pAdr->pFrameCounter;
				GlobTomb4.KeysToStop=pKeyPad->OldTastiBloccati;
				GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
							pKeyPad->ValoreInserito;
				return pKeyPad->ValoreInserito;
			}else {
				// si vuole animazione finale.
				// allora: avviare fase attesa per animaizione finale
				pKeyPad->Fase = FKP_WAIT_END_ANIMATION;
				EseguiAnimazione(pKeyPad->ScriptSettings.IndexAfterAnim,0, false);
				return 0;
			}
		}
		
		return -1;
	}
	// se lara e' stata spostata abortire subito



	if (GlobTomb4.pAdr->pLara->CordX != pKeyPad->LaraX ||
		GlobTomb4.pAdr->pLara->CordZ != pKeyPad->LaraZ ) {
		// lara e' stata spostata, uscire subito
		pKeyPad->ValoreInserito=-1;
		if (pKeyPad->IndiceTasto != -1) {
			CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTasto,0,true);
		}
		GlobTomb4.KeysToStop=pKeyPad->OldTastiBloccati;
		pKeyPad->LastFrameExit=*GlobTomb4.pAdr->pFrameCounter;
		if (pKeyPad->TestAttivo) {
			VerificaSingleGlobalTrigger(GT_KEYPAD_REMOVED, 
						GetNgleIndice(pKeyPad->IndiceKeypad),false);
		}
		pKeyPad->TestAttivo=false;
		GlobTomb4.pBaseVariableTRNG->Globals.LastInputNumber = 
					pKeyPad->ValoreInserito;
		return -1;
	}
	TestPremutoTasto=false;


	// calcolare movimenti e tasti


	Riga = pKeyPad->IndiceTasto / 3;
	Colonna = pKeyPad->IndiceTasto % 3;
	TastiInput = GlobTomb4.KeysStopped;

	// se siamo in modalita' singolo tasto considerare
	// il tasto entrer (28) in keybloccati) come se fosse un ctrl
	if (pKeyPad->ValoreAtteso > 9999 && 
		GlobTomb4.VetKeysToStop[28] != 0 && 
		pKeyPad->TotInseriti == 0) {

		GlobTomb4.VetKeysToStop[28]=0;
		TastiInput |= 0x40;
	}


	FramePassati = *GlobTomb4.pAdr->pFrameCounter - pKeyPad->LastFrameKey;
	TestNuovoTasto=false;

	if (TastiInput & 0x01) {
		// in alto
		// ignorarlo se passato troppo poco tempo e il tasto e' lo stesso
		if (FramePassati >= TempoDelay ) {
			if (Riga > 0) Riga--;
			TestNuovoTasto=true;

		}

	}

	if (TastiInput & 0x02) {
		// in basso
		if (FramePassati >= TempoDelay) {
			if (Riga < 3) Riga++;

			TestNuovoTasto=true;
		}

	}

	if (TastiInput & 0x04) {
		// a sinistra
		if (FramePassati >= TempoDelay ) {
			if (Colonna > 0) Colonna--;

			TestNuovoTasto=true;
		}

	}

	if (TastiInput & 0x08) {
		// a destra
		if (FramePassati >= TempoDelay ) {
			if (Colonna < 2) Colonna++;

			TestNuovoTasto=true;
		}

	}

	// gestire anche tasti dirtti, ossia se viene premuto "0" "1" ecc.
	for (i=2;i<12;i++) {
		if (GlobTomb4.VetKeysToStop[i]) {
			if (FramePassati >= TempoDelay ) {			
				if (i == 11) {
					// tasto 0

					Colonna=1;
					Riga=3;
				}else {
					ValoreTasto = i -2;
					Riga = ValoreTasto / 3;
					Colonna = ValoreTasto % 3;
				}

				TastiInput |= 0x40;
				TestNuovoTasto=true;

				break;
			}
		}
	}

	if (TestNuovoTasto == true) {
		pKeyPad->LastFrameKey = *GlobTomb4.pAdr->pFrameCounter;
	}

	// calcolare nuovo indice
	pKeyPad->IndiceTasto = Riga*3 + Colonna;

	// vedere se e' stato prmeuto # o tasto canc (per cancellare visore)
	if (FramePassati >= TempoDelay) {
		if (TastoVKPremuto(VK_BACK)==true  || 
			(pKeyPad->IndiceTasto == 9 && (TastiInput & 0x40) != 0)) {
			pKeyPad->IndiceTasto = 9;

			// premuto tasto # per cancellare l'ultimo carattere		
			if (pKeyPad->TotInseriti > 0) pKeyPad->TotInseriti--;
			pKeyPad->LastFrameKey= *GlobTomb4.pAdr->pFrameCounter;
			TestPremutoTasto=true;
		}
	}

	// vedere se e' stato premuto ENTER oppure tasto asterisco
	
	if (FramePassati >= TempoDelay) {
		if (GlobTomb4.VetKeysToStop[28] != 0 ||
			((TastiInput & 0x40) != 0 && pKeyPad->IndiceTasto == 11)) {

			// prendere numero e uscire
			pKeyPad->IndiceTasto = 11;

			ValoreTasto =0;
			Moltiplicatore=1;
			for (i=pKeyPad->TotInseriti-1;i>=0;i--) {
				ValoreTasto += Moltiplicatore * pKeyPad->VetInseriti[i];
				Moltiplicatore *= 10;
			}
			pKeyPad->ValoreInserito = ValoreTasto;

			pKeyPad->LastFrameKey= *GlobTomb4.pAdr->pFrameCounter;
			TestPremutoTasto=true;



		}
	}
	// se selezione e' diversa da precedente, cambiare carattere evidenziato
	if (pKeyPad->IndiceTasto != pKeyPad->IndiceTastoOld ) {
		// se c'era una selezione precenete, rimettere a posto quel tasto
		if (pKeyPad->IndiceTastoOld != -1) {
			CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTastoOld,0 ,true); 
			
		}
	}
	// evidenziare tasto selezionato
	// adesso abbassare la mesh del tasto, in modo da mostrare
	// quella nera
	CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTasto,0  ,false);
	CambiaTasto(pKeyPad->pStartMeshTree,pKeyPad->IndiceTasto + 12,10,false);

	// vedere se e' stato premuto ctrl
	if ((TastiInput & 0x40) != 0 && FramePassati >= TempoDelay) {
		ValoreTasto=11;
		if (pKeyPad->IndiceTasto < 9) {
			ValoreTasto = pKeyPad->IndiceTasto +1;
		}
		if (pKeyPad->IndiceTasto == 10) ValoreTasto = 0;
		MaxCifre = 4;
		// controllare tasti permessi nel caso ci sia ocb > 10000
		TestIgnora=false;
		if (pKeyPad->ValoreAtteso > 9999) { 
			MaxCifre =1;
			Range = pKeyPad->ValoreAtteso - 10000;
			if (ValoreTasto > Range ||
				(ValoreTasto == 0 && Range < 10)) TestIgnora=true;
		}

			
		if (ValoreTasto < 10  && 
			pKeyPad->TotInseriti < MaxCifre && 
			TestIgnora==false) {
			// qui ci sarebbe da fare animazione, ma per ora prendere solo tasto
			// e visuslizzare numero
			// inserirlo in prima posizione
			i=pKeyPad->TotInseriti;
			pKeyPad->VetInseriti[i] = (BYTE) ValoreTasto;
			pKeyPad->TotInseriti++;
			pKeyPad->LastFrameKey = *GlobTomb4.pAdr->pFrameCounter;
			pKeyPad->Fase = FKP_ANIMAZIONE_TASTO;
			TestPremutoTasto=true;
		}
		
	}

	pKeyPad->IndiceTastoOld = pKeyPad->IndiceTasto;

	if (GlobTomb4.VetKeysToStop[1]) {
		// premuto escape
		pKeyPad->Fase = FKP_ATTENDI_FINE_ESCAPE;
		pKeyPad->ValoreInserito =-1;


	}

	if (TestPremutoTasto==true) {
		// attivare animazione di tasto
		pKeyPad->CordYAnimazione = 0;
		pKeyPad->IncYAnimazione=2;
		pKeyPad->Fase = FKP_ANIMAZIONE_TASTO;
		if (pKeyPad->ScriptSettings.ClickSound != SCRIPT_IGNORE)
				SoundEffect(pKeyPad->ScriptSettings.ClickSound ,0,0);

	}
	/*
	if (pKeyPad->Fase == FKP_ANIMAZIONE_TASTO) {
		EseguiAnimazione(197,0);
	}
	*/

	return 0;
}



void GestioneSwitchKeyPad(StrItemTr4 *pSwitch)
{
	StrKeyPad *pKeyPad;

	pKeyPad = &GlobTomb4.BaseKeyPad;

	if (pKeyPad->TestAttivo==true) return;

	GestioneKeyPad(TKP_SWITCH, pSwitch->OcbCode & 0x3fff, pSwitch->SlotID );

}

bool VerificaSwitchKeypad(void)
{
	// chiamata da controllo switch
	// se keypad e' attivo, e' di tipo switch ed e' stato impostato
	// tesdtattivatriggerswitch
	// allora rimuovere keypad e restituire true
	StrKeyPad *pKeyPad;
	pKeyPad  = & GlobTomb4.BaseKeyPad;

	if (pKeyPad->TestAttivo== true && pKeyPad->TipoKeyPad == TKP_SWITCH &&
		pKeyPad->TestAttivaTriggerSwitch==true) {
		//ok
		pKeyPad->Fase = FKP_RIMUOVI;
		GestioneKeyPad(TKP_SWITCH,0,0);
		return true;
	}
	return false;


}

// chiamata quando lara e' davanti a switch
// preme ctrl e c'e' un ocb per switch trng con animazione
void GestioneSwitchClick(short IndiceSwitch, StrItemTr4 *pSwitch)
{

	WORD ValOcb;
	int Anim;
	WORD StateIdPrima;
	StrItemTr4 *pLara;


	ValOcb = pSwitch->OcbCode;
	Anim = ValOcb & 0x0fff;

	if (ValOcb & OCB_SW_INVERSE) {
		StateIdPrima = 1;
	}else {
		StateIdPrima = 0;
	}

	pLara = GlobTomb4.pAdr->pLara;

	if (ValOcb &  OCB_SW_FLIP_FLOP) {
		// e' flip flop
		if (pSwitch->StateIdCurrent != StateIdPrima) {
			// e' gia' attivo, allora usare animazione +1
			Anim++;
			StateIdPrima ^=1;
			
		}
		// mettere 0 se era 1 e viceversa
		pSwitch->StateIdNext = StateIdPrima ^ 1;
		pLara->AnimationNow = Anim;
		pLara->StateIdCurrent = 062; // state id pickup
		return;
	}
	// non e' flipflop
	// eseguirlo solo se non ancora attivato
	if (pSwitch->StateIdCurrent == StateIdPrima) {
		pSwitch->StateIdNext = StateIdPrima ^ 1;
		pLara->AnimationNow = Anim;
		pLara->StateIdCurrent = 062; // state id pickup
	}


}




