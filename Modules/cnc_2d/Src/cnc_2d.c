/*
 * cnc_2d.c
 *
 *  Created on: Mar 7, 2024
 *      Author: soufiane
 */

#include "cnc_2d.h"



void cnc_init(void){

	Motor1.Conf.ENB = 0;
	Motor1.Conf.MS1 = 0;
	Motor1.Conf.MS2 = 0;
	Motor1.Conf.MS3 = 0;
	Motor1.Conf.DIR = 0;
	Motor1.Status = ST_OFF;
	Motor1.tar_steps = 0;
	Motor1.Steps_count = 0;

	Motor2.Conf.ENB = 0;
	Motor2.Conf.MS1 = 0;
	Motor2.Conf.MS2 = 0;
	Motor2.Conf.MS3 = 0;
	Motor2.Conf.DIR = 0;
	Motor2.Status = ST_OFF;
	Motor2.tar_steps = 0;
	Motor2.Steps_count = 0;

	StM_Pin_Conf_v2(&Motor1,&Motor2);
	StM_Conf_Init(&Motor1);
	StM_Conf_Init(&Motor2);
}


void cnc_DriveM(Step_Driver_Handler* Motor){
	if (Motor->Status == ST_ORDER){
			  STM_Step(Motor);
			  if (Motor->Steps_count == Motor->tar_steps){
					  Motor->Steps_count = 0;
					  Motor->Status = ST_OFF;
					  HAL_GPIO_WritePin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP,GPIO_PIN_RESET);
			  	  }
	}
}


void cnc_Step_X();
void cnc_Step_Y();
