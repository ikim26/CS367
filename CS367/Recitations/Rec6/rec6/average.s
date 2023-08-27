	.text
.global average
	.type   average,@function
average:

	# RDI = a, RSI = b
	# YOUR CODE HERE
	movq %rdi, %rdx	   #a-> rdx
	movq %rsi, %rcx    #b-> rcx

	addq %rcx, %rdx    #a = a + b
	shrq $1, %rdx	   #a/2
	movq %rdx, %rax

	ret
	# END YOUR CODE
	
	.size   average, .-average
