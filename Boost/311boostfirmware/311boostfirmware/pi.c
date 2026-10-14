#include "pi.h"

static float Int_out = 0.0f;
static float PI_out  = 0.0f;

float sat_limit_pi(float result)
{
	if (result > PI_LIMIT) {
		return PI_LIMIT;
		} else if (result < -PI_LIMIT) {
		return -PI_LIMIT;
		} else {
		return result;
	}
}

void pi_controller(uint16_t V_out, uint16_t V_ref)
{
	int32_t V_err = (int32_t)V_out - (int32_t)V_ref;
	float Prop_out = (float)V_err * VOLTAGE_KP;

	/* 1. Calculate tentative integral step */
	float Int_step = (float)V_err * VOLTAGE_KI * T_SAMPLE;
	float PI_tentative = Prop_out + (Int_out + Int_step);

	/* 2. Conditional Integration (Anti-Windup Clamping) */
	if (PI_tentative > PI_LIMIT) {
		/* Pushed against upper ceiling: only integrate if error is pulling us down */
		if (V_err < 0) {
			Int_out += Int_step;
		}
	}
	else if (PI_tentative < -PI_LIMIT) {
		/* Pushed against lower floor: only integrate if error is pulling us up */
		if (V_err > 0) {
			Int_out += Int_step;
		}
	}
	else {
		/* Inside linear range: integrate normally */
		Int_out += Int_step;
	}

	/* 3. Clamp final values */
	Int_out = sat_limit_pi(Int_out);
	PI_out  = sat_limit_pi(Prop_out + Int_out);
}

uint8_t pi_get_compare(void)
{
	float duty = PI_out * PWM_TOP / (2.0f * PI_LIMIT) + PWM_TOP / 2.0f;

	if (duty > (float)PWM_TOP) {
		duty = (float)PWM_TOP;
		} else if (duty < 0.0f) {
		duty = 0.0f;
	}

	return (uint8_t)duty;
}

void pi_reset_integrator(void)
{
	Int_out = 0.0f;
	PI_out  = 0.0f;
}