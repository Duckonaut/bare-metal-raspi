#include <stdint.h>

#define GPIO 0x20200000

inline void mmio_write(uint32_t reg, uint32_t data);

inline uint32_t mmio_read(uint32_t reg);