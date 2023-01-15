/*
 * led.c
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#include "led.h"

void led_on(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_RESET );
	led->mode = LED_BLINK_OFF;
}
void led_off(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_SET );
	led->mode = LED_BLINK_OFF;
}
void led_blink(xLed_tt * led , uint32_t frq){
	if(frq == 0){
		led_off(led);
		return;
	}
	if(frq > 1000){
		led_on(led);
		return;
	}
	timer_set(&led->timer, 1000 / frq);
	led->mode = LED_BLINK_ON;
}

uint8_t led_state(xLed_tt * led){
	return HAL_GPIO_ReadPin(led->GPIOx, led->pin);
}
uint8_t led_mode(xLed_tt * led){
	return led->mode;
}

void led_update(xLed_tt * led){
	if(led->mode == LED_BLINK_ON){
		if(timer_expired(&led->timer)){
			timer_reset(&led->timer);
			HAL_GPIO_TogglePin(led->GPIOx, led->pin);

		}
	}
}
