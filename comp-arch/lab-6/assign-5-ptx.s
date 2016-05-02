main:
	
	# r0 will not be used from now on. - It is the argument for sinh/tanh
	call(calcSinh)
	
	# Result stored in r1 >> r64
	add.f32 r63, r1, 0
	
	call(calcTanh)
	# Result stored in r1 >> r64
	add.f32 r63, r1, 0

exit;
	
# Argument in f0, result in f1
# Because we only need the first 4 terms we don't need to use the formula to approximate
# We can simply use the constants from the 1st 4 terms.
calcSinh:
	
	add.f32 $f3, r0, 0
	add.f32 $f4, $f3, 0
	mul.f32 $f4, $f4, $f4
	mul.f32 $f4, $f4, $f3 # $f4 now holds x^3
	mul.f32 $f4, -1.0, $f4 # $f4 now holds -x^3
	div.f32 $f4, $f4, 6 # $f4 now holds -x^3/6
	
	add.f32 $f7, $f3, 0
	mul.f32 $f7, $f7, $f7
	mul.f32 $f7, $f7, $f7 # $f7 now holds x^4
	mul.f32 $f7, $f7, $f3 # $f7 now holds x^5
	mul.f32 $f7, $f7, 3.0 # $f7 now holds 3x^5
	div.f32 $f7, $f7, 40.0 # $f7 now holds 3x^5/40
	
	# 4th term is -5x^7/112
	add.f32 $f8, $f3, 0
	mul.f32 $f8, $f8, $f8
	mul.f32 $f8, $f8, $f8 # $f8 now holds x^4
	mul.f32 $f8, $f8, $f8 # $f8 now holds x^8
	div.f32 $f8, $f8, $f3 # $f8 now holds x^7
	mul.f32 $f8, $f8, 5.0 # $f7 now holds 5x^7
	div.f32 $f8, $f8, 112 # $f7 now holds 5x^7/112
	mul.f32 $f8, $f8, -1.0 # $f7 now holds -5x^7/112
	
	add.f32 r1, $f3, $f4
	add.f32 r1, r1, $f7
	add.f32 r1, r1, $f8

	ret;


# Argument in f0, result in f1
calcTanh:
	
	add.f32 $f3, r0, 0 # x
	mul.f32 $f4, $f4, $f4 # x^2
	
	mul.f32 $f4, $f4, $f3 # x^3
	
	mul.f32 $f7, $f4, $f4 # x^6
	div.f32 $f7, $f7, $f3 # x^5
	
	mul.f32 $f8, $f7, $f3 #x^6
	mul.f32 $f8, $f8, $f3 #x^7
	
	div.f32 $f4, $f4, 3.0 # x^3/3
	
	div.f32 $f7, $f7, 5.0 # x^5/5
	
	div.f32 $f8, $f8, 7.0 # x^7/7
	
	add.f32 r1, $f3, $f4
	add.f32 r1, r1, $f7
	add.f32 r1, r1, $f8
	
	
	ret;
	
	
	
	
	
	
	
	
	
	
	
	
	
	