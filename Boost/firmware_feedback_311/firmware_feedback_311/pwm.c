#include <avr/io.h>
#include "pwm.h"

// Timer0 clock select bits for the chosen prescaler
#if   PWM_PRESCALER == 1
  #define PWM_CS  (1 << CS00)
#elif PWM_PRESCALER == 8
  #define PWM_CS  (1 << CS01)
#elif PWM_PRESCALER == 64
  #define PWM_CS  ((1 << CS01) | (1 << CS00))
#elif PWM_PRESCALER == 256
  #define PWM_CS  (1 << CS02)
#elif PWM_PRESCALER == 1024
  #define PWM_CS  ((1 << CS02) | (1 << CS00))
#else
  #error "PWM_PRESCALER must be 1, 8, 64, 256 or 1024"
#endif

void pwm_init(void)
{
    // PD5 must already be an output (gpio_init)
    OCR0A  = (uint8_t)PWM_TOP;                                   // TOP sets the frequency
    pwm_set_duty(PWM_DUTY_START);                                // OCR0B sets the duty
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);        // non inverting, mode 7
    TCCR0B = (1 << WGM02) | PWM_CS;                              // start the timer last
}

void pwm_set_duty(float duty)
{
    if (duty < PWM_DUTY_MIN) duty = PWM_DUTY_MIN;
    if (duty > PWM_DUTY_MAX) duty = PWM_DUTY_MAX;

    OCR0B = (uint8_t)(duty * (float)(PWM_TOP + 1UL) + 0.5f);
}
