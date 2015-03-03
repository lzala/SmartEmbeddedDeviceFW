/**
  ******************************************************************************
  * @file    bsp.h
  * @author  Lucian Zala
  * @version V1.0.1
  * @date    03-03-2015
  * @brief   board support package routines
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t LowLevel_Init(void);
void BSP_Pointer_Update(void);
void init_USART1(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */
/*************************** End of file *****************************/
