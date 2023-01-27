/*
 * gate.h
 *
 *  Created on: 24 янв. 2023 г.
 *      Author: Denis
 */

#ifndef GATE_H_
#define GATE_H_
#include "dom.h"

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




#endif /* GATE_H_ */
