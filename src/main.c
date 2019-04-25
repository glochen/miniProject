/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

void nano_wait(unsigned int n);

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
    startGame();
}

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void printLCD(int lcd, int line, char * msg, int len){
    if(lcd == 1){                       // LCD 1
        if(line == 1){                  // line 1
            if(len <= 20){              // static
                display1_line1(msg);
            }else{                      // scrolling
                    for(int offset = 0; offset < len; offset++) {
                        display1_line1(&msg[offset]);
                        nano_wait(100000000);
                    }
            }
        }else{                          // line 2
            if(len <= 20){              // static
                display1_line2(msg);
            }else{                      // scrolling
                for(int offset = 0; offset < len; offset++) {
                    display1_line2(&msg[offset]);
                    nano_wait(100000000);
                }
            }
        }
    }else{                              // LCD 2
        if(line == 1){                  // line 1
            if(len <= 20){              // static
                display2_line1(msg);
            }else{                      // scrolling
                for(int offset = 0; offset < len; offset++) {
                    display2_line1(&msg[offset]);
                    nano_wait(100000000);
                }
            }
        }else{                          // line 2
            if(len <= 20){              // static
                display2_line2(msg);
            }else{                      // scrolling
                for(int offset = 0; offset < len; offset++) {
                    display2_line2(&msg[offset]);
                    nano_wait(100000000);
                }
            }
        }
    }

}
