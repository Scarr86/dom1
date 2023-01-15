/*
 * timer.h
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"

typedef struct{
	uint32_t start;
	uint32_t interval;
}xTimer_tt;

void timer_set(xTimer_tt * t, uint32_t interval);
void timer_reset(xTimer_tt *t);
void timer_restart(xTimer_tt *t);
uint8_t timer_expired(xTimer_tt * t);

#endif /* TIMER_H_ */
