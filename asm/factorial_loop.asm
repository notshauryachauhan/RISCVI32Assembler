addi a0, zero, 5
    addi t0, zero, 1
    add t1, a0, zero
    addi t5, zero, 1

fact_loop:
    bge t5, t1, fact_end
    add t2, t0, zero
    add t3, t1, zero
    add t4, zero, zero

mul_loop:
    beq t3, zero, mul_end
    add t4, t4, t2
    addi t3, t3, -1
    jal zero, mul_loop

mul_end:
    add t0, t4, zero
    addi t1, t1, -1
    jal zero, fact_loop

fact_end:
    add a0, t0, zero

Exit:
    jal zero, Exit