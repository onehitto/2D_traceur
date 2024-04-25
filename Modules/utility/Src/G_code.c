/*
 * G_code.c
 *
 *  Created on: Sep 13, 2023
 *      Author: soufiane
 */

#include "G_code.h"


gc_block_t gc_block;

gc_state_t gc_state;

Buf_Handler_t Gcode_Stack;

void G_Code_Init(){
    // init the blocks and sys_parm

    memset(&gc_state,0,sizeof(gc_state_t));
    memset(&gc_block,0,sizeof(gc_block_t));

    Buf_Init(&Gcode_Stack);

    Com_SendMsg("G_Code Init ... \n");
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

	gc_execute_line(G_Code->data);

}


uint8_t gc_execute_line(char *line){

    memset(&gc_block, 0, sizeof(gc_block_t));
    uint8_t char_counter = 0;
    uint8_t int_value;
    uint8_t letter;
    float value;
    //parsing a G code and store the value in approriate structure
    //  motion : G0/G1/G2/G3

    while (line[char_counter] != 0){
      if  (line[char_counter] == ' ')
        char_counter++;
      else{
          letter = line[char_counter];
          char_counter++;
          read_float(line, &char_counter, &value);
          int_value = trunc(value);
          switch (letter){
            case ('G'):
                switch (int_value)
                {
                case 0: case 1:
                  gc_block.modal.motion = MODAL_G1;
                    break;
                case 2:
                  gc_block.modal.motion = MODAL_G2;
                    break;
                case 3:
                  gc_block.modal.motion = MODAL_G3;
                    break;
                case 43:
                  gc_block.modal.motion = MODAL_G43;
                  break;
                case 49:
                  gc_block.modal.motion = MODAL_G49;
                  break;
                default:
                    break;
                }
                break;
            case 'M':
              break;
            case 'F':gc_block.values.f = value; break;
            case 'I':gc_block.values.ijk[X_AXIS] = value;break;
            case 'J':gc_block.values.ijk[Y_AXIS] = value;break;
            case 'K':gc_block.values.ijk[Z_AXIS] = value;break;
            case 'R':gc_block.values.r= value;break;
            case 'X':gc_block.values.xyz[X_AXIS] = value;break;
            case 'Y':gc_block.values.xyz[Y_AXIS] = value;break;
            case 'Z':gc_block.values.xyz[Z_AXIS] = value;break;
            default:
                printf("Not recognized \n");
                break;
            }
        }
    }
    // parsing completed
    switch (gc_block.modal.motion)
    {
    case MODAL_G0:case MODAL_G1:
    		float x0 = gc_state.coord_sys[X_AXIS];
    		float y0 = gc_state.coord_sys[Y_AXIS];
    		//float z0 = gc_state.coord_sys[Z_AXIS];

    		float x1 = gc_block.values.xyz[X_AXIS];
    		float y1 = gc_block.values.xyz[Y_AXIS];
    		//float z1 = gc_block.values.xyz[Z_AXIS];
    		// store xyz sys coordinate to previous coordinate
    		gc_state.coord_sys_prev[X_AXIS] = x0;
    		gc_state.coord_sys_prev[Y_AXIS] = y0;
    		//gc_state.coord_sys_prev[Z_AXIS] = z0;

    		int  max_step = 0;
    		float xInc, yInc,m,b,cal_save;
    		// check if x0 == x1 Yinc++ else if y1 == y0 xInc else
    		if (x0 == x1){
    			//Y ++/--
    			yInc = (y1 > y0) ? RESOLUTION_MN_XY : -RESOLUTION_MN_XY;
    			xInc = 0;
    			//a voir
    			Motor1.Conf.DIR = (yInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				Motor2.Conf.DIR = (yInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				StM_Conf_Dir(&Motor1);
				StM_Conf_Dir(&Motor2);
    		}else if (y0 == y1){
    			//X ++/--
    			xInc = (x1 > x0) ? RESOLUTION_MN_XY : -RESOLUTION_MN_XY;
    			yInc = 0;
    			//a voir
    			Motor1.Conf.DIR = (xInc > 0) ? ST_CLOCKWISE: ST_ANTICLOCKWISE;
				Motor2.Conf.DIR = (xInc > 0) ? ST_ANTICLOCKWISE: ST_CLOCKWISE;
				StM_Conf_Dir(&Motor1);
				StM_Conf_Dir(&Motor2);
    		}else{
    			m = (y1 - y0)/(x1 - x0);
    			b = y0 - m * x0;
    			cal_save = 1 / sqrt(m*m + 1);
    			xInc = (x1 > x0) ? RESOLUTION_MN_XY : -RESOLUTION_MN_XY;
				yInc = (y1 > y0) ? RESOLUTION_MN_XY : -RESOLUTION_MN_XY;
				sys.xInc_d = (x1 > x0) ? RESOLUTION_MN_DIAG : -RESOLUTION_MN_DIAG;
				sys.yInc_d = (y1 > y0) ? RESOLUTION_MN_DIAG : -RESOLUTION_MN_DIAG;
    		}
    		// calculate the m/ b ( 0= mx -y +b) of the interpolation (x0,y0) (x1,y1)
    		//  m = (y1 - y0)/(x1 - x0)
    		//  b = y0 - m * x0
    		// the distance between the point x,y and the line is fabs( m * x - y + b) / sqrt(m*m + 1)

			// update the SYS variables
			sys.status = SYS_RUNNING;
			sys.num_event = 0;
			sys.m = m;
			sys.b = b;
			sys.cal_save = cal_save;
			sys.xInc = xInc;
			sys.yInc = yInc;
			sys.max_step = max_step;
			sys.Move = LINEAR;
			HAL_TIM_Base_Start_IT(htim_step);

    	break;
    case MODAL_G2:case MODAL_G3:
        //
        break;
    case MODAL_G43:
        gc_state.tool_length_offset = gc_block.values.xyz[Z_AXIS];
      break;
    case MODAL_G49:
        gc_state.tool_length_offset = 0;
      break;
    default:
      break;
    }

    return 1 ;
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

uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr)
{
  char *ptr = line + *char_counter;
  unsigned char c;

  // Grab first character and increment pointer. No spaces assumed in line.
  c = *ptr++;

  // Capture initial positive/minus character
  bool isnegative = false;
  if (c == '-') {
    isnegative = true;
    c = *ptr++;
  } else if (c == '+') {
    c = *ptr++;
  }

  // Extract number into fast integer. Track decimal in terms of exponent value.
  uint32_t intval = 0;
  int8_t exp = 0;
  uint8_t ndigit = 0;
  bool isdecimal = false;
  while(1) {
    c -= '0';
    if (c <= 9) {
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) {
        if (isdecimal) { exp--; }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      } else {
        if (!(isdecimal)) { exp++; }  // Drop overflow digits
      }
    } else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal)) {
      isdecimal = true;
    } else {
      break;
    }
    c = *ptr++;
  }

  // Return if no digits have been read.
  if (!ndigit) { return(false); };

  // Convert integer into floating point.
  float fval;
  fval = (float)intval;

  // Apply decimal. Should perform no more than two floating point multiplications for the
  // expected range of E0 to E-4.
  if (fval != 0) {
    while (exp <= -2) {
      fval *= 0.01;
      exp += 2;
    }
    if (exp < 0) {
      fval *= 0.1;
    } else if (exp > 0) {
      do {
        fval *= 10.0;
      } while (--exp > 0);
    }
  }

  // Assign floating point value with correct sign.
  if (isnegative) {
    *float_ptr = -fval;
  } else {
    *float_ptr = fval;
  }

  *char_counter = ptr - line - 1; // Set char_counter to next statement

  return(true);
}

