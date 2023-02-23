/*
 * dom.c
 *
 *  Created on: Jan 13, 2023
 *      Author: Denis
 */


#include "dom.h"

//uint16_t pwm_break;
//uint16_t pwm_full;
//uint16_t pwm_accel;
//uint16_t angle_break;


xLed_pwm_tt led_pwm = {
		.ccr = (uint32_t)&TIM1->CCR2,
};

xLed_tt led []={
		{
				.GPIOx = LED_MOTOR_MOVE_GPIO_Port,
				.pin = LED_MOTOR_MOVE_Pin
		},
		{
				.GPIOx = LED_OPEN_CLOSE_GATE_1_GPIO_Port,
				.pin = LED_OPEN_CLOSE_GATE_1_Pin
		},
		{
				.GPIOx = LED_OPEN_CLOSE_GATE_2_GPIO_Port,
				.pin = LED_OPEN_CLOSE_GATE_2_Pin
		},
		{
				.GPIOx = LED_RAIN_GPIO_Port,
				.pin = LED_RAIN_Pin
		}
};


xButton_tt btn[] = {
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_0,
			.on_click = dom_btn_1_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_1,
			.on_click = dom_btn_2_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_2,
			.on_click = dom_btn_3_on_click,
		},
		{
			.GPIOx = GPIOD,
			.pin =  GPIO_PIN_3,
			.on_click = dom_btn_4_on_click,
		}
};

xSensor_tt sensor[] ={
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_10,
				.on_detected = dom_sensor_1_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_11,
				.on_detected = dom_sensor_2_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_12,
				.cmpVal = 1,
				.on_detected = dom_sensor_3_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_13,
				.on_detected = dom_sensor_4_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_14,
				.on_detected = dom_sensor_5_on_detected
		},
		{
				.GPIOx = GPIOD,
				.pin = GPIO_PIN_15,
				.on_detected = dom_sensor_6_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_2,
				.on_detected = dom_sensor_7_on_detected
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_3,
				.on_detected = dom_sensor_8_on_detected
		},

};

xSensor_rain_tt  sensor_rain[] = {
		{
				.GPIOx = SENSOR_RAIN_GPIO_Port,
				.pin = SENSOR_RAIN_Pin,
				.on_change = dom_sensor_rain_1_on_change
		}
};

xMotor_tt motor[]={
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_1,
				.ccr = (uint32_t)&TIM12->CCR2,
		},
		{
				.GPIOx = GPIOG,
				.pin = GPIO_PIN_0,
				.ccr = (uint32_t)&TIM3->CCR2,
		},
		{
				.GPIOx = GPIOF,
				.pin = GPIO_PIN_15,
				.ccr = (uint32_t)&TIM3->CCR3,
		},
		{
				.GPIOx = GPIOF,
				.pin = GPIO_PIN_14,
				.ccr = (uint32_t)&TIM1->CCR1,
		},
		{
				.GPIOx = 0,
				.pin = 0,
				.ccr = (uint32_t)&TIM1->CCR2,
		},
};


xRele_tt rele[]={
		{
				.GPIOx = SS_REL_1_GPIO_Port,
				.pin = SS_REL_1_Pin
		},
		{
				.GPIOx = SS_REL_2_GPIO_Port,
				.pin = SS_REL_2_Pin
		},
		{
				.GPIOx = SS_REL_3_GPIO_Port,
				.pin = SS_REL_3_Pin
		}
};

xOdometer_tt odometer[4] = {
	{
			.on_change = dom_odometer_1_on_change
	},
	{
			.on_change = dom_odometer_2_on_change
	},
	{
			.on_change = dom_odometer_3_on_change
	},
	{
			.on_change = dom_odometer_4_on_change
	}
};

uint8_t odometer_observers_count;
odometer_observers_fn odometer_observers[4];

uint8_t btn_observers_count;
btn_observers_fn btn_observers[4];

uint8_t sensor_observers_count;
sensor_observers_fn sensor_observers[4];

uint8_t sensor_rain_observers_count;
odometer_observers_fn sensor_rain_observers[4];


xDom_settings_tt dom_settings;
uint8_t settings_is_valid;


void dom_init(){

	settings_is_valid  = settings_read(&dom_settings);
	//settings_is_valid  = 0;

	if(!settings_is_valid){
	//{ // todo debug
		dom_settings.pwm_break = PWM_BREAK_DEF;
		dom_settings.pwm_full = PWM_FULL_DEF;
		dom_settings.pwm_accel = PWM_ACCEL_DEF;
		dom_settings.angle_break = ANGLE_BREAK_DEF;
	}

	for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
		btn_init(&btn[i]);
		if(!settings_is_valid){
			dom_settings.btn_settings[i].debounceTime = BUTTON_DEBOUNCE_TIME_DEF;
		}
		btn_set(&btn[i], dom_settings.btn_settings[i].debounceTime);

	}
	for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
		sensor_init(&sensor[i]);
		if(!settings_is_valid){
			dom_settings.sensor_settings[i].cmpVal = SENSOR_CMP_VAL_DEF;
		}
		sensor_set(&sensor[i], dom_settings.sensor_settings[i].cmpVal);
	}
	for(uint16_t i = 0; i < MOTOR_COUNT; ++i){
		motor_init(&motor[i], i);
		if(!settings_is_valid){
			dom_settings.motor_settings[i].deg_speed = MOTOR_DEG_SPEED_DEF;
			dom_settings.motor_settings[i].dir = MOTOR_DIR_DEF;
		}
		motor_set(&motor[i], dom_settings.motor_settings[i].dir, dom_settings.pwm_break, dom_settings.motor_settings[i].deg_speed, dom_settings.pwm_accel);
	}
	for(uint16_t i = 0; i < RELE_COUNT; ++i){
		rele_inactive(&rele[i]);
	}

	for(uint16_t i = 0; i < SENSOR_RAIN_COUNT; ++i){
		sensor_rain_init(&sensor_rain[i]);
		if(!settings_is_valid){
			dom_settings.sensor_rain_settings[i].enable = SENSOR_RAIN_ENABLE_DEF;
			dom_settings.sensor_rain_settings[i].cmp_val = SENSOR_RAIN_CMP_VAL_DEF;
		}
		sensor_rain_set(&sensor_rain[i],
				dom_settings.sensor_rain_settings[i].enable,
				dom_settings.sensor_rain_settings[i].cmp_val);
	}




	for(uint8_t i = 0; i < 5; ++i ){
		led_pwm_off(&led_pwm);
		HAL_Delay(300);
		led_pwm_on(&led_pwm);
		HAL_Delay(300);
	}

	//motor_forward(&motor[MOTOR_5]);
	//led_pwm_blink(&led_pwm, 4);
}
void dom_poll(){
	for(uint16_t i = 0; i < BUTTON_COUNT; ++i){
		btn_poll(&btn[i]);
	}
	for(uint16_t i = 0; i < SENSOR_COUNT; ++i){
		sensor_poll(&sensor[i]);
	}
	for(uint16_t i = 0; i < ODOMETER_COUNT; ++i){
		odometer_poll(&odometer[i]);
	}
	for(uint16_t i = 0; i < SENSOR_RAIN_COUNT; ++i){
		sensor_rain_poll(&sensor_rain[i]);
	}

}

uint16_t settings_write(xDom_settings_tt * ds){
	uint8_t data[sizeof(xDom_settings_tt) + 2];
	memcpy(data, ds, sizeof(xDom_settings_tt));
	uint16_t iks1 = sizeof data - 2;
	uint16_t iks2 = sizeof data - 1;
	ks16(ds, sizeof(xDom_settings_tt), &data[iks1], &data[iks2]);

	return flash_write((void *) data, sizeof data);
}
uint8_t settings_read(xDom_settings_tt * ds){
	uint8_t data[sizeof(xDom_settings_tt) + 2];

	flash_read((void *)data, sizeof data);
	if(ks16_check(data, sizeof data)){
		memcpy(ds, data, sizeof(xDom_settings_tt));
		return 1;
	}
	return 0;
}
xButton_tt * get_button(BUTTON_ENUM id){
	return &btn[id];
}
xSensor_tt * get_sensor(SENSOR_ENUM id){
	return &sensor[id];
}
xSensor_rain_tt * get_sensor_rain(SENSOR_RAIN_ENUM id){
	return &sensor_rain[id];
}
xMotor_tt * get_motor(MOTOR_ENUM id){
	return &motor[id];
}

// BUTTON FUNCTION START
int8_t dom_btn_set(uint8_t id, int16_t debounceTime){
	if(id == 0 || id > BUTTON_COUNT){
		return 1;
	}
	if(debounceTime == 0){
		return 0;
	}

	dom_settings.btn_settings[id - 1].debounceTime = debounceTime;
	btn_set(&btn[id - 1], debounceTime);
	if(!settings_write(&dom_settings)){
		return 0;
	}
	return 1;
}
int8_t dom_btn_state(uint8_t id){
	if(id == 0 || id > BUTTON_COUNT){
		return  -1;
	}
	return btn_state(&btn[id - 1]);
}
int16_t dom_btn_debounce_time(uint8_t id){
	if(id == 0 || id > BUTTON_COUNT){
		return -1;
	}
	return btn_debounceTime(&btn[id - 1]);
}
uint8_t dom_btn_subscribe(btn_observers_fn obs){
	if(btn_observers_count < 4){
		btn_observers[btn_observers_count] = obs;
		++btn_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_btn_notify(uint8_t indx){
	for(uint16_t i = 0; i < btn_observers_count; ++i){
		btn_observers[i](indx);
	}
	return 0;
}
void dom_btn_1_on_click(){
	dom_btn_notify(BUTTON_1);
}
void dom_btn_2_on_click(){
	dom_btn_notify(BUTTON_2);
}
void dom_btn_3_on_click(){
	dom_btn_notify(BUTTON_3);
}
void dom_btn_4_on_click(){
	dom_btn_notify(BUTTON_4);
}
// BUTTON FUNCTION END

// LED FUNCTION START
void dom_led_on(uint8_t id){
	led_on(&led[id]);
}
void dom_led_off(uint8_t id){
	led_off(&led[id]);
}
// LED FUNCTION END

// LED-PWM FUNCTION START
uint8_t dom_led_pwm_set(uint8_t on, uint8_t mode, uint8_t frq){
	if(mode == LED_BLINK_OFF){
		if(on)
			led_pwm_on(&led_pwm);
		else
			led_pwm_off(&led_pwm);
	}
	if(mode == LED_BLINK_ON){
		led_pwm_blink(&led_pwm, frq);
	}
	return 0;
}
uint8_t dom_led_pwm_mode(){
	return led_pwm_mode(&led_pwm);
}
uint8_t dom_led_pwm_state(){
	return led_pwm_state(&led_pwm);
}
uint16_t dom_led_pwm_frq(){
	return led_pwm_frq(&led_pwm);
}
// LED-PWM FUNCTION END

// SENSOR FUNCTION START
uint8_t dom_sensor_set(uint8_t id, uint8_t cmpVal){
	if(id == 0 || id > SENSOR_COUNT){
		return 1;
	}
	dom_settings.sensor_settings[id - 1].cmpVal = !!cmpVal;
	sensor_set(&sensor[id - 1], !!cmpVal);
	if(!settings_write(&dom_settings)){
		return 0;
	}
	return 1;
}
uint8_t dom_sensor_subscribe(sensor_observers_fn obs){
	if(sensor_observers_count < 4){
		sensor_observers[sensor_observers_count] = obs;
		++sensor_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_sensor_notify(uint8_t indx){
	for(uint16_t i = 0; i < sensor_observers_count; ++i){
		sensor_observers[i](indx);
	}
	return 0;
}
int8_t dom_sensor_state(uint8_t id){
	if(id == 0 || id > SENSOR_COUNT)
		return  -1;
	return sensor_state(&sensor[id - 1]);
}
int8_t dom_sensor_cmp_val(uint8_t id){
	if(id == 0 || id > SENSOR_COUNT){
		 return - 1;
	}
	return sensor_cmp_val(&sensor[id - 1]);
}
int8_t dom_sensor_is_detected(uint8_t id){
	return sensor_is_detected(&sensor[id]);
}
void dom_sensor_1_on_detected(){
	dom_sensor_notify(SENSOR_1);
}
void dom_sensor_2_on_detected(){
	dom_sensor_notify(SENSOR_2);
}
void dom_sensor_3_on_detected(){
	dom_sensor_notify(SENSOR_3);
}
void dom_sensor_4_on_detected(){
	dom_sensor_notify(SENSOR_4);
}
void dom_sensor_5_on_detected(){
	dom_sensor_notify(SENSOR_5);
}
void dom_sensor_6_on_detected(){
	dom_sensor_notify(SENSOR_6);
}
void dom_sensor_7_on_detected(){
	dom_sensor_notify(SENSOR_7);
}
void dom_sensor_8_on_detected(){
	dom_sensor_notify(SENSOR_8);
}
// SENSOR FUNCTION END


// SENSOR-RAIN  FUNCTION START
uint8_t dom_sensor_rain_set(uint8_t id, int8_t enable, int8_t cmpVal){

	if(enable >= 0)
		dom_settings.sensor_rain_settings[id].enable = enable;
	if(cmpVal >= 0)
		dom_settings.sensor_rain_settings[id].cmp_val = cmpVal;

	sensor_rain_set(&sensor_rain[id], enable, cmpVal);

	return settings_write(&dom_settings);
}
uint8_t dom_sensor_rain_subscribe(sensor_rain_observers_fn obs){
	if(sensor_rain_observers_count < 4){
		sensor_rain_observers[sensor_rain_observers_count] = obs;
		++sensor_rain_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_sensor_rain_notify(uint8_t indx){
	for(uint16_t i = 0; i < sensor_rain_observers_count; ++i){
		sensor_rain_observers[i](indx);
	}
	return 0;
}
int8_t dom_sensor_rain_state(uint8_t id){
	return sensor_rain_state(&sensor_rain[id]);
}
int8_t dom_sensor_rain_is_detected(uint8_t id){
	return sensor_rain_is_detected(&sensor_rain[id]);
}
int8_t dom_sensor_rain_is_enable(uint8_t id){
	return sensor_rain_is_enable(&sensor_rain[id]);
}
uint8_t dom_sensor_rain_cmpval(uint8_t id){
	return sensor_rain_cmpval(&sensor_rain[id]);
}
void dom_sensor_rain_1_on_change(){
	dom_sensor_rain_notify(SENSOR_RAIN_1);
}
// SENSOR-RAIN FUNCTION END

// MOTOR FUNCTION START
int8_t dom_motor_state(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return -1;
	return motor_state(&motor[id-1]);
}
int16_t dom_motor_speed(uint8_t id){
	return motor_speed(&motor[id]);
}
int16_t dom_motor_deg_speed(uint8_t id){
	return dom_settings.motor_settings[id].deg_speed;
}
int8_t dom_motor_dir(uint8_t id){
	if(id == 0 || id > MOTOR_COUNT)
		return -1;
	return motor_dir(&motor[id-1]);
}
uint8_t dom_motor_set(uint8_t id, int8_t dir, int16_t speed, int16_t min_speed, int16_t deg_speed, int16_t accel){

	if(~dir){
		dom_settings.motor_settings[id].dir = dir;
	}
	if(deg_speed != -1){
		dom_settings.motor_settings[id].deg_speed = deg_speed;
	}
	if(speed != -1){
		dom_settings.pwm_full = speed;
	}
	if(min_speed != -1){
		dom_settings.pwm_break = min_speed;
	}
	if(~accel){
		dom_settings.pwm_accel = accel;
	}

	motor_set(&motor[id], dir, speed, deg_speed, accel);

	if(~dir || speed != -1 || deg_speed != -1 || min_speed != -1){
		return settings_write(&dom_settings);
	}

	return 1;
}
void dom_motor_forward(uint8_t id, uint16_t speed){
	motor_forward(&motor[id], speed);
}
void dom_motor_back(uint8_t id, uint16_t speed){
	motor_back(&motor[id], speed);
}
void dom_motor_stop(uint8_t id){
	motor_stop(&motor[id]);
}
int32_t dom_motor_pos(uint8_t id){
	return motor_pos(&motor[id]);
}
//void dom_motor_dist_clear(uint8_t id){
//	motor_dist_clear(&motor[id]);
//}
//void dom_motor_dist_start(uint8_t id){
//	motor_dist_start(&motor[id]);
//}
//void dom_motor_dist_stop(uint8_t id){
//	motor_dist_stop(&motor[id]);
//}

void dom_motor_save_pos_0(uint8_t id){
	motor_save_pos_0(&motor[id]);
}
void dom_motor_save_pos_90(uint8_t id){
	motor_save_pos_90(&motor[id]);
}
int32_t dom_motor_dist(uint8_t id){
	return motor_dist(&motor[id]);
}
int32_t dom_motor_pos_0(uint8_t id){
	return motor_pos_0(&motor[id]);
}
int32_t dom_motor_pos_90(uint8_t id){
	return motor_pos_90(&motor[id]);
}
// MOTOR FUNCTION END

// ODOMETER FUNCTION START
uint8_t dom_odometer_subscribe(odometer_observers_fn obs){
	if(odometer_observers_count < 4){
		odometer_observers[odometer_observers_count] = obs;
		++odometer_observers_count;
		return 0;
	}
	return 1;
}
uint8_t dom_odometer_notify(uint8_t id){
	for(uint16_t i = 0; i < odometer_observers_count; ++i){
		odometer_observers[i](id);
	}
	return 0;
}
xOdometer_tt * dom_odometer(uint8_t id){
	return &odometer[id];
}
uint32_t dom_odometer_value(uint8_t id){
	return odometer[id].val;
}
void dom_odometer_1_on_change(){
	dom_odometer_notify(ODOMETER_1);
}
void dom_odometer_2_on_change(){
	dom_odometer_notify(ODOMETER_2);
}
void dom_odometer_3_on_change(){
	dom_odometer_notify(ODOMETER_3);
}
void dom_odometer_4_on_change(){
	dom_odometer_notify(ODOMETER_4);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
		case GPIO_PIN_7:
			odometer_add(&odometer[ODOMETER_1], 1);
		break;
		case GPIO_PIN_8:
			odometer_add(&odometer[ODOMETER_2], 1);
		break;
		case GPIO_PIN_9:
			odometer_add(&odometer[ODOMETER_3], 1);
		break;
		case GPIO_PIN_10:
			odometer_add(&odometer[ODOMETER_4], 1);
		break;
		default: break;
	}
}
// ODOMETER FUNCTION END

// RELE FUNCTION START
void dom_rele_active(uint8_t id){
	rele_active(&rele[id]);
}
void dom_rele_inactive(uint8_t id){
	rele_inactive(&rele[id]);
}
uint8_t dom_rele_is_active(uint8_t id){
	return rele_is_active(&rele[id]);
}
// RELE FUNCTION END

// MOVE PARAMS START
uint16_t dom_pwm_break(){
	return dom_settings.pwm_break;
}
uint16_t dom_pwm_full(){
	return dom_settings.pwm_full;
}
uint16_t dom_pwm_accel(){
	return dom_settings.pwm_accel;
}
uint16_t dom_angle_break(){
	return dom_settings.angle_break;
}

uint8_t dom_move_params_set(uint16_t pwm_break, uint16_t pwm_full, uint16_t accel, uint16_t angle_break, float koef1, float koef2, uint8_t rain_inf){
	dom_settings.pwm_break = pwm_break;
	dom_settings.pwm_full = pwm_full;
	dom_settings.pwm_accel = accel;
	dom_settings.angle_break = angle_break;
	dom_settings.sensor_rain_settings[0].cmp_val = rain_inf;
	sensor_rain_set(&sensor_rain[0], -1, rain_inf);
	return settings_write(&dom_settings); // 0- ok 1- error
}
// MOVE PARAMS END












