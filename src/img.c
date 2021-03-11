#include "img.h"
#include "libmy/gbfs.h"
#include "libmy/mode3.h"
#include "libmy/fade.h"
#include "libmy/bios_arm.h"
#include "text.h"


//---------------------------------------------------------------------------
ST_IMG Img;


//---------------------------------------------------------------------------
EWRAM_CODE void ImgInit(void)
{
	_Memset(&Img, 0x00, sizeof(ST_IMG));

	Img.textFadeNum = 8;
	Img.textWaitNum = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExec(void)
{
	if(Img.isTextClear == TRUE)
	{
		ImgExecTextClear();
		return;
	}

	if(Img.isClear == TRUE)
	{
		ImgExecClear();
		return;
	}

	if(Img.isUpdate == TRUE)
	{
		ImgExecUpdate();
		return;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExecClear(void)
{
	// TODO エフェクト処理

	switch(Img.clearType)
	{
	case IMG_EFFECT_TEXT:
		Img.isClear = FALSE;
		break;

	default:
	case IMG_EFFECT_FADE_BLACK:
		switch(Img.step)
		{
		case 0:
			FadeSetDrakIn(Img.clearNum);
			Img.step++;
			break;

		case 1:
			FadeExecIn();

			if(FadeIsEnd() == TRUE)
			{
				Mode3DrawFullFill(RGB5(0, 0, 0));
				Img.step++;
			}
			break;

		case 2:
			FadeSetClear();

			Img.step = 0;
			Img.isClear = FALSE;
			break;
		}
		break;

	case IMG_EFFECT_FADE_WHITE:
		switch(Img.step)
		{
		case 0:
			FadeSetLightIn(Img.clearNum);
			Img.step++;
			break;

		case 1:
			FadeExecIn();

			if(FadeIsEnd() == TRUE)
			{
				Mode3DrawFullFill(RGB5(31, 31, 31));
				Img.step++;
			}
			break;

		case 2:
			FadeSetClear();

			Img.step = 0;
			Img.isClear = FALSE;
			break;
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExecUpdate(void)
{
	// TODO エフェクト
	u16 i;

	switch(Img.updateType)
	{
	case IMG_EFFECT_TEXT:
		switch(Img.step)
		{
		case 0:
			FadeSetDrakIn2(1, Img.textFadeNum);
			Img.step++;
			break;

		case 1:
			FadeExecIn();

			if(FadeIsEnd() == TRUE)
			{
				TextSetDisp();

				Img.isText = TRUE;
				Img.step = 0;
				Img.isUpdate = FALSE;
			}
			break;
		}
		break;

	case IMG_EFFECT_NORMAL:
	case IMG_EFFECT_FADE_MASK:
		ImgExecBg();
		ImgExecChr();
		Img.isUpdate = FALSE;
		break;

	default:
	case IMG_EFFECT_FADE_BLACK:
		switch(Img.step)
		{
		case 0:
			FadeSetDrakOut(Img.updateNum);
			ImgExecBg();
			ImgExecChr();

			Img.step++;
			break;

		case 1:
			FadeExecOut();

			if(FadeIsEnd() == TRUE)
			{
				Img.step = 0;
				Img.isUpdate = FALSE;
			}
			break;
		}
		break;

	case IMG_EFFECT_FADE_WHITE:
		switch(Img.step)
		{
		case 0:
			FadeSetLightOut(Img.updateNum);
			ImgExecBg();
			ImgExecChr();

			Img.step++;
			break;

		case 1:
			FadeExecOut();

			if(FadeIsEnd() == TRUE)
			{
				Img.step = 0;
				Img.isUpdate = FALSE;
			}
			break;
		}
		break;

	case IMG_EFFECT_WAVE:
		switch(Img.step)
		{
		case 0:
			Img.updateNum = 0;
			Img.step++;
			break;

		case 1:
			for(i=0; i<32; i++)
			{
				Mode3DrawPsetVram(Img.updateNum, i, RGB5(0, 0, 0));
				Mode3DrawPsetVram(Img.updateNum+1, i, RGB5(0, 0, 0));
				Mode3DrawPsetVram(240-Img.updateNum, 160-i, RGB5(0, 0, 0));
				Mode3DrawPsetVram(240-Img.updateNum+1, 160-i, RGB5(0, 0, 0));
			}
			Img.updateNum += 2;

			if(Img.updateNum > 240)
			{
				Img.step = 0;
				Img.isUpdate = FALSE;
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExecTextClear(void)
{
	switch(Img.step)
	{
	case 0:
		TextSetUnDisp();
		TextSetUnDispCur();
		FadeSetDrakOut2(1, Img.textFadeNum);

		Img.step++;
		break;

	case 1:
		FadeExecOut();

		if(FadeIsEnd() == TRUE)
		{
			Img.isText = FALSE;

			Img.step = 0;
			Img.isTextClear = FALSE;
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExecBg(void)
{
	_ASSERT(Img.bg.isDraw == TRUE);


	switch(Img.bg.type)
	{
	default:
	case IMG_BG_TYPE_CLEAR:
		Mode3DrawClear();
		break;

	case IMG_BG_TYPE_FILL:
		Mode3DrawFullFill(RGB5(31, 31, 31));
		break;

	case IMG_BG_TYPE_LOGO:
		ImgSetBg("TITLE0.img");					// 学校の背景
		Mode3DrawFull(Img.bg.pDat);

		ImgSetBg("TITLE.img");					// 雫（決め打ち）
		Mode3DrawImg(84, 13, Img.bg.cx, Img.bg.cy, Img.bg.pDat);
		break;

	case IMG_BG_TYPE_BACK:
	case IMG_BG_TYPE_VISUAL:
	case IMG_BG_TYPE_HCG:
		if(Img.bg.cx == SCREEN_WIDTH && Img.bg.cy == SCREEN_HEIGHT)
		{
			Mode3DrawFull(Img.bg.pDat);
		}
		else
		{
			u16 sx = (SCREEN_WIDTH / 2) - (Img.bg.cx / 2);
			u16 sy = (SCREEN_HEIGHT / 2) - (Img.bg.cy / 2);

			Mode3DrawImg(sx, sy, Img.bg.cx, Img.bg.cy, Img.bg.pDat);
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgExecChr(void)
{
	u16 sx, sy;
	u16 i;

	for(i=0; i<3; i++)
	{
		if(Img.chr[i].isDraw == FALSE)
		{
			continue;
		}


		if(i == 0)
		{
			// 左
			sx = 10;
		}
		else if(i == 1)
		{
			// 右
			sx = SCREEN_WIDTH - Img.chr[i].cx - 10;
		}
		else
		{
			// 中央
			sx = (SCREEN_WIDTH / 2) - (Img.chr[i].cx / 2);
		}
		sy = (SCREEN_HEIGHT - Img.chr[i].cy);


		Mode3DrawImgMask(sx, sy, Img.chr[i].cx, Img.chr[i].cy, Img.chr[i].pDat);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBg(char* pStr)
{
	ST_IMG_HEADER* h = (ST_IMG_HEADER*)GbfsGetSafePointer(pStr);

	_Strncpy(Img.bg.fname, pStr, IMG_MAX_FILE_LEN);
	Img.bg.cx     = h->cx;
	Img.bg.cy     = h->cy;
	Img.bg.pDat   = (u16*)(h + 1);
	Img.bg.type   = IMG_BG_TYPE_BACK;
	Img.bg.isDraw = TRUE;

	ImgSetChrClear(IMG_CHR_TYPE_ALL);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBg2(u16 no)
{
	if(no == 0)
	{
		ImgSetBgClear();
		return;
	}

	char buf[64];
	_Sprintf(buf, IMG_BG_STR, no);

	ImgSetBg(buf);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBgV(u16 no)
{
	char buf[64];
	_Sprintf(buf, IMG_VIS_STR, no);

	ImgSetBg(buf);
	Img.bg.type = IMG_BG_TYPE_VISUAL;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBgH(u16 no)
{
	char buf[64];
	_Sprintf(buf, IMG_HVS_STR, no);

	ImgSetBg(buf);
	Img.bg.type = IMG_BG_TYPE_HCG;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBgFill(void)
{
	Img.bg.type = IMG_BG_TYPE_FILL;
	Img.bg.isDraw = TRUE;

	ImgSetChrClear(IMG_CHR_TYPE_ALL);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBgLogo(void)
{
	Img.bg.type = IMG_BG_TYPE_LOGO;
	Img.bg.isDraw = TRUE;

	ImgSetChrClear(IMG_CHR_TYPE_ALL);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetBgClear(void)
{
	Img.bg.type = IMG_BG_TYPE_CLEAR;
	Img.bg.isDraw = TRUE;

	ImgSetChrClear(IMG_CHR_TYPE_ALL);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetChr(char* pStr, u16 type)
{
	ST_IMG_HEADER* h = (ST_IMG_HEADER*)GbfsGetSafePointer(pStr);

	_Strncpy(Img.bg.fname, pStr, IMG_MAX_FILE_LEN);
	Img.chr[type].cx     = h->cx;
	Img.chr[type].cy     = h->cy;
	Img.chr[type].pDat   = (u16*)(h + 1);
	Img.chr[type].type   = type;
	Img.chr[type].isDraw = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetChr2(u16 no, u16 type)
{
	if(no == 0x99)
	{
		ImgSetChrClear(IMG_CHR_TYPE_ALL);
		return;
	}

	if(no == 0x6a)
	{
		// 窓ガラスのハンドル画像は、非表示にします
		return;
	}

	if(no == 0x1c)
	{
		// 沙織がバレーボール部室で座りこむ画像は、中央表示にします
		type = IMG_CHR_TYPE_MID;
	}

	char buf[64];
	_Sprintf(buf, IMG_CHR_STR, no);

	ImgSetChr(buf, type);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetChrClear(u16 type)
{
	if(type == IMG_CHR_TYPE_ALL)
	{
		Img.chr[0].isDraw = FALSE;
		Img.chr[1].isDraw = FALSE;
		Img.chr[2].isDraw = FALSE;
	}
	else
	{
		Img.chr[type].isDraw = FALSE;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetEffectClear(u16 type)
{
	ImgSetEffectClear2(type, 2);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetEffectClear2(u16 type, u16 num)
{
	Img.isTextClear = Img.isText;

	Img.clearType = type;
	Img.clearNum = num;

	Img.step = 0;
	Img.isClear = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetEffectUpdate(u16 type)
{
	ImgSetEffectUpdate2(type, 2);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetEffectUpdate2(u16 type, u16 num)
{
	if(type == IMG_EFFECT_TEXT && Img.isText == TRUE)
	{
		return;
	}
	Img.isTextClear = Img.isText;

	Img.updateType = type;
	Img.updateNum = num;

	Img.step = 0;
	Img.isUpdate = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 ImgGetTextFadeNum(void)
{
	return Img.textFadeNum;
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 ImgGetTextWaitNum(void)
{
	return Img.textWaitNum;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetTextFadeNum(u16 num)
{
	Img.textFadeNum = num;
	FadeSetDirect(num);		// 強制的に変更します
}
//---------------------------------------------------------------------------
EWRAM_CODE void ImgSetTextWaitNum(u16 num)
{
	Img.textWaitNum = num;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool ImgIsEnd(void)
{
	return (Img.isTextClear == TRUE || Img.isClear == TRUE || Img.isUpdate == TRUE) ? FALSE : TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool ImgIsText(void)
{
	return Img.isText;
}
