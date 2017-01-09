#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

void init_tlb();

make_helper(mov_r2cr) {
	ModR_M m;
#ifdef DEBUG
	char *cr_name;
#endif
	m.val = instr_fetch(eip + 1, 1);
	uint32_t src = reg_l(m.R_M);
	switch (m.reg) {
		case 0:	cpu.CR0.val = src;
#ifdef DEBUG
				cr_name = "cr0";
#endif
				break;
		case 3:
			cpu.CR3.val = src;
#ifdef DEBUG
			cr_name = "cr3";
#endif
			init_tlb();
			break;
		default: assert(0);
	}

	print_asm("mov %%%s,%%%s", regsl[m.R_M], cr_name);
	return 2;
}

make_helper(mov_cr2r) {
	ModR_M m;
#ifdef DEBUG
	char *cr_name;
#endif
	m.val = instr_fetch(eip + 1, 1);
	uint32_t src;
	switch (m.reg) {
		case 0: src = cpu.CR0.val;
#ifdef DEBUG
				cr_name = "cr0";
#endif
				break;
		case 3: src = cpu.CR3.val;
#ifdef DEBUG
				cr_name = "cr3";
#endif
				break;
		default: assert(0);
	}
	reg_l(m.R_M) = src;

	print_asm("mov %%%s,%%%s", cr_name, regsl[m.R_M]);
	return 2;
}

void desc_add(uint8_t);

make_helper(mov_rm2s_l) {
	int len = 1 + decode_rm2r_w(eip + 1);
	uint8_t sr = op_dest->reg;
	sreg(sr) = (uint16_t)op_src->val;
	desc_add(sr);

	print_asm("movw %s,%%%s", op_src->str, sregs[sr]);
	return len;
}

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
