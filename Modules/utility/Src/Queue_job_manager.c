/*
 * Queue_job_manager.c
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */

#include "Queue_job_manager.h"

/*
 * Buf_Init
 */
/**************************************************************************
 * @fn					- Buf_Init
 *
 * @brief				- Initialize the BUF hanlder
 * @param[in]			- Buf_Handler_t
 *
 * @return				- None
 *
 * @Note				- None
 *
 **************************************************************************/
void Buf_Init(Buf_Handler_t* buf){
	for (int index = 0;index<BUF_SIZE;index++)
		memset(buf->data[buf->rear].data,'\0',MESSAGE_SIZE);
	buf->front = -1;
	buf->rear = -1;
	buf->mutex = xSemaphoreCreateMutex();
	if (buf->mutex == NULL){

	}
}
/*
 * Queue Message to Stack
 */
/**************************************************************************
 * @fn					- Buf_Queue
 *
 * @brief				- Queue Message to Stack
 * @param[in]			- Buf_Handler_t
 * 						- Data_t
 *
 * @return				- HAL_StatusTypeDef
 *
 * @Note				- None
 *
 **************************************************************************/
HAL_StatusTypeDef Buf_Queue(Buf_Handler_t* buf,Data_t* ptr){

	xSemaphoreTake(buf->mutex,TIMEOUT_1MS);
	if (Buf_IsFull(buf) == BUF_FULL ){
		xSemaphoreGive(buf->mutex);
		return HAL_ERROR;
	}
	if (Buf_IsEmpty(buf) == BUF_EMPTY ){
		buf->front = 0;
		buf->rear = 0;
	}else{
		buf->front = (buf->front+1) % BUF_SIZE ;
	}
	memcpy(&buf->data[buf->front],ptr,sizeof(Data_t));
	xSemaphoreGive(buf->mutex);
	return HAL_OK;
}

/*
 * DeQueue Message to Buf
 */
/**************************************************************************
 * @fn					- Buf_Dequeue
 *
 * @Note				- None
 *
 **************************************************************************/
Data_t* Buf_Dequeue(Buf_Handler_t* buf){
	xSemaphoreTake(buf->mutex,TIMEOUT_1MS);
	if (Buf_IsEmpty(buf) == BUF_EMPTY ){
		xSemaphoreGive(buf->mutex);
		return NULL;
	}
	static Data_t rt;
	memcpy(&rt,&(buf->data[buf->rear]),sizeof(Data_t));
	if (buf->front == buf->rear){
		buf->front = buf->rear = -1;
	}else {
		buf->rear = (buf->rear + 1) % BUF_SIZE;
	}
	xSemaphoreGive(buf->mutex);
	return &rt ;
}
/*
 * Buf_IsFull
 */
/**************************************************************************
 * @fn					- Buf_IsFull
 *
 * @Note				- func to check if the Buf is full
 *
 **************************************************************************/
Buf_Status_t Buf_IsFull(Buf_Handler_t* buf){
	return ((buf->front+1) % BUF_SIZE) == buf->rear;
}

/*
 * Buf_IsEmpty
 */
/**************************************************************************
 * @fn					- Buf_IsEmpty
 *
 * @Note				- func to check if the Buf is empty
 *
 **************************************************************************/
Buf_Status_t Buf_IsEmpty(Buf_Handler_t* buf){
	return !(buf->front == -1);
}

/*
 * Buf_Size
 */
/**************************************************************************
 * @fn					- Buf_Size
 *
 * @Note				- return the size of the buf
 *
 **************************************************************************/
uint8_t Buf_Size(Buf_Handler_t* buf){
	uint8_t size;
	xSemaphoreTake(buf->mutex,TIMEOUT_1MS);
	if (buf->front == -1 )
		size = 0;
	else if (buf->rear >= buf->front )
		size = (BUF_SIZE - buf->rear) - buf->front +1;
	else{
		size = buf->front - buf->rear + 1;
	}
	xSemaphoreGive(buf->mutex);
	return size;
}

/*
 * Buf_Peek
 */
/**************************************************************************
 * @fn					- Buf_Peek
 *
 * @Note				- func return the next item without dequeue
 *
 **************************************************************************/
Data_t* Buf_Peek(Buf_Handler_t* buf){
	xSemaphoreTake(buf->mutex,TIMEOUT_1MS);
		if (Buf_IsEmpty(buf) == BUF_EMPTY ){
			xSemaphoreGive(buf->mutex);
			return NULL;
		}else{
			xSemaphoreGive(buf->mutex);
			return &(buf->data[buf->rear]);
		}

}
