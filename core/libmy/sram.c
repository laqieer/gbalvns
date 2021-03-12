#include "sram.h"


//---------------------------------------------------------------------------
ST_SRAM Sram;


//---------------------------------------------------------------------------
EWRAM_CODE void SramInit(void)
{
	_Memset((u8*)&Sram, 0x00, sizeof(ST_SRAM));
	Sram.p = (u8*)SRAM;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramSeek(u32 offset)
{
	Sram.p = (u8*)SRAM + offset;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8* SramGetPointer(void)
{
	return Sram.p;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 SramRead8(void)
{
	return (u8)*Sram.p++;
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 SramRead16(void)
{
	u16 ret = 0;

	ret |= (u16)*Sram.p++;
	ret |= (u16)*Sram.p++ << 8;

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE u32 SramRead32(void)
{
	u16 ret = 0;

	ret |= (u32)*Sram.p++;
	ret |= (u32)*Sram.p++ <<  8;
	ret |= (u32)*Sram.p++ << 16;
	ret |= (u32)*Sram.p++ << 24;

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramReadCopy(u8* p, u32 size)
{
	u32 i;

	for(i=0; i<size; i++)
	{
		*p++ = *Sram.p++;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramWrite8(u8 data)
{
	*Sram.p++ = (u8)data;
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramWrite16(u16 data)
{
	*Sram.p++ = (u8)((data   ) & 0x00ff);
	*Sram.p++ = (u8)((data>>8) & 0x00ff);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramWrite32(u32 data)
{
	*Sram.p++ = (u8)((data    ) & 0x000000ff);
	*Sram.p++ = (u8)((data>> 8) & 0x000000ff);
	*Sram.p++ = (u8)((data>>16) & 0x000000ff);
	*Sram.p++ = (u8)((data>>24) & 0x000000ff);
}
//---------------------------------------------------------------------------
EWRAM_CODE void SramWriteCopy(u8* p, u32 size)
{
	u32 i;

	for(i=0; i<size; i++)
	{
		*Sram.p++ = *p++;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE bool SramIsStr(char* str)
{
	u16 len = _Strlen(str);
	s16 i;

	for(i=0; i<len; i++)
	{
		if(str[i] != *Sram.p++)
		{
			return FALSE;
		}
	}

	return TRUE;
}
