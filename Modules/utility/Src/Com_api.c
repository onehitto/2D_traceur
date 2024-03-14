/*
 * Com_api.c
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */



#include "Com_api.h"




/**************************************************************************
 * @fn					- Com_Init
 *
 * @Note				- Init the Com Stack
 *
 **************************************************************************/
void Com_Init(){

	// init the Queue that transfer data from usb isr to the com
	queueUSBtoCom = xQueueCreate(10, 64 * sizeof(uint8_t));

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
	Data_t msg = {.data = "cmd test",.id = 0 , .state = 0};
	Data_t msg1 = {.data = "cmd G code",.id = 0 , .state = 0};
	Data_t msg2 = {.data = "cmd execute code",.id = 0 , .state = 0};
	//if (strncmp((char*)buffer, "up:01", 6) == 0)
	//	flag_data_received = 0;
	while (pdPASS == xQueueReceive( queueUSBtoCom, &ptr.data, 0 )){
		// check if the msg is request of info
		if (strncmp((char*)ptr.data, "cmd:info", 8) == 0){

			Com_Queue_msg(&msg);
		}// check if the msg is a G code "Start with G"
		else if (strncmp((char*)ptr.data, "G", 1) == 0){
			Com_Queue_msg(&msg1);
		}// check if the msg is a execute code "Start with cmd"
		else if (strncmp((char*)ptr.data, "cmd:move", 7) == 0){
			Com_Queue_msg(&msg2);
		}// check if the msg is to apply a conf
		else if (strncmp((char*)ptr.data, "conf:", 5) == 0){
			Com_Queue_msg(&msg2);
		}else{

		}

	}
	/*
	if (Buf_IsFull(&Com_RXstorage) != BUF_FULL && flag_data_received == 1 ){
		memcpy(ptr.data,buffer,MAX_SIZE_MESSAGE);
		flag_data_received = 0;
		Buf_Queue(&Com_RXstorage,&ptr);

	}
	else if (Buf_IsFull(&Com_RXstorage) == BUF_FULL && flag_data_received == 1){
				while(CDC_Transmit_FS((uint8_t*) "RX:Full\n", 8) != USBD_OK);
				flag_data_received = 0;
			}
	*/
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


