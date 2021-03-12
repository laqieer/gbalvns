#include "mode3.h"
#include "bios_arm.h"
#include "../res.h"

//---------------------------------------------------------------------------
ST_MODE3 Mode3 EWRAM_BSS;


//---------------------------------------------------------------------------
EWRAM_CODE void Mode3Init(void)
{
	BiosExecFixClear(&Mode3, sizeof(ST_MODE3));


	Mode3.pDat  = (u16*)&mplus_s10dBitmap;
	Mode3.imgCx = MPLUS_S10D_IMG_CX;
	Mode3.imgCy = MPLUS_S10D_IMG_CY;
	Mode3.scnCx = SCREEN_WIDTH  / MPLUS_S10D_IMG_CX;
	Mode3.scnCy = SCREEN_HEIGHT / MPLUS_S10D_IMG_CY;

	REG_DISPCNT = MODE_3 | BG2_ON;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3Draw(void)
{
	if(Mode3.isDraw == TRUE)
	{
		BiosExecFast(Mode3.buf, (void*)VRAM, MODE3_MAX_BUF_SIZE);
		Mode3.isDraw = FALSE;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawClear(void)
{
	Mode3DrawFullFill(RGB5(0, 0, 0));
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawFull(u16* pBuf)
{
	BiosExecFast(pBuf, Mode3.buf, MODE3_MAX_BUF_SIZE);

	Mode3.curX   = 0;
	Mode3.curY   = 0;
	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawFullLz77(u16* pBuf)
{
	LZ77UnCompWram(pBuf, Mode3.buf);

	Mode3.curX   = 0;
	Mode3.curY   = 0;
	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawFullFill(u16 col)
{
	u32 c =((u32)col << 16) | (u32)col;

	BiosExecFastFix(&c, Mode3.buf, MODE3_MAX_BUF_SIZE);

	Mode3.curX   = 0;
	Mode3.curY   = 0;
	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawPset(u16 x, u16 y, u16 col)
{
	if(x  >= SCREEN_WIDTH)  return;
	if(y  >= SCREEN_HEIGHT) return;

	u16* pScn = (u16*)Mode3.buf + x + (y * SCREEN_WIDTH);
	*pScn = col;

	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawPsetVram(u16 x, u16 y, u16 col)
{
	// バッファとVRAMに直接書き込みます（チラツキあり）
	if(x  >= SCREEN_WIDTH)  return;
	if(y  >= SCREEN_HEIGHT) return;

	u16* pScn  = (u16*)Mode3.buf + x + (y * SCREEN_WIDTH);
	u16* pVram = (u16*)VRAM + x + (y * SCREEN_WIDTH);

	*pScn = col;
	*pVram = col;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 col)
{
	s16 dx, dy;
	s16 sx, sy;
	s16 x, y;
	s16 E;
	s16 i;

	sx = ( x2 > x1 ) ? 1 : -1;
	dx = ( x2 > x1 ) ? x2 - x1 : x1 - x2;
	sy = ( y2 > y1 ) ? 1 : -1;
	dy = ( y2 > y1 ) ? y2 - y1 : y1 - y2;

	x = x1;
	y = y1;

	if(dx >= dy)
	{
		// 傾きが1以下の場合
		E = -dx;

		for(i=0; i<=dx; i++)
		{
			Mode3DrawPset(x, y, col);

			x += sx;
			E += 2 * dy;

			if(E >= 0)
			{
				y += sy;
				E -= 2 * dx;
			}
		}
	}
	else
	{
		// 傾きが1より大きい場合
		E = -dy;
		
		for(i=0; i<=dy; i++)
		{
			Mode3DrawPset(x, y, col);

			y += sy;
			E += 2 * dx;

			if(E >= 0)
			{
				x += sx;
				E -= 2 * dy;
			}
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawCircle(u16 sx, u16 sy, u16 rad, u16 col)
{
	u16 x = rad;
	u16 y = 0;
	s16 p = 3 - 2 * rad;

	while(x >= y)
	{
		Mode3DrawPset(sx + x, sy + y, col);
		Mode3DrawPset(sx - x, sy + y, col);
		Mode3DrawPset(sx + x, sy - y, col);
		Mode3DrawPset(sx - x, sy - y, col);
		Mode3DrawPset(sx + y, sy + x, col);
		Mode3DrawPset(sx - y, sy + x, col);
		Mode3DrawPset(sx + y, sy - x, col);
		Mode3DrawPset(sx - y, sy - x, col);

		if(p >= 0)
		{
			x--;
			p -= 4 * x;
		}

		y++;
		p += 4 * y + 2;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawImg(u16 sx, u16 sy, u16 cx, u16 cy, u16* img)
{
	u16* p = (u16*)Mode3.buf + (sy * SCREEN_WIDTH) + sx;

	u16 x, y;

	for(y=0; y<cy; y++)
	{
		for(x=0; x<cx; x++)
		{
			p[y*SCREEN_WIDTH + x] = img[y*cx + x];
		}
	}

	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawImgMask(u16 sx, u16 sy, u16 cx, u16 cy, u16* img)
{
	u16* p = (u16*)Mode3.buf + (sy * SCREEN_WIDTH) + sx;

	u16 x, y;

	for(y=0; y<cy; y++)
	{
		for(x=0; x<cx; x++)
		{
			if(img[y*cx + x] == 0x0000)
			{
				continue;
			}

			p[y*SCREEN_WIDTH + x] = img[y*cx + x];
		}
	}

	Mode3.isDraw = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawStr(char* str)
{
	s16 len = _Strlen(str);
	s16 i;

	for(i=0; i<len; i++)
	{
		Mode3DrawChr(str[i]);
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawStrLf(char* str)
{
	Mode3DrawStr(str);

	Mode3.curX = 0;
	Mode3.curY++;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawChr(char chr)
{
	ST_MODE3* p = &Mode3;

	if(p->curX >= p->scnCx)
	{
		p->curX = 0;
		p->curY++;
	}

	if(p->curY >= p->scnCy)
	{
		p->curX = 0;
		p->curY = 0;
	}

	_ASSERT(chr >= 0x20 && chr <= 0x7e && "chr idx error");

	u16* pFont = p->pDat + (p->imgCx * p->imgCy) * (chr - 0x20);
	s16  x, y;

	for(y=0; y<p->imgCy; y++)
	{
		for(x=0; x<p->imgCx; x++)
		{
			Mode3DrawPset((p->curX * p->imgCx) + x, (p->curY * p->imgCy) + y, *pFont++);
		}
	}

	p->curX++;
}
//---------------------------------------------------------------------------
IWRAM_CODE void Mode3DrawNum(s16 num)
{
	char buf[80];

	_Sprintf(buf, "%d", num);
	Mode3DrawStr(buf);
}
//---------------------------------------------------------------------------
EWRAM_CODE void Mode3SetLocate(u16 x, u16 y)
{
	Mode3.curX = x;
	Mode3.curY = y;
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 Mode3GetScnCy(void)
{
	return Mode3.scnCy;
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 Mode3GetImgCy(void)
{
	return Mode3.imgCy;
}
