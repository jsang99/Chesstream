#ifndef LED_H_
#define LED_H_

#include <stdint.h>

// Already defined in application code file
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RgbColorPacket;

#endif LED_H_ // LED_H_