#include "cpu/exec/helper.h"

make_helper(seta) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.CF == 0 && cpu.ZF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("seta %s", op_src->str);
    return len + 1;
}

make_helper(setae) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.CF == 0 || cpu.ZF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setae %s", op_src->str);
    return len + 1;
}

make_helper(setb) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.CF == 1 && cpu.ZF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setb %s", op_src->str);
    return len + 1;
}

make_helper(setbe) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.CF == 1 || cpu.ZF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setbe %s", op_src->str);
    return len + 1;
}

make_helper(sete) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.ZF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("sete %s", op_src->str);
    return len + 1;
}

make_helper(setne) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.ZF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setne %s", op_src->str);
    return len + 1;
}

make_helper(setg) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF == cpu.OF && cpu.ZF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setg %s", op_src->str);
    return len + 1;
}

make_helper(setge) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF == cpu.OF || cpu.ZF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setge %s", op_src->str);
    return len + 1;
}

make_helper(setl) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF != cpu.OF && cpu.ZF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setl %s", op_src->str);
    return len + 1;
}

make_helper(setle) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF != cpu.OF || cpu.ZF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setle %s", op_src->str);
    return len + 1;
}

make_helper(setno) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.OF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setno %s", op_src->str);
    return len + 1;
}

make_helper(setnp) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.PF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setnp %s", op_src->str);
    return len + 1;
}

make_helper(setns) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF == 0)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setns %s", op_src->str);
    return len + 1;
}

make_helper(seto) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.OF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("seto %s", op_src->str);
    return len + 1;
}

make_helper(setp) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.PF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("setp %s", op_src->str);
    return len + 1;
}

make_helper(sets) {
    int len = decode_rm_b(eip + 1);
    int cc = 0;
    if(cpu.SF == 1)
        cc = 1;

    if(op_src->type == OP_TYPE_REG) {
         reg_b(op_src->reg) = cc;
    }
    else if(op_src->type == OP_TYPE_MEM) {
        swaddr_write(op_src->addr, 1, op_src->sreg, cc);
    }

    print_asm("sets %s", op_src->str);
    return len + 1;
}
