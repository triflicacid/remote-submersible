#ifndef _LORA_H_
#define _LORA_H_

#include <stdint.h>

typedef struct lora_t lora_t;

// setup the given LoRa device
void lora_setup(lora_t *lora, SPI_InitTypeDef *spi, GPIO_TypeDef *nss_port, uint16_t nss_pin);

// receive message from LoRa device
// insert message into `buffer`, message size into `size` (clamped by `max_size`)
void lora_receive(lora_t *lora, uint8_t *buffer, uint8_t *size, uint8_t max_size);

// send message using LoRa device
void lora_send(lora_t *lora, uint8_t *buffer, uint8_t size);

#endif
