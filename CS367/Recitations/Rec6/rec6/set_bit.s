	.text
.global set_bit
	.type   set_bit,@function
set_bit:

	# RDI = flags, RSI = n
	# YOUR CODE HERE
	movq %rsi, %rcx		#n -> cl
	movq %rdi, %rdx		#flags -> rdx
	
	movq $1, %r9
	shlq %cl, %r9

	addq %r9, %rdx
	movq %rdx, %rax

	ret 
	# END YOUR CODE
	
	.size   set_bit, .-set_bit
