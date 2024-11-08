[BITS 16]

%include "src/constants.asm"

global _start

_start:
    jmp kernel_entry

print_char:
    push bp
    mov bp, sp

    push ax
    push bx

    mov ax, [bp+4]
    mov ah, BIOS_INT_10H_OUTPUT
    mov bh, BIOS_INT_10H_PAGE_NUMBER_0
    mov bl, BIOS_INT_10H_FOREGROUND_COLOR_GM
    int BIOS_INT_10H

    pop bx
    pop ax

    pop bp

    ret

print_string:
    push bp
    mov bp, sp

    push ax
    push bx
    push si

    mov si, [bp+6]

    cld
loop_print_string:
    lodsb

    cmp al, NULL_CHARACTER
    je end_print_string

    push ax
    call print_char
    pop ax
    jmp loop_print_string

end_print_string:
    pop si
    pop bx
    pop ax

    pop bp

    retf

kernel_entry:
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov ax, msg
    push ax
    call KERNEL_SEGMENT:print_string
    pop ax

    jmp 0x3000:big_dummy_kernel

msg db "Dummy Kernel Loaded!", CARRIER_RETURN, LINE_FEED, NULL_CHARACTER

TIMES 0x30000-($-$$) DB 0

big_dummy_kernel:
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov ax, big_msg
    push ax
    call KERNEL_SEGMENT:print_string
    pop ax

    jmp 0x6000:big_big_dummy_kernel

big_msg db "Big Dummy Kernel Loaded!", CARRIER_RETURN, LINE_FEED, NULL_CHARACTER   

TIMES 0x60000-($-$$) DB 0

big_big_dummy_kernel:
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov ax, big_big_msg
    push ax
    call KERNEL_SEGMENT:print_string
    pop ax

    jmp $

big_big_msg db "Big Big Dummy Kernel Loaded!", CARRIER_RETURN, LINE_FEED, NULL_CHARACTER   