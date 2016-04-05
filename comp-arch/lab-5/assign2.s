### Computer Architecture Lab ###
# Name: Zachary Blanco
# RUID: 158007117
# netID: zdb5
# Section: 1A
### Computer Architecture Lab ###
.data 0x10000480
A: .word 1, 2, 3
B: .word 8, 7, 6
C: .space 36
sp: .asciiz ", "

.text
main:
 # Calculate the Kronecker Product of A and B
	la $t0, A #A(0) is in t0
	la $t1, B #B(0) is in t1
	la $s0, C #C(0) not set in t2
	add $t2, $zero, $zero # Counter A
	add $t3, $zero, $zero # Counter B
	addi $t4, $zero, 3 # length of A and B
	loopA:
		loopB:
			lw $t5, 0($t0) #load A(i)
			lw $t6, 0($t1) #load B(j)
			mul $t7, $t5, $t6 # C(k) = A(i)*B(j)
			sw $t7, 0($s0) #put C(k) into memory
############ CODE TO PRINT RESULTS ##############
#			add $a0, $t7, $zero #print			#
#			li $v0, 1							#
#			syscall								#
#			la $a0, sp							#
#			li $v0, 4							#
#			syscall								#
############ CODE TO PRINT RESULTS ##############
			addi $t3, $t3, 1
			addi $t1, $t1, 4
			addi $s0, $s0, 4
			blt $t3, $t4, loopB
		add $t3, $zero, $zero #set counter B to 0
		la $t1, B #set B location back to 0
		addi $t0, $t0, 4 #increase position of A
		addi $t2 $t2, 1 #increase counter of A
		blt $t2, $t4, loopA 
			
	exit:
		li $v0, 10
		syscall
	