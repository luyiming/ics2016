#include "cpu/exec/helper.h"

void load_sreg(uint32_t);

make_helper(ljmp) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	uint16_t sreg = instr_fetch(eip + 5, 2);
	cpu.CS = sreg;
	load_sreg(R_CS);
	cpu.eip = addr - 7;
	print_asm("ljmp $0x%x,$0x%x", sreg, addr);
	return 7;
}

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
    int len = decode_rm_w(eip + 1);
    cpu.eip = op_src->val;
    print_asm("jmp 0x%x", cpu.eip);
    cpu.eip -= (1 + len);
    return 1 + len; // eip = eip + 1 + len
}

make_helper(jmp_rm_l) {
    int len = decode_rm_l(eip + 1);
    cpu.eip = op_src->val;
    print_asm("jmp 0x%x", cpu.eip);
    cpu.eip -= (1 + len);
    return 1 + len;  // eip = eip + 1 + len
}

/* for instruction encoding overloading */
make_helper_v(jmp_i)
make_helper_v(jmp_rm)
