#include "tm1637.h"

#include <stdio.h>
#include <inttypes.h>

#define TM1637_COMM1 0x40
#define TM1637_COMM2 0xC0
#define TM1637_COMM3 0x80

#define TM1637_DIGITS 4

// map digits to segments
const uint8_t digit_masks[] = {
  //XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

static void delay(void) {
	delay_us(1);
}

static void start(tm1637_t *tm) {
  write_pin(tm->port, tm->dio, false);
  delay();
}

static void stop(tm1637_t *tm) {
	write_pin(tm->port, tm->dio, false);
	delay();
	write_pin(tm->port, tm->clk, true);
	delay();
	write_pin(tm->port, tm->dio, true);
	delay();
}

static void read_ack(tm1637_t *tm) {
  // don't actually read, emulate
  write_pin(tm->port, tm->clk, false);
  write_pin(tm->port, tm->dio, true);
  delay();

  write_pin(tm->port, tm->clk, true);
  delay();

  write_pin(tm->port, tm->dio, false);
  delay();

  write_pin(tm->port, tm->clk, false);
  delay();
}

static void write(tm1637_t *tm, uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    write_pin(tm->port, tm->clk, false);
    delay();

    write_pin(tm->port, tm->dio, byte & (1 << i));
    delay();

    write_pin(tm->port, tm->clk, true);
    delay();
  }

  read_ack(tm);
}

static void write_buffer(tm1637_t *tm, uint8_t buffer[], uint8_t length, uint8_t pos) {
  start(tm);
  write(tm, 0x40);
  stop(tm);

  start(tm);
  write(tm, 0xc0 + (pos & 0x3));

  for (uint8_t i = 0; i < length; i++) {
    write(tm, buffer[i]);
  }

  stop(tm);

  start(tm);
  write(tm, 0x80 + (tm->brightness & 0xf));
  stop(tm);
}

void tm1637_init(tm1637_t *tm, port_t *port, pin_t clk, pin_t dio) {
  tm->port = port;
  tm->clk = clk;
  tm->dio = dio;
  tm->brightness = 8;

  write_pin(port, clk, false);
  write_pin(port, dio, false);
}

void tm1637_display(tm1637_t *tm, int32_t value, bool separator) {
  // extract digits from value
  uint8_t digits[4];

  for (int8_t i = sizeof(digits) - 1; i >= 0; i--) {
    digits[i] = digit_masks[value % 10];
    value /= 10;

    // enable central separator?
    if (separator && i + 1 == sizeof(digits) / 2) {
      digits[i] |= 1 << 7;
    }
  }

  // write segments
  write_buffer(tm, digits, sizeof(digits), 0);
}

void tm1637_brightness(tm1637_t *tm, uint8_t brightness) {
  tm->brightness = brightness;
}
