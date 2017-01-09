#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	int i, count = 0;
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);

	/* Update EFLAGS. */
	//panic("please implement me");
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.SF = MSB(result);
	cpu.ZF = (result == 0);
	result &= 0xff;
	for (i = 0; i < 8; i++, result >>= 1) {
		count += (result & 0x1);
	}
	cpu.PF = (count % 2 == 0);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
