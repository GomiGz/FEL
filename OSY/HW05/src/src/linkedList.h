#include <stdbool.h>
#include "generics.h"

#define linkedListNode_t T_NAME(linkedListNode, TAlias)
#define linkedList_t T_NAME(linkedList, TAlias)
#define linkedList_(NAME) F_NAME(linkedList, TAlias, NAME)

typedef struct linkedListNode_t {
    struct linkedListNode_t * next;
    struct linkedListNode_t * previous;
    T item;
} linkedListNode_t;

// has to be castable to linkedListNode_t
typedef struct linkedList_t {
    linkedListNode_t * first;    // = linkedListNode_t.next
    linkedListNode_t * last;     // = linkedListNode_t.previous
} linkedList_t;

linkedList_t linkedList_(empty)();

void linkedList_(addFirst)(linkedList_t * list, T item);
T linkedList_(removeFirst)(linkedList_t * list);
bool linkedList_(tryRemoveFirst)(linkedList_t * list, T * out_item);

void linkedList_(addLast)(linkedList_t * list, T item);
T linkedList_(removeLast)(linkedList_t * list);
bool linkedList_(tryRemoveLast)(linkedList_t * list, T * out_item);

void linkedList_(clear)(linkedList_t * list);
