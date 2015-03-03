/**
	******************************************************************************
	* @file    UartHAL.h
	* @author  Lucian Zala
	* @version V1.0.1
	* @date    03-03-2015
	* @brief   UART Hardware abstraction layer
	******************************************************************************
*/

#ifndef USARTHAL_H_
#define USARTHAL_H_

#include "global_includes.h"

int8_t uartPutChar(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, char data);
char uartGetChar(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq);
int8_t UsartSendString(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq,
					   uint8_t length, const char* Data);
int8_t UsartReceiveString(USART_TypeDef* USARTx, xQueueHandle UARTx_RXq,
						  uint8_t length, char* bufferRx);

#endif /* USARTHAL_H_ */
