#ifndef __SND_H__
#define __SND_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"

//---------------------------------------------------------------------------
#define SND_CPU_CLOCK			(16 * 1024 * 1024)
#define SND_AUDIO_RATE			8192


enum {
	SND_ID_BGM = 0,
	SND_ID_SE,
};

enum {
	SND_ACT_DONOTHING = 0,
	SND_ACT_START,
	SND_ACT_STOP,
	SND_ACT_PLAY,
};


//---------------------------------------------------------------------------
typedef struct {
	u16  act;
	s32  cnt;

    u8*  data;
    u32  size;
    s16  frameSize;
    bool isLoop;
} ST_SND;


//---------------------------------------------------------------------------
EWRAM_CODE void SndInit(void);
IWRAM_CODE void SndSetData(u16 id, u8* data, u32 size, s16 adjust, bool isLoop);
IWRAM_CODE void SndPlay(u16 id);
IWRAM_CODE void SndStop(u16 id);

IWRAM_CODE bool SndIsPlay(u16 id);
IWRAM_CODE void SndTerminate(void);

IWRAM_CODE void SndBgmIntr(void);
IWRAM_CODE void SndBgmIntrStart(void);
IWRAM_CODE void SndBgmIntrStop(void);
IWRAM_CODE void SndSeIntr(void);
IWRAM_CODE void SndSeIntrStart(void);
IWRAM_CODE void SndSeIntrStop(void);



#ifdef __cplusplus
}
#endif
#endif
