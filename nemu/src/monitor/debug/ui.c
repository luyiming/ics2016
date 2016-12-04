#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
extern char* get_symbol_name(uint32_t addr);
void debug_cache(hwaddr_t addr);
void debug_page(lnaddr_t addr);
extern uint64_t cpu_time;

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;
    char *temp_line = NULL;

	temp_line = readline("(nemu) ");

    int i, valid = 0, len = strlen(temp_line);
    for(i = 0; i < len; i++) {
        if(!isspace(temp_line[i])) {
            valid = 1;
            break;
        }
    }

    if(valid) {
        if(line_read) {
            free(line_read);
        }
        line_read = temp_line;
        add_history(line_read);
    }
    else {
        if(temp_line) {
            free(temp_line);
        }
    }

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static int cmd_bt(char *args);
static int cmd_addr(char *args);
static int cmd_page(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Step one instruction exactly", cmd_si },
	{ "info", "Show information", cmd_info },
	{ "x", "Examine memory", cmd_x },
	{ "p", "Print expression", cmd_p },
	{ "w", "Set watchpoint", cmd_w },
	{ "d", "Delete watchpoint", cmd_d },
	{ "bt", "Print stack chain", cmd_bt },
	{ "addr", "Print cache addr info.", cmd_addr },
	{ "page", "Print page translation info.", cmd_page },

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args) {
    char *arg = strtok(NULL, " ");
    int n = 1;
    if(arg != NULL) {
        n = atoi(arg);
    }
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args) {
    char *arg = strtok(NULL, " ");

	if(arg == NULL) {
		printf("Usage: info r/w\n");
	}
    else if(strcmp(arg, "r") == 0) {
        int i;
        for(i = R_EAX; i <= R_EDI; i++) {
            printf("%s    0x%08x\n", regsl[i], reg_l(i));
        }
        printf("eip    0x%08x\n", cpu.eip);
    }
	else if(strcmp(arg, "w") == 0) {
        print_wp();
    }
    else {
        printf("Usage: info r/w\n");
    }
    return 0;
}

static int cmd_x(char *args) {
    char *arg = strtok(NULL, " ");
    int i, n;
    n = atoi(arg);
    if(n == 0)
        n = 1;
    else
        arg = strtok(NULL, " ");
    bool success = false;
    uint32_t t_addr = expr(arg, &success);
    for(i = 0; i < n; i ++)
    {
        printf("%8x :\t0x%08x\t", t_addr + 4 * i, swaddr_read(t_addr + 4 * i, 4, R_DS));
        int j;
        for(j = 0; j < 4; j ++) {
            printf("%02x ", swaddr_read(t_addr + 4 * i + j, 1, R_DS));
        }
        printf("\n");
    }
    return 0;
}

static int cmd_p(char *args) {
    if(args) {
        bool success = false;
        int value = expr(args, &success);
        if(success) {
            printf("%d\t\t0x%x\n", value, value);
        }
        else
            printf("Bad expression\n");
    }
    return 0;
}

static int cmd_w(char *args) {
    WP *wp = new_wp();
    if(wp == NULL) {
        printf("no more watchpoint\n");
    }
    else {
        bool success = false;
        wp->value = expr(args, &success);
        if(!success) {
            printf("bad expression\n");
            free_wp(wp->NO);
        }
        else {
            strcpy(wp->str, args);
            printf("watchpoint #%d\tvalue:%d/0x%x\t\texpr:%s\n", wp->NO, wp->value, wp->value, wp->str);
        }
    }
    return 0;
}

static int cmd_d(char *args) {
    char *arg = strtok(NULL, " ");
    int n = atoi(arg);
    if(n == 0) {
        printf("syntax error. Usage d [N]\n");
    }
    else {
        if(free_wp(n))
            printf("delete watchpoint #%d\n", n);
        else
            printf("cannot find watchpoint #%d\n", n);
    }
    return 0;
}

static int cmd_bt(char *args) {
	uint32_t cur_ebp = cpu.ebp;
	swaddr_t ret_addr = 0;
	uint32_t func_args[4] = {0};
	char* func_name;
	int cnt = 0;
	do {
		int i;
		for(i = 0; i < 4; i ++) {
			func_args[i] = swaddr_read(cur_ebp + 8 + 4 * i, 4, R_SS);
		}
		if(cnt == 0)
			func_name = get_symbol_name(cpu.eip);
		else
			func_name = get_symbol_name(ret_addr);
		if(cnt == 0)
			printf("#%d  %s (%x, %x, %x, %x)\n", cnt, func_name, func_args[0], func_args[1], func_args[2], func_args[3]);
		else
			printf("#%d  0x%x in %s (%x, %x, %x, %x)\n", cnt, ret_addr, func_name, func_args[0], func_args[1], func_args[2], func_args[3]);
		ret_addr = swaddr_read(cur_ebp + 4, 4, R_SS);
		cur_ebp = swaddr_read(cur_ebp, 4, R_SS);
		cnt++;
	} while(cur_ebp != 0);
    return 0;
}

int nemu_atoi(char *str) {
	int value = 0, sign = 1, radix = 10;
	if(str == NULL) return 0;
	if(*str == '-') {
		sign = -1;
		str++;
	}
	if(*str == '0' && *(str + 1) == 'x') {
		radix = 16;
		str += 2;
	}
	else if(*str == '0') {
		radix = 8;
		str++;
	}
	else
		radix = 10;
	while(*str) {
		if(radix == 16) {
			if(*str >= '0' && *str <= '9')
				value = value * radix + *str - '0';
			else
				value = value * radix + (*str | 0x20) - 'a' + 10;
		}
		else
			value = value * radix + *str - '0';
		str++;
	}
	return sign * value;
}

static int cmd_addr(char *args) {
    char *arg = strtok(NULL, " ");
    int n = nemu_atoi(arg);
    if(n == 0) {
		printf("cpu_time with cache:\t%lld\n", cpu_time);
        //printf("syntax error. Usage addr n\n");
    }
    else {
        debug_cache(n);
    }
    return 0;
}

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg);
static int cmd_page(char *args){
    if(args == NULL){
        printf("Please input a number!\n");
        return 0;
    }
    bool success = false;
    int addr = expr(args, &success);
    if(success) {
		lnaddr_t lnaddr = seg_translate(addr, R_SS);
	    printf("R_SS lnaddr = 0x%x\n", lnaddr);
	    debug_page(lnaddr);
		lnaddr = seg_translate(addr, R_DS);
	    printf("R_DS lnaddr = 0x%x\n", lnaddr);
	    debug_page(lnaddr);
    }
    else
        printf("Bad expression\n");
    return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
