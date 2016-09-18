#include "cpu/exec/helper.h"

make_helper(ja_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("ja 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.CF == 0 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jae_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jae 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.CF == 0 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jb_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jb 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.CF == 1 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jbe_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jbe 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.CF == 1 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(je_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("je 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jne_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jne 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jg_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jg 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF == cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jge_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jge 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF == cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jl_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jl 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF != cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jle_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jle 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF != cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jno_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jno 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.OF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jnp_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jnp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.PF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jns_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jns 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jo_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jo 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.OF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(jp_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("jp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.PF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

make_helper(js_b) {
	int8_t rel = instr_fetch(eip + 1, 1);
    print_asm("js 0x%x", (int32_t)cpu.eip + (int32_t)rel + 2);
    if(cpu.SF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 2;
}

/***************** disp 16 *********************/
make_helper(ja_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("ja 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.CF == 0 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jae_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jae 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.CF == 0 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jb_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jb 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.CF == 1 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jbe_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jbe 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.CF == 1 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(je_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("je 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jne_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jne 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jg_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jg 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF == cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jge_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jge 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF == cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jl_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jl 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF != cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jle_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jl 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF != cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jno_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jno 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.OF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jnp_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jnp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.PF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jns_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jns 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF == 0)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jo_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jo 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.OF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(jp_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("jp 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.PF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

make_helper(js_w) {
	int16_t rel = instr_fetch(eip + 1, 2);
    print_asm("js 0x%x", (int32_t)cpu.eip + (int32_t)rel + 5);
    if(cpu.SF == 1)
        cpu.eip = (int32_t)cpu.eip + (int32_t)rel;
	return 4; // opcode + disp (without prefix)
}

/***************** disp 32 *********************/
make_helper(ja_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("ja 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.CF == 0 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jae_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jae 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.CF == 0 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jb_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jb 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.CF == 1 && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jbe_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jbe 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.CF == 1 || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(je_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("je 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jne_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jne 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jg_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jg 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF == cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jge_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jge 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF == cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jl_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jl 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF != cpu.OF && cpu.ZF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jle_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jl 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF != cpu.OF || cpu.ZF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jno_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jno 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.OF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jnp_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jnp 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.PF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jns_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jns 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF == 0)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jo_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jo 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.OF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(jp_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("jp 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.PF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

make_helper(js_l) {
	int32_t rel = instr_fetch(eip + 1, 4);
    print_asm("js 0x%x", (int32_t)cpu.eip + rel + 6);
    if(cpu.SF == 1)
        cpu.eip = (int32_t)cpu.eip + rel;
	return 5; // opcode + disp (without prefix)
}

/***************** helper v *********************/
make_helper(ja_v) {
    if(ops_decoded.is_operand_size_16)
        return ja_w(eip);
    else
        return ja_l(eip);
}

make_helper(jae_v) {
    if(ops_decoded.is_operand_size_16)
        return jae_w(eip);
    else
        return jae_l(eip);
}

make_helper(jb_v) {
    if(ops_decoded.is_operand_size_16)
        return jb_w(eip);
    else
        return jb_l(eip);
}

make_helper(jbe_v) {
    if(ops_decoded.is_operand_size_16)
        return jbe_w(eip);
    else
        return jbe_l(eip);
}

make_helper(je_v) {
    if(ops_decoded.is_operand_size_16)
        return je_w(eip);
    else
        return je_l(eip);
}

make_helper(jne_v) {
    if(ops_decoded.is_operand_size_16)
        return jne_w(eip);
    else
        return jne_l(eip);
}

make_helper(jg_v) {
    if(ops_decoded.is_operand_size_16)
        return jg_w(eip);
    else
        return jg_l(eip);
}

make_helper(jge_v) {
    if(ops_decoded.is_operand_size_16)
        return jge_w(eip);
    else
        return jge_l(eip);
}

make_helper(jl_v) {
    if(ops_decoded.is_operand_size_16)
        return jl_w(eip);
    else
        return jl_l(eip);
}

make_helper(jle_v) {
    if(ops_decoded.is_operand_size_16)
        return jle_w(eip);
    else
        return jle_l(eip);
}

make_helper(jno_v) {
    if(ops_decoded.is_operand_size_16)
        return jno_w(eip);
    else
        return jno_l(eip);
}

make_helper(jnp_v) {
    if(ops_decoded.is_operand_size_16)
        return jnp_w(eip);
    else
        return jnp_l(eip);
}

make_helper(jns_v) {
    if(ops_decoded.is_operand_size_16)
        return jns_w(eip);
    else
        return jns_l(eip);
}

make_helper(jo_v) {
    if(ops_decoded.is_operand_size_16)
        return jo_w(eip);
    else
        return jo_l(eip);
}

make_helper(jp_v) {
    if(ops_decoded.is_operand_size_16)
        return jp_w(eip);
    else
        return jp_l(eip);
}

make_helper(js_v) {
    if(ops_decoded.is_operand_size_16)
        return js_w(eip);
    else
        return js_l(eip);
}
