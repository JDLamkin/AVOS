; Boot Stage 1; See README.md

section .text

[bits 16]

extern print_string_16
extern gather_info_16

global start_16
start_16:
    push STRING.start
    call print_string_16
    call gather_info_16
    cli
    lgdt [GDT.Pointer]
    mov eax, cr0
    or eax, 0x00000001 ; Protected mode bit
    mov cr0, eax
    jmp GDT.Code:start_32

[bits 32]
start_32:
    xor ebx, ebx
    mov bx, GDT.Data
    mov ss, bx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov bx, bp
    mov ebp, ebx
    mov esp, ebp
    mov ecx, 0x1000 ; Number of doublewords in four pages
    mov edi, PAGE_TABLE
    xor eax, eax
    cld
    rep stosd
    mov dword [PAGE_TABLE.Root], PAGE_TABLE.Low2 + 0x3
    mov dword [PAGE_TABLE.Low2], PAGE_TABLE.Low1 + 0x3
    mov dword [PAGE_TABLE.Low1], PAGE_TABLE.Low0 + 0x3
    mov eax, 0x3
    mov edi, PAGE_TABLE.Low0
.loop:
    mov [edi], eax
    add eax, 0x1000
    add edi, 8
    cmp eax, 0x100000 ; Only map one megabyte
    jb .loop
    mov [edi], eax
    lidt [IDT.Pointer]
    mov eax, 0x000000A0
    mov cr4, eax
    mov eax, PAGE_TABLE.Root
    mov cr3, eax
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x00000100
    wrmsr
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov byte [GDT.CF], 0xAF
    mov byte [GDT.DF], 0xAF
    jmp GDT.Code:start_64

[bits 64]
extern _START

start_64:
    call _START
    hlt

section .data

GDT:
.Null equ $ - GDT
    dq 0x0000000000000000
.Code equ $ - GDT
    dw 0xFFFF
    dd 0x9A000000
.CF:
    db 0xCF
    db 0x00
.Data equ $ - GDT
    dw 0xFFFF
    dd 0x92000000
.DF:
    db 0xCF
    db 0x00
.Pointer:
    dw $ - GDT - 1
    dd GDT

IDT:
.Pointer:
    dw $ - IDT - 1
    dd IDT

STRING:
.start:
    db "BOOT STAGE 1...", `\n\0`

section .bss.paged  nobits  write

PAGE_TABLE:
.Root:
    resb 0x1000
.Low2:
    resb 0x1000
.Low1:
    resb 0x1000
.Low0:
    resb 0x1000
