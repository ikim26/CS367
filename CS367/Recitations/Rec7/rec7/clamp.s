	.text
	.global clamp
	.type   clamp,@function


clamp:
	# Assume that RDI = x, RSI = a, and RDX = b
	# YOUR CODE HERE
	cmpq %rsi, %rdi
	jle aGreaterthanX
	cmpq %rdx, %rdi
	jge bLessthanX
	movq %rdi, %rax
	jmp end

	aGreaterthanX:
		movq %rsi, %rax
		jmp end
	bLessthanX:
		movq %rdx, %rax
		jmp end
	end:

	ret # returns what is in RAX
	# END YOUR CODE
	
	.size   clamp, .-clamp
