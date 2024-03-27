/*
 * cnc_2d.c
 *
 *  Created on: Mar 7, 2024
 *      Author: soufiane
 */

#include "cnc_2d.h"


Sys_t sys;
Step_Driver_Handler Motor1;
Step_Driver_Handler Motor2;
Servo_Handle_t servo;

TIM_HandleTypeDef* htim_servo;
TIM_HandleTypeDef* htim_step;



void cnc_init(TIM_HandleTypeDef* htim_servo_p,TIM_HandleTypeDef* htim_step_p){

	Motor1.Conf.ENB = 0;
	Motor1.Conf.MS1 = 0;
	Motor1.Conf.MS2 = 0;
	Motor1.Conf.MS3 = 0;
	Motor1.Conf.DIR = 0;
	Motor1.Status = ST_OFF;
	Motor1.tar_steps = 0;
	Motor1.Steps_count = 0;
	Motor1.num_steps = 0;

	Motor2.Conf.ENB = 0;
	Motor2.Conf.MS1 = 0;
	Motor2.Conf.MS2 = 0;
	Motor2.Conf.MS3 = 0;
	Motor2.Conf.DIR = 0;
	Motor2.Status = ST_OFF;
	Motor2.tar_steps = 0;
	Motor2.Steps_count = 0;
	Motor2.num_steps = 0;

	//Configured the pins for m1 & m2
	StM_Pin_Conf_v2(&Motor1,&Motor2);

	// initialize m1 & m2
	StM_Conf_Init(&Motor1);
	StM_Conf_Init(&Motor2);

	htim_servo = htim_servo_p;
	htim_step = htim_step_p;
	// init the servo motor , attach it to htim
	Servo_Init(&servo, htim_servo);

	sys.id_exe = 0;
	sys.status = SYS_STOP;

	G_Code_Init();
	Com_Init();
}


void cnc_DriveM_TIMcallback(Step_Driver_Handler* Motor){
	if (Motor->Status == ST_ORDER){
		  if (Motor->Steps_count == Motor->tar_steps){
				  Motor->Steps_count = 0;
				  Motor->tar_steps = 0;
				  Motor->Status = ST_OFF;
				  HAL_GPIO_WritePin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP,GPIO_PIN_RESET);
			  }else{
				STM_Step(Motor);
			  }

	}
}




void cnc_Step_X();
void cnc_Step_Y();






/******************************************************************************************************************************************************************************************************************************
 * This section is about sending information via USB
 *  Transmit Information about CNC
 */

/**************************************************************************
 * @fn					- SpyServoMotor
 *
 * @Note				- Send information the cnc hardware (motor 1&2 and Servo motor)
 *
 **************************************************************************/
void Spycnc(void){
	SpyServoMotor();
	SpyStepsMotor();
}


/**************************************************************************
 * @fn					- SpyServoMotor
 *
 * @Note				- Send to the buffer TX_COM information about Servo motor
 *
 **************************************************************************/
void SpyServoMotor(void){
	Data_t msg;
	memset(&msg.data, '\0', MESSAGE_SIZE);
	snprintf(msg.data,sizeof(msg.data),"<InfoServo>%d,%d,%lu,%lu,%lu\n",servo.Status,servo.curr_pos,servo.ptim->Instance->CCR1,servo.ptim->Init.Prescaler,servo.ptim->Init.Period);
	if (HAL_OK != Com_Queue_msg(&msg))
	{

	}
}

/**************************************************************************
 * @fn					- SpyStepsMotor
 *
 * @Note				- Send to the buffer TX_COM information about Steps motor 1 & 2
 *
 **************************************************************************/
void SpyStepsMotor(void){
	Data_t msg;
	memset(&msg.data, '\0', MESSAGE_SIZE);
	snprintf(msg.data,sizeof(msg.data),"<InfoM1>%d,%lu,%lu,%ld,%d,%d,%d,%d\n",Motor1.Status,Motor1.Steps_count,Motor1.tar_steps,Motor1.num_steps,Motor1.Conf.DIR,Motor1.Conf.MS1,Motor1.Conf.MS2,Motor1.Conf.MS3);
	if (HAL_OK != Com_Queue_msg(&msg))
	{

	}
	memset(&msg.data, '\0', MESSAGE_SIZE);
	snprintf(msg.data,sizeof(msg.data),"<InfoM2>%d,%lu,%lu,%ld,%d,%d,%d,%d\n",Motor2.Status,Motor2.Steps_count,Motor2.tar_steps,Motor2.num_steps,Motor2.Conf.DIR,Motor2.Conf.MS1,Motor2.Conf.MS2,Motor2.Conf.MS3);
	if (HAL_OK != Com_Queue_msg(&msg))
	{

	}

}

void SpyComErr(void){
	Data_t msg;
	snprintf(msg.data,sizeof(msg.data),"Com Error : %lu\n",errortosendqueue);
	if (HAL_OK != Com_Queue_msg(&msg))
	{

	}
}

void SpyGcode_Stack(void){
	// Spy the G code Queue Stack
}
/******************************************************************************************************************************************************************************************************************************
 * This section is about apply information received via USB
 *
 */
void ApplyConf(Data_t ptr){

	const char *token;

	// Tokenize the message by semicolon
	token = strtok(ptr.data, ":");
	token = strtok(NULL, ":");
	while (token != NULL) {
		if (strncmp(token, "M1", 2) == 0) {
				token = strtok(NULL, ":");
				Motor1.Conf.MS1 = token[0]- '0';
	            Motor1.Conf.MS2 = token[1]- '0';
	            Motor1.Conf.MS3 = token[2]- '0';
		} else if (strncmp(token, "M2", 2) == 0) {
			// Tokenize motor configuration for M2 by comma
			token = strtok(NULL, ":");
			Motor2.Conf.MS1 = token[0]- '0';
			Motor2.Conf.MS2 = token[1]- '0';
			Motor2.Conf.MS3 = token[2]- '0';
		}
		token = strtok(NULL, ":"); // Move to the next motor configuration
	}

	StM_Conf_Init(&Motor1);
	StM_Conf_Init(&Motor2);
}

void CmdMove(Data_t ptr){

	const char *token;

	// Tokenize the message by semicolon
	token = strtok(ptr.data, ":");
	token = strtok(NULL, ":");
	while (token != NULL) {
		if (strncmp(token, "M1", 2) == 0) {
				token = strtok(NULL, ":");
				GoToStep(&Motor1, atoi(token), 0);
				token = strtok(NULL, ":");
				Motor1.Conf.DIR = token[0]- '0';
		} else if (strncmp(token, "M2", 2) == 0) {
			// Tokenize motor configuration for M2 by comma
			token = strtok(NULL, ":");
			GoToStep(&Motor2, atoi(token), 0);
			token = strtok(NULL, ":");
			Motor2.Conf.DIR = token[0]- '0';
		}
		token = strtok(NULL, ":"); // Move to the next motor configuration
	}
	StM_Conf_Dir(&Motor1);
	StM_Conf_Dir(&Motor2);
	HAL_TIM_Base_Start_IT(htim_step);

}
