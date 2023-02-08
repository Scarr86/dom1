/*
 * rele.h
 *
 *  Created on: Jan 30, 2023
 *      Author: Denis
 */

#ifndef RELE_H_
#define RELE_H_

#include "main.h"

typedef struct{
	GPIO_TypeDef * GPIOx;
	uint32_t pin;
}xRele_tt;

void rele_active(xRele_tt * rele);
void rele_inactive(xRele_tt * rele);
uint8_t rele_is_active(xRele_tt * rele);



#endif /* RELE_H_ */
