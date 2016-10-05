#include "cpu/exec/helper.h"

make_helper(movsx_rm2r_b2w) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         int8_t t1 = (int8_t)reg_b(op_src->reg);
         int16_t t2 = t1;
         reg_w(op_dest->reg) = (uint16_t)t2;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        int8_t t1 = (int8_t)swaddr_read(op_src->addr, 1);
        int16_t t2 = t1;
        reg_w(op_dest->reg) = (uint16_t)t2;
    }

    print_asm("movsx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movsx_rm2r_b2l) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         int8_t t1 = (int8_t)reg_b(op_src->reg);
         int32_t t2 = t1;
         reg_l(op_dest->reg) = (uint32_t)t2;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        int8_t t1 = (int8_t)swaddr_read(op_src->addr, 1);
        int32_t t2 = t1;
        reg_l(op_dest->reg) = (uint32_t)t2;
    }

    print_asm("movsx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movsx_rm2r_w2l) {
    int len = decode_rm2r_b(eip + 1);

    if(op_src->type == OP_TYPE_REG) {
         int16_t t1 = (int16_t)reg_w(op_src->reg);
         int32_t t2 = t1;
         reg_l(op_dest->reg) = (uint32_t)t2;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        int16_t t1 = (int16_t)swaddr_read(op_src->addr, 2);
        int32_t t2 = t1;
        reg_l(op_dest->reg) = (uint32_t)t2;
    }

    print_asm("movsx %s,%s", op_src->str, op_dest->str);
    return len + 1;
}

make_helper(movsx_rm2r_v) {
    if(ops_decoded.is_operand_size_16)
        return movsx_rm2r_b2w(eip);
    else
        return movsx_rm2r_b2l(eip);
}
