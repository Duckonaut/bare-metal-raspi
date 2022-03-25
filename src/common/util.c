#include <common/util.h>
#include <stdint.h>
#include <kernel/mmio.h>

// raspi 1B doesnt have a div instruction
uint32_t div(uint32_t dividend, uint32_t divisor)
{
	uint32_t denom = divisor;
	uint32_t current = 1;
	uint32_t answer = 0;

	if (denom > dividend)
		return 0;

	if (denom == dividend)
		return 1;

	while (denom <= dividend) {
		denom <<= 1;
		current <<= 1;
	}

	denom >>= 1;
	current >>= 1;

	while (current != 0) {
		if (dividend >= denom) {
			dividend -= denom;
			answer |= current;
		}
		current >>= 1;
		denom >>= 1;
	}
	return answer;
}

uint32_t mod(uint32_t dividend, uint32_t divisor)
{
	uint32_t res;

	res = div(dividend, divisor);

	return dividend - res * divisor;
}

char* itoa(int num, int base)
{
	static char intbuf[33];
	uint32_t j = 0, isneg = 0, i;
	uint32_t divmod_res;

	if (num == 0) {
		intbuf[0] = '0';
		intbuf[1] = '\0';
		return intbuf;
	}

	if (base == 10 && num < 0) {
		isneg = 1;
		num = -num;
	}

	i = (uint32_t)num;

	while (i != 0) {
		divmod_res = mod(i, base);
		intbuf[j++] = (divmod_res) < 10 ? '0' + (divmod_res) : 'a' + (divmod_res)-10;
		i = div(i, base);
	}

	if (isneg)
		intbuf[j++] = '-';

	if (base == 16) {
		intbuf[j++] = 'x';
		intbuf[j++] = '0';
	}
	else if (base == 8) {
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
		isneg = intbuf[i];
		intbuf[i] = intbuf[j];
		intbuf[j] = isneg;
		i++;
		j--;
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
