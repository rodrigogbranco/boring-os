[BITS 16]

%include "src/constants.asm"

global _start

_start:
    jmp load_kernel

os_size: ;in sectors
    dw 769, 0

disk_index: ;if floppy, code will change it to 0
    db 11111111b

sectors_per_track: ; 
    db 0

number_of_heads:
    db 0

current_lba_address: ; second sector
    dw 1 

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

    ; let's reset the disk system first
    mov dl, [disk_index]
    mov ah, BIOS_INT_13H_RESET_DISK_SYSTEM

    ; let's get disk geometry
    mov ah, BIOS_INT_13H_GET_DISK_GEOMETRY
    push es
    xor di, di
    mov es, di ; hack to get es:di = 0x0000:0x0000
    int BIOS_INT_13H
    pop es

    mov [number_of_heads], dh
    inc byte [number_of_heads]

    and cl, BIOS_INT_13H_SECTOR_MASK
    mov [sectors_per_track], cl

    ; we'll copy sectors to here
    mov bx, KERNEL_SEGMENT
    mov es, bx    

read_sector:
    ; which Logical Block Address do we have to copy now? 
    mov ax, [current_lba_address]

    ; have we reached the end?
    cmp ax, word [os_size]
    jg end_read_sector

    ; not yet, lets get CHS parameters
    mov bl, [sectors_per_track]
    div bl
    inc ah
    mov cl, ah ; here, we know which sector we start
    xor ah, ah ; al has the track number. 

    div byte [number_of_heads]
    mov dh, ah ; now we get the head number...
    mov ch, al ; ... and the cylinder number. We have all we need.

    mov dl, [disk_index] 
    mov al, BIOS_INT_13H_SECTOR_COUNT ; how many sectors are we reading at once?
    mov ah, BIOS_INT_13H_READ
    mov bx, KERNEL_INIT ; bx will be always the same. es is the register that will change     
    int BIOS_INT_13H

    mov ax, es
    add ax, BIOS_INT_13H_SEGMENT_OFFSET
    mov es, ax ; we've read BIOS_INT_13H_SECTOR_COUNT*SECTOR_SIZE bytes. move es to new position (move to segment BIOS_INT_13H_SEGMENT_OFFSET)

    add word [current_lba_address], BIOS_INT_13H_SECTOR_COUNT ; move LBA pointer

    jmp read_sector

end_read_sector:
    ; the kernel is entirely in the memory. Jump to its start address
    jmp KERNEL_SEGMENT:KERNEL_INIT

TIMES 510-($-$$) DB 0

BOOT_SIGNATURE