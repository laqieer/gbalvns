#include "sys.h"
#include "libmy/libmy.h"
#include "irq_arm.h"
#include "game.h"

//---------------------------------------------------------------------------
ST_SYS Sys;


//---------------------------------------------------------------------------
EWRAM_CODE void SysInit(void)
{
	REG_WSCNT = 0x4317;
	Sys.act = SYS_INIT;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SysExec(void)
{
	for(;;)
	{
		switch(Sys.act)
		{
		case SYS_INIT:
			SysExecInit();
			break;

		case SYS_RUN:
			SysExecRun();
			break;

		default:
			SystemError("[Err] SysExec Sys.act=%d\n", Sys.act);
			break;
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void SysExecInit(void)
{
//	MtInit();
//	MemInit();
	SramInit();
	GbfsInit();

//	BgInit();
	Mode3Init();

	SprInit();
	KeyInit();
//	DmaInit();
	BiosInit();
	LexInit();
	FadeInit();
//	DebugInit();

	AdInit();
//	SndInit();
//	AdrvInit();

	IntrInit();
	IrqInit();
	IntrStart();

	GameInit();


	Sys.act = SYS_RUN;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SysExecRun(void)
{
	for(;;)
	{
		SystemCall(5);

		// Dma
//		DmaTransfer();

		// Draw
		FadeDraw();
		SprDraw();
		Mode3Draw();
//		DebugDraw();

		// Exec
//		BgExec();
		KeyExec();
//		DebugExec();
		GameExec();
	}
}
