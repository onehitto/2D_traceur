/*
 * Spy.c
 *
 *  Created on: Sep 11, 2023
 *      Author: soufiane
 */


#include "Spy.h"


uint8_t Spy_erro_cnt = 0;

/**************************************************************************
 * @fn					- SpyServoMotor
 *
 * @Note				- Queue information about servo motor (status,curr_pos...)
 *
 **************************************************************************/
void SpyServoMotor(Servo_Handle_t* servo){
	Data_t msg;
	snprintf(msg.data,sizeof(msg.data),"<InfoServo>,%d,%d,%lu,%lu,%lu,\n",servo->Status,servo->curr_pos,servo->ptim->Instance->CCR1,servo->ptim->Init.Prescaler,servo->ptim->Init.Period);
	if (HAL_OK != Com_Queue_msg(&msg))
	{
		Spy_erro_cnt++;
	}

}
/**************************************************************************
 * @fn					- SpyJobManger
 *
 * @Note				-
 *
 **************************************************************************/
void SpyJobManger(){
	// to be define
}
/**************************************************************************
 * @fn					- Spy_erro
 *
 * @Note				-
 *
 **************************************************************************/
void Spy_erro(uint8_t value){
	Data_t msg;
	sprintf(msg.data,"<Infoerro>,%d,\n",value);

	if (HAL_OK != Com_Queue_msg(&msg))
	{
		Spy_erro_cnt++;
	}
}
