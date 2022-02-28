#include <common/util.h>
#include <kernel/graphics.h>
#include <kernel/mmio.h>
#include <stddef.h>
#include <stdint.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void)r0;
	(void)r1;
	(void)atags;

	while (graphics(1920, 1080));

	char text[] = "\n\n\n\n\n\nMANKIND IS DEAD.\nBLOOD IS FUEL.\nHELL IS FULL.";
	pixel_t red = {0, 0, 255};
	pixel_t black = {0, 0, 0};

	for (uint32_t i = 0; i < sizeof(text); i++) {
		gpu_putc(text[i], black, red);

		delay(1000000);
	}

	uint32_t gpio_enable_output = 0x00040000;
	uint32_t gpio_comm_16 = 0x00010000;
	uint32_t delay_num = 0x003F0000;

	mmio_write(GPIO + 4, gpio_enable_output);

	while (1) {
		mmio_write(GPIO + 40, gpio_comm_16);

		delay(100000);

		mmio_write(GPIO + 28, gpio_comm_16);

		delay(100000);
	}
}