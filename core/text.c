#include "text.h"
#include "libmy/spr.h"
#include "libmy/bios_arm.h"
#include "history.h"
#include "res.h"
#include "img.h"


// スプライト
// No.0～7 文字表示
// No.8,9  カーソル

//---------------------------------------------------------------------------
ST_TEXT Text;


//---------------------------------------------------------------------------
EWRAM_CODE void TextInit(void)
{
	_Memset(&Text, 0x00, sizeof(ST_TEXT));

	Text.pImg = (u16*)&bin_k12x10wBitmap;
	Text.pSjis2Leaf = (u16*)&txt_sjis2leaf_txt;


	SprSetDat((u16*)&spr_chrTiles, spr_chrTilesLen, (u16*)&spr_chrPal, spr_chrPalLen);

	SprSetChr(0,  18,  22,  512, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(1,  18,  86,  768, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(2,  82,  22,  520, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(3,  82,  86,  776, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(4, 146,  22,  528, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(5, 146,  86,  784, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(6, 210,  22,  536, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(7, 210,  86,  792, ATTR0_SQUARE, ATTR1_SIZE_64);
	SprSetChr(8, 240, 160, 1020, ATTR0_WIDE,   ATTR1_SIZE_8);
	SprSetChr(9, 240, 160, 1022, ATTR0_WIDE,   ATTR1_SIZE_8);
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextExec(void)
{
	TextExecChr();
	TextExecCur();
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextExecChr(void)
{
	if(Text.isDraw == FALSE || ImgIsText() == FALSE)
	{
		return;
	}

	if(Text.waitCnt < (ImgGetTextWaitNum() - 1))
	{
		Text.waitCnt++;
		return;
	}
	Text.waitCnt = 0;


	if(Text.drawCnt >= Text.workCnt)
	{
		TextDebugLf();
		Text.isDraw = FALSE;
	}
	else
	{
		TextSetChr(Text.workBuf[Text.drawCnt++], TRUE);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextExecCur(void)
{
	if(Text.isCurDraw == FALSE)
	{
		return;
	}

	if(Text.curWaitCnt < TEXT_CURSOR_WAIT_CNT)
	{
		Text.curWaitCnt++;
		return;
	}
	Text.curWaitCnt = 0;


	if(Text.isCurBlink == TRUE)
	{
		Text.isCurBlink = FALSE;

		SprSetEnable(8);
		SprSetEnable(9);
	}
	else
	{
		Text.isCurBlink = TRUE;

		SprSetDisable(8);
		SprSetDisable(9);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetImgWhite(void)
{
	Text.pImg = (u16*)&bin_k12x10wBitmap;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetImgGray(void)
{
	Text.pImg = (u16*)&bin_k12x10gBitmap;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetDisp(void)
{
	u16 i;

	for(i=0; i<8; i++)
	{
		SprSetEnable(i);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetDispCur(void)
{
	if(Text.isCurDraw == TRUE)
	{
		return;
	}

	u16 x = TEXT_CURSOR_SX + Text.x * TEXT_IMG_CX;
	u16 y = TEXT_CURSOR_SY + Text.y * (TEXT_IMG_CY + TEXT_IMG_BLANK_CY);

	Text.isCurDraw  = TRUE;
	Text.isCurBlink = TRUE;
	Text.curWaitCnt = 0;

	SprSetMove(8, x, y);
	SprSetMove(9, x, y+8);
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetUnDisp(void)
{
	u16 i;

	for(i=0; i<8; i++)
	{
		SprSetDisable(i);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetUnDispCur(void)
{
	SprSetDisable(8);
	SprSetDisable(9);

	Text.isCurDraw = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetClear(void)
{
	SprSetClearDat();
	HistoryAddBuf(Text.outBuf);
	BiosExecFixClear(Text.outBuf, TEXT_SCREEN_CX * TEXT_SCREEN_CY * 2);

	Text.x = 0;
	Text.y = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetClearNoBuf(void)
{
	SprSetClearDat();
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetClearXY(void)
{
	Text.x = 0;
	Text.y = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetNewPage(void)
{
	Text.isNewPage = TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextSetBuf(u16 x, u16 y, u16 code)
{
	Text.outBuf[(y * TEXT_SCREEN_CX) + x] = code;
}
//---------------------------------------------------------------------------
// しおりの項目名をテキストより抽出します
IWRAM_CODE void TextSetBufSiori(u16* pStr, u16 len)
{
	u16* pS = pStr;
	u16  cnt;
	u16  i;

	if(len >= TEXT_SIORI_CX-1)
	{
		cnt = TEXT_SIORI_CX-1;
	}
	else
	{
		cnt = len;
	}

	for(i=0; i<cnt; i++)
	{
		Text.sioriBuf[i] = TextGetSjisCode(*pS++);
	}

	for(; i<TEXT_SIORI_CX; i++)
	{
		Text.sioriBuf[i] = 0x0000;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetBufWork(u16* pStr, u16 len)
{
	TextSetBufSiori(pStr, len);


	if(Text.isNewPage == TRUE)
	{
		Text.isNewPage = FALSE;

		TextSetClear();
		TextCalcStrBoundary(pStr, len);
	}
	else
	{
		// 画面に入りきれるかチェックをします
		if(TextCalcStrBoundary(pStr, len) == FALSE)
		{
			TextSetClear();
			TextCalcStrBoundary(pStr, len);
		}
	}

	// 表示はウェイトなしかチェックをします
	if(ImgGetTextWaitNum() == 0)
	{
		TextSetStr(Text.workBuf, Text.workCnt, TRUE);
	}
	else
	{
		Text.waitCnt = 0;
		Text.drawCnt = 0;
		Text.isDraw  = TRUE;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetBufWorkNoWait(u16* pStr, u16 len, bool isBuf)
{
	if(Text.isNewPage == TRUE)
	{
		Text.isNewPage = FALSE;
		TextSetClear();
	}

	bool is = TextCalcStrBoundary(pStr, len);
	_ASSERT(is == TRUE);


	TextSetStr(Text.workBuf, Text.workCnt, isBuf);
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetRestore(void)
{
	u16 x, y;

	for(y=0; y<TEXT_SCREEN_CY; y++)
	{
		for(x=0; x<TEXT_SCREEN_CX; x++)
		{
			TextSetDat(x, y, Text.outBuf[(y * TEXT_SCREEN_CX) + x]);
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetDat(u16 x, u16 y, u16 code)
{
	_ASSERT(x < TEXT_SCREEN_CX && y < TEXT_SCREEN_CY);


	u8 flag = 0x00;

	if(x & 0x01)
	{
		flag |= 0x01;
	}

	if(y & 0x01)
	{
		flag |= 0x10;
	}

	// 座標の偶数、奇数によって書き込み方法を変更します
	switch(flag)
	{
	case 0x00:
		TextSetDatSub1(x, y, code);
		break;

	case 0x01:
		TextSetDatSub2(x, y, code);
		break;

	case 0x10:
		TextSetDatSub3(x, y, code);
		break;

	case 0x11:
		TextSetDatSub4(x, y, code);
		break;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetDatSub1(u16 x, u16 y, u16 code)
{
	u16* pSrc1 = SprGetDatBuf() + (y * 768) + (x * 24);
	u16* pSrc2 = pSrc1 + 16;
	u16* pSrc3 = pSrc1 + 512;
	u16* pSrc4 = pSrc1 + 512 + 16;
	u16* pDst  = Text.pImg + ((TEXT_IMG_CX / 4) * TEXT_IMG_CY) * code;
	u16  i;

	for(i=0; i<8; i++)
	{
		*pSrc1++ = *pDst++;
		*pSrc1++ = *pDst++;
		*pSrc2++ = *pDst++;

		pSrc2++;
	}

	for(i=0; i<2; i++)
	{
		*pSrc3++ = *pDst++;
		*pSrc3++ = *pDst++;
		*pSrc4++ = *pDst++;

		pSrc4++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetDatSub2(u16 x, u16 y, u16 code)
{
	u16* pSrc1 = SprGetDatBuf() + (16+1) + (y * 768) + ((x-1) * 24);
	u16* pSrc2 = pSrc1 + 16 - 1;
	u16* pSrc3 = pSrc1 + 512;
	u16* pSrc4 = pSrc2 + 512;
	u16* pDst  = Text.pImg + ((TEXT_IMG_CX / 4) * TEXT_IMG_CY) * code;
	u16  i;

	for(i=0; i<8; i++)
	{
		*pSrc1++ = *pDst++;
		*pSrc2++ = *pDst++;
		*pSrc2++ = *pDst++;

		pSrc1++;
	}

	for(i=0; i<2; i++)
	{
		*pSrc3++ = *pDst++;
		*pSrc4++ = *pDst++;
		*pSrc4++ = *pDst++;

		pSrc3++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetDatSub3(u16 x, u16 y, u16 code)
{
	u16* pSrc1 = SprGetDatBuf() + (512 + 8) + ((y-1) * 768) + (x * 24);
	u16* pSrc2 = pSrc1 + 16;
	u16* pSrc3 = pSrc1 + 512 - 8;
	u16* pSrc4 = pSrc2 + 512 - 8;
	u16* pDst  = Text.pImg + ((TEXT_IMG_CX / 4) * TEXT_IMG_CY) * code;
	u16  i;

	for(i=0; i<4; i++)
	{
		*pSrc1++ = *pDst++;
		*pSrc1++ = *pDst++;
		*pSrc2++ = *pDst++;

		pSrc2++;
	}

	for(i=0; i<6; i++)
	{
		*pSrc3++ = *pDst++;
		*pSrc3++ = *pDst++;
		*pSrc4++ = *pDst++;

		pSrc4++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetDatSub4(u16 x, u16 y, u16 code)
{
	u16* pSrc1 = SprGetDatBuf() + (528 + 8 + 1) + ((y-1) * 768) + ((x-1) * 24);
	u16* pSrc2 = pSrc1 +  16 - 1;
	u16* pSrc3 = pSrc1 + 512 - 8;
	u16* pSrc4 = pSrc3 +  16 - 1;
	u16* pDst  = Text.pImg + ((TEXT_IMG_CX / 4) * TEXT_IMG_CY) * code;
	u16  i;

	for(i=0; i<4; i++)
	{
		*pSrc1++ = *pDst++;
		*pSrc2++ = *pDst++;
		*pSrc2++ = *pDst++;

		pSrc1++;
	}

	for(i=0; i<6; i++)
	{
		*pSrc3++ = *pDst++;
		*pSrc4++ = *pDst++;
		*pSrc4++ = *pDst++;

		pSrc3++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetLf(void)
{
	// 選択肢の改行専用

	if(Text.x == 0)
	{
		return;
	}

	Text.x = 0;
	Text.y++;
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetChr(u16 code, bool isBuf)
{
	if(Text.x >= TEXT_SCREEN_CX)
	{
		Text.x = 0;
		Text.y++;
	}

	TextSetDat(Text.x, Text.y, code);
	TextDebug(code);

	if(isBuf == TRUE)
	{
		TextSetBuf(Text.x, Text.y, code);
	}

	Text.x++;
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetStr(u16* pStr, u16 len, bool isBuf)
{
	u16 i;

	for(i=0; i<len; i++)
	{
		TextSetChr(pStr[i], isBuf);
	}

	TextDebugLf();
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetStrSjis(u16 x, u16 y, u16* pStr)
{
	u16 code;

	while(*(u8*)pStr != '\0')
	{
		code = TextGetLeafCode(*pStr++);
		TextSetDat(x, y, code);

		x++;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void TextSetStrFull(u16* pStr)
{
	u16 x, y;

	for(y=0; y<TEXT_SCREEN_CY; y++)
	{
		for(x=0; x<TEXT_SCREEN_CX; x++)
		{
			TextSetDat(x, y, *pStr++);
		}
	}
}
//---------------------------------------------------------------------------
// Sjis -> Leaf
IWRAM_CODE u16 TextGetLeafCode(u16 code)
{
	u16 i;

	for(i=0; i<TEXT_IMG_MAX_FONT_CNT; i++)
	{
		if(code == Text.pSjis2Leaf[i])
		{
			return i;
		}
	}

	_ASSERT(0 && "LeafCode not found");
	return 0;
}
//---------------------------------------------------------------------------
// Leaf -> Sjis
IWRAM_CODE u16 TextGetSjisCode(u16 code)
{
	_ASSERT(code < TEXT_IMG_MAX_FONT_CNT);

	return Text.pSjis2Leaf[code];
}
//---------------------------------------------------------------------------
EWRAM_CODE u16* TextGetSioriStr(void)
{
	return Text.sioriBuf;
}
//---------------------------------------------------------------------------
// 禁則文字の処理
// 引数pSrc よりテキスト画面からあふれる場合、FALSE を戻り値に返します
// 成功時 TRUE。Text.workBuf とText.workCnt に値が格納されます
IWRAM_CODE bool TextCalcStrBoundary(u16* pSrc, u16 len)
{
	u16 x = Text.x;
	u16 y = Text.y;
	u16 s = 0;
	u16 d = 0;
	u16 code;

	// 行末だった場合、行頭に移動します
	if(x >= TEXT_SCREEN_CX)
	{
		x = 0;
		y++;
	}

	// 1つ前の行末だった場合、空白１つ入れて行頭に移動します
	if(x == (TEXT_SCREEN_CX-1))
	{
		Text.workBuf[d++] = 0x0000;

		x = 0;
		y++;
	}

	// 最初の文字が空白の場合、空白を入れます。以降の空白はスキップします
	if(pSrc[s] == 0x0000)
	{
		Text.workBuf[d++] = 0x0000;

		s++;
		x++;
	}

	for(;;)
	{
		_ASSERT(d <= (TEXT_SCREEN_CX * TEXT_SCREEN_CY) && "workBuf overflow");

		if(d >= TEXT_SCREEN_CX * TEXT_SCREEN_CY)
		{
			// 画面あふれの文字は表示しません
			// この処理は、異星人編のテキスト「オラオラ・・・」専用の対応となります
			goto End;
		}
		else if(s >= len)
		{
			goto End;
		}
		else if(y >= TEXT_SCREEN_CY)
		{
			return FALSE;
		}

		code = pSrc[s++];


		if(code == 0x0000)
		{
			// 空白チェック
			// EMPTY
		}
		else if(x == 0 && TextIsChrBoundaryBefore(code) == TRUE && d != 0)
		{
			// 行頭チェック
			Text.workBuf[d+0] = Text.workBuf[d-1];
			Text.workBuf[d-1] = 0x0000;
			Text.workBuf[d+1] = code;
			d += 2;

			y++;
			x = 2;
		}
		else if(x == (TEXT_SCREEN_CX-1) && TextIsChrBoundaryAfter(code) == TRUE)
		{
			// 行末チェック
			Text.workBuf[d+0] = 0x0000;
			Text.workBuf[d+1] = code;
			d += 2;

			y++;
			x = 1;
		}
		else
		{
			// 通常の場合
			Text.workBuf[d++] = code;
			x++;

			if(x >= TEXT_SCREEN_CX)
			{
				x = 0;
				y++;
			}
		}
	}

End:
	Text.workCnt = d;

	return TRUE;
}
//---------------------------------------------------------------------------
IWRAM_CODE bool TextIsChrBoundaryBefore(u16 code)
{
	switch(code)
	{
	case 0x0030:	// ゃ
	case 0x0031:	// ゅ
	case 0x0032:	// ょ
	case 0x0033:	// っ
	case 0x0057:	// 。
	case 0x0058:	// 、
	case 0x0059:	// ・
	case 0x005a:	// …
	case 0x005c:	// 』
	case 0x005e:	// 」
	case 0x0060:	// ）
	case 0x0061:	// ？
	case 0x0062:	// ！
	case 0x0063:	// !?
	case 0x0064:	// －
	case 0x0065:	// ー
	case 0x0097:	// ャ
	case 0x0098:	// ュ
	case 0x0099:	// ョ
	case 0x009a:	// ッ
	case 0x04e9:	// ヶ
	case 0x04ea:	// ぁ
	case 0x04eb:	// ぃ
	case 0x04ec:	// ぅ
	case 0x04ed:	// ぇ
	case 0x04ee:	// ぉ
	case 0x04ef:	// ァ
	case 0x04f0:	// ィ
	case 0x04f1:	// ゥ
	case 0x04f2:	// ェ
	case 0x04f3:	// ォ
	case 0x0683:	// ．
	case 0x06ff:	// ～
		return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------------
IWRAM_CODE bool TextIsChrBoundaryAfter(u16 code)
{
	switch(code)
	{
	case 0x005b:	// 『
	case 0x005d:	// 「
	case 0x005f:	// （
		return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool TextIsEnd(void)
{
	return (Text.isDraw == TRUE && ImgIsText() == TRUE) ? FALSE : TRUE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextDebug(u16 code)
{
}
//---------------------------------------------------------------------------
EWRAM_CODE void TextDebugLf(void)
{
}
