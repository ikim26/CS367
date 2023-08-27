	.file	"rec6.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"error - wrong number of arguments.\n\n"
.LC1:
	.string	"\tusage: %s %s"
.LC2:
	.string	" arg%d"
.LC3:
	.string	"\n\n"
	.text
	.type	check_func_params, @function
check_func_params:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movl	%ecx, -40(%rbp)
	movl	-40(%rbp), %eax
	addl	$2, %eax
	cmpl	%eax, -36(%rbp)
	jne	.L2
	movl	$1, %eax
	jmp	.L6
.L2:
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$36, %edx
	movl	$1, %esi
	movl	$.LC0, %edi
	call	fwrite
	movq	stderr(%rip), %rax
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, -4(%rbp)
	jmp	.L4
.L5:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-40(%rbp), %eax
	jle	.L5
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$2, %edx
	movl	$1, %esi
	movl	$.LC3, %edi
	call	fwrite
	movl	$1, %edi
	call	exit
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	check_func_params, .-check_func_params
	.section	.rodata
	.align 8
.LC4:
	.string	"error - not enough arguments.\n\n\tusage: %s funcname arg1 [arg2 arg3 ...]\n\n"
	.align 8
.LC5:
	.string	"error - memory allocation failed.\n\n"
.LC6:
	.string	"mul_24"
.LC7:
	.string	"%ld\n"
.LC8:
	.string	"average"
.LC9:
	.string	"diff_sq"
.LC10:
	.string	"set_bit"
.LC11:
	.string	"%ld (0x%lX)\n"
.LC12:
	.string	"set_bit_param"
.LC13:
	.string	"%ld [%ld (0x%lX)]\n"
.LC14:
	.string	"set_bit_val"
.LC15:
	.string	"func1"
.LC16:
	.string	"func2"
.LC17:
	.string	"func3"
	.align 8
.LC18:
	.string	"error - unrecognized command '%s'.\n"
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
	pushq	%rbx
	subq	$248, %rsp
	.cfi_offset 3, -24
	movl	%edi, -244(%rbp)
	movq	%rsi, -256(%rbp)
	cmpl	$2, -244(%rbp)
	jg	.L8
	movq	-256(%rbp), %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC4, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, %eax
	jmp	.L9
.L8:
	movl	-244(%rbp), %eax
	cltq
	movl	$8, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	%rax, -40(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L10
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$35, %edx
	movl	$1, %esi
	movl	$.LC5, %edi
	call	fwrite
	movl	$1, %eax
	jmp	.L9
.L10:
	movl	-244(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset
	movl	$2, -20(%rbp)
	jmp	.L11
.L12:
	movl	-20(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-256(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	leaq	0(,%rdx,8), %rcx
	movq	-32(%rbp), %rdx
	leaq	(%rcx,%rdx), %rbx
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	strtol
	movq	%rax, (%rbx)
	addl	$1, -20(%rbp)
.L11:
	movl	-20(%rbp), %eax
	cmpl	-244(%rbp), %eax
	jl	.L12
	movq	-256(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	-256(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movl	$.LC6, %edx
	movl	$7, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L13
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$1, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -64(%rbp)
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	mul_24
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L13:
	movq	-56(%rbp), %rax
	movl	$.LC8, %edx
	movl	$8, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L15
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$2, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -72(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -80(%rbp)
	movq	-80(%rbp), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	average
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L15:
	movq	-56(%rbp), %rax
	movl	$.LC9, %edx
	movl	$8, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L16
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$2, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -88(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	diff_sq
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L16:
	movq	-56(%rbp), %rax
	movl	$.LC10, %edx
	movl	$8, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L17
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$2, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -104(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -112(%rbp)
	movq	-112(%rbp), %rdx
	movq	-104(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	set_bit
	movq	%rax, -120(%rbp)
	movq	-120(%rbp), %rdx
	movq	-120(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC11, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L17:
	movq	-56(%rbp), %rax
	movl	$.LC12, %edx
	movl	$14, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L18
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$2, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -216(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -128(%rbp)
	movq	-128(%rbp), %rdx
	leaq	-216(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	set_bit_param
	movq	%rax, -136(%rbp)
	movq	-216(%rbp), %rcx
	movq	-216(%rbp), %rdx
	movq	-136(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC13, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L18:
	movq	-56(%rbp), %rax
	movl	$.LC14, %edx
	movl	$12, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L19
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$3, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -224(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -144(%rbp)
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rdx
	movq	-144(%rbp), %rcx
	leaq	-224(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	set_bit_val
	movq	%rax, -160(%rbp)
	movq	-224(%rbp), %rcx
	movq	-224(%rbp), %rdx
	movq	-160(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC13, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L19:
	movq	-56(%rbp), %rax
	movl	$.LC15, %edx
	movl	$6, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L20
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$1, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -168(%rbp)
	movq	-168(%rbp), %rax
	movq	%rax, %rdi
	call	func1
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L20:
	movq	-56(%rbp), %rax
	movl	$.LC16, %edx
	movl	$6, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L21
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$4, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -176(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -184(%rbp)
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -192(%rbp)
	movq	-40(%rbp), %rax
	movq	32(%rax), %rax
	movq	%rax, -200(%rbp)
	movq	-200(%rbp), %rcx
	movq	-192(%rbp), %rdx
	movq	-184(%rbp), %rsi
	movq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	func2
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L21:
	movq	-56(%rbp), %rax
	movl	$.LC17, %edx
	movl	$6, %ecx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	repz cmpsb
	seta	%dl
	setb	%al
	subl	%eax, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L22
	movl	-244(%rbp), %edx
	movq	-56(%rbp), %rsi
	movq	-48(%rbp), %rax
	movl	$3, %ecx
	movq	%rax, %rdi
	call	check_func_params
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -232(%rbp)
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -240(%rbp)
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -208(%rbp)
	movq	-208(%rbp), %rdx
	leaq	-240(%rbp), %rcx
	leaq	-232(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	func3
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L14
.L22:
	movq	-256(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC18, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$2, %eax
	jmp	.L9
.L14:
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
.L9:
	addq	$248, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.4.1 20200928 (Red Hat 8.4.1-1)"
	.section	.note.GNU-stack,"",@progbits
