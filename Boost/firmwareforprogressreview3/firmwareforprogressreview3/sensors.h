#ifndef SENSORS_H
#define SENSORS_H

#include "adc.h"

// ---- Board values (replace the placeholders with your real parts) ----

// Output voltage divider (R22 on top, R24 on the bottom, into MCU_VFB)
// For the Proteus template the ADC pin reads the RC filter directly, so the ratio is 1.
#define SENSE_V_RTOP_OHM     10000f       // R22. Keep 0 for the Proteus template
#define SENSE_V_RBOT_OHM     10000f    // R24

// Current sensing chain
#define SENSE_I_SHUNT_OHM    0.047f       // R5 (placeholder)
#define SENSE_I_GAIN         10.0f      // U2B gain, 1 + R6/R8 (placeholder)
#define SENSE_I_OFFSET_V     2.14f       // volts at the pin with zero current (ramp and offset)

float sensors_vout(void);       // output voltage in volts
float sensors_current(void);    // input current in amps

#endif
