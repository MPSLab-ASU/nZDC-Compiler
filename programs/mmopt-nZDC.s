	.text
	.file	"../programs/mm.ll"
	.globl	multiply
	.align	2
	.type	multiply,@function
multiply:                               // @multiply
// BB#0:                                // %entry
	sub	 x18, x30, xzr
	stp	x24, x23, [sp, #-32]!
	ldp	x24, x23, [x21, #-32]!
	sub	 x25, x23, x14
	cbnz	x25, .LBB0_3
	sub	 x25, x24, x15
	cbnz	x25, .LBB0_3
	sub	x25, sp, x21
	cbnz	x25, .LBB0_3
	mov	 x9, xzr
	mov	 x3, xzr
	stp	x20, x19, [sp, #16]
	ldp	x20, x19, [x21, #16]
	sub	 x25, x19, x12
	cbnz	x25, .LBB0_3
	sub	 x25, x20, x13
	cbnz	x25, .LBB0_3
	add	x8, x8, #8              // =8
	add	x2, x2, #8              // =8
	add	x6, x6, #12             // =12
	add	x0, x0, #12             // =12
.LBB0_1:                                // %for.cond.1.preheader
                                        // =>This Inner Loop Header: Depth=1
	add	 x10, x8, x9
	add	 x4, x2, x3
	add	 x11, x6, x9
	add	 x5, x0, x3
	add	x9, x9, #16             // =16
	add	x3, x3, #16             // =16
	stur	wzr, [x4, #-8]
	ldur	wzr, [x10, #-8]
	cbnz	xzr, .LBB0_3
	ldur	w12, [x11, #-12]
	ldur	w19, [x5, #-12]
	ldr	 w13, [x7]
	ldr	 w20, [x1]
	mul	 w12, w13, w12
	mul	 w19, w20, w19
	stur	w19, [x4, #-8]
	ldur	w19, [x10, #-8]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-8]
	ldur	w20, [x5, #-8]
	ldr	w14, [x7, #16]
	ldr	w23, [x1, #16]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stur	w19, [x4, #-8]
	ldur	w19, [x10, #-8]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-4]
	ldur	w20, [x5, #-4]
	ldr	w14, [x7, #32]
	ldr	w23, [x1, #32]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stur	w19, [x4, #-8]
	ldur	w19, [x10, #-8]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldr	 w13, [x11]
	ldr	 w20, [x5]
	ldr	w14, [x7, #48]
	ldr	w23, [x1, #48]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stp	w19, wzr, [x4, #-8]
	ldp	w19, wzr, [x10, #-8]
	cbnz	xzr, .LBB0_3
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w12, [x11, #-12]
	ldur	w19, [x5, #-12]
	ldr	w13, [x7, #4]
	ldr	w20, [x1, #4]
	mul	 w12, w13, w12
	mul	 w19, w20, w19
	stur	w19, [x4, #-4]
	ldur	w19, [x10, #-4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-8]
	ldur	w20, [x5, #-8]
	ldr	w14, [x7, #20]
	ldr	w23, [x1, #20]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stur	w19, [x4, #-4]
	ldur	w19, [x10, #-4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-4]
	ldur	w20, [x5, #-4]
	ldr	w14, [x7, #36]
	ldr	w23, [x1, #36]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stur	w19, [x4, #-4]
	ldur	w19, [x10, #-4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldr	 w13, [x11]
	ldr	 w20, [x5]
	ldr	w14, [x7, #52]
	ldr	w23, [x1, #52]
	str	 wzr, [x4]
	ldr	 wzr, [x10]
	cbnz	xzr, .LBB0_3
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stur	w19, [x4, #-4]
	ldur	w19, [x10, #-4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w12, [x11, #-12]
	ldur	w19, [x5, #-12]
	ldr	w13, [x7, #8]
	ldr	w20, [x1, #8]
	mul	 w12, w13, w12
	mul	 w19, w20, w19
	str	 w19, [x4]
	ldr	 w19, [x10]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-8]
	ldur	w20, [x5, #-8]
	ldr	w14, [x7, #24]
	ldr	w23, [x1, #24]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	str	 w19, [x4]
	ldr	 w19, [x10]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-4]
	ldur	w20, [x5, #-4]
	ldr	w14, [x7, #40]
	ldr	w23, [x1, #40]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	str	 w19, [x4]
	ldr	 w19, [x10]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldr	 w13, [x11]
	ldr	 w20, [x5]
	ldr	w14, [x7, #56]
	ldr	w23, [x1, #56]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	stp	 w19, wzr, [x4]
	ldp	 w19, wzr, [x10]
	cbnz	xzr, .LBB0_3
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w12, [x11, #-12]
	ldur	w19, [x5, #-12]
	ldr	w13, [x7, #12]
	ldr	w20, [x1, #12]
	mul	 w12, w13, w12
	mul	 w19, w20, w19
	str	w19, [x4, #4]
	ldr	w19, [x10, #4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-8]
	ldur	w20, [x5, #-8]
	ldr	w14, [x7, #28]
	ldr	w23, [x1, #28]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	str	w19, [x4, #4]
	ldr	w19, [x10, #4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldur	w13, [x11, #-4]
	ldur	w20, [x5, #-4]
	ldr	w14, [x7, #44]
	ldr	w23, [x1, #44]
	mul	 w13, w14, w13
	mul	 w20, w23, w20
	add	 w12, w12, w13
	add	 w19, w19, w20
	str	w19, [x4, #4]
	ldr	w19, [x10, #4]
	sub	w25, w19, w12
	cbnz	w25, .LBB0_3
	ldr	 w11, [x11]
	ldr	 w5, [x5]
	ldr	w13, [x7, #60]
	ldr	w20, [x1, #60]
	mul	 w11, w13, w11
	mul	 w5, w20, w5
	cmp	 w3, #64                // =64
	add	 w11, w12, w11
	add	 w5, w19, w5
	str	w5, [x4, #4]
	ldr	w5, [x10, #4]
	sub	w25, w5, w11
	cbnz	w25, .LBB0_3
	b.ne	.LBB0_1
// BB#2:                                // %for.end.19
	ldp	x13, x12, [x21, #16]
	ldp	x20, x19, [sp, #16]
	ldp	x15, x14, [x21], #32
	ldp	x24, x23, [sp], #32
	ret
.LBB0_3:
	sub	 x25, x25, x25
.Lfunc_end0:
	.size	multiply, .Lfunc_end0-multiply

	.globl	main
	.align	2
	.type	main,@function
main:                                   // @main
// BB#0:                                // %entry
	sub	 x6, x0, xzr
	sub	 x7, x1, xzr
	sub	 x8, x2, xzr
	sub	 x9, x3, xzr
	sub	 x10, x4, xzr
	sub	 x11, x5, xzr
	sub	 x12, x19, xzr
	sub	 x13, x20, xzr
	sub	 x14, x23, xzr
	sub	 x15, x24, xzr
	sub	 x16, x28, xzr
	sub	 x17, x29, xzr
	sub	 x18, x30, xzr
	sub	x21, sp, xzr
	stp	x20, x19, [sp, #-32]!
	ldp	x20, x19, [x21, #-32]!
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	stp	x29, x30, [sp, #16]
	ldp	x29, x30, [x21, #16]
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	add	x17, x21, #16           // =16
	add	x29, sp, #16            // =16
	sub	x21, x21, #192          // =192
	sub	sp, sp, #192            // =192
	adrp	x6, .Lmain.mat2
	adrp	x0, .Lmain.mat2
	add	x6, x6, :lo12:.Lmain.mat2
	add	x0, x0, :lo12:.Lmain.mat2
	add	x7, x21, #64            // =64
	add	x1, sp, #64             // =64
	add	x8, x21, #0             // =0
	mov	 x2, sp
	ldp	q6, q7, [x6, #32]
	ldp	q0, q1, [x0, #32]
	ldp	 q8, q9, [x6]
	ldp	 q2, q3, [x0]
	sub	x6, x17, #80            // =80
	sub	x0, x29, #80            // =80
	stp	q0, q1, [x29, #-48]
	stp	q2, q3, [x29, #-80]
	stp	q0, q1, [sp, #96]
	stp	q2, q3, [sp, #64]
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	multiply
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	adrp	x6, .Lstr
	adrp	x0, .Lstr
	add	x6, x6, :lo12:.Lstr
	add	x0, x0, :lo12:.Lstr
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	puts
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	adrp	x12, .L.str.1
	adrp	x19, .L.str.1
	ldr	 w7, [x21]
	ldr	 w1, [sp]
	add	x12, x12, :lo12:.L.str.1
	add	x19, x19, :lo12:.L.str.1
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #4]
	ldr	w1, [sp, #4]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #8]
	ldr	w1, [sp, #8]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #12]
	ldr	w1, [sp, #12]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	movz	w13, #0xa
	movz	w20, #0xa
	mov	 w6, w13
	mov	 w0, w20
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	putchar
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #16]
	ldr	w1, [sp, #16]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #20]
	ldr	w1, [sp, #20]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #24]
	ldr	w1, [sp, #24]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #28]
	ldr	w1, [sp, #28]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	mov	 w6, w13
	mov	 w0, w20
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	putchar
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #32]
	ldr	w1, [sp, #32]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #36]
	ldr	w1, [sp, #36]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #40]
	ldr	w1, [sp, #40]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #44]
	ldr	w1, [sp, #44]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	mov	 w6, w13
	mov	 w0, w20
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	putchar
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #48]
	ldr	w1, [sp, #48]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #52]
	ldr	w1, [sp, #52]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #56]
	ldr	w1, [sp, #56]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	ldr	w7, [x21, #60]
	ldr	w1, [sp, #60]
	mov	 x6, x12
	mov	 x0, x19
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	printf
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	mov	 w6, w13
	mov	 w0, w20
	sub	 x25, x0, x6
	cbnz	x25, .LBB1_1
	sub	 x25, x1, x7
	cbnz	x25, .LBB1_1
	sub	 x25, x2, x8
	cbnz	x25, .LBB1_1
	sub	 x25, x3, x9
	cbnz	x25, .LBB1_1
	sub	 x25, x4, x10
	cbnz	x25, .LBB1_1
	sub	 x25, x5, x11
	cbnz	x25, .LBB1_1
	sub	 x25, x19, x12
	cbnz	x25, .LBB1_1
	sub	 x25, x20, x13
	cbnz	x25, .LBB1_1
	sub	 x25, x23, x14
	cbnz	x25, .LBB1_1
	sub	 x25, x24, x15
	cbnz	x25, .LBB1_1
	sub	 x25, x28, x16
	cbnz	x25, .LBB1_1
	sub	 x25, x29, x17
	cbnz	x25, .LBB1_1
	sub	 x25, x30, x18
	cbnz	x25, .LBB1_1
	sub	x25, sp, x21
	cbnz	x25, .LBB1_1
	bl	putchar
	sub	x21, sp, xzr
	sub	 x18, x30, xzr
	sub	 x17, x29, xzr
	sub	 x16, x28, xzr
	sub	 x15, x24, xzr
	sub	 x14, x23, xzr
	sub	 x13, x20, xzr
	sub	 x12, x19, xzr
	sub	 x11, x5, xzr
	sub	 x10, x4, xzr
	sub	 x9, x3, xzr
	sub	 x8, x2, xzr
	sub	 x7, x1, xzr
	sub	 x6, x0, xzr
	mov	 w6, wzr
	mov	 w0, wzr
	sub	x21, x17, #16           // =16
	sub	sp, x29, #16            // =16
	ldp	x17, x18, [x21, #16]
	ldp	x29, x30, [sp, #16]
	ldp	x13, x12, [x21], #32
	ldp	x20, x19, [sp], #32
	ret
.LBB1_1:
	sub	 x25, x25, x25
.Lfunc_end1:
	.size	main, .Lfunc_end1-main

	.type	.Lmain.mat2,@object     // @main.mat2
	.section	.rodata,"a",@progbits
	.align	2
.Lmain.mat2:
	.word	1                       // 0x1
	.word	1                       // 0x1
	.word	1                       // 0x1
	.word	1                       // 0x1
	.word	2                       // 0x2
	.word	2                       // 0x2
	.word	2                       // 0x2
	.word	2                       // 0x2
	.word	3                       // 0x3
	.word	3                       // 0x3
	.word	3                       // 0x3
	.word	3                       // 0x3
	.word	4                       // 0x4
	.word	4                       // 0x4
	.word	4                       // 0x4
	.word	4                       // 0x4
	.size	.Lmain.mat2, 64

	.type	.L.str.1,@object        // @.str.1
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.1:
	.asciz	"%d "
	.size	.L.str.1, 4

	.type	.Lstr,@object           // @str
	.section	.rodata.str1.16,"aMS",@progbits,1
	.align	4
.Lstr:
	.asciz	"Result matrix is "
	.size	.Lstr, 18


	.ident	"clang version 3.7.1 (tags/RELEASE_371/final)"
	.section	".note.GNU-stack","",@progbits
