#include "lora.h"

#define REG_FIFO_ACCESS 0x00

#define REG_OP_MODE 0x01
#define OP_MODE_SLEEP 0x00
#define OP_MODE_LORA_SLEEP 0x80
#define OP_MODE_LORA_STANDBY 0x81
#define OP_MODE_LORA_TX 0x83

#define REG_PA_CONF 0x09
#define REG_FIFO_ADDR_PTR 0x0D
#define REG_TX_BASE 0x0E
#define TX_BASE_ADDR 0x80
#define REG_RX_BASE 0x0F
#define RX_BASE_ADDR 0x00
#define REG_NUMBER_OF_BYTES 0x13
#define REG_CONF_1 0x1D
#define REG_CONF_2 0x1E
#define REG_PA_DAC 0x4D
#define REG_CONF_3 0x26
#define REG_PAYLOAD_LEN 0x22
#define REG_MAX_PAYLOAD_LEN 0x23

struct lora_t {
	SPI_InitTypeDef *spi;
	GPIO_TypeDef *nss_port;
	uint16_t nss_pin;
	uint32_t timeout;
};

// start transaction - reset NSS
static void start_transaction(lora_t *lora) {
	HAL_GPIO_WritePin(lora->nss_port, lora->nss_pin, GPIO_PIN_RESET);
}

// end transaction - set NSS
static void end_transaction(lora_t *lora) {
	HAL_GPIO_WritePin(lora->nss_port, lora->nss_pin, GPIO_PIN_SET);
}

// SPI - transmit data
static HAL_StatusTypeDef spi_transmit(lora_t *lora, uint8_t *data, uint16_t size) {
	return HAL_SPI_Transmit(lora->spi, data, size, lora->timeout);
}

// SPI - recieve data
static HAL_StatusTypeDef spi_receive(lora_t *lora, uint8_t *data, uint16_t size) {
	return HAL_SPI_Receive(lora->spi, data, size, lora->timeout);
}

// read bytes from the given address
static HAL_StatusTypeDef read_bytes(lora_t *lora, uint8_t address, uint8_t *buffer, uint16_t size) {
	start_transaction(lora);

	// payload - address to read
	uint8_t payload = address & 0x7f;
	
	// request address to read
	HAL_StatusTypeDef status = spi_transmit(lora, &payload, 1);
	
	if (status != HAL_OK) {
		end_transaction(lora);
		return status;
	}
	
	// receive address' value
	status = spi_receive(lora, buffer, size);
	
	end_transaction(lora);
	
	return status;
}

// write byte to the given address
static HAL_StatusTypeDef write_byte(lora_t *lora, uint8_t address, uint8_t value) {
	// payload - address to write to, and value to write
	uint8_t payload[2] = { address | 0x80, value };
	
	// send payload
	start_transaction(lora);
	HAL_StatusTypeDef status = spi_transmit(lora, payload, sizeof(payload));
	end_transaction(lora);
	
	return status;
}


void lora_setup(lora_t *lora, SPI_InitTypeDef *spi, GPIO_TypeDef *nss_port, uint16_t nss_pin) {
	// assign known fields in struct
	lora->spi = spi;
	lora->nss_port = nss_port;
	lora->nss_pin = nss_pin;
	lora->timeout = 1000;
	
	// enter into LoRa mode
	write_byte(lora, REG_OP_MODE, OP_MODE_SLEEP);
	write_byte(lora, REG_OP_MODE, OP_MODE_LORA_SLEEP);
	
	// set up base addresses
	write_byte(lora, REG_RX_BASE, RX_BASE_ADDR);
	write_byte(lora, REG_TX_BASE, TX_BASE_ADDR);
	
	// max out power
	write_byte(lora, REG_PA_CONF, 0xFF);
	write_byte(lora, REG_PA_DAC, 0x87);
	
	// set max payload length
	write_byte(lora, REG_MAX_PAYLOAD_LEN, 64);
	
	// configure modem
	write_byte(lora, REG_CONF_1, 0x72);
	write_byte(lora, REG_CONF_2, 0x74);
	write_byte(lora, REG_CONF_3, 0x04);
}

void lora_receive(lora_t *lora, uint8_t *buffer, uint8_t *size, uint8_t max_size) {
	// read payload length
	read_bytes(lora, REG_NUMBER_OF_BYTES, size, 1);
	
	// clamp to max_size
	if (size > max_size) {
		size = max_size;
	}
	
	// reset FIFO address ptr
	write_byte(lora, REG_FIFO_ADDR_PTR, 0);
	
	// read received payload
	read_bytes(lora, REG_FIFO_ACCESS, buffer, size);
	
	// return to sleep
	write_byte(lora, REG_OP_MODE, OP_MODE_LORA_SLEEP);
}

void lora_send(lora_t *lora, uint8_t *buffer, uint8_t size) {
	// set payload length
	write_byte(lora, REG_PAYLOAD_LEN, size);
	
	// move to standby mode
	write_byte(lora, REG_OP_MODE, OP_MODE_LORA_STANDBY);
	
	// set pointer to TX base
	write_byte(lora, REG_FIFO_ADDR_PTR, TX_BASE_ADDR);
	
	// write payload to buffer
	for (uint16_t i = 0; i < size; i++) {
		write_byte(lora, REG_FIFO_ACCESS, buffer[i]);
	}
	
	// set to TX (transmit) mode
	write_byte(lora, REG_OP_MODE, OP_MODE_LORA_TX);
	
	// TODO wait? do we need to switch states here?
	
	// return to sleep
	write_byte(lora, REG_OP_MODE, OP_MODE_LORA_SLEEP);
}
