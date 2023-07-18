/*
 * uart.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Denis
 */

#ifndef UART_H_
#define UART_H_
#include "main.h"
#include "fifo.h"
#define UART_BUFFER_FIFO_SIZE (128)
#define UART_BUFFER_SIZE (1024)

struct xUart;
typedef void (*uart_observer_tt)(struct xUart * uart, uint8_t * buf, uint16_t len);

typedef void (*uart_rede485_tt)(uint8_t);

typedef struct xUart{
	uint8_t need_poll;
	UART_HandleTypeDef * pxhuart;
	//FIFO(UART_BUFFER_SIZE) tx_fifo;
	FIFO(UART_BUFFER_SIZE) rx_fifo;
 	//uint8_t tx_buf[UART_BUFFER_SIZE];
	//uint16_t tx_len;
	uart_observer_tt observer[4];
	uint8_t observer_count;

	uart_rede485_tt toggleReDe;

}xUart_tt;






uint8_t uart_init(xUart_tt * uart);


uint8_t uart_send(xUart_tt * uart, uint8_t * buf, uint16_t len);
uint8_t uart_subscribe(xUart_tt * uart, uart_observer_tt obs);
void uart_poll();








#endif /* UART_H_ */
