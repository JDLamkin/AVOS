; Provides a simple interface for basic machine info storage.

MEMORY_LIMIT equ 0x100

section .bss.low    nobits  write

MEMORY_MAP:
    resb 24*MEMORY_LIMIT

section .text

; void stdcall()
global gather_info_16
gather_info_16:
    ret
