/*
 * custom_ws2812b_driver.c
 *
 * Created: 2023-12-05 6:39:12 PM
 *  Author: vicga
 */ 

#include "SPIDriver/SPIDriver.h"
#include "custom_ws2812b_driver.h"
#include "GameLogicThread/GameLogicThread.h"

/*
* Sets the LED at index 
*/
void set_single_LED(uint32_t rgb, uint8_t index){
	
	 uint8_t r, g, b;
	 uint32_t color;
	 
	 r = (uint8_t)((rgb >> 16) & 0xFF);			//Isolate 8 bit red value			
	 g = (uint8_t)((rgb >> 8) & 0xFF);          //Isolate 8 bit green value
	 b = (uint8_t)((rgb >> 0) & 0xFF);          //Isolate 8 bit blue value
	 
	 color = ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | b;  //24 bit 
	 
	 for(int i = FRAME_LENGTH - 1; i>=0; i--){
		 if(color & 0x01){ //If LSB = 1
			 led_board_state[index*FRAME_LENGTH + i] = ONE;
		 } else {
			 led_board_state[index*FRAME_LENGTH + i] = ZERO;
		 }
		 color = color >> 1;
	 }
}

void set_board_LED(uint32_t *rgb_array, uint32_t length){
	
	for(int i = 0; i<length; i++){
		set_single_LED(rgb_array[i], i);
	}
	
	while(SPIGetMutex(1000) != ERROR_NONE);{
		
	}
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_write_buffer_job(&spi_master_instance, led_board_state, SPI_BUFFER_LENGTH);
	spi_select_slave(&spi_master_instance, &slave, false);
	SPIFreeMutex();
	
}

void reset_board_LED(){
	for(int i = 0; i<SPI_BUFFER_LENGTH; i++){
		led_board_state[i] = ZERO;
	}
	
	while(SPIGetMutex(1000) != ERROR_NONE);{
		
	}
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_write_buffer_job(&spi_master_instance, led_board_state, SPI_BUFFER_LENGTH);
	spi_select_slave(&spi_master_instance, &slave, false);
	SPIFreeMutex();
}

void set_LED_board_with_struct(){
	
	uint32_t gs_rgb[NUM_LED];
	
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){
			int index = board_ptr[i][j]->LEDNum;
			switch(board_ptr[i][j]->LEDStatus){
				case('r'):{
					gs_rgb[index] = RED;
					break;
				}
				case('g'):{
					gs_rgb[index] = GREEN;
					break;
				}
				case('b'):{
					gs_rgb[index] = BLUE;
					break;
				}
				case('p'):{
					gs_rgb[index] = PURPLE;
					break;
				}
				case(' '):{
					gs_rgb[index] = OFF;
					break;
				}
				default: break;
			}
		}
	}
	set_board_LED(gs_rgb, NUM_LED);
}

void reset_LED_board_with_struct(){
	
	for(int i = 0; i<8; i++){
		for(int j = 0; j<8; j++){
			board_ptr[i][j]->LEDStatus = ' ';
		}
	}
}

