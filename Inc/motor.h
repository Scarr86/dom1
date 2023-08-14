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
	uint8_t dir;
	uint16_t cur_speed;
	uint16_t deg_speed;
	uint8_t state;
	int32_t pos;
	uint8_t is_pos_0;
	int32_t pos_0;
	uint8_t is_pos_90;
	int32_t pos_90;
	xTimer_tt pos_timer;

	uint16_t accel;
	xTimer_tt accel_timer;
}xMotor_tt;


void motor_init(xMotor_tt * m, uint8_t id);
void motor_stop(xMotor_tt * m);
void motor_forward(xMotor_tt * m, uint16_t speed);
void motor_back(xMotor_tt * m, uint16_t speed);

void motor_rele_forward(xMotor_tt * m);
void motor_rele_back(xMotor_tt * m);

void motor_speed_set(xMotor_tt * m, uint16_t speed);
void motor_set(xMotor_tt * m, int8_t dir, int16_t speed, int16_t deg_speed, int16_t accel);
uint16_t motor_speed(xMotor_tt *m);
uint16_t motor_accel(xMotor_tt *m);
uint16_t motor_deg_speed(xMotor_tt * m);
int8_t motor_dir(xMotor_tt *m);
uint8_t motor_state(xMotor_tt * m);

int32_t motor_pos(xMotor_tt * m);
void motor_pos_start(xMotor_tt * m, void(*calc_pos)(xTimer_tt * t, void * thisArg));
void motor_pos_stop(xMotor_tt * m);
void motor_pos_adjust(xMotor_tt * m);
void calc_pos_inc(xTimer_tt * t, void * thisArg);
void calc_pos_dec(xTimer_tt * t, void * thisArg);


//void accel_inc(xTimer_tt * t, void * thisArg);
//void accel_dec(xTimer_tt * t, void * thisArg);

void accel_stop(xTimer_tt * t, void * thisArg);
void accel_move_inc(xTimer_tt * t, void * thisArg);
void accel_move_dec(xTimer_tt * t, void * thisArg);



void motor_save_pos_0(xMotor_tt * m);
void motor_save_pos_90(xMotor_tt * m);

int32_t motor_pos_0(xMotor_tt * m);
int32_t motor_pos_90(xMotor_tt * m);

int32_t motor_dist(xMotor_tt * m);



#endif /* MOTOR_H_ */
