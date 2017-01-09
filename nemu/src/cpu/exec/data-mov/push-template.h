#include "cpu/exec/template-start.h"

#define	instr	push

static void do_execute() {
	int dec = (DATA_BYTE == 2 ? 2 : 4);
	swaddr_t des;
	uint32_t src = (int32_t)(DATA_TYPE_S) op_src->val;
	if (DATA_BYTE == 2) {
		reg_w(R_SP) -= dec;
	}
	else {
		reg_l(R_SP) -= dec;
	}
	des = (DATA_BYTE == 2 ? reg_w(R_SP) : reg_l(R_ESP));
	swaddr_write(des, dec, S_SS, src); 
	
	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 4 || DATA_BYTE == 2
make_instr_helper(r)
make_instr_helper(rm)

make_helper(concat(pusha_, SUFFIX)) {
	DATA_TYPE temp = REG(R_ESP);
	int i;
	for (i = 0; i < 8; i ++) {
		REG(R_ESP) -= DATA_BYTE;
		if (R_EAX + i == R_ESP) {
			MEM_W(REG(R_ESP), S_SS, temp);
		}
		else {
			MEM_W(REG(R_ESP), S_SS, REG(R_EAX + i));
		}
	}
	
	print_asm("pusha");
	return 1;	
}
#endif

#include "cpu/exec/template-end.h"
