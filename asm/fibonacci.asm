addi x10, x0, 10
addi t0, zero, 0
addi t1, zero, 1
addi x28, x10, -1

fib:
	add t2, t1, t0
	add t0, t1, zero
	add t1, t2, zero
	addi x28, x28, -1
	bne x28, zero, fib

exit:
	add a0, t2, zero