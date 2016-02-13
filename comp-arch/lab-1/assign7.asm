.text
main:
  sll $t0, $s3, 2 # i, Multiply value by 4 with a bitshift
  sll $t1, $s4, 2 # j, Multiply value by 4 with a bitshift
  sll $t2, $s5, 2 # k, Multiply value by 4 with a bitshift
  
  add, $t0, $t0, $s4 # Add base of A + offset*4
  add, $t1, $t1, $s4 # Add base of A + offset*4
  
  lw $t0, 0($t0)     # Load A[i]
  lw $t1, 0($t1)     # Load A[j]
  add $t3, $t0, $t1  # Add A[i] + A[j]
  mul $t3, $t3, $t1  # MUltiply above by A[j]
  
  add $t4, $t2, $s5  # Get exact location of memory of B[k]
  sw $t3, 0($t4)      # Store calculated value in B[k]