/*
 * fletcher.c
 *
 *  Created on: 21 янв. 2023 г.
 *      Author: Denis
 */

#include "fletcher.h"


void ks16(uint8_t * ptr, uint16_t len, uint8_t * pks1, uint8_t * pks2){
	uint8_t f0 = 1;
	uint8_t f1 = 0;

	while(len--){
		f1 += f0 += *ptr++;
	}
	*pks1 = 0 - f0 - f1;
	*pks2 = f1;
}

uint8_t ks16_check(uint8_t * ptr, uint16_t len){
	uint8_t f0 = 1;
	uint8_t f1 = 0;
	if(len > 2){
		while(len--){
			f1 += f0 += *ptr++;
		}
	}
	if(f0 == 0 && f1 == 0)
		return 1; // packet true
	return 0; // packet false
}
