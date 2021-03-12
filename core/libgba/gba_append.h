// 修正分、追加分
#ifndef	_gba_append_h_
#define	_gba_append_h_


#define DEBUG					0



//---------------------------------------------------------------------------
// Reg
#define REG_IRQ_WAITFLAGS		*(volatile u16*)0x3007FF8
#define REG_WSCNT				*(volatile u16*)0x4000204


//---------------------------------------------------------------------------
// Mask
#define KEY_NONE				0x0000
#define STAT_MASK				0x00ff


//---------------------------------------------------------------------------
// SetMode
#define OBJ_2D_MAP				0x0


//---------------------------------------------------------------------------
// Timer
#define TIMER_FREQ_PER_1		0
#define TIMER_FREQ_PER_64		1
#define TIMER_FREQ_PER_256		2
#define TIMER_FREQ_PER_1024		3
#define TIMER_CASCADE			(1<<2)


//---------------------------------------------------------------------------
// Mosaic
#define MOSAIC_BG_H(x)			((x)<<0)
#define MOSAIC_BG_V(x)			((x)<<4)
#define MOSAIC_OBJ_H(x)			((x)<<8)
#define MOSAIC_OBJ_V(x)			((x)<<12)


//---------------------------------------------------------------------------
// Window
#define WIN_RIGHT(x)			((x)<<0)		// REG_WIN0H, WIN1H
#define WIN_LEFT(x)				((x)<<8)

#define WIN_DOWN(x)				((x)<<0)		// REG_WIN0V, WIN1V
#define WIN_TOP(x)				((x)<<8)

#define WIN_0_BG0				(1<<0)			// REG_WININ, WINOUT
#define WIN_0_BG1				(1<<1)
#define WIN_0_BG2				(1<<2)
#define WIN_0_BG3				(1<<3)
#define WIN_0_OBJ				(1<<4)
#define WIN_0_SPE				(1<<5)
#define WIN_1_BG0				(1<<8)
#define WIN_1_BG1				(1<<9)
#define WIN_1_BG2				(1<<10)
#define WIN_1_BG3				(1<<11)
#define WIN_1_OBJ				(1<<12)
#define WIN_1_SPE				(1<<13)


//---------------------------------------------------------------------------
// Blend
#define BLEND_TOP_BG0			(1<<0)			// REG_BLDCNT
#define BLEND_TOP_BG1			(1<<1)
#define BLEND_TOP_BG2			(1<<2)
#define BLEND_TOP_BG3			(1<<3)
#define BLEND_TOP_OBJ			(1<<4)
#define BLEND_TOP_BD			(1<<5)
#define BLEND_LOW_BG0			(1<<8)
#define BLEND_LOW_BG1			(1<<9)
#define BLEND_LOW_BG2			(1<<10)
#define BLEND_LOW_BG3			(1<<11)
#define BLEND_LOW_OBJ			(1<<12)
#define BLEND_LOW_BD			(1<<13)
                     
#define BLEND_MODE_OFF			(0<<6)			// REG_BLDALPHA
#define BLEND_MODE_ALPHA		(1<<6)
#define BLEND_MODE_LIGHT		(2<<6)
#define BLEND_MODE_DARK			(3<<6)
#define BLEND_LOW(n)			((n)<<0)
#define BLEND_HIGH(n)			((n)<<8)
#define BLEND_LEVEL(n)			(BLEND_LOW(n) | BLEND_HIGH(n))
#define BLEND_BALANCE(n)		(BLEND_LOW(n) | BLEND_HIGH(16-n))

#define BLEND_DEPTH(n)			((n)<<0)		// REG_BLDY


//---------------------------------------------------------------------------
// Snd

// REG_SOUNDCNT_L
#define DMG_LEFT_VOLUME(x)		((x)<<0)		// 0-7
#define DMG_RIGHT_VOLUME(x)		((x)<<4)		// 0-7
#define SOUND1_LEFT_OUTPUT		(1<<8)
#define SOUND2_LEFT_OUTPUT		(1<<9)
#define SOUND3_LEFT_OUTPUT		(1<<10)
#define SOUND4_LEFT_OUTPUT		(1<<11)
#define SOUND1_RIGHT_OUTPUT		(1<<12)
#define SOUND2_RIGHT_OUTPUT		(1<<13)
#define SOUND3_RIGHT_OUTPUT		(1<<14)
#define SOUND4_RIGHT_OUTPUT		(1<<15)

// SOUNDCNT_H
#define SOUND_OUTPUT_RATIO(x)	((x)<<0)		// サウンド出力レシオ 0-2 (0=25%,1=50%,2=100%)

// REG_SOUNDCNT_X
#define SOUND1_STATUS			(1<<0)			// サウンドの状態
#define SOUND2_STATUS			(1<<1)			// サウンドの状態
#define SOUND3_STATUS			(1<<2)			// サウンドの状態
#define SOUND4_STATUS			(1<<3)			// サウンドの状態
#define ALL_SOUND_ENABLE		(1<<7)			// 全サウンド使用

// REG_SOUND1CNT_L
#define SOUND1_SWEEP_SHIFT(x)	((x)<<0)		// スィープシフト 0-7
#define SOUND1_SWEEP_DIR(x)		((x)<<3)		// スィープ方向 0-1
#define SOUND1_SWEEP_TIME(x)	((x)<<4)		// スィープ時間 0-7

// REG_SOUND1CNT_H
#define SOUND1_LENGTH(x)		((x)<<0)		// サウンドの長さ 0-63
#define SOUND1_DUTY_CYCLE(x)	((x)<<6)		// デューティサイクル 0-3
#define SOUND1_ENVELOP_STEP(x)	((x)<<8)		// エンベロープステップ時間 0-7
#define SOUND1_ENVELOP_DIR(x)	((x)<<11)		// エンベロープ方向 0-1
#define SOUND1_ENVELOP_VALUE(x)	((x)<<12)		// エンベロープ初期値 0-15

// REG_SOUND1CNT_X
#define SOUND1_FREQ(x)			((x)<<0)		// 周期 0-2048
#define SOUND1_CONTINUE			(0<<14)			// 継続
#define SOUND1_TIMED			(1<<14)			// 時間制限
#define SOUND1_RESET			(1<<15)			// サウンド1リセット

// REG_SOUND2CNT_L
#define SOUND2_LENGTH(x)		((x)<<0)		// サウンドの長さ 0-63
#define SOUND2_DUTY_CYCLE(x)	((x)<<6)		// デューティサイクル 0-3
#define SOUND2_ENVELOP_STEP(x)	((x)<<8)		// エンベロープステップ時間 0-7
#define SOUND2_ENVELOP_DIR(x)	((x)<<11)		// エンベロープ方向 0-1
#define SOUND2_ENVELOP_VALUE(x)	((x)<<12)		// エンベロープ初期値 0-15

// REG_SOUND2CNT_H
#define SOUND2_FREQ(x)			((x)<<0)		// 周期 0-2048
#define SOUND2_CONTINUE			(0<<14)			// 継続
#define SOUND2_TIMED			(1<<14)			// 時間制限
#define SOUND2_RESET			(1<<15)			// サウンド2リセット

// REG_SOUND3CNT_L
#define SOUND3_BANK_2x32		(0<<5)			// 32bitのバンク二つ
#define SOUND3_BANK_1x64		(1<<5)			// 64bitのバンク一つ
#define SOUND3_BANK_SELECT(x)	((x)<<6) 		// バンクを選択 選択していないほうが書き込める 0-1
#define SOUND3_OUTPUT_ENABLE	(1<<7) 			// サウンド3出力
#define SOUND3_OUTPUT_DISABLE	(0<<7)			// サウンド3出力しない

// REG_SOUND3CNT_H
#define SOUND3_LENGTH(x)		((x)<<0)		// サウンドの長さ 0-255
#define SOUND3_VOLUME_RATIO(x)	((x)<<13)		// ボリューム出力レシオ 0-4 1が100%


// REG_SOUND3CNT_X
#define SOUND3_FREQ(x)			((x)<<0)		// 周期 0-2048
#define SOUND3_CONTINUE			(0<<14)			// 継続
#define SOUND3_TIMED			(1<<14)			// 時間制限
#define SOUND3_RESET			(1<<15)			// サウンド1リセット

#define REG_SGWR0				*(volatile u32*)0x4000090
#define REG_SGWR1				*(volatile u32*)0x4000094
#define REG_SGWR2				*(volatile u32*)0x4000098
#define REG_SGWR3				*(volatile u32*)0x400009C


// REG_SOUND4CNT_L
#define SOUND4_LENGTH(x)		((x)<<0)		// サウンドの長さ 0-63
#define SOUND4_ENVELOP_STEP(x)	((x)<<8)		// エンベロープステップ時間 0-7
#define SOUND4_ENVELOP_DIR(x)	((x)<<11)		// エンベロープ方向 0-1
#define SOUND4_ENVELOP_VALUE(x)	((x)<<12)		// エンベロープ初期値 0-15

// REG_SOUND4CNT_H
#define SOUND4_DIVIDE_FREQ(x)	((x)<<0)		// 周期 0-7
#define SOUND4_STAGE_15			(0<<3)			// 15Bit カウンタステージ
#define SOUND4_STAGE_7			(1<<3)			// 7Bit  カウンタステージ
#define SOUND4_STAGE_FREQ(x)	((x)<<4)		// ステップごとの周期 0-13
#define SOUND4_CONTINUE			(0<<14)			// 継続
#define SOUND4_TIMED			(1<<14)			// 時間制限
#define SOUND4_RESET			(1<<15)			// サウンド3リセット


// A, B
#define SNDA_TIMER0				(0 << 10)
#define SNDB_TIMER1				(1 << 14)

//---------------------------------------------------------------------------
// Sprite
#define SP_PAL(pal)				((pal) << 12)
#define SP_NOFLIP				((0x4000))
#define SP_HFLIP				((0x1000) | (0x4000))
#define SP_VFLIP				((0x2000) | (0x4000))
#define SP_HVFLIP				(SP_HFLIP | SP_VFLIP)


//---------------------------------------------------------------------------
// Bios
#define CPUSET_SRC_FIX		(1<<24)
#define CPUSET_16BIT		(0<<26)
#define CPUSET_32BIT		(1<<26)


//---------------------------------------------------------------------------
// Etc
#define	TRACEOUT				if(DEBUG) _Printf
#define	_ASSERT(x)				if(DEBUG) if(!(x)) SystemError("[Assert] %s(%d): "#x"\n", __FILE__, __LINE__)
#define __PACKED				__attribute__((__packed__))

#define HIBYTE(x)				((u8)(((x) >> 8) & 0xff))
#define LOBYTE(x)				((u8)((x) & 0xff))
#define MAKEWORD(l, h)			(((l) & 0xff) | (((h) << 8) & 0xff00))

#define ROM_CODE				__attribute__((section (".text"),long_call))
#define ROM_DATA				__attribute__((section (".roda")))

typedef enum { FALSE, TRUE } bool;

enum {
	BG_SYS  = 0x00,
};


//---------------------------------------------------------------------------------
#include "common.h"
#include "vba.h"


#endif //_gba_append_h
