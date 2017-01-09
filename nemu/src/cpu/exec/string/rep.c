#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		len = exec(eip + 1) - 1;
	}
	else {
		while(cpu.ecx) {
			bool exit = false;
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);

			/* Jump out of the while loop if necessary. */
			switch (ops_decoded.opcode) {
				case 0xa4: case 0xa5: 
				case 0xaa: case 0xab:
					exit = (cpu.ecx == 0);
					break;
				case 0xa6: case 0xa7:
				case 0xae: case 0xaf:
					exit = (cpu.ZF == 0);
					break;
				default:
					break;
			}
			if (exit) {
				break;
			}
		}
		len = 1;
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}

make_helper(repnz) {
	int count = 0;
	while(cpu.ecx) {
		exec(eip + 1);
		count ++;
		cpu.ecx --;
		assert(ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
			  );

		/* Jump out of the while loop if necessary. */
		if (cpu.ZF == 1) {
			break;
		}
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "repnz %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif

	return 1 + 1;
}
