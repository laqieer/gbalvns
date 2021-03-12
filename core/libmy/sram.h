#ifndef __SRAM_H__
#define __SRAM_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

typedef struct {
	u8* p;
} ST_SRAM;


//---------------------------------------------------------------------------
EWRAM_CODE void SramInit(void);
EWRAM_CODE void SramSeek(u32 offset);
EWRAM_CODE u8*  SramGetPointer(void);

EWRAM_CODE u8   SramRead8(void);
EWRAM_CODE u16  SramRead16(void);
EWRAM_CODE u32  SramRead32(void);
EWRAM_CODE void SramReadCopy(u8* p, u32 size);

EWRAM_CODE void SramWrite8(u8 data);
EWRAM_CODE void SramWrite16(u16 data);
EWRAM_CODE void SramWrite32(u32 data);
EWRAM_CODE void SramWriteCopy(u8* p, u32 size);

EWRAM_CODE bool SramIsStr(char* str);

#ifdef __cplusplus
}
#endif
#endif
