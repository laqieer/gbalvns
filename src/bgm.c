#include "bgm.h"
#include "libmy/ad_arm.h"


//---------------------------------------------------------------------------
ST_BGM Bgm;


//---------------------------------------------------------------------------
EWRAM_CODE void BgmInit(void)
{
	_Memset(&Bgm, 0x00, sizeof(ST_BGM));
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmPlay(u16 no, bool isLoop)
{
	_ASSERT(no <= BGM_MAX_SND_CNT);

	if(Bgm.no == no && Bgm.isLoop == isLoop)
	{
		return;
	}

	if(no == 0)
	{
		AdStop();
		return;
	}


	char buf[64] ALIGN(4);
	_Sprintf(buf, BGM_SND_STR, no);

	AdSetData2(buf, isLoop);
	AdPlay();

	Bgm.no = no;
	Bgm.isLoop = isLoop;
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmPlayRestart(void)
{
	bool isLoop = Bgm.isLoop;
	u16  no     = Bgm.no;

	BgmStop();
	BgmPlay(no, isLoop);
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmPlayNext(void)
{
	// ƒV[ƒ“Ø‚è‘Ö‚¦Žž‚ÉÄ¶‚µ‚Ü‚·

	if(Bgm.isNext == TRUE)
	{
		Bgm.isNext = FALSE;

		BgmPlay(Bgm.nextNo, Bgm.isNextLoop);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmStop(void)
{
	Bgm.no = 0;
	AdStop();
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmStopOut(void)
{
	Bgm.no = 0;
	AdStopOut();
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgmSetNext(u16 no, bool isLoop)
{
	Bgm.isNext = TRUE;

	Bgm.nextNo = no;
	Bgm.isNextLoop = isLoop;
}
