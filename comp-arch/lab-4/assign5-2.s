#####################
# Zachary Blanco
# Comp Arch Lab 4
# Assignment 3
# Section 1A
#####################
.data
inputStr: .asciiz "Enter an input, n: "
outputStr: .asciiz "Output: "
overflowStr: .asciiz "Arthmetic Overflow\n"
newline: .asciiz "\n"

.text
main:

	start:
	# Get input
	li $t9, 0 # Set exception to 0
	la $a0, inputStr
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	
	# Move input to a0 and call function
	add $a0, $v0, $zero
	jal calcNum
	
	#v0 now contains result - print
	add $t1, $v0, $zero
	
	la $a0, outputStr
	li $v0, 4
	syscall
	
	add $a0, $t1, $zero
	li $v0, 1
	syscall
	
	la $a0, newline
	li $v0, 4
	syscall

exit:
	li $v0, 10
	syscall

## Calculate a[n] ## (Argument in $a0)
calcNum:
	# First check for base cases
	li $t0, 0
	li $t1, 1
	li $t2, 2
	
	# Branch in case of base cases - return respective values
	beq $a0, $t0, r0
	beq $a0, $t2, r12
	beq $a0, $t1, r12
	
	li $t0, 3 # set counter (i) equal to 3 b/c it we determined n > 2
	li $t1, 0
	li $t2, 1
	li $t3, 1
	li $t4, 0
	calcLoop:
		#calculate a[i]
		add $t4, $t3, $t2
		add $t4, $t4, $t1
		
		# Overflow can occur here
		# Check whether or not there was an overflow error
		# go back to beginning of program if there was overflow
		li $t8, 0xC
		beq $t9, $t8, start
		
		#shift over
		add $t1, $t2, $zero
		add $t2, $t3, $zero
		add $t3, $t4, $zero
		
		addi $t0, $t0, 1 # Increase the counter
		
		
		ble $t0, $a0, calcLoop # Branch back if we haven't reached n
	
	add $v0, $t3, $zero #Move result to correct register
	
	j calcNumReturn
	
	r0:
		li $v0, 0
		j calcNumReturn
		
	r12:
		li $v0, 1
		j calcNumReturn
	
	
	calcNumReturn:
		jr $ra


.ktext 0x80000180
	#Print Error
	mfc0 $t9, $13
	srl $t9, $t9, 2
	andi $t9, $t9, 0xC # 12 = Arithmetic Overflow cause
	
	la   $a0, overflowStr  # address of string to print
	li   $v0, 4
	syscall
	#go back to instruction which caused
	
	mfc0 $k0,$14   # Coprocessor 0 register $14 has address of trapping instruction
	addi $k0,$k0,4 # Add 4 to point to next instruction
	mtc0 $k0,$14   # Store new address back into $14

	eret








