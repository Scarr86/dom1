/*
 * dom.c
 *
 *  Created on: Jan 13, 2023
 *      Author: Denis
 */


#include "dom.h"
xLed_tt led = {
		.GPIOx = GPIOB,
		.pin = GPIO_PIN_14,
		.frq = 1,
};


xButton_tt btn[4] = {
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_0,
			.debounceTime = 100,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_1,
			.debounceTime = 100,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_2,
			.debounceTime = 100,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_3,
			.debounceTime = 100,
		}
};


void dom_init(){
	led_blink(&led, 1);
}

void dom_poll(){

}

int8_t btn_state_by_id(uint8_t id){
	if(id > BUTTON_COUNT){
		return  -1;
	}
	return btn_state(&btn[id - 1]);
}
int16_t btn_debounce_time_by_id(uint8_t id){
	if(id > BUTTON_COUNT){
		return -1;
	}
	return btn_debounceTime(&btn[id - 1]);
}

uint8_t dom_led_set(uint8_t on, uint8_t mode, uint8_t frq){
	if(mode == LED_BLINK_OFF){
		if(on)
			led_on(&led);
		else
			led_off(&led);
	}
	if(mode == LED_BLINK_ON){
		led_blink(&led, frq);
	}
	return 0;
}

uint8_t dom_led_mode(){
	return led_mode(&led);
}

uint8_t dom_led_state(){
	return led_state(&led);
}
uint16_t dom_led_frq(){
	return led_frq(&led);
}
