#include "history.h"
#include "libmy/key.h"
#include "libmy/bios_arm.h"
// #include "text.h"


//---------------------------------------------------------------------------
ST_HISTORY History EWRAM_BSS;


//---------------------------------------------------------------------------
EWRAM_CODE void HistoryInit(void)
{
	_Memset(&History, 0x00, sizeof(ST_HISTORY));
}
//---------------------------------------------------------------------------
EWRAM_CODE void HistoryInitAct()
{
	History.act = HISTORY_INIT;
}
//---------------------------------------------------------------------------
EWRAM_CODE void HistoryExec(void)
{
	u16 trg = KeyGetTrg();
	u16 rep = KeyGetRep();

	switch(History.act)
	{
	case HISTORY_INIT:
		History.selNum = 0;
		History.act = HISTORY_SET_BUF;
		break;

	case HISTORY_SET_BUF:
		HistorySetBuf(History.selNum);
		History.act = HISTORY_SELECT;
		break;

	case HISTORY_SELECT:
		if(trg & KEY_B)
		{
			History.act = HISTORY_END_READY;
		}
		else if(rep & KEY_LEFT)
		{
			if((History.selNum+1) < History.curNum)
			{
				History.selNum++;
				History.act = HISTORY_SET_BUF;
			}
		}
		else if(rep & KEY_RIGHT)
		{
			if(History.selNum != 0)
			{
				History.selNum--;
				History.act = HISTORY_SET_BUF;
			}
			else
			{
				History.act = HISTORY_END_READY;
			}
		}
		break;

	case HISTORY_END_READY:
		// EMPTY
		History.act = HISTORY_END;
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void HistoryAddBuf(u16* pBuf)
{
	BiosExec(pBuf, &History.buf[History.reserveNum][0], TEXT_SCREEN_CX * TEXT_SCREEN_CY * 2);

	History.reserveNum++;
	History.reserveNum &= (HISTORY_MAX_CNT - 1);

	if(History.curNum < HISTORY_MAX_CNT)
	{
		History.curNum++;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void HistorySetBuf(u16 sel)
{
	u16 num = (History.reserveNum - sel - 1) & (HISTORY_MAX_CNT - 1);

	// Textモジュールへ履歴バッファを書き込みます
	TextSetImgGray();
	TextSetStrFull(&History.buf[num][0]);
	TextSetImgWhite();
}
//---------------------------------------------------------------------------
EWRAM_CODE bool HistoryIsEmpty(void)
{
	return (History.curNum == 0) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool HistoryIsEnd(void)
{
	return (History.act == HISTORY_END) ? TRUE : FALSE;
}
