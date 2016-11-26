#include "nemu.h"

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
    // sreg:
    // 00 ES
    // 01 CS
    // 10 SS
    // 11 DS
    assert(sreg == 0 || sreg == 1 || sreg == 2 || sreg == 3);
    if(cpu.CR0.protect_enable == 0)
        return addr;

    int i;
    uint8_t tmp[8];
    for(i = 0; i < 8; ++ i){
		tmp[i] = lnaddr_read(cpu.GDTR.Base + cpu.SR[sreg].Index * 8 + i, 1);
	}
    SegDesc *segdesc = (SegDesc*)tmp;
    uint32_t limit = (segdesc->limit_19_16 << 16) + segdesc->limit_15_0;
	uint32_t base = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
    Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.GDTR.Base + cpu.SR[sreg].Index * 8);
	Assert(cpu.SR[sreg].Index * 8 < limit, "Segment overflow!");
	return base + addr;
}
