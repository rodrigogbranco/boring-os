[BITS 16]

%include "src/constants.asm"

global _start

_start:
    jmp load_kernel

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

    mov si, [bp+4]

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

    ret

load_kernel:
    mov ax, STACK_SEG_BOOTLOADER
    mov ss, ax
    mov sp, STACK_POINTER_BOOTLOADER      

    mov ax, SEGMENTS_BOOTLOADER
    mov ds, ax
    mov ax, NEW_BOOTLOADER_SEGMENT
    mov es, ax

    mov si, BOOTLOADER_INIT
    mov di, BOOTLOADER_INIT

    mov cx, SECTOR_SIZE

    cld
    rep movsb
before_jump:
    jmp word NEW_BOOTLOADER_SEGMENT:new_boot_region
    
new_boot_region:
    mov ax, NEW_BOOTLOADER_SEGMENT
    mov ds, ax
  
    mov ax, 'p'
    push ax
    call print_char
    pop ax

    mov ax, msg
    push ax
    call print_string
    pop ax

    jmp $

msg db "Hello, world", CARRIER_RETURN, LINE_FEED, NULL_CHARACTER

TIMES 510-($-$$) DB 0

BOOT_SIGNATURE