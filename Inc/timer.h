/*
 * timer.h
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"


struct xTimer;

typedef void(*on_timeout_fn)(struct xTimer * t , void * thisArg);
typedef struct xTimer{
	struct xTimer * next;
	uint32_t start;
	uint32_t interval;
	void * thisArg;
	on_timeout_fn on_timeout;
}xTimer_tt;


void timer_add(xTimer_tt * timer);
void timer_del(xTimer_tt * timer);
void  timer_request_poll(void);

void timer_set(xTimer_tt * t, uint32_t interval, on_timeout_fn on_timeout, void * thisArg);
void timer_reset(xTimer_tt *t);
void timer_restart(xTimer_tt *t);
void timer_stop(xTimer_tt * t);
uint8_t timer_expired(xTimer_tt * t);
uint8_t timers_poll();
//uint8_t timer_poll(xTimer_tt * t);

#endif /* TIMER_H_ */
