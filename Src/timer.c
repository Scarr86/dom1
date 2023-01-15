/*
 * timer.c
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#include "timer.h"

void timer_set(xTimer_tt * t, uint32_t interval){
	t->interval = interval;
	t->start = HAL_GetTick();
}

void timer_reset(xTimer_tt *t){
	t->start += t->interval;
}

void timer_restart(xTimer_tt *t){
	t->start = HAL_GetTick();
}

uint8_t timer_expired(xTimer_tt * t){
	uint32_t diff = (HAL_GetTick() - t->start) + 1;
	return diff > t->interval;
}
