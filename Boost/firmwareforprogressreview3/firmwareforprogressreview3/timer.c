#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#if   TIMER1_PRESCALER == 1
#define T1_CS  (1 << CS10)
#elif TIMER1_PRESCALER == 8
#define T1_CS  (1 << CS11)
#elif TIMER1_PRESCALER == 64
#define T1_CS  ((1 << CS11) | (1 << CS10))
#elif TIMER1_PRESCALER == 256
#define T1_CS  (1 << CS12)
#elif TIMER1_PRESCALER == 1024
#define T1_CS  ((1 << CS12) | (1 << CS10))
#else
#error "TIMER1_PRESCALER must be 1, 8, 64, 256 or 1024"
#endif

static volatile uint8_t tick = 0;
static volatile uint8_t overruns = 0;

void timer_init(void)
{
	TCCR1A = 0;
	TCNT1  = 0;
	OCR1A  = (uint16_t)TIMER1_TOP;
	TIMSK1 = (1 << OCIE1A);                  // compare match A interrupt
	TCCR1B = (1 << WGM12) | T1_CS;           // CTC mode, start the timer last
}

ISR(TIMER1_COMPA_vect)
{
	if (tick && overruns < 255) overruns++;  // previous tick was not handled in time
	tick = 1;
}

uint8_t timer_tick_pending(void) { return tick; }
void    timer_tick_clear(void)   { tick = 0; }
uint8_t timer_overruns(void)     { return overruns; }
