; Boot Stage 0; See README.md

[bits 16]

section .text

extern start_16
extern BOOT_SEGMENTS
extern BOOT_1_START

boot_start:
    mov [BOOT_DRIVE], dl
    mov bp, boot_start  ; Create a 0x7700 byte stack
    mov sp, bp          ; just below our boot sector
    push STRING.start
    call print_string_16
    mov ah, 0x02            ; Read from disk
    mov al, BOOT_SEGMENTS   ; Read the linked sector count
    mov bx, BOOT_1_START    ; Read at the linked memory slot
    mov cx, 0x0002          ; Start from cylinder 0, sector 2
    mov dh, 0x00            ; From head 0; Drive already selected
    int 0x13
    jc .disk_read_fail
    cmp al, BOOT_SEGMENTS   ; Check that the right number of sectors are read
    jne .disk_read_fail
    jmp start_16
.disk_read_fail:
    push STRING.boot_drive_read_error
    call print_string_16
    jmp $

; void stdcall(ptr16)
; print_string_16(str)
global print_string_16
print_string_16:
    push bp
    mov bp, sp
    push bx
    mov bx, [bp + 4]
    mov ah, 0x0E
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
.print:
    int 0x10
    cmp al, 0x0A
    je .newline
    inc bx
    jmp .loop
.newline:
    mov al, 0x0D ; Print carriage return
    jmp .print
.done:
    pop bx
    pop bp
    ret 2

section .rodata

STRING:
.start:
    db "BOOT STAGE 0...", `\n\0`
.boot_drive_read_error:
    db "ERROR: Unable to load data from boot disk.", `\n\0`

section .bss.low    nobits  write

BOOT_DRIVE:
    resb 1
