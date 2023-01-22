/*
 * fletcher.h
 *
 *  Created on: 21 янв. 2023 г.
 *      Author: Denis
 */

#ifndef FLETCHER_H_
#define FLETCHER_H_
#include "stdint.h"


void ks16(uint8_t * ptr, uint16_t len, uint8_t * pks1, uint8_t * pks2);
uint8_t ks16_check(uint8_t * ptr, uint16_t len);



#endif /* FLETCHER_H_ */
