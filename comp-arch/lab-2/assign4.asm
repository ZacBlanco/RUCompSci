.data 0x10000000
 star: .asciiz "*"
 space: .asciiz " "
 newline: .asciiz "\n"

.text
main:
  li $v0, 5
  syscall
  move $s0, $v0 #s0 is the number of lines
  move $t0, $v0
  srl $t0, $t0, 1
  li $t1, 1
  loop:  
  beq $t1, $t0, preLoop2
  move $t6, $t1
  sll $t6, $t6, 1   #t6 is for stars
  addi $t6, $t6, -1
  move $t7, $t0     #$t7 is for spaces
  addi $t7, $t0, 1
  srl $t7, $t7, 1
  sub $t7, $t7, $t1
  la $a0, space
  printSpaces:
    blez $t7, printStars
    li $v0, 4
    syscall
    addi $t7, $t7, -1
    j printSpaces
  printStars:
    blez $t6, last
    la $a0, star
    li $v0, 4
    syscall
    addi $t6, $t6, -1
    j printStars
  last:
    addi, $t1, $t1, 1
    la $a0, newline
    li $v0, 4
    syscall
    j loop
    
  preLoop2:
    srl $t1, $s0, 1
    li $t2, 2
    div $s0, $t2
    mfhi $t3
    beq $t3, $zero, loop
    addi $t1, $t1, 1
  loop2:  
  beq $t1, $zero, exit
  move $t6, $t0
  sll $t6, $t6, 1   #t6 is for stars
  addi $t6, $t6, -1
  move $t7, $t0     #$t7 is for spaces
  addi $t7, $t1, 1
  srl $t7, $t7, 1
  add $t7, $t7, $t1
  la $a0, space
  printSpaces2:
    blez $t7, printStars2
    li $v0, 4
    syscall
    addi $t7, $t7, -1
    j printSpaces2
  printStars2:
    blez $t6, last2
    la $a0, star
    li $v0, 4
    syscall
    addi $t6, $t6, -1
    j printStars2
  last2:
    addi, $t1, $t1, -1
    la $a0, newline
    li $v0, 4
    syscall
    j loop2


exit:
  li $v0, 10
  syscall