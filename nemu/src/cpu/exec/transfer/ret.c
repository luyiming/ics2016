#include "cpu/exec/helper.h"

make_helper(ret) {
    cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;

    print_asm("ret");
    cpu.eip--;
    return 1; // eip = eip + 1
}

make_helper(ret_imm) {
    cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
    uint16_t rel = instr_fetch(eip + 1, 2);
    cpu.esp = cpu.esp + 4 + rel;

    print_asm("ret 0x%x", rel);
    cpu.eip -= 3;
    return 1 + 2; // eip = eip + 3
}
