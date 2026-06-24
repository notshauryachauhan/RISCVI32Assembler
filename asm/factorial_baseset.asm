addi a0, zero, 5
    jal ra, fact

Exit:
    jal zero, Exit

fact:
    addi sp, sp, -8
    sw a0, 4(sp)
    sw ra, 0(sp)
    
    addi t0, a0, -1
    bge t0, zero, L1
    
    addi a0, zero, 1
    addi sp, sp, 8
    jalr zero, 0(ra)

L1:
    addi a0, a0, -1
    jal ra, fact
    
    add t1, a0, zero
    
    lw ra, 0(sp)
    lw a0, 4(sp)
    
    add t2, zero, zero
    add t3, a0, zero

mul_loop:
    beq t3, zero, mul_done
    add t2, t2, t1
    addi t3, t3, -1
    jal zero, mul_loop

mul_done:
    add a0, t2, zero
    
    addi sp, sp, 8
    jalr zero, 0(ra)