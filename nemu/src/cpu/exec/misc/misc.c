#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include <unistd.h>

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

void raise_intr(uint8_t);
extern CPU_state cpu;

make_helper(int_i) {
	uint8_t NO = instr_fetch(eip + 1, 1);
	cpu.eip = eip + 2;
	raise_intr(NO);

	/* should not reach here */
	return 2;
}

make_helper(hlt) {
	while (cpu.IF && (!cpu.INTR)) {
		/* nop */;
	}

	print_asm("hlt");
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

make_helper(lgdt) {
	extern CPU_state cpu;
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	uint16_t limit = lnaddr_read(op_src->addr, 2);
	uint32_t base = lnaddr_read(op_src->addr + 2, 4);
	if (ops_decoded.is_operand_size_16) {
		base = (base & 0x00ffffff);
	}
	cpu.gdtr = ((uint64_t)base << 16) | limit;

	print_asm("lgdt 0x%x", op_src->addr);
	return 1 + len;
}

make_helper(lidt) {
	extern CPU_state cpu;
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	uint16_t limit = lnaddr_read(op_src->addr, 2);
	uint32_t base = lnaddr_read(op_src->addr + 2, 4);
	if (ops_decoded.is_operand_size_16) {
		base = (base & 0x00ffffff);
	}
	cpu.idtr = ((uint64_t)base << 16) | limit;

	print_asm("lidt 0x%x", op_src->addr);
	return 1 + len;
}

make_helper(std) {
	cpu.DF = 1;

	print_asm("std");
	return 1;
}

make_helper(cld) {
	cpu.DF = 0;

	print_asm("cld");
	return 1;
}

make_helper(sti) {
	cpu.IF = 1;

	print_asm("sti");
	return 1;
}

make_helper(cli) {
	cpu.IF = 0;

	print_asm("cli");
	return 1;
}

make_helper(leave) {
    cpu.esp = cpu.ebp;
    cpu.ebp = swaddr_read(cpu.esp, 4, S_SS);
    cpu.esp += 4;

    print_asm("leave");
    return 1;
}
