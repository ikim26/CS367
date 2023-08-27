	.text
	.global call_or8
	.type   call_or8,@function
call_or8:

	# YOUR CODE HERE
	sub 0x18, %rsp
	movq $0xa, %rdi
	movq $0x14, %rsi
	movq $0x1d, %rdx
	movq $0x28, %rcx
	movq $0x32, %r8
	movq $0x3c, %r9
	pushq $0x46
	pushq $0x50

	callq or8
	add 0x18, %rsp
	ret
	# END YOUR CODE
	
	.size   call_or8, .-call_or8
