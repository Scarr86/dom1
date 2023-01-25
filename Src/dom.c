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
			.on_click = dom_btn_1_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_1,
			.on_click = dom_btn_2_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_2,
			.on_click = dom_btn_3_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_3,
			.on_click = dom_btn_4_on_click,
		}
};

xSensor_tt sensor[] ={
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_10,
				.on_detected = dom_sensor_1_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_11,
				.on_detected = dom_sensor_2_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_12,
				.cmpVal = 1,
				.on_detected = dom_sensor_3_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_13,
				.on_detected = dom_sensor_4_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_14,
				.on_detected = dom_sensor_5_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_15,
				.on_detected = dom_sensor_6_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_2,
				.on_detected = dom_sensor_7_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_3,
				.on_detected = dom_sensor_8_on_detected
		},

};

xMotor_tt motor[]={
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_1,
				.ccr = (uint32_t)&TIM12->CCR2,
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_0,
				.speed = 250,
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

xDom_settings_tt dom_settings;


uint8_t btn_observers_count;
btn_observers_fn btn_observers[4];

uint8_t sensor_observers_count;
sensor_observers_fn sensor_observers[4];




void dom_init(){

	if(! settings_read(&dom_settings)){
		for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
			dom_settings.btn_settings[i].debounceTime = BUTTON_DEBOUNCE_TIME_DEF;
		}
		for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
			dom_settings.sensor_settings[i].cmpVal = SENSOR_CMP_VAL_DEF;
		}
		for(uint16_t i = 0; i < MOTOR_COUNT; ++i){
			dom_settings.motor_settings[i].speed = MOTOR_SPEED_DEF;
		}
	}
	for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
		btn_init(&btn[i]);
		btn_set(&btn[i], dom_settings.btn_settings[i].debounceTime);
	}
	for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
		sensor_init(&sensor[i]);
		sensor_set(&sensor[i], dom_settings.sensor_settings[i].cmpVal);
	}
	for(uint16_t i = 0; i < MOTOR_COUNT; ++i){
		motor_init(&motor[i]);
		motor_set(&motor[i], dom_settings.motor_settings[i].speed);
	}
	led_on(&led);
	motor_forward(&motor[MOTOR_5]);
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

uint16_t settings_write(xDom_settings_tt * ds){
	uint8_t data[sizeof(xDom_settings_tt) + 2];
	memcpy(data, ds, sizeof(xDom_settings_tt));
	uint16_t iks1 = sizeof data - 2;
	uint16_t iks2 = sizeof data - 1;
	ks16(ds, sizeof(xDom_settings_tt), &data[iks1], &data[iks2]);

	return flash_write((void *) data, sizeof data);
}
uint8_t settings_read(xDom_settings_tt * ds){
	uint8_t data[sizeof(xDom_settings_tt) + 2];

	flash_read((void *)data, sizeof data);
	if(ks16_check(data, sizeof data)){
		memcpy(ds, data, sizeof(xDom_settings_tt));
		return 1;
	}
	return 0;
}


xButton_tt * get_button(BUTTON_ENUM id){
	return &btn[id];
}
xSensor_tt * get_sensor(SENSOR_ENUM id){
	return &sensor[id];
}
xMotor_tt * get_motor(MOTOR_ENUM id){
	return &motor[id];
}

// BUTTON FUNCTION START
int8_t dom_btn_set(uint8_t id, int16_t debounceTime){
	if(id == 0 || id > BUTTON_COUNT){
		return 1;
	}
	if(debounceTime == 0){
		return 0;
	}

	dom_settings.btn_settings[id - 1].debounceTime = debounceTime;
	btn_set(&btn[id - 1], debounceTime);
	if(!settings_write(&dom_settings)){
		return 0;
	}
	return 1;
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
		btn_observers[i](indx);
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
uint8_t dom_sensor_set(uint8_t id, uint8_t cmpVal){
	if(id == 0 || id > SENSOR_COUNT){
		return 1;
	}
	dom_settings.sensor_settings[id - 1].cmpVal = !!cmpVal;
	sensor_set(&sensor[id - 1], !!cmpVal);
	if(!settings_write(&dom_settings)){
		return 0;
	}
	return 1;
}
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
		sensor_observers[i](indx);
	}
	return 0;
}
int8_t dom_sensor_state_by_id(uint8_t id){
	if(id == 0 || id > SENSOR_COUNT)
		return  -1;
	return sensor_state(&sensor[id - 1]);
}
int8_t dom_sensor_cmp_val_by_id(uint8_t id){
	if(id == 0 || id > SENSOR_COUNT){
		 return - 1;
	}
	return sensor_cmp_val(&sensor[id - 1]);
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
uint8_t dom_motor_set(uint8_t id, uint16_t speed){
	if(id == 0 || id > MOTOR_COUNT || speed > MOTOR_SPEED_MAX){
		return 1;
	}
	if(speed == 0){
		return 0;
	}
	dom_settings.motor_settings[id - 1].speed = speed;
	motor_set(&motor[id-1], speed);
	if(!settings_write(&dom_settings)){
		return 0;
	}
	return 1;
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














