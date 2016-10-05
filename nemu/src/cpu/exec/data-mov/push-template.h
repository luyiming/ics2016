#include "cpu/exec/template-start.h"

#define instr push

#if DATA_BYTE == 1
// push imm8 stores 4 bytes
static void do_execute () {
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, op_src->val);

    print_asm_template1();
}

#endif

#if DATA_BYTE == 2 || DATA_BYTE == 4

static void do_execute () {
    cpu.esp -= DATA_BYTE;
    swaddr_write(cpu.esp, DATA_BYTE, op_src->val);

    print_asm_template1();
}
/*
make_helper(concat(push_m_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	do_execute();
	return len + 1;
}
*/

make_instr_helper(rm)
make_instr_helper(r)
#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"
