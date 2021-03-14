	#include "leafCode.h"
	.include "EVTDef.s"
	
	.align 4
SCN001:
	.hword  EVT001 - SCN001, MSG001 - SCN001
	.byte 0x43, 0, 0xA0, 4
	.align 4
EVT001:
EVT001_0:
	.hword 3, 0xA, 0xB, 0x42, 0x43
	.byte EventEnd
EVT001_1:
	.byte EventSetBgm, 2
	.byte EventSetBg, 1
	.byte 0x5A
	.byte EventSetEffect2, 1
	.byte 3
	.byte 0x1E
	.byte EventSetPrev
	.byte 0x64
	.byte EventMsg, 0
	.byte EventMsg, 1
	.byte EventEnding3, 0
	.byte EventEnding2, 0
	.byte EventEnd
	.byte EventEndAll
	.align 4
MSG001:
	.hword 1
	.hword MSG001_1 - MSG001, MSG001_2 - MSG001
MSG001_1:
	.byte LeafCodeSpace 誰 で も ビ ジ ュ ア ル ノ ベ ル を 楽 に 作 れ る こ と を 目 指 す も の で す LeafCodePeriod 'k','r', LeafCodeSpace Ｇ Ｂ Ａ Ｄ Ｅ Ｖ Ｅ Ｌ Ｏ Ｐ Ｗ Ｉ Ｋ Ｉ の 先 人 に 感 謝 を LeafCodePeriod 'p','C','c','0','1','$'
MSG001_2:
	.byte LeafCodeDoubleQuoteLeft 一 緒 に ゲ ー ム を 作 り ま し ょ う か LeafCodeDoubleQuoteRight 'p','C','c','0','2','$'
	