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
	.ascii "@’N‚Å‚àƒrƒWƒ…ƒAƒ‹ƒmƒxƒ‹‚ğŠy‚Éì‚ê‚é‚±‚Æ‚ğ–Úw‚·‚à‚Ì‚Å‚·Bkr@‚f‚a‚`‚c‚d‚u‚d‚k‚n‚o‚v‚h‚j‚h‚Ìæl‚ÉŠ´Ó‚ğBpCc01$"
MSG001_2:
	.ascii "wˆê‚ÉƒQ[ƒ€‚ğì‚è‚Ü‚µ‚å‚¤‚©xpCc02$"
	