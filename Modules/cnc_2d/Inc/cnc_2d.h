/*
 * cnc_2d.h
 *
 *  Created on: Mar 7, 2024
 *      Author: soufiane
 */

#ifndef CNC_2D_H_
#define CNC_2D_H_

#include "SG90_controller.h"
#include "Spy.h"
#include "Step_Driver.h"
#include "Com_api.h"
#include "G_code.h"
#include "Queue_job_manager.h"
#include "Step_Driver.h"



Step_Driver_Handler Motor1;
Step_Driver_Handler Motor2;

void cnc_init(void);
void StM_Pin_Conf(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void StM_Pin_Conf_v2(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void cnc_DriveM(Step_Driver_Handler* Motor);

void cnc_Step_X();
void cnc_Step_Y();

#endif /* CNC_2D_H_ */
