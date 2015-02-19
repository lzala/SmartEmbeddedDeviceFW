/**
  ******************************************************************************
  * @file    stm32xxx_it.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Exceptions Handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"

#include "test_program.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "usb_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TS_Pressed;
extern uint32_t demo_mode;


__IO uint32_t ButtonPressed = 0x00;

extern xQueueHandle UART1_RXq;
extern xQueueHandle UART1_TXq;
extern uint8_t RxOverflow;
uint16_t tmpCC4[2] = {0, 0};
/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler( void );
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

  xPortSysTickHandler();

}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{ 
  ButtonPressed = 0x01;
  
  EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
}

/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler ( void ){

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if ( USART_GetITStatus (USART1 , USART_IT_RXNE ) != RESET){
		uint8_t data;
		USART_ClearITPendingBit (USART1 , USART_IT_RXNE );
		data = USART_ReceiveData (USART1) & 0xff;
	if ( xQueueSendFromISR (UART1_RXq , &data ,& xHigherPriorityTaskWoken ) != pdTRUE)
		RxOverflow = 1;
	}
	if ( USART_GetITStatus (USART1 , USART_IT_TXE ) != RESET) {
		uint8_t data;
		if ( xQueueReceiveFromISR (UART1_TXq , &data ,& xHigherPriorityTaskWoken ) == pdTRUE){
			USART_SendData (USART1 , data);
		}
		else {
			// turn off interrupt
			USART_ITConfig (USART1 , USART_IT_TXE , DISABLE);
		}
	}
	// Cause a scheduling operation if necessary
	portEND_SWITCHING_ISR ( xHigherPriorityTaskWoken );
}

/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */

void TIM2_IRQHandler(void)
{

}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{

}

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{

}

/**
  * @brief  This function handles DMA2 Stream1 global interrupt request.
  * @param  None
  * @retval None
  */

void DMA2_Stream1_IRQHandler(void)
{
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  LTDC_ISR_Handler();
}

/**
  * @brief  This function handles DMA2D global interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  DMA2D_ISR_Handler();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
