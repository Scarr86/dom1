/*
 * dom.h
 *
 *  Created on: Jan 13, 2023
 *      Author: Denis
 */

#ifndef DOM_H_
#define DOM_H_
#include "main.h"
#include "button.h"
#include "led.h"
typedef enum{
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_COUNT
}BUTTON_ENUM;



void dom_init();



int8_t btn_state_by_id(uint8_t id);
uint16_t btn_debounce_time_by_id(uint8_t id);


uint8_t dom_led_mode();
uint8_t dom_led_state();
uint16_t dom_led_frq();
uint8_t dom_led_set(uint8_t on, uint8_t mode, uint8_t frq);


void dom_poll();



#endif /* DOM_H_ */
