.text

main:
  li $t0, 1
  li $a0, 10
  jal calc # call procedure

ex:

  li $v0, 10 # Exit Procedure
  syscall

calc:

  move $t6, $a0
  sub $t6, $t6, 3
  li $t2, 1
  li $t1, 1
  li $t0, 0
    
    loop:
      li $t3, 0
      add $t3, $t1, $t0
      add $t3, $t3, $t2
      move $t0, $t1
      move $t1, $t2
      move $t2, $t3
      beq $t7, $t6, ex
      addi $t7, $t7, 1
     
      j loop
      
      # Answers
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


