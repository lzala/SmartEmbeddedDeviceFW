/**
	******************************************************************************
	* @file    main.c
	* @author  Lucian Zala
	* @version V1.0.1
	* @date    30-01-2014
	* @brief   Main program body
	******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bsp.h"
#include "timers.h"
#include "queue.h"
#include "time_utils.h"
#include "cpu_utils.h"

#include "IconBkGround.h"
#include "IconBkLogo.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MAX_VALUE 180

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************/

/* Private define ------------------------------------------------------------*/
#define Background_Task_PRIO (tskIDLE_PRIORITY + 1)
#define Background_Task_STACK (512)

#define Draw_Task_PRIO (tskIDLE_PRIORITY + 2)
#define Draw_Task_STACK (1280)

#define BluetoothTx_Task_PRIO (tskIDLE_PRIORITY + 3)
#define BluetoothTx_Task_STACK (256)

#define BluetoothRx_Task_PRIO (tskIDLE_PRIORITY + 4)
#define BluetoothRx_Task_STACK (256)

#define ID_TIMER_TIME 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
xTaskHandle Task_Handle;
xTaskHandle Draw_Handle;
xTaskHandle BluetoothTx_Handle;
xTaskHandle BluetoothRx_Handle;
xTimerHandle TouchScreenTimer;
GUI_PID_STATE TS_State;
volatile uint32_t globalErrCnt;
xQueueHandle queue_handle = NULL;
uint32_t demo_mode = 0;
char bluetoothConnected = 0;
volatile uint32_t SPP_Error = 0;

/* Private function prototypes -----------------------------------------------*/
static void Background_Task (void* pvParameters);
static void Draw_Task (void* pvParameters);
static void BluetoothTx_Task (void* pvParameters);
static void BluetoothRx_Task (void* pvParameters);
static void vTimerCallback (xTimerHandle pxTimer);

extern volatile uint32_t BluetoothReady, GPSReady;
extern xQueueHandle UART1_TXq;
extern xQueueHandle UART1_RXq;
char RxOverflow;
extern void MainMenu();
extern int8_t USART_SendString (USART_TypeDef* USARTx, xQueueHandle UARTx_TXq,
							   uint8_t length, const char* Data);
extern int8_t USART_ReceiveString (USART_TypeDef* USARTx, xQueueHandle UARTx_RXq,
								  uint8_t length,char* bufferRx);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main (void) {
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_4);
	/* Setup SysTick Timer for 1 msec interrupts. */
	if (SysTick_Config (SystemCoreClock / 1000)) {
		/* Capture error */
		while (1);
	}

	BluetoothReady = pdFALSE;
	/*initialization for test only*/
	GPSReady = pdTRUE;
	/*reset global error counter*/
	globalErrCnt = 0;

	/* Create Touch Panel task */
	xTaskCreate (Background_Task,
				 (signed char const*)"BackgroundTask",
				 Background_Task_STACK,
				 NULL,
				 Background_Task_PRIO,
				 &Task_Handle);

	/* Create Draw task */
	xTaskCreate (Draw_Task,
				 (signed char const*)"DrawTASK",
				 Draw_Task_STACK,
				 NULL,
				 Draw_Task_PRIO,
				 &Draw_Handle);

	/* Create BluetoothlTx task */
	xTaskCreate (BluetoothTx_Task,
				 (signed char const*)"BluetoothTxTASK",
				 BluetoothTx_Task_STACK,
				 NULL,
				 BluetoothTx_Task_PRIO,
				 &BluetoothTx_Handle);

	/* Create BluetoothlRx task */
	xTaskCreate (BluetoothRx_Task,
				 (signed char const*)"BluetoothRxTASK",
				 BluetoothRx_Task_STACK,
				 NULL,
				 BluetoothRx_Task_PRIO,
				 &BluetoothRx_Handle);

	/* create corresponding queues for RX and Tx -- check USART1 IRQ */
	UART1_TXq =  xQueueCreate (1, sizeof(char));
	/* We want this queue to be viewable in a RTOS kernel aware debugger, so register it. */
	vQueueAddToRegistry (UART1_TXq, (signed char *)"BT_TxQueue");

	UART1_RXq =  xQueueCreate (20, sizeof(char));
	/* We want this queue to be viewable in a RTOS kernel aware debugger, so register it. */
	vQueueAddToRegistry (UART1_RXq, (signed char *)"BT_RxQueue");

	/* this communication tasks are not needed and will be suspended
	 * until Bluetooth module will be installed*/
	vTaskSuspend (BluetoothRx_Handle);
	vTaskSuspend (BluetoothTx_Handle);

	/* Start the FreeRTOS scheduler */
	vTaskStartScheduler ();
}

/**
  * @brief  Background task
  * @param  pvParameters not used
  * @retval None
  */
static void Background_Task (void* pvParameters) {
	uint32_t ticks = 0;

	/* Launch Touchscreen Timer 20 ms rate */
	TouchScreenTimer = xTimerCreate ((const signed char *)"Timer", 20, pdTRUE,
									 (void *)1, vTimerCallback);

	if( TouchScreenTimer != NULL ) {
		if (xTimerStart(TouchScreenTimer, 0) != pdPASS) {
		/* The timer could not be set into the Active state. */
		}
	}

	/* Run the Background task */
	while (1) {
		if(ticks++ > 10) {
			ticks = 0;
			/* toggle LED3 each 100ms */
			STM_EVAL_LEDToggle (LED3);
		}

		/* This task is handled periodically, each 10 ms */
		vTaskDelay (10);
	}
}

/**
  * @brief  Draw task
  * @param  pvParameters not used
  * @retval None
  */

static void Draw_Task (void* pvParameters) {
	LowLevel_Init ();
	GUI_Init ();

	/* Change Skin */
	PROGBAR_SetDefaultSkin (PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin (RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin (SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin (SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin (SPINBOX_SKIN_FLEX);
	BUTTON_SetDefaultSkin (BUTTON_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin (DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin (FRAMEWIN_SKIN_FLEX);
	/* Setup layer configuration during startup */
	GUI_SetBkColor (GUI_TRANSPARENT);
	GUI_SelectLayer (1);
	GUI_Clear ();
	GUI_SetBkColor (GUI_TRANSPARENT);
	GUI_SelectLayer (0);

	GUI_DrawBitmap (&bmBKground ,0 ,0);
	GUI_DrawBitmap (&bmfreertosLogo, LCD_GetXSize() - bmfreertosLogo.XSize,
					LCD_GetYSize() - bmfreertosLogo.YSize - 12);
	GUI_SetAlpha (0xE0);
	GUI_DrawBitmap (&bmSTM32_F4Logo, LCD_GetXSize() - bmSTM32_F4Logo.XSize,
					LCD_GetYSize() - bmSTM32_F4Logo.YSize - 40);

	GUI_SetAlpha (0x00);
	MainMenu ();

	GUI_CURSOR_Select (&GUI_CursorCrossS);
	GUI_CURSOR_Show ();

	while(1) {
		/* update the user interface -- redrawing*/
		GUI_Delay (10);
		/* This task is handled periodically, each 20 ms */
		vTaskDelay (20);
	}
}

/**
  * @brief  BluetoothTx_Task task
  * @param  pvParameters not used
  * @retval None
  */
static void BluetoothTx_Task (void* pvParameters) {
	/* This task is handled periodically, each 1000 ms */
	while(1) {
		if (USART_SendString(USART1, UART1_TXq, 10, "Alive!!!\r\n")) {
			SPP_Error++;
		}
		/* This task is handled periodically, each 1000 ms */
		vTaskDelay(1000);
	}
}

/**
  * @brief  BluetoothRx_Task task
  * @param  pvParameters not used
  * @retval None
  */
static void BluetoothRx_Task (void* pvParameters) {
	char rxBuffer[30];

	while(1) {
		/* clear rx buffer */
		memset (rxBuffer, '\0', sizeof(rxBuffer));
		if(USART_ReceiveString (USART1, UART1_RXq, sizeof(rxBuffer), rxBuffer)) {
			SPP_Error++;
		}
		/* CR and LF are expected when SPP service is started */
		if (!strcmp (rxBuffer,"\r\n")) {
			/* here bluetooth status is expected
			 * e.g. CONNECT  '001B-10-00018B' else DISCONNECT  '001B-10-00018B' */
			if (USART_ReceiveString(USART1, UART1_RXq, sizeof(rxBuffer), rxBuffer)) {
				SPP_Error++;
			}
			if(bluetoothConnected == pdFALSE) {
				/* clear 7'th position rx buffer because CONNECT
				 * is expected -- bluetooth address is not */
				rxBuffer[7] =  '\0';
				if (!strcmp (rxBuffer,"CONNECT")) {
					bluetoothConnected = pdTRUE;
					/* resume BluetoothTx_Handle :: bluetooth link successfully open */
					vTaskResume (BluetoothTx_Handle);
				}
			}
			else {
				/* clear 10'th position rx buffer because DISCONNECT is expected' */
				rxBuffer[10] = '\0';
				if (!strcmp (rxBuffer,"DISCONNECT")) {
					bluetoothConnected = pdFALSE;
					/* suspend BluetoothTx task */
					vTaskSuspend (BluetoothTx_Handle);
				}
			}
		}
		else {
			/*this needs to be defined*/
		}

		/* This task is handled periodically, each 50 ms */
		vTaskDelay (50);
	}
}

/**
  * @brief  Timer callback
  * @param  pxTimer
  * @retval None
  */
static void vTimerCallback (xTimerHandle pxTimer) {
	BSP_Pointer_Update ();
}

/**
  * @brief  Error callback function
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook ( void ) {
	while (1) {
	}
}
/*************************** End of file ****************************/
