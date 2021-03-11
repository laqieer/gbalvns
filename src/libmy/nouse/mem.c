#include "mem.h"
#include "bios_arm.h"


//---------------------------------------------------------------------------
ST_MEM Mem EWRAM_BSS;


//---------------------------------------------------------------------------
EWRAM_CODE void MemInit(void)
{
	_Memset((u8*)&Mem, 0x00, sizeof(ST_MEM));

	MemFree2((u32)&Mem.ram[0], MEM_SIZE);
}
//---------------------------------------------------------------------------
IWRAM_CODE void* MemMalloc(u32 size)
{
	// サイズバッファ用
	size += 4;

	// u32単位で切り分けます
	size += 3;
	size &= 0xfffffffc;


	ST_MEM* p = &Mem;
	u16 i;

	for(i=0; i<p->cnt; i++)
	{
		if(p->info[i].size < size)
		{
			continue;
		}

		u32* addr = (u32*)p->info[i].addr;

		p->info[i].addr += size;
		p->info[i].size -= size;

		if(p->info[i].size == 0)
		{
			p->cnt--;

			// free[i]がなくなったので前へ詰めます
			while(i < p->cnt)
			{
				p->info[i] = p->info[i + 1];
				i++;
			}
		}

		TRACEOUT("MemAlloc addr=0x%x size=%d\n", addr, size);
		*addr = size;
		return addr + 1;
	}

	SystemError("[Err] MemAlloc size=%d\n", size);
	return NULL;
}
//---------------------------------------------------------------------------
IWRAM_CODE void* MemCalloc(u32 size)
{
	size += 3;
	size &= 0xfffffffc;

	void* p = MemMalloc(size);
	BiosExecFixClear(p, size);
//	_Memset(p, 0x00, size);

	return p;
}
//---------------------------------------------------------------------------
IWRAM_CODE void MemFree(void* addr)
{
	u32* s1 = addr;
	s1--;

	MemFree2((u32)s1, *(u32*)s1);
}
//---------------------------------------------------------------------------
IWRAM_CODE void MemFree2(u32 addr, u32 size)
{
	ST_MEM* p = &Mem;
	u16 i, j;

	TRACEOUT("MemFree addr=0x%x size=%d\n", addr, size);

	// free[]をaddr順に並べる為、free[]の挿入場所を決めます
	for(i=0; i<p->cnt; i++)
	{
		if(p->info[i].addr > addr)
		{
			break;
		}
	}

	// 前の空き領域があるかチェックをします
	if((i > 0) && (p->info[i-1].addr + p->info[i-1].size == addr))
	{
		p->info[i-1].size += size;

		// 後ろの空き領域があるかチェックをします
		if((i < p->cnt) && (addr + size == p->info[i].addr))
		{
			p->info[i-1].size += p->info[i].size;
			p->cnt--;

			// free[i]がなくなったので前へ詰めます
			while(i < p->cnt)
			{
				p->info[i] = p->info[i + 1];
				i++;
			}
		}

		return;
	}

	// 前の空き領域とまとめられなかった場合、後ろがあるかチェックをします
	if((i < p->cnt) && (addr + size == p->info[i].addr))
	{
		p->info[i].addr  = addr;
		p->info[i].size += size;

		return;
	}

	// 前にも後ろの領域にもまとめられない場合、free[i]より後にずらして隙間を作ります
	if(p->cnt < MEM_MAX_INFO_CNT)
	{
		for(j=p->cnt; j>i; j--)
		{
			p->info[j] = p->info[j-1];
		}
		p->cnt++;

		p->info[i].addr = addr;
		p->info[i].size = size;

		return;
	}

	SystemError("MemFree Error\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE u32 MemGetFreeSize(void)
{
	ST_MEM* p = &Mem;

	u32 freeSize = 0;
	u16 i;

	for(i=0; i<p->cnt; i++)
	{
		freeSize += p->info[i].size;
	}

	return freeSize;
}
