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
    //testLED();
    //testButton();

    //testShift();

    //LCD 1
    //timer();

    testADC();

    //LCD 2
    //welcome();
    //selectPeg(1);
    //invalidSpot();
    //selectSpot(15);
    //end(3);
}

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}
