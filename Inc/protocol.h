/*
 * protocol.h
 *
 *  Created on: Jan 31, 2023
 *      Author: Denis
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "dom.h"
#include "gate.h"
#include "uart.h"

typedef struct{
	void (*handler)(uint8_t * params, uint16_t len);
	char * cmd;
}xCmd_tt;


typedef uint8_t (*protocol_sender_tt)(uint8_t*, uint16_t);

void protocol_init(protocol_sender_tt protocol_sender);
void protocol_parser(uint8_t * buf, uint16_t len);
void protocol_parser_from_uart(xUart_tt * uart, uint8_t * buf, uint16_t len);
#endif /* PROTOCOL_H_ */
