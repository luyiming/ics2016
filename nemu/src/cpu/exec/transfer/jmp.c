#include "cpu/exec/helper.h"

make_helper(jmp_i_b) {
    int8_t rel = instr_fetch(eip + 1, 1);
    cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
    print_asm("jmp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    return 2;
}

make_helper(jmp_i_w) {
    int16_t rel = instr_fetch(eip + 1, 2);
    cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
    print_asm("jmp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 3);
    return 3;
}

make_helper(jmp_i_l) {
    int32_t rel = instr_fetch(eip + 1, 4);
    cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
    print_asm("jmp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    return 5;
}

make_helper(jmp_rm_w) {
    decode_rm_w(eip + 1);
    cpu.eip = op_src->val;
    print_asm("jmp 0x%x", cpu.eip);
    return 0; // eip has been updated
}

make_helper(jmp_rm_l) {
    decode_rm_l(eip + 1);
    cpu.eip = op_src->val;
    print_asm("jmp 0x%x", cpu.eip);
    return 0;  // eip has been updated
}

/* for instruction encoding overloading */
make_helper_v(jmp_i)
make_helper_v(jmp_rm)
