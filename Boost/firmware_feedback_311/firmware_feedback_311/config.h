#ifndef CONFIG_H
#define CONFIG_H

// ---- Project wide settings ----

#ifndef F_CPU
#define F_CPU 16000000UL          // CPU clock in Hz
#endif

#define VOUT_TARGET_V   5.0f      // output voltage the PI controller aims for

// Test mode for the Proteus template (PWM into RC filter into ADC pin)
//   1 = hold the PWM at TEST_DUTY, no PI, no protection
//   0 = normal operation (PI controller and protection)
#define TEST_LOOPBACK   1
#define TEST_DUTY       0.50f

// Debug printing over the UART (Proteus virtual terminal)
#define DEBUG_UART_ENABLE          1
#define DEBUG_PRINT_PERIOD_TICKS   250    // print once every N loop ticks

#endif
