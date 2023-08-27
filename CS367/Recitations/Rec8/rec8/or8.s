	.text
	.global or8
	.type   or8,@function
or8:

	# YOUR CODE HERE
	push %r12
	push %rbx
	push %r13
	push %r14
	push %r15
	
	orq %r12, %rdi
	orq %r13, %rdi
	orq %rbx, %rdi
	orq %r14, %rdi
	orq %r15, %rdi
	orq 0x4(%rbp), %rdi
	orq 0x8(%rbp), %rdi
	
	movq %rdi, %rax
	pop %r12	
	pop %rbx
	pop %r13
	pop %r14
	pop %r15

    	ret	
  	# END YOUR CODE
	
	.size   or8, .-or8
