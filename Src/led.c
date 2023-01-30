/*
 * led.c
 *
 *  Created on: 30 янв. 2023 г.
 *      Author: Denis
 */


#include "led.h"

void led_on(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_RESET);
}

void led_off(xLed_tt * led){
	HAL_GPIO_WritePin(led->GPIOx, led->pin, GPIO_PIN_SET);
}
