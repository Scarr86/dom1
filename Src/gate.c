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
static void gate_leaf_stop_def();
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
				.on_detected_1 = gate_leaf_stop_def,
				.on_detected_2 = gate_leaf_stop_def,
				.on_detected_3 = gate_leaf_stop_def,
				.on_detected_4 = gate_leaf_stop_def,
		},
		// closing
		{
				.on_click_close = gate_stop,
				.on_click_open = gate_stop,
				.on_detected_1 = gate_leaf_1_stop,
				.on_detected_2 = gate_leaf_stop_def,
				.on_detected_3 = gate_leaf_2_stop,
				.on_detected_4 = gate_leaf_stop_def,
		},
		// opening
		{
				.on_click_close = gate_stop,
				.on_click_open = gate_stop,
				.on_detected_1 = gate_leaf_stop_def ,
				.on_detected_2 = gate_leaf_1_stop,
				.on_detected_3 = gate_leaf_stop_def,
				.on_detected_4 = gate_leaf_2_stop,
		},

};


uint16_t gate_observer_count;
static gate_observer_fn observer[4];

void gate_init(){
	dom_btn_subscribe(on_ckick_button);
	dom_sensor_subscribe(on_sensor_detected);
	dom_sensor_rain_subscribe(on_sensor_rain_detected);
	gates[GATE_1].angle = -1;
	gates[GATE_2].angle = -1;
}

xGate_tt * get_gate(GATE_ENUM id){
	if(id > GATE_COUNT)
		return NULL;
	return &gates[id];
}


uint16_t gate_speed(uint8_t id){
	uint16_t deg = dome_encoder(id);
	if(deg > dom_angle_break() && deg < (90 - dom_angle_break())){
		return dom_pwm_full();
	}
	else{
		return dom_pwm_break();
	}
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
		motor_forward(m1, gate_speed(g->id));
	}
	if(!is_detected_s2){
		motor_forward(m2, gate_speed(g->id));
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
//		// если в середине движения
//		if(dome_encoder(g->id) > dom_angle_break() && dome_encoder(g->id) < (90 - dom_angle_break()))
//			motor_back(m1, dom_pwm_full());
//		else // если подходим к границе
//			motor_back(m1, dom_pwm_break());
		motor_back(m1, gate_speed(g->id));
	}
	if(!is_detected_s2){
//		// если в середине движения
//		if(dome_encoder(g->id) > dom_angle_break() && dome_encoder(g->id) < (90 - dom_angle_break()))
//			motor_back(m2, dom_pwm_full());
//		else// если подходим к границе
//			motor_back(m2, dom_pwm_break());
		motor_back(m2, gate_speed(g->id));
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

void gate_leaf_stop_def(){

}

void gate_leaf_1_stop(xGate_tt * g){
	xMotor_tt * m1 = get_motor(g->mid[0]);
	xMotor_tt * m2 = get_motor(g->mid[1]);
	motor_stop(m1);
	if(g->state == GATE_STATE_OPENING){
		motor_save_pos_0(m1);
	}
	if(g->state == GATE_STATE_ClOSING){
		motor_save_pos_90(m1);
	}

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
	if(g->state == GATE_STATE_OPENING){
		motor_save_pos_0(m2);
	}
	if(g->state == GATE_STATE_ClOSING){
		motor_save_pos_90(m2);
	}
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
		case BUTTON_1: gate_states[gates[GATE_1].state].on_click_close(&gates[GATE_1]);  gates[GATE_1].angle = -1; break;
		case BUTTON_2: gate_states[gates[GATE_1].state].on_click_open(&gates[GATE_1]);   gates[GATE_1].angle = -1; break;
		case BUTTON_3: gate_states[gates[GATE_2].state].on_click_close(&gates[GATE_2]);  gates[GATE_2].angle = -1; break;
		case BUTTON_4: gate_states[gates[GATE_2].state].on_click_open(&gates[GATE_2]);	 gates[GATE_2].angle = -1; break;
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
			dome_open(GATE_1, 0);
			dome_open(GATE_2, 0);
		}
		else{
			dom_led_off(LED_RAIN);
		}
	}
}


void gate_poll(uint8_t id){
	uint16_t deg = dome_encoder(id);
	if(~gates[id].angle){
		if(gates[id].state == GATE_STATE_ClOSING){
			if(deg >= gates[id].angle){
				gate_stop(&gates[id]);
			}
		}
		if(gates[id].state == GATE_STATE_OPENING){
			if(deg <=  gates[id].angle){
				gate_stop(&gates[id]);
			}
		}
	}

	motor_speed_set(get_motor(gates[id].mid[0]), gate_speed(id));
	motor_speed_set(get_motor(gates[id].mid[1]), gate_speed(id));
}

void dome_poll(){
	gate_poll(GATE_1);
	gate_poll(GATE_2);
}

//void dome_close(){
//	gates[0].angle = -1;
//	gates[1].angle = -1;
//	gate_close(&gates[GATE_1]);
//	gate_close(&gates[GATE_2]);
//}


void dome_open(uint8_t id, uint16_t angle){

	uint16_t deg = dome_encoder(id);
	uint16_t need_deg = 90 - angle;

	gates[id].angle = -1;

	if(need_deg == 0){
		gate_open(&gates[id]);
	}
	else if(need_deg == 90){
		gate_close(&gates[id]);
	}
	else{
		if(deg == need_deg){
			return;
		}
		gates[id].angle = need_deg;
		if(deg > need_deg ){
			gate_open(&gates[id]);
		}
		else{
			gate_close(&gates[id]);
		}
	}
}
void dome_stop(){
	gate_stop(&gates[GATE_1]);
	gate_stop(&gates[GATE_2]);
}
uint8_t dome_state(uint8_t id){
	return gates[id].state;
}

uint8_t dome_status(uint8_t id){
	xSensor_tt * s_close_1 = get_sensor(gates[id].sid[0]);
	xSensor_tt * s_close_2 = get_sensor(gates[id].sid[2]);

	xSensor_tt * s_open_1 = get_sensor(gates[id].sid[1]);
	xSensor_tt * s_open_2 = get_sensor(gates[id].sid[3]);

	if(sensor_is_detected(s_close_1) && sensor_is_detected(s_close_2)){
		return 0;
	}
	if(sensor_is_detected(s_open_1) && sensor_is_detected(s_open_2)){
		return 1;
	}
	return 3;
}
//возращает угол закрытия верхней створки
uint8_t dome_encoder(uint8_t id){
	return (dom_motor_deg(gates[id].mid[0]) + dom_motor_deg(gates[id].mid[1])) / 2;
}
float dome_koef(uint8_t id){
	//TODO
	return gates[id].koef;
}

int32_t dome_dist(uint8_t id){
	int32_t dist_1 = dom_motor_dist(gates[id].mid[0]);
	int32_t dist_2 = dom_motor_dist(gates[id].mid[1]);
	if(~dist_1 && ~dist_2){
		return dist_1 + dist_2;
	}
	return -1;
}
int32_t dome_pos(uint8_t id){
	xMotor_tt * m1 = get_motor(gates[id].mid[0]);
	xMotor_tt * m2 = get_motor(gates[id].mid[1]);

	if(m1->is_pos_0 && m2->is_pos_0){
		return (motor_pos(m1) -  m1->pos_0) + (motor_pos(m2) -  m2->pos_0);
	}
	if(m1->is_pos_90 && m2->is_pos_90){
		return ((m1->pos_90 - motor_pos(m1)) + (m2->pos_90 - motor_pos(m2))) * -1;
	}
	return -1;
}

uint16_t dome_deg_speed(uint8_t id){
	xMotor_tt * m1 = get_motor(gates[id].mid[0]);
	xMotor_tt * m2 = get_motor(gates[id].mid[1]);
	return (motor_deg_speed(m1) + motor_deg_speed(m2))  / 2 ;
}

uint8_t dome_move_params_set(uint16_t pwm_break, uint16_t pwm_full, uint16_t accel, uint16_t angle_break, float koef1, float koef2, uint8_t rain_inf){
	uint8_t result = dom_move_params_set(pwm_break, pwm_full, accel, angle_break, koef1, koef2, rain_inf);

	motor_speed_set(get_motor(gates[0].mid[0]), gate_speed(0));
	motor_speed_set(get_motor(gates[0].mid[1]), gate_speed(0));

	motor_speed_set(get_motor(gates[1].mid[0]), gate_speed(1));
	motor_speed_set(get_motor(gates[1].mid[1]), gate_speed(1));

	return result;
}




