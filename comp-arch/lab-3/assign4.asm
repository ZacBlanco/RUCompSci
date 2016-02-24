.data
input1: .asciiz "input values for m, n, and p in mx^2 + nx +p = 0"
input2: .asciiz "m: "
input3: .asciiz "n: "
input4: .asciiz "p: "
input5: .asciiz "Enter a starting number for x_0: "


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
	move $f3, $f0
	la $a0, input3
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	move $f4, $f0
	la $a0, input4
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	move $f5, $f0
	la $a0, input5
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	move $f6, $f0
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
		div.s $f9, $f10, $f9
		sub.s $f7, $f6, $f9
		li.s $f20, 0.00001
		
		bltz $
		j recursiveLoop
		
	li.s	
	
	j exit
	
exit:
	li $v0, 10
	syscall
	