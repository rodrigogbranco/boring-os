global isr, isr_size, isr0x999
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

; Exceptions
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
isr_error 0x0a ; Invalid TSS
isr_error 0x0b ; Segment Not Present
isr_error 0x0c ; Stack-Segment Fault
isr_error 0x0d ; General Protection Fault
isr_error 0x0e ; Page Fault
isr_no_error 0x0f ; Reserved
isr_no_error 0x10 ; x87 Floating-Point Exception
isr_error 0x11 ; Alignment Check
isr_no_error 0x12 ; Machine Check
isr_no_error 0x13 ; SIMD Floating-Point Exception
isr_no_error 0x14 ; Virtualization Exception
isr_no_error 0x15 ; Control Protection Exception
isr_no_error 0x16 ; Reserved
isr_no_error 0x17 ; Reserved
isr_no_error 0x18 ; Reserved
isr_no_error 0x19 ; Reserved
isr_no_error 0x1a ; Reserved
isr_no_error 0x1b ; Reserved
isr_no_error 0x1c ; Hypervisor Injection Exception
isr_error 0x1d ; VMM Communication Exception
isr_error 0x1e ; Security Exception
isr_no_error 0x1f ; Reserved
; IRQS
isr_no_error 0x20 ; IRQ 0 - System Timer
isr_no_error 0x21 ; IRQ 1 - keyboard controller
isr_no_error 0x22 ; IRQ 2 - Slave PIC
isr_no_error 0x23 ; IRQ 3 - serial port 2
isr_no_error 0x24 ; IRQ 4 - serial port 1
isr_no_error 0x25 ; IRQ 5 - parallel port 3 or ISA sound card
isr_no_error 0x26 ; IRQ 6 - floppy disk controller
isr_no_error 0x27 ; IRQ 7 - parallel port 1/parallel port 2/secondary ISA sound card 
isr_no_error 0x28 ; IRQ 8 - Real Time Clock
isr_no_error 0x29 ; IRQ 9 - ACPI
isr_no_error 0x2a ; IRQ 10 - Other (SCSI/NIC)
isr_no_error 0x2b ; IRQ 11 - Other (SCSI/NIC)
isr_no_error 0x2c ; IRQ 12 - PS2 Mouse
isr_no_error 0x2d ; IRQ 13 - CPU Coprocessor/float point unit
isr_no_error 0x2e ; IRQ 14 - Primary ATA channel
isr_no_error 0x2f ; IRQ 16 - Secondary ATA channel

isr:
; exceptions
dd   isr0x00, isr0x01, isr0x02, isr0x03, isr0x04,  isr0x05, isr0x06, isr0x07, isr0x08, isr0x09, 
dd   isr0x0a, isr0x0b, isr0x0c, isr0x0d, isr0x0e,  isr0x0f, isr0x10, isr0x11, isr0x12, isr0x13, 
dd   isr0x14, isr0x15, isr0x16, isr0x17, isr0x18, isr0x19, isr0x1a, isr0x1b, isr0x1c, isr0x1d,
dd   isr0x1e, isr0x1f
; irqs - MASTER PIC
dd   isr0x20, isr0x21, isr0x22, isr0x23, isr0x24, isr0x25, isr0x26, isr0x27
; irqs - SLAVE PIC
dd   isr0x28, isr0x29, isr0x2a, isr0x2b, isr0x2c, isr0x2d, isr0x2e, isr0x2f
isr_end:

isr_size:
dd (isr_end - isr)/DWORD_SIZE

prepare_isr:
    pusha
    mov ebp, esp

    mov ebx, [ebp+32] ; isr_nr on the stack
    push dword [ebp+36] ; err_code to the handler
    push ebx
    call [isr_handlers + 4*ebx]
    add esp, 8 ; remove err_code and isr_nr off the stack

    popa
    add esp, 8 ; remove error code and isr_nr from stack
    iret

; default handler
isr_no_error 0x999
