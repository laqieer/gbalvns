#ifndef __HISTORY_H__
#define __HISTORY_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"
#include "text.h"

//---------------------------------------------------------------------------
#define HISTORY_MAX_CNT					32


enum {
	HISTORY_INIT = 0x01,
	HISTORY_SET_BUF,
	HISTORY_SELECT,
	HISTORY_END_READY,
	HISTORY_END,
};

//---------------------------------------------------------------------------

typedef struct {
	u16 act;
	s16 selNum;
	u16 curNum;									// バッファへの登録数
	u16 reserveNum;								// 書き込み場所（リングバッファ用）
	u16 buf[HISTORY_MAX_CNT][TEXT_SCREEN_CX * TEXT_SCREEN_CY]  ALIGN(4);
} ST_HISTORY;


//---------------------------------------------------------------------------
EWRAM_CODE void HistoryInit(void);
EWRAM_CODE void HistoryInitAct();
EWRAM_CODE void HistoryExec();

EWRAM_CODE u16* HistoryGetBuf(u16 no);
EWRAM_CODE void HistoryAddBuf(u16* pBuf);
EWRAM_CODE void HistorySetBuf(u16 sel);

EWRAM_CODE bool HistoryIsEmpty(void);
EWRAM_CODE bool HistoryIsEnd(void);


#ifdef __cplusplus
}
#endif
#endif

