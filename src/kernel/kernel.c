#include <common/io.h>
#include <common/rand.h>
#include <common/util.h>
#include <kernel/graphics.h>
#include <kernel/interrupts.h>
#include <kernel/mmio.h>
#include <kernel/timer.h>
#include <stddef.h>
#include <stdint.h>

const uint32_t UART0_BASE = 0x20201000;
const uint32_t UART0_IMSC = (UART0_BASE + 0x38);

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void)r0;
	(void)r1;
	(void)atags;

	while (graphics(320, 240))
		;

	printf(INTERRUPTS_ENABLED() ? "YES\n" : "NO\n");

	interrupts_init();
	timer_init();
	timer_set(3000);
	printf(INTERRUPTS_ENABLED() ? "YES\n" : "NO\n");

	// int32_t a = 0x7FFFFFF0;
	// printf(itoa(a, 16));
	// a += 0x10;

	// printf(itoa(a, 16));


	char text[] = "HELLO.\n";
	pixel_t red = {0, 0, 255};
	pixel_t black = {0, 0, 0};

	gpu_puts(text, black, red);

	for (uint32_t i = 0;; i = rand()) {
		char* s = itoa(i, 16);

		gpu_puts(s, black, red);

		gpu_putc('\n', black, black);
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
