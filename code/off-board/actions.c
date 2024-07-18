#include "actions.h"
#include "constants.h"
#include "main.h"
#include "shared/stored-code.h"

// convert uint16_t result from ADC joystick to float [0,1].
static float adc_joystick_conv(uint32_t value) {
	// TODO proper conversion
	return value / 0xFFFF;
}

void action_propeller(uint32_t x, uint32_t y) {
	// convert raw ADC values into range
	propeller_data data;
	data.x = adc_joystick_conv(x);
	data.y = adc_joystick_conv(y);

	transmit(&g_lora, OP_PROPELLER, &data, sizeof(data));
}

// return state {-1,0,1} depending on pins
// `down` pin refers to -1, `up` pin to 1
static int8_t read_tri_state(GPIO_TypeDef *down_port, uint16_t down_pin, GPIO_TypeDef *up_port, uint16_t up_pin) {
	if (HAL_GPIO_ReadPin(down_port, down_pin) == GPIO_PIN_SET) {
		return -1;
	}

	if (HAL_GPIO_ReadPin(up_port, up_pin) == GPIO_PIN_SET) {
		return 1;
	}

	return 0;
}

void action_ballast(void) {
	// determine tri-state switch mode and transmit payload
	ballast_data data = { read_tri_state(BALLAST_PORT, BALLAST_DOWN_PIN, BALLAST_PORT, BALLAST_UP_PIN) };
	transmit(&g_lora, OP_BALLAST, &data, sizeof(data));
}

void action_send_code(void) {
	// create payload to contain our stored code
	code_data data = { fetch_code(CODE_INTERNAL) };
	transmit(&g_lora, OP_SEND_CODE, &data, sizeof(data));
}

void action_request_code(void) {
	transmit_opcode(&g_lora, OP_REQUEST_CODE);
}

void action_release_pod(void) {
	transmit_opcode(&g_lora, OP_RELEASE_POD);
}

void recv_send_code(code_data *data) {
	// save downloaded code and display to operator
	save_code(CODE_DOWNLOADED, data->code);
	display_write(&g_display, data->code, 0x0);
}
