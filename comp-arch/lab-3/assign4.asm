###############################
# Zachary Blanco - Section 1A #
# Student ID -     1580007117 #
# NetID - 	    zdb5		#
###############################
.data
input1: .asciiz "input values for m, n, and p in mx^2 + nx +p = 0\n"
input2: .asciiz "m: "
input3: .asciiz "n: "
input4: .asciiz "p: "
input5: .asciiz "Enter a starting number for x_0: "
output: .asciiz "There is a root at: "
noRoot: .asciiz "No root was found for this equation"

.text

main:
	la $a0, input1
	li $v0, 4
	syscall
	# m = f3
	# n= f4
	# p = f5
	la $a0, input2
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	mov.s $f3, $f0
	la $a0, input3
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	mov.s $f4, $f0
	la $a0, input4
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	mov.s $f5, $f0
	la $a0, input5
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	#put a counter in t9 to check for large values
	li $t9, 0
	#put a max value here
	li $t8, 0xFFF0
	mov.s $f6, $f0
	recursiveLoop:
		# xi = $f6
		# xi+1 = $f7
		# x^2 = f8
		mul.s $f8, $f6, $f6
		mul.s $f8, $f3, $f8 # mx^2
		mul.s $f9, $f4, $f6 # nx
		add.s $f9, $f8, $f9 # mx^2 + nx + p
		add.s $f9, $f5, $f9 # mx^2 + nx + p ($f5)
		li.s $f2, 2.0
		mul.s $f10, $f2, $f6
		mul.s $f10, $f10 $f3
		add.s $f10, $f10, $f4
		div.s $f9, $f9, $f10
		sub.s $f7, $f6, $f9
		li.s $f20, 0.00001
		
		# Get absolute value of x_i - x_{i+1}
		sub.s $f15, $f7, $f6
		c.le.s $f7, $f6
		bc1f absVal
		li.s $f14, -1.0
		mul.s $f15, $f14, $f15
		absVal:
			c.lt.s $f15, $f20
			bc1t finR
		mov.s $f6 $f7
		bgt $t9, $t8, nort
		addi $t9, $t9, 1
		j recursiveLoop
		
	#finished calculating
	finR:
		mov.s $f12, $f7
		la $a0, output
		li $v0, 4
		syscall
		li $v0, 2
		syscall
		j exit
	nort:
		la $a0, noRoot
		li $v0, 4
		syscall
		j exit
	
exit:
	li $v0, 10
	syscall
	