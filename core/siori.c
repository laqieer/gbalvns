#include "siori.h"
#include "libmy/sram.h"
#include "script.h"
#include "bgm.h"
#include "img.h"

#include "text.h"
#include "anime.h"
#include "history.h"


// SRAM内部構造

// 0x00 - 	"SZ"（2バイト） + バージョン（2バイト） + しおりサイズ（2バイト）
// 0x10 - 	共通フラグ（4バイト）
// 0x20 - 	しおり（計9個）

//---------------------------------------------------------------------------
// 例外参照
extern ST_SCRIPT Script;
extern ST_BGM Bgm;
extern ST_IMG Img;


//---------------------------------------------------------------------------
ST_SIORI Siori;


//---------------------------------------------------------------------------
EWRAM_CODE void SioriInit(void)
{
	_Memset(&Siori, 0x00, sizeof(ST_SIORI));

	Siori.size = 2 + TEXT_SIORI_CX * 2 + sizeof(ST_SCRIPT) + sizeof(ST_BGM) + sizeof(ST_IMG);

	TRACEOUT("[しおりサイズ: 0x%x]\n", Siori.size);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriSaveSig(void)
{
	TRACEOUT("[シグネチャ書き込み]\n");

	SramSeek(0);
	SramWrite8('S');
	SramWrite8('Z');
	SramWrite16(100);
	SramWrite16(Siori.size);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriSaveFlag(void)
{
	TRACEOUT("[フラグ書き込み]\n");

	SramSeek(0x10);

	u8* p = (u8*)&Script.flag;

	SramWrite8(*(p+0));
	SramWrite8(*(p+1));
	SramWrite8(*(p+7));
	SramWrite8(*(p+8));
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriLoadFlag(void)
{
	if(SioriIsSram() == FALSE)
	{
		SioriSaveSig();
		SioriSaveFlag();

		TRACEOUT("[初回起動]\n");
		return;
	}


	TRACEOUT("[フラグ読み込み]\n");

	SramSeek(0x10);

	u8* pS = SramGetPointer();
	u8* pD = (u8*)&Script.flag;

	pD[0] = *pS++;
	pD[1] = *pS++;
	pD[7] = *pS++;
	pD[8] = *pS++;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriSave(u16 no)
{
	_ASSERT(no < SIORI_MAX_CNT);


	TRACEOUT("[しおり　セーブ: %d]\n", no);

	u16  i;
	u8*  p;

	SioriSaveSig();
	SioriSaveFlag();


	SramSeek(0x20 + Siori.size * no);
	SramWrite8('S');
	SramWrite8('V');

	p = (u8*)TextGetSioriStr();
	for(i=0; i<TEXT_SIORI_CX; i++)
	{
		SramWrite8(*p++);
		SramWrite8(*p++);
	}

	p = (u8*)&Script;
	for(i=0; i<sizeof(ST_SCRIPT); i++)
	{
		SramWrite8(*p++);
	}

	p = (u8*)&Bgm;
	for(i=0; i<sizeof(ST_BGM); i++)
	{
		SramWrite8(*p++);
	}

	p = (u8*)&Img;
	for(i=0; i<sizeof(ST_IMG); i++)
	{
		SramWrite8(*p++);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE bool SioriLoad(u16 no)
{
	TRACEOUT("[しおり　ロード: %d]\n", no);

	if(SioriIsSram() == FALSE)
	{
		return FALSE;
	}


	SramSeek(0x20 + Siori.size * no);
	u8* pS = SramGetPointer();
	u8* pD;
	u16 i;

	if(*(pS+0) != 'S' || *(pS+1) != 'V')
	{
		return FALSE;
	}
	pS += 2 + (TEXT_SIORI_CX * 2);


	pD = (u8*)&Script;
	for(i=0; i<sizeof(ST_SCRIPT); i++)
	{
		*pD++ = *pS++;
	}

	pD = (u8*)&Bgm;
	for(i=0; i<sizeof(ST_BGM); i++)
	{
		*pD++ = *pS++;
	}

	pD = (u8*)&Img;
	for(i=0; i<sizeof(ST_IMG); i++)
	{
		*pD++ = *pS++;
	}


	// 復帰処理をします

	// 共有のフラグ
	SioriLoadFlag();

	// スクリプト
	if(Script.act == SCRIPT_ACT_EVENT || Script.actTmp == SCRIPT_ACT_SELECT)
	{
		Script.act = SCRIPT_ACT_EVENT;
		Script.pEventCur = Script.pEventOld;
	}
	else
	{
		Script.act = SCRIPT_ACT_MSG;
		Script.pMsgCur = Script.pMsgOld;
	}

	// BGM
	BgmPlayRestart();

	// 画面
	TextSetClearNoBuf();
	ImgSetEffectClear(IMG_EFFECT_FADE_BLACK);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_BLACK);

	HistoryInit();
	TextInit();

	return TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriSavePrev(void)
{
	SioriSave(SIORI_MAX_CNT - 1);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SioriLoadPrev(void)
{
	SioriLoad(SIORI_MAX_CNT - 1);
}
//---------------------------------------------------------------------------
EWRAM_CODE char* SioriGetName(u16 no)
{
	SramSeek(0x20 + Siori.size * no);
	u8* p = SramGetPointer();

	if(*(p+0) != 'S' || *(p+1) != 'V')
	{
		return BLANK_SIORI_NAME;
	}

	return (char*)p + 2;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool SioriIsSram(void)
{
	SramSeek(0);

	if(SramRead8() != 'S') return FALSE;
	if(SramRead8() != 'Z') return FALSE;

	return TRUE;
}
