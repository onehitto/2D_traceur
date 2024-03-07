/*
 * SG90_controller.h
 *
 *  Created on: Aug 28, 2023
 *      Author: soufiane
 */

#ifndef SG90_CONTROLLER_H_
#define SG90_CONTROLLER_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"



/*
 * Calibration
 */
#define MAX_ANGLE 180
#define Min_ANGLE 0
#define OFFSET 250 // the servo motor is not perfect the duty 1ms != 0 degree
/*
 * Status
 */
typedef enum {
	ON = 0,
	OFF,
}Servo_Status_t;

/*
 * Servo Handle
 */
typedef struct{
	TIM_HandleTypeDef* ptim; //pointer to the driver that control PWM
	uint8_t curr_pos;
	Servo_Status_t Status;
}Servo_Handle_t;


/*********************************************************************************************************************
 * 													APIs supported by this driver
 * 								For more information about the APIs check the function definitions
 *********************************************************************************************************************/
/*
 * Servo Init
 */
void Servo_Init(Servo_Handle_t* servo,TIM_HandleTypeDef* ptim);
/*
 * Servo check position -90 -75 -60 -45 -30 -15 0 15 30 45 60 75 90
 */
HAL_StatusTypeDef Servo_Check_Operation(Servo_Handle_t* servo);
/*
 * Servo go to the given angle
 */
void Servo_Goto(Servo_Handle_t* servo,uint8_t angle);
/*
 * Start Servo
 */
void Servo_Start(Servo_Handle_t* servo);
/*
 * Stop Servo
 */
void Servo_Stop(Servo_Handle_t* servo);

#endif /* SG90_CONTROLLER_H_ */
