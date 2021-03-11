#ifndef __BIOS_H__
#define __BIOS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

typedef struct {
	u32 dummy ALIGN(4);
} ST_BIOS;


//---------------------------------------------------------------------------
EWRAM_CODE void BiosInit(void);

IWRAM_CODE void BiosExec(void* src, void* dst, u32 size);
IWRAM_CODE void BiosExecFix(void* src, void* dst, u32 size);
IWRAM_CODE void BiosExecFixClear(void* dst, u32 size);

IWRAM_CODE void BiosExecFast(void* src, void* dst, u32 size);
IWRAM_CODE void BiosExecFastFix(void* src, void* dst, u32 size);
IWRAM_CODE void BiosExecFastFixClear(void* dst, u32 size);


#ifdef __cplusplus
}
#endif
#endif
