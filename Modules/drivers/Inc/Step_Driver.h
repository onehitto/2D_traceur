/*
 * Step_driver.h
 *
 *  Created on: Sep 17, 2023
 *      Author: soufiane
 */

#ifndef STEP_DRIVER_H_
#define STEP_DRIVER_H_

#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

typedef struct{
	GPIO_TypeDef* port_ENB;
	uint16_t pin_ENB;

	GPIO_TypeDef* port_MS1;
	uint16_t pin_MS1;

	GPIO_TypeDef* port_MS2;
	uint16_t pin_MS2;

	GPIO_TypeDef* port_MS3;
	uint16_t pin_MS3;

	GPIO_TypeDef* port_DIR;
	uint16_t pin_DIR;

	GPIO_TypeDef* port_STEP;
	uint16_t pin_STEP;

}Setup_Pins;

typedef struct{
	uint8_t ENB;
	uint8_t MS1;
	uint8_t MS2;
	uint8_t MS3;
	uint8_t DIR;
	uint8_t STEP;

}Step_Conf_t;

typedef enum {
	ST_OFF = 0,
	ST_ORDER,
}Step_Status_t;

typedef struct{
	Setup_Pins Pins;
	Step_Conf_t Conf;
	Step_Status_t Status;
	volatile uint32_t Steps_count;
	volatile uint32_t tar_steps;
	uint8_t feed_rate;
}Step_Driver_Handler;

typedef enum {
	ST_CLOCKWISE = 0,
	ST_ANTICLOCKWISE,
}Step_Direction_t;


#define DIR_MOTOR_CLOCKWISE(Motor) HAL_GPIO_WritePin(Motor->Pins.port_DIR, Motor->Pins.pin_DIR, ST_CLOCKWISE);
#define DIR_MOTOR_ST_ANTICLOCKWISE(Motor) HAL_GPIO_WritePin(Motor->Pins.port_DIR, Motor->Pins.pin_DIR, ST_ANTICLOCKWISE);

void StM_Pin_Conf(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2);
void StM_Conf_Init(Step_Driver_Handler * Motor);
void GoToStep(Step_Driver_Handler * Motor,uint16_t Step,uint8_t acc);
void STM_Step(Step_Driver_Handler* Motor);





#endif /* STEP_DRIVER_H_ */
