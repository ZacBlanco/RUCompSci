.data
inputStr: .asciiz "Please enter a number n to estimate e:"
outputStr: .asciiz "Approximation of e: "

.text

main:
	
	la $a0, inputStr
	li $v0, 4
	syscall # Output prompt string
	
	li $v0, 5 
	syscall # Read Input
	
	addi $t9, $v0, 0
	
	li $t2, 0 # counter for 1-n
	li.s $f6, 0.0
	sumLoop:
		# Approximation of e stored in $t1
		
		
		li $t0, 1
		li.s $f0, 1.0 # counter variable
		li.s $f1, 1.0 # product holder
		li.s $f5, 1.0 # Store 1.0 in $f5 - won't change
		factLoop:
		
			mul.s $f1, $f1, $f0
			add.s $f0, $f0, $f5
			addi $t0, $t0, 1
			
		ble, $t0, $t2, factLoop
		
		endFactLoop:
		
		# Calculate 1/factorial and add it to the result
		li.s $f2, 1.0
		div.s $f2, $f2, $f1
		
		add.s $f6, $f6, $f2
		
		addi $t2, $t2, 1
		
		ble $t2, $t9, sumLoop
		
	la $a0, outputStr
	li $v0, 4
	syscall
	
	mov.s $f12, $f6
	li $v0, 2
	syscall

exit:
	li $v0, 10
	syscall 