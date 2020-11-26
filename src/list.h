#ifndef __INTLIST_H__
#define __INTLIST_H__

#include <stddef.h>

struct IntListItem {
    int value;
    struct IntListItem *next;
};
typedef struct IntListItem IntListItem;

struct IntList {
    IntListItem *head;
};
typedef struct IntList IntList;

size_t size(IntList *l);
void insert_before(IntList *l, IntListItem *before, IntListItem *item);

void remove_cs101(IntList *l, IntListItem *target);
void remove_elegant(IntList *l, IntListItem *target);

#endif /* !__INTLIST_H__ */
