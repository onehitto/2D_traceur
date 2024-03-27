/*
 * G_code.h
 *
 *  Created on: Sep 13, 2023
 *      Author: soufiane
 */

#ifndef G_CODE_H_
#define G_CODE_H_


#include <stdlib.h>
#include "Queue_job_manager.h"
#include "SG90_controller.h"
#include "Com_api.h"
#include <math.h>

typedef enum
{
  RUNNING       = 0x00U,
  PENDING,
  STOPED
}G_Status_t;

typedef struct{
	float x,y,i,j;
}Pos_t;

#define QUEUE_G_CODE(X) Buf_Queue(&Gcode_Stack,X)
#define DEQUEUE_G_CODE(X) Buf_Dequeue(&Gcode_Stack,X)


extern Buf_Handler_t Gcode_Stack;


void G_Code_Init();


//parsers
void GCode_Parser(Data_t* G_Code);
void G1_Parser(Data_t* G_Code);

//utility
int isGCodeLine(const char* line);
int isdigit(int c);
float stringToFloat(const char *str);


#endif /* G_CODE_H_ */
