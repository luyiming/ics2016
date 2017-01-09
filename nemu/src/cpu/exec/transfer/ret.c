#include "cpu/exec/helper.h"

#define	DATA_BYTE	2
#include "ret-template.h"
#undef	DATA_BYTE

#define	DATA_BYTE	4
#include "ret-template.h"
#undef	DATA_BYTE

extern CPU_state cpu;
void desc_add(uint8_t sr);

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, S_SS);
	cpu.esp += 4;
	desc_add(S_CS);
	cpu.cs = swaddr_read(cpu.esp, 4, S_SS);
	cpu.esp += 4;
	cpu.eflags = swaddr_read(cpu.esp, 4, S_SS);
	cpu.esp += 4;

	cpu.eip -= 1;
	print_asm("iret");
	return 1;
}

make_helper_v(ret)
make_helper_v(ret_i)
