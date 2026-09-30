#include "pi.h"

void pi_init(pi_t *pi)
{
    pi->integral = 0.0f;
}

float pi_update(pi_t *pi, float target, float measured)
{
    float error = target - measured;

    float p = PI_KP * error;
    float integral_next = pi->integral + PI_KI * error * PI_DT_S;

    float out = PI_BIAS + PI_DIR * (p + integral_next);

    // Anti windup: only keep the new integral value while the output is not clamped
    if (out > PI_OUT_MAX) {
        out = PI_OUT_MAX;
    } else if (out < PI_OUT_MIN) {
        out = PI_OUT_MIN;
    } else {
        pi->integral = integral_next;
    }

    return out;
}
