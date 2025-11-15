// Raspberry Pi 5 Boot Code in ARM64 Assembly
// Based on pi5_os reference implementation

.section ".text.boot"
.global _start

_start:
    // Ensure we're running on core 0
    mrs x0, mpidr_el1
    and x0, x0, #3
    cbnz x0, halt
    
    // Set stack pointer - Pi5 has more memory
    ldr x0, =0x80000000
    mov sp, x0
    
    // Clear BSS section
    ldr x0, =_BSS_START
    ldr x1, =_BSS_END
    
clear_bss:
    cmp x0, x1
    b.ge clear_done
    str xzr, [x0], #8
    b clear_bss
    
clear_done:
    // Jump to C main function
    bl kernel_main
    
halt:
    wfe
    b halt

// Exception vector table
.align 11
.global _start_vectors
_start_vectors:
    // Current EL with SP0
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt
    
    // Current EL with SPx
    .align 7
    b halt
    .align 7
    b irq_handler
    .align 7
    b halt
    .align 7
    b halt
    
    // Lower EL using AArch64
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt
    
    // Lower EL using AArch32
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt
    .align 7
    b halt

// IRQ handler stub
irq_handler:
    // Save all registers
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    stp x20, x21, [sp, #-16]!
    stp x22, x23, [sp, #-16]!
    stp x24, x25, [sp, #-16]!
    stp x26, x27, [sp, #-16]!
    stp x28, x29, [sp, #-16]!
    str x30, [sp, #-8]!

    // Call C interrupt handler
    bl c_irq_handler

    // Restore all registers
    ldr x30, [sp], #8
    ldp x28, x29, [sp], #16
    ldp x26, x27, [sp], #16
    ldp x24, x25, [sp], #16
    ldp x22, x23, [sp], #16
    ldp x20, x21, [sp], #16
    ldp x18, x19, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16

    eret
