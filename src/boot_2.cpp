#include "console.h"

extern "C" { void START(); }

using namespace BootConsole;

void START() {
    init();
    printl("BOOT STAGE 2...");
    printf("pi = %f%nn = %d = %x%n%A0PCOLOR%P ok done%n", 3.141592, 17, 17);
}
