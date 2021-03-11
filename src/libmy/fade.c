#include "fade.h"


//---------------------------------------------------------------------------
ST_FADE Fade;


//---------------------------------------------------------------------------
EWRAM_CODE void FadeInit(void)
{
	_Memset(&Fade, 0x00, sizeof(ST_FADE));

	REG_BLDY   = 0;
	REG_BLDCNT = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDefault(u16 mode)
{
	Fade.isEnd   = FALSE;
	Fade.isClear = FALSE;
	Fade.cnt     = 0;
	Fade.min     = 0;
	Fade.max     = 15;
	Fade.wait    = 0;
	Fade.waitCnt = 0;

	REG_BLDCNT = (mode | BLEND_TOP_BG2);
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDrakIn(u16 wait)
{
	FadeSetDefault(BLEND_MODE_DARK);
	Fade.wait = wait;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDrakIn2(u16 wait, u16 max)
{
	FadeSetDefault(BLEND_MODE_DARK);
	Fade.wait = wait;
	Fade.max  = max;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDrakIn3(u16 cnt)
{
	FadeSetDefault(BLEND_MODE_DARK);
	Fade.cnt = cnt;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDrakOut(u16 wait)
{
	FadeSetDefault(BLEND_MODE_DARK);
	Fade.cnt  = 15;
	Fade.wait = wait;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDrakOut2(u16 wait, u16 cnt)
{
	FadeSetDefault(BLEND_MODE_DARK);
	Fade.cnt  = cnt;
	Fade.wait = wait;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetLightIn(u16 wait)
{
	FadeSetDefault(BLEND_MODE_LIGHT);
	Fade.wait = wait;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetLightOut(u16 wait)
{
	FadeSetDefault(BLEND_MODE_LIGHT);
	Fade.cnt  = 15;
	Fade.wait = wait;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetClear(void)
{
	Fade.isClear = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeSetDirect(u16 num)
{
	// —áŠOˆ’u
	REG_BLDY = num;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeExecIn(void)
{
	if(Fade.cnt >= Fade.max)
	{
		Fade.isEnd = TRUE;
		return;
	}

	if(Fade.waitCnt++ < Fade.wait)
	{
		return;
	}
	Fade.waitCnt = 0;

	Fade.cnt++;
	Fade.isEnd   = FALSE;
	Fade.isClear = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeExecOut(void)
{
	if(Fade.cnt <= Fade.min)
	{
		Fade.isEnd = TRUE;
		return;
	}

	if(Fade.waitCnt++ < Fade.wait)
	{
		return;
	}
	Fade.waitCnt = 0;

	Fade.cnt--;
	Fade.isEnd   = FALSE;
	Fade.isClear = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void FadeDraw(void)
{
	if(Fade.isClear == TRUE)
	{
		REG_BLDY = 0;
		return;
	}

	if(FadeIsEnd() == FALSE)
	{
		REG_BLDY = Fade.cnt;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE bool FadeIsEnd(void)
{
	return Fade.isEnd;
}
