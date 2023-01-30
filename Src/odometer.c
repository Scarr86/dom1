/*
 * odometer.c
 *
 *  Created on: Jan 29, 2023
 *      Author: Denis
 */
#include "odometer.h"

//static void odometer_notify(xOdometer_tt * o);

void odometer_poll(xOdometer_tt * o){
	if(o->needpoll){
		o->needpoll = 0;
		o->on_change();
	}
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
