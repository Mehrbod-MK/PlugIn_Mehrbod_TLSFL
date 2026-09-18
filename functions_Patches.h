typedef void (__cdecl* TYPE_StopAllSounds) (void);
TYPE_StopAllSounds StopAllSounds = (TYPE_StopAllSounds) 0x45EC50;

typedef void* (__cdecl * TYPE_GetFloor) (DWORD CordX, int CordY, DWORD CordZ, short* pRoom);
TYPE_GetFloor GetFloor = (TYPE_GetFloor) 0x449880;

typedef void (__cdecl *TYPE_AddActiveItem) (int Indice);
TYPE_AddActiveItem AddActiveItem = (TYPE_AddActiveItem) 0x453C10;

typedef void (__cdecl* TYPE_SoundEffect) (int NumeroSample, void *pCoordinate, int Flags);

TYPE_SoundEffect SoundEffect = (TYPE_SoundEffect) 0x45E440;


typedef bool (__cdecl *TYPE_OnTwoBlockPlatform) (StrItemTr4 *pItem, DWORD CordX, DWORD CordZ);
TYPE_OnTwoBlockPlatform OnTwoBlockPlatform = (TYPE_OnTwoBlockPlatform) 0x415A10;

typedef int (__cdecl *TYPE_GetCeiling) (void *pFloor, DWORD CordX, int CordY, DWORD CordZ);
TYPE_GetCeiling GetCeiling = (TYPE_GetCeiling) 0x44AD20;

typedef int (__cdecl* TYPE_ControlSideCar) (short ItemIndex);
TYPE_ControlSideCar ControlSideCar = (TYPE_ControlSideCar) 0x463E00;

typedef int (__cdecl* TYPE_ControlJeep) (short ItemIndex);
TYPE_ControlJeep ControlJeep = (TYPE_ControlJeep) 0x466610;

typedef void (__cdecl* TYPE_PrintString) (int OrgX, int OrgY, WORD Colore, char *pTesto, WORD Flags);
TYPE_PrintString PrintString = (TYPE_PrintString) 0x462D00;

typedef int (__cdecl* TYPE_DoBar) (int CordX, int CordY, int SizeX, int SizeY, int Percentuale, DWORD Colore, DWORD ColoreFade);
TYPE_DoBar DoBar = (TYPE_DoBar) 0x47AF60;

typedef void (__cdecl * TYPE_CreatureDie) (int ItemIndex, bool TestEsplosione);
TYPE_CreatureDie CreatureDie =(TYPE_CreatureDie) 0x440840;

typedef void (__cdecl *TYPE_phd_RotY) (WORD Orient);
TYPE_phd_RotY phd_RotY = (TYPE_phd_RotY) 0x48E130;
