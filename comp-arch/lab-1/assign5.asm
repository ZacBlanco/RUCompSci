# Read a number of ints and multiply them together

.text
main:
	li $v0, 5 		# Read n number of times
	syscall	
	move $t1, $v0	# store our n in t1
	li $t2, 1       # Load value 1 into $t2 to store result 
	
loop:
	blez $t1, exit # If t1 (n) is  <= 0 exit
	
	li $v0, 5      # Read an int from the user
	syscall
	
	mul $t2, $v0, $t2 #multiply our result register by input
	sub $t1, $t1, 1   #decrement $t1 (n)
	j loop            # loop back again

exit:
	move $a0, $t2     # Move result to argument register
	li $v0, 1         # load correct v0 to print result
	syscall
	
	li $v0, 10        # exit
	syscall