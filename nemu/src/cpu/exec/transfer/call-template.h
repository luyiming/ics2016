#include "cpu/exec/template-start.h"

#define instr	call

extern CPU_state cpu;

make_helper(concat(call_i_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), R_SS, eip + len);
	cpu.eip += op_src->val + len;
	if (DATA_BYTE == 2) {
		cpu.eip &= 0xffff;
	}

	cpu.eip -= len;
	print_asm_template1();	
	return len;
}

make_helper(concat(call_rm_, SUFFIX)) {
	int len = 1 + concat(decode_rm_, SUFFIX)(eip + 1);
	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), R_SS, eip + len);
	cpu.eip = op_src->val;
	if (DATA_BYTE == 2) {
		cpu.eip &= 0xffff;
	}

	cpu.eip -= len;
	print_asm_template1();
	return len;
}

#include "cpu/exec/template-end.h"
