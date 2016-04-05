# Calculate the sum of even numbers inside a given array
.data 0x10000480
ArrayA:
 .word 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144
.text
.globl main
main:
la $t1, ArrayA
li $t2, 0 # count = 0
li $t3, 12 # number of elements in ArrayA
li $t4, 1 # mask
loop:
 lw $t5, 0($t1)
 and $t6, $t5, $t4
 beq $t6, $t4, cont
 add $t2, $t2, $t5
cont:
 addi $t1, $t1, 4
 addi $t3, $t3, -1
 bgt $t3, $0, loop
li $v0, 10
syscall