/*
 * led.c
 *
 *  Created on: 30 янв. 2023 г.
 *      Author: Denis
 */


#include "led.h"

void led_on(xLed_tt * led){
	led->state = GPIO_PIN_RESET;
	if(!HAL_GPIO_ReadPin(SW_LED_OFF_GPIO_Port, SW_LED_OFF_Pin))
		led_update(led);
}

void led_off(xLed_tt * led){
	led->state = GPIO_PIN_SET;
	if(!HAL_GPIO_ReadPin(SW_LED_OFF_GPIO_Port, SW_LED_OFF_Pin))
		led_update(led);
}
void led_hard_off(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_SET);
}
void led_update(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, led->state);
}
void led_toggle(xLed_tt * led){
	if(led->state == GPIO_PIN_SET){
		led_on(led);
	}
	else{
		led_off(led);
	}
}
