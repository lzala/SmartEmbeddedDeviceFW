/**
  ******************************************************************************
  * @file    UART_SettingsDLG.c
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    09-02-2014
  * @brief   UART configuration program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "global_includes.h"
#include "DIALOG.h"

/* External functions --------------------------------------------------------*/
extern void init_USART1(void);

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmserial_port[];
extern volatile uint32_t BluetoothReady;
extern xTaskHandle BluetoothRx_Handle;

/* function prototypes */

uint8_t GetItemPos(uint32_t* array, uint32_t data);
int8_t uartPutChar (USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, char data);
char uartGetChar (USART_TypeDef* USARTx, xQueueHandle UARTx_TXq);
int8_t USART_SendString(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq,
						uint8_t length, const char* Data);
int8_t USART_ReceiveString(USART_TypeDef* USARTx, xQueueHandle UARTx_RXq,
						   uint8_t length, char* bufferRx);
static void delay(volatile uint32_t);

enum AT_COMMAND_IDs{
	AT_BAUDRATE = 'L',
	AT_STOPBITS = 'K',
	AT_PARITY   = 'M',
};

struct BTUARTConfig{
	uint8_t Parity;
	uint8_t BaudRate;
	uint8_t StopBits;

}currentBTUARTSettings,lastBTUARTSettings;


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_0 (GUI_ID_USER + 0x06)
#define ID_DROPDOWN_1 (GUI_ID_USER + 0x07)
#define ID_DROPDOWN_2 (GUI_ID_USER + 0x08)
#define ID_TEXT_0 (GUI_ID_USER + 0x09)
#define ID_TEXT_1 (GUI_ID_USER + 0x0A)
#define ID_TEXT_2 (GUI_ID_USER + 0x0B)
#define ID_DROPDOWN_3 (GUI_ID_USER + 0x0D)
#define ID_TEXT_3 (GUI_ID_USER + 0x0E)
#define ID_TEXT_4 (GUI_ID_USER + 0x0F)
#define ID_TEXT_5 (GUI_ID_USER + 0x10)
#define ID_IMAGE_INFO (GUI_ID_USER + 0x11)
#define ID_TIMER_BLUETOOTH_CONNECTION 0
#define UART_QUEUE_WAITING_TIME (100) /* 100 ms delay*/
#define UART_ERR_MSG (-1)
#define BTM222_ACK_MSG "OK\r\n"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aUARTCreate[] = {
	{ FRAMEWIN_CreateIndirect, "UART Settings", ID_FRAMEWIN_0, 0, 0, 240, 295, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "CONNECT", ID_BUTTON_0, 10, 140, 120, 40, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Parity", ID_DROPDOWN_0, 110, 80, 100, 50, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "DataBits", ID_DROPDOWN_1, 110, 50, 100, 30, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "BaudRate", ID_DROPDOWN_2, 110, 20, 100, 80, 0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "StopBits", ID_DROPDOWN_3, 110, 110, 100, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Bits per second:", ID_TEXT_0, 10, 20, 90, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Data Bits:", ID_TEXT_1, 11, 50, 90, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Parity:", ID_TEXT_2, 10, 80, 90, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Stop Bits:", ID_TEXT_3, 11, 110, 90, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Status:", ID_TEXT_4, 4, 256, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "DISCONNECTED!", ID_TEXT_5, 48, 256, 160, 20, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "Image", ID_IMAGE_INFO, 120, 120, 90, 90, 0, 0, 0 },
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

uint32_t BTbaudRateArray[6] = {4800,9600,19200,38400,57600,115200};
uint32_t BTparityArray[3] = {USART_Parity_No,USART_Parity_Even,USART_Parity_Odd};
uint32_t BTstopBitsArray[2] = {USART_StopBits_1,USART_StopBits_2};
uint32_t BTWordLengthArray[2] = { USART_WordLength_8b,USART_WordLength_9b};

xQueueHandle UART1_TXq;
xQueueHandle UART1_RXq;

char RxOverflow;

extern char bluetoothConnected;
USART_InitTypeDef BT_USART1_InitStructure;

/*********************************************************************
*
*       _cbUARTDialog
*/
static void _cbUARTDialog(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  int          NCode;
  int          Id;
  int
  static WM_HTIMER hTimerTime;
  WM_HWIN hWin;

  char rxBuffer[20] = {0};
  /*used when BTM 222 is configured via AT commands -- common settings for all commands*/
  char ATCommandArray[5] = {0};
  uint32_t configErr = 0;
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetTitleHeight(hItem, 0);
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
    FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);

    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
    IMAGE_SetBitmap(hItem, bmserial_port);
    //
    /* Initialization of 'CONNECT' */
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED,GUI_BLUE );
    //
    // Initialization of 'Parity'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
    DROPDOWN_SetFont(hItem, GUI_FONT_13B_ASCII);
    DROPDOWN_AddString(hItem, "None");
    DROPDOWN_AddString(hItem, "Even");
    DROPDOWN_AddString(hItem, "Odd");
    lastBTUARTSettings.Parity = GetItemPos(BTparityArray,BT_USART1_InitStructure.USART_Parity);
    DROPDOWN_SetSel(hItem,lastBTUARTSettings.Parity);

    //
    // Initialization of 'DataBits'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
    DROPDOWN_SetFont(hItem, GUI_FONT_13B_ASCII);
    DROPDOWN_AddString(hItem, "8");
    DROPDOWN_AddString(hItem, "9");
    DROPDOWN_SetSel(hItem,GetItemPos(BTWordLengthArray,BT_USART1_InitStructure.USART_WordLength));
    //
    // Initialization of 'BaudRate'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_2);
    DROPDOWN_SetFont(hItem, GUI_FONT_13B_ASCII);
    DROPDOWN_AddString(hItem, "4800");
    DROPDOWN_AddString(hItem, "9600");
    DROPDOWN_AddString(hItem, "19200");
    DROPDOWN_AddString(hItem, "38400");
    DROPDOWN_AddString(hItem, "57600");
    DROPDOWN_AddString(hItem, "115200");
    lastBTUARTSettings.BaudRate = GetItemPos(BTbaudRateArray,BT_USART1_InitStructure.USART_BaudRate);
    DROPDOWN_SetSel(hItem,lastBTUARTSettings.BaudRate);

    //
    // Initialization of 'StopBits'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_3);
    DROPDOWN_SetFont(hItem, GUI_FONT_13B_ASCII);
    DROPDOWN_AddString(hItem, "1");
    DROPDOWN_AddString(hItem, "2");
    lastBTUARTSettings.StopBits = GetItemPos(BTstopBitsArray,BT_USART1_InitStructure.USART_StopBits);
    DROPDOWN_SetSel(hItem,lastBTUARTSettings.StopBits);
    //
    // Initialization of 'Bits per second:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    //
    // Initialization of 'Data Bits:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    //
    // Initialization of 'Parity:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);

    //
    // Initialization of 'Stop Bits:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetTextColor(hItem, 0x00000000);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);

    //
    // Initialization of 'Status:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextColor(hItem, 0x00000000);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);

    //
    // Initialization of 'Status MSG :: CONNECTED!, DISCONNECTED!, ERROR:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetTextColor(hItem, GUI_RED);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);

	hTimerTime = WM_CreateTimer(WM_GetClientWindow(hWin), ID_TIMER_BLUETOOTH_CONNECTION, 100, 0);
    break;

  case WM_TIMER:
	  WM_RestartTimer(pMsg->Data.v, 100);
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	  if(bluetoothConnected == pdFALSE ){
		  BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED,GUI_RED);
	//	  BUTTON_SetText(hItem, "DISCONNECTED!");
	  }
	  else{
		  BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED,GUI_DARKGREEN);
		  BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), "CONNECTED!");
	  }

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'CONNECT'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
    	  if(bluetoothConnected != pdTRUE){
			  /*UART will be initialize with default settings in order to communicate with the bluetooth module*/
			  init_USART1();
			  USART_Init(USART1, &BT_USART1_InitStructure);
			  USART_Cmd(USART1, ENABLE); // enable USART1


			  /**BTM222 ACK the module should send OK as a message result*/
			  /*try to send string of data :: if the returned value is different than 0 an error occurred*/
			  if(USART_SendString(USART1,UART1_TXq, 4,"AT\r\0"))
				  configErr++;
			  /*try to receive string of data :: if the returned value is different than 0 an error occurred*/
			  if(USART_ReceiveString(USART1, UART1_RXq, sizeof(rxBuffer), rxBuffer))
				  configErr++;
			  else
				  configErr = strcmp(rxBuffer, BTM222_ACK_MSG);

			  /*BTM222 parity selection M0-M1 -- find in BTM 222 Datasheet AT commands table*/
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
			  currentBTUARTSettings.Parity = DROPDOWN_GetSel(hItem);
			  if( currentBTUARTSettings.Parity !=  lastBTUARTSettings.Parity){
				  lastBTUARTSettings.Parity = currentBTUARTSettings.Parity;
				  BT_USART1_InitStructure.USART_Parity = BTparityArray[currentBTUARTSettings.Parity];
				  /*try to send string of data :: if the returned data is different than 0 an error occurred*/
				  switch(currentBTUARTSettings.Parity){
					  case 0: if(USART_SendString(USART1, UART1_TXq, 6, "ATM0\r\0")) configErr++; break; /* no parity*/
					  case 1: if(USART_SendString(USART1, UART1_TXq, 6, "ATM2\r\0")) configErr++; break; /* Even parity setting. */
					  case 2: if(USART_SendString(USART1, UART1_TXq, 6, "ATM1\r\0")) configErr++; break; /* Odd parity setting. */
					  default: break;
				  }
				  /*try to receive string of data :: if the returned value is different than 0 an error occurred*/
				  if(USART_ReceiveString(USART1,UART1_RXq, sizeof(rxBuffer), rxBuffer))
					  configErr++;
				  else
					  configErr = strcmp(rxBuffer,BTM222_ACK_MSG);
				  USART_Init(USART1, &BT_USART1_InitStructure);
				  delay(100); /*this delay is mandatory for BTM222 changes to take place*/
			  }
			  /*BTM222 baud rate selection L0-L5 -- find in BTM 222 Datasheet AT commands table*/
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_2);
			  currentBTUARTSettings.BaudRate= DROPDOWN_GetSel(hItem);
			  if( currentBTUARTSettings.BaudRate !=  lastBTUARTSettings.BaudRate){
				  lastBTUARTSettings.BaudRate = currentBTUARTSettings.BaudRate;
				  BT_USART1_InitStructure.USART_BaudRate = BTbaudRateArray[currentBTUARTSettings.BaudRate];
				  sprintf(ATCommandArray,"AT%c%c\r", AT_BAUDRATE, currentBTUARTSettings.BaudRate + '0');
				  /*try to send string of data :: if the returned value is different than 0 an error occurred*/
				  if(USART_SendString(USART1,UART1_TXq, sizeof(ATCommandArray), ATCommandArray))
					  configErr++;
				  /*try to receive string of data :: if the returned value is different than 0 an error occurred*/
				  if(USART_ReceiveString(USART1,UART1_RXq, sizeof(rxBuffer), rxBuffer))
					  configErr++;
				  else
					  configErr = strcmp(rxBuffer, BTM222_ACK_MSG);

				  USART_Init(USART1, &BT_USART1_InitStructure);
				  delay(100); /*this delay is mandatory for BTM222 changes to take place*/
			  }

			  /*BTM222 stop bit selection K0-K1 -- find in BTM 222 Datasheet AT commands table*/
			  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_3);
			  currentBTUARTSettings.StopBits= DROPDOWN_GetSel(hItem);
			  if( currentBTUARTSettings.StopBits !=  lastBTUARTSettings.StopBits){
				  lastBTUARTSettings.StopBits = currentBTUARTSettings.StopBits;
				  BT_USART1_InitStructure.USART_StopBits = BTstopBitsArray[currentBTUARTSettings.StopBits];
				  sprintf(ATCommandArray,"AT%c%c\r", AT_STOPBITS, currentBTUARTSettings.StopBits + '0');
				  /*try to send string of data :: if the returned value is different than 0 an error occurred*/
				  if(USART_SendString(USART1,UART1_TXq, sizeof(ATCommandArray), ATCommandArray))
					  configErr++;
				  /*try to receive string of data :: if the returned value is different than 0 an error occurred*/
				  if(USART_ReceiveString(USART1,UART1_RXq, sizeof(rxBuffer), rxBuffer))
					  configErr++;
				  else
					  configErr = strcmp(rxBuffer, BTM222_ACK_MSG);
				  USART_Init(USART1, &BT_USART1_InitStructure);
				  delay(100); /*this delay is mandatory for BTM222 changes to take place*/
			  }

			  if(configErr){
				  BluetoothReady = pdFALSE;
			  }
			  else{
				  BluetoothReady = pdTRUE;
				  vTaskResume(BluetoothRx_Handle); /*resume BluetoothRx_Handle*/
			  }
			  WM_RestartTimer(pMsg->Data.v, 100);
    	  }
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;

      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_0: // Notifications sent by 'Parity'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_1: // Notifications sent by 'DataBits'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_2: // Notifications sent by 'BaudRate'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_DROPDOWN_3: // Notifications sent by 'StopBits'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;

      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    if(NCode ==  WM_NOTIFICATION_CHILD_DELETED)
      WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
    break;

  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateUART_Settings
*/
/**
  * @brief  CreateUART_Settings
  * @param  hWin: Window handle
  * @retval None
  */
void CreateUART_Settings(WM_HWIN hWin);
void CreateUART_Settings(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aUARTCreate, GUI_COUNTOF(_aUARTCreate), _cbUARTDialog, hWin, 0, 0);
}
// USER START (Optionally insert additional public code)
// USER END

/**
* @brief  GetItemPos
* @param  pointer to array
* @param  current data
* @retval position of baud rate in array
*/
uint8_t GetItemPos(uint32_t* array, uint32_t data){
	uint8_t i;
	for(i=0;i<6;i++){
		if(array[i] == data)
			break;
	}

	return i;
}


/**
  * @brief  Transmits single data through the USARTx peripheral using xQueueSend .
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param UARTx_TXq  transmission queue handle. where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  data: the data to transmit.
  * @retval None
  */

int8_t uartPutChar (USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, char data){
	if(xQueueSend (UARTx_TXq , &data, UART_QUEUE_WAITING_TIME ) !=  pdPASS){
		/* Failed to post the message, even after 100 ticks.*/
		return UART_ERR_MSG;
	}
	/*enable the transmitter interrupt*/
	USART_ITConfig (USARTx , USART_IT_TXE , ENABLE);
	return 0;
}
/**
  * @brief  Receive single data through the USARTx peripheral using xQueueReceive .
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral. (this parameter is defined only for future use -- no functionality so far)
  * @param UARTx_RXq  receive queue handle. where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  data: the data to transmit.
  * @retval The received data
  */
char uartGetChar (USART_TypeDef* USARTx, xQueueHandle UARTx_RXq){
	char buf;
	if(xQueueReceive (UARTx_RXq , &buf , UART_QUEUE_WAITING_TIME ) != pdPASS){
		/* Failed to receive the message, even after 100 ticks .*/
		return UART_ERR_MSG;
	}
	return buf;
}
/**
  * @brief  Transmits sting through the USARTx peripheral (End of string '\0').
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param UARTx_TXq  transmission queue handle. where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @param  Data: the data to transmit.
  * @retval error messages
  */
int8_t USART_SendString(USART_TypeDef* USARTx, xQueueHandle UARTx_TXq, uint8_t length, const char* Data){

  /* Transmit Data string*/
  while(length){
	  if(*Data == '\0') break; /*end of string -- do not transmit*/
	  if(uartPutChar(USARTx, UARTx_TXq, *Data) == UART_ERR_MSG){
		  return UART_ERR_MSG;
	  }
	  Data++  ;
	  length--;
  }
  return 0;
}

/**
  * @brief  Writes bufferRx array with data received  by the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
  *         UART peripheral.
  * @param UARTx_RXq  receive queue handle. where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART queue
  * @retval error messages
  */
int8_t USART_ReceiveString(USART_TypeDef* USARTx, xQueueHandle UARTx_RXq, uint8_t length, char* bufferRx){
  uint16_t i = 0;
  /* Receive Data via Rx queue*/
  while(length){

	  if((bufferRx[i] = uartGetChar(USARTx, UARTx_RXq)) == (char)UART_ERR_MSG){
		  return UART_ERR_MSG;
	  }
	  if(bufferRx[i] == '\n') break; /*end of string*/
	  i++  ;
	  length--;
  }
  return 0;
}


/**
 * @brief delay routine
 * param ncount delay value
 * return none
 */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for(index = (100000 * nCount); index != 0; index--){

  }
}
/*************************** End of file ****************************/
