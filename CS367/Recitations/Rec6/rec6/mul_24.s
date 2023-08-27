.text
.global mul_24
	.type   mul_24,@function
mul_24:

	# RDI = x
	# YOUR CODE HERE
	movq %rdi, %rcx
	shlq $5, %rcx
	movq %rdi, %rsi
	shlq $3, %rsi
	subq %rsi, %rcx
	movq %rcx, %rax
	ret
	# END YOUR CODE
	
	.size   mul_24, .-mul_24
