#include "cpu/exec/helper.h"

make_helper(cmps_b) {
    uint8_t x = swaddr_read(reg_l(R_ESI), 1, R_DS);
    uint8_t y = swaddr_read(reg_l(R_EDI), 1, R_ES);
    uint8_t z = x - y;
    int8_t zs = x - y;

    cpu.CF = (x < y ? 1 : 0);
    cpu.PF = get_parity(z);
    cpu.AF = 0;
    cpu.ZF = (z == 0 ? 1 : 0);
    cpu.SF = (zs < 0 ? 1 : 0);

    int neg_over = x < 0 && y > 0 && zs >= 0;
    int pos_over = x >= 0 && y < 0 && zs < 0;
    cpu.OF = neg_over || pos_over;

    if(cpu.DF == 0) {
        reg_l(R_ESI) += 1;
        reg_l(R_EDI) += 1;
    }
    else {
        reg_l(R_ESI) -= 1;
        reg_l(R_EDI) -= 1;
    }
    print_asm("cmpsb");
    return 1;
}

make_helper(cmps_w) {
    uint16_t x = swaddr_read(reg_l(R_ESI), 2, R_DS);
    uint16_t y = swaddr_read(reg_l(R_EDI), 2, R_ES);
    uint16_t z = x - y;
    int16_t zs = x - y;

    cpu.CF = (x < y ? 1 : 0);
    cpu.PF = get_parity(z);
    cpu.AF = 0;
    cpu.ZF = (z == 0 ? 1 : 0);
    cpu.SF = (zs < 0 ? 1 : 0);

    int neg_over = x < 0 && y > 0 && zs >= 0;
    int pos_over = x >= 0 && y < 0 && zs < 0;
    cpu.OF = neg_over || pos_over;

    if(cpu.DF == 0) {
        reg_l(R_ESI) += 2;
        reg_l(R_EDI) += 2;
    }
    else {
        reg_l(R_ESI) -= 2;
        reg_l(R_EDI) -= 2;
    }
    print_asm("cmpsw");
    return 1;
}

make_helper(cmps_l) {
    uint32_t x = swaddr_read(reg_l(R_ESI), 4, R_DS);
    uint32_t y = swaddr_read(reg_l(R_EDI), 4, R_ES);
    uint32_t z = x - y;
    int32_t zs = x - y;

    cpu.CF = (x < y ? 1 : 0);
    cpu.PF = get_parity(z);
    cpu.AF = 0;
    cpu.ZF = (z == 0 ? 1 : 0);
    cpu.SF = (zs < 0 ? 1 : 0);

    int neg_over = x < 0 && y > 0 && zs >= 0;
    int pos_over = x >= 0 && y < 0 && zs < 0;
    cpu.OF = neg_over || pos_over;

    if(cpu.DF == 0) {
        reg_l(R_ESI) += 4;
        reg_l(R_EDI) += 4;
    }
    else {
        reg_l(R_ESI) -= 4;
        reg_l(R_EDI) -= 4;
    }
    print_asm("cmpsl");
    return 1;
}

make_helper(cmps_v) {
    if(ops_decoded.is_operand_size_16)
        return cmps_w(eip);
    else
        return cmps_l(eip);
}
