#include "stm.h"

uint16_t dispmem1[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

uint16_t dispmem2[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

void display1_line1(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
            dispmem1[x+1] = s[x] | 0x200;
        else
            break;
    for(  ; x<16; x+=1)
        dispmem1[x+1] = 0x220;
}

void display1_line2(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x] != '\0')
            dispmem1[x+18] = s[x] | 0x200;
        else
            break;
    for(   ; x<16; x+=1)
        dispmem1[x+18] = 0x220;
}

void init_lcd1(void) {
    // Enable Port B
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    // Set the mode for PA4,PA5,PA7 for alt function 0
    GPIOA -> MODER &= ~(3<<(2*4));
    GPIOA -> MODER |= 2<<(2*4);

    GPIOA -> MODER &= ~(3<<(2*5));
    GPIOA -> MODER |= 2<<(2*5);

    GPIOA -> MODER &= ~(3<<(2*7));
    GPIOA -> MODER |= 2<<(2*7);

    GPIOA -> AFR[0] &= ~(0xf<<(4*4));
    GPIOA -> AFR[0] &= ~(0xf<<(5*4));
    GPIOA -> AFR[0] &= ~(0xf<<(7*4));

    // SPI2
    RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1 -> CR1 |= SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

    SPI1 -> CR2 =  SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI1 -> CR1 |= SPI_CR1_SPE;

    lcd1_startup();

    //DMA
    RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
    SPI1 -> CR2 =  SPI_CR2_TXDMAEN | SPI_CR2_SSOE| SPI_CR2_FRXTH | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    DMA1_Channel3 -> CPAR = (uint32_t)(&(SPI1 -> DR));
    DMA1_Channel3 -> CMAR = (uint32_t)(dispmem1);
    DMA1_Channel3 -> CNDTR = 34;
    DMA1_Channel3 -> CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_DIR | DMA_CCR_CIRC | DMA_CCR_TEIE;
    DMA1_Channel3 -> CCR &= (~DMA_CCR_PL);
    DMA1_Channel3 -> CCR |= DMA_CCR_EN;
}

void spi1_cmd(char b) {
    // Your code goes here.
    while((SPI1 -> SR & SPI_SR_TXE) == 0);
    SPI1 -> DR = b;
}

void spi1_data(char b) {
    // Your code goes here.
    while((SPI1 -> SR & SPI_SR_TXE) == 0);
    SPI1 -> DR = 0x200 + b;
}

void lcd1_startup(void) {
    nano_wait(100000000); // Give it 100ms to initialize
    spi1_cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    spi1_cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    spi1_cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    spi1_cmd(0x02);  // put the cursor in the home position
    spi1_cmd(0x06);  // 0000 01IS: set display to increment
}

void display2_line1(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
            dispmem2[x+1] = s[x] | 0x200;
        else
            break;
    for(  ; x<16; x+=1)
        dispmem2[x+1] = 0x220;
}

void display2_line2(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x] != '\0')
            dispmem2[x+18] = s[x] | 0x200;
        else
            break;
    for(   ; x<16; x+=1)
        dispmem2[x+18] = 0x220;
}

void init_lcd2(void) {
    // Enable Port B
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set the mode for PB12,13,15 for alt function 0
    GPIOB -> MODER &= ~(3<<(2*12));
    GPIOB -> MODER |= 2<<(2*12);

    GPIOB -> MODER &= ~(3<<(2*13));
    GPIOB -> MODER |= 2<<(2*13);

    GPIOB -> MODER &= ~(3<<(2*15));
    GPIOB -> MODER |= 2<<(2*15);

    GPIOB -> AFR[1] &= ~(0xf<<(4*4));
    GPIOB -> AFR[1] &= ~(0xf<<(5*4));
    GPIOB -> AFR[1] &= ~(0xf<<(7*4));

    // SPI2
    RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;
    SPI2 -> CR1 |= SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;

    SPI2 -> CR2 =  SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI2 -> CR1 |= SPI_CR1_SPE;
    lcd2_startup();

    //DMA
    RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
    SPI2 -> CR2 =  SPI_CR2_TXDMAEN | SPI_CR2_SSOE| SPI_CR2_FRXTH | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    DMA1_Channel5 -> CPAR = (uint32_t)(&(SPI2 -> DR));
    DMA1_Channel5 -> CMAR = (uint32_t)(dispmem2);
    DMA1_Channel5 -> CNDTR = 34;
    DMA1_Channel5 -> CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_DIR | DMA_CCR_CIRC | DMA_CCR_TEIE;
    DMA1_Channel5 -> CCR &= (~DMA_CCR_PL);
    DMA1_Channel5 -> CCR |= DMA_CCR_EN;
}

void spi2_cmd(char b) {
    // Your code goes here.
    while((SPI2 -> SR & SPI_SR_TXE) == 0);
    SPI2 -> DR = b;
}

void spi2_data(char b) {
    // Your code goes here.
    while((SPI2 -> SR & SPI_SR_TXE) == 0);
    SPI2 -> DR = 0x200 + b;
}

void lcd2_startup(void) {
    nano_wait(100000000); // Give it 100ms to initialize
    spi2_cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    spi2_cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    spi2_cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    spi2_cmd(0x02);  // put the cursor in the home position
    spi2_cmd(0x06);  // 0000 01IS: set display to increment
}
