#include "cpu/exec/template-start.h"

#define	instr	jmp

make_helper(concat(jmp_i_, SUFFIX)) {
	int len = 1 + concat(decode_si_, SUFFIX)(eip + 1);
	cpu.eip += op_src->val + len;
	if (DATA_BYTE == 2) {
		cpu.eip &= 0xffff;
	}

	cpu.eip -= len;
	print_asm_template1();
	return len;
}

#if	DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_, SUFFIX)) {
	int len = 1 + concat(decode_rm_, SUFFIX)(eip + 1);
	cpu.eip = op_src->val;
	if (DATA_BYTE == 2) {
		cpu.eip &= 0xffff;
	}

	cpu.eip -= len;
	print_asm_template1();
	return len;
}

make_helper(concat(jmp_ptr_, SUFFIX)) {
	uint32_t xip;
	int len = 1;
	if (ops_decoded.is_operand_size_16) {
		xip = instr_fetch(eip + 1, 2);
		cpu.eip = xip;
		len += 2;
	}
	else {
		xip = instr_fetch(eip + 1, 4);
		cpu.eip = xip;
		len += 4;
	}
	uint16_t sltor = instr_fetch(eip + len, 2);
	sreg(R_CS) = sltor;

	cpu.eip -= len;
	print_asm("ljmp $0x%x,$0x%x", sltor, xip);
	return len;
}

#endif

#include "cpu/exec/template-end.h"
