/*
 * led.h
 *
 *  Created on: 30 янв. 2023 г.
 *      Author: Denis
 */

#ifndef LED_H_
#define LED_H_

#include "main.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	uint8_t state;
}xLed_tt;

void led_on(xLed_tt * led);
void led_off(xLed_tt * led);
void led_hard_off(xLed_tt * led);
void led_update(xLed_tt * led);
void led_toggle(xLed_tt * led);


#endif /* LED_H_ */
