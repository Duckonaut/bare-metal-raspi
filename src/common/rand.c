#include <stdint.h>
#include <common/rand.h>
#include <common/util.h>

const uint32_t RAND_MAX = 0xFFFF;
const uint32_t RAND_MAX_HALF = 0x7FFF;

static uint32_t next;


uint32_t rand() { 
	next = next * 1103515245 + 12345;
    return next; //(uint32_t)mod((div(next, RAND_MAX)), RAND_MAX_HALF);
    //next = (uint32_t)(next * next * 0xEF00 + next * 1 + 73);
    //return next;
}

void srand(uint32_t seed) {
	next = seed;
}

