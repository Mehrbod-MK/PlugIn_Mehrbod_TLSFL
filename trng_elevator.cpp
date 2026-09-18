
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

extern TYPE_DoBloodSplat DoBloodSplat;
extern TYPE_GetRandomControl GetRandomControl;
extern TYPE_OnTwoBlockPlatform OnTwoBlockPlatform;
extern TYPE_GetFloor GetFloor;
extern TYPE_GetCeiling GetCeiling;
extern TYPE_AddActiveItem AddActiveItem;
// -> last declare line
void TrovaAbsPosition(StrItemTr4 *pItem, StrMovePosition *pMove, int NMesh);
// -> last declare line
void ShowMsgDebug(TCHAR *szFormat, ...);
int CreaNuovaAzioneProgressiva(void);
void AggiungiItemMosso(WORD Indice);
extern TYPE_SoundEffect SoundEffect;
bool InviaLog(char *pMessaggio);
void AggiornaPosizioneItem(short ItemIndex, 
						   DWORD CordX, int CordY, 
						   DWORD CordZ, int ExtraY);
int GetNgleIndice(int TombIndice);
WORD GetAlignedOrient(short Orient, bool TestForceHortogonal, int *pGap);
bool AddTabLogScript(void);
int EsecuzioneActionTrigger(WORD PluginID, WORD AzioneTimer, int ItemIndex, WORD Flags);
void SubTabLogScript(void);
void FermaSoundEffect(int IndiceAbs);
void EseguiAnimazione(WORD NumeroAnimazione, int IndiceSlot, bool TestSetNextStateId);
int GestioneKeyPad(int TipoKeyPad, WORD OcbCode, WORD Slot);


// -> last declare line


// ++++++ functions +++++++++++++++
// #TAG_ADDING_NEW_FUNCTIONS#
// -> last added function bodies
// -> last added function bodies
void AddEffectBlood(StrItemTr4 *pItem,  int NMesh, 
					int DispX, int DispY, int DispZ, WORD Flags)
{

	StrMovePosition  TriRec;
	WORD Orient;

	TriRec.RelX = DispX;
	TriRec.RelY = DispY;
	TriRec.RelZ = DispZ;


	
	Orient = pItem->OrientationV;
	if (Flags & FADD_VORIENT_90) pItem->OrientationV += 0x4000;
	if (Flags & FADD_VORIENT_180) pItem->OrientationV += 0x7fff;
	if (Flags & FADD_VORIENT_270) pItem->OrientationV -= 0x4000;
	TrovaAbsPosition(pItem, &TriRec, NMesh);
	pItem->OrientationV = Orient;
	
	Orient = pItem->OrientationH;
	if (Flags & FADD_ROTATE_90) Orient += 0x4000;
	if (Flags & FADD_ROTATE_180) Orient += 0x8000;
	if (Flags & FADD_ROTATE_270) Orient += 0xC000;
	DoBloodSplat(TriRec.RelX, TriRec.RelY, TriRec.RelZ, 
				(GetRandomControl() & 3) + 3, 
				Orient, 
				pItem->Room);


}
// -> last added function bodies


// questa funziona crea l'azione progressiva peraprire o chiudere la porta

void GestionePortaAnimating(StrScriptElevator *pElevatore, bool TestApri)
{
	// aprire o chiudere porta, usare un azione progressiva
	// calcolare incrmeento

	int IndiceAzione;

	StrProgressiveAction *pAzione;
	StrInfoAnimDoor *pAnimDoor;
	StrItemTr4 *pPorta;	

	pAnimDoor = &pElevatore->AnimDoor;
	
	pPorta = &GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoor];

	IndiceAzione = CreaNuovaAzioneProgressiva();

	pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
	pAzione->ActionType = AZ_MOVE_ANIMATING;
	AggiungiItemMosso(pElevatore->IndexFirstDoor);
	pAzione->Arg2 = pAnimDoor->FlagsMov;
	pAzione->ItemIndex = pElevatore->IndexFirstDoor;
	pAzione->VetArg[3] = 0;  // prima esecuzione

	if (TestApri==true) {
		pAzione->VetArg[0] = pAnimDoor->Incremento;
		pAzione->VetArg[1] = pAnimDoor->CordAperta;
		SoundEffect(GlobTomb4.pBaseCustomize->VetCustSFX[TS_ANIMATING_DOOR_OPEN], 
				&pPorta->CordX, 0);
	}else {
		// chiuderla
		pAzione->VetArg[0] = -pAnimDoor->Incremento;
		pAzione->VetArg[1] = pAnimDoor->CordChiusa;
		SoundEffect(GlobTomb4.pBaseCustomize->VetCustSFX[TS_ANIMATING_DOOR_CLOSE], 
			&pPorta->CordX, 0);
	}

	
	

	// se c'e' porta secondaria crea un'altra azione progressiva
	// per muovere anche l'altra porta
	if (pElevatore->Flags & EF_DOUBLE_DOOR) {

		pAnimDoor = &pElevatore->AnimDoorSecondary;
		
		pPorta = &GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoorSecodnary];

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_MOVE_ANIMATING;
		AggiungiItemMosso(pElevatore->IndexFirstDoorSecodnary);
		pAzione->Arg2 = pAnimDoor->FlagsMov;
		pAzione->ItemIndex = pElevatore->IndexFirstDoorSecodnary;
		pAzione->VetArg[3] = 0;  // prima esecuzione

		if (TestApri==true) {
			pAzione->VetArg[0] = pAnimDoor->Incremento;
			pAzione->VetArg[1] = pAnimDoor->CordAperta;
		}else {
			// chiuderla
			pAzione->VetArg[0] = -pAnimDoor->Incremento;
			pAzione->VetArg[1] = pAnimDoor->CordChiusa;
		}
	}
	
}

// salva globtomb4.baseeventsnow  l'evento attuale
// nota: se e' gia' accaduto esattamente guuale lo ignora
void ImpostaEventoNow(short GT_Evento, short Parameter)
{
	StrBaseEventiNow *pBase;
	int i;

	pBase = &GlobTomb4.BaseEventiNow;

	for (i=0;i<pBase->TotEventi;i++) {
		if (pBase->VetEventi[i].GlobalTrigger == GT_Evento && 
			pBase->VetEventi[i].Parameter == Parameter) return;
	}

	// non c'era, aggiungerlo adesso
	i = pBase->TotEventi;
	if (i >= MAX_EVENTI_NOW) {
		InviaLog("ERROR: reached max number of Events for global trigger");
		return;
	}

	pBase->VetEventi[i].GlobalTrigger = GT_Evento;
	pBase->VetEventi[i].Parameter = Parameter;
	pBase->TotEventi++;



}

// imposta dati per movimento ascensore, e' stato appena impostato il numero
// del piano dove andare
void InitMovimentoAscensore(StrElevator *pAscensore, 
							StrScriptElevator *pElevatore)
{
	// prima stabilire direzione incremento
	int Incremento;

	if (pAscensore->FloorNow > pAscensore->FloorTarget)
		Incremento=+1;
	else
		Incremento=-1;

	// settare velocita' e coordinata finale
	pAscensore->IncY = Incremento * pElevatore->Speed;
	pAscensore->LastIncy = pAscensore->IncY;
	pElevatore->pItem->SpeedV = pAscensore->IncY;
	pAscensore->OrgYTarget = pElevatore->FirstFloorY - 
			256 * pElevatore->ClickDistance * pAscensore->FloorTarget;
	pAscensore->Status = EST_MOVIMENTO;
	SoundEffect(GlobTomb4.Suoni.ElevatorMove,&pElevatore->pItem->CordX,2);


}

// aggiorna posizione di ItemIndex (relativo ad ascensore) aggiungedo a coordinata y
// il valore IncY
// aggiorna anche numero di room in modo corretto
void AggiornaPosY(int ItemIndex, int IncY)
{
	StrItemTr4 *pItem;

	int  CordY;

	
	pItem = &GlobTomb4.pAdr->pVetItems[ItemIndex];
	CordY = pItem->CordY + IncY;

	AggiornaPosizioneItem(ItemIndex, 
		pItem->CordX,CordY,pItem->CordZ, -0x200);


}


// aggiunge sangue a lara alla mesh (joint) specificata
// se TestImmediato =true lo esegue subito, se invece e' false crea
// un azione progressiva
void CreaSangueLara(int IndiceMesh, 
					int DispX, int DispY, int DispZ, 
					bool TestImmediato, int DurataFrame, 
					WORD FrameEmit, WORD FramePausa)
{

	StrProgressiveAction *pAzione;
	int IndiceAzione;



	if (TestImmediato==true) {

		AddEffectBlood(GlobTomb4.pAdr->pLara,IndiceMesh,DispX,DispY,DispZ,0);

	}else {
		// creare azione progressiva

		IndiceAzione = CreaNuovaAzioneProgressiva();

		pAzione = &GlobTomb4.VetProgressiveActions[IndiceAzione];
		pAzione->ActionType = AZ_ADD_EFFECT;
		pAzione->ItemIndex = *GlobTomb4.pAdr->pLaraIndex;
		pAzione->Arg1 = DurataFrame;
		pAzione->VetArgShort[4] = -1;
		// metteere arg2 per emttere sangue su mesh attacco capelli
		// usare nuovo formato
		pAzione->Bytes.AlfaByte = ADD_BLOOD;
		pAzione->Bytes.BetaByte = IndiceMesh;
		pAzione->VetArgShort[3] = FADD_NONE;
		// mettere modifica coordinate
		pAzione->VetArgShort[0] = DispX;
		pAzione->VetArgShort[1] = DispY;
		pAzione->VetArgShort[2] = DispZ;
		pAzione->VetArgShort[10] = 0;
		pAzione->VetArgShort[11] = 0;

		pAzione->VetArgShort[6] = FrameEmit;
		pAzione->VetArgShort[8] = FramePausa;

		

	}
}
// funzione simile a quella usata per controllare attivazione
// di switch in gioco.
// modificata per lavorare con il finto keypad interno di ascensore
// restituisce:
// 0 = posizione sballata
// 1 = posizione ok (si puo' attivare keypad)
// 2 = lara si sta muovendo per allineanento

__declspec(naked) int SwitchCollision(short IndiceSwitch, StrItemTr4 *pLara)
{
	__asm {
	mov  ecx, [esp+04h]	; INIZIO PROCEDURA 909 ;  SIZE=0x290  ; (ARG_ESP + 0x4) 
	push ebx 
	push ebp 
	push esi 
	push edi 
	mov  ebp, [esp+18h]  ; (ARG_ESP + 0x8) 
	movsx  edi, cx 
	xor  ebx, ebx 
	lea  eax, [edi][edi*2] 
	lea  eax, [edi][eax*4] 
	lea  edx, [eax][eax*8] 
	lea  eax, [edi][edx*8] 
	mov edx, 7FE16Ch    ;Ptr_ZonaItems
	mov  edx, dword ptr [edx] 
	lea  eax, [eax][eax*2] 
	lea  esi, [edx][eax*2] 
	mov eax, 5355D8h    ;InputGameCommands
	mov  al, byte ptr  [eax] 
	test al, 40h 
	jz   go_swt1 
	cmp  word ptr [ebp+0eh], 02h 
	jnz  go_swt1 
	cmp  word ptr [ebp+14h], 67h 
	jnz  go_swt1 
	push eax
	mov eax, 80DEC2h   ;FlagsLaraHands
	cmp  word ptr [eax], bx 
	pop eax
	jz  go_swt2 

go_swt1: 
	push eax
	mov eax, 80DF04h   ;FlagsLara
	test byte ptr [eax], 20h 
	pop eax
	jz   go_FuoriPosizione 
	push eax
	mov eax, 80DFC0h   ;ObjectActive
	cmp  dword ptr [eax], edi 
	pop eax
	jnz  go_FuoriPosizione 
go_swt2: 
	push esi 
	mov eax, 4504E0h    ;GetBoundsAccurate
	call eax 

	add  esp, 04h 

	mov  dx, [eax] 
	sub  dx, 100h 
	push eax
	mov eax, 4ADE6Ch   ;LOC_4ade6c
	mov  word ptr [eax], dx 
	pop eax
	mov  dx, [eax+02h] 
	add  dx, 100h 
	push eax
	mov eax, 4ADE6Eh   ;LOC_4ade6e
	mov  word ptr [eax], dx 
	pop eax
	cmp  [esi+2ch], bx 
	mov  dx, [eax+08h] 
	jz   go_swt5 
	sub  dx, 200h 
	push eax
	mov eax, 4ADE74h   ;LOC_4ade74
	mov  word ptr [eax], dx 
	pop eax
	mov  dx, [eax+0ah] 
	add  dx, 200h 
	push eax
	mov eax, 4ADE76h   ;LOC_4ade76
	mov  word ptr [eax], dx 
	pop eax
	cmp  [esi+2ch], cx 
	jnz  go_swt4 
	movsx  eax, word ptr [eax+08h] 
	sub  eax, 100h 
	push ebx
	mov ebx, 4BF5D0h   ;LOC_4bf5d0
	mov  dword ptr [ebx], eax 
	pop ebx
	jmp  go_swt6 
go_swt4: 
	movsx  ecx, word ptr [eax+08h] 
	sub  ecx, 80h 
	push eax
	mov eax, 4BF5D0h   ;LOC_4bf5d0
	mov  dword ptr [eax], ecx 
	pop eax
	jmp  go_swt6 
go_swt5: 
	sub  dx, 0c8h 
	push eax
	mov eax, 4ADE74h   ;LOC_4ade74
	mov  word ptr [eax], dx 
	pop eax
	mov  cx, [eax+0ah] 
	add  cx, 0c8h 
	push eax
	mov eax, 4ADE76h   ;LOC_4ade76
	mov  word ptr [eax], cx 
	pop eax
	movsx  edx, word ptr [eax+08h] 
	sub  edx, 40h 
	push eax
	mov eax, 4BF5D0h   ;LOC_4bf5d0
	mov  dword ptr [eax], edx 
	pop eax
go_swt6: 
	push ebp 
	push esi 
	push  4ADE6Ch ;LOC_4ADE6C 
	mov eax, 4476A0h    ;TestLaraPosition
	call eax 
	add  esp, 0ch 
	test eax, eax 
	jz   go_FuoriPosizione 
	push ebp 
	push esi 
	push  4BF5C8h ;LOC_4BF5C8 
	mov eax, 447B50h    ;MoveLaraPosition
	call eax 
	add  esp, 0ch 
	test eax, eax 
	jz   go_AllineaInCorso 
	// lara e' in posizione perfetta
	mov  eax, 1
	jmp  Go_LaraArrivata
go_AllineaInCorso: 
	mov eax, 2
Go_LaraArrivata:
	push eax
	mov eax, 80DFC0h   ;ObjectActive
	mov  dword ptr [eax], edi 
	pop eax

	pop  edi 
	pop  esi 
	pop  ebp 
	pop  ebx 
	ret 
go_FuoriPosizione: 

	xor eax,eax
	pop  edi 
	pop  esi 
	pop  ebp 
	pop  ebx 
	ret 
	nop 
 

	}
}
// nuova gestione di twoblockplatform quando ci sono ascensori
void ControlTwoBlockPlatform(short ItemIndex)
{

	// prima localizzare record a cui corrisponde questo elevatore
	int i,Result;
	int Dif;
	int Indice;
	bool TestArrivato;


	int AltezzaLara;
	bool TestDentro;

	void *pFloor;

	bool TestAccucciata;
	WORD StateId;
	short NRoom;
	int Ceiling;
	StrElevator *pAscensore;

	int FloorY;
	int LaraY;
	int Differenza;
	bool TestSovrapposta;
	StrScriptElevator *pElevatore;
	int TotElevatori;

	StrRectPos *pRect;
	int NFloor;
	int NextFloor;
	WORD LaraOrient;

	WORD ValOcb;


	TotElevatori=GlobTomb4.BaseElevator.TotElelevators;
	for (i=0;i<TotElevatori;i++) {
		pElevatore = &GlobTomb4.BaseElevator.VetScriptElevators[i];
		if (pElevatore->IndiceElevatore == ItemIndex) break;
	}

	if (i == TotElevatori) return;
	pAscensore = &GlobTomb4.BaseElevator.VetAscensori[i];

	SHOW_FULL_DEBUG("Elevator=%d", 
		GetNgleIndice(pElevatore->IndiceElevatore),0);

	AddTabLogScript();

	// controllo fisso per collisione pavimento/soffitto

	if ((GlobTomb4.pAdr->pLara->CordY -0x400)  >=  (pElevatore->pItem->CordY - 0x800)) {
		// collisione pavimento
		pAscensore->Soffitto =0;
	}else {
		// collisione soffitto
		pAscensore->Soffitto = 0x800;
	}

	switch (pAscensore->Status) {
	case EST_OPEN_AND_STOP:

		SHOW_FULL_DEBUG("OPEN AND STOP",0,0);

		// arrivare a piano in modo stop and go
		// comando di arrivo al piano quando c'e' modalita' stop and go
		// impostare gia' ora prossimo piano
		// e aprire porta (se c'e')
		NextFloor = pAscensore->FloorNow;
		pElevatore->pItem->SpeedV = 0;
		if (pAscensore->LastIncy > 0) {
			// sta scendnedp
			NextFloor--;
			if (NextFloor < 0)  {
				// farlo rimbalzare pe tornare su
				NextFloor +=2;
				pAscensore->IncY = -pElevatore->Speed;
			}
		}else {
			// sta salendo
			NextFloor++;
			if (NextFloor >= pElevatore->TotFloors) {
				// e' arrivato in cima, ora rimbalza e torna giu
				NextFloor -=2;
				pAscensore->IncY = pElevatore->Speed;
			}
		}
		pElevatore->pItem->SpeedV = pAscensore->IncY;
		pAscensore->LastIncy = pAscensore->IncY;
		pAscensore->FloorTarget = NextFloor;

		// a livello teorico anche il modo stop and go potrebbe
		// richiedere multi door o single door
		if ((pElevatore->Flags & EF_MULTI_DOORS)!= 0 ||
			(pElevatore->Flags & EF_SINGLE_DOOR) !=0) {

			// aprire  porta
			if (pElevatore->Flags & EF_SINGLE_DOOR) {
				Indice = pElevatore->IndexFirstDoor;
				if (pElevatore->Flags & EF_ANIM_DOOR) {
					GestionePortaAnimating(pElevatore,true);
				}else {
					EsecuzioneActionTrigger(0, 0x011A, Indice, SCANF_DIRECT_CALL);
					
					if (pElevatore->Flags & EF_DOUBLE_DOOR) {
						if (pElevatore->IndexFirstDoorSecodnary != SCRIPT_IGNORE) {
							EsecuzioneActionTrigger(0, 0x011A, pElevatore->IndexFirstDoorSecodnary,SCANF_DIRECT_CALL);
							
						}
					}
				}

			}else {
				Indice = pElevatore->VetDoors[pAscensore->FloorNow];
				EsecuzioneActionTrigger(0, 0x011A, Indice,SCANF_DIRECT_CALL);
				
				if (pElevatore->Flags & EF_DOUBLE_DOOR) {
					// aprire anche la seconda porta
					if (pElevatore->VetDoorsSecondary[pAscensore->FloorNow] != SCRIPT_IGNORE) {
						Indice= pElevatore->VetDoorsSecondary[pAscensore->FloorNow];
						EsecuzioneActionTrigger(0, 0x011A, Indice,SCANF_DIRECT_CALL);
						
					}
				}
			}


			pAscensore->Delay = FRAME_SECONDO *3;
		}else {
			pAscensore->Delay = FRAME_SECONDO *2;
		}

		pAscensore->Status = EST_ATTENDI_DELAY;

		pAscensore->NextStatus = EST_CLOSE_AND_GO;
		break;
	case EST_CLOSE_AND_GO:

		SHOW_FULL_DEBUG("CLOSE AND GO",0,0);

		// mandare evento per GT_ELEVATOR_STARTS_FROM_FLOOR
		ImpostaEventoNow(GT_ELEVATOR_STARTS_FROM_FLOOR, 
					((pAscensore->FloorNow+1) << 12) | pElevatore->IndiceElevatore);

		// partire da piano in modo stop and go
		// chiudere porta (se c'e') attendere e poi iniziare movimento
		pElevatore->pItem->SpeedV = 0;
		if ((pElevatore->Flags & EF_MULTI_DOORS)!= 0 ||
			(pElevatore->Flags & EF_SINGLE_DOOR) !=0) {
			if (pElevatore->Flags & EF_SINGLE_DOOR) {
				Indice = pElevatore->IndexFirstDoor;
				if (pElevatore->Flags & EF_ANIM_DOOR) {
					GestionePortaAnimating(pElevatore,false);
				}else {
					EsecuzioneActionTrigger(0, 0x001A, Indice,SCANF_DIRECT_CALL);
					
					if (pElevatore->Flags & EF_DOUBLE_DOOR) {
						EsecuzioneActionTrigger(0, 0x001A, 
							pElevatore->IndexFirstDoorSecodnary  ,SCANF_DIRECT_CALL);
						
					}
				}
			}else {
				Indice = pElevatore->VetDoors[pAscensore->FloorNow];
				// chiudere  porta
				EsecuzioneActionTrigger(0, 0x001A, Indice, SCANF_DIRECT_CALL);
				
				if (pElevatore->Flags & EF_DOUBLE_DOOR) {
					EsecuzioneActionTrigger(0, 0x001A, 
						pElevatore->VetDoorsSecondary[pAscensore->FloorNow] ,SCANF_DIRECT_CALL);
					
				}
			}

			// ora attesa e poi movimento
			pAscensore->Status = EST_ATTENDI_DELAY;
			pAscensore->Delay = FRAME_SECONDO + FRAME_SECONDO/2;
			pAscensore->NextStatus = EST_INIZIO_MOVIMENTO;
		}else {
			// non c'e' porta allora iniziare subito movimento
			pAscensore->Status = EST_INIZIO_MOVIMENTO;
		}
		
		break;

	case EST_ATTENDI_KEYPAD:
		SHOW_FULL_DEBUG("WAIT KEYPAD input",0,0);

		GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;
		pElevatore->pItem->SpeedV = 0;
		// abbiamo avviato keypad, ora si deve attendere che abbia finito

		if (GlobTomb4.BaseKeyPad.TestAttivo == true) {
			SubTabLogScript();
			return;
		}
		// non e' piu' attivo, ora qui si dovrebbe prendere il valore
		// se c'e'
		NFloor=GlobTomb4.BaseKeyPad.ValoreInserito;
		if (NFloor == -1 || (NFloor-1) == pAscensore->FloorNow ) {
			pAscensore->Status = EST_ATTESA;
			SubTabLogScript();
			return;
		}
		NFloor--;
		pAscensore->FloorTarget = NFloor;

		// mandare evento per GT_ELEVATOR_STARTS_FROM_FLOOR
		ImpostaEventoNow(GT_ELEVATOR_STARTS_FROM_FLOOR, 
					((pAscensore->FloorNow+1) << 12) | pElevatore->IndiceElevatore);
		// e' stato inserito un piano valido
		// adesso bisognerebbe andare a quel piano..
		// anzi prima chiudere la porta magari 
		if ((pElevatore->Flags & EF_MULTI_DOORS)!= 0 ||
			(pElevatore->Flags & EF_SINGLE_DOOR) !=0) {
			if (pElevatore->Flags & EF_SINGLE_DOOR) {
				Indice = pElevatore->IndexFirstDoor;
				// chiudere porta
				if (pElevatore->Flags & EF_ANIM_DOOR) {
					// muovere porta animating
					GestionePortaAnimating(pElevatore, false);
				}else {
					// aprire porta tradizionale
					EsecuzioneActionTrigger(0, 0x001A, Indice, SCANF_DIRECT_CALL);
					
					if (pElevatore->Flags & EF_DOUBLE_DOOR) {
						EsecuzioneActionTrigger(0, 0x001A, pElevatore->IndexFirstDoorSecodnary ,SCANF_DIRECT_CALL);

						
					}
				}

			}else {
				Indice = pElevatore->VetDoors[pAscensore->FloorNow];
				// chiudere porta
				EsecuzioneActionTrigger(0, 0x001A, Indice, SCANF_DIRECT_CALL);
				
				if (pElevatore->Flags & EF_DOUBLE_DOOR) {
					EsecuzioneActionTrigger(0, 0x001A, 
						pElevatore->VetDoorsSecondary[pAscensore->FloorNow], SCANF_DIRECT_CALL);
					
				}


			}



			pAscensore->Status = EST_ATTENDI_DELAY;
			pAscensore->Delay = FRAME_SECONDO + FRAME_SECONDO/2;
			pAscensore->NextStatus = EST_INIZIO_MOVIMENTO;
		}else {
			InitMovimentoAscensore(pAscensore, pElevatore);

		}
		break;
	case EST_MOVIMENTO:

		SHOW_FULL_DEBUG("Moving: Floor Now=%d  Speed=%d",
			pAscensore->FloorNow, pAscensore->IncY);

		// controllare se abbiamo raggiunto piano
		pElevatore->pItem->SpeedV = pAscensore->LastIncy;
		TestArrivato=false;
		if (pAscensore->IncY > 0) {
			if (pElevatore->pItem->CordY >= pAscensore->OrgYTarget) TestArrivato=true;
		}else {
			if (pElevatore->pItem->CordY <= pAscensore->OrgYTarget) TestArrivato=true;
		}
		if (TestArrivato==true) {
			// aggiornare ultimo incremento in modo che conduca esattamente 
			// al piano corretto
			pAscensore->IncY =  pAscensore->OrgYTarget - pElevatore->pItem->CordY;
		}
		// calcolare se lara e' dentro questo ascensore:
		TestDentro=false;
		TestSovrapposta=false;
		LaraY = GlobTomb4.pAdr->pLara->CordY;
		FloorY = pElevatore->pItem->CordY +30;

		if (LaraY <= FloorY && LaraY > (FloorY-0x700)) {
			TestDentro=true;
		}

		TestSovrapposta = OnTwoBlockPlatform(pElevatore->pItem, 
					GlobTomb4.pAdr->pLara->CordX, 
					GlobTomb4.pAdr->pLara->CordZ);

		// vedere se lara e' accucciata

		StateId= GlobTomb4.pAdr->pLara->StateIdCurrent;
		switch (StateId) {

		case 0x47:
		case 0x48:
		case 0x50:
		case 0x51:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x69:
		case 0x6A:

			TestAccucciata=true;
			AltezzaLara = 0x150;
			break;
		case 56:
		case 57:
		case 58:
		case 59:
		case 60:
		case 61:
			TestAccucciata=false;
			AltezzaLara = 0x250;
			break;
		default:
			TestAccucciata=false;
			AltezzaLara= 0x300;
		}

		// se lara e' dentroaggiornare anche sua coordinata

		if (TestSovrapposta==true && TestDentro == true) {
			
			GlobTomb4.pAdr->pLara->CordY += pAscensore->IncY;

		}

		// effettuare aggiornamento di tutta la roba prevista
		AggiornaPosY(pElevatore->IndiceElevatore, pAscensore->IncY);

		// se c'e' porta singola, aggiornarla
		if (pElevatore->Flags & EF_SINGLE_DOOR) {
			AggiornaPosY(pElevatore->IndexFirstDoor, pAscensore->IncY);
			if (pElevatore->Flags & EF_DOUBLE_DOOR) {
				AggiornaPosY(pElevatore->IndexFirstDoorSecodnary, pAscensore->IncY);

			}
		}
		// se c'e' keypad inerno aggiornarlo
		
		if (pElevatore->Flags & EF_INNER_KEYPAD) {
			AggiornaPosY(pElevatore->KeyPadIndex, pAscensore->IncY);
			AggiungiItemMosso(pElevatore->KeyPadIndex);
		}
		

		// se ci sono frame, aggironarli
		for (i=0;i<pElevatore->TotFrameItems;i++) {
			AggiornaPosY(pElevatore->VetFrameItems[i],pAscensore->IncY);
		}


		if (TestArrivato==true) {
			//fermare suono di movimento
			pElevatore->pItem->SpeedV = 0;		
			FermaSoundEffect(GlobTomb4.Suoni.ElevatorMove);
			
			pAscensore->FloorNow = pAscensore->FloorTarget;

			// se non c'e' porta effettuare suono di arrivo piano
			if ((pElevatore->Flags & (EF_MULTI_DOORS + EF_SINGLE_DOOR))==0) {
				SoundEffect(GlobTomb4.Suoni.ElevatorStop ,
							&pElevatore->pItem->CordX,0);
			}

			ImpostaEventoNow(GT_ELEVATOR_STOPS_AT_FLOOR, 
					((pAscensore->FloorNow+1) << 12) | pElevatore->IndiceElevatore);

			if (pElevatore->Flags & EF_MODE_STOP_AND_GO) {
				pAscensore->Status = EST_OPEN_AND_STOP;
				SubTabLogScript();
				return;
			}

			if (pElevatore->Flags & EF_MODE_YO_YO) {
				// andare al prossimo piano, o il primo o l'ultimo
				if (pAscensore->FloorNow == 0) {
					// andare all'ultimo piano
					pAscensore->FloorTarget = pElevatore->TotFloors -1;
				}else {
					// andare al primo piano
					pAscensore->FloorTarget = 0;
				}
				pAscensore->Status = EST_INIZIO_MOVIMENTO;
				SubTabLogScript();
				return;
			}

			pAscensore->Status = EST_ATTESA;
			// aprire porta di questo piano?
			if (pElevatore->Flags & EF_MULTI_DOORS) {
				EsecuzioneActionTrigger(0, 0x011A,pElevatore->VetDoors[pAscensore->FloorNow], SCANF_DIRECT_CALL);
				
				if (pElevatore->Flags & EF_DOUBLE_DOOR) {
					EsecuzioneActionTrigger(0, 0x011A,pElevatore->
							VetDoorsSecondary[pAscensore->FloorNow], SCANF_DIRECT_CALL);
					
				}
			}
			if (pElevatore->Flags & EF_SINGLE_DOOR) {

				// aprire porta
				// aprire porta
				if (pElevatore->Flags & EF_ANIM_DOOR) {
					// muovere porta animating
					GestionePortaAnimating(pElevatore, true);
				}else {
					// aprire porta tradizionale
					EsecuzioneActionTrigger(0, 0x011A,pElevatore->IndexFirstDoor , SCANF_DIRECT_CALL);
					
				}			
			}
	
				
		} else {
			// movimento in corso



			SoundEffect(GlobTomb4.Suoni.ElevatorMove, 
						&pElevatore->pItem->CordX,2);
			// controllare se ascensore sta schiacciando lara
			// se lara e' morta (o invulnerabile) non fare niente
			if (*GlobTomb4.pAdr->pFlagsLara & 0x08) break;
			if (TestDentro==true) break;
			if (GlobTomb4.pAdr->pLara->Health <=0 ) break;

			// fare analisi diverse a seconda se lara sta sopra pavimento o
			// sopra soffitto
			if (pAscensore->Soffitto == 0) {
	
				// se ascenosre sta andando verso l'alto nessun schicciamento
				if (pAscensore->IncY < 0) break;
				if (TestSovrapposta==false) break;


				// calcolo cordy di elevatore considerando la parte
				// bassa che e' circa un click piu' sotto
				FloorY = pElevatore->pItem->CordY + 0x100;
				// calcolare cordy di lara sulla testa
				// qui bisognera' cambiare qualcosda se lara e' accucciata

				LaraY = GlobTomb4.pAdr->pLara->CordY - AltezzaLara;



				// lara e' su pavimento o sotto
				// se lara e' sopra pavimento niente
				if (LaraY > FloorY ) break;



				// c'e' contatto
				// se pero' lara non sta toccando il pacvimento allora
				// fare analisi particolare per lara aggrappata
				if (GlobTomb4.pAdr->pLara->HeightFloor <= GlobTomb4.pAdr->pLara->CordY) {
					
					// ok, ammazzare lara per schicciamwento e fermare ascensore
					pAscensore->Status = EST_ATTESA;
					CreaSangueLara(8, 0,-80,0, false,2 * FRAME_SECONDO,1,29);



					EseguiAnimazione(139,0,false);
					GlobTomb4.pAdr->pLara->Health=0;
				}else {
					// spingerla giu' e togliere ctrl
					*GlobTomb4.pAdr->pInputExtGameCommands &= ~0x40;

					// creare azione progressiva per mostrare sangue
					// sulla testa di lara
					CreaSangueLara(8, 0,-80,0, false,3 * FRAME_SECONDO,1,29);
					SoundEffect(GlobTomb4.pBaseCustomize->
							VetCustSFX[TS_ELEVATOR_SQUASHED_LARA],
							NULL,2);

					GlobTomb4.pAdr->pLara->CordY += 0x100;
					if (GlobTomb4.pAdr->pLara->SpeedV == 0) 
						GlobTomb4.pAdr->pLara->SpeedV = pAscensore->IncY*2;

					EseguiAnimazione(103,0,false);
				

				}
				break;
			}else {
				// vedere se lara e' sul soffitto di ascensore
				// e viene schiacciata addosso ad un muro (complicato)
				if (pAscensore->LastIncy  > 0 ) break;
				// vedere pero' se e' proprio sopra ascensore
				if (TestSovrapposta==false) break;

				Dif = (FloorY - 0x800) - LaraY;
				
				if (Dif >=-0x100 && Dif < 0x1a0) {
					// lara e 'sopra soffitto
					// se lara e' attaccata a qualche cosa
					// costringerla a staccarsi
					if (*GlobTomb4.pAdr->pFlagsLaraHands  == 1 && TestAccucciata==false) {
						// cambiare animazione per costringerla 
						// a staccarsi
						EseguiAnimazione(103,0,false);

					

					}
				}
				// se lara non e' nemmeno nella stanza di ultimo piano
				// allora ifgnorare
				if (GlobTomb4.pAdr->pLara->Room != pElevatore->MaxRoom) break;

				// c'e' collisione sul soffitto quindi lara sta sopra 
				// ascensore


				// vedere distanza che c'e' tra piedi di lara 
				// e superfice di soffitto ascensore

				Dif = (FloorY - 0x800) - LaraY;
				
				if (Dif >=0 && Dif < 0x40) {
					// lara e 'sopra soffitto
					// se lara e' attaccata a qualche cosa
					// costringerla a staccarsi
					if (*GlobTomb4.pAdr->pFlagsLaraHands  == 1 && 
						TestAccucciata == false &&  Dif < 0x20) {
						// cambiare animazione per costringerla 
						// a staccarsi
						EseguiAnimazione(98,0,false);
						CreaSangueLara(6, 0,0,-30, false,3 * FRAME_SECONDO,1,29);
						CreaSangueLara(3, 0,0,-30, false,3 * FRAME_SECONDO,1,29);


					

					}
	
					// trovare altezza soffitto
					NRoom =GlobTomb4.pAdr->pLara->Room;
					pFloor = GetFloor(GlobTomb4.pAdr->pLara->CordX, 
						GlobTomb4.pAdr->pLara->CordY  , 
						GlobTomb4.pAdr->pLara->CordZ, &NRoom);

					Ceiling = GetCeiling(pFloor, GlobTomb4.pAdr->pLara->CordX, 
							GlobTomb4.pAdr->pLara->CordY , GlobTomb4.pAdr->pLara->CordZ);


					Dif = LaraY - Ceiling;

					if (Dif <= (AltezzaLara- 0x50)) {
						// fare morire lara

						CreaSangueLara(8, 0,-80,-40, false,2 * FRAME_SECONDO,1,29);

						pAscensore->Status = EST_ATTESA;
						EseguiAnimazione(139,0, false);
						GlobTomb4.pAdr->pLara->Health=0;
					}
					
				}

					

			}
		}
		break;
	case EST_ATTENDI_DELAY:
		SHOW_FULL_DEBUG("Wait delay",0,0);

		GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;
		pElevatore->pItem->SpeedV = 0;
		pAscensore->Delay --;
		if (pAscensore->Delay) break;

		pAscensore->Status = pAscensore->NextStatus;
		break;
	case EST_INIZIO_MOVIMENTO:
		SHOW_FULL_DEBUG("Starting movement",0,0);

		GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;
		InitMovimentoAscensore(pAscensore, pElevatore);
		break;

	case EST_ATTENDI_ALLINEAMENTO_LARA:
		SHOW_FULL_DEBUG("Wait Lara's alignment",0,0);

		// lara si sta muovendo per avvicinarsi a giusta posizone
		pElevatore->pItem->SpeedV = 0;
		GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;
		if (SwitchCollision(pElevatore->KeyPadIndex, GlobTomb4.pAdr->pLara)==1) {
			// ora si puo' mostrare keypad
			
			// impostare ocb per avere richiesta di singolo numero
			// e impostare i limiti acettabili
			ValOcb = 10000 +  pElevatore->TotFloors;

			// ok, attivare keypad e cambiare fase in attesa inputkeypad
			GlobTomb4.BaseKeyPad.LastFrameExit=0;
			GestioneKeyPad(TKP_ELEVATOR, ValOcb, 
						GlobTomb4.pAdr->pVetItems[pElevatore->KeyPadIndex].SlotID);

			pAscensore->Status = EST_ATTENDI_KEYPAD;
		}
		break;
	
	case EST_ATTESA:
		SHOW_FULL_DEBUG("Waiting...",0,0);

		// controllare se lara sta premendo ctrl davanti a keypad interno
		if ((pElevatore->Flags & EF_INNER_KEYPAD)== 0) break;

		GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;
		// controllare solo se utente ha premuto azione
		if ((*GlobTomb4.pAdr->pInputGameCommands & 0x40) ==0) break;
		// vedere se lara e' nei limiti previsti

		pElevatore->pItem->SpeedV = 0;
		// prima cosa vedee se laray e' uguale a ascensore y
		if (GlobTomb4.pAdr->pLara->CordY != pElevatore->pItem->CordY) break;

		// vedere se l'orientamento e' corretto

		LaraOrient=GetAlignedOrient(GlobTomb4.pAdr->pLara->OrientationH, true, &Differenza);	
		if (LaraOrient != pElevatore->OrientLaraPerKeyPad) break;
		// tutto ok, fin qui, ora vedere se le coordinate sono quelle giuste
		pRect = &pElevatore->LimitiBloccoKeypad;
		if (GlobTomb4.pAdr->pLara->CordX >= pRect->MinX &&
			GlobTomb4.pAdr->pLara->CordX <= pRect->MaxX &&
			GlobTomb4.pAdr->pLara->CordZ >= pRect->MinZ &&
			GlobTomb4.pAdr->pLara->CordZ <= pRect->MaxZ) {
			Result = SwitchCollision(pElevatore->KeyPadIndex, GlobTomb4.pAdr->pLara);
			switch (Result) {

			case 2:
				// lara e' in fase di avvicinamento

				pAscensore->Status = EST_ATTENDI_ALLINEAMENTO_LARA;
				break;
			case 1:
				// e' gia' in posizione perfetta

				
				// impostare ocb per avere richiesta di singolo numero
				// e impostare i limiti acettabili
				ValOcb = 10000 +  pElevatore->TotFloors;
				GlobTomb4.BaseKeyPad.IndiceKeypad = pElevatore->KeyPadIndex;

				// ok, attivare keypad e cambiare fase in attesa inputkeypad
				GestioneKeyPad(TKP_ELEVATOR, ValOcb, 
							GlobTomb4.pAdr->pVetItems[pElevatore->KeyPadIndex].SlotID);

				pAscensore->Status = EST_ATTENDI_KEYPAD;
				break;
			}
			
		}
		break;
	}

	
	SubTabLogScript();

}

// viene chiamata per verificare se biosgna cambiare cordy di twoblockplatform
// in modo da simulare soffitto di ascensore
// imposta nella variabile globale GapSoffittoY il valore da sottrarre
// a coordinata y di elevato (o 0x800 oppure 0)
void ImpostaGapSoffitto(StrItemTr4 *pItem)
{
	WORD i;
	
	GlobTomb4.BaseElevator.GapSoffittoY=0;

	for (i=0;i<GlobTomb4.BaseElevator.TotElelevators ;i++) {
		if (pItem == GlobTomb4.BaseElevator.VetScriptElevators[i].pItem) {
			GlobTomb4.BaseElevator.GapSoffittoY = GlobTomb4.BaseElevator.VetAscensori[i].Soffitto;
			return;
		}
	}

}


// sostituisce TwoBlockPlatformFloor
__declspec(naked) void TwoBlockPlatformFloor(void)
{
	__asm {
	mov  eax, [esp+04h] ; indirizzo di record elevatore
	push eax
	call  ImpostaGapSoffitto
	add  esp,4

	mov  eax, [esp+10h]	; INIZIO PROCEDURA 138 ;  SIZE=0x50  ; (ARG_ESP + 0x10) 
	mov  ecx, [esp+08h]  ; (ARG_ESP + 0x8) 
	push esi 
	mov  esi, [esp+08h]  ; (ARG_ESP + 0x4) 
	push eax   ;cordz 
	push ecx   ;cordx 
	push esi     ;record 
	mov eax, 415A10h    ;OnTwoBlockPlatform
	call eax 
	add  esp, 0ch 
	test eax, eax 
	jz   Salto1 
	mov  eax, [esi+44h]    ;qui c'e' cord y di piattaform 
	// qui aumentare per mettere collisione su soffitto di elevatore
	sub  eax, dword ptr [GlobTomb4.BaseElevator.GapSoffittoY]

	mov  ecx, [esp+10h]  ; (ARG_ESP + 0xC)   ;cord y 
	lea  edx, [eax+20h]  ;lo si abbassa di 32 (chissa' perche') 
	cmp  ecx, edx   ; 
	jg   Salto1 
	;se la coordinata y e' minore o uguale vuol dire che sta toccando superfice 
	mov  ecx, [esp+18h]  ; (ARG_ESP + 0x14) 
	cmp  eax, [ecx]    ;questo non so cos'e' sembra un altra cordy 
	jge  Salto1 
	mov  [ecx], eax    ;forza la nuova posizione y 
	push eax
	mov eax, 7FE0E0h   ;TestOverPlatform
	mov  dword ptr [eax], 01h 
	pop eax
	push eax
	mov eax, 7FE170h   ;ValoreHeight
	mov  dword ptr [eax], 00h 
	pop eax
Salto1: 
	pop  esi 
	ret 
 
	}
}

// sostituisce TwoBlockPlatformCeiling: 
//	;esp+14= LONG* (puntatore a long)  nuova cordy
//	;esp+10=long  cordz
//	;esp+C=long   cordy
//	;esp+8=long    cordx
//	;esp+4 = ITEM_INFO* di piattaforma
__declspec(naked) void TwoBlockPlatformCeiling(void)
{
	__asm {
	mov  eax, [esp+04h] ; indirizzo di record elevatore
	push eax
	call  ImpostaGapSoffitto
	add  esp,4

	mov  eax, [esp+10h]	; INIZIO PROCEDURA 139 ;  SIZE=0x40  ; (ARG_ESP + 0x10) 
	mov  ecx, [esp+08h]  ; (ARG_ESP + 0x8) 
	push esi 
	mov  esi, [esp+08h]  ; (ARG_ESP + 0x4) 
	push eax 
	push ecx 
	push esi 
	mov eax, 415A10h    ;OnTwoBlockPlatform
	call eax 
	add  esp, 0ch 
	test eax, eax 
	jz   Salto1 
	mov  eax, [esi+44h]   ;cord y di piattaforma 
	// qui modificare cordy
	sub  eax, dword ptr [GlobTomb4.BaseElevator.GapSoffittoY]

	mov  ecx, [esp+10h]  ; (ARG_ESP + 0xC) 
	lea  edx, [eax+20h]    ;solo 32 ? 
	cmp  ecx, edx 
	jle  Salto1 
	mov  ecx, [esp+18h]  ; (ARG_ESP + 0x14) 
	cmp  eax, [ecx] 
	jle  Salto1 
	// lara e' sotto piattaforma
	add  eax, 100h 
	mov  [ecx], eax 

Salto1: 
	pop  esi 
	ret 
 

	}
}

// imposta i dati usati per muovere porta anim door di elevatore
void CalcolaInfoAnimDoor(StrScriptElevator *pElevatore)
{

	StrItemTr4 *pPorta;
	WORD Orient;
	StrInfoAnimDoor *pSecondario;
	StrItemTr4 *pPorta2;
	bool TestSecondario;

	if (pElevatore->Flags & EF_DOUBLE_DOOR) {
		TestSecondario = true;
	}else {
		TestSecondario=false;
	}

	pPorta = &GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoor];
	if (TestSecondario) {
		pPorta2= &GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoorSecodnary];
	}

	Orient = (WORD) pPorta->OrientationH;
	pSecondario = &pElevatore->AnimDoorSecondary;

	if (TestSecondario == true) {
		// se ci sono due porte il movimento e' opposto
		// le porte devono scivolare verso l'esterno
		switch (Orient) {
		case 0x8000:
			// muovere verso nord
			pElevatore->AnimDoor.FlagsMov  = fmov_CordX;
			pElevatore->AnimDoor.Incremento  = +45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordX;
			pElevatore->AnimDoor.CordAperta = pPorta->CordX + 1024;
			// secondaria
			pSecondario->FlagsMov  = fmov_CordX;
			pSecondario->Incremento  = -45;
			pSecondario->CordChiusa = pPorta2->CordX;
			pSecondario->CordAperta = pPorta2->CordX - 1024;
			break;
		case 0xc000:
			// muovere verso est
			pElevatore->AnimDoor.FlagsMov  = fmov_CordZ;
			pElevatore->AnimDoor.Incremento  = -45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordZ;
			pElevatore->AnimDoor.CordAperta = pPorta->CordZ - 1024;

			// muovere verso est
			pSecondario->FlagsMov  = fmov_CordZ;
			pSecondario->Incremento  =45;
			pSecondario->CordChiusa = pPorta->CordZ;
			pSecondario->CordAperta = pPorta->CordZ + 1024;
			break;
		case 0x0000:
			// muovere verso sud
			pElevatore->AnimDoor.FlagsMov  = fmov_CordX;
			pElevatore->AnimDoor.Incremento  = -45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordX;
			pElevatore->AnimDoor.CordAperta = pPorta->CordX - 1024;

			// muovere verso sud
			pSecondario->FlagsMov  = fmov_CordX;
			pSecondario->Incremento  = 45;
			pSecondario->CordChiusa = pPorta2->CordX;
			pSecondario->CordAperta = pPorta2->CordX + 1024;

			break;
		case 0x4000:
			// muovere verso ovest
			pElevatore->AnimDoor.FlagsMov  = fmov_CordZ;
			pElevatore->AnimDoor.Incremento  = +45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordZ;
			pElevatore->AnimDoor.CordAperta = pPorta->CordZ + 1024;
			// muovere verso ovest
			pSecondario->FlagsMov  = fmov_CordZ;
			pSecondario->Incremento  = -45;
			pSecondario->CordChiusa = pPorta2->CordZ;
			pSecondario->CordAperta = pPorta2->CordZ - 1024;

			break;
		}



	}else {
		// normale

		switch (Orient) {
		case 0x8000:
			// muovere verso nord
			pElevatore->AnimDoor.FlagsMov  = fmov_CordX;
			pElevatore->AnimDoor.Incremento  = -45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordX;
			pElevatore->AnimDoor.CordAperta = pPorta->CordX - 1024;


			break;
		case 0xc000:
			// muovere verso est
			pElevatore->AnimDoor.FlagsMov  = fmov_CordZ;
			pElevatore->AnimDoor.Incremento  =45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordZ;
			pElevatore->AnimDoor.CordAperta = pPorta->CordZ + 1024;
			break;
		case 0x0000:
			// muovere verso sud
			pElevatore->AnimDoor.FlagsMov  = fmov_CordX;
			pElevatore->AnimDoor.Incremento  = 45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordX;
			pElevatore->AnimDoor.CordAperta = pPorta->CordX + 1024;
			break;
		case 0x4000:
			// muovere verso ovest
			pElevatore->AnimDoor.FlagsMov  = fmov_CordZ;
			pElevatore->AnimDoor.Incremento  = -45;
			pElevatore->AnimDoor.CordChiusa = pPorta->CordZ;
			pElevatore->AnimDoor.CordAperta = pPorta->CordZ - 1024;
			break;
		}
	}

}

int GetNgleRoomIndice(int TombIndice)
{
	int i;
	for (i=0;i<MAX_ROOMS;i++) {
		if (GlobTomb4.VetRemapRooms[i] == TombIndice) return i;
	
	}
	return -1;


}

// inizializza valori per ascnsori. Preleva i dati da script elevator
// (gia' scnadito) e li converte per il livello appena caricato
// #callback#
void PreparaElevator(void)
{


	WORD ItemIndex;
	short ValShort;
	StrGlobalTrigger *pGlobal;
	StrElevator *pAscensore;
	StrScriptElevator *pElevatore;
	WORD TotElevators;
	int IdElevatore;
	WORD NSlot;
	WORD i,j,z;
	void *pFloor;
	int Indice;
	int DoorY;
	bool TestSavegame;
	static char *VetMode[] = {"New Level", "Load Savegame"};
	StrItemTr4 *pDoor;
	DWORD DoorX, DoorZ;
	int GapY;
	StrItemTr4 *pItem;
	StrRectPos *pRect;
	int OrgX,OrgZ;


	if (GlobTomb4.BaseElevator.TotElelevators == 0) return;
	// se = 4 allora e' stato caricato un savegame
	if (GlobTomb4.TestAsSavegame) 
		TestSavegame=true;
	else
		TestSavegame=false;

	AddTabLogScript();

	SHOW_DEBUG("Initialize Elevators. Mode=%s", VetMode[(int) TestSavegame],0);
	AddTabLogScript();
	
	TotElevators = GlobTomb4.BaseElevator.TotElelevators;

	for (i=0;i<TotElevators;i++) {
		pElevatore = &GlobTomb4.BaseElevator.VetScriptElevators[i];
		pAscensore = &GlobTomb4.BaseElevator.VetAscensori[i];

		
		// convertire indice di elevatore
		ItemIndex = pElevatore->IndiceElevatore;
		IdElevatore = ItemIndex;

		SHOW_DEBUG("Elevator=%d", IdElevatore,0);
		AddTabLogScript();

		if (ItemIndex == SCRIPT_IGNORE) {
			SHOW_DEBUG("ERROR: missing index for elevator item in Elevator=IGNORE",
				0,0);
			SubTabLogScript();
			GlobTomb4.BaseElevator.TotElelevators =0;
			break;
		}

		pElevatore->IndiceElevatore = GlobTomb4.VetRemapObjects[ItemIndex];
		if (pElevatore->IndiceElevatore == SCRIPT_IGNORE) {

			SHOW_DEBUG("ERROR elevator: Wrong elevator index (%d) in Elevator=%d script dat",
				ItemIndex, ItemIndex);
			SubTabLogScript();
			GlobTomb4.BaseElevator.TotElelevators=0;
			break;
		}

		// convertire indice index door
		ItemIndex=pElevatore->IndexFirstDoor;
		if (ItemIndex != SCRIPT_IGNORE) {

			pElevatore->IndexFirstDoor = GlobTomb4.VetRemapObjects[ItemIndex];

			SHOW_DEBUG("Index first door: NgleIndex=%d  TombIndex=%d",
				ItemIndex, pElevatore->IndexFirstDoor);

			if (pElevatore->IndexFirstDoor  == SCRIPT_IGNORE) {

				SHOW_DEBUG("ERROR: Wrong first door index (%d) in Elevator=%d",
					ItemIndex, IdElevatore);

				SubTabLogScript();
				GlobTomb4.BaseElevator.TotElelevators=0;
				break;
			}	

			if (pElevatore->Flags & EF_DOUBLE_DOOR) {
				pElevatore->IndexFirstDoorSecodnary = 
					GlobTomb4.VetRemapObjects[ItemIndex+1];

				if (pElevatore->IndexFirstDoorSecodnary == SCRIPT_IGNORE) {
					SHOW_DEBUG("ERROR in Elevator command: cann't find the paired door of double door with main door index = %d",
						ItemIndex,0);

					SubTabLogScript();
					GlobTomb4.BaseElevator.TotElelevators=0;
					break;
					
				}
			}
			// imposta flag interno per segnalare se e' una porta animating
			// o una porta tradizionale
			// qui c'e' il crash
			NSlot = GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoor].SlotID;
			if (NSlot < 0x142 || NSlot > 0x14f) {
				// e' una porta animating
				pElevatore->Flags |= EF_ANIM_DOOR;

				// impostare la posizione e il tipo di incremento
				// per la porta anim
				CalcolaInfoAnimDoor(pElevatore);
				

			}

		}else {
			SHOW_FULL_DEBUG("No door for current elevator",0,0);
			pElevatore->Flags &= ~EF_MULTI_DOORS;

			pElevatore->Flags &= ~EF_SINGLE_DOOR;
		}
		// convertire indice keyupad (se presente)
		if (pElevatore->Flags & EF_INNER_KEYPAD) {
			ItemIndex = pElevatore->KeyPadIndex;
			SHOW_FULL_DEBUG("Required INNER_KEYPAD with index=%d",ItemIndex,0);

			if (ItemIndex == SCRIPT_IGNORE) {

				SHOW_DEBUG("ERROR: is missing index for inner keypad",0,0);

				pElevatore->Flags &= ~EF_INNER_KEYPAD;

				SubTabLogScript();
				GlobTomb4.BaseElevator.TotElelevators=0;
				break;
			}
			pElevatore->KeyPadIndex = 
					GlobTomb4.VetRemapObjects[ItemIndex];
			if (pElevatore->KeyPadIndex  == SCRIPT_IGNORE) {

				SHOW_DEBUG("ERROR elevator: Wrong inner keypad index (%d) in script dat",
					ItemIndex,0);

				SubTabLogScript();
				GlobTomb4.BaseElevator.TotElelevators=0;
				break;
			}

			pItem = &GlobTomb4.pAdr->pVetItems[pElevatore->KeyPadIndex];


			// correggere posizione di keypad
			// inserire anche i dati di orientamernto che deve
			// avere lara per essere difornte al keypad
			pElevatore->OrientLaraPerKeyPad = 
					pItem->OrientationH;
			// ora inserire i valori di controllo dei limiti
			// del settore, anzi, calcolare subito i limiti
			// ristretti
			pRect = &pElevatore->LimitiBloccoKeypad;			
			OrgX = pItem->CordX & ~0x3ff;
			OrgZ = pItem->CordZ & ~0x3ff;
			switch ((WORD) pItem->OrientationH) {
			case 0x8000:
	
				pItem->CordZ   += 0x20;
				// keypad verso est (ngle) lara deve essere verso ovest
				pRect->MinZ = OrgZ;
				pRect->MaxZ = OrgZ + 300;
				pRect->MinX = OrgX + 256;
				pRect->MaxX = OrgX + 1024-256;
				break;
			case 0x0000:
			
				pItem->CordZ -= 0x20;
				// keypad rivolto verso ovest
				pRect->MinZ = OrgZ+1024-300;
				pRect->MaxZ = OrgZ+1024;
				pRect->MinX = OrgX + 256;
				pRect->MaxX = OrgX + 1024-256;				
				break;
			case 0x4000:
				// rivolta verso sud
	
				pItem->CordX -= 0x20;
				pRect->MinX = OrgX+1024-300;
				pRect->MaxX = OrgX+1024;
				pRect->MinZ = OrgZ+256;
				pRect->MaxZ = OrgZ+1024-256;
				break;
			case 0xc000:
				// rivolto verso nord

				pItem->CordX += 0x20;

				pRect->MinX = OrgX;
				pRect->MaxX = OrgX+300;

				pRect->MinZ = OrgZ+256;
				pRect->MaxZ = OrgZ+1024-256;

				break;
			}

			
		}

		// convertire eventuali frameitems
		for (j=0;j<pElevatore->TotFrameItems;j++) {
			ItemIndex = pElevatore->VetFrameItems[j];
			SHOW_DEBUG("FrameItem[%d] with index=%d",
				j+1, ItemIndex);

			if (ItemIndex != SCRIPT_IGNORE) {
				pElevatore->VetFrameItems[j] = 
							GlobTomb4.VetRemapObjects[ItemIndex];
				SHOW_FULL_DEBUG("Tomb Frame index =%d", 
					pElevatore->VetFrameItems[j],0);
				
				if (pElevatore->VetFrameItems[j] == SCRIPT_IGNORE) {

					SHOW_DEBUG("ERROR: invalid frame index %d", ItemIndex,0);
					SubTabLogScript();
					SubTabLogScript();
					GlobTomb4.BaseElevator.TotElelevators=0;
					return;
				}
			}
		
		}
		pElevatore->pItem = &GlobTomb4.pAdr->pVetItems[pElevatore->IndiceElevatore];

		// se abbiamo appena caricato da savegame, aggiornare
		// tutti i valori

		if (TestSavegame==true) {

			pElevatore->FirstFloorY = pAscensore->FirstFloorY;

			// aggiornare subito cordy di elevatore
			pElevatore->pItem->CordY = pAscensore->CordYElevator;
			// se e' con porta singola aggiornare anche il cordy di porta
			if (pElevatore->Flags & EF_SINGLE_DOOR) {
				GlobTomb4.pAdr->pVetItems[pElevatore->IndexFirstDoor].CordY = 
						pAscensore->CordYElevator;
			}
			// aggiornare cordy di tutti i frame
			for (j=0;j<pElevatore->TotFrameItems;j++) {
				Indice=pElevatore->VetFrameItems[j];
				pItem = &GlobTomb4.pAdr->pVetItems[Indice];
				if (pItem->CordY != pAscensore->VetCordYFrame[j]) {
					AggiornaPosizioneItem(Indice, pItem->CordX, 
							pAscensore->VetCordYFrame[j],pItem->CordZ, -0x200);
				}
			}



		}
		// se ci sono multidoor scoprire tutte le altre door
		if (pElevatore->Flags & EF_MULTI_DOORS) {


			ItemIndex = pElevatore->IndexFirstDoor;
			SHOW_FULL_DEBUG("Required EF_MULTI_DOORS. First Door tomb index=%d", 
						ItemIndex,0);

			
			pElevatore->TotDoors = pElevatore->TotFloors;
			pDoor = &GlobTomb4.pAdr->pVetItems[ItemIndex];
			DoorX = pDoor->CordX;
			DoorY = pDoor->CordY;
			DoorZ = pDoor->CordZ;

			sprintf(BufferLog, "FirstDoor:  X=%d  Y=%d Z=%d",
				DoorX, DoorY, DoorZ);
			SHOW_FULL_DEBUG(BufferLog,0,0);


			GapY = pElevatore->ClickDistance * 256;
			pElevatore->VetDoors[0] = pElevatore->IndexFirstDoor;
			if (pElevatore->Flags & EF_DOUBLE_DOOR) {
				SHOW_FULL_DEBUG("Required EF_DOUBLE_DOOR: first secondary door index=%d", 
							pElevatore->IndexFirstDoorSecodnary,0);

				pElevatore->VetDoorsSecondary[0] = 
						pElevatore->IndexFirstDoorSecodnary;
			}

			for (j=1;j<pElevatore->TotDoors;j++) {
				DoorY -= GapY;

				SHOW_DEBUG("Searching door for %dst floor with CordY=%d", 
					j+1, DoorY);
				
				// ora cercare una porta che abbia coordinate di door
				for (z=0;z<*GlobTomb4.pAdr->pTotItems;z++) {
					pDoor = &GlobTomb4.pAdr->pVetItems[z];


					if (pDoor->CordX == DoorX &&
						pDoor->CordY == DoorY &&
						pDoor->CordZ == DoorZ && 
						pDoor->SlotID >= 322 && 
							pDoor->SlotID <= 335) break;
				}

				if (z == *GlobTomb4.pAdr->pTotItems) {

					SHOW_DEBUG("ERROR: cann't find the door for %dth floor",
						j+1,0);


					z = SCRIPT_IGNORE;
				}else {

					pElevatore->VetDoors[j] = z;
					SHOW_DEBUG("Found for floor %dst the item door = %d",
						j+1, GetNgleIndice(z));

					if (z != SCRIPT_IGNORE && 
						(pElevatore->Flags & EF_DOUBLE_DOOR) !=0) {
						// localizzare porta doppia
						z = GetNgleIndice(z);

						pElevatore->VetDoorsSecondary[j] =
								GlobTomb4.VetRemapObjects[z+1];

						if (pElevatore->VetDoorsSecondary[j] == SCRIPT_IGNORE) {
							sprintf(BufferLog, "ERROR in Elevator command: cann't find the paired door of double door with main door index = %d",
								z);
							InviaLog(BufferLog);
						}
					

					}
				}
			}
		}


		// ora inizializza dati dinamici di ascensore
		// a meno che non abbiamo appena caricato savegame

		if (TestSavegame==false) {
			pElevatore->FirstFloorY  = 
					GlobTomb4.pAdr->pVetItems[pElevatore->IndiceElevatore ].CordY;
			SHOW_FULL_DEBUG("New Level Mode: reset elevator record",0,0);
			pAscensore->FloorNow =0;
			pAscensore->FloorTarget =0;
			pAscensore->IncY =0;
			// salvare origine y

			pAscensore->Soffitto =0;
			pAscensore->Status = EST_ATTESA;
			pAscensore->LastIncy = pElevatore->Speed;
		}
		// calcolare cordy di massima altezza di ascensore
		pElevatore->MaxYFloor = pElevatore->FirstFloorY - 
					pElevatore->ClickDistance * 256 * pElevatore->TotFloors;
		pElevatore->MaxRoom = pElevatore->pItem->Room;
		SHOW_FULL_DEBUG("MaxYFloor=%d  Highest Room=%d",
			pElevatore->MaxYFloor, 
			GetNgleRoomIndice(pElevatore->pItem->Room));

		pFloor= GetFloor(pElevatore->pItem->CordX, 
				pElevatore->MaxYFloor, 
				pElevatore->pItem->CordZ, 
				&pElevatore->MaxRoom);
		
		// ora usare numero di stanza per calcolare coordinata y di soffitto
		pElevatore->MaxYFloor = GlobTomb4.pAdr->pVetRooms[pElevatore->MaxRoom].OrigYBottom -1;

		// attivare item
		AddActiveItem(pElevatore->IndiceElevatore);
		if (TestSavegame==false) {
			// controllare che porta di piano attuale sia aperta
			if (((pElevatore->Flags & EF_MULTI_DOORS) || 
				(pElevatore->Flags & EF_SINGLE_DOOR)) && 
				(pElevatore->Flags & EF_MODE_YO_YO)==0) {
				
				if (pElevatore->Flags & EF_MULTI_DOORS) {
					ItemIndex = pAscensore->FloorNow;
					Indice = pElevatore->VetDoors[ItemIndex];
					// aprire porta tradizionale
					SHOW_DEBUG("MULTI_DOORS: Open door=%d", 
						GetNgleIndice(Indice),0);

					EsecuzioneActionTrigger(0, 0x011A, Indice, SCANF_DIRECT_CALL);
					
					if (pElevatore->Flags & EF_DOUBLE_DOOR) {
						SHOW_DEBUG("Oper double door=%d", 
							GetNgleIndice(pElevatore->VetDoorsSecondary[ItemIndex]),0);
						EsecuzioneActionTrigger(0,0x011A, 
							pElevatore->VetDoorsSecondary[ItemIndex], SCANF_DIRECT_CALL);
						
					}
				}else {
					// se e' singola porta usare indice fornito
					Indice = pElevatore->IndexFirstDoor;
					SHOW_DEBUG("SINGLE_DOOR: Open door %d", GetNgleIndice(Indice),0);
					// aprire porta
					if (pElevatore->Flags & EF_ANIM_DOOR) {
						// muovere porta animating

						GestionePortaAnimating(pElevatore, true);
					}else {
						// aprire porta tradizionale
						EsecuzioneActionTrigger(0, 0x011A, Indice, SCANF_DIRECT_CALL);
						
						if (pElevatore->Flags & EF_DOUBLE_DOOR) {
							SHOW_DEBUG("Open double door %d", 
								GetNgleIndice(pElevatore->IndexFirstDoorSecodnary),0);
							EsecuzioneActionTrigger(0, 0x011A, pElevatore->IndexFirstDoorSecodnary ,SCANF_DIRECT_CALL);
							
						}
					}
				}


				
			}
			
			if (pElevatore->Flags & EF_MODE_YO_YO) {
				// inserire comando per muovere verso l'alto
				pAscensore->Status = EST_INIZIO_MOVIMENTO;
				pAscensore->FloorTarget = pElevatore->TotFloors -1;
			}

			if (pElevatore->Flags & EF_MODE_STOP_AND_GO) {
				// attivare apertura porta e attesa
				pAscensore->Status = EST_OPEN_AND_STOP;
				pAscensore->FloorTarget=0;
			}
			SHOW_DEBUG("Floor Target = %dst", pAscensore->FloorTarget +1,0);
		}
		SubTabLogScript();


	}
	// se ci sono global trigger relativi a indici ascensori 
	// convetitre adesso i loro indici da ngle a tomb4
	for (i=0;i<GlobTomb4.pBaseGlobalTriggers->TotTriggers;i++) {
		pGlobal = &GlobTomb4.pBaseGlobalTriggers->VetTriggers[i];

		if (pGlobal->GlobalTrigger == GT_ELEVATOR_STOPS_AT_FLOOR ||
			pGlobal->GlobalTrigger == GT_ELEVATOR_STARTS_FROM_FLOOR) {

			// convertire valore parametro
			ValShort = pGlobal->Parameter & 0x0fff;
			SHOW_DEBUG("Convert Elevator Index=%d for GlobalTrigger=%d",
				ValShort, pGlobal->Id);

			ValShort = GlobTomb4.VetRemapObjects[ValShort];
			SHOW_DEBUG("New tomb index = %d", ValShort,0);
			ValShort |= (pGlobal->Parameter & 0xf000);
			pGlobal->Parameter = ValShort;
		}
	}

	// attivare evento global trigger per ascensore arrivato al primo
	// piano per tutti gli ascensori (a patto che non siano in fase
	// caricasavegame
	if (TestSavegame==false) {
		for (i=0;i<GlobTomb4.BaseElevator.TotElelevators;i++) {
			// evento per piano 1
			ValShort = GlobTomb4.BaseElevator.VetScriptElevators[i].IndiceElevatore;
			ValShort |= 0x1000;
			ImpostaEventoNow(GT_ELEVATOR_STOPS_AT_FLOOR, ValShort);
		}
	}
	SubTabLogScript();
	SubTabLogScript();

}

// when there are elevator= command in the script, the twoblockplatform object will be redirected
// to our custom procedure to transform it in the trng elevator
// #callback#
void SetSlotElevator(void)
{
	StrSlot *pSlotNow;

	if (GlobTomb4.BaseElevator.TotElelevators > 0) {
		pSlotNow =&GlobTomb4.pAdr->pVetSlot[150];
		pSlotNow->pProcCeiling = &TwoBlockPlatformCeiling;
		pSlotNow->pProcFloor = &TwoBlockPlatformFloor;
		pSlotNow->pProcControl = &ControlTwoBlockPlatform;
	}
}





