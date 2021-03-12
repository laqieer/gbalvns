#include "snd.h"


// Timer0 BGM
// Timer1 SE
// DMA1   BGM
// DMA2   SE

//---------------------------------------------------------------------------
ST_SND Snd[2];


//---------------------------------------------------------------------------
EWRAM_CODE void SndInit(void)
{
	_Memset(&Snd, 0x00, sizeof(ST_SND) * 2);

	REG_SOUNDCNT_X = SNDSTAT_ENABLE;
	REG_SOUNDCNT_L = 0;
	REG_SOUNDCNT_H = SNDA_RESET_FIFO | SNDB_RESET_FIFO | SNDA_VOL_100 | SNDB_VOL_100 | SNDA_TIMER0 | SNDB_TIMER1;

	REG_TM0CNT_L   = 0xffff - (SND_CPU_CLOCK / SND_AUDIO_RATE);
	REG_TM1CNT_L   = 0xffff - (SND_CPU_CLOCK / SND_AUDIO_RATE);
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndSetData(u16 id, u8* data, u32 size, s16 adjust, bool isLoop)
{
	ST_SND* p = &Snd[id];

	p->act       = SND_ACT_DONOTHING;
	p->cnt       = 0;

	p->data      = data;
	p->size      = size;
	p->frameSize = (size * 60) / SND_AUDIO_RATE + adjust;
	p->isLoop    = isLoop;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndPlay(u16 id)
{
	Snd[id].act = SND_ACT_START;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndStop(u16 id)
{
	Snd[id].act = SND_ACT_STOP;
}
//---------------------------------------------------------------------------
IWRAM_CODE bool SndIsPlay(u16 id)
{
	return (Snd[id].act == SND_ACT_PLAY) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndTerminate(void)
{
	REG_SOUNDCNT_X = 0;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndBgmIntr(void)
{
	ST_SND* p = &Snd[0];

	switch(p->act)
	{
	case SND_ACT_DONOTHING:
		return;

	case SND_ACT_START:
start:
		SndBgmIntrStart();
		p->act = SND_ACT_PLAY;
		return;

	case SND_ACT_STOP:
stop:
		SndBgmIntrStop();
		p->act = SND_ACT_DONOTHING;
		return;

	case SND_ACT_PLAY:
		p->cnt--;

		if(p->cnt <= 0)
		{
			if(p->isLoop == TRUE)
			{
				goto start;
			}
			else
			{
				goto stop;
			}
		}
		return;

	default:
		SystemError("[Err] SndIntrBgm");
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndBgmIntrStart(void)
{
	REG_TM0CNT_H = 0;
	REG_DMA1CNT  = 0;

	DMA1COPY(Snd[0].data, &REG_FIFO_A, DMA_SPECIAL | DMA32 | DMA_REPEAT | DMA_SRC_INC | DMA_DST_FIXED);
	REG_TM0CNT_H    = TIMER_FREQ_PER_1 | TIMER_START;
	REG_SOUNDCNT_H |= (SNDA_R_ENABLE | SNDA_L_ENABLE | SNDA_RESET_FIFO);

	Snd[0].cnt = Snd[0].frameSize;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndBgmIntrStop(void)
{
	REG_SOUNDCNT_H &= ~(SNDA_R_ENABLE | SNDA_L_ENABLE);
	REG_TM1CNT_H    = 0;
	REG_DMA1CNT     = 0;

	Snd[0].cnt = 0;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndSeIntr(void)
{
	ST_SND* p = &Snd[1];

	switch(p->act)
	{
	case SND_ACT_DONOTHING:
		return;

	case SND_ACT_START:
start:
		SndSeIntrStart();
		p->act = SND_ACT_PLAY;
		return;

	case SND_ACT_STOP:
stop:
		SndSeIntrStop();
		p->act = SND_ACT_DONOTHING;
		return;

	case SND_ACT_PLAY:
		p->cnt--;

		if(p->cnt <= 0)
		{
			if(p->isLoop == TRUE)
			{
				goto start;
			}
			else
			{
				goto stop;
			}
		}
		return;

	default:
		SystemError("[Err] SndIntrSe");
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndSeIntrStart(void)
{
	REG_TM1CNT_H = 0;
	REG_DMA2CNT  = 0;

	DMA2COPY(Snd[1].data, &REG_FIFO_B, DMA_SPECIAL | DMA32 | DMA_REPEAT | DMA_SRC_INC | DMA_DST_FIXED);
	REG_TM1CNT_H    = TIMER_FREQ_PER_1 | TIMER_START;
	REG_SOUNDCNT_H |= (SNDB_R_ENABLE | SNDB_L_ENABLE | SNDB_RESET_FIFO);

	Snd[1].cnt = Snd[1].frameSize;
}
//---------------------------------------------------------------------------
IWRAM_CODE void SndSeIntrStop(void)
{
	REG_SOUNDCNT_H &= ~(SNDB_R_ENABLE | SNDB_L_ENABLE);
	REG_TM1CNT_H    = 0;
	REG_DMA2CNT     = 0;

	Snd[1].cnt = 0;	
}
