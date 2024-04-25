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


typedef enum
{
  SYS_RUNNING       = 0x00U,
  SYS_PAUSED,
  SYS_STOP
}Sys_Status_t;

typedef enum
{
	LINEAR       = 0x00U,
  ARC,
  CMD,
  STOP,
  NONE
}Callback_move_T;

typedef struct{
	uint16_t id_exe;
	Callback_move_T Move;
	Sys_Status_t status;
	uint32_t num_event;
	float xInc, yInc ,xInc_d, yInc_d ;
	float max_step ;
	float m,b,cal_save;

}Sys_t;


#define STEPS_PER_MM_DIAG 83
#define RESOLUTION_MN_DIAG 0.012

#define STEPS_PER_MM_XY 62
#define RESOLUTION_MN_XY 0.016


extern Step_Driver_Handler Motor1;
extern Step_Driver_Handler Motor2;

extern Servo_Handle_t servo;

extern TIM_HandleTypeDef* htim_servo;
extern TIM_HandleTypeDef* htim_step;

extern Sys_t sys;


void cnc_init(TIM_HandleTypeDef* htim_servo,TIM_HandleTypeDef* htimstep);
void StM_Pin_Conf(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void StM_Pin_Conf_v2(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void cnc_DriveM_TIMcallback(Step_Driver_Handler* Motor);

void move_line_callback();
void move_arc_callback();


void SpyServoMotor(void);
void SpyStepsMotor(void);
void Spycnc(void);
void SpyComErr(void);
void SpySys(void);

void CmdMove(Data_t ptr);
void ApplyConf(Data_t ptr);

#endif /* CNC_2D_H_ */
