#include "nemu.h"

void init_seg() {
	int i;
	for(i = 0; i < 4; ++i)
		cpu.seg_cache[i].valid = 0;
	cpu.seg_cache[R_CS].Base = 0;
	cpu.seg_cache[R_CS].Limit = 0xffffffff;
}

#define load_sreg desc_add
void desc_add(uint32_t sreg) {
	// load seg_descriptor into cache
    uint8_t tmp[8];
	int i;
    for(i = 0; i < 8; i++) {
        tmp[i] = lnaddr_read(cpu.GDTR.Base + (cpu.SR[sreg].Index << 3) + i, 1);
    }
    SegDesc *segdesc = (SegDesc*)tmp;
    uint32_t Limit = (segdesc->limit_19_16 << 16) + segdesc->limit_15_0;
	uint32_t Base = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
	//Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.GDTR.Base + cpu.SR[sreg].Index * 8);
	cpu.seg_cache[sreg].valid = true;
	cpu.seg_cache[sreg].Limit = Limit;
	cpu.seg_cache[sreg].Base = Base;
	cpu.seg_cache[sreg].DPL = segdesc->privilege_level;
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
    // sreg:
    // 00 ES
    // 01 CS
    // 10 SS
    // 11 DS
    Assert(sreg >= 0 && sreg <= 3, "sreg index out of range");
    if(cpu.CR0.protect_enable == 0)
        return addr;

    if(cpu.seg_cache[sreg].valid == 0)
        desc_add(sreg);
	return cpu.seg_cache[sreg].Base + addr;
}
