#ifndef __DMA_H__
#define __DMA_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define MAX_DMA_CHAIN_CNT		16			// 


//---------------------------------------------------------------------------
typedef struct {
	u32* src;
	u32* dst;
	u32  mode;
} ST_DMA_CHAIN;


typedef struct {
	u16 reserveNum;
	u16 execNum;

	ST_DMA_CHAIN chain[MAX_DMA_CHAIN_CNT];
} ST_DMA;


//---------------------------------------------------------------------------
EWRAM_CODE void DmaInit(void);

IWRAM_CODE void DmaEnqueue(void* src, void* dst, u32 size);
IWRAM_CODE void DmaEnqueueFix(void* src, void* dst, u32 size);
IWRAM_CODE void DmaEnqueueFixClear(void* dst, u32 size);
IWRAM_CODE void DmaTransfer(void);

IWRAM_CODE void DmaExec(void* src, void* dst, u32 size);
IWRAM_CODE void DmaExecFix(void* src, void* dst, u32 size);
IWRAM_CODE void DmaExecFixClear(void* dst, u32 size);


#ifdef __cplusplus
}
#endif
#endif
