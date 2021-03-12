#ifndef __SYS_H__
#define __SYS_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
enum {
	SYS_INIT = 0x01,
	SYS_RUN,
};


//---------------------------------------------------------------------------

typedef struct {
	u16 act;
} ST_SYS;


//---------------------------------------------------------------------------
EWRAM_CODE void SysInit(void);

IWRAM_CODE void SysExec(void);
IWRAM_CODE void SysExecInit(void);
IWRAM_CODE void SysExecRun(void);


#ifdef __cplusplus
}
#endif
#endif
