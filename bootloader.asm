global _start

_start:
    jmp print_s

print_s:
    mov ah, 0x0e
    mov al, 'S'
    mov bh, 0x00
    mov bl, 0x02
    int 0x10

TIMES 510-($-$$) DB 0

db 0x55, 0xaa