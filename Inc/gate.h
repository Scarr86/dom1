/*
 * gate.h
 *
 *  Created on: 24 янв. 2023 г.
 *      Author: Denis
 */

#ifndef GATE_H_
#define GATE_H_
#include "dom.h"
#include "led.h"

typedef enum{
	GATE_STATE_STOP = 0,
	GATE_STATE_ClOSING,
	GATE_STATE_OPENING
}GATE_STATE_ENUM;

typedef enum{
	GATE_1 = 0,
	GATE_2,
	GATE_COUNT
}GATE_ENUM;

typedef struct{
	uint8_t id;
	MOTOR_ENUM mid[2];
	SENSOR_ENUM sid[4];
	GATE_STATE_ENUM state;
	float koef;
	int16_t angle;
	uint16_t speed;
}xGate_tt;


typedef struct{
	void (*on_click_open)(xGate_tt * gate);
	void (*on_click_close)(xGate_tt * gate);
	void (*on_detected_1)(xGate_tt * gate);
	void (*on_detected_2)(xGate_tt * gate);
	void (*on_detected_3)(xGate_tt * gate);
	void (*on_detected_4)(xGate_tt * gate);

}xGate_state_tt;


typedef void (* gate_observer_fn)(GATE_ENUM id);

void gate_init();

uint8_t gate_subscribe(gate_observer_fn observer_fn);

xGate_tt * get_gate(GATE_ENUM id);

uint16_t gate_speed(uint8_t id);

void gate_poll(uint8_t id);

void dome_poll();

//void dome_close();

// указать номер ворот и нужный угол (считается от вертикали)
void dome_open(uint8_t id, uint16_t angle);

void dome_stop();

uint8_t dome_state(uint8_t id);

uint8_t dome_status(uint8_t id);

//возращает угол закрытия верхней створки
uint8_t dome_encoder(uint8_t id);

float dome_koef(uint8_t id);

int32_t dome_dist(uint8_t id);

int32_t dome_pos(uint8_t id);

// возращает измереное значение
uint16_t dome_deg_speed(uint8_t id);

uint8_t dome_move_params_set(uint16_t pwm_break, uint16_t pwm_full, uint16_t accel, uint16_t angle_break, float koef1, float koef2, uint8_t rain_inf);

#endif /* GATE_H_ */
