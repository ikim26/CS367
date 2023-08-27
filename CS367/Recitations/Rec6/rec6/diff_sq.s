.text
.global diff_sq
	.type   diff_sq,@function
diff_sq:

	# RDI = x, RSI = y
	# YOUR CODE HERE
	movq %rdi, %rdx		#x -> rdx
	movq %rsi, %rcx		#y -> rcx

	imulq %rdx, %rdx	#x = x^2
	imulq %rcx, %rcx	#y = y^2
	
	subq %rcx, %rdx
	movq %rdx, %rax

  	ret
	# END YOUR CODE
	
	.size   diff_sq, .-diff_sq
