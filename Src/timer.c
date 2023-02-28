/*
 * timer.c
 *
 *  Created on: 15 янв. 2023 г.
 *      Author: Denis
 */

#include "timer.h"

uint8_t timer_request_poll_flag;
xTimer_tt * timerlist;

void timer_add(xTimer_tt * timer){

	if(timer->on_timeout == NULL)
		return;

	timer_request_poll();

	xTimer_tt * t;

	for(t = timerlist; t != NULL; t = t->next){
		if(t == timer)
			return;
	}
	timer->is_running = 1;
	timer->next = timerlist;
	timerlist = timer;
}

void timer_del(xTimer_tt * timer){
	xTimer_tt * t;
	if(timer == timerlist){
		timerlist = timerlist->next;
	}
	else{
		for( t = timerlist; t != NULL && t->next != timer; t = t->next);

		if(t != NULL){
			t->next = timer->next;
		}
	}
	timer->next = NULL;
	timer->is_running = 0;
}

void timer_request_poll(void){
	if(timerlist != NULL)
		timer_request_poll_flag = 1;
}
uint8_t timers_poll(){
	xTimer_tt * t;

	if(timer_request_poll_flag){
		timer_request_poll_flag = 0;
		for(t = timerlist; t != NULL; t = t->next ){
			if(timer_expired(t)){
				timer_del(t);
				t->on_timeout(t, t->thisArg);
			}
		}
	}

	return 1;
}

//uint8_t timer_poll(xTimer_tt * t){
//		if(timer_expired(t, t->thisArg)){
//			t->on_timeout(t);
//		}
//}

void timer_stop(xTimer_tt * t){
	timer_del(t);
}
void timer_set(xTimer_tt * t, uint32_t interval, on_timeout_fn on_timeout, void * thisArg){
	t->interval = interval;
	t->start = HAL_GetTick();
	t->thisArg = thisArg;
	t->on_timeout = on_timeout;
	timer_add(t);
}

void timer_reset(xTimer_tt *t){
	t->start += t->interval;
	timer_add(t);
}

void timer_restart(xTimer_tt *t){
	t->start = HAL_GetTick();
	timer_add(t);
}

uint8_t timer_expired(xTimer_tt * t){
	uint32_t diff = (HAL_GetTick() - t->start) + 1;
	return diff > t->interval;
}

uint32_t timer_remaining(xTimer_tt * t){
	if(timer_expired(t))
		return 0;
	else
		return t->start + t->interval - HAL_GetTick();
}

uint8_t timer_is_running(xTimer_tt * t){
	return t->is_running;
}
