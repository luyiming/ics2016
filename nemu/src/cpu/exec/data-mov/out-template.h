/*#include "cpu/exec/template-start.h"

#define instr out

void pio_write(ioaddr_t, size_t, uint32_t);

make_helper(concat(out_, SUFFIX)) {
	pio_write(reg_w(R_DX), DATA_BYTE, concat(reg_, SUFFIX)(R_EAX));
    print_asm("out %%%s, (%%dx)", concat(regs, SUFFIX)[R_EAX]);
	return 1;
}

// TODO: fix
make_helper(concat(out_imm_, SUFFIX)) {
    uint8_t port = instr_fetch(eip + 1, 1);
    pio_write(port, DATA_BYTE, concat(reg_, SUFFIX)(R_EAX));
    print_asm("out %%%s, %%%d", concat(regs, SUFFIX)[R_EAX], port);
	return 2;
}


#include "cpu/exec/template-end.h"
*/

#include "cpu/exec/template-start.h"

#define instr out

void pio_write(ioaddr_t, size_t, uint32_t);

make_helper(concat(out_, SUFFIX)) {
	pio_write(reg_w(R_DX), 1, reg_b(R_AL));
	print_asm("out" " %%al,(%%dx)");
	return 1;
}

make_helper(concat(out_imm_, SUFFIX)) {
	panic("not implemented");
	return 0;
}

#include "cpu/exec/template-end.h"
