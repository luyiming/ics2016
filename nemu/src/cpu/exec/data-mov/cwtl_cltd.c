#include "cpu/exec/helper.h"

make_helper(cwtl) {
    int16_t val = (uint16_t)reg_w(R_AX);
    reg_w(R_DX) = (val < 0 ? 0xffff : 0);
    print_asm("cwtl");
    return 1;
}

make_helper(cltd) {
    int32_t val = (uint32_t)reg_l(R_EAX);
    reg_l(R_EDX) = (val < 0 ? 0xffffffff : 0);
    print_asm("cltd");
    return 1;
}

make_helper(cwtl_cltd) {
    if(ops_decoded.is_operand_size_16)
        return cwtl(eip);
    else
        return cltd(eip);
}
