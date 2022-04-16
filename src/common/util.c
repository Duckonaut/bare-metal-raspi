#include <common/util.h>
#include <kernel/mmio.h>
#include <stdint.h>

uint32_t div(uint32_t a, uint32_t b)
{
	unsigned int c = 1, result = 0;

	if (b == 0)
		return (uint32_t)-1 /*infinity*/;

	while (((int32_t)b > 0) && (b < a)) {
		b = b << 1;
		c = c << 1;
	}

	do {
		if (a >= b) {
			a -= b;
			result += c;
		}
		b = b >> 1;
		c = c >> 1;
	} while (c);

	return result;
}

uint32_t mod(uint32_t dividend, uint32_t divisor)
{
	uint32_t res;

	res = div(dividend, divisor);

	return dividend - (res * divisor);
}

uint32_t valid_hex_byte(char high, char low) {
    return ((high >= '0' && high <= '9') || (high >= 'a' && high <= 'f') || (high >= 'A' && high <= 'F')) && ((low >= '0' && low <= '9') || (low >= 'a' && low <= 'f') || (low >= 'A' && low <= 'F')); 
}

uint8_t xtob(char* c) {
    char high = c[0];
    char low = c[1];
    uint8_t result = 0;
    
    if (!valid_hex_byte(high, low))
        return 0;

    if (high >= '0' && high <= '9')
        result += (high - '0') << 4;
    else if (high >= 'a' && high <= 'f')
        result += (high - 'a' + 10) << 4;
    else if (high >= 'A' && high <= 'F')
        result += (high - 'A' + 10) << 4;

    if (low >= '0' && low <= '9')
        result += low - '0';
    else if (low >= 'a' && low <= 'f')
        result += low - 'a' + 10;
    else if (low >= 'A' && low <= 'F')
        result += low - 'A' + 10;

    return result;
}

char* itoa(uint32_t num, uint32_t base)
{
	static char intbuf[35];
	uint32_t j = 0, i;
	uint32_t divmod_res;

	if (num == 0) {
		intbuf[0] = '0';
		intbuf[1] = '\0';
		return intbuf;
	}

	i = num;

	while (i != 0) {
		divmod_res = mod(i, base);
		intbuf[j] = (divmod_res) < 10 ? '0' + (divmod_res) : 'a' + (divmod_res)-10;
		j++;
		i = div(i, base);
	}

	if (base == 16) {
		intbuf[j++] = 'x';
		intbuf[j++] = '0';
	}
	else if (base == 8) {
		intbuf[j++] = 'o';
		intbuf[j++] = '0';
	}
	else if (base == 2) {
		intbuf[j++] = 'b';
		intbuf[j++] = '0';
	}

	intbuf[j] = '\0';
	j--;
	i = 0;
	while (i < j) {
		char t = intbuf[i];
		intbuf[i] = intbuf[j];
		intbuf[j] = t;
		i++;
		j--;
	}

	return intbuf;
}

char* btoa(uint32_t num)
{
	static char intbuf[35];

	intbuf[0] = '0';
	intbuf[1] = 'b';
	intbuf[34] = '\0';

	for (uint32_t i = 33; i > 1; i--) {
		if (num & 1) {
			intbuf[i] = '1';
		}
		else {
			intbuf[i] = '0';
		}

		num >>= 1;
	}

	return intbuf;
}

int atoi(char* num)
{
	int res = 0, power = 0, digit, i;
	char* start = num;

	// Find the end
	while (*num >= '0' && *num <= '9') {
		num++;
	}

	num--;

	while (num != start) {
		digit = *num - '0';
		for (i = 0; i < power; i++) {
			digit *= 10;
		}
		res += digit;
		power++;
		num--;
	}

	return res;
}

void memcpy(void* dest, const void* src, int bytes)
{
	uint8_t* d = dest;
	const uint8_t* s = src;
	while (bytes--) {
		*d++ = *s++;
	}
}

void memset(void* dest, uint8_t c, int bytes)
{
	uint8_t* d = dest;
	while (bytes--) {
		*d++ = c;
	}
}

void bzero(void* dest, int bytes)
{
	memset(dest, 0, bytes);
}

// Loop <delay> picoseconds
void delay(int32_t count)
{
	uint64_t startTime = mmio_read(0x20003004);

	while (mmio_read(0x20003004) < startTime + count)
		;
}
