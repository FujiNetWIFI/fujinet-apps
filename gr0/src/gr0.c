#include <atari.h>
#include <conio.h>
#include <stdio.h>

extern void do_gr0();
extern void debug();

int main(void) {
    printf("before. press a key...");
    cgetc();

    do_gr0();

    printf("after. press a key...");
    cgetc();

    return 0;
}
