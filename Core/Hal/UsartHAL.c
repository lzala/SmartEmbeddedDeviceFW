/**
	******************************************************************************
	* @file    UartHAL.c
	* @author  Lucian Zala
	* @version V1.0.1
	* @date    03-03-2015
	* @brief   UART Hardware abstraction layer
	******************************************************************************
*/

#include "UsartHAL.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define UART_QUEUE_WAITING_TIME (100) /* 100 ms delay*/
#define UART_ERR_MSG (-1)

/**
  * @brief  Transmits single data through the USARTx peripheral using xQueueSend .
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param  UARTx_TXq  transmission queue handle.
  *         where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  data: the data to transmit.
  * @retval None
  */
int8_t uartPutChar(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, char data) {
	if (pdPASS != xQueueSend(UARTx_TXq, &data, UART_QUEUE_WAITING_TIME)) {
		/* Failed to post the message, even after 100 ticks.*/
		return UART_ERR_MSG;
	}
	/* enable the transmitter interrupt */
	USART_ITConfig(USARTx , USART_IT_TXE , ENABLE);
	return 0;
}

/**
  * @brief  Receive single data through the USARTx peripheral using xQueueReceive .
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral. (this parameter is defined only for future use)
  * @param  UARTx_RXq  receive queue handle.
  *         where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  data: the data to transmit.
  * @retval The received data
  */
char uartGetChar(USART_TypeDef* USARTx, xQueueHandle UARTx_RXq) {
	char buf;
	if (pdPASS != xQueueReceive(UARTx_RXq, &buf, UART_QUEUE_WAITING_TIME)) {
		/* Failed to receive the message, even after 100 ticks .*/
		return UART_ERR_MSG;
	}
	return buf;
}

/**
  * @brief  Transmits sting through the USARTx peripheral (End of string '\0').
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param  UARTx_TXq  transmission queue handle.
  *         Where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  Data: the data to transmit.
  * @retval error messages
  */
int8_t UsartSendString(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, uint8_t length,
						const char* Data) {
	/* Transmit Data string*/
	while (length) {
		/* end of string -- do not transmit */
		if ('\0' == *Data) {
			break;
		}
		if (UART_ERR_MSG == uartPutChar(USARTx, UARTx_TXq, *Data)) {
			return UART_ERR_MSG;
		}
		Data++;
		length--;
	}
	return 0;
}

/**
  * @brief  Writes bufferRx array with data received  by the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param  UARTx_RXq  receive queue handle.
  *         where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @retval error messages
  */
int8_t UsartReceiveString(USART_TypeDef* USARTx, xQueueHandle UARTx_RXq, uint8_t length,
							char* bufferRx) {
	uint16_t i = 0;
	/* Receive Data via Rx queue*/
	while (length) {
		if ((char)UART_ERR_MSG == (bufferRx[i] = uartGetChar(USARTx, UARTx_RXq))) {
			return UART_ERR_MSG;
		}
		/* end of string */
		if ('\n' == bufferRx[i]) {
			break;
		}
		i++;
		length--;
	}
	return 0;
}
