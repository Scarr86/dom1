/*
 * button.h
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: Denis
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "main.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
	void (*on_click)();
}xButton_tt;


uint8_t btn_state(xButton_tt * btn);



#endif /* BUTTON_H_ */
