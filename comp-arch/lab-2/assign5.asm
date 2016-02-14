.data

failInfo: .asciiz "Failed. Please Enter a password between 6 and 10 characters\n"
setPass: 	.asciiz "Set a password: "
rEnter: 	.asciiz "Please re-enter the password: "
inc1: 	.asciiz "Incorrect, you have 1 more chance! Please re-enter the password: "
inc2: 	.asciiz "Incorrect, you have 2 more chances! Please re-enter the password: "
success: 	.asciiz "Password is setup\n"
newline: 	.asciiz "\n"
exString:	.asciiz "Too Many Attempts. Exiting"
password:	.space 20
password2: .space 20

.text
#### BEGIN METHOD ####
main:
    la $a0, setPass	
    li $v0, 4
    syscall
    li $v0, 8
    li $a1, 11
    la $a0, password
    syscall
    jal getStringLength
    addi $t0, $v0, -6
    bltz $t0, failedLength
    addi $t0, $v0, -10
    blez $t0, lengthPass
    failedLength:
    		la $a0, failInfo
		li $v0, 4
		syscall
    		j exit
    lengthPass:
    		la $a0, rEnter	
    		li $v0, 4
    		syscall
		li $v0, 8
    		la $a0, password2
		li $a1, 11
    		syscall
		la $a0, password
		la $a1, password2
		jal compStrings
		bne $v0, $zero, passMatch
	EnterPassAgain:
		la $a0, inc2	
    		li $v0, 4
    		syscall
		li $v0, 8
    		la $a0, password2
		li $a1, 11
    		syscall
		la $a0, password
		la $a1, password2
		jal compStrings
		bne $v0, $zero, passMatch
		
		la $a0, inc1	
    		li $v0, 4
    		syscall
		li $v0, 8
    		la $a0, password2
		li $a1, 11
    		syscall
		la $a0, password
		la $a1, password2
		jal compStrings
		bne $v0, $zero, passMatch
		la $a0, exString # Too Many Attempts
    		li $v0, 4
    		syscall
		j exit
	passMatch:
    		la $a0, success
		li $v0, 4
		syscall
		j exit
## END METHOD ##

#### BEGIN METHOD ####
getStringLength:
    li $t0, 0
    li $t1, 0
    gtsLoop:
    		lb $t1, 0($a0)		  	#Load character into t1
		beq $t1, $zero, gtsExit  #Exit if char is 0
		addi $a0, $a0, 1
		addi $t0, $t0, 1
		j gtsLoop
		
    gtsExit:
    		move $v0, $t0
		addi $v0, $t0, -1
		jr $ra
#### END METHOD ####
		
		
#### BEGIN METHOD ####
compStrings:
    move $t0, $a0
    move $t1, $a1 
    csLoop:
    		lb $t2, 0($t0)
		lb $t3, 0($t1)
		beq $t2, $zero, csEnd1
		beq $t3, $zero, csEnd2
		bne $t2, $t3, csNonMatching
		addi $t0, $t0, 1
		addi $t1, $t1, 1
		j csLoop

    csEnd1:
    		lb $t2, 0($t0)
		lb $t3, 0($t1)
		bne $t2, $t3, csNonMatching
		j csMatching
		
    csEnd2:
    		lb $t2, 0($t0)
		lb $t3, 0($t1)
		bne $t2, $t3, csNonMatching
    		j csMatching
		
    csMatching:
    		li $v0, 1
		j csExit
		
    csNonMatching:
    		li $v0, 0
		j csExit
		
    csExit:
    		jr $ra
#### END METHOD ####
    
    
exit:
    li $v0, 10
    syscall