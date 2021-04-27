#ifndef OS_CONSOLE_H
#define OS_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

enum bc_color {
    bc_black    = 0x0,
    bc_d_blue   = 0x1,
    bc_d_green  = 0x2,
    bc_d_cyan   = 0x3,
    bc_d_red    = 0x4,
    bc_d_mag    = 0x5,
    bc_d_yellow = 0x6,
    bc_l_gray   = 0x7,
    bc_d_gray   = 0x8,
    bc_l_blue   = 0x9,
    bc_l_green  = 0xA,
    bc_l_cyan   = 0xB,
    bc_l_red    = 0xC,
    bc_l_mag    = 0xD,
    bc_l_yellow = 0xE,
    bc_white    = 0xF,
};

void boot_console_init();
void boot_console_color(enum bc_color fg, enum bc_color bg);
void boot_print_char(char);
void boot_print(const char*);

#ifdef __cplusplus
}
#endif

#endif
