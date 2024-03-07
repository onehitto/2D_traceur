/*
 * SG90_controller.c
 *
 *  Created on: Aug 28, 2023
 *      Author: soufiane
 */


#include "SG90_controller.h"








/*
 * Servo Init
 */
/**************************************************************************
 * @fn					- Servo_Init
 *
 * @brief				- Initialize the servo hanlder and check the servo operation
 * @param[in]			- Servo_Handle_t
 *
 * @return				- HAL_StatusTypeDef HAL_OK = 0 / HAL_ERROR = 1
 *
 * @Note				- None
 *
 **************************************************************************/
void Servo_Init(Servo_Handle_t* servo,TIM_HandleTypeDef* ptim)
{
	servo->ptim = ptim;
	servo->curr_pos = 0;
	servo->Status = OFF;
	Servo_Goto(servo, 0);


}
/*
 * Servo check position 0 15 30 45 60 75 90 105 120 135 150 165 180
 */
/**************************************************************************
 * @fn					- Servo_Check_Operation
 *
 * @brief				- check angles 0 15 30 45 60 75 90 105 120 135 150 165 180 / btw every angles a delay of 300 ms
 * @param[in]			- Servo_Handle_t
 *
 * @return				- HAL_StatusTypeDef HAL_OK = 0 / HAL_ERROR = 1
 *
 * @Note				- None
 *
 **************************************************************************/
HAL_StatusTypeDef Servo_Check_Operation(Servo_Handle_t* servo){

	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 15);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 30);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 45);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 60);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 75);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 90);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 105);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 120);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 135);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 150);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 165);
	vTaskDelay(pdMS_TO_TICKS(2000));
	Servo_Goto(servo, 180);

	return HAL_OK;
}

/*
 * Servo go to the given angle
 */
/**************************************************************************
 * @fn					- Servo_Goto
 *
 * @brief				- Servo go to the given anglet
 * @param[in]			- Servo_Handle_t
 * @param[in]			- angle (degree 0->180)
 *
 * @return				- None
 *
 * @Note				- None
 *
 **************************************************************************/
void Servo_Goto(Servo_Handle_t* servo,uint8_t angle){
		uint16_t Period = servo->ptim->Init.Period;
		uint16_t duty = angle *( ( (Period / 10) - OFFSET) / MAX_ANGLE )+ OFFSET;
		servo->curr_pos = angle;
		Servo_Start(servo);
		__HAL_TIM_SET_COMPARE(servo->ptim,TIM_CHANNEL_1,duty);
}
/*
 * Start Servo
 */
/**************************************************************************
 * @fn					- Servo_Start
 *
 * @brief				- This function enable Servo
 * @param[in]			- Servo_Handle_t
 *
 * @return				- None
 *
 * @Note				- None
 *
 **************************************************************************/
void Servo_Start(Servo_Handle_t* servo){
	if(servo->Status == OFF ){
		HAL_TIM_PWM_Start(servo->ptim, TIM_CHANNEL_1);
		servo->Status = ON;
	}
}
/*
 * Stop Servo
 */
/**************************************************************************
 * @fn					- Servo_Stop
 *
 * @brief				- This function disable Servo
 * @param[in]			- Servo_Handle_t
 *
 * @return				- None
 *
 * @Note				- None
 *
 **************************************************************************/
void Servo_Stop(Servo_Handle_t* servo){
	if(servo->Status == ON ){
		HAL_TIM_PWM_Stop(servo->ptim, TIM_CHANNEL_1);
		servo->Status = OFF;
	}
}
