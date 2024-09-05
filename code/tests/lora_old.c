#include "lora_old.h"

#define REG_FIFO_ACCESS 0x00

#define REG_OP_MODE 0x01
#define MODE_LONG_RANGE 0x80
#define MODE_LOW_FREQ  0x08

#define REG_FR_MSB 0x06
#define REG_FR_MID 0x07
#define REG_FR_LSB 0x08
#define REG_PA_CONF 0x09
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_FIFO_CURR_ADDR_PTR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_TX_BASE 0x0E
#define TX_BASE_ADDR 0x80
#define REG_RX_BASE 0x0F
#define REG_RX_CURR_ADDR 0x10
#define RX_BASE_ADDR 0x00
#define REG_NUMBER_OF_BYTES 0x13
#define REG_MODEM_STAT 0x18
#define REG_CONF_1 0x1D
#define REG_CONF_2 0x1E
#define REG_PREAMBLE_MSB 0x20
#define REG_PREAMBLE_LSB 0x21
#define REG_PAYLOAD_LEN 0x22
#define REG_MAX_PAYLOAD_LEN 0x23
#define REG_CONF_3 0x26
#define REG_DIO_MAP1 0x40
#define REG_PA_DAC 0x4D

// crystal oscillator frequency (32MHz for RFM95-8)
#define LORA_FXOSC 32000000

uint8_t g_lora_buffer[LORA_MAX_PAYLOAD_SIZE];

// config table, see defined constants LORA_CONFIG_ for indexes
uint8_t configs[][3] = {
  // MC1, MC2, MC3
  {0x72, 0x70, 0x00},
  {0x92, 0x74, 0x00},
  {0x48, 0x94, 0x00},
  {0x78, 0xC4, 0x00},
  {0x73, 0x74, 0x00}
};

// start transaction - reset NSS
static void start_transaction(lora_t *lora) {
  write_pin(lora->nss_port, lora->nss_pin, false);
  HAL_Delay(1);
}

// end transaction - set NSS
static void end_transaction(lora_t *lora) {
  write_pin(lora->nss_port, lora->nss_pin, true);
  HAL_Delay(1);
}

// SPI - transmit data
static inline void spi_transmit(lora_t *lora, uint8_t *data, uint16_t size) {
  HAL_SPI_Transmit(lora->spi, data, size, LORA_TIMEOUT);
}

// SPI - recieve data
static inline void spi_receive(lora_t *lora, uint8_t *data, uint16_t size) {
  HAL_SPI_Receive(lora->spi, data, size, LORA_TIMEOUT);
}

// SPI - recieve data in interrupt mode
static inline void spi_receive_it(lora_t *lora, uint8_t *data, uint16_t size) {
  HAL_SPI_Receive_IT(lora->spi, data, size);
}

// read bytes from the given address
 void read_bytes(lora_t *lora, uint8_t address, uint8_t *buffer, uint16_t size) {
  // command - request address to read
  uint8_t cmd = address & 0x7F;

  start_transaction(lora);
  spi_transmit(lora, &cmd, 1);
  spi_receive(lora, buffer, size);
  end_transaction(lora);
}

// write bytes starting at the given address
void write_bytes(lora_t *lora, uint8_t address, uint8_t *data, uint8_t size) {
  // command - tell device to write to address
  uint8_t cmd = 0x80 | address;

  start_transaction(lora);
  spi_transmit(lora, &cmd, 1);
  spi_transmit(lora, data, size);
  end_transaction(lora);
}

// write byte to the given address
void write_byte(lora_t *lora, uint8_t address, uint8_t value) {
  write_bytes(lora, address, &value, 1);
}


void lora_setup(lora_t *lora, SPI_HandleTypeDef *spi, port_t *nss_port, pin_t nss_pin, port_t *reset_port, pin_t reset_pin) {
  lora->spi = spi;
  lora->nss_port = nss_port;
  lora->nss_pin = nss_pin;
  lora->reset_port = reset_port;
  lora->reset_pin = reset_pin;
  lora->dio_mapping = 0;
  
  // set reset to high and ensure wait time
  write_pin(reset_port, reset_pin, true);
  lora_reset(lora);

  // ensure select is high
  end_transaction(lora);

  // enter into LoRa mode
  HAL_Delay(10);
  write_byte(lora, REG_OP_MODE, MODE_SLEEP);
  write_byte(lora, REG_OP_MODE, MODE_LONG_RANGE | MODE_SLEEP);
  
  // set up FIFO base addresses
  write_byte(lora, REG_RX_BASE, RX_BASE_ADDR);
  write_byte(lora, REG_TX_BASE, TX_BASE_ADDR);
  lora_mode_standby(lora);

  lora_configure_modem(lora, LORA_CONFIG_BW125_CR45_SF128);
  //lora_set_preamble_length(lora, 8);
  //lora_set_frequency(lora, LORA_DEFAULT_FREQ);
  //lora_set_tx_power(lora, 15, false);
  write_byte(lora, REG_MAX_PAYLOAD_LEN, LORA_MAX_PAYLOAD_SIZE);
}

void lora_reset(lora_t *lora) {
  write_pin(lora->reset_port, lora->reset_pin, false);
  HAL_Delay(1);
  write_pin(lora->reset_port, lora->reset_pin, true);
  HAL_Delay(5);
}

void lora_configure_modem(lora_t *lora, uint8_t config_idx) {
  uint8_t *config = configs[config_idx];

  write_byte(lora, REG_CONF_1, config[0]);
  write_byte(lora, REG_CONF_2, config[1]);
  write_byte(lora, REG_CONF_3, config[2]);
}

void lora_set_tx_power(lora_t *lora, int8_t power, bool use_rfo) {
  // if using RFO, configure differently
  if (use_rfo) {
    CLAMP(power, -1, 14)
    write_byte(lora, REG_PA_CONF, 0x70 | (power + 1));
    return;
  }

  CLAMP(power, 5, 23)

  // enable PA DAC if power > 20dBm
  if (power > 20) {
    write_byte(lora, REG_PA_DAC, 0x07);
    power -= 3;
  } else {
    write_byte(lora, REG_PA_DAC, 0x04);
  }

  write_byte(lora, REG_PA_CONF, 0x80 | (power - 5));
}

void lora_maximise_tx_power(lora_t *lora) {
  write_byte(lora, REG_PA_CONF, 0xFF);
  write_byte(lora, REG_PA_DAC, 0x07);
}

void lora_set_frequency(lora_t *lora, uint32_t hz) {
  // device must be SLEEP or STANDBY for adjustment to take effect
  lora_mode_standby(lora);

  // f_RF = (XOSC * Fr_f) / 2^19
  uint64_t frf = (uint64_t)hz * (LORA_FXOSC / 524288);
  write_byte(lora, REG_FR_MSB, (frf >> 16) & 0xFF);
  write_byte(lora, REG_FR_MID, (frf >> 8) & 0xFF);
  write_byte(lora, REG_FR_LSB, frf & 0xFF);
}

void lora_set_preamble_length(lora_t *lora, uint16_t bytes) {
  write_byte(lora, REG_PREAMBLE_MSB, bytes >> 8);
  write_byte(lora, REG_PREAMBLE_LSB, bytes & 0xff);
}

uint8_t lora_rx_size(lora_t *lora) {
  uint8_t size;
  read_bytes(lora, REG_NUMBER_OF_BYTES, &size, 1);
  return size;
}

void lora_rx_reset_buffer(lora_t *lora) {
  write_byte(lora, REG_FIFO_ADDR_PTR, 0);
}

void lora_rx_point_next_packet(lora_t *lora) {
  // reset FIFO address ptr to start of next packet
  uint8_t start_addr;
  read_bytes(lora, REG_FIFO_CURR_ADDR_PTR, &start_addr, 1);
  write_byte(lora, REG_FIFO_ADDR_PTR, start_addr);
}

void lora_prepare_receive(lora_t *lora) {
  lora_mode_standby(lora);

  // set ptr to rx base
  uint8_t rx_base;
  read_bytes(lora, REG_RX_BASE, &rx_base, 1);
  write_bytes(lora, REG_FIFO_ADDR_PTR, &rx_base, 1);
}

uint8_t lora_read_fifo(lora_t *lora, uint8_t *buffer, uint8_t max_size) {
  // read payload length
  uint8_t size = lora_rx_size(lora);

  if (size > 0) {
    // clamp to max_size
    if (size > max_size) {
      size = max_size;
    }

    // set FifoPtrAddr = FifoRxCurrentAddr
    uint8_t rx_ptr;
    read_bytes(lora, REG_RX_CURR_ADDR, &rx_ptr, 1);
    write_byte(lora, REG_FIFO_ADDR_PTR, rx_ptr);

    for (uint8_t i = 0; i < size; ++i) {
    	read_bytes(lora, REG_FIFO_ACCESS, buffer + i, 1);
    }
  }

  return size;
}

void lora_send(lora_t *lora, uint8_t *buffer, uint8_t size) {
  // move to standby mode & clear IRQ flags
  lora_mode_standby(lora);
  lora_irq_clear(lora);

  // set pointer to TX base
  write_byte(lora, REG_FIFO_ADDR_PTR, TX_BASE_ADDR);

  // set payload length
  write_byte(lora, REG_PAYLOAD_LEN, size);
  
  // write payload buffer
  write_bytes(lora, REG_FIFO_ACCESS, buffer, size);

  // set to TX (transmit) mode
  // LoRa will switch to STANDBY when done
  lora_mode_tx(lora);
}

uint8_t lora_mode(lora_t *lora) {
  uint8_t reg;
  read_bytes(lora, REG_OP_MODE, &reg, 1);
  return reg & 0x7;
}

// set mode register (uses OR to preserve 0xF8 & current)
static void lora_set_mode(lora_t *lora, uint8_t mode) {
  uint8_t flag;
  read_bytes(lora, REG_OP_MODE, &flag, 1);

  // write: preserve all but lower `mode` bits
  write_byte(lora, REG_OP_MODE, (flag & 0xF8) | mode);
}

void lora_mode_standby(lora_t *lora) {
  lora_set_mode(lora, MODE_STANDBY);
}

void lora_mode_sleep(lora_t *lora) {
  lora_set_mode(lora, MODE_SLEEP);
}

void lora_mode_rx(lora_t *lora, bool continuous) {
  lora->dio_mapping = DIO_MAP_RX_DONE;
  write_byte(lora, REG_DIO_MAP1, DIO_MAP_RX_DONE << 6); // interrupt DIO0 on RxDone
  lora_set_mode(lora, continuous ? MODE_RXCONTINUOUS : MODE_RXSINGLE);
}

void lora_mode_tx(lora_t *lora) {
  lora->dio_mapping = DIO_MAP_TX_DONE;
  write_byte(lora, REG_DIO_MAP1, DIO_MAP_TX_DONE << 6); // interrupt DIO0 on TxDone
  lora_set_mode(lora, MODE_TX);
}

void lora_mode_cad(lora_t *lora) {
  lora_set_mode(lora, MODE_CAD);
  lora->dio_mapping = DIO_MAP_CAD_DONE;
  write_byte(lora, REG_DIO_MAP1, DIO_MAP_CAD_DONE << 6); // interrupt DIO0 on CadDone
}

uint8_t lora_irq(lora_t *lora) {
  uint8_t flags;
  read_bytes(lora, REG_IRQ_FLAGS, &flags, 1);
  return flags;
}

void lora_irq_clear(lora_t *lora) {
  // flags clear on write
  write_byte(lora, REG_IRQ_FLAGS, 0xFF);
}

bool lora_test_modem_status(lora_t *lora, uint8_t mask) {
  uint8_t stats;
  read_bytes(lora, REG_MODEM_STAT, &stats, 1);
  return stats & mask;
}
