#include "bg.h"
#include "dma_arm.h"


// bg0 ASCII（SYSTEM 用）
// bg1 ASCII
// bg2 ステージ
// bg3 None

//---------------------------------------------------------------------------
ST_BG Bg[BG_MAX_CNT] EWRAM_BSS;
ST_BG_SJIS BgSjis;


//---------------------------------------------------------------------------
EWRAM_CODE void BgInit(void)
{
	_Memset((u8*)&Bg, 0x00, sizeof(ST_BG) * BG_MAX_CNT);

	BgSjisInit();

	// TODO 変更時にmemcpyが付加されていないかチェックをすること
	const u16 map[]  = { 20, 24, 28,  0 };
	const u16 tile[] = {  0,  0,  1,  3 };
	const u16 pal[]  = {  1,  1,  3,  4 };

	u32 i;

	for(i=0; i<BG_MAX_CNT; i++)
	{
		Bg[i].map       = MAP_BASE(map[i]);
		Bg[i].pMapBase  = MAP_BASE_ADR(map[i]);
		Bg[i].tile      = TILE_BASE(tile[i]);
		Bg[i].pTileBase = TILE_BASE_ADR(tile[i]);
		Bg[i].pal       = pal[i];
		Bg[i].pPalBase  = &BG_PALETTE[(pal[i] * 16)];
	}

	for(i=0; i<BG_MAX_CNT; i++)
	{
		DmaExecFixClear(Bg[i].pTileBase, 0x2000 * 2);
		DmaExecFixClear(Bg[i].pMapBase, 64*64 * 2);
	}

	REG_DISPCNT = (MODE_0 | BG0_ON | BG1_ON | BG2_ON);
	REG_BG0CNT  = (BG_SIZE_3 | BG_16_COLOR | Bg[0].tile | Bg[0].map | 0);
	REG_BG1CNT  = (BG_SIZE_3 | BG_16_COLOR | Bg[1].tile | Bg[1].map | 0);
	REG_BG2CNT  = (BG_SIZE_3 | BG_16_COLOR | Bg[2].tile | Bg[2].map | 0);
//	REG_BG3CNT  = (BG_SIZE_3 | BG_16_COLOR | Bg[3].tile | Bg[3].map | 3 | BG_WRAP);
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgSetDat(u16 bg, u16* pTile, u32 tileSize, u16* pPal, u32 palSize)
{
	DmaEnqueue(pTile, Bg[bg].pTileBase, tileSize);
	DmaEnqueue(pPal, Bg[bg].pPalBase, palSize);
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgSetHofs2(s16 hofs)
{
	REG_BG2HOFS = hofs;
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgSetVofs2(s16 vofs)
{
	REG_BG2VOFS = vofs;
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgExec(void)
{
	u16 i;

	for(i=0; i<BG_MAX_CNT; i++)
	{
		if(Bg[i].isDraw == TRUE)
		{
			DmaEnqueue(Bg[i].mapBuf, Bg[i].pMapBase, BG_MAX_MAP_BUF_SIZE);
			Bg[i].isDraw = FALSE;
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgDrawChr(u16 bg, u16 x, u16 y, u16 chr)
{
	u16 pal = (Bg[bg].pal << 12);

	if(x < 32)
	{
		Bg[bg].mapBuf[x + y * 32] = chr | pal;
	}
	else
	{
		// 512x512（64x64タイル）専用
		Bg[bg].mapBuf[1024 + (x-32) + (y*32)] = chr | pal;
	}

	Bg[bg].isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgDrawChr4(u16 bg, u16 x, u16 y, u16 chr)
{
	u16 oX = x * 4;
	u16 oY = y * 4;
	u16 i, j;

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			BgDrawChr(bg, oX + j, oY + i, chr + j + (i * 32));
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgDrawStr(u16 bg, u16 x, u16 y, char* s)
{
	while(*s != NULL)
	{
		BgDrawChr(bg, x++, y, *s++ - ' ');
	}

	Bg[bg].isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgDrawCap(u16 bg)
{
	u16 cnt = 0;
	u16 x, y;

	for(y=0; y<20; y++)
	{
		for(x=0; x<30; x++)
		{
			BgDrawChr(bg, x, y, cnt++);
		}
	}

	Bg[bg].isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgDrawCls(u16 bg)
{
	u16 x, y;

	for(y=0; y<64; y++)
	{
		for(x=0; x<64; x++)
		{
			BgDrawChr(bg, x, y, 0);
		}
	}

	Bg[bg].isDraw = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void BgSjisInit(void)
{
	_Memset((u8*)&BgSjis, 0x00, sizeof(ST_BG_SJIS));

	// TODO BgShisSetDat()

//	BgSjis.pDat = (u16*)&font_mikaTiles;
//	BgSjis.pCct = (u8*)&cct_mika_bin + sizeof(ST_CCT_HEAD);
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgSjisDrawStr(u16 bg, u16 x, u16 y, char* str)
{
	u16 i = 0;
	u16 chr;

	for(;;)
	{
		chr = str[i++];

		if(chr == '\0' || chr == CR || chr == LF)
		{
			Bg[bg].isDraw = TRUE;
			return;
		}

		if(x >= BG_FONT_SCX)
		{
			x = 0;
			y++;

			if(y >= BG_FONT_SCY)
			{
				y = 0;
			}
		}

		if(_IsDigit(chr) == TRUE)
		{
			chr = 0x824f + chr - '0';
		}
		else
		{
			chr = (chr << 8) | str[i++];
		}

		BgSjisDrawChr(bg, x, y, chr);
		x++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void BgSjisDrawChr(u16 bg, u16 x, u16 y, u16 chr)
{
	u16 idx = BgSjisGetIdx(chr);
	u16 i;

	u16* s1 = BgSjis.pDat + idx * BG_FONT_DAT_SIZE / 2;
	u16* s2 = s1 + 32 / 2;
	u16* s3 = s2 + 32 / 2;
	u16* s4 = s3 + 32 / 2;

	if(x & 0x1)
	{
		u16* d1 = Bg[bg].mapBuf + y*1024 + (((((x+1)/2)*3)-2) * 32 / 2) + 1;
		u16* d2 = d1 + 15;
		u16* d3 = d1 + 512;
		u16* d4 = d2 + 512;

		for(i=0; i<8; i++)
		{
			*d1++ = *s1++;
			d1++;

			*d2++ = *s1++;
			*d2++ = *s2++;
			s2++;
		}

		for(i=0; i<8; i++)
		{
			*d3++ = *s3++;
			d3++;

			*d4++ = *s3++;
			*d4++ = *s4++;
			s4++;
		}
	}
	else
	{
		u16* d1 = Bg[bg].mapBuf + y*1024 + ((x/2)*3) * 32 / 2;
		u16* d2 = d1 + 32 / 2;
		u16* d3 = d1 + 512;
		u16* d4 = d3 + 32 / 2;

		for(i=0; i<8; i++)
		{
			*d1++ = *s1++;
			*d1++ = *s1++;

			*d2++ = *s2++;
			d2++;
			s2++;
		}

		for(i=0; i<8; i++)
		{
			*d3++ = *s3++;
			*d3++ = *s3++;

			*d4++ = *s4++;
			d4++;
			s4++;
		}
	}

	Bg[bg].isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE u16 BgSjisGetIdx(u16 code)
{
	if(_IsSJIS(HIBYTE(code)) == FALSE)
	{
		return BG_FONT_INVALID_INDEX;
	}


	// level 1 ---------------------------------
	u16 c0 = HIBYTE(code) >> 5;
	u16 c1 = HIBYTE(code) & 0x1f;
	u16 i1;

	if(c0 == 4)
	{
		// 80-9F
		i1 = ((u16*)BgSjis.pCct)[c1];
	}
	else
	{
		// E0-FF
		i1 = ((u16*)BgSjis.pCct)[c1 + 32];
	}

	if(i1 == 0)
	{
		return BG_FONT_INVALID_INDEX;
	}


	// level 2 ---------------------------------
	u16 c2 = LOBYTE(code) >> 6;
	u16 i2 = ((u16*)(BgSjis.pCct + i1))[c2];

	if(i2 == 0)
	{
		return BG_FONT_INVALID_INDEX;
	}


	// level 3 ---------------------------------
	ST_CCT_ENTRY* pXcctEntry = (ST_CCT_ENTRY*)(BgSjis.pCct + i2);
	ST_CCT_INFO*  pXcctInfo  = (ST_CCT_INFO*)(BgSjis.pCct + i2 + sizeof(ST_CCT_ENTRY));

	u16 c3 = LOBYTE(code) & 0x3f;
	u16 i;

	for(i=0; i<pXcctEntry->count; i++)
	{
		if(c3 >= pXcctInfo->start && c3 <= pXcctInfo->end)
		{
			return pXcctInfo->offset + (c3 - pXcctInfo->start);
		}

		pXcctInfo++;
	}

	return BG_FONT_INVALID_INDEX;
}
