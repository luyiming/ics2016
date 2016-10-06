#include "cpu/exec/template-start.h"

#define instr add

static void do_execute () {
    DATA_TYPE result = op_dest->val + op_src->val;
    OPERAND_W(op_dest, result);

    /*  Update EFLAGS. */
    Assert((DATA_TYPE)op_dest->val == op_dest->val, "add op error");
    Assert((DATA_TYPE)op_src->val == op_src->val, "add op error");

    cpu.CF = (op_dest->val > result || op_src->val > result ? 1 : 0);
    cpu.PF = get_parity(result);
    cpu.AF = 0;
    cpu.ZF = (result == 0 ? 1 : 0);
    cpu.SF = MSB(result);

    // CSAPP ex2.30
    DATA_TYPE_S x = (DATA_TYPE)op_dest->val;
    DATA_TYPE_S y = (DATA_TYPE)op_src->val;
    int neg_over = x < 0 && y < 0 && (DATA_TYPE_S)result >= 0;
    int pos_over = x >= 0 && y >= 0 && (DATA_TYPE_S)result < 0;
    cpu.OF = neg_over || pos_over;

    print_asm_template2();
}
//
make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
