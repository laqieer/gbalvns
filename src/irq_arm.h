#ifndef __IRQ_H__
#define __IRQ_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
EWRAM_CODE void IrqInit(void);

IWRAM_CODE void IrqTimer0(void);
IWRAM_CODE void IrqVcount(void);
IWRAM_CODE void IrqVblank(void);


#ifdef __cplusplus
}
#endif
#endif
