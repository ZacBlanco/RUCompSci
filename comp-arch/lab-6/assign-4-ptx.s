.global s32 detMem[6]
.global .s32 r1[25]
.global .s32 r41[16]
.global .s32 r31[9]

main:
	# Set up constants
	add.s32 r27, 5 # Number of Rows/Cols
	add.s32 r29, -1 # -1 (constant)
	add.s32, r40, 0, 0 # load zero as a constant
	
	calcDeterminant:
	
	add.s32 r31, 0 # Running sum in S5
	add.s32 r32, 0 # used for constant mul.s32tipadd.s32cation
	
	add.s32 r25, 0
	add.s32 r26, 0
	call(calc4Det) # v0 now contains det
	
	ld.global.s32  r32, 0(r28) # load "a"
	mul.s32 r32, r32, r0
	add.s32 r31, r31, r32
	
	add.s32 r25, 0
	add.s32 r26, 1
	call(calc4Det) # v0 now contains det
	
	ld.global.s32  r32, 0(r28) # load "b"
	mul.s32 r32, r32, r0
	mul.s32 r32, r32, r29 # multiply by -1
	add.s32 r31, r31, r32
	
	add.s32 r25, 0
	add.s32 r26, 2
	call(calc4Det) # v0 now contains det
	
	ld.global.s32  r32, 0(r28) # load "c"
	mul.s32 r32, r32, r0
	add.s32 r31, r31, r32
	
	add.s32 r25, 0
	add.s32 r26, 3
	call(calc4Det) # v0 now contains det
	
	ld.global.s32  r32, 0(r28) # load "d"
	mul.s32 r32, r32, r0
	mul.s32 r32, r32, r29 # multiply by -1
	add.s32 r31, r31, r32
	
	add.s32 r25, 0
	add.s32 r26, 4
	call(calc4Det) # v0 now contains det
	
	ld.global.s32  r32, 0(r28) # load "e"
	mul.s32 r32, r32, r0
	add.s32 r31, r31, r32
	
	
	add.s32 r0, 1, 0 # Result in r0
	add.s32 r25, r31, 0
	syscall
	
	

exit;	
	
# Calculates the determinant of a 3x3 matrix.
# Uses a0, a1 for rows to ignore of original
calc3Det:
	# First load all values into 3x3 Matrix by looping over 
	add.s32 r10, 0 # row counter for 4x4
	add.s32 r11, 0 # column counter 4x4
	add.s32 r13, 0 # row counter
	add.s32 r14, 0 # column counter
	add.s32 r12, 4 # 4 as const
	add.s32 r19, 3
		rowLoop3:
			#Check if r13 is >= 4, if so, exit the colLoop
			bge r13, r19, exitRowLoop3
		
		
			bne r10, r25, noSkipRow3
			add.s32 r10, r10, 1
			noSkipRow3:
			
			add.s32 r11, 0 #reset
			colLoop3:
				#Check if r14 is >= 4, if so, exit the colLoop
				bge r14, r19, exitColLoop3
				
				# Skip col loading
				bne r11, r26, noSkipCol3
				add.s32 r11, r11, 1
				noSkipCol3:
				## Get location of word to load from r4[1-5]
				# Location to load is base + ((t0*20) + (t1*4))
				add.s32 r16, r40, 16
				mul.s32 r16, r16, r10
				mul.s32 r17, r11, r12
				add.s32 r17, r17, r16
				add.s32 r17, r17, r28
				ld.global.s32  r18, 0(r17) # Word @ location i,j is in r18
				
				## Store word in r3[1-5]
				# Location to store word is base+((r13*16)+(r14*4))
				add.s32 r16, r40, 12
				mul.s32 r16, r16, r13
				mul.s32 r17, r14, r12
				add.s32 r17, r17, r16
				add.s32 r17, r17, r15
				st.global.s32 r18, r31[r17] # Word @ i, j is now in x, y of 3 col matrix	
				
				
				# Iterate over the columns until r14 is equal to 4
				# add.s32 1 to each col for  r11, r14
				add.s32 r11, r11, 1
				add.s32 r14, r14, 1
				bra colLoop3
			exitColLoop3:
			
			# Iterate over the columns until r14 is equal to 4
			# add.s32 1 to each col for  r11, r14
			add.s32 r10, r10, 1
			add.s32 r13, r13, 1
			bra rowLoop3
			
		exitRowLoop3:
	
	# Use r10 as holding spot for result of a product
	
	#+aei
	ld.global.s32  r11, 0(r19) #1st row 1st col
	ld.global.s32  r15, 16(r19) # 2nd row middle col = 12(r1) + 4
	ld.global.s32  r18, 32(r19) # 3rd row 3rd col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	st.global.s32 r10, 0(r30)
	
	#+bfg
	ld.global.s32  r11, 4(r19) #1st row 2nd col
	ld.global.s32  r15, 20(r19) # 2nd row 3rd
	ld.global.s32  r18, 24(r19) # 3rd row 1st col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	st.global.s32 r10, 4(r30)
	
	#+cdh
	ld.global.s32  r11, 8(r19) #1st row 3rd col
	ld.global.s32  r15, 12(r19) # 2nd row 1st col
	ld.global.s32  r18, 28(r19) # 3rd row 2nd col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	st.global.s32 r10, 8(r30)
	
	#-ceg
	ld.global.s32  r11, 8(r19) #1st row 3rd col
	ld.global.s32  r15, 16(r19) # 2nd row 2nd col
	ld.global.s32  r18, 24(r19) # 3rd row 3rd col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	mul.s32 r10, r10, r29
	st.global.s32 r10, 12(r30)
	
	#-bdi
	ld.global.s32  r11, 4(r19) #1st row 2nd col
	ld.global.s32  r15, 12(r19) # 2nd row 1st col
	ld.global.s32  r18, 32(r19) # 3rd row 3rd col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	mul.s32 r10, r10, r29
	st.global.s32 r10, 16(r30)
	
	#-afh
	ld.global.s32  r11, 0(r19) #1st row 1st col
	ld.global.s32  r15, 20(r19) # 2nd row 3rd col
	ld.global.s32  r18, 28(r19) # 3rd row 2nd col
	mul.s32 r10, r11, r15
	mul.s32 r10, r10, r18
	mul.s32 r10, r10, r29
	
	## We don't need t1-t9 anymore so we can overwrite them
	
	ld.global.s32  r11, 0(r30)
	ld.global.s32  r12, 4(r30)
	ld.global.s32  r13, 8(r30)
	ld.global.s32  r14, 12(r30)
	ld.global.s32  r15, 16(r30)
	
	add.s32 r10, r10, r11
	add.s32 r10, r10, r12
	add.s32 r10, r10, r13
	add.s32 r10, r10, r14
	add.s32 r10, r10, r15
	
	add.s32 r0, r10, 0 # Return the determinant

	ret;

# Calculates the determinant of a 4x4 Matrix
# Arguments (r25, r26), integers of row/column to ignore from the 5x5 matrix
calc4Det:
	
	
	# First load all values into 4x4 Matrix by looping over 
	add.s32 r10, 0 # row counter for 5x5
	add.s32 r11, 0 # column counter 5x5
	add.s32 r13, 0 # row counter
	add.s32 r14, 0 # column counter
	add.s32 r12, 4 # 4 as const
		rowLoop4:
			#Check if r13 is >= 4, if so, exit the colLoop
			setp.ge.s32 r50, r13, r12
			@r50 bra exitRowLoop4
		
			setp.ne.s32 r50, r10, r25
			@r50 bra nowSkipRow4
			add.s32 r10, r10, 1
			noSkipRow4:
			
			add.s32 r11, 0 #reset
			colLoop4:
				#Check if r14 is >= 4, if so, exit the colLoop
				setp.ge.s32 r50, r14, r12
				@r50 bra exitColLoop4
				
				# Skip col loading
				setp.ne.s32 r50, r11, r26
				@r50 bra nowSkipCol4
				add.s32 r11, r11, 1
				noSkipCol4:
				## Get location of word to load from r[1-5]
				# Location to load is base + ((t0*20) + (t1*4))
				add.s32 r16, r40, 20
				mul.s32 r16, r16, r10
				mul.s32 r17, r11, r12
				add.s32 r17, r17, r16
				add.s32 r17, r17, r28
				ld.global.s32  r18, 0(r17) # Word @ location i,j is in r18
				
				## Store word in r4[1-5]
				# Location to store word is base+((r13*16)+(r14*4))
				add.s32 r16, r40, 16
				mul.s32 r16, r16, r13
				mul.s32 r17, r14, r12
				add.s32 r17, r17, r16
				add.s32 r17, r17, r15
				st.global.s32 r18, 0(r17) # Word @ i, j is now in x, y of 4 col matrix	
				
				
				# Iterate over the columns until r14 is equal to 4
				# add.s32 1 to each col for  r11, r14
				add.s32 r11, r11, 1
				add.s32 r14, r14, 1
				bra colLoop4
			exitColLoop4:
			
			# Iterate over the columns until r14 is equal to 4
			# add.s32 1 to each col for  r11, r14
			add.s32 r10, r10, 1
			add.s32 r13, r13, 1
			bra rowLoop4
			
		exitRowLoop4:

	# Should probably store arguments in memory here...maybe?
	
	# Calculate the determinant of the 4x4 located in
	add.s32 r10, 0 ## Running SUM for Det
	add.s32 r12, -1, 0
	#row 0 col 0
	
	add.s32 r25, 0
	add.s32 r26, 0
	call(calc3Det) # r0 now contains det
	
	ld.global.s32  r13, 0(r11) # Get a - multiply by v0
	mul.s32 r13, r13, r0 # multiply a by det
	add.s32 r10, r13, r10 # add.s32 t3 (result) to final running sum
	
	
	#row 0 col 1
	add.s32 r25, 0
	add.s32 r26, 1
	call(calc3Det)
	
	ld.global.s32  r13, 4(r11) # Get b - multiply by v0
	mul.s32 r13, r13, r0 # multiply b by det
	mul.s32 r13, r13, r12 #multiply by -1
	add.s32 r10, r13, r10 # add.s32 t3 (result) to final running sum
	
	
	#row 0 col 2
	add.s32 r25, 0
	add.s32 r26, 2
	call(calc3Det)
	
	ld.global.s32  r13, 8(r11) # Get c - multiply by v0
	mul.s32 r13, r13, r0 # multiply c by det
	add.s32 r10, r13, r10 # add t3 (result) to final running sum

	
	#row 0 col 3
	add.s32 r25, 0
	add.s32 r26, 3
	call(calc3Det)
	
	ld.global.s32  r13, 8(r11) # Get d - multiply by v0
	mul.s32 r13, r13, r0 # multiply d by det
	mul.s32 r13, r13, r12 #multiply by -1
	add.s32 r10, r13, r10 # add t3 (result) to final running sum
	
	add.s32 r0, r10, 0
	
	ret;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	