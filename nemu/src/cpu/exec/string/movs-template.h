#include "cpu/exec/template-start.h"

make_helper(concat(movs_, SUFFIX)) {
	swaddr_t src = reg_l(R_ESI);
	swaddr_t des = reg_l(R_EDI);
	int inc;
	MEM_W(des, S_ES, MEM_R(src, S_DS));
	inc = (cpu.DF == 0 ? DATA_BYTE : - DATA_BYTE);
	reg_l(R_ESI) += inc;
	reg_l(R_EDI) += inc;

	print_asm("movs" str(SUFFIX));
	return 1;
}


#include "cpu/exec/template-end.h"
