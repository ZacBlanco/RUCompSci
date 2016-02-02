.text
main:
  li $t0, 1 # Load 1 into t1
  li $a0, 9 # Calculate for 9
  jal calc # call procedure to calculate

ex:

  li $v0, 1     # set syscall for print int
  move $a0, $t3 # Move from result to print argument
  syscall
  li $v0, 10    # Exit 
  syscall

calc:

  move $t6, $a0   # Move our argument to $t6 for temp storage
  sub $t6, $t6, 3 # Subtract 3 automatically for 1st 3 values of sequence
  li $t2, 1       # Initialize a[2]
  li $t1, 1       # Initialize a[1]
  li $t0, 0       # Initialize a[0]
    
    loop:
      li $t3, 0         # set current temp register to 0
      add $t3, $t1, $t0 # add t1, t0, t2 to t3
      add $t3, $t3, $t2 # add t1, t0, t2 to t3
      move $t0, $t1     # Move all registers up
      move $t1, $t2
      move $t2, $t3
      beq $t7, $t6, ex  # Branch if we've finished
      addi $t7, $t7, 1
     
      j loop            # loop