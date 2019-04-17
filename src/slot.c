#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

void testLED();
void testButton();
void setup_pwm();
void setup_gpio();
void changeLED(int r, int g);
void setup_button();

void testLED(){
    setup_gpio();
    setup_pwm();
    changeLED(20, 20);
}

void testButton(){
    setup_button();
}

void setup_pwm() {
    RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1 -> BDTR |= 1 << 15;
    TIM1 -> CR1 &= ~TIM_CR1_DIR;
    TIM1 -> CR1 &= ~TIM_CR1_CMS;
    TIM1 -> CCMR1 |= TIM_CCMR1_OC2PE;
    TIM1 -> CCMR2 |= TIM_CCMR2_OC3PE;
    TIM1 -> CCMR1 |= TIM_CCMR1_OC2M;
    TIM1 -> CCMR2 |= TIM_CCMR2_OC3M;
    TIM1 -> PSC = 480 - 1;
    TIM1 -> ARR = 100 - 1;
    TIM1 -> CCER |= TIM_CCER_CC2E;
    TIM1 -> CCER |= TIM_CCER_CC3E;
    TIM1 -> CR1 |= TIM_CR1_CEN;
}

void setup_gpio() {
    // Enable Port A
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    // Set the mode for PA9,10 for alt function
    GPIOA -> MODER &= ~(3<<(2*9));
    GPIOA -> MODER |= 2<<(2*9);

    GPIOA -> MODER &= ~(3<<(2*10));
    GPIOA -> MODER |= 2<<(2*10);

    GPIOA -> AFR[1] &= ~(0xf<<(1*4));
    GPIOA -> AFR[1] |= 0x2<<(1*4);

    GPIOA -> AFR[1] &= ~(0xf<<(2*4));
    GPIOA -> AFR[1] |= 0x2<<(2*4);
}

void setup_button(){
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> MODER &= ~(3<<(2*13));
    GPIOB -> PUPDR &= ~(3 << 2*13);
    GPIOB -> PUPDR |= (2 << 2*13);
}

void changeLED(int r, int g){
    TIM1 -> CCR2 = g;
    TIM1 -> CCR3 = r;
}

void setup_timer3() {
    // Enable sys clock for timer 3
    RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Prescaler 48MHz (48MHz/1)
    TIM3 -> PSC = 1 - 1;
    // Auto-reload 48000
    TIM3 -> ARR = 48000 - 1;
    TIM3 -> DIER |= TIM_DIER_UIE;
    NVIC -> ISER[0] = 1<<TIM3_IRQn;
    TIM3 -> CR1 |= TIM_CR1_CEN;
}

void TIM3_IRQHandler()
{
    TIM3 -> SR &= ~(1);
    if((GPIOB -> IDR >> 13 & 1) != 0){
        if(TIM1 -> CCR2 == 0){
            changeLED(20, 20);
        }else{
            changeLED(0, 0);
        }
    }
}
