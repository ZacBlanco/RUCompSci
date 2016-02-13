.text 0x00400000
.align 2
.globl main
main:
	lw $a0, Size 		# load the size of array into $a0, using lw
	addi $a0, $a0, -1 	# init index j with size - 1
	li $a1, 0 			# init index i = 0
	li $t2, 4			# t2 contains constant 4, init t2
loop:
	mul $t1, $a1, $t2 	# t1 gets i*4
	mul $t4, $a0, $t2 	# t4 gets j*4
	lw $a3, Array1($t1) # a3 = Array[i]
	sll $a3, $a3, 1		# double the value a3 = a3 * 2
	sw $a3, Array1($t4)	# store a3 to Array1[j]
	addi $a1, $a1, 1	# i = i+1
	sub $a0, $a0, 1		# j = j-1
	bge $a1, $a0, END	#
	j loop				#
END:
	 li $v0, 10 # syscall code 10 is for exit
	 syscall # make the syscall
.data 0x10000000
.align 2
Array1: .word 2 5 6 7 12 16 25 27
Size: .word 8