#include "lex.h"


//---------------------------------------------------------------------------
ST_LEX Lex EWRAM_BSS;


//---------------------------------------------------------------------------
EWRAM_CODE void LexInit(void)
{
	_Memset(&Lex, 0x00, sizeof(ST_LEX));
}
//---------------------------------------------------------------------------
EWRAM_CODE void LexSetCur(char* pCur)
{
	Lex.pCur = pCur;
	Lex.pOld = pCur;
}
//---------------------------------------------------------------------------
EWRAM_CODE char* LexGetCur(void)
{
	return Lex.pCur;
}
//---------------------------------------------------------------------------
EWRAM_CODE s32 LexGetNum(void)
{
	LexGetToken(FALSE);

	return (Lex.id == TOKEN_NUM) ? Lex.num : -1;
}
//---------------------------------------------------------------------------
EWRAM_CODE char* LexGetStr(void)
{
	LexGetToken(FALSE);

	return (Lex.id == TOKEN_STR) ? Lex.str : NULL;
}
//---------------------------------------------------------------------------
EWRAM_CODE char LexGetChr(void)
{
	LexGetToken(TRUE);

	return (Lex.id == TOKEN_STR) ? Lex.str[0] : '\0';
}
//---------------------------------------------------------------------------
EWRAM_CODE s32 LexGetSafeNum(void)
{
	s32 ret = LexGetNum();

	if(ret == -1)
	{
		SystemError("[Err] LexGetNumSafe -1");
	}

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE char* LexGetSafeStr(void)
{
	char* ret = LexGetStr();

	if(ret == NULL)
	{
		SystemError("[Err] LexGetStrSafe NULL");
	}

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE char LexGetSafeChr(void)
{
	char ret = LexGetChr();

	if(ret == '\0')
	{
		SystemError("[Err] LexGetChrSafe \\0");
	}

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE void LexGetToken(bool isChr)
{
	char buf[LEX_MAX_STR_CNT];
	s32  num, cnt;

	ST_LEX* p = &Lex;
	p->pOld = p->pCur;

st: ;

	u16 id = LexGetId(*p->pCur);

	switch(id)
	{
	// 空白、タブ
	case TOKEN_SPACE:
		p->pCur++;
		goto st;

	// コンマ
	case TOKEN_CUM:
		p->pCur++;
		goto st;

	// CR
	case TOKEN_CR:
		p->pCur += 2;
		goto st;

	// LF
	case TOKEN_LF:
		p->pCur++;
		goto st;

	// コメント
	case TOKEN_COMMENT:
		LexNextLine();
		goto st;

	// 終端
	case TOKEN_END:
		p->id = id;
		break;

	// 数字
	case TOKEN_NUM:
		num = 0;
		cnt = 0;

		do {
			num = (num * 10) + (*p->pCur++ - '0');
			cnt++;

		} while((LexGetId(*p->pCur) == TOKEN_NUM) && (cnt < 5));

		p->id  = id;
		p->num = num;
		break;

	// 文字列
	case TOKEN_STR:
		cnt = 0;

		do {
			buf[cnt++] = *p->pCur++;

		} while((LexGetId(*p->pCur) != TOKEN_SPACE) && (LexGetId(*p->pCur) != TOKEN_CR) && (LexGetId(*p->pCur) != TOKEN_LF) && (cnt < LEX_MAX_STR_CNT-1) && (isChr == FALSE));

		buf[cnt] = '\0';

		p->id  = id;
		p->num = cnt;
		_Strncpy(p->str, buf, cnt+1);
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE u16 LexGetId(char chr)
{
	if(chr == ' ' || chr == '\t')
	{
		return TOKEN_SPACE;
	}
	else if(chr == CR)
	{
		return TOKEN_CR;
	}
	else if(chr == LF)
	{
		return TOKEN_LF;
	}
	else if(chr == ',')
	{
		return TOKEN_CUM;
	}
	else if(chr == '#')
	{
		return TOKEN_COMMENT;
	}
	else if(chr == '\0')
	{
		return TOKEN_END;
	}
	else if(_IsDigit(chr) == TRUE)
	{
		return TOKEN_NUM;
	}

	return TOKEN_STR;
}
//---------------------------------------------------------------------------
EWRAM_CODE void LexNextLine(void)
{
	ST_LEX* p = &Lex;

	while(*p->pCur != '\0')
	{
		if(*p->pCur == CR)
		{
			p->pCur += 2;
			return;
		}

		if(*p->pCur == LF)
		{
			p->pCur += 1;
			return;
		}

		p->pCur++;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void LexUnGet(void)
{
	Lex.pOld = Lex.pCur;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool LexIsTag(char* str)
{
	ST_LEX* p = &Lex;

	while(*p->pCur != '\0')
	{
		if(_Strncmp(p->pCur, str, _Strlen(str)) == 0)
		{
			return TRUE;
		}

		LexNextLine();
	}

	return FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool LexIsEnd(void)
{
	return (Lex.pCur == '\0') ? TRUE : FALSE;
}
