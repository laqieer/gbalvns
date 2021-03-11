# ftp://ftp.kernel.org/pub/linux/utils/kernel/hotplug/udev-015.tar.bz2

/*
This license applies to all files in directory and its subdirectories,
unless otherwise noted in individual files.


Some files are derived from files derived from the include/ directory
of the Linux kernel, and are licensed under the terms of the GNU
General Public License, version 2, as released by the Free Software
Foundation, Inc.; incorporated herein by reference.

				-----

Some files are derived from files copyrighted by the Regents of The
University of California, and are available under the following
license:

Note: The advertising clause in the license appearing on BSD Unix
files was officially rescinded by the Director of the Office of
Technology Licensing of the University of California on July 22
1999. He states that clause 3 is "hereby deleted in its entirety."

 * Copyright (c)
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

				-----

For all remaining files, the following license applies:

 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * Any copyright notice(s) and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#
# arch/arm/setjmp-thumb.S
#
# setjmp/longjmp for the ARM/thumb architecture
#

#
# The jmp_buf is assumed to contain the following, in order:
#		lr
#		r4
#		r5
#		r6
#		r7
#		r8
#		r9
#		r10
#		fp
#		sp
#


	.text
	.section .iwram,"ax",%progbits
	.thumb
	.align 2

@---------------------------------------------------------------------------------
	.global	Setjmp
	.thumb_func
@---------------------------------------------------------------------------------
Setjmp:
	mov	r3, lr
	stmia	r0!, {r3-r7}
	mov	r3, r8
	mov	r4, r9
	mov	r5, r10
	mov	r6, fp
	mov	r7, sp
	stmia	r0!, {r3-r7}

	mov	r0, #0
	mov	pc, lr

@---------------------------------------------------------------------------------
	.global	Longjmp
	.thumb_func
@---------------------------------------------------------------------------------
Longjmp:
	mov	r2, r0
	add	r0, #5*4
	ldmia	r0!, {r3-r7}
	mov	r8, r3
	mov	r9, r4
	mov	r10, r5
	mov	fp, r6
	mov	sp, r7
	ldmia	r2!, {r3-r7}
	mov	r0, r1
	bne	1f
	mov	r0, #1
1:	mov	lr, r3
	mov	pc, lr
