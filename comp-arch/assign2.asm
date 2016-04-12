###################################
### Computer Architecture Lab 6 ###
### Name: Zachary Blanco		###
### NetID: zdb5					###
### Email: zac.blanco@rutgers.edu #
###################################

.local .s16 x[10] //10*2 bytes
.local .s16 y[10] //10*2 bytes
.local .s16 z[10] //10*2 bytes

main:
	add.s16 r2, 0x0, 0x0 // pos equal to zero
	mov.s16 r3, r0	 // copy to neg 
	mov.s16 r4, r0 	 // copy to zero
	
	add.s16 r5, 0x0, 1 // initialize  i =1
	add.s16 r10, 10, 0x0 //const 10
	loop1:
		setp.gt.s16  r6, r5, r10;
		@r6 bra loop1Exit // branch if i > 10
		
		//Load x[i] 
		mul.s16 r11, r5, 2
		ld.local r12, [x+r11]; //x[i] in r12
		
		// Check x[i] < 0
		setp.gt.s16 r13, r12, 0x0
		@r13 bra elif1;
		
		xlt0:
			mul.s16 r13, r3, 2 // neg*2 for offset
			st.local.s16 [y+r14], r12; // put x[i] in y[neg]
			add.s16 r3, r3, 1 //neg = neg + 1;
			bra endLoop;
		
		elif1:
			//x[i] is already in r12
			//Check if x[i] > 0
			setp.lt.s16 r13, r12, 0x0
			@r13 bra endElse;//go to end else if not 
			mul.s16 r14, r4, 2 // 2*pos for offset
			st.local.s16 [z+r14], r12; // put x[i] in z[pos]
			add.s16 r4, r4, 1 //neg = neg + 1;
			bra endLoop;
			
		endElse:
			add.s16 r4, r4, 1 //zero = zero + 1;
			
		endLoop:
			add.s16 r5, r5, 1 //add 1 to i
			bra loop1
		
	loop1Exit:
	
exit;