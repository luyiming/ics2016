#ifndef __REG_H__
#define __REG_H__

#include "common.h"

/* Do NOT change the order of the constant definitions. */
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

 typedef union {
     struct {
          uint16_t RPL   :2;
          uint16_t TI    :1;
          uint16_t Index:13;
     };
     uint16_t val;
  } SegSel;

typedef struct {
	union{
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
		struct {
			uint32_t eax;
			uint32_t ecx;
			uint32_t edx;
			uint32_t ebx;
			uint32_t esp;
			uint32_t ebp;
			uint32_t esi;
			uint32_t edi;
		};
	};
	swaddr_t eip;
	volatile union {
		struct {
			uint32_t CF: 1;
			uint32_t : 1;
			uint32_t PF: 1;
			uint32_t : 1;
			uint32_t AF: 1;
			uint32_t : 1;
			uint32_t ZF: 1;
			uint32_t SF: 1;
			uint32_t : 1;
			uint32_t IF: 1;
			uint32_t DF: 1;
			uint32_t OF: 1;
			uint32_t : 0;
		};
		uint32_t eflags;
	};
	/* the Control Register 0 */
    union {
        struct {
            uint32_t protect_enable      : 1;
            uint32_t monitor_coprocessor : 1;
            uint32_t emulation           : 1;
            uint32_t task_switched       : 1;
            uint32_t extension_type      : 1;
            uint32_t numeric_error       : 1;
            uint32_t pad0                : 10;
            uint32_t write_protect       : 1;
            uint32_t pad1                : 1;
            uint32_t alignment_mask      : 1;
            uint32_t pad2                : 10;
            uint32_t no_write_through    : 1;
            uint32_t cache_disable       : 1;
            uint32_t paging              : 1;
        };
        uint32_t val; //initialized to zero
    } CR0;
	#define cr0 CR0.val
	/* the Control Register 3 (physical address of page directory) */
    union {
        struct {
            uint32_t pad0                : 3;
            uint32_t page_write_through  : 1;
            uint32_t page_cache_disable  : 1;
            uint32_t pad1                : 7;
            uint32_t page_directory_base : 20;
        };
        uint32_t val;
    } CR3;
	#define cr3 CR3.val;

	union {
		struct {
			unsigned Limit: 16;
	        unsigned Base : 32;
		};
        uint64_t val :48;
    } GDTR;

	union {
		struct {
			unsigned Limit: 16;
	        unsigned Base : 32;
		};
        uint64_t val :48;
    } IDTR;

	struct {
        bool valid;
        uint32_t base;
        uint32_t limit;
        uint32_t DPL : 2;
    } seg_cache[4];

	union{
        SegSel SR[4];
		struct {uint16_t ES, CS, SS, DS;}; // initialized to zero
    };

	volatile bool INTR;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define sreg(index) (cpu.SR[index].val)
#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregs[];

#endif
