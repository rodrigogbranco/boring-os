[BITS 16]

%include "src/constants.asm"

global _start

_start:
    jmp load_kernel

os_size:
    dw 1, 0

disk_index: 
    db 0xff

load_kernel:
    mov ax, STACK_SEG_BOOTLOADER
    mov ss, ax
    mov sp, STACK_POINTER_BOOTLOADER
    mov bp, sp

    mov ax, SEGMENTS_BOOTLOADER
    mov ds, ax
    mov ax, NEW_BOOTLOADER_SEGMENT
    mov es, ax

    mov si, BOOTLOADER_INIT
    mov di, BOOTLOADER_INIT

    mov cx, SECTOR_SIZE

    mov [ds:disk_index], dl

    cld
    rep movsb
before_jump:
    jmp word NEW_BOOTLOADER_SEGMENT:new_boot_region
    
new_boot_region:
    mov ax, NEW_BOOTLOADER_SEGMENT
    mov ds, ax

    mov ah, BIOS_INT_13H_READ
    mov al, [ds:os_size]
    mov ch, BIOS_INT_13H_CYLINDER_NUMBER
    mov dh, BIOS_INT_13H_HEAD_NUMBER    
    mov cl, BIOS_INT_13H_SECTOR_NUMBER
    mov dl, [ds:disk_index]
    mov bx, KERNEL_SEGMENT
    mov es, bx
    mov bx, KERNEL_INIT
    int BIOS_INT_13H

    jmp KERNEL_SEGMENT:KERNEL_INIT

TIMES 510-($-$$) DB 0

BOOT_SIGNATURE