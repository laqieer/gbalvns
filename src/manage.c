#include "manage.h"
#include "anime.h"
#include "script.h"
#include "img.h"
#include "menu.h"
#include "text.h"


//---------------------------------------------------------------------------
ST_MANAGE Manage;


//---------------------------------------------------------------------------
EWRAM_CODE void ManageInit(void)
{
	_Memset(&Manage, 0x00, sizeof(ST_MANAGE));

	ManageSetAct(MANAGE_ACT_JINGLE);
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageSetAct(u16 act)
{
	Manage.act = act;
	Manage.actStep = 0;
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageExec(void)
{
	switch(Manage.act)
	{
	case MANAGE_ACT_JINGLE:
		ManageExecJingle();
		break;

	case MANAGE_ACT_OPENING:
		ManageExecOpening();
		break;

	case MANAGE_ACT_TITLE:
		ManageExecTitle();
		break;

	case MANAGE_ACT_SCRIPT:
		ManageExecScript();
		break;

	default:
		SystemError("[Err] ManageExec Manage.act=%d\n", Manage.act);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageExecJingle(void)
{
	switch(Manage.actStep)
	{
	case 0:
		AnimeSetType(ANIME_TYPE_JINGLE, TRUE);
		Manage.actStep++;
		break;

	case 1:
		AnimeExec();

		if(AnimeIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 2:
		if(AnimeIsKeyOn() == TRUE)
		{
			ManageSetAct(MANAGE_ACT_TITLE);
		}
		else
		{
			ManageSetAct(MANAGE_ACT_OPENING);
		}
		break;

	default:
		SystemError("[Err] ManageExecJingle Manage.actStep = %d\n", Manage.actStep);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageExecOpening(void)
{
	switch(Manage.actStep)
	{
	case 0:
		AnimeSetType(ANIME_TYPE_OPENING, TRUE);

		Manage.actStep++;
		break;

	case 1:
		AnimeExec();

		if(AnimeIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 2:
		ManageSetAct(MANAGE_ACT_TITLE);
		break;

	default:
		SystemError("[Err] ManageExecOpening Manage.actStep = %d\n", Manage.actStep);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageExecTitle(void)
{
	switch(Manage.actStep)
	{
	case 0:
		if(AnimeIsKeyOn() == TRUE)
		{
			AnimeSetType(ANIME_TYPE_TITLE, FALSE);
			Manage.actStep++;
		}
		else
		{
			Manage.actStep = 2;
		}
		break;

	case 1:
		AnimeExec();

		if(AnimeIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 2:
		ImgSetEffectUpdate(IMG_EFFECT_TEXT);
		Manage.actStep++;
		break;

	case 3:
		if(ImgIsEnd() == TRUE)
		{
			MenuSetTitle();
			Manage.actStep++;
		}
		break;

	case 4:
		MenuExec();

		if(MenuIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 5:
		ImgSetEffectClear2(IMG_EFFECT_FADE_BLACK, 2);
		Manage.actStep++;
		break;

	case 6:
		if(ImgIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 7:
		ManageSetAct(MANAGE_ACT_SCRIPT);
		break;

	default:
		SystemError("[Err] ManageExecTitle Manage.actStep = %d\n", Manage.actStep);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void ManageExecScript(void)
{
	switch(Manage.actStep)
	{
	case 0:
		ScriptExec();

		if(ScriptIsEnd() == TRUE)
		{
			Manage.actStep++;
		}
		break;

	case 1:
		// ïúãAèàóù
		AnimeSetKeyOn(TRUE);
		ManageSetAct(MANAGE_ACT_TITLE);
		break;

	default:
		SystemError("[Err] ManageExecScript Manage.actStep = %d\n", Manage.actStep);
		break;
	}
}
