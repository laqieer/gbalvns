#include "key.h"


//---------------------------------------------------------------------------
ST_KEY Key;


//---------------------------------------------------------------------------
EWRAM_CODE void KeyInit(void)
{
	_Memset(&Key, 0x00, sizeof(ST_KEY));

	Key.tmp[SYS_KEY_A]  = KEY_A;
	Key.tmp[SYS_KEY_B]  = KEY_B;
	Key.tmp[SYS_KEY_SL] = KEY_SELECT;
	Key.tmp[SYS_KEY_ST] = KEY_START;
	Key.tmp[SYS_KEY_RI] = KEY_RIGHT;
	Key.tmp[SYS_KEY_LE] = KEY_LEFT;
	Key.tmp[SYS_KEY_UP] = KEY_UP;
	Key.tmp[SYS_KEY_DO] = KEY_DOWN;
	Key.tmp[SYS_KEY_R]  = KEY_R;
	Key.tmp[SYS_KEY_L]  = KEY_L;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool KeyChgBtn(u16 b1, u16 b2)
{
	if(b1 > KEY_MAX_BTN_CNT)
	{
		return FALSE;
	}

	Key.tmp[b1] = b2;

	return TRUE;
}
//---------------------------------------------------------------------------
// vblank中に1回だけ呼び出します（チャタリング防止）
IWRAM_CODE void KeyExec(void)
{
	u16 inkey = REG_KEYINPUT;
	u16 cnt   = 0;

	if(inkey & Key.tmp[SYS_KEY_A])  cnt += KEY_A;
	if(inkey & Key.tmp[SYS_KEY_B])  cnt += KEY_B;
	if(inkey & Key.tmp[SYS_KEY_SL]) cnt += KEY_SELECT;
	if(inkey & Key.tmp[SYS_KEY_ST]) cnt += KEY_START;
	if(inkey & Key.tmp[SYS_KEY_RI]) cnt += KEY_RIGHT;
	if(inkey & Key.tmp[SYS_KEY_LE]) cnt += KEY_LEFT;
	if(inkey & Key.tmp[SYS_KEY_UP]) cnt += KEY_UP;
	if(inkey & Key.tmp[SYS_KEY_DO]) cnt += KEY_DOWN;
	if(inkey & Key.tmp[SYS_KEY_R])  cnt += KEY_R;
	if(inkey & Key.tmp[SYS_KEY_L])  cnt += KEY_L;

	cnt     = ~cnt;
	Key.trg = (Key.trg ^ cnt) & ~Key.cnt;
	Key.off = Key.off ^ (~cnt & Key.cnt);
	Key.cnt = cnt;


	// キーリピート
	if(Key.trg & DPAD || Key.repCnt == 0)
	{
		Key.rep = Key.cnt;
		Key.repCnt = KEY_REPEAT_CNT;
	}
	else
	{
		Key.rep = 0;
	}

	if(Key.cnt & DPAD)
	{
		if(Key.repCnt != 0) Key.repCnt--;
	}
	else
	{
		Key.repCnt = 0;
	}
}
//---------------------------------------------------------------------------
// 現在押されているボタン
EWRAM_CODE u16 KeyGetCnt(void)
{
	return Key.cnt;
}
//---------------------------------------------------------------------------
// 押された時のボタン
EWRAM_CODE u16 KeyGetTrg(void)
{
	return Key.trg;
}
//---------------------------------------------------------------------------
// 離された時のボタン
EWRAM_CODE u16 KeyGetOff(void)
{
	return Key.off;
}
//---------------------------------------------------------------------------
// キーリピートのボタン。十字キーのみ対応
EWRAM_CODE u16 KeyGetRep(void)
{
	return Key.rep;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool KeyIsCntAB(void)
{
	return (Key.cnt & (KEY_A | KEY_B)) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool KeyIsTrgAB(void)
{
	return (Key.trg & (KEY_A | KEY_B)) ? TRUE : FALSE;
}
