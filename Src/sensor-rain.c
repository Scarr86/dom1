/*
 * sensor-rain.c
 *
 *  Created on: Jan 30, 2023
 *      Author: Denis
 */

#include "sensor-rain.h"

void sensor_rain_init(xSensor_rain_tt * s){
	s->state = HAL_GPIO_ReadPin(s->GPIOx, s->pin);
}
void sensor_rain_poll(xSensor_rain_tt * s){
	if(s->state != HAL_GPIO_ReadPin(s->GPIOx, s->pin)){
		s->state = !s->state;
		s->on_change();
	}
}
uint8_t sensor_rain_state(xSensor_rain_tt * s){
	return s->state;
}
uint8_t sensor_rain_is_detected(xSensor_rain_tt * s){
	return s->state == s->cmp_val;
}
