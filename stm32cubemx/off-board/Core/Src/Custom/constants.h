#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// update depth estimate (comment out if not)
#define PREDICT_DEPTH

#ifdef PREDICT_DEPTH
// platform constant acceleration in m/s (1 in/s)
#define PLATFORM_ACCELERATION 0.0245
#endif

// hard-coded code
#define CODE_INITIAL_VALUE 1234

// main SPI handle of type SPI_HandleTypeDef
#define SPI_HANDLE hspi1

// LoRa NSS
#define LORA_NSS_PORT GPIOA
#define LORA_NSS_PIN GPIO_PIN_10

// LoRa DIO0
#define LORA_DIO0_PORT GPIOA
#define LORA_DIO0_PIN GPIO_PIN_8

// LoRa reset
#define LORA_RESET_PORT GPIOA
#define LORA_RESET_PIN GPIO_PIN_9

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
#define BALLAST_UP_PIN GPIO_PIN_6
#define BALLAST_DOWN_PIN GPIO_PIN_7

// 7-segment display select
#define DISPLAY_SELECT1_2_PORT GPIOB
#define DISPLAY_SELECT1_2_PIN GPIO_PIN_0

#define DISPLAY_SELECT3_4_PORT GPIOB
#define DISPLAY_SELECT3_4_PIN GPIO_PIN_1

// TIM_HandleTypeDef, configured to 100ms
#define TIMER_HANDLE htim7

// TIM_HandleTypeDef, configured to 100ms (only enabled when moving up or down)
#define TIMER_DEPTH_HANDLE htim6

// TIMER_DEPTH_HANDLE: get interval in seconds
#define TIMER_DEPTH_INTERVAL 0.1

// ADC handle of type ADC_HandleTypeDef
#define ADC_HANDLE hadc1
// number of ADC conversions per (item count)
#define ADC_NCONV 2

// joystick x-y axis bounds
#define JOYSTICK_X_MIN 851
#define JOYSTICK_X_MAX 2842
#define JOYSTICK_Y_MIN 939
#define JOYSTICK_Y_MAX 2862

#endif
