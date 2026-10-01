#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "config.h"

// ---- Settings you can change ----
#define ADC_CH_CURRENT    0       // IFB_F on PC0 (ADC0)
#define ADC_CH_VOLTAGE    1       // MCU_VFB on PC1 (ADC1). Wire the Proteus ADC monitoring point here.

#define ADC_PRESCALER     128     // 2, 4, 8, 16, 32, 64 or 128. 128 gives 125 kHz at 16 MHz
#define ADC_DUMMY_READS   1       // throwaway conversions after switching channel (0 to skip)

#define ADC_BUF_CURRENT   8       // moving average length for the current channel (1 to 255)
#define ADC_BUF_VOLTAGE   8       // moving average length for the voltage channel (1 to 255)

#define ADC_VREF_V        5.0f    // AVCC, used as the ADC reference
#define ADC_COUNTS        1024.0f // 10 bit ADC

// Timing note: one conversion takes about 104 us at 125 kHz.
// One adc_update() does 2 channels x (ADC_DUMMY_READS + 1) conversions,
// which is about 0.42 ms with the defaults. Keep the loop period longer than that.

void     adc_init(void);
uint16_t adc_read_raw(uint8_t channel);   // one blocking conversion, returns 0 to 1023
void     adc_update(void);                // read both channels once and store the results
uint16_t adc_voltage_raw(void);           // averaged code for the voltage channel
uint16_t adc_current_raw(void);           // averaged code for the current channel

// Fake input mode: skip the real ADC and use these voltages at the pin instead
#define ADC_FAKE_INPUT      1      // 1 = use the fake values below
#define ADC_FAKE_VOLTAGE_V  4.0f    // pretend voltage on the voltage channel pin
#define ADC_FAKE_CURRENT_V  0.5f    // pretend voltage on the current channel pin

#endif
