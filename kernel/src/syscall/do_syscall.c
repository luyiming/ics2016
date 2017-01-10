#include "irq.h"

#include <sys/syscall.h>

void     add_irq_handle(int, void (*)(void));
uint32_t mm_brk(uint32_t);
int      fs_ioctl(int, uint32_t, void *);
void     serial_printc(char);

int fs_open (const char *pathname, int flags);
int fs_read (int fd, void *buf,  int len);
int fs_write(int fd, void *buf,  int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

static int sys_open(TrapFrame *tf) {
	return fs_open((char *)tf->ebx, tf->ecx);
}

static int sys_read(TrapFrame *tf) {
	return fs_read(tf->ebx, (void *)tf->ecx, tf->edx);
}

static int sys_write(TrapFrame *tf) {
	// ebx:file-descriptor, ecx:str, edx:len
	if (tf->ebx == 1 || tf->ebx == 2) {
#ifdef HAS_DEVICE
		int i;
		for(i = 0; i < tf->edx; i++)
			serial_printc(*(char *)(tf->ecx + i));
#else
		asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
#endif
		return tf->edx;
	}
	else {
		return fs_write(tf->ebx, (void *)tf->ecx, tf->edx);
	}
}

static int sys_lseek(TrapFrame *tf) {
	return fs_lseek(tf->ebx, tf->ecx, tf->edx);
}

static int sys_close(TrapFrame *tf) {
	return fs_close(tf->ebx);
}

static int sys_brk(TrapFrame *tf) {
	return mm_brk(tf->ebx);
}

static int sys_ioctl(TrapFrame *tf) {
	return fs_ioctl(tf->ebx, tf->ecx, (void *)tf->edx);
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The `add_irq_handle' system call is artificial. We use it to
		 * let user program register its interrupt handlers. But this is
		 * very dangerous in a real operating system. Therefore such a
		 * system call never exists in GNU/Linux.
		 */
		case 0:
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk:   tf->eax = sys_brk(tf);   break;
		case SYS_ioctl: tf->eax = sys_ioctl(tf); break;
		case SYS_write: tf->eax = sys_write(tf); break;
		case SYS_lseek: tf->eax = sys_lseek(tf); break;
		case SYS_read:  tf->eax = sys_read(tf);  break;
		case SYS_open:  tf->eax = sys_open(tf);  break;
		case SYS_close: tf->eax = sys_close(tf); break;

		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d, eip = 0x%08x", tf->eax, tf->eip);
	}
}
