#include "debug.h"
#include "bg.h"


//---------------------------------------------------------------------------
ST_DEBUG Debug;


//---------------------------------------------------------------------------
EWRAM_CODE void DebugInit(void)
{
	_Memset(&Debug,  0x00, sizeof(ST_DEBUG));
}
//---------------------------------------------------------------------------
IWRAM_CODE void DebugExec(void)
{
	ST_DEBUG* p = &Debug;

	p->fpsCnt++;
	p->gameTime++;
	p->gameFrame++;

	if(p->gameFrame < p->frameCnt)
	{
		p->delay     = p->frameCnt - p->gameFrame;
		p->gameFrame = p->frameCnt;
	}
	else
	{
		p->delay = 0;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void DebugDraw(void)
{
	char str[32];

	_Sprintf(str, "DLY:%4d", Debug.delay);
	BgDrawStr(BG_SYS, 22, 18, str);
	_Sprintf(str, "FPS:%4d", Debug.fps);
	BgDrawStr(BG_SYS, 22, 19, str);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DebugIntr(void)
{
	ST_DEBUG* p = &Debug;

	p->frameCnt++;
	p->fpsTimer++;

	if(p->fpsTimer == DEBUG_FPS_CNT)
	{
		p->fpsTimer = 0;
		p->fps      = p->fpsCnt;
		p->fpsCnt   = 0;
	}
}
