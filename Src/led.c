/*
 * led.c
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#include "led.h"

void led_on(xLed_tt * led){
	led->mode = LED_BLINK_OFF;
	timer_stop(&led->timer);
	led->pwm_value = LED_MIN;
	*((uint32_t *)led->ccr) = led->pwm_value;
}
void led_off(xLed_tt * led){
	led->mode = LED_BLINK_OFF;
	timer_stop(&led->timer);
	led->pwm_value = LED_MAX;
	*((uint32_t *)led->ccr) = led->pwm_value;
}
void led_blink(xLed_tt * led , uint32_t frq){
	if(frq == 0){
		frq = led->frq == 0 ? 1: led->frq;
	}
	if(frq > 1000){
		led_on(led);
		return;
	}
	led->mode = LED_BLINK_ON;
	led->frq = frq;
	led->pwm_value = LED_MAX;
	timer_set(&led->timer, frq, led_on_timeout, led);
}

uint8_t led_state(xLed_tt * led){
	if(led->mode == LED_BLINK_OFF){
		if(led->pwm_value == LED_MIN){
			return 1;
		}
	}
	return 0;
}
uint8_t led_mode(xLed_tt * led){
	return led->mode;
}
uint16_t led_frq(xLed_tt * led){
	return led->frq;
}

void led_on_timeout(xTimer_tt * t,  void * thisArg){
	xLed_tt * led = (xLed_tt *) thisArg;

	timer_reset(t);
	if(led->pwm_value < (LED_MIN + 1)) led->step = 1;
	if(led->pwm_value > (LED_MAX - 1)) led->step = -1;
	led->pwm_value += led->step;
	*((uint32_t *)led->ccr) = led->pwm_value;
}


