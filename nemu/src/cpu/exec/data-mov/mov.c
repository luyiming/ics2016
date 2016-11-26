#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)

extern const char* regsl[];

make_helper(mov_cr2r) {
	// ModR/M: mod(2 bits) reg(3 bits) r/m(3 bits)
	// mod field always be 11
	// reg field specify special registers
	// r/m field specify general registers
	uint8_t r = instr_fetch(eip + 1, 1);
	if(((r >> 3) & 0x7) == 0)
		reg_l(r & 0x7) = cpu.CR0.val;
	else
		reg_l(r & 0x7) = cpu.CR3.val;
	print_asm("mov %%cr%d,%%%s", (r >> 3) & 0x7, regsl[r & 0x7]);
	return 2;
}

make_helper(mov_r2cr) {
	uint8_t r = instr_fetch(eip + 1, 1);
	if(((r >> 3) & 0x7) == 0)
		cpu.CR0.val = reg_l(r & 0x7);
	else
		cpu.CR3.val = reg_l(r & 0x7);
	print_asm("mov %%%s,%%cr%d", regsl[r & 0x7], (r >> 3) & 0x7);
	return 2;
}
