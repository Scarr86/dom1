/*
 * rele.h
 *
 *  Created on: Jan 30, 2023
 *      Author: Denis
 */
#include "rele.h"

void rele_active(xRele_tt * rele){
	HAL_GPIO_WritePin(rele->GPIOx, rele->pin, GPIO_PIN_RESET);
}
void rele_inactive(xRele_tt * rele){
	HAL_GPIO_WritePin(rele->GPIOx, rele->pin, GPIO_PIN_SET);
}

uint8_t rele_is_active(xRele_tt * rele){
	return HAL_GPIO_ReadPin(rele->GPIOx, rele->pin) == GPIO_PIN_RESET;
}
