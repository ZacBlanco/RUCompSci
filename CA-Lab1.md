# Lab 1 Code and Exercise Assignments

**Assignment 2 - Commented Code**

```assembly
# Exercise1 is used in assignments 1 and 2

.data 0x10000000
.text 0x00400000
.globl main

main:
	addi $10, $0, 8 # Add immediately the value of 8 (1000) to register 0, store in reg 10
	li $11, 6 		# Load immediately into register 
	# calculate the $11 -th power of $10
	add $8, $0, $10 # Add the values of registers 0 and 10 and store in register 8 
	li $12, 2		# Load into register 12 the value 2 (10)
powerloop:
	bgt $12, $11, powerexit # Branch if greater than - if $12 is > $11 go to power exit
	sub $11, $11, 1 # Subtract the value 1 (1) from register 11 and store in register 11
	mul $10, $10, $8# Multiply register 8 and register 10 together, store in register 10
	j powerloop 	# jump to the address of the code 'powerloop'
powerexit:
	# power operation loop is over
	# Is the result in $10 correct? The result in $10 is hexadecimal
	li $2, 1		# Load immediately 10 into register $2.
	syscall			# End the program because 10 is in register $2
```
## Exercise 2

**Assignment 3**

Fill out the missing instructions according to the comments.
2. Run the program step by step by pressing the function key f10. Observe the change of
each register and each memory place at each step.
3. Record the data segment of this program and check the output.
4. Explain why index j is (size-1) but not size? 


```assembly
# Exercise2 is used in assignments 3
.text 0x00400000
.align 2
.globl main
main:
	lw $a0, Size 		# load the size of array into $a0, using lw
	addi $a0, $a0, -1 	# init index j with size - 1
	li $a1, 0 			# init index i = 0
	li $t2, 4			# t2 contains constant 4, init t2
loop:
	mul $t1, $a1, $t2 	# t1 gets i*4
	mul $t4, $a0, $t2 	# t4 gets j*4
	lw $a3, Array1($t1) # a3 = Array[i]
	sll $a3, $a3, 1		# double the value a3 = a3 * 2
	sw $a3, Array1($t4)	# store a3 to Array1[j]
	addi $a1, $a1, 1	# i = i+1
	sub $a0, $a0, 1		# j = j-1
	bge $a1, $a0, END	#
	j loop				#
END:
	 li $v0, 10 # syscall code 10 is for exit
	 syscall # make the syscall
.data 0x10000000
.align 2
Array1: .word 2 5 6 7 12 16 25 27
Size: .word 8
```

**Exercise 4.3**

Read and print a String

```assembly
# Read and print String from a user

.data
var: .asciiz "" # Empty string variable

.text
main:
	la $a0, var		# Store address of variable
	li $v0, 8		# Set syscall code to read string
	li $a1, 20000	# Set num characters argument to a large number
	syscall			# Do our syscall
	li $v0, 4		# Set syscall code to print string
	syscall			# Do our syscall to print
	
	li $v0, 10		## Exit Program ##
	syscall
```

```assembly
# Read a number of ints and sum them together

.text
main:
	li $v0, 5 		# Read n number of times
	syscall	
	move $t1, $v0	# store in t1
	li $t2, 1
	
loop:
	blez $t1, exit
	
	li $v0, 5
	syscall
	
	mul $t2, $v0, $t2
	sub $t1, $t1, 1
	j loop

exit:

	move $a0, $t2
	li $v0, 1
	syscall
	
	li $v0, 10
	syscall
```

## Assignment 6

```
main:
  li $t0, 1
  li $a0 9
  jal calc # call procedure

exit:
  li $v0, 10
  syscall

calc:

  bgt
  
    n3:
      li $t0, 1

    n2:
    
    n3:
  loop:
    

  # $t0 = 0
  # $t1 = 1
  # $t2 = 1
  # $t3 = 2
  # $t4 = 4
  # $t5 = 7
  # $t6 = 13
  # $t7 = 24
  # $t8 = 44
  # $t9 = 



```


