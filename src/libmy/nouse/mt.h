#ifndef __MT_H__
#define __MT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
EWRAM_CODE void MtInit(void);
IWRAM_CODE void MtSrand(u32 s);
IWRAM_CODE u32  MtRand(u16 num);




#ifdef __cplusplus
}
#endif
#endif

