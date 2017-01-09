#include "cpu/exec/template-start.h"

make_helper(concat(lods_, SUFFIX)) {
	swaddr_t src = reg_l(R_ESI);
	int inc = (cpu.DF == 0 ? DATA_BYTE : - DATA_BYTE);
	REG(R_EAX) = MEM_R(src, S_DS);
	reg_l(R_ESI) += inc;

	print_asm("lods" str(SUFFIX));
	return 1;
}

#include "cpu/exec/template-end.h"
