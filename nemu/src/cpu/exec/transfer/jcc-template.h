#include "cpu/exec/template-start.h"

make_helper(concat(ja_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.CF == 0 && cpu.ZF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("ja" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jae_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.CF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jae" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jb_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.CF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jb" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jbe_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.CF == 1 || cpu.ZF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jbe" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(je_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.ZF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("je" str(SUFFIX) " %s", op_src->str);
	return len;
}

#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(jecxz_, SUFFIX)) {
	int len = 1 + decode_i_b(eip + 1);
	int8_t temp = op_src->val;
	int32_t s = temp;
	if (REG(R_ECX) == 0) {
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	if (DATA_BYTE == 4) {
		print_asm("jecxz" str(SUFFIX) " %s", op_src->str);
	}
	else {
		print_asm("jcxz" str(SUFFIX) " %s", op_src->str);
	}
	return len;
}
#endif

make_helper(concat(jg_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.ZF == 0 && cpu.SF == cpu.OF);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jg" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jge_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.SF == cpu.OF);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jge" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jl_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.SF != cpu.OF);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jl" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jle_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.ZF == 1 || cpu.SF != cpu.OF);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jle" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jne_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.ZF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jne" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jno_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.OF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jno" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jnp_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.PF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jnp" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jns_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.SF == 0);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jns" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jo_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.OF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jo" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(jp_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.PF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("jp" str(SUFFIX) " %s", op_src->str);
	return len;
}

make_helper(concat(js_, SUFFIX)) {
	int len = 1 + concat(decode_i_, SUFFIX)(eip + 1);
	bool cond = (cpu.SF == 1);	
	if (cond) {
		DATA_TYPE_S temp = (DATA_TYPE) op_src->val;
		int32_t s = temp;
		cpu.eip += s + len;
		if (DATA_BYTE == 2) {
			cpu.eip &= 0xffff;
		}
		cpu.eip -= len;
	}

	print_asm("js" str(SUFFIX) " %s", op_src->str);
	return len;
}

#include "cpu/exec/template-end.h"
