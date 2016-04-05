### Computer Architecture Lab ###
# Name: Zachary Blanco
# RUID: 158007117
# netID: zdb5
# Section: 1A
### Computer Architecture Lab ###
.data 0x10000800
OriginRow_0: .word 1, 2, 3, 4, 5, 6
OriginRow_1: .word 7, 8, 9, 10, 11, 12
OriginRow_2: .word 13, 14, 15, 16, 17, 18
OriginRow_3: .word 19, 20, 21, 22, 23, 24
OriginRow_4: .word 25, 26, 27, 28, 29, 30
OriginRow_5: .word 31, 32, 33, 34, 35, 36
.data 0x10000900
TransRow_0: .word 0, 0, 0, 0, 0, 0
TransRow_1: .word 0, 0, 0, 0, 0, 0
TransRow_2: .word 0, 0, 0, 0, 0, 0
TransRow_3: .word 0, 0, 0, 0, 0, 0
TransRow_4: .word 0, 0, 0, 0, 0, 0
TransRow_5: .word 0, 0, 0, 0, 0, 0

.text 0x00400000
main: 

	# Transpose the matrix - each row is 6*4 bytes long
	add $t0, $zero, $zero #row  counter 
	add $t1, $zero, $zero #column counter
	addi $t9, $zero, 24 # 24 bytes in a row 
	addi $t6, $zero, 6
	la $s1, TransRow_0 # current row of counter
	loopRow:
		la $s0, OriginRow_0 # current col(0) of counter
		addi $t3, $zero, 4
		mul $t3, $t3, $t0
		add $s0, $t3, $s0 # s0 is now first item in row 0 of correct column 
		loopColumn:
			lw $t7, 0($s0)
			sw $t7, 0($s1)
			addi $s1, $s1, 4 #advance to next column
			add $s0, $s0, $t9 #go to next row
			addi $t1, $t1, 1
			blt $t1, $t6, loopColumn
		add $t1, $zero, $zero
		addi $t0, $t0, 1
		blt $t0, $t6, loopRow

	li $v0, 10
	syscall
		