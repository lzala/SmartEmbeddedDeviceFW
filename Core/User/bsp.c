/**
  ******************************************************************************
  * @file    bsp.c
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    02-February-2014
  * @brief   board support package routines
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TS_Pressed;

/* Private function prototypes -----------------------------------------------*/
uint32_t BSP_TSC_Init(void);
void init_USART1(void);

extern USART_InitTypeDef BT_USART1_InitStructure;
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Inititialize the target hardware.
* @param  None
* @retval 0: if all initializations are OK.
*/
uint32_t LowLevel_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure GPIO PC1 to set L3GD20 Chip Select to Reset */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Deselect : Chip Select high */
  GPIO_SetBits(GPIOC, GPIO_Pin_1);
  
  /* Initialize the LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /*Init Touchscreen */
  BSP_TSC_Init();
  
  /* Initialize the SDRAM */
  SDRAM_Init();
  /*Initialize UART1 structure */
  BT_USART1_InitStructure.USART_BaudRate = 115200;
  BT_USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
  BT_USART1_InitStructure.USART_StopBits = USART_StopBits_1;
  BT_USART1_InitStructure.USART_Parity = USART_Parity_No;
  BT_USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  BT_USART1_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;


  /* Enable the CRC Module */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  
  return 0;
}



/**
  * @brief  Initializes the IO Expander registers.
  * @param  None
  * @retval 0: if all initializations are OK.
*/
uint32_t BSP_TSC_Init(void)
{
  __disable_irq();   
  IOE_Config();
  __enable_irq ();
  return 0; /* Configuration is OK */
}

/**
  * @brief  Read the TS data and update the GUI Touch structure
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;
  TP_STATE  *ts;

  portENTER_CRITICAL();
  ts = IOE_TP_GetState();
  portEXIT_CRITICAL();

  TS_State.x = ts->X;
  TS_State.y = ts->Y;

  /*update pointer position only if it was pressed*/
  TS_State.Pressed = (ts->TouchDetected == 0x80);
  TS_State.Layer = 1;
  GUI_TOUCH_StoreStateEx(&TS_State);

}


/**
* @brief  Inititialize USART1 hardware.
* @param  None
* @retval None
*/
void init_USART1(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure ;
	/* enable peripheral clock for USART1 Module */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* GPIOA Configuration:  USART1 TX on PA9 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIOA Configuration:  USART1 RX on PA10 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*safe priority settings*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_LOWEST_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init( &NVIC_InitStructure );
	/*enable reception interrupts*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

}
