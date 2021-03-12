#include "dma_arm.h"


//---------------------------------------------------------------------------
ST_DMA Dma ALIGN(4);
u32 DmaDummy ALIGN(4);


//---------------------------------------------------------------------------
EWRAM_CODE void DmaInit(void)
{
	_Memset(&Dma, 0x00, sizeof(ST_DMA));
	DmaDummy = 0;
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaEnqueue(void* src, void* dst, u32 size)
{
	Dma.chain[Dma.reserveNum].src  = (u32*)src;
	Dma.chain[Dma.reserveNum].dst  = (u32*)dst;
	Dma.chain[Dma.reserveNum].mode = (size >> 1) | (DMA_SRC_INC | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | DMA_ENABLE);
	Dma.reserveNum++;

	Dma.reserveNum &= (MAX_DMA_CHAIN_CNT - 1);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaEnqueueFix(void* src, void* dst, u32 size)
{
	Dma.chain[Dma.reserveNum].src  = (u32*)src;
	Dma.chain[Dma.reserveNum].dst  = (u32*)dst;
	Dma.chain[Dma.reserveNum].mode = (size >> 1) | (DMA_SRC_FIXED | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | DMA_ENABLE);
	Dma.reserveNum++;

	Dma.reserveNum &= (MAX_DMA_CHAIN_CNT - 1);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaEnqueueFixClear(void* dst, u32 size)
{
	DmaEnqueueFix(&DmaDummy, dst, size);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaTransfer(void)
{
	while(Dma.chain[Dma.execNum].mode != 0)
	{
		while (REG_DMA3CNT & DMA_ENABLE) {}
		REG_DMA3SAD = (u32)Dma.chain[Dma.execNum].src;
		REG_DMA3DAD = (u32)Dma.chain[Dma.execNum].dst;
		REG_DMA3CNT = (u32)Dma.chain[Dma.execNum].mode;

		Dma.chain[Dma.execNum].mode = 0;
		Dma.execNum++;

		Dma.execNum &= (MAX_DMA_CHAIN_CNT - 1);
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaExec(void* src, void* dst, u32 size)
{
	REG_DMA3SAD = (u32)src;
	REG_DMA3DAD = (u32)dst;
	REG_DMA3CNT = (size >> 1) | (DMA_SRC_INC | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | DMA_ENABLE);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaExecFix(void* src, void* dst, u32 size)
{
	REG_DMA3SAD = (u32)src;
	REG_DMA3DAD = (u32)dst;
	REG_DMA3CNT = (size >> 1) | (DMA_SRC_FIXED | DMA_DST_INC | DMA16 | DMA_IMMEDIATE | DMA_ENABLE);
}
//---------------------------------------------------------------------------
IWRAM_CODE void DmaExecFixClear(void* dst, u32 size)
{
	DmaExecFix(&DmaDummy, dst, size);
}
