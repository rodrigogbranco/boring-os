global isr, isr_size, default_isr
extern isr_handlers

%define DWORD_SIZE 4

%macro isr_no_error 1
isr%1:
    push 0
    push %1
    jmp prepare_isr
%endmacro

%macro isr_error 1
isr%1:
    push %1
    jmp prepare_isr
%endmacro

isr_no_error 0x00 ; Division Error
isr_no_error 0x01 ; Debug
isr_no_error 0x02 ; Non-maskable Interrupt
isr_no_error 0x03 ; Breakpoint
isr_no_error 0x04 ; Overflow
isr_no_error 0x05 ; Bound Range Exceeded
isr_no_error 0x06 ; Invalid Opcode
isr_no_error 0x07 ; Device Not Available
isr_error 0x08 ; Double Fault
isr_no_error 0x09 ; Coprocessor Segment Overrun
isr_error 0x10 ; Invalid TSS
isr_error 0x11 ; Segment Not Present
isr_error 0x12 ; Stack-Segment Fault
isr_error 0x13 ; General Protection Fault
isr_error 0x14 ; Page Fault
isr_no_error 0x15 ; Reserved
isr_no_error 0x16 ; x87 Floating-Point Exception
isr_error 0x17 ; Alignment Check
isr_no_error 0x18 ; Machine Check
isr_no_error 0x19 ; SIMD Floating-Point Exception
isr_no_error 0x20 ; Virtualization Exception
isr_error 0x21 ; Control Protection Exception
isr_no_error 0x22 ; Reserved
isr_no_error 0x23 ; Reserved
isr_no_error 0x24 ; Reserved
isr_no_error 0x25 ; Reserved
isr_no_error 0x26 ; Reserved
isr_no_error 0x27 ; Reserved
isr_no_error 0x28 ; Hypervisor Injection Exception
isr_error 0x29 ; VMM Communication Exception
isr_error 0x30 ; Security Exception
isr_no_error 0x31 ; Reserved

isr:
dd   isr0x00, isr0x01, isr0x02, isr0x03, isr0x04,  isr0x05, isr0x06, isr0x07, isr0x08, isr0x09, 
dd   isr0x10, isr0x11, isr0x12, isr0x13, isr0x14, isr0x15, isr0x16, isr0x17, isr0x18, isr0x19, 
dd   isr0x20, isr0x21, isr0x22, isr0x23, isr0x24, isr0x25, isr0x26, isr0x27, isr0x28, isr0x29, 
dd   isr0x30, isr0x31
isr_end:

isr_size:
dd (isr_end - isr)/DWORD_SIZE

prepare_isr:
    pusha
    mov ebp, esp

    mov ebx, [ebp+32] ; isr_nr on the stack
    push dword [ebp+36] ; err_no to the handler
    call [isr_handlers + 4*ebx]
    add esp, 4 ; remove err_no off the stack

    popa
    add esp, 8 ; remove error code and isr_nr from stack
    iret

default_isr:
    iret
