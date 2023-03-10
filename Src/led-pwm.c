/*
 * led.c
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#include <led-pwm.h>


void led_pwm_update(xLed_pwm_tt * led){
	if(led->state == LED_PWM_ON)
		led_pwm_on(led);
	if(led->state == LED_PWM_OFF){
		led_pwm_off(led);
	}
	if(led->state == LED_PWM_BLINK){
		led_pwm_blink(led, led->frq);
	}
}
void led_pwm_on(xLed_pwm_tt * led){
	led->mode = LED_BLINK_OFF;
	led->state = LED_PWM_ON;
	timer_stop(&led->timer);
	led->pwm_value = LED_MIN;
	if(!HAL_GPIO_ReadPin(SW_LED_OFF_GPIO_Port, SW_LED_OFF_Pin))
		*((uint32_t *)led->ccr) = led->pwm_value;
}
void led_pwm_off(xLed_pwm_tt * led){
	led->mode = LED_BLINK_OFF;
	led->state = LED_PWM_OFF;
	timer_stop(&led->timer);
	led->pwm_value = LED_MAX;
	*((uint32_t *)led->ccr) = led->pwm_value;
}
void led_pwm_hard_off(xLed_pwm_tt * led){
	timer_stop(&led->timer);
	*((uint32_t *)led->ccr) = LED_MAX;
}
void led_pwm_blink(xLed_pwm_tt * led , uint32_t frq){
	if(frq == 0){
		frq = led->frq == 0 ? 1: led->frq;
	}
	if(frq > 1000){
		led_pwm_on(led);
		return;
	}
	led->mode = LED_BLINK_ON;
	led->state = LED_PWM_BLINK;
	led->frq = frq;
	led->pwm_value = LED_MAX;
	if(!HAL_GPIO_ReadPin(SW_LED_OFF_GPIO_Port, SW_LED_OFF_Pin))
		timer_set(&led->timer, frq, led_pwm_on_timeout, led);
}

uint8_t led_pwm_state(xLed_pwm_tt * led){
	if(led->mode == LED_BLINK_OFF){
		if(led->pwm_value == LED_MIN){
			return 1;
		}
	}
	return 0;
}
uint8_t led_pwm_mode(xLed_pwm_tt * led){
	return led->mode;
}
uint16_t led_pwm_frq(xLed_pwm_tt * led){
	return led->frq;
}

void led_pwm_on_timeout(xTimer_tt * t,  void * thisArg){
	xLed_pwm_tt * led = (xLed_pwm_tt *) thisArg;

	timer_reset(t);
	if(led->pwm_value < (LED_MIN + 1)) led->step = 1;
	if(led->pwm_value > (LED_MAX - 1)) led->step = -1;
	led->pwm_value += led->step;
	*((uint32_t *)led->ccr) = led->pwm_value;
}


