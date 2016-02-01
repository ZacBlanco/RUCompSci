# Computer Architecture - Lab 1

Zachary Blanco

RUID - 158007117


## Assignment 1

| op 	| rs	 | rt	  | rd	   | shamt  | funct  | 
|-|-|-|-|-|-|
| 6 bits| 5 bits | 5 bits | 5 bits | 5 bits | 6 bits |

| op | rs | rt | rd | shamt | funct |
|-|-|-|-|-|-|
| 000000| 00000 | 01010 | 01000 | 00000 | 10000 |

| op | rs | rt | constant |
|-|-|-|-|
|001000|00000|01010|000 0000 0000 0000|

**Assignment 2 - Commented Code**

Below you will find the code that was commented out on each. line. This details what each command is doing throughout the execution of the program

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

## Assignment 3

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

2 - Images of Register when running the program

Image of Registers when first starting program

![img](a-3-1-1.png)

Image of register on 2nd line

![img](a-3-1-2.png)

Image of registers after executing loop and exiting

![img](a-3-1-3.png)

3 - Output

The all of the values of the array, A1, have been doubled, thus making the output is

```
2, 5, 6, 7, 14, 12, 10, 8, 4
```


4 - Why is index (j-1)

The reason for this is because the memory slots are zero-based. This means that the first value will occur at the 0th position, not the 1st position. So we must subtract 1 from j because of this.

## Assignment 4

1 - Type and Record the value of each register



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

```assembly

.text

main:
  li $t0, 1
  li $a0, 10
  jal calc # call procedure

ex:

  li $v0, 5
  move $t0, $a0
  syscall
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
```

## Assignment 7

```assembly

.text
main:
  lw $t0, $s1($s5)
  lw $t1, $s2($s5)
  lw $t2, $s3($s5)
  add $t3, $t0, $t1
  mul $t4, $t3, $t2
  sw $t4, $s5($s3)

```

## Assignment 8

```assembly
.data 0x10000000
 ask: .asciiz "\nEnter a number:"
 ans: .asciiz "Answer: "
.text 0x00400000
.globl main
main:
	li $v0, 4
	la $a0, ask # Loads the ask string
	syscall # Display the ask string
	li $v0, 5 # Read the input
	syscall
	move $t0, $v0 # n = $v0, Move the user input
	addi $t1, $0, 0 # i = 0
	addi $t2, $0, 189 # ans = 189, Starting case (n=0)
	li $t3, 2
 
loop:
	beq $t1, $t0, END # from i = 0 to n-1 (n times)
	addi $t1, $t1, 1 # i = i+1
	div $t2, $t3 # LO = ans / 2, The result is stored on LO
	mflo $t2 # ans = LO, Copies the content of LO to t2
	j loop
END:
	li $v0, 4
	la $a0, ans # Loads the ans string
	syscall
	move $a0, $t2 # Loads the answer
	li $v0, 1
	syscall
	li $v0, 10
	syscall

```


































