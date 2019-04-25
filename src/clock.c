#include "stm.h"

void setup_timer2(void) {
    // Enable sys clock for timer 2
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Prescaler 48MHz (48MHz/1)
    TIM2 -> PSC = 1 - 1;
    // Auto-reload 480000
    TIM2 -> ARR = 48000000 - 1;
    TIM2 -> DIER |= TIM_DIER_UIE;
    NVIC -> ISER[0] = 1<<TIM2_IRQn;
    TIM2 -> CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler()
{
    TIM2 -> SR &= ~(1);
    if(mode == 2){
        clockDown();
    }else{
        clockUp();
    }
}

void clockUp(void) {
    int min = seconds / 60;
    int sec = seconds % 60;
    char line[20];
    sprintf(line, "Time: %02d:%02d", min, sec);
    display1_line1(line);
    seconds += 1;
}

void clockDown(){
    int secLeft = MAX_TIME - seconds;
    int min = secLeft / 60;
    int sec = secLeft % 60;
    char line[20];
    sprintf(line, "Time: %02d:%02d", min, sec);
    display1_line1(line);
    if(secLeft <= 0){
        endTimer2();
        display1_line1("TIME IS UP");
    }
    seconds += 1;
}

void endTimer2(){
    TIM2 -> CR1 &= ~(TIM_CR1_CEN);
}
