/*
 * motor.c
 *
 *  Created on: Jan 19, 2023
 *      Author: Denis
 */

#include "motor.h"

void motor_init(xMotor_tt * m, uint8_t id){
	m->id = id;
	motor_stop(m);
}
void motor_stop(xMotor_tt * m){
	m->state = MOTOR_STATE_STOP;
	*((uint32_t *)m->ccr) = 0;
}
void motor_forward(xMotor_tt * m){
	m->state = MOTOR_STATE_FORWARD;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_RESET);
	*((uint32_t *)m->ccr) = m->speed;
}
void motor_back(xMotor_tt * m){
	m->state = MOTOR_STATE_BACK;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_SET);
	*((uint32_t *)m->ccr) = m->speed;
}
void motor_set(xMotor_tt * m, uint16_t speed){
	m->speed = speed;

	if(m->state == MOTOR_STATE_BACK){
		motor_back(m);
	}
	if(m->state == MOTOR_STATE_FORWARD){
		motor_forward(m);
	}
}
uint16_t motor_speed(xMotor_tt *m){
	return m->speed;
}
int8_t motor_dir(xMotor_tt *m){
	if(m->GPIOx && m->pin)
		return HAL_GPIO_ReadPin(m->GPIOx, m->pin);
	return -1;
}
uint8_t motor_state(xMotor_tt * m){
	return m->state;
}
