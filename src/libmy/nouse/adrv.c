#include "adrv.h"


//---------------------------------------------------------------------------
const u16 FreqData[] = {
//	   C	  C+	   D	  D+	   E	   F	  F+	   G	  G+	   A	  A+	   B
	  44,	 156,	 262,	 363,	 457,	 547,	 631,	 710,	 786,	 854,	 923,	 986,	// o2
	1046,	1102,	1155,	1205,	1253,	1297,	1339,	1379,	1417,	1452,	1486,	1517,	// o3
	1546,	1575,	1602,	1627,	1650,	1673,	1694,	1714,	1732,	1750,	1767,	1783,	// o4
	1798,	1812,	1825,	1837,	1849,	1860,	1871,	1881,	1890,	1899,	1907,	1915,	// o5
	1923,	1930,	1936,	1943,	1949,	1954,	1959,	1964,	1969,	1974,	1978,	1982,	// o6
	1985,	1988,	1992,	1995,	1998,	2001,	2004,	2006,	2009,	2011,	2013,	2015,	// o7
};


const char* DebugCmd[] = {
	"C2",
	"Cs2",
	"D2",
	"Ds2",
	"E2",
	"F2",
	"Fs2",
	"G2",
	"Gs2",
	"A2",
	"As2",
	"B2",
	"C3",
	"Cs3",
	"D3",
	"Ds3",
	"E3",
	"F3",
	"Fs3",
	"G3",
	"Gs3",
	"A3",
	"As3",
	"B3",
	"C4",
	"Cs4",
	"D4",
	"Ds4",
	"E4",
	"F4",
	"Fs4",
	"G4",
	"Gs4",
	"A4",
	"As4",
	"B4",
	"C5",
	"Cs5",
	"D5",
	"Ds5",
	"E5",
	"F5",
	"Fs5",
	"G5",
	"Gs5",
	"A5",
	"As5",
	"B5",
	"C6",
	"Cs6",
	"D6",
	"Ds6",
	"E6",
	"F6",
	"Fs6",
	"G6",
	"Gs6",
	"A6",
	"As6",
	"B6",
	"C7",
	"Cs7",
	"D7",
	"Ds7",
	"E7",
	"F7",
	"Fs7",
	"G7",
	"Gs7",
	"A7",
	"As7",
	"B7",
	"CMD_DUMMY",
	"CMD_R",
	"CMD_Q",
	"CMD_P",
	"CMD_W",
	"CMD_S", 
	"CMD_D",
	"CMD_ECHO",
	"CMD_ECHOVOL",
	"CMD_AND",
	"CMD_TAI",
	"CMD_LOOPBEGIN",
	"CMD_LOOPEND",
	"CMD_VOLUME",
	"CMD_TONE",
	"CMD_TRACKLOOP",
	"CMD_TRACKEND",

	// ONE専用コマンド

	"CMD_MATUYOSI_SW",
	"CMD_MATUYOSI_PL",
	"CMD_MATUYOSI_EV",
	"CMD_MATUYOSI_VL",
	"CMD_MATUYOSI_SP",

	"CMD_DUMMY_END",
};


//---------------------------------------------------------------------------
ST_ADRV Adrv;


//---------------------------------------------------------------------------
EWRAM_CODE void AdrvInit(void)
{
	_Memset(&Adrv, 0x00, sizeof(ST_ADRV));


	// Sound
	REG_SOUNDCNT_X = ALL_SOUND_ENABLE;

	REG_SOUNDCNT_L = DMG_LEFT_VOLUME(3) | DMG_RIGHT_VOLUME(3) |
	                 SOUND1_LEFT_OUTPUT | SOUND1_RIGHT_OUTPUT |
	                 SOUND2_LEFT_OUTPUT | SOUND2_RIGHT_OUTPUT |
	                 SOUND3_LEFT_OUTPUT | SOUND3_RIGHT_OUTPUT |
	                 SOUND4_LEFT_OUTPUT | SOUND4_RIGHT_OUTPUT;

	REG_SOUNDCNT_H = SOUND_OUTPUT_RATIO(2);

/*
	REG_SGWR0 = 0x2064A8EC;
	REG_SGWR1 = 0xDF9B5713;
	REG_SGWR2 = 0x2064A8EC;
	REG_SGWR3 = 0xDF9B5713;

	REG_SGWR0 = 0x98badcfe;
	REG_SGWR1 = 0x89abcdef;
	REG_SGWR2 = 0x44556622;
	REG_SGWR3 = 0x99cc3355;

	REG_SGWR0 = 0xFFFF9920;
	REG_SGWR1 = 0x55557777;
	REG_SGWR2 = 0xFFFF9920;
	REG_SGWR3 = 0x55557777;
*/
	REG_SGWR0 = 0x10325476;
	REG_SGWR1 = 0x98badcfe;
	REG_SGWR2 = 0x10325476;
	REG_SGWR3 = 0x98badcfe;

	// Timer
	REG_TM3CNT_L = ACRV_INTERUPT_TIMER;
	REG_TM3CNT_H = TIMER_FREQ_PER_256 | TIMER_IRQ | TIMER_START;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdrvSetData(u8* pData)
{
	_Memset(&Adrv, 0x00, sizeof(ST_ADRV));


	ST_AC_HEADER* pHeader = (ST_AC_HEADER*)pData;
	u16 i;

	for(i=0; i<ACRV_MAX_TRACK_CNT; i++)
	{
		if(pHeader->offset[i] == ACRV_NO_DATA)
		{
			continue;
		}

		ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[i];


		// GBA
		p->sweepShift  =  0;
		p->sweepDir    =  1;
		p->sweepTime   =  7;
		p->duty        =  0;
		p->envelopStep =  0;
		p->envelopDir  =  0;
		p->envelopVol  = 12;
		p->freq        =  0;

		// MML
		p->isUse       = TRUE;
		p->pTop        = pData + pHeader->offset[i];
		p->pCur        = pData + pHeader->offset[i];

		p->isAnd       = FALSE;
		p->isPan       = FALSE;
		p->pan         =  3;
		p->gate        = 16;
		p->gateWork    =  0;
		p->echo        = 16;
		p->echoWork    =  0;
		p->echoVol     = 15;
		p->detune      =  0;
		p->note        =  0;
		p->length      =  1;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvPlay(void)
{
	Adrv.isUse = TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvStop(void)
{
	Adrv.isUse = FALSE;

	u16 i;

	for(i=0; i<ACRV_MAX_TRACK_CNT; i++)
	{
		Adrv.status[i].isAnd = FALSE;
		Adrv.status[i].freq  = 0;

		AdrvExec(i);
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvExec(u16 num)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[num];

	if(p->isPan == TRUE)
	{
		AdrvExecPan(num);
		p->isPan = FALSE;
	}

	if(p->isAnd == TRUE)
	{
		p->isAnd = FALSE;
		return;
	}


	switch(num)
	{
	case 0:
		AdrvExecCh1();
		break;

	case 1:
		AdrvExecCh2();
		break;

	case 2:
		AdrvExecCh3();
		break;

	case 3:
		AdrvExecCh4();
		break;
	}
}
//---------------------------------------------------------------------------
// スイープ+エンベロープ+周波数
IWRAM_CODE void AdrvExecCh1(void)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[0];

	if(p->freq == 0)
	{
		REG_SOUND1CNT_H = 0;
		return;
	}


	REG_SOUND1CNT_L = SOUND1_SWEEP_SHIFT(p->sweepShift)   |
	                  SOUND1_SWEEP_DIR(p->sweepDir)       |
	                  SOUND1_SWEEP_TIME(p->sweepTime);

	REG_SOUND1CNT_H = SOUND1_LENGTH(0)                    |
	                  SOUND1_DUTY_CYCLE(p->duty)          |
	                  SOUND1_ENVELOP_STEP(p->envelopStep) |
	                  SOUND1_ENVELOP_DIR(p->envelopDir)   |
	                  SOUND1_ENVELOP_VALUE(p->envelopVol);

	REG_SOUND1CNT_X = SOUND1_FREQ(p->freq) | SOUND1_CONTINUE | SOUND1_RESET;
}
//---------------------------------------------------------------------------
// エンベロープ+周波数
IWRAM_CODE void AdrvExecCh2(void)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[1];

	if(p->freq == 0)
	{
		REG_SOUND2CNT_L = 0;
		return;
	}


	REG_SOUND2CNT_L = SOUND2_LENGTH(0)                    |
	                  SOUND2_DUTY_CYCLE(p->duty)          |
	                  SOUND2_ENVELOP_STEP(p->envelopStep) |
	                  SOUND2_ENVELOP_DIR(p->envelopDir)   |
	                  SOUND2_ENVELOP_VALUE(p->envelopVol);

	REG_SOUND2CNT_H = SOUND2_FREQ(p->freq) | SOUND2_CONTINUE | SOUND2_RESET;
}
//---------------------------------------------------------------------------
// ウェーブ
IWRAM_CODE void AdrvExecCh3(void)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[2];

	if(p->freq == 0)
	{
		REG_SOUND3CNT_L = 0;
		return;
	}


	REG_SOUND3CNT_L = SOUND3_BANK_2x32 | SOUND3_BANK_SELECT(3) | SOUND3_OUTPUT_ENABLE;
	REG_SOUND3CNT_H = SOUND3_LENGTH(0) | SOUND3_VOLUME_RATIO(p->envelopVol);
	REG_SOUND3CNT_X = SOUND3_FREQ(p->freq) | SOUND3_CONTINUE | SOUND3_RESET;
}
//---------------------------------------------------------------------------
// エンベロープ+ホワイトノイズ
IWRAM_CODE void AdrvExecCh4(void)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[3];

	if(p->freq == 0)
	{
		REG_SOUND4CNT_L = 0;
		return;
	}


	REG_SOUND4CNT_L = SOUND4_LENGTH(0)                    |
	                  SOUND4_ENVELOP_STEP(p->envelopStep) |
	                  SOUND4_ENVELOP_DIR(p->envelopDir)   |
	                  SOUND4_ENVELOP_VALUE(p->envelopVol);

	REG_SOUND4CNT_H = SOUND4_STAGE_FREQ(5)                |
	                  SOUND4_DIVIDE_FREQ(p->note)         |
	                  SOUND4_CONTINUE                     |
	                  SOUND4_RESET                        |
	                  SOUND4_STAGE_7;
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvExecPan(u16 num)
{
	u8 r = (Adrv.status[num].pan & 0x02) >> 1;
	u8 l = (Adrv.status[num].pan & 0x01);

	switch(num)
	{
	case 0:
		REG_SOUNDCNT_L &= 0xeeff;
		REG_SOUNDCNT_L |= (r <<  8) | (l << 12);
		break;

	case 1:
		REG_SOUNDCNT_L &= 0xddff;
		REG_SOUNDCNT_L |= (r <<  9) | (l << 13);
		break;

	case 2:
		REG_SOUNDCNT_L &= 0xbbff;
		REG_SOUNDCNT_L |= (r << 10) | (l << 14);
		break;

	case 3:
		REG_SOUNDCNT_L &= 0x77ff;
		REG_SOUNDCNT_L |= (r << 11) | (l << 15);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvIntr(void)
{
	if(Adrv.isUse == TRUE)
	{
		AdrvIntrSub();
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvIntrSub(void)
{
	u16 i;

	for(i=0; i<ACRV_MAX_TRACK_CNT; i++)
	{
		ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[i];

		if(p->isUse == FALSE)
		{
			continue;
		}

		p->length--;

		// 発声中かチェックをします
		if(p->length == 0)
		{
			AdrvNextCmd(i);
			AdrvExec(i);

			continue;
		}

		if(p->gateWork != 0 && p->gateWork > p->length)
		{
			p->freq = 0;
			AdrvExec(i);

			p->gateWork = 0;
		}
		else if(p->echoWork != 0 && p->echoWork > p->length)
		{
			u8 tmpVol = p->envelopVol;
			p->envelopVol = p->echoVol;

			// ボリウムを小さくして発声します
			AdrvExec(i);

			p->envelopVol = tmpVol;
			p->echoWork = 0;
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvNextCmd(u16 num)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[num];

ReScan: ;

	u8 cmd   = *p->pCur++;
	u8 param = *p->pCur++;


	switch(cmd)
	{
	// アンド
	case CMD_AND:
		p->isAnd = TRUE;
		goto ReScan;

	// 音量
	case CMD_VOLUME:
		p->envelopVol = param;
		goto ReScan;

	// ステップタイム
	case CMD_S:
		p->envelopStep = param;
		goto ReScan;

	// パターン・デューティ
	case CMD_W:
		p->duty = param;
		goto ReScan;

	// ゲート
	case CMD_Q:
		p->gate = param;
		goto ReScan;

	// 1chエコー
	case CMD_ECHO:
		p->echo = param;
		goto ReScan;

	// 1chエコー音量
	case CMD_ECHOVOL:
		p->echoVol = param;
		goto ReScan;

	// パンポット
	case CMD_P:
		p->pan   = param;
		p->isPan = TRUE;
		goto ReScan;

	// デチューン
	case CMD_D:
		p->detune = (param > 128) ? param - 255 : param;
		goto ReScan;

	// ループ開始
	case CMD_LOOPBEGIN:
		// TODO
		goto ReScan;

	// ループ終了
	case CMD_LOOPEND:
		// TODO
		goto ReScan;

	// 音色
	case CMD_TONE:
		// TODO
		goto ReScan;

	case CMD_TRACKEND:
		// TODO
		goto ReScan;

	case CMD_MATUYOSI_SW:
		p->sweepShift = (param & 0x70) >> 4;
		p->sweepDir   = (param & 0x08) >> 3;
		p->sweepTime  = (param & 0x07);
		goto ReScan;

	case CMD_MATUYOSI_PL:
		p->duty  = (param & 0xC0) >> 6;
		goto ReScan;

	case CMD_MATUYOSI_EV:
		p->envelopStep = (param & 0x03);
		p->envelopDir  = (param & 0x08) >> 3;
		p->envelopVol  = (param & 0xf0) >> 4;
		goto ReScan;

	case CMD_MATUYOSI_VL:
		p->envelopVol  = (param & 0x60) >> 13;
		goto ReScan;

	case CMD_MATUYOSI_SP:
		p->sp = param;
		goto ReScan;

	// ループ
	case CMD_TRACKLOOP:
		p->pCur   = p->pTop;
		p->freq   = 0;
		p->length = 1;
		break;

	// タイ
	case CMD_TAI:
		p->length = param;
		break;

	// 休符
	case CMD_R:
		if(p->echo == 16)
		{
			p->freq = 0;
		}
		p->length = param;
		break;

	// 通常のキー
	default:
		// Gate
		p->gateWork = (param * (16 - p->gate)) / 16;
		if(p->gateWork == 0)
		{
			// 計算誤差で q0 になってしまった場合、強制的に1に格上げします
			p->gateWork++;
		}

		// 1ch Echo
		p->echoWork = (param * (16 - p->echo)) / 16;
		if(p->echoWork == 0)
		{
			// 計算誤差で E0 になってしまった場合、強制的に1に格上げします
			p->echoWork++;
		}

		if(p->isAnd == FALSE)
		{
			p->note = cmd;
			p->freq = FreqData[cmd] + p->detune;
		}

		p->length = param * p->sp;
		break;

	} // switch
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdrvDebug(u16 num)
{
	ST_ADRV_STATUS* p = (ST_ADRV_STATUS*)&Adrv.status[num];

	if(p->note > CMD_DUMMY_END)
	{
		TRACEOUT("Iligal note!! \n");
	}
	else
	{
		TRACEOUT("%s(%x) %x\n", DebugCmd[p->note], p->note, p->length);
	}
}
