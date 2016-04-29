.data
r1: .word 2, 4, 6, 8, 10
r2: .word 12, 14, 16, 18, 20
r3: .word 22, 24, 26, 28, 30
r4: .word 32, 34, 36, 38, 40
r5: .word 42, 44, 46, 48, 50

detMem: .space 24 # Used to store determinant components of 3x3 matrix

r41: .space 16
r42: .space 16
r43: .space 16
r44: .space 16

r31: .space 12
r32: .space 12
r33: .space 12

outputStr: .asciiz "The Determinant is: "
## Determinant should equal 0 ## !!!


.text

main:
	# Set up constants
	li $s0, 5 # Number of Rows/Cols
	la $s1, r1
	li $s2, -1 # -1 (constant)
	la $s3, detMem # Loads location of detMem (never changed)
	
	calcDeterminant:
	
	li $s5, 0 # Running sum in S5
	li $s6, 0 # used for constant multiplication
	
	li $a0, 0
	li $a1, 0
	jal calc4Det # v0 now contains det
	
	lw $s6, 0($s1) # load "a"
	mul $s6, $s6, $v0
	add $s5, $s5, $s6
	
	li $a0, 0
	li $a1, 1
	jal calc4Det # v0 now contains det
	
	lw $s6, 0($s1) # load "b"
	mul $s6, $s6, $v0
	mul $s6, $s6, $s2 # multiply by -1
	add $s5, $s5, $s6
	
	li $a0, 0
	li $a1, 2
	jal calc4Det # v0 now contains det
	
	lw $s6, 0($s1) # load "c"
	mul $s6, $s6, $v0
	add $s5, $s5, $s6
	
	li $a0, 0
	li $a1, 3
	jal calc4Det # v0 now contains det
	
	lw $s6, 0($s1) # load "d"
	mul $s6, $s6, $v0
	mul $s6, $s6, $s2 # multiply by -1
	add $s5, $s5, $s6
	
	li $a0, 0
	li $a1, 4
	jal calc4Det # v0 now contains det
	
	lw $s6, 0($s1) # load "e"
	mul $s6, $s6, $v0
	add $s5, $s5, $s6
	
	la $a0, outputStr
	li $v0, 4
	syscall
	
	li $v0, 1
	move $a0, $s5
	syscall
	
	

exit:
	li $v0, 10
	syscall
	
	
# Calculates the determinant of a 3x3 matrix.
# Uses a0, a1 for rows to ignore of original
calc3Det:
	# First load all values into 3x3 Matrix by looping over 
	li $t0, 0 # row counter for 4x4
	li $t1, 0 # column counter 4x4
	li $t3, 0 # row counter
	li $t4, 0 # column counter
	li $t2, 4 # 4 as const
	li $t9, 3
	la $t5, r31
		rowLoop3:
			#Check if $t3 is >= 4, if so, exit the colLoop
			bge $t3, $t9, exitRowLoop3
		
		
			bne $t0, $a0, noSkipRow3
			addi $t0, $t0, 1
			noSkipRow3:
			
			li $t1, 0 #reset
			colLoop3:
				#Check if $t4 is >= 4, if so, exit the colLoop
				bge $t4, $t9, exitColLoop3
				
				# Skip col loading
				bne $t1, $a1, noSkipCol3
				addi $t1, $t1, 1
				noSkipCol3:
				## Get location of word to load from r4[1-5]
				# Location to load is base + ((t0*20) + (t1*4))
				addi $t6, $zero, 16
				mul $t6, $t6, $t0
				mul $t7, $t1, $t2
				add $t7, $t7, $t6
				add $t7, $t7, $s1
				lw $t8, 0($t7) # Word @ location i,j is in $t8
				
				## Store word in r3[1-5]
				# Location to store word is base+(($t3*16)+($t4*4))
				addi $t6, $zero, 12
				mul $t6, $t6, $t3
				mul $t7, $t4, $t2
				add $t7, $t7, $t6
				add $t7, $t7, $t5
				sw $t8, 0($t7) # Word @ i, j is now in x, y of 3 col matrix	
				
				
				# Iterate over the columns until $t4 is equal to 4
				# Add 1 to each col for  $t1, $t4
				addi $t1, $t1, 1
				addi $t4, $t4, 1
				j colLoop3
			exitColLoop3:
			
			# Iterate over the columns until $t4 is equal to 4
			# Add 1 to each col for  $t1, $t4
			addi $t0, $t0, 1
			addi $t3, $t3, 1
			j rowLoop3
			
		exitRowLoop3:
	
	# Use $t0 as holding spot for result of a product
	la $t9, r31
	
	#+aei
	lw $t1, 0($t9) #1st row 1st col
	lw $t5, 16($t9) # 2nd row middle col = 12(r1) + 4
	lw $t8, 32($t9) # 3rd row 3rd col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	sw $t0, 0($s3)
	
	#+bfg
	lw $t1, 4($t9) #1st row 2nd col
	lw $t5, 20($t9) # 2nd row 3rd
	lw $t8, 24($t9) # 3rd row 1st col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	sw $t0, 4($s3)
	
	#+cdh
	lw $t1, 8($t9) #1st row 3rd col
	lw $t5, 12($t9) # 2nd row 1st col
	lw $t8, 28($t9) # 3rd row 2nd col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	sw $t0, 8($s3)
	
	#-ceg
	lw $t1, 8($t9) #1st row 3rd col
	lw $t5, 16($t9) # 2nd row 2nd col
	lw $t8, 24($t9) # 3rd row 3rd col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	mul $t0, $t0, $s2
	sw $t0, 12($s3)
	
	#-bdi
	lw $t1, 4($t9) #1st row 2nd col
	lw $t5, 12($t9) # 2nd row 1st col
	lw $t8, 32($t9) # 3rd row 3rd col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	mul $t0, $t0, $s2
	sw $t0, 16($s3)
	
	#-afh
	lw $t1, 0($t9) #1st row 1st col
	lw $t5, 20($t9) # 2nd row 3rd col
	lw $t8, 28($t9) # 3rd row 2nd col
	mul $t0, $t1, $t5
	mul $t0, $t0, $t8
	mul $t0, $t0, $s2
	
	## We don't need t1-t9 anymore so we can overwrite them
	
	lw $t1, 0($s3)
	lw $t2, 4($s3)
	lw $t3, 8($s3)
	lw $t4, 12($s3)
	lw $t5, 16($s3)
	
	add $t0, $t0, $t1
	add $t0, $t0, $t2
	add $t0, $t0, $t3
	add $t0, $t0, $t4
	add $t0, $t0, $t5
	
	move $v0, $t0 # Return the determinant

	jr $ra

# Calculates the determinant of a 4x4 Matrix
# Arguments ($a0, $a1), integers of row/column to ignore from the 5x5 matrix
calc4Det:
	
	
	# First load all values into 4x4 Matrix by looping over 
	li $t0, 0 # row counter for 5x5
	li $t1, 0 # column counter 5x5
	li $t3, 0 # row counter
	li $t4, 0 # column counter
	li $t2, 4 # 4 as const
	la $t5, r41
		rowLoop4:
			#Check if $t3 is >= 4, if so, exit the colLoop
			bge $t3, $t2, exitRowLoop4
		
		
			bne $t0, $a0, noSkipRow4
			addi $t0, $t0, 1
			noSkipRow4:
			
			li $t1, 0 #reset
			colLoop4:
				#Check if $t4 is >= 4, if so, exit the colLoop
				bge $t4, $t2, exitColLoop4
				
				# Skip col loading
				bne $t1, $a1, noSkipCol4
				addi $t1, $t1, 1
				noSkipCol4:
				## Get location of word to load from r[1-5]
				# Location to load is base + ((t0*20) + (t1*4))
				addi $t6, $zero, 20
				mul $t6, $t6, $t0
				mul $t7, $t1, $t2
				add $t7, $t7, $t6
				add $t7, $t7, $s1
				lw $t8, 0($t7) # Word @ location i,j is in $t8
				
				## Store word in r4[1-5]
				# Location to store word is base+(($t3*16)+($t4*4))
				addi $t6, $zero, 16
				mul $t6, $t6, $t3
				mul $t7, $t4, $t2
				add $t7, $t7, $t6
				add $t7, $t7, $t5
				sw $t8, 0($t7) # Word @ i, j is now in x, y of 4 col matrix	
				
				
				# Iterate over the columns until $t4 is equal to 4
				# Add 1 to each col for  $t1, $t4
				addi $t1, $t1, 1
				addi $t4, $t4, 1
				j colLoop4
			exitColLoop4:
			
			# Iterate over the columns until $t4 is equal to 4
			# Add 1 to each col for  $t1, $t4
			addi $t0, $t0, 1
			addi $t3, $t3, 1
			j rowLoop4
			
		exitRowLoop4:

	# Should probably store arguments in memory here...maybe?
	
	# Calculate the determinant of the 4x4 located in
	li $t0, 0 ## Running SUM for Det
	la $t1, r41 
	li $t2, -1
	#row 0 col 0
	addi $sp, $sp, -16 #?
	sw $ra, 0($sp) # store ra
	sw $t0, 4($sp) # store t0
	sw $t1, 8($sp) # store r41
	sw $t2, 12($sp) # store -1
	
	li $a0, 0
	li $a1, 0
	jal calc3Det # v0 now contains det
	
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 0($t1) # Get a - multiply by v0
	mul $t3, $t3, $v0 # Multiply a by det
	add $t0, $t3, $t0 # add t3 (result) to final running sum
	
	## store all the values again
	sw $t0, 4($sp) # store t0
	sw $t1, 8($sp) # store r41
	sw $t2, 12($sp) # store -1
	
	#row 0 col 1
	li $a0, 0
	li $a1, 1
	jal calc3Det
	
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 4($t1) # Get b - multiply by v0
	mul $t3, $t3, $v0 # Multiply b by det
	mul $t3, $t3, $t2 #multiply by -1
	add $t0, $t3, $t0 # add t3 (result) to final running sum
	
	## store all the values again
	sw $t0, 4($sp) # store t0
	sw $t1, 8($sp) # store r41
	sw $t2, 12($sp) # store -1
	
	#row 0 col 2
	li $a0, 0
	li $a1, 2
	jal calc3Det
	
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 8($t1) # Get c - multiply by v0
	mul $t3, $t3, $v0 # Multiply c by det
	add $t0, $t3, $t0 # add t3 (result) to final running sum
	
	## store all the values again
	sw $t0, 4($sp) # store t0
	sw $t1, 8($sp) # store r41
	sw $t2, 12($sp) # store -1
	
	#row 0 col 3
	li $a0, 0
	li $a1, 3
	jal calc3Det
	
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 8($t1) # Get d - multiply by v0
	mul $t3, $t3, $v0 # Multiply d by det
	mul $t3, $t3, $t2 #multiply by -1
	add $t0, $t3, $t0 # add t3 (result) to final running sum
	
	lw $ra, 0($sp)
	addi $sp, $sp, 16
	move $v0, $t0
	
	jr $ra
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	