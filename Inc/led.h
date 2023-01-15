/*
 * led.h
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#ifndef LED_H_
#define LED_H_
#include "timer.h"
#include "main.h"

typedef enum{
	LED_BLINK_OFF,
	LED_BLINK_ON
}LED_MODE;

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	xTimer_tt timer;
	LED_MODE mode;
}xLed_tt;

void led_on(xLed_tt * led);
void led_off(xLed_tt * led);
void led_blink(xLed_tt * led , uint32_t frq);
void led_update(xLed_tt * led);
uint8_t led_state(xLed_tt * led);
uint8_t led_mode(xLed_tt * led);




#endif /* LED_H_ */
