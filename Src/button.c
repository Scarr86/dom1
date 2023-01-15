/*
 * button.c
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: Denis
 */
#include "button.h"

uint8_t btn_state(xButton_tt * btn){
	GPIO_PinState ps = HAL_GPIO_ReadPin(btn->GPIOx, btn->pin);
	if(ps == GPIO_PIN_RESET){
		return 0;
	}
	if(ps == GPIO_PIN_SET){
		return 1;
	}
	return 0;
}

