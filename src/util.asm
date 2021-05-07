[bits 64]
section .text

; __stdcall unsigned char  _inport8 (unsigned short);
global _inport8
_inport8:
    mov dx, di
    in al, dx
    ret
; __stdcall unsigned short _inport16(unsigned short);
global _inport16
_inport16:
    mov dx, di
    in ax, dx
    ret
; __stdcall unsigned int   _inport32(unsigned short);
global _inport32
_inport32:
    mov dx, di
    in eax, dx
    ret
; __stdcall _outport8 (unsigned short, unsigned char );
global _outport8
_outport8:
    mov dx, di
    mov ax, si
    out dx, al
    ret
; __stdcall _outport16(unsigned short, unsigned short);
global _outport16
_outport16:
    mov dx, di
    mov ax, si
    out dx, ax
    ret
; __stdcall _outport32(unsigned short, unsigned int  );
global _outport32
_outport32:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

; __stdcall void _memset_raw  (void* dst, unsigned long v, unsigned long length);
global _memset_raw
_memset_raw:
    mov rax, rsi
    mov rcx, rdx
    cld
    rep stosq
    ret
; __stdcall void _memcpy_inc8 (void* dst, const void* src, unsigned long length);
global _memcpy_inc8
_memcpy_inc8:
    mov rcx, rdx
    cld
    rep movsb
    ret
; __stdcall void _memcpy_inc16(void* dst, const void* src, unsigned long length);
global _memcpy_inc16
_memcpy_inc16:
    mov rcx, rdx
    cld
    rep movsw
    ret
; __stdcall void _memcpy_inc32(void* dst, const void* src, unsigned long length);
global _memcpy_inc32
_memcpy_inc32:
    mov rcx, rdx
    cld
    rep movsd
    ret
; __stdcall void _memcpy_inc64(void* dst, const void* src, unsigned long length);
global _memcpy_inc64
_memcpy_inc64:
    mov rcx, rdx
    cld
    rep movsq
    ret
; __stdcall void _memcpy_dec8 (void* dst, const void* src, unsigned long length);
global _memcpy_dec8
_memcpy_dec8:
    mov rcx, rdx
    std
    rep movsb
    ret
; __stdcall void _memcpy_dec16(void* dst, const void* src, unsigned long length);
global _memcpy_dec16
_memcpy_dec16:
    mov rcx, rdx
    std
    rep movsw
    ret
; __stdcall void _memcpy_dec32(void* dst, const void* src, unsigned long length);
global _memcpy_dec32
_memcpy_dec32:
    mov rcx, rdx
    std
    rep movsd
    ret
; __stdcall void _memcpy_dec64(void* dst, const void* src, unsigned long length);
global _memcpy_dec64
_memcpy_dec64:
    mov rcx, rdx
    std
    rep movsq
    ret
