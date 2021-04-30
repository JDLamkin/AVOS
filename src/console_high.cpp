#include "console.h"

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

void printl(const char* str) {
    print(str);
    printl();
}

void print(long val, unsigned long radix, const char* digits) {
    if(val < 0) {
        val = -val;
        print('-');
    }
    print((unsigned long) val, radix, digits);
}
void print(unsigned long val, unsigned long radix, const char* digits) {
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
void print(double val, unsigned long radix, const char* digits) {
    
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
    print_format_arg(format, (long) arg);
}

void print_format_arg(const char*& format, unsigned int arg) {
    print_format_arg(format, (unsigned long) arg);
}

void print_format_arg(const char*& format, long arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'c': print((char) arg); break;
            case 'd': print(arg); break;
            case 'x': print("0x"); print(arg, 16); break;
            case 'f': print((double) arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, unsigned long arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'c': print((char) arg); break;
            case 'd': print(arg); break;
            case 'x': print("0x"); print(arg, 16); break;
            case 'f': print((double) arg); break;
            case 's': print(arg); break;
            default: return;
        }
        format += 2;
    }
}

void print_format_arg(const char*& format, double arg) {
    consume(format);
    if(format[0] == '%') {
        switch(format[1]) {
            case 'f': print((double) arg); break;
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
