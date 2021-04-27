#include "util_low.h"

void memcpy(void* dst, const void* src, unsigned long length) {
    unsigned char* dst8 = (unsigned char*) dst;
    const unsigned char* src8 = (const unsigned char*) src;
    unsigned long align = dst8 - src8;
    align = align & -align & 0x7; // LSB (up to 64-bit max)
    if(align == 0) align = 0x8;
    if(dst < src) {
        while(length && ((unsigned long)dst8 & (align - 1))) {
            *(dst8++) = *(src8++);
            --length;
        }
        unsigned long reps = length/align;
        length %= align;
        switch(align) {
        case 0x1: memcpy_inc8 (dst8, src8, reps); break;
        case 0x2: memcpy_inc16(dst8, src8, reps); break;
        case 0x4: memcpy_inc32(dst8, src8, reps); break;
        case 0x8: memcpy_inc64(dst8, src8, reps); break;
        }
        dst8 += reps*align;
        src8 += reps*align;
        while(length--) *(dst8++) = *(src8++);
    } else if(dst > src) {
        dst8 += length;
        src8 += length;
        while(length && ((unsigned long)dst8 & (align - 1))) {
            *(--dst8) = *(--src8);
            --length;
        }
        unsigned long reps = length/align;
        length %= align;
        dst8 -= align;
        src8 -= align;
        switch(align) {
        case 0x1: memcpy_dec8 (dst8, src8, reps); break;
        case 0x2: memcpy_dec16(dst8, src8, reps); break;
        case 0x4: memcpy_dec32(dst8, src8, reps); break;
        case 0x8: memcpy_dec64(dst8, src8, reps); break;
        }
        dst8 -= (reps - 1)*align;
        src8 -= (reps - 1)*align;
        while(length--) *(--dst8) = *(--src8);
    }
}

void memset8 (void* dst, unsigned char  v, unsigned long length) {
    unsigned char* ptr = (unsigned char*) dst;
    while(length && (unsigned long) ptr & 0x7) {
        *(ptr++) = v;
        --length;
    }
    unsigned long v64 = v * 0x0101010101010101L;
    memset_raw(ptr, v64, length/8);
    v64 = length % 8;
    ptr += length - v64;
    length = v64;
    while(length--) *(ptr++) = v;
}

void memset16(void* dst, unsigned short v, unsigned long length) {
    length <<= 1;
    unsigned char* ptr = (unsigned char*) dst;
    while(length && (unsigned long) ptr & 0x7) {
        unsigned char vc = v & 0xFF;
        v = (v >> 8) | (vc << 8);
        *(ptr++) = vc;
        --length;
    }
    unsigned long v64 = v * 0x0001000100010001L;
    memset_raw(ptr, v64, length/8);
    v64 = length % 8;
    ptr += length - v64;
    length = v64;
    while(length--) {
        unsigned char vc = v & 0xFF;
        v = (v >> 8) | (vc << 8);
        *(ptr++) = vc;
    }
}
