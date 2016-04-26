###################################
### Computer Architecture Lab 6 ###
### Name: Zachary Blanco		###
### NetID: zdb5					###
### Email: zac.blanco@rutgers.edu #
###################################

.local .s32 e[10] //10*2 bytes
.local .s32 f[10] //10*2 bytes

main:

	add.s32 r5, 10, 0 	 // n 
	add.s32 r6, 0x0, 0x0 // a
	add.s32 r7, 0x0, 0x0 // b
	add.s32 r8, 0x0, 0x0 // c
	add.s32 r9, 0x0, 0x0 // d
	
	add.s32 r10, 0x0, 0x0 // i
	
	
	loop:
		setp.gt.s32  r11, r10, r5;
		@r11 bra loopExit // branch if i > 10
		
		mul.s32 r12, r6, r7 // a*b
		mul.s32 r13, r8, r9 // c*d
		
		sub.s32 r12, r12, r13 //a*b - c*d  ==> r12 
	
		
		mul.s32 r14, r10, 4 // Pointer to e[i]
		
		st.local.s32 e[r14], r12
		
		mul.s32 r12, r6, r9 // a*d
		mul.s32 r13, r7, r8 // c*b
		
		add.s32 r12, r12, r13 // a*d + c*b ==> r12
		
		st.local.s32 f[r14], r12 // Store r12 into f[14]
		
		add.s32 r6, r6, r7
		add.s32 r8, r8, r9
		
		
		endLoop:
			add.s32 r10, r10, 1 //add 1 to i
			bra loop1
	
	loopExit:
	
exit;
