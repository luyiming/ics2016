#include "cpu/exec/template-start.h"

make_helper(concat(scas_, SUFFIX)) {
	swaddr_t des = reg_l(R_EDI);
	int inc = (cpu.DF == 0 ? DATA_BYTE : - DATA_BYTE), i, count = 0;
	DATA_TYPE r = REG(R_EAX), d = MEM_R(des, S_ES), temp;
	temp = r - d;
	reg_l(R_EDI) += inc;

	cpu.ZF = (temp == 0);
	cpu.SF = MSB(temp);
	cpu.CF = (r < d);
	cpu.OF = ((MSB(r) != MSB(d)) && (MSB(temp) != MSB(r)));
	cpu.AF = ((r & 0xf) < (d & 0xf));
	temp &= 0xff;
	for (i = 0; i < 8; i++, temp >>= 1) {
		count += (temp & 0x1);
	}
	cpu.PF = (count % 2 == 0);

	print_asm("scas" str(SUFFIX));
	return 1;
}

#include "cpu/exec/template-end.h"
