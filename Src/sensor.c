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
		if(s->state == s->cmp_val){
			s->on_detected();
		}
	}
}
void sensor_set(xSensor_tt * s, uint8_t cmp_val){
	s->cmp_val = cmp_val;
}

uint8_t sensor_state(xSensor_tt * s){
	return s->state;
}

