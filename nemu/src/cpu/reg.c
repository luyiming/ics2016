#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *sregs[] = {"es", "cs", "ss", "ds" };

void p_info_r() {
	int i;
	for (i = 0; i < 8; i++) {
		printf("$%-6s%-12u0x%.8x\n", regsl[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
	}
	for (i = 0; i < 4; i++) {
		printf("$%-6s%-12u0x%.8x\n", sregs[i], cpu.SR[i].val, cpu.SR[i].val);
	}
	printf("$%-6s%-12u0x%.8x\n", "eip", cpu.eip, cpu.eip);
	printf("%-7s[", "flags");
	cpu.CF ? printf(" CF") : (void)0;
	cpu.PF ? printf(" PF") : (void)0;
	cpu.AF ? printf(" AF") : (void)0;
	cpu.ZF ? printf(" ZF") : (void)0;
	cpu.SF ? printf(" SF") : (void)0;
	cpu.IF ? printf(" IF") : (void)0;
	cpu.DF ? printf(" DF") : (void)0;
	cpu.OF ? printf(" OF") : (void)0;
	printf(" ]\n");
	return;
}

uint32_t reg_val(char *reg, bool *success) {
	/* reg should be lower chars */
	int i;
	*success = true;	
	for (i = 0; i < 8; i++) {
		if (strcmp(regsl[i], reg) == 0) {
			return cpu.gpr[i]._32;
		}
		else if (strcmp(regsw[i], reg) == 0) {
			return cpu.gpr[i]._16;
		}
		else if (strcmp(regsb[i], reg) == 0) {
			return cpu.gpr[i % 4]._8[i / 4];
		}
	}
	if (strcmp("eip", reg) == 0) {
		return cpu.eip;
	}
	*success = false;
	return -1;
}

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

