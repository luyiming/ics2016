#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

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

    if(strcmp(arg, "r") == 0) {
        int i;
        for(i = R_EAX; i <= R_EDI; i++) {
            printf("%s    0x%08x\n", regsl[i], reg_l(i));
        }
        printf("eip    0x%08x\n", cpu.eip);
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
        printf("%8x :\t0x%08x\t", t_addr + 4 * i, swaddr_read(t_addr + 4 * i, 4));
        int j;
        for(j = 0; j < 4; j ++) {
            printf("%02x ", swaddr_read(t_addr + 4 * i + j, 1));
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
