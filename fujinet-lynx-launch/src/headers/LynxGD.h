#ifndef __LYNX_GD__
#define __LYNX_GD__

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef struct
{
	char	szFilename[13];
	u8		bDirectory;
} SDirEntry;

typedef enum {
	FR_OK = 0,			/* 0 */
	FR_DISK_ERR,		/* 1 */
	FR_NOT_READY,		/* 2 */
	FR_NO_FILE,			/* 3 */
	FR_NOT_OPENED,		/* 4 */
	FR_NOT_ENABLED,		/* 5 */
	FR_NO_FILESYSTEM	/* 6 */
} FRESULT;

typedef struct {
	u32		fsize;		/* File size */
	u16		fdate;		/* Last modified date */
	u16		ftime;		/* Last modified time */
	u8		fattrib;	/* Attribute */
	char	fname[13];	/* File name */
} SFileInfo;

#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
#define AM_MASK	0x3F	/* Mask of defined bits */

void LynxGD_Init();
void LynxGD_LowPowerMode();
FRESULT __fastcall__ LynxGD_OpenDir(const char *pDir);
FRESULT __fastcall__ LynxGD_ReadDir(SFileInfo *pInfo);
FRESULT __fastcall__ LynxGD_OpenFile(const char *pFile);
FRESULT __fastcall__ LynxGD_ReadFile(void *pBuffer, u16 nSize);
FRESULT __fastcall__ LynxGD_WriteFile(void *pBuffer, u16 nSize);
FRESULT LynxGD_CloseFile();
FRESULT __fastcall__ LynxGD_SeekFile(u32 nSeekPos);
u32 LynxGD_GetFileSize();
FRESULT __fastcall__ LynxGD_ProgramROMFromFile(u16 nStartBlock, u8 nBlockSize, u16 nBlockCount, u8 b512BlockCard);
FRESULT __fastcall__ LynxGD_ClearROMBlocks(u16 nStartBlock, u16 nBlocks, u8 b512BlockCard);

#endif // __LYNX_SD__