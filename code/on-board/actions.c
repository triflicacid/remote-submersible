#include "actions.h"
#include "constants.h"
#include "main.h"
#include "shared/stored-code.h"

void recv_send_code(code_data *data) {
	// cache code, we ned to send it later
	save_code(0, data->code);
}

void recv_request_code(void) {
	// respond with our cached code
	code_data data = { fetch_code(0) };
	transmit(&g_lora, OP_SEND_CODE, &data, sizeof(data));
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
