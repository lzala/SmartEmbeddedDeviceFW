/**
  ******************************************************************************
  * @file    PaintDLG.c
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    01-03-2014
  * @brief   PAINT interface program body
  ******************************************************************************
  */

#include "DIALOG.h"
#include "global_includes.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NO_OF_PAINT_BUTTONS	12
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_SET_BLUE (GUI_ID_USER + 0x01)
#define ID_BUTTON_SET_GREEN (GUI_ID_USER + 0x02)
#define ID_BUTTON_SET_RED (GUI_ID_USER + 0x03)
#define ID_BUTTON_SET_MAGENTA (GUI_ID_USER + 0x04)
#define ID_BUTTON_SET_YELLOW (GUI_ID_USER + 0x05)
#define ID_BUTTON_SET_BLACK (GUI_ID_USER + 0x06)
#define ID_BUTTON_NEW (GUI_ID_USER + 0x07)
#define ID_BUTTON_7 (GUI_ID_USER + 0x08)
#define ID_BUTTON_8 (GUI_ID_USER + 0x09)
#define ID_BUTTON_9 (GUI_ID_USER + 0x0A)
#define ID_BUTTON_10 (GUI_ID_USER + 0x0B)
#define ID_BUTTON_11 (GUI_ID_USER + 0x0C)

/*********************************************************************
*
*       global  data
*
**********************************************************************
*/
WM_HWIN  vFrame;
WM_HWIN drawWin;
GUI_RECT drawAreaRect;
static uint8_t flg;
GUI_COLOR currentColor = GUI_BLUE;

/*********************************************************************
*
*       _aDialogPaintCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogPaintCreate[] = {
	{ FRAMEWIN_CreateIndirect, "PAINT", ID_FRAMEWIN_0, 0, 0, 240, 295, WM_CF_STAYONTOP, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_BLUE, 110, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_GREEN, 130, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_RED, 150, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_MAGENTA, 170, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_YELLOW, 190, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_SET_BLACK, 210, 0, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "New", ID_BUTTON_NEW, 0, 0, 40, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_7, 0, 110, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_8, 0, 130, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_9, 0, 150, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_10, 0, 170, 20, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_11, 0, 190, 20, 20, 0, 0x0, 0 },
};

/**Paint Button color Array*/
GUI_COLOR  PaintButtonsColor[NO_OF_PAINT_BUTTONS] = {GUI_BLUE,
													GUI_GREEN,
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

/**
  * @brief  Callback function of the image frame
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDrawWindow(WM_MESSAGE *pMsg) {
	static uint8_t xpos,ypos,sendDrawReq;
	const WM_PID_STATE_CHANGED_INFO * pInfo;
	drawWin = pMsg->hWin;

	switch (pMsg->MsgId) {
		case WM_CREATE:
			break;
		case WM_PAINT:
			WM_GetInsideRect(&drawAreaRect);
			GUI_SetBkColor(GUI_WHITE);
			if(!flg) {
				/* lock GUI clear */
				flg = 1;
				GUI_Clear();
				break;
			}
			GUI_SetColor(currentColor);
			GUI_SetPenSize(8);
			if(sendDrawReq) {
				sendDrawReq = 0; /*clear request*/
				GUI_DrawPoint(xpos,ypos);
			}
			break;
		case WM_INIT_DIALOG:
			break;
		case WM_MOTION:
			break;
		case WM_PID_STATE_CHANGED:
			break;
		case WM_TOUCH:
			pInfo = (const WM_PID_STATE_CHANGED_INFO *)pMsg->Data.p;
			xpos = pInfo->x;
			ypos = pInfo->y;
			sendDrawReq = 1;
			WM_InvalidateRect(drawWin, &drawAreaRect);
			break;
		case WM_DELETE:
			break;
		default:
			WM_DefaultProc(pMsg);
		}
}

/*********************************************************************
*
*       _cbPaintDialog
*/
static void _cbPaintDialog(WM_MESSAGE *pMsg) {
	WM_HWIN hItem,hClient;
	int NCode;
	int Id;
	int buttnInc;
	GUI_RECT r;

	switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_13B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetClientColor(hItem,GUI_LIGHTBLUE|0x7070);
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
			/*initialize buttons */
			for(buttnInc = 0; buttnInc < NO_OF_PAINT_BUTTONS; buttnInc++ ) {
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SET_BLUE + buttnInc);
				BUTTON_SetSkinClassic(hItem);
				BUTTON_SetBkColor(hItem,BUTTON_CI_PRESSED,PaintButtonsColor[buttnInc]);
				BUTTON_SetBkColor(hItem,BUTTON_CI_UNPRESSED,PaintButtonsColor[buttnInc]);
			}
			hClient = WM_GetClientWindow(pMsg->hWin);
			WM_GetClientRectEx(hClient, &r);
			vFrame = WM_CreateWindowAsChild(r.x0 +20, r.y0 + 22, r.x1 , r.y1 - 25,
											hClient, WM_CF_SHOW, _cbDrawWindow , 0);
			break;
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				case ID_BUTTON_SET_BLUE:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_SET_GREEN:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_SET_RED:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_SET_MAGENTA:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_SET_YELLOW:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_SET_BLACK:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							currentColor = BUTTON_GetBkColor(WM_GetDialogItem(pMsg->hWin, Id),
															 BUTTON_CI_PRESSED);
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_NEW:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							/* unlock GUI clear */
							flg = 0;
							/* request repaint (WM_PAINT) for drawWin */
							WM_InvalidateRect(drawWin, &drawAreaRect);
							break;
					}
					break;
				case ID_BUTTON_7:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_8:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_9:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_10:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_11:
					switch(NCode) {
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
			}
			if(NCode ==  WM_NOTIFICATION_CHILD_DELETED){
				WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
				flg = 0; /*unlock GUI clear*/
			}
			break;
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
	GUI_CreateDialogBox(_aDialogPaintCreate, GUI_COUNTOF(_aDialogPaintCreate),
						_cbPaintDialog, hWin, 0, 0);
}

/*************************** End of file ****************************/
