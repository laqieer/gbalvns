#ifndef __MODE3_H__
#define __MODE3_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "../libgba/gba.h"

//---------------------------------------------------------------------------
#define MODE3_MAX_BUF_SIZE			(SCREEN_WIDTH * SCREEN_HEIGHT * 2)


//---------------------------------------------------------------------------
typedef struct {
	bool isDraw;
	u8   buf[MODE3_MAX_BUF_SIZE] ALIGN(4);

	// Cursor
	s16  curX;
	s16  curY;

	// Ascii
	u16* pDat;
	u16  imgCx;			// フォントのサイズ
	u16  imgCy;
	u16  scnCx;			// フォントを登録できる数（スクリーン上）
	u16  scnCy;

} ST_MODE3;


//---------------------------------------------------------------------------
EWRAM_CODE void Mode3Init(void);

IWRAM_CODE void Mode3Draw(void);
IWRAM_CODE void Mode3DrawClear(void);
IWRAM_CODE void Mode3DrawFull(u16* pBuf);
IWRAM_CODE void Mode3DrawFullLz77(u16* pBuf);
IWRAM_CODE void Mode3DrawFullFill(u16 col);
IWRAM_CODE void Mode3DrawPset(u16 x, u16 y, u16 col);
IWRAM_CODE void Mode3DrawPsetVram(u16 x, u16 y, u16 col);
IWRAM_CODE void Mode3DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 col);
IWRAM_CODE void Mode3DrawCircle(u16 sx, u16 sy, u16 rad, u16 col);
IWRAM_CODE void Mode3DrawImg(u16 sx, u16 sy, u16 cx, u16 cy, u16* img);
IWRAM_CODE void Mode3DrawImgMask(u16 sx, u16 sy, u16 cx, u16 cy, u16* img);
IWRAM_CODE void Mode3DrawStr(char* str);
IWRAM_CODE void Mode3DrawStrLf(char* str);
IWRAM_CODE void Mode3DrawChr(char chr);
IWRAM_CODE void Mode3DrawNum(s16 num);

EWRAM_CODE void Mode3SetLocate(u16 x, u16 y);
EWRAM_CODE u16  Mode3GetScnCy(void);
EWRAM_CODE u16  Mode3GetImgCy(void);


#ifdef __cplusplus
}
#endif
#endif
