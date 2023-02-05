/*
 * motor.c
 *
 *  Created on: Jan 19, 2023
 *      Author: Denis
 */

#include "motor.h"


void motor_init(xMotor_tt * m, uint8_t id){
	m->id = id;
	m->dist = 0;
	motor_stop(m);
}
void motor_stop(xMotor_tt * m){
	motor_dist_stop(m);
	m->state = MOTOR_STATE_STOP;
	*((uint32_t *)m->ccr) = 0;
}
void motor_forward(xMotor_tt * m){
	m->state = MOTOR_STATE_FORWARD;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_RESET);
	*((uint32_t *)m->ccr) = m->speed;
	motor_dist_start(m, calc_dist_inc);
}
void motor_back(xMotor_tt * m){
	m->state = MOTOR_STATE_BACK;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_SET);
	*((uint32_t *)m->ccr) = m->speed;
	motor_dist_start(m, calc_dist_dec);
}
void motor_set(xMotor_tt * m, int16_t speed, int16_t deg_speed){
	if(speed != -1){
		m->speed = speed;
		if(m->state == MOTOR_STATE_BACK){
			motor_back(m);
		}
		if(m->state == MOTOR_STATE_FORWARD){
			motor_forward(m);
		}
	}
	if(deg_speed != -1){
		m->deg_speed = deg_speed;
	}
}
uint16_t motor_speed(xMotor_tt *m){
	return m->speed;
}
uint16_t motor_deg_speed(xMotor_tt * m){
	return m->deg_speed;
}
int8_t motor_dir(xMotor_tt *m){
	if(m->GPIOx && m->pin)
		return HAL_GPIO_ReadPin(m->GPIOx, m->pin);
	return -1;
}
uint8_t motor_state(xMotor_tt * m){
	return m->state;
}


uint32_t motor_deg(xMotor_tt * m){
	uint32_t deg = m->dist / (m->deg_speed * (1000 / DIST_TIMER_GAP) );
	return (uint8_t)deg;
}

uint32_t motor_dist(xMotor_tt * m){
	return m->dist;
}

void motor_dist_clear(xMotor_tt * m){
	m->dist = 0;
}
void motor_dist_start(xMotor_tt * m, void(*calc_dist)(xTimer_tt * t, void * thisArg)){
	timer_set(&m->timer, DIST_TIMER_GAP, calc_dist, m);
}
void motor_dist_stop(xMotor_tt * m){
	timer_stop(&m->timer);
	uint32_t diff = DIST_TIMER_GAP - timer_remaining(&m->timer);
	if(m->state == MOTOR_STATE_FORWARD){
		m->dist += diff;
	}
	if(m->state == MOTOR_STATE_BACK){
		m->dist =  m->dist < diff ? 0 : m->dist - diff;
	}
	return (m->speed * diff)/ DIST_TIMER_GAP;
}
void calc_dist_inc(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = thisArg;
	timer_reset(t);
	m->dist += m->speed;
}
void calc_dist_dec(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = thisArg;
	timer_reset(t);
	if(m->dist < m->speed){
		m->dist = 0;
	}
	else{
		m->dist -= m->speed;
	}
}
