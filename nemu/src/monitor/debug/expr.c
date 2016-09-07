#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, NUM, REG, VAR, LOR, LAN, EQ, NE, LE, GE, SHL, SHR, NEG, DEREF, NOP

	/* TODO: Add more token types */

};

static bool is_operator(int type) {
    if(type == '(' || type == ')' || type == NUM || type == REG || type == VAR)
        return false;
    else
        return true;
}

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{ " +",	            NOTYPE },   // spaces
	{ "==",             EQ },       // equal
	{ "!=",             NE },       // not equal
	{ "\\$[a-zA-Z]+",   REG },      // register
	{ "[a-zA-Z_]+",     VAR },      // variable
	{ "(0x)?[0-9a-f]+", NUM },      // number
	{ "&&",             LAN },      // logical and
	{ "\\|\\|",         LOR },      // logical or
	{ "<<",             SHL },      // shift left
	{ ">>",             SHR },      // shift right
	{ ">=",             GE },       // greater equal
	{ "<=",             LE },       // less eqaul
	{ ">",              '>'},
	{ "<",              '<'},
	{ "&",              '&'},
	{ "~",              '~'},
	{ "!",              '!'},
	{ "\\^",            '^'},
	{ "\\|",            '|'},
	{ "\\(",            '('},
	{ "\\)",            ')'},
	{ "\\+",            '+'},
	{ "-",              '-'},
	{ "\\*",            '*'},
	{ "/",              '/'},
	{ "%",              '%'},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				 Assert(nr_token < 64, "tokens out of range");

				 switch(rules[i].token_type) {
                     case NUM:
                     {
                         tokens[nr_token].type = NUM;
                         strncpy(tokens[nr_token].str, substr_start, substr_len);
                         tokens[nr_token].str[substr_len] = '\0';
                         // printf("num: %s\n", tokens[nr_token].str);
                         nr_token++;
                         break;
                     }
                     case REG:   //remove prefix $
                     {
                         tokens[nr_token].type = REG;
                         strncpy(tokens[nr_token].str, substr_start + 1, substr_len - 1);
                         tokens[nr_token].str[substr_len - 1] = '\0';
                         // printf("reg name: %s\n", tokens[nr_token].str);
                         nr_token++;
                         break;
                     }
 					case VAR:
                     {
                         tokens[nr_token].type = VAR;
                         strncpy(tokens[nr_token].str, substr_start, substr_len);
                         tokens[nr_token].str[substr_len] = '\0';
                         nr_token++;
                         break;
                     }
                     case NOTYPE: break;
 					default: tokens[nr_token++].type = rules[i].token_type; break;
 				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '(' || is_operator(tokens[i - 1].type)) ) {
			tokens[i].type = DEREF;
		}
	}
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == '(' || is_operator(tokens[i - 1].type)) ) {
			tokens[i].type = NEG;
		}
	}
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '+' && (i == 0 || tokens[i - 1].type == '(' || is_operator(tokens[i - 1].type)) ) {
			tokens[i].type = NOP;
		}
	}

	return true;
}

static int priority(int tk) {
    switch(tk) {
        case NOP: case NEG: case DEREF: case '!': case '~':
            return 11;
        case '*': case '/': case '%':
            return 10;
        case '+': case '-':
            return 9;
        case SHL: case SHR:
            return 8;
        case '>': case '<': case LE: case GE:
            return 7;
        case EQ: case NE: return 6;
        case '&': return 5;
        case '^': return 4;
        case '|': return 3;
        case LAN: return 2;
        case LOR: return 1;
        case NOTYPE: return -1;
        default:  return -1;
    }
}

static int v_st[64], op_st[64]; // value_stack, operator_stack
static int v_top = -1, op_top = -1;   // v_st_top, op_st_top
static bool success = true;

static int calc_once(int op) {
    switch(op) {
        case NOP:
            break;
        case NEG:
            if(v_top < 0) {
                printf("Negation: missing operand\n");
                return 0;
            }
            v_st[v_top] = - v_st[v_top];
            break;
        case DEREF:
            if(v_top < 0) {
                printf("Dereference operation: missing operand\n");
                return 0;
            }
            if(v_st[v_top] < 0) {
                printf("Address out of range: 0x%x\t %d\n", v_st[v_top], v_st[v_top]);
                return 0;
            }
            v_st[v_top] = swaddr_read(v_st[v_top], 4);
            break;
        case '!':
            if(v_top < 0) {
                printf("! operation: missing operand\n");
                return 0;
            }
            v_st[v_top] = ! v_st[v_top];
            break;
        case '~':
            if(v_top < 0) {
                printf("~ operation: missing operand\n");
                return 0;
            }
            v_st[v_top] = ~ v_st[v_top];
            break;
        case '*':
            if(v_top < 1) {
                printf("Multiplication: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] * v_st[v_top];
            v_top--;
            break;
        case '/':
            if(v_top < 1) {
                printf("Division: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] / v_st[v_top];
            v_top--;
            break;
        case '%':
            if(v_top < 1) {
                printf("Modulo operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] % v_st[v_top];
            v_top--;
            break;
        case '+':
            if(v_top < 1) {
                printf("Addition: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] + v_st[v_top];
            v_top--;
            break;
        case '-':
            if(v_top < 1) {
                printf("Subtraction: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] - v_st[v_top];
            v_top--;
            break;
        case SHL:
            if(v_top < 1) {
                printf("<< operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] << v_st[v_top];
            v_top--;
            break;
        case SHR:
            if(v_top < 1) {
                printf(">> operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] >> v_st[v_top];
            v_top--;
            break;
        case '>':
            if(v_top < 1) {
                printf("> operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] > v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case '<':
            if(v_top < 1) {
                printf("< operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] < v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case GE:
            if(v_top < 1) {
                printf(">= operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] >= v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case LE:
            if(v_top < 1) {
                printf("<= operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] <= v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case EQ:
            if(v_top < 1) {
                printf("== operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] == v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case NE:
            if(v_top < 1) {
                printf("!= operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] != v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case '&':
            if(v_top < 1) {
                printf("& operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] & v_st[v_top];
            v_top--;
            break;
        case '^':
            if(v_top < 1) {
                printf("^ operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] ^ v_st[v_top];
            v_top--;
            break;
        case '|':
            if(v_top < 1) {
                printf("| operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = v_st[v_top-1] | v_st[v_top];
            v_top--;
            break;
        case LAN:
            if(v_top < 1) {
                printf("&& operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] && v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        case LOR:
            if(v_top < 1) {
                printf("|| operation: missing operand\n");
                return 0;
            }
            v_st[v_top-1] = (v_st[v_top-1] || v_st[v_top] ? 1 : 0);
            v_top--;
            break;
        default: printf("Unknown operator: %d\n", op); return 0;
    }
    return 1;
}

static int calc() {
    op_st[++op_top] = NOTYPE;
    int i;
    for(i = 0; i < nr_token; ++ i) {
        if(tokens[i].type == NUM) {
            char *s = tokens[i].str;
            int value = *s++ - '0';
            if(value != 0) {  // Dec
                while(*s >= '0' && *s <= '9')
                    value = value * 10 + *s++ - '0';
            }
            else {
                if(*s == 'x' || *s == 'X') { // Hex
                    s++;
                    while((*s >= '0' && *s <= '9') || (*s >= 'a' && *s <= 'f') || (*s >= 'A' && *s <= 'F')) {
                        value = value * 16 + (*s & 0xf) + (*s > '9' ? 9 : 0);
                        s++;
                    }
                }
                else {  // Oct
                    while(*s >= '0' && *s <= '7') {
                        value = value * 8 + *s++ - '0';
                    }
                }
            }
            // printf("get num: %d\n", value);
            v_st[++v_top] = value;
        }
        else if(tokens[i].type == REG) {
            int value = 0;
            int j;
            for(j = R_EAX; j <= R_EDI; j ++) {
                if(strcmp(tokens[i].str, regsl[j]) == 0) {
                    value = reg_l(j);
                }
            }
            for(j = R_AX; j <= R_DI; j ++) {
                if(strcmp(tokens[i].str, regsw[j]) == 0) {
                    value = reg_w(j);
                }
            }
            for(j = R_AL; j <= R_BH; j ++) {
                if(strcmp(tokens[i].str, regsb[j]) == 0) {
                    value = reg_b(j);
                }
            }
            if(strcmp(tokens[i].str, "eip") == 0)
                value = cpu.eip;
            // printf("get register: %s  %d\n", tokens[i].str, value);
            v_st[++v_top] = value;
        }
        else if(tokens[i].type == '(') {
            op_st[++op_top] = '(';
        }
        else if(tokens[i].type == ')') {
            while(op_top > 0 && op_st[op_top] != '(') {
                if(calc_once(op_st[op_top--]) == 0) {
                    success = false;
                    return 0;
                }
            }
            if(op_top == 0) { // miss left (
                printf("Missing left parenthesis\n");
                return 0;
            }
            if(op_top >= 0 && op_st[op_top] == '(') {
                op_top--;
            }
        }
        else {
            if(priority(tokens[i].type) > priority(op_st[op_top])) {
                op_st[++op_top] = tokens[i].type;
            }
            else {
                if(priority(op_st[op_top]) == priority(NEG) && priority(tokens[i].type) == priority(NEG))
                    op_st[++op_top] = tokens[i].type;
                else {
                    while(op_top > 0 && priority(op_st[op_top]) >= priority(tokens[i].type)) {
                        if(calc_once(op_st[op_top--]) == 0) {
                            success = false;
                            return 0;
                        }
                    }
                    op_st[++op_top] = tokens[i].type;
                }
            }
        }
    }
    while(op_top > 0) {
        if(calc_once(op_st[op_top--]) == 0) {
            success = false;
            return 0;
        }
    }
    if(v_top != 0) {
        return 0;
    }
    return v_st[0];
}

int expr(char *e, bool *suc) {
    success = true;
    v_top = -1, op_top = -1;

	if(!make_token(e)) {
		*suc = false;
		return 0;
	}
/*
    if(check_parentheses(0, nr_token - 1) == -1) {
		*success = false;
		return 0;
    }
*/
    int value = calc();
    *suc = success;
    return value;
}
