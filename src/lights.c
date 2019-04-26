#include "stm.h"

void setup_shift(){
    RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC -> MODER |= 1 << 0;
    GPIOC -> MODER |= 1 << 2*1;
    GPIOC -> MODER |= 1 << 2*2;
}

void setLights(Slot * lights){
    for(int i = 14; i >= 0; i--){
        if(lights[i].color == 0){               // 1 1 (off)
            GPIOC -> ODR |= 1 << 2;     // send 1
            GPIOC -> ODR &= ~(1 << 1);
            GPIOC -> ODR |= 1;
            nano_wait(200);
            GPIOC -> ODR &= ~1;
            GPIOC -> ODR |= 1 << 1;
            nano_wait(200);
            GPIOC -> ODR |= 1 << 2;     // send 1
        }else if(lights[i].color == 1){         // 0 1 (red)
            GPIOC -> ODR &= ~(1 << 2);  // send 0
            GPIOC -> ODR &= ~(1 << 1);
            GPIOC -> ODR |= 1;
            nano_wait(200);
            GPIOC -> ODR &= ~1;
            GPIOC -> ODR |= 1 << 1;
            nano_wait(200);
            GPIOC -> ODR |= 1 << 2;     // send 1
        }else if(lights[i].color == 2){         // 1 0 (green)
            GPIOC -> ODR |= 1 << 2;     // send 1
            GPIOC -> ODR &= ~(1 << 1);
            GPIOC -> ODR |= 1;
            nano_wait(200);
            GPIOC -> ODR &= ~1;
            GPIOC -> ODR |= 1 << 1;
            nano_wait(200);
            GPIOC -> ODR &= ~(1 << 2);  // send 0
        }else{                          // 0 0 (yellow)
            GPIOC -> ODR &= ~(1 << 2);  // send 0
            GPIOC -> ODR &= ~(1 << 1);
            GPIOC -> ODR |= 1;
            nano_wait(200);
            GPIOC -> ODR &= ~1;
            GPIOC -> ODR |= 1 << 1;
            nano_wait(200);
            GPIOC -> ODR &= ~(1 << 2);  // send 0
        }
        GPIOC -> ODR &= ~(1 << 1);
        GPIOC -> ODR |= 1;
        nano_wait(200);
        GPIOC -> ODR &= ~1;
        GPIOC -> ODR |= 1 << 1;
        nano_wait(200);
    }
}

void testLights(){
    for(int i = 0; i < 32; i++){
        if(i % 2 == 0){
            GPIOC -> ODR &= ~(1 << 2);     // send 1
        }else{
            GPIOC -> ODR |= (1 << 2);     // send 1
        }
        GPIOC -> ODR &= ~(1 << 1);
        GPIOC -> ODR |= 1;
        nano_wait(200);
        GPIOC -> ODR &= ~1;
        GPIOC -> ODR |= 1 << 1;
        nano_wait(200);
    }
}


