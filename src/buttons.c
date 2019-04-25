#include "stm.h"

#define SAMPLE_TIME_MS 10
#define SAMPLE_COUNT (SAMPLE_TIME_MS)

#define THRESHOLD_TIME_MS 2
#define THRESHOLD (THRESHOLD_TIME_MS)

#define KEY_PRESS_MASK  0b11000111
#define KEY_REL_MASK    0b11100011
int col = 0;
int int_array[5][3] = { {0, 1, 2},
                         {3, 4, 5},
                         {6, 7, 8},
                         {9, 10, 11},
                         {12, 13, 14} };

uint8_t key_samples[5][3]  = { {0}, {0}, {0}, {0}, {0} };
uint8_t key_pressed[5][3]  = { {0}, {0}, {0}, {0}, {0} };
uint8_t key_released[5][3]  = { {0}, {0}, {0}, {0}, {0} };

int getSlot(){
    setup_button_matrix();
    setup_timer3();
    while(!gameOver()){
        int slot = get_button_pressed();
        if(slot != -1){
            return slot;
      }
    }
    return -1;
}

int start(){
    setup_button_matrix();
    setup_timer3();
    int offset = 0;
    while(1){
        const char * msg = "              Press any button to start             ";
        display1_line2(&msg[offset]);
        nano_wait(100000000);
        offset++;
        if(offset >= 50){ offset = 0; }
        int slot = get_button_pressed();
        if(slot != -1){
            return slot;
      }
    }
    return -1;
}

void setup_button_matrix() {
    // Enable Port A & B
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;

    // columns
    GPIOA -> MODER &= ~(3 << 2*0);
    GPIOA -> MODER |= (1 << 2*0);
    GPIOA -> MODER &= ~(3 << 2*1);
    GPIOA -> MODER |= (1 << 2*1);
    GPIOA -> MODER &= ~(3 << 2*2);
    GPIOA -> MODER |= (1 << 2*2);

    // rows
    GPIOB -> MODER &= ~(3 << 2*0);
    GPIOB -> MODER &= ~(3 << 2*1);
    GPIOB -> MODER &= ~(3 << 2*2);
    GPIOB -> MODER &= ~(3 << 2*3);
    GPIOB -> MODER &= ~(3 << 2*4);

    GPIOB -> PUPDR &= ~(3 << 2*0);
    GPIOB -> PUPDR |= (2 << 2*0);
    GPIOB -> PUPDR &= ~(3 << 2*1);
    GPIOB -> PUPDR |= (2 << 2*1);
    GPIOB -> PUPDR &= ~(3 << 2*2);
    GPIOB -> PUPDR |= (2 << 2*2);
    GPIOB -> PUPDR &= ~(3 << 2*3);
    GPIOB -> PUPDR |= (2 << 2*3);
    GPIOB -> PUPDR &= ~(3 << 2*4);
    GPIOB -> PUPDR |= (2 << 2*4);
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

void update_samples(int row) {
    for(int i = 0; i < 5; i++) {
        uint8_t curr_history = key_samples[i][col];
        curr_history = curr_history << 1;

        if(row == i)
            curr_history |= 1;

        key_samples[i][col] = curr_history;
    }
}

void update_button_press() {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 3; j++) {
            if ((key_samples[i][j] & KEY_PRESS_MASK) == 0b00000111) {
                key_pressed[i][j] = 1;
                key_samples[i][j] = 0xFF;
            }

            if ((key_samples[i][j] & KEY_REL_MASK) == 0b11100000) {
                key_released[i][j] = 1;
                key_samples[i][j] = 0x00;
            }
        }
    }
}

int get_button_pressed() {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 3; j++) {
            if(key_released[i][j] == 1 && key_pressed[i][j] == 1) {
                key_released[i][j] = 0;
                key_pressed[i][j] = 0;
                return int_array[i][j];
            }
        }
    }
    return -1;
}


void TIM3_IRQHandler()
{
    int current_row = -1;
    int input = (GPIOB -> IDR);
    input &= 31;
    if(input == 0b00001){ current_row = 0; }
    else if(input == 0b00010){ current_row = 1; }
    else if(input == 0b00100){ current_row = 2; }
    else if(input == 0b01000){ current_row = 3; }
    else if(input == 0b10000){ current_row = 4; }
    update_samples(current_row);
    update_button_press();
    col++;
    if(col > 2){ col = 0; }
    GPIOA -> ODR = 1 << col;
    TIM3 -> SR &= ~(1);
}
