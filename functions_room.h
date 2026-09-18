
// questo file creava problemi chissa' perche'
// speriamo che cambiando tutto vada a posto


typedef void (__cdecl *TYPE_DrawTextures) (void);
typedef void (__cdecl* TYPE_ModificaPulsante) (StrDatiPulsante *pPulsante, 
								DWORD ColoreFondo, DWORD ColoreTesto, 
								DWORD TestPremuto);
typedef void (__cdecl *TYPE_HighlightPalette) (bool TestImposta);
typedef void (__cdecl* TYPE_ModificaPulsanteEx) (StrDatiPulsante *pPulsante, 
								DWORD ColoreFondo, DWORD ColoreTesto, 
								DWORD TestPremuto, char* pNewTesto);

typedef void (__cdecl* TYPE_ShowTestoSuSchermo) (DWORD CordX, DWORD CordY, char *PTesto);

typedef int (__cdecl* TYPE_SaveProject) (char* NomeFile);


typedef void (__cdecl* TYPE_Sub_PulsanteFlipMap) (DWORD Qualunque);
typedef int (__cdecl* TYPE_RoomWrite) (const void *pBuffer, size_t SizeRecord, 
									   size_t NRecords, FILE *pFile);

typedef int (__cdecl* TYPE_RoomRead) (const void *pBuffer, size_t SizeRecord, 
									   size_t NRecords, FILE *pFile);
typedef int (__cdecl* TYPE_ControllaConflittoTrigger) (StrTriggerSelezionato *pNewTrigger, int OrgX, int OrgZ, int SizeX, int SizeZ, int Indice);

typedef int (__cdecl* TYPE_roomGetCategoriaTipoOggetto) (int Indice);
typedef void (__cdecl* TYPE_roomAggiornaZonaSchermo) (int OrgX, int OrgY, int SizeX, int SizeY);
typedef void (__cdecl* TYPE_roomDisegnaCornice) (int x1, int y1, int SizeX, int SizeY, int Colore1, int Colore2);
typedef void (__cdecl* TYPE_roomCreaFileTom) (char NomeTom[]);
typedef void (__cdecl* TYPE_roomAlteraFloorInput) (int IndiceStanza, int XDisplay, 
						int ZDisplay, int XSizeBlocks, int ZSizeBlocks, 
						int Operazione, int Valore);
typedef void (__cdecl* TYPE_roomBackupUndo) (void);
typedef void (__cdecl* TYPE_roomSalvaScreenShot) (char *NomeFile, int SizeX, int SizeY, 
										BYTE * pStartMemoriaVideo, DWORD pPaletteColori);
typedef int  (__cdecl* TYPE_CicloMessaggi) (void);
typedef void (__cdecl* TYPE_PulFloorPiu) (StrInfoMouse *pInfoMouse);
typedef void (__cdecl* TYPE_SubFaceEdit) (StrInfoMouse *pInfoMouse);
typedef void (__cdecl* TYPE_PulFloorMeno) (StrInfoMouse *pInfoMouse);
typedef void (__cdecl* TYPE_PulCeilingPiu) (StrInfoMouse *pInfoMouse);
typedef void (__cdecl* TYPE_PulCeilingMeno) (StrInfoMouse *pInfoMouse);
typedef void (__cdecl* TYPE_roomSmoothFloor) (int IndiceStanza, int XDisplay, 
						int ZDisplay, int XSizeBlocks, int ZSizeBlocks, 
						int Operazione, int Valore);
typedef void (__cdecl* TYPE_roomMostraFinestra) (StrDatiFinestra *pFinestra);
typedef void (__cdecl* TYPE_roomChiudiFinestra) (StrDatiFinestra *pFinestra);
typedef void (__cdecl* TYPE_roomEsecuzioneProgramma) (HMODULE hModulo);
typedef int (__cdecl* TYPE_roomCicloMessaggi) (void);
typedef void (__cdecl* TYPE_roomBoundRoom) (void *);
typedef void (__cdecl* TYPE_roomCreaDoor) (void *);
typedef void (__cdecl* TYPE_roomCreaFlipMap) (void *);
typedef void* (__cdecl* TYPE_roomChiediMemoria)(int SizeMemoria, char *pMex);
typedef void* (__cdecl* TYPE_roomMostraTrigger) (void);
typedef int* (__cdecl* TYPE_roomMouseInRettangolo) (int OrgX, int OrgY, int SizeX, int SizeY);
typedef void (__cdecl* TYPE_roomShowNomeRoom) (char *pNomeRoom, int NumeroFlipMap);
typedef void (__cdecl* TYPE_roomRefreshSchermoEValori) (void);
typedef void (__cdecl* TYPE_roomCenterRoom)(void *Punta);

typedef void (__cdecl* TYPE_DisegnaRettangolo) (DWORD OrgX, DWORD OrgY, 
												DWORD SizeX, DWORD SizeY, DWORD Colore);
typedef void (__cdecl *TYPE_Sub_FindTexture) (DWORD Qualunque);
typedef void (__cdecl* TYPE_roomMessaggioLog) (int Numero, char Messaggio[]);
typedef void (__cdecl* TYPE_InitialiseObjects) (void); 
typedef void * (__cdecl *TYPE_ReallocNGLE) (void *pOldMemory, DWORD NewMemory);
typedef void (__cdecl *TYPE_toggle_bool) (StrDatiPulsante *pPulsante);
typedef void (__cdecl *TYPE_LeggiDirectInput) (int Valore);
typedef bool (__cdecl *TYPE_LoadTGAFile) (void);
typedef void (__cdecl *TYPE_Draw3DWindow) (void);
typedef void (__cdecl *TYPE_RemoveTrigger) (short ObjTriggerIndex, short RoomIndex);
typedef void (__cdecl *TYPE_RemoveObject) (short ObjectIndex, short RoomIndex);

TYPE_RemoveObject RemoveObject = (TYPE_RemoveObject) 0x423C60;
TYPE_RemoveTrigger RemoveTrigger = (TYPE_RemoveTrigger) 0x42DB80;
TYPE_Draw3DWindow Draw3DWindow = (TYPE_Draw3DWindow) 0x401200;
TYPE_LoadTGAFile LoadTGAFile = (TYPE_LoadTGAFile) 0x406EE0;
TYPE_LeggiDirectInput LeggiDirectInput = (TYPE_LeggiDirectInput) 0x4440D0;
TYPE_toggle_bool toggle_bool = (TYPE_toggle_bool) 0x433FE0;
TYPE_ReallocNGLE ReallocNGLE = (TYPE_ReallocNGLE) 0x449B47;
TYPE_InitialiseObjects InitialiseObjects = (TYPE_InitialiseObjects) 0x41FD90;
TYPE_roomMessaggioLog roomMessaggioLog=(TYPE_roomMessaggioLog) 0x449870;
TYPE_SubFaceEdit SubFaceEdit=(TYPE_SubFaceEdit) 0x4104E0;
TYPE_ModificaPulsante ModificaPulsante = (TYPE_ModificaPulsante) 0x4148e0;
TYPE_ModificaPulsanteEx ModificaPulsanteEx = (TYPE_ModificaPulsanteEx) 0x414910;
TYPE_ShowTestoSuSchermo ShowTestoSuSchermo = (TYPE_ShowTestoSuSchermo) 0x444020;
TYPE_SaveProject SaveProject = (TYPE_SaveProject) 0x4275e0;
TYPE_DisegnaRettangolo DisegnaRettangolo = (TYPE_DisegnaRettangolo) 0x43692E;
TYPE_Sub_PulsanteFlipMap Sub_PulsanteFlipMap = (TYPE_Sub_PulsanteFlipMap) 0x40E130;
TYPE_RoomWrite RoomWrite=(TYPE_RoomWrite) 0x44AD68;
TYPE_RoomRead RoomRead=(TYPE_RoomRead) 0x44A4D9;
TYPE_roomCicloMessaggi roomCicloMessaggi = (TYPE_roomCicloMessaggi) 0x444060;
TYPE_roomEsecuzioneProgramma roomEsecuzioneProgramma = (TYPE_roomEsecuzioneProgramma) 0x444190;
TYPE_roomMostraFinestra roomMostraFinestra = (TYPE_roomMostraFinestra) 0x4374B0;
TYPE_roomChiudiFinestra roomChiudiFinestra = (TYPE_roomChiudiFinestra) 0x4376E3;
TYPE_roomSmoothFloor roomSmoothFloor = (TYPE_roomSmoothFloor) 0x40B040;
TYPE_PulCeilingPiu PulCeilingPiu = (TYPE_PulCeilingPiu) 0x40D0D0;
TYPE_PulCeilingMeno PulCeilingMeno = (TYPE_PulCeilingMeno) 0x40D110;
TYPE_PulFloorPiu PulFloorPiu = (TYPE_PulFloorPiu) 0x40D050;
TYPE_PulFloorMeno PulFloorMeno = (TYPE_PulFloorMeno) 0x40D090;
TYPE_roomSalvaScreenShot roomSalvaScreenShot = (TYPE_roomSalvaScreenShot) 0x42BB60;
TYPE_roomBackupUndo roomBackupUndo = (TYPE_roomBackupUndo) 0x408030;
TYPE_roomAlteraFloorInput roomAlteraFloorInput = (TYPE_roomAlteraFloorInput) 0x40A870;
TYPE_roomCreaFileTom roomCreaFileTom = (TYPE_roomCreaFileTom) 0x42DDB0;
TYPE_roomDisegnaCornice roomDisegnaCornice = (TYPE_roomDisegnaCornice) 0x436762;
TYPE_roomAggiornaZonaSchermo roomAggiornaZonaSchermo = (TYPE_roomAggiornaZonaSchermo) 0x43662E;
TYPE_roomGetCategoriaTipoOggetto  roomGetCategoriaTipoOggetto =
			(TYPE_roomGetCategoriaTipoOggetto) 0x421240; // GetCategoriaTipoOggetto
TYPE_HighlightPalette  HighlightPalette  = (TYPE_HighlightPalette) 0x410430; 
TYPE_roomCenterRoom roomCenterRoom = (TYPE_roomCenterRoom) 0x426010;
TYPE_roomRefreshSchermoEValori roomRefreshSchermoEValori = (TYPE_roomRefreshSchermoEValori) 0x41A4A0;
TYPE_roomShowNomeRoom roomShowNomeRoom = (TYPE_roomShowNomeRoom) 0x413580;
TYPE_roomMouseInRettangolo roomMouseInRettangolo = (TYPE_roomMouseInRettangolo) 0x434000;
TYPE_roomMostraTrigger roomMostraTrigger = (TYPE_roomMostraTrigger) 0x413770;
TYPE_roomChiediMemoria roomChiediMemoria = (TYPE_roomChiediMemoria) 0x41A440;
TYPE_roomCreaFlipMap roomCreaFlipMap = (TYPE_roomCreaFlipMap) 0x40DDB0;
TYPE_roomCreaDoor roomCreaDoor = (TYPE_roomCreaDoor) 0x408930;
TYPE_roomBoundRoom roomBoundRoom = (TYPE_roomBoundRoom) 0x40D250;
TYPE_ControllaConflittoTrigger ControllaConflittoTrigger = (TYPE_ControllaConflittoTrigger) 0x42D9E0;

TYPE_Sub_FindTexture Sub_FindTexture = (TYPE_Sub_FindTexture) 0x42A1B0;
TYPE_CicloMessaggi CicloMessaggi = (TYPE_CicloMessaggi) 0x444060;
TYPE_DrawTextures DrawTextures = (TYPE_DrawTextures) 0x42AB00;