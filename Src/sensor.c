/*
 * sensor.c
 *
 *  Created on: Jan 18, 2023
 *      Author: Denis
 */
#include "sensor.h"

void sensor_init(xSensor_tt * s){
	s->state = HAL_GPIO_ReadPin(s->GPIOx, s->pin);
}
void sensor_poll(xSensor_tt * s){
	if(s->state != HAL_GPIO_ReadPin(s->GPIOx, s->pin)){
		s->state = !s->state;
		if(s->state == s->cmpVal){
			s->on_detected();
		}
	}
}
void sensor_set(xSensor_tt * s, uint8_t cmpVal){
	s->cmpVal = cmpVal;
}

uint8_t sensor_state(xSensor_tt * s){
	return s->state;
}
uint8_t sensor_cmp_val(xSensor_tt * s){
	return s->cmpVal;
}

uint8_t sensor_is_detected(xSensor_tt * s){
	return s->state == s->cmpVal;
}

