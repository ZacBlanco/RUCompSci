.data
sinhStr: .asciiz "sinh^(-1)(x): "
tanhStr: .asciiz "tanh^(-1)(x): "
inputStr: .asciiz "Please enter an input, x: "
newline: .asciiz "\n"

.text

main:
	
	la $a0, inputStr
	li $v0, 4
	syscall
	
	li $v0, 6
	syscall # $f0 now contains the input - assuming input is [0, pi/2]
	
	# $f0 will not be used from now on. - It is the argument for sinh/tanh
	jal calcSinh
	
	# Result stored in $f1
	la $a0, sinhStr
	li $v0, 4
	syscall
	mov.s $f12, $f1
	li $v0, 2
	syscall
	la $a0, newline
	li $v0, 4
	syscall
	
	jal calcTanh
	# Result stored in $f1
	la $a0, tanhStr
	li $v0, 4
	syscall
	mov.s $f12, $f1
	li $v0, 2
	syscall
	la $a0, newline
	li $v0, 4
	syscall
	
	j exit

exit:
	li $v0, 10
	syscall
	
# Argument in f0, result in f1
# Because we only need the first 4 terms we don't need to use the formula to approximate
# We can simply use the constants from the 1st 4 terms.
calcSinh:
	
	li.s $f5, -1.0 # Negative 1 constant
	
	mov.s $f3, $f0
	mov.s $f4, $f3
	mul.s $f4, $f4, $f4
	mul.s $f4, $f4, $f3 # $f4 now holds x^3
	mul.s $f4, $f5, $f4 # $f4 now holds -x^3
	li.s $f6, 6.0
	div.s $f4, $f4, $f6 # $f4 now holds -x^3/6
	
	mov.s $f7, $f3
	mul.s $f7, $f7, $f7
	mul.s $f7, $f7, $f7 # $f7 now holds x^4
	mul.s $f7, $f7, $f3 # $f7 now holds x^5
	li.s $f6, 3.0
	mul.s $f7, $f7, $f6 # $f7 now holds 3x^5
	li.s $f6, 40.0
	div.s $f7, $f7, $f6 # $f7 now holds 3x^5/40
	
	# 4th term is -5x^7/112
	mov.s $f8, $f3
	mul.s $f8, $f8, $f8
	mul.s $f8, $f8, $f8 # $f8 now holds x^4
	mul.s $f8, $f8, $f8 # $f8 now holds x^8
	div.s $f8, $f8, $f3 # $f8 now holds x^7
	li.s $f6, 5.0
	mul.s $f8, $f8, $f6 # $f7 now holds 5x^7
	li.s $f6, 112.0
	div.s $f8, $f8, $f6 # $f7 now holds 5x^7/112
	mul.s $f8, $f8, $f5 # $f7 now holds -5x^7/112
	
	add.s $f1, $f3, $f4
	add.s $f1, $f1, $f7
	add.s $f1, $f1, $f8

	jr $ra


# Argument in f0, result in f1
calcTanh:
	
	mov.s $f3, $f0 # x
	mul.s $f4, $f4, $f4 # x^2
	
	mul.s $f4, $f4, $f3 # x^3
	
	mul.s $f7, $f4, $f4 # x^6
	div.s $f7, $f7, $f3 # x^5
	
	mul.s $f8, $f7, $f3 #x^6
	mul.s $f8, $f8, $f3 #x^7
	
	li.s $f5, 3.0
	div.s $f4, $f4, $f5 # x^3/3
	
	li.s $f5, 5.0
	div.s $f7, $f7, $f5 # x^5/5
	
	li.s $f5, 7.0
	div.s $f8, $f8, $f5 # x^7/7
	
	add.s $f1, $f3, $f4
	add.s $f1, $f1, $f7
	add.s $f1, $f1, $f8
	
	
	jr $ra
	
	
	
	
	
	
	
	
	
	
	
	
	
	