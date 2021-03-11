#include "bios_arm.h"


//---------------------------------------------------------------------------
ST_BIOS Bios ALIGN(4);


//---------------------------------------------------------------------------
EWRAM_CODE void BiosInit(void)
{
	_Memset(&Bios, 0x00, sizeof(ST_BIOS));
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExec(void* src, void* dst, u32 size)
{
	if((size & 0x3) == 0)
	{
		CpuSet(src, dst, (size / 4) | CPUSET_32BIT);
	}
	else
	{
		CpuSet(src, dst, (size / 2));
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExecFix(void* src, void* dst, u32 size)
{
	if((size & 0x3) == 0)
	{
		CpuSet(src, dst, (size / 4) | CPUSET_32BIT | CPUSET_SRC_FIX);
	}
	else
	{
		CpuSet(src, dst, (size / 2) | CPUSET_SRC_FIX);
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExecFixClear(void* dst, u32 size)
{
	BiosExecFix(&Bios.dummy, dst, size);
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExecFast(void* src, void* dst, u32 size)
{
	CpuFastSet(src, dst, size / 4);
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExecFastFix(void* src, void* dst, u32 size)
{
	CpuFastSet(src, dst, size / 4 | CPUSET_SRC_FIX);
}
//---------------------------------------------------------------------------
IWRAM_CODE void BiosExecFastFixClear(void* dst, u32 size)
{
	BiosExecFastFix(&Bios.dummy, dst, size);
}
