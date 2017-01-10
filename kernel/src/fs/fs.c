#include "common.h"
#include <sys/ioctl.h>
#include "irq.h"
#include <string.h>

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
/*static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"f.mkf", 186966, 5195401},
	{"fbp.mkf", 1128064, 3232609},	{"fire.mkf", 834728, 4360673},
	{"gop.mkf", 11530322, 5382367}, {"m.msg", 188232, 19986709},
	{"map.mkf", 1496578, 16912689}, {"mgo.mkf", 1577442, 18409267},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};*/
/* my sort */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"f.mkf", 186966, 3232609},
	{"fbp.mkf", 1128064, 3419575},	{"fire.mkf", 834728, 4547639},
	{"gop.mkf", 11530322, 5382367}, {"m.msg", 188232, 16912689},
	{"map.mkf", 1496578, 17100921}, {"mgo.mkf", 1577442, 18597499},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_ioctl(int fd, uint32_t request, void *p) {
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

/* TODO: implement a simplified file system here. */

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);
void serial_printc(char);

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

Fstate file_state[NR_FILES + 3];

void init_fs() {
	memset((void*)file_state, 0, sizeof(file_state));
	int i = 0;
	for (i = 0; i < 3; i ++) {
		file_state[i].opened = true;
	}
}

void sys_write(TrapFrame *tf) {
	// ebx:file-descriptor, ecx:str, edx:len
	if (tf->ebx == 1 || tf->ebx == 2) {
#ifdef HAS_DEVICE
		int i;
		for(i = 0; i < tf->edx; i++)
			serial_printc(*(char *)(tf->ecx + i));
#else
		asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
#endif
		tf->eax = tf->edx;
	}
	else if (tf->ebx > 2 && tf->ebx < NR_FILES + 3) {
#ifdef DEBUG
		assert(file_state[tf->ebx].opened);
		assert(file_state[tf->ebx].offset + tf->edx <= file_table[tf->ebx - 3].size);
#endif
		ide_write((void*)tf->ecx,
				file_table[tf->ebx - 3].disk_offset + file_state[tf->ebx].offset,
				tf->edx);
		file_state[tf->ebx].offset += tf->edx;
		tf->eax = tf->edx;
	}
	else {
		tf->eax = -1;
	}
}

void sys_open(TrapFrame *tf) {
	char *pathname = (char*)tf->ebx;
	int idx;
	for (idx = 0; idx < NR_FILES; idx ++) {
		if (strcmp(pathname, file_table[idx].name) == 0) {
			break;
		}
	}
#ifdef DEBUG
	Log("%s", pathname);
	assert(idx < NR_FILES);
#endif
	file_state[idx + 3].opened = true;
	file_state[idx + 3].offset = 0;
	// return value
	tf->eax = idx + 3;
}

void sys_read(TrapFrame *tf) {
	if (tf->ebx < 3 || tf->ebx >= NR_FILES + 3) {
		tf->eax = -1;
		return;
	}
	assert(file_state[tf->ebx].opened);
	int len = tf->edx;
	if (file_state[tf->ebx].offset + tf->edx > file_table[tf->ebx - 3].size) {
		len = file_table[tf->ebx - 3].size - file_state[tf->ebx].offset;
	}
	ide_read((void*)tf->ecx,
			file_table[tf->ebx - 3].disk_offset + file_state[tf->ebx].offset,
			len);
	file_state[tf->ebx].offset += len;
	// return value
	tf->eax = len;
}

void sys_lseek(TrapFrame *tf) {
	if (tf->ebx > 2 && tf->ebx < NR_FILES + 3) {
#ifdef DEBUG
		assert(file_state[tf->ebx].opened);
#endif
		switch (tf->edx) {
			case SEEK_SET:
				file_state[tf->ebx].offset = tf->ecx;
				break;
			case SEEK_CUR:
				file_state[tf->ebx].offset += tf->ecx;
				break;
			case SEEK_END:
				file_state[tf->ebx].offset = file_table[tf->ebx - 3].size - tf->ecx;
				break;
			default: assert(0);
		}
#ifdef DEBUG
		assert(file_state[tf->ebx].offset >= 0 &&
				file_state[tf->ebx].offset <= file_table[tf->ebx - 3].size);
#endif
		tf->eax = file_state[tf->ebx].offset;
	}
	else {
		tf->eax = -1;
	}
}

void sys_close(TrapFrame * tf) {
	if (tf->ebx > 2 && tf->ebx < NR_FILES + 3) {
#ifdef DEBUG
		assert(file_state[tf->ebx].opened);
#endif
		file_state[tf->ebx].opened = false;
		tf->eax = 0;
	}
	else {
		tf->eax = -1;
	}
}
