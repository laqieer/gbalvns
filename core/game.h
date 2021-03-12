#ifndef __GAME_H__
#define __GAME_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
enum {
	GAME_INIT  = 0x01,
	GAME_RUN,
};


//---------------------------------------------------------------------------

typedef struct {
	u16 act;
} ST_GAME;


//---------------------------------------------------------------------------
EWRAM_CODE void GameInit(void);
IWRAM_CODE void GameExec(void);
IWRAM_CODE void GameExecInit(void);
IWRAM_CODE void GameExecRun(void);


#ifdef __cplusplus
}
#endif
#endif
