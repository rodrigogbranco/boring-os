[BITS 16]

%include "src/constants.asm"

global _start

SECTION .text

_start:
    jmp load_kernel

os_size: ;in sectors
    dw 0, 0

disk_index: ;if floppy, code will change it to 0
    db 11111111b

sectors_per_track:
    db 0

number_of_heads:
    db 0

current_lba_address: ; second sector
    dw 1

gdt:

gdt_null:
    dq 0

gdt_kcs:
    dw GDT_KCS_LIMIT_BYTES_0_1, GDT_KCS_BASE_BYTES_2_3
    db GDT_KCS_BASE_BYTE_4, GDT_KCS_ACCESS_BYTE_5 , GDT_KCS_MIDDLE_LIMIT_FLAGS_BYTE_6, GDT_KCS_BASE_BYTE_7

gdt_kds:
    dw GDT_KDS_LIMIT_BYTES_0_1, GDT_KDS_BASE_BYTES_2_3
    db GDT_KDS_BASE_BYTE_4, GDT_KDS_ACCESS_BYTE_5 , GDT_KDS_MIDDLE_LIMIT_FLAGS_BYTE_6, GDT_KDS_BASE_BYTE_7

gdt_end:

gdt_pointer:
    dw gdt_end - gdt - 1
    dd gdt+(NEW_BOOTLOADER_SEGMENT << 4)

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
reset_disk:
    mov dl, [disk_index]
    mov ah, BIOS_INT_13H_RESET_DISK_SYSTEM
    jc reset_disk ; reset went wrong. Try again

get_disk_parameters:
    ; let's get disk geometry
    mov ah, BIOS_INT_13H_GET_DISK_GEOMETRY
    push es
    xor di, di
    mov es, di ; hack to get es:di = 0x0000:0x0000
    int BIOS_INT_13H
    pop es
    jc get_disk_parameters ; get_disk_parameters went wrong. Try again

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

    jc read_sector ; if CF is set, an error occurred. Try again

    mov ax, es
    add ax, BIOS_INT_13H_SEGMENT_OFFSET
    mov es, ax ; we've read BIOS_INT_13H_SECTOR_COUNT*SECTOR_SIZE bytes. Move es to new position (move to segment BIOS_INT_13H_SEGMENT_OFFSET)

    add word [current_lba_address], BIOS_INT_13H_SECTOR_COUNT ; move LBA pointer

    jmp read_sector

end_read_sector:
    ; the kernel is entirely in the memory.     

    ; Let's enable A20 gate
    mov ax, BIOS_INT_15H_ENABLE_A20
    int BIOS_INT_15H
    jnc a20_success

    ; write here code for Keyboard Controller or 0x92 Port!!!

a20_success:
   ; clear interruptions. BIOS INT will not be accessible anymore!
    cli

    lgdt [gdt_pointer]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    mov ax, gdt_kds - gdt
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, (STACK_SEG_BOOTLOADER * 16) + STACK_POINTER_BOOTLOADER
    mov ebp, esp    

    jmp gdt_kcs - gdt:KERNEL_INIT