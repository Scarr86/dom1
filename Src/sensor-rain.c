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
		if(sensor_rain_is_enable(s))
			s->on_change();
	}
}
void sensor_rain_set(xSensor_rain_tt * s, int8_t enable, int8_t cmpVal){
	if(enable >= 0)
		s->enable = enable;
	if(cmpVal >= 0)
		s->cmp_val = cmpVal;
}
uint8_t sensor_rain_is_enable(xSensor_rain_tt * s){
	return s->enable;
}
uint8_t sensor_rain_state(xSensor_rain_tt * s){
	return s->state;
}
uint8_t sensor_rain_is_detected(xSensor_rain_tt * s){
	return s->state == s->cmp_val;
}
