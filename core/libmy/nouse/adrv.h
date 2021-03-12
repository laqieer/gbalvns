#ifndef __ADRV_H__
#define __ADRV_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define ACRV_INTERUPT_TIMER			(65535-950)	// 割り込みタイミング
												// 少し速めにしてます。本来は1097

#define ACRV_MAX_SCALE_CNT			12
#define ACRV_MAX_TRACK_CNT			4
#define ACRV_NO_DATA				0x0000


enum {
	C2 = 0,
	Cs2,
	D2,
	Ds2,
	E2,
	F2,
	Fs2,
	G2,
	Gs2,
	A2,
	As2,
	B2,
	C3,
	Cs3,
	D3,
	Ds3,
	E3,
	F3,
	Fs3,
	G3,
	Gs3,
	A3,
	As3,
	B3,
	C4,
	Cs4,
	D4,
	Ds4,
	E4,
	F4,
	Fs4,
	G4,
	Gs4,
	A4,
	As4,
	B4,
	C5,
	Cs5,
	D5,
	Ds5,
	E5,
	F5,
	Fs5,
	G5,
	Gs5,
	A5,
	As5,
	B5,
	C6,
	Cs6,
	D6,
	Ds6,
	E6,
	F6,
	Fs6,
	G6,
	Gs6,
	A6,
	As6,
	B6,
	C7,
	Cs7,
	D7,
	Ds7,
	E7,
	F7,
	Fs7,
	G7,
	Gs7,
	A7,
	As7,
	B7	,			// 音符
	CMD_DUMMY,		// ダミー
	CMD_R,			// 休符
	CMD_Q,			// ゲート
	CMD_P,			// パンポット
	CMD_W,			// デューティ
	CMD_S,			// ステップタイム
	CMD_D,			// デチューン(周波数ずらし)
	CMD_ECHO,		// エコー
	CMD_ECHOVOL,	// エコーボリウム
	CMD_AND,		// タイ
	CMD_TAI,		// タイ
	CMD_LOOPBEGIN,	// ループ開始
	CMD_LOOPEND,	// ループ終了
	CMD_VOLUME,		// 音量
	CMD_TONE,		// 音色
	CMD_TRACKLOOP,	// トラック先頭に戻る
	CMD_TRACKEND,	// トラック終了

	CMD_MATUYOSI_SW,		// スィープ(ch1 only)
	CMD_MATUYOSI_PL,		// パターン・レングス(all channel)
	CMD_MATUYOSI_EV,		// エンベロープ(ch1,ch2 only)
	CMD_MATUYOSI_VL,		// ボリューム(ch3 only)
	CMD_MATUYOSI_SP,		// 再生スピード

	CMD_DUMMY_END,
};


//---------------------------------------------------------------------------
// 音楽データのヘッダ
typedef struct {
	char id[8];				// 'ANDRIVER'
	u16  offset[4];			// 各トラックの先頭からの位置,0の場合には、このトラック未使用

} __PACKED ST_AC_HEADER;


// トラックのステータス
typedef struct {
	// GBA
	u8  sweepShift;			// スウィープの量  （0-7）
	u8  sweepDir;			// スウィープの方向（0=Inc, 1=Dec）
	u8  sweepTime;			// スウィープの時間（0-7）
	u8  duty;				// デューティー比（0-3）
	u8  envelopStep;		// エンベロープの減衰（0-7）
	u8  envelopDir;			// エンベロープの方向（0=Dec, 1=Inc）
	u8  envelopVol;			// エンベロープのボリューム（0-15）
	u16 freq;				// 周波数（0: 休符）

	// MML
	bool isUse;				// 使用の有無
	u8*  pTop;				// 先頭位置
	u8*  pCur;				// 作業位置

	bool isAnd;				// アンド
	bool isPan;				// パン
	u8   pan;
	u8   gate;				// ゲート
	u8   gateWork;
	u8   echo;				// エコー
	u8   echoWork;
	u8   echoVol;
	s8   detune;			// デチューン
	u8   note;				// 鍵盤
	u16  length;			// 長さ（タイマ割り込み毎）

	// ONE 追加分
	u8   sp;

} ST_ADRV_STATUS;


// 各種ドライバのステータス
typedef struct {
	bool isUse;
	ST_ADRV_STATUS status[4];

} ST_ADRV;


//---------------------------------------------------------------------------
EWRAM_CODE void AdrvInit(void);
EWRAM_CODE void AdrvSetData(u8* pData);

IWRAM_CODE void AdrvPlay(void);
IWRAM_CODE void AdrvStop(void);

IWRAM_CODE void AdrvExec(u16 num);
IWRAM_CODE void AdrvExecCh1(void);
IWRAM_CODE void AdrvExecCh2(void);
IWRAM_CODE void AdrvExecCh3(void);
IWRAM_CODE void AdrvExecCh4(void);
IWRAM_CODE void AdrvExecPan(u16 num);


IWRAM_CODE void AdrvIntr(void);
IWRAM_CODE void AdrvIntrSub(void);

IWRAM_CODE void AdrvNextCmd(u16 num);

IWRAM_CODE void AdrvDebug(u16 num);


#ifdef __cplusplus
}
#endif
#endif
