; Boot Stage 1; See README.md

section .text

[bits 16]

MAX_MEM equ 0x40

extern print_string_16
extern print_hex_16
extern load_mem_entries

global start_16
start_16:
    push STRING.start
    call print_string_16
    push WORD MAX_MEM
    push MEM_TABLE
    call load_mem_entries
    push ax
    mov ax, sp
    push WORD 2
    push ax
    call print_hex_16
    push STRING.newline
    call print_string_16
    pop di
    mov bx, MEM_TABLE
.loop:
    sub di, 1
    jc .done
    mov si, 8
    call .print_hex_entry
    call .print_hex_entry
    mov si, 4
    call .print_hex_entry
    call .print_hex_entry
    push STRING.newline
    call print_string_16
    jmp .loop
.print_hex_entry:
    push si
    push bx
    call print_hex_16
    push STRING.space
    call print_string_16
    add bx, si
    ret
.done:
    jmp $

[bits 32]
start_32:
    jmp $

[bits 64]
extern _START

start_64:
    call _START
    jmp $

global _boot_print_low
_boot_print_low:
    ret

section .data

STRING:
.start:
    db "BOOT STAGE 1..."
.newline:
    db `\n\0`
.space:
    db ` \0`

;GDT:
;.Null:
;    dq 0x0000000000000000
;.Code:
;    dq 0x00209A0000000000
;    dq 0x0000920000000000
;ALIGN 4
;    dw 0
;.Pointer:
;    dw $ - GDT - 1
;    dd GDT

section .bss.low    nobits  write

MEM_TABLE:
    resb 24*MAX_MEM
