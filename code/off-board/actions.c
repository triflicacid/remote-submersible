#include "actions.h"
#include "main.h"
#include "shared/stored-code.h"

// convert uint16_t result from ADC joystick to float [0,1].
static float adc_joystick_conv(uint16_t value) {
	// TODO proper conversion
	return value / 0xFFFF;
}

void action_propeller(uint16_t x, uint16_t y) {
	// convert raw ADC values into range
	propeller_data data;
	data.x = adc_joystick_conv(x);
	data.y = adc_joystick_conv(y);

	transmit(&g_lora, OP_PROPELLER, &data, sizeof(data));
}

void action_ballast(void) {
	// TODO
	
	ballast_data data;
	transmit(&g_lora, OP_BALLAST, &data, sizeof(data));
}

void action_send_code(void) {
	// create payload to contain our stored code
	code_data data = { fetch_code() };
	transmit(&g_lora, OP_SEND_CODE, &data, sizeof(data));
}

void action_request_code(void) {
	transmit_opcode(&g_lora, OP_REQUEST_CODE);
}

void action_release_pod(void) {
	transmit_opcode(&g_lora, OP_RELEASE_POD);
}

void recv_send_code(code_data *data) {
	save_code(data->code);
	// TODO display code
}
