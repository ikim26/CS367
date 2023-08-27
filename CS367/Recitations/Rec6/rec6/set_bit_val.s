	.text
	.global set_bit_val
	.type  set_bit_val,@function
set_bit_val:

	# RDI = &flags, RSI = n, RDX = b
	# YOUR CODE HERE

  	ret
	# END YOUR CODE
	
	.size   set_bit_val, .-set_bit_val
