/*
 * gate.c
 *
 *  Created on: 24 янв. 2023 г.
 *      Author: Denis
 */
#include "gate.h"
static void on_ckick_button(uint8_t id);
static void on_sensor_detected(uint8_t id);
static void gate_sensors_motor_1_detected(xGate_tt * g);
static void gate_sensors_motor_2_detected(xGate_tt * g);


static void gate_close(xGate_tt * gate);
static void gate_stop(xGate_tt * gate);
static void gate_open(xGate_tt * gate);
static void gate_change(GATE_ENUM id, GATE_STATE_ENUM new_state);
static void gate_notify(GATE_ENUM id);

xGate_tt gates[]={
		{
				.id = 0,
				.mid = {MOTOR_1, MOTOR_2},
				.sid = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4},
				.state = GATE_STATE_STOP
		},
		{
				.id = 1,
				.mid = {MOTOR_3, MOTOR_4},
				.sid = {SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8},
				.state = GATE_STATE_STOP
		}
};





xGate_state_tt gate_states[] = {
		{
				.on_click_close = gate_close,
				.on_click_open = gate_open,
		},
		{
				.on_click_close = gate_stop,
				.on_click_open = gate_stop,
		}
};


uint16_t gate_observer_count;
static gate_observer_fn observer[4];

xGate_tt * get_gate(GATE_ENUM id){
	if(id > GATE_COUNT)
		return NULL;
	return &gates[id];
}

uint8_t gate_subscribe(gate_observer_fn observer_fn){
	if(gate_observer_count < 4){
		observer[gate_observer_count] = observer_fn;
		++gate_observer_count;
		return 0;
	}
	return 1;
}

void gate_notify(GATE_ENUM id){
	for(uint16_t i = 0; i < gate_observer_count; ++i){
		observer[i](id);
	}
}

void gate_init(){
	dom_btn_subscribe(on_ckick_button);
	dom_sensor_subscribe(on_sensor_detected);
}


void gate_close(xGate_tt * g){
	xSensor_tt * s1 = get_sensor(g->sid[0]);
	xSensor_tt * s2 = get_sensor(g->sid[2]);
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	if(!sensor_is_detected(s1)){
		motor_forward(m1);
	}
	if(!sensor_is_detected(s2)){
		motor_forward(m2);
	}
	gate_change(g->id, GATE_STATE_ClOSING);
}

void gate_open(xGate_tt * g){
	xSensor_tt * s1 = get_sensor(g->sid[1]);
	xSensor_tt * s2 = get_sensor(g->sid[4]);
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	if(!sensor_is_detected(s1)){
		motor_back(m1);
	}
	if(!sensor_is_detected(s2)){
		motor_back(m2);
	}
	gate_change(g->id, GATE_STATE_OPENING);

}
void gate_stop(xGate_tt * g){
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	motor_stop(m1);
	motor_stop(m2);
	gate_change(g->id, GATE_STATE_STOP);
}

void gate_change(GATE_ENUM id, GATE_STATE_ENUM new_state){
	gates[id].state = new_state;
	gate_notify(id);
}

void gate_sensors_motor_1_detected(xGate_tt * g){

	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);

	motor_stop(m1);

	if(motor_state(m2) == MOTOR_STATE_STOP){
		gate_change(g->id, GATE_STATE_STOP);
	}
}
void gate_sensors_motor_2_detected(xGate_tt * g){

	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);

	motor_stop(m2);

	if(motor_state(m1) == MOTOR_STATE_STOP){
		gate_change(g->id, GATE_STATE_STOP);
	}
}

void on_ckick_button(uint8_t id){

	// state reduce to 0 or 1
	switch(id){
		case BUTTON_1:
			gate_states[!!gates[GATE_1].state].on_click_close(&gates[GATE_1]);
		break;
		case BUTTON_2:
			gate_states[!!gates[GATE_1].state].on_click_open(&gates[GATE_1]);
		break;
		case BUTTON_3:
			gate_states[!!gates[GATE_2].state].on_click_close(&gates[GATE_2]);
		break;
		case BUTTON_4:
			gate_states[!!gates[GATE_2].state].on_click_open(&gates[GATE_2]);
		break;
		default:
		break;
	}
}

void on_sensor_detected(uint8_t id){
	switch(id){
		case SENSOR_1:
		case SENSOR_2:
			gate_sensors_motor_1_detected(&gates[GATE_1]);
		break;
		case SENSOR_3:
		case SENSOR_4:
			gate_sensors_motor_2_detected(&gates[GATE_1]);
		break;
		case SENSOR_5:
		case SENSOR_6:
			gate_sensors_motor_1_detected(&gates[GATE_2]);
		break;
		case SENSOR_7:
		case SENSOR_8:
			gate_sensors_motor_2_detected(&gates[GATE_2]);
		break;
		default:
		break;
	}
}

