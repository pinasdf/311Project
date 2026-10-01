#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include "sensors.h"
#include "pi.h"
#include "protection.h"
#include "timer.h"
#include "uart.h"

static pi_t pi;

#if DEBUG_UART_ENABLE
static void debug_print(float vout)
{
	static uint16_t count = 0;
	if (++count < DEBUG_PRINT_PERIOD_TICKS) return;
	count = 0;

	uart_print("Vraw=");      uart_print_int((int16_t)adc_voltage_raw());
	uart_print(" Iraw=");     uart_print_int((int16_t)adc_current_raw());
	uart_print(" Vout_mV=");  uart_print_int((int16_t)(vout * 1000.0f));
	uart_print(" OCR0B=");    uart_print_int((int16_t)OCR0B);
	uart_print(" fault=");    uart_print_int((int16_t)protection_fault());
	uart_print("\r\n");
}
#endif

static void control_step(void)
{
	adc_update();
	float vout    = sensors_vout();
	float current = sensors_current();

	#if TEST_LOOPBACK
	(void)current;
	pwm_set_duty(TEST_DUTY);
	#else
	protection_check(vout, current);

	if (protection_fault()) {
		pwm_set_duty(PWM_DUTY_MAX);
		} else {
		pwm_set_duty(pi_update(&pi, VOUT_TARGET_V, vout));
	}
	#endif

	#if DEBUG_UART_ENABLE
	debug_print(vout);
	#endif
}

int main(void)
{
	gpio_init();
	uart_init();
	pwm_init();
	adc_init();
	protection_init();
	pi_init(&pi);
	timer_init();

	sei();

	while (1) {
		if (timer_tick_pending()) {
			timer_tick_clear();
			control_step();
		}
	}
}