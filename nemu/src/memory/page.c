#include "nemu.h"

void init_tlb();
hwaddr_t tlb_read(lnaddr_t addr);

typedef union {
	struct {
		uint32_t offset	:	12;
		uint32_t page	:	10;
		uint32_t dir	:	10;
	};
	uint32_t val;
} addr_page_t;

uint32_t hwaddr_read(hwaddr_t, size_t);

PTE page_read(lnaddr_t addr){
	addr_page_t lnaddr;
	lnaddr.val = addr;

	// page directory entry
	PDE dir_entry;
	dir_entry.val = hwaddr_read((cpu.CR3.page_directory_base << 12) + 4 * lnaddr.dir, 4);

	// page table entry
	PTE page_table_entry;
	page_table_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);

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
	addr_page_t lnaddr;
	lnaddr.val = addr;
	printf("lnaddr = 0x%x, dir = 0x%x, page = 0x%x, offset = 0x%x\n", lnaddr.val, lnaddr.dir, lnaddr.page, lnaddr.offset);
	printf("CR3.page_directory_base = 0x%x, dir_entry_addr = 0x%x\n", cpu.CR3.page_directory_base, (cpu.CR3.page_directory_base << 12) + 4 * lnaddr.dir);
	PDE dir_entry;
	dir_entry.val = hwaddr_read((cpu.CR3.page_directory_base << 12) + lnaddr.dir * 4, 4);
	printf("page directory: present = %x, page_frame << 12 = 0x%x\n", dir_entry.present, dir_entry.page_frame<<12);

	PTE page_table_entry;
	page_table_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	printf("page table item: present = %x, page_frame << 12 = 0x%x\n", page_table_entry.present, page_table_entry.page_frame<<12);

	printf("--->hwaddr = 0x%x\n",  (page_table_entry.page_frame << 12) + lnaddr.offset);
}
