# Linked lists, pointer tricks and good taste

* [Introduction](#introduction)
* [The code](#the-code)
   * [The CS101 version](#the-cs101-version)
   * [A more elegant solution](#a-more-elegant-solution)
* [How does it work?](#how-does-it-work)
   * [Integrating the head pointer](#integrating-the-head-pointer)
   * [Maintaining a handle](#maintaining-a-handle)
* [Going beyond](#going-beyond)
   * [Inserting before existing items](#inserting-before-existing-items)
   * [Quick refactor](#quick-refactor)
   * [Implementing insert_before()](#implementing-insert_before)
* [Conclusion](#conclusion)


## Introduction

In a 2016 [TED interview][ted] (14:10) Linus Torvalds speaks about what he
considers *good taste* in coding. As an example, he presents two
implementations of item removal in singly linked lists (reproduced below).  In
order to remove the first item from a list, one of the implementations requires
a special case, the other one does not.  Linus, obviously, prefers the latter.

His comment is:

> [...] I don't want you to understand why it doesn't have the if statement.
> But I want you to understand that sometimes you can see a problem in a
> different way and rewrite it so that a special case goes away and becomes the
> normal case, and that's *good code*. [...] -- L. Torvalds

The code snippets he presents are C-style pseudocode and are simple enough to
follow. However, as Linus mentions in the comment, the snippets lack a
conceptual explanation and it is not immediately evident how the more elegant
solution actually works.

The next two sections look at the technical approach in detail and demonstrate
how and why the indirect addressing approach is so neat. The last section
extends the solution from item deletion to insertion.


## The code

The basic data structure for a singly linked list of integers is shown in
Figure 1.

<p align="center">
<img alt="linked list" src="img/linked-list.png" width="600">
<br />
<b>Figure 1</b>: Singly linked list of integers.
</p>

Numbers are arbitrarily chosen integer values and arrows indicate pointers.
`head` is a pointer of type `IntListItem*` and each of the boxes
is an instance of an `IntListItem` struct, each with a member variable (called
`next` in the code) of type `IntListItem*` that points to the next item.

The C implementation of the data structure is:

```c
struct IntListItem {
    int value;
    struct IntListItem* next;
};
typedef struct IntListItem IntListItem;

struct IntList {
    IntListItem* head;
};
typedef struct IntList IntList;

```
We also include a (minimal) API:

```c
/* The textbook version */
void remove_cs101(IntList* l, IntListItem* target);
/* A more elegant solution */
void remove_elegant(IntList* l, IntListItem* target);
```

With that in place, let's have a look at the implementations of
`remove_cs101()` and `remove_elegant()`. The code of these examples is true
to the pseudocode from Linus' example and also compiles and runs.

### The CS101 version

<p align="center">
<img alt="simple data model" src="img/data-model-cs101.png" width="600">
<br />
<b>Figure 2</b>: The conceptual model for the list data structure in the CS101 algorithm.
</p>

```c
void remove_cs101(IntList *l, IntListItem *target)
{
    IntListItem *cur = l->head, *prev = NULL;
    while (cur != target) {
        prev = cur;
        cur = cur->next;
    }
    if (prev) {
        prev->next = cur->next;
    } else {
        l->head = cur->next;
    }
}
```

The standard CS101 approach makes use of two traversal pointers `cur` and
`prev`, marking the current and previous traversal position in the list,
respectively.  `cur` starts at the list head `head`, and advances until the
target is found.  `prev` starts at `NULL` and is subsequently updated with the
previous value of `cur` every time `cur` advances. After the target is found,
the algorithm tests if `prev` is non-`NULL`. If yes, the item is not at the
beginning of the list and the removal consists of re-routing the linked list
around `cur`. If `prev` is `NULL`, `cur` is pointing to the first element in
the list, in which case, removal means moving the list head forward.

### A more elegant solution

The more elegant version has less code and does not require a separate branch
to deal with deletion of the first element in a list.

```c
void remove_elegant(IntList *l, IntListItem *target)
{
    IntListItem **p = &l->head;
    while ((*p) != target) {
        p = &(*p)->next;
    }
    *p = target->next;
}
```

The code uses an indirect pointer `p` that holds the address of a pointer to a
list item, starting with the address of `head`.  In every iteration, that
pointer is advanced to hold the address of the pointer to the next list item,
i.e. the address of the `next` element in the current `IntListItem`.
When the pointer to the list item `(*p)` equals `target`, we exit the search
loop and remove the item from the list.


## How does it work?

The key insight is that using an indirect pointer `p` has two conceptual
benefits:

1. It allows us to interpret the linked list in a way that makes the `head`
   pointer an integral part the data structure. This eliminates the need 
   for a special case to remove the first item.
2. It also allows us to evaluate the condition of the `while` loop without
   having to let go of the pointer that points to `target`. This allows us to
   modify the pointer that points to `target` and to get away with a single
   iterator as opposed to `prev` and `cur`.

Let's look each of these points in turn.

### Integrating the `head` pointer

The standard model interprets the linked list as a sequence of `IntListItem`
instances. The beginning of the sequence can be accessed through a `head`
pointer. This leads to the conceptual model illustrated in Figure 2 above. The `head` pointer is
merely considered as a handle to access the start of the list. `prev` and `cur`
are pointers of type `IntListItem*` and always point to an item or `NULL`.

The elegant implementation uses indirect addressing scheme that yields a different
view on the data structure:

<p align="center">
<img alt="Data model for indirect addressing" src="img/data-model-indirect.png" width="600">
<br />
<b>Figure 3</b>: The conceptual model for the list data structure in the more
elegant approach.
</p>

Here, `p` is of type `IntListItem**` and holds the address of the pointer to
the current list item. When we advance the pointer, we forward to the address
of the pointer to the next list item.

In code, this translates to `p = &(*p)->next`, meaning we

1. `(*p)`: dereference the address to the pointer to the current list item
2. `->next`: dereference that pointer again and select the field that holds
   the address of the next list item
3. `&`: take the address of that address field (i.e. get a pointer to it)

This corresponds to an interpretation of the data structure where the list is a
a sequence of pointers to `IntListItem`s (cf. Figure 3).

### Maintaining a handle

An additional benefit of that particular interpretation is that it supports
editing the `next` pointer of the predecessor of the current item throughout the
entire traversal.

With `p` holding the address of a pointer to a list item, the comparison in the
search loop becomes

```c
while ((*p) != target) {
    ...
}
```

The search loop will exit if `(*p)` equals `target`, and once it does, we are
still able to modify `(*p)` since we hold its address `p`. Thus, despite
iterating the loop until we hit `target`, we still maintain a handle (the
address of the `next` field or the `head` pointer) that can be used to directly
modify the pointer that points *to* the item.

This is the reason why we can modify the incoming pointer to an item to point
to a different location using `*p = target->next` and why we do not need `prev`
and `cur` pointers to traverse the list for item removal.

## Going beyond

It turns out that the idea behind `remove_elegant()` can be applied to yield a
particularly concise implementation of another function in the list API:
`insert_before()`, i.e. inserting a given item before another one.

### Inserting before existing items

First, let's add the following declaration to the list API in `list.h`:

```c
void insert_before(IntList *l, IntListItem *before, IntListItem *item);
```

The function will take a pointer to a list `l`, a pointer `before` to an 
item in that list and a pointer to a new list item `item` that the function
will insert before `before`.

### Quick refactor

Before we move on, we refactor the search loop into a separate
function

```c

static inline IntListItem **find_indirect(IntList *l, IntListItem *target)
{
    IntListItem **p = &l->head;
    while ((*p) && (*p) != target) {
        p = &(*p)->next;
    }
    return p;
}

```

and use that function in `remove_elegant()` like so

```c
void remove_elegant(IntList *l, IntListItem *target)
{
    IntListItem **p = find_indirect(l, target);
    *p = target->next;
}
```

### Implementing `insert_before()`

Using `find_indirect()`, it is straightforward to implement `insert_before()`:

```c
void insert_before(IntList *l, IntListItem *before, IntListItem *item)
{
    IntListItem **p = find_indirect(l, before);
    *p = item;
    item->next = before;
}
```

A particularly beautiful outcome is that the implementation has consistent
semantics for the edge cases: if `before` points to the list head, the new item
will be inserted at the beginning of the list, if `before` is `NULL` or invalid
(i.e. the item does not exist in `l`), the new item will be appended at the
end.


## Conclusion

The premise of the more elegant solution for item deletion is a single, simple
change: using an indirect `IntListItem**` pointer to iterate over the pointers
to the list items.  Everything else flows from there: there is no need for a
special case or branching and a single iterator is sufficient to find and
remove the target item.
It also turns out that the same approach provides an elegant solution for item
insertion in general and for insertion *before* an existing item in particular.

So, going back to Linus' initial comment: is it good taste? Hard to say, but
it's certainly a different, creative and very elegant solution to a well-known
CS task.

[ted]: https://www.ted.com/talks/linus_torvalds_the_mind_behind_linux
