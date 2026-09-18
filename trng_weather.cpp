#include "stdafx.h"
#include "bass.h"
#include "stdio.h"
#include <math.h>


#include "Tomb_NextGeneration.h" // mnemonic constants defined in tomb_NextGeneration.dll

#include "structures.h" // structure of tomb4 program and trng dll
#include "DefTomb4Funct.h" // defines of tomb4 procedure prototypes
#include "macros.h"  // definition of macros



// #TAG_ADDING_NEW_VARIABLES#
extern StrGlobaliTomb4 GlobTomb4;
extern char BufferLog[];
DWORD SalvAdrRoom;
StrWeather RainSnowData;

// #TAG_ADDING_NEW_DECLARES#
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
// -> last declare line
extern TYPE_GetHeight GetHeight;
extern TYPE_S_SoundSampleIsPlaying S_SoundSampleIsPlaying;
// -> last declare line


// #TAG_ADDING_NEW_FUNCTIONS#
// -> last added function bodies

__declspec(naked) void ClearRainSnowBuffers(void)
{
	__asm {

	xor  ecx, ecx	
	xor  eax, eax
	push esi
	push ebx

	lea esi, RainSnowData.RainBuffer
	lea ebx, RainSnowData.SnowBuffer
Salto1:
	mov  dword ptr [esi][eax], ecx
	mov  dword ptr [ebx][eax], ecx
	add  eax, 14h
	cmp  eax, SIZE_RAIN_BUFFER
	jl   Salto1

	pop  ebx
	pop  esi
	retn
	}


}

void SetCustomizeWeatherDefault(void)
{
	StrDatiXRain *pRain;

	pRain = &GlobTomb4.DatiRain;

	// ora copiare i valori
	RainSnowData.Rain_Float_1 = pRain->Rain_Float_1;
	RainSnowData.Rain_Float_2 = pRain->Rain_Float_2;
	RainSnowData.Rain_Float_4 = pRain->Rain_Float_4;
	RainSnowData.Rain_Float_8 = pRain->Rain_Float_8;
	RainSnowData.Rain_Float_16 = pRain->Rain_Float_16;
	RainSnowData.Max_Rain = pRain->Max_Rain;
	RainSnowData.Min_Rain = pRain->Min_Rain;
	RainSnowData.Splash_Rain = pRain->SplashRain;
	GlobTomb4.DatiRain.LastRoomCamera = -1;
	GlobTomb4.DatiSnow.LastRoomCamera = -1;

}
// initialise default data for rain & snow
 void InitRainSnow(void)
 {
	static float VettoreSizeSnow[] = {-24,-24,-24,+24,+24,-24,+24,+24,-12,-12,-12,+12,+12,-12,+12,+12,
					-8,-8,-8,+8,+8,-8,+8,+8,-6,-6,-6,+6,+6,-6,+6,+6};

	int i;

	memset(&RainSnowData, 0, sizeof(StrWeather));

	RainSnowData.ContaFrameSnow=0;
	RainSnowData.Rain_Float_1 = 1;
	RainSnowData.Rain_Float_16 = 16;
	RainSnowData.Rain_Float_2 = 2;
	RainSnowData.Rain_Float_20480 = 0x5000;
	RainSnowData.Rain_Float_4 = 4;
	RainSnowData.Rain_Float_8 = 8;
	RainSnowData.Splash_Rain = 1;

	for (i=0;i<32;i++) {
		RainSnowData.VettoreSizeSnow[i] = VettoreSizeSnow[i];
	}
 }


 // chiamata all'inizio del livello (o del gioco)
// imposta in globtomb4 i valori di default di pioggia
 // initialise data for rain & snow of current level
void InitWeatherDefault(void)
{
	StrDatiXRain *pRain;
	StrDatiXSnow *pSnow;

	pRain = &GlobTomb4.DatiRain;

	pRain->LastIntensita = -1;
	pRain->LastRoomCamera = -1;

	pRain->Max_Rain = RainSnowData.Max_Rain;
	pRain->Min_Rain = RainSnowData.Min_Rain;
	pRain->Rain_Float_1 = 1;
	pRain->Rain_Float_16 = 16;
	pRain->Rain_Float_2 = 2;
	pRain->Rain_Float_20480 = 20480;
	pRain->Rain_Float_4 = 4;
	pRain->Rain_Float_8 = 8;
	pRain->SplashRain  = 1;
	pRain->Flags = 0;
	pRain->SoundSFX = -1;


	pSnow = &GlobTomb4.DatiSnow;

	pSnow->LastIntensita = -1;
	pSnow->LastRoomCamera = -1;
	pSnow->Max_Snow = RainSnowData.Max_Snow;
	pSnow->Min_Snow = RainSnowData.Min_Snow;





	
}


__declspec(naked) void ImpostaMinMaxForRainAndSnow(void)
{
	__asm {
		
	mov eax, 754468h    ;Ptr_BaseGlobale
	mov  eax, dword ptr [eax]	
	test byte ptr [eax+4ch], 80h   ;modo hardware
	jz   Salto1

	mov  eax, 800h    // era 800h
	mov  word ptr [RainSnowData.Max_Snow], ax 
	mov  word ptr [RainSnowData.Max_Rain], ax 
	mov  eax, 80h    // era 80h
	mov  word ptr [RainSnowData.Min_Snow], ax 
	mov  word ptr [RainSnowData.Min_Rain], ax 
	pushad

	lea  eax, InitWeatherDefault
	call eax
	popad

	ret
	// se e' modo softare diminuire le gocce/fiocchi
Salto1:
	mov  eax, 100h
	mov  word ptr [RainSnowData.Max_Snow], ax 
	mov  word ptr [RainSnowData.Max_Rain], ax 
	mov  eax, 08h
	mov  word ptr [RainSnowData.Min_Snow], ax 
	mov  word ptr [RainSnowData.Min_Rain], ax 
	ret

	}
}
// chiamata prima di dorain e dosnow
// verifica se lara o stanza di camera e' in una stanza con pioggia o neve
// con intensita' diversa da quella precedente.
// se lo e' imposta tutti i nuovi valori
void CambioIntensitaWeather(void)
{
	short *pCamera_PosNow_Room = (short *) 0x7FE71C; // era 0x7FE71C;
	int Indice;
	BYTE NewIntensity;
	BYTE **p2BaseGlobale = (BYTE**) 0x754468;
	BYTE *pBaseGlobale;
	StrDatiXRain *pRain;

	StrRoomTr4 *pRoom;
	StrDatiXSnow *pSnow;

	static DWORD VetSplash[] = {1,1,1,1};
	static float VetGocce[] = {1.5,2.5,3.5,5.0};

	
	// se modalita' non e' hardware non cambiare mai niente
	pBaseGlobale = *p2BaseGlobale;

	if ((pBaseGlobale[0x4c] & 0x80)== 0) return;
	Indice = *pCamera_PosNow_Room;
	
	pRoom = &GlobTomb4.pAdr->pVetRooms[Indice];
	pRain  = &GlobTomb4.DatiRain;

	if (pRoom->FlagsRoom & 0x0800) {
		// stanza e' pioggia o deve essere impostato la prima volta



		if (pRain->LastRoomCamera == Indice) return;

		// e' cambiata la stanza sorgente di camera
		pRain->LastRoomCamera = Indice;
		// vedere se intensita' e' cambiata
		NewIntensity = GlobTomb4.VetExtraInfoRoom[Indice].WaterIntensity;
	
		if (NewIntensity == GlobTomb4.DatiRain.LastIntensita ) return;
		pRain->LastIntensita = NewIntensity;

		// ora costruire nuovi dati sulla base di indice
		// max rain: usare come unita Max_rain di default / 4
		// in questo modo i valori saranno:
		// default era: 0x800
		// 0x400   0x600  0x800  0xa00 (massimo)
	
		if (pRain->Max_Rain == 0x800) {
			RainSnowData.Max_Rain = (pRain->Max_Rain / 4) * (NewIntensity+2);
		}else {
			RainSnowData.Max_Rain = pRain->Max_Rain;
		}

		if (RainSnowData.Max_Rain > MAX_RAIN_PARTICLES) RainSnowData.Max_Rain = MAX_RAIN_PARTICLES;

		// min_rain
		// default 0x80
		if (pRain->Min_Rain == 0x80) {
			RainSnowData.Min_Rain = (pRain->Min_Rain / 4) * (NewIntensity+2);
		}else {
			RainSnowData.Min_Rain = pRain->Min_Rain;
		}

		// dimensione splash
		if (pRain->SplashRain == 1) {
			RainSnowData.Splash_Rain = VetSplash[NewIntensity];
		}else {
			RainSnowData.Splash_Rain=pRain->SplashRain;
		}
		
		// dimensione goccia
		if (pRain->Rain_Float_2 == 2) {
			RainSnowData.Rain_Float_2 = VetGocce[NewIntensity];
		}else {
			RainSnowData.Rain_Float_2 = pRain->Rain_Float_2;
		}

		return;			
	}

	// qui poi andra' fatto calcolo anche per neve
	pSnow = &GlobTomb4.DatiSnow;	
	if (pRoom->FlagsRoom & 0x0400) {
		// trovare intensita' neve


		if (pSnow->LastRoomCamera == Indice) return;

		// e' cambiata la stanza sorgente di camera
		pSnow->LastRoomCamera = Indice;
		// vedere se intensita' e' cambiata
		NewIntensity = GlobTomb4.VetExtraInfoRoom[Indice].WaterIntensity;
	
		if (NewIntensity == GlobTomb4.DatiSnow.LastIntensita ) return;
		pSnow->LastIntensita = NewIntensity;

		RainSnowData.Max_Snow = (pSnow->Max_Snow / 4) * (NewIntensity+1);
		if (RainSnowData.Max_Snow > MAX_SNOW_PARTICLES) RainSnowData.Max_Snow = MAX_SNOW_PARTICLES;

		// min_Snow
		// default 0x80
		RainSnowData.Min_Snow = (pSnow->Min_Snow / 4) * (NewIntensity+1);

	}

}
// gia' ottenuto pFloor (da GetFloor) ora ottenere l'altezza del floor
// in questo punto e restituirla
int TrovaYSplash(DWORD OrgX, int OrgY, DWORD OrgZ, 
					StrRoomTr4 *pRoom,  void *pFloor)
{
	int MiaY;


	MiaY =GetHeight(pFloor,OrgX, pRoom->OrigYBottom ,OrgZ);
	//MiaY -= 256;
	return MiaY;

}


// funzione prelevata da tomb5 e usata per DoRain
__declspec(naked) AddPolyLine(void)
{
	__asm {
	sub  esp, 12ch	; INIZIO PROCEDURA 2724 ;  SIZE=0x2C0 
	mov  ecx, [esp+130h] 
	push ebx 
	mov ebx, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ebx, dword ptr [ebx] 
	push ebp 
	mov  eax, [ecx+04h] 
	mov  edx, [ecx+20h] 
	fld  dword ptr [ecx] 
	mov  [esp+10h], eax 
	mov  eax, [ecx+24h] 
	mov  [esp+0ch], eax 
	mov  [esp+08h], edx 
	fld  dword ptr [esp+0ch] 
	fsub dword ptr [esp+10h] 
	push esi 
	mov  si, [ebx] 
	push edi 
	mov  di, [ebx+02h] 
	fabs 
	fld  dword ptr [esp+10h] 
	fsub st(0), st(2) 
	fabs 
	fxch st(1) 
	fxch st(1) 
	fcompp 
	fstsw  ax 
	test ah, 41h 
	fld  st(0) 
	jnz  Salto1 
	fld  dword ptr [esp+18h] 
	fadd dword ptr [RainSnowData.Rain_Float_2]   
	mov  [esp+30h], edx 
	fstp dword ptr [esp+28h] 
	fld  dword ptr [esp+14h] 
	fadd dword ptr [RainSnowData.Rain_Float_2]   
	fstp dword ptr [esp+24h] 
	jmp  Salto2 
Salto1: 
	fadd dword ptr [RainSnowData.Rain_Float_2]   
	fld  dword ptr [esp+10h] 
	fadd dword ptr [RainSnowData.Rain_Float_2]   
	mov  eax, [esp+18h] 
	mov  edx, [esp+14h] 
	mov  [esp+28h], eax 
	mov  [esp+24h], edx 
	fstp dword ptr [esp+30h] 
Salto2: 
	mov  eax, [ecx+10h] 
	mov  ebp, [ecx+18h] 
	fld  dword ptr [ecx+0ch] 
	mov  edx, [ecx+30h] 
	mov  [esp+4ch], eax 
	fst  dword ptr [esp+1ch] 
	mov  [esp+6ch], eax 
	mov  eax, [esp+10h] 
	mov  [esp+20h], ebp 
	mov  ebp, [ecx+1ch] 
	mov  [esp+7ch], eax 
	mov  eax, [esp+14h] 
	mov  [esp+38h], ebp 
	mov  ebp, [ecx+38h] 
	mov  ecx, [ecx+3ch] 
	mov  [esp+80h], eax 
	mov  eax, [esp+1ch] 
	mov  [esp+2ch], ebp 
	mov  [esp+88h], eax 
	mov  eax, [esp+2ch] 
	mov  [esp+34h], ecx 
	mov  ecx, [esp+18h] 
	mov  ebp, [esp+28h] 
	mov  [esp+94h], eax 
	mov  eax, [esp+34h] 
	mov  [esp+40h], ecx 
	mov  ecx, [esp+20h] 
	mov  [esp+98h], eax 
	mov  eax, [esp+30h] 
	mov  [esp+60h], ebp 
	mov  ebp, [esp+1ch] 
	mov  [esp+9ch], eax 
	mov  eax, [esp+24h] 
	mov  [esp+54h], ecx 
	mov  ecx, [esp+38h] 
	mov  [esp+0a0h], eax 
	mov  eax, [esp+1ch] 
	mov  [esp+68h], ebp 
	mov  ebp, [esp+20h] 
	mov  [esp+0a8h], eax 
	mov  eax, [esp+2ch] 
	mov  [esp+58h], ecx 
	fxch st(2) 
	fstp dword ptr [esp+3ch] 
	fxch st(1) 
	fstp dword ptr [esp+48h] 
	mov  [esp+74h], ebp 
	mov  ebp, [esp+38h] 
	fstp dword ptr [esp+5ch] 
	mov  [esp+0b4h], eax 
	mov  eax, [esp+34h] 
	mov  ecx, 0ff000000h 
	mov  [esp+78h], ebp 
	mov  [esp+50h], ecx 
	mov  [esp+70h], ecx 
	mov  [esp+8ch], edx 
	mov  [esp+90h], ecx 
	mov  [esp+0b8h], eax 
	mov  [esp+0ach], edx 
	mov  [esp+0b0h], ecx 
	mov  [ebx], si 
	mov ecx, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ecx, dword ptr [ecx] 
	mov  [ecx+02h], si 
	mov edx, 753854h    ;Ptr_MemStruttturaSchermo
	mov  edx, dword ptr [edx] 
	mov  ecx, [esp+144h] 
	mov  [edx+04h], di 
	mov eax, 753854h    ;Ptr_MemStruttturaSchermo
	mov  eax, dword ptr [eax] 
	mov  [eax+06h], di 
	mov  ax, [ecx] 
	pop  edi 
	pop  esi 
	pop  ebp 
	cmp  ax, 03h 
	pop  ebx 
	jz   Salto3 
	cmp  ax, 02h 
	jz   Salto3 
	push 00h 
	push ecx 
	push 03h 
	push 02h 
	push 01h 
	lea  ecx, [esp+40h] 
	push 00h 
	push ecx 
	mov eax, 5339A8h    ;Ptr_AddQuadClippedZBuffer
	call dword ptr [eax] 
	add  esp, 1ch 
	add  esp, 12ch 
	ret 
Salto3: 
	push 00h 
	push ecx 
	push 03h 
	push 02h 
	push 01h 
	lea  edx, [esp+40h] 
	push 00h 
	push edx 
	mov eax, 5339A4h    ;Ptr_AddQuadClippedSorted
	call dword ptr [eax] 
	add  esp, 1ch 
	add  esp, 12ch 
	ret 
 


	}

}


__declspec(naked) void DoRain(void)
{
	__asm {


	sub  esp, 0c8h	; INIZIO PROCEDURA 2722 ;  SIZE=0x9B0 
	push ebx 
	push ebp 
	push esi 
	push edi 




	xor  edi, edi 
	xor  ebx, ebx 
	cmp  word ptr [RainSnowData.Max_Rain], di    
	mov  [esp+18h], edi 
	jle  DopoLoopRain 
	// mov  esi, offset RainBuffer + 8    
	lea esi, RainSnowData.RainBuffer[8]
LoopRain: 
	push eax
	mov eax, 7FD1E0h   ;TestPresenteOrizzonte
	cmp  dword ptr [eax], edi 
	pop eax
	jz   MuoviGocciaRain 
	cmp  [esi-08h], edi 
	jnz  MuoviGocciaRain 
	movsx  eax, word ptr [RainSnowData.Min_Rain]    
	cmp  ebx, eax 
	jge  MuoviGocciaRain 
 
	inc  ebx 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax  ;GetRandomDraw 
	and  eax, 7ffh 
	mov  ebp, 0fffffc00h 
	sub  ebp, eax 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax  ;GetRandomDraw 
	mov  edi, eax 
	and  edi, 1fffh 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ;GetRandomDraw 
	and  eax, 1ffeh 
	mov edx, 7FE700h    ;Camera_SourceX
	mov  edx, dword ptr [edx] 
	movsx  ecx, word ptr [4B34D0h][eax*2] ;MatriceOrientamento 
	movsx  eax, word ptr [ 4B34D2h][eax*2] ;MatriceOrientamento+2 
	imul ecx, edi 
	imul eax, edi 
	mov edi, 7FE708h    ;Camera_SourceZ
	mov  edi, dword ptr [edi] 
	sar  ecx, 0ch 
	add  ecx, edx 
	mov edx, 7FE704h    ;Camera_SourceY
	mov  edx, dword ptr [edx] 
	sar  eax, 0ch 
	add  eax, edi 
	add  edx, ebp 
	push eax 
	push edx 
	push ecx 
	mov  [esi-08h], ecx 
	mov  [esi-04h], edx 
	mov  [esi], eax 
	// impostare roomnumber = -1
	mov eax, 7FE6C2h ; RoomNumber
	mov word ptr [eax], -1

	mov eax, 44C050h    ;IsRoomOutside
	call eax 
	add  esp, 0ch 
	mov eax, 7FE6C2h ; RoomNumber
	
	cmp word ptr [eax], -1
	jnz  go_isOutside
	// se e' -1 vuol dire che devo rinunciare
	mov  dword ptr [esi-08h], 00h 
	xor  edi, edi 
	jmp  NextLoopRain2 
go_isOutside: 
	mov eax, 7FE6C2h    ;RoomNumber
	movsx  eax, word ptr [eax] 
	lea  ecx, [eax][eax*8] 
	lea  edx, [eax][ecx*4] 
	mov eax, 533934h    ;Ptr_VetRooms
	mov  eax, dword ptr [eax]
	shl  edx, 02h 
	test byte ptr [edx+4eh][eax], 01h   ;controlla water, si puo' controllare anche rain 
	jz  go_MostraPioggia 

go_NonStanzaRain:
	mov  dword ptr [esi-08h], 00h 
	xor  edi, edi 
	jmp  NextLoopRain2 
go_MostraPioggia: 
	// vedere se e' stanza con rain
	// a meno che non ci sia attivo flag per all rain
	test word ptr [edx+4eh][eax], 20h // outside
	jz  go_NonStanzaRain
	test word ptr [RainSnowData.LevelFlags], fngl_Rain_All
	jnz  FareRain

	test word ptr [edx+4eh][eax], 800h  ; nuovo flag per RAIN
	jz  go_NonStanzaRain
FareRain:
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	and  al, 07h 
	sub  al, 04h 
	mov  [esi+04h], al 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	and  al, 03h 
	add  al, 18h 
	mov  [esi+05h], al 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	mov edx, 7FE6C2h    ;RoomNumber
	mov  dx, word ptr [edx] 
	and  al, 07h 
	sub  al, 04h 
	mov  cl, 40h 
	mov  [esi+06h], al 
	mov  al, [esi+05h] 
	sub  cl, al 
	mov  [esi+0ah], dx //  <- e' qui che viene messo il numero di stanza sballato
	mov  [esi+07h], cl 
	
	xor  edi, edi 
MuoviGocciaRain: 
	mov  ecx, [esi-08h] 
	cmp  ecx, edi 
	jz   NextLoopRain2 
	cmp  byte ptr [esi+07h], 0f0h 
	ja   NextLoopRain 
	mov eax, 7E70A0h    ;Camera_Look_X
	mov  eax, dword ptr [eax] 
	sub  eax, ecx 
	cdq 
	xor  eax, edx 
	sub  eax, edx 
	cmp  eax, 1770h     ;6000 
	jg   NextLoopRain 
	mov eax, 7E70A8h    ;Camera_Look_Z
	mov  eax, dword ptr [eax] 
	mov  edx, [esi] 
	sub  eax, edx 
	cdq 
	xor  eax, edx 
	sub  eax, edx 
	cmp  eax, 1770h   ;6000 
	jg   NextLoopRain 
	movsx  eax, byte ptr [esi+04h] 
	mov ecx, 804D40h    ;RobaHair_X
	mov  ecx, dword ptr [ecx] 
	mov  ebp, [esi-08h] 
	lea  edx, [eax][ecx*4] 
	mov  ecx, [esi-04h] 
	xor  eax, eax 
	add  ebp, edx 
	mov  al, [esi+05h] 
	mov  [esi-08h], ebp 
	movsx  edx, byte ptr [esi+06h] 
	shl  eax, 03h 
	mov  ebp, [esi] 
	add  ecx, eax 
	mov eax, 804D44h    ;RobaHair_Z
	mov  eax, dword ptr [eax] 
	mov  [esi-04h], ecx 
	lea  edx, [edx][eax*2] 
	movsx  eax, word ptr [esi+0ah] 
	add  ebp, edx 
	lea  edx, [eax][eax*8] 
	mov  [esi], ebp 
	mov ebp, 533934h    ;Ptr_VetRooms
	mov  ebp, dword ptr [ebp] 
	lea  eax, [eax][edx*4] 
	shl  eax, 02h 
	add  eax, ebp 
	mov dword ptr [SalvAdrRoom], eax

	cmp  ecx, [eax+24h]  // OrigY Top Room
	jle  Salto1 
	cmp  ecx, [eax+20h]  //OrigY Bottom room
	jge  Salto1 
	mov  ecx, [eax+1ch]  // OrigZ Room
	mov  edx, [esi] 
	lea  ebp, [ecx+400h] 
	cmp  edx, ebp 
	jle  Salto1 
	movsx  ebp, word ptr [eax+28h] 
	shl  ebp, 0ah 
	lea  ecx, [ecx-400h][ebp] 
	cmp  edx, ecx 
	jge  Salto1 
	mov  ecx, [eax+14h] // OrigX Room
	mov  edx, [esi-08h] 
	lea  ebp, [ecx+400h] 
	cmp  edx, ebp 
	jle  Salto1 
	movsx  eax, word ptr [eax+2ah] 
	shl  eax, 0ah 
	lea  ecx, [eax-400h][ecx] 
	cmp  edx, ecx 
	jl   Salto4 
Salto1: 
	mov  dx, [esi+0ah] // stanza
	mov  ecx, [esi]   // orgx
	lea  eax, [esp+2ch] 
	mov  [esp+2ch], edx 
	mov  edx, [esi-04h] // orgy

	push eax 
	mov  eax, [esi-08h] // orgx
	push ecx   // cordz
	push edx   // cordy
	push eax  // cordx

	mov eax, 449880h    ;GetFloor  
	call eax 
	mov  ecx, [esp+3ch] 
	add  esp, 10h 
	cmp  cx, [esi+0ah] // vedere se e' la stessa stanza
	jnz  Salto2 
	// qui c'e' la splash per stanza diversa da water
	// se e' attiva la patfch per correzione sprinkler
	// calcolare il vlaore da usare come cordy
	test word ptr [GlobTomb4.DatiRain.Flags], FR_CORRECT_SPRINKLERS

	jz  ComeSempre
	pushad

	
	push eax // pFloor ottenuto da GetFloor

	mov eax, dword ptr [SalvAdrRoom]
	push eax  // pRoom 

	mov eax, [esi] // cordx
	push eax

	mov eax, [esi-4] // cordy
	push eax

	mov eax, [esi-8] // cordx
	push eax

	call TrovaYSplash
	add esp,14h
	mov  [esi-4], eax

	popad

ComeSempre:
	mov  ecx, [esi] 
	mov  edx, [esi-04h] 
	mov  eax, [esi-08h] 

	mov  eax, dword ptr [RainSnowData.Splash_Rain]
	push  eax
	mov  eax, [esi-08h] 
	push ecx 


	push edx 
	push eax 
	mov eax, 439030h    ;TriggerSmallSplash
	call eax 
	add  esp, 10h 
// era qui il soundeffect
	test word ptr [GlobTomb4.DatiRain.Flags], FR_PLAY_SFX
	jz   NextLoopRain

	push 0
	call S_SoundSampleIsPlaying
	add esp,4
	test al,al
	jnz NextLoopRain
	lea eax, [esi-8]
	push 0
	push eax
	movzx eax, word ptr [GlobTomb4.DatiRain.SoundSFX]
	push eax

	mov eax, 45E440h    ;SoundEffect
	call eax 
	add esp, 0ch

	jmp  NextLoopRain 
	// in cx c'e' il numero della stanza di una goccia
Salto2: 
	movsx  eax, cx 
	lea  edx, [eax][eax*8] 
	lea  eax, [eax][edx*4] 
	mov edx, 533934h    ;Ptr_VetRooms
	mov  edx, dword ptr [edx]
	shl  eax, 02h 
	// non e' qui, anche mettendo un jump immediato a saltapioggia
	// non cambia niente
	// qui ho fatto modifica
	// il punto e' che se la stanza attuale e' per pioggia
	// si deve saltare in StanzaPioggia

	test byte ptr [eax+4eh][edx], 01h  ;controlla se e' water 
	jnz StanzaPioggia
	mov ax, word ptr [eax+4eh][edx]
	test ax, 20h //  ROOM_OUTSIDE
	jz   StanzaPioggia
	// e' outside
	test word ptr [RainSnowData.LevelFlags], fngl_Rain_All
	jnz  Salto3
	// vedere se e' stanza rain
	test ax, 0800h // stanza rain
	jnz  Salto3
StanzaPioggia:
	mov  eax, [esi] 
	mov  ecx, [esi-04h] 

	mov  edx, dword ptr [RainSnowData.Splash_Rain]
	push edx   
	mov  edx, [esi-08h] 
	push eax 

	push ecx 
	push edx 
	mov eax, 439030h    ;TriggerSmallSplash
	call eax 
	add  esp, 10h 
	jmp  NextLoopRain 
Salto3: 
	mov  [esi+0ah], cx 
Salto4: 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax 
	mov  ecx, eax 
	and  ecx, 03h 
	cmp  cl, 03h 
	jz   Salto6 
	mov  dl, [esi+04h] 
	mov  cl, al 
	and  cl, 03h 
	add  cl, dl 
	dec  cl 
	cmp  cl, 0fch 
	mov  [esi+04h], cl 
	jge  Salto5 
	mov  byte ptr [esi+04h], 0fch 
	jmp  Salto6 
Salto5: 
	cmp  cl, 04h 
	jle  Salto6 
	mov  byte ptr [esi+04h], 04h 
Salto6: 
	sar  eax, 02h 
	and  eax, 03h 
	mov  ecx, eax 
	cmp  ecx, 03h 
	jz   Salto8 
	mov  al, [esi+06h] 
	add  al, cl 
	dec  al 
	cmp  al, 0fch 
	mov  [esi+06h], al 
	jge  Salto7 
	mov  byte ptr [esi+06h], 0fch 
	jmp  Salto8 
Salto7: 
	cmp  al, 04h 
	jle  Salto8 
	mov  byte ptr [esi+06h], 04h 
Salto8: 
	mov  cl, [esi+07h] 
	add  cl, 0feh 
	mov  al, cl 
	mov  [esi+07h], cl 
	cmp  al, 0f0h 
	jbe  NextLoopRain2 
NextLoopRain: 
	mov  [esi-08h], edi 
NextLoopRain2: 
	mov  eax, [esp+18h]  ;index to count loop 
	add  esi, 14h 
	movsx  edx, word ptr [RainSnowData.Max_Rain]    
	inc  eax 
	cmp  eax, edx 
	mov  [esp+18h], eax 
	jl   LoopRain 
DopoLoopRain: 
	push eax
	mov eax, 753C20h   ;ValFloat_MisteroC
	fld  dword ptr [eax] 
	pop eax
	fadd dword ptr [RainSnowData.Rain_Float_4]  ;mistero    
	mov eax, 753C64h    ;ValFloat_MisteroE
	mov  eax, dword ptr [eax] 
	mov ecx, 754034h    ;ValFloat_MisteroF
	mov  ecx, dword ptr [ecx] 
	mov  word ptr [esp+0b0h], 02h 
	mov  [esp+0b2h], di 
	mov  [esp+0b4h], di 
	mov  [esp+20h], eax 
	fstp dword ptr [esp+1ch] 
	push eax
	mov eax, 753BACh   ;ValFloat_MisteroB
	fld  dword ptr [eax] 
	pop eax
	fsub dword ptr [RainSnowData.Rain_Float_4]    
	mov  [esp+24h], ecx 
	fstp dword ptr [esp+28h] 
	mov eax, 48DF30h    ;phd_PushMatrix
	call eax 
	mov eax, 80E01Ch    ;Ptr_StrutturaLara
	mov  eax, dword ptr [eax] 
	mov  edx, [eax+48h] 
	mov  ecx, [eax+44h] 
	push edx 
	mov  edx, [eax+40h] 
	push ecx 
	push edx 
	mov eax, 48E690h    ;phd_TranslateAbs
	call eax 
	add  esp, 0ch 
	mov eax, 48E7B0h    ;SetD3DViewMatrix
	call eax 
	cmp  word ptr [RainSnowData.Max_Rain], di    
//	mov  esi, offset RainBuffer    
	lea esi, RainSnowData.RainBuffer
	mov  [esp+18h], edi 
	jle  Salto25 
	mov ebp, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ebp, dword ptr [ebp] 
	mov  ebx, 0ff000000h    ; -1.7 float 
Salto9: 
	mov  eax, [esi] 
	cmp  eax, edi 
	jnz  Salto10 
	add  esi, 14h 
	jmp  Salto24 
Salto10: 
	mov ecx, 80E01Ch    ;Ptr_StrutturaLara
	mov  ecx, dword ptr [ecx] 
	mov  edx, [ecx+40h] 
	sub  eax, edx 
	mov edx, 804D40h    ;RobaHair_X
	mov  edx, dword ptr [edx] 
	add  edx, edx 
	sub  eax, edx 
	mov  edx, [esi+04h] 
	mov  [esp+10h], eax 
	xor  eax, eax 
	mov  al, [esi+0dh] 
	fild dword ptr [esp+10h] 
	shl  eax, 03h 
	sub  edx, eax 
	mov  eax, [ecx+44h] 
	sub  edx, eax 
	mov  eax, [esi+08h] 
	mov  [esp+10h], edx 
	mov  edx, [ecx+48h] 
	mov ecx, 804D44h    ;RobaHair_Z
	mov  ecx, dword ptr [ecx] 
	sub  eax, edx 
	fild dword ptr [esp+10h] 
	lea  edx, [ecx][ecx] 
	sub  eax, edx 
	mov  [esp+10h], eax 
	fild dword ptr [esp+10h] 
	push eax
	mov eax, 753B18h   ;Matrice3dView + 020h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B08h   ;Matrice3dView + 010h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AF8h   ;Matrice3dView
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B28h   ;Matrice3dView + 030h
	fadd dword ptr [eax]  ;mistero 
	pop eax
	fstp dword ptr [esp+14h] 
	push eax
	mov eax, 753B1Ch   ;Matrice3dView + 024h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B0Ch   ;Matrice3dView + 014h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AFCh   ;Matrice3dView + 04h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B2Ch   ;Matrice3dView + 034h
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+10h] 
	push eax
	mov eax, 753B20h   ;Matrice3dView + 028h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B10h   ;Matrice3dView + 018h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B00h   ;Matrice3dView + 08h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B30h   ;Matrice3dView + 038h
	fadd dword ptr [eax] 
	pop eax
	fstp st(3) 
	fstp st(0) 
	fstp st(0) 
	push eax
	mov eax, 754038h   ;FloatValore5000PerSchermo
	fld  dword ptr [eax] 
	pop eax
	fsub st(0), st(1) 
	fld  dword ptr [RainSnowData.Rain_Float_1]    
	push eax
	mov eax, 754038h   ;FloatValore5000PerSchermo
	fdiv dword ptr [eax] 
	pop eax
	fmulp  st(1), st(0) 
	fsubr  dword ptr [RainSnowData.Rain_Float_1]    
	fmul dword ptr [RainSnowData.Rain_Float_8]    
	fadd dword ptr [RainSnowData.Rain_Float_8]    
	mov eax, 49DA5Ch    ;SwapULong
	call eax 
	mov  ecx, eax 
	push eax
	mov eax, 753BA4h   ;FloatTotRigheCar
	fcom dword ptr [eax] 
	pop eax
	or   ecx, 0ffff8000h    ;-32768 
	mov  edx, [esp+14h] 
	shl  ecx, 08h 
	or   ecx, eax 
	mov  [esp+44h], ebx 
	shl  ecx, 08h 
	or   ecx, eax 
	mov  eax, [esp+10h] 
	mov  [esp+4ch], eax 
	mov  [esp+40h], ecx 
	fstsw  ax 
	mov  [esp+48h], edx 
	test ah, 01h 
	jnz  Salto15 
	fcom dword ptr [RainSnowData.Rain_Float_20480]    
	fstsw  ax 
	test ah, 41h 
	jnz  Salto11 
	fstp st(0) 
	push eax
	mov eax, 753BF8h   ;FloatWordAltaTotRigheCar
	fld  dword ptr [eax] 
	pop eax
	mov  edi, 10h 
Salto11: 
	push eax
	mov eax, 753BA8h   ;ValFloat_MisteroA
	fld  dword ptr [eax] 
	pop eax
	fdiv st(0), st(1) 
	fld  st(0) 
	fmul dword ptr [esp+14h] 
	push eax
	mov eax, 753BFCh   ;FloatSchermoX_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+30h] 
	fld  st(0) 
	fmul dword ptr [esp+10h] 
	push eax
	mov eax, 753C00h   ;FloatSchermoY_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+34h] 
	push eax
	mov eax, 753C24h   ;ValFloat_MisteroD
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	fstp dword ptr [esp+3ch] 
	fstp st(0) 
	fld  dword ptr [esp+30h] 
	fcomp  dword ptr [esp+1ch] 
	fstsw  ax 
	test ah, 01h 
	jz   Salto12 
	inc  edi 
	jmp  Salto13 
Salto12: 
	fld  dword ptr [esp+30h] 
	fcomp  dword ptr [esp+28h] 
	fstsw  ax 
	test ah, 41h 
	jnz  Salto13 
	add  edi, 02h 
Salto13: 
	fld  dword ptr [esp+34h] 
	fcomp  dword ptr [esp+20h] 
	fstsw  ax 
	test ah, 01h 
	jz   Salto14 
	add  edi, 04h 
	jmp  Salto16 
Salto14: 
	fld  dword ptr [esp+34h] 
	fcomp  dword ptr [esp+24h] 
	fstsw  ax 
	test ah, 41h 
	jnz  Salto16 
	add  edi, 08h 
	jmp  Salto16 
Salto15: 
	mov  edi, 0ffffff80h     ;-128 
Salto16: 
	fstp dword ptr [esp+38h] 
	mov  [ebp+00h], di 
	mov eax, 80E01Ch    ;Ptr_StrutturaLara
	mov  eax, dword ptr [eax] 
	mov  ecx, [esi] 
	add  ebp, 02h 
	mov  edx, [eax+40h] 
	mov  edi, [eax+44h] 
	sub  ecx, edx 
	mov  edx, [esi+04h] 
	mov  [esp+10h], ecx 
	mov  ecx, [esi+08h] 
	fild dword ptr [esp+10h] 
	sub  edx, edi 
	xor  edi, edi 
	mov  [esp+10h], edx 
	mov  edx, [eax+48h] 
	fild dword ptr [esp+10h] 
	sub  ecx, edx 
	mov  [esp+10h], ecx 
	fild dword ptr [esp+10h] 
	push eax
	mov eax, 753B18h   ;Matrice3dView + 020h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B08h   ;Matrice3dView + 010h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AF8h   ;Matrice3dView
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B28h   ;Matrice3dView + 030h
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+14h] 
	push eax
	mov eax, 753B1Ch   ;Matrice3dView + 024h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B0Ch   ;Matrice3dView + 014h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AFCh   ;Matrice3dView + 04h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B2Ch   ;Matrice3dView + 034h
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+10h] 
	push eax
	mov eax, 753B20h   ;Matrice3dView + 028h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	push eax
	mov eax, 753B10h   ;Matrice3dView + 018h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(3) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B00h   ;Matrice3dView + 08h
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(4) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B30h   ;Matrice3dView + 038h
	fadd dword ptr [eax] 
	pop eax
	fstp st(3) 
	fstp st(0) 
	fstp st(0) 
	push eax
	mov eax, 754038h   ;FloatValore5000PerSchermo
	fld  dword ptr [eax] 
	pop eax
	fsub st(0), st(1) 
	fld  dword ptr [RainSnowData.Rain_Float_1]    
	push eax
	mov eax, 754038h   ;FloatValore5000PerSchermo
	fdiv dword ptr [eax] 
	pop eax
	fmulp  st(1), st(0) 
	fsubr  dword ptr [RainSnowData.Rain_Float_1]    
	fmul dword ptr [RainSnowData.Rain_Float_16]    
	fadd dword ptr [RainSnowData.Rain_Float_16]    
	mov eax, 49DA5Ch    ;SwapULong
	call eax 
	mov  edx, eax 
	or   edx, 0ffff8000h 
	shl  edx, 08h 
	or   edx, eax 
	shl  edx, 08h 
	or   edx, eax 
	push eax
	mov eax, 753BA4h   ;FloatTotRigheCar
	fcom dword ptr [eax] 
	pop eax
	mov  eax, [esp+14h] 
	mov  ecx, [esp+10h] 
	mov  [esp+68h], eax 
	mov  [esp+60h], edx 
	mov  [esp+64h], ebx 
	mov  [esp+6ch], ecx 
	fstsw  ax 
	test ah, 01h 
	jnz  Salto21 
	fcom dword ptr [RainSnowData.Rain_Float_20480]  ;0x5000    
	fstsw  ax 
	test ah, 41h 
	jnz  Salto17 
	fstp st(0) 
	push eax
	mov eax, 753BF8h   ;FloatWordAltaTotRigheCar
	fld  dword ptr [eax] 
	pop eax
	mov  edi, 10h 
Salto17: 
	push eax
	mov eax, 753BA8h   ;ValFloat_MisteroA
	fld  dword ptr [eax] 
	pop eax
	fdiv st(0), st(1) 
	fld  st(0) 
	fmul dword ptr [esp+14h] 
	push eax
	mov eax, 753BFCh   ;FloatSchermoX_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+50h] 
	fld  st(0) 
	fmul dword ptr [esp+10h] 
	push eax
	mov eax, 753C00h   ;FloatSchermoY_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+54h] 
	push eax
	mov eax, 753C24h   ;ValFloat_MisteroD
	fld  dword ptr [eax] 
	pop eax
	fmul st(0), st(1) 
	fstp dword ptr [esp+5ch] 
	fstp st(0) 
	fld  dword ptr [esp+50h] 
	fcomp  dword ptr [esp+1ch] 
	fstsw  ax 
	test ah, 01h 
	jz   Salto18 
	inc  edi
	jmp  Salto19 
Salto18: 
	fld  dword ptr [esp+50h] 
	fcomp  dword ptr [esp+28h] 
	fstsw  ax 
	test ah, 41h 
	jnz  Salto19 
	add  edi, 02h 
Salto19: 
	fld  dword ptr [esp+54h] 
	fcomp  dword ptr [esp+20h] 
	fstsw  ax 
	test ah, 01h 
	jz   Salto20 
	add  edi, 04h 
	jmp  Salto22 
Salto20: 
	fld  dword ptr [esp+54h] 
	fcomp  dword ptr [esp+24h] 
	fstsw  ax 
	test ah, 41h 
	jnz  Salto22 
	add  edi, 08h 
	jmp  Salto22 
Salto21: 
	mov  edi, 0ffffff80h 
Salto22: 
	fstp dword ptr [esp+58h] 
	mov  [ebp+00h], di 
	mov ebp, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ebp, dword ptr [ebp] 
	cmp  word ptr [ebp+00h], 00h 
	jnz  Salto23 
	cmp  word ptr [ebp+02h], 00h 
	jnz  Salto23 
	lea  edx, [esp+0b0h] 
	lea  eax, [esp+30h] 
	push edx 
	push eax 
	call AddPolyLine    
	mov ebp, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ebp, dword ptr [ebp] 
	add  esp, 08h 
Salto23: 
	add  esi, 14h 
	xor  edi, edi 
Salto24: 
	mov  eax, [esp+18h] 
	movsx  ecx, word ptr [RainSnowData.Max_Rain]    
	inc  eax 
	cmp  eax, ecx 
	mov  [esp+18h], eax 
	jl   Salto9 
Salto25: 
	mov edx, 7E70C8h    ;ptr_ZonaRecord30
	mov  edx, dword ptr [edx] 
 
	mov  eax, 30h 
	pop  edi 
	sub  edx, eax 

	pop  esi 
	pop  ebp 
	push eax
	mov eax, 7E70C8h   ;ptr_ZonaRecord30
	mov  dword ptr [eax], edx 
	pop eax
 
	pop  ebx 
	add  esp, 0c8h 
	ret 


	}

}



__declspec(naked) void DoSnow(void)
{
	__asm {

	// se versione ridotta, saltare un frame ogni due

	sub  esp, 0bch	; INIZIO PROCEDURA 2721 ;  SIZE=0x7F0 

	push ebx 
	push ebp 
	push esi 
	push edi 



	xor  edi, edi 
	cmp  word ptr [RainSnowData.Max_Snow], di   
	mov  [esp+14h], edi  ; (LOCALE_ESP - 0xB8) 
	mov  [esp+18h], edi  ; (LOCALE_ESP - 0xB4) 
	jle  go_ShowFiocchiNeve 
	// mov  esi, offset SnowBuffer + 4   
	lea esi, RainSnowData.SnowBuffer[4]
	mov  bl, 10h 
	jmp  Salto2 
Salto1:	
	xor  edi, edi 
Salto2:
	cmp  [esi-04h], edi 
	jnz  Salto5 
	cmp  dword ptr [GlobTomb4.TestOrizzonteVisibile], edi   
	jz   Salto22 
	movsx  eax, word ptr [RainSnowData.Min_Snow]   
	mov  ecx, [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	cmp  ecx, eax 
	jge  Salto22 
	mov  edi, ecx 
	inc  edi 
	mov  [esp+14h], edi  ; (LOCALE_ESP - 0xB8) 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	and  eax, 7ffh 
	mov  ebp, 0fffffc00h 
	sub  ebp, eax 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	mov  edi, eax 
	and  edi, 1fffh 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	and  eax, 0fffh 
	mov edx, 7FE700h    ;Camera_TargetX
	mov  edx, dword ptr [edx] 
	shl  eax, 1h 
	movsx  ecx, word ptr [4B34D0h][eax*2] ;MatriceOrientamento 
	movsx  eax, word ptr [ 4B34D2h][eax*2] ;MatriceOrientamento+2 
	imul ecx, edi 
	imul eax, edi 
	mov edi, 7FE708h    ;Camera_TargetZ
	mov  edi, dword ptr [edi] 
	sar  ecx, 0ch 
	add  ecx, edx 
	mov edx, 7FE704h    ;Camera_TargetY
	mov  edx, dword ptr [edx] 
	sar  eax, 0ch 
	add  eax, edi 
	add  edx, ebp 
	push eax 
	push edx 
	push ecx 
	mov  [esi-04h], ecx 
	mov  [esi], edx 
	mov  [esi+04h], eax 
	mov eax, 44C050h    ;IsRoomOutside
	call eax 
	add  esp, 0ch 
	test eax, eax 
	jge  Salto3 
	mov  dword ptr [esi-04h], 00h 
	jmp  Salto22 
Salto3: 
	mov eax, 7FE6C2h    ;RoomNumber
	movsx  eax, word ptr [eax] 
	lea  ecx, [eax][eax*8] 
	lea  edx, [eax][ecx*4] 
	mov eax, 533934h    ;Ptr_VetRooms
	mov  eax, dword ptr [eax] 
	shl  edx, 02h 
	test byte ptr [edx+4eh][eax], 01h 
	jz   Salto4 
IgnoraStanza:
	mov  dword ptr [esi-04h], 00h 
	jmp  Salto22 
Salto4: 
	test word ptr [RainSnowData.LevelFlags], fngl_Snow_All
	jnz  FaiLaNeve
	// verificare che sia stanza con neve
	test word ptr [edx+4eh][eax], 400h
	jz   IgnoraStanza
	// e se questa fosse la simulazione per le modifiche delle coordinate?
FaiLaNeve:
	mov  word ptr [esi+0ch], 00h 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax 
	// calcola spostamento orizzontale x
	and  al, 07h     ;piccolo movimento orizzontale
	sub  al, 04h 
	mov  [esi+08h], al 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax 
	// calcola spostamento verticale

	and  al, 0fh 
	add  al, 08h 
	
	shl  al, 03h 

	mov  [esi+09h], al 
	mov eax, 48EB70h    ;GetRandomDraw
	call eax ; 
	mov  cl, [esi+09h] 
	and  al, 07h 
	sub  al, 04h 
	mov  dl, 70h 
	shr  cl, 02h 
	mov  [esi+0ah], al 
	mov eax, 7FE6C2h    ;RoomNumber
	mov  ax, word ptr [eax] 
	sub  dl, cl 
	mov  [esi+0eh], ax 
	mov  [esi+0bh], dl 
Salto5: 
	mov  ecx, [esi+04h] 
	cmp  word ptr [esi+0ch], 00h 
	mov  edi, [esi-04h] 
	mov  ebp, [esi] 
	mov  [esp+10h], ecx  ; (LOCALE_ESP - 0xBC) 
	jnz  Salto11 
	movsx  edx, byte ptr [esi+08h] 
	mov  eax, edi 
	mov  ecx, [esi] 
	add  eax, edx 
	mov  [esi-04h], eax 
	xor  eax, eax 
	mov  al, [esi+09h] ;eppure questo sembra davvero l'incremento
	movsx  edx, byte ptr [esi+0ah] 
	shr  eax, 1h 
	and  al, 0fch 
	add  ecx, eax 
	mov  eax, [esi+04h] 
	add  eax, edx 
	mov  [esi], ecx 
	mov  [esi+04h], eax 
	movsx  eax, word ptr [esi+0eh] 
	lea  edx, [eax][eax*8] 
	lea  eax, [eax][edx*4] 
	mov edx, 533934h    ;Ptr_VetRooms
	mov  edx, dword ptr [edx] 
	shl  eax, 02h 
	add  eax, edx 
	cmp  ecx, [eax+24h] 
	jle  Salto7 
	cmp  ecx, [eax+20h] 
	jge  Salto7 
	mov  ecx, [eax+1ch] 
	mov  edx, [esi+04h] 
	lea  ebx, [ecx+400h] 
	cmp  edx, ebx 
	jle  Salto6 
	movsx  ebx, word ptr [eax+28h] 
	shl  ebx, 0ah 
	lea  ecx, [ebx-400h][ecx] 
	cmp  edx, ecx 
	jge  Salto6 
	mov  ecx, [eax+14h] 
	mov  edx, [esi-04h] 
	lea  ebx, [ecx+400h] 
	cmp  edx, ebx 
	jle  Salto6 
	movsx  eax, word ptr [eax+2ah] 
	shl  eax, 0ah 
	lea  ecx, [eax-400h][ecx] 
	cmp  edx, ecx 
	jl   Salto10 
Salto6: 
	mov  bl, 10h 
Salto7: 
	mov  dx, [esi+0eh] 
	mov  ecx, [esi+04h] 
	lea  eax, [esp+20h]  ; (LOCALE_ESP - 0xAC) 
	mov  [esp+20h], edx  ; (LOCALE_ESP - 0xAC) 
	mov  edx, [esi] 
	push eax 
	mov  eax, [esi-04h] 
	push ecx 
	push edx 
	push eax 
	mov eax, 449880h    ;GetFloor
	call eax 
	mov  ecx, [esp+30h]  ; (LOCALE_ESP - 0xAC) 
	add  esp, 10h 
	cmp  cx, [esi+0eh] 
	jnz  Salto8 
	mov  dword ptr [esi-04h], 00h 
	jmp  Salto22 
Salto8: 
	movsx  eax, cx 
	lea  edx, [eax][eax*8] 
	lea  eax, [eax][edx*4] 
	mov edx, 533934h    ;Ptr_VetRooms
	mov  edx, dword ptr [edx] 
	shl  eax, 02h 
	// mi sa che queste due righe qua sotto sono un errore
	// test word ptr [eax+4eh][edx], 400h
	// jz   ComeSeFosseAcqua
	test byte ptr [eax+4eh][edx], 01h 
	jz   Salto9 
	;stanza con acqua
// ComeSeFosseAcqua:
	mov  eax, [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	mov  word ptr [esi+0ch], 01h 
	mov  [esi+04h], eax 
	mov  al, [esi+0bh] 
	cmp  al, bl 
	mov  [esi-04h], edi 
	mov  [esi], ebp 
	jbe  Salto11 
	mov  [esi+0bh], bl 
	jmp  Salto11 
Salto9: 
	mov  [esi+0eh], cx 
	jmp  Salto11 
Salto10: 
	mov  bl, 10h 
Salto11: 
	mov  cl, [esi+0bh] 
	test cl, cl 
	jz   Salto13 
	mov eax, 7E70A0h    ;Camera_Look_X
	mov  eax, dword ptr [eax] 
	mov  edx, [esi-04h] 
	sub  eax, edx 
	cdq 
	xor  eax, edx 
	sub  eax, edx 
	cmp  eax, 1770h 
	jg   Salto12 
	mov eax, 7E70A8h    ;Camera_Look_Z
	mov  eax, dword ptr [eax] 
	mov  edx, [esi+04h] 
	sub  eax, edx 
	cdq 
	xor  eax, edx 
	sub  eax, edx 
	cmp  eax, 1770h 
	jle  Salto15 
Salto12: 
	test cl, cl 
	jnz  Salto14 
Salto13: 
	mov  dword ptr [esi-04h], 00h 
	jmp  Salto22 
Salto14: 
	cmp  cl, bl 
	jbe  Salto15 
	mov  [esi+0bh], bl 
Salto15: 
	mov  al, [esi+08h] 
	mov edx, 804D40h    ;RobaHair_X
	mov  edx, dword ptr [edx] 
	movsx  ecx, al 
	shl  edx, 02h 
	cmp  ecx, edx 
	jge  Salto16 
	add  al, 02h 
	jmp  Salto17 
Salto16: 
	jle  Salto18 
	sub  al, 02h 
Salto17: 
	mov  [esi+08h], al 
Salto18: 
	mov  al, [esi+0ah] 
	mov edx, 804D44h    ;RobaHair_Z
	mov  edx, dword ptr [edx] 
	movsx  ecx, al 
	shl  edx, 02h 
	cmp  ecx, edx 
	jge  Salto19 
	add  al, 02h 
	jmp  Salto20 
Salto19: 
	jle  Salto21 
	sub  al, 02h 
Salto20: 
	mov  [esi+0ah], al 
Salto21: 
	mov  al, [esi+0bh] 
	add  al, 0feh 
	mov  [esi+0bh], al 
	mov  al, [esi+09h] 

	and  al, 07h 
	cmp  al, 07h 

	jnc  Salto22 
	

	inc  byte ptr [esi+09h] 

Salto22: 
	mov  eax, [esp+18h]  ; (LOCALE_ESP - 0xB4) 
	add  esi, 14h 
	movsx  ecx, word ptr [RainSnowData.Max_Snow]   
	inc  eax 
	cmp  eax, ecx 
	mov  [esp+18h], eax  ; (LOCALE_ESP - 0xB4) 
	jl   Salto1 
	xor  edi, edi 
go_ShowFiocchiNeve: 
	push 0ff0000ffh 
	mov eax, 425D20h    ;void_effect
	call eax 
	mov eax,dword ptr [GlobTomb4.pSlotDefaultSprite]    ;Slot_463_DEFAULT_SPRITES + 2
	movsx  eax, word ptr [eax+2] 
	// per eseperimento provare ad usare un altro sprite
	add  eax, 0ah 
	
	lea  edx, [eax][eax*2] 
	mov eax, 533994h    ;Ptr_VetSpriteTextures
	mov  eax, dword ptr [eax] 
	fld  dword ptr [eax+08h][edx*8]    ;x1 float        0.0039062500000000000 
	fld  dword ptr [eax+10h][edx*8]   ;x2 float         0.1210937500000000000 
	fld  dword ptr [eax+0ch][edx*8]   ;y1 float         0.5664062500000000000 
	lea  eax, [eax][edx*8] 
	fld  st(1)		; 0.1210937500000000000 
	mov  ecx, [eax+14h]      	;y2 float     3F2F0000 
	mov  word ptr [esp+28h], 02h  ; (LOCALE_ESP - 0xA8)    StructA:00: WORD = 2 
	mov  [esp+2ch], di  ; (LOCALE_ESP - 0xA4) 
	mov  dx, [eax]		;TexturePage     0000000C 
	fstp dword ptr [esp+2eh]  ; (LOCALE_ESP - 0xA2)   ;x2 float  (00 00 F8 3D) 
	mov  [esp+2ah], dx  ; (LOCALE_ESP - 0xA6)        ;pagina texture 
	mov  eax, ecx   ;y2 float 
	fstp dword ptr [esp+32h]  ; (LOCALE_ESP - 0x9E)    ;y1 float     0.5664062500000000000 
	mov  edx, ecx      ;y2 float   3F2F0000 
	mov  [esp+18h], ecx  ; (LOCALE_ESP - 0xB8)     	;y2 float 
	fstp dword ptr [esp+36h]  ; (LOCALE_ESP - 0x9A)   ;'x2 float   0.1210937500000000000 
	mov  [esp+3ah], eax  ; (LOCALE_ESP - 0x96)  	;y2 float 
	mov  [esp+42h], ecx  ; (LOCALE_ESP - 0x8E) 
	fst  dword ptr [esp+3eh]  ; (LOCALE_ESP - 0x92)     ;x1 float 0.0039062500000000000 
	fstp dword ptr [esp+46h]  ; (LOCALE_ESP - 0x8A)  ;x1 float 
	mov  [esp+4ah], edx  ; (LOCALE_ESP - 0x86) 
	mov eax, 48DF30h    ;phd_PushMatrix
	call eax 
	mov eax, 7FE708h    ;Camera_TargetZ
	mov  eax, dword ptr [eax] 
	mov ecx, 7FE704h    ;Camera_TargetY
	mov  ecx, dword ptr [ecx] 
	mov edx, 7FE700h    ;Camera_TargetX
	mov  edx, dword ptr [edx] 
	push eax 
	push ecx 
	push edx 
	mov eax, 48E690h    ;phd_TranslateAbs
	call eax 
	add  esp, 10h 
	mov eax, 48E7B0h    ;SetD3DViewMatrix
	call eax 
	mov eax, 753854h    ;Ptr_MemStruttturaSchermo
	mov  eax, dword ptr [eax] 
	lea  ebx, RainSnowData.SnowBuffer   
	mov  [esp+18h], edi  ; (LOCALE_ESP - 0xB4) 
	mov  [eax], di 
	mov ecx, 753854h    ;Ptr_MemStruttturaSchermo
	mov  ecx, dword ptr [ecx] 
	mov  [ecx+02h], di 
	mov edx, 753854h    ;Ptr_MemStruttturaSchermo
	mov  edx, dword ptr [edx] 
	mov  [edx+04h], di 
	mov eax, 753854h    ;Ptr_MemStruttturaSchermo
	mov  eax, dword ptr [eax] 
	mov  [eax+06h], di 
	cmp  word ptr [RainSnowData.Max_Snow], di   
	jle  Salto36 
Salto23: 
	mov  eax, [ebx] 
	cmp  eax, edi 
	jnz  Salto24 
	add  ebx, 14h 
	jmp  Salto35 
Salto24: 
	mov esi, 7FE700h    ;Camera_TargetX
	mov  esi, dword ptr [esi] 
	mov  ecx, [ebx+04h] 
	mov edx, 7FE704h    ;Camera_TargetY
	mov  edx, dword ptr [edx] 
	sub  eax, esi 
	mov  [esp+10h], eax  ; (LOCALE_ESP - 0xBC) 
	mov eax, 7FE708h    ;Camera_TargetZ
	mov  eax, dword ptr [eax] 
	fild dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	sub  ecx, edx 
	mov  edx, [ebx+08h] 
	mov  [esp+1ch], ecx  ; (LOCALE_ESP - 0xB0) 
	sub  edx, eax 
	fstp dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	fild dword ptr [esp+1ch]  ; (LOCALE_ESP - 0xB0) 
	mov  [esp+14h], edx  ; (LOCALE_ESP - 0xB8) 
	fstp dword ptr [esp+1ch]  ; (LOCALE_ESP - 0xB0) 
	fild dword ptr [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	fstp dword ptr [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	push eax
	mov eax, 753B20h   ;Matrice3dView + 028h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	push eax
	mov eax, 753B10h   ;Matrice3dView + 018h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+1ch]  ; (LOCALE_ESP - 0xB0) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B00h   ;Matrice3dView + 08h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B30h   ;Matrice3dView + 038h
	fadd dword ptr [eax] 
	pop eax
	push eax
	mov eax, 753BA4h   ;FloatTotRigheCar
	fcom dword ptr [eax] 
	pop eax
	fstsw  ax 
	test ah, 01h 
	jnz  Salto33 
	mov  cl, [ebx+0dh] 
	mov  al, cl 
	and  al, 07h 
	cmp  al, 07h 
	jnc  Salto25 
	mov  dl, cl 
	and  edx, 07h 
	shl  edx, 04h 
	mov  eax, edx 
	jmp  Salto27 
Salto25: 
	mov  al, [ebx+0fh] 
	cmp  al, 20h 
	jbe  Salto26 
	mov  eax, 82h 
	jmp  Salto27 
Salto26: 
	and  eax, 0ffh 
	shl  eax, 03h 
Salto27: 
	push eax
	mov eax, 753BA8h   ;ValFloat_MisteroA
	fdivr  dword ptr [eax] 
	pop eax
	mov  edx, eax 
	and  ecx, 18h 
	or   edx, 0ffffff00h 
	shl  edx, 08h 
	or   edx, eax 
	lea  ebp, dword ptr [RainSnowData.VettoreSizeSnow][ecx*4]   
	shl  edx, 08h 
	or   edx, eax 
	xor  esi, esi 
	lea  ecx, [esp+58h]  ; (LOCALE_ESP - 0x74) 
	push eax
	mov eax, 753B18h   ;Matrice3dView + 020h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	push eax
	mov eax, 753B08h   ;Matrice3dView + 010h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+1ch]  ; (LOCALE_ESP - 0xB0) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AF8h   ;Matrice3dView
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B28h   ;Matrice3dView + 030h
	fadd dword ptr [eax] 
	pop eax
	push eax
	mov eax, 753B1Ch   ;Matrice3dView + 024h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+14h]  ; (LOCALE_ESP - 0xB8) 
	push eax
	mov eax, 753B0Ch   ;Matrice3dView + 014h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+1ch]  ; (LOCALE_ESP - 0xB0) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753AFCh   ;Matrice3dView + 04h
	fld  dword ptr [eax] 
	pop eax
	fmul dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753B2Ch   ;Matrice3dView + 034h
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	fmul st(0), st(1) 
	fld  dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	fmul st(0), st(2) 
Salto28: 
	fld  st(2) 
	fmul dword ptr [ebp+00h] 
	add  ebp, 04h 
	xor  edi, edi 
	fld  st(3) 
	fmul dword ptr [ebp+00h] 
	add  ebp, 04h 
	fstp dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	fld  st(2) 
	faddp  st(1), st(0) 
	push eax
	mov eax, 753BFCh   ;FloatSchermoX_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fld  st(1) 
	fadd dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	push eax
	mov eax, 753C00h   ;FloatSchermoY_Diviso2
	fadd dword ptr [eax] 
	pop eax
	fstp dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	push eax
	mov eax, 753C20h   ;ValFloat_MisteroC
	fcom dword ptr [eax] 
	pop eax
	fstsw  ax 
	test ah, 01h 
	jz   Salto29 
	mov  edi, 01h 
	jmp  Salto30 
Salto29: 
	push eax
	mov eax, 753BACh   ;ValFloat_MisteroB
	fcom dword ptr [eax] 
	pop eax
	fstsw  ax 
	test ah, 41h 
	jnz  Salto30 
	mov  edi, 02h 
Salto30: 
	fld  dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	push eax
	mov eax, 753C64h   ;ValFloat_MisteroE
	fcomp  dword ptr [eax] 
	pop eax
	fstsw  ax 
	test ah, 01h 
	jz   Salto31 
	add  edi, 04h 
	jmp  Salto32 
Salto31: 
	fld  dword ptr [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	push eax
	mov eax, 754034h   ;ValFloat_MisteroF
	fcomp  dword ptr [eax] 
	pop eax
	fstsw  ax 
	test ah, 41h 
	jnz  Salto32 
	add  edi, 08h 
Salto32: 
	fstp dword ptr [ecx-0ch] 
	mov  eax, [esp+10h]  ; (LOCALE_ESP - 0xBC) 
	mov  [ecx+04h], edx 
	push eax
	mov eax, 753C24h   ;ValFloat_MisteroD
	fld  dword ptr [eax] 
	pop eax
	mov  [ecx-08h], eax 
	xor  eax, eax 
	fmul st(0), st(3) 
	mov  [ecx+0ch], eax 
	mov  [ecx+10h], eax 
	mov eax, 753854h    ;Ptr_MemStruttturaSchermo
	mov  eax, dword ptr [eax] 
	mov  dword ptr [ecx+08h], 0ff000000h 
	add  esi, 02h 
	fstp dword ptr [ecx] 
	mov  [esi-02h][eax], di 
	add  ecx, 20h 
	cmp  esi, 08h 
	jl   Salto28 
	lea  ecx, [esp+24h]  ; (LOCALE_ESP - 0xA8) 
	push 01h 
	push ecx 
	push 01h 
	fstp st(0) 
	push 00h 
	lea  edx, [esp+5ch]  ; (LOCALE_ESP - 0x80) 
	fstp st(0) 
	push 02h 
	push edx 
	fstp st(0) 
	mov eax, 53399Ch    ;Ptr_AddTriClippedSorted
	call dword ptr [eax] 
	add  esp, 18h 
	jmp  Salto34 
Salto33: 
	fstp st(0) 
Salto34: 
	add  ebx, 14h 
	xor  edi, edi 
Salto35: 
	mov  eax, [esp+18h]  ; (LOCALE_ESP - 0xB4) 
	movsx  ecx, word ptr [RainSnowData.Max_Snow]   
	inc  eax 
	cmp  eax, ecx 
	mov  [esp+18h], eax  ; (LOCALE_ESP - 0xB4) 
	jl   Salto23 
Salto36: 
	mov edx, 7E70C8h    ;ptr_ZonaRecord30
	mov  edx, dword ptr [edx] 

	mov  eax, 30h 
	pop  edi 
	sub  edx, eax 

	pop  esi 
	pop  ebp 
	push eax
	mov eax, 7E70C8h   ;ptr_ZonaRecord30
	mov  dword ptr [eax], edx 
	pop eax
 
	pop  ebx 
	add  esp, 0bch 
	ret 
 


	}

}

__declspec(naked) void DoWeather (void)
{
	__asm {

	// set level flags for rain/snow
	mov eax, dword ptr [GlobTomb4.pScriptLevelNow]
	mov cx, word ptr [eax]   ;read LevelFlaga from ScriptLevelNow structure
	mov word ptr [RainSnowData.LevelFlags], cx

	mov  ax, word ptr [RainSnowData.LevelFlags]
	test ax, fngl_Rain + fngl_Snow
	jz		NoWeather

	call CambioIntensitaWeather
	
	mov  ax, word ptr [RainSnowData.LevelFlags]
	test ax, fngl_Rain
	jz	 ProvaSnow
	// eseguire pioggia 

	call DoRain
ProvaSnow:

	mov  ax, word ptr [RainSnowData.LevelFlags]
	test ax, fngl_Snow
	jz	  NoWeather
	lea  eax, DoSnow
	call eax

NoWeather:
	retn


	}
}

