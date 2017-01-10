#include "common.h"
#include "stdlib.h"
#include "time.h"
#include "../../lib-common/x86-inc/mmu.h"

PTE page_read(lnaddr_t);

#define ADDRESS_WIDTH 32
#define OFFSET_WIDTH  12
#define LINE_WIDTH    6
#define TAG_WIDTH     (ADDRESS_WIDTH - OFFSET_WIDTH)

#define NR_LINE (1 << LINE_WIDTH)
#define NR_TAG  (1 << TAG_WIDTH)

#define TLB_TAG(addr) 	 ((addr >> OFFSET_WIDTH) & ((1 << TAG_WIDTH) - 1))
#define TLB_OFFSET(addr) (addr & ((1 << OFFSET_WIDTH) - 1))

typedef	struct {
	PTE pte;
	uint32_t tag : TAG_WIDTH;
	bool valid;
} TLB_Block_t;

static TLB_Block_t tlb[NR_LINE];

void init_tlb() {
	int i = 0;
	for (i = 0; i < NR_LINE; ++i) {
		tlb[i].valid = false;
	}
}

hwaddr_t tlb_read(lnaddr_t addr) {
	int i;
	for (i = 0; i < NR_LINE; ++i) {
		if (tlb[i].valid == false) {
			break;
		}
		if (tlb[i].tag == TLB_TAG(addr)) {
			return (tlb[i].pte.page_frame << 12) + TLB_OFFSET(addr);
		}
	}
	if (i == NR_LINE){
		srand(time(0));
		i = rand() % NR_LINE;
	}
	tlb[i].valid = true;
	tlb[i].tag = TLB_TAG(addr);
	tlb[i].pte = page_read(addr);
	return (tlb[i].pte.page_frame << 12) + TLB_OFFSET(addr);
}
