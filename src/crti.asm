SECTION .init
GLOBAL _init

_init:
    push ebp
    mov ebp, esp

SECTION .fini
GLOBAL _fini

_fini:
    push ebp
    mov ebp, esp    