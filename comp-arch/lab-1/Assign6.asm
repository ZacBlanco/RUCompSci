.text

main:
  li $t0, 1
  li $a0 9
  jal calc # call procedure

exit:
  li $v0, 10
  syscall

calc:

  move $a0, $t6
  beq $a0, 3, n3
  beq $a0, 2, n2
  beq $a0, 1, n1
  
    n3:
      addi $t0, $t0, 1
      li $t2, 1
    n2:
      addi $t0, $t0, 1
      li $t1, 1
    n1:
      addi $t0, $t0, 0
      li $t1, 1
    
    loop:
      li $t3, 0
      add $t3, $t2, $t1
      add $t3, $t3, $t1
      move $t1, $t0
      move $t2, $t1
      move $t3, $t1
      addi $t7, $t7, 1
      beq $t7, $t6, exit
      # $t0 = 0
      # $t1 = 1
      # $t2 = 1
      # $t3 = 2
      # $t4 = 4
      # $t5 = 7
      # $t6 = 13
      # $t7 = 24
      # $t8 = 44
      # $t9 = 68


