#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "config.h"

// ---- Settings you can change ----
#define PWM_FREQ_HZ      100000UL   // PWM frequency
#define PWM_PRESCALER    1          // Timer0 prescaler: 1, 8, 64, 256 or 1024

// Duty limits sent to the UC3843 through the RC filter.
// Higher duty = higher Vpi_in = LOWER peak current (the op amp inverts).
// 0.32 to 0.61 is about 1.6V to 3.05V, assuming Rop1 4.7k and Rop2 10k like the slides.
// Confirm R20 and R17 on your board. Widen these for bench tests if needed.
#define PWM_DUTY_MIN     0.32f
#define PWM_DUTY_MAX     0.61f
#define PWM_DUTY_START   PWM_DUTY_MAX   // start at the lowest current limit

// ---- Calculated from the settings above (do not edit) ----
// Fast PWM mode 7: f = F_CPU / (N * (TOP + 1))
#define PWM_TOP          ((F_CPU / (PWM_PRESCALER * PWM_FREQ_HZ)) - 1UL)

#if PWM_TOP > 255
#error "PWM_TOP does not fit in 8 bits. Use a bigger PWM_PRESCALER or a higher PWM_FREQ_HZ."
#endif

void pwm_init(void);
void pwm_set_duty(float duty);    // duty from 0.0 to 1.0, clamped to the limits above

#endif
