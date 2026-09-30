#include "sensors.h"

static float counts_to_pin_volts(uint16_t counts)
{
    return (float)counts * ADC_VREF_V / ADC_COUNTS;
}

float sensors_vout(void)
{
    float vpin  = counts_to_pin_volts(adc_voltage_raw());
    float ratio = (SENSE_V_RTOP_OHM + SENSE_V_RBOT_OHM) / SENSE_V_RBOT_OHM;
    return vpin * ratio;
}

float sensors_current(void)
{
    float vpin = counts_to_pin_volts(adc_current_raw());
    return (vpin - SENSE_I_OFFSET_V) / (SENSE_I_GAIN * SENSE_I_SHUNT_OHM);
}
