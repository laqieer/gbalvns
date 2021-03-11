#ifndef __FADE_H__
#define __FADE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
typedef struct {
	bool isEnd;
	bool isClear;
	s16  cnt;
	s16  min;
	s16  max;
	s16  wait;
	s16  waitCnt;
} ST_FADE;


//---------------------------------------------------------------------------
EWRAM_CODE void FadeInit(void);

EWRAM_CODE void FadeSetDefault(u16 mode);
EWRAM_CODE void FadeSetDrakIn(u16 wait);
EWRAM_CODE void FadeSetDrakIn2(u16 wait, u16 max);
EWRAM_CODE void FadeSetDrakIn3(u16 cnt);
EWRAM_CODE void FadeSetDrakOut(u16 wait);
EWRAM_CODE void FadeSetDrakOut2(u16 wait, u16 cnt);
EWRAM_CODE void FadeSetLightIn(u16 wait);
EWRAM_CODE void FadeSetLightOut(u16 wait);
EWRAM_CODE void FadeSetClear(void);
EWRAM_CODE void FadeSetDirect(u16 num);

EWRAM_CODE void FadeExecIn(void);
EWRAM_CODE void FadeExecOut(void);
EWRAM_CODE void FadeDraw(void);

EWRAM_CODE bool FadeIsEnd(void);


#ifdef __cplusplus
}
#endif
#endif
