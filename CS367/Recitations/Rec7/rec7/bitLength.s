	.text
	.global bitLength
	.type   bitLength,@function

bitLength:

	# Assume that RDI = n 
	# ADD your code here
	
	incr:
		movq $0, %rsi
		jmp test
	top: 	
		incq %rsi
		shrq $1, %rdi
	test:
		cmpq $0, %rdi
		jg top
		movq %rsi, %rax

end:
	ret # Returns what is in RAX
	# END your code

	.size   bitLength, .-bitLength
