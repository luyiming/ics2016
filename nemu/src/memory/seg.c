#include "nemu.h"

#define NR_SREG 4

void add_sreg(uint32_t sreg);

void init_seg() {
	int i;
	for(i = 0; i < NR_SREG; ++i) {
		cpu.seg_cache[i].valid = 0;
	}
	cpu.seg_cache[R_CS].Base  = 0;
	cpu.seg_cache[R_CS].Limit = 0xffffffff;
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
    // sreg:
    // 00 ES
    // 01 CS
    // 10 SS
    // 11 DS
    Assert(sreg >= 0 && sreg < NR_SREG, "sreg index out of range");
    if(cpu.CR0.protect_enable == 0) {
		return addr;
	}

    if(cpu.seg_cache[sreg].valid == 0) {
		add_sreg(sreg);
	}

	return cpu.seg_cache[sreg].Base + addr;
}


void add_sreg(uint32_t sreg) {
	// load seg_descriptor into cache
    uint8_t tmp[8];
	SegDesc *segdesc = (SegDesc*)tmp;
	int i;
    for(i = 0; i < 8; ++i) {
        tmp[i] = lnaddr_read(cpu.GDTR.Base + (cpu.SR[sreg].Index << 3) + i, 1);
    }
	// Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.GDTR.Base + cpu.SR[sreg].Index * 8);

    uint32_t Limit = SEG_DESC_LIMIT(segdesc);
	uint32_t Base = SEG_DESC_BASE(segdesc);

	cpu.seg_cache[sreg].valid = true;
	cpu.seg_cache[sreg].Limit = Limit;
	cpu.seg_cache[sreg].Base  = Base;
	cpu.seg_cache[sreg].DPL   = segdesc->privilege_level;
}
