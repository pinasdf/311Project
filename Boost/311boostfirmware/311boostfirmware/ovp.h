#ifndef OVP_H
#define OVP_H

#include <stdint.h>

/* Safety trip thresholds */
#define OVP_TRIGGER_MV  5500      /* 5.5 V trip threshold     */
#define OVP_RECOVER_MV  5300      /* 5.3 V recovery threshold */

/* Configure pin PD2 (Header P3 Pin 2) as High-Z input at boot */
void ovp_init(void);

/* Pass the latest Vout (in mV). 
 * Updates the hysteresis state and drives PD2.
 * Returns 1 if in OVP fault, 0 if normal. */
uint8_t ovp_update(uint16_t vout_mv);

/* Check if currently in OVP without updating */
uint8_t ovp_is_active(void);

#endif /* OVP_H */