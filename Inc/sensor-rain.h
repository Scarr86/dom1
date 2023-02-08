/*
 * sensor-rain.h
 *
 *  Created on: Jan 30, 2023
 *      Author: Denis
 */

#ifndef SENSOR_RAIN_H_
#define SENSOR_RAIN_H_
#include "main.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	uint8_t state;
	uint8_t cmp_val;
	uint8_t enable;
	void (*on_change)();
}xSensor_rain_tt;


void sensor_rain_init(xSensor_rain_tt * s);
void sensor_rain_poll(xSensor_rain_tt * s);
void sensor_rain_set(xSensor_rain_tt * s, int8_t enable, int8_t cmpVal);
uint8_t sensor_rain_is_enable(xSensor_rain_tt * s);
uint8_t sensor_rain_state(xSensor_rain_tt * s);
uint8_t sensor_rain_cmpval(xSensor_rain_tt * s);
uint8_t sensor_rain_is_detected(xSensor_rain_tt * s);


#endif /* SENSOR_RAIN_H_ */
