#ifndef __DEBUG_H__
#define __DEBUG_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "../libgba/gba.h"

//---------------------------------------------------------------------------
#define DEBUG_FPS_CNT			60


//---------------------------------------------------------------------------
typedef struct {
	s16 fpsTimer;
	s16 fpsCnt;
	s16 gameTime;
	s16 gameFrame;
	s16 frameCnt;
	u16 fps;
	u16 delay;
} ST_DEBUG;


//---------------------------------------------------------------------------
EWRAM_CODE void DebugInit(void);
IWRAM_CODE void DebugExec(void);
IWRAM_CODE void DebugDraw(void);
IWRAM_CODE void DebugIntr(void);


#ifdef __cplusplus
}
#endif
#endif
