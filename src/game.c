#include "game.h"
#include "manage.h"
#include "anime.h"
#include "bgm.h"
#include "img.h"
#include "text.h"
#include "history.h"
#include "script.h"
#include "siori.h"


//---------------------------------------------------------------------------
ST_GAME Game;


//---------------------------------------------------------------------------
EWRAM_CODE void GameInit(void)
{
	_Memset(&Game, 0x00, sizeof(ST_GAME));

	Game.act = GAME_INIT;
}
//---------------------------------------------------------------------------
IWRAM_CODE void GameExec(void)
{
	switch(Game.act)
	{
	case GAME_INIT:
		GameExecInit();
		break;

	case GAME_RUN:
		GameExecRun();
		break;
	
	default:
		SystemError("[Err] GameExec Game.act=%d\n", Game.act);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void GameExecInit(void)
{
	ManageInit();

	BgmInit();
	ImgInit();
	TextInit();
	AnimeInit();
	SioriInit();
	ScriptInit();
	HistoryInit();

	Game.act = GAME_RUN;
}//---------------------------------------------------------------------------
IWRAM_CODE void GameExecRun(void)
{
	ManageExec();

	ImgExec();
	TextExec();
}
