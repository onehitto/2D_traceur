/*
 * Step_Driver.c
 *
 *  Created on: Sep 17, 2023
 *      Author: soufiane
 */



#include "Step_Driver.h"

void StM_Pin_Conf(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2){

	// pin B9 en
	Motor1->Pins.port_ENB = GPIOB;
	Motor1->Pins.pin_ENB = GPIO_PIN_9;
	// pin 8 MS1 : 0
	Motor1->Pins.port_MS1 = GPIOB;
	Motor1->Pins.pin_MS1 = GPIO_PIN_8;
	// pin 7 MS2 : 0
	Motor1->Pins.port_MS2 = GPIOB;
	Motor1->Pins.pin_MS2 = GPIO_PIN_7;
	// pin 6 MS3 : 0
	Motor1->Pins.port_MS3 = GPIOB;
	Motor1->Pins.pin_MS3 = GPIO_PIN_6;
	// pin 4 DIR : 0
	Motor1->Pins.port_DIR = GPIOB;
	Motor1->Pins.pin_DIR = GPIO_PIN_4;
	// pin B5 Step
	Motor1->Pins.port_STEP = GPIOB;
	Motor1->Pins.pin_STEP = GPIO_PIN_5;



	// pin B9 EN
	Motor2->Pins.port_ENB = GPIOB;
	Motor2->Pins.pin_ENB = GPIO_PIN_9;
	// pin A7 MS1
	Motor2->Pins.port_MS1 = GPIOA;
	Motor2->Pins.pin_MS1 = GPIO_PIN_7;
	// pin A6 MS2
	Motor2->Pins.port_MS2 = GPIOA;
	Motor2->Pins.pin_MS2 = GPIO_PIN_6;
	// pin A5 MS3
	Motor2->Pins.port_MS3 = GPIOA;
	Motor2->Pins.pin_MS3 = GPIO_PIN_5;
	// pin A3 DIR
	Motor2->Pins.port_DIR = GPIOA;
	Motor2->Pins.pin_DIR = GPIO_PIN_3;
	// pin A4 Step
	Motor2->Pins.port_STEP = GPIOA;
	Motor2->Pins.pin_STEP = GPIO_PIN_4;

}

void StM_Pin_Conf_v2(Step_Driver_Handler * Motor1,Step_Driver_Handler * Motor2){

	// pin B9 en
	Motor1->Pins.port_ENB = GPIOB;
	Motor1->Pins.pin_ENB = GPIO_PIN_9;
	// pin 8 MS1 : 0
	Motor1->Pins.port_MS1 = GPIOB;
	Motor1->Pins.pin_MS1 = GPIO_PIN_7;
	// pin 7 MS2 : 0
	Motor1->Pins.port_MS2 = GPIOB;
	Motor1->Pins.pin_MS2 = GPIO_PIN_6;
	// pin 6 MS3 : 0
	Motor1->Pins.port_MS3 = GPIOB;
	Motor1->Pins.pin_MS3 = GPIO_PIN_5;
	// pin 4 DIR : 0
	Motor1->Pins.port_DIR = GPIOB;
	Motor1->Pins.pin_DIR = GPIO_PIN_3;
	// pin B5 Step
	Motor1->Pins.port_STEP = GPIOB;
	Motor1->Pins.pin_STEP = GPIO_PIN_4;



	// pin B9 EN
	Motor2->Pins.port_ENB = GPIOB;
	Motor2->Pins.pin_ENB = GPIO_PIN_9;
	// pin A7 MS1
	Motor2->Pins.port_MS1 = GPIOA;
	Motor2->Pins.pin_MS1 = GPIO_PIN_7;
	// pin A6 MS2
	Motor2->Pins.port_MS2 = GPIOA;
	Motor2->Pins.pin_MS2 = GPIO_PIN_6;
	// pin A5 MS3
	Motor2->Pins.port_MS3 = GPIOA;
	Motor2->Pins.pin_MS3 = GPIO_PIN_5;
	// pin A3 DIR
	Motor2->Pins.port_DIR = GPIOA;
	Motor2->Pins.pin_DIR = GPIO_PIN_3;
	// pin A4 Step
	Motor2->Pins.port_STEP = GPIOA;
	Motor2->Pins.pin_STEP = GPIO_PIN_4;

}

void StM_Conf_Init(Step_Driver_Handler * Motor){

	// pin ENB
	HAL_GPIO_WritePin(Motor->Pins.port_ENB, Motor->Pins.pin_ENB, Motor->Conf.ENB);
	// pin MS1
	HAL_GPIO_WritePin(Motor->Pins.port_MS1, Motor->Pins.pin_MS1, Motor->Conf.MS1);
	// pin MS2
	HAL_GPIO_WritePin(Motor->Pins.port_MS2, Motor->Pins.pin_MS2, Motor->Conf.MS2);
	// pin MS3
	HAL_GPIO_WritePin(Motor->Pins.port_MS3, Motor->Pins.pin_MS3, Motor->Conf.MS3);
	// pin DIR
	HAL_GPIO_WritePin(Motor->Pins.port_DIR, Motor->Pins.pin_DIR, Motor->Conf.DIR);

	HAL_GPIO_WritePin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP, GPIO_PIN_RESET);

}
void GoToStep(Step_Driver_Handler * Motor,uint16_t tar_Step,uint8_t acc)
{
	if (Motor->Status == ST_OFF){
		Motor->Status = ST_ORDER;
		Motor->tar_steps = tar_Step;
	}
}

void STM_Step(Step_Driver_Handler* Motor){

	HAL_GPIO_TogglePin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP);
	if (HAL_GPIO_ReadPin(Motor->Pins.port_STEP, Motor->Pins.pin_STEP) == GPIO_PIN_SET)
		Motor->Steps_count++;

}
