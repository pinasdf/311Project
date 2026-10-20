#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "pi.h"
#include "timer.h"
#include "ovp.h"

#define VTARGET_MV  5000      /* 5.0 V regulated target */
#define DEBUG_PIN   PB0           /* Timing probe on PB0    */

int main(void)
{
	uint16_t V_out;
	uint8_t  cmp;

	/* ---- Initialisation ---- */
	DDRB |= (1 << DEBUG_PIN);     /* Timing probe */

	ovp_init();                   /* OVP pin PD2 init      */
	adc_init();                   /* ADC on PC0            */
	timer0_pwm_init();            /* PWM DAC on PD5 (OC0B) */
	timer1_init();                /* 2 kHz control tick    */

	sei();                        /* Enable interrupts     */

	/* ---- 2 kHz Control Loop ---- */
	while (1) {
		if (timer_tick) {
			timer_tick = 0;

			PORTB |= (1 << DEBUG_PIN);

			V_out = adc_read_vout_mV();

			/* Check and update OVP */
			if (ovp_update(V_out)) {
				/* In OVP: command zero current and clear integrator */
				timer0_set_compare(96);
				pi_reset_integrator();
				} else {
				/* Normal operation: closed-loop PI regulation */
				pi_controller(V_out, VTARGET_MV);
				cmp = pi_get_compare();
				timer0_set_compare(cmp);
			}

			PORTB &= ~(1 << DEBUG_PIN);
		}
	}
}