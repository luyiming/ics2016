#include "nemu.h"

typedef union {
	struct {
		uint32_t offset	:	12;
		uint32_t page	:	10;
		uint32_t dir	:	10;
	};
	uint32_t val;
} line_addr;

uint32_t hwaddr_read(hwaddr_t, size_t);

PTE page_read(lnaddr_t addr){
	line_addr lnaddr;
	lnaddr.val = addr;

	// first level page table - page directory entry
	PDE dir_entry;
	//Log("addr = 0x%x, dir = 0x%x, CR3.val = 0x%x, dir_entry_addr = 0x%x", addr, lnaddr.dir,cpu.CR3.val, (cpu.CR3.page_directory_base << 12) + 4 * lnaddr.dir);
	dir_entry.val = hwaddr_read((cpu.CR3.page_directory_base << 12) + 4 * lnaddr.dir, 4);
	//Assert(dir_entry.present == 1, "dir_entry is not valid!  addr = 0x%x, dir = 0x%x", addr, lnaddr.dir);

	// second level page table - page table entry
	PTE page_table_entry;
	page_table_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	//Assert(page_table_entry.present == 1, "page_table_entry is not valid!  addr = 0x%x, page_frame = 0x%x, page_table_entry = 0x%x",
	//		addr, dir_entry.page_frame, page_table_entry.val);
	return page_table_entry;
}

void init_tlb();
hwaddr_t tlb_read(lnaddr_t addr);
uint32_t this_CR3;
hwaddr_t page_translate(lnaddr_t addr) {
	if(this_CR3 != cpu.CR3.val) {
		init_tlb();
		this_CR3 = cpu.CR3.val;
	}
	return tlb_read(addr);
}

void debug_page(lnaddr_t addr){
	line_addr lnaddr;
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
