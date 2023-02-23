/*
 * led.h
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#ifndef LED_PWM_H_
#define LED_PWM_H_
#include "timer.h"
#include "main.h"

#define LED_MIN (400)
#define LED_MAX (500)

typedef enum{
	LED_BLINK_OFF,
	LED_BLINK_ON
}LED_MODE;

typedef enum{
	LED_PWM_ON,
	LED_PWM_OFF,
	LED_PWM_BLINK
}LED_PWM_STATE;



typedef struct{
	uint32_t ccr;
	int16_t step;
	uint16_t pwm_value;
	xTimer_tt timer;
	LED_MODE mode;
	uint16_t frq;
	uint8_t state;
}xLed_pwm_tt;

void led_pwm_update(xLed_pwm_tt * led);
void led_pwm_on(xLed_pwm_tt * led);
void led_pwm_off(xLed_pwm_tt * led);
void led_pwm_blink(xLed_pwm_tt * led , uint32_t frq);

void led_pwm_on_timeout(xTimer_tt * t, void * thisArg);
uint8_t led_pwm_state(xLed_pwm_tt * led);
uint8_t led_pwm_mode(xLed_pwm_tt * led);
uint16_t led_pwm_frq(xLed_pwm_tt * led);





#endif /* LED_H_ */
