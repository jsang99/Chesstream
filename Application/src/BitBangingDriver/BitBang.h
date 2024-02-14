/*
 * BitBang.h
 *
 * Created: 2023-12-02 10:31:26 PM
 *  Author: vicga
 */ 


#ifndef BITBANG_H_
#define BITBANG_H_

#include <FreeRTOS.h>
#include <asf.h>
#include <semphr.h>
#include <task.h>

void configure_bit_bang_pin();

void bit_bang_send_HIGH();

void bit_bang_send_LOW();

void bit_bang_send_color(uint32_t rgb);



#endif /* BITBANG_H_ */