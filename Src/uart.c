/*
 * uart.c
 *
 *  Created on: Feb 25, 2023
 *      Author: Denis
 */
#include "uart.h"

#define UART_COUNT (4)
xUart_tt * px_uart[UART_COUNT];
uint8_t uart_count = 0;

uint8_t uart_request_poll_flag = 0;

void uart_notify(xUart_tt * uart);

void uart_notify(xUart_tt * uart){
	while(!FIFO_IS_EMPTY(uart->rx_fifo)){
		for(uint8_t i = 0; i < uart->observer_count; ++i){
			uart->observer[i](uart, &FIFO_TAIL(uart->rx_fifo), 1);
		}
		FIFO_POP(uart->rx_fifo);
	}
}

uint8_t uart_init(xUart_tt * uart){
	if(uart->pxhuart == NULL)
		return 1;
	if(uart_count < UART_COUNT){
		px_uart[uart_count] = uart;
		++uart_count;
		HAL_StatusTypeDef result = HAL_UART_Receive_IT(uart->pxhuart, &FIFO_HEAD(uart->rx_fifo), 1);
		return result;
	}
	return 1;
}


uint8_t uart_send(xUart_tt * uart, uint8_t * buf, uint16_t len){
	HAL_StatusTypeDef result;
	if(uart->tx_len + len >= UART_BUFFER_SIZE){
		return 1;
	}

	if(uart->pxhuart->gState == HAL_UART_STATE_READY){
		memcpy(&uart->tx_buf[0] + uart->tx_len, buf, len);
		uart->tx_len += len;
		result = HAL_UART_Transmit_IT(uart->pxhuart, uart->tx_buf, uart->tx_len);
		uart->tx_len = 0;
	}
	else{
		__NOP();
	}

	return result;
}
uint8_t uart_subscribe(xUart_tt * uart, uart_observer_tt obs){
	if(uart->observer_count < 4){
		uart->observer[uart->observer_count] = obs;
		++uart->observer_count;
		return 0;
	}
	return 1;
}
void uart_poll(){
	if(uart_request_poll_flag){
		uart_request_poll_flag = 0;
		for(uint8_t i = 0; i < UART_COUNT; ++i){
			if(px_uart[i] && !FIFO_IS_EMPTY(px_uart[i]->rx_fifo)){
				uart_notify(px_uart[i]);
			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();

	uart_request_poll_flag = 1;
	for(uint8_t i = 0; i < UART_COUNT; ++i){
		if(px_uart[i] && px_uart[i]->pxhuart == huart){

			if(!FIFO_IS_FULL(px_uart[i]->rx_fifo)){
				FIFO_INCREMENT(px_uart[i]->rx_fifo);
			}
			HAL_UART_Receive_IT(huart,&FIFO_HEAD(px_uart[i]->rx_fifo), 1);
		}
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();
	for(uint8_t i = 0; i < UART_COUNT; ++i){
		if(px_uart[i]->pxhuart == huart){
			if(px_uart[i]->tx_len){
				HAL_StatusTypeDef result = HAL_UART_Transmit_IT(px_uart[i]->pxhuart, px_uart[i]->tx_buf, px_uart[i]->tx_len);
				if(result == HAL_OK){
					px_uart[i]->tx_len = 0;
				}
			}
		}
	}
}



