/*
 * ShiftReg.c
 *
 * Created: 2023-12-12 5:10:30 PM
 *  Author: vicga
 */ 

#include "ShiftReg.h"
#include "SerialConsole/SerialConsole.h"

/*
INPUTS: int index --> index of reed_state
OUTPUTS: an integer from 0 to 63 equal to (row-1) * 8 + (col-1) (Example row 1, col 5 -> returns 4)
*/

int convert_to_chess_tile(int index){
	switch(index)
	{
		case 36: return 32;
		case 37: return 33;
		case 38: return 34;
		case 39: return 35;
		case 20: return 4;
		case 21: return 5;
		case 22: return 6;
		case 23: return 7;
		case 34: return 40;
		case 35: return 41;
		case 33: return 42;
		case 32: return 43;
		case 18: return 12;
		case 19: return 13;
		case 17: return 14;
		case 16: return 15;
		case 44: return 48;
		case 45: return 49;
		case 46: return 50;
		case 47: return 51;
		case 28: return 20;
		case 29: return 21;
		case 30: return 22;
		case 31: return 23;
		case 43: return 56;
		case 42: return 57;
		case 41: return 58;
		case 40: return 59;
		case 27: return 28;
		case 26: return 29;
		case 25: return 30;
		case 24: return 31;
		case 52: return 0;
		case 53: return 1;
		case 54: return 2;
		case 55: return 3;
		case 4: return 36;
		case 5: return 37;
		case 6: return 38;
		case 7: return 39;
		case 50: return 8;
		case 51: return 9;
		case 49: return 10;
		case 48: return 11;
		case 2: return 44;
		case 3: return 45;
		case 1: return 46;
		case 0: return 47;
		case 60: return 16;
		case 61: return 17;
		case 62: return 18;
		case 63: return 19;
		case 12: return 52;
		case 13: return 53;
		case 14: return 54;
		case 15: return 55;
		case 59: return 24;
		case 58: return 25;
		case 57: return 26;
		case 56: return 27;
		case 11: return 60;
		case 10: return 61;
		case 9: return 62;
		case 8: return 63;
		default: return -1;
	}
}


int convert_to_shift_index(int tile){
	switch(tile)
	{
		case 32: return 36;
		case 33: return 37;
		case 34: return 38;
		case 35: return 39;
		case 4: return 20;
		case 5: return 21;
		case 6: return 22;
		case 7: return 23;
		case 40: return 42;
		case 41: return 43;
		case 42: return 44;
		case 43: return 45;
		case 12: return 18;
		case 13: return 19;
		case 14: return 17;
		case 15: return 16;
		case 48: return 44;
		case 49: return 45;
		case 50: return 46;
		case 51: return 47;
		case 20: return 28;
		case 21: return 29;
		case 22: return 30;
		case 23: return 31;
		case 56: return 43;
		case 57: return 42;
		case 58: return 41;
		case 59: return 40;
		case 28: return 27;
		case 29: return 26;
		case 30: return 25;
		case 31: return 24;
		case 0: return 52;
		case 1: return 53;
		case 2: return 54;
		case 3: return 55;
		case 36: return 4;
		case 37: return 5;
		case 38: return 6;
		case 39: return 7;
		case 8: return 50;
		case 9: return 51;
		case 10: return 49;
		case 11: return 48;
		case 44: return 2;
		case 45: return 3;
		case 46: return 1;
		case 47: return 0;
		case 16: return 60;
		case 17: return 61;
		case 18: return 62;
		case 19: return 63;
		case 52: return 12;
		case 53: return 13;
		case 54: return 14;
		case 55: return 15;
		case 24: return 59;
		case 25: return 58;
		case 26: return 57;
		case 27: return 56;
		case 63: return 8;
		case 62: return 9;
		case 61: return 10;
		case 60: return 11;


		default: return -1;
	}
}


//Serial Data In - PA03
//CLK - PA06
//LD - PA02

void initialize_shift_reg(){
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(SHIFT_LOAD_PIN, &config_port_pin); //Set LD to output
	port_pin_set_config(SHIFT_CLK_PIN, &config_port_pin); //Set CLK to output
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = LOW;
	port_pin_set_config(SHIFT_SERIAL_IN, &config_port_pin); //Set SERIAL IN as input
	port_pin_set_config(SHIFT_SERIAL_IN_ALT, &config_port_pin); //Set SERIAL IN ALT as input
	
	port_pin_set_output_level(SHIFT_LOAD_PIN, 0); //LD set to low initially
	port_pin_set_output_level(SHIFT_CLK_PIN, 0); //CLK set to low initially
	
}

void read_array(){
	
	char bufferPrint[64];
	
	//memset(reed_state, 0, NUM_REEDS);
	
	port_pin_set_output_level(SHIFT_LOAD_PIN, 0); 
	port_pin_set_output_level(SHIFT_LOAD_PIN, 1); //Toggle LOAD to start shift
	

	//SerialConsoleWriteString("LOAD HIGH, START SHIFT\r\n");

	
	for(int i = 0; i<DAISY_CHAIN_LENGTH*8; i++){
		
		if(port_pin_get_input_level(SHIFT_SERIAL_IN_ALT)){
			reed_state[i+32] = 0;
// 			snprintf(bufferPrint, 64, "HIGH at index %d \r\n", i+32);
// 			SerialConsoleWriteString(bufferPrint);
		} else {
// 			snprintf(bufferPrint, 64, "LOW at index %d \r\n", i+32);
// 			SerialConsoleWriteString(bufferPrint);
			reed_state[i+32] = 1;
		}
		
		if(port_pin_get_input_level(SHIFT_SERIAL_IN)){
			reed_state[i] = 0;
// 			snprintf(bufferPrint, 64, "HIGH at index %d \r\n", i);
// 			SerialConsoleWriteString(bufferPrint);
		} else {
// 			snprintf(bufferPrint, 64, "LOW at index %d \r\n", i);
// 			SerialConsoleWriteString(bufferPrint);
			reed_state[i] = 1;
		}
		//Advance clock cycle
		port_pin_set_output_level(SHIFT_CLK_PIN, 1);
		vTaskDelay(20);
		port_pin_set_output_level(SHIFT_CLK_PIN, 0);
	}
	

	//SerialConsoleWriteString("PRINTING BOARD STATE\r\n");
// 	for(int i = 0; i<NUM_REEDS; i++){
// 		if(reed_state[i]){
// //			int temp = convert_to_chess_tile(i);
// // 			snprintf(bufferPrint, 64, "tile %d, %d is occupied!\r\n", temp/8 + 1, (temp % 8) + 1);
// // 			SerialConsoleWriteString(bufferPrint);
// 		}
// 		
// 	}

	
	
}
