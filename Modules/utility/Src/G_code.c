/*
 * G_code.c
 *
 *  Created on: Sep 13, 2023
 *      Author: soufiane
 */

#include "G_code.h"


Buf_Handler_t Gcode_Stack;

union{
	uint32_t d;
	float f;
}t;
float x;

void G_Code_Init(){
	Buf_Init(&Gcode_Stack);
}

// Function to verify if a line is a G-Code line
int isGCodeLine(const char* line){

	// Check for a null pointer or empty string
	if (line == NULL || *line == '\0') {
		return 0;
	}

	// Check if the line starts with 'G' or 'M' and followed by numbers
	if (*line == 'G' || *line == 'M') {
		line++; // Move to the next character

		// There should be digits after 'G'/'M'
		if (!isdigit((unsigned char)*line)) {
			return 0;
		}
		while (isdigit((unsigned char)*line)) { // Skip all digits
			line++;
		}
		//copy the line to not change the orignal
		char cpy_line[64];
		memcpy(cpy_line,line,64);
		// Now, we should have parameters or end of line
		char* token = strtok(cpy_line, " ");
		while (token != NULL) {
			// Each parameter should follow a specific pattern, e.g., 'X' followed by a number
			if (strlen(token) < 2 || !(token[0] == 'X' || token[0] == 'Y' || token[0] == 'Z') || !isdigit((unsigned char)token[1])) {
				return 0; // Does not meet the required pattern
			}
			// Check the rest of the token for valid number format
			for (int i = 2; token[i] != '\0'; i++) {
				if (!isdigit((unsigned char)token[i]) && token[i] != '.') {
					return 0; // Invalid character in parameter's value
				}
			}
			token = strtok(NULL, " ");
		}
		return 1; // Passed all checks
	}

	return 0; // Not a G-Code line
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}


void GCode_Parser(Data_t* G_Code) {

	int8_t idx = 1;
	switch(G_Code->data[idx]){
		case('1'):
				G1_Parser(G_Code);
				break;
		default:
			Com_SendMsg("G_Code not supported !!\n");
	}

}
void G1_Parser(Data_t* G_Code) {
	char cpy_line[64];
	memcpy(cpy_line,G_Code->data,64);
	char* token = strtok(cpy_line, " ");
	Data_t msg;

	token = strtok(NULL, " ");
	while (token != NULL){
		switch(token[0]){
			case('X'):

				x = stringToFloat(&token[1]);
				t.f = x;
				snprintf(msg.data,sizeof(msg.data),"X: %lu\n",t.d);
				Com_Queue_msg(&msg);

				break;
			case('Y'):
				x = stringToFloat(&token[1]);
				t.f = sqrt(x);
				snprintf(msg.data,sizeof(msg.data),"X: %lu\n",t.d);
				Com_Queue_msg(&msg);
				break;
			default:
				;
		}
		token = strtok(NULL, " ");
	}
}


float stringToFloat(const char *str) {
    float result = 0.0;
    float divisor = 1.0;
    int decimalFound = 0;

    // Process each character
    for (; *str; str++) {
        if (*str == '.') {
            decimalFound = 1;
            continue;
        }

        if (decimalFound) {
            // Handle decimal places
            divisor *= 10.0;
            result += (*str - '0') / divisor;
        } else {
            // Build integer part
            result = result * 10.0 + (*str - '0');
        }
    }

    return result;
}
