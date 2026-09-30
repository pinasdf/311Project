#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_CH_CURRENT 0   // IFB_F on PC0
#define ADC_CH_VOLTAGE 1   // MCU_VFB on PC1

void     adc_init(void);
uint16_t adc_read(uint8_t channel);
uint16_t adc_read_avg(uint8_t channel, uint8_t n);

#endif