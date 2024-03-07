/*
 * Spy.h
 *
 *  Created on: Sep 11, 2023
 *      Author: soufiane
 */

#ifndef SPY_H_
#define SPY_H_

#include "stdio.h"

#include "Com_api.h"
#include "G_code.h"
#include "SG90_controller.h"

/*
 * global variable
 */

extern uint8_t Spy_erro_cnt;

void SpyServoMotor(Servo_Handle_t* servo);
void SpyJobManger();
void Spy_erro(uint8_t value);
//void SpyStepMotor(&servo);

#endif /* SPY_H_ */
