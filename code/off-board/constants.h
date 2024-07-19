#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// update depth estimate (comment out if not)
#define PREDICT_DEPTH

#ifdef PREDICT_DEPTH
// platform constant acceleration in m/s (1 in/s)
#define PLATFORM_ACCELERATION 0.0245
#endif

// we want to store two codes
#define STORED_CODE_COUNT 2
// index of internal code
#define CODE_INTERNAL 0
// index of downloaded code
#define CODE_DOWNLOADED 1
// hard-coded code
#define CODE_INTERNAL_VALUE 1234

// LoRa device SPI handler name of type SPI_HandleTypeDef
#define LORA_SPI_HANDLE hspi1

// LoRa NSS
#define LORA_NSS_PORT GPIOA
#define LORA_NSS_PIN GPIO_PIN_4

// send-code button
#define SEND_CODE_PORT GPIOC
#define SEND_CODE_PIN GPIO_PIN_12

// request-code button
#define REQUEST_CODE_PORT GPIOC
#define REQUEST_CODE_PIN GPIO_PIN_11

// release escape pod button
#define RELEASE_POD_PORT GPIOC
#define RELEASE_POD_PIN GPIO_PIN_10

// ballast tri-state switch
#define BALLAST_PORT GPIOC
#define BALLAST_UP_PIN GOIO_PIN_6
#define BALLAST_DOWN_PIN GPIO_PIN_7

// 7-segment display segment pins
#define DISPLAY_SEGMENT_PORT GPIOB
#define DISPLAY_SEGMENT_A GPIO_PIN_0
#define DISPLAY_SEGMENT_B GPIO_PIN_1
#define DISPLAY_SEGMENT_C GPIO_PIN_2
#define DISPLAY_SEGMENT_D GPIO_PIN_4
#define DISPLAY_SEGMENT_E GPIO_PIN_5
#define DISPLAY_SEGMENT_F GPIO_PIN_6
#define DISPLAY_SEGMENT_G GPIO_PIN_7
#define DISPLAY_SEGMENT_DP GPIO_PIN_8

// number of digits on 7-segment display
#define DISPLAY_DIGITS 4
// 7-segment display digit pins
#define DISPLAY_DIGIT_PORT GPIOB
#define DISPLAY_DIGIT_1 GPIO_PIN_12
#define DISPLAY_DIGIT_2 GPIO_PIN_13
#define DISPLAY_DIGIT_3 GPIO_PIN_14
#define DISPLAY_DIGIT_4 GPIO_PIN_15

#define TIMED_EVENTS_LIST_COUNT 1

#ifdef PREDICT_DEPTH
#define TIMED_EVENTS_LIST_CAPACITY 2
#else
#define TIMED_EVENTS_LIST_CAPACITY 1
#endif

// timer handle of type TIM_HandleTypeDef
#define TIMER_HANDLE htim7
// tick amount in timer (100ms)
#define TIMER_TICK_PER 100

// ADC handle
#define ADC_HANDLE 
// number of ADC conversions per (item count)
#define ADC_NCONV 2

#endif
