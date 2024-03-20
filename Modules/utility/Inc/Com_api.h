/*
 * Com_api.h
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */

#ifndef COM_API_H_
#define COM_API_H_

#include "stm32f1xx_hal.h"
#include <string.h>
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "cnc_2d.h"
#include "G_code.h"

/*
 * Calibration
 */
#define MAX_SIZE_MESSAGE 64
#define MAX_MESSAGE_STACK  10

/*
 * def
 */

extern Buf_Handler_t Com_TXstorage;
extern Buf_Handler_t Com_RXstorage;
extern QueueHandle_t queueUSBtoCom;
extern uint32_t errortosendqueue;
/*
 * Prototype fnc
 */
void Com_Init();
HAL_StatusTypeDef Com_Queue_msg(Data_t * msg);
void Com_Transmit();
void Com_Receive();
void Com_Assign();



/*
 * utility
 */



#endif /* COM_API_H_ */
