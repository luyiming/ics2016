#include "common.h"
#include "stdlib.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#define CACHE_WIDTH 22
#define CACHE_SIZE (4 << 20)
#define COL_WIDTH 6
#define ROW_WIDTH 4
#define SET_WIDTH 12  // == (CACHE_WIDTH - COL_WIDTH - ROW_WIDTH)
#define TAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_SET (1 << SET_WIDTH)

#define HW_MEM_SIZE (1 << (COL_WIDTH + SET_WIDTH + TAG_WIDTH))
#define COL_MASK ((1 << COL_WIDTH) - 1)

typedef union {
	struct {
		uint32_t col: COL_WIDTH;
		uint32_t set: SET_WIDTH;
		uint32_t tag: TAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

typedef struct {
    uint8_t data[NR_COL];
	uint32_t tag;
    bool valid;
	bool dirty;
} Block;

Block L2_cache[NR_SET][NR_ROW];

void init_L2_cache() {
	int i, j;
	for(i = 0; i < NR_SET; i ++) {
		for(j = 0; j < NR_ROW; j ++) {
			L2_cache[i][j].valid = false;
			L2_cache[i][j].dirty = false;
		}
	}
}

int L2_cache_miss(hwaddr_t addr) {
    int i, j;
    cache_addr caddr;
    caddr.addr = addr;
    uint32_t set = caddr.set;
    for(i = 0; i < NR_ROW; i++) {
        if(L2_cache[set][i].valid == false)
            break;
    }
    if(i == NR_ROW) {
        i = rand() % NR_ROW;
    }
	// write back
	if(L2_cache[set][i].dirty == true) {
		caddr.tag = L2_cache[set][i].tag;
		caddr.col = 0;
		for(j = 0; j < NR_COL; j += 4) {
			dram_write(caddr.addr + j, 4, unalign_rw(L2_cache[set][i].data + j, 4));
		}
	}
    addr = addr & ~COL_MASK;  // set col filed to zero
    for(j = 0; j < NR_COL; j += 4) {
        unalign_rw(L2_cache[set][i].data + j, 4) = dram_read(addr + j, 4);
    }
	L2_cache[set][i].dirty = false;
    L2_cache[set][i].valid = true;
    L2_cache[set][i].tag = caddr.tag;
    return i;
}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) {
    Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

    cache_addr caddr;
    caddr.addr = addr;
    uint32_t col = caddr.col;
    uint32_t set = caddr.set;
    uint32_t tag = caddr.tag;
    uint32_t offset = 0;
    uint8_t temp[8];
    int i;
    // addr cross boundary
    if(col + len > NR_COL) {
        unalign_rw(temp + NR_COL - col, 4) = L2_cache_read(addr + NR_COL - col, len);
        Assert(addr + NR_COL - col == ((addr + NR_COL - col) & ~COL_MASK),
               "second L2_cache read fail, addr: %x", addr + NR_COL - col);
        len = NR_COL - col;
    }
    for(i = 0; i < NR_ROW; i++) {
        if(L2_cache[set][i].valid && L2_cache[set][i].tag == tag) {
            memcpy(temp, &L2_cache[set][i].data[col], len);
            return unalign_rw(temp + offset, 4);
        }
    }
    // L2_cache miss handle:
    i = L2_cache_miss(addr);
    memcpy(temp, &L2_cache[set][i].data[col], len);
    return unalign_rw(temp + offset, 4);
}

void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data) {
    Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

    int i;
    cache_addr caddr;
    caddr.addr = addr;
    uint32_t col = caddr.col;
    uint32_t set = caddr.set;
    uint32_t tag = caddr.tag;
    // addr cross boundary
    if(col + len > NR_COL) {
        L2_cache_write(addr + NR_COL - col, len - (NR_COL - col), data >> (8*(NR_COL - col)));
        len = NR_COL - col;
    }
    for(i = 0; i < NR_ROW; i++) {
        if(L2_cache[set][i].valid && L2_cache[set][i].tag == tag) {
            memcpy(&L2_cache[set][i].data[col], &data, len);
			L2_cache[set][i].dirty = true;
            return;
        }
    }
    i = L2_cache_miss(addr);
    memcpy(&L2_cache[set][i].data[col], &data, len);
	L2_cache[set][i].dirty = true;
}

void L2_debug_cache(hwaddr_t addr) {
    cache_addr caddr;
    caddr.addr = addr;
    uint32_t col = caddr.col;
    uint32_t set = caddr.set;
    uint32_t tag = caddr.tag;
    int i, j;
    printf("addr = 0x%08x, tag = 0x%x, set = 0x%x, col = 0x%x\n", addr, tag, set, col);
    for(i = 0; i < NR_ROW; i++) {
        if(L2_cache[set][i].valid && L2_cache[set][i].tag == tag) {
            printf("Hit L2_cache[0x%x][%d]\n", set, i);
            printf("address 0x%08x data: 0x%02x\n", addr, L2_cache[set][i].data[col]);
            printf("L2_cache block begin at %08x:\n", addr & ~COL_MASK);
            for(j = 0; j < NR_COL; ++j){
                printf("%02x ", L2_cache[set][i].data[j]);
                if(j % 16 == 15) printf("\n");
            }
            printf("\n");
            return;
        }
    }
    printf("Cache miss!\n");
    for(i = 0; i < NR_ROW; ++i){
        printf("L2_cache[0x%x][%d]: valid:%d, tag:0x%x\n", set, i, L2_cache[set][i].valid, L2_cache[set][i].tag);
    }
}

#undef CACHE_WIDTH
#undef CACHE_SIZE
#undef COL_WIDTH
#undef ROW_WIDTH
#undef SET_WIDTH
#undef TAG_WIDTH
#undef NR_COL
#undef NR_ROW
#undef NR_SET
#undef HW_MEM_SIZE
#undef COL_MASK
