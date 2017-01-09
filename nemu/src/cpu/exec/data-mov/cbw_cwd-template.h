#include "cpu/exec/template-start.h"

make_helper(concat(cbw_cwd_, SUFFIX)) {
	if (ops_decoded.is_operand_size_16) {
		reg_w(0) = (int16_t)(int8_t)reg_b(0);
		print_asm("cbw");
	}
	else {
		reg_l(0) = (int32_t)(int16_t)reg_w(0);
		print_asm("cwd");
	}

	return 1;
}

#include "cpu/exec/template-end.h"
