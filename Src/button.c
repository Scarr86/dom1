/*
 * button.c
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: Denis
 */
#include "button.h"

uint8_t btn_init(xButton_tt * btn){
	btn->state = btn_state(btn);
}
uint8_t btn_set(xButton_tt * btn, uint16_t debounceTime){
	btn->debounceTime = debounceTime;
}
uint16_t btn_debounceTime(xButton_tt * btn){
	return btn->debounceTime;
}
uint8_t btn_update(xButton_tt * btn){

}

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
