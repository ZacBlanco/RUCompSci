.text
main:
li $s0, 300
li $t7, 1

outer:
  add $t0, $s0, $zero
  addi $t0, $t0, -1
inner:
  beq $t0, 1, exit
  div $s0, $t0
  mfhi $t6
  beq $t6, $zero, s1
  addi $t0, $t0, -1
  j inner
  s1:
    addi $s0, $s0, -1
    j outer

exit:
move $a0, $s0
li $v0, 1
syscall
li $v0, 10
syscall
  