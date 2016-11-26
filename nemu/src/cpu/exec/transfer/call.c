#include "cpu/exec/helper.h"

make_helper(call_rel32) {
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, eip + 5, R_SS);

    int32_t rel32 = instr_fetch(eip + 1, 4);
    cpu.eip = (int32_t)cpu.eip + rel32;

	print_asm("call %x", cpu.eip + 5);
	return 1 + 4;
}

make_helper(call_rm_w) {
    int len = decode_rm_w(eip + 1);

    cpu.esp -= 2;
    swaddr_write(cpu.esp, 2, eip + len + 1, R_SS);

    cpu.eip = op_src->val;

    print_asm("call *%%%s(0x%x)", regsw[op_src->reg], cpu.eip);
    cpu.eip -= (1 + len);
    return 1 + len; // eip = eip + 1 + len
}

make_helper(call_rm_l) {
    int len = decode_rm_l(eip + 1);

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, eip + len + 1, R_SS);

    cpu.eip = op_src->val;

    print_asm("call *%%%s(0x%x)", regsl[op_src->reg], cpu.eip);
    cpu.eip -= (1 + len);
    return 1 + len; // eip = eip + 1 + len
}

make_helper(call_rm_v) {
    if(ops_decoded.is_operand_size_16)
        return call_rm_w(eip);
    else
        return call_rm_l(eip);
}
