#ifndef __SCRIPT2_H__
#define __SCRIPT2_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define SCRIPT_MAX_EVENT_CNT				34


//---------------------------------------------------------------------------

typedef struct {
	u8    op;									// 命令のオペコード
	void  (*pFunc)(void);						// 命令の関数
} ST_SCRIPT_EVENT_TABLE;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEvent(void);

EWRAM_CODE void ScriptExecEventEnd(void);
EWRAM_CODE void ScriptExecEventEffect(void);
EWRAM_CODE void ScriptExecEventJump(void);
EWRAM_CODE void ScriptExecEventSelect(void);
EWRAM_CODE void ScriptExecEventSetPrev(void);
EWRAM_CODE void ScriptExecEventSetBg(void);
EWRAM_CODE void ScriptExecEventSetBgCls(void);
EWRAM_CODE void ScriptExecEventSetBgH(void);
EWRAM_CODE void ScriptExecEventSetChr(void);
EWRAM_CODE void ScriptExecEventSetChr2(void);
EWRAM_CODE void ScriptExecEventSetMark2(void);
EWRAM_CODE void ScriptExecEventSetEffect2(void);
EWRAM_CODE void ScriptExecEventIf(void);
EWRAM_CODE void ScriptExecEventIf2(void);
EWRAM_CODE void ScriptExecEventSetFlag(void);
EWRAM_CODE void ScriptExecEventAddFlag(void);
EWRAM_CODE void ScriptExecEventMsg(void);
EWRAM_CODE void ScriptExecEventSetBgm(void);
EWRAM_CODE void ScriptExecEventEnding(void);
EWRAM_CODE void ScriptExecEventEnding2(void);
EWRAM_CODE void ScriptExecEventEnding3(void);

EWRAM_CODE void ScriptExecEventSkip1(void);
EWRAM_CODE void ScriptExecEventSkip2(void);
EWRAM_CODE void ScriptExecEventSkip3(void);


#ifdef __cplusplus
}
#endif
#endif
