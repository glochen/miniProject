#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include "pegs.h"

void nano_wait(unsigned int n);

//LCD using SPI1
void spi1_cmd(char b);
void spi1_data(char b);
void lcd1_startup(void);

void init_lcd1(void);
void display1_line1(const char *);
void display1_line2(const char *);

//LCD using SPI2
void spi2_cmd(char b);
void spi2_data(char b);
void lcd2_startup(void);

void init_lcd2(void);
void display2_line1(const char *);
void display2_line2(const char *);

// ADC
void setup_adc(void);
int read_adc_channel(void);
void setup_timer15(void);
void endTimer15(void);

// clock
void setup_timer2(void);
void clockUp(void);
void clockDown(void);
void endTimer2(void);

// buttons
void setup_timer3(void);
void update_samples(int row);
void update_button_press(void);
int get_button_pressed(void);
void setup_button_matrix(void);

// lights
void setup_shift(void);
void setLights(Slot * lights);
