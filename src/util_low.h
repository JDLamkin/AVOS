#ifndef OS_UTIL_LOW_H
#define OS_UTIL_LOW_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char  inport8 (unsigned short);
extern unsigned short inport16(unsigned short);
extern unsigned int   inport32(unsigned short);
extern void outport8 (unsigned short, unsigned char );
extern void outport16(unsigned short, unsigned short);
extern void outport32(unsigned short, unsigned int  );

extern void memset_raw  (void* dst, unsigned long v, unsigned long length);
extern void memcpy_inc8 (void* dst, const void* src, unsigned long length);
extern void memcpy_inc16(void* dst, const void* src, unsigned long length);
extern void memcpy_inc32(void* dst, const void* src, unsigned long length);
extern void memcpy_inc64(void* dst, const void* src, unsigned long length);
extern void memcpy_dec8 (void* dst, const void* src, unsigned long length);
extern void memcpy_dec16(void* dst, const void* src, unsigned long length);
extern void memcpy_dec32(void* dst, const void* src, unsigned long length);
extern void memcpy_dec64(void* dst, const void* src, unsigned long length);

void memcpy(void* dst, const void* src, unsigned long length);
void memset8 (void* dst, unsigned char  v, unsigned long length);
void memset16(void* dst, unsigned short v, unsigned long length);
//void memset32(void* dst, unsigned int   v, unsigned long length);
//void memset64(void* dst, unsigned long  v, unsigned long length);

#ifdef __cplusplus
}
#endif

#endif
