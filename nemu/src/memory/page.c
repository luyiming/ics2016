#include "nemu.h"

uint32_t hwaddr_read(hwaddr_t, size_t);

typedef union {
	struct {
		uint32_t offset     :12;
		uint32_t page_index :10;
		uint32_t dir_index  :10;
	};
	uint32_t val;
} linear_addr;

hwaddr_t page_translate(lnaddr_t addr) {
	linear_addr lnaddr;
	lnaddr.val = addr;

	PDE page_dir_entry;
	page_dir_entry.val = hwaddr_read((cpu.CR3.page_directory_base << 12) + 4 * lnaddr.dir_index, 4);
	Assert(page_dir_entry.present == 1, "page_dir_entry is not valid!  addr = 0x%x, dir_entry_addr = 0x%x", addr, page_dir_entry.val);

	PTE page_table_entry;
	page_table_entry.val = hwaddr_read((page_dir_entry.page_frame << 12) + 4 * lnaddr.page_index, 4);
	Assert(page_table_entry.present == 1, "page_table_entry is not valid!  addr = 0x%x, page_table_entry = 0x%x", addr, page_table_entry.val);

	return (page_table_entry.page_frame << 12) + lnaddr.offset;
}
