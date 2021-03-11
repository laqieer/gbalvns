#ifndef __MENU_H__
#define __MENU_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define MENU_MAX_STR_LEN					40
#define MENU_MAX_ITEM_CNT					10

enum {
	MENU_ACT_READY = 0x01,
	MENU_ACT_DRAW,
	MENU_ACT_SELECT,
	MENU_ACT_SELECTED,
	MENU_ACT_END_READY,
	MENU_ACT_END,
};

enum {
	MENU_RET_TITLE = 0x01,
	MENU_RET_SYSTEM,
};

//---------------------------------------------------------------------------

typedef struct {
	char str[MENU_MAX_STR_LEN] ALIGN(4);
	void (*pFunc)(void);
} ST_MENU_ITEM;


typedef struct {
	u16  act;
	u16  actStep;
	u16  sx;										// 配置座標
	u16  sy;
	bool isCancel;									// キャンセルの有無
	char title[MENU_MAX_STR_LEN] ALIGN(4);			// 選択肢のタイトル

	s16  selectNum;									// 選択中の場所
	s16  itemCnt;									// 選択肢の登録数
	ST_MENU_ITEM item[MENU_MAX_ITEM_CNT] ALIGN(4);	// 選択肢のアイテム

	u16  trg;										// キー入力格納
	u16  ret;										// 表示階層時の退避用

} ST_MENU;


//---------------------------------------------------------------------------
EWRAM_CODE void MenuInit(void);
EWRAM_CODE void MenuExec(void);
EWRAM_CODE void MenuExecDraw(void);
EWRAM_CODE void MenuExecSelect(void);
EWRAM_CODE void MenuExecSelected(void);

EWRAM_CODE void MenuSetAct(u16 act);
EWRAM_CODE void MenuSetStrNum(u16 no, u16 num);
EWRAM_CODE void MenuSetTitle(void);
EWRAM_CODE void MenuSetSystem(void);
EWRAM_CODE void MenuSetSystemOption(void);
EWRAM_CODE void MenuSetSave(void);
EWRAM_CODE void MenuSetLoad(void);
EWRAM_CODE void MenuSetOption(u16 sx, u16 sy, char* pTitle, bool isCancel);
EWRAM_CODE void MenuAddItem(char* pStr, void* pFunc);

EWRAM_CODE bool MenuIsEnd(void);

EWRAM_CODE void MenuExecItemStart(void);
EWRAM_CODE void MenuExecItemLoad(void);

EWRAM_CODE void MenuExecItemNext(void);
EWRAM_CODE void MenuExecItemPrev(void);
EWRAM_CODE void MenuExecItemRef(void);
EWRAM_CODE void MenuExecItemTextUnDisp(void);
EWRAM_CODE void MenuExecItemSave(void);
EWRAM_CODE void MenuExecItemLoad2(void);
EWRAM_CODE void MenuExecItemOption(void);
EWRAM_CODE void MenuExecItemGameEnd(void);

EWRAM_CODE void MenuExecItemSaveSiori(void);
EWRAM_CODE void MenuExecItemLoadSiori(void);

EWRAM_CODE void MenuExecItemTextWait(void);
EWRAM_CODE void MenuExecItemTextFade(void);


#ifdef __cplusplus
}
#endif
#endif
