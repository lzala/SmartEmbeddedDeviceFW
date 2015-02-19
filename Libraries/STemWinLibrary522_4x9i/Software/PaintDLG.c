/**
  ******************************************************************************
  * @file    PaintDLG.c
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    01-03-2014
  * @brief   PAINT interface program body
  ******************************************************************************
  */

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NO_OF_PAINT_BUTTONS	12
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x03)
#define ID_BUTTON_3 (GUI_ID_USER + 0x04)
#define ID_BUTTON_4 (GUI_ID_USER + 0x05)
#define ID_BUTTON_5 (GUI_ID_USER + 0x06)
#define ID_BUTTON_6 (GUI_ID_USER + 0x07)
#define ID_BUTTON_7 (GUI_ID_USER + 0x08)
#define ID_BUTTON_8 (GUI_ID_USER + 0x09)
#define ID_BUTTON_9 (GUI_ID_USER + 0x10)
#define ID_BUTTON_10 (GUI_ID_USER + 0x11)
#define ID_BUTTON_11 (GUI_ID_USER + 0x12)


// USER START (Optionally insert additional defines)
// USER END

//External data variables
extern GUI_PID_STATE TS_State;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "PAINT", ID_FRAMEWIN_0, 0, 0, 240, 295, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_0, 110, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_1, 130, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_2, 150, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_3, 170, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_4, 190, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_5, 210, 0, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "New", ID_BUTTON_6, 0, 0, 40, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_7, 0, 110, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_8, 0, 130, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_9, 0, 150, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_10, 0, 170, 20, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_11, 0, 190, 20, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/**Paint Button color Array*/
uint32_t PaintButtonsColor[NO_OF_PAINT_BUTTONS] = {	GUI_BLUE,GUI_GREEN,
													GUI_RED,
													GUI_MAGENTA,
													GUI_YELLOW,
													GUI_BLACK,
													GUI_WHITE,
													GUI_WHITE,
													GUI_WHITE,
													GUI_WHITE,
													GUI_WHITE,
													GUI_WHITE,
													};
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END
/**
  * @brief  Callback function of the image frame
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDrawWindow(WM_MESSAGE * pMsg) 
{
  GUI_RECT r;
  WM_HWIN hItem,hClient;
  static WM_HTIMER hTimerTime;  

  switch (pMsg->MsgId) 
  {

  case WM_INIT_DIALOG:

    break;

  case  WM_MOTION:

	  break;

  case  WM_PID_STATE_CHANGED:
//	GUI_DrawPoint(TS_State.x,TS_State.y);
    break;

  case  WM_TOUCH:
	//pointerStatus = pMsg->Data.p;
	GUI_SelectLayer(1);
	GUI_SetColor(GUI_RED);
	GUI_SetPenSize(8);

#if 0
    pState = (const GUI_PID_STATE *)pMsg->Data.p;
    if (pState) {
      if (pState->Pressed == 1)
      {
        IMAGE_Enlarge ^= 1;
        new_display = 1;
        if(IMAGE_Enlarge)
        {
          
          WM_AttachWindowAt(pMsg->hWin, WM_GetDesktopWindowEx(1), 0, 0);
          WM_SetSize(pMsg->hWin,LCD_GetXSize(), LCD_GetYSize());
        }
        else
        {
          WM_AttachWindowAt(pMsg->hWin, IMAGE_hWin, 25, 35);
          WM_SetSize(pMsg->hWin,267, 165);
        }
        WM_InvalidateWindow(pMsg->hWin);
      }
    }
#endif
    break;

  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 50, 0);
    break;
    
  case WM_NOTIFY_VIS_CHANGED:
    
    break;
    
  case WM_PAINT:
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
//	GUI_SetColor(GUI_RED);
//	GUI_FillCircle(TS_State.x,TS_State.y,3);
    WM_GetInsideRect(&r);
    if (!new_display)
    {    
		//add
    }
    GUI_ClearRectEx(&r);
		//add
    new_display = 0;
    break;
    
  case WM_TIMER:
    WM_RestartTimer(pMsg->Data.v, 0);
    break;

  case WM_NOTIFICATION_LOST_FOCUS:
    hItem = pMsg->hWin;
    WM_SetFocus(hItem);
   break;

  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }

 if(TS_State.Pressed && TS_State.y > 75 && TS_State.y < 290 && TS_State.x > 8 && TS_State.x < 232){
	GUI_DrawPoint(TS_State.x,TS_State.y);
	GUI_Exec();
 }
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     buttnInc;	
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PAINT'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetClientColor(hItem,GUI_LIGHTBLUE);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
	/*initialize buttons */
	for(buttnInc = 0; buttnInc < NO_OF_PAINT_BUTTONS; buttnInc++ ){
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0 + buttnInc);
		BUTTON_SetDefaultSkinClassic(hItem);
		BUTTON_SetBkColor(PaintButtonsColor(buttnInc));
	}
    hClient = WM_GetClientWindow(pMsg->hWin);
    WM_GetClientRectEx(hClient, &r);
    vFrame = WM_CreateWindowAsChild(r.x0 +20, r.y0 + 2, r.x1 , r.y1 - 25, hClient, WM_CF_SHOW, _cbDrawWindow , 0);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_5: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_6: // Notifications sent by 'New'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_7: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_8: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_9: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_10: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_11: // Notifications sent by ''
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
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
    break;
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
*       CreatePAINT
*/

/**
  * @brief  CreatePAINT
  * @param  hWin: Window handle
  * @retval None
  */
void CreatePAINT(WM_HWIN hWin);
void CreatePAINT(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialogPaintCreate, GUI_COUNTOF(_aDialogPaintCreate), _cbPaintDialog, hWin, 0, 0);
}
// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/