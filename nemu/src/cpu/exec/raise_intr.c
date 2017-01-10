#include <setjmp.h>
#include "nemu.h"
#include "cpu/helper.h"

extern jmp_buf jbuf;

void desc_add(uint8_t sr);

void raise_intr(uint8_t NO) {
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, R_SS, cpu.eflags);
	cpu.IF = 0;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, R_SS, cpu.cs);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, R_SS, cpu.eip);
	
	lnaddr_t addr_desc = (cpu.IDTR.val >> 16) + (NO << 3);
	cpu.cs = lnaddr_read(addr_desc + 2, 2);
	desc_add(R_CS);
	cpu.eip = (lnaddr_read(addr_desc + 6, 2) << 16) + lnaddr_read(addr_desc, 2);

	/* jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
