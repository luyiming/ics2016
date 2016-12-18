#include "cpu/exec/template-start.h"

#define instr int
void raise_intr(uint8_t NO);

static void do_execute() {
	print_asm_template1();
	int len = DATA_BYTE + 1;
	cpu.eip += len;
	raise_intr(op_src->val);
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
