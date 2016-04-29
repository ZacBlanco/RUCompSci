main:
	
	// We assume input is in r1
	
	add.f32 r2, 0, 0 // Set our result as 0 to register r2
	add.f32 r3, 0, 1 // Set factorial to 1
	add.f32 r4, 0, 0 // set our counter to 0
	sumLoop:
		
		rcp.f32 r5, r3 // Do factorial
		add.f32 r2, r2, r5 //add 1/fact to sum
		
		add.f32 r4, r4, 1 //add 1 to factorial
		mul.f32 r3, r3, r4
		setp.le.f32 r6 r4, r1
		@r5 bra sumLoop
	
	// Result is now in r2
	
exit;