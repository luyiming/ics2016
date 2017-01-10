#include "nemu.h"

void init_tlb();
hwaddr_t tlb_read(lnaddr_t addr);
uint32_t hwaddr_read(hwaddr_t, size_t);

#define DIR_ENTRY(addr)    ((addr >> 22) & ((1 << 10) - 1))
#define PAGE_ENTRY(addr)   ((addr >> 12) & ((1 << 10) - 1))
#define IN_PAGE_ADDR(addr) (addr & ((1 << 12) - 1))

PTE page_read(lnaddr_t addr) {
	// page directory entry
	PDE dir_entry;
	dir_entry.val = hwaddr_read((cpu.CR3.page_directory_base << 12) + 4 * DIR_ENTRY(addr), 4);

	// page table entry
	PTE page_table_entry;
	page_table_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * PAGE_ENTRY(addr), 4);

	return page_table_entry;
}

hwaddr_t page_translate(lnaddr_t addr) {
    static uint32_t tmp_CR3;
	if(tmp_CR3 != cpu.CR3.val) {
		init_tlb();
		tmp_CR3 = cpu.CR3.val;
	}
	return tlb_read(addr);
}

void debug_page(lnaddr_t addr){
// DEPRICATED:
}

#undef DIR_ENTRY
#undef PAGE_ENTRY
#undef IN_PAGE_ADDR
