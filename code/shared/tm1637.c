#include "tm1637.h"

#include <stdio.h>

#define TM1637_COMM1 0x40
#define TM1637_COMM2 0xC0
#define TM1637_COMM3 0x80

#define TM1637_DIGITS 4

// map digits to segments
const uint8_t digit_map[] = {
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

// pin delay
static void delay(void) {
  HAL_Delay(1);
}

// start transfer -- lower data I/O pin
static void tm1637_start(tm1637_t *tm) {
  write_pin(tm->port, tm->dio, false);
  delay();
}

// end transfer -- raise data I/O pin
static void tm1637_stop(tm1637_t *tm) {
  write_pin(tm->port, tm->dio, false);
  delay();

  write_pin(tm->port, tm->clk, true);
  delay();

  write_pin(tm->port, tm->dio, true);
  delay();
}

// send a byte to the TM1637 module, return ACK
static bool tm1637_write(tm1637_t *tm, uint8_t data) {
  // oscillate DIO and clock while sending data
  for (uint8_t i = 0, k = 1; i < sizeof(data); i++, k <<= 1) {
    write_pin(tm->port, tm->clk, false);
    delay();

    write_pin(tm->port, tm->dio, data & k);
    delay();

    write_pin(tm->port, tm->clk, true);
    delay();
  }

  // wait for ACK
  write_pin(tm->port, tm->clk, false);
  write_pin(tm->port, tm->dio, true);
  delay();

  write_pin(tm->port, tm->clk, true);
  delay();

  // read ACK
  bool ack = read_pin(tm->port, tm->dio);

  if (!ack) {
    write_pin(tm->port, tm->dio, false);
  }

  delay();

  write_pin(tm->port, tm->clk, false);
  delay();

  return ack;
}

// write data `data` starting at `pos` digit address
static void tm1637_write_buffer(tm1637_t *tm, uint8_t *data, uint8_t size, uint8_t pos) {
  // COMM1
  tm1637_start(tm);
  tm1637_write(tm, TM1637_COMM1);
  tm1637_stop(tm);

  // COMM2 + first digit address, data
  tm1637_start(tm);
  tm1637_write(tm, TM1637_COMM2 + (pos & 0x03));

  for (uint8_t i = 0; i < size; i++) {
    write_byte(tm, data[i]);
  }

  tm1637_stop(tm);

  // COMM3 + brightness
  tm1637_start(tm);
  tm1637_write(tm, TM1637_COMM3 + (tm->brightness & 0xF));
  tm1637_stop(tm);
}

void tm1637_init(tm1637_t *tm, port_t *port, pin_t clk, pin_t dio) {
  tm->port = port;
  tm->clk = clk;
  tm->dio = dio;
  tm->brightness = 0;

  // set pins to default values
  write_pin(port, clk, false);
  write_pin(port, dio, false);

  // set max brightness
  tm1637_brightness(tm, 7);
}

void tm1637_brightness(tm1637_t *tm, uint8_t brightness, bool on) {
  tm->brightness = (brightness & 0x7) | (on ? 0x08 : 0x00);
}

void tm1637_write_int(tm1637_t *tm, int32_t digit, uint8_t pos) {
  char str[7];
  uint8_t buffer[TM1637_DIGITS] = {0};

  // convert int to str
  snprintf(str, sizeof(str), "%d", digit);

  // convert to buffer
  for (uint8_t i = 0; i < sizeof(buffer); i++) {
    if (str[i] == '-') {
      buffer[i] = 0x40;
    } else if (str[i] >= '0' && str[i] <= '9') {
      buffer[i] = digit_map[str[i] - '0'];
    }
  }

  // write buffer to module
  tm1637_write_buffer(tm, buffer, sizeof(buffer), pos);
}

void tm1637_write_float(tm1637_t *tm, float digit, uint8_t dp, uint8_t pos) {
  char str[8];
  uint8_t buffer[TM1637_DIGITS] = {0};

  // format string for printf
  char format_str[] = "%.\0f";
  format_str[2] = '0' + dp;

  // convert float to str
  snprintf(str, sizeof(str), format_str, digit);

  // convert to buffer
  for (uint8_t i = 0, j = 0; i <= sizeof(buffer); i++) {
    if (str[i] == '-') {
      buffer[j++] = 0x40;
    } else if (str[i] >= '0' && str[i] <= '9') {
      buffer[j++] = digit_map[str[i] - '0'];
    } else if (str[i] == '.') {
      if (j > 0) {
        buffer[j - 1] |= 0x80;
      }
    } else {
      buffer[j] = 0;
      break;
    }
  }

  // write buffer to module
  tm1637_write_buffer(tm, buffer, sizeof(buffer), pos);
}
