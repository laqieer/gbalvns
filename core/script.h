#ifndef __SCRIPT_H__
#define __SCRIPT_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define SCRIPT_MAX_SYS_FLAG_CNT				14
#define SCRIPT_MAX_SCN_CNT					197
#define SCRIPT_MAX_SELECT_CNT				10
#define SCRIPT_SCN_STR						"SCN%03d.DAT"


enum {
	SCRIPT_ACT_EVENT = 0x01,
	SCRIPT_ACT_MSG,
	SCRIPT_ACT_ANIME,
	SCRIPT_ACT_WAIT_PAGE,
	SCRIPT_ACT_WAIT_KEY,
	SCRIPT_ACT_HISTORY,
	SCRIPT_ACT_SELECT,
	SCRIPT_ACT_MENU,
	SCRIPT_ACT_END,
};

//---------------------------------------------------------------------------

typedef struct {
	// 状態
	u16  act;
	u16  actTmp;
	u16  actStep;
	u16  actStepTmp;
	bool isLoop;

	// スクリプトデータ
	u16  scnNo;
	u16  eventSize;
	u16  msgSize;
	u8*  pEvent;									// イベントデータの位置
	u8*  pEventCur;
	u8*  pEventOld;
	u8*  pMsg;										// メッセージデータの位置
	u8*  pMsgCur;
	u8*  pMsgOld;

	// ゲーム関係
	u8   flag[SCRIPT_MAX_SYS_FLAG_CNT];				// スクリプト制御フラグ
	bool isNext;									// 次の選択肢まで進むフラグ

	// 選択肢
	u8   selMsg;
	u8   selCnt;
	s8   selNum;
	u8   selItem[SCRIPT_MAX_SELECT_CNT];
	u8   selJump[SCRIPT_MAX_SELECT_CNT];
} ST_SCRIPT;


// スクリプトファイルのヘッダ部
typedef struct {
	u16 eventOffset;
	u16 msgOffset;
	u16 eventSize;
	u16 msgSize;
} __PACKED ST_SCRIPT_HEADER;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptInit(void);
EWRAM_CODE void ScriptInitFlag(void);

EWRAM_CODE void ScriptExec(void);
EWRAM_CODE void ScriptExecInit(void);
EWRAM_CODE void ScriptExecAnime(void);
EWRAM_CODE void ScriptExecWaitPage(void);
EWRAM_CODE void ScriptExecWaitKey(void);
EWRAM_CODE void ScriptExecHistory(void);
EWRAM_CODE void ScriptExecMenu(void);
EWRAM_CODE void ScriptExecSelect(void);
EWRAM_CODE void ScriptExecSelectSub(u8 msgNo, bool isSel, bool isBuf);

EWRAM_CODE void ScriptSetAct(u16 act);
EWRAM_CODE void ScriptPushAct(u16 act);
EWRAM_CODE void ScriptPopAct(void);

EWRAM_CODE void ScriptSetNext(bool is);
EWRAM_CODE void ScriptSetScn(u16 scnNo);
EWRAM_CODE void ScriptSetEvent(u16 eventNo);
EWRAM_CODE void ScriptSetMsg(u16 msgNo);
EWRAM_CODE void ScriptSetFlag(u8 no, u8 val);
EWRAM_CODE void ScriptAddFlag(u8 no, u8 val);
EWRAM_CODE u8   ScriptGetFlag(u8 no);

EWRAM_CODE u8   ScriptGetMapFlag(u8 no);
EWRAM_CODE u8   ScriptGetMapBg(u8 no);
EWRAM_CODE u8   ScriptGetMapBg2(u8 c1, u8 c2);
EWRAM_CODE u8   ScriptGetMapVisual(u8 no);
EWRAM_CODE u8   ScriptGetMapVisual2(u8 c1, u8 c2);
EWRAM_CODE u8   ScriptGetMapEffect(u8 c1, u8 c2);
EWRAM_CODE u8   ScriptGetMapBgm(u8 no);
EWRAM_CODE u8   ScriptGetMapChrPos(u8 pos);
EWRAM_CODE u8   ScriptGetMapDig(u8 c1, u8 c2);
EWRAM_CODE u8   ScriptGetMapTolower(u8 c);
EWRAM_CODE u8   ScriptGetMapHexToDig(u8 c);
EWRAM_CODE u8   ScriptGetMapHexToDig2(u8 c1, u8 c2);

EWRAM_CODE bool ScriptCalcKey();
EWRAM_CODE bool ScriptIsEnd(void);


#ifdef __cplusplus
}
#endif
#endif

