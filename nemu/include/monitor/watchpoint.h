#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
    char str[256];
    int value;
    int hit_times;
	struct watchpoint *next;
} WP;

extern WP* new_wp();
extern bool free_wp(int number);
extern void print_wp();
extern bool hit_wp();

#endif
