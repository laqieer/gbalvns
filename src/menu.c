#include "menu.h"
#include "libmy/key.h"
#include "text.h"
#include "siori.h"
#include "script.h"
#include "history.h"
#include "img.h"


//---------------------------------------------------------------------------
ST_MENU Menu EWRAM_BSS;


//---------------------------------------------------------------------------
EWRAM_CODE void MenuInit(void)
{
	_Memset(&Menu, 0x00, sizeof(ST_MENU));
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuExec(void)
{
	switch(Menu.act)
	{
	case MENU_ACT_READY:
		TextSetClearNoBuf();
		MenuSetAct(MENU_ACT_DRAW);
		break;

	case MENU_ACT_DRAW:
		MenuExecDraw();
		break;

	case MENU_ACT_SELECT:
		MenuExecSelect();
		break;

	case MENU_ACT_SELECTED:
		MenuExecSelected();
		break;

	case MENU_ACT_END_READY:
		TextSetRestore();
		MenuSetAct(MENU_ACT_END);
		break;

	default:
		SystemError("[Err] MenuExec Menu.act=%d\n", Menu.act);
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuExecDraw(void)
{
	TextSetStrSjis(Menu.sx, Menu.sy, (u16*)Menu.title);

	u16 i;

	for(i=0; i<Menu.itemCnt; i++)
	{
		if(i == Menu.selectNum)
		{
			TextSetImgWhite();
		}
		else
		{
			TextSetImgGray();
		}

		TextSetStrSjis(Menu.sx, Menu.sy + 2 + i, (u16*)Menu.item[i].str);
	}
	TextSetImgWhite();


	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuExecSelect(void)
{
	ST_MENU* p = &Menu;

	p->trg = KeyGetTrg();

	if((p->trg & KEY_B) && Menu.isCancel == TRUE)
	{
		if(Menu.ret == MENU_RET_TITLE)
		{
			MenuSetTitle();
		}
		else if(Menu.ret == MENU_RET_SYSTEM)
		{
			MenuSetSystem();
		}
		else
		{
			MenuSetAct(MENU_ACT_END_READY);
		}
	}
	else if((p->trg & KEY_UP) && Menu.selectNum > 0)
	{
		Menu.selectNum--;
		MenuSetAct(MENU_ACT_DRAW);
	}
	else if((p->trg & KEY_DOWN) && (Menu.selectNum + 1) < Menu.itemCnt)
	{
		Menu.selectNum++;
		MenuSetAct(MENU_ACT_DRAW);
	}
	else if((p->trg & KEY_LEFT) || (p->trg & KEY_RIGHT) || (p->trg & KEY_A))
	{
		if(Menu.selectNum != -1)
		{
			MenuSetAct(MENU_ACT_SELECTED);
		}
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuExecSelected(void)
{
	Menu.item[Menu.selectNum].pFunc();
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetAct(u16 act)
{
	Menu.act     = act;
	Menu.actStep = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetStrNum(u16 no, u16 num)
{
	// 0x82 0x4f = SJISコード「０」

	Menu.item[no].str[14] = 0x82;
	Menu.item[no].str[15] = (num >= 10) ? 0x50 : 0x4f;
	Menu.item[no].str[16] = 0x82;
	Menu.item[no].str[17] = (num >= 10) ? 0x4f + (num - 10) : 0x4f + num;
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetTitle(void)
{
	MenuInit();
	MenuSetAct(MENU_ACT_READY);

	MenuSetOption(5, 4, "", FALSE);
	MenuAddItem("ゲームを始める", &MenuExecItemStart);
	MenuAddItem("ロードする",     &MenuExecItemLoad);
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetSystem(void)
{
	MenuInit();
	MenuSetAct(MENU_ACT_READY);

	MenuSetOption(3, 0, "－　システムメニュー　－", TRUE);
	MenuAddItem("次の選択肢に進む",     &MenuExecItemNext);
	MenuAddItem("１つ前の選択肢に戻る", &MenuExecItemPrev);
	MenuAddItem("シナリオ回想",         &MenuExecItemRef);
	MenuAddItem("文字を消す",           &MenuExecItemTextUnDisp);
	MenuAddItem("セーブ",               &MenuExecItemSave);
	MenuAddItem("ロード",               &MenuExecItemLoad2);
	MenuAddItem("オプション",           &MenuExecItemOption);
	MenuAddItem("ゲーム終了",           &MenuExecItemGameEnd);
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetSystemOption(void)
{
	MenuInit();
	MenuSetAct(MENU_ACT_READY);

	MenuSetOption(3, 0, "－　システム設定　－", TRUE);
	MenuAddItem("文字ウェイト　００", &MenuExecItemTextWait);
	MenuAddItem("背景輝度　　　００", &MenuExecItemTextFade);

	MenuSetStrNum(0, ImgGetTextWaitNum());
	MenuSetStrNum(1, ImgGetTextFadeNum());
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetSave(void)
{
	MenuInit();
	MenuSetAct(MENU_ACT_READY);

	MenuSetOption(3, 0, "　　－　セーブ　－", TRUE);

	u16 i;

	for(i=0; i<SIORI_MAX_MENU_CNT; i++)
	{
		MenuAddItem(SioriGetName(i), &MenuExecItemSaveSiori);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetLoad(void)
{
	MenuInit();
	MenuSetAct(MENU_ACT_READY);

	MenuSetOption(3, 0, "　　－　ロード　－", TRUE);

	u16 i;

	for(i=0; i<SIORI_MAX_MENU_CNT; i++)
	{
		MenuAddItem(SioriGetName(i), &MenuExecItemLoadSiori);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuSetOption(u16 sx, u16 sy, char* pTitle, bool isCancel)
{
	Menu.sx = sx;
	Menu.sy = sy;
	Menu.isCancel  = isCancel;
	Menu.selectNum = 0;
	Menu.itemCnt   = 0;

	_Strncpy(Menu.title, pTitle, MENU_MAX_STR_LEN);
}
//---------------------------------------------------------------------------
EWRAM_CODE void MenuAddItem(char* pStr, void* pFunc)
{
	_ASSERT(Menu.itemCnt <  MENU_MAX_ITEM_CNT);


	_Strncpy(Menu.item[Menu.itemCnt].str, pStr, MENU_MAX_STR_LEN);
	Menu.item[Menu.itemCnt].pFunc = pFunc;

	Menu.itemCnt++;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool MenuIsEnd(void)
{
	return (Menu.act == MENU_ACT_END) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
// タイトル　スタート
EWRAM_CODE void MenuExecItemStart(void)
{
	if(Menu.trg & KEY_A)
	{
		ScriptInitFlag();
		ScriptSetScn(0);
		ScriptSetEvent(1);

		MenuSetAct(MENU_ACT_END_READY);
		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// タイトル　ロード
EWRAM_CODE void MenuExecItemLoad(void)
{
	if(Menu.trg & KEY_A)
	{
		MenuSetLoad();
		Menu.ret = MENU_RET_TITLE;

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// 次の選択肢に進む
EWRAM_CODE void MenuExecItemNext(void)
{
	if(Menu.trg & KEY_A)
	{
		ScriptSetNext(TRUE);
		MenuSetAct(MENU_ACT_END_READY);

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// １つ前の選択肢に戻る
EWRAM_CODE void MenuExecItemPrev(void)
{
	if(Menu.trg & KEY_A)
	{
		SioriLoadPrev();
		MenuSetAct(MENU_ACT_END_READY);

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// シナリオ回想
EWRAM_CODE void MenuExecItemRef(void)
{
	switch(Menu.actStep)
	{
	case 0:
		if(Menu.trg & KEY_A && HistoryIsEmpty() == FALSE)
		{
			Menu.actStep++;
		}
		else
		{
			MenuSetAct(MENU_ACT_SELECT);
		}
		break;

	case 1:
		HistoryInitAct();
		Menu.actStep++;
		break;

	case 2:
		HistoryExec();

		if(HistoryIsEnd() == TRUE)
		{
			MenuSetAct(MENU_ACT_READY);
		}
		break;
	}
}
//---------------------------------------------------------------------------
// 文字を消す
EWRAM_CODE void MenuExecItemTextUnDisp(void)
{
	switch(Menu.actStep)
	{
	case 0:
		if(Menu.trg & KEY_A)
		{
			Menu.actStep++;
		}
		else
		{
			MenuSetAct(MENU_ACT_SELECT);
		}
		break;

	case 1:
		ImgSetEffectClear(IMG_EFFECT_TEXT);
		Menu.actStep++;
		break;

	case 2:
		Menu.trg = KeyGetTrg();

		if(Menu.trg & KEY_A)
		{
			ImgSetEffectUpdate(IMG_EFFECT_TEXT);

			MenuSetAct(MENU_ACT_SELECT);
		}
	}
}
//---------------------------------------------------------------------------
// セーブ　システムメニュー
EWRAM_CODE void MenuExecItemSave(void)
{
	if(Menu.trg & KEY_A)
	{
		MenuSetSave();
		Menu.ret = MENU_RET_SYSTEM;

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// ロード　システムメニュー
EWRAM_CODE void MenuExecItemLoad2(void)
{
	if(Menu.trg & KEY_A)
	{
		MenuSetLoad();
		Menu.ret = MENU_RET_SYSTEM;

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// オプション
EWRAM_CODE void MenuExecItemOption(void)
{
	if(Menu.trg & KEY_A)
	{
		MenuSetSystemOption();
		Menu.ret = MENU_RET_SYSTEM;

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// ゲーム終了
EWRAM_CODE void MenuExecItemGameEnd(void)
{
	if(Menu.trg & KEY_A)
	{
		HistoryInit();
		TextInit();

		ScriptSetAct(SCRIPT_ACT_END);
		MenuSetAct(MENU_ACT_END_READY);
		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// セーブ　しおり１つ選択済み
EWRAM_CODE void MenuExecItemSaveSiori(void)
{
	if(Menu.trg & KEY_A)
	{
		SioriSave(Menu.selectNum);
		MenuSetAct(MENU_ACT_END_READY);

		return;
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// ロード　しおり１つ選択済み
EWRAM_CODE void MenuExecItemLoadSiori(void)
{
	if(Menu.trg & KEY_A)
	{
		if(SioriLoad(Menu.selectNum) == TRUE)
		{
			MenuSetAct(MENU_ACT_END_READY);
			return;
		}
	}

	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// 文字ウェイト
EWRAM_CODE void MenuExecItemTextWait(void)
{
	u16 wait = ImgGetTextWaitNum();

	if(Menu.trg & KEY_LEFT && wait > 0)
	{
		wait--;
		ImgSetTextWaitNum(wait);

		MenuSetStrNum(0, wait);
		MenuSetAct(MENU_ACT_DRAW);
		return;
	}
	else if(Menu.trg & KEY_RIGHT && wait < 15)
	{
		wait++;
		ImgSetTextWaitNum(wait);

		MenuSetStrNum(0, wait);
		MenuSetAct(MENU_ACT_DRAW);
		return;
	}


	MenuSetAct(MENU_ACT_SELECT);
}
//---------------------------------------------------------------------------
// 背景輝度
EWRAM_CODE void MenuExecItemTextFade(void)
{
	u16 fade = ImgGetTextFadeNum();

	if(Menu.trg & KEY_LEFT && fade > 0)
	{
		fade--;
		ImgSetTextFadeNum(fade);

		MenuSetStrNum(1, fade);
		MenuSetAct(MENU_ACT_DRAW);
		return;
	}
	else if(Menu.trg & KEY_RIGHT && fade < 15)
	{
		fade++;
		ImgSetTextFadeNum(fade);

		MenuSetStrNum(1, fade);
		MenuSetAct(MENU_ACT_DRAW);
		return;
	}


	MenuSetAct(MENU_ACT_SELECT);
}
