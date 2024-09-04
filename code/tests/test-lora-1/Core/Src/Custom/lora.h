#ifndef _LORA_H_
#define _LORA_H_

#include "util.h"

typedef struct {
  SPI_HandleTypeDef *spi;
  port_t *ss_port;
  pin_t ss_pin;
  port_t *rst_port;
  pin_t rst_pin;

  void (*on_receive)(int); // rceived `size` bytes
  void (*on_cad_done)(bool); // CAD done: arg indicates if activity detected
  void (*on_tx_done)(void);

  uint32_t _pos; // packet position
  bool _implicit_header; // implicit header mode?
} lora_t;

// initialise LoRa struct
void lora_init(lora_t *lora, SPI_HandleTypeDef *spi, port_t *ss_port, pin_t ss_pin, port_t *rst_port, pin_t rst_pin);

// begin transmission of a packet
// followed by variable reads, then end
// this is blocking
void lora_begin_packet(lora_t *lora, bool implicitHeader);

// end packet transmission; wrap up and send
// async: configure DIO interrupt pin if true, else blocking (doesn't call on_receive)
void lora_end_packet(lora_t *lora, bool async);

// are we currently transmitting anything?
bool lora_is_transmitting(lora_t *lora);

// parse packet, aka, setup for read
// size: expected packet size. if 0, determine size dynamically in explicit mode, else implicit mode
// returns packet size
// if not in RX mode, sets to RXSINGLE
int lora_parse_packet(lora_t *lora, int size);

// write byte byte to FIFO
size_t lora_write(lora_t *lora, uint8_t byte);

// write buffer to FIFO
size_t lora_write_bytes(lora_t *lora, const uint8_t *buffer, size_t size);

// get number of available bytes in FIFO
int lora_available(lora_t *lora);

// read one byte from FIFO
uint8_t lora_read(lora_t *lora);

// peek topmost byte from FIFO
uint8_t lora_peek(lora_t *lora);

// request to receive `size` bytes, place in RX mode with DIO interrupt mode
void lora_receive(lora_t *lora, int size);

// put into CAD mode
void lora_cad(lora_t *lora);

// set to IDLE (standby) mode
void lora_idle(lora_t *lora);

// set to SLEEP
void lora_sleep(lora_t *lora);

// set transfer power
// if >17, enable PA_BOOST
void lora_set_tx_power(lora_t *lora, int level);

void lora_set_frequency(lora_t *lora, long frequency);

long lora_get_frequency(lora_t *lora);

uint8_t lora_get_spreading_factor(lora_t *lora);

void lora_set_spreading_factor(lora_t *lora, int sf);

long lora_get_signal_bandwidth(lora_t *lora);

void lora_set_signal_bandwidth(lora_t *lora, long sbw);

void lora_set_ldo_flag(lora_t *lora);

void lora_set_ldo_flag_force(lora_t *lora, bool ldoOn);

// set coding rate to 4/<denom>
void lora_set_coding_rate(lora_t *lora, int denominator);

void lora_set_preamble_length(lora_t *lora, long length);

void lora_set_sync_word(lora_t *lora, uint8_t sw);

void lora_enable_crc(lora_t *lora);

void lora_disable_crc(lora_t *lora);

void lora_enable_invert_iq(lora_t *lora);

void lora_disable_invert_iq(lora_t *lora);

void lora_set_ocp(lora_t *lora, uint8_t mA);

void lora_set_gain(lora_t *lora, uint8_t gain);

// handle DIO0 interrupt
void lora_dio_interrupt(lora_t *lora);
#endif
