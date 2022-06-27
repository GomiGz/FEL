#include <stdlib.h>
#include "linkedList.h"

linkedList_t linkedList_(empty)()
{
    linkedList_t list;
    list.first = NULL;
    list.last = NULL;
    return list;
}

void linkNodes(linkedList_t * list, linkedListNode_t * node1, linkedListNode_t * node2)
{
    (node1 ? node1 : (linkedListNode_t*)list)->next = node2;
    (node2 ? node2 : (linkedListNode_t*)list)->previous = node1;
}

void linkedList_(addFirst)(linkedList_t * list, T item)
{
    linkedListNode_t * newNode = malloc(sizeof(linkedListNode_t));
    newNode->item = item;

    linkNodes(list, newNode, list->first);
    linkNodes(list, NULL, newNode);
}

T linkedList_(removeFirst)(linkedList_t * list)
{
    linkedListNode_t * oldNode = list->first;
    T item = oldNode->item;

    linkNodes(list, NULL, oldNode->next);
    free(oldNode);

    return item;
}

bool linkedList_(tryRemoveFirst)(linkedList_t * list, T * out_item)
{
    if (list->first == NULL)
        return false;

    *out_item = linkedList_(removeFirst)(list);
    return true;
}

void linkedList_(addLast)(linkedList_t * list, T item)
{
    linkedListNode_t * newNode = malloc(sizeof(linkedListNode_t));
    newNode->item = item;

    linkNodes(list, list->last, newNode);
    linkNodes(list, newNode, NULL);
}

T linkedList_(removeLast)(linkedList_t * list)
{
    linkedListNode_t * oldNode = list->last;
    T item = oldNode->item;
   
    linkNodes(list, oldNode->previous, NULL);
    free(oldNode);

    return item;
}

bool linkedList_(tryRemoveLast)(linkedList_t * list, T * out_item)
{
    if (list->last == NULL)
        return false;

    *out_item = linkedList_(removeLast)(list);
    return true;
}

void linkedList_(clear)(linkedList_t * list)
{
    for (linkedListNode_t * current = list->first, * next; current != NULL; current = next)
    {
        next = current->next;
        free(current);
    }

    linkNodes(list, NULL, NULL);
}
