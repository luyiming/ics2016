#include "nemu.h"

void init_seg() {
	int i;
	for(i = 0; i < 4; ++i)
		cpu.SR_cache[i].valid = 0;
	cpu.SR_cache[R_CS].base = 0;
	cpu.SR_cache[R_CS].limit = 0xffffffff;
}

void load_sreg(uint32_t sreg) {
	// load seg_descriptor into cache
    uint8_t tmp[8];
	int i;
	printf("desc: sreg %d, 0x%x\n", sreg, cpu.SR[sreg].Index);
    for(i = 0; i < 8; i++) {
        tmp[i] = lnaddr_read(cpu.GDTR.Base + cpu.SR[sreg].Index * 8 + i, 1);
		printf("%x ", tmp[i]);
    }
	printf("\n");
    SegDesc *segdesc = (SegDesc*)tmp;
    uint32_t limit = (segdesc->limit_19_16 << 16) + segdesc->limit_15_0;
	uint32_t base = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
	printf("present %d\n", segdesc->present);
	printf("limit %x\n", limit);
	printf("base %x\n", base);
	//Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.GDTR.Base + cpu.SR[sreg].Index * 8);
	//Assert(cpu.SR[sreg].Index * 8 < limit, "Segment overflow: limit %d", cpu.SR_cache[sreg].limit);
	cpu.SR_cache[sreg].valid = true;
	cpu.SR_cache[sreg].limit = limit;
	cpu.SR_cache[sreg].base = base;
	cpu.SR_cache[sreg].dpl = segdesc->privilege_level;
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

    if(cpu.SR_cache[sreg].valid == 0)
        load_sreg(sreg);
	return cpu.SR_cache[sreg].base + addr;
}
