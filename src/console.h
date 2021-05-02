#ifndef OS_CONSOLE_H
#define OS_CONSOLE_H

#include "utypes.h"

namespace BootConsole {

static const char* STANDARD_DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

enum class Color {
    black    = 0x0,
    d_blue   = 0x1,
    d_green  = 0x2,
    d_cyan   = 0x3,
    d_red    = 0x4,
    d_mag    = 0x5,
    d_yellow = 0x6,
    l_gray   = 0x7,
    d_gray   = 0x8,
    l_blue   = 0x9,
    l_green  = 0xA,
    l_cyan   = 0xB,
    l_red    = 0xC,
    l_mag    = 0xD,
    l_yellow = 0xE,
    white    = 0xF,
};

// Low-level console functions
void init();
void set_color(Color fg, Color bg);
void get_color(Color& fg, Color& bg);
void clear_color();
void move(int dx, int dy);
void set_pos(int x, int y);
void get_pos(int& x, int& y);
void print(char);

// High-level console functions
void print(const char*);
void printl();
void print(  int64, uint64 radix = 10, const char* digits = STANDARD_DIGITS);
void print( uint64, uint64 radix = 10, const char* digits = STANDARD_DIGITS);
void print(float64, uint64 radix = 10, const char* digits = STANDARD_DIGITS);

/*  Printf specification:
 *  The format string is printed as-is except for escape sequences beginning with '%'. Those escape
 *  sequences are listed as follows:
 *   - %%       - print '%'
 *   - %n       - go to the next line
 *   - %c       - print a char argument
 *   - %d       - print a decimal integer
 *   - %x       - print a hexadecimal integer
 *   - %f       - print a float/double argument
 *   - %s       - print a string argument
 *   - %(F)(B)P - set the color: foreground F, background B. Given by their encodings 0-F
 *   - %P       - clear the color.
 *  All sequences are processed in order, and any that accept arguments consume their arguments from
 *  left to right.
 */
template<typename... TArgs>
void printf(const char* format, TArgs... args);

void print_format_arg(const char*&, int);
void print_format_arg(const char*&, unsigned int);
void print_format_arg(const char*&,  int64);
void print_format_arg(const char*&, uint64);
void print_format_arg(const char*&, float64);
void print_format_arg(const char*&, char);
void print_format_arg(const char*&, const char*);
template<> void printf(const char* format);

template<typename T, typename... TArgs>
void printf(const char* format, T arg, TArgs... args) {
    print_format_arg(format, arg);
    printf(format, args...);
}

template<typename T>
void printl(T val) {
    print(val);
    printl();
}

}

#endif
