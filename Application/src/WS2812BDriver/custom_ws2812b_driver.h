/*
 * custom_ws2812b_driver.h
 *
 * Created: 2023-12-05 6:39:27 PM
 *  Author: vicga
 */ 


//NOTE THAT ARRAY THAT STORES CURRENT STATE OF 8X8 LED GRID IS DEFINED IN CLITHREAD.C, MAKE THIS GLOBAL

#ifndef CUSTOM_WS2812B_DRIVER_H_
#define CUSTOM_WS2812B_DRIVER_H_

#include "GAME LOGIC/square.h"
#include "main.h"

#define FRAME_LENGTH 24 //24 bit frame for single LED

#define NUM_LED 64	//64 for entire chessboard
#define ONE 0x07	//0 bit encoding in SPI
#define ZERO 0x1F	//1 bit encoding in SPI
#define SPI_BUFFER_LENGTH (NUM_LED * 24)

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define PURPLE 0x400040
#define OFF 0x000000

uint8_t led_board_state [SPI_BUFFER_LENGTH];

void set_single_LED(uint32_t rgb, uint8_t index);
void set_board_LED(uint32_t *rgb_array, uint32_t length);
void reset_board_LED();
void set_LED_board_with_struct();
void reset_LED_board_with_struct();



#endif /* CUSTOM_WS2812B_DRIVER_H_ */