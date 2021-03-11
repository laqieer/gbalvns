#ifndef __LEX_H__
#define __LEX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define LEX_MAX_STR_CNT				128

enum {
	TOKEN_NUM,
	TOKEN_STR,
	TOKEN_CUM,
	TOKEN_SPACE,
	TOKEN_COMMENT,
	TOKEN_CR,
	TOKEN_LF,
	TOKEN_END,
};

//---------------------------------------------------------------------------
typedef struct {
	char* pCur;								// カーソル
	char* pOld;								// UnGet用カーソル
	u16   id;								// トークンID
	s32   num;								// 数字 or 文字列の長さ
	char  str[LEX_MAX_STR_CNT] ALIGN(4);	// 文字列
} ST_LEX;


//---------------------------------------------------------------------------
EWRAM_CODE void  LexInit(void);
EWRAM_CODE void  LexSetCur(char* pCur);
EWRAM_CODE char* LexGetCur(void);

EWRAM_CODE s32   LexGetNum(void);
EWRAM_CODE char* LexGetStr(void);
EWRAM_CODE char  LexGetChr(void);
EWRAM_CODE s32   LexGetSafeNum(void);
EWRAM_CODE char* LexGetSafeStr(void);
EWRAM_CODE char  LexGetSafeChr(void);

EWRAM_CODE void  LexGetToken(bool isChr);
EWRAM_CODE u16   LexGetId(char chr);
EWRAM_CODE void  LexNextLine(void);
EWRAM_CODE void  LexUnGet(void);

EWRAM_CODE bool  LexIsTag(char* str);
EWRAM_CODE bool  LexIsEnd(void);

#ifdef __cplusplus
}
#endif
#endif
