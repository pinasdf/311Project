#include "gpio.h"

void gpio_init(void)
{
    PWM_DDR |= (1 << PWM_BIT);                      // PD5 output

    OVP_DDR  |= (1 << OVP_BIT);                     // PD4 output
    OVP_PORT &= ~(1 << OVP_BIT);                    // start low (no shutdown)

    VO_DDR &= ~(1 << VO_BIT);                       // PD2 input

    // PC0 and PC1 stay as inputs for the ADC (digital buffers are turned off in adc_init)
}

void ovp_set(bool on)
{
    if (on) OVP_PORT |=  (1 << OVP_BIT);
    else    OVP_PORT &= ~(1 << OVP_BIT);
}

bool vo_flag_active(void)
{
    return (VO_PINREG & (1 << VO_BIT)) != 0;
}
