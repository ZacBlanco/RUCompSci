.data 
array: .space 40   ##
unsorted: .asciiz "Unsorted Array: "
sortedArray: .asciiz "Sorted Array: "	
ask: .asciiz "Please enter 10 integers and press enter after each:\n"
positives: .asciiz "The number of positives is: "
negatives: .asciiz "The number of negatives is: "
zeroes: .asciiz "The number of zeroes is: "
median: .asciiz "The median is: "
size: .asciiz "Enter how many numbers wanted in Array:\n"
space: .asciiz " "
newline: .asciiz "\n"

.text 
.globl main 

main:

        la $s0, array
        li $s1, 10
        
        move $a0, $s0
		move $a1, $s1
        
        la $t0, 0($a0)
        li $t1, 0
        
        li $v0, 4
        la $a0, ask
        syscall
    loop:
        sll $t3, $t1, 2
		add $t2, $t3, $t0 
        
        li $v0, 5
        syscall
        sw $v0, 0($t2)
         
        addi $t1, $t1,  1
		sub  $t4, $s1, $t1
        bgtz $t4, loop
        
    print:
        move $a0, $s0 
        la $t0, 0($a0)
        li $t1, 0
        li $v0, 4
        la $a0, unsorted
	   syscall
	printloop:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		move $a0, $t3
		li $v0, 1
		syscall
        li $v0, 4
		la $a0, space
		syscall
		addi $t1, $t1,  1
		sub  $t4, $s1, $t1
		bgtz $t4, printloop	
    
    li $v0, 4
    la $a0, newline
	syscall    
         
start:
         
        move $a0, $s0
		move $a1, $s1
		jal sort
		move $a0, $s0      
		jal print1
		move $a0, $s0
		jal getmedian
		move $a0, $s0
		jal getinfo

j exit

        
sort:
        li $t0, 1
    oloop:
        li $t0, 0
        li $t1, 0               
    iloop:                
        sll $t8, $t1, 2
        add $t8, $a0, $t8    
        lw  $t2, 0($t8)        
        lw  $t3, 4($t8)    
        slt $t5, $t2, $t3       
        beq $t5, $0, swap       
        j continue
    swap:
        beq $t2, $t3, continue
        sw  $t3, 0($t8)         
        sw  $t2, 4($t8)     
        li $t0, 1
continue:
        addi $t1, $t1, 1            
        sub $t9, $s1, $t1
        addi $t9, $t9, -1
        bgtz $t9, iloop	 	
	    bgtz $t0, oloop 
    jr $ra

print1:
	la $t0, 0($a0)
	li $t1, 0
    li $v0, 4
    la $a0, sortedArray
	syscall
	printloop1:
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
		move $a0, $t3
		li $v0, 1
		syscall
        li $v0, 4
		la $a0, space
		syscall
		addi $t1, $t1,  1
		sub  $t4, $s1, $t1
		bgtz $t4, printloop1	
    
    li $v0, 4
    la $a0, newline
	syscall
	jr $ra
        
getmedian:
	la $t0, 0($a0) 
	move $t1, $a1 
	
	li $t2, 2
	div $t1, $t2 
	mfhi $t3  
	beq $t3, $0, even
    j odd
	odd:
		mflo $t3
		sll $t3, $t3, 2
		add $t4, $t3, $t0
		lw $t5, 0($t4)
		j PrintMed
	even:
		mflo $t6
        sll $t6, $t6, 2
		add $t6, $t6, $t0
        
		lw $t3, 0($t6)
		lw $t4, -4($t6)
		add $t5, $t4, $t3
		div $t5, $t2
		mflo $t5
		j PrintMed
	PrintMed:
        li $v0, 4
		la $a0, median
		syscall
		li $v0, 1
		move $a0, $t5
		syscall
    
    li $v0, 4    
    la $a0, newline
	syscall
	jr $ra


getinfo:
	la $t0, 0($a0)
	li $t1, 0              
	li $t6, 0               
    li $t7, 0              
	li $t8, 0            
    looparray:
        sub $t9, $s1, $t1   
        blez $t9, printinfo
		sll $t3, $t1, 2
		add $t2, $t3, $t0
		lw $t3, 0($t2)
        
		beq $t3, $0, zero
        blt $t3, $0, negative
        bgt $t3, $0, positive
        
    positive:
        addi $t1, $t1, 1
		addi $t7, $t7, 1
		j looparray
    negative:
		addi $t1, $t1, 1
		addi $t6, $t6, 1
		j looparray
    zero:
        addi $t1, $t1, 1
		addi $t8, $t8, 1
		j looparray
	
	printinfo:
        li $v0, 4
		la $a0, positives
		syscall
		li $v0, 1
		move $a0, $t7
		syscall
    
    li $v0, 4    
    la $a0, newline
	syscall
        
        li $v0, 4
		la $a0, negatives
		syscall
		li $v0, 1
		move $a0, $t6
		syscall
    
    li $v0, 4    
    la $a0, newline
	syscall
    
        li $v0, 4
		la $a0, zeroes
		syscall
		li $v0, 1
		move $a0, $t8
		syscall
    jr $ra

        
exit:
	    li $v0, 10
	    syscall
