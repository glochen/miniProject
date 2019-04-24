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

    // adc setup
    setup_adc();
    setup_timer15();

    testLEDs();
    //testButtons();

    // static tests
    /*printLCD(1,1,"display1line1", 13);
    printLCD(1,2,"display1line2", 13);
    printLCD(2,1,"display2line1", 13);
    printLCD(2,2,"display2line2", 13);*/

    // scrolling tests
    /*printLCD(1,1,"                 display1line1 scrolling                  ", 50);
    printLCD(1,2,"                 display1line2 scrolling                  ", 50);
    printLCD(2,1,"                 display2line1 scrolling                  ", 50);
    printLCD(2,2,"                 display2line2 scrolling                  ", 50);*/
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
