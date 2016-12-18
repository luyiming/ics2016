#include "cpu/exec/template-start.h"
#include "cpu/reg.h"

#define instr pusha

static void do_execute(){
	int i;
	uint32_t temp = cpu.esp;
	for (i = 0; i < 8; i++){
		cpu.esp -= DATA_BYTE;
		if (i == R_ESP){
			swaddr_write(cpu.esp, DATA_BYTE, temp, R_SS);
		}
		else{
			swaddr_write(cpu.esp, DATA_BYTE, reg_l(i), R_SS);
		}
	}
	print_asm("pusha");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
