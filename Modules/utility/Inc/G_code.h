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


#define Queue_Job(x) Buf_Queue(&Job_Stack,x)
#define Queue_Gcode(x) Buf_Queue(&Gcode_Stack,x)



Buf_Handler_t Job_Stack;
Buf_Handler_t Gcode_Stack;


void G_Code_Init();
void G_Code_Execute_G();
void G_Code_Execute_J(Servo_Handle_t* servo);
uint8_t ParseCmd_servo(const char* cmd, uint8_t *angle );



#endif /* G_CODE_H_ */
