#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	cpu.esp -= DATA_BYTE;
	swaddr_write(cpu.esp, DATA_BYTE, op_src->val);
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4

/*
make_helper(concat(push_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	do_execute();
	return len + 1;
}
*/

make_instr_helper(r)
make_instr_helper(rm)

#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"
