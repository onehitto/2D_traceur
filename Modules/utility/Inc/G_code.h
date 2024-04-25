/*
 * G_code.h
 *
 *  Created on: Sep 13, 2023
 *      Author: soufiane
 */

#ifndef G_CODE_H_
#define G_CODE_H_


#include <stdlib.h>
#include "Queue_job_manager.h"
#include "SG90_controller.h"
#include "Com_api.h"
#include <stdbool.h>
#include "math.h"


#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2


#define MODAL_G0 0 // G0 rapid positioning
#define MODAL_G1 1 // G1 linear interpolation
#define MODAL_G2 2  // CW_ARC circular/helical interpolation (clockwise)
#define MODAL_G3 3  // CCW_ARC circular/helical interpolation (counterclockwise)

#define MODAL_G43 43 //tool length offset (plus)
#define MODAL_G49 49 //cancel tool length offset

#define MODAL_M0 //program stop
#define MODAL_M2 //program end
#define MODAL_M30 //program end, pallet shuttle, and reset

#define NO_MODAL_G28 28// return to home

#define PROGRAM_RUNNING 0
#define PROGRAM_STOP 1



#define MAX_INT_DIGITS 8 // Maximum number of digits in int32 (and float)
#define false 0
#define true 1

#define QUEUE_G_CODE(X) Buf_Queue(&Gcode_Stack,X)
#define DEQUEUE_G_CODE(X) Buf_Dequeue(&Gcode_Stack,X)


typedef struct{
    uint8_t motion; // groupe 1 = {G0, G1, G2, G3} motion
    uint8_t tool_length; // group 8 = {G43, G49} tool length offset
    uint8_t program_flow; // group 4 = {M0, M2, M30, M60} stopping
}gc_modal_t;

typedef struct {
    float f;        //feed rate
    float ijk[3];
    float r;         // Arc radius
    float xyz[3];    // X,Y,Z Translational axes
} gc_values_t;

typedef struct{
    gc_modal_t modal;
    uint8_t speed;
    float coord_sys[3];
    float coord_sys_prev[3];
    float coord_offset[3];
    float tool_length_offset;
}gc_state_t;

typedef struct{
    uint8_t non_modal; //group 0 = {G28}
    gc_modal_t modal;
    gc_values_t values;
}gc_block_t;

extern Buf_Handler_t Gcode_Stack;

extern gc_block_t gc_block;

extern gc_state_t gc_state;


// Initialize the parser
void G_Code_Init();

//parsers
void GCode_Parser(Data_t* G_Code);
// Execute one block of rs275/ngc/g-code
uint8_t gc_execute_line(char *line);

//utility
int isGCodeLine(const char* line);
int isdigit(int c);
float stringToFloat(const char *str);
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);

#endif /* G_CODE_H_ */
