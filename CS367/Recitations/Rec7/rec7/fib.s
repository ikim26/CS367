	.text
	.global fib
	.type   fib,@function

fib:
	# Assume that RDI = n
	# ADD your code here

end:
	ret # Returns what is in RAX
	# END your code


	.size   fib, .-fib
