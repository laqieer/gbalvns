#include "anime.h"
#include "img.h"
#include "bgm.h"
#include "res.h"
#include "text.h"
#include "libmy/lex.h"
#include "libmy/key.h"


//---------------------------------------------------------------------------
const ST_ANIME_DB AnimeDb[ANIME_MAX_DB_CNT] = {
	{ &ani_1_jingle_txt  },
	{ &ani_2_opening_txt },
	{ &ani_3_title_txt   },
	{ &ani_4_namida_txt  },
	{ &ani_5_ending_txt  },
	{ &ani_6_clear_txt   },
};

const ST_ANIME_TABLE AnimeTable[ANIME_MAX_TABLE_CNT] = {
	{ "image",    (void*)AnimeExecImage    },		// 背景画像　表示
	{ "image2",   (void*)AnimeExecImage2   },		// "雫"　　　表示（決め打ち）
	{ "image3",   (void*)AnimeExecImage3   },		// 連番画像　表示
	{ "update",   (void*)AnimeExecUpdate   },
	{ "print",    (void*)AnimeExecPrint    },
	{ "printcls", (void*)AnimeExecPrintCls },
	{ "fill",     (void*)AnimeExecFill     },
	{ "blackin",  (void*)AnimeExecBlackIn  },
	{ "blackout", (void*)AnimeExecBlackOut },
	{ "whitein",  (void*)AnimeExecWhiteIn  },
	{ "whiteout", (void*)AnimeExecWhiteOut },
	{ "wait",     (void*)AnimeExecWait     },
	{ "waitkey",  (void*)AnimeExecWaitKey  },
	{ "wave",     (void*)AnimeExecWave     },
	{ "bgm",      (void*)AnimeExecBgm      },
	{ "setval",   (void*)AnimeExecSetVal   },
	{ "setstr",   (void*)AnimeExecSetStr   },
	{ "setpos",   (void*)AnimeExecSetPos   },
	{ "inc",      (void*)AnimeExecInc      },
	{ "jb" ,      (void*)AnimeExecJb       },
	{ "end",      (void*)AnimeExecEnd      },
};


//---------------------------------------------------------------------------
ST_ANIME Anime;


//---------------------------------------------------------------------------
EWRAM_CODE void AnimeInit(void)
{
	_Memset(&Anime, 0x00, sizeof(ST_ANIME));
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeSetType(u16 type, bool isStop)
{
	LexSetCur(AnimeDb[type].pDat);

	Anime.type   = type;
	Anime.isStop = isStop;
	Anime.isKey  = FALSE;
	Anime.act    = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeSetFunc(void)
{
	u16 i;
	char* pStr = LexGetSafeStr();
	TRACEOUT("AnimeSetFunc: %s\n", pStr);


	for(i=0; i<ANIME_MAX_TABLE_CNT; i++)
	{
		if(_Strncmp(pStr, AnimeTable[i].pStr, ANIME_MAX_STR_LEN) == 0)
		{
			Anime.pFunc = (void*)AnimeTable[i].pFunc;
			Anime.funcStep = 0;

			return;
		}
	}

	_ASSERT(0 && "AnimeSetFunc miss match");
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeSetKeyOn(bool is)
{
	Anime.isKey = is;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExec(void)
{
	_ASSERT(Anime.act == ANIME_ACT_OP || Anime.act == ANIME_ACT_FUNC);


	if(ImgIsEnd() == FALSE)
	{
		return;
	}

	if(KeyIsTrgAB() == TRUE && Anime.isStop == TRUE)
	{
		AnimeSetType(ANIME_TYPE_CLEAR, FALSE);
		Anime.isKey = TRUE;
	}

	if(Anime.act == ANIME_ACT_OP)
	{
		AnimeSetFunc();
		Anime.act = ANIME_ACT_FUNC;
	}

	Anime.pFunc();
}
//---------------------------------------------------------------------------
// 背景画像
EWRAM_CODE void AnimeExecImage(void)
{
	char* pStr = LexGetSafeStr();
	char  buf[64] ALIGN(4);

	AnimeCalcRemoveStrQuart(buf, pStr);
	ImgSetBg(buf);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
// "雫"（決め打ち）
EWRAM_CODE void AnimeExecImage2(void)
{
	ImgSetBgLogo();

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
// 連番画像
EWRAM_CODE void AnimeExecImage3(void)
{
	char str[ANIME_MAX_STR_LEN];

	_Sprintf(str, Anime.str, Anime.val);
	ImgSetBg(str);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecUpdate(void)
{
	ImgSetEffectUpdate(IMG_EFFECT_NORMAL);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecPrint(void)
{
	char* pStr = LexGetSafeStr();

	u16 y = LexGetSafeNum();
	u16 x = (TEXT_SCREEN_CX / 2)  - (_Strlen(pStr) / 4) + 1;

	char  buf[64] ALIGN(4);
	AnimeCalcRemoveStrQuart(buf, pStr);


	ImgSetEffectUpdate(IMG_EFFECT_TEXT);
	TextSetStrSjis(x, y, (u16*)buf);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecPrintCls(void)
{
	TextSetClearNoBuf();

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecFill(void)
{
	ImgSetBgFill();

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecBlackIn(void)
{
	u16 num = LexGetSafeNum();
	ImgSetEffectClear2(IMG_EFFECT_FADE_BLACK, num);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecBlackOut(void)
{
	u16 num = LexGetSafeNum();
	ImgSetEffectUpdate2(IMG_EFFECT_FADE_BLACK, num);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecWhiteIn(void)
{
	u16 num = LexGetSafeNum();
	ImgSetEffectClear2(IMG_EFFECT_FADE_WHITE, num);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecWhiteOut(void)
{
	u16 num = LexGetSafeNum();
	ImgSetEffectUpdate2(IMG_EFFECT_FADE_WHITE, num);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecWait(void)
{
	switch(Anime.funcStep)
	{
	case 0:
		Anime.wait = LexGetSafeNum();
		Anime.waitCnt = 0;

		Anime.funcStep++;
		break;

	case 1:
		Anime.waitCnt++;
	
		if(Anime.waitCnt > Anime.wait)
		{
			Anime.act = ANIME_ACT_OP;
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecWaitKey(void)
{
	switch(Anime.funcStep)
	{
	case 0:
		Anime.isStop = FALSE;
		Anime.funcStep++;
		break;

	case 1:
		if(KeyIsTrgAB() == TRUE)
		{
			Anime.act = ANIME_ACT_OP;
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecWave(void)
{
	ImgSetEffectUpdate(IMG_EFFECT_WAVE);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecBgm(void)
{
	u16 num  = LexGetSafeNum();
	u16 num2 = LexGetSafeNum();

	bool isLoop = (num2 == 1) ? TRUE : FALSE;
	BgmPlay(num, isLoop);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecSetVal(void)
{
	Anime.val = LexGetSafeNum();

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecSetStr(void)
{
	char* pStr = LexGetSafeStr();
	char  buf[64] ALIGN(4);

	AnimeCalcRemoveStrQuart(buf, pStr);
	_Strncpy(Anime.str, buf, ANIME_MAX_STR_LEN);

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecSetPos(void)
{
	Anime.pCur = LexGetCur();

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecInc(void)
{
	Anime.val++;

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecJb(void)
{
	u16 num = LexGetSafeNum();

	if(Anime.val < num)
	{
		LexSetCur(Anime.pCur);
	}

	Anime.act = ANIME_ACT_OP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeExecEnd(void)
{
	BgmStop();

	Anime.act = ANIME_ACT_END;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool AnimeIsKeyOn(void)
{
	return Anime.isKey;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool AnimeIsEnd(void)
{
	return (Anime.act == ANIME_ACT_END) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AnimeCalcRemoveStrQuart(char* pDst, char* pSrc)
{
	// 文字列に入っているダブルクォートを除去します

	char* pS = pSrc;
	char* pD = pDst;

	while(*pS != '\0')
	{
		if(*pS == '"')
		{
			pS++;
		}
		else
		{
			*pD++ = *pS++;
		}
	}

	*pD = '\0';
}
