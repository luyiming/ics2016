#include "cpu/exec/helper.h"

make_helper(lgdt) {
    cpu.GDTR.Limit = instr_fetch(eip + 1, 2);
    cpu.GDTR.Base  = instr_fetch(eip + 3, 4);
	print_asm("lgdt");
	return 7;
}
