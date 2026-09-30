#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdbool.h>

// ---- Pin assignments (change here if you move a signal) ----

// PWM output to MCU_PI. PD5 is OC0B, fixed by Timer0.
#define PWM_DDR    DDRD
#define PWM_BIT    PD5

// Shutdown output to the OVP line (drives Q3, pulls COMP low). High = shut down.
#define OVP_DDR    DDRD
#define OVP_PORT   PORTD
#define OVP_BIT    PD4

// Vo flag from comparator U2A. High = overvoltage.
// Note: the comparator runs from 10V, so add a divider or clamp before this pin.
#define VO_DDR     DDRD
#define VO_PINREG  PIND
#define VO_BIT     PD2

void gpio_init(void);
void ovp_set(bool on);
bool vo_flag_active(void);

#endif
