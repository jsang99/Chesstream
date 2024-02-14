/*
 * ShiftReg.h
 *
 * Created: 2023-12-12 5:10:46 PM
 *  Author: vicga
 */ 
#include <FreeRTOS.h>
#include <asf.h>
#include <semphr.h>
#include <task.h>
#include <stdbool.h>

#ifndef SHIFTREG_H_
#define SHIFTREG_H_

#define DAISY_CHAIN_LENGTH 4
#define NUM_REEDS 64
#define ALT_READ_LENGTH 32

#define SHIFT_LOAD_PIN PIN_PA02
#define SHIFT_CLK_PIN PIN_PA06
#define SHIFT_SERIAL_IN PIN_PA03
#define SHIFT_SERIAL_IN_ALT PIN_PB03

bool reed_state [NUM_REEDS]; 

void initialize_shift_reg();
void read_array();
int convert_to_chess_tile(int index);
int convert_to_shift_index(int tile);




#endif /* SHIFTREG_H_ */