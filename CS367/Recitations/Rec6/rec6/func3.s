	.file	"func3.c"
	.text
	.globl	func3
	.type	func3, @function
func3:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rcx
	movq	(%rsi), %rax
	imulq	%rdx, %rcx
	movq	%rcx, (%rdi)
        subq    %rax, %rcx
        shlq    $1, %rcx
        movq	%rcx,%rax
	movq	%rax, (%rsi)
	ret
	.cfi_endproc
.LFE0:
	.size	func3, .-func3
	.ident	"GCC: (GNU) 6.3.1 20170216 (Red Hat 6.3.1-3)"
	.section	.note.GNU-stack,"",@progbits
