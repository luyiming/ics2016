#include "cpu/exec/template-start.h"

make_helper(concat(stos_, SUFFIX)) {
	swaddr_t des = reg_l(R_EDI);
	int inc = (cpu.DF == 0 ? DATA_BYTE : - DATA_BYTE);
	MEM_W(des, S_ES, REG(R_EAX));
	reg_l(R_EDI) += inc;

	print_asm("stos" str(SUFFIX));
	return 1;
}

#include "cpu/exec/template-end.h"
