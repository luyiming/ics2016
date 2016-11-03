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
	int len = decode_rm2r_l(eip + 1);
	char *cr_name[] = {"cr0", "cr1", "cr2", "cr3"};
	uint32_t data;
	switch(op_src->reg) {
		case 0: data = cpu.CR0; break;
		case 2: data = cpu.CR2; break;
		case 3: data = cpu.CR3; break;
		default: panic("mov %%cr, %%r fail\n");
	}
	reg_l(op_dest->reg) = data;

	print_asm("mov %%%s, %%%s", cr_name[op_src->reg], regsl[op_dest->reg]);
	return 1 + len;
}

make_helper(mov_r2cr) {
	int len = decode_r2rm_l(eip + 1);
	char *cr_name[] = {"cr0", "cr1", "cr2", "cr3"};
	switch(op_dest->reg) {
		case 0: cpu.CR0 = reg_l(op_src->reg); break;
		case 2: cpu.CR2 = reg_l(op_src->reg); break;
		case 3: cpu.CR3 = reg_l(op_src->reg); break;
		default: panic("mov %%r, %%cr fail\n");
	}

	print_asm("mov %%%s, %%%s", regsl[op_src->reg], cr_name[op_dest->reg]);
	return 1 + len;
}
