#include <avr/io.h>
#include "adc.h"

// ---- Compile time checks and register bits ----
#if (ADC_BUF_CURRENT < 1) || (ADC_BUF_CURRENT > 255) || (ADC_BUF_VOLTAGE < 1) || (ADC_BUF_VOLTAGE > 255)
#error "ADC_BUF_CURRENT and ADC_BUF_VOLTAGE must be between 1 and 255"
#endif

#if   ADC_PRESCALER == 2
#define ADC_PS_BITS  (1 << ADPS0)
#elif ADC_PRESCALER == 4
#define ADC_PS_BITS  (1 << ADPS1)
#elif ADC_PRESCALER == 8
#define ADC_PS_BITS  ((1 << ADPS1) | (1 << ADPS0))
#elif ADC_PRESCALER == 16
#define ADC_PS_BITS  (1 << ADPS2)
#elif ADC_PRESCALER == 32
#define ADC_PS_BITS  ((1 << ADPS2) | (1 << ADPS0))
#elif ADC_PRESCALER == 64
#define ADC_PS_BITS  ((1 << ADPS2) | (1 << ADPS1))
#elif ADC_PRESCALER == 128
#define ADC_PS_BITS  ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0))
#else
#error "ADC_PRESCALER must be 2, 4, 8, 16, 32, 64 or 128"
#endif

#define ADC_REF_BITS   (1 << REFS0)     // AVCC reference, right adjusted result

#define ADC_FILL_COUNT  ((ADC_BUF_CURRENT > ADC_BUF_VOLTAGE) ? ADC_BUF_CURRENT : ADC_BUF_VOLTAGE)

// ---- Moving average storage, one per channel ----
typedef struct {
	uint16_t *buf;     // circular buffer of raw readings
	uint8_t   size;    // number of readings kept
	uint8_t   idx;     // next slot to overwrite
	uint32_t  sum;     // running sum of everything in the buffer
} adc_avg_t;

static uint16_t cur_buf[ADC_BUF_CURRENT];
static uint16_t vol_buf[ADC_BUF_VOLTAGE];

static adc_avg_t cur_avg = { cur_buf, ADC_BUF_CURRENT, 0, 0 };
static adc_avg_t vol_avg = { vol_buf, ADC_BUF_VOLTAGE, 0, 0 };

static void avg_push(adc_avg_t *a, uint16_t value)
{
	a->sum -= a->buf[a->idx];          // remove the oldest reading
	a->buf[a->idx] = value;            // store the new one
	a->sum += value;
	a->idx++;
	if (a->idx >= a->size) a->idx = 0;
}

static uint16_t avg_get(const adc_avg_t *a)
{
	return (uint16_t)((a->sum + (a->size / 2)) / a->size);   // rounded average
}

// ---- Low level conversion ----
static uint16_t adc_convert(void)
{
	ADCSRA |= (1 << ADSC);                    // start conversion
	while (ADCSRA & (1 << ADSC)) { }          // wait until it finishes
	return ADC;
}

#if ADC_FAKE_INPUT
static uint16_t fake_raw(float volts)
{
	float r = volts * ADC_COUNTS / ADC_VREF_V;   // volts to ADC code
	if (r < 0.0f)    r = 0.0f;
	if (r > 1023.0f) r = 1023.0f;
	return (uint16_t)r;
}
#endif

uint16_t adc_read_raw(uint8_t channel)
{
	#if ADC_FAKE_INPUT
	return fake_raw(channel == ADC_CH_VOLTAGE ? ADC_FAKE_VOLTAGE_V : ADC_FAKE_CURRENT_V);
	#else
	ADMUX = ADC_REF_BITS | (channel & 0x0F);

	for (uint8_t i = 0; i < ADC_DUMMY_READS; i++) {
		adc_convert();
	}
	return adc_convert();
	#endif
}

// ---- Public functions ----
void adc_init(void)
{
	ADMUX  = ADC_REF_BITS;
	ADCSRA = (1 << ADEN) | ADC_PS_BITS;                       // ADC on, prescaler set
	DIDR0  = (1 << ADC_CH_CURRENT) | (1 << ADC_CH_VOLTAGE);   // digital buffers off on used pins

	// Fill both buffers with real readings so the averages are valid from the start
	for (uint8_t i = 0; i < ADC_FILL_COUNT; i++) {
		adc_update();
	}
}

void adc_update(void)
{
	avg_push(&cur_avg, adc_read_raw(ADC_CH_CURRENT));
	avg_push(&vol_avg, adc_read_raw(ADC_CH_VOLTAGE));
}

uint16_t adc_voltage_raw(void) { return avg_get(&vol_avg); }
uint16_t adc_current_raw(void) { return avg_get(&cur_avg); }