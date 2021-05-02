#include "console.h"
#include "utypes.h"

namespace BootConsole {

void print(const char* str) {
    while(*str) {
        print(*str);
        ++str;
    }
}

void printl() {
    print('\r');
    print('\n');
}

void print(int64 val, uint64 radix, const char* digits) {
    if(val < 0) {
        val = -val;
        print('-');
    }
    print((uint64) val, radix, digits);
}
void print(uint64 val, uint64 radix, const char* digits) {
    // ASSERT(radix >= 2)
    static const unsigned int b_size = 65;
    char buffer[b_size];
    char* str = buffer + b_size;
    if(val == 0) {
        print(digits[0]);
        return;
    }
    *--str = '\0';
    while(val) {
        *--str = digits[val % radix];
        val /= radix;
    }
    print(str);
}
static unsigned int dcount(double& val, double rad) {
    if(val < rad) return 0;
    unsigned int d = 2*dcount(val, rad*rad);
    if(val >= rad) {
        val /= rad;
        ++d;
    }
    return d;
}
void pndig(double& val, uint64 radix, const char* digits) {
    uint64 v = (uint64)val;
    if(v >= radix) v = radix - 1;
    print(digits[v]);
    val -= v;
    val *= radix;
}
void print(double val, uint64 radix, const char* digits) {
    if(val != val) {
        print("NaN");
        return;
    }
    if(val < 0) {
        val = -val;
        print('-');
    }
    if(val > 1 && val/radix == val) {
        print("Infinity");
        return;
    }
    double v = val;
    unsigned int dcnt = dcount(v, radix) + 1;
    while(dcnt--) pndig(val, radix, digits);
    if(val != 0) print('.');
    dcnt = 16;
    while(val != 0 && dcnt--) pndig(val, radix, digits);
}

static bool deCol(Color& c, char e) {
    if('0' <= e && e <= '9') {
        c = Color(e - '0' + 0x0);
        return true;
    }
    if('A' <= e && e <= 'F') {
        c = Color(e - 'A' + 0xA);
        return true;
    }
    return false;
}

static void consume(const char*& format) {
    for(; *format; ++format) if(*format == '%') {
        switch(*++format) {
            case '%': print('%'); break;
            case 'n': printl(); break;
            case 'P': clear_color(); break;
            default: {
                Color f, b;
                if(deCol(f, format[0]) && deCol(b, format[1]) && format[2] == 'P')
                    set_color(f, b);
                else {
                    --format;
                    return;
                }
                format += 2;
            }
        }
    } else print(*format);
}

void print_format_arg(const char*& format, int arg) {
    print_format_arg(format, (int64) arg);
}

void print_format_arg(const char*& format, unsigned int arg) {
    print_format_arg(format, (uint64) arg);
}

void print_format_arg(const char*& format, int64 arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'c': print((char) arg); break;
            case 'd': print(arg); break;
            case 'x': print("0x"); print(arg, 16); break;
            case 'f': print((float64) arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, uint64 arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'c': print((char) arg); break;
            case 'd': print(arg); break;
            case 'x': print("0x"); print(arg, 16); break;
            case 'f': print((float64) arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, float64 arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'f': print(arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, char arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'c': print(arg); break;
            case 'd': print((long) arg); break;
            case 'x': print("0x"); print((long) arg, 16); break;
            case 'f': print((double) arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, const char* arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

template<>
void printf(const char* format) {
    consume(format);
    if(*format) return;
}

}
