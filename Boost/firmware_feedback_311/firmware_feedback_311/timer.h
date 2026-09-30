#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "config.h"

// ---- Settings you can change ----
#define LOOP_RATE_HZ      1000UL   // how often the control loop runs
#define TIMER1_PRESCALER  64       // 1, 8, 64, 256 or 1024

// ---- Calculated (do not edit) ----
// CTC mode: f = F_CPU / (N * (TOP + 1))
#define TIMER1_TOP        ((F_CPU / (TIMER1_PRESCALER * LOOP_RATE_HZ)) - 1UL)

#if TIMER1_TOP > 65535
#error "TIMER1_TOP does not fit in 16 bits. Use a bigger TIMER1_PRESCALER or a higher LOOP_RATE_HZ."
#endif

void    timer_init(void);
uint8_t timer_tick_pending(void);   // 1 if a new loop tick is waiting
void    timer_tick_clear(void);     // call once the tick has been handled
uint8_t timer_overruns(void);       // ticks that arrived before the last one was handled

#endif
