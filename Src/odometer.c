/*
 * odometer.c
 *
 *  Created on: Jan 29, 2023
 *      Author: Denis
 */
#include "odometer.h"

static void odometer_notify(xOdometer_tt * o);

void odometer_poll(xOdometer_tt * o){
	if(o->needpoll){
		o->needpoll = 0;
		odometer_notify(o);
	}
}

static void odometer_notify(xOdometer_tt * o){
	for(uint8_t i = 0; i <  o->observer_count; ++i){
		o->observers[i]();
	}
}

uint8_t odometer_subscribe(xOdometer_tt * o,  observer_fn * obs){
	if(o->observer_count < 4){
		o->observers[o->observer_count] = obs;
		++o->observer_count;
		return o->observer_count;
	}
	return 0;
}

void odometer_set(xOdometer_tt * o, uint32_t val){
	o->val = val;
}
void odometer_add(xOdometer_tt * o, uint32_t val){
	o->val += val;
	o->needpoll = 1;
}

uint32_t odometer_value(xOdometer_tt * o){
	return o->val;
}
