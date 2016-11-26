#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
    OPERAND_W(op_src, MEM_R(cpu.esp, R_SS));
    cpu.esp += DATA_BYTE;

    print_asm_template1();
}

/*
make_helper(concat(pop_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	do_execute();
	return len + 1;
}
*/

make_instr_helper(rm)
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
