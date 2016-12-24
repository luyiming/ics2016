#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(leave) {
    cpu.esp = cpu.ebp;
    cpu.ebp = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;

    print_asm("leave");
    return 1;
}

make_helper(lgdt) {
    ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	cpu.GDTR.Limit = lnaddr_read(op_src->addr, 2);
	cpu.GDTR.Base  = lnaddr_read(op_src->addr + 2, 4);
	print_asm("lgdt limit:%2x, base:%x", cpu.GDTR.Limit, cpu.GDTR.Base);
	return 1 + len;
}

make_helper(lidt) {
    ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	cpu.IDTR.Limit = lnaddr_read(op_src->addr, 2);
	cpu.IDTR.Base  = lnaddr_read(op_src->addr + 2, 4);
	print_asm("lidt limit:%2x, base:%x", cpu.IDTR.Limit, cpu.IDTR.Base);
	return 1 + len;
}

make_helper(cld){
	cpu.DF = 0;
	print_asm("cld");
	return 1;
}

make_helper(std){
	cpu.DF = 1;
	print_asm("std");
	return 1;
}

make_helper(cli){
	cpu.IF = 0;
	print_asm("cli");
	return 1;
}

make_helper(hlt) {
	while (!cpu.INTR);
	print_asm("hlt");
	return 1;
}
