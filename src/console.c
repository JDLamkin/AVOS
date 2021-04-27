#include "console.h"
#include "util_low.h"

static unsigned short* const console_buffer = (unsigned short*)0xB8000;

static const unsigned short CGA_idx = 0x3D4;
static const unsigned short CGA_dat = 0x3D5;

static const unsigned char NROWS = 25;
static const unsigned char NCOLS = 80;
static const unsigned short CSIZE = NROWS*NCOLS;

static unsigned short cursor;
static unsigned char color;

static void update_cursor() {
    unsigned char clow, chi;
    clow = (unsigned char)(cursor & 0xFF);
    chi  = (unsigned char)(cursor >> 8);
    outport8(CGA_idx, 0x0F);
    outport8(CGA_dat, clow);
    outport8(CGA_idx, 0x0E);
    outport8(CGA_dat, chi);
}

// Set the raw ascii value at the cursor
static void set_char(char c) {
    console_buffer[cursor] = color << 8 | c;
}

// Fill the screen with ' '
static void clear() {
    unsigned short data = color << 8 | ' ';
    memset16(console_buffer, data, CSIZE);
}

// Scroll rows lines up
static void do_scroll(signed char rows) {
    unsigned char nclr = rows < 0 ? -rows : rows;
    unsigned char ncpy = NROWS - nclr;
    unsigned long cpy_length = ncpy * NCOLS * 2;
    unsigned long clr_length = nclr * NCOLS;
    unsigned short* offset_region = console_buffer + clr_length;
    unsigned short data = color << 8 | ' ';
    if(rows > 0) {
        memcpy(console_buffer, offset_region, cpy_length);
        memset16(console_buffer + ncpy * NCOLS, data, clr_length);
    } else if(rows < 0) {
        memcpy(offset_region, console_buffer, cpy_length);
        memset16(console_buffer, data, clr_length);
    }
}

// Move cursor by absolute coordinates
static void moveA(unsigned short X, unsigned short Y) {
    cursor = X + Y*NCOLS;
    update_cursor();
}

// Move cursor by relative coordinates
static void moveR(signed short X, signed short Y) {
    signed short pos = cursor;
    pos += X + Y*NCOLS;
    signed char scroll = 0;
    while(pos < 0) {
        --scroll;
        pos += NCOLS;
    }
    while(pos >= CSIZE) {
        ++scroll;
        pos -= NCOLS;
    }
    do_scroll(scroll);
    cursor = pos;
    update_cursor();
}

void boot_print_char(char c) {
    if(c >= 0x20 && c < 0x80) {
        set_char(c);
        moveR(1, 0);
    } else switch(c) {
    case 0x8:
        if(cursor % NCOLS == 0) break;
        moveR(-1, 0);
        set_char(' ');
        break;
    case 0x9: moveR(4, 0); break;
    case 0xA: moveR(0, 1); break;
    case 0xB: moveR(0, 4); break;
    case 0xC: clear(); moveA(0, 0); break;
    case 0xD: moveR(-(cursor % NCOLS), 0); break;
    }
}

void boot_console_init() {
    unsigned char clow, chi;
    outport8(CGA_idx, 0x0F);
    clow = inport8(CGA_dat);
    outport8(CGA_idx, 0x0E);
    chi  = inport8(CGA_dat);
    cursor = (chi << 8) | clow;
    color = console_buffer[cursor] >> 8;
}

void boot_console_color(enum bc_color fg, enum bc_color bg) {
    color = bg << 8 | fg;
}

void boot_print(const char* str) {
    while(*str) {
        if(*str == '\n') boot_print_char('\r');
        boot_print_char(*str);
        ++str;
    }
}
