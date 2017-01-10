#include "cpu/exec/helper.h"

//void load_sreg(uint32_t);
void desc_add(uint8_t sr);

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS) - 1; //函数返回后eip会增加1
	cpu.esp += 4;

	desc_add(R_CS);
	cpu.CS = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	//load_sreg(R_CS);

	cpu.eflags = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	print_asm("iret");
	return 1;
}
