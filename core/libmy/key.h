#ifndef __KEY_H__
#define __KEY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define KEY_MAX_BTN_CNT		10
#define KEY_REPEAT_CNT		10


enum {
	SYS_KEY_A = 0x00,
	SYS_KEY_B,
	SYS_KEY_SL,
	SYS_KEY_ST,
	SYS_KEY_RI,
	SYS_KEY_LE,
	SYS_KEY_UP,
	SYS_KEY_DO,
	SYS_KEY_R,
	SYS_KEY_L,
};

//---------------------------------------------------------------------------
typedef struct {
	u16 tmp[KEY_MAX_BTN_CNT];	// ボタンの判定用
	u16 cnt;					// 現在のキー
	u16 trg;					// 押されたキー
	u16 off;					// 離されたキー
	u16 rep;					// リピートキー
	s16 repCnt;					// リピートカウント
} ST_KEY;


//---------------------------------------------------------------------------
EWRAM_CODE void KeyInit(void);
EWRAM_CODE bool KeyChgBtn(u16 b1, u16 b2);
IWRAM_CODE void KeyExec(void);

EWRAM_CODE u16  KeyGetCnt(void);
EWRAM_CODE u16  KeyGetTrg(void);
EWRAM_CODE u16  KeyGetOff(void);
EWRAM_CODE u16  KeyGetRep(void);

EWRAM_CODE bool KeyIsCntAB(void);
EWRAM_CODE bool KeyIsTrgAB(void);

#ifdef __cplusplus
}
#endif
#endif
