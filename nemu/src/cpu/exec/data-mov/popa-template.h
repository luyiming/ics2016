#include "cpu/exec/template-start.h"

#define instr popa

static void do_execute() {
	int i;
	for (i = 7; i >= 0; i--) {
		if (i != R_ESP)
			cpu.gpr[i]._32 = swaddr_read(cpu.esp, DATA_BYTE, R_SS);
		cpu.esp += DATA_BYTE;
	}
	print_asm("popa");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
