#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

volatile uint8_t timer_tick = 0;

/* ---------------- Timer0: PWM DAC (slide setup) ---------------- */
void timer0_pwm_init(void)
{
    DDRD |= (1 << DDD5);                 /* PD5 (OC0B) as output    */

    OCR0A = PWM_TOP;                     /* TOP: 100 kHz            */
    OCR0B = PWM_TOP / 2;                 /* start at 50% duty       */

    TCCR0A = (1 << COM0B1) |             /* non-inverting OC0B      */
             (1 << WGM01)  |
             (1 << WGM00);               /* Mode 7: Fast PWM, TOP=OCR0A */

    TCCR0B = (1 << WGM02) |
             (1 << CS00);                /* start timer, N = 1      */
}

void timer0_set_compare(uint8_t cmp)
{
    OCR0B = cmp;
}

/* ---------------- Timer1: control-loop tick ---------------- */
void timer1_init(void)
{
    TCCR1A = 0;                          /* CTC mode (WGM12 below)  */

    /* Prescaler 8 -> 2 MHz timer clock.
     * OCR1A = F_CPU / (8 * TICK_HZ) - 1  (999 for 2 kHz = 500 us)  */
    OCR1A = (uint16_t)(F_CPU / (8UL * TICK_HZ) - 1UL);

    TIMSK1 = (1 << OCIE1A);              /* compare match A interrupt */

    TCCR1B = (1 << WGM12) |              /* CTC, TOP = OCR1A        */
             (1 << CS11);                /* start timer, N = 8      */
}

ISR(TIMER1_COMPA_vect)
{
    timer_tick = 1;
}