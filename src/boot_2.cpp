#include "console.h"

extern "C" { void START(); }

using namespace BootConsole;

double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164;

void START() {
    init();
    printl("BOOT STAGE 2...");
    printf("TEST: pi = %f%nn = %d = %x%n%A0PCOLOR%P ok done%n", PI, 17, 17);
}
