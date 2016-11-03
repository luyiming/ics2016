#include "cpu/exec/helper.h"

make_helper(lidt) {
    cpu.IDTR.Limit = instr_fetch(eip + 1, 2);
    cpu.IDTR.Base  = instr_fetch(eip + 3, 4);
	print_asm("lidt");
	return 7;
}
