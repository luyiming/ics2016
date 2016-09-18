#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute () {
    DATA_TYPE result = op_dest->val - op_src->val;
    OPERAND_W(op_dest, result);

    /*  Update EFLAGS. */
    Assert((DATA_TYPE)op_dest->val == op_dest->val, "sub op error");
    Assert((DATA_TYPE)op_src->val == op_src->val, "sub op error");

    cpu.CF = ((DATA_TYPE)op_dest->val < (DATA_TYPE)op_src->val ? 1 : 0);
    cpu.PF = get_parity(result);
    cpu.AF = 0;
    cpu.ZF = (result == 0 ? 1 : 0);
    cpu.SF = MSB(result);

    // CSAPP 2.74
    /*
    if (y == INT_MIN)
	   return x >= 0;
    int minus_y = -y;
    int sum = x + minus_y;
    int pos_over = !(x & INT_MIN) && !(minus_y & INT_MIN) && (sum & INT_MIN);
    int neg_over = (x & INT_MIN) && (minus_y & INT_MIN) && !(sum & INT_MIN);
    */
    // int32_t x = op_dest->val;
    // int32_t y = op_src->val;
    // int32_t result = x - y
    DATA_TYPE_S x = (DATA_TYPE)op_dest->val;
    DATA_TYPE_S y = (DATA_TYPE)op_src->val;
    int neg_over = x < 0 && y >= 0 && (DATA_TYPE_S)result >= 0;
    int pos_over = x >= 0 && y < 0 && (DATA_TYPE_S)result < 0; // 0 - (-finity)
    cpu.OF = neg_over || pos_over;
/* add overflow
    DATA_TYPE_S x = (DATA_TYPE)op_dest->val;
    DATA_TYPE_S y = (DATA_TYPE)op_src->val;
    int neg_over = x < 0 && y < 0 && (DATA_TYPE_S)result >= 0;
    int pos_over = x >= 0 && y >= 0 && (DATA_TYPE_S)result < 0; // 0 - (-finity)
    cpu.OF = neg_over || pos_over;
*/
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
