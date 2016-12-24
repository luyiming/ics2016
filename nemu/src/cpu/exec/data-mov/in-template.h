#include "cpu/exec/template-start.h"

#define instr in

uint32_t pio_read(ioaddr_t, size_t);

make_helper(concat(in_, SUFFIX)) {
	concat(reg_, SUFFIX)(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);
	print_asm("in (%%dx), %%%s", concat(regs, SUFFIX)[R_EAX]);
	return 1;
}

make_helper(concat(in_imm_, SUFFIX)) {
    uint8_t port = instr_fetch(eip + 1, 1);
	concat(reg_, SUFFIX)(R_EAX) = pio_read(port, DATA_BYTE);
	print_asm("in %%%d, %%%s", port, concat(regs, SUFFIX)[R_EAX]);
	return 2;
}


#include "cpu/exec/template-end.h"
