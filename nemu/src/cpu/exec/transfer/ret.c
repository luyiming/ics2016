#include "cpu/exec/helper.h"

make_helper(ret) {
    cpu.eip = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;

    print_asm("ret");
    return 0; // eip has been updated
}

make_helper(ret_imm) {
    cpu.eip = swaddr_read(cpu.esp, 4);
    uint16_t rel = instr_fetch(eip + 1, 2);
    cpu.esp = cpu.esp + 4 + rel;

    print_asm("ret 0x%x", rel);
    return 0; // eip has been updated
}
