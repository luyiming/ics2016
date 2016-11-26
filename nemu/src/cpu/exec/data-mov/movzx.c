#include "cpu/exec/helper.h"

make_helper(movzx_rm2r_b2w) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         reg_w(op_dest->reg) = (uint16_t)reg_b(op_src->reg);
    }
    else if(op_src->type == OP_TYPE_MEM) {
        uint8_t t = (uint8_t)swaddr_read(op_src->addr, 1, op_src->sreg);
        reg_w(op_dest->reg) = (uint16_t)t;
    }

    print_asm("movzx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movzx_rm2r_b2l) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         reg_l(op_dest->reg) = (uint32_t)reg_b(op_src->reg);
    }
    else if(op_src->type == OP_TYPE_MEM) {
        uint8_t t = (uint8_t)swaddr_read(op_src->addr, 1, op_src->sreg);
        reg_l(op_dest->reg) = (uint32_t)t;
    }

    print_asm("movzx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movzx_rm2r_w2l) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         reg_l(op_dest->reg) = (uint32_t)reg_w(op_src->reg);
    }
    else if(op_src->type == OP_TYPE_MEM) {
        uint16_t t = (uint16_t)swaddr_read(op_src->addr, 2, op_src->sreg);
        reg_l(op_dest->reg) = (uint32_t)t;
    }

    print_asm("movzx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movzx_rm2r_v) {
    if(ops_decoded.is_operand_size_16)
        return movzx_rm2r_b2w(eip);
    else
        return movzx_rm2r_b2l(eip);
}
