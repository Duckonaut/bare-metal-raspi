#include <kernel/mmio.h>
#include <stdint.h>

// Turns out inline functions can (and should) be defined in headers. Who knew? not me.

//void mmio_write(uint32_t reg, uint32_t data)
//{
//	*(volatile uint32_t*)reg = data;
//}

//uint32_t mmio_read(uint32_t reg)
//{
//	return *(volatile uint32_t*)reg;
//}
