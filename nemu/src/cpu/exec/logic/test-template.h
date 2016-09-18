#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
    DATA_TYPE result = op_dest->val & op_src->val;

	/* Update EFLAGS. */
    cpu.OF = 0;
    cpu.CF = 0;
    // cpu.AF undefined
    cpu.SF = MSB(result);
    cpu.ZF = (result == 0 ? 1 : 0);
    cpu.PF = get_parity(result);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
