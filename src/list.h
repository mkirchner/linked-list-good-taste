#ifndef __INTLIST_H__
#define __INTLIST_H__

#include <stddef.h>

struct list_item {
	int value;
	struct list_item *next;
};
typedef struct list_item list_item;

struct list {
	struct list_item *head;
};
typedef struct list list;

size_t size(list *l);
void insert_before(list *l, list_item *before, list_item *item);

void remove_cs101(list *l, list_item *target);
void remove_elegant(list *l, list_item *target);

#endif /* !__INTLIST_H__ */
