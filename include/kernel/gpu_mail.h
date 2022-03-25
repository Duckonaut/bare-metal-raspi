#ifndef KERNEL_GPU_MAIL_H
#define KERNEL_GPU_MAIL_H

#include <stdint.h>

typedef enum
{
	POSTMAN_READ = 0x2000B880,
	POSTMAN_POLL = 0x2000B890,
	POSTMAN_SENDER = 0x2000B894,
	POSTMAN_STATUS = 0x2000B898,
	POSTMAN_CONFIG = 0x2000B89C,
	POSTMAN_WRITE = 0x2000B8A0,
} postman_info_t;

typedef union {
	struct {
		uint8_t channel : 4;
		uint32_t data : 28;
	};
	uint32_t bits;
} mail_message_t;

typedef union {
	struct {
		uint32_t reserved : 30;
		uint8_t empty : 1;
		uint8_t full : 1;
	};
	uint32_t bits;
} mail_status_t;

mail_message_t postman_read(uint32_t channel);

void postman_send(uint32_t channel, mail_message_t data);

#endif
