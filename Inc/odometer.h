/*
 * odometer.h
 *
 *  Created on: Jan 29, 2023
 *      Author: Denis
 */

#ifndef ODOMETER_H_
#define ODOMETER_H_
#include "stdint.h"

typedef void (* observer_fn)();

typedef struct{
	uint32_t val;
//	observer_fn observers[4];
//	uint8_t observer_count;
	uint8_t needpoll;
	void (*on_change)();
}xOdometer_tt;


void odometer_poll(xOdometer_tt * o);

uint8_t odometer_subscribe(xOdometer_tt * o,  observer_fn * obs);
void odometer_set(xOdometer_tt * o, uint32_t val);
void odometer_add(xOdometer_tt * o, uint32_t val);
uint32_t odometer_value(xOdometer_tt * o);




#endif /* ODOMETER_H_ */
