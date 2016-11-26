#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

 typedef struct {
     uint16_t RPL   :2;
     uint16_t TI    :1;
     uint16_t Index:13;
 } SegSel;

typedef struct {
    union {
        union {
            uint32_t _32;
            uint16_t _16;
            uint8_t _8[2];
        } gpr[8];

        /* Do NOT change the order of the GPRs' definitions. */
        struct {
            uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        };
    };

    union {
        struct {        //    init      description
            unsigned  CF: 1; // 0 * S   Carry Flag
            unsigned    : 1; // 1
            unsigned  PF: 1; // 0 * S   Parity Flag
            unsigned    : 1; // 0
            unsigned  AF: 1; // 0   S   Auxiliary Carry Flag
            unsigned    : 1; // 0
            unsigned  ZF: 1; // 0 * S   Zero Flag
            unsigned  SF: 1; // 0 * S   Sign Flag
            unsigned  TF: 1; // 0   X   Trap Flag
            unsigned  IF: 1; // 0   X   Interrupt Enable Flag
            unsigned  DF: 1; // 0   C   Direction Flag
            unsigned  OF: 1; // 0 * S   Overflow Flag
            unsigned IOPL:2; // 0   X   I/O Privilege Level
            unsigned  NT: 1; // 0   X   Nested Task
            unsigned    : 1; // 0
            unsigned  RF: 1; // 0   X   Resume Flag
            unsigned  VM: 1; // 0   X   Virtual-8086 Mode
            unsigned  AC: 1; // 0   X   Alignment Check
            unsigned VIF: 1; // 0   X   Virtual Interrupt Flag
            unsigned VIP: 1; // 0   X   Virtual Interrupt Pending
            unsigned  ID: 1; // 0   X   Identification Flag
            unsigned    :10; // 0
        };
        uint32_t eflags;
    };


    union{
        SegSel SR[4];
        struct {uint16_t ES, CS, SS, DS;}; // initialized to zero
    };

    struct {
        unsigned Limit: 16;
        unsigned Base : 32;
    } GDTR;

    struct {
        unsigned Limit: 16;
        unsigned Base : 32;
    } IDTR;

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

    /* CR2 is used for handling page faults when PG is set. The processor stores
     * in CR2 the linear address that triggers the fault.
     */

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

	swaddr_t eip;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
