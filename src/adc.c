#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

void testADC(void);
void setup_adc();
int read_adc_channel();
void setup_timer15();

int prevMode = -1;

void testADC(void){
    //PC3 ADCIN_13
    setup_adc();
    setup_timer15();

}

void setup_adc() {
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable clock to ADC unit
    RCC -> CR2 |= RCC_CR2_HSI14ON; // Turn on Hi - spd internal 14 MHz clock
    while(!(RCC -> CR2 & RCC_CR2_HSI14RDY)); // wait for 14 MHz clock to be ready
    ADC1 -> CR |= ADC_CR_ADEN; // Enable ADC
    while(!(ADC1 -> ISR & ADC_ISR_ADRDY)) // Wait for ADC to be ready
    while((ADC1 -> CR & ADC_CR_ADSTART)); // Wait for ADCstart to be 0
}

int read_adc_channel() {
    ADC1 -> CHSELR = 0;
    ADC1 -> CHSELR |= 1 << 13;
    while(!(ADC1 -> ISR & ADC_ISR_ADRDY));
    ADC1 -> CR |= ADC_CR_ADSTART;
    while(!(ADC1 -> ISR & ADC_ISR_EOC));
    return ADC1 -> DR;
}

void TIM15_IRQHandler() {
    TIM15 -> SR &= ~(1);
    int x = read_adc_channel();
    if((x >= 1800 && prevMode == 1) || prevMode == -1){
        selectMode(2);
        prevMode = 2;
    }
    else if((x < 1800 && prevMode == 2) || prevMode == -1){
        selectMode(1);
        prevMode = 1;
    }
}

void setup_timer15() {
    // Enable sys clock for timer 3
    RCC -> APB2ENR |= RCC_APB2ENR_TIM15EN;
    // Counting direction: 0 = up, 1=down
    TIM15 -> CR1 &= ~TIM_CR1_DIR;
    // Prescaler 48MHz (48MHz/1)
    TIM15 -> PSC = 1 - 1;
    // Auto-reload 9600
    TIM15 -> ARR = 9600 - 1;
    TIM15 -> DIER |= TIM_DIER_UIE;
    NVIC -> ISER[0] = 1<<TIM15_IRQn;
    TIM15 -> CR1 |= TIM_CR1_CEN;
}
