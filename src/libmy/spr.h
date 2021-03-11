#ifndef __SPR_H__
#define __SPR_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "../libgba/gba.h"

//---------------------------------------------------------------------------
#define MAX_SPRITE_CNT					10
#define MAX_SPRITE_DAT_SIZE				(((256/2) * 128) - ((16/2) * 16))			// スプライト全体バッファ - カーソルサイズ


//---------------------------------------------------------------------------

typedef struct {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	u16 attr3;
} ST_SPRITE;

typedef struct {
	bool isChrDraw;
	bool isDatDraw;

	ST_SPRITE chr[MAX_SPRITE_CNT] ALIGN(4);
	u8 dat[MAX_SPRITE_DAT_SIZE] ALIGN(4);
} ST_SPR_BUF;


//---------------------------------------------------------------------------
EWRAM_CODE void SprInit(void);
EWRAM_CODE void SprSetDat(u16* pTile, u32 tileSize, u16* pPal, u32 palSize);
EWRAM_CODE void SprSetChr(u16 num, u16 x, u16 y, u16 tile, u16 shape, u16 size);
EWRAM_CODE void SprSetMove(u16 num, u16 x, u16 y);
EWRAM_CODE void SprSetClearDat(void);
EWRAM_CODE void SprSetEnable(u16 num);
EWRAM_CODE void SprSetDisable(u16 num);
IWRAM_CODE u16* SprGetDatBuf(void);


IWRAM_CODE void SprDraw(void);


#ifdef __cplusplus
}
#endif
#endif
