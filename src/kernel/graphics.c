#include <common/util.h>
#include <kernel/charmap.h>
#include <kernel/gpu_mail.h>
#include <kernel/graphics.h>
#include <stdint.h>

framebuffer_info_t fb_info __attribute__((aligned(16)));
char_display_info_t char_display_info;

void write_pixel(uint32_t x, uint32_t y, const pixel_t* pix)
{
	uint8_t* location = fb_info.buf + y * fb_info.pitch + x * (3);
	memcpy(location, pix, (3));
}

void gpu_putc(char c, const pixel_t bg, const pixel_t fg)
{
	uint8_t w, h;
	uint8_t mask;
	const uint8_t* bmp = font(c);
	uint32_t i, num_rows = div(fb_info.height, CHAR_HEIGHT);

	// shift everything up one row
	if (char_display_info.y >= num_rows) {
		memcpy(fb_info.buf, fb_info.buf + fb_info.pitch * CHAR_HEIGHT, fb_info.pitch * (num_rows - 1) * CHAR_HEIGHT);

		// zero out the last row
		bzero(fb_info.buf + fb_info.pitch * (num_rows - 1) * CHAR_HEIGHT, fb_info.pitch * CHAR_HEIGHT);
		char_display_info.y--;
	}

	if (c == '\n') {
		char_display_info.x = 0;
		char_display_info.y++;
		return;
	}

	for (w = 0; w < CHAR_WIDTH; w++) {
		for (h = 0; h < CHAR_HEIGHT; h++) {
			mask = 1 << (w);
			if (bmp[h] & mask)
				write_pixel(char_display_info.x * CHAR_WIDTH + w, char_display_info.y * CHAR_HEIGHT + h, &fg);
			else
				write_pixel(char_display_info.x * CHAR_WIDTH + w, char_display_info.y * CHAR_HEIGHT + h, &bg);
		}
	}

	char_display_info.x++;
	if (char_display_info.x > div(fb_info.width, CHAR_WIDTH)) {
		char_display_info.x = 0;
		char_display_info.y++;
	}
}

uint32_t graphics(uint32_t width, uint32_t height)
{
	fb_info.width = fb_info.v_width = width;
	fb_info.height = fb_info.v_height = height;
	fb_info.bit_depth = 24;
	fb_info.pitch = fb_info.width * 3;
	fb_info.x = fb_info.y = 0;

	mail_message_t msg;
	msg.data = ((uint32_t)(&fb_info) + GPU_OFFSET) >> 4;
	postman_send(1, msg);

	// if (!postman_read(1).data) return 1;

	pixel_t p;
	p.red = 0;
	p.green = 0;
	p.blue = 0;

	for (uint32_t j = 0; j < fb_info.height; j++) {
		for (uint32_t i = 0; i < fb_info.width; i++) {
			write_pixel(i, j, &p);
		}
	}

	return 0;
}