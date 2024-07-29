#include "7-segment.h"

#include <stdlib.h>

#define MSP_TIMEOUT 100
#define MSP_ADDR_GPIOA 0x12
#define MSP_ADDR_GPIOB 0x13

// a = bit 0, g = bit 6
static uint8_t segment_mask[] = { 0x3F,  0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
static uint8_t dp_mask = 1 << 7;

// get control byte
inline uint8_t get_control_byte(bool write, uint8_t hw_addr) {
  return 0x40 | ((hw_addr << 1) & 0xE) | (write & 1);
}

static void write(display_t *display, uint8_t unit, uint8_t address, uint8_t data) {
  ploc_t *select = display->units + unit;

  // select device
  write_pin(select->port, select->pin, false);

  uint8_t payload[] = {
    get_control_byte(true, display->hw_addr),
    address,
    data
  };

  HAL_SPI_Transmit(display->spi, &payload, sizeof(payload), MSP_TIMEOUT);

  // de-select device
  write_pin(display->units[unit].port, display->units[unit].pin, true);
}

static uint8_t read(display_t *display, uint8_t unit, uint8_t address) {
  ploc_t *select = display->units + unit;

  // select device
  write_pin(select->port, select->pin, false);

  uint8_t payload[] = {
    get_control_byte(true, display->hw_addr),
    address
  };

  HAL_SPI_Transmit(display->spi, &payload, sizeof(payload), MSP_TIMEOUT);

  // read data sent back
  uint8_t data;
  HAL_SPI_Receive(display->spi, &data, sizeof(data), MSP_TIMEOUT);

  // de-select device
  write_pin(display->units[unit].port, display->units[unit].pin, true);
}

void display_init(display_t *display, SPI_HandleTypeDef *spi, ploc_t *units, uint8_t digit_count) {
  uint8_t unit_count = (digit_count + 1) / 2;

  display->spi = spi;
  display->digit_count = digit_count;
  display->units = malloc(sizeof(ploc_t) * unit_count);

  for (uint8_t i = 0; i < unit_count; i++) {
    display->units[i] = units[i];
  }
}

void display_write(display_t *display, uint64_t value, uint32_t decimal_points) {
  // iterate, extracting and writing digits
  for (uint16_t i = 0, k = 1; i < display->digit_count; i++, k <<= 1) {
    display_write_digit(display, i, value % 10, decimal_points & k);
    value /= 10;
  }
}

void display_write_digit(display_t *display, uint8_t segment, uint8_t value, bool decimal_point) {
  if (segment >= display->digit_count || value > 9) {
    return;
  }

  // get unit location and determine port (for register address)
  uint8_t unit = segment / 2;
  bool port_a = segment % 2;

  // calculate register data 
  uint8_t data = segment_mask[value];

  if (decimal_point) {
    data |= dp_mask;
  }

  // write to correct register
  write(display, unit, port_a ? MSP_ADDR_GPIOA : MSP_ADDR_GPIOB, data);
}

void display_clear(display_t *display) {
  for (uint16_t i = 0; i < display->digit_count; i++) {
    write(display, i / 2, i % 2 ? MSP_ADDR_GPIOB : MSP_ADDR_GPIOA, 0);
  }
}
