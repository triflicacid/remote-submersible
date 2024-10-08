#include "lora.h"

#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_RSSI_VALUE           0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06
#define MODE_CAD                 0x07

#define PA_BOOST                 0x80

#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40
#define IRQ_CAD_DONE_MASK          0x04
#define IRQ_CAD_DETECTED_MASK      0x01

#define RF_MID_BAND_THRESHOLD    525E6
#define RSSI_OFFSET_HF_PORT      157
#define RSSI_OFFSET_LF_PORT      164

#define MAX_PKT_LENGTH           255

static uint8_t _read(lora_t *lora, uint8_t address) {
  // command - request address to read
  uint8_t cmd = address & 0x7F, data;

  write_pin(lora->ss_port, lora->ss_pin, false);
  HAL_SPI_Transmit(lora->spi, &cmd, 1, 100);
  HAL_SPI_Receive(lora->spi, &data, 1, 100);
  write_pin(lora->ss_port, lora->ss_pin, true);

  return data;
}

static void _write_bytes(lora_t *lora, uint8_t address, const uint8_t *data, uint8_t size) {
  // command - tell device to write to address
  uint8_t cmd = 0x80 | address;

  write_pin(lora->ss_port, lora->ss_pin, false);
  HAL_SPI_Transmit(lora->spi, &cmd, 1, 100);
  HAL_SPI_Transmit(lora->spi, data, size, 100);
  write_pin(lora->ss_port, lora->ss_pin, true);
}

static void _write(lora_t *lora, uint8_t address, uint8_t data) {
  _write_bytes(lora, address, &data, 1);
}

static void explicit_header_mode(lora_t *lora) {
  lora->_implicit_header = false;
  _write(lora, REG_MODEM_CONFIG_1, _read(lora, REG_MODEM_CONFIG_1) & 0xfe);
}

static void implicit_header_mode(lora_t *lora) {
  lora->_implicit_header = true;
  _write(lora, REG_MODEM_CONFIG_1, _read(lora, REG_MODEM_CONFIG_1) | 0x01);
}


void lora_init(lora_t *lora, SPI_HandleTypeDef *spi, port_t *ss_port, pin_t ss_pin, port_t *rst_port, pin_t rst_pin) {
  lora->spi = spi;
  lora->ss_port = ss_port;
  lora->ss_pin = ss_pin;
  lora->rst_port = rst_port;
  lora->rst_pin = rst_pin;
  lora->on_receive = NULL;
  lora->on_cad_done = NULL;
  lora->on_tx_done = NULL;

  // set SS high
  write_pin(lora->ss_port, lora->ss_pin, true);

  // toggle reset
  write_pin(lora->rst_port, lora->rst_pin, false);
  HAL_Delay(5);
  write_pin(lora->rst_port, lora->rst_pin, true);
  HAL_Delay(5);

  // put in sleep mode
  lora_sleep(lora);

  // set base addresses
  _write(lora, REG_FIFO_TX_BASE_ADDR, 0);
  _write(lora, REG_FIFO_RX_BASE_ADDR, 0);

  // set LNA boost
  _write(lora, REG_LNA, _read(lora, REG_LNA) | 0x03);

  // set auto AGC
  _write(lora, REG_MODEM_CONFIG_3, 0x04);

  // set to +17dBm
    lora_set_tx_power(lora, 17);

  // put in standby mode
  lora_idle(lora);
}

void lora_begin_packet(lora_t *lora, bool implicitHeader) {
  // put in standby mode
  lora_idle(lora);

  if (implicitHeader) {
    implicit_header_mode(lora);
  } else {
    explicit_header_mode(lora);
  }

  // reset FIFO address and payload length
  _write(lora, REG_FIFO_ADDR_PTR, 0);
  _write(lora, REG_PAYLOAD_LENGTH, 0);
}

// write byte byte to FIFO
size_t lora_write(lora_t *lora, uint8_t byte) {
  return lora_write_bytes(lora, &byte, sizeof(byte));
}

size_t lora_write_bytes(lora_t *lora, const uint8_t *buffer, size_t size) {
  int current_length = _read(lora, REG_PAYLOAD_LENGTH);

  // write data
  //for (size_t i = 0; i < size; i++) {
  //  _write(lora, REG_FIFO, buffer[i]);
  //}
  _write_bytes(lora, REG_FIFO, buffer, size);

  // update length
  _write(lora, REG_PAYLOAD_LENGTH, current_length + size);

  return size;
}

void lora_end_packet(lora_t *lora, bool async) {
  if (async)
      _write(lora, REG_DIO_MAPPING_1, 0x40); // DIO0 -> TXDONE

  // put in TX mode
  _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

  if (!async) {
    // wait for TX done
    while (!(_read(lora, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK))
      ;

    // clear IRQ's
    _write(lora, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
  }
}

bool lora_is_transmitting(lora_t *lora) {
  if (_read(lora, REG_OP_MODE) & MODE_TX) {
    return true;
  }

  if (_read(lora, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) {
    // clear IRQ's
    _write(lora, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
  }

  return false;
}

int lora_parse_packet(lora_t *lora, int size) {
  int packet_length = 0;
  int irq_flags = _read(lora, REG_IRQ_FLAGS);

  if (size > 0) {
    implicit_header_mode(lora);
    _write(lora, REG_PAYLOAD_LENGTH, size & 0xff);
  } else {
    explicit_header_mode(lora);
  }

  // clear IRQ's
  _write(lora, REG_IRQ_FLAGS, irq_flags);

  if ((irq_flags & IRQ_RX_DONE_MASK) && !(irq_flags & IRQ_PAYLOAD_CRC_ERROR_MASK)) {
    // received a packet
    lora->_pos = 0;

    // read packet length
    if (lora->_implicit_header) {
      packet_length = _read(lora, REG_PAYLOAD_LENGTH);
    } else {
      packet_length = _read(lora, REG_RX_NB_BYTES);
    }

    // set FIFO address to current RX address
    _write(lora, REG_FIFO_ADDR_PTR, _read(lora, REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    lora_idle(lora);
  } else if (_read(lora, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)) {
    // not currently in RX mode

    // reset FIFO address
    _write(lora, REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
    _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
  }

  return packet_length;
}

// get number of available bytes in FIFO
int lora_available(lora_t *lora) {
  return _read(lora, REG_RX_NB_BYTES) - lora->_pos;
}

// read one byte from FIFO
uint8_t lora_read(lora_t *lora) {
  if (!lora_available(lora)) {
    return 0;
  }

  lora->_pos++;

  return _read(lora, REG_FIFO);
}

// peek topmost byte from FIFO
uint8_t lora_peek(lora_t *lora) {
  if (!lora_available(lora)) {
    return -1;
  }

  // store current FIFO address
  uint8_t address = _read(lora, REG_FIFO_ADDR_PTR);

  // read byte
  uint8_t b = _read(lora, REG_FIFO);

  // restore FIFO address
  _write(lora, REG_FIFO_ADDR_PTR, address);

  return b;
}

void lora_receive(lora_t *lora, int size) {
  _write(lora, REG_DIO_MAPPING_1, 0x00); // DIO0 -> RXDONE

  if (size > 0) {
    implicit_header_mode(lora);

    _write(lora, REG_PAYLOAD_LENGTH, size & 0xff);
  } else {
    explicit_header_mode(lora);
  }

  _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

void lora_cad(lora_t *lora) {
  _write(lora, REG_DIO_MAPPING_1, 0x80);// DIO0 -> CADDONE
  _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_CAD);
}

void lora_idle(lora_t *lora) {
  _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void lora_sleep(lora_t *lora) {
  _write(lora, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void lora_set_tx_power(lora_t *lora, int level) {
  if (level > 17) {
    if (level > 20) {
      level = 20;
    }

    level -= 3;

    // high power operation
    _write(lora, REG_PA_DAC, 0x87);
    lora_set_ocp(lora, 140);
  } else {
    if (level < 2) {
      level = 2;
    }

    _write(lora, REG_PA_DAC, 0x84);
    lora_set_ocp(lora, 100);
  }

  _write(lora, REG_PA_CONFIG, PA_BOOST | (level - 2));
}

void lora_set_frequency(lora_t *lora, long frequency) {
  uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

  _write(lora, REG_FRF_MSB, (uint8_t)(frf >> 16));
  _write(lora, REG_FRF_MID, (uint8_t)(frf >> 8));
  _write(lora, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

long lora_get_frequency(lora_t *lora) {
  uint64_t frf = (_read(lora, REG_FRF_MSB) << 16) | (_read(lora, REG_FRF_MID) << 8) | _read(lora, REG_FRF_LSB);

  return (frf * 32000000) >> 19;
}

uint8_t lora_get_spreading_factor(lora_t *lora) {
  return _read(lora, REG_MODEM_CONFIG_2) >> 4;
}

void lora_set_spreading_factor(lora_t *lora, int sf) {
  if (sf < 6) {
    sf = 6;
  } else if (sf > 12) {
    sf = 12;
  }

  if (sf == 6) {
    _write(lora, REG_DETECTION_OPTIMIZE, 0xc5);
    _write(lora, REG_DETECTION_THRESHOLD, 0x0c);
  } else {
    _write(lora, REG_DETECTION_OPTIMIZE, 0xc3);
    _write(lora, REG_DETECTION_THRESHOLD, 0x0a);
  }

  _write(lora, REG_MODEM_CONFIG_2, (_read(lora, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
  lora_set_ldo_flag(lora);
}

long lora_get_signal_bandwidth(lora_t *lora) {
  uint8_t bw = _read(lora, REG_MODEM_CONFIG_1) >> 4;

  switch (bw) {
    case 0: return 7.8E3;
    case 1: return 10.4E3;
    case 2: return 15.6E3;
    case 3: return 20.8E3;
    case 4: return 31.25E3;
    case 5: return 41.7E3;
    case 6: return 62.5E3;
    case 7: return 125E3;
    case 8: return 250E3;
    case 9: return 500E3;
  }

  return -1;
}

void lora_set_signal_bandwidth(lora_t *lora, long sbw) {
  int bw;

  if (sbw <= 7.8E3) {
    bw = 0;
  } else if (sbw <= 10.4E3) {
    bw = 1;
  } else if (sbw <= 15.6E3) {
    bw = 2;
  } else if (sbw <= 20.8E3) {
    bw = 3;
  } else if (sbw <= 31.25E3) {
    bw = 4;
  } else if (sbw <= 41.7E3) {
    bw = 5;
  } else if (sbw <= 62.5E3) {
    bw = 6;
  } else if (sbw <= 125E3) {
    bw = 7;
  } else if (sbw <= 250E3) {
    bw = 8;
  } else /*if (sbw <= 250E3)*/ {
    bw = 9;
  }

  _write(lora, REG_MODEM_CONFIG_1, (_read(lora, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
  lora_set_ldo_flag(lora);
}

void lora_set_ldo_flag(lora_t *lora) {
  long symbol_duration = 1000 / (lora_get_signal_bandwidth(lora) / (1L << lora_get_spreading_factor(lora)));
  lora_set_ldo_flag_force(lora, symbol_duration > 16);
}

void lora_set_ldo_flag_force(lora_t *lora, bool ldoOn)
{
  uint8_t config3 = _read(lora, REG_MODEM_CONFIG_3);

  // set/clear bit 3
  if (ldoOn) {
    config3 |= 0x8;
  } else {
    config3 &= ~0x8;
  }

  _write(lora, REG_MODEM_CONFIG_3, config3);
}

// set coding rate to 4/<denom>
void lora_set_coding_rate(lora_t *lora, int denominator) {
  if (denominator < 5) {
    denominator = 5;
  } else if (denominator > 8) {
    denominator = 8;
  }

  int cr = denominator - 4;
  _write(lora, REG_MODEM_CONFIG_1, (_read(lora, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void lora_set_preamble_length(lora_t *lora, long length) {
  _write(lora, REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
  _write(lora, REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void lora_set_sync_word(lora_t *lora, uint8_t sw) {
  _write(lora, REG_SYNC_WORD, sw);
}

void lora_enable_crc(lora_t *lora) {
  _write(lora, REG_MODEM_CONFIG_2, _read(lora, REG_MODEM_CONFIG_2) | 0x04);
}

void lora_disable_crc(lora_t *lora) {
  _write(lora, REG_MODEM_CONFIG_2, _read(lora, REG_MODEM_CONFIG_2) & 0xfb);
}

void lora_enable_invert_iq(lora_t *lora) {
  _write(lora, REG_INVERTIQ,  0x66);
  _write(lora, REG_INVERTIQ2, 0x19);
}

void lora_disable_invert_iq(lora_t *lora) {
  _write(lora, REG_INVERTIQ,  0x27);
  _write(lora, REG_INVERTIQ2, 0x1d);
}

void lora_set_ocp(lora_t *lora, uint8_t mA) {
  uint8_t ocpTrim = 27;

  if (mA <= 120) {
    ocpTrim = (mA - 45) / 5;
  } else if (mA <=240) {
    ocpTrim = (mA + 30) / 10;
  }

  _write(lora, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void lora_set_gain(lora_t *lora, uint8_t gain) {
  // check allowed range
  if (gain > 6) {
    gain = 6;
  }
  
  // set to standby
  lora_idle(lora);
  
  // set gain
  if (gain == 0) {
    // if gain = 0, enable AGC
    _write(lora, REG_MODEM_CONFIG_3, 0x04);
  } else {
    // disable AGC
    _write(lora, REG_MODEM_CONFIG_3, 0x00);
	
    // clear Gain and set LNA boost
    _write(lora, REG_LNA, 0x03);
	
    // set gain
    _write(lora, REG_LNA, _read(lora, REG_LNA) | (gain << 5));
  }
}

void lora_dio_interrupt(lora_t *lora) {
  int irq_flags = _read(lora, REG_IRQ_FLAGS);

  // clear IRQ's
  _write(lora, REG_IRQ_FLAGS, irq_flags);

  if (irq_flags & IRQ_CAD_DONE_MASK) {
    if (lora->on_cad_done) {
      lora->on_cad_done(irq_flags & IRQ_CAD_DETECTED_MASK);
    }
  } else if (!(irq_flags & IRQ_PAYLOAD_CRC_ERROR_MASK)) {
    if (irq_flags & IRQ_RX_DONE_MASK) {
      // received a packet
      lora->_pos = 0;

      // read packet length
      int length = lora->_implicit_header ? _read(lora, REG_PAYLOAD_LENGTH) : _read(lora, REG_RX_NB_BYTES);

      // set FIFO address to current RX address
      _write(lora, REG_FIFO_ADDR_PTR, _read(lora, REG_FIFO_RX_CURRENT_ADDR));

      if (lora->on_receive) {
        lora->on_receive(length);
      }
    } else if (irq_flags & IRQ_TX_DONE_MASK) {
      if (lora->on_tx_done) {
        lora->on_tx_done();
      }
    }
  }
}
