/*
 * settings.c
 *
 *  Created on: 21 янв. 2023 г.
 *      Author: Denis
 */

#include "settings.h"
extern IWDG_HandleTypeDef hiwdg;

static uint32_t flash_address=0x08040000;
static uint16_t flash_write_byte(uint8_t byte, uint32_t address);
static uint8_t flash_read_byte(uint32_t address);

void flash_erase(){
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(6, FLASH_VOLTAGE_RANGE_3 );
	HAL_FLASH_Lock();
}
uint16_t flash_write(void * data, uint32_t len){
	uint8_t * d = (uint8_t*)data;
	uint16_t result = HAL_OK;
	flash_erase();

	HAL_FLASH_Unlock();
	for(uint32_t i = 0; i < len; ++i){
		HAL_IWDG_Refresh(&hiwdg);
		result = flash_write_byte(d[i], flash_address + i);
		if(result != HAL_OK){
			break;
		}

	}
	HAL_FLASH_Lock();
	return result;
}
uint16_t flash_write_byte(uint8_t byte, uint32_t address){
	uint16_t result;
	result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, byte);
	return result;
}

void flash_read(void * data, uint32_t len){
	uint8_t * d = (uint8_t*)data;
	HAL_FLASH_Unlock();
	for(uint32_t i = 0; i < len; ++i){
		d[i] = flash_read_byte(flash_address + i);
	}
	HAL_FLASH_Lock();
}
uint8_t flash_read_byte(uint32_t address){
	return *(__IO uint8_t*)address;
}
