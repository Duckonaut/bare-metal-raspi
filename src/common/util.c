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

/*uint32_t div(uint32_t n, uint32_t d) {
	// n is dividend, d is divisor
	// store the result in q: q = n / d
	uint32_t q = 0;

	// as long as the divisor fits into the remainder there is something to do
	while (n >= d) {
		uint32_t i = 0, d_t = d;
		// determine to which power of two the divisor still fits the dividend
		//
		// i.e.: we intend to subtract the divisor multiplied by powers of two
		// which in turn gives us a one in the binary representation
		// of the result
		while (n >= (d_t << 1) && ++i)
			d_t <<= 1;
		// set the corresponding bit in the result
		q |= 1 << i;
		// subtract the multiple of the divisor to be left with the remainder
		n -= d_t;
		// repeat until the divisor does not fit into the remainder anymore
	}
	return q;
}*/

/*uint32_t div(uint32_t a, uint32_t b) {
	uint32_t ans = 0; // the quotient is intialized

	for (uint32_t i = 31; i >= 0; i--) {
		if (b << i <= a) { // checking if b multiplied by 2**i is <= a
			a -= b << i;   // subtracting b << i from a
			ans += 1 << i; // adding 2 power i to the answer
		}
	}
	return ans;
}*/

uint32_t mod(uint32_t dividend, uint32_t divisor)
{
	uint32_t res;

	res = div(dividend, divisor);

	return dividend - (res * divisor);
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

	uint32_t i = 33;

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
