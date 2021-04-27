; Provides memory mapping capability interfacing with the BIOS

section .text

[bits 16]

; uint16 stdcall(ptr16, uint16)
; cnt = load_mem_entries(dst_addr, max_cnt)
global load_mem_entries
load_mem_entries:
    push bp
    mov bp, sp
    push ebx
    push di
    push si
    mov di, [bp + 4]
    xor si, si
    xor ebx, ebx
.next:
    cmp si, [bp + 6]
    jae .done
    mov eax, 0x0000E820
    mov edx, 0x534D4150
    mov ecx, 24
    int 0x15
    jc .done
    add di, 24
    inc si
    cmp ebx, 0
    jne .next
.done:
    mov ax, si
    pop si
    pop di
    pop ebx
    pop bp
    ret 4
