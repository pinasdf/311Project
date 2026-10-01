// 

#include <avr/io.h>
#include "adc.h"

void adc_init(void)
{
	ADMUX  = (1 << REFS0);                                   // AVCC reference
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // on, /128
	DIDR0  = (1 << ADC0D) | (1 << ADC1D);                    // digital buffers off
}

uint16_t adc_read(uint8_t channel)
{
	ADMUX = (1 << REFS0) | (channel & 0x0F);   // pick channel
	ADCSRA |= (1 << ADSC);                     // start conversion
	while (ADCSRA & (1 << ADSC));              // wait until done
	return ADC;
}

uint16_t adc_read_avg(uint8_t channel, uint8_t n)
{
	uint32_t sum = 0;
	adc_read(channel);                         // throwaway read after switching
	for (uint8_t i = 0; i < n; i++)
	sum += adc_read(channel);
	return (uint16_t)(sum / n);
}