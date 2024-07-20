#include "actions.h"
#include "constants.h"
#include "main.h"

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
