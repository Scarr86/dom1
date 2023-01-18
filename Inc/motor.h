/*
 * motor.h
 *
 *  Created on: Jan 19, 2023
 *      Author: Denis
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "main.h"

#define MOTOR_SPEED_MAX (500)

typedef enum{
	MOTOR_STATE_STOP = 0,
	MOTOR_STATE_FORWARD,
	MOTOR_STATE_BACK
}MOTOR_STATE_ENUM;

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	uint32_t ccr;
	uint16_t speed;
	uint8_t state;
}xMotor_tt;


void motor_init(xMotor_tt * m);
void motor_stop(xMotor_tt * m);
void motor_forward(xMotor_tt * m);
void motor_back(xMotor_tt * m);
void motor_set(xMotor_tt * m, uint16_t speed);
uint16_t motor_speed(xMotor_tt *m);
uint8_t motor_dir(xMotor_tt *m);
uint8_t motor_state(xMotor_tt * m);




#endif /* MOTOR_H_ */
