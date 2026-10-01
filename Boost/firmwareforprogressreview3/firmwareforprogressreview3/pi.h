#ifndef PI_H
#define PI_H

#include "config.h"
#include "timer.h"
#include "pwm.h"

// ---- Settings you can change (starting guesses, tune in Proteus) ----
#define PI_KP        0.5f    // duty change per volt of error
#define PI_KI        500.0f    // duty change per volt of error per second

// The UC3843 op amp inverts, so a HIGHER duty means a LOWER peak current.
// If Vout is too low we need more current, so the duty must go down.
// -1.0f = duty decreases when the error is positive (correct for this board)
#define PI_DIR       (-1.0f)

#define PI_OUT_MIN   PWM_DUTY_MIN
#define PI_OUT_MAX   PWM_DUTY_MAX
#define PI_BIAS      PWM_DUTY_MAX       // duty when error and integral are zero

#define PI_DT_S      (1.0f / (float)LOOP_RATE_HZ)   // time step, matches the loop rate

typedef struct {
	float integral;
} pi_t;

void  pi_init(pi_t *pi);
float pi_update(pi_t *pi, float target, float measured);   // returns the duty to apply

#endif
