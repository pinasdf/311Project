#ifndef PI_H
#define PI_H

#include <stdint.h>
#include "timer.h"

/* Values from the lecture slides */
#define VOLTAGE_KP  1.8f //1.8
#define VOLTAGE_KI 10.0f //1000
#define T_SAMPLE    (1.0f / (float)TICK_HZ)
#define PI_LIMIT    2500.0f

/* Timer0 TOP (OCR0A = 159 -> 100 kHz PWM) */
#define PWM_TOP     159

/* Limit a value to +/- PI_LIMIT */
float sat_limit_pi(float result);

/* One PI update. Call at a fixed rate (T_SAMPLE). Voltages in mV. */
void pi_controller(uint16_t V_out, uint16_t V_ref);

/* Map the latest PI_out to the Timer0 compare value (0 to PWM_TOP).
 * Load the result into OCR0B. */
uint8_t pi_get_compare(void);

/* ADDED FOR OVP: Clears stored error so the controller doesn't spike on recovery */
void    pi_reset_integrator(void);

#endif /* PI_H */