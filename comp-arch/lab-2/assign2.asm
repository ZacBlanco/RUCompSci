.text
main:
li $s0, 300  # Set max value to 300
li $t7, 1

outer:
  add $t0, $s0, $zero # set t0 = current s0
  addi $t0, $t0, -1   # subtract 1
inner:
  beq $t0, 1, exit  # if t0 is 1, then we've found our prime
  div $s0, $t0		# divide s0 by t0
  mfhi $t6
  beq $t6, $zero, s1 #determine remainder - if there is one, it could still be prime.
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
  