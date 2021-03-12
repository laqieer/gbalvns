// 「30日でできる！ＯＳ自作入門」のソースコードを使わせて頂いています
// ライセンスは「川合堂ライセンス-01」です。http://hrb.osask.jp/
#ifndef __MEM_H__
#define __MEM_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define MEM_MAX_INFO_CNT			2048
#define MEM_SIZE					(100000+64)		// u32単位

//---------------------------------------------------------------------------
typedef struct {
	u32 addr;
	u32 size;
} ST_MEM_INFO;

typedef struct {
	u8  ram[MEM_SIZE] ALIGN(4);
	u32 cnt;
	ST_MEM_INFO info[MEM_MAX_INFO_CNT] ALIGN(4);
} ST_MEM;


//---------------------------------------------------------------------------
EWRAM_CODE void  MemInit(void);

IWRAM_CODE void* MemMalloc(u32 size);
IWRAM_CODE void* MemCalloc(u32 size);
IWRAM_CODE void  MemFree(void* addr);
IWRAM_CODE void  MemFree2(u32 addr, u32 size);
EWRAM_CODE u32   MemGetFreeSize(void);


#ifdef __cplusplus
}
#endif
#endif

