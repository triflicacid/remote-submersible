#include "actions.h"
#include "constants.h"
#include "depth.h"
#include "main.h"
#include "shared/stored-code.h"
#include "shared/tri-state-switch.h"

// convert result from ADC joystick to float [0,1].
inline double adc_joystick_conv(dma_t value) {
	// TODO proper conversion
	// ADC is configured to 12-bit
	return (double)value / 0xFFF;
}

void action_propeller(dma_t x, dma_t y) {
	// convert raw ADC values into range
	propeller_data data;
	data.x = adc_joystick_conv(x);
	data.y = adc_joystick_conv(y);

	transmit(&g_lora, OP_PROPELLER, &data, sizeof(data));
}

void action_ballast(void) {
	// determine tri-state switch mode
	ballast_data data = { tri_state_switch_read(BALLAST_PORT, BALLAST_DOWN_PIN, BALLAST_PORT, BALLAST_UP_PIN) };

#ifdef PREDICT_DEPTH
	// update vertical direction for depth estimation
	set_vert_dir(data.mode);

	// halt or restart timed event depending on switch mode
	if (data.mode == TRISTATE_MID) {
		timed_event_stop(g_depth_event, true);
	} else {
		timed_event_start(g_depth_event);
	}
#endif

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

void action_rx_done(void) {
	// fetch data from LoRa device
	lora_rx_point_next_packet(&g_lora);
	lora_receive(&g_lora, g_lora_buffer, sizeof(g_lora_buffer));

	// clear IRQ flags
	lora_irq_clear(&g_lora);

	// deliver payload
	on_recv_payload(g_lora_buffer);

	// go back to listening on RX
	lora_mode_rx(&g_lora, false);
}

void recv_send_code(code_data *data) {
	// save downloaded code and display to operator
	save_code(CODE_DOWNLOADED, data->code);
	display_write(&g_display, data->code, 0x0);
}
