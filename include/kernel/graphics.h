#ifndef KERNEL_GRAPHICS_H
#define KERNEL_GRAPHICS_H

#include <stdint.h>

#define GPU_OFFSET 0x40000000

typedef struct {
	uint32_t width;
	uint32_t height;
	uint32_t v_width;
	uint32_t v_height;
	uint32_t pitch;
	uint32_t bit_depth;
	uint32_t x;
	uint32_t y;
	void* buf;
	uint32_t buf_size;
} framebuffer_info_t;

typedef union pixel {
	struct {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	};
	struct {
		uint8_t b;
		uint8_t g;
		uint8_t r;
	};
} pixel_t;

typedef struct {
	uint32_t x;
	uint32_t y;
	uint32_t max_x;
	uint32_t max_y;
} char_display_info_t;

extern framebuffer_info_t fb_info;
extern char_display_info_t char_display_info;

void gpu_putc(char c, const pixel_t bg, const pixel_t fg);
void gpu_puts(char* s, const pixel_t bg, const pixel_t fg);
void gpu_setcursor(uint32_t x, uint32_t y); 
uint32_t graphics(uint32_t width, uint32_t height);
void write_pixel(uint32_t x, uint32_t y, const pixel_t* pix);

#endif
