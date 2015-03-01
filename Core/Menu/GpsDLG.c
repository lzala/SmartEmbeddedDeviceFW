/**
	******************************************************************************
	* @file    GpsDLG.c
	* @author  Lucian Zala
	* @version V1.0.1
	* @date    01-03-2015
	* @brief   GPS Dialog Window program body
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
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_TEXT_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_1 (GUI_ID_USER + 0x04)
#define ID_TEXT_2 (GUI_ID_USER + 0x05)
#define ID_TEXT_3 (GUI_ID_USER + 0x06)
#define ID_TEXT_4 (GUI_ID_USER + 0x07)
#define ID_TEXT_5 (GUI_ID_USER + 0x08)
#define ID_TEXT_6 (GUI_ID_USER + 0x09)
#define ID_TEXT_7 (GUI_ID_USER + 0x0A)
#define ID_TEXT_8 (GUI_ID_USER + 0x0B)
#define ID_TEXT_9 (GUI_ID_USER + 0x0C)
#define ID_TEXT_10 (GUI_ID_USER + 0x0D)
#define ID_TEXT_11 (GUI_ID_USER + 0x0E)
#define ID_TEXT_12 (GUI_ID_USER + 0x0F)
#define ID_TEXT_13 (GUI_ID_USER + 0x10)
#define ID_TEXT_14 (GUI_ID_USER + 0x12)

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
static const GUI_WIDGET_CREATE_INFO _aGPSCreate[] = {
	{ FRAMEWIN_CreateIndirect, "GPS", ID_FRAMEWIN_0, 0, 0, 240, 295, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "START", ID_BUTTON_0, 9, 245, 80, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "GPS Streaming:", ID_TEXT_0, 10, 10, 100, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "No. of satellites:", ID_TEXT_1, 10, 30, 100, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "LAT:", ID_TEXT_2, 10, 50, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "LON:", ID_TEXT_3, 10, 70, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "ALT:", ID_TEXT_4, 10, 89, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TIME:", ID_TEXT_5, 10, 130, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "DATE:", ID_TEXT_6, 10, 150, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "SPEED:", ID_TEXT_7, 10, 110, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "0", ID_TEXT_8, 110, 30, 20, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_9, 60, 50, 60, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_10, 60, 70, 60, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_11, 60, 90, 60, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_12, 60, 110, 60, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_13, 60, 130, 60, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "NAN", ID_TEXT_14, 60, 150, 60, 20, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbDialog
*/
static void _cbGPSDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;

	switch (pMsg->MsgId) {
		/* Initialization of 'GPS' */
		hItem = pMsg->hWin;
		FRAMEWIN_SetFont (hItem, GUI_FONT_13B_ASCII);
		FRAMEWIN_SetTextAlign (hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		FRAMEWIN_AddCloseButton (hItem,FRAMEWIN_BUTTON_RIGHT,0);
		/* Initialization of 'GPS Streaming:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00FF0000);
		/* Initialization of 'No. of satellites:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_1);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		/* Initialization of 'LAT:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		/* Initialization of 'LON:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_3);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		/* Initialization of 'ALT:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_4);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		/* Initialization of 'TIME:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_5);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		/* Initialization of 'DATE:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_6);
		TEXT_SetTextColor (hItem, 0x00C08000);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		/* Initialization of 'SPEED:' */
		hItem = WM_GetDialogItem (pMsg->hWin, ID_TEXT_7);
		TEXT_SetFont (hItem, GUI_FONT_13B_ASCII);
		TEXT_SetTextColor (hItem, 0x00C08000);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId (pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id) {
			case ID_BUTTON_0:
				switch (NCode) {
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
				}
				break;
		}
		if (WM_NOTIFICATION_CHILD_DELETED == NCode) {
			WM_NotifyParent(WM_GetParent (pMsg->hWin), 0x500);
		}
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
/*********************************************************************
*
*       CreateGPS
*/
void CreateGPS(WM_HWIN hWin);
void CreateGPS(WM_HWIN hWin) {
	GUI_CreateDialogBox (_aGPSCreate, GUI_COUNTOF (_aGPSCreate), _cbGPSDialog, hWin, 0, 0);
}
/*************************** End of file ****************************/
