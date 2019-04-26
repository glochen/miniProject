#include "stm.h"

int main(void)
{
    // lcd setup
    init_lcd1();
    init_lcd2();
    // led setup
    setup_shift();
    // button setup
    setup_button_matrix(); // set up push button matrix
    setup_timer3(); // set up timer for button matrix
    // set up adc
    setup_adc();
    setup_timer15();
//    testLights();
    startGame();
}

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}
