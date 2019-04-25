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

void setpin(int pin);
void clearpin(int pin);
void nano_wait(unsigned int n);
void enable_gpioc(void);
int testShift();

void setpin(int pin){
    GPIOC->ODR |= 1<<pin;
    return;
}
void clearpin(int pin){
    GPIOC->ODR &= ~(1<<pin);
    return;
}

void enable_gpioc(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(3<<2*2);
    GPIOC->MODER &= ~(3<<0);

    GPIOC->MODER |= 1<<2*2;
    GPIOC->MODER |= 1<<0;

    return;
}
int testShift(void){
    //=======================================================
    // "bit bang" the value of data bit by bit from LSB to
    // MSB into PC2, while also generating a bit banged serial
    // clock. Bit banging is embedded speak for toggling the
    // output depending on the current bit. As an example
    // consider 0b1001 0010, bit banging this value will set
    // PC2 output to the following values logic 0, logic 1,
    // logic 0, logic 0, logic 1, logic 0, logic 0, logic 1.
    // This is bit banging from LSB to MSB.

        enable_gpioc();
        int bit = 0;
        int buttons[8] = {1,1,1,1,1,1,1,1};
        for(bit = 0; bit < 8; bit++){
            if(buttons[bit] == 1){
                setpin(2);
            }
            else{
                clearpin(2);
            }
            setpin(0);
            nano_wait(200);
            clearpin(0);
            nano_wait(200);
        }
}
