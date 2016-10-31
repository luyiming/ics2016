#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#define CACHE_WIDTH 16
#define BLOCK_WIDTH 6
#define SET_BLOCK_WIDTH 3
#define SET_WIDTH (CACHE_WIDTH - BLOCK_WIDTH - SET_BLOCK_WIDTH)
#define TAG_WIDTH (27 - BLOCK_WIDTH - SET_WIDTH)

#define BLOCK_SIZE (1 << BLOCK_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_BLOCK (1 << SET_BLOCK_WIDTH)

typedef union {
	struct {
		uint32_t block_addr	: BLOCK_WIDTH;
		uint32_t nr_set	: SET_WIDTH;
		uint32_t tag	: TAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

typedef struct {
    uint8_t data[BLOCK_SIZE];
    bool valid;
    uint32_t tag;
} Block;

typedef struct {
    Block blocks[NR_SET][NR_BLOCK];
} Cache;

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

