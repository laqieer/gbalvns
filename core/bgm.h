#ifndef __BGM_H__
#define __BGM_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define BGM_MAX_SND_CNT						26
#define BGM_SND_STR							"tr_%03d.8ad"


//---------------------------------------------------------------------------

typedef struct {
	bool isLoop;
	u16  no;

	bool isNext;
	bool isNextLoop;
	u16  nextNo;

} ST_BGM;




//---------------------------------------------------------------------------
EWRAM_CODE void BgmInit(void);
EWRAM_CODE void BgmPlay(u16 no, bool isLoop);
EWRAM_CODE void BgmPlayRestart(void);
EWRAM_CODE void BgmPlayNext(void);
EWRAM_CODE void BgmStop(void);
EWRAM_CODE void BgmStopOut(void);

EWRAM_CODE void BgmSetNext(u16 no, bool isLoop);


#ifdef __cplusplus
}
#endif
#endif
