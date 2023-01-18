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

#define LED_MIN (400)
#define LED_MAX (500)

typedef enum{
	LED_BLINK_OFF,
	LED_BLINK_ON
}LED_MODE;

typedef struct{
	uint32_t ccr;
	int16_t step;
	uint16_t pwm_value;
	xTimer_tt timer;
	LED_MODE mode;
	uint16_t frq;
}xLed_tt;

void led_on(xLed_tt * led);
void led_off(xLed_tt * led);
void led_blink(xLed_tt * led , uint32_t frq);

void led_on_timeout(xTimer_tt * t, void * thisArg);
uint8_t led_state(xLed_tt * led);
uint8_t led_mode(xLed_tt * led);
uint16_t led_frq(xLed_tt * led);




#endif /* LED_H_ */
