/*
 * cnc_2d.h
 *
 *  Created on: Mar 7, 2024
 *      Author: soufiane
 */

#ifndef CNC_2D_H_
#define CNC_2D_H_

#include "SG90_controller.h"
#include "Step_Driver.h"
#include "Com_api.h"
#include "G_code.h"
#include "Queue_job_manager.h"
#include "Step_Driver.h"



extern Step_Driver_Handler Motor1;
extern Step_Driver_Handler Motor2;

extern Servo_Handle_t servo;

extern TIM_HandleTypeDef* htim_servo;
extern TIM_HandleTypeDef* htim_step;

void cnc_init(TIM_HandleTypeDef* htim_servo,TIM_HandleTypeDef* htimstep);
void StM_Pin_Conf(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void StM_Pin_Conf_v2(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void cnc_DriveM_TIMcallback(Step_Driver_Handler* Motor);


void cnc_Step_X();
void cnc_Step_Y();


void SpyServoMotor(void);
void SpyStepsMotor(void);
void Spycnc(void);


void CmdMove(Data_t ptr);
void ApplyConf(Data_t ptr);

#endif /* CNC_2D_H_ */
