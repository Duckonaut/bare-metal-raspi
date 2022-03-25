#include <common/io.h>
#include <kernel/graphics.h>

const static pixel_t black = {0,0,0};
const static pixel_t white = {255,255,255};

void printf(char* s) {
	gpu_puts(s, black, white);
}
