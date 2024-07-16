#ifndef _PINS_H
#define _PINS_H

// send-code button
#define SEND_CODE_PORT GPIOA
#define SEND_CODE_PIN GPIO_PIN_1

// request-code button
#define REQUEST_CODE_PORT GPIOA
#define REQUEST_CODE_PIN GIO_PIN_2

// release escape pod button
#define RELEASE_POD_PORT GPOIA
#define RELEASE_POD_PIN GPIO_PIN_3

// ballast tri-state switch
#define BALLAST_PORT GPPIOA
#define BALLAST_UP_PIN GPIO_PIN_4
#define BALLAST_DOWN_PIN GPIO_PIN_5

// 7-segment display segment pins
#define DISPLAY_SEGMENT_PORT GPIOB
#define DISPLAY_SEGMENT_A GPIO_PIN_1
#define DISPLAY_SEGMENT_B GPIO_PIN_2
#define DISPLAY_SEGMENT_C GPIO_PIN_3
#define DISPLAY_SEGMENT_D GPIO_PIN_4
#define DISPLAY_SEGMENT_E GPIO_PIN_5
#define DISPLAY_SEGMENT_F GPIO_PIN_6
#define DISPLAY_SEGMENT_G GPIO_PIN_7
#define DISPLAY_SEGMENT_DP GPIO_PIN_8

// 7-segment display digit pins
#define DISPLAY_DIGIT_PORT GPIOB
#define DISPLAY_DIGIT_1 GPIO_PIN_12
#define DISPLAY_DIGIT_2 GPIO_PIN_13
#define DISPLAY_DIGIT_3 GPIO_PIN_14
#define DISPLAY_DIGIT_4 GPIO_PIN_15

#endif
