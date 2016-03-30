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

# recursive algorithm - return calcNum[n-1] + calcNum[n-2] + calcnum[n-3]
# Argument in a0
calcNum:
	# First check for base cases
	li $t0, 0
	li $t1, 1
	li $t2, 2
	
	# Branch in case of base cases - return respective values
	beq $a0, $t0, r0
	beq $a0, $t2, r12
	beq $a0, $t1, r12
	
	
	# Need to save $ra and $a0 b/c recursion
	addi $sp, $sp, -16
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	
	# Get calcNum[n-1]
	addi $a0, $a0, -1
	jal calcNum #puts result into v0
	sw $v0 8($sp)
	
	# Getting calcNum[n-2]
	addi $a0, $a0, -1
	jal calcNum
	sw $v0, 12($sp)
	
	# Getting calcNum[n-3]
	addi $a0, $a0, -1
	jal calcNum
	
	lw $ra, 0($sp)
	lw $a0, 4($sp)
	lw $t4, 8($sp)
	lw $t5, 12($sp)
	addi $sp, $sp, 16
	
	# calcNum[n-3] is in v0
	# Add other results to v0
	add $v0, $v0, $t4
	add $v0, $v0, $t5
	
	# Check whether or not there was an overflow error
	# go back to beginning of program if there was
	li $t8, 0xC
	beq $t9, $t8, start
	
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
	andi $t9, $t9, 0xC # 12 = Arithmetic cause
	
	la   $a0, overflowStr  # address of string to print
	li   $v0, 4
	syscall
	#go back to instruction which caused
	eret
	
	
	
	
	
	
	
	