/*
 * settings.h
 *
 *  Created on: 21 янв. 2023 г.
 *      Author: Denis
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "main.h"
//  Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbytes
//  Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbytes
//  Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbytes
//  Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbytes
//  Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbytes
//  Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbytes
//  Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbytes
//  .
//  .
//  .
//  .
//  .
//  .
//  .
//  .
//  .
//  Sector 11 0x080E 0000 - 0x080F FFFF 128 Kbytes


typedef struct{
	uint16_t debounceTime;
}xButton_settings_tt;

typedef struct{
	uint16_t cmpVal;
}xSensor_settings_tt;

typedef struct{
	uint8_t enable;
	uint8_t cmp_val;
}xSensor_rain_settings_tt;

typedef struct{
	uint16_t deg_speed;
	uint8_t dir;
}xMotor_settings_tt;




uint16_t flash_write(void * data, uint32_t len);
void flash_read(void * data, uint32_t len);
void flash_erase();





#endif /* SETTINGS_H_ */
