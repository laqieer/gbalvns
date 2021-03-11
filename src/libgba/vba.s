@ Functions available to be used with VBA
@ Compile with GAS
        
	.file	"vba.s"
	.section .iwram,"ax",%progbits
	.code	16
	.text
	.align	2
	.global	mcount
	.thumb_func
	.type	mcount,function
@ records call-graph
@ r12 contains LR of previous caller               
mcount:
	swi 0xfb
        mov r12, r0
        mov r0, lr
        add r0, $4
        mov lr, r0
        mov r0, r12
        bx lr

        .align 2
        .global monstartup
        .thumb_func
        .type   monstartup,function
@ starts profiling function
@ r0=low PC, r1=high PC                
monstartup:
        swi 0xfe
        bx lr

        .align 2
        .global moncontrol
        .thumb_func
        .type   moncontrol,function
@ controls profiling
@ r0=0 stops profiling, any other value starts it
moncontrol:
        swi 0xfd
        bx lr

        .align 2
        .global moncleanup
        .thumb_func
        .type   moncleanup,function
@ clean up and stop profiling        
moncleanup:
        swi 0xfc
        bx lr

        .align 2
        .global vbalog
        .thumb_func
        .type   vbalog,function
@ log a message to VBA's output console or GDB console
@ r0=message to log                
vbalog:
        swi 0xff
        bx lr


        .align 2
        .global mappylog
        .thumb_func
        .type   mappylog,function
mappylog:
        mov r2,r0
        ldr r0,=0xc0ded00d
        mov r1,#0
        and r0,r0
        bx lr


        .data
