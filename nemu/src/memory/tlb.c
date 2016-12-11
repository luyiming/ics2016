#include "common.h"
#include "stdlib.h"
#include "time.h"
#include "../../lib-common/x86-inc/mmu.h"
#define ADDRESS_BITS 32

#define OFFSET_WIDTH 12
#define LINE_WIDTH 6
#define TAG_WIDTH (ADDRESS_BITS - OFFSET_WIDTH)

#define NR_LINE (1 << LINE_WIDTH)


typedef union {
	struct {
		uint32_t offset	: OFFSET_WIDTH;
		uint32_t tag	: TAG_WIDTH;
	};
	uint32_t val;
} tlb_addr;

typedef	struct {
	PTE pte;
	uint32_t valid : 1;
	uint32_t tag : TAG_WIDTH;
} tlb_block;

tlb_block tlb[NR_LINE];

void init_tlb(){
	int i = 0;
	for(i = 0; i < NR_LINE; ++i)
		tlb[i].valid = 0;
}

PTE page_read(lnaddr_t);
hwaddr_t tlb_read(lnaddr_t addr){
	tlb_addr tlbaddr;
	tlbaddr.val = addr;
	uint32_t i;
	for(i = 0; i < NR_LINE; ++i){
		if(tlb[i].valid == 0) break;
		if(tlb[i].tag == tlbaddr.tag)
			return (tlb[i].pte.page_frame << 12) + tlbaddr.offset;
	}
	if(i == NR_LINE){
		srand(time(0));
		i = rand() % NR_LINE;
	}
	tlb[i].valid = true;
	tlb[i].tag = tlbaddr.tag;
	tlb[i].pte = page_read(addr);
	return (tlb[i].pte.page_frame << 12) + tlbaddr.offset;
}
