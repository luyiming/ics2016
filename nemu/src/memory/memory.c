#include "common.h"
#include "stdlib.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#define CACHE_WIDTH 16
#define BLOCK_WIDTH 6
#define SET_BLOCK_WIDTH 3
#define SET_WIDTH (CACHE_WIDTH - BLOCK_WIDTH - SET_BLOCK_WIDTH)
#define TAG_WIDTH (27 - BLOCK_WIDTH - SET_WIDTH)

#define BLOCK_SIZE (1 << BLOCK_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_SET_BLOCK (1 << SET_BLOCK_WIDTH)

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

Block cache[NR_SET][NR_SET_BLOCK];


void init_cache() {
	int i, j;
	for(i = 0; i < NR_SET; i ++) {
		for(j = 0; j < NR_SET_BLOCK; j ++) {
			cache[i][j].valid = false;
		}
	}
}

int cache_miss(hwaddr_t addr) {
    int i, j;
    int k;
    uint32_t data;
    cache_addr temp;
    temp.addr = addr;
    uint32_t nr_set = temp.nr_set;
    printf("cache_miss addr before: %x\n", addr);
    for(i = 0; i < NR_SET_BLOCK; i++) {
        if(cache[nr_set][i].valid == false)
            break;
    }
    if(i == NR_SET_BLOCK) {
        i = rand() % NR_SET_BLOCK;
    }
    addr = addr & ~((1 << BLOCK_WIDTH) - 1);
    for(j = 0; j < BLOCK_SIZE; j += 4) {
        data = dram_read(addr + j, 4);
        memcpy(cache[nr_set][i].data + j, &data, 4);
    }
    printf("cache_miss addr after: %x\n", addr);
    printf("cache_miss block_addr: %x\n", temp.block_addr);
    Assert(j == BLOCK_SIZE, "j != BLOCK_SIZE");
    for(k = 0; k < BLOCK_SIZE; k++) {
        Assert(cache[nr_set][i].data[k] == (dram_read(addr + k, 1) & 0xff), "cache_miss fail");
    }
    cache[nr_set][i].valid = true;
    cache[nr_set][i].tag = temp.tag;
    return i;
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
    int i;
    int k;
    uint32_t data;
    cache_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t nr_set = temp.nr_set;
    uint32_t tag = temp.tag;
    for(i = 0; i < NR_SET_BLOCK; i++) {
        if(cache[nr_set][i].valid && cache[nr_set][i].tag == tag) {
            memcpy(&data, &cache[nr_set][i].data[block_addr], len);
            return data;
        }
    }
    i = cache_miss(addr);
    printf("\ncache:\n");
    for(k = 0; k < BLOCK_SIZE - block_addr; k++) {
        printf("%x ", cache[nr_set][i].data[block_addr + k]);
    }
    printf("\ndram:\n");
    for(k = 0; k < BLOCK_SIZE; k++) {
        printf("addr: %x ", addr + k);
        printf("%x \n", dram_read(addr + k, 1) & 0xff);
    }
    memcpy(&data, &cache[nr_set][i].data[block_addr], len);
	Assert((dram_read(addr, len) & (~0u >> ((4 - len) << 3))) == data, "cache_read fail");
    printf("cache_read return data: %x\n", data);
    return data;
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
    printf("cache_write0\n");
    int i;
    uint32_t test_data;
    cache_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t nr_set = temp.nr_set;
    uint32_t tag = temp.tag;
    for(i = 0; i < NR_SET_BLOCK; i++) {
        if(cache[nr_set][i].valid && cache[nr_set][i].tag == tag) {
            memcpy(&cache[nr_set][i].data[block_addr], &data, len);
            dram_write(addr, len, data);
            return;
        }
    }
    printf("cache_write\n");
    i = cache_miss(addr);
    printf("cache_write2\n");
    memcpy(&cache[nr_set][i].data[block_addr], &data, len);
    printf("cache_write3\n");
    dram_write(addr, len, data);
    printf("cache_write4\n");
    memcpy(&test_data, &cache[nr_set][i].data[block_addr], len);
	Assert((dram_read(addr, len) & (~0u >> ((4 - len) << 3))) == test_data, "cache_read fail");
}
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return cache_read(addr, len);
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write(addr, len, data);
	//dram_write(addr, len, data);
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

