#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* AVCC reference. For best accuracy set this to the measured AVCC
 * voltage in mV (for example 4980) instead of the nominal 5000.      */
#define ADC_VREF_MV      4995UL

/* Output-voltage divider (schematic: R22 = top, from 5V; R24 = bottom,
 * to GND; C14 = 10 nF across R24 filters it). Gvs = Rb / (Ra + Rb).
 * Source impedance (Ra || Rb) should stay at or below about 10 kohm. */
#define ADC_RA_OHM       12000UL   /* R22 */
#define ADC_RB_OHM       33000UL   /* R24 */

/* Vout when the ADC is at full scale = Vref / Gvs  (= 7000 mV here)  */
#define ADC_FULLSCALE_MV ((ADC_VREF_MV * (ADC_RA_OHM + ADC_RB_OHM)) / ADC_RB_OHM)

/* Configure ADC0 (PC0): AVCC reference, right adjusted, prescaler 128 */
void adc_init(void);

/* Blocking read of ADC0. Returns raw 10-bit code (0 to 1023). */
uint16_t adc_read_raw(void);

/* Blocking read of ADC0 converted to output voltage in millivolts. */
uint16_t adc_read_vout_mV(void);

#endif /* ADC_H */