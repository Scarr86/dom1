/*
 * sensor.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Denis
 */

#ifndef SENSOR_H_
#define SENSOR_H_
#include "main.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	uint8_t state;
	uint8_t cmpVal;
	void (*on_detected)();
}xSensor_tt;


void sensor_init(xSensor_tt * s);
void sensor_poll(xSensor_tt * s);
void sensor_set(xSensor_tt * s, uint8_t cmpVal);
uint8_t sensor_state(xSensor_tt * s);
uint8_t sensor_cmp_val(xSensor_tt * s);
uint8_t sensor_is_detected(xSensor_tt * s);

#endif /* SENSOR_H_ */
