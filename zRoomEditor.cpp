
#include "stdafx.h"
#include "stdio.h"
#include "mioCutscene.h"
#include <dshow.h>
#include <sys/stat.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdarg.h>
#include <math.h>
#include <winuser.h>
#include <direct.h>
#include <time.h>
#include <zmouse.h>
#include "bass.h"
#include "RoomEditor.h"
#include "myconstants.h"
#include "macros.h"
#include "functions_room.h"

#pragma warning( error : 4706 )

#define DatiMouseStandard 0x45E4E0


// GLOBALI PER TOM2PC --------------------------------------------------
int TestCryptTr4 = 0;
// FINE GLOBALEI TOM2PC ------------------------------------------------



// ---------------- VARIABILI ESTERNE ----------------------------------

extern char BufferLog[8192];
extern int VetStandardSwapMesh[];
extern int TestAttivaCryptTr4; //usato in tom2pc per segnale di [CryptTr4] 
extern int VetMeshSwapSlot[];
extern char Dir_Trle[256];
extern HINSTANCE MiaIstanzaDll;
extern char BufferTemp[256];
extern DWORD SalvaStack;
extern int VetSlotMeshMoveables[];
extern int TipoExe; 
extern int NumeroBloccoCrypt; // segnala se e' tex1,tex2,tex3 o geometry
extern char NomeTr4Pc[256];	
extern char MyTempFolder[256];
extern char MyCrashFolder[256];
// ----------------- FINE ESTERNE ---------------------------------------
StrGlobNgle GlobNgle; 
int TestTgaProject; // se =1 si sta caricando dal progetto  altrimenti da pulsante
int VetIndexTextureDD[16384];
int SchermoOffY=0;
DWORD *pVetPtrOriginaliMesh=NULL;
bool TestPlayInCorso=false;
char *VetDescrizioniRemark[1024];
char *VetDescrizioniHelp[1024];
int TotDescrizioniRemark;
HICON IconaRoom;
HWND lp_WindListBox;
StrBaseButtonsWindow BaseWindow;
bool TestCreaNewAnimRanges; // usato in tom2pc per remap tail
char OldFrameInfoText[3000];
StrListElenco *pZonaVetList;
int TGA_SizeX;
int TGA_SizeY;
int LastRoomPlay; // stanza selezionata prima di [Play]
bool TestAvviaTomb=false;
HBITMAP HandleBitMap=NULL;
HBITMAP OldBmp;  // vecchio bitmap nell'hdc compatibile da usare per eliminare tutto
char NomeClasseNG[]="NG_ROOM";
ATOM hClasseNG;
WORD SimulaKey=0;
WORD SimulaStatusKey=0;
StrMiaMemory BaseMiaMemory;
WORD OldSimulaKey=0;
WORD OldSimulaStatusKey=0;
WORD NuovoIndiceObj;
int TipoListBox;
int IndiceTailNow;
DWORD VetNuovoRecordTexFaces[0x0c];
DWORD SwapDword;
int EsitoMoveable;
BYTE *pMappaTexture;
StrGlobMetaPc BaseMeta;
char MexProgrammaTerminato[] = "ABORT PROGRAM";
int SizeMappaTexture;
char MexFareBackup[] = "Unrecoverable error. The program will be terminated. Do you want save current project in new backup file?";
char MexMancaLetteraTrigger[] = "ERROR: missing name or first letter for trigger category\r\n\r\nYou have to type in front of the number, a letter to decide trigger category: example,  \"F234\"  or \"A44\"  or \"C53\".\r\n\r\nF= Flipeffect  ;  C=Condition  ; A=Action";

int UltimoIndiceOggetto=-1;
int IndiceStaticNow; // conta gli static di stanza
DWORD LastTimeCheckMemory=0;
WORD IndiceTomTailSource; // usato in tom2pc per remap tail
BYTE VetOutTexBumpMap[MAX_TAIL_INFOS];
void RefreshButtons(HDC MioHdc);
int TotPtrOriginaliMesh=0;
DWORD LastTimeMessaggio;
bool TestMessaggio;
int TestBloccoUndo=0;
bool TestLoadProject;
char NomeLoadProject[256];
HWND WindowLastProjects;
// ultima stanza e coordinata scandita da room editor
int ScanRoom;
int ScanCordX;
int ScanCordZ;
StrNomeSlot  *pVetSalvaNomiSlot=NULL;
int TotSalvaNomiSlot;

StrNGConstants MainBaseCostanti;
struct StrTr4Zl BaseTr4Zl;
DWORD VetCounterAnim[60]; // tiene conto di quando mostrare next frame animazione
long QuantoLungo(FILE *tempfile);
double TrovaRaggioLatoY(double Lato, double h, double *pRaggio);
int TrovaClickSfera(double x, double z, double R);
DWORD ApplicaPatchMemoria(StrPatch VetPatch[], int TotPatch, DWORD OffsetOldInizio, 
				  DWORD OffsetOldFine, DWORD NewSizeMem, char NomeZonaMemoria[]);
void PatchTotSizeSegnaliMesh(void);

int __cdecl GestioneEccezioni(
								EXCEPTION_RECORD *pExceptRecord,
								void *pFrame, 
								TIPO_CONTEXT *pContesto,
								void *pDispatcher);
bool EsisteFile(char *NomeFile);
void AddZonaMemoria(DWORD StartOffset, DWORD SizeMemory, char *pName);
void PatchGenericheRoom(void);
void InitTabellaGrid(StrStanzaRoomEdit *pRoom);
void Menu_InfoAboutProject(void *pPunta);

int Float2Int(float x);
void SelezionaListBoxValore(HWND WindList, int Valore);
void FineFindReplace(void);
void CreaButtonWindow(void);
bool FreeMine(void *pMemory);
void AnalisiErroreDirectX(char *pMexErrore, DWORD LastReturn);
void ApplicaPatchDato(StrTotElementi VetPatch[], int TotPatch, 
					   DWORD TotOld, DWORD TotNew);

void SalvaWaterIntensity(StrStanzaRoomEdit *pRoom);
void Menu_SmoothSlope(void *pPunta);
void Menu_CSteppedSlope(void *pPunta);
StrListaFiles *TrovaFiles(char DirSource[], char Maschera[], int *pTotFiles);
bool IsMoveable(short Indice, bool TestExport);
char *SoloNomeSenzaExt(char FileName[]);
void ShowDatiTexParziali(StrBaseParziali *pBaseParziali);
void Menu_CSmoothSlope(void *pPunta);
bool ParseNgField(WORD *pNgArray, DWORD CurrentIndex, 
										StrParseNGField* pParseNGField);
void ShowStatistichePC(HWND hDialogo, bool TestShowTesto );
void GosubFasullaPoppa(int NumeroPulsante);
char *TrovaNomeMessaggio(DWORD MessaggioWM);
int ConfrontaDate(SYSTEMTIME *pPrima, SYSTEMTIME *pSeconda);
void Menu_SteppedSlope(void *pPunta);
void SetFunctionArray(void* VetFunzioni[]);
void Menu_RandomFloorUp(void *pPunta);
void CopiaInHeader(WORD NGTag, DWORD TotItem, 
				   DWORD SizeSingleItem, void *pDato,
				   WORD **p2VetExtra, int *pNWords);
void Menu_RandomFloorDown(void *pPunta);
void Menu_RandomCeilingUp(void *pPunta);
bool WriteNGHeader(char *pNomeFile, WORD *VetExtra, DWORD NWords);
void Menu_RandomCeilingDown(void *pPunta);
int Casuale(int Massimo);
void Menu_BendFloor(void *pPunta);
DWORD GetSizeMemTempTexture(void);
bool LeggePluginScript(char *pNomeScript);
void Menu_BendInvFloor(void *pPunta);
bool ExtractNGHeader(char *pNomeFile, StrExtractNG* pExtractNG);
void Menu_BendCeiling(void *pPunta);
void Menu_BendInvCeiling(void *pPunta);
void Menu_SetRandomSlope(void *pPunta);
void Menu_CreaTriangoli(void *pPunta);
void Menu_CCreaTriangoli(void *pPunta);
void GosubFasullaPapera(void);
void SpostaSchermo(int OrgX, int OrgY);
void RefreshUsedTriggersByPlugins(void);
void CalcolaVersione(StrVersionHeader *pBaseVer);
void Menu_ClearTextureFloor(void *pPunta);
void Menu_ClearTextureCeiling(void *pPunta);
StrFlyByArrayRoomEditor* CreaListaFlyBy(int *pTotFlyBy);
void CryptaFasullaTr4(char *NomeTr4);
bool GetDataDelFileSys(char *NomeFile, SYSTEMTIME *pData);
void Menu_ShowRemapObj(void *pPunta);
bool InviaLog(char *pMessaggio);
void CriptaZona(BYTE *pZona, DWORD SizeZona);
bool IniziaShape(bool TestGetSelezione, bool TestNoWall, int MinSize, 
				 bool TestMostraWait);
void ShowListaProgrammi(HWND hDialogo);
int CreaCrashReport(DWORD *pVetStack, TIPO_CONTEXT *pContesto, 
					 EXCEPTION_RECORD *pInfoEccezione);
StrNGConstants * TrovaBaseCostanti(int IdPlugin);

char *TrovaSoundWav(char *pNomeSound);
void InitTabellaGrid(StrStanzaRoomEdit *pRoom);
void CreaCupola(bool TestCeiling, bool TestInversa);
void CalcolaVetRemapRooms(void);
void PatchMaximizeNgle(void);
void ImpostaComboTipoAnim(HWND hDialogo);
void TerminaShape(bool TestMettiSelezione, bool TestMostraWait);
char *InputTextBox(char *pDescrizione, char *pTitolo, char *pDefault, int TipoPosizione);

void OrdinaRangeAnimati(void);
void InizializzaRecordOld(StrOldProject *pRecord);
void SetMiniSel(int CasaX, int CasaY);
StrFindReplaceTex DatiFindReplace;

char *pErrorMixedTriggers = "Error: you cann't mix in same trigger CONDITION (for activation) with FLIPEFFECT or ACTION (for what trigger) because both settings require special value in TIMER field.\r\nYou should use two different triggers: first trigger with CONDITION (for Activation) and TIMER_FIELD or OBJECT or PARAMATER as 'what trigger', and second trigger with common TRIGGER for activation and ACTION or FLIPEFFECT as 'what trigger'";




StrVet20 VetCurva[] = {
		{ 2,  0,  2},
		{ 2,  0,  0,  2},
		{ 4,  1,  0,  1,  4},
		{ 7,  2,  0,  0,  2,  7},
		{ 7,  3,  1,  0,  1,  3,  7},
		{ 9,  4,  1,  0,  0,  1,  4,  9},
		{ 8,  4,  2,  0,  0,  0,  2,  4,  8},
		{ 8,  4,  2,  1,  0,  0,  1,  2,  4,  8},
		{11,  6,  3,  1,  0,  0,  0,  1,  3,  6, 11},
		{12,  6,  3,  1,  0,  0,  0,  0,  1,  3,  6, 12},
		{11,  7,  4,  2,  1,  0,  0,  0,  1,  2,  4,  7, 11},
		{11,  7,  4,  2,  1,  0,  0,  0,  0,  1,  2,  4,  7, 11},
		{12,  8,  5,  3,  2,  1,  0,  0,  0,  1,  2,  3,  5,  8, 12},
		{12,  8,  5,  3,  2,  1,  0,  0,  0,  0,  1,  2,  3,  5,  8, 12},
		{12,  8,  5,  3,  2,  1,  0,  0,  0,  0,  0,  1,  2,  3,  5,  8, 12},
		{13,  9,  6,  4,  2,  1,  0,  0,  0,  0,  0,  0,  1,  2,  4,  6,  9, 13},
		{14, 10,  7,  5,  3,  2,  1,  0,  0,  0,  0,  0,  1,  2,  3,  5,  7, 10, 14}};
DWORD VetShiftDelay[]={10,60,110,160,210,260,310,360,410,460};
		//				    1  2  3  4  5    6   7   8   9   10 11   12  13
int VetIncrementoStep[] = {10,20,40,80,120, 160,200,260,340,440,550,650,750};
int VetIncrementoMax[] = {200, 300,500, 800, 1200, 1700, 2300, 3000, 3800, 4700,5200,6100,7000};
const int TotIncrementoStep = 13;


char InputBufDescrizione[256];
char InputBufTitolo[80];
char InputBufDefault[80];

DWORD EsitoMappa;
char *VetNomiVisuale[] = {"South", "West", "North", "East"};
StrStanza2d VetStanze2d[512];
bool TestUltimaTrasparente=false;
bool TestMappaInCorso=false;  //quandoe' true si sta aggiornando mappa verticale
// salva tutte gli indirizzi di texface prese da salvaindicetexface
DWORD VetAdrTexFace[10000];
WORD  VetIndexTail[10000];  //  indice tail
int  TotAdrTexFace;		// numero di strutture salvate
WORD SalvaTotFloorData;
StrPreviewType VetPreview[] = {{293,10,805,522,"Little"},
{112, 84, 912, 684, "Default"}, {0,0,1024,768, "Big"}, 
{0,0,1024,768,"Full Screen"}};
const int TotPreview = 4;
char *VetMiniOpen[]={"NONE", "TGA", "WAD", "WAS","TR4","FOLDER"};
char *VetTipoOpen[]={"NONE", "Tga_Texture", "Wad_Objects", "Was_Objects","Tr4_Level", "Open_Folder"};
const int TotTipoOpen = 6;
const int TotMiniOpen = 6;
char MexUnmapView[] = "UnmapViewOfFile";
// Menu texture (modifica dell'originale)
char MexLoadPalette[] = "Load Palette";
char MexSavePalette[] = "Save Palette";
char MexRimuoviTailInfos[] = "Remove unused tail infos";
char MexFindAndReplaceTex[] = "Substitute Textures";
char MexCheckIntegrityTexture[] = "Check integrity textures";
char MexCenter[] = "Center     SHIFT C";
BaseNomiSlot VetMieiNomiSlot;
// Menu ScreenShots
char NomeMenu_ScreenShots[] = "ScreenShots";
char NomeMenu_ShotFullScreen[] =	"Full Screen            ALT 5";
char NomeMenu_ShotPanel2d[] =		"2d Panel              ALT 6";
char NomeMenu_ShotPanel3d[] =		"3d Panel              ALT 7";
char NomeMenu_ShotPreview[] =	    "Preview Zone          ALT 8";
char NomeMenu_ShotPreviewRGB[] = "Preview Zone (RGB)     ALT 9";
char NomeMenu_ShotTextures[] =      "Texture Zone          ALT 0";
char NomeMenu_ShotInfoRoom[] =      "Info Room Panel        ALT F11";
char NomeMenu_ShotLights[] =        "Light Panel            ALT F12";
// Menu Shapes
char NomeMenu_Shapes[] = "Shapes";
char NomeMenu_SmoothSlope[] =         "Smooth Slope Floor            CTRL F1";
char NomeMenu_SteppedSlope[] =        "Stepped Slope Floor           CTRL F2";
char NomeMenu_CSmoothSlope[] =        "Smooth Slope Ceiling           CTRL F3";
char NomeMenu_CSteppedSlope[] =        "Stepped Slope Ceiling          CTRL F4";
char NomeMenu_RandomFloorUp[] =        "Random Smooth Floor Up        ALT 1";
char NomeMenu_RandomFloorDown[] =      "Random Smooth Floor Down      ALT 2";
char NomeMenu_RandomCeilingUp[] =      "Random Smooth Ceiling Up       ALT 3";
char NomeMenu_RandomCeilingDown[]=     "Random Smooth Ceiling Down     ALT 4";
char NomeMenu_SetRandomSlope[]=        "Set Random Smooth Parameters  SHIFT F2";
char NomeMenu_CreaTriangoli[] =		   "Create Floor Triangular Tex     SHIFT F3";
char NomeMenu_CCreaTriangoli[]=        "Create Ceiling Triangular Tex    SHIFT F4";
char NomeMenu_Piramide[] =             "Pyramid Floor                CTRL F5";
char NomeMenu_InvPiramide[] =          "Inverse Pyramid Floor          CTRL F6";
char NomeMenu_CPiramide[]=             "Pyramid Ceiling                CTRL F7";
char NomeMenu_CInvPiramide[]=          "Inverse Pyramid Ceiling         CTRL F8";
char NomeMenu_BendFloor[] =			   "Bend Floor                   CTRL 1";
char NomeMenu_BendInvFloor[] =         "Inverse Bend Floor            CTRL 2";
char NomeMenu_CBend[] =                "Bend Ceiling                 CTRL 3";
char NomeMenu_CBendInv[]=			   "Inverse Bend Ceiling           CTRL 4";
char NomeMenu_Cupola[]=				   "Dome Floor                   CTRL 5";
char NomeMenu_CupolaInv[]=             "Inverse Dome Floor            CTRL 6";
char NomeMenu_CCupola[]=               "Dome Ceiling                 CTRL 7";
char NomeMenu_CCupolaInv[]=            "Inverse Dome Ceiling           CTRL 8";
char NomeMenuGridAllWall[] = "Grid all walls";
char NomeMenuGridSelectedWall[]="Grid selected wall";
char NomeMenuRemoveGridSelectedWall[]="Remove editing from selection   CTRL F11";
char NomeMenuRemoveGridAllWall[] =    "Remove all grids             CTRL F12";
char NomeMenuRuotaTexture[] =         "Turn Selected Textures";
char NomeMenuClearTextureFloor[] =    "Clear Floor Selected Textures    ALT F9";
char NomeMenuClearTextureCeiling[] =  "Clear Ceiling Selected Textures   ALT F10";

// Menu Help
char NomeMenu_Help[] = "Help";
char NomeMenu_HelpAbout[] = "About NGLE";
char NomeMenu_HelpUpdateNG[] = "Check for updates";
char NomeMenu_HelpKeyboardCommand[] = "Keyboard Commands";
char NomeMenu_ShowButtonsWindowsHelp[] = "Buttons Window";
char NomeMenu_ShowRemapObj[]	=	  "Object IDs for Trep";
char NomeMenu_InfoAboutProject[] =    "Info about project         SHIFT F1";
char NomeMenu_ShowDefaultHelp[]=	  "Default Level Editor Help";
char NomeMenu_ShowRevisedHelp[]=      "Revised Level Editor Help";
char NomeMenu_ShowNGHelp[]=			  "NGLE Help";
char NomeMenu_Plugins[]=			   "Plugins";
// menu Plugins
char NomeMenu_InfoMountedPlugins[] = "Infos about mounted Plugins";
char NomeMenu_ListUnhandledTriggers[] = "List of unhandled Triggers";
char NomeMenu_ShowTriggerListDiPlugin[] = "Show Triggers owned by Plugin";
char NomeMenu_ChangePluginId[] = "Change ID of Plugin";
char NomeMenu_RemoveTriggerDiPlugin[] = "Remove Triggers of Plugin";

char *VetDescWav[] = {
		"MUD:  fs_mud1, fs_mud2", 
		"SNOW:  fs_sn1, fs_sn2, fs_sn3 (hardcoded)",
		"SAND:  fs_snd1, fs_snd2", 
		"GRAVEL:  fs_grv1, fs_grv2",
		"ICE:  fs_ice1, fs_ice2 (hardcoded)", 
		"WATER:  fsw1, fsw2, fsw3, fsw4 (hardcoded)",
		"STONE:  foot01, foot02, foot03, foot04", 
		"WOOD_1:  fs_wd1, fs_wd2",
		"METAL_1:  fs_met1, fs_met2",
		"MARBLE:  fs_mb1, fs_mb2, fs_mb3, fs_mb4  (hardcoded)",
		"GRASS: fs_snd1, fs_snd2 (remixed sand)",
		"CONCRETE: (hardcoded)",
		"WOOD_2:  fs_wd1, fs_wd2 (remixed wood_1)",
		"METAL_2: fs_met1, fs_met2  (remixed metal_1)"};

char * VetNomiFloor[] = {"Trigger","Overlap", "Unknown_5","Opacity", 
						"Death", "Box","Climb_East", "Climb_South", 
						"Climb_West","Climb_North", "Unknown_1", "Floor_Door", 
						"Opacity2", "Unknown_2", "Monkey", "Unknown_3",
						"Unknown_4", "NoCollision_South_Floor_Triangle", 
						"NoCollision_North_Floor_ Triangle", 
						"NoCollision_South_Ceiling_Triangle", 
						"NoCollision_North_Ceiling_Triangle", 
						"Triggerer", "Beetle", "Climb_Any"};
int MostraMessageBoxNGLE(char Messaggio[],  int TipoMessageBox );
int VetCollegate[512];  // indici di stanze collegate a quella attuale
int TotCollegate;
short VetStack_74[0x400]; // vettore che sostituisce vettore stack di 256 word
StrRobaTexture DatiTexture;
HPEN PenNulla, PenNero, PenVerde, PenRosso;
HBRUSH BrushNero, BrushNullo, BrushRosso;
char NomeFileWad[256];
StrTailInfo VetBigTails[MAX_TAIL_INFOS];

StrTailInfo VetTempTails[MAX_TAIL_INFOS];
int  TempTotTails;
BYTE VetTempSonorePrj[MAX_TAIL_INFOS];

char *VetNomiList[MAX_ITEM_FIND_LIST];
int VetPuntaList[MAX_ITEM_FIND_LIST];
int TotList;
void CreaCupola(bool TestCeiling, bool TestInversa);
bool TestListOrdinato=false;
StrDatiRotazioni DatiRotazioni;
extern DWORD ValEsito;
StrTestoFinestra DatiTestoFinestra;
StrDatiFinestra DatiWaitWindow;
DWORD VetFlagFloor[24];
StrSalvaTailInfo VetSalvaTail[256];
int TotSalvaTail;
StrRoomAnimaUV VetRoomAnimaUV[40]; // usata solo in preview
	HBRUSH ColoreControllo;
	HBRUSH ColoreFondo;
StrRobaShape DatiShape;
HWND WindComboProgrammi;
StrDatiParabola DatiParabola;
char MexVersione[30];
StrPunto3d Centro;   // centro della sfera, usato per cupole e bending
HCURSOR CursoreClessidra;
int RetValore;
char LastErroreLog[512];
int OldNumeroObjects;
RECT PosizioneLastRoom;
DWORD AdrResult;

int OldEffects;
int SalvaIndiceRoom;
StrBaseRotazioni BaseRotazioni;
int TestRemoveItems=0;
int EsisteErrore;
int TestBloccoTRY;
OPENFILENAME ofn;
int	 VersioneProject=49;
bool TestLoadedProject=false;
bool TestFattoBackup=false;
BYTE *pVetTextureBumpMap;
bool TestIgnoraInput=false; // ignora input in LeggiComandiTasti
int TestMappaVerticale=0;
int TipoMappaVerticale=0;
DWORD LastTimeStandBy=0; // ultima volta che e' stato ricevuto un comando
char LastErrorDX[1024];
DWORD LastTimeMsgBox;  // ultimo tempo di msgbox da quando msgbox viene chiuso
						// fino al prossimo messggio
int TotMsgbox;  // nmumero di msgbox consecutvii
bool TestIgnoraMsgBox; // ignora msgbox 

char NomeLastSavedProject[256];
StrBaseTempTrigger BaseTempTrigger;
char LastInfoRoom[512];
bool TestShowTitleInfo;
char LastTitleWindow[512];
char LastMexSelezione[256];
int TotCambiBackup;
DWORD RetValue;
char GlobNomeTom[256];
StrExtractNG LastNGHeader;
short *pGlobRemapObj;
short *pGlobRemapRoom;
StrSalvaVettoriRemap SalvaVettoriRemap;
DWORD GlobHandleFileLog;
bool TestAttivatoCrash;
StrTriggerSelezionato* PtrUltimoTrigger;
StrStanzaRoomEdit SalvaRoomCollision;
BYTE *pSalvaGridCollision=NULL;

// prototipi miei
void Sub_PulsanteSpotIn(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteSpotOut(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteSpotInt(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightCut(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightLen(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightY(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightX(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightBlu(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteLightVerde(StrInfoMouse *pDatiPulsante);
void RiordinaRangeAnimati(void);
void Sub_PulsanteLightRosso(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteObjectBlu(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteObjectVerde(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteObjectRosso(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteBackup(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteVisuale2d(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteAmbienteBlu(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteAmbienteVerde(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteAmbienteRosso(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteDamage(void);
void SalvaMappaTexture(FILE *pFile, int NTailExport);
void Sub_PulsanteBigTexture(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteRemoveItems(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteMoveLara(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteRotate(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteGoBack(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteSettings(StrInfoMouse *pDatiPulsante);
WORD * TrovaOggetti(WORD Slot,  int *pTotTrovati);
void Sub_PulsanteNewRoomUp(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteNewRoomDown(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteNewTunnel(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteSearch(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteV2dMap(StrInfoMouse *pDatiPulsante);
void Sub_PulsanteExitAndPlay(StrInfoMouse *pDatiPulsante);
void Sub_PulsantePlay(StrInfoMouse *pPulsante);
void Sub_PulsanteGo(StrInfoMouse *pDatiPulsante);
void ShowFinestraTrigger(void);
void SalvaUltimoMsg(char **pVetMex);

void ImpostaComboSpeed(HWND hDialogo);
bool EsisteDirectory(char *NomeDir);
char NomePulsanteBackup[]= "Backup Project";
char NomePulsanteGo[] = "GO";
char NomePulsanteDamage[]= "DMG";
char NomePulsanteBigTexture[] = "Big Texture";
char NomePulsanteRemoveItems[] = "Hide Objects";
char NomePulsanteRotate[] = "Rotate Object";
char NomePulsanteGoBack[] = "Go back old room";
char NomePulsanteSettings[] = "Settings";
char NomePulsanteNewRoomUp[] = "New Up";
char NomePulsanteNewTunnel[] = "New Tunnel Room";
char NomePulsanteNewRoomDown[] = "New Down";
char NomePulsanteSearch[] = "Advanced Search";
char NomePulsanteMoveLara[] = "Move Lara here";
char NomePulsanteV2dMap[] = "V2d Map";
char NomePulsanteVisuale2d[] = "South";
char NomePulsanteExitAndPlay[] = "Exit & Play";
char NomePulsantePlay[] = "Play";
void Menu_RemoveGridSelectedWall(void *pPunta);
void Menu_GridAllWall(void *pPunta);
StrFindRoom BaseFind;
extern StrAdrMemory VetMemoryInventory[];
extern StrAdrMemory VetMemorySavegame[];
extern StrAdrMemory VetMemoryAnimation[];
extern StrAdrMemory VetItemMemory[];
extern StrAdrMemory VetSlotMemory[];
extern StrAdrMemory VetCodeMemory[];
// nomi costanti per seeunza item in record inventaria
char *VetNomiInventoryRecords[] = { 
		"UZI_ITEM", "PISTOLS_ITEM", "SHOTGUN_ITEM", "SIXSHOOTER_ITEM", 
		"SIXSHOOTER_ITEM + LASER_SIGHT", "CROSSBOW_ITEM", "CROSSBOW_ITEM + LASER_SIGHT", "GRENADE_GUN_ITEM", 
		"SHOTGUN_AMMO1_ITEM", "SHOTGUN_AMMO2_ITEM", "GRENADE_GUN_AMMO1_ITEM", "GRENADE_GUN_AMMO2_ITEM", 
		"GRENADE_GUN_AMMO3_ITEM", "CROSSBOW_AMMO1_ITEM", "CROSSBOW_AMMO2_ITEM", "CROSSBOW_AMMO3_ITEM", 
		"SIXSHOOTER_AMMO_ITEM", "UZI_AMMO_ITEM", "PISTOLS_AMMO_ITEM", "LASERSIGHT_ITEM", 
		"BIGMEDI_ITEM", "SMALLMEDI_ITEM", "BINOCULARS_ITEM", "FLARE_INV_ITEM", 
		"COMPASS_ITEM", "MEMCARD_LOAD_INV_ITEM", "MEMCARD_SAVE_INV_ITEM", "WATERSKIN1_EMPTY", 
		"WATERSKIN1_1", "WATERSKIN1_2", "WATERSKIN1_3", "WATERSKIN2_EMPTY", 
		"WATERSKIN2_1", "WATERSKIN2_2", "WATERSKIN2_3", "WATERSKIN2_4", 
		"WATERSKIN2_5", "PUZZLE_ITEM1", "PUZZLE_ITEM2", "PUZZLE_ITEM3", 
		"PUZZLE_ITEM4", "PUZZLE_ITEM5", "PUZZLE_ITEM6", "PUZZLE_ITEM7", 
		"PUZZLE_ITEM8", "PUZZLE_ITEM9", "PUZZLE_ITEM10", "PUZZLE_ITEM11", 
		"PUZZLE_ITEM12", "PUZZLE_ITEM1_COMBO1", "PUZZLE_ITEM1_COMBO2", "PUZZLE_ITEM2_COMBO1", 
		"PUZZLE_ITEM2_COMBO2", "PUZZLE_ITEM3_COMBO1", "PUZZLE_ITEM3_COMBO2", "PUZZLE_ITEM4_COMBO1", 
		"PUZZLE_ITEM4_COMBO2", "PUZZLE_ITEM5_COMBO1", "PUZZLE_ITEM5_COMBO2", "PUZZLE_ITEM6_COMBO1", 
		"PUZZLE_ITEM6_COMBO2", "PUZZLE_ITEM7_COMBO1", "PUZZLE_ITEM7_COMBO2", "PUZZLE_ITEM8_COMBO1", 
		"PUZZLE_ITEM8_COMBO2", "KEY_ITEM1", "KEY_ITEM2", "KEY_ITEM3", 
		"KEY_ITEM4", "KEY_ITEM5", "KEY_ITEM6", "KEY_ITEM7", 
		"KEY_ITEM8", "KEY_ITEM9", "KEY_ITEM10", "KEY_ITEM11", 
		"KEY_ITEM12", "KEY_ITEM1_COMBO1", "KEY_ITEM1_COMBO2", "KEY_ITEM2_COMBO1", 
		"KEY_ITEM2_COMBO2", "KEY_ITEM3_COMBO1", "KEY_ITEM3_COMBO2", "KEY_ITEM4_COMBO1", 
		"KEY_ITEM4_COMBO2", "KEY_ITEM5_COMBO1", "KEY_ITEM5_COMBO2", "KEY_ITEM6_COMBO1", 
		"KEY_ITEM6_COMBO2", "KEY_ITEM7_COMBO1", "KEY_ITEM7_COMBO2", "KEY_ITEM8_COMBO1", 
		"KEY_ITEM8_COMBO2", "PICKUP_ITEM1", "PICKUP_ITEM2", "PICKUP_ITEM3", 
		"PICKUP_ITEM4", "PICKUP_ITEM1_COMBO1", "PICKUP_ITEM1_COMBO2", "PICKUP_ITEM2_COMBO1", 
		"PICKUP_ITEM2_COMBO2", "PICKUP_ITEM3_COMBO1", "PICKUP_ITEM3_COMBO2", "PICKUP_ITEM4_COMBO1", 
		"PICKUP_ITEM4_COMBO2", "QUEST_ITEM1", "QUEST_ITEM2", "QUEST_ITEM3", 
		"QUEST_ITEM4", "QUEST_ITEM5", "QUEST_ITEM6", "BURNING_TORCH_ITEM", 
		"CROWBAR_ITEM", "CLOCKWORK_BEETLE", "CLOCKWORK_BEETLE_COMBO2", "CLOCKWORK_BEETLE_COMBO1", 
		"EXAMINE1", "EXAMINE2", "EXAMINE3", NULL};
int VetExtraLaraSlots[] = {1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
								25,26,27,28,29,30, 33, 417,418,419,420,421,466,468,469,493,501,503,505,
								507,508,509,510,511,512,513,514,515,516,517,518,519};

char *pVetMesi[] = {".....", "January", "February", "March", "April", "May", "June", 
			"July", "August", "September", "October", "November", "December"};

char *VetNomiEffects[] = {"LIGHT", "SHADOW", "SUN", "LIGHTEFFECT", 
			"SPOT", "FOGBULB", "SOUND", "SINK", 
			"CAMERA", "CAMERA_FIXED", "CAMERA_FLYBY"};
WORD VetIdEffects[] = {0x4000, 0x6000, 0x4200, 0x5000,
			0x4100, 0x4020, 0x4C00, 0x4400,
			0x4800, 0x4080, 0x4040};
const int TotEffects = 11;

// dati per comando GoBack old room -----

BYTE OldSalvaRoom[0x1d1];	// salva i dati di room attuale
int OldIndiceRoom;	// indice di ultima stanza salvata in OldSalvaRoom
BYTE OldGridRoom[20 * 20 * 0x8c];  //zona per tutti i dati di tabgrid di oldroom
DWORD OldSizeGrid;  // dimensione in byte di zona grid
DWORD OldLastTime;  // tick count di ultimo controllo effettuato
int OldIndiceProject; // Indice di ultimo progetto

// fine dati per comando GoBack old room ----
StrPrefRoom MyPrefRoom;
char NomePrefRoom[] = "ng_settings.bin";
StrDatiPulsante DatiPulsanteNewTunnel = { 
				NULL, 
				0x31, // flag per pulsante room
				1,332+4*16, // Origine
				104,16, // Size
				0, // Reserved
				&Sub_PulsanteNewTunnel, 
				0x00, // colore testo
				0x0a};  // colore fondo (giallo)


StrDatiPulsante DatiPulsanteNewRoomDown = { 
				&DatiPulsanteNewTunnel, 
				0x31, // flag per pulsante room
				1+52,332+3*16, // Origine
				52,16, // Size
				0, // Reserved
				&Sub_PulsanteNewRoomDown, 
				0x00, // colore testo
				0x0a};  // colore fondo (giallo)

StrDatiPulsante DatiPulsanteNewRoomUp = { 
				&DatiPulsanteNewRoomDown, 
				0x31, // flag per pulsante room
				1,332+3*16, // Origine
				52,16, // Size
				0, // Reserved
				&Sub_PulsanteNewRoomUp, 
				0x00, // colore testo
				0x0a};  // colore fondo (giallo)

StrDatiPulsante DatiPulsanteSettings = { 
				&DatiPulsanteNewRoomUp, 
				0x31, // flag per pulsante room
				710,714, // Origine
				95,16, // Size
				0, // Reserved
				&Sub_PulsanteSettings, 
				0x00, // colore testo
				0x02};  // colore fondo (grigio scuro)

StrDatiPulsante DatiPulsanteGo = { 
				&DatiPulsanteSettings, 
				0x31, // flag per pulsante room
				680,714, // Origine
				25,16, // Size
				0, // Reserved
				&Sub_PulsanteGo, 
				0x00, // colore testo
				0x02};  // colore fondo (grigio scuro)

StrDatiPulsante DatiPulsanteRemoveItems = { 
				&DatiPulsanteGo, 
				0x31, // flag per pulsante room
				179,574, // Origine
				103,16, // Size
				0, // Reserved
				&Sub_PulsanteRemoveItems, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)

StrDatiPulsante DatiPulsantePlay = { 
				&DatiPulsanteRemoveItems, 
				0x31, // flag per pulsante room
				179,543, // Origine
				103,16, // Size
				0, // Reserved
				&Sub_PulsantePlay, 
				0x00, // colore testo
				0x0a};  // colore fondo (giallo)

StrDatiPulsante DatiPulsanteExitAndPlay = { 
				&DatiPulsantePlay, 
				0x31, // flag per pulsante room
				179,558, // Origine
				103,16, // Size
				0, // Reserved
				&Sub_PulsanteExitAndPlay, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)




StrDatiPulsante DatiPulsanteVisuale2d = { 
				&DatiPulsanteExitAndPlay, 
				0x31, // flag per pulsante room
				232,526, // Origine
				51,16, // Size
				0, // Reserved
				&Sub_PulsanteVisuale2d, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)

StrDatiPulsante DatiPulsanteV2dMap = { 
				&DatiPulsanteVisuale2d, 
				0x31, // flag per pulsante room
				180,526, // Origine
				51,16, // Size
				0, // Reserved
				&Sub_PulsanteV2dMap, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)

StrDatiPulsante DatiPulsanteMoveLara = {
				&DatiPulsanteV2dMap,
				0x31,
				176 , 434,  // origine
				105,16,  // size
				0,  //reserved
				&Sub_PulsanteMoveLara, 
				0x00, 
				0x02};

StrDatiPulsante DatiPulsanteSpotIn = {
				&DatiPulsanteMoveLara,
				0x431,
				178 , 699,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteSpotIn, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteSpotOut = {
				&DatiPulsanteSpotIn,
				0x431,
				178 , 681,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteSpotOut, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteSpotInt = {
				&DatiPulsanteSpotOut ,
				0x431,
				178 , 663,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteSpotInt, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightCut = {
				&DatiPulsanteSpotInt ,
				0x431,
				82 , 717,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightCut, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightLen = {
				&DatiPulsanteLightCut,
				0x431,
				82 , 699,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightLen, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightY = {
				&DatiPulsanteLightLen,
				0x431,
				82 , 681,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightY, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightX = {
				&DatiPulsanteLightY ,
				0x431,
				82 , 663,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightX, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightBlu = {
				&DatiPulsanteLightX,
				0x431,
				242 , 699,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightBlu, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightVerde = {
				&DatiPulsanteLightBlu ,
				0x431,
				242 , 681,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightVerde, 
				0x01, 
				0x01};


StrDatiPulsante DatiPulsanteLightRosso = {
				&DatiPulsanteLightVerde ,
				0x431,
				242 , 663,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteLightRosso, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteObjectBlu = {
				&DatiPulsanteLightRosso ,
				0x431,
				211 , 507,  // origine
				30,14,  // size
				0,  //reserved
				&Sub_PulsanteObjectBlu, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteObjectVerde = {
				&DatiPulsanteObjectBlu ,
				0x431,
				211 , 489,  // origine
				30,14,  // size
				0,  //reserved
				&Sub_PulsanteObjectVerde, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteObjectRosso = {
				&DatiPulsanteObjectVerde ,
				0x431,
				211 , 471,  // origine
				30,14,  // size
				0,  //reserved
				&Sub_PulsanteObjectRosso, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteAmbienteBlu = {
				&DatiPulsanteObjectRosso ,
				0x431,
				18 , 699,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteAmbienteBlu, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteAmbienteVerde = {
				&DatiPulsanteAmbienteBlu,
				0x431,
				18 , 681,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteAmbienteVerde, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteAmbienteRosso = {
				&DatiPulsanteAmbienteVerde ,
				0x431,
				18 , 663,  // origine
				22,14,  // size
				0,  //reserved
				&Sub_PulsanteAmbienteRosso, 
				0x01, 
				0x01};

StrDatiPulsante DatiPulsanteSearch = {
				&DatiPulsanteAmbienteRosso ,
				0x31,
				179 , 590,  // origine
				103,16,  // size
				0,  //reserved
				&Sub_PulsanteSearch, 
				0x00, 
				0x01};

StrDatiPulsante DatiPulsanteGoBack = {
				&DatiPulsanteSearch,
				0x31,
				701 , 622,  // origine
				0x66,0x10,  // size
				0,  //reserved
				&Sub_PulsanteGoBack, 
				0x00, 
				0x01};

StrDatiPulsante DatiPulsanteRotate = {
				&DatiPulsanteGoBack,
				0x31,
				599 , 622,  // origine
				0x66,0x10,  // size
				0,  //reserved
				&Sub_PulsanteRotate, 
				0x00, 
				0x01};

StrDatiPulsante DatiPulsanteBigTexture = { &DatiPulsanteRotate, 
				0x31, // flag per pulsante room
				497,622, // Origine
				0x66,0x10, // Size
				0, // Reserved
				&Sub_PulsanteBigTexture, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)

StrDatiPulsante DatiPulsanteDamage = { &DatiPulsanteBigTexture, 
				0x431, // flag per pulsante room
				146,310, // Origine
				24,16, // Size
				0, // Reserved
				&Sub_PulsanteDamage, 
				0x00, // colore testo
				0x01};  // colore fondo (grigio standard)

StrDatiPulsante DatiPulsanteBackup = { &DatiPulsanteDamage , // prossimo record
				0x31, // flags
				0x18b,0x026E, // origine
				0x66, 0x10, // dimensione
				0, //reserved
				&Sub_PulsanteBackup, // funzione di gestione pulsante
				0x0,    // coloretesto  (nero)
				0x0a};  // colore fondo (giallo)


void MioMessageBox(char *pMessaggio, char *pTitolo)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
					
	MessageBox(*pMiaWind,pMessaggio,pTitolo,MB_APPLMODAL);

}

void RefreshSchermoRoom(void)
{
	__asm {



		pushad
		mov ecx, 563F58h    ;BaseStrutturaInfo
		mov  ecx, dword ptr [ecx]
		push ecx
		mov eax, 444BF0h    ;RefreshTutto
		call eax
		add	esp,4h

		popad
	}


}
// nome: CaricaFile
// input: NomeFile
// scopo: Carica interamente in memoria file di testo NomeFile
// scopo: e restituisce puntatore alla memoria alloca
// output: Se errore restituisce NULL (e ha gia' visualizzato
// output: messaggio d'errore
// note: La memoria deve essere liberata da funzione chiamante

char * CaricaFile(char NomeFile[])
{
	FILE *TempFile;
	char BufferTemp[256];
	char *BaseMem;
	long Size;



	TempFile=fopen(NomeFile,"rb");
	if (TempFile ==NULL) {

		return NULL;
		}
	Size=QuantoLungo(TempFile);
	BaseMem = (char *) malloc(Size+1);
	if (BaseMem == NULL) {
		sprintf(BufferTemp,"Not enough memory to load file \"%s\"",
			NomeFile);

		fclose(TempFile);
		return NULL;
		}
	fread(BaseMem,Size,1,TempFile);
	fclose(TempFile);
	BaseMem[Size]=0;
	return BaseMem;

}
// resituisce dimensione di pNomeFile 
// nota: funziona come QuantoLungo() ma in questo caso il file NON deve essere gia' aperto

long DimensioneFile(char *pNomeFile)
{
	FILE *pFile;
	long Size;

	pFile = fopen(pNomeFile, "rb");
	if (pFile==NULL) return 0;
	Size = QuantoLungo(pFile);
	fclose(pFile);
	return Size;

}
// sostituisce la versione per tomb4.
// questa non memorizza dati relativi a patch ma solo scrive nella memoria dell'eseguibile la patch

void ApplyCodePatch(DWORD StartAdr, BYTE VetBytes[], int TotBytes)
{
	BYTE *pMem;
	int i;

	pMem= (BYTE *) StartAdr;

	for (i=0;i<TotBytes;i++) {
		pMem[i] = VetBytes[i];
	}


}

// legge chiave di registro (per scoprire percorso file)
char * LeggiValoreKey(char NomeChiave[])
{
	int RetVal;
	HKEY NuovoHandle;
	char BufferNome[512];
	static BYTE BufDati[512];
	DWORD SizeBufDati;
	DWORD SizeBufferNome;

	RetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, NomeChiave, 0, KEY_READ, &NuovoHandle);
	if (RetVal != ERROR_SUCCESS) return NULL;
	
	SizeBufDati=511;
	SizeBufferNome=511;
	RetVal = RegEnumValue(NuovoHandle, 0, BufferNome, &SizeBufferNome, NULL, NULL, BufDati, &SizeBufDati);
	if (RetVal != ERROR_SUCCESS) return NULL;


	return (char *) BufDati;

}


// visualizza messaggio nel room editor nella zona messaggi
void ShowInfoRoom(char *pBuffer)
{
	DisegnaRettangolo(296,699, 0x1fc, 10, 1);
	ShowTestoSuSchermo(296,699, pBuffer);
	TestMessaggio=true;
	LastTimeMessaggio = GetTickCount();
	RefreshSchermoRoom();
}

// restituisce  (un puntatore  a variabile statica interna) con
// il nuovo nome file dove e' stata attacata la nuova estgensione
// newext
// nota: NewExt deve esswere nel formato ".tr4"

char* CambiaEstensione(char NomeFile[], char NewExt[])
{
	int i;
	static char NomeCambiato[256];
	
	strcpy(NomeCambiato,NomeFile);

	
	for (i=strlen(NomeCambiato)-1;i>=0;i--) {
		if (NomeCambiato[i] == '.') {
			NomeCambiato[i] =0;
			strcat(NomeCambiato,NewExt);
			break;
		}
	}

	return NomeCambiato;

}

// riceve messaggio per titolo fienstra standard.
// se pero' siamo in modalita TestShowTitleInfo = true 
// (mostra info room nel titolo)
// non cambia la barra del titolo ma lo copia in LastTitleWindow
void CambiaTitoloNgle(char *pMex)
{
	HWND *pMiaWind = (HWND*) 0x46E559;

	strcpy(LastTitleWindow, pMex);
	if (TestShowTitleInfo == false) {
		SetWindowText(*pMiaWind, pMex);
	}

}
// copia contenuto di pRecordSrc in pRecordDest
void CopiaRect(RECT *pRecordDest, RECT *pRecordSrc)
{
	memcpy(pRecordDest,pRecordSrc,sizeof(RECT));

}
char *TrovaDirectoryCorrente(void)
{
	static char MiaDirectory[256];

	GetCurrentDirectory(255,MiaDirectory);
	return MiaDirectory;
}

char *GetFileTrle(char *pNomeFile)
{
	static char NomeTrle[256];

	sprintf(NomeTrle,"%s\\%s",Dir_Trle,pNomeFile);
	return NomeTrle;

}

char *GetFileTemp(char * pNomeFile)
{
	static char NomeFolder[256];

	sprintf(NomeFolder,"%s\\%s",MyTempFolder,pNomeFile);
	return NomeFolder;
}

char *GetFileCrash(char * pNomeFile)
{
	static char NomeFolder[256];

	sprintf(NomeFolder,"%s\\%s",MyCrashFolder,pNomeFile);
	return NomeFolder;
}
// nome: Search
// input: char *pInizio, char Stringa[], int TipoSearch (ts_Avanti/ts_Indietro),
// input: char *pFine (indirizzo per fine ricerca)
// scopo: Cerca, a partire da pInizio la sequenza di caratteri in Stringa.
// scopo: Se esito negativo restituisce NULL
// output: char * (indirizzo immediatamente seguente a stringa trovata) o NULL
// note: I dati puntati da pInizio devono terminare con uno zero
// note: Se in ricerca avanti pFine == NULL, allora si cerhera da pInizio 
// note: fino alla fine della stringa (ossia fino allo zero finale)

char *Search(char *pInizio, char Stringa[],int TipoSearch,char *pFine)
{ 
	long	n,j,c1,c2;
	int  Trovato;
	bool TestFine;
	if (TipoSearch == ts_Avanti && pFine == NULL) {
		pFine = pInizio + strlen(pInizio);
		TestFine = false;
	}else {
		TestFine = true;
	}

	n=strlen(Stringa);
	Trovato=FALSE;
	while ((TipoSearch == ts_Avanti && ((*pInizio || TestFine==true) &&
             pInizio < pFine)) ||
            (TipoSearch == ts_Indietro && pInizio >= pFine)) {

		for (j=0;j<n;j++) {
			c1=tolower(pInizio[j]);
			c2=tolower(Stringa[j]);
			if (c1 != c2) break;
			}
		if (j==n) {
			Trovato=TRUE;
			break;
			}

		if (TipoSearch == ts_Avanti) pInizio++;
						    else pInizio--;
		}
	if (Trovato) return pInizio;

	return NULL;
}


// simula comportamento di InStr di visual basic, con la differenza
// che la ricerca parte da indice 0 e se non trova nulla restituisce -1 
// invece che 0
int InStr(DWORD Start, char *pTesto, char *pFind)
{
	DWORD i;
	DWORD n;
	DWORD j;
	DWORD TotCar;
	char BufSingle[2000];

	
	n = strlen(pFind);
	if (n > strlen(pTesto)) return -1;

	TotCar = strlen(pTesto)-n+1;

	for (i=Start;i<TotCar;i++) {
		for (j=0;j<n;j++) BufSingle[j]=pTesto[i+j];
		BufSingle[j]=0;

		if (strcmpi(BufSingle,pFind) == 0) return i;

	}
	return -1;
					
}
// funziona in modo simile al replace di visual basic
// solo che lavora sempre: dall'inizio, tutte le occorrenze, confronto testo
// nota: pTesto deve corrisponde ad una zona di memoria piu' ampia
//   di quella del testo contenuto perche' in seguito al replae
//   potrebbe aumentare il numero di caratteri da ospitare
char *Replace(char *pTesto, char *pFind, char *pReplace)
{
	int Indice;
	int Differenza;
	int NewSize;
	int i;
	int OldSize;
	int TotRiduci;
	int SizeReplace;

	// calcola differenza di lunghezza tra replace e find
	SizeReplace = strlen(pReplace);
	Differenza = SizeReplace - strlen(pFind);
	
	Indice = InStr(0,pTesto, pFind);

	while (Indice != -1) {
		OldSize = strlen(pTesto);
		// trovato un'occorrenza
		if (Differenza > 0) {
			// bisogna fare spazio allocando piu' memoria
			NewSize = OldSize + 1 + Differenza;

			// creare spazio del giusto numero di bytes
			for (i=OldSize;i>=Indice;i--) {
				pTesto[i+Differenza] = pTesto[i];
			}

			
		}

		if (Differenza < 0) {
			// non c'e' bisogno di nuova memoria
			// pero' devo restringere zona
			TotRiduci = -Differenza;

			for (i=Indice;i<=OldSize;i++) {
				pTesto[i] = pTesto[i+TotRiduci];
			}
		}

		for (i=0;i<SizeReplace;i++) {
			pTesto[Indice+i] = pReplace[i];
		}
		Indice += SizeReplace;
		Indice = InStr(Indice, pTesto, pFind);
	}
	return pTesto;
		

}

// elimina spazi e tab da stringa di input
char *SuperTrim(char *pStringa)
{
	int i;
	int Fine;
	int Inizio;
	static char BufStringa[4096];
	int j;

	
	// trovare primo carattere diverso da spazio
	for (i=0;i< (int) strlen(pStringa);i++) {
		if (pStringa[i] != ' ' && pStringa[i] != '\t') break;
	}

	Inizio=i;

	for (i=strlen(pStringa)-1;i>=0;i--) {
		if (pStringa[i] != ' ' && pStringa[i] != '\t') break;
	}

	Fine=i;
	if (Fine < Inizio) {
		BufStringa[0]=0;
		return BufStringa;
	}

	j=0;
	for (i=Inizio;i<=Fine;i++) {
		BufStringa[j++] = pStringa[i];
	}
	BufStringa[j]=0;

	return BufStringa;

}
// elimina spazi dai lati della stringa
char *Trim(char *pStringa)
{
	int i;
	int Fine;
	int Inizio;
	static char BufStringa[4096];
	int j;

	
	// trovare primo carattere diverso da spazio
	for (i=0;i< (int) strlen(pStringa);i++) {
		if (pStringa[i] != ' ') break;
	}

	Inizio=i;

	for (i=strlen(pStringa)-1;i>=0;i--) {
		if (pStringa[i] != ' ') break;
	}

	Fine=i;
	if (Fine < Inizio) {
		BufStringa[0]=0;
		return BufStringa;
	}

	j=0;
	for (i=Inizio;i<=Fine;i++) {
		BufStringa[j++] = pStringa[i];
	}
	BufStringa[j]=0;

	return BufStringa;

}
int GetIndiceProgettoNow(void)
{
	int i;
	// restituisce indice di progetto attuale
	// se indiceprogettoattuale = -1 crea un nuovo record e restituisce l'indice 
	// corrispondente
	if (MyPrefRoom.IndiceProjectNow == -1) {
		// creare nuovo record
		for (i=0;i<MyPrefRoom.TotOldProjects;i++) {
			if (MyPrefRoom.VetOldProjects[i].NomeProject[0] == 0) {
				// utilizzare questo
				break;
			}
		}

		if (i == MyPrefRoom.TotOldProjects) {

			i = MyPrefRoom.TotOldProjects;

			if (i >= MAX_PROGETTI){
				i--;
			}else
			{
				
				MyPrefRoom.TotOldProjects++;
			}
		}

		MyPrefRoom.IndiceProjectNow = i;
		InizializzaRecordOld(&MyPrefRoom.VetOldProjects[i]);
	}
	return MyPrefRoom.IndiceProjectNow;

}
void AggiornaSlotNomi(void)
{

	int i;
	int Slot;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	char *pOriginale;
	DWORD SizeNew;
	DWORD SizeOld;

	// ok, ora fare sostituzione
	pVetSlotOggetti = *p2VetSlotOggetti;
	
	for (i=0;i<VetMieiNomiSlot.TotNomiSlot;i++) {
		Slot=VetMieiNomiSlot.VetNomi[i].Slot;
		pOriginale=pVetSlotOggetti[Slot].pNome;

		SizeOld = strlen(pOriginale)+1;
		SizeNew = strlen(VetMieiNomiSlot.VetNomi[i].NomeSlot)+1;

		if  (SizeNew > SizeOld) {
			pOriginale = (char *) ReallocNGLE(pOriginale, SizeNew);
			pVetSlotOggetti[Slot].pNome = pOriginale;
		}

		strcpy(pOriginale, VetMieiNomiSlot.VetNomi[i].NomeSlot);
	}

}

void EseguiBackupNgle(void) 
{
	Sub_PulsanteBackup(NULL);
}
// usato solo per debug di comandi script in modalita' tomb4
// restituisce un testo descrittivo del trigger esportato pTrigger
char *GetTestoScriptTrigger(StrScriptTrigger *pTrigger)
{

	int i,j;
	int TipoCampoTrigger;  // SEZ_...
	int NumeroCampo;
	int Valore;
	static char Buffer[300];
	static char MiniBuf[80];
	int Indice;
	char *pMexTipo;
	static char * VetMexCmd[] = {"<undefined>", "GOTO", "EXIT", "TCMD_SET_TIMER", "TCMD_SET_FULL_TIMER", 
								"TCMD_SET_EXTRA_TIMER", "TCMD_SET_EXTRA_CONDITION", "TCMD_SET_OBJECT", 
								"TCMD_LOG", "TCMD_PAUSE", "TCMD_TIMER_FIELD"};
	char *pArg3;
	int z;

	StrNGConstants * pBaseCostanti;

	pBaseCostanti= &MainBaseCostanti;

	StrRecordSezione *pSezione;
	pMexTipo="UNKNOWN";

	if ((pTrigger->Flags & TGROUP_COMMAND) == TGROUP_COMMAND) {
		switch (pTrigger->Object) {
		case TCMD_EXIT:
			// e' TCMD_EXIT e llora il terzo valore e' vero o false
			if (pTrigger->Timer == 0) {
				pArg3="FALSE";
			}else {
				pArg3="TRUE";
			}
			break;
		default:
			sprintf(MiniBuf, "%d ($%04X)", pTrigger->Timer, pTrigger->Timer);
			pArg3=MiniBuf;
		break;
		}
		z= pTrigger->Object;
		if (z >= TCMD_MAX_ID) z=0;

		sprintf(Buffer,"COMMAND: %s %s", VetMexCmd[z], pArg3);
		return Buffer;			
	}

	if (pTrigger->Flags & TGROUP_FLIPEFFECT) {
		pMexTipo = "FLIPEFFECT";

		TipoCampoTrigger = SEZ_TRIGGERWHAT;
		NumeroCampo = 9;
		Valore =pTrigger->Object & 0x3ff;

		sprintf(MiniBuf,"<&> = %d  (E) = %d", pTrigger->Timer & 0xff,
			(pTrigger->Timer & 0x7f00) >> 8);
		
	}

	if (pTrigger->Flags & TGROUP_ACTION) {
		pMexTipo = "ACTION";
		TipoCampoTrigger = SEZ_TRIGGERWHAT;
		NumeroCampo = 11;
		Valore = pTrigger->Timer & 0xff;
		Indice = pTrigger->Object & 0x3ff;

		sprintf(MiniBuf, "NgleIndex=%d (E) = %d", 
			Indice, (pTrigger->Timer & 0x7f00) >> 8);

	}

	if (pTrigger->Flags & TGROUP_CONDITION_TRIGGER) {
		pMexTipo="CONDITION";
		TipoCampoTrigger = SEZ_TRIGGERTYPE;
		NumeroCampo = 12;
		Valore = pTrigger->Timer & 0xff;
		sprintf(MiniBuf, "<#> = %d (E) = %d", pTrigger->Object & 0x3ff, 
			(pTrigger->Timer & 0x7f00) >> 8);
	}



	if (pTrigger->PluginId > 0) {

		sprintf(Buffer, "%s trigger of plugin with ScriptID=%d  <%s>",
			pMexTipo, pTrigger->PluginId, MiniBuf);

		return Buffer;
	}

	for (i=0;i<pBaseCostanti->TotSezioni;i++) {
		pSezione = &pBaseCostanti->pVetSezioni[i];

		if (pSezione->TipoSezione  == TipoCampoTrigger &&
			pSezione->NumeroSezione == NumeroCampo) {

			// ok, ora cercare il numero specifico nella lista
			for (j=0;j < pSezione->TotValori;j++) {
				if (pSezione->pVetValori[j].Numero == Valore) {
					// trovato
					// ora calcolare anche gli argomenti

					sprintf(Buffer,"%s trigger: \"%s\" <%s>",  
						pMexTipo, pSezione->pVetValori[j].pDescrizione, MiniBuf);
					return Buffer;
				}
			}
		}
	}
	return MiniBuf;


}
// restituisce un vettore di puntatori stringhe alle parti di pStringa
// divise dal (singolo) carattere Separatore
// imposta in pTotPezzi il numero di elementi trovati
// nota: se pBufferHost e' diverso da NULL, verra' usato questo buffere
// per salvare le singole stringhe
// Stesso discorso per pVetPtr che se non e' null ospitera' un vettore
// di puntatori carattere
// se TestSoloPrimo = true, si ferma nel dividere al primo separatore
// ignorando quelli eventalmente successivi
// nota e' previsto che non possano esserci piu di 50 elementi finali
// l'intera pStringa non deve essere piu' lunga di 1023 caratteri
char ** Split(char *pStringa, char Separatore, 
			  int* pTotPezzi, char *pBufferHost, 
			  char **pVetPtr, bool TestSoloPrimo) 

{

	static char * pVettore[50];
	static char Buffer[1024];

	DWORD		i;
	int  IndiceOld;
	int		TotItem;
	char *pTemp;
	DWORD  MaxLen;
	char *pOut;
	char **pVetPointer;

	if (pBufferHost != NULL) 
		pOut = pBufferHost;
	else
		pOut = Buffer;

	if (pVetPtr != NULL) 
		pVetPointer= pVetPtr;
	else
		pVetPointer = pVettore;


	IndiceOld =0;
	strcpy(pOut,pStringa);
	TotItem=0;
	MaxLen = strlen(pOut);

	for (i =0;i<MaxLen;i++) {
		if (pOut[i] == Separatore) {
			// salvare il precedente
			pOut[i]=0;
			pVetPointer[TotItem++] = &pOut[IndiceOld];

			IndiceOld=i+1;
			if (TestSoloPrimo) break;
		}
	}
	
	// salvare ultimo indiceold a meno che non sia nullo
	pTemp = &pOut[IndiceOld];
	if (strlen(pTemp) > 0) {
	 	pVetPointer[TotItem++] = pTemp;
	}


	*pTotPezzi = TotItem;
	return  pVetPointer;

}




char * SoloDir(char FileName[])
{

	// restituisce solo la cartella del file
	int i;
	static char BufOut[256];

	for (i= strlen(FileName)-1  ; i >=0 ; i--) {
		if (FileName[i] == '\\') break;
	}
	strcpy(BufOut,FileName);

	if (i > 0 && FileName[i] == '\\') {
		BufOut[i]= 0;

	}
	return BufOut;
}
// cerca a partire da *pIndice e aggiorna il indice a posizione immediatamente
// seguente l'ultimo codice di ritorno a capo incontrato
char *PrendiLinea(char *pBufLinea, DWORD *pIndice)
{
	DWORD Indice;
	DWORD i,j;
	static char StrArgomenti[256];
	char Car;
	DWORD z;

	Indice = *pIndice;
	z=0;
	StrArgomenti[0]=0;
	for (j=Indice;j<strlen(pBufLinea);j++) {
		Car=pBufLinea[j];
		if (Car == '\n' || Car == '\r') break;
		StrArgomenti[z++] = Car;
		
	}
	StrArgomenti[z]=0;

	// superare tutti gli eventuali caratteri di fine linea
	for (i=j;i<strlen(pBufLinea);i++) {
		Car= pBufLinea[i];
		if (Car != '\n' && Car != '\r') break;
	}

	*pIndice =i;
	return StrArgomenti;

}
// funzione usata per leggere dati testuali di media manager
// cerca in testo il tag pNomeTag. se lo trova restituisce il puntatore
// agli argomenti del tag ed elimina l'intera accoppiata #NomeTag#=Argomenti 
// da pTesto
// se non lo trova restituisce NULL
char *EstraeDatiTag(char *pTesto, char *pNomeTag)
{
	DWORD i,j;
	static char BufArgomenti[512];
	char *pChar;
	DWORD n,z;
	DWORD Indice;



	i=InStr(0,pTesto,pNomeTag);
	if (i == -1) return NULL;
	j = i + strlen(pNomeTag);
	
	pChar = PrendiLinea(pTesto,&j);
	Indice =j;



	if (pChar == NULL) return NULL;
	if (pChar[0] ==0) {
		BufArgomenti[0]='~';
		BufArgomenti[1]=0;
	}else {
		strcpy(BufArgomenti, pChar);
	}
	// ora togliere tutto il blocco di testo di questo tag.
	n=strlen(pTesto);
	for (z=i;z< n-strlen(BufArgomenti);z++) {
		pTesto[z] = pTesto[z+strlen(BufArgomenti)];
	}
	pTesto[z]=0;

	return BufArgomenti;

}

// estrae dal testo complessivo Testo la sezione NomeParte
// Per esempio se NomeParte = "TEXT" estrarra il testo contenuto
// in <TEXT> <END_TEXT>
// ATTENZIONE: il testo restituito da questa funzione e' stato allocato
// dinamicamente con malloc() per cui andrebbe rilasciato quando non
// viene piu' utilizzato
// nota: Se TestRimuovi = true la porzione estratta viene eliminata 
// dal testo sorgente (*Testo), 
// se invece e' false viene lasciata
char *EstraeParteTesto(char *Testo, char *NomeParte, bool TestRimuovi)
{
	DWORD j;
	int Indice;
	char StartSection[80];
	char EndSection[80];
	DWORD z;
	char *pResult;
	DWORD SizeTesto;
	DWORD Inizio,Fine;

	sprintf(StartSection, "<%s>", NomeParte);
	Indice = InStr(0,Testo,StartSection);

	if (Indice < 0) return NULL;
	j = Indice;
	Inizio = j;

	j += strlen(StartSection);
	// trovare parte finale
	sprintf(EndSection,"<END_%s>", NomeParte);
	z = InStr(j,Testo,EndSection);
	if (z < 0) {
		return NULL;
	}
	Fine= z + strlen(EndSection);

	SizeTesto= z-j;

	pResult = (char*) malloc(SizeTesto+2);
	memcpy(pResult, &Testo[j],SizeTesto);
	pResult[SizeTesto] =0;

	if (TestRimuovi==true) {
		if (Testo[Fine] == 0x0a) Fine++;

		SizeTesto = Fine-Inizio;
		strcpy(&Testo[Inizio], &Testo[Fine]);

	}

	return pResult;

	
}
// verifica che testo in StrNumero sia corretto in decimale o esadecimale
// Se lo e' lo converte e restituisce il valore, se non lo e' restituisc
// false e imposta TestErrore = true
int GetArgNumerico(char *StrNumero, bool *TestErrore)
{
	char *pChar;
	DWORD Start;
	int Valore;



	pChar = Trim(StrNumero);
	
	Valore= -153920;
	Start=0;
	if (*pChar == '$') {

		pChar++;
		sscanf(pChar, "%x",&Valore);

	}else {
		sscanf(pChar, "%d",&Valore);
	}

	if (Valore == -153920) {
		*TestErrore=true;
		Valore=0;
	}else {
		*TestErrore=false;
	}

	return Valore;

}
// supponendo che il campo che stiamo cercando ha un solo argomento numerico
// come se fosse:
// "#CODICE#=143
// restituisce il numero trovato gia' tradotto oppure imposta il valore 0
// se e' assente.
// se c'e' un errore di sintassi imposta TestErrore = true

int EstraeNumericField(char *Testo, char *NomeField, bool *TestErrore)
{
	char *StrArgomento;

	*TestErrore = false;

	StrArgomento = EstraeDatiTag(Testo,NomeField);
	if (StrArgomento==NULL) return 0;
	return GetArgNumerico(StrArgomento, TestErrore);

}

char * SoloNome(char FileName[])
{

	// restituisce solo il nome del file
	int i;

	for (i= strlen(FileName)-1  ; i >=0 ; i--) {
		if (FileName[i] == '\\') break;
	}
	if (i > 0 && FileName[i] == '\\') {
		return & FileName[i+1];
	}
	return FileName;
}
// restituisce solo nome del file, senza cartella e senza estensione
char *SoloNomeSenzaExt(char FileName[])
{
	static char BufferTemp[256];
	int		i;

	strcpy(BufferTemp,SoloNome(FileName));
	for (i = strlen(BufferTemp)-1;i>=0;i--) {
		if (BufferTemp[i] == '.') {
			BufferTemp[i]=0;
			break;
		}
	}

	return BufferTemp;


}
// restituisce solo estensione del file senza "."
// esempio "tom"  "tr4"
char *SoloEstensione(char *pNomeFile)
{
	static char MexEstensione[80];
	int i;

	MexEstensione[0]=0;
	for (i=strlen(pNomeFile)-1;i>=0;i--) {
		if (pNomeFile[i] == '.') {
			strcpy(MexEstensione, &pNomeFile[i+1]);
			break;
		}
	}
	return MexEstensione;

}

// restituisce nome corto di file NomeLungo
char *TrovaNomeCorto(char *NomeLungo)
{

	static char NomeCorto[256];

	if (GetShortPathName(NomeLungo,NomeCorto,255)==0) 
		return NomeLungo;
	else
		return NomeCorto;

}

char *TrovaNomeLungo(char *pNomeCorto)
{
	static char NomeLungo[256];
	char *pParteNome;


	if (GetFullPathName(pNomeCorto,255,NomeLungo,&pParteNome)==0)
		return pNomeCorto;
	else
		return NomeLungo;

}
// restituisce indice di room IndiceRoom, oppure se IndiceRoom = -1
// lavora su indice di stanza attuale, tenendo conto se stanza attuale
// e' collegata ad una stanza flippata e se il pulsante flipmap e' premuto
// in pratica in alcuni casi restituira' l'indice diretto della stanza flippata

int GetIndiceRoom(int IndiceRoom)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int *pTestFlipMap = (int *) 0x583BF0;

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;
	if (IndiceRoom == -1) 
			IndiceRoom = *pIndiceRoomAttuale;

	if (pVetRooms[IndiceRoom].IndexFlipRoom != -1 && (*pTestFlipMap) !=0) 
		IndiceRoom= pVetRooms[IndiceRoom].IndexFlipRoom;

	return IndiceRoom;
}
char * TrovaCartellaNgCenter(void)
{
	char *pChar;
	DWORD i;

	pChar = LeggiValoreKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NG_Center.exe");
	if (pChar == NULL) return NULL;

	// se c'e' NG_Center.exe prendere solo cartella
	if (InStr(0, pChar, "NG_Center.exe") != -1) {

		pChar = SoloDir(pChar);
	}else {
		// e' gia' cartella.
		// limitarsi a togliere l'eventuale "\" finale
		i = strlen(pChar);
		if (pChar[i-1] == '\\') i--;
		pChar[i]  =0;
	}

	return pChar;

}
void LiberaMiaMemoriaGlobale(void)
{
	if (BaseMiaMemory.SizeTotale == 0) return;
	free(BaseMiaMemory.pBaseMem);
	BaseMiaMemory.pBaseMem=0;
	BaseMiaMemory.SizeTotale =0;

}
// miamemory basata su blocco unico (singole zone concesse non sono rdimensionabili ne' rilaciabili in modo autonomo)
void AllocaMiaMemoriaGlobale(int SizeTotale)
{
	// se era allocato un blocco precedente liberarlo
	if (BaseMiaMemory.SizeTotale) LiberaMiaMemoriaGlobale();

	BaseMiaMemory.pBaseMem = (BYTE *) malloc(SizeTotale+1000);
	BaseMiaMemory.SizeTotale = SizeTotale;
	BaseMiaMemory.pPointer = BaseMiaMemory.pBaseMem;
	BaseMiaMemory.pEndMemory = &BaseMiaMemory.pBaseMem[SizeTotale];

}
// restituisce memoria libera prelevata da blocco unico allocato con AllocaMiaMemoriaGlobale
// se la memoria e' finita restituisce NULL
void *GetMemory(int Size)
{
	BYTE *pOltre;
	void *pMem;

	pOltre = &BaseMiaMemory.pPointer[Size];
	if (pOltre >= BaseMiaMemory.pEndMemory) {
		InviaLog("ERROR: Out of memory in GetMemory() function");
		// out of memory
		return NULL;
	}

	pMem = BaseMiaMemory.pPointer;
	BaseMiaMemory.pPointer += Size;

	return pMem;

}
// valori globali in BaseTr4Zl  con file gia' aperto in lettura e nella posizione giusta
// carica in pGeo tutta la geometria da file *pFile (non compresso)
bool CaricaGeometria(Cab_GeometryTr4 *pGeo){

	FILE *pFile;
	Cab_RoomTr4 *pRoom;
	int i;
	int Size;
	Cab_AnimationData *pDataA;

	pFile = BaseTr4Zl.pFile;

	//ok, ora caricare da pFile (qualunque esso sia)
	fread(&pGeo->Mistery, 4,1, pFile);
	
    fread(&pGeo->N_Rooms, 2,1, pFile);

	if (pGeo->N_Rooms > 0) {
		pGeo->pVetRooms = (struct Cab_RoomTr4 *) GetMemory(sizeof(struct Cab_RoomTr4) * pGeo->N_Rooms);
	}

    for (i=0;i<pGeo->N_Rooms;i++) {
		pRoom = &pGeo->pVetRooms[i];

        // --------------------------- CARICA ROOM ----------------------------
		fread(&pRoom->OrgZ, 4,1, pFile);
		fread(&pRoom->OrgX, 4,1, pFile);
		fread(&pRoom->OrgYBottom, 4,1, pFile);
		fread(&pRoom->OrgYTop, 4,1, pFile);

		fread(&pRoom->StaticDataWords, 4, 1, pFile);
		if (pRoom->StaticDataWords > 0) {
			pRoom->pStaticData = (short *) GetMemory(pRoom->StaticDataWords * 2);

			fread(pRoom->pStaticData, 2, pRoom->StaticDataWords,pFile);
		}

		fread(&pRoom->NDoors, 2,1,pFile);
		if (pRoom->NDoors) {
			pRoom->pVetDoors = (Cab_DoorTr4 *) GetMemory(sizeof(struct Cab_DoorTr4) * pRoom->NDoors);
			fread(pRoom->pVetDoors, sizeof(struct Cab_DoorTr4), pRoom->NDoors,pFile);
		}
		fread(&pRoom->XSizeSectors, 2,1, pFile);
		fread(&pRoom->ZSizeSectors,2,1, pFile);

		// matrice 
		Size = sizeof(struct Cab_CollisionSectorTR4) * pRoom->XSizeSectors * pRoom->ZSizeSectors;
		pRoom->pCollisionSectorGrid = (Cab_CollisionSectorTR4 *) GetMemory(Size);

		fread(pRoom->pCollisionSectorGrid, Size, 1, pFile);

		fread(&pRoom->LightIntensity, 4,1, pFile);
		
		fread(&pRoom->NLights,2,1,pFile);
		if (pRoom->NLights) {
			Size= sizeof(struct Cab_LightTr4) * pRoom->NLights;
			pRoom->pVetLights = (Cab_LightTr4 *) GetMemory(Size);
			fread(pRoom->pVetLights, Size,1,pFile);
		}

		fread(&pRoom->NStaticMesh, 2,1, pFile);
		if (pRoom->NStaticMesh) {
			Size = sizeof(struct Cab_StaticMeshTr4) * pRoom->NStaticMesh;
			pRoom->pVetStaticMesh = (Cab_StaticMeshTr4*) GetMemory(Size);
			fread(pRoom->pVetStaticMesh, Size,1,pFile);

		}
		
		fread(&pRoom->AlternateRoom, 2,1,pFile);
		fread(&pRoom->RoomFlags, 2,1,pFile);
		fread(&pRoom->WaterWaveIntensity,1,1,pFile);
		fread(&pRoom->Mistery,2,1,pFile);



	}

	// floor data  (qui c'e' il problema che potrebbe essere necessario ridimensionarla
	// mannaggia e non sono neanche sicuro
	// ma credo di si
	// comunque: magari nel mix finale posso allocarla a parte
	// intanto in lettura le lascio fisse

	fread(&pGeo->FloorData.TotFloorData, 4,1,pFile);

	if (pGeo->FloorData.TotFloorData) {
		pGeo->FloorData.pVetFloorData = (WORD *) GetMemory(pGeo->FloorData.TotFloorData * 2);
		fread(pGeo->FloorData.pVetFloorData, 2, pGeo->FloorData.TotFloorData,pFile);
	}
    //---------------------------------------- OBJECT MESH
	fread(&pGeo->ObjectMeshNWords, 4,1,pFile);
	if (pGeo->ObjectMeshNWords) {
		pGeo->pObjectMesh = (short*) GetMemory(pGeo->ObjectMeshNWords * 2);
		fread(pGeo->pObjectMesh, 2, pGeo->ObjectMeshNWords,pFile);
	}

	fread(&pGeo->N_MeshPointer, 4,1,pFile);
	if (pGeo->N_MeshPointer) {
		pGeo->pVetMeshPointer = (int *) GetMemory(pGeo->N_MeshPointer *4);
		fread(pGeo->pVetMeshPointer, 4, pGeo->N_MeshPointer,pFile);
	}

	// -------------------------------------- ANIMATION DATA
	pDataA = & pGeo->AnimationData;

	fread(&pDataA->NAnimation,4,1,pFile);
	
	if (pDataA->NAnimation) {
		Size = sizeof(struct Cab_AnimationTr4) * pDataA->NAnimation;

		pDataA->pVetAnimation = (Cab_AnimationTr4*) GetMemory(Size);
		fread(pDataA->pVetAnimation, Size,1,pFile);
	}

	fread(&pDataA->N_StateChanges, 4,1, pFile);
	if (pDataA->N_StateChanges) {
		Size = sizeof(struct Cab_StateChanges) * pDataA->N_StateChanges;
		pDataA->pVetStateChanges = (Cab_StateChanges*) GetMemory(Size);
		fread(pDataA->pVetStateChanges, Size,1,pFile);
	}

	fread(&pDataA->NAnimDispatch , 4,1, pFile);
	if (pDataA->NAnimDispatch ) {
		Size = sizeof(struct Cab_AnimDispatchTr4) * pDataA->NAnimDispatch;
		pDataA->pVetAnimDispatch  = (Cab_AnimDispatchTr4*) GetMemory(Size);
		fread(pDataA->pVetAnimDispatch, Size,1,pFile);
	}


	fread(&pDataA->NAnimCommand, 4,1, pFile);
	if (pDataA->NAnimCommand) {
		pDataA->pVetAnimCommand = (short *) GetMemory(pDataA->NAnimCommand * 2);
		fread(pDataA->pVetAnimCommand, 2, pDataA->NAnimCommand,pFile);
	}

	fread(&pDataA->NMeshTree, 4,1,pFile);
	if (pDataA->NMeshTree) {
		pDataA->pVetMeshTree = (int *) GetMemory(pDataA->NMeshTree * 4);
		fread(pDataA->pVetMeshTree,4, pDataA->NMeshTree,pFile);
	}
	
	fread(&pDataA->NFrames,4,1,pFile);
	if (pDataA->NFrames) {
		pDataA->pVetFrames = (short *) GetMemory(pDataA->NFrames * 2);
		fread(pDataA->pVetFrames, 2, pDataA->NFrames, pFile);
	}

    //-------------------------------------- SLOT MOVEABLES

	fread(&pGeo->NSlotMoveables, 4, 1, pFile);
	if (pGeo->NSlotMoveables) {
		Size= sizeof(struct Cab_SlotT4) * pGeo->NSlotMoveables;
		pGeo->pVetSlotMoveables = (Cab_SlotT4*) GetMemory(Size);
		fread(pGeo->pVetSlotMoveables, Size,1,pFile);
	}

    //-------------------------------------- STATIC OBJECTS
	fread(&pGeo->NStaticObjects, 4,1, pFile);
	if (pGeo->NStaticObjects) {
		Size = sizeof(struct Cab_StaticSlotTr4) * pGeo->NStaticObjects;
		pGeo->pVetStaticObjects = (Cab_StaticSlotTr4*) GetMemory(Size);
		fread(pGeo->pVetStaticObjects, Size,1,pFile);
	}
    //---------------------------------------- SPRITE
	fread(pGeo->MexSpiteTex, 3,1,pFile);
	pGeo->MexSpiteTex[3]=0;
	if (strcmp(pGeo->MexSpiteTex, "SPR")!=0) {
		InviaLog("ERROR: failed Sprite check [SPR] loading uncompressed geometry");
		fclose(pFile);
		return false;
	}

	fread(&pGeo->NSpriteTex, 4,1,pFile);
	if (pGeo->NSpriteTex) {
		Size = sizeof(struct Cab_SpriteTexTr4) * pGeo->NSpriteTex;
		pGeo->pVetSpriteTex = (Cab_SpriteTexTr4*) GetMemory(Size);
		fread(pGeo->pVetSpriteTex, Size,1,pFile);
	}

	fread(&pGeo->NSpriteSeq, 4,1,pFile);
	if (pGeo->NSpriteSeq) {
		Size=sizeof(struct Cab_SpriteSeq) * pGeo->NSpriteSeq;
		pGeo->pVetSpriteSeq = (Cab_SpriteSeq*) GetMemory(Size);
		fread(pGeo->pVetSpriteSeq, Size,1,pFile);
	}

    //--------------------------------------- CAMERAS

	fread(&pGeo->NCameras, 4,1,pFile);
	if (pGeo->NCameras) {
		Size= sizeof(struct Cab_CameraTr4) * pGeo->NCameras;
		pGeo->pVetCameras = (Cab_CameraTr4 *) GetMemory(Size);
		fread(pGeo->pVetCameras, Size,1,pFile);
	}
    //--------------------------------------- FLYBY

	fread(&pGeo->NFlyBy, 4,1, pFile);
	if (pGeo->NFlyBy) {
		Size=sizeof(struct Cab_FlyCameraTr4) * pGeo->NFlyBy;
		pGeo->pVetFlyBy = (Cab_FlyCameraTr4*) GetMemory(Size);
		fread(pGeo->pVetFlyBy, Size,1,pFile);

	}
    //-------------------------------------- LOCAL SOUNDS
	fread(&pGeo->NLocalSounds, 4,1,pFile);
	if (pGeo->NLocalSounds) {
		Size= sizeof(struct Cab_LocalSound) * pGeo->NLocalSounds;
		pGeo->pVetLocalSounds = (Cab_LocalSound*) GetMemory(Size);
		fread(pGeo->pVetLocalSounds, Size,1,pFile);
	}
    //--------------------------------------- BOXES
	fread(&pGeo->NBox, 4,1,pFile);
	if (pGeo->NBox) {
		Size= sizeof(struct Cab_BoxTr4) * pGeo->NBox;
		pGeo->pVetBox = (Cab_BoxTr4*) GetMemory(Size);
		fread(pGeo->pVetBox, Size,1,pFile);

	}
    //---------------------------------------- OVERLAPS
	fread(&pGeo->NOverlaps, 4,1,pFile);
	if (pGeo->NOverlaps) {
		pGeo->pVetOverlaps = (short *) GetMemory(pGeo->NOverlaps * 2);
		fread(pGeo->pVetOverlaps, 2, pGeo->NOverlaps,pFile);
	}

    //----------------------------------------- ZONES
    //matrice (10 x NBox)
	Size = 10 * pGeo->NBox * 2;
	pGeo->pVetZones = (short *) GetMemory(Size);
	fread(pGeo->pVetZones, Size,1,pFile);
    //----------------------------------------- DATA ANIM
	fread(&pGeo->DataAnimWords, 4,1,pFile);
	if (pGeo->DataAnimWords) {
		pGeo->pVetAnimWords = (short *) GetMemory(pGeo->DataAnimWords * 2);
		fread(pGeo->pVetAnimWords, 2, pGeo->DataAnimWords,pFile);
	}

	fread(&pGeo->NUvRange,1,1,pFile);


    //------------------------------------------ OBJ CHECK
	fread(pGeo->ObjTextCheck,3,1,pFile);
	pGeo->ObjTextCheck[3]=0;
	if (strcmp(pGeo->ObjTextCheck, "TEX")!=0) {
		fclose(pFile);
		InviaLog("ERROR: failed Object Texture check [TEX], loading uncompressed geometry");
		return false;
	}
    //-------------------------------------------- TEX INFOS
	fread(&pGeo->NTexInfo,4,1,pFile);
	if (pGeo->NTexInfo) {
		Size= sizeof(struct Cab_TexInfoTr4) * pGeo->NTexInfo;
		pGeo->pVetTexInfo = (Cab_TexInfoTr4*) GetMemory(Size);
		fread(pGeo->pVetTexInfo, Size,1,pFile);
	}
    //-------------------------------------------- ITEMS
	fread(&pGeo->NItems,4,1,pFile);
	if (pGeo->NItems) {
		Size = sizeof(struct Cab_ItemTr4) * pGeo->NItems;
		pGeo->pVetItems = (Cab_ItemTr4*) GetMemory(Size);
		fread(pGeo->pVetItems, Size,1,pFile);

	}
    //--------------------------------------------- AI DATA
	fread(&pGeo->N_AiData, 4,1,pFile);
	if (pGeo->N_AiData) {
		Size = sizeof(struct Cab_AiDataTr4) * pGeo->N_AiData;
		pGeo->pVetAiData = (Cab_AiDataTr4*) GetMemory(Size);
		fread(pGeo->pVetAiData, Size,1,pFile);
		
	}
    //--------------------------------------------- DEMO DATA E SOUND TABLE
	fread(&pGeo->SizeDemoData, 2,1,pFile);
	if (pGeo->SizeDemoData == 2048) {
		Size = 2048*2;
	}else {
		Size = 370 *2;
	}
	pGeo->pSoundTables = (short*) GetMemory(Size);
	fread(pGeo->pSoundTables, Size,1,pFile);
    //---------------------------------------------- SOUND INFOS
	fread(&pGeo->NSoundInfos, 4,1,pFile);
	if (pGeo->NSoundInfos) {
		Size = sizeof(struct Cab_SoundInfosTr4) * pGeo->NSoundInfos;
		pGeo->pVetSoundInfos = (Cab_SoundInfosTr4*) GetMemory(Size);
		fread(pGeo->pVetSoundInfos, Size,1,pFile);		
	}
    //---------------------------------------------- SOUND INDICES
	fread(&pGeo->NSoundIndices, 4,1,pFile);
	if (pGeo->NSoundIndices) {
		pGeo->pVetSoundIndices = (int *) GetMemory(pGeo->NSoundIndices * 4);
		fread(pGeo->pVetSoundIndices, 4, pGeo->NSoundIndices, pFile);
	}

	fread(pGeo->FillerArray, 6,1,pFile);

	return true;
}
// valori globali in BaseTr4Zl  con file gia' aperto in scrittura e nella posizione giusta
// salva la geometria in pGeo nel file *pFile (in modo non compresso)
bool SalvaGeometria(Cab_GeometryTr4 *pGeo){

	FILE *pFile;
	Cab_RoomTr4 *pRoom;
	int i;
	int Size;
	Cab_AnimationData *pDataA;

	pFile = BaseTr4Zl.pFile;

	//ok, ora salvare da pFile (qualunque esso sia)
	fwrite(&pGeo->Mistery, 4,1, pFile);
	
    fwrite(&pGeo->N_Rooms, 2,1, pFile);

    for (i=0;i<pGeo->N_Rooms;i++) {
		pRoom = &pGeo->pVetRooms[i];

        // --------------------------- salva ROOM ----------------------------
		fwrite(&pRoom->OrgZ, 4,1, pFile);
		fwrite(&pRoom->OrgX, 4,1, pFile);
		fwrite(&pRoom->OrgYBottom, 4,1, pFile);
		fwrite(&pRoom->OrgYTop, 4,1, pFile);

		fwrite(&pRoom->StaticDataWords, 4, 1, pFile);
		if (pRoom->StaticDataWords > 0) {

			fwrite(pRoom->pStaticData, 2, pRoom->StaticDataWords,pFile);
		}

		fwrite(&pRoom->NDoors, 2,1,pFile);
		if (pRoom->NDoors) {
			fwrite(pRoom->pVetDoors, sizeof(struct Cab_DoorTr4), pRoom->NDoors,pFile);
		}
		fwrite(&pRoom->XSizeSectors, 2,1, pFile);
		fwrite(&pRoom->ZSizeSectors,2,1, pFile);

		// matrice 
		Size = sizeof(struct Cab_CollisionSectorTR4) * pRoom->XSizeSectors * pRoom->ZSizeSectors;

		fwrite(pRoom->pCollisionSectorGrid, Size, 1, pFile);

		fwrite(&pRoom->LightIntensity, 4,1, pFile);
		
		fwrite(&pRoom->NLights,2,1,pFile);
		if (pRoom->NLights) {
			Size= sizeof(struct Cab_LightTr4) * pRoom->NLights;
			fwrite(pRoom->pVetLights, Size,1,pFile);
		}

		fwrite(&pRoom->NStaticMesh, 2,1, pFile);
		if (pRoom->NStaticMesh) {
			Size = sizeof(struct Cab_StaticMeshTr4) * pRoom->NStaticMesh;
			fwrite(pRoom->pVetStaticMesh, Size,1,pFile);
		}
		
		fwrite(&pRoom->AlternateRoom, 2,1,pFile);
		fwrite(&pRoom->RoomFlags, 2,1,pFile);
		fwrite(&pRoom->WaterWaveIntensity,1,1,pFile);
		fwrite(&pRoom->Mistery,2,1,pFile);



	}

	// --------------------------------------- floor data  

	fwrite(&pGeo->FloorData.TotFloorData, 4,1,pFile);

	if (pGeo->FloorData.TotFloorData) {
		fwrite(pGeo->FloorData.pVetFloorData, 2, pGeo->FloorData.TotFloorData,pFile);
	}
    //---------------------------------------- OBJECT MESH
	fwrite(&pGeo->ObjectMeshNWords, 4,1,pFile);
	if (pGeo->ObjectMeshNWords) {
		fwrite(pGeo->pObjectMesh, 2, pGeo->ObjectMeshNWords,pFile);
	}

	fwrite(&pGeo->N_MeshPointer, 4,1,pFile);
	if (pGeo->N_MeshPointer) {
		fwrite(pGeo->pVetMeshPointer, 4, pGeo->N_MeshPointer,pFile);
	}

	// -------------------------------------- ANIMATION DATA
	pDataA = & pGeo->AnimationData;

	fwrite(&pDataA->NAnimation,4,1,pFile);
	
	if (pDataA->NAnimation) {
		Size = sizeof(struct Cab_AnimationTr4) * pDataA->NAnimation;

		fwrite(pDataA->pVetAnimation, Size,1,pFile);
	}

	fwrite(&pDataA->N_StateChanges, 4,1, pFile);
	if (pDataA->N_StateChanges) {
		Size = sizeof(struct Cab_StateChanges) * pDataA->N_StateChanges;
		fwrite(pDataA->pVetStateChanges, Size,1,pFile);
	}

	fwrite(&pDataA->NAnimDispatch , 4,1, pFile);
	if (pDataA->NAnimDispatch ) {
		Size = sizeof(struct Cab_AnimDispatchTr4) * pDataA->NAnimDispatch;
		fwrite(pDataA->pVetAnimDispatch, Size,1,pFile);
	}


	fwrite(&pDataA->NAnimCommand, 4,1, pFile);
	if (pDataA->NAnimCommand) {
		fwrite(pDataA->pVetAnimCommand, 2, pDataA->NAnimCommand,pFile);
	}

	fwrite(&pDataA->NMeshTree, 4,1,pFile);
	if (pDataA->NMeshTree) {
		fwrite(pDataA->pVetMeshTree,4, pDataA->NMeshTree,pFile);
	}
	
	fwrite(&pDataA->NFrames,4,1,pFile);
	if (pDataA->NFrames) {
		fwrite(pDataA->pVetFrames, 2, pDataA->NFrames, pFile);
	}

    //-------------------------------------- SLOT MOVEABLES

	fwrite(&pGeo->NSlotMoveables, 4, 1, pFile);
	if (pGeo->NSlotMoveables) {
		Size= sizeof(struct Cab_SlotT4) * pGeo->NSlotMoveables;
		fwrite(pGeo->pVetSlotMoveables, Size,1,pFile);
	}

    //-------------------------------------- STATIC OBJECTS
	fwrite(&pGeo->NStaticObjects, 4,1, pFile);
	if (pGeo->NStaticObjects) {
		Size = sizeof(struct Cab_StaticSlotTr4) * pGeo->NStaticObjects;
		fwrite(pGeo->pVetStaticObjects, Size,1,pFile);
	}
    //---------------------------------------- SPRITE
	fwrite(pGeo->MexSpiteTex, 3,1,pFile);
	if (strcmp(pGeo->MexSpiteTex, "SPR")!=0) {
		InviaLog("ERROR: failed Sprite check [SPR] saving uncompressed geometry");
	}

	fwrite(&pGeo->NSpriteTex, 4,1,pFile);
	if (pGeo->NSpriteTex) {
		Size = sizeof(struct Cab_SpriteTexTr4) * pGeo->NSpriteTex;
		fwrite(pGeo->pVetSpriteTex, Size,1,pFile);
	}

	fwrite(&pGeo->NSpriteSeq, 4,1,pFile);
	if (pGeo->NSpriteSeq) {
		Size=sizeof(struct Cab_SpriteSeq) * pGeo->NSpriteSeq;
		fwrite(pGeo->pVetSpriteSeq, Size,1,pFile);
	}

    //--------------------------------------- CAMERAS

	fwrite(&pGeo->NCameras, 4,1,pFile);
	if (pGeo->NCameras) {
		Size= sizeof(struct Cab_CameraTr4) * pGeo->NCameras;
		fwrite(pGeo->pVetCameras, Size,1,pFile);
	}
    //--------------------------------------- FLYBY

	fwrite(&pGeo->NFlyBy, 4,1, pFile);
	if (pGeo->NFlyBy) {
		Size=sizeof(struct Cab_FlyCameraTr4) * pGeo->NFlyBy;
		fwrite(pGeo->pVetFlyBy, Size,1,pFile);

	}
    //-------------------------------------- LOCAL SOUNDS
	fwrite(&pGeo->NLocalSounds, 4,1,pFile);
	if (pGeo->NLocalSounds) {
		Size= sizeof(struct Cab_LocalSound) * pGeo->NLocalSounds;
		fwrite(pGeo->pVetLocalSounds, Size,1,pFile);
	}
    //--------------------------------------- BOXES
	fwrite(&pGeo->NBox, 4,1,pFile);
	if (pGeo->NBox) {
		Size= sizeof(struct Cab_BoxTr4) * pGeo->NBox;
		fwrite(pGeo->pVetBox, Size,1,pFile);

	}
    //---------------------------------------- OVERLAPS
	fwrite(&pGeo->NOverlaps, 4,1,pFile);
	if (pGeo->NOverlaps) {
		fwrite(pGeo->pVetOverlaps, 2, pGeo->NOverlaps,pFile);
	}

    //----------------------------------------- ZONES
    //matrice (10 x NBox)
	Size = 10 * pGeo->NBox * 2;
	fwrite(pGeo->pVetZones, Size,1,pFile);
    //----------------------------------------- DATA ANIM
	fwrite(&pGeo->DataAnimWords, 4,1,pFile);
	if (pGeo->DataAnimWords) {
		fwrite(pGeo->pVetAnimWords, 2, pGeo->DataAnimWords,pFile);
	}

	fwrite(&pGeo->NUvRange,1,1,pFile);


    //------------------------------------------ OBJ CHECK
	fwrite(pGeo->ObjTextCheck,3,1,pFile);
	if (strcmp(pGeo->ObjTextCheck, "TEX")!=0) {
		InviaLog("ERROR: failed Object Texture check, saving uncompressed geometry");
	}
    //-------------------------------------------- TEX INFOS
	fwrite(&pGeo->NTexInfo,4,1,pFile);
	if (pGeo->NTexInfo) {
		Size= sizeof(struct Cab_TexInfoTr4) * pGeo->NTexInfo;
		fwrite(pGeo->pVetTexInfo, Size,1,pFile);
	}
    //-------------------------------------------- ITEMS
	fwrite(&pGeo->NItems,4,1,pFile);
	if (pGeo->NItems) {
		Size = sizeof(struct Cab_ItemTr4) * pGeo->NItems;
		fwrite(pGeo->pVetItems, Size,1,pFile);

	}
    //--------------------------------------------- AI DATA
	fwrite(&pGeo->N_AiData, 4,1,pFile);
	if (pGeo->N_AiData) {
		Size = sizeof(struct Cab_AiDataTr4) * pGeo->N_AiData;
		fwrite(pGeo->pVetAiData, Size,1,pFile);
		
	}
    //--------------------------------------------- DEMO DATA E SOUND TABLE
	fwrite(&pGeo->SizeDemoData, 2,1,pFile);
	if (pGeo->SizeDemoData == 2048) {
		Size = 2048*2;
	}else {
		Size = 370 *2;
	}
	fwrite(pGeo->pSoundTables, Size,1,pFile);
    //---------------------------------------------- SOUND INFOS
	fwrite(&pGeo->NSoundInfos, 4,1,pFile);
	if (pGeo->NSoundInfos) {
		Size = sizeof(struct Cab_SoundInfosTr4) * pGeo->NSoundInfos;
		fwrite(pGeo->pVetSoundInfos, Size,1,pFile);		
	}
    //---------------------------------------------- SOUND INDICES
	fwrite(&pGeo->NSoundIndices, 4,1,pFile);
	if (pGeo->NSoundIndices) {
		fwrite(pGeo->pVetSoundIndices, 4, pGeo->NSoundIndices, pFile);
	}

	fwrite(pGeo->FillerArray, 6,1,pFile);

	return true;
}

bool CaricaTr4DaFile(Cab_BaseTr4 *pTr4)
{
	int i;
	FILE *pFile;
	Cab_SoundEffect *pSound;
	int Size;

	pFile = BaseTr4Zl.pFile;


	fread(&pTr4->MexTr4, 4,1,pFile);
	fread(&pTr4->NRoomTexPages,2,1,pFile);
	fread(&pTr4->NObjTexPages ,2,1,pFile);
	fread(&pTr4->NRoomBumPages ,2,1,pFile);
	// --------- texture 1
	fread(&pTr4->Tex1SizeUncompress,4,1,pFile);
	fread(&pTr4->Tex1SizeCompress,4,1,pFile);
	if (pTr4->Tex1SizeCompress) {
		pTr4->pTex1Bytes = (BYTE *) GetMemory(pTr4->Tex1SizeCompress);
		fread(pTr4->pTex1Bytes, pTr4->Tex1SizeCompress,1,pFile);
	}

	// --------- texture 2
	fread(&pTr4->Tex2SizeUncompress,4,1,pFile);
	fread(&pTr4->Tex2SizeCompress,4,1,pFile);
	if (pTr4->Tex2SizeCompress) {
		pTr4->pTex2Bytes = (BYTE *) GetMemory(pTr4->Tex2SizeCompress);
		fread(pTr4->pTex2Bytes, pTr4->Tex2SizeCompress,1,pFile);
	}

	// --------- texture 3
	fread(&pTr4->Tex3SizeUncompress,4,1,pFile);
	fread(&pTr4->Tex3SizeCompress,4,1,pFile);
	if (pTr4->Tex3SizeCompress) {
		pTr4->pTex3Bytes = (BYTE *) GetMemory(pTr4->Tex3SizeCompress);
		fread(pTr4->pTex3Bytes, pTr4->Tex3SizeCompress,1,pFile);
	}

	// ----------- geometry 
	fread(&pTr4->GeometrySizeUncompress, 4,1,pFile);
	fread(&pTr4->GeometrySizeCompress,4,1,pFile);
	
	BaseTr4Zl.SizeCompressed = pTr4->GeometrySizeCompress;
	BaseTr4Zl.SizeUncompressed = pTr4->GeometrySizeUncompress;

	if (CaricaGeometria(&pTr4->Geometry)==false) {

		InviaLog("ERROR: failed to load tr4 geometry");
		return false;
	}
	fread(&pTr4->NSoundEffect, 4,1,pFile);
	if (pTr4->NSoundEffect) {
		Size= sizeof(struct Cab_SoundEffect) *pTr4->NSoundEffect;
		pTr4->pVetSoundEffects = (Cab_SoundEffect*) GetMemory(Size);
		for (i=0;i<pTr4->NSoundEffect;i++) {
			pSound = &pTr4->pVetSoundEffects[i];


			fread(&pSound->SizeUncompress ,4,1,pFile);
			fread(&pSound->SizeCompress,4,1,pFile);
			if (pSound->SizeCompress) {
				pSound->pSoundBytes = (BYTE *) GetMemory(pSound->SizeCompress);
				fread(pSound->pSoundBytes, pSound->SizeCompress,1,pFile);
			}

		}
	}

	return true;
}

bool CaricaCabinet(char *pNomeCabinet, Cab_Cabinet *pCabinet) {
	FILE *pFile;

	if (EsisteFile(pNomeCabinet)==false) {
		sprintf(BufferLog, "ERROR: not found cabinet file: %s", pNomeCabinet);
		InviaLog(BufferLog);
		return false;
	}
	
	pFile = fopen(pNomeCabinet, "rb");
	BaseTr4Zl.pFile=pFile;
	BaseTr4Zl.SizeFile = QuantoLungo(pFile);

	// caricare header
	fread(&pCabinet->Header, sizeof(struct Cab_Cabinet),1,pFile);

	fseek(pFile, pCabinet->Header.SizeHeader, SEEK_SET);

	// caricare livello meta
	if (CaricaTr4DaFile(&pCabinet->MetaLevel)==false) {
		sprintf(BufferLog, "ERROR: failed loading of meta tr4 level from cabinet file: %s", pNomeCabinet);
		InviaLog(BufferLog);
		return false;
	}
	fclose(pFile);
	// caricare anche ng header
	if (ExtractNGHeader(pNomeCabinet, &pCabinet->ExtraNgHeader) == false) {
		sprintf(BufferLog, "ERROR trying to load extra ng header from cabinet file: %s", pNomeCabinet);
		InviaLog(BufferLog);
		return false;
	}
	return true;
}
// estrae da floor data che parte da valore IndiceStart, la serie di dati
// relativi a triggers (se TestTriggers==true) oppure 
// se TestTriggers==false, quelli relativi a collisioni
// Se TestTogliFineSerie == true, rimuove il flag di fine sequenza dal blocco
//		ATTENZIONE: se invece =false, allora lo piazza forzatamente all'ultimo main (a meno che non sia 0)
// altrimenti la lascia e ci sara' sempre

StrFloorEstratti * EstraiFloorData(Cab_BaseFloorData * pFloor, int IndiceStart,
									bool TestTriggers, bool TestTogliFineSerie, int * pTotEstratti)
{
	static StrFloorEstratti VetEstratti[500];
	int TotEstratti;
	WORD UltimaFunzione;
	int Indice;
	WORD Valore;
	bool TestMemo;
	WORD Operando;
	WORD Numero;
	WORD MainCode;
	int IndiceFineSerie;

	TotEstratti=0;


	Indice=IndiceStart;
	while (Indice < pFloor->TotFloorData) {
		// salva indice dove si trovera' ultimo maincode (che sara' poi l'ultimo, quello col flag di fine serie)
		IndiceFineSerie = TotEstratti;
		MainCode=pFloor->pVetFloorData[Indice];
		Indice++;
		if (MainCode==0) goto Termina;
		UltimaFunzione = MainCode & 0x1f;
		// vedere se e' da memorizzare
		TestMemo=false;
		if (UltimaFunzione == floor_Climb || UltimaFunzione == floor_Lava || UltimaFunzione == floor_MINER_TYPE ||
		   UltimaFunzione == floor_MONKEY_TYPE || UltimaFunzione == floor_Trigger ||
		   UltimaFunzione == floor_TRIGTRIGGER_TYPE) {
			// roba trigger
			if (TestTriggers==true) TestMemo=true;
		}else {
			// roba collisioni
			if (TestTriggers==false) TestMemo=true;
		}
		
		if (TestMemo==true) {
			// salvare la word iniziale
			VetEstratti[TotEstratti].IndexFloorOld = Indice-1;
			VetEstratti[TotEstratti++].ValTrigger  = MainCode;
		}

		// ora scandire (e salvare o meno) tutte le word seguenti di questo stesso blocco
		// ALCUNI hanno un'estra word
		// altri nessuna
		// e poi il trigger puo' averne molte e variabili
		// --------- codici con UNA extra word -------------------
		if ((UltimaFunzione >= floor_Door && UltimaFunzione <= floor_Roof) ||
			(UltimaFunzione >= floor_Split1 && UltimaFunzione <= floor_Split4) ||
			(UltimaFunzione >= floor_NOCOLF1T && UltimaFunzione <= floor_NOCOLC2B)) {
			// c'e' un'extra word
			Operando = pFloor->pVetFloorData[Indice];
			Indice++;
			if (TestMemo == true) {
				VetEstratti[TotEstratti].IndexFloorOld = Indice-1;
				VetEstratti[TotEstratti++].ValTrigger  = Operando;

			}
        
		}
    
		//------------ triggers ------------------------------
		if (UltimaFunzione == floor_Trigger) {
			Valore = pFloor->pVetFloorData[Indice];
			Indice++;
			if (TestMemo == true) {
				//salvare Trigger button
				VetEstratti[TotEstratti].IndexFloorOld = Indice-1;
				VetEstratti[TotEstratti++].ValTrigger  = Valore;
			}
			// ora c'e' la serie di trigger: alcuni avranno una word extra, altri no
			do {
				Valore = pFloor->pVetFloorData[Indice];
				Indice++;

				if (TestMemo == true) {
					// salvare Trigger valore trigger
					VetEstratti[TotEstratti].IndexFloorOld = Indice-1;
					VetEstratti[TotEstratti++].ValTrigger  = Valore;
				}
				Numero = Valore & 0x3C00;
				// era: Numero = Numero \ (2 ^ 10)
				Numero = Numero >> 10;

				if (Numero == 1 || Numero == 9 || Numero == 11) {
					// c'e' un dato extra per camera target o flyby o flipeffect o action
					Valore = pFloor->pVetFloorData[Indice];
					Indice++;
					if (TestMemo == true) {
						// salvare Trigger valore trigger
						VetEstratti[TotEstratti].IndexFloorOld = Indice-1;
						VetEstratti[TotEstratti++].ValTrigger  = Valore;
					}
                
				}
			}while ((Valore & 0x8000) == 0);
		}
		// qui devo scoprire se e' finito l'intero blocco
		if (MainCode & 0x8000) goto Termina;
	}
Termina:
	// vedere se va tolto (o messo) flag di fine serie
	if (IndiceFineSerie < TotEstratti) {
		if (TestTogliFineSerie==true) {
			VetEstratti[IndiceFineSerie].ValTrigger  &= 0x7fff;
		}else {
			// mettere fine serie
			VetEstratti[IndiceFineSerie].ValTrigger  |= 0x8000;
		}
	}

	// imposta valori modifcati

	*pTotEstratti= TotEstratti;
	return VetEstratti;

}

void SetClipboard(char *pTesto)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	HGLOBAL pGlob;
	char *pCarGlob;

	// registrare negli appunti
	pGlob=GlobalAlloc(GHND,(DWORD) strlen(pTesto)+1);
	if (pGlob == NULL) return;

	pCarGlob=(char*) GlobalLock(pGlob);
	strcpy(pCarGlob,pTesto);

	GlobalUnlock(pGlob);
	OpenClipboard(*pMiaWind);
	EmptyClipboard();
	SetClipboardData(CF_TEXT,pGlob);
	CloseClipboard();		
}
// alloca memoria per ospitare pstringa, la copia dentro e restituisce la memoria
char *AllocaStringa(char *pStringa)
{
	char *pMex;

	if (pStringa==NULL) return NULL;

	pMex = (char*) malloc(strlen(pStringa)+1);

	strcpy(pMex,pStringa);
	return pMex;

}
// invia il log in un file di testo in cartella trle
// viene usato quando non si puo' usare invialog standard
void LogTestuale(char *pMex)
{
	FILE *pLog;

	pLog =fopen(GetFileCrash("ng_startup_log.txt"),"a+t");
	fprintf(pLog,"%s\r\n", pMex);
	fclose(pLog);
}
// trasforma scritta ascii numerica "423" in numero
// se c'e' un errore imposta =true in pTestErrore
// restituisce il numero convertito
// se il primo carattere e' '$' lo converte da esadecimale
int GetNumero(char *pCifre, bool *pTestErrore)
{
	int Numero;
	*pTestErrore=false;

	Numero = 81818181;
	if (pCifre[0] == '$') {
		pCifre++;
		sscanf(pCifre, "%x", &Numero);
	}else {
		sscanf(pCifre,"%d", &Numero);
	}

	if (Numero == 81818181) {
		*pTestErrore=true;
		return 0;
	}
	return Numero;

}
// legge header ng di file .prj e copia i dati in GlobNgle

void LeggiNGHeaderProject(char *pNomeFile)
{

	int i;
	DWORD Size;
	WORD NumeroWord;
	StrParseNGField  ParseField;
	StrExtractNG ExtractNG;
	StrPluginNames VetPluginCopia[MAX_PLUGINS];
	int TotPluginCopia;
	char *pNomeCopia;
	int z;
	int IdCopia;
	int j;

	GlobNgle.RoomTexAnimate.TestPresente=false;
	// qui imposta a 64 dimensioned i default ma in realta', in tom2pc
	// verra' poi calcolata la dimensione standard delle tail
	// sulla base delle prima tail infos lette da file .tom
	GlobNgle.RoomTexAnimate.SizeDefault = 64;
	GlobNgle.RoomTexAnimate.TotaleRangeNG=0;
	GlobNgle.RoomTexAnimate.Tot_UV_Rotate=0;

	for (i=0;i<40;i++) {
		GlobNgle.RoomTexAnimate.VetInfoRangeAnim[i] =0;
	}


	GlobNgle.VersioneProject.Flags=0;
	for (i=0;i<4;i++) {
		GlobNgle.VersioneProject.VetVersione[i]=0;
	}
	

	ExtractNG.pNGArray=NULL;
	if ( ExtractNGHeader(pNomeFile,&ExtractNG)==false) {

		switch (ExtractNG.Result ) {
		case -2:

			sprintf(BufferLog,"ERROR: cann't open file: \"%s\" (to read extra ng header)",SoloNome(pNomeFile));
		
			InviaLog(BufferLog);

			return;
		case -1:

			InviaLog("WARNING: Extra NG Header appears corrupted");

			return;
		case 0:

			InviaLog("Current file has no Extra NG Header");

			return;
		default:
			sprintf(BufferLog,"UNKNOWN ERROR trying to read NG header from file: %s",pNomeFile);
			InviaLog(BufferLog);
			return;
		}
	}


	InviaLog("Found Extra NG Header");

	// scandire dati, saltando prima word di controllo "NG"
	i=0;
	while (ParseNgField(ExtractNG.pNGArray ,i,&ParseField)) {
		
		// analizzare tipo di pacchetto
		switch (ParseField.Type) {

		case NGTAG_PLUGIN_NAMES_NGLE:
			j=0;
			// prima copiarli in una zona temporanea per controllare gli id
			TotPluginCopia=ParseField.pData[j++];

			memcpy(&VetPluginCopia[0], &ParseField.pData[j], sizeof(StrPluginNames) * TotPluginCopia);

			// ora fare in modo che gli id di quelli caricati dal cartella trle siano modificati
			// in modo da essere uguali a quelli stabiliti nel progetto
			// dare errore se manca qualcosa
			// idea: in quelli finali mettere il numero di id corretto (letto da file prj + 1000)
			// cois' alla fine si scopre se qualcuno manca e si puo' fare correzione

			for (j=0;j<TotPluginCopia;j++) {
				pNomeCopia= VetPluginCopia[j].Nome;
				IdCopia=VetPluginCopia[j].PluginId;

				for (z=0;z<GlobNgle.TotPluginNgle;z++) {
					if (strcmpi(pNomeCopia, GlobNgle.VetPluginNgle[z].Nome )==0) {
						// trovato: forzare id
						GlobNgle.VetPluginNgle[z].PluginId = IdCopia+1000;
						// e inserire totusati
						GlobNgle.VetPluginNgle[z].TotUsato = VetPluginCopia[j].TotUsato;
						break;
					}
				}
				if (z== GlobNgle.TotPluginNgle) {
					// questo manca: dare messaggio d'errore. se risultava essere stato usato
					if (VetPluginCopia[j].TotUsato > 0) {

						sprintf(BufferLog, "WARNING: current project could have used (%d) triggers for %s plugin, but this plugin is not currently loaded in NGLE program\r\nIf you try now to modifying/converting this project and saving it, all triggers that had been set for (missing) plugin will be broken forever.\r\nIt's better now to quit ngle (avoiding to save current project) and then try to get the missing plugin. When you found it, install it in [Plugin] panel of NG_Center with [Install New Plugin] button. Then you'll be able to reload this project",
							VetPluginCopia[j].TotUsato, pNomeCopia);
						MioMessageBox(BufferLog, "Missing Plugin");
					}
				}

			}

		

			// ora vedere se nei plugin di base ve ne sono alcuni (in piu') che non avevano un ID nel progetto
			// a quel punto bisognera' trovare un id nuovo per loro
			for (j=0;j<GlobNgle.TotPluginNgle;j++) {
				if (GlobNgle.VetPluginNgle[j].PluginId < 1000) {
					// non modifcato
					// si vede che e' un plugin nuovo che non aveva trigger nel progetto
					// darlgi un id qualunque ma divers da tutti gli altri
					IdCopia= 1000; // in modo che parta da 1001 ossia primo id valido per plugin "1" + 1000
					do {
						IdCopia++;
						for (z=0;z<GlobNgle.TotPluginNgle;z++) {
							if (GlobNgle.VetPluginNgle[z].PluginId == IdCopia) break;
						}
					}while(z < GlobNgle.TotPluginNgle);

					GlobNgle.VetPluginNgle[j].PluginId= IdCopia;
				}
			}

			// ora ripristina gli id dei plugin togliendo il +1000
			for (j=0;j<GlobNgle.TotPluginNgle;j++) {
				if (GlobNgle.VetPluginNgle[j].PluginId < 1000) {
					// errore interno
					MioMessageBox("Internal error in adjustment plugin IDs phase","Internal Error");
				}else {
					GlobNgle.VetPluginNgle[j].PluginId -= 1000;
				}
			}


			break;

		case NGTAG_ANIMATED_TEXTURES:
			
			Size= sizeof(StrDatiExtraAnimazioni);
			if (Size == ParseField.SizeData) {

				// vettore di 40 elementi con info per animazioni
				memcpy(&GlobNgle.RoomTexAnimate, ParseField.pData, 
						sizeof(StrDatiExtraAnimazioni));
				GlobNgle.RoomTexAnimate.TestPresente=true;
			}else {
				memset(&GlobNgle.RoomTexAnimate,0, 
						sizeof(StrDatiExtraAnimazioni));
			}


			break;
		case NGTAG_VERSION_HEADER:

			Size = sizeof(StrVersionHeader);

			memcpy(&GlobNgle.VersioneProject, ParseField.pData, Size);
			
			break;


		case NGTAG_SLOT_NAMES:
			i=0;


			// carica lista di nomi custom per slot
			NumeroWord =ParseField.pData[i++];
			if (NumeroWord == 0) break;

			VetMieiNomiSlot.TotNomiSlot = NumeroWord;

			memcpy(VetMieiNomiSlot.NomeWad, &ParseField.pData[i], 64);
			i+= 32;
			memcpy(&VetMieiNomiSlot.VetNomi[0], &ParseField.pData[i], 
				sizeof(StrNomeSlot) * NumeroWord);

			AggiornaSlotNomi();
			break;


		}
		// puntare a chunk successivo
		i = ParseField.NextIndex;
	}
	free(ExtractNG.pNGArray );
	ExtractNG.pNGArray=NULL;

}


// salva in file pNomeFile l'extra ng header del project attuale
void SalvaNGHeaderProject(char *pNomeFile)
{

	WORD *pVetExtra=NULL;
	int NWords;
	static int *pTotRangeExported = (int*) 0x568280;
	static int *VetIndiciRange = (int*) 0x56C440;
	static DWORD *pVersioneWad = (DWORD *) 0x624570;


	NWords =0;

	CopiaInHeader(NGTAG_ANIMATED_TEXTURES, NO_ARRAY, 
			sizeof(StrDatiExtraAnimazioni), &GlobNgle.RoomTexAnimate,
			&pVetExtra, &NWords);

	GlobNgle.RoomTexAnimate.TestPresente=true;



	// ----- salva nomi slot ---------
	if (VetMieiNomiSlot.TotNomiSlot) {
		CopiaInHeader(NGTAG_SLOT_NAMES, NO_ARRAY,
			66 + sizeof(StrNomeSlot) * VetMieiNomiSlot.TotNomiSlot,
			&VetMieiNomiSlot,
			&pVetExtra, &NWords);

	}
	CopiaInHeader(NGTAG_VERSION_HEADER, NO_ARRAY,
				sizeof(StrVersionHeader), &GlobNgle.BaseVersione ,
				&pVetExtra, &NWords);

	// ----- salva plugin names ------------------------

	CopiaInHeader(NGTAG_PLUGIN_NAMES_NGLE, GlobNgle.TotPluginNgle,sizeof(StrPluginNames),  &GlobNgle.VetPluginNgle[0],
			&pVetExtra, &NWords);

	if (WriteNGHeader(pNomeFile,pVetExtra,NWords)==false) {

		sprintf(BufferLog,"ERROR: Trying to save extra ng header, cann't find or open file: %s", pNomeFile);
		InviaLog(BufferLog);
		MostraMessageBoxNGLE(BufferLog,3);

	}
	if (pVetExtra !=NULL) free(pVetExtra);
}

bool IsLineaNulla(char *pLinea)
{
	DWORD i;
	char MioCar;

	// se c'e' un carattere non nullo (invisibile) restituisce false

	for (i=0;i<strlen(pLinea);i++) {
		MioCar = pLinea[i];
		
		if (MioCar != ' ' && MioCar != '\t' && MioCar != '\r' && MioCar != '\n') return false;
	}

	return true;
}
// carica l'intero file ng_constants.txt in memoria
// TipoLoading e' una costant LNGC_... 
// se TestMain = true carfica da risorse il file principale
// se testmain = false carica "my_ng_constants.txt" 
// e aggiorna in modo dinamico le voci trovate
void CaricaNGConstants(WORD TipoLoading, StrNGConstants * pBaseCostanti, char *pNomeFile )
{

	int n,z;
	char *pBufLinea;
	DWORD SizeLinea;
	char *pMexRemark;
	char *pMexHelp;
	char *pMexTipoTrigger;
	bool TestMainTrigger; // se action, flipeffect o condition
	int IndicePrimaNota;
	int IndiceRemark;
	int IndiceHelp;
	int Step; // per #repeat#
	bool TestDentro;
	char *pChar;
	char *pBaseMyConstants;
	char **VetParti;
	int BaseValori;
	char *pStrNumero;
	bool TestLineaConNumero;
	char SalvaCar;
	int Inizio,Fine;
	StrRecordSezione MyTempRecord;
	bool TestErrore;
	char *pFound;
	int LastTriggerNumber;
	int TotParti;
	int TipoShow;
	int Indice;
	int NSez;
	int Numero;
	bool TestMain;
	char *pTesto;
	int i,j;
	int SizeTesto;
	FILE  *pFile;
	int *pInt;
	char *pMemTemp;
	bool TestFinito;
	bool TestContinua;
	char VetDecrypt[] = {33,127,67,91,55,66, 33, 23,54,0};
	bool TestPlugin; 
	char *pFineTesto;
	StrRecordSezione *pTempRecord;
	HRSRC HandleRes;
	char BufLastValidLinee[256];
	char *pTempLine;


	static char *VetSezioni[] = {"TRIGGERWHAT", "EFFECT",
			"TRIGGERTYPE", "CONDITION", "ACTION", "TEXTS"};

	static char *VetListe[] = {"#WAD-SLOTS#", 
				"#INVENTORY-ITEMS#",
				"#MOVEABLES#", "#DEGREES#","#SEQUENCE_32#", 
				"#SEQUENCE_128#","#COLORS#", "#INVENTORY-ITEM-INDEX#",
				"#FRAG_2x2#", "#FRAG_3x3#", "#FRAG_4x4#", "#HALF_CLICKS#",
				"#HALF_CLICKS_32#", "#TIME_LIST_128#", "#ANIMATION_RANGE#", 
				"#STRING_LIST_255#", "#NG_STRING_LIST_255#", "#CD_TRACK_LIST#",
				"#FMV_LIST#", "#SOUND_EFFECT_A#", "#SOUND_EFFECT_B#", 
				"#TIME_LIST_32#", "#ANIMATION_LIST_32A#", "#ANIMATION_LIST_32B#", 
				"#ANIMATION_LIST_32C#", "#ANIMATION_LIST_255#", "#STATE_ID_LIST#",
				"#LARA_ANIM_SLOT#", "#LARA_POS_OCB#", "#ANIMATION_LIST_B#", 
				"#KEYBOARD_MODE#", "#ADD_EFFECT_255#", "#CLICK_DISTANCE_32#",
				"#ROOMS_255#", "#PERCENTAGE#", "#TEX_SEQUENCE#", 
				"#BACKUP_LIST#","#SWAP_MESH_SLOT#",
				"#SET_STANDARD_MESH#","#RECHARGE_256#", "#CAMERA_EFFECTS#",
				"#TIMER_SIGNED#","#FLYBY_LIST#", "#SINK_LIST#", 
				"#BUTTONS_LIST#", "#STATIC_LIST#", "#TIMER_SIGNED_LONG#", 
				"#STATIC_SLOTS#", "#FOG_DISTANCES#", "#PSX_STRING_LIST#", 
				"#PC_STRING_LIST#", "#FLY_BY_SEQ#", "#TRANSPARENCY32#", 
				"#VAR_NORMALS#", "#VAR_STORES#", "#VAR_TEXT#", 
				"#MEMORY_SAVE#", "#BIT_LIST#", "#NEGATIVE_NUMBERS#", 
				"#MEMORY_ITEM#", "#VAR_LONG_STORE#", "#MEMORY_CODE#",  
				"#MEMORY_SLOT#", "#MEMORY_ANIMATION#", "#MICRO_CLICKS#",  
				"#COLLISION_FLOOR#", "#COLLISION_CEILING#", 
				"#MEM_INVENTORY_INDICES#", "#MEMORY_INVENTORY#",
				"#LARA_OTHER_SLOTS#", "#SLOT_MESH_MOVEABLES#",
				"#NG_STRING_LIST_ALL#", "#SFX_1024#", "#NG_STRING_LIST_512#",
				 NULL};


	TestMain=false;
	TestPlugin=false;

	if (TipoLoading == LNGC_MAIN) TestMain=true;


	if (TipoLoading == LNGC_PLUGIN) TestPlugin=true;

	LastTriggerNumber=-1;
	TestMainTrigger=false;
	if (TestMain) {
			// quando e' file principale si trova nelle risorse binarie
		pBaseCostanti->TotSezioni=0;

		HandleRes = FindResource(MiaIstanzaDll, 
						MAKEINTRESOURCE(IDR_NGCONSTANTS), "LCURSOR");
		if (HandleRes == 0) return;

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
		pNomeFile="<Internal NG_Constants.txt>";
	}else {

		MyTempRecord.TotValori=0;
		MyTempRecord.pVetValori=NULL;
		// caricare file esterno
		pChar = CaricaFile(pNomeFile);
		if (pChar==NULL) {
			sprintf(BufferLog,"SYNTAX ERROR trying to scan trigger list of file: %s\r\nFILE NOT FOUND",
						pNomeFile);
			MostraMessageBoxNGLE(BufferLog,3);
			return;
		}

		pBaseMyConstants = pChar;
		pFile = fopen(pNomeFile, "rb");

		SizeTesto = QuantoLungo(pFile);
		fclose(pFile);
		pTesto = pBaseMyConstants;
		pMemTemp = pBaseMyConstants;
	}


	if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
		pBaseCostanti->IndiceSezTesti=-1;
	}
	TestDentro=false;


	pFineTesto= &pTesto[SizeTesto];

	while (pTesto < pFineTesto && *pTesto != 0) {
		// caricare in pBufLinea la linea a cui punta pTesto attualmente
		TestFinito=false;
		// salvare inizio linea
		// poi calcolare dimensione complessiva linea
		pBufLinea = pTesto;
		SizeLinea=0;
		while (*pTesto != 0x0d && *pTesto != 0x0a) {
			if (pTesto >= pFineTesto || *pTesto == 0) {
				TestFinito=true;
				break;
			}

			SizeLinea++;
			pTesto++;
		}

		if (TestFinito==true) break;
		// saltare eventuali codici 0x0d e 0x0a in fondo a linea attuale

		while (*pTesto == 0x0d || *pTesto == 0x0a ) {
			pTesto++;

		}
		// mettere zero alla fine di linea in modo da avere stringa completa
		SalvaCar= pBufLinea[SizeLinea];
		pBufLinea[SizeLinea]=0;
		// vedere se salvare questa lina come ultima linea valida
		if (strlen(pBufLinea) > 5) {
			if (strlen(pBufLinea) < 255) {
				strcpy(BufLastValidLinee, pBufLinea);
			}else {
				// e' troppo lunga per copiarla tutta, copiare solo gli ultimi 250 bytes
				n = strlen(pBufLinea) - 250;
				pTempLine = &pBufLinea[n];
				strcpy(BufLastValidLinee, pTempLine);
			}


		}

		// vedere se ci sono dati extra per #REMARK# o #START_DOC
		IndiceRemark=-1;
		IndiceHelp=-1;
		pMexHelp=NULL;
		pMexRemark=NULL;


		if (pBufLinea[0] != ';') {
				
			IndiceRemark = InStr(0, pBufLinea, "#REMARK#");
			IndiceHelp = InStr(0,pBufLinea, "#START_DOC#");

		}

		IndicePrimaNota=IndiceRemark;
		if (IndicePrimaNota == -1) IndicePrimaNota = IndiceHelp;

		if (IndicePrimaNota != -1) {
			// ripristare vecchia linea, escludendo zone di remark

			pBufLinea[SizeLinea]=SalvaCar;
			// ora pBufLinea punta a descrizione SENZA remark o help
			pBufLinea[IndicePrimaNota]=0;
			
			// salvare eventuale remark
			if (IndiceRemark != -1) {
				IndiceRemark += strlen("#REMARK#");
				pMexRemark =  &pBufLinea[IndiceRemark];
				// mettere zero per remark
				if (IndiceHelp == -1) {
					// non c'era #START_DOC#  allora usare SizeLinea
					pBufLinea[SizeLinea]=0;
				}else {
					// c'era anche #START_DOC#: fare finire qui testo remark
					pBufLinea[IndiceHelp]=0;
				}
			}

			// salvare eventuale #start_doc#
			if (IndiceHelp != -1) {
				IndiceHelp += strlen("#START_DOC#");
				pMexHelp = &pBufLinea[IndiceHelp];
				// saltare eventuale riga vuota inziale
				while (*pMexHelp == '\r' || *pMexHelp=='\n') pMexHelp++;

				// azzerarlo dove c'e' #END_DOC#
				i = InStr(0, pMexHelp, "#END_DOC#");
				if (i == -1) {
					// errore
					sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, syntax error in #START_DOC#, missing the #END_DOC# tag after line :\r\n%s\r\nAfter line: %s",
								pBufLinea, BufLastValidLinee);
					MostraMessageBoxNGLE(BufferLog,3);
					free(pBaseMyConstants);
					return;
				}
				pMexHelp[i]=0;

				// impostare pTesto per prossima linea
				i+= strlen("#END_DOC#");


				pTesto = &pMexHelp[i];

				// saltare eventuali caratteri  
				while (*pTesto == '\r' || *pTesto == '\n') pTesto++;
			
			}
		}
			

		// analisi speciale per due possibili formati:
		// 

		if (TestDentro == true) {
			if (strcmpi(SuperTrim(pBufLinea),"<END>")==0) {
				TestDentro=false;
				TestMainTrigger=false;
				if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
					pBaseCostanti->TotSezioni++;
				}else {
					// completato caricamento di record trasdotto
					// in mytemprecord
					// ora cercare corrispondenza in base costanti
					// e sostituire tutto
					for (i=0;i<pBaseCostanti->TotSezioni;i++) {
						if (MyTempRecord.NumeroSezione == 
								pBaseCostanti->pVetSezioni[i].NumeroSezione &&
							MyTempRecord.TipoSezione == 
								pBaseCostanti->pVetSezioni[i].TipoSezione &&
							MyTempRecord.TipoListaShow ==
							pBaseCostanti->pVetSezioni[i].TipoListaShow) {

							// ok, trovata, ora copiarci le descrizioni
							// e liberare quelle di temprecord
							pTempRecord = &pBaseCostanti->pVetSezioni[i];

							// cercare solo i valori presenti
							for (j=0;j<MyTempRecord.TotValori;j++) {
								n=MyTempRecord.pVetValori[j].Numero;


								for (z=0;z<pTempRecord->TotValori;z++) {
									if (pTempRecord->pVetValori[z].Numero == n) break;
										
									
								}
								if (z < pTempRecord->TotValori) {

									free(pTempRecord->pVetValori[z].pDescrizione);
									if (pTempRecord->pVetValori[z].pMexHelp) {
										free(pTempRecord->pVetValori[z].pMexHelp );
										pTempRecord->pVetValori[z].pMexHelp =NULL;
									}
									if (pTempRecord->pVetValori[z].pMexRemark) {
										free(pTempRecord->pVetValori[z].pMexRemark);
										pTempRecord->pVetValori[z].pMexRemark=NULL;
									}
						

									pTempRecord->pVetValori[z].pDescrizione  = 
										AllocaStringa(MyTempRecord.pVetValori[j].pDescrizione);

									pTempRecord->pVetValori[z].pMexHelp  = 
										AllocaStringa(MyTempRecord.pVetValori[j].pMexHelp);

									pTempRecord->pVetValori[z].pMexRemark  = 
										AllocaStringa(MyTempRecord.pVetValori[j].pMexRemark);
								}
							}
							// ora lierare memora anche anche di vettore
							// temporaneo
							
							if (MyTempRecord.TotValori) {

									free(MyTempRecord.pVetValori);
									MyTempRecord.pVetValori=NULL;
							}
							
							break;
						}
					}
				}
			}else {
				// siamo dentro ma non e' linea <END>
				// vedere se e' linea di commento
				if (pBufLinea[0] != ';' && IsLineaNulla(pBufLinea)==false) {
					// non e' commento
					// invece di Split() usare controllo manuuale considerando solo il prino ":"
					// vedere se e' linea nel formato;
					// 135: Descrizone trigger
					// oppure altro tipo, come linea:
					// <START_TRIGGERWHAT_9_O_H>

					
					TestLineaConNumero=false;
					i = InStr(0, pBufLinea, ":");
					if (i != -1) {
						// se i e' troppo grande, allora non sta prendendo il separatore per il numero ma chissa' cosa
						if (i > 5) i=-1;
					}
					if (i !=-1) {
						pStrNumero= pBufLinea;
						pStrNumero[i]=0;
						pBufLinea += i+1;
						TestLineaConNumero=true;

					}

					if (TestLineaConNumero == false) {
						// vedere se e' una lista prefefinita
						TestContinua=false;
						TotParti=1;
						// prima controllare se e' la linea
						// speciale #SAME_OF#=
						pFound = Search(pBufLinea,"#SAME_OF#=",ts_Avanti,NULL);
						if (pFound) {

							// TROVATA COPIA DI LISTA VALORI #SAME_OF#=
							pFound += strlen("#SAME_OF#=");
							VetParti = Split(pFound,'_',&TotParti,NULL,NULL,false);
							if (TotParti != 3) {
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, syntax error in #SAME_OF#= tag in line :\r\n%s\r\nAfter line:%s",
											pBufLinea, BufLastValidLinee);
								MostraMessageBoxNGLE(BufferLog,3);
								free(pMemTemp);
								return;
							}								
							// ci dovrebbero essere solo due parti:
							// la prima e' il tipo "EFFECT" o "ACTION"
							// l'altra e' il numero
							// verificare nome sezione
							pChar = VetParti[0];

							for (i=0;i<SEZ_TOT;i++) {
								if (strcmpi(VetSezioni[i], pChar) == 0) break;
							}
							if ( i == SEZ_TOT) {
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, unknown name of section %s in line :\r\n%s\r\nAfter line: %s",
											pChar, pBufLinea, BufLastValidLinee);
								MostraMessageBoxNGLE(BufferLog,3);
								free(pMemTemp);
								return;
							}
							NSez=i;
							// ora prendere numero
							Numero = -8999;
							pChar =VetParti[1];
							sscanf(pChar,"%d", &Numero);
							if (Numero == -8999) {
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, syntax error in #SAME_OF#=  tag in line :\r\n%s\r\nAfter line: %s",
											pBufLinea, BufLastValidLinee);
								MostraMessageBoxNGLE(BufferLog,3);
								return;
							}
							// devo trovare anche tipo show 
							// osia "T" "H" "B" ecc.
							pChar = VetParti[2];
							switch (pChar[0]) {
							case 'T':

								TipoShow =	SHOW_IN_TIMER;
								break;
							case 'O':
								TipoShow =	SHOW_IN_OBJECT;
								break;

							case 'E':

								TipoShow  = SHOW_IN_EXTRA;
								break;
							case 'B':
								TipoShow =	SHOW_IN_BUTTONS;
								break;
							default:
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list. In #SAME_OF#= tag the section tag is not correct, the 4th parameter in following line :\r\n%s\r\n\r\nUnknown type output list '%c', the valid values are 'O' (Object), 'T' (Timer) (B) Buttons and 'E' (Extra)",
											pBufLinea, pChar[0]);
								MostraMessageBoxNGLE(BufferLog,3);
								free(pMemTemp);
								return;
							}							
							// ok, cercare quella lista in quelle gia' memorizzate
							for (i=0;i<pBaseCostanti->TotSezioni;i++) {
								if (pBaseCostanti->pVetSezioni[i].NumeroSezione == Numero && 
									pBaseCostanti->pVetSezioni[i].TipoSezione == NSez && 
									pBaseCostanti->pVetSezioni[i].TipoListaShow == TipoShow) {

									TestContinua=true;
									break;
								}
							}

							if (TestContinua==false) {
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, not yet defined the list required in #SAME_OF#=  tag in line :\r\n%s\r\nAfter line:%s",
											pBufLinea, BufLastValidLinee);
								MostraMessageBoxNGLE(BufferLog,3);
								free(pMemTemp);
								return;
							}

							// ok ora copiare tutto o quasi
							Indice = i;

							if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
								j = pBaseCostanti->TotSezioni;
								pTempRecord = &pBaseCostanti->pVetSezioni[j];
							}else {
								// secondario
								pTempRecord = &MyTempRecord;
							}
							pTempRecord->TotValori = 
								pBaseCostanti->pVetSezioni[Indice].TotValori;

							// creare memoria per ospitare tutti i valori
							pTempRecord->pVetValori = (StrRecordRigaConst *)
									malloc(sizeof(StrRecordRigaConst) * 
											pTempRecord->TotValori);


							// copiare solo lista e valori
							for (i=0;i<pBaseCostanti->pVetSezioni[Indice].TotValori;i++) {
							    pTempRecord->pVetValori[i].Numero = 
									pBaseCostanti->pVetSezioni[Indice].pVetValori[i].Numero;
								pTempRecord->pVetValori[i].pDescrizione = 
									AllocaStringa(pBaseCostanti->pVetSezioni[Indice].pVetValori[i].pDescrizione);
								pTempRecord->pVetValori[i].pMexHelp =
									AllocaStringa(pBaseCostanti->pVetSezioni[Indice].pVetValori[i].pMexHelp);

								pTempRecord->pVetValori[i].pMexRemark =
									AllocaStringa(pBaseCostanti->pVetSezioni[Indice].pVetValori[i].pMexRemark);
							
							}

						}
						
						if (TestContinua==false) {
							// non e' lkista sameof
							// vedere pero' se e' altra linea 
							// speciale
							// #REPEAT#NOME#1#100
							// 0    1       2            3  4    5        6   
							//   #REPEAT#GlobalTrigger=#1#127#StartValue#step
							// opzionalmentre puo' essere un altro valore
							// e se c'e' e' il valore di base
							// da cui partire nei valori
							

							if (InStr(0,pBufLinea, "#REPEAT#") != -1) {
								// e' una lista da costruire
								// dinamicamente
								VetParti = Split(pBufLinea,'#',
												&TotParti,NULL,NULL,false);
								TestContinua=true;

								if (TotParti < 5) {				
									sprintf(BufferLog,"ERROR #REPEAT# tag has a wrong number of arguments in line:\r\n%s\r\nAfter line: %s",
											pBufLinea, BufLastValidLinee);
									MostraMessageBoxNGLE(BufferLog,3);
									break;
								}
								// trovare il numero iniziale e finale
								Inizio = GetNumero(VetParti[3], &TestErrore);
								if (TestErrore==true) {
									sprintf(BufferLog,"ERROR in #REPEAT# tag: start number is not decimal in line: %s",
										pBufLinea);
									MostraMessageBoxNGLE(BufferLog,3);
									break;
								}
								
								Fine = GetNumero(VetParti[4], &TestErrore);
								if (TestErrore==true) {
									sprintf(BufferLog,"ERROR in #REPEAT# tag: end number is not decimal in line: %s",
										pBufLinea);
									MostraMessageBoxNGLE(BufferLog,3);
									break;
								}

								// e' stato fornito un startvalue (diverso da quello Inizio?
								BaseValori =Inizio;

								if (TotParti >= 6) {
									// e' stato fornito il valore
									// di partenza da assegnare
									BaseValori = GetNumero(VetParti[5], &TestErrore);
									if (TestErrore==true) {
										sprintf(BufferLog,"ERROR in #REPEAT# tag: [Start Value] number is not a decimal value in line: %s",
											pBufLinea);
										MostraMessageBoxNGLE(BufferLog,3);
										free(pMemTemp);
										break;
									}

								}
								// e' stato fornito un valore Step (diverso da 1)?
								
								Step=1;
								if (TotParti >= 7) {
									Step = GetNumero(VetParti[6], &TestErrore);
									if (TestErrore==true) {
										sprintf(BufferLog,"ERROR in #REPEAT# tag: [Step Value] number is not a decimal value in line: %s",
											pBufLinea);
										MostraMessageBoxNGLE(BufferLog,3);
										free(pMemTemp);
										break;
									}
								}
								
								// ok, ora costruire lista costanti
								// chiedere memoria
								if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
									j=pBaseCostanti->TotSezioni;
									pTempRecord = &pBaseCostanti->pVetSezioni[j];


								}else {
									// secondario
									pTempRecord = &MyTempRecord;
								}
								// chiedere memroia per ospitare
								// tutti i valori
								pTempRecord->TotValori= Fine-Inizio+Step;

								pTempRecord->TotValori /= Step;
								

								pTempRecord->pVetValori = 
										(StrRecordRigaConst *) malloc(
											sizeof(StrRecordRigaConst) * 
												pTempRecord->TotValori);
								
								// che palle devo allocare
								// memoria per ogni nuovo testo da copiare
								i=0;

								for (j=Inizio;j<=Fine;j+= Step) {
									sprintf(BufferLog, "%s %5d", 
											VetParti[2], j);
									pTempRecord->pVetValori[i].pDescrizione=
												AllocaStringa(BufferLog);
									pTempRecord->pVetValori[i].pMexHelp = NULL;
									pTempRecord->pVetValori[i].pMexRemark = NULL;

									pTempRecord->pVetValori[i].Numero = BaseValori;
	
									BaseValori++;
									

									i++;
								}


							}
						}
						if (TestContinua==false && (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN)) {
							// vedere se corrisponde a lista
							// preconfezionata
							if (TestLineaConNumero==false) {
								i=0;
								pChar = SuperTrim(pBufLinea);
								while (VetListe[i] != NULL) {
									if (strcmpi(VetListe[i], pChar) ==0) {
										TestContinua=true;
										j=pBaseCostanti->TotSezioni;
										pBaseCostanti->pVetSezioni[j].ListPredefinito = i+1;
										break;
									}
									i++;
								}
							}
							if (TestContinua==false) {
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list (%s) in following line :\r\n%s\r\nAfter line: %s",
											pNomeFile, pBufLinea, BufLastValidLinee);
								MostraMessageBoxNGLE(BufferLog,3);
							}
						}

					}else {
						// e' riga nel formato:
						// 24: Descrizione trigger
						// pBufLinea = linea descrizone + eventuali #REMARK# o "#START_DOC#" fino a "#END_DOC"
						// pStrNumero = stringa di numero

						
						Numero=-90000;
						sscanf(pStrNumero,"%d",&Numero);
						if (Numero == -90000) {
							i= strlen(pStrNumero);
							pStrNumero[i]=':';
							pBufLinea=pStrNumero;

							sprintf(BufferLog,"SYNTAX ERROR scanning trigger list in following line:\r\n%s\r\n\r\nFirst field is not numeric",
								pBufLinea);
							MostraMessageBoxNGLE(BufferLog,3);

						}else {
							// TROVATA NUOVO VALORE DI SEZIONE ATTUALE ******
							if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
								j=pBaseCostanti->TotSezioni;
								
								pTempRecord = &pBaseCostanti->pVetSezioni[j];
							}else {
								// secodnario
								pTempRecord = &MyTempRecord;
							}
							if (TestMain==true && LastTriggerNumber != -1 && TestMainTrigger==true &&
								(LastTriggerNumber+1) != Numero) {
								if (InStr(0,pBufLinea, "OldFlip.")==-1) {
									// dare avviso
									sprintf(BufferLog, "WARNING: in binary ng_constants perhaps it has been skipped some %s triggers from %d trigger to %d trigger\r\nLast Line: %s",
										pMexTipoTrigger, LastTriggerNumber, Numero, pBufLinea);
									MostraMessageBoxNGLE(BufferLog, 3);
								}

							}
							LastTriggerNumber=Numero;

							i = pTempRecord->TotValori;
							// aumentare numero valori
							pTempRecord->pVetValori = (StrRecordRigaConst *)
								realloc(pTempRecord->pVetValori ,
									sizeof(StrRecordRigaConst) * (i+1));

							pTempRecord->pVetValori[i].Numero = Numero;
							pTempRecord->pVetValori[i].pDescrizione = 
											AllocaStringa(pBufLinea);
							pTempRecord->pVetValori[i].pMexHelp = 
											AllocaStringa(pMexHelp);
							pTempRecord->pVetValori[i].pMexRemark =
											AllocaStringa(pMexRemark);
							i++;
							pTempRecord->TotValori = i;
						}
					}


				}
			}
		} 
		else {
			// siamo fuori da ogni sezione, bisogna cercare inizio sezione
			LastTriggerNumber=-1;
			if (pBufLinea[0] != ';' && pBufLinea[0] != 0) {
				VetParti = Split(pBufLinea,'_',&TotParti,NULL,NULL,false);
				if (TotParti != 4 && TotParti !=5) {

					sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, start section tag is not correct in following line :\r\n%s\r\nAfter line: %s",
								pBufLinea, BufLastValidLinee);
					MostraMessageBoxNGLE(BufferLog,3);

				}else {
					if (strcmpi(VetParti[0],"<START") != 0) {
						sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, start section tag is not correct in following line :\r\n%s\r\nAfter line: %s",
									pBufLinea, BufLastValidLinee);
						MostraMessageBoxNGLE(BufferLog,3);

					}else {
						// INIZIO NUOVA SEZIONE *********************
						

						pChar = VetParti[1];
						if (TestMain ==false) {

							MyTempRecord.TotValori=0;
						}
						for (i=0;i<SEZ_TOT;i++) {
							if (strcmpi(VetSezioni[i], pChar) == 0) break;
						}
						if ( i == SEZ_TOT) {
							sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, unknown name of start section %s in line :\r\n%s\r\nAfter line: %s",
										pChar, pBufLinea, BufLastValidLinee);
							MostraMessageBoxNGLE(BufferLog,3);
						}
						if (TipoLoading == LNGC_MAIN || TipoLoading == LNGC_PLUGIN) {
							j = pBaseCostanti->TotSezioni;

							// creare nuovo record sezione
							pBaseCostanti->pVetSezioni = (StrRecordSezione*)
								realloc(pBaseCostanti->pVetSezioni, 
									sizeof(StrRecordSezione) * (j+1));

							pTempRecord = &pBaseCostanti->pVetSezioni[j];
							if (i == SEZ_TEXTS) {
								// slvare l'indice per sezione testi
								pBaseCostanti->IndiceSezTesti = j;
							}
						}else {
							// secondario
							pTempRecord = &MyTempRecord;
						}

						pTempRecord->TipoSezione = i;
						pTempRecord->ListPredefinito = 0;
						pTempRecord->pVetValori = NULL;
						pTempRecord->TotValori=0;


						// ora scoprire  numero per quella sezione
						Numero = -8000;
						sscanf(VetParti[2],"%d",&Numero);
						if (Numero == -8000) {
							sprintf(BufferLog,"SYNTAX ERROR scanning trigger list, start section tag is not correct (3th parameter) in following line :\r\n%s\r\nAfter line: %s",
										pBufLinea, BufLastValidLinee);
							MostraMessageBoxNGLE(BufferLog,3);
						}
						pTempRecord->NumeroSezione = Numero;


						pChar = VetParti[3];
	
						switch (pChar[0]) {
						case 'T':

							pTempRecord->TipoListaShow =
									SHOW_IN_TIMER;
							break;
						case 'O':
							pTempRecord->TipoListaShow =
									SHOW_IN_OBJECT;
							break;

						case 'E':

							pTempRecord->TipoListaShow = 
									SHOW_IN_EXTRA;
							break;
						case 'B':
							pTempRecord->TipoListaShow =
									SHOW_IN_BUTTONS;
							break;
						default:
							sprintf(BufferLog,"SYNTAX ERROR scanning trigger list. Start section tag is not correct, the 4th parameter in following line :\r\n%s\r\n\r\nUnknown type output list '%c', the valid values are 'O' (Object), 'T' (Timer) and 'E' (Extra)",
										pBufLinea, pChar[0]);
							MostraMessageBoxNGLE(BufferLog,3);
							free(pMemTemp);
							break;
						}
						pTempRecord->Flags = FTL_NONE;

						if (TotParti == 5) {
							pChar= VetParti[4];
							switch (pChar[0]){
							case 'H':
								// nascondere i numeri in lista
								pTempRecord->Flags |= FTL_HIDE_NUMBERS;

								break;
							case 'M':
								// mostrare numeero ma in fondo alla riga
								pTempRecord->Flags |= FTL_MOVE_NUMBERS;
								break;

							default:
								sprintf(BufferLog,"SYNTAX ERROR scanning trigger list. Start section tag is not correct, the 5th parameter in following line :\r\n%s\r\n\r\nUnknown flag character  '%c'",
											pBufLinea, pChar[0] );
								MostraMessageBoxNGLE(BufferLog,3);
								break;
							}
						}

						// vedere se questa e' una sezione di trigger principale, ossia
						// quella dove e' presente la descrizione di un flipeffect, action  o conditioon
						TestMainTrigger=false;
						if (pTempRecord->TipoSezione == SEZ_TRIGGERWHAT) {
							// potrebbe essewre flipeffect o action
							if (pTempRecord->NumeroSezione == 9) {
								// flipeffect
								TestMainTrigger=true;
								pMexTipoTrigger="FLIPEFFECT";
							}
							if (pTempRecord->NumeroSezione == 11) {
								// action
								TestMainTrigger=true;
								pMexTipoTrigger="ACTION";
							}
						}
						if (pTempRecord->TipoSezione == SEZ_TRIGGERTYPE &&
							pTempRecord->NumeroSezione == 12) {
							// e' condition
							TestMainTrigger=true;
							pMexTipoTrigger="CONDTION";
						}
						TestDentro=true;
						
					

					}
				}
			}
		}
		
	}

	free(pMemTemp);



}
// copiare 50 bytes da pSrc a pDest ricordand pero' di ridurre di 43
// l'indice iniziale
void FaiCopiaStrana(BYTE *pDest, BYTE *pSrc)
{

	BYTE Valore;
	BYTE Cambio;
	BYTE AltroBuffer[256];
	int i;

	for (i=0;i<40;i++) {
		Cambio = *pSrc;
		Valore = *pDest;
		pSrc--;
		pDest--;
		Cambio = Valore ^ 0x52;
		Valore = Cambio+1;
		Valore &= Cambio;
		AltroBuffer[i] = Valore;
	}
	// tutta la roba sopra e' solo per ridurre di 43 dove punta
	// ora si devono togliere altri 3 bytes
	pDest -= 3;
	pSrc -= 3;
	for (i=0;i<50;i++) {
		pDest[i] = pSrc[i] ^  AltroBuffer[i];
		pDest[i] ^= AltroBuffer[i];

	}

}
// deve essere chiamata immediaamente prima di caricare un progetto

// prende i nomi originali da 
// StrNomeSlot  *pVetSalvaNomiSlot=NULL;
// int TotSalvaNomiSlot;
void RipristinaNomiSlot(void)
{
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int i;
	WORD Slot;


	VetMieiNomiSlot.NomeWad[0]=0;
	VetMieiNomiSlot.TotNomiSlot=0;

	int *pTotSlot = (int*) 0x56D9F0; // TotOggettiWad

	pVetSlotOggetti = *p2VetSlotOggetti;

	if (TotSalvaNomiSlot==0) return;

	for (i=0;i<TotSalvaNomiSlot;i++) {
		Slot= pVetSalvaNomiSlot[i].Slot;

		strcpy(pVetSlotOggetti[Slot].pNome, pVetSalvaNomiSlot[i].NomeSlot);
	}


}
// effettua l'avvio di programma indice
void StartProgrammaEsterno(int Indice)
{
	char *pNomeExe;
	char LineaComandi[256];
	char *pNomeFileTga = (char*) 0x46BA78;
	HWND *pMiaWind = (HWND*) 0x46E559;
	char *pNome;
	char NomeFolder[256];

	pNomeExe = MyPrefRoom.VetExternPrograms[Indice].PathExe;
	//	char *VetMiniOpen[]={"none", "tga", "wad", "was","tr4","folder"};
	switch (MyPrefRoom.VetExternPrograms[Indice].TipoOpen) {
	case 0:
		LineaComandi[0]=0;
		break;
	case 1:
		//  TGA
		strcpy(LineaComandi,pNomeFileTga);
		break;
	case 2:
		// WAD
		sprintf(LineaComandi,"%s\\%s.wad", 
					SoloDir(NomeFileWad), 
					SoloNomeSenzaExt(NomeFileWad));
		break;
	case 3:
		// WAS
		sprintf(LineaComandi,"%s\\%s.was", 
					SoloDir(NomeFileWad), 
					SoloNomeSenzaExt(NomeFileWad));
		break;
	case 4:
		// tr4
		sprintf(LineaComandi,"%s\\data\\%s.tr4", Dir_Trle, 
					SoloNomeSenzaExt(NomeFileWad));
		break;
	case 5:
		// foldee
		LineaComandi[0]=0;
		sprintf(NomeFolder,"%s\\",pNomeExe);
		pNomeExe = NomeFolder;

		break;

	}
	if ((MyPrefRoom.Flags & FPR_NO_MINIMIZE)==0) 
			ShowWindow(*pMiaWind,SW_MINIMIZE);
	if (EsisteFile(LineaComandi)==false) LineaComandi[0]=0;

	if (MyPrefRoom.Flags & FPR_USE_SHORT_PATH) {

		if (LineaComandi[0] != 0) {
			pNome = TrovaNomeCorto(LineaComandi);
			strcpy(LineaComandi, pNome);
		}
	}else {
		// impostare nome lungo
		if (LineaComandi[0] !=0) {
			pNome = TrovaNomeLungo(LineaComandi);
			strcpy(LineaComandi,pNome);
		}
	}
	if (MyPrefRoom.VetExternPrograms[Indice].Flags & FEP_CLIPBOARD) {
		SetClipboard(LineaComandi);
		LineaComandi[0]=0;
	}
	LastTimeStandBy=0;	
	ShellExecute(NULL,"open",pNomeExe,LineaComandi,SoloDir(pNomeExe),SW_SHOW);

}

void CongelaBackupUndo(void)
{
	TestBloccoUndo=1;


}

void RipristinaBackupUndo(void)
{
	TestBloccoUndo=0;



}
// a seconda se e' attivo blocco undo (TestBloccoUndo) registra
// il nuovo backup undo o no
__declspec(naked) void PatchBloccaUndo(void)
{
	__asm {

		lea	  eax, TestBloccoUndo
		cmp  dword ptr [eax],0
		jz	EseguiUndo
		stc
		retn
EseguiUndo:


		mov eax, 4669A0h    ;IndiceUndoAttuale
		mov  eax, dword ptr [eax]
		cmp  eax, 20h
		clc
		retn


	}

}


void BreveSleep(void)
{
	Sleep(50);
}

void RimuoviMessaggiInput(void)
{
	
	
	__asm {
		pushad

		mov eax, 4357B0h    ;SvuotaBuffer
		call eax
	
		mov eax, 46E0A4h   ;StatusPulsantiMouse
		mov  dword ptr [eax], 0
	

		popad
	}
	

	InviaLog("Remove input mexages");

}

// crea processo e resituisce il threadid oppure -1 se c'e' errore
// imposta anche pHandleProcesso
int CreaProcesso(char *NomeExe, char *LineaComandi, HANDLE *pHandleProcesso, 
				bool TestNascondi)
{

	PROCESS_INFORMATION proc;
	STARTUPINFO StartInf;
	char BufLineaComandi[512];
	
	memset(&proc,0,sizeof(PROCESS_INFORMATION));
	memset(&StartInf,0,sizeof(STARTUPINFO));

	*pHandleProcesso=NULL;
	StartInf.cb = sizeof(STARTUPINFO);

	if (TestNascondi==true) {
		StartInf.dwFlags = STARTF_USESHOWWINDOW;
		StartInf.wShowWindow = SW_HIDE;
	}
	sprintf(BufLineaComandi,"%s %s", TrovaNomeCorto(NomeExe), LineaComandi);

	if (CreateProcess(TrovaNomeCorto(NomeExe), 
			BufLineaComandi,
			NULL,NULL,FALSE,0, NULL,
			SoloDir(NomeExe),&StartInf,&proc) == FALSE) return -1;
	*pHandleProcesso = proc.hProcess;
	return (int) proc.dwThreadId;

}
// visualizza una finestrella sullo schermo del room editor per avvisare
// di un'operazione in corso
// poi bisogna chiamare ChiudiFinestraWait()
void ApriFinestraWait(char *pTesto)
{
	memset(&DatiWaitWindow,0,sizeof(StrDatiFinestra));

	DatiWaitWindow.ColoreFondo=1;
	DatiWaitWindow.ColoreTesto = 0;
	DatiWaitWindow.OrgX = 0x1d5;
	DatiWaitWindow.OrgY = 0xfb;
	DatiWaitWindow.pTestoFinestra = &DatiTestoFinestra;
	DatiWaitWindow.pTestoFinestra->pTesto = pTesto;
	DatiWaitWindow.pTestoFinestra->Mistero1 =0;
	DatiWaitWindow.pTestoFinestra->Mistero2 = 0;
	DatiWaitWindow.pTestoFinestra->OffX= 6;
	DatiWaitWindow.pTestoFinestra->OffY = 0x0a;
	DatiWaitWindow.SizeX = 256;
	DatiWaitWindow.SizeY = 30;
	
	roomMostraFinestra(&DatiWaitWindow);


}

void ChiudiFinestraWait(void)
{
	roomChiudiFinestra(&DatiWaitWindow);

}

// imposta in pFloorClick e pCeilingClick le altezze in click piu' frequenti
// di stanza
// i valori infatti in struttura room non sono del tutto attendibili
// considera come valori base quelli piu' frequenti
void TrovaAltezzaMediaStanza(int IndiceRoom, int* pFloorClick, int* pCeilingClick)
{
	int VetFloor[20*20];
	int VetCeiling[20*20];
	int TotFloor, TotCeiling;
	int	i;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int SizeX,SizeY;
	int x,y;
	int NFloor,NCeiling;
	int VetTotFloor[20*20];
	int VetTotCeiling[20*20];
	int Max,Indice;

	pVetRooms = *p2VetRooms;

	DatiShape.pRoom = &pVetRooms[IndiceRoom];

	SizeX = pVetRooms[IndiceRoom].XSizeBlocks;
	SizeY = pVetRooms[IndiceRoom].ZSizeBlocks;
	TotFloor=0;
	TotCeiling=0;

	for (y=0;y<SizeY;y++) {
		for (x=0;x<SizeX;x++) {
			NFloor =pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX].WorldFloorClick;
			NCeiling=pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX].WorldYCeilingClick;

			for (i=0;i<TotFloor;i++) {
				if (NFloor == VetFloor[i]) break;
			}
			if (i == TotFloor) {
				// non c'era, aggiungerlo adesso
				VetFloor[TotFloor]= NFloor;
				VetTotFloor[TotFloor]=1;
				TotFloor++;
			}else
			{
				// era gia' presente, incrementare la sua quantita'
				VetTotFloor[i]++;
			}

			// ora stessa cosa per ceiling
			for (i=0;i<TotCeiling;i++) {
				if (NCeiling == VetCeiling[i]) break;
			}
			if (i == TotCeiling) {
				// non c'era, aggiungerlo adesso
				VetCeiling[TotCeiling]= NCeiling;
				VetTotCeiling[TotCeiling]=1;
				TotCeiling++;
			}else
			{
				// era gia' presente, incrementare la sua quantita'
				VetTotCeiling[i]++;
			}
		}
	}

	// adesso trovare il valore di floor che e' piu' frequente
	Max = 0;
	Indice=0;
	for (i=0;i<TotFloor;i++) {
		if (VetTotFloor[i] > Max) {
			Max = VetTotFloor[i];
			Indice=i;
		}
	}
	(*pFloorClick) = VetFloor[Indice];

	// ora stessa cosa per ceiling

	// adesso trovare il valore di floor che e' piu' frequente
	Max = 0;
	Indice=0;
	for (i=0;i<TotCeiling;i++) {
		if (VetTotCeiling[i] > Max) {
			Max = VetTotCeiling[i];
			Indice=i;
		}
	}
	(*pCeilingClick) = VetCeiling[Indice];


}
// converte colore da 0 a 0x3f in colore da 0 a ff
BYTE ConvertiColore64(BYTE Colore64)
{
	DWORD Valore;

	if (Colore64 == 0) return 0;
	Valore = Colore64+1;
	Valore = Valore << 2;
	Valore--;

	return (BYTE) Valore;

}
// salva in file NomeFile (.bmp) la porzione di schermo OrgX, OrgY SizeX SizeY
// di schermo room editor
// cattura screenshot di tipo TSS_
// #define TSS_PANEL2D  1
// #define TSS_PANEL3d  2
// #define TSS_PREVIEW  3
// #define TSS_PREVIEW_RGB 4
// #define TSS_INFOROOM 6
// #define TSS_LIGHTS 7
void SalvaScreenShot(char*NomeFile, int TipoScreenShot)
{
	StrBaseSchermo **p2BaseSchermo = (StrBaseSchermo**) 0x563F58;
	StrBaseSchermo *pBaseSchermo;
	BITMAPINFO *pInfoDIB;
	int OrgX,OrgY,SizeX,SizeY;
	bool TestResize;
	int DestX,DestY;
	HWND *pMiaWind = (HWND*) 0x46E559;
	FILE *pTemp;
	BITMAPFILEHEADER HeaderBmp;
	BYTE *pMiaMemoria;
	pBaseSchermo = *p2BaseSchermo;
	HDC  HdcScreen, MemHdc;
	HBITMAP MioBitMap, VecchioBmp;
	POINT Origine;
	HDC RoomHdc;
	int SizeImage;
	bool TestRgb;
	// RGBTRIPLE *pBasePalette;
	int NLinee;
	int TotColori;
	// int	i;

	TestRgb=false;
	switch (TipoScreenShot) {
	case TSS_FULLSCREEN:
		OrgX=0;
		OrgY=0;
		SizeX=1024;
		SizeY=768;
		DestX=SizeX;
		DestY=SizeY;
		break;

	case TSS_PANEL2D:
		OrgX = 0;
		OrgY = 9;
		SizeX = 280;
		SizeY = 302;
		DestX = SizeX;
		DestY = SizeY;
		break;
	case TSS_PANEL3D:
		OrgX = 293;
		OrgY = 10;
		SizeX = 512;
		SizeY = 512;
		DestX = SizeX;
		DestY = SizeY;
		break;
	case TSS_PREVIEW_RGB:
		TestRgb=true;
	case TSS_PREVIEW:

		OrgX = 112;
		OrgY = 84;
		SizeX = 700;
		SizeY = 600;
		DestX = SizeX;
		DestY = SizeY;
		break;


	case TSS_TEXTURES:
		OrgX = 816;
		OrgY = 9;
		SizeX = 208;
		SizeY = 740;
		DestX = SizeX;
		DestY = SizeY;
		TestResize=false;
		break;
	case TSS_INFOROOM:
		OrgX = 293;
		OrgY = 647;
		SizeX = 512;
		SizeY = 64;
		DestX = SizeX;
		DestY = SizeY;
		break;
	case TSS_LIGHTS:
		OrgX = 0;
		OrgY = 641;
		SizeX = 283;
		SizeY = 127;
		DestX = SizeX;
		DestY = SizeY;
		break;

	}


	RoomHdc = GetDC(*pMiaWind);

	HdcScreen = GetDC(0);
	Origine.x=0;
	Origine.y=0;
	GetDCOrgEx(RoomHdc,&Origine);
	ReleaseDC(*pMiaWind, RoomHdc);

	MioBitMap=CreateCompatibleBitmap(HdcScreen,DestX,DestY);
	MemHdc= CreateCompatibleDC(HdcScreen);

	VecchioBmp= (HBITMAP) SelectObject(MemHdc,MioBitMap);


	// -------- fine gestione palette ------
	// ora copiare nell'hdc compatibile la porzione richiesta

	BitBlt(MemHdc,0,0,SizeX,SizeY,HdcScreen,
			OrgX+Origine.x , OrgY+Origine.y ,SRCCOPY);
	
	if (TestRgb==true) 
		TotColori=0;
	else
		TotColori=256;


	pInfoDIB = (BITMAPINFO*) malloc(sizeof(BITMAPINFO) + 4 * TotColori);

	memset(pInfoDIB,0,sizeof(BITMAPINFO));


	pInfoDIB->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pInfoDIB->bmiHeader.biWidth = DestX;
	pInfoDIB->bmiHeader.biHeight = DestY;
	pInfoDIB->bmiHeader.biPlanes = 1;
	if (TestRgb==true) {
		pInfoDIB->bmiHeader.biBitCount = 24;
		pInfoDIB->bmiHeader.biCompression = BI_RGB;
		SizeImage = DestX * DestY * 3;
	}
	else {
		pInfoDIB->bmiHeader.biBitCount =8;
		pInfoDIB->bmiHeader.biClrUsed = 256;
		pInfoDIB->bmiHeader.biCompression= BI_RLE8;	
		pInfoDIB->bmiHeader.biClrUsed = 256;
		SizeImage = DestX * DestY;
		pInfoDIB->bmiHeader.biSizeImage = SizeImage;
	}

	pInfoDIB->bmiHeader.biXPelsPerMeter = 0xB12;
	pInfoDIB->bmiHeader.biYPelsPerMeter = 0xB12;

	pInfoDIB->bmiHeader.biClrImportant = 0;



	pMiaMemoria = (BYTE*) malloc(SizeImage);	
	NLinee= GetDIBits(MemHdc,MioBitMap,0,DestY,
				pMiaMemoria,pInfoDIB,DIB_RGB_COLORS);

	if (TestRgb==false)
			SizeImage = pInfoDIB->bmiHeader.biSizeImage;

	pTemp = fopen(NomeFile,"wb");
	HeaderBmp.bfType= 0x4d42;  // tipo "BM"
	HeaderBmp.bfReserved1=0;
	HeaderBmp.bfReserved2=0;
	HeaderBmp.bfSize = sizeof(BITMAPFILEHEADER) + SizeImage + 
			sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)* TotColori;
	HeaderBmp.bfOffBits= sizeof(BITMAPFILEHEADER) + 
			sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)* TotColori;

	fwrite(&HeaderBmp,sizeof(BITMAPFILEHEADER),1,pTemp);
	fwrite(pInfoDIB,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)* TotColori,1,pTemp);
	fwrite(pMiaMemoria,SizeImage,1,pTemp);
	fclose(pTemp);

	// liberare hdc e bitmap

	//ReleaseDC(*pMiaWind, HdcScreen);
	ReleaseDC(0,HdcScreen);

	SelectObject(MemHdc,VecchioBmp);
	DeleteObject(MioBitMap);
	DeleteDC(MemHdc);
	free(pInfoDIB);
	free(pMiaMemoria);

	

}

void InizializzaRecordOld(StrOldProject *pRecord)
{
	int i;

	for (i=0;i<MAX_OLD_ROOMS;i++) {
		pRecord->VetLista [i] =-1;
	}
	pRecord->IndiceRead=0;
	pRecord->IndiceWrite=0;
	pRecord->NomeProject[0]=0;

}
// effettua patch per impostare dimensione di preview
void ImpostaSizePreview(void)
{
	int *pVetValori = (int *) 0x0445499;
	RECT *pRect;

	pRect = &VetPreview[MyPrefRoom.PreviewSize].Zona;

	pVetValori[1] = pRect->left;
	pVetValori[3] = pRect->right;
	pVetValori[5] = pRect->top;
	pVetValori[7] = pRect->bottom;

}

// Creata in data: 13/02/2013 11.02.04
void Patch_perBloccoAttesa(void)
{
	int  i;
	static BYTE VetBytes[]={0x75, 0xD8};
	BYTE *pMem = (BYTE*) 0x434306;

	for (i=0;i<2;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 19/12/2013 2.38.16
// Chiamata da: 0x4039E0
// MOV AX,77                                                   ;  INIZIO PROCEDURA 23 ;  SIZE=0x3A0
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perHiDEobjdrawroom(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x77, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0};
	BYTE *pMem = (BYTE*) 0x4039E0;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Aggiunge in progetto Indice la StanzaNow nella lista delle old room
// nota: se StanzaNow e Indice = -1 allora aggiunge in automatico stanzanow
// in indicecorrente
void AddInOldRoomList(int StanzaNow, int Indice) 
{
	int i;

	if (StanzaNow == -1) {
		// impostare dati in modo autonomo
		StanzaNow = GetIndiceRoom(-1);
		Indice = GetIndiceProgettoNow();
	}

	// se e' gia' presente uscire e basta
	for (i=0;i<MAX_OLD_ROOMS;i++) {
		if (MyPrefRoom.VetOldProjects[Indice].VetLista[i] == StanzaNow) return;
	}

	for (i=0;i<MAX_OLD_ROOMS;i++) {
		if (MyPrefRoom.VetOldProjects[Indice].VetLista[i] == -1) break;
	}

	if (i < MAX_OLD_ROOMS) {
		// c'e' un buco libero, trovarlo
		i = MyPrefRoom.VetOldProjects[Indice].IndiceWrite;
		while (MyPrefRoom.VetOldProjects[Indice].VetLista[i] != -1) {
			i++;
			if (i >= MAX_OLD_ROOMS) i=0;
		}
	}else
	{
		// tutti occupati, registrare semplicemente in prossimo indicewrite
		i = MyPrefRoom.VetOldProjects[Indice].IndiceWrite;
		i++;
		if (i >= MAX_OLD_ROOMS) i=0;
	}
	MyPrefRoom.VetOldProjects[Indice].IndiceWrite = i;
	MyPrefRoom.VetOldProjects[Indice].IndiceRead  = i;
	MyPrefRoom.VetOldProjects[Indice].VetLista[i] = StanzaNow;

	sprintf(BufferLog,"Added current room %d in circular list in position %d",
		StanzaNow,i);
	InviaLog(BufferLog);
	ShowInfoRoom(BufferLog);


}

// alza o abbassa floor o ceiling usando funzioni specifiche del room editor
// l'area interessata all'effetto e' quella di DatiShape.MiniSel
void PulsantiFloor(bool TestFloor, bool TestPiu, 
					bool TestSmooth, int FrecciaDirezione, int TotClick)
{
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int *pTipoFrecciaSelezione = (int *) 0x583BB6;
	int *pIndiceOggettoSel = (int*) 0x583BA4;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	int Operazione;
	HCURSOR *pHandleIconaMouse = (HCURSOR*) 0x493110;

	// impostare selezione nel formato del room edit
	pRectSelezione2d->left = DatiShape.MiniSel.left  + DatiShape.pRoom->XDisplayOffset;
	pRectSelezione2d->top = DatiShape.MiniSel.top  + DatiShape.pRoom->ZDisaplyOffset;
	pRectSelezione2d->right = DatiShape.MiniSel.right;
	pRectSelezione2d->bottom= DatiShape.MiniSel.bottom;
	*pTestEsisteSelezioneBox=1;
	*pTipoFrecciaSelezione = FrecciaDirezione;
	*pIndiceOggettoSel = -1;




	// stabilire operazione
	if (TestFloor==true) {
		if (TestPiu==true) 
			Operazione = AFI_FLOOR_PIU;
		else
			Operazione = AFI_FLOOR_MENO;
	}else {
		if (TestPiu==true) 
			Operazione = AFI_CEILING_PIU;
		else
			Operazione = AFI_CEILING_MENO;
	}
	// prima chiamare sempre funzione altera floor
	roomAlteraFloorInput(*pIndiceRoomAttuale, 
			pRectSelezione2d->left ,pRectSelezione2d->top,
			pRectSelezione2d->right,pRectSelezione2d->bottom, Operazione, TotClick);

	// ora se e' attivo smooth chiamare anche funzione room
	if (TestSmooth==true) {
		roomSmoothFloor(*pIndiceRoomAttuale, 
			pRectSelezione2d->left ,pRectSelezione2d->top,
			pRectSelezione2d->right,pRectSelezione2d->bottom, Operazione, TotClick);
	}
	*pHandleIconaMouse = CursoreClessidra;
	SetCursor(CursoreClessidra);

}
// viene chiamata ad ogni ciclo
// effettua controllo per room attuale
// verifica se e' uguale alla precendete se se non lo e' aggiorna i dati old
// e copia l'indice nel buffer delle precedenti posizioni
// confronta con precedneti: OldIndiceRoom e OldIndiceProject

void ControllaOldRoom(BYTE *pRoomNow, BYTE *pTabGrid, int SizeX, int SizeY)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int Indice;
	int StanzaNow;
	int		i;
	bool TestPresente;
	bool TestCambiata;
	DWORD SizeNow;
	DWORD *pTestPreview = (DWORD*) 0x583BF8;
	DWORD TimeNow;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;


	pVetRooms = *p2VetRooms;
	if ((*pTestPreview) != 0) return;

	TimeNow = GetTickCount() - OldLastTime;

	if (TimeNow <200) return;
	
	Indice = GetIndiceProgettoNow();
	StanzaNow = GetIndiceRoom(-1);

	// vedere se era stato imposta [hide object] e si e' cambiata stanza
	if (TestRemoveItems) {
		if (Indice== OldIndiceProject && StanzaNow != OldIndiceRoom) {
			// ok e' cambiata stanza
			// ripristinare numero originale
			pVetRooms[SalvaIndiceRoom].N_ObjAndTriggers=OldNumeroObjects;
			pVetRooms[SalvaIndiceRoom].N_Effects=OldEffects;
			// azzerare pulsante
			TestRemoveItems = 0;
			ModificaPulsante(&DatiPulsanteRemoveItems, 1, 0, 0);			
		}
	}

	if (pVetRooms[StanzaNow].TestEmptySlot != 0) return;

#ifdef LOG_OLDPROJECT
	sprintf(BufferLog,"Check change for room %d",StanzaNow);
	InviaLog(BufferLog);
#endif
	// vedere se e' gia' presente
	TestPresente=false;
	for (i=0;i<MAX_OLD_ROOMS;i++) {
		if (MyPrefRoom.VetOldProjects[Indice].VetLista[i] == StanzaNow) {
			MyPrefRoom.VetOldProjects[Indice].IndiceWrite = i;
			MyPrefRoom.VetOldProjects[Indice].IndiceRead = i;
			TestPresente=true;
			break;
		}
	}

	SizeNow = SizeX * SizeY * 0x8c;
	TestCambiata=false;

	if (TestPresente==false && StanzaNow == OldIndiceRoom  && Indice == OldIndiceProject) {
		TestCambiata=true;
		// finora tutto uguale
		// ora confrontare struttura room
		if (memcmp(pRoomNow,&OldSalvaRoom,0x1d1) ==0 && 
			memcmp(pTabGrid,&OldGridRoom,SizeNow) ==0  && 
			SizeNow == OldSizeGrid) TestCambiata=false;									
		
	}
#ifdef 	LOG_OLDPROJECT
	sprintf(BufferLog,"\tPresent=%d  Changed=%d TestLoadProject=%d  OldRoom=%d  ProjectNow=%d  OldProject=%d",
		TestPresente, TestCambiata,TestLoadProject, OldIndiceRoom,Indice, OldIndiceProject);
	InviaLog(BufferLog);
#endif

	if (TestPresente==false && StanzaNow==OldIndiceRoom && Indice == OldIndiceProject && 
			TestCambiata==true) {
		// aggiungere in lista questa stanza
		// vedere se c'e' almeno un posto libero
		if (TestLoadProject == false) {
			AddInOldRoomList(StanzaNow,Indice);
		}else
		{
			// se e' appena stato caricato un progetto non aggiungerla
			// ma inizializzarla la zona dati
			TestCambiata=true;
			TestPresente=false;
			TestLoadProject=false;
		}

	}


	if (TestCambiata==true || StanzaNow!= OldIndiceRoom || Indice !=OldIndiceProject) {
		// salvare i dati attuali
		OldSizeGrid = SizeNow;
		OldIndiceRoom = StanzaNow;
		OldIndiceProject=Indice;
		if (TestPresente == false) {
			memcpy(&OldSalvaRoom, pRoomNow,0x1d1);
			memcpy(&OldGridRoom, pTabGrid, SizeNow);
		}
	}
		
	OldLastTime=GetTickCount();

}

// controlla se uno degli scancode o entrambi sono attualmente premuti
// se non si vuole effettuare i lcontrollo anche del secondo scancode settarlo a -1
bool TastoPremutoNGLE(int ScanCode1, int ScanCode2)
{
	BYTE * pVetInputKeyboard = (BYTE *) 0x46DF98;

	if (pVetInputKeyboard[ScanCode1] != 0 ) {
		if (ScanCode2 == -1) return true;
		if (pVetInputKeyboard[ScanCode2] != 0) return true;
	}
	return false;


}



void SpostaPulsantiniRoom(StrDatiPulsante* pPulsante)
{
	// Il primo e' NL, poi viene D e infine Water

	// Pulsante NL
	pPulsante->Flags=0x0431;
	pPulsante->OrgX=162;
	pPulsante->OrgY=326;


	pPulsante++;

	// Pulsante D
	pPulsante->Flags=0x0431;
	pPulsante->OrgX=170;
	pPulsante->OrgY=310;
	pPulsante->SizeX=24;
	pPulsante->SizeY=16;
	pPulsante->ColoreFondo=1;	

	pPulsante++;

	// Pulsante Water
	pPulsante->Flags=0x431;
	pPulsante->OrgX=146;
	pPulsante->OrgY=294;
	pPulsante->SizeX=48;
	pPulsante->SizeY=16;
	pPulsante->ColoreFondo=1;


}



void SpostaPulsantiClimb(StrDatiPulsante* pPulsante)
{

	// modificare 6 pulsanti
	int i;

	for (i=0;i<6;i++) {
		pPulsante[i].OrgX += 3;
		pPulsante[i].OrgY += 12;
	}
}
// spostare e ridimensionare i 6 pulsanti room
void SpostaPulsantiRoom(StrDatiPulsante* pPulsante)
{
	const int OrgX=1;
	const int OrgY=332;
	const int SizeX=52;
	const int SizeY=16;

	int x,y;
	int Indice;


	for (y=0;y<3;y++) {
		for (x=0;x<2;x++) {
			Indice= x + y *2;
			pPulsante[Indice].OrgX = OrgX + x * SizeX;
			pPulsante[Indice].OrgY = OrgY + y * SizeY;
			pPulsante[Indice].SizeX = SizeX;
			pPulsante[Indice].SizeY = SizeY;
		}
	}
	PosizioneLastRoom.left = OrgX;
	PosizioneLastRoom.top = OrgY + 3 * SizeY;
	PosizioneLastRoom.right = SizeX;
	PosizioneLastRoom.bottom = SizeY;

}

// usata da room editor: sulla base del nome del file wad attuale
// legge il file .sfx per sapere quali suoni sono effettivamente
// presenti
// salva il contenuto in variabile global soundsfx e restituisce anche il numero
// di suoni usato sulla base di versione wad
// nome file wad: NomeFileWad
void LeggeTabellaSuoniSfx(void)
{
	
	FILE *pFile;
	int NVersione;
	char *pNomeSfx;

	GlobNgle.SoundVetSfx.TotSfx=0;

	if (EsisteFile(NomeFileWad)==false) return;

	pFile = fopen(NomeFileWad,"rb");
	if (pFile==NULL) return;

	fread(&NVersione, 1,4, pFile);
	fclose(pFile);

	if (NVersione == 129) 
		GlobNgle.SoundVetSfx.TotSfx = 370;
	else
		GlobNgle.SoundVetSfx.TotSfx = 2048;

	// ora trovare file .sfx
	pNomeSfx = CambiaEstensione(NomeFileWad,".sfx");
	
	if (EsisteFile(pNomeSfx)==false) {
		sprintf(BufferLog, "WARNING: I cann't find the file:  %s\r\n\r\nNGLE uses this file to discover what sound samples are presents or less in your wad files",
			pNomeSfx);
		MostraMessageBoxNGLE(BufferLog,3);
		GlobNgle.SoundVetSfx.TotSfx  =0;
		return; 
	}

	pFile = fopen(pNomeSfx, "rb");
	fread(GlobNgle.SoundVetSfx.VetSfx, 2, GlobNgle.SoundVetSfx.TotSfx,pFile);
	fclose(pFile);



}

// per room editor
void PatchZonaMemoriaVertexSecondario(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pZonaMemoriaVertexSecondario;

	StrPatch VetPatch[] = {{0x401221, true},
		{0x40143A, true},
		{0x4019B5, true}};

	OffsetOldInizio = 0x5A51E0;
	OffsetOldFine=0x5F7260;
	TotPatchs=3;
	// size originale = 0x52080
	pZonaMemoriaVertexSecondario = (DWORD *) 0x454616;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
				OffsetOldFine, 0x148200,  
				"ZonaMemoriaVertexSecondario");
	*pZonaMemoriaVertexSecondario = OffsetNew;
}
// room editor
void PatchZonaMemoriaVertex(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pZonaMemoriaVertex;

	StrPatch VetPatch[] = {{0x40121C, true},
		{0x401444, true},
		{0x4019BF, true},
		{0x402EF8, true},
		{0x402F0D, true},
		{0x402F1F, true},
		{0x402F3F, true},
		{0x403005, true},
		{0x403012, true},
		{0x40301B, true},
		{0x403039, true},
		{0x403040, true},
		{0x403047, true},
		{0x403050, true},
		{0x403059, true},
		{0x403063, true},
		{0x40306C, true},
		{0x4030AE, true},
		{0x4030C0, true},
		{0x4030C6, true},
		{0x4030D1, true},
		{0x4030E6, true},
		{0x4030F5, true},
		{0x403104, true},
		{0x40344B, true},
		{0x403459, true},
		{0x403478, true},
		{0x403653, true},
		{0x403684, true},
		{0x40368B, true},
		{0x403692, true},
		{0x403699, true},
		{0x4036AB, true},
		{0x4036B8, true},
		{0x403707, true},
		{0x403710, true},
		{0x40371F, true},
		{0x40373A, true},
		{0x403747, true},
		{0x403762, true},
		{0x418AB6, true},
		{0x418AFB, true},
		{0x418B8D, true},
		{0x418CDA, true},
		{0x41E2A5, true},
		{0x41E2AF, true},
		{0x41E2BF, true},
		{0x41E2C9, true},
		{0x425237, true},
		{0x4253A3, true},
		{0x4253A8, true},
		{0x437E56, true},
		{0x437E5E, true},
		{0x437E6C, true},
		{0x437E73, true},
		{0x437E81, true},
		{0x437E8A, true},
		{0x437EA8, true},
		{0x437EAE, true},
		{0x437EBC, true},
		{0x437ED7, true},
		{0x437EDD, true},
		{0x437EEF, true},
		{0x437F41, true},
		{0x437F52, true},
		{0x437F63, true},
		{0x437F6E, true},
		{0x437F84, true},
		{0x437F95, true},
		{0x438026, true},
		{0x438039, true},
		{0x438050, true},
		{0x43806C, true},
		{0x438080, true},
		{0x438087, true},
		{0x43808E, true},
		{0x438095, true},
		{0x4380A3, true},
		{0x4380AC, true},
		{0x4380D2, true},
		{0x4380D8, true},
		{0x4380DE, true},
		{0x438103, true},
		{0x438109, true},
		{0x43811B, true},
		{0x438170, true},
		{0x438181, true},
		{0x438192, true},
		{0x4381A2, true},
		{0x4381B3, true},
		{0x4381C4, true},
		{0x438217, true},
		{0x438229, true},
		{0x43822F, true},
		{0x438251, true},
		{0x438260, true},
		{0x438381, true},
		{0x438389, true},
		{0x43839B, true},
		{0x4383A8, true},
		{0x4383AF, true},
		{0x4383BF, true},
		{0x4383E9, true},
		{0x438400, true},
		{0x43840B, true},
		{0x43841A, true},
		{0x43842A, true},
		{0x438430, true},
		{0x438446, true},
		{0x438468, true},
		{0x4384BB, true},
		{0x4384CC, true},
		{0x4384DD, true},
		{0x4384EE, true},
		{0x438503, true},
		{0x438514, true},
		{0x438525, true},
		{0x438536, true},
		{0x4385E0, true},
		{0x4385F1, true},
		{0x43861B, true},
		{0x438627, true},
		{0x438647, true},
		{0x438661, true},
		{0x438668, true},
		{0x43866F, true},
		{0x438678, true},
		{0x438681, true},
		{0x43868B, true},
		{0x438692, true},
		{0x43869B, true},
		{0x4386B7, true},
		{0x4386BF, true},
		{0x4386D1, true},
		{0x4386E0, true},
		{0x4386F0, true},
		{0x438706, true},
		{0x438719, true},
		{0x43876B, true},
		{0x43877C, true},
		{0x43878D, true},
		{0x43879E, true},
		{0x4387B3, true},
		{0x4387C4, true},
		{0x4387D5, true},
		{0x4387E0, true},
		{0x438839, true},
		{0x43884B, true},
		{0x438855, true},
		{0x438860, true},
		{0x438875, true},
		{0x438884, true},
		{0x438897, true},
		{0x4389B6, true},
		{0x4389BE, true},
		{0x4389CC, true},
		{0x4389D3, true},
		{0x4389E1, true},
		{0x4389EA, true},
		{0x438A00, true},
		{0x438A07, true},
		{0x438A10, true},
		{0x438A43, true},
		{0x438A49, true},
		{0x438A57, true},
		{0x438A72, true},
		{0x438A78, true},
		{0x438A8A, true},
		{0x438ADC, true},
		{0x438AED, true},
		{0x438AFE, true},
		{0x438B09, true},
		{0x438B1F, true},
		{0x438B30, true},
		{0x438BC6, true},
		{0x438BD9, true},
		{0x438BF0, true},
		{0x438C0C, true},
		{0x438C20, true},
		{0x438C27, true},
		{0x438C2E, true},
		{0x438C35, true},
		{0x438C43, true},
		{0x438C4C, true},
		{0x438C6A, true},
		{0x438C71, true},
		{0x438C78, true},
		{0x438CAD, true},
		{0x438CB3, true},
		{0x438CB9, true},
		{0x438CDE, true},
		{0x438CE4, true},
		{0x438CF6, true},
		{0x438D47, true},
		{0x438D58, true},
		{0x438D69, true},
		{0x438D79, true},
		{0x438D8A, true},
		{0x438D9B, true},
		{0x438DEE, true},
		{0x438DF5, true},
		{0x438DFC, true},
		{0x438E0B, true},
		{0x438E3F, true},
		{0x438E45, true},
		{0x438E67, true},
		{0x438E76, true},
		{0x438F91, true},
		{0x438F99, true},
		{0x438FAB, true},
		{0x438FB8, true},
		{0x438FBF, true},
		{0x438FCF, true},
		{0x438FF0, true},
		{0x439009, true},
		{0x439012, true},
		{0x43901B, true},
		{0x439046, true},
		{0x43904C, true},
		{0x43905A, true},
		{0x439065, true},
		{0x439075, true},
		{0x43907B, true},
		{0x43908D, true},
		{0x43909C, true},
		{0x4390EF, true},
		{0x439100, true},
		{0x439111, true},
		{0x439122, true},
		{0x439137, true},
		{0x439148, true},
		{0x439159, true},
		{0x43916A, true},
		{0x439207, true},
		{0x439223, true},
		{0x43924C, true},
		{0x439258, true},
		{0x439278, true},
		{0x439292, true},
		{0x439299, true},
		{0x4392A0, true},
		{0x4392A7, true},
		{0x4392B5, true},
		{0x4392BE, true},
		{0x4392DC, true},
		{0x4392E3, true},
		{0x4392EA, true},
		{0x4392F9, true},
		{0x43931F, true},
		{0x43932A, true},
		{0x439337, true},
		{0x439342, true},
		{0x43935B, true},
		{0x43936A, true},
		{0x4393C0, true},
		{0x4393D1, true},
		{0x4393E2, true},
		{0x4393F3, true},
		{0x439404, true},
		{0x439415, true},
		{0x439426, true},
		{0x439431, true},
		{0x43948B, true},
		{0x439492, true},
		{0x439499, true},
		{0x4394A1, true},
		{0x4394B1, true},
		{0x4394DC, true},
		{0x4394E2, true},
		{0x4394ED, true},
		{0x439502, true},
		{0x439511, true},
		{0x439520, true},
		{0x439EE9, true},
		{0x43A0D7, true},
		{0x43A100, true},
		{0x43A2B9, true},
		{0x43A710, true},
		{0x43A718, true},
		{0x43A72A, true},
		{0x43A731, true},
		{0x43A73F, true},
		{0x43A748, true},
		{0x43A766, true},
		{0x43A76C, true},
		{0x43A77A, true},
		{0x43A795, true},
		{0x43A79B, true},
		{0x43A7AD, true},
		{0x43A7FF, true},
		{0x43A810, true},
		{0x43A821, true},
		{0x43A832, true},
		{0x43A843, true},
		{0x43A84E, true},
		{0x43A864, true},
		{0x43A875, true},
		{0x43A886, true},
		{0x43A956, true},
		{0x43A969, true},
		{0x43A980, true},
		{0x43A99C, true},
		{0x43A9B0, true},
		{0x43A9B7, true},
		{0x43A9BE, true},
		{0x43A9C5, true},
		{0x43A9D3, true},
		{0x43A9DC, true},
		{0x43AA02, true},
		{0x43AA08, true},
		{0x43AA0E, true},
		{0x43AA33, true},
		{0x43AA39, true},
		{0x43AA4B, true},
		{0x43AAA0, true},
		{0x43AAB1, true},
		{0x43AAC2, true},
		{0x43AAD2, true},
		{0x43AAE3, true},
		{0x43AAF4, true},
		{0x43AB04, true},
		{0x43AB15, true},
		{0x43AB26, true},
		{0x43ABA9, true},
		{0x43ABBB, true},
		{0x43ABC1, true},
		{0x43ABE3, true},
		{0x43ABF2, true},
		{0x43AD18, true},
		{0x43AD20, true},
		{0x43AD33, true},
		{0x43AD3A, true},
		{0x43AD48, true},
		{0x43AD51, true},
		{0x43AD74, true},
		{0x43AD8B, true},
		{0x43AD96, true},
		{0x43ADA1, true},
		{0x43ADB1, true},
		{0x43ADB7, true},
		{0x43ADC9, true},
		{0x43ADEB, true},
		{0x43AE3C, true},
		{0x43AE4D, true},
		{0x43AE5D, true},
		{0x43AE6D, true},
		{0x43AE7D, true},
		{0x43AE8E, true},
		{0x43AE9F, true},
		{0x43AEAF, true},
		{0x43AEC0, true},
		{0x43AED1, true},
		{0x43AEE1, true},
		{0x43AEF2, true},
		{0x43AFC6, true},
		{0x43AFE3, true},
		{0x43B00C, true},
		{0x43B018, true},
		{0x43B038, true},
		{0x43B052, true},
		{0x43B059, true},
		{0x43B060, true},
		{0x43B067, true},
		{0x43B075, true},
		{0x43B07E, true},
		{0x43B09B, true},
		{0x43B0AB, true},
		{0x43B0B3, true},
		{0x43B0C7, true},
		{0x43B0E0, true},
		{0x43B0F3, true},
		{0x43B149, true},
		{0x43B15A, true},
		{0x43B16B, true},
		{0x43B17C, true},
		{0x43B18D, true},
		{0x43B19E, true},
		{0x43B1AF, true},
		{0x43B1C0, true},
		{0x43B1D1, true},
		{0x43B1E2, true},
		{0x43B1F3, true},
		{0x43B1FE, true},
		{0x43B295, true},
		{0x43B2A7, true},
		{0x43B2B1, true},
		{0x43B2BC, true},
		{0x43B2D1, true},
		{0x43B2E0, true},
		{0x43B2F3, true},
		{0x43B3F3, true},
		{0x43B404, true},
		{0x43B42E, true},
		{0x43B43E, true},
		{0x43B47A, true},
		{0x43B480, true},
		{0x43B486, true},
		{0x43B48C, true},
		{0x43B497, true},
		{0x43B49E, true},
		{0x43B4C4, true},
		{0x43B4D5, true},
		{0x43B767, true},
		{0x43B76D, true},
		{0x43B7B1, true},
		{0x43B7C2, true},
		{0x43B7D4, true},
		{0x43B7E5, true},
		{0x43B867, true},
		{0x43B86E, true},
		{0x43B880, true},
		{0x43B895, true},
		{0x43B89B, true},
		{0x43B8DF, true},
		{0x43B8F0, true},
		{0x43B902, true},
		{0x43B913, true},
		{0x43B95D, true},
		{0x43B963, true},
		{0x43B999, true},
		{0x43B9A1, true},
		{0x43B9A7, true},
		{0x43B9AF, true},
		{0x43B9B9, true},
		{0x43B9C3, true},
		{0x43BA8C, true},
		{0x43BA9D, true},
		{0x43BB12, true},
		{0x43BB19, true},
		{0x43BB20, true},
		{0x43BB39, true},
		{0x43BB40, true},
		{0x43BB47, true},
		{0x43BB6D, true},
		{0x43BB74, true},
		{0x43BB7B, true},
		{0x43CF57, true},
		{0x43CF5E, true},
		{0x43CF77, true},
		{0x43CF7E, true},
		{0x43CF89, true},
		{0x43CF92, true},
		{0x43CFF0, true},
		{0x43CFFB, true},
		{0x43D006, true},
		{0x43D045, true},
		{0x43D04B, true},
		{0x43D076, true},
		{0x43D0DC, true},
		{0x43D0ED, true},
		{0x43D11B, true},
		{0x43D12C, true},
		{0x43D154, true},
		{0x43D165, true},
		{0x43D264, true},
		{0x43D27A, true},
		{0x43D28E, true},
		{0x43D406, true},
		{0x43D41B, true},
		{0x43D443, true},
		{0x43D44A, true},
		{0x43D451, true},
		{0x43D458, true},
		{0x43D46A, true},
		{0x43D477, true},
		{0x43D4A1, true},
		{0x43D4A7, true},
		{0x43D4B9, true},
		{0x43D4D4, true},
		{0x43D4DA, true},
		{0x43D4F8, true},
		{0x43D566, true},
		{0x43D57A, true},
		{0x43D5AD, true},
		{0x43D5B8, true},
		{0x43D5EC, true},
		{0x43D5F7, true},
		{0x43D67C, true},
		{0x43D682, true},
		{0x43D694, true},
		{0x43D6B9, true},
		{0x43D6BF, true},
		{0x43D6D5, true},
		{0x43D8DC, true},
		{0x43D8E3, true},
		{0x43D8F3, true},
		{0x43D8FA, true},
		{0x43D90A, true},
		{0x43D911, true},
		{0x43D97F, true},
		{0x43D98C, true},
		{0x43D999, true},
		{0x43D9A4, true},
		{0x43D9D1, true},
		{0x43D9DB, true},
		{0x43DA00, true},
		{0x43DA20, true},
		{0x43DA7F, true},
		{0x43DA8A, true},
		{0x43DABE, true},
		{0x43DACF, true},
		{0x43DAF8, true},
		{0x43DB09, true},
		{0x43DB36, true},
		{0x43DB47, true},
		{0x43DC2B, true},
		{0x43DC32, true},
		{0x43DC64, true},
		{0x43DC70, true},
		{0x43DD1F, true},
		{0x43DD28, true},
		{0x43DD35, true},
		{0x43DD55, true},
		{0x43DD5C, true},
		{0x43DD63, true},
		{0x43DD6A, true},
		{0x43DD78, true},
		{0x43DD81, true},
		{0x43DDA6, true},
		{0x43DDB6, true},
		{0x43DDC3, true},
		{0x43DDD2, true},
		{0x43DDF7, true},
		{0x43DE0E, true},
		{0x43DE7E, true},
		{0x43DE8F, true},
		{0x43DEBD, true},
		{0x43DEC8, true},
		{0x43DEF8, true},
		{0x43DF09, true},
		{0x43DF3A, true},
		{0x43DF4B, true},
		{0x43DFC6, true},
		{0x43DFD8, true},
		{0x43DFE2, true},
		{0x43DFED, true},
		{0x43E002, true},
		{0x43E011, true},
		{0x43E024, true},
		{0x43E15E, true},
		{0x43E16F, true},
		{0x43E197, true},
		{0x43E1A9, true},
		{0x43E1FA, true},
		{0x43E200, true},
		{0x43E206, true},
		{0x43E20C, true},
		{0x43E22D, true},
		{0x43E23E, true},
		{0x43E747, true},
		{0x43E74E, true},
		{0x43E767, true},
		{0x43E76E, true},
		{0x43E779, true},
		{0x43E782, true},
		{0x43E7E0, true},
		{0x43E7EB, true},
		{0x43E7F6, true},
		{0x43E835, true},
		{0x43E83B, true},
		{0x43E866, true},
		{0x43E8CC, true},
		{0x43E8DC, true},
		{0x43E8ED, true},
		{0x43E91B, true},
		{0x43E92C, true},
		{0x43E93C, true},
		{0x43E966, true},
		{0x43E977, true},
		{0x43E987, true},
		{0x43EA7F, true},
		{0x43EA8A, true},
		{0x43EAAB, true},
		{0x43ECA0, true},
		{0x43ECAD, true},
		{0x43ECCD, true},
		{0x43ECD4, true},
		{0x43ECDB, true},
		{0x43ECE2, true},
		{0x43ECF0, true},
		{0x43ECF9, true},
		{0x43ED1F, true},
		{0x43ED25, true},
		{0x43ED33, true},
		{0x43ED4E, true},
		{0x43ED54, true},
		{0x43ED6E, true},
		{0x43EDC7, true},
		{0x43EDD8, true},
		{0x43EDE9, true},
		{0x43EE16, true},
		{0x43EE27, true},
		{0x43EE38, true},
		{0x43EE65, true},
		{0x43EE76, true},
		{0x43EE87, true},
		{0x43EEA9, true},
		{0x43EEB0, true},
		{0x43EEC0, true},
		{0x43EED8, true},
		{0x43EEDF, true},
		{0x43EEEF, true},
		{0x43EF07, true},
		{0x43EF0E, true},
		{0x43EF1E, true},
		{0x43EF8A, true},
		{0x43EF9C, true},
		{0x43EFA2, true},
		{0x43EFC4, true},
		{0x43EFD3, true},
		{0x43F1DC, true},
		{0x43F1E3, true},
		{0x43F1F3, true},
		{0x43F1FA, true},
		{0x43F20A, true},
		{0x43F211, true},
		{0x43F263, true},
		{0x43F28C, true},
		{0x43F299, true},
		{0x43F2A4, true},
		{0x43F2D3, true},
		{0x43F2DD, true},
		{0x43F302, true},
		{0x43F322, true},
		{0x43F382, true},
		{0x43F393, true},
		{0x43F3A4, true},
		{0x43F3D2, true},
		{0x43F3E3, true},
		{0x43F3F4, true},
		{0x43F41E, true},
		{0x43F42F, true},
		{0x43F440, true},
		{0x43F46D, true},
		{0x43F47E, true},
		{0x43F48F, true},
		{0x43F4B3, true},
		{0x43F4BA, true},
		{0x43F4C0, true},
		{0x43F4E3, true},
		{0x43F4EA, true},
		{0x43F4FD, true},
		{0x43F513, true},
		{0x43F51A, true},
		{0x43F520, true},
		{0x43F542, true},
		{0x43F549, true},
		{0x43F54F, true},
		{0x43F614, true},
		{0x43F631, true},
		{0x43F665, true},
		{0x43F671, true},
		{0x43F71D, true},
		{0x43F753, true},
		{0x43F75A, true},
		{0x43F761, true},
		{0x43F768, true},
		{0x43F776, true},
		{0x43F77F, true},
		{0x43F79C, true},
		{0x43F7AD, true},
		{0x43F7BA, true},
		{0x43F7C5, true},
		{0x43F7EA, true},
		{0x43F7FD, true},
		{0x43F859, true},
		{0x43F86A, true},
		{0x43F896, true},
		{0x43F8A7, true},
		{0x43F8D3, true},
		{0x43F8E4, true},
		{0x43F913, true},
		{0x43F924, true},
		{0x43F94D, true},
		{0x43F954, true},
		{0x43F965, true},
		{0x43F97E, true},
		{0x43F985, true},
		{0x43F996, true},
		{0x43F9B1, true},
		{0x43F9B8, true},
		{0x43F9C9, true},
		{0x43F9E2, true},
		{0x43F9E9, true},
		{0x43F9FA, true},
		{0x43FA77, true},
		{0x43FA89, true},
		{0x43FA8F, true},
		{0x43FA95, true},
		{0x43FAAF, true},
		{0x43FABE, true},
		{0x43FACD, true},
		{0x43FC0B, true},
		{0x43FC12, true},
		{0x43FC55, true},
		{0x43FC65, true},
		{0x43FC70, true},
		{0x43FC77, true},
		{0x43FC7D, true},
		{0x43FCC2, true},
		{0x43FCC8, true},
		{0x43FCCF, true},
		{0x43FCD5, true},
		{0x43FCDB, true},
		{0x43FCE2, true},
		{0x43FD0C, true},
		{0x43FD1A, true},
		{0x43FD2E, true},
		{0x43FD35, true},
		{0x43FD4C, true},
		{0x4400C6, true},
		{0x4400CE, true},
		{0x4400DC, true},
		{0x4400E3, true},
		{0x4400F1, true},
		{0x4400FA, true},
		{0x440118, true},
		{0x44011E, true},
		{0x44012C, true},
		{0x440147, true},
		{0x44014D, true},
		{0x44015F, true},
		{0x4401B1, true},
		{0x4401C2, true},
		{0x4401D3, true},
		{0x4401DE, true},
		{0x4401F4, true},
		{0x440205, true},
		{0x440296, true},
		{0x4402A9, true},
		{0x4402C0, true},
		{0x4402DC, true},
		{0x4402F0, true},
		{0x4402F7, true},
		{0x4402FE, true},
		{0x440305, true},
		{0x440313, true},
		{0x44031C, true},
		{0x440342, true},
		{0x440348, true},
		{0x44034E, true},
		{0x440373, true},
		{0x440379, true},
		{0x44038B, true},
		{0x4403E0, true},
		{0x4403F1, true},
		{0x440402, true},
		{0x440412, true},
		{0x440423, true},
		{0x440434, true},
		{0x440487, true},
		{0x440499, true},
		{0x44049F, true},
		{0x4404C1, true},
		{0x4404D0, true},
		{0x4405F1, true},
		{0x4405F9, true},
		{0x44060B, true},
		{0x440618, true},
		{0x44061F, true},
		{0x44062F, true},
		{0x440659, true},
		{0x440670, true},
		{0x44067B, true},
		{0x44068A, true},
		{0x44069A, true},
		{0x4406A0, true},
		{0x4406B6, true},
		{0x4406D8, true},
		{0x44072B, true},
		{0x44073C, true},
		{0x44074D, true},
		{0x44075E, true},
		{0x440773, true},
		{0x440784, true},
		{0x440795, true},
		{0x4407A6, true},
		{0x440850, true},
		{0x440861, true},
		{0x44088B, true},
		{0x440897, true},
		{0x4408B7, true},
		{0x4408D1, true},
		{0x4408D8, true},
		{0x4408DF, true},
		{0x4408E8, true},
		{0x4408F1, true},
		{0x4408FB, true},
		{0x440902, true},
		{0x44090B, true},
		{0x440927, true},
		{0x44092F, true},
		{0x440941, true},
		{0x440950, true},
		{0x440960, true},
		{0x440976, true},
		{0x440989, true},
		{0x4409DB, true},
		{0x4409EC, true},
		{0x4409FD, true},
		{0x440A0E, true},
		{0x440A23, true},
		{0x440A34, true},
		{0x440A45, true},
		{0x440A50, true},
		{0x440AA9, true},
		{0x440ABB, true},
		{0x440AC5, true},
		{0x440AD0, true},
		{0x440AE5, true},
		{0x440AF4, true},
		{0x440B07, true},
		{0x440BFF, true},
		{0x440C08, true},
		{0x440C39, true},
		{0x440C4B, true},
		{0x440C72, true},
		{0x440C78, true},
		{0x440C7E, true},
		{0x440C84, true},
		{0x440CA1, true},
		{0x440CB2, true},
		{0x441192, true},
		{0x4411B1, true},
		{0x4411CF, true},
		{0x4411D8, true},
		{0x44126A, true},
		{0x4412D1, true}};

	OffsetOldInizio = 0x509D80;
	OffsetOldFine=0x55BE00;
	TotPatchs=828;
	pZonaMemoriaVertex = (DWORD *) 0x454612;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio, 
				OffsetOldFine, 0x148200,"ZonaMemoriaVertex");
	*pZonaMemoriaVertex = OffsetNew;
}

char *FormattaTipoOpen(StrRecordExternProgram *pRecord, bool TestCorto, bool TestLista)
{
	char *pClip;
	static char Buffer[256];
	char *pNome;

	int i;

	if (TestLista==true) {
		Buffer[0]=0;
		for (i=0;i<TotTipoOpen;i++) {
			strcat(Buffer,VetMiniOpen[i]);
			strcat(Buffer,"  ");
		}
		return Buffer;
	}

	if (pRecord->Flags & FEP_CLIPBOARD) 
		pClip="#";
	else
		pClip="";
	
	if (TestCorto==true) 
		pNome = VetMiniOpen[pRecord->TipoOpen];
	else
		pNome = VetTipoOpen[pRecord->TipoOpen];

	sprintf(Buffer,"%s%s",pClip,pNome);

	return Buffer;
}
void AggiornaComboProgrammi(void)
{
	int i;
	int n;

	SendMessage(WindComboProgrammi,CB_RESETCONTENT,0,0);

	for (i=0;i<MyPrefRoom.TotExternPrograms;i++) {
		n = i+1;
		if (n==10) n=0;

		sprintf(BufferLog,"(SHIFT %d) %s <%s>",n,
					MyPrefRoom.VetExternPrograms[i].Titolo,
					FormattaTipoOpen(&MyPrefRoom.VetExternPrograms[i],true,false));
		SendMessage(WindComboProgrammi,CB_ADDSTRING,0,(LPARAM) BufferLog);
			
	}
	if (MyPrefRoom.TotExternPrograms > 0) {
		i = MyPrefRoom.TotExternPrograms -1;
		SendMessage(WindComboProgrammi,CB_SETCURSEL,(WPARAM) i,0);
	} else {
		// mettere messaggio di avviso
		SendMessage(WindComboProgrammi,CB_ADDSTRING,0,(LPARAM) "<NO PROGRAM TO LAUNCH>");
		SendMessage(WindComboProgrammi,CB_SETCURSEL,0,0);
	}
}
// room editor
void PatchMemoriaTexFaces(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pMemoriaTexFaces;

	StrPatch VetPatch[] = {{0x421881, true},
		{0x42189A, true},
		{0x4218B3, true},
		{0x4218C9, true},
		{0x4218CF, true},
		{0x4218DC, true},
		{0x4218E3, true},
		{0x43A39A, true}};

	OffsetOldInizio = 0x4BD640;
	OffsetOldFine=0x506A20;
	TotPatchs=8;
	// size originale = 0x493E0
	pMemoriaTexFaces = (DWORD *) 0x45461A;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,OffsetOldFine, 
				0x124F80,  
				"MemoriaTexFaces");
	*pMemoriaTexFaces = OffsetNew;
}

// per room editor
void PatchMemoriaTextureDD(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pMemoriaTextureDD;

	StrPatch VetPatch[] = {{0x445B1D, true},
		{0x445D2D, true},
		{0x445FE9, true},
		{0x445FF5, true},
		{0x445FFB, true},
		{0x44600E, true},
		{0x44601B, true},
		{0x446028, true},
		{0x446035, true},
		{0x446042, true},
		{0x44604F, true},
		{0x446071, true}};

	OffsetOldInizio = 0x46EA78;
	OffsetOldFine=    0x492A78;
	TotPatchs=12;
	// dimensione originale: 0x24000
	pMemoriaTextureDD = (DWORD *) 0x454622;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,OffsetOldFine, 
			0x290000,"MemoriaTextureDD");
	*pMemoriaTextureDD = OffsetNew;
}
void RemoveNewLine(char *pTesto)
{
	DWORD i;

	i = strlen(pTesto);

	if (i > 0) {
		if (pTesto[i-1] == 0x0a) pTesto[i-1]=0;
	}

}
// per room editor
void PatchVetSegnaliMesh(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pVetSegnaliMesh;

	StrPatch VetPatch[] = {{0x41ED7D, true},
		{0x41EDFE, true},
		{0x41EE2A, true},
		{0x41EE44, true},
		{0x41EE6E, true},
		{0x41EE88, true},
		{0x41EEB4, true},
		{0x41EED8, true},
		{0x41EF04, true}};

	OffsetOldInizio = 0x56DA60;
	OffsetOldFine=0x56F360;
	TotPatchs=9;
	pVetSegnaliMesh = (DWORD *) 0x45462A;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio, 
		OffsetOldFine, 0x6400,"VetSegnaliMesh");
	*pVetSegnaliMesh = OffsetNew;
}
void PatchTuttoSegnaliMesh(void)
{
	PatchTotSizeSegnaliMesh();
	PatchVetSegnaliMesh();

}
// per room editor
void PatchMemoriaTexture8Bits(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pMemoriaTexture8Bits;

	StrPatch VetPatch[] = {{0x407035, true},
		{0x414C7C, true},
		{0x414CE8, true},
		{0x414D0E, true},
		{0x414DE4, false},
		{0x4157CF, true},
		{0x415CED, true},
		{0x4163AF, true},
		{0x416863, true},
		{0x41F965, true},
		{0x42A700, true},
		{0x42B952, true}};

	OffsetOldInizio = 0x5737C0;
	OffsetOldFine=0x5837C0;
	TotPatchs=12;
	pMemoriaTexture8Bits = (DWORD *) 0x454626;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio, 
			OffsetOldFine, 0x40000,"MemoriaTexture8Bits");
	*pMemoriaTexture8Bits = OffsetNew;
}

// restituisce true se il tasto virtualkey e' attualmente premuto
bool TastoPremuto(int VirtualKey) 
{
	if (GetAsyncKeyState(VirtualKey) < 0) 
		return true;
	else
		return false;

}
void MettiNomeProject(char *pTitolo, OPENFILENAME *pOpen, char *pOldNome)
{
	char *pPezzoNome;
	char *pSrcFileName;

	if (strcmpi(pTitolo,"Save Project") != 0) return;

	// ok, e' uguale. ora estrarre solo nome del vecchio

	pPezzoNome = SoloNome(pOldNome);
	pSrcFileName = pOpen->lpstrFile;
	if (pSrcFileName != NULL) strcpy(pSrcFileName,pPezzoNome);


}

void StampaLogTomb3(char Messaggio[])
{
	FILE *pFile;

	pFile = fopen("mylog.txt","ab");
	if (pFile == NULL) return;
	fprintf(pFile,"%s\r\n", Messaggio);
	fclose(pFile);

}
int MostraMessageBoxNGLE(char Messaggio[],  int TipoMessageBox )
{
	int TipoBox;
	char *pTitolo;
	int Valore;
	HWND *pMiaWind = (HWND*) 0x46E559;
	DWORD TempoNow;


	if (TestIgnoraMsgBox == true && TipoMessageBox != 0x0f) return 0;

	TempoNow = GetTickCount()- LastTimeMsgBox;
	if (TempoNow < 500) 
		TotMsgbox++;
	else
		TotMsgbox=0;


	TipoBox= 0;
	switch (TipoMessageBox)
	{		
	case 0x0F:
		pTitolo= "REQUEST - NGLE Room Editor";
		TipoBox = 1;
		break;
	case 0x03:
		pTitolo="ERROR - NGLE Room Editor";
		break;
	default:
		pTitolo = "NGLE Room Editor";
	}
	// Ok = 1 / Cancel = 2
	Valore= MessageBox(*pMiaWind,Messaggio, pTitolo,TipoBox | MB_APPLMODAL	);
	if (Valore==2) Valore =0;
	if (TotMsgbox > 2) {
		if (MessageBox(*pMiaWind, "You are receiving many messages in short time. If you wish it's possible disable showing for further mexages\r\nDo you want ignore following mexages?", 
				"NGLE",MB_YESNO) == IDYES) 
			TestIgnoraMsgBox=true;
		else
			TotMsgbox = 0;
	}
	LastTimeMsgBox = GetTickCount();
	return Valore;

}


// se stanza IndiceRoom e' flippata, restituisce l'indice della stanza principale
// se non e' flippata restituisce IndiceRoom
int GetStanzaPrincipale(int IndiceRoom)
{
	int i;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;

	pVetRooms = *p2VetRooms;
	if (pVetRooms[IndiceRoom].RoomButtons & 0x2) {
		// questa stanza e' flippata: cercare la stanza principale di questa
		for (i=0;i<*pTotRooms;i++) {
			if (pVetRooms[i].TestEmptySlot == 0 && 
					pVetRooms[i].IndexFlipRoom == IndiceRoom) {
				IndiceRoom = i;
				break;
			}
		}
	}
	return IndiceRoom;
	
}

// visualizza nuova stanza con indice IndiceNuovaStanza.
// Se IndiceNuovaStanza corrisponde ad una stanza flippata, mostra la stanza
//      flippata e poi attiva pulsante flipmap
// inserisce sempre anche l'indice di stanza non flippata in IndiceRoomAttuale
void MostraNuovaStanza(int IndiceNuovaStanza)
{
	DWORD *pIndiceItemSelezionato = (DWORD *) 0x583BA4;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int  Indice;
	int *pAltroMostraFlip = (int*) 0x46DF90;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;

	pVetRooms = *p2VetRooms;

	if (IndiceNuovaStanza >= *pTotRooms) {
		sprintf(BufferLog,"ERROR: attempt to show room with index higher than number of rooms: %d (Number of rooms=%d)\r\nRedirect to room 0",
			IndiceNuovaStanza, *pTotRooms);
		InviaLog(BufferLog);
		IndiceNuovaStanza=0;
	}

	// analisi per vedere se stanza e' flippata
	Indice = GetStanzaPrincipale(IndiceNuovaStanza);

	InitTabellaGrid(&pVetRooms[Indice]);
	

	sprintf(BufferLog,"Move to room: 0x%X  (Elaborate index=0x%X)", 
				IndiceNuovaStanza, Indice);
	InviaLog(BufferLog);



	OldLastTime = GetTickCount();

	__asm {
		pushad
		mov eax, 426090h    ;MostraStanze
		call eax
		// impostare solo adesso IndiceRoomAttuale
		mov  eax, 583BA0h ;IndiceRoomAttuale
		lea   ecx, Indice 
		mov  ecx, dword ptr [ecx]
		mov  dword  ptr [eax], ecx ;copia nuovo numero di stanza

		push 00h	
		mov eax, 410120h    ;ShowSelezione2d
		call eax
		add  esp, 04h
		push	0
		mov		eax, 425F70h; VisualizzaNuovaStanza
		call	eax
		add		esp,4
		popad
	}

	if (Indice != IndiceNuovaStanza) {

		InviaLog("\tFlipped room, required to enable flipmap button");

		if (*pAltroMostraFlip == 0) {

			// la stanza bisogna renderla flippata
			__asm {
				pushad
				mov		eax, 40E130h  ; Menu_FlipMap
				push	1
				call	eax
				add		esp,4
				mov eax, 401200h    ;CicloPrincipale
				call eax	
				popad
			}
		}else {
			// era gia' attiva flipmap
			InviaLog("Mappa flippata era gia' attiva");
		
		}

	}else {
		// stanza NON e' flippata, se pero' attualmente e' attivo il flip
		// disattivare flipmap
#ifdef LOG_OLDPROJECT
		sprintf(BufferLog, "\tCurrent value of FlipMapTest=0x%X", *pAltroMostraFlip);
		InviaLog(BufferLog);
	
#endif

		if (*pAltroMostraFlip != 0) {
#ifdef LOG_OLDPROJECT
			InviaLog("\tNon Flipped Room: required to disable Flip Map Button");
#endif

			__asm {
				pushad
				mov		eax, 40E130h  ; Menu_FlipMap
				push	0
				call	eax
				add		esp,4
				mov eax, 401200h    ;CicloPrincipale
				call eax	

				popad
			}
		}
		
	}


}


void RefreshStanzaAttuale(void)
{

	MostraNuovaStanza(GetIndiceRoom(-1));
}

void GotoPreviousRoom(int Incremento)
{
	DWORD Indice;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	int	i;
	int StanzaNow;
	bool TestSoloQuesta;
	bool TestTrovata;
	char MexOltre[] = " (except current room) ";
	char MexNo[] ="";
	char *pMex;
	int *pTotRooms = (int*) 0x56C710;
	int IndiceRoom;
	bool TestElimina;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;


	pVetRooms = *p2VetRooms;

	// scorrere lista oldroom e visualizzare la prossima

	Indice = GetIndiceProgettoNow();
	// eliminare da elenco le stanze che NON sono allocate o che hanno numero
	// maggiori di totslot
	for (i=0;i<MAX_OLD_ROOMS;i++) {
		IndiceRoom = MyPrefRoom.VetOldProjects[Indice].VetLista[i];
		if (IndiceRoom !=-1) {
			TestElimina=false;
			if (IndiceRoom >= *pTotRooms) {
				TestElimina=true;
			}else {
				if (pVetRooms[IndiceRoom].TestEmptySlot != 0) TestElimina=true;
			}
			if (TestElimina==true) {
				MyPrefRoom.VetOldProjects[Indice].VetLista[i]=-1;
			}
		}
	}

	StanzaNow = GetIndiceRoom(-1);
	TestSoloQuesta=false;
	TestTrovata=false;

	// prima verificare che ci sia un indice di stanza diverso da quello attuale
	for (i=0;i<MAX_OLD_ROOMS;i++) {
		if (MyPrefRoom.VetOldProjects[Indice].VetLista[i] != -1 &&  
				MyPrefRoom.VetOldProjects[Indice].VetLista [i] != StanzaNow) {

			TestTrovata=true;
			break;
		}
		if (MyPrefRoom.VetOldProjects[Indice].VetLista[i] == StanzaNow) {
			TestSoloQuesta=true;
		}
	}

	if (TestTrovata==false) {
		if (TestSoloQuesta==true) {
			pMex = MexOltre;
		}else{
			pMex= MexNo;
		}

		sprintf(BufferLog,"No previous edited room %s.\r\nTIP: You can add a room in edited room list using keyboard command M (Mark) or removing from list using keyboard command U (Unmark)",
			pMex);
		MostraMessageBoxNGLE(BufferLog,3);
		return;
	}

	// ok ora cercare seguendo l'ordine stabilito da incremnto
	i = MyPrefRoom.VetOldProjects[Indice].IndiceRead;
	while (MyPrefRoom.VetOldProjects[Indice].VetLista[i] == -1 ||
		MyPrefRoom.VetOldProjects[Indice].VetLista[i] == StanzaNow) {
		
		i += Incremento;
		if (i < 0) i = MAX_OLD_ROOMS -1;
		
		if (i >= MAX_OLD_ROOMS) i = 0;
	}
	StanzaNow = MyPrefRoom.VetOldProjects[Indice].VetLista[i];

#ifdef LOG_OLDPROJECT 

	sprintf(BufferLog,"GoBackRoom: goto room %d with index list = %d",
			StanzaNow,i);
	InviaLog(BufferLog);
#endif
	MyPrefRoom.VetOldProjects[Indice].IndiceRead = i;
	sprintf(BufferLog,"Go Back [%d/%d]", i+1, 
				MAX_OLD_ROOMS);
	ShowInfoRoom(BufferLog);
	MostraNuovaStanza(StanzaNow);

}
// viene chiamato dopo aver caricato o salvato il progetto
void AggiornaTitoloFinestra(char *pNomeProject, HWND MiaWind, bool TestLoad)

{

	char BufferTemp[512];
	char *pNome;
	int i,j;
	int SizeX;
	int SizeY;
	int* pN_TextureTails = (int*) 0x56C4F4;
	bool TestErrore;
	DWORD *pBaseTga = (DWORD*) 0x56C4F8;
	bool TestPresente;
	StrOldProject NewRecord;
	int IndicePresente;
	int IndiceVuoto;
	short IndiceRoom;
	int* pProjectVersionNumber = (int*) 0x56C704;
	DWORD *pTestBigTexture = (DWORD *) 0x454ffc;

	if (TestLoad == true) {
		VersioneProject = *pProjectVersionNumber;
		TestLoadedProject=true;
		DatiTexture.TotLastSort=0;
	}
	strcpy(NomeLoadProject,pNomeProject);
	if (strlen(pNomeProject)> 0) {		
		pNome = pNomeProject;
	}
	else{
		pNome= "<EMPTY>";
	}

	TotCambiBackup=0;

#ifdef LOG_OLDPROJECT
	sprintf(BufferLog,"AggiornaTitoloFinestra() TestLoad=%d  File=\"%s\"",
			TestLoad,pNomeProject);
	InviaLog(BufferLog);

	InviaLog("\tLISTA ATTUALE:");
	for (i=0;i<MyPrefRoom.TotOldProjects;i++) {
		sprintf(BufferLog,"\t%d) %s\r\n",i, MyPrefRoom.VetOldProjects[i].NomeProject);
		InviaLog(BufferLog);
	}


#endif

	sprintf(BufferTemp,"NGLE %s - %s  (v%d)",MexVersione, pNome, VersioneProject);
	CambiaTitoloNgle(BufferTemp);

	TestErrore=false;
	// controlla vetbigtails
	for (i=0;i< *pN_TextureTails;i++) {
		SizeX= VetBigTails[i].SizeX / 256;
		SizeY= VetBigTails[i].SizeY / 256;
		SizeX++;
		SizeY++;

		if ((SizeX & 0x0f) != 0 || (SizeY & 0x0f) != 0) {

			sprintf(BufferTemp,"ERROR: Current project [%s] has room tail infos corrupted", 
				pNome);
			MostraMessageBoxNGLE(BufferTemp,3);
			InviaLog(BufferTemp);

			TestErrore=true;
			break;
		}
	}
	if (TestLoad == false && TestErrore ==false) {
		MostraMessageBoxNGLE("Project saved OK",7);
		sprintf(BufferTemp, "Saved project in: %s", pNome);
		InviaLog(BufferTemp);

	}	
	if (TestLoad == true) {
		sprintf(BufferTemp, "Loaded project from: %s", pNome);
		InviaLog(BufferTemp);
	}
	// gestione per old progetti
	
	IndicePresente=-1;
	IndiceVuoto=-1;
	InizializzaRecordOld(&NewRecord);
	strcpy(NewRecord.NomeProject,pNomeProject);

	TestPresente=false;
	// prima verificare se esiste gia' il progetto appena caricato o salvato
	for (i=0;i< MyPrefRoom.TotOldProjects ;i++) {
		if (strcmpi(MyPrefRoom.VetOldProjects[i].NomeProject , pNomeProject)==0) {
			IndicePresente =i;
			TestPresente=true;
			break;
		}

		if (MyPrefRoom.VetOldProjects[i].NomeProject[0] == 0) {
			IndiceVuoto=i;
		}

	}
#ifdef LOG_OLDPROJECT
	sprintf(BufferLog,"\tIndicePresente=%d   IndiceVuoto=%d",IndicePresente,IndiceVuoto);
	InviaLog(BufferLog);
#endif
	
	if (IndiceVuoto != -1) {
		// copiarci nome
		strcpy(MyPrefRoom.VetOldProjects[IndiceVuoto].NomeProject,pNomeProject);
		TestPresente=true;
		IndicePresente=IndiceVuoto;
	}
#ifdef LOG_OLDPROJECT
	if (TestPresente==true) {
		sprintf(BufferLog,"\tRecord presente: %s",MyPrefRoom.VetOldProjects[IndicePresente].NomeProject);
		InviaLog(BufferLog);
	}
#endif

	// ora inizializzare NewRecord da aggiungere
	if (TestPresente == false && TestLoad==false) {
		//abbiamo salvato con un nome diverso.
		// utilizzare i dati di vecchio indiceprogettonow e copiarci il nome attuale
#ifdef LOG_OLDPROJECT
		InviaLog("\tSalvataggio con nuovo nome file non ancora presente");
#endif
		i = MyPrefRoom.IndiceProjectNow;
		if (i != -1) {
			memcpy(&NewRecord,&MyPrefRoom.VetOldProjects[i],sizeof(NewRecord));
			strcpy(NewRecord.NomeProject,pNomeProject);
			
		}
	}

	if (TestPresente==true) {
		// usare i dati IndicePresente
		memcpy(&NewRecord,&MyPrefRoom.VetOldProjects[IndicePresente],sizeof(NewRecord));
	}

#ifdef LOG_OLDPROJECT
	sprintf(BufferLog,"\tNewRecord: %s",NewRecord.NomeProject );
	InviaLog(BufferLog);
#endif
	// ok, ora in NewRecord ci sono i dati da aggiungere in posizione 0
	// ma prima bisogna eliminare (se presente) il record di indicepresente
	// in questo modo sara' in prima posizione
	if (TestPresente == true  && IndicePresente != 0) {
		// eliminare record in vecchia posizione
#ifdef LOG_OLDPROJECT
		sprintf(BufferLog,"\tEliminare vecchia posizione record %d",IndicePresente);
		InviaLog(BufferLog);
#endif
		for (i=IndicePresente;i<MyPrefRoom.TotOldProjects - 1;i++) {
			memcpy(&MyPrefRoom.VetOldProjects[i],&MyPrefRoom.VetOldProjects[i+1],
					sizeof(StrOldProject));
		}
	}

	if (TestPresente == false) {
		// incrementare il numero di progetti se non gia' al massimo
		if (MyPrefRoom.TotOldProjects < MAX_PROGETTI) MyPrefRoom.TotOldProjects ++;
#ifdef LOG_OLDPROJECT
		sprintf(BufferLog,"\tIncrementare numero progetti: %d",
				MyPrefRoom.TotOldProjects);
		InviaLog(BufferLog);
#endif
	}

	// ora spostare tutti i record in avanti per fare posto nella prima posizione

	if ((TestPresente==true && IndicePresente != 0) || TestPresente==false) {
#ifdef LOG_OLDPROJECT
		InviaLog("\tCreare spazio in posizione indice 0");
#endif
		for (i=MyPrefRoom.TotOldProjects-2;i>=0;i--){
			memcpy(&MyPrefRoom.VetOldProjects[i+1],&MyPrefRoom.VetOldProjects[i],
					sizeof(StrOldProject));
		}
	}

	//e infine copiare in prima posizione 
	memcpy(&MyPrefRoom.VetOldProjects[0],&NewRecord,sizeof(StrOldProject));
	MyPrefRoom.IndiceProjectNow=0;

#ifdef LOG_OLDPROJECT
	InviaLog("\tLISTA DOPO AGGIORNAMENTO:");

	for (i=0;i<MyPrefRoom.TotOldProjects;i++) {
		sprintf(BufferLog,"\t\t%d) %s\r\n",i, MyPrefRoom.VetOldProjects[i].NomeProject);
		InviaLog(BufferLog);
	}
#endif

	if (TestLoad==true) {
		// posizionarsi su ultima room modificata di questo progetto
		j = MyPrefRoom.IndiceProjectNow ;
		i = MyPrefRoom.VetOldProjects[j].IndiceWrite;
		IndiceRoom = MyPrefRoom.VetOldProjects[j].VetLista[i];
#ifdef LOG_OLDPROJECT
		sprintf(BufferLog,"\tGoback a ultima room (%d) di progetto %s",
			IndiceRoom,MyPrefRoom.VetOldProjects[j].NomeProject);
		InviaLog(BufferLog);
#endif
		if (IndiceRoom != -1) {
			MostraNuovaStanza((DWORD) IndiceRoom);		
		}


	}

	
	TestLoadProject=false;
#ifdef LOG_TEXTURE
	sprintf(BufferTemp,"BASE TGA MEMORY = 0x%X", *pBaseTga);
	InviaLog(BufferTemp);
#endif
	if (TestLoad == true) {
		// analisi per versione file
		if (VersioneProject == 49) { 	
			*pTestBigTexture=1;
			// aggiornare pulsante togliendo pressione
			Sub_PulsanteBigTexture(NULL);
		}else
		{
			*pTestBigTexture=0;
			Sub_PulsanteBigTexture(NULL);

		}
	}

	if (TestLoad==true) {
		// aggiorna visualizzazione di mappa texture
		__asm {
			pushad

			mov		eax, 583BBAh ; IndiceTextureSelezionata
			mov		dword ptr [eax],0

			mov		eax, 583BD0h  ; TextureScroll_Y
			mov		dword ptr [eax], 0

			mov		eax, 4595D8h   ; Val_Scroll_Y_Pulsante
			mov		dword ptr [eax], 0

			push  4595BCh ;DatiPulsante_ScrollTexture
			mov eax, 433E80h    ;GestioneBarraScrollTexture
			call eax
			add		esp,4

			mov		eax, 42AB00h   ; RefreshZonaTexture
			call	eax

			popad
		}

	}
}

/// ax=7
// mostra valore di pulsante multiplo Water, e mostra premuto o meno
// pulsante Cold (prima era Pulsante D) e anche nuovo pulsante poison
void ShowPulsantiRoom(StrDatiPulsante *pPulsanteWater, 
					  StrDatiPulsante *pPulsanteCold,  WORD *pFlags)
{


	WORD VetFlags[] = {0, 0x0800, 0x0400, 0x0004, 0x0001};
	char *VetNomi[] ={{" Normal"}, 
						{" Rain"}, {" Snow"}, {" Q-Sand"}, {" Water"}};
	int i;
	int Indice;
	DWORD TestPremuto;

	const TotFlags = 5;
	WORD  FlagRoom;
	DWORD ColoreFondo;

	FlagRoom = *pFlags;
	FlagRoom &= 0x0c05;

	for (i=0;i<TotFlags;i++) {
		if (VetFlags[i] == FlagRoom) break;
	}

	if (i == TotFlags) i =0;

	Indice =i;
	
	if (Indice == 0) {
		TestPremuto=0;
		ColoreFondo = 1; // Grigio standard
	}else
	{
		TestPremuto=1;
		ColoreFondo = 10; // giallo

	}

	ModificaPulsanteEx(pPulsanteWater, ColoreFondo, 0, TestPremuto, VetNomi[Indice]);

	// aggiornare anche pulsante COLD
	if (*pFlags & 0x1000) {
		TestPremuto=1;
		ColoreFondo=10;
	}else{
		TestPremuto=false;
		ColoreFondo=1;
	}
	ModificaPulsanteEx(pPulsanteCold, ColoreFondo,0,TestPremuto, "Cold");

	// analisi nuovo pulsante Damage
	if (*pFlags & 0x10) {
		TestPremuto=1;
		ColoreFondo=10;
	}else {
		TestPremuto=false;
		ColoreFondo=1;
	}
	ModificaPulsanteEx(&DatiPulsanteDamage, ColoreFondo,0,TestPremuto, "DMG");

	
}

// ax=6
// Gestisce quando viene premuto pulsante mutliplo water/rain/snow
void CambiaPulsantiWater(WORD *pFlags, BYTE *pWaterQuantity)
{
	WORD VetFlags[] = {0, 0x0001, 0x0400, 0x0800, 0x0004 };

	int i;
	int Indice;
	const TotFlags = 5;
	WORD  FlagRoom;

	FlagRoom = *pFlags;
	FlagRoom &= 0x0c05;

	for (i=0;i<TotFlags;i++) {
		if (VetFlags[i] == FlagRoom) break;
	}


	// ora incrementarlo in modo rotatorio	
	Indice=i +1;


	if (Indice >= TotFlags) Indice=0;

	// adesso aggiornare flags
	// prima azzerare tutti i flags
	*pFlags &= ~0xc05;
	// ora attivare il flag
	*pFlags |= VetFlags[Indice];

	if (Indice == 1) {
		// appena usciti da "Normale" quindi inizializzare valore acqua 
		// a valore = 2
		*pWaterQuantity = 2;
	}

}

// restituisce numero di tipo triangolo selezione texture
// sulla base dei valori di input.
// OffX, OffY = Posizione relativa del mouse rispetto all'inizio della textuere selezionata
// SizeX, SizeY = Dimensione complessiva della texture
int CalcoloTextureTriangolo(int OffX, int OffY, int SizeX, int SizeY) 
{
	if (OffX > (SizeX / 2)) {
		//parte destra, o triangolo 1 o 2
		if (OffY > (SizeY / 2)) {
			// parte in basso = 2
			return 2;
		}
		// parte alta = 1
		return 1;
	}

	// parte sinistra, triangolo o 0 oppure 3

	if (OffY > (SizeY / 2)) {
		// parte bassa = 3
		return 3;
	}
	// rimane solo il trriangolo in alto a sinistra
	return 0;

}

// converte coordinate mouse da assoluto (schermo) a relativo a mappa tga
// convertendo anche da 48x48 a 64x64
void ConverteCordMouse(int *pX, int *pY)
{

	int x, y;
	int * TextureScroll_Y = (int*) 0x583BD0;

	x = *pX;
	y = *pY;

	x -= 0x331;
	y -= 0x0a;
	y += *TextureScroll_Y;


	x *= 4;
	x /= 3;

	y *= 4;
	y /= 3;

	*pX = x;
	*pY = y;


}

// gestione click con mouse sinistro in zona texture
// viene chiamata sia per bigtexture =0 che = 1
// restituisce nuovo valore per TextureTipoTriangolo

int AnalisiClickLeftTexture(int MouseX, int MouseY, 
							 StrTailInfo* pTextureTailInfos)
{
	
	int *pScrollY = (int*) 0x583BD0;
	int *pNTailInfos = (int*) 0x56C4F4;
	int *pIndiceTextureSel = (int *) 0x583BBA;
	int RestoY;
	DWORD *pBaseTga = (DWORD*) 0x56C4F8;
	DWORD OffsetTga;
	DWORD GrigliaX, GrigliaY;
	int*  TestBigTexture = (int*) 0x454FFC;
	int  Size;
	int  RestoX;
	DWORD SizeTex;

	ConverteCordMouse(&MouseX,&MouseY);

	if (*TestBigTexture == 0) {
		Size= 64;
	}else
	{
		Size=128;
	}


	RestoX = MouseX & (Size-1);
	RestoY = MouseY & (Size-1);


	// ora bisogna calcolare valore di griglia allineata a 256 x 256
	GrigliaY = MouseY / Size;  
	GrigliaX = MouseX / Size;
	
	// adesso calcolare numero di byte precedenti questa origine
	OffsetTga =  GrigliaY * 256 * Size;
	OffsetTga += GrigliaX * Size;
	OffsetTga += *pBaseTga;
	
	// ok, ora si puo' inserire i dati in texturetailinfo
	SizeTex = Size * 256;
	SizeTex--;

	pTextureTailInfos[*pNTailInfos].pMemImage  = (BYTE *) OffsetTga;
	pTextureTailInfos[*pNTailInfos].SizeX = (WORD) SizeTex;
	pTextureTailInfos[*pNTailInfos].SizeY = (WORD) SizeTex;

	// metterli temporaneamente anche in vetbigtails
	VetBigTails[*pNTailInfos].pMemImage = (BYTE*) OffsetTga;
	VetBigTails[*pNTailInfos].SizeX = (WORD) SizeTex;
	VetBigTails[*pNTailInfos].SizeY = (WORD) SizeTex;

	// imposta texture selezionata
	*pIndiceTextureSel = *pNTailInfos;
	
	// adesso ci sarebbe analisi per triangolo
	
	return CalcoloTextureTriangolo(RestoX,RestoY,Size,Size);

}


// Nome: PreparaOpenFile
// input: char Titolo[],char TipoFile[],char TipoMaschera[],BYTE TipoOperazione 
// input: (to_Load / to_Save)
// scopo: Inizializza struttura per GetOpenFileName. 
// note: Titolo[] = titolo finestra / TipoFile[] = descrizione file es. "QUESTIONARI" /
// note: TipoMaschera[] = maschera di ricerca es. "*.QST" / TipoOperazione =
// note: caricamento (file deve esistere) to_Load o registrazione (file puo' essere
// note: creato con nuovo nome) to_Save

void PreparaOpenFile(char Titolo[],char TipoFile[], 
					 char TipoMaschera[],BYTE TipoOperazione, 
					 char*pDirDefault, char *pNomeFile)
{
	WORD  	i;
  //   char		BufDirDefault[256];
	static char BufFiltri[256];
	char *pMex;

	memset( &ofn, 0, sizeof(ofn));

	// creare coppia stringhe "Descrizione" "Maschera.*"
	memset(BufFiltri, 0, 255);
	strcpy(BufFiltri,TipoFile);
	i = strlen(TipoFile)+1;

	strcpy(&BufFiltri[i],TipoMaschera);
	i += strlen(TipoMaschera)+1;

	// mettere sempre anche "all files"
	pMex = "All Files (*.*)";
	strcpy(&BufFiltri[i],pMex);
	i += strlen(pMex)+1;

	pMex = "*.*";
	strcpy(&BufFiltri[i],pMex);
	i += strlen(pMex)+1;
	// mettere zero finale per segnalare fine di coppie di stringhe

	BufFiltri[i]=0;

	


	ofn.lStructSize=sizeof (ofn);
	ofn.hwndOwner=NULL;
	ofn.hInstance=NULL;
	ofn.lpstrFilter = BufFiltri;
	ofn.lpstrCustomFilter= NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=0;
	ofn.lpstrFile=pNomeFile;
	ofn.nMaxFile= 256;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = NULL;
	ofn.lpstrInitialDir = pDirDefault;
	ofn.lpstrTitle = Titolo;

	if (TipoOperazione == to_Load) 
				ofn.Flags =OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			else
				ofn.Flags =OFN_HIDEREADONLY;

	ofn.nFileOffset=0;
	ofn.nFileExtension=0;
	ofn.lpstrDefExt=NULL;
	ofn.lCustData= 0;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
}

void SalvaUltimoTempoInput(void)
{
	HWND *pMiaWind = (HWND*) 0x46E559;

	if (GetForegroundWindow() == *pMiaWind) 
		LastTimeStandBy = GetTickCount();

}
// riceve l'indice di una texture full che risulta usata anche da
// un'altra texture parziale.
// controllare se l'indicefull fa parte di un range di animazione
// e se lo e' incrementa il cotnatore di multirange per quel range
// di cui fa parte
// oltre ad aggiornre contatore salva (ma solo per texture scrolling)
// le accoppiate IndiceFull -> IndiceRange e la posizione
// relativa. di indice range rispetto a indice full.
// restituisce true se questo e' un frammento di texture di
// qualche range d'animazione, oppure false se non lo e'
bool AggiornaMultiAnimRange(int IndiceFull, int IndiceParziale, int OffX, int OffY)
{
	int i;
	int *pTotRangeExported = (int*) 0x568280;
	int *VetIndiciRange = (int*) 0x56C440;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	WORD TestScroll;
	WORD Maschera;
	int Indice;
	int N;
	StrTexParziali *pParziale;

	Maschera =  GlobNgle.RoomTexAnimate.SizeDefault - 1;

	OffX = OffX & Maschera;
	OffY = OffY & Maschera;



	// prima vedere se IndiceFull fa parte di qualche range
	for (i=0;i<*pTotRangeExported;i++) {
		Indice = VetIndiciRange[i];
		if (VetAnimRange[Indice].TestUsato) {
			if ((GlobNgle.RoomTexAnimate.VetInfoRangeAnim[Indice] & 
						FAN_ANY_ROTATE ) !=0 ) 
				TestScroll=1;
			else
				TestScroll=0;


			if (IndiceFull >= VetAnimRange[Indice].IndiceStart && 
				IndiceFull <= VetAnimRange[Indice].IndiceEnd) {

				// si, questa texture fa parte di un range animato
				sprintf(BufferLog,"\tAppartiene a range %d", Indice);
				InviaLog(BufferLog);

				// ora salvare i dati di texfull e parziale
				N = GlobNgle.BaseTexParziali.TotRecords;
				pParziale = &GlobNgle.BaseTexParziali.VetParziali[N];
				pParziale->IndiceFull  = IndiceFull;

				pParziale->IndiceFrammento  = IndiceParziale;
				pParziale->IndiceRange = i;
				pParziale->OffX = OffX;
				pParziale->OffY = OffY;
				GlobNgle.BaseTexParziali.TotRecords++;
				if (TestScroll == 0) return false;
				return true;

			}
			
		}
	}
	InviaLog("\tNon appartiene ad alcun range animazione");
	return false;

}
// chiamata quando viene alvato header ng per un file tom
void SalvaDatiNGTomFile(char *pNomeFile)
{
	WORD *pVetExtra=NULL;
	int NWords;
	int i;
	static int *pTotRangeExported = (int*) 0x568280;
	static int *VetIndiciRange = (int*) 0x56C440;
	static DWORD *pVersioneWad = (DWORD *) 0x624570;
	DWORD SizeMem;
	WORD TotBytes;
	WORD NumeroWords;
	StrDatiExtraAnimazioni *pExtra;
	int Indice;
	int j;
	BYTE *pBytes;


	NWords =0;


	CopiaInHeader(NGTAG_EXTRA_INFO_ROOMS, GlobNgle.TotExtraInfoRoom, 
			sizeof(StrExtraInfoRoom), &GlobNgle.VetExtraInfoRoom[0],
			&pVetExtra, &NWords);


	// ------ pacchetto per vetremapstatici -------------

	CopiaInHeader(NGTAG_REMAP_STATICS, NO_ARRAY,
			sizeof(StrCercaStatic) * 6000, &GlobNgle.VetRemapStatics[0],
			&pVetExtra, &NWords);

	// ------  pacchetto per info relativi ad animazioni -----

	// questo purtroppo lo devo salvare in modo caotico
	SizeMem = NWords *2;

	TotBytes = sizeof(StrDatiExtraAnimazioni);
	NumeroWords = TotBytes / 2 + 2;
	SizeMem += NumeroWords * 2;

	pVetExtra = (WORD*) realloc(pVetExtra, SizeMem);

	// paccheto per info relativi ad animazioni
	pVetExtra[NWords++] = NumeroWords;
	pVetExtra[NWords++] = NGTAG_ANIMATED_TEXTURES;  // vettore info per ogni animazione
	GlobNgle.RoomTexAnimate.TestPresente=true;
	// salvare l'intera struttura, facendo si che 
	// che i range siano ordinati
	pExtra = &GlobNgle.RoomTexAnimate;

	pVetExtra[NWords++] = (BYTE) pExtra->TestPresente + pExtra->Tot_UV_Rotate * 256;
	pVetExtra[NWords++] = pExtra->TotaleRangeNG;

	for (i=0;i<*pTotRangeExported;i++) {
		Indice = VetIndiciRange[i];
		pVetExtra[NWords++] = pExtra->VetInfoRangeAnim[Indice];
	}
	// salvare residui
	for (j=i;j<40;j++) {
		pVetExtra[NWords++] = 0;
	}
	// ora salvare i dati per inizio fine tex
	// sono gia' ordinati
	for (i=0;i<40;i++) {
		pVetExtra[NWords++] = pExtra->VetFromTex[i];
	}

	for (i=0;i<40;i++) {
		pVetExtra[NWords++] = pExtra->VetToTex[i];
	}

	pVetExtra[NWords++] = pExtra->SizeDefault;

	// ----------- copiare remapobj -------------------
	CopiaInHeader(NGTAG_REMAP_OBJ,NO_ARRAY ,
				6000 * sizeof(WORD), &SalvaVettoriRemap.VetObjRemap[0],
				&pVetExtra, &NWords);
	// ----- salva plugin names ------------------------

	CopiaInHeader(NGTAG_PLUGIN_NAMES_NGLE, GlobNgle.TotPluginNgle,sizeof(StrPluginNames),  &GlobNgle.VetPluginNgle[0],
			&pVetExtra, &NWords);

	// ----------- texture animate parziali ------------
	CopiaInHeader(NGTAG_TEX_PARZIALE, GlobNgle.BaseTexParziali.TotRecords,
				sizeof(StrTexParziali), &GlobNgle.BaseTexParziali.VetParziali[0],
				&pVetExtra, &NWords);
	

	// ---------- registra floordataplugin -----------------
	// tabella a dimensione byte (mentre la floor data era a dikmensione word)
	// dove per ogni byte c'e' l'id del plugin (fonte ngle) del trigger nella posizone corrispondente
	// in tabella floordata
	TotBytes=0;

	for (i=0;i<GlobNgle.TotPosFloor;i++) {
		if (GlobNgle.VetPosFloorIndex[i] >= TotBytes) {
			TotBytes = GlobNgle.VetPosFloorIndex[i] +1;
		}
	}
	pBytes=NULL;
	if (TotBytes > 0) {
		// se numero e' disapri aumentarlo di 1
		if (TotBytes & 0x01) TotBytes++;

		pBytes=(BYTE *) malloc(TotBytes);
		memset(pBytes, 0, TotBytes);
		for (i=0;i<GlobNgle.TotPosFloor;i++) {
			Indice=GlobNgle.VetPosFloorIndex[i];

			pBytes[Indice] = GlobNgle.VetPosFloorPluginId[i];
		}

		CopiaInHeader(NGTAG_PLUGIN_ID_FLOOR_TABLE, TotBytes, 1, pBytes, 
				&pVetExtra, &NWords);
		free(pBytes);
	}else {
		// scrive valore vuoto
		CopiaInHeader(NGTAG_PLUGIN_ID_FLOOR_TABLE, 0, sizeof(WORD), &TotBytes, 
				&pVetExtra, &NWords);


	}

	// ------- registra flag per livello attuale --------
	NumeroWords = 4;

	SizeMem = NWords * 2;

	SizeMem += NumeroWords * 2 + 10;
	pVetExtra = (WORD*) realloc(pVetExtra, SizeMem);

	pVetExtra[NWords++] = NumeroWords;
	pVetExtra[NWords++] = NGTAG_FLAG_LEVEL_TR4;
	pVetExtra[NWords++] = GlobNgle.FlagsLevelTr4;
	pVetExtra[NWords++] = GlobNgle.Flags2LevelTr4;

	// ----- salva remap per le room ------------
	CalcolaVetRemapRooms();

	CopiaInHeader(NGTAG_VET_REMAP_ROOMS, NO_ARRAY, MAX_ROOMS * 2, 
			&GlobNgle.VetRemapRooms[0], &pVetExtra, &NWords);

	CopiaInHeader(NGTAG_TOM_VERSION, NO_ARRAY,
				sizeof(StrVersionHeader), &GlobNgle.BaseVersione,
				&pVetExtra, &NWords);

	if (WriteNGHeader(pNomeFile,pVetExtra,NWords)==false) {

		sprintf(BufferLog,"ERROR: Trying to save extra ng header, cann't find or open file: %s", pNomeFile);
		InviaLog(BufferLog);
		MostraMessageBoxNGLE(BufferLog,3);

	}
	if (pVetExtra !=NULL) free(pVetExtra);

		
}

// chiamata da ngle direttamente da roomeditor exe
int SalvaTomTailInfos(FILE *pFile, StrTailInfo* pVetTailInfos,  
					  short* pVetPrimario)
{
	BYTE * Ptr_MemoriaFileTexture;
	BYTE * Ptr_TextureRawImage;
	int IndiceFull;
	int IndiceNuovoI;
	WORD  TipoAnim;
	BYTE ** Ptr2_MemoriaFileTexture = (BYTE **) 0x583C2D;
	BYTE ** Ptr2_TextureRawImage = (BYTE **) 0x56C4F8;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	//int	*VetAnimCodes = (int*) 0x56BF80;
	//int *pTotAnimCodes = (int*) 0x56C420;
	int *pTotRangeExported = (int*) 0x568280;
	int *VetIndiciRange = (int*) 0x56C440;
	int * pVetPtrTextureExport = (int*) 0x566680;
	int IndiceNow, GrigliaX,GrigliaY;
	int *pNTextureTails = (int*) 0x56C4F4;
	int  MaxGrigliaY;
	int  MaxSizeImage;
	BYTE *pGriglia;
	int  IndiceNew;
	int  x,y,i,j;
	StrTailInfoOut MyTail;
	int  SizeX,SizeY, IndiceOld, Valore;
	bool TestOk;
	int  cx,cy;
	int NTailExport;
	int LimiteGrigliaY;
	int OrgX, OrgY;
	int RIGA_VETTORE;
	int Temp;
	int Size_Texture;
	int MaxTile;
	int VetTexFullScroll[256];
	int TotTexFullScroll;
	bool  TestFullScroll;
	int Indice;
	StrDatiTexture *pVetTex;
	StrDatiTexture *pTexNow;
	WORD TempSizeX,TempSizeY;

	BYTE *pSrc;
	BYTE *pDest;

	if (VersioneProject == 50) 
		Size_Texture=128;
	else {
		Size_Texture=64;
		VersioneProject=49;
	}

	GlobNgle.BaseTexParziali.TotRecords=0;

	GlobNgle.RoomTexAnimate.SizeDefault = Size_Texture;

	// azzerare valori residui di VetOutTexBumpMap

	for (i=0x100;i<MAX_TAIL_INFOS;i++) {
		VetOutTexBumpMap[i] = 0;
	}

	sprintf(BufferLog,"Export Tail Infos for tom file: project version=%d", VersioneProject);
	InviaLog(BufferLog);

	RIGA_VETTORE = 256 / Size_Texture;

	Ptr_MemoriaFileTexture = *Ptr2_MemoriaFileTexture;
	Ptr_TextureRawImage = *Ptr2_TextureRawImage;

	// trovare tutte le tex che richiedono full scroll
	TotTexFullScroll=0;
	GlobNgle.RoomTexAnimate.TotaleRangeNG = *pTotRangeExported;

	for (i=0;i< *pTotRangeExported;i++) {
		Indice = VetIndiciRange[i];
		if (VetAnimRange[Indice].TestUsato) {
			
			// vedere se questo range richiede full scroll
			TipoAnim = GlobNgle.RoomTexAnimate.
						VetInfoRangeAnim[Indice] & FAN_MASK_ANIM;

			if (TipoAnim  == FAN_FULL_ROTATE || TipoAnim== FAN_RIVER_ROTATE) {

				//  questo rrange e' di tipo full scroll: salvare tutte
				// gli indici tail
				for (j=VetAnimRange[Indice].IndiceStart;
							j<= VetAnimRange[Indice].IndiceEnd;j++) {

					VetTexFullScroll[TotTexFullScroll++] = j;
				}
			}
		}
	}



	// prima contare quante texture sono da esportare
	NTailExport=0;
	InviaLog("Primay Array (export tails):");
	// prima era i<0x3ff
	for (i=0;i<*pNTextureTails;i++) {
		if (pVetPtrTextureExport[i] != -1) {
			
			sprintf(BufferLog,"EXPORT %d (0x%X)", i,i);
			
			InviaLog(BufferLog);
		}
		if (pVetPrimario[i] != -1) {
			// prima era qui il ntailexport
			NTailExport++;
			Indice = pVetPrimario[i];
			sprintf(BufferLog,"VetPrimario[%d] = %d (0x%X)  VetPtrTextureExport[] =%d", i, 
					pVetPrimario[i],pVetPrimario[i], pVetPtrTextureExport[i] );
			InviaLog(BufferLog);
			SizeX = VetBigTails[i].SizeX +1;
			SizeX = SizeX >> 8;
			SizeY = VetBigTails[i].SizeY +1;
			SizeY = SizeY >> 8;

			sprintf(BufferLog, "\t\tSizeX=%d SizeY=%d",
				SizeX,
				SizeY);
			InviaLog(BufferLog);



		}
	
	}
	sprintf(BufferLog,"Tot primary exported tails=%d (0x%X)", NTailExport,NTailExport);
	InviaLog(BufferLog);





	// allocare memoria per mappa immagine.
	// la dimensione massima sarebbe se tutte le texture fossero di 256 x 256
	// a cui poi aggiungere un altro 256 per allineamento
	// calcolare numero masimo di grigliaY 
	// MaxGrigliaY = (NTailExport+1)*4;

	MaxGrigliaY = (NTailExport+1)* RIGA_VETTORE;

	// MaxSizeImage = (NTailExport+1) * 256;
	// massima altezza mappa corrisponde a massima altezza mappa originale
	MaxSizeImage = 16384 + 256;
	MaxSizeImage *= 256 * 3;

	pMappaTexture = (BYTE *) malloc(MaxSizeImage);

	memset(pMappaTexture,0,MaxSizeImage);
	// allocare vettore per contenere le singole texture 64x64 gia' usate
	// e localizzare in questo modo la posizione libera per tutte le texture
	// da aggiungere
	pGriglia = (BYTE *) malloc(RIGA_VETTORE * MaxGrigliaY);
	pVetTex = (StrDatiTexture*) malloc(*pNTextureTails * sizeof(StrDatiTexture));

	memset(pGriglia,0,RIGA_VETTORE * MaxGrigliaY);
	IndiceNew=0;
	// inserire in pVetTex[] tutte le informazioni su ogni texture da convertire
	LimiteGrigliaY = -1;
	// prima era i<0x3ff
	for (i=0;i<*pNTextureTails;i++) {
		

		if (pVetPtrTextureExport[i] != -1) {
			TestFullScroll=false;

			// vedere se questa texture (con indicetail = i) 
			// e' una di quelle da animare in full scroll
			for (j=0;j<TotTexFullScroll;j++) {
				if (VetTexFullScroll[j] == i) {
					TestFullScroll =true;
					break;
				}
			}

			TempSizeX= VetBigTails[i].SizeX;
			TempSizeY = VetBigTails[i].SizeY;

			if (TestFullScroll==true) {
				// raddoppiare dimensione verticale
				switch (TempSizeY) {
				case 0x3fff:
					TempSizeY = 0x7fff;
					break;
				case 0x7fff:
					TempSizeY = 0xffff;
				}
			}
			memset(&pVetTex[IndiceNew],0,sizeof(StrDatiTexture));
			// trovare dimensione massima per questa texture
			/*
			if (TempSizeX > 0x7fff || 
				TempSizeY > 0x7fff) {
				// maggiore di 128, 
				SizeX=RIGA_VETTORE;
				SizeY=RIGA_VETTORE;
			}else{
				if (TempSizeX > 0x3fff ||
					TempSizeY > 0x3fff) {
					// maggiore di 64,ma minore o uguale 128 usare 4 riquadri
					
					SizeX=RIGA_VETTORE / 2;
					SizeY=RIGA_VETTORE / 2;
				}else{
					// minore o uguale a 64
					SizeX=1;
					SizeY=1;
				}

			}
			*/
			// fare analisi distinta per entrambe le dinensione
			// per vedere quanti riquadri sono necessari
			if (TempSizeX > 0x7fff) {
				SizeX = RIGA_VETTORE;
			}else {
				if (TempSizeX > 0x3fff) 
					SizeX=RIGA_VETTORE / 2;
				else
					SizeX=1;
				
			}

			if (TempSizeY > 0x7fff) {
				SizeY = RIGA_VETTORE;
			}else {
				if (TempSizeY > 0x3fff) 
					SizeY=RIGA_VETTORE / 2;
				else
					SizeY=1;
				
			}


			Temp = (int) VetBigTails[i].pMemImage;
			Valore = (int) Ptr_TextureRawImage;
			Temp = Temp - Valore;

			OrgY = Temp / 256;
			OrgX = Temp % 256;

			// adesso in OrgY c'e' la riga di pixel (non di griglia)
			// in OrgX c'e' il singolo pixel di colonne (non di griglia)
			// ora ottenere l'allineamento esatto a griglia 

			cx = OrgX & ~ (Size_Texture-1);
			cy = OrgY & ~ (Size_Texture-1);

			pVetTex[IndiceNew].Sorg_OrgTextureX = cx;
			pVetTex[IndiceNew].Sorg_OrgTextureY = cy;
			pVetTex[IndiceNew].Sorg_PixelTailX = OrgX;
			pVetTex[IndiceNew].Sorg_PixelTailY = OrgY;
			pVetTex[IndiceNew].SizeTextureX = SizeX * Size_Texture;
			pVetTex[IndiceNew].SizeTextureY = SizeY * Size_Texture;
			pVetTex[IndiceNew].IndiceOld = i;
			pVetTex[IndiceNew].GrigliaX = -1;
			pVetTex[IndiceNew].GrigliaY = -1;
			pVetTex[IndiceNew].TestFullScroll = TestFullScroll;
			pVetTex[IndiceNew].NewSizeY = TempSizeY;


			
			// scoprire IndiceTextureNow di questo tailinfo
			
			GrigliaX = OrgX / Size_Texture;
			GrigliaY = OrgY / Size_Texture;

			// pVetTex[IndiceNew].Sorg_IndiceTexture = GrigliaX + GrigliaY *4;
			pVetTex[IndiceNew].Sorg_IndiceTexture = 
							GrigliaX + GrigliaY * RIGA_VETTORE;

			IndiceNew++;
		}
	}
	// prima ordinare range animati
	OrdinaRangeAnimati();

	NTailExport = IndiceNew;

	for (i=0;i<NTailExport;i++) {

		// ora trovare una zona libera in griglia
		// dove ci siano l'intero riquadro SizeX,SizeY libero
		cx = pVetTex[i].Sorg_PixelTailX;
		cy = pVetTex[i].Sorg_PixelTailY;
		SizeX = pVetTex[i].SizeTextureX;
		SizeY = pVetTex[i].SizeTextureY;
		IndiceOld=pVetTex[i].IndiceOld;

		VetOutTexBumpMap[i] = pVetTextureBumpMap[IndiceOld];
		IndiceNow = pVetTex[i].Sorg_IndiceTexture;
#ifdef LOG_TEXTURE
		sprintf(BufferLog,"%d) TailExport: OrgX=0x%X  OrgY=0x%X  TailX=0x%X  TailY=0x%X  IndTexture=%d  GridX=%d  GridY=%d", 
					i, pVetTex[i].Sorg_OrgTextureX, pVetTex[i].Sorg_OrgTextureY, 
					pVetTex[i].Sorg_PixelTailX, pVetTex[i].Sorg_PixelTailY,
					pVetTex[i].Sorg_IndiceTexture, pVetTex[i].GrigliaX, pVetTex[i].GrigliaY);
		InviaLog(BufferLog);
#endif

		// prima controllare che questa texture non faccia parte di una texture
		// gia allocata
		TestOk=false;
		// prima nella condizione c'era anche "pVetTex[j].TestFullScroll == false "
		// ma ora l'ho tolto
		for (j=0;j<i;j++) {
			if (IndiceNow == pVetTex[j].Sorg_IndiceTexture && 
				pVetTex[j].GrigliaX != -1 && 
				SizeX <= pVetTex[j].SizeTextureX && 
				SizeY <= pVetTex[j].SizeTextureY ) {
				// usare la stessa posizione di griglia di quest texture
#ifdef LOG_TEXTURE
				sprintf(BufferLog,"\tUsaGriglia di Tex=%d:: OrgX=0x%X  OrgY=0x%X  TailX=0x%X  TailY=0x%X  IndTexture=%d  GridX=%d  GridY=%d", 
							j, pVetTex[j].Sorg_OrgTextureX, pVetTex[j].Sorg_OrgTextureY, 
							pVetTex[j].Sorg_PixelTailX, pVetTex[j].Sorg_PixelTailY,
							pVetTex[j].Sorg_IndiceTexture, pVetTex[j].GrigliaX, pVetTex[j].GrigliaY);
				InviaLog(BufferLog);
#endif

				// correggere status bump map usand
				// quello di indice originale della texture ecc.
				VetOutTexBumpMap[i] = 
							pVetTextureBumpMap[pVetTex[j].IndiceOld];

				// IndiceFull, i = parziale
				IndiceFull = pVetTex[j].IndiceOld;

				if (AggiornaMultiAnimRange(IndiceFull,i, 
						pVetTex[i].Sorg_PixelTailX, 
						pVetTex[i].Sorg_PixelTailY)==false) {
					// non fa parte di tex animate, allora ok usare 
					// sovrapposizione griglia
					pVetTex[i].GrigliaX = pVetTex[j].GrigliaX;
					pVetTex[i].GrigliaY = pVetTex[j].GrigliaY;
					TestOk=true;
				}
				else {
					// annullare
					InviaLog("\t\tANNULLATO SOTTO GRIGLIA: FA PARTE DI ANIMAZIONI");
				}
				break;
			}
		}

		if (TestOk ==false) {
			// non era gia' stata usata, trovarle un buco dove salvarla adesso
			SizeX = pVetTex[i].SizeTextureX / Size_Texture;
			SizeY = pVetTex[i].SizeTextureY / Size_Texture;
			
			
			// calcolare a che tipo di allineamento su riga y e' consentio

			for (y=0;y <= (MaxGrigliaY-SizeY);y++) {

				if ((y % SizeY) == 0) {

					for (x=0;x <= (RIGA_VETTORE-SizeX);x++) {
						TestOk=true;
						for (cx =0;cx <SizeX;cx++) {
							for (cy=0;cy <SizeY;cy++) {
								IndiceNow = (x+cx) + (y+cy) * RIGA_VETTORE;
								if (pGriglia[IndiceNow] != 0) {
									TestOk=false;
									break;
								}
							}
							if (TestOk ==false) break;
						}
						if (TestOk==true) break;
					}

				}
				else {
					TestOk=false;
				}
				if (TestOk==true) break;

			}

			if (TestOk==true) {
				// trovata origine in (x,y) con dimensione sottogriglia (cx,cy)
				pVetTex[i].GrigliaX = x;
				pVetTex[i].GrigliaY = y;


				// ora impostare a 1 tutti gli elementi della griglia
				// usati da questa texture da origine (x,y) con dimensione (cx,cy)


				for (cx=x;cx< (x+SizeX);cx++) {
					for (cy=y;cy < (y+SizeY);cy++) {
						IndiceNow = cx + cy*RIGA_VETTORE;
						pGriglia[IndiceNow]=1;
					}
				}
				
			}
		}
			
	}


		

	MaxTile=0;
	// adesso trasferire tutte le texture
	// e salvare le tail info con le nuove coordinate
	for (i=0;i<NTailExport;i++) {
	
		IndiceNuovoI = i; 
		pTexNow = &pVetTex[IndiceNuovoI];

		// salvare attributo texture sonore
		IndiceOld = pTexNow->IndiceOld;
		// e se questa operazione fosse inutile?
		// IndiceNow = pVetPrimario[IndiceOld];


		// imposta ptrtextureexport l'indice in corretta posizione
		// provare a lasciare = i invece di "= IndiceNuovoI" come prima
		pVetPtrTextureExport[IndiceOld] = i;
		// iniziare riempiemento tailinfo da salvare

		MyTail.SizeX = VetBigTails[IndiceOld].SizeX;
		MyTail.SizeY = VetBigTails[IndiceOld].SizeY;
		// se questa tex e' di quelle full scroll raddoppiare la dimensione
		if (pTexNow->TestFullScroll == true) 
				MyTail.SizeY = pTexNow->NewSizeY;


		// trovare orgx orgy  punto d'origine in mappa originale
		OrgX = pTexNow->Sorg_PixelTailX;
		OrgY = pTexNow->Sorg_PixelTailY;

		cx = pTexNow->Sorg_OrgTextureX ;
		cy = pTexNow->Sorg_OrgTextureY;
		// mentre in cx, cy c'e' origine allineato a texture 64x64
		
		// salvare coordinate per tailinfo da esportare
		// nota: in questo calcolo c'e' il problema che se l'origine
		// si discosta oltre i primi 63 pixel non verra' calcolata correttamente

		// MyTail.OrgX = pVetTex[i].GrigliaX * 64 + (OrgX & 0x3f);
		// y = pVetTex[i].GrigliaY * 64 + (OrgY & 0x3f);

		MyTail.OrgX = pTexNow->GrigliaX * Size_Texture + (OrgX & (Size_Texture-1));
		y = pTexNow->GrigliaY * Size_Texture + (OrgY & (Size_Texture-1));


		MyTail.OrgY = (y & 0xff);
		MyTail.IndiceTail = y / 256;

		if (MyTail.IndiceTail > MaxTile) MaxTile=MyTail.IndiceTail;

		// salvare tailinfo
		RoomWrite(&MyTail,sizeof(MyTail),1, pFile);
		
		OrgX *= 3;

		// adesso bisogna copiare immagine di intera texture
		pSrc = Ptr_MemoriaFileTexture;
		pSrc += cx *3 + cy * 256 * 3;

		pDest = pMappaTexture;
		// pDest += pVetTex[i].GrigliaX * 64 * 3 + pVetTex[i].GrigliaY * 64 * 256 * 3;

		pDest += pTexNow->GrigliaX * Size_Texture * 3 + 
					pTexNow->GrigliaY * Size_Texture * 256 * 3;

		if (pTexNow->TestFullScroll == true) {
			// calcolo particolare per duplicare meta alta in meta' bassa
			cy = pVetTex[i].SizeTextureY  / 2;
			// calcolare il gap per seconda meta di texture
			OrgY = cy * 256*3;

			// qui probabilmente si potrebbe cambiare anche ciclo x
			// per copiare solo la striscia effettivamennte usata
			// questa e' una cosa sbgliata che dovrei migliorare

			for (y =0;y<cy;y++) {
				for (x=0;x< (pTexNow->SizeTextureX * 3);x++) {
					pDest[x] = pSrc[x];
					pDest[x+OrgY] = pSrc[x];
				}
				pDest += 256*3;
				pSrc += 256*3;

			}
		}else {
			// calcolo normale, copiare l'intero blocco
			for (y =0;y<pTexNow->SizeTextureY;y++) {
				for (x=0;x< (pVetTex[i].SizeTextureX * 3);x++) {
					pDest[x] = pSrc[x];
				}
				pDest += 256*3;
				pSrc += 256*3;

			}
		}


	}

	// ok, ora calcolare dimensione da salvare della nuova mappa texture
	// provare ad allineare dimensione a limite finale di texture 256 x 256
	/*
	Temp = LimiteGrigliaY / 256;
	if (LimiteGrigliaY % 256) Temp++;
	
	LimiteGrigliaY = Temp*256;
	*/
	SizeMappaTexture = (MaxTile+1)*256 * 256 * 3;

	// inizializzare vettore di remap (solo numero tail)
	GlobNgle.BaseRemapTail.TotTails = NTailExport;
	memset(GlobNgle.BaseRemapTail.VetRemapTail, 255, 
				sizeof(StrRemapTailInfo) * MAX_TAIL_INFOS);
	
	return NTailExport;


}

void SalvaMappaTexture(FILE *pFile, int NTailExport)
{
	if (NTailExport == 0) {
		SizeMappaTexture = 0;
		RoomWrite(&SizeMappaTexture,4,1,pFile);
	}else{
		// salvare anche VetOutTexBumpMap
		RoomWrite(VetOutTexBumpMap,1,NTailExport,pFile);

		RoomWrite(&SizeMappaTexture,4,1,pFile);
		RoomWrite(pMappaTexture,SizeMappaTexture,1,pFile);
		free(pMappaTexture);
	}
}

void Menu_AboutRoomEditor(void *pPunta)
{


	sprintf(BufferLog, "NGLE (Next Generation Level Editor) is based on original Tomb Raider Level Editor by Eidos Interactive (TM)\r\n\r\nPatcher Library Next Generation %s by Paolone - paolone2011@alice.it - http://www.TrLevelManager.eu/ng.htm",MexVersione);
	MioMessageBox(BufferLog,"About NGLE program");

}

// visualizza file .pdf che trova in cartella MANUAL

void Menu_ShowDefaultHelp(void *pPunta)
{
	char NomeHelp[256];


	sprintf(NomeHelp,"%s\\manual\\manual.pdf",Dir_Trle);

	if (EsisteFile(NomeHelp)==false) {

		MostraMessageBoxNGLE("Cann't find file 'manual.pdf' in [trle\\manual] folder\r\nNote: If your .pdf manual has a name different that 'manual', rename it as 'manual.pdf', please",
				0);
		return;
		
	}
	ShellExecute(NULL,"open",NomeHelp,"",SoloDir(NomeHelp),SW_SHOW);
	
}
// visualizza (o scarica) help trle revised
void Menu_ShowRevisedHelp(void *pPunta)
{
	char NomeHelp[256];

	// era \start.htm
	sprintf(NomeHelp,"%s\\help\\_START_INDEX.htm",Dir_Trle);

	if (EsisteFile(NomeHelp)==false) {
		// provare con altro nome
		sprintf(NomeHelp,"%s\\start.htm",Dir_Trle);
		if (EsisteFile(NomeHelp)==false) {
			if (MostraMessageBoxNGLE("Cann't find file 'next_generation.htm' in [trle\\help] folder.\r\n\r\nDo you want download now the Revised Help for Level Editor? (5.5 Mb)",
					0x0f)== 0) return;
			MostraMessageBoxNGLE("When download is complete, decompress zip file directly in trle folder and it will be created a new \"HELP\" sub-folder with help files",0);

			// aprire indirizzo dove c'e' file help trle
			ShellExecute(NULL,"open","http://www.TrLevelManager.eu/download/trle_help.zip",
					"","",SW_SHOW);

			return;

		}
	}
	ShellExecute(NULL,"open",NomeHelp,"",SoloDir(NomeHelp),SW_SHOW);
	
}
// visualizza informazioni riguardo plugin attuali nel progetto
void Menu_ShowInfoPlugins(void *pPunta)
{
	FILE *pFile;
	char NomeFile[256];
	int i;
	StrPluginNames *pRecNgle;
	int j;
	char *pNome;
	int IdScript;
	char *pNomePlugin;
	char MiniBufScript[80];
	char *pMexErrore;
	StrListaFiles * pVetFiles;
	int TotFiles;

	// aggiornare trigger usati
	RefreshUsedTriggersByPlugins();

	// leggere dati da script
	LeggePluginScript(GetFileTrle("script\\script.txt"));

	sprintf(NomeFile, "%s\\InfoPluginsTable.txt", MyTempFolder);
	pFile=fopen(NomeFile, "wb");
	fprintf(pFile, "MOUNTED PLUGINS\r\n");
	fprintf(pFile, "---------------\r\n");
	fprintf(pFile, "\r\n");
	if (GlobNgle.TotPluginNgle > 0) {



		fprintf(pFile, "%-40s Triggers Ngle-Id Script-Id\r\n", "PluginName");
		fprintf(pFile, "----------------------------------------------------------------------\r\n");

		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			pRecNgle=&GlobNgle.VetPluginNgle[i];
			pNomePlugin=pRecNgle->Nome;
			// trovare id plugin script
			// ora vedere che id ha questo pNomePlugin nello script
			IdScript=-1;
			for (j=0;j<GlobNgle.PluginScriptTable.TotPlugins;j++) {
				if (strcmpi(GlobNgle.PluginScriptTable.VetPlugins[j].Name, pNomePlugin)==0) {
					IdScript = GlobNgle.PluginScriptTable.VetPlugins[j].PluginId;
					break;
				}

			}		
			if (IdScript == -1) {
				strcpy(MiniBufScript,"MISSING");
			}else {
				sprintf(MiniBufScript, "%3d", IdScript);
			}
			pMexErrore="";
			if (pRecNgle->PluginId < 1 || pRecNgle->PluginId > 255) {
				pMexErrore = "\r\n\tERROR: Invalid ngle Id! (Valid range: 1/255)\r\n";
			}

			fprintf(pFile, "%-37s %8d %7d %9s %s\r\n", 
				pNomePlugin, pRecNgle->TotUsato, pRecNgle->PluginId, MiniBufScript, pMexErrore);
		}
		fprintf(pFile, "----------------------------------------------------------------------\r\n");
	}else {
		fprintf(pFile, "There is no mounted plugin.\r\n");
		fprintf(pFile, "Note: ngle mounts only plugins having some trigger, i.e. those plugins having a [.TRG] file in trle folder\r\n");
	}
	fprintf(pFile, "\r\n");

	fprintf(pFile, "NOT MOUNTED PLUGINS\r\n");
	fprintf(pFile, "-------------------\r\n");
	fprintf(pFile, "\r\n");
	
	pVetFiles= TrovaFiles(Dir_Trle, "plugin*.dll", &TotFiles);
	if (TotFiles > 0) {

		fprintf(pFile, "Plugin libraries installed in trle but with no trigger\r\n");
		fprintf(pFile, "------------------------------------------------------\r\n");
		for (i=0;i<TotFiles;i++) {
			pNome = SoloNomeSenzaExt(pVetFiles[i].Testo);

			// visualizzarlo solo e' assente tra quelli mounted in ngle
			for (j=0;j<GlobNgle.TotPluginNgle;j++) {
				if (strcmpi(pNome, GlobNgle.VetPluginNgle[j].Nome)==0) {
					break;
				}

			}
			if (j == GlobNgle.TotPluginNgle){
				// non era tra quelli mounted
				// visualizzarlo
				fprintf(pFile, "%s\r\n", pNome);
			}
		}
		fprintf(pFile, "------------------------------------------------------\r\n");
	}else {
		fprintf(pFile, "Missing unmounted plugins\r\n");
	}
	fclose(pFile);

	ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);
}

// restituisce il nome del plugin che possiede l'id fornito, oppure NULL se non si trova
int GetPluginIndexFromId(int IdPlugin)
{
	int i;

	for (i=0;GlobNgle.TotPluginNgle;i++) {
		if (GlobNgle.VetPluginNgle[i].PluginId == IdPlugin) {
			return i;
		}
	}

	return -1;
}
// ricerca trigger ed esegue l'operazione TRT_...
void CercaTriggers(DWORD TrtTipo, int MainId, int ReplaceId)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;
	int IndicePlugin;
	int i;
	int j;
	int Indice;
	int IndiceBeta;
	StrTriggerTrovati VetTriggerTrovati[200];
	int TotTriggerTrovati;
	WORD IdPlugin;
	char * pTipoTrigger;
	int NumeroTrigger;
	FILE *pFile;
	char NomeFile[256];
	int TotRimuovi;
	int VetRimuovi[256];


	pVetTriggers = *P2_VetTriggers;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetRooms = *p2VetRooms;
	TotTriggerTrovati=0;
	
	// ----------------- operazioni preliminari -----------------------
	switch (TrtTipo) {
	case TRT_REFRESH_USED_TRIGGERS:
		
		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			GlobNgle.VetPluginNgle[i].TotUsato =0;
		}
		break;

	}

	// --------------- Scansione di tutti i triggers -----------------------

	for (i = 0;i< *pTotRooms; i++) {
		// cercare in stanza con indice i
		if (pVetRooms[i].TestEmptySlot == 0) {
			// cercare in objandtriger
			TotRimuovi=0;
			for (j=0;j<pVetRooms[i].N_ObjAndTriggers;j++) {
				// trovare indice a 
				Indice= pVetRooms[i].VetObjAndTriggerCodes[j];
					

				if (pVetBaseOggetti[Indice].TypeObject == 0x10) {
					// trovato un trigger
					// ottenere indice a dati di trigger
					IndiceBeta = pVetBaseOggetti[Indice].SlotIndex;
					// ora in pVetTriggers[IndiceBeta] ci sono i dati del trigger attuale
					switch (TrtTipo) {
					case TRT_REMOVE_TRIGGER_WITH_ID:
						IdPlugin=pVetTriggers[IndiceBeta].PluginId;
						if (IdPlugin == 0) break;
						if (IdPlugin != MainId) break;
						
						// devo rimuovere il trigger tra gli oggetti di questa stanza
						// pero' farlo nel mezzo del loop e' un casino.
						// per cui: prima salvo in un vettore l'indice da eliminare
						// e poi al termine del loop li rimuovo tutti
						VetRimuovi[TotRimuovi]= Indice;
						TotRimuovi++;
						break;

					case TRT_CHANGE_TRIGGER_ID:
						IdPlugin=pVetTriggers[IndiceBeta].PluginId;
						if (IdPlugin == 0) break;
						if (IdPlugin == MainId) {
							// sostituirlo con ReplaceId
							pVetTriggers[IndiceBeta].PluginId = ReplaceId;
						}
						break;

					case TRT_REFRESH_USED_TRIGGERS:
						// aggiorna il numero "TotUsato" di tutti i trigger di qualche plugin
						IdPlugin=pVetTriggers[IndiceBeta].PluginId;
						if (IdPlugin == 0) break;
						IndicePlugin=GetPluginIndexFromId(IdPlugin);
						if (IndicePlugin==-1) break;
						GlobNgle.VetPluginNgle[IndicePlugin].TotUsato++;
						break;

					case TRT_LIST_UNHANDLED_TRIGGERS:
						// salva tutti i dati di triggers con id di plugin mancante
						if (TotTriggerTrovati >= 200) break;
						IdPlugin=pVetTriggers[IndiceBeta].PluginId;
						if (IdPlugin == 0) break;
						if (GetPluginIndexFromId(IdPlugin) != -1) break;
						// trovato un trigger non gestito
						VetTriggerTrovati[TotTriggerTrovati].Id = IdPlugin;
						VetTriggerTrovati[TotTriggerTrovati].Room = i;
						// scoprire la posizione dei settori
						VetTriggerTrovati[TotTriggerTrovati].XOrg = pVetBaseOggetti[Indice].XOrgBlock;
						VetTriggerTrovati[TotTriggerTrovati].ZOrg = pVetBaseOggetti[Indice].ZOrgBlock;
						// creare descrizione sintetica: Flippefct 34 / Action 52 / Condition 16
						if (pVetTriggers[IndiceBeta].TriggerAttivazione  == 12) {
							// e' una condizione
							pTipoTrigger = "CONDITION";
							NumeroTrigger= pVetTriggers[IndiceBeta].Timer;
						}else {
							// vedere se e' flipeffect o action
							if (pVetTriggers[IndiceBeta].TriggerCosaAttivare == 9) {
								pTipoTrigger = "FLIPEFFECT";
								NumeroTrigger= pVetTriggers[IndiceBeta].OggettoDaTriggerare;
							}else {
								pTipoTrigger = "ACTION";
								NumeroTrigger= pVetTriggers[IndiceBeta].Timer;
							}
						}
						sprintf(VetTriggerTrovati[TotTriggerTrovati].Descrizione, "%s %d", 
									pTipoTrigger, NumeroTrigger);
						TotTriggerTrovati++;

					}

				}
			}

			if (TrtTipo == TRT_REMOVE_TRIGGER_WITH_ID && TotRimuovi > 0) {
				// rimuovere i trigger di questa stanza da eliminare
				for (j=0;j<TotRimuovi;j++) {
					RemoveObject(VetRimuovi[j], i);
				}

			}
		}
	}

	// ---------------- esecuzione ----------------------------
	switch (TrtTipo) {
	case TRT_LIST_UNHANDLED_TRIGGERS:
		if (TotTriggerTrovati==0) {
			MioMessageBox("No unhandled trigger it has been found", "Good News");
			break;
		}
		sprintf(NomeFile, "%s\\List_unhandled_triggers.txt", MyTempFolder);
		pFile=fopen(NomeFile, "wb");
		fprintf(pFile, "TRIGGER           ID  Room  X  Z\r\n");
		fprintf(pFile, "---------------------------------\r\n");
		for (i=0;i<TotTriggerTrovati;i++) {
			//  FLIPEFFECT 32 Id Room X Y
			fprintf(pFile, "%-15s  %3d  %3d  %2d %2d\r\n",
				VetTriggerTrovati[i].Descrizione, 
				VetTriggerTrovati[i].Id, 
				VetTriggerTrovati[i].Room,
				VetTriggerTrovati[i].XOrg,
				VetTriggerTrovati[i].ZOrg);
		}
		fprintf(pFile, "---------------------------------\r\n");
		fclose(pFile);
		// aprire indirizzo dove c'e' file help trle
		ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);
		break;
	}
}
// fa scansione di tutte le room per verificare quanti triggers sono usati nella mappa da ogni plugin
void RefreshUsedTriggersByPlugins(void)
{

	CercaTriggers(TRT_REFRESH_USED_TRIGGERS, 0,0);

}
// cerca trigger con un id plugin non presente nella tabell plugn
void Menu_ListUnhandledTriggers(void *pPunta)
{
	CercaTriggers(TRT_LIST_UNHANDLED_TRIGGERS, 0, 0);
}

// mostra descrizione triggers di tutti i plugin (documento di testo)
void Menu_ShowTriggerListDiPlugin(void *pPunta)
{
	char *pNomePlugin;
	int IdPlugin;
	int i;
	StrNGConstants  *pBase;
	FILE *pFile;
	char NomeFile[256];
	int IndiceSezione;
	StrRecordSezione *pSezione;

	if (GlobNgle.TotPluginNgle==0) {
		MioMessageBox("Currently there is NO mounted plugin","Warning");
		return;
	}

	if (GlobNgle.TotPluginNgle > 1) {
		pNomePlugin=InputTextBox("Type the name of plugin whose you wish having the trigger list\r\n(Don't type the \".dll\" extension)", 
				"Choose Plugin", "Plugin",TP_CENTRALE);

		if (pNomePlugin==NULL) return;
	}else {
		// deve esserci un solo plugin: usare quello
		pNomePlugin = GlobNgle.VetPluginNgle[0].Nome;
	}

	// trovare id
	IdPlugin=-1;

	for (i=0;i<GlobNgle.TotPluginNgle;i++) {
		if (strcmpi(pNomePlugin, GlobNgle.VetPluginNgle[i].Nome) ==0) {
			IdPlugin=GlobNgle.VetPluginNgle[i].PluginId;
			break;
		}
	}

	if (IdPlugin==-1) {
		sprintf(BufferLog, "It has been NOT found any plugin with \"%s\" name", pNomePlugin);
		MioMessageBox(BufferLog, "Error");
		return;
	}

	pBase = TrovaBaseCostanti(IdPlugin);

	if (pBase==NULL) {
		sprintf(BufferLog, "Internal ERROR: cann't find constant data for plugin with id = %d", IdPlugin);
		MioMessageBox(BufferLog, "Internal Error");
		return;
	}

	// stampare il prospetto in un file di testo
	sprintf(NomeFile, "%s\\TriggerListFor_%s.txt", MyTempFolder, pNomePlugin);
	
	pFile=fopen(NomeFile, "wb");

	fprintf(pFile, "TRIGGERS OF \"%s\" PLUGIN\r\n", pNomePlugin);
	fprintf(pFile, "-----------------------------------------\r\n");
	fprintf(pFile, "\r\n");

	// ---------- flipeffects ----------------------------
	fprintf(pFile, "FLIPEFFECTS\r\n");
	fprintf(pFile, "-----------\r\n");
	// trovare sezione di flipeffect: <START_TRIGGERWHAT_9_O_H>
	IndiceSezione=-1;
	for (i=0;i<pBase->TotSezioni;i++) {
		if (pBase->pVetSezioni[i].TipoSezione  == SEZ_TRIGGERWHAT &&
			pBase->pVetSezioni[i].NumeroSezione == 9) {
			IndiceSezione=i;
			break;
		}
	}

	if (IndiceSezione != -1) {
		pSezione= &pBase->pVetSezioni[IndiceSezione];

		// visualizzare tutti i flipeffects
		for (i=0;i<pSezione->TotValori ;i++) {
			fprintf(pFile, "%4d: %s\r\n", 
				pSezione->pVetValori[i].Numero, pSezione->pVetValori[i].pDescrizione);
		}
			
	}
	fprintf(pFile, "\r\n");

	// ----------------- ACTIONS ---------------------------
	fprintf(pFile, "ACTIONS\r\n");
	fprintf(pFile, "-------\r\n");

	// trovare sezione per aciotns: START_TRIGGERWHAT_11_T_H>
	IndiceSezione=-1;
	for (i=0;i<pBase->TotSezioni;i++) {
		if (pBase->pVetSezioni[i].TipoSezione  == SEZ_TRIGGERWHAT &&
			pBase->pVetSezioni[i].NumeroSezione == 11) {
			IndiceSezione=i;
			break;
		}
	}


	if (IndiceSezione != -1) {
		pSezione= &pBase->pVetSezioni[IndiceSezione];

		// visualizzare tutti le actions
		for (i=0;i<pSezione->TotValori ;i++) {
			fprintf(pFile, "%4d: %s\r\n", 
				pSezione->pVetValori[i].Numero, pSezione->pVetValori[i].pDescrizione);
		}
			
	}
	fprintf(pFile, "\r\n");

	// ---------------- CONDITIONS ---------------------------
	fprintf(pFile, "CONDITIONS\r\n");
	fprintf(pFile, "----------\r\n");
	// trovare sezione per condition: <START_TRIGGERTYPE_12_T_H>
	IndiceSezione=-1;
	for (i=0;i<pBase->TotSezioni;i++) {
		if (pBase->pVetSezioni[i].TipoSezione  == SEZ_TRIGGERTYPE &&
			pBase->pVetSezioni[i].NumeroSezione == 12) {
			IndiceSezione=i;
			break;
		}
	}


	if (IndiceSezione != -1) {
		pSezione= &pBase->pVetSezioni[IndiceSezione];

		// visualizzare tutti le actions
		for (i=0;i<pSezione->TotValori ;i++) {
			fprintf(pFile, "%4d: %s\r\n", 
				pSezione->pVetValori[i].Numero, pSezione->pVetValori[i].pDescrizione);
		}
			
	}
	fprintf(pFile, "\r\n");
	fclose(pFile);
	ShellExecute(NULL,"open",NomeFile,"",SoloDir(NomeFile),SW_SHOW);
}

// consente di modificare id di plugin

void Menu_ChangePluginId(void *pPunta)
{
	WORD IdPlugin;
	char *pNomePlugin;
	char *pStrNumero;
	int NuovoId;
	bool TestErrore;
	int i;
	HWND *pMiaWind = (HWND*) 0x46E559;
	int PluginIndex;
	bool TestOk;

	if (GlobNgle.TotPluginNgle==0) {
		MioMessageBox("Currently there is NO mounted plugin","Warning");
		return;
	}

	if (GlobNgle.TotPluginNgle > 1) {
		pNomePlugin=InputTextBox("Type the name of plugin whose you wish modifying the ID\r\n(Don't type the \".dll\" extension)", 
				"Choose Plugin", "Plugin",TP_CENTRALE);

		if (pNomePlugin==NULL) return;

	}else {
		// deve esserci un solo plugin: usare quello
		pNomePlugin = GlobNgle.VetPluginNgle[0].Nome;
	
	}

	// trovare id e indice di plugin
	IdPlugin=-1;

	for (i=0;i<GlobNgle.TotPluginNgle;i++) {
		if (strcmpi(pNomePlugin, GlobNgle.VetPluginNgle[i].Nome) ==0) {
			PluginIndex= i;
			IdPlugin=GlobNgle.VetPluginNgle[i].PluginId;
			break;
		}
	}
	if (IdPlugin == -1) {
		sprintf(BufferLog, "ERROR: cann't find the \"%s\" plugin, between currently mounted plugins in this project",
			pNomePlugin);
		MioMessageBox(BufferLog, "Error");
		return;
	}	
	sprintf(BufferLog, "Type the new ID to assign to \"%s\" plugin with current id = %d",
		pNomePlugin, IdPlugin);
	pStrNumero=InputTextBox(BufferLog, "Change plugin ID", "", TP_CENTRALE);
	if (pStrNumero==NULL) return;

	NuovoId=GetNumero(pStrNumero, &TestErrore);

	if (TestErrore==true) {
		sprintf(BufferLog, "ERROR: The typed text \"%s\" is not a valid decimal number", pStrNumero);
		MioMessageBox(BufferLog, "Error");
		return;
	}


	// verificare che sia un id valido
	if (NuovoId <1 || NuovoId > 255) {
		MioMessageBox("Invalid value. The number you typed is outside of valid range: 1/255", "Error");
		return;
	}

	// verificare se l'id scelto non sia gia' presente 
	for (i=0;i<GlobNgle.TotPluginNgle;i++) {
		if (GlobNgle.VetPluginNgle[i].PluginId == NuovoId) {
			sprintf(BufferLog, "Warning: the ID you chose it is already used by \"%s\" plugin.\r\nDo you want anyway set the new id %d for \"%s\" plugin?",
				GlobNgle.VetPluginNgle[i].Nome , NuovoId, pNomePlugin);

			if (MessageBox(*pMiaWind,BufferLog,"ID conflict",MB_YESNO) == IDNO) return;


		}
	}
	TestOk=false;
	// ora chiedere se modificare id in plugin table
	sprintf(BufferLog, "First Change.\r\nDo you want to change, in PLUGIN TABLE, the id of \"%s\" plugin with new Id value %d?",
		pNomePlugin, NuovoId);

	if (MessageBox(*pMiaWind, BufferLog, "Changes in Plugin Table",MB_YESNO) == IDYES) {
		GlobNgle.VetPluginNgle[PluginIndex].PluginId=NuovoId;
		TestOk=true;
	}

	// ora chiedere se modificare anche i triggers
	sprintf(BufferLog, "Second Change.\r\nDo you want to change, in all TRIGGERS in the level, the id of \"%s\" plugin with new Id value %d?",
		pNomePlugin, NuovoId);

	if (MessageBox(*pMiaWind, BufferLog, "Changes in the Triggers",MB_YESNO) == IDYES) {
		CercaTriggers(TRT_CHANGE_TRIGGER_ID, IdPlugin, NuovoId);
		TestOk=true;
	}

	if (TestOk) MioMessageBox("All changes have been performed", "Change of Plugin's ID");

}

void Menu_RemoveTriggersOfPlugin(void *pPunta)
{
	char *pNomePlugin;
	int IdPlugin;
	bool TestErrore;
	HWND *pMiaWind = (HWND*) 0x46E559;
	int PluginIndex;
	int j;
	int i;

	pNomePlugin=InputTextBox("Type, or the plugin NAME, or the Plugin ID of triggers to remove", 
			"Choose Owner of triggers", "Plugin",TP_CENTRALE);

	if (pNomePlugin==NULL) return;
	if (strlen(pNomePlugin) < 6) {
		// dovrebbe essere un numero
		IdPlugin=GetNumero(pNomePlugin, &TestErrore);
		if (TestErrore==true) {
			sprintf(BufferLog, "ERROR: The typed text \"%s\" is not a valid decimal number", pNomePlugin);
			MioMessageBox(BufferLog, "Error");
			return;
		}
	}else {

		// trovare id di nome plugin fornito
		IdPlugin=-1;

		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			if (strcmpi(pNomePlugin, GlobNgle.VetPluginNgle[i].Nome) ==0) {

				IdPlugin=GlobNgle.VetPluginNgle[i].PluginId;
				break;
			}
		}
		if (IdPlugin == -1) {
			sprintf(BufferLog, "ERROR: cann't find the \"%s\" plugin, between currently mounted plugins in this project",
				pNomePlugin);
			MioMessageBox(BufferLog, "Error");
			return;
		}
	}
	
	// eliminare triggers
	CercaTriggers(TRT_REMOVE_TRIGGER_WITH_ID, IdPlugin,0);

	// vedere se c'e' anche un plugin con quell'id

	PluginIndex=GetPluginIndexFromId(IdPlugin);
	if (PluginIndex != -1) {

		// chiedere se rimuovere anche pluging da plugin table
		pNomePlugin = GlobNgle.VetPluginNgle[PluginIndex].Nome;
		sprintf(BufferLog, "Do you want remove also the \"%s\" plugin (with id=%d) from mounted plugin table?",
			pNomePlugin, IdPlugin);

		if (MessageBox(*pMiaWind,BufferLog,"Confirm for elimination",MB_YESNO) == IDYES) {
			j=PluginIndex;
			// rimuovere record di plugin con indeice PluginIndex
			for (i=j;i<GlobNgle.TotPluginNgle-1;i++) {
				GlobNgle.VetPluginNgle[i] = GlobNgle.VetPluginNgle[i+1];
			}
			GlobNgle.TotPluginNgle--;

		}else {
			// lasciare plugin nella tabella, pero' impostare a zero i trigger usati
			GlobNgle.VetPluginNgle[PluginIndex].TotUsato=0;
		}
	}
	Draw3DWindow();
	MioMessageBox("Trigger removal has been completed","Ngle Editor");
}

// visualizza o scaricare next generation help
void Menu_ShowNGHelp(void *pPunta)
{
	char NomeHelp[256];


	sprintf(NomeHelp,"%s\\help\\next_generation.htm",Dir_Trle);

	if (EsisteFile(NomeHelp)==false) {

		if (MostraMessageBoxNGLE("Cann't find file 'start.htm' in [trle\\help] folder.\r\n\r\nDo you want download now the Next Generation Help? (1 Mb)",
				0x0f)== 0) return;
		MostraMessageBoxNGLE("When download is complete, decompress zip file directly in trle folder and it will be created a new \"HELP\" sub-folder with help files",0);

		// aprire indirizzo dove c'e' file help trle
		ShellExecute(NULL,"open","http://www.TrLevelManager.eu/download/trle_help.zip",
				"","",SW_SHOW);

		return;

		
	}
	ShellExecute(NULL,"open",NomeHelp,"",SoloDir(NomeHelp),SW_SHOW);
	
}


// salva con nome progressivo screenshot
void GosubSalvaScreenShot(int TipoScreenShot)
{
	DWORD *pTestPreview = (DWORD*) 0x583BF8;
	char *pParteShot;
	bool	TestMessaggi;
	char *pVetNomiShot[]={"Screen", "2D_Panel",  "3D_Panel", 
						"Preview", "RPreview", "Textures",
						"InfoRoom", "LightPanel"};
	char NomeFile[256];
	char *pNome,*pDirSrc;
	int	i;

	pParteShot = pVetNomiShot[TipoScreenShot];

	if (TipoScreenShot == TSS_PREVIEW || TipoScreenShot== TSS_PREVIEW_RGB) {
		TestMessaggi=false;
		if ((*pTestPreview) == 0) {
			MostraMessageBoxNGLE("No preview in progress",3);
			return;
		}
	}
	else {
		if ((*pTestPreview) != 0) 
			TestMessaggi=false;
		else
			TestMessaggi=true;
	}
	i = 1;
	pDirSrc = Dir_Trle;
	do {
		sprintf(NomeFile,"%s\\%s_%04d.bmp", pDirSrc, pParteShot,i);
		i++;
	} while (EsisteFile(NomeFile)==true);

	if (TestMessaggi==false) {
		pNome = NomeFile;
	}else
	{
		sprintf(BufferLog,"Save screenshot (%s type) with following name. (It will be saved in TRLE folder)\r\n\r\nRemark: Following name assures to avoid overwritting of previous screnshot files", 
				pParteShot);
		pNome = InputTextBox(BufferLog, "Save Screenshot", 
								SoloNomeSenzaExt(NomeFile), TP_LUCI);
		if (pNome==NULL) return;
		sprintf(NomeFile,"%s\\%s.bmp",pDirSrc,pNome);
		pNome = NomeFile;
	}
	SalvaScreenShot(pNome, TipoScreenShot);
	if (TestMessaggi==false) MessageBeep(-1);


}
void Menu_ShotFullScreen(void *pPunta)
{

	GosubSalvaScreenShot(TSS_FULLSCREEN);

}
void Menu_ShotPanel2d(void *pPunta)
{
	GosubSalvaScreenShot(TSS_PANEL2D);
}
void Menu_ShotPanel3d(void *pPunta)
{
	GosubSalvaScreenShot(TSS_PANEL3D);

}
void Menu_ShotPreview(void *pPunta)
{
	GosubSalvaScreenShot(TSS_PREVIEW);
}	
void Menu_ShotPreviewRGB(void *pPunta)
{
	GosubSalvaScreenShot(TSS_PREVIEW_RGB);
}

void Menu_ShotTextures(void *pPunta)
{
	GosubSalvaScreenShot(TSS_TEXTURES);
}

void Menu_ShotInfoRoom(void *pPunta)
{
	GosubSalvaScreenShot(TSS_INFOROOM);
}
void Menu_ShotLights(void *pPunta)
{
	GosubSalvaScreenShot(TSS_LIGHTS);
}
void Menu_ShowButtonsWindowsHelp(void *pPunta)
{
	char NomeFile[256];

	sprintf(NomeFile, "%s\\my_help_ButtonsWindow.txt", Dir_Trle);
	if (EsisteFile(NomeFile)==false) {
		sprintf(NomeFile, "%s\\help_ButtonsWindow.txt", Dir_Trle);
	}

	if (EsisteFile(NomeFile)==false) {
		sprintf(BufferLog, "ERROR: missing \"help_ButtonsWindow.txt\" file in trle folder. Not found file: %s",
			NomeFile);
		MioMessageBox(BufferLog, "Error");
		return;
	}

	ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);

}

void Menu_ShowKeyBoardCommands(void *pPunta)
{

	char NomeFile[256];
	FILE *pFile;
	
	sprintf(NomeFile, "%s\\KeyBoardCommands.txt", MyTempFolder);

	pFile = fopen(NomeFile,"wb");
	
	fprintf(pFile,"KEYBOARD COMMMANDS OF NEXT GENERATION WINROOMEDIT v%s\r\n", 
					MexVersione);
	fprintf(pFile,"----------------------------------------------------------------------------\r\n");
	fprintf(pFile,"left SHIFT    Slow down rotations and zoom in 3d view panel.\r\n");
	fprintf(pFile,"              Note: you have to keep CONTINUOSLY down the key to get slowdown function\r\n");

	fprintf(pFile,"CTRL B        Enable/Disable big texture mode. Like [Big Texture] button.\r\n");
	fprintf(pFile,"SHIFT C       Center the rotation pivot at centre of room. (Like menu command [Room->Centre])\r\n");
	fprintf(pFile,"CTRL F        Rotate selected object. Like [Rotate Object] button.\r\n\r\n");

	fprintf(pFile,"SHIFT F1      Show infos about current project (Wad path and tga path)\r\n");
	fprintf(pFile,"SHIFT F2      Invoke Settings window to customize NGLE parameters\r\n\r\n");

	fprintf(pFile,"SHIFT F3      Texturize missing triangular floor faces\r\n");
	fprintf(pFile,"SHIFT F4      Texturize missing triangular ceiling faces\r\n\r\n");


	fprintf(pFile,"SHIFT F5      Set size of 2d selection\r\n\r\n");

	fprintf(pFile,"CTRL F1       Floor smooth slope\r\n");
	fprintf(pFile,"CTRL F2       Floor stepped slope\r\n");
	fprintf(pFile,"CTRL F3       Ceiling smooth slope\r\n");
	fprintf(pFile,"CTRL F4       Ceiling stepped slope\r\n\r\n");

	fprintf(pFile,"CTRL F5       Floor Pyramid\r\n");
	fprintf(pFile,"CTRL F6       Inverse Floor Pyramid\r\n");
	fprintf(pFile,"CTRL F7       Ceiling Pyramid\r\n");
	fprintf(pFile,"CTRL F8       Inverse Ceiling Pyramid\r\n\r\n");

	fprintf(pFile,"ALT F9        Clear Floor Selected Textures\r\n");
	fprintf(pFile,"ALT F10       Clear Ceiling Selected Textures\r\n\r\n");
	fprintf(pFile,"F11           Create grid in selected wall squares. Like [Grid selected wall] menu\r\n");
	fprintf(pFile,"CTRL F11      Remove all editing from selected wall\r\n");
	fprintf(pFile,"F12           Create grid in all wall squares. Like [Grid all walls] menu\r\n");
	fprintf(pFile,"CTRL F12      Remove all wall grids\r\n\r\n");

	fprintf(pFile,"ALT 1         Random smooth floor up\r\n");
	fprintf(pFile,"ALT 2         Random smooth floor down\r\n");
	fprintf(pFile,"ALT 3         Random smooth ceiling up\r\n");
	fprintf(pFile,"ALT 4         Random smooth ceiling down\r\n\r\n");

	fprintf(pFile,"ALT 5         Save screenshot of room edit window\r\n");
	fprintf(pFile,"ALT 6         Save screenshot of 2D panel\r\n");
	fprintf(pFile,"ALT 7         Save screenshot of 3D panel\r\n");
	fprintf(pFile,"ALT 8         Save screenshot of preview area\r\n");
	fprintf(pFile,"ALT 9         Save RGB screenshot of preview area\r\n");
	fprintf(pFile,"ALT 0         Save screenshot of texture panel\r\n");
	fprintf(pFile,"ALT F11       Save screenshot of Info Room Panel\r\n");
	fprintf(pFile,"ALT F12       Save screenshot of Light Panel\r\n\r\n");

	fprintf(pFile, "ALT T        Reload current TGA file from disk\r\n\r\n");
	
	fprintf(pFile,"CTRL 0        Show/Restore the Buttons Window '0' = digit zero\r\n\r\n");
	fprintf(pFile,"CTRL 1        Bend Floor\r\n");
	fprintf(pFile,"CTRL 2        Inverse Bend Floor\r\n");
	fprintf(pFile,"CTRL 3        Bend Ceiling\r\n");
	fprintf(pFile,"CTRL 4        Inverse Bend Ceiling\r\n");
	fprintf(pFile,"CTRL 5        Dome Floor\r\n");
	fprintf(pFile,"CTRL 6        Inverse Dome Floor\r\n");
	fprintf(pFile,"CTRL 7        Dome Ceiling\r\n");
	fprintf(pFile,"CTRL 8        Inverse Dome Ceiling\r\n\r\n");

	fprintf(pFile,"CTRL 9        Open tga file with default graphical program set in Windows\r\n\r\n");

	fprintf(pFile,"G             Goto previous edited room. Like [Go back old room] button.\r\n");
	fprintf(pFile,"CTRL G        Goto following edited room.\r\n");
	fprintf(pFile,"CTRL H        Move Lara to selected square in current room. Like [Move Lara Here] button\r\n");
	fprintf(pFile,"CTRL+SHIFT+I  Interrupt program with simulated crash, to use when ngle freezes to can save backup and get crash report\r\n");
	fprintf(pFile,"CTRL J        Swap infos showed in Title bar of ngle: you can show current Project infos, or current room infos\r\n");
	fprintf(pFile,"CTRL L        Load last projects. Show list of last projects. Like [Load Last Projects] button\r\n");
	fprintf(pFile,"CTRL M        Minimize main window\r\n");
	fprintf(pFile,"M             Mark current room as 'edited room' to reach it using G command\r\n");
	fprintf(pFile,"CTRL O        Reload current Object wad file. ('O' = literal)\r\n");
	fprintf(pFile,"SHIFT O       Permit to choose a new Object wad file. Like old (and removed) button [Load Objects]\r\n");
	fprintf(pFile,"CTRL S        Write in log file mexage '--- CTRL S --' for reference\r\n");
	fprintf(pFile,"CTRL T        Change current triangular texture selection\r\n");
	fprintf(pFile,"U             Unmark current room to remove it from 'edited room' list\r\n");
	fprintf(pFile,"CTRL ALT SHIFT  Increase number of rooms\r\n");
	fclose(pFile);

	ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);

}
void Menu_UpdateRoomEditor(void *pPunta)
{
	ShellExecute(NULL,"open","http://www.TrLevelManager.eu/ng.htm","","",SW_SHOW);

}
// modifica selezione2d restituendo solo il rettangolo effettivo a partire 
// dall'inizio di stanza attuale
// scrive anche in SizeRoomX SizeRoomY la dimensione della stanza
void AdattaSelezione(RECT *pNewRect, int*SizeRoomX, int* SizeRoomY)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;

	
	pVetRooms = *p2VetRooms;
	pNewRect->left  = pRectSelezione2d->left - 
							pVetRooms[*pIndiceRoomAttuale].XDisplayOffset;
	pNewRect->top = pRectSelezione2d->top - 
							pVetRooms[*pIndiceRoomAttuale].ZDisaplyOffset;
	pNewRect->right = pRectSelezione2d->right;
	pNewRect->bottom = pRectSelezione2d->bottom;

	*SizeRoomX = pVetRooms[*pIndiceRoomAttuale].XSizeBlocks;
	*SizeRoomY = pVetRooms[*pIndiceRoomAttuale].ZSizeBlocks;


}

// inserisce in variabile globale MiaSelezione le caselle selezionate
// partendo dall'origine della room
// se restituisce false allora non c'e' una selezione 2d oppure
// la selezione e' inferiore a LatoMinimo
bool PreparaMiaSelezione(int LatoMinimo,bool TestNienteWall)
{
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int SizeX,SizeY;
	int MaxX,MaxZ;

	if ((*pTestEsisteSelezioneBox) ==0)  {
		MostraMessageBoxNGLE("No selected square",3);
		return false;
	}
	AdattaSelezione(&DatiShape.MiaSelezione, &DatiShape.RoomSizeX , 
				&DatiShape.RoomSizeY);
	SizeX = DatiShape.MiaSelezione.right;
	SizeY = DatiShape.MiaSelezione.bottom;
	MaxX = DatiShape.MiaSelezione.left + SizeX;
	MaxZ = DatiShape.MiaSelezione.top  + SizeY;
	
	if (SizeX < LatoMinimo && SizeY < LatoMinimo) {
		sprintf(BufferLog,"ERROR: selected area is too little. Minimum required size is: (%d x %d)",
			LatoMinimo,LatoMinimo);
		MostraMessageBoxNGLE(BufferLog,3);
		return false;
	}

	if (DatiShape.MiaSelezione.left < 0 || DatiShape.MiaSelezione.top < 0 ||
		MaxX > DatiShape.RoomSizeX || MaxZ > DatiShape.RoomSizeY) {
		return false;
	}


	if (TestNienteWall == true) {
		if (DatiShape.MiaSelezione.left==0 || DatiShape.MiaSelezione.top==0 ||
			MaxX == DatiShape.RoomSizeX  ||
			MaxZ == DatiShape.RoomSizeY ){
			MostraMessageBoxNGLE("ERROR: only floor sectors are permitted. Please don't select wall sectors for this command.",3);
			return false;
		}
	}



	return true;

}

// restituisce indice speculare di angolo corner a seconda se e' attivo florr
// o ceiling
// ma in realta' (se non ricordo male) l'inversione deve avvenire
// solo su asse x, quindi il 1 diventa 3 e 0 diventa 2
int Ruota(int Indice)
{
	if (DatiShape.TestCeiling==false) return Indice;
	// e' ceiling invertire specularment.
	Indice = 3-Indice;
	return Indice;

}

// verifica se i corner di input creano un triangolo nel blocco oppure no
// Se (Angolo(0) - Angolo(3))  ==  (Angolo(1) - Angolo(2)) allora non ci sono triangoli
bool IsTriangle(char *pVetCorners)
{
	if ((pVetCorners[Ruota(0)] - pVetCorners[Ruota(3)]) == 
		(pVetCorners[Ruota(1)] - pVetCorners[Ruota(2)])) return false;
	return true;

}
// visualizza offset di campo struttura nel formato:
// OffsetMemoria:OffsetRelativo
// invia a InviaLog
void ShowOffsetStruttura(void *pBaseStruttura, void *pCampoNow)
{
	DWORD Base,Campo;

	Base = (DWORD) pBaseStruttura;
	Campo = (DWORD) pCampoNow;

	sprintf(BufferLog,"%08X:%04X", Campo, Campo-Base);
	InviaLog(BufferLog);
}
// ruota le textue di zona selezionata 
// TipoZona:
//		tz_Floor
//      tz_Ceiling
//      tz_Walls

void RuotaTextures(int TipoZona, int NRotazioni)
{

	int SizeX,SizeY;
	int x,y;
	StrGridXRoom *pGriglia;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int i;
	int MaxY,MaxX;
	static int MaxFloor,MinFloor,MaxCeiling,MinCeiling;
	static short MaxClick;
	static int VetIndiciFloor[] = {0, 8, -1};
	static int VetIndiciCeiling[] = {1, 9, -1};
	static int VetIndiciWall[] = {2,3,4,5,6,7, 10,11,12,13,-1};
	int *pVetIndici;
	int j;
	int IndiceRoom;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int n;

	pVetRooms = *p2VetRooms;
	IndiceRoom = *pIndiceRoomAttuale;

	SizeX = pVetRooms[IndiceRoom].XSizeBlocks;
	SizeY = pVetRooms[IndiceRoom].ZSizeBlocks;
	MaxX= DatiShape.MiniSel.left + DatiShape.MiniSel.right;
	MaxY = DatiShape.MiniSel.top + DatiShape.MiniSel.bottom;
	// controllare che la selezione non comprenda punti esterni alla stanza
	// attuale
	if (DatiShape.MiniSel.left < 0 || DatiShape.MiniSel.top < 0 ||
		MaxX > SizeX || MaxY > SizeY) {
		// zona e' esterna
		MioMessageBox("ERROR: selection extern of current room", "Error");


		return;
	}
	switch (TipoZona) {
	case TZ_FLOOR:
		pVetIndici = &VetIndiciFloor[0];
		break;
	case TZ_CEILING:
		pVetIndici = &VetIndiciCeiling[0];
		break;
	case TZ_WALLS:
		pVetIndici = &VetIndiciWall[0];
		break;
	}

	for (y=DatiShape.MiniSel.top;y<MaxY;y++) {
		for (x=DatiShape.MiniSel.left;x<MaxX;x++) {
			pGriglia = &pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX];
			i = 0;
			while (pVetIndici[i] != -1) {
				j = pVetIndici[i++];
				if (pGriglia->TextureInfos[j].Status == 7) {
					n = NRotazioni;
					if (n==4) n =Casuale(4);

					pGriglia->TextureInfos[j].Rotation += n;
					if (pGriglia->TextureInfos[j].Rotation > 3) {
						pGriglia->TextureInfos[j].Rotation -= 4;
					}
				}
			}
		
		}
	}
}

void Menu_RuotaTexture(void *pPunta) 
{
	int TipoZona;
	int NumeroRotazioni;
	char *pChar;

	// qui bisogna chidere il tipo di superfice e il numero di rotazioni
	pChar = InputTextBox("Choose what type of textures to turn:\r\n\r\nF = Floor\r\nC = Ceiling\r\nW = Walls\r\n\r\nType: F, C or W", 
		"Turn Selected Textures", "F", TP_CENTRALE);
	if (pChar == NULL) return;
	switch (pChar[0]) {
	case  'F':
	case 'f':
		TipoZona = TZ_FLOOR;
		break;
	case  'C':
	case 'c':
		TipoZona = TZ_CEILING;
		break;
	case 'W':
	case 'w':
		TipoZona = TZ_WALLS;
		break;
	default:
		MioMessageBox("Wrong type!","ERROR");
		return;
	}

	pChar = InputTextBox("Type the number of rotation to apply to selected textures. Each rotation is 90 degrees in clockwise direction\r\nRemark: if you type '4' it will be applied different turning for each texture in radom way (useful for the water surface)\r\n\r\nType: 1, 2, 3, or 4 for random facing", 
			"Number of rotations", "1", TP_CENTRALE);

	if (pChar == NULL) return;
	NumeroRotazioni = pChar[0] - '0';
	if (NumeroRotazioni < 1 || NumeroRotazioni > 4) {
		MioMessageBox("Wrong number of rotations. Valid range: 1 - 4", "ERROR");
		return;
	}

	if (IniziaShape(true,false,1,false)==false) return;
	RuotaTextures(TipoZona, NumeroRotazioni);
	TerminaShape( true,false);
}
// modifica Settori di DatiShape.MiniSel sulla base del tipo di operazione:
// #define FTO_SET_NONE 0x00  // nullo per resettare record
// #define FTO_SET_FLOOR 0x01
// #define FTO_SET_CEILING 0x02
// ... molti altri nel file .h ....
// gli argomenti successivi dipendono dal tipo di operazione
// se ci sono piu' flag contemporanei ogni argomento e' associato nell'ordine
// qua sopra ai valori previsti
// se TestResetRecord == true prima di effettuare le modifiche 
// azzera tutti tutti dati di altezza corner e wall della struttura
// e imposta DatiShape.BaseFloor e BaseCeiling (valori globali)
// se operazione e' di richiesta dati restiuisce un puntatore void* ai dati richisti
// nota: in caso di richiesta dati verranno restiuiti sempre i dati della
// prima casella della MiniSel e poi esce
void* ModificaClickSettore(int TipoOperazione, bool TestResetRecord, ... )
{

	int SizeX,SizeY;
	int x,y;
	va_list marker;
	StrGridXRoom *pGriglia;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int i;
	int MaxY,MaxX;
	int IndiceTex;
	int Orientamento;
	int  n;
	static int MaxFloor,MinFloor,MaxCeiling,MinCeiling;
	static short MaxClick;
	int FirstRotation, FirstTriangle;
	bool TestTriangoloTexture;
	int FirstIndex,LastIndex;
	bool TestFloor;
	bool TestWall;
		//                         ---- FLOOR _----------------   -- CEILING ------- 
	static int VetIndiciFloor[] = {0,8,0xa, 0xc, 2 , 5, 4, 7,     1, 9, 0xb, 0xd, 3, 6, 4, 7 };
		//                         0 1  2    3   4   5  6  7      8  9   10  11   12 13 14 15 

	int Scarto;
	int j;
	int IndiceStart,IndiceEnd;
	int Numero,Valore;
	int IndiceRoom;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;

	pVetRooms = *p2VetRooms;
	IndiceRoom = *pIndiceRoomAttuale;

	SizeX = pVetRooms[IndiceRoom].XSizeBlocks;
	SizeY = pVetRooms[IndiceRoom].ZSizeBlocks;
	MaxX= DatiShape.MiniSel.left + DatiShape.MiniSel.right;
	MaxY = DatiShape.MiniSel.top + DatiShape.MiniSel.bottom;
	// controllare che la selezione non comprenda punti esterni alla stanza
	// attuale
	if (DatiShape.MiniSel.left < 0 || DatiShape.MiniSel.top < 0 ||
		MaxX > SizeX || MaxY > SizeY) {
		// zona e' esterna
		InviaLog("WARNING: selection extern of current room in ModificaClickSettore()");

		for (i=0;i<80;i++) {
			BufferLog[i] = 0;
		}
		return BufferLog;
	}
	
	if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_FLOOR) {
		CopiaRect(&BaseRotazioni.SelezioneFloor,&DatiShape.MiniSel);
		BaseRotazioni.IndiceRoom = GetIndiceRoom(-1);
#ifdef LOG_SHAPE
		InviaLog("Salva tabella rotazioni floor");
#endif


	}
	MaxFloor = -300;
	MaxCeiling = -300;
	MinFloor = 300;
	MinCeiling=300;


	if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_CEILING) {
		CopiaRect(&BaseRotazioni.SelezioneCeiling,&DatiShape.MiniSel);
		BaseRotazioni.IndiceRoom = GetIndiceRoom(-1);
#ifdef LOG_SHAPE
		InviaLog("Salva tabella rotazioni ceiling");
#endif
	}

	if (TipoOperazione & FTO_SHOW_INFOGRID) {
		InviaLog("-----------------------------------------------------");
		InviaLog("*************** GRID CASE REPORT ********************");
		InviaLog("-----------------------------------------------------");
	}

	for (y=DatiShape.MiniSel.top;y<MaxY;y++) {
		for (x=DatiShape.MiniSel.left;x<MaxX;x++) {
			pGriglia = &pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX];
			
			// ora eseguire tutte le possibili operazioni in ordine
			// inizializzare lista parametri
			va_start( marker, TestResetRecord );

			if (TestResetRecord==true) {
				// azzerare tutto usando BaseFloor e BaseCeiling
				pGriglia->WorldFloorClick = DatiShape.BaseFloor;
				pGriglia->WorldYCeilingClick= DatiShape.BaseCeiling;
				pGriglia->TestTriangleSplit = 0;
				pGriglia->Mistero =0;

				for (i=0;i<4;i++) {
					pGriglia->FloorCornerClicks[i] =0;
					pGriglia->CeilingCornerClicks[i]=0;
					pGriglia->WallCeilingCornerClicks[i]=0;
					pGriglia->WallFloorCornerClicks[i]=0;
				}
			}

			if (TipoOperazione & FTO_SHOW_INFOGRID) {

				// stampare tutti i campi di textuteinfo
				
				sprintf(BufferLog,"\r\n--------------- CASE[x=%d , y=%d] ------------------", x,y);
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->Type);
				sprintf(BufferLog,"\t\tType=0x%X", pGriglia->Type);
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->Flag1);
				sprintf(BufferLog,"\t\tFlags=0x%X", pGriglia->Flag1);
				InviaLog(BufferLog);
				
				ShowOffsetStruttura(pGriglia, &pGriglia->WorldFloorClick);
				sprintf(BufferLog,"\t\tWorldFloorClick=%d", pGriglia->WorldFloorClick);
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->WorldYCeilingClick );
				sprintf(BufferLog,"\t\tWorldCeilingClick=%d", pGriglia->WorldYCeilingClick);
				InviaLog(BufferLog);
				
				ShowOffsetStruttura(pGriglia, &pGriglia->FloorCornerClicks[0]);
				strcpy(BufferLog,"\t\tFloorCornerClick  =");
				for (i=0;i<4;i++) {
					sprintf(BufferTemp,"%03d  ",pGriglia->FloorCornerClicks[i]);
					strcat(BufferLog,BufferTemp);
				}
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->CeilingCornerClicks[0]);
				strcpy(BufferLog,"\t\tCeilingCornerClick=");
				for (i=0;i<4;i++) {
					sprintf(BufferTemp,"%03d  ",pGriglia->CeilingCornerClicks[i]);
					strcat(BufferLog,BufferTemp);
				}
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->WallFloorCornerClicks[0]);
				strcpy(BufferLog,"\t\tWallFloorCorner  =");
				for (i=0;i<4;i++) {
					sprintf(BufferTemp,"%03d  ",pGriglia->WallFloorCornerClicks[i]);
					strcat(BufferLog,BufferTemp);
				}
				InviaLog(BufferLog);
				
				ShowOffsetStruttura(pGriglia, &pGriglia->WallCeilingCornerClicks[0]);
				strcpy(BufferLog,"\t\tWallCeilingCorner=");
				for (i=0;i<4;i++) {
					sprintf(BufferTemp,"%03d  ",pGriglia->WallCeilingCornerClicks[i]);
					strcat(BufferLog,BufferTemp);
				}
				InviaLog(BufferLog);

				ShowOffsetStruttura(pGriglia, &pGriglia->Mistero);
				sprintf(BufferLog,"\t\tMistero=0x%X", pGriglia->Mistero);
				InviaLog(BufferLog);
					

				InviaLog("\t\tTextureInfo[14] Status Index  Rot  Tri  Mistery");
				for (i=0;i<14;i++) 		{
					ShowOffsetStruttura(pGriglia, 
								&pGriglia->TextureInfos[i].Status);
					sprintf(BufferLog,"\t\tTextureInfo[%02d] 0x%04X 0x%04X 0x%02X 0x%02X 0x%04X",
						i,
						pGriglia->TextureInfos[i].Status,
						pGriglia->TextureInfos[i].IndexAndFlags,
						pGriglia->TextureInfos[i].Rotation,
						pGriglia->TextureInfos[i].Triangle,
						pGriglia->TextureInfos[i].TriangleCouple);
					InviaLog(BufferLog);
				}

				ShowOffsetStruttura(pGriglia, &pGriglia->TestTriangleSplit);
				sprintf(BufferLog,"\t\tTestTriangleSplit=%d",pGriglia->TestTriangleSplit);
				InviaLog(BufferLog);

			}
				


			if (TipoOperazione & FTO_GET_FLOOR_CORNERS) {
				return (void*) &pGriglia->FloorCornerClicks;
			}
			if (TipoOperazione & FTO_GET_FLOOR_GRID) {
				return (void*) &pGriglia->WallFloorCornerClicks;
			}

			if (TipoOperazione & FTO_GET_CEILING_CORNERS) {
				return (void*) &pGriglia->CeilingCornerClicks;
			}
			if (TipoOperazione & FTO_GET_CEILING_GRID) {
				return (void*) &pGriglia->WallCeilingCornerClicks;
			}
			if (TipoOperazione & FTO_GET_FLOOR) {
				return (void*) &pGriglia->WorldFloorClick;
			}

			if (TipoOperazione & FTO_GET_CEILING) {
				return (void*) &pGriglia->WorldYCeilingClick;
			}

			if (TipoOperazione & FTO_SET_TRIANGLESPLIT) {
				pGriglia->TestTriangleSplit =1;

			}

			if (TipoOperazione & FTO_GET_MAX_FLOOR) {
				Numero = pGriglia->WorldFloorClick;
				for (i=0;i<4;i++) {
					Valore = Numero+pGriglia->FloorCornerClicks[i];
					if (Valore > MaxFloor) MaxFloor = Valore;
				}
			}

			if (TipoOperazione & FTO_GET_MAX_CEILING) {
				Numero = pGriglia->WorldYCeilingClick;
				for (i=0;i<4;i++) {
					Valore = Numero+pGriglia->CeilingCornerClicks[i];
					if (Valore > MaxCeiling) MaxCeiling=Valore;
				}
			}

			if (TipoOperazione & FTO_GET_MIN_FLOOR) {
				Numero = pGriglia->WorldFloorClick;
				for (i=0;i<4;i++) {
					Valore = Numero+pGriglia->FloorCornerClicks[i];
					if (Valore < MinFloor) MinFloor=Valore;
				}
			}

			if (TipoOperazione & FTO_GET_MIN_CEILING) {
				Numero = pGriglia->WorldYCeilingClick;
				for (i=0;i<4;i++) {
					Valore = Numero + pGriglia->CeilingCornerClicks[i];
					if (Valore < MinCeiling) MinCeiling=Valore;
				}
			}

			if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_FLOOR) {
				// salvare in TabellaRotazione il valore di rotazione
				// di casella floor attuale
				BaseRotazioni.TabellaFloor[x][y] = 
								pGriglia->TextureInfos[0].Rotation;
			}

			if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_CEILING) {
				// salvare in TabellaRotazione il valore di rotazione
				// di casella ceiling attuale
				BaseRotazioni.TabellaCeiling[x][y] = 
								pGriglia->TextureInfos[1].Rotation;
			}			

			if (TipoOperazione & FTO_SET_FLOOR) {
				pGriglia->WorldFloorClick = va_arg( marker, int);
			}

			if (TipoOperazione & FTO_SET_CEILING) {
				pGriglia->WorldYCeilingClick  = va_arg( marker, int);
			}

			if (TipoOperazione & FTO_SET_FLOOR_CORNERS) {
				for (i=0;i<4;i++) {
					pGriglia->FloorCornerClicks[i] = va_arg( marker, int);
				}
			}
			if (TipoOperazione & FTO_SET_CEILING_CORNERS) {
				for (i=0;i<4;i++) {
					pGriglia->CeilingCornerClicks[i] = va_arg( marker, int);
				}
			}

			if (TipoOperazione & FTO_SET_FLOOR_GRID) {
				for (i=0;i<4;i++) {
					pGriglia->WallFloorCornerClicks[i] = va_arg( marker, int);
				}
			}			

			if (TipoOperazione & FTO_SET_CEILING_GRID) {
				for (i=0;i<4;i++) {
					pGriglia->WallCeilingCornerClicks[i] = va_arg( marker, int);
				}
			}
			if (TipoOperazione & FTO_ADD_FLOOR) {
				pGriglia->WorldFloorClick += va_arg( marker, int);
			}

			if (TipoOperazione & FTO_ADD_CEILING) {
				pGriglia->WorldYCeilingClick += va_arg( marker, int);
			}

			if (TipoOperazione & FTO_ADD_FLOOR_CORNERS) {
				for (i=0;i<4;i++) {
					pGriglia->FloorCornerClicks[i] += va_arg( marker, int);
				}
			}
			if (TipoOperazione & FTO_ADD_CEILING_CORNERS) {
				for (i=0;i<4;i++) {
					pGriglia->CeilingCornerClicks[i] += va_arg( marker, int);
				}
			}
			if (TipoOperazione & FTO_GET_FLOOR_HEIGHT) {
				// restituisce il puntatore short all'altezza massima del 
				// pavimento in questo riquadro
				MaxClick=pGriglia->WorldFloorClick;
				n = -128;
				// trovare il valore massimo di floorcornerclick
				for (i=0;i<4;i++) {
					if (pGriglia->FloorCornerClicks[i] > n) 
								n = pGriglia->FloorCornerClicks[i];
				}
				MaxClick += n;

				return &MaxClick;
			}

			if (TipoOperazione & FTO_GET_CEILING_HEIGHT) {
				MaxClick=pGriglia->WorldYCeilingClick;
				n = 128;
				// trovare il valore minimo di ceilingcornerclick
				for (i=0;i<4;i++) {
					if (pGriglia->CeilingCornerClicks[i] < n) 
								n = pGriglia->CeilingCornerClicks[i];
				}
				MaxClick += n;

				return &MaxClick;
			}
			if (TipoOperazione & 
				(FTO_CLEAR_FLOOR_TEXTURES + FTO_CLEAR_CEILING_TEXTURES)) {
				// rimuovere texture da floor o ceiling
				// prima scoprire se si tratta di muro
				if ((pGriglia->Type & 0xf) ==  0x0e || 
					pGriglia->Type == 6) 
					TestWall=true;
				else
					TestWall=false;

				if (TipoOperazione & FTO_CLEAR_FLOOR_TEXTURES) 
					// se e' floor
					Scarto = 0;
				else
					// e' ceiling:
					Scarto = 8;

				switch (DatiShape.DirezioneFreccia) {
				case 0:
					// solo superfice
					IndiceStart=0;
					IndiceEnd=2;
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					// freccia ortogonale Superficie+lati
					IndiceStart=0;
					IndiceEnd=8;
					break;
				default:
					// freccia diagonale: SOLO lati
					IndiceStart=2;
					IndiceEnd=8;
					break;
				}

				IndiceStart+= Scarto;
				IndiceEnd+= Scarto;

				if (TestWall==true) {
					IndiceStart=0;
					IndiceEnd=16;
				}

				for (i=IndiceStart;i<IndiceEnd;i++) {
					j = VetIndiciFloor[i];


					if (pGriglia->TextureInfos[j].Status == 7) {
						pGriglia->TextureInfos[j].Status=0;
						pGriglia->TextureInfos[j].IndexAndFlags=0;
					}
					
				}
			}

			TestTriangoloTexture=false;
			if ((TipoOperazione & FTO_SET_TRIANGLE_TEXTURE_FLOOR)) {
				if (IsTriangle(pGriglia->FloorCornerClicks) == true) {
					TestTriangoloTexture=true;
					TestFloor=true;
				}else {
					// non e' un triangolo.
					// se e' attivo USE_TABELLAROTAZIONI
					// modificare la rotazione di questa texture (se presente)
					if ((TipoOperazione & FTO_USE_TABELLA_ROTAZIONE_FLOOR) &&
						pGriglia->TextureInfos[0].Status == 7) {
						Orientamento=BaseRotazioni.TabellaFloor[x][y];
						pGriglia->TextureInfos[0].Rotation = Orientamento;
#ifdef LOG_SHAPE
						sprintf(BufferLog,"Casa x=%d y=%d Orientamento=%d",
								x,y,Orientamento);
						InviaLog(BufferLog);
						InviaLog("\tRestore rotazione floor per texture quadrata");
#endif
					}
				}
			}
			if (TipoOperazione & FTO_SET_TRIANGLE_TEXTURE_CEILING) {
				if (IsTriangle(pGriglia->CeilingCornerClicks )==true)  {
					TestTriangoloTexture=true;
					TestFloor=false;
				}else {
					if ((TipoOperazione & FTO_USE_TABELLA_ROTAZIONE_CEILING) &&
						pGriglia->TextureInfos[1].Status == 7) {
						Orientamento=BaseRotazioni.TabellaCeiling[x][y];
						pGriglia->TextureInfos[1].Rotation = Orientamento;
#ifdef LOG_SHAPE
						sprintf(BufferLog,"Casa x=%d y=%d Orientamento=%d",
								x,y,Orientamento);
						InviaLog(BufferLog);
						InviaLog("\tRestore rotazione ceiling per texture quadrata");
#endif
					}

				}
			}

			if (TestTriangoloTexture==true) {
				FirstIndex=0;
				LastIndex=8;
				if (TestFloor==false) {
					FirstIndex++;
					LastIndex++;
				}
				// controllare se c'e' solo una texture su uno dei due triangoli
				// mettere una texture triangolare anche sul secondo
				// piazzando quello piu' corretta
				if (pGriglia->TextureInfos[FirstIndex].Status == 7) {

					IndiceTex = pGriglia->TextureInfos[FirstIndex].IndexAndFlags;
					Orientamento = pGriglia->TextureInfos[FirstIndex].Rotation;

					if (TipoOperazione & FTO_USE_TABELLA_ROTAZIONE_FLOOR) {
						
						Orientamento = BaseRotazioni.TabellaFloor[x][y];
#ifdef LOG_SHAPE
						InviaLog("Usa orientamento floor di tabella");
#endif
					}

					if (TipoOperazione & FTO_USE_TABELLA_ROTAZIONE_CEILING) {
						Orientamento = BaseRotazioni.TabellaCeiling[x][y];
#ifdef LOG_SHAPE


						InviaLog("Usa orientamento ceiling di tabella");
#endif
					}
#ifdef LOG_SHAPE
					sprintf(BufferLog,"Casa x=%d  y=%d  IndiceTex=0x%X Orientamento=%d TriangleCouple=%d",
						x,y,IndiceTex,Orientamento,pGriglia->TextureInfos[FirstIndex].TriangleCouple );
					InviaLog(BufferLog);
#endif

					pGriglia->TextureInfos[LastIndex].IndexAndFlags= IndiceTex;
					pGriglia->TextureInfos[LastIndex].Status= 7;

					switch (pGriglia->TextureInfos[FirstIndex].TriangleCouple) {
					case 0:
						// coppia di triangoli Nord-Ovest ; Sud-Est
						switch (Orientamento) {
						case 0:
							FirstRotation=2;
							FirstTriangle=3;
							break;
						case 1:
							FirstRotation=2;
							FirstTriangle=2;
							break;
						case 2:
							FirstRotation=2;
							FirstTriangle=1;
							break;
						case 3:
							FirstRotation=2;
							FirstTriangle=0;
							break;
						}
						break;

					case 1:
						// coppia di triangoli Nord-Est  ; Sud-Ovest
						switch (Orientamento) {
						case 0:
							FirstRotation=0;
							FirstTriangle=0;
							break;
						case 1:
							FirstRotation=0;
							FirstTriangle=3;
							break;
						case 2:
							FirstRotation=0;
							FirstTriangle=2;
							break;
						case 3:
							FirstRotation=0;
							FirstTriangle=1;
							break;
						}
						break;
					}

					pGriglia->TextureInfos[FirstIndex].Rotation=FirstRotation;
					pGriglia->TextureInfos[FirstIndex].Triangle=FirstTriangle;
					if (TestFloor==true) {
						n = FirstTriangle + 2;
						n &= 3;

					}else {
						n = FirstTriangle -2 ;
						n &= 3;
					}
					pGriglia->TextureInfos[LastIndex].Rotation = 1;
					pGriglia->TextureInfos[LastIndex].Triangle = n;

#ifdef LOG_SHAPE
					sprintf(BufferLog,"\tRotation1=%d  Triangle1=%d  Rotation2=%d  Triangle2=%d",
						FirstRotation,FirstTriangle,1,n);
					InviaLog(BufferLog);
#endif

				}

			}

				


		}
#ifdef LOG_SHAPE
		if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_FLOOR) {
			BufferLog[0]=0;
			for (x=DatiShape.MiniSel.left;x<MaxX;x++) {
				sprintf(BufferTemp,"%d ", BaseRotazioni.TabellaFloor[x][y]);
				strcat(BufferLog,BufferTemp);
			}
			InviaLog(BufferLog);
		}
		if (TipoOperazione & FTO_SET_TABELLA_ROTAZIONE_CEILING) {
			BufferLog[0]=0;
			for (x=DatiShape.MiniSel.left;x<MaxX;x++) {
				sprintf(BufferTemp,"%d ", BaseRotazioni.TabellaCeiling[x][y]);
				strcat(BufferLog,BufferTemp);
			}
			InviaLog(BufferLog);
		}
#endif
	}

	va_end( marker );
	if (TipoOperazione & FTO_SHOW_INFOGRID) {
		InviaLog("**************** END GRID REPORT *********************");
	}

	if (TipoOperazione & FTO_GET_MAX_FLOOR) return &MaxFloor;
	if (TipoOperazione & FTO_GET_MAX_CEILING) return &MaxCeiling;
	if (TipoOperazione & FTO_GET_MIN_FLOOR) return &MinFloor;
	if (TipoOperazione & FTO_GET_MIN_CEILING) return &MinCeiling;

	return NULL;

}
// chiamata in fase di caricamento progetto
// verifica se nome tga e' corretto, se non lo e' chiede subito
// di selezionarlo.
// nota: se il file e' .pcx restituisce true, se invece e' tga restituisce false
bool ControllaNomeTga(char NomeTga[])
{

	char NomeFile[256];
	bool TestPcx;
	char *pDirSrc;
	char BufDirShort[257];	
	char DirSrc[256];
	char *pDir;

	HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow

	TestPcx = true;
	if (strcmpi(SoloEstensione(NomeTga),"tga") == 0) TestPcx=false;

	if (EsisteFile(NomeTga)==true) {
		pDirSrc = SoloDir(NomeTga);
		GetShortPathName(pDirSrc, BufDirShort, 256);

		sprintf(NomeTga, "%s\\%s", BufDirShort, SoloNome(NomeTga));

		return TestPcx;
	}
	


	sprintf(BufferLog, "ERROR: cann't locate Texture file:\r\n\r\n%s\r\n\r\nPlease, select now the correct path for TGA texture map", NomeTga);
	MostraMessageBoxNGLE(BufferLog, 3);


	// scoprire cartellad a impostare
	pDir = SoloDir(NomeLoadProject);
	sprintf(NomeFile, "%s\\%s", pDir,SoloNome(NomeTga));
	if (EsisteFile(NomeFile)==false) {
		// non c'e' in cartella attuale
		// allora provare in cartella graphics/wads
		sprintf(NomeFile, "%s\\graphics\\wads\\%s", 
			Dir_Trle, SoloNome(NomeTga));
		
	}

	strcpy(DirSrc,SoloDir(NomeFile));
	// preparare nome file
	strcpy(NomeFile, SoloNome(NomeTga));
	PreparaOpenFile("Select Texture Map File",
		"TGA Map Files (*.tga)",
		"*.tga",to_Load,DirSrc,NomeFile);
	ofn.hwndOwner= *pWindRoom;


	if (GetOpenFileName(&ofn)==FALSE) return TestPcx;

	// mettere nome corto per cartella
	pDirSrc = SoloDir(NomeFile);
	GetShortPathName(pDirSrc, BufDirShort, 256);

	sprintf(NomeTga, "%s\\%s", BufDirShort, SoloNome(NomeFile));

	return TestPcx;

}

/*
#define FTO_FLOOR_PIU  0x64 
#define FTO_FLOOR_MENO 0x65 
#define FTO_CEILING_PIU 0x66  
#define FTO_CEILING_MENO 0x67 
#define FTO_FLOOR2_PIU 0x78 
#define FTO_FLOOR2_MENO 0x79
#define FTO_CEILING2_PIU 0x7a 
#define FTO_CEILING2_MENO 0x7b 
*/
// effettua la modifica a griglia di muro selezionato in DatiShape.MiniSel
// se TestElimina = true elimina ogni griglia dal muro

void ModificaGrigliaMuro(bool TestElimina)
{
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int TotClick;
	int N;

	pVetRooms = *p2VetRooms;

	TotClick = DatiShape.BaseCeiling - DatiShape.BaseFloor;

	if (TestElimina==true) {
		// attenzione questo era stato modifciato
		ModificaClickSettore(FTO_SET_NONE,true);
		return;
	}

	N = TotClick / 4;

	switch (N) {
	case 4:
		// 4 sezioni (altezza 16 click 
		N= TotClick / 4;
		ModificaClickSettore(FTO_SET_FLOOR | FTO_SET_CEILING |
							 FTO_SET_FLOOR_GRID,true, 
				DatiShape.BaseFloor+N*2, DatiShape.BaseCeiling-N,
				-N,-N,-N,-N);		// floor grid
		break;
	case 3:
		// 3 divisioni (altezza 12 click)
		N = TotClick / 3;
		ModificaClickSettore(FTO_SET_FLOOR | FTO_SET_FLOOR_GRID,true ,
				DatiShape.BaseFloor + N*2,
				-N,-N,-N,-N);
		break;
	case 2:
		// 2 divisioni (altezza 8 click)
		N = TotClick / 2;
		ModificaClickSettore(FTO_SET_FLOOR,true,
				DatiShape.BaseFloor + N);

		break;
	default:
		if (N >= 5) {
			N = TotClick / 5;
			// fare 5 sezioni
			ModificaClickSettore(FTO_SET_FLOOR | FTO_SET_CEILING |
						  FTO_SET_FLOOR_GRID |
						  FTO_SET_CEILING_GRID,true,
					DatiShape.BaseFloor + N*2, DatiShape.BaseCeiling - N*2,
					-N,-N,-N,-N,
					N,N,N,N);

			break;
		}
		// N < 2
		N = TotClick / 2;
		ModificaClickSettore(FTO_SET_FLOOR,true,
				DatiShape.BaseFloor + N);
		break;

	}



}

// crea (o elimina se TestElimina=true) griglie in tutti i muri della stanza attuale
void CambiaTuttiWall(bool TestElimina)
{

	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int SizeX,SizeY;
	int OrgDispX,OrgDispY;

	// qui bisogna selezionare tutti e quattro i muri uno per uno
	pVetRooms = *p2VetRooms;


	OrgDispX = 0;
	OrgDispY = 0;

	SizeX = pVetRooms[*pIndiceRoomAttuale].XSizeBlocks;
	SizeY = pVetRooms[*pIndiceRoomAttuale].ZSizeBlocks;
	
	// lato nord

	DatiShape.MiniSel.left = 1;
	DatiShape.MiniSel.top = 0;
	DatiShape.MiniSel.right = SizeX-2;
	DatiShape.MiniSel.bottom = 1;
	ModificaGrigliaMuro(TestElimina);

	// lato ovest

	DatiShape.MiniSel.left = 0;
	DatiShape.MiniSel.top = 1;
	DatiShape.MiniSel.right = 1;
	DatiShape.MiniSel.bottom = SizeY-2;
	ModificaGrigliaMuro(TestElimina);

	// lato sud

	DatiShape.MiniSel.left = 1;
	DatiShape.MiniSel.top = SizeY-1;
	DatiShape.MiniSel.right = SizeX-2;
	DatiShape.MiniSel.bottom = 1;
	ModificaGrigliaMuro(TestElimina);

	// lato est

	DatiShape.MiniSel.left = SizeX -1;
	DatiShape.MiniSel.top = 1;
	DatiShape.MiniSel.right = 1;
	DatiShape.MiniSel.bottom = SizeY-2;
	ModificaGrigliaMuro(TestElimina);
}

// funzione chiamata prima di ogni modifica del menu shape
// se c'e' un errore restituisce false
// effettua il backup undo
bool IniziaShape(bool TestGetSelezione, bool TestNoWall, int MinSize, 
				 bool TestMostraWait)
{

	int *pTipoFrecciaSelezione = (int *) 0x583BB6;
	int IndiceRoom;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;	
	HCURSOR *pHandleIconaMouse = (HCURSOR*) 0x493110;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;

	IndiceRoom = *pIndiceRoomAttuale;

	DatiShape.SalvaCursore = *pHandleIconaMouse;
	if (TestGetSelezione==true) {
		if (PreparaMiaSelezione(MinSize,TestNoWall)== false) return false;
		DatiShape.SalvaFrecciaSelezione = *pTipoFrecciaSelezione;
		DatiShape.DirezioneFreccia = DatiShape.SalvaFrecciaSelezione;
		memcpy(&DatiShape.SalvaRectSelezione,pRectSelezione2d,sizeof(RECT));
		memcpy(&DatiShape.MiniSel, &DatiShape.MiaSelezione,sizeof(RECT));
	}
	TrovaAltezzaMediaStanza(IndiceRoom,&DatiShape.BaseFloor,
				&DatiShape.BaseCeiling);
	roomBackupUndo();
	CongelaBackupUndo();
	TotCambiBackup++;
	SetCursor(CursoreClessidra);
	*pHandleIconaMouse = CursoreClessidra;

	if (TestMostraWait==true) 
			ApriFinestraWait("Editing shape in progress, wait please...");
	return true;
}

void RefreshSelezione(void)
{
	__asm {
		pushad

		mov eax, 412660h    ;ShowToolTipSelezione
		call eax
		mov eax, 40FD80h    ;ShowRiquadro2d
		call eax
		mov eax, 401200h    ;CicloPrincipale
		call eax
		push	1
		mov eax, 410120h    ;ShowSelezione2d
		call  eax
		add		esp,4

		mov eax, 401200h    ;CicloPrincipale
		call eax

		popad

	}
}
// da chiamare dopo aver effettuato modifiche del menu shape
// riattiva undo e visualizza stanza, se TestMettiSelezione==true
// riattiva la selezione originale
void TerminaShape(bool TestMettiSelezione, bool TestMostraWait)
{
	int *pTipoFrecciaSelezione = (int *) 0x583BB6;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	HCURSOR *pHandleIconaMouse = (HCURSOR*) 0x493110;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;

	if (TestMostraWait==true) 
			ChiudiFinestraWait();

	SetCursor(DatiShape.SalvaCursore);
	*pHandleIconaMouse = DatiShape.SalvaCursore;


	RefreshStanzaAttuale();	
	RipristinaBackupUndo();	
	if (TestMettiSelezione==true) {
		// ripristina selezione
		// convertire da MiaSelezione a *pSelezione2d
		*pTestEsisteSelezioneBox=1;
		*pTipoFrecciaSelezione = DatiShape.SalvaFrecciaSelezione;
		memcpy(pRectSelezione2d, &DatiShape.SalvaRectSelezione,sizeof(RECT));
		// ora chiamare varie procedure per fare aggiornaemnto

		RefreshSelezione();
		
	}
	// controllare modifiche alla stanza
	ControllaOldRoom((BYTE *) &pVetRooms[*pIndiceRoomAttuale],
			(BYTE *) pVetRooms[*pIndiceRoomAttuale].pBlockGridCollision,
			pVetRooms[*pIndiceRoomAttuale].XSizeBlocks,
			pVetRooms[*pIndiceRoomAttuale].ZSizeBlocks);


}
// elimina tutte le texture da zona selezionata di floor o ceiling
void ClearTexture(bool TestFloor)
{
	// mettere a zero tutti campi dove c'e' tipo tga
	if (TestFloor==true) {
		DatiShape.ShiftCeiling =1;
		DatiShape.TestCeiling = false;
	}
	else {
		DatiShape.ShiftCeiling =2;
		DatiShape.TestCeiling = true;
	}
	ModificaClickSettore(FTO_CLEAR_FLOOR_TEXTURES * DatiShape.ShiftCeiling ,false);

}

void Menu_GridAllWall(void *pPunta)
{
	if (IniziaShape(false,false,1,false)==false) return;
	CambiaTuttiWall(false);
	TerminaShape(false,false);
}

void Menu_RemoveGridAllWall(void *pPunta)
{
	if (IniziaShape(false,false,1,false)==false) return;
	CambiaTuttiWall(true);
	TerminaShape(false,false);
}

void Menu_ClearTextureFloor(void *pPunta)
{
	if (IniziaShape(true,false,1,false)==false) return;
	ClearTexture(true);
	TerminaShape( true,false);
}
// cerca di costruire griglia completa anche quando c'e' una porta nel muro	
void GridWallDoor(void)
{
	short *pFloorGrid;
	short VetFloor[4];
	int		i;

	pFloorGrid = (short*) ModificaClickSettore(FTO_GET_FLOOR_GRID,false);
	for (i=0;i<4;i++) VetFloor[i]=pFloorGrid[i];
	ModificaClickSettore(FTO_SET_FLOOR_GRID,false, 
		VetFloor[0]+12, VetFloor[1]+12, VetFloor[2]+12, VetFloor[3]+12);


}
void Menu_GridDoorFloor(void *pPunta)
{
	if (IniziaShape(true,false,1,false)==false) return;
	GridWallDoor();
	TerminaShape( true,false);
}

void Menu_ClearTextureCeiling(void *pPunta)
{
	if (IniziaShape(true,false,1,false)==false) return;
	ClearTexture(false);
	TerminaShape(true,false);
}

void CreaPiramide(bool TestFloor, bool TestInversa)
{

	int SizeX,SizeY;
	bool TestPiu;
	int ShiftC;
	bool TestUguale;
	RECT SelLarga;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	bool TestSmooth;
	int IndiceRoom;	
	RECT rec;
	char VetClick[4];
	char *pFloor;
	int i;
	char Incremento;	

#ifdef LOG_SHAPE
	sprintf(BufferLog,"CreaPiramide(TestFloor=%d , TestInversa=%d)",
			TestFloor,TestInversa);
	InviaLog(BufferLog);
#endif

	if (TestFloor==true) {
		DatiShape.TestCeiling=false;
		ShiftC = 1;
	}else {
		DatiShape.TestCeiling=true;
		ShiftC = 2;
	}
	IndiceRoom=*pIndiceRoomAttuale;

	// calcolare se piramide va fatta con smooth o meno
	if (DatiShape.SalvaFrecciaSelezione != 0) 
		TestSmooth=false;
	else
		TestSmooth=true;
	// salvare dati per ricostruzione texture triangoli
	// ampliare la selezione di una casa in ogni direzione 
	SizeX = DatiShape.MiaSelezione.right;
	SizeY = DatiShape.MiaSelezione.bottom;

	CopiaRect(&SelLarga,&DatiShape.MiaSelezione);
	if ((SelLarga.left -1) != 0) {
		SelLarga.left --;
		SelLarga.right++;
	}
	if ((SelLarga.top -1) !=0) {
		SelLarga.top--;
		SelLarga.bottom++;
	}
	if ((SelLarga.left + SizeX+1) != DatiShape.pRoom->XSizeBlocks) 
		SelLarga.right++;

	if ((SelLarga.top + SizeY+1) != DatiShape.pRoom->ZSizeBlocks)
		SelLarga.bottom++;


	// adesso vedere se e' necessario dati texture adesso oppure
	// lasciare quella precedente
	
	TestUguale=true;
	if (TestFloor==true) {
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneFloor.left || 
			SelLarga.top != BaseRotazioni.SelezioneFloor.top ||
			SelLarga.right != BaseRotazioni.SelezioneFloor.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneFloor.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;
	}else {
		// confronto dati ceiling
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneCeiling.left || 
			SelLarga.top != BaseRotazioni.SelezioneCeiling.top ||
			SelLarga.right != BaseRotazioni.SelezioneCeiling.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneCeiling.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;

	}

	if (TestUguale==false) {
		// bisogna creare adesso tabella
		CopiaRect(&DatiShape.MiniSel,&SelLarga);
		ModificaClickSettore(FTO_SET_TABELLA_ROTAZIONE_FLOOR*ShiftC,false);
	}


	// fine salvataggio texture triangoli

	// aumentare di un click in zone concentriche
	CopiaRect(&DatiShape.MiniSel,&DatiShape.MiaSelezione);
	DatiShape.MiniSel.right -= 2;
	DatiShape.MiniSel.bottom -= 2;
	DatiShape.MiniSel.left++;
	DatiShape.MiniSel.top++;
	do {
		SizeX = DatiShape.MiniSel.right;
		SizeY = DatiShape.MiniSel.bottom;
		if (TestInversa==true) 
			TestPiu=false;
		else
			TestPiu=true;


		PulsantiFloor(TestFloor,TestPiu,TestSmooth,0,1);

		DatiShape.MiniSel.right -= 2;
		DatiShape.MiniSel.bottom -= 2;
		DatiShape.MiniSel.left++;
		DatiShape.MiniSel.top++;
	}while (DatiShape.MiniSel.right > 0 && DatiShape.MiniSel.bottom > 0);
	// ora creare texture triangolari mancanti
	



	// qui fare gli angoli (se siamo sul floor e smooth e' attivo)
	if (TestSmooth == true &&
		TestFloor==true && 
		(MyPrefRoom.Flags & FPR_PUNTA_PIRAMIDI) != 0) {

		CopiaRect(&rec,&DatiShape.MiniSel);

		rec.right += 2;
		rec.bottom += 2;
		rec.left --;
		rec.top --;

		CopiaRect(&DatiShape.MiniSel,&SelLarga);
		ModificaClickSettore(FTO_SET_TRIANGLESPLIT, false);
		if (TestInversa==true) 
			Incremento = -1;
		else
			Incremento = 1;

		if (rec.right > 1 && rec.bottom > 1 ) {
			// prima fare gli angoli che sono uguali per tutti

			// angolo in alto a sinistra -------
			SetMiniSel(rec.left,rec.top);
			pFloor= (char*) ModificaClickSettore(FTO_GET_FLOOR_CORNERS, false);
			// angolo 3 (sud-est)
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];
			VetClick[3]+= Incremento;

			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

			// angolo in alto a destra -------------------------
			SetMiniSel(rec.left + rec.right -1,rec.top);
			pFloor= (char*)ModificaClickSettore(FTO_GET_FLOOR_CORNERS, false);
			// angolo 0 (sud-ovest)
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];
			VetClick[0]+= Incremento;
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

			// angolo in basso a sinistra ----------------------
			SetMiniSel(rec.left,rec.top + rec.bottom -1);
			pFloor= (char *) ModificaClickSettore(FTO_GET_FLOOR_CORNERS, false);
			// angolo 2 (nord-est)
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];
			VetClick[2]+=Incremento;
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

			// angolo in basso a destra ----------------------
			SetMiniSel(rec.left + rec.right -1,rec.top + rec.bottom -1);
			pFloor= (char *) ModificaClickSettore(FTO_GET_FLOOR_CORNERS, false);
			// angolo 1 (nord-ovest)
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];
			VetClick[1]+= Incremento;
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);
		}
		// ora gestire eventuali case intermedie
		if (rec.right>2 && rec.bottom > 1) {
			// lato lungo e' orizzontale
			rec.left++;
			rec.right -=2;
			// preparare prima riga
			SetMiniSel(rec.left,rec.top);
			pFloor = (char* ) ModificaClickSettore(FTO_GET_FLOOR_CORNERS,false);
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];

			// fare riga alta con l'incremento verso il basso
			VetClick[0]+=Incremento;
			VetClick[3]+=Incremento;
			CopiaRect(&DatiShape.MiniSel, &rec);
			DatiShape.MiniSel.bottom --;

			// ora modifica
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

			// ora fare riga in basso
			DatiShape.MiniSel.top++;
			pFloor = (char* ) ModificaClickSettore(FTO_GET_FLOOR_CORNERS,false);
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];			

			VetClick[1]+=Incremento;
			VetClick[2]+=Incremento;
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

		}
		if(rec.right > 1 && rec.bottom > 2) {
			// lato lungo e' verticale
			rec.top++;
			rec.bottom  -=2;
			// preparare prima riga
			SetMiniSel(rec.left,rec.top);
			pFloor = (char* ) ModificaClickSettore(FTO_GET_FLOOR_CORNERS,false);
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];

			// fare riga di sinistra con l'incremento verso destra
			VetClick[2]+=Incremento;
			VetClick[3]+=Incremento;
			CopiaRect(&DatiShape.MiniSel, &rec);
			DatiShape.MiniSel.right  --;

			// ora modifica
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

			// ora fare riga di destra
			DatiShape.MiniSel.left++;
			pFloor = (char* ) ModificaClickSettore(FTO_GET_FLOOR_CORNERS,false);
			for (i=0;i<4;i++) VetClick[i] = pFloor[i];			
			// incrementi verso sinistra
			VetClick[0]+=Incremento;
			VetClick[1]+=Incremento;
			ModificaClickSettore(FTO_SET_FLOOR_CORNERS, false,
				VetClick[0], VetClick[1], VetClick[2], VetClick[3]);

		}


	}
	CopiaRect(&DatiShape.MiniSel,&SelLarga);
	ModificaClickSettore(FTO_SET_TRIANGLE_TEXTURE_FLOOR * ShiftC | 
						 FTO_USE_TABELLA_ROTAZIONE_FLOOR * ShiftC,false);

}
void Menu_Piramide(void *pPunta)
{
	int N;

	InviaLog("Menu_Piramide Floor");
	if (MyPrefRoom.Flags & FPR_PUNTA_PIRAMIDI) 
		N = 2;
	else
		N = 3;

	if (IniziaShape(true,true,N,false)==false) return;
	CreaPiramide(true,false);
	TerminaShape(true,false);
}

void Menu_InvPiramide(void *pPunta)
{
	int N;
	InviaLog("Menu_InvPiramide Floor");
	if (MyPrefRoom.Flags & FPR_PUNTA_PIRAMIDI) 
		N = 2;
	else
		N = 3;
	if (IniziaShape(true,true,N,false)==false) return;
	CreaPiramide(true,true);
	TerminaShape(true,false);
}

void Menu_CPiramide(void *pPunta)
{

	InviaLog("Menu_Piramide Ceiling");

	if (IniziaShape(true,true,3,false)==false) return;
	CreaPiramide(false,false);
	TerminaShape(true,false);
}

void Menu_CInvPiramide(void *pPunta)
{
#ifdef LOG_SHAPE
	InviaLog("Menu_Piramide Ceiling inversa");
#endif
	if (IniziaShape(true,true,3,false)==false) return;
	CreaPiramide(false,true);
	TerminaShape(true,false);
}


// mette griglia su muro selezionato
void Menu_GridSelectedWall(void *pPunta)
{
	if (IniziaShape(true,false,1,false)==false) return;

	ModificaGrigliaMuro(false);

	TerminaShape(true,false);


}

// mette griglia su muro selezionato
void Menu_RemoveGridSelectedWall(void *pPunta)
{
	if (IniziaShape(true,false,1,false)==false) return;
	ModificaGrigliaMuro(true);
	TerminaShape(true,false);

}

void Menu_Cupola(void *pPunta)
{
	CreaCupola(false,false);

}

void PatchTotRigheAnimation(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x42B34C, 'B'},
		{0x42B350, 'D'},
		{0x42B5C1, 'B'}};

	TotPatchs=3;
	TotOld = 0xA;
	TotNew=0x14;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchSizeYAnimation(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x42B102, 'D'},
		{0x42B22A, 'D'},
		{0x42B260, 'D'},
		{0x42B2A8, 'D'},
		{0x42B3B8, 'D'},
		{0x42B589, 'D'},
		{0x45C350, 'D'},
		{0x45C364, 'W'},
		{0x45C37C, 'W'},
		{0x45C3A0, 'D'}};

	TotPatchs=10;
	TotOld = 0x140;
	TotNew=0x280;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}
void PatchOrgYAnimation(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x42B10C, 'D'},
		{0x42B1B7, 'D'},
		{0x42B234, 'D'},
		{0x42B26A, 'D'},
		{0x42B2B2, 'D'},
		{0x42B320, 'D'},
		{0x42B3C2, 'D'},
		{0x42B49F, 'D'},
		{0x42B593, 'D'},
		{0x454EAC, 'D'},
		{0x45C398, 'D'}};

	TotPatchs=11;
	TotOld = 0x153;
	TotNew=0x13;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void Menu_CupolaInv(void *pPunta)
{
	CreaCupola(false,true);

}
void Menu_CCupola(void *pPunta)
{
	CreaCupola(true,false);

}
void Menu_CCupolaInv(void *pPunta)
{
	CreaCupola(true,true);

}
__declspec(naked) void PatchImpostaMenu(void)
{
	// viene chiamata con un JMP [...] immediatamente
	// all'inizio di inserimento dati menu
	// nella stessa funzione poi si impostano anche i pulsanti principale
	__asm {
		mov  eax, 436cb7h ;chiama la Call LOC_436cb7 eliminata da questa patch
		call  eax
		// ------ qui si possono inserire altri menu o pulsanti ------

		// prima modificare quello che era ultimo pulsante in room edit
		// in modo che punti a pulsante backup:
		mov  eax, 458A14h ;DatiPulsante_ZonaViewOggetto
		lea ecx, DatiPulsanteBackup
		mov dword ptr [eax], ecx

		// ora spostare pulsanti Climb
		mov  eax, 458f3ch  ;DatiPulsante_B
		push  eax
		lea eax, SpostaPulsantiClimb
		call eax
		add esp,4

		// sposta e ridimensiona pulsanti room
		mov  eax, 45938Ch  ;DatiPulsante_SelectRoom
		push	eax
		lea		eax,SpostaPulsantiRoom
		call	eax
		add		esp,4


		// ora spostare scritta "Climb"
		mov  eax, 4114C0h  ;punta a CordY di testo
		add dword ptr [eax], 12
		add dword ptr [eax+5], 3

		// ora spostare pulsanti water, nl e D
		mov eax, 4594B0h ;DatiPulsante_NL (gli altri seguono)
		push  eax
		lea  eax, SpostaPulsantiniRoom
		call  eax
		add esp,4

		// cambiare dimensione di pulsante place object
		mov		eax,458FD6h  ;size di pulsantre object
		mov		word ptr [eax], 171

		// cambiare dimensione di pulsante Flip Palette
		mov		eax, 4595F2h
		mov		word ptr [eax], 128

		// PULSANTE BACKUP ora chiamare funzione per impostare pulsante backup
		lea eax, NomePulsanteBackup
		push eax
		lea eax, DatiPulsanteBackup
		push eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		/// FINE PULSNATE BACKUP
		// PULSANTE Damage
		lea eax, NomePulsanteDamage
		push eax
		lea eax, DatiPulsanteDamage
		push eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE Damage

		// PULSANTE BIG TEXTURE
		lea  eax, NomePulsanteBigTexture
		push eax
		lea eax, DatiPulsanteBigTexture
		push eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE BIG TEXTURE

		// PULSANTE ROTATE OBJECT
		lea		eax, NomePulsanteRotate
		push	eax
		lea		eax, DatiPulsanteRotate
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// fine pulsante rotate

		// PULSANTE GO BACK OLD ROOM
		lea		eax, NomePulsanteGoBack
		push	eax
		lea		eax, DatiPulsanteGoBack
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE GOBACK

		// PULSANTE SEARCH

		lea		eax, NomePulsanteSearch
		push	eax
		lea		eax, DatiPulsanteSearch
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE SEARCH

		// PULSANTE MOVE LARA
		lea		eax, NomePulsanteMoveLara
		push	eax
		lea		eax, DatiPulsanteMoveLara
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE MOVE LARA

		// PULSANTE V2dMap
		lea		eax, NomePulsanteV2dMap
		push	eax
		lea		eax, DatiPulsanteV2dMap
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE V2dMap

		// PULSANTE Visuale2d
		lea		eax, NomePulsanteVisuale2d
		push	eax
		lea		eax, DatiPulsanteVisuale2d
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE Visuale2d

		// PULSANTE Play
		lea		eax, NomePulsantePlay
		push	eax
		lea		eax, DatiPulsantePlay
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE play

		// PULSANTE Exit and Play
		lea		eax, NomePulsanteExitAndPlay
		push	eax
		lea		eax, DatiPulsanteExitAndPlay
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE Exit and play

		// PULSANTE Remove Items
		lea		eax, NomePulsanteRemoveItems
		push	eax
		lea		eax, DatiPulsanteRemoveItems
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE removeitem

		// PULSANTE Go
		lea		eax, NomePulsanteGo
		push	eax
		lea		eax, DatiPulsanteGo
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE go

		// PULSANTE settings
		lea		eax, NomePulsanteSettings
		push	eax
		lea		eax, DatiPulsanteSettings
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE settings

		// PULSANTE New room up
		lea		eax, NomePulsanteNewRoomUp
		push	eax
		lea		eax, DatiPulsanteNewRoomUp
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE new room up
		// PULSANTE new room down
		lea		eax, NomePulsanteNewRoomDown
		push	eax
		lea		eax, DatiPulsanteNewRoomDown
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE new room down

		// PULSANTE new tunnel
		lea		eax, NomePulsanteNewTunnel
		push	eax
		lea		eax, DatiPulsanteNewTunnel
		push	eax
		mov  eax, 4125A0h  ;ImpostaPulsante
		call eax
		add esp, 8
		// FINE PULSANTE new tunnel
		// Modifica dimensione Animation Range window ------
		lea eax, PatchSizeYAnimation
		call  eax
		lea eax, PatchOrgYAnimation
		call  eax

		lea eax, PatchTotRigheAnimation
		call eax

		// fine modifica dimensione

		// ------- inizio inserimento nuovi menu -----------
		// salvare stack attuale
		lea		eax, SalvaStack
		mov		dword ptr [eax], esp

		// nota il byte alto di scan code ha questi signficati:
		// 00  = ALT
		// 40  = CTRL
		// 80  = tasto funzione
		// --------------------------------------- HELP menu ---------------------
		// help->Info about project
		lea		eax, Menu_InfoAboutProject
		push	eax										;1
		push	0										;2
		lea		eax, NomeMenu_InfoAboutProject
		push	eax										;3

		// help->Keyboard Commands
		lea		eax, Menu_ShowKeyBoardCommands
		push	eax										;1
		push	0										;2
		lea		eax, NomeMenu_HelpKeyboardCommand
		push	eax										;3

		// help->Buttons Window
		lea		eax, Menu_ShowButtonsWindowsHelp        ;4
		push	eax
		push	0
		lea		eax, NomeMenu_ShowButtonsWindowsHelp
		push	eax

		// help->Object IDs for Trep
		lea		eax, Menu_ShowRemapObj
		push	eax
		push	0
		lea		eax, NomeMenu_ShowRemapObj
		push	eax

		// help->Check for patch updates
		lea		eax, Menu_UpdateRoomEditor
		push	eax										;4
		push	0	;scan code							;5
		lea		eax, NomeMenu_HelpUpdateNG
		push	eax										;6

		// help->About Next Generation
		lea		eax, Menu_AboutRoomEditor
		push	eax		;Procedura per Help->About		;7
		push	0		;scan code						;8
		lea		eax, NomeMenu_HelpAbout
		push	eax		;Nome menu						;9

		// help->Default help
		lea		eax, Menu_ShowDefaultHelp
		push	eax		;Procedura						;7
		push	0		;scan code						;8
		lea		eax, NomeMenu_ShowDefaultHelp
		push	eax		;Nome menu						;9

		// help->Revised help
		lea		eax, Menu_ShowNGHelp
		push	eax		;Procedura per Help->About		;7
		push	0		;scan code						;8
		lea		eax, NomeMenu_ShowNGHelp
		push	eax		;Nome menu						;9

		// help-> Next Generation help
		lea		eax, Menu_ShowRevisedHelp
		push	eax		;Procedura						;7
		push	0		;scan code						;8
		lea		eax, NomeMenu_ShowRevisedHelp
		push	eax		;Nome menu						;9

		// gruppo menu help
		push	183		;larghezza x del menu Help		;10
		push	9		;numero di item di menu help	;11
		lea		eax, NomeMenu_Help
		push	eax										;12

		// ------------------------ PLUGINS menu ------------------------------------------


		// Plugins->Show info about mounted plugins
		lea		eax, Menu_ShowInfoPlugins
		push	eax										;1
		push	0										;2
		lea		eax, NomeMenu_InfoMountedPlugins
		push	eax										;3

		// Plugins->Look for unhandled Plugins
		lea		eax, Menu_ListUnhandledTriggers
		push	eax										;4
		push	0										;5
		lea		eax, NomeMenu_ListUnhandledTriggers
		push	eax										;6

		// Plugins->Show Triggers Owned by Plugin
		lea		eax, Menu_ShowTriggerListDiPlugin
		push	eax										;7
		push	0										;8
		lea		eax, NomeMenu_ShowTriggerListDiPlugin		
		push	eax										;9

		// Plugins->Change ID of Plugin
		lea		eax, Menu_ChangePluginId				;10
		push	eax
		push	0										;11
		lea		eax, NomeMenu_ChangePluginId	
		push	eax										;12

		// Plugins->Remove Triggers of Plugin
		lea		eax, Menu_RemoveTriggersOfPlugin
		push	eax										;13
		push	0										;14
		lea		eax, NomeMenu_RemoveTriggerDiPlugin
		push	eax										;15

		// gruppo menu Plugins
		push	150		;larghezza x del menu plugins	;10
		push	5		;numero di item di menu plugins	;11
		lea		eax, NomeMenu_Plugins
		push	eax										;12

		// -----------------------  SHAPES menu ---------------------------------------------
		// Shapes->Smooth Slope
		lea		eax, Menu_SmoothSlope
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_SmoothSlope
		push	eax		;Nome menu						;3

		// Shapes->Stepped Slope
		lea		eax, Menu_SteppedSlope
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_SteppedSlope
		push	eax		;Nome menu						;3

		// Shapes->Smooth Slope Ceiling
		lea		eax, Menu_CSmoothSlope
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CSmoothSlope
		push	eax		;Nome menu						;3

		// Shapes->Stepped Slope Ceiling
		lea		eax, Menu_CSteppedSlope
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CSteppedSlope
		push	eax		;Nome menu						;3

		// Shapes->Random Floor up
		lea		eax, Menu_RandomFloorUp
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_RandomFloorUp
		push	eax		;Nome menu						;3

		// Shapes->Random Floor down
		lea		eax, Menu_RandomFloorDown
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_RandomFloorDown
		push	eax		;Nome menu						;3


		// Shapes->Random Ceiling up
		lea		eax, Menu_RandomCeilingUp
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_RandomCeilingUp
		push	eax		;Nome menu						;3

		// Shapes->Random Floor down
		lea		eax, Menu_RandomCeilingDown
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_RandomCeilingDown
		push	eax		;Nome menu						;3

		// Shapes->Set Random Slope parameter
		lea		eax, Menu_SetRandomSlope
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_SetRandomSlope
		push	eax		;Nome menu						;3


		// Shapes->Pyramid Floor
		lea		eax, Menu_Piramide
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_Piramide
		push	eax		;Nome menu						;3
		
		// Shapes->Inverse Piramide Floor
		lea		eax, Menu_InvPiramide
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_InvPiramide
		push	eax		;Nome menu						;3

		// Shapes->Piramide Ceiling
		lea		eax, Menu_CPiramide
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CPiramide
		push	eax		;Nome menu						;3
		
		// Shapes->Inverse Piramide Ceiling
		lea		eax, Menu_CInvPiramide
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CInvPiramide
		push	eax		;Nome menu						;3



		// Shapes-> Bend Floor
		lea		eax, Menu_BendFloor
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_BendFloor
		push	eax		;Nome menu						;3

		// Shapes-> Inverse Bend Floor
		lea		eax, Menu_BendInvFloor
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_BendInvFloor
		push	eax		;Nome menu						;3

		// Shapes-> Bend Ceiling
		lea		eax, Menu_BendCeiling
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CBend
		push	eax		;Nome menu						;3

		// Shapes-> Inverse Bend Ceiling
		lea		eax, Menu_BendInvCeiling
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CBendInv
		push	eax		;Nome menu						;3
		
		// Shapes-> Cupola floor
		lea		eax, Menu_Cupola
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_Cupola
		push	eax		;Nome menu						;3

		// Shapes-> Inverse cupola floor
		lea		eax, Menu_CupolaInv
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CupolaInv
		push	eax		;Nome menu						;3

		// Shapes-> cupola Ceiling
		lea		eax, Menu_CCupola
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CCupola
		push	eax		;Nome menu						;3

		// Shapes-> Inverse Cupola Ceiling
		lea		eax, Menu_CCupolaInv
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CCupolaInv
		push	eax		;Nome menu						;3


		// Shapes->Grid Selected Wall
		lea		eax, Menu_GridSelectedWall
		push	eax		;Procedura 						;1
		push	8057h		;scan code F11				;2
		lea		eax, NomeMenuGridSelectedWall
		push	eax		;Nome menu						;3

		// Shapes->Grid All Walls
		lea		eax, Menu_GridAllWall
		push	eax		;Procedura 						;4
		push	8058h		;scan code F12				;5
		lea		eax, NomeMenuGridAllWall
		push	eax		;Nome menu						;6

		// Shapes->Remove Grid Selected Wall
		lea		eax, Menu_RemoveGridSelectedWall
		push	eax		;Procedura 						;1
		push	00		;								;2
		lea		eax, NomeMenuRemoveGridSelectedWall
		push	eax		;Nome menu						;3

		// Shapes->Remove Grid From all walls
		lea		eax, Menu_RemoveGridAllWall
		push	eax		;Procedura 						;4
		push	0						;5
		lea		eax, NomeMenuRemoveGridAllWall
		push	eax		;Nome menu						;6


		// gruppo menu Shapes
		push	200		;larghezza		1
		push	25		;item				2
		lea		eax, NomeMenu_Shapes
		push	eax					;	3
		// ------------------------------ SCREENSHOTS menu  ------------
		lea		eax, Menu_ShotFullScreen
		push	eax
		push	0
		lea		eax, NomeMenu_ShotFullScreen
		push	eax

		lea		eax, Menu_ShotPanel2d
		push	eax
		push	0
		lea		eax, NomeMenu_ShotPanel2d
		push	eax

		lea		eax, Menu_ShotPanel3d
		push	eax
		push	0
		lea		eax, NomeMenu_ShotPanel3d
		push	eax

		lea		eax, Menu_ShotPreview
		push	eax
		push	0
		lea		eax, NomeMenu_ShotPreview
		push	eax

		lea		eax, Menu_ShotPreviewRGB
		push	eax
		push	0
		lea		eax, NomeMenu_ShotPreviewRGB
		push	eax

		lea		eax, Menu_ShotTextures
		push	eax
		push	0
		lea		eax, NomeMenu_ShotTextures
		push	eax

		lea		eax, Menu_ShotInfoRoom
		push	eax
		push	0
		lea		eax, NomeMenu_ShotInfoRoom
		push	eax

		lea		eax, Menu_ShotLights
		push	eax
		push	0
		lea		eax, NomeMenu_ShotLights
		push	eax


		push	160		;larghezza	;  1
		push	8		;nitem		;  2
		lea		eax, NomeMenu_ScreenShots
		push	eax					;  3

		// Modifiche globali a serie menu
		// imposta numero di gruppi menu
		mov		eax,410cc5h  ; punta a tot gruppi menu
		mov		byte ptr [eax], 11   ;originale era 7

		;==============  modifica valori stack ====================
		// calcola come e' variato stack dall'inizio di aggiunta 
		// argomenti menu
		lea		eax, SalvaStack
		mov		eax, dword ptr[eax]
		sub		eax, esp
	;era 2d0
		add		eax, 2DCh	;valore originale precedente
		mov		ecx, 410cf4h  ;punta a add esp, ...
		mov		dword ptr [ecx], eax
	
		// modifica testo per menu "center"
		lea		ecx, MexCenter
		mov		eax, 410B40h  ;zona di Push MexCenter
		mov		dword ptr [eax], ecx


		// -------  fine zona inserimenti menu o pulsanti

		// TORNARE A ROOM EDIT: Uscire tramite un salto
		push 4229F0h  ;Menu_FindObject
		mov  eax, 4109EAh  ;ritorno a riga successiva a patgch in impostatuttomenu
		jmp  eax
	}
}

void __declspec(naked) Sub_PulsanteDamage(void)
{

	// premuto pulsante Damage
	// bisogna invertire lo stato di flag 0x10 di flag room
	// e poi chiamare funzione per visualizzare lo stato dei pulsanti
	__asm {
		mov ecx, 583BA0h    ;IndiceRoomAttuale
		mov  ecx, dword ptr [ecx]
		mov  eax, ecx
		shl  eax, 05h
		sub  eax, ecx
		mov ecx, 46B850h    ;Ptr_VetRoomSlots
		mov  ecx, dword ptr [ecx]
		lea  eax, [eax][eax*2]
		lea  edx, [ecx][eax*4]
		add  eax, edx
		xor  word ptr [eax+1c8h], 10h
		lea  eax, TotCambiBackup
		add dword ptr [eax] ,1
		mov  eax, 414430h ;MostraStatoPulsanti
		jmp  eax
		
	}
}
// lavora su TestMappaVerticale
void Sub_PulsanteV2dMap(StrInfoMouse *pDatiPulsante)
{
	int ColoreFondo;
	int *pTipoModoView = (int*) 0x583BD8;

	TestMappaVerticale ^= 1;

	if (TestMappaVerticale == 0) {
		// visuaslizza pulsante sollevato  e con colore grigio
		ColoreFondo = 1;
	}else
	{	// visualizza pulsante premuto e con colore di fondo giallo
		ColoreFondo= 10;
	}
	ModificaPulsante(&DatiPulsanteV2dMap, ColoreFondo, 0, TestMappaVerticale);
	

	if (*pTipoModoView == 1) {
		__asm {
			mov	eax,1
			push eax
			mov eax, 4266F0h    ;MostraPianta2d
			call eax
			add  esp, 04h
		}
	}else
	{
		if (TestMappaVerticale==1) {


			__asm {
				mov  eax, 4260F0h  ; Sub_Pulsante_2D_Map
				push	0
				call	eax
				add		esp,4
				push  4590BCh ;DatiPulsante_2D_Map
				mov eax, 433FE0h    ;BlinkPulsante1
				call eax
				add		esp,4
				push  4590BCh ;DatiPulsante_2D_Map
				mov eax, 433E20h    ;BlinkPulsante2
				call eax
				add	esp,4
			}
		}

	}
}

void Sub_PulsanteBigTexture(StrInfoMouse *pDatiPulsante)
{
	DWORD *pTestBigTexture = (DWORD *) 0x454ffc;
	DWORD TestPremuto, ColoreFondo;


	if (VersioneProject != 50 && VersioneProject !=51) {
		*pTestBigTexture ^= 1;
		VersioneProject=49;
	}else {
		*pTestBigTexture = 1;
	}


	if (*pTestBigTexture == 0) {
		// visuaslizza pulsante sollevato  e con colore grigio
		ColoreFondo = 1;
	}else
	{	// visualizza pulsante premuto e con colore di fondo giallo
		ColoreFondo= 10;
	}
	TestPremuto = *pTestBigTexture;
	if (VersioneProject > 49) {
		ColoreFondo=4;
	}

	ModificaPulsante(&DatiPulsanteBigTexture, ColoreFondo, 0, TestPremuto);

}
// rimuove o rimette gli oggetti di stanza attuale
void Sub_PulsanteRemoveItems(StrInfoMouse *pDatiPulsante)
{
	int TestPremuto, ColoreFondo;
	char *pTesto;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;
	pTesto = "Temporary hiding of all items from current room to facilitate texturizing operations\r\n\r\nREMEMBER to release the [Hide Objects] button before perform following operations:\r\n\r\n* Adding new objects or triggers\r\n* Saving project\r\n* Output wad";

	if ((MyPrefRoom.Flags & FPR_NO_INFO_HIDE_OBJECTS)==0 && TestRemoveItems==0) {
		if (TestRemoveItems==0 && MostraMessageBoxNGLE(pTesto,0x0f)==0) return;
	}
	if (TestRemoveItems==0) {
		// eseguire backup senza chiedere
		Sub_PulsanteBackup((StrInfoMouse *) 1);
		// salvare in OldNumeroObjects il numero di oggetti di stanza attuale
		// e poi reimpostarlo quando il pulsante viene rilasciato
		OldNumeroObjects = pVetRooms[*pIndiceRoomAttuale].N_ObjAndTriggers;
		OldEffects = pVetRooms[*pIndiceRoomAttuale].N_Effects;
		pVetRooms[*pIndiceRoomAttuale].N_ObjAndTriggers = 0;
		pVetRooms[*pIndiceRoomAttuale].N_Effects=0;
		SalvaIndiceRoom = *pIndiceRoomAttuale;
	}else
	{
		// ripristinare numero originale
		pVetRooms[SalvaIndiceRoom].N_ObjAndTriggers=OldNumeroObjects;
		pVetRooms[SalvaIndiceRoom].N_Effects=OldEffects;
	}
	TestRemoveItems ^= 1;
	if (TestRemoveItems == 0) {
		// visuaslizza pulsante sollevato  e con colore grigio
		ColoreFondo = 1;
	}else
	{	// visualizza pulsante premuto e con colore di fondo giallo
		ColoreFondo= 10;
	}
	TestPremuto = TestRemoveItems;


	ModificaPulsante(&DatiPulsanteRemoveItems, ColoreFondo, 0, TestPremuto);

	// mostrare nuovamente stanza
	RefreshStanzaAttuale();
}

// da chiamare prima di tutte quelle operazioni dove ci sarebbero problemi se c'era [hide objects]
// e si esce dalla stanza o si salva o usa il wad
void IsToDisableRemoveItems(void)
{
	if (TestRemoveItems==0) return;

	Sub_PulsanteRemoveItems(NULL);
}
// cambia TipoMappaVerticale
// possibili valori
// 0 = visione da sud
// 1 = visione da ovest
// 2 = visione da nord
// 3 = visione da est
void Sub_PulsanteVisuale2d(StrInfoMouse *pDatiPulsante)
{
	TipoMappaVerticale++;
	TipoMappaVerticale &= 3;

	// visualizzare nuovo nome nel pulsante
	ModificaPulsanteEx(&DatiPulsanteVisuale2d, 1, 0, 0, 
				VetNomiVisuale[TipoMappaVerticale]);
	__asm {
		mov eax, 401200h    ;CicloPrincipale
		call eax
	}
	
}
// avvia programma visualizzato in combo
void Sub_PulsanteGo(StrInfoMouse *pDatiPulsante)
{
	int Indice;
	if (MyPrefRoom.TotExternPrograms == 0) {
		MostraMessageBoxNGLE("No program to launch. Please, click on [Settings] button to select the programs you wish launch with this command",3);
		return;
	}
	Indice = SendMessage(WindComboProgrammi,CB_GETCURSEL,0,0);
	if (Indice == CB_ERR) {
		MostraMessageBoxNGLE("No program selected",3);
		return;
	}

	StartProgrammaEsterno(Indice);
}
void DisableFlipMap(void)
{
	__asm {
		// se modo flipmap e' attivo disattivarlo
		mov eax, 583BF0h    ;TestMostraFlipMap
		mov  eax, dword ptr [eax]
		test eax, eax
		jz   Salto1
		push 00h
		mov eax, 40E130h    ;Menu_FlipMap
		call eax
		add  esp, 04h
Salto1:
	}
}
int SalvaProgetto(char *NomeFile)
{
	DisableFlipMap();
	return SaveProject(NomeFile);
}
// se il puntatore = 1 NON chiede di modificare il nome del progetto
void Sub_PulsanteBackup(StrInfoMouse *pDatiPulsante)
{
	// premuto pulsante Backup
	char *pNomeProject = (char*) 0x46B858;  // BufferNomeProject
	char NomeFile[256];
	char *pDirSrc;
	unsigned int i;
	char *pParteNome;
	char *pTemp;
	char *pInizio;
	char Buffer[256];
	int *pTestFlipMap = (int *) 0x583BF0;
	HWND *pMiaWind = (HWND*) 0x46E559;
	char *pNome;

	if (EsisteFile(pNomeProject)==false) {
		DisegnaRettangolo(296,699, 0x1fe, 10, 1);
		ShowTestoSuSchermo(296,699, "NO FILE LOADED");
		return;
	}

	IsToDisableRemoveItems();

	pDirSrc = SoloDir(pNomeProject);
	pParteNome = SoloNome(pNomeProject);
	
	// analisi per vedere se questo nome e' gia' un nome di bakcup
	// che inizia con "BKNNN_...
	pInizio = pParteNome;

	pTemp = Search(pInizio,"BK",ts_Avanti,NULL);
	if (pTemp == pInizio) {
		pTemp += 2;    //saltare i due caratteri
		// vedere se ora ci sono solo caratteri numericii fino a _
		

		for (i = 0;i<strlen(pTemp);i++) {
			if (pTemp[i] == '_') {
				// si, quindi usare punto successivo come nome
				pInizio = pTemp + i + 1;
				break;
			}
			if (pTemp[i] < '0' || pTemp[i] > '9') break;

		}

	}

	// ok, ora provare con tutti i numeri possibili finche si trova un file assente

	i = 1;

	do {
		sprintf(NomeFile,"%s\\BK%d_%s", pDirSrc, i, pInizio);
		i++;
	} while (EsisteFile(NomeFile)==true);

	// ok, ora salvare il progetto con questo nome:
	if (*pTestFlipMap != 0) Sub_PulsanteFlipMap(0);
	
	if (pDatiPulsante == (StrInfoMouse*) 1) {
		pNome = NomeFile;
	}else
	{
		pNome = InputTextBox("Save current project with following name. (It will be keeped same folder)\r\nRemark: Following name assures to avoid overwritting of previous project files", 
			"Backup Project", SoloNomeSenzaExt(NomeFile), TP_CENTRALE);
		if (pNome==NULL) return;
	}

	sprintf(NomeFile,"%s\\%s.prj", pDirSrc,SoloNomeSenzaExt(pNome));
	if (EsisteFile(NomeFile)==true) {
		sprintf(BufferLog,"Project file \"%s\" already exist. Do you want replace it?", 
				pNome);
		if (MostraMessageBoxNGLE(BufferLog, 0x0f) == 0) return;
	}
	
	if (SalvaProgetto(NomeFile) == 0) {
		sprintf(Buffer,"Error trying to save backup file in: %s", NomeFile);

		MostraMessageBoxNGLE(Buffer,3);

	}else	{

		pTemp = SoloNome(NomeFile);
		sprintf(Buffer,"LAST BACKUP: %s", pTemp);
		ShowInfoRoom(Buffer);
		MessageBeep(-1);
		TotCambiBackup=0;


	}


}

void PatchSize_2000_Puntatori(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x41FDB3, 'D'},
		{0x4297A3, 'D'}};

	TotPatchs=2;
	TotOld = 0x1F40;
	TotNew=0x7D00;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchSize_2000_Objects(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x41FDA4, 'D'}};

	TotPatchs=1;
	TotOld = 0x14820;
	TotNew=0x3D860;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchTot_2000_Objects(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x416C75, 'D'},
		{0x41719F, 'D'},
		{0x41FDF0, 'D'},
		{0x423885, 'D'},
		{0x423958, 'D'},
		{0x423D65, 'D'},
		{0x4246F1, 'D'},
		{0x426D17, 'D'},
		{0x426D3A, 'D'},
		{0x426D4B, 'D'},
		{0x428BEB, 'D'},
		{0x42907A, 'W'},
		{0x4297B3, 'D'},
		{0x429B19, 'D'},
		{0x437AEA, 'D'},
		{0x437CC2, 'D'}};

	TotPatchs=16;
	TotOld = 0x7D0;
	TotNew=0x1770;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchTutto2000Oggetti(void)
{
	PatchSize_2000_Puntatori();
	PatchSize_2000_Objects();
	PatchTot_2000_Objects();
}

void PatchMaxBoxOverlap(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x406A61, 'D'},
		{0x406B1A, 'D'}};

	TotPatchs=2;
	TotOld = 0x3A98;
	TotNew=0x4000;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}
// room editor
void PatchZonaOverlaps(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pZonaBoxOverlapped;

	StrPatch VetPatch[] = {{0x406A7F, true},
		{0x406A90, true},
		{0x406AA2, true},
		{0x406B3A, true},
		{0x406B4B, true},
		{0x406B5D, true},
		{0x406B7B, true},
		{0x406CF0, true},
		{0x4326BC, true}};

	OffsetOldInizio = 0x591D40;
	OffsetOldFine=0x599270;
	TotPatchs=9;
	pZonaBoxOverlapped = (DWORD *) 0x45462E;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
			OffsetOldFine, 0x8000,"ZonaOverlaps");
	*pZonaBoxOverlapped = OffsetNew;
}

// viene chiamata quando utente ha fatto la scelta di uscire con "ok"
// o doppio click o enter, da un list box window
// si limita a prerndre indice effettivo selezionato e mandarlo come
// argomento del messaggio PostQuitMessage

void SceltoFile(HWND hDialogo, HWND lp_WindListBox, int TipoLista)
{
	int Indice;
	char *pLinea;
	char **pVetParti;
	int TotParti;

	Indice = SendMessage(lp_WindListBox,LB_GETCURSEL,0,0);
	if (Indice == LB_ERR) {
		EndDialog(hDialogo,-1);
		return;
	}



	if (TipoLista == TLB_RemapObjects ||
		TipoLista == TLB_RemapRooms) {	
		// attenzione se la lista e' di tipo object remap
		// o room remap catturare indice da ultima colonna
		// della linea selezionata{
		SendMessage(lp_WindListBox,LB_GETTEXT, (WPARAM) Indice, 
					(LPARAM)BufferLog);
		pLinea = Trim(BufferLog);

		pVetParti = Split(pLinea,' ',&TotParti,NULL,NULL,false);
		pLinea = pVetParti[TotParti-1];

		sscanf(pLinea,"%d",&Indice);			

	}else {
	// otenere indice reale 
		Indice= SendMessage(lp_WindListBox,LB_GETITEMDATA,Indice,0);
	}
	EndDialog(hDialogo,Indice);





}
void PatchTuttoOverlap(void)
{
	PatchMaxBoxOverlap();
	PatchZonaOverlaps();
}


// riordina nomi sulla base di ordine alfabetico
void OrdinaPuntaList(void)
{
	int i,j;
	int Temp;
	int Indice1;
	int Indice2;

	TestListOrdinato=true;	
	
	for (i=0;i<TotList;i++) VetPuntaList[i]=i;

	for (i=0;i<TotList-1;i++) {
		for (j=i+1;j<TotList;j++) {
			Indice1= VetPuntaList[i];
			Indice2= VetPuntaList[j];

			if (strcmpi(VetNomiList[Indice1],VetNomiList[Indice2]) > 0) {
				// invertire
				Temp = VetPuntaList[i];
				VetPuntaList[i] = VetPuntaList[j];
				VetPuntaList[j]= Temp;
			}
		}
	}

}
// sostituisce con "_" gli spazi della stringa pLinea

void SottolineaTesto(char *pLinea)
{
	DWORD i;

	for (i=0;i<strlen(pLinea);i++) {
		if (pLinea[i] == ' ') pLinea[i] = '.';
	}



}

// riempie list box con dati in VetNomiList[] VetPuntaList[] TotList
// nota: Se TestItemData = true i valori in vetpuntalist vengono inseriti in itemdata
//		 Set TestItemData = false i valori in vetpuntalist sono usati come indici per ordinamento
void ShowLista(HWND WindList, bool TestItemData )
{
	int i;
	char *pNome;

	int Indice;
	
	SendMessage(WindList,LB_RESETCONTENT,0,0);

	// con dati in VetNomiList[]
	// secondo oridne di VetPuntaList[]
	for (i=0;i<TotList;i++) {
		if (TestItemData==true) {
			Indice=i;
		}else{

			Indice = VetPuntaList[i];
		}
		if (strlen(VetNomiList[Indice])>0) {
			// aggiungere questo elemento

			pNome = VetNomiList[Indice];
			
				
			Indice=SendMessage(WindList,LB_ADDSTRING,0,(LPARAM) pNome);
			SendMessage(WindList,LB_SETITEMDATA,Indice,(LPARAM) VetPuntaList[i]);
		}
		
	}
	// seleziona il primo della lista
	if (TipoListBox== TLB_ListaOggetti && UltimoIndiceOggetto !=-1)  {
		// selezionare l'ultimo indice
		SelezionaListBoxValore(WindList, UltimoIndiceOggetto);
		
	}else {

		SendMessage(WindList,LB_SETCURSEL,0,0);
	}
	SetFocus(WindList);
}
// inserisce in VetNomiList / VetPuntaList TotList
// gli elementi di tipo TLF_ ... e visualizza la lista nel list boc
// e inizializza BaseFind
void RiempieListaFind(HWND WindList, int TipoListaFind)
{
	int		i;
	static StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	static int *pN_WasSlots = (int*) 0x56D9F0;
	pVetSlotOggetti = *p2VetSlotOggetti;

	static char **pVetNomiTriggerActivation = (char**) 0x569C00;
	static int *pTotNomiTriggerActivation = (int*) 0x569CA0;
	char **pVetNomiTriggerWhat = (char**) 0x569CC0;
	static int *pTotNomiTriggerWhat = (int*) 0x569B88;
	static DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	static StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	bool TestIgnora;
	static int *pN_UsedObjectCodes = (int *) 0x56C970;
	static int **Ptr2_ObjectCodeArray = (int**) 0x46B810;
	int *pObjectCodeArray;
	int Indice;
	char MexIndice[20];
	char MexNome[70];
	int Slot;
	int Tipo;
	bool TestLaraPos;

	BaseFind.Incremento=1;
	BaseFind.IndiceAlfa=0;
	BaseFind.IndiceBeta = 0;
	BaseFind.IndiceRoom = *pIndiceRoomAttuale;
	BaseFind.TipoRicerca = TipoListaFind;
	TotList=0;


	switch (TipoListaFind) {
	case TLF_OBJECTS:

		// riempire totlista vetlnomipunta vetpuntalist con dati di objects

		
		for (i=0;i<*pN_WasSlots;i++) {
			if (pVetSlotOggetti[i].Tipo == 0x10 || 
				pVetSlotOggetti[i].Tipo == 0x110) {


				strcpy(VetNomiList[TotList],pVetSlotOggetti[i].pNome);
				VetPuntaList[TotList] = i;

				TotList++;
			}
		}


		break;
	case TLF_TRIGGER_OF_PLUGIN:
		// mettere lista di nomi plugin
		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			strcpy(VetNomiList[TotList], GlobNgle.VetPluginNgle[i].Nome);
			VetPuntaList[TotList] = GlobNgle.VetPluginNgle[i].PluginId;
			TotList++;
		}
		break;

	case TLF_TRIGGER_UNHANDLED:
		// mettere lista di possibili ID
		strcpy(VetNomiList[TotList], "With ANY ID");
		VetPuntaList[TotList]=-1;
		TotList++;

		for (i=1;i<256;i++) {
			sprintf(MexIndice, "With ID=%3d", i);
			strcpy(VetNomiList[TotList], MexIndice);
			VetPuntaList[TotList]=i;
			TotList++;
		}
		break;

	case TLF_INDICES:
		// indici degli item
		pVetBaseOggetti= *P2_VetBaseOggetti;
		pObjectCodeArray = *Ptr2_ObjectCodeArray;
		for (i=0;i<*pN_UsedObjectCodes;i++) {
			Indice= i;

			TestLaraPos=false;

			if (pVetBaseOggetti[Indice].TypeObject) {
				sprintf(MexIndice, "%d", Indice);
				TestIgnora=false;
				if (pVetBaseOggetti[Indice].TypeObject == 8) {

					Slot= pVetBaseOggetti[Indice].SlotIndex;
					sprintf(MexNome, "%s", pVetSlotOggetti[Slot].pNome);
					if (Slot == 406) {
						TestLaraPos =true;
					
					}
				}else {
					// non e' moveable o static
					Tipo=roomGetCategoriaTipoOggetto(pVetBaseOggetti[Indice].TypeObject);

					switch (Tipo) {
					case 1:
						strcpy(MexNome, "Door");
						TestIgnora=true;
						break;
					case 2:
						strcpy(MexNome, "Light");
						TestIgnora=true;
						break;
					case 0x10:
						strcpy(MexNome, "Trigger");
						TestIgnora=true;
						break;
					case 0x20:
						strcpy(MexNome, "Fly/Fix/Camera");
						break;
					case 0x40:
						strcpy(MexNome, "Sink");
						break;
					case 0x80:
						strcpy(MexNome, "Sound");
					default:
						strcpy(MexNome, "Unknown??");
						break;
					}
				}
					
				if (TestIgnora==false) {
					if (TestLaraPos==true) {
						sprintf(BufferLog, "  ocb: %5d (index=%5d) %s", 
								pVetBaseOggetti[Indice].OCBCode, Indice, MexNome);

					}else {

						sprintf(BufferLog, "index: %5s (OCB=%5d) %s", MexIndice, 
											pVetBaseOggetti[Indice].OCBCode,  MexNome);
					}
					strcpy(VetNomiList[TotList], BufferLog);
					VetPuntaList[TotList] = Indice;
					TotList++;
				}
			}
		}
		break;

	case TLF_FLOOR_TYPE:
		// status floor

		for (i=0;i<24;i++) {
			strcpy(VetNomiList[i], VetNomiFloor[i]);
			VetPuntaList[i] = VetFlagFloor[i];
		}

		TotList = 24;

		break;
	case TLF_TRIGGER_ACTIVATION:
		// trigger attivazione
		for (i=0;i<*pTotNomiTriggerActivation;i++) {
			strcpy(VetNomiList[i], pVetNomiTriggerActivation[i]);
		}
		TotList = *pTotNomiTriggerActivation;
		for (i=0;i<TotList;i++) VetPuntaList[i] =i;
		break;

	case TLF_TRIGGER_WHAT:
		// trigger cosa attivare
		for (i=0;i< *pTotNomiTriggerWhat;i++) {
			strcpy(VetNomiList[i], pVetNomiTriggerWhat[i]);
		}
		TotList = *pTotNomiTriggerWhat;
		for (i=0;i<TotList;i++) VetPuntaList[i] =i;
		break;
	case TLF_EFFECTS:
		// effect s
		for (i=0;i<TotEffects;i++) {
			strcpy(VetNomiList[i], VetNomiEffects[i]);
			VetPuntaList[i] = VetIdEffects[i];

		}
		TotList = TotEffects;
		break;

	}



	ShowLista(WindList,true);


}
char* GetDataFile(char* pNomeFile)
{
	SYSTEMTIME DataInizio;
	SYSTEMTIME  DataOk;
	FILETIME lpTime;
	HANDLE	InFile;
	int Giorno,Mese,Anno;
	static char MyDataLog[256];

     InFile=CreateFile(pNomeFile,GENERIC_READ,0,NULL,OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,NULL);

	if (InFile == INVALID_HANDLE_VALUE) return NULL;



	 GetFileTime(InFile, NULL,NULL,&lpTime);
	 FileTimeToSystemTime(&lpTime,&DataInizio);
	 // converte alla data loale
	 SystemTimeToTzSpecificLocalTime(NULL, &DataInizio, &DataOk);

	 CloseHandle(InFile);
	 Giorno = DataOk.wDay;
	 Mese= DataOk.wMonth;
	 Anno = DataOk.wYear;


	 sprintf(MyDataLog,"%d %s %d  (%d:%d:%d)", Giorno, pVetMesi[Mese], Anno, 
			DataOk.wHour , DataOk.wMinute, DataOk.wSecond );
	return MyDataLog;
	

}
// restituisce la data dell'ultima modifica di pNomeFile
void GetDataDelFile(char *pNomeFile, FILETIME *pFileTime)
{
	HANDLE	InFile;

     InFile=CreateFile(pNomeFile,GENERIC_READ,0,NULL,OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,NULL);

	if (InFile == INVALID_HANDLE_VALUE) return;
	 GetFileTime(InFile, NULL,NULL,pFileTime);
	 CloseHandle(InFile);

}
// viene chiamata quando cambia la selezione nel list box selezione
void CambiataSelList(HWND hDialogo, HWND WindList, int TipoListBox)
{
	char BufferTemp[256];
	char *pNomeFile;
	char *pData;
	int	Indice;


	BufferTemp[0]=0;
	Indice = SendMessage(WindList,LB_GETCURSEL,0,0);
	if (Indice != LB_ERR) {

		// otenere indice reale 
		Indice= SendMessage(WindList,LB_GETITEMDATA,Indice,0);	
		switch (TipoListBox){
		case TLB_LoadLastProjects:
			// visualizzare informazioni sul file
			// ottenere la data
			pNomeFile = MyPrefRoom.VetOldProjects[Indice].NomeProject;
			pData = GetDataFile(pNomeFile);
			if (pData != NULL) {
				sprintf(BufferTemp,"%s - Last Change Date: %s", SoloNome(pNomeFile), pData);

			}
			break;
		default:
			strcpy(BufferTemp,VetNomiList[Indice]);
			
		}


	}
	SetDlgItemText(hDialogo,IDC_LABEL_INFO, BufferTemp);


}

// estrare tutto il contenuto del list box (incluso l'header) e lo inserisce in un testo
// poi visualizza il file 
void ShowTextList(HWND hDialogo, HWND WindList, bool TestTriggerWindow)
{
	DWORD		i;
	char  BufferTemp[256];
	char   NomeFile[] = "outlist.txt";
	FILE *pTemp;
	DWORD		TotList;
	char BufLinea[256];
	int Indice;
	WORD TipoMessaggio;



	if (TestTriggerWindow) {
		TotList =SendMessage(WindList,CB_GETCOUNT,0,0);
		strcpy(BufferTemp, "#ID  DESCRIPTION");

		for (i=0;i<80;i++) {
			BufLinea[i]='-';
		}
		BufLinea[i]=0;

		TipoMessaggio = CB_GETLBTEXT;
	}else {
		TipoMessaggio = LB_GETTEXT;
		// lista per oggetti
		TotList =SendMessage(WindList,LB_GETCOUNT,0,0);
		GetDlgItemText(hDialogo,IDC_LABEL_COLONNE, BufferTemp,255);
		for (i=0;i<90;i++) {
			BufLinea[i] = '-';
		}
		BufLinea[i]=0;
	}

	pTemp = fopen(GetFileTemp(NomeFile),"wb");

	if (pTemp != NULL) {
		fprintf(pTemp, "%s\r\n", BufferTemp);
		fprintf(pTemp,"%s\r\n", BufLinea);



		for (i=0;i<TotList;i++) {
			SendMessage(WindList,TipoMessaggio,i,(LPARAM) BufferTemp);

			if (TestTriggerWindow) {
				Indice = SendMessage(WindList, CB_GETITEMDATA, i,0);
				fprintf(pTemp,"%3d: %s\r\n", Indice, BufferTemp);
			}else {
				fprintf(pTemp,"%s\r\n",BufferTemp);
			}
		}
		fprintf(pTemp,"%s\r\n",BufLinea);
		if (TestTriggerWindow) {
			fprintf(pTemp,"Total Items = %d\r\n", TotList);


		}
		fclose(pTemp);
		ShellExecute(NULL,"open",GetFileTemp(NomeFile),"","",SW_SHOW);		
	}




}
// estrare tutto il contenuto del list box (incluso l'header) e lo inserisce in un testo
// poi visualizza il file 
void ShowListFind(HWND hDialogo, HWND WindList)
{
	DWORD		i;
	char  BufferTemp[256];
	char   NomeFile[] = "OutFindList.txt";
	FILE *pTemp;
	DWORD		TotList;
	DWORD Valore;



	pTemp = fopen(GetFileTemp(NomeFile),"wb");
	if (pTemp != NULL) {


		TotList =SendMessage(WindList,LB_GETCOUNT,0,0);

		for (i=0;i<TotList;i++) {
			SendMessage(WindList,LB_GETTEXT,i,(LPARAM) BufferTemp);
			Valore=SendMessage(WindList,LB_GETITEMDATA,i,0);

			fprintf(pTemp,"[%2d:%2d]: %s\r\n",i, Valore, BufferTemp);
		}

		fclose(pTemp);
		ShellExecute(NULL,"open",GetFileTemp(NomeFile),"","",SW_SHOW);		
	}




}

// crea un dc compabtile con WindHdc e ci carica il bitmap delle texture
// poi restituisce l'hdc compatibile
HDC CreaDCTexture(HDC WindHdc)

{

	HDC CompHdc;


	CompHdc = CreateCompatibleDC(WindHdc);

	OldBmp= (HBITMAP) SelectObject(CompHdc,HandleBitMap);
	return CompHdc;

}


void EliminaDCTexture(HDC CompHdc)
{
	SelectObject(CompHdc, OldBmp);
	DeleteDC(CompHdc);

}

// Nome: RettangoloPieno
// input: HDC hdcOut,WORD x1, WORD y1, WORD x2, WORD y2
// scopo: Disegna rettangolo in hdcOut con brush attuale senza bordo. Corregge
// scopo: le anomalie della funazione Rectangle. Le due coppie di coordinate
// scopo: sono comprese nella zona disegnata
// note: Seleziona penna nulla in hdcOut

void RettangoloPieno(HDC hdcOut,int x1, int y1, int x2, int y2, HBRUSH BrushColore)
{
	SelectObject(hdcOut,PenNulla);
	SelectObject(hdcOut,BrushColore);
	Rectangle(hdcOut,x1,y1,x2+2,y2+2);
}

// dopo modifica a valore combo_tipoanim, verificva se combo  rotate
// deve essere mostrato o no
void MostraComboRotate(HWND hDialogo)
{
	WORD TempWord;
	int i;
	HWND TempWind;

	TempWind = GetDlgItem(hDialogo,IDC_COMBO_TIPOANIM);
	i =SendMessage(TempWind,CB_GETCURSEL,0,0);
	if (i == CB_ERR) i =0;


	TempWord  = (WORD) SendMessage(TempWind, 
									CB_GETITEMDATA,i,0);

	TempWind = GetDlgItem(hDialogo, IDC_COMBO_UVROTATE);
	if (TempWord & FAN_ANY_ROTATE) {
		// mostrare combo uvrotate
		ShowWindow(TempWind,SW_SHOW);
	}else {
		// nascondere uvrotate
		ShowWindow(TempWind,SW_HIDE);
	}
}

// disegna rettangolo con penna attuale (niente dentro)
void Rettangolo(HDC hdcOut,int x1, int y1, int x2, int y2, HPEN PennaBordo)
{
	SelectObject(hdcOut,BrushNullo);
	SelectObject(hdcOut,PennaBordo);
	Rectangle(hdcOut,x1,y1,x2+2,y2+2);
}
// cerca l'indice relativo (solo delle tex esportate) fornendo
// l'inddice tail originale
// se non ci riesce resittuisce -1
int TrovaIndiceRelativoTail(StrDatiTexture *pVetTex, int TotTex,int IndiceOldTail)
{
	int i;

	for (i=0;i<TotTex;i++) {
		if (pVetTex[i].IndiceOld == IndiceOldTail) return i;
	}

	sprintf(BufferLog,"ERRORE: non trovo indice relativo per tail con indiceoldfull=%d", IndiceOldTail);
	InviaLog(BufferLog);
	return -1;

}

void SelezionaTexture(int IndiceTexture, HPEN PenColore)
{
	int IndiceStart,IndiceEnd;
	int Indice;
	int	x,y;
	int Size;

	// prima verifcare che la texture sia nell'area inquadrata
	IndiceStart = DatiTexture.IndicePrimaTexture;
	IndiceEnd  = IndiceStart + DatiTexture.GrigliaDstX * DatiTexture.GrigliaDstY;
	IndiceEnd--;

	if (IndiceTexture < IndiceStart || IndiceTexture > IndiceEnd) return;

	// ora trovare le coordinate nella griglia destinazione
	Indice = IndiceTexture - DatiTexture.IndicePrimaTexture;

	x = Indice % DatiTexture.GrigliaDstX;
	y = Indice / DatiTexture.GrigliaDstX;

	Size = DatiTexture.SizeDst + DatiTexture.Gap;

	x *= Size;
	y *= Size;

	x += DatiTexture.BaseX;
	y += DatiTexture.BaseY;

	x++;
	y++;



	Rettangolo(DatiTexture.MioHdc,x,y,x+Size-3,y+Size-3, PenColore);



}

void DrawRetta(HDC hdcOut,int x,int y,int x2,int y2, HPEN ColorePenna)
{
	SelectObject(hdcOut,ColorePenna);

	MoveToEx(hdcOut,x,y,NULL);
	LineTo(hdcOut,x2,y2);
}
// disegna texture nell'hdc miohdc. usa i dati globali di DatiTexture
void DisegnaTexture(void)
{
	int	x,y;
	int	IndiceNow;
	int OrgX,OrgY,SizeQuadrato;
	int	SizeSrc;
	int Gap;
	int	IndiceSerie;
	int SizeDst;
	int SrcX,SrcY;
	int x1,y1,x2,y2;
	int **p2VetAnimatedTiles = (int**) 0x454646;
	int *VetAnimatedTiles;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int	*VetAnimCodes = (int*) 0x56BF80;
	int *pTotAnimCodes = (int*) 0x56C420;
	int IndiceInizio,IndiceFine;
	VetAnimatedTiles = *p2VetAnimatedTiles;
	HPEN ColorePenna;

	if (DatiTexture.Ready != true) return;
	RiordinaRangeAnimati();	
	SizeQuadrato = DatiTexture.Gap + DatiTexture.SizeDst;
	SizeSrc=DatiTexture.SizeSrc;
	SizeDst=DatiTexture.SizeDst;
	Gap=DatiTexture.Gap ;
	IndiceInizio=-1;
	IndiceFine=-1;
	if (DatiTexture.SelInizio !=-1 ) {

		IndiceInizio = DatiTexture.SelInizio;
		IndiceFine= DatiTexture.SelFine;
		if (IndiceFine == -1 && DatiTexture.TipoTexture == TT_SONORE) {
			IndiceFine=IndiceInizio;
		}else {

			if (IndiceFine < IndiceInizio) {
				// invertire
				x = IndiceInizio;
				IndiceInizio=IndiceFine;
				IndiceFine=x;
			}
		}
	}
		
	for (y=0;y<DatiTexture.GrigliaDstY;y++) {
		for (x=0;x<DatiTexture.GrigliaDstX;x++) {
			OrgX = DatiTexture.BaseX + SizeQuadrato * x;
			OrgY = DatiTexture.BaseY + SizeQuadrato * y;

			// calcolare indice complessivo di texture attuale rispetto a mappa sorgente
			IndiceNow = y*DatiTexture.GrigliaDstX  + x +
						DatiTexture.IndicePrimaTexture;

			if (IndiceNow < DatiTexture.TotTexture ) {

				SrcX = IndiceNow % DatiTexture.GrigliaSrcX;
				SrcY = IndiceNow / DatiTexture.GrigliaSrcX;
				// visuslizzare texture
				x1 = OrgX+Gap/2;
				y1 = OrgY+Gap/2;
				x2 = x1 + SizeDst;
				y2 = y1 + SizeDst;
				
				// cacolare colore penna per box
				ColorePenna = PenNero;
				switch (DatiTexture.TipoTexture) {
				case TT_SONORE:
					if (IndiceNow >= IndiceInizio && 
						IndiceNow <= IndiceFine ) ColorePenna=PenRosso;
					break;
				case TT_ANIMATE:

					// vedere se IndiceNow fa parte di una serie animata
					if (IndiceNow < 256) {
						IndiceSerie = VetAnimatedTiles[IndiceNow];

						if (IndiceSerie != -1) {
							if (IndiceSerie == DatiTexture.IndiceSelSerie ) 
								ColorePenna=PenRosso;
							else
								ColorePenna=PenVerde;
							break;
						}
						
						// se texture attuale fa parte di selezione corrente
						// usare il rosso
						if (IndiceInizio != -1) {
							if (IndiceNow >= IndiceInizio && 
								IndiceNow <= IndiceFine) {
								ColorePenna=PenRosso;
								break;
							}
						}

					}

					break;
				}


				SelezionaTexture(IndiceNow,ColorePenna);

				StretchBlt(DatiTexture.MioHdc,x1,y1,SizeDst,SizeDst, 
						DatiTexture.CompHdc , SrcX*SizeSrc,SrcY*SizeSrc,SizeSrc,SizeSrc,SRCCOPY);


				// se questa texture e' dalla 256 in poi mettere un segnale
				// per evidenziare che non si puo' usare
				if (IndiceNow >= 256) {
					DrawRetta(DatiTexture.MioHdc,x1,y1,x2,y2,PenRosso);

					DrawRetta(DatiTexture.MioHdc,x2,y1,x1,y2,PenRosso);
				
					

				}
			}else
			{
				// disegnare box nero
				RettangoloPieno(DatiTexture.MioHdc,OrgX,OrgY, 
						OrgX + SizeQuadrato, OrgY + SizeQuadrato,BrushNero);
			}
		}
	}



}

// usa DatiTexture per calcolare la nuova griglia di destinazione
// sulla base del nuovo valore di SizeDst
// imposta anche nuovi valori per scroll bar
void RicalcolaDatiTexture(HWND WindScroll)
{
	int	SizeTex;
	int MaxValore;

	SCROLLINFO ScrollInfo;

	SizeTex = DatiTexture.SizeDst;
	if (SizeTex==64) {
		DatiTexture.GrigliaDstX = 10;
		DatiTexture.GrigliaDstY = 6;
		DatiTexture.BaseX = 8;
		DatiTexture.BaseY = 6;
	}else{
		DatiTexture.GrigliaDstX = 5;
		DatiTexture.GrigliaDstY = 3;
		DatiTexture.BaseX = 21;
		DatiTexture.BaseY = 13;
	}


	SizeTex += DatiTexture.Gap;

	
	DatiTexture.IndicePrimaTexture =0;
	DatiTexture.SelInizio = -1;
	DatiTexture.SelFine = -1;
	DatiTexture.IndiceSelSerie = -1;

	MaxValore=DatiTexture.TotTexture / DatiTexture.GrigliaDstX;
	if ((DatiTexture.TotTexture % DatiTexture.GrigliaDstX)==0) MaxValore--;

	MaxValore -= DatiTexture.GrigliaDstY - 1;

	DatiTexture.ScrollMax = MaxValore;



	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMin = 0;
	ScrollInfo.nMax = MaxValore;
	ScrollInfo.nPage = 1;
	ScrollInfo.nPos = 0;
	
	SetScrollInfo(WindScroll,SB_CTL,&ScrollInfo,TRUE);

	DatiTexture.Ready = true;
	RettangoloPieno(DatiTexture.MioHdc,0,0,
					DatiTexture.ScreenX , DatiTexture.ScreenY, BrushNero);
	DisegnaTexture();



	

}

void SelezionaComboValore(HWND WindCombo, int Valore)
{

	int TotItem;
	int i, Indice;

	TotItem = SendMessage(WindCombo, CB_GETCOUNT,0,0);
	Indice=-1;
	for (i=0;i<TotItem;i++) {
		if (SendMessage(WindCombo,CB_GETITEMDATA,i,0) == Valore) {
			Indice=i;
			break;
		}
	}
	SendMessage(WindCombo,CB_SETCURSEL,Indice,0);

}


void SelezionaListBoxValore(HWND WindList, int Valore)
{

	int TotItem;
	int i, Indice;

	TotItem = SendMessage(WindList, LB_GETCOUNT,0,0);
	Indice=-1;
	for (i=0;i<TotItem;i++) {
		if (SendMessage(WindList,LB_GETITEMDATA,i,0) == Valore) {
			Indice=i;
			break;
		}
	}
	SendMessage(WindList,LB_SETCURSEL,Indice,0);

}
// se c'e' una serie animata selezinata agiornare valore di frame rate 
// nel combo speed e tipoanim

void AggiornaComboAnimazioni(HWND hDialogo)
{

	HWND WindCombo;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int  Tempo;
	int TipoAnim;
	int Rotate;

	WORD TempWord;
	char Numero;


	// ora aggironare tipoanim
	TipoAnim = GlobNgle.RoomTexAnimate.
				VetInfoRangeAnim[DatiTexture.IndiceSelSerie] & FAN_MASK_ANIM;
	WindCombo=GetDlgItem(hDialogo, IDC_COMBO_TIPOANIM);
	DatiTexture.TipoAnim = TipoAnim;
	SelezionaComboValore(WindCombo,TipoAnim);
	// stabilisce contenuto di combo speed dato che cambia a seconda
	// se modo scroll
	ImpostaComboSpeed(hDialogo);

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_SPEED);

	Tempo=0;
	if (DatiTexture.IndiceSelSerie != -1) {

		Tempo = GlobNgle.RoomTexAnimate.
				VetInfoRangeAnim[DatiTexture.IndiceSelSerie];
		if (TipoAnim & FAN_ANY_ROTATE) {
			Tempo &= FAN_MASK_FPS_UV;
		
		}else {
			Tempo &= FAN_MASK_DELAY;
		}

	}

	SelezionaComboValore(WindCombo,Tempo);

	DatiTexture.Speed = Tempo;

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_UVROTATE);

	if ((TipoAnim & FAN_ANY_ROTATE) == 0) {
		ShowWindow(WindCombo,SW_HIDE);
		return;
	}

	ShowWindow(WindCombo, SW_SHOW);
	// qui bisogna trasformare numero in numero con segno
	// del byte basso
	TempWord = GlobNgle.RoomTexAnimate.
				VetInfoRangeAnim[DatiTexture.IndiceSelSerie] & FAN_MASK_UVROTATE;

	Numero = (char) TempWord;
	Rotate=Numero;
	DatiTexture.UVRotate = Numero;


	SelezionaComboValore(WindCombo,Rotate);
	
		
}

char *CalcolaOrdineRange(void)
{
	static char BufferTemp[256];
	int Indice,i;

	BufferTemp[0]=0;

	if (DatiTexture.TipoTexture == TT_ANIMATE) {
		Indice = DatiTexture.IndiceSelSerie;
		// trovare il numero d'ordine del range attuale
		if (Indice == -1) {
			strcpy(BufferTemp,"<NO RANGE SELECTED>  ");
		}else {
			for (i=0;i<DatiTexture.TotLastSort;i++) {
				if (Indice == DatiTexture.VetLastSorg[i]) break;
			}

			if ( i == DatiTexture.TotLastSort) {
				// non trovato
				strcpy(BufferTemp,"<RANGE NOT YET SET>  ");
			}else {
				// visuaslizzare numero progressivo
				sprintf(BufferTemp,"<%dth Range>  ",i+1);

			}
		}


	}
	return BufferTemp;

}

// gestisce eventi del mouse per finestra texture
// i valori bool  TestClick sono true solo quando viene effettuato il primo click
// in FlagMouse invece si puo' controllare se i pulsanti sono ancora premuti o meno
void GestioneMouseTexWind(HWND hDialogo, int MouseX, int MouseY, bool TestClickSinistro, 
					bool TestClickDestro, DWORD FlagMouse)
{
	int	x,y,IndiceSel;
	int SizeQuadro;
	HWND WindCombo;
	BYTE ** p2VetTexSonorePrj = (BYTE **) 0x45463E;
	BYTE *pVetTexSonorePrj;
	int	 IndiceSound;
	BYTE** p2VTBumpMap = (BYTE **) 0x454642;
	BYTE *pVetBumpMap;
	HWND TempWind;
	int **p2VetAnimatedTiles = (int**) 0x454646;
	int *VetAnimatedTiles;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int	*VetAnimCodes = (int*) 0x56BF80;
	int *pTotAnimCodes = (int*) 0x56C420;
	VetAnimatedTiles = *p2VetAnimatedTiles;
	int Indice,i;
	int IndiceInizio,IndiceFine;
	int NumTex;
	WORD TempWord;

	bool	TestErrore;


	pVetBumpMap = *p2VTBumpMap;
	pVetTexSonorePrj = *p2VetTexSonorePrj;

	MouseX = MouseX - DatiTexture.BaseX;
	MouseY = MouseY - DatiTexture.BaseY;


	if (MouseX < 0 || MouseY < 0) return;

	SizeQuadro = DatiTexture.SizeDst + DatiTexture.Gap;

	x = MouseX / SizeQuadro;
	y = MouseY / SizeQuadro;



	if (x >= DatiTexture.GrigliaDstX || 
		y >= DatiTexture.GrigliaDstY ) return;

	// calcolare indice assoluto
	IndiceSel = x + y * DatiTexture.GrigliaDstX + 
						DatiTexture.IndicePrimaTexture;

	sprintf(BufferLog,"%sTexture Index = %d",CalcolaOrdineRange(), IndiceSel);
	SetWindowText(GetDlgItem(hDialogo,IDC_INFO_SOUND),BufferLog);
	
	if (IndiceSel >= 256) {

		return;
	}
	// analisi in comune per entrambe le modalita'
	if (TestClickDestro== true) {
		// inizio selezione texture
		DatiTexture.SelInizio = IndiceSel;
		DatiTexture.SelFine = IndiceSel;
		DatiTexture.IndiceSelSerie =-1;
		DisegnaTexture();
		return;
	}
	// se c'e' una selezione in corso e ora il pulsante destro
	// non e' attivo inserire serie animata 
	if (DatiTexture.SelInizio != -1 && !(FlagMouse & MK_RBUTTON) && 
		TestClickSinistro==false ) {
		// vedere se si puo' aggiungere serie

		// se una delle texture era gia' allocata per una serie
		// dare errore


		IndiceInizio = DatiTexture.SelInizio;
		IndiceFine= DatiTexture.SelFine;

		if (IndiceFine < IndiceInizio) {
			// invertire
			if (IndiceFine == -1) {
				IndiceFine=IndiceInizio;
			}else {
				x = IndiceInizio;
				IndiceInizio=IndiceFine;
				IndiceFine=x;
			}
		}
		TestErrore=false;
		if (DatiTexture.TipoTexture==TT_SONORE) {
			// lasciare selezione multipla 
			DatiTexture.SelInizio = IndiceInizio;
			DatiTexture.SelFine = IndiceFine;
			DisegnaTexture();
			return;
		}
		// siamo in texture animate
		for (i=IndiceInizio;i<=IndiceFine;i++) {
			if (VetAnimatedTiles[i] != -1) {
				MessageBox(hDialogo, "ERROR: Cannot have overlapping ranges", 
							"Animation Ranges",MB_APPLMODAL	);
				TestErrore=true;
				break;
			}
		}
		// vedere che range di animazione di tipo frame non sia magiore di 16 tex
		NumTex = IndiceFine-IndiceInizio+1;
		// scoprire il tipo di animazione attuale
		TempWord=0;
		// prima scoprire tipo di animazione
		TempWind = GetDlgItem(hDialogo, IDC_COMBO_TIPOANIM);
		i = SendMessage(TempWind,CB_GETCURSEL,0,0);
		if (i == CB_ERR) {
			MessageBox(hDialogo,"No [Anim-Type] selected to assign",
					"Error",MB_APPLMODAL);
			TestErrore=true;
		} else {		
			TempWord = (WORD) SendMessage(TempWind, CB_GETITEMDATA,i,0);
		}

		if (TestErrore==false) {
			if (NumTex > 16 && (TempWord == FAN_FRAMES || TempWord == FAN_P_FRAMES)) {
				sprintf(BufferLog, "ERROR: an animated (frame) range cann't having more than 16 textures. You currently selected %d textures",
					NumTex);
				MessageBox(hDialogo, BufferLog, 
							"Animation Ranges",MB_APPLMODAL	);
				TestErrore=true;
		
			}
		}
		if (TestErrore==false) {
			Indice = VetAnimCodes[*pTotAnimCodes];
			(*pTotAnimCodes)++;
			VetAnimRange[Indice].TestUsato =1;
			VetAnimRange[Indice].IndiceStart = IndiceInizio;
			VetAnimRange[Indice].IndiceEnd = IndiceFine;

			DatiTexture.IndiceSelSerie = Indice;
			DatiTexture.TipoAnim = TempWord;
			
			// aggiornare anche puntatori in vettiles
			for (i=IndiceInizio ;i<=IndiceFine;i++) {
				VetAnimatedTiles[i] = Indice;
			}
			GlobNgle.RoomTexAnimate.VetInfoRangeAnim[Indice] = TempWord;


			// aggiornare in combo frame attuale
			AggiornaComboAnimazioni(hDialogo);

		}
		
		if (DatiTexture.TipoTexture == TT_ANIMATE) {
			DatiTexture.SelInizio=-1;
			DatiTexture.SelFine =-1;
			DisegnaTexture();
			return;
		}
	
	}

	if (FlagMouse & MK_RBUTTON) {
		// e' premuto il pulsante destro del mouse
		DatiTexture.SelFine = IndiceSel;
		DisegnaTexture();
		return;
	}


	switch (DatiTexture.TipoTexture) {
	case TT_SONORE:
		if (TestClickSinistro==true) {
			DatiTexture.SelInizio = IndiceSel;
			DatiTexture.SelFine = IndiceSel;
			DisegnaTexture();
			// ora localizzare l'indice sonoro di questa texture e attivare 
			// il corrispondente valore nel combo box
			IndiceSound = pVetTexSonorePrj[IndiceSel];

			WindCombo = GetDlgItem(hDialogo,IDC_COMBO_SOUNDS);
			PostMessage(WindCombo,CB_SETCURSEL, IndiceSound,0);
			TempWind = GetDlgItem(hDialogo,IDC_INFO_SOUND);
			SetWindowText(TempWind, VetDescWav[IndiceSound]);
			// stessa cosa per tipo di bump map
			IndiceSound = pVetBumpMap[IndiceSel];
			WindCombo = GetDlgItem(hDialogo,IDC_COMBO_BUMP);
			SendMessage(WindCombo,CB_SETCURSEL,IndiceSound,0);
		}
		break;

	case TT_ANIMATE:
		if (TestClickSinistro==true) {
			// controllare se la texture selezionata fa parte di uno dei range
			DatiTexture.IndiceSelSerie = VetAnimatedTiles[IndiceSel];
			DatiTexture.SelInizio=-1;
			DatiTexture.SelFine =-1;
			// aggiornare in combo frame attuae
			AggiornaComboAnimazioni(hDialogo);

			DisegnaTexture();

			sprintf(BufferLog,"%sTexture Index = %d",CalcolaOrdineRange(), IndiceSel);
			SetWindowText(GetDlgItem(hDialogo,IDC_INFO_SOUND),BufferLog);
			break;
		}


			
		
		break;

	}
	

}

void ImpostaComboTipoAnim(HWND hDialogo)
{

	HWND WindCombo;
	int i;
	int Indice;
	static char *VetNomiTipoAnim[] = {"Frames", "P-Frames", 
				"Full-Rotate", "Half-Rotate", "River-Rotate"};
	static WORD VetValTipoAnim[] = {FAN_FRAMES, FAN_P_FRAMES,
		FAN_FULL_ROTATE, FAN_HALF_ROTATE, FAN_RIVER_ROTATE};
	const int TotNomiTipoAnim = 5;

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_TIPOANIM);

	SendMessage(WindCombo,CB_RESETCONTENT,0,0);
	for (i=0;i< TotNomiTipoAnim;i++) {
		sprintf(BufferLog,"%s",VetNomiTipoAnim[i]);
		Indice=SendMessage(WindCombo,CB_ADDSTRING,0,(LPARAM) BufferLog);
		SendMessage(WindCombo,CB_SETITEMDATA,Indice,VetValTipoAnim[i]);
	}


}
// chiama un suono a caso solo per far allocare librerie dei suoni prima
// che le palle di directx imballino il computer
void ChiamaPrimoSuono(void)
{
	PlaySound("c:\\windows\\media\\notify.wav",NULL ,SND_SYNC);
}
// visualizza animazione di serie che hanno texture con indici da inizio a fine
// esce quando l'utente preme il tasto spazio
// nota: se il range e' di tipo uv allora sia inizio che fine avranno lo stesso
//	vaore che corrispondera' a texture di cui effettuare il preview
void ShowTextureAnimata(HWND hDialogo, int Inizio, int Fine)
{

	int OrgX,OrgY,SizeX,SizeY;
	DWORD StartTime;
	int i;
	POINT VetSource[256];
	int TotSource;
	int GrigliaX,GrigliaY;
	int IndiceNow;
	DWORD TempoImmagine;
	char Mex[]="HIT <ESCAPE> TO STOP";
	RECT MioRect;
	MSG		MyMsg;
	WORD TipoAnim;
	bool TestUv;
	int GapY;
	bool TestFullScroll;
	HDC SrcHdc;
	HDC MioHdc;
	HBITMAP  TempBitMap;
	HBITMAP OldBmp;
	int SrcX,SrcY,SizeSrcX,SizeSrcY;
	int UVRotate;


	// cancellare schermo
	DatiTexture.Ready = false;
	RettangoloPieno(DatiTexture.MioHdc,0,0,
					DatiTexture.ScreenX , DatiTexture.ScreenY, BrushNero);
	
	// ora trovare coordinata (centrale) dove mostrare il preview
	SizeX = DatiTexture.SizeSrc;
	SizeY = SizeX;

	OrgX = (DatiTexture.ScreenX - SizeX) / 2;
	OrgY = (DatiTexture.ScreenY - SizeY) / 2;

	// costruire le coordinate di partenza nell'hdc sorgente
	TotSource=0;
	for (i=Inizio;i<=Fine;i++) {

		GrigliaX = i % DatiTexture.GrigliaSrcX;
		GrigliaY = i / DatiTexture.GrigliaSrcX;

		VetSource[TotSource].x = GrigliaX * SizeX;
		VetSource[TotSource].y = GrigliaY * SizeY;

		TotSource++;
	}
	// visualizzare scritta per escape
	i = OrgY + SizeY + 20;
	MioRect.left=DatiTexture.BaseX;
	MioRect.top = i;
	MioRect.right = DatiTexture.ScreenX;
	MioRect.bottom = i+40;
	// nota: qui non serve replce placeolders perche' siamo in NGLE
	DrawText(DatiTexture.MioHdc,Mex,strlen(Mex), &MioRect,DT_CENTER);

	IndiceNow=0;
	// vedere tipo di animazione
	TipoAnim = DatiTexture.TipoAnim  &  FAN_MASK_ANIM;
	TestUv=false;
	TestFullScroll=false;
	switch (TipoAnim) {
	case FAN_FRAMES:
		TempoImmagine = (DWORD) DatiTexture.Speed;
		break;
	case FAN_P_FRAMES:
		TempoImmagine=1000;
		break;
	case FAN_FULL_ROTATE:
	case FAN_RIVER_ROTATE:
		TestUv=true;
		UVRotate = DatiTexture.UVRotate;

		TestFullScroll=true;

		// qui e' uno schifo ancora da migliorare
		// creare un hdc compatibile dove copiarci la texture attuale, raddoppiata

		TempBitMap=CreateCompatibleBitmap(DatiTexture.MioHdc ,SizeX,SizeY*2);
		MioHdc= CreateCompatibleDC(DatiTexture.MioHdc );

		OldBmp= (HBITMAP) SelectObject(MioHdc,TempBitMap);
		SrcHdc = MioHdc;
		
		// ora bisognerebbe impostare coordinate e dimensioni d'origine
		// relative a SrcHdc
		SrcX = 0;
		SrcY = 0;
		SizeSrcX = SizeX;
		SizeSrcY = SizeY;
		/*
		BitBlt(DatiTexture.MioHdc,OrgX,OrgY,SizeX,SizeY,
				DatiTexture.CompHdc ,VetSource[IndiceNow].x ,
				VetSource[IndiceNow].y,SRCCOPY);
		*/
		// copiare l'immagine di texture nel'hdc compatibile
		BitBlt(SrcHdc,SrcX,SrcY,SizeSrcX,SizeSrcY,DatiTexture.CompHdc,
				VetSource[0].x, VetSource[0].y,SRCCOPY);

		// ora stessa cosa ma copiando texture sotto quella gia' copiata
		BitBlt(SrcHdc,SrcX,SrcY+SizeSrcY,SizeSrcX,SizeSrcY,DatiTexture.CompHdc,
				VetSource[0].x, VetSource[0].y,SRCCOPY);

		break;
	case FAN_HALF_ROTATE:
		TestUv=true;
		UVRotate = DatiTexture.UVRotate;	
		TestFullScroll=false;
		TempoImmagine= 10;
		SrcHdc = DatiTexture.CompHdc;
		SrcX = VetSource[0].x;
		SrcY = VetSource[0].y;
		SizeSrcX = SizeX;
		SizeSrcY = SizeY / 2;


		break;
	}

	if (UVRotate == 0) {
		// impostare valore di  uvrotate preso da pref che dovrebbe 
		// corrisponedere a quello di script
		UVRotate = MyPrefRoom.DefaultUvRotate;
	
		
	}
	if (TestUv) {
		GapY=0;
		// qui bisognera' calcolare tempo per scroll
		TempoImmagine = DatiTexture.Speed >> 8;
		if (TipoAnim == FAN_RIVER_ROTATE) TempoImmagine=0;

		if (TempoImmagine==0) TempoImmagine=FRAME_SECONDO;
		TempoImmagine = 1000 / TempoImmagine;
	}
		

	while (TastoPremuto(VK_ESCAPE)==false) {
		StartTime = GetTickCount();
		// effettuare scrolling diverso a seconda di tipo animazione
		if (TestUv==false) {

			BitBlt(DatiTexture.MioHdc,OrgX,OrgY,SizeX,SizeY,
					DatiTexture.CompHdc ,VetSource[IndiceNow].x ,
					VetSource[IndiceNow].y,SRCCOPY);
		}else {
			// e' di tipo scrolling uv
			GapY -= UVRotate;
			GapY &= (SizeSrcY-1);

			if (TestFullScroll) {
				// full scroll
				BitBlt(DatiTexture.MioHdc,OrgX,OrgY,SizeX,SizeY,
					SrcHdc,SrcX,SrcY+GapY,SRCCOPY);
			}else {
				// half scroll, bisogna ridimensionare immagine
				StretchBlt(DatiTexture.MioHdc,OrgX,OrgY,SizeX,SizeY,
					SrcHdc,SrcX,SrcY+GapY,SizeSrcX,SizeSrcY,SRCCOPY);


			}
		}
		// attendere il tempo
		while ((GetTickCount()-StartTime) < TempoImmagine  && 
				TastoPremuto(VK_ESCAPE)==false);
		if (TestUv==false) {

			IndiceNow++;
			if (IndiceNow >= TotSource) IndiceNow=0;
		}
	}

	// eliminare tutti i messaggi wm_command
	while (PeekMessage(&MyMsg,hDialogo,WM_COMMAND,WM_COMMAND,PM_REMOVE));
	if (TestUv && TestFullScroll) {
		// elimiknare hdc
		SelectObject(SrcHdc,OldBmp);
		DeleteObject(TempBitMap);
		DeleteDC(SrcHdc);

	}
	// alla fine riattivare aggiornamento
	DatiTexture.Ready=true;

}
// resetta tutti i range
void ResetAnimazioni(HWND hDialogo)
{
	__asm {
		pushad
		mov		eax, 42B670h ; InizializzaTextureAnimate
		call	eax
		popad
	}
	DatiTexture.IndiceSelSerie =-1;
	ImpostaComboTipoAnim(hDialogo);
	DisegnaTexture();


}
// resetta tutti i suoni e tutti i bump mapping
void ResetSuoni(void)
{
	BYTE **p2VetTexSonorePrj = (BYTE** ) 0x45463E;
	BYTE *pVetTexSonorePrj;
	int i;
	BYTE** p2VTBumpMap = (BYTE **) 0x454642;
	BYTE *pVetBumpMap;

	pVetTexSonorePrj = *p2VetTexSonorePrj;
	pVetBumpMap = *p2VTBumpMap;
	for (i=0;i<MAX_TAIL_INFOS;i++) {
		pVetTexSonorePrj[i]=6;
		pVetBumpMap[i]=0;
	}
	DisegnaTexture();

	
}
// riempie valori di combo speed a seconda di corrente tipo di animazione
void ImpostaComboSpeed(HWND hDialogo)
{
	HWND TempWind;
	int Indice,i;
	WORD TipoAnim;
	WORD TempWord;

	TempWind = GetDlgItem(hDialogo,IDC_COMBO_TIPOANIM);
	i= SendMessage(TempWind,CB_GETCURSEL,0,0);
	if (i == CB_ERR) i=0;
	
	TipoAnim = (WORD) SendMessage(TempWind, CB_GETITEMDATA,i,0);

	TempWind = GetDlgItem(hDialogo,IDC_COMBO_SPEED);
	if (TipoAnim == FAN_P_FRAMES) 
		ShowWindow(TempWind,SW_HIDE);
	else
		ShowWindow(TempWind,SW_SHOW);


	if (((TipoAnim ^ DatiTexture.TipoAnimOld) & FAN_ANY_ROTATE) == 0) return;

	SendMessage(TempWind,CB_RESETCONTENT,0,0);

	if (TipoAnim & FAN_ANY_ROTATE) {
		// impostare speed per setting rotate
		Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) "MAX FPS");
		SendMessage(TempWind,CB_SETITEMDATA,Indice, 0);

		for (i=31;i>0;i--) {
			TempWord = i << 8;
			sprintf(BufferLog,"%d fps", i);
			Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) BufferLog);
			SendMessage(TempWind,CB_SETITEMDATA,Indice, TempWord);
		}


	}else {

		Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) "DEFAULT");
		SendMessage(TempWind,CB_SETITEMDATA,Indice, 0);

		for (i =30;i>1;i--) {
			sprintf(BufferLog,"%d fps ", i);
			Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) BufferLog);
			SendMessage(TempWind,CB_SETITEMDATA,Indice,1000 / i );
		}

		for (i = 1;i<9;i++) {
			sprintf(BufferLog,"%d SPF", i);
			Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) BufferLog);
			SendMessage(TempWind,CB_SETITEMDATA,Indice,1000 * i );
		}
	}
	DatiTexture.TipoAnimOld = TipoAnim;

}
// window message per finestra di tipo list box
// in lParam di wm_initdialog c'e' il tipo di list box TipoListBox
int CALLBACK WindProcedureTexture(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int TipoTexture;

	HWND  TempWind;
	static HWND WindScroll;
	int		i,TotTex;
	char *pChar;
	int  Numero;
	int		j;
	int		Indice;
	bool	TestOk;
	POINTS	Mouse;
	WORD TempWord;
    PAINTSTRUCT ps ;
	DWORD	TempoPerSuono,StartTime;
	BYTE ** p2VetTexSonorePrj = (BYTE **) 0x45463E;
	BYTE *pVetTexSonorePrj;
	static char * VetNomiSuoni[] = {"Mud", "Snow", "Sand", "Gravel", "Ice",
			"Water", "Stone", "Wood", "Metal", "Marble", "Grass",
			"Concrete", "Old Wood","Old Metal"};
	const int TotNomiSuoni = 14;



	static char * VetNomiBump[] = {"None", "Bump 1", "Bump 2"};

	const int TotNomiBump = 3;

	static char * VetFileWav[] = {
			"fs_mud1|fs_mud2", "fs_sn1|fs_sn2|fs_sn3", "fs_snd1|fs_snd2",
			"fs_grv1|fs_grv2", "fs_ice1|fs_ice2", "fsw1|fsw2|fsw3|fsw4",
			"foot01|foot02|foot03|foot04", "fs_wd1|fs_wd2",
			"fs_met1|fs_met2", "fs_mb1|fs_mb2|fs_mb3|fs_mb4",
			"fs_snd1|fs_snd2", "", "fs_wd1|fs_wd2", "fs_met1|fs_met2"};

	static int VetIdSuoni[] = {IDC_COMBO_SOUNDS,IDC_PUL_ASSIGN,IDC_COMBO_BUMP,
		IDC_PUL_TEST, IDC_PUL_ASSIGNBUMP};
	const int TotIdSuoni = 5;

	static int VetIdAnimazioni[] = {IDC_DEL_RANGE, 
				IDC_TEST_ANIMAZIONE,IDC_COMBO_SPEED,IDC_ASSIGN_FRAME,
				IDC_LABEL_TIPOANIM, IDC_COMBO_TIPOANIM, IDC_COMBO_UVROTATE};
	const int TotIdAnimazioni = 7;

	const int TotFileWav = 14;
	char  NomeFileWav[256];
	HWND *pMiaWind = (HWND*) 0x46E559;
	int		TotParti;
	char ** VetParti;
	int **p2VetAnimatedTiles = (int**) 0x454646;
	int *VetAnimatedTiles;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int	*VetAnimCodes = (int*) 0x56BF80;
	int *pTotAnimCodes = (int*) 0x56C420;


	VetAnimatedTiles = *p2VetAnimatedTiles;
	BYTE** p2VTBumpMap = (BYTE **) 0x454642;
	BYTE *pVetBumpMap;

	pVetBumpMap = *p2VTBumpMap;
	pVetTexSonorePrj = *p2VetTexSonorePrj;

	switch (uMsg) {
		case  WM_INITDIALOG:

			EnableWindow(*pMiaWind,FALSE);
			// salvare tipo wind list
			TipoTexture = lParam;

			switch (TipoTexture) {
			case TT_SONORE:
				SetWindowText(hDialogo,"  Sound Texture Properties");
				break;
			case TT_ANIMATE: 
				SetWindowText(hDialogo,"  Texture Animation Ranges");
				RiordinaRangeAnimati();
				break;
			}

			WindScroll = GetDlgItem(hDialogo,IDC_SCROLLBAR1);

			DatiTexture.MioHdc  = GetDC(hDialogo);
			SetStretchBltMode(DatiTexture.MioHdc,COLORONCOLOR);
			DatiTexture.CompHdc = CreaDCTexture(DatiTexture.MioHdc);
			DatiTexture.Gap =4;
			DatiTexture.ScreenX = 10 * 68 + 17;
			DatiTexture.ScreenY = 6 * 68 + 12;

			if (VersioneProject == 50) 
				DatiTexture.SizeSrc = 128;
			else {
				DatiTexture.SizeSrc = 64;
				VersioneProject=49;
			}
			DatiTexture.GrigliaSrcX = TGA_SizeX / DatiTexture.SizeSrc;
			DatiTexture.GrigliaSrcY = TGA_SizeY / DatiTexture.SizeSrc;

			DatiTexture.TotTexture = DatiTexture.GrigliaSrcX * DatiTexture.GrigliaSrcY;
			DatiTexture.TipoTexture = TipoTexture;



			// se la modalita' non e' di 128 x 128 rende invisibile il pulsante
			if (DatiTexture.SizeSrc == 64) {
				TempWind= GetDlgItem(hDialogo,IDC_REDUCE_SIZE);
				ShowWindow(TempWind,SW_HIDE);
			}

			DatiTexture.SizeDst = DatiTexture.SizeSrc;


			switch (TipoTexture) {
			case TT_SONORE:

				// inizializza per texture sonore
				// rende invisibili controlli per animazioni
				for (i=0;i<TotIdAnimazioni;i++) {
					TempWind = GetDlgItem(hDialogo,VetIdAnimazioni[i]);
					ShowWindow(TempWind,SW_HIDE);
				}
				// riempie combo suoni
				TempWind = GetDlgItem(hDialogo,IDC_COMBO_SOUNDS);
				SendMessage(TempWind,CB_RESETCONTENT,0,0);
				for (i=0;i<TotNomiSuoni;i++) {
					SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) VetNomiSuoni[i]);
				}

				// riempie combo bumpmap
				TempWind = GetDlgItem(hDialogo,IDC_COMBO_BUMP);
				SendMessage(TempWind,CB_RESETCONTENT,0,0);
				for (i=0;i<TotNomiBump;i++) {
					SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) VetNomiBump[i]);
				}


				break;
			case TT_ANIMATE:
				// inizializzazione per texture animate
				// prima rendere invisibili tutti i controlli che erano usati
				// solo per texture sonore
				// ripulire indice start da eventuali residui 
				// di temporizzazione texture
				for (i=0;i<40;i++) {
					VetAnimRange[i].IndiceStart &= 0xff;
				}

				ImpostaComboTipoAnim(hDialogo);
				for (i=0;i<TotIdSuoni;i++) {
					TotParti = VetIdSuoni[i];
					TempWind = GetDlgItem(hDialogo, TotParti);


					if (TempWind != NULL) ShowWindow(TempWind,SW_HIDE);
				}
				DatiTexture.TipoAnim =0;
				DatiTexture.TipoAnimOld = 0x8000;
					
				ImpostaComboSpeed(hDialogo);


				DatiTexture.Speed = 0;


				// riempie combo box per uv rotate
				TempWind=GetDlgItem(hDialogo,IDC_COMBO_UVROTATE);
				for (i=-64;i<0;i++) {
					sprintf(BufferLog,"UVRotate: %d", i);
					Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) BufferLog);
					SendMessage(TempWind,CB_SETITEMDATA,Indice,i );
				}				

				Indice=SendMessage(TempWind,CB_ADDSTRING,0, 
							(LPARAM) "DEFAULT (from script)");
				SendMessage(TempWind,CB_SETITEMDATA,Indice,i );
				for (i=1;i<65;i++) {
					sprintf(BufferLog,"UVRotate: %d", i);
					Indice=SendMessage(TempWind,CB_ADDSTRING,0,(LPARAM) BufferLog);
					SendMessage(TempWind,CB_SETITEMDATA,Indice,i );
				}
				SelezionaComboValore(TempWind,0);
				break;
				
			}

			RicalcolaDatiTexture(WindScroll);
			return false;

		case WM_CLOSE: 
			// rilasciare gli hdc
			EliminaDCTexture(DatiTexture.CompHdc);
			ReleaseDC(hDialogo,DatiTexture.MioHdc);
			EndDialog(hDialogo,-1);
			return true;

		case WM_LBUTTONDOWN:
			Mouse = MAKEPOINTS(lParam);	
			GestioneMouseTexWind(hDialogo, Mouse.x, Mouse.y ,true,false,wParam);
			break;

		case WM_RBUTTONDOWN:
			Mouse = MAKEPOINTS(lParam);
			GestioneMouseTexWind(hDialogo, Mouse.x, Mouse.y, false,true,wParam);
			break;
		case WM_RBUTTONUP:
			Mouse = MAKEPOINTS(lParam);
			GestioneMouseTexWind(hDialogo, Mouse.x, Mouse.y, false,false,wParam);
			break;
			
		case WM_MOUSEMOVE:
			// calcolare indice di texture selezionata
			Mouse = MAKEPOINTS(lParam);
			GestioneMouseTexWind(hDialogo, Mouse.x, Mouse.y, false,false, wParam);


			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {


				case IDOK:
				
					// rilasciare gli hdc
					EliminaDCTexture(DatiTexture.CompHdc);
					ReleaseDC(hDialogo,DatiTexture.MioHdc);
					EndDialog(hDialogo,-1);

					return true;
				case IDC_REDUCE_SIZE:
					if (IsDlgButtonChecked(hDialogo,IDC_REDUCE_SIZE) == 1) 
						// ridurre dimensione a 64 x 64
						DatiTexture.SizeDst = 64;
					else
						DatiTexture.SizeDst = 128;

					RicalcolaDatiTexture(WindScroll);
					return true;
				case IDC_PUL_RESET:

					if (DatiTexture.TipoTexture == TT_ANIMATE) {
						if (MessageBox(hDialogo,"Do you want reset animation ranges?",
							"Reset Animations",MB_YESNO) == IDYES) {	
							ResetAnimazioni(hDialogo);
							RiordinaRangeAnimati();							
						}
					}else {
						if (MessageBox(hDialogo,"Do you want reset all sounds and bump mppaing for textures?",
							"Reset Sounds",MB_YESNO) == IDYES) {
							ResetSuoni();

						}
						

					}
					return true;

				case IDC_PUL_ASSIGN:
					// assegnare attuale indice sound all'attuale texture
					// selezionata

					if (DatiTexture.SelInizio == -1) {
						MessageBox(hDialogo,"No selected texture yet", 
								"Sound Textures",MB_APPLMODAL);
						break;
					}

					TempWind = GetDlgItem(hDialogo,IDC_COMBO_SOUNDS);
					i= SendMessage(TempWind,CB_GETCURSEL,0,0);

					if (i == CB_ERR) {
						MessageBox(hDialogo,"No selected sound to assign",
									"Sound Textures",MB_APPLMODAL);
						break;
					}
					for (j=DatiTexture.SelInizio;j<=DatiTexture.SelFine;j++) {
						pVetTexSonorePrj[j] = i;
					}

					return true;
				case IDC_PUL_ASSIGNBUMP:
					TempWind = GetDlgItem(hDialogo, IDC_COMBO_BUMP);

					i = SendMessage(TempWind,CB_GETCURSEL,0,0);
					if (i == CB_ERR) {
						MessageBox(hDialogo,"No selected bump type to assign",
								"Error",MB_APPLMODAL);
						break;
					}
					if (DatiTexture.SelInizio == -1) {
						MessageBox(hDialogo,"No selected texture where to assign",
							"Error",MB_APPLMODAL);
						break;
					}
					for (j=DatiTexture.SelInizio;j<=DatiTexture.SelFine;j++) {
						pVetBumpMap[j] = i;
					}
					break;

				case IDC_ASSIGN_FRAME:
					// imposta corrente velocita' frame nell'animazione attuale
					if (DatiTexture.IndiceSelSerie == -1) {
						MessageBox(hDialogo,"No selected range to assign frame rate",
							"Error",MB_APPLMODAL);
						break;
					}
					TempWord=0;
					// prima scoprire tipo di animazione
					TempWind = GetDlgItem(hDialogo, IDC_COMBO_TIPOANIM);
					i = SendMessage(TempWind,CB_GETCURSEL,0,0);
					if (i == CB_ERR) {
						MessageBox(hDialogo,"No [Anim-Type] selected to assign",
								"Error",MB_APPLMODAL);
						break;
					}						
					TempWord = (WORD) SendMessage(TempWind, CB_GETITEMDATA,i,0);
					// ora, a seconda se e' tipo scroll o meno
					// interpretare in modo diverso i valori
					// in combo speed

					TempWind = GetDlgItem(hDialogo,IDC_COMBO_SPEED);
					
					i= SendMessage(TempWind,CB_GETCURSEL,0,0);
					if (i == CB_ERR) {
						MessageBox(hDialogo,"No Frame Rate selected to assign",
								"Error",MB_APPLMODAL);
						break;
					}
					i = SendMessage(TempWind, CB_GETITEMDATA,i,0);

					// valore di frame rate
					TempWord |= i;

					// se tipo rotate prendere dati anche per uvrotate
					if (TempWord & FAN_ANY_ROTATE) {
						TempWind = GetDlgItem(hDialogo,IDC_COMBO_UVROTATE);
						i= SendMessage(TempWind,CB_GETCURSEL,0,0);
						if (i == CB_ERR) {
							MessageBox(hDialogo,"No [UV Rotate] selected to assign",
									"Error",MB_APPLMODAL);
							break;
						}
						i = SendMessage(TempWind, CB_GETITEMDATA,i,0);
						
						
						i &= FAN_MASK_UVROTATE;
						TempWord |= i;

					}
				
					GlobNgle.RoomTexAnimate.VetInfoRangeAnim[
							DatiTexture.IndiceSelSerie]=TempWord;
					RiordinaRangeAnimati();
	
					break;

				case IDC_PUL_TEST:
					// premuto pulsante test

					
					TempWind = GetDlgItem(hDialogo,IDC_COMBO_SOUNDS);
					i= SendMessage(TempWind,CB_GETCURSEL,0,0);

					if (i == CB_ERR) {
						MessageBox(hDialogo,"No selected sound to play",
							"Sound Textures",MB_APPLMODAL);
						break;
					}
					VetParti = Split(VetFileWav[i],'|', &TotParti,NULL,NULL,false);
					if (TotParti == 0) {
						MessageBox(hDialogo,"No wav file, only hardcoded",
								"Sound Textures",MB_APPLMODAL);
						break;
					}
					TempoPerSuono= 2000 / TotParti;
					
					for (i=0;i<TotParti;i++) {
						sprintf(NomeFileWav, "sound\\samples\\%s.wav", VetParti[i]);

						if (EsisteFile(NomeFileWav)==false) {
							sprintf(BufferLog,"ERROR: cann't locate wav file: %s", NomeFileWav);
							MessageBox(hDialogo,BufferLog,"Sound Textures",MB_APPLMODAL);
							break;
						}
						StartTime=GetTickCount();

						PlaySound(NomeFileWav,NULL ,SND_NODEFAULT | 
										SND_FILENAME |  SND_NOWAIT  );
						while ((GetTickCount()-StartTime) < TempoPerSuono);

					}

					return true;	
				case IDC_COMBO_SOUNDS:
					TempWind = GetDlgItem(hDialogo,IDC_COMBO_SOUNDS);
					i= SendMessage(TempWind,CB_GETCURSEL,0,0);

					if (i == CB_ERR) break;

					TempWind = GetDlgItem(hDialogo,IDC_INFO_SOUND);
					SetWindowText(TempWind, VetDescWav[i]);
					return true;


				case IDC_DEL_RANGE:
					// cancellare range attivo
					if (DatiTexture.IndiceSelSerie == -1) {
						MessageBox(hDialogo,"No animation range selected",
								"Animation Ranges",MB_APPLMODAL);
						break;
					}
					
					Indice = DatiTexture.IndiceSelSerie;
					if (VetAnimRange[Indice].IndiceStart >
						VetAnimRange[Indice].IndiceEnd) {
						MioMessageBox("Current range is corrupted. Start texture has index greather than last texture. It's necessary reset all animation data, sorry","Error");
						ResetAnimazioni(hDialogo);
						break;
					}
					// mettere a -1 in tails per singole texture
					for (i=VetAnimRange[Indice].IndiceStart;
							i<=VetAnimRange[Indice].IndiceEnd;i++) {
						
						VetAnimatedTiles[i] = -1;
					}
					
					VetAnimRange[Indice].TestUsato =0;

					// aggiornare vetttore codice array
					i= *pTotAnimCodes;
					i--;
					VetAnimCodes[i] = Indice;
					*pTotAnimCodes = i;

					DatiTexture.IndiceSelSerie=-1;

					DisegnaTexture();
					return true;
				case IDC_COMBO_SPEED:
					if (HIWORD(wParam) == CBN_SELENDOK) {
						// cambiare la velocita'
						TempWind = GetDlgItem(hDialogo,IDC_COMBO_SPEED);
						i = SendMessage(TempWind,CB_GETCURSEL,0,0);
						if (i != CB_ERR) {
							DatiTexture.Speed  = SendMessage(TempWind, 
														CB_GETITEMDATA,i,0);
						}
					}
					return true;
				case IDC_COMBO_UVROTATE:
					if (HIWORD(wParam) == CBN_SELENDOK) {
						// cambiare la velocita'
						TempWind = GetDlgItem(hDialogo,IDC_COMBO_UVROTATE);
						i = SendMessage(TempWind,CB_GETCURSEL,0,0);
						if (i != CB_ERR) {
							DatiTexture.UVRotate  = SendMessage(TempWind, 
														CB_GETITEMDATA,i,0);
						}
					}
					return true;
				case IDC_COMBO_TIPOANIM:
					if (HIWORD(wParam) == CBN_SELENDOK) {
						// cambiare il tipo animazione
						TempWind = GetDlgItem(hDialogo,IDC_COMBO_TIPOANIM);
						i = SendMessage(TempWind,CB_GETCURSEL,0,0);
						if (i != CB_ERR) {
							DatiTexture.TipoAnim = SendMessage(TempWind, 
														CB_GETITEMDATA,i,0);
							
						}
						ImpostaComboSpeed(hDialogo);
						MostraComboRotate(hDialogo);
						RiordinaRangeAnimati();

						
					}
					return true;
				case IDC_TEST_ANIMAZIONE:
					if (HIWORD(wParam) == BN_CLICKED) {

						if (DatiTexture.IndiceSelSerie==-1) {
							MessageBox(hDialogo,"No animation range selected to test",
									"Animation Ranges",MB_APPLMODAL);
							break;
						}
						i = DatiTexture.IndiceSelSerie;
						if (DatiTexture.TipoAnim & FAN_ANY_ROTATE) {
							// chiedere indice di texture da animare
							TotTex = VetAnimRange[i].IndiceEnd - VetAnimRange[i].IndiceStart +1;
							sprintf(BufferLog,"Type position index of texture to show in preview for current range\r\n\r\nPossible values: 1 - %d",
								TotTex);

							pChar = InputTextBox(BufferLog,"Choose texture to show",
										"1",TP_CENTRALE);
							Numero=-1;
							sscanf(pChar, "%d", &Numero);
							Numero--;
							if (Numero < 0 ||
								Numero >= TotTex) {

								MessageBox(hDialogo,"Not valid index for current range","Error",MB_APPLMODAL);
								break;
							}
							Numero = VetAnimRange[i].IndiceStart + Numero;

							ShowTextureAnimata(hDialogo,Numero,Numero);


						}else {

							ShowTextureAnimata(hDialogo, VetAnimRange[i].IndiceStart , 
											VetAnimRange[i].IndiceEnd);
						}
						DisegnaTexture();

						return false;
					}
					break;

			}
		case WM_VSCROLL:
			TestOk=false;

			switch LOWORD(wParam) {

			case SB_THUMBTRACK:


				i = HIWORD(wParam);

				DatiTexture.IndicePrimaTexture = i * DatiTexture.GrigliaDstX;
				DisegnaTexture();

				return 0;
			case SB_THUMBPOSITION:
				// posizione finale da omologare
				i = HIWORD(wParam);
				TestOk=true;
				break;


			case SB_BOTTOM:
				// mostrare ultima pagina
				i = DatiTexture.ScrollMax;
				TestOk=true;
				break;
			case SB_TOP:
				i =0;
				TestOk=true;
				break;

			case SB_LINEUP:
				i = DatiTexture.IndicePrimaTexture / DatiTexture.GrigliaDstX;
				i --;
				TestOk=true;
				break;
			case SB_LINEDOWN:
				i = DatiTexture.IndicePrimaTexture / DatiTexture.GrigliaDstX;
				i++;
				TestOk=true;
				break;
			case SB_PAGEDOWN:
				i = DatiTexture.IndicePrimaTexture / DatiTexture.GrigliaDstX;
				i += (DatiTexture.GrigliaDstY / 2) ;
				TestOk=true;
				break;
			case SB_PAGEUP:
				i = DatiTexture.IndicePrimaTexture / DatiTexture.GrigliaDstX;
				i -= (DatiTexture.GrigliaDstY / 2);
				TestOk=true;
				break;

			}


			if (TestOk==true) {


				if (i<0) i = 0;
				if (i>DatiTexture.ScrollMax) i = DatiTexture.ScrollMax;

				SetScrollPos(WindScroll,SB_CTL,i,TRUE);
				DatiTexture.IndicePrimaTexture = i * DatiTexture.GrigliaDstX;
				DisegnaTexture();
				return 0;
			}

			
			return 1;
		case WM_PAINT:
	      	BeginPaint (hDialogo, &ps) ;
			DisegnaTexture();

		    EndPaint (hDialogo, &ps) ;
	
	}
	return false;
}

// window message per finestra di tipo input text box
// in lParam di wm_initdialog c'e' il tipo di list box TipoListBox
int CALLBACK WindProcedureInput(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static char TestoFinale[256];
	int TipoPosizione;
	HWND  TempWind;
	POINT Origine;
	HWND *pMiaWind = (HWND*) 0x46E559;
	RECT RectRoom;
	RECT RectDialog;
	int OrgX,OrgY;
	int SizeX,SizeY;

	switch (uMsg) {
		case  WM_INITDIALOG:
			TipoPosizione = lParam;

			switch (TipoPosizione) {
			case TP_LUCI:
				// posizione 292, 525 
				Origine.x = 292;
				Origine.y = 525;
				ClientToScreen(*pMiaWind,&Origine);

				break;
			case TP_CENTRALE:
				// posizione centrale
				GetClientRect(*pMiaWind,&RectRoom);
				GetWindowRect(hDialogo,&RectDialog);
				SizeX = RectDialog.right - RectDialog.left;
				SizeY = RectDialog.bottom - RectDialog.top;

				OrgX = (RectRoom.right - SizeX) / 2;
				OrgY = (RectRoom.bottom - SizeY) / 2;

				Origine.x = OrgX;
				Origine.y = OrgY;
				ClientToScreen(*pMiaWind, &Origine);

				break;
			}
			// mettere testi iniziali nei controlli
			TempWind = GetDlgItem(hDialogo,IDC_INPUT_INFO);
			SetWindowText(TempWind,InputBufDescrizione);

			SetWindowText(hDialogo,InputBufTitolo);

			TempWind=GetDlgItem(hDialogo,IDC_INPUT_TEXT);
			SetWindowText(TempWind,InputBufDefault);
			if (strlen(InputBufDefault) > 0) {
				SendMessage(TempWind, EM_SETSEL,0,-1);
			}
			SetWindowPos(hDialogo,NULL,Origine.x, Origine.y,0,0,
						SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);


			SetFocus(TempWind);

			return FALSE;
		case WM_CLOSE: 
			EndDialog(hDialogo,0);

			return true; 
		case	WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDCANCEL:
					EndDialog(hDialogo,0);
					return true;

				case IDOK:
					// salva testo inserito in edit text
					TempWind = GetDlgItem(hDialogo,IDC_INPUT_TEXT);
					GetWindowText(TempWind,TestoFinale,255);
					EndDialog(hDialogo, (int) TestoFinale);

					return true;
			}

	}
	return FALSE;

}
// chiamata per suonare effetto selezionato in finestra list sound effects
// nota: se l'effetto e' multiplo, lo suono tutto subito e 
// poi restituisce false per segnalare che il preview e' gia' finito
// se invece il suono e' singolo lo esegue in modalita' loop e 
// restituisce true
bool SuonaEffetto(HWND hDialogo)
{

	HWND WindList;
	int Indice;
	int i;
	char *pNomeSound;
	char *pFileWav;
	char NomeWav[256];	
	char **VetParti;
	int TotParti;
	int j;

	WindList = GetDlgItem(hDialogo, IDC_LISTBOX);
	i = SendMessage(WindList,LB_GETCURSEL,0,0);
	if (i == LB_ERR)  {
		MessageBox(hDialogo,"No item selected","Error",MB_APPLMODAL);
		return false;
	}
	Indice = SendMessage(WindList,LB_GETITEMDATA,i,0);
	pNomeSound = pZonaVetList[Indice].pNome;

	pFileWav = TrovaSoundWav(pNomeSound);
	VetParti=Split(pFileWav,' ',&TotParti,NULL,NULL,false);
	if (TotParti==0) return false;
	if (TotParti==1) {
		sprintf(NomeWav,"sound\\samples\\%s.wav", VetParti[0]);

		if (EsisteFile(GetFileTrle(NomeWav))==false) return false;
		sndPlaySound(GetFileTrle(NomeWav),SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		return true;

	}
	for (j=0;j<TotParti;j++) {
		sprintf(NomeWav,"sound\\samples\\%s.wav", VetParti[j]);

		if (EsisteFile(GetFileTrle(NomeWav))== true) {
			sndPlaySound(GetFileTrle(NomeWav),SND_SYNC);
		}

	}	

	return false;
}

void SaveBinaryList(int TipoListBox)
{
	char NomeFile[256];
	FILE *pFile;
	int Numero;
	char *pChar;

	pChar = SalvaVettoriRemap.NomeFileTom;

	switch (TipoListBox) {
	case TLB_RemapObjects:

		sprintf(NomeFile,"%s\\%s.OCT",
			SoloDir(pChar), SoloNomeSenzaExt(pChar));
		pFile = fopen(NomeFile,"wb");
		if (pFile==NULL) {
			sprintf(BufferLog,"Cann't create file: %s", NomeFile);
			MioMessageBox(BufferLog,"Error Ngle");
			return;
		}
		Numero = 6000;
		fwrite(&Numero,4,1,pFile);
		
		fwrite(&SalvaVettoriRemap.VetObjRemap, 2,Numero,pFile);
		fclose(pFile);

		break;
	case TLB_RemapRooms:
		sprintf(NomeFile,"%s\\%s.RCT",
			SoloDir(pChar), SoloNomeSenzaExt(pChar));
		pFile = fopen(NomeFile,"wb");
		if (pFile==NULL) {
			sprintf(BufferLog,"Cann't create file: %s", NomeFile);
			MioMessageBox(BufferLog,"Error Ngle");
			return;
		}		
		Numero = SalvaVettoriRemap.TotRooms;
		fwrite(&Numero,4,1,pFile);
		fwrite(&SalvaVettoriRemap.VetRoomRemap,2,1,pFile);
		fclose(pFile);
		break;

	}
	sprintf(BufferLog,"Saved binary list in file: %s", NomeFile);
	MioMessageBox(BufferLog,"Ngle");
	
}
void CambiaNomeSlot(HWND hDialogo, HWND lp_WindListBox)
{
	char *pChar;
	int Indice;
	char *pNomeOriginale;
	int Slot;
	char Buffer[512];
	DWORD SizeNew;
	DWORD SizeOld;
	int i;
	int IndiceLista;

	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;


	pVetSlotOggetti = *p2VetSlotOggetti;

	Indice = SendMessage(lp_WindListBox,LB_GETCURSEL,0,0);

	if (Indice == LB_ERR) return;

	IndiceLista = SendMessage(lp_WindListBox,LB_GETITEMDATA,Indice,0);

	Slot = pZonaVetList[IndiceLista].Indice;

	pNomeOriginale = pVetSlotOggetti[Slot].pNome;


	sprintf(Buffer, "You can rename the %s (%d) item. The change will be applied only for current project", pNomeOriginale, Slot);

	pChar = InputTextBox(Buffer, "Rename Object",pNomeOriginale,TP_CENTRALE);
	if (pChar == NULL) return;

	SizeNew = strlen(pChar) +1;
	if (SizeNew > 32) {
		sprintf(BufferLog,"ERROR: the name you typed is too long (%d chars). The max length is 31 characters. The name will be truncated at 31th character",
			SizeNew);
		MioMessageBox(BufferLog,"Warning");
		SizeNew = 32;
		pChar[SizeNew-1] =0;

	}
	SizeOld = strlen(pNomeOriginale) +1;

	if (SizeNew > SizeOld) {
		// rilocare memoria
		pNomeOriginale = (char*) ReallocNGLE(pNomeOriginale, SizeNew);
		pVetSlotOggetti[Slot].pNome = pNomeOriginale;
	}
	strcpy(pNomeOriginale, pChar);
	// ora memorizzare cambio nel vettore
	for (i=0;i<VetMieiNomiSlot.TotNomiSlot;i++) {
		if (VetMieiNomiSlot.VetNomi[i].Slot == Slot) break;
	}
	if (i == VetMieiNomiSlot.TotNomiSlot) {
		// non c'era questo record: crearlo adesso
		if (i >= MAX_NOMI_SLOT) {
			MioMessageBox("ERROR: Reached max  number of renamed slot (300)", "ERROR");
			return;
		}
		
		VetMieiNomiSlot.TotNomiSlot++;
	}
	strcpy(&VetMieiNomiSlot.VetNomi[i].NomeSlot[0], pChar);
	VetMieiNomiSlot.VetNomi[i].Slot = Slot;

	// aggiornare anche list box
	SendMessage(lp_WindListBox, LB_GETTEXT, Indice, (int) &Buffer);
	// fare 25 spazi
	for (i=0;i<25;i++) {
		Buffer[i] = ' ';
	}
	// ora copiare il nuovo nome
	for (i=0;i< (int) strlen(pChar);i++) {
		Buffer[i] = pChar[i];
	}
	// ora togliere lavecchia stringa e mettere la nuova
	SendMessage(lp_WindListBox, LB_DELETESTRING, Indice, 0);
	// adesso inserire  quella nuova
	SendMessage(lp_WindListBox, LB_INSERTSTRING, Indice, (int) &Buffer);
	// adesso rimettrere l'item data
	SendMessage(lp_WindListBox, LB_SETITEMDATA,Indice, IndiceLista);
	SendMessage(lp_WindListBox, LB_SETCURSEL, Indice, 0);


}
// window message per finestra di tipo list box
// in lParam di wm_initdialog c'e' il tipo di list box TipoListBox
int CALLBACK WindProcedureList(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static HWND WindList;
	char *pTitolo;
	char BufferTemp[256];
	char BufferColonne[256];
	int		i;
	char *pColonne;
	HWND TempWind;
	
	switch (uMsg) {
		case  WM_INITDIALOG:
			// salvare tipo wind list
			TipoListBox = lParam;

			switch (TipoListBox) {
			case TLB_LoadLastProjects:
				pTitolo = "Load Last Projects";
				pColonne = "Name                         v.  Folder";
				break;
			case TLB_ListaStanze:
				pTitolo = "Select Room";
				pColonne = "Name                         Size   Height Effct Trigg Items  Buttons";
				break;
			case TLB_ListaOggetti:
				pTitolo = "Select Object";
				pColonne = "Name                      Slot      Type  Mesh  #RMI   #AMI  Vert  Coll  Anim";
				TempWind=GetDlgItem(hDialogo,IDC_PUL_RENAME);
				ShowWindow(TempWind,SW_SHOW);				
				break;
			case TLB_ListaSounds:
				pTitolo = "Select Sound Effect";
				pColonne = "Name                        ID  Level    OnDisk   File";
				TempWind=GetDlgItem(hDialogo,IDC_PUL_PLAY);
				ShowWindow(TempWind,SW_SHOW);
				break;
			case TLB_RemapObjects:
				sprintf(BufferColonne, "Room                 Item                  Sector  Map_Id  Tr4_Id  (%s)",
					SoloNome(SalvaVettoriRemap.NomeFileTom));
				pTitolo="Object IDs conversion list";
				pColonne = BufferColonne;
				TempWind=GetDlgItem(hDialogo,IDC_PUL_SAVE_BINARY);
				ShowWindow(TempWind,SW_SHOW);
				break;
			case TLB_RemapRooms:
				pTitolo = "Room indices conversion";
				pColonne = "Room                 Map-Index   Tr4-Index";
				TempWind=GetDlgItem(hDialogo,IDC_PUL_SAVE_BINARY);
				ShowWindow(TempWind,SW_SHOW);
				break;
			default:
				pTitolo="Unknown window";
				pColonne = "";
			}

			sprintf(BufferTemp,"%s  (<Enter> or <Double Click> to select ; <Esc> to abort ; <F1> output list)", 
					pTitolo);

			// Mettere titolo messaggio
			SetWindowText(hDialogo,BufferTemp);
			WindList = GetDlgItem(hDialogo,IDC_LISTBOX);
			
			// mettere tipo di colonne
			SetDlgItemText(hDialogo,IDC_LABEL_COLONNE, pColonne);			
			// impostare modo non alfabetico
			CheckRadioButton(hDialogo,IDC_SORT_NO,IDC_SORT_YES,IDC_SORT_NO);
			
			// riempire list box
			ShowLista(WindList,false);


			return FALSE;
		case WM_CLOSE: 
			EndDialog(hDialogo,-1);



			return TRUE; 
		// qui modificare in qualche modo
		case WM_KEYDOWN:
		case WM_VKEYTOITEM:

			switch(LOWORD(wParam)) {

			case VK_F1:
				// visualizzare l'intero contenuto del list box in un file
				ShowTextList(hDialogo,  WindList, false);
				return -2;
			case VK_SPACE:
				// invertire ordinamento
				if (IsDlgButtonChecked(hDialogo,IDC_SORT_YES)==1) {
					// era attiva ordinamento alfabetico
					// per cui ora attivare quello NO
					CheckRadioButton(hDialogo, IDC_SORT_NO, IDC_SORT_YES, 
									IDC_SORT_NO);
					for (i=0;i<TotList;i++) VetPuntaList[i]=i;
					ShowLista(WindList,false);
				}else{
					CheckRadioButton(hDialogo, IDC_SORT_NO, IDC_SORT_YES, 
									IDC_SORT_YES);
					// ordina
					OrdinaPuntaList();
					ShowLista(WindList,false);
				}
				return -2;		
			}
			return -1;
		case WM_LBUTTONDBLCLK:
			SceltoFile(hDialogo, WindList, TipoListBox);
			return TRUE;


		case	WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDCANCEL:
					switch (HIWORD(wParam)) {
					case BN_CLICKED:

						EndDialog(hDialogo,-1);

						break;
					}
					return true;
				case IDC_PUL_RENAME:
					if (TipoListBox != TLB_ListaOggetti) break;
					CambiaNomeSlot(hDialogo, WindList);
					return true;
				case IDC_PUL_PLAY:
					if (TipoListBox != TLB_ListaSounds) break;

					if (IsDlgButtonChecked(hDialogo, IDC_PUL_PLAY)==1) {
						// effettuare suono
						// ottenere nomi dei file wav
						if (SuonaEffetto(hDialogo) == false) {
							// disabilitare pulsante
							CheckDlgButton(hDialogo,IDC_PUL_PLAY,0);

						}
					}else {
						// sospendere suoni in corso
						sndPlaySound(NULL,0);

					}

					return true;
				case IDC_PUL_SAVE_BINARY:
					SaveBinaryList(TipoListBox);
					return true;

				case IDC_OK:
					switch (HIWORD(wParam)) {
					case BN_CLICKED:
						SceltoFile(hDialogo,WindList,TipoListBox);
						break;
					}



					return TRUE;

				case IDC_CANCEL:
					EndDialog(hDialogo,-1);

					return TRUE;

				case IDC_SORT_YES:
					// ordina
					OrdinaPuntaList();
					ShowLista(WindList,false);
					return true;
				case IDC_SORT_NO:
					for (i=0;i<TotList;i++) VetPuntaList[i]=i;
					ShowLista(WindList,false);
					return true;
				case IDC_LISTBOX:
					switch (HIWORD(wParam)) {
					case LBN_SELCHANGE:
						CambiataSelList(hDialogo, WindList,TipoListBox);
						return true;
					case LBN_DBLCLK:
						SceltoFile(hDialogo,WindList,TipoListBox);
						return -1;
					}
					break;

					
					
			}
		}
	return false;
}
// chiama funzioni del room editor per visualizzare la stanza e l'elemento selezionato
void MostraTrovato(int IndiceBaseItem)
{
	
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	int *pIndiceSlotSelezionato = (int*) 0x583BA8;
	int *pIndiceItemSelezionato = (int*) 0x583BA4;
	int IndiceSlot;
	int IndiceRoom;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	WORD *pTriggerSelezionato = (WORD *) 0x583BAC;
	pVetSlotOggetti = *p2VetSlotOggetti;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	bool  TestFloor;
	int  OrgX, OrgY;
	int *pOrientamentoTexture = (int*) 0x583BB6;



	pVetRooms = *p2VetRooms;
	pVetBaseOggetti = *P2_VetBaseOggetti;

	IndiceRoom = pVetBaseOggetti[IndiceBaseItem].IndexRoom;

	__asm {
		pushad
		mov eax, 426090h    ;MostraStanze
		call eax
		popad
	}

	MostraNuovaStanza(IndiceRoom);
#ifdef LOG_SEARCHITEM
	sprintf(BufferLog,"\t\tTrovato elemento in stanza=%d con ItemIndex=%d", 
			IndiceRoom, IndiceBaseItem);
	InviaLog(BufferLog);
#endif
	*pIndiceSlotSelezionato=-1;

	if (BaseFind.TipoRicerca == TLF_FLOOR_TYPE) 
		TestFloor=true; 
	else 
		TestFloor=false;

	if (TestFloor==false && pVetBaseOggetti[IndiceBaseItem].TypeObject == 8) {
		// e' oggetto wad
		IndiceSlot = pVetBaseOggetti[IndiceBaseItem].SlotIndex;
		*pIndiceSlotSelezionato = IndiceSlot;
#ifdef LOG_SEARCHITEM
		sprintf(BufferLog,"\t\tTypeObject=8 ; IndiceSlot=%d", IndiceSlot);
		InviaLog(BufferLog);
#endif

		// se e' anche un moveable inserire dati in datitrigger

		if (pVetSlotOggetti[IndiceSlot].Tipo  == 0x10) {
			// e' un moveable. Salvare dati in struttura trigger
			memset(pTriggerSelezionato,0,10);
			pTriggerSelezionato[1] = IndiceBaseItem;
#ifdef LOG_SEARCHITEM
			sprintf(BufferLog,"\t\tE' un moveable (non statico): inizializza struttura trigger con indice assoluto=%d",
				IndiceBaseItem);
			InviaLog(BufferLog);
#endif
		}
	}

	if (TestFloor ==false && pVetBaseOggetti[IndiceBaseItem].TypeObject == 0x10) {
		*pTestEsisteSelezioneBox=1;
	}

	if (pVetRooms[IndiceRoom].RoomButtons & 2) {

#ifdef LOG_SEARCHITEM
		InviaLog("\t\tElemento e' in una flipped room");
#endif

	}
	if (TestFloor==true) {
		// calcolare coordiante x y sulla base di indice assoluto IndiceBaseItem
		OrgY = IndiceBaseItem / pVetRooms[IndiceRoom].XSizeBlocks;
		OrgX = IndiceBaseItem % pVetRooms[IndiceRoom].XSizeBlocks;

		pRectSelezione2d->left = pVetRooms[IndiceRoom].XDisplayOffset + OrgX;
		
		pRectSelezione2d->top  = pVetRooms[IndiceRoom].ZDisaplyOffset + OrgY;

		pRectSelezione2d->right = 1;

		pRectSelezione2d->bottom  = 1;


	}else
	{
		// impostare dimensioni selezione
		pRectSelezione2d->left = pVetRooms[IndiceRoom].XDisplayOffset + 
									pVetBaseOggetti[IndiceBaseItem].XOrgBlock;	
		pRectSelezione2d->top  = pVetRooms[IndiceRoom].ZDisaplyOffset + 
									pVetBaseOggetti[IndiceBaseItem].ZOrgBlock;
		pRectSelezione2d->right = pVetBaseOggetti[IndiceBaseItem].XSizeBlock;

		pRectSelezione2d->bottom  = pVetBaseOggetti[IndiceBaseItem].ZSizeBlock;
		*pIndiceItemSelezionato = IndiceBaseItem;
	}
#ifdef LOG_SEARCHITEM
	sprintf(BufferLog,"\t\tRecSelezione2d(%d,%d,%d,%d)",
			pRectSelezione2d->left, pRectSelezione2d->top,
			pRectSelezione2d->right, pRectSelezione2d->bottom);
	InviaLog(BufferLog);
#endif



	*pOrientamentoTexture =0;

	// ora chiamare varie procedure per fare aggiornaemnto

	__asm {
		pushad

		mov eax, 412660h    ;ShowToolTipSelezione
		call eax
		mov eax, 40FD80h    ;ShowRiquadro2d
		call eax
		mov eax, 401200h    ;CicloPrincipale
		call eax
		push	1
		mov eax, 410120h    ;ShowSelezione2d
		call  eax
		add		esp,4

		mov eax, 401200h    ;CicloPrincipale
		call eax

		popad

	}
	



}
// cerca elemento
void GosubCerca(HWND hDialogo, HWND WindList, bool TestPrimo, bool TestAvanti)
{
	int Indice;
	int Codice;
	int	j;
	int	i;
	int z;
	int StartAlfa;
	bool TestInizioCiclo;
	int TipoOggetto;
	int IndiceBeta;
	int TotBlocchi;
	int IdNow;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;


	pVetTriggers = *P2_VetTriggers;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetRooms = *p2VetRooms;

	// prima trovare indice di elemento da trovare
	Indice = SendMessage(WindList,LB_GETCURSEL,0,0);
	if (Indice == LB_ERR)  {
		MessageBox(NULL,"No item selected","Error",MB_APPLMODAL);
		return;
	}

	// otenere indice reale 
	Codice= SendMessage(WindList,LB_GETITEMDATA,Indice,0);
	

	if (TestPrimo == true) {
		// inizializzare indici per ricerca dall'inizio
		BaseFind.Incremento = 1;
		BaseFind.IndiceAlfa = 0;
		BaseFind.IndiceBeta =0;
		BaseFind.IndiceRoom = 0;
		StartAlfa=0;

	}else{
		if (TestAvanti==true) {
			BaseFind.Incremento =1;
		}else{
			BaseFind.Incremento = -1;
			
		}
		// aumentare indicealfa di incremento

		StartAlfa = BaseFind.IndiceAlfa + BaseFind.Incremento ;

	
	}

	switch (BaseFind.TipoRicerca) {
	case TLF_OBJECTS:
		TipoOggetto=8;
		break;
	case TLF_TRIGGER_ACTIVATION:
	case TLF_TRIGGER_WHAT:
	case TLF_TRIGGER_OF_PLUGIN:
	case TLF_TRIGGER_UNHANDLED:
		TipoOggetto= 0x10;
		break;

	default:
		TipoOggetto=Codice;
	}

#ifdef LOG_SEARCHITEM
	sprintf(BufferLog, "GosubCerca(): IndiceRoom=%d IndiceAlfa=%d  Incremento=%d Codice=%d",
			BaseFind.IndiceRoom , BaseFind.IndiceAlfa , BaseFind.Incremento, Codice);
	InviaLog(BufferLog);
#endif
	TestInizioCiclo=true;

	if (BaseFind.TipoRicerca == TLF_INDICES) {
		MostraTrovato(Codice);
		BaseFind.IndiceRoom = 0;
		BaseFind.IndiceAlfa = 0;
		return;
	}
	

	for (i = BaseFind.IndiceRoom;i >=0 && i< *pTotRooms; i += BaseFind.Incremento) {
		// cercare in stanza con indice i
		if (pVetRooms[i].TestEmptySlot == 0) {
#ifdef LOG_SEARCHITEM
			sprintf(BufferLog,"\tScan Room=%d", i);
			InviaLog(BufferLog);
#endif

			switch (BaseFind.TipoRicerca ) {
			case TLF_OBJECTS:
			case TLF_TRIGGER_ACTIVATION:
			case TLF_TRIGGER_WHAT:
			case TLF_TRIGGER_OF_PLUGIN:
			case TLF_TRIGGER_UNHANDLED:

				if (TestInizioCiclo==false) {
				

					if (TestAvanti==true) {
						StartAlfa=0;
					}else{
						StartAlfa=pVetRooms[i].N_ObjAndTriggers -1;
					}
				}
				TestInizioCiclo=false;

				// cercare in objandtriger
				for (j=StartAlfa;j>=0 && 
						j<pVetRooms[i].N_ObjAndTriggers;j+= BaseFind.Incremento ) {
					// trovare indice a 
					Indice= pVetRooms[i].VetObjAndTriggerCodes[j];
#ifdef LOG_SEARCHITEM
					sprintf(BufferLog,"\t\tVetObjAndTrigger[%d]=%d  -> Slot=%d",j,Indice, 
							pVetBaseOggetti[Indice].SlotIndex);
					InviaLog(BufferLog);
				
#endif
					if (pVetBaseOggetti[Indice].TypeObject == TipoOggetto) {
						switch (BaseFind.TipoRicerca ) {
						case TLF_OBJECTS:
							// stiamo cercando oggetto slot
							// basta che .l'indice slot sia quello gisuto e tutto e' ok
							if (pVetBaseOggetti[Indice].SlotIndex == Codice ) {
								// trovao oggetto dello stesso tipo di slot.
								MostraTrovato(Indice);
								BaseFind.IndiceRoom = i;
								BaseFind.IndiceAlfa = j;
								return;
							}
							break;
						case TLF_TRIGGER_ACTIVATION:
							// ottenere indice a dati di trigger
							IndiceBeta = pVetBaseOggetti[Indice].SlotIndex;
							if (pVetTriggers[IndiceBeta].TriggerAttivazione == Codice) {
								// trovato trigger con lo specifico modo di attivazione
								MostraTrovato(Indice);
								BaseFind.IndiceRoom = i;
								BaseFind.IndiceAlfa = j;
								return;
							}
							break;
						case TLF_TRIGGER_WHAT:
							// ottenere indice a dati di trigger
							IndiceBeta = pVetBaseOggetti[Indice].SlotIndex;
							if (pVetTriggers[IndiceBeta].TriggerCosaAttivare == Codice) {
								// trovato trigger con lo specifico what
								MostraTrovato(Indice);
								BaseFind.IndiceRoom = i;
								BaseFind.IndiceAlfa = j;
								return;
							}
							break;
						case TLF_TRIGGER_OF_PLUGIN:
							// ottenere indice a dati di trigger
							IndiceBeta = pVetBaseOggetti[Indice].SlotIndex;
							if (pVetTriggers[IndiceBeta].PluginId == Codice) {

								// trovato trigger di plugin codice 
								MostraTrovato(Indice);
								BaseFind.IndiceRoom = i;
								BaseFind.IndiceAlfa = j;
								return;
							}
							break;	
						case TLF_TRIGGER_UNHANDLED:
							// ottenere indice a dati di trigger
							IndiceBeta = pVetBaseOggetti[Indice].SlotIndex;
							IdNow=pVetTriggers[IndiceBeta].PluginId;
							if (IdNow==0 ) break;

							if (Codice != -1 && IdNow != Codice) break;

							// e' il trigger di un plugin: vedere se c'e' quell'id
							for (z=0;z<GlobNgle.TotPluginNgle;z++) {
								if (GlobNgle.VetPluginNgle[z].PluginId == IdNow) break;
							}

							if (z== GlobNgle.TotPluginNgle) {
								// trovato!
								// trovato trigger di plugin codice 
								MostraTrovato(Indice);
								BaseFind.IndiceRoom = i;
								BaseFind.IndiceAlfa = j;
								return;
							}




						}
					}

				}
			
				break;

			case TLF_EFFECTS:
				if (TestInizioCiclo==false) {
				

					if (TestAvanti==true) {
						StartAlfa=0;
					}else{
						StartAlfa=pVetRooms[i].N_Effects -1;
					}
				}
				TestInizioCiclo=false;	
				for (j=StartAlfa;j>=0 && j < pVetRooms[i].N_Effects;
									j += BaseFind.Incremento) {
					Indice = pVetRooms[i].VetEffectCode [j];
					if (pVetBaseOggetti[Indice].TypeObject == TipoOggetto) {
						// trovato
						MostraTrovato(Indice);
						BaseFind.IndiceRoom = i;
						BaseFind.IndiceAlfa = j;
						return;
					}
				}
				break;
						
			
			case TLF_FLOOR_TYPE:


				// calcolare numero di elementi di tabella grid collision
				TotBlocchi = pVetRooms[i].XSizeBlocks * pVetRooms[i].ZSizeBlocks;
				if (TestInizioCiclo==false) {
				

					if (TestAvanti==true) {
						StartAlfa=0;
					}else{
						StartAlfa=TotBlocchi -1;
					}
				}
				TestInizioCiclo=false;
#ifdef LOG_SEARCHITEM
				sprintf(BufferLog,"\t\tTotGridBlocks = %d  [%d x %d]",TotBlocchi, 
					pVetRooms[i].XSizeBlocks,pVetRooms[i].ZSizeBlocks);
				InviaLog(BufferLog);
#endif

				for (j=StartAlfa;j>=0 && j<TotBlocchi;j += BaseFind.Incremento) {
#ifdef LOG_SEARCHITEM
					sprintf(BufferLog,"%d: Flag1=0x%X  Flag2=0x%X",
							j,
							pVetRooms[i].pBlockGridCollision[j].Flag1,
							pVetRooms[i].pBlockGridCollision[j].Flags2);
					InviaLog(BufferLog);
#endif
					if (pVetRooms[i].pBlockGridCollision[j].Flag1 & Codice) {
						// trovato flag
						MostraTrovato(j);
						BaseFind.IndiceRoom = i;
						BaseFind.IndiceAlfa = j;
						return;
					}


				}
				break;
			}
			

		}
	}
	MessageBox(hDialogo,"Not found", "Find",MB_APPLMODAL);

}
// restituisce il valore dell'item data dell'elemento selezionato
// nel combo box WindCombo
int GetComboItemData(HWND WindCombo)
{
	int Indice;

	Indice = SendMessage(WindCombo,CB_GETCURSEL,0,0);
	if (Indice == CB_ERR) return 0;
	return SendMessage(WindCombo,CB_GETITEMDATA, (WPARAM) Indice,0);

}
// prendi tutti i valori di preferenze e li inserisce in MyPrefRoom
// se c'e' qualche errore restituisce false
bool PrendiSettings(HWND hDialogo)
{
	HWND WindCombo;
	int Valore;
	// cancellare tutti i flags tranne quelli relativi a finestra di programmi
	MyPrefRoom.Flags &= FPR_NO_MINIMIZE | FPR_USE_SHORT_PATH;


	// ----- SPINNING ---------------

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_SLOWDOWN);
	MyPrefRoom.SpinningTimeInput = GetComboItemData(WindCombo);

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_FASTUP);
	MyPrefRoom.SpinningStep = GetComboItemData(WindCombo);

	// ---- RANDOM SMOOTH FLOOR ----------

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_EFFECT);
	Valore = GetComboItemData(WindCombo);

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_HITS);
	Valore |= GetComboItemData(WindCombo);

	if (IsDlgButtonChecked(hDialogo, IDC_CHECK_LIMITHEIGHT)== 1) {
		Valore |= RF_LIMIT_HEIGHT;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_SMOOTH) == 1) {
		Valore |= RF_DONT_SMOOTH;
	}

	MyPrefRoom.ParameterRandomFloor = Valore;

	// ------- MOUSE WHEEL -----------------

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_MINSTEP_MOUSE);
	MyPrefRoom.MouseMinStep  = GetComboItemData(WindCombo);

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_MAXSTEP_MOUSE);
	MyPrefRoom.MouseMaxStep  = GetComboItemData(WindCombo);

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_SENSITIVITY);
	MyPrefRoom.MouseSensitivity = GetComboItemData(WindCombo);

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_DISABLE_ACC) == 1) {
		MyPrefRoom.Flags |= FPR_NO_ACCELERATION;
	}

	// ------ GLOBAL SETTINGS ---------------


	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_SHOW_FLIPPED) == 1) {
		MyPrefRoom.Flags |= FPR_SAVE_MAP_BMP;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_REMOVE_AUTOSAVE) == 1) {
		MyPrefRoom.Flags |= FPR_REMOVE_AUTOSAVE;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_EXTEND_MEMORY) == 1) {
		MyPrefRoom.Flags |= FPR_NO_EXTENDED_MEMORY;
	}	


	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_DISABLE_CRS) == 1) {
		MyPrefRoom.Flags |= FPR_NO_RESUME_CRASH;
	}

	if (IsDlgButtonChecked(hDialogo, IDC_CHECK_NO_OBJ_ON_DRAWDOORS) == 1) {
		MyPrefRoom.Flags |= FPR_HIDE_OBJ_DRAW_DOORS;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_RELOAD_PROJECT) == 1) {
		MyPrefRoom.Flags |= FPR_LOAD_LAST_PROJECT;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_EXIT_PLAY_PROGRESSIVE) == 1) {
		MyPrefRoom.Flags |= FPR_SAVE_PLAY_PROGRESSIVE;
	}	

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_INFO_HIDEOBJ) == 1) {
		MyPrefRoom.Flags |= FPR_NO_INFO_HIDE_OBJECTS;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_STANDBY)==1) {
		MyPrefRoom.Flags |= FPR_NO_STANDBY;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_EXTRA_INFOS) ==1) {
		MyPrefRoom.Flags |= FPR_EXTRA_INFO_SELECTION;
	}

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_DISABLE_TRIGGERS) == 1) {
		MyPrefRoom.Flags |= FPR_DISABLE_NEW_TRIGGERS;
	}
	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_PREVIEW);
	MyPrefRoom.PreviewSize  = GetComboItemData(WindCombo);

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_MAXIMIZE)==1) {
		MyPrefRoom.Flags |= FPR_MAXIMIZE;
	}

	WindCombo = GetDlgItem(hDialogo,IDC_COMBO_BEND);
	MyPrefRoom.BendType = GetComboItemData(WindCombo);

	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_PUNTAPIRAMIDI) ==1) {
		MyPrefRoom.Flags |= FPR_PUNTA_PIRAMIDI;
	}
	WindCombo=GetDlgItem(hDialogo,IDC_COMBO_AUTO_BACKUP);
	MyPrefRoom.AutoBackup = GetComboItemData(WindCombo);

	WindCombo=GetDlgItem(hDialogo,IDC_COMBO_DEFAULT_UVROTATE);
	MyPrefRoom.DefaultUvRotate = GetComboItemData(WindCombo);


	return true;
}


BOOL CALLBACK WindProcedureSettings(HWND hDialogo, UINT uMsg, 
									WPARAM wParam, LPARAM lParam)
{

	HWND WindCombo;
	int i;
	int Valore;
	int Indice;
	static char *VetMexBend[] = {"Constant (Prefixed Bending)",
								"Progressive (Circle formula)",
								"Progressive (Parabola formula)"};
	const int TotMexBend = 3;
	static int VetAutoBackup[] = {3,10,15,20,25,30,35,40,50,70,100};
	const int TotAutoBackup = 11;

	switch (uMsg) {
		case  WM_INITDIALOG:

			// -------- COMBO  TIME SHIFT ------------------

			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_SLOWDOWN);

			// riempire combo box
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);


			for (i=0;i<10;i++) {
				sprintf(BufferLog,"%d ms",VetShiftDelay[i]);
				Indice=SendMessage(WindCombo,CB_ADDSTRING,0,(LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA,Indice,i);				

			}
			// seleziona valore
			SelezionaComboValore(WindCombo,MyPrefRoom.SpinningTimeInput );
			// ------- COMBO STEP SPINNING -------
			WindCombo = GetDlgItem(hDialogo,IDC_COMBO_FASTUP);

			// riempire combo box
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);


			for (i=0;i<TotIncrementoStep;i++) {
				sprintf(BufferLog,"%d / %d",VetIncrementoStep[i] ,
						VetIncrementoMax[i]);
				Indice=SendMessage(WindCombo,CB_ADDSTRING,0,(LPARAM) BufferLog);				
				SendMessage(WindCombo, CB_SETITEMDATA,Indice,i);

			}
			// seleziona valore
			SelezionaComboValore(WindCombo,MyPrefRoom.SpinningStep);
			
			// ----- PREFERENZE RANDOM SMOOTH -------------------
			// ---- COMBO EFFECT ---------
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_EFFECT);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			
			SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) "Hills");
			SendMessage(WindCombo,CB_SETITEMDATA, 0, 0);

			SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) "Mountains");
			SendMessage(WindCombo,CB_SETITEMDATA, 1, 1);

			Indice = MyPrefRoom.ParameterRandomFloor & 1;
			SelezionaComboValore(WindCombo,Indice);

			// ---- COMBO CHANGE PER HITS -------
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_HITS);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);

			SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) "Default");
			SendMessage(WindCombo,CB_SETITEMDATA, 0, 0);		
			
			SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) "Less Changes");
			SendMessage(WindCombo,CB_SETITEMDATA, 1, 16);			

			SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) "More Changes");
			SendMessage(WindCombo,CB_SETITEMDATA, 2, 4);
			// considera i due flags
			Indice = MyPrefRoom.ParameterRandomFloor & 20;
			SelezionaComboValore(WindCombo,Indice);

			if (MyPrefRoom.ParameterRandomFloor  & RF_DONT_SMOOTH) {
				CheckDlgButton(hDialogo,IDC_CHECK_NO_SMOOTH,1);
			}

			if (MyPrefRoom.ParameterRandomFloor  & RF_LIMIT_HEIGHT) {
				CheckDlgButton(hDialogo,IDC_CHECK_LIMITHEIGHT,1);
			}
			
			// ---- SETTINGS PER ROTELLA MOUSE ---------
			// ---- Min step 
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_MINSTEP_MOUSE);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);

			for (i=0;i<10;i++) {
				Valore = (i+1) * 16;
				sprintf(BufferLog,"%d pixels", Valore);
				Indice=SendMessage(WindCombo, CB_ADDSTRING,0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA,Indice, Valore);

			}
			SelezionaComboValore(WindCombo, MyPrefRoom.MouseMinStep);
			// ---- Max step
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_MAXSTEP_MOUSE);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);

			for (i=0;i<10;i++) {
				Valore= (i+1) * 64;
				sprintf(BufferLog,"%d pixels", Valore);
				Indice=SendMessage(WindCombo, CB_ADDSTRING,0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA,Indice, Valore);
			}
			SelezionaComboValore(WindCombo, MyPrefRoom.MouseMaxStep);

			// ---- sensitivity
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_SENSITIVITY);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			for (i=0;i<10;i++) {
				Valore= i+1;
				sprintf(BufferLog,"Level %d",Valore);
				Indice=SendMessage(WindCombo, CB_ADDSTRING,0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA,Indice, Valore);
			}
			SelezionaComboValore(WindCombo,MyPrefRoom.MouseSensitivity);
			// --- Disable accelleration ----
			if (MyPrefRoom.Flags & FPR_NO_ACCELERATION) {
				CheckDlgButton(hDialogo, IDC_CHECK_DISABLE_ACC, 1);
			}
			// -----  GLOBAL SETTINGS -------------------------
			if (MyPrefRoom.Flags & FPR_SAVE_MAP_BMP) {
				CheckDlgButton(hDialogo, IDC_CHECK_SHOW_FLIPPED,1);
			}
	
			if (MyPrefRoom.Flags & FPR_REMOVE_AUTOSAVE) {
				CheckDlgButton(hDialogo, IDC_CHECK_REMOVE_AUTOSAVE,1);
			}

			if (MyPrefRoom.Flags & FPR_NO_EXTENDED_MEMORY) {
				CheckDlgButton(hDialogo, IDC_CHECK_NO_EXTEND_MEMORY,1);
			}				
			
			if (MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) {
				CheckDlgButton(hDialogo, IDC_CHECK_DISABLE_CRS ,1);
			}				

			if (MyPrefRoom.Flags & FPR_HIDE_OBJ_DRAW_DOORS) {
				CheckDlgButton(hDialogo, IDC_CHECK_NO_OBJ_ON_DRAWDOORS ,1);
			}				

			if (MyPrefRoom.Flags & FPR_LOAD_LAST_PROJECT) {
				CheckDlgButton(hDialogo, IDC_CHECK_RELOAD_PROJECT ,1);
			}

			if (MyPrefRoom.Flags & FPR_SAVE_PLAY_PROGRESSIVE) {
				CheckDlgButton(hDialogo, IDC_CHECK_EXIT_PLAY_PROGRESSIVE ,1);
			}
			if (MyPrefRoom.Flags & FPR_NO_INFO_HIDE_OBJECTS) {
				CheckDlgButton(hDialogo, IDC_CHECK_NO_INFO_HIDEOBJ ,1);
			}
			if (MyPrefRoom.Flags & FPR_NO_STANDBY) {
				CheckDlgButton(hDialogo,IDC_CHECK_NO_STANDBY,1);
			}

			if (MyPrefRoom.Flags & FPR_EXTRA_INFO_SELECTION) {
				CheckDlgButton(hDialogo,IDC_CHECK_EXTRA_INFOS,1);
			}

			if (MyPrefRoom.Flags & FPR_DISABLE_NEW_TRIGGERS) {
				CheckDlgButton(hDialogo,IDC_CHECK_DISABLE_TRIGGERS,1);
			}

			// preview
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_PREVIEW);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			for (i=0;i<TotPreview;i++) {
				sprintf(BufferLog,"%s  (%d x %d)", 
						VetPreview[i].Descrizione,
						VetPreview[i].Zona.right- VetPreview[i].Zona.left ,
						VetPreview[i].Zona.bottom - VetPreview[i].Zona.top );

				Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,(WPARAM) Indice,(LPARAM) i);
			}
			SelezionaComboValore(WindCombo,MyPrefRoom.PreviewSize);
			
			if (MyPrefRoom.Flags & FPR_MAXIMIZE) {
				CheckDlgButton(hDialogo, IDC_CHECK_MAXIMIZE,1);
			}
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_BEND);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);

			for (i=0;i<TotMexBend;i++) {
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
										(LPARAM) VetMexBend[i]);
				SendMessage(WindCombo,CB_SETITEMDATA,(WPARAM) Indice,(LPARAM) i);
			}
			SelezionaComboValore(WindCombo,MyPrefRoom.BendType);

			if (MyPrefRoom.Flags & FPR_PUNTA_PIRAMIDI) {
				CheckDlgButton(hDialogo,IDC_CHECK_PUNTAPIRAMIDI,1);
			}


			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_AUTO_BACKUP);
			// riempire combo per auto-backup
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			// inserire il primo valore con testo "never"
			i = SendMessage(WindCombo, CB_ADDSTRING,0, (LPARAM) "NEVER (Disabled)");
			SendMessage(WindCombo,CB_SETITEMDATA,(WPARAM) i,(LPARAM) 0);
			for (i=0;i<TotAutoBackup;i++) {
				sprintf(BufferLog,"After %d changes",VetAutoBackup[i]);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,
						(WPARAM) Indice, (LPARAM) VetAutoBackup[i]);
			}

			SelezionaComboValore(WindCombo, MyPrefRoom.AutoBackup);

			// combo per default uv rotate
			WindCombo = GetDlgItem(hDialogo, IDC_COMBO_DEFAULT_UVROTATE);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			for (i=-64;i<65;i++) {
				sprintf(BufferLog,"%d",i);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,
						(WPARAM) Indice, (LPARAM) i);
			}
			SelezionaComboValore(WindCombo, MyPrefRoom.DefaultUvRotate);


			return false;
		case WM_CLOSE: 
			EndDialog(hDialogo,-1);

			return TRUE; 

		case WM_QUIT:
			EndDialog(hDialogo,-1);

			return true;
		case	WM_COMMAND:

			switch (LOWORD(wParam)) {
				case IDCANCEL:
					EndDialog(hDialogo,-1);
					return true;

				case IDOK:
					if (PrendiSettings(hDialogo)==false) return true;
					EndDialog(hDialogo,0);
					return true;
				case IDC_PUL_LISTA_PROGRAMMI:
					if (HIWORD(wParam) == BN_CLICKED) {
						ShowListaProgrammi(hDialogo);
						AggiornaComboProgrammi();
					}
					return true;
			
					
			}

	}
	return false;
}

// window message per finestra di ricerca FIND

BOOL CALLBACK WindProcedureFind(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND WindCombo;
	static HWND WindList;
	int		i;
	int *pIndiceItemSelezionato = (int*) 0x583BA4;
	int IndiceSlot;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;

	static char *VetNomiTipo[] = {"Object (Wad Object)", "Indices of all Items", "Floor Type", "Trigger (Activation Mode)",  
					"Trigger (What to trigger)", "Trigger of Plugin", "Trigger not handled",
					"Effect (Lights, sinks, cameras..,)", "Texture (currently selected)"};



	switch (uMsg) {
		case  WM_INITDIALOG:

			// riempire combo box
			WindList = GetDlgItem(hDialogo, IDC_S_LISTBOX);
			WindCombo = GetDlgItem(hDialogo, IDC_S_COMBO);

			// riempire combo box
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);


			for (i=0;i<TLF_TOT_ITEM;i++) {

					
				SendMessage(WindCombo,CB_ADDSTRING,0,(LPARAM) VetNomiTipo[i]);				

			}
			// seleziona il primo della lista
			PostMessage(WindCombo,CB_SETCURSEL,0,0);
		
			// riempire list box
			RiempieListaFind(WindList, TLF_OBJECTS);
			if (*pIndiceItemSelezionato != -1) {
				pVetBaseOggetti = *P2_VetBaseOggetti;
				if (pVetBaseOggetti[*pIndiceItemSelezionato].TypeObject == 8) {
					IndiceSlot =pVetBaseOggetti[*pIndiceItemSelezionato].SlotIndex;

					// se c'e' un item selezionato posizionarsi su quello
					for (i=0;i<TotList;i++) {
						// selezionare l'elemento che abbia lo stesso item data
						if (SendMessage(WindList,LB_GETITEMDATA,i,0) == 
													IndiceSlot) {
							// selezionare questo
							SendMessage(WindList,LB_SETCURSEL,i,0);
							break;
						}
					}
				}
			}


			return TRUE;
		case WM_CLOSE: 
			EndDialog(hDialogo,-1);

			return TRUE; 


		case WM_LBUTTONDBLCLK:
			GosubCerca(hDialogo, WindList,true,true);
			return TRUE;

		case WM_QUIT:
			EndDialog(hDialogo,-1);

			return true;

		case WM_VKEYTOITEM:
		case WM_KEYDOWN:
			switch (LOWORD(wParam)) {
			case VK_F1:
				ShowListFind(hDialogo,WindList);
				return -2;
			}
			return -1;

		case WM_COMMAND:

			switch (LOWORD(wParam)) {
				case IDCANCEL:
					EndDialog(hDialogo,-1);
					return true;

				case IDC_S_FIND:
					// Cercare primo
					GosubCerca(hDialogo, WindList,true,true);

					return TRUE;

				case IDC_S_CANCEL:
					EndDialog(hDialogo,-1);
					return TRUE;

				case IDC_S_PREVIOUS:
					GosubCerca(hDialogo, WindList,false,false);
					return true;

				case IDC_S_NEXT:
					GosubCerca(hDialogo, WindList,false,true);
					return true;
				case IDC_S_COMBO:
					switch (HIWORD(wParam)) {
					case CBN_SELENDOK:
						// cambiata selezione in combo box:
						// selezionare 
						i= SendMessage(WindCombo,CB_GETCURSEL,0,0);
						RiempieListaFind(WindList,i+1);
						return true;
					}
					break;
				case IDC_S_LISTBOX:
					switch (HIWORD(wParam)) {
					case LBN_DBLCLK:
						GosubCerca(hDialogo, WindList,true,true);
						return -1;
					}

					
			}
		}
	return false;
}



// crea una finestra che contiene un list box
// la gestione e' diversa a seconda di TipoListBox i cui valori
// iniziano con TLB_...
// restituisce l'indice che e' stato selezionato, o -1 se l'utente ha premuto cancel
int InputListBox(int TipoListBox)
{

	HWND *pMiaWind = (HWND*) 0x46E559;
	int RetValore;
	int	i;

	// mettrere puntatori in ordine sequenziale (nessun ordinamento)
	for (i=0;i<TotList;i++) {
		VetPuntaList[i]=i;
	}


	RetValore= DialogBoxParam(MiaIstanzaDll,
				MAKEINTRESOURCE(IDD_LISTBOX_WIND), *pMiaWind, 
				&WindProcedureList, 
				(LPARAM) TipoListBox);
	RimuoviMessaggiInput();
	return RetValore;




}
// visualizza un inputbox e restituisce il testo inserito, oppure NULL
// se utente ha premuto annulla
char *InputTextBox(char *pDescrizione, char *pTitolo, char *pDefault, int TipoPosizione)
{

	char *pResult;

	HWND *pMiaWind = (HWND*) 0x46E559;
	// copiare in variabili globali i testi usati per l'inizializzazione
	strcpy(InputBufDescrizione,pDescrizione);
	strcpy(InputBufTitolo,pTitolo);
	strcpy(InputBufDefault,pDefault);

	pResult= (char*) DialogBoxParam(MiaIstanzaDll,
				MAKEINTRESOURCE(IDD_INPUTBOX), *pMiaWind, 
				&WindProcedureInput, TipoPosizione);
	RimuoviMessaggiInput();
	
	return pResult;
}

void CambiaColore(DWORD *pMem, int TipoColore)
{
	char *pColore;
	char BufferTemp[256];
	char *pResult;
	char MexValore[80];
	DWORD Colore;
	DWORD Maschera;
	DWORD Temp;

	switch (TipoColore) {
	case TC_ROSSO:
		pColore="Red";
		break;
	case TC_VERDE:
		pColore="Green";
		break;
	case TC_BLU:
		pColore="Blue";
		break;
	}
	Colore = *pMem;

	Colore = Colore >> (TipoColore*8);
	Colore &= 0xff;
	sprintf(MexValore,"%d",Colore);

	sprintf(BufferTemp,"Set %s intensity (Range 0 - 255)",pColore);

	pResult=InputTextBox(BufferTemp,"Set Color", MexValore, TP_LUCI);

	if (pResult == NULL) return;
	// ora convertire il numero ricevuto
	Colore=300;
	sscanf(pResult,"%d",&Colore);

	if (Colore > 255) {
		MioMessageBox("Value out of range (0-255)","Error");
		return;
	}
	Maschera = 0xff << (TipoColore*8);
	Maschera ^= 0x00ffffff;

	Colore = Colore << (TipoColore*8);
	
	Temp = *pMem;
	Temp &= Maschera;
	Temp |= Colore;

	*pMem = Temp;

	__asm {
		
		mov eax, 401200h    ;CicloPrincipale
		call eax

		mov eax, 412B60h    ;AggionaValoriLuci
		call eax
	}
		



}
void Sub_PulsanteAmbienteRosso(StrInfoMouse *pDatiPulsante)
{

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	DWORD *pColore;

	pVetRooms = *p2VetRooms;	

	pColore = &pVetRooms[*pIndiceRoomAttuale].AmbientLightColor;

	CambiaColore(pColore,TC_ROSSO);

}

void Sub_PulsanteAmbienteVerde(StrInfoMouse *pDatiPulsante)
{

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	DWORD *pColore;

	pVetRooms = *p2VetRooms;	

	pColore = &pVetRooms[*pIndiceRoomAttuale].AmbientLightColor;

	CambiaColore(pColore,TC_VERDE);

}
// chiede a utente di modificare valore di colore oggetto
void CambiaColoreOggetto(void *pMemColore, int TipoOggetto, int TipoColore)
{
	char *pColore;
	char BufDescrizione[256];
	char *pTesto;
	int Numero;
	WORD MascheraWord;
	WORD *pMemWord;
	WORD ColoreWord;
	DWORD ColoreDword;
	DWORD MascheraDword;
	DWORD *pMemDword;

	switch (TipoColore) {
	case TC_ROSSO:
		pColore="Red";
		break;
	case TC_VERDE:
		pColore="Green";
		break;
	case TC_BLU:
		pColore="Blue";
		break;
	}

	sprintf(BufDescrizione, "Set %s intensity  (Range: 0 - 255)",pColore);
	pTesto = InputTextBox(BufDescrizione,"Set Color","",TP_LUCI);
	if (pTesto==NULL) return;
	Numero = 260;
	sscanf(pTesto,"%d",&Numero);
	if (Numero > 255) {
		MioMessageBox("Value out of range","Error");
		return;
	}

	// se e' luce usa 3 byte, se e' oggetto usa una singola word e ogni colore 
	// avra' 0x1f bit
	switch (TipoOggetto) {
	case 8:
		// ogggetto: convertire colore in modo da avere al massimo 0x1f valore
		Numero = Numero >> 3;

		// ora shiftarlo a seconda della posizione (5 bit di colore)
		Numero = Numero << (TipoColore * 5);  
		// ora calcolare maschera
		MascheraWord = 0x1f << (TipoColore *5);

		MascheraWord ^= 0xffff;
		pMemWord = (WORD *) pMemColore;
		ColoreWord = *pMemWord;

		ColoreWord &= MascheraWord;
		ColoreWord |= Numero;
		*pMemWord = ColoreWord;
		break;
	case 2:
		// luci, tre byte rosso, verde blue
		Numero = Numero << (TipoColore *8);
		MascheraDword = 0xff << (TipoColore * 8);
		MascheraDword ^= 0xffffffff;

		pMemDword = (DWORD *) pMemColore;
		ColoreDword = *pMemDword;

		ColoreDword &= MascheraDword;
		ColoreDword |= Numero;

		*pMemDword = ColoreDword;
		break;
	}

	__asm {
		lea		eax, TipoOggetto
		cmp		word ptr [eax], 2
		jnz		Zompa

		push 01h
		mov eax, 418A20h    ;AggiornaIntensitaLuce
		call eax
		add	 esp,4
Zompa:

		mov eax, 401200h    ;CicloPrincipale
		call eax

		mov eax, 412B60h    ;AggionaValoriLuci
		call eax
	}
				

}
// chiamato quando si vuole modificare colore per object (o luce)
// restituisce false se non e' possibile farlo (e visualizza un messaggio di spiegazione)
// se restituisce true imposta un puntatore alla memoria del colore e setta il tipo
// (oggetto =8  o luce = 2)
bool VerificaColoreObject(void **pBaseColore, int *pTipoOggetto)
{

	int *pTestModoLightning = (int *) 0x583BE8;
	int *pTestFaceEdit = (int*) 0x583BDC;
	int *pIndiceOggettoSel = (int*) 0x583BA4;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	int Tipo;
	void *pMem;
	StrItemEffectRoom **p2VetEffetti = (StrItemEffectRoom**) 0x56F498;
	StrItemEffectRoom*pVetEffetti;
	int FirstEffect;

	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetEffetti = *p2VetEffetti;

	if ((*pTestModoLightning) ==0) {
		MioMessageBox("No changing to object color when LIGHTNING button is off","Error");
		return false;
	}

	if ((*pTestFaceEdit) ==0) {
		MioMessageBox("No changing to object color when FACE EDIT button is off","Error");
		return false;
	}
	if ((*pIndiceOggettoSel) ==-1) {
		MioMessageBox("No object selected","Error");
		return false;
	}

	Tipo = pVetBaseOggetti[*pIndiceOggettoSel].TypeObject;
	*pTipoOggetto = roomGetCategoriaTipoOggetto(Tipo);
	switch (*pTipoOggetto) {
	case 8:
		// oggetto
		pMem = &pVetBaseOggetti[*pIndiceOggettoSel].ObjTintOrSpeed;
		break;
	case 2:
		// luce
		// se pero' e' shadow dare errore
		if (Tipo == 0x6000) {
			MioMessageBox("You cann't set color for Shadows","Error");
			return false;
		}
		FirstEffect= pVetBaseOggetti[*pIndiceOggettoSel].SlotIndex;

		pMem = &pVetEffetti[FirstEffect].Red_Color;
		break;


	default:
		MioMessageBox("You can change color only for Objects and Lights","Error");
		return false;
	}

	*pBaseColore= pMem;
	return true;

}

void Sub_PulsanteObjectRosso(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;
	if (TipoOggetto == 2) {
		MioMessageBox("You should use frame named \"Colour\" to change colors of lights","Warning");
		return;
	}

	CambiaColoreOggetto(pMem,TipoOggetto,TC_ROSSO);

}

void Sub_PulsanteObjectVerde(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;
	if (TipoOggetto == 2) {
		MioMessageBox("You should use frame named \"Colour\" to change colors of lights","Warning");
		return;
	}

	CambiaColoreOggetto(pMem,TipoOggetto,TC_VERDE);

}

void Sub_PulsanteObjectBlu(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;
	if (TipoOggetto == 2) {
		MioMessageBox("You should use frame named \"Colour\" to change colors of lights","Warning");
		return;
	}

	CambiaColoreOggetto(pMem,TipoOggetto,TC_BLU);

}

void Sub_PulsanteLightRosso(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;

	CambiaColoreOggetto(pMem,TipoOggetto,TC_ROSSO);

}

void Sub_PulsanteLightVerde(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;

	CambiaColoreOggetto(pMem,TipoOggetto,TC_VERDE);

}

void Sub_PulsanteLightBlu(StrInfoMouse *pDatiPulsante)
{
	void *pMem;
	int  TipoOggetto;

	if (VerificaColoreObject(&pMem,&TipoOggetto)==false) return;

	CambiaColoreOggetto(pMem,TipoOggetto,TC_BLU);

}
// chiede di modificare valore per luce attualmente selezionata e campo TipoCampo
void CambiaValoreLuce(int TipoCampoLuce)
{

	char *pDescrizione;
	float *pValore;
	float Minimo, Massimo;
	int *pTestModoLightning = (int *) 0x583BE8;
	int *pTestFaceEdit = (int*) 0x583BDC;
	int *pIndiceOggettoSel = (int*) 0x583BA4;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	char MexDefault[80];
	char *pTesto;
	StrItemEffectRoom **p2VetEffetti = (StrItemEffectRoom**) 0x56F498;
	StrItemEffectRoom*pVetEffetti;
	int FirstEffect;
	float NuovoValore;
	float TempValore;
	WORD ValWord;

	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetEffetti = *p2VetEffetti;

	if ((*pTestModoLightning) ==0) {
		MioMessageBox("No changing to lights when LIGHTNING button is off","Error");
		return;
	}

	if ((*pTestFaceEdit) ==0) {
		MioMessageBox("No changing to lights when FACE EDIT button is off","Error");
		return;
	}
	if ((*pIndiceOggettoSel) ==-1) {
		MioMessageBox("No light selected","Error");
		return;
	}

	FirstEffect= pVetBaseOggetti[*pIndiceOggettoSel].SlotIndex;



	switch (TipoCampoLuce) {
	case TCL_LIGHT_X:
		pValore = &pVetEffetti[FirstEffect].Degree_X;
		pDescrizione = "Set X Light Degree (Range: 0 - 359)";
		Minimo=0;
		Massimo=359.9f;
		break;

	case TCL_LIGHT_Y:
		pValore = &pVetEffetti[FirstEffect].Degree_Y ;
		pDescrizione = "Set Y Light Degree (Range: 0 - 359)";
		Minimo=0;
		Massimo=359.9f;
		break;
	case TCL_LIGHT_LEN:
		pValore = &TempValore;
		TempValore= pVetEffetti[FirstEffect].Spot_Len;
		TempValore /= 1024;

		pDescrizione = "Set Light Length (Range: 0 - 31)";
		Minimo=0;
		Massimo=31.9f;
		break;
	case TCL_LIGHT_CUT:
		pValore = &TempValore;
		TempValore= pVetEffetti[FirstEffect].Spot_Cut;
		TempValore /=1024;
		pDescrizione = "Set Light Cut (Range: 0 - 63)";
		Minimo=0;
		Massimo=63.9f;
		break;
	case TCL_SPOT_INT:
		TempValore = pVetEffetti[FirstEffect].Intensity;
		pValore =&TempValore;
		TempValore /= 8192;
		pDescrizione = "Set Spot Intensity (Range: -1.0 -  1.0)";
		Minimo=-1.0f;
		Massimo = 1.0f;
		break;
	case TCL_SPOT_OUT:
		TempValore = pVetEffetti[FirstEffect].Light_out;
		pDescrizione = "Set Outside Spot Radius (Range: 0 - 31)";
		pValore = &TempValore;
		TempValore /=1024;

		Minimo=0;
		Massimo = 31.99f;
		break;
	case TCL_SPOT_IN:
		TempValore = pVetEffetti[FirstEffect].Light_In;
		pValore = &TempValore;
		TempValore /=1024;
		pDescrizione = "Set Inside Spot Radius (Range: 0 -  31)";
		Minimo=0;
		Massimo = 31.99f;
		break;
	}

	sprintf(MexDefault,"%.2f", *pValore);

	pTesto = InputTextBox(pDescrizione,"Set Light Parameter",MexDefault,TP_LUCI);
	if (pTesto==NULL) return;
	NuovoValore=-1;

	sscanf(pTesto,"%f", &NuovoValore);
	if (NuovoValore < Minimo || NuovoValore > Massimo) {
		MioMessageBox("Value out of range","Error");
		return;
	}

	
	*pValore = NuovoValore;
	switch (TipoCampoLuce ) {
	case TCL_SPOT_INT:
		// va convertito in un intero sbalengo
		TempValore = NuovoValore * 8192;
		ValWord = (WORD) TempValore;
		pVetEffetti[FirstEffect].Intensity = ValWord;
		break;
	case TCL_LIGHT_LEN:
		TempValore *= 1024;
		pVetEffetti[FirstEffect].Spot_Len = TempValore;
		break;
	case TCL_LIGHT_CUT:
		TempValore *= 1024;
		pVetEffetti[FirstEffect].Spot_Cut = TempValore;
		break;
	case TCL_SPOT_OUT:
		TempValore *= 1024;
		pVetEffetti[FirstEffect].Light_out = TempValore;
		break;
	case TCL_SPOT_IN:
		TempValore *= 1024;
		pVetEffetti[FirstEffect].Light_In = TempValore;
		break;
	}
	__asm {

		push 01h
		mov eax, 418A20h    ;AggiornaIntensitaLuce
		call eax
		add	 esp,4

		mov eax, 401200h    ;CicloPrincipale
		call eax

		mov eax, 412B60h    ;AggionaValoriLuci
		call eax
	}	

}
void Sub_PulsanteLightX(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_LIGHT_X);

}

void Sub_PulsanteLightY(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_LIGHT_Y);

}

void Sub_PulsanteLightLen(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_LIGHT_LEN);

}

void Sub_PulsanteLightCut(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_LIGHT_CUT);

}

void Sub_PulsanteSpotInt(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_SPOT_INT);

}

void Sub_PulsanteSpotOut(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_SPOT_OUT);

}

void Sub_PulsanteSpotIn(StrInfoMouse *pDatiPulsante)
{

	CambiaValoreLuce(TCL_SPOT_IN);

}
// viene chiamata prima di salvare vettore di rilocazione di stanze
// per poi salvarle subito dopo in file .tom
void CalcolaVetRemapRooms(void)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	int i;
	int IndiceRel;

	pVetRooms = *p2VetRooms;

	for (i=0;i<MAX_ROOMS;i++) {
		GlobNgle.VetRemapRooms[i] = -1;
	}

	IndiceRel=0;
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {
			GlobNgle.VetRemapRooms[i] = IndiceRel;
			IndiceRel++;
		}
	}

}
void Sub_PulsanteAmbienteBlu(StrInfoMouse *pDatiPulsante)
{

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	DWORD *pColore;

	pVetRooms = *p2VetRooms;	

	pColore = &pVetRooms[*pIndiceRoomAttuale].AmbientLightColor;

	CambiaColore(pColore,TC_BLU);

}

void ShowTexture(int TipoTexture)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	

	if (HandleBitMap == NULL) {
		MostraMessageBoxNGLE("No tga map loaded",3);
		return;
	}

	// codice originale, da riattivare dopo
	DialogBoxParam(MiaIstanzaDll,MAKEINTRESOURCE(IDD_DIALOG_TEXTURE),
				*pMiaWind, &WindProcedureTexture, 
				(LPARAM) TipoTexture);

	RimuoviMessaggiInput();
}
// visualizza finestra e consente ricerca di diversi elementi
void FindInRoom(void)
{


	HWND *pMiaWind = (HWND*) 0x46E559;


	DialogBox(MiaIstanzaDll,
				MAKEINTRESOURCE(IDD_DIALOG_FIND), *pMiaWind, &WindProcedureFind);

	RimuoviMessaggiInput();
	
}

// visualizza finestra delle preferenze di ngle
void ShowSettings(void)
{

	HWND *pMiaWind = (HWND*) 0x46E559;
	int RetValue;
	StrPrefRoom SalvaPref;


	memcpy(&SalvaPref,&MyPrefRoom,sizeof(StrPrefRoom));

	RetValue =DialogBox(MiaIstanzaDll,
				MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), *pMiaWind, 
				&WindProcedureSettings);
	if (RetValue==-1) {
		// premuto CANCEL, ripristinare preferenze originali
		memcpy(&MyPrefRoom,&SalvaPref,sizeof(StrPrefRoom));

	}else {
		// avvisare se sono state effettuate modifiche visibili solo al prossimo
		// riavvio
		ImpostaSizePreview();
		if (((MyPrefRoom.Flags ^ SalvaPref.Flags) & FPR_NO_EXTENDED_MEMORY) ||
			((MyPrefRoom.Flags ^ SalvaPref.Flags ) & FPR_DISABLE_NEW_TRIGGERS))  {
			MioMessageBox( "Warning: You changed some setting that requires to close and re-launch ngle to be updated.","Information");
		}
	}
	RimuoviMessaggiInput();


}
// cancella tutti i tasti rilevati da ultimo directinput
void AzzeraTastiPremuti(void)
{
	static BYTE * pVetInputKeyboard = (BYTE *) 0x46DF98;
	int i;

	for (i=0;i<256;i++) {
		pVetInputKeyboard[i]=0;
	}

}

void CaricaProgettoMio(char *pNome)
{

	char *pNomeProject = (char*) 0x46B858;  // BufferNomeProject
	strcpy(pNomeProject, pNome);



	__asm {
		pushad

		lea		eax, pNome
		mov		eax,dword ptr [eax]
		push	eax

		mov eax, 426B00h ; GosubLoadProject:
		call	eax
		add		esp,4
		call AzzeraTastiPremuti
		popad
	}


}

// mostra finestra con elementi di oggetti riallocati
void ShowListObjectRemapping(void)
{
	int i;
	int Indice;
	int TipoOggetto;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	char *pNomeOggetto;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int IndiceRoom;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;

	pVetSlotOggetti=*p2VetSlotOggetti;
	pVetRooms = *p2VetRooms;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	if (SalvaVettoriRemap.TestAttivo == false) {
		MostraMessageBoxNGLE("The list of object ID remapping is available only AFTER an output wad operation. Currently no output wad (creation of tom file) has been performed",0);
		return;
	}
	TotList=0;
	for	(i=0;i<6000;i++) {
		Indice = SalvaVettoriRemap.VetObjRemap[i];
		if (Indice != -1) {
			TipoOggetto = pVetBaseOggetti[i].TypeObject;

			// trovare nome oggetto
			switch (TipoOggetto) {
			case 8:
				// oggetto slot
				pNomeOggetto = 
					pVetSlotOggetti[pVetBaseOggetti[i].
							SlotIndex].pNome;
				break;
			case 0x4800:
				// camera
				pNomeOggetto = "CAMERA";
				break;
			case 0x4080:
				pNomeOggetto= "CAMERA_FIXED";
				break;
			case 0x4040:
				pNomeOggetto = "CAMERA_FLYBY";
				break;
			case 0x4400:
				pNomeOggetto = "SINK";
				break;
			}
			IndiceRoom= pVetBaseOggetti[i].IndexRoom;
			// formato:
			// NomeStanza   Oggetto  Posizione  IndiceOld IndiceNew
			sprintf(BufferLog,"%-20s %-20s  (%2d,%-2d) %4d   %4d",
				pVetRooms[IndiceRoom].Nome, pNomeOggetto, 
				pVetBaseOggetti[i].XOrgBlock, 
				pVetBaseOggetti[i].ZOrgBlock,
				i, Indice);
			strcpy(VetNomiList[TotList],BufferLog);

			TotList++;
									
		}
	}

	Indice = InputListBox(TLB_RemapObjects);

	if (Indice != -1) {
		// indice e' il numero di indice in tr4
		sprintf(BufferLog,"%d", Indice);
		SetClipboard(BufferLog);
	}

}

void Menu_ShowRemapObj(void *pPunta)
{
	ShowListObjectRemapping();
}
void Sub_LastProjects(StrInfoMouse *pPulsante)
{
	int Indice;
	char *pNome;
	char *pNomeProject = (char*) 0x46B858;  // BufferNomeProject
	int	i;
	char BufferTemp[256];
	FILE *pTempFile;
	int		NVersione;

	// inizializzare VetNomiList[] e VetPuntaList[]/ TotList sulla base di TipoListBox

	// mettere nomi di vecchi progetti
	TotList= MyPrefRoom.TotOldProjects;

	for (i=0;i<MyPrefRoom.TotOldProjects;i++) {
		pNome = MyPrefRoom.VetOldProjects[i].NomeProject;
		if (EsisteFile(pNome)==true) {
			// leggere numero di versione
			pTempFile = fopen(pNome,"rb");
			fseek(pTempFile,8,SEEK_SET);

			fread(&NVersione,4,1,pTempFile);
			fclose(pTempFile);

			
		}else
		{	
			MyPrefRoom.VetOldProjects[i].NomeProject[0]=0;
			pNome="";
		}
		if (strlen(pNome) > 0) {
			sprintf(BufferTemp,"%-28s %2d  %s", SoloNomeSenzaExt(pNome), 
											NVersione, SoloDir(pNome));
			SottolineaTesto(BufferTemp);
			strcpy(VetNomiList[i],BufferTemp);
		}else
		{
			strcpy(VetNomiList[i], "");
		}


	}


	Indice = InputListBox(TLB_LoadLastProjects);

	if (Indice != -1) {
		pNome = MyPrefRoom.VetOldProjects[Indice].NomeProject;
		//DestroyWindow(WindowLastProjects);
		// salvare il nome del file anche pBufferproject
		CaricaProgettoMio(pNome);



	}

}
// aggiunge alcune zone di memoria da controllare del room editor
void AggiungiControlloMemoria(void)
{

	AddZonaMemoria(0x46AC10, 0x400, "VetPtrMeshObject");
	AddZonaMemoria(0x469800, 0x400, "ZonaVerticiObjectMesh");




}
// scandisce cartella trle per trovare file "Plugin*.trg"
// e li carica globngle.vertplugin
void CaricaPluginsForNgle(void)
{
	int i;
	StrListaFiles *pVetFiles;
	int TotFiles;
	char *pSoloNome;
	int j;
	char NomeFile[256];
	SYSTEMTIME ScambioData;
	StrListaFiles ScambioNome;
	SYSTEMTIME *pVetDate;

	GlobNgle.TotPluginNgle=0;
	pVetFiles = TrovaFiles(Dir_Trle, "Plugin*.trg", &TotFiles);

	if (TotFiles==0) return;

	// ora ordinare i nomi file in base alla data: prima i piu' vecchi
	// e dopo i piu' recenti
	pVetDate= (SYSTEMTIME*) malloc(sizeof(SYSTEMTIME) * TotFiles);

	for (i=0;i<TotFiles;i++){
		GetDataDelFileSys(pVetFiles[i].Testo, &pVetDate[i]);
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

				strcpy(ScambioNome.Testo, pVetFiles[i].Testo);
				strcpy(pVetFiles[i].Testo, pVetFiles[j].Testo);
				strcpy(pVetFiles[j].Testo, ScambioNome.Testo);

			}
		}

	}


	for (i=0;i<TotFiles;i++) {
		sprintf(NomeFile, "%s\\%s", Dir_Trle, pVetFiles[i].Testo );

		pSoloNome= SoloNomeSenzaExt(pVetFiles[i].Testo);

		j= GlobNgle.TotPluginNgle;
		strcpy(GlobNgle.VetPluginNgle[j].Nome, pSoloNome);
		GlobNgle.VetPluginNgle[j].TotUsato=0;
		GlobNgle.VetPluginNgle[j].PluginId = i+1;

		CaricaNGConstants(LNGC_PLUGIN, &GlobNgle.VetPluginCostants[j], NomeFile);
		GlobNgle.TotPluginNgle++;

	}


}
// chiamata alla'avvio di room editor, e dopo aver effettuato le patch di memoria
void InizializzaRoom(void)
{
	int i;
	int j;
	FILE *pFile;
	char *pMem;
	DWORD *pPatchGenericheAsm = (DWORD *) 0x454f04;
	
	static int VetInventory[] = {
			0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 
			0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 
			0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 
			0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 
			0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 
			0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 
			0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 
			0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 
			0x100, 0x101, 0x128, 0x129, 0x12A, 0x12B, 0x12C, 0x12D, 
			0x12E, 0x12F, 0x130, 0x131, 0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162, 
			0x163, 0x164, 0x165, 0x166, 0x167, 0x169, 0x16A, 0x16B, 0x16C, 
			0x16E, 0x16F, 0x170, 0x171, 0x172, 0x173, 0x174,-1};

	// cancellare file di log per caricamento ng_constants.txt
	remove(GetFileCrash("ng_startup_log.txt"));

	// Inizializzare Preferenze: MyPrefRoom
	memset(&MyPrefRoom,0, sizeof(MyPrefRoom));
	memset(&VetMieiNomiSlot, 0, sizeof(BaseNomiSlot));
	memset(&GlobNgle, 0, sizeof(StrGlobNgle));
	

	for (i=0;VetInventory[i]!=-1;i++) {
		GlobNgle.VetInventory[i] = VetInventory[i];
	}

	GlobNgle.VetInventory[i] = -1;

	for (i=0;i<MAX_PROGETTI;i++) {
		for (j=0;j<MAX_OLD_ROOMS;j++) {
			MyPrefRoom.VetOldProjects[i].VetLista[j]=-1;
		}
	}

	// provare ad anticipare inizializzazione di puntatore a
	// patch Patch_GenericheAsm poiche' e' usata da InviaLog
	*pPatchGenericheAsm = (DWORD) &PatchGenericheRoom;


	MyPrefRoom.SpinningTimeInput = 1; // indice di riferimento a dati slowdoan
	MyPrefRoom.SpinningStep = 5; // indice di riferimento ai dati di rotazione
	MyPrefRoom.MouseSensitivity = 120;
	MyPrefRoom.MouseMinStep = 16;
	MyPrefRoom.MouseMaxStep = 256;
	MyPrefRoom.PreviewSize =1;
	MyPrefRoom.AutoBackup =0;
	MyPrefRoom.Flags = FPR_NO_RESUME_CRASH;
	DatiTexture.TotLastSort=0;
	MyPrefRoom.DefaultUvRotate = 1;

	// impostare in VetPreview (totprevieeww-1) la dimensione attuale di schermo
	GetClientRect(GetDesktopWindow(), &VetPreview[TotPreview-1].Zona);

	DatiFindReplace.TestAttivo=false;

	// --- fine inizialzizazione
	// azzerare file wad
	NomeFileWad[0]=0;

	if (EsisteFile(NomePrefRoom)== true) {
		// caricare il file
		pFile = fopen(GetFileTrle(NomePrefRoom),"rb");
		if (pFile != NULL) {		
			fread(&MyPrefRoom,sizeof(MyPrefRoom),1,pFile);
			fclose(pFile);

		}
		
	}

	if (MyPrefRoom.SpinningStep >= TotIncrementoStep) MyPrefRoom.SpinningStep = 5;
	if (MyPrefRoom.SpinningTimeInput >= 10) MyPrefRoom.SpinningTimeInput = 2;


	// allocare 256 carattere per ogni elemento di VetNomiList
	// in modo da poter salvare e modificare gli elementi
	pMem = (char *) malloc(256 * MAX_ITEM_FIND_LIST);
	for (i=0;i<MAX_ITEM_FIND_LIST;i++) {
		VetNomiList[i] = pMem;
		pMem+= 256;
	}


	MyPrefRoom.IndiceProjectNow=-1;
	OldIndiceRoom =-1;
	OldSizeGrid=0;
	OldIndiceProject=-1;

	// esegue patch per inserire indrizzo di sub per pulsante carica last projects
	__asm {
		pushad

		mov		eax, 45921eh ;Sub per DatiPulsante_LoadObjects
		lea		ebx, Sub_LastProjects
		mov		dword ptr [eax], ebx

		popad
	}

	hClasseNG = NULL;

	// inizializzare flag per tipo floor
	for (i=0;i<24;i++) {
		VetFlagFloor[i] = 1 << i;
	}
	VetFlagFloor[23] = 0x3C0;

	__asm {
		// setta memoria di inizio di VetBigTail
		mov eax, 454FF8h  ;Ptr_VetBigTails
		lea ecx, VetBigTails
		mov dword ptr [eax], ecx
	}

	PenNulla=CreatePen(PS_NULL,2,NULL);
	PenNero=CreatePen(PS_SOLID,2,RGB_Nero);
	PenRosso=CreatePen(PS_SOLID,2,RGB_Rosso);
	PenVerde=CreatePen(PS_SOLID,2,RGB_Verde);
	BrushRosso = CreateSolidBrush(RGB_Rosso);
	BrushNero = CreateSolidBrush(RGB_Nero);
	BrushNullo = (HBRUSH ) GetStockObject( HOLLOW_BRUSH);

	AggiungiControlloMemoria();

	CursoreClessidra=LoadCursor(NULL,IDC_WAIT);



	// azzera roba per rotazioni
	memset(&DatiRotazioni,0,sizeof(StrDatiRotazioni));
	// azzera struttura per button window
	memset(&BaseWindow,0, sizeof(StrBaseButtonsWindow));
	TestBloccoUndo=0;
	WindComboProgrammi=NULL;
	LastTimeStandBy=0;
	LastTimeMsgBox=0;
	TotMsgbox=0;
	TestIgnoraMsgBox=false;
	GlobNgle.RoomTexAnimate.Tot_UV_Rotate =0;

	for  (i=0;i<40;i++) {
		GlobNgle.RoomTexAnimate.VetInfoRangeAnim[i]=0;
	}
	LastInfoRoom[0]=0;
	TestShowTitleInfo=false;
	sprintf(LastTitleWindow,"NGLE %s - <EMPTY>", MexVersione);
	LastMexSelezione[0]=0;
	TotCambiBackup=0;
	GlobNomeTom[0]=0;
	GlobHandleFileLog=0;
	if ((MyPrefRoom.Flags & FPR_DISABLE_NEW_TRIGGERS) == 0) {
		CaricaNGConstants(LNGC_MAIN, &MainBaseCostanti, NULL );
		// se c'e' file tradotto caricare anche quello
		if (EsisteFile("my_ng_constants.txt")== true) {
			LogTestuale("Detected extra language file: \"my_ng_constants.txt\" file");
			LogTestuale("================= BEGIN PARSING EXTRA LANGUAGE FILE ============");

			CaricaNGConstants(LNGC_TRANSLATION, NULL, "my_ng_constants.txt");
			LogTestuale("=============== COMPLETED PARSING EXTRA LANGUAGE FILE ==========");
		}
	}

	CaricaPluginsForNgle();
	CalcolaVersione(&GlobNgle.BaseVersione);
}

// room editor
void PatchVetTextureBumpMap(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pVTBumpMap;

	StrPatch VetPatch[] = {{0x42704E, true},
		{0x427AA8, true},
		{0x42C081, true},
		{0x42C177, true},
		{0x42CB49, true},
		{0x42CC61, true},
		{0x42CC8C, true},
		{0x42CC96, true},
		{0x42CCAC, true},
		{0x42CCB8, true}};

	OffsetOldInizio = 0x56BE80;
	OffsetOldFine=0x56BF80;
	TotPatchs=10;
	pVTBumpMap = (DWORD *) 0x454642;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
			OffsetOldFine, MAX_TAIL_INFOS,"VetTextureBumpMap");
	*pVTBumpMap = OffsetNew;
	pVetTextureBumpMap = (BYTE*) OffsetNew;
}
// room editor
void PatchVetAnimatedTilesTextures(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pVetAnimatedTilesTextures;

	StrPatch VetPatch[] = {{0x426FF4, true},
		{0x427A72, true},
		{0x42B144, true},
		{0x42B1FB, true},
		{0x42B291, true},
		{0x42B42E, true},
		{0x42B67A, true},
		{0x42B6D9, true}};

	OffsetOldInizio = 0x56C020;
	OffsetOldFine=0x56C420;
	TotPatchs=8;
	pVetAnimatedTilesTextures = (DWORD *) 0x454646;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,OffsetOldFine, 
			0x1000,"VetAnimatedTilesTextures");
	*pVetAnimatedTilesTextures = OffsetNew;
}

void PatchMax_ScrollY(void)
{
	
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x4595CE, 'D'}};

	TotPatchs=1;
	TotOld = 0x2E4;
	TotNew=0xd0;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

int TrovaSizeDefaultTexture(void)
{
	if (VersioneProject == 50) return 128;

	return 64;
	

}

// riceve l'idnice tail con le info di tex selezionata attuale
// resittuisce l'indice corrispondente a texture full
// nella zona della mappa. se il valore e' maggiore di 256
// resituuisce -1
int TrovaIndiceSelTail(short IndiceTail)
{

	DWORD *Ptr_TextureRawImage = (DWORD*) 0x56C4F8;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	DWORD MemImage;
	DWORD BaseTga;
	DWORD Cx, Cy;
	DWORD SizeSrc;
	DWORD GrigliaX,GrigliaY;
	DWORD NPerRow;

	pVetTailInfos = *p2VetTailInfos;

	BaseTga = (DWORD) *Ptr_TextureRawImage;
	MemImage = (DWORD) pVetTailInfos[IndiceTail].pMemImage;
	MemImage -= BaseTga;

	// adesso scomporre in coordinata in modo da avere 
	// le due coordinate di griglia.
	Cx = MemImage & 0xFF;
	Cy = MemImage >> 8;
	// ora ho le due coordinate in pixel
	// adesso devo scomporle in griglia a seconda di dimensione
	// standard di texture

	if (VersioneProject == 50) {
		SizeSrc = 128;
		NPerRow=2;
	}
	else {
		SizeSrc = 64;
		NPerRow=4;
	}
	

	GrigliaX = Cx / SizeSrc;
	GrigliaY = Cy / SizeSrc;

	return (GrigliaY * NPerRow)+GrigliaX;
	
}
// aggiorna testi e status dei pulsanti sulla base dei valori di ngle
void AggiornaButtonsWindow(void)
{
	int i;
	StrButtonRec *pRec;
	HWND TempWind;
	int IdPulsante;
	StrHeaderButWindow *pHeader;
	HFONT hFont, OldFont;
	char *pTesto;
	int j;
	HDC MioHdc;
	pHeader= &BaseWindow.Header;

	for (i=0;i<BaseWindow.TotButtons;i++) {
		pRec = &BaseWindow.pVetButtons[i];

		if (pRec->TestFlipFlop) {
			// aggiornare status dei pulsanti checkbox

			if (pRec->OldBackColor == pRec->pDatiPulsante->ColoreFondo) {
				// unchecked
				CheckDlgButton(BaseWindow.hWind, i*16, BST_UNCHECKED);
			}else {
				// checked
				CheckDlgButton(BaseWindow.hWind, i*16, BST_CHECKED);
			}

		}
		// se c'e' info frame, fare redraw per finestra
		// uhm, o forse riscrivo io
		if (pRec->TipoRecord=='F') {
			// troppi lampeggimenti a rifare tutto
			// meglio riscrivere solo il testo e solo quando e' cambiato
			if (strcmpi(OldFrameInfoText, BaseWindow.LastInfoStampato) != 0) {

				MioHdc=GetDC(BaseWindow.hWind);
				// impostare colori
				SetBkMode(MioHdc, OPAQUE);
				SetBkColor(MioHdc, pRec->BackColor);
				SetTextColor(MioHdc, pRec->ForeColor);
				// stampare testo

				pTesto = OldFrameInfoText;
				strcpy(BaseWindow.LastInfoStampato, pTesto);

				hFont=pHeader->FontFrames.hFont;
				OldFont = (HFONT) SelectObject(MioHdc, pHeader->FontFrames.hFont);
						
				FillRect(MioHdc,&pRec->Box, pRec->hBrush );
				DrawText(MioHdc,
					pTesto, 
					strlen(pTesto), 
					&pRec->Box, 
					DT_EXPANDTABS | DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL);
				SelectObject(MioHdc, OldFont);
				ReleaseDC(BaseWindow.hWind, MioHdc);
			}
		}
		if (pRec->TipoRecord=='C') {
			// color palette
			// vedere se selezione e' cambiata
			if (* BaseWindow.pTestUsaTexture != BaseWindow.PaletteLastTest ||
				*BaseWindow.pIndiceTexture != BaseWindow.PaletteLastSelezionato) {
				if (BaseWindow.PaletteLastSelezionato != -1) {
					IdPulsante = i*16 + BaseWindow.PaletteLastSelezionato;
					TempWind = GetDlgItem(BaseWindow.hWind, IdPulsante);
					RedrawWindow(TempWind,NULL,NULL, RDW_INVALIDATE);
				}
				
				j = *BaseWindow.pIndiceTexture;
				if (j < 15) {
					IdPulsante = i*16 + j;
					TempWind = GetDlgItem(BaseWindow.hWind, IdPulsante);
					RedrawWindow(TempWind,NULL,NULL, RDW_INVALIDATE);					

				}


			}
			

		}

	}
}
// visualizza in zona info, informazioni riguardo texture
void ShowStatistiche(void)
{
	BYTE *pColorePerTesto = (BYTE*) 0x461CA8;
	int* pTotTexture64x64 = (int*) 0x56C4F0;
	int* pN_TextureTails = (int*) 0x56C4F4;
	char BufferTemp[256];
	int  TotaleTextures;
	int *pTotOverlaps = (int*) 0x599270;
	int *pTotBoxes = (int*) 0x5A51C4;
	int TotTextures;
	short *pIndiceTextureSelezionata = (short*) 0x583BBA;
	int  TextureNow;
	int  *pTotEffects = (int*) 0x56F488;

	// per ora mettere come fisso il numero di texture a 1024 ma poi a seconda
	// di versione progetto usare 256 se sono quelle 128x128
	TotTextures=*pTotTexture64x64;

	
	if (VersioneProject !=  50 &&  VersioneProject!=51) {
		TotaleTextures=MAX_TAIL_INFOS;
		VersioneProject=49;
	}
	else{
		TotaleTextures=256;
		TotTextures /= 4;

	}


	sprintf(BufferTemp,"Textures: %d of %d", TotTextures,TotaleTextures);
	
	*pColorePerTesto=0;
	if (TotaleTextures == 256) {
		if (TotTextures >= 240) *pColorePerTesto=4;
	}else{
		if (TotTextures >= 1000) *pColorePerTesto=4;
	}

	strcat(OldFrameInfoText, " ");
	strcat(OldFrameInfoText, BufferTemp);

	ShowTestoSuSchermo(350, 666,BufferTemp);

	sprintf(BufferTemp,"TexInfos: %d of %d", *pN_TextureTails, MAX_TAIL_INFOS);
	if (*pN_TextureTails >= 1000) 
		*pColorePerTesto=4;
	else
		*pColorePerTesto=0;

	strcat(OldFrameInfoText, " ");
	strcat(OldFrameInfoText, BufferTemp);
	ShowTestoSuSchermo(470, 666, BufferTemp);

	if (*pIndiceTextureSelezionata == -1) {
		strcpy(BufferTemp,"Current Texture: none");
	}else
	{
		TextureNow = TrovaIndiceSelTail(*pIndiceTextureSelezionata);
		sprintf(BufferTemp,"Current Texture: %d",TextureNow);
	}
	*pColorePerTesto=0;
	ShowTestoSuSchermo(600,666, BufferTemp);

	strcat(OldFrameInfoText, " ");	
	strcat(OldFrameInfoText, BufferTemp);

	if (MyPrefRoom.Flags & FPR_EXTRA_INFO_SELECTION) {
		sprintf(BufferLog," Changes: %d",TotCambiBackup);
	}else {
		BufferLog[0]=0;
	}


	sprintf(BufferTemp,"Effects: %d of %d Overlaps: %d of 16384  Boxes: %d of 2040 %s", 
		*pTotEffects, 0x300, *pTotOverlaps, *pTotBoxes, BufferLog);
	if (*pTotEffects >= 750) 
		*pColorePerTesto=4;
	else
		*pColorePerTesto=0;

	ShowTestoSuSchermo(296,674, BufferTemp);
	strcat(OldFrameInfoText, "\r\n");	
	strcat(OldFrameInfoText, BufferTemp);

	*pColorePerTesto=0;

}

// room editor
void PatchZonaPeTextureWad(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pZonaPeTextureWad;

	StrPatch VetPatch[] = {{0x41F2E6, true},
		{0x41F31F, true},
		{0x41F37C, true},
		{0x41F44D, true},
		{0x41F516, true},
		{0x41F528, true},
		{0x41F8B4, true},
		{0x41FB5E, true},
		{0x41FB87, true},
		{0x41FB94, true},
		{0x41FBA3, true},
		{0x41FBB6, true},
		{0x41FBD8, true},
		{0x41FBDE, true},
		{0x41FC0E, true}};

	OffsetOldInizio = 0x469C00;
	OffsetOldFine=0x46A400;
	TotPatchs=15;
	pZonaPeTextureWad = (DWORD *) 0x45464A;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio, 
				OffsetOldFine, 0x1000,"ZonaPeTextureWad");
	*pZonaPeTextureWad = OffsetNew;
}

// room editor
void PatchVetPtrObjectsItems(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pVetPtrObjectsItems;

	StrPatch VetPatch[] = {{0x41FD0A, true}};

	OffsetOldInizio = 0x46A400;
	OffsetOldFine=0x46AC00;
	TotPatchs=1;
	pVetPtrObjectsItems = (DWORD *) 0x45464E;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
			OffsetOldFine, 0x1000,"VetPtrObjectsItems");
	*pVetPtrObjectsItems = OffsetNew;
}

void Patch_AnimaTexture(void)
{
	int  i;
	BYTE VetBytes[]={0x66, 0xB8, 0x35, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xC3, 0x90};
	BYTE *pMem = (BYTE*) 0x425720;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}
// crea collegamento a nuova finestra trigger type
void Patch_FinestraTrigger(void)
{
	int  i;
	BYTE VetBytes[]={0x66, 0xB8, 0x40, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xC3, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42D5A0;

	for (i=0;i<16;i++) {
		pMem[i] = VetBytes[i];
	}
}


// altera i valori di accellerazione e cambio inquadratura
// per accellerare rotazioni
void Patch_FastRotate(void)
{
	int  i;
	BYTE VetBytes[]={0x66, 0x5, 0x0, 0x1, 0x66, 0x3D, 0x0, 0x12, 0x66, 0xA3, 
			0x50, 0x38, 0x58, 0x0, 0x7E, 0xA, 0x66, 0xB8, 0x0, 0x12, 
			0x66, 0xA3, 0x50, 0x38, 0x58, 0x0, 0x84, 0xD2, 0x74, 0x9, 
			0x66, 0x29, 0x5, 0x70, 0xF4, 0x56, 0x0, 0xEB, 0xB, 0x84, 
			0xC9, 0x74, 0x7, 0x66, 0x1, 0x5, 0x70, 0xF4, 0x56, 0x0, 
			0x8A, 0xD, 0x60, 0xE0, 0x46, 0x0, 0x84, 0xC9, 0x74, 0x9, 
			0x66, 0x29, 0x5, 0x6E, 0xF4, 0x56, 0x0, 0xEB, 0x11, 0x8A, 
			0xD, 0x68, 0xE0, 0x46, 0x0, 0x84, 0xC9, 0x74, 0x7, 0x66, 
			0x1, 0x5, 0x6E, 0xF4, 0x56, 0x0, 0x8A, 0xD, 0x61, 0xE0, 
			0x46, 0x0, 0x84, 0xC9, 0x74, 0x42, 0xF, 0xBF, 0xD, 0x60, 
			0xF4, 0x56, 0x0, 0xF, 0xBF, 0xD0, 0x2B, 0xCA, 0x81, 0xF9, 
			0x0, 0x2, 0x0, 0x0, 0x7D, 0x18, 0x66, 0xC7, 0x5, 0x60, 
			0xF4, 0x56, 0x0, 0x0, 0x2, 0xE8, 0x42, 0xC5, 0xFE, 0xFF, 
			0xE8, 0x3D, 0xC7, 0xFE, 0xFF, 0xE9, 0x26, 0x20, 0x2, 0x0, 
			0x66, 0x29, 0x5, 0x60, 0xF4, 0x56, 0x0, 0xE8, 0x2C, 0xC5, 
			0xFE, 0xFF, 0xE8, 0x27, 0xC7, 0xFE, 0xFF, 0xE9, 0x10, 0x20, 
			0x2, 0x0, 0x8A, 0xD, 0x69, 0xE0, 0x46, 0x0, 0x84, 0xC9, 
			0x74, 0x33, 0xF, 0xBF, 0xD, 0x60, 0xF4, 0x56, 0x0, 0xF, 
			0xBF, 0xD0, 0x3, 0xCA, 0x81, 0xF9, 0x0, 0xF0, 0x0, 0x0, 
			0x7E, 0x18, 0x66, 0xC7, 0x5, 0x60, 0xF4, 0x56, 0x0, 0x0, 
			0xF0, 0xE8, 0xF6, 0xC4, 0xFE, 0xFF};
	BYTE *pMem = (BYTE*) 0x414A3C;

	for (i=0;i<206;i++) {
		pMem[i] = VetBytes[i];
	}
}

// crea aggancio per chiamare nuova funziona di rotazione3d
void Patch_ChiamaRotazioni3d(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x25, 0x58, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4149E0;

	for (i=0;i<13;i++) {
		pMem[i] = VetBytes[i];
	}
}

void Patch_ChiamaSalvaMappa2d(void)
{
	int  i;
	BYTE VetBytes[]={0x83, 0xC4, 0x18, 0xFF, 0x15, 0x5C, 0x4F, 0x45, 0x0, 0x90};
	BYTE *pMem = (BYTE*) 0x42E96F;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_ErroriDirectX(void)
{
	int  i;
	BYTE VetBytes[]={0x8B, 0x4C, 0x24, 0x4, 0x66, 0xB8, 0x4B, 0x0, 0xFF, 0x15, 
			0x4, 0x4F, 0x45, 0x0, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4420C0;

	for (i=0;i<39;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_CreaErroreDirectX(void)
{
	int  i;
	BYTE VetBytes[]={0xB8, 0x7E, 0x1, 0x76, 0x88, 0x50, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x444D3F;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_CreaEccezioneInMostraSpot(void)
{
	int  i;
	BYTE VetBytes[]={0xBD, 0x0, 0x0, 0x0, 0x0, 0xC6, 0x45, 0x0, 0x1, 0x90, 
			0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x444F82;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_LinkCalcoloBreveIota(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x25, 0x68, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90};
	BYTE *pMem = (BYTE*) 0x442520;

	for (i=0;i<51;i++) {
		pMem[i] = VetBytes[i];
	}
}

void Patch_PerWriteFile(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x25, 0x80, 0x4F, 0x45, 0x0, 0x90};
	BYTE *pMem = (BYTE*) 0x44AD68;

	for (i=0;i<7;i++) {
		pMem[i] = VetBytes[i];
	}
}


void Patch_AttendiFineInputMouse(void)
{
	int  i;
	BYTE VetBytes[]={0x66, 0xB8, 0x53, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xC3};
	BYTE *pMem = (BYTE*) 0x4342E0;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}

void patch_PerOutTrigger1(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x25, 0x98, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4302FF;

	for (i=0;i<27;i++) {
		pMem[i] = VetBytes[i];
	}
}
void patch_PerOutTrigger2(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x15, 0x9C, 0x4F, 0x45, 0x0, 0x90};
	BYTE *pMem = (BYTE*) 0x4304F3;

	for (i=0;i<7;i++) {
		pMem[i] = VetBytes[i];
	}
}

void patch_PerOutTrigger3(void)
{
	int  i;
	BYTE VetBytes[]={0x66, 0xB8, 0x54, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x72, 0x30, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x430264;

	for (i=0;i<20;i++) {
		pMem[i] = VetBytes[i];
	}
}

// aggiunge item per essere visibile in room NRoom
// se pero' e' gia' presente item in quella stanza non lo aggiunge
void AggiungiAdItemVisibili(WORD ItemIndex, WORD NRoom)
{
	WORD *pTotZonaItemInStanza = (WORD*) 0x4BF2CE;
	StrRecordItemRoom *pZonaItemInStanza = (StrRecordItemRoom*) 0x7FE280;
	WORD i;
	WORD N;

	N=*pTotZonaItemInStanza;
	for (i=0;i<N;i++) {
		if (pZonaItemInStanza[i].ItemIndex == ItemIndex &&
			pZonaItemInStanza[i].NumeroStanza == NRoom) return;
	}

	// non e' ancora stato inserito
	pZonaItemInStanza[N].ItemIndex = ItemIndex;
	pZonaItemInStanza[N].NumeroStanza = NRoom;
	N++;
	*pTotZonaItemInStanza =N;
}

// controlla se MemTga fa parte di texture di Room o di object
bool IsRoomTga(DWORD MemTga)
{

	DWORD *Ptr_TextureRawImage = (DWORD*) 0x56C4F8;
	DWORD Differenza;

	if (MemTga < *Ptr_TextureRawImage) return false;

	Differenza = MemTga - *Ptr_TextureRawImage;

	if (Differenza < 0x400000) 
		return true;
	else
		return false;


}
// chiamata quando si deve copiare l'immagine all'interno del buffer di memoria
// usata dalla directsurfacedesc che conterra' la texture attuale
void CopiaTexture(BYTE * pSrc, WORD *pDest, StrColoriPalette* pTabellaColori, 
				  int SizeTextureX, int SizeTextureY, int TestInversa)
{

	int x,y;
	int Indice;
	int MulX,MulY;
	int DivX,DivY;
	int SizeX,SizeY;
	int OldY;
	bool TestBig;
	int* pN_TextureTails = (int*) 0x56C4F4;
	bool TestErrore;
	WORD Colore;
	BYTE Rosso,Verde,Blu;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	float IncX,IncY, xNow, yNow;
	int	NewY;
	bool TestNonRoom;

	pVetTailInfos = *p2VetTailInfos;
#ifdef LOG_TEXTURE 
	sprintf(BufferLog,"CopiaTexture: IndiceTailNow=%d   ImageTga=0x%X   SizeSrcX=0x%X  SizeSrcY=0x%X  TestInversa=%d", 
		IndiceTailNow, pSrc,SizeTextureX,SizeTextureY,TestInversa);

	InviaLog(BufferLog);
#endif

	// scoprirer indice di texture basandosi sul valore globale IndiceTailNow
	Indice=IndiceTailNow;
	TestBig=false;
	TestErrore=false;
	TestNonRoom=false;
	if (IsRoomTga((DWORD) pSrc)== false) {
#ifdef LOG_TEXTURE
		InviaLog("\tTga memory diversa da mappa tga per Room");
#endif
		Indice=-1;
		TestNonRoom=true;
	}

	
	if (Indice != -1) {

		if (pVetTailInfos[Indice].pMemImage != pSrc) {
			if (TestNonRoom==false) {
				// errore
				sprintf(BufferLog,"\tERROR TailIndex %d is not in according with image tga pointer 0x%X (Original from tail: 0x%X)",
					Indice,pSrc,pVetTailInfos[Indice].pMemImage);
				InviaLog(BufferLog);
			}
	
			TestErrore=true;

		}
	}

#ifdef LOG_TEXTURE
	if (Indice == -1 && TestNonRoom==false) {
		InviaLog("\tWARNING: Unknown TailInfoIndex");
	}
#endif
	


	if (Indice != -1 && TestErrore == false) {
		SizeX = VetBigTails[Indice].SizeX >> 8;
		SizeY = VetBigTails[Indice].SizeY >> 8 ;
		SizeX ++;
		SizeY ++;

		DivX = 1;
		DivY = 1;
		MulX = 1;
		MulY = 1;

		// ora calcolare il rapporto inteso come numero di righe ma solo
		// se texture e' di tipo maxi
		if (SizeX > 64 || SizeY > 64) {
			TestBig=true;
			IncX = (float) SizeX / 64;
			IncY = (float) SizeY / 64;
		}

	}

	if (TestBig==true) {
		if ((SizeX & 0x0f)!=0 || (SizeY & 0x0f)!=0) {

			InviaLog("\tWARNING: texture size uncorrect: disable big handling");

			TestBig=false;
		}
	}
	
	if ((MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) == 0) 
			TestBloccoTRY++;

	try {

			if (TestBig==true) {

#ifdef LOG_TEXTURE
				sprintf(BufferLog,"\tUse Big Texture: SizeX=%d  SizeY=%d",
					SizeX,SizeY);
				InviaLog(BufferLog);
#endif

				// calcolo per texture maxi

				yNow = 0;
				OldY = 0;
				for (y=0;y<64;y++) {
					xNow = 0;
					for (x=0;x<64;x++) {
															
						Indice = (BYTE) pSrc[int(xNow)];
						Rosso = pTabellaColori[Indice].Red /2;
						Verde = pTabellaColori[Indice].Green /2;
						Blu = pTabellaColori[Indice].Blue /2;

						// ogni colore deve avere solo 5 bits
						Colore = Rosso | Verde << 5 | Blu << 10;
						if (TestInversa !=0 || Indice != 0) Colore |= 0x8000;

						pDest[x] = Colore;
						xNow += IncX;


					}
					// qui bisogna stabilire se bisogna aumentare riga o no
					yNow += IncY;
					NewY = int(yNow);
					
					pSrc += 256 * (NewY - OldY);
					
					OldY = NewY;
					pDest += 64;

					
				}
				


			}
			else {
				

				// calcolo per texture normali
				for (y=0;y<64 && y<=SizeTextureY;y++) {
					for (x=0;x <64 && x <=SizeTextureX;x++) {
						Indice = (BYTE) pSrc[x];
						Rosso = pTabellaColori[Indice].Red /2;
						Verde = pTabellaColori[Indice].Green /2;
						Blu = pTabellaColori[Indice].Blue /2;

						// ogni colore deve avere solo 5 bits
						Colore = Rosso | Verde << 5 | Blu << 10;
						if (TestInversa !=0 || Indice != 0) Colore |= 0x8000;

						pDest[x] = Colore;

					}
					
					pSrc += 256;
					pDest += 64;
					
				}


			}

	
	}
	catch(...) 
	{

		InviaLog("\tERROR: EXECPTION IN [CopyImgTexture()]");

		
	}	
	// rimozione di controllo eccezioni
	if ((MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) == 0) 
				TestBloccoTRY--;
	
}

// aggiunge in VetTailInfos la nuova texture che attualmente
// e' puntatata da VetTailInfos[TotTailInfos]
// restituisce l'idnice di dove e' stata salvata
int AggiungiTailInfo(void)
{
	int* pN_TextureTails = (int*) 0x56C4F4;
	int i, TotTail;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;

	pVetTailInfos = *p2VetTailInfos;
	TotTail = *pN_TextureTails;
#ifdef LOG_TEXTURE
	sprintf(BufferLog,"AggiungiTailInfo(): N_TextureTails=%d (0x%X)  ImageTga=0x%X  SizeX=%X  SizeY=%X", 
		TotTail, TotTail, pVetTailInfos[TotTail].pMemImage, pVetTailInfos[TotTail].SizeX,
		pVetTailInfos[TotTail].SizeY );

	InviaLog(BufferLog);
#endif
	TotCambiBackup++;
	// confronta con dati in VetBigTails[] se e' gia' prsente o meno
	// e se le dimensioni sono maggiori di 64 salva la vera dinensione
	// in VetBigTails[] e mette in pVetTail una dimnensione di 64x64
	for (i=0;i< TotTail;i++) {
		if (pVetTailInfos[TotTail].pMemImage == VetBigTails[i].pMemImage && 
			pVetTailInfos[TotTail].SizeX == VetBigTails[i].SizeX &&
			pVetTailInfos[TotTail].SizeY == VetBigTails[i].SizeY ) {
#ifdef LOG_TEXTURE
			sprintf(BufferLog,"\tGia' presente, Indice=%d (0x%X)",
				i,i);
			InviaLog(BufferLog);
#endif
			
			return i;
		}
	}

	// aggiungerla
	if (TotTail >= MAX_TAIL_INFOS) {
		InviaLog("ERROR: Too many user defined textures");
		MostraMessageBoxNGLE("ERROR: Too many user defined textures",3);
		return 0;
	}

	// aggiungere veri dati in vetbigtails
	VetBigTails[TotTail].pMemImage = pVetTailInfos[TotTail].pMemImage ;
	VetBigTails[TotTail].SizeX = pVetTailInfos[TotTail].SizeX ;
	VetBigTails[TotTail].SizeY = pVetTailInfos[TotTail].SizeY;

	// ora se una delle dimensioni in pVetTailInfos e' maggiore di 64
	// sostituire entrambe le dimensioni con 64 (ossia 3fff
	if (pVetTailInfos[TotTail].SizeX > 0x3fff ||
		pVetTailInfos[TotTail].SizeY > 0x3fff) {
#ifdef LOG_TEXTURE
		sprintf(BufferLog,"\tSet in VetTailInfos[%d] both size to 0x3fff 0x3fff",
			TotTail);
		InviaLog(BufferLog);
#endif
		pVetTailInfos[TotTail].SizeX = 0x3fff;
		pVetTailInfos[TotTail].SizeY = 0x3fff;
	}

	// aumentare tottail
	*pN_TextureTails = TotTail+1;
	return TotTail;		


}


// Chiamata durante caricamento project
// dopo che sono gia' state caricate tailinfo originali in pVetTailInfos[]
// e convertiti i valori di puntatori a immagine
// legge tailinfo e converte i dati salvando le dimensioni reali
// in VetBigTails[] e mettendo in pVetTailInfos[] dimensioni a 64 x 64
// se la dimensione era maggiore di 64

void CaricaPrjTailInfos(void)
{

	int* pN_TextureTails = (int*) 0x56C4F4;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	int i;

	pVetTailInfos = *p2VetTailInfos;



	// ora copiare in pVetTailInfos[] originale, tutti gli elementi
	// trasformando pero' le dimensioni in 64x64 quando sono maggiori di 64
	for (i=0;i<*pN_TextureTails;i++) {

		VetBigTails[i].pMemImage = pVetTailInfos[i].pMemImage;
		VetBigTails[i].SizeX = pVetTailInfos[i].SizeX ;
		VetBigTails[i].SizeY = pVetTailInfos[i].SizeY ;


		if (pVetTailInfos[i].SizeX > 0x3fff ||
			pVetTailInfos[i].SizeY > 0x3fff) {

			// forzare dimensione a 64 x 64
			pVetTailInfos[i].SizeX = 0x3fff;
			pVetTailInfos[i].SizeY = 0x3fff;
		}
	}

}


int TrovaTipoStrutturaTexFaces(short *pVetTexFaces)
{
	// sulla base delle prima due word di struttura TexFaces ricava l'indice (di tipo long)
	// da usare per accedere a campo memtga.
	// se la struttura e' da ignorare restituisce -1
	// eventuali messaggi d'errore vengono inviati da questa funzione

	switch (pVetTexFaces[0]){
	case 0x4000:
	case 9:
	case 0:
	case 2:
	case 1:
	case -1:
		// va ignorata
#ifdef LOG_TEXTURE
		sprintf(BufferLog,"\tIgnored: Word1=0x%X",pVetTexFaces[0]);
		InviaLog(BufferLog);
#endif
		return -1;

		
	case 4:
		//se seconda word e' = 3 provare ad usare record 0x1c
		if (pVetTexFaces[1] == 3) return 0x1c / 4;
	case 7:
		// record 24
		return 0x24 / 4;
	case 5:
	case 8:
		return 0x2c / 4;
	default:
		// controllare word secondaria

		// analizzare seconda word
		switch (pVetTexFaces[1]) {
		case 3:
			return 0x24 /4;
		case 4:
			return 0x2c / 4;
		default:

			sprintf(BufferLog,"\tERROR: unknown value of secondary word id in TexFaces structure 0x%X (Word1=%d  Word2=%d)",
				pVetTexFaces,pVetTexFaces[0],pVetTexFaces[1]);
			InviaLog(BufferLog);

			return -1;
		}

	}
	return -1;

}
// salvare indice tail e corrispettivo AdTexFace in vettori globali

// DWORD VetAdrTexFace[10000];
// WORD  VetIndexTail[10000];  //  indice tail
// int  TotAdrTexFace;		// numero di strutture salvate

void SalvaIndiceTexFaces(int AdrTexFacesNow, int IndiceTail, 
						 int MemTga,DWORD OffsetCodice)
{
	int *pNTextureTails = (int*) 0x56C4F4;

	DWORD *pTestPreview = (DWORD*) 0x583BF8;
	int **p2VetAnimatedTiles = (int**) 0x454646;
	int *VetAnimatedTiles;

	VetAnimatedTiles = *p2VetAnimatedTiles;
#ifdef LOG_TEXTURE
	WORD  *pWord;
	sprintf(BufferLog,"SalvaIndiceTexFaces: AdrTexFacesNow=0x%X   IndiceTail=%d (0x%X)  MemTga=0x%X  Offset=0x%X",
			AdrTexFacesNow,IndiceTail,IndiceTail,MemTga,OffsetCodice);
	InviaLog(BufferLog);

	pWord = (WORD *) AdrTexFacesNow;
	sprintf(BufferLog,"\tWord1=0x%X  Word2=0x%X", pWord[0],pWord[1]);
	InviaLog(BufferLog);


#endif
	if (IsRoomTga(MemTga)==false) {
		// ignorare indice tail attuale
#ifdef LOG_TEXTURE
		InviaLog("\tIgnore because ptrTextureTail is not for room tail infos");
#endif
		
		return;
	}

	if (IndiceTail >= MAX_TAIL_INFOS) {
		sprintf(BufferLog,"\tWARNING: IndiceTail too high (greater than %d",
			MAX_TAIL_INFOS);
		InviaLog(BufferLog);
		return;
	}


	if (IndiceTail >= *pNTextureTails) {
		sprintf(BufferLog, "ERROR: IndiceTailInfo (%d) exced value of Number of Tails (%d)",
			IndiceTail, *pNTextureTails);
		InviaLog(BufferLog);
		return;
	}

	if ((int) VetBigTails[IndiceTail].pMemImage != MemTga ) {
		// ignorarlo a meno che non siamo in preview e indicetail corrisponde
		// ad una texture animata
		if (*pTestPreview == 0 || VetAnimatedTiles[IndiceTail] == -1) {

			sprintf(BufferLog,"\tWARNING: Ignore because TgaPointer=0x%X is different than TgaPointer in VetBigTails[%d]=0x%X",
				MemTga,IndiceTail,VetBigTails[IndiceTail].pMemImage);
			InviaLog(BufferLog);

			return;
		}
	}

	// per ora fare il controllo in modo semplice, se poi funziona
	// provare con codice ottimizzato
	if (TotAdrTexFace >= 10000) {
		InviaLog("ERROR: reached max index in VetAdrTexFaces");
		return;
	}
	VetAdrTexFace[TotAdrTexFace] = AdrTexFacesNow;
	VetIndexTail[TotAdrTexFace] = IndiceTail;
#ifdef LOG_TEXTURE
	sprintf(BufferLog,"\tSTORE: VetAdrTexFace[%d]=0x%X",
		TotAdrTexFace,IndiceTail);
	InviaLog(BufferLog);
#endif

	TotAdrTexFace++;
#ifdef LOG_TEXTURE
	sprintf(BufferLog,"TotAdrTexFaces=%d", TotAdrTexFace);
	InviaLog(BufferLog);
#endif



}
// controlla se attuale valore puntato da pVetTailInfos corrisponde
// a tail di stanza (Ptr_MemoriaRoomTexInfos) o di oggetti wad (Ptr_ObjectTextureWad)
// NOTA: se MemTga e' diverso da zero usa questo valore per verificare


bool IsRoomTail(void)
{
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	DWORD* Ptr_MemoriaRoomTexInfos = (DWORD*) 0x56C4E4;
	DWORD BaseMemTailInfo;
	
	pVetTailInfos = *p2VetTailInfos;
	
	BaseMemTailInfo = (DWORD) pVetTailInfos;

	if (BaseMemTailInfo != *Ptr_MemoriaRoomTexInfos) return false;
	
	return true;

}


// chiamata da funzione SetIndiceTail, 
// Verifica se indirizzo di struttura AdrTexFacesNow e' uno di quelli
// memorizzati. se lo e' imposta IndiceTailNow con valore associato a struttura
// se non c'e' imposta indicetailnow = -1
void ImpostaIndiceTailNow(DWORD AdrTexFacesNow)
{

	int i;
	int *pNTextureTails = (int*) 0x56C4F4;



	if (TotAdrTexFace == 0) return;

	if (IsRoomTail()==false) return;


#ifdef LOG_TEXTURE
	sprintf(BufferLog,"ImpostaIndiceTailNow() for record TexFaces: 0x%X  (Record stored=%d)",
			AdrTexFacesNow, TotAdrTexFace);
	InviaLog(BufferLog);
#endif
	// provare ad accellerare
	// vecchio codice
	
	for (i=0;i<TotAdrTexFace;i++) {


		if (abs(VetAdrTexFace[i] - AdrTexFacesNow) < 0x10) {
			IndiceTailNow = VetIndexTail[i];
#ifdef LOG_TEXTURE
			sprintf(BufferLog,"\tFound IndexTail=0x%X  in position VetAdrTexFace[%d]=0x%X",
				IndiceTailNow,i, VetAdrTexFace[i]);
			InviaLog(BufferLog);
#endif
			return;
		}
	}

#ifdef LOG_TEXTURE
	InviaLog("\tNot found");
#endif


}

// Chiamata da ApplicaTutteTexture nel loopDatiPoligoni per impostare il 
// numero di indice 
// texture IndiceTailNow
// cui fa riferimento l'attuale record memoria TexFaces.
// il valore di indice (globale) sara' poi usato da funzione copiatexture
__declspec(naked) void SetIndiceTail(void)
{
	__asm {
			mov  esi, [ebx]   ;mette in esi puntatore a recordo memoria TexFaces

			pushad
			;struttura esi e' esattamente l'inizio della stuttura memoria TexFaces

			push esi   ;record struttura TexFaces attuale


			
			lea  eax, ImpostaIndiceTailNow
			call eax
			add  esp,4


			popad


			mov  ax, [esi]
			add  esi, 02h
			retn

	}
}

void ErroreRecordTexFaces(DWORD RetAdr)
{
	char BufferTemp[256];

	sprintf(BufferTemp, "ERROR: cann't recognize return address 0x%X", RetAdr);
	InviaLog(BufferTemp);

}



// chiamata da diversi punti del programma quando si salva in memoriaTexFaces
// la memoria tga della tailinfo attuale
// a seconda dell'indirizzo di ritorno nello stack esegue diverse analisi
__declspec(naked) void SalvaRecordTexFaces(void)
{

	__asm {
		cmp dword ptr [esp], 4038cdh
		jnz		Salto2

		;togliere 0x1c
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
	// prima era eliminato
	// sembra che riguardi triangolo (un triangolo rialzato da terra a piramide
	// assieme ad altri)

	
		pushad
		push	4038cdh	;offset patch
		mov		ebx, [edx][ecx*8]
		push	ebx		;memtga
		push	ecx		;indice tail
		mov		eax, dword ptr [eax]
		sub		eax, 1ch

		push	eax		;indirizzo struttura

		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h
	
		popad

		mov  eax, dword ptr [eax]
		add  eax, 04h
		mov  edx, [edx][ecx*8]
		mov  [eax-04h], edx
		retn
Salto2:
		cmp dword ptr [esp], 43d190h
		jnz Salto3
		;record 0x1c
		;in eax  = indice tail



		mov ecx, 506A78h    ;Ptr_TextureTailInfos
		mov  ecx, dword ptr [ecx]

		mov  edx, [ecx][eax*8]
		// prima era eliminato

		pushad
		push	43d190h  ;codice patch
		push	edx		;mem tga
		push	eax		;indice tail
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		push	eax		;record texface
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h
		popad

		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  [eax+1ch], edx
		retn
Salto3:
		cmp  dword ptr [esp], 43d61eh
		jnz   Salto4
		;record 0x1c
		;indice in esi ma come valore gia moltiplicato per 8
		mov eax, 506A78h    ;Ptr_TextureTailInfos
		mov  eax, dword ptr [eax]
		mov edx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  edx, dword ptr [edx]			
		// prima era eliminato

		pushad
		push	43d61eh
		mov  ecx, [esi][eax]
		push	ecx		;memtga
		shr		esi,3	;divide per 8
		push	esi		;indicetail
		push	edx		;record textfac
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h
		popad

		mov  ecx, [esi][eax]
		mov  [edx+1ch], ecx
		retn
Salto4:
		cmp	dword ptr [esp], 43d795h
		jnz		Salto5
		;togliere 0x1c
		;indicetail in esi ma gia' moltiplicato per 8
		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  ecx, [esi][edx]
		// prima era eliminato

		pushad
		push	43d795h
		push	ecx		;memtga
		shr		esi,3
		push	esi		;indicetail
		sub		eax,1ch
		push	eax		;struttura texface
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h
		popad

		mov  [eax], ecx
		retn
Salto5:
		cmp		dword ptr [esp], 43DB78h
		jnz		Salto6
		;togliere 0x24
		;IndiceTail in eax ma gia' moltiplicato per 8

		mov  ecx, [eax][edx]   ;edx=puntatore memoria tga
		mov edx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  edx, dword ptr [edx]
		mov  [edx+24h], ecx

		pushad
		push	43DB78h
		push	ecx
		shr		eax,3
		push	eax		;indicetail
		push	edx		;adrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h

		popad
		retn
Salto6:
		cmp		dword ptr [esp], 43DF72h
		jnz		Salto7
		;24
		;indicetail in ebp gia' moltiplicato per 8
		mov eax, 506A78h    ;Ptr_TextureTailInfos
		mov  eax, dword ptr [eax]
		mov  edx, [eax][ebp]
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  [eax+24h], edx	

		pushad
		push	43DF72h
		push	edx
		shr		ebp,3
		push	ebp		;IndiceTail
		push	eax		;AdrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad

		retn
Salto7:
		cmp		dword ptr [esp], 43E103h
		jnz		Salto8
		;togliere 0x24
		;indice tail in ebp gia' moltiplicato per 8

		mov ecx, 506A78h    ;Ptr_TextureTailInfos
		mov  ecx, dword ptr [ecx]

		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]

		mov  edx, [ecx][ebp]
		;qui c'era 		mov  ecx, [esp+48h]
		
		mov  [eax], edx
		pushad
		push	43E103h
		push	edx
		shr		ebp,3
		push	ebp		;indicetail
		sub		eax, 24h
		push	eax		;adrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad

		retn
Salto8:
		cmp		dword ptr [esp], 43E9B2h
		jnz		Salto9
		;22
		;indice tail in ecx

		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov  eax, [edx][ecx*8]
		// prima era eliminato

		pushad
		push	43E9B2h
		push	eax		;memtga
		push	ecx		;indicetail
		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		push	ecx		;struttura tex face
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad

		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		mov  [ecx+22h], eax
		retn
Salto9:
		cmp		dword ptr [esp], 43EF3Ch
		jnz		Salto10
		;22
		;indice tail in ecx

		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov  eax, [edx][ecx*8]
		// prima era eliminato

		pushad

		push	43EF3Ch
		push	eax		;memtga
		push	ecx		;indicetail
		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		push	ecx		;struttura texface
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad

		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		mov  [ecx+22h], eax
		retn
Salto10:
		cmp		dword ptr [esp], 43F095h
		jnz		Salto11
		;togliere 0x2C
		;indicetail ecx



		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		
		mov  ecx, [edx][ecx*8]
		mov  [eax], ecx


		pushad

		push	43F095h
		push	0
		push	ecx		;indicetail
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		sub		eax, 2Ch
		push	eax		;AdrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h

		popad

		retn
Salto11:
		cmp		dword ptr [esp], 43F57Fh
		jnz		Salto12
		;2c
		;ecx = indicetail moltiplicato per 8
		mov  edx, [ecx][eax]
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  [eax+2ch], edx	
		// prima era eliminato

		pushad
		push	43F57Fh
		push	0
		shr		ecx,3
		push	ecx		;indicetail
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		push	eax		;AdrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp, 10h
		popad


		retn

Salto12:
		cmp		dword ptr [esp], 43FA22h
		jnz		Salto13
		;togliere 0x2c
		;ecx = indicetail gia' moltiplicato per 8

		mov		dword ptr [offset SwapDword], ecx

		mov  edx, [ecx][edx]
		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		mov  [ecx+2ch], edx	

		pushad	
		mov		ecx, dword ptr [offset SwapDword]
		push	43FA22h
		mov  ebp, [ecx][edx]
		push	edx  ;memtga
		shr		ecx,3
		push	ecx		;indicetail
		mov ecx, 506A70h    ;Ptr_MemoriaTexFaces
		mov  ecx, dword ptr [ecx]
		push	ecx		;AdrTexFaces
		lea		eax,SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad

	
		retn
Salto13:
		cmp		dword ptr [esp], 43FBA6h
		jnz		GestioneErrore
		;togliere 2c
		;edx = indicetail gia' moltiplicato per 8
		mov ecx, 506A78h    ;Ptr_TextureTailInfos
		mov  ecx, dword ptr [ecx]
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  ecx, [edx][ecx]
		mov  [eax], ecx		


		pushad
		push	43FBA6h
		push	ecx	;memoriatga
		shr		edx, 3
		push	edx		;IndiceTail
		sub		eax, 2Ch
		push	eax		;AdrTexFaces
		lea		eax, SalvaIndiceTexFaces
		call	eax
		add		esp,10h
		popad
	
		retn
		;non e' possibile che si arrivi fin qui
GestioneErrore:
		mov		eax, dword ptr [esp]
		push	eax
		lea		eax, ErroreRecordTexFaces
		call	eax
		add		esp,4
		retn



	}
}

void ShowMexCheckIndex(int IndiceTexture)
{
	char BufferTemp[256];

	DWORD * pTotMemoriaTextureDD = (DWORD*) 0x493128;

	sprintf(BufferTemp,"Controlla TextureDD: VetIndexTextureDD[%d]=%d  (IndiceTailNow=%d  TotTexDD=%d)", 
				IndiceTexture, VetIndexTextureDD[IndiceTexture], IndiceTailNow, 
				*pTotMemoriaTextureDD);

	InviaLog(BufferTemp);
	

}

void ShowMexRestTexFaces(void)
{
	InviaLog("=========== RESET TexFaces MEMORY ==========");

}
// Store IndexArray[IndiceTail] = IndexTexDD
void ShowMexStoreIndex(int IndiceTextureDD)
{
	char BufferTemp[256];

	sprintf(BufferTemp,"STORE INDEX: VetIndexTextureDD[%d]=%d (IndiceTailNow)",
				IndiceTextureDD, IndiceTailNow);
	InviaLog(BufferTemp);

}

void RuotaOggettoSelezionato(void)
{
	DWORD *pIndiceItemSelezionato = (DWORD *) 0x583BA4;
	WORD Orientation;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	if (*pIndiceItemSelezionato == -1) {
		MostraMessageBoxNGLE("ERROR: no object selected",3);
		return;
	}
	Orientation = pVetBaseOggetti[*pIndiceItemSelezionato].ObjFacing ;

	Orientation += 0x2000;

	pVetBaseOggetti[*pIndiceItemSelezionato].ObjFacing = Orientation;

	__asm {
		pushad

		mov		eax, 401200h ;
		call	eax

		popad
	}
	AddInOldRoomList(-1,-1);
	TotCambiBackup++;

}

void MostraLoadObj(void)
{
	MostraMessageBoxNGLE("Object data loaded OK",7);	
}

void CaricaUltimoWad(void)
{


	char BufferTemp[512];
	char *pNomeWad = (char*) 0x56C7A0;

	sprintf(BufferTemp,"Do you want reload current Object Wad File?\r\n\r\n%s", 
			NomeFileWad);
	if (MostraMessageBoxNGLE(BufferTemp,0x0f) != 0) {
		strcpy(pNomeWad,NomeFileWad);

		__asm {
			pushad

			push	0  
			mov		eax, 420453h ;Sub_CaricaFileWad
			call	eax
			add		esp,4
			mov eax, 401200h    ;CicloPrincipale
			call eax			
		
			lea		eax, MostraLoadObj
			call	eax

			popad
			
		}
	}



}

__declspec(naked) int CaricaNuovoWad(void)
{
	__asm {
		pushad
		
		push	0
		mov		eax, 420420h; Menu_LoadObjects
		call	eax
		add		esp,4
		mov eax, 401200h    ;CicloPrincipale
		call eax
		lea		eax, MostraLoadObj
		call	eax
		popad
		
		retn
		
	}


}
// muove lara da posizione dove si trova (se c'e') 
// a stanza attuale
void MuoviLara(void)
{
	int *pIndiceItemSelezionato = (int *) 0x583BA4;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	int  *pNObjectsCode = (int*) 0x56C970;
	int  Indice;
	int *pIndiceSlotSelezionato = (int*) 0x583BA8;
	int RoomOggetto;
	int *pTestFlipMap = (int *) 0x583BF0;
	int SalvaRoom;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int PosX,PosY;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	WORD *VetTrovati;
	int TotTrovati;

	pVetRooms = *p2VetRooms;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	if (NomeFileWad[0]==0) {
		MostraMessageBoxNGLE("Missing Lara object. Not yet loaded wad file",3);
		return;
	}

	if (*pTestFlipMap) {
		MostraMessageBoxNGLE("Cann't place non static objects in flipped room",3);
		return;
	}

	if ((*pTestEsisteSelezioneBox)==0) {
		MostraMessageBoxNGLE("ERROR: no selected sector.\r\nBefore clicking this button you should select the sector where you want lara will be moved.",3);
		return;
	}

	PosX = pRectSelezione2d->left;
	PosY = pRectSelezione2d->top;

	PosX -= pVetRooms[*pIndiceRoomAttuale].XDisplayOffset;
	PosY -= pVetRooms[*pIndiceRoomAttuale].ZDisaplyOffset;

	
	Indice=-1;
	VetTrovati = TrovaOggetti(0, &TotTrovati);
	if (TotTrovati > 0) { 
		Indice = VetTrovati[0];
		RoomOggetto = pVetBaseOggetti[Indice].IndexRoom;
	}


	if (Indice != -1) {
		// eliminare la vecchia lara
		*pIndiceItemSelezionato = Indice;
		SalvaRoom = *pIndiceRoomAttuale;
		__asm {
			pushad
			lea eax, RoomOggetto    ;IndiceRoomAttuale
			mov  eax, dword ptr [eax]	
			mov ecx, 583BA4h    ;IndiceItemSelezionato
			mov  cx, word ptr [ecx]
			push eax
			push ecx
			mov eax, 423C60h    ;ToglieOggettoDaIndiciArray
			call eax
			add		esp,8
			popad

		}
		*pIndiceRoomAttuale = SalvaRoom;

	}


	// ora predisporre per piazza oggetto
	*pIndiceSlotSelezionato=0;
	*pIndiceItemSelezionato =-1;
	__asm {
		pushad
		xor		ebx,ebx
		push 01h  ;sizey
		push 01h  ;sizex
		lea		eax, PosY
		push	dword ptr [eax]
		
		lea		eax, PosX
		push	dword ptr [eax]

		mov		eax, 583BA0h ;IndiceRoomAttuale
		mov		eax, dword ptr [eax]
		push	eax
		push 08h	;tipo oggetto
		mov eax, 423860h    ;AggiungiOggettoInRoom
		call eax
		add		esp, 18h
		movsx  esi, ax
		xor  edx, edx

		lea  eax, dword ptr [00h][esi*8]
		sub  eax, esi
		lea  edi, [eax][eax*2]
		mov eax, 46B814h    ;Ptr_VetBaseOggetti
		mov  eax, dword ptr [eax]
		add  edi, edi
		add  eax, edi
		mov  [eax+0ah], bx
		mov ecx, 583BA8h    ;IndiceSlotOggSelezionato
		mov  cx, word ptr [ecx]
		mov  [eax+0eh], cx
		mov  word ptr [eax+26h], 4210h
		mov  [eax+28h], bx
		mov  [eax+24h], bx
		mov  [eax+22h], bx
		mov  [eax+20h], bx
		mov  word ptr [eax+12h], 0  ;0 = pavimento 1= soffitto
		mov eax, 422F20h    ;LOC_422f20
		push	esi  ;argomnento: indice item selezionato
		call eax
		add	esp,4

		mov eax, 583BA4h   ;IndiceItemSelezionato
		mov  dword ptr [eax], esi

		/*
		mov eax, 46B814h    ;Ptr_VetBaseOggetti
		mov  eax, dword ptr [eax]
		*/
		mov eax, 583BB6h   ;TipoFrecciaSelezione
		mov  dword ptr [eax], ebx
		
		/*
		mov edx, 46B818h    ;Ptr_VetSlotOggetti
		mov  edx, dword ptr [edx]
		movsx  eax, word ptr [edi+0eh][eax]
		mov  ecx, eax

		shl  ecx, 04h
		add  ecx, eax
		
		mov  ax, [edx+04h][ecx*8]   ;qui avviene il crash
		and  eax, 100h
		test ax, ax
		jnz  Salto1
		*/

		mov eax, 583BB2h   ;DatiTriggerSelezionato + 06h
		mov  word ptr [eax], bx
		
		
		mov eax, 583BB0h   ;DatiTriggerSelezionato + 04h
		mov  word ptr [eax], bx
		
		
		mov eax, 583BACh   ;DatiTriggerSelezionato
		mov  word ptr [eax], bx
		
		mov eax, 583BB4h   ;DatiTriggerSelezionato + 08h
		mov  word ptr [eax], bx
		
		mov eax, 583BAEh   ;DatiTriggerSelezionato + 02h
		mov  word ptr [eax], si

		mov eax, 413770h    ;MostraTriggerInZonaTesto
		call eax
		/*
	Salto1:
		*/
		mov eax, 401200h    ;CicloPrincipale
		call eax		
		popad
		

	}

}

// chiamata in fase di chiusura. verifica se cancellare autosave.prj

void EliminaAutoSave(void)
{
	if (MyPrefRoom.Flags & FPR_REMOVE_AUTOSAVE) {
		__asm {
			push  45A77Ch	;  ASCII "autosave.prj"
			mov eax, 44A4AFh    ;EliminaFile
			call eax

			add	esp,4
		}
	}


}

__declspec(naked) void Attiva2dMap(void)
{
	__asm {
		pushad
		mov eax, 583BD8h   ;TipoModeView
		cmp dword ptr [eax], 1
		jz  GiaFatto

	mov eax, 583BD8h   ;TipoModeView
	mov  dword ptr [eax], 00h 

	mov eax, 461B98h   ;NumeroFinestraAttiva
	mov  dword ptr [eax], 01h 

	mov eax, 401200h    ;Draw3DWindow
	call  eax 
		/*
		push 46CE1Ch  //  DatiInfoMouse
		mov eax, 4260F0h // Sub_Pulsante_2D_Map
		call eax
		add esp,4
		*/

GiaFatto:
		popad
		retn
	}

}
// incrementa parte numerica di nome progetto.
// se non c'e' parte numerica la crea adesso
char *CreaNomeProgressivoPrj(char *pNomeIniziale)
{
	static char NuovoNome[2048];
	DWORD i;
	char *pDirSrc;
	char *pParteNome;
	char *pInizio;
	char *pTemp;

	if (EsisteFile(pNomeIniziale)==false) return pNomeIniziale;

	pDirSrc = SoloDir(pNomeIniziale);
	pParteNome = SoloNome(pNomeIniziale);
	
	// analisi per vedere se questo nome e' gia' un nome di bakcup
	// che inizia con "BK"
	pInizio = pParteNome;

	pTemp = Search(pInizio,"BK",ts_Avanti,NULL);
	if (pTemp == pInizio) {
		pTemp += 2;    //saltare i due caratteri
		// vedere se ora ci sono solo caratteri numericii fino a _
		

		for (i = 0;i<strlen(pTemp);i++) {
			if (pTemp[i] == '_') {
				// si, quindi usare punto successivo come nome
				pInizio = pTemp + i + 1;
				break;
			}
			if (pTemp[i] < '0' || pTemp[i] > '9') break;

		}

	}

	// ok, ora provare con tutti i numeri possibili finche si trova un file assente

	i = 1;

	do {
		sprintf(NuovoNome,"%s\\BK%d_%s", pDirSrc, i, pInizio);
		i++;
	} while (EsisteFile(NuovoNome)==true);

	return NuovoNome;


}
// fa output wad, converte file .tom e chiama tomb4
// usare come base NomeFileWad
void GiocaLivello(bool TestClose)
{

	char NomeTom[256];
	char NomeExe[256];
	char *pTemp;
	HANDLE HandleProcesso;
	int *pTestNienteMessaggi = (int*) 0x4697E4;
	int SalvaTest;
	int Result;
	HWND *pMiaWind = (HWND*) 0x46E559;
	char NomeTr4[256];
	DWORD	Esito;
	char *pNomeProject = (char*) 0x46B858;  // BufferNomeProject
	char SalvaTom[256];
	char BufferInterno[2048];
	char *pMexSave;
	char MiniBuf[200];
	static int *pIndiceRoomAttuale = (int*) 0x583BA0;

	if (EsisteFile(NomeFileWad)==false) {
		MostraMessageBoxNGLE("Not yet loaded any .wad file",3);
		return;
	}
	LastRoomPlay = *pIndiceRoomAttuale;

	// usare nome di prohect attuale oppure progressivo
	if (MyPrefRoom.Flags & FPR_SAVE_PLAY_PROGRESSIVE) {
		pTemp = CreaNomeProgressivoPrj(NomeLoadProject);
	}else {
		pTemp = NomeLoadProject;
	}

	if (MyPrefRoom.Flags & FPR_SAVE_PLAY_PROGRESSIVE) {
		sprintf(MiniBuf, "assigning new name: %s", SoloNome(pTemp));
		pMexSave=MiniBuf;

	}else {
		pMexSave = "same name";
	}
	if (TestClose==true) {
		sprintf(BufferInterno, "It will be performed following operations:\r\n\r\n1) Perform saving of current project (%s)\r\n2) Output wad\r\n3) Converting .tom -> tr4\r\n4) Starting tomb4.exe\r\n5) Close Room Editor program",
			pMexSave);

	}else
	{
		sprintf(BufferInterno, "It will be performed following operations:\r\n\r\n1) Perform saving of current project (same name)\r\n2) Output wad\r\n3) Converting .tom -> tr4\r\n4) Starting tomb4.exe\r\n5) Minimize NGLE",
			pMexSave);


	}
	Result = MessageBox(*pMiaWind,BufferInterno,"Play Level (Batch Procedure)",
				MB_OKCANCEL);

	if (Result == IDCANCEL) return;

	if (TestClose ==false) {
		Attiva2dMap();
		Sleep(1000);
	}



	if (SalvaProgetto(pTemp)==0) {
		sprintf(BufferLog,"ERROR trying to save project: %s",pTemp);
		ShowInfoRoom(BufferLog);
		InviaLog(BufferLog);
		if (MostraMessageBoxNGLE(BufferLog,7)==0) return;
		
	}else
	{
		sprintf(BufferLog,"Saved project: %s",pTemp);
		InviaLog(BufferLog);
		ShowInfoRoom(BufferLog);
		strcpy(NomeLoadProject, pTemp);

	}
	// aggiornare titolo finestra
	AggiornaTitoloFinestra(NomeLoadProject, *pMiaWind, false);	


	sprintf(NomeTom,"%s\\%s.tom",
			SoloDir(NomeFileWad),SoloNomeSenzaExt(NomeFileWad));
	pTemp = TrovaNomeCorto(NomeTom);
	

	// ripristinare nome finale lungo
	sprintf(NomeTom,"%s\\%s.tom", SoloDir(pTemp), SoloNomeSenzaExt(NomeFileWad));
	strcpy(SalvaTom, NomeTom);

	DeleteFile(NomeTom);
	SalvaTest = *pTestNienteMessaggi;
	*pTestNienteMessaggi = 1;
	ShowInfoRoom("Output Wad in progress...");
	roomCreaFileTom(NomeTom);


	*pTestNienteMessaggi = SalvaTest;

	if (EsisteFile(SalvaTom)==false) {
		MostraMessageBoxNGLE("Some error creating .tom file",3);
		return;
	}
	sprintf(NomeTr4,"data\\%s.tr4", SoloNomeSenzaExt(SalvaTom));

	DeleteFile(NomeTr4);

	sprintf(NomeExe,"%s\\ng_tom2pc.exe", Dir_Trle);
	if (EsisteFile(NomeExe)==false) {
		sprintf(NomeExe,"%s\\tom2pc.exe", Dir_Trle);
		if (EsisteFile(NomeExe)==false) {
			MostraMessageBoxNGLE("ERROR: cann't find program 'ng_tom2pc.exe' or 'tom2pc.exe' in folder %d\r\nCann't convert .tom file in .tr4 format",3);
			return;
		}
	}


	ShowInfoRoom("Converting Tom -> Tr4 in progress...");

	if (CreaProcesso(NomeExe,SalvaTom,&HandleProcesso,false) == -1) {
		sprintf(BufferLog,"ERROR performing: %s",SoloNome(NomeExe));
		MostraMessageBoxNGLE(BufferLog,3);
		return;
	}

	// attendere che abbia finito il processo
	do {
		Esito=WaitForSingleObject(HandleProcesso,500);
	}while (Esito  == WAIT_TIMEOUT);

	ShowInfoRoom("Start Tomb4.exe");

	if (TestClose==false) {

		// minimizzare winroomedit
		ShowWindow(*pMiaWind,SW_MINIMIZE);

		sprintf(NomeExe,"%s\\tomb4.exe",Dir_Trle);
		
		CreaProcesso(NomeExe,"",&HandleProcesso,false);
		TestPlayInCorso=true;
	}


	if (TestClose == true) {

		__asm {
			/*
			lea		eax, EliminaAutoSave
			call	eax

			mov eax, 563F58h    ;BaseSchermo
			mov  eax, dword ptr [eax]
			push eax	; |Arg1 => 00A55DA0
			mov eax, 43657Eh    ;RilasciaGlobaliStrutturaInfo
			call eax
			add  esp, 04h
			mov eax, 4379C9h    ;SalvaIndirizzoRitorno
			call eax
			mov eax, 444570h    ;RilasciaRisorse
			call eax
			push 01h        ;qui mettere 1 in modo che si capisca che in questo caso
					;e' un uscita normale
		*/
		//	mov eax, 44A3BCh    ;TerminaProgramma
		//	call eax
			mov byte ptr [TestAvviaTomb], 1
			mov eax, dword ptr [MyPrefRoom.Flags]
			test eax, FPR_REMOVE_AUTOSAVE
			jz   salta
			push  45A77Ch	;  ASCII "autosave.prj" 
			mov eax, 44A4AFh    ;remove
			call eax 
			add esp, 04h
salta:
			mov eax, 563F58h    ;BaseSchermo
			mov  eax, dword ptr [eax] 
			push eax	; |Arg1 => 00A55DA0 
			mov eax, 43657Eh    ;close_scrn
			call eax 
			add  esp, 04h 
			mov eax, 4379C9h    ;rem_sys
			call eax 
			mov eax, 444570h    ;WinClose
			call eax 
			push 01h        ;qui mettere 1 in modo che si capisca che in questo caso 
					;e' un uscita normale 
			mov eax, 44A3BCh    ;exit
			call eax      ;TerminaProgramma 
Salto1: 
			jmp salto1


			
		}

	}

}


// visualizza informazioni su livello attuale
void Menu_InfoAboutProject(void *pPunta)
{
	char *pNomeFileTga = (char*) 0x46BA78;

	sprintf(BufferLog,"WAD: %s\r\nTGA: %s\r\n", 
			NomeFileWad, 
			pNomeFileTga);
	MostraMessageBoxNGLE(BufferLog,7);



}
void MessaggioTroppoCorto(int MinSizeDirezione)
{
	sprintf(BufferLog,"ERROR: too short selection (in white arrows direction) for current shape effect. (Minimum required lenght=%d)", 
			MinSizeDirezione);

	MostraMessageBoxNGLE(BufferLog,3);
}
// imposta in DatiShape i valori di inizio e fine striscia cosi' come l'incremnto
// basandosi su miaselezione e DirezioneFreccia
// se la direzione della freccia e' lungo un lato che e' piu' corto di MinSizeDirezione
// visualizza messaggio d'errore e restituuisce false 
bool CalcolaDirezioneShape(int MinSizeDirezione)
{
	int  n;
	if (DatiShape.TestCeiling==true) {
		DatiShape.ShiftCeiling = 2;
		// ora invertire direzione freccia
		n=DatiShape.DirezioneFreccia;
		switch (n) {
		case 1:
			n =3;
			break;
		case 3:
			n=1;
			break;
		}
		DatiShape.DirezioneFreccia = n;

	}
	else {
		DatiShape.ShiftCeiling = 1;
	}
	switch (DatiShape.DirezioneFreccia) {
	case 0:
		// nessuna freccia selezionata, dare errore
		MostraMessageBoxNGLE("ERROR: missing white arrows to select direction of current shape effect",3);
		return false;
	case 1:
		// ovest
		
		if (DatiShape.MiaSelezione.right < MinSizeDirezione) {
			MessaggioTroppoCorto(MinSizeDirezione);
			return false;
		}

		DatiShape.StrisciaIniziale = DatiShape.MiaSelezione.left + DatiShape.MiaSelezione.right -1;
		DatiShape.StrisciaFinale = DatiShape.MiaSelezione.left;
		DatiShape.StrisciaIncremento = -1;
		DatiShape.StrisciaSize = DatiShape.MiaSelezione.bottom;
		DatiShape.TestVerticale =false;
		DatiShape.StrisciaTot = DatiShape.MiaSelezione.right;

		break;
	case 2:
		// verso nord
		if (DatiShape.MiaSelezione.bottom < MinSizeDirezione) {
			MessaggioTroppoCorto(MinSizeDirezione);
			return false;
		}
		DatiShape.StrisciaIniziale = DatiShape.MiaSelezione.top + DatiShape.MiaSelezione.bottom -1;
		DatiShape.StrisciaFinale = DatiShape.MiaSelezione.top;
		DatiShape.StrisciaIncremento = -1;
		DatiShape.StrisciaSize = DatiShape.MiaSelezione.right;
		DatiShape.StrisciaTot = DatiShape.MiaSelezione.bottom;
		DatiShape.TestVerticale=true;
		break;
	case 3:
		// verso est
		if (DatiShape.MiaSelezione.right < MinSizeDirezione) {
			MessaggioTroppoCorto(MinSizeDirezione);
			return false;
		}

		DatiShape.StrisciaIniziale = DatiShape.MiaSelezione.left;
		DatiShape.StrisciaFinale = DatiShape.MiaSelezione.left + DatiShape.MiaSelezione.right -1;
		DatiShape.StrisciaIncremento = 1;
		DatiShape.StrisciaSize = DatiShape.MiaSelezione.bottom;
		DatiShape.TestVerticale =false;
		DatiShape.StrisciaTot = DatiShape.MiaSelezione.right;
		break;

	case 4:
		// verso sud
		if (DatiShape.MiaSelezione.bottom < MinSizeDirezione) {
			MessaggioTroppoCorto(MinSizeDirezione);
			return false;
		}
		DatiShape.StrisciaIniziale = DatiShape.MiaSelezione.top;
		DatiShape.StrisciaFinale = DatiShape.MiaSelezione.top + DatiShape.MiaSelezione.bottom -1;
		DatiShape.StrisciaIncremento = 1;
		DatiShape.StrisciaSize = DatiShape.MiaSelezione.right;
		DatiShape.StrisciaTot = DatiShape.MiaSelezione.bottom;
		DatiShape.TestVerticale=true;
		break;
	default:
		// freccia angolari, rifiutare
		MostraMessageBoxNGLE("ERROR: white arrows of selection must follow hortogonal directions for current shape effect",3);
		return false;
	}
	return true;


}


// crea una salita o una discesa a seconda di Incremento (+1 = salita / -1 = discesa)
// se TestStepped = true crea scalini
// se TestStepped = false crea una salita liscia 
// I settori interessati sono quelli di miaselezione e la direzione
// e' stabilita da TipoFrecciaSelezione
// aumenta (o diminuisce) di un click la serie
// se c'e' un errore restituisce false (e non modifica niente)
bool CreaSalita(bool TestStepped, int Incremento, 
				bool TestCeiling, bool TestProgressive)
{

	int IndiceStriscia;
	float Temp1,Temp2;
	int n;
	int VetFloor[4];
	short *pFloorPrima, *pFloorDopo;
	RECT SelNow;
	RECT SelDopo;
	int i;
	int ShiftC;
	char *pCorners;
	char *pCornersDopo;

	DatiShape.IncrementoAltezza = Incremento;
	DatiShape.TestStepped = TestStepped;
	DatiShape.TestCeiling=TestCeiling;

	if (CalcolaDirezioneShape(2)==false) return false;
	ShiftC = DatiShape.ShiftCeiling;

	// calcolare l'incremento (o decreemnto) in click da eseguire ad ogni ciclo
	// prima calcolare altezza attuale di primo e ultimo settore
	// poi incrementare (o decrementare a seconda di incremnto attuale
	DatiShape.MiniSel.right=1;
	DatiShape.MiniSel.bottom=1;
	if (DatiShape.TestVerticale == true) {
		// verticale
		// ottenere click iniziale
		DatiShape.MiniSel.top  = DatiShape.StrisciaIniziale;
		DatiShape.MiniSel.left  = DatiShape.MiaSelezione.left;
		pFloorPrima= (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);
		DatiShape.ClickInizio = *pFloorPrima;

		// ottenere click finale
		DatiShape.MiniSel.top  = DatiShape.StrisciaFinale;
		pFloorDopo = (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);

		DatiShape.ClickFine = *pFloorDopo;

	}else {
		// orizzontale
		// click iniziale
		DatiShape.MiniSel.left  = DatiShape.StrisciaIniziale;
		DatiShape.MiniSel.top   = DatiShape.MiaSelezione.top;
		pFloorPrima = (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);
		DatiShape.ClickInizio = *pFloorPrima;
		// click finale
		DatiShape.MiniSel.left  = DatiShape.StrisciaFinale;
		pFloorDopo = (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);
		DatiShape.ClickFine = *pFloorDopo;

	}



	// ora incrmeentare (seguendo incrmentoaltezza)
	DatiShape.ClickFine += DatiShape.IncrementoAltezza;

	// ora calcolare l'incremento da assegnare ad ogni settore
	Temp1 = (float) DatiShape.ClickInizio;
	Temp2 = (float) DatiShape.ClickFine;
	DatiShape.ClickIncremento = (Temp2 - Temp1) / (DatiShape.StrisciaTot-1);
	DatiShape.ClickNow= (float) DatiShape.ClickInizio;
	IndiceStriscia = DatiShape.StrisciaIniziale;

	do {
		// eseguire stessa modifica per tutta la larghezza di striscia attuale
		
		if (DatiShape.TestVerticale==true) {

			DatiShape.MiniSel.left = DatiShape.MiaSelezione.left;
			DatiShape.MiniSel.top = IndiceStriscia;
			DatiShape.MiniSel.right =DatiShape.MiaSelezione.right;
			DatiShape.MiniSel.bottom =1;


		}else {
			// orizzontale
			DatiShape.MiniSel.left = IndiceStriscia;
			DatiShape.MiniSel.top = DatiShape.MiaSelezione.top;
			DatiShape.MiniSel.right = 1;
			DatiShape.MiniSel.bottom= DatiShape.MiaSelezione.bottom;

		}
		// ora modificare l'altezza floor e azzerare i corner floor


		n = Float2Int(DatiShape.ClickNow);



		// qui ho messo SET invece di ADD
		ModificaClickSettore(FTO_SET_FLOOR * ShiftC,false,n);
		
		IndiceStriscia += DatiShape.StrisciaIncremento;
		DatiShape.ClickNow += DatiShape.ClickIncremento;
		n = DatiShape.StrisciaFinale + DatiShape.StrisciaIncremento;
		
	}while(IndiceStriscia != n);

	if (DatiShape.TestStepped == false) {
		// ora inclinare i corner
		IndiceStriscia = DatiShape.StrisciaIniziale;

		do {
			// eseguire stessa modifica per tutta la larghezza di striscia attuale
			
			if (DatiShape.TestVerticale==true) {

				DatiShape.MiniSel.left = DatiShape.MiaSelezione.left;
				DatiShape.MiniSel.top = IndiceStriscia;
				DatiShape.MiniSel.right =DatiShape.MiaSelezione.right;
				DatiShape.MiniSel.bottom =1;
				// salvare questa selezione now in SelNow
				CopiaRect(&SelNow,&DatiShape.MiniSel);
				// ora calcolare la selezone di casella successiva
				CopiaRect(&SelDopo,&SelNow);
				SelDopo.top += DatiShape.StrisciaIncremento;

			}else {
				// orizzontale
				DatiShape.MiniSel.left = IndiceStriscia;
				DatiShape.MiniSel.top = DatiShape.MiaSelezione.top;
				DatiShape.MiniSel.right = 1;
				DatiShape.MiniSel.bottom= DatiShape.MiaSelezione.bottom;
				// salvare questa selezione now in SelNow
				CopiaRect(&SelNow,&DatiShape.MiniSel);
				// ora calcolare la selezone di casella successiva
				CopiaRect(&SelDopo,&SelNow);
				SelDopo.left += DatiShape.StrisciaIncremento;
			}
			// leggere altezza floor attuale e successiva
			pFloorPrima = (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);
			pCorners = (char*) ModificaClickSettore(FTO_GET_FLOOR_CORNERS * ShiftC,false);
			
			CopiaRect(&DatiShape.MiniSel,&SelDopo);
			pFloorDopo = (short*) ModificaClickSettore(FTO_GET_FLOOR * ShiftC,false);
			pCornersDopo = (char*) ModificaClickSettore(FTO_GET_FLOOR_CORNERS * ShiftC,false);

			// ripristinare minisel
			CopiaRect(&DatiShape.MiniSel,&SelNow);

			// ora calcolare differenza di click


			n = (*pFloorDopo) - (*pFloorPrima);

#ifdef LOG_SHAPE

			sprintf(BufferLog,"X=%d  ClickPrima=%d  ClickDopo=%d   n=%d",
				DatiShape.MiniSel.left, *pFloorPrima,*pFloorDopo,n);
			InviaLog(BufferLog);
			sprintf(BufferLog,"\tCorner[0]=%d  Corner[1]=%d  Corner[2]=%d Corner[3]=%d",
				pCorners[0],pCorners[1],pCorners[2],pCorners[3]);
			InviaLog(BufferLog);

#endif
			
			// adesso bisogna alzare di questo valore solo i due corner 
			// che sono orientati verso il prossimo blocco
			// azzerare tutto

			for (i=0;i<4;i++) VetFloor[i]=0;
			switch (DatiShape.DirezioneFreccia) {
			case 1:
				// verso ovest (indice 0 e 1)
				VetFloor[Ruota(0)] = pCornersDopo[Ruota(3)] - pCorners[Ruota(0)] + n;
				VetFloor[Ruota(1)] = pCornersDopo[Ruota(2)] - pCorners[Ruota(1)] +  n;
				break;
			case 2:
				// verso nord (indice 1 e 2)
				VetFloor[Ruota(1)] = pCornersDopo[Ruota(0)] - pCorners[Ruota(1)] +n;
				VetFloor[Ruota(2)] = pCornersDopo[Ruota(3)] - pCorners[Ruota(2)] +n;
				break;
			case 3:
				// verso est (indice 2 e 3)
				VetFloor[Ruota(2)] = pCornersDopo[Ruota(1)] - pCorners[Ruota(2)] +n;
				VetFloor[Ruota(3)] = pCornersDopo[Ruota(0)] - pCorners[Ruota(3)] +n;
				break;
			case 4:
				// verso sud (indice 3 e 0)
				VetFloor[Ruota(0)]= pCornersDopo[Ruota(1)] - pCorners[Ruota(0)] +n;
				VetFloor[Ruota(3)]= pCornersDopo[Ruota(2)] - pCorners[Ruota(3)] +n;
				break;
			}
			ModificaClickSettore(FTO_ADD_FLOOR_CORNERS * ShiftC,false,
					VetFloor[0], VetFloor[1], VetFloor[2], VetFloor[3]);

			IndiceStriscia += DatiShape.StrisciaIncremento;
			// in questo caso fermarsi una striscia prima
			n = DatiShape.StrisciaFinale;
			
		}while(IndiceStriscia != n);
		// nell'ultimo settore usare la stessa inclinazione precedente
		CopiaRect(&DatiShape.MiniSel,&SelDopo);
		ModificaClickSettore(FTO_ADD_FLOOR_CORNERS * ShiftC,false,
				VetFloor[0], VetFloor[1], VetFloor[2], VetFloor[3]);
	}
	return true;
}
// crea una salita liscia in area selezionata sul floor
void Menu_SmoothSlope(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaSalita(false,1,false,true);
	TerminaShape(true,false);
}
// crea un tunnel dove c'e' la selezione e richiede le dimensioni
void CreaTunnel(void) 
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	bool TestWall;  // se tunnel orizzontale
	bool TestFloor;  // se tunnel verticale
	int Muro; // 0 = ovest, 1=nord, 2=est, 3 = sud
	int SizeX,SizeY;
	int x1,y1,x2,y2;
	bool TestErrore;
	int x,y;
	int MaxFloor;
	int MinCeiling;
	int *pInt;
	int RoomSizeX,RoomSizeZ,RoomSizeY;
	int RoomOrgX,RoomOrgZ,RoomOrgY;
	int Lunghezza,Altezza;
	char *pMex;
	char **pVetParti;
	int TotParti;
	int StanzaOld,StanzaNew;
	int i;
	int *pTotRooms = (int*) 0x56C710;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTipoFrecciaSelezione = (int *) 0x583BB6;
	int ClickFloor,ClickCeiling;
	bool TestFlippata;
	bool TestAllineaFloor;


	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;
	if (IniziaShape(true,false,1,false)==false) return;
	StanzaOld = *pIndiceRoomAttuale;
	if (DatiShape.DirezioneFreccia == 3) 
		TestAllineaFloor=false;
	else
		TestAllineaFloor=true;

	if (pVetRooms[GetIndiceRoom(StanzaOld)].RoomButtons & 0x2) 
		// stanza e' una flip room
		TestFlippata=true;
	else
		TestFlippata=false;

	// analizzare la seelzione e verificare che non siano mescolati settori
	// wall e floor
	TestWall=false;
	TestFloor=false;
	Muro=-1;
	SizeX = pVetRooms[StanzaOld].XSizeBlocks -1;
	SizeY = pVetRooms[StanzaOld].ZSizeBlocks -1;
	x1 = DatiShape.MiaSelezione.left;
	y1 = DatiShape.MiaSelezione.top;
	x2 = DatiShape.MiaSelezione.left + DatiShape.MiaSelezione.right -1;
	y2 = DatiShape.MiaSelezione.top + DatiShape.MiaSelezione.bottom -1;

	// controllare muro Nord
	if (y1 == 0) {
		TestWall=true;
		Muro = MURO_NORD;
	}

	// muro ovest
	if (x2== SizeX) {
		TestWall =true;
		Muro= MURO_EST;
	}

	// muro sud
	if (y2 == SizeY) {
		TestWall=true;
		Muro=MURO_SUD;
	}

	// muro ovest
	if (x1 == 0) {
		TestWall=true;
		Muro=MURO_OVEST;
	}
	// ora controlla se c'e' un settore floor
	// se entrambe le dimensioni hanno size maggiori di 0 allora
	// c'e' anche settore floor

	for (x=x1;x<=x2;x++) {
		for (y=y1;y<=y2;y++) {
			if (x !=0 && x != SizeX && y !=0 && y != SizeY) {
				TestFloor=true;
				break;
			}
		}
		if (TestFloor==true) break;
	}

	if (TestFloor==true && TestWall==true) {
		MostraMessageBoxNGLE("ERROR: you cann't select WALL and also FLOOR sectors in same moment",3);
		TerminaShape(false,false);
		return;
	}
	// vedere se ci sono settori d'angolo
	TestErrore=false;

	if (x1 == 0 && y1 == 0) TestErrore=true;
	
	if (x1 == 0 && y1 == SizeY) TestErrore=true;

	if (x2 == SizeX && y1 == SizeY) TestErrore = true;

	if (x2 == SizeX && y1 == 0) TestErrore=true;

	if (TestErrore==true) {
		MostraMessageBoxNGLE("ERROR: you cann't select a CORNER sector as door",3);
		TerminaShape(false,false);
		return;
	}
	// se tipo wall controllare altezza massima di floor e minima di ceiling
	MaxFloor=0;
	MinCeiling=0;
	if (TestWall==true) {

		SetMiniSel(x1,y1);
		pInt = (int*) ModificaClickSettore(FTO_GET_MAX_FLOOR,false);
		MaxFloor=*pInt;

		pInt = (int*) ModificaClickSettore(FTO_GET_MAX_CEILING, false);
		MinCeiling= *pInt;
	}

	// ora chiedere dati
	if (TestWall==true) {
		pMex = InputTextBox("Type the length in sectors and the height of new room in clicks.\r\nSeparate two values with comma \",\" character.\r\nExample:\r\n\r\n3,8\r\n\r\nIt will create a new room with height of 8 clicks (2 sectors) and lenght of 3 sectors",
					"Creation of new tunnel room","2,4",TP_CENTRALE);
		if (pMex==NULL) {
			TerminaShape(false,false);
			return;
		}
		pVetParti=Split(pMex,',',&TotParti,NULL,NULL,false);
		if (TotParti != 2) {
			MostraMessageBoxNGLE("ERROR: syntax error in parameter list",3);
			TerminaShape(false,false);
			return;
		}
		Lunghezza=0;
		Altezza=0;

		sscanf(pVetParti[0],"%d",&Lunghezza);
		if (Lunghezza < 1 || Lunghezza > 18) {
			MostraMessageBoxNGLE("ERROR: length (first) parameter is out of range. (Valid range: 1 / 18)",3);
			TerminaShape(false,false);
			return;
		}

		sscanf(pVetParti[1],"%d",&Altezza);
		if (Altezza < 1 || Altezza > 128) {
			MostraMessageBoxNGLE("ERROR: height (second) parameter is out of range. (Valid range: 1 / 128)",3);
			TerminaShape(false,false);
			return;
		}
		// ok, ora costruire tutti i dati della stanza
		switch (Muro) {
		case MURO_NORD:
		case MURO_SUD:
			// ----- valori uguali sia per NORD che per SUD ---------
			// SizeX e' la larghezza della selezione
			RoomSizeX = DatiShape.MiaSelezione.right;
			// SizeZ e' la lunghezza
			RoomSizeZ = Lunghezza;
			// SizeY e' altezza
			RoomSizeY = Altezza;
			// RoomOrgX = primo settore di wall selezionato
			RoomOrgX = pVetRooms[StanzaOld].XBlocksWorld + x1-1;
			// roomOrgY = MaxFloor+Altezza
			if (TestAllineaFloor==true) 
				RoomOrgY = MaxFloor + Altezza;
			else
				RoomOrgY = MinCeiling;

			// ---- fine valori uguali --------
			if (Muro==MURO_NORD) {
				// NORD
				// RoomOrgZ = OrgOldZ - RoomSizeZ
				RoomOrgZ = pVetRooms[StanzaOld].ZBlocksWorld - RoomSizeZ;
			}else
			{
				// SUD
				RoomOrgZ = pVetRooms[StanzaOld].ZBlocksWorld + SizeY -1;
			}
			break;
		case MURO_EST:
		case MURO_OVEST:
			// --- valori uguali sia per EST che per OVEST --------
			RoomSizeX = Lunghezza;
			RoomSizeY = Altezza;
			RoomSizeZ = DatiShape.MiaSelezione.bottom;
			if (TestAllineaFloor==true) 
				RoomOrgY = MaxFloor+Altezza;
			else
				RoomOrgY = MinCeiling;

			RoomOrgZ = pVetRooms[StanzaOld].ZBlocksWorld + y1-1;
			if (Muro == MURO_OVEST) {
				RoomOrgX = pVetRooms[StanzaOld].XBlocksWorld - RoomSizeX;
			}else {
				RoomOrgX = pVetRooms[StanzaOld].XBlocksWorld + SizeX-1;
			}
			break;
		}			

	}else {
		// tunnel verticale
		pMex = InputTextBox("Type lenght of vertical tunnel in clicks.\r\nUse negative numbers if you want a tunnel from floor selection, or use a positive number to create a tunnel in ceiling selection.\r\nExample:\r\n-12\r\nIt creates a floor tunnel with depth of 3 sectors (i.e. 12 clicks)",
				"Creation of vertical tunnel","-8",TP_CENTRALE);
		if (pMex==NULL) {
			TerminaShape(false,false);
			return;
		}
		Altezza=0;
		sscanf(pMex,"%d",&Altezza);
		if (Altezza == 0 || Altezza < -128 || Altezza > 128) {
			MostraMessageBoxNGLE("ERROR: length out of range",3);
			TerminaShape(false,false);
			return;
		}

		// vedere se e' sul floor o sul sofitto
		if (Altezza > 0) 
			TestFloor=false;
		else {
			TestFloor=true;
			Altezza=-Altezza;
		}

		// inserire prima valori uguali per entrambi
		RoomOrgX = pVetRooms[StanzaOld].XBlocksWorld + x1 -1;
		RoomOrgZ = pVetRooms[StanzaOld].ZBlocksWorld + y1 -1;
		RoomSizeX = DatiShape.MiaSelezione.right;
		RoomSizeZ = DatiShape.MiaSelezione.bottom;
		RoomSizeY = Altezza;

		if (TestFloor==true) {
			// tunnel sul pavimento
			RoomOrgY = pVetRooms[StanzaOld].OrgYClickBottom;
		}else {
			// tunnel sul soffitto
			RoomOrgY = pVetRooms[StanzaOld].OrgYClickTop + Altezza;
		}

	}
	


	// ora cercare una stanza vuota
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot) break;
	}

	if (i == *pTotRooms) {
		MostraMessageBoxNGLE("Cann't find an empty slot room for new room to add",3);
		TerminaShape(false,false);
		return;
	}
	StanzaNew=i;
	MostraNuovaStanza(StanzaNew);		



	// ora siamo nella nuova stanza, prima di tutto modificare selzione
	// ora fare un bound in modo da avere la dimensione x e z corretta
	// prima impostare selezione ai valori desiderati
	pRectSelezione2d->left  = 1;
	pRectSelezione2d->top = 1;
	pRectSelezione2d->right = RoomSizeX;
	pRectSelezione2d->bottom = RoomSizeZ;
	*pTestEsisteSelezioneBox =1;
	*pTipoFrecciaSelezione=0;
	roomBoundRoom(NULL);

	// ora cambiare orgy e dimensioney
	ClickCeiling=RoomOrgY;
	ClickFloor = RoomOrgY-RoomSizeY;

	pVetRooms[StanzaNew].XBlocksWorld = RoomOrgX;
	pVetRooms[StanzaNew].OrgYClickTop = ClickCeiling;
	pVetRooms[StanzaNew].OrgYClickBottom = ClickFloor;
	pVetRooms[StanzaNew].ZBlocksWorld = RoomOrgZ;

	// ora bisogna aggiornare tutti i valori della griglia
	MostraNuovaStanza(StanzaNew);

	DatiShape.TestCeiling =false;
	SetMiniSel(0,0);
	DatiShape.MiniSel.right = pVetRooms[StanzaNew].XSizeBlocks;
	DatiShape.MiniSel.bottom  = pVetRooms[StanzaNew].ZSizeBlocks;

	ModificaClickSettore(FTO_SET_FLOOR,false,ClickFloor);
	ModificaClickSettore(FTO_SET_CEILING, false,ClickCeiling);

	MostraNuovaStanza(StanzaNew);
	// copiare luminocista d'ambiente
	pVetRooms[StanzaNew].AmbientLightColor = pVetRooms[StanzaOld].AmbientLightColor;

	if (TestFlippata==true) {
		// adesso bisogna creare una nuova stanza flippata per la stanzanew
		// e assegnarle lo stesso numero di flipmap di quella di stanzaold
		pVetRooms[StanzaNew].FlipMapValue = pVetRooms[StanzaOld].FlipMapValue;
		roomCreaFlipMap(NULL);

	}
	if (TestWall==true || TestFloor==true) {
		// ora ripristinare stanza old
		MostraNuovaStanza(GetIndiceRoom(StanzaOld));
		// e ripristinare selezione originale per poter applicare door
		TerminaShape(true,false);
		// adesso chiamare creadoor
		roomCreaDoor(NULL);
	}else {
		// tunnel sul soffitto.
		// bisogna partire da stanza new (che e' quella attuale)
		// selezionare tutto il pavimento
		pRectSelezione2d->left = pVetRooms[StanzaNew].XDisplayOffset +1;
		pRectSelezione2d->top = pVetRooms[StanzaNew].ZDisaplyOffset + 1;
		pRectSelezione2d->right = pVetRooms[StanzaNew].XSizeBlocks -2;
		pRectSelezione2d->bottom = pVetRooms[StanzaNew].ZSizeBlocks -2;
		*pTestEsisteSelezioneBox =1;
		*pTipoFrecciaSelezione=0;
		// adesso chiamare creadoor
		roomCreaDoor(NULL);
		TerminaShape(false,false);

	}


}
void Menu_SteppedSlope(void *pPunta)
{
	if (IniziaShape(true,true,1,false)== false) return;

	CreaSalita(true,1,false,false);
	TerminaShape(true,false);

}

// crea una discesa in ceiling
void Menu_CSmoothSlope(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaSalita(false,-1,true,false);
	TerminaShape(true,false);
}

void Menu_CSteppedSlope(void *pPunta)
{
	if (IniziaShape(true,true,1,false)== false) return;

	CreaSalita(true,-1,true,false);
	TerminaShape(true,false);

}
void GosubCreaTriangoliTexture(bool TestFloor)
{
	int ShiftC;

	if (IniziaShape(true,true,1,false)== false) return;
	if (TestFloor==true) {
		DatiShape.TestCeiling =false;
		ShiftC=1;
	}
	else {
		DatiShape.TestCeiling=true;
		ShiftC=2;
	}

	CopiaRect(&DatiShape.MiniSel,&DatiShape.MiaSelezione);
	ModificaClickSettore(FTO_SET_TRIANGLE_TEXTURE_FLOOR * ShiftC,false);
	TerminaShape(true,false);
}




// imposta datishape.minisel le coordinate di input impostando la dimensione
// della selezione a 1,1
void SetMiniSel(int CasaX, int CasaY)
{
	DatiShape.MiniSel.left = CasaX;
	DatiShape.MiniSel.top = CasaY;
	DatiShape.MiniSel.right = 1;
	DatiShape.MiniSel.bottom = 1;

}
// crea salita o discesa casuale su pavimento o soffitto.
// a seconda di freccie selezione sceglie la direzione di crescita maggiore
// se non ci sono freccie alza tutto con la stessa probabilita'
bool CreaRandomFloor(bool TestFloor, bool TestPiu)
{

	BYTE Tabella[20][20];
	int  x,y;
	POINT MaxOrigine;
	int SizeX,SizeY;
	POINT Org;
	bool TestNoSalita;
	bool TestDiagonale;
	bool TestVerticale;
	float Distanza;
	float Potenza1,Potenza2;
	float MaxDistanza;
	int TotClick, TotOperazioni;
	int Numero;
	int Direzione;
	int i,n;
	bool TestSmooth;
	bool TestHills;

	WORD VetRandom[20*20*25];
	int TotRandom;
	WORD Indice;
	int ShiftC;

	short *pMinCeiling;
	short *pMaxFloor;
	int  MinCeiling;
	int MaxFloor;
	bool TestLimiteAltezza;
	bool TestInverso;
	RECT SelLarga;
	bool TestUguale;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;


	if (MyPrefRoom.ParameterRandomFloor  & RF_DONT_SMOOTH) 
		TestSmooth=false;
	else
		TestSmooth=true;

	if (TestFloor==true) 
		DatiShape.TestCeiling=false;
	else
		DatiShape.TestCeiling=true;

	if (TestFloor==true) 
		ShiftC=1;
	else
		ShiftC=2;

	// se e' ceiling invertire freccie
	if (TestFloor==false) {

		// ora invertire direzione freccia
		n=DatiShape.DirezioneFreccia;
		switch (n) {
		case 1:
			n =3;
			break;
		case 3:
			n=1;
			break;
		case 5:
			n=6;
			break;
		case 6:
			n =5;
			break;
		case 7:
			n=8;
			break;
		case 8:
			n=7;
			break;

		}
		DatiShape.DirezioneFreccia = n;
	}
	SizeX= DatiShape.MiaSelezione.right;
	SizeY = DatiShape.MiaSelezione.bottom;
	Org.x = DatiShape.MiaSelezione.left;
	Org.y = DatiShape.MiaSelezione.top;

	// ampliare la selezione di una casa in ogni direzione 
	
	CopiaRect(&SelLarga,&DatiShape.MiaSelezione);
	if ((SelLarga.left -1) != 0) {
		SelLarga.left --;
		SelLarga.right++;
	}
	if ((SelLarga.top -1) !=0) {
		SelLarga.top--;
		SelLarga.bottom++;
	}
	if ((SelLarga.left + SizeX+1) != DatiShape.pRoom->XSizeBlocks) 
		SelLarga.right++;

	if ((SelLarga.top + SizeY+1) != DatiShape.pRoom->ZSizeBlocks)
		SelLarga.bottom++;

#ifdef LOG_SHAPE
	sprintf(BufferLog,"Seleziona Allargata: x=%d y=%d sizex=%d sizey=%d",
		SelLarga.left,SelLarga.top,SelLarga.right,SelLarga.bottom);
	InviaLog(BufferLog);
#endif
	// adesso vedere se e' necessario dati texture adesso oppure
	// lasciare quella precedente
	
	TestUguale=true;
	if (TestFloor==true) {
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneFloor.left || 
			SelLarga.top != BaseRotazioni.SelezioneFloor.top ||
			SelLarga.right != BaseRotazioni.SelezioneFloor.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneFloor.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;
	}else {
		// confronto dati ceiling
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneCeiling.left || 
			SelLarga.top != BaseRotazioni.SelezioneCeiling.top ||
			SelLarga.right != BaseRotazioni.SelezioneCeiling.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneCeiling.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;

	}

	if (TestUguale==false) {
		// bisogna creare adesso tabella
		CopiaRect(&DatiShape.MiniSel,&SelLarga);
		ModificaClickSettore(FTO_SET_TABELLA_ROTAZIONE_FLOOR*ShiftC,false);
	}
	// trovare punto di origine dove l'incremento sara' massimo
	/*
	0 = nessuna freccia
	1 = Verso ovest
	2 = verso nord
	3 = verso est
	4 = verso sud
	5 = verso nord-ovest
	6 = verso nord-est
	7 = verso sud-est
	8 = verso sud-ovest
	*/
	TestNoSalita=false;
	
	switch (DatiShape.SalvaFrecciaSelezione) 
	{
	case 0:
		// nessuna freccia, quindi anche nessuna variazione (usare sempre 2)
		TestNoSalita=true;

		break;
	case 1:
		// verso ovest
		TestDiagonale=false;
		TestVerticale=false;
		MaxOrigine.x = Org.x;
		MaxDistanza = (float) SizeX;

		break;
	case 2:
		// verso nord
		TestDiagonale=false;
		TestVerticale=true;
		MaxOrigine.y = Org.y;
		MaxDistanza = (float) SizeY;
		break;
	case 3:
		// Verso est
		TestDiagonale=false;
		TestVerticale=false;
		MaxDistanza = (float) SizeX;
		MaxOrigine.x = Org.x + SizeX-1;
		break;
	case 4:
		// verso sud
		TestDiagonale=false;
		TestVerticale=true;
		MaxOrigine.y = Org.y + SizeY-1;
		MaxDistanza = (float) SizeY;
		break;
	case 5:
		// verso nord ovest
		TestDiagonale=true;
		MaxOrigine.x = Org.x;
		MaxOrigine.y = Org.y;
		break;
	case 6:
		// verso nord est
		TestDiagonale=true;
		MaxOrigine.x= Org.x + SizeX-1;
		MaxOrigine.y = Org.y;
		break;
	case 7:
		// Verso sud est
		TestDiagonale=true;
		MaxOrigine.x = Org.x + SizeX-1;
		MaxOrigine.y = Org.y + SizeY-1;
		break;
	case 8:
		// verso sud ovest
		TestDiagonale=true;
		MaxOrigine.x = Org.x;
		MaxOrigine.y = Org.y + SizeY-1;
		break;
	}
	if (TestDiagonale ==true) {
		// calcolare distanza massima
		Potenza1 = (float) SizeX*SizeX;
		Potenza2 = (float) SizeY*SizeY;
		MaxDistanza = (float) sqrt(Potenza1+Potenza2);
	}
	TestHills=false;
	if ((MyPrefRoom.ParameterRandomFloor & RF_MOUNTAINS) == 0) TestHills=true;
	TestLimiteAltezza=false;
	if ((MyPrefRoom.ParameterRandomFloor & RF_LIMIT_HEIGHT) && 
		((TestFloor == true && TestPiu==true) ||
		(TestFloor==false && TestPiu==false))) TestLimiteAltezza=true;

	TotRandom=0;
#ifdef LOG_SHAPE
	sprintf(BufferLog,"FlagRandomSlope=0x%X TestHills=%d  MaxDistanza=%f  TestSmooth=%d",
		MyPrefRoom.ParameterRandomFloor ,TestHills,MaxDistanza,TestSmooth);
	InviaLog(BufferLog);
#endif

	for (y=Org.y ;y< (Org.y + SizeY);y++) {
		for (x=Org.x; x <(Org.x + SizeX);x++) {
			if (TestNoSalita==true) {
				// stessi click per tutta la griglia
				Tabella[x][y] = 2;
				Distanza=2;
			}else
			{
				// ora calcolare numero a seconda dalla distanza da punto
				// massimo
				if (TestDiagonale==true) {
					// e' un punto diagonale, calcolare distanza
					//   __________________________________
					// \| (Abs(x1-x2))^2  + (Abs(x1-x2))^2
					Potenza1 = (float)x - MaxOrigine.x;
					Potenza1 *= Potenza1;

					Potenza2 = (float)y - MaxOrigine.y;
					Potenza2 *= Potenza2;

					Distanza = (float) sqrt(Potenza1 + Potenza2);
				}else{
					// distanza solo su un lato
					if (TestVerticale==true) 
						Distanza = (float) abs(y - MaxOrigine.y);
					else
						Distanza = (float) abs(x - MaxOrigine.x);

				}
				// ora calcolare un click per ogni blocco
			
				Distanza = MaxDistanza - Float2Int(Distanza);
				
					
				Tabella[x][y] = (BYTE) Float2Int(Distanza);


			}
			if (TestHills==true) {
				// costruire vettore di probabilita
				// calcoolare indice assoluto
				Indice = x + y *256;
				for (i =0;i<Distanza;i++) {
					VetRandom[TotRandom++] = (WORD) Indice;
				}
			}
		}
#ifdef LOG_SHAPE
	char BufferTemp[256];
		BufferLog[0]=0;
		for (x=Org.x; x <(Org.x + SizeX);x++) {
			sprintf(BufferTemp,"%2d ",Tabella[x][y]);
			strcat(BufferLog,BufferTemp);
		}
		InviaLog(BufferLog);
#endif


	}
	// adesso prima provare ad alzare di vari click tutta la zona.

	Numero = SizeX * SizeY;

	if (TestHills==true)
		Numero /=3;
	else
		Numero /=8;

	if (TestNoSalita==true && TestHills==false ) Numero *=2;

	if (MyPrefRoom.ParameterRandomFloor  & RF_MANY_CHANGES) Numero *= 2;

	if (MyPrefRoom.ParameterRandomFloor  & RF_FEW_CHANGES) Numero /=2;


	// calcolare il numero di operazioni da eseguire (la media e' di uno
	// ogni quadrato di 2x2 settori
	// TotOperazioni = Casuale(Numero);
	
	TotOperazioni=Numero+1;

		
#ifdef LOG_SHAPE
	sprintf(BufferLog,"CreaSlope() SizeX=%d  SizeY=%d TestFloor=%d  TestPiu=%d  TotOperazioni=%d TotRandom=%d",
		SizeX,SizeY,TestFloor,TestPiu,TotOperazioni, TotRandom);
	InviaLog(BufferLog);
#endif
	for (i=0;i<TotOperazioni;i++) {
		// calcolare posizione dove agire
		

		// calcolare di quanti click alzare o abbasare floor
		if (TestHills == true) {
			TotClick= 1;
			n= Casuale(TotRandom);
			Indice =VetRandom[n];
			y = Indice >> 8;
			x = Indice & 255;
		}
		else {
			x= Casuale(SizeX);
			y= Casuale(SizeY);
			x+= Org.x;
			y+= Org.y;
			if (TestNoSalita==true ) {

				n= Casuale(6);
			}
			else {
				n = Tabella[x][y];
				n = Casuale(n);

				if (n > 4) n-= 2;
			}
			if (n==0) n++;

			TotClick=n;
		}

		// se e' attivo controllo su altezza verificare che questi click
		// non siano tali da arrivare pavimento soffitto a toccarsi o peggio
		// a incrociarsi
		// nota: il controllo va fatto solo se si alavora su floor up
		// o su ceiling down
	
		if (TestLimiteAltezza==true) {

			pMinCeiling= (short*) ModificaClickSettore(FTO_GET_CEILING_HEIGHT,false);
			MinCeiling = *pMinCeiling;
			pMaxFloor = (short *) ModificaClickSettore(FTO_GET_FLOOR_HEIGHT,false);
			MaxFloor = *pMaxFloor;
			n = MinCeiling - MaxFloor;
			
			n--;
			if (TotClick > n) TotClick=n;
			if (TotClick < 0) TotClick=0;
#ifdef LOG_SHAPE
			sprintf(BufferLog,"MinCeiling=%d MaxFloor=%d n=%d  TotClick=%d",
				MinCeiling,MaxFloor,n,TotClick);
			InviaLog(BufferLog);
#endif
		}

		// ora bisogna decidere anche freccia casuale
		// (ammesse tutte tranne quelle ortogonali)
		if (MyPrefRoom.ParameterRandomFloor  & RF_DONT_SMOOTH ) {
			Numero = Casuale(9);
		}else {
			if (TestHills==false) {
				// non consentire faccia piana
				Numero = Casuale(4);
				Numero += 5;
			}else {
				Numero = Casuale(5);
				if (Numero > 0) Numero+= 4;
			}
		}
		Direzione = Numero;
#ifdef LOG_SHAPE
		sprintf(BufferLog,"\tTabella[%d,%d]=%d  Direzione=%d TotClick=%d",
			x,y,Tabella[x][y],Direzione,TotClick);
		InviaLog(BufferLog);
#endif
		if (TotClick > 0) {
			SetMiniSel(x,y);
			// ora alzare o abbassare floor o ceiling
			PulsantiFloor(TestFloor,TestPiu,TestSmooth,Direzione,TotClick);
		}

	}

	// se c'e' limitazione di altezza ora ricontrollare tutti i corner
	// e abbassare i corner di quelli troppo alti in modo proporzionale
	if (TestLimiteAltezza==true) {
		for (y=Org.y ;y< (Org.y + SizeY);y++) {
			for (x=Org.x; x <(Org.x + SizeX);x++) {
				SetMiniSel(x,y);
				pMinCeiling = 
					(short *) ModificaClickSettore(FTO_GET_CEILING_HEIGHT,false);
				MinCeiling= *pMinCeiling;

				pMaxFloor =
					(short *) ModificaClickSettore(FTO_GET_FLOOR_HEIGHT,false);
				MaxFloor = *pMaxFloor;

				n = MinCeiling-MaxFloor;
				if (n < 0) {
					n =-n;
					if (TestPiu == true)
						TestInverso=false;
					else
						TestInverso=true;

					PulsantiFloor(TestFloor,TestInverso,TestSmooth,0,n);

					
				}
			}
		}
	}

	// ora creare texture triangolari mancanti
	
	CopiaRect(&DatiShape.MiniSel,&SelLarga);
#ifdef LOG_SHAPE
	InviaLog("Ripristina texture dopo CreaRandomFloor()");
#endif
	ModificaClickSettore(FTO_SET_TRIANGLE_TEXTURE_FLOOR * ShiftC | 
						 FTO_USE_TABELLA_ROTAZIONE_FLOOR * ShiftC,false);
	

	return true;
}
void Menu_RandomFloorUp(void *pPunta)
{
	if (IniziaShape(true,true,2,true)== false) return;

	CreaRandomFloor(true,true);
	TerminaShape(true,true);

}

void Menu_RandomFloorDown(void *pPunta)
{
	if (IniziaShape(true,true,2,true)== false) return;

	CreaRandomFloor(true,false);
	TerminaShape(true,true);

}

void Menu_RandomCeilingUp(void *pPunta)
{
	if (IniziaShape(true,true,2,true)== false) return;

	CreaRandomFloor(false,true);
	TerminaShape(true,true);

}

void Menu_RandomCeilingDown(void *pPunta)
{
	if (IniziaShape(true,true,2,true)== false) return;

	CreaRandomFloor(false,false);
	TerminaShape(true,true);

}

// crea triangoli floor
void Menu_CreaTriangoli(void *pPunta)
{
	GosubCreaTriangoliTexture(true);
}

// crea triangoli ceiling
void Menu_CCreaTriangoli(void *pPunta)
{
	GosubCreaTriangoliTexture(false);
}
// imposta i parametri in FlagRandomSlope per modificare effetto random slope
void Menu_SetRandomSlope(void *pPunta)
{
	ShowSettings();
	
}

// imposta i coefficienti a, b , c della formula della parabola
// con lato e h di altezza nella variabile globale DatiParabola
void TrovaDatiParabola(double Lato, double h)
{
	double VerticeX, VerticeY;
	double a,Gatto,Topo,b;
#ifdef LOG_SHAPE
	sprintf(BufferLog,"TrovaDatiParabola()  Lato=%f   h=%f",
		Lato,h);
	InviaLog(BufferLog);
#endif

	Lato *= 4.0;

	VerticeX= Lato/ 2.0;
	VerticeY = h;

	DatiParabola.c=0;

	Topo = 4 * VerticeY;
	Gatto = -2 * VerticeX;
#ifdef LOG_SHAPE
	sprintf(BufferLog,"Vertice(%f   %f)   Topo=%f   Gatto=%f",
		VerticeX,VerticeY,Topo,Gatto);
	InviaLog(BufferLog);
#endif

	a = - Topo / pow(Gatto,2);
	DatiParabola.a = a;

	b = Gatto * a;
	DatiParabola.b = b;

#ifdef LOG_SHAPE
	sprintf(BufferLog,"DatiParabola: a=%f   b=%f   c=%f",
		  DatiParabola.a, DatiParabola.b, DatiParabola.c);
	InviaLog(BufferLog);
#endif

}

int TrovaClickParabola(double x)
{
   double Risultato;

   x *= 4.0;
   Risultato = DatiParabola.a * pow(x,2)  + DatiParabola.b * x + DatiParabola.c;

   return Float2Int((float) Risultato);

}
// curva floor o ceiling (TestCeiling) 
// alzandolo o abbassandolo (Incremento)
// se c'e' un errore restituisce false e non effettua alcuna operazione
bool CreaBlend(bool TestCeiling, bool TestInversa)
{

	int ShiftC;
	short VetClick[20];
	int i;
	int n;
	int Massimo;
	short Numero;
	short BaseClick;			
	int  IndiceStriscia;
	int ClickPrima, ClickDopo;
	int VetFloor[4];
	bool TestAdatta;
	int BaseFloor;
	double h;
	double Lato;
	short *pShort;
	double Raggio;
	short MaxFloor;
	double x,z;
	int *pValore;
	int k;
	int OldDifferenza;

	DatiShape.TestCeiling=TestCeiling;

	if (CalcolaDirezioneShape(2)==false) return false;
	ShiftC =DatiShape.ShiftCeiling;
#ifdef LOG_SHAPE
	InviaLog("CreaBlend() ------------------------------------");
#endif

	// calcolare punti di vertici
	switch (MyPrefRoom.BendType) {
	case 0:
		// tipo fisso (metodo vecchio basata su tabella prefissata)
		n = DatiShape.StrisciaTot;
		Lato=n;

		// sistema vecchio con bend fisso
		for (i=0;i<=n;i++) {
			VetClick[i] = VetCurva[n-2].VetClick[i];
		}
		SetMiniSel(DatiShape.MiaSelezione.left, DatiShape.MiaSelezione.top);

		if (DatiShape.TestCeiling == true) 
			pShort = (short*) ModificaClickSettore(FTO_GET_MAX_CEILING,false);
		else 
			pShort = (short*) ModificaClickSettore(FTO_GET_MIN_FLOOR,false);
		BaseClick = *pShort;

		break;
	case 1:
	case 2:
		// sistemi basati su sfera o su parabola



		// ------ trova dinamicamente striscia bend usando sfera ------
		// trovare massimo e minimo
		
		CopiaRect(&DatiShape.MiniSel,&DatiShape.MiaSelezione);
		// impostare il minimo
		pValore = (int*) ModificaClickSettore( 
				FTO_GET_MIN_FLOOR * DatiShape.ShiftCeiling,false);
		BaseFloor = *pValore;

		// ora trovare valore massimo attuale
		pValore = (int*) ModificaClickSettore(
				FTO_GET_MAX_FLOOR * DatiShape.ShiftCeiling,false);
		MaxFloor = *pValore;
#ifdef LOG_SHAPE
		sprintf(BufferLog,"BaseFloor=%d  MaxFloor=%d",
			BaseFloor,MaxFloor);
		InviaLog(BufferLog);
#endif

		
		if (DatiShape.TestCeiling == true ) {
			// invertire i due valori
			Numero = BaseFloor;
			BaseFloor = MaxFloor;
			MaxFloor= Numero;
#ifdef LOG_SHAPE
			InviaLog("Invertire basefloor e maxfloor");
#endif

		}
		
		// ora calcolare differenza (ignorando segni)
		h = abs(MaxFloor-BaseFloor);
		OldDifferenza = (int) h;

		do {
			// ora aumentarlo di 1 click
			h++;	


			// trovare centro della sfera
			Lato = DatiShape.StrisciaTot;
			if (MyPrefRoom.BendType == 1) {
				// se e' sfera, calcolare raggio e centro

				Centro.x = (float) (Lato * 4.0) / 2.0;
				Centro.y = TrovaRaggioLatoY(Lato,h, &Raggio);
				Centro.z = (double) (Lato * 4.0) / 2,0;

#ifdef LOG_SHAPE
				sprintf(BufferLog,"Raggio = %f  Centro{%f  %f  %f}  h=%f",
						Raggio,Centro.x,Centro.y, Centro.z, h);
				InviaLog(BufferLog);
#endif

				// se centro.y < 0 zero, ok altrimenti non eseguire niente
				if (Centro.y >= 0) {
#ifdef LOG_SHAPE
					InviaLog("Centro.y sfera = 0: rifiutarsi di continuare");
#endif
					MessageBeep(-1);
					return false;
				}
			}
			if (MyPrefRoom.BendType == 2) {
				// se e' parabola, calcolare dati per parabola
				TrovaDatiParabola(Lato, h);
			}
			// ora riempire striscia in VetClick[]
			if (DatiShape.TestVerticale == true) {
				// dall'alto in basso
				x = DatiShape.StrisciaTot / 2.0;
				z = 0;
			}else {
				// orizzontale, da sinistra a destra
				x = 0;
				z = DatiShape.StrisciaTot   / 2.0;
			}
#ifdef LOG_SHAPE
			InviaLog("Catturare altezza click:");
#endif

			for (i = 0;i<=Lato;i++) {
				if (DatiShape.TestVerticale==true) 
					z = i;
				else 
					x = i;
				if (MyPrefRoom.BendType == 1) 
					VetClick[i] = (short) TrovaClickSfera(x,z,Raggio);
				else
					VetClick[i] = (short) TrovaClickParabola(i);
#ifdef LOG_SHAPE
				sprintf(BufferLog,"\tx=%f  z=%f  Click=%d",
					x,z,VetClick[i]);
				InviaLog(BufferLog);
#endif

			}
#ifdef LOG_SHAPE


			InviaLog("Incrementi originali bending:");
			for (i=0;i<=Lato;i++) {
				sprintf(BufferLog,"\t%d", VetClick[i]);
				InviaLog(BufferLog);
			}

#endif	

			// trovare il click massimo per verificare che 
			// sia cambiato dall'inizio del comando
			Numero=0;
			for (i=0;i<=Lato;i++) {
				if (VetClick[i] > Numero) Numero = VetClick[i];
			}
			// adesso verificare che il massimo sia proprio h se non lo e'
			// aumentare massimo per eguagliarlo a h
		} while (Numero == OldDifferenza);
		// ok, ora bisogna fare il complemento dei click perche' il bend se
		// li aspettava in questo modo		
		// ora fare il complemento
		for (i=0;i<=Lato;i++) {
			VetClick[i] = Numero - VetClick[i];
		}
		n = (int) Lato;
		BaseClick=BaseFloor;
		break;
	}

#ifdef LOG_SHAPE


	InviaLog("Incrementi bending rapportati a soffitto:");
	for (i=0;i<=n;i++) {
		sprintf(BufferLog,"\t%d", VetClick[i]);
		InviaLog(BufferLog);
	}

#endif	
		
		
	// ------- fine ricerca striscia bend ------------


	// vedere se c'e' bisogno di adattare gli incrementi
	TestAdatta=false;
	if (TestCeiling == false && TestInversa == false) TestAdatta=true;
	if (TestCeiling == true && TestInversa == true) TestAdatta=true;


	if (TestAdatta==true) {
		// prima trovare il valore massimo
		Massimo =-1;
		for (i=0;i<=n;i++) {
			if (VetClick[i] > Massimo) Massimo=VetClick[i];
		}

		// ora convertire tutto
		for (i=0;i<=n;i++) {
			Numero = Massimo - VetClick[i];
			VetClick[i]=Numero;
		}
	}
	if (DatiShape.StrisciaIncremento < 0) {
		// invertire tutto
		Numero = DatiShape.StrisciaIniziale;
		DatiShape.StrisciaIniziale = DatiShape.StrisciaFinale;
		DatiShape.StrisciaFinale = Numero;
		DatiShape.StrisciaIncremento = 1;
	}

	IndiceStriscia = DatiShape.StrisciaIniziale;
	for (k=0;k<DatiShape.StrisciaTot;k++) {

		// creare minisel per coprire l'intera larghezza della striscia

		if (DatiShape.TestVerticale==true) {

			DatiShape.MiniSel.left = DatiShape.MiaSelezione.left;
			DatiShape.MiniSel.top = IndiceStriscia + k;
			DatiShape.MiniSel.right =DatiShape.MiaSelezione.right;
			DatiShape.MiniSel.bottom =1;


		}else {
			// orizzontale
			DatiShape.MiniSel.left = IndiceStriscia+k;
			DatiShape.MiniSel.top = DatiShape.MiaSelezione.top;
			DatiShape.MiniSel.right = 1;
			DatiShape.MiniSel.bottom= DatiShape.MiaSelezione.bottom;

		}

		// adesso individuare i corner
		ClickPrima = VetClick[k];
		ClickDopo = VetClick[k+1];
		if (DatiShape.TestCeiling==true) {
			// nel caso di ceiling gli incremnti devono essere empre negativi
			ClickPrima = -ClickPrima;
			ClickDopo = -ClickDopo;

		}


		// ora 
		for (i=0;i<4;i++) VetFloor[i]=ClickPrima;

		if (DatiShape.TestVerticale == true) {
			// verso sud (indice 3 e 0)
			VetFloor[Ruota(0)]= ClickDopo;
			VetFloor[Ruota(3)]=ClickDopo;
		}else
		{
			// orizzontale
			// verso est (indice 2 e 3) o ovest (forzando est)
			VetFloor[Ruota(2)] = ClickDopo;
			VetFloor[Ruota(3)] = ClickDopo;
		}

#ifdef LOG_SHAPE
		sprintf(BufferLog,"ModificaClickSettore():  BaseClick=%d  VetFloor[]=%d %d %d %d",
			BaseClick,VetFloor[0], VetFloor[1], VetFloor[2], VetFloor[3]);
		InviaLog(BufferLog);
#endif
	
		ModificaClickSettore(FTO_SET_FLOOR * ShiftC | FTO_SET_FLOOR_CORNERS * ShiftC,
				false,
				BaseClick,
				VetFloor[0], VetFloor[1], VetFloor[2], VetFloor[3]);
	}

	return true;
}
// curva floor verso l'alto (collina)
void Menu_BendFloor(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaBlend(false,false);
	TerminaShape(true,false);


}
void Menu_BendInvFloor(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaBlend(false,true);
	TerminaShape(true,false);	
}

void Menu_BendCeiling(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaBlend(true,false);
	TerminaShape(true,false);
	
}
void Menu_BendInvCeiling(void *pPunta)
{
	if (IniziaShape(true,true,1,false)==false) return;
	CreaBlend(true,true);
	TerminaShape(true,false);	
}


// permette di impostare la dimensione della selezione 2d
void SetSizeSelezione2d(void)
{
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	char *pTesto;
	char MexNow[50];
	char **pVetParti;
	int TotParti;
	int SizeX;
	int SizeY;
	int OrgX,OrgY;

	if ((*pTestEsisteSelezioneBox) == 0) {
		MostraMessageBoxNGLE("Missing selection.\r\nBefore using this command you have to select at least a square in current room",
				3);
		return;
	}
	sprintf(MexNow,"%dx%d",pRectSelezione2d->right,pRectSelezione2d->bottom);
	OrgX= pRectSelezione2d->left;
	OrgY= pRectSelezione2d->top;

	pTesto= InputTextBox("Type new size for 2d selection.\r\nExample:\r\n3x4\r\n(to select a zone large 3 sectors and height 4 sectors. Please, no space between digits and 'x' sign",
			"Change size of selection", MexNow,TP_LUCI);
	if (pTesto==NULL) return;
	pVetParti = Split(pTesto,'x',&TotParti,NULL,NULL,false);
	if (TotParti != 2) {
		MostraMessageBoxNGLE("Syntax error. You have to type text with NO spaces, in format \"5x2\" (no quotes)",3);
		return;
	}
	SizeX=-1;
	sscanf(pVetParti[0],"%d",&SizeX);
	if (SizeX <1  || (SizeX+OrgX) > 20) {
		MostraMessageBoxNGLE("Out of range for width selection. Valid range between 1 - 20",3);
		return;
	}

	SizeY=-1;
	sscanf(pVetParti[1],"%d",&SizeY);
	if (SizeY <1  || (SizeY+OrgY) > 20) {
		MostraMessageBoxNGLE("Out of range for height selection. Valid range between 1 - 20",3);
		return;
	}

	pRectSelezione2d->right = SizeX;
	pRectSelezione2d->bottom = SizeY;
	RefreshSelezione();

}

void EseguiTestVelocita(void)
{
	WORD *pPosVerticale = (WORD *) 0x56F46E;
	WORD SalvaInizio;
	DWORD SalvaTempo;
	DWORD TempoNow;
	char *pMex = "";
	ShowInfoRoom("Speed Test in progress... (hit SHIFT key to abort)");
	SalvaTempo = GetTickCount();
	SalvaInizio = *pPosVerticale;


	do {
		(*pPosVerticale) += 0x400;
		if (TastoPremuto(VK_SHIFT)==true) {
			pMex = "<BREAK> ";
			break;
		}
		__asm {
			pushad

			mov eax, 401000h    ;Show3d_DaNuovaPosizione
			call eax

	mov eax, 401200h    ;CicloPrincipale
	call eax
	mov eax, 436AEEh    ;ElaboraQualcosaMouse
	call  eax



			popad
		}

		
	}while ((*pPosVerticale) != SalvaInizio);

	TempoNow = GetTickCount()-SalvaTempo;
	sprintf(BufferLog,"%sElapsed Microseconds=%d", pMex, TempoNow);
	MioMessageBox(BufferLog,"Speed Test");


}
// invia un log completo di stanza pRoom
void CreaLogPerRoom(StrStanzaRoomEdit *pRoom)
{
	int i;
	StrGridXRoom *pGriglia;
	int TotCelle;
	int x,y;

	sprintf(BufferLog,"Struttura Room: %s  (0x%X)",pRoom->Nome, pRoom);
	InviaLog(BufferLog);
	InviaLog("----------------------------------------------");

	sprintf(BufferLog,"XSizeBlock=%d   ZSizeBlock=%d",
			pRoom->XSizeBlocks, pRoom->ZSizeBlocks);
	InviaLog(BufferLog);

	sprintf(BufferLog,"yClickTop=%d  yClickBottom=%d", 
			pRoom->OrgYClickTop , pRoom->OrgYClickBottom );
	InviaLog(BufferLog);

	sprintf(BufferLog,"Mistery1=0x%X  Mistery2=0x%X  Mistery3=0x%X",
		pRoom->Mistery1, pRoom->Mistery2, pRoom->Mistery3 );
	InviaLog(BufferLog);

	sprintf(BufferLog,"N_Doors=%d", pRoom->N_Doors);
	InviaLog(BufferLog);

	sprintf(BufferLog,"N_Effects=%d", pRoom->N_Doors);
	InviaLog(BufferLog);

	sprintf(BufferLog,"N_ObjAndTriggers=%d", pRoom->N_ObjAndTriggers);
	InviaLog(BufferLog);

	sprintf(BufferLog,"LinkedRoom=%d",pRoom->LinkedRoom);
	InviaLog(BufferLog);

	sprintf(BufferLog,"IndexFlipRoom=%d", pRoom->IndexFlipRoom);
	InviaLog(BufferLog);

	sprintf(BufferLog,"RoomButtons=0x%X", pRoom->RoomButtons);
	InviaLog(BufferLog);

	sprintf(BufferLog,"FlipMapValue=0x%X", pRoom->FlipMapValue);
	InviaLog(BufferLog);

	pGriglia = pRoom->pBlockGridCollision;
	TotCelle = pRoom->XSizeBlocks * pRoom->ZSizeBlocks;
	InviaLog("SectorGrid:");

	for (i=0;i<TotCelle;i++) {
		y = i / pRoom->XSizeBlocks;
		x = i % pRoom->XSizeBlocks;

		sprintf(BufferLog,"\t%d) Grid[%d,%d] (0x%X)", i,x,y, &pGriglia[i]);
		InviaLog(BufferLog);

		sprintf(BufferLog,"\t\tType=0x%X", pGriglia[i].Type);
		InviaLog(BufferLog);

		sprintf(BufferLog,"\t\tFlag1=0x%X", pGriglia[i].Flag1);
		InviaLog(BufferLog);

		sprintf(BufferLog,"\t\tFloorClick=%d   CeilingClick=%d",
			pGriglia[i].WorldFloorClick, pGriglia[i].WorldYCeilingClick);
		InviaLog(BufferLog);
	}
	InviaLog("---------------------------------------");

}
// mette nel log file una descrizione di stanza attuale
void ShowLogRoomNow(void)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;

	CreaLogPerRoom(&pVetRooms[*pIndiceRoomAttuale]);

}

// mette nel log una descrizione dettagliata di griglia di stanza attuale
void ShowLogGrigliaNow(void)
{
	
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrStanzaRoomEdit *pRoom;

	pVetRooms = *p2VetRooms;

	pRoom = &pVetRooms[*pIndiceRoomAttuale];


	// impostare come selezione l'intera stanza
	DatiShape.MiniSel.left = 0;
	DatiShape.MiniSel.top = 0;
	DatiShape.MiniSel.right = pRoom->XSizeBlocks;
	DatiShape.MiniSel.bottom = pRoom->ZSizeBlocks;

	ModificaClickSettore(FTO_SHOW_INFOGRID,false);

}

// analizza le tail info di tutto il progetto e rimuove quelle che hanno
// indice maggiore di TotTailInfos
// in piu' crea un file di log  TailInfo_log.txt

void TrovaTailInfoErrate(void)
{

	int* pN_TextureTails = (int*) 0x56C4F4;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	int x,y,i,j;
	int TotX,TotY;
	StrGridXRoom *pGriglia;
	FILE *pFile;
	int TotErrori;
	int Indice;

	if (MostraMessageBoxNGLE("Current command find misplaced texture tail infos and remove them. At end it will be created also a log file named \"TailInfo_log.txt\"",0x0f) == 0) return;

	pVetRooms = *p2VetRooms;
	TotErrori=0;
	pFile=fopen(GetFileTemp("TailInfo_log.txt"),"wb");
	fprintf(pFile,"Room  Sector\r\n");
	fprintf(pFile,"----------------------\r\n");

	// ora scandire tutte le stanze, tutti i settori e tutte le texinfos
	// e registrare tutte le tex usate
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {
			// ora scandire tutta la griglia dei settori
			TotX = pVetRooms[i].XSizeBlocks;
			TotY = pVetRooms[i].ZSizeBlocks;
			
			for (x = 0;x<TotX;x++) {
				for (y=0;y<TotY;y++) {
					pGriglia = &pVetRooms[i].pBlockGridCollision[x+y*TotX];

					for (j=0;j<14;j++) {

						if (pGriglia->TextureInfos[j].Status == 7) {
							Indice = pGriglia->TextureInfos[j].IndexAndFlags & 0x3ff;
							if (Indice >= *pN_TextureTails) {
								// trovata una tail info sbagliata
								TotErrori++;
								fprintf(pFile," %-2d   (%d,%d)\r\n", i, x,y);
								pGriglia->TextureInfos[j].IndexAndFlags=0;
								pGriglia->TextureInfos[j].Status =0;


							}
						}
						
					}

				}
			}
		}
	}
	fprintf(pFile,"----------------------\r\n");
	fclose(pFile);
	if (TotErrori==0) {
		MostraMessageBoxNGLE("No problem found with tail infos",0);
		return;
	}
	sprintf(BufferLog,"Found and removed %d misplaced tail infos",TotErrori);
	MostraMessageBoxNGLE(BufferLog,0);

	ShellExecute(NULL,"open",GetFileTemp("TailInfo_log.txt"),"","",SW_SHOW);
}

__declspec(naked) void PatchPerStack74(void)
{
	__asm {

		mov  dword ptr [566680h][eax*4], ebx ;VetPtrTextureExport
		lea		edi, VetStack_74
		mov		word ptr [edi][eax*2], bx
		retn
	}
	
}

// iniza procedura di find and replace di texture
void FindAndReplaceTex(void)
{
	char *pMex;
	WORD *pIndiceTextureSelezionata = (WORD*) 0x583BBA;
	char BufferTemp[256];
	int Numero;
	int SizeTex;
	int Valore;
	int *pMask1 = (int *) 0x0042A1E1;
	int *pMask2 = (int *) 0x0042A1EF;
	int Mask;
	// prima trovare indice di texture da trovare 
	// (quella attualmente selezionata)

	DatiFindReplace.IndiceFind = 
		TrovaIndiceSelTail(*pIndiceTextureSelezionata);	
	sprintf(BufferTemp, "Type the index of the texture that will be used to replace current selected texture in texture panel (Index=%d)",
		DatiFindReplace.IndiceFind);

	pMex = InputTextBox(BufferTemp,	"Find and Replace Textures",
						"",TP_CENTRALE);
	if (pMex==NULL) return;

	Numero=-1;
	sscanf(pMex,"%d",&Numero);
	if (Numero == -1 || Numero >=256) {
		MostraMessageBoxNGLE("ERROR: invalid texture index (valid range=  0 - 255)",3);
		return;
	}
	// impostare i valori corretti per ricerca di texture
	// parziali, basato sulla dimensione di default delle texture
	// di progetto attuale
	SizeTex =TrovaSizeDefaultTexture();
	SizeTex--;
	Mask = ~SizeTex;
	Valore = Mask << 8;
	Mask &= 0xff;
	Valore |= Mask;

	*pMask1 = Valore;
	*pMask2 = Valore;

	DatiFindReplace.IndiceReplace = Numero;
	DatiFindReplace.TotTrovate=0;
	DatiFindReplace.TestAttivo=true;
	Sub_FindTexture(0);
	DatiFindReplace.TestAttivo=false;

}
// verifica quante talinfo (da numero 256 in poi) sono inutilizzate
void TrovaUnusedTailInfo(void)
{
	StrTailInfo** p2VetRoomTails = (StrTailInfo**) 0x56C4E4;
	StrTailInfo * pVetRoomTails;
	int* pN_TextureTails = (int*) 0x56C4F4;
	BYTE VetUsata[MAX_TAIL_INFOS];
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	int i;
	int TotX,TotY;
	int x,y;
	int j;
	int Indice;
	int TotFree;
	StrGridXRoom *pGriglia;
	int VetOldIndice[MAX_TAIL_INFOS];
	int VetNewIndice[MAX_TAIL_INFOS];
	int Numero;
	int TotTails;
	int **p2VetTextureNero = (int**) 0x56C4FC;
	int *VetTextureNero;
	int k;
	int VetFree[MAX_TAIL_INFOS];
	int TotCambi;
	int IndiceOld;
	int IndiceNew;
	int VetIndici[MAX_TAIL_INFOS];
	int * pVetPtrTextureExport = (int*) 0x566680;

	pVetRoomTails = *p2VetRoomTails;
	pVetRooms = *p2VetRooms;
	VetTextureNero = *p2VetTextureNero;
	// prima azzerare il valore usato
	for (i=0;i<*pN_TextureTails;i++) VetUsata[i]=0;

	// ora scandire tutte le stanze, tutti i settori e tutte le texinfos
	// e registrare tutte le tex usate
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {
			// ora scandire tutta la griglia dei settori
			TotX = pVetRooms[i].XSizeBlocks;
			TotY = pVetRooms[i].ZSizeBlocks;
			
			for (x = 0;x<TotX;x++) {
				for (y=0;y<TotY;y++) {
					pGriglia = &pVetRooms[i].pBlockGridCollision[x+y*TotX];

					for (j=0;j<14;j++) {

						if (pGriglia->TextureInfos[j].Status == 7) {
							Indice = pGriglia->TextureInfos[j].IndexAndFlags & 0x3ff;
							VetUsata[Indice]=1;
						}
						
					}

				}
			}
		}
	}

	// ora contare le tail infos SOLO per inidici da 256
	TotFree=0;
	for (i=256;i<*pN_TextureTails;i++) {
		if (VetUsata[i] == 0) {
			VetFree[TotFree] = i;
			TotFree++;
		}
	}
#ifdef LOG_COMMANDS
	sprintf(BufferLog, "VetFree(%d)", TotFree);
	InviaLog(BufferLog);
	for (i=0;i<TotFree;i++) {
		sprintf(BufferLog,"\t%d) 0x%X", i, VetFree[i]);
		InviaLog(BufferLog);
	}

#endif
	if (TotFree == 0) {
		MostraMessageBoxNGLE("Found no unused tail info",0);
		return;
	}

	sprintf(BufferLog,"You can free %d tail infos.\r\nDo you want remove unused tail infos?",
					TotFree);
	if (MostraMessageBoxNGLE(BufferLog,0x0f)==0) return;

	ApriFinestraWait("Removing unused tail infos in progress... wait please");

	// ora bisogna spostare le tail
	// prima di tutto ordinarle per numero decrescente
	
	for (i=0;i<TotFree-1;i++) {
		for (j=i+1;j<TotFree;j++) {
			if (VetFree[i] < VetFree[j]) {
				// scambiare
				Numero = VetFree[i];
				VetFree[i] = VetFree[j];
				VetFree[j] = Numero;
			}
		}
	}
#ifdef LOG_COMMANDS
	sprintf(BufferLog, "VetFree(%d)  (Dopo ordinamento)", TotFree);
	InviaLog(BufferLog);
	for (i=0;i<TotFree;i++) {
		sprintf(BufferLog,"\t%d) 0x%X", i, VetFree[i]);
		InviaLog(BufferLog);
	}

#endif
	// adesso ridurre roomvettail e vetbigtail 
	TotTails = *pN_TextureTails;

	Numero=TotTails;
	// salvare sequenza oriigina e poi invertirla in modo
	// da seguire tutti i cambi
	for (i=256;i<TotTails;i++) {
		VetIndici[i] = i;
	}

	for (i=0;i<TotFree;i++) {
		IndiceNew = VetFree[i];
		// inserire in record indice i dati ultima tail info attuale
		
		IndiceOld = TotTails-1;

		// adesso spostare i record
		memcpy(&pVetRoomTails[IndiceNew],&pVetRoomTails[IndiceOld],
									sizeof(StrTailInfo));
		memcpy(&VetBigTails[IndiceNew],&VetBigTails[IndiceOld], 
									sizeof(StrTailInfo));
		VetTextureNero[IndiceNew] = VetTextureNero[IndiceOld];
		VetIndici[IndiceNew] = VetIndici[IndiceOld];
		pVetPtrTextureExport[IndiceNew] = pVetPtrTextureExport[IndiceOld];

#ifdef LOG_COMMANDS
		sprintf(BufferLog,"Old=0x%X   New=0x%X", IndiceOld,IndiceNew);
		InviaLog(BufferLog);
#endif
		TotTails--;
	}
#ifdef LOG_COMMANDS
	InviaLog("Analisi cambia di indici:");
#endif

	TotCambi=0;
	for (i = 256;i<TotTails;i++) {
		if (i != VetIndici[i]) {
			// trovato un cambio
			VetNewIndice[TotCambi] = i;
			VetOldIndice[TotCambi] = VetIndici[i];
#ifdef LOG_COMMANDS
			sprintf(BufferLog,"\t%d) %d (0x%X)", i, VetIndici[i], VetIndici[i]);
			InviaLog(BufferLog);
#endif
			TotCambi++;
		}
	}

	// ora frugare in tutte le stanze e sostituire il vecchio idnice old
	// con quello nuovo
	// ora scandire tutte le stanze, tutti i settori e tutte le texinfos
	// e registrare tutte le tex usate
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {
			// ora scandire tutta la griglia dei settori
			TotX = pVetRooms[i].XSizeBlocks;
			TotY = pVetRooms[i].ZSizeBlocks;

	
			for (x = 0;x<TotX;x++) {
				for (y=0;y<TotY;y++) {
					pGriglia = &pVetRooms[i].pBlockGridCollision[x+y*TotX];
					for (j=0;j<14;j++) {
						if (pGriglia->TextureInfos[j].Status == 7) {
							Indice = pGriglia->TextureInfos[j].IndexAndFlags & 0x3ff;
							// se questo indice corisponde ad uno di quelli old
							// sostituirlo con quello new
							for (k=0;k<TotCambi;k++) {
								if (Indice == VetOldIndice[k]) {
#ifdef LOG_COMMANDS
									sprintf(BufferLog,"Originale=0x%X  New=0x%X  (Room=%d Griglia(%d,%d) TextureInfo[%d]",
											Indice, VetNewIndice[k], i, x,y,j);
									InviaLog(BufferLog);
#endif
									pGriglia->TextureInfos[j].IndexAndFlags 
												&= ~0x3ff;
									pGriglia->TextureInfos[j].IndexAndFlags 
												|= VetNewIndice[k];
									break;
								}

							}
							if (k == TotCambi) {
								// non c'e' stato alcun cambio
#ifdef LOG_COMMANDS
								if (Indice >= *pN_TextureTails) {
									// errore,
									sprintf(BufferLog, "ERRORE: c'e' un indice tail info (%d) che non e' stata sostituita e che e' maggiore di totale tail info in stanza = %d  Griglia= (%d,%d) TextureInfo[%d]",
										Indice,i,x,y,j);
									InviaLog(BufferLog);
								}
#endif
							}
						}
						
					}
				}
			}
		}
	}
	*pN_TextureTails =TotTails;
	//ResetAnimazioni();
	ChiudiFinestraWait();
	sprintf(BufferLog,"Removed %d unused tail infos", TotFree);
	MostraMessageBoxNGLE(BufferLog,0);



}
// restituisce il numero di click di dove e' raggio, assunto che la base
// della calotta sia su floor 0
// imposta anche la lunghezza complessiva del raggio in pRaggio
// se il valore restituito e' >= 0 allora non si deve proseguire
// Input: 
//		Lato = ampiezza della selezione (in settori)
//		h    = Altezza massima attuale in click da raggiungere di cupola

double TrovaRaggioLatoY(double Lato, double h, double *pRaggio)
{
	double Quadro1, Quadro2, D;
	
	// convertire il lato da settori a click (moltiplicare per 4)
	D = (Lato * 4.0) / 2.0;

	Quadro1 = D * D;
	Quadro2 = h*h;
	*pRaggio = (Quadro1 + Quadro2) / (2.0 * h);

	return h - *pRaggio;

}
// restituisce il numero di click di alteza della coordinata x,z
// lavorando sulla sfera con centro Centro e di raggion R
// note:
//	1) Tutto il calcolo e' basato allienando la base della cupola al piano y=0
//	   e la griglia della sfera parte sempre da x=0,z=0
//  2) Se il valore restituito e' zero vuol dire che non fa parte della calotta
//  3) I valori x e z defono riferirsi a settori (poi saranno convertiti in click
//    da questa funzione

int TrovaClickSfera(double x, double z, double R) 
{
	double c, Delta, b,y;
	x *= 4;
	z *= 4;

	// ora calcolo lungo di dati
	c = pow(R,2) - pow(x-Centro.x,2) - pow(z- Centro.z,2);
	
	b = Centro.y;
	c = c-  pow(b,2);
	b= -b *2;
	c=-c;

	Delta = pow(b,2) - 4.0 * c;

	
	if (Delta < 0) return 0;

	if (Delta > 0) {
		y = (-b + sqrt(Delta)) / 2.0;
		if (y > 0) return Float2Int((float) y);

		y = (-b - sqrt(Delta)) / 2.0;
		if (y > 0) return Float2Int((float) y);
		return 0;
	}

	// delta = 0, c'e' una sola soluzione
	y = -b / 2;
	if (y < 0) y = 0;
	return  Float2Int((float) y);

}

void CreaCupola(bool TestCeiling, bool TestInversa)
{
	// 
	double Lato,Raggio;
	double h;
	int BaseFloor;

	int MaxFloor;
	int Numero;
	int x,z;
	int MaxCupola;
	char VetCorners[4];
	bool TestUguale;
	RECT SelLarga;
	int ShiftC;
	int SizeX,SizeY;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int OldDifferenza, Massimo;
	int *pValore;
	bool TestParziale;
	double MaxLato;
	int nx,ny;
	int MinLato;

#ifdef LOG_SHAPE
	char BufferTemp[256];
#endif

	BYTE  GrigliaClick[40][40];


	if (IniziaShape(true,true,3,false)==false) return;
	if (DatiShape.DirezioneFreccia == 0) {
		TestParziale=false;

		// se non e' quadrata uscire
		if (DatiShape.MiaSelezione.bottom != DatiShape.MiaSelezione.right) {
			sprintf(BufferLog,"ERROR: current selected zone is not a perfect square. This command requires a square selection. The size of two sides is different (%d x %d)\r\n\r\nIf you have problems to select a square use the command SHIFT F5 to set the size of selection.",
				DatiShape.MiaSelezione.right, DatiShape.MiaSelezione.bottom);
			MostraMessageBoxNGLE(BufferLog,3);
			TerminaShape(true,false);
			return;
		}
		MaxLato= DatiShape.MiaSelezione.right;
	}else {
		// parziale
		TestParziale=true;
		// trovare lato massimo
		if (DatiShape.MiaSelezione.right >= DatiShape.MiaSelezione.bottom) 
			MaxLato = DatiShape.MiaSelezione.right;
		else
			MaxLato = DatiShape.MiaSelezione.bottom;
		if (DatiShape.DirezioneFreccia > 4) {
			// freccie diagonali
			MaxLato=MaxLato*2;
		}
	}

	if (TestCeiling == true) {
		DatiShape.ShiftCeiling = 2;
	}else
	{
		DatiShape.ShiftCeiling = 1;
	}


	ShiftC= DatiShape.ShiftCeiling;

	DatiShape.TestCeiling = TestCeiling;
	// prendere base floor da valore floor di primo riquadro a sinistra
	// di selezione
	CopiaRect(&DatiShape.MiniSel,&DatiShape.MiaSelezione);

	// impostare il minimo
	pValore = (int*) ModificaClickSettore( 
			FTO_GET_MIN_FLOOR * DatiShape.ShiftCeiling,false);
	BaseFloor = *pValore;

	// ora trovare valore massimo attuale
	pValore = (int*) ModificaClickSettore(
			FTO_GET_MAX_FLOOR * DatiShape.ShiftCeiling,false);
	MaxFloor = *pValore;
#ifdef LOG_SHAPE
	sprintf(BufferLog,"BaseFloor=%d  MaxFloor=%d",
		BaseFloor,MaxFloor);
	InviaLog(BufferLog);
#endif

	
	if (TestInversa==true) {
		// invertire i due valori
		Numero = BaseFloor;
		BaseFloor = MaxFloor;
		MaxFloor= Numero;
#ifdef LOG_SHAPE
		InviaLog("Invertire basefloor e maxfloor");
#endif

	}



	// ora calcolare differenza (ignorando segni)
	h = abs(MaxFloor-BaseFloor);
	OldDifferenza = (int) h;

	do {

		// ora aumentarlo di 1 click
		h++;

		// trovare centro della sfera
		
		Lato = MaxLato;
		SizeX = (int) Lato;
		SizeY = (int) Lato;
		Centro.x = (double) (Lato * 4.0) / 2.0;
		Centro.y = TrovaRaggioLatoY(Lato,h, &Raggio);
		Centro.z = (double) (Lato * 4.0) / 2,0;
		// se centro.y < 0 zero, ok altrimenti non eseguire niente
		if (Centro.y >= 0) {
			// fare un beep
			MessageBeep(-1);
			TerminaShape(true,false);
			return;
		}

		// ok, ora ho raggio e centro della sfera
		// rimempire griglia di riferimento con i click (sempre positivi)
		Massimo = -1;

		for (z = 0;z <= Lato ;z++) {
			BufferLog[0]=0;
			for (x=0;x<=Lato;x++) {
				GrigliaClick[x][z] = (BYTE) TrovaClickSfera(x,z,Raggio);
				if (GrigliaClick[x][z] > Massimo) 
					  Massimo = GrigliaClick[x][z];
				
#ifdef LOG_SHAPE
				sprintf(BufferTemp,"%2d ",GrigliaClick[x][z]);
				strcat(BufferLog,BufferTemp);
#endif

			}
#ifdef LOG_SHAPE
			InviaLog(BufferLog);
#endif
		}

	}while (Massimo == OldDifferenza);


	// fare modifiche a griglia e basefloor a seconda se diretta o inversa

	// prima trovare valore massimo cupola
	MaxCupola = (int) h;  // dovrebbe sempre essere giusto 


	// forse sia che sia floor che ceiling l'unica cosa che cambia
	// e' l'inversa
	
	if (TestInversa==true) {
		// floor inverso. Il BaseFloor deve essere abbassato
		// della massima profondita' della nuova cupola
		
		BaseFloor -= MaxCupola;
		// ora modificare tutti i click in modo inverso a maxcupola
		for (z=0;z<=Lato;z++) {
			for (x=0;x<=Lato;x++) {
				GrigliaClick[x][z] = MaxCupola - GrigliaClick[x][z];
			}
		}
		

	}
	

	// procedura per salvar einformazioni texture
	// salva selezione in sellarga
	
	CopiaRect(&SelLarga,&DatiShape.MiaSelezione);



	// adesso vedere se e' necessario dati texture adesso oppure
	// lasciare quella precedente
	
	TestUguale=true;
	if (DatiShape.TestCeiling == false ) {
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneFloor.left || 
			SelLarga.top != BaseRotazioni.SelezioneFloor.top ||
			SelLarga.right != BaseRotazioni.SelezioneFloor.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneFloor.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;
	}else {
		// confronto dati ceiling
		// confrontare dati di selezione floor
		if (SelLarga.left != BaseRotazioni.SelezioneCeiling.left || 
			SelLarga.top != BaseRotazioni.SelezioneCeiling.top ||
			SelLarga.right != BaseRotazioni.SelezioneCeiling.right ||
			SelLarga.bottom != BaseRotazioni.SelezioneCeiling.bottom) TestUguale=false;
		if (BaseRotazioni.IndiceRoom != GetIndiceRoom(-1)) TestUguale=false;

	}

	if (TestUguale==false) {
		// bisogna creare adesso tabella
		CopiaRect(&DatiShape.MiniSel,&SelLarga);
		ModificaClickSettore(FTO_SET_TABELLA_ROTAZIONE_FLOOR*ShiftC,false);
	}
	// impostare sfasamenti nel caso di sfera parziale
	nx=0;
	ny=0;
	if (TestParziale==true) {
		// calcolare incremento a origine di griglia
		// per compensare diffrente larghezza
		if (MaxLato > DatiShape.MiaSelezione.right) {
			// il lato x e' il piu' corto
			MinLato = DatiShape.MiaSelezione.right;
			// a seconda di freccia (ortogonale) 
			// stabilire nx
			switch (DatiShape.DirezioneFreccia) {
			case 1:
				// parte da sinistra
				nx=0;
				break;
			case 3:
				// parte da destra
				nx = (int) MaxLato-MinLato;
				break;
			case 5:
				// direzione nord-ovest
				nx=0;
				ny=0;
				break;
			case 6:
				// direzione nord-est
				nx = (int) MaxLato - DatiShape.MiaSelezione.right;
				break;
			case 7:
				// direzione sud-est
				nx = (int) MaxLato-DatiShape.MiaSelezione.right;
				ny = (int) MaxLato-DatiShape.MiaSelezione.bottom;
				break;
			case 8:
				// direzione sud-ovest
				nx =0;
				ny = (int) MaxLato-DatiShape.MiaSelezione.bottom;
				break;
			default:
				// altre freccie, usa porzione centrale
				nx = ((int) MaxLato - MinLato) / 2;
				break;
			}
		}else {
			// il lato z e' il piu' corto
			MinLato = DatiShape.MiaSelezione.bottom;
			switch (DatiShape.DirezioneFreccia) {
			case 2:
				// verso l'alto
				ny =0;
				break;
			case 4:
				// verso il basso
				ny = (int) MaxLato - MinLato;
				break;
			default:
				ny = ((int) MaxLato - MinLato) / 2;
				break;
			}
		}
	}


	// ora applicare griglia
	for (z=0;z<DatiShape.MiaSelezione.bottom ;z++) {
		for (x=0;x<DatiShape.MiaSelezione.right ;x++) {
			// impostare selezione su cui lavorare
			DatiShape.MiniSel.left = DatiShape.MiaSelezione.left + x;
			DatiShape.MiniSel.top  = DatiShape.MiaSelezione.top + z;
			DatiShape.MiniSel.right =1;
			DatiShape.MiniSel.bottom =1;
			// ora impostare gli angoli per questo settore



			// metterli in VetCorners[] aggiungendo basefloor
			// in basso a sinistra
			VetCorners[Ruota(0)] = GrigliaClick[x+nx][z+ny+1];

			// in alto a sinistra
			VetCorners[Ruota(1)] = GrigliaClick[x+nx][z+ny];
			
			// in alto a destra
			VetCorners[Ruota(2)] = GrigliaClick[x+nx+1][z+ny];

			// in basso a destra
			VetCorners[Ruota(3)] = GrigliaClick[x+nx+1][z+ny+1];


			// ok, ora impostare tutto


			ModificaClickSettore(FTO_SET_FLOOR * DatiShape.ShiftCeiling |
								 FTO_SET_FLOOR_CORNERS * DatiShape.ShiftCeiling,
								 false,
								 BaseFloor,
								 VetCorners[0], VetCorners[1], VetCorners[2], VetCorners[3]);
	

		}
	}

	


	// ripristinare texture
	CopiaRect(&DatiShape.MiniSel,&SelLarga);
#ifdef LOG_SHAPE
	InviaLog("Ripristina texture dopo CreaCupola()");
#endif
	ModificaClickSettore(FTO_SET_TRIANGLE_TEXTURE_FLOOR * ShiftC | 
						 FTO_USE_TABELLA_ROTAZIONE_FLOOR * ShiftC,false);
	


	TerminaShape(true,false);
}

// crea una nuova stanza con la stessa larghezza di quella attuale
// e la posizione sopra (TestSopra==true) o sotto
// chiede il numero di click da assegnare alla stanza
void AggiungiStanzaVerticale(bool TestSopra)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int StanzaOld;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;	
	int i;
	int StanzaNew;
	char *pTesto;
	int OldAltezza;
	int NewAltezza;
	char BufferTemp[256];
	int *pTotRooms = (int*) 0x56C710;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int *pTipoFrecciaSelezione = (int *) 0x583BB6;
	int ClickFloor, ClickCeiling;
	bool TestDoor;
	char *pTitolo;
	bool TestFlippata;

	pVetRooms = *p2VetRooms;
	StanzaOld = *pIndiceRoomAttuale;
	OldAltezza = abs(pVetRooms[StanzaOld].OrgYClickTop - 
						pVetRooms[StanzaOld].OrgYClickBottom);

	if (pVetRooms[GetIndiceRoom(-1)].RoomButtons & 0x02) 
		TestFlippata=true;
	else
		TestFlippata=false;


	sprintf(BufferLog,"%d",OldAltezza);
	if (TestSopra==true) 
		pTitolo = "upstairs";
	else
		pTitolo = "downstairs";

	sprintf(BufferTemp,"Creation of stacked %s room", pTitolo);

	pTesto = InputTextBox("Type height of new room in clicks\r\n\r\nRemark: by default the new room will be linked with a vertical portal (door) at current room. If you don't want create this portal, you can type the height of new room as a negative value. \r\n(Example: type \"-8\" to have a new room with height 8 clicks without creating any door)",
					BufferTemp,BufferLog,TP_CENTRALE);
	if (pTesto==NULL) return;
	
	NewAltezza=0;
	sscanf(pTesto,"%d",&NewAltezza);
	if (NewAltezza==0) {
		MostraMessageBoxNGLE("Illegal value",3);
		return;
	}

	if (NewAltezza < 0) {
		NewAltezza = -NewAltezza;
		TestDoor=false;
	}else {
		TestDoor=true;
	}
	
	// ora cercare una stanza vuota
	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot) break;
	}

	if (i == *pTotRooms) {
		MostraMessageBoxNGLE("Cann't find an empty slot room for new room to add",3);
		return;
	}
	StanzaNew=i;
	MostraNuovaStanza(StanzaNew);

	// ora fare un bound in modo da avere la dimensione x e z corretta
	// prima impostare selezione ai valori desiderati
	pRectSelezione2d->left  = pVetRooms[StanzaOld].XDisplayOffset+1;
	pRectSelezione2d->top = pVetRooms[StanzaOld].ZDisaplyOffset+1;
	pRectSelezione2d->right = pVetRooms[StanzaOld].XSizeBlocks-2;
	pRectSelezione2d->bottom = pVetRooms[StanzaOld].ZSizeBlocks-2;
	*pTestEsisteSelezioneBox =1;
	*pTipoFrecciaSelezione=0;
	roomBoundRoom(NULL);
	


	// adesso cambiare dimensione e posizione
	ClickFloor = pVetRooms[StanzaOld].OrgYClickBottom;
	ClickCeiling = pVetRooms[StanzaOld].OrgYClickTop;

	if (TestSopra==true) {
		pVetRooms[StanzaNew].OrgYClickBottom  = ClickCeiling;
		pVetRooms[StanzaNew].OrgYClickTop = ClickCeiling + NewAltezza;
		
		
		
	}else
	{
		// stanza sotto
		pVetRooms[StanzaNew].OrgYClickTop = ClickFloor;
		pVetRooms[StanzaNew].OrgYClickBottom = ClickFloor - NewAltezza;
	}

	ClickFloor = pVetRooms[StanzaNew].OrgYClickBottom;
	ClickCeiling = pVetRooms[StanzaNew].OrgYClickTop;


	pVetRooms[StanzaNew].XBlocksWorld = pVetRooms[StanzaOld].XBlocksWorld;
	pVetRooms[StanzaNew].XDisplayOffset = pVetRooms[StanzaOld].XDisplayOffset;
	pVetRooms[StanzaNew].XPixelWorld = pVetRooms[StanzaOld].XPixelWorld;
	pVetRooms[StanzaNew].ZBlocksWorld = pVetRooms[StanzaOld].ZBlocksWorld;
	pVetRooms[StanzaNew].ZDisaplyOffset = pVetRooms[StanzaOld].ZDisaplyOffset;
	pVetRooms[StanzaNew].ZOriginePixel = pVetRooms[StanzaOld].ZOriginePixel;
	pVetRooms[StanzaNew].ZSizeBlocks = pVetRooms[StanzaOld].ZSizeBlocks;

	// ora bisogna aggiornare tutti i valori della griglia
	MostraNuovaStanza(StanzaNew);

	DatiShape.TestCeiling =false;
	SetMiniSel(0,0);
	DatiShape.MiniSel.right = pVetRooms[StanzaNew].XSizeBlocks;
	DatiShape.MiniSel.bottom  = pVetRooms[StanzaNew].ZSizeBlocks;

	ModificaClickSettore(FTO_SET_FLOOR,false,ClickFloor);
	ModificaClickSettore(FTO_SET_CEILING, false,ClickCeiling);

	MostraNuovaStanza(StanzaNew);
	pVetRooms[StanzaNew].AmbientLightColor = pVetRooms[StanzaOld].AmbientLightColor;

	// se siamo in modo flippato, convertire nuova stanza 
	// in mdo che sia flippata
	if (TestFlippata == true) {
		pVetRooms[StanzaNew].FlipMapValue = pVetRooms[StanzaOld].FlipMapValue;
		roomCreaFlipMap(NULL);
	}
	if (TestDoor==false) return;

	// adesso vedere se bisogna posizionarsi su stanza sopra o sotto
	if (TestSopra == false) {
		// la stanza creata era sotto, quindi bisogna posizionarsi su stanzaold
		MostraNuovaStanza(GetIndiceRoom(StanzaOld));
	}
	// adesso selezionare tutto il pavimento (e solo il pavimento)
	pRectSelezione2d->left  = pVetRooms[StanzaOld].XDisplayOffset+1;
	pRectSelezione2d->top = pVetRooms[StanzaOld].ZDisaplyOffset+1;
	pRectSelezione2d->right = pVetRooms[StanzaOld].XSizeBlocks-2;
	pRectSelezione2d->bottom = pVetRooms[StanzaOld].ZSizeBlocks-2;
	*pTestEsisteSelezioneBox =1;
	*pTipoFrecciaSelezione=0;
	// adesso chiamare creadoor
	roomCreaDoor(NULL);
	TotCambiBackup++;

}
// mette messaggio in log
void ShowDebugMex(void)
{
	int *pOrgX = (int*) 0x563F68;
	int *pOrgY = (int*) 0x563F6C;
	
	sprintf(BufferLog,"x=0x%X  y=0x%X", *pOrgX,*pOrgY);
	InviaLog(BufferLog);

	 
}
void InvertiShowInfoTitle(void)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	if (TestShowTitleInfo == true) {
		TestShowTitleInfo=false;
		// mostra titolo
		SetWindowText(*pMiaWind, LastTitleWindow);
	}else {
		TestShowTitleInfo =true;
		// mostra info room
		SetWindowText(*pMiaWind, LastInfoRoom);
	}
		
}

void AumentaNumeroSlot(void)
{
	char *pMex;
	int *pTotRooms = (int*) 0x56C710;	
	int Max;
	char MexDefault[256];
	int NewTot;
	int TotRoom;

	TotRoom = *pTotRooms;

	Max = TotRoom + 1;
	if (Max > 255) return;

	sprintf(BufferLog, "Type new number of room slot. \r\nMax value = 255\r\nCurrent number = %d",
		TotRoom);

	sprintf(MexDefault, "%d", Max);

	pMex = InputTextBox(BufferLog,"Set number of room slots", 
			MexDefault,TP_CENTRALE);
	if (pMex == NULL) return;

	NewTot = -1;
	sscanf(pMex,"%d",&NewTot);

	if (NewTot <= TotRoom || NewTot > 255) {
		MostraMessageBoxNGLE("Value out of range",3);
		return;
	}

	__asm {
		pushad

		lea  eax, NewTot   
		mov  eax, dword ptr [eax]

		mov edx, 46B850h    ;Ptr_VetRoomSlots
		mov  edx, dword ptr [edx]
		mov  ecx, eax
		push ebx
		mov ebx, 56C710h   ;Tot_RoomSlots
		mov  dword ptr [ebx], eax
		pop ebx
		shl  ecx, 05h
		sub  ecx, eax
		lea  eax, [ecx][ecx*2]
		lea  ecx, [eax][eax*4]
		push ecx
		push edx
		mov eax, 449B47h    ;ReallocMemory
		call eax
		add esp,8


		mov edx, 46B854h    ;Ptr_VetPointerRooms
		mov  edx, dword ptr [edx]
		push ebx
		mov ebx, 46B850h   ;Ptr_VetRoomSlots
		mov  dword ptr [ebx], eax
		pop ebx
		mov eax, 56C710h    ;Tot_RoomSlots
		mov  eax, dword ptr [eax]
		lea  ecx, dword ptr [00h][eax*4]
		push ecx
		push edx
		mov eax, 449B47h    ;ReallocMemory
		call eax
		add esp,8 

		push ebx
		mov ebx, 46B854h   ;Ptr_VetPointerRooms
		mov  dword ptr [ebx], eax
		pop ebx
		mov eax, 56C710h    ;Tot_RoomSlots (nuovo numero)
		mov  eax, dword ptr [eax]
		push eax
		lea  eax, TotRoom ; vecchio numero
		mov eax, dword ptr [eax]
		push eax
		mov eax, 425830h    ;InizializzaTuttiSlotRoomVuoti
		call eax
		add  esp,8

		popad

	}

	// adesso bisogna visualizzare stanza 0
	MostraNuovaStanza(0);

}

void ApriFileTga(void)
{
	char *pNomeFileTga = (char*) 0x46BA78;
	HWND *pMiaWind = (HWND*) 0x46E559;


	if (pNomeFileTga[0]==0 || EsisteFile(pNomeFileTga)==false) {
		MostraMessageBoxNGLE("No tga file loaded", 7);
		return;
	}

	if ((MyPrefRoom.Flags & FPR_NO_MINIMIZE)==0) ShowWindow(*pMiaWind,SW_MINIMIZE);

	ShellExecute(NULL, "open", pNomeFileTga, "", SoloDir(pNomeFileTga), SW_SHOW);

}

void ReloadTgaFile(void)
{
	char *pNomeFileTga = (char*) 0x46BA78;

	if (pNomeFileTga[0]==0 || EsisteFile(pNomeFileTga)==false) {
		MostraMessageBoxNGLE("No tga file loaded", 7);
		return;
	}

	LoadTGAFile();

}
void LeggeComandiTasti(void)
{	
	DWORD *pTextureTipoTriangolo = (DWORD *) 0x583BF4;
	DWORD Valore;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	HWND *pMiaWind = (HWND*) 0x46E559;
	int StanzaNow;
	int IndiceProject;
	int i;
	
	if (TestIgnoraInput == true)  return;
	TestIgnoraInput=true;

	// CTRL + S : Messaggio nel log
	if (TastoPremutoNGLE(0x1d, 0x1f)==true) {
		InviaLog("================ USER HIT CTRL+S KEYS ==================");
	}
	// CTRL + T : Cambia triangolo texture
	if (TastoPremutoNGLE(0x1d, 0x14) == true) {
		// cambiare triangolo
		Valore = *pTextureTipoTriangolo;
		Valore++;
		Valore &= 0x03;
		*pTextureTipoTriangolo = Valore;

		__asm {
			pushad
			mov eax, 42AB00h    ;RefreshZonaTexture
			call eax
			popad
		}

	}
	// CTRL + F : ruota orientamento oggetto
	if (TastoPremutoNGLE(0x1d, 0x21) == true) RuotaOggettoSelezionato();

	// CTRL + B : inverte big texture
	if (TastoPremutoNGLE(0x1d, 0x30)==true) Sub_PulsanteBigTexture(NULL);

	// G = goto previous room
	if (TastoPremutoNGLE(0x22,-1)==true && TastoPremutoNGLE(0x1d,-1)==false) GotoPreviousRoom(-1);

	// CTRL G = goto following room
	if (TastoPremutoNGLE(0x1d, 0x22)==true) GotoPreviousRoom(1);

	// M = marka questa stanza come edited
	if (TastoPremutoNGLE(0x32,-1) ==true && TastoPremutoNGLE(0x1d,-1)==false) {
		IndiceProject = GetIndiceProgettoNow();
		StanzaNow= *pIndiceRoomAttuale;
		AddInOldRoomList(StanzaNow,IndiceProject);
	}

	// CTRL M  minimizza programma
	if (TastoPremutoNGLE(0x1d,0x32)==true) {
		ShowWindow(*pMiaWind, SW_SHOWMINIMIZED);
	}

	// U = Unmark: toglie stanza attuale da lista old
	if (TastoPremutoNGLE(0x16,-1) ==true) {
		IndiceProject = GetIndiceProgettoNow();
		StanzaNow= *pIndiceRoomAttuale;
		for (i=0;i<MAX_OLD_ROOMS;i++) {
			if (MyPrefRoom.VetOldProjects[IndiceProject].VetLista[i] == StanzaNow) {
				MyPrefRoom.VetOldProjects[IndiceProject].VetLista[i] = -1;
				OldIndiceRoom = -1;
			}
		}
		ShowInfoRoom("Removed current room from edited old room list");
	}

	// CTRL + L : load last projects
	if (TastoPremutoNGLE(0x1d, 0x26)==true) Sub_LastProjects(NULL);

	// CTRL + H : Move lara here
	if (TastoPremutoNGLE(0x1d, 0x23)==true) MuoviLara();

	// SHIFT + 9 visuaslizza log di stanza attuale
	if (TastoPremutoNGLE(0x2a, 0x0a) == true) ShowLogRoomNow();

	// CTRL + J inverte visualizzazione info sulla barra del titolo
	if (TastoPremutoNGLE(0x1d, 0x24) == true) InvertiShowInfoTitle();

	// SHIFT + C  centra stanza
	if (TastoPremutoNGLE(0x2a, 0x2e) == true) {
		roomCenterRoom(NULL);
		MessageBeep(-1);
	}


	// comando non documentato "ALT + CTRL + SHIFT"
	if (TastoPremutoNGLE(0x38,0x1d) == true && TastoPremutoNGLE(0x2a,-1)==true) {
		AumentaNumeroSlot();
	}
	// ctrl + O  (ricarica ultimo file oggetti)
	if (TastoPremutoNGLE(0x1d, 0x18)==true) CaricaUltimoWad();
	// SHIFT (left) + O
	if (TastoPremutoNGLE(0x2A, 0x18)== true) 	CaricaNuovoWad();
	
	// tasti per lancio programmi esterni
	if (TastoPremutoNGLE(0x2a,-1)==true) {
		for (i=0;i<MyPrefRoom.TotExternPrograms;i++) {
			if (TastoPremutoNGLE(2 + i,-1) == true) {
				// premuto SHIFT + N (da 1 a 0)
				StartProgrammaEsterno(i);
				break;
			}
		}
	}

	// ctrl + 9 (apre tga file con programma default)
	if (TastoPremutoNGLE(0x1d, 10) == true) {
		ApriFileTga();
	}
				
	// tasti appoggiati ai menu --------

	// CTRL + F1  smooth Slope
	if (TastoPremutoNGLE(0x1d,0x3b)==true)  Menu_SmoothSlope(NULL);

	// CTRL + F2  stepped slope
	if (TastoPremutoNGLE(0x1d,0x3C)==true) Menu_SteppedSlope(NULL);

	// CTRL + F3  smooth Slope ceiling
	if (TastoPremutoNGLE(0x1d,0x3d)==true)  Menu_CSmoothSlope(NULL);

	// CTRL + F4  stepped slope ceiling
	if (TastoPremutoNGLE(0x1d,0x3e)==true) Menu_CSteppedSlope(NULL);

	// CTRL + %0.1f  pyramid
	if (TastoPremutoNGLE(0x1d,0x3f)==true) Menu_Piramide(NULL);

	// CTRL + F6  Piramide inverse floor
	if (TastoPremutoNGLE(0x1d,0x40)==true) Menu_InvPiramide(NULL);

	// CTRL + F7  Piramide ceiling
	if (TastoPremutoNGLE(0x1d,0x41)==true) Menu_CPiramide(NULL);

	// CTRL + F8  Piramide inverse ceiling
	if (TastoPremutoNGLE(0x1d,0x42)==true) Menu_CInvPiramide(NULL);

	// CTRL + F11  remove grid da selected wal
	if (TastoPremutoNGLE(0x1d, 0x57) == true) Menu_RemoveGridSelectedWall(NULL);

	// CTRL + F12  remove all grid da room
	if (TastoPremutoNGLE(0x1d, 0x58) == true) Menu_RemoveGridAllWall(NULL);

	// SHIFT + F1  mostra info about project
	if (TastoPremutoNGLE(0x2A, 0x3b)==true) Menu_InfoAboutProject(NULL);

	// SHIFT + F2  Set Random Slope paramters
	if (TastoPremutoNGLE(0x2A,0x3c)==true) ShowSettings();

	// SHIFT + F3  Crea triangoli texture floor
	if (TastoPremutoNGLE(0x2A,0x3D)==true) Menu_CreaTriangoli(NULL);

	// SHIFT + F4  Crea triangoli texture ceiling
	if (TastoPremutoNGLE(0x2A,0x3E)==true) Menu_CCreaTriangoli(NULL);

	// SHIFT + F5  Permette di modificare dimensione di selezione 2d
	if (TastoPremutoNGLE(0x2a,0x3f)==true) SetSizeSelezione2d();


	// ALT + 1 Random floor up
	if (TastoPremutoNGLE(0x38, 2)==true) Menu_RandomFloorUp(NULL);

	// ALT + 2 Random floor Down
	if (TastoPremutoNGLE(0x38, 3)==true) Menu_RandomFloorDown(NULL);

	// ALT + 3 Random ceiling up
	if (TastoPremutoNGLE(0x38, 4)==true) Menu_RandomCeilingUp(NULL);

	// ALT + 4 Random ceiling Down
	if (TastoPremutoNGLE(0x38, 5)==true) Menu_RandomCeilingDown(NULL);

	// ALT + 5  crea screenshot fullscreen 
	if (TastoPremutoNGLE(0x38, 6)==true) Menu_ShotFullScreen(NULL);

	// ALT + 6  crea screenshot Panel2d 
	if (TastoPremutoNGLE(0x38, 7)==true) Menu_ShotPanel2d(NULL);

	// ALT + 7  crea screenshot Panel3d
	if (TastoPremutoNGLE(0x38, 8)==true) Menu_ShotPanel3d(NULL);

	// ALT + 8  crea screenshot Preview
	if (TastoPremutoNGLE(0x38, 9)==true) Menu_ShotPreview(NULL);

	// ALT + 9  crea screenshot Preview RGB
	if (TastoPremutoNGLE(0x38, 0x0a)==true) Menu_ShotPreviewRGB(NULL);

	// ALT + 0  crea screenshot texture
	if (TastoPremutoNGLE(0x38, 0x0b)==true) Menu_ShotTextures(NULL);

	// ALT + F9 clear texture floor
	if (TastoPremutoNGLE(0x38, 0x43)==true) Menu_ClearTextureFloor(NULL);

	// ALT + F10 clear texture floor
	if (TastoPremutoNGLE(0x38, 0x44)==true) Menu_ClearTextureCeiling(NULL);

	// ALT + F11  crea screenshot Info Room
	if (TastoPremutoNGLE(0x38, 0x57)==true) Menu_ShotInfoRoom(NULL);

	// ALT + F12  crea screenshot light
	if (TastoPremutoNGLE(0x38, 0x58)==true) Menu_ShotLights(NULL);

	// ALT + T  (reload current tga file)
	if (TastoPremutoNGLE(0x38, 0x14)==true) ReloadTgaFile();

	// CTRL 0 restore button window
	if (TastoPremutoNGLE(0x1d, 11)==true) {
		if (BaseWindow.TotButtons > 0) {
			if (BaseWindow.hWind == NULL) {
				CreaButtonWindow();
			}else {
				ShowWindow(BaseWindow.hWind, SW_RESTORE	);
			}
		}
	}
	// CTRL 1 bend floor
	if (TastoPremutoNGLE(0x1d, 2) ==true) Menu_BendFloor(NULL);

	// CTRL 2 Bend inverse floor
	if (TastoPremutoNGLE(0x1d,3) == true) Menu_BendInvFloor(NULL);

	// CTRL 3 Bending ceiling
	if (TastoPremutoNGLE(0x1d, 4) ==true) Menu_BendCeiling(NULL);

	// CTRL 4 Bending inverse ceiling
	if (TastoPremutoNGLE(0x1d,5) == true) Menu_BendInvCeiling(NULL);

	// CTRL 5 Cupola floor
	if (TastoPremutoNGLE(0x1d,6) == true) Menu_Cupola(NULL);

	// CTRL 6 Cupola floor inversa
	if (TastoPremutoNGLE(0x1d,7) == true) Menu_CupolaInv(NULL);

	// CTRL 7 Cupola ceiling
	if (TastoPremutoNGLE(0x1d,8) == true) Menu_CCupola(NULL);

	// CTRL 8 Cupola ceiling inversa
	if (TastoPremutoNGLE(0x1d,9) == true) Menu_CCupolaInv(NULL);

	// 

	TestIgnoraInput=false;

		
}

void Sub_PulsanteRotate(StrInfoMouse *pPulsante) 
{
	RuotaOggettoSelezionato();
}

void Sub_PulsanteMoveLara(StrInfoMouse *pPulsante) 
{
	MuoviLara();
}

void Sub_PulsanteExitAndPlay(StrInfoMouse *pPulsante) 
{
	IsToDisableRemoveItems();
	GiocaLivello(true);

}

void Sub_PulsantePlay(StrInfoMouse *pPulsante) 
{
	IsToDisableRemoveItems();
	GiocaLivello(false);

}

void Sub_PulsanteGoBack(StrInfoMouse *pPulsante)
{
	IsToDisableRemoveItems();
	GotoPreviousRoom(-1);
}
void Sub_PulsanteSettings(StrInfoMouse *pPulsante)
{
	ShowSettings();
}

void Sub_PulsanteNewRoomUp(StrInfoMouse *pPulsante)
{
	IsToDisableRemoveItems();
	AggiungiStanzaVerticale(true);
}

void Sub_PulsanteNewRoomDown(StrInfoMouse *pPulsante)
{

	IsToDisableRemoveItems();
	AggiungiStanzaVerticale(false);
}

void Sub_PulsanteNewTunnel(StrInfoMouse *pPulsante)

{
	IsToDisableRemoveItems();
	CreaTunnel();

}
void Sub_PulsanteSearch(StrInfoMouse *pPulsante)
{


	IsToDisableRemoveItems();
	FindInRoom();
}
// adatta coordinata restituendo il nuovo valore per coordinata
// in modo che sia un multiplo di GrigliaSize al valore piu' vicino
int AdattaGriglia(int Coordinata, int GrigliaSize)
{
	float Cord;
	float Griglia;
	
	Cord = (float) Coordinata;
	Griglia = (float) GrigliaSize;

	return Float2Int(Cord / Griglia) * GrigliaSize;

}
// sostituisce l'intero processo di selezione con pulsante destro della texture
// seleziona texture e imposta i dati nell'ultimo NTextureTailInfo
// esce solo quando utente smette di premere pulsante destro
// restiuisce il numero di triangolo.
int SelezionaDestroTexture(int OrgXMouse, int OrgYMouse)
{

	int* pTextureTipoTriangolo = (int*) 0x583BF4;
	BYTE *pPulsantiMouse = (BYTE*) 0x46DA88;
	DWORD **ptr2_MemXTexture = (DWORD **) 0x56C4FC;
	DWORD* Ptr_MemXTexture;
	DWORD *Ptr_TextureRawImage = (DWORD*) 0x56C4F8;
	short  *pCordXMouse = (short*) 0x46E4D4;
	short *pCordYMouse = (short*) 0x46E4D6; 
	WORD *pIndiceTextureSelezionata = (WORD*) 0x583BBA;
	int* pN_TextureTails = (int*) 0x56C4F4;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	int*  TestBigTexture = (int*) 0x454FFC;
	int  OrgXTexture;
	int  OrgYTexture;
	int  Massimo, Minimo;
	int		IndiceTextureNow; //(inteso come posizione in griglia)
	int   OrgXGriglia; // griglia di caselle 64 x 64
	int	  OrgYGriglia;
	int		SizeX,SizeY;
	int		x1,y1,x2,y2;
	int		temp;
	int		MinX,MinY,MaxX,MaxY;
	int		TotTails;
	int  xMouse,yMouse;
	int RestoX,RestoY;


	BYTE * pMemTga;

	pVetTailInfos = *p2VetTailInfos;
	Ptr_MemXTexture = *ptr2_MemXTexture;
	
	// correggere origine iniziale mouse per avere coordinate relative
	// a mappa texture
	// convertire orgx orgy secondo pixel 64
	ConverteCordMouse(&OrgXMouse,&OrgYMouse);

	OrgXGriglia = OrgXMouse / 64;
	OrgYGriglia = OrgYMouse / 64;


	IndiceTextureNow = OrgXGriglia + OrgYGriglia*4;	
	

	// sulla base di TestBigTexture calcolare dimensione massima, origine
	// e multiplo minimo per selezione
	if (*TestBigTexture == 0) {
		Minimo=16;
		Massimo=64;

	}else
	{
		Minimo=16;
		Massimo=256;

	}

	// adesso calcolare origine in pixel di texture attuale


	OrgXGriglia = OrgXMouse / Massimo;
	OrgYGriglia = OrgYMouse / Massimo;

	
	OrgXTexture = OrgXGriglia * Massimo;
	OrgYTexture = OrgYGriglia * Massimo;

	RestoX = OrgXMouse - OrgXTexture;
	RestoY = OrgYMouse - OrgYTexture;


	MinX = OrgXTexture;
	MaxX = OrgXTexture + Massimo;

	MinY = OrgYTexture;
	MaxY = OrgYTexture + Massimo;


	TotTails = *pN_TextureTails;
	*pIndiceTextureSelezionata = (WORD) TotTails;

	Ptr_MemXTexture[TotTails] = Ptr_MemXTexture[IndiceTextureNow];
	// correggere orgxmouse per avere arrotondamento ad unita' minima piu' vicina
	OrgXMouse = AdattaGriglia(OrgXMouse,Minimo);
	OrgYMouse = AdattaGriglia(OrgYMouse,Minimo);


	do {
		// calcolare rettangolo x1,y1, x2,y2

		x1 = AdattaGriglia(OrgXMouse,Minimo);
		y1 = AdattaGriglia(OrgYMouse,Minimo);

		xMouse = *pCordXMouse;
		yMouse = *pCordYMouse;

		ConverteCordMouse(&xMouse,&yMouse);

		x2 = AdattaGriglia(xMouse,Minimo);
		y2 = AdattaGriglia(yMouse,Minimo);



		if (x1 > x2) {
			temp = x1;
			x1 = x2;
			x2 = temp;
		}

		if (y1 > y2) {
			temp = y1;
			y1=y2;
			y2=temp;
		}

		if (x1 < MinX) x1=MinX;
		if (x1 >= MaxX) x1 = MaxX-Minimo;

		if (y1 < MinY) y1=MinY;
		if (y1 >= MaxY) y1 = MaxY-Minimo;

		if (x2 < MinX) x2 = MinX+Minimo;
		if (x2 > MaxX) x2 = MaxX;

		if (y2 < MinY) y2 = MinY+Minimo;
		if (y2 > MaxY) y2 = MaxY;


		// ora forzare dimensione minima

		if (x1 == x2) x2 += Minimo;
		if (y1 == y2) y2 += Minimo;

		// ora sulla base di questo rettangolo costruire dimensione 
		// e puntatore immagine
		// e copiare il tutto in tailinfo[ntailinfo]
		pMemTga = (BYTE*) *Ptr_TextureRawImage;
		pMemTga += x1 + y1*256;
		SizeX = x2-x1;
		SizeY = y2-y1;

		SizeX *= 256;
		SizeY *= 256;
		SizeX --;
		SizeY --;


		pVetTailInfos[TotTails].pMemImage = pMemTga;
		pVetTailInfos[TotTails].SizeX = SizeX;
		pVetTailInfos[TotTails].SizeY = SizeY;
		VetBigTails[TotTails].pMemImage = pMemTga;
		VetBigTails[TotTails].SizeX = SizeX;
		VetBigTails[TotTails].SizeY = SizeY;

		// ok, ora mostrare la texture 
		__asm {
			pushad
			mov eax, 42AB00h    ;RefreshZonaTexture
			call eax
			mov eax, 41A2B0h    ;AggiornaMouseNow
			call eax
			test eax, eax
			jz   Salta
			mov eax, 436AEEh    ;ElaboraQualcosaMouse
			call eax
Salta:
			popad

		}


	}while((*pPulsantiMouse) & 0x02);

// ora impostare triangolo texture
	// ricovertire size a valori normali
	SizeX++;
	SizeY++;
	SizeX /= 256;
	SizeY /= 256;

	return CalcoloTextureTriangolo(RestoX,RestoY,SizeX,SizeY);

}





HICON OttieneHandleIcona(HINSTANCE hInst)
{
	IconaRoom= LoadIcon(hInst,"MAIN");
	return IconaRoom;

}
// cerca in file sounds.txt il nome del file wav (o dei files)
// corrispondenti a pNomeSound
// restituisce la lista dei files separata da spazi.
char *TrovaSoundWav(char *pNomeSound)
{
	static char BufWav[256];
	FILE *pFile;
	char BufLinea[300];
	int i;
	char *pTemp;
	char *pInizio;
	char *pFine;
	int N;
	char MexCerca[256];


	sprintf(MexCerca,"%s:", pNomeSound);

	pFile = fopen(GetFileTrle("sound\\LevelSFX Creator\\Sounds.txt"),"rb");
	if (pFile == NULL) {
		return "ERROR: can't open 'sounds.txt' file";
	}
	while (feof(pFile) == 0) {
		fgets(BufLinea,299,pFile);
		RemoveNewLine(BufLinea);

		// ora scandire 
		pTemp = Search(BufLinea,MexCerca,ts_Avanti,NULL);
		if (pTemp != NULL) {

			pTemp += strlen(MexCerca);
			pInizio=pTemp;

			pFine = Search(pInizio,"VOL",ts_Avanti,NULL);
			if (pFine == NULL) {
				fclose(pFile);
				return "";

			}
			N = pFine-pInizio;
			for (i=0;i<N;i++) {
				if (pInizio[i] == 9) 
					BufferLog[i]=' ';
				else
					BufferLog[i] = pInizio[i];
			}
			BufferLog[i]=0;
			pTemp = Trim(BufferLog);
			strcpy(BufWav,pTemp);
			fclose(pFile);
			return BufWav;

		}

	}
	fclose(pFile);
	sprintf(BufWav,"ERROR: cann't find sound \"%s\" in 'sounds.txt' file", pNomeSound);
	return BufWav;


}
// chiamata quando viene azzerato vettore texturedd
void AzzerateTextureDD(void)
{
	TotAdrTexFace=0;
#ifdef LOG_TEXTURE
	InviaLog("================ Cleared TextureDD ==============");
#endif
}
// eseguita ad ogni ciclo per verificare se e' passato il tempo di visualizzazione
// messaggio
void ControllaDeleteMessaggi(void)
{
	DWORD TempoNow;

	if (TestMessaggio==false) return;

	TempoNow = GetTickCount() - LastTimeMessaggio;
	if (TempoNow >= 1000) {
		ShowInfoRoom("");
		TestMessaggio=false;
	}




}

__declspec(naked) void SpostaSchermo(int OrgX, int OrgY)
{
	__asm {

	push esi	; INIZIO PROCEDURA 796 ;  SIZE=0x50 
	mov  esi, [esp+0ch]   ;orgy
	push edi 
	mov  edi, [esp+0ch]   ;ogx
	push esi 
	push edi 
	push  461B80h	;  ASCII "DXMove : x %d y %d" ;Mex_DXMove_x_Argd_y_Argd 
	push 02h 
	mov eax, 449870h    ;InviaMessaggioLog
	call eax 
	mov eax, 46D908h    ;ptr_BaseDirectXLibrary
	mov  eax, dword ptr [eax] 
	add  esp, 10h 
	test eax, eax 
	jz   Salto1 
	test byte ptr [eax+44h], 01h 
	jnz  Salto1 
	mov  ecx, [eax+20h] 
	mov  edx, [eax+1ch] 
	add  ecx, esi 
	add  edx, edi 
	// mia modifica
	push esi ;salva
	neg esi
	add  ecx, esi
	pop esi
	// fine mod
	push ecx	; /Bottom 
	push edx	; |Right 
	push esi	; |Top    FFFFFEF2  -270 
	add  eax, 34h 
	push edi	; |Left 
	push eax	; |pRect 
	mov eax, 455190h    ;SetRect
	call dword ptr [eax]	; \SetRect 
	mov ecx, 563F58h    ;BaseSchermo
	mov  ecx, dword ptr [ecx] 
	push ecx 
	mov eax, 444BF0h    ;RefreshTutto
	call eax 
	add  esp, 04h 
Salto1: 
	pop  edi 
	pop  esi 
	ret 
	}
}


// viene eseguita ad ogni pressione di tasto o pressione del mouse
__declspec(naked) void EseguiOgniCiclo(void)
{
	__asm {
		pushad

		mov	  ecx, 583BA0h  ;IndiceRoomAttuale
		mov  ecx, dword ptr [ecx]


		mov  eax, ecx
		shl  eax, 05h
		sub  eax, ecx
		mov ecx, 46B850h    ;Ptr_VetRoomSlots
		mov  ecx, dword ptr [ecx]
		lea  eax, [eax][eax*2]
		lea  edx, [ecx][eax*4]
		add  eax, edx	
		;in eax adesso c'e' record room attuale
		movzx	ebx, word ptr [eax+72h]  ;ZSizeBlock
		push	ebx
		movzx	ebx, word ptr [eax+70h]  ;XSizeBolock
		push	ebx
		mov		ebx, dword ptr [eax+74h]  ;PtrTabGrid
		push	ebx
		push	eax		;RecordRoom
		lea		eax, ControllaOldRoom
		call	eax
		add		esp, 10h
		

;------- controllo per messaggi da cancellare
		lea		eax, ControllaDeleteMessaggi
		call	eax

		popad
		retn
	}


}
// visualizza lista degli effetti sonori
// restituisce indice di suono
int ShowListaSounds(StrListElenco *pVetListSounds, int TotListSounds)
{
	int i;
	char BufferTemp[256];
	int Indice;
	char NomeWav[256];
	char *pMexOk;
	char *pFileWav;
	int j;
	char **VetParti;
	int TotParti;
	char *pInLevel;


	// carica in VetList[] VetPunta[] TotList tutti i valori dell'elenco
	LeggeTabellaSuoniSfx();
	if (TotListSounds > 1024) TotListSounds=1024;

	TotList = 0;
	for (i=0;i<TotListSounds;i++) {

		pFileWav = TrovaSoundWav(pVetListSounds[i].pNome);
		VetParti=Split(pFileWav,' ',&TotParti,NULL,NULL,false);
		for (j=0;j<TotParti;j++) {
			sprintf(NomeWav,"sound\\samples\\%s.wav", VetParti[j]);

			if (EsisteFile(GetFileTrle(NomeWav))==false) break;
		}

		// vedere se file e' presente nel livello attuale
		pInLevel= "MISSING";
		if (i < GlobNgle.SoundVetSfx.TotSfx) {
			if (GlobNgle.SoundVetSfx.VetSfx[i] != -1) pInLevel ="OK";
		}
		if (j == TotParti && j != 0)
			pMexOk= "Ok";
		else
			pMexOk= "MISSING";


		sprintf(BufferTemp,"%-25s  %3d  %-8s %-8s %s", 
					pVetListSounds[i].pNome, 
					pVetListSounds[i].Indice,
					pInLevel, 
					pMexOk,
					pFileWav);

		strcpy(VetNomiList[TotList],BufferTemp);
		VetPuntaList[TotList]=i;
		TotList++;
		

	}

	Indice = InputListBox(TLB_ListaSounds);
	if (Indice != -1) {
		Indice= pVetListSounds[Indice].Indice ;
	}
	sndPlaySound(NULL,0);
	return Indice;

}
// visualizza gli oggetti degli slot per selezione oggetto corrente
int ShowListaOggetti(StrListElenco *pVetListObject, int TotListObj)
{
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int		i;
	DWORD **p2VetMeshPointers = (DWORD**) 0x46B828;   
	DWORD *pVetMeshPointers;
	char *pNome;
	int   Indice;
	char *pTipo;
	char  BufferTemp[256];
	int  IndiceRel;
	int  IndiceAbs;
	int	j;
	bool TestSprite;
	DWORD LastValue;
	int		NMesh;
	DWORD  VerticesAmount;
	char  *pCollision;
	WORD *pWord;
	int  FirstAnim;

	
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetMeshPointers = *p2VetMeshPointers;


	TotList = TotListObj;
	// caricare in vetnomilist  vetpuntalist
	for (i=0;i<TotList;i++) {
		pNome = pVetListObject[i].pNome;
		Indice = pVetListObject[i].Indice;
		TestSprite=false;
		switch (pVetSlotOggetti[Indice].Tipo) {
		case 8:
			pTipo = "Sprite";
			TestSprite=true;
			break;
		case 0x10:
			pTipo = "Moveable";
			break;
		case 0x110:
			pTipo = "Static";
			break;
		default:
			pTipo = "UNKNOWN";
		}
		IndiceAbs = pVetSlotOggetti[Indice].MeshIndex;
		IndiceRel=0;
		if (IndiceAbs < TotPtrOriginaliMesh && TestSprite==false) {
			// contare numero puntatori validi in vetmeshpointer originael
			// per scopire l'indice relativo
			LastValue=0;
			for (j=0;j<IndiceAbs;j++) {
				if (pVetPtrOriginaliMesh[j] > LastValue) {
					IndiceRel++;
					LastValue= pVetPtrOriginaliMesh[j];
				}
			}
			IndiceRel++;

		}
		pCollision="NO";
		VerticesAmount=0;
		FirstAnim=-1;
		if (TestSprite==false) {
			NMesh = pVetSlotOggetti[Indice].TotMesh;
			// trovare numero di vertici e se c'e' collisione
			pWord = (WORD *) pVetMeshPointers[IndiceAbs];
			if (pWord[0] == 0) {
				pCollision = "Yes";
			}else{
				pCollision = "No";
			}

			VerticesAmount = pWord[1];
			FirstAnim = pVetSlotOggetti[Indice].AnimStartIndex;


		}else
		{	
			NMesh = 0;
		}

		sprintf(BufferTemp,"%-25s  %3d  %8s  %4d   %3d   %4d  %4d   %3s  %4d", pNome, 
						Indice,pTipo,NMesh,	IndiceRel,IndiceAbs, 
						VerticesAmount,pCollision,  FirstAnim); 
		strcpy(VetNomiList[i],BufferTemp);
		VetPuntaList[i]=i;
		// per debug, controllare mesh associata ad indiceabs


	}

	Indice = InputListBox(TLB_ListaOggetti);
	if (Indice != -1) {
		UltimoIndiceOggetto=Indice;
		Indice= pVetListObject[Indice].Indice ;

	}
	return Indice;

}


// visualizza tutte le stanze e restituisce indice stanza scelta  (o -1)
int ShowListaStanze(StrListElenco *pVetListaStanze, int TotListaStanze)
{

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int		i;
	char *pNome;
	int		Indice;
	char BufferTemp[256];
	int  Altezza;
	char  BufNome[256];
	int TotTriggers;
	int TotObjects;
	int	j;
	int IndiceObj;
	int SizeX;
	int SizeY;
	bool TestEmpty;
	char MexButtons[256];

	WORD VetFlagButtons[] = {0x0001, 0x0020, 0x0800, 0x0400, 
							0x0010, 0x0004, 0x1000, 
							0x0200, 0x0100, 0x0080, 0x0008};
	char *VetMexButtons[] = {"Water", "Outside", "Rain", "Snow", 
							"Damage", "Quick-Sand", "Cold",
							"Reflex", "Mist", "NonLensFlare", "Horizont"};

	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	pVetRooms = *p2VetRooms;


	TotList = TotListaStanze;
	for (i=0;i<TotListaStanze;i++) {
		pNome = pVetListaStanze[i].pNome;
		if (strcmpi(pNome,"empty")==0) {
			sprintf(BufNome,"%s (%d)", pNome,i);
			TestEmpty=true;
		}else {
			strcpy(BufNome,pNome);
			TestEmpty=false;
		}

		Indice = pVetListaStanze[i].Indice;
		Altezza = pVetRooms[Indice].OrgYClickTop - pVetRooms[Indice].OrgYClickBottom;
		// conta oggetti  e trigger
		TotTriggers=0;
		TotObjects=0;
		for (j=0;j<pVetRooms[Indice].N_ObjAndTriggers;j++) {
			IndiceObj = pVetRooms[Indice].VetObjAndTriggerCodes[j];
			IndiceObj = pVetBaseOggetti[IndiceObj].TypeObject;
			IndiceObj = roomGetCategoriaTipoOggetto(IndiceObj);

			switch (IndiceObj) {
			case 0x10:
				TotTriggers++;
				break;
			case 0x08:
				TotObjects++;
				break;
			}
		}
		// registra flag buttons di room
		MexButtons[0]=0;
		for (j=0;j<11;j++) {
			if (pVetRooms[Indice].RoomButtons & VetFlagButtons[j]) {
				strcat(MexButtons, VetMexButtons[j]);
				strcat(MexButtons, ";");
			}
		}

		j = strlen(MexButtons)-1;

		if (j >=0) MexButtons[j]=0;

		SizeX = pVetRooms[Indice].XSizeBlocks-2;
		SizeY = pVetRooms[Indice].ZSizeBlocks-2;
		if (TestEmpty == true) {
			SizeX =0;
			SizeY =0;
			TotTriggers=0;
			TotObjects=0;
			MexButtons[0]=0;
			Altezza=0;

		}


		sprintf(BufferTemp,"%-25s  %2d x %2d   %3d    %3d  %3d   %3d    %s",
				BufNome, SizeX, SizeY, 
				Altezza, pVetRooms[Indice].N_Effects,TotTriggers,TotObjects,MexButtons );
		strcpy(VetNomiList[i], BufferTemp);
	}

	
	Indice = InputListBox(TLB_ListaStanze);
	if (Indice !=-1) {
		Indice = pVetListaStanze[Indice].Indice;

	}


	return Indice;



}
// viene chiamata all'inizio di sub_load (room editor)
// salva il nome del file che si sta per caricare nella variabile globale
// NomeLoadProject

void SalvaNomeLoad(char *pNomeFile)
{
	TestLoadProject=true;
	OldIndiceProject=-1;
	strcpy(NomeLoadProject,pNomeFile);

}

// inizializza tutte i tail infos allo stesso modo fatto
// dal room editor subito dopo aver allocato la memoria per le texure
// nella funzione InizializzaMemoriaTexture
// chiamarla anche prima di caricare un nuovo progetto in modo da avere
// stesse condizioni di partenza
__declspec(naked) void InitTailInfos(void)
{
	__asm {
		pushad

		mov eax, 56C4E4h    ;Ptr_MemoriaRoomTexInfos
		mov  eax, dword ptr [eax]
		push ebx
		mov ebx, 506A78h   ;Ptr_TextureTailInfos
		mov  dword ptr [ebx], eax
		pop ebx
		xor  ecx, ecx
		mov  esi, 3fffh
	Salto1:
		mov  eax, ecx
		cdq
		and  edx, 03h
		add  eax, edx
		mov  edx, ecx
		sar  eax, 02h
		shl  eax, 08h
		and  edx, 80000003h
		jns  Salto2
		dec  edx
		or   edx, -04h
		inc  edx
	Salto2:
		add  eax, edx
		mov edx, 56C4F8h    ;Ptr_TextureRawImage
		mov  edx, dword ptr [edx]
		shl  eax, 06h
		add  eax, edx
		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov  [edx][ecx*8], eax
		mov eax, 506A78h    ;Ptr_TextureTailInfos
		mov  eax, dword ptr [eax]
		mov  [eax+04h][ecx*8], si
		mov edx, 506A78h    ;Ptr_TextureTailInfos
		mov  edx, dword ptr [edx]
		mov  [edx+06h][ecx*8], si
		mov eax, 56C4FCh    ;Ptr_MemTestTextureNero
		mov  eax, dword ptr [eax]
		mov  dword ptr [eax][ecx*4], 00h
		inc  ecx
		cmp  ecx, 400h  
		jl   Salto1
		popad
		retn
	}
}
// viene chiamata immediatamente prima di caricare file tga, allo scopo
// di impostare in texturetail tutte le tail per coprire le MAX_TAIL_INFOS texture
// a 64 pixel.

void BackupTailInfos(void)
{
	int	SizeZona;
	int* pN_TextureTails = (int*) 0x56C4F4;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	BYTE **p2VetTexSonorePrj = (BYTE** ) 0x45463E;
	BYTE *pVetTexSonorePrj;

	pVetTailInfos = *p2VetTailInfos;
	pVetTexSonorePrj = *p2VetTexSonorePrj;

	// vedere se siamo nella situazione in cui si deve fare backup e restore
	TestFattoBackup=false;
#ifdef LOG_TEXTURE

	sprintf(BufferLog,"BackupTailInfos() for LoadTga: TestTgaProject=%d  TestLoadedProject %d",
		TestTgaProject,TestLoadedProject);
	InviaLog(BufferLog);
#endif

	if (TestTgaProject == 0 && TestLoadedProject == true) {
		// effettuare backup
		TestFattoBackup=true;
		InviaLog("\tPerform backup");
		// salvare zona texture tails e numero
		SizeZona = sizeof(StrTailInfo) * MAX_TAIL_INFOS;
		memcpy(&VetTempTails, pVetTailInfos,SizeZona);
		TempTotTails = *pN_TextureTails;

		// ora salvare anche pVetTexSonorePrj
		memcpy(VetTempSonorePrj, pVetTexSonorePrj,MAX_TAIL_INFOS);
		// e adesso scrivere le textail standard per tutte le tailinfos
		InitTailInfos();

	}


}
// viene chiamata DOPO aver caricato load tga. viene usata per l'eventuale
// rirpristino dei vettori che erano stati modificati dalla funzione InizializzaTexture
void RipristinaTailInfos(void)
{
	
	int	SizeZona;
	int* pN_TextureTails = (int*) 0x56C4F4;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	BYTE **p2VetTexSonorePrj = (BYTE** ) 0x45463E;
	BYTE *pVetTexSonorePrj;


	pVetTailInfos = *p2VetTailInfos;
	pVetTexSonorePrj = *p2VetTexSonorePrj;

	sprintf(BufferLog,"RestoreTailInfos()  TestBackup=%d", TestFattoBackup);
	InviaLog(BufferLog);


	if (TestFattoBackup==true) {
		// effettuare backup
		InviaLog("\tRestore backup");
		// riprstinare zona texture tails e numero
		SizeZona = sizeof(StrTailInfo) * MAX_TAIL_INFOS;
		memcpy(pVetTailInfos, &VetTempTails,SizeZona);
		*pN_TextureTails = TempTotTails;

		// ora ripristinare anche pVetTexSonorePrj
		memcpy(pVetTexSonorePrj, &VetTempSonorePrj,MAX_TAIL_INFOS);
		TestFattoBackup=false;
	}


	
}

// viene chiamata subito dopo che programma ha caricato in memoria file tga
// di texture
void AllocaFileTga(StrHeaderTga *pHeaderTga)
{

	BYTE *pBaseTga;
	BYTE *pSrc;
	BYTE *pDest;
	BYTE *pBaseOut;
	int		TotPixel;
	int		NewSize;
	int		i;
	int		j;
	HWND *pMiaWind = (HWND*) 0x46E559;
	BITMAPINFO InfoDIB;

	// non c'entra niente col bitmap, ma qui si effettua un backup
	// delle tail info attuali + una nuova inizializzazione per consntire
	// un caricamento corretto di tutte le texture

	BackupTailInfos();
	
	if (HandleBitMap != NULL) {
		// cancellare il precedente bitmap
		DeleteObject(HandleBitMap);
	}

	memset(&InfoDIB,0,sizeof(BITMAPINFO));
	// forzare dimensione inferiore allineata ai 64 pixel
	TGA_SizeX = pHeaderTga->SizeX & ~0x3f;
	TGA_SizeY = pHeaderTga->SizeY;

	if (TGA_SizeY & 0x3f) {
		NewSize = TGA_SizeY / 64;
		NewSize++;
		NewSize *= 64;

		sprintf(BufferLog,"WARNING: current texture map has an height (Y Size) wrong. NGLE requires that height of texture map was a multiple of 64 pixels.\r\nCurrent texture map has size %d x %d. Height %d is not a multiple of 64, please change it to force the tga file with new height = %d pixels",
			pHeaderTga->SizeX, pHeaderTga->SizeY, pHeaderTga->SizeY, NewSize);
		MostraMessageBoxNGLE(BufferLog,0);
	}

	sprintf(BufferLog,"Allocate tga image: SizeX=%d  SizeY=%d", TGA_SizeX, TGA_SizeY);
	InviaLog(BufferLog);

	// calcolare dove inizia zona pixel del file tga
	pBaseTga = (BYTE *) pHeaderTga;
	pBaseTga += sizeof(StrHeaderTga);

	TotPixel = TGA_SizeX * TGA_SizeY;

	pBaseOut = (BYTE *) malloc(TotPixel * 4);
	
	pSrc = pBaseTga;
	pDest = pBaseOut;

	// ora copiare tutti i pixel convertendo il formato da tri a quad
	for (i =0;i<TotPixel;i++) {
		for (j=0;j<3;j++) {
			pDest[j] = pSrc[j];
		}
		pDest[j] = 0;
		pDest+= 4;
		pSrc+= 3;
	}

	// ora creare il bitmap

	InfoDIB.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	InfoDIB.bmiHeader.biWidth = TGA_SizeX;
	if (pHeaderTga->Descriptor == 0) 
		InfoDIB.bmiHeader.biHeight = TGA_SizeY;
	else
		InfoDIB.bmiHeader.biHeight = -TGA_SizeY;
	
	InfoDIB.bmiHeader.biPlanes = 1;
	InfoDIB.bmiHeader.biBitCount =32;
	InfoDIB.bmiHeader.biCompression = BI_RGB;

	
	HandleBitMap = CreateDIBitmap(GetDC(*pMiaWind),&InfoDIB.bmiHeader,CBM_INIT, 
						pBaseOut, &InfoDIB, DIB_RGB_COLORS	);
	if (HandleBitMap==NULL) {
		InviaLog("ERROR trying to create DIBitmap");
	}

	// vedere se si puo' cancellare subito memoria di dati bitmap
	// ok
	free(pBaseOut);
	InviaLog("\tLoading tga map completed: Ok");


	

}

void ShowObjColor(int OrgX, int OrgY, int Colore)
{
	int Risultato;
	char BufferTemp[256];
	// prima cancellare zona totale dove scrivere numero

	DisegnaRettangolo(OrgX,OrgY,30,14,1);

	// ora calcolare numero
	if (Colore== 0) {
		Risultato =0;
	}else {

		Risultato = Colore+1;
		Risultato = Risultato << 3;
		Risultato--;
	}
	sprintf(BufferTemp,"%d",Risultato);

	// ora scrivere testo
	ShowTestoSuSchermo(OrgX+7, OrgY+3, BufferTemp);

	// adesso aggiornare zona schermo
	roomAggiornaZonaSchermo(OrgX,OrgY,30,14);



}
// visualizza nel riquadro "Object Tint" le componennti Reg Green e Blu di ColoreObject
void MostraColoreObject(WORD ColoreObject)
{

	WORD  Maschera;
	WORD  Colore;

	Maschera = 0x1f;
	// colore rosso
	Colore = ColoreObject & Maschera;
	ShowObjColor(211,471, Colore);
	
	// colore verde
	Maschera = Maschera << 5;

	Colore = ColoreObject & Maschera;
	Colore = Colore >> 5;

	ShowObjColor(211, 489, Colore);

	// colore blu
	Maschera = Maschera << 5;
	Colore = ColoreObject & Maschera;
	Colore = Colore >> 10;
	ShowObjColor(211, 507, Colore);

}
// viene chiamata un attimo prima di caricare file wad
// salva il nome in variabile globale NomeFileWad
void SalvaNomeWad(char *pNomeWad)
{
	strcpy(NomeFileWad,pNomeWad);

}

// sostituisce la funzione che in room editor anima le texture di tipo frame
void AnimaTexturePreview(void)
{
	int *pTotIndiciAnim  = (int*) 0x56C4E8;
	int *pVetIndiciAnim = (int*) 0x56C440;
	StrAnimRange *pVetAnimRange = (StrAnimRange*) 0x56C520;
	StrTailInfo **p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo *pVetTailInfos;
	int	i,j;
	int  TotTexture;
	DWORD GapMemoria;
	int IndiceTail;
	DWORD Tempo;
	int	Indice;
	int Inizio,Fine;
	BYTE *pSalvaMemTga;
	WORD TempWord;
	DWORD TickNow;
	StrRoomAnimaUV *pRoomAnim;
	bool TestEsegui;
	BYTE *pNewMem;


	pVetTailInfos = *p2VetTailInfos;
	// animare texture di tipo fraame.
	TickNow = GetTickCount();

	for (i=0 ;i<*pTotIndiciAnim;i++) {
		// veedere se eseguire adesso l'animazione di questa serie texture
		Indice = pVetIndiciAnim[i];
		// estrarre frame 
		TempWord = GlobNgle.RoomTexAnimate.VetInfoRangeAnim[Indice];
		switch (TempWord & FAN_MASK_ANIM) {
		case FAN_FRAMES:
			Tempo = TempWord & FAN_MASK_DELAY;

			// in frames c'e' il numero di tick di attesa
			if ((TickNow - VetCounterAnim[Indice]) >= Tempo) {


				// eseguire il cambio
				Inizio = pVetAnimRange[Indice].IndiceStart & 0xff;
				Fine = pVetAnimRange[Indice].IndiceEnd & 0xff;

				
				// qui avviene il crash di delta
				pSalvaMemTga = pVetTailInfos[Inizio].pMemImage;


				for (j=Inizio;j<Fine;j++) {
					pVetTailInfos[j].pMemImage = pVetTailInfos[j+1].pMemImage;
				}

				pVetTailInfos[j].pMemImage = pSalvaMemTga;


				// impostare nuovo valore
				VetCounterAnim[Indice]= TickNow;
			}
			break;
		case FAN_FULL_ROTATE:
		case FAN_HALF_ROTATE:
		case FAN_RIVER_ROTATE:
			// eseguire animazione scroll
			pRoomAnim = &VetRoomAnimaUV[Indice];
			TestEsegui=true;
			if (pRoomAnim->Delay) {
				Tempo = TickNow - pRoomAnim->LastTime;
				if (Tempo < pRoomAnim->Delay) TestEsegui=false;
			}
			if (TestEsegui) {
				pRoomAnim->LastTime = TickNow;

				pRoomAnim->ScrollPos -= pRoomAnim->UVRotate;
				pRoomAnim->ScrollPos &= pRoomAnim->MascheraScroll;
				TotTexture = pVetAnimRange[Indice].IndiceEnd - 
								pVetAnimRange[Indice].IndiceStart +1;
				GapMemoria = pRoomAnim->ByteRiga * pRoomAnim->ScrollPos;

				for (j=0;j<TotTexture;j++) {
					IndiceTail = pVetAnimRange[Indice].IndiceStart + j;

					pNewMem = pRoomAnim->VetMemOrigine[j] + GapMemoria;
					VetBigTails[IndiceTail].pMemImage = pNewMem;
					pVetTailInfos[IndiceTail].pMemImage = pNewMem;
				}
			}

				
			break;
		}

	}

}
// sulla base di attuale valore di TipoMappaVerticale
// restituisce il valore di coordinata piu' vicina all'angolo di visuale 
// di stanza attuale
// fare in modo che i punti piu' distanti dalla visuale siano grandi
// e quelli piu' vicini piccoli
int CalcolaDistanzaVisuale(StrStanzaRoomEdit *pRoom)
{
	int Valore;

	switch (TipoMappaVerticale)
	{
	case 0:
		// visuale da sud quindi il valore e' orgz + sizez
		 Valore= pRoom->ZBlocksWorld + pRoom->ZSizeBlocks;
		 Valore = -Valore;
		break;
	case 1:
		// visione da ovest (da sinistra): origine x
		Valore= pRoom->XBlocksWorld;
		break;

	case 2:
		// visione da nord (dall'alto): origine z
		Valore= pRoom->ZBlocksWorld;
		break;
	case 3:
		// visione da est (da destra): orgx+ sizex
		Valore= pRoom->XBlocksWorld + pRoom->XSizeBlocks;
		Valore = -Valore;
		break;
	}
	return Valore;

}
// visualizza mappa 2d in verticale
// il tipo di visuale dipende anche dalla variabile TipoMappaVerticale
// possibili valori
// 0 = visione da sud
// 1 = visione da ovest
// 2 = visione da nord
// 3 = visione da est
void ShowMappaVerticale(int *pVetIndici, int TotRooms)
{
	int  i;
	int  j;
	int  Temp;
	int OrgX,OrgY,SizeX,SizeY;
	int Colore;
	int Indice;
	int Alfa, Beta;
	BYTE *pPulsantiMouse = (BYTE *) 0x46DA88;
	int ValoreAlfa,ValoreBeta;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int ValoreSelezione;
	bool TestPulsanteDestro;




	pVetRooms = *p2VetRooms;

	if ((*pPulsantiMouse) & 2) 
		TestPulsanteDestro=true;
	else
		TestPulsanteDestro=false;



	// ordinare i puntatori in modo che i primi (che verranno disegnati per primi)
	// siano quelli piu' lontani dal punto di visuale

	for (i=0;i<TotRooms;i++) {
		Indice= pVetIndici[i];
		VetStanze2d[Indice].Valore = CalcolaDistanzaVisuale(&pVetRooms[Indice]);
		if (Indice == *pIndiceRoomAttuale) 
					ValoreSelezione=VetStanze2d[Indice].Valore;



	}

	for (i=0;i<TotRooms-1;i++) {
		for (j=i+1;j<TotRooms;j++) {

			Alfa = pVetIndici[i];
			Beta = pVetIndici[j];
			
			ValoreAlfa = VetStanze2d[Alfa].Valore;
			ValoreBeta = VetStanze2d[Beta].Valore;
			if (ValoreAlfa < ValoreBeta || 
				(ValoreAlfa == ValoreBeta && Alfa == *pIndiceRoomAttuale)) {
			
				// invertire
				Temp = pVetIndici[i];
				pVetIndici[i] = pVetIndici[j];
				pVetIndici[j] = Temp;
			}
		}
	}

	// adesso visualizzare tutte le stanze dalla piu' lontana alla piu' vicina

	for (i=0;i<TotRooms;i++) {
		Indice = pVetIndici[i];

		// valori sull'asse y sono sempre uguali
		SizeY = pVetRooms[Indice].OrgYClickTop - pVetRooms[Indice].OrgYClickBottom;		
		
		// provare a lasciare 4 click
		/*
		// fare in modo che 4 click verticali siano 5 pixel
		TempFloat = (float) SizeY;
		TempFloat = TempFloat * 5.0f;
		TempFloat = TempFloat / 4.0f;
		SizeY = (int) TempFloat;
		*/

		//SizeY -=3;

		OrgY = pVetRooms[Indice].OrgYClickTop;
		
		// ora renderlo sempre positivo in modo che la coordinata 0 
		// corrisponda a posizione centrale in verticale
		OrgY = 256 - OrgY;
		

		// adesso per gli altri lati cambia tutto a seconda di punto di visuale
		switch (TipoMappaVerticale) {
		case 0:
			// visione da sud, SizeX = Sizex
			OrgX = pVetRooms[Indice].XBlocksWorld +1;
			SizeX = pVetRooms[Indice].XSizeBlocks-2;
			break;
		case 1:
			// visione da ovest (da sinistra)
			OrgX = pVetRooms[Indice].ZBlocksWorld+1;
			SizeX = pVetRooms[Indice].ZSizeBlocks-2;
			break;
		case 2:
			// visione da nord (dall'alto) 
			// questo e' un casino
			SizeX = pVetRooms[Indice].XSizeBlocks-2;
			OrgX = pVetRooms[Indice].XBlocksWorld + SizeX ;
			OrgX = 102 - OrgX;

			break;
		case 3:
			// visione da est (da destra)
			SizeX = pVetRooms[Indice].ZSizeBlocks-2;
			OrgX = pVetRooms[Indice].ZBlocksWorld + SizeX;
			OrgX = 102 - OrgX;

			break;
		}
		// a dimensione SizeX togliere sempre 2 perche' nei valori
		// sono conteggia anche i muri esterni
	
	

		// moltiplicare per 5 (5 pixel per ogni blocco orizzontale)
		OrgX *= 5;
		SizeX *= 5;



		OrgX += 293;
		OrgY += 10;
		// calcolo colore
		if (Indice == *pIndiceRoomAttuale ) {
			Colore = 4;  // rosso
		}else
		{
			// se sono molto vicine usare azzurro
			if ((abs(abs(VetStanze2d[Indice].Valore) - abs(ValoreSelezione))) <= 2 ) {
				Colore = 6;  // azzurro
			}else {
				if (VetStanze2d[Indice].Valore > ValoreSelezione) 
					Colore= 2;  // grigio scuro
				else
					Colore= 1;  // grigio chiaro
			}
		
		}
		VetStanze2d[Indice].Colore = Colore;
		VetStanze2d[Indice].Posizione.left = OrgX;
		VetStanze2d[Indice].Posizione.top = OrgY;
		VetStanze2d[Indice].Posizione.right = OrgX+SizeX;
		VetStanze2d[Indice].Posizione.bottom = OrgY+SizeY;

		if (TestPulsanteDestro == false || Colore !=1) {
			// adesso disegnare questa stanza
			DisegnaRettangolo(OrgX,OrgY,SizeX,SizeY,Colore);
		}else
		{
			// trasparente
			VetStanze2d[Indice].Colore  = -1;
		}
		roomDisegnaCornice(OrgX,OrgY, SizeX+1,SizeY+1,0,0);



	}

	roomAggiornaZonaSchermo(293,10,512,512);



}
// alza o abbassa stanza IndiceRoom aggiungendo NumeroClick ad ogni 
// WorldFloorClick e WorldCeilingClick
// nota: NON cambia valori OrgYClickTop e OrgYClickBottom
void ModificaClickStanza(int IndiceRoom, int NumeroClick)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;		
	int SizeX,SizeY;
	int x,y;

	pVetRooms=*p2VetRooms;
	SizeX = pVetRooms[IndiceRoom].XSizeBlocks;
	SizeY = pVetRooms[IndiceRoom].ZSizeBlocks;
	for (y =0;y<SizeY;y++) {
		for (x=0;x<SizeX;x++) {
			pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX].WorldFloorClick += NumeroClick;
			pVetRooms[IndiceRoom].pBlockGridCollision[x+y*SizeX].WorldYCeilingClick += NumeroClick;
		}
	}

}	
// trova tutte le stanze collegate, incluse quelle flippate
void TrovaStanzeCollegate(int IndiceRoom)
{
	int Indice;
	int i;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;	
	int n;
	int *pTotRooms = (int*) 0x56C710;

	pVetRooms = *p2VetRooms;
	TotCollegate=0;
	VetCollegate[TotCollegate] = IndiceRoom;
	TotCollegate++;
	Indice = IndiceRoom;


	do {
		Indice = pVetRooms[Indice].LinkedRoom;

		// vedere se gia' presente
		for (i=0;i<TotCollegate;i++) {
			if (Indice == VetCollegate[i]) {
				// terminato
				Indice = -1;
				break;
			}
		}
		

		if (Indice !=-1) {
			// aggiungerla
			VetCollegate[TotCollegate] = Indice;
			TotCollegate++;
			// vedere se questa stanza ha una stanza flippata
			if (pVetRooms[Indice].IndexFlipRoom != -1) {
				n=pVetRooms[Indice].IndexFlipRoom;
				VetCollegate[TotCollegate] = n;
				TotCollegate++;				
			}

		}
	}
	while(Indice != -1);	
	// verificare anche per stanza principale indiceroom se ha una stanza
	// flippata
	if (pVetRooms[IndiceRoom].IndexFlipRoom != -1) {
		n=pVetRooms[IndiceRoom].IndexFlipRoom;
		VetCollegate[TotCollegate] = n;
		TotCollegate++;				
	}
	

}

// rileva stanza clickata
// se il click e' valido restituisce indice di stanza, altrimenti 
// restituisce -1
// nota: se il pulsante e' quello destro e sta centrando la stanza
// gia' selezionata inizia movimento dell'intero blocco
// e esce solo quando viene mollato pulsante destro restituiendo -1

int ClickMouseMappa(int MouseX,int MouseY, int* pVetIndici, int TotIndici)
{
	int		i;
	int		Indice;
	BYTE *pPulsantiMouse = (BYTE *) 0x46DA88;
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	int IndiceTrovato;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	short  *pCordXMouse = (short*) 0x46E4D4;
	short *pCordYMouse = (short*) 0x46E4D6; 
	pVetRooms = *p2VetRooms;
	int NowX, NowY;
	int DifX, DifY;
	int IncX,IncY,IncZ;
	int TestErrore;
	int OrgX,OrgY,OrgZ;
	int BaseX,BaseY, BaseZ;
	int RoomSizeX,RoomSizeZ;
	int RoomOrgX,RoomOrgY,RoomOrgZ;
	int RoomClickTop;
	int SalvaRoom;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;

	
	if (TestMappaInCorso==true) return -1;
	
#ifdef LOG_MAPPA
	sprintf(BufferLog,"Click Mouse 0x%X  TotIndici=%d",
		*pPulsantiMouse, TotIndici);
	InviaLog(BufferLog);
#endif
	

	// trovare la prima stanza (partendo dal fondo)
	// che rientra nelle coordinate del mouse
	IndiceTrovato=-1;

	for (i=TotIndici-1;i>=0;i--) {
		Indice = pVetIndici[i];
		if (VetStanze2d[Indice].Colore != -1 && 
			MouseX >= VetStanze2d[Indice].Posizione.left && 
			MouseX <= VetStanze2d[Indice].Posizione.right && 
			MouseY >= VetStanze2d[Indice].Posizione.top &&
			MouseY <= VetStanze2d[Indice].Posizione.bottom) {

			IndiceTrovato = Indice;
			break;
		}
	}

	//se non e' stato sleezionato niente rimostrare piante
	// in modo che a seconda se click e 'con pulsante destro o sinistro
	// cambiare la trasparenza
	if (IndiceTrovato == -1) {

		// mostrare mappa trasparente
		__asm {
		push 1
		mov eax, 4266F0h    ;MostraPianta2d
		call eax
		add  esp, 04h
		}

		return -1;
	}

	if ((*pPulsantiMouse) & 2) {
#ifdef LOG_MAPPA
		sprintf(BufferLog,"Clickato pulsante destro mouse. Mouse=0x%X  (IndiceTrovato=%d)", 
					*pPulsantiMouse, IndiceTrovato);
		InviaLog(BufferLog);
#endif
		if (IndiceTrovato == *pIndiceRoomAttuale) {
			// gestione movimenti di tutte le stanze collegate a quella
			// selezionata attuale
			// trovare elenco di tutte le stanze collegate
			TrovaStanzeCollegate(IndiceTrovato);



			// ora muovere tutte le stanze in vetcollegate[] totcollegate
			// della differenza tra mousex mousey inziale e i nuovi valori
			// mouse.
			NowX = *pCordXMouse;
			NowY = *pCordYMouse;


			RoomSizeX = pVetRooms[IndiceTrovato].XSizeBlocks;
			RoomSizeZ = pVetRooms[IndiceTrovato].ZSizeBlocks;
			RoomClickTop = pVetRooms[IndiceTrovato].OrgYClickTop;
			roomBackupUndo();
#ifdef LOG_MAPPA
			InviaLog("Stanze prima di spostamento:");

			for (i=0;i<TotCollegate;i++) {
				Indice=VetCollegate[i];
				sprintf(BufferLog,"\tIndice=%d  OrgYClickTop=%d   OrgYClickBottom=%d",
					Indice,
					pVetRooms[Indice].OrgYClickTop,
					pVetRooms[Indice].OrgYClickBottom);
				InviaLog(BufferLog);
			}
#endif


			while ((*pPulsantiMouse) & 2) {
				RoomOrgX = pVetRooms[IndiceTrovato].XBlocksWorld;
				RoomOrgY = pVetRooms[IndiceTrovato].OrgYClickTop;
				RoomOrgZ = pVetRooms[IndiceTrovato].ZBlocksWorld;
				// trovare coordinata assoluta corrispondente a NowX NowY
				

				DifX = NowX - 293;
				DifY = NowY - 10;

				if (DifX > 0 && DifX < 512 && DifY >0 && DifY < 512) {


					// construire coordinata X,Y,Z nel formato blocchi
					// e poi eguagliarla a stanza attuale, creando gli incrmeenti
					// prima calcolare la posizione in click sull'asse y
					BaseY = 511 - DifY;
					BaseY = BaseY - 256;



					BaseX = pVetRooms[IndiceTrovato].XBlocksWorld;
					BaseZ = pVetRooms[IndiceTrovato].ZBlocksWorld;

					switch (TipoMappaVerticale) {
					case 0:
						// sud
						// movimento x cambiare coordinata x
						BaseX = DifX / 5;

						break;
					case 1:
						// ovest
						// movimento x cambia coordinata z
						BaseZ = DifX / 5;

						break;
					case 2:
						// nortd
						// cambia x in modo inverso
						
						BaseX = DifX / 5;
						BaseX += RoomSizeX;
						BaseX = 102 - BaseX;

						break;
					case 3:
						// est
						// cambia z in modo inverso
						BaseZ = DifX / 5;
						BaseZ += RoomSizeZ;
						BaseZ = 102 - BaseZ;

						break;
					}

					// ora calcolare incrementi
					IncX = BaseX - RoomOrgX;
					IncY = BaseY - RoomOrgY;
					IncZ = BaseZ - RoomOrgZ;


					// ora controllare che nessuna stanza esca dai limiti consentiti
					// il problema e' conoscere i limiti per numero di cick
					// se non erro e' + 255 /  - 255 ma in realta' questo 
					// e' il limite teorico massimo
					TestErrore=false;
					for (i=0;i<TotCollegate;i++) {
						Indice = VetCollegate[i];
						
						OrgX = pVetRooms[Indice].XBlocksWorld + IncX;
						if (IncX > 0) OrgX += pVetRooms[Indice].XSizeBlocks;

						if (IncY > 0) 
							OrgY = pVetRooms[Indice].OrgYClickTop + IncY;
						else
							OrgY = pVetRooms[Indice].OrgYClickBottom + IncY;
						
						OrgZ = pVetRooms[Indice].ZBlocksWorld + IncZ;
						if (IncZ > 0) OrgZ += pVetRooms[Indice].ZSizeBlocks;

						if (OrgX < 0 || OrgX >102) {
							TestErrore=true;
							break;
						}

						if (OrgY > 255 || OrgY < -255) {
							TestErrore=true;
							break;
						}

						if (OrgZ < 0 || OrgZ > 120) {
							TestErrore=true;
							break;
						}
					}

					if (TestErrore==false) {


						// ora cambiare tutte le coordinate
						for (i=0;i<TotCollegate;i++) {
							Indice = VetCollegate[i];
							pVetRooms[Indice].XBlocksWorld += IncX;
							pVetRooms[Indice].OrgYClickTop += IncY;
							pVetRooms[Indice].OrgYClickBottom += IncY;
							pVetRooms[Indice].ZBlocksWorld += IncZ;
						}
					
					}
				}

				

					
				// ora aggiornare mappa e controllare di nuovo mouse
				__asm {
					pushad
					push 1	
					mov eax, 4266F0h    ;MostraPianta2d
					call eax
					add  esp, 04h

					mov eax, 41A2B0h    ;AggiornaMouseNow
					call eax
					test eax, eax
					jz   Salta
					mov eax, 436AEEh    ;ElaboraQualcosaMouse
					call eax
Salta:
					popad

				}
				if (IncX != 0 || IncY != 0 || IncZ != 0) {
					MouseX = NowX;
					MouseY = NowY;
				}

				NowX = *pCordXMouse;
				NowY = *pCordYMouse;

			}
#ifdef LOG_MAPPA
			InviaLog("Dopo spostamento e prima di correzione:");
			for (i=0;i<TotCollegate;i++) {
				Indice=VetCollegate[i];
				sprintf(BufferLog,"\tIndice=%d  OrgYClickTop=%d   OrgYClickBottom=%d",
					Indice,
					pVetRooms[Indice].OrgYClickTop,
					pVetRooms[Indice].OrgYClickBottom);
				InviaLog(BufferLog);
			}
#endif
			// dopo modifica posizione
			// ora aggiornare anche le coordinate xpixel world e zpixel world
			// trovare differenza da posizione iniziale di click
			RoomClickTop -= pVetRooms[IndiceTrovato].OrgYClickTop;
			
			// se valore e' negativo allora e'stata alzata
			// invertire segno in modo che il segno meno voglia dire abbassare
			RoomClickTop = -RoomClickTop;

			SalvaRoom = *pIndiceRoomAttuale;	
			
			// ora provare ad usare funzione specifica per alzare o abbassare
			// ogni stanza
			TestMappaInCorso=true;

			for (i=0;i<TotCollegate;i++) {
				Indice = VetCollegate[i];
				pVetRooms[Indice].XPixelWorld = 
					pVetRooms[Indice].XBlocksWorld << 10;
				pVetRooms[Indice].ZOriginePixel = 
					pVetRooms[Indice].ZBlocksWorld << 10;			
#ifdef LOG_MAPPA
				sprintf(BufferLog,"Prima: Indice=%d  ClickYTop=%d  ClickYBottom=%d",
					Indice,
					pVetRooms[Indice].OrgYClickTop,
					pVetRooms[Indice].OrgYClickBottom);
				InviaLog(BufferLog);
#endif

				ModificaClickStanza(Indice,	RoomClickTop);
				
#ifdef LOG_MAPPA
				sprintf(BufferLog,"Dopo: Indice=%d  ClickYTop=%d  ClickYBottom=%d",
					Indice,
					pVetRooms[Indice].OrgYClickTop,
					pVetRooms[Indice].OrgYClickBottom);
				InviaLog(BufferLog);
#endif
			
			}
			TestMappaInCorso=false;

			return IndiceTrovato;


		}
		// pulsante destro premuto
		// se la stanza beccata e' proprio quella attuale
		// salvare mouse atuale e iniziare a seguire i movimenti del mouse
	}
	

	return IndiceTrovato;


}

// carica tutte le texture da mappa tga appena caricata 
// mettendole in pBaseOut
// imposta anche PtrMemTextureNero
// se tutto ok restiutuisce 0
// se c'e' un errore restiuisce 1
int NewInizializzaTexture(StrDatiPerTga *pHeaderTga, BYTE *pBaseOut)
{

	int **p2VetTextureNero = (int**) 0x56C4FC;
	int *VetTextureNero;
	int GrigliaXSrc, GrigliaYSrc;
	int IndiceNow;
	int SizeX, SizeY;
	int SizeTex;
	int	x,y;
	BYTE *pBaseTga;
	int cx,cy;
	BYTE *pSrc;
	BYTE *pDest;
	int GrigliaXDst;
	bool TestNulla;
	int TestUnoNero;
	BYTE Colore;
	int  MaxTexture;
	int *pTotTexture64 = (int*) 0x56C4F0;
	int BloccoTex;
	int SizeRigaSrc,SizeRigaDest;
	int *pProjectVersionNumber = (int*) 0x56C704;

	VetTextureNero = *p2VetTextureNero;
	
	SizeX = pHeaderTga->SizeX;
	SizeY = pHeaderTga->SizeY;

	pBaseTga = pHeaderTga->pMemoriaFileTexture;

	VersioneProject = *pProjectVersionNumber;

	if (VersioneProject != 50) {
		SizeTex = 64;
		MaxTexture=MAX_TAIL_INFOS;
		BloccoTex=1;
		VersioneProject=49;
	}
	else {
		SizeTex = 128;
		MaxTexture=256;
		BloccoTex=4;
	}
		

	GrigliaXSrc = SizeX / SizeTex;
	GrigliaYSrc = SizeY / SizeTex;

	GrigliaXDst = 256 / SizeTex;
	
	IndiceNow=0;
	(*pTotTexture64)=0;

	SizeRigaSrc= SizeX * SizeTex;
	SizeRigaDest = 256 * SizeTex;

	for (y=0;y<GrigliaYSrc;y++) {

		for (x=0;x<GrigliaXSrc;x++) {
			if (IndiceNow >= MaxTexture) {
				MostraMessageBoxNGLE("ERROR: Too many textures - loaded maximum",7);
				return 0;
			}

			// calcolare punto di inizio di texture attuale in griglia sorgente
			pSrc = pBaseTga + y*SizeRigaSrc +x*SizeTex;
			// ora destinazione
			// prima calcolare posizione in griglia destinazione
			cy = IndiceNow / GrigliaXDst;
			cx = IndiceNow % GrigliaXDst;
			// ora calcolare byte di inizio di questa griglia
			pDest = pBaseOut;
			pDest += cy*SizeRigaDest +cx*SizeTex;
			// ora copiare la texture pixel pixel annotando i pixel neri (0)
			TestNulla=true;
			TestUnoNero=0;
		
			for (cy=0;cy<SizeTex;cy++) {
				for (cx=0;cx<SizeTex;cx++) {
					Colore= pSrc[cx];
					pDest[cx]=Colore;
					if (Colore == 0) 
						TestUnoNero=1;
					else
						TestNulla=false;
				}
				pSrc += SizeX;
				pDest += 256;  // una riga di pixel destinazione
			}
			// solo per davide tgliere l'uscita

			 // if (TestNulla == true) return 0;

			VetTextureNero[IndiceNow] = TestUnoNero;
			IndiceNow++;
			(*pTotTexture64) += BloccoTex;
		}
	}
	return 0;

}

void ShowScanCode(BYTE CodiceScansione, WORD TastiShift)
{
	sprintf(BufferLog,"CodiceScansione=0x%X   TastiShift0x%X",
			CodiceScansione,TastiShift);
	InviaLog(BufferLog);

}


// visualizza informazioni sulla selezione attuale
// formato originale era: "Selected Block [ X:%d Y:%d ]"

void ShowInfoSelezione(StrStanzaRoomEdit *pRoom, char *pBuffer)
{
	char BufferTemp[256];
	int  x,y;
	short  floor, ceiling;
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	short *pShort;
	int *pInt;
	int MaxFloor, MinFloor;
	int MaxCeiling, MinCeiling;


	InitTabellaGrid(pRoom);
	LastMexSelezione[0]=0;

	x = pRoom->XBlocksWorld + pRectSelezione2d->left - pRoom->XDisplayOffset -1;
	y = pRoom->ZBlocksWorld + pRectSelezione2d->top - pRoom->ZDisaplyOffset -1;

	sprintf(BufferTemp, "Selected Block [ X:%d Y:%d ", x,y);

	if (pRectSelezione2d->right == 1 && pRectSelezione2d->bottom==1) {
		// vedere che settore non sia esterno a stanza


		// calcolare altezza floor e ceiling di singolo blocco
		PreparaMiaSelezione(1,false);
		DatiShape.pRoom = pRoom;
		CopiaRect(&DatiShape.MiniSel,&DatiShape.MiaSelezione);

		pShort = (short*) ModificaClickSettore(FTO_GET_FLOOR,false);
		floor = *pShort;

		pShort = (short*) ModificaClickSettore(FTO_GET_CEILING, false);
		ceiling = *pShort;
		if (MyPrefRoom.Flags & FPR_EXTRA_INFO_SELECTION) {

			pInt = (int*) ModificaClickSettore(FTO_GET_MAX_FLOOR,false);
			MaxFloor= *pInt;

			pInt = (int*) ModificaClickSettore(FTO_GET_MAX_CEILING,false);
			MaxCeiling= *pInt;

			pInt = (int*) ModificaClickSettore(FTO_GET_MIN_FLOOR,false);
			MinFloor= *pInt;

			pInt = (int*) ModificaClickSettore(FTO_GET_MIN_CEILING,false);
			MinCeiling= *pInt;
			sprintf(BufferLog,"F:%d/%d/%d  C:%d/%d/%d ]",
				floor,MinFloor,MaxFloor,ceiling,MinCeiling,MaxCeiling);

		}
		else {
		
			sprintf(BufferLog,"F:%d C:%d ]", floor,ceiling);
		}
		strcat(BufferTemp,BufferLog);
	

	}
	else {
		
		sprintf(BufferTemp,"W:%d H:%d ]", 
			pRectSelezione2d->right, pRectSelezione2d->bottom);

	}



	strcpy(pBuffer,BufferTemp);
	strcpy(LastMexSelezione,BufferTemp);


}
// salva tot di object code e vettore object con dimensione diversa 2000 o 6000
// a seconda del numero effettivamente usato di codici
void SalvaObjectCodes(FILE *pFile)
{
	int *pN_UsedObjectCodes = (int *) 0x56C970;
	int **Ptr2_ObjectCodeArray = (int**) 0x46B810;
	int *pObjectCodeArray;
	int TotCodici;

	pObjectCodeArray = *Ptr2_ObjectCodeArray;
	
	if ((*pN_UsedObjectCodes) >= 2000) 
		TotCodici=6000;
	else
		TotCodici=2000;

	RoomWrite(&TotCodici,1,4,pFile);
	RoomWrite(pObjectCodeArray,TotCodici,4,pFile);

}

void ShowStatics(char *pBuffer, int TotMoveables)
{
	int *pN_UsedObjectCodes = (int *) 0x56C970;
	int *pTotStatics = (int*) 0x454E6C;
	HWND *pMiaWind = (HWND*) 0x46E559;
	int *N_UsedTriggerCodes = (int *) 0x569BF4;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrStanzaRoomEdit *pRoom;
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;

	pVetRooms = *p2VetRooms;
	
	pRoom = &pVetRooms[*pIndiceRoomAttuale];

	sprintf(pBuffer,"Moveables: %d of 1024   Statics: %d    Items: %d of 6000",
		TotMoveables,*pTotStatics, *pN_UsedObjectCodes);
	// costruire anche informazioni precedenti per metterle in LastInfoRoom
	
	sprintf(LastInfoRoom,"NGLE - X:%d  Y:%d  Size:%dx%d  Floor:%d  Ceiling:%d  %s",
		pRoom->XBlocksWorld, pRoom->ZBlocksWorld,
		pRoom->XSizeBlocks -2, pRoom->ZSizeBlocks -2,
		pRoom->OrgYClickBottom , pRoom->OrgYClickTop ,
		LastMexSelezione);

	sprintf(OldFrameInfoText,"Room X:%d  Y:%d  Size:%dx%d  Floor:%d  Ceiling:%d  %s\r\nTriggers: %d of 512 %s\r\nDoors: %d ",
		pRoom->XBlocksWorld, pRoom->ZBlocksWorld,
		pRoom->XSizeBlocks -2, pRoom->ZSizeBlocks -2,
		pRoom->OrgYClickBottom , pRoom->OrgYClickTop ,
		LastMexSelezione,
		*N_UsedTriggerCodes,
		pBuffer, 
		pRoom->N_Doors);
	
	if (TestShowTitleInfo == true) {
		SetWindowText(*pMiaWind, LastInfoRoom);
	}



}
// restituisce base costanti sulla base di IdPlugin 
StrNGConstants * TrovaBaseCostanti(int IdPlugin)
{
	int i;

	if (IdPlugin==0) {
		return &MainBaseCostanti;
	}

	for (i=0;i<GlobNgle.TotPluginNgle;i++) {
		if (GlobNgle.VetPluginNgle[i].PluginId == IdPlugin) {
			return &GlobNgle.VetPluginCostants[i];
		}
	}

	sprintf(BufferLog, "WARNING: cann't locate the plugin with id=%d", IdPlugin);
	MioMessageBox(BufferLog, "Error");

	return &MainBaseCostanti;


}

// Analizza BaseTempTrigger per stabilire se con i valori attuali
// e' prevista la visualizzazione dell'extra combo per ospitare
// dati memorizzati nella parte alta del valore timer oppure nel campo pulsanti
// questo puo' accadere quasndo c'e' flipeffect che richiede extra field
// o con condition
// RETURN
// se non c'e' extra timer o extra buttons restituisce -1
// se c'e'extra timer restituisce l'indice di lista di BaseCostanti
// con i valori da visualizzare in combo_extra
// Imposta TestExtraButtons = true per extra timer
// e	   TestExtraTimer = true per extra buttons
int TrovaExtraField(void)
{
	int i;
	int Numero;
	StrNGConstants * pBaseCostanti;

	BaseTempTrigger.TestExtraButtons=false;
	BaseTempTrigger.TestExtraTimer =false;

	pBaseCostanti= TrovaBaseCostanti(BaseTempTrigger.IdPlugin);

	// prima possibilita': il triggerwhat e' flipeffect e il numero
	// di flipeffect attuale e' uno che ha una lista Extra
	if (BaseTempTrigger.TriggerWhat == 9) {
		// trigger FLIPEFFECT
		Numero = BaseTempTrigger.Valore;

		for (i=0;i<pBaseCostanti->TotSezioni;i++) {
			if (pBaseCostanti->pVetSezioni[i].NumeroSezione==Numero && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_EFFECT &&
				pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_EXTRA) {
				BaseTempTrigger.TestExtraTimer=true;
				return i;
			}
		}
		return -1;
	}

	// seconda possibilita': trigger attivazione e' condizione e il numero
	// di condizione attuale ha una lista extra
	if (BaseTempTrigger.TriggerActivation ==0x0C ) {
		// trigger attivazione e' CONDITION
		Numero = BaseTempTrigger.Timer & 0x00ff;

		for (i=0;i<pBaseCostanti->TotSezioni;i++) {
			if (pBaseCostanti->pVetSezioni[i].NumeroSezione==Numero && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_CONDITION &&
				pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_BUTTONS) {
				BaseTempTrigger.TestExtraButtons=true;
				return i;
			}
		}
		return -1;
	}

	// ci sarebbe anche la terza possibilita'
	// trigger what e' action e nei bit bassi di extra timer c'e' 
	// la condizione e in quelli alti ci puo' essere extra value.

	if (BaseTempTrigger.TriggerWhat == 0xB) {
		// trigger what ACTION
		Numero = BaseTempTrigger.Timer & 0x00ff;


		for (i=0;i<pBaseCostanti->TotSezioni;i++) {
			if (pBaseCostanti->pVetSezioni[i].NumeroSezione==Numero && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_ACTION &&
				pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_EXTRA) {
				BaseTempTrigger.TestExtraTimer=true;
				return i;
			}
		}

	}

	return -1;
}

// visualizza in combo box (per finestra trigger) l'elenco dei sound samples
// se gruppo = 0 visualizza i primi 256 suoni (da 0 a 255) 
// altrimenti i successivi
void CaricaSoundSamples(HWND WindCombo, int Gruppo)
{
	int i;
	char BufferTemp[256];
	int Indice;
	char NomeWav[256];
	char *pMexOk;
	char *pFileWav;
	int j;
	char **VetParti;
	int TotParti;
	int *TotSoundEffects = (int*) 0x56F48C;
	char ***p2VetSoundEffects = (char***) 0x56F494 ;
	char **VetSoundEffect;
	int  Minimo, Massimo;
	int NOrdine;

	
	if (Gruppo == 2) {
		Minimo=0;
		Massimo = 1024;
	}else {
		Minimo = 0 + Gruppo*256;
		Massimo = 256 + Gruppo*256;
	}
	if (Massimo > *TotSoundEffects) Massimo = *TotSoundEffects;

	VetSoundEffect = *p2VetSoundEffects;

	// carica in VetList[] VetPunta[] TotList tutti i valori dell'elenco
	
	NOrdine=0;
	for (i=Minimo;i<Massimo;i++) {

		pFileWav = TrovaSoundWav(VetSoundEffect[i]);
		VetParti=Split(pFileWav,' ',&TotParti,NULL,NULL,false);
		for (j=0;j<TotParti;j++) {
			sprintf(NomeWav,"sound\\samples\\%s.wav", VetParti[j]);

			if (EsisteFile(GetFileTrle(NomeWav))==false) break;
		}
		if (j == TotParti && j != 0)
			pMexOk= "Ok";
		else
			pMexOk= "MISSING";


		sprintf(BufferTemp,"%-25s  %3d  %-8s %s", 
					VetSoundEffect[i], 
					i, 
					pMexOk,
					pFileWav);


		Indice=SendMessage(WindCombo,CB_ADDSTRING,0, 
				(LPARAM) BufferTemp);
		SendMessage(WindCombo,CB_SETITEMDATA,Indice, NOrdine);
		NOrdine++;

	}

}
// cerca di localizzare il nome lingua attuale
char *TrovaLinguaAttuale(void)
{
	static char* VetNomiLingue[] = {"english", "FRENCH", "GERMAN",
			"ITALIAN", "SPANISH", "US", "JAPAN", "DUTCH", NULL};
	int i;
	char NomeFile[256];

	i=0;
	while (VetNomiLingue[i] != NULL) {
		sprintf(NomeFile, "%s\\%s.dat", Dir_Trle, VetNomiLingue[i]);
		if (EsisteFile(NomeFile)==true) {
			return VetNomiLingue[i];
		}
		i++;
	}
	return VetNomiLingue[0];

}
// carica direttamente da file language.txt le stringhe nel combo box WindCombo

void CaricaStringheScript(HWND WindCombo, bool TestNG, int IndiceSezione, int MaxStringIndex)
{
	char *pNomeLingua;
	char NomeFile[256];
	FILE *pFile;
	char BufLinea[512];
	int IndiceStringa;
	bool TestDentro;
	DWORD i,j;
	char *pChar;
	char *pNomeSezione;
	static char* VetSezioni[] = {"[Strings]", "[PSXStrings]",
		"[PCStrings]", "[ExtraNG]"};
	const int TotSezioni = 4;
	char StrNumero[20];
	int Indice;
	bool TestBlocco256;


	TestBlocco256=false;
	if (MaxStringIndex == 511) TestBlocco256=true;

	pNomeLingua = TrovaLinguaAttuale();
	sprintf(NomeFile,"%s\\script\\%s.txt",Dir_Trle,pNomeLingua);
	if (EsisteFile(NomeFile)==false) {
		sprintf(BufferLog,"ERROR: cann't find file: %s",NomeFile);
		InviaLog(BufferLog);
		MessageBox(NULL,BufferLog,"ERROR",0);
		return;
	}
	

	pFile = fopen(NomeFile,"rt");


	TestDentro=false;

	IndiceStringa=0;


	pNomeSezione = VetSezioni[IndiceSezione];

	while (feof(pFile) == 0) {
		BufLinea[0]=0;
		fgets(BufLinea,511,pFile);
		RemoveNewLine(BufLinea);

		if (TestDentro == false) {
			// controllare solo se linea attuale fa entrare nella sezione
			if (Search(BufLinea,pNomeSezione,ts_Avanti,NULL)!= NULL) {
				TestDentro=true;
			}
		}else {
			// siamo dentro sezione.
			// come prima cosa controllare che questa riga
			// non sia prima riga di sezione successiva
			for (i=0;i<TotSezioni;i++) {
				if (Search(BufLinea,VetSezioni[i],ts_Avanti,NULL)!=NULL) {
					// terminato tutto
					fclose(pFile);
					return;
				}
			}
			// controllare che non sia vuota
			pChar = Trim(BufLinea);
			if (strlen(pChar) > 0) {
				// ora analisi per indice se e' di tipo ng
				if (TestNG==true) {
					i=0;

					for (j=0;j<strlen(BufLinea);j++) {
						if (BufLinea[j] == ':') break;
						if (BufLinea[j] >= '0' && BufLinea[j] <= '9') {
							StrNumero[i++] = BufLinea[j];
						}
					}
					StrNumero[i]=0;
					IndiceStringa=-1;
					sscanf(StrNumero,"%d",&IndiceStringa);
					if (IndiceStringa == -1) {
						sprintf(BufferLog,"ERROR scanning file [%s]: illegal string index in line:\r\n\r\n%s",
							SoloNome(NomeFile),BufLinea);
						MessageBox(NULL,BufferLog,"ERROR",0);
						fclose(pFile);
						return;
					}
				}
				if (IndiceStringa > MaxStringIndex) {
					fclose(pFile);
					return;
				}

				if (TestBlocco256) IndiceStringa -= 256;
				if (IndiceStringa >= 0) {
					Indice=SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufLinea);
					SendMessage(WindCombo,CB_SETITEMDATA,Indice, IndiceStringa);
				}
				if (TestNG==false) IndiceStringa++;


			}


			
		}
	}

	fclose(pFile);


}
// restituisce un testo che descrive i pulsanti trigger attivati:
// cosa tipo "[1][2][3][4][5] [OneShot]"
char *CalcolaMascheraPulsanti(int Numero)
{
	int i;
	static char Buffer[80];
	int Maschera;
	char MiniBuf[30];


	Buffer[0]=0;

	for (i=1;i<=5;i++) {
		Maschera = 1 << i;
		if (Maschera & Numero) {
			sprintf(MiniBuf, "[%d]  ", i);
			strcat(Buffer,MiniBuf);
		
		}

	}

	if (Numero & 0x1) strcat(Buffer,"[One-Shot] ");

	if (Numero == 0x3e || Numero == 0x3f) {
		strcat(Buffer, " (Immediate activation)");
	}
	return Buffer;
	
}

// Trova tuti gli "effetti" di livello
//restituisce gli indici relativi a pvetbaseoggetti
// nota: il valore di TipoEffetto e' quello che si trova
// nel campo pVetBaseOggetti[Indice].TypeObject

WORD *TrovaEffetti(WORD TipoEffetto, int *pTotTrovati)
{
	static WORD VetTrovati[1024];
	int TotTrovati;
	int i;
	int j;
	int Indice;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetRooms = *p2VetRooms;

	TotTrovati=0;
	*pTotTrovati=0;
	
	for (i = 0;i< *pTotRooms; i++) {
		// cercare in stanza con indice i
		if (pVetRooms[i].TestEmptySlot == 0) {
			for (j=0;j<pVetRooms[i].N_Effects;j++) {
				Indice = pVetRooms[i].VetEffectCode[j];
				if (pVetBaseOggetti[Indice].TypeObject == TipoEffetto) {
				
					// trovato effetto richiesto
					VetTrovati[TotTrovati++] = Indice;
				}
			}

		}
	}
	*pTotTrovati = TotTrovati;
	return &VetTrovati[0];


}
// cerca in tutte le stanze gli oggetti di tipo Oggetto
// con slot = Slot
// restituisce vettore di word con gli indici e imposta in pTotTrovati
// il totale di item trovati
// nota: gli indice ottenuti si usano per accedere a pVetBaseOggetti[..]
WORD * TrovaOggetti(WORD Slot,  int *pTotTrovati)
{
	static WORD VetTrovati[1024];
	int TotTrovati;
	int i;
	int j;
	int Indice;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetRooms = *p2VetRooms;

	TotTrovati=0;
	*pTotTrovati=0;
	
	for (i = 0;i< *pTotRooms; i++) {
		// cercare in stanza con indice i
		if (pVetRooms[i].TestEmptySlot == 0) {

			// cercare in objandtriger
			for (j=0;j<pVetRooms[i].N_ObjAndTriggers;j++ ) {
				// trovare indice a 
				Indice= pVetRooms[i].VetObjAndTriggerCodes[j];

				if (pVetBaseOggetti[Indice].TypeObject == 8 && 
						pVetBaseOggetti[Indice].SlotIndex == Slot ) {
						// trovao oggetto dello stesso tipo di slot.
						VetTrovati[TotTrovati++] = Indice;	
				}
			}
			
		}
	}

	*pTotTrovati=TotTrovati;
	return &VetTrovati[0];
}
// restituisce stringa con indice = indice
// le strnghe del programma sono nella sezione <START_TEXTS in ng_constnats
char *GetText(int Indice)
{
	StrRecordSezione *pRec;
	StrNGConstants * pBaseCostanti;
	int i;

	pBaseCostanti= &MainBaseCostanti;

	if (pBaseCostanti->IndiceSezTesti == -1) return "MISSING";

	pRec = &pBaseCostanti->pVetSezioni[pBaseCostanti->IndiceSezTesti];

	for (i=0;i<pRec->TotValori;i++) {
		if (pRec->pVetValori[i].Numero == Indice) return pRec->pVetValori[i].pDescrizione;
	}

	return "Not Found Text";


}
// riempie in combo box la lista TipoLista secondo le costanti TLS_...
// #define TLS_ALLMOVEABLES 1    -->  Tutti i moveables del livello
// #define TLS_TRIGGERMOVEABLES 2  --> Tutti i moveable attivati da trigger
//									   di parametro (triggerwhat)
// #define TLS_LISTINDEX 3  ---> dati in pBaseCostanti->pVetSezioni[paramwetro]
// TipoIndiceValore (segnala da quale campo di trigger si prende l'indice
//				per selezionare il valore attivo nella lista)
//		TIV_TIMER_ALL = usa il timer, l'intero valore
//		TIV_TIMER_BASSO = usa il timer, la parte bassa 0x00ff
//		TIV_TIMER_ALTO = usa il timer, i bit alti      0x7f00
//      TIV_OBJECT = usa il campo VALUE (oggetto da attivare) del trigger
//			
void RiempieComboListaTriggers(HWND hDialogo, HWND WindCombo, int TipoLista, 
							int Parametro, int TipoIndiceValore)
{

	int *pN_UsedObjectCodes = (int *) 0x56C970;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrFlyByArrayRoomEditor *pVetFlyBy;
	int n;
	StrRecordSezione *pSez;
	int TotItems;
	bool TestMainList;
	char *pTex;
	static char *VetNomeNum[] = {"Alfa", "Beta", "Delta", "Timer"};
	static char *VetNomeTex[] = {"Text1", "Text2", "Text3", "Text4"};
	WORD Codice;
	WORD VetSequenze[512];
	int TotSequenze;
	int Sequenza;
	int IndexMin, ValueMin;
	int TotFlyBy;
	char *pDesc;
	WORD * pTotOggettiMoveable = (WORD *) 0x56D980;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	int *pTotRooms = (int*) 0x56C710;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;
	pVetTriggers = *P2_VetTriggers;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetRooms = *p2VetRooms;
	StrStanzaRoomEdit *pRoom;
	WORD *VetTrovati;
	int TotTrovati;
	WORD *pVetIndici;
	int TotIndici;
	bool TestEffetti;
	int i,j,w;	
	int Inizio,Fine;

	int IndiceSezione;
	int Indice;
	char BufMini[20];
	int IndiceRoom;
	char MiniBuf[20];
	char *pNomeOggetto;
	int z;
	bool TestTarget;
	int VetOggetti[10];
	int TotOggetti;
	int IndicePredefinito;
	int *pTotSlot = (int*) 0x56D9F0;
	bool TestMoveable;
	bool TestParametro;
	bool TestColors;
	short IndiceAttiva;
	int Numero;
	int **Ptr2_ObjectCodeArray = (int**) 0x46B810;
	int *pObjectCodeArray;
	HWND AltroCombo;
	StrItemsRoom *pObj;
	char *pChar;
	char *pTipoCond;

	static POINT VetGruppiInventario[] = {
		{0xaf,0xf6}, {0xf8,0x103},{0x128,0x131}, 
		{0x15d,0x175}};
	const int TotGruppiInventario = 4;
	int  x,y;
	int MaxGrid;
	int Valore;
	/* era:
	static char *VetVert[] = {"horizontal", "vertical"};
	static char *VetDiag[] = {"bottom-up", "top-down"};
	*/
	char *VetVert[2];
	char *VetDiag[2];
	int IndiceVert;
	int IndiceInv;
	float Result;
	StrNGConstants * pBaseCostanti;
	pObjectCodeArray = *Ptr2_ObjectCodeArray;
	static DWORD VetColori[] = {
			0x000000,0x0F5C83C, 0x78C470,0x0CACCE6,
			0x804000,0x404040,0x0F3E8EC,0x40C0,
			0x8000,0x96AC9D,0x808080,0x0CCA37B,
			0x0B1A28C,0x0DFBF,0x6FFFDF,0x0F4D898,
			0x0F8C03C,0x0FC0000,0x0C65F57,0x0E29776,
			0x0F8EBCE,0x1E10,0x0FADEA7,0xDAAF75,
			0x0E1BF4E,0x4D8C8D,0x4B59A,0x0FFAE00};
	const int TotColori = 28;
	static char * VetNomiColori[] = {"Black", "Gold", 
			"Lime Green","Light blue", "Maroon", "Dark Gray",
			"White", "Blue", "Dark Green", "Gray light",
			"Gray", "Khaki", "Wood", "Green", "Aqua",
			"Bisque 1", "Orange", "Red", "Sienna", "Salmon",
			"Corn Silk", "Almost Black", "Bisque 2",
			"Burly Wood", "Dark Khaki", "Teal", "Dark Turquoise",
			"Dark Orange"};


	BYTE *pByte;

	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetRooms = *p2VetRooms;
	pVetTriggers = *P2_VetTriggers;

	pVetRooms=*p2VetRooms;

	// pRoom->VetEffectCode 
	TestMoveable=false;
	TestParametro =false;
	TestEffetti =false;
	TestTarget=false;
	SendMessage(WindCombo,CB_RESETCONTENT,0,0);

	// scoprire che tipo di lista e'
	IndiceSezione=-1;
	TotOggetti=0;
	IndicePredefinito=0;
	// caricare alcuni testi
	VetVert[0] = GetText(2);
	VetVert[1] = GetText(3);

	VetDiag[0] = GetText(4);
	VetDiag[1] = GetText(5);


	switch (TipoLista)
	{
	case TLS_ALLMOVEABLES:
		VetOggetti[TotOggetti++] = 8;
		TestMoveable=true;
		break;
	case TLS_TRIGGERMOVEABLES:
		// oggetti in accordo con tipo trigger
		TestMoveable=true;
		switch (Parametro) 
		{
		case 0:
			// object
			// se pero' l'oggetto era una fly by visualizzare
			// solo oggetti flycamera
			Indice = BaseTempTrigger.Valore;
			if (pVetBaseOggetti[Indice].TypeObject == 0x4040) {
				VetOggetti[TotOggetti++] = 0x4040;
				TestEffetti=true;
				break;

			}
		case 0x0b:
			// action
			VetOggetti[TotOggetti++] = 8;
			TestMoveable=true;
			break;
		case 1:
			// camera
			VetOggetti[TotOggetti++] = 0x4800; // type_camera
			VetOggetti[TotOggetti++] = 0x4080; // type_camera fixed
			TestEffetti=true;
			break;
		case 6:
			// target
			// qui ci vorrebbe oggetti ma facendo poi il controllo per 
			// limitarlo solo a target_camera
			VetOggetti[TotOggetti++] = 8;
			TestMoveable=true;
			break;
		case 0x0c:
			// flyby
			IndicePredefinito = LST_FLYBY_SEQ;
			TestMoveable=false;
			break;
		case 2:
			// sink
			VetOggetti[TotOggetti++] = 0x4400;
			TestEffetti=true;
			break;
		}
		break;
		case TLS_LISTINDEX:
			// indice a lista
			pBaseCostanti = TrovaBaseCostanti(BaseTempTrigger.IdPlugin);

			if (pBaseCostanti->pVetSezioni[Parametro].ListPredefinito == 0) {
				IndiceSezione=Parametro;
				break;
			}

			// lista predefinita
			IndicePredefinito = pBaseCostanti->pVetSezioni[Parametro].ListPredefinito;
			break;

	}
	if (IndiceSezione > -1) {
		// e' una lista conentua in BaseCostanti 
		pBaseCostanti = TrovaBaseCostanti(BaseTempTrigger.IdPlugin);
		pSez = &pBaseCostanti->pVetSezioni[IndiceSezione];
		// vedere se si tratta di descrizione principale di trigger trng: flipeffect, action o condition

		TestMainList=false;

		// FLIPEFFECT ?
		if (BaseTempTrigger.TriggerWhat== 9 && TipoIndiceValore == TIV_OBJECT) {
			// e' descrizione main di flipeffects
			TestMainList=true;
		}

		// ACTION ?
		if (BaseTempTrigger.TriggerWhat == 11 && 
			(TipoIndiceValore == TIV_TIMER_ALL || TipoIndiceValore == TIV_TIMER_BASSO)) {
			// e' descrizione main di action
			TestMainList=true;
		}

		// CONDITION?
		if (BaseTempTrigger.TriggerActivation == 12 && 
			(TipoIndiceValore == TIV_TIMER_ALL || TipoIndiceValore == TIV_TIMER_BASSO)) {
			// e' descrizione main di condition
			TestMainList=true;
		}
		

		for (i=0;i<pSez->TotValori;i++) {
			pDesc = pSez->pVetValori[i].pDescrizione;


			if (pSez->Flags & FTL_HIDE_NUMBERS) {
				strcpy(BufferLog, pDesc);
			}
			else {
				if (pSez->Flags & FTL_MOVE_NUMBERS) 
					sprintf(BufferLog,"%s (%d)", 
							pDesc,
							pSez->pVetValori[i].Numero);
				else

					sprintf(BufferLog,"%3d: %s",
							pSez->pVetValori[i].Numero,
							pDesc);
			}
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0, (LPARAM) BufferLog);

			Valore=pSez->pVetValori[i].Numero;

			if (TestMainList==true) {
				// aggiungere (temporaneamente) anche indice originale
				Valore |= (i << 16);

			}
			SendMessage(WindCombo,CB_SETITEMDATA, Indice, Valore);
		}	
		// terminato inserimento di tutta la lista
		// ora costrurire in VetDescrizoniRemark/Help[]/TotDescrizioniRemark la lista dei puntatori
		// alla descrizione completa di ogni trigger, nell'ordine finale (ordinato)
		if (TestMainList==true) {
			TotItems = SendMessage(WindCombo, CB_GETCOUNT,0,0);
			TotDescrizioniRemark=TotItems;
			// azzerare valori residui
			for (i=0;i<1024;i++) {
				VetDescrizioniRemark[i]=NULL;
				VetDescrizioniHelp[i]=NULL;
			}
			for (i=0;i<TotItems;i++) {
				Valore=SendMessage(WindCombo,CB_GETITEMDATA,i,0);
				// riottenere indice originali a psez
				z = (Valore >> 16) & 0x7fff;
				Valore &= 0xffff;
				// mmetterlo in vetdewscrizioni indicizzato con valore (numero) di trigger:
				VetDescrizioniRemark[Valore] = pSez->pVetValori[z].pMexRemark;
				VetDescrizioniHelp[Valore] = pSez->pVetValori[z].pMexHelp;

				// aggiornare totdescrizinifull
				if ((Valore+1) > TotDescrizioniRemark) {
					TotDescrizioniRemark = Valore+1;
				}
				// ora rimuovere informazioni su idnice di item data
				
				SendMessage(WindCombo, CB_SETITEMDATA, i, Valore);


			}


		}

	}

	if (IndicePredefinito != 0) {
		// e' una lista predefinita
		switch (IndicePredefinito) {
		case LST_PERCENTAGE:
			// lista di percentuali da usare per varie cose

			Indice=0;

			// adesso mettere lista tra 0.1 e 0.9
			for (i=1;i<10;i++) {
				pTex = GetText(6);
				sprintf(BufferLog,"  0.%d %s", i, pTex);
				j = SendMessage(WindCombo, CB_ADDSTRING, 
							0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA, j, Indice);
				Indice++;
			}
			// adesso mettere lista da 1 a 9
			for (i=1;i<10;i++) {
				sprintf(BufferLog,"%3d %s", i, pTex);
				j = SendMessage(WindCombo, CB_ADDSTRING, 
							0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA, j, Indice);
				Indice++;
			}
			// prima lista da 10 fino a 100 % 
			for (i=10;i<=100;i+=10) {
				
				sprintf(BufferLog,"%3d %s", i, pTex);
				j = SendMessage(WindCombo, CB_ADDSTRING, 
							0, (LPARAM) BufferLog);
				SendMessage(WindCombo, CB_SETITEMDATA, j, Indice);
				Indice++;
			}



			break;
	
	
		case LST_SLOTS:

			// tutti gli slot dei moveables
			// (ignorare quelli vuoti e quelli statici)
			for (i=0;i<*pTotSlot;i++) {
				if (pVetSlotOggetti[i].Tipo == 0x10) {
					sprintf(BufferLog,"%-24s  Id=%d",
						pVetSlotOggetti[i].pNome, i);
					j = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				}
			}
			break;
		case LST_STATIC_SLOT:
			for (i=0;i<*pTotSlot;i++) {
				if (pVetSlotOggetti[i].Tipo == 0x110) {
					n = i - *pTotOggettiMoveable;
					sprintf(BufferLog,"%-24s  Id=%d",
						pVetSlotOggetti[i].pNome, 
						n );
					j = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, n);
				}
			}		
			break;
		case LST_TRANSPARENCY32:
			pTex=GetText(7);

			// mettere 31 valore distribuiti tra 0 e 127
			for (i=0;i<31;i++) {
				n = i * 4;
				sprintf(BufferLog,"%s = %3d", pTex, n);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

			}
			// ora aggiungere anche 126 ossia valore massimo usando 31
			n = 126;
			sprintf(BufferLog,"%s = %3d", pTex, n);
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
			SendMessage(WindCombo,CB_SETITEMDATA,j, 31);
			break;
		case LST_BIT_LIST:
			for (i=0;i<32;i++) {
				Valore = (1 << i);
				sprintf(BufferLog,"Bit %2d  ($%08X ; %d)", i, Valore, Valore);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			break;

		case LST_NEGATIVE_NUMBERS:
			// il numero andra' poi modiicato cosi':
			// Valore = Timer - 128
			pTex=GetText(8);
			for (i=0;i<128;i++) {
				Valore = i - 128;
				sprintf(BufferLog,"%s %+3d", pTex, Valore);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			break;
		case LST_COLLISION_FLOOR:
			pTex = GetText(9);  // No change
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) pTex);
			SendMessage(WindCombo,CB_SETITEMDATA, j, 0);
			pTex = GetText(10); // Decrease floor collision by %2d clicks

			for (i=1;i<64;i++) {
				sprintf(BufferLog,pTex, i);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			pTex = GetText(40); // Increase floor collision by %2d clicks

			for (i=1;i<64;i++) {
				sprintf(BufferLog,pTex, i);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i + 64);
			}
			break;

		case LST_COLLISION_CEILING:
			pTex = GetText(9); // No change
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) pTex);
			SendMessage(WindCombo,CB_SETITEMDATA, j, 0);
			pTex = GetText(11); // Decrease ceiling collision by %2d clicks

			for (i=1;i<64;i++) {
				sprintf(BufferLog, pTex, i);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			pTex = GetText(12); // Increase ceiling collision by %2d clicks
			for (i=1;i<64;i++) {
				sprintf(BufferLog, pTex, i);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i + 64);
			}
			break;
		case LST_MEM_INVENTORY_INDICES:
			i =0;
			while (VetNomiInventoryRecords[i]) {
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, 
							(LPARAM) VetNomiInventoryRecords[i]);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				i++;
			}
			break;

		case LST_MICRO_CLICKS:
			for (i=0;i<128;i++) {
				Valore = (i+1) * 8;
				pTex = GetText(13);   // Units
				sprintf(BufferLog,"%s = %4d ", pTex, Valore);
				switch (Valore) {
				case 256:
					strcat(BufferLog,"(One click)");
					break;
				case 512:
					strcat(BufferLog, "(Two clicks)");
					break;
				case 768:
					strcat(BufferLog, "(Three Clicks)");
					break;
				case 1024:
					strcat(BufferLog, "(One sector)");
					break;
				}
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);


			}
			break;

		case LST_MEMORY_ANIMATION:
			// memoria per record animazione
			// solo indice a cui poi va aggiunto MEM_TIPO_ANIMATION
			i =0;
			while (VetMemoryAnimation[i].pDescrizione) {

				switch (VetMemoryAnimation[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}
				sprintf(BufferLog, "%s (%s)", VetMemoryAnimation[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;			

		case LST_MEMORY_INVENTORY:
			// memoria per record di inventario
			i= 0;
			while (VetMemoryInventory[i].pDescrizione) {
				switch (VetMemoryInventory[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}
				sprintf(BufferLog, "%s (%s)", VetMemoryInventory[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;
		case LST_MEMORY_SLOT:
			// memoria per struttura slot
			// solo indici a cui poi va aggiunto MEM_TIPO_SLOT
			i =0;
			while (VetSlotMemory[i].pDescrizione) {

				switch (VetSlotMemory[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}
				sprintf(BufferLog, "%s (%s)", VetSlotMemory[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;
		case LST_MEMORY_CODE:
			// memoria per codice
			// nota: agli indici andra' poi aggiunto MEM_TIPO_CODICE
			i=0;
			while (VetCodeMemory[i].pDescrizione) {

				switch (VetCodeMemory[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}
				sprintf(BufferLog, "%s (%s)", VetCodeMemory[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;				
		case LST_MEMORY_ITEM:
			// memoria per moveable
			// nota: agli indici andra' aggiunto MEM_TIPO_ITEM
			i=0;
			while (VetItemMemory[i].pDescrizione) {
				switch (VetItemMemory[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}
				sprintf(BufferLog, "%s (%s)", VetItemMemory[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;
		case LST_MEMORY_SAVE:
			// visualziza lista di indirizzi critci per savegame
			i=0;
			while (VetMemorySavegame[i].pDescrizione) {


				switch (VetMemorySavegame[i].TipoSize) {
				case 'B':
					pChar = "Byte";
					break;
				case 'S':
					pChar = "Short";
					break;
				case 'L':
					pChar = "Long";
					break;
				}

				sprintf(BufferLog, "%s (%s)", VetMemorySavegame[i].pDescrizione, 
						pChar);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

				i++;
			}
			break;

		case LST_VAR_TEXT:
			// lista per variabili testuali


			for (i=0;i<4;i++) {
				Codice = VAR_TYPE_TEXT + i;
				sprintf(BufferLog,"%s (79 characters)", VetNomeTex[i]);


				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

			}
			// aggiungere lastinputtext
			Codice = VAR_SPECIAL_INPUT_TEXT; // codice speciale da convertire
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) "Last Input Text  (79 characters)");
			SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

			Codice = VAR_SPECIAL_BIG_TEXT; // codice speciale da convertire
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) "BigText (319 characters)");
			SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);


			break;
		case LST_LONG_STORE:
			// visualizza solo variabili long e usa come valori da 0 ..
			// poi dovra' essere aggiunto 0x1c0 per ottenere i codici completi
			for (i=0;i<16;i++) {
				sprintf(BufferLog,"Store Long %c", 'A'+i);
				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			break;


		case LST_VAR_STORE:

			// inserire lista e descrzione per variabili store.
			// nota: al valoreottenuto bisognera' aggiungere VAR_TYPE_STORE
			for (i=0;i<16;i++) {
				// locale long
				Codice = STORE_TYPE_LONG+i;
				sprintf(BufferLog, "Store Long %c", 'A'+i);

				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

				// variabili short
				for (z =0;z<2;z++) {

					// 	------ variabili local short -----
					Codice = STORE_TYPE_SHORT + i*2 + z;
					sprintf(BufferLog, "Store Short %c%d", 'A'+i, z+1);

					j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
				}

				// variabili byte
				for (z =0;z <4;z++) {
					// ------ variabili local byte --------
					Codice = STORE_TYPE_BYTE + i*4+ z;
					sprintf(BufferLog, "Store Byte %c%d", 'A'+i, z+1);

					j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
				}
			
			}


			break;

		case LST_VAR_NORMALS:
			// inssrire descerizione e codici per variabili numeriche
			// (escluse quelle store)
			// inserire tutte le variabili locali


			for (i=0;i<4;i++) {
				// locale long
				Codice = VAR_TYPE_LOCAL + VAR_TYPE_LONG + i;
				sprintf(BufferLog, "Local Long %s", VetNomeNum[i]);


				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

				// globale long
				Codice = VAR_TYPE_LONG + i;
				sprintf(BufferLog, "Global Long %s", VetNomeNum[i]);

				j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
				
				// non effettuare frammenti per timer (i = 3)
				if (i != 3) {

					// variabili short
					for (z =0;z<2;z++) {

						// 	------ variabili local short -----
						Codice = VAR_TYPE_LOCAL + VAR_TYPE_SHORT + i*2 +z;
						sprintf(BufferLog, "Local Short %s%d", VetNomeNum[i], z+1);
						j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

						// 	------ variabili global short -----
						Codice = VAR_TYPE_SHORT + i*2+ z;
						sprintf(BufferLog, "Global Short %s%d", VetNomeNum[i], z+1);
						j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
					}

					// variabili byte
					for (z =0;z <4;z++) {
						// ------ variabili local byte --------
						Codice = VAR_TYPE_LOCAL + VAR_TYPE_BYTE + i*4+ z;
						sprintf(BufferLog, "Local Byte %s%d", VetNomeNum[i], z+1);
						j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

						// ------ variabili global byte --------
						Codice = VAR_TYPE_BYTE + i*4+z;
						sprintf(BufferLog, "Global Byte %s%d", VetNomeNum[i], z+1);
						j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
					}
				}
			}
			// adesso la variabile speciale CurrentValue
			// usando valore solo temporaneo VAR_SPECIAL_CURRENT_VALUE (0xff)
			Codice = VAR_SPECIAL_CURRENT_VALUE;
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) "Current Value");
			SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);

			Codice = VAR_SPECIAL_INPUT_NUMBER;
			j = SendMessage(WindCombo, CB_ADDSTRING, 0, (LPARAM) "Last Input Number");
			SendMessage(WindCombo,CB_SETITEMDATA,j, Codice);
			break;


		case LST_FLYBY_SEQ:
			// creare la lista di sequenza visualizzando anche i dati
			// della rima camera
			pVetFlyBy = CreaListaFlyBy(&TotFlyBy);
			TotSequenze=0;

			for (i=0;i<TotFlyBy;i++) {
				Sequenza = pVetFlyBy[i].Sequenza;
				pTex = GetText(14); // Sequence
				// ignorarla se questa sequenza e' gia' stata inserita
				for (j=0;j<TotSequenze;j++) {
					if (Sequenza == VetSequenze[j]) break;
				}
				if (j == TotSequenze) {

					// ora trovare la camera che ha il numero minore
					IndexMin=-1;
					ValueMin = 1000;
					for (j=0;j<TotFlyBy;j++) {
						if (pVetFlyBy[j].Number < ValueMin) {
							IndexMin = pVetFlyBy[j].ObjIndex;
							ValueMin = pVetFlyBy[j].Number ;
						}
					}
					// ok, ora in IndexMin c'e' il codice oggetto
					pObj = &pVetBaseOggetti[IndexMin];
					// stringa 39:  "%s=%3d FlyByCamera(%d) Room:%d  Sector=(%d,%d)"
					sprintf(BufferLog,GetText(39),
						  pTex, Sequenza, IndexMin, pObj->IndexRoom, pObj->XOrgBlock , pObj->ZOrgBlock);
					j = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, Sequenza);	
					VetSequenze[TotSequenze++] = Sequenza;

				}
			}

			break;
		case LST_FOG_DISTANCES:
			pTex = GetText(15);  // Sectors
			// lista di valori (in blocchi) per distanza fog
			// sia positivi che negativi
			for (i=0;i<240;i++) {
				n= 120 -i;
				sprintf(BufferLog,"%+4d %s", n, pTex);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);	
			}
			break;
				
		case LST_TEX_SEQUENCE_128:
			// inserire lista di 128 comandi script TextureSequnce=
			for (i=0;i<128;i++) {


				sprintf(BufferLog,"TextureSequence=%3d",
					i+1);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i+1);
				
			}		
			break;
		case LST_BACKUP_GAMES:
			pTex = GetText(16); // Game Backup
			for (i=0;i<128;i++) {
				sprintf(BufferLog,"%s %3d",
					pTex, i+1);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i+1);
				
			}		
			break;
		case LST_TIMER_SIGNED_LONG:
			// lista da +511 a -512
			for (i=-512;i<512;i++) {
				sprintf(MiniBuf,"%+04d", i);
				if (i < 0) {
					pTex = GetText(17); // %s seconds       (Wait <#> seconds before activating)
					sprintf(BufferLog, pTex, MiniBuf);
				}

				if (i > 0) {
					pTex = GetText(18); //%s seconds       (Activate and wait <#> seconds before deactivating)
					sprintf(BufferLog, pTex, MiniBuf);
				}

				if (i == 0) {
					pTex = GetText(19); // (NULL. No timer operation) 0 timer
					sprintf(BufferLog, pTex,MiniBuf);
				}
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			break;


		case LST_TIMER_SIGNED:
			// valori timer
			sprintf(BufferLog,"Timer= +%02d",0);
			j = SendMessage(WindCombo,CB_ADDSTRING,0, 
					(LPARAM) BufferLog);
			SendMessage(WindCombo,CB_SETITEMDATA,j, 0);
			
			for (i=1;i<=63;i++) {

				sprintf(MiniBuf, "%02d", i);

				sprintf(BufferLog,"Timer= +%s", MiniBuf);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

			}
			for (i=64;i<128;i++) {
				Indice = i-63;
				sprintf(MiniBuf,"%02d", Indice);

				sprintf(BufferLog,"Timer= -%s", MiniBuf);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);

			}
			break;
			
		case LST_OTHER_LARA_SLOTS:
			for (i=1;i<=30;i++) {
				sprintf(BufferLog,"SLOT %3d  %s",
					i, pVetSlotOggetti[i].pNome);
				j=SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
			}
			break;


		case LST_SLOT_MOVEABLES:
			i =0;

			while (VetSlotMeshMoveables[i] !=-1) {
				j = VetSlotMeshMoveables[i];
				sprintf(BufferLog, "SLOT %3d  %s",
					j, pVetSlotOggetti[j].pNome);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				i++;
				
			}		
			break;

		case LST_STANDARD_MESH:

			i=0;
			
			while (VetStandardSwapMesh[i] != -1) {
				j = VetStandardSwapMesh[i];

				sprintf(BufferLog, "SLOT %3d  %s",
					j, pVetSlotOggetti[j].pNome);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				i++;
				
			}		
			break;
		case LST_SWAP_MESH_SLOT:
			i=0;
			
			while (VetMeshSwapSlot[i] > 0) {
				j = VetMeshSwapSlot[i];

				sprintf(BufferLog, "SLOT %3d  %s",
					j, pVetSlotOggetti[j].pNome);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				i++;
				
			}		
			break;
		case LST_CAMERA_EFFECTS:
			// list SOLO di camera e camera_fixed
			TestEffetti=true;
			TotOggetti=0;
			VetOggetti[TotOggetti++] = 0x4800;
			VetOggetti[TotOggetti++] = 0x4080;
			break;
		case LST_FLYBY:
			// lista solo flyby camera
			TestEffetti=true;
			TotOggetti=0;
			VetOggetti[TotOggetti++] = 0x4040;
			break;
		case LST_STATIC_LIST:
			// lista statics
			TestEffetti=false;
			TotOggetti=0;
			VetOggetti[TotOggetti++] = 0x10008; //nuovo tipo per static
			break;
		case LST_SINK:
			TestEffetti=true;
			TotOggetti=0;
			VetOggetti[TotOggetti++] = 0x4400;
			break;
		case LST_BUTTONS_LIST:
			// lista dei pulsanti
			for (i=1;i<0x40;i++) {
				pChar = CalcolaMascheraPulsanti(i);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) pChar);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				
			}				
			break;

		case LST_RECHARGE:
			pTex = GetText(20);  // Recharge of %3d units
			for (i=1;i<=200;i++) {
				sprintf(BufferLog, pTex, i);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				
			}
			pTex = GetText(21); // Recharge with %3d %% of full bar
			for (i=1;i<=10;i++) {
				sprintf(BufferLog, pTex, i*10);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i+200);
				
			}
			break;

		case LST_INVENTORY:
			// inserire gli slot di oggetti visuslizzabili in inventario

			for (i=0;GlobNgle.VetInventory[i] !=-1;i++) {
				Numero = GlobNgle.VetInventory[i];

				sprintf(BufferLog,"%-30s  \tSlot=%d",
					pVetSlotOggetti[Numero].pNome, Numero);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, Numero);
				Numero++;
					
				
			}


			break;
		case LST_MOVEABLES:
			// tutti i moveables nella mappa del livello
			VetOggetti[TotOggetti++]=8;
			TestMoveable=true;
			break;
		case LST_DEGREES:
			// lista di gradi da 45 fino a 360 con intervalli di 45
			i=45;
			pTex = GetText(22); // Moved of %3d degrees
			for (j=0;j<8;j++) {

				sprintf(BufferLog,pTex, i);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, j);
				i += 45;				
				
			}
			break;
		case LST_SEQUENCE_32:
			// lista di numeri da 0 a 31
			
			for (j=0;j<32;j++) {

				sprintf(BufferLog,"  %2d ", j);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, j);			
				
			}
			break;
		case LST_SEQUENCE_128:
			// lista di numeri da 0 a 128
			
			for (j=0;j<128;j++) {

				sprintf(BufferLog,"  %3d ", j);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, j);			
				
			}
			break;
		case LST_COLORS:
			// list di 28 colori
			pTex = GetText(23); // %2d: %s  (Red=%d  Green=%d  Blue=%d)
			for (i=0;i<TotColori;i++) {
				pByte = (BYTE*) &VetColori[i];
				sprintf(BufferLog, pTex, 
					i, VetNomiColori[i],  pByte[2], pByte[1], pByte[0]);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);			
				
			}
			// aggiungere anche codice speciale 100 per disattivare
			// del tutto la visione delle fogbulb
			Indice = SendMessage(WindCombo, CB_ADDSTRING,0, 
						(LPARAM)"SPECIAL. Disable all fog bulbs (to enable newly set a color)");
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 100);
			TestColors=true;

			break;
		case LST_INVENTORY_INDEX:
			// inserire gli slot di oggetti visuslizzabili in inventario

			for (i=0;GlobNgle.VetInventory[i] !=-1;i++) {
				Numero = GlobNgle.VetInventory[i];

				sprintf(BufferLog,"%-30s  \tSlot=%d",
					pVetSlotOggetti[Numero].pNome, Numero);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
				Numero++;
					
				
			}
			break;

		case LST_FRAG_2x2:
		case LST_FRAG_3x3:
		case LST_FRAG_4x4:
			// lista con impostazione per fragmented triggers
			switch (IndicePredefinito) {
			case LST_FRAG_2x2:
				MaxGrid = 2;
				break;
			case LST_FRAG_3x3:
				MaxGrid = 3;
				break;
			case LST_FRAG_4x4:
				MaxGrid = 4;
				break;

			}

	
			// ora fare tutte coordinate griglia
			pTex= GetText(24); // Square fragment
			for (y=0;y<MaxGrid;y++) {

				for (x=0;x<MaxGrid;x++) {

					sprintf(BufferLog, "%s (%d,%d)", pTex, x+1,y+1);
					// creare valore
					Valore= 0;

					
					Valore |= (x << 2);
					Valore |= y;

					Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore);
				}
			}

			// adesso fare strisce singole

			for (IndiceVert=0;IndiceVert<2;IndiceVert++) {
				for (x=0;x<MaxGrid;x++) {
					sprintf(BufferLog,GetText(0),
						x+1, VetVert[IndiceVert]);

					// creare valore
					Valore= TFF_NONE;

					// setting perche si lavora su striscia
					Valore |= TFF_STRIP;
					// impostare se verticale
					if (IndiceVert) Valore |= TFF_VERT_STRIP;
					// ora mettere numero di striscia
					Valore |= x;

					
					Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore);
				}
			}

			// e ora fare strisce incrociate
			// ora fare tutte coordinate griglia
			for (y=0;y<MaxGrid;y++) {
				for (x=0;x<MaxGrid;x++) {

					sprintf(BufferLog, GetText(1), x+1,y+1);
					// creare valore
					Valore= TFF_STRIP | TFF_CROSSED_STRIP;
					
					
					Valore |= (x << 2);
					Valore |= y;

					Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore);
				}
			}
			pTex = GetText(25);  // Diagonal %s passing at point (%d,%d)
			// adesso singole strisce diagonali
			for (IndiceInv=0;IndiceInv<2;IndiceInv++) {
				// ora non visualizzare tutti i riqudri ma solo quelli centrali
				for (y=0;y<MaxGrid;y++) {
					for (x=0;x<MaxGrid;x++) {
						sprintf(BufferLog, pTex,
							VetDiag[IndiceInv], x+1,y+1);
						// calcolare valore
						Valore = TFF_DIAGONAL;
						if (IndiceInv) Valore |= TFF_VERT_STRIP;
						Valore |= (x << 2);
						Valore |= y;
						Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
								(LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore);
					}
				}
			}

			// ora incroci diagonali
			pTex = GetText(26); // Two crossed diagonals passing at point (%d,%d)
			// ora non visualizzare tutti i riqudri ma solo quelli centrali
			for (y=0;y<MaxGrid;y++) {
				for (x=0;x<MaxGrid;x++) {
					sprintf(BufferLog, pTex, x+1,y+1);
					// calcolare valore
					Valore = TFF_DIAGONAL | TFF_CROSSED_STRIP;

					Valore |= (x << 2);
					Valore |= y;
					Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore);
				}
			}

			break;
		case LST_HALF_CLICKS:
		case LST_HALF_CLICKS32:
			if (IndicePredefinito == LST_HALF_CLICKS32) {
				Inizio = 1;
				Fine = 33;
			}else {
				Inizio = 0;
				Fine=513;
			}
			pTex = GetText(27); // Clicks = %5s (Half-Clicks=%d)
			// lista di half click, tutto normale ma i valori 
			// ogni valore e' mezzo half click
			for (i=Inizio;i<Fine;i++) {
				Valore=i;
				Result = (float) (Valore) / 2.0f;
				sprintf(MiniBuf, "%3.1f", Result);

				sprintf(BufferLog, pTex, MiniBuf, Valore);
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, Valore-Inizio);
			}
			break;
		case LST_TIME_128:
			pTex = GetText(28); // Forever (use other action/effect to disable it)
			// lista per tempo
			pChar= pTex;
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
					(LPARAM) pChar);
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 0);
			pTex=GetText(29); // seconds
			for (i=1;i<128;i++) {
				sprintf(BufferLog," %3d %s",i, pTex);

				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}
			break;
		case LST_ANIM_RANGES:
			pTex = GetText(30); // %2dth Animation Range
			// lista per indici animation range
			for (i=0;i<40;i++) {
				sprintf(BufferLog, pTex, i+1);
				Indice=SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}
			break;

		case LST_PSX_STRINGS:
			CaricaStringheScript(WindCombo,false,TEX_SEZ_PSXSTRINGS,255);
			break;
		case LST_PC_STRINGS:
			CaricaStringheScript(WindCombo, false, TEX_SEZ_PCSTRINGS,255);
			break;

		case LST_STRING_LIST_255:
			// lista per stringhe standard
			CaricaStringheScript(WindCombo,false,TEX_SEZ_STRINGS,255);
			break;
		case LST_NGSTRING_LIST_255:
			// lista per stringhe ng
			CaricaStringheScript(WindCombo,true,TEX_SEZ_EXTRANG,255);
			break;
		case LST_NGSTRING_LIST_512:
			// lista per stringhe ng a partire da 256
			CaricaStringheScript(WindCombo,true, TEX_SEZ_EXTRANG, 511);
			break;

		case LST_NGSTRING_ALL:
			// lista per stringhe ng
			CaricaStringheScript(WindCombo,true,TEX_SEZ_EXTRANG,10000);
			break;
		case LST_CD_TRACKS:
			// lista cd tracce
			for (i=0;i<256;i++) {
				sprintf(BufferLog,"AUDIO\\%03d", i);

				Indice=SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
				
			}
			break;
		case LST_FMV_LIST:
			// list fmv
			for (i=1;i<128;i++) {
				sprintf(BufferLog,"FMV=%3d , ... (\"FMVS\\fmv%d.ext\")", i,i);
				Indice=SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}
			break;
		case LST_SOUND_EFFECT_A:
			// lista di sound effects primo gruppo
			CaricaSoundSamples(WindCombo, 0);
			break;
		case LST_SOUND_EFFECT_B:
			// lista di sound effects secondo gruppo
		 	CaricaSoundSamples(WindCombo, 1);
			break;
		case LST_SFX_1024:
			// lista dei primi 1024 suoni
			CaricaSoundSamples(WindCombo, 2);
			break;

		case LST_TIME_LIST_32:
			// tempo per extra timer 
			pChar= GetText(28);  // forever (use other action/efffect to disable it)
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
					(LPARAM) pChar);
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 0);
			pTex = GetText(29); // seconds
			for (i=1;i<31;i++) {
				sprintf(BufferLog," %3d %s",i, pTex);

				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}

			// valore 31 per singola esecuzione
			pChar = GetText(31); //"Perform one single time";
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
					(LPARAM) pChar);
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 31);
			break;
		case LST_ANIMATION_32A:
		case LST_ANIMATION_32B:
		case LST_ANIMATION_32C:
			switch (IndicePredefinito) {
			case LST_ANIMATION_32A:
				Inizio=0;
				Fine=32;
				break;
			case LST_ANIMATION_32B:
				Inizio =32;
				Fine= 64;
				break;
			case LST_ANIMATION_32C:
				Inizio = 64;
				Fine=96;
				break;
			}
			for (i=Inizio;i<Fine;i++) {
				sprintf(BufferLog," %3d Animation",i);

				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i-Inizio);
			}
			break;
		case LST_ANIMATION_255:
		case LST_ANIMATION_LARA_B:
			if (IndicePredefinito == LST_ANIMATION_LARA_B) {
				Inizio = 256;
				Fine= 512;
			}else {
				Inizio =0;
				Fine=256;
			}

			for (i=Inizio;i<Fine;i++) {
				sprintf(BufferLog," %3d Animation",i);

				Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i- Inizio);
			}			
		
			break;
		case LST_STATE_ID:
			// in tutto sono 118 da 0 a 117
			// aggiungere pero' un valore che vale "Non cambiare"

			for (i=0;i<127;i++) {

				sprintf(BufferLog,"StateId = %03d",i);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				
			}
			pTex = GetText(32); // DON'T CHANGE
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0, 
					(LPARAM) pTex);
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 127);
			break;
		case LST_SLOT_ANIM_LARA:
			// (ignorare quelli vuoti e quelli statici)
			for (i=0;i<128;i++) {
				if (pVetSlotOggetti[i].Tipo == 0x10) {
					sprintf(BufferLog,"SLOT_%03d %s",
						i,pVetSlotOggetti[i].pNome);
					j = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				}
			}
			break;
		case LST_ADD_EFFECT:
			for (i=0;i<256;i++) {
				sprintf(BufferLog,"AddEffect=%3d , ...", i+1);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);	
			}
			break;
		case LST_CLICK_DISTANCE32:
			// prima aggiungere singoli click
			pTex = GetText(15); // Sectors
			for (i=0;i<32;i++) {
				if (((i+1) % 4) == 0) {
					// aggiungere anche numero settori
					sprintf(BufMini, "  (%d %s)", (i+1) / 4, pTex);
				}else {
					BufMini[0]=0;
				}
				sprintf(BufferLog,"%3d clicks  %s", i+1, BufMini);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);	
			}
			break;

		case LST_OCB_LARA_START:
			// visuaslizzare tutti i valori OCB di lara start pos
			// piazzati nel livello
			VetTrovati = TrovaOggetti(406, &TotTrovati);
			pTex=GetText(33); // OCB=%3d of LARA_START_POS(%d) in sector:(%d,%d) of %s
			for (i=0;i<TotTrovati;i++) {
				Indice = VetTrovati[i];

				// visualizzare dati di questo lara start pos
				IndiceRoom = pVetBaseOggetti[Indice].IndexRoom;
				sprintf(BufferLog, pTex,
					pVetBaseOggetti[Indice].OCBCode,
					Indice,
					pVetBaseOggetti[Indice].XOrgBlock,
					pVetBaseOggetti[Indice].ZOrgBlock,
					pVetRooms[IndiceRoom].Nome);

				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, 
						pVetBaseOggetti[Indice].OCBCode);
					
				
			}
			break;
		case LST_ROOM_LIST255:
			pVetRooms = *p2VetRooms;
			z=0;
			for (i=0;i<*pTotRooms;i++) {
				if (pVetRooms[i].TestEmptySlot == 0) {
					// stanza piena
					sprintf(BufferLog,"[%03d] %s", z,
						pVetRooms[i].Nome);
					j = SendMessage(WindCombo,CB_ADDSTRING,0, 
							(LPARAM) BufferLog);
					SendMessage(WindCombo,CB_SETITEMDATA,j, z);
					z++;
					if (z >= 255) break;
				}
			}
			break;


		case LST_KEYBOARD_MODE:
			// mettere flag per poter stabilire se condizione deve essere eseguita
			// anche dopo che era positiva e se status tasto deve essere
			// ON o off
			for (i=0;i<4;i++) {
				if (i & 1) 
					pChar = GetText(34); // "ACTIVE";
				else
					pChar = GetText(35); // "INACTIVE";

				if (i & 2) 
					pTipoCond = GetText(36);// "Multi shot for positive condition";
				else
					pTipoCond = GetText(37); // "Single shot for positive condition";

				sprintf(BufferLog,"%s (%s)",pChar, pTipoCond);
				j = SendMessage(WindCombo,CB_ADDSTRING,0, 
						(LPARAM) BufferLog);
				SendMessage(WindCombo,CB_SETITEMDATA,j, i);
				
			}
			break;

		}
	}

	if (TipoIndiceValore == TIV_OBJECT) {
		BaseTempTrigger.TestMoveable = TestMoveable;
		BaseTempTrigger.TestEffect = TestEffetti;
	}


	if (TotOggetti > 0) {

		// si tratta di oggetti da mostrare.
		// visualizzare solo gli oggetti in VetOggetti[]


		for (i=0;i<*pTotRooms;i++) {
			pRoom = &pVetRooms[i];
			if (pRoom->TestEmptySlot == 0) {
				// qui cercare moveable o effetti in modo sepaato
				if (TestEffetti==true) {
					TotIndici= pRoom->N_Effects;
					pVetIndici = pRoom->VetEffectCode;
				}else {
					// se non e' effetti deve essere moveable
					TotIndici=pRoom->N_ObjAndTriggers;
					pVetIndici = pRoom->VetObjAndTriggerCodes;
				}

				for (w=0;w<TotIndici;w++) {
					Indice = pVetIndici[w];
					// vedere se questo oggetto con indice item = indice
					// e' uno di quelli di tipo giust
					for (j=0;j<TotOggetti;j++) {
						if ((VetOggetti[j] & 0xffff) == 
							pVetBaseOggetti[Indice].TypeObject) {

								
							// stesso tipo di oggetto
							// se pero' il tipo e' 8, ignorarlo nel
							// caso sia uno static

							if ((VetOggetti[j] & 0xffff) != 8) break;

							z=pVetBaseOggetti[Indice].SlotIndex;
							// vedere se era richiesto moveable o static
							if (VetOggetti[j] == 0x10008) {
								// richiesto static
								if (pVetSlotOggetti[z].Tipo == 0x0110) break;
							}else {
								// richiesto moveable
								if (pVetSlotOggetti[z].Tipo == 0x10) break;
							}
							
						
						}
							
					}

					if (j < TotOggetti) {
						// trovato oggetto

						// trovare nome oggetto
						switch ((VetOggetti[j] & 0xffff)) {
						case 8:
							// oggetto slot
							pNomeOggetto = 
								pVetSlotOggetti[pVetBaseOggetti[Indice].
										SlotIndex].pNome;
							break;
						case 0x4800:
							// camera
							pNomeOggetto = "CAMERA";
							break;
						case 0x4080:
							pNomeOggetto= "CAMERA_FIXED";
							break;
						case 0x4040:
							pNomeOggetto = "CAMERA_FLYBY";
							break;
						case 0x4400:
							pNomeOggetto = "SINK";
							break;
						}
						pTex= GetText(38); // %-26s ID:%-6d in sector:(%d,%d) of %s
						sprintf(BufferLog,pTex,
							pNomeOggetto, Indice,
							pVetBaseOggetti[Indice].XOrgBlock,
							pVetBaseOggetti[Indice].ZOrgBlock,
							pRoom->Nome);

						j = SendMessage(WindCombo,CB_ADDSTRING,0, 
								(LPARAM) BufferLog);
						SendMessage(WindCombo,CB_SETITEMDATA,j, Indice);


						
					}
				}
			}
		}
	}

	// trovare quale elemento selezionare nel combo appena riempito
	BaseTempTrigger.TipoValoreTimer = TipoIndiceValore;

	switch (TipoIndiceValore) {
	case TIV_TIMER_ALL:
		IndiceAttiva = BaseTempTrigger.Timer;
		break;
	case TIV_TIMER_BASSO:
		IndiceAttiva = BaseTempTrigger.Timer & 0xff;
		break;
	case TIV_TIMER_ALTO:
		IndiceAttiva = BaseTempTrigger.Timer >> 8;
		break;
	case TIV_BUTTONS:
		// pulsanti dovrebbero essere 0x3e (mentre 0x01 sarebbe oneshot)
		IndiceAttiva = BaseTempTrigger.MaskPulsanti >> 1;
		IndiceAttiva &= 0x1f;

		break;

	case TIV_OBJECT:
		IndiceAttiva = BaseTempTrigger.Valore;
		break;
	}
	SelezionaComboValore(WindCombo, IndiceAttiva);



	// se trigger e' di tipo condition modificare triggerwhat
	// come object o parameter a seconda se e' o meno un moveable

	if (TipoIndiceValore == TIV_OBJECT && 
				BaseTempTrigger.TriggerActivation == 0x0c) {
		// vedere se in combo timer (per condizione) 
		// c'e' il simbolo "#" nella descrizione
		TestParametro=false;
		AltroCombo = GetDlgItem(hDialogo, IDC_COMBO_TIMER);
		i = SendMessage(AltroCombo,CB_GETCURSEL,0,0);
		SendMessage(AltroCombo,CB_GETLBTEXT,i,(LPARAM) BufferLog);
		if (Search(BufferLog,"#",ts_Avanti,NULL) != NULL) TestParametro=true;

		// e' il combo valore 
		// ora vedere se sono moveable o no
		if (TestMoveable==true) {
			// sono moveable: se era di tipo parameter
			// cambiare triggerwhat a object
			if (BaseTempTrigger.TriggerWhat == 0x0d) 
						BaseTempTrigger.TriggerWhat =0;
		}else {
			// non e' moveable.
			// se e' di tipo object o action mettere parameter
			if (BaseTempTrigger.TriggerWhat == 0 ||
				BaseTempTrigger.TriggerWhat == 0x0B || 
				TestParametro==true)
						BaseTempTrigger.TriggerWhat  = 0x0d;

			
		}
	}



}

// legge lo stato dei pulsanti e scrive il valore 
// corrispondente nel campo MaskPulsanti
void LeggePulsantiTrigger(HWND hDialogo)
{
	int i;
	BaseTempTrigger.MaskPulsanti = 0;
	for (i=0;i<6;i++) {
		if (IsDlgButtonChecked(hDialogo,IDC_PUL_ONESHOT+i) ==1) {
			BaseTempTrigger.MaskPulsanti |= (1 << i);
		}
	}
}

// modifica lo stato dei pulsanti sulla base del
// valore in MaskPulsanti
void ImpostaPulsantiTrigger(HWND hDialogo)
{
	int i;

	// impostare tutti i pulsanti
	for (i=0;i<6;i++) {
		if (BaseTempTrigger.MaskPulsanti & (1 << i)) 
			CheckDlgButton(hDialogo,IDC_PUL_ONESHOT+i,1);
		else
			CheckDlgButton(hDialogo,IDC_PUL_ONESHOT+i,0);

		
	}
}
void AggiornaTitoloTrigger(HWND hDialogo)
{
	char Titolo[100];

	strcpy(Titolo, "Set Trigger Type");

	if (BaseTempTrigger.TriggerActivation == 12) {
		sprintf(Titolo, "Set Trigger Type - CONDITION %d", 
			BaseTempTrigger.Timer & 0xff);
	}else {
		if (BaseTempTrigger.TriggerWhat == 9) {
			sprintf(Titolo, "Set Trigger Type - FLIPEFFECT %d", 
				BaseTempTrigger.Valore );
		}
		if (BaseTempTrigger.TriggerWhat == 11) {
			sprintf(Titolo, "Set Trigger Type - ACTION %d", 
				BaseTempTrigger.Timer & 0xff);
		}			
	}
	
	SetWindowText(hDialogo, Titolo);

}

// riceve in input la descrizione completa del trigger attuale
// se e' presente uno #START_DOC# attiva pulsante help
// se e' presente #remark# mette il testo in label remark
void AggiornaHelpRemark(HWND hDialogo, char *pRemark, char *pHelp)
{
	char *pChar;
	HWND TempWind;
	int SizeY;
	RECT MyRect;
	int OldSizeX;
	int OldSizeY;
	int GapY;

	pChar = pRemark;
	if (pChar) {
		TempWind= GetDlgItem(hDialogo, IDC_LABEL_REMARK);
		SetWindowText(TempWind,pRemark);
	}
	// scoprire dimensione di bordi e title bar
	GapY = GetSystemMetrics(SM_CYBORDER) * 2 + GetSystemMetrics(SM_CYSIZE);

	pChar = pHelp;
	if (pChar) {
		// rende visibile pulsante help
		TempWind = GetDlgItem(hDialogo, IDC_PUL_HELP);
		ShowWindow(TempWind, SW_SHOW);
		// imposta dimensione per finestra
		SizeY= 483+GapY;
	}else {
		// quando manca:
		SizeY = 233+GapY;
	}


	// scoprire dimensione attuale di finestra
	GetWindowRect(hDialogo, &MyRect);
	// se e' diversa la dimensione cambiarla
	OldSizeX = MyRect.right - MyRect.left;
	OldSizeY= MyRect.bottom - MyRect.top;
	if (OldSizeY != SizeY) {
		SetWindowPos(hDialogo, 0, 0,0, OldSizeX, SizeY, SWP_NOMOVE	| SWP_NOZORDER); 
	}
	// visualizzare subito testo di help in textbox a meno che non sia file esterno
	if (pHelp==NULL) return;
	if (InStr(0, pHelp, "@\"") != -1) return;
	// inserire testo normale in texteditor
	TempWind = GetDlgItem(hDialogo, IDC_TEXT_HELP);
	SetWindowText(TempWind, pHelp);

}
// visualizza tutti i dati contgenuti in basetemptrigger
// modifica anche il contenuto dei combo box, eccetto quelli
// per triggerwhat, plugins e triggerattivazione che sono sempre uguali
void ImpostaValoriTrigger(HWND hDialogo)
{
	HWND WindCombo;
	HWND TempWind;
	int *pN_UsedObjectCodes = (int *) 0x56C970;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	bool TestCombo;
	int i;
	WORD NumTimer;
	int IndiceSezione;
	int **Ptr2_ObjectCodeArray = (int**) 0x46B810;
	int *pObjectCodeArray;
	int TipoTLS;
	int Parametro;
	int IndiceExtra;
	int TipoTimer;
	int TipoTIV;
	short NumeroShort;
	StrNGConstants * pBaseCostanti;




	BaseTempTrigger.pMexHelp=NULL;
	BaseTempTrigger.pMexRemark=NULL;


	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;


	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pObjectCodeArray = *Ptr2_ObjectCodeArray;
	pVetRooms=*p2VetRooms;

	// inserire valori in text box
	// Text OBJECT (valore)
	TempWind = GetDlgItem(hDialogo,IDC_TEXT_VALORE);
	NumeroShort =(short) BaseTempTrigger.Valore;

	sprintf(BufferLog,"%d",NumeroShort);
	SetWindowText(TempWind,BufferLog);

	// Text TIMER
	TempWind = GetDlgItem(hDialogo,IDC_TEXT_TIMER);
	sprintf(BufferLog,"%d",BaseTempTrigger.Timer);
	SetWindowText(TempWind,BufferLog);


	// trigger type
	WindCombo=GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_TYPE);
	SelezionaComboValore(WindCombo, BaseTempTrigger.TriggerActivation);

	// plugin
	WindCombo=GetDlgItem(hDialogo,IDC_COMBO_PLUGIN);
	SelezionaComboValore(WindCombo, BaseTempTrigger.IdPlugin);

	// remark info
	TempWind = GetDlgItem(hDialogo,IDC_LABEL_REMARK);
	BufferLog[0]=0;
	SetWindowText(TempWind,BufferLog);

	// rende invisibile pulsante trigger's help
	TempWind = GetDlgItem(hDialogo,IDC_PUL_HELP);
	ShowWindow(TempWind, SW_HIDE);

	IndiceExtra = TrovaExtraField();

	TempWind=GetDlgItem(hDialogo, IDC_STATIC_EXTRA);
	ShowWindow(TempWind,SW_HIDE);

	ImpostaPulsantiTrigger(hDialogo);
	// rendere invisibili combo
	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_VALORE);
	ShowWindow(WindCombo,SW_HIDE);

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_TIMER);
	ShowWindow(WindCombo,SW_HIDE);

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_EXTRA);
	ShowWindow(WindCombo, SW_HIDE);
	// rendere invisibili anche i pulsanti realitivi a print list
	WindCombo = GetDlgItem(hDialogo, IDC_PUL_LIST_VALORI);
	ShowWindow(WindCombo,SW_HIDE);

	WindCombo = GetDlgItem(hDialogo, IDC_PUL_LIST_TIMER);
	ShowWindow(WindCombo,SW_HIDE);

	WindCombo = GetDlgItem(hDialogo, IDC_PUL_LIST_EXTRA);
	ShowWindow(WindCombo, SW_HIDE);



	WindCombo = GetDlgItem(hDialogo, IDC_STATIC_EXTRA);
	ShowWindow(WindCombo, SW_HIDE);
	TempWind=GetDlgItem(hDialogo, IDC_PUL_EXPORT_ANIMCOMMAND);


	// decidere se pulsante export deve essere visibile o meno
	if (BaseTempTrigger.TriggerWhat== 9 ||
		BaseTempTrigger.TriggerWhat == 11) {
		// se triggerwhat e' flipeffect o action mostrare pulsante

		ShowWindow(TempWind,SW_SHOW);
		TempWind=GetDlgItem(hDialogo, IDC_PUL_EXPORT_SCRIPT);
		ShowWindow(TempWind,SW_SHOW);

	}else {
		ShowWindow(TempWind,SW_HIDE);
		// se e' condition attivare export script
		TempWind = GetDlgItem(hDialogo, IDC_PUL_EXPORT_SCRIPT);
		if (BaseTempTrigger.TriggerActivation == 12) {
			// e' attiva condition
			ShowWindow(TempWind, SW_SHOW);
		}else {
			ShowWindow(TempWind, SW_HIDE);
		}

	}

	// se e' disattivata la visione di liste costanti uscire subito
	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_LIST)==1) {


		// e rendere visibili textbox
		TempWind = GetDlgItem(hDialogo,IDC_TEXT_VALORE);
		ShowWindow(TempWind,SW_SHOW);

		TempWind = GetDlgItem(hDialogo,IDC_TEXT_TIMER);
		ShowWindow(TempWind,SW_SHOW);
		AggiornaTitoloTrigger(hDialogo);
		return;
	}

	// ora vedere se si deve mostrare i list box o meno

	// --------  ANALISI PER OBJECT (VALORE) COMBO BOX ------------
	TestCombo =false;
	pBaseCostanti= TrovaBaseCostanti(BaseTempTrigger.IdPlugin);

	IndiceSezione=-1;
	TipoTLS=0;
	Parametro=-1;
	// combo object puo' essere influenzato anche da triggertype =condition

	if (BaseTempTrigger.TriggerActivation == 12) {

		// trigger condition
		// analizzare in tutte le sezioni se  c'e' una lista predefinita
		// per campo object per il tipo di condizione attuale (che e' in valore timer)
		NumTimer = BaseTempTrigger.Timer  & 0xff;


		for (i=0;i<pBaseCostanti->TotSezioni;i++) {
			if (pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_OBJECT && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_CONDITION &&
				pBaseCostanti->pVetSezioni[i].NumeroSezione == 
												NumTimer) {
				
				// trovata.
				TipoTLS = TLS_LISTINDEX;
				Parametro = i;
				TestCombo=true;
				break;
			}
		}
	
	}
	if (BaseTempTrigger.TriggerWhat == 11) {
		// trigger action, vedere se c'e' una listra specifica
		NumTimer = BaseTempTrigger.Timer  & 0xff;
		for (i=0;i<pBaseCostanti->TotSezioni;i++) {


			if (pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_OBJECT && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_ACTION &&
				pBaseCostanti->pVetSezioni[i].NumeroSezione == 
												NumTimer) {
				
				// trovata.
				TipoTLS = TLS_LISTINDEX;
				Parametro = i;
				TestCombo=true;
				break;
			}
		}
	
	}
	if (TestCombo == false) {

		switch (BaseTempTrigger.TriggerWhat) {
		case 0:
			// trigger OBJECT
		case 1:
			// trigger CAMERA
		case 2:
			// trigger SINK
		case 6:
			// target
		case 0x0c:
			// flyby
		case 0x0b:
			// trigger ACTION
			// visualizzare tutti i moveables del progetto in accordo col 
			// trigger d'attivazione
			TipoTLS = TLS_TRIGGERMOVEABLES;
			TestCombo=true;
			Parametro = BaseTempTrigger.TriggerWhat;
			break;
		}
				

	}

	if (TestCombo == false) {
		// non trovato, provare ad usare criterio generale in basesezioni
		for (i=0;i<pBaseCostanti->TotSezioni;i++) {
			if (pBaseCostanti->pVetSezioni[i].NumeroSezione == 
									BaseTempTrigger.TriggerWhat  && 
				pBaseCostanti->pVetSezioni[i].TipoListaShow ==SHOW_IN_OBJECT && 
				pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_TRIGGERWHAT) {

				Parametro= i;
				TipoTLS = TLS_LISTINDEX;
				TestCombo=true;
				break;
			}
		}
	}

	TempWind = GetDlgItem(hDialogo, IDC_TEXT_VALORE);
	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_VALORE);
	if (TestCombo == true) {
		RiempieComboListaTriggers(hDialogo, WindCombo,TipoTLS,
						Parametro, TIV_OBJECT);

		// mostra combo e nascondi text
		ShowWindow(WindCombo,SW_SHOW);
		ShowWindow(GetDlgItem(hDialogo, IDC_PUL_LIST_VALORI), SW_SHOW);
		ShowWindow(TempWind,SW_HIDE);
	}else {
		// nascondi combo  e mostra text
		ShowWindow(WindCombo,SW_HIDE);
		ShowWindow(GetDlgItem(hDialogo, IDC_PUL_LIST_VALORI), SW_HIDE);
		ShowWindow(TempWind,SW_SHOW);
	}



	// --------- FINE ANALISI COMBO OBJECT (VALORE) ----------------

	// --------- ANALISI PER COMBO TIMER ------------------

	// analizzare tutte le sezioni
	TestCombo=false;
	TipoTLS = 0;
	Parametro = -1;

	// BaseTempTrigger.Timer 
	
	for (i=0;i<pBaseCostanti->TotSezioni;i++) {
		if (pBaseCostanti->pVetSezioni[i].TipoListaShow == SHOW_IN_TIMER) {
			// trovata una sezione timer.
			// vedere se e' triggerwhat
			if (pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_TRIGGERWHAT && 
				pBaseCostanti->pVetSezioni[i].NumeroSezione == 
					BaseTempTrigger.TriggerWhat) {
				// trovata sezione 
				Parametro=i;
				TipoTLS = TLS_LISTINDEX;
				TestCombo=true;
				break;
			}

			// vedere se e' triggertype
			if (pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_TRIGGERTYPE && 
				pBaseCostanti->pVetSezioni[i].NumeroSezione == 
					BaseTempTrigger.TriggerActivation) {
				// trova sezione
				Parametro=i;
				TipoTLS = TLS_LISTINDEX;
				TestCombo=true;
				break;
			}
			
			// vedere se e' un effect e siamo in modalita' effect

			if (pBaseCostanti->pVetSezioni[i].TipoSezione == SEZ_EFFECT && 
				BaseTempTrigger.TriggerWhat == 9 && 
				pBaseCostanti->pVetSezioni[i].NumeroSezione == 
						BaseTempTrigger.Valore ) {
				// trovata sezione
				Parametro=i;
				TipoTLS = TLS_LISTINDEX;
				TestCombo=true;
				break;
			}

			
		}
	}

	WindCombo = GetDlgItem(hDialogo, IDC_COMBO_TIMER);	
	TempWind = GetDlgItem(hDialogo, IDC_TEXT_TIMER);

	if (TestCombo == true) {
		// ok, c'e' da attivare combo box per TIMER
		if (BaseTempTrigger.TestExtraTimer==true) 
			TipoTimer = TIV_TIMER_BASSO;
		else
			TipoTimer = TIV_TIMER_ALL;

		RiempieComboListaTriggers(hDialogo, WindCombo,TipoTLS, 
										Parametro, TipoTimer );


		// mostra combo e nascondi text
		ShowWindow(WindCombo,SW_SHOW);
		ShowWindow(GetDlgItem(hDialogo, IDC_PUL_LIST_TIMER), SW_SHOW);
		ShowWindow(TempWind,SW_HIDE);

	}else {
		// nascondi combo  e mostra text
		ShowWindow(WindCombo,SW_HIDE);
		ShowWindow(GetDlgItem(hDialogo, IDC_PUL_LIST_TIMER), SW_HIDE);
		ShowWindow(TempWind,SW_SHOW);
	}


	

	// vedere se bisogna visualizzare anche extra timer
	if (BaseTempTrigger.TestExtraTimer == true ||
		BaseTempTrigger.TestExtraButtons == true) {

		WindCombo = GetDlgItem(hDialogo, IDC_COMBO_EXTRA);
		ShowWindow(WindCombo,SW_SHOW);
		ShowWindow(GetDlgItem(hDialogo, IDC_PUL_LIST_EXTRA), SW_SHOW);
		TempWind=GetDlgItem(hDialogo, IDC_STATIC_EXTRA);
		ShowWindow(TempWind,SW_SHOW);
		TipoTLS = TLS_LISTINDEX;
		// vedere se nel combo extra si deve prelevare valore
		// da timer o da pulsanti
		if (BaseTempTrigger.TestExtraTimer == true)
			TipoTIV = TIV_TIMER_ALTO;
		else
			TipoTIV = TIV_BUTTONS;

		RiempieComboListaTriggers(hDialogo,WindCombo,TipoTLS, 
					IndiceExtra,TipoTIV);
	}
	// triggerwhat
	WindCombo=GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_WHAT);
	SelezionaComboValore(WindCombo, BaseTempTrigger.TriggerWhat);

	// gestione remark e help
	BaseTempTrigger.pMexHelp = NULL;
	BaseTempTrigger.pMexRemark = NULL;

	i=-1;
	// FLIPEFFECT?
	if (BaseTempTrigger.TriggerWhat== 9) {
		i = BaseTempTrigger.Valore;

	}
	// ACTION o CONDITION
	if (BaseTempTrigger.TriggerWhat == 11 || 
		BaseTempTrigger.TriggerActivation == 12) {

		i = BaseTempTrigger.Timer & 0xff;
	}

	
	if (i >= 0 && i < TotDescrizioniRemark) {
		BaseTempTrigger.pMexHelp  = VetDescrizioniHelp[i];
		BaseTempTrigger.pMexRemark = VetDescrizioniRemark[i];
	}

	// aggiornare info remark

	AggiornaHelpRemark( hDialogo, BaseTempTrigger.pMexRemark, BaseTempTrigger.pMexHelp );
	

	AggiornaTitoloTrigger(hDialogo);
}
// imposta in basetemptrigger i dati iniziali di trigger.
// se attualmente e' selezionato un trigger nel room usa quello
// altrimenti usa i dati in pDatiLastTrigger

void ImpostaTempTrigger(void)
{
	StrTriggerSelezionato *pDatiLastTrigger = (StrTriggerSelezionato*) 0x583BAC;
	int *pIndiceItemSelezionato = (int *) 0x583BA4;
	int Indice;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	int *pN_UsedObjectCodes = (int *) 0x56C970;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;


	pVetTriggers = *P2_VetTriggers;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	// inizializzare valori di trigger in BaseTemptrigger


	// prima usare come default quelli di ultimo trigger selezionato
	BaseTempTrigger.MaskPulsanti = pDatiLastTrigger->MaskPulsanti;
	BaseTempTrigger.Timer = pDatiLastTrigger->Timer;
	BaseTempTrigger.TriggerActivation = pDatiLastTrigger->TriggerActivation;
	BaseTempTrigger.TriggerWhat = pDatiLastTrigger->TriggerWhat;
	BaseTempTrigger.Valore = pDatiLastTrigger->Valore;
	BaseTempTrigger.IdPlugin = pDatiLastTrigger->PluginId;
	BaseTempTrigger.TestIndiceTrigger =false;
	BaseTempTrigger.IndiceTrigger = 0;

	

	// ora vedere se c'e' un trigger selezonato
	Indice = *pIndiceItemSelezionato;
	if (Indice != -1) {
		if (pVetBaseOggetti[Indice].TypeObject == 0x0010) {
				// e' un trigger: impostarlo
			Indice = pVetBaseOggetti[Indice].SlotIndex;
			BaseTempTrigger.MaskPulsanti = 
					pVetTriggers[Indice].TriggerButtons;
			BaseTempTrigger.Timer = pVetTriggers[Indice].Timer ;
			BaseTempTrigger.TriggerActivation = 
					pVetTriggers[Indice].TriggerAttivazione;
			BaseTempTrigger.TriggerWhat = 
					pVetTriggers[Indice].TriggerCosaAttivare;
			BaseTempTrigger.Valore = 
					pVetTriggers[Indice].OggettoDaTriggerare;
			BaseTempTrigger.IdPlugin =
					pVetTriggers[Indice].PluginId;

			BaseTempTrigger.TestIndiceTrigger =true;
			BaseTempTrigger.IndiceTrigger = Indice;

		}
	}


	// se il tipo e' object ma non c'e' nessun indiceitemselezionato
	// allora cercare di dedurre il tipo di oggetto e quindi cambiare
	// anche il tipo trigger what
	if (BaseTempTrigger.TriggerWhat == 0) {
		Indice = BaseTempTrigger.Valore;
		if (Indice < 4096) {

			switch (pVetBaseOggetti[Indice].TypeObject) {
			case 0x4800:  // type_camera
			case 0x4080:  // type_camera
				// camera
				BaseTempTrigger.TriggerWhat = 1;
				break;

			case 0x4400:    //sink
				BaseTempTrigger.TriggerWhat =2;
				break;

			}
		}


	}
	
	// i pulsanti numerici sono invertiti e vengono poi
	// reinvertiti all'uscita
	BaseTempTrigger.MaskPulsanti ^= 0x3e;
			
}
// inserisce valore in pValore
// se c'e' errore (valore non numerico) restituise false
bool PrendiNumeroDaTextBox(HWND TextBox, int *pValore)
{
	GetWindowText(TextBox,BufferLog,255);

	*pValore = -48000;
	sscanf(BufferLog,"%d", pValore);
	if (*pValore == -48000) 
		return false;
	else
		return true;

}
// trasorma stringa numerica in numero. se non e' numero resituisce false
bool PrendiNumero(char *pTesto, int *pValore)
{
	*pValore = -48000;
	sscanf(pTesto,"%d", pValore);
	if (*pValore == -48000) 
		return false;
	else
		return true;
}

// prende i dati di tutti i controlli di finestra trigger e li inserisce
// in corrispondente trigger attuale
// nota: se c'e' qualche dato mancante o sbagliato restituisce false
bool PrendiValoriTrigger(HWND hDialogo)
{
	// alcuni valori potrebbero essere gia' impostati a patto che il 
	// corrispettivo combo sia gia' attivo
	// questo vale sempre per i combo sempre attivi
	HWND TempWind;
	int Valore;
	int *pTestEsisteSelezioneBox = (int*) 0x583BBC;
	int i;
	bool TestSelOk;
	StrTriggerSelezionato *pDatiLastTrigger = (StrTriggerSelezionato*) 0x583BAC;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerSelezionato MioTrigger;
	StrTriggerRoom *pVetTriggers;
	int *pIndiceItemSelezionato = (int*) 0x583BA4;
	int IndiceTrig;

	pVetTriggers = *P2_VetTriggers;
	// ----------------- CAMPO VALORE OBJECT ----------------------
	TempWind = GetDlgItem(hDialogo, IDC_COMBO_VALORE);
	if (IsWindowVisible(TempWind) == FALSE) {
		// c'e' il text box, bisogna prendere il valore da questo campo
		TempWind = GetDlgItem(hDialogo,IDC_TEXT_VALORE);
		if (PrendiNumeroDaTextBox(TempWind,&Valore) == false) {
			MessageBox(hDialogo, "ERROR: not valid numeric value in [Object to trigger] field", "Error",0);
			return false;
		}

		BaseTempTrigger.Valore = Valore;

	}

	// ---------------- CAMPO TIMER -----------------------
	TempWind = GetDlgItem(hDialogo,IDC_COMBO_TIMER);
	if (IsWindowVisible(TempWind)==FALSE) {
		// c'e' il text box per trigger
		TempWind = GetDlgItem(hDialogo,IDC_TEXT_TIMER);
		if (PrendiNumeroDaTextBox(TempWind,&Valore) == false) {
			MessageBox(hDialogo, "ERROR: not valid numeric value in [Timer] field", "Error",0);
			return false;
		}
		BaseTempTrigger.Timer = Valore;

	}

	// --------------  PULSANTI ------------------------

	BaseTempTrigger.MaskPulsanti ^= 0x3e;

	// controllare che non sia CONDITION + (ACTIVATION or FLIPEFFECT)
	if (BaseTempTrigger.TriggerActivation == 0x0C) {
		if (BaseTempTrigger.TriggerWhat == 9 ||
			BaseTempTrigger.TriggerWhat == 0x0b) {

			MessageBox(hDialogo, pErrorMixedTriggers ,"Mixed special new triggers",0);
			return false;
		}
	}
	// prendere selezione attuale
	if (*pTestEsisteSelezioneBox)  {
		TestSelOk=PreparaMiaSelezione(1,false);
	}else {
		TestSelOk=false;

	}
	if (BaseTempTrigger.TestIndiceTrigger==true) 
		IndiceTrig= *pIndiceItemSelezionato;
	else
		IndiceTrig=-1;


	// non va been BaseTempTrigger ci sono campi in in piu'
	// ora fare controllone per trigger speciali sovrapposti
	MioTrigger.MaskPulsanti = BaseTempTrigger.MaskPulsanti;
	MioTrigger.Timer = BaseTempTrigger.Timer;
	MioTrigger.TriggerActivation = BaseTempTrigger.TriggerActivation;
	MioTrigger.TriggerWhat = BaseTempTrigger.TriggerWhat;
	MioTrigger.Valore = BaseTempTrigger.Valore;
	MioTrigger.PluginId= BaseTempTrigger.IdPlugin;

	if (TestSelOk) {

		if (ControllaConflittoTrigger(&MioTrigger, 
					DatiShape.MiaSelezione.left,DatiShape.MiaSelezione.top,
					DatiShape.MiaSelezione.right, DatiShape.MiaSelezione.bottom, 
					IndiceTrig ) == 1) {
			return false;
		}
	}

	// adesso salvarlo in ultimo trigger
	pDatiLastTrigger->MaskPulsanti = BaseTempTrigger.MaskPulsanti;
	pDatiLastTrigger->Timer = BaseTempTrigger.Timer;
	pDatiLastTrigger->TriggerActivation = BaseTempTrigger.TriggerActivation;
	pDatiLastTrigger->TriggerWhat = BaseTempTrigger.TriggerWhat;
	pDatiLastTrigger->Valore = BaseTempTrigger.Valore;
	pDatiLastTrigger->PluginId = BaseTempTrigger.IdPlugin;

	if (BaseTempTrigger.IdPlugin != 0) {
		// aggiornare l'utilizzo di trigger per questo plugin
		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			if (GlobNgle.VetPluginNgle[i].PluginId == BaseTempTrigger.IdPlugin) {
				GlobNgle.VetPluginNgle[i].TotUsato ++;
				break;
			}
		}

	}

	// se c'era anche indice trigger copiarlo pure li
	if (BaseTempTrigger.TestIndiceTrigger == true) {
		i = BaseTempTrigger.IndiceTrigger;
		pVetTriggers[i].OggettoDaTriggerare = BaseTempTrigger.Valore;
		pVetTriggers[i].Timer = BaseTempTrigger.Timer;
		pVetTriggers[i].TriggerButtons = BaseTempTrigger.MaskPulsanti;
		pVetTriggers[i].TriggerCosaAttivare = BaseTempTrigger.TriggerWhat;
		pVetTriggers[i].TriggerAttivazione = BaseTempTrigger.TriggerActivation;
		pVetTriggers[i].PluginId = BaseTempTrigger.IdPlugin;
	}

	return true;

}
// restituisce testo descrittivi di trigger in modo sintentico
// ossia nella stessa forma usata da ngle
// si basa su finestra trigger aperta e su dati di BaseTempTrigger
char *GetLineaTrigger(HWND hDialogo)
{

	char MexTriggerPlugin[120];
	char MexTriggerWhat[120];
	char MexTrigger[120];

	WORD ValPulsanti;
	static char LineaTrigger[128];
	HWND TempWind;

	TempWind = GetDlgItem(hDialogo, IDC_COMBO_TRIGGER_TYPE);
	GetWindowText(TempWind, MexTrigger,119);

	TempWind = GetDlgItem(hDialogo, IDC_COMBO_TRIGGER_WHAT);
	GetWindowText(TempWind, MexTriggerWhat,119);

	TempWind = GetDlgItem(hDialogo, IDC_COMBO_PLUGIN);
	GetWindowText(TempWind, MexTriggerPlugin,119);

	ValPulsanti = BaseTempTrigger.MaskPulsanti;
	ValPulsanti ^= 0x3e;

	sprintf(LineaTrigger,"%s(%d:%d) for %s(%d) {%s}",
		MexTrigger, BaseTempTrigger.Timer, ValPulsanti,
		MexTriggerWhat, BaseTempTrigger.Valore, MexTriggerPlugin);
	return LineaTrigger;


}
// crea una descrizione testuale del trigger attuale
// usa i valori dei combo box e text box 
// per localizzare quello che serve
char *GetDescrizioneTrigger(HWND hDialogo, bool TestScript)
{

	static char Buffer[512];
	HWND TempWind;
	char MexObject[120];
	char MexTimer[120];
	char MexExtra[120];
	char *pTipoTrigger;
	char *pCommento;
	char *pLineaTrigger;
	char NomeFinestra[256];

	if (TestScript==true) 
		pCommento="; ";
	else
		pCommento="";

	if (BaseTempTrigger.TriggerActivation == 12) {
		// e' una condizione
		pTipoTrigger = "CONDITION";
	}else {
		// vedere se e' flipeffect o action
		if (BaseTempTrigger.TriggerWhat == 9) {
			pTipoTrigger = "FLIPEFFECT";
		}else {
			pTipoTrigger = "ACTION";
		}
	}
	
	if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_LIST)==1) {
		//solo valori testuali
		TempWind = GetDlgItem(hDialogo, IDC_TEXT_VALORE);
		GetWindowText(TempWind, MexObject, 119);

		TempWind = GetDlgItem(hDialogo, IDC_TEXT_TIMER);
		GetWindowText(TempWind, MexTimer, 119);

		MexExtra[0]=0;
	}else{
		// prendre testo dei combo
		TempWind = GetDlgItem(hDialogo, IDC_COMBO_VALORE);
		GetWindowText(TempWind, MexObject,119);

		
		TempWind = GetDlgItem(hDialogo, IDC_COMBO_TIMER);
		GetWindowText(TempWind, MexTimer,119);

		MexExtra[0]=0;

		TempWind = GetDlgItem(hDialogo, IDC_COMBO_EXTRA);
		if (IsWindowVisible(TempWind)== TRUE) {
			GetWindowText(TempWind, MexExtra,119);
		}
	}
	pLineaTrigger = GetLineaTrigger(hDialogo);
	GetWindowText(hDialogo, NomeFinestra, 255);

	sprintf(Buffer, "%s%s\r\n%sExporting: %s\r\n%s<#> : %s\r\n%s<&> : %s\r\n%s(E) : %s\r\n",
		pCommento, NomeFinestra, 
		pCommento, pLineaTrigger, pCommento, MexObject, 
		pCommento, MexTimer, pCommento, MexExtra);
	return Buffer;

}

WORD AnalisiTriggerFlyBy(WORD ItemIndex)
{
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	// nuova zona rilocata di vetcamera
	StrFlyByRoom **p2ZonaVetCamera = (StrFlyByRoom **) 0x454652;
	StrFlyByRoom *pZonaVetCamera;
	WORD *pTotCamera = (WORD *) 0x5651A0;
	WORD i;
	WORD TipoItem;


	pVetBaseOggetti = *P2_VetBaseOggetti;
	pZonaVetCamera = *p2ZonaVetCamera;

	TipoItem = pVetBaseOggetti[ItemIndex].TypeObject;	

	if (TipoItem == 0x4040) {
		// e' flybycamera
		// convertire indice
		for (i=0;i<*pTotCamera;i++) {
			if (pZonaVetCamera[i].IndiceNgle == ItemIndex) {
				ItemIndex = pZonaVetCamera[i].IndiceFlyBy;
				break;
			}
		}

	}
	return ItemIndex;

}
// legge da file script.txt di cartella trle attuale
// i comandi Plugin=Id,Name,....
// dalla sezione [Options] e li carica in  GlobNgle.PluginScriptTable 
// se trova un errore di sitnassi o altri problemi restituisce false ma prima
// ha gia' visualizzato l'errore

bool LeggePluginScript(char *pNomeScript)
{
	FILE *pFile;
	char BufLinea[4096];
	StrPluginTableScriptNgle *pTable;
	bool TestInizioOptions;
	int Indice;
	char StrNumero[10];
	int NChar;
	bool TestErrore;
	char MioCar;
	int PluginId;
	char NomePlugin[81];
	int i;

	pFile= fopen(pNomeScript, "rt");
	if (pFile==NULL) {
		sprintf(BufferLog, "ERROR: cann't locate script.txt file: %s", pNomeScript);
		MioMessageBox(BufferLog,"Error");
		return false;
	}
	pTable = &GlobNgle.PluginScriptTable;
	
	pTable->TotPlugins=0;
	TestInizioOptions=false;

	while (fgets(BufLinea, 4095, pFile)) {
		// togliere commento
		Indice=InStr(0, BufLinea, ";");
		if (Indice != -1) {
			BufLinea[Indice]=0;
		}

		if (TestInizioOptions==false) {
			// cercare solo inizio di sezione [options]
			if (InStr(0,BufLinea, "[Options]") != -1) {
				TestInizioOptions=true;
			}
		}else {

			// siamo in sezione [options]
			// se pero' troviamo una sezione [title] o [level] terminare
			if (InStr(0, BufLinea, "[Title]") != -1 ||
				InStr(0, BufLinea, "[Level]") != -1) break;

			// vedere se c'e' comando plugin=
			Indice=InStr(0, BufLinea, "Plugin=");

			if (Indice !=-1) {
				// il primo valore e' l'id
				NChar=0;
				PluginId=-1;
				Indice += strlen("Plugin=");


				for (i=Indice;i< (int) strlen(BufLinea);i++) {
					MioCar= BufLinea[i];
					if (MioCar==',') {
	
						StrNumero[NChar]=0;
						PluginId=GetArgNumerico(StrNumero, &TestErrore);


						if (TestErrore) PluginId=-1;

						break;
					}


					if (MioCar != ' ' && MioCar != 9) {
						StrNumero[NChar++] = MioCar;
					}
	
				}
				if (PluginId == -1) {
					sprintf(BufferLog, "ERROR reading script.txt file:\r\nCann't find valid PluginId in command line:\r\n%s",
						BufLinea);
					MioMessageBox(BufferLog, "Error");
				}else {
					// ora cercare il nome
					i++;
					NChar=0;

					while (i < (int) strlen(BufLinea) && BufLinea[i] != ',') {
						MioCar=BufLinea[i++];
						if (MioCar != ' ' && MioCar != 9) {
							NomePlugin[NChar++] = MioCar;
						}
					}

					NomePlugin[NChar]=0;

					if (InStr(0, NomePlugin, "Plugin") == -1) {
						sprintf(BufferLog, "ERROR reading script.txt file:\r\nCann' find plugin name in command line:\r\n%s",
							BufLinea);
						MioMessageBox(BufferLog, "Error");
					}else {

						// salvare i dati
						strcpy(pTable->VetPlugins[pTable->TotPlugins].Name , NomePlugin);
						pTable->VetPlugins[pTable->TotPlugins].PluginId = PluginId;
						pTable->TotPlugins++;

					}
				}
										
			}

		}


	}
	fclose(pFile);
	return true;

}
void ShowExportFunction(HWND hDialogo)
{

	char *pNomePlugin;
	int i;
	int PluginIdExport;
	char Buffer[90];
	int Arg1, Arg2;
	FILE *pFile;
	char NomeFile[256];
	char Testo[256];
	char BufIndex[30];
	bool TestOk;

	pNomePlugin="NULL";
	PluginIdExport=BaseTempTrigger.IdPlugin;
	TestOk=false;

	// se usa engine diversa aggiungere in word alta numero di plugin
	if (PluginIdExport > 0) {

		// leggere dati da script
		LeggePluginScript(GetFileTrle("script\\script.txt"));


		// trovare il nome dle plugin con quell'id in ngle
		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			if (GlobNgle.VetPluginNgle[i].PluginId == BaseTempTrigger.IdPlugin) {
				pNomePlugin= GlobNgle.VetPluginNgle[i].Nome;
				sprintf(Buffer, "\"%s\"", pNomePlugin);
				pNomePlugin = Buffer;
				TestOk=true;
				break;
			}
		}
	}

	if (BaseTempTrigger.TriggerWhat == 9) {
		// trigger FLIPEFFECT
		Arg1 = BaseTempTrigger.Timer;
		Arg2 = 0;
		if (BaseTempTrigger.TestExtraTimer) {
			Arg2= (BaseTempTrigger.Timer >> 8) & 0x7f;
			Arg1= BaseTempTrigger.Timer & 0xff;
		}

		sprintf(Testo, "PerformFlipeffect(%s, %d, %d, %d);",
			pNomePlugin, BaseTempTrigger.Valore, Arg1, Arg2);
		sprintf(NomeFile, "Export_Function_Flip_%d.txt", BaseTempTrigger.Valore);
		TestOk=true;			
	}

	if (BaseTempTrigger.TriggerWhat == 11) {
		// action trigger
		if (BaseTempTrigger.TestMoveable==true || BaseTempTrigger.TestEffect == true) {
			sprintf(BufIndex, "%d | NGLE_INDEX", BaseTempTrigger.Valore);
		}else {
			sprintf(BufIndex, "%d", BaseTempTrigger.Valore);
		}
		Arg1 = BaseTempTrigger.Timer & 0xff;

		Arg2 =(BaseTempTrigger.Timer >> 8) & 0x7f;

		sprintf(Testo, "PerformActionTrigger(%s, %d, %s, %d);", 
			pNomePlugin, Arg1,  BufIndex, Arg2);

		sprintf(NomeFile, "Export_Function_Action_%d.txt", Arg1);
		TestOk=true;
	}
	if (BaseTempTrigger.TriggerActivation == 12) {
		// e' attiva condition
		Arg1= BaseTempTrigger.Timer & 0xff;
		Arg2=0;
		if (BaseTempTrigger.TestExtraButtons) {
			Arg2 = BaseTempTrigger.MaskPulsanti >> 1;
			Arg2 &= 0x1f;
	
		}
		
		if (BaseTempTrigger.TestMoveable) {
			sprintf(BufIndex, "%d | NGLE_INDEX", BaseTempTrigger.Valore);
		}else {
			sprintf(BufIndex, "%d", BaseTempTrigger.Valore);
		}

		sprintf(Testo, "PerformConditionTrigger(%s, %d, %s, %d);",
			pNomePlugin, Arg1, BufIndex, Arg2);

		sprintf(NomeFile, "Export_Function_Cond_%d.txt", Arg1);
		TestOk=true;

	}
	if (TestOk==false) return;
	
	pFile = fopen(GetFileTemp(NomeFile),"wb");
	fprintf(pFile, "\r\n%s",GetDescrizioneTrigger(hDialogo, true));
	fprintf(pFile, "\r\n%s\r\n", Testo);
	fclose(pFile);

	ShellExecute(NULL,"open",GetFileTemp(NomeFile),"","",SW_SHOW);

}

// visualizza testo con i dati da usare per trigger nello script
// si basa su BaseTempTrigger
void ShowExportScriptTrigger(HWND hDialogo)
{
	WORD  Valore2, Valore3;
	DWORD Valore1;
	FILE *pFile;
	char NomeFile[256];
	WORD ExtraButtons;
	bool TestCollision;
	char *pNomePlugin;
	int i;
	int PluginIdExport;

	Valore1=0;
	Valore2=0;
	Valore3=0;
	ExtraButtons=0;

	pNomePlugin="UNKNOWN_PLUGIN";
	PluginIdExport=BaseTempTrigger.IdPlugin;

	// se usa engine diversa aggiungere in word alta numero di plugin
	if (PluginIdExport > 0) {

		// leggere dati da script
		LeggePluginScript(GetFileTrle("script\\script.txt"));


		// trovare il nome dle plugin con quell'id in ngle
		for (i=0;i<GlobNgle.TotPluginNgle;i++) {
			if (GlobNgle.VetPluginNgle[i].PluginId == BaseTempTrigger.IdPlugin) {
				pNomePlugin= GlobNgle.VetPluginNgle[i].Nome;
				break;
			}
		}
		if (i==GlobNgle.TotPluginNgle) {
			sprintf(BufferLog, "INTERNAL ERROR: cann't locate the plugin name linked with Ngle plugin ID=%d",
				BaseTempTrigger.IdPlugin);
			MioMessageBox(BufferLog, "Error Plugin ID");
		}else {

			// ora vedere che id ha questo pNomePlugin nello script
			for (i=0;i<GlobNgle.PluginScriptTable.TotPlugins;i++) {
				if (strcmpi(GlobNgle.PluginScriptTable.VetPlugins[i].Name, pNomePlugin)==0) {
					PluginIdExport = GlobNgle.PluginScriptTable.VetPlugins[i].PluginId;
					break;
				}

			}

			if (i == GlobNgle.PluginScriptTable.TotPlugins) {
				sprintf(BufferLog, "WARNING: the plugin that owns this trigger (the plugin: \"%s\" with ngle Id=%d) is missing in Plugin= script commands of current script.txt file.\r\nYou have to add a plugin= command for this plugin and it's advsable you use the #define @plugin directive that it will be showed in trigger export report",
					pNomePlugin, PluginIdExport);
				MioMessageBox(BufferLog, "Warning");
			}
		}
		Valore1 |= (PluginIdExport << 16);
		
	}
	if (BaseTempTrigger.TriggerActivation == 12) {
		// tipo CONDIZIONE
		Valore1 |= TGROUP_CONDITION_TRIGGER;
		// se si stanno usando extra buttons, copiare il valore
		if (BaseTempTrigger.TestExtraButtons) {
			ExtraButtons = BaseTempTrigger.MaskPulsanti >> 1;
			ExtraButtons &= 0x1f;
			// ora metterlo in byte alto
			ExtraButtons = ExtraButtons << 8;

		}


			
		
	}else {
		if (BaseTempTrigger.TriggerWhat == 9) {
			// flipeffect
			Valore1 |= TGROUP_FLIPEFFECT;
		}else {
			// azione
			Valore1 |= TGROUP_ACTION;
		}
	}
	// aggiungere a prima word il flag (eventuale) se valore e'
	// l'indice di un moveable (che andra' poi convertito)
	// valore oggetto
	if (BaseTempTrigger.TriggerWhat == 0 ||
		(BaseTempTrigger.TriggerWhat == 11 && 
		IsMoveable(BaseTempTrigger.Valore, true) )) {
		// se trigger what e' azione o oggetto allora 
		// il valore deve essere l'indice di un moveable
		Valore1 |= TGROUP_MOVEABLE;
	}

	Valore2 = BaseTempTrigger.Valore;


	Valore3 = BaseTempTrigger.Timer;
	
	Valore3  |= ExtraButtons;

	TestCollision=false;

	if (BaseTempTrigger.TriggerWhat == 9 && 
		BaseTempTrigger.Valore >= 310 && BaseTempTrigger.Valore <= 329) {

		// non consentire exporting
		TestCollision=true;
	}

	if (PluginIdExport >0) {
		
		sprintf(NomeFile,"Exported_ScriptTrigger_$%06X_%d_$%X.txt",
				Valore1,Valore2,Valore3);
	}else {

		sprintf(NomeFile,"Exported_ScriptTrigger_$%04X_%d_$%X.txt",
				Valore1,Valore2,Valore3);
	}


	pFile = fopen(GetFileTemp(NomeFile),"wb");
	fprintf(pFile, "\r\n%s",GetDescrizioneTrigger(hDialogo, true));
	if (PluginIdExport > 0) {
		fprintf(pFile, "; WARNING:\r\n");
		fprintf(pFile, "; If you'll give this trigger to other people or you'll use it with a script.txt different than current\r\n");
		fprintf(pFile, "; you should add to the script, first of triggergroup where you'll use this trigger, the following line:\r\n");
		fprintf(pFile, "; #define @%s  %d\r\n",
			pNomePlugin, PluginIdExport);
	}

	if (TestCollision == false) {
		if (PluginIdExport > 0) {
			fprintf(pFile, "; Values to add in script command: $%06X, %d, $%X\r\n",
				Valore1, Valore2, Valore3);
		}else {
			fprintf(pFile, "; Values to add in script command: $%04X, %d, $%X\r\n",
				Valore1, Valore2, Valore3);
		}
	}else {
		fprintf(pFile, "; YOU CANN'T EXPORT Collision Triggers because they are FAKE triggers\r\n");

	}



	fclose(pFile);

	ShellExecute(NULL,"open",GetFileTemp(NomeFile),"","",SW_SHOW);

}
// visualizza in un file di testo i dati relativi ad anim comamnd
// setposition corrispondente al trigger attuale
void ShowExportTrigger(HWND hDialogo)
{
	short ValoreX, ValoreY, ValoreZ;
	short Frame;
	char *pChar;
	int Numero;
	short Temp;
	FILE *pFile;
	char NomeFile[256];
	bool TestCollision;

	TestCollision=false;

	if (BaseTempTrigger.IdPlugin > 0) {
		MioMessageBox("Sorry, you cann't export as AnimCommand a trigger handled by some plugin. There is not enough space in 3 words of animcommand to store all required infos.\r\nAnyway you can bypass this limit, exporting as animcommand a common tomb_nextgeneration trigger to perform a TriggerGroup in script file, and then in that triggergroup you'll be able to export triggers from plugin engines","Warning: trigger of plugin");
		return;
	}
	if (BaseTempTrigger.TriggerWhat == 9 && 
		BaseTempTrigger.Valore >= 310 && BaseTempTrigger.Valore <= 329) {

		// non consentire exporting
		TestCollision=true;
	}

	BufferLog[0]=0;
	Frame =0;
	if (TestCollision ==false) {

		pChar=InputTextBox("Type the FRAME number for this ng anim command.\r\n(This anim command will be performed when this frame will be played)\r\n\r\nNote: type -1 if you wish that this animcommand will be performed in all frames of current animation. (Useful for conditional triggergroup)",
				"Insert Frame Number", "0",TP_CENTRALE);
		if (pChar == NULL) return;
		Numero = -20;
		sscanf(pChar,"%d", &Numero);
		if (Numero <  -1) {
			MessageBox(hDialogo,"ERROR: non correct numberic value","ERROR frame",0);
			return;
		}
		
	}

	Frame = (short) Numero;
	Frame &= 0xFF; // -1 diventa 255
	ValoreX=0;
	ValoreY=0;
	ValoreZ=0;
	// mettere frame
	ValoreX |= Frame;
	// mettere segnale di ng anim command 
	ValoreX |= 0x8000;

	// mettere bit per tipo action o flipeffect
	if (BaseTempTrigger.TriggerWhat == 9) {
		// tipo flipeffect
		ValoreX |= 0x2000;
		// mettere numero di flipeffec
		ValoreY = BaseTempTrigger.Valore;
		// salvare valore timer e extra timer
		ValoreZ = BaseTempTrigger.Timer;

	}else {
		// e' action: chiedee se il comando agira su oggetto che possiede
		// azione
		if (TestCollision==false) {

			if (MessageBox(hDialogo, "Do you want that the current action will act on same object owns the animation?\r\n(Note: if you choose NO the action will be performed only on current object selected in trigger window)",
				"What object",MB_YESNO)== IDNO	) {
				// si usa indice di oggetto
				ValoreX |= 0x1000;
				// copiare indice oggetto
				Temp = BaseTempTrigger.Valore;
				// mettere parte bassa di indice
				Temp &= 255;
				Temp = Temp << 8;
				ValoreY |= Temp;
				// ora mettere i due eventuali bit alti in bytealto valorex
				Temp = BaseTempTrigger.Valore & 0x0F00;
				ValoreX |= Temp;
			}
		}
		// siamo in action
		// inserire il numero di azione (presente in byte basso di timer
		Temp = BaseTempTrigger.Timer & 0xff;
		ValoreY |= Temp;
		// salvare valore extra timer
		Temp = BaseTempTrigger.Timer & 0xff00;
		ValoreZ = Temp;



	}
	sprintf(NomeFile,"Exported_AnimCommand_%d_%d_%d.txt",
		ValoreX, ValoreY, ValoreZ);

	pFile = fopen(GetFileTemp(NomeFile),"wb");

	if (TestCollision==false) {
		fprintf(pFile,"Add a SetPosition anim command with following values:\r\n\r\nSet Position [%d] [%d] [%d]\r\n",
			ValoreX,ValoreY,ValoreZ);
	}else {
		fprintf(pFile,"YOU CANN'T EXPORT Collision triggers because they are FAKE triggers");
	
	}
	fprintf(pFile, "\r\nInfos about exported trigger\r\n-------------------------------------------------------------------------------\r\n%s\r\n",
		GetDescrizioneTrigger(hDialogo,false));
	if (Frame == 255) {
		fprintf(pFile, "AnimCommand to perform in all frames of animation (set as -1 the number of the frame)\r\n");
	}else {
		fprintf(pFile, "AnimCommand: Frame=%d  ", Frame);
	}
	if (BaseTempTrigger.TriggerWhat == 11) {
		if ((ValoreX & 0x1000)==0) {
			fprintf(pFile, "MoveableIndex= <Index of owner of current AnimCommand>");
		}else {
			fprintf(pFile, "MoveableIndex= %d", BaseTempTrigger.Valore);
		}
	}
	fprintf(pFile,"\r\n");


	fprintf(pFile,"-------------------------------------------------------------------------------\r\n\r\n");

	fclose(pFile);

	ShellExecute(NULL,"open",GetFileTemp(NomeFile),"","",SW_SHOW);

}
// cerca in hCombo la maschera di ricerca BufMask e seleziona
// l'item trovato
bool CercaInCombo(HWND hDialogo, HWND hCombo, char BufMask[])
{
	char **pVetParti;
	int TotParti;
	int i;
	char *pChar;
	int TotItems;
	int StartIndex;
	char BufLinea[256];
	bool TestOk;
	int j;

	pChar = Trim(BufMask);
	if (pChar[0] == 0) return false;

	pVetParti = Split(pChar, ' ', &TotParti,NULL,NULL,false);

	TotItems = SendMessage(hCombo, CB_GETCOUNT,0,0);

	if (TotItems==0) return false;

	StartIndex = SendMessage(hCombo, CB_GETCURSEL, 0,0);

	if (StartIndex == CB_ERR) StartIndex =0;


	i = StartIndex+1;
	if (i >= TotItems) i =0;

	while (i != StartIndex) {


		if (i == 118) {
			BufferLog[0]=0;
		}
		// vedere se in elemento (i) la maschera di ricerca e' soddisfatta
		SendMessage(hCombo, CB_GETLBTEXT, i, (int) &BufLinea);

		// ora verificare se in questo testo ci sono tutte le porzioni di stringa
		TestOk=true;
		for (j=0;j<TotParti;j++) {
			if (InStr(0, BufLinea, pVetParti[j]) == -1) {
				TestOk=false;
				break;
			}
		}

		if (TestOk == true) {
			// visualizzare l'elemento
			SendMessage(hCombo, CB_SETCURSEL, i, 0);
			return true;
		}
		i++;
		if (i >= TotItems) i =0;
	}
		
	MessageBox(hDialogo, "Not Found", "NGLE", MB_OK);
	return false;
	
}

// se testo e' nel formato [-24573] [217] [3]
// restituire true e in pWord i tre valori
// se c'e' un errore di sintassi imposta testerrore=true
bool ImputAnimCommand(char *pTesto, WORD *pWord, bool *pTestErrore)
{
	char MioCar;
	char StrNumero[20];
	int i;
	int TotNumeri;
	int Indice;
	int Numero;
	DWORD j;

	TotNumeri=0;

	*pTestErrore=false;

	Indice=InStr(0, pTesto, "[");
	if (Indice==-1) return false;

	i=Indice;

	while (pTesto[i]) {
		MioCar=pTesto[i];

		if (MioCar=='[') {
			// trovato inizio numero
			// copiarlo fino a "]"
			i++;
			for (j=0;pTesto[j+i];j++) {
				if (pTesto[j+i] == ']') break;
				StrNumero[j]=pTesto[j+i];
			}
			StrNumero[j]=0;
			Numero=GetNumero(StrNumero, pTestErrore);
			if (*pTestErrore) {
				sprintf(BufferLog, "Not valid numeric value in %dth field \"%s\"", TotNumeri+1, StrNumero);
				MessageBox(NULL, BufferLog, "NGLE", MB_OK);
				return false;
			}				
			pWord[TotNumeri++] = (WORD) Numero;
			i += j;
			i--;
			if (TotNumeri> 3) break;
				
		}
		i++;
	}
	
	if (TotNumeri != 3) {
		sprintf(BufferLog, "ERROR: wrong number of arguments: %d (instead by 3)", TotNumeri);
		MessageBox(NULL, BufferLog, "NGLE", MB_OK);
		*pTestErrore=true;
		return false;
	}

	if ((pWord[0] & 0x8000) ==0) {
		MessageBox(NULL, "ERROR: This numeric triple is not a TRNG exported anim command but only a common (default) SetPosition animcommand","NGLE", 0);
		*pTestErrore=true;
		return false;

	}
	return true;
}
// se testo e' nel formato "$9000, 530, $254"
// restiuisce true e imposta in pWord1/2/3 le tre cifre

bool InputExportedTrigger(char *pTesto, WORD *pWord, bool *pTestErrore, WORD *pPluginId)
{
	char **VetParti;
	int Numero;
	int TotParti;
	char *pChar;
	int i;

	*pTestErrore=false;

	if (InStr(0, pTesto, ",")==-1) return false;



	VetParti = Split(pTesto, ',', &TotParti, NULL, NULL, false);

	if (TotParti != 3) {
		MessageBox(NULL, "Syntax error: I cann't find three fields in the script trigger string you typed", "NGLE", MB_OK);
		*pTestErrore=true;
		return false;
	}

	for (i=0;i<TotParti;i++) {
		pChar = VetParti[i];
		pChar = Trim(pChar);
		Numero = GetNumero(pChar, pTestErrore);

		pWord[i]=(WORD) Numero;

		if (*pTestErrore==true) {
			sprintf(BufferLog, "Syntax error: number %d is unvalid", i+1);
			MessageBox(NULL, BufferLog, "NGLE", MB_OK);
			return false;
		}

		if (i== 0) {
			*pPluginId = (Numero >> 16) & 0x7fff;
		}
	}

	if (pWord[0]== TGROUP_COMMAND) {
		MessageBox(NULL, "Warning: These values are about a TGROUP_COMMAND. It is not a real trigger!", "NGLE", MB_OK);
		*pTestErrore=true;
		return false;

	}
	return true;

}
// riceve in input un testo che dovrebbe contenere un numero decimale
// se tutto ok restituisce il vlaore numerico corrispondente, 
// se c''e erroe imposta *pTestErrore = true
// Nota: prima effettua un trim() nel caso ci fossero spazi non significagivi

int PrendiNumeroTrim(char *pBuffer, bool *pTestErrore)
{
	char *pChar;
	int Numero;

	*pTestErrore=false;

	pChar = Trim(pBuffer);

	Numero=-1234567;

	sscanf(pChar, "%d", &Numero);

	if (Numero == -1234567) {
		*pTestErrore=true;
		return 0;
	}
	return Numero;

}
// riceve in input il campo testuale di trigger esportato mediandte funzione
// cerca di restituire un valore numerico
// il formato potrebbe essere "223 | NGLE_INDEX"
// ma anche senza il | NGLE_INDEX
// oppure con una variabile, nel qual caso si restituire' sempre 0
// non da mai errore ma se trova un valore numrico lo rewtituisce

int PrendiIndiceOggetto(char *pBuffer)
{
	char *pChar;
	char BufNumero[40];
	DWORD i;
	char MioCar;
	int Numero;

	pChar=Trim(pBuffer);
	for (i=0;i<strlen(pChar);i++) {
		MioCar=pChar[i];
		if (MioCar < '0' || MioCar > '9') break;
		BufNumero[i]=MioCar;
	}
	BufNumero[i]=0;
	
	PrendiNumero(BufNumero, &Numero);
	return Numero;

}
// InputFunctionTrigger(pChar, VetWords, &TestErrore, pNomePlugin);
// verifica se il testo inserito e' un trigger esportato come funzione
// scrive 3 word in VetWords usando stesso formato di script triggers
bool InputFunctionTrigger(char * pChar, WORD VetWords[], bool *pTestErrore, char **pNomePlugin)
{
	// tipo di fuznzione
	static char *VetFunzioni[] = {"PerformActionTrigger(", "PerformFlipeffect(", "PerformConditionTrigger("};
	int i;
	int Indice;
	int TipoTrigger;
	int Inizio;
	int Fine;
	char MioBuffer[100];
	int j;
	char **pVetParti;
	int TotParti;
	char *pArg;
	static char BufNome[100];
	WORD Extra;

	*pTestErrore=false;

	for (i=0;i<3;i++) {
		Indice = InStr(0, pChar, VetFunzioni[i]);
		if (Indice != -1) break;
	}

	if (i==3) {
		// non e' di tipo funzione
		return false;
	}
	// e' una funzione esportata
	// inizializzare valori
	*pNomePlugin="";
	VetWords[0]=0;
	VetWords[1]=0;
	VetWords[2]=0;

	TipoTrigger=i;

	// ora dividere in campi gli argomenti tra parentesi tonde
	Inizio= Indice+strlen(VetFunzioni[TipoTrigger]);
	Fine = InStr(Inizio, pChar, ")");
	if (Fine==-1) {
		*pTestErrore=true;
		MessageBox(NULL, "Syntax error: I cann't find final round parenthesis inside of this function", "NGLE", MB_OK);
		return true;
	}
	i=0;
	for (j=Inizio;j<Fine;j++) {
		MioBuffer[i++] = pChar[j];
	}
	MioBuffer[i]=0;

	// ora divicdere usando come separatori le virgole
	pVetParti= Split(MioBuffer, ',', &TotParti, NULL, NULL, false);

	if (TotParti != 4) {
		MessageBox(NULL, "Syntax error: unexpected number of input arguments for current function. They should be four arguments", "NGLE", MB_OK);
		*pTestErrore=true;
		return true;
	}

	// trovare nome di plugin
	pArg= Trim(pVetParti[0]);

	if (strcmpi(pArg, "NULL") != 0) {
		// c'e' un nome di plugin
		// devo solo togliere viroglette all'inizio e alla fine
		Inizio=1;
		Fine=InStr(Inizio, pArg, "\"");
		if (Fine==-1) {
			MessageBox(NULL, "Syntax error: missing of final quote character to bound Plugin's name.", "NGLE", MB_OK);
			*pTestErrore=true;
			return true;
		}

		i=0;
		for (j=Inizio;j<Fine;j++) {
			BufNome[i++] = pArg[j];
		}
		BufNome[i]=0;
		*pNomePlugin= BufNome;

	}

	// calcoli diversi a seconda di tipo trigger:
	switch (TipoTrigger) {
	case 0:
		// PerformActionTrigger(NULL, 89, 188 | NGLE_INDEX, 5);
		VetWords[0] = TGROUP_ACTION;
		VetWords[1] = PrendiIndiceOggetto(pVetParti[2]);
		VetWords[2] = PrendiNumeroTrim(pVetParti[1], pTestErrore);
		
		if (*pTestErrore == true) {
			MessageBox(NULL, "Syntax error: second input parameter of function is not a valid decimal value", "NGLE", MB_OK);
			*pTestErrore=true;
			return true;
		}
		// aggiungi extra timer
		Extra= PrendiNumeroTrim(pVetParti[3], pTestErrore);
		if (*pTestErrore == false) {
			VetWords[2] |= (Extra << 8);
		}

		break;
	case 1:
		// PerformFlipeffect("Plugin_trng", 800, 1, 0);
		VetWords[0] = TGROUP_FLIPEFFECT;
		VetWords[1] = PrendiNumeroTrim(pVetParti[1], pTestErrore);
		if (*pTestErrore == true) {
			MessageBox(NULL, "Syntax error: second input parameter of function is not a valid decimal value", "NGLE", MB_OK);
			*pTestErrore=true;
			return true;
		}
		VetWords[2] = PrendiNumeroTrim(pVetParti[2], pTestErrore);
		if (*pTestErrore == true) {
			MessageBox(NULL, "Syntax error: third input parameter of function is not a valid decimal value", "NGLE", MB_OK);
			*pTestErrore=true;
			return true;
		}		

		// aggiungere extra timer

		Extra= PrendiNumeroTrim(pVetParti[3], pTestErrore);
		if (*pTestErrore == false) {
			VetWords[2] |= (Extra << 8);
		}
		break;
	case 2:
		// PerformConditionTrigger("Plugin_trng", 1, 223 | NGLE_INDEX, 0);
		VetWords[0] = TGROUP_CONDITION_TRIGGER;
		VetWords[2] = PrendiNumeroTrim(pVetParti[1], pTestErrore);
		if (*pTestErrore == true) {
			MessageBox(NULL, "Syntax error: second input parameter of function is not a valid decimal value", "NGLE", MB_OK);
			*pTestErrore=true;
			return true;
		}
		VetWords[1] = PrendiIndiceOggetto(pVetParti[2]);
		Extra= PrendiNumeroTrim(pVetParti[3], pTestErrore);
		if (*pTestErrore == false) {
			VetWords[2] |= (Extra << 8);
		}

		break;
	}
	return true;
	
}
// clickato su pulsante per impostare numero di trigger attuale
void ShowNumeroTrigger(HWND hDialogo)
{
	char *pChar;
	DWORD i;
	char MioCar;
	DWORD j;
	char Tipo;
	int Numero;
	WORD VetWords[5];
	bool TestScript;
	bool TestErrore;
	DWORD Valore;
	bool TestAnimCommand;
	int Indice;
	char MyBuffer[100];
	char *pNomePlugin;
	WORD PluginId;
	bool TestFunction;


	pChar = InputTextBox("You can locate a trigger using one of following formats:\r\n\r\n\"F143\" (Flipeffect 143)  \"A38\" (Action 38)  \"C21\" (Condition 21)\r\n\"$9000, 530, $254\" (Exported Script Trigger)\r\n\"[-24573] [217] [3]\" (Exported AnimCommand Trigger)\r\n\r\nYou can add in front of all above formats the \"PluginName:\" text to locate a trigger for given plugin\r\n\r\nYou can also insert an exported trigger function in C++ syntax, like:\r\nPerformConditionTrigger(\"Plugin_trng\", 1, 223 | NGLE_INDEX, 0);",
		"Find Trigger Number", "", TP_CENTRALE);
	if (pChar == NULL) return;

	pNomePlugin="";
	i=InStr(0, pChar, ":");
	if (i != -1) {
		pNomePlugin=pChar;

		pChar= &pChar[i+1];

		pNomePlugin[i]=0;
	}


	Tipo=0;
	PluginId=0;

	TestFunction=false;
	TestScript=false;
	TestAnimCommand=false;

	// vedere se e' tipo funzione esportata, una cosa del tipo:
	// PerformConditionTrigger("Plugin_trng", 1, 223 | NGLE_INDEX, 0);

	TestFunction = InputFunctionTrigger(pChar, VetWords, &TestErrore, &pNomePlugin);

	if (TestErrore==true) return;

	if (TestFunction==false) {
		// vedere se e' il genere tripletta
		TestScript = InputExportedTrigger(pChar, VetWords, &TestErrore, &PluginId);

		if (TestErrore) return;

	}

	if (TestScript==false && TestFunction==false) {
		TestAnimCommand= ImputAnimCommand(pChar, VetWords, &TestErrore);
		if (TestErrore) return;
	}
	
	if (TestScript==false && TestAnimCommand==false && TestFunction==false) {

		for (i=0;i<strlen(pChar);i++) {
			// trovare la prima lettera
			MioCar = pChar[i];
			
			if (MioCar != ' ') {
				Tipo = MioCar;
				break;
			}
		}
		
		// ora cercare il primo numero
		Numero = -1;
		for (j=i+1;j<strlen(pChar);j++) {
			MioCar = pChar[j];

			if (MioCar >= '0' && MioCar <= '9') {
				pChar += j;
				sscanf(pChar,"%d", &Numero);
				break;
			}
		
		}
	}
	// qui interpretare il tipo di trigger a seconda dei vlaori Numero1/2/3
	if (TestScript==true || TestFunction==true) {
		Tipo=0;
		Numero=-1;
		Valore=0;

		if (VetWords[0] & TGROUP_CONDITION_TRIGGER) {
			Tipo='C';
			Numero = VetWords[2] & 0xff;
		}

		if (VetWords[0] & TGROUP_ACTION) {
			Tipo='A';
			Numero = VetWords[2] & 0xff;
		}

		if (VetWords[0] & TGROUP_FLIPEFFECT) {
			Numero = VetWords[1];
			Tipo='F';

		}

	}
	if (TestAnimCommand) {
		Tipo=0;
		Numero=-1;
		Valore=0;
		// era anim command: DECOMPRIMERE significato
		if (VetWords[0] & 0x2000) {
			// tipo flipeffect
			Tipo='F';
			Numero = VetWords[1] & 0x3ff;
		}else {
			// action trigger
			Tipo='A';
			Numero = VetWords[1] & 0xff;
		}

	}

	
	BaseTempTrigger.IdPlugin= (BYTE) PluginId;

	if (TestScript==false) {

		if (pNomePlugin[0]) {
			// e' stato fornito nome di plugin
			// cercarlo in tutti i plugin caricati
			for (i=0;i< (DWORD) GlobNgle.TotPluginNgle;i++) {
				if (InStr(0, GlobNgle.VetPluginNgle[i].Nome, pNomePlugin) != -1) {
					BaseTempTrigger.IdPlugin= GlobNgle.VetPluginNgle[i].PluginId;
					break;
				}
			}

			if (i == (DWORD) GlobNgle.TotPluginNgle) {
				sprintf(BufferLog, "WARNING: cann't locate any plugin with text \"%s\" in its name", pNomePlugin);

				MioMessageBox(BufferLog,"Plugin not found");
			}
		}
	}
	
	if (Tipo == 0 || Numero == -1) {
		MioMessageBox("Syntax Error", "Error");
		return;
	}

	BaseTempTrigger.TestExtraButtons=false;
	BaseTempTrigger.TipoValoreTimer = TIV_TIMER_ALL;

	switch (Tipo) {
	case 'f':
	case 'F':
		// impostare flipeffect
		BaseTempTrigger.TriggerWhat = 9;
		BaseTempTrigger.Valore = Numero;
		BaseTempTrigger.TriggerActivation = 0;
		BaseTempTrigger.Timer =0;
		BaseTempTrigger.TipoValoreTimer =TIV_TIMER_ALL;
		if (TestScript == true || TestFunction== true) {
			BaseTempTrigger.Timer = VetWords[2];
			if (VetWords[2] & 0xff00) {
				BaseTempTrigger.TestExtraTimer = true;
				BaseTempTrigger.TipoValoreTimer = TIV_TIMER_ALTO;
			
			}

		}
		if (TestAnimCommand) {
			BaseTempTrigger.Timer = VetWords[2];
			if (BaseTempTrigger.Timer & 0xff00) {
				BaseTempTrigger.TestExtraTimer = true;
				BaseTempTrigger.TipoValoreTimer = TIV_TIMER_ALTO;
			}
		}
		break;
	case 'a':
	case 'A':
		// imppostare action
		BaseTempTrigger.TriggerWhat = 11;
		BaseTempTrigger.Valore = 0;
		BaseTempTrigger.TriggerActivation = 0;
		BaseTempTrigger.Timer = Numero;
		BaseTempTrigger.TipoValoreTimer =TIV_TIMER_BASSO;
		if (TestScript == true || TestFunction== true) {
			BaseTempTrigger.Valore = VetWords[1];
			BaseTempTrigger.Timer = VetWords[2];

			if (VetWords[2] & 0xff00) {
				BaseTempTrigger.TestExtraTimer =true;
				
			}
		}

		if (TestAnimCommand) {
			if (VetWords[0] & 0x1000) {
				// indice
				Indice= (VetWords[1] >> 8) & 0xff;
				Indice |= (VetWords[0] & 0x0f00);
				BaseTempTrigger.Valore = Indice;


			}
			// ricostruire timer
			Indice= VetWords[1] & 0xff;
			Indice |= (VetWords[2] & 0xff00);
			BaseTempTrigger.Timer = Indice;
			if (BaseTempTrigger.Timer & 0x7ff00) {
				BaseTempTrigger.TestExtraButtons=true;
			}

		
		}



		break;
	case 'c':
	case 'C':
		// condizione
		BaseTempTrigger.TriggerActivation = 12;
		BaseTempTrigger.TriggerWhat = 0;
		BaseTempTrigger.Timer = Numero;
		BaseTempTrigger.TipoValoreTimer =TIV_TIMER_BASSO;
		BaseTempTrigger.Valore = 0;
		if (TestScript == true || TestFunction== true) {
			BaseTempTrigger.Valore = VetWords[1];
			if (VetWords[2] & 0x7f00) {
				BaseTempTrigger.TestExtraButtons =true;
				BaseTempTrigger.MaskPulsanti &= 1;
				BaseTempTrigger.MaskPulsanti |= ((VetWords[2] & 0x7f00) >> 7);
			}
		}
		break;
	default:
		// non e' stata inserita la lettera per il tipo di trigger
		MessageBox(hDialogo, MexMancaLetteraTrigger, 
					"Unvalid Trigger Format",0);
		return;
	}

	ImpostaValoriTrigger(hDialogo);

	// se era animcommand visualizzare valori extra in un messagebox
	if (TestAnimCommand) {
		if ((VetWords[0] & 0x3000) == 0) {
			pChar="This action animcommand will act on the object that owns this animcommand (no item index set)";
		}else {
			pChar="";
		}
		j= VetWords[0] & 0xff;
		if (j==255) {
			strcpy(MyBuffer, "in all frames");
		}else {
			sprintf(MyBuffer, "on the frame number %d", j);
		}
		sprintf(BufferLog, "Extra infos about exporting of this AnimCommand:\r\n\r\nThe animcommand has been set to work %s\r\n%s",
			MyBuffer, pChar);
		MessageBox(hDialogo, BufferLog, "Extra info about current AnimCommand", 0);
	}




}
// restituisce testo che descrive in modo sintenico trigger attuale
char * GetDescrizioneTrigger(HWND hDialogo)
{
	HWND TempWind;
	char BufNomeTitolo[256];
	char NomePlugin[256];
	int Indice;
	char *pMexTrigger;
	static char DescTrigger[256];

	TempWind=hDialogo;
	GetWindowText(TempWind, BufNomeTitolo, 255);

	TempWind = GetDlgItem( hDialogo, IDC_COMBO_PLUGIN);
	GetWindowText(TempWind, NomePlugin, 255);

	Indice= InStr(0, BufNomeTitolo, "- ");
	if (Indice == -1) {
		Indice=0;
	}

	Indice +=2;
	pMexTrigger = &BufNomeTitolo[Indice];

	sprintf(DescTrigger, "%s_%s", NomePlugin, pMexTrigger);
	return DescTrigger;


}
void ShowRemark(HWND hDialogo)
{
	char *pChar;
	char NomeFile[256];

	FILE *pFile;

	pChar= BaseTempTrigger.pMexRemark;
	if (pChar==NULL) {
		MessageBox(NULL, "Current trigger has no remark", "Ngle Warning", 0);
		return;
	}

	sprintf(NomeFile, "%s\\Remark_%s.txt", MyTempFolder, GetDescrizioneTrigger(hDialogo));

	pFile=fopen(NomeFile, "wb");
	fprintf(pFile, pChar);
	fclose(pFile);
	
	ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);
}
// restituisce il nome del plugin con id IdPlugin
// resittuisce NULL se non c'e' un plugin con quell'id
// restituisce "TRNG" se IdPlugin =0

char * TrovaNomePlugin(int IdPlugin)
{

	char *pNomePlugin;
	int i;

	if (IdPlugin ==0) {
		// e' next generation
		return "TRNG";
	}

	// trovare il nome dle plugin con quell'id in ngle
	for (i=0;i<GlobNgle.TotPluginNgle;i++) {
		if (GlobNgle.VetPluginNgle[i].PluginId == BaseTempTrigger.IdPlugin) {
			pNomePlugin= GlobNgle.VetPluginNgle[i].Nome;
			break;
		}
	}
	if (i==GlobNgle.TotPluginNgle) {
		sprintf(BufferLog, "INTERNAL ERROR: cann't locate the plugin name linked with Ngle plugin ID=%d",
			BaseTempTrigger.IdPlugin);
		MioMessageBox(BufferLog, "Error Plugin ID");
		return NULL;
	}
	return pNomePlugin;
}
// mostra l'help del trigger attuale
void ShowTriggerHelp(HWND hDialogo)
{
	char *pChar;
	char NomeFile[256];
	int IndiceStart;
	int IndiceEnd;
	int i;
	char PezzoNome[80];
	int j;
	char *pDirNgCenter;
	char *pNomePlugin;

	FILE *pFile;

	pChar= BaseTempTrigger.pMexHelp;
	if (pChar==NULL) {
		MessageBox(NULL, "Current trigger has not any help description", "Ngle Warning", 0);
		return;
	}
	// controllare se in help c'e' il riferimento ad un file esterno
	// sintassi @"FileName.ext"
	IndiceStart=InStr(0, pChar, "@\"");
	if (IndiceStart != -1) {
		IndiceStart+=2;

		IndiceEnd= InStr(IndiceStart, pChar, "\"");
		if (IndiceEnd != -1) {
			// estrarre nome e cercare di localizzarlo
			j=0;
			for (i=IndiceStart;i<IndiceEnd;i++) {
				PezzoNome[j++] = pChar[i];
			}

			PezzoNome[j]=0;

			// ora trovare cartella di ng_center
			pDirNgCenter=TrovaCartellaNgCenter();
			if (pDirNgCenter==NULL) {
				sprintf(BufferLog, "Internal error: cann't locate the folder of NG_Center program.\r\nImpossible opening external help file: \"%s\"",
					PezzoNome);
				MioMessageBox(BufferLog, "Error Message");
				return;
			}

			// ora trovare nome di sottocartella (la stessa di plugin)
			pNomePlugin=TrovaNomePlugin(BaseTempTrigger.IdPlugin);

			if (pNomePlugin==NULL) return;

			if (strcmpi(pNomePlugin, "TRNG")==0) {
				// usare come sottocartella quella nuova "DOCs"
				pNomePlugin = "DOCs";
			}

			// crare nome file completo
			sprintf(NomeFile, "%s\\%s\\%s", 
				pDirNgCenter, pNomePlugin, PezzoNome);
			if (EsisteFile(NomeFile)==false) {
				sprintf(BufferLog, "ERROR: cann't find the external help file in path: \"%s\"", NomeFile);
				MioMessageBox(BufferLog, "Error Message");
				return;

			}


		}
	}else {
		// controllare se nella descrizione c'e' uno o piu' link "http:" e in quel caso estrarli
		// e poi chiedere se si vuole aprirli #dafare#

		sprintf(NomeFile, "%s\\Help_%s.txt", MyTempFolder, GetDescrizioneTrigger(hDialogo));

		pFile=fopen(NomeFile, "wb");
		fprintf(pFile, pChar);
		fclose(pFile);
	}
	// aprire il file
	ShellExecute(NULL,"open",NomeFile,"","",SW_SHOW);

	
}

BOOL CALLBACK WindProcedureTriggers(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	char **VetNomiTriggerWhat = (char**) 0x569CC0;
	int *pTotNomiTriggerWhat = (int *) 0x569B88;
	char **VetNomiTriggerAttivazione = (char**) 0x569C00;
	int *pTotNomiTriggerAttivazione = (int*) 0x569CA0;
	int i;
	HWND WindCombo;
	int Indice;
	int Numero;
	HWND TempWind;
	int Valore;
	char BufMask[256];


	sprintf(BufferLog,"uMsg=0x%X  wParam=0x%X", uMsg, wParam);
	InviaLog(BufferLog);

	switch (uMsg) {
		case  WM_INITDIALOG:
			// qui bisogna inserire in  BaseTempTrigger
			// i dati attuali di trigger
			ImpostaTempTrigger();

			// riempire i combo box
			// ---- Combo plugin -------------
			WindCombo= GetDlgItem(hDialogo,IDC_COMBO_PLUGIN);
			SendMessage(WindCombo, CB_RESETCONTENT, 0, 0);

			// prima aggiungere sempre l'id di tombnexteneration
			Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
					(LPARAM) "Tomb_NextGeneration");
			SendMessage(WindCombo,CB_SETITEMDATA,Indice, 0 );
				
			
			for (i=0;i< GlobNgle.TotPluginNgle;i++) {
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
						(LPARAM) GlobNgle.VetPluginNgle[i].Nome);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, GlobNgle.VetPluginNgle[i].PluginId );
			}
			SelezionaComboValore(WindCombo, BaseTempTrigger.IdPlugin);

			// --- Combo trigger what ------
			WindCombo = GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_WHAT);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			for (i=0;i<*pTotNomiTriggerWhat;i++) {
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
						(LPARAM) VetNomiTriggerWhat[i]);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}
			SelezionaComboValore(WindCombo,BaseTempTrigger.TriggerWhat);


			// --- combo  type trigger actrivation
			WindCombo = GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_TYPE);
			SendMessage(WindCombo,CB_RESETCONTENT,0,0);
			for (i=0;i<*pTotNomiTriggerAttivazione;i++) {
				Indice = SendMessage(WindCombo,CB_ADDSTRING,0,
						(LPARAM) VetNomiTriggerAttivazione[i]);
				SendMessage(WindCombo,CB_SETITEMDATA,Indice, i);
			}
			SelezionaComboValore(WindCombo,BaseTempTrigger.TriggerActivation);



			ImpostaValoriTrigger(hDialogo);

			return FALSE;



		case WM_COMMAND:

			switch (LOWORD(wParam)) {
				case IDC_PUL_EXPORT_REMARK:
					ShowRemark(hDialogo);
					return TRUE;
				case IDC_PUL_HELP:
					ShowTriggerHelp(hDialogo);
					return TRUE;
				case IDCANCEL:
					EndDialog(hDialogo,0);
					return TRUE;
				case ID_PUL_OK_TRIGGER:
					if (PrendiValoriTrigger(hDialogo)== false) return TRUE;
					EndDialog(hDialogo,1);
					return TRUE;
				case IDC_PUL_EXPORT_ANIMCOMMAND:
					// mostrare i valori di basetemptrigger in un file
					// di testo
					ShowExportTrigger(hDialogo);
					return TRUE;
				case IDC_PUL_TRIGGER_NUMBER:
					// pulsante per scegliere un trigger dal suo numero
					ShowNumeroTrigger(hDialogo);
					return TRUE;

				case IDC_PUL_EXPORT_SCRIPT:
					// mostrare valori per trigger script
					ShowExportScriptTrigger(hDialogo);
					return TRUE;
				case IDC_PUL_EXPORT_FUNCTION:
					// mostrare esport per funzione
					ShowExportFunction(hDialogo);
					return TRUE;

				case IDC_PUL_LIST_VALORI:
					// lista di combo valore
					WindCombo = GetDlgItem(hDialogo, IDC_COMBO_VALORE);
					ShowTextList(hDialogo,  WindCombo, true);
					return TRUE;

				case IDC_PUL_LIST_TIMER:
					// lista combo timer
					// visualizzare l'intero contenuto del list box in un file
					WindCombo = GetDlgItem(hDialogo, IDC_COMBO_TIMER);
					ShowTextList(hDialogo,  WindCombo, true);
					return TRUE;
				case IDC_PUL_LIST_EXTRA:
					// lista extra timer
					// visualizzare l'intero contenuto del list box in un file
					WindCombo = GetDlgItem(hDialogo, IDC_COMBO_EXTRA);
					ShowTextList(hDialogo,  WindCombo, true);
					return TRUE;
					

				case IDC_PUL_FIND_OBJ:
					GetWindowText(GetDlgItem(hDialogo, IDC_FIND_TEXT_OBJ), BufMask, 254);
					if (CercaInCombo(hDialogo, 
							GetDlgItem(hDialogo, 
							IDC_COMBO_VALORE), BufMask)==true) {

						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_VALORE);
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (Numero == BaseTempTrigger.Valore ) return TRUE;
						// e' cambiato valore di triggerwhat (object/parameter/sink/fmv)
						BaseTempTrigger.Valore = Numero;
						ImpostaValoriTrigger(hDialogo);
					}
					return TRUE;
				case IDC_PUL_FIND_TIMER:
					GetWindowText(GetDlgItem(hDialogo, IDC_FIND_TEXT_TIMER), BufMask, 254);
					if (CercaInCombo(hDialogo, 
							GetDlgItem(hDialogo, 
							IDC_COMBO_TIMER), BufMask)==true) {
						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_TIMER);
						
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (BaseTempTrigger.TipoValoreTimer != TIV_TIMER_ALL) {
							BaseTempTrigger.Timer &= 0xff00; 
							BaseTempTrigger.Timer |= Numero;
						}else {
							BaseTempTrigger.Timer = Numero;
						}

						if (BaseTempTrigger.TriggerActivation == 12) {
							// era condition: reimpostare pulsanti 
							BaseTempTrigger.MaskPulsanti |= 0x3e;
						}

						ImpostaValoriTrigger(hDialogo);
					}
					return TRUE;


				case IDC_PUL_ONESHOT:
				case IDC_PUL_1:
				case IDC_PUL_2:
				case IDC_PUL_3:
				case IDC_PUL_4:
				case IDC_PUL_5:
					LeggePulsantiTrigger(hDialogo);
					ImpostaValoriTrigger(hDialogo);
					return TRUE;

				case IDC_TEXT_VALORE:
					if (HIWORD(wParam) == EN_CHANGE) {
						// cambiato il valore in text box valore
						TempWind = (HWND) lParam;
						if (IsWindowVisible(TempWind)==FALSE) break;
						if (PrendiNumeroDaTextBox(TempWind,&Valore) == false) break;
						BaseTempTrigger.Valore = Valore;
						return TRUE;
						
					}
					return FALSE;
				case IDC_TEXT_TIMER:
					if (HIWORD(wParam) == EN_CHANGE) {
						// cambiato il valore in text box timer
						TempWind = (HWND) lParam;
						if (IsWindowVisible(TempWind)==FALSE) break;
						if (PrendiNumeroDaTextBox(TempWind,&Valore) == false) break;
						BaseTempTrigger.Timer  = Valore;
						return TRUE;
						
					}
					return false;
				case IDC_COMBO_PLUGIN:
					if (HIWORD(wParam) == CBN_CLOSEUP) {
						// combo trigger what
						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_PLUGIN);
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (Numero == BaseTempTrigger.IdPlugin ) return TRUE;
						// e' cambiato valore di plugin
						// azzerare quasi tutti i campi di basetemptrigger
						BaseTempTrigger.MaskPulsanti |= 0x3e;
						BaseTempTrigger.IdPlugin= Numero;
						ImpostaValoriTrigger(hDialogo);
					}

					return TRUE;

				case IDC_COMBO_TRIGGER_WHAT:
					if (HIWORD(wParam) == CBN_CLOSEUP) {

						// combo trigger what
						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_WHAT);
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (Numero == BaseTempTrigger.TriggerWhat) return TRUE;
						// e' cambiato valore di triggerwhat
						BaseTempTrigger.TriggerWhat = Numero;

						// se trigger what appena impostato e' ACTION
						// o FLIPEFFECT e attivazione era CONDITION
						// modficare CONDITION in trigger
						if (Numero == 0x9 || Numero == 0x0B) {
							if (BaseTempTrigger.TriggerActivation == 0x0c) {
								// forzare TRIGGER generico per attivazione
								BaseTempTrigger.TriggerActivation = 0;
								// e avvisare
								MessageBox(hDialogo, pErrorMixedTriggers, 
											"Mixed special new triggers",0);

							}
						}
						ImpostaValoriTrigger(hDialogo);
					}
					return TRUE;

				case IDC_COMBO_VALORE:
					if (HIWORD(wParam) == CBN_CLOSEUP) { 
						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_VALORE);
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (Numero == BaseTempTrigger.Valore ) return TRUE;
						// e' cambiato valore di triggerwhat
						BaseTempTrigger.Valore = Numero;
						ImpostaValoriTrigger(hDialogo);
					}
					return TRUE;

				case IDC_COMBO_TRIGGER_TYPE:
					if (HIWORD(wParam) == CBN_CLOSEUP) {

						WindCombo=GetDlgItem(hDialogo,IDC_COMBO_TRIGGER_TYPE);
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (Numero == BaseTempTrigger.TriggerActivation  ) return TRUE;
						// e' cambiato valore di triggertype
						BaseTempTrigger.TriggerActivation  = Numero;

						if (Numero == 0x0c) {
							// scelta CONDITION come attivazione
							// se il trigger what era action oflipeffect
							// modificare trigger what in parameter 
							// e dare avviso
							if (BaseTempTrigger.TriggerWhat == 0x9 ||
								BaseTempTrigger.TriggerWhat == 0xB) {
								// forzare PARAMETER come triggerwhat
								BaseTempTrigger.TriggerWhat = 0x0D;
								MessageBox(hDialogo, pErrorMixedTriggers, 
											"Mixed special new triggers",0);
							}
							// ripristinare pulsanti trigger
							BaseTempTrigger.MaskPulsanti |= 0x3e;

						}
						ImpostaValoriTrigger(hDialogo);
						
					}


					return TRUE;
				case IDC_COMBO_TIMER:
					if (HIWORD(wParam) == CBN_CLOSEUP) {

						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_TIMER);
						
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (BaseTempTrigger.TriggerActivation == 12) {
							// e' condizione: se e;' cambiato il numero di condizione, reimpostare pulsnati trigger
							if (Numero != (BaseTempTrigger.Timer & 0xff)) {
								// si, e' cambiato
								BaseTempTrigger.MaskPulsanti |= 0x3e;


							}

						}
						if (BaseTempTrigger.TipoValoreTimer != TIV_TIMER_ALL) {
							BaseTempTrigger.Timer &= 0xff00; 
							BaseTempTrigger.Timer |= Numero;
						}else {
							BaseTempTrigger.Timer = Numero;
						}



						ImpostaValoriTrigger(hDialogo);
					}
					return TRUE;
				case IDC_COMBO_EXTRA:
					if (HIWORD(wParam) == CBN_CLOSEUP) {
						// e' stato modificato valore di extra timer
						// copiare il valore in bit alti di timer
						// oppure in campo pulsanti
						// a seconda di cosa e' impostato
						WindCombo = GetDlgItem(hDialogo,IDC_COMBO_EXTRA);
						
						i = SendMessage(WindCombo,CB_GETCURSEL, 0,0);
						if (i == CB_ERR) break;
						Numero = SendMessage(WindCombo,CB_GETITEMDATA,i,0);
						if (BaseTempTrigger.TestExtraTimer == true) {

							Numero = Numero << 8;
							BaseTempTrigger.Timer &= 0xff;
							BaseTempTrigger.Timer |= Numero;

						}

						if (BaseTempTrigger.TestExtraButtons == true) {
							Numero &= 0x1f;
							Numero = Numero << 1;
							BaseTempTrigger.MaskPulsanti &= 0x0001;
							BaseTempTrigger.MaskPulsanti |= Numero;
						}
						ImpostaValoriTrigger(hDialogo);


					}

					return TRUE;


				case IDC_CHECK_NO_LIST:
					ImpostaValoriTrigger(hDialogo);
					return TRUE;

			}
	}
	return FALSE;

}

void ShowElencoProgrammi(HWND WindList, int IndiceAttivo)
{
	int i,n;


	SendMessage(WindList,LB_RESETCONTENT,0,0);
	for (i=0;i<MyPrefRoom.TotExternPrograms;i++) {
		n = i +1;
		if (n == 10) n=0;
		
		sprintf(BufferLog,"%-21s SHIFT %d    %-12s  %s",
			MyPrefRoom.VetExternPrograms[i].Titolo,
			n, FormattaTipoOpen(&MyPrefRoom.VetExternPrograms[i],false,false),
			MyPrefRoom.VetExternPrograms[i].PathExe);
		SendMessage(WindList,LB_ADDSTRING,0,(LPARAM) BufferLog);
	}
	if (MyPrefRoom.TotExternPrograms > 0) 
		SendMessage(WindList,LB_SETCURSEL,(WPARAM) IndiceAttivo,0);
	
}
BOOL CALLBACK WindProcedureProgrammi(HWND hDialogo, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND WindList;
	int i;
	int j;
	char *pTesto;
	char NomeExe[256];
	bool TestFlag;

	switch (uMsg) {
		case  WM_INITDIALOG:
			// riempire il list box
			WindList = GetDlgItem(hDialogo,IDC_LIST_PROGRAMMI);
			ShowElencoProgrammi(WindList,0);
			if (MyPrefRoom.Flags & FPR_NO_MINIMIZE) 
					CheckDlgButton(hDialogo,IDC_CHECK_NO_MINIMIZE,1);
			if (MyPrefRoom.Flags & FPR_USE_SHORT_PATH) 
					CheckDlgButton(hDialogo,IDC_CHEDCK_SHORT_PATH,1);

			return false;

		case	WM_COMMAND:
			if (HIWORD(wParam)== BN_CLICKED) {

				switch (LOWORD(wParam)) {

					case IDOK:
						// prendere opzioni
						if (IsDlgButtonChecked(hDialogo,IDC_CHECK_NO_MINIMIZE)==1) 
							MyPrefRoom.Flags |= FPR_NO_MINIMIZE;
						else
							MyPrefRoom.Flags &= ~FPR_NO_MINIMIZE;

						if (IsDlgButtonChecked(hDialogo,IDC_CHEDCK_SHORT_PATH)==1)
							MyPrefRoom.Flags |= FPR_USE_SHORT_PATH;
						else
							MyPrefRoom.Flags &= ~FPR_USE_SHORT_PATH;

						EndDialog(hDialogo,-1);
						return true;
					case IDC_NEW_PROGRAM:
						if (MyPrefRoom.TotExternPrograms == 10) {
							MessageBox(hDialogo,"Max number of programs is 10. You cann't add other program",
									"Error",MB_APPLMODAL);
							return true;
						}
						NomeExe[0]=0;
						PreparaOpenFile("Select program to add",
							"Executables (*.exe)",
							"*.exe",to_Load,NULL,NomeExe);
						if (GetOpenFileName(&ofn)==FALSE) break;
						// aggiungere nuovo record
						i = MyPrefRoom.TotExternPrograms;
						strcpy(MyPrefRoom.VetExternPrograms[i].PathExe, NomeExe);
						MyPrefRoom.VetExternPrograms[i].TipoOpen =0;
						MyPrefRoom.VetExternPrograms[i].Flags = 0;
						strcpy(MyPrefRoom.VetExternPrograms[i].Titolo,
									SoloNomeSenzaExt(NomeExe));
						i++;
						MyPrefRoom.TotExternPrograms = i;
						ShowElencoProgrammi(WindList,i-1);
						return true;
					case IDC_CHANGE_NAME:
						i = SendMessage(WindList,LB_GETCURSEL,0,0);
						if (i == LB_ERR) {
							MessageBox(hDialogo,"No item selected","Error",MB_APPLMODAL);
							return true;
						}
						pTesto = InputTextBox("Type a short name to identify current program.\r\nThis name will be showed in main window",
								"Set program name",
								MyPrefRoom.VetExternPrograms[i].Titolo,
								TP_CENTRALE);
						if (pTesto==NULL) return true;
						strcpy(MyPrefRoom.VetExternPrograms[i].Titolo,pTesto);
						ShowElencoProgrammi(WindList,i);
						return true;
					case IDC_CHANGE_FILE_OPEN:
						i = SendMessage(WindList,LB_GETCURSEL,0,0);
						if (i == LB_ERR) {
							MessageBox(hDialogo,"No item selected","Error",MB_APPLMODAL);
							return true;
						}
						sprintf(BufferLog,"Insert the type of file to open with current program.\r\n\r\nPossible values: %s\r\n\r\nNote: you can add in first position the character '#' to force that the file path was copied in clipboard, then you'll can paste the path of file to load with CTRL V  or 'paste' menu command", 
								FormattaTipoOpen(NULL,true,true));

						pTesto = InputTextBox(BufferLog,"File to open",
								FormattaTipoOpen(&MyPrefRoom.VetExternPrograms[i],
								true,false),
								TP_CENTRALE); 
						if (pTesto==NULL) return true;
						if (pTesto[0]=='#') {
							TestFlag=true;
							pTesto++;
						}else {
							TestFlag=false;
						}

						for (j=0;j<TotMiniOpen;j++) {
							if (strcmpi(pTesto,VetMiniOpen[j])==0) break;
						}

						if (j == TotMiniOpen) {
							MessageBox(hDialogo,"Syntax error: text you typed is not correct",
									"Error",MB_APPLMODAL);
							return true;
						}
						MyPrefRoom.VetExternPrograms[i].TipoOpen = j;
						
						if (TestFlag==true) 
							MyPrefRoom.VetExternPrograms[i].Flags |= FEP_CLIPBOARD;
						else
							MyPrefRoom.VetExternPrograms[i].Flags  &= ~FEP_CLIPBOARD;
						if (j == 5) {
							// aprire folder, togliere nome file
							strcpy(BufferLog,MyPrefRoom.VetExternPrograms[i].PathExe);
							strcpy(MyPrefRoom.VetExternPrograms[i].PathExe ,
										SoloDir(BufferLog));
							strcpy(MyPrefRoom.VetExternPrograms[i].Titolo, 
										SoloNome(MyPrefRoom.VetExternPrograms[i].PathExe));

						}
						ShowElencoProgrammi(WindList,i);

						return true;
					case IDC_REMOVE_PROGRAM:
						i = SendMessage(WindList,LB_GETCURSEL,0,0);
						if (i == LB_ERR) {
							MessageBox(hDialogo,"No item selected","Error",MB_APPLMODAL);
							return true;
						}
						for (j=i;j<MyPrefRoom.TotExternPrograms-1;j++) {
							memcpy( &MyPrefRoom.VetExternPrograms[j], 
								   &MyPrefRoom.VetExternPrograms[j+1], 
								   sizeof(StrRecordExternProgram));
						}
						MyPrefRoom.TotExternPrograms --;
						ShowElencoProgrammi(WindList,i);
						return true;





				}
			}
	}
	return false;

}
// visualizza dialog box per la modifica della lista dei programmi esterni da lanciare.
void ShowListaProgrammi(HWND hDialogo)
{

	DialogBox(MiaIstanzaDll,
				MAKEINTRESOURCE(IDD_DIALOG_LISTAPROGRAMMI), 
				hDialogo, &WindProcedureProgrammi);
	RimuoviMessaggiInput();
}

// mostra finestra trigger
void ShowFinestraTrigger(void)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	int RetValore;


	RetValore=DialogBox(MiaIstanzaDll,MAKEINTRESOURCE(IDD_DIALOG_TRIGGER), 
				*pMiaWind, &WindProcedureTriggers);
	RimuoviMessaggiInput();
	if (RetValore==1) roomMostraTrigger();

}

// verfica se zona di bound scelta esclude settori con oggetti
// in caso affermativo visualizza msgbox per chiedere se proseguire
// restituisce FALSE  se bisogna abortire
// restiuisce TRUE se si puo' continuare
BOOL ControllaBound(StrStanzaRoomEdit *pRoom)
{
	RECT *pRectSelezione2d = (RECT*) 0x583BC0;
	RECT Mio;
	bool TestErrore;
	int Indice;
	int i;
	int x,y;
	char *pMex;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	Mio.left = pRectSelezione2d->left - pRoom->XDisplayOffset;
	Mio.top = pRectSelezione2d->top - pRoom->ZDisaplyOffset;

	Mio.right = Mio.left + pRectSelezione2d->right -1;
	Mio.bottom = Mio.top + pRectSelezione2d->bottom -1;

	// ora dare errore se c'e' almeno un oggetto esterno alla zona di selezione
	TestErrore=false;
	for (i = 0;i<pRoom->N_Effects;i++) {
		Indice=pRoom->VetEffectCode[i];
		x = pVetBaseOggetti[Indice].XOrgBlock;
		y = pVetBaseOggetti[Indice].ZOrgBlock;

		if (x < Mio.left || x > Mio.right || 
			y < Mio.top || y > Mio.bottom ) {
			TestErrore=true;
			pMex = "effect";
			break;
		}
	}

	if (TestErrore==false) {
		// provare con obj and trigger
		for (i=0;i<pRoom->N_ObjAndTriggers;i++) {
			Indice=pRoom->VetObjAndTriggerCodes[i];
			x = pVetBaseOggetti[Indice].XOrgBlock;
			y = pVetBaseOggetti[Indice].ZOrgBlock;

			if (x < Mio.left || x > Mio.right || 
				y < Mio.top || y > Mio.bottom ) {
				TestErrore=true;
				pMex = "object or trigger";
				break;
			}
		}
	}			

	if (TestErrore==true) {
		sprintf(BufferLog,"WARNING: there is some %s in zone extern of current bound. Performing this operation you could get EVIL error mexages in your project.\r\nIt's better now you click on CANCEL button and remove objects extern to bounding zone before performing the BOUND command.\r\nAnyway if you want perform the bound click on OK button", pMex);
		if (MostraMessageBoxNGLE(BufferLog,0x0f)==0) return FALSE;
		
	}
	return TRUE;


}
// chiamata ogni volta che si sta per eliminare stanza IndiceRoom
// controlla se ci sono oggetti nell'eventuale stanza flippata
// se non bisogna eliminare stanza restituisce FALSE
// se tutto e' ok restituisce TRUE
BOOL ControllaDeleteRoom(int IndiceRoom)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int IndiceFlippata;
	char *pMex;

	pVetRooms = *p2VetRooms;
	
	IndiceFlippata=-1;
	if (pVetRooms[IndiceRoom].RoomButtons & 0x0002) {
		// stanza e' la fliproom
		IndiceFlippata=IndiceRoom;
	}else {
		// non e' flippata, vedere pero' se ha una stanza flippata
		if (pVetRooms[IndiceRoom].IndexFlipRoom !=-1) 
					IndiceFlippata = pVetRooms[IndiceRoom].IndexFlipRoom;
	}

	if (IndiceFlippata != -1) {
		// controllare che non ci siano oggetti
		if (pVetRooms[IndiceFlippata].N_Effects !=0 ||
			pVetRooms[IndiceFlippata].N_ObjAndTriggers != 0) {
			//bisogna impedire l'eliminazione
			if (IndiceFlippata == IndiceRoom) 
				pMex= "In current flipped room ";
			else
				pMex="Current room  has a flipped room where ";

			sprintf(BufferLog,"%s there are yet items (lights or statics). It's dangerous delete room in this situation because you could damage your project.\r\nIt's better you click on [CANCEL] button and remove items from flipped room before deleting it. Anyway if you want delete this room now, click on [OK] button", pMex);
			
			if (MostraMessageBoxNGLE(BufferLog,0x0f) == 0) return FALSE;
		}
		
	}

	return TRUE;
}
// eseguita all'avio del programma un attimo primo di entrare nel ciclo 
// dei messaggi
void CaricaOldProgetto(void)
{
	char *pNome;
	int i;


	if (MyPrefRoom.Flags & FPR_LOAD_LAST_PROJECT) {
		
		for (i=0;i<MyPrefRoom.TotOldProjects;i++) {
			pNome =	MyPrefRoom.VetOldProjects[i].NomeProject;
			if (EsisteFile(pNome)==true) {


				CaricaProgettoMio(pNome);
				break;
			}
		}
	}
}
// confrontando Ptr_TextureRawImage e Imageptr di corrente tail info
// e considerando il tipo di progetto attuale, restituisce il valore
// di sound texture 
int TrovaTextureSonora(int IndiceTail)
{
	int *Ptr2_TextureRawImage = (int *) 0x56C4F8;
	BYTE *pMemImage;
	int Differenza;
	int SizeTex;
	DWORD *pTestBigTexture = (DWORD *) 0x454ffc;

	int OrgX,OrgY;
	int Indice;
	BYTE **p2VetTexSonorePrj = (BYTE **) 0x45463E;
	BYTE *pVetTexSonore;
	int Valore;

	pVetTexSonore = *p2VetTexSonorePrj;


	pMemImage = VetBigTails[IndiceTail].pMemImage;

	pMemImage -= *Ptr2_TextureRawImage;
	Differenza = (int) pMemImage;

	if ((*pTestBigTexture) == 0)  
		SizeTex = 64;
	else
		SizeTex = 128;

	
	// trovare origine in pixel
	OrgY = Differenza / 256;
	OrgX = Differenza % 256;

	// ora convertirlo in grigli
	OrgX /= SizeTex;
	OrgY /= SizeTex;

	// e infine in indice assoluto texture
	Indice = OrgY * 256 / SizeTex + OrgX;
	

	Valore = pVetTexSonore[Indice];
	return Valore;


}
// chiamata dopo ogni refresh schermo completo
void RinfrescaSchermo(void)
{
	if (WindComboProgrammi != NULL) 
			RedrawWindow(WindComboProgrammi,NULL,NULL,RDW_INVALIDATE);

}
// chiamata alla inizio di FaiQualcosaConDoor per vedere se
// stanza attuale ha il pgridcollision
void ControllaGridRoomAttuale(void)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;

	InitTabellaGrid(&pVetRooms[*pIndiceRoomAttuale]);

}

// eseguire backup automatico se:
// TotCambiBackup >= valore preferenze per cambi
// Non siamo in preview, e nemmeno in flipmap mode
void AnalisiBackupAuto(void)
{
	DWORD *pTestPreview = (DWORD*) 0x583BF8;
	int *pTestFlipMap = (int *) 0x583BF0;
	
	if (*pTestPreview != 0 || *pTestFlipMap != 0) return;

	if (MyPrefRoom.AutoBackup == 0) return;
	if (TotCambiBackup >= MyPrefRoom.AutoBackup) {
		// eseguire backup automatico
		Sub_PulsanteBackup((StrInfoMouse*) 1);
		TotCambiBackup=0;
		
	}
}

// fare mio ciclo per messaggi a finestra button con peekmessage
void MioCicloMessaggi(void)
{
	MSG MioMsg;
	HACCEL *pAcc = (HACCEL *) 0x46D930;

	if (BaseWindow.TotButtons == 0 || BaseWindow.hWind == NULL) return;
	AggiornaButtonsWindow();

	while (PeekMessage(&MioMsg, BaseWindow.hWind, 0, 0, PM_REMOVE)) {
		TranslateAccelerator(BaseWindow.hWind, *pAcc, &MioMsg);
		DispatchMessage(&MioMsg);
		
	}
}
// pLastInput punta a valore di menu o pulsante  appena premuto.
// azzerando questo valore si ottiene il blocco dell'input
void AnalisiStandBy(int *pLastInput)
{
	DWORD TempoNow;


	MioCicloMessaggi();
	if (MyPrefRoom.Flags & FPR_NO_STANDBY) return;


	if ((*pLastInput) != 0) SalvaUltimoTempoInput();

	TempoNow = GetTickCount() - LastTimeStandBy;
	if (TempoNow > 2000) {
		AnalisiBackupAuto();
	}
	if (TempoNow > 500) {
		Sleep(100);
	}
}


// viene chiamata al termine di avvio programma quando e' in grado di
// riceevere comandi
void PrimaEsecuzione(void)
{
	PatchMaximizeNgle();

}
// solo per debug (non e' necessaria) controlla il nunmero di tail info
// da esportare secondo analisi di room editor
void AnalisiTailExport(short *pVetExportTail)
{

	int i;
	int TotExport;

	InviaLog("Export Tail Infos List:");
	TotExport=0;
	for (i=0;i<MAX_TAIL_INFOS;i++) {
		if (pVetExportTail[i] != -1) {
			sprintf(BufferLog,"\t%d) %d (0x%X)", 
					i, pVetExportTail[i], pVetExportTail[i]);
			InviaLog(BufferLog);
			TotExport++;
		}
	}
	sprintf(BufferLog,"Number of exported tail = %d (0x%X)", TotExport,TotExport);
	InviaLog(BufferLog);
}
// chiamata all'inizio di salvataggio project (anche autosave.prj) 
// usata per salvare nome progetto appena salvato
void SalvaProjectName(char *pNomeProject)
{
	strcpy(NomeLastSavedProject,pNomeProject);

}
// aggiunge i testi allo schermo
void AggiuntiTestoSchermo(void)
{
	ShowTestoSuSchermo(23,319,"Room Buttons");
}
// restituisce vettore con tutti i dati per ogni camera flyby
StrFlyByArrayRoomEditor* CreaListaFlyBy(int *pTotFlyBy)
{
	// FILE *pFile;
	static StrFlyByArrayRoomEditor pVetFlyBy[512];
	WORD ValWord;
	int i;
	int TotTrovati;
	WORD *VetTrovati;
	int Indice;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;

	pVetBaseOggetti = *P2_VetBaseOggetti;

	VetTrovati=TrovaEffetti(0x4040, &TotTrovati);

	// ora riempire tabella 
	*pTotFlyBy = 0;
	for (i=0;i<TotTrovati;i++) {
		Indice =VetTrovati[i];
		// trovare numero sequenza
		ValWord = pVetBaseOggetti[Indice].SlotIndex;

		ValWord >>= 13;
		ValWord &= 0x07;
		pVetFlyBy[i].Sequenza = (BYTE) ValWord;

		// ora trova Num
		ValWord = pVetBaseOggetti[Indice].SlotIndex;
		ValWord >>=8;
		ValWord &= 0x1F;

		pVetFlyBy[i].Number = (BYTE) ValWord;

		pVetFlyBy[i].ObjIndex = (WORD) Indice;
	}

	*pTotFlyBy = TotTrovati;

	return &pVetFlyBy[0];
		

	/*
	pFile = fopen("list_flyby.txt","wb");
	if (pFile==NULL) return;

	for (i=0;i<*pTotFlyBy;i++) {
		pFlyBy = &pVetFlyBy[i];

		fprintf(pFile,"FlyBy(%d) = Sequenza=%d  Num=%d\r\n",
			pFlyBy->ObjIndex, pFlyBy->Sequenza, pFlyBy->Number);
	}
	fclose(pFile);
	*/

}
// imposta in pTime la data di modifcia del file
// se c'e' qualche errore restituisce false
bool GetDataDelFileSys(char *NomeFile, SYSTEMTIME *pData)
{
	HANDLE MioFile;
	FILETIME DataTime;


	MioFile = CreateFile(NomeFile,GENERIC_READ, 
							FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (MioFile == INVALID_HANDLE_VALUE) return false;

	GetFileTime(MioFile,NULL,NULL, &DataTime);


	CloseHandle(MioFile);
	if (FileTimeToSystemTime(&DataTime, pData)==FALSE) return false;


	return true;

}
// restituisce true se pData e' minore di anno, mese, giorno forniti
bool DataMaggioreDi(SYSTEMTIME *pData, WORD Anno, WORD Mese, WORD Giorno)
{
	if (Anno > pData->wYear) return false;
	if (Anno < pData->wYear) return true;

	// anno e' uguale, ora vedere mese

	if (Mese > pData->wMonth) return false;
	if (Mese < pData->wMonth) return true;

	// mese e' uguale, ora controllare il giorno
	if (Giorno > pData->wDay) return false;

	return true;


}
// deve essere chiamata subito dopo aver caricato un progetto
void RiparaFlyBy(char *NomePrj)
{
	StrFlyByArrayRoomEditor *VetFlyBy;
	int TotFlyBy;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;
	int i;
	int j;
	int z;
	int ItemIndex;
	int IndiceTrigger;
	WORD ObjIndex;
	bool TestTriggerSpeciale;
	FILE *pFile;
	bool TestAvviso;
	bool TestTitle;
	char *pNome;
	static WORD VetFromVersion[4] = {1,2,0,1};
	char NomeFile[256];



	sprintf(NomeFile, "%s\\log_flyby.txt", MyTempFolder);


	// analisi per versione di progetto
	for (i=0;i<4;i++) {
		if (GlobNgle.VersioneProject.VetVersione[i] > VetFromVersion[i]) return;
		if (GlobNgle.VersioneProject.VetVersione[i] < VetFromVersion[i]) break;
	}
	// se tutto uguale non c'e bisogno di far econtrollo
	if (i == 4) return;


	// ora cercare di scoprire se questo e' un livello che sara' usato
	// per title
	pNome = SoloNome(NomePrj);
	TestTitle = false;
	if (InStr(0,pNome,"title") != -1) {
		TestTitle=true;
	}else {
		// il nome e' sbagliato ma ora provre a vedere nome
		// di file wad
		if (InStr(0,NomeFileWad, "title") != -1) TestTitle=true;
		
	}
		
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetTriggers = *P2_VetTriggers;
	pVetRooms = *p2VetRooms;
	VetFlyBy = CreaListaFlyBy(&TotFlyBy);
	remove(NomeFile);

	if (TotFlyBy==0) return;

	TestAvviso=false;

	pFile = fopen(NomeFile,"wb");

	for (i=0;i< *pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot==0) {
			TestTriggerSpeciale=false;
			for (j=0;j< pVetRooms[i].N_ObjAndTriggers;j++) {
				ItemIndex = pVetRooms[i].VetObjAndTriggerCodes[j];
				if (pVetBaseOggetti[ItemIndex].TypeObject== 0x10) {
					// e' un trigger
					IndiceTrigger = pVetBaseOggetti[ItemIndex].SlotIndex;

					if (pVetTriggers[IndiceTrigger].TriggerCosaAttivare == 0x0c) {

						// trovato un trigger per flyby camera
						ObjIndex = pVetTriggers[IndiceTrigger].OggettoDaTriggerare;
						// vedee se puo' essere un indice di camera
						for (z=0;z<TotFlyBy;z++) {
							if (VetFlyBy[z].ObjIndex == ObjIndex) break;
						}

						if (z < TotFlyBy) {
							
							// punta ad una singola camera flyby
							if (TestTitle==true) {
								// devo reimpostare solo il numero di sequenza
								pVetTriggers[IndiceTrigger].OggettoDaTriggerare = VetFlyBy[z].Sequenza;
							}else {

								// sostituirlo con un trigger object
								pVetTriggers[IndiceTrigger].TriggerCosaAttivare =0;
							}

							fprintf(pFile,"Corrected ex FlyBy Trigger in Room=\"%s\" Sector=(%d, %d)\r\n",
								pVetRooms[i].Nome, 
								pVetBaseOggetti[ItemIndex].XOrgBlock,
								pVetBaseOggetti[ItemIndex].ZOrgBlock);
							TestAvviso=true;
						}
					}
				}
			}
		}
	}
	fclose(pFile);

	if (TestAvviso==true) {
		MostraMessageBoxNGLE("WARNING: Current project has been changed, remember to save it.\r\nFound some damaged triggers to start flyby cameras. The triggers have been corrected anyway you should read the \"flyby_log.txt\" log file and then check them",3);
		ShellExecute(NULL,"open",NomeFile,"",SoloDir(NomeFile),SW_SHOW);
	}else {
		remove(NomeFile);
	}

}
// e' stato fatto un click.
// il pulsante pPulsante e' di tipo 0x32 (testo)
// verificare se ' il mouse e' nella zona giusta e gestire il testo
// con input box

void PulsanteNomeRoom(StrDatiPulsante *pPulsante, 
					 int OrgX, int OrgY, int SizeX, int SizeY)
{
	int *pIndiceRoomAttuale = (int*) 0x583BA0;
	char *pNomeOriginale;
	char *pChar;

	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;

	if (roomMouseInRettangolo(OrgX,OrgY,SizeX,SizeY) == 0) return;

	pNomeOriginale = pVetRooms[*pIndiceRoomAttuale].Nome;
	
	pChar = InputTextBox("Change name of current room", 
		"Room Name",pNomeOriginale,TP_CENTRALE);
	if (pChar == NULL) return;

	if (strlen(pChar) > 79) {
		MostraMessageBoxNGLE("Name too long: max characters 80",3);
		return;
	}


	TotCambiBackup++;
	strcpy(pVetRooms[*pIndiceRoomAttuale].Nome, pChar);
	// ora aggiornare i valori
	roomShowNomeRoom(pVetRooms[*pIndiceRoomAttuale].Nome, 
			pVetRooms[*pIndiceRoomAttuale].FlipMapValue);
}

// riceve in input record di stanza e record di oggetti (vetbaseoggetti)
// restituisce il valore di indice slot di questo oggetto
// e se l'indice e' sballato lo corregge e restituisce zero per 
// fare riferimento a lara
int ControllaSlotOggetto(StrStanzaRoomEdit *pRoom, StrItemsRoom *pOggetto)
{
	int *pTotOggettiWad = (int*) 0x56D9F0;
	int IndiceSlot;

	IndiceSlot= pOggetto->SlotIndex;
	
	if (IndiceSlot < 0 || IndiceSlot >= *pTotOggettiWad) {
		sprintf(BufferLog,"ERROR: in room %s there is an object in sector(%d,%d) that has a wrong slot index. To avoid crash the slot index will be changed with that of Lara slot",
			pRoom->Nome, pOggetto->XOrgBlock, pOggetto->ZOrgBlock);
		MostraMessageBoxNGLE(BufferLog,3);
		IndiceSlot=0;
		pOggetto->SlotIndex = IndiceSlot;
	}
	return IndiceSlot;

}
// se utente preme CTRL+SHIFT+ I crea un crash simulato
void SimulaCrash(void)
{



	if (TestAttivatoCrash == true) return;

	// SHIFT CTRL I
	if (TastoPremuto(VK_SHIFT)== true && 
		TastoPremuto(0x49)== true && 
		TastoPremuto(VK_CONTROL) == true) {
		
		// crea crash
		TestBloccoTRY=0; // disabilita ogni blocco try
		TestAttivatoCrash=true;
		InviaLog("SHIFT+CTRL+I : User requires a simulated crash");

		__asm {

			mov eax,2
			mov ecx, 17h
			mov byte ptr [eax][ecx], 4
		}
		
	}

}
// chiamata al termine di output wad
// salva i vettori di remap obj e room in variabile 
// globale SalvaVettoriRemap
// 
void CopiaVettoriRemap(void)
{
	int i;
	int *pTotRooms = (int*) 0x56C710;
	int *pTotZonaVetCamera = (int*) 0x5651A0;
	StrFlyByRoom **p2ZonaVetCamera = (StrFlyByRoom**) 0x454652;
	StrFlyByRoom *pZonaVetCamera;
	short Indice;

	pZonaVetCamera = *p2ZonaVetCamera;

	for (i=0;i<6000;i++) {
		SalvaVettoriRemap.VetObjRemap[i] = pGlobRemapObj[i];
	}
	// aggiungere informazioni per flyby e (forse) sink
	for (i=0;i<*pTotZonaVetCamera;i++) {
		Indice = pZonaVetCamera[i].IndiceNgle;

		if (SalvaVettoriRemap.VetObjRemap[Indice] == -1) {
			SalvaVettoriRemap.VetObjRemap[Indice] = 
						pZonaVetCamera[i].IndiceFlyBy;
		}
	}

	SalvaVettoriRemap.TotRooms = *pTotRooms;
	for (i=0;i<*pTotRooms;i++) {
		SalvaVettoriRemap.VetRoomRemap[i] = pGlobRemapRoom[i];
	}
	strcpy(SalvaVettoriRemap.NomeFileTom, GlobNomeTom);
	SalvaVettoriRemap.TestAttivo=true;
	
}

void GosubFineInputMouse(void)
{
	DWORD StartTempo;
	DWORD TempoNow;
	WORD *pPulsantiMouse = (WORD*) 0x46DA88;
	DWORD *pStatusPulsanti = (DWORD*) 0x46E0A4;

	if (BaseWindow.TestSospesa==true) {
		ShowWindow(BaseWindow.hWind, SW_SHOW);
		BaseWindow.TestSospesa=false;

	}
	if (BaseWindow.MioComandoInCorso == true) {
		// azzera
		*pPulsantiMouse=0;
		*pStatusPulsanti=0;
		return;
	}

	StartTempo = GetTickCount();
	do {
		__asm {
			pushad
			mov eax, 436CB7h    ;AltroCicloTutto
			call eax
			mov eax, 444990h    ;LeggePosizioneMouse
			call eax
			mov eax, 436AEEh    ;ElaboraQualcosaMouse
			call eax
			popad
		}
		SimulaCrash();
		TempoNow = GetTickCount()-StartTempo;

	} while (TempoNow < 1000 && (*pPulsantiMouse) != 0);

	// azzera
	*pPulsantiMouse=0;
	*pStatusPulsanti=0;

}
// chiamata in window animation range allo scopo di poter poi ordinare
// i ranges

void RiordinaRangeAnimati(void)
{

	// questi sono gli indici corretti

	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int TotIndici;
	int VetIndiciRange[256];
	int IndiceI, IndiceJ;
	int i,j;
	int Indice;
	int TotRange;

	// prima prendere i range effettivamente presenti
	TotIndici=0;

	for (i=0;i<40;i++) {
		Indice = i;
		if (VetAnimRange[Indice].TestUsato) {
			VetIndiciRange[TotIndici++] = Indice;
		}
	}
	// ora ordinarli

	TotRange = TotIndici;
	DatiTexture.TotLastSort=0;

	for (i=0;i< TotRange-1;i++) {
		for (j=i+1;j< TotRange;j++) {
			IndiceI = VetIndiciRange[i];
			IndiceJ = VetIndiciRange[j];

			if ((GlobNgle.RoomTexAnimate.VetInfoRangeAnim[IndiceI] & FAN_ANY_ROTATE) == 0 && 
				(GlobNgle.RoomTexAnimate.VetInfoRangeAnim[IndiceJ] & FAN_ANY_ROTATE)) {
				// il range I non ha UV range mentre quello J lo ha: invertire 
				// gli indici
				VetIndiciRange[i] = IndiceJ;
				VetIndiciRange[j] = IndiceI;
				break;
			}
		}
	}
	DatiTexture.TotLastSort  = TotRange;
	for (i=0;i<TotRange;i++) {
		DatiTexture.VetLastSorg[i] = VetIndiciRange[i];
	}

	

}
void OrdinaRangeAnimati(void)
{
	int i;
	int j;
	int IndiceI;
	int IndiceJ;
	int *pTotRangeExported = (int*) 0x568280;
	int *VetIndiciRange = (int*) 0x56C440;
	int TotRange;

	TotRange = *pTotRangeExported;
	DatiTexture.TotLastSort=0;

	for (i=0;i< TotRange-1;i++) {
		for (j=i+1;j< TotRange;j++) {
			IndiceI = VetIndiciRange[i];
			IndiceJ = VetIndiciRange[j];

			if ((GlobNgle.RoomTexAnimate.VetInfoRangeAnim[IndiceI] & FAN_ANY_ROTATE) == 0 && 
				(GlobNgle.RoomTexAnimate.VetInfoRangeAnim[IndiceJ] & FAN_ANY_ROTATE)) {
				// il range I non ha UV range mentre quello J lo ha: invertire 
				// gli indici
				VetIndiciRange[i] = IndiceJ;
				VetIndiciRange[j] = IndiceI;
				break;
			}
		}
	}
	DatiTexture.TotLastSort  = TotRange;
	for (i=0;i<TotRange;i++) {
		DatiTexture.VetLastSorg[i] = VetIndiciRange[i];
	}
}


// ptch chiamata con Jmp
// usata per correggere calcolo di numero d'ordine di texture
// basato sulla dimensione di default delle texture
__declspec(naked) void PatchDefaultSizeTex1(void)
{
	__asm {
		mov  cx, [ecx+06h] 
		mov  [esp+28h], cx  ; (LOCALE_ESP - 0xC) 
		
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 64
		jnz Altro1
Default64:
		shr  ax, 06h 
		shr  dx, 06h 
		mov ecx, 2  ;moltiplica per 4
		jmp Calcolo
Altro1:
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 128
		jnz Altro2
		shr ax, 07h
		shr dx, 07h
		mov ecx, 1  ;moltiplica per 2
		jmp Calcolo

Altro2:
		// se non c'e' l'header ng considerare dimensione di 64 x64
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 256
		jnz Default64
		shr ax, 08h
		shr dx, 08h
		// non moltiplicare
		mov ecx, 0
Calcolo:
		mov  [esp+14h], esi  ; (LOCALE_ESP - 0x20) 
		shl eax, cl
		lea  eax, [edx][eax]


		// torna al codice tom2pc
		mov ecx, 404579h  ;  Continua
		jmp ecx
 


	}
}

// ptch chiamata con Jmp
// usata per correggere calcolo di numero d'ordine di texture
// basato sulla dimensione di default delle texture
__declspec(naked) void PatchDefaultSizeTex2(void)
{
	__asm {

	and  ecx, 800000ffh 
	jns  Salto1 
	dec  ecx 
	or   ecx, 0ffffff00h 
	inc  ecx 
Salto1: 
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 64
		jnz Altro1
Default64:
	shr  cx, 06h
	shr  ax, 06h 		
	lea  eax, [ecx][eax*4] 
	jmp Fine
Altro1:
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 128
		jnz Altro2
		// 128 x 128 
	shr  cx, 7h
	shr  ax, 7h 		
	lea  eax, [ecx][eax*2] 
	jmp Fine
Altro2:
		// se non c'e' l'header ng considerare dimensione di 64 x64
		cmp word ptr [GlobNgle.RoomTexAnimate.SizeDefault], 256
		jnz Default64
		// deve essere 256 x 256
	shr cx, 8h
	shr ax, 8h
	lea eax, [ecx][eax]
	
Fine:
		// torna al codice tom2pc
		mov ecx, 4045E8h ; Continua2
		jmp ecx
 


	}
}

void StampaLogTom2Pc(char *pMex)
{
	FILE *pFile;

	pFile = fopen(GetFileCrash("log_tom2pc.txt"), "a+t");
	fprintf(pFile, "%s\r\n", pMex);
	fclose(pFile);

}

// chiamata da tom2pc subito dopo aver concluso creazione
// file tr4.
// aggiunge l'eventuale extrangheader al file .tr4

void SalvaNGHeaderInTr4(char *NomeTr4)
{
	WORD *pVetExtra=NULL;
	int NWords;
	int i;
	StrParseNGField  ParseField;
	static int *pTotRangeExported = (int*) 0x568280;
	static int *VetIndiciRange = (int*) 0x56C440;
	static DWORD *pVersioneWad = (DWORD *) 0x624570;
	DWORD SizeMem;
	WORD TotBytes;
	WORD NumeroWords;

	if (LastNGHeader.Result < 1 || LastNGHeader.NWords == 0) return;
	if (EsisteFile(NomeTr4)==false) {
		sprintf(BufferLog,"ERROR: cann't find %s file. Cann't add Extra NG Header", 
					NomeTr4);
		InviaLog(BufferLog);
		StampaLogTom2Pc(BufferLog);
		return;
	}
	
	// dato che salva in tr4 vuol dire che siamo in tom2pc
	NWords =0;

	// ------ copia l'intero header ng in file tr4 ------

	if (LastNGHeader.Result < 1) return;
	// siamo in ng2tom, se versione wad e' 0x82
	// scandire ngheader per trovare ngtag NGTAG_FLAG_LEVEL_TR4
	// e salvari flag FLT_EXTRA_SOUND_TABLE
	if (*pVersioneWad == 0x82) {
		i=0;
		while (ParseNgField(LastNGHeader.pNGArray  ,i,&ParseField)) {
			
			// analizzare tipo di pacchetto
			switch (ParseField.Type) {
			case NGTAG_FLAG_LEVEL_TR4:
				ParseField.pData[0] |= FLT_EXTRA_SOUND_TABLE;
				StampaLogTom2Pc("Found ExtraSoundTable (Global SFX Map)");
				break;
			}


			// puntare a chunk successivo
			i = ParseField.NextIndex;
		}

	}

	NWords = LastNGHeader.NWords-2;
	SizeMem = NWords * 2;

	pVetExtra = (WORD*) malloc(SizeMem);

	for (i=0;i<NWords;i++) {
		pVetExtra[i] = LastNGHeader.pNGArray[i];
	}
	// ------- aggiungere vettore remap tail
	TotBytes = sizeof(StrRemapTailInfo) * GlobNgle.BaseRemapTail.TotTails;
	sprintf(BufferLog, "Number of remapped tails = %d", GlobNgle.BaseRemapTail.TotTails);
	StampaLogTom2Pc(BufferLog);

	NumeroWords = 3 + TotBytes / 2;
	SizeMem += NumeroWords * 2;

	pVetExtra = (WORD *) realloc(pVetExtra, SizeMem);

	pVetExtra[NWords++] = NumeroWords;
	pVetExtra[NWords++] = NGTAG_REMAP_TAILS;
	pVetExtra[NWords++] = GlobNgle.BaseRemapTail.TotTails;

	memcpy(&pVetExtra[NWords], 
			GlobNgle.BaseRemapTail.VetRemapTail, TotBytes);
	NWords += TotBytes / 2;


	// aggiungere per tutti gli header il tag di versione
	sprintf(BufferLog, "Set header version: %d.%d.%d.%d",
		GlobNgle.BaseVersione.VetVersione[0],
		GlobNgle.BaseVersione.VetVersione[1],
		GlobNgle.BaseVersione.VetVersione[2],
		GlobNgle.BaseVersione.VetVersione[3]);
	StampaLogTom2Pc(BufferLog);
	
	CopiaInHeader(NGTAG_VERSION_HEADER, NO_ARRAY,
				sizeof(StrVersionHeader), &GlobNgle.BaseVersione,
				&pVetExtra, &NWords);
	
	// aggiungere (se necessario) tabella conversione plugin id (dopo utilizzo di meta cabinet)
	if (BaseMeta.BaseConvertID.TotConvert > 0) {
		CopiaInHeader( NGTAG_REMAP_PLUGIN_IDS, BaseMeta.BaseConvertID.TotConvert, sizeof(StrTripleteId), 
			&BaseMeta.BaseConvertID.VetConvert[0], &pVetExtra, &NWords);
	}

	if (WriteNGHeader(NomeTr4,pVetExtra,NWords)==false) {

		sprintf(BufferLog,"ERROR: Trying to save extra ng header, cann't find or open file: %s", NomeTr4);
		InviaLog(BufferLog);


	}
	if (pVetExtra !=NULL) free(pVetExtra);


	sprintf(BufferLog, "Added Extra NG Header to %s", NomeTr4);
	StampaLogTom2Pc(BufferLog);
	InviaLog(BufferLog);
	// salvare sempre file di testo NG_Tom2Pc_Last_Log.txt
	ShowStatistichePC(NULL, false);


}
// verifica che nome sound sample NomeSound sia prswene
// se non lo e' da messaggio d'errore e poi sostituisce 
// il nome col sound sample "back_jm1.wav"
void VerficaSoundSample(char *pNomeSound)
{
	if (EsisteFile(pNomeSound)==false) {
		sprintf(BufferLog,"*** File Not Found \"%s\" replaced with \"back_jm1.wav\"", 
				SoloNome(pNomeSound));
		InviaLog(BufferLog);
		StampaLogTom2Pc(BufferLog);

		sprintf(BufferLog, "%s\\back_jm1.wav", SoloDir(pNomeSound));
		strcpy(pNomeSound,BufferLog);
	}

}


// scopre la dimensione standard delle tail info appena
// caricate da file .tom
void TrovaSizeTexDefault(StrTailInfoTom *pVetTail, int TotTail)
{
	int i;
	WORD Size;

	for (i=0;i<TotTail;i++) {
		if (pVetTail[i].SizeX == pVetTail[i].SizeY) {
			Size = pVetTail[i].SizeX;
			GlobNgle.RoomTexAnimate.SizeDefault = Size+1;
			sprintf(BufferLog, "SizeDefaultTexture = %d", Size+1);
			StampaLogTom2Pc(BufferLog);
			return;
		}
	}
		

}
// eax= indice tom
// ecx =indice tr4
__declspec(naked) void PatchRemapTailQuad(void)
{
	__asm {
		push edx

		movzx edx, word ptr [GlobNgle.BaseRemapTail.TotTails]
		// se e' attivo output di nuovi range (frame)
		cmp byte ptr [TestCreaNewAnimRanges], 0
		jz NormaleTail
		// allora salvare in campo di indice tom
		// l'attuale indice di range che sta creando questa texture
		push ebx
		mov bx, word ptr [IndiceTomTailSource]
		or bx, 400h
		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + edx * 4], bx
		pop ebx
		jmp  Salta
		
NormaleTail:
		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + edx * 4], ax
Salta:

		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + 2 + edx * 4], cx
		inc word ptr [GlobNgle.BaseRemapTail.TotTails]
		// estrarre qui il valorfe di bump map
		// usando la memoria espansa per questa zona di memoria
		mov edx, 406BD8h // indirizzo con memoria espansa
		mov edx, dword ptr[edx]
		mov al, [edx][eax]

		pop edx

		retn
 

	}

}





// viene chiamata quando wad ha versione diversa da 0x81
// visualiza il messaggio nel log, ma soprattutto salva 
// nell'header ng il flag per segnalare che il file .tr4
// e' di quelli con tabella estesa
void GestionePCTabellaSuoniEstesa(int VersioneWad)
{
	
	sprintf(BufferLog,"Detected Wad Version %d (0x%X) (Global Sound Samples Table)", 
		VersioneWad, VersioneWad);
	StampaLogTom2Pc(BufferLog);
	InviaLog(BufferLog);



}

// chiamata un attimo prima di salvare NUvRanges di texture animate
// se per questo file .tom c'era un extra headr forza il numero previsto
// altrimenti, lo lascia com'e'
// usata da tom2pc
void ForzaUvRotate(void)
{
	BYTE *pTotUvRange = (BYTE*) 0x421068;

	if (GlobNgle.RoomTexAnimate.TestPresente == true) {
		*pTotUvRange = GlobNgle.RoomTexAnimate.Tot_UV_Rotate;
	}


}


void ControllaCryptCheckBox(HWND hDialogo)
{
	NumeroBloccoCrypt =0;
	TestAttivaCryptTr4 = IsDlgButtonChecked(hDialogo, 1021);

}

// chiamata all'inizio di WriteAnimTextures in tom2pc
__declspec(naked) void PatchAttivaCreaRanges(void)
{
	__asm {
		mov byte ptr [TestCreaNewAnimRanges], 1
		mov ebp, 423180h    ;TotAnimRanges
		mov  ebp, dword ptr [ebp] 
		xor  edx, edx 
		retn

	}
}

// estrae header da file tom, e salva i dati in GlobNgle

void LeggiHeaderNGFileTom(char *pNomeFile)
{

	int i;
	DWORD Size;
	StrParseNGField  ParseField;
	StrExtractNG ExtractNG;
	int j;	;
	int Numero;

	//e' file .tom
	// visuaizzare in log anche la versione dll attuale
	sprintf(BufferLog,"Version: Tomb_NextGeneration.dll (v.%s)", MexVersione);
	InviaLog(BufferLog);
	

	GlobNgle.RoomTexAnimate.TestPresente=false;
	// qui imposta a 64 dimensioned i default ma in realta', in tom2pc
	// verra' poi calcolata la dimensione standard delle tail
	// sulla base delle prima tail infos lette da file .tom
	GlobNgle.RoomTexAnimate.SizeDefault = 64;
	GlobNgle.RoomTexAnimate.TotaleRangeNG=0;
	GlobNgle.RoomTexAnimate.Tot_UV_Rotate=0;

	for (i=0;i<40;i++) {
		GlobNgle.RoomTexAnimate.VetInfoRangeAnim[i] =0;
	}


	ExtractNG.pNGArray=NULL;
	if ( ExtractNGHeader(pNomeFile,&ExtractNG)==false) {

		switch (ExtractNG.Result ) {
		case -2:

			sprintf(BufferLog,"ERROR: cann't open file: \"%s\" (to read extra ng header)",SoloNome(pNomeFile));
		
			InviaLog(BufferLog);

			return;
		case -1:

			InviaLog("WARNING: Extra NG Header appears corrupted");

			return;
		case 0:

			InviaLog("Current file has no Extra NG Header");

			return;
		default:
			sprintf(BufferLog,"UNKNOWN ERROR trying to read NG header from file: %s",pNomeFile);
			InviaLog(BufferLog);
			return;
		}
	}

	InviaLog("Found Extra NG Header");

	// scandire dati, saltando prima word di controllo "NG"
	i=0;
	while (ParseNgField(ExtractNG.pNGArray ,i,&ParseField)) {
		
		// analizzare tipo di pacchetto
		switch (ParseField.Type) {
		case NGTAG_PLUGIN_ID_FLOOR_TABLE:
			// salvare tabella plugin in dati per conversione 
			j=0;
			 
			BaseMeta.BaseConvertID.PluginTableOld.TotTable = ParseField.pData[j++];
			Numero= BaseMeta.BaseConvertID.PluginTableOld.TotTable;

			if (Numero > 0) {
				if (Numero > 0x10000) {
					sprintf(BufferLog, "ERROR: PluginIdFloorTabel, read from .tom file, is wider than max size available table size = %d (max size=65536)",
						Numero);
					InviaLog(BufferLog);
					Numero= 0x10000;
					BaseMeta.BaseConvertID.PluginTableOld.TotTable= Numero;
				}
				memcpy(&BaseMeta.BaseConvertID.PluginTableOld.VetTable[0], &ParseField.pData[j], Numero );

			}

			
			break;
		case NGTAG_ANIMATED_TEXTURES:
			
			Size= sizeof(StrDatiExtraAnimazioni);
			if (Size == ParseField.SizeData) {
				StampaLogTom2Pc("Tom File: found AnimatedTexture (regular format)");

				// vettore di 40 elementi con info per animazioni
				memcpy(&GlobNgle.RoomTexAnimate, ParseField.pData, 
						sizeof(StrDatiExtraAnimazioni));
				GlobNgle.RoomTexAnimate.TestPresente=true;
			}else {
				memset(&GlobNgle.RoomTexAnimate,0, 
						sizeof(StrDatiExtraAnimazioni));
				StampaLogTom2Pc("Tom File: WARNING, found AnimatedTexture with old (irregular) format. Clear animated textures data");
			}


			break;

		case NGTAG_TOM_VERSION:
			Size = sizeof(StrVersionHeader);
			memcpy(&GlobNgle.VersioneProject, ParseField.pData,Size);
			// visualizzare nel log la vesione tom
			sprintf(BufferLog,"TOM Version: v.%d.%d.%d.%d", 
					GlobNgle.VersioneProject.VetVersione[0], 
					GlobNgle.VersioneProject.VetVersione[1], 
					GlobNgle.VersioneProject.VetVersione[2], 
					GlobNgle.VersioneProject.VetVersione[3]);
			StampaLogTom2Pc(BufferLog);
			InviaLog(BufferLog);

			break;
		case NGTAG_PLUGIN_NAMES_NGLE:
			j=0;
			GlobNgle.TotPluginNgle= ParseField.pData[j++];
			if (GlobNgle.TotPluginNgle > 0) {
				memcpy(&GlobNgle.VetPluginNgle[0], &ParseField.pData[j], sizeof(StrPluginNames) * GlobNgle.TotPluginNgle);
				sprintf(BufferLog, "PLUGINS: current level used %d plugins:", GlobNgle.TotPluginNgle);
				StampaLogTom2Pc(BufferLog);
				InviaLog(BufferLog);

				for (j=0;j<GlobNgle.TotPluginNgle; j++) {
					sprintf(BufferLog, "    \"%s.dll\" (%d triggers)",
						GlobNgle.VetPluginNgle[j].Nome, GlobNgle.VetPluginNgle[j].TotUsato);
					StampaLogTom2Pc(BufferLog);
					InviaLog(BufferLog);
				}
			}
			break;


		}
		// puntare a chunk successivo
		i = ParseField.NextIndex;
	}
	free(ExtractNG.pNGArray );
	ExtractNG.pNGArray=NULL;

}

// patch TOM2PC per salvare corrispondenza tra indice tail di tom e di tr4
// ebx = indice tom
// ecx = indice tr4
__declspec(naked) void PatchRemapTailTriangolo(void)
{
	__asm {
		push eax

		movzx eax, word ptr [GlobNgle.BaseRemapTail.TotTails]
		// se e' attivo output di nuovi range (frame)
		cmp byte ptr [TestCreaNewAnimRanges], 0
		jz NormaleTail
		// allora salvare in campo di indice tom
		// l'attuale indice di range che sta creando questa texture
		push ebx
		mov bx, word ptr [IndiceTomTailSource]
		or bx, 400h
		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + eax * 4], bx
		pop ebx
		jmp  Salta
		
NormaleTail:
		// aggiungere 0x1000 per segnalare triangolo
		push ebx
		or bx, 1000h

		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + eax * 4], bx
		pop ebx
Salta:
		mov word ptr [GlobNgle.BaseRemapTail.VetRemapTail + 2 + eax * 4], cx
		inc word ptr [GlobNgle.BaseRemapTail.TotTails]

		mov eax, 406BD8h // indirizzo con memoria espansa per vetbumpmap
		mov eax, dword ptr[eax]
		mov bl, [ebx][eax]

		pop eax

		retn

		

	}
}

// patch per TOM2PC
void AzzeraTempMemTex(int TotUsedPcTexture, BYTE *pStartMem)
{
	int SizeMem;


	if (pStartMem != NULL) {
		SizeMem = GetSizeMemTempTexture();
		memset(pStartMem,0,SizeMem);
	}
}


// azzera memoria MemTextureAllocTable in tom2pc
// in eax c'e' memoria appena allocata
__declspec(naked) void ClearMemoryTempTex(void)
{
	__asm {
		pushad 

		mov ebx, 626584h   ;MemTextureAllocTable
		mov  dword ptr [ebx], eax
		push	eax  ;base memoria

		mov eax, 419840h    ;TotUsedPcTexture
		mov  eax, dword ptr [eax]
		push	eax  ;numer TotUsedPcTexture
		lea		eax, AzzeraTempMemTex
		call	eax
		add		esp,8

		popad
		test eax, eax
		jz		NoMem
		clc
		retn
NoMem:
		stc
		retn
	}
}

// restituisce dimensione di memoria MemTextureAllocTable tom2pc
DWORD GetSizeMemTempTexture(void)
{
	DWORD RetValore;

	__asm {
		mov eax, 419840h    ;TotUsedPcTexture
		mov  eax, dword ptr [eax]
		lea  eax, [eax+03h][eax*2]
		lea  eax, [eax][eax*4]
		shl  eax, 12h
		push  ebx
		lea		ebx, RetValore
		mov		dword ptr [ebx], eax
		pop  ebx
	}
	return RetValore;

}
__declspec(naked) void PatchOffsetFrammenti1(void)
{
	__asm {
	movzx ebp, word ptr [GlobNgle.RoomTexAnimate.SizeDefault]
	cmp ebp, 0
	jnz pupo
	mov ebp, 64
pupo:
	sub ebp,1
	mov  cl, [esp+14h]  ; (LOCALE_ESP - 0x20) 
	and  ecx, ebp
	jnz  Nextloop_ScanFrammenti 
	mov  ecx, [esp+20h]  ; (LOCALE_ESP - 0x14) 
	and  ecx, ebp
	xor  ebp, ebp 
	mov  bp, [esi+04h] 
	push 404651h ; DopoFramm1
	retn
Nextloop_ScanFrammenti:
	push 404715h  ; Nextloop_ScanFrammenti
	retn
 
	}
}

__declspec(naked) void PatchOffsetFrammenti2(void)
{
	__asm {
	movzx ebp, word ptr [GlobNgle.RoomTexAnimate.SizeDefault]
	cmp ebp, 0
	jnz pupo
	mov ebp, 64
pupo:	
	sub ebp,1
	and  edx, ebp  ;qui secondo me c'e' l'inghippo 
	mov  [eax+ 421984h], dx ;ZonaFrammentiAnimTex + 04h 
	mov  edx, [esp+20h]  ; (LOCALE_ESP - 0x14) 
	and  edx, ebp
	push 4047ACh  ;  DopoFramm2
	retn
 
	}
}



// usata in tom2pc solo per debug (si direbbe)
// indicetail = indice tail di file tom, + evetnuali flag (usare maschera 0x3ff)
// TipoTex :  1 = triangolo / 2 = quad
void AnalisiFindTexAnim(WORD IndiceTail, WORD TipoTex)
{
	StrTexInfoPc **p2MemRoomTail  = (StrTexInfoPc**) 0x41E1CC;
	StrTexInfoPc *pVetRoomTail;
	BYTE *pVetTotTexRange = (BYTE *) 0x421068;
	StrFrammentiTex *pVetFrammenti = (StrFrammentiTex*) 0x421980;
	int TotTex;
	StrTexInfoPc *pTail;
	StrFrammentiTex *pFram;
	int i;

	pVetRoomTail = *p2MemRoomTail;

	TotTex = *pVetTotTexRange;
	pTail = &pVetRoomTail[IndiceTail & 0x3ff];

	sprintf(BufferLog,"IndiceTail=0x%X  TipoTex=%d",
		IndiceTail, TipoTex);
	InviaLog(BufferLog);

	sprintf(BufferLog,"\tOrgX=%d  OrgY=%d  SizeX=0x%X  SizeY=0x%X",
		pTail->OrgX, pTail->OrgY, pTail->SizeX, pTail->SizeY);
	InviaLog(BufferLog);

	// se ci sono frammenti visualizzare i dati
	if (TotTex) {
		sprintf(BufferLog,"\tFrammenti=%d", TotTex);
		InviaLog(BufferLog);

		for (i=0;i<TotTex;i++) {
			pFram = &pVetFrammenti[i];
			sprintf(BufferLog,"\t\t0x%04X   0x%04X  %d  %d  0x%X  0x%X",
				pFram->Attribute, pFram->Flag, 
				pFram->OrgX, pFram->OrgY, 
				pFram->SizeX, pFram->SizeY);
			InviaLog(BufferLog);
		}
	}


}

// chiamata per ogni texture trovata per vedere se fa parte di qualche
// range di texture animate
__declspec(naked) void FindAnimTex(void)
{
	__asm {
		mov eax, dword ptr [esp+4] // Indice tail
		mov ecx, dword ptr [esp+8] // tipo texture 1=tria, 2=quad
		pushad
		push  ecx  // tipo tex
		push eax // indice tail
		call AnalisiFindTexAnim
		add esp,8

		popad

		sub  esp, 24h	; INIZIO PROCEDURA 47 ;  SIZE=0x2F0 
		mov ecx, 41E1CCh    ;Memoria_RoomTailInfos
		mov  ecx, dword ptr [ecx] 
		push ebx 
		mov  ebx, [esp+2ch]  ; (ARG_ESP + 0x4)  // indice tail da analizzare 
		mov eax, 40451Eh
		jmp eax
 
	}

}

// se c=1 salta a fine procedura
__declspec(naked) void PatchBugTriangolo(void)
{
	__asm {
		mov  cx, [esi-02h]  ;attribute 
		mov  edi, ebx 
		mov  ebp, ecx 
		and  edi, 0fbffh 
		and  ebp, 0ffffh 
		and  ebp, 0fffffbffh 
		xor  edi, ebp 
		jz   Esci
		clc
		retn
Esci:
		stc
		retn
	}

}
// riceve id di pulsante premuto in tom2pc
void GestionePulsantiPC(HWND hDialogo, int IdPulsante)
{
	

	switch (IdPulsante) {
	case 1020:
		// mostrare statistiche
		ShowStatistichePC(hDialogo, true);
		break;
	case 1021:
		// gestione check box Crypt
		TestCryptTr4 ^= 1;
		CheckDlgButton(hDialogo,1021,TestCryptTr4);
		break;
	case 1009:
	case 1010:
		// premuto uno dei due pulsanti build
		//  creeazione
		// di patch autentiche che palle pero', c'e' poco spazio per nascondere
		GosubFasullaPoppa(IdPulsante);
		break;

	}

}

void SalvaNomeFileTom(char *pNomeTom)
{
	strcpy(BaseMeta.NomeFileTom, pNomeTom);
}
// carica geometria (non compressa) da file .lev

bool CaricaGeometriaLev(char *pNomeLev, Cab_GeometryTr4 *pGeo)
{

	FILE *pFile;
	DWORD SizeFile;
	DWORD Offset;

	SizeFile=DimensioneFile(pNomeLev);
	pFile=fopen(pNomeLev, "rb");
	if (pFile==NULL) {
		sprintf(BufferLog, "ERROR: cann't opening \"%s\" file", pNomeLev);
		InviaLog(BufferLog);
		return false;
	}

	// impostare offset da dove inizia geometria
	Offset = BaseMeta.OffsetGeoLev;

	fseek(pFile, Offset, SEEK_SET);

	// ok, ora qua inizia geometria
	BaseTr4Zl.SizeFile = SizeFile;
	BaseTr4Zl.pFile = pFile;
	BaseTr4Zl.SizeUncompressed = SizeFile-Offset;
	
	if (CaricaGeometria(pGeo)==false) return false;
	fclose(pFile);
	return true;
}
// chiamata da tom2pc all'inizio di Reprocess
// verifica se ci sono le condizoni per usare file metacab
// se si, restituire "true"

bool VerificaMetaCab(char *pNomeLev)
{
	char *pNome;
	long Size;
	int i;
	char Buffer[4096];
	Cab_GeometryTr4 *pGeoMeta;
	Cab_GeometryTr4 *pGeo;
	Cab_RoomTr4 *pRoomMeta;
	Cab_RoomTr4 *pRoom;
	DWORD FlagsMeta;	
	DWORD FlagsCab;
	int TotErrori;
	int NRooms;

	// inizializzare basemeta
	BaseMeta.TestUsaMetaCab=false;
	strcpy(BaseMeta.NomeFileLev, pNomeLev);

	// trovare nome di file .mtcab
	pNome = CambiaEstensione(BaseMeta.NomeFileTom, ".mtcab");
	strcpy(BaseMeta.NomeFileCab, pNome);
	// azzerare tabella conversione plugin id
	BaseMeta.BaseConvertID.TotConvert=0;

	// vedere se esiste un file mtcab
	if (EsisteFile(pNome)==false) return false;
	InviaLog("Detected MetaCab File");

	// ora caricare tutto
	// prima allocare memoria globale facendo somma tra dimensione file .lev e quella di metacab 
	Size = DimensioneFile(BaseMeta.NomeFileCab);
	Size += DimensioneFile(BaseMeta.NomeFileLev);
	// nel dubbio aumentare anxcora un po' 
	Size += 100000;

	// allocare memoria globale
	AllocaMiaMemoriaGlobale(Size);
	sprintf(BufferLog, "   Parsing \"%s\" file in progress...", SoloNome(BaseMeta.NomeFileCab));
	InviaLog(BufferLog);

	if (CaricaCabinet(BaseMeta.NomeFileCab, &BaseMeta.BaseCabinet)==false) {
		InviaLog("   Failed loading metacab");
		return false;
	}
	sprintf(BufferLog, "   Parsing \"%s\" file in progress...", SoloNome(BaseMeta.NomeFileLev));
	InviaLog(BufferLog);	
	// caricare geometria in file .lev
	if (CaricaGeometriaLev(BaseMeta.NomeFileLev, &BaseMeta.GeometryLevel)==false) {
		InviaLog("   Failed loading current level geometry");
		return false;
	}

	// ora fare super controllo per vedere se sono compatibili le stanze
	Buffer[0]=0;
	pGeo = &BaseMeta.GeometryLevel;
	pGeoMeta = &BaseMeta.BaseCabinet.MetaLevel.Geometry;
	FlagsMeta = BaseMeta.BaseCabinet.Header.FlagsMETA;
	FlagsCab = BaseMeta.BaseCabinet.Header.FlagsCAB;
	TotErrori=0;
	
	NRooms = pGeoMeta->N_Rooms;

	if (pGeo->N_Rooms != pGeoMeta->N_Rooms) {
		sprintf(BufferLog, "   Different Amount of Rooms (Now=%d  Cabinet=%d)\r\n", pGeo->N_Rooms, pGeoMeta->N_Rooms);
		InviaLog(BufferLog);
		strcat(Buffer,BufferLog);
		TotErrori++;
		if (FlagsCab & CAB_ON_ERROR_SKIP) {
			InviaLog("   SKIPPED meta cabinet usage");
			return false;
		}
		// impostare come numero room quello piu' piccolo
		if (pGeo->N_Rooms < NRooms) NRooms=pGeo->N_Rooms;
	}

	// controllo per room 
	pRoom = &pGeo->pVetRooms[0];
	pRoomMeta = &pGeoMeta->pVetRooms[0];

	for (i=0;i<NRooms;i++) {
		if (pRoom->OrgX != pRoomMeta->OrgX ||
			pRoom->OrgZ != pRoomMeta->OrgZ ) {

			sprintf(BufferLog, "   Changed position of room %d\r\n", i);
			InviaLog(BufferLog);
			strcat(Buffer,BufferLog);
			TotErrori++;
			if (FlagsCab & CAB_ON_ERROR_SKIP) {
				InviaLog("   SKIPPED meta cabinet usage");
				return false;
			}
		}

		if (pRoom->XSizeSectors != pRoomMeta->XSizeSectors ||
			pRoom->ZSizeSectors != pRoomMeta->ZSizeSectors ||
			pRoom->OrgYTop  != pRoomMeta->OrgYTop ||
			pRoom->OrgYBottom != pRoomMeta->OrgYBottom) {

			sprintf(BufferLog, "   Changed size of room %d\r\n", i);
			InviaLog(BufferLog);
			strcat(Buffer,BufferLog);
			TotErrori++;
			if (FlagsCab & CAB_ON_ERROR_SKIP) {
				InviaLog("   SKIPPED meta cabinet usage");
				return false;
			}

		}			

		if (pRoom->NDoors != pRoomMeta->NDoors) {
			sprintf(BufferLog, "   Changed amount of doors (portals) in room %d\r\n", i);
			InviaLog(BufferLog);
			strcat(Buffer,BufferLog);
			TotErrori++;
			if (FlagsCab & CAB_ON_ERROR_SKIP) {
				InviaLog("   SKIPPED meta cabinet usage");
				return false;
			}

		}	
		
		// aumentare room pointers
		pRoom++;
		pRoomMeta++;
	}

	if (TotErrori > 0) {
		if (FlagsCab & CAB_ON_ERROR_MSGBOX) {
			// visualizzare msgbox e chiedere che fare
			sprintf(BufferLog, "Found uncompatibility issues in mtcab file.\r\nDo you want to skip the importing of metacab?\r\n\r\nFound %d problems:\r\n%s",
				TotErrori, Buffer);

			if (MessageBox(NULL, BufferLog, "NG Tom2pc - Cabinet Importing",MB_YESNO) == IDYES) return false;
		}
	}

	// ora qui dovrei fare il controllo per vedere se si puo' usare metacab
	BaseMeta.TestUsaMetaCab=true;
	return true;
}
// pTr4->GeometrySizeCompress = ComprimiZl(pMem, pOut, pTr4->GeometrySizeUncompress);
DWORD ComprimiZL(byte *pMem, byte *pOut, DWORD SizeUncompress)
{
	DWORD SizeCompressa;

	__asm{
		pushad
		mov eax, dword ptr [SizeUncompress]
		push eax // dimensione non compressa

		mov eax, dword ptr [pOut]
		push eax  // buffer output

		mov eax, dword ptr [pMem]
		push eax   // buffer con dati da comprimere

		mov eax, 4065A0h    ;deflate
		call eax	; \TOM2PC.004065A0 	
		add esp, 0ch
		mov dword ptr [SizeCompressa], eax
		popad
	}
	return SizeCompressa;
}

// aggiorna vecchi e nuovi indici a basefloor usando i dati in pVetEstratti[] TotEstratti
// sapendo che il primo di quei valori verra' scritto apartire da IndiceFromFloor di nuova tabella floor
void AggiornaPluginId(int IndiceFromFloor, StrFloorEstratti *pVetEstratti, int TotEstratti)
{
	StrTablePluginIdFloor *pTab;
	StrFloorConvert *pConv;
	int i;
	int IndiceOld;
	int IndiceNew;


	pTab = &BaseMeta.BaseConvertID.PluginTableOld;
	pConv = &BaseMeta.BaseConvertID;

	// salvare i dati dei vari trigger SOLO se corrispondevano ad un plugin (con id diverso da zero)
	for (i=0;i<TotEstratti;i++) {
		IndiceOld = pVetEstratti[i].IndexFloorOld;
		IndiceNew = IndiceFromFloor+i;

		if (IndiceOld < pTab->TotTable) {
			if (pTab->VetTable[IndiceOld] != 0) {
				// trovato un id plugin
				// aggiungere tripletta di conversione
				pConv->VetConvert[pConv->TotConvert].IdPlugin = pTab->VetTable[IndiceOld];
				pConv->VetConvert[pConv->TotConvert].IndexFloorNow = IndiceNew;
				pConv->VetConvert[pConv->TotConvert].IndexFloorOld = IndiceOld;
				pConv->TotConvert++;
			}
		}

	}

	
}
// restituisce testo multilinea, con tutti i flag attualmente attivi
char * GetDescriptionMetaFlags(DWORD Flags)
{
	static char *pVetNomeFlag[] = {"META_DOORS", "META_ROOM_COLLISIONS", "META_STATIC_ITEMS", "META_LIGHTS", 
									"META_CAMERAS", "META_MESH_OBIECTS", "META_STATIC_SLOTS", "META_MOVEABLE_SLOTS",
									"META_FLYBY_CAMERA", "META_ANIM_TEXTURE", "META_ANIM_SLOT", "META_BOX_ZONES_OVERLAPS",
									"META_SPRITE_TAILS_AND_SEQUENCE", "META_ROOM_POSITION", NULL};
	static char Buffer[4096];
	DWORD ValNow;
	int i;

	Buffer[0]=0;

	i =0;
	ValNow=1;
	while (pVetNomeFlag[i]) {
		if (Flags & ValNow) {

			strcat(Buffer, pVetNomeFlag[i]);
			strcat(Buffer, "\r\n");
		}
		ValNow = ValNow << 1;
		i++;
	}
	return Buffer;

}
// usa come valore finale quello di livello attuale, e aggiunge (e aggiorna) dati floor, vetitems ecc.
// se pFile non e' NULL, allora bisogna fare il log
void CreaMixedCabLevel(FILE *pFile)
{
	Cab_GeometryTr4 *pGeo;
	Cab_GeometryTr4 *pGeoMeta;
	int TotImportTrigger;
	int TotImportCollision;

	int i;
	Cab_RoomTr4 *pRoom;
	Cab_RoomTr4 *pRoomMeta;
	int NRooms;
	DWORD Flags;
	int j;
	WORD *pVetFloor;
	WORD TotFloor;
	int IndiceFloor;
	int IndiceFloorMeta;
	int x,z;
	Cab_CollisionSectorTR4 *pSector;
	Cab_CollisionSectorTR4 *pSectorMeta;
	int TotTriggers;
	int TotCollision;
	StrFloorEstratti VetTriggers[1000];
	StrFloorEstratti *pEstratti;
	char *pChar;
	bool TestTogli;
	int k;
	int NSectors;
	DWORD FlagsCAB;

	Flags = BaseMeta.BaseCabinet.Header.FlagsMETA;
	FlagsCAB = BaseMeta.BaseCabinet.Header.FlagsCAB;

	BaseMeta.BaseConvertID.TotConvert=0;

	pGeo = &BaseMeta.GeometryLevel;
	pGeoMeta = &BaseMeta.BaseCabinet.MetaLevel.Geometry;

	// soprattutto per floor data sara' un casino.
	NRooms=pGeo->N_Rooms;
	if (NRooms > pGeoMeta->N_Rooms) NRooms=pGeoMeta->N_Rooms;

	pRoom = pGeo->pVetRooms;
	pRoomMeta = pGeoMeta->pVetRooms;

	if (pFile) {
		fprintf(pFile, "---------------------------------------------- Mixing Geometry for %s level\r\n", 
			SoloNomeSenzaExt(BaseMeta.NomeFileLev));
		fprintf(pFile, "Rooms: MetaLevel=%d CurrentLevel=%d (it will be mixed %d rooms)\r\n", 
							pGeoMeta->N_Rooms, pGeo->N_Rooms, NRooms);
		fprintf(pFile, "ACTIVE IMPORTING FLAGS:\r\n");
		fprintf(pFile, "-----------------------\r\n");
		pChar = GetDescriptionMetaFlags(Flags);
		fprintf(pFile,pChar);
		fprintf(pFile, "-----------------------\r\n\r\n");
	}
	// se e' richiesto importing anche di vetfloor inizializzare il vettore che ospitera' il nuovo Vetfloor
	if (Flags & META_ROOM_COLLISIONS) {
		if (pFile) {
			fprintf(pFile, "Importing Collisions (META_ROOM_COLLISIONS flag): rewrite FloorData\r\n");
		}
		TotFloor=0;
		// dimensionarlo a valore arbitrariamnete grande
		// il doppio del massimo valore tra i due livelli
		j= pGeo->FloorData.TotFloorData;
		if (j < pGeoMeta->FloorData.TotFloorData) j =pGeoMeta->FloorData.TotFloorData;

		j *=2;
		
		pVetFloor = (WORD *) malloc(j *2);
		if (pFile) {
			fprintf(pFile, "malloc() required buffer (0x%X address) of %d words to host new floordata\r\n", pVetFloor, j);
		}
		// mettere il valore iniziale di default 0
		pVetFloor[TotFloor++]=0;
	}
		

	for (i=0;i<NRooms;i++) {
		if (pFile) {
			fprintf(pFile, "************* ROOM %d ****************\r\n", i);
		}
		// ----------------------------------- ROOM POSITION ?
		if (Flags & META_ROOM_POSITION) {
			pRoom->OrgX = pRoomMeta->OrgX;
			pRoom->OrgYBottom = pRoomMeta->OrgYBottom;
			pRoom->OrgYTop = pRoomMeta->OrgYTop;
			pRoom->OrgZ = pRoomMeta->OrgZ;
		}
		// ------------------------------------ STATIC DATA (sempre)
		if (pFile) {
			fprintf(pFile, "\tIMPORTED Static Data: MetaLevelSize=%d CurrentLevelSize=%d\r\n", 
				pRoomMeta->StaticDataWords, pRoom->StaticDataWords);
		}
		// copiare i room static data (sempre)
		pRoom->StaticDataWords = pRoomMeta->StaticDataWords;
		pRoom->pStaticData = pRoomMeta->pStaticData;

		// ----------------------------------- DOORS ? 
		if (Flags & META_DOORS) {
			if (pFile) {
				fprintf(pFile, "\tIMPORTED Doors (portals): MetaLevelDoors=%d CurrentLevelDoors=%d\r\n",
					pRoomMeta->NDoors, pRoom->NDoors);
			}
		
			pRoom->NDoors = pRoomMeta->NDoors;
			pRoom->pVetDoors = pRoomMeta->pVetDoors;

		}
		// ---------------------------------- COLLISION ?
		if (Flags & META_ROOM_COLLISIONS) {
			// lavoraccio: ricreare l'intera zona floor, prelevando, settore per settore
			// tutti i dati
			// dal file meta prelevare quelli collisione
			// dal file ngle solo quelli tipo trigger
			// nota: se la dimensione delle due stanze e' diversa copiare integralmente quelle di ngle

			if (pRoom->XSizeSectors != pRoomMeta->XSizeSectors ||
				pRoom->ZSizeSectors != pRoomMeta->ZSizeSectors) {
				if (pFile) {
					fprintf(pFile, "\tWARNING: different size in sectors! (Meta: %d x %d / LevelNow: %d x %d)\r\n",
						pRoomMeta->XSizeSectors, pRoomMeta->ZSizeSectors,
						pRoom->XSizeSectors, pRoom->ZSizeSectors);
				}

				pSector = pRoom->pCollisionSectorGrid;

				for (z=0;z<pRoom->ZSizeSectors;z++) {
					for (x=0;x<pRoom->XSizeSectors;x++) {
			
						IndiceFloor=pSector->FloorDataIndex;

						if (IndiceFloor!=0) {
							// (nel caso sia 0 non modifcare niente)
							// il nuovo indicefloor per questo settore sara' l'attuale TotFloor
							pSector->FloorDataIndex = TotFloor;

							// estrarre prima i trigger senza togliere flag finale (per vedre se ci sono trigger)
							TestTogli=false;

							pEstratti= EstraiFloorData(&pGeo->FloorData, IndiceFloor, true, false, &TotTriggers);
							if (TotTriggers > 0) {
								memcpy(VetTriggers, pEstratti, TotTriggers * sizeof(StrFloorEstratti));
								// visto che ci sono trigger, sara' questa serie a creare la fine serie
								// per cui bisgnera' toglier l'eventuale fine serie da collision
								TestTogli=true;
							}

							// estrarre collision
							pEstratti = EstraiFloorData(&pGeo->FloorData, IndiceFloor, false, TestTogli, &TotCollision);

							// ora copiare entrambi i gruppi file

							for (k=0;k<TotCollision;k++) {
								pVetFloor[TotFloor++] = pEstratti[k].ValTrigger;
							}

							AggiornaPluginId(TotFloor, VetTriggers, TotTriggers);
							for (k=0;k<TotTriggers;k++) {
								pVetFloor[TotFloor++] = VetTriggers[k].ValTrigger;
							}

						}
						pSector++;
					}
				}
			}else 
			{
				// le due stanze hanno stessa dimensione
				// per cui fare trasferimento super figo
				pSector = pRoom->pCollisionSectorGrid;
				pSectorMeta = pRoomMeta->pCollisionSectorGrid;
				TotImportCollision=0;
				TotImportTrigger=0;
				for (z=0;z<pRoom->ZSizeSectors;z++) {
					for (x=0;x<pRoom->XSizeSectors;x++) {

						// prendere roba trigger
						IndiceFloor=pSector->FloorDataIndex;
						IndiceFloorMeta = pSectorMeta->FloorDataIndex;

						// copiare gli altri dati di settore
						if (Flags & META_BOX_ZONES_OVERLAPS) {
							pSector->BoxZoneIndex = pSectorMeta->BoxZoneIndex;
						}
						if (Flags & META_DOORS) {
							pSector->AboveRoomIndex = pSectorMeta->AboveRoomIndex;
							pSector->BelowRoomIndex = pSectorMeta->BelowRoomIndex;
						}
						// altezza collsione floor e ceiling, si, perche' gia'
						// c'e' il flag META_ROOM_COLLISIONS attivo
						pSector->FloorHeight = pSectorMeta->FloorHeight;
						pSector->CeilingHeight = pSectorMeta->CeilingHeight;

						if (IndiceFloor!=0 || IndiceFloorMeta!=0) {
							// (nel caso siano  0 entrambi non modifcare niente)
							// il nuovo indicefloor per questo settore sara' l'attuale TotFloor
							pSector->FloorDataIndex = TotFloor;

							// estrarre prima i trigger senza togliere flag finale (per vedre se ci sono trigger)
							TestTogli=false;
							TotTriggers=0;
							if (IndiceFloor != 0) {
								pEstratti= EstraiFloorData(&pGeo->FloorData, IndiceFloor, true, false, &TotTriggers);
								if (TotTriggers > 0) {
									memcpy(VetTriggers, pEstratti, TotTriggers * sizeof(StrFloorEstratti));
									// visto che ci sono trigger, sara' questa serie a creare la fine serie
									// per cui bisgnera' toglier l'eventuale fine serie da collision
									TestTogli=true;
									TotImportTrigger += TotTriggers;
								}
							}
							TotCollision=0;
							if (IndiceFloorMeta != 0) {

								// estrarre collision da stanza meta cabinet
								pEstratti = EstraiFloorData(&pGeoMeta->FloorData, IndiceFloorMeta, false, TestTogli, &TotCollision);
								TotImportCollision += TotCollision;
							}

							// ora copiare entrambi i gruppi file
							for (k=0;k<TotCollision;k++) {
								pVetFloor[TotFloor++] = pEstratti[k].ValTrigger;
							}
							AggiornaPluginId(TotFloor, VetTriggers, TotTriggers);
							for (k=0;k<TotTriggers;k++) {
								pVetFloor[TotFloor++] = VetTriggers[k].ValTrigger;
							}

							if ((TotCollision+TotTriggers)==0) {
								// se non c'e' niente allora rimettere 0 come indice floor
								pSector->FloorDataIndex =0;
							}

						}
						pSector++;
						pSectorMeta++;
					}
				}
				if (pFile) {
					fprintf(pFile, "\tIMPORTED FloorData: CollisionDataFromMeta=%d words / PreservedTriggersFromCurrentLevel=%d words\r\n",
							TotImportCollision, TotImportTrigger);
				}
			}

		} else {
			// attenzione; META_ROOM_COLLISIONS non e' attivo, pero' se fosse attivo un flag riguardo a box 
			// o doors, dovro' comunque scandire tutti i settori per copiare per copiare i valorid di indice a box
			// e di room sopra o sotto
			// (a patto pero' che le due stanze abbiano stessa dimensione)
			if (pRoom->XSizeSectors == pRoomMeta->XSizeSectors &&
				pRoom->ZSizeSectors == pRoomMeta->ZSizeSectors &&
				(Flags & (META_DOORS | META_BOX_ZONES_OVERLAPS) != 0)) {
				// ok, scandire in sequenza tutti i settori
				pSector = pRoom->pCollisionSectorGrid;
				pSectorMeta = pRoomMeta->pCollisionSectorGrid;
				NSectors = pRoom->XSizeSectors * pRoom->ZSizeSectors;
				for (k=0;k<NSectors;k++) {
					if (Flags & META_DOORS) {
						pSector->AboveRoomIndex = pSectorMeta->AboveRoomIndex;
						pSector->BelowRoomIndex = pSectorMeta->BelowRoomIndex;
					}
					if (Flags & META_BOX_ZONES_OVERLAPS) {
						pSector->BoxZoneIndex = pSectorMeta->BoxZoneIndex;
					}
					// passare a prossimo settore
					pSector++;
					pSectorMeta++;
				}

			}

		}
		// -------------------------- LIGHTS ?
		if (Flags & META_LIGHTS) {
			if (pFile) {
				if (pRoom->NLights != pRoomMeta->NLights) {
					fprintf(pFile, "\tIMPORTED Lights: Forced the %d Lights form Meta, replaced the %d Lights of current level\r\n",
						pRoomMeta->NLights, pRoom->NLights);
				}
			}
			pRoom->LightIntensity = pRoomMeta->LightIntensity;

			pRoom->NLights = pRoomMeta->NLights;
			pRoom->pVetLights = pRoomMeta->pVetLights;

		}

		// --------------------------- STATIC ITEMS ?
		if (Flags & META_STATIC_ITEMS) {
			if (pFile) {
				if (pRoom->NStaticMesh != pRoomMeta->NStaticMesh) {
					fprintf(pFile, "\tIMPORTED StaticItem: Forced the %d StaticItem form Meta, replaced the %d StaticItems of current level\r\n",
						pRoomMeta->NStaticMesh, pRoom->NStaticMesh);
				}
			}
			pRoom->NStaticMesh = pRoomMeta->NStaticMesh;
			pRoom->pVetStaticMesh = pRoomMeta->pVetStaticMesh;
		}

		// ----- passa a room successiva
		pRoom++;
		pRoomMeta++;
	}

	if (pFile) {
		fprintf(pFile, "\r\n");
	}

	// -------------------------- SOSTITUIRE FLOOR DATA?
	if (Flags & META_ROOM_COLLISIONS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED Mixed FloorData: MixedFloorData=%d words / CurrentLevel=%d words\r\n",
				TotFloor, pGeo->FloorData.TotFloorData);
		}
		pGeo->FloorData.TotFloorData = TotFloor;
		pGeo->FloorData.pVetFloorData = pVetFloor;
	}
	// --------------------------- OBJECT MESH ?
	if (Flags & META_MESH_OBIECTS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED ObjectMeshMeta=%d words NPointers=%d / CurrentLevel=%d words NPointers=%d\r\n",
				pGeoMeta->ObjectMeshNWords, pGeoMeta->N_MeshPointer, pGeo->ObjectMeshNWords, pGeo->N_MeshPointer);
		}
		pGeo->ObjectMeshNWords = pGeoMeta->ObjectMeshNWords;
		pGeo->pObjectMesh = pGeoMeta->pObjectMesh;

		pGeo->N_MeshPointer = pGeoMeta->N_MeshPointer;
		pGeo->pVetMeshPointer = pGeoMeta->pVetMeshPointer;
	}

	// -------------------------- ANIMATION DATA?
	if (Flags & META_ANIM_SLOT) {
		if (pFile) {
			fprintf(pFile, "IMPORTED All Animation Data: Animations %d, AnimCommands %d, StateChanges %d, Dispatchers %d, MeshTrees %d, Frames %d\r\n",
				pGeoMeta->AnimationData.NAnimation,
				pGeoMeta->AnimationData.NAnimCommand,
				pGeoMeta->AnimationData.N_StateChanges, 
				pGeoMeta->AnimationData.NAnimDispatch,
				pGeoMeta->AnimationData.NMeshTree,
				pGeoMeta->AnimationData.NFrames);
		}

		pGeo->AnimationData = pGeoMeta->AnimationData;
	}
	// -------------------------- MOVEABLE SLOTS?
	if (Flags & META_MOVEABLE_SLOTS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED MoveableSlots: MetaSlots=%d / CurrentLevelSlots=%d\r\n",
				pGeoMeta->NSlotMoveables, pGeo->NSlotMoveables);
		}

		pGeo->NSlotMoveables = pGeoMeta->NSlotMoveables;
		pGeo->pVetSlotMoveables = pGeoMeta->pVetSlotMoveables;
	}

	// -------------------------- STATIC SLOTS?
	if (Flags & META_STATIC_SLOTS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED StaticSlots: MetaStaticSlots=%d / CurrentLevelStaticSlots=%d\r\n",
				pGeoMeta->NStaticObjects , pGeo->NStaticObjects);
		}
		pGeo->NStaticObjects = pGeoMeta->NStaticObjects;
		pGeo->pVetStaticObjects = pGeoMeta->pVetStaticObjects;
	}
	
	// ------------------------- SPRITE TEXTURE ?
	if (Flags & META_SPRITE_TAILS_AND_SEQUENCE) {
		if (pFile) {
			fprintf(pFile, "IMPORTED SpriteTexTailsAndSequences: MetaTails=%d  MetaSequences=%d/ CurrentLevelTails=%d CurrentLevelSequences=%d\r\n",
				pGeoMeta->NSpriteTex , pGeoMeta->NSpriteSeq  , pGeo->NStaticObjects, pGeo->NSpriteSeq );
		}
		pGeo->NSpriteTex = pGeoMeta->NSpriteTex;
		pGeo->pVetSpriteTex = pGeoMeta->pVetSpriteTex;
		pGeo->NSpriteSeq = pGeoMeta->NSpriteSeq;
		pGeo->pVetSpriteSeq = pGeoMeta->pVetSpriteSeq;
	}

	// ------------------------- CAMERAS ?
	if (Flags & META_CAMERAS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED Cameras: MetaCameras=%d / CurrentLevelCameras=%d\r\n",
				pGeoMeta->NCameras, pGeo->NCameras);
		}
		pGeo->NCameras = pGeoMeta->NCameras;
		pGeo->pVetCameras = pGeoMeta->pVetCameras;
	}

	// ------------------------- FLYBY CAMERA ?
	if (Flags & META_FLYBY_CAMERA) {
		if (pFile) {
			fprintf(pFile, "IMPORTED FlyByCameras: MetaFlyBy=%d / CurrentLevelFlyBy=%d\r\n",
				pGeoMeta->NFlyBy , pGeo->NFlyBy );
		}
		pGeo->NFlyBy = pGeoMeta->NFlyBy;
		pGeo->pVetFlyBy = pGeoMeta->pVetFlyBy;
	}

	// -------------------------- BOX ZONES AND OVERLAPS ?
	if (Flags & META_BOX_ZONES_OVERLAPS) {
		if (pFile) {
			fprintf(pFile, "IMPORTED BoxesAndOverlaps: MetaBoxes=%d MetaOverlaps=%d / CurrentLevelBoxes=%d CurrentLevelOverlaps=%d\r\n",
				pGeoMeta->NBox, pGeoMeta->NOverlaps,
				pGeo->NBox, pGeo->NOverlaps);
		}
		pGeo->NBox = pGeoMeta->NBox;
		pGeo->pVetBox = pGeoMeta->pVetBox;
		pGeo->NOverlaps = pGeoMeta->NOverlaps;
		pGeo->pVetOverlaps = pGeoMeta->pVetOverlaps;
		pGeo->pVetZones = pGeoMeta->pVetZones;

	}
	// -------------------------- ANIMATED TEXTURES?
	if (Flags & META_ANIM_TEXTURE) {
		if (pFile) {
			fprintf(pFile, "IMPORTED TexAnimatedData: MetaTexAnimated = %d words / CurrentLevelTexAnimated = %d words\r\n",  
				pGeoMeta->DataAnimWords, pGeo->DataAnimWords);
		}
		pGeo->DataAnimWords = pGeoMeta->DataAnimWords;
		pGeo->pVetAnimWords = pGeoMeta->pVetAnimWords;
		pGeo->NUvRange = pGeoMeta->NUvRange;
	}
	// ------------------------ TEX INFO TAILS (SEMPRE)
	if (pFile) {
		fprintf(pFile, "IMPORTED TexTailInfos: MetaTails=%d / CurrentLevelTails=%d\r\n",
			pGeoMeta->NTexInfo, pGeo->NTexInfo);
	}
	pGeo->NTexInfo = pGeoMeta->NTexInfo;
	pGeo->pVetTexInfo = pGeoMeta->pVetTexInfo;

	if (pFile) {
		fprintf(pFile, "---------------------------------------------- Completed mixing of Geometry\r\n");

	}
}
void IncrementoBloccoCrypt(void)
{
	char Buffer[20];
	int Alfa;
	int Beta;

	Alfa= 0;
	__asm {
		pushad
		mov ecx, 4
		mov dword ptr [Beta], ecx
		mov eax, 0
		lea edi, Buffer
		rep stosb 
		mov ecx, 6
		mov al, byte ptr [Alfa]
		add al, byte ptr [Beta]
		rep stosb 
		lea ebx, NumeroBloccoCrypt+5380h
		mov dword ptr [Alfa], ebx
		xor ecx, ebx
		sub ebx, 5380h
		add byte ptr [ebx],1
		mov dword ptr [Beta], ecx
		popad

	}
	
}
// ora posso creare file tr4 , tutto fino a geometry, poi il resto lo fara' il resto del codice
void CreaMainFileTr4(void)
{
	char NomeTr4[256];
	char *pChar;
	FILE *pFile;
	Cab_BaseTr4 *pTr4;
	char NomeGeoNonCompressa[256];
	char NomeGeoCompressa[256];
	char NomeLog[256];
	FILE *pFileOut;
	BYTE *pMem;
	BYTE *pOut;
	DWORD FlagsCAB;
	FILE *pFileLog;
	char *pNome;
	char *pDir;

	pChar = CambiaEstensione(BaseMeta.NomeFileLev, ".tr4");
	sprintf(BufferLog, "Building %s level in progress...", SoloNome(pChar));
	InviaLog(BufferLog);

	strcpy(NomeTr4, pChar);
	// salvare anche in variabile globale NomeTr4Pc perche' verra' usata per aggiungere extra ng header
	strcpy(NomeTr4Pc, pChar);

	FlagsCAB = BaseMeta.BaseCabinet.Header.FlagsCAB;
	pFileLog=NULL;
	
	// vedere se bisogna creare log
	if (FlagsCAB & CAB_CREATE_LOG_FILE) {
		pNome = SoloNomeSenzaExt(BaseMeta.NomeFileLev);
		pDir = SoloDir(BaseMeta.NomeFileLev);
		sprintf(NomeLog, "%s\\log_%s.txt", pDir, pNome);

		remove(NomeLog);
		pFileLog = fopen(NomeLog, "ab");

		fprintf(pFileLog, "MIXING META CAB FILES LOG\r\n");
		fprintf(pFileLog, "-------------------------\r\n");
		fprintf(pFileLog, "Meta Cabinet File: %s\r\n", BaseMeta.NomeFileCab);
		fprintf(pFileLog, "Target Tr4 File  : %s\r\n", NomeTr4);
		fprintf(pFileLog, "\r\n");
	
	}
	// ora si deve creare (in cabinet) il mix tra i due livelli
	CreaMixedCabLevel(pFileLog);

	// poi lo si registra

	pFile = fopen(NomeTr4, "wb");

	BaseTr4Zl.pFile=pFile;

	
	// scrivere file .tr4

	
	pTr4= &BaseMeta.BaseCabinet.MetaLevel;

	// marcatore TR4
	if (TestAttivaCryptTr4) {
		pTr4->MexTr4[3]='c';
	}
	fwrite(pTr4->MexTr4, 4,1,pFile);
	pTr4->MexTr4[3]=0;

	// Numero pagine texture
	
	fwrite(&pTr4->NRoomTexPages,2,1,pFile);
	fwrite(&pTr4->NObjTexPages,2,1,pFile);
	fwrite(&pTr4->NRoomBumPages,2,1,pFile);
	
	// mappa texture1
	fwrite(&pTr4->Tex1SizeUncompress,4,1,pFile);
	fwrite(&pTr4->Tex1SizeCompress,4,1,pFile);
	if (TestAttivaCryptTr4) {
		IncrementoBloccoCrypt();
		CriptaZona(pTr4->pTex1Bytes, pTr4->Tex1SizeCompress);
	}
	fwrite(pTr4->pTex1Bytes, pTr4->Tex1SizeCompress, 1,pFile);
	if (pFileLog) {

		fprintf(pFileLog, "NRoomTexPages=%d\r\n", pTr4->NRoomTexPages);
		fprintf(pFileLog, "NObjTexPages =%d\r\n", pTr4->NObjTexPages);
		fprintf(pFileLog, "NBumpMapPages=%d\r\n", pTr4->NRoomBumPages);
		fprintf(pFileLog, "Tex1 Size = %d\r\n", pTr4->Tex1SizeUncompress);


	}
	// mappa texture2
	fwrite(&pTr4->Tex2SizeUncompress,4,1,pFile);
	fwrite(&pTr4->Tex2SizeCompress,4,1,pFile);
	if (TestAttivaCryptTr4) {
		IncrementoBloccoCrypt();
		CriptaZona(pTr4->pTex2Bytes, pTr4->Tex2SizeCompress);
	}
	fwrite(pTr4->pTex2Bytes, pTr4->Tex2SizeCompress, 1,pFile);
	if (pFileLog) {
		fprintf(pFileLog, "Tex2 Size = %d\r\n", pTr4->Tex2SizeUncompress);
	}
	// mappa texture3
	fwrite(&pTr4->Tex3SizeUncompress,4,1,pFile);
	fwrite(&pTr4->Tex3SizeCompress,4,1,pFile);
	if (TestAttivaCryptTr4) {
		IncrementoBloccoCrypt();
		CriptaZona(pTr4->pTex3Bytes, pTr4->Tex3SizeCompress);
	}
	fwrite(pTr4->pTex3Bytes, pTr4->Tex3SizeCompress, 1,pFile);	
	if (pFileLog) {
		fprintf(pFileLog, "Tex3 Size = %d\r\n", pTr4->Tex3SizeUncompress);
	}
	fclose(pFile);

	// -------------- geometry ----------------------
	// ora grande casino:
	// la devo scrivere tutta in un file e poi lo comprimo
	pChar =CambiaEstensione(BaseMeta.NomeFileLev, ".geo");
	strcpy(NomeGeoNonCompressa, pChar);

	pChar = CambiaEstensione(BaseMeta.NomeFileLev, ".zlgeo");
	strcpy(NomeGeoCompressa, pChar);

	remove(NomeGeoNonCompressa);
	remove(NomeGeoCompressa);

	pFileOut=fopen(NomeGeoNonCompressa,"wb");

	BaseTr4Zl.pFile = pFileOut;
	if (SalvaGeometria(&BaseMeta.GeometryLevel ) ==false) {
		InviaLog("ERROR trying to save geometry in tr4 file");
		return;
	}
	fclose(pFileOut);

	pTr4->GeometrySizeUncompress = DimensioneFile(NomeGeoNonCompressa);

	// chiedere memoria per i due blocchi di memoria

	pMem= (BYTE *) CaricaFile(NomeGeoNonCompressa);
	pOut = (BYTE *) malloc(pTr4->GeometrySizeUncompress);

	pTr4->GeometrySizeCompress = ComprimiZL(pMem, pOut, pTr4->GeometrySizeUncompress);
	if (TestAttivaCryptTr4) {
		IncrementoBloccoCrypt();
		CriptaZona(pOut, pTr4->GeometrySizeCompress);
	}

	// perfetto ora registrare tutto nel file tr4
	pFile = fopen(NomeTr4, "ab");
	
	fwrite(&pTr4->GeometrySizeUncompress, 4,1,pFile);
	fwrite(&pTr4->GeometrySizeCompress,4,1,pFile);
	fwrite(pOut, pTr4->GeometrySizeCompress,1,pFile);
	if (pFileLog) {
		fprintf(pFileLog, "Geometry Size = %d\r\n", pTr4->GeometrySizeUncompress);
	}
	fclose(pFile);
	remove(NomeGeoNonCompressa);
	remove(NomeGeoCompressa);

	free(pOut);
	free(pMem);
	if (pFileLog) {
		fprintf(pFileLog,"Completed Tr4 Creation\r\n");
		fclose(pFileLog);
		pFileLog=NULL;
	}

}
int ResultMeta1;

void ConvertiAbsAnimDispatch(StrAnimDispatch *pVetDispatch, int TotDispatch)
{
	int i;
	WORD NextAnim;
	for (i=0;i<TotDispatch;i++) {
		NextAnim = pVetDispatch[i].Animation;
		if ((NextAnim & MASK_ASB_ANIM)== FLAG_ABS_ANIM) {
			// trovato un valore nextanim in formato abs
			// togliere il flag
			NextAnim &= FLAG_ABS_ANIM-1;
			pVetDispatch[i].Animation  = NextAnim;
		}
	}
}

// sequenza di patch generiche per tom2pc 
__declspec(naked) void PatchGenerichePc(void)
{
	__asm {
		cmp  ax,0
		jnz		Patch2
		;effettua controllo sulle zone di  memoria per le texture al termine
		;dell'elaborazione

		;codice originale eliminato da patch
		push	esi
		mov eax, 40B6F0h    ;fCloseFile
		call eax
		add		esp,4
		mov edx, 420330h    ;MemRoomTextureFlags
		mov  edx, dword ptr [edx]
		retn
Patch2:
		cmp		ax,1
		jnz		Patch3
		;chiamata da gestione messaggi per rilevare quando viene premuto
		;un pulsante.
		;in edx c'e' il numero di id -1002
		;resituisce c=1 se il numero di id e' maggiore di quelli previsti
		pushad
		add		edx, 1002
		push	edx
		mov ecx, 423E8Ch    ;HandleDialogWindow
		mov  ecx, dword ptr [ecx]
		push	ecx

		lea		eax, GestionePulsantiPC
		call	eax
		add		esp,8

		popad
		mov		eax,edx
		cmp		eax, 0x0c
		ja		IgnoraPulsante
		clc
		retn
IgnoraPulsante:
		stc
		retn

Patch3:
		cmp		ax,2
		jnz		Patch4



		;codice eliminato da patch
		mov ecx, 626584h    ;MemTextureAllocTable
		mov  ecx, dword ptr [ecx]
		push ecx
		mov eax, 40BBBFh    ;LiberaBloccoMemoria
		call eax
		add		esp,4

		retn
Patch4:
		cmp		ax,3
		jnz		Patch5
		// 
		// chiamata alla fine di creazione di file .tr4
		// quando file .tr4 e' gia' stato chiuso
		pushad
		// vecchia patch, ora uso un altro punto per salvare nome.tr4
		// salva extra header (se presente)
		// in file tr4 e qui salva il nome ma ancora non lo usa
		push  offset NomeTr4Pc
		call SalvaNGHeaderInTr4
		add  esp,4
	
		// ora si puo' liberare memoria globale per metacab
		call LiberaMiaMemoriaGlobale

		// questa e' patch fasulla per cryptare file tr4
		// e' sempre presnete ma e' fatta in modo 
		// da non danneggiare quella auentica
		push offset NomeTr4Pc
		call  CryptaFasullaTr4
		add esp,4
		popad
		;codice eliminato
		mov edx, 872828h    ;MemAllocSpaceTexture
		mov  edx, dword ptr [edx]
		push edx
		mov eax, 40BBBFh    ;LiberaBloccoMemoria
		call eax
		add		esp,4

		retn

Patch5:
		cmp		ax,4
		jnz		Patch6
		// patch per leggere extra header di file .tom
		pushad
		// salva nome file .tom in BaseMeta
		mov		eax, 6725C0h  ; NomeFileTom
		push	eax
		call SalvaNomeFileTom
		add esp,4
		popad

		pushad

		push  0
		mov		eax, 6725C0h  ; NomeFileTom
		push	eax
		lea		eax,  LeggiHeaderNGFileTom
		call	eax
		add		esp,8
		mov word ptr [GlobNgle.BaseRemapTail.TotTails], 0
		mov byte ptr [TestCreaNewAnimRanges], 0

		// vedere se e' attivo o meno checkbox di "crypt tr4"
		mov ecx, 423E8Ch    ;HandleDialogWindow
		mov  ecx, dword ptr [ecx]
		push ecx  ;hDialogo
		call   ControllaCryptCheckBox
		add  esp,4
		
		popad
	;codice che era stato eliminato da patch

		mov eax, 42026Ch   ;LOC_42026c
		mov  dword ptr [eax], ebx

		mov eax, 41E1D0h   ;Memoria_RoomImageTextures
		mov  dword ptr [eax], ebx
		retn

Patch6:
		cmp		ax,5
		jnz		Patch7
		// patch per forzare nuovo numero di range textture animate uvrotate
		pushad
		lea		eax, ForzaUvRotate
		call	eax
		popad
		// salvataggio valore (zona eliminata da patch)
		push ebx
		push 01h
		push 01h
		push  421068h ;TotUVRanges
		mov eax, 40B8C8h    ;fWrite_FunzioneC
		call eax
		add	esp,10h
		retn
Patch7:
		cmp ax,6
		jnz  Patch8
		// patch per salvare nome file tr4
		lea  edx, [ebp-2270h] 
		pushad
		// salvare in variabile globale il nome del file tr4 appena aperto

		push  edx  ;NomeFile tr4
		push offset NomeTr4Pc
		call strcpy
		add  esp,8

		popad
	;codice eliminato da patch
		push  419038h	;  ASCII "wb+" ;Mex_wb 
		push edx 
		mov eax, 40B79Eh    ;fopen
		call eax 
		add  esp, 08h 
 
		retn
Patch8:
		cmp ax, 7
		jnz  Patch9
		// nota: viene chiamata DOPO aver caricato tailinfos
		// in Memoria_RoomTailInfos


		// patch per controllare versione di wad file e se e' 0x82
		// salva nell'header ng il flag per segnalare che il tr4
		// usa una tabella estesa
		// in edx = versione .tom
		// in ecx = versione .wad

		cmp  ecx, 0x82
		jnz  EseguiCodiceNormale
		// la versione del wad 'e 0x82
		// quindi usa tabella estesa: tutto ok
		// ora impostare tutto usando il linguaggio C:
		pushad
		push  ecx  ;numero versione wad
		call  GestionePCTabellaSuoniEstesa
		add esp,4
		popad
		// se pero' versione del file tom e' diversa da 0x81
		// allora dare comunque errore (ma dopo aver impostato
		// i valori
		cmp edx, 0x81
		jz TuttoOk

EseguiCodiceNormale:
		mov eax, edx  ;version .tom

		push eax 
		push ecx 
		push  4195C8h	;  ASCII "ERROR: Object wad version %d, room wad version %d" ;Mex_ERROR_Object_wad_versio 
		mov eax, 4064E1h    ;Output
		call eax 
		push 00h	; |Arg1 = 00000000 
		mov eax, 4064B0h    ;AggiornaFinestra
		call eax	; \TOM2PC.004064B0 
		add  esp, 10h 
TuttoOk:
		retn
 		
Patch9:
		cmp ax,8
		jnz  Patch10
		// salva in variabile globale l'indice di texture
		// principale di anim range che potrebbe generare
		// range di animazione secondari
		// edi = indice tom tail
		mov word ptr [IndiceTomTailSource], di

		mov  dx, [esi+ 421986h] ;ZonaFrammentiAnimTex + 06h 
		mov  ax, [esi+ 421980h] ;ZonaFrammentiAnimTex 

		retn
Patch10:
		cmp ax, 9
		jnz Patch11
			
		// salva word per sizedemodata (in realta totcinematic)
		// mette zero se versione wad e' quella standard
		// mentre scrive il numero di suoni di tabella estesa 2048

		mov word ptr [RetValue], 0

		mov eax, 624570h  // VersioneFileWad
		mov ax, word ptr [eax]
		cmp ax, 82h
		jnz  StandardTab
		mov word ptr [RetValue], 2048
StandardTab:

		mov edx, 41E1C8h    ;HandleOutFileUncomp
		mov  edx, dword ptr [edx] 
		lea  eax, RetValue
		push edx	; /Arg4 => 00000000 
		push 01h	; |Arg3 = 00000001 
		push 02h	; |Arg2 = 00000002 
		push eax	; |Arg1 

		mov eax, 40B8C8h    ;fwrite
		call eax	; \TOM2PC.0040B8C8 
		add  esp, 10h 		
		
		retn
Patch11:
		cmp ax, 0ah
		jnz Patch12
	// calcola dimensione default di texture leggendo tail info
		// calcolare sizedefault di tailinfos
		pushad
		mov eax, 626594h    ;Tot_RoomTailInfos
		movsx  eax, word ptr [eax] 
		push eax // tottailinfos

		mov eax, 41E1CCh    ;Memoria_RoomTailInfos
		mov eax, dword ptr [eax]
		push eax // vettore tail infos
		call TrovaSizeTexDefault
		add esp,8
		popad
	// codice eliminato da patch
	push esi 
	push 300h 
	push 01h 
	push  892860h ;ZonaPaletteColoriRoom 
	mov eax, 40B7B1h    ;fread
	call eax 
	mov eax, 40315Ch
	jmp eax

Patch12:
		cmp ax, 0bh
		jnz Patch13
	// gestione sound sample mancanti in fast di creazione tr4
	// in edx l'indirizzo del file, poi da copiare in eax
	rep movsb 
	
	pushad
	push edx  // nome file
	call VerficaSoundSample
	add esp,4

	popad
	

	// qui procedura standard
	mov eax, edx // indirizzo file
	push ebx
	push eax 

	mov eax, 402070h    ;load_file
	call eax  ;carica file sound sample 
	mov ecx, 41E1C8h    ;HandleOutFileUncomp
	mov  ecx, dword ptr [ecx]
	mov edx, 401E09h // codice dopo patch
	jmp edx
Patch13:
		cmp ax, 0ch
		jnz Patch14
		// chiamata con jmp all'inizio di Reprocess
		// verifica se si puo' usare metacabinet
		// in caso affermativo ritorna subito da procedura reprocess
		
		
		mov eax, dword ptr [esp+04h] // nome file .lev
		pushad

		push eax
		call VerificaMetaCab
		add esp,4
		mov dword ptr [ResultMeta1], eax
		popad
		cmp byte ptr [ResultMeta1], 0
		jz  NienteMetaCab
		// si! verra' usato meta cab: uscire subito da funzione reprocess
		retn
NienteMetaCab:
		// codice eliminato 
	sub  esp, 2ch
	push ebx 
	push ebp 
	push esi 
	mov  esi, [esp+3ch] 
	mov eax, 403B2Ah // ritorno dopo codice patch
	jmp eax
Patch14:
		cmp ax, 0dh
		jnz Patch15
	// salva posizione seek attuale di file aperto in scrittura
	pushad

	// fare un fseek alla fine del file
	push 02h 		
	push 00h
	mov eax, 41E1C8h    ;HandleOutFileUncomp
	mov  eax, dword ptr [eax] 
	push eax
	mov eax, 40BDE2h    ;fseek
	call eax 
	add esp, 0ch

	mov eax, 41E1C8h    ;HandleOutFileUncomp
	mov  eax, dword ptr [eax] 
	push eax
	mov eax, 40BA01h    ;ftell
	call eax
	add esp,4
	mov dword ptr [BaseMeta], eax
	popad
	// codice originale
	push ecx	; INIZIO PROCEDURA 23 ;  SIZE=0x160 
	mov eax, 41E1C8h    ;HandleOutFileUncomp
	mov  eax, dword ptr [eax] 
	push esi 
	push eax 
	push 01h 
	mov edx, 401F1Ah
	jmp edx

Patch15:
	cmp ax, 0eh
	jnz Patch16
	// verificare se bisogna sostituire procedura di creazine file tr4
	cmp byte ptr [BaseMeta+04], 0  // bool TestUsaMetaCab;
	jnz UsaCabinet
	// non usare cabinet
	// codice eliminato
	push ebp	; INIZIO PROCEDURA 71 ;  SIZE=0x55B 
	mov  ebp, esp 
	mov  eax, 227ch 
	mov ecx, 40BB90h    ;SubAlloca4096BytesStack
	call ecx 
	mov eax, 406617h // ritorno al codice dopo patch
	jmp eax
UsaCabinet:
	// creare file tr4
	pushad
	call CreaMainFileTr4
	popad
	retn

	// patch per sistemare nextanim in formato abs in vettore animdispach (chiamata subito dopo aver caricato wad file
	// in tom2pc
	// nota: valore eax era stato salvato in ecx
Patch16:
	cmp ax, 0fh
	jnz Patch17
	// ripristina eax
	mov eax, ecx
	// codice eliminato da pathc
	push eax
	mov eax, 894B64h   ;N_StaticObjects
	mov  dword ptr [eax], edx 
	pop eax

	push ebx
	mov ebx, 41E204h   ;ptr_VetStaticObjects
	mov  dword ptr [ebx], eax 
	pop ebx

	// effettua correzione di animdispatch

	pushad
	mov eax, 894B68h // N_AnimDispatches
	mov eax, dword ptr [eax]
	push eax

	mov eax, 41E1F0h // Ptr_VetAnimDispatches
	mov eax, dword ptr [eax]
	push eax

	call ConvertiAbsAnimDispatch
	add esp, 8

	popad
Patch17:
	retn

	}
}



// salva in file .tom i dati di texture animate ma prima le riordina
// sulla base di uv range e imposta il numero di uvranges
void SalvaTextureAnimate(FILE *pFile)
{
	int *pTotRangeExported = (int*) 0x568280;
	int *VetIndiciRange = (int*) 0x56C440;
	StrAnimRange* VetAnimRange = (StrAnimRange*) 0x56C520;
	int * pVetPtrTextureExport = (int*) 0x566680;
	int i;
	int j;
	int IndiceI;
	StrTexParziali *pTexP;
	int NUVRange;
	int TotRange;
	int IndiceTail;
	int z;
	int IndiceStart, IndiceEnd;
	// salvare numero di range
	RoomWrite(pTotRangeExported,4,1,pFile);
	TotRange = *pTotRangeExported;
	DatiTexture.TotLastSort=0;
	if (TotRange == 0) return;

	// ordinare range sulla base di attributo uvrange.
	// i range dove c'e' animazione uv range metterli all'inizio
	// forse non e' piu' necessario se l'ordinamento viene fatto
	// direttamente in salvatomtailinfos
	// OrdinaRangeAnimati();

	// ora contare quanti uvrange ci sono
	NUVRange=0;
	for (i=0;i<TotRange;i++) {
		IndiceI = VetIndiciRange[i];
		if (GlobNgle.RoomTexAnimate.VetInfoRangeAnim[IndiceI] & 
			FAN_ANY_ROTATE) {
			NUVRange++;
		}
	}

	GlobNgle.RoomTexAnimate.Tot_UV_Rotate=NUVRange;
	GlobNgle.RoomTexAnimate.TotaleRangeNG = TotRange;
	GlobNgle.RoomTexAnimate.TestPresente=true;

	// salvare in DatiTexture il vettore indici per poter poi localizzare
	// la posizione dei vari range
	DatiTexture.TotLastSort  = TotRange;
	for (i=0;i<TotRange;i++) {
		DatiTexture.VetLastSorg[i] = VetIndiciRange[i];
	}



	InviaLog("RANGE SAVED IN FILE .TOM");
	// ora salvare le texture iniziali e finali
	for (i=0;i<TotRange;i++) {
		sprintf(BufferLog,"RANGE %d  Ordine=%d", i, VetIndiciRange[i]);
		InviaLog(BufferLog);

		IndiceI = VetIndiciRange[i];
		IndiceTail = VetAnimRange[IndiceI].IndiceStart;
		IndiceStart = IndiceTail;
		sprintf(BufferLog,"\tSTART: IndiceTex=%d   NewIndiceTex=%d",
			IndiceTail, pVetPtrTextureExport[IndiceTail]);
		InviaLog(BufferLog);
		
		IndiceTail = pVetPtrTextureExport[IndiceTail];
		RoomWrite(&IndiceTail,4,1,pFile);
		// salvare indice di tail di questo range
		// anche in datiextraanim
		GlobNgle.RoomTexAnimate.VetFromTex[i] = IndiceTail;

		IndiceTail = VetAnimRange[IndiceI].IndiceEnd;
		IndiceEnd = IndiceTail;
		sprintf(BufferLog,"\tEND: IndiceTex=%d   NewIndiceTex=%d",
			IndiceTail, pVetPtrTextureExport[IndiceTail]);
		InviaLog(BufferLog);

		IndiceTail = pVetPtrTextureExport[IndiceTail];
		RoomWrite(&IndiceTail,4,1,pFile);
		GlobNgle.RoomTexAnimate.VetToTex[i] =  IndiceTail;
		// ora convertire gli indicifull in datitexparziali
		// per avere gli indici realtivi delle full che saranno
		// usati in file .tom
		// full di questo range
		for (j=IndiceStart;j<=IndiceEnd;j++) {
			// trovare tutte le tex parziali di texture full = j
			for (z=0;z<GlobNgle.BaseTexParziali.TotRecords;z++) {
				if (GlobNgle.BaseTexParziali.VetParziali[z].IndiceFull  == j) {
					// trovata
					pTexP = &GlobNgle.BaseTexParziali.VetParziali[z];

					// salvare indice di range
					pTexP->IndiceRange = i;
					// converte indicefull al nuovo indice relativo
					pTexP->IndiceFull = pVetPtrTextureExport[j];

					sprintf(BufferLog,"\t\tIndiceRange=%d  TexParziale=%d per IndiceFull=%d  old(%d) with OffX=%d OffY=%d",
						pTexP->IndiceRange,
						pTexP->IndiceFrammento,
						pTexP->IndiceFull, j,
						pTexP->OffX ,
						pTexP->OffY);
					InviaLog(BufferLog);


				}
			}
		}


	}
	// mostrare dati 

	ShowDatiTexParziali(&GlobNgle.BaseTexParziali);

}
// chiamata all'inizio di preview screen
void InitPreviewRoom(void)
{
	StrAnimRange *pVetAnimRange = (StrAnimRange*) 0x56C520;
	StrTailInfo **p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo *pVetTailInfos;
	int *pTotIndiciAnim  = (int*) 0x56C4E8;
	int *pVetIndiciAnim = (int*) 0x56C440;
	int i,Indice;
	WORD TempWord;
	WORD TipoAnim;
	int j;
	DWORD Numero;
	int IndiceTail;
	BYTE *pMemoria;
	char NumeroChar;
	int SizeTexture;
	BYTE *pSrc;
	int  x,y;


	pVetTailInfos=*p2VetTailInfos;
	if (VersioneProject == 50) 
		SizeTexture=128;
	else
		SizeTexture=64;

	// salvare tutte le tail info che hanno animazione  di tipo scroll
	TotSalvaTail=0;

	for (i=0 ;i<*pTotIndiciAnim;i++) {
		// veedere se eseguire adesso l'animazione di questa serie texture
		Indice = pVetIndiciAnim[i];
		// estrarre frame 
		TempWord = GlobNgle.RoomTexAnimate.VetInfoRangeAnim[Indice];
		if (TempWord & FAN_ANY_ROTATE) {
			TipoAnim = TempWord & FAN_MASK_ANIM;
			j=0;
			for (IndiceTail = pVetAnimRange[Indice].IndiceStart;
					IndiceTail<= pVetAnimRange[Indice].IndiceEnd;IndiceTail++) {
				VetSalvaTail[TotSalvaTail].IndexTail = IndiceTail;
				VetSalvaTail[TotSalvaTail].SalvaBigTailInfo = 
										VetBigTails[IndiceTail];
				VetSalvaTail[TotSalvaTail].SalvaTailInfo = 
										pVetTailInfos[IndiceTail];

				if (TipoAnim == FAN_FULL_ROTATE ||
					TipoAnim == FAN_RIVER_ROTATE) {
					// --------- FULL SCROLL ------------
					pMemoria = (BYTE*) malloc(256 * SizeTexture * 2);
					VetRoomAnimaUV[Indice].VetMemOrigine[j++] = pMemoria;
					VetSalvaTail[TotSalvaTail].TestAllocata = true;
					VetSalvaTail[TotSalvaTail].pMemAllocata = pMemoria;
					// ora copiare immgine (doppia) da mappa normale
					// a memoria allocata appositiamente
					pSrc = VetBigTails[IndiceTail].pMemImage;

					// ora impostare in vettail la nuova memoria
					VetBigTails[IndiceTail].pMemImage = pMemoria;
					pVetTailInfos[IndiceTail].pMemImage = pMemoria;

					// copiare nella nuova memoria l'immagined ella texture (2 volte)


					Numero = 256 * SizeTexture;
					for (y=0;y<SizeTexture;y++) {
						for (x=0;x<SizeTexture;x++) {
							pMemoria[x] = pSrc[x];
							pMemoria[x+Numero] = pSrc[x];
						}
						pMemoria += 256;
						pSrc += 256;

					}


				}else {
					// ---------- HALF SCROLL ------------
					VetRoomAnimaUV[Indice].VetMemOrigine[j++] = 
								pVetTailInfos[IndiceTail].pMemImage;
					VetSalvaTail[TotSalvaTail].TestAllocata = false;
					// diminuire della meta' la dimensione y di questa texture
					if (VersioneProject == 50) {
						// dimensione e' 128, farla diventare 64
						// in vetbigtails e 32 in vettailinfo
						VetBigTails[IndiceTail].SizeY = 0x3fff;
						pVetTailInfos[IndiceTail].SizeY = 0x1fff;

					}else {
						// dimensione e' 64, fare divetnar entrambe a 32
						VetBigTails[IndiceTail].SizeY = 0x1fff;
						pVetTailInfos[IndiceTail].SizeY = 0x1fff;

					}
				}
				TotSalvaTail++;
			}

			// inizializzare campi comunque a entrambi i tipi di scroll
			// valore UvRotate ---
			VetRoomAnimaUV[Indice].ScrollPos =0;

			NumeroChar = (char) GlobNgle.RoomTexAnimate.
								VetInfoRangeAnim[Indice] & FAN_MASK_UVROTATE;
			if (NumeroChar == 0) {
				NumeroChar= (char) MyPrefRoom.DefaultUvRotate;
			}
			VetRoomAnimaUV[Indice].UVRotate = NumeroChar;
			// valore tempo ----
			VetRoomAnimaUV[Indice].LastTime = 0;
			Numero = TempWord >> 8;
			Numero &= 0x1f;
			if (TipoAnim == FAN_RIVER_ROTATE) Numero =0;

			if (Numero ==0) Numero=FRAME_SECONDO;
			Numero = 1000 / Numero;

			VetRoomAnimaUV[Indice].Delay = Numero;
			VetRoomAnimaUV[Indice].ByteRiga = 256 ;
			// ora che questa texture e' stata salvata si puo' modificarla
			// a seconda di che genere e'
			switch (TempWord & FAN_MASK_ANIM) {
			case FAN_HALF_ROTATE:

				if (VersioneProject == 50) 
					VetRoomAnimaUV[Indice].MascheraScroll = 0x3f;
				else
					VetRoomAnimaUV[Indice].MascheraScroll = 0x1f;

				break;
			case FAN_FULL_ROTATE:
			case FAN_RIVER_ROTATE:
				if (VersioneProject == 50) 
					VetRoomAnimaUV[Indice].MascheraScroll = 0x7f;
				else
					VetRoomAnimaUV[Indice].MascheraScroll = 0x3f;

				break;
			}

		}
	}


}

// chiamata alla fine di preview screen
void EndPreviewRoom(void)
{
	// ripristinare tutte le tail info modificate
	int i;
	int IndiceTail;
	StrTailInfo **p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo *pVetTailInfos;
	BYTE *pMemoria;

	pVetTailInfos = *p2VetTailInfos;

	for (i=0;i<TotSalvaTail;i++) {
		IndiceTail = VetSalvaTail[i].IndexTail;
		pMemoria= VetSalvaTail[i].pMemAllocata;


		VetBigTails[IndiceTail] = VetSalvaTail[i].SalvaBigTailInfo;
		pVetTailInfos[IndiceTail] = VetSalvaTail[i].SalvaTailInfo;
		if (VetSalvaTail[i].TestAllocata) free(pMemoria);

		
	}

}
// viene chiamato per ogni texinfo di tutto il livello
// quando c'e' una texture
// deve controllare se indicetail presente e' uno di quelli
// in VetIndici[] (terminate con -1)
// se la trova la sostituisce se si tratta dell'indice di tex
// full, mentre non fa niente nel caso e' una tex parziale

void AnalisiTexTrovata(StrTextureInfo *pTexInfo, short *VetIndici)
{

	int IndiceTail;
	int i;

	IndiceTail = pTexInfo->IndexAndFlags & 0x3ff;
	
	i=0;
	while (VetIndici[i] !=-1) {
		if (IndiceTail == VetIndici[i]) {
			// trovata
			DatiFindReplace.TotTrovate++;
			if (IndiceTail == DatiFindReplace.IndiceFind) {
				// e' quella full: sostituirla 
				pTexInfo->IndexAndFlags &= ~0x3ff;
				pTexInfo->IndexAndFlags |= DatiFindReplace.IndiceReplace;
			}
			return;
		}
		i++;
	}

}

// Invia a InviaMessaggioPerLog una stringa formattata basata su un testo
// e un valore che rappresente la memoria appena allocata
void FormattaTestoMemoria(char* pTesto, DWORD Numero)
{

	char Buffer[512];

	if (strcmp("SaveRawDataFile: remap_object", pTesto) != 0) {

		sprintf(Buffer,"MemAlloc=0x%X  (%s)",Numero, pTesto);

		__asm {
			lea	 eax, Buffer
			push	eax
			push	2
			mov	eax, 449870h  ;InviaMessaggioLog
			call	eax
			add		esp, 8
		}
	}
}
// chiamata all'inizio di output wad
// aggiunge flag acqua alle stanze con intensita' diversa da 4
// in modo da avere l'ondeggiamento della superfice
void AggiungePaludeAcqua(void)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	int i;

	pVetRooms = *p2VetRooms;

	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {	
			if ((pVetRooms[i].RoomButtons & 0x4) != 0 &&
				pVetRooms[i].WaterValue != 3) {
				pVetRooms[i].RoomButtons |= 0x01;
			}
		}
	}

}
// viene chiamata alla fine di output wad
// toglie flag acqua da stanze palude
void EliminaPaludeAcqua(void)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	int i;

	pVetRooms = *p2VetRooms;

	for (i=0;i<*pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot == 0) {	
			if (pVetRooms[i].RoomButtons & 0x4) {
				pVetRooms[i].RoomButtons &= ~0x01;
			}
		}
	}	
}

void AzzeraVetStaticRemap(void)
{
	int i;

	for (i=0;i<6000;i++) {
		GlobNgle.VetRemapStatics[i].IndiceStatic=-1;
	}
}
// salva indice static con i dati di room e indice realtivo
// il numero di stanza e' in variabile globale: 583BA0h   ;IndiceRoomAttuale
// mentre l'indice nel vettore di stanza e' salvato in IndiceStaticNow
// entrambi sono int
// nota: ricordarsi di incrmentare IndiceStaticNow
void SalvaIndiceStatic(short IndiceNGLE)
{
	WORD *pIndiceRoomAttuale = (WORD*) 0x583BA0;

	GlobNgle.VetRemapStatics[IndiceNGLE].IndiceRoom = *pIndiceRoomAttuale;
	GlobNgle.VetRemapStatics[IndiceNGLE].IndiceStatic = IndiceStaticNow;
	IndiceStaticNow++;
}

char *MessaggioEvilCord(void)
{
	static char BufMex[256];

	sprintf(BufMex, "EVIL: Unknown object type detected in room %d in case (%d,%d)",
		ScanRoom, ScanCordX, ScanCordZ);
	InviaLog(BufMex);
	return BufMex;


}
// legge l'intero percorso dal ile pFile
// ignora spazi intermedi
// se percorso e' vuoto restituisce NULL
char *LeggePercorsoEndSpace(FILE *pFile, bool TestWad)
{

	static char *VetExtWad[] = {".wad", ".was", ".swd", 
							".sam", ".sfx", ".tom", ".cd", NULL};
	static char *VetExtTga[] = {".tga", ".pcx", NULL};
	char CarNow;
	bool TestFinito;
	char **pVetExt;
	static char Buffer[256];
	int i;
	int j;

	if (TestWad) {
		pVetExt = VetExtWad;
	}else {
		pVetExt = VetExtTga;
	}

	TestFinito=false;
	Buffer[0]=0;
	i=0;
	do {
		RoomRead(&CarNow, 1,1, pFile);
		if (CarNow == ' ') {
			// considerarlo ok solo se una delle estensoni e' presente
			// oppure e' vuoto oppure c'era NA
			if (i == 0) return NULL;
			if (strcmpi(Buffer, "NA")==0) return NULL;

			j =0;

			while (pVetExt[j]) {
				if (InStr(0, Buffer, pVetExt[j]) != -1) {
					TestFinito=true;
					break;
				}
				j++;
			}

		}

		if (TestFinito==false) {
			// salvare carattere
			Buffer[i++] = CarNow;
			Buffer[i]=0;
		}

	}while (TestFinito==false);

	return Buffer;
}

bool CatturaPercorsoTga(char BufOut[], FILE *pFile)
{
	char *pNome;
	HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow	



	pNome = LeggePercorsoEndSpace(pFile, false);
	if (pNome == NULL) return false;

	
	strcpy(BufOut, pNome);

	return true;
}
// carica direttamente da file project aperto il percorso del file wad
// se trova "NA" o "" o utente rifiuta resittuisce false
// e non scrive niene in BufOut
// se tutto va bene copia in BufOut[] il percorso corretto

bool CatturaPercorsoWad(char BufOut[], FILE *pFile)
{
	char *pNome;
	HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow	
	char DirSrc[256];
	char *pDir;
	char NomeFile[256];


	pNome = LeggePercorsoEndSpace(pFile, true);
	if (pNome == NULL) return false;

	if (EsisteFile(pNome)== false) {
		// manca object.wad
		// dare messaggio di wad sbagliato e poi chiederlo cercando di 
		// impostare la cartella dove si trova effettivaemnte
		sprintf(BufferLog, "ERROR: cann't locate Object Wad file:\r\n\r\n%s\r\n\r\nPlease, select now the correct Object Wad", 
				pNome);
		MostraMessageBoxNGLE(BufferLog, 3);
		

		// scoprire la directory dove si trova
		// prima provare nella stessa directory dove c'e' il file di progetto
		pDir = SoloDir(NomeLoadProject);
		sprintf(NomeFile, "%s\\%s", pDir,SoloNome(pNome));
		if (EsisteFile(NomeFile)==false) {
			// non c'e' in cartella attuale
			// allora provare in cartella graphics/wads
			sprintf(NomeFile, "%s\\graphics\\wads\\%s", 
				Dir_Trle, SoloNome(pNome));
			
		}

		strcpy(DirSrc,SoloDir(NomeFile));


		sprintf(NomeFile, "%s.was", SoloNomeSenzaExt(NomeFile));
		
		PreparaOpenFile("Load Object Wad", "Was Files (.was)",
				"*.was",to_Load,DirSrc,NomeFile);
		ofn.hwndOwner= *pWindRoom;
		ofn.nMaxFile = 255;
	
		if (GetOpenFileName(&ofn)==FALSE) return false;
		strcpy(NomeFile, ofn.lpstrFile);
		if (EsisteFile(NomeFile)==false) return false;
	
		pNome = NomeFile;
	}
	
	// adesso in pNome c'e' il percorso ma potrebbe essere
	// un percorso lungo
	// farlo diventare corto.
	pDir= SoloDir(pNome);
	GetShortPathName(pDir, DirSrc,256);

	sprintf(BufferLog,"%s\\%s", DirSrc, SoloNome(pNome));

	if (strlen(BufferLog) > 95) {
		MostraMessageBoxNGLE("WARNING: in current project file the path of Object Wad excedes the space in the buffer name for Object Wad (max space = 95 characters)", 3);
	}
	
	strcpy(BufOut, BufferLog);


	
	return true;
}
// riceve record di settore r dati di trigger per modifica collisione
// modiica la collisione nel settoer esulla base delle indicazioni
// del trigger
void ModificaCollisione(StrItemsRoom *pRecItem, StrTriggerRoom *pTrigger, 
						StrStanzaRoomEdit *pRoom)
{
	int i;
	short NumeroClick;
	short ClickHight;
	StrGridXRoom *pSettore;
	int x,z;
	bool TestSlope;
	bool TestCeiling;
	short *pBase;
	char *pVetAngoli;
	int OrgX, OrgY;
	int SizeX, SizeY;
	int ItemIndex;
	int IndiceBase;
	WORD ValSplit;

	ClickHight = pTrigger->Timer & 0xff;

	i = pTrigger->Timer >> 8;


	NumeroClick = i & 0x3f;

	if (i & 0x40) {
		NumeroClick = -NumeroClick;
	}

	TestSlope=false;
	TestCeiling=false;
	ValSplit=0;
	
	switch (pTrigger->OggettoDaTriggerare ) {
	case 310:
		// 0:North-West Triangle
		i=1;
		break;
	case 311:
		// 1:North-East Triangle
		i=2;
		break;
	case 312:
		// 2:South-East Triangle
		i=3;
		break;
	case 313:
		// 3:South-West triangle
		i=0;
		break;
	case 314:
		// North slope collision
		TestSlope=true;
		i=1;
		break;
	case 315:
		// South slope collisione
		TestSlope=true;
		i=3;
		break;
	case 316:
		// east slope collision
		TestSlope=true;
		i=2;
		break;
	case 317:
		// west slope collision
		TestSlope=true;
		i=0;
		break;
		// ------------------- CEILING -----------
	case 318:
		// north ceiling slope
		TestSlope=true;
		TestCeiling=true;
		i = 1;
		break;
	case 319:
		// south ceiling slope
		TestSlope=true;
		TestCeiling=true;
		i=3;
		break;
	case 320:
		// east ceiling slope
		TestSlope=true;
		TestCeiling=true;
		i=0;
		break;
	case 321:
		// west ceiling slope
		TestSlope=true;
		TestCeiling=true;
		i =2;
		break;
	case 322:
		// North-west tri ceiling
		TestCeiling=true;
		i=2;
		break;
	case 323:
		// north-east tri ceiling
		TestCeiling=true;
		i = 1;
		break;
	case 324:
		// south-west tri ceiling
		TestCeiling=true;
		i=3;
		break;
	case 325:
		// South-east tri ceiling
		TestCeiling=true;
		i=0;
		break;
		// -------------- FINE CEILING -----------
	case 326:
		// 0:North-West Triangle
		i=1;
		ValSplit=1;
		break;
	case 327:
		// 1:North-East Triangle
		ValSplit=1;
		i=2;
		break;
	case 328:
		// 2:South-East Triangle
		ValSplit=1;
		i=3;
		break;
	case 329:
		// 3:South-West triangle
		ValSplit=1;
		i=0;
		break;
	default:
		MostraMessageBoxNGLE("ERROR: unknown collision flipeffect number",3);
		i=-1;
		break;
	}
	IndiceBase=i;

	if (TestCeiling) {
		ClickHight = -ClickHight;
	}

	// ora trovare tutti i settori di questo trigger
	OrgX = pRecItem->XOrgBlock;
	OrgY = pRecItem->ZOrgBlock;
	SizeX = OrgX+pRecItem->XSizeBlock;
	SizeY = OrgY+pRecItem->ZSizeBlock;

	if (OrgX < 0 || OrgX >22 || OrgY < 0 || OrgY > 22 ||
		SizeX > 22 || SizeX < 0 || SizeY > 22 || SizeY < 0) {
		
		MostraMessageBoxNGLE("ERROR: irregular box zone tgrigger",3);
	}

	for (x=OrgX;x<SizeX;x++) {
		for (z=OrgY;z<SizeY;z++) {
			ItemIndex = x +z * pRoom->XSizeBlocks;
			pSettore = &pRoom->pBlockGridCollision[ItemIndex];
			i = IndiceBase;
			if (TestCeiling) {
				pBase = &pSettore->WorldYCeilingClick;
				pVetAngoli = &pSettore->CeilingCornerClicks[0];



			}else {
				pBase = &pSettore->WorldFloorClick;
				pVetAngoli = &pSettore->FloorCornerClicks[0];
			}

			pVetAngoli[i] += ClickHight;	
			if (TestSlope) {
				i++;
				if (i > 3) i=0;
				pVetAngoli[i] += ClickHight;
			}
			(*pBase) -= NumeroClick;
			pSettore->TestTriangleSplit = ValSplit;

		}
	}

}
// chiamata prima di creare gliglia collision per file tom
// salva la stanz attule e la sua griglia di collisione e poi eminina
// eventuali trigger per collisioni
// salva qui:
// StrStanzaRoomEdit SalvaRoomCollision;
// BYTE *pSalvaGridCollision=NULL;
void PreparaCollisioni(void)
{
	DWORD SizeGrid;
	int i;
	int TotElimina;
	short VetElimina[80];
	int IndiceTrigger;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;
	StrTriggerRoom *pTrigger;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int ItemIndex;


	int *pIndiceRoomAttuale = (int *) 0x583BA0; // IndiceRoomAttuale:
	StrStanzaRoomEdit *pRoom;
	int j;

	pVetTriggers = *P2_VetTriggers;
	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetRooms = *p2VetRooms;

	pRoom = &pVetRooms[*pIndiceRoomAttuale];


	// prima copiare zona griglia e record room
	SizeGrid = pRoom->XSizeBlocks * pRoom->ZSizeBlocks * sizeof(StrGridXRoom );

	pSalvaGridCollision = (BYTE *) malloc(SizeGrid);
	memcpy(pSalvaGridCollision, pRoom->pBlockGridCollision, SizeGrid);

	memcpy(&SalvaRoomCollision, pRoom, sizeof(StrStanzaRoomEdit));
	TotElimina=0;
	// vedere se c'e' il trigger speciale
	for (i=0;i<pRoom->N_ObjAndTriggers;i++) {
		ItemIndex = pRoom->VetObjAndTriggerCodes[i];
		if (pVetBaseOggetti[ItemIndex].TypeObject== 0x10) {
			// e' un trigger
			// ora vedere se e' prorpio quello magico delle colliioni
			IndiceTrigger = pVetBaseOggetti[ItemIndex].SlotIndex;

			pTrigger  = &pVetTriggers[IndiceTrigger];
			
			if (pTrigger->TriggerCosaAttivare== 9  &&
				(pTrigger->OggettoDaTriggerare >= 310 && 
						pTrigger->OggettoDaTriggerare <=329)) {

				// si, e' il trrigger flipeffect di collisione
				// salvarequesto indicie perce' poi andra' eliminato
				VetElimina[TotElimina++] = i;

				ModificaCollisione(&pVetBaseOggetti[ItemIndex], pTrigger, pRoom);

			}
		}
	}
	// ora eliminare i lipeffect
	for (i=TotElimina-1;i >=0;i--) {
		ItemIndex = VetElimina[i];
		IndiceTrigger = pVetBaseOggetti[ItemIndex].SlotIndex;
		// togliere l'indice di questo trigger 
		for (j=ItemIndex;j<pRoom->N_ObjAndTriggers-1;j++) {
			pRoom->VetObjAndTriggerCodes[j] =
				pRoom->VetObjAndTriggerCodes[j+1];

			
		}

		pRoom->N_ObjAndTriggers--;
	}



}
// ciamata subito dopo
// ripristina la stanza originale.
// restore da qui:
// StrStanzaRoomEdit SalvaRoomCollision;
// BYTE *pSalvaGridCollision=NULL;
void RestoreCollisioni(void)
{
	DWORD SizeGrid;
	int *pIndiceRoomAttuale = (int *) 0x583BA0; // IndiceRoomAttuale:
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	StrStanzaRoomEdit *pRoom;

	pVetRooms = *p2VetRooms;

	pRoom = &pVetRooms[*pIndiceRoomAttuale];

	SizeGrid = pRoom->XSizeBlocks * pRoom->ZSizeBlocks * sizeof(StrGridXRoom );
	memcpy(pRoom->pBlockGridCollision, pSalvaGridCollision, SizeGrid);
	free(pSalvaGridCollision);

	memcpy(pRoom, &SalvaRoomCollision, sizeof(StrStanzaRoomEdit));

}


// chiamata alla fine del caricamento del project
// vedere di aggiornare i nomi slot e capire
// se sono applicabili col wad attuale
void SalvaWadInVetMieiSlot(void)
{
	char *pWadNow;	

	if (EsisteFile(NomeFileWad)==false) return;
	
	pWadNow = SoloNomeSenzaExt(NomeFileWad);
	strcpy(VetMieiNomiSlot.NomeWad, pWadNow);

}
// chiamata subito dopo aver caricato objects.h (una volta sola)
// salva i nomi originali di tutti gli slot in:
// StrNomeSlot  *pVetSalvaNomiSlot=NULL;
// int TotSalvaNomiSlot;
void SalvaNomiSlot(void)
{
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;	
	int i;	
	int *pTotSlot = (int*) 0x56D9F0; // TotOggettiWa
	DWORD SizeTesto;

	TotSalvaNomiSlot = *pTotSlot;
	if (TotSalvaNomiSlot==0) return;
	pVetSlotOggetti = *p2VetSlotOggetti;
	pVetSalvaNomiSlot = (StrNomeSlot*) malloc(TotSalvaNomiSlot * sizeof(StrNomeSlot));

	for (i=0;i<TotSalvaNomiSlot;i++) {
		SizeTesto = strlen(pVetSlotOggetti[i].pNome)+1;
		pVetSalvaNomiSlot[i].Slot = i;
		
		strcpy(pVetSalvaNomiSlot[i].NomeSlot, pVetSlotOggetti[i].pNome);
	}


}

	//	push eax //dati pulsanti
	//	push ecx // nome pulsante
	//	call CatturaPulsante
void CatturaPulsante(char *pNome, StrDatiPulsante *pDati)
{
	int i;

	i = BaseWindow.TotStandardButtons;
	if ((DWORD) pDati == 0x458F0C) {
		i=i+0;

	}
	BaseWindow.VetStandardButtons[i].pDatiPulsante = pDati;
	BaseWindow.VetStandardButtons[i].pNome = pNome;
	BaseWindow.VetStandardButtons[i].OldBackColor = pDati->ColoreFondo;
	if (strcmpi(BaseWindow.VetStandardButtons[i].pNome, "ON")==0) {
		// usare griio come colore di fondo
		BaseWindow.VetStandardButtons[i].OldBackColor = 1;
	}
	BaseWindow.TotStandardButtons++;


}
// verifica se linea e' di commento oppure e' vuota
bool IsLineaCommento(char Buffer[])
{
	DWORD i;
	
	if (Buffer[0]==0) return true;

	for (i=0;i<strlen(Buffer);i++) {
		if (Buffer[i] == ';') return true;
		if (Buffer[i] != ' ' && Buffer[i]!= '\t') return false;
	}
	// e' una linea tutta piena solo di spazi
	return true;

}
// toglie da buffer spazi inizxiali e finali o doppi
char *NormalizzaTesto(char Buffer[])
{
	int i;
	static char NuovoTesto[200];
	int j;
	char LastCar,MioCar;

	j=0;
	LastCar=' ';
	for (i=0;i< (int) strlen(Buffer); i++) {
		MioCar=Buffer[i];
		if (MioCar != ' ' || MioCar != LastCar) NuovoTesto[j++] = MioCar;
		LastCar=MioCar;
	}

	// ora a ritroso eliminare spazi finali
	for (i=j-1;i>=0;i--) {
		if (NuovoTesto[i] != ' ') break;
	}

	NuovoTesto[i+1] = 0;

	return NuovoTesto;



}
// riceve il record di un pulsante di tipo #P (preset) dove in original name
// potrebbe essere la definizione dell'origine del pulsante (x,y)
// se la sintassi non è qwuella restituisce false
// se invece è tutto ok, copia l'indirizzo dei dati pulsanti
bool DecodeOrigineButton(StrButtonRec *pRec) 
{
	char *pNome;
	DWORD n;
	char **VetParti;
	int TotParti;
	bool TestErrore;
	int OrgX,OrgY;
	int i;

	pNome=Trim(pRec->OriginalName);
	n=strlen(pNome);

	if (n == 0) return false;

	if (pNome[0] != '(' || pNome[n-1] != ')') return false;

	pNome[n-1]=0;
	pNome++;

	// ok, ora dovrebbe essere "234,43"
	VetParti=Split(pNome, ',', &TotParti, NULL,NULL, false);

	if (TotParti != 2) return false;

	OrgX = GetNumero(VetParti[0],&TestErrore);
	if (TestErrore) return false;

	OrgY = GetNumero(VetParti[1], &TestErrore);
	if (TestErrore) return false;

	// ok, cercare tra tutti i pulsanti
	for (i=0;i<BaseWindow.TotStandardButtons;i++) {
		if (BaseWindow.VetStandardButtons[i].pDatiPulsante->OrgX == OrgX &&
			BaseWindow.VetStandardButtons[i].pDatiPulsante->OrgY == OrgY) {

			pRec->pDatiPulsante = BaseWindow.VetStandardButtons[i].pDatiPulsante;
			return true;
		}
	}
	return false;


	
}
// chiamato da WM_PAINT
// fa il refresh della finestra buttonswindow
void RefreshButtons(HDC MioHdc) 
{
	int i;
	StrButtonRec *pRec;
	int j;
	StrHeaderButWindow *pHeader;
	DWORD TipoAlign;
	char *pTesto;
	HFONT hFont;
	HFONT OldFont;
	HBRUSH LastBrush;
	HWND TempWind;
	
	LastBrush=NULL;

	pHeader=&BaseWindow.Header;
	for (i=0;i<BaseWindow.TotButtons;i++) {
		pRec = &BaseWindow.pVetButtons[i];
		if (pRec->TipoRecord == 'R' || pRec->TipoRecord == 'F') {
			// impostare colori
			SetBkMode(MioHdc, OPAQUE);
			SetBkColor(MioHdc, pRec->BackColor);
			SetTextColor(MioHdc, pRec->ForeColor);
			// stampare testo
			if (pRec->TipoRecord == 'R') {
				LastBrush = pRec->hBrush;
				pTesto= pRec->OriginalName;
				hFont=pHeader->FontRows.hFont;
				TipoAlign=DT_CENTER;
				OldFont = (HFONT) SelectObject(MioHdc, pHeader->FontRows.hFont);
			}else {
				pTesto = OldFrameInfoText;
				strcpy(BaseWindow.LastInfoStampato, pTesto);
				TipoAlign=DT_LEFT;
				hFont=pHeader->FontFrames.hFont;
				OldFont = (HFONT) SelectObject(MioHdc, pHeader->FontFrames.hFont);
			}
			

			FillRect(MioHdc,&pRec->Box, pRec->hBrush );

			

			DrawText(MioHdc,
				pTesto, 
				strlen(pTesto), 
				&pRec->Box, 
				DT_EXPANDTABS | TipoAlign | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL);
			SelectObject(MioHdc, OldFont);

		}
		if (pRec->TipoRecord == 'C') {
			if (LastBrush) {
				// ridisegnare il fondo della palette colori
				FillRect(MioHdc, &pRec->Box, LastBrush);
			}
			// e tutti i pulsanti
			for (j=0;j<15;j++) {
				TempWind=GetDlgItem(BaseWindow.hWind, i*16+j);
				RedrawWindow(TempWind,NULL,NULL,RDW_INVALIDATE);
			}

		}
	}


}
LRESULT CALLBACK WndProcButton(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
     PAINTSTRUCT ps ;
	 static StrInfoMouse *pInfoMouse = (StrInfoMouse *) 0x46CE1C;
	 HWND MiaWind;
	 static DWORD *pStatusPulsanti = (DWORD*) 0x46E0A4;
	 static BYTE *pPulsantiMouse = (BYTE *) 0x46DA88;
	TYPE_GenericaSubPulsante MiaCall;
	static HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow	
	StrHeaderButWindow *pHeader;
	// qui punta a zonafallaci che contiene la palette variata
	static StrColoriPalette *BaseColoriPalette = (StrColoriPalette*) 0x583860;
	static StrColoriPalette *pZonaPal = (StrColoriPalette*) 0x5838A0;
	int i;
	RECT MioRect;
	POINT PuntoNgle;
	int j;
	int SizeWindow;
	DWORD idCtl;
	DWORD ExtraFlags;
	HBRUSH OldBrush;
	HPEN OldPen;
	DWORD MioColore;
	DRAWITEMSTRUCT *pDati;
	int OrgX, OrgY;
	int Riga, Colonna;
	int SizeX, SizeY;
	char Tipo;
	int n;
	int Resto;
	int NRighe;
	WORD Flags;
	bool TestErrore;
	HWND TempWind;
	StrButtonRec *pRec;
	HDC MioHdc;
	DWORD BackColor, ForeColor;
	static HINSTANCE *pHandleIstanzaRoom = (HINSTANCE*) 0x46E555;



     switch (uMsg)
          {

		case WM_CREATE:
			// creare tutti i pulsanti
			Riga=0;
			Colonna=0;

			pHeader= &BaseWindow.Header;

			for (i=0;i<BaseWindow.TotButtons;i++) {
				pRec = &BaseWindow.pVetButtons[i];

				SizeX= pHeader->ButtonSizeX;
				SizeY= pHeader->ButtonSizeY;
				OrgX = Colonna * pHeader->ButtonSizeX;
				OrgY = Riga * pHeader->ButtonSizeY;
				Tipo = pRec->TipoRecord;
				BackColor=pRec->BackColor;
				ForeColor=pRec->ForeColor;

				// vedere se dimensione e' diversa
				if (Tipo == 'R') {
					SizeX = SizeX * pHeader->Colonne;

				}

				if (Tipo == 'F') {
					SizeX *= pHeader->Colonne;
					n = GetNumero(pRec->NewName, &TestErrore);
					SizeY *= n;
				}
				switch (Tipo) {
				case 'B':
				case 'M':
				case 'P':
				case 'K':
					ExtraFlags=0;
					if (pRec->TestFlipFlop == true) {
						ExtraFlags = BS_AUTOCHECKBOX;

					}
					TempWind=CreateWindowEx(0,"button",
								pRec->NewName,
								WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
								BS_PUSHBUTTON | ExtraFlags ,
								OrgX, OrgY, 
								SizeX,
								SizeY,
								hwnd,(HMENU) (i*16),*pHandleIstanzaRoom, NULL);

					SendMessage(TempWind, WM_SETFONT, 
						(WPARAM) BaseWindow.Header.FontButtons.hFont, MAKELPARAM(TRUE, 0)); 
					RedrawWindow(TempWind,NULL, NULL, RDW_ERASE | RDW_INVALIDATE); 
					Colonna++;
					if (Colonna >= pHeader->Colonne) {
						Colonna=0;
						Riga++;
					}

					break;
				case 'C':
					// BS_OWNERDRAW
					// serie di 8 pulsanti col backround del giusto colore
					SizeWindow= pHeader->Colonne * pHeader->ButtonSizeX;
					Resto = SizeWindow  % 15;
					SizeX = SizeWindow / 15;
					pRec->Box.left = 0;
					pRec->Box.top = OrgY;
					pRec->Box.bottom = OrgY + pHeader->ButtonSizeY;
					pRec->Box.right = pHeader->ButtonSizeX * pHeader->Colonne;
					for (j=0;j<15;j++) {

						OrgX = j * SizeX + Resto/2;
						
						n=SizeX;
						

						TempWind=CreateWindowEx(0,"button","",
									WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
									BS_OWNERDRAW,
									OrgX, OrgY, 
									n,
									SizeY,
									hwnd,(HMENU) (i*16+j),
									*pHandleIstanzaRoom, NULL);


						MioColore = RGB(BaseColoriPalette[j].Blue << 2,
									BaseColoriPalette[j].Green << 2,
									BaseColoriPalette[j].Red << 2);
						
						BaseWindow.VetBrushPalette[j] = 
								CreateSolidBrush(MioColore);


					}
					
					if (Colonna > 0) {
						Colonna=0;
						Riga++;
					}
					Riga++;

					break;
				case 'F':
				case 'R':
					// frame o riga, in ogni caso
					// partono dall'inizio e sono zone di testo
					if (Colonna !=0) {
						Riga++;
						Colonna=0;
					}
					OrgX = Colonna * pHeader->ButtonSizeX;
					OrgY = Riga * pHeader->ButtonSizeY;
					SizeX = pHeader->ButtonSizeX * pHeader->Colonne;
					if (Tipo == 'R') {

						NRighe=1;
					}else {
						// framers

						NRighe = GetNumero(pRec->NewName, &TestErrore);
					}
					SizeY = pHeader->ButtonSizeY * NRighe;


					// salvare i dati di rettangolo 

					pRec->Box.left = OrgX;
					pRec->Box.top = OrgY;
					pRec->Box.right = OrgX+SizeX;
					pRec->Box.bottom = OrgY+SizeY;
					

					Riga+=NRighe;

					break;
				}
						

			}

			return 0;

		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED) {
				idCtl = (int) LOWORD(wParam);
				idCtl = idCtl >> 4;

				pRec=&BaseWindow.pVetButtons[idCtl];

				if (pRec->TipoRecord == 'K') {

					//  simulare invio del tasto
					SimulaKey = pRec->CodiciTasti[0];
					SimulaStatusKey = pRec->CodiciTasti[1];

					LeggiDirectInput(0);

					//PostMessage(*pWindRoom, WM_KEYDOWN, 0, 0); 
					SetFocus(*pWindRoom);

				}

				if (pRec->TipoRecord == 'B' ||
					pRec->TipoRecord == 'P' ||
					pRec->TipoRecord== 'M') {
					
					
					pInfoMouse->NaturaPulsante = 0x20;
					pInfoMouse->OffsetSubComando =NULL;
					pInfoMouse->pRecordPulsante = NULL;

					pInfoMouse->StatusTasti=0;
					if (pRec->TipoRecord=='M') {
						MiaCall = (TYPE_GenericaSubPulsante) pRec->pCallMenu;
						pInfoMouse->OffsetSubComando = pRec->pCallMenu;
						pInfoMouse->NaturaPulsante=0;
					}else {
						pInfoMouse->pRecordPulsante = pRec->pDatiPulsante;
						// vedere se bisogna infvertire status
						Flags = pRec->pDatiPulsante->Flags;
						if (Flags & 0x0001)  {
							if ((Flags & 0x0400)==0) {
								if (Flags & 0x0100) {
									toggle_bool(pRec->pDatiPulsante);
								}
							}
						}
						MiaCall = (TYPE_GenericaSubPulsante) pRec->pDatiPulsante->pSubGestione;
					}
					*pPulsantiMouse = 1;
					*pStatusPulsanti=0;
					BaseWindow.MioComandoInCorso=true;
					SetFocus(*pWindRoom);

					__asm {
						pushad
						mov eax, dword ptr [pInfoMouse]
						push eax
						mov ecx, dword ptr [MiaCall]
						call ecx
						add esp,4
						popad
					}
					BaseWindow.MioComandoInCorso=false;
					//MiaCall(pInfoMouse);
					
					break;
				}
			}
			break;

		case WM_DRAWITEM:
			// bisogna cambiare qualcosa del pulsante
			idCtl = (UINT) wParam;
			pDati = (DRAWITEMSTRUCT *) lParam;

			if (pDati->CtlType != ODT_BUTTON) break;
			i = idCtl >> 4;
			j = idCtl & 0xf;

			// calcolare dimensione
			pRec = &BaseWindow.pVetButtons[i];

			



			switch (pDati->itemAction) {
			case ODA_DRAWENTIRE:
				// disegnare il pulsante
				OldBrush= (HBRUSH) SelectObject(pDati->hDC, BaseWindow.VetBrushPalette[j]);

				if (*BaseWindow.pTestUsaTexture ==false &&
					*BaseWindow.pIndiceTexture == j) {
					// ridisegnare l'altro
					i = BaseWindow.PaletteLastSelezionato;

					if (i != -1 && i != j) {

						// dare messaggio di redraw quella window
						i = (idCtl & ~0x0f) | i;
						MiaWind= GetDlgItem (hwnd, i);
						RedrawWindow(MiaWind, NULL, NULL, RDW_INVALIDATE);
					}
					BaseWindow.PaletteLastSelezionato = j;
					BaseWindow.PaletteLastTest=false;

					OldPen = (HPEN) SelectObject(pDati->hDC, BaseWindow.PennaRossa);
				}else {
					OldPen = (HPEN) SelectObject(pDati->hDC, BaseWindow.PennaNera);
				}
				Rectangle(pDati->hDC, pDati->rcItem.left , pDati->rcItem.top ,
										pDati->rcItem.right, pDati->rcItem.bottom );
				SelectObject(pDati->hDC, OldBrush);
				SelectObject(pDati->hDC, OldPen);


				break;
			case ODA_SELECT:
				
				
				if (pDati->itemState == ODS_FOCUS) {
					// selezionato un colore
					// rendre questo attivo
					// 
					// ma se diverso toglirlo da palette di ngle
					if (j != *BaseWindow.pIndiceTexture) {
						HighlightPalette(false);
					}
					*BaseWindow.pTestUsaTexture = false;
					*BaseWindow.pIndiceTexture = j;

					// ridisegnare l'altro
					i = BaseWindow.PaletteLastSelezionato;

					if (i != -1 && i != j) {

						// dare messaggio di redraw quella window
						i = (idCtl & ~0x0f) | i;
						MiaWind= GetDlgItem (hwnd, i);
						RedrawWindow(MiaWind, NULL, NULL, RDW_INVALIDATE);
					}
					BaseWindow.PaletteLastSelezionato = j;
					BaseWindow.PaletteLastTest =false;
					// ora rendere visibile il tutto

					OldBrush= (HBRUSH) SelectObject(pDati->hDC, BaseWindow.VetBrushPalette[j]);
					OldPen = (HPEN) SelectObject(pDati->hDC, BaseWindow.PennaRossa );

					Rectangle(pDati->hDC, pDati->rcItem.left , pDati->rcItem.top ,
											pDati->rcItem.right, pDati->rcItem.bottom );
					SelectObject(pDati->hDC, OldBrush);
					SelectObject(pDati->hDC, OldPen);

					
					HighlightPalette(true);
					DrawTextures();
					SetFocus(*pWindRoom);
				}

				
				break;
			}
			return TRUE;

		case WM_PAINT:
	    	BeginPaint (hwnd, &ps) ;
			MioHdc =(HDC) GetDC(hwnd);
			RefreshButtons(MioHdc);
			ReleaseDC(hwnd, MioHdc);
			EndPaint (hwnd, &ps) ;
           return 0 ;  
		   
		case WM_CLOSE:
			DestroyWindow(hwnd);
			BaseWindow.hWind = NULL;
			return 0;
		case WM_MOVE:
			// mossa la finestra button

			if (hwnd != BaseWindow.hWind) break;
			if (IsIconic(hwnd)) {
				InviaLog("WndProcButton(): WM_MOVE  Finestra IsIconic==true, abortire");
				break;
			}
			
			// voglio sapere la posizione della finestra button alla barra
			GetWindowRect(hwnd, &MioRect);
			sprintf(BufferLog, "WndProcButton()> WM_MOVE: MioRect= {%d,%d, %d, %d}",
					MioRect.left, MioRect.top, MioRect.right, MioRect.bottom);
			InviaLog(BufferLog);
			// ottiene punto diorigine client di ngle
			PuntoNgle.x=0;
			PuntoNgle.y=0;
			ClientToScreen(*pWindRoom, &PuntoNgle);			

			MyPrefRoom.ButWndOrgX = MioRect.left  - PuntoNgle.x;
			MyPrefRoom.ButWndOrgY = MioRect.top  - PuntoNgle.y;

			break;





	 }



     return DefWindowProc (hwnd, uMsg, wParam, lParam);
}
// crea il font per la button windows e i suoi pulsanti
bool ImpostaFont(StrMioFont *pFont)
{

	LOGFONT TempFont;
	int SizeX, SizeY;

	memset(&TempFont, 0, sizeof(TempFont));


	TempFont.lfWeight  = pFont->FontBold;
	

	//TempFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;

	strcpy(TempFont.lfFaceName ,pFont->FontName );
	SizeX = pFont->FontSize/2;
	SizeY = pFont->FontSize;



	TempFont.lfWidth = SizeX;
	TempFont.lfHeight  = SizeY;


	pFont->hFont = CreateFontIndirect(&TempFont);

	if (pFont->hFont == NULL) return false;
	return true;



}
// crea finestra con pulsanti extra
void CreaButtonWindow(void)
{

	DWORD SizeX, SizeY;
	StrHeaderButWindow *pHeader;
	DWORD TotRighe;
	int ScreenX, ScreenY;
	int i;
	POINT MioPunto;
	int Colonne;
	int NRighe;
	int OrgX, OrgY;
	bool TestErrore;
	HDC MioHdc;
	HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow
	HINSTANCE *pHandleIstanzaRoom = (HINSTANCE*) 0x46E555;	

	DWORD WindStyle;
	char NomeClasse[] = "RoomEditClass";
	StrButtonRec *pRec;


	if (BaseWindow.TotButtons == 0) return;




	// calcolare quella che dovra' essere la dimensione
	// dell'area client della finestra
	pHeader = &BaseWindow.Header;

	// piu' complicata l'altezza, perche' alcuni puslanti speciali 
	// valgonoda soli una o piu' righe
	TotRighe=0;
	Colonne=0;
	for (i=0;i<BaseWindow.TotButtons;i++) {
		pRec = &BaseWindow.pVetButtons[i];

		switch (pRec->TipoRecord) {
		case 'B':
		case 'M':
		case 'P':
		case 'K':
			// ha la larghezza di un normale pulsante
			Colonne++;
			if (Colonne >= pHeader->Colonne) {
				TotRighe++;
				Colonne=0;
			}
			break;
		case 'R':
			// crea brush per questa riga
			pRec->hBrush = CreateSolidBrush(pRec->BackColor);
		case 'C':
			// richiede un'intera linea
			if (Colonne > 0) {
				TotRighe++;
			}

			TotRighe++;
			Colonne=0;

			break;
		case 'F':
			// frame che puo' essere alto piu' di una riga
			if (Colonne > 0) {
				TotRighe++;
			}
			pRec->hBrush = CreateSolidBrush(pRec->BackColor);
			NRighe = GetNumero(pRec->NewName, &TestErrore);
			TotRighe += NRighe;
			Colonne=0;
			break;
		}
	}
	if (Colonne>0) TotRighe++;

	SizeX = pHeader->ButtonSizeX * pHeader->Colonne;
	SizeY = pHeader->ButtonSizeY * TotRighe;

	SizeY += GetSystemMetrics(SM_CYCAPTION);
	SizeY += GetSystemMetrics(SM_CYEDGE)*2;

	SizeX += GetSystemMetrics(SM_CXEDGE)*2;

	SizeX +=2;
	SizeY +=2;

	// adesso vedere la dimensione che deve avere la finestra per
	// ospitare la zona client di quelle dimensioni

	WindStyle = WS_POPUP | WS_MINIMIZEBOX  | WS_SYSMENU | WS_CAPTION | WS_BORDER;


	// creare penna nera e rossa
	BaseWindow.PennaNera = CreatePen(PS_SOLID,1, 0x00000);
	BaseWindow.PennaRossa = CreatePen(PS_SOLID,1, 0x0000ff);


	MioPunto.x=0;
	MioPunto.y=0;

	ClientToScreen(*pWindRoom, &MioPunto);

	OrgX = MioPunto.x +MyPrefRoom.ButWndOrgX;
	OrgY = MioPunto.y +MyPrefRoom.ButWndOrgY;
	// correzione per posizione

	ScreenX = GetSystemMetrics(SM_CXSCREEN);
	ScreenY = GetSystemMetrics(SM_CYSCREEN);
	sprintf(BufferLog, "ScreenX=%d  ScreenY=%d", ScreenX, ScreenY);
	InviaLog(BufferLog);

	if (OrgX < 0 || OrgY < 0 || OrgX >= ScreenX || OrgY >= ScreenY) {
		sprintf(BufferLog, "Origin out of screen(%d, %d): clean up", OrgX, OrgY);
		InviaLog(BufferLog);
		OrgX=0;
		OrgY=0;
		MyPrefRoom.ButWndOrgX=0;
		MyPrefRoom.ButWndOrgY=0;
	}

	
	BaseWindow.WindSizeX = SizeX;
	BaseWindow.WindSizeY = SizeY;



	BaseWindow.hWind=NULL;
	BaseWindow.hWind = CreateWindowEx(WS_EX_APPWINDOW,NomeClasse, 
			BaseWindow.Header.Titolo, WindStyle, OrgX,OrgY, 
			SizeX,SizeY, *pWindRoom,NULL,*pHandleIstanzaRoom, 0);





	if (BaseWindow.hWind == NULL) {
		MioMessageBox("Error: can't create button window", "Error");
		BaseWindow.TotButtons=0;
		return;
	}

	// creare font
	if (ImpostaFont(&BaseWindow.Header.FontButtons)==false) {
		MioMessageBox("Failed the creation of font for BUTTONS", "ButtonsWindow error");
		BaseWindow.TotButtons = 0;

		return;
	}
	if (ImpostaFont(&BaseWindow.Header.FontFrames)== false) {
		MioMessageBox("Failed the creation of font for FRAMES", "ButtonsWindow error");
		BaseWindow.TotButtons = 0;

		return;
	}
	if (ImpostaFont(&BaseWindow.Header.FontRows)==false) {
		MioMessageBox("Failed the creation of font for ROWS", "ButtonsWindow error");
		BaseWindow.TotButtons = 0;

		return;
	}

	MioHdc = GetDC(BaseWindow.hWind);

	SelectObject(MioHdc, BaseWindow.Header.FontRows.hFont);
	ReleaseDC(BaseWindow.hWind, MioHdc);
	
	ShowWindow(BaseWindow.hWind, SW_SHOW);

	SetWindowPos(BaseWindow.hWind, HWND_TOP ,0,0,0,0, 
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	SetFocus(*pWindRoom);


}
// prova a caricare eventuali dati per ButtonsWindow
void ScansioneButtonsWindow(void)
{
	StrHeaderButWindow *pHeader;
	FILE *pFile;
	char BufLinea[513];
	char **VetParti;
	static char *VetStore[50];
	StrMioFont *pMioFont;
	char **VetPezzi;
	int TotPezzi;
	char BufferHost[256];
	int TotParti;
	int i,j;
	char NomeOriginale[80];
	char *pNome;
	char *pTipo;
	char BufErrore[100];
	char *pMiaLinea;
	static StrMiaPresetButtons VetPreset[] = {
		{"TINT_RED+",  0x458AA4}, {"TINT_RED-",  0x458ABC}, 
								{"TINT_RED_NUMBER", (DWORD) &DatiPulsanteObjectRosso },
		{"TINT_GREEN+",  0x458AD4}, {"TINT_GREEN-", 0x458AEC}, 
								{"TINT_GREEN_NUMBER", (DWORD) &DatiPulsanteObjectVerde},
		{"TINT_BLUE+",  0x458B04}, {"TINT_BLUE-",  0x458B1C}, 
								{"TINT_BLUE_NUMBER", (DWORD) &DatiPulsanteObjectBlu},
		{"OBJECT+", 0x458A2C}, {"OBJECT-", 0x458A44}, {"OBJECT_NAME", 0x458A5C},
		{"AMBIENCE_RED+", 0x458B4C}, {"AMBIENCE_RED-", 0x458B64}, 
								{"AMBIENCE_RED_NUMBER", (DWORD) &DatiPulsanteAmbienteRosso},
		{"AMBIENCE_GREEN+", 0x458B7C}, {"AMBIENCE_GREEN-", 0x458B94},
								{"AMBIENCE_GREEN_NUMBER", (DWORD) &DatiPulsanteAmbienteVerde},
		{"AMBIENCE_BLUE+", 0x458BAC}, {"AMBIENCE_BLUE-", 0x458BC4}, 
								{"AMBIENCE_BLUE_NUMBER", (DWORD) &DatiPulsanteAmbienteBlu},
		{"LIGHT_X+", 0x458D2C}, {"LIGHT_X-", 0x458D14}, 
								{"LIGHT_X_NUMBER", (DWORD) &DatiPulsanteLightX},
		{"LIGHT_Y+", 0x458D5C}, {"LIGHT_Y-", 0x458D44},
								{"LIGHT_Y_NUMBER", (DWORD) &DatiPulsanteLightY},
		{"LIGHT_LEN+", 0x458D8C}, {"LIGHT_LEN-", 0x458D74},
								{"LIGHT_LEN_NUMBER", (DWORD) &DatiPulsanteLightLen},
		{"LIGHT_CUT+", 0x458DBC}, {"LIGHT_CUT-", 0x458DA4},
								{"LIGHT_CUT_NUMBER", (DWORD) &DatiPulsanteLightCut},
		{"LIGHT_INT+", 0x458C84}, {"LIGHT_INT-", 0x458C6C},
								{"LIGHT_INT_NUMBER", (DWORD) &DatiPulsanteSpotInt},
		{"LIGHT_OUT+", 0x458CE4}, {"LIGHT_OUT-", 0x458CCC},
								{"LIGHT_OUT_NUMBER", (DWORD) &DatiPulsanteSpotOut},
		{"LIGHT_IN+", 0x458CB4}, {"LIGHT_IN-", 0x458C9C},
								{"LIGHT_IN_NUMBER", (DWORD) &DatiPulsanteSpotIn},
		{"COLOUR_RED+", 0x458BDC}, {"COLOUR_RED-", 0x458BF4},
								{"COLOUR_RED_NUMBER", (DWORD) &DatiPulsanteLightRosso},
		{"COLOUR_GREEN+", 0x458C0C}, {"COLOUR_GREEN-", 0x458C24}, 
								{"COLOUR_GREEN_NUMBER", (DWORD) &DatiPulsanteLightVerde},
		{"COLOUR_BLUE+", 0x458C3C}, {"COLOUR_BLUE-", 0x458C54},
								{"COLOUR_BLUE_NUMBER", (DWORD) &DatiPulsanteLightBlu},
		{NULL, 0}};




	char *pMexErrore;
	static char *VetTipoHeader[] = {"@COLUMNS", "@TITLE", "@BUTTON_WIDTH",
					"@BUTTON_HEIGHT", "@BACK_COLOR", "@FORE_COLOR",
					"@FONT", NULL};

	static char *VetFlipFlop[]= {"O", "DMG", "Cold", "NL", "L", "F", "M", "R",
						"V2d Map", "Lighting", "On", "2D Map", "Face Edit",
						"Draw Doors", "Transparent", "Double Sided", 
						"No Collision", "Crack Mode", "Show Light Meshes", 
						"Big Texture", NULL};
	static char *VetFontTarget[] = {"BUTTONS", "ROWS", "FRAMES", NULL};
	bool TestErrore;
	StrButtonRec *pRec;
	StrDatiPulsante *pButNow = (StrDatiPulsante *) 0x459678;


	BaseWindow.pIndiceTexture = (WORD *) 0x583BBA;
	BaseWindow.pTestUsaTexture = (bool *) 0x583BD4;
	BaseWindow.TestStopMemo =true;
	BaseWindow.PaletteLastSelezionato=-1;
	BaseWindow.PaletteLastTest=false;
	BaseWindow.LastInfoStampato[0]=0;
	// inizializzare valori per button window
	BaseWindow.pVetButtons=NULL;
	BaseWindow.TotButtons =0;
	pHeader = &BaseWindow.Header;

	pHeader->BackColor = 0x989898;
	pHeader->ForeColor = 0x000000;

	pHeader->ButtonSizeX = 100;
	pHeader->ButtonSizeY = 16;

	pHeader->Colonne = 3;
	pMioFont= &pHeader->FontButtons;

	pMioFont->FontBold = 300;
	strcpy(pMioFont->FontName,"Arial");
	pMioFont->FontSize = 14;

	pHeader->FontFrames = pHeader->FontButtons;
	pHeader->FontRows = pHeader->FontButtons;

	strcpy(	pHeader->Titolo,"Button Windows");


	pFile = fopen(GetFileTrle("ButtonsWindow.txt"),"rt");
	if (pFile == NULL) return;

	// ora integrare pulsanti standard (con testo)
	// con tutti quelli senza testo
	while (pButNow) {
		// vedere se questo e' presente
		for (i=0;i<BaseWindow.TotStandardButtons;i++) {
			if (BaseWindow.VetStandardButtons[i].pDatiPulsante == pButNow) break;
		}

		if (i == BaseWindow.TotStandardButtons) {
			// non c'era, aggiungerlo ora
			BaseWindow.VetStandardButtons[i].OldBackColor = pButNow->ColoreFondo;
			BaseWindow.VetStandardButtons[i].pDatiPulsante = pButNow;
			BaseWindow.VetStandardButtons[i].pNome = "";
			BaseWindow.TotStandardButtons++;

		}

		pButNow= pButNow->pNextRecord;
	}

	while (feof(pFile) == 0) {
		fgets(BufLinea, 512, pFile);
		RemoveNewLine(BufLinea);

		if (IsLineaCommento(BufLinea) == false) {
			pMiaLinea=Replace(BufLinea,"\t","");

			VetParti = Split(pMiaLinea,':',&TotParti,NULL,NULL,false);

			pTipo = VetParti[0];

			switch (pTipo[0]) {
			case '@':
				// e' roba di header

				i=0;
				while (VetTipoHeader[i]!= NULL) {
					if (strcmpi(VetTipoHeader[i], pTipo) == 0) break;
					i++;
				}
				if (VetTipoHeader[i]==NULL) {
					sprintf(BufErrore, "Unknown header command: '%s'",
						pTipo);
					pMexErrore = BufErrore;
					goto GoErrore;

				}
				switch (i) {
				case TH_COLUMNS:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}
					pHeader->Colonne = GetNumero(VetParti[1], &TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;

					}
					break;
				case TH_TITLE:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}

					strcpy(pHeader->Titolo, VetParti[1]);
					break;
				case TH_BUTTON_WIDTH:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}

					pHeader->ButtonSizeX = GetNumero(VetParti[1], &TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;

					}
					break;
				case TH_BUTTON_HEIGHT:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}

					pHeader->ButtonSizeY = GetNumero(VetParti[1], &TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;

					}
					break;
				case TH_BACK_COLOR:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}

					pHeader->BackColor = GetNumero(VetParti[1], &TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;

					}
					break;
				case TH_FORE_COLOR:
					if (TotParti!=2) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}

					pHeader->ForeColor = GetNumero(VetParti[1], &TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;

					}
					break;		
				case TH_FONT:
					if (TotParti!=5) {
						pMexErrore = "Wrong number of fields separated by ':' character";
						goto GoErrore;

					}
					i =0;
					while (VetFontTarget[i]) {
						if (strcmpi(VetFontTarget[i], VetParti[1]) == 0) break;
						i++;
					}

					switch (i) {
					case TF_BUTTONS:
						pMioFont = &pHeader->FontButtons;
						break;
					case TF_ROWS:
						pMioFont = &pHeader->FontRows;
						break;
					case TF_FRAMES:
						pMioFont = &pHeader->FontFrames;
						break;
					default:
						pMexErrore = "Unknown Target for Font command";
						goto GoErrore;
					}

					strcpy(pMioFont->FontName, VetParti[2]);
					
					pMioFont->FontSize = GetNumero(VetParti[3], &TestErrore);
					if (TestErrore) {
						pMexErrore = "Not valid number for Size font field";
						goto GoErrore;
					}
					pMioFont->FontBold = GetNumero(VetParti[4], &TestErrore);
					if (TestErrore) {
						pMexErrore = "Not valid number for Bold Weight field";
						goto GoErrore;
					}


					break;

				}
				break;
			case '#':
				// non e' un comando header ma di definizione button
				if (TotParti < 2 || TotParti > 4) {
					pMexErrore = "Wrong number di fields (separated by ':' character";
					goto GoErrore;
				}
				i=0;
				//allocare memoria per nuovopulante


				BaseWindow.pVetButtons = (StrButtonRec *) 
							realloc(BaseWindow.pVetButtons, 
								sizeof(StrButtonRec) * 
									(BaseWindow.TotButtons+1));
				
				pRec = &BaseWindow.pVetButtons[BaseWindow.TotButtons];
				BaseWindow.TotButtons++;

				pRec->TipoRecord = pTipo[1];
				
				if (pRec->TipoRecord == 'R' ||  pRec->TipoRecord == 'C') {
				
					// e' una riga,  palette colors
					strcpy(pRec->OriginalName, VetParti[1]);
				}else {
					VetPezzi = Split(VetParti[1], '=', &TotPezzi, 
							BufferHost,VetStore, true);
					strcpy(pRec->OriginalName, VetPezzi[0]);
					
					if (TotPezzi == 2) {
						strcpy(pRec->NewName, VetPezzi[1]);
					}else {
						strcpy(pRec->NewName, pRec->OriginalName);
					}

					if (pRec->TipoRecord== 'K') {
						// prendere da campo nome il codice di scansione
						j = GetNumero(pRec->OriginalName, &TestErrore);
						if (TestErrore) {
							pMexErrore="Not valid number as scan code in OriginalName field";
							goto GoErrore;
						}
						pRec->CodiciTasti[0] = j & 0xff;
						pRec->CodiciTasti[1] = (j & 0x0f00) >> 8;

						
					}
					if (pRec->TipoRecord == 'F') {
						// verificare che sia stato fornito il numero di righe
						// in NewName
						if (TotPezzi != 2) {
							pMexErrore="Wrong number of arguments";
							goto GoErrore;
						}
						j = GetNumero(pRec->NewName, &TestErrore);
						if (TestErrore) {
							pMexErrore="Not valid number as number of rows in NewName field";
							goto GoErrore;
						}
						if (j < 1 || j > 20) {
							pMexErrore = "Number of rows is out of valid range: 1 / 20";
							goto GoErrore;
						}
					}
				}


					
				pRec->BackColor = pHeader->BackColor;
				pRec->ForeColor = pHeader->ForeColor;
				pRec->TestFlipFlop = false;
				if (TotParti >= 3) {
					// c'e' anche backcolor
					pRec->BackColor  = GetNumero(VetParti[2],&TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;
					}
				}

				if (TotParti >=4) {
					// c'e' anche forcolor
					pRec->ForeColor = GetNumero(VetParti[3],&TestErrore);
					if (TestErrore) {
						pMexErrore= "Not valid number";
						goto GoErrore;
					}
				}

				// ok, ora la cosa piu' complicata, devo localizzare
				// i record di tutti i pulsanti
				// e se sono flipflop devo cercare valore da controllare
				// e memoria. 
				switch (pRec->TipoRecord) {
				case 'B':
					// button standard
					pNome = NormalizzaTesto(pRec->OriginalName);
					strcpy(NomeOriginale, pNome);

					for (j=0;j<BaseWindow.TotStandardButtons;j++) {
						if (strcmpi(NormalizzaTesto(BaseWindow.VetStandardButtons[j].pNome),
									NomeOriginale) == 0) {

                             // trovato
							 pRec->pDatiPulsante = BaseWindow.VetStandardButtons[j].pDatiPulsante;
							 pRec->OldBackColor = BaseWindow.VetStandardButtons[j].OldBackColor;
							 break;
						}
					}
					if (j == BaseWindow.TotStandardButtons) {
						pMexErrore= "Cann't find a standard button with the supplied OriginalName";
						goto GoErrore;
					}
					break;
				case 'M':
					// tipo menu
					pNome = NormalizzaTesto(pRec->OriginalName);
					strcpy(NomeOriginale, pNome);
					for (j=0;j<BaseWindow.TotStandardMenu;j++) {
						if (strcmpi(NormalizzaTesto(BaseWindow.VetStandardMenu[j].pNome),
									NomeOriginale) == 0) {

                             // trovato
							 pRec->pCallMenu = BaseWindow.VetStandardMenu[j].pCallMenu;
							 break;
						}
					}
					if (j == BaseWindow.TotStandardMenu) {
						pMexErrore= "Cann't find the menu with the supplied OriginalName";
						goto GoErrore;
					}
					break;
				case 'P':
					// tipo preset
					// usare vettore di conversione
					j=0;
					while (VetPreset[j].pOffset) {
						if (strcmpi(pRec->OriginalName , VetPreset[j].pNome)==0) {
							// trovato preset button
							pRec->pDatiPulsante = VetPreset[j].pOffset;
							break;
						}
						j++;
					}
					if (VetPreset[j].pOffset == NULL) {
						// non trovato, ma potrebbe essere nel formato di origine coordinate
						// ossia: (342,234)
						if (DecodeOrigineButton(pRec)==false) {
							pMexErrore = "Cann't find the preset name of OriginalName field";
							goto GoErrore;
						}
					}
					break;

				}
				// scoprire se e' un pulsante flip/flop
				pRec->TestFlipFlop=false;				
				if (pRec->TipoRecord == 'B' || pRec->TipoRecord == 'P') {

					j=0;
					while (VetFlipFlop[j]) {
						if (strcmpi(VetFlipFlop[j], pRec->OriginalName)==0) {
							pRec->TestFlipFlop =true;
							break;
						}
						j++;
					}

				}
				break;
			default:
				pMexErrore = "Missing '#' character of command type";
				goto GoErrore;


			}
			


		}
	}
	//WM_CTLCOLORBTN
	fclose(pFile);

	CreaButtonWindow();
	
	if (BaseWindow.hWind) {
		SendMessage(BaseWindow.hWind, WM_CLOSE, 0,0);
		CreaButtonWindow();
	}
	
	return;
GoErrore:
	sprintf(BufferLog, "Error in line:\r\n\r\n%s\r\n\r\n%s",
		 BufLinea, pMexErrore);
	MioMessageBox(BufferLog,"Error parsing ButtonsWindow.txt");
	fclose(pFile);
	BaseWindow.TotButtons=0;
	if (BaseWindow.pVetButtons != NULL) {
		free(BaseWindow.pVetButtons);
		BaseWindow.pVetButtons = NULL;
	}

}

// legge eventuali messggi wm_mouse
void  LeggiPeekMouse(void)
{

	MSG MioMsg;
	static HACCEL *pAcc = (HACCEL *) 0x46D930;
	static HWND *pWindRoom = (HWND*) 0x46E559;  //   ;HandleMainWindow

	if (PeekMessage(&MioMsg, *pWindRoom, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE)) {
		TranslateAccelerator(*pWindRoom, *pAcc, &MioMsg);
		DispatchMessage(&MioMsg);
		
	}	
}

// usa valori globali :
//WORD SimulaKey=0;
//WORD SimulaStatusKey=0;
void GestioneSimulaTasto(BYTE VetTasti[])
{

	if (SimulaKey) {
		VetTasti[SimulaKey] = 0x80;
	}
	// convertire da bit status a effetivi codici
	if (SimulaStatusKey & 0x1) {

		// right shift
		VetTasti[0x36] = 0x80;
	}

	if ((SimulaStatusKey & 0x02) !=0 &&
		(SimulaStatusKey & 0x01) == 0) {

		// left shift
		VetTasti[0x2a] = 0x80;

	}

	if (SimulaStatusKey & 0x04) {
		// ctrl
		VetTasti[0x1d] = 0x80;

	}

	if (SimulaStatusKey & 0x08) {
		// alt
		VetTasti[0x38] = 0x80;
	}

	OldSimulaKey=SimulaKey;
	OldSimulaStatusKey=SimulaStatusKey;


}

void MostraErroreNotFoundDevice(void)
{
	MioMessageBox("ERROR: Not Found Compatible Directx Video Device\r\n\r\nNote: this error happens with some new directx versions.\r\nTo fix it, you should click, with right mouse button, on NGLE.exe icon, choose Properties, select Compatibility panel and, in Settings frame, check the option [Reduce Colors], setting [16 bit Colors (65536)]\r\nNow NGLE will quit", "Direct X Error");

}

void MostraErrCreateDx(void)
{
	MioMessageBox("ERROR: Ngle failed to Create DirectX interface.\r\nYou should reduce (in compatibility properties of NGLE program) the number of colors to 16 (65536), or try to do work NGLE in compatibility mode for WindowsX or Windows7.\r\nNow NGLE will quit", "Direct X Error");
	MyPrefRoom.Flags |= FPR_NO_RESUME_CRASH;
}
// --------------------- PATCH SU ROOM EDITOR -------------------------
__declspec(naked) void RoomPatch_00(void)
{
	__asm {
	// Patch per chiudere sia view of file che log file
		mov eax, 454EFCh    ;handle della view
		mov eax, dword ptr [eax]
		test eax,eax
		jz   Salta
		push eax
		call dword ptr [UnmapViewOfFile]
Salta:
		mov eax, 493130h    ;handle del file log
		mov eax, dword ptr [eax]
		push  eax
		call dword ptr [CloseHandle]
		retn


	}
}

__declspec(naked) void RoomPatch_01(void)
{
	__asm {

		retn


	}
}
void FormattaTestoArgomento(char *pTesto, void* pArgomento)
{
	char Buffer[512];

	sprintf(Buffer,pTesto,pArgomento);
	__asm {
		lea	 eax, Buffer
		push	eax
		push	2
		mov	eax, 449870h  ;InviaMessaggioLog
		call	eax
		add		esp, 8
	}


}


__declspec(naked) void RoomPatch_02(void)
{
	__asm {
		// sostituire InviaMessaggioStdOut ricevendo due parametri
		// direttamente dallo stack
		mov		ecx, [esp+8]  ;Testo principale
		mov		eax, [esp+0ch] ;eventuale parametro per sprintf

		pushad
		
		push	eax
		push	ecx
		lea  eax, FormattaTestoArgomento
		call  eax
		add		esp, 8

		popad
		retn


	}
}

__declspec(naked) void RoomPatch_03(void)
{
	__asm {
		// sostituisce TerminaProgramma
		// nello stack ci sara' in [esp + 8] un numero
		// se e' 1 vuol dire che sta uscendo normalmente
		// se e' 0 e' un aborto, nel qual caso chiedere se si vuole fare un backup
		mov		eax, [esp+8]
		lea	   ecx, EsisteErrore
		mov  dword ptr [ecx], eax

		test eax,eax
		jnz	  UscitaNormale
		// c'e' stato un errore o un crash
		// crea report sul crash
		mov		ecx, esp
		push	0  ;nulli i puntatori per eccezioni
		push	0
		push	ecx
		lea		eax, CreaCrashReport
		call	eax
		add		esp,0ch

		
		// qui bisognerebbe fare un backup
		
		push 0fh
		lea   eax, MexFareBackup
		push eax
		lea  eax, MostraMessageBoxNGLE
		call  eax
		add esp, 8

		test eax,eax
		jz   UscitaNormale

		// ha dato l'ok per il salvataggio
		push 0
		lea   eax, Sub_PulsanteBackup
		call eax
		add esp,4
UscitaNormale:
		lea	 eax, MexProgrammaTerminato
		push	eax
		push	2
		mov	eax, 449870h  ;InviaMessaggioLog
		call	eax
		add		esp, 8

		lea		eax, EsisteErrore
		mov		eax, dword ptr [eax]
		test	eax,eax
		jz		UscitaConErrore
		push	0
		push	0
		push	0
		mov	  eax, 44A3DEh ;termina processo
		call  eax
		add esp, 0ch
		retn
UscitaConErrore:
		mov		eax, 44A47Bh  ;salta a exit process
		push	0
		jmp		eax


	}
}

__declspec(naked) void RoomPatch_04(void)
{
	__asm {
		// Gestione save project per aggiungere nome attuale del progetto
		// nel save dialog box
		// in [esi] c'e' titolo finestra, deve essere "Save Project"
		// per creare il meccanismo

		pushad

		// prima scrivere linea che e' stata tolta a causa della patch
		// mov dword ptr [StructOpenFile +2Ch], offset BufDirectoryMap 0x46E5E0
		mov		eax, 46E589h  // Base di StrucOpenFileName 
		mov		dword ptr [eax+2Ch], 46E5E0h  ;BufDirectoryMap
		
		// ora passare controllo a funzione c per gestire il controllo
		// e le operazioni vari
		push	46B858h;  BufferNomeFilePrj
		push    eax   ;base di OpenFileName
		push	esi   ;puntatore a stringa nome titolo finestra load o save
		
		lea  eax, MettiNomeProject
		call eax
		add  esp, 0Ch

		popad
		retn



	}
}

__declspec(naked) void RoomPatch_05(void)
{
	__asm {
		// visualizza sulla barra del titolo il nuovo nome del file 
		// viene chiamata dopo aver salvato un file project
		pushad
		// passare come argomenti l'handle della finestra
		// e il nome attuale del project
		push  0  ;tipo save
		mov   eax, 46E559h ;HandleMainWindow
		mov  eax, dword ptr [eax]
		push  eax
		mov  eax, 46B858h  ;BufferNomeFilePrj
		push eax

		lea   eax,AggiornaTitoloFinestra
		call eax
		add  esp, 0ch

		popad
		retn

	}
}

__declspec(naked) void RoomPatch_06(void)
{
	__asm {
		//  Chiamata quando viene cliclato il pulsante multiplo Water/rain/snow ecc
		// in Ecx puntatore a word con i flag della stanza
		// in edx puntatore byte alla quantita di acqua

		push  edx  ; Puntatore byte quantita water
		push  ecx  ; Puntatore word flag room
		lea  eax, CambiaPulsantiWater
		call  eax
		add esp, 08h
		retn

	}
}

__declspec(naked) void RoomPatch_07(void)
{
	__asm {
		// Chiamata ogni volta che viene visualizzata una nuova stanza
		// e bisogna aggiornare i pulsanti room sotto visuale 2d
		// in ECX puntatore word  a flag rooms

		push ecx
		push 4594C8h ;DatiPulsante_D_Room (adesso Cold)
		push 4594E0h ;DatiPulsante_Water  (adesso pulsante multiplo)
		lea  eax, ShowPulsantiRoom
		call  eax
		add esp, 0ch
		retn

	}
}

__declspec(naked) void RoomPatch_08(void)
{
	__asm {
		// esegue copyfile al posto di EseguiComando
		// in ecx = File sorgnete
		// in edx = File destinazione

		pushad

		push  00 ;sovrascrivi se il file gia' esiste
		push  edx ;destinazione
		push  ecx ;file sorgente
		call dword ptr [CopyFile]

		popad
		retn


	}
}

__declspec(naked) void RoomPatch_09(void)
{
	__asm {
		// in ESI c'e' il nome del file project che si sta per caricare

		pushad
		// inizalizzare nuovamente le tailinfos prima di caricare progetto
		lea		eax, InitTailInfos
		call	eax

		push	esi
		lea		eax, SalvaNomeLoad
		call	eax
		add		esp,4
		// ripristina nomi originali slot
		call	RipristinaNomiSlot
		// carica eventuale extra header da file project
		push	01h
		lea		eax, NomeLoadProject
		push	eax
		lea		eax, LeggiNGHeaderProject
		call	eax
		add		esp,8

		popad
		// ora mettere codice tolto dalla patch per aprire il file
		push  edi
		push 45B3D4h ;Mex_Rb
		push  esi   ;nomefile
		mov  eax, 44A22Fh  ;OpenFile
		call eax
		add esp,0ch
		mov ebp, eax

		retn

	}
}

__declspec(naked) void RoomPatch_0A(void)
{
	__asm {
		// prima patch per controllo conflitto trigger speciali
		// in [ESI] c'e' record trigger da controllare
		// se il trigger va bene restituisce c=1
		// altrimenti c=0

		cmp  byte ptr [esi], 0  ;Metodo attivazione
		jnz  Diverso
		cmp  word ptr [esi+6], 0  ;Pulsanti trigger
		jnz  Diverso
		cmp  [esi+4], 0   ;timer
		jz   TriggerOk
		;tutto ok tranne timer, considerarlo pero' ok se il COSA attivare
		;e' ACTION o FLIPEFFECT
		cmp word ptr [esi+8], 9    ;flipeffect
		jz	TriggerOk
		cmp word ptr [esi+8], 0bh ;Action
		jz  TriggerOk
Diverso:
		clc
		retn
TriggerOk:
		stc
		retn


	}
}

__declspec(naked) void RoomPatch_0B(void)
{
	__asm {

		// seconda patch per controllo conflitto trigger speciali
		// in [ECX] record da controllare
		// se ok restituisce c=1
		// altrimenti c=0
		// qui potrei fare: se trigggerwhat e' camera allora ok
		cmp word ptr [ecx+8], 1  ;camera
		jz  TriggerOk

		cmp  word ptr [ecx+6], 0  ;Pulsanti trigger
		jnz  Diverso
		cmp  [ecx+4], 0   ;timer
		jz   TriggerOk
		;tutto ok tranne timer, considerarlo pero' ok se il COSA attivare
		;e' ACTION o FLIPEFFECT 

		cmp word ptr [ecx+8], 9    ;flipeffect
		jz	TriggerOk
		cmp word ptr [ecx+8], 0bh ;ACTION
		jz  TriggerOk
Diverso:
		clc
		retn

TriggerOk:
		stc
		retn
	}
}

__declspec(naked) void RoomPatch_0C(void)
{
	__asm {
		// Patch per click sinistro in zona texture

		// restituisce c=1 se bisogna saltare codice originale
		//          se c-0 invece bisogna eseguire codice originale
		push  edi
		push  ebp
		push  ebx

		// impostare a 1 TextTextureOrColorPalette
		mov eax, 583BD4h   ;TestTextureOrColorPalette
		mov  dword ptr [eax], ebx  ;lo imposta a 1
		// se bigtexture = 0 allora nessuna patch

		// chiamare funzione per calcolo texture speciali
		mov  eax, 506A78h  ;Ptr_TextureTailInfos
		mov  eax, dword ptr [eax]
		push eax   ;inizio memoria di texturetailinfos
		push  edx  ;cord y
		push  esi  ;cord x
		
		lea  eax, AnalisiClickLeftTexture;
		call  eax
		add  esp,0Ch

		pop		ebx
		pop		ebp
		pop		edi
		stc
		retn


	}
}

__declspec(naked) void RoomPatch_0D(void)
{
	__asm {
		// Sostituisce funzione AggiungiTailInfo con quella personalizzata NG

		lea  eax, AggiungiTailInfo
		call eax
		
		retn
				

	}
}

__declspec(naked) void RoomPatch_0E(void)
{
	__asm {
		pushad


		// deve salvare sia bumpmap texture che mappa texture
		// ebx = handle del file
		// ecx = numero texture da esportare
		// edx = Numero texture da esportare
		push  edx
		push  ebx
		lea  eax, SalvaMappaTexture

		call  eax
		add esp, 8

		popad
		retn

	}
}

__declspec(naked) void RoomPatch_0F(void)
{
	__asm {
		// Inizializza VetBigTails con stessi datgi delle prime 256 tailinfo
		// gia' impostate

		pushad 
		// inizializzare tail
		lea		eax,InitTailInfos
		call	eax

		lea  edi, VetBigTails
		mov  eax, 0x506A78  ; Ptr_TextureTailInfos
		mov  esi, dword ptr [eax]
		mov ecx, 512  
		rep movsd 

		// impostare dimensione massima per mappa texture con valore piu' alto
		mov		eax, 4595CAh  ;valscroll_y di datipulsante_scrolltexture
		mov		dword ptr [eax], 2e00h

		popad
		
		mov eax, 0x56C4F4  ; N_TextureTails
		mov dword ptr [eax], 0x100  

		mov  eax, 0x429D20 ; InizializzaEdgePointers
		call eax
		retn

	}
}

__declspec(naked) void RoomPatch_10(void)
{
	__asm {
		// in ecx c'e' indice di tailinfo applicata a record MemoriaTexFaces
		// il record memoria TexFaces e' puntato Ptr_MemoriaTexFaces attuale
		// bisogna restituire in eax il valore di Ptr_MemoriaTexFaces

		// codice tolto da patch
		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		mov  dword ptr [offset SwapDword], ecx
		mov  ecx, [edx][ecx*8] ;estrae da tailinfo indirizzo memoria image
		mov  [eax], ecx  ; e lo copia  

		pushad 

		push	4032CCh ;indirizzo ritorno		
		push	ecx  ;memoria tga

		mov eax, 506A70h    ;Ptr_MemoriaTexFaces
		mov  eax, dword ptr [eax]
		// nota: quando lighitnin e' qattivo qui e'  0x2c 

		sub eax, 24h   ;per puntare all'inizio del record
		mov		ecx, dword ptr [SwapDword] 
		push ecx   ;IndiceTail

		push eax  ;AdrTexFacesNow

		lea   eax, SalvaIndiceTexFaces
		call eax
		add  esp, 10h

		popad 

		
		retn

	}
}

__declspec(naked) void RoomPatch_11(void)
{
	__asm {
		// PATCH ESEGUITA AD OGNI CICLO

		pushad
		lea		eax, EseguiOgniCiclo
		call	eax


		popad

		;codice che era stato eliminato
		mov ecx, 583BA0h    ;IndiceRoomAttuale
		mov  ecx, dword ptr [ecx]
		; codice lasciato in room: sub  esp, 20h
		mov  eax, ecx
		shl  eax, 05h
		sub  eax, ecx
		mov ecx, 46B850h    ;Ptr_VetRoomSlots
		mov  ecx, dword ptr [ecx]

		retn

	}
}

__declspec(naked) void RoomPatch_12(void)
{
	__asm {
		// chiamata quando si aggiunge nuova texturedd
		// salvare vetindextexturedd[indicetexture] = indicetail

		mov ecx, 493128h    ;TotMemoriaTextureDD
		mov  ecx, dword ptr [ecx]
		mov edx, 46E098h    ;Obj_DirectDrawSurface4
		mov  edx, dword ptr [edx]
#ifdef LOG_TEXTURE
		pushad
		push	ecx
		lea		eax, ShowMexStoreIndex
		call	eax
		add		esp,4
		popad
#endif
		pushad

		lea		eax, IndiceTailNow
		mov		eax, dword ptr [eax]

		;salvare indicetail nel corrispettivo indice texturedd
		lea		ebx, VetIndexTextureDD
		mov		dword ptr [ebx][ecx*4],eax

		popad

		retn

	}
}

__declspec(naked) void RoomPatch_13(void)
{
	__asm {
		// chiamata in LeggiDirectInput per controllare zone memoria


		// qui c'era chiamata a LeggeComandiTasti
		mov  eax, dword ptr [493118h] ;  AltroOggettoDirectInput
		test eax, eax
		jz   	MettiC1
		mov eax, 49311Ch    ;OggettoDirectInput
		mov  eax, dword ptr [eax]
		clc
		retn
MettiC1:
		mov		ecx, 46D704h ;TotTastiNelBuffer
		mov		dword ptr [ecx], 0
		stc
		retn

	}
}

__declspec(naked) void RoomPatch_14(void)
{
	__asm {
		// chiamata quando si preme click col pulsante destro in zona texture
		//   ecx = cordx mouse iniziale
		//   edx = cordy mosue iniziale
		// restituisce in ecx il valore ditriangolo
		push    ebp
		push	edi	
		push	esi

		push	edx		;cordy
		push	ecx		;cordx
		lea		eax, SelezionaDestroTexture
		call	eax
		add		esp,8
		mov		ecx,eax
		pop		esi
		pop		edi
		pop		ebp
		retn

	}
}

__declspec(naked) void RoomPatch_15(void)
{
	__asm {
		// imposta in window_class l'handle dell'icona per la finestra del room editor
		pushad

		push	ebp  ;handle instance
		lea		eax, OttieneHandleIcona
		call	eax
		add		esp,4

	
		mov ecx, 46E575h   ;WindowClass + 014h
		mov  dword ptr [ecx], eax   ;hICON
		
		
		mov eax, 46E581h   ;WindowClass + 020h
		mov  dword ptr [eax], ebx   ;Menu

		mov eax, 46E585h   ;WindowClass + 024h
		mov  dword ptr [eax],  461C64h ;Mex_RoomEditClass	
		popad

		retn

	}
}

__declspec(naked) void RoomPatch_16(void)
{
	__asm {
		// chiamata infunzione RilasciaTextureDD

		pushad
		mov eax, 493128h   ;TotMemoriaTextureDD
		mov  dword ptr [eax], 00h
		
		lea		eax,AzzerateTextureDD
		call	eax
		
		popad

		retn

	}
}

__declspec(naked) void RoomPatch_17(void)
{
	__asm {
		// patch chiamata alla fine del loading progetto
		// passare come argomenti l'handle della finestra
		// e il nome attuale del project
		pushad

		mov   eax, 46E559h ;HandleMainWindow
		push	1  ;tipo load
		mov  eax, dword ptr [eax]
		push  eax
		lea		esi, NomeLoadProject
		push  esi   ;nome file caricato

		lea   eax,AggiornaTitoloFinestra
		call eax
		add  esp, 0ch

		call SalvaWadInVetMieiSlot
		// aggiornare numero di trigger usati
		call RefreshUsedTriggersByPlugins

		popad
		retn

	}
}

__declspec(naked) void RoomPatch_18(void)
{
	__asm {
		// patch chiamata quando si alza o si abbassa un oggetto
		// se il tipo di oggetto (in edi) e' di quelli spostabili
		// memorizza stanza attuale in lista circolare
		// se il tipo di oggetto e' da muovere restiuisce c=1
		// se non e' restituisce c=0
		
		cmp  edi, 02h
		jz   OggettoDaSpostare
		cmp  edi, 08h
		jz   OggettoDaSpostare
		cmp  edi, 20h
		jz   OggettoDaSpostare
		cmp  edi, 40h
		jz   OggettoDaSpostare
		cmp  edi, 80h
		jz   OggettoDaSpostare
		// non e' da spostare
		clc
		retn
OggettoDaSpostare:
		pushad

		push	-1
		push	-1
		lea		eax, AddInOldRoomList
		call	eax
		add		esp,8

		popad
		stc
		retn



	}
}
// partch per room editor
void PatchZonaListaElenco(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;

	static StrPatch VetPatch[] = {{0x4357C1, true},
		{0x435800, true},
		{0x435807, true},
		{0x435838, true},
		{0x435A40, true}};

	OffsetOldInizio = 0x563F80;
	OffsetOldFine=0x564F80;
	TotPatchs=5;

	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
			OffsetOldFine, 0x2008,"ZonaListaElenco");
	pZonaVetList = (StrListElenco *) OffsetNew;
}

void PatchTotRecordElenco(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x4357C6, 'D'},
		{0x435814, 'D'}};

	TotPatchs=2;
	TotOld = 0x200;
	TotNew=0x400;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

// Creata in data: 2/13/2008 5:28:17 PM
void Patch_Per2Sun(void)
{
	int  i;
	static BYTE VetBytes[]={0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x431238;

	for (i=0;i<45;i++) {
		pMem[i] = VetBytes[i];
	}
}

__declspec(naked) void RoomPatch_19(void)
{
	__asm {
		// chiamata dopo l'esecuzione di ogni comando pulsante azionato dal mouse
		

		pushad
;----  Chiama ControllaOldRoom()


		mov		ecx, 583BA0h  ;IndiceRoomAttuale
		mov		ecx, dword ptr [ecx]

		mov  eax, ecx
		shl  eax, 05h
		sub  eax, ecx
		mov ecx, 46B850h    ;Ptr_VetRoomSlots
		mov  ecx, dword ptr [ecx]
		lea  eax, [eax][eax*2]
		lea  edx, [ecx][eax*4]
		add  eax, edx	
		;in eax adesso c'e' record room attuale
		movzx	ebx, word ptr [eax+72h]  ;ZSizeBlock
		push	ebx
		movzx	ebx, word ptr [eax+70h]  ;XSizeBolock
		push	ebx
		mov		ebx, dword ptr [eax+74h]  ;PtrTabGrid
		push	ebx
		push	eax		;RecordRoom
		lea		eax, ControllaOldRoom
		call	eax
		add		esp, 10h		
		popad
		;codice eliminato
		mov eax, 4697E8h   ;LOC_4697e8
		cmp  word ptr [eax], 00h
		
		jz   EsciConSalto
		
		mov eax, 46E4D4h    ;CordXMouse
		mov  ax, word ptr [eax]
		and eax, 0xffff
		clc
		
		retn
EsciConSalto:
		stc
		retn

	}
}

__declspec(naked) void RoomPatch_1A(void)
{
	__asm {
		; visualizza list box per selezione stanza
		mov eax, 564F80h    ;TotListaStanze
		mov  eax, dword ptr [eax]		
		push	eax

		mov		eax, dword ptr [pZonaVetList] ;  era: 563F80h  ; ZonaListaStanze
		push	eax
		lea		eax, ShowListaStanze
		call	eax
		add		esp,8
		retn


	}
}

__declspec(naked) void RoomPatch_1B(void)
{
	__asm {
		; gestisce uscita da selezione stanze per verificare se la stanza 
		; scelta e' la versione normale di una stanza flippata
		;in ebx c'e' l'indice restituito da seleziona stanze

		mov eax, 426090h    ;MostraStanze
		call eax

		lea		eax,MostraNuovaStanza
		push	ebx
		call	eax
		add		esp,4

		retn


	}
}

__declspec(naked) void RoomPatch_1C(void)
{
	__asm {
		; visualizza list box per selezione oggetto wad
		mov eax, 564F80h    ;TotListaElenco
		mov  eax, dword ptr [eax]		
		push	eax

		mov		eax, dword ptr [pZonaVetList]  ;  era: 563F80h  ; ZonaListaElenco
		push	eax
		lea		eax, ShowListaOggetti
		call	eax
		add		esp,8
		retn

	}
}

__declspec(naked) void RoomPatch_1D(void)
{
	__asm {
		;filtro per messaggio texture (1)
#ifdef LOG_TEXTURE
		push  461918h	;  ASCII "DXCreateSurface" ;Mex_DXCreateSurface
		push 02h
		mov eax, 449870h    ;InviaMessaggioLog
		call eax
		add		esp,8

#endif
		retn

	}
}

__declspec(naked) void RoomPatch_1E(void)
{
	__asm {
		;filtro messaggi texture (2)
#ifdef LOG_TEXTURE
		push ebx
		push edx
		push  461C94h	;  ASCII "%d %d" ;Mex_ArgD_ArgD
		push 05h
		mov eax, 449870h    ;InviaMessaggioLog
		call eax
		add		esp, 10h
#endif
		mov  ecx, 1fh
		xor  eax, eax

		retn

	}
}

__declspec(naked) void RoomPatch_1F(void)
{
	__asm {
		;filtro messaggi texture (3)
#ifdef LOG_TEXTURE
		push ecx		;prima era push eax
		push edx
		push  461C74h	;  ASCII "Texture Surface" ;Mex_Texture_Surface
		push  461650h	;  ASCII "Released %s @ %x - RefCnt = %d" ;Mex_Releas_Args_Argx_RefCnt_Argd
		push 04h
		mov eax, 449870h    ;InviaMessaggioLog
		call eax
		add  esp, 14h
#endif

		retn

	}
}

__declspec(naked) void RoomPatch_20(void)
{
	__asm {
#ifdef LOG_TEXTURE
		push ecx		;era push eax
		mov  eax, [esi+04h]
		push eax
		push  459AC4h	;  ASCII "Texture" ;Mex_Texture
		push  461650h	;  ASCII "Released %s @ %x - RefCnt = %d" ;Mex_Releas_Args_Argx_RefCnt_Argd
		push 04h
		mov eax, 449870h    ;InviaMessaggioLog
		call eax
		add  esp, 14h
#endif

		retn

	}
}

__declspec(naked) void RoomPatch_21(void)
{
	__asm {

		// inizializza vettore VetStack_74

		lea		edi, VetStack_74
		mov		ecx, 200h
		mov		eax,-1
		rep  stosd
		;codice tolto da patch
		mov edx, 46B850h    ;Ptr_VetRoomSlots
		mov  edx, dword ptr [edx]
		retn

	}
}

__declspec(naked) void RoomPatch_22(void)
{
	__asm {
		// confronta se indice edx di vetstack_74 e' uguale a -1 lo porta a -2
		lea		eax, VetStack_74
		cmp		word ptr [eax][edx*2], -1
		jnz		GiaUno
		mov		word ptr [eax][edx*2], -2
GiaUno:
		retn

	}
}

__declspec(naked) void RoomPatch_23(void)
{
	__asm {
		// conta numero di tail info da esportare



		xor		edi,edi
		lea		eax, VetStack_74
		// e se qui si dovesse controllare solo per le prime 256 ?


		mov  ecx, 400h
zompo1:
		cmp  word ptr [eax], -01h
		jz   zompo2
		mov  [eax], di
		inc  edi
zompo2:
		add  eax, 02h
		dec  ecx
		jnz  zompo1
		// analizzare in dettaglio i segnali di tail info da esportare

		pushad
		lea		eax, VetStack_74
		push	eax
		lea		eax, AnalisiTailExport
		call	eax
		add		esp,4
		popad

		retn

	}
}

__declspec(naked) void RoomPatch_24(void)
{
	__asm {
		pushad


		mov ebp, 56C4F8h    ;Ptr_TextureRawImage
		mov  edi, dword ptr [ebp]
		mov		eax, 00000004h
		mov		ecx, 100000h  ;per 4mb ufficiali  SIZE_MAP_TEXTURE / 4
		rep stosd

		mov		ecx, (SIZE_MAP_TEXTURE & 0x1fffff) / 4
		xor		eax,eax
		rep stosd
		
		popad
		mov ecx, 583C25h    ;Tga_SizeY
		movsx  ecx, word ptr [ecx]
		mov edx, 583C23h    ;Tga_SizeX
		movsx  edx, word ptr [edx]

	
		retn

	}
}
// chiamata quando viene premuto pulsante "Statistcs" in tom2pc
void ShowStatistichePC(HWND hDialogo, bool TestShowTesto)
{
	char Testo[10000];
	HWND *pWindInfoListBox =  (HWND*) 0x423E88;
	HWND WindListBox;
	int TotItem;
	int	i;
	FILE *TempFile;


	WindListBox = *pWindInfoListBox;
	
	Testo[0]=0;
	if (WindListBox != NULL) {
		TotItem = SendMessage(WindListBox,LB_GETCOUNT,0,0);
		strcat(Testo,"Standard Log Report\r\n----------------------------\r\n");

		for (i =0 ;i<TotItem;i++) {
			SendMessage(WindListBox,LB_GETTEXT, i, (LPARAM) BufferLog);

			strcat(Testo,BufferLog);
			strcat(Testo,"\r\n");
		}

		strcat(Testo,"----------------------------\r\n");
	}

	// ora visualizzare il file
	TempFile= fopen(GetFileCrash("NG_Tom2Pc_Last_Log.txt"),"wb");
	fprintf(TempFile,Testo);
	fclose(TempFile);
	if (TestShowTesto)	ShellExecute(NULL,"open",GetFileCrash("NG_Tom2Pc_Last_Log.txt"),"","",SW_SHOW);
		

}
__declspec(naked) void RoomPatch_25(void)
{
	__asm {
		pushad
		lea		eax, ShowStatistiche
		call	eax
		popad
		;codice eliminato da patch
		push 3eh	; |Arg4 = 0000003E
		push 1feh	; |Arg3 = 000001FE
		push 288h	; |Arg2 = 00000288
		push 126h	; |Arg1 = 00000126
		
		mov eax, 461CA8h   ;ColorePerTesto
		mov  byte ptr [eax], 00h

		mov eax, 43662Eh    ;AggiornaZonaSchermo
		call eax
		add		esp,10h




		retn

	}
}

__declspec(naked) void RoomPatch_26(void)
{
	__asm {

		// in edi base iniziale di file tga appena caricato
		pushad
		push	edi
		lea		eax, AllocaFileTga
		call	eax
		add		esp,4
		popad
		// codice eliminato
		
		mov  dx, [edi+0ch]
		push eax
		mov eax, 583C23h   ;Tga_SizeX
		mov  word ptr [eax], dx
		pop eax
		mov  ax, [edi+0eh]
		and	ax, ~3fh
		push ebx
		mov ebx, 583C25h   ;Tga_SizeY
		mov  word ptr [ebx], ax
		pop ebx
	




		retn

	}
}

__declspec(naked) void RoomPatch_27(void)
{
	__asm {
		// salva il nome dell'ultimo file oggetti caricato
		push  45B3D8h	;  ASCII "WAD" ;Mex_WAD
		push  56C7A0h ;NomeFileWas
		mov eax, 41A3B0h    ;ForzaEstensione
		call eax
		add		esp,8

		pushad

		push	56C7A0h ;NomeFileWas
		lea		eax, SalvaNomeWad
		call	eax
		add		esp,4
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_28(void)
{
	__asm {
		// patch per scrivere versione del project
		pushad
		push	ebp
		lea		eax,SalvaProjectName
		call	eax
		add		esp,4


		push ebx
		push 01h
		push 04h
		push  56C704h ;ProjectVersionNumber

		lea	 eax, VersioneProject   ;valore globale di next generation
		mov	 eax, dword ptr [eax]
		mov	 ecx, 56C704h ;ProjectVersionNumber	
		mov  dword ptr [ecx], eax
		
		mov eax, 44AD68h    ;fWrite
		call eax

		add	 esp, 10h

		popad

		retn

	}
}

__declspec(naked) void RoomPatch_29(void)
{
	__asm {

		// chiamata prima di carficare file tga chiamato da pulsante
		;azzera test tga project
		lea		eax,TestTgaProject
		mov		dword ptr [eax], 0
		
		// codice eliminato da patch
		push  4581E8h	 ;Mex_TGA
		mov eax, 433D50h    ;PreparaPatternFiles  
		call eax
		add		esp,4

		retn

	}
}

__declspec(naked) void RoomPatch_30(void)
{
	__asm {
		// chiamata prima loadtga fatto da caricamento project
		push	eax
		lea		eax,TestTgaProject
		mov		dword ptr [eax], 1
		pop		eax
		;codice eliminato da patch
		not  ecx
		dec  ecx
		mov  eax,  46BA78h ;NomeFile_Tga
		mov  edi, ecx
		sub  eax, 03h
		add  edi, eax
		mov  ecx, 04h
		xor  edx, edx
		// verifica se nome tga e' corretto, se non lo e'
		// chiede subito di selezionarlo
		pushad

		push  46BA78h
		call  ControllaNomeTga
		mov byte ptr [EsitoMappa], al
		add esp,4
		popad
		cmp byte ptr [EsitoMappa], 0
		jz  NormaleTga
		// e' pcx
		stc
		retn
NormaleTga:
		clc
		retn
	}
}

__declspec(naked) void RoomPatch_31(void)
{
	__asm {
		// ripristino dei dati textail modificati prima di caricare
		// file tga
		push  583C20h ;HeaderTga
		mov eax, 42ADF0h    ;InizializzaTexture
		call eax
		add		esp,4
		pushad
		lea		eax, RipristinaTailInfos
		call	eax

		popad

		retn

	}
}

__declspec(naked) void RoomPatch_32(void)
{
	__asm {
		// visualizza finestra animazioni texture
		pushad
		push	2  ;Tipo Animazione
		lea		eax,ShowTexture
		call	eax
		add		esp,4

		mov		eax, 42B820h   ;CreaIndiciPerAnimationRange
		call	eax

		popad
		retn

	}
}

__declspec(naked) void RoomPatch_33(void)
{
	__asm {
		// visualizza finestra texture sonore
		pushad
		push	1  ;Tipo Animazione
		lea		eax,ShowTexture
		call	eax
		add		esp,4
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_34(void)
{
	__asm {
		// in word ESI+26h c'e' il valore colore object tint da aggiornare sullo schermo
		pushad
		movzx	ebx, word ptr [esi+26h]
		push	ebx
		lea		eax, MostraColoreObject
		call	eax
		add		esp,4
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_35(void)
{
	__asm {
		// sostituisce funzione AnimaTextureInPreview
		pushad
		lea		eax,AnimaTexturePreview
		call	eax
		popad
		retn


	}
}

__declspec(naked) void RoomPatch_36(void)
{
	__asm {
		// patch per gestire visione verticale 2d di mappa
		// se la gestione e' verticale restituisce c=1
		// se invece restituisce c=0 allora bisogna continuare per 
		// mostrare mappa 2d orizzonale
		mov eax, 56C70Ch   ;Tot_PointerRoom
		mov  dword ptr [eax], edx

		lea		eax,TestMappaVerticale
		mov		eax, dword ptr [eax]
		test eax,eax
		jz		MappaNormale
		// mappa verticale 2d
		pushad
		push	edx		;TotPointer
		push	ecx		;base pointer room
		lea		eax,   ShowMappaVerticale
		call	eax
		add		esp,8
		popad
		stc
		retn
MappaNormale:
		push  4267D0h   ;call back per pointer room ;Sub_Gestione_PointerRoom
		push 04h  ;dimensione di ogni pointer?
		push edx  ;tot pointer
		push ecx  ;Base pointer room

		mov eax, 44AB58h    ;OrdinaPuntatori
		call eax
		mov eax, 56C70Ch    ;Tot_PointerRoom
		mov  eax, dword ptr [eax]
		add  esp, 10h
		clc

		retn

	}
}

__declspec(naked) void RoomPatch_37(void)
{
	__asm {
		// chiamata all'inizio di MostraPianta2d
		// per settare colore di fondo di zona mappa 2d
		lea		eax, TestMappaVerticale
		mov		eax, dword ptr [eax]
		test eax,eax
		jnz		SfondoGiallo
		// codice eliminato da patch
		mov eax, 583BF0h    ;TestMostraFlipMap
		mov  eax, dword ptr [eax]
		test eax, eax
		setnz  al
		dec  al
		and		eax,3
		retn
SfondoGiallo:
		mov		eax,10  ;giallo
		retn

	}
}

__declspec(naked) void RoomPatch_38(void)
{
	__asm {
		// patch per identificare click su mappa 2d
		// in edx = cordx mouse,  ecx = cordy mouse
		lea		eax, TestMappaVerticale
		mov		eax, dword ptr [eax]
		test eax,eax
		jz		MappaOrizzontale
		// mappa verticale

		mov		eax, 56C70Ch  ; Tot_PointerRoom
		mov		eax, dword ptr [eax]
		push	eax	 ;TotIndici

		mov		eax, 46B854h   ; Ptr_VetPointerRooms
		mov		eax, dword ptr [eax]
		push	eax	;VetIndici

		push	ecx		;mousey
		push	edx		;mousex
		lea		eax, ClickMouseMappa
		call	eax
		add		esp,10h
		mov		esi,eax
		retn
MappaOrizzontale:
		push ecx
		push edx
		mov eax, 426600h    ;TrovaStanzaClickata2dMap
		call eax
		add  esp, 08h
		mov  esi, eax
		retn

	}
}

__declspec(naked) void RoomPatch_39(void)
{
	__asm {
		// patch per allocare una memoria maggiore del previsto per il file tga
		// in modo che se anche non e' un multiplo di 64 non vada in crash
		and		cl, 0c0h
		movzx	eax, cx

		push ebx
		mov ebx, 583C25h   ;Tga_SizeY
		mov  word ptr [ebx], ax
		pop ebx
		shr		eax, 6  ;divide per 64
		add		eax, 1	;aumenta di 1
		shl		eax, 6  ;e ora rimoltiplica per 64


		movzx  ecx, dx

		
		imul eax, ecx   ;eax = size y     /  ecx = sizex

		retn


	}
}

__declspec(naked) void RoomPatch_3A(void)
{
	__asm {
		// gestisce controllo per tasti associati a voci dei menu



	Balzo1:
		mov  edx, [edi+15h]   ;codice scansione ?
		test edx, edx
		jz   Balzo5
	Balzo2:
		mov  cl, [edx+0ch]   ;qui ci sono i tasti addizionali (ctrl alt ecc) per menu
		test cl, cl
		jz   NienteShift  ;se non e' previsto controlla solo codice scansione
		;controlla se e' uguale tasto associato (ctrl alt)
		movsx  eax, word ptr [esi+18h]  ;codice associato scansione premuto adesso
		mov  ebp, eax
		and  ecx, 0ffh
		and  ebp, 04h  ;conrolla 4
		cmp  ebp, ecx
		jz   Balzo3  ;ok tasto associato
		and  eax, 08h
		cmp  eax, ecx
		jnz  Balzo4
		jmp	 Balzo3
		// il menu attuale non prevede alcun tasto shift extra
		// modificar ein modo che venga accettato SOLO se anche l'input non riceve
		// alcun tasto shift cltr o alt
NienteShift:
		mov		ax, word ptr [esi+18h]  // tasti sfhit di input
		test  ax, 0x0e  ;se 2 o 4 o 8  premuti ignorare comando
		jnz		Balzo4

		;tasto associato e' uguale (o non previsto)
	Balzo3:
		mov  cl, [esi+16h]    ;codice tasto premuto adesso
		mov  al, [edx+0dh]    ;codice tasto del menu
		cmp  cl, al
		jz   BalzoTrovato
	Balzo4:
		mov  edx, [edx]
		test edx, edx
		jnz  Balzo2
	Balzo5:
		mov  edi, [edi]
		test edi, edi
		jnz  Balzo1
		;trovato niente
		clc
		retn
BalzoTrovato:
		stc
		retn

	}
}

__declspec(naked) void RoomPatch_3B(void)
{
	__asm {
		// chiamato quando e' stato premuto tasto f12 e prima di eseguire salva snapshot
		// in ecx base dati tasto premuto
		// in byte  [ecx+16 ] codice scansione tasto
		// in word [ecx+18] tasti shift
		mov		eax, ecx
		mov		cx, word ptr [eax+18h]
		test	cx, 8  ;tasto ALT ?
		jz		SaltaSnap

		mov eax, 42B960h    ;SaveSnapShot
		call eax
		push 00h
		mov eax, 419B40h    ;DopoComandoEseguito
		call eax
		add  esp, 04h
SaltaSnap:
		retn

	}
}

__declspec(naked) void RoomPatch_3C(void)
{
	__asm {
		// patch chiamata da InizializzaTexture che sostituisce quasi
		// integralmente la funzione originaria
		// in EDI c'e' l'indizio dell'header tga
		// se tutto ok deve restituire 0 altrimenti 1
		mov		eax, 56C4F8h    ;Ptr_TextureRawImage
		mov		eax, dword ptr [eax]
		push	eax		;memoria dove copiare mappa
		push	edi
		lea		eax, NewInizializzaTexture
		call	eax
		add		esp,8
		retn

	}
}

__declspec(naked) void RoomPatch_3D(void)
{
	__asm {
		// visualizza info room riguardo selected box
		// esi = record room
		// ecx = buffer dove salvare testo
		pushad


		push	ecx  ;buffer che riceve testo
		push	esi  ;record room
		lea		eax, ShowInfoSelezione
		call	eax
		add		esp,8
		popad
		retn


	}
}

__declspec(naked) void RoomPatch_3E(void)
{
	__asm {
		// salva in project il vettore di objectcode array
		// ebx = handle del file
		pushad
		push	ebx
		lea		eax, SalvaObjectCodes
		call	eax
		add		esp,4
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_3F(void)
{
	__asm {
		// visualizza nuove info riguardo statics e items
		// in edi numero di moveables
		// in ecx buffer dove scrivere testo
		pushad
		push	edi  ;numero di moveables
		push	ecx  ;buffer testo
		lea		eax, ShowStatics
		call	eax
		add		esp,8
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_40(void)
{
	__asm {
		// sostituisce finestra set trigger type
		pushad
		lea		eax,ShowFinestraTrigger
		call	eax
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_41(void)
{
	__asm {
		// chiamata quando si alloca memoria per caricare un generico file
		// carica una memoria maggiore del richiesto
		// in esi quantita di memoria richiesta
		mov		eax,esi
		add		eax, 49152 ; corrisponde a 256*3*64
		push	eax
		push 01h
		mov eax, 44A617h    ;SubChiediMemoria
		call eax
		mov  edi, eax
		add  esp, 08h
		retn

	}
}

__declspec(naked) void RoomPatch_42(void)
{
	__asm {

	
		mov		eax, 445B00h  ; RilasciaTextureDD:
		call	eax
		
		retn

	}
}

__declspec(naked) void RoomPatch_43(void)
{
	__asm {
		// salva ultimo messaggio passato ainvia messaggio log, anche se
		// finestra log non e' attiva
		lea		eax, [esp+4]
		pushad
		push	eax ;indirizzo stack
		lea		eax, SalvaUltimoMsg
		call	eax
		add		esp,4
		popad
		retn
		

	}
}

__declspec(naked) void RoomPatch_44(void)
{
	__asm {
		// riceve in ecx l'indice tail da analizzare
		// e bisogna restituire in eax il valore di texture sonora associata
		push	ecx  ;indice tail
		lea		eax, TrovaTextureSonora
		call	eax
		add		esp,4
		retn

	}
}

__declspec(naked) void RoomPatch_45(void)
{
	__asm {
		// chiamata all'inizio di programma, prima di entrare
		// nel ciclo dei messaggi
		// nota: viene chiamata SOLO se manca autosave.prj
		// o se utente non vuole caricarlo

		pushad
		lea		eax, CaricaOldProgetto
		call	eax
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_46(void)
{
	__asm {
		// chiamata in refreshtutto per ridisegnare il combo box
		pushad
		lea		eax, RinfrescaSchermo
		call	eax
		popad
		retn

	}
}

__declspec(naked) void RoomPatch_47(void)
{
	__asm {
		// controllo in CicloPrincipale per blocco pulsanti
		// rimossa
		retn
		

	}
}

__declspec(naked) void RoomPatch_48(void)
{
	__asm {
		// chiamata all'inizio di ControlloPerAutosave
		// in pratica quando il programma e' pronto a ricevere comandi
		pushad
		lea		eax ,PrimaEsecuzione
		call	eax
		popad
		push  45B3D4h	;  ASCII "rb" ;Mex_rb
		push  45A77Ch	;  ASCII "autosave.prj" ;Mex_autosave_prj
		mov eax, 44A22Fh    ;OpenFile
		call eax
		add  esp, 08h
		retn

	}
}


__declspec(naked) void RoomPatch_49(void)
{
	__asm {
		// chiamata in procedura usata per eliminare un stanza
		// fa controllo se c'e' possibile bug per eliminazaione stanza flippata
		// con oggetti e restitusce c=1 se si deve evitare di eliminare stanza
		// e c=0 se si puo' continuare
		// in edi c'e' l'indice della stanza che sta per essere eliminata
		pushad
		push	edi  // indice stanze
		lea		eax, ControllaDeleteRoom
		call	eax
		add		esp,4
		test	al,al
		jz		VietaEliminazione
		// ok, eliminare stanza
		popad
		// codice eliminato da patch
		mov  eax, edi
		or   ebx, -01h
		shl  eax, 05h
		sub  eax, edi
		lea  esi, [eax][eax*2]
		mov eax, 46B850h    ;Ptr_VetRoomSlots
		mov  eax, dword ptr [eax]
		clc
		retn
VietaEliminazione:
		popad
		stc
		retn


	}
}

__declspec(naked) void RoomPatch_4A(void)
{
	__asm {
		// controllo per bound room 
		// in esi c'e' record di stanza attuale
		pushad
		push	esi  ;record stanza
		lea		eax,ControllaBound
		call	eax
		add		esp,4
		test	al,al
		jz		AnnullaBound
		popad
		mov edx, 583BC8h    ;RectSelezione2d + 8
		mov  edx, dword ptr [edx]
		mov eax, 583BCCh    ;RectSelezione2d + 0Ch
		mov  eax, dword ptr [eax]
		lea  ebp, [edx+02h]
		clc
		retn
AnnullaBound:
		popad
		stc
		retn

	}
}

__declspec(naked) void RoomPatch_4B(void)
{
	__asm {
		//nuova gestione di  GestioneErroreDirectX
		// in ecx numero dell'errore
		mov  eax, ecx
		test eax, eax
		jl   Salto1
		;nessun errore
		xor  eax, eax
		ret
	Salto1:
		push eax
		mov eax, 4414F0h    ;ErroreDXToMessaggio
		call eax
		pushad

		push	eax  ;testo messaggio
		lea		eax, AnalisiErroreDirectX
		call	eax
		add		esp,4

		popad
		push eax		;in eax c'e' il testo del messaggio
		push  4615C8h	;  ASCII "ERROR : %s" ;Mex_ERROR_Args
		push 01h
		mov eax, 449870h    ;InviaMessaggioLog
		call eax
		add  esp, 10h
		mov  eax, 01h
		ret


	}
}

__declspec(naked) void RoomPatch_4C(void)
{
	__asm {
		// visualizza altri testi su schermo

		pushad
		// aggiunge altri testi allo schermo
		lea   eax, AggiuntiTestoSchermo
		call  eax

		popad
		push  4596F0h	;  ASCII "Colour" ;Mex_Colour
		push 2ceh
		push 0f1h
		mov eax, 444020h    ;ShowTestoSuSchermo
		call eax
		add  esp,0ch
				
		retn

	}
}

__declspec(naked) void RoomPatch_4D(void)
{
	__asm {
		pushad
	//togliere finestra attiva
	mov eax, 461B98h   ;NumeroFinestraAttiva
	mov  dword ptr [eax], 00
		popad
		// patch per ripristinnare finestra edit object

		push edx
		push  45B434h	;  ASCII "Floor Type" ;Mex_Floor_Type
		mov eax, 435830h    ;VisualizzaFinestraElenco
		call eax
		add  esp, 08h
		pushad
	mov eax, 461B98h   ;NumeroFinestraAttiva
	mov  dword ptr [eax], 01
		popad

		retn


	}
}

__declspec(naked) void RoomPatch_4E(void)
{
	__asm {
		// visualizza lista effetti
		// se viene restituito -1 restituisce c=1
		// altrimenti c=0
		mov eax, 56F48Ch    ;TotSoundEffects
		mov  eax, dword ptr [eax]
		push  eax  ;tot sounds
		mov eax, dword ptr [pZonaVetList]
		push eax     ;era: 563F80h;  // ZonaListaElenco
		lea		eax, ShowListaSounds
		call	eax
		add		esp,8
		test  eax,eax
		jns   OkSound
		// e' stato scelto cancel
		xor   eax,eax
		retn
OkSound:
		push 4c00h   ;tipo sound
		mov  esi, eax    ;indice
		mov eax, 416C20h    ;AttendeImmissioneEffetto
		call eax
		add  esp,4
		retn

	}
}

__declspec(naked) void RoomPatch_4F(void)
{
	__asm {
		// chiamata da procedura che conta i moveable.
		// se trova un indice sballato lo sostituisce con quello
		// di lara e avvisa l'utente
		// ebx = record di room attuale
		// esi = record di VetBaseOggetti
	movsx  eax, word ptr [esi+0eh]  ;indice slot

	mov edx, 46B818h    ;Ptr_VetSlotOggetti
	mov  edx, dword ptr [edx]
	// ora effettuare controllo
	pushad
	push  esi ; record vetbaseoggetti
	push  ebx ; ptr room attuale

	lea   eax, ControllaSlotOggetto
	call	eax
	add		esp, 8
	lea   ecx, RetValue
	mov   dword ptr [ecx],eax

	popad
	// usa come indice slot quello restituito da funzione
	lea  eax, RetValue
	mov  eax, dword ptr [eax]

	mov  ecx, eax
	shl  ecx, 04h
	add  ecx, eax
	mov  ax, [edx+04h][ecx*8]  ;tipo   ;qui avviene il crash

		retn


	}
}

__declspec(naked) void RoomPatch_50(void)
{
	__asm {
		// chiamata quando si clicka in pulsante di tipo tex box
		// restituisce  c=1 se bisogna abortire (mouse non e' incluso)
		// nello stack ci sono gia' i parametri:
		// orgx, orgy, sizex, sizey
		// in esi c'e' l'indirizzo del pulsante attuale
		cmp  esi, 459570h  ;DatiPulsante_ZonaTestoNomeRoom
		jnz	  NormalPulsante
		// usare procedura personalizzata
		push esi
		lea  eax, PulsanteNomeRoom
		call eax
		add  esp,14h
		jmp  UscireProc
NormalPulsante:
		mov eax, 434000h    ;IsMouseInRettangolo
		call eax
		add  esp,10h
		test eax, eax
		jz  UscireProc
		// proseguire
		mov  eax, 4340ACh ;gestire pulsante text in modo normale
		jmp  eax
UscireProc:
		mov  eax, 4340e7h ;esce daprocedure con eax =0
		jmp  eax

	}
}

__declspec(naked) void RoomPatch_51(void)
{
	__asm {
		// controlla stanza prima di FaiQualcosaConDoor
		// per vedere se pgridcollision e' presente
		pushad
		lea   eax, ControllaGridRoomAttuale
		call  eax
		popad
		mov esi, 46B814h    ;Ptr_VetBaseOggetti
		mov  esi, dword ptr [esi]
		lea  eax, dword ptr [00h][ebx*8]
		sub  eax, ebx
		retn
	

	}
}
// chiamata alla fine di outputwad
__declspec(naked) void RoomPatch_52(void)
{
	__asm {
		// salva in vettori globali i vettori 
		// di remap per oggetti e room
		pushad
		lea   eax, CopiaVettoriRemap
		call  eax

		call  EliminaPaludeAcqua
		popad
		// codice eliminato da patch
		push ebp
		mov eax, 44A07Bh    ;ChiudiFile
		call eax
		mov eax, 46BEBCh    ;HandleFileLogPerTom
		mov  eax, dword ptr [eax]
		add  esp, 04h

		retn

	}
}



__declspec(naked) void RoomPatch_53(void)
{
	__asm {
		// sostiuisce AttendiFineInputMouse

	pushad


	mov eax, 46DA88h   ;PulsantiMouse
	cmp  word ptr [eax], 00h
	jz   Malto2
	

	call GosubFineInputMouse

	

Malto2:
	popad
	ret


	}
}

__declspec(naked) void RoomPatch_54(void)
{
	__asm {
		// ignora trigger speciali per differenza timer
		// se il trigger what e' ACTION o FLIPEFFECT
		// se e' un trigger specialee restituisce c=1
		// se e' un trigger normale c=0
		cmp  byte ptr [esi], 00h   ;tipoAttivazione.Trigger
		jnz  go_TriggerNonDiBase
		cmp  word ptr [esi+06h], 00h   ;Buttons
		jnz  go_TriggerNonDiBase	
		// finora e' un trigger comune	

		cmp  word ptr [esi+04h], 00h  ;Timer
		jnz  DiversoTimer
		// tutto uguale
TriggerUguale:
		clc
		retn
		 // il timer e' diverso ma andra' ignorato 
		 // se il trigger what e' flipeffect o action
DiversoTimer:
		cmp word ptr [esi+08h], 9   ;FLIPEFFECT
		jz   TriggerUguale
		cmp word ptr [esi+08h], 0bh  ;ACTION
		jz   TriggerUguale
go_TriggerNonDiBase:
		stc
		retn
		

	}
}

__declspec(naked) void RoomPatch_55(void)
{
	__asm {
		// salva valore oggetto e triggerwhat in output floor data per trigger
		// in esi c'e' struttura di trigger da cui prendere trigger what
		// e value
		mov  ax, [esi+08h]  ;TriggerWhat
		mov  cx, [esi+02h]  ;OggettoDaTriggerare (o indice di qualcosa)
	
		cmp ax, TO_OBJECT
		jz  Analizza
		cmp ax, TO_CAMERA
		jz  Analizza
		cmp ax, TO_TARGET
		jz  Analizza
		cmp ax, TO_ACTION
		jz  Analizza
		cmp ax, TO_FLYBY
		jnz  Salta
Analizza:
		pushad

		push ecx
		call AnalisiTriggerFlyBy
		add esp,4
		mov word ptr [NuovoIndiceObj], ax
		popad
		mov cx, word ptr [NuovoIndiceObj]
Salta:
		and cx, 03ffh  ;maschera 
		shl  ax, 0ah
		add  cx, ax
		mov eax, 46BEC4h    ;Ptr_MemoriaPerFloorData
		mov  eax, dword ptr [eax]
		xor  edx, edx
		retn

	}
}

__declspec(naked) void RoomPatch_56(void)
{
	__asm {
		// patch per chiamare nuova procedura SaveAnimatedTexture
		// con lo scopo di ordinare i range texture sulla base del criterio
		// uv rotating
		// in ECX handle di file tom aperto
		lea   eax, SalvaTextureAnimate
		push  ecx
		call  eax
		add esp,4
		retn



	}
}

__declspec(naked) void RoomPatch_57(void)
{
	__asm {
		// patch per chiamare funzione prpria di inizializzazione 
		// prima del preview
		lea   eax, InitPreviewRoom
		call  eax
		mov  eax,1
		retn

	}
}

__declspec(naked) void RoomPatch_58(void)
{
	__asm {
		// chiamata al termine di preview, usata per rirpistinare 
		// i valori di tail info modificate da scroll terxture
		lea   eax, EndPreviewRoom
		call  eax
		retn

	}
}

__declspec(naked) void RoomPatch_59(void)
{
	__asm {
		; in ESI c'e' record room che sta per essere salvatra adesso
		pushad

		push esi
		lea  eax, SalvaWaterIntensity
		call  eax
		add esp,4
		
		popad
		// codice eliminato da patch
		push ebp 
		push 01h 
		lea  ecx, [esi+5ah]  ;XPixelWorld 
		push 04h 
		push ecx 
		mov eax, 44AD68h    ;fWrite
		call eax 
		mov  eax, 42E5F0h ;salto a istruzione successiva in patch
		jmp  eax

	}
}
// controlla confronto versione tra object wad e project
// in edx = versione wad
// in ecx = versione object.h
__declspec(naked) void RoomPatch_5A(void)
{
	__asm {
		cmp edx, 0x82
		jnz  FaiControllo
		mov eax,1
		retn
FaiControllo:
		mov eax, edx

		push ecx   ;versione objects.h 
		push eax  ;versione wad file 
		push  45B2C8h	;  ASCII "WARNING: Version %d meshes, version %d objects.h" ;Mex_WARNDiverseVersioniMeshAndObject 
		push  56F380h ;BufferErrore 
		mov eax, 449F64h    ;sprintf
		call eax 
		push 0fh 
		push ebp 
		push ebp 
		push  56F380h ;BufferErrore 
		mov eax, 432F60h    ;auto_req
		call eax 
		add  esp, 20h 
		retn
 

	}
}
// trovate texture, se siamo in modalita' find & replace
// sostituirla
// in edx c'e' il valore da copiare in eax che corrisponde
// a primo indice di texture da cercare
// se NON siamo in modalita' Find & replace, 
//		resittuisce c=1 se la textue e' stata trovatra, 
//		mentre resituisce c=0 se non e' presente 
// se siamo in modalita find & replace resituisce sempre c=0 
// per continuare ricerca
// ecx = record StrTextureInfo  attualmente esplorato
// 
__declspec(naked) void RoomPatch_5B(void)
{
	__asm {
		mov eax, edx
		cmp byte ptr [DatiFindReplace.TestAttivo], 0
		jz CodiceNormale
		// elaborare i dati trovati

		pushad
		push 569E80h //  VetIndiciTailDaCercare
		push ecx // record textureinfo trovato
		call AnalisiTexTrovata
		add esp,8
		popad
		clc
		retn
		// codice rimosso da patch.
		// cerca normalmente texture
CodiceNormale:
		mov  dx, [ecx+02h]  ;indicetail 
		mov  ecx,  569E80h ;VetIndiciTailDaCercare 
		and  dh, 03h   ;	;questo corrisponde and and dx, 0x3ff 
findTex_LoopTexSurface: 
		cmp  dx, ax 
		jz   findTex_TrovataTexture 
		mov  ax, [ecx+02h] 
		add  ecx, 02h 
		cmp  ax, 0ffffh 
		jnz  findTex_LoopTexSurface 
		// non trovata
		clc
		retn
findTex_TrovataTexture:
		stc
		retn

	}
}
// terminata scansione di livello in find textures (o find and replace)
__declspec(naked) void RoomPatch_5C(void)
{
	__asm {

	mov eax, 45C25Ch   ;LOC_45c25c
	mov  dword ptr [eax], edi 

	mov eax, 45C258h   ;LOC_45c258
	mov  dword ptr [eax], edi 

	mov eax, 45C254h   ;LOC_45c254
	mov  dword ptr [eax], edi 

	mov eax, 45C260h   ;IndiceUltimaRoomTrovata
	mov  dword ptr [eax], edi 


		cmp byte ptr [DatiFindReplace.TestAttivo], 0
		jz  CodiceNormale
	pushad
	call FineFindReplace
	popad
	mov ebp, 0
	jmp  Continua
CodiceNormale:
	push 03h 
	push ebp 
	push ebp 
	push  45C81Ch	;  ASCII "Texture not found!" ;Mex_Texture_not_found 

	mov eax, 432F60h    ;auto_req
	call eax 
	add esp,10h
Continua:
	mov eax, 426090h    ;Edit2Floor
	call eax 


	mov eax, 583BBCh   ;TestEsisteSelezioneBox
	mov  dword ptr [eax], ebp 

	mov eax, 583BB6h   ;TipoFrecciaSelezione
	mov  dword ptr [eax], ebp 

	mov eax, 583BA4h   ;IndiceItemSelezionato
	mov  dword ptr [eax], edi 

	mov eax, 410120h    ;HighlightFloor
	push ebp 
	call eax 
	add  esp, 4h 
	retn
	}
}
// ennesima patch per il ricalcolo di indice ordinale sulla base
// di distanza da inizio mappa tga
__declspec(naked) void RoomPatch_5D(void)
{
	__asm {

		movsx  eax, cx 
		cdq 
		cmp  dword ptr [VersioneProject], 49
		jz  Size64
		cmp dword ptr [VersioneProject], 50
		jz Size128
		cmp dword ptr [VersioneProject],51
		jz Size256

Size64:

		and  edx, 3fh   ; and 63 

		add  eax, edx 
		mov  ecx, eax 
		movsx  eax, si 
		cdq 
		and  edx, 3fh  ;and 63 
		add  eax, edx 
		sar  ecx, 06h 
		sar  eax, 06h 
		;e poi moltiplica per 4 le righe y 
		lea  eax, [ecx][eax*4] 
		retn
		// codice nel caso texture siano 128x128
Size128:
		and  edx, 7fh   
		add  eax, edx 
		mov  ecx, eax 
		movsx  eax, si 
		cdq 
		and  edx, 7fh   
		add  eax, edx 
		sar  ecx, 07h 
		sar  eax, 07h 
		;e poi moltiplica per 2 le righe y 
		lea  eax, [ecx][eax*2] 
		retn
		// codice per texture 256 x 256
Size256:
		and  edx, 0ffh   
		add  eax, edx 
		mov  ecx, eax 
		movsx  eax, si 
		cdq 
		and  edx, 0ffh   
		add  eax, edx 
		sar  ecx, 08h 
		sar  eax, 08h 
		;e poi moltiplica per 1 le righe y in pratica basta gia' eax
		retn
	}
}
// azzera indicie di static relativo a staza attuale
__declspec(naked) void RoomPatch_5E(void)
{
	__asm {
		mov dword ptr [IndiceStaticNow], 0
		retn

	}
}
// salva indice static in vettore conversione
__declspec(naked) void RoomPatch_5F(void)
{
	__asm {
		mov eax, ebp
		and eax, 0ffffh
		movsx  ecx, word ptr [ebx+0eah][eax*2]
		// ok, ora in ecx c'e' indice ngle
		pushad
		push ecx //indice ngle
		call SalvaIndiceStatic
		add esp,4
		popad
		// codice originale eliminato da patch
	push edi 
	push 01h 
	lea  eax, [esi+28h] 
	push 02h 
	push eax 
	mov eax, 44AD68h    ;fWrite
	call eax 
	mov eax, 431EF1h // torna a codice originale
	jmp eax

	}
}
// chiamata in fase di scritttura flor data per i trigger
__declspec(naked) void RoomPatch_60(void)
{
	__asm {
	mov eax, 569D60h    ;Ptr_VetTriggers
	mov  eax, dword ptr [eax] 
	cmp  word ptr [eax+08h][ecx*2], 00h  ;TRIGGER.CosaAttivare (!!!!) 
	lea  esi, [eax][ecx*2] 
	jz  GoOggetto
	cmp word ptr [eax+08h][ecx*2], 01h  ;CAMERA
	jz  GoCamera

	// non oggetto, non camera
	mov eax, 430264h ; go_TriggerNonPerCamera:
	jmp eax
GoOggetto:
	mov eax, 430226h
	jmp eax
GoCamera:
	mov eax, 430248h
	jmp eax
	}
}
// patch per trigger ->object aqnche per camera e flyby trigger
__declspec(naked) void RoomPatch_61(void)
{
	__asm {
	mov eax, 569D60h    ;Ptr_VetTriggers
	mov  eax, dword ptr [eax] 
	mov di, [eax+08h][ecx*2]

	lea  eax, [eax][ecx*2] 
	cmp  di, 0
	jz  CercaOggetto
	cmp di, 1 // triggerwhat = camera
	jz  CercaOggetto

	// non disponibile
	xor edi,edi
	mov eax, 4244FBh
	jmp eax
CercaOggetto:
	xor edi,edi
	mov ecx, 424539h  // go_CercaOggetto
	jmp ecx



	}
}
// messaggio d'errore per unknown object
__declspec(naked) void RoomPatch_62(void)
{
	__asm {
		// migliorare messaggio
		pushad
		call MessaggioEvilCord
		mov  [AdrResult], eax
		popad
	push 00h	;  Cases 0,3,-4,-1 of switch 00421245 
	push 00h 
	push 00h 
	mov eax, dword ptr [AdrResult]

	push  eax	;  ASCII "EVIL: Unknown object type detected" ;Mex_EVIL_Unknow_object_type 
	mov eax, 432F60h    ;auto_req
	call eax 
	add  esp, 10h 
	mov eax, -1
	retn

	}
}
// patch persalvare stanza e coordiante di zona scandita per oggetto o trigger
__declspec(naked) void RoomPatch_63(void)
{
	__asm {
		mov eax, dword ptr [esp+04h] //  cordx
		mov ecx, dword ptr [esp+08h] // cordz
		mov dword ptr [ScanCordX], eax
		mov dword ptr [ScanCordZ], ecx

		mov eax, 583BA0h    ;IndiceRoomAttuale
		mov eax, dword ptr [eax]
		mov dword ptr [ScanRoom], eax

		// codice originale rimosso da patch
	push ecx	; INIZIO PROCEDURA 352 ;  SIZE=0x230 
	mov ecx, 583BA0h    ;IndiceRoomAttuale
	mov  ecx, dword ptr [ecx] 
	push ebx 
	mov  eax, ecx 
	push ebp 
	mov edx, 42363Bh
	jmp edx

	}

}
// patch chesostituisce fase di lettura da file aperto 
// di percorso di file wad (di solito .swd) 
// in ebp l'handle del file gia' aperto
// se tutto va bene copia il nome del file  in NomeFileWas
// se invece trova "" o "NA" o utente rifiuta di caricare un wad
// restituisc c=1 e non scrive niente in NomeFileWas

__declspec(naked) void RoomPatch_64(void)
{
	__asm {

		pushad
		push ebp  // FILE *pFile
		push 56C7A0h // NomeFileWas
		call CatturaPercorsoWad
		add esp, 8
		mov  dword ptr [ValEsito], eax
		popad

		cmp byte ptr [ValEsito], 0
		jz  Abortisci
		// ok
		clc
		retn
Abortisci:
		stc
		retn

	}
}
//  chiamata per carica direttamente da file su disco
// il percorso del file .tga
// in ebp c''e FILE
// se il percorso non c'e' restituisce c=1
// se va tutto bene copia il percorso in NomeFile_Tga
__declspec(naked) void RoomPatch_65(void)
{
	__asm {

		pushad
		push ebp  // FILE *pFile
		push 46BA78h  // NomeFile_Tga
		call CatturaPercorsoTga
		add esp, 8
		mov  dword ptr [ValEsito], eax
		popad

		cmp byte ptr [ValEsito], 0
		jz  Abortisci
		// ok
		clc
		retn
Abortisci:
		stc
		retn

	}
}
// patch chiaamta in fase output was, dopo la creazione dei vertci
// ma un attimo prima di elaborare gridcollision
// e' usata per creare collisioni traingolari.
__declspec(naked) void RoomPatch_66(void)
{
	__asm {
		pushad
		call PreparaCollisioni
		popad

	push 00h 
	push ebx  ;vetindicioggetti (1 word per ogni oggetto) 
	push edi  ;vetpointer room 
	push ebp  ;handle file .tom 
	mov eax, 42F2B0h    ;SaveRawFloorData
	call eax    ;SalvaCollisionData 
	;ora salvare collision data e porte verticai 
	push 01h 
	push ebx 
	push edi 
	push ebp 
	mov eax, 42F2B0h    ;SaveRawFloorData
	call eax 
		pushad
		call RestoreCollisioni
		popad
	mov eax, 42E713h
	jmp eax

	}
}
// patch per consentire di spostare pannelli collisione sotto la coordinata 0
__declspec(naked) void RoomPatch_67(void)
{
	__asm {
	cmp  word ptr [esi+0ah], 00h 
	jge  Salto1 
	// sarebbe da azzerare ma se questo item e 'un moveable
	// ed e' un pannello savegame allora non azzerarlo
	cmp word ptr [esi], 8
	jnz  Azzera
	mov ax, word ptr [esi+0eh] // slotid
	cmp ax, PANEL_START
	jl  Azzera
	cmp ax, PANEL_END
	jle Salto1
Azzera:
	mov  word ptr [esi+0ah], 00h 
Salto1: 
	retn
	}
}

__declspec(naked) void RoomPatch_68(void)
{
	__asm {

		pushad
		call SalvaNomiSlot
		popad
	push eax
	mov eax, 56C794h   ;TotSprites
	mov  dword ptr [eax], ebx 
	pop eax
	push eax
	mov eax, 56C790h   ;N_MeshPointers
	mov  dword ptr [eax], ebx 
	pop eax
	retn

	}
}

__declspec(naked) void RoomPatch_69(void)
{
	__asm {
		pushad
		call ScansioneButtonsWindow
		popad
	mov eax, 4697D8h    ;HandleScreen
	mov  eax, dword ptr [eax] 
	mov  edx, [eax+16h] 
	movsx  eax, word ptr [eax+06h] 
	retn
	}
}

// patch a inizio di ButtonText per salvare accoppiata di testo e datipulsante
__declspec(naked) void RoomPatch_6A(void)
{
	__asm {
		cmp byte ptr [BaseWindow.TestStopMemo], 0
		jnz Ignora
		mov eax, [esp+4]  // datipulsanti
		mov ecx, [esp+8]  // testo
		pushad
		push eax //dati pulsanti
		push ecx // nome pulsante
		call CatturaPulsante
		add esp,8
		popad
Ignora:
	push edi	; INIZIO PROCEDURA 180 ;  SIZE=0x60 
	mov  edi, [esp+08h]  ; (ARG_ESP + 0x4) 
	mov  eax, [edi+0eh] 
	test eax, eax 
	mov ecx, 4125AAh
	jmp ecx


	}
}

__declspec(naked) void RoomPatch_6B(void)
{
	__asm {

	// codice eliminato da patch
	mov  al, [esp+0ch] 	; Arg3: Colore Testo	; INIZIO PROCEDURA 195 ;  SIZE=0xD0  ; (ARG_ESP + 0xC) 
	mov  ecx, [esp+08h]  	; Arg4: Colore fondo  ; (ARG_ESP + 0x8) 
		// forzare il nuovo colore di fondo
		mov edx, [esp+04h]  // datipulsante
		mov byte ptr [edx+17h], cl
	push ebx 
	; lightning corona size 
	push esi 
	mov ebx, 41491Ah
	jmp ebx
	}
}

void AnalisiInReadMouse(void)
{
	
	static HWND *pMiaWind = (HWND*) 0x46E559;
	static int *pDatiImmagineMouse = (int *) 0x46D15C;
	


	if (*pDatiImmagineMouse != DatiMouseStandard) {
		// mouse e' strano
		if (BaseWindow.TestSospesa==false && BaseWindow.hWind != 0) {
			ShowWindow(BaseWindow.hWind, SW_HIDE);
			BaseWindow.TestSospesa=true;
			//GlobNgle.TestAttendiFineMouse=false;
		}

	}else {
		if (BaseWindow.TestSospesa) {
			BaseWindow.TestSospesa=false;
			ShowWindow(BaseWindow.hWind, SW_SHOWNA);
		}
	}


}

__declspec(naked) void RoomPatch_6C(void)
{
	__asm {
		pushad
		call AnalisiInReadMouse
		popad
		cmp byte ptr [BaseWindow.MioComandoInCorso], 0
		jz Normale
	pushad
	call LeggiPeekMouse
	popad
Normale:
	sub  esp, 08h	; INIZIO PROCEDURA 807 ;  SIZE=0x50 
	lea  eax, [esp+00h]  ; (LOCALE_ESP - 0x8) 
	push eax	; /pPoint 
	mov eax, 4551E4h    ;GetCursorPos
	call dword ptr [eax]	; \GetCursorPos 
	mov edx, 44499Eh
	jmp edx

	}
}

__declspec(naked) void RoomPatch_6D(void)
{
	__asm {
		mov ax, word ptr [SimulaKey]
		or ax, word ptr [SimulaStatusKey]
		test ax,ax
		jz Niente
		// inserire simulazione tasti
		pushad
		mov eax, 46DF98h // VetInputKeyboard
		push eax
		call GestioneSimulaTasto
		add esp,4
		popad
		mov word ptr [SimulaKey], 0
		mov word ptr [SimulaStatusKey], 0

Niente:
		pushad

;---- Legge comandi tastiera

		call  LeggeComandiTasti


		popad

	// codice eliminato da patch
	mov edx, 46E050h    ;TastoSCROLL_ALT_Rilasciato
	mov  dl, byte ptr [edx] 
	mov ecx, 46D704h    ;TotTastiNelBuffer
	mov  ecx, dword ptr [ecx] 
	retn
	}
}

__declspec(naked) void RoomPatch_6E(void)
{
	__asm {
		mov eax, ecx // ripristinare eax
	lea  ecx, dword ptr [00h][edx*8] 
	sub  ecx, edx 
	mov edx, 46B814h    ;Ptr_VetBaseOggetti
	mov  edx, dword ptr [edx] 
	lea  ecx, [ecx][ecx*2] 
	mov  [edx+0eh][ecx*2], ax 
	call Draw3DWindow
	retn
	}
}

__declspec(naked) void RoomPatch_6F(void)
{
	__asm {
		cmp byte ptr [BaseWindow.MioComandoInCorso], 0
		jz Normale
	pushad
	mov eax, 4440D0h  // LeggiDirectInput
	call eax
	popad
Normale:
	lea  edx, [esp+08h]  ; (LOCALE_ESP - 0x4) 
	lea  eax, [esp+00h]  ; (LOCALE_ESP - 0xC) 
	push edx 
	lea  ecx, [esp+08h]  ; (LOCALE_ESP - 0x8) 
	push eax 
	push ecx 
	mov eax, 419FF0h    ;GetPosition
	call eax 
	add  esp, 0ch 
	mov ecx, 419FD4h
	jmp ecx
	}
}


__declspec(naked) void RoomPatch_70(void)
{
	__asm {
	pushad
	// impostare puntatore mouse con forma di mirno
	push 45A610h ;Dati_MostraTarget
	push 04h 
	push 04h 
	push 0ah 
	push 0ah 
	mov eax, 436A64h    ;init_pointer
	call eax 
	mov eax, 436AEEh    ;plot_mouse
	call eax 
	add esp, 5*4
	popad
	push ebx 
	mov eax, 410120h    ;HighlightFloor
	call eax 
	lea  eax, [esp+10h] 
	mov ecx, 422E21h // ritorno a codice
	jmp ecx
	
	}
}
// patch per rimuovere puntatore moiuse alterato per place object
__declspec(naked) void RoomPatch_71(void)
{
	__asm {
	pushad
	mov eax, 436CB7h    ;rem_mouse
	call eax 
	mov eax, 437A71h    ;DefPointer
	call eax 
	mov eax, 436AEEh    ;plot_mouse
	call eax 
	popad
	mov eax, 401200h    ;Draw3DWindow
	jmp  eax 
	}
}

// patch per cambiare puntatore mouse quando si muove oggetto
__declspec(naked) void RoomPatch_72(void)
{
	__asm {
	pushad
	// impostare puntatore mouse con forma di mirno
	push 45A610h ;Dati_MostraTarget
	push 04h 
	push 04h 
	push 0ah 
	push 0ah 
	mov eax, 436A64h    ;init_pointer
	call eax 
	mov eax, 436AEEh    ;plot_mouse
	call eax 
	add esp, 5*4
	popad

	push 00h 
	mov eax, 410120h    ;HighlightFloor
	call eax 
	lea  eax, [esp+10h] ;era +0c
	lea  ecx, [esp+0ch] ;era +08
	push eax 
	lea  edx, [esp+0ch] ;era +8h
	push ecx 
	push edx 
	mov eax, 419FF0h    ;GetPosition
	call eax 
	add  esp, 10h
	retn
	}
}
// ripristina puntatore mouse dopo spostamento oggetto
__declspec(naked) void RoomPatch_73(void)
{
	__asm {
	pushad
	mov eax, 436CB7h    ;rem_mouse
	call eax 
	mov eax, 437A71h    ;DefPointer
	call eax 
	mov eax, 436AEEh    ;plot_mouse
	call eax 
	popad
	;codice eliminato da patch
	mov eax, 583BA4h    ;IndiceItemSelezionato
	mov  eax, dword ptr [eax] 
	push eax 
	mov eax, 422F20h    ;PositionObject
	call eax 
	mov eax, 422D37h  ;riotrno a codice
	jmp eax

	}
}
// patch chiamata prima di salvare project
__declspec(naked) void RoomPatch_74(void)
{
	__asm {

	pushad
	call IsToDisableRemoveItems
	popad

	mov eax, 583BF0h    ;TestMostraFlipMap
	mov  eax, dword ptr [eax] 
	test eax, eax 
	jz   Salto1 
	;toglie flipmap 
	push 00h 
	mov eax, 40E130h    ;Menu_FlipMap
	call eax 
	add  esp, 04h 
Salto1: 
	retn
	}
}

// patch chiamata all'inizio di pulsante place object

__declspec(naked) void RoomPatch_75(void)
{
	__asm {

		pushad
		call IsToDisableRemoveItems
		popad
	mov ecx, 583BA0h    ;IndiceRoomAttuale
	mov  ecx, dword ptr [ecx] 
	mov  eax, ecx 
	shl  eax, 05h 
	retn
	}
}
// patch chiamata prima di piazzare trigger nella mappa
__declspec(naked) void RoomPatch_76(void)
{
	__asm {
		pushad
		call IsToDisableRemoveItems
		popad
	mov ecx, 583BA0h    ;IndiceRoomAttuale
	mov  ecx, dword ptr [ecx] 
	mov esi, 583BC4h    ;RectSelezione2d + 4
	mov  esi, dword ptr [esi]
	retn
	}
}
__declspec(naked) void RoomPatch_77(void)
{
	__asm {
		// se e' attiva setting di nascondere oggetti e siamo in drawdoor
		// abortire subito procedura
		test word ptr [MyPrefRoom.Flags], FPR_HIDE_OBJ_DRAW_DOORS
		jz  Normale
		mov eax, 583BECh // TestDrawDoors
		cmp byte ptr [eax], 0
		jz Normale
		retn
		
Normale:
	sub  esp, 24h	; INIZIO PROCEDURA 23 ;  SIZE=0x3A0 
	push edi 
	mov edi, 583BDCh    ;TestFaceEdit
	mov  edi, dword ptr [edi] 
	mov eax, 4039EAh
	jmp eax
	}
}
// gestione errore di device directx non trovato (accade su windows 10 )

__declspec(naked) void RoomPatch_78(void)
{
	__asm {
		// visualizzare message box
		pushad
		call MostraErroreNotFoundDevice
		popad
		// codice eliminato
	push  461A80h	;  ASCII "No Matching Device Found" ;Mex_No_Matching_Device_Found 
	push 01h 
	mov eax, 449870h    ;InviaMessaggioLog
	call eax 
	add  esp, 08h 
	// fine codeice eliminato
	retn
	}
}

void AggiornaNomeStanza(void)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;
	static DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;

	roomShowNomeRoom(pVetRooms[*pIndiceRoomAttuale].Nome, 
			pVetRooms[*pIndiceRoomAttuale].FlipMapValue);
}

void RealizzaPalette(void)
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	HDC RoomHdc;

	RoomHdc = GetDC(*pMiaWind);
	RealizePalette(RoomHdc);

}
// patch chiamata con jump da Draw2DWindow() per visualizzare il nome della stanza
__declspec(naked) void RoomPatch_79(void)
{
	__asm {
		pushad
		call AggiornaNomeStanza
		popad

		// codice eliminato da patch
	mov ecx, 583BA0h    ;IndiceRoomAttuale
	mov  ecx, dword ptr [ecx]	 
	push ebx 
	mov  eax, ecx 
	push ebp 
	mov edx, 40FD8Ah // ritorno al codice
	jmp edx
	}
}

void *VetRoomPatch_Salti[] = {
		&RoomPatch_00, &RoomPatch_01, &RoomPatch_02, &RoomPatch_03, 
		&RoomPatch_04, &RoomPatch_05, &RoomPatch_06, &RoomPatch_07, 
		&RoomPatch_08, &RoomPatch_09, &RoomPatch_0A, &RoomPatch_0B, 
		&RoomPatch_0C, &RoomPatch_0D, &RoomPatch_0E, &RoomPatch_0F, 
		&RoomPatch_10, &RoomPatch_11, &RoomPatch_12, &RoomPatch_13, 
		&RoomPatch_14, &RoomPatch_15, &RoomPatch_16, &RoomPatch_17, 
		&RoomPatch_18, &RoomPatch_19, &RoomPatch_1A, &RoomPatch_1B, 
		&RoomPatch_1C, &RoomPatch_1D, &RoomPatch_1E, &RoomPatch_1F, 
		&RoomPatch_20, &RoomPatch_21, &RoomPatch_22, &RoomPatch_23, 
		&RoomPatch_24, &RoomPatch_25, &RoomPatch_26, &RoomPatch_27, 
		&RoomPatch_28, &RoomPatch_29, NULL, NULL, 
		NULL, NULL, NULL, NULL, 
		&RoomPatch_30, &RoomPatch_31, &RoomPatch_32, &RoomPatch_33, 
		&RoomPatch_34, &RoomPatch_35, &RoomPatch_36, &RoomPatch_37, 
		&RoomPatch_38, &RoomPatch_39, &RoomPatch_3A, &RoomPatch_3B, 
		&RoomPatch_3C, &RoomPatch_3D, &RoomPatch_3E, &RoomPatch_3F, 
		&RoomPatch_40, &RoomPatch_41, &RoomPatch_42, &RoomPatch_43, 
		&RoomPatch_44, &RoomPatch_45, &RoomPatch_46, &RoomPatch_47, 
		&RoomPatch_48, &RoomPatch_49, &RoomPatch_4A, &RoomPatch_4B, 
		&RoomPatch_4C, &RoomPatch_4D, &RoomPatch_4E, &RoomPatch_4F, 
		&RoomPatch_50, &RoomPatch_51, &RoomPatch_52, &RoomPatch_53, 
		&RoomPatch_54, &RoomPatch_55, &RoomPatch_56, &RoomPatch_57, 
		&RoomPatch_58, &RoomPatch_59, &RoomPatch_5A, &RoomPatch_5B,
		&RoomPatch_5C, &RoomPatch_5D, &RoomPatch_5E ,&RoomPatch_5F, 
		&RoomPatch_60, &RoomPatch_61, &RoomPatch_62, &RoomPatch_63,
		&RoomPatch_64, &RoomPatch_65, &RoomPatch_66, &RoomPatch_67,
		&RoomPatch_68, &RoomPatch_69, &RoomPatch_6A, &RoomPatch_6B,
		&RoomPatch_6C, &RoomPatch_6D, &RoomPatch_6E, &RoomPatch_6F,
		&RoomPatch_70, &RoomPatch_71, &RoomPatch_72, &RoomPatch_73,
		&RoomPatch_74, &RoomPatch_75, &RoomPatch_76, &RoomPatch_77,
		&RoomPatch_78, &RoomPatch_79
};


// patch generiche per room eidtor
__declspec(naked) void PatchGenericheRoom(void)
{
	__asm {
		and eax, 0xffff
		jmp dword ptr VetRoomPatch_Salti[eax*4]

	}
				
}
void ShowMexUguale(void)
{
	InviaLog("\tUGUALE: (perche' ha stesso indicetail attuale)");

}

void ShowMexNonUguale(void)
{
	InviaLog("\tDIVERSA (perche' ha indice tail diverso da attuale)");
}

// chiamata nel loop di controllo per texturedd gia' inserite
// in ecx c'e' l'indice attuale di texturedd
// se texture attuale e' diversa restituisce c=1
__declspec(naked) void ControllaTextureDD(void)
{
	__asm {
		// se diverso qualcosa restituire c=1
		cmp  esi, [eax+10h]
		jnz	 Diverso
		cmp	 edi, [eax+14h]
		jnz  Diverso
		;sarebbe tutto uguale
		;se pero' l'indice attuale ecx di texturedd e' diverso
		;da qeullo di vetindex[indicetail] allora e' diverso


#ifdef LOG_TEXTURE
		pushad
		push	ecx		;indicetexturedd
		lea		eax, ShowMexCheckIndex
		call	eax
		add		esp,4
		popad
#endif

		pushad

		lea  edx, IndiceTailNow
		mov  edx, dword ptr [edx]
		cmp  edx, -1
		jnz	Guarda
		// non definito, allora considerarla uguale

		popad
		clc
		retn

Diverso:
		stc
		retn
Guarda:

		lea  ebx, VetIndexTextureDD

		cmp  dword ptr [ebx][ecx*4], edx
		popad
		jnz		AltroDiverso
		;considrarla uguale perche' anche l'indice e' lo stesso
#ifdef LOG_TEXTURE
		pushad
		lea   eax, ShowMexUguale
		call	eax
		popad
#endif
		;indice e' uguale

		clc
		retn
AltroDiverso:

#ifdef	LOG_TEXTURE
		pushad
		lea   eax,ShowMexNonUguale
		call eax
		popad
#endif
		stc
		retn




	}
}

// chiamata quando utente preme la [X] della finestra
// metodo usato per uscire in modo rapido senza rilevare crash 
// ma consentendo il salvataggio delle preferenze ecc.

void ChiudiRoomEdit(void)
{

	__asm {

		mov eax, 563F58h    ;BaseSchermo
		mov  eax, dword ptr [eax]
		push eax	; |Arg1 => 00A55DA0
		mov eax, 43657Eh    ;RilasciaGlobaliStrutturaInfo
		call eax
		/*
		mov eax, 444570h    ;RilasciaRisorse
		call eax
		*/
		lea		eax, EliminaAutoSave
		call	eax

		;e ora chiudere il programma

		push 00h
		push 00h
		push 01h  ;segnala che non ci sono bug
		mov eax, 44A3DEh    ;LOC_44a3de
		call eax
		add		esp,10h
continua:
		jmp		continua
	}

}
// se il valore e' positivo diminuisce scroll_y se e' negativo lo incrementa
// input in ecx valore di incremento oltre che il segno
__declspec(naked) void ScrollaTexture(void)
{
	__asm {
		pushad
		movsx	ecx,cx
		test	ecx,ecx
		js		negativo
		// positivo diminuire
		mov		eax, 583BD0h   ;TextureScroll_Y
		mov		eax, dword ptr [eax]
		sub		eax, ecx

		test	eax,eax
		jns		zompo
		xor		eax,eax
		jmp		zompo
		// incrementare
Negativo:
		neg		ecx
		mov		eax, 583BD0h   ;TextureScroll_Y
		mov		eax, dword ptr [eax]
		add		eax, ecx

		// qui bisognerebbe controllare che superi limite massimo
		cmp		eax, 11588
		jl		zompo
		mov		eax, 11588
Zompo:
		mov		ecx, 583BD0h   ;TextureScroll_Y
		mov		dword ptr [ecx], eax
		mov		ecx, 4595D8h   ;Val_Scroll_Y_Pulsante
		mov		dword ptr [ecx], eax
		push  4595BCh ;DatiPulsante_ScrollTexture
		mov eax, 433E80h    ;GestioneBarraScrollTexture
		call eax
		add  esp, 04h
		mov eax, 42AB00h    ;RefreshZonaTexture
		call eax

		popad
		retn
	}
}
// sulla base di valore assoluto di valoreruota calcola i pixel di scorrimento
short CalcolaIncRuota(short ValoreRuota)
{
	int Numero;
	int Segno;
	int VetIncrementi[10];
	int Step;
	int Valore;
	int i;
	int Divisore;
	int ValSenso;
	static int VetSenso[10] = {1,10,20,40,80,120,160,200,240,300};
	const int TotIncrementi=10;

	if (ValoreRuota > 0) 
		Segno=1;
	else
		Segno=-1;


	ValSenso = VetSenso[MyPrefRoom.MouseSensitivity];
	

	// costruire lista di incrmenti
	Step = (MyPrefRoom.MouseMaxStep - MyPrefRoom.MouseMinStep) / 
			(TotIncrementi-1);
	Valore=MyPrefRoom.MouseMinStep;

	for (i=0;i<TotIncrementi;i++) {
		VetIncrementi[i] = Valore;
		Valore+= Step;
	}
	// calcolo accellerazione
	if (MyPrefRoom.Flags & FPR_NO_ACCELERATION) {
		Numero = 4;
	}else {

		Numero = abs(ValoreRuota);
		Numero -= ValSenso;
		if (Numero <1) Numero=1;

		Divisore = ValSenso / 4;
		if (Divisore < 1) Divisore =1;

		Numero  /= Divisore;

		if (Numero >= TotIncrementi) Numero = TotIncrementi-1;
	}
	return (short) (VetIncrementi[Numero] * Segno);

	
}


// se si resituiwsce FALSE vuol dire che non si deve passare il messaggio
// alla window procedure standard
bool HookMessaggiRoomEditor(HWND hWind, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	HCURSOR *pHandleIconaMouse = (HCURSOR*) 0x493110;
	short Valore;
	HINSTANCE *pHandleIstanzaRoom = (HINSTANCE*) 0x46E555;
	int *pNumFinestraAttiva = (int*) 0x461B98;
	int *pOrgX = (int*) 0x563F68;
	int *pOrgY = (int*) 0x563F6C;
	int xPos, yPos;
	static BYTE *pTastoNumero1 = (BYTE *) 0x46DF99;
	static int *pDatiImmagineMouse = (int *) 0x46D15C;
	/*
	sprintf(BufferLog,"hWind=0x%X uMsg=0x%X  wParam=0x%X  lParam=0x%X  TickCount=%d",
		hWind,uMsg,wParam,lParam, GetTickCount());
	InviaLog(BufferLog);
	
	*/


	SimulaCrash();
	switch (uMsg) {




	case WM_SETFOCUS:
		if (hWind == *pMiaWind) {
			// impostare puntatore mouse
			if (*pHandleIconaMouse == NULL) {
				// impostare mouse clessidra
				SetCursor(CursoreClessidra);
			}else{
				SetCursor(*pHandleIconaMouse);
			}
		}
		// rifrescare finestra SOLO se prima c'era stato comando [Play]
		if (TestPlayInCorso) {
			TestPlayInCorso=false;
			MostraNuovaStanza(LastRoomPlay);
		}

		break;
	
	case WM_LBUTTONDOWN:
		// dare il focus a finestra principale
		if (hWind == *pMiaWind) 
			SetFocus(*pMiaWind);
		SalvaUltimoTempoInput();


		
		break;
	case WM_KEYDOWN:
	case WM_RBUTTONDOWN:
		if (hWind == *pMiaWind) 
					SalvaUltimoTempoInput();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_COMBO_PROGRAMMI:
			switch (HIWORD(wParam)) {
			case CBN_CLOSEUP:

				SetFocus(*pMiaWind);
				break;
			}
		}


		return false;

	case WM_CREATE:
		// creare combobox per lancio programmi esterni
		WindComboProgrammi =  CreateWindow ("combobox",NULL,
				 WS_CHILD | WS_VISIBLE | WS_VSCROLL |				  
				 CBS_DROPDOWNLIST | CBS_HASSTRINGS,
				 426, 710,
				  250, 262,
				 hWind,(HMENU) ID_COMBO_PROGRAMMI,*pHandleIstanzaRoom, NULL);
		
		AggiornaComboProgrammi();




		break;



	case WM_MOVE:
		// se c'e' finestra buttons aggiornare posizione
		xPos = (int)(short) LOWORD(lParam);    // horizontal position 
		yPos = (int)(short) HIWORD(lParam);    // vertical position 
		InviaLog("HookMessaggiRoomEditor()> WM_MOVE");
		if (BaseWindow.TotButtons !=0 && BaseWindow.hWind != NULL) {
			// aggiornare posizione finestra
			// se e' mnimizzata farla aprire
			if (IsIconic(BaseWindow.hWind)){

				ShowWindow(BaseWindow.hWind, SW_RESTORE);
			}


			// ora spostarla
			MoveWindow(BaseWindow.hWind, 
				xPos+MyPrefRoom.ButWndOrgX, yPos+MyPrefRoom.ButWndOrgY,
				BaseWindow.WindSizeX, BaseWindow.WindSizeY, TRUE);


		}
		break;

	case WM_MOUSEWHEEL:
	// case WM_MOUSELAST:
		if (*pNumFinestraAttiva == 2) return true;
		if (*pOrgX != 0 || *pOrgY != 0) return true;

		Valore = (short) HIWORD(wParam);

		Valore = CalcolaIncRuota(Valore);
		__asm {
			pushad
			lea		ecx, Valore
			mov		cx, word ptr [ecx]
			lea		eax,ScrollaTexture
			call	eax
			popad
		}

		return true;
	

	}
	return true;
}
// intercetta messaggi per window procedure di room editor
// esegue tutto tramite salti diretti senza return
__declspec(naked) void PatchHookMessaggi(void)
{
	__asm {
		// ebx = hWind
		// esi = message
		// edi = lparam
		mov ecx, dword ptr [esp+58h]
		// ecx = wparam
		// se winfdow è quella di button window
		// chiamare la window procedure
		cmp dword ptr [BaseWindow.TotButtons],0
		jz  Ignora
		// se window è quella di button window reindirizzare
		mov eax, 46E559h // window room editor
		mov eax, dword ptr [eax]
		cmp eax, ebx
		jz Ignora
		// finestra diversa da room editor
		//  chiamare mia proc
		push edi  // lparam
		push ecx  // wparam
		push esi // msg
		push ebx // hwind
		call WndProcButton

		// ora ripristinare e uscire
		pop edi
		pop esi
		pop ebx
		add esp, 40h
		ret 10h
Ignora:
		pushad
		push	edi
		push	ecx
		push	esi
		push	ebx
		call	HookMessaggiRoomEditor
		add		esp,10h
		lea		ecx, RetValore
		mov		dword ptr [ecx],eax
		popad

		lea		eax, RetValore
		cmp		dword ptr [eax],0
		jnz		Continua
		// bisogna uscire direttamentre in default message
		mov		eax,  444664h  ;esce con defwindowproc
		jmp		eax
Continua:
 		cmp  esi, 105h		;261  Switch (cases 1..208)
 		ja   SaltoPerSwitchMessaggi
 		cmp  esi, 104h
 		jnc  SaltoPerLeggiDirectInput
		;salto per controllo successivo
		mov		eax, 44463Ah
		jmp		eax
SaltoPerSwitchMessaggi:
		mov		eax, 444722h  ; SaltoPerSwitchMessaggi
		jmp		eax
SaltoPerLeggiDirectInput:
		mov		eax, 4447F2h  ; SaltoPerLeggiDirectInput
		jmp		eax

	}

}
// alza (Incremento+1) o abbassa (incremento -1) stanza attuale
// piu' eventuali stanze collegate
// se stanza attuale non e' collegata ad altre restituisce false e non fa niente
bool MuoviBloccoRoom(int Incremento)
{
	DWORD *pIndiceRoomAttuale = (DWORD*) 0x583BA0;
	int i;
	int IncClick;
	BYTE *pPulsantiMouse = (BYTE*) 0x46DA88;
	int	Indice;
	int SizeX,SizeY;
	int x,y;
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;

	pVetRooms = *p2VetRooms;
	TrovaStanzeCollegate(*pIndiceRoomAttuale);
	if (TotCollegate == 1) return false;
	if ((*pPulsantiMouse) & 2) 
		IncClick = 4 * Incremento;
	else
		IncClick = Incremento;

	for (i=0;i<TotCollegate;i++) {
		Indice = VetCollegate[i];
		pVetRooms[Indice].OrgYClickBottom  += IncClick;
		pVetRooms[Indice].OrgYClickTop += IncClick;
		
		SizeX = pVetRooms[Indice].XSizeBlocks;
		SizeY = pVetRooms[Indice].ZSizeBlocks;
		for (y=0;y<SizeY;y++) {
			for (x=0;x<SizeX;x++) {
				
				pVetRooms[Indice].pBlockGridCollision[x+y*SizeX].WorldFloorClick += IncClick;
				pVetRooms[Indice].pBlockGridCollision[x+y*SizeX].WorldYCeilingClick += IncClick;
			}
		}
	}
	// mostrare mappa 
	__asm {
		pushad
		push 1
		mov eax, 4266F0h    ;MostraPianta2d
		call eax
		add  esp, 04h
		//provare a NON fare ciclo principale
		mov eax, 401200h    ;CicloPrincipale
		call eax
		mov eax, 4342E0h    ;AttendiFineInputMouse
		call eax


		popad
	}

	return true;

}
// chiamata quando si preme room meno
__declspec(naked) void PatchRoomMeno(void)
{
	__asm {
		lea		eax, TestMappaVerticale
		cmp		dword ptr [eax], 0
		jz		CodiceNormale
		// gestione con spostamento multiplo
		push	-1
		lea		eax, MuoviBloccoRoom
		call	eax
		add		esp,4
		test    al,al
		jz		CodiceNormale
		// gia' eseguito spostamento
		stc
		retn
CodiceNormale:
		push eax
		mov eax, 46DA88h   ;PulsantiMouse
		test byte ptr [eax], 02h
		pop eax
		jz   Salto1
		;se pulsante destro 
		mov eax, 40CD20h    ;TrovaMaxSpostaGiuRoom
		call eax
		clc
		retn
Salto1:
		mov  eax, 01h
		clc
		retn

	}

}
__declspec(naked) void PatchRoomPiu(void)
{
	__asm {
		lea		eax, TestMappaVerticale
		cmp		dword ptr [eax],0
		jz		CodiceNormale
		// gestione con spostamento multiplo
		push	1
		lea		eax, MuoviBloccoRoom
		call	eax
		add		esp,4
		test    al,al
		jz		CodiceNormale
		stc
		retn
CodiceNormale:
		push eax
		mov eax, 46DA88h   ;PulsantiMouse
		test byte ptr [eax], 02h
		pop eax
		jz   Salto1
		mov eax, 40CBB0h    ;TrovaMaxSpostaSuRoom
		call eax
		clc
		retn
Salto1:
		mov  eax, 01h
		clc
		retn

	}

}


__declspec(naked) void PatchLockSurface(void)
{
	__asm {
		test eax,eax
		jz	 TuttoOk
		// c'e' stato un errore, uscire e basta
		push eax
		mov eax, 4420C0h    ;GestioneErroreDirectX
		call eax
		add		esp,4

		mov		eax, 445001h  ; EsciNow
		jmp		eax
		
TuttoOk:
		push eax
		mov eax, 4420C0h    ;GestioneErroreDirectX
		call eax
		mov		eax, 444D4Eh ;salto a istruzione seguente questa patch
		jmp		eax

	}


}

void MonitorTexOggetto1(int Valore)
{
	
	if (MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) {
		// eseguire senza conrollo eccezioni
		__asm {

			lea		eax,Valore
			mov		eax, dword ptr [eax]
			push	eax
			mov eax, 404DE0h    ;GestioneTextureOggetto
			call eax
			add		esp,4
			

		}
		return;
	}

	TestBloccoTRY++;

	try {
		__asm {

			lea		eax,Valore
			mov		eax, dword ptr [eax]
			push	eax
			mov eax, 404DE0h    ;GestioneTextureOggetto
			call eax
			add		esp,4
			

		}
	} catch(...) {

#ifdef LOG_CRASH
		InviaLog("Resumed crash in MonitorTexOggetto1()");
#endif

	}
	TestBloccoTRY--;
			
}
__declspec(naked) void ControllaTextureOggetto1(void)
{
	__asm {
		push	esi
		lea		eax, MonitorTexOggetto1
		call	eax
		add		esp,4

		retn

	}
}

// qui passa il controllo nel momento in cui c'e' un eccezione
// in al c'e' 1 per il primo gestore e 2 per il secondo
__declspec(naked) void PreparaEccezioniRoom(void)
{
	__asm {
#ifdef LOG_CRASH
		pushad
		lea		eax, ShowInfoEccezione
		call	eax
		popad 
#endif
		lea  ecx, TestBloccoTRY
		cmp  dword ptr [ecx], 0
		jnz  StandardEccezioni
		// ok non e' gestita usare controllo eccezioni generale
		lea	 eax, GestioneEccezioni
		jmp eax
StandardEccezioni:
		cmp	 al,1
		jnz	 Gestore2
		// mette codice rimosso e poi salta a codice seguente di gestore 1
		mov  ecx, [esp+04h]	;  Structured exception handler
		test dword ptr [ecx+04h], 06h
		// ora saltare a continuazione
		mov	 eax, 44D2F3h
		jmp  eax
Gestore2:
		// mette codice rimosso e poi salta a codice seguente
		push ebp	;  Structured exception handler
		mov  ebp, esp
		sub  esp, 08h
		push ebx
		push esi
		push edi
		push ebp
		// e ora saltare al codice rimanente
		mov		eax,44D3CAh  
		jmp		eax

	}

}

// sostituisce funzione originale EseguiRotazioni3d
// con lo scopo di applicare preferenze di rotazione slow o fast
void PatchRotazioni3d(void)
{

	int ValZoom;
	short *pMemOrizzontale = (short*) 0x56F470;
	short *pMemVerticale = (short *) 0x56F46E;
	short *pMemZoom = (short*) 0x56F460;
	int Incremento;
	static BYTE * pVetInputKeyboard = (BYTE *) 0x46DF98;
	int * pTotTasti = (int *) 0x46D704;
	// prima fare controllo per slowdown
	// fattori da 0 a 9 da attivare SOLO se shift e' premuto
	if (TastoPremutoNGLE(0x2a,-1) == true) {
		// controllare che siano passati tot tick da ultima esecuzione
		Sleep(VetShiftDelay[MyPrefRoom.SpinningTimeInput]);

	}

	// ora vedere se ci sono tasti che giusticiano rotazione
	// prima escludere se ci sono premuti tasti ctrl o alt
	// oppure se nessuno dei tasti per rotazioni e zoom e' premuto
	if ((TastoPremutoNGLE(0x1d,-1) == true || TastoPremutoNGLE(0x38,-1)== true) ||
		(TastoPremutoNGLE(0xc8,-1)==false && TastoPremutoNGLE(0xc9,-1)==false && 
		 TastoPremutoNGLE(0xcb,-1)==false && TastoPremutoNGLE(0xcd,-1)==false && 
		 TastoPremutoNGLE(0xd0,-1)==false && TastoPremutoNGLE(0xd1,-1)==false)) {

		DatiRotazioni.IncrementoRotazioni=0;
		return;
	}

	// ok, adesso c'e' sicuramente qualche cosa da fare.
	// calcolare nuovo incremento da usare
	DatiRotazioni.IncrementoRotazioni += VetIncrementoStep[MyPrefRoom.SpinningStep];
	if (DatiRotazioni.IncrementoRotazioni >= VetIncrementoMax[MyPrefRoom.SpinningStep]) {
		DatiRotazioni.IncrementoRotazioni = VetIncrementoMax[MyPrefRoom.SpinningStep];
	}

	Incremento=DatiRotazioni.IncrementoRotazioni;

	// ora incrementare i valori specifici facendo controllo
	// perche' non superino dimensione massima prevista

	// ----- freccia in ALTO: diminuire orientamento verticale
	if (TastoPremutoNGLE(0xc8,-1)==true) 
			(*pMemVerticale) -= Incremento;

	// ------ freccia SINISTRA: aumenta orientamento orizzontale
	if (TastoPremutoNGLE(0xcb,-1)==true)
			(*pMemOrizzontale) += Incremento;

	// ----- freccia DESTRA: diminuire orientamento orizzontale
	if (TastoPremutoNGLE(0xcd,-1) ==true) 
			(*pMemOrizzontale) -= Incremento;
	
	// ----- freccia BASSO: aumentare orientamento verticale
	if (TastoPremutoNGLE(0xd0,-1)==true) 
			(*pMemVerticale) += Incremento;

	ValZoom = *pMemZoom;
	// ------ PAGINA ALTO: diminuire Distanzazoom
	if (TastoPremutoNGLE(0xc9,-1)==true) {
		ValZoom -= Incremento;
		if (ValZoom < 0x100) ValZoom=0x100;
	}

	// ------ PAGINA BASSO: aumentare distanza zoom
	if (TastoPremutoNGLE(0xd1,-1)==true) {
		ValZoom += Incremento;
		if (ValZoom > 0x7fff) ValZoom= 0x7fff;
	}
	*pMemZoom = ValZoom;
	SalvaUltimoTempoInput();
	if (OldSimulaStatusKey==0 && 
		(OldSimulaKey == 0xCB || OldSimulaKey == 0xCD ||
		OldSimulaKey == 0xC8 || OldSimulaKey == 0xD0 ||
		OldSimulaKey == 0xC9 || OldSimulaKey == 0xD1) ) {
		
		// bloccare l'input per evitare loop infiniti
		pVetInputKeyboard[OldSimulaKey]=0;
		*pTotTasti=0;
		OldSimulaKey=0;
		OldSimulaStatusKey=0;

	}

	// ora aggiornare rotazione
	__asm {
		pushad
		mov eax, 401000h    ;Show3d_DaNuovaPosizione
		call eax
		mov eax, 401200h    ;CicloPrincipale
		call eax
		mov eax, 436AEEh    ;ElaboraQualcosaMouse
		call  eax
		popad
	}




}
// se e' attivo in preferenze salva il bitmap dell'ultima mappa 
// creata subito dopo l'outputwad
void SalvareMappa2d(void)
{
	char NomeFile[256];
	char *pNomeProject = (char*) 0x46B858;  // BufferNomeProject

	if (MyPrefRoom.Flags & FPR_SAVE_MAP_BMP)  {
		sprintf(NomeFile,"Last_2dMap_%s.bmp", SoloNomeSenzaExt(pNomeProject));
		SalvaScreenShot(NomeFile,TSS_PANEL3D);
		sprintf(BufferLog,"Saved 2d map: %s", SoloNome(NomeFile));
		ShowInfoRoom(BufferLog);
	
	}

}


// eseguita ad ogni ciclo di controllo per menu interni e shortcut di menu
// eax punta all'ultimo record di input rilevato
// se dword ptr [eax] <> 0 allora c'e' un input
__declspec(naked) void PatchCicloSleep(void)
{

	__asm {
		pushad
		mov	  ecx, eax
		push	ecx
		lea		eax,AnalisiStandBy
		call	eax
		add		esp,4
		popad
		retn
	}

}

// provare con linguaggio c
void PatchMaximizeNgle(void) 
{
	HWND *pMiaWind = (HWND*) 0x46E559;
	int *pTestFinestraMax = (int*) 0x493120;
	RECT *pRectFinestraPrecedente = (RECT *) 0x46EA64;

	if ((MyPrefRoom.Flags & FPR_MAXIMIZE) != 0 && 
		(*pTestFinestraMax) == 0) {
		// effettuare massimizzazione
		GetWindowRect(*pMiaWind, pRectFinestraPrecedente);
		SetWindowLong(*pMiaWind,GWL_STYLE,0);
		ShowWindow(*pMiaWind, SW_SHOWMAXIMIZED);
		*pTestFinestraMax = 1;
	}
}
// salva in variabile globale il nome del file tom
void SalvaNomeTom(char *pFileTom)
{
	strcpy(GlobNomeTom,pFileTom);
	

}

// analizza tutte le stanze e modifica la posizione di indici
// in VetObjAndTriggerIndex[] per fare in modoche se c'e' un trigger
// speciale sianell'ultima posizione (poiche' nella creazione
// di floor data vengono scanditi gli indici a partire dall'ultimo)

void RiordinaTriggerSpeciali(void)
{
	StrStanzaRoomEdit **p2VetRooms = (StrStanzaRoomEdit**) 0x46B850;  //  Ptr_VetRoomSlots:
	StrStanzaRoomEdit *pVetRooms;
	int *pTotRooms = (int*) 0x56C710;
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrTriggerRoom ** P2_VetTriggers = (StrTriggerRoom**) 0x569D60;
	StrTriggerRoom *pVetTriggers;
	int i;
	int j;
	int ItemIndex;
	int IndiceTrigger;
	int IndiceOld,IndiceNew;
	bool TestTriggerSpeciale;
	WORD ValScambio;

	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetTriggers = *P2_VetTriggers;
	pVetRooms = *p2VetRooms;

	for (i=0;i< *pTotRooms;i++) {
		if (pVetRooms[i].TestEmptySlot==0) {
			TestTriggerSpeciale=false;
			for (j=0;j< pVetRooms[i].N_ObjAndTriggers;j++) {
				ItemIndex = pVetRooms[i].VetObjAndTriggerCodes[j];
				if (pVetBaseOggetti[ItemIndex].TypeObject== 0x10) {
					// e' un trigger
					IndiceTrigger = pVetBaseOggetti[ItemIndex].SlotIndex;

					if (pVetTriggers[IndiceTrigger].TriggerAttivazione == 0x0c) {
						// e' una condizione,  e' necessario che si trovi
						// in prima posizione
						TestTriggerSpeciale =true;
						IndiceOld = j;
						break;
					}
				}
			}

			if (TestTriggerSpeciale==true) {
				IndiceNew = pVetRooms[i].N_ObjAndTriggers-1;

				if (IndiceNew != IndiceOld) {
					// effettuare scambio
					ValScambio = 
							pVetRooms[i].VetObjAndTriggerCodes[IndiceOld];
					pVetRooms[i].VetObjAndTriggerCodes[IndiceOld]= 
							pVetRooms[i].VetObjAndTriggerCodes[IndiceNew];
					pVetRooms[i].VetObjAndTriggerCodes[IndiceNew] = 
							ValScambio;
				}
			}
		}
	}

}

__declspec(naked) void PatchBugFlipMap(void)
{
	__asm {
		// analizzare anche nome file .tom per salvarlo in GlobNomeFileTom[]
		pushad
		// indirizzo di nome tom dovrebbe essere in esp+6c
		mov  eax, dword ptr [esp+6ch]
		lea  ecx, SalvaNomeTom
		push  eax
		call ecx
		add esp, 4
		// azzerare TotPosFloor che nel corso di registrazione floor data memorizzera'
		// i trigger con un plugin diverso da trng
		mov dword ptr [GlobNgle.TotPosFloor], 0

		// se c'era attivo [hide object] lo disattiva
		push 0
		call IsToDisableRemoveItems
		add esp, 4

		lea		eax, DisableFlipMap
		call	eax

		// azzera poulsanti mouse
		mov  eax, 46DA88h  ; PulsantiMouse
		mov word ptr [eax], 0

		mov  eax, 46E0A4h  ; StatusPulsantiMouse
		mov word ptr [eax], 0

		// riordinare trigger in modo che trigger speciale
		// sia sempre analizzato per primo in creazione floor data
		lea   eax, RiordinaTriggerSpeciali
		call  eax

		// aggiunge flag acqua alle stanze palude con intensita diversa da 4
		call AggiungePaludeAcqua

		call AzzeraVetStaticRemap

		popad
		
		mov eax, 56C710h    ;Tot_RoomSlots
		mov  eax, dword ptr [eax]
		xor  esi, esi
		retn
	}
}
// chiamata subito dopo il salvataggio (con successo) di un file projec
// lacvora anfhe per autosave.prj
// usata per salvare nel file project appena salvato l'extra header ng
__declspec(naked) void PatchDopoSaveProject(void)
{
	__asm {
		pushad
		lea		eax, NomeLastSavedProject
		push	eax
		lea		eax, SalvaNGHeaderProject
		call	eax
		add		esp,4
		popad
		// restituire eax =1
		mov	 eax,1
		retn
	}
	
}
// chiamata con un jump quando si caricano i dati di menu texture
// e' usata per alterare le voci di quel menu
__declspec(naked) void PatchMenuTextures(void)
{
	__asm {
		push  406EA0h ;Menu_LoadTGA
		push 00h
		push  459B20h	;  ASCII "Load TGA" ;Mex_Load_TGA
		// Shapes->Create foor traingular tex
		lea		eax, Menu_CreaTriangoli
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CreaTriangoli
		push	eax		;Nome menu						;3

		// Shapes->Create ceiling traingular tex
		lea		eax, Menu_CCreaTriangoli
		push	eax		;Procedura 						;1
		push	0										;2
		lea		eax, NomeMenu_CCreaTriangoli
		push	eax		;Nome menu						;3


		push  42B9C0h ;Menu_LoadDepthCue
		push 00h
		lea	  eax, MexLoadPalette
		push  eax


		push 42BAC0h  ; Menu_SaveDepthCue
		push 00h
		lea  eax, MexSavePalette
		push  eax
		// rimuove tail infos
		lea	 eax, TrovaUnusedTailInfo
		push eax

		push 00h
		lea  eax, MexRimuoviTailInfos
		push eax
		// fine rimuovi tail infos
		lea  eax, FindAndReplaceTex
		push eax

		push 00h
		lea  eax, MexFindAndReplaceTex
		push eax

		push  429E20h ;Menu_ClearRoom
		push 00h
		push  459AFCh	;  ASCII "Clear Room" ;Mex_Clear_Room


		; -------  inizio NUOVE voci ---------
		;salvare stack per poi calcolare la variazione di valore da alterare
		lea		eax, SalvaStack
		mov		dword ptr [eax], esp

		// Shapes->Turn Selected Textures
		lea		eax, Menu_RuotaTexture
		push	eax		;Procedura 						;4
		push	0						;5
		lea		eax, NomeMenuRuotaTexture
		push	eax		;Nome menu						;6

		// Shapes->Clear floor textures
		lea		eax, Menu_ClearTextureFloor
		push	eax		;Procedura 						;4
		push	0						;5
		lea		eax, NomeMenuClearTextureFloor
		push	eax		;Nome menu						;6

		// Shapes->Clear Ceiling textures
		lea		eax, Menu_ClearTextureCeiling
		push	eax		;Procedura 						;4
		push	0						;5
		lea		eax, NomeMenuClearTextureCeiling
		push	eax		;Nome menu						;6
		
		// texture->Check texture integrity
		lea		eax, TrovaTailInfoErrate
		push	eax
		push	0
		lea		eax, MexCheckIntegrityTexture
		push	eax

		;------  fine nuove voci -----------
		;calcolare nuovo valore di stack
		lea		eax, SalvaStack
		mov		eax, dword ptr[eax]
		sub		eax, esp

		mov		ecx, 410cf4h  ;punta a add esp, ...
		add		dword ptr [ecx], eax

		;intestazione texture
		push 200   ;larghezza
			;prima 0ah
		push 0ch   ;era 0bh  numero elementi
		push  459AC4h	;  ASCII "Texture" ;Mex_Texture

		mov eax, 410B08h ;salto per tornare a menu room
		jmp	 eax

	}
}
// controlla se stanza pRoom ha gia' installato il puntatore 
// a griglia collisioni.
// se non c'e', lo alloca adesso e lo inizializza
void InitTabellaGrid(StrStanzaRoomEdit *pRoom)
{
	StrGridXRoom *pGriglia;
	int SizeX,SizeY;
	int SizeMemoria;
	int x,y;
	int Indice;

	// roomChiediMemoria
	if (pRoom->pBlockGridCollision == NULL && pRoom->TestEmptySlot == 0) {
		sprintf(BufferLog,"WARNING: missing pBlockGridCollision in room \"%s\". Trying to correct this bug",
			pRoom->Nome);
		InviaLog(BufferLog);
		SizeX = pRoom->XSizeBlocks;
		SizeY = pRoom->ZSizeBlocks;
		SizeMemoria = SizeX * SizeY * 144;
		pGriglia = (StrGridXRoom*) roomChiediMemoria(SizeMemoria,"InitTabellaGrid() Allocate pGridCollision");
		memset(pGriglia,0,SizeMemoria);
		
		for (y=0;y<SizeY;y++) {
			for (x=0;x<SizeX;x++) {
				Indice = x + y * SizeX;
				// vedere se e' un settore muro
				if (y == 0 || x == 0 || y == (SizeY-1) || x == (SizeX-1)) 
					pGriglia[Indice].Type = 0x1e; // bound wall
				else
					pGriglia[Indice].Type = 1;  // floor
				pGriglia[Indice].WorldFloorClick = pRoom->OrgYClickBottom;
				pGriglia[Indice].WorldYCeilingClick  = pRoom->OrgYClickTop;
			}
		}
		pRoom->pBlockGridCollision = pGriglia;

	}

}
// controlla il puntatore a tabella griglia di stanza attuale
// sia stato allocato.
// se non lo e' stato lo alloca adesso

__declspec(naked) void PatchCheckGrid(void)
{
	__asm {
		// input:
		//		ecx = puntatore memoria a stanza attuale
		pushad
		push	ecx
		lea		eax, InitTabellaGrid
		call	eax
		add		esp,4
		popad

		mov  eax, [ecx+74h]   ;puntatore a tasbella grid che risulta vuoto
		lea  eax, [eax][edx*2]
		mov  dx, [eax+06h]  ;qui avviene un crash
		retn
	}


}
// analizza indirizzo di sub allo scopo di capire se e' un conando
// che causa una modifica al progetto
// se lo e' aumenta TotCambiBackup
void AnalisiUltimoComando(DWORD AdrSub)
{
	static DWORD VetNoCambi[] = { 0x425D50,  0x4242D0,
				  0x4243A0, 0x424200, 0x4244A0,
				  0x424620, 0x417660, 0x4260F0,
				  0x4104E0, 0x408900, 0x4247E0,
				  0x408870, 0x42A1B0, 0x429EB0,
				  0x41E690, 0x413970, 0x419930,
				  0x41E540, 0x410990, 0x410240,
				  0x424000, 0x42A840, 0x40E990,
				  0x40E6B0, 0};
	int i;

	/*
	sprintf(BufferLog,"0x%X,", AdrSub);
	InviaLog(BufferLog);
	*/
	// se pulsante e' relativo a next gneeration ignorarlo
	if (AdrSub > 0x454532) return;

	// se e' diverso da tutti quelli nulli aumentare i cambi di 1
	i =0;
	while (VetNoCambi[i] != 0) {
		if (AdrSub == VetNoCambi[i++]) return;
	}
	// e' diverso da tutti quelli nulli
	TotCambiBackup++;



}
// verifica ultimo comando impartito nel room editor 
__declspec(naked) void PatchUltimoComando(void)
{
	__asm {
		pushad
		// in ecx indirizzo di sub pulsante o menu che viedne chiamata
		push  ecx
		lea  eax, AnalisiUltimoComando
		call  eax
		add  esp,4

		popad
		retn
	}

}
// se il tasto (contneuto in pInfo) corrisponde ad un comando
// che modifica il livello incrementa TotCambiBackup
void AnalisiUltimoTasto(StrInfoMouse *pInfo)
{
	static WORD VetCodici[] = { 0x04cb, 0x4cd, 0x4c8, 0x4d0,
			0x8cb, 0x8cd,0x8c8, 0x8d0, 
			0x7, 0x8,0x9,0xd3, 0x18, 0x10,0x1e,
			0x11, 0x1f,0x12,0x20,0x13,0x21,0};
	WORD CarNow;
	WORD Temp;
	int i;

	if (pInfo->CodiceTasto !=0) {
		Temp = pInfo->StatusTasti & 0x0f;
		Temp = Temp << 8;
		CarNow = pInfo->CodiceTasto;
		CarNow |= Temp;

	

	
		i =0;

		while (VetCodici[i] != 0) {
			if (CarNow == VetCodici[i++]) {
				TotCambiBackup++;
				return;
			}
		}
	}

}
// patch per contare byte registrati in file .tom
// (da usare solo temporaneamente)
// situazione argomenti nello stack:
// ARG ESP+10 push ebx      handle file
// ARG ESP+C  push 01h     numero elementi
// ARG ESP+8   push 04h   size elemento
// ARG ESP+4   offset Buffer
__declspec(naked) void PatchWriteFile(void)
{

	__asm {
		// analisi per vedere se e' l'handle file .tom (0x465ED0)
		mov  eax, dword ptr [esp+10h]
		cmp  eax, 465ED0h  ;handle fiel tom ?
		jnz  TornaAWrite
		// ok, e' handle .tom
		mov  eax, dword ptr [esp+0ch]  ;numero elementi
		mov  ecx, dword ptr [esp+8]   ;size elemento
		imul eax,ecx
		// ora in eax ci dovrebbe essere il totale dei bytes
		mov  ecx, 454FF0h  ;ContaOffset
		add dword ptr [ecx], eax

		// codice originale rimosso da patch
TornaAWrite:
		push ebp
		mov  ebp, esp
		push esi
		push dword ptr [ebp+14h]
		mov	eax, 44AD6Fh  ;salta a seekendfile
		jmp  eax

	}
}
// chiamato prima di eseguire ogni comando tastiera
__declspec(naked) void PatchUltimoTasto(void)
{
	__asm {
		pushad
		push	eax
		lea   eax, AnalisiUltimoTasto
		call	eax
		add		esp,4
		popad
		retn
	}
}
// ridimensionare la texture di dimensione sizex sizey 
// in modo da forzare ognuno dei due lati a non piu' di 64 pixel
// e poi modifica anche valori in record tail info
void RiduciObjTexture(StrTailInfo *pTailTexture,int SizeX, int SizeY)
{
	int x,y;
	int OldY;
	float IncX,IncY, xNow, yNow;
	int	NewY;
	BYTE *pSrc;
	BYTE *pDest;
	int  DestSizeX,DestSizeY;

	pSrc = pTailTexture->pMemImage;
	pDest = pSrc;

#ifdef LOG_TEXTURE 
	sprintf(BufferLog,"Reduce obj texture: ImageTga=0x%X   SizeSrcX=0x%X  SizeSrcY=0x%X", 
		pSrc,SizeX,SizeY);

	InviaLog(BufferLog);
#endif


	DestSizeX = SizeX;
	DestSizeY = SizeY;

	if (DestSizeX > 64) DestSizeX = 64;
	if (DestSizeY > 64) DestSizeY = 64;


	IncX = (float) SizeX / DestSizeX;
	IncY = (float) SizeY /DestSizeY;
	
	if ((MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) == 0)  TestBloccoTRY++;

	try {


		// ridimensione texture

		yNow = 0;
		OldY = 0;
		for (y=0;y<DestSizeY;y++) {
			xNow = 0;
			for (x=0;x<DestSizeX;x++) {

				pDest[x] = pSrc[int(xNow)];
				xNow += IncX;
			}
			// qui bisogna stabilire se bisogna aumentare riga o no
			yNow += IncY;
			NewY = int(yNow);
			
			pSrc += 256 * (NewY - OldY);
			
			OldY = NewY;
			pDest += 256;
		
		}	
	}
	catch(...) 
	{

		InviaLog("\tERROR: EXECPTION IN [ReduceObjTexture()]");

		
	}	
	// impostare nuova dimensione
	DestSizeX = DestSizeX << 8;
	DestSizeY = DestSizeY << 8;
	DestSizeX--;
	DestSizeY--;
	pTailTexture->SizeX = (WORD) DestSizeX;
	pTailTexture->SizeY = (WORD) DestSizeY;

	// rimozione di controllo eccezioni
	if ((MyPrefRoom.Flags & FPR_NO_RESUME_CRASH) == 0) TestBloccoTRY--;
	

}
void RimappaObjTextures(BYTE *pMemImage, StrTailInfo* pVetTailInfos, 
						int TotTails)
{
	int i;
	int SizeX,SizeY;
	
	for (i=0;i<TotTails;i++) {
		SizeX = pVetTailInfos[i].SizeX >> 8;
		SizeY = pVetTailInfos[i].SizeY >> 8 ;
		SizeX ++;
		SizeY ++;		

		if (SizeX > 64 || SizeY > 64) 
					RiduciObjTexture(&pVetTailInfos[i],SizeX,SizeY);
	}
}


// chiamata appena caricate texture per oggetti
// riduce tutte le texture maggiori di 64x64 e modifica le tail info
// degli oggetti

__declspec(naked) void PatchRemapObjTextures(void)
{

	__asm {
		// in esp + 1c (che pero' diventa esp + 20 
		// c'e' il numero di tail info di oggetti
		mov  ecx, dword ptr [esp+20h]
		pushad 

		push ecx ;tot tail objects
		mov eax, 56C4ECh    ;Ptr_ObjectTextureWad
		mov  eax, dword ptr [eax]
		push  eax   ;tail info per objects
		mov ecx, 46B81Ch    ;Ptr_BaseImmagineTextureOggetti
		mov  ecx, dword ptr [ecx]
		push ecx   ;immagine texture oggetti
		lea		eax, RimappaObjTextures
		call	eax
		add		esp, 0ch

		popad


		// codice che era stato rimosso da patch
		mov eax, 56C4ECh    ;Ptr_ObjectTextureWad
		mov  eax, dword ptr [eax]
		mov ecx, 46B81Ch    ;Ptr_BaseImmagineTextureOggetti
		mov  ecx, dword ptr [ecx]
		retn


	}
}
// salvare file di log
void SalvaHandleFileLog(DWORD HandleFileLog)
{
	InviaLog("******************** START OUTPUT WAD LOG ******************");
	GlobHandleFileLog = HandleFileLog;


}
// chiamata cicliclamente in outputwad, una volta per ogni stanza salvata
// usare GlobNgle.TotExtraInfoRoom come riferimento a indice attuale
void SalvaWaterIntensity(StrStanzaRoomEdit *pRoom)
{
	// salvare water intensity
	GlobNgle.VetExtraInfoRoom[GlobNgle.TotExtraInfoRoom++].WaterIntensity = 
													pRoom->WaterValue;

}
// salva in memoria globale vettori per rimappare oggetti e room
// nota in EAX c'e' il nome del file di log appena aperto
__declspec(naked) void PatchSalvaRemapArray(void)
{
	__asm {
		// salvare handle di file log tom
		pushad
		push  eax
		lea   eax, SalvaHandleFileLog
		call  eax
		add  esp,4

		// azzera indice epr contare stanze che vengono salvate in file .tom
		lea  eax, GlobNgle.TotExtraInfoRoom
		mov dword ptr [eax], 0
		popad

		pushad
		lea		eax, pGlobRemapObj
		mov  dword ptr [eax], edi   ;remap obj

		lea		eax, pGlobRemapRoom
		mov	dword ptr [eax], esi    ;remap rooms

		popad
		// codice rimosso da patch
		mov ecx, 466B2Ch    ;TestAllocaMemoriaTexture
		mov  ecx, dword ptr [ecx]
		mov edx, 56C910h    ;VersioneObjectH
		mov  edx, dword ptr [edx]
		retn	

	}

}
// chiamata all'inizio di fnzione fileFprintf
// VetStack punta allo stack originale all'inziio della funzione:
// VetStack[0] = indirizzo ritorno di procedura
// VetStack[1] = Handle file log
// VetStack[2] = Testo principale
// VetStack[3,4,5,6,7] = argomenti %d %s per testo principale
void CatturaFileLog(DWORD *VetStack)
{
	char *pStringa;
	char BufferTemp[512];

	pStringa = (char*) VetStack[2];

	sprintf(BufferLog, pStringa, VetStack[3], VetStack[4], 
				VetStack[5],VetStack[6]);
	sprintf(BufferTemp,"\t\tLOG_0x%X: %s", VetStack[0],BufferLog);

	InviaLog(BufferTemp);
	

}

// intercetta tutti i messaggi testuali
// viene chiamata esattamente all'inziio di fileprintf() in roomeditor
__declspec(naked) void PatchPrintF(void)
{
	__asm {
		mov eax, esp
		pushad
		push  eax
		lea  eax, CatturaFileLog
		call  eax
		add  esp,4
		popad
		// codice eliminato da patch
		push ebx
		push esi
		mov  esi, [esp+0ch]  ;Handle del file
		mov  eax, 44AEA7h
		jmp  eax
	}

}

void ShowLogBoxLoop(int IndiceRoom, int CordX, int CordZ)
{
	sprintf(BufferLog,"BoxCompute:  Room=%d  Case(%d,%d)",
		IndiceRoom,CordX,CordZ);
	InviaLog(BufferLog);

}

__declspec(naked) void PatchShowLogBoxes(void)
{
	__asm {
		// visualizzare dati di zone box
		;ebx = indice stanza
		;ebp = cordx
		;esi  = cordy
		pushad
		push  ebp
		push  esi
		push  ebx
		lea   eax, ShowLogBoxLoop
		call  eax
		add   esp,0ch


		popad
		// codice eliminato da patch
		test esi, esi
		jz   Bimbo_SettoreMuroBound
		test ebp, ebp
		jz   Bimbo_SettoreMuroBound
		mov  ecx, 4051D8h  
		jmp ecx
Bimbo_SettoreMuroBound:
		mov  ecx, 405212h  ; Bimbo_SettoreMuroBound
		jmp  ecx

	}

}

// viene chiamata subito dopo aver scritto i dati di un 
// trigger
// tutti i valori in variabili globali
// aggiunge campo extra word dove necessario
// e corregge trigger action dove era solo object\
// nota: attualemnte totfloordata punta esattamente alla word 
// formata da <TRIGGER_WHAT> + <ITEM_DATA>
// e deve essere incrementato almeno di 1 totfloo

void CorreggiTriggerOut(void)
{
	WORD **p2BaseMemoriaFloor = (WORD **) 0x46BEC4;
	WORD *PtrMemoriaFloor;
	WORD *pTotFloor = (WORD *) 0x566660;
	WORD IndiceTot;
	WORD BitFine;
	int i;

	PtrMemoriaFloor = *p2BaseMemoriaFloor;
	IndiceTot = *pTotFloor;

	BitFine = PtrMemoriaFloor[SalvaTotFloorData] & 0x8000;


	// analisi per salvare info quando trigger e' di un plugin
	if (PtrUltimoTrigger->PluginId > 0) {
		i=GlobNgle.TotPosFloor;
		GlobNgle.VetPosFloorIndex[i] = IndiceTot;
		GlobNgle.VetPosFloorPluginId[i] = PtrUltimoTrigger->PluginId;
		GlobNgle.TotPosFloor++;


	}
	// vedere se e' uno di quei trigger per cui bisogna
	// effettuare modifiche
	if (PtrUltimoTrigger->TriggerWhat == 0xb) {
		// e' ACTION
		// bisogna correggere campo alto dove c'e' codice di triggerwhat
		//perche' inizialemnte e' stato messo un generico OBJECT (0)
		// salvare in indice originale di quando abbiamo salvato ultimo trigger

		PtrMemoriaFloor[SalvaTotFloorData] &= ~0x3c00;
		PtrMemoriaFloor[SalvaTotFloorData] |= (0x0b << 10);
	}

	if (PtrUltimoTrigger->TriggerWhat == 9 ||
		PtrUltimoTrigger->TriggerWhat == 0xb) {
		// e' FLIPEFFECT o ACTION, bisogna aggiungere extra word di timer
		// in realta' timer sarebbe un qualche parametro...
		// se c'era bit8000 allora togliero da quello attuale
		// e metterlo al mio argomento
		if (BitFine) {
			PtrMemoriaFloor[SalvaTotFloorData] &= 0x7fff;
		}
		IndiceTot = SalvaTotFloorData+1;
		PtrMemoriaFloor[IndiceTot] = PtrUltimoTrigger->Timer & 0x7fff;
		PtrMemoriaFloor[IndiceTot] |= BitFine;

	}
	IndiceTot++;
	*pTotFloor=IndiceTot;
}
// nota: da true per moveable, camera e sink
// nel caso di  TestExport da true anche per flyby camera
bool IsMoveable(short Indice, bool TestExport)
{
	StrItemsRoom ** P2_VetBaseOggetti = (StrItemsRoom **) 0x46B814;
	StrItemsRoom * pVetBaseOggetti;
	StrObjSlotRoom ** p2VetSlotOggetti = (StrObjSlotRoom**)  0x46B818; 
	StrObjSlotRoom * pVetSlotOggetti;
	WORD TipoItem;

	int SlotIndex;

	if (Indice < 0) return false;

	pVetBaseOggetti = *P2_VetBaseOggetti;
	pVetSlotOggetti = *p2VetSlotOggetti;
	TipoItem = pVetBaseOggetti[Indice].TypeObject;
	if (TipoItem!=8) {
		// se pero' camera/sink  or fixed camera considerarlo come moveable
		if (TipoItem == 0x4400 || TipoItem == 0x4800 ||
			TipoItem == 0x4080) return true;

		if (TestExport==true && TipoItem == 0x4040) return true;
		return false;
	}

	SlotIndex = pVetBaseOggetti[Indice].SlotIndex;


	if (pVetSlotOggetti[SlotIndex].Tipo != 0x10) return false;


	return true;



}
__declspec(naked) void PatchActionTrigger(void)
{
	__asm {
		
		// salvare puntatore a record trigger
		// che viene attualmente elaborato
	
		lea  eax, PtrUltimoTrigger
		mov dword ptr [eax], esi
		// salvare anche totdati floor attuale
		lea  eax, SalvaTotFloorData
		mov ecx, 566660h    ;TotDatiPerMemFloorData
		mov cx, word ptr [ecx]
		mov word ptr [eax], cx


		mov  cx, word ptr [esi+8]
		movsx eax, cx

		// provare a saltare ad object anche quando e' FLYBY, CAMERA o SINK
		cmp  eax, 1     // CAMERA
		jz  SaltoPerObject
		cmp  eax, 2			//  SINK
		jz  SaltoPerObject

		cmp  eax, 0bh  ;ACTION
		jnz  VediAltro
		// e' Action trigger
		// se pero' non punta ad un moveable ma ad uno static
		// allora lasciare indice senza modifiche
		mov ax,  [esi+02h]  ;OggettoDaTriggerare 
		pushad
		push 0
		push eax 
		call IsMoveable
		add esp,8
		mov dword ptr [EsitoMoveable], eax
		popad
		cmp byte ptr [EsitoMoveable], 0
		jnz SaltoPerObject
		jmp Salto3

VediAltro:
		test eax, eax
		jnz   Salto1
		// salto per trigger what action o object
SaltoPerObject:
		mov  eax, 430391h  ;go_triggerWhat_OBJECT
		jmp  eax
Salto1:
		cmp  eax, 6
		jnz  Salto2
		// salto per go_TriggerWhat_TARGET
		mov  eax, 430366h  ;go_TriggerWhat_TARGET
		jmp  eax
Salto2:
		cmp  eax,8
		jnz  Salto3
		// salto per CD
		mov  eax, 43031ah   ;salta a trigger CD
		jmp  eax
Salto3:
		// salto per go_TriggerWhat_generico
		mov  eax, 430340h   ;go_TriggerWhat_generico
		jmp  eax
	}

}
__declspec(naked) void PatchAggiuntaTrigger(void) 
{
	__asm {
		pushad
		lea  eax, CorreggiTriggerOut
		call  eax
		popad
		retn

	}

}


void PatchTotSizeMemoriaTexture8Bits(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x40702D, 'D'},
		{0x42A6FB, 'D'}};

	TotPatchs=2;
	TotOld = 0x4000;
	TotNew=0x10000;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchTuttoTexture8Bits(void)
{
	PatchMemoriaTexture8Bits();
	PatchTotSizeMemoriaTexture8Bits();

}
// cerca di monitorare con try catch la procedura in textureoggetto
void Patch_TextureOggetto1(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x15, 0x38, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x402C13;

	for (i=0;i<9;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_MemDinamicaVertici(void)
{
	int  i;
	BYTE VetBytes[]={0x68, 0x8, 0x58, 0x2, 0x0, 0xE8, 0x80, 0xFB, 0xFF, 0xFF, 
			0x68, 0xEC, 0xA8, 0x45, 0x0, 0x68, 0x0, 0xC8, 0x0, 0x0, 
			0xA3, 0xF8, 0x97, 0x46, 0x0, 0x8B, 0xF0, 0xE8, 0x6A, 0xFB, 
			0xFF, 0xFF, 0x8B, 0xD, 0xF8, 0x97, 0x46, 0x0, 0x83, 0xC4, 
			0x10, 0x85, 0xC9, 0xA3, 0xFC, 0x97, 0x46, 0x0, 0x74, 0x25, 
			0x85, 0xC0, 0x74, 0x21, 0x68, 0xD4, 0xA8, 0x45, 0x0, 0x68, 
			0x0, 0x80, 0xC, 0x0, 0x66, 0xC7, 0x46, 0x2, 0x0, 0x64};

	BYTE *pMem = (BYTE*) 0x41A8B6;

	for (i=0;i<70;i++) {
		pMem[i] = VetBytes[i];
	}
}
void PatchTotSizePoligoni(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x439E10, 'D'},
		{0x439E7B, 'D'},
		{0x43A3A8, 'D'}};

	TotPatchs=3;
	TotOld = 0x1388;
	TotNew=0x4E20;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}
void PatchMemoriaPoligoni(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pMemoriaPoligoni;

	StrPatch VetPatch[] = {{0x4028A4, true},
		{0x4028AD, true},
		{0x402BBC, true},
		{0x402DAE, true},
		{0x43A390, true},
		{0x43A3CB, true},
		{0x43A3D4, true},
		{0x43A40B, true},
		{0x43A412, true},
		{0x43A41F, true},
		{0x43A42B, true},
		{0x43A438, true},
		{0x43A448, true},
		{0x43A44F, true},
		{0x43A456, true},
		{0x43A45D, true},
		{0x43A464, true},
		{0x43A46B, true},
		{0x43A472, true},
		{0x43A479, true},
		{0x43A4CB, true},
		{0x43A4D2, true},
		{0x43A4DF, true},
		{0x43A4EB, true},
		{0x43A4F8, true},
		{0x43A508, true},
		{0x43A50F, true},
		{0x43A516, true},
		{0x43A51D, true},
		{0x43A524, true},
		{0x43A52B, true},
		{0x43A532, true},
		{0x43A539, true},
		{0x43A579, true}};

	OffsetOldInizio = 0x496520;
	OffsetOldFine=0x4BD620;
	TotPatchs=34;
	// dimensione originale=0x27100
	pMemoriaPoligoni = (DWORD *) 0x45461E;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio,
			OffsetOldFine, 0x9C400,
			"MemoriaPoligoni");
	*pMemoriaPoligoni = OffsetNew;
}

void PatchTuttoMemoriaPoligoni(void)
{

	PatchMemoriaPoligoni();
	PatchTotSizePoligoni();
}
// room editor
void PatchVetTexSonorePrj(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pVetTexSonorePrj;

	StrPatch VetPatch[] = {{0x427028, true},
		{0x427039, true},
		{0x427A96, true},
		{0x42AE02, true},
		{0x42BFAA, true},
		{0x42C360, true},
		{0x42CAD8, true},
		{0x42CBEC, true}};

	OffsetOldInizio = 0x569D80;
	OffsetOldFine=0x569E80;
	TotPatchs=8;
	pVetTexSonorePrj = (DWORD *) 0x45463E;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs,OffsetOldInizio, 
			OffsetOldFine, MAX_TAIL_INFOS,"VetTexSonorePrj");
	*pVetTexSonorePrj = OffsetNew;
}

void PatchTot_TexSonore(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x42701E, 'D'},
		{0x42ADF8, 'D'}};

	TotPatchs=2;
	TotOld = 0x40;
	TotNew=0x100;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

void PatchMax_Texture(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x428E36, 'D'},
		{0x42AE5E, 'D'},
		{0x42B672, 'D'}};

	TotPatchs=3;
	TotOld = 0x100;
	TotNew=MAX_TAIL_INFOS;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}



void PatchSizeMappaTexture(void)
{
	int TotPatchs;
	DWORD TotOld;
	DWORD TotNew;
	StrTotElementi VetPatch[] = {{0x429C34, 'D'}};

	TotPatchs=1;
	TotOld = 0x100000;
	TotNew=  SIZE_MAP_TEXTURE;
	ApplicaPatchDato(VetPatch,TotPatchs,TotOld,TotNew);
}

// patch dinamica fatta SOLO per creare un eccezione quando si preme face edit
void Patch_GeneraEccezioneFaceEdit(void)
{
	int  i;
	BYTE VetBytes[]={0x33, 0xC0, 0xC7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x40176A;

	for (i=0;i<16;i++) {
		pMem[i] = VetBytes[i];
	}
}
// redirezione gestore eccezioni 1
void Patch_GestoreEccezioni_1(void)
{
	int  i;
	BYTE VetBytes[]={0xB0, 0x1, 0xFF, 0x25, 0x4C, 0x4F, 0x45, 0x0, 0x90, 0x90, 
			0x90};
	BYTE *pMem = (BYTE*) 0x44D2E8;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_GestoreEccezioni_2(void)
{
	int  i;
	BYTE VetBytes[]={0xB0, 0x2, 0xFF, 0x25, 0x4C, 0x4F, 0x45, 0x0, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x44D3C0;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_LogPerLoopBox(void)
{
	int  i;
	BYTE VetBytes[]={0xFF, 0x25, 0x94, 0x4F, 0x45, 0x0, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4051D0;

	for (i=0;i<8;i++) {
		pMem[i] = VetBytes[i];
	}
}
void patch_PerSalvataggioTriggerWhat(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x55, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x430340;

	for (i=0;i<15;i++) {
		pMem[i] = VetBytes[i];
	}
}

void Patch_PerInitPreviewRoom(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x57, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xC3, 0x90};
	BYTE *pMem = (BYTE*) 0x4256B2;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}

void Patch_PerEndPreviewRoom(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x58, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xE9, 0xED, 0xE, 0x1, 0x0, 0x90};
	BYTE *pMem = (BYTE*) 0x42570C;

	for (i=0;i<16;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_PerSalvaTexAnim(void)
{
	int  i;
	static BYTE VetBytes[]={0x8B, 0x4C, 0x24, 0x4, 0x66, 0xB8, 0x56, 0x0, 0xFF, 0x15, 
			0x4, 0x4F, 0x45, 0x0, 0xC3, 0x90};
	BYTE *pMem = (BYTE*) 0x4325A0;

	for (i=0;i<16;i++) {
		pMem[i] = VetBytes[i];
	}
}
void Patch_PerSalvaWaterIntensity(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x59, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42E5E2;

	for (i=0;i<14;i++) {
		pMem[i] = VetBytes[i];
	}
}




// per room editor
// Creata in data: 1/27/2008 1:15:05 AM
// Chiamata da: 0x42069F
// MOV AX,5A                                            ;  ASCII "WARNING: Version %d meshes, version %d objects.h"
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerSoundTableEstesa_NGLE(void)
{
	int  i;
	static BYTE VetBytes[]={0x8B, 0xD0, 0x66, 0xB8, 0x5A, 0x0, 0xFF, 0x15, 0x4, 0x4F, 
			0x45, 0x0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42069D;

	for (i=0;i<34;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 3/22/2008 11:09:16 PM
// Chiamata da: 0x42A3A5
// MOV AX,5B
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerFindAndReplace1(void)
{
	int  i;
	static BYTE VetBytes[]={0x8B, 0xD0, 0x66, 0xB8, 0x5B, 0x0, 0xFF, 0x15, 0x4, 0x4F, 
			0x45, 0x0, 0x72, 0x12, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42A3A3;

	for (i=0;i<30;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 3/23/2008 1:08:03 AM
// Chiamata da: 0x42A4B8
// MOV AX,5C
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]                   ;  ASCII "Texture not found!"

void Patch_PerFindAndReplace2(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x5C, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42A4B8;

	for (i=0;i<70;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 4/26/2008 11:56:45 PM
void Patch_PerControlloCameraWhat(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0x83, 0x7E, 0x8, 0x1, 0x7F, 0x2C};
	BYTE *pMem = (BYTE*) 0x42D9E9;

	for (i=0;i<7;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Creata in data: 6/14/2008 8:44:53 AM
// Chiamata da: 0x4320B6
// MOV AX,5D
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerSizeTexAnimate(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x5D, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4320B6;

	for (i=0;i<29;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 9/8/2008 5:22:04 PM
// Chiamata da: 0x431DF5
// MOV AX,5E                                           ;  patch per azzerare indicenow di posizione di static nella stanza
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerAzzeraIndiceStatic(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x5E, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x431DF5;

	for (i=0;i<13;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 9/8/2008 6:02:15 PM
// Chiamata da: 0x431EE3
// MOV AX,5F                                           ;  OCB Code
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerSalvaIndiceStatic(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x5F, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x431EE3;

	for (i=0;i<14;i++) {
		pMem[i] = VetBytes[i];
	}
}

void PatchZonaVetCamera(void)
{
	int TotPatchs;
	DWORD OffsetOldInizio;
	DWORD OffsetNew;
	DWORD OffsetOldFine;
	DWORD *pZonaVetCamera;

	static StrPatch VetPatch[] = {{0x42E29B, true},
		{0x42E2A8, true},
		{0x42E2B3, true},
		{0x42E32A, true},
		{0x42E347, true},
		{0x42E34F, true},
		{0x42E359, true},
		{0x4303C0, true},
		{0x4303D0, true}};

	OffsetOldInizio = 0x564FA0;
	OffsetOldFine=0x5651A0;
	TotPatchs=9;
	pZonaVetCamera = (DWORD *) 0x454652;
	OffsetNew = ApplicaPatchMemoria(VetPatch,TotPatchs, 
				OffsetOldInizio,OffsetOldFine, 0x800,"ZonaVetCamera");
	*pZonaVetCamera = OffsetNew;
}


// Creata in data: 12/20/2008 11:38:59 AM
// Chiamata da: 0x430216
// MOV AX,60
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerOutputTriggerCamera(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x60, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x430216;

	for (i=0;i<16;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 12/22/2008 8:33:14 PM
// Chiamata da: 0x4244EC
// MOV AX,61
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerTriggerObject(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x61, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4244EC;

	for (i=0;i<15;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 6/19/2009 1:22:17 PM
// Chiamata da: 0x4212D1
// MOV AX,62
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerOggettoSconosciuto(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x62, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4212D1;

	for (i=0;i<19;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 6/19/2009 9:00:05 PM
// Chiamata da: 0x423630
// MOV AX,63                                                                                        ;  questa procedura usato per cercare un item (trigger, oggetto, door, effetto) presente nella casella di input di stanza attuale alla fine restituisce un indice per vetbaseoggetti ESP+10 		push 00h   ;a
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerSalvaCordScansione(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x63, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90};
	BYTE *pMem = (BYTE*) 0x423630;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Creata in data: 7/15/2009 12:44:30 AM
// Chiamata da: 0x426E44
// MOV AX,30                                                                                        ;  patch per segnalare che il load tga viene da caricamento progetto
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerFindMappaTexture(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x30, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x72, 0x15, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x426E44;

	for (i=0;i<26;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Creata in data: 7/29/2009 10:50:53 PM
// Chiamata da: 0x427129
// MOV AX,64
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerLoadPathWad(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x64, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xF, 0x82, 0x2E, 0x2, 0x0, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x427129;

	for (i=0;i<57;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 7/30/2009 4:10:50 PM
// Chiamata da: 0x426DFA
// MOV AX,65
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerLoadPathTGA(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x65, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0xF, 0x82, 0xA8, 0x1, 0x0, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x426DFA;

	for (i=0;i<57;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 8/19/2009 4:00:34 PM
// Chiamata da: 0x42E6FF
// MOV AX,66                                  ;  salvare floor data (triggers, suoni)
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]    ;  handle file .tom

void patch_PerCollisioniTriangoli(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x66, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42E6FF;

	for (i=0;i<20;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 8/28/2009 7:21:11 PM
// Chiamata da: 0x40D007
// MOV AX,67
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerPannelliCollisione(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x67, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x40D007;

	for (i=0;i<13;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 10/2/2009 4:54:52 PM
// Chiamata da: 0x4203D5
// MOV AX,68                                                        ;  fare patch per salvare i nomi originali di objects.h
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerDopoObjects(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x68, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4203D5;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 7/13/2011 2:34:58 AM
// Chiamata da: 0x419E27
// MOV AX,69
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerCreaButtonWindow(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x69, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x419E27;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 7/13/2011 2:48:05 AM
// Chiamata da: 0x4125A0
// MOV AX,6A                                            ;  INT ButtonText(bool_gad *, char *) INIZIO PROCEDURA 180 ;  SIZE=0x60
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerCatturaPulsanti(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x6A, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0};
	BYTE *pMem = (BYTE*) 0x4125A0;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 13/07/2011 10:45:06
// Chiamata da: 0x0
// CALL DWORD PTR DS:[<PatchCatturaMenu>]          ;  in ecx si punta a procedura di menu

void Patch_perCatturaMenu(void)
{
	int  i;
	static BYTE VetBytes[]={0xFF, 0x15, 0xA4, 0x4F, 0x45, 0x0};
	BYTE *pMem = (BYTE*) 0x4120F1;

	for (i=0;i<6;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Creata in data: 7/23/2011 3:39:59 PM
// Chiamata da: 0x414910
// MOV AX,6B                                       ;  e' uguale a SetButton ma qui si deve fornire come primo argomento anche il nuovo testo da assegnare 14: Arg1: Push Mex_D  ;Nuovo testo da assegnare 10: Arg2: push 00h   ;0=non premuto, mentre 1= premu
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]         ;  Arg4: Colore fondo

void patch_PerAggiornaPulsante(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x6B, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0};
	BYTE *pMem = (BYTE*) 0x414910;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 7/24/2011 11:37:59 AM
// Chiamata da: 0x444990
// MOV AX,6C                                       ;  INIZIO PROCEDURA 807 ;  SIZE=0x50
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerAggiornaMouse(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x6C, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x444990;

	for (i=0;i<14;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 10/08/2011 11:31:33
// Chiamata da: 0x44411D
// MOV AX,6D
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perSimlulaPressioneTasto(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x6D, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x44411D;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}
// Creata in data: 8/17/2011 5:51:17 PM
// Chiamata da: 0x4229C1
// MOV AX,6E
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void patch_PerBugChangeObject(void)
{
	int  i;
	static BYTE VetBytes[]={0x8B, 0xC8, 0x66, 0xB8, 0x6E, 0x0, 0xFF, 0x25, 0x4, 0x4F, 
			0x45, 0x0, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x4229BF;

	for (i=0;i<14;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 18/08/2011 09:43:56
// Chiamata da: 0x419FBD
// MOV AX,6F
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perBugPlaceTarget(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x6F, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x419FBD;

	for (i=0;i<23;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 31/07/2013 16.21.36
// Chiamata da: 0x422E17
// MOV AX,70
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPuntatorePlaceObj(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x70, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0};
	BYTE *pMem = (BYTE*) 0x422E17;

	for (i=0;i<10;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 31/07/2013 16.48.18
// Chiamata da: 0x422F0F
// MOV AX,71
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPuntatorePlaceObj2(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x71, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90};
	BYTE *pMem = (BYTE*) 0x422F0F;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 31/07/2013 21.15.08
// Chiamata da: 0x422CA0
// MOV AX,72
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPuntatoreMoveObj(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x72, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x422CA0;

	for (i=0;i<30;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 31/07/2013 21.25.41
// Chiamata da: 0x422D2C
// MOV AX,73
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPuntatoreMoveObj2(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x73, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90};
	BYTE *pMem = (BYTE*) 0x422D2C;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 12/08/2013 14.22.23
// Chiamata da: 0x0
// JMP DWORD PTR DS:[<PatchGetPosition>]              ;  INIZIO PROCEDURA 267 ;  SIZE=0x270

void Patch_perGetPosition(void)
{
	int  i;
	static BYTE VetBytes[]={0xFF, 0x25, 0xA8, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x419FF0;

	for (i=0;i<9;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 14/08/2013 13.46.04
// Chiamata da: 0x427547
// MOV AX,74
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perSalvaProgetto(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x74, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x427547;

	for (i=0;i<19;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Creata in data: 14/08/2013 14.41.19
// Chiamata da: 0x422D97
// MOV AX,75
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPlaceObject(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x75, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90};
	BYTE *pMem = (BYTE*) 0x422D97;

	for (i=0;i<11;i++) {
		pMem[i] = VetBytes[i];
	}
}

// Creata in data: 14/08/2013 15.59.23
// Chiamata da: 0x42D77D
// MOV AX,76
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_perPlaceTrigger(void)
{
	int  i;
	static BYTE VetBytes[]={0x66, 0xB8, 0x76, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90};
	BYTE *pMem = (BYTE*) 0x42D77D;

	for (i=0;i<12;i++) {
		pMem[i] = VetBytes[i];
	}
}


// Date creation: 30/01/2014 9.58.48
void Patch_perTriggerPlugin2(void)
{
	static BYTE VetBytes[]={0x80, 0x3C, 0x56, 0x10, 0x90};

	ApplyCodePatch(0x42DA9A, VetBytes, 5);
}

// Date creation: 30/01/2014 10.01.23
void Patch_perTriggerPlugin3(void)
{
	static BYTE VetBytes[]={0x80, 0x3C, 0x4A, 0x0, 0x90};

	ApplyCodePatch(0x42DAB5, VetBytes, 5);
}

// Date creation: 30/01/2014 23.25.54
void Patch_perTriggerPlugin4(void)
{
	static BYTE VetBytes[]={0x80, 0x3E, 0x0, 0x90};

	ApplyCodePatch(0x430248, VetBytes, 4);
}





// cambia l'accesso a word TirggerAttivaction in modo che sia byte
// cosi' si lascia un byte per il tipo plugin
void Patch_perTriggerPlugin(void)
{
	static DWORD VetAdr[] = {0x41282F, 0x412860,0x412891, 0x4128CD, 
		0x413802, 0x413828, 0x41385B, 0x4138A9, 0x42D502, 0};
	int i;
	BYTE *pMem;

	i=0;
	while (VetAdr[i]!= NULL) {
		pMem= (BYTE *) VetAdr[i];
		i++;

		if (*pMem != 0xBF) {
			// errore
			MioMessageBox("ERROR: patch trigger plugin for byte access","Error");
		}else {
			*pMem = 0xbe;
		}
	}

	Patch_perTriggerPlugin2();
	Patch_perTriggerPlugin3();
	Patch_perTriggerPlugin4();


}

// patch per numero di obj texture (tail info)
// nota: il buffer di memoria l'avevo gia' espanso ma poi avevo dienticato di ampliare
// anche il humero corrispondnete di tail info caricabili (da 2500 a 10000)
void PatchTotObjTailInfo(void)
{
	static DWORD VetAdr[] = {0x4206DD, 0x4206F9, 0x420720, 0};
	int i;
	DWORD *pMem;

	i=0;
	while (VetAdr[i]) {
		pMem = (DWORD *) VetAdr[i++];
		*pMem = 0x2710;  // 10.000
	}
}

// Date creation: 20/09/2016 21.25.41
void Patch_PerExpandObjTexConvert(void)
{
	static BYTE VetBytes[]={0x68, 0x0, 0x0, 0x10, 0x0, 0xE8, 0x61, 0xE6, 0xFE, 0xFF, 
			0x8B, 0xD8, 0xB9, 0x0, 0x0, 0x4, 0x0, 0x33, 0xC0, 0x8B, 
			0xFB, 0xF3, 0xAB, 0x8B, 0x4C, 0x24, 0x44, 0xB8, 0x56, 0x55, 
			0x55, 0x55, 0xF7, 0xE9, 0x8B, 0xC2, 0x68, 0x64, 0xCA, 0x45, 
			0x0, 0xC1, 0xE8, 0x1F, 0x3, 0xD0, 0x89, 0x5C, 0x24, 0x24, 
			0x8B, 0xF2, 0x8D, 0x8E, 0x0, 0x0, 0x1, 0x0};

	ApplyCodePatch(0x42BDD5, VetBytes, 58);
}

// Date creation: 18/11/2016 12.24.02
// Called from: 0x44340A
// MOV AX,78                                                       ;  ASCII "No Matching Device Found"
// CALL DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerErrDeviceNotFound(void)
{
	static BYTE VetBytes[]={0x66, 0xB8, 0x78, 0x0, 0xFF, 0x15, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90, 0x90, 0x90, 0x90};

	ApplyCodePatch(0x44340A, VetBytes, 15);
}

// Date creation: 18/11/2016 13.36.45
// Called from: 0x0
// JMP DWORD PTR DS:[<PatchCreateDXError>]

void Patch_PerErrCreateDirectX(void)
{
	static BYTE VetBytes[]={0xFF, 0x25, 0xAC, 0x4F, 0x45, 0x0, 0x90, 0x90, 0x90, 0x90, 
			0x90, 0x90, 0x90, 0x90};

	ApplyCodePatch(0x4443D1, VetBytes, 14);
}

// Date creation: 19/01/2017 0.06.19
// Called from: 0x40FD80
// MOV AX,78                                        ;  INIZIO PROCEDURA 151 ;  SIZE=0x138
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerNomeStanza(void)
{
	static BYTE VetBytes[]={0x66, 0xB8, 0x79, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0};

	ApplyCodePatch(0x40FD80, VetBytes, 10);
}

// Date creation: 19/01/2017 1.53.29
// Called from: 0x42BAA2
// MOV AX,7A
// JMP DWORD PTR DS:[<Patch_GenericheAsm>]

void Patch_PerFixBugPalette(void)
{
	static BYTE VetBytes[]={0x66, 0xB8, 0x7A, 0x0, 0xFF, 0x25, 0x4, 0x4F, 0x45, 0x0, 
			0x90, 0x90};

	ApplyCodePatch(0x42BAA2, VetBytes, 12);
}




void StartUpRoomEditor(void)
{
	InizializzaRoom();
	// qui disattiva espansione di memoria a seconda di flag preferenze
	if ((MyPrefRoom.Flags & FPR_NO_EXTENDED_MEMORY)==0) {

		PatchZonaMemoriaVertex();			// si direbbero necessarie
		PatchZonaMemoriaVertexSecondario(); 
		Patch_MemDinamicaVertici();  
		PatchMemoriaTexFaces();  
		PatchTuttoMemoriaPoligoni(); // necessaria
		// PatchMemoriaTextureDD(); //  provare a toglierla
		PatchTuttoSegnaliMesh();
	}
	PatchTuttoTexture8Bits();
	PatchTutto2000Oggetti();
	PatchTuttoOverlap();

	//PatchZonaMemAlfaTexWad();
	//PatchZonaMemBetaTexWad();  // beta e delta sono in realta
	//PatchZonaMemDeltaTexWad();  // associate con palette
	PatchVetTexSonorePrj();

	PatchTot_TexSonore();
	PatchVetTextureBumpMap();
	PatchVetAnimatedTilesTextures();
	PatchMax_Texture();
	PatchMax_ScrollY();
	PatchSizeMappaTexture();
	// PatchZonaPeTextureWad();
	// PatchVetPtrObjectsItems();
	Patch_AnimaTexture();


	Patch_GestoreEccezioni_1(); 
	Patch_GestoreEccezioni_2();
	// Patch_GeneraEccezioneFaceEdit(); // genera eccezione (ma lo becca in try()
	// Patch_GeneraEccezioneThread();  // crea eccezione
	// Patch_FastRotate(); // accellera parametri di rotazione e zoom
	//Patch_BloccaCiclo();  // evita sovrapposizioni di esecuzione (non serve a niente)
	// Patch_LockSurface(); // esce da proc se lock fallisce
	// Patch_CambioFlagLock();
	Patch_TextureOggetto1();  // prova a monitrare gestionetextureoggetto con try
	Patch_ChiamaRotazioni3d(); // chiama aggancio a nuove rotazioni 3d
	Patch_ChiamaSalvaMappa2d(); // chiama patch per salvare mappa 2d di output wad
	ImpostaSizePreview(); // lavora come una patch per modificare dimensione preview
	Patch_ErroriDirectX(); // redireziona gestioneerroridirectx
	Patch_LinkCalcoloBreveIota(); // per impedire loop infinito
	//Patch_CreaErroreDirectX(); // crea un errore directx
	// Patch_CreaEccezioneInMostraSpot(); // crea un eccezione subito dopo eerroedirectx
	Patch_PerWriteFile();  // patch per contare byte scritti in file .tom


	Patch_LogPerLoopBox(); // visualizza room e (x,y) di crea zona box
	//if ((MyPrefRoom.Flags & FPR_DISABLE_NEW_TRIGGERS) == 0) {
		Patch_FinestraTrigger();
		patch_PerOutTrigger1(); // prima patch per output trigger in tom
		patch_PerOutTrigger2(); // corregge trigger appena scritto
		patch_PerOutTrigger3();  // ignora timer diverso come speciale
		patch_PerSalvataggioTriggerWhat(); // aggiusta triggerwhat+valueoggetto
		GlobNgle.FlagsLevelTr4 |= FLT_NEW_TRIGGERS;
	//}

	Patch_PerSalvaTexAnim(); // crea aggancio per salvare tex animate in tom
	Patch_PerInitPreviewRoom(); // aggancio per inizializzare preview
	Patch_PerEndPreviewRoom(); // aggancio per chiamare endpreview
	Patch_PerSalvaWaterIntensity(); //per salvare extra info room in ng header
	patch_PerSoundTableEstesa_NGLE();
	PatchZonaListaElenco();
	PatchTotRecordElenco();
	Patch_Per2Sun();
	Patch_PerFindAndReplace1();
	Patch_PerFindAndReplace2();
	Patch_PerControlloCameraWhat();
	Patch_PerSizeTexAnimate();
	patch_PerAzzeraIndiceStatic();
	patch_PerSalvaIndiceStatic();
	PatchZonaVetCamera();
	patch_PerOutputTriggerCamera();
	patch_PerTriggerObject();
	patch_PerOggettoSconosciuto();
	patch_PerSalvaCordScansione();
	patch_PerFindMappaTexture();
	patch_PerLoadPathWad();
	patch_PerLoadPathTGA();
	patch_PerCollisioniTriangoli();
	patch_PerPannelliCollisione();
	Patch_PerDopoObjects();
	patch_PerCreaButtonWindow();
	patch_PerCatturaPulsanti();
	Patch_perCatturaMenu();
	patch_PerAggiornaPulsante();
	patch_PerAggiornaMouse();
	Patch_perSimlulaPressioneTasto();
	patch_PerBugChangeObject();   // versione 1.2.2.7
	Patch_perBugPlaceTarget();
	Patch_perBloccoAttesa();
	Patch_perPuntatorePlaceObj();
	Patch_perPuntatorePlaceObj2();
	Patch_perPuntatoreMoveObj();
	Patch_perPuntatoreMoveObj2();
	Patch_perGetPosition();
	Patch_perSalvaProgetto();
	Patch_perPlaceObject();
	Patch_perPlaceTrigger();
	Patch_perHiDEobjdrawroom();

	Patch_perTriggerPlugin();   // versione 1.3.0.0
	PatchTotObjTailInfo();
	Patch_PerExpandObjTexConvert();
	Patch_PerErrDeviceNotFound();
	Patch_PerErrCreateDirectX();

	Patch_PerNomeStanza();	// versione 1.3.0.4

}
void InizializzaTom2Pc(void)
{

	DWORD *pTotPagineTexTemp = (DWORD *) 0x401A16;

	memset(&GlobNgle, 0, sizeof(StrGlobNgle));
	GlobNgle.RoomTexAnimate.TestPresente=false;
	GlobNgle.RoomTexAnimate.SizeDefault=64;
	NomeTr4Pc[0]=0;
	// qua effettuare una chiamata a controlladebuggerpc() ma effettuarlo
	// in una prcoedure fasulla nidificata
	GosubFasullaPapera();
	*pTotPagineTexTemp = 7;

	// modificare il nome del programma
#ifdef CRYPTEXE
	// ciamare funzione che altera i salti indiretti di createwindow 
	AlteraChiamateAPI();
#else
	char *pTitolo = (char *) 0x416420;
	sprintf(pTitolo,"NG - PC Level Converter (v. %s)", MexVersione);
#endif

	remove("log_tom2pc.txt");
	CalcolaVersione(&GlobNgle.BaseVersione);
	BaseMiaMemory.pBaseMem=NULL;
	BaseMiaMemory.SizeTotale=0;
}



bool IsResumeCrashDisabledInNgle(void)
{
	if (MyPrefRoom.Flags & FPR_NO_RESUME_CRASH)  return true;
	return false;

}
// chiamata quando e' appena finito l'output wad per salvare
// la mappa2d se c'e' voce nelle preferenze
__declspec(naked) void PatchSalvaMappa2d(void)
{
	__asm {
		pushad
		call  SalvareMappa2d
		call  RefreshUsedTriggersByPlugins
		mov eax, offset GlobNomeTom
		push eax
		call	SalvaDatiNGTomFile
		add esp,4
		popad
		mov eax, 4697E4h    ;TestNienteMessaggi
		mov  eax, dword ptr [eax]
		test eax, eax
		retn

	}
}

// gestione fallimewnto in createdirectx
// chiamata con jmp
__declspec(naked) void PatchCreateDXError(void)
{
	__asm {
		cmp al, 0
		jnz  TuttoOk

		pushad 
		call MostraErrCreateDx
		popad
		
		mov ecx, 44B5F3h
		jmp ecx

		// codice eliminato
TuttoOk:
	mov eax, 46E559h    ;HandleMainWindow
	mov  eax, dword ptr [eax] 
	push 06h	; /Flags = SWP_NOMOVE|SWP_NOZORDER 
	push ecx	; |Height 
	sub  edx, [esp+18h]  ; (LOCALE_ESP - 0x68) 
	// tornare a codice originale
	mov ecx, 4443DFh
	jmp ecx


	}
}
void TerminaRoomEditor(void)
{
	FILE *pFile;


	pFile = fopen(GetFileTrle(NomePrefRoom),"wb");
	if (pFile == NULL) {
		sprintf(BufferLog,"ERROR trying to save preference file in \"%s\"",
			NomePrefRoom);
		InviaLog(BufferLog);
	}else{

		fwrite(&MyPrefRoom,sizeof(MyPrefRoom),1,pFile);
		fclose(pFile);
		InviaLog("Saved preferences");

	}

	// eliminare (se allocato) il bitmap delle texture
	if (HandleBitMap != NULL) {
		DeleteObject(HandleBitMap);
		HandleBitMap=NULL;
	}

	DeleteObject(PenNulla); 
	DeleteObject(PenNero);
	DeleteObject(PenRosso);
	DeleteObject(PenVerde);
	DeleteObject(BrushNero);
	DeleteObject(BrushRosso);

	// rilasciare memoria con nomi originali di slot salvsati


	if (pVetSalvaNomiSlot) free(pVetSalvaNomiSlot);




}
// chiamata alla fine di find and replace per ngle
void FineFindReplace(void)
{
	DWORD MemFind, MemReplace;
	int Differenza;
	StrTailInfo** p2VetTailInfos = (StrTailInfo**) 0x506A78;
	StrTailInfo * pVetTailInfos;
	int i;
	int IndiceFind;
	int IndiceParziale;
	short *VetIndiciFind = (short*) 0x569E80;  // VetIndiciTailDaCercare:
	// ora effettuare anche sostituzione di eventuali indici parziali
	// (ossia tutte tranne che indicefind full)
	pVetTailInfos= *p2VetTailInfos;
	IndiceFind = DatiFindReplace.IndiceFind;
	MemFind = (DWORD) pVetTailInfos[IndiceFind].pMemImage;
	MemReplace = (DWORD) pVetTailInfos[DatiFindReplace.IndiceReplace].pMemImage;

	Differenza = MemReplace - MemFind;

	// ora sostituire le tail infos di tutte le tex parziali
	// in modo da farle puntare alla zona di memoria della tex replace
	i=0;
	while (VetIndiciFind[i] != -1) {
		IndiceParziale = VetIndiciFind[i++];
		if (IndiceParziale != IndiceFind) {
			// questa e' una tex parziale
			pVetTailInfos[IndiceParziale].pMemImage += Differenza;
			VetBigTails[IndiceParziale].pMemImage += Differenza;
		}
	}
	sprintf(BufferLog,"Replaced %d textures", DatiFindReplace.TotTrovate);
	MostraMessageBoxNGLE(BufferLog,7);
	DatiFindReplace.TestAttivo=false;
	
}
// salvare i puntatori in 
// DWORD *pVetPtrOriginaliMesh=NULL;
// int TotPtrOriginaliMesh=0;
void SalvaVetPtrMesh(DWORD *pVetMesh, int TotMesh)
{
	int  i;

	if (pVetPtrOriginaliMesh==NULL) {
		pVetPtrOriginaliMesh = (DWORD *) malloc(TotMesh * 4);
	}else
	{
		pVetPtrOriginaliMesh = (DWORD *) realloc(pVetPtrOriginaliMesh, TotMesh * 4);
	}
	TotPtrOriginaliMesh=TotMesh;
	for (i=0;i<TotMesh;i++) {
		pVetPtrOriginaliMesh[i] = pVetMesh[i];
	}

}
// viene chiamata per salvare i puntaotri originali delle mesh degli oggetti
// prima che vengano modificati
// in eax c'e' il numero di puntaotri 
__declspec(naked) void Patch_SalvaMeshPointers(void)
{
	__asm {
	

	mov ecx, 46B828h    ;Ptr_VetMeshPointers
	mov  ecx, dword ptr [ecx]
	xor  edi, edi
	pushad

	push	eax		;N_Pointers
	push	ecx		;base di vettore puntaorri

	lea		eax, SalvaVetPtrMesh
	call	eax
	add		esp,8

	popad
	retn
	}
}

__declspec(naked) void PatchCalcoloBreveIota(void)
{
	__asm {
	mov  eax, [esp+04h]
	xor  cl, cl
	test al, 01h
	jnz  Salto2
	test eax, eax
	jz	Esci
Salto1:
	shr  eax, 1h
	inc  cl
	test al, 01h
	jz   Salto1
Salto2:
	mov  edx, [esp+08h]
	mov  [edx], cl
	xor  cl, cl
	test al, 01h
	jz   Salto4
Salto3:
	shr  eax, 1h
	inc  cl
	test al, 01h
	jnz  Salto3
	mov  eax, [esp+0ch]
	mov  [eax], cl
	ret
Salto4:
	mov  edx, [esp+0ch]
	mov  [edx], cl
Esci:
	ret


	}
}
// usata per salvare base stack di thread secodnario in room editor
__declspec(naked) void PatchSalvaStack2(void)
{
	__asm {
		// salvare ebp (che contiene top stack)
		mov  eax, 454FECh   ; SalvaBaseStack2
		mov  dword ptr [eax], ebp  
		// codice eliminato da patch
		push  0ffh
		push  456D38h ;LOC_456D38
		mov   eax, 44B42Eh  ;torna subito dopo patch
		jmp   eax


	}

}

void CatturaMenu(char *pNome, TYPE_GenericaSubPulsante *pCall)
{
	int i;

	i = BaseWindow.TotStandardMenu;

	BaseWindow.VetStandardMenu[i].pCallMenu = pCall;
	BaseWindow.VetStandardMenu[i].pNome = pNome;
	BaseWindow.TotStandardMenu ++;
}
__declspec(naked) void PatchCatturaMenu(void) 
{
	__asm {
	mov  ecx, [ebp+00h]  ;in ecx si punta a procedura di menu 
			;menttre in edx c'e' testo menu 
	add  ebp, 04h 

	cmp byte ptr [BaseWindow.TestStopMemo], 0
	jnz Ignora
	pushad
	push ecx // procedura
	push edx // nome
	call CatturaMenu
	add esp, 8
	popad
Ignora:
	retn

	}
}
__declspec(naked) int MyGetPosition(void)
{
	__asm {
	mov eax, 4342E0h    ;wait_release
	call eax	; INIZIO PROCEDURA 267 ;  SIZE=0x270 
	push ebx 
	push ebp 
	push esi 
	push edi 
	mov eax, 419FF9h
	jmp eax

	}
}

__declspec(naked) void PatchGetPosition(void)
{
	__asm {
		mov eax, esp  // salvare inizio stack
		mov ecx, eax

		mov eax, dword ptr [ecx+0ch]
		push eax

		mov eax, dword ptr [ecx+08h]
		push eax

		mov eax, dword ptr [ecx+04h]
		push eax

		call MyGetPosition
		add esp, 0ch

		test al,al
		jnz  GoOk
		// ripristinare cursore
	pushad
	mov eax, 436CB7h    ;rem_mouse
	call eax 
	mov eax, 437A71h    ;DefPointer
	call eax 
	mov eax, 436AEEh    ;plot_mouse
	call eax 
	popad
GoOk:
		retn



	}
}

void SetFunctionTom2Pc(void* VetFunzioni[])
{
	int i;
	i=0;

	// indirizzi per tom2pc
	VetFunzioni[i++]= &PatchGenerichePc;
	VetFunzioni[i++]= &ClearMemoryTempTex;
	VetFunzioni[i++]= &PatchRemapTailTriangolo;
	VetFunzioni[i++]= &PatchRemapTailQuad;
	VetFunzioni[i++]= &PatchAttivaCreaRanges;
	VetFunzioni[i++]= &PatchDefaultSizeTex1;
	VetFunzioni[i++]= &PatchDefaultSizeTex2;
	VetFunzioni[i++]= &PatchOffsetFrammenti1;
	VetFunzioni[i++]= &PatchOffsetFrammenti2;
	VetFunzioni[i++]= &FindAnimTex;
	VetFunzioni[i++] = &PatchBugTriangolo;
}
void SetFunctionRoomEditor(void* VetFunzioni[])
{
	int i;

	i=0;
	VetFunzioni[i++] = &MostraMessageBoxNGLE;
	VetFunzioni[i++] = &PatchGenericheRoom;
	VetFunzioni[i++] = &PatchImpostaMenu;
	VetFunzioni[i++] = &Sub_PulsanteBackup;
	VetFunzioni[i++] = &Sub_PulsanteDamage;
	VetFunzioni[i++] = &CopiaTexture;
	VetFunzioni[i++] = &SetIndiceTail;
	VetFunzioni[i++] = &SalvaTomTailInfos;
	VetFunzioni[i++] = &CaricaPrjTailInfos;
	VetFunzioni[i++] = &SalvaRecordTexFaces;
	VetFunzioni[i++] = &ControllaTextureDD;
	VetFunzioni[i++] = &EseguiOgniCiclo;
	VetFunzioni[i++] = &Patch_SalvaMeshPointers;
	VetFunzioni[i++] = &PatchPerStack74;
	VetFunzioni[i++] = &ControllaTextureOggetto1;  
	VetFunzioni[i++] = &ChiudiRoomEdit;
	VetFunzioni[i++] = &PatchHookMessaggi;
	VetFunzioni[i++] = &PatchRoomMeno;
	VetFunzioni[i++] = &PatchRoomPiu;
	VetFunzioni[i++] = &PreparaEccezioniRoom;
	VetFunzioni[i++] = &PatchCicloSleep;
	VetFunzioni[i++] = &PatchLockSurface;
	VetFunzioni[i++] = &PatchRotazioni3d;
	VetFunzioni[i++] = &PatchSalvaMappa2d;
	VetFunzioni[i++] = &PatchBloccaUndo;
	VetFunzioni[i++] = &PatchBugFlipMap;
	VetFunzioni[i++] = &PatchCalcoloBreveIota;
	VetFunzioni[i++] = &PatchDopoSaveProject;
	VetFunzioni[i++] = &PatchMenuTextures;
	VetFunzioni[i++] = &PatchCheckGrid;
	VetFunzioni[i++] = &PatchUltimoComando;
	VetFunzioni[i++] = &PatchUltimoTasto;
	VetFunzioni[i++] = &PatchWriteFile;
	VetFunzioni[i++] = &PatchRemapObjTextures;
	VetFunzioni[i++] = &PatchSalvaRemapArray;
	VetFunzioni[i++] = &PatchPrintF;
	VetFunzioni[i++] = &PatchSalvaStack2;
	VetFunzioni[i++] = &PatchShowLogBoxes;
	VetFunzioni[i++] = &PatchActionTrigger;
	VetFunzioni[i++] = &PatchAggiuntaTrigger;
	VetFunzioni[i++] = NULL;
	VetFunzioni[i++] = &PatchCatturaMenu;
	VetFunzioni[i++] = &PatchGetPosition;
	VetFunzioni[i++] = &PatchCreateDXError;
}




