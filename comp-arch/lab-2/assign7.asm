.data
#arr: 	.space 2000
arr:		.word 1, 5, 9, 7, 8, 3, 5, 0xF
inputStr: .asciiz "Please enter an integer. End input by typing 0xF:\n"
numPos: .asciiz "Number of Positive Integers"
numNeg: .asciiz "Number of Negative Integers"
median: .asciiz "The median of the Array is:"

.text 

#### Descriptions of Variables ###
####
#### $s0 - The base address of 'arr'
#### $s1 - The memory pointer to a location of 'arr'
#### $s2 - The length of the array


main:
	la $a0, inputStr
	li $v0, 4
	syscall
	li $s2, 7
	la $s0, arr
	# We're going to now get values into the array
#	la $s0, arr
#	move $s1, $s0
#	li $s2, 0
#	inputLoop:
#		li $v0, 5		# Read an integer
#		syscall
#		move $t9, $v0
#		beq $v0, 0xF, endInput # Exit input if input value is 0xF
#		sw $v0, 0($s1)
#		addi $s1, 4
#		addi $s2, 1
#		j inputLoop
#	
	endInput:
		move $a0, $s0
		move $a1, $s2
		jal bubbleSort
		move $a0, $s0
		jal printArray
#		move $t0, $v0
#		jal getMedian
#		move $t1, $v0
#		jal getPositives
#		move $t2, $v0
#		jal getNegatives
#		move $t3, $v0
#		jal printArray
	j exit

bubbleSort:
	li $t0, 1
	# $t9 ---> Temp variable for swap
	outerPass:
		li $t0, 0
		li $t1, 0 # i variable
		innerPass:
			sll $t3, $t1, 2 # [i]
			add $t3, $a0, $t1
			addi $t4, $t3, 4  # [i+1]
			lw $t5, 0($t3)
			lw $t6, 0($t4)
			sub $t7, $t5, $t6
			bltz $t7, switchPlaces
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

getMedian:


getPositives:


getNegatives:


printArray:
	la $t0, 0($a0)
	lui $t1, 0
	printArrIndex:
		sll $t3, $t1, 2
		lw $t3, 0($t0)
		move $a0, $t3
		li $v0, 4
		syscall
		addi $t1, 1
		sub $t4, $s2, $t1
		bgtz $t4, printArrIndex
	jr $ra

exit:
	li $v0, 10
	syscall

















