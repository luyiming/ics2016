#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32


static WP wp_pool[NR_WP];
static WP *head, *free_;

static void check_wp() {
    int i = 1;
    WP *p;
    for(p = head; p != NULL; p = p->next) {
        p->NO = i++;
    }
}

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i + 1;
		wp_pool[i].next = &wp_pool[i + 1];
        wp_pool[i].hit_times = 0;
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
    check_wp();
}

WP* new_wp() {
    if(free_ == NULL)
        return NULL;
    WP *p = free_;
    free_ = free_->next;
    p->next = NULL;

    if(head == NULL)
        head = p;
    else {
        WP *p2 = head;
        while(p2->next != NULL)
            p2 = p2->next;
        p2->next = p;
    }
    check_wp();
    return p;
}

bool free_wp(int number) {
    WP *p1, *p2;
    for(p1 = NULL, p2 = head; p2 != NULL; p1 = p2, p2 = p2->next) {
        if(p2->NO == number)
            break;
    }
    if(p2 == NULL)
        return false;
    if(p2->NO == number) {
        if(p2 == head)
            head = head->next;
        else
            p1->next = p2->next;
    }
    p2->hit_times = 0;
    p2->next = free_;
    free_ = p2;
    check_wp();
    return true;
}

void print_wp() {
    WP *p;
    if(head == NULL) {
        printf("no watchpoints\n");
        return;
    }
    printf("Num\t\tExpression\n");
    for(p = head; p != NULL; p = p->next) {
        printf("%d\t\t%s\n", p->NO, p->str);
    }
}

bool hit_wp()
{
    bool is_hit = false;
    WP *p;
    for(p = head; p != NULL; p = p->next) {
        bool success = false;
        int temp = expr(p->str, &success);
        if(temp != p->value)
        {
            printf("Hit watchpoint #%d\texpr = %s, value = %d/0x%x --> %d/0x%x\n", p->NO, p->str, p->value, p->value, temp, temp);
            p->value = temp;
            p->hit_times++;
            is_hit = true;
        }
    }
    return is_hit;
}
