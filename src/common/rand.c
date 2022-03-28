#include <common/rand.h>
#include <common/util.h>
#include <stdint.h>

const uint32_t RAND_MAX = 0xFFFF;
const uint32_t RAND_MAX_HALF = 0x7FFF;

static uint32_t next;

uint32_t rand()
{
	next = next * 1103515245 + 12345;
	return (uint32_t)mod(div(next, 65536), 32768);
}

void srand(uint32_t seed)
{
	next = seed;
}
