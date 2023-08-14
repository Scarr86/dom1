/*
 * button.c
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: Denis
 */
#include "button.h"

uint8_t btn_init(xButton_tt * btn){
	btn->state = btn_state(btn);
	return 0;
}
uint8_t btn_set(xButton_tt * btn, uint16_t debounceTime){
	btn->debounceTime = debounceTime;
	return 0;
}
uint16_t btn_debounceTime(xButton_tt * btn){
	return btn->debounceTime;
}
uint8_t btn_poll(xButton_tt * btn){
	uint8_t st = btn_state(btn);
	if(st != btn->state){
		btn->state = st;
		timer_set(&btn->timer, btn->debounceTime, btn_on_tomeout, btn);
	}
	return 0;
}

void btn_on_tomeout(xTimer_tt * t, void * thisArg){
	xButton_tt * btn = (xButton_tt *) thisArg;
	if(btn_state(btn) == 0){
		btn->on_click();
	}
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

