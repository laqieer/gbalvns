#include "spr.h"
#include "bios_arm.h"


//---------------------------------------------------------------------------
ST_SPR_BUF SprBuf;


//---------------------------------------------------------------------------
EWRAM_CODE void SprInit(void)
{
	BiosExecFixClear(&SprBuf, sizeof(ST_SPR_BUF));
	BiosExecFixClear(OAM, sizeof(ST_SPRITE) * MAX_SPRITE_CNT);

	REG_DISPCNT |= (OBJ_ON | OBJ_2D_MAP);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetDat(u16* pTile, u32 tileSize, u16* pPal, u32 palSize)
{
	// BITMAP_OBJ_BASE_ADR
	// OBJ_BASE_ADR

	BiosExec(pTile, BITMAP_OBJ_BASE_ADR, tileSize);
	BiosExec(pPal, OBJ_COLORS, palSize);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetChr(u16 num, u16 x, u16 y, u16 tile, u16 shape, u16 size)
{
	ST_SPRITE* pChr = (ST_SPRITE*)SprBuf.chr + num;

	pChr->attr0 = (y & 0x00ff) | ATTR0_COLOR_16 | ATTR0_DISABLED | (shape);
	pChr->attr1 = (x & 0x01ff) | (size);
	pChr->attr2 = (tile) | ATTR2_PRIORITY(0);

	SprBuf.isChrDraw = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetMove(u16 num, u16 x, u16 y)
{
	ST_SPRITE* pChr = (ST_SPRITE*)SprBuf.chr + num;

	pChr->attr0 &= 0xff00;
	pChr->attr1 &= 0xfe00;
	pChr->attr0 |= (y & 0x00ff);
	pChr->attr1 |= (x & 0x01ff);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetClearDat(void)
{
	BiosExecFixClear(SprBuf.dat, MAX_SPRITE_DAT_SIZE);
	SprBuf.isDatDraw = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetEnable(u16 num)
{
	ST_SPRITE* pChr = (ST_SPRITE*)SprBuf.chr + num;
	pChr->attr0 ^= ATTR0_DISABLED;

	SprBuf.isChrDraw = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SprSetDisable(u16 num)
{
	ST_SPRITE* pChr = (ST_SPRITE*)SprBuf.chr + num;
	pChr->attr0 |= ATTR0_DISABLED;

	SprBuf.isChrDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE u16* SprGetDatBuf(void)
{
	SprBuf.isDatDraw = TRUE;
	return (u16*)SprBuf.dat;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SprDraw(void)
{
	if(SprBuf.isChrDraw == TRUE)
	{
		BiosExec(SprBuf.chr, OAM, sizeof(ST_SPRITE) * MAX_SPRITE_CNT);
		SprBuf.isChrDraw = FALSE;
	}

	if(SprBuf.isDatDraw == TRUE)
	{
		BiosExec(SprBuf.dat, BITMAP_OBJ_BASE_ADR, MAX_SPRITE_DAT_SIZE);
		SprBuf.isDatDraw = FALSE;
	}
}
