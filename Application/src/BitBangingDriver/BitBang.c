/*
 * BitBang.c
 *
 * Created: 2023-12-02 10:31:09 PM
 *  Author: vicga
 */ 

#include "BitBang.h"

#define LED_DRIVE_PIN EXT1_PIN_3

void configure_bit_bang_pin(){

	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_DRIVE_PIN, &config_port_pin); //PA02
}

void bit_bang_send_HIGH(){
	
	while(true){
		port_pin_set_output_level(LED_DRIVE_PIN, false);
		delay_ms(1);
		port_pin_set_output_level(LED_DRIVE_PIN, true);
		delay_ms(1);
	}
	
	
	
}

void bit_bang_send_LOW(){
	
}

void bit_bang_send_color(uint32_t rgb){
	
}