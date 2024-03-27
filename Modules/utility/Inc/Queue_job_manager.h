/*
 * Queue_job_manager.h
 *
 *  Created on: Sep 4, 2023
 *      Author: soufiane
 */

#ifndef QUEUE_JOB_MANAGER_H_
#define QUEUE_JOB_MANAGER_H_

#include "stm32f1xx_hal.h"
#include "string.h"
#include "cmsis_os.h"


#define BUF_SIZE 10
#define MESSAGE_SIZE 64
#define TIMEOUT_1MS pdMS_TO_TICKS(1)

typedef struct {
	char data[MESSAGE_SIZE];
	uint8_t state;
	uint16_t id;
}Data_t;
/*
 * Typedef
 */
typedef struct{
	Data_t data [BUF_SIZE];
	SemaphoreHandle_t mutex;
	int8_t front;
	int8_t rear;
}Buf_Handler_t;

typedef enum
{
  BUF_EMPTY       = 0x00U,
  BUF_FULL
}Buf_Status_t;


/*
 * Init the Buf
 */
void Buf_Init(Buf_Handler_t* buf);
/*
 * Queue Message to Buf
 */
HAL_StatusTypeDef Buf_Queue(Buf_Handler_t* buf,Data_t* ptr);
/*
 * DeQueue Message to Buf
 */
uint8_t Buf_Dequeue(Buf_Handler_t* buf,Data_t* rt);

/*
 * peek next item
 */
Data_t* Buf_Peek(Buf_Handler_t* buf);
/*
 * func to check if the Buf is full
 */
Buf_Status_t Buf_IsFull(Buf_Handler_t* buf);
/*
 * func to check if the Buf is empty
 */
Buf_Status_t Buf_IsEmpty(Buf_Handler_t* buf);
/*
 * func return the buf size
 */
uint8_t Buf_Size(Buf_Handler_t* buf);



#endif /* QUEUE_JOB_MANAGER_H_ */
