#include "list.h"

void remove_cs101(IntList *l, IntListItem *target)
{
	IntListItem *cur = l->head, *prev = NULL;
	while (cur != target) {
		prev = cur;
		cur = cur->next;
	}
	if (prev)
		prev->next = cur->next;
	else
		l->head = cur->next;
}

/**
 * Find an item in the list.
 *
 * Finds the item identified by `target` in the list `l` and returns a
 * the address of the `next` pointer of the item before `target`.
 * This is a linear O(n) search.
 *
 * @param l A list of integers
 * @param target The list item to find
 * @return Pointer to the `next` pointer of the item that precedes
 *         `target`, or `head`. Pointer to the last `next` field in the list
 *         if `target` is not in `l`.
 */
static inline IntListItem **find_indirect(IntList *l, IntListItem *target)
{
	IntListItem **p = &l->head;
	while (*p && *p != target)
		p = &(*p)->next;
	return p;
}

void remove_elegant(IntList *l, IntListItem *target)
{
	// undef results if target is not in l
	IntListItem **p = find_indirect(l, target);
	*p = target->next;
}

/**
 * Insert an item into the list.
 *
 * Inserts an item `item` into the list `l`, before the item identified
 * by `before`. Runtime is O(n) where n refers to the distance of before from
 * the list head.
 *
 * @param l A list of integers
 * @param before Pointer to item before which the new item should be inserted.
 *               If `before` points to the list head, the new item will be
 *               inserted at the beginning; if `before` is NULL or invalid,
 *               the item will be appended at the the end of the list
 * @param item The item to insert
 */
void insert_before(IntList *l, IntListItem *before, IntListItem *item)
{
	/* Using indirect pointers really allows us to write
	 * tight code here */
	IntListItem **p = find_indirect(l, before);
	*p = item;
	item->next = before;
}

size_t size(IntList *l)
{
	size_t k = 0;
	IntListItem *cur = l->head;
	while (cur) {
		cur = cur->next;
		k++;
	}
	return k;
}
