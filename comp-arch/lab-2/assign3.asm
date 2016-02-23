.data 0x10000000
 tooBig: .asciiz "Your number was too big. Please Restart"
 tooSmall: .asciiz "Your Number was too small. Please Restart "

.text
main:

	## This section gets user input and makes sure it is within correct range
  li $v0, 5
  syscall
  move $t0, $v0
  addi $t1, $t0, -10
  bltz $t1, errorSmall
  addi $t1, $t0, -500
  bgtz $t1, errorBig
  move $t6, $t0
  li $v0, 5
  syscall
  move $t0, $v0
  addi $t1, $t0, -10
  bltz $t1, errorSmall
  addi $t1, $t0, -500
  bgtz $t1, errorBig
  move $t7, $t0			
  
  # Determine which input is the higher and lower values
  sub $t1, $t6, $t7
  bltz $t1, t7Higher
  j t7Lower
  t7Higher:
    move $s1, $t7
    move $s0, $t6
    j preLoop
  t7Lower:
    move $s0, $t7
    move $s1, $t6
  
  
  # Determine sum from the set of numbers between two inputs
  preLoop:
  move $t4, $s0       #$t4 is first number to sum
  li $t0, 2
  div $t1, $s0, $t0
  mfhi $t3
  bne $t3, $zero, fNum
  fNumPlus:
    addi $t4, $t4, 1
  fNum:
    addi $t4, $t4, 1
    
    li $t5, 0 # inital sum
    move $t6, $t4
  sumLoop:
    sub $t7, $t6, $s1
    bgez, $t7, print
    add $t5, $t5, $t6
    addi $t6, $t6, 2
    j sumLoop


# Error handling logic

errorBig:
  la $a0, tooBig
  li $v0, 4
  syscall
  j exit
errorSmall:
  la $a0, tooSmall
  li $v0, 4
  syscall
  j exit
print:
  move $a0, $t5
  li $v0, 1
  syscall
  j exit

exit:
li $v0, 10
syscall
