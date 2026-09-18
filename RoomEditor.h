
// -------------------------- COSTANTI ----------------------------------------------

#define SIZE_TEXTURE 128
#define MAX_TEX_PER_FRAME 16
#define MAX_OLD_ROOMS 10
#define MAX_PROGETTI 20
#define MAX_EXTERN_PROGRAMS 10
#define MAX_NOMI_SLOT 600
#define MAX_TAIL_INFOS 1024
#define MAX_ITEM_FIND_LIST 6000
#define MAX_PLUGIN_NGLE 1000

#define ts_Avanti		1
#define ts_Indietro		2

#define TC_ROSSO 0
#define TC_VERDE 1
#define TC_BLU   2

#define FLAG_ABS_ANIM 0x4000
#define MASK_ASB_ANIM 0xC000

#define NO_ARRAY 0x10000

// flags per tipo operazioni di roomAlteraFloorInput
#define AFI_FLOOR_PIU 0x64
#define AFI_FLOOR_MENO 0x65
#define AFI_CEILING_PIU 0x66
#define AFI_CEILING_MENO 0x67

#define FTO_SET_NONE                          0x00  // usato solo per resettare record
#define FTO_SET_FLOOR                         0x01
#define FTO_SET_CEILING                       0x02
#define FTO_SET_FLOOR_CORNERS                 0x04
#define FTO_SET_CEILING_CORNERS               0x08
#define FTO_SET_FLOOR_GRID                    0x10
#define FTO_SET_CEILING_GRID                  0x20
#define FTO_ADD_FLOOR                         0x40
#define FTO_ADD_CEILING                       0x80
#define FTO_GET_FLOOR_GRID                   0x100
#define FTO_GET_CEILING_GRID                 0x200
#define FTO_GET_FLOOR_CORNERS                0x400
#define FTO_GET_CEILING_CORNERS              0x800
#define FTO_GET_FLOOR                       0x1000
#define FTO_GET_CEILING                     0x2000
#define FTO_SET_TRIANGLE_TEXTURE_FLOOR      0x4000
#define FTO_SET_TRIANGLE_TEXTURE_CEILING    0x8000 
#define FTO_GET_FLOOR_HEIGHT               0x10000
#define FTO_GET_CEILING_HEIGHT		       0x20000
#define FTO_SET_TABELLA_ROTAZIONE_FLOOR    0x40000
#define FTO_SET_TABELLA_ROTAZIONE_CEILING  0x80000
#define FTO_USE_TABELLA_ROTAZIONE_FLOOR   0x100000
#define FTO_USE_TABELLA_ROTAZIONE_CEILING 0x200000
#define FTO_ADD_FLOOR_CORNERS             0x400000
#define FTO_ADD_CEILING_CORNERS			  0x800000
#define FTO_CLEAR_FLOOR_TEXTURES		 0x1000000
#define FTO_CLEAR_CEILING_TEXTURES		 0x2000000
#define FTO_GET_MAX_FLOOR                0x4000000
#define FTO_GET_MAX_CEILING              0x8000000
#define FTO_GET_MIN_FLOOR               0x10000000
#define FTO_GET_MIN_CEILING             0x20000000
#define FTO_SET_TRIANGLESPLIT			0x40000000
#define FTO_SHOW_INFOGRID				0x80000000

#define TZ_FLOOR   1
#define TZ_CEILING 2
#define TZ_WALLS 3

// tipi sezioni
#define SEZ_TRIGGERWHAT 0
#define SEZ_EFFECT 1
#define SEZ_TRIGGERTYPE 2
#define SEZ_CONDITION 3
#define SEZ_ACTION 4
#define SEZ_TEXTS 5
#define SEZ_TOT 6

// tipo parametro random slope
/*
1 = Mountains   (0=Hills)
2 = Limit max height to room height
4 = Many changes for hit
8 = Don't smooth 
16 = Few changes for hit
*/
#define RF_NONE 0
#define RF_MOUNTAINS 1
#define RF_LIMIT_HEIGHT 2
#define RF_MANY_CHANGES 4
#define RF_DONT_SMOOTH 8
#define RF_FEW_CHANGES 16

// flags per preferenze myprefroom.flags
#define FPR_NONE 0
#define FPR_LOAD_LAST_PROJECT 1
#define FPR_NO_RESUME_CRASH 2
#define FPR_NO_EXTENDED_MEMORY 4
#define FPR_REMOVE_AUTOSAVE 8
#define FPR_SAVE_MAP_BMP 16
#define FPR_NO_INFO_HIDE_OBJECTS 32
#define FPR_NO_ACCELERATION 64
#define FPR_NO_MINIMIZE 128
#define FPR_USE_SHORT_PATH 256
#define FPR_NO_STANDBY 512
#define FPR_MAXIMIZE 1024
#define FPR_EXTRA_INFO_SELECTION 0x800
#define FPR_PUNTA_PIRAMIDI      0x1000
#define FPR_DISABLE_NEW_TRIGGERS 0x2000
#define FPR_SAVE_PLAY_PROGRESSIVE 0x4000
#define FPR_HIDE_OBJ_DRAW_DOORS 0x8000

// costanti per tipo di caricamento ng_costanti

#define LNGC_MAIN 1
#define LNGC_TRANSLATION 2 // My_NG_Costants.txt
#define LNGC_PLUGIN 3 // carica file "plugin_name.trg"


// definizioni di trigger what
#define TO_OBJECT 0
#define TO_CAMERA 1
#define TO_SINK 2
#define TO_FLIPMAP 3
#define TO_FLIPON 4
#define TO_FLIPOFF 5
#define TO_TARGET 6
#define TO_FINISH 7
#define TO_CD 8
#define TO_FLIPEFFECT 9
#define TO_SECRET 10
#define TO_ACTION 11
#define TO_FLYBY 12
#define TO_PARAMETER 13
#define TO_FMV 14
#define TO_TIMER_FIELD 15

// tipo indice valore

#define TIV_TIMER_ALL 1
#define TIV_TIMER_BASSO 2
#define TIV_TIMER_ALTO 3
#define TIV_BUTTONS 4
#define TIV_OBJECT 5 // campo value del timer
// tip oscreenshot
#define TSS_FULLSCREEN 0
#define TSS_PANEL2D  1
#define TSS_PANEL3D  2
#define TSS_PREVIEW  3
#define TSS_PREVIEW_RGB 4
#define TSS_TEXTURES 5
#define TSS_INFOROOM 6
#define TSS_LIGHTS 7

// target font
#define TF_BUTTONS 0
#define TF_ROWS 1
#define TF_FRAMES 2

#define EXE_TOMB4  1
#define EXE_EDITOR 2
#define EXE_TOM2PC 3
#define EXE_TOMB5  4
#define EXE_TOMB3  5

#define PANEL_START 472
#define PANEL_END 479

#define SIZE_MAP_TEXTURE 0x500000

// tipo di muro
#define MURO_OVEST 0
#define MURO_NORD 1
#define MURO_EST 2
#define MURO_SUD 3

#define STORE_MASK_INDEX 0x003F // indice
#define STORE_MASK_SIZE  0x00C0  // tipo size byte, short, long 
#define STORE_TYPE_BYTE  0x0040
#define STORE_TYPE_SHORT 0x0080 
#define STORE_TYPE_LONG  0x00C0

#define TFF_NONE 0
#define TFF_VERT_STRIP 0x10
#define TFF_STRIP 0x20
#define TFF_CROSSED_STRIP 0x40
#define TFF_DIAGONAL 0x80


#define TEX_SEZ_STRINGS  0
#define TEX_SEZ_PSXSTRINGS 1
#define TEX_SEZ_PCSTRINGS 2
#define TEX_SEZ_EXTRANG 3

// costanti per coandi header di buttonswindowtxt
#define TH_COLUMNS  0
#define TH_TITLE 1
#define TH_BUTTON_WIDTH 2
#define TH_BUTTON_HEIGHT 3
#define TH_BACK_COLOR 4
#define TH_FORE_COLOR 5
#define TH_FONT 6


// costanti per nuovi triggers
#define SHOW_IN_TIMER 1
#define SHOW_IN_OBJECT 2
#define SHOW_IN_EXTRA 3
#define SHOW_IN_BUTTONS 4

#define TGROUP_USE_FOUND_ITEM_INDEX		 0x0001
#define TGROUP_USE_TRIGGER_ITEM_INDEX	 0x0002
#define TGROUP_USE_OWNER_ANIM_ITEM_INDEX 0x0004
#define TGROUP_SINGLE_SHOT_RESUMED       0x0008
#define TGROUP_OR						 0x0010
#define TGROUP_NOT						 0x0020
#define TGROUP_ELSE						 0x0040
#define TGROUP_CONDITION				 0x0080
#define TGROUP_USE_EXECUTOR_ITEM_INDEX   0x0100
#define TGROUP_DISABLED					 0x0200 // non eseguirlo piu'
#define TGROUP_SINGLE_SHOT				 0x0400 // dopo averlo eseguito una volta disattivarlo
#define TGROUP_USE_ITEM_USED_BY_LARA_INDEX   0x0800 // indice di oggetto con cui lara sta interagendo
// questi sono impostati nella fase di exporting
#define TGROUP_CONDITION_TRIGGER		0x8000
#define TGROUP_ACTION					0x4000
#define TGROUP_FLIPEFFECT				0x2000
#define TGROUP_MOVEABLE					0x1000

#define to_Load 1
#define to_Save 2

#define FTL_NONE 0
#define FTL_HIDE_NUMBERS 1
#define FTL_MOVE_NUMBERS 2

// Flag ANimated texture
#define FAN_MASK_FPS_UV    0x1F00
#define FAN_MASK_UVROTATE  0x00FF
#define FAN_MASK_DELAY     0x1FFF
#define FAN_MASK_ANIM      0xE000
#define FAN_FRAMES         0x0000
#define FAN_P_FRAMES       0x4000
#define FAN_FULL_ROTATE    0x8000
#define FAN_RIVER_ROTATE   0xA000 // 0x8000 + 0x2000
#define FAN_HALF_ROTATE    0xC000
#define FAN_ANY_ROTATE     0x8000

#define TLB_LoadLastProjects  1
#define TLB_ListaStanze 2
#define TLB_ListaOggetti 3
#define TLB_ListaSounds 4
#define TLB_RemapObjects 5
#define TLB_RemapRooms 6


#define TLF_OBJECTS				1
#define TLF_INDICES				2
#define TLF_FLOOR_TYPE			3
#define TLF_TRIGGER_ACTIVATION	4
#define TLF_TRIGGER_WHAT		5
#define TLF_TRIGGER_OF_PLUGIN	6
#define TLF_TRIGGER_UNHANDLED	7
#define TLF_EFFECTS				8
#define TLF_TOT_ITEM			9

// Ripo Ricerca Trigger per funzione CercaTriggers()
#define TRT_LIST_UNHANDLED_TRIGGERS 1
#define TRT_CHANGE_TRIGGER_ID		2
#define TRT_REMOVE_TRIGGER_WITH_ID  3
#define TRT_REFRESH_USED_TRIGGERS	4

// ---- flag per level_tr4 
#define		FLT_NONE 0
#define		FLT_NEW_TRIGGERS 0x0001
#define     FLT_EXTRA_SOUND_TABLE 0x0002

#define MAX_ROOMS 512

#define RGB_Nero 0x0
#define RGB_Rosso 0x000000FF
#define RGB_Verde 0x0000ff00

#define TP_LUCI 1
#define TP_CENTRALE 2

#define TCL_LIGHT_X 1
#define TCL_LIGHT_Y 2
#define TCL_LIGHT_LEN 3
#define TCL_LIGHT_CUT 4
#define TCL_SPOT_INT 5
#define TCL_SPOT_OUT 6
#define TCL_SPOT_IN 7

#define FRAME_SECONDO 30

// costanti per accedere a variabili (BaseVariabili)
#define VAR_MASK_TYPE  0x0070  // -111 ----
#define VAR_MASK_SIZE  0x0030  // --11 ----
#define VAR_MASK_INDEX 0x000F  // ---- 1111
#define VAR_TYPE_TEXT  0x0020  // --10 ----
#define VAR_TYPE_LOCAL 0x0040  // 64
#define VAR_TYPE_BYTE  0x0000  //  0
#define VAR_TYPE_SHORT 0x0010  // 16
#define VAR_TYPE_LONG  0x0030  // 48

#define VAR_TYPE_STORE          0x0100  // e' un tipo di variabile store

#define VAR_TYPE_INPUT_NUMBER   0x0200  // variabile numerica LastInputNumber
#define VAR_TYPE_INPUT_TEXT     0x0400 // ultimo testo inserito da utente
#define VAR_TYPE_CURRENT_VALUE  0x0800 // variabile speciale current value
#define VAR_TYPE_BIG_TEXT       0x1000 // variable speciale big text

#define VAR_SPECIAL_INPUT_NUMBER 0x0035 // numero speciale per pStartFog poi da cambiare
#define VAR_SPECIAL_CURRENT_VALUE 0x00FF // solo per LST_ poi dovra' essere convertito
#define VAR_SPECIAL_INPUT_TEXT 0x0036 // numero speciale per LST_
#define VAR_SPECIAL_BIG_TEXT   0x0037 // numero per LST_

// costanti per floordata di file tr4
#define    floor_EmptyCommonFloor  0
#define    floor_Door  1
#define    floor_Tilt  2
#define    floor_Roof  3
#define    floor_Trigger  4
#define    floor_Lava  5
#define    floor_Climb  6
#define    floor_Split1  7
#define    floor_Split2  8
#define    floor_Split3  9
#define    floor_Split4  10
#define    floor_NOCOLF1T  11
#define    floor_NOCOLF1B  12
#define    floor_NOCOLF2T  13
#define    floor_NOCOLF2B  14
#define    floor_NOCOLC1T  15
#define    floor_NOCOLC1B  16
#define    floor_NOCOLC2T  17
#define    floor_NOCOLC2B  18
#define    floor_MONKEY_TYPE  19
#define    floor_TRIGTRIGGER_TYPE  20
#define    floor_MINER_TYPE  21

// flag per cabinet (come agire se livelli incompatibili)
#define CAB_NONE  0
#define CAB_ON_ERROR_MSGBOX  0x01 //  se incompatibile mostra msgbox  e chiede cosa fare
#define CAB_ON_ERROR_IGNORE  0x02  //cerca comunque di usare cabinet anche se pare incompatibile
#define CAB_ON_ERROR_SKIP  0x04  // da avviso e non usa cabinet
#define CAB_CREATE_LOG_FILE 0x08 // durante covnersione crea un log con tutti i dati di fusione

// flag per elementi meta da importare in livello attuale
#define META_NONE  0
#define META_DOORS                      0x0001
#define META_ROOM_COLLISIONS            0x0002
#define META_STATIC_ITEMS               0x0004
#define META_LIGHTS                     0x0008 
#define META_CAMERAS                    0x0010
#define META_MESH_OBIECTS               0x0020  
#define META_STATIC_SLOTS               0x0040
#define META_MOVEABLE_SLOTS             0x0080
#define META_FLYBY_CAMERA               0x0100
#define META_ANIM_TEXTURE               0x0200
#define META_ANIM_SLOT                  0x0400
#define META_BOX_ZONES_OVERLAPS         0x0800
#define META_SPRITE_TAILS_AND_SEQUENCE  0x1000
#define META_ROOM_POSITION              0x2000


// ---------------- DUPLICATI DI TOMB4 -------------------------------
// valore assoluto di prima word (no maschera bit)
#define TGROUP_COMMAND					0x0003


#define TCMD_GOTO  1   // go to Index trigger of current TriggerGroup
#define TCMD_EXIT  2   // quit from current triggergroup, returning the value true/false (1/0) in conditions

#define TCMD_MAX_ID             11 // limit of TCMD values (lo stesso di tomb_nextgeneration.h


// ------------------- STRUTTURE -------------------------------------------------
#pragma pack(1)

typedef struct StrPatch {
	DWORD Offset;
	bool TestInizio;
}PatchFields;

typedef struct StrTotElementi {
	DWORD Offset;
	char Tipo;
}TotElementiFields;

typedef struct StrPatchImmediata {
	DWORD Offset;
	char Tipo;
	int Differenza;
}PatchImmediataFields;


typedef struct StrSalvaVettoriRemap {
	short VetObjRemap[6000];
	int  TotRooms;
	short VetRoomRemap[400];
	bool  TestAttivo;
	char NomeFileTom[256];
}SalvaVettoriRemapFields;


typedef struct StrTriggerTrovati {
	WORD Id;
	char Descrizione[80];
	int Room;
	int XOrg;
	int ZOrg;
}FieldsTriggerTrovati;

typedef struct StrDatiPulsante {
	StrDatiPulsante *pNextRecord;		// 00
	WORD Flags;				// 04
	WORD OrgX;				// 06
	WORD OrgY;				// 08
	WORD SizeX;				// 0A
	WORD SizeY;				// 0C
	DWORD Reserved;			// 0E
	void * pSubGestione;		// 12
	BYTE ColoreTesto;		// 16
	BYTE ColoreFondo;		// 17
}DatiPulsanteFields;


typedef struct StrTailInfo {
	BYTE *pMemImage;
	WORD SizeX;
	WORD SizeY;
}TailInfoFields;



typedef struct StrFindReplaceTex {
	int TotTrovate;
	int IndiceReplace;
	int IndiceFind;
	bool TestAttivo;
}FindReplaceTexFields;



typedef struct StrTailInfoOut {
	BYTE OrgX;		// 00	
	BYTE OrgY;		// 01 
	WORD IndiceTail; // 02
	WORD SizeX;      // 04
	WORD SizeY;      // 06
}TailInfoOutFields;


typedef struct StrRecordItemRoom {
		WORD  ItemIndex;
		WORD  NumeroStanza;	
}RecordItemRoomFields;

typedef struct StrSalvaTailInfo {
	int IndexTail;
	bool TestAllocata; // memoria nuova e' stata allocata
	BYTE *pMemAllocata;
	StrTailInfo  SalvaTailInfo;
	StrTailInfo  SalvaBigTailInfo;
}SalvaTailInfoFields;

typedef struct StrRoomAnimaUV {
	BYTE *VetMemOrigine[MAX_TEX_PER_FRAME];
	int  ScrollPos;  // byte da aggiungere a pMemOrigine per avere
					// attuale punto di inquadratura texture
	int  UVRotate; // valore positiivo o negativo da aggiungere a scroll pos
	int  ByteRiga;  //numero di byte per ogni riga
	int  MascheraScroll; // 0x1f o 0x3f
	DWORD LastTime;
	DWORD Delay;

}RoomAnimaUVFields;


typedef struct StrDatiTexture {
	int IndiceOld;   //indice a VetBigTails
	int GrigliaX;
	int GrigliaY;
	int Sorg_OrgTextureX;	// coordinata sorgente di texture 256, 128 o 64
	int Sorg_OrgTextureY;
	int Sorg_PixelTailX;  // coordinata in pixel di singola porzione
	int Sorg_PixelTailY;
    int Sorg_IndiceTexture; // indice di texture 64 x 64
	int SizeTextureX; // 64 , 128 o 256
	int SizeTextureY; //  "    "     "
	bool TestFullScroll; // true= raddoppia texture in verticale
	WORD NewSizeY; // da usare solo se TestFullScroll = true
}DatiTextureFields;

typedef struct StrColoriPalette {
	BYTE Blue;
	BYTE Green;
	BYTE Red;
}ColoriPaletteFields;


typedef struct StrItemsRoom {
	WORD TypeObject;		// 0x00
	WORD XOrgBlock;			// 0x02
	WORD ZOrgBlock;			// 0x04
	WORD XSizeBlock;		// 0x06
	WORD ZSizeBlock;		// 0x08
	WORD YClickAboveFloor;  // 0x0A
	WORD IndexRoom;			// 0x0C
	WORD SlotIndex;			// 0x0E  
	WORD OcbButtons;		// 0x10
	WORD Orientation;		// 0x12
	DWORD XPixelOrigin;		// 0x14
	DWORD YPixelOrigin;		// 0x18
	DWORD ZPixelOrigin;		// 0x1C
	WORD Mistero;			// 0x20
	WORD ObjFacing;			// 0x22
	WORD Roll;				// 0x24
	WORD ObjTintOrSpeed;	// 0x26
	WORD OCBCode;			// 0x28
}ItemsRoomFields;

typedef struct StrFlyByRoom  {
	BYTE IndiceFlyBy;
	BYTE NumCamera;
	WORD IndiceNgle;
}FlyByRoomFields;





typedef struct StrFlyByArrayRoomEditor {
	BYTE Sequenza;
	BYTE Number;
	WORD ObjIndex;
}FlyByArrayRoomEditorFields;




// usata in room editor
typedef struct StrTextureInfo {
	WORD	Status;				// 00
	WORD	IndexAndFlags;		// 02
	BYTE	Rotation;			// 04
	BYTE	Triangle;			// 05
	WORD	TriangleCouple;		// 06
}TextureInfoFields;					// 08

typedef struct StrGridXRoom {
	WORD	Type;								// 00
	DWORD	Flag1;   // in project invece       // 02
	short   WorldFloorClick;                    // 06
	short	WorldYCeilingClick;					// 08
	char	FloorCornerClicks[4];               // 0A
	char	CeilingCornerClicks[4];				// 0E
	char	WallFloorCornerClicks[4];			// 12
	char	WallCeilingCornerClicks[4];			// 16
	WORD	Mistero;							// 1A
	StrTextureInfo TextureInfos[14];			// 1C
	WORD	TestTriangleSplit;					// 8C
}GridXRoomFields;										// 8E


typedef struct StrStanzaRoomEdit {
	WORD  	TestEmptySlot;       // 00
	char	Nome[80];			 // 02
	int  	ZOriginePixel;		 // 52    
	int		YOriginePixelTop;	//  56
	int		XPixelWorld;		//  5A
	int		YOriginePixelBottom; // 5E
	WORD	Mistery1;			 // 62
	WORD    Mistery2;			// 64
	WORD	Mistery3;			// 66
	void *  pMemVertici;		// 68
	WORD  	XDisplayOffset;		// 6C		
	WORD	ZDisaplyOffset;		// 6E
	WORD	XSizeBlocks;		// 70   (ok)
	WORD	ZSizeBlocks;		// 72   (ok)
	StrGridXRoom *  pBlockGridCollision;	// 74
	WORD	XBlocksWorld;		// 78
	WORD	ZBlocksWorld;		// 7A
	WORD	N_Doors;			// 7C  (ok)
	WORD 	VetDoorCode[20];	// 7E
	WORD	N_Effects;			// A6
	WORD	VetEffectCode[32];  // A8
	WORD	N_ObjAndTriggers;   // E8
	WORD	VetObjAndTriggerCodes[100]; // EA
	WORD	LinkedRoom;			// 1B2
	short    OrgYClickBottom;	// 1B4
	short   OrgYClickTop;		// 1B6
	DWORD	AmbientLightColor;	// 1B8
	WORD    Mistery_1bc;		// 1BC
	WORD	Mistery_1be;		// 1BE
	WORD    Mistery_1c0;		// 1CO
	WORD	Mistery_1c2;		// 1C2
	WORD	Mistery_1c4;        // 1C4
	short	IndexFlipRoom;		// 1C6
	WORD	RoomButtons;		// 1C8
	WORD    Mistery_1ca;		// 1CA
	BYTE	WaterValue;			// 1CC
	BYTE	MistValue;			// 1CD
	BYTE	ReflectionValue;	// 1CE
	BYTE	FlipMapValue;		// 1CF
	BYTE    StatusPulsante_L;	// 1D0 (giusto di sicuro)
}StanzaRoomEditFields;

typedef struct StrInvItem {
	WORD  SlotId;      // 0x00
	WORD  Mistero1;    // 0x02
	WORD  Mistero2;    // 0x04
	WORD  Mistero3;    // 0x06
	WORD  Mistero4;    // 0x08
	WORD  Mistero5;    // 0x0A
	WORD  Flags;	   // 0x0C
	WORD  IndexString; // 0x0E
	WORD  Mistero6;    // 0x10
	WORD  Mistero7;    // 0x12
}InvItemFields;



typedef struct StrTriggerRoom {
	BYTE TriggerAttivazione;     // 00  (era word)
	BYTE PluginId;				//  01  (aggiunto da me per ospitare idplugin)
	short OggettoDaTriggerare;   // 02
	short Timer;                 // 04
	WORD TriggerButtons;         // 06
	WORD TriggerCosaAttivare;    // 08
}TriggerRoomFields;


typedef struct StrOldProject {
	char  NomeProject[256];
	int   IndiceRead;
	int	  IndiceWrite;
	short VetLista[MAX_OLD_ROOMS];
}OldProjectFields;
// flag per record programma esterno da lanciare
#define FEP_NONE 0
#define FEP_CLIPBOARD 1

typedef struct StrRecordExternProgram {
	char PathExe[256];
	char Titolo[64];
	short TipoOpen;
	WORD Flags;
}RecordExternProgramFields;

typedef struct StrPrefRoom {
	DWORD Flags;
	int	TotOldProjects;
	int IndiceProjectNow;
	struct StrOldProject VetOldProjects[MAX_PROGETTI];
	DWORD ParameterRandomFloor;
	int SpinningTimeInput;
	int SpinningStep;
	int MouseMinStep;
	int MouseMaxStep;
	int MouseSensitivity;
	int TotExternPrograms;
	StrRecordExternProgram VetExternPrograms[MAX_EXTERN_PROGRAMS];
	int PreviewSize;  // indice a tipo dimensione
	int BendType;  // tipo di bend usato: fisso, sfera, parabola
	int AutoBackup; // numero di cambi prima di backup automatico
	int DefaultUvRotate; // valore di uvrotate da usare in preview al posto
					// di valore script.dat
	int ButWndOrgX; // distanza da orgx,orgy di finestra 
	int ButWndOrgY; // principale, calcolata su area client
}PrefRoomFields;
	
typedef struct StrPreviewType {
	RECT Zona;
	char Descrizione[60];
}PreviewTypeFields;


typedef struct StrListElenco {
	char *pNome;
	int  Indice;
}ListElencoFields;

typedef struct StrRiverRange {
	int TotTexture;
	WORD SettingFps;
	int IndiceRangeNG;
	WORD VetIndiciTail[50];
}RiverRangeFields;

typedef struct StrObjSlotRoom {
	char *pNome;		    // 00
	WORD Tipo;			    // 04
	WORD	MeshIndex;      // 06 questo e' lo slot per gli statici
	WORD    TotMesh;             //08
	DWORD    AnimStartIndex;     // 0A
	DWORD   DistanceView;        // 0E
	WORD    RotationX;           // 12
	WORD    RotationY;
	WORD    ROtationZ;
	void *   pMemoriaMesh;
	WORD    West;
	WORD    North;
	WORD    East;
	WORD    South;
	WORD	HeightTable[5][5];
	WORD	ModeTable[5][5];
}ObjSlotRoomFields;

typedef struct StrFindRoom {
	int  IndiceRoom;
	int  IndiceAlfa;
	int  IndiceBeta;
	int  TipoRicerca;  // TLF
	int  Incremento;  // o +1 o -1
}FindRoomFields;

// record  per salvare nomi custom degli slot
typedef struct StrNomeSlot {
	WORD Slot; // numero slot
	char NomeSlot[32];
}NomeSlotFields;

typedef struct BaseNomiSlot {
	short TotNomiSlot;
	char NomeWad[64]; // solo nome senza dir
	StrNomeSlot VetNomi[MAX_NOMI_SLOT];
}BaseNomiSlotFields;

typedef struct StrHeaderTga {
	BYTE	Info;				// 0x00
	BYTE	ColorType;			// 0x01
	BYTE	ImageType;			// 0x02
	WORD	StartDacTable;		// 0x03
	WORD	ColorDacTable;		// 0x05
	BYTE	EnterDacTable;		// 0x07
	short	OrgX;				// 0x08
	short	OrgY;				// 0x0A
	WORD	SizeX;				// 0x0C
	WORD	SizeY;				// 0x0E
	BYTE	BitPerPixel;		// 0x10
	BYTE	Descriptor;			// 0x11
}HeaderTgaFields;


typedef struct StrRobaTexture {
	int		BaseX;		// punto di inizio di zona texture
	int		BaseY;
	int		ScreenX;	// dimensione di zona texture nella fienstra
	int		ScreenY;
	int		SizeSrc;  // dimensione originale di ogni texture in mappa texture
	int		SizeDst;  // dimensione finale in finestra di ogni texture
	HDC		CompHdc;  // hdc sorgente con mappa tga
	HDC		MioHdc;   // hdc destinazione di finestra
	int		GrigliaSrcX;   // griglia di mappa tga
	int		GrigliaSrcY;
	int		GrigliaDstX;   // griglia in finestra
	int		GrigliaDstY;
	int		IndicePrimaTexture;  //indice della prima texture visibile
	int		TotTexture;    // numero totale di texture
	int		SelInizio;   // indice prima texturee selezionata ( o -1)
	int		SelFine;  // indice ultima texture selezionata (o -1)
	int		TipoTexture;
	int		Gap;		// distanza tra due texture
	bool	Ready;
	int		ScrollMax;  // valore inseriot in scroll bar come massimo
	int		IndiceSelSerie;  // indice di serie animata selezionata (in rosso)
	int		Speed;  // frame per secondo di animazione texture (solo per [Test])
	int		TipoAnim; // tipo animazione da usare (solo per [Test])
	int     UVRotate; // valore uvrotate (solo per [Test])
	int     TipoAnimOld; // precedente valore anim (usato per settare combo speed)
	int		TotLastSort; // numero di indici range anim salvati in tom
	int     VetLastSorg[40]; // indici di range nell'ordine in cui sono stati salvati
}RobaTextureFields;


typedef struct StrAnimRange {
	int		TestUsato;
	int		IndiceStart;
	int		IndiceEnd;
}AnimRangeFields;

typedef struct StrItemEffectRoom {
	WORD   Intensity;
	float  Light_In;
	float  Light_out;
	float  Degree_X;
	float  Degree_Y;
	float  Spot_Len;
	float  Spot_Cut;
	BYTE   Red_Color;
	BYTE   Green_Color;
	BYTE   Blue_Color;
	BYTE   State;
}ItemEffectRoomFields;

typedef struct StrStanza2d {
	int		Colore;
	int		Valore;   // valore di riferimento
	RECT	Posizione;
}Stanza2dFields;

// valida per tomb4 o tomb5
typedef struct StrAnimDispatch {
	WORD Low;
	WORD High;
	WORD Animation;
	WORD Frame;
}AnimDispatchFields;


typedef struct StrInfoMouse {
	int NaturaPulsante;        // 0x00
	int Mistero1;              // 0x04
	int Mistero2;              // 0x08
	int Mistero3;              // 0x0C
	WORD Mistero4;             // 0x10
	void * OffsetSubComando;   // 0x12
	BYTE CodiceTasto;          // 0x16
	BYTE Mistero6;             // 0x17
	WORD StatusTasti;          // 0x18
	StrDatiPulsante *pRecordPulsante;  // 0x1A
	WORD Mistero7;             // 0x1E
}InfoMouseFields;

typedef void (__cdecl* TYPE_GenericaSubPulsante) (StrInfoMouse *pInfoMouse);
// record per pulsanti di buttonsWindow
typedef struct StrButtonRec {
	char OriginalName[80];
	char NewName[80];
	DWORD BackColor;
	DWORD ForeColor;
	HBRUSH hBrush; // usato solo per "R" e "F"
	RECT Box; // usato per row e infobox
 	char TipoRecord;  // 'B', 'M', 'P', 'R', 'F', 'C' 'K'
	BYTE OldBackColor; // indice colore in datipulsante
	bool TestFlipFlop;
	union {
		StrDatiPulsante *pDatiPulsante;
		void * pCallMenu;
		WORD CodiciTasti[2]; // [0] = codice [1] = status 
	};

}ButtonRecFields;



typedef struct StrMiaPresetButtons {
	char *pNome;
	union {
		DWORD Offset;
		StrDatiPulsante * pOffset;

	};
}MiaPresetButtonsFields;


typedef struct StrMiaInfoPulsante {
	char *pNome;
	StrDatiPulsante *pDatiPulsante;
	BYTE OldBackColor;
}MiaInfoPulsanteFields;

typedef struct StrMiaInfoMenu {
	char *pNome;
	void *pCallMenu;
}MiaInfoMenuFields;

typedef struct StrTriggerSelezionato {
	BYTE TriggerActivation;   // 0x00  ( era WORD, modificato per ospitare campo pluginId)
	BYTE PluginId;			  // 0x01   (creato, prima non c'era)
	short Valore;			  // 0x02
	short Timer;              // 0x04
	WORD MaskPulsanti;        // 0x06
	WORD TriggerWhat;         // 0x08
}TriggerSelezionatoFields;

typedef struct StrBaseTempTrigger {
	BYTE TriggerActivation;
	BYTE IdPlugin;
	short Valore;
	short Timer;
	int TipoValoreTimer; // TIV_..
	WORD MaskPulsanti;
	WORD TriggerWhat;
	bool TestIndiceTrigger;
	int IndiceTrigger;
	bool TestExtraTimer;
	bool TestExtraButtons;
	bool TestMoveable; 
	bool TestEffect; // camera fixed camera
	char *pMexHelp;
	char *pMexRemark;

}BaseTempTriggerFields;

typedef struct StrTestoFinestra {
	int Mistero1;
	WORD OffX;
	WORD OffY;
	char *pTesto;
	int  Mistero2;
}TestoFinestraFields;

typedef struct StrDatiFinestra {
	int  Reserved1;
	int  Reserved2;
	int  OrgX;
	int  OrgY;
	int  SizeX;
	int  SizeY;
	StrTestoFinestra *pTestoFinestra;
	BYTE ColoreTesto;
	BYTE ColoreFondo;
	int  Mistero1;
	int  Mistero2;
}DatiFinestraFields;



typedef struct StrMioFont {
	char FontName[80];
	int FontSize;
	WORD FontBold;
	HFONT hFont;
}MioFontFields;

typedef struct StrHeaderButWindow {
	char Titolo[256];
	int Colonne;
	int ButtonSizeX;
	int ButtonSizeY;
	DWORD BackColor;
	DWORD ForeColor;
	StrMioFont FontButtons;
	StrMioFont FontRows;
	StrMioFont FontFrames;


}HeaderButWindowFields;


		

typedef struct StrBaseButtonsWindow {

	HWND hWind; // handle indow della finestra o 0 se eliminata
	StrHeaderButWindow Header;
	int TotButtons;
	StrButtonRec *pVetButtons;
	int TotStandardButtons;
	HBRUSH VetBrushPalette[16];
	bool MioComandoInCorso;
	HPEN PennaNera;
	HPEN PennaRossa;
	bool TestSospesa;  // se resa invisibile temporaneamentre mentre si piazza oggetto
	int WindSizeX;
	int WindSizeY;
	int PaletteLastSelezionato;
	bool PaletteLastTest;
	// dati per colori palette
	WORD *pIndiceTexture; // o indice colore
	bool *pTestUsaTexture; // false =usa palette
	char LastInfoStampato[800];
	StrMiaInfoPulsante VetStandardButtons[500];
	int TotStandardMenu;
	StrMiaInfoMenu VetStandardMenu[500];

	bool TestStopMemo;
}BaseButtonsWindowFields;




typedef struct StrDatiPerTga {
	BYTE  Mistero1[3];      // 0x00
	WORD  SizeX;	  		// 0x03
	WORD  SizeY;    		// 0x05
	WORD  Mistero2;			// 0x07
	DWORD Mistero3;			// 0x09
	BYTE *pMemoriaFileTexture;	// 0x0D
}DatiPerTgaFields;

typedef struct StrBaseSchermo {
	int  Mistero1;     // 0x00
	WORD Mistero2; 	   // 0x04
	WORD ScreenSizeX;  // 0x06
	WORD ScreenSizeY;  // 0x08
	int  Mistero3;	    // 0x0A
	int  Mistero4;	    // 0x0E
	int  Mistero5;      // 0x12
	BYTE *pMemoriaVideo;   // 0x16
	int  Mistero6;	    // 0x1A
	RGBTRIPLE VetPaletteTri[1];  // 0x1E
}BaseSchermoFields;

typedef struct StrRobaShape {
	int BaseFloor;
	int BaseCeiling;
	int SalvaFrecciaSelezione;
	int DirezioneFreccia;
	int RoomSizeX;
	int RoomSizeY;
	RECT SalvaRectSelezione;
	RECT MiaSelezione;
	bool TestVerticale;  // se true movimenti alto/basso altrimenti destra/sinistra
	int StrisciaIniziale;  // orizzontale o verticale
	int StrisciaFinale;  // questi due valori sono il tratto uguale come trattamento
	int StrisciaSize;
	int StrisciaIncremento; // per andare da striscia iniziale a striscia finae
	int StrisciaTot;  // numero di striscie
	int IncrementoAltezza;  // +1  alza  -1 abpStartFoga
	bool TestStepped;  // =true alza o abbassa senza inclinare floor
	float ClickIncremento;  //porzione da aggiungere ad ogni striscia di click
	int ClickInizio; // usato per salite / discese, punto attuale piu' basso
	int ClickFine;  // punto attuale piu' alto
	float ClickNow;
	StrStanzaRoomEdit *pRoom; // puntatore a stanza attuale
	RECT MiniSel;	// selezione usata da ModificaClickFloor() per le sue operazioni
	int ShiftCeiling;  // 1= floor 2=ceiling usato moltiplicandolo per TOP_ 
	bool TestCeiling;  
	HCURSOR SalvaCursore;
	int RuotaTex; // valore per rotazione di texturee
}RobaShapeFields;

typedef struct StrVet20 {
	short VetClick[20];
}Vet20Fields;

typedef struct StrBaseRotazioni {
	RECT  SelezioneFloor;
	RECT  SelezioneCeiling;
	int   IndiceRoom;
	BYTE TabellaFloor[20][20];
	BYTE TabellaCeiling[20][20];
}BaseRotazioniFields;



typedef struct StrPunto3d {
	double x;
	double y;
	double z;
}Punto3dFields;
typedef struct StrDatiParabola {
	double a;
	double b;
	double c;
}DatiParabolaFields;


typedef struct StrDatiRotazioni {

	int IncrementoRotazioni;
}DatiRotazioniFields;

typedef struct StrDatiExtraAnimazioni {
	bool TestPresente;
	BYTE Tot_UV_Rotate; // numero di animation range con uv rotate
	WORD TotaleRangeNG; //numero originale di animation ranges
	WORD VetInfoRangeAnim[40]; // ogni valore contiene info su velocita frame
								// di ogni record animazione range
	WORD VetFromTex[40];
	WORD VetToTex[40];
	// usati solo per esport in headerng di file tom
	WORD SizeDefault; // 64 o 128

}DatiExtraAnimazioniFields;


typedef struct StrRecordRigaConst {
	int Numero;
	char *pDescrizione;
	char *pMexRemark;  // puntatore a sola parte di eventuale #remark# (senza codice #remark#)
	char *pMexHelp;    //   puntatore a sola di parte di eventuale testo #START_DOC# (senza codici inziialei e finali)
}RecordRigaConstFields;

typedef struct StrRecordSezione {
	int TipoSezione;  // valori SEZ_...
	int NumeroSezione;  // codice trigger relativo a sezione
	int ListPredefinito;  // 0 = no, altrimenti tipo di costante LST_
	int TipoListaShow;  // dove copiare lista SHOW_IN.. valori
	int TotValori;
	WORD Flags;  // flag FTL_
	StrRecordRigaConst *pVetValori;
}RecordSezioneFields;

typedef struct StrNGConstants {
	int TotSezioni;
	StrRecordSezione *pVetSezioni;
	int IndiceSezTesti; // indice a pvetsezioni per testi
}NGConstantsFields;



// struttura per contesto altrimenti non ci capisco niente
typedef struct TIPO_CONTEXT {

    //
    // The flags values within this flag control the contents of
    // a CONTEXT record.
    //
    // If the context record is used as an input parameter, then
    // for each portion of the context record controlled by a flag
    // whose value is set, it is assumed that that portion of the
    // context record contains valid context. If the context record
    // is being used to modify a threads context, then only that
    // portion of the threads context will be modified.
    //
    // If the context record is used as an IN OUT parameter to capture
    // the context of a thread, then only those portions of the thread's
    // context corresponding to set flags will be returned.
    //
    // The context record is never used as an OUT only parameter.
    //

    DWORD ContextFlags;

    //
    // This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
    // set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
    // included in CONTEXT_FULL.
    //

    DWORD   Dr0;
    DWORD   Dr1;
    DWORD   Dr2;
    DWORD   Dr3;
    DWORD   Dr6;
    DWORD   Dr7;

    //
    // This section is specified/returned if the
    // ContextFlags word contians the flag CONTEXT_FLOATING_POINT.
    //

    FLOATING_SAVE_AREA FloatSave;

    //
    // This section is specified/returned if the
    // ContextFlags word contians the flag CONTEXT_SEGMENTS.
    //

    DWORD   SegGs;
    DWORD   SegFs;
    DWORD   SegEs;
    DWORD   SegDs;

    //
    // This section is specified/returned if the
    // ContextFlags word contians the flag CONTEXT_INTEGER.
    //
	
    DWORD   Edi;
    DWORD   Esi;
    DWORD   Ebx;
    DWORD   Edx;
    DWORD   Ecx;
    DWORD   Eax;

    //
    // This section is specified/returned if the
    // ContextFlags word contians the flag CONTEXT_CONTROL.
    //

    DWORD   Ebp;
    DWORD   Eip;
    DWORD   SegCs;              // MUST BE SANITIZED
    DWORD   EFlags;             // MUST BE SANITIZED
    DWORD   Esp;
    DWORD   SegSs;

    //
    // This section is specified/returned if the ContextFlags word
    // contains the flag CONTEXT_EXTENDED_REGISTERS.
    // The format and contexts are processor specific
    //

    BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];

} ROBA_CONTEXT;

typedef struct StrAdrMemory {
	union {
		BYTE *pByte;
		short *pShort;
		int *pLong;
	};
	char TipoSize; // B  S L
	char *pDescrizione;
}AdrMemoryFields;

typedef struct StrSoundSfx {
	short VetSfx[2048];
	int TotSfx;
}SoundSfxFields;

typedef struct StrScriptTrigger {
	WORD PluginId;
	WORD Flags;
	WORD Object;
	union {
		WORD Timer;
		short TimerSigned;
	};
}ScriptTriggerFields;

typedef struct StrTexParziali {
	WORD  IndiceRange; // indice range
	WORD  IndiceFull; // indice della texture full di cui fa parte questo frammento
	WORD IndiceFrammento;
	WORD  OffX;
	WORD  OffY;
}TexParzialiFields;


typedef struct StrBaseParziali {
	WORD TotRecords;
	StrTexParziali VetParziali[4096];
}BaseParzialiFields;

typedef struct StrExtraInfoRoom {
	BYTE  WaterIntensity; // usato per pioggia e neve
	BYTE  Reserved[7]; // altri 8 byte da usare in futuro
}ExtraInfoRoomFields;

typedef struct StrCercaStatic {
	WORD IndiceRoom;
	short IndiceStatic; // interno a stanza
}CercaStaticFields;



typedef struct StrRemapTailInfo {
	WORD IndiceTom;
	WORD IndiceTr4;
}RemapTailInfoFields;

typedef struct StrBaseRemapTailInfo {
	WORD TotTails;
	StrRemapTailInfo VetRemapTail[MAX_TAIL_INFOS*2];
}BaseRemapTailInfoFields;

typedef struct StrVersionHeader {
	WORD VetVersione[4];
	WORD Flags;	// VER_... flags

}VersionHeaderFields;

typedef struct StrParseNGField {
	DWORD NextIndex;
	WORD *pData;
	DWORD SizeData;
	DWORD StartDataIndex;
	WORD  Type;
}ParseNGFieldFields;

typedef struct StrExtractNG {
	int Result;
	DWORD SizeHeader;
	DWORD StartOffset;
	DWORD NWords;
	WORD *pNGArray;
}ExtractNGFields;

typedef struct StrTailInfoTom {
	BYTE OrgX;		// 00
	WORD TopPixel;	// 01
	BYTE Flags;     // 03
	BYTE Mistery1;  // 04
	BYTE SizeX;     // 05
	BYTE Mistery2;  // 06
	BYTE SizeY;      // 07
}TailInfoTomFields;

typedef struct StrTexInfoPc {
	WORD OrgX;
	WORD OrgY;
	WORD SizeX;
	WORD SizeY;
}TexInfoPcFields;
// usatra solo per mantgenere frammenti di tex animate in tom2pc
typedef struct StrFrammentiTex {
	WORD Attribute;  // bit alti di indicetail ricevuta da funzione
	WORD Flag; // (1= triangolo 2= quad)
	short OrgX;
	short OrgY;
	WORD SizeX;
	WORD SizeY;
}FrammentiTexFields;

typedef struct StrListaFiles {
	char Testo[80];
}ListaFilesFields;



typedef struct StrPluginNames {
	int PluginId;
	char Nome[80];
	DWORD TotUsato; // non molto preciso ma almeno evita warning quando non e' mai stato usato
}PluginNamesFields;

typedef struct StrRecPluginScriptNgle {
	char Name[80];
	int PluginId;
}RecPluginScriptFields;

// the table "pluginName = pluginId" read directly from script.txt file
typedef struct StrPluginTableScriptNgle {
	int TotPlugins;
	StrRecPluginScriptNgle VetPlugins[MAX_PLUGIN_NGLE];
}PluginTableScriptFields;

typedef struct StrGlobNgle {
	StrSoundSfx SoundVetSfx;
	StrDatiExtraAnimazioni RoomTexAnimate;
	WORD TotExtraInfoRoom; // numero di stanze effettivamente presenti
	StrExtraInfoRoom VetExtraInfoRoom[0x400];
	StrBaseParziali BaseTexParziali;
	int VetInventory[128];  // slot di item inventario	
	StrCercaStatic VetRemapStatics[6000];
	short VetRemapRooms[MAX_ROOMS];
	StrVersionHeader BaseVersione;
	StrVersionHeader VersioneProject;
	StrBaseRemapTailInfo BaseRemapTail;
	WORD FlagsLevelTr4;   // FLT_
	WORD Flags2LevelTr4;  // FLT2_
	bool TestAttendiFineMouse;
	int TotPluginNgle;
	StrPluginNames VetPluginNgle[MAX_PLUGIN_NGLE];
	StrNGConstants VetPluginCostants[MAX_PLUGIN_NGLE];
	int TotPosFloor;
	WORD VetPosFloorIndex[512];
	BYTE VetPosFloorPluginId[512];
	StrPluginTableScriptNgle PluginScriptTable;
}GlobNgleFields;

// -----------------------  Tom2Pc Gestione Meta Cabinet ---------------------------------------

struct Cab_BaseFloorData {
    int TotFloorData;
    WORD *pVetFloorData;
};



//------ strutture per memorizzare tr4 file -----------------------
struct Cab_TriplePosShort {
    short X;
    short Y;
    short Z;
};

struct Cab_DoorTr4 {
    short LinkedRoomIndex;
    Cab_TriplePosShort VisiblePoint;
    Cab_TriplePosShort DoorCorners[4]; 
};
struct Cab_CollisionSectorTR4 {
    short FloorDataIndex;
    short BoxZoneIndex;
    BYTE BelowRoomIndex;
    BYTE FloorHeight;
    BYTE AboveRoomIndex;
    BYTE CeilingHeight;
};


struct Cab_LightTr4 {
    int OrigX;
    int OrigY;
    int OrigZ;
    BYTE RGB_Color[3]; 
    BYTE LightType;
    BYTE Mistery1;
    BYTE Intensity;
    float LightIn;
    float LIghtOut;
    float LightLen;
    float LightCutOff;
    float Target[3]; 
   
};

struct Cab_StaticMeshTr4 {
    int X; 
    int Y; 
    int Z; 
    short Rotation; 
    short Color; 
    short OCB; 
    short ObjectSlotId;
};


struct Cab_RoomTr4 {
    int OrgZ; 
    int OrgX; 
    int OrgYBottom;
    int OrgYTop; 
    int StaticDataWords;
    short *pStaticData;
    short NDoors;
    Cab_DoorTr4 * pVetDoors;
    short XSizeSectors;
    short ZSizeSectors; 
    //questa sarebbe una matrice bidimensionale (z,x)
    Cab_CollisionSectorTR4 *pCollisionSectorGrid; 
    int LightIntensity;
    short NLights; 
    Cab_LightTr4 *pVetLights;
    short NStaticMesh;
    Cab_StaticMeshTr4 *pVetStaticMesh; 
    short AlternateRoom;
    short RoomFlags;
    BYTE WaterWaveIntensity;
    short Mistery;
};

struct Cab_SlotT4 {
    int ObjectId;
    short NMesh;
    short StartingMesh;
    int MeshTree;
    int FrameOffset;
    short Animation;
};
struct Cab_CollBoxShort {
    short XMin;
    short XMax;
    short YMin;
    short YMax;
    short ZMin;
    short ZMax;
};

struct Cab_StaticSlotTr4 {
    int ObjectId;
    short MeshIndex;
    Cab_CollBoxShort ViewBox;
    Cab_CollBoxShort CollisionBox; 
    short StaticFlags;
};

struct Cab_SpriteTexTr4 {
    short TexturePage;
    BYTE Origin[2]; //x and y byte
    short Size[2];  //sizex and sizey
    short RelativePosition[4];
};

struct Cab_SpriteSeq {
    int ObjectId;
    short NegativeLength;
    short FirstSpriteTexture;
};
struct Cab_PosLong {
    int X;
    int Y;
    int Z;
};

struct Cab_CameraTr4 {
    Cab_PosLong Position;
    short Room;
    short CameraFlags;
};

struct Cab_FlyCameraTr4 {
    Cab_PosLong Position;
    Cab_PosLong Target;
    BYTE Seq;
    BYTE Num;
    short FrameOffset;
    short Roll;
    short Timer;
    short Speed;
    short CameraButtons;
    short RoomIndex;
    short FOV;
};

struct Cab_LocalSound {
    Cab_PosLong Position;
    short SoundId;
    short Flags;
};

struct Cab_BoxTr4 {
    BYTE X_Range[2];  //min x max x bytes
    BYTE Z_Range[2];  //min z max z bytes
    short Flags;
    short OverlapIndex;
};



struct Cab_VerticeShort2d {
    short X;
    short Y;
};
struct Cab_VerticaLong2d {
    int X;
    int Y;
};

struct Cab_TexInfoTr4 {
    short TransparentFlags;
    short TailIndex;
    short BumpMappingFlags;
    Cab_VerticeShort2d Vertices[4];
    Cab_VerticaLong2d Origin;
    Cab_VerticaLong2d SizeXY;
};

struct Cab_ItemTr4 {
    short ObjectId;
    short RoomIndex;
    Cab_PosLong Position;
    short HOrient;
    short Color;
    short OCB;
    short Buttons;
};

struct Cab_AiDataTr4 {
    short ObjectId;
    short RoomIndex;
    Cab_PosLong Position;
    short OCB;
    short ObjectButtons;
    int Angle;
};

struct Cab_SoundInfosTr4 {
    short SampleIndex; 
    BYTE Volume;
    BYTE RAD;
    BYTE CH;
    BYTE PIT;
    short Flags;
};

struct Cab_AnimationTr4 {
    int FrameOffset;
    BYTE FrameRate;
    BYTE FrameSize;
    short StateId;
    int Speed;
    int Accel;
    int SpeedSide;
    int AccelSide;
    short FrameStart;
    short FrameEnd;
    short NextAnimation;
    short NextFrame;
    short NumStateChanges;
    short StateChangeOffset;
    short NumAnimCommands;
    short AnimCommand;
};

struct Cab_StateChanges {
    short StateId;
    short N_AnimDispatches;
    short AnimDispatch;
};

struct Cab_AnimDispatchTr4 {
    short Low;
    short High;
    short NextAnimation;
    short NextFrame;
};

struct Cab_AnimationData {
    int NAnimation;
    Cab_AnimationTr4 *pVetAnimation; 
    int N_StateChanges;
    Cab_StateChanges *pVetStateChanges;
    int NAnimDispatch;
    Cab_AnimDispatchTr4 *pVetAnimDispatch; 
    int NAnimCommand;
    short *pVetAnimCommand; 
    int NMeshTree;
    int *pVetMeshTree; 
    int NFrames;
    short *pVetFrames;
};

struct Cab_GeometryTr4 {
    int Mistery;
    short N_Rooms;
    Cab_RoomTr4 *pVetRooms;
    Cab_BaseFloorData FloorData;
    int ObjectMeshNWords;
    short *pObjectMesh;
    int N_MeshPointer;
    int *pVetMeshPointer;
    Cab_AnimationData AnimationData;
    int NSlotMoveables;
    Cab_SlotT4 *pVetSlotMoveables; 
    int NStaticObjects;
    Cab_StaticSlotTr4 *pVetStaticObjects; 
    char MexSpiteTex[4];  // i caratteri sarebbero 3 ma ne alloco 4 per mettere lo zero finale
    int NSpriteTex;
    Cab_SpriteTexTr4 *pVetSpriteTex;
    int NSpriteSeq;
    Cab_SpriteSeq *pVetSpriteSeq;
    int NCameras;
    Cab_CameraTr4 *pVetCameras; 
    int NFlyBy;
    Cab_FlyCameraTr4 *pVetFlyBy;
    int NLocalSounds;
    Cab_LocalSound *pVetLocalSounds;
    int NBox;
    Cab_BoxTr4 *pVetBox; 
    int NOverlaps;
    short *pVetOverlaps;
    short *pVetZones; //matrice (10 x NBox)
    int DataAnimWords;
    short *pVetAnimWords;
    BYTE NUvRange; //nessun vettore, sara// il primo dei range gia// conosciuti
    char ObjTextCheck[4];   // i caratteri sarebbero 3 ma ne alloco 4 per mettere lo zero finale
    int NTexInfo;
    Cab_TexInfoTr4 *pVetTexInfo;
    int NItems;
    Cab_ItemTr4 *pVetItems;
    int N_AiData;
    Cab_AiDataTr4 *pVetAiData; 
    short SizeDemoData;
    short *pSoundTables;  //370 o 2048 (se SizeDemoData = 2048)
    int NSoundInfos;
    Cab_SoundInfosTr4 *pVetSoundInfos;
    int NSoundIndices;
    int *pVetSoundIndices;
    BYTE FillerArray[6]; 

};

struct Cab_SoundEffect {
    int SizeUncompress;
    int SizeCompress;
    BYTE *pSoundBytes; //compressed
};


struct Cab_BaseTr4 {
    char MexTr4[4];
    short NRoomTexPages;
    short NObjTexPages;
    short NRoomBumPages; //forse sprite
    int Tex1SizeUncompress;
    int Tex1SizeCompress;
    BYTE *pTex1Bytes; //compressed
    int Tex2SizeUncompress;
    int Tex2SizeCompress;
    BYTE *pTex2Bytes;
    int Tex3SizeUncompress;
    int Tex3SizeCompress;
    BYTE *pTex3Bytes;
    int GeometrySizeUncompress;
    int GeometrySizeCompress;
    Cab_GeometryTr4 Geometry;
    int NSoundEffect;
    Cab_SoundEffect *pVetSoundEffects;

};


typedef struct Cab_HeaderCabinet {
    DWORD SizeHeader;		// numero di versione (parte da 100)
    char NomeMetaTr4[256];  //nome originale di file tr4 caricato
    double Date;			// data in cui e' stato aggiunto file tr4
    DWORD FlagsCAB;			//flags di tipo CAB_ flags
    DWORD FlagsMETA;		//flags di tipo META_  per stabulire cosa importare
}HeaderCabinetFields;

typedef struct Cab_Cabinet {
    Cab_HeaderCabinet Header;
    Cab_BaseTr4 MetaLevel;
	StrExtractNG ExtraNgHeader;
}CabinetFields;

typedef struct StrTr4Zl	{
    int SizeCompressed;
    int SizeUncompressed;
    int SizeFile;  // intero file .tr4 o cabinet
    FILE *pFile; // 'handle principale di file
}Tr4ZlFields;

// struttura per mia gestione di memoria basata su un unico blocco (unico limite: le zone di memoria richeiste 
// non sono ridimensionsabili)

struct StrMiaMemory {
	BYTE *pBaseMem; // da usare per liberarla
	int SizeTotale; // totale memoria allocata
	BYTE *pPointer; // punta a memoria libera (verra' aumentato ogni volta che si concede della memoria)
	BYTE *pEndMemory; // punta a byte successivo a zona di memoria (per controllo di overflow)
};


// duplicato preso da structures.h
typedef struct StrTablePluginIdFloor {
	int TotTable;
	BYTE VetTable[0x10000];
}TablePluginIdFloor;
typedef struct StrTripleteId {
	WORD IndexFloorOld; // posiizone di trigger (all'interno di floordata) in meta level
	WORD IndexFloorNow;  // posiizione attuale in nuovo livello di trigger
	short IdPlugin;  // id del plugin (preso da vecchia tabella)
}TripleteIdFields;

// struttura per operazioni di aggiornmento id plugin in fusioine di due livelli
typedef struct StrFloorConvert {
	StrTablePluginIdFloor PluginTableOld;
	StrTablePluginIdFloor PluginTable;
	int TotConvert;
	StrTripleteId VetConvert[512];
}FloorConvertFields;

// struttura globale per tom2pc e gestione cabinet
typedef struct StrGlobMetaPc {
	DWORD OffsetGeoLev; // 0000: offset a partire dal quale, in file.lev, c'e' geometria
	bool TestUsaMetaCab; // 0004: imposta se usare o meno file metacab
	char NomeFileLev[256];
	char NomeFileCab[256]; // nome file .mtcab in graphic\wads
	char NomeFileTom[256];
	Cab_Cabinet BaseCabinet; // cabinet caricato in memoria
	Cab_GeometryTr4 GeometryLevel; // geometria di livello ngle appena creato
	StrFloorConvert BaseConvertID; // dati per conversione id plugin dopo aver spostato triggers
}GlobMetaPcFields;

typedef struct StrFloorEstratti {
	WORD ValTrigger; // word estratta da floor old
	int IndexFloorOld; // indice di posizione in vecchia tabella floor
}FloorEstrattiFields;

// ------------------------ fine Tom2Pc Gestione Meta Cabinet -----------------------------------

#pragma pack(2)
