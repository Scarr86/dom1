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
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_RESET );

}
void led_off(xLed_tt * led){
	led->mode = LED_BLINK_OFF;
	timer_stop(&led->timer);
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_SET );

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
	timer_set(&led->timer, 1000 / frq, led_on_timeout, led);
}

uint8_t led_state(xLed_tt * led){
	return !HAL_GPIO_ReadPin(led->GPIOx, led->pin);
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
	HAL_GPIO_TogglePin(led->GPIOx, led->pin);
}


