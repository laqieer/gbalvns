#ifndef __BG_H__
#define __BG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"

//---------------------------------------------------------------------------
#define BG_MAX_CNT				4
#define BG_MAX_MAP_BUF_CNT		(64 * 64)
#define BG_MAX_MAP_BUF_SIZE		(BG_MAX_MAP_BUF_CNT * 2)

#define BG_FONT_SCX				20
#define BG_FONT_SCY				10
#define BG_FONT_DAT_SIZE		128
#define BG_FONT_INVALID_INDEX	5		// エラー時のインデックスコード "・"

//---------------------------------------------------------------------------
typedef struct {
	u16  map;
	u16* pMapBase;
	u16  tile;
	u16* pTileBase;
	u16  pal;
	u16* pPalBase;

	u16  mapBuf[BG_MAX_MAP_BUF_CNT];
	bool isDraw;
} ST_BG;


typedef struct {
	u8 sig[2];
	u8 ver;
	u8 size;
	u8 index;
	u8 reserved[11];
} __PACKED ST_CCT_HEAD;


typedef struct {
	u8 count;
	u8 reserved[3];
} __PACKED ST_CCT_ENTRY;


typedef struct {
	u8  start;
	u8  end;
	u16 offset;
} __PACKED ST_CCT_INFO;


typedef struct {
	u16* pDat;			// フォントデータ
	u8*  pCct;			// フォントシート
} ST_BG_SJIS;


//---------------------------------------------------------------------------
EWRAM_CODE void BgInit(void);
EWRAM_CODE void BgSetDat(u16 bg, u16* pTile, u32 tileSize, u16* pPal, u32 palSize);
EWRAM_CODE void BgSetHofs2(s16 hofs);
EWRAM_CODE void BgSetVofs2(s16 vofs);
IWRAM_CODE void BgExec(void);

IWRAM_CODE void BgDrawChr(u16 bg, u16 x, u16 y, u16 chr);
IWRAM_CODE void BgDrawChr4(u16 bg, u16 x, u16 y, u16 chr);
IWRAM_CODE void BgDrawStr(u16 bg, u16 x, u16 y, char* s);
IWRAM_CODE void BgDrawCap(u16 bg);
IWRAM_CODE void BgDrawCls(u16 bg);


EWRAM_CODE void BgSjisInit(void);
IWRAM_CODE void BgSjisDrawStr(u16 bg, u16 x, u16 y, char* str);
IWRAM_CODE void BgSjisDrawChr(u16 bg, u16 x, u16 y, u16 chr);
IWRAM_CODE u16  BgSjisGetIdx(u16 chr);


#ifdef __cplusplus
}
#endif
#endif
