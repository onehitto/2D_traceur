/*
 * Com_api.c
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */



#include "Com_api.h"



Buf_Handler_t Com_TXstorage;
QueueHandle_t queueUSBtoCom;
uint32_t errortosendqueue = 0;
uint32_t G_id = 0;

/**************************************************************************
 * @fn					- Com_Init
 *
 * @Note				- Init the Com Stack
 *
 **************************************************************************/
void Com_Init(){

	// init the Queue that transfer data from usb isr to the com
	queueUSBtoCom = xQueueCreate(5, 64 * sizeof(uint8_t));

	Buf_Init(&Com_TXstorage) ;
	MX_USB_DEVICE_Init();
}

/**************************************************************************
 * @fn					- Com_Transmit
 *
 * @Note				- Transmit Message
 *
 **************************************************************************/
void Com_Transmit(){
	Data_t ptr;

	while(Buf_IsEmpty(&Com_TXstorage) != BUF_EMPTY){
		if (HAL_OK == Buf_Dequeue(&Com_TXstorage,&ptr))
				while(CDC_Transmit_FS((uint8_t*) ptr.data, strlen(ptr.data)) != USBD_OK);
	}
	if (errortosendqueue != 0){
		SpyComErr();
		errortosendqueue = 0;
	}
}

/**************************************************************************
 * @fn					- Com_Queue_msg
 *
 * @Note				- store the message to the TX buffer
 *
 **************************************************************************/
HAL_StatusTypeDef Com_Queue_msg(Data_t * msg){

	if (HAL_OK == Buf_Queue(&Com_TXstorage, msg))
		return HAL_OK;
	else
		return HAL_ERROR;
}


/**************************************************************************
 * @fn					- Com_Receive
 *
 * @Note				- Store message received in the RXbuffer
 *
 **************************************************************************/
void Com_Receive(){
	Data_t ptr;

	while (pdPASS == xQueueReceive( queueUSBtoCom, &ptr.data, 0 )){
		// check if the msg is request of info
		if (strncmp((char*)ptr.data, "info", 4) == 0){
			Spycnc();
		}// check if the msg is a G code "Start with G"
		else if (strncmp((char*)ptr.data, "G", 1) == 0){
			ptr.id = G_id;
			ptr.state = PENDING ;
			if (1 == isGCodeLine(ptr.data)){
				if (HAL_OK == QUEUE_G_CODE(&ptr)){
								G_id++;
				}else
					Com_SendMsg("No More Space !!\n");
			}else
				Com_SendMsg("Not G_code !!\n");
		}// check if the msg is a execute code "Start with cmd"
		else if (strncmp((char*)ptr.data, "cmd:", 4) == 0){
			CmdMove(ptr);
		}// check if the msg is to apply a conf
		else if (strncmp((char*)ptr.data, "conf:", 5) == 0){
			ApplyConf(ptr);
		}else{

		}

	}

}

/**************************************************************************
 * @fn					- Com_Assign
 *
 * @Note				- assign the message to the right buffer Job_stack/G_code_stack
 *
 **************************************************************************/
void Com_SendMsg(char * msg){
	Data_t ptr;
	memcpy(ptr.data,msg,strlen(msg)+1);
	if (HAL_OK != Com_Queue_msg(&ptr))
	{

	}
}
