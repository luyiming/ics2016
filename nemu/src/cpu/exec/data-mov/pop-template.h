#include "cpu/exec/template-start.h"

#define	instr	pop

static void do_execute() {
	OPERAND_W(op_src, MEM_R(REG(R_ESP), S_SS));
	REG(R_ESP) += DATA_BYTE;
	print_asm_template1();
}

make_instr_helper(rm)
make_instr_helper(r)

#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(popa_, SUFFIX)) {
	int i;
	for (i = 7; i >= 0; i --) {
		if (R_EAX + i != R_ESP) {
			REG(R_EAX + i) = MEM_R(REG(R_ESP), S_SS);
		}
		REG(R_ESP) += DATA_BYTE;
	}

	print_asm("popa");
	return 1;
}
#endif
#include "cpu/exec/template-end.h"
