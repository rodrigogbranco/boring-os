[BITS 16]

%include "src/constants.asm"

global _start

_start:
    jmp load_kernel

os_size:
    dw 385, 0

disk_index: 
    db 11111111b

sectors_per_track:
    db 0

number_of_heads:
    db 0

current_lba_address:
    dw 1

track_number:
    db 0    

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

    mov [disk_index], dl

    cld
    rep movsb
before_jump:
    jmp word NEW_BOOTLOADER_SEGMENT:new_boot_region
    
new_boot_region:
    mov ax, NEW_BOOTLOADER_SEGMENT
    mov ds, ax

    mov dl, [disk_index]
    mov ah, BIOS_INT_13H_RESET_DISK_SYSTEM

    mov ah, BIOS_INT_13H_GET_DISK_GEOMETRY
    push es
    xor di, di
    mov es, di
    int BIOS_INT_13H
    pop es

get_parameters:
    mov [number_of_heads], dh
    inc byte [number_of_heads]

    and cl, BIOS_INT_13H_SECTOR_MASK
    mov [sectors_per_track], cl

    mov bx, KERNEL_SEGMENT
    mov es, bx    

read_sector:
    mov ax, [current_lba_address]

    cmp ax, word [os_size]
    jg end_read_sector

    mov bl, [sectors_per_track]
    div bl
    inc ah
    mov cl, ah
    mov [track_number], al
    xor ah, ah

    div byte [number_of_heads]
    mov dh, ah
    mov ch, al

    mov dl, [disk_index]
    mov al, BIOS_INT_13H_SECTOR_COUNT
    mov ah, BIOS_INT_13H_READ
    mov bx, KERNEL_INIT      
    int BIOS_INT_13H

    mov ax, es
    add ax, BIOS_INT_13H_SEGMENT_OFFSET
    mov es, ax

    add word [current_lba_address], BIOS_INT_13H_SECTOR_COUNT

    jmp read_sector

end_read_sector:
    jmp KERNEL_SEGMENT:KERNEL_INIT

TIMES 510-($-$$) DB 0

BOOT_SIGNATURE