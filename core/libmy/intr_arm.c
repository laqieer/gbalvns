#include "intr_arm.h"


//---------------------------------------------------------------------------------
ST_INTR_TABLE IntrTable;


//---------------------------------------------------------------------------------
EWRAM_CODE void IntrInit(void)
{
	REG_IME =  0;
	REG_IF  = ~0;
	REG_IE  =  0;

	u16 i;

	for(i=0; i<MAX_INTS; i++)
	{
		IntrTable.handler[i] = IntrDummy;
	}

	INT_VECTOR = (u32)IntrMain;
}
//---------------------------------------------------------------------------------
IWRAM_CODE void IntrMain(void)
{
	asm volatile (
		"ldr	r3, =0x04000200"	"\n"
		"ldrh	r1, [r3, #0x08]"	"\n" // r1に REG_IME をロード
		"mrs	r0, spsr"			"\n"
		"stmfd	sp!, {r0,r1,lr}"	"\n" // SPSR, REG_IME, lrを保存
		"mov	r0, #1"				"\n"
		"strh	r0, [r3, #0x08]"	"\n" // REG_IME に#1をストア

		"ldr	r1, [r3,#0x00]"		"\n" // r1に REG_IE, REG_IF をロード
		"and	r0, r1,r1,lsr #16"	"\n" // r0 = REG_IE & REG_IF

		"ldr	r2, =IntrTable"		"\n" // r2 = &IntrTable[0]

		//-------------------------------------------
		"ands	r1, r0, #1"			"\n" // VBlank
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #2"			"\n" // HBlank
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #4"			"\n" // VCount
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #8"			"\n" // Timer0
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #16"		"\n" // Timer1
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #32"		"\n" // Timer2
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #64"		"\n" // Timer3
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #128"		"\n" // Serial
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #256"		"\n" // DMA0
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #512"		"\n" // DMA1
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #1024"		"\n" // DMA2
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #2048"		"\n" // DMA3
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n"
		"ands	r1, r0, #4096"		"\n" // Joy 
		"bne	.Ljump_intr"		"\n"
		"add	r2, r2, #4"			"\n" 
		"ands	r1, r0, #8192"		"\n" // Cart

#if HANDLE_CART_INTERRUPT
		"strneb  r1, [r3, #0x84 - 0x200]" "\n" // Stop sound if cart removed (REG_SOUNDCNT_X)
".Lcart_loop:"						"\n"
		"bne	.Lcart_loop"		"\n" // Infinite loop if cart removed
#endif

".Ljump_intr:"						"\n"
		"strh	r1, [r3, #0x2]"		"\n" // Acknowledge Interrupt(REG_IF Clear)

		"mrs	r3, CPSR"			"\n"
#if 0
		"bic	r3, r3, #0xdf"		"\n" // Switch to System Mode
		"orr	r3, r3, #0x1f"		"\n"
//		"mov	r3, #0x12"			"\n" // Switch to System Mode
#else
		"orr	r3, r3, #0xc0"		"\n" // Switch to IRQ Mode
#endif
		"msr	CPSR, r3"			"\n"
		"ldr	r1, [r2]"			"\n"
		"adr	lr, .Lintr_return"	"\n"
		"bx		r1"					"\n"

".Lintr_return:"					"\n"
		"mrs	r3, CPSR"			"\n"
#if 0
		"bic	r3, r3, #0xdf"		"\n"
		"orr	r3, r3, #0x92"		"\n" // Switch to IRQ Mode, disable IRQ
#else
		"orr	r3, r3, #0x80"		"\n" // Disable IRQ
#endif
		"msr	CPSR, r3"			"\n"
		"ldmfd	sp!,{r0,r1,lr}"		"\n"
		"msr	spsr, r0"			"\n"
	::);
	return;
	asm (".pool");
}
//---------------------------------------------------------------------------------
IWRAM_CODE void IntrDummy(void)
{
	// EMPTY
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrStart(void)
{
	REG_IME = 1;
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrStop(void)
{
	REG_IME = 0;
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrSetEnable(irqMASK mask)
{
	REG_IME = 0;

	if(mask & IRQ_VBLANK) REG_DISPSTAT |= LCDC_VBL;
	if(mask & IRQ_HBLANK) REG_DISPSTAT |= LCDC_HBL;
	if(mask & IRQ_VCOUNT) REG_DISPSTAT |= LCDC_VCNT;

	REG_IE |= mask;
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrSetDisable(irqMASK mask)
{
	REG_IME = 0;

	if(mask & IRQ_VBLANK) REG_DISPSTAT &= ~LCDC_VBL;
	if(mask & IRQ_HBLANK) REG_DISPSTAT &= ~LCDC_HBL;
	if(mask & IRQ_VCOUNT) REG_DISPSTAT &= ~LCDC_VCNT;

	REG_IE &= ~mask;
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrSetHandler(u16 num, void* func)
{
	if(num >= MAX_INTS)
	{
		return;
	}

	if(func != NULL)
	{
		IntrTable.handler[num] = func;
	}
	else
	{
		IntrTable.handler[num] = IntrDummy;
	}
}
//---------------------------------------------------------------------------------
EWRAM_CODE void IntrSetVcount(s16 cnt)
{
	REG_DISPSTAT |= VCOUNT(cnt);
}
