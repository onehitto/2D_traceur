/*
 * G_code.c
 *
 *  Created on: Sep 13, 2023
 *      Author: soufiane
 */

#include "G_code.h"


Buf_Handler_t Job_Stack;
Buf_Handler_t Gcode_Stack;



void G_Code_Init(){
	Buf_Init(&Job_Stack);
	Buf_Init(&Gcode_Stack);
}


void G_Code_Execute_J(Servo_Handle_t* servo){
	Data_t* ptr;
	uint8_t angle = 0;
	if (Buf_IsEmpty(&Job_Stack) != BUF_EMPTY){
		ptr = Buf_Peek(&Job_Stack);
		ptr->state = BUF_PENDING;
		/*
		if (1 == ParseCmd_servo(&ptr->data, &angle) ){
			Servo_Goto(servo, angle);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}*/
		Buf_Dequeue(&Job_Stack);
	}
}

uint8_t ParseCmd_servo(const char* cmd, uint8_t *angle ){

	if(strncmp(cmd, "<cmdservo>,", 11) == 0) {
	        // Extract the angle value after the comma
	        *angle = atoi(&cmd[11]);
	        return 1;  // Successfully parsed
	    }
	return 0 ;
}
