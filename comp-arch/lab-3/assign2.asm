# Zachary Blanco
# Computer Architecture
.data
arr1: 	.space 2000
beginInput: .asciiz "Enter the length of the array of integers: "
inputStr: .asciiz "Please enter an integer:\n"
newline: .asciiz "\n"
spaceChar: .asciiz " "
theSortedArray: .asciiz "The sorted array is: "
inputNewNum: .asciiz "Please enter an integer to insert into the array: "

.text 

#### Descriptions of Variables ###
####
#### $s0 - The base address of 'arr'
#### $s2 - The length of the array


main:
	la $a0, beginInput
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	move $s2, $v0#Length of array
	
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
		la $a0, inputNewNum
		li $v0, 4
		syscall
		li $v0, 5
		syscall
		move $a2, $v0
		move $a0, $s0
		move $a1, $s2
		jal binarySearchAndInsert
		addi $s2 $s2, 1
		move $a0, $s0
		move $a1, $s2
		jal printArray
		j exit
		
		# Now we need to insert a new integer into our array using binary search
		
	j exit

exit:
	li $v0, 10
	syscall

# $a2 is search value
# lo is $t2
# hi is $t3
# mid is $t4
binarySearchAndInsert:
	move $a0, $t0 #The base of the array
	move $a1, $t1 # The length of the array
	li $t2, 0 		# The lo starts @ 0
	add $t3, $a1, -1	# The hi starts @ length - 1
	add $t4, $t3, $t2
	srl $t4, $t4, 1
	
	bsLoop:
		sub $t5, $t2, $t3
		bgtz $t5, stopped	  # if low is > hi then stop
		sll $t5, $t4, 2
		add $t5, $t0, $t5 
		lw $t5, 0($t5)
		beq $t5, $a2, stopped # Check if arr[mid] == searchval
		sub $t5, $t5, $a2
		bltz $t5, bsLower
		bsElse:
			addi $t3, $t4, -1
			j bsAfterCheck
		bsLower:
			addi $t2, $t4, 1
		bsAfterCheck:
			add $t4, $t3, $t2
			srl $t4, $t4, 1
			j bsLoop
	
	
	stopped:
		sll $t5, $t4, 2
		add $t5, $t0, $t5
		lw $t5, 0($t5) #t5 is the value which mid exited at
		beq $t5, $a2, foundValue
		valueNotExists:
			# we should replace everything starting at lo instead
			add $t5, $t2, $zero
			sll $t5, $t5, 2 #memory offset for mid+1
			add $t6, $t5, $zero # put memory offset of mid+1 into t6
			add $t7 $t6, $t0 # memory location for the mid+1
			lw $t8, 0($t7) #store temp in t8
			sw $a2, 0($t7)
			move $t9, $t1
			sll $t9, $t9, 2 ##address of offset
			add $t9, $t9, $t0 # absolute address 
			addi $t7, $t7, 4
			insertLoop:
				bgt $t7, $t9, endLoop
				move $t6, $t8
				lw $t8, 0($t7)
				sw $t6, 0($t7)
				addi $t7, $t7, 4
				j insertLoop
		foundValue:
			#t4 is the index
			# we want to put $a2 into mid+1
			addi $t5, $t4, 1
			sll $t5, $t5 2 #memory offset for mid+1
			add $t6, $t5, $zero # put memory offset of mid+1 into t6
			add $t7 $t6, $t0 # memory location for the mid+1
			lw $t8, 0($t7) #store temp in t8
			sw $a2, 0($t7)
			move $t9, $t1
			sll $t9, $t9, 2 ##address of offset
			add $t9, $t9, $t0 # absolute address 
			addi $t7, $t7, 4
			insertLoop2:
				bgt $t7, $t9, endLoop
				move $t6, $t8
				lw $t8, 0($t7)
				sw $t6, 0($t7)
				addi $t7, $t7, 4
				j insertLoop2
				
			
			
			
			
	endLoop:
		jr $ra

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