#include "console.h"
#include "util_low.h"

namespace BootConsole {

static uint16* const console_buffer = (uint16*)0xB8000;

static const uint16 CGA_idx = 0x3D4;
static const uint16 CGA_dat = 0x3D5;

static const uint8 NROWS = 25;
static const uint8 NCOLS = 80;
static const uint16 CSIZE = NROWS*NCOLS;

static uint16 cursor;
static uint8 color;
static uint8 base_col;

static void update_cursor() {
    uint8 clow, chi;
    clow = (uint8)(cursor & 0xFF);
    chi  = (uint8)(cursor >> 8);
    outport8(CGA_idx, 0x0F);
    outport8(CGA_dat, clow);
    outport8(CGA_idx, 0x0E);
    outport8(CGA_dat, chi);
}

static void set_char(char c) {
    console_buffer[cursor] = color << 8 | c;
}

static void clear() {
    clear_color();
    uint16 data = color << 8 | ' ';
    memset16(console_buffer, data, CSIZE);
}

static void do_scroll(int8 rows) {
    uint8 nclr = rows < 0 ? -rows : rows;
    uint8 ncpy = NROWS - nclr;
    uint64 cpy_length = ncpy * NCOLS * 2;
    uint64 clr_length = nclr * NCOLS;
    uint16* offset_region = console_buffer + clr_length;
    uint16 data = color << 8 | ' ';
    if(rows > 0) {
        memcpy(console_buffer, offset_region, cpy_length);
        memset16(console_buffer + ncpy * NCOLS, data, clr_length);
    } else if(rows < 0) {
        memcpy(offset_region, console_buffer, cpy_length);
        memset16(console_buffer, data, clr_length);
    }
}

void set_pos(int x, int y) {
    int16 pos = x + y*NCOLS;
    cursor = x + y*NCOLS;
    int8 scroll = 0;
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

void get_pos(int& x, int& y) {
    x = cursor % NCOLS;
    y = cursor / NCOLS;
}

void move(int x, int y) {
    set_pos(cursor + x, y);
}

void print(char c) {
    if(c >= 0x20 && c < 0x7F) {
        set_char(c);
        move(1, 0);
    } else switch(c) {
    case 0x8:
        if(cursor % NCOLS == 0) break;
        move(-1, 0);
        set_char(' ');
        break;
    case 0x9: move(4, 0); break;
    case 0xA: move(0, 1); break;
    case 0xB: move(0, 4); break;
    case 0xC: clear(); set_pos(0, 0); break;
    case 0xD: move(-(cursor % NCOLS), 0); break;
    }
}

void init() {
    uint8 clow, chi;
    outport8(CGA_idx, 0x0F);
    clow = inport8(CGA_dat);
    outport8(CGA_idx, 0x0E);
    chi  = inport8(CGA_dat);
    cursor = (chi << 8) | clow;
    base_col = console_buffer[cursor] >> 8;
    clear_color();
}

void clear_color() {
    color = base_col;
}

void set_color(Color fg, Color bg) {
    color = (int)bg << 4 | (int)fg;
}

void get_color(Color& fg, Color& bg) {
    bg = Color(color >> 4);
    fg = Color(color & 0xF);
}

}
