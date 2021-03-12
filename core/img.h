#ifndef __IMG_H__
#define __IMG_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define IMG_MAX_FILE_LEN					14

#define IMG_BG_STR							"MAX_S%02d.img"
#define IMG_CHR_STR							"MAX_C%02X.img"
#define IMG_HVS_STR							"HVS%02d.img"
#define IMG_VIS_STR							"VIS%02d.img"


enum {
	IMG_EFFECT_FADE_BLACK,					// 0.Black フェード
	IMG_EFFECT_GURUGURU,					// 1.ぐるぐる
	IMG_EFFECT_SLANTTILE,					// 2.斜ずれ横モザイクスライド
	IMG_EFFECT_FADE_SQUARE,					// 3.ひし形◆フェード
	IMG_EFFECT_WIPE_SQUARE_LTOR,			// 4.ひし形◆左ワイプ
	IMG_EFFECT_FADE_MASK,					// 5.マスクフェード
	IMG_EFFECT_WIPE_TTOB,					// 6.上からワイプ
	IMG_EFFECT_WIPE_LTOR, 					// 7.左からワイプ
	IMG_EFFECT_WIPE_MASK_LTOR,				// 8.左からワイプ(マスク)
	IMG_EFFECT_VERTCOMPOSITION,				// 9.縦方向ではさみこみ
	IMG_EFFECT_SLIDE_LTOR,					// 10.左から縦スライド
	IMG_EFFECT_NORMAL,						// 11.単純表示
	IMG_EFFECT_FADE_MASK2,					// 12.マスクフェード
	IMG_EFFECT_FADE_GUNYARI,				// 13.ぐにゃり
	IMG_EFFECT_FADE_GUNYARI2,				// 14.ぐにゃり2
	IMG_EFFECT_SHAKE,						// 15.画面を揺らす

	// 追加
	IMG_EFFECT_FADE_WHITE,					// 16.White フェード
	IMG_EFFECT_WAVE,						// 17.Wave タイトルロゴ用
	IMG_EFFECT_TEXT,						// 18.メッセージ フェード
};

enum {
	IMG_CHR_TYPE_LEFT,
	IMG_CHR_TYPE_RIGHT,
	IMG_CHR_TYPE_MID,
	IMG_CHR_TYPE_ALL,
};

enum {
	IMG_BG_TYPE_BACK,
	IMG_BG_TYPE_VISUAL,
	IMG_BG_TYPE_HCG,

	IMG_BG_TYPE_FILL,						// 白色
	IMG_BG_TYPE_LOGO,						// ロゴ表示
	IMG_BG_TYPE_CLEAR,						// 何もなし（黒色）
};

//---------------------------------------------------------------------------

// 画像ファイルのヘッダ
typedef struct {
	u16  cx;
	u16  cy;
} __PACKED ST_IMG_HEADER;


// 画像管理用
typedef struct {
	bool isDraw;
	char fname[IMG_MAX_FILE_LEN];
	u16  type;
	u16  cx;
	u16  cy;
	u16* pDat;
} ST_CONTROL;


typedef struct {
	u16  step;

	// 背景＋キャラクタ管理
	ST_CONTROL bg;
	ST_CONTROL chr[3];

	// 画面遷移（Clear）
	bool isClear;
	u16  clearType;
	u16  clearNum;

	// 画面遷移（Update）
	bool isUpdate;
	u16  updateType;
	u16  updateNum;

	// 画面遷移（テキスト）
	bool isTextClear;
	bool isText;							// テキストの表示有無
	u16  textFadeNum;						// 表示時のフェード
	u16  textWaitNum;						// テキスト表示時のウェイト

} ST_IMG;


//---------------------------------------------------------------------------
EWRAM_CODE void ImgInit(void);
EWRAM_CODE void ImgExec(void);
EWRAM_CODE void ImgExecClear(void);
EWRAM_CODE void ImgExecUpdate(void);
EWRAM_CODE void ImgExecTextClear(void);
EWRAM_CODE void ImgExecBg(void);
EWRAM_CODE void ImgExecChr(void);

EWRAM_CODE void ImgSetBg(char* pStr);
EWRAM_CODE void ImgSetBg2(u16 no);
EWRAM_CODE void ImgSetBgV(u16 no);
EWRAM_CODE void ImgSetBgH(u16 no);
EWRAM_CODE void ImgSetBgFill(void);
EWRAM_CODE void ImgSetBgLogo(void);
EWRAM_CODE void ImgSetBgClear(void);

EWRAM_CODE void ImgSetChr(char* pStr, u16 type);
EWRAM_CODE void ImgSetChr2(u16 no, u16 type);
EWRAM_CODE void ImgSetChrClear(u16 type);

EWRAM_CODE void ImgSetEffectClear(u16 type);
EWRAM_CODE void ImgSetEffectClear2(u16 type, u16 num);
EWRAM_CODE void ImgSetEffectUpdate(u16 type);
EWRAM_CODE void ImgSetEffectUpdate2(u16 type, u16 num);

EWRAM_CODE u16  ImgGetTextFadeNum(void);
EWRAM_CODE u16  ImgGetTextWaitNum(void);
EWRAM_CODE void ImgSetTextFadeNum(u16 num);
EWRAM_CODE void ImgSetTextWaitNum(u16 num);

EWRAM_CODE bool ImgIsEnd(void);
EWRAM_CODE bool ImgIsText(void);


#ifdef __cplusplus
}
#endif
#endif
