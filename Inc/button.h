/*
 * button.h
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: Denis
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "main.h"
#include "timer.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	xTimer_tt timer;
	uint16_t debounceTime;
	uint8_t state;
	void (*on_click)();
}xButton_tt;

uint8_t btn_init(xButton_tt * btn);
uint8_t btn_set(xButton_tt * btn, uint16_t debounceTime);
uint8_t btn_poll(xButton_tt * btn);
uint8_t btn_state(xButton_tt * btn);
uint16_t btn_debounceTime(xButton_tt * btn);







#endif /* BUTTON_H_ */
