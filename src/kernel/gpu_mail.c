#include <kernel/gpu_mail.h>
#include <kernel/mmio.h>
#include <stdint.h>

mail_message_t postman_read(uint32_t channel)
{
	mail_status_t stat;
	mail_message_t res;

	// Make sure that the message is from the right channel
	do {
		// Make sure there is mail to recieve
		do {
			stat.bits = mmio_read(POSTMAN_STATUS);
		} while (stat.empty);

		// Get the message
		res.bits = mmio_read(POSTMAN_READ);
	} while (res.channel != channel);

	res.bits = 0;

	return res;
}

void postman_send(uint32_t channel, mail_message_t data)
{
	data.channel = channel;

	mail_status_t status;

	do {
		status.bits = mmio_read(POSTMAN_STATUS);
	} while (status.full);

	mmio_write(POSTMAN_WRITE, data.bits);
}
