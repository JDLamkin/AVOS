; Provides basic output ability (16 bit) interfacing with BIOS, not including the print string
; utility in the boot sector.

section .text

[bits 16]

extern print_string_16

; void stdcall(ptr16, uint16)
; print_hex_16(addr, length)
global print_hex_16
print_hex_16:
    push bp
    mov bp, sp
    push si
    mov dx, [bp + 4]
    mov si, [bp + 6]
    mov ax, 0x0E30
    int 0x10
    cmp si, 0
    je .retn
    mov al, 0x78
    int 0x10
    push bx
.start:
    sub si, 1
    jc .end
    mov bx, dx
    mov al, [bx + si]
    xor ch, ch
    mov cl, al
    mov bx, cx
    shr bx, 4
    mov al, [digits + bx]
    int 0x10
    mov bx, cx
    and bx, 0xF
    mov al, [digits + bx]
    int 0x10
    jmp .start
.end:
    pop bx
.retn:
    pop si
    pop bp
    ret 4

section .rodata

digits:
    db "0123456789ABCDEF"
