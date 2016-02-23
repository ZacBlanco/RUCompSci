.data

hexStr: 	.asciiz "Hex String: "
invHex: 	.asciiz "Invalid hex string. Exiting.\n"
valHex: 	.asciiz "Valid hex string.\n"
bValue: 	.asciiz "Hex to Binary Value"
zero: 	.asciiz "0000"
one: 	.asciiz "0001"
two: 	.asciiz "00010"
three: 	.asciiz "0011"
four: 	.asciiz "0100"
five: 	.asciiz "0101"
six: 	.asciiz "0110"
seven: 	.asciiz "0111"
eight: 	.asciiz "1000"
nine: 	.asciiz "1001"
ten: 	.asciiz "1010"
eleven: 	.asciiz "1011"
twelve: 	.asciiz "1100"
thriteen: .asciiz "1101"
fourteen: .asciiz "1110"
fifteen: 	.asciiz "1111"
hexValues:.asciiz "1234567890ABCDEF\n"
hexInput: .space 200
vlidChar: .asciiz "Character Valid\n"

.text
main:
	la $a0, hexStr
	li $v0, 4
	syscall
	li $v0, 8
	la $a0, hexInput
	li $a1, 20
	syscall
	la $a0, hexInput
	jal checkValidString # Return value is either 0 or 1
	beqz $v0, invalidString ## Check whether string is valid ##
	validString:
		la $a0, valHex
		li $v0, 4
		syscall
		la $a0, hexInput
		jal convert_print_binary
		j exit
	invalidString:
		la $a0, invHex
		li $v0, 4
		syscall
		j exit

convert_print_binary:
	move $t0, $a0
#	addi $t3, $t0, 4
	lbu $t2 0($t0)
	# Switch statement to print correct binary representation
	printLoop:
		lbu $t2, 0($t0)
		beq $t2, $zero, cpbExit
		li $t1, 48
		beq $t2, $t1, printZero
		li $t1, 49
		beq $t2, $t1, printOne
		li $t1, 50
		beq $t2, $t1, printTwo
		li $t1, 51
		beq $t2, $t1, printThree
		li $t1, 52
		beq $t2, $t1, printFour
		li $t1, 53
		beq $t2, $t1, printFive
		li $t1, 54
		beq $t2, $t1, printSix
		li $t1, 55
		beq $t2, $t1, printSeven
		li $t1, 56
		beq $t2, $t1, printEight
		li $t1, 57
		beq $t2, $t1, printNine
		li $t1, 65
		beq $t2, $t1, printA
		li $t1, 66
		beq $t2, $t1, printB
		li $t1, 67
		beq $t2, $t1, printC
		li $t1, 68
		beq $t2, $t1, printD
		li $t1, 69
		beq $t2, $t1, printE
		li $t1, 70
		beq $t2, $t1, printF
		j cpbExit
		printZero:
			la $a0, zero
			li $v0, 4
			syscall
			j endLoop
		printOne:
			la $a0, one
			li $v0, 4
			syscall
			j endLoop
		printTwo:
			la $a0, two
			li $v0, 4
			syscall
			j endLoop
		printThree:
			la $a0, three
			li $v0, 4
			syscall
			j endLoop
		printFour:
			la $a0, four
			li $v0, 4
			syscall
			j endLoop
		printFive:
			la $a0, five
			li $v0, 4
			syscall
			j endLoop
		printSix:
			la $a0, six
			li $v0, 4
			syscall
			j endLoop
		printSeven:
			la $a0, seven
			li $v0, 4
			syscall
			j endLoop
		printEight:
			la $a0, eight
			li $v0, 4
			syscall
			j endLoop
		printNine:
			la $a0, nine
			li $v0, 4
			syscall
			j endLoop
		printA:
			la $a0, ten
			li $v0, 4
			syscall
			j endLoop
		printB:
			la $a0, eleven
			li $v0, 4
			syscall
			j endLoop
		printC:
			la $a0, twelve
			li $v0, 4
			syscall
			j endLoop
		printD:
			la $a0, thirteen
			li $v0, 4
			syscall
			j endLoop
		printE:
			la $a0, fourteen
			li $v0, 4
			syscall
			j endLoop
		printF:
			la $a0, fifteen
			li $v0, 4
			syscall
			j endLoop
		
		endLoop:
			addi $t0, $t0, 1
			j printLoop
		
	cpbExit:
		jr $ra

checkValidString:
	la $a1, hexValues
	move $t1, $a0
	li $t6, 10 # value equaling end of input string
	charLoop:
		lbu $t3, 0($t1) 	# Load a char from the input string
		beq $t3, $zero, cvsValid 	# We finished checking and are at end of the string
		move $t8, $a1
		checkChar:		# Check whether it is a valid character
			lbu $t4, 0($t8)
			beqz $t4, cvsInvalid
			beq $t3, $t4, hexCharValid
			bne $t3, $t4, nextHex
			nextHex:
				addi $t8, $t8, 1
				j checkChar
				
		hexCharValid:
			addi $t1, $t1, 1 # Had a valid character
#			syscall
			j charLoop
		
		
	cvsInvalid:
		li $v0, 0
		j cvsExit
	cvsValid:
		li $v0, 1
		j cvsExit
	cvsExit:
		jr $ra
	
exit:
	li $v0, 10
	syscall