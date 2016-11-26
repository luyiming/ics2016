#include "cpu/exec/helper.h"

make_helper(lods_b) {
    reg_b(R_AL) = swaddr_read(reg_l(R_ESI), 1, R_DS);
    if(cpu.DF == 0)
        reg_l(R_ESI) += 1;
    else
        reg_l(R_ESI) -= 1;
    print_asm("lodsb 0x%x", reg_l(R_ESI));
    return 1;
}

make_helper(lods_w) {
    reg_w(R_AX) = swaddr_read(reg_l(R_ESI), 2, R_DS);
    if(cpu.DF == 0)
        reg_l(R_ESI) += 2;
    else
        reg_l(R_ESI) -= 2;
    print_asm("lodsw 0x%x", reg_l(R_ESI));
    return 1;
}

make_helper(lods_l) {
    reg_l(R_EAX) = swaddr_read(reg_l(R_ESI), 4, R_DS);
    if(cpu.DF == 0)
        reg_l(R_ESI) += 4;
    else
        reg_l(R_ESI) -= 4;
    print_asm("lodsl 0x%x", reg_l(R_ESI));
    return 1;
}

make_helper(lods_v) {
    if(ops_decoded.is_operand_size_16)
        return lods_w(eip);
    else
        return lods_l(eip);
}
