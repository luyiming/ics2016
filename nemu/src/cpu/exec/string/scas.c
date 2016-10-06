#include "cpu/exec/helper.h"

make_helper(scas_b) {
    uint8_t x = reg_b(R_AL);
    uint8_t y = swaddr_read(reg_l(R_EDI), 1);
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
        reg_l(R_EDI) += 1;
    }
    else {
        reg_l(R_EDI) -= 1;
    }
    print_asm("scasb");
    return 1;
}

make_helper(scas_w) {
    uint16_t x = reg_w(R_AX);
    uint16_t y = swaddr_read(reg_l(R_EDI), 2);
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
        reg_l(R_EDI) += 2;
    }
    else {
        reg_l(R_EDI) -= 2;
    }
    print_asm("scasw");
    return 1;
}

make_helper(scas_l) {
    uint32_t x = reg_l(R_EAX);
    uint32_t y = swaddr_read(reg_l(R_EDI), 4);
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
        reg_l(R_EDI) += 4;
    }
    else {
        reg_l(R_EDI) -= 4;
    }
    print_asm("scasl");
    return 1;
}

make_helper(scas_v) {
    if(ops_decoded.is_operand_size_16)
        return scas_w(eip);
    else
        return scas_l(eip);
}
