/*
 * dom.c
 *
 *  Created on: Jan 13, 2023
 *      Author: Denis
 */


#include "dom.h"
xLed_tt led = {
		.ccr = (uint32_t)&TIM1->CCR2,
};


xButton_tt btn[] = {
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_0,
			.debounceTime = 100,
			.on_click = dom_btn_1_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_1,
			.debounceTime = 100,
			.on_click = dom_btn_2_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_2,
			.debounceTime = 100,
			.on_click = dom_btn_3_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_3,
			.debounceTime = 100,
			.on_click = dom_btn_4_on_click,
		}
};

xSensor_tt sensor[] ={
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_10,
				.cmp_val = 1,
				.on_detected = dom_sensor_1_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_11,
				.cmp_val = 1,
				.on_detected = dom_sensor_2_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_12,
				.cmp_val = 1,
				.on_detected = dom_sensor_3_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_13,
				.cmp_val = 1,
				.on_detected = dom_sensor_4_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_14,
				.cmp_val = 1,
				.on_detected = dom_sensor_5_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_15,
				.cmp_val = 1,
				.on_detected = dom_sensor_6_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_2,
				.cmp_val = 1,
				.on_detected = dom_sensor_7_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_3,
				.cmp_val = 1,
				.on_detected = dom_sensor_8_on_detected
		},

};
//(uint32_t)&TIM1->CCR2,

xMotor_tt motor[]={
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_1,
				.ccr = (uint32_t)&TIM12->CCR2,
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_0,
				.ccr = (uint32_t)&TIM3->CCR2,
		},
		{
				.GPIOx = GPIOF,
				.pin = GPIO_PIN_15,
				.ccr = (uint32_t)&TIM3->CCR3,
		},
		{
				.GPIOx = GPIOF,
				.pin = GPIO_PIN_14,
				.ccr = (uint32_t)&TIM1->CCR1,
		},
		{
				.GPIOx = 0,
				.pin = 0,
				.ccr = (uint32_t)&TIM1->CCR2,
		},
};

uint8_t btn_observers_count;
btn_observers_fn btn_observers[4];

uint8_t sensor_observers_count;
sensor_observers_fn sensor_observers[4];




void dom_init(){

	for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
		btn_init(&btn[i]);
	}
	for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
		sensor_init(&sensor[i]);
	}
	for(uint16_t i = 0; i < MOTOR_COUNT; ++i){
		motor_init(&motor[i]);
	}
	motor_forward(&motor[MOTOR_5]);
	led_on(&led);
	//led_blink(&led, 4);
}
void dom_poll(){
	for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
		btn_poll(&btn[i]);
	}
	for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
		sensor_poll(&sensor[i]);
	}
}


// BUTTON FUNCTION START
void dom_btn_set(uint8_t id, int16_t debounceTime){
	if(id == 0 || id > BUTTON_COUNT || debounceTime == 0){
		return;
	}
	btn_set(&btn[id - 1], debounceTime);
}
int8_t dom_btn_state_by_id(uint8_t id){
	if(id == 0 || id > BUTTON_COUNT){
		return  -1;
	}
	return btn_state(&btn[id - 1]);
}
int16_t dom_btn_debounce_time_by_id(uint8_t id){
	if(id == 0 || id > BUTTON_COUNT){
		return -1;
	}
	return btn_debounceTime(&btn[id - 1]);
}
uint8_t dom_btn_subscribe(btn_observers_fn obs){
	if(btn_observers_count < 4){
		btn_observers[btn_observers_count] = obs;
		++btn_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_btn_notify(uint8_t indx){
	for(uint16_t i = 0; i < btn_observers_count; ++i){
		btn_observers[i](indx + 1);
	}
	return 0;
}
void dom_btn_1_on_click(){
	dom_btn_notify(BUTTON_1);
}
void dom_btn_2_on_click(){
	dom_btn_notify(BUTTON_2);
}
void dom_btn_3_on_click(){
	dom_btn_notify(BUTTON_3);
}
void dom_btn_4_on_click(){
	dom_btn_notify(BUTTON_4);
}
// BUTTON FUNCTION END



// LED FUNCTION START
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
// LED FUNCTION END

// SENSOR FUNCTION START
uint8_t dom_sensor_subscribe(sensor_observers_fn obs){
	if(sensor_observers_count < 4){
		sensor_observers[sensor_observers_count] = obs;
		++sensor_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_sensor_notify(uint8_t indx){
	for(uint16_t i = 0; i < sensor_observers_count; ++i){
		sensor_observers[i](indx + 1);
	}
	return 0;
}
int8_t dom_sensor_state_by_id(uint8_t id){
	if(id == 0 || id > SENSOR_COUNT)
		return  -1;
	return sensor_state(&sensor[id - 1]);
}
void dom_sensor_1_on_detected(){
	dom_sensor_notify(SENSOR_1);
}
void dom_sensor_2_on_detected(){
	dom_sensor_notify(SENSOR_2);
}
void dom_sensor_3_on_detected(){
	dom_sensor_notify(SENSOR_3);
}
void dom_sensor_4_on_detected(){
	dom_sensor_notify(SENSOR_4);
}
void dom_sensor_5_on_detected(){
	dom_sensor_notify(SENSOR_5);
}
void dom_sensor_6_on_detected(){
	dom_sensor_notify(SENSOR_6);
}
void dom_sensor_7_on_detected(){
	dom_sensor_notify(SENSOR_7);
}
void dom_sensor_8_on_detected(){
	dom_sensor_notify(SENSOR_8);
}
// SENSOR FUNCTION END


// MOTOR FUNCTION START
int8_t dom_motor_state(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return -1;
	return motor_state(&motor[id-1]);
}
int16_t dom_motor_speed(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return -1;
	return motor_speed(&motor[id-1]);
}
int8_t dom_motor_dir(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return -1;
	return motor_dir(&motor[id-1]);
}
void dom_motor_set(uint8_t id, uint16_t speed){
	if(id == 0 || id > MOTOR_COUNT || speed == 0 || speed > MOTOR_SPEED_MAX)
		return;
	motor_set(&motor[id-1], speed);
}
void dom_motor_forward(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return;
	motor_forward(&motor[id-1]);
}
void dom_motor_back(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return;
	motor_back(&motor[id-1]);
}
void dom_motor_stop(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return;
	motor_stop(&motor[id-1]);
}
// MOTOR FUNCTION END














