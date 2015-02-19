/**
  ******************************************************************************
  * @file    demo_systeminfo.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Demo system information
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
#include "DIALOG.h"

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bminfo[];

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO 		(GUI_ID_USER + 0x02)
#define ID_IMAGE_INFO   	    (GUI_ID_USER + 0x03)
#define ID_TEXT_BOARD    	    (GUI_ID_USER + 0x04)
#define ID_TEXT_CORE    	    (GUI_ID_USER + 0x05)
#define ID_TEXT_CPU    		    (GUI_ID_USER + 0x06)
#define ID_TEXT_OWNER			(GUI_ID_USER + 0x07)
#define ID_TEXT_VERSION			(GUI_ID_USER + 0x08)
#define ID_BUTTON_INFO_CLOSE    (GUI_ID_USER + 0x09)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Dialog resource using a WINDOW widget */
static const GUI_WIDGET_CREATE_INFO _aDialogInfoCreate[] = {
  { FRAMEWIN_CreateIndirect, "System Information", ID_FRAMEWIN_INFO, 1, 0, 240, 295, WM_CF_STAYONTOP, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_INFO, 75, 6, 90, 90, 0, 0, 0 },  
  { TEXT_CreateIndirect, "Board : STM32F429I-DISCO", ID_TEXT_BOARD, 5, 120, 179, 23, 0, 0, 0 },
  { TEXT_CreateIndirect, "Core: CortexM4F", ID_TEXT_CORE, 5, 140, 226, 30, 0, 0, 0 },
  { TEXT_CreateIndirect, "CPU Speed : 168MHz", ID_TEXT_CPU, 5, 160, 238, 22, 0, 0, 0 },
  { TEXT_CreateIndirect, "Created by : Lucian Zala", ID_TEXT_OWNER, 5, 180, 238, 22, 0, 0, 0 },
  { TEXT_CreateIndirect, "Firmware : v0.1 02/2014", ID_TEXT_VERSION, 5, 200, 238, 22, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_BUTTON_INFO_CLOSE, 80, 240, 80, 32, 0, 0, 0 },  
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbInfoDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    /* Initialization of 'System Information' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13HB_ASCII);
    FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    
    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
    IMAGE_SetBitmap(hItem, bminfo);
    
    /* Initialization of 'Board : STM324x9I' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);
    
    /* Initialization of 'Core: STM32F-4 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);
    
    /* Initialization of 'CPU Speed : 168MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);
    
    /* Initialization of 'Created by : Lucian Zala' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_OWNER);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);

    /* Initialization of 'Firmware : v0.1 02/2014' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);
    /* Initialization of 'Close' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO_CLOSE);
    BUTTON_SetFont(hItem, GUI_FONT_13HB_ASCII);
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_INFO_CLOSE:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;

    }
    if(NCode ==  WM_NOTIFICATION_CHILD_DELETED)
       WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
       break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  SystemInfo
  * @param  hWin: Window handle
  * @retval None
  */
void SystemInfo(WM_HWIN hWin);
void SystemInfo(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialogInfoCreate, GUI_COUNTOF(_aDialogInfoCreate), _cbInfoDialog, hWin, 0, 0);
}
/*************************** End of file ****************************/
