#include "cpu/exec/template-start.h"

make_helper(concat(bt_r2rm_, SUFFIX)) {
	int len = 1 + concat(decode_r2rm_, SUFFIX)(eip + 1);
	uint32_t source, bit;
	if (op_dest->type == OP_TYPE_REG) {
		source = op_dest->val;
	}
	else {
		source = swaddr_read(op_dest->addr + op_src->val / (DATA_BYTE * 8), DATA_BYTE, R_DS);
	}
	bit = op_src->val % (DATA_BYTE * 8);
	source >>= (bit / 8) * 8;
	bit %= 8;
	uint8_t mask = 1 << bit;
	uint8_t result = mask & source;
	cpu.CF = !!result;

	print_asm("bt" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
	return len;
}

make_helper(concat(bt_i2rm_, SUFFIX)) {
	int len = 1 + concat(decode_rm_, SUFFIX)(eip + 1);
	uint32_t offset = instr_fetch(eip + len, 1);
	uint32_t source, bit;
	if (op_src->type == OP_TYPE_REG) {
		source = op_src->val;
	}
	else {
		source = swaddr_read(op_src->addr + offset / (DATA_BYTE * 8), DATA_BYTE, R_DS);
	}
	bit = offset % (DATA_BYTE * 8);
	source >>= (bit / 8) * 8;
	bit %= 8;
	uint8_t mask = 1 << bit;
	uint8_t result = mask & source;
	cpu.CF = !!result;

	print_asm("bt" str(SUFFIX) " %d,%s", offset, op_src->str);
	return len + 1;
}

#include "cpu/exec/template-end.h"
