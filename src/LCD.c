#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

extern void (*cmd)(char b);
extern void (*data)(char b);
extern void (*display1)(const char *);
extern void (*display2)(const char *);

void (*cmd)(char b) = 0;
void (*data)(char b) = 0;
void (*display1)(const char *) = 0;
void (*display2)(const char *) = 0;

void spi_cmd(char);
void spi_data(char);

void dma_spi_init_lcd(void);
void circdma_display1(const char *);
void circdma_display2(const char *);
void generic_lcd_startup(void);
void nano_wait(unsigned int n);

void welcome(void);
void test_bothScroll(void);
void test_bothBlink(void);
void test_bothStatic(void);

uint16_t dispmem[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};
void welcome(){
    cmd = spi_cmd;
    data = spi_data;
    display1 = circdma_display1;
    display2 = circdma_display2;
    dma_spi_init_lcd();
    display1("Welcome!");
    const char *msg2 = "                Created by Team 24               ";
    int offset = 0;
    while(1) {
        display2(&msg2[offset]);
        nano_wait(100000000);
        offset += 1;
        if (offset == 32)
            offset = 0;
    }
}

void test_bothScroll(){
    cmd = spi_cmd;
    data = spi_data;
    display1 = circdma_display1;
    display2 = circdma_display2;
    dma_spi_init_lcd();
    const char *msg1 = "                test moving1...               ";
    const char *msg2 = "                test moving2...               ";
    int offset = 0;
    while(1) {
        display1(&msg1[offset]);
        display2(&msg2[offset]);
        nano_wait(100000000);
        offset += 1;
        if (offset == 32)
            offset = 0;
    }
}

void test_bothBlink(){
    cmd = spi_cmd;
    data = spi_data;
    display1 = circdma_display1;
    display2 = circdma_display2;
    dma_spi_init_lcd();
    int offset = 0;
    while(1) {
        if ((offset / 2) & 1){
            display1("test blink1");
            display2("test blink2");
            nano_wait(1000000000);
        }
        else{
            display1("");
            display2("");
            nano_wait(100000000);
        }
        offset += 1;
        if (offset == 32)
            offset = 0;
    }
}

void test_bothStatic(){
    cmd = spi_cmd;
    data = spi_data;
    display1 = circdma_display1;
    display2 = circdma_display2;
    dma_spi_init_lcd();
    display1("test static1");
    display2("test static2");
}

void circdma_display1(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
            dispmem[x+1] = s[x] | 0x200;
        else
            break;
    for(  ; x<16; x+=1)
        dispmem[x+1] = 0x220;
}

void circdma_display2(const char *s) {
    int x;
    for(x=0; x<16; x+=1)
        if (s[x] != '\0')
            dispmem[x+18] = s[x] | 0x200;
        else
            break;
    for(   ; x<16; x+=1)
        dispmem[x+18] = 0x220;
}

void dma_spi_init_lcd(void) {
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
    generic_lcd_startup();
    //DMA
    RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
    SPI2 -> CR2 =  SPI_CR2_TXDMAEN | SPI_CR2_SSOE| SPI_CR2_FRXTH | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    DMA1_Channel5 -> CPAR = (uint32_t)(&(SPI2 -> DR));
    DMA1_Channel5 -> CMAR = (uint32_t)(dispmem);
    DMA1_Channel5 -> CNDTR = 34;
    DMA1_Channel5 -> CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_DIR | DMA_CCR_CIRC | DMA_CCR_TEIE;
    DMA1_Channel5 -> CCR &= (~DMA_CCR_PL);
    DMA1_Channel5 -> CCR |= DMA_CCR_EN;
}

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void generic_lcd_startup(void) {
    nano_wait(100000000); // Give it 100ms to initialize
    cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    cmd(0x02);  // put the cursor in the home position
    cmd(0x06);  // 0000 01IS: set display to increment
}

void spi_cmd(char b) {
    while((SPI2 -> SR & SPI_SR_TXE) == 0);
    SPI2 -> DR = b;
}

void spi_data(char b) {
    while((SPI2 -> SR & SPI_SR_TXE) == 0);
    SPI2 -> DR = 0x200 + b;
}
