#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* Timer0 TOP: OCR0A = 159 -> 16 MHz / 160 = 100 kHz PWM (slides).
 * Same value as in pi.h, so it is guarded against double definition. */
#ifndef PWM_TOP
#define PWM_TOP  159
#endif

/* Control-loop rate in Hz. Set T_SAMPLE in pi.h to 1.0f / TICK_HZ.
 * Keep it slow enough that ADC read (about 104 us) + PI maths fit
 * inside one period.                                                  */
#define TICK_HZ  2000UL

/* Set by the Timer1 ISR every 1/TICK_HZ seconds. main() clears it. */
extern volatile uint8_t timer_tick;

/* Timer0: Fast PWM (Mode 7), OC0B on PD5. This is the PWM "DAC". */
void timer0_pwm_init(void);

/* Load a new duty value (0 to PWM_TOP) into OCR0B. */
void timer0_set_compare(uint8_t cmp);

/* Timer1: CTC mode, interrupt every 1/TICK_HZ seconds.
 * main() must call sei() after the init functions.                    */
void timer1_init(void);

#endif /* TIMER_H */