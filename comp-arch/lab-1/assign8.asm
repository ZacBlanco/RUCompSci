.data 0x10000000
 ask: .asciiz "\nEnter a number:"
 ans: .asciiz "Answer: "
.text 0x00400000
.globl main
main:
	li $v0, 4
	la $a0, ask # Loads the ask string
	syscall # Display the ask string
	li $v0, 5 # Read the input
	syscall
	move $t0, $v0 # n = $v0, Move the user input
	addi $t1, $0, 0 # i = 0
	addi $t2, $0, 189 # ans = 189, Starting case (n=0)
	li $t3, 2
 
loop:
	beq $t1, $t0, END # from i = 0 to n-1 (n times)
	addi $t1, $t1, 1 # i = i+1
	div $t2, $t3 # LO = ans / 2, The result is stored on LO
	mflo $t2 # ans = LO, Copies the content of LO to t2
	j loop
END:
	li $v0, 4
	la $a0, ans # Loads the ans string
	syscall
	move $a0, $t2 # Loads the answer
	li $v0, 1
	syscall
	li $v0, 10
	syscall