/*
 * gate.c
 *
 *  Created on: 24 янв. 2023 г.
 *      Author: Denis
 */
#include "gate.h"





static void on_ckick_button(uint8_t id);
static void on_sensor_detected(uint8_t id);
static void on_sensor_rain_detected(uint8_t id);


//typedef void (* sensor_rain_observers_fn)(uint8_t id);


/*
 * купол состоит из 2х ворот, каждые ворота состоят из 2х створок
 * ниже фукции для открытия, закрыти, остоновки ворот и
 * остановки створок, соответственно
 */

// закрыть ворота (передается указатель на ворота)
static void gate_close(xGate_tt * gate);
// открыть ворота (параметры указатель на ворота)
static void gate_open(xGate_tt * gate);
// остановить ворота (параметры указатель на ворота)
static void gate_stop(xGate_tt * gate);

// остановка створки по умолчанию
static void gate_def_leaf_stop();
// остановить 1ю створку(нижняя) (в параметрах указатель на ворота)
static void gate_leaf_1_stop(xGate_tt * g);
// остановить 2ю створку(верхняя)(в параметрах указатель на ворота)
static void gate_leaf_2_stop(xGate_tt * g);


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




// описывает 3 состояния для обработки по нажатию кнопок и срабатыванию датчиков
xGate_state_tt gate_states[] = {
		// stop
		{
				.on_click_close = gate_close,
				.on_click_open = gate_open,
				.on_detected_1 = gate_def_leaf_stop,
				.on_detected_2 = gate_def_leaf_stop,
				.on_detected_3 = gate_def_leaf_stop,
				.on_detected_4 = gate_def_leaf_stop,
		},
		// closing
		{
				.on_click_close = gate_stop,
				.on_click_open = gate_stop,
				.on_detected_1 = gate_leaf_1_stop,
				.on_detected_2 = gate_def_leaf_stop,
				.on_detected_3 = gate_leaf_2_stop,
				.on_detected_4 = gate_def_leaf_stop,
		},
		// opening
		{
				.on_click_close = gate_stop,
				.on_click_open = gate_stop,
				.on_detected_1 = gate_def_leaf_stop ,
				.on_detected_2 = gate_leaf_1_stop,
				.on_detected_3 = gate_def_leaf_stop,
				.on_detected_4 = gate_leaf_2_stop,
		},

};


uint16_t gate_observer_count;
static gate_observer_fn observer[4];

void gate_init(){
	dom_btn_subscribe(on_ckick_button);
	dom_sensor_subscribe(on_sensor_detected);
	dom_sensor_rain_subscribe(on_sensor_rain_detected);
}

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

void gate_close(xGate_tt * g){
	xSensor_tt * s1 = get_sensor(g->sid[0]);
	xSensor_tt * s2 = get_sensor(g->sid[2]);
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	uint8_t is_detected_s1 = sensor_is_detected(s1);
	uint8_t is_detected_s2 = sensor_is_detected(s2);
	if(!is_detected_s1){
		motor_forward(m1);
	}
	if(!is_detected_s2){
		motor_forward(m2);
	}
	if(!is_detected_s1 || !is_detected_s2){
		gate_change(g->id, GATE_STATE_ClOSING);
	}
}

void gate_open(xGate_tt * g){
	xSensor_tt * s1 = get_sensor(g->sid[1]);
	xSensor_tt * s2 = get_sensor(g->sid[3]);
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	uint8_t is_detected_s1 = sensor_is_detected(s1);
	uint8_t is_detected_s2 = sensor_is_detected(s2);
	if(!is_detected_s1){
		motor_back(m1);
	}
	if(!is_detected_s2){
		motor_back(m2);
	}
	if(!is_detected_s1 || !is_detected_s2){
		gate_change(g->id, GATE_STATE_OPENING);
	}
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

	if(new_state == GATE_STATE_STOP){
		dom_led_off(LED_MOTOR_MOVE);
	}
	else{
		dom_led_on(LED_MOTOR_MOVE);
		if(id == GATE_1){
			dom_led_off(LED_OPEN_CLOSE_GATE_1);
		}
		if(id == GATE_2){
			dom_led_off(LED_OPEN_CLOSE_GATE_2);
		}
	}

	gate_notify(id);
}

void gate_def_leaf_stop(){

}

void gate_leaf_1_stop(xGate_tt * g){
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	motor_stop(m1);
	if(motor_state(m2) == MOTOR_STATE_STOP){
		if(g->id == GATE_1){
			dom_led_on(LED_OPEN_CLOSE_GATE_1);
		}
		if(g->id == GATE_2){
			dom_led_on(LED_OPEN_CLOSE_GATE_2);
		}
		gate_change(g->id, GATE_STATE_STOP);
	}
}

void gate_leaf_2_stop(xGate_tt * g){
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	motor_stop(m2);
	if(motor_state(m1) == MOTOR_STATE_STOP){
		if(g->id == GATE_1){
			dom_led_on(LED_OPEN_CLOSE_GATE_1);
		}
		if(g->id == GATE_2){
			dom_led_on(LED_OPEN_CLOSE_GATE_2);
		}
		gate_change(g->id, GATE_STATE_STOP);
	}
}

void on_ckick_button(uint8_t id){
	switch(id){
		case BUTTON_1: gate_states[gates[GATE_1].state].on_click_close(&gates[GATE_1]); break;
		case BUTTON_2: gate_states[gates[GATE_1].state].on_click_open(&gates[GATE_1]);  break;
		case BUTTON_3: gate_states[gates[GATE_2].state].on_click_close(&gates[GATE_2]); break;
		case BUTTON_4: gate_states[gates[GATE_2].state].on_click_open(&gates[GATE_2]);	break;
		default: break;
	}
}

/* датчик с 1-4 - ворота 1
 * датчик с 5-8 - ворота 2
 */
void on_sensor_detected(uint8_t id){
	switch(id){
		case SENSOR_1: gate_states[gates[GATE_1].state].on_detected_1(&gates[GATE_1]);	break;
		case SENSOR_2: gate_states[gates[GATE_1].state].on_detected_2(&gates[GATE_1]);	break;
		case SENSOR_3: gate_states[gates[GATE_1].state].on_detected_3(&gates[GATE_1]);  break;
		case SENSOR_4: gate_states[gates[GATE_1].state].on_detected_4(&gates[GATE_1]);	break;
		case SENSOR_5: gate_states[gates[GATE_2].state].on_detected_1(&gates[GATE_2]);	break;
		case SENSOR_6: gate_states[gates[GATE_2].state].on_detected_2(&gates[GATE_2]);	break;
		case SENSOR_7: gate_states[gates[GATE_2].state].on_detected_3(&gates[GATE_2]);	break;
		case SENSOR_8: gate_states[gates[GATE_2].state].on_detected_4(&gates[GATE_2]);	break;
		default:	break;
	}
}

void on_sensor_rain_detected(uint8_t id){
	if(id == SENSOR_RAIN_1){
		if(dom_sensor_rain_is_detected(SENSOR_RAIN_1)){
			dom_led_on(LED_RAIN);
			dome_close();
		}
		else{
			dom_led_off(LED_RAIN);
		}
	}
}

void dome_close(){
	gate_close(&gates[GATE_1]);
	gate_close(&gates[GATE_2]);
}
void dome_open(uint8_t id, uint16_t angle){
	if(angle >= 90){
		gate_open(&gates[id]);
	}
	else{
		//TODO
		gate_open(&gates[id]);
	}
}
void dome_stop(){
	gate_stop(&gates[GATE_1]);
	gate_stop(&gates[GATE_2]);
}

uint8_t dome_state(uint8_t id){
	//TODO
	return 3;
}
//возращает угол закрытия верхней створки
uint8_t dome_encoder(uint8_t id){
	// TODO
	return 0;
}
float dome_koef(uint8_t id){
	//TODO
	return gates[id].koef;
}



