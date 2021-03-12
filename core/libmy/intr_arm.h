#ifndef __INTR_H__
#define __INTR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------

enum {
	INTR_VBLANK = 0,
	INTR_HBLANK,
	INTR_VCOUNT,
	INTR_TIMER0,
	INTR_TIMER1,
	INTR_TIMER2,
	INTR_TIMER3,
	INTR_SERIAL,
	INTR_DMA0,
	INTR_DMA1,
	INTR_DMA2,
	INTR_DMA3,
	INTR_KEYPAD,
	INTR_GAMEPAK,
};


//---------------------------------------------------------------------------
typedef struct {
	void* handler[MAX_INTS] ALIGN(4);
} ST_INTR_TABLE;


//---------------------------------------------------------------------------
EWRAM_CODE void IntrInit(void);
IWRAM_CODE void IntrMain(void);
IWRAM_CODE void IntrDummy(void);

EWRAM_CODE void IntrStart(void);
EWRAM_CODE void IntrStop(void);

EWRAM_CODE void IntrSetEnable(irqMASK mask);
EWRAM_CODE void IntrSetDisable(irqMASK mask);
EWRAM_CODE void IntrSetHandler(u16 num, void* func);
EWRAM_CODE void IntrSetVcount(s16 cnt);


#ifdef __cplusplus
}
#endif
#endif
