/*
 * dom.h
 *
 *  Created on: Jan 13, 2023
 *      Author: Denis
 */

#ifndef DOM_H_
#define DOM_H_
#include "main.h"
#include "button.h"
#include "led-pwm.h"
#include "led.h"
#include "sensor.h"
#include "sensor-rain.h"
#include "motor.h"
#include "odometer.h"

#include "rele.h"

#include "settings.h"
#include "fletcher.h"

#define BUTTON_DEBOUNCE_TIME_DEF (100)
#define SENSOR_CMP_VAL_DEF (1)
#define MOTOR_SPEED_DEF (250)

typedef enum{
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_COUNT
}BUTTON_ENUM;

typedef enum{
	MOTOR_1,
	MOTOR_2,
	MOTOR_3,
	MOTOR_4,
	MOTOR_5,
	MOTOR_COUNT
}MOTOR_ENUM;

typedef enum{
	SENSOR_1 = 0,
	SENSOR_2,
	SENSOR_3,
	SENSOR_4,
	SENSOR_5,
	SENSOR_6,
	SENSOR_7,
	SENSOR_8,
	SENSOR_COUNT
}SENSOR_ENUM;

typedef enum{
	SENSOR_RAIN_1 = 0,
	SENSOR_RAIN_COUNT
}SENSOR_RAIN_ENUM;

typedef enum{
	ODOMETER_1,
	ODOMETER_2,
	ODOMETER_3,
	ODOMETER_4,
	ODOMETER_COUNT
}ODOMETER_ENUM;


typedef enum{
	LED_MOTOR_MOVE,
	LED_OPEN_CLOSE_GATE_1,
	LED_OPEN_CLOSE_GATE_2,
	LED_COUNT
}LED_ENUM;

typedef enum{
	RELE_1,
	RELE_2,
	RELE_3,
	RELE_COUNT
}RELE_ENUM;



typedef struct{
	xButton_settings_tt btn_settings[BUTTON_COUNT];
	xSensor_settings_tt sensor_settings[SENSOR_COUNT];
	xMotor_settings_tt motor_settings[MOTOR_COUNT];
}xDom_settings_tt;

typedef void (* btn_observers_fn)(uint8_t id);
typedef void (* sensor_observers_fn)(uint8_t id);
typedef void (* sensor_rain_observers_fn)(uint8_t id);
typedef void (* odometer_observers_fn)(uint8_t);

//переопределение ф-ции для внешнего прерывания(используется для одометра)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


void dom_init();
void dom_poll();
uint16_t settings_write(xDom_settings_tt * ds);
uint8_t settings_read(xDom_settings_tt * ds);

xButton_tt * get_button(BUTTON_ENUM id);
xSensor_tt * get_sensor(SENSOR_ENUM id);
xSensor_rain_tt * get_sensor_rain(SENSOR_RAIN_ENUM id);
xMotor_tt * get_motor(MOTOR_ENUM id);



// BUTTON START
int8_t dom_btn_set(uint8_t id, int16_t debounceTime);
int8_t dom_btn_state(uint8_t id);
int16_t dom_btn_debounce_time(uint8_t id);
uint8_t dom_btn_subscribe(btn_observers_fn obs);
uint8_t dom_btn_notify(uint8_t indx);
void dom_btn_1_on_click();
void dom_btn_2_on_click();
void dom_btn_3_on_click();
void dom_btn_4_on_click();
// BUTTON END

// LED START
void dom_led_on(uint8_t id);
void dom_led_off(uint8_t id);
// LED END

// LED-PWM START
uint8_t dom_led_pwm_set(uint8_t on, uint8_t mode, uint8_t frq);
uint8_t dom_led_pwm_mode();
uint8_t dom_led_pwm_state();
uint16_t dom_led_pwm_frq();
// LED-PWM END


// SENSOR START
uint8_t dom_sensor_set(uint8_t id, uint8_t cmpVal);
uint8_t dom_sensor_subscribe(sensor_observers_fn obs);
uint8_t dom_sensor_notify(uint8_t indx);
int8_t dom_sensor_state(uint8_t id);
int8_t dom_sensor_cmp_val(uint8_t id);
void dom_sensor_1_on_detected();
void dom_sensor_2_on_detected();
void dom_sensor_3_on_detected();
void dom_sensor_4_on_detected();
void dom_sensor_5_on_detected();
void dom_sensor_6_on_detected();
void dom_sensor_7_on_detected();
void dom_sensor_8_on_detected();
// SENSOR END

// SENSOR-RAIN START
uint8_t dom_sensor_rain_subscribe(sensor_rain_observers_fn obs);
uint8_t dom_sensor_rain_notify(uint8_t indx);
int8_t dom_sensor_rain_state(uint8_t id);
void dom_sensor_rain_1_on_change();
// SENSOR-RAIN END


// MOTOR START
int8_t dom_motor_state(uint8_t id);
int16_t dom_motor_speed(uint8_t id);
int8_t dom_motor_dir(uint8_t id);
uint8_t dom_motor_set(uint8_t id, uint16_t speed);
void dom_motor_forward(uint8_t id);
void dom_motor_back(uint8_t id);
void dom_motor_stop(uint8_t id);
// MOTOR END

// ODOMETER START
uint8_t dom_odometer_subscribe(odometer_observers_fn obs);
uint8_t dom_odometer_notify(uint8_t id);
xOdometer_tt * dom_odometer(uint8_t id);
uint32_t dom_odometer_value(uint8_t id);
void dom_odometer_1_on_change();
void dom_odometer_2_on_change();
void dom_odometer_3_on_change();
void dom_odometer_4_on_change();
// ODOMETER START

// RELE START
void dom_rele_active(uint8_t id);
void dom_rele_inactive(uint8_t id);
// RELE END







#endif /* DOM_H_ */
