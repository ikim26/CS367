	.file	"switch.c"
	.text
	.globl	do_switch
	.type	do_switch, @function
do_switch:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	$0, -8(%rbp)
	cmpq	$6, -24(%rbp)
	ja	.L2
	movq	-24(%rbp), %rax
	salq	$3, %rax
	addq	$.L4, %rax
	movq	(%rax), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L4:
	.quad	.L2
	.quad	.L7
	.quad	.L6
	.quad	.L2
	.quad	.L5
	.quad	.L3
	.quad	.L3
	.text
.L7:
	movq	$16, -8(%rbp)
	jmp	.L8
.L6:
	movq	$512, -8(%rbp)
	jmp	.L8
.L5:
	movq	$16384, -8(%rbp)
	jmp	.L8
.L3:
	movq	$96, -8(%rbp)
	jmp	.L8
.L2:
	movq	$0, -8(%rbp)
	nop
.L8:
	movq	-8(%rbp), %rax
	imulq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	do_switch, .-do_switch
	.section	.rodata
.LC0:
	.string	"Switching on %ld\n"
.LC1:
	.string	"Returned %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	call	rand
	movl	%eax, %ecx
	movl	$-1840700269, %edx
	movl	%ecx, %eax
	imull	%edx
	leal	(%rdx,%rcx), %eax
	sarl	$2, %eax
	movl	%eax, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %edx
	sall	$3, %edx
	subl	%eax, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	cltq
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	do_switch
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.4.1 20200928 (Red Hat 8.4.1-1)"
	.section	.note.GNU-stack,"",@progbits
