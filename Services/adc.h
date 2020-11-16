#ifndef ADC_H
#define ADC_H

#include "stm32f103xb.h"


void GPIOC_init(void);
void init_adc(void);
int get_batterie(void);
int get_angle(void);
int warning_grand_angle(void);
int warning_low_batterie(void);
#endif
