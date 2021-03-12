#ifndef __AD_H__
#define __AD_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define AD_BUF_SIZE					224						// 18157Hz = 304  13379Hz = 224
#define AD_SAMPLE_TIME				(280896 / AD_BUF_SIZE)


enum {
	AD_ACT_STOP,
	AD_ACT_READY,
	AD_ACT_PLAY,
};

//---------------------------------------------------------------------------
typedef struct {
	u16  act;
	bool isLoop;

	u8*  pCur;
	u8*  pTop;
	u8*  pEnd;

	s32  lastSample;
	s32  lastIdx;

	u16  bufIdx;
	s8   buf[2][AD_BUF_SIZE] ALIGN(4);
} ST_AD;


//---------------------------------------------------------------------------
EWRAM_CODE void AdInit(void);

EWRAM_CODE void AdSetData(u8* pData, u32 size, bool isLoop);
EWRAM_CODE void AdSetData2(char* pStr, bool isLoop);
EWRAM_CODE void AdReset(void);
EWRAM_CODE void AdPlay(void);
EWRAM_CODE void AdStop(void);
EWRAM_CODE void AdStopOut(void);

IWRAM_CODE void AdIntrVcount(void);
IWRAM_CODE void AdIntrVblank(void);
IWRAM_CODE void AdIntrNextBuf(void);


#ifdef __cplusplus
}
#endif
#endif
