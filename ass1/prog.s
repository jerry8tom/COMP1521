# COMP1521 19t2 ... Game of Life on a NxN grid
#
# Written by Mukul, June/July 2019
# zID:	z5220980

	.globl	main
	.globl	decideCell
	.globl	neighbours
	.globl	copyBackAndShow
#======#
#.DATA #
#======#
	.data
	.align 2
msg1: 	.asciiz "# Iterations: "
msg2_1: .asciiz "=== After iteration "
msg2_2: .asciiz " ===\n"
eol:  	.asciiz "\n"
dot: 	.asciiz "."
hash: 	.asciiz "#"
byte_0: .byte 0
byte_1: .byte 1

#=============#
#.TEXT <main> #
#=============#
	.text
	.globl main
main:

# Frame:	...
    sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... int maxiters
	sw  $s4, -12($fp)		# save $s1 to use as ... int N
	addi	$sp, $sp, -16	# reset $sp to last pushed item

	lw $s4, N 				# N from boardX.s file

	# main()
	la $a0, msg1
	li $v0, 4
	syscall 				# printf("# Iterations: ")

	li $v0, 5
	syscall					# scanf ("%d", &maxiters);
	move $s0, $v0				

	li $s1, 1				# int n = 1

	iter:
		li $s5, 0 					# arr index
		bgt $s1, $s0, end_iter		# n <= maxiters
		li $s2, 0					# int i = 0
	row:
		beq $s2, $s4, end_row		# i < N
	 	li $s3, 0					# int j = 0
	col: 
		beq $s3, $s4, end_col		# j < N
	
		move $a0, $s2
		move $a1, $s3
		jal neighbours				# neighbours(i, j)
	
		move $t0, $v0				# int nn = neighbours(i, j)
		lb $a0, board($s5)			# first arg board[i][j]
		move $a1, $t0				# second arg nn
		jal decideCell
		sb $v0, newBoard($s5)		# newBoard[i][j] = decideCell (board[i][j], nn)

		addi $s5, $s5, 1			# arr index ++
		addi $s3, $s3, 1			# j++
		j col

	end_col:
		addi $s2, $s2, 1			# i++
		j row
	end_row:

		la $a0, msg2_1
		li $v0, 4
		syscall 					# printf("=== After iteration ")	

		move $a0, $s1				# printing 'n'				
		li $v0, 1
		syscall 					# %d

		la $a0, msg2_2
		li $v0, 4
		syscall 					# printf(" ===\n")
		
		addi $s1, $s1, 1			# n++
		
		jal copyBackAndShow			# copyBackAndShow();
		j iter

	end_iter:
		j main__post

main__post:
	lw $s4, -12($fp)
	lw $s0, -8($fp)
	lw $ra, -4($fp)
	la $sp, 4($fp)
	lw $fp, ($fp)

	li $v0, 0
	jr	$ra


#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
# decideCell(int old, int nn) #
#-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
decideCell:
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... char ret
	addi	$sp, $sp, -12	# reset $sp to last pushed item

	move $t0, $a0			# int old
	move $t1, $a1			# int nn
	lb 	 $t2, byte_1		
	lb   $t3, byte_0
	li   $t4, 2
	li   $t5, 3
	lb   $t6, byte_1

	if:
		bne $t0, $t6, elif			# if(old == 1)

		if_if:
			blt $t1, $t4, ret_0		# nn < 2	
		if_elif:
			beq $t1, $t4, ret_1		# nn == 2
			beq $t1, $t5, ret_1 	# nn == 3
		if_else:
			j ret_0
	elif:							# else if(nn == 3)
		beq $t1, $t5, ret_1
	else:
		j ret_0
	ret_0:
		lb $t7, byte_0				# ret = 0
		j end_decideCell
	ret_1:
		lb $t7, byte_1				# ret = 1
		j end_decideCell

end_decideCell:
	lw $s0, -8($fp)
	lw $ra, -4($fp)
	la $sp, 4($fp)
	lw $fp, ($fp)

	move $v0, $t7					# return ret
	jr $ra


#-+-+-+-+-+-+-+-+-+-+-+-+-+#
# neighbours(int i, int j) #
#-+-+-+-+-+-+-+-+-+-+-+-+-+#
neighbours:
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... int nn
	addi	$sp, $sp, -12	# reset $sp to last pushed item

	li $s0, 0				# int nn = 0
	li $t1, -1				# int x = -1
	li $t2, -1				# int y = -1
	move $t3, $a0			# int i
	move $t4, $a1 			# int j
	li $t5, 1				
	lw $t6, N
	addi $t6, $t6, -1		# N-1

	x_loop:
		bgt $t1, $t5, end_x		# x <= 1
		li $t2, -1				# int y = -1
	y_loop:
		bgt $t2, $t5, end_y		# y <= 1

		add $t7, $t3, $t1
		bltz $t7, continue		# i + x < 0
		bgt $t7, $t6, continue	# i + x > N-1

		add $t7, $t4, $t2
		bltz $t7, continue		# j + y < 0
		bgt $t7, $t6, continue 	# j + y > N-1
		
		bnez $t1, last_if		# x == 0
		bnez $t2, last_if		# y == 0

	continue:
		addi $t2, $t2, 1		# y++
		j y_loop
	last_if:
		addi $t6, $t6, 1		# temp change N-1 to N
		mul $t8, $t1, $t6		# x*N
		add $t8, $t8, $t2		# + y
		add $t8, $s5, $t8		# + arr index
		lb $t9, board($t8)

		addi $t6, $t6, -1		# chane N back to N-1

		lb $t0, byte_1
		bne $t0, $t9, continue	# board[i][j] == 0
		addi $s0, $s0, 1		# nn++
		j continue
	end_y:
		addi $t1, $t1, 1		# x++
		j x_loop 
	end_x:
		move $t0, $s0			# store nn for return
		j end_neighbours

end_neighbours:
	lw $s0, -8($fp)
	lw $ra, -4($fp)
	la $sp, 4($fp)
	lw $fp, ($fp)
	
	move $v0, $t0
	jr $ra


#-+-+-+-+-+-+-+-+-+-+-+-#
# copyBackAndShow(void) #
#-+-+-+-+-+-+-+-+-+-+-+-#
copyBackAndShow:
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	addi	$sp, $sp, -8	# reset $sp to last pushed item

	lw $t0, N
	li $t1, 0				# int i = 0
	li $t3, 0				# arr index
	li $t5, 0				

	copy_row:
		beq $t1, $t0, end_copy_row	# i < N
		li  $t2, 0					# int j = 0
	copy_col:
		beq $t2, $t0, end_copy_col		# j < N

		lb $t4, newBoard($t3)
		sb $t4, board($t3)			# board[i][j] = newboard[i][j]
		if_dead:
			beq $t4, $t5, print_dot	# board[i][j] == 0
		if_alive:
			j print_hash
		
		print_dot:
			addi $t2, $t2, 1		# j++
			addi $t3, $t3, 1		# arr index ++
			lb $a0, dot
			li $v0, 11
			syscall					# putchar('.')
			j copy_col
		print_hash:
			addi $t2, $t2, 1		# j++
			addi $t3, $t3, 1		# arr index ++
			lb $a0, hash
			li $v0, 11
			syscall					# putchar('#')
			j copy_col
	end_copy_col:
		addi $t1, $t1, 1			# i++
		print_eol:
			la $a0, eol
			li $v0, 4
			syscall 				# putchar('\n')
		j copy_row
	end_copy_row:
		j end_copyBackAndShow

end_copyBackAndShow:
	lw $ra, -4($fp)					# restore $ra for return
	la $sp, 4($fp)					# restore $sp
	lw $fp, ($fp)					# restore $fp

	jr $ra

