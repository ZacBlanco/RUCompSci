.data
inputStr1: .asciiz "Enter A: "
inputStr2: .asciiz "Enter B: "
pleaseSelect: .asciiz "Please select one of the following operations \n"
option1: .asciiz "1) A and B\n"
option2: .asciiz "2) A or B\n"
option3: .asciiz "3) A xnor B\n"
option4: .asciiz "4) A xor B\n"
option5: .asciiz "5) A nand B\n"
option6: .asciiz "6) Exit\n"
choice:  .asciiz "Your Choice: "
badChoice: .asciiz "Choice does not exist\n"
andStr: .asciiz "A and B = "
orStr: .asciiz "A or B  = "
xnorStr: .asciiz "A xnor B = "
xorStr: .asciiz "A xor B = "
nandStr: .asciiz "A nand B = "
newline: .asciiz "\n"

.text

main:
	# Get inputs from user
	la $a0, inputStr1
	li $v0, 4
	syscall
	li $v0, 5 # Now reading integer
	syscall
	move $t0, $v0 #A is in t0
	la $a0, inputStr2
	li $v0, 4
	syscall
	li $v0, 5 # Now reading integer
	syscall
	move $t1, $v0 # B is in t1
	
	jal printOptions
	
	loop:	
		la $a0, choice
		li $v0, 4
		syscall
		# Now going to read integer
		li $v0, 5
		syscall
		move $t2, $v0 #Putting choice in t2
		li $t3, 1
		beq $t2, $t3, andOp
		li $t3, 2
		beq $t2, $t3, orOp
		li $t3, 3
		beq $t2, $t3, xnorOp
		li $t3, 4
		beq $t2, $t3, xorOp
		li $t3, 5
		beq $t2, $t3, nandOp
		li $t3, 6
		beq $t2, $t3, exit
		andOp:
			la $a0, andStr
			li $v0, 4
			syscall
			and $t7, $t0, $t1
			j printResult
		orOp:
			la $a0, orStr
			li $v0, 4
			syscall
			or $t7, $t0, $t1
			j printResult
		xnorOp:
			la $a0, xnorStr
			li $v0, 4
			syscall
			xor $t7, $t0, $t1
			not $t7, $t7
			j printResult
		xorOp:
			la $a0, xorStr
			li $v0, 4
			syscall
			xor $t7, $t0, $t1
			j printResult
		nandOp:
			la $a0, nandStr
			li $v0, 4
			syscall
			and $t7, $t0, $t1
			not $t7, $t7
			j printResult
		error:
			la $a0, badChoice
			li $v0, 4
			syscall
			j loop
		
		printResult:
			move $a0, $t7
			li $v0, 1
			syscall
			la $a0, newline
			li $v0, 4
			syscall
			j loop
			
	exit:
		li $v0, 10
		syscall

printOptions:
	la $a0, pleaseSelect
	li $v0, 4
	syscall
	la $a0, option1
	li $v0, 4
	syscall
	la $a0, option2
	li $v0, 4
	syscall
	la $a0, option3
	li $v0, 4
	syscall
	la $a0, option4
	li $v0, 4
	syscall
	la $a0, option5
	li $v0, 4
	syscall
	la $a0, option6
	li $v0, 4
	syscall

	jr $ra