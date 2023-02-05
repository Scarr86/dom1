/*
 * motor.h
 *
 *  Created on: Jan 19, 2023
 *      Author: Denis
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "main.h"
#include "timer.h"

#define DIST_TIMER_GAP (100)


#define MOTOR_SPEED_MAX (500)

typedef enum{
	MOTOR_STATE_STOP = 0,
	MOTOR_STATE_FORWARD,
	MOTOR_STATE_BACK
}MOTOR_STATE_ENUM;

typedef struct{
	uint8_t id;
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	uint32_t ccr;
	uint16_t speed;
	uint16_t deg_speed;
	uint8_t state;
	uint8_t full_open;
	uint8_t full_close;
	uint32_t dist;
	xTimer_tt timer;
}xMotor_tt;


void motor_init(xMotor_tt * m, uint8_t id);
void motor_stop(xMotor_tt * m);
void motor_forward(xMotor_tt * m);
void motor_back(xMotor_tt * m);
void motor_set(xMotor_tt * m, int16_t speed, int16_t deg_speed);
uint16_t motor_speed(xMotor_tt *m);
uint16_t motor_deg_speed(xMotor_tt * m);
int8_t motor_dir(xMotor_tt *m);
uint8_t motor_state(xMotor_tt * m);


uint32_t motor_deg(xMotor_tt * m);
uint32_t motor_dist(xMotor_tt * m);
void motor_dist_clear(xMotor_tt * m);
void motor_dist_start(xMotor_tt * m, void(*calc_dist)(xTimer_tt * t, void * thisArg));
void motor_dist_stop(xMotor_tt * m);
void calc_dist_inc(xTimer_tt * t, void * thisArg);
void calc_dist_dec(xTimer_tt * t, void * thisArg);



#endif /* MOTOR_H_ */
