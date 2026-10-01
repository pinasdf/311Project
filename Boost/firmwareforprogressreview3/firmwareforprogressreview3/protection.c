#include <stdint.h>
#include "protection.h"
#include "gpio.h"

static bool    fault = false;
static uint8_t over_v_count = 0;
static uint8_t over_i_count = 0;

void protection_init(void)
{
	fault = false;
	over_v_count = 0;
	over_i_count = 0;
	ovp_set(false);
}

void protection_check(float vout, float current)
{
	if (fault) return;                         // stay tripped until protection_reset()

	if (vout > PROT_VOUT_MAX_V) {
		if (over_v_count < 255) over_v_count++;
		} else {
		over_v_count = 0;
	}

	if (current > PROT_CURRENT_MAX_A) {
		if (over_i_count < 255) over_i_count++;
		} else {
		over_i_count = 0;
	}

	bool trip = (over_v_count >= PROT_TRIP_COUNT) || (over_i_count >= PROT_TRIP_COUNT);

	#if PROT_USE_VO_FLAG
	if (vo_flag_active()) trip = true;         // hardware overvoltage flag, trip straight away
	#endif

	if (trip) {
		fault = true;
		ovp_set(true);                         // pulls COMP low, UC3843 stops switching
	}
}

bool protection_fault(void)
{
	return fault;
}

void protection_reset(void)
{
	fault = false;
	over_v_count = 0;
	over_i_count = 0;
	ovp_set(false);
}
