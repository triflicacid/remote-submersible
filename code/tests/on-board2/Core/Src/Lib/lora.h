#ifndef _LORA_H_
#define _LORA_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef LORA_MAX_PAYLOAD_SIZE
// LoRa maximum payload size (uint8_t)
#define LORA_MAX_PAYLOAD_SIZE 64
#endif

#ifndef LORA_DEFAULT_FREQ
// LoRa device default frequency in Hz
#define LORA_DEFAULT_FREQ 434000000
#endif

#ifndef LORA_TIMEOUT
// LoRa device timeout on transmit/receive in ms
#define LORA_TIMEOUT 100
#endif

// config: 125KHz bandwidth, 4/5 error coding rate, 128 chips/symbol, CRC enabled. Default medium range.
#define LORA_CONFIG_BW125_CR45_SF128 0

// config: 500KHz bandwidth, 4/5 error coding rate, 128 chips/symbol, CRC enabled. Fast, short range..
#define LORA_CONFIG_BW500_CR45_SF128 1

// config: 31.25KHz bandwidth, 4/8 error coding rate, 512 chips/symbol, CRC enabled. Slow, long range.
#define LORA_CONFIG_BW31_25_CR48_SF512 2

// config: 125KHz bandwidth, 4/8 error coding rate, 4096 chips/symbol, CRC enabled. Slow, long range.
#define LORA_CONFIG_BW125_CR48_SF4096 3

// config: 125KHz bandwidth, 4/8 error coding rate, 4096 chips/symbol, CRC enabled. Default medium range with implicit headers.
#define LORA_CONFIG_BW125_CR48_SF4096_IMPL 4

#define IRQ_RX_TIMEOUT 0x80
#define IRQ_RX_DONE 0x40
#define IRQ_CRC_ERROR 0x20
#define IRQ_VALID_HEADER 0x10
#define IRQ_RF95_TX_DONE 0x08
#define IRQ_RF95_CAD_DONE 0x04
#define IRQ_RF95_FHSS_CHANGE_CHANNEL 0x02
#define IRQ_CAD_DETECTED 0x01

#define MODEM_STATUS_CLEAR 0x10
#define MODEM_STATUS_HEADER_INFO_VALID 0x08
#define MODEM_STATUS_RX_ONGOING 0x04
#define MODEM_STATUS_SIGNAL_SYNCHRONIZED 0x02
#define MODEM_STATUS_SIGNAL_DETECTED 0x01

#define MODE_SLEEP 0x00
#define MODE_STANDBY 0x01
#define MODE_TX 0x03
#define MODE_RXCONTINUOUS 0x05
#define MODE_RXSINGLE 0x06
#define MODE_CAD 0x07

// global buffer of size `MAX_PAYLOAD_SIZE` for utility purposes
extern uint8_t g_lora_buffer[LORA_MAX_PAYLOAD_SIZE];

#include "util.h"

// represents a LoRa-capable device
// !DO NOT ACCESS MANUALLY
typedef struct {
  SPI_HandleTypeDef *spi;
  port_t *nss_port;
  pin_t nss_pin;
  port_t *reset_port;
  pin_t reset_pin;
} lora_t;

// setup the given LoRa device using the LORA_CONFIG_* configuration
void lora_setup(lora_t *lora, SPI_HandleTypeDef *spi, port_t *nss_port, pin_t nss_pin, port_t *reset_port, pin_t reset_pin);

// reset LoRa device
void lora_reset(lora_t *lora);

// set modem config (LORA_CONFIG_* value)
void lora_configure_modem(lora_t *lora, uint8_t config);

// set TX power
void lora_set_tx_power(lora_t *lora, int8_t power, bool use_rfo);

// max-out TX power (uses PA_BOOST)
void lora_maximise_tx_power(lora_t *lora);

// set frequency in Hz
void lora_set_frequency(lora_t *lora, uint32_t hz);

// set preamble length
void lora_set_preamble_length(lora_t *lora, uint16_t bytes);

// get number of unread bytes in RX buffer
uint8_t lora_rx_size(lora_t *lora);

// reset address of FIFO RX buffer
void lora_rx_reset_buffer(lora_t *lora);

// move receive pointer to start of next packet
// TIP: do this to ensure reading correct packet on RxDone
void lora_rx_point_next_packet(lora_t *lora);

// read full RX FIFO buffer from device
// read maximum `max_size` bytes into `buffer` and return its size
// NOTE IRQ flags are not cleared
uint8_t lora_receive(lora_t *lora, uint8_t *buffer, uint8_t max_size);

// read first `size` bytes of RX FIFO buffer from device in non-blocking (interrupt) mode
// call RxCompltCallback when done
// NOTE IRQ flags are not cleared
void lora_receive_async(lora_t *lora, uint8_t *buffer, uint16_t size);

// send message using LoRa device
// states: set to STANDY, then set to TX. Automatically set to STANDBY on trnsmission completion
void lora_send(lora_t *lora, uint8_t *buffer, uint8_t size);

// read LoRa device mode
// (isolates low 0-2 bits to allow direct comparisons with `MODE_*` constants)
uint8_t lora_mode(lora_t *lora);

// set to standby mode
void lora_mode_standby(lora_t *lora);

// set to sleep mode (low powered)
void lora_mode_sleep(lora_t *lora);

// set to receive: continuous or single mode?
// RXCONTINUOUS remains in RXCONTINUOUS even after RxDone is set
// RXSINGLE switches automatically to STANDBY on RxDone set
void lora_mode_rx(lora_t *lora, bool continuous);

// set to transmit mode
void lora_mode_tx(lora_t *lora);

// set to CAD mode
void lora_mode_cad(lora_t *lora);

// check if IRQ flag `IRQ_*` is set
bool lora_test_irq(lora_t *lora, uint8_t mask);

// same as `lora_test_irq`, but clear masked bit(s) after
bool lora_test_irq_clear(lora_t *lora, uint8_t mask);

// clear all IRQ flags
void lora_irq_clear(lora_t *lora);

// check if modem state `MODEM_STATUS_*` is set
bool lora_test_modem_status(lora_t *lora, uint8_t mask);

void read_bytes(lora_t *lora, uint8_t address, uint8_t *buffer, uint16_t size);
void write_bytes(lora_t *lora, uint8_t address, uint8_t *data, uint8_t size);
void write_byte(lora_t *lora, uint8_t address, uint8_t value);

#endif
