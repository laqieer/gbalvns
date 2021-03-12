#include "irq_arm.h"
#include "libmy/intr_arm.h"
// #include "libmy/debug.h"
// #include "libmy/snd.h"
#include "libmy/ad_arm.h"


//---------------------------------------------------------------------------------
EWRAM_CODE void IrqInit(void)
{
	IntrSetHandler(INTR_VBLANK, IrqVblank);
	IntrSetEnable(IRQ_VBLANK);

	IntrSetHandler(INTR_VCOUNT, IrqVcount);
	IntrSetEnable(IRQ_VCOUNT);
	IntrSetVcount(0);

//	IntrSetHandler(INTR_TIMER0, IrqTimer0);
//	IntrSetEnable(IRQ_TIMER0);
}
//---------------------------------------------------------------------------------
IWRAM_CODE void IrqTimer0(void)
{
	// EMPTY
}
//---------------------------------------------------------------------------------
IWRAM_CODE void IrqVcount(void)
{
	AdIntrVcount();
}
//---------------------------------------------------------------------------------
IWRAM_CODE void IrqVblank(void)
{
//	DebugIntr();

//	SndBgmIntr();
//	SndSeIntr();

//	AdrvIntr();
	AdIntrVblank();

	REG_IRQ_WAITFLAGS |= IRQ_VBLANK;
}
