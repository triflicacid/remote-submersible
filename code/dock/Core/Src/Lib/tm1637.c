#include "tm1637.h"

#define TM1637_WRITE  0x40
#define TM1637_ADDR   0xc0
#define TM1637_BRIGHT 0x80
#define TIM1637_DP    0x80

// map digits to segments
static const uint8_t digit_masks[] = {
  //GFEDCBA
  0b0111111,    // 0
  0b0000110,    // 1
  0b1011011,    // 2
  0b1001111,    // 3
  0b1100110,    // 4
  0b1101101,    // 5
  0b1111101,    // 6
  0b0000111,    // 7
  0b1111111,    // 8
  0b1101111,    // 9
  0b1110111,    // A
  0b1111100,    // b
  0b0111001,    // C
  0b1011110,    // d
  0b1111001,    // E
  0b1110001     // F
};

static void delay(void) {
  delay_us(5);
}

static void start(tm1637_t *tm) {
  reset_pin(tm->dio);
  delay();
}

static void stop(tm1637_t *tm) {
  reset_pin(tm->dio);
  delay();
  set_pin(tm->clk);
  delay();
  set_pin(tm->dio);
  delay();
}

static void read_ack(tm1637_t *tm) {
  // don't actually read, emulate
  reset_pin(tm->clk);
  set_pin(tm->dio);
  delay();

  set_pin(tm->clk);
  delay();

  reset_pin(tm->dio);
  delay();

  reset_pin(tm->clk);
  delay();
}

static void write(tm1637_t *tm, uint8_t byte) {
  for (uint8_t i = 0, mask = 0; i < 8; i++, mask <<= 1) {
    reset_pin(tm->clk);
    delay();

    write_pin(tm->dio, byte & mask);
    delay();

    set_pin(tm->clk);
    delay();
  }

  read_ack(tm);
}

static void write_buffer(tm1637_t *tm, const uint8_t buffer[], uint8_t length, uint8_t pos) {
  // control byte - enter normal writing mode
  start(tm);
  write(tm, TM1637_WRITE);
  stop(tm);

  // command - set writing address to `pos`
  start(tm);
  write(tm, TM1637_ADDR | (pos & 0x3));

  //send sequence of bytes
  for (uint8_t i = 0; i < length; i++) {
    write(tm, buffer[i]);
  }

  stop(tm);

  // command - set brightness level to cached `brightness`
  start(tm);
  write(tm, TM1637_BRIGHT | (tm->brightness & 0xf));
  stop(tm);
}

void tm1637_init(tm1637_t *tm, const pin_t *clk, const pin_t *dio) {
  tm->clk = clk;
  tm->dio = dio;
  tm->brightness = 8;

  // lower both pins
  reset_pin(clk);
  reset_pin(dio);
}

void tm1637_display(tm1637_t *tm, uint32_t value, bool separator) {
  // extract digits from value
  uint8_t digits[4];

  for (int8_t i = sizeof(digits) - 1; i >= 0; i--) {
    digits[i] = digit_masks[value % 10];
    value /= 10;

    // enable central separator?
    if (separator && i + 1 == sizeof(digits) / 2) {
      digits[i] |= TIM1637_DP;
    }
  }

  // write segments
  write_buffer(tm, digits, sizeof(digits), 0);
}

void tim1637_brightness(tm1637_t *tm, uint8_t brightness) {
  tm->brightness = brightness;
}
