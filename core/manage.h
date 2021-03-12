#ifndef __MANAGE_H__
#define __MANAGE_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------

enum {
	MANAGE_ACT_JINGLE = 0x01,
	MANAGE_ACT_OPENING,
	MANAGE_ACT_TITLE,
	MANAGE_ACT_SCRIPT,
};


//---------------------------------------------------------------------------

typedef struct {
	u16 act;
	u16 actStep;

} ST_MANAGE;


//---------------------------------------------------------------------------
EWRAM_CODE void ManageInit(void);
IWRAM_CODE void ManageSetAct(u16 act);

IWRAM_CODE void ManageExec(void);
IWRAM_CODE void ManageExecJingle(void);
IWRAM_CODE void ManageExecOpening(void);
IWRAM_CODE void ManageExecTitle(void);
IWRAM_CODE void ManageExecScript(void);


#ifdef __cplusplus
}
#endif
#endif
