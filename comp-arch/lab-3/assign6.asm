###############################
# Zachary Blanco - Section 1A #
# Student ID -     1580007117 #
# NetID - 	    zdb5       #
###############################
.data
lengthStr: .asciiz "How many numbers would you like to input?\n"
inputStr:  .asciiz "Please enter an input: "
meanStr: 	 .asciiz "The mean is: "
stdDevStr: .asciiz "The standard deviation is: "
newline:   .asciiz "\n"
.align 2
arr1: .space 1000


.text
main:
	la $a0, lengthStr
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	move $s2, $v0 # Length of array
	
#	la $a0, inputStr
#	li $v0, 4
#	syscall
#	
	
	# We're going to now get values into the array
	la $s0, arr1
	move $s1, $s0
	li $t1, 0 #counter variable
	inputLoop:
		sub $t2, $t1, $s2
		bgez $t2, endInput # Exit input if we have all inputs
		la $a0, inputStr
		li $v0, 4
		syscall
		li $v0, 6		# Read a float
		syscall
		swc1 $f0, 0($s1)
		addi $s1, 4
		addi $t1, 1
		j inputLoop
	
	
	endInput:
		la $a0, meanStr
		li $v0, 4
		syscall
		jal calcMean #stores result into f0
		mov.s $f12, $f0
		li $v0, 2 #prints float in f0
		syscall
		la $a0, newline
		li $v0, 4
		syscall
		
		
		mov.s $f1, $f0 #put mean in f1 for stdDeviation
		
		la $a0, stdDevStr
		li $v0, 4
		syscall
		jal calcStdDeviation #stores result into f0
		mov.s $f12, $f0
		li $v0, 2 #prints float in f0
		syscall
		la $a0, newline
		li $v0, 4
		syscall
		j exit

exit:
	li $v0, 10
	syscall
		

calcMean:
	#sum of all in f5
	#num is in s2
	#move s2 to f2
	#divide sum by f2, return in f0
	mtc1 $s2, $f2 ## total num of items
	cvt.s.w $f2, $f2
	li.s $f5, 0.0
	li $t1, 0 #counter
	li $t3, 0
	sumLoop:
		sub $t2, $t1, $s2
		bgez $t2, sumExit # Exit input if we have all inputs
		sll $t2, $t1, $t1
		add $t4, $t3, $s0
		l.s $f1, 0($t4)
		add.s $f5, $f5, $f1
		addi $t3, $t3, 4
		addi $t1, $t1, 1
		j sumLoop
	
	sumExit:
		div.s $f7, $f5, $f2
		mov.s $f0, $f7
		jr $ra

calcStdDeviation:
	# mean stored in f1
	mtc1 $s2, $f2 ## total num of items
	cvt.s.w $f2, $f2
	li.s $f5, 0.0
	li $t1, 0 # counter
	li $t3, 0
	sLoop:
		sub $t2, $t1, $s2
		bgez $t2, sExit # Exit input if we have all inputs
		sll $t2, $t1, $t1
		add $t4, $t3, $s0
		l.s $f3, 0($t4)
		
		#got the number - now we need difference between that and mean
		sub.s $f6, $f1, $f3 #f6 stores the difference
		#need to make it positive

		#Square it
		mul.s $f6, $f6, $f6
		
#		li.s $f20, 0.0
#		c.le.s $f20, $f6
#		bc1t posDiff
#		li.s $f19, -1.0
#		mul.s $f6, $f6, $f19
		
		posDiff:
			add.s $f5, $f5, $f6
			addi $t3, $t3, 4
			addi $t1, $t1, 1
			
		j sLoop
	
	sExit:
		div.s $f7, $f5, $f2
		mov.s $f0, $f7
		li.s $f7, 0.0
		c.le.s $f0, $f7 # check if our variance is 0
		bc1t returnSq
		addi $sp, $sp, -4
		sw $ra, 0($sp)
		jal squareRoot # Gets square root of f0 and returns in f0
		lw $ra, 0($sp)
		addi $sp, $sp, 4
		returnSq:
		jr $ra
		
squareRoot:
	li.s $f15, 1.5
	li.s $f14, 0.5
	mul.s $f9, $f0, $f14 # multiply input by 0.5
	mfc1 $t1, $f0
	li $t5, 0x5f3759df
	srl $t1, $t1, 1
	sub $t1, $t5, $t1
	mtc1 $t1, $f10
	
	#Run newton's method 10 times for accuracy
	li $t9, 0
	li $t8, 10
	nMethod:
	# Newtons method - increase accuracy
	bgt $t9, $t8, sqEnd
	mul.s $f11, $f10, $f10
	mul.s $f11, $f11, $f9
	sub.s $f12, $f15, $f11
	mul.s $f10, $f10, $f12
	addi $t9, $t9, 1
	j nMethod
	
	sqEnd:
	
	li.s $f1, 1.0
	div.s $f16, $f1, $f10
	mov.s $f0, $f16
	jr $ra
