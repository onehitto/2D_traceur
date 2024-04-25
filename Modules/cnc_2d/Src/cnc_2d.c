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

	Com_Init();
	G_Code_Init();
	memset(&sys,0,sizeof(Sys_t));

	sys.status = SYS_STOP;
}


void cnc_DriveM_TIMcallback(Step_Driver_Handler* Motor){
	if (Motor->Status == ST_ORDER){
		  if (Motor->Steps_count == Motor->tar_steps){
				  Motor->Steps_count = 0;
				  Motor->tar_steps = 0;
				  Motor->Status = ST_OFF;
				  sys.Move = STOP;
				  HAL_GPIO_WritePin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP,GPIO_PIN_RESET);
			  }else{
				STM_Step_High(Motor);
			  }

	}
}




void move_line_callback(){

	//count the step needed
	float x = gc_state.coord_sys[X_AXIS];
	float y = gc_state.coord_sys[Y_AXIS];
	float x1 = gc_block.values.xyz[X_AXIS];
	float y1 = gc_block.values.xyz[Y_AXIS];
	float m = sys.m;
	float b = sys.b;
	float cal_save = sys.cal_save;
	if (fabs(x-x1)< RESOLUTION_MN_XY && fabs(y-y1)<RESOLUTION_MN_XY ){
		sys.Move = STOP;
		gc_state.coord_sys[X_AXIS] = x1;
		gc_state.coord_sys[Y_AXIS] = y1;

	}else{
		if (sys.xInc == 0 ){
			// movement y+
			gc_state.coord_sys[Y_AXIS] += sys.yInc;

			STM_Step_High(&Motor1);
			STM_Step_High(&Motor2);
		}else if (sys.yInc == 0){
			// movement x+
			gc_state.coord_sys[X_AXIS] += sys.xInc;

			STM_Step_High(&Motor1);
			STM_Step_High(&Motor2);
		}else{
			// the distance between the point x,y and the line is fabs( m * x - y + b) / sqrt(m*m + 1)
			// x + 0.016 , y
			float dist_x = fabs( m * (x + sys.xInc) - y + b) * cal_save;
			// x ,  y + 0.016
			float dist_y = fabs( m * x  - (y + sys.yInc) + b) * cal_save;
			// x+ 0.012 ,  y + 0.012
			float dist_diag = fabs( m * (x + sys.yInc_d) - (y + sys.yInc_d) + b) * cal_save;

			if (dist_diag < dist_x && dist_diag < dist_y){

				if((sys.xInc_d > 0 && sys.yInc_d > 0 )||(sys.xInc_d < 0 && sys.yInc_d < 0)){
					//M1 ++/--
					Motor1.Conf.DIR = (sys.xInc_d > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;

					StM_Conf_Dir(&Motor1);
					STM_Step_High(&Motor1);
				}else{
					//M2 ++ /--
					Motor2.Conf.DIR = (sys.xInc_d > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
					StM_Conf_Dir(&Motor2);
					STM_Step_High(&Motor2);
				}
				// update coordinate
				gc_state.coord_sys[X_AXIS] += sys.xInc_d;
				gc_state.coord_sys[Y_AXIS] += sys.yInc_d;

			}else if (dist_x < dist_diag && dist_x < dist_y){
				//X++/--
				gc_state.coord_sys[X_AXIS] += sys.xInc;
				// set direction xInc
				Motor1.Conf.DIR = (sys.xInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				Motor2.Conf.DIR = (sys.xInc > 0) ? ST_ANTICLOCKWISE: ST_CLOCKWISE;
				// apply the direction to pin
				StM_Conf_Dir(&Motor1);
				StM_Conf_Dir(&Motor2);
				// set pin high
				STM_Step_High(&Motor1);
				STM_Step_High(&Motor2);
			}else if (dist_y < dist_diag && dist_y < dist_x){
				//Y++/--
				gc_state.coord_sys[Y_AXIS] += sys.yInc;
				// set direction yInc
				Motor1.Conf.DIR = (sys.yInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				Motor2.Conf.DIR = (sys.yInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				// apply the direction to pin
				StM_Conf_Dir(&Motor1);
				StM_Conf_Dir(&Motor2);
				// set pin high
				STM_Step_High(&Motor1);
				STM_Step_High(&Motor2);
			}
		}
	}
}


void move_arc_callback(){

}



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
	SpySys();
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

void SpySys(void){
	// Spy the G code Queue Stack
	Data_t msg;
	memset(&msg.data, '\0', MESSAGE_SIZE);
	float x = gc_state.coord_sys[X_AXIS];
	float y = gc_state.coord_sys[Y_AXIS];
	snprintf(msg.data,sizeof(msg.data),"<sys>%d,%d,%d.%d,%d.%d\n",sys.Move,sys.status,(int)x,(int)((x - (int)x) * 1000),(int)y,(int)((y - (int)y) * 1000));
	if (HAL_OK != Com_Queue_msg(&msg))
	{

	}
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
				GoToStep(&Motor1, atoi(token));
				token = strtok(NULL, ":");
				Motor1.Conf.DIR = token[0]- '0';
		} else if (strncmp(token, "M2", 2) == 0) {
			// Tokenize motor configuration for M2 by comma
			token = strtok(NULL, ":");
			GoToStep(&Motor2, atoi(token));
			token = strtok(NULL, ":");
			Motor2.Conf.DIR = token[0]- '0';
		}
		token = strtok(NULL, ":"); // Move to the next motor configuration
	}
	StM_Conf_Dir(&Motor1);
	StM_Conf_Dir(&Motor2);
	sys.Move = CMD;
	sys.status = SYS_RUNNING;
	sys.num_event = 0;
	HAL_TIM_Base_Start_IT(htim_step);

}
