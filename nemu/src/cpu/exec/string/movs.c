#include "cpu/exec/helper.h"

make_helper(movs_b) {
    uint8_t val = swaddr_read(reg_l(R_ESI), 1, R_DS);
    swaddr_write(reg_l(R_EDI), 1, val, R_ES);
    if(cpu.DF == 0) {
        reg_l(R_ESI) += 1;
        reg_l(R_EDI) += 1;
    }
    else {
        reg_l(R_ESI) -= 1;
        reg_l(R_EDI) -= 1;
    }
    print_asm("movs 0x%x,0x%x", reg_l(R_ESI), reg_l(R_EDI));
    return 1;
}

make_helper(movs_w) {
    uint16_t val = swaddr_read(reg_l(R_ESI), 2, R_DS);
    swaddr_write(reg_l(R_EDI), 2, val, R_ES);
    if(cpu.DF == 0) {
        reg_l(R_ESI) += 2;
        reg_l(R_EDI) += 2;
    }
    else {
        reg_l(R_ESI) -= 2;
        reg_l(R_EDI) -= 2;
    }
    print_asm("movs 0x%x,0x%x", reg_l(R_ESI), reg_l(R_EDI));
    return 1;
}

make_helper(movs_l) {
    uint32_t val = swaddr_read(reg_l(R_ESI), 4, R_DS);
    swaddr_write(reg_l(R_EDI), 4, val, R_ES);
    if(cpu.DF == 0) {
        reg_l(R_ESI) += 4;
        reg_l(R_EDI) += 4;
    }
    else {
        reg_l(R_ESI) -= 4;
        reg_l(R_EDI) -= 4;
    }
    print_asm("movs 0x%x,0x%x", reg_l(R_ESI), reg_l(R_EDI));
    return 1;
}

make_helper(movs_v) {
    if(ops_decoded.is_operand_size_16)
        return movs_w(eip);
    else
        return movs_l(eip);
}
