.data 0x10000000
 star: .asciiz "*"
 space: .asciiz " "
 newline: .asciiz "\n"

.text
main:
  li $v0, 5
  syscall
  move $s0, $v0   # s0 is the number of lines
  move $t0, $v0   # a Copy of $s0 is stored in $t0
  srl $t0, $t0, 1 # divide $s0 by 2
  li $t1, 1       # Load 1 into $t1
                  # $t1 is our row counter
  loop:  
  bgt $t1, $t0, preLoop2 # We've printed all the rows that we need already
  move $t6, $t1          # t6 is the number of stars we need to print
  sll $t6, $t6, 1        # t6 is for stars - 2*row - 1 = 1, 3, 5, 7, 9, ...
  addi $t6, $t6, -1
  move $t7, $s0          # $t7 is for spaces, Should be maxRows+1/2 - rowNum
  addi $t7, $t0, 1       # add 1 to our max rows
  srl $t7, $t7, 1        # divide by 2
  sub $t7, $t7, $t1      # subtract by (row number - 1) -> - rownumber + 1
  addi $t7, $t7, 2
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
      li $s1, 1
      srl $t1, $s0, 1
      li $t2, 2
      div $s0, $t2
      mfhi $t3
      beq $t3, $zero, loop2
      addi $t1, $t1, 1
    loop2:  
      beq $t1, $zero, exit # We've printed all the rows that we need already
      move $t6, $t1          # t6 is the number of stars we need to print
      sll $t6, $t6, 1        # t6 is for stars - 2*row - 1 = 1, 3, 5, 7, 9, ...
      addi $t6, $t6, -1
      move $t7, $s0          # $t7 is for spaces, Should be maxRows+1/2 - (maxRows - rowNum-1)
      addi $t7, $t0, 1       # 
      srl $t7, $t7, 1        # Divide by 2
      sub $t7, $t7, $t1
      addi $t7, $t7, 2
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