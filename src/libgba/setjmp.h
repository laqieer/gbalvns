#ifndef __SETJMP_H__
#define __SETJMP_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gba.h"


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Setjmp, Longjmp

typedef struct {
    s32 reg[10];
} ST_JMP_BUF;



/*

	if(Setjmp(&JmpBuf) == 0)
    {
		TRACEOUT("A\n");
		Longjmp(&JmpBuf, 1);
	}
	else
	{
		// longjmp‚Å–ß‚è‚Ü‚·
		TRACEOUT("B\n");
	}

	TRACEOUT("C\n");
*/

//---------------------------------------------------------------------------
IWRAM_CODE s32  Setjmp(ST_JMP_BUF* buf);
IWRAM_CODE void Longjmp(ST_JMP_BUF* buf, s32 ret);


#ifdef __cplusplus
}
#endif
#endif
