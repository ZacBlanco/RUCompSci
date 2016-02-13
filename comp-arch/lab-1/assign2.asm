# Exercise1 is used in assignments 1 and 2

.data 0x10000000
.text 0x00400000
.globl main

main:
	addi $10, $0, 8 # Add immediately the value of 8 (1000) to register 0, store in reg 10
	li $11, 6 		# Load immediately into register 
	# calculate the $11 -th power of $10
	add $8, $0, $10 # Add the values of registers 0 and 10 and store in register 8 
	li $12, 2		# Load into register 12 the value 2 (10)
powerloop:
	bgt $12, $11, powerexit # Branch if greater than - if $12 is > $11 go to power exit
	sub $11, $11, 1 # Subtract the value 1 (1) from register 11 and store in register 11
	mul $10, $10, $8# Multiply register 8 and register 10 together, store in register 10
	j powerloop 	# jump to the address of the code 'powerloop'
powerexit:
	# power operation loop is over
	# Is the result in $10 correct? The result in $10 is hexadecimal
	li $2, 1		# Load immediately 10 into register $2.
	syscall			# End the program because 10 is in register $2