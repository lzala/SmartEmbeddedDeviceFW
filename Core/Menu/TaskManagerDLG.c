/**
	******************************************************************************
	* @file    TaskManager.c
	* @author  Lucian Zala
	* @version V1.0.1
	* @date    01-03-2015
	* @brief   Task Manager program body
	******************************************************************************
*/

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)

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
static const GUI_WIDGET_CREATE_INFO _aDialogTaskManagerCreate[] = {
	{ FRAMEWIN_CreateIndirect, "TaskManager", ID_FRAMEWIN_0, 0, 0, 240, 295, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbTaskManagerDialog
*/
static void _cbTaskManagerDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hItem;
	int Id, NCode;

	switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			/* Initialization of 'TaskManager' */
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont (hItem, GUI_FONT_13HB_ASCII);
			FRAMEWIN_SetTextAlign (hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_AddCloseButton (hItem,FRAMEWIN_BUTTON_RIGHT,0);
			break;
		case WM_NOTIFY_PARENT:
			/* Id of widget */
			Id = WM_GetId (pMsg->hWinSrc);
			/* Notification code */
			NCode = pMsg->Data.v;
			switch (NCode) {
				case WM_NOTIFICATION_RELEASED:
					break;
			}
			break;
			case WM_DELETE:
				WM_NotifyParent (WM_GetParent(pMsg->hWin), 0x500);
				break;
			default:
				WM_DefaultProc (pMsg);
				break;
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/**
  * @brief  CreateTaskManager
  * @param  hWin: Window handle
  * @retval None
  */
void CreateTaskManager (WM_HWIN hWin);
void CreateTaskManager (WM_HWIN hWin) {
	GUI_CreateDialogBox (_aDialogTaskManagerCreate, GUI_COUNTOF (_aDialogTaskManagerCreate),
						 _cbTaskManagerDialog, hWin, 0, 0);
}
/*************************** End of file ****************************/
