#include "cpu/exec/helper.h"

make_helper(stos_b) {
    uint8_t val = reg_b(R_AL);
    swaddr_write(reg_l(R_EDI), 1, val);
    if(cpu.DF == 0)
        reg_l(R_EDI) += 1;
    else
        reg_l(R_EDI) -= 1;
    print_asm("stosb 0x%x", reg_l(R_EDI));
    return 1;
}

make_helper(stos_w) {
    uint16_t val = reg_w(R_AX);
    swaddr_write(reg_l(R_EDI), 2, val);
    if(cpu.DF == 0)
        reg_l(R_EDI) += 2;
    else
        reg_l(R_EDI) -= 2;
    print_asm("stosw 0x%x", reg_l(R_EDI));
    return 1;
}

make_helper(stos_l) {
    uint32_t val = reg_l(R_EAX);
    swaddr_write(reg_l(R_EDI), 4, val);
    if(cpu.DF == 0)
        reg_l(R_EDI) += 4;
    else
        reg_l(R_EDI) -= 4;
    print_asm("stosl 0x%x", reg_l(R_EDI));
    return 1;
}

make_helper(stos_v) {
    if(ops_decoded.is_operand_size_16)
        return stos_w(eip);
    else
        return stos_l(eip);
}
