#include "cpu/exec/template-start.h"

#define	instr	ret

extern CPU_state cpu;

static void do_execute() {
	cpu.eip = (DATA_TYPE)MEM_R(REG(R_ESP), S_SS);
	REG(R_ESP) += DATA_BYTE;
	/* to print asm in helper func */
}

make_helper(concat(ret_, SUFFIX)) {
	do_execute();
	
	print_asm("ret" str(SUFFIX));	
	cpu.eip -= 1;
	return 1;
}

make_helper(concat(ret_i_, SUFFIX)) {
	decode_i_w(eip + 1);	
	do_execute();
	REG(R_ESP) += op_src->val;
	
	print_asm_template1();
	cpu.eip -= 3;
	return 3;
}

#include "cpu/exec/template-end.h"
