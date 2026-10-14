#include <avr/io.h>
#include "adc.h"

void adc_init(void)
{
    /* REFS1:0 = 01 -> AVCC reference
     * ADLAR   = 0  -> right adjusted (use all 10 bits)
     * MUX3:0  = 0000 -> ADC0 (PC0)                                  */
    ADMUX = (1 << REFS0);

    /* Disable the digital input buffer on PC0 (it is an analog input) */
    DIDR0 |= (1 << ADC0D);

    /* ADEN = 1 -> enable ADC
     * ADPS2:0 = 111 -> prescaler 128 -> 16 MHz / 128 = 125 kHz      */
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read_raw(void)
{
    ADCSRA |= (1 << ADSC);              /* start conversion            */
    while (ADCSRA & (1 << ADSC)) { }    /* wait until ADSC clears      */
    return ADC;                         /* ADCL then ADCH (10-bit)     */
}

uint16_t adc_read_vout_mV(void)
{
    uint32_t raw = adc_read_raw();

    /* Vout_mV = raw * (Vref / Gvs) / 1024 = raw * ADC_FULLSCALE_MV / 1024              */
    return (uint16_t)((raw * ADC_FULLSCALE_MV) / 1024UL);
}