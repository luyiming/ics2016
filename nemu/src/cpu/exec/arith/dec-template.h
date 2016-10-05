#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	cpu.CF = ((DATA_TYPE)op_src->val < 1 ? 1 : 0);
    cpu.PF = get_parity(result);
    cpu.AF = 0;
    cpu.ZF = (result == 0 ? 1 : 0);
    cpu.SF = MSB(result);

	DATA_TYPE_S x = (DATA_TYPE)op_src->val;
	cpu.OF = x < 0 && (DATA_TYPE_S)result >= 0;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
