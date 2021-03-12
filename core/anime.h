#ifndef __ANIME_H__
#define __ANIME_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define ANIME_MAX_DB_CNT				6
#define ANIME_MAX_TABLE_CNT				21
#define ANIME_MAX_STR_LEN				13

enum {
	ANIME_TYPE_JINGLE = 0x00,
	ANIME_TYPE_OPENING,
	ANIME_TYPE_TITLE,
	ANIME_TYPE_NAMIDA,
	ANIME_TYPE_ENDING,
	ANIME_TYPE_CLEAR,
};

enum {
	ANIME_ACT_OP = 0x01,						// オペコードを取得中
	ANIME_ACT_FUNC,								// 機能実行中
	ANIME_ACT_END,								// 終了
};

//---------------------------------------------------------------------------

typedef struct {
	u16   act;									// 状態
	void  (*pFunc)(void);						// 命令の関数
	u16   funcStep;								// 命令の実行ステップ

	u16   type;									// アニメーションのタイプ
	bool  isStop;								// キー入力の中断有無
	bool  isKey;								// キーが押された場合のフラグ

	// 作業用変数
	u16   val;
	u16   wait;
	u16   waitCnt;
	char  str[ANIME_MAX_STR_LEN];
	char* pCur;
} ST_ANIME;


typedef struct {
	char* pDat;									// アニメーションファイルの格納位置
} ST_ANIME_DB;

typedef struct {
	char* pStr;									// 命令の文字列
	void  (*pFunc)(void);						// 命令の関数
} ST_ANIME_TABLE;

//---------------------------------------------------------------------------
EWRAM_CODE void AnimeInit(void);
EWRAM_CODE void AnimeSetType(u16 type, bool isStop);
EWRAM_CODE void AnimeSetFunc(void);
EWRAM_CODE void AnimeSetKeyOn(bool is);
EWRAM_CODE void AnimeExec(void);

EWRAM_CODE void AnimeExecImage(void);
EWRAM_CODE void AnimeExecImage2(void);
EWRAM_CODE void AnimeExecImage3(void);
EWRAM_CODE void AnimeExecUpdate(void);
EWRAM_CODE void AnimeExecPrint(void);
EWRAM_CODE void AnimeExecPrintCls(void);
EWRAM_CODE void AnimeExecFill(void);
EWRAM_CODE void AnimeExecBlackIn(void);
EWRAM_CODE void AnimeExecBlackOut(void);
EWRAM_CODE void AnimeExecWhiteIn(void);
EWRAM_CODE void AnimeExecWhiteOut(void);
EWRAM_CODE void AnimeExecWait(void);
EWRAM_CODE void AnimeExecWaitKey(void);
EWRAM_CODE void AnimeExecWave(void);
EWRAM_CODE void AnimeExecBgm(void);
EWRAM_CODE void AnimeExecSetVal(void);
EWRAM_CODE void AnimeExecSetStr(void);
EWRAM_CODE void AnimeExecSetPos(void);
EWRAM_CODE void AnimeExecInc(void);
EWRAM_CODE void AnimeExecJb(void);
EWRAM_CODE void AnimeExecEnd(void);

EWRAM_CODE bool AnimeIsKeyOn(void);
EWRAM_CODE bool AnimeIsEnd(void);

EWRAM_CODE void AnimeCalcRemoveStrQuart(char* pDst, char* pSrc);



#ifdef __cplusplus
}
#endif
#endif
