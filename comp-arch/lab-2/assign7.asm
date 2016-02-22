.data
arr1: 	.space 60
beginInput: .asciiz "Enter 10 integers:\n"
inputStr: .asciiz "Please enter an integer:\n"
numPos: .asciiz "Number of Positive Integers: "
numNeg: .asciiz "Number of Negative Integers: "
numZero: .asciiz "Number of Zeros: "
median: .asciiz "The median of the Array is: "
newline: .asciiz "\n"
spaceChar: .asciiz " "
theSortedArray: .asciiz "The sorted array is: "

.text 

#### Descriptions of Variables ###
####
#### $s0 - The base address of 'arr'
#### $s2 - The length of the array


main:
	la $a0, beginInput
	li $v0, 4
	syscall
	li $s2, 10 #Length of array
	
	# We're going to now get values into the array
	la $s0, arr1
	move $s1, $s0
	li $t1, 0
	inputLoop:
		sub $t2, $t1, $s2
		bgez $t2, endInput # Exit input if we have all inputs
		la $a0, inputStr
		li $v0, 4
		syscall
		li $v0, 5		# Read an integer
		syscall
		move $t9, $v0
		sw $v0, 0($s1)
		addi $s1, 4
		addi $t1, 1
		j inputLoop


	endInput:
		move $a0, $s0
		move $a1, $s2
		jal bubbleSort
		move $a0, $s0 # Finished Sorting Array
		jal printArray
		move $t0, $v0
		move $a0, $s0
		jal printMedian
		move $t1, $v0
		move $a0, $s0
		jal printPositives
		move $t2, $v0
		move $a0, $s0
		jal printNegatives
		move $a0, $s0
		jal printZeros
		
	j exit

bubbleSort:
	li $t0, 1
	outerPass:
		li $t0, 0
		li $t1, 0 # i variable
		innerPass:
			add $t3, $t1, $t1
			add $t3, $t3, $t3 # Multiply t1 by 4
			add $t3, $a0, $t3 # Add 4*i to base address of arr
			addi $t4, $t3, 4  # [i+1]
			lw $t5, 0($t3)	   # $t5 and $t6 store actual array values.
			lw $t6, 0($t4)
			sub $t7, $t5, $t6
			bgtz $t7, switchPlaces
			j afterSwitch
			switchPlaces: #switches location i with i+1
				sw $t6, 0($t3)
				sw $t5, 0($t4)
				li $t0, 1
			afterSwitch:
				addi $t1, $t1, 1
				sub $t8, $t1, $s2
				addi $t8, $t8, 1
				bltz $t8, innerPass		
	bgtz $t0, outerPass
	jr $ra

printMedian:
	la $t0, 0($a0) # Base address of array in t0
	move $t1, $a1 # Length of Array in $t1
	
	li $t2, 2
	div $t1, $t2 #divide t2 by 2
	mfhi $t3  # check whether t3 is zero or 1 not.
			# t3 == 1 - then median is location [t3/2 + 1]
			# t3 == 0 - then median is average of [t3/2] and [t3/2 + 1]
	beq $t3, $zero, medAvg
	j medVal
	medVal:
		mflo $t3
#		addi $t3, $t3, 1
		li $t4, 4
		mul $t3, $t3, $t4
		add $t3, $t3, $t0
		lw, $t5, 0($t3)
		j raPrintMed
	medAvg:
		mflo $t6 # Getting median
		move $t7, $t6
		addi $t6, $t6, -1
		li $t4, 4
		mul $t6, $t6, $t4
		mul $t7, $t7, $t4
		add $t6, $t6, $t0
		add $t7, $t7, $t0
		lw $t3, 0($t6)
		lw $t4, 0($t7)
		add $t4, $t4, $t3
		div $t4, $t2
		mflo $t5
		j raPrintMed
	raPrintMed:
		la $a0, median
		li $v0, 4
		syscall
		li $v0, 1
		move $a0, $t5
		syscall
		la $a0, newline
		li $v0, 4
		syscall
		jr $ra

printNegatives:
	la $t0, 0($a0)
	lui $t1, 0
	li $t6, 0
	loopNegatives:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		bgez $t3, notNeg
		addi $t6, $t6, 1
		notNeg:
		addi $t1, 1
		sub $t4, $s2, $t1
		bgtz $t4, loopNegatives
		
	printNumNeg:
		la $a0, numNeg
		li $v0, 4
		syscall
		li $v0, 1
		move $a0, $t6
		syscall
		la $a0, newline
		li $v0, 4
		syscall
	
	jr $ra

printPositives:
	la $t0, 0($a0)
	lui $t1, 0
	lui $t6, 0
	loopPositives:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		blez $t3, notPos
		addi $t6, $t6, 1
		notPos:
		addi $t1, 1
		sub $t4, $s2, $t1
		bgtz $t4, loopPositives
	
	printNumPos:
		la $a0, numPos
		li $v0, 4
		syscall
		li $v0, 1
		move $a0, $t6
		syscall
		la $a0, newline
		li $v0, 4
		syscall
	
	jr $ra


printZeros:
	la $t0, 0($a0)
	lui $t1, 0
	lui $t6, 0
	loopZeros:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		bne $t3, $zero, nonZero
		addi $t6, $t6, 1
		nonZero:
		addi $t1, 1
		sub $t4, $s2, $t1
		bgtz $t4, loopZeros
	
	prZeros:
		la $a0, numZero
		li $v0, 4
		syscall
		li $v0, 1
		move $a0, $t6
		syscall
		la $a0, newline
		li $v0, 4
		syscall
	
	jr $ra


printArray:
	la $t0, 0($a0)
	lui $t1, 0
	la $a0, theSortedArray
	li $v0, 4
	syscall
	printArrIndex:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		move $a0, $t3
		li $v0, 1
		syscall
		la $a0, spaceChar
		li $v0, 4
		syscall
		addi $t1, 1
		sub $t4, $s2, $t1
		bgtz $t4, printArrIndex
	
	la $a0, newline
	li $v0, 4
	syscall
	jr $ra

exit:
	li $v0, 10
	syscall

















