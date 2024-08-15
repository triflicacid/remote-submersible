#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define LORA_PROVIDE_GLOBAL_BUFFER

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

// request-code button
#define REQUEST_CODE_PORT GPIOC
#define REQUEST_CODE_PIN GPIO_PIN_2

// 7 segment display pins
#define DISPLAY_PORT GPIOA
#define DISPLAY_DIO GPIO_PIN_0
#define DISPLAY_CLK GPIO_PIN_1

#endif
