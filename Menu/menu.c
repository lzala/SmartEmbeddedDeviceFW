/**
  ******************************************************************************
  * @file    menu.c
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    08-02-2014
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WM.h"
#include "..\ICON\ICON_paint.c"
#include "..\ICON\ICON_info.c"
#include "..\ICON\ICON_serial_port.c"
#include "..\ICON\ICON_task_manager.c"
#include "..\ICON\ICON_Bluetooth.c"
#include "..\ICON\ICON_BluetoothStatus.c"
#include "..\ICON\ICON_GPS.c"
#include "..\ICON\ICON_GPS_Status.c"
#include "cpu_utils.h"

/* External variables --------------------------------------------------------*/
extern void SystemInfo ( WM_HWIN hWin);
extern void CreatePAINT ( WM_HWIN hWin);
extern void CreateTaskManager ( WM_HWIN hWin);
extern void CreateUART_Settings ( WM_HWIN hWin);
extern void CreateGPS(WM_HWIN hWin);

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  const GUI_BITMAP * pBitmap;  
  const char       * pText;
  const char       * pExplanation;
} BITMAP_ITEM;

/* Private defines -----------------------------------------------------------*/
#define ID_TIMER_TIME                  1

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t current_module = 0xFF;
volatile uint32_t  BluetoothReady = pdFALSE, GPSReady = pdFALSE;

static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmpaint,  		 	"PAINT"      	 , "Launch Paint tool"},
  {&bmICON_Bluetooth,   "Bluetooth"    	 , "UART Settings"},
  {&bmICON_GPS,         "GPS"   		 , "GPS Information"},
  {&bmtask_manager,  	"Task Manager"   , "Get Task Information"},
  {&bminfo,          	"System Info"    , "Get System Information"},
};

static void (* _apModules[])( WM_HWIN hWin) = 
{
  CreatePAINT,
  CreateUART_Settings,
  CreateGPS,
  CreateTaskManager,
  SystemInfo,

};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback routine of desktop window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  uint32_t NCode, Id, sel;
  static uint32_t module_mutex = 0;

  switch (pMsg->MsgId)
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_TRANSPARENT);
    GUI_Clear();
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch (NCode)
    {
    case WM_NOTIFICATION_CLICKED:
      if (Id == '0')
      {
        sel = ICONVIEW_GetSel(pMsg->hWinSrc);
        if(sel < GUI_COUNTOF(_aBitmapItem))
        {
          if(module_mutex == 0)
          {
            module_mutex = 1;
            _apModules [sel](pMsg->hWinSrc);
            current_module = sel;
          }
        }
      }
      break;

    case 0x500:
      module_mutex = 0;
      current_module = 0xFF;
      ICONVIEW_SetSel(pMsg->hWinSrc, -1);
      break;

    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the status bar
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbStatus(WM_MESSAGE * pMsg) {
  int xSize, ySize;
  static uint8_t TempStr[50];

  static WM_HTIMER hTimerTime;
  WM_HWIN hWin;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {

  case WM_CREATE:
    hTimerTime = WM_CreateTimer(hWin, ID_TIMER_TIME, 1000, 0);
    break;
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
  case WM_TIMER:
    WM_InvalidateWindow(hWin);
    WM_RestartTimer(pMsg->Data.v, 0);
    break;

  case WM_PAINT:
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);

    /* Draw background */
    GUI_SetColor(0x303030);
    GUI_FillRect(0, 0, xSize , ySize - 3);
    GUI_SetColor(0x808080);
    GUI_DrawHLine(ySize - 2, 0, xSize );
    GUI_SetColor(0x404040);
    GUI_DrawHLine(ySize - 1, 0, xSize );

    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_13B_ASCII);

    /* Serial port*/
    if(BluetoothReady == pdTRUE)
    {
      GUI_DrawBitmap(&bmICON_BluetoothStatus, xSize - 155, 0);
    }

    if(GPSReady == pdTRUE)
    {
      GUI_DrawBitmap(&bmICON_GPS_Status, xSize - 125, 0);
    }

    sprintf((char *)TempStr, "CPU : %d %%", FreeRTOS_GetCPUUsage());

    if(FreeRTOS_GetCPUUsage() < 75 )
    {
      GUI_SetColor(GUI_WHITE);
    }
    else
    {
      GUI_SetColor(GUI_RED);
    }
    GUI_DispStringAt( (char *)TempStr, 4, 4);
    GUI_SetColor(GUI_WHITE);
    break;

  default:
    WM_DefaultProc(pMsg);
  }
}


/**
  * @brief  Main menu
  * @param  None
  * @retval None
  */
void MainMenu(void)
{
  ICONVIEW_Handle hIcon;
  WM_HWIN hStatusWin;
  int i = 0;

  WM_SetCallback(WM_GetDesktopWindowEx(1), _cbBk);
  
  /*create header window for status display e.g. CPU load, Bluetooth status , GPS status */
  hStatusWin = WM_CreateWindowAsChild(
                                      0,
                                      0,
                                      LCD_GetXSize(),
                                      20,
                                      WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS , _cbStatus, 0);
  
  hIcon = ICONVIEW_CreateEx(0, 20, LCD_GetXSize(), LCD_GetYSize()- 26, WM_GetDesktopWindowEx(1), WM_CF_SHOW | WM_CF_HASTRANS ,0 ,'0', 115, 95);
  
  ICONVIEW_SetFont(hIcon, &GUI_Font13B_ASCII);
  
  ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, 0x941000 | 0x80404040);
  
  ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, 3);
  
  ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 1);
  
  /*fill Icon view with available bitmaps*/
  for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++)
  {
    ICONVIEW_AddBitmapItem(hIcon,_aBitmapItem[i].pBitmap, _aBitmapItem[i].pText);
  }

  /*select layer with index 1*/
  GUI_SelectLayer(1);


}

/*************************** End of file ****************************/
