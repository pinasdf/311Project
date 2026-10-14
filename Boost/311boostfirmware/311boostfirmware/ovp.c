#include <avr/io.h>
#include "ovp.h"

#define OVP_PIN   PD2             /* Pin 1 on DIP-28 */

static uint8_t ovp_state = 0;

static inline void ovp_pin_assert(void)
{
	PORTD |= (1 << OVP_PIN);      /* Drive HIGH (5 V) to turn on Q3 */
	DDRD  |= (1 << OVP_PIN);      /* Enable output driver           */
}

static inline void ovp_pin_release(void)
{
	DDRD  &= ~(1 << OVP_PIN);     /* Set to Input (High-Z)          */
	PORTD &= ~(1 << OVP_PIN);     /* Turn off internal pull-up      */
}

void ovp_init(void)
{
	ovp_state = 0;
	ovp_pin_release();            /* Disconnected at startup        */
}

uint8_t ovp_update(uint16_t vout_mv)
{
	/* If normal and voltage hits 5.5 V -> trip */
	if (!ovp_state && (vout_mv >= OVP_TRIGGER_MV)) {
		ovp_state = 1;
		ovp_pin_assert();
	}
	/* If tripped and voltage cools down to 5.3 V -> recover */
	else if (ovp_state && (vout_mv <= OVP_RECOVER_MV)) {
		ovp_state = 0;
		ovp_pin_release();
	}

	return ovp_state;
}

uint8_t ovp_is_active(void)
{
	return ovp_state;
}