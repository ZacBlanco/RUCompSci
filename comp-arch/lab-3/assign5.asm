###############################
# Zachary Blanco - Section 1A #
# Student ID -     1580007117 #
# NetID - 	    zdb5		#
###############################
.data
input1: .asciiz "Please input a value for the radius of the cone: "
input2: .asciiz "Please input a value for the height of the cone: "
outputS: .asciiz "Surface Area: "
outputV: .asciiz "Volume: "
newline: .asciiz "\n"

.text

main:
	# Load pi as a constant
	li.s $f3, 3.14159265359	
	
	# Print the input prompt for radius
	la $a0, input1
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	mov.s $f20, $f0
	
	# Print the input prompt for height
	la $a0, input2
	li $v0, 4
	syscall
	li $v0, 6
	syscall
	mov.s $f21, $f0
	
	# Calculate and print surface area
	jal calcSurf
	la $a0, outputS
	li $v0, 4
	syscall
	li $v0, 2
	mov.s $f12, $f0
	syscall
	la $a0, newline
	li $v0, 4
	syscall
	
	# Calculate and print volume
	jal calcVol
	la $a0, outputV
	li $v0, 4
	syscall
	li $v0, 2
	mov.s $f12, $f0
	syscall
	

exit:
	li $v0, 10
	syscall
	
# Fast and easy way to calculate a square root
# Calculate of the square root of $f0
# Argument in $f0
# Algorithm found at https://en.wikipedia.org/wiki/Fast_inverse_square_root
squareRoot:
	li.s $f15, 1.5
	li.s $f14, 0.5
	mul.s $f9, $f0, $f14 # multiply input by 0.5
	mfc1 $t1, $f0
	li $t5, 0x5f3759df
	srl $t1, $t1, 1
	sub $t1, $t5, $t1
	mtc1 $t1, $f10
	
	# Newtons method - increase accuracy
	mul.s $f11, $f10, $f10
	mul.s $f11, $f11, $f9
	sub.s $f12, $f15, $f11
	mul.s $f10, $f10, $f12
	# Newtons method - increase accuracy - pass 2
	mul.s $f11, $f10, $f10
	mul.s $f11, $f11, $f9
	sub.s $f12, $f15, $f11
	mul.s $f10, $f10, $f12
	# Newtons method - increase accuracy - pass 3
	mul.s $f11, $f10, $f10
	mul.s $f11, $f11, $f9
	sub.s $f12, $f15, $f11
	mul.s $f10, $f10, $f12
	
	
	mov.s $f0, $f10
	jr $ra
	

# radius in $f20
# height in $f21
calcSurf:
	mul.s $f4, $f20, $f20
	mul.s $f5, $f3, $f4 # This should be added to next result (pi*r^2)
	mul.s $f7, $f21, $f21
	add.s $f0, $f4, $f7
	addi $sp, $sp -4
	sw $ra, 0($sp)
	jal squareRoot # This will store 1/(sqrt(h^2+r^2)) into $f0 
	lw $ra, 0($sp)
	addi $sp, $sp -4
	li.s $f10, 1.0 # Getting sqrt
	div.s $f8, $f10, $f0 # square root now in $f8
	
	
	mul.s $f8, $f3, $f8 # multiply pi by sqrt value
	mul.s $f8, $f20, $f8 # Multiply by r 
	add.s $f0 $f5, $f8
	jr $ra
# radius in $f20
# height in $f21
calcVol: 
	mul.s $f5, $f20, $f20
	mul.s $f5, $f21, $f5
	mul.s $f5, $f5, $f3
	li.s $f9, 3.0000
	div.s $f0, $f5, $f9
	jr $ra


























