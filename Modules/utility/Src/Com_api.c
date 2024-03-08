/*
 * Com_api.c
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */



#include "Com_api.h"


uint8_t buffer [64];
uint8_t flag_data_received = 0;


/**************************************************************************
 * @fn					- Com_Init
 *
 * @Note				- Init the Com Stack
 *
 **************************************************************************/
void Com_Init(){
	Buf_Init(&Com_TXstorage) ;
	Buf_Init(&Com_RXstorage) ;
	memset(buffer,'\0',64);
	MX_USB_DEVICE_Init();
}

/**************************************************************************
 * @fn					- Com_Transmit
 *
 * @Note				- Transmit Message
 *
 **************************************************************************/
void Com_Transmit(){
	Data_t* ptr = NULL;

	while(Buf_IsEmpty(&Com_TXstorage) != BUF_EMPTY){
		ptr = Buf_Dequeue(&Com_TXstorage);
		while(CDC_Transmit_FS((uint8_t*) ptr->data, strlen(ptr->data)+1) != USBD_OK);
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

	if (Buf_IsFull(&Com_RXstorage) != BUF_FULL && flag_data_received == 1 ){
		memcpy(ptr.data,buffer,MAX_SIZE_MESSAGE);
		flag_data_received = 0;
		Buf_Queue(&Com_RXstorage,&ptr);

	}
	else if (Buf_IsFull(&Com_RXstorage) == BUF_FULL && flag_data_received == 1){
				while(CDC_Transmit_FS((uint8_t*) "RX:Full\n", 8) != USBD_OK);
				flag_data_received = 0;
			}

}

/**************************************************************************
 * @fn					- Com_Assign
 *
 * @Note				- assign the message to the right buffer Job_stack/G_code_stack
 *
 **************************************************************************/
void Com_Assign(){
	Data_t* ptr = NULL;
	while(Buf_IsEmpty(&Com_RXstorage) != BUF_EMPTY && Buf_IsFull(&Job_Stack) != BUF_FULL){
			ptr = Buf_Dequeue(&Com_RXstorage);
			ptr->state = BUF_PENDING;
			Queue_Job(ptr);
		}
}
