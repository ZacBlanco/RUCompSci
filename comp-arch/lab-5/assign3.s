### Computer Architecture Lab ###
# Name: Zachary Blanco
# RUID: 158007117
# netID: zdb5
# Section: 1A
### Computer Architecture Lab ###
.data 0x10000860
Vector_X: .word 1, 2, 3, 4, 5, 6, 7
.data 0x10000880
Vector_Y: .word 4, 5, 6, 7, 8, 9, 10
.data 0x10000C80
Matrix_T: 	.word 0, -4, -7, 2, -6, 5, 3
.data 0x10001080
.word 4, 0, -5, -1, 3, -7, 6
.data 0x10001480
.word 7, 5, 0, -6, -2, 4, -1
.data 0x10001880
.word -2, 1, 6, 0, -7, -3, 5
.data 0x10001C80
.word 6, -3, 2, 7, 0, -1, -4
.data 0x10002080
.word -5, 7, -4, 3, 1, 0, -2
.data 0x10002480
.word -3, -6, 1, -5, 4, 2, 0
.data 0x10002880
Vector_Z: .word 0, 0, 0, 0, 0, 0, 0
pl: .asciiz "+"
nl: .asciiz "\n"

.text 0x00400000
.globl main
	# Need to calculate Z where each value of Z is dot product
	# i.e z_1 = Y1*T11 + Y2*T12 + ... Y7*T17
	
	
	add $t0, $zero, $zero #Initialize counter for T and Y to 0
	add $t1, $zero, $zero # initialize counter for Z to 0
	addi $t7, $zero, 7 # set limit to set
	la $s0, Matrix_T #address of row(0+1) of matrix T
	la $s1, Vector_Y
	la $s2, Vector_Z
	
	loopZ:
		add $t0, $zero, $zero #Initialize counter for T and Y to 0
		add $t6, $zero, $zero # sum for row
		add $t2, $zero, $zero # set offset to 0
		loopXY:
			add $t3, $t2, $s0
			add $t4, $t2, $s1
			lw $t3, 0($t3)
			lw $t4, 0($t4)
			mul $t5, $t3, $t4
############ CODE TO PRINT RESULTS ##############
#			add $a0, $t5, $zero #print			#
#			li $v0, 1							#
#			syscall								#
#			la $a0, pl							#
#			li $v0, 4							#
#			syscall								#
############ CODE TO PRINT RESULTS ##############
			add $t6, $t6, $t5
			addi $t0, $t0, 1
			addi $t2, $t2, 4
			blt $t0, $t7, loopXY
		sw $t6, 0($s2)
############ CODE TO PRINT RESULTS ##############
#			la $a0, nl							#
#			li $v0, 4							#
#			syscall								#
#			add $a0, $t6, $zero #print			#
#			li $v0, 1							#
#			syscall								#
#			la $a0, nl							#
#			li $v0, 4							#
#			syscall								#
############ CODE TO PRINT RESULTS ##############
		addi $t1, $t1, 1
		addi $s2, $s2, 4
		addi $s0, $s0, 1024
		blt $t1, $t7, loopZ
	
	
	exit:
	li $v0, 10
	syscall
	