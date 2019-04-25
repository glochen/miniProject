#include "stm.h"

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

int getMode(){
    return mode;
}

void TIM15_IRQHandler() {
    TIM15 -> SR &= ~(1);
    int x = read_adc_channel();
    if(x < 1000){
        mode = 0;
        display2_line1("GAMEMODE: Easy");
        display2_line2("Hints Enabled");
    }else if(x < 3000){
        mode = 1;
        display2_line1("GAMEMODE: Normal");
        display2_line2("Hints Disabled");
    }else{
        mode = 2;
        display2_line1("GAMEMODE: Hard");
        display2_line2("5 Min Limit");
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

void endTimer15(){
    TIM15 -> CR1 &= ~(TIM_CR1_CEN);
}
