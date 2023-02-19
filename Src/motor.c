/*
 * motor.c
 *
 *  Created on: Jan 19, 2023
 *      Author: Denis
 */

#include "motor.h"

//uint16_t new_speed;


void motor_init(xMotor_tt * m, uint8_t id){
	m->id = id;
	m->pos = 0;
	m->cur_speed = 0;

	m->pos_0 = 0;
	m->pos_90 = 0;
	m->is_pos_0 = 0;
	m->is_pos_90 = 0;

	//	m->pos_0 = -50000; // todo debug
	//	m->pos_90 = 50000; // todo debug
	//	m->is_pos_0 = 1; // todo debug
	//	m->is_pos_90 = 1; // todo debug

	motor_stop(m);
}
void motor_stop(xMotor_tt * m){

	m->state = MOTOR_STATE_STOP;
	m->cur_speed = m->cur_speed > m->accel ? m->cur_speed - m->accel: 0;
	*((uint32_t *)m->ccr) = m->cur_speed;
	if(m->cur_speed)
		timer_set(&m->accel_timer, DIST_TIMER_GAP, accel_stop, m);
	else{
		timer_del(&m->accel_timer);
		motor_pos_stop(m);
	}
}

void motor_speed_set(xMotor_tt * m, uint16_t speed){

	m->speed = speed;
	if(m->state == MOTOR_STATE_STOP){
		return;
	}
	if(m->cur_speed == speed){
		return;
	}
	if(timer_is_running(&m->accel_timer)){
		return;
	}


	if(m->cur_speed > speed){
		if(m->cur_speed > m->accel){
			m->cur_speed = m->cur_speed - m->accel;
		}
		else{
			m->cur_speed = speed;
		}
		if(m->cur_speed != speed){	//вызываются разные callback
			timer_set(&m->accel_timer, DIST_TIMER_GAP, accel_move_dec, m);
		}
	}

	if(m->cur_speed < speed){
		m->cur_speed = m->cur_speed + m->accel < speed ?  m->cur_speed + m->accel: speed;
		if(m->cur_speed != speed){//вызываются разные callback
			timer_set(&m->accel_timer, DIST_TIMER_GAP, accel_move_inc, m);
		}
	}

	*((uint32_t *)m->ccr) = m->cur_speed;
}
void motor_forward(xMotor_tt * m, uint16_t speed){
	m->state = MOTOR_STATE_FORWARD;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_RESET);
	motor_speed_set(m, speed);
	motor_pos_start(m, calc_pos_inc);
	//*((uint32_t *)m->ccr) = m->cur_speed;
	//motor_pos_start(m, calc_pos_inc);
	//timer_set(&m->accel_timer, DIST_TIMER_GAP, accel_stop, m);
}

void motor_back(xMotor_tt * m, uint16_t speed){
	m->state = MOTOR_STATE_BACK;
	if(m->GPIOx && m->pin)
		HAL_GPIO_WritePin(m->GPIOx, m->pin, GPIO_PIN_SET);
	motor_speed_set(m, speed);
	motor_pos_start(m, calc_pos_dec);
}

void motor_set(xMotor_tt * m, int16_t speed, int16_t deg_speed, int16_t accel){

	if(~accel){
		m->accel = accel;
	}

	if(speed != -1){
		m->speed = speed;
//		if(m->state == MOTOR_STATE_BACK){
//			motor_back(m);
//		}
//		if(m->state == MOTOR_STATE_FORWARD){
//			motor_forward(m);
//		}
	}
	if(deg_speed != -1){
		m->deg_speed = deg_speed;
	}
}
uint16_t motor_speed(xMotor_tt *m){
	return m->speed;
}
uint16_t motor_deg_speed(xMotor_tt * m){
	int32_t dist = motor_dist(m);
	if(~dist){ // if has dist
		return (dist / 90) / (1000 / DIST_TIMER_GAP);
	}
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
int16_t motor_deg(xMotor_tt * m){

	if(m->is_pos_0 && m->is_pos_90){
		return ((m->pos - m->pos_0) * 90) / motor_dist(m);
	}
	else if(m->is_pos_0){
		return ((m->pos - m->pos_0) / m->deg_speed) / (1000 / DIST_TIMER_GAP);
	}
	else if( m->is_pos_90){
		return 90 - (((m->pos_90 - m->pos) / m->deg_speed) / (1000 / DIST_TIMER_GAP));
	}
	else{
		return -1;
	}
}
int32_t motor_pos(xMotor_tt * m){
	return m->pos;
}
void motor_pos_clear(xMotor_tt * m){
	m->pos = 0;
}
void motor_pos_start(xMotor_tt * m, void(*calc_pos)(xTimer_tt * t, void * thisArg)){
	timer_set(&m->pos_timer, DIST_TIMER_GAP, calc_pos, m);
}
void motor_pos_stop(xMotor_tt * m){
	timer_stop(&m->pos_timer);
	uint32_t diff = DIST_TIMER_GAP - timer_remaining(&m->pos_timer);
	uint32_t dist = (m->speed * diff)/ DIST_TIMER_GAP;
	if(m->state == MOTOR_STATE_FORWARD){
		m->pos += dist;
	}
	if(m->state == MOTOR_STATE_BACK){
		m->pos -=  dist;
	}
	motor_pos_adjust(m);

}

void motor_pos_adjust(xMotor_tt * m){
	if(m->is_pos_90 && m->pos > m->pos_90){
		m->pos_90 = m->pos;
	}

	if(m->is_pos_0 && m->pos < m->pos_0){
		m->pos_0 = m->pos;
	}
}
void calc_pos_inc(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = thisArg;
	timer_reset(t);
	m->pos += m->cur_speed;
	motor_pos_adjust(m);
}
void calc_pos_dec(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = thisArg;
	timer_reset(t);
	m->pos -= m->cur_speed;
	motor_pos_adjust(m);
}
void motor_save_pos_0(xMotor_tt * m){
	m->is_pos_0 = 1;
	m->pos_0 = m->pos;
}
void motor_save_pos_90(xMotor_tt * m){
	m->is_pos_90 = 1;
	m->pos_90 = m->pos;
}
int32_t motor_pos_0(xMotor_tt * m){
	if(m->is_pos_0)
		return m->pos_0;
	return -1;
}
int32_t motor_pos_90(xMotor_tt * m){
	if(m->is_pos_90)
		return m->pos_90;
	return -1;
}
int32_t motor_dist(xMotor_tt * m){
	if(m->is_pos_0 && m->is_pos_90 && (m->pos_90 > m->pos_0))
		return m->pos_90 - m->pos_0;
	return -1;
}


void accel_stop(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = (xMotor_tt *) thisArg;
	m->cur_speed = m->cur_speed > m->accel ? m->cur_speed - m->accel: 0;
	if(m->cur_speed){
		timer_reset(t);
	}
	else{
		motor_pos_stop(m);
	}
	*((uint32_t *)m->ccr) = m->cur_speed;
}
void accel_move_inc(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = (xMotor_tt *) thisArg;

	m->cur_speed = m->cur_speed + m->accel < m->speed ?  m->cur_speed + m->accel: m->speed;

	if(m->cur_speed != m->speed){
		timer_reset(t);
	}

	*((uint32_t *)m->ccr) = m->cur_speed;
}
void accel_move_dec(xTimer_tt * t, void * thisArg){
	xMotor_tt * m = (xMotor_tt *) thisArg;

	if(m->cur_speed > m->accel){
		m->cur_speed = (m->cur_speed - m->accel) > m->speed ? m->cur_speed - m->accel:  m->speed;
	}
	else{
		m->cur_speed = m->speed;
	}

	if(m->cur_speed != m->speed){
		timer_reset(t);
	}

	*((uint32_t *)m->ccr) = m->cur_speed;

}
